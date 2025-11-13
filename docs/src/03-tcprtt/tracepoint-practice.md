# Lab 3-2: 開發tcprtt_tp

- 練習1：使用 hash map 開發，完成 **tcprtt_tp.bpf.c** 程式
- 練習2：編譯並執行後，確認輸出的 ip、port 數值正確。
<div class="warning">
    <strong>Note:</strong> 請截圖上方的執行畫面並放入報告中。 <strong>(30 points)</strong>
</div>

> 使用 Lab 3-1 的本機命令測試時，COMM 只會出現 curl ，但 ip、port 應正確顯示

- 回答問題:
    - 1. 請簡要說明fentry跟tracepoint的差異在哪裡。 <strong>(20 points)</strong>
    - 2. fentry跟tracepoint的差異是否有帶來tcprtt程式表現上的差異? 為什麼? <strong>(20 points)</strong>
