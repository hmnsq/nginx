
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_CORE_H_INCLUDED_
#define _NGX_CORE_H_INCLUDED_


#അന്തര്ഭാവ <ngx_config.h>


തരനിർവചനം ഘടന ngx_module_s          ngx_module_t;
തരനിർവചനം ഘടന ngx_conf_s            ngx_conf_t;
തരനിർവചനം ഘടന ngx_cycle_s           ngx_cycle_t;
തരനിർവചനം ഘടന ngx_pool_s            ngx_pool_t;
തരനിർവചനം ഘടന ngx_chain_s           ngx_chain_t;
തരനിർവചനം ഘടന ngx_log_s             ngx_log_t;
തരനിർവചനം ഘടന ngx_open_file_s       ngx_open_file_t;
തരനിർവചനം ഘടന ngx_command_s         ngx_command_t;
തരനിർവചനം ഘടന ngx_file_s            ngx_file_t;
തരനിർവചനം ഘടന ngx_event_s           ngx_event_t;
തരനിർവചനം ഘടന ngx_event_aio_s       ngx_event_aio_t;
തരനിർവചനം ഘടന ngx_connection_s      ngx_connection_t;
തരനിർവചനം ഘടന ngx_thread_task_s     ngx_thread_task_t;
തരനിർവചനം ഘടന ngx_ssl_s             ngx_ssl_t;
തരനിർവചനം ഘടന ngx_ssl_cache_s       ngx_ssl_cache_t;
തരനിർവചനം ഘടന ngx_proxy_protocol_s  ngx_proxy_protocol_t;
തരനിർവചനം ഘടന ngx_quic_stream_s     ngx_quic_stream_t;
തരനിർവചനം ഘടന ngx_ssl_connection_s  ngx_ssl_connection_t;
തരനിർവചനം ഘടന ngx_udp_connection_s  ngx_udp_connection_t;

തരനിർവചനം ശൂന്യം (*ngx_event_handler_pt)(ngx_event_t *ev);
തരനിർവചനം ശൂന്യം (*ngx_connection_handler_pt)(ngx_connection_t *c);


#define  NGX_OK          0
#define  NGX_ERROR      -1
#define  NGX_AGAIN      -2
#define  NGX_BUSY       -3
#define  NGX_DONE       -4
#define  NGX_DECLINED   -5
#define  NGX_ABORT      -6


#അന്തര്ഭാവ <ngx_errno.h>
#അന്തര്ഭാവ <ngx_atomic.h>
#അന്തര്ഭാവ <ngx_thread.h>
#അന്തര്ഭാവ <ngx_rbtree.h>
#അന്തര്ഭാവ <ngx_time.h>
#അന്തര്ഭാവ <ngx_socket.h>
#അന്തര്ഭാവ <ngx_string.h>
#അന്തര്ഭാവ <ngx_files.h>
#അന്തര്ഭാവ <ngx_shmem.h>
#അന്തര്ഭാവ <ngx_process.h>
#അന്തര്ഭാവ <ngx_user.h>
#അന്തര്ഭാവ <ngx_dlopen.h>
#അന്തര്ഭാവ <ngx_parse.h>
#അന്തര്ഭാവ <ngx_parse_time.h>
#അന്തര്ഭാവ <ngx_log.h>
#അന്തര്ഭാവ <ngx_alloc.h>
#അന്തര്ഭാവ <ngx_palloc.h>
#അന്തര്ഭാവ <ngx_buf.h>
#അന്തര്ഭാവ <ngx_queue.h>
#അന്തര്ഭാവ <ngx_array.h>
#അന്തര്ഭാവ <ngx_list.h>
#അന്തര്ഭാവ <ngx_hash.h>
#അന്തര്ഭാവ <ngx_file.h>
#അന്തര്ഭാവ <ngx_crc.h>
#അന്തര്ഭാവ <ngx_crc32.h>
#അന്തര്ഭാവ <ngx_murmurhash.h>
#അന്തര്ഭാവ <ngx_siphash.h>
#എങ്കിൽ (NGX_PCRE)
#അന്തര്ഭാവ <ngx_regex.h>
#endif
#അന്തര്ഭാവ <ngx_radix_tree.h>
#അന്തര്ഭാവ <ngx_times.h>
#അന്തര്ഭാവ <ngx_rwlock.h>
#അന്തര്ഭാവ <ngx_shmtx.h>
#അന്തര്ഭാവ <ngx_data.h>
#അന്തര്ഭാവ <ngx_json.h>
#അന്തര്ഭാവ <ngx_slab.h>
#അന്തര്ഭാവ <ngx_inet.h>
#അന്തര്ഭാവ <ngx_cycle.h>
#അന്തര്ഭാവ <ngx_resolver.h>
#എങ്കിൽ (NGX_OPENSSL)
#അന്തര്ഭാവ <ngx_event_openssl.h>
#എങ്കിൽ (NGX_QUIC)
#അന്തര്ഭാവ <ngx_event_quic.h>
#endif
#endif
#അന്തര്ഭാവ <ngx_process_cycle.h>
#എങ്കിൽ (NGX_CONTROL_API)
#അന്തര്ഭാവ <ngx_control.h>
#endif
#അന്തര്ഭാവ <ngx_conf_file.h>
#അന്തര്ഭാവ <ngx_module.h>
#അന്തര്ഭാവ <ngx_open_file_cache.h>
#അന്തര്ഭാവ <ngx_os.h>
#അന്തര്ഭാവ <ngx_connection.h>
#അന്തര്ഭാവ <ngx_syslog.h>
#അന്തര്ഭാവ <ngx_proxy_protocol.h>
#എങ്കിൽ (NGX_HAVE_BPF)
#അന്തര്ഭാവ <ngx_bpf.h>
#endif


#define LF     (u_char) '\n'
#define CR     (u_char) '\r'
#define CRLF   "\r\n"


#define ngx_abs(മൂല്യം)       (((മൂല്യം) >= 0) ? (മൂല്യം) : - (മൂല്യം))
#define ngx_max(val1, val2)  ((val1 < val2) ? (val2) : (val1))
#define ngx_min(val1, val2)  ((val1 > val2) ? (val2) : (val1))

ശൂന്യം ngx_cpuinfo(ശൂന്യം);

#എങ്കിൽ (NGX_HAVE_OPENAT)
#define NGX_DISABLE_SYMLINKS_OFF        0
#define NGX_DISABLE_SYMLINKS_ON         1
#define NGX_DISABLE_SYMLINKS_NOTOWNER   2
#endif

#endif /* _NGX_CORE_H_INCLUDED_ */
