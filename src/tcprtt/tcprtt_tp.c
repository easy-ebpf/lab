#include <signal.h>
#include <stdio.h>
#include <arpa/inet.h>

#include "tcprtt.h"
#include "tcprtt_tp.skel.h"

volatile sig_atomic_t cont = 1;

void handler(int _) {
    cont = 0;
}

int callback(void *_, void *data, size_t sz) {
    const struct event *e = data;
    
    char src[INET_ADDRSTRLEN];
    char dst[INET_ADDRSTRLEN];
    struct in_addr saddr = {.s_addr = e->saddr};
    struct in_addr daddr = {.s_addr = e->daddr};
    
    printf("%-7d %-16s %-16s:%-6d --> %-16s:%-6d %.2f\n", e->pid, e->comm,
        inet_ntop(AF_INET, &saddr, src, sizeof(src)), e->sport,
        inet_ntop(AF_INET, &daddr, dst, sizeof(dst)), e->dport,
        e->rtt / 1000.0);
    return 0;
}

int main() {
    signal(SIGINT, handler);

    struct tcprtt_tp_bpf *skel = tcprtt_tp_bpf__open_and_load();
    if (!skel) {
        perror("open skel");
        return 1;
    }

    int err = tcprtt_tp_bpf__attach(skel);
    if (err) {
        perror("attach skel");
        goto destroy;
    }

    // FIXME: Initialize a new ring buffer `rb`
    // struct ring_buffer *rb = ... ;
    if (!rb) {
        perror("ringbuf create");
        goto destroy;
    }

    printf("%-7s %-16s %-23s     %-23s %s\n", "PID", "COMM", "SRC", "DST", "LAT(ms)");
    while (cont) {
        // FIXME: Poll the ring buffer
        // err = ... ;
        if (err < 0 && err != -EINTR) {
            perror("ringbuf poll");
            goto destroy;
        }
    }

    return 0;

destroy:
    tcprtt_tp_bpf__destroy(skel);
    return 1;
}
