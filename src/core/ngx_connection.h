
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_CONNECTION_H_INCLUDED_
#define _NGX_CONNECTION_H_INCLUDED_


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


प्रकारव्याख्या संरचना ngx_listening_s  ngx_listening_t;

संरचना ngx_listening_s {
    ngx_socket_t        fd;

    संरचना sockaddr    *sockaddr;
    socklen_t           socklen;    /* size of sockaddr */
    माप_प्रकार              addr_text_max_len;
    ngx_str_t           addr_text;

    पूर्णांक                 type;
    पूर्णांक                 protocol;

    पूर्णांक                 backlog;
    पूर्णांक                 rcvbuf;
    पूर्णांक                 sndbuf;
#जर (NGX_HAVE_KEEPALIVE_TUNABLE)
    पूर्णांक                 keepidle;
    पूर्णांक                 keepintvl;
    पूर्णांक                 keepcnt;
#endif

    /* handler of accepted connection */
    ngx_connection_handler_pt   handler;

    शून्य               *servers;  /* array of ngx_http_in_addr_t, for example */

    ngx_log_t           log;
    ngx_log_t          *logp;

    माप_प्रकार              pool_size;

    ngx_listening_t    *previous;
    ngx_connection_t   *connection;

    ngx_rbtree_t        rbtree;
    ngx_rbtree_गाठ_t   sentinel;

    ngx_uint_t          worker;

    अचिन्हित            खोलो:1;
    अचिन्हित            remain:1;
    अचिन्हित            ignore:1;

    अचिन्हित            bound:1;       /* already bound */
    अचिन्हित            inherited:1;   /* inherited from previous process */
    अचिन्हित            nonblocking_accept:1;
    अचिन्हित            listen:1;
    अचिन्हित            nonblocking:1;
    अचिन्हित            shared:1;    /* shared between threads or processes */
    अचिन्हित            addr_ntop:1;
    अचिन्हित            wildcard:1;

#जर (NGX_HAVE_INET6)
    अचिन्हित            ipv6only:1;
#endif
    अचिन्हित            reuseport:1;
    अचिन्हित            add_reuseport:1;
    अचिन्हित            keepalive:2;
    अचिन्हित            quic:1;

    अचिन्हित            change_protocol:1;

    अचिन्हित            deferred_accept:1;
    अचिन्हित            delete_deferred:1;
    अचिन्हित            add_deferred:1;
#जर (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
    अक्षर               *accept_filter;
#endif
#जर (NGX_HAVE_SETFIB)
    पूर्णांक                 setfib;
#endif

#जर (NGX_HAVE_TCP_FASTOPEN)
    पूर्णांक                 fastopen;
#endif

};


प्रकारव्याख्या गणना {
    NGX_ERROR_ALERT = 0,
    NGX_ERROR_ERR,
    NGX_ERROR_INFO,
    NGX_ERROR_IGNORE_ECONNRESET,
    NGX_ERROR_IGNORE_EINVAL,
    NGX_ERROR_IGNORE_EMSGSIZE
} ngx_connection_log_error_e;


प्रकारव्याख्या गणना {
    NGX_TCP_NODELAY_UNSET = 0,
    NGX_TCP_NODELAY_SET,
    NGX_TCP_NODELAY_DISABLED
} ngx_connection_tcp_nodelay_e;


प्रकारव्याख्या गणना {
    NGX_TCP_NOPUSH_UNSET = 0,
    NGX_TCP_NOPUSH_SET,
    NGX_TCP_NOPUSH_DISABLED
} ngx_connection_tcp_nopush_e;


#define NGX_LOWLEVEL_BUFFERED  0x0f
#define NGX_SSL_BUFFERED       0x01
#define NGX_HTTP_V2_BUFFERED   0x02


संरचना ngx_connection_s {
    शून्य               *data;
    ngx_event_t        *पठ;
    ngx_event_t        *लिख;

    ngx_socket_t        fd;

    ngx_recv_pt         recv;
    ngx_send_pt         send;
    ngx_recv_chain_pt   recv_chain;
    ngx_send_chain_pt   send_chain;

    ngx_listening_t    *listening;

    off_t               sent;

    ngx_log_t          *log;

    ngx_pool_t         *pool;

    पूर्णांक                 type;

    संरचना sockaddr    *sockaddr;
    socklen_t           socklen;
    ngx_str_t           addr_text;

    ngx_proxy_protocol_t  *proxy_protocol;

#जर (NGX_QUIC || NGX_COMPAT)
    ngx_quic_stream_t     *quic;
#endif

#जर (NGX_SSL || NGX_COMPAT)
    ngx_ssl_connection_t  *ssl;
#endif

    ngx_udp_connection_t  *udp;

    संरचना sockaddr    *local_sockaddr;
    socklen_t           local_socklen;

    ngx_buf_t          *buffer;

    ngx_queue_t         queue;

    ngx_atomic_uint_t   संख्या;

    ngx_msec_t          start_time;
    ngx_uint_t          requests;

    अचिन्हित            buffered:8;

    अचिन्हित            log_error:3;     /* ngx_connection_log_error_e */

    अचिन्हित            timedout:1;
    अचिन्हित            error:1;
    अचिन्हित            destroyed:1;
    अचिन्हित            pipeline:1;

    अचिन्हित            idle:1;
    अचिन्हित            reusable:1;
    अचिन्हित            बंद:1;
    अचिन्हित            shared:1;

    अचिन्हित            sendfile:1;
    अचिन्हित            sndlowat:1;
    अचिन्हित            tcp_nodelay:2;   /* ngx_connection_tcp_nodelay_e */
    अचिन्हित            tcp_nopush:2;    /* ngx_connection_tcp_nopush_e */

    अचिन्हित            need_last_buf:1;
    अचिन्हित            need_flush_buf:1;

#जर (NGX_HAVE_SENDFILE_NODISKIO || NGX_COMPAT)
    अचिन्हित            busy_मोजणी:2;
#endif

    अचिन्हित            ktls_recv:1;

#जर (NGX_THREADS || NGX_COMPAT)
    ngx_thread_task_t  *sendfile_task;
#endif
};


#define ngx_set_connection_log(c, l)                                         \
                                                                             \
    c->log->file = l->file;                                                  \
    c->log->next = l->next;                                                  \
    c->log->writer = l->writer;                                              \
    c->log->wdata = l->wdata;                                                \
    जर (!(c->log->log_level & NGX_LOG_DEBUG_CONNECTION)) {                   \
        c->log->log_level = l->log_level;                                    \
    }


ngx_listening_t *ngx_create_listening(ngx_conf_t *cf, संरचना sockaddr *sockaddr,
    socklen_t socklen);
ngx_int_t ngx_clone_listening(ngx_cycle_t *cycle, ngx_listening_t *ls);
ngx_int_t ngx_set_inherited_sockets(ngx_cycle_t *cycle);
ngx_int_t ngx_open_listening_sockets(ngx_cycle_t *cycle);
शून्य ngx_configure_listening_sockets(ngx_cycle_t *cycle);
शून्य ngx_close_listening_sockets(ngx_cycle_t *cycle);
शून्य ngx_close_connection(ngx_connection_t *c);
शून्य ngx_close_idle_connections(ngx_cycle_t *cycle);
ngx_int_t ngx_connection_local_sockaddr(ngx_connection_t *c, ngx_str_t *s,
    ngx_uint_t port);
ngx_int_t ngx_tcp_nodelay(ngx_connection_t *c);
ngx_int_t ngx_connection_error(ngx_connection_t *c, ngx_err_t err, अक्षर *text);

ngx_connection_t *ngx_get_connection(ngx_socket_t s, ngx_log_t *log);
शून्य ngx_free_connection(ngx_connection_t *c);

शून्य ngx_reusable_connection(ngx_connection_t *c, ngx_uint_t reusable);

#endif /* _NGX_CONNECTION_H_INCLUDED_ */
