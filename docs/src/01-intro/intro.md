# eBPF 介紹

屬於核心（kernel）程式開發。與核心模組主要差別在**驗證器**（verifier）和 **map** 的設計。驗證器由 Linux kernel 提供，確認被加載的程式的正確性。map 則提供便利的通訊介面，讓 BPF 核心程式間和用戶程式能夠輕易的協作。

eBPF 程式的兩個目的：觀測追蹤（trace）的類別和改變核心行為的類別。

## 系統架構

系統元件圖:  
![System Block Diagram](https://hackmd.io/_uploads/ByPo075gJe.png)

- `hellp.py`、`hello()`：eBPF 程式，有核心和用戶兩個部分。課程中兩個部分都用 C 開發
- `trace pseudofile`：eBPF 程式核心和用戶溝通的機制，透過特殊檔案或 map
- `execve`、`小蜜蜂`：核心中的 tracepoint ，觸發附著的 eBPF 核心程式
- `Apps`：系統上所有應用程式，可能在系統呼叫時進入 tracepoint

屬於核心（kernel）程式開發。與核心模組主要差別在**驗證器**（verifier）和 **map** 的設計。驗證器由 Linux kernel 提供，確認被加載的程式的正確性。map 則提供便利的通訊介面，讓 BPF 核心程式間和用戶程式能夠輕易的協作。
