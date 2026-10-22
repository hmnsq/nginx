
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_CORE_H_INCLUDED_
#define _NGX_CORE_H_INCLUDED_


#অন্তর্ভাৱ <ngx_config.h>


প্রকারনির্দেশ সংরচনা ngx_module_s          ngx_module_t;
প্রকারনির্দেশ সংরচনা ngx_conf_s            ngx_conf_t;
প্রকারনির্দেশ সংরচনা ngx_cycle_s           ngx_cycle_t;
প্রকারনির্দেশ সংরচনা ngx_pool_s            ngx_pool_t;
প্রকারনির্দেশ সংরচনা ngx_chain_s           ngx_chain_t;
প্রকারনির্দেশ সংরচনা ngx_log_s             ngx_log_t;
প্রকারনির্দেশ সংরচনা ngx_open_file_s       ngx_open_file_t;
প্রকারনির্দেশ সংরচনা ngx_command_s         ngx_command_t;
প্রকারনির্দেশ সংরচনা ngx_file_s            ngx_file_t;
প্রকারনির্দেশ সংরচনা ngx_event_s           ngx_event_t;
প্রকারনির্দেশ সংরচনা ngx_event_aio_s       ngx_event_aio_t;
প্রকারনির্দেশ সংরচনা ngx_connection_s      ngx_connection_t;
প্রকারনির্দেশ সংরচনা ngx_thread_task_s     ngx_thread_task_t;
প্রকারনির্দেশ সংরচনা ngx_ssl_s             ngx_ssl_t;
প্রকারনির্দেশ সংরচনা ngx_ssl_cache_s       ngx_ssl_cache_t;
প্রকারনির্দেশ সংরচনা ngx_proxy_protocol_s  ngx_proxy_protocol_t;
প্রকারনির্দেশ সংরচনা ngx_quic_stream_s     ngx_quic_stream_t;
প্রকারনির্দেশ সংরচনা ngx_ssl_connection_s  ngx_ssl_connection_t;
প্রকারনির্দেশ সংরচনা ngx_udp_connection_s  ngx_udp_connection_t;

প্রকারনির্দেশ শূন্য (*ngx_event_handler_pt)(ngx_event_t *ev);
প্রকারনির্দেশ শূন্য (*ngx_connection_handler_pt)(ngx_connection_t *c);


#define  NGX_OK          0
#define  NGX_ERROR      -1
#define  NGX_AGAIN      -2
#define  NGX_BUSY       -3
#define  NGX_DONE       -4
#define  NGX_DECLINED   -5
#define  NGX_ABORT      -6


#অন্তর্ভাৱ <ngx_errno.h>
#অন্তর্ভাৱ <ngx_atomic.h>
#অন্তর্ভাৱ <ngx_thread.h>
#অন্তর্ভাৱ <ngx_rbtree.h>
#অন্তর্ভাৱ <ngx_time.h>
#অন্তর্ভাৱ <ngx_socket.h>
#অন্তর্ভাৱ <ngx_string.h>
#অন্তর্ভাৱ <ngx_files.h>
#অন্তর্ভাৱ <ngx_shmem.h>
#অন্তর্ভাৱ <ngx_process.h>
#অন্তর্ভাৱ <ngx_user.h>
#অন্তর্ভাৱ <ngx_dlopen.h>
#অন্তর্ভাৱ <ngx_parse.h>
#অন্তর্ভাৱ <ngx_parse_time.h>
#অন্তর্ভাৱ <ngx_log.h>
#অন্তর্ভাৱ <ngx_alloc.h>
#অন্তর্ভাৱ <ngx_palloc.h>
#অন্তর্ভাৱ <ngx_buf.h>
#অন্তর্ভাৱ <ngx_queue.h>
#অন্তর্ভাৱ <ngx_array.h>
#অন্তর্ভাৱ <ngx_list.h>
#অন্তর্ভাৱ <ngx_hash.h>
#অন্তর্ভাৱ <ngx_file.h>
#অন্তর্ভাৱ <ngx_crc.h>
#অন্তর্ভাৱ <ngx_crc32.h>
#অন্তর্ভাৱ <ngx_murmurhash.h>
#অন্তর্ভাৱ <ngx_siphash.h>
#যদি (NGX_PCRE)
#অন্তর্ভাৱ <ngx_regex.h>
#endif
#অন্তর্ভাৱ <ngx_radix_tree.h>
#অন্তর্ভাৱ <ngx_times.h>
#অন্তর্ভাৱ <ngx_rwlock.h>
#অন্তর্ভাৱ <ngx_shmtx.h>
#অন্তর্ভাৱ <ngx_data.h>
#অন্তর্ভাৱ <ngx_json.h>
#অন্তর্ভাৱ <ngx_slab.h>
#অন্তর্ভাৱ <ngx_inet.h>
#অন্তর্ভাৱ <ngx_cycle.h>
#অন্তর্ভাৱ <ngx_resolver.h>
#যদি (NGX_OPENSSL)
#অন্তর্ভাৱ <ngx_event_openssl.h>
#যদি (NGX_QUIC)
#অন্তর্ভাৱ <ngx_event_quic.h>
#endif
#endif
#অন্তর্ভাৱ <ngx_process_cycle.h>
#যদি (NGX_CONTROL_API)
#অন্তর্ভাৱ <ngx_control.h>
#endif
#অন্তর্ভাৱ <ngx_conf_file.h>
#অন্তর্ভাৱ <ngx_module.h>
#অন্তর্ভাৱ <ngx_open_file_cache.h>
#অন্তর্ভাৱ <ngx_os.h>
#অন্তর্ভাৱ <ngx_connection.h>
#অন্তর্ভাৱ <ngx_syslog.h>
#অন্তর্ভাৱ <ngx_proxy_protocol.h>
#যদি (NGX_HAVE_BPF)
#অন্তর্ভাৱ <ngx_bpf.h>
#endif


#define LF     (u_char) '\n'
#define CR     (u_char) '\r'
#define CRLF   "\r\n"


#define ngx_abs(মান)       (((মান) >= 0) ? (মান) : - (মান))
#define ngx_max(val1, val2)  ((val1 < val2) ? (val2) : (val1))
#define ngx_min(val1, val2)  ((val1 > val2) ? (val2) : (val1))

শূন্য ngx_cpuinfo(শূন্য);

#যদি (NGX_HAVE_OPENAT)
#define NGX_DISABLE_SYMLINKS_OFF        0
#define NGX_DISABLE_SYMLINKS_ON         1
#define NGX_DISABLE_SYMLINKS_NOTOWNER   2
#endif

#endif /* _NGX_CORE_H_INCLUDED_ */
