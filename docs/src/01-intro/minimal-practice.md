# Lab 1： 操作和觀察 minimal 和 verifier 

- 練習1:建置 minimal 和觀察程式運行

    - `make minimal` 建置執行檔，然後用 `sudo ./minimal` 執行
    - 查看 eBPF 輸出訊息

    ```shell
    $ sudo cat /sys/kernel/debug/tracing/trace_pipe
        <...>-3840345 [010] d... 3220701.101143: bpf_trace_printk: BPF triggered from PID 3840345.
        <...>-3840345 [010] d... 3220702.101265: bpf_trace_printk: BPF triggered from PID 3840345.
    ```

    - 查看當前附著的程式

    ```shell
    $ sudo bpftool perf show
    pid 232272  fd 17: prog_id 394  kprobe  func do_execve  offset 0
    pid 232272  fd 19: prog_id 396  tracepoint  sys_enter_execve
    ```

- 練習2:註解核心程式的 license 聲明，觀察 verifier 驗證不通過時的 log 提示
