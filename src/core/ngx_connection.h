
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#यदि_अपरिभाषितम् _NGX_CONNECTION_H_INCLUDED_
#परिभाषय _NGX_CONNECTION_H_INCLUDED_


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


प्रकारनिर्देश संरचना ngx_listening_s  ngx_listening_t;

संरचना ngx_listening_s {
    ngx_socket_t        fd;

    संरचना sockaddr    *sockaddr;
    socklen_t           socklen;    /* size of sockaddr */
    आकार_t              addr_text_max_len;
    ngx_str_t           addr_text;

    पूर्णाङ्क                 type;
    पूर्णाङ्क                 protocol;

    पूर्णाङ्क                 backlog;
    पूर्णाङ्क                 rcvbuf;
    पूर्णाङ्क                 sndbuf;
#यदि (NGX_HAVE_KEEPALIVE_TUNABLE)
    पूर्णाङ्क                 keepidle;
    पूर्णाङ्क                 keepintvl;
    पूर्णाङ्क                 keepcnt;
#यदि_अन्तः

    /* handler of accepted connection */
    ngx_connection_handler_pt   handler;

    शून्य               *servers;  /* array of ngx_http_in_addr_t, for example */

    ngx_log_t           लघुगणकः;
    ngx_log_t          *logp;

    आकार_t              pool_आकार;

    ngx_listening_t    *previous;
    ngx_connection_t   *connection;

    ngx_rbtree_t        rbtree;
    ngx_rbtree_ग्रन्थिः_t   sentinel;

    ngx_uint_t          कार्यकर्ता;

    अचिह्नित            उद्घाटय:1;
    अचिह्नित            remain:1;
    अचिह्नित            ignore:1;

    अचिह्नित            bound:1;       /* already bound */
    अचिह्नित            inherited:1;   /* inherited from previous process */
    अचिह्नित            nonblocking_accept:1;
    अचिह्नित            listen:1;
    अचिह्नित            nonblocking:1;
    अचिह्नित            shared:1;    /* shared between threads or processes */
    अचिह्नित            addr_ntop:1;
    अचिह्नित            wildcard:1;

#यदि (NGX_HAVE_INET6)
    अचिह्नित            ipv6only:1;
#यदि_अन्तः
    अचिह्नित            reuseport:1;
    अचिह्नित            सङ्कलय_reuseport:1;
    अचिह्नित            keepalive:2;
    अचिह्नित            quic:1;

    अचिह्नित            change_protocol:1;

    अचिह्नित            deferred_accept:1;
    अचिह्नित            delete_deferred:1;
    अचिह्नित            सङ्कलय_deferred:1;
#यदि (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
    वर्ण               *accept_filter;
#यदि_अन्तः
#यदि (NGX_HAVE_SETFIB)
    पूर्णाङ्क                 setfib;
#यदि_अन्तः

#यदि (NGX_HAVE_TCP_FASTOPEN)
    पूर्णाङ्क                 fastopen;
#यदि_अन्तः

};


प्रकारनिर्देश गणना {
    NGX_ERROR_ALERT = 0,
    NGX_ERROR_ERR,
    NGX_ERROR_INFO,
    NGX_ERROR_IGNORE_ECONNRESET,
    NGX_ERROR_IGNORE_EINVAL,
    NGX_ERROR_IGNORE_EMSGSIZE
} ngx_connection_log_error_e;


प्रकारनिर्देश गणना {
    NGX_TCP_NODELAY_UNSET = 0,
    NGX_TCP_NODELAY_समुच्चयः,
    NGX_TCP_NODELAY_DISABLED
} ngx_connection_tcp_nodelay_e;


प्रकारनिर्देश गणना {
    NGX_TCP_NOPUSH_UNSET = 0,
    NGX_TCP_NOPUSH_समुच्चयः,
    NGX_TCP_NOPUSH_DISABLED
} ngx_connection_tcp_nopush_e;


#परिभाषय NGX_LOWLEVEL_BUFFERED  0x0f
#परिभाषय NGX_SSL_BUFFERED       0x01
#परिभाषय NGX_HTTP_V2_BUFFERED   0x02


संरचना ngx_connection_s {
    शून्य               *दत्त;
    ngx_event_t        *पठ;
    ngx_event_t        *लिख;

    ngx_socket_t        fd;

    ngx_recv_pt         recv;
    ngx_send_pt         send;
    ngx_recv_chain_pt   recv_chain;
    ngx_send_chain_pt   send_chain;

    ngx_listening_t    *listening;

    off_t               प्रेषितम्;

    ngx_log_t          *लघुगणकः;

    ngx_pool_t         *pool;

    पूर्णाङ्क                 type;

    संरचना sockaddr    *sockaddr;
    socklen_t           socklen;
    ngx_str_t           addr_text;

    ngx_proxy_protocol_t  *proxy_protocol;

#यदि (NGX_QUIC || NGX_COMPAT)
    ngx_quic_प्रवाहः_t     *quic;
#यदि_अन्तः

#यदि (NGX_SSL || NGX_COMPAT)
    ngx_ssl_connection_t  *ssl;
#यदि_अन्तः

    ngx_udp_connection_t  *udp;

    संरचना sockaddr    *local_sockaddr;
    socklen_t           local_socklen;

    ngx_buf_t          *बफर;

    ngx_queue_t         queue;

    ngx_atomic_uint_t   सङ्ख्या;

    ngx_msec_t          start_time;
    ngx_uint_t          अनुरोधाः;

    अचिह्नित            buffered:8;

    अचिह्नित            log_error:3;     /* ngx_connection_log_error_e */

    अचिह्नित            timedout:1;
    अचिह्नित            error:1;
    अचिह्नित            destroyed:1;
    अचिह्नित            pipeline:1;

    अचिह्नित            idle:1;
    अचिह्नित            reusable:1;
    अचिह्नित            close:1;
    अचिह्नित            shared:1;

    अचिह्नित            sendfile:1;
    अचिह्नित            sndlowat:1;
    अचिह्नित            tcp_nodelay:2;   /* ngx_connection_tcp_nodelay_e */
    अचिह्नित            tcp_nopush:2;    /* ngx_connection_tcp_nopush_e */

    अचिह्नित            need_चरमः_buf:1;
    अचिह्नित            need_flush_buf:1;

#यदि (NGX_HAVE_SENDFILE_NODISKIO || NGX_COMPAT)
    अचिह्नित            busy_गणनम्:2;
#यदि_अन्तः

    अचिह्नित            ktls_recv:1;

#यदि (NGX_तन्तवः || NGX_COMPAT)
    ngx_thread_task_t  *sendfile_task;
#यदि_अन्तः
};


#परिभाषय ngx_समुच्चयः_connection_log(c, l)                                         \
                                                                             \
    c->लघुगणकः->file = l->file;                                                  \
    c->लघुगणकः->अग्र = l->अग्र;                                                  \
    c->लघुगणकः->writer = l->writer;                                              \
    c->लघुगणकः->wdata = l->wdata;                                                \
    यदि (!(c->लघुगणकः->log_level & NGX_LOG_DEBUG_CONNECTION)) {                   \
        c->लघुगणकः->log_level = l->log_level;                                    \
    }


ngx_listening_t *ngx_create_listening(ngx_conf_t *cf, संरचना sockaddr *sockaddr,
    socklen_t socklen);
ngx_int_t ngx_clone_listening(ngx_आवृत्तिः_t *आवृत्तिः, ngx_listening_t *ls);
ngx_int_t ngx_समुच्चयः_inherited_sockets(ngx_आवृत्तिः_t *आवृत्तिः);
ngx_int_t ngx_open_listening_sockets(ngx_आवृत्तिः_t *आवृत्तिः);
शून्य ngx_configure_listening_sockets(ngx_आवृत्तिः_t *आवृत्तिः);
शून्य ngx_close_listening_sockets(ngx_आवृत्तिः_t *आवृत्तिः);
शून्य ngx_close_connection(ngx_connection_t *c);
शून्य ngx_close_idle_connections(ngx_आवृत्तिः_t *आवृत्तिः);
ngx_int_t ngx_connection_local_sockaddr(ngx_connection_t *c, ngx_str_t *s,
    ngx_uint_t पत्तनम्);
ngx_int_t ngx_tcp_nodelay(ngx_connection_t *c);
ngx_int_t ngx_connection_error(ngx_connection_t *c, ngx_err_t err, वर्ण *text);

ngx_connection_t *ngx_get_connection(ngx_socket_t s, ngx_log_t *लघुगणकः);
शून्य ngx_free_connection(ngx_connection_t *c);

शून्य ngx_reusable_connection(ngx_connection_t *c, ngx_uint_t reusable);

#यदि_अन्तः /* _NGX_CONNECTION_H_INCLUDED_ */
