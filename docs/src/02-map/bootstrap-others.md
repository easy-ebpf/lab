# bootstrap核心程式的其他介紹

- 讀取系統核心的資料或記憶體，需要特別的函式。例如從系統取得目前行程的 `struct task_struct` 讀取 ppid (parent pid)

    ```c
    struct task_struct *task = (struct task_struct *)bpf_get_current_task();
    pid_t ppid = BPF_CORE_READ(task, real_parent, tgid);
    ```

    功能在普通 c 語言等同於

    ```c
    pid_t ppid = task->real_parent->tgid;
    ```

    已經宣告變數時，要傳入指標和大小，同系列的 api 有：
    
    ```c
    BPF_CORE_READ_INTO(&ppid, task, real_parent, tgid);
    bpf_core_read(&ppid, sizeof(ppid), real_parent);
    ```

- 從參數中讀取資料

    ```c
    SEC("tp/sched/sched_process_exec")
    int handle_exec(struct trace_event_raw_sched_process_exec *ctx)
    {
        ...
        fname_off = ctx->__data_loc_filename & 0xFFFF
        bpf_probe_read_str(&e->filename, sizeof(e->filename), (void *)ctx + fname_off);
    }
    ```

- 唯讀的全域變數定義須包含 const volatile ，跟 MMIO 操作的道理相同

    ```c
    const volatile unsigned long long min_duration_ns = 0;
    ```

