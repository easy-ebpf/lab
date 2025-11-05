# Lab 1： 操作和觀察 minimal 和 verifier 

- 如果你的電腦還沒有設定好eBPF的編譯工具，請參考[這裡](https://github.com/easy-ebpf/practice_vm)設定好你的環境，並下載範例程式碼:
    ```shell
    git clone https://github.com/easy-ebpf/lab
    ```
- 如果你在資電館三樓的電腦教室，桌面應該已經有vm映像檔可以直接匯入，不用再下載一次。

- 練習1: 建置 minimal 和觀察程式運行

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
