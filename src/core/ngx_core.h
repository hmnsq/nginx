
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अगर_अघोषित _NGX_CORE_H_INCLUDED_
#घोषणा _NGX_CORE_H_INCLUDED_


#समावेश <ngx_config.h>


प्रकार काष्ठा ngx_module_s          ngx_module_t;
प्रकार काष्ठा ngx_conf_s            ngx_conf_t;
प्रकार काष्ठा ngx_cycle_s           ngx_cycle_t;
प्रकार काष्ठा ngx_pool_s            ngx_pool_t;
प्रकार काष्ठा ngx_chain_s           ngx_chain_t;
प्रकार काष्ठा ngx_log_s             ngx_log_t;
प्रकार काष्ठा ngx_open_file_s       ngx_open_file_t;
प्रकार काष्ठा ngx_command_s         ngx_command_t;
प्रकार काष्ठा ngx_file_s            ngx_file_t;
प्रकार काष्ठा ngx_event_s           ngx_event_t;
प्रकार काष्ठा ngx_event_aio_s       ngx_event_aio_t;
प्रकार काष्ठा ngx_connection_s      ngx_connection_t;
प्रकार काष्ठा ngx_thread_task_s     ngx_thread_task_t;
प्रकार काष्ठा ngx_ssl_s             ngx_ssl_t;
प्रकार काष्ठा ngx_ssl_cache_s       ngx_ssl_cache_t;
प्रकार काष्ठा ngx_proxy_protocol_s  ngx_proxy_protocol_t;
प्रकार काष्ठा ngx_quic_stream_s     ngx_quic_stream_t;
प्रकार काष्ठा ngx_ssl_connection_s  ngx_ssl_connection_t;
प्रकार काष्ठा ngx_udp_connection_s  ngx_udp_connection_t;

प्रकार व्योम (*ngx_event_handler_pt)(ngx_event_t *ev);
प्रकार व्योम (*ngx_connection_handler_pt)(ngx_connection_t *c);


#घोषणा  NGX_OK          0
#घोषणा  NGX_ERROR      -1
#घोषणा  NGX_AGAIN      -2
#घोषणा  NGX_BUSY       -3
#घोषणा  NGX_DONE       -4
#घोषणा  NGX_DECLINED   -5
#घोषणा  NGX_ABORT      -6


#समावेश <ngx_errno.h>
#समावेश <ngx_atomic.h>
#समावेश <ngx_thread.h>
#समावेश <ngx_rbtree.h>
#समावेश <ngx_time.h>
#समावेश <ngx_socket.h>
#समावेश <ngx_string.h>
#समावेश <ngx_files.h>
#समावेश <ngx_shmem.h>
#समावेश <ngx_process.h>
#समावेश <ngx_user.h>
#समावेश <ngx_dlopen.h>
#समावेश <ngx_parse.h>
#समावेश <ngx_parse_time.h>
#समावेश <ngx_log.h>
#समावेश <ngx_alloc.h>
#समावेश <ngx_palloc.h>
#समावेश <ngx_buf.h>
#समावेश <ngx_queue.h>
#समावेश <ngx_array.h>
#समावेश <ngx_list.h>
#समावेश <ngx_hash.h>
#समावेश <ngx_file.h>
#समावेश <ngx_crc.h>
#समावेश <ngx_crc32.h>
#समावेश <ngx_murmurhash.h>
#समावेश <ngx_siphash.h>
#अगर (NGX_PCRE)
#समावेश <ngx_regex.h>
#पूर्ण_अगर
#समावेश <ngx_radix_tree.h>
#समावेश <ngx_times.h>
#समावेश <ngx_rwlock.h>
#समावेश <ngx_shmtx.h>
#समावेश <ngx_data.h>
#समावेश <ngx_json.h>
#समावेश <ngx_slab.h>
#समावेश <ngx_inet.h>
#समावेश <ngx_cycle.h>
#समावेश <ngx_resolver.h>
#अगर (NGX_OPENSSL)
#समावेश <ngx_event_openssl.h>
#अगर (NGX_QUIC)
#समावेश <ngx_event_quic.h>
#पूर्ण_अगर
#पूर्ण_अगर
#समावेश <ngx_process_cycle.h>
#अगर (NGX_CONTROL_API)
#समावेश <ngx_control.h>
#पूर्ण_अगर
#समावेश <ngx_conf_file.h>
#समावेश <ngx_module.h>
#समावेश <ngx_open_file_cache.h>
#समावेश <ngx_os.h>
#समावेश <ngx_connection.h>
#समावेश <ngx_syslog.h>
#समावेश <ngx_proxy_protocol.h>
#अगर (NGX_HAVE_BPF)
#समावेश <ngx_bpf.h>
#पूर्ण_अगर


#घोषणा LF     (u_char) '\n'
#घोषणा CR     (u_char) '\r'
#घोषणा CRLF   "\r\n"


#घोषणा ngx_abs(मूल्य)       (((मूल्य) >= 0) ? (मूल्य) : - (मूल्य))
#घोषणा ngx_max(val1, val2)  ((val1 < val2) ? (val2) : (val1))
#घोषणा ngx_min(val1, val2)  ((val1 > val2) ? (val2) : (val1))

व्योम ngx_cpuinfo(व्योम);

#अगर (NGX_HAVE_OPENAT)
#घोषणा NGX_DISABLE_SYMLINKS_OFF        0
#घोषणा NGX_DISABLE_SYMLINKS_ON         1
#घोषणा NGX_DISABLE_SYMLINKS_NOTOWNER   2
#पूर्ण_अगर

#पूर्ण_अगर /* _NGX_CORE_H_INCLUDED_ */
