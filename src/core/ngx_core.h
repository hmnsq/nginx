
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ _NGX_CORE_H_INCLUDED_
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _NGX_CORE_H_INCLUDED_


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ngx_module_s          ngx_module_t;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ngx_conf_s            ngx_conf_t;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ngx_ਚੱਕਰ_s           ngx_ਚੱਕਰ_t;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ngx_pool_s            ngx_pool_t;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ngx_chain_s           ngx_chain_t;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ngx_log_s             ngx_log_t;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ngx_open_file_s       ngx_open_file_t;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ngx_command_s         ngx_command_t;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ngx_file_s            ngx_file_t;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ngx_event_s           ngx_event_t;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ngx_event_aio_s       ngx_event_aio_t;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ngx_connection_s      ngx_connection_t;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ngx_thread_task_s     ngx_thread_task_t;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ngx_ssl_s             ngx_ssl_t;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ngx_ssl_cache_s       ngx_ssl_cache_t;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ngx_proxy_protocol_s  ngx_proxy_protocol_t;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ngx_quic_ਧਾਰਾ_s     ngx_quic_ਧਾਰਾ_t;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ngx_ssl_connection_s  ngx_ssl_connection_t;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ngx_udp_connection_s  ngx_udp_connection_t;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (*ngx_event_handler_pt)(ngx_event_t *ev);
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (*ngx_connection_handler_pt)(ngx_connection_t *c);


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ  NGX_OK          0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ  NGX_ERROR      -1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ  NGX_AGAIN      -2
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ  NGX_BUSY       -3
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ  NGX_ਪੂਰਾ       -4
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ  NGX_DECLINED   -5
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ  NGX_ABORT      -6


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_errno.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_atomic.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_thread.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_rbtree.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_time.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_socket.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_files.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_shmem.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_process.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_user.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_dlopen.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_parse.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_parse_time.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_log.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_alloc.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_palloc.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_buf.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_queue.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_array.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_list.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_hash.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_file.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_crc.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_crc32.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_murmurhash.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_siphash.h>
#ਜੇ (NGX_PCRE)
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_regex.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_radix_tree.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_times.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_rwlock.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_shmtx.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_data.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_json.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_slab.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_inet.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_cycle.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_resolver.h>
#ਜੇ (NGX_OPENSSL)
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_event_openssl.h>
#ਜੇ (NGX_QUIC)
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_event_quic.h>
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_process_cycle.h>
#ਜੇ (NGX_CONTROL_API)
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_control.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_conf_file.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_module.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_open_file_cache.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_os.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_connection.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_syslog.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_proxy_protocol.h>
#ਜੇ (NGX_HAVE_BPF)
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_bpf.h>
#ਜੇ_ਅੰਤ


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LF     (u_char) '\n'
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CR     (u_char) '\r'
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CRLF   "\r\n"


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_abs(ਮੁੱਲ)       (((ਮੁੱਲ) >= 0) ? (ਮੁੱਲ) : - (ਮੁੱਲ))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_max(val1, val2)  ((val1 < val2) ? (val2) : (val1))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_min(val1, val2)  ((val1 > val2) ? (val2) : (val1))

ਖਾਲੀ ngx_cpuinfo(ਖਾਲੀ);

#ਜੇ (NGX_HAVE_OPENAT)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_DISABLE_SYMLINKS_OFF        0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_DISABLE_SYMLINKS_ON         1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_DISABLE_SYMLINKS_NOTOWNER   2
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ /* _NGX_CORE_H_INCLUDED_ */
