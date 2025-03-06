# eBPF 程式開發介紹： minimal

## minimal 簡介
核心監測到用戶使用系統呼叫 `write()` 時輸出訊息，用戶附著完核心程式後會不斷使用 `write()` 

## 程式架構
- `minimal.bpf.c` ：核心程式
- `minimal.skel.h` : 由`minimal.bpf.o`產生而來的skeleton，以便userspace中的`minimal.c`調用。
- `minimal.c` ：用戶程式，負責加載和附著核心程式

## minimal.bpf.c

```c
// SPDX-License-Identifier: GPL-2.0 OR BSD-3-Clause
/* Copyright (c) 2020 Facebook */

// 這裡不符常規，用 "linux/bpf.h" 取代 "vmlinux.h"
// 需要定義在 bpf header 前面，因為 header 需要 kernel 類別
#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>

// SEC("license") 指定 symbol's section 
char LICENSE[] SEC("license") = "Dual BSD/GPL";

// supported since Linux 5.5
int my_pid = 0;

// define program type and attachment point
SEC("tp/syscalls/sys_enter_write")
int handle_tp(void *ctx)
{
    // return u64. [63: 32] is tgid, [31: 0] is pid
    // tgid is pid and pid is tid in kernel terminology
    int pid = bpf_get_current_pid_tgid() >> 32;

    if (pid != my_pid)
        return 0;

    // number of arguments is limited
    bpf_printk("BPF triggered from PID %d.\n", pid);

    return 0;
}
```

- `char LICENSE[] SEC("license")`：聲明程式的 license 。有些 bpf api 需要 license 才能使用，載入程式的時候 verifier 會驗證
- `my_pid`：核心和用戶也可以利用共用變數溝通

## minimal.skel.h

kernel 程式的抽象用結構體代表

```c
...

// 在呼叫 api 會使用 maps/progs/links
// bss/data/rodata 用戶可以直接存取
struct minimal_bpf {
	struct bpf_object_skeleton *skeleton;
	struct bpf_object *obj;
	struct {
		struct bpf_map *bss;
	} maps;
	struct {
		struct bpf_program *handle_tp;
	} progs;
	struct {
		struct bpf_link *handle_tp;
	} links;
	struct minimal_bpf__bss {
		int my_pid;
	} *bss;
};
```

skeleton 提供用戶的 api ，簡單的 eBPF 用戶程式可以不用自己呼叫 libbpf ，只需要使用 skeleton api

```c
// setup
static inline struct minimal_bpf *minimal_bpf__open(void) { ... }
static inline int minimal_bpf__load(struct minimal_bpf *obj) { ... }
static inline struct minimal_bpf *minimal_bpf__open_and_load(void) { ... }
static inline int minimal_bpf__attach(struct minimal_bpf *obj) { ... }

// teardown
static inline void minimal_bpf__destroy(struct minimal_bpf *obj) { ... }
static inline void minimal_bpf__detach(struct minimal_bpf *obj) { ... }
```

## minimal.c

用戶程式透過 `my_pid` 設定核心的過濾條件，只有 `minimal.c` 觸發的 `write()` 才會印 log

```c
int main(int argc, char **argv)
{
    struct minimal_bpf *skel = minimal_bpf__open();
    skel->bss->my_pid = getpid();   // set global variable
    err = minimal_bpf__load(skel);  // Load & verify BPF programs
    err = minimal_bpf__attach(skel);

    for (;;) {
        fprintf(stderr, ".");
        sleep(1);
    }

    minimal_bpf__destroy(skel);
    ...
}
```

- 共用變數 `my_pid` 必須在程式加載前設定
- tracepoints, kprobes 和特定類型的 eBPF 程式 libbpf 可以查看 `SEC()` 進行自動附著
