
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_CORE_H_INCLUDED_
#define _NGX_CORE_H_INCLUDED_


#شامل <ngx_config.h>


تعریفنوع ساختار ngx_module_s          ngx_module_t;
تعریفنوع ساختار ngx_conf_s            ngx_conf_t;
تعریفنوع ساختار ngx_cycle_s           ngx_cycle_t;
تعریفنوع ساختار ngx_pool_s            ngx_pool_t;
تعریفنوع ساختار ngx_chain_s           ngx_chain_t;
تعریفنوع ساختار ngx_log_s             ngx_log_t;
تعریفنوع ساختار ngx_open_file_s       ngx_open_file_t;
تعریفنوع ساختار ngx_command_s         ngx_command_t;
تعریفنوع ساختار ngx_file_s            ngx_file_t;
تعریفنوع ساختار ngx_event_s           ngx_event_t;
تعریفنوع ساختار ngx_event_aio_s       ngx_event_aio_t;
تعریفنوع ساختار ngx_connection_s      ngx_connection_t;
تعریفنوع ساختار ngx_thread_task_s     ngx_thread_task_t;
تعریفنوع ساختار ngx_ssl_s             ngx_ssl_t;
تعریفنوع ساختار ngx_ssl_cache_s       ngx_ssl_cache_t;
تعریفنوع ساختار ngx_proxy_protocol_s  ngx_proxy_protocol_t;
تعریفنوع ساختار ngx_quic_stream_s     ngx_quic_stream_t;
تعریفنوع ساختار ngx_ssl_connection_s  ngx_ssl_connection_t;
تعریفنوع ساختار ngx_udp_connection_s  ngx_udp_connection_t;

تعریفنوع تهی (*ngx_event_handler_pt)(ngx_event_t *ev);
تعریفنوع تهی (*ngx_connection_handler_pt)(ngx_connection_t *c);


#define  NGX_OK          0
#define  NGX_ERROR      -1
#define  NGX_AGAIN      -2
#define  NGX_BUSY       -3
#define  NGX_DONE       -4
#define  NGX_DECLINED   -5
#define  NGX_ABORT      -6


#شامل <ngx_errno.h>
#شامل <ngx_atomic.h>
#شامل <ngx_thread.h>
#شامل <ngx_rbtree.h>
#شامل <ngx_time.h>
#شامل <ngx_socket.h>
#شامل <ngx_string.h>
#شامل <ngx_files.h>
#شامل <ngx_shmem.h>
#شامل <ngx_process.h>
#شامل <ngx_user.h>
#شامل <ngx_dlopen.h>
#شامل <ngx_parse.h>
#شامل <ngx_parse_time.h>
#شامل <ngx_log.h>
#شامل <ngx_alloc.h>
#شامل <ngx_palloc.h>
#شامل <ngx_buf.h>
#شامل <ngx_queue.h>
#شامل <ngx_array.h>
#شامل <ngx_list.h>
#شامل <ngx_hash.h>
#شامل <ngx_file.h>
#شامل <ngx_crc.h>
#شامل <ngx_crc32.h>
#شامل <ngx_murmurhash.h>
#شامل <ngx_siphash.h>
#اگر (NGX_PCRE)
#شامل <ngx_regex.h>
#endif
#شامل <ngx_radix_tree.h>
#شامل <ngx_times.h>
#شامل <ngx_rwlock.h>
#شامل <ngx_shmtx.h>
#شامل <ngx_data.h>
#شامل <ngx_json.h>
#شامل <ngx_slab.h>
#شامل <ngx_inet.h>
#شامل <ngx_cycle.h>
#شامل <ngx_resolver.h>
#اگر (NGX_OPENSSL)
#شامل <ngx_event_openssl.h>
#اگر (NGX_QUIC)
#شامل <ngx_event_quic.h>
#endif
#endif
#شامل <ngx_process_cycle.h>
#اگر (NGX_CONTROL_API)
#شامل <ngx_control.h>
#endif
#شامل <ngx_conf_file.h>
#شامل <ngx_module.h>
#شامل <ngx_open_file_cache.h>
#شامل <ngx_os.h>
#شامل <ngx_connection.h>
#شامل <ngx_syslog.h>
#شامل <ngx_proxy_protocol.h>
#اگر (NGX_HAVE_BPF)
#شامل <ngx_bpf.h>
#endif


#define LF     (u_char) '\n'
#define CR     (u_char) '\r'
#define CRLF   "\r\n"


#define ngx_abs(مقدار)       (((مقدار) >= 0) ? (مقدار) : - (مقدار))
#define ngx_max(val1, val2)  ((val1 < val2) ? (val2) : (val1))
#define ngx_min(val1, val2)  ((val1 > val2) ? (val2) : (val1))

تهی ngx_cpuinfo(تهی);

#اگر (NGX_HAVE_OPENAT)
#define NGX_DISABLE_SYMLINKS_OFF        0
#define NGX_DISABLE_SYMLINKS_ON         1
#define NGX_DISABLE_SYMLINKS_NOTOWNER   2
#endif

#endif /* _NGX_CORE_H_INCLUDED_ */
