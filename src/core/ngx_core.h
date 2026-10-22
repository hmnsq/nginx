
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_CORE_H_INCLUDED_
#define _NGX_CORE_H_INCLUDED_


#अन्तर्भाव <ngx_config.h>


प्रकारव्याख्या संरचना ngx_module_s          ngx_module_t;
प्रकारव्याख्या संरचना ngx_conf_s            ngx_conf_t;
प्रकारव्याख्या संरचना ngx_cycle_s           ngx_cycle_t;
प्रकारव्याख्या संरचना ngx_pool_s            ngx_pool_t;
प्रकारव्याख्या संरचना ngx_chain_s           ngx_chain_t;
प्रकारव्याख्या संरचना ngx_log_s             ngx_log_t;
प्रकारव्याख्या संरचना ngx_open_file_s       ngx_open_file_t;
प्रकारव्याख्या संरचना ngx_command_s         ngx_command_t;
प्रकारव्याख्या संरचना ngx_file_s            ngx_file_t;
प्रकारव्याख्या संरचना ngx_event_s           ngx_event_t;
प्रकारव्याख्या संरचना ngx_event_aio_s       ngx_event_aio_t;
प्रकारव्याख्या संरचना ngx_connection_s      ngx_connection_t;
प्रकारव्याख्या संरचना ngx_thread_task_s     ngx_thread_task_t;
प्रकारव्याख्या संरचना ngx_ssl_s             ngx_ssl_t;
प्रकारव्याख्या संरचना ngx_ssl_cache_s       ngx_ssl_cache_t;
प्रकारव्याख्या संरचना ngx_proxy_protocol_s  ngx_proxy_protocol_t;
प्रकारव्याख्या संरचना ngx_quic_stream_s     ngx_quic_stream_t;
प्रकारव्याख्या संरचना ngx_ssl_connection_s  ngx_ssl_connection_t;
प्रकारव्याख्या संरचना ngx_udp_connection_s  ngx_udp_connection_t;

प्रकारव्याख्या शून्य (*ngx_event_handler_pt)(ngx_event_t *ev);
प्रकारव्याख्या शून्य (*ngx_connection_handler_pt)(ngx_connection_t *c);


#define  NGX_OK          0
#define  NGX_ERROR      -1
#define  NGX_AGAIN      -2
#define  NGX_BUSY       -3
#define  NGX_DONE       -4
#define  NGX_DECLINED   -5
#define  NGX_ABORT      -6


#अन्तर्भाव <ngx_errno.h>
#अन्तर्भाव <ngx_atomic.h>
#अन्तर्भाव <ngx_thread.h>
#अन्तर्भाव <ngx_rbtree.h>
#अन्तर्भाव <ngx_time.h>
#अन्तर्भाव <ngx_socket.h>
#अन्तर्भाव <ngx_string.h>
#अन्तर्भाव <ngx_files.h>
#अन्तर्भाव <ngx_shmem.h>
#अन्तर्भाव <ngx_process.h>
#अन्तर्भाव <ngx_user.h>
#अन्तर्भाव <ngx_dlopen.h>
#अन्तर्भाव <ngx_parse.h>
#अन्तर्भाव <ngx_parse_time.h>
#अन्तर्भाव <ngx_log.h>
#अन्तर्भाव <ngx_alloc.h>
#अन्तर्भाव <ngx_palloc.h>
#अन्तर्भाव <ngx_buf.h>
#अन्तर्भाव <ngx_queue.h>
#अन्तर्भाव <ngx_array.h>
#अन्तर्भाव <ngx_list.h>
#अन्तर्भाव <ngx_hash.h>
#अन्तर्भाव <ngx_file.h>
#अन्तर्भाव <ngx_crc.h>
#अन्तर्भाव <ngx_crc32.h>
#अन्तर्भाव <ngx_murmurhash.h>
#अन्तर्भाव <ngx_siphash.h>
#जर (NGX_PCRE)
#अन्तर्भाव <ngx_regex.h>
#endif
#अन्तर्भाव <ngx_radix_tree.h>
#अन्तर्भाव <ngx_times.h>
#अन्तर्भाव <ngx_rwlock.h>
#अन्तर्भाव <ngx_shmtx.h>
#अन्तर्भाव <ngx_data.h>
#अन्तर्भाव <ngx_json.h>
#अन्तर्भाव <ngx_slab.h>
#अन्तर्भाव <ngx_inet.h>
#अन्तर्भाव <ngx_cycle.h>
#अन्तर्भाव <ngx_resolver.h>
#जर (NGX_OPENSSL)
#अन्तर्भाव <ngx_event_openssl.h>
#जर (NGX_QUIC)
#अन्तर्भाव <ngx_event_quic.h>
#endif
#endif
#अन्तर्भाव <ngx_process_cycle.h>
#जर (NGX_CONTROL_API)
#अन्तर्भाव <ngx_control.h>
#endif
#अन्तर्भाव <ngx_conf_file.h>
#अन्तर्भाव <ngx_module.h>
#अन्तर्भाव <ngx_open_file_cache.h>
#अन्तर्भाव <ngx_os.h>
#अन्तर्भाव <ngx_connection.h>
#अन्तर्भाव <ngx_syslog.h>
#अन्तर्भाव <ngx_proxy_protocol.h>
#जर (NGX_HAVE_BPF)
#अन्तर्भाव <ngx_bpf.h>
#endif


#define LF     (u_char) '\n'
#define CR     (u_char) '\r'
#define CRLF   "\r\n"


#define ngx_abs(मूल्य)       (((मूल्य) >= 0) ? (मूल्य) : - (मूल्य))
#define ngx_max(val1, val2)  ((val1 < val2) ? (val2) : (val1))
#define ngx_min(val1, val2)  ((val1 > val2) ? (val2) : (val1))

शून्य ngx_cpuinfo(शून्य);

#जर (NGX_HAVE_OPENAT)
#define NGX_DISABLE_SYMLINKS_OFF        0
#define NGX_DISABLE_SYMLINKS_ON         1
#define NGX_DISABLE_SYMLINKS_NOTOWNER   2
#endif

#endif /* _NGX_CORE_H_INCLUDED_ */
