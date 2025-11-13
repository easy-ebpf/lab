# Lab 3-1: 開發tcprtt

- 練習1：使用 ring buffer 開發，完成 **tcprtt.c** 和 **tcprtt.bpf.c** 程式
- 練習2：編譯並執行tcprtt。觀察輸出的 ip、port 數值是否正確。 
    在本機可以透過命令列工具簡單製造 tcp 連線

    ```shell
    # server
    $ python3 -m http.server

    # client
    $ curl 0.0.0.0:8000
    ```
<div class="warning">
    <strong>Note:</strong> 請截圖上方的執行畫面並放入報告中。 <strong>(30 points)</strong>
</div>
