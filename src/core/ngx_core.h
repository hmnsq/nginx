
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_CORE_H_INCLUDED_
#define _NGX_CORE_H_INCLUDED_


#ଅନ୍ତର୍ଭାଵ <ngx_config.h>


ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା ngx_module_s          ngx_module_t;
ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା ngx_conf_s            ngx_conf_t;
ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା ngx_cycle_s           ngx_cycle_t;
ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା ngx_pool_s            ngx_pool_t;
ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା ngx_chain_s           ngx_chain_t;
ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା ngx_log_s             ngx_log_t;
ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା ngx_open_file_s       ngx_open_file_t;
ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା ngx_command_s         ngx_command_t;
ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା ngx_file_s            ngx_file_t;
ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା ngx_event_s           ngx_event_t;
ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା ngx_event_aio_s       ngx_event_aio_t;
ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା ngx_connection_s      ngx_connection_t;
ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା ngx_thread_task_s     ngx_thread_task_t;
ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା ngx_ssl_s             ngx_ssl_t;
ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା ngx_ssl_cache_s       ngx_ssl_cache_t;
ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା ngx_proxy_protocol_s  ngx_proxy_protocol_t;
ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା ngx_quic_stream_s     ngx_quic_stream_t;
ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା ngx_ssl_connection_s  ngx_ssl_connection_t;
ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା ngx_udp_connection_s  ngx_udp_connection_t;

ପ୍ରକାରବ୍ୟାଖ୍ୟା ଶୂନ୍ୟ (*ngx_event_handler_pt)(ngx_event_t *ev);
ପ୍ରକାରବ୍ୟାଖ୍ୟା ଶୂନ୍ୟ (*ngx_connection_handler_pt)(ngx_connection_t *c);


#define  NGX_OK          0
#define  NGX_ERROR      -1
#define  NGX_AGAIN      -2
#define  NGX_BUSY       -3
#define  NGX_DONE       -4
#define  NGX_DECLINED   -5
#define  NGX_ABORT      -6


#ଅନ୍ତର୍ଭାଵ <ngx_errno.h>
#ଅନ୍ତର୍ଭାଵ <ngx_atomic.h>
#ଅନ୍ତର୍ଭାଵ <ngx_thread.h>
#ଅନ୍ତର୍ଭାଵ <ngx_rbtree.h>
#ଅନ୍ତର୍ଭାଵ <ngx_time.h>
#ଅନ୍ତର୍ଭାଵ <ngx_socket.h>
#ଅନ୍ତର୍ଭାଵ <ngx_string.h>
#ଅନ୍ତର୍ଭାଵ <ngx_files.h>
#ଅନ୍ତର୍ଭାଵ <ngx_shmem.h>
#ଅନ୍ତର୍ଭାଵ <ngx_process.h>
#ଅନ୍ତର୍ଭାଵ <ngx_user.h>
#ଅନ୍ତର୍ଭାଵ <ngx_dlopen.h>
#ଅନ୍ତର୍ଭାଵ <ngx_parse.h>
#ଅନ୍ତର୍ଭାଵ <ngx_parse_time.h>
#ଅନ୍ତର୍ଭାଵ <ngx_log.h>
#ଅନ୍ତର୍ଭାଵ <ngx_alloc.h>
#ଅନ୍ତର୍ଭାଵ <ngx_palloc.h>
#ଅନ୍ତର୍ଭାଵ <ngx_buf.h>
#ଅନ୍ତର୍ଭାଵ <ngx_queue.h>
#ଅନ୍ତର୍ଭାଵ <ngx_array.h>
#ଅନ୍ତର୍ଭାଵ <ngx_list.h>
#ଅନ୍ତର୍ଭାଵ <ngx_hash.h>
#ଅନ୍ତର୍ଭାଵ <ngx_file.h>
#ଅନ୍ତର୍ଭାଵ <ngx_crc.h>
#ଅନ୍ତର୍ଭାଵ <ngx_crc32.h>
#ଅନ୍ତର୍ଭାଵ <ngx_murmurhash.h>
#ଅନ୍ତର୍ଭାଵ <ngx_siphash.h>
#ଯଦି (NGX_PCRE)
#ଅନ୍ତର୍ଭାଵ <ngx_regex.h>
#endif
#ଅନ୍ତର୍ଭାଵ <ngx_radix_tree.h>
#ଅନ୍ତର୍ଭାଵ <ngx_times.h>
#ଅନ୍ତର୍ଭାଵ <ngx_rwlock.h>
#ଅନ୍ତର୍ଭାଵ <ngx_shmtx.h>
#ଅନ୍ତର୍ଭାଵ <ngx_data.h>
#ଅନ୍ତର୍ଭାଵ <ngx_json.h>
#ଅନ୍ତର୍ଭାଵ <ngx_slab.h>
#ଅନ୍ତର୍ଭାଵ <ngx_inet.h>
#ଅନ୍ତର୍ଭାଵ <ngx_cycle.h>
#ଅନ୍ତର୍ଭାଵ <ngx_resolver.h>
#ଯଦି (NGX_OPENSSL)
#ଅନ୍ତର୍ଭାଵ <ngx_event_openssl.h>
#ଯଦି (NGX_QUIC)
#ଅନ୍ତର୍ଭାଵ <ngx_event_quic.h>
#endif
#endif
#ଅନ୍ତର୍ଭାଵ <ngx_process_cycle.h>
#ଯଦି (NGX_CONTROL_API)
#ଅନ୍ତର୍ଭାଵ <ngx_control.h>
#endif
#ଅନ୍ତର୍ଭାଵ <ngx_conf_file.h>
#ଅନ୍ତର୍ଭାଵ <ngx_module.h>
#ଅନ୍ତର୍ଭାଵ <ngx_open_file_cache.h>
#ଅନ୍ତର୍ଭାଵ <ngx_os.h>
#ଅନ୍ତର୍ଭାଵ <ngx_connection.h>
#ଅନ୍ତର୍ଭାଵ <ngx_syslog.h>
#ଅନ୍ତର୍ଭାଵ <ngx_proxy_protocol.h>
#ଯଦି (NGX_HAVE_BPF)
#ଅନ୍ତର୍ଭାଵ <ngx_bpf.h>
#endif


#define LF     (u_char) '\n'
#define CR     (u_char) '\r'
#define CRLF   "\r\n"


#define ngx_abs(ମୂଲ୍ୟ)       (((ମୂଲ୍ୟ) >= 0) ? (ମୂଲ୍ୟ) : - (ମୂଲ୍ୟ))
#define ngx_max(val1, val2)  ((val1 < val2) ? (val2) : (val1))
#define ngx_min(val1, val2)  ((val1 > val2) ? (val2) : (val1))

ଶୂନ୍ୟ ngx_cpuinfo(ଶୂନ୍ୟ);

#ଯଦି (NGX_HAVE_OPENAT)
#define NGX_DISABLE_SYMLINKS_OFF        0
#define NGX_DISABLE_SYMLINKS_ON         1
#define NGX_DISABLE_SYMLINKS_NOTOWNER   2
#endif

#endif /* _NGX_CORE_H_INCLUDED_ */
