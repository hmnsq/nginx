
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_CORE_H_INCLUDED_
#define _NGX_CORE_H_INCLUDED_


#ضمن <ngx_config.h>


تعريفنوع بنية ngx_module_s          ngx_module_t;
تعريفنوع بنية ngx_conf_s            ngx_conf_t;
تعريفنوع بنية ngx_cycle_s           ngx_cycle_t;
تعريفنوع بنية ngx_pool_s            ngx_pool_t;
تعريفنوع بنية ngx_chain_s           ngx_chain_t;
تعريفنوع بنية ngx_log_s             ngx_log_t;
تعريفنوع بنية ngx_open_file_s       ngx_open_file_t;
تعريفنوع بنية ngx_command_s         ngx_command_t;
تعريفنوع بنية ngx_file_s            ngx_file_t;
تعريفنوع بنية ngx_event_s           ngx_event_t;
تعريفنوع بنية ngx_event_aio_s       ngx_event_aio_t;
تعريفنوع بنية ngx_connection_s      ngx_connection_t;
تعريفنوع بنية ngx_thread_task_s     ngx_thread_task_t;
تعريفنوع بنية ngx_ssl_s             ngx_ssl_t;
تعريفنوع بنية ngx_ssl_cache_s       ngx_ssl_cache_t;
تعريفنوع بنية ngx_proxy_protocol_s  ngx_proxy_protocol_t;
تعريفنوع بنية ngx_quic_stream_s     ngx_quic_stream_t;
تعريفنوع بنية ngx_ssl_connection_s  ngx_ssl_connection_t;
تعريفنوع بنية ngx_udp_connection_s  ngx_udp_connection_t;

تعريفنوع فراغ (*ngx_event_handler_pt)(ngx_event_t *ev);
تعريفنوع فراغ (*ngx_connection_handler_pt)(ngx_connection_t *c);


#define  NGX_OK          0
#define  NGX_ERROR      -1
#define  NGX_AGAIN      -2
#define  NGX_BUSY       -3
#define  NGX_DONE       -4
#define  NGX_DECLINED   -5
#define  NGX_ABORT      -6


#ضمن <ngx_errno.h>
#ضمن <ngx_atomic.h>
#ضمن <ngx_thread.h>
#ضمن <ngx_rbtree.h>
#ضمن <ngx_time.h>
#ضمن <ngx_socket.h>
#ضمن <ngx_string.h>
#ضمن <ngx_files.h>
#ضمن <ngx_shmem.h>
#ضمن <ngx_process.h>
#ضمن <ngx_user.h>
#ضمن <ngx_dlopen.h>
#ضمن <ngx_parse.h>
#ضمن <ngx_parse_time.h>
#ضمن <ngx_log.h>
#ضمن <ngx_alloc.h>
#ضمن <ngx_palloc.h>
#ضمن <ngx_buf.h>
#ضمن <ngx_queue.h>
#ضمن <ngx_array.h>
#ضمن <ngx_list.h>
#ضمن <ngx_hash.h>
#ضمن <ngx_file.h>
#ضمن <ngx_crc.h>
#ضمن <ngx_crc32.h>
#ضمن <ngx_murmurhash.h>
#ضمن <ngx_siphash.h>
#אם (NGX_PCRE)
#ضمن <ngx_regex.h>
#endif
#ضمن <ngx_radix_tree.h>
#ضمن <ngx_times.h>
#ضمن <ngx_rwlock.h>
#ضمن <ngx_shmtx.h>
#ضمن <ngx_data.h>
#ضمن <ngx_json.h>
#ضمن <ngx_slab.h>
#ضمن <ngx_inet.h>
#ضمن <ngx_cycle.h>
#ضمن <ngx_resolver.h>
#אם (NGX_OPENSSL)
#ضمن <ngx_event_openssl.h>
#אם (NGX_QUIC)
#ضمن <ngx_event_quic.h>
#endif
#endif
#ضمن <ngx_process_cycle.h>
#אם (NGX_CONTROL_API)
#ضمن <ngx_control.h>
#endif
#ضمن <ngx_conf_file.h>
#ضمن <ngx_module.h>
#ضمن <ngx_open_file_cache.h>
#ضمن <ngx_os.h>
#ضمن <ngx_connection.h>
#ضمن <ngx_syslog.h>
#ضمن <ngx_proxy_protocol.h>
#אם (NGX_HAVE_BPF)
#ضمن <ngx_bpf.h>
#endif


#define LF     (u_char) '\n'
#define CR     (u_char) '\r'
#define CRLF   "\r\n"


#define ngx_abs(value)       (((value) >= 0) ? (value) : - (value))
#define ngx_max(val1, val2)  ((val1 < val2) ? (val2) : (val1))
#define ngx_min(val1, val2)  ((val1 > val2) ? (val2) : (val1))

فراغ ngx_cpuinfo(فراغ);

#אם (NGX_HAVE_OPENAT)
#define NGX_DISABLE_SYMLINKS_OFF        0
#define NGX_DISABLE_SYMLINKS_ON         1
#define NGX_DISABLE_SYMLINKS_NOTOWNER   2
#endif

#endif /* _NGX_CORE_H_INCLUDED_ */
