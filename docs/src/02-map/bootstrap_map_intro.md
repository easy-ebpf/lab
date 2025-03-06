# boostrap map 介紹

附著到核心執行和結束程式的 tracepoint ，測量執行時間。

## hash map

### bootstrap.bpf.c
宣告 eBPF map 的程式碼如下。根據種類，結構需要的成員各有不同， hash map 包含 `type`、`key`、`value`

```c
struct {
	__uint(type, BPF_MAP_TYPE_HASH);
	__uint(max_entries, 8192);
	__type(key, pid_t);
	__type(value, u64);
} exec_start SEC(".maps");
```

程式執行的時候，在 hash map 紀錄 pid 和時間

```c
SEC("tp/sched/sched_process_exec")
int handle_exec(struct trace_event_raw_sched_process_exec *ctx)
{
    ...
    bpf_map_update_elem(&exec_start, &pid, &ts, BPF_ANY);
}
```

程式結束的時候，利用 pid 查詢執行開始的時間，計算時長。將資訊透過 ring buffer 傳遞給用戶

```c
SEC("tp/sched/sched_process_exit")
int handle_exit(struct trace_event_raw_sched_process_template *ctx)
{
    ...
    /* if we recorded start of the process, calculate lifetime duration */
    start_ts = bpf_map_lookup_elem(&exec_start, &pid);
    if (start_ts)
        duration_ns = bpf_ktime_get_ns() - *start_ts;
    else if (min_duration_ns)
        return 0;
    bpf_map_delete_elem(&exec_start, &pid);
}
```


<details>
    <summary>核心態 map api 相關說明</summary>

```c
/*
 * bpf_map_lookup_elem
 *
 * 	Perform a lookup in *map* for an entry associated to *key*.
 *
 * Returns
 * 	Map value associated to *key*, or **NULL** if no entry was
 * 	found.
 */
static void *(* const bpf_map_lookup_elem)(void *map, const void *key) = (void *) 1;

/*
 * bpf_map_update_elem
 *
 * 	Add or update the value of the entry associated to *key* in
 * 	*map* with *value*. *flags* is one of:
 *
 * 	**BPF_NOEXIST**
 * 		The entry for *key* must not exist in the map.
 * 	**BPF_EXIST**
 * 		The entry for *key* must already exist in the map.
 * 	**BPF_ANY**
 * 		No condition on the existence of the entry for *key*.
 *
 * 	Flag value **BPF_NOEXIST** cannot be used for maps of types
 * 	**BPF_MAP_TYPE_ARRAY** or **BPF_MAP_TYPE_PERCPU_ARRAY**  (all
 * 	elements always exist), the helper would return an error.
 *
 * Returns
 * 	0 on success, or a negative error in case of failure.
 */
static long (* const bpf_map_update_elem)(void *map, const void *key, const void *value, __u64 flags) = (void *) 2;

/*
 * bpf_map_delete_elem
 *
 * 	Delete entry with *key* from *map*.
 *
 * Returns
 * 	0 on success, or a negative error in case of failure.
 */
static long (* const bpf_map_delete_elem)(void *map, const void *key) = (void *) 3;
```
</details>

<div class="warning">
核心及用戶 api 格式可能很像，但兩者是不同概念，一個使用系統呼叫，一個使用核心內的函式。
</div>

## ring buffer
### bootstrap.bpf.c

eBPF ring buffer map 的宣告

```c
struct {
	__uint(type, BPF_MAP_TYPE_RINGBUF);
	__uint(max_entries, 256 * 1024);
} rb SEC(".maps");
```

從核心傳遞資料到用戶程式，api 操作流程大致為：

```c
// 分配記憶體
struct event *e = bpf_ringbuf_reserve(&rb, sizeof(*e), 0);

// 設定 e 的數值...

// 發送資料
bpf_ringbuf_submit(e, 0);
```

### bootstrap.c
用戶接收部分則是：

```c
// * 獲取資源，用 skeleton maps 下的成員指定 map
// 在程式加載之後才能操作 map
// * callback "handle_event" 來處理資料
rb = ring_buffer__new(bpf_map__fd(skel->maps.rb), handle_event, NULL, NULL);
...

// 處理資料的迴圈
while (!exiting) {
    err = ring_buffer__poll(rb, 100 /* timeout, ms */);
}
...

// 釋放資源
ring_buffer__free(rb);
```
