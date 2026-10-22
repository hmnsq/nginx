
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#నిర్వచితం_కాకపోతే _NGX_CORE_H_INCLUDED_
#నిర్వచించు _NGX_CORE_H_INCLUDED_


#అన్తర్భావ <ngx_config.h>


రకనిర్వచనం నిర్మాణం ngx_module_s          ngx_module_t;
రకనిర్వచనం నిర్మాణం ngx_conf_s            ngx_conf_t;
రకనిర్వచనం నిర్మాణం ngx_చక్రం_s           ngx_చక్రం_t;
రకనిర్వచనం నిర్మాణం ngx_pool_s            ngx_pool_t;
రకనిర్వచనం నిర్మాణం ngx_chain_s           ngx_chain_t;
రకనిర్వచనం నిర్మాణం ngx_log_s             ngx_log_t;
రకనిర్వచనం నిర్మాణం ngx_open_file_s       ngx_open_file_t;
రకనిర్వచనం నిర్మాణం ngx_command_s         ngx_command_t;
రకనిర్వచనం నిర్మాణం ngx_file_s            ngx_file_t;
రకనిర్వచనం నిర్మాణం ngx_event_s           ngx_event_t;
రకనిర్వచనం నిర్మాణం ngx_event_aio_s       ngx_event_aio_t;
రకనిర్వచనం నిర్మాణం ngx_connection_s      ngx_connection_t;
రకనిర్వచనం నిర్మాణం ngx_thread_task_s     ngx_thread_task_t;
రకనిర్వచనం నిర్మాణం ngx_ssl_s             ngx_ssl_t;
రకనిర్వచనం నిర్మాణం ngx_ssl_cache_s       ngx_ssl_cache_t;
రకనిర్వచనం నిర్మాణం ngx_proxy_protocol_s  ngx_proxy_protocol_t;
రకనిర్వచనం నిర్మాణం ngx_quic_ప్రవాహం_s     ngx_quic_ప్రవాహం_t;
రకనిర్వచనం నిర్మాణం ngx_ssl_connection_s  ngx_ssl_connection_t;
రకనిర్వచనం నిర్మాణం ngx_udp_connection_s  ngx_udp_connection_t;

రకనిర్వచనం శూన్య (*ngx_event_handler_pt)(ngx_event_t *ev);
రకనిర్వచనం శూన్య (*ngx_connection_handler_pt)(ngx_connection_t *c);


#నిర్వచించు  NGX_OK          0
#నిర్వచించు  NGX_ERROR      -1
#నిర్వచించు  NGX_AGAIN      -2
#నిర్వచించు  NGX_BUSY       -3
#నిర్వచించు  NGX_పూర్తి       -4
#నిర్వచించు  NGX_DECLINED   -5
#నిర్వచించు  NGX_ABORT      -6


#అన్తర్భావ <ngx_errno.h>
#అన్తర్భావ <ngx_atomic.h>
#అన్తర్భావ <ngx_thread.h>
#అన్తర్భావ <ngx_rbtree.h>
#అన్తర్భావ <ngx_time.h>
#అన్తర్భావ <ngx_socket.h>
#అన్తర్భావ <ngx_string.h>
#అన్తర్భావ <ngx_files.h>
#అన్తర్భావ <ngx_shmem.h>
#అన్తర్భావ <ngx_process.h>
#అన్తర్భావ <ngx_user.h>
#అన్తర్భావ <ngx_dlopen.h>
#అన్తర్భావ <ngx_parse.h>
#అన్తర్భావ <ngx_parse_time.h>
#అన్తర్భావ <ngx_log.h>
#అన్తర్భావ <ngx_alloc.h>
#అన్తర్భావ <ngx_palloc.h>
#అన్తర్భావ <ngx_buf.h>
#అన్తర్భావ <ngx_queue.h>
#అన్తర్భావ <ngx_array.h>
#అన్తర్భావ <ngx_list.h>
#అన్తర్భావ <ngx_hash.h>
#అన్తర్భావ <ngx_file.h>
#అన్తర్భావ <ngx_crc.h>
#అన్తర్భావ <ngx_crc32.h>
#అన్తర్భావ <ngx_murmurhash.h>
#అన్తర్భావ <ngx_siphash.h>
#ఒకవేళ (NGX_PCRE)
#అన్తర్భావ <ngx_regex.h>
#అయితే_ముగింపు
#అన్తర్భావ <ngx_radix_tree.h>
#అన్తర్భావ <ngx_times.h>
#అన్తర్భావ <ngx_rwlock.h>
#అన్తర్భావ <ngx_shmtx.h>
#అన్తర్భావ <ngx_data.h>
#అన్తర్భావ <ngx_json.h>
#అన్తర్భావ <ngx_slab.h>
#అన్తర్భావ <ngx_inet.h>
#అన్తర్భావ <ngx_cycle.h>
#అన్తర్భావ <ngx_resolver.h>
#ఒకవేళ (NGX_OPENSSL)
#అన్తర్భావ <ngx_event_openssl.h>
#ఒకవేళ (NGX_QUIC)
#అన్తర్భావ <ngx_event_quic.h>
#అయితే_ముగింపు
#అయితే_ముగింపు
#అన్తర్భావ <ngx_process_cycle.h>
#ఒకవేళ (NGX_CONTROL_API)
#అన్తర్భావ <ngx_control.h>
#అయితే_ముగింపు
#అన్తర్భావ <ngx_conf_file.h>
#అన్తర్భావ <ngx_module.h>
#అన్తర్భావ <ngx_open_file_cache.h>
#అన్తర్భావ <ngx_os.h>
#అన్తర్భావ <ngx_connection.h>
#అన్తర్భావ <ngx_syslog.h>
#అన్తర్భావ <ngx_proxy_protocol.h>
#ఒకవేళ (NGX_HAVE_BPF)
#అన్తర్భావ <ngx_bpf.h>
#అయితే_ముగింపు


#నిర్వచించు LF     (u_char) '\n'
#నిర్వచించు CR     (u_char) '\r'
#నిర్వచించు CRLF   "\r\n"


#నిర్వచించు ngx_abs(విలువ)       (((విలువ) >= 0) ? (విలువ) : - (విలువ))
#నిర్వచించు ngx_max(val1, val2)  ((val1 < val2) ? (val2) : (val1))
#నిర్వచించు ngx_min(val1, val2)  ((val1 > val2) ? (val2) : (val1))

శూన్య ngx_cpuinfo(శూన్య);

#ఒకవేళ (NGX_HAVE_OPENAT)
#నిర్వచించు NGX_DISABLE_SYMLINKS_OFF        0
#నిర్వచించు NGX_DISABLE_SYMLINKS_ON         1
#నిర్వచించు NGX_DISABLE_SYMLINKS_NOTOWNER   2
#అయితే_ముగింపు

#అయితే_ముగింపు /* _NGX_CORE_H_INCLUDED_ */
