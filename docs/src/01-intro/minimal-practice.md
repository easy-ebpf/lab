# Lab 1： 操作和觀察 minimal 和 verifier 

- 如果你的電腦還沒有設定好eBPF的編譯工具，請參考[這裡](https://github.com/easy-ebpf/practice_vm)設定好你的環境，並下載最新版本的範例程式碼:
    ```shell
    git clone https://github.com/easy-ebpf/lab
    ```
- 如果你在資電館三樓的電腦教室，桌面應該已經有vm映像檔可以直接匯入，不用再下載一次，但請一樣clone最新的程式碼。

- 練習1: 建置 minimal 和觀察程式運行。
    - 修改`src/minimal/minimal.bpf.c`中的TODO，將`STUDENT_ID`換成你自己的學號。
    - `make minimal` 建置執行檔，然後用 `sudo ./minimal` 執行
    - 打開新的terminal視窗/分頁，查看 eBPF 輸出訊息
        ```bash
        sudo cat /sys/kernel/debug/tracing/trace_pipe
        ```
        <img width="1569" height="387" alt="image" src="https://github.com/user-attachments/assets/a221ef32-59b7-4b17-9f7a-07d9794c7031" />


    - 查看當前附著的程式
        ```bash
        sudo bpftool perf show
        ```
        <img width="594" height="494" alt="image" src="https://github.com/user-attachments/assets/58a8b4e5-6f64-46ef-b9fb-b2d43df22787" />

        
<div class="warning">
    <strong>Note:</strong> 請截圖上方指令的執行畫面，並截圖附在報告中。 <strong>(15 points)</strong>
</div>

- 練習2:註解核心程式的 license 聲明，觀察 verifier 驗證不通過時的 log 提示並截圖。
<div class="warning">
    <strong>Note:</strong> 請截圖上方的執行失敗畫面，並截圖附在報告中。 <strong>(15 points)</strong>
</div>

- <strong>回答問題</strong>:
  - 1. eBPF相較於kernel module的差異在哪裡? <strong>(10 points)</strong>
  - 2. 參考[這裡](https://docs.kernel.org/bpf/bpf_licensing.html#using-bpf-programs-in-the-linux-kernel)，並回答為什麼註解掉license聲明就會導致不通過verifier的驗證? <strong>(5 points)</strong>
  - 3. 參考[這裡](https://docs.kernel.org/bpf/verifier.html)，並回答linux kernel使用verifier檢查了eBPF程式的哪些面向來確保eBPF程式的安全性? <strong>(5 points)</strong>
