# Lab 2：操作和觀察 bootstrap

- 練習1：
    - 參考minimal的範例，撰寫 makefile 並編譯生成 bootstrap 執行檔
    - 運行 `sudo ./bootstrap` 並確認輸出訊息，如果輸出沒有偵測到process，則需要在系統中執行其它程式來觸發訊息的輸出。
<div class="warning">
    <strong>Note:</strong> 請截圖上方bootstrap程式的執行畫面，並截圖附在報告中。 <strong>(30 points)</strong>
</div>

- 回答問題:
    - 1. eBPF中的ring buffer跟hash map的差異在哪裡? <strong>(10 points)</strong>
    - 2. 追蹤bootstrap的程式碼，並回答它是如何在程式中使用ring buffer跟hash map的? <strong>(10 points)</strong>

- 其他：可以先熟悉一下 hash map 和 ring buffer 程式 api ， Lab 3 要開發 map 的程式
