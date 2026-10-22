
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ _NGX_CONNECTION_H_INCLUDED_
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _NGX_CONNECTION_H_INCLUDED_


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ngx_listening_s  ngx_listening_t;

ਬਣਤਰ ngx_listening_s {
    ngx_socket_t        fd;

    ਬਣਤਰ sockaddr    *sockaddr;
    socklen_t           socklen;    /* size of sockaddr */
    ਮਾਪ_t              addr_ਪਾਠ_max_len;
    ngx_str_t           addr_ਪਾਠ;

    ਪੂਰਨ                 type;
    ਪੂਰਨ                 protocol;

    ਪੂਰਨ                 backlog;
    ਪੂਰਨ                 rcvbuf;
    ਪੂਰਨ                 sndbuf;
#ਜੇ (NGX_HAVE_KEEPALIVE_TUNABLE)
    ਪੂਰਨ                 keepidle;
    ਪੂਰਨ                 keepintvl;
    ਪੂਰਨ                 keepcnt;
#ਜੇ_ਅੰਤ

    /* handler of accepted connection */
    ngx_connection_handler_pt   handler;

    ਖਾਲੀ               *servers;  /* array of ngx_http_in_addr_t, for example */

    ngx_log_t           ਲਘੂਗਣਕ;
    ngx_log_t          *logp;

    ਮਾਪ_t              pool_ਮਾਪ;

    ngx_listening_t    *ਪਿਛਲਾ;
    ngx_connection_t   *connection;

    ngx_rbtree_t        rbtree;
    ngx_rbtree_ਗੰਢ_t   sentinel;

    ngx_uint_t          ਕਾਮਾ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ            open:1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ            remain:1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ            ignore:1;

    ਬਿਨਾਨਿਸ਼ਾਨੀ            bound:1;       /* already bound */
    ਬਿਨਾਨਿਸ਼ਾਨੀ            inherited:1;   /* inherited from previous process */
    ਬਿਨਾਨਿਸ਼ਾਨੀ            nonblocking_accept:1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ            listen:1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ            nonblocking:1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ            shared:1;    /* shared between threads or processes */
    ਬਿਨਾਨਿਸ਼ਾਨੀ            addr_ntop:1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ            wildcard:1;

#ਜੇ (NGX_HAVE_INET6)
    ਬਿਨਾਨਿਸ਼ਾਨੀ            ipv6only:1;
#ਜੇ_ਅੰਤ
    ਬਿਨਾਨਿਸ਼ਾਨੀ            reuseport:1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ            ਜੋੜੋ_reuseport:1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ            keepalive:2;
    ਬਿਨਾਨਿਸ਼ਾਨੀ            quic:1;

    ਬਿਨਾਨਿਸ਼ਾਨੀ            change_protocol:1;

    ਬਿਨਾਨਿਸ਼ਾਨੀ            deferred_accept:1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ            delete_deferred:1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ            ਜੋੜੋ_deferred:1;
#ਜੇ (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
    ਅੱਖਰ               *accept_filter;
#ਜੇ_ਅੰਤ
#ਜੇ (NGX_HAVE_SETFIB)
    ਪੂਰਨ                 setfib;
#ਜੇ_ਅੰਤ

#ਜੇ (NGX_HAVE_TCP_FASTOPEN)
    ਪੂਰਨ                 fastopen;
#ਜੇ_ਅੰਤ

};


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਗਿਣਤੀ {
    NGX_ERROR_ALERT = 0,
    NGX_ERROR_ERR,
    NGX_ERROR_INFO,
    NGX_ERROR_IGNORE_ECONNRESET,
    NGX_ERROR_IGNORE_EINVAL,
    NGX_ERROR_IGNORE_EMSGSIZE
} ngx_connection_log_error_e;


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਗਿਣਤੀ {
    NGX_TCP_NODELAY_UNSET = 0,
    NGX_TCP_NODELAY_ਸੈੱਟ,
    NGX_TCP_NODELAY_DISABLED
} ngx_connection_tcp_nodelay_e;


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਗਿਣਤੀ {
    NGX_TCP_NOPUSH_UNSET = 0,
    NGX_TCP_NOPUSH_ਸੈੱਟ,
    NGX_TCP_NOPUSH_DISABLED
} ngx_connection_tcp_nopush_e;


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_LOWLEVEL_BUFFERED  0x0f
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_SSL_BUFFERED       0x01
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_HTTP_V2_BUFFERED   0x02


ਬਣਤਰ ngx_connection_s {
    ਖਾਲੀ               *ਅੰਕੜੇ;
    ngx_event_t        *ਪਠ;
    ngx_event_t        *ਲਿਖ;

    ngx_socket_t        fd;

    ngx_recv_pt         recv;
    ngx_send_pt         send;
    ngx_recv_chain_pt   recv_chain;
    ngx_send_chain_pt   send_chain;

    ngx_listening_t    *listening;

    off_t               ਭੇਜਿਆ;

    ngx_log_t          *ਲਘੂਗਣਕ;

    ngx_pool_t         *pool;

    ਪੂਰਨ                 type;

    ਬਣਤਰ sockaddr    *sockaddr;
    socklen_t           socklen;
    ngx_str_t           addr_ਪਾਠ;

    ngx_proxy_protocol_t  *proxy_protocol;

#ਜੇ (NGX_QUIC || NGX_COMPAT)
    ngx_quic_ਧਾਰਾ_t     *quic;
#ਜੇ_ਅੰਤ

#ਜੇ (NGX_SSL || NGX_COMPAT)
    ngx_ssl_connection_t  *ssl;
#ਜੇ_ਅੰਤ

    ngx_udp_connection_t  *udp;

    ਬਣਤਰ sockaddr    *local_sockaddr;
    socklen_t           local_socklen;

    ngx_buf_t          *ਬਫਰ;

    ngx_ਕਤਾਰ_t         ਕਤਾਰ;

    ngx_atomic_uint_t   ਸੰਖਿਆ;

    ngx_msec_t          ਸ਼ੁਰੂ_ਸਮਾਂ;
    ngx_uint_t          ਬੇਨਤੀਆਂ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ            buffered:8;

    ਬਿਨਾਨਿਸ਼ਾਨੀ            log_error:3;     /* ngx_connection_log_error_e */

    ਬਿਨਾਨਿਸ਼ਾਨੀ            timedout:1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ            error:1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ            destroyed:1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ            pipeline:1;

    ਬਿਨਾਨਿਸ਼ਾਨੀ            idle:1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ            reusable:1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ            close:1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ            shared:1;

    ਬਿਨਾਨਿਸ਼ਾਨੀ            sendfile:1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ            sndlowat:1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ            tcp_nodelay:2;   /* ngx_connection_tcp_nodelay_e */
    ਬਿਨਾਨਿਸ਼ਾਨੀ            tcp_nopush:2;    /* ngx_connection_tcp_nopush_e */

    ਬਿਨਾਨਿਸ਼ਾਨੀ            need_ਆਖਰੀ_buf:1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ            need_flush_buf:1;

#ਜੇ (NGX_HAVE_SENDFILE_NODISKIO || NGX_COMPAT)
    ਬਿਨਾਨਿਸ਼ਾਨੀ            busy_ਗਿਣਤੀ:2;
#ਜੇ_ਅੰਤ

    ਬਿਨਾਨਿਸ਼ਾਨੀ            ktls_recv:1;

#ਜੇ (NGX_ਧਾਗੇ || NGX_COMPAT)
    ngx_thread_task_t  *sendfile_task;
#ਜੇ_ਅੰਤ
};


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_ਸੈੱਟ_connection_log(c, l)                                         \
                                                                             \
    c->ਲਘੂਗਣਕ->file = l->file;                                                  \
    c->ਲਘੂਗਣਕ->ਅਗਲਾ = l->ਅਗਲਾ;                                                  \
    c->ਲਘੂਗਣਕ->writer = l->writer;                                              \
    c->ਲਘੂਗਣਕ->wdata = l->wdata;                                                \
    ਜੇ (!(c->ਲਘੂਗਣਕ->log_level & NGX_LOG_DEBUG_CONNECTION)) {                   \
        c->ਲਘੂਗਣਕ->log_level = l->log_level;                                    \
    }


ngx_listening_t *ngx_create_listening(ngx_conf_t *cf, ਬਣਤਰ sockaddr *sockaddr,
    socklen_t socklen);
ngx_int_t ngx_clone_listening(ngx_ਚੱਕਰ_t *ਚੱਕਰ, ngx_listening_t *ls);
ngx_int_t ngx_ਸੈੱਟ_inherited_sockets(ngx_ਚੱਕਰ_t *ਚੱਕਰ);
ngx_int_t ngx_open_listening_sockets(ngx_ਚੱਕਰ_t *ਚੱਕਰ);
ਖਾਲੀ ngx_configure_listening_sockets(ngx_ਚੱਕਰ_t *ਚੱਕਰ);
ਖਾਲੀ ngx_close_listening_sockets(ngx_ਚੱਕਰ_t *ਚੱਕਰ);
ਖਾਲੀ ngx_close_connection(ngx_connection_t *c);
ਖਾਲੀ ngx_close_idle_connections(ngx_ਚੱਕਰ_t *ਚੱਕਰ);
ngx_int_t ngx_connection_local_sockaddr(ngx_connection_t *c, ngx_str_t *s,
    ngx_uint_t ਪੋਰਟ);
ngx_int_t ngx_tcp_nodelay(ngx_connection_t *c);
ngx_int_t ngx_connection_error(ngx_connection_t *c, ngx_err_t err, ਅੱਖਰ *ਪਾਠ);

ngx_connection_t *ngx_get_connection(ngx_socket_t s, ngx_log_t *ਲਘੂਗਣਕ);
ਖਾਲੀ ngx_free_connection(ngx_connection_t *c);

ਖਾਲੀ ngx_reusable_connection(ngx_connection_t *c, ngx_uint_t reusable);

#ਜੇ_ਅੰਤ /* _NGX_CONNECTION_H_INCLUDED_ */
