# eBPF 追蹤程式開發：tcprtt 和 tcprtt_tp

追蹤程式底下可以再分成 kprobe/kretprobe, fentry/fexit 和 tracepoint ：
- kprobe/kretprobe, fentry/fexit: 主要觀察核心中的函式的進入和退出，可以取得呼叫函式的引數（arguments）和回傳值。被追蹤的程式無須修改和重新編譯，屬於動態追蹤的技術。
- tracepoint: 核心程式碼設立和提供 tracepoint 以供觀測，屬於靜態追蹤。這套機制由 Linux 提供，屬於穩定的公開介面。

程式功能：輸出 TCP/IPV4 的延遲時間（round trip time），範例輸出如下：

```shell
$ sudo ./tcprtt_tp 
PID     COMM             SRC                         DST                     LAT(ms)
0       swapper/0        192.168.68.64   :53393  --> 20.42.65.94     :47873  196934.87
0       swapper/0        192.168.68.64   :52931  --> 172.217.163.36  :20480  6854.23
```

追蹤程式共通流程：
- 核心程式：搜集網路通訊資料，使用 ring buffer 傳遞到用戶
- 用戶程式：加載和附著核心程式，迴圈不斷接收 ring buffer 資料輸出到終端
