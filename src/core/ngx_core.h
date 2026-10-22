
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#यदि_अपरिभाषितम् _NGX_CORE_H_INCLUDED_
#परिभाषय _NGX_CORE_H_INCLUDED_


#अन्तर्भाव <ngx_config.h>


प्रकारनिर्देश संरचना ngx_module_s          ngx_module_t;
प्रकारनिर्देश संरचना ngx_conf_s            ngx_conf_t;
प्रकारनिर्देश संरचना ngx_आवृत्तिः_s           ngx_आवृत्तिः_t;
प्रकारनिर्देश संरचना ngx_pool_s            ngx_pool_t;
प्रकारनिर्देश संरचना ngx_chain_s           ngx_chain_t;
प्रकारनिर्देश संरचना ngx_log_s             ngx_log_t;
प्रकारनिर्देश संरचना ngx_open_file_s       ngx_open_file_t;
प्रकारनिर्देश संरचना ngx_command_s         ngx_command_t;
प्रकारनिर्देश संरचना ngx_file_s            ngx_file_t;
प्रकारनिर्देश संरचना ngx_event_s           ngx_event_t;
प्रकारनिर्देश संरचना ngx_event_aio_s       ngx_event_aio_t;
प्रकारनिर्देश संरचना ngx_connection_s      ngx_connection_t;
प्रकारनिर्देश संरचना ngx_thread_task_s     ngx_thread_task_t;
प्रकारनिर्देश संरचना ngx_ssl_s             ngx_ssl_t;
प्रकारनिर्देश संरचना ngx_ssl_cache_s       ngx_ssl_cache_t;
प्रकारनिर्देश संरचना ngx_proxy_protocol_s  ngx_proxy_protocol_t;
प्रकारनिर्देश संरचना ngx_quic_प्रवाहः_s     ngx_quic_प्रवाहः_t;
प्रकारनिर्देश संरचना ngx_ssl_connection_s  ngx_ssl_connection_t;
प्रकारनिर्देश संरचना ngx_udp_connection_s  ngx_udp_connection_t;

प्रकारनिर्देश शून्य (*ngx_event_handler_pt)(ngx_event_t *ev);
प्रकारनिर्देश शून्य (*ngx_connection_handler_pt)(ngx_connection_t *c);


#परिभाषय  NGX_OK          0
#परिभाषय  NGX_ERROR      -1
#परिभाषय  NGX_AGAIN      -2
#परिभाषय  NGX_BUSY       -3
#परिभाषय  NGX_समाप्तम्       -4
#परिभाषय  NGX_DECLINED   -5
#परिभाषय  NGX_ABORT      -6


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
#यदि (NGX_PCRE)
#अन्तर्भाव <ngx_regex.h>
#यदि_अन्तः
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
#यदि (NGX_OPENSSL)
#अन्तर्भाव <ngx_event_openssl.h>
#यदि (NGX_QUIC)
#अन्तर्भाव <ngx_event_quic.h>
#यदि_अन्तः
#यदि_अन्तः
#अन्तर्भाव <ngx_process_cycle.h>
#यदि (NGX_CONTROL_API)
#अन्तर्भाव <ngx_control.h>
#यदि_अन्तः
#अन्तर्भाव <ngx_conf_file.h>
#अन्तर्भाव <ngx_module.h>
#अन्तर्भाव <ngx_open_file_cache.h>
#अन्तर्भाव <ngx_os.h>
#अन्तर्भाव <ngx_connection.h>
#अन्तर्भाव <ngx_syslog.h>
#अन्तर्भाव <ngx_proxy_protocol.h>
#यदि (NGX_HAVE_BPF)
#अन्तर्भाव <ngx_bpf.h>
#यदि_अन्तः


#परिभाषय LF     (u_char) '\n'
#परिभाषय CR     (u_char) '\r'
#परिभाषय CRLF   "\r\n"


#परिभाषय ngx_abs(मानम्)       (((मानम्) >= 0) ? (मानम्) : - (मानम्))
#परिभाषय ngx_max(val1, val2)  ((val1 < val2) ? (val2) : (val1))
#परिभाषय ngx_min(val1, val2)  ((val1 > val2) ? (val2) : (val1))

शून्य ngx_cpuinfo(शून्य);

#यदि (NGX_HAVE_OPENAT)
#परिभाषय NGX_DISABLE_SYMLINKS_OFF        0
#परिभाषय NGX_DISABLE_SYMLINKS_ON         1
#परिभाषय NGX_DISABLE_SYMLINKS_NOTOWNER   2
#यदि_अन्तः

#यदि_अन्तः /* _NGX_CORE_H_INCLUDED_ */
