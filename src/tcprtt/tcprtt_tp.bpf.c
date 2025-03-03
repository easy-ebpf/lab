#include "vmlinux.h"
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>
#include <bpf/bpf_core_read.h>

#include "tcprtt.h"

#include "bpf_tracing_net.h"

struct {
	__uint(type, BPF_MAP_TYPE_RINGBUF);
	__uint(max_entries, 256 * 1024);
} rb SEC(".maps");

struct {
	__uint(type, BPF_MAP_TYPE_HASH);
	__uint(max_entries, 8 * 1024);
	__type(key, struct sock *);
	__type(value, u64);
} record SEC(".maps");

SEC("tracepoint/sock/inet_sock_set_state")
int handle_set_state(struct trace_event_raw_inet_sock_set_state *ctx)
{
    // handle ipv4 only
	if (ctx->family != AF_INET)
        return 0;

    u64 curr = bpf_ktime_get_ns();
	struct sock *sk = (struct sock *)ctx->skaddr;
    if (ctx->newstate == TCP_ESTABLISHED &&
		(ctx->oldstate == TCP_SYN_RECV || ctx->oldstate == TCP_SYN_SENT)) {
	// FIXME: Look up the element with key of `sk` in the hash map `record`
    	// u64 *prev = 
		if (prev) {
			// FIXME: Reserve space in the ring buffer `rb` for your event `e`
			// struct event *e = ... ;
			if (e) {
				bpf_core_read(&e->saddr, sizeof(e->saddr), &ctx->saddr); 
				bpf_core_read(&e->daddr, sizeof(e->daddr), &ctx->daddr); 
				e->sport = ctx->sport;
				e->dport = ctx->dport;

				e->rtt = (curr - *prev) / 1000;

				e->pid = bpf_get_current_pid_tgid() >> 32;
				bpf_get_current_comm(&e->comm, sizeof(e->comm));
				// FIXME: Submit your event `e` to ring buffer `rb`
				// ...
			}
		}
	}

	if (ctx->newstate == TCP_CLOSE) {
		// FIXME: delete the element of key `sk` from hash map `record`
		// ... ;
	} else {
		// FIXME: update the element of key `sk` from hash map `record`
		// ... ;
	}
	return 0;
}

char LICENSE[] SEC("license") = "Dual BSD/GPL";
