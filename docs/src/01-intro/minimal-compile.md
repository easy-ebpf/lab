# eBPF 編譯流程
![截圖 2024-11-08 下午3.50.40](https://hackmd.io/_uploads/SkVxXBs-yg.png)

- `minimal.bpf.c` ：核心程式
  - Userspace程式調用之前會先用`bpftool`生成 `minimal.skel.h` skeleton 檔間接調用/載入該eBPF程式。
- `minimal.c` ：用戶程式，加載和附著核心程式
- `vmlinux.h` ：具有 kernel 內所有類別的定義
  - 由`bpftool btf dump file /sys/kernel/btf/vmlinux format c > vmlinux.h`生成。

## makefile

makefile 是自動化建置工具，透過撰寫檔名為 makefile 或 Makefile 的文字檔，定義檔案的相依和生成規則，將程式碼編譯成執行檔。使用時，在 makefile 檔案所在的目錄執行 `make <目標文件>` 生成特定目標， `make` 則是生成第一個規則的目標。

### makefile 規則撰寫格式

```makefile
# 檔名為 makefile 或 Makefile

目標文件 ...：依賴項目 ...  # 多個項目透過空格分開
    終端命令  # 開頭一定要是 TAB
    ...練習
```

特殊符號說明：
- `$^`： 所有依賴項目
- `$<`： 第一個依賴項目
- `$@`： 目標文件

### minimal makefile 示範

```makefile
# generate vmlinux header
vmlinux.h:
	bpftool btf dump file /sys/kernel/btf/vmlinux format c > vmlinux.h 

# build bpf kernel object file
minimal.bpf.o: minimal.bpf.c vmlinux.h
	clang -g -O2 -target bpf -c $< -o $@

# generate skeleton
minimal.skel.h: minimal.bpf.o
	bpftool gen skeleton $< > $@

# build app
minimal: minimal.c minimal.skel.h
	clang -g minimal.c -lbpf -lelf -lz -o $@

```

- `-O2`, `-target bpf` 是必要的選項
- `-lbpf -lelf -lz` 執行檔動態連結函式庫
- 執行檔不用連結核心目標檔
