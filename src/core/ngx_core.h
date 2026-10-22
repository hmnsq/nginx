
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_CORE_H_INCLUDED_
#define _NGX_CORE_H_INCLUDED_


#அந்தர்பாவ <ngx_config.h>


வகைவரை கட்டமைப்பு ngx_module_s          ngx_module_t;
வகைவரை கட்டமைப்பு ngx_conf_s            ngx_conf_t;
வகைவரை கட்டமைப்பு ngx_cycle_s           ngx_cycle_t;
வகைவரை கட்டமைப்பு ngx_pool_s            ngx_pool_t;
வகைவரை கட்டமைப்பு ngx_chain_s           ngx_chain_t;
வகைவரை கட்டமைப்பு ngx_log_s             ngx_log_t;
வகைவரை கட்டமைப்பு ngx_open_file_s       ngx_open_file_t;
வகைவரை கட்டமைப்பு ngx_command_s         ngx_command_t;
வகைவரை கட்டமைப்பு ngx_file_s            ngx_file_t;
வகைவரை கட்டமைப்பு ngx_event_s           ngx_event_t;
வகைவரை கட்டமைப்பு ngx_event_aio_s       ngx_event_aio_t;
வகைவரை கட்டமைப்பு ngx_connection_s      ngx_connection_t;
வகைவரை கட்டமைப்பு ngx_thread_task_s     ngx_thread_task_t;
வகைவரை கட்டமைப்பு ngx_ssl_s             ngx_ssl_t;
வகைவரை கட்டமைப்பு ngx_ssl_cache_s       ngx_ssl_cache_t;
வகைவரை கட்டமைப்பு ngx_proxy_protocol_s  ngx_proxy_protocol_t;
வகைவரை கட்டமைப்பு ngx_quic_stream_s     ngx_quic_stream_t;
வகைவரை கட்டமைப்பு ngx_ssl_connection_s  ngx_ssl_connection_t;
வகைவரை கட்டமைப்பு ngx_udp_connection_s  ngx_udp_connection_t;

வகைவரை வெறுமை (*ngx_event_handler_pt)(ngx_event_t *ev);
வகைவரை வெறுமை (*ngx_connection_handler_pt)(ngx_connection_t *c);


#define  NGX_OK          0
#define  NGX_ERROR      -1
#define  NGX_AGAIN      -2
#define  NGX_BUSY       -3
#define  NGX_DONE       -4
#define  NGX_DECLINED   -5
#define  NGX_ABORT      -6


#அந்தர்பாவ <ngx_errno.h>
#அந்தர்பாவ <ngx_atomic.h>
#அந்தர்பாவ <ngx_thread.h>
#அந்தர்பாவ <ngx_rbtree.h>
#அந்தர்பாவ <ngx_time.h>
#அந்தர்பாவ <ngx_socket.h>
#அந்தர்பாவ <ngx_string.h>
#அந்தர்பாவ <ngx_files.h>
#அந்தர்பாவ <ngx_shmem.h>
#அந்தர்பாவ <ngx_process.h>
#அந்தர்பாவ <ngx_user.h>
#அந்தர்பாவ <ngx_dlopen.h>
#அந்தர்பாவ <ngx_parse.h>
#அந்தர்பாவ <ngx_parse_time.h>
#அந்தர்பாவ <ngx_log.h>
#அந்தர்பாவ <ngx_alloc.h>
#அந்தர்பாவ <ngx_palloc.h>
#அந்தர்பாவ <ngx_buf.h>
#அந்தர்பாவ <ngx_queue.h>
#அந்தர்பாவ <ngx_array.h>
#அந்தர்பாவ <ngx_list.h>
#அந்தர்பாவ <ngx_hash.h>
#அந்தர்பாவ <ngx_file.h>
#அந்தர்பாவ <ngx_crc.h>
#அந்தர்பாவ <ngx_crc32.h>
#அந்தர்பாவ <ngx_murmurhash.h>
#அந்தர்பாவ <ngx_siphash.h>
#எனில் (NGX_PCRE)
#அந்தர்பாவ <ngx_regex.h>
#endif
#அந்தர்பாவ <ngx_radix_tree.h>
#அந்தர்பாவ <ngx_times.h>
#அந்தர்பாவ <ngx_rwlock.h>
#அந்தர்பாவ <ngx_shmtx.h>
#அந்தர்பாவ <ngx_data.h>
#அந்தர்பாவ <ngx_json.h>
#அந்தர்பாவ <ngx_slab.h>
#அந்தர்பாவ <ngx_inet.h>
#அந்தர்பாவ <ngx_cycle.h>
#அந்தர்பாவ <ngx_resolver.h>
#எனில் (NGX_OPENSSL)
#அந்தர்பாவ <ngx_event_openssl.h>
#எனில் (NGX_QUIC)
#அந்தர்பாவ <ngx_event_quic.h>
#endif
#endif
#அந்தர்பாவ <ngx_process_cycle.h>
#எனில் (NGX_CONTROL_API)
#அந்தர்பாவ <ngx_control.h>
#endif
#அந்தர்பாவ <ngx_conf_file.h>
#அந்தர்பாவ <ngx_module.h>
#அந்தர்பாவ <ngx_open_file_cache.h>
#அந்தர்பாவ <ngx_os.h>
#அந்தர்பாவ <ngx_connection.h>
#அந்தர்பாவ <ngx_syslog.h>
#அந்தர்பாவ <ngx_proxy_protocol.h>
#எனில் (NGX_HAVE_BPF)
#அந்தர்பாவ <ngx_bpf.h>
#endif


#define LF     (u_char) '\n'
#define CR     (u_char) '\r'
#define CRLF   "\r\n"


#define ngx_abs(மதிப்பு)       (((மதிப்பு) >= 0) ? (மதிப்பு) : - (மதிப்பு))
#define ngx_max(val1, val2)  ((val1 < val2) ? (val2) : (val1))
#define ngx_min(val1, val2)  ((val1 > val2) ? (val2) : (val1))

வெறுமை ngx_cpuinfo(வெறுமை);

#எனில் (NGX_HAVE_OPENAT)
#define NGX_DISABLE_SYMLINKS_OFF        0
#define NGX_DISABLE_SYMLINKS_ON         1
#define NGX_DISABLE_SYMLINKS_NOTOWNER   2
#endif

#endif /* _NGX_CORE_H_INCLUDED_ */
