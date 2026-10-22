
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#اگر_تعریف_نشده _NGX_CORE_H_INCLUDED_
#تعریف_کن _NGX_CORE_H_INCLUDED_


#ضمن <ngx_config.h>


تعريفنوع بنية ngx_module_s          ngx_module_t;
تعريفنوع بنية ngx_conf_s            ngx_conf_t;
تعريفنوع بنية ngx_چرخه_s           ngx_چرخه_t;
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
تعريفنوع بنية ngx_quic_جریان_s     ngx_quic_جریان_t;
تعريفنوع بنية ngx_ssl_connection_s  ngx_ssl_connection_t;
تعريفنوع بنية ngx_udp_connection_s  ngx_udp_connection_t;

تعريفنوع فراغ (*ngx_event_handler_pt)(ngx_event_t *ev);
تعريفنوع فراغ (*ngx_connection_handler_pt)(ngx_connection_t *c);


#تعریف_کن  NGX_OK          0
#تعریف_کن  NGX_ERROR      -1
#تعریف_کن  NGX_AGAIN      -2
#تعریف_کن  NGX_BUSY       -3
#تعریف_کن  NGX_انجامشده       -4
#تعریف_کن  NGX_DECLINED   -5
#تعریف_کن  NGX_ABORT      -6


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
#إذا (NGX_PCRE)
#ضمن <ngx_regex.h>
#پایان_اگر
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
#إذا (NGX_OPENSSL)
#ضمن <ngx_event_openssl.h>
#إذا (NGX_QUIC)
#ضمن <ngx_event_quic.h>
#پایان_اگر
#پایان_اگر
#ضمن <ngx_process_cycle.h>
#إذا (NGX_CONTROL_API)
#ضمن <ngx_control.h>
#پایان_اگر
#ضمن <ngx_conf_file.h>
#ضمن <ngx_module.h>
#ضمن <ngx_open_file_cache.h>
#ضمن <ngx_os.h>
#ضمن <ngx_connection.h>
#ضمن <ngx_syslog.h>
#ضمن <ngx_proxy_protocol.h>
#إذا (NGX_HAVE_BPF)
#ضمن <ngx_bpf.h>
#پایان_اگر


#تعریف_کن LF     (u_char) '\n'
#تعریف_کن CR     (u_char) '\r'
#تعریف_کن CRLF   "\r\n"


#تعریف_کن ngx_abs(مقدار)       (((مقدار) >= 0) ? (مقدار) : - (مقدار))
#تعریف_کن ngx_max(val1, val2)  ((val1 < val2) ? (val2) : (val1))
#تعریف_کن ngx_min(val1, val2)  ((val1 > val2) ? (val2) : (val1))

فراغ ngx_cpuinfo(فراغ);

#إذا (NGX_HAVE_OPENAT)
#تعریف_کن NGX_DISABLE_SYMLINKS_OFF        0
#تعریف_کن NGX_DISABLE_SYMLINKS_ON         1
#تعریف_کن NGX_DISABLE_SYMLINKS_NOTOWNER   2
#پایان_اگر

#پایان_اگر /* _NGX_CORE_H_INCLUDED_ */
