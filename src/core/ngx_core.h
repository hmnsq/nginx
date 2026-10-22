
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_CORE_H_INCLUDED_
#define _NGX_CORE_H_INCLUDED_


#ಅನ್ತರ್ಭಾವ <ngx_config.h>


ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ ngx_module_s          ngx_module_t;
ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ ngx_conf_s            ngx_conf_t;
ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ ngx_cycle_s           ngx_cycle_t;
ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ ngx_pool_s            ngx_pool_t;
ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ ngx_chain_s           ngx_chain_t;
ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ ngx_log_s             ngx_log_t;
ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ ngx_open_file_s       ngx_open_file_t;
ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ ngx_command_s         ngx_command_t;
ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ ngx_file_s            ngx_file_t;
ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ ngx_event_s           ngx_event_t;
ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ ngx_event_aio_s       ngx_event_aio_t;
ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ ngx_connection_s      ngx_connection_t;
ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ ngx_thread_task_s     ngx_thread_task_t;
ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ ngx_ssl_s             ngx_ssl_t;
ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ ngx_ssl_cache_s       ngx_ssl_cache_t;
ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ ngx_proxy_protocol_s  ngx_proxy_protocol_t;
ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ ngx_quic_stream_s     ngx_quic_stream_t;
ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ ngx_ssl_connection_s  ngx_ssl_connection_t;
ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ ngx_udp_connection_s  ngx_udp_connection_t;

ಪ್ರಕಾರನಿರ್ದೇಶ ಶೂನ್ಯ (*ngx_event_handler_pt)(ngx_event_t *ev);
ಪ್ರಕಾರನಿರ್ದೇಶ ಶೂನ್ಯ (*ngx_connection_handler_pt)(ngx_connection_t *c);


#define  NGX_OK          0
#define  NGX_ERROR      -1
#define  NGX_AGAIN      -2
#define  NGX_BUSY       -3
#define  NGX_DONE       -4
#define  NGX_DECLINED   -5
#define  NGX_ABORT      -6


#ಅನ್ತರ್ಭಾವ <ngx_errno.h>
#ಅನ್ತರ್ಭಾವ <ngx_atomic.h>
#ಅನ್ತರ್ಭಾವ <ngx_thread.h>
#ಅನ್ತರ್ಭಾವ <ngx_rbtree.h>
#ಅನ್ತರ್ಭಾವ <ngx_time.h>
#ಅನ್ತರ್ಭಾವ <ngx_socket.h>
#ಅನ್ತರ್ಭಾವ <ngx_string.h>
#ಅನ್ತರ್ಭಾವ <ngx_files.h>
#ಅನ್ತರ್ಭಾವ <ngx_shmem.h>
#ಅನ್ತರ್ಭಾವ <ngx_process.h>
#ಅನ್ತರ್ಭಾವ <ngx_user.h>
#ಅನ್ತರ್ಭಾವ <ngx_dlopen.h>
#ಅನ್ತರ್ಭಾವ <ngx_parse.h>
#ಅನ್ತರ್ಭಾವ <ngx_parse_time.h>
#ಅನ್ತರ್ಭಾವ <ngx_log.h>
#ಅನ್ತರ್ಭಾವ <ngx_alloc.h>
#ಅನ್ತರ್ಭಾವ <ngx_palloc.h>
#ಅನ್ತರ್ಭಾವ <ngx_buf.h>
#ಅನ್ತರ್ಭಾವ <ngx_queue.h>
#ಅನ್ತರ್ಭಾವ <ngx_array.h>
#ಅನ್ತರ್ಭಾವ <ngx_list.h>
#ಅನ್ತರ್ಭಾವ <ngx_hash.h>
#ಅನ್ತರ್ಭಾವ <ngx_file.h>
#ಅನ್ತರ್ಭಾವ <ngx_crc.h>
#ಅನ್ತರ್ಭಾವ <ngx_crc32.h>
#ಅನ್ತರ್ಭಾವ <ngx_murmurhash.h>
#ಅನ್ತರ್ಭಾವ <ngx_siphash.h>
#ಯದಿ (NGX_PCRE)
#ಅನ್ತರ್ಭಾವ <ngx_regex.h>
#endif
#ಅನ್ತರ್ಭಾವ <ngx_radix_tree.h>
#ಅನ್ತರ್ಭಾವ <ngx_times.h>
#ಅನ್ತರ್ಭಾವ <ngx_rwlock.h>
#ಅನ್ತರ್ಭಾವ <ngx_shmtx.h>
#ಅನ್ತರ್ಭಾವ <ngx_data.h>
#ಅನ್ತರ್ಭಾವ <ngx_json.h>
#ಅನ್ತರ್ಭಾವ <ngx_slab.h>
#ಅನ್ತರ್ಭಾವ <ngx_inet.h>
#ಅನ್ತರ್ಭಾವ <ngx_cycle.h>
#ಅನ್ತರ್ಭಾವ <ngx_resolver.h>
#ಯದಿ (NGX_OPENSSL)
#ಅನ್ತರ್ಭಾವ <ngx_event_openssl.h>
#ಯದಿ (NGX_QUIC)
#ಅನ್ತರ್ಭಾವ <ngx_event_quic.h>
#endif
#endif
#ಅನ್ತರ್ಭಾವ <ngx_process_cycle.h>
#ಯದಿ (NGX_CONTROL_API)
#ಅನ್ತರ್ಭಾವ <ngx_control.h>
#endif
#ಅನ್ತರ್ಭಾವ <ngx_conf_file.h>
#ಅನ್ತರ್ಭಾವ <ngx_module.h>
#ಅನ್ತರ್ಭಾವ <ngx_open_file_cache.h>
#ಅನ್ತರ್ಭಾವ <ngx_os.h>
#ಅನ್ತರ್ಭಾವ <ngx_connection.h>
#ಅನ್ತರ್ಭಾವ <ngx_syslog.h>
#ಅನ್ತರ್ಭಾವ <ngx_proxy_protocol.h>
#ಯದಿ (NGX_HAVE_BPF)
#ಅನ್ತರ್ಭಾವ <ngx_bpf.h>
#endif


#define LF     (u_char) '\n'
#define CR     (u_char) '\r'
#define CRLF   "\r\n"


#define ngx_abs(ಮೌಲ್ಯ)       (((ಮೌಲ್ಯ) >= 0) ? (ಮೌಲ್ಯ) : - (ಮೌಲ್ಯ))
#define ngx_max(val1, val2)  ((val1 < val2) ? (val2) : (val1))
#define ngx_min(val1, val2)  ((val1 > val2) ? (val2) : (val1))

ಶೂನ್ಯ ngx_cpuinfo(ಶೂನ್ಯ);

#ಯದಿ (NGX_HAVE_OPENAT)
#define NGX_DISABLE_SYMLINKS_OFF        0
#define NGX_DISABLE_SYMLINKS_ON         1
#define NGX_DISABLE_SYMLINKS_NOTOWNER   2
#endif

#endif /* _NGX_CORE_H_INCLUDED_ */
