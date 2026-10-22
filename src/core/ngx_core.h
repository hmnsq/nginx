
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#اگر_غیر_تعریف_شدہ _NGX_CORE_H_INCLUDED_
#تعریف_کرو _NGX_CORE_H_INCLUDED_


#شامل <ngx_config.h>


قسمتعریف ساخت ngx_module_s          ngx_module_t;
قسمتعریف ساخت ngx_conf_s            ngx_conf_t;
قسمتعریف ساخت ngx_چکر_s           ngx_چکر_t;
قسمتعریف ساخت ngx_pool_s            ngx_pool_t;
قسمتعریف ساخت ngx_chain_s           ngx_chain_t;
قسمتعریف ساخت ngx_log_s             ngx_log_t;
قسمتعریف ساخت ngx_open_file_s       ngx_open_file_t;
قسمتعریف ساخت ngx_command_s         ngx_command_t;
قسمتعریف ساخت ngx_file_s            ngx_file_t;
قسمتعریف ساخت ngx_event_s           ngx_event_t;
قسمتعریف ساخت ngx_event_aio_s       ngx_event_aio_t;
قسمتعریف ساخت ngx_connection_s      ngx_connection_t;
قسمتعریف ساخت ngx_thread_task_s     ngx_thread_task_t;
قسمتعریف ساخت ngx_ssl_s             ngx_ssl_t;
قسمتعریف ساخت ngx_ssl_cache_s       ngx_ssl_cache_t;
قسمتعریف ساخت ngx_proxy_protocol_s  ngx_proxy_protocol_t;
قسمتعریف ساخت ngx_quic_دھارا_s     ngx_quic_دھارا_t;
قسمتعریف ساخت ngx_ssl_connection_s  ngx_ssl_connection_t;
قسمتعریف ساخت ngx_udp_connection_s  ngx_udp_connection_t;

قسمتعریف خالی (*ngx_event_handler_pt)(ngx_event_t *ev);
قسمتعریف خالی (*ngx_connection_handler_pt)(ngx_connection_t *c);


#تعریف_کرو  NGX_OK          0
#تعریف_کرو  NGX_ERROR      -1
#تعریف_کرو  NGX_AGAIN      -2
#تعریف_کرو  NGX_BUSY       -3
#تعریف_کرو  NGX_مکمل       -4
#تعریف_کرو  NGX_DECLINED   -5
#تعریف_کرو  NGX_ABORT      -6


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
#اگر_ختم
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
#اگر_ختم
#اگر_ختم
#شامل <ngx_process_cycle.h>
#اگر (NGX_CONTROL_API)
#شامل <ngx_control.h>
#اگر_ختم
#شامل <ngx_conf_file.h>
#شامل <ngx_module.h>
#شامل <ngx_open_file_cache.h>
#شامل <ngx_os.h>
#شامل <ngx_connection.h>
#شامل <ngx_syslog.h>
#شامل <ngx_proxy_protocol.h>
#اگر (NGX_HAVE_BPF)
#شامل <ngx_bpf.h>
#اگر_ختم


#تعریف_کرو LF     (u_char) '\n'
#تعریف_کرو CR     (u_char) '\r'
#تعریف_کرو CRLF   "\r\n"


#تعریف_کرو ngx_abs(قدر)       (((قدر) >= 0) ? (قدر) : - (قدر))
#تعریف_کرو ngx_max(val1, val2)  ((val1 < val2) ? (val2) : (val1))
#تعریف_کرو ngx_min(val1, val2)  ((val1 > val2) ? (val2) : (val1))

خالی ngx_cpuinfo(خالی);

#اگر (NGX_HAVE_OPENAT)
#تعریف_کرو NGX_DISABLE_SYMLINKS_OFF        0
#تعریف_کرو NGX_DISABLE_SYMLINKS_ON         1
#تعریف_کرو NGX_DISABLE_SYMLINKS_NOTOWNER   2
#اگر_ختم

#اگر_ختم /* _NGX_CORE_H_INCLUDED_ */
