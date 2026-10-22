
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_CORE_H_INCLUDED_
#define _NGX_CORE_H_INCLUDED_


#અન્તર્ભાવ <ngx_config.h>


પ્રકારવ્યાખ્યા સંરચના ngx_module_s          ngx_module_t;
પ્રકારવ્યાખ્યા સંરચના ngx_conf_s            ngx_conf_t;
પ્રકારવ્યાખ્યા સંરચના ngx_cycle_s           ngx_cycle_t;
પ્રકારવ્યાખ્યા સંરચના ngx_pool_s            ngx_pool_t;
પ્રકારવ્યાખ્યા સંરચના ngx_chain_s           ngx_chain_t;
પ્રકારવ્યાખ્યા સંરચના ngx_log_s             ngx_log_t;
પ્રકારવ્યાખ્યા સંરચના ngx_open_file_s       ngx_open_file_t;
પ્રકારવ્યાખ્યા સંરચના ngx_command_s         ngx_command_t;
પ્રકારવ્યાખ્યા સંરચના ngx_file_s            ngx_file_t;
પ્રકારવ્યાખ્યા સંરચના ngx_event_s           ngx_event_t;
પ્રકારવ્યાખ્યા સંરચના ngx_event_aio_s       ngx_event_aio_t;
પ્રકારવ્યાખ્યા સંરચના ngx_connection_s      ngx_connection_t;
પ્રકારવ્યાખ્યા સંરચના ngx_thread_task_s     ngx_thread_task_t;
પ્રકારવ્યાખ્યા સંરચના ngx_ssl_s             ngx_ssl_t;
પ્રકારવ્યાખ્યા સંરચના ngx_ssl_cache_s       ngx_ssl_cache_t;
પ્રકારવ્યાખ્યા સંરચના ngx_proxy_protocol_s  ngx_proxy_protocol_t;
પ્રકારવ્યાખ્યા સંરચના ngx_quic_stream_s     ngx_quic_stream_t;
પ્રકારવ્યાખ્યા સંરચના ngx_ssl_connection_s  ngx_ssl_connection_t;
પ્રકારવ્યાખ્યા સંરચના ngx_udp_connection_s  ngx_udp_connection_t;

પ્રકારવ્યાખ્યા શૂન્ય (*ngx_event_handler_pt)(ngx_event_t *ev);
પ્રકારવ્યાખ્યા શૂન્ય (*ngx_connection_handler_pt)(ngx_connection_t *c);


#define  NGX_OK          0
#define  NGX_ERROR      -1
#define  NGX_AGAIN      -2
#define  NGX_BUSY       -3
#define  NGX_DONE       -4
#define  NGX_DECLINED   -5
#define  NGX_ABORT      -6


#અન્તર્ભાવ <ngx_errno.h>
#અન્તર્ભાવ <ngx_atomic.h>
#અન્તર્ભાવ <ngx_thread.h>
#અન્તર્ભાવ <ngx_rbtree.h>
#અન્તર્ભાવ <ngx_time.h>
#અન્તર્ભાવ <ngx_socket.h>
#અન્તર્ભાવ <ngx_string.h>
#અન્તર્ભાવ <ngx_files.h>
#અન્તર્ભાવ <ngx_shmem.h>
#અન્તર્ભાવ <ngx_process.h>
#અન્તર્ભાવ <ngx_user.h>
#અન્તર્ભાવ <ngx_dlopen.h>
#અન્તર્ભાવ <ngx_parse.h>
#અન્તર્ભાવ <ngx_parse_time.h>
#અન્તર્ભાવ <ngx_log.h>
#અન્તર્ભાવ <ngx_alloc.h>
#અન્તર્ભાવ <ngx_palloc.h>
#અન્તર્ભાવ <ngx_buf.h>
#અન્તર્ભાવ <ngx_queue.h>
#અન્તર્ભાવ <ngx_array.h>
#અન્તર્ભાવ <ngx_list.h>
#અન્તર્ભાવ <ngx_hash.h>
#અન્તર્ભાવ <ngx_file.h>
#અન્તર્ભાવ <ngx_crc.h>
#અન્તર્ભાવ <ngx_crc32.h>
#અન્તર્ભાવ <ngx_murmurhash.h>
#અન્તર્ભાવ <ngx_siphash.h>
#જો (NGX_PCRE)
#અન્તર્ભાવ <ngx_regex.h>
#endif
#અન્તર્ભાવ <ngx_radix_tree.h>
#અન્તર્ભાવ <ngx_times.h>
#અન્તર્ભાવ <ngx_rwlock.h>
#અન્તર્ભાવ <ngx_shmtx.h>
#અન્તર્ભાવ <ngx_data.h>
#અન્તર્ભાવ <ngx_json.h>
#અન્તર્ભાવ <ngx_slab.h>
#અન્તર્ભાવ <ngx_inet.h>
#અન્તર્ભાવ <ngx_cycle.h>
#અન્તર્ભાવ <ngx_resolver.h>
#જો (NGX_OPENSSL)
#અન્તર્ભાવ <ngx_event_openssl.h>
#જો (NGX_QUIC)
#અન્તર્ભાવ <ngx_event_quic.h>
#endif
#endif
#અન્તર્ભાવ <ngx_process_cycle.h>
#જો (NGX_CONTROL_API)
#અન્તર્ભાવ <ngx_control.h>
#endif
#અન્તર્ભાવ <ngx_conf_file.h>
#અન્તર્ભાવ <ngx_module.h>
#અન્તર્ભાવ <ngx_open_file_cache.h>
#અન્તર્ભાવ <ngx_os.h>
#અન્તર્ભાવ <ngx_connection.h>
#અન્તર્ભાવ <ngx_syslog.h>
#અન્તર્ભાવ <ngx_proxy_protocol.h>
#જો (NGX_HAVE_BPF)
#અન્તર્ભાવ <ngx_bpf.h>
#endif


#define LF     (u_char) '\n'
#define CR     (u_char) '\r'
#define CRLF   "\r\n"


#define ngx_abs(કિંમત)       (((કિંમત) >= 0) ? (કિંમત) : - (કિંમત))
#define ngx_max(val1, val2)  ((val1 < val2) ? (val2) : (val1))
#define ngx_min(val1, val2)  ((val1 > val2) ? (val2) : (val1))

શૂન્ય ngx_cpuinfo(શૂન્ય);

#જો (NGX_HAVE_OPENAT)
#define NGX_DISABLE_SYMLINKS_OFF        0
#define NGX_DISABLE_SYMLINKS_ON         1
#define NGX_DISABLE_SYMLINKS_NOTOWNER   2
#endif

#endif /* _NGX_CORE_H_INCLUDED_ */
