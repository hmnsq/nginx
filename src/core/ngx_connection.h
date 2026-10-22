
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#که_نه_تعریف_شوی _NGX_CONNECTION_H_INCLUDED_
#تعریف_کړه _NGX_CONNECTION_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


تعريفنوع بنية ngx_listening_s  ngx_listening_t;

بنية ngx_listening_s {
    ngx_socket_t        fd;

    بنية sockaddr    *sockaddr;
    socklen_t           socklen;    /* size of sockaddr */
    اندازه_t              addr_متن_max_len;
    ngx_str_t           addr_متن;

    صحيح                 type;
    صحيح                 protocol;

    صحيح                 backlog;
    صحيح                 rcvbuf;
    صحيح                 sndbuf;
#إذا (NGX_HAVE_KEEPALIVE_TUNABLE)
    صحيح                 keepidle;
    صحيح                 keepintvl;
    صحيح                 keepcnt;
#د_که_پای

    /* handler of accepted connection */
    ngx_connection_handler_pt   handler;

    فراغ               *servers;  /* array of ngx_http_in_addr_t, for example */

    ngx_log_t           لوګاریتم;
    ngx_log_t          *logp;

    اندازه_t              pool_اندازه;

    ngx_listening_t    *پخوانی;
    ngx_connection_t   *connection;

    ngx_rbtree_t        rbtree;
    ngx_rbtree_غوټه_t   sentinel;

    ngx_uint_t          کارګر;

    بینښه            open:1;
    بینښه            remain:1;
    بینښه            ignore:1;

    بینښه            bound:1;       /* already bound */
    بینښه            inherited:1;   /* inherited from previous process */
    بینښه            nonblocking_accept:1;
    بینښه            listen:1;
    بینښه            nonblocking:1;
    بینښه            shared:1;    /* shared between threads or processes */
    بینښه            addr_ntop:1;
    بینښه            wildcard:1;

#إذا (NGX_HAVE_INET6)
    بینښه            ipv6only:1;
#د_که_پای
    بینښه            reuseport:1;
    بینښه            جمع_کړه_reuseport:1;
    بینښه            keepalive:2;
    بینښه            quic:1;

    بینښه            change_protocol:1;

    بینښه            deferred_accept:1;
    بینښه            delete_deferred:1;
    بینښه            جمع_کړه_deferred:1;
#إذا (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
    حرف               *accept_filter;
#د_که_پای
#إذا (NGX_HAVE_SETFIB)
    صحيح                 setfib;
#د_که_پای

#إذا (NGX_HAVE_TCP_FASTOPEN)
    صحيح                 fastopen;
#د_که_پای

};


تعريفنوع تعداد {
    NGX_ERROR_ALERT = 0,
    NGX_ERROR_ERR,
    NGX_ERROR_INFO,
    NGX_ERROR_IGNORE_ECONNRESET,
    NGX_ERROR_IGNORE_EINVAL,
    NGX_ERROR_IGNORE_EMSGSIZE
} ngx_connection_log_error_e;


تعريفنوع تعداد {
    NGX_TCP_NODELAY_UNSET = 0,
    NGX_TCP_NODELAY_سېټ,
    NGX_TCP_NODELAY_DISABLED
} ngx_connection_tcp_nodelay_e;


تعريفنوع تعداد {
    NGX_TCP_NOPUSH_UNSET = 0,
    NGX_TCP_NOPUSH_سېټ,
    NGX_TCP_NOPUSH_DISABLED
} ngx_connection_tcp_nopush_e;


#تعریف_کړه NGX_LOWLEVEL_BUFFERED  0x0f
#تعریف_کړه NGX_SSL_BUFFERED       0x01
#تعریف_کړه NGX_HTTP_V2_BUFFERED   0x02


بنية ngx_connection_s {
    فراغ               *معلومات;
    ngx_event_t        *اقرأ;
    ngx_event_t        *اكتب;

    ngx_socket_t        fd;

    ngx_recv_pt         recv;
    ngx_send_pt         send;
    ngx_recv_chain_pt   recv_chain;
    ngx_send_chain_pt   send_chain;

    ngx_listening_t    *listening;

    off_t               لېږل_شوی;

    ngx_log_t          *لوګاریتم;

    ngx_pool_t         *pool;

    صحيح                 type;

    بنية sockaddr    *sockaddr;
    socklen_t           socklen;
    ngx_str_t           addr_متن;

    ngx_proxy_protocol_t  *proxy_protocol;

#إذا (NGX_QUIC || NGX_COMPAT)
    ngx_quic_بهیر_t     *quic;
#د_که_پای

#إذا (NGX_SSL || NGX_COMPAT)
    ngx_ssl_connection_t  *ssl;
#د_که_پای

    ngx_udp_connection_t  *udp;

    بنية sockaddr    *local_sockaddr;
    socklen_t           local_socklen;

    ngx_buf_t          *بفر;

    ngx_کتار_t         کتار;

    ngx_atomic_uint_t   شمېره;

    ngx_msec_t          پیل_وخت;
    ngx_uint_t          غوښتنې;

    بینښه            buffered:8;

    بینښه            log_error:3;     /* ngx_connection_log_error_e */

    بینښه            timedout:1;
    بینښه            error:1;
    بینښه            destroyed:1;
    بینښه            pipeline:1;

    بینښه            idle:1;
    بینښه            reusable:1;
    بینښه            close:1;
    بینښه            shared:1;

    بینښه            sendfile:1;
    بینښه            sndlowat:1;
    بینښه            tcp_nodelay:2;   /* ngx_connection_tcp_nodelay_e */
    بینښه            tcp_nopush:2;    /* ngx_connection_tcp_nopush_e */

    بینښه            need_وروستی_buf:1;
    بینښه            need_flush_buf:1;

#إذا (NGX_HAVE_SENDFILE_NODISKIO || NGX_COMPAT)
    بینښه            busy_شمېر:2;
#د_که_پای

    بینښه            ktls_recv:1;

#إذا (NGX_تارونه || NGX_COMPAT)
    ngx_thread_task_t  *sendfile_task;
#د_که_پای
};


#تعریف_کړه ngx_سېټ_connection_log(c, l)                                         \
                                                                             \
    c->لوګاریتم->file = l->file;                                                  \
    c->لوګاریتم->بل = l->بل;                                                  \
    c->لوګاریتم->writer = l->writer;                                              \
    c->لوګاریتم->wdata = l->wdata;                                                \
    إذا (!(c->لوګاریتم->log_level & NGX_LOG_DEBUG_CONNECTION)) {                   \
        c->لوګاریتم->log_level = l->log_level;                                    \
    }


ngx_listening_t *ngx_create_listening(ngx_conf_t *cf, بنية sockaddr *sockaddr,
    socklen_t socklen);
ngx_int_t ngx_clone_listening(ngx_دوره_t *دوره, ngx_listening_t *ls);
ngx_int_t ngx_سېټ_inherited_sockets(ngx_دوره_t *دوره);
ngx_int_t ngx_open_listening_sockets(ngx_دوره_t *دوره);
فراغ ngx_configure_listening_sockets(ngx_دوره_t *دوره);
فراغ ngx_close_listening_sockets(ngx_دوره_t *دوره);
فراغ ngx_close_connection(ngx_connection_t *c);
فراغ ngx_close_idle_connections(ngx_دوره_t *دوره);
ngx_int_t ngx_connection_local_sockaddr(ngx_connection_t *c, ngx_str_t *s,
    ngx_uint_t پورټ);
ngx_int_t ngx_tcp_nodelay(ngx_connection_t *c);
ngx_int_t ngx_connection_error(ngx_connection_t *c, ngx_err_t err, حرف *متن);

ngx_connection_t *ngx_get_connection(ngx_socket_t s, ngx_log_t *لوګاریتم);
فراغ ngx_free_connection(ngx_connection_t *c);

فراغ ngx_reusable_connection(ngx_connection_t *c, ngx_uint_t reusable);

#د_که_پای /* _NGX_CONNECTION_H_INCLUDED_ */
