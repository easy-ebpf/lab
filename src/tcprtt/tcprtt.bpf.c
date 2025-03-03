#include "vmlinux.h"
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>
#include <bpf/bpf_core_read.h>
#include <bpf/bpf_endian.h>

#include "tcprtt.h"

#include "bpf_tracing_net.h"

struct {
	__uint(type, BPF_MAP_TYPE_RINGBUF);
	__uint(max_entries, 256 * 1024);
} rb SEC(".maps");

SEC("fentry/tcp_rcv_established")
int BPF_PROG(tcp_rcv, struct sock *sk)
{
    // handle ipv4 only
    if (sk->__sk_common.skc_family != AF_INET)
        return 0;

    // FIXME: Reserve space in ring buffer `rb` for the event `e`
    // struct event *e = ... ;
    if (!e)
        return 0;

    e->saddr = sk->__sk_common.skc_rcv_saddr;
    e->daddr = sk->__sk_common.skc_daddr;
    e->sport = sk->__sk_common.skc_num;
    e->dport = bpf_ntohs(sk->__sk_common.skc_dport);

	// u32	srtt_us;	/* smoothed round trip time << 3 in usecs */
    struct tcp_sock *ts = tcp_sk(sk);
    e->rtt = BPF_CORE_READ(ts, srtt_us) >> 3;

    e->pid = bpf_get_current_pid_tgid() >> 32;
    bpf_get_current_comm(&e->comm, sizeof(e->comm));

    // FIXME: Submit your event `e` to ring buffer `rb`
    // ...
    return 0;
}

char LICENSE[] SEC("license") = "Dual BSD/GPL";
