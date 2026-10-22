
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ _NGX_CONNECTION_H_INCLUDED_
#ವ್ಯಾಖ್ಯಾನಿಸು _NGX_CONNECTION_H_INCLUDED_


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ ngx_listening_s  ngx_listening_t;

ರಚನೆ ngx_listening_s {
    ngx_socket_t        fd;

    ರಚನೆ sockaddr    *sockaddr;
    socklen_t           socklen;    /* size of sockaddr */
    ಗಾತ್ರ_t              addr_ಪಠ್ಯ_max_len;
    ngx_str_t           addr_ಪಠ್ಯ;

    ಪೂರ್ಣಾಂಕ                 type;
    ಪೂರ್ಣಾಂಕ                 protocol;

    ಪೂರ್ಣಾಂಕ                 backlog;
    ಪೂರ್ಣಾಂಕ                 rcvbuf;
    ಪೂರ್ಣಾಂಕ                 sndbuf;
#ಆದರೆ (NGX_HAVE_KEEPALIVE_TUNABLE)
    ಪೂರ್ಣಾಂಕ                 keepidle;
    ಪೂರ್ಣಾಂಕ                 keepintvl;
    ಪೂರ್ಣಾಂಕ                 keepcnt;
#ಆದರೆ_ಅಂತ್ಯ

    /* handler of accepted connection */
    ngx_connection_handler_pt   handler;

    ಶೂನ್ಯ               *servers;  /* array of ngx_http_in_addr_t, for example */

    ngx_log_t           ಲಘುಗಣಕ;
    ngx_log_t          *logp;

    ಗಾತ್ರ_t              pool_ಗಾತ್ರ;

    ngx_listening_t    *ಹಿಂದಿನ;
    ngx_connection_t   *connection;

    ngx_rbtree_t        rbtree;
    ngx_rbtree_ಗಂಟು_t   sentinel;

    ngx_uint_t          ಕೆಲಸಗಾರ;

    ಚಿಹ್ನೆರಹಿತ            open:1;
    ಚಿಹ್ನೆರಹಿತ            remain:1;
    ಚಿಹ್ನೆರಹಿತ            ignore:1;

    ಚಿಹ್ನೆರಹಿತ            bound:1;       /* already bound */
    ಚಿಹ್ನೆರಹಿತ            inherited:1;   /* inherited from previous process */
    ಚಿಹ್ನೆರಹಿತ            nonblocking_accept:1;
    ಚಿಹ್ನೆರಹಿತ            listen:1;
    ಚಿಹ್ನೆರಹಿತ            nonblocking:1;
    ಚಿಹ್ನೆರಹಿತ            shared:1;    /* shared between threads or processes */
    ಚಿಹ್ನೆರಹಿತ            addr_ntop:1;
    ಚಿಹ್ನೆರಹಿತ            wildcard:1;

#ಆದರೆ (NGX_HAVE_INET6)
    ಚಿಹ್ನೆರಹಿತ            ipv6only:1;
#ಆದರೆ_ಅಂತ್ಯ
    ಚಿಹ್ನೆರಹಿತ            reuseport:1;
    ಚಿಹ್ನೆರಹಿತ            ಕೂಡಿಸು_reuseport:1;
    ಚಿಹ್ನೆರಹಿತ            keepalive:2;
    ಚಿಹ್ನೆರಹಿತ            quic:1;

    ಚಿಹ್ನೆರಹಿತ            change_protocol:1;

    ಚಿಹ್ನೆರಹಿತ            deferred_accept:1;
    ಚಿಹ್ನೆರಹಿತ            delete_deferred:1;
    ಚಿಹ್ನೆರಹಿತ            ಕೂಡಿಸು_deferred:1;
#ಆದರೆ (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
    ಅಕ್ಷರ               *accept_filter;
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ (NGX_HAVE_SETFIB)
    ಪೂರ್ಣಾಂಕ                 setfib;
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ (NGX_HAVE_TCP_FASTOPEN)
    ಪೂರ್ಣಾಂಕ                 fastopen;
#ಆದರೆ_ಅಂತ್ಯ

};


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಎಣಿಕೆ {
    NGX_ERROR_ALERT = 0,
    NGX_ERROR_ERR,
    NGX_ERROR_INFO,
    NGX_ERROR_IGNORE_ECONNRESET,
    NGX_ERROR_IGNORE_EINVAL,
    NGX_ERROR_IGNORE_EMSGSIZE
} ngx_connection_log_error_e;


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಎಣಿಕೆ {
    NGX_TCP_NODELAY_UNSET = 0,
    NGX_TCP_NODELAY_ಗಣ,
    NGX_TCP_NODELAY_DISABLED
} ngx_connection_tcp_nodelay_e;


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಎಣಿಕೆ {
    NGX_TCP_NOPUSH_UNSET = 0,
    NGX_TCP_NOPUSH_ಗಣ,
    NGX_TCP_NOPUSH_DISABLED
} ngx_connection_tcp_nopush_e;


#ವ್ಯಾಖ್ಯಾನಿಸು NGX_LOWLEVEL_BUFFERED  0x0f
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_SSL_BUFFERED       0x01
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_HTTP_V2_BUFFERED   0x02


ರಚನೆ ngx_connection_s {
    ಶೂನ್ಯ               *ದತ್ತಾಂಶ;
    ngx_event_t        *ಪಠ;
    ngx_event_t        *ಲಿಖ;

    ngx_socket_t        fd;

    ngx_recv_pt         recv;
    ngx_send_pt         send;
    ngx_recv_chain_pt   recv_chain;
    ngx_send_chain_pt   send_chain;

    ngx_listening_t    *listening;

    off_t               ಕಳುಹಿಸಿದ;

    ngx_log_t          *ಲಘುಗಣಕ;

    ngx_pool_t         *pool;

    ಪೂರ್ಣಾಂಕ                 type;

    ರಚನೆ sockaddr    *sockaddr;
    socklen_t           socklen;
    ngx_str_t           addr_ಪಠ್ಯ;

    ngx_proxy_protocol_t  *proxy_protocol;

#ಆದರೆ (NGX_QUIC || NGX_COMPAT)
    ngx_quic_ಹರಿವು_t     *quic;
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ (NGX_SSL || NGX_COMPAT)
    ngx_ssl_connection_t  *ssl;
#ಆದರೆ_ಅಂತ್ಯ

    ngx_udp_connection_t  *udp;

    ರಚನೆ sockaddr    *local_sockaddr;
    socklen_t           local_socklen;

    ngx_buf_t          *ಬಫರ್;

    ngx_ಸರತಿ_t         ಸರತಿ;

    ngx_atomic_uint_t   ಸಂಖ್ಯೆ;

    ngx_msec_t          ಆರಂಭ_ಸಮಯ;
    ngx_uint_t          ವಿನಂತಿಗಳು;

    ಚಿಹ್ನೆರಹಿತ            buffered:8;

    ಚಿಹ್ನೆರಹಿತ            log_error:3;     /* ngx_connection_log_error_e */

    ಚಿಹ್ನೆರಹಿತ            timedout:1;
    ಚಿಹ್ನೆರಹಿತ            error:1;
    ಚಿಹ್ನೆರಹಿತ            destroyed:1;
    ಚಿಹ್ನೆರಹಿತ            pipeline:1;

    ಚಿಹ್ನೆರಹಿತ            idle:1;
    ಚಿಹ್ನೆರಹಿತ            reusable:1;
    ಚಿಹ್ನೆರಹಿತ            close:1;
    ಚಿಹ್ನೆರಹಿತ            shared:1;

    ಚಿಹ್ನೆರಹಿತ            sendfile:1;
    ಚಿಹ್ನೆರಹಿತ            sndlowat:1;
    ಚಿಹ್ನೆರಹಿತ            tcp_nodelay:2;   /* ngx_connection_tcp_nodelay_e */
    ಚಿಹ್ನೆರಹಿತ            tcp_nopush:2;    /* ngx_connection_tcp_nopush_e */

    ಚಿಹ್ನೆರಹಿತ            need_ಕೊನೆಯ_buf:1;
    ಚಿಹ್ನೆರಹಿತ            need_flush_buf:1;

#ಆದರೆ (NGX_HAVE_SENDFILE_NODISKIO || NGX_COMPAT)
    ಚಿಹ್ನೆರಹಿತ            busy_ಎಣಿಕೆ:2;
#ಆದರೆ_ಅಂತ್ಯ

    ಚಿಹ್ನೆರಹಿತ            ktls_recv:1;

#ಆದರೆ (NGX_ಎಳೆಗಳು || NGX_COMPAT)
    ngx_thread_task_t  *sendfile_task;
#ಆದರೆ_ಅಂತ್ಯ
};


#ವ್ಯಾಖ್ಯಾನಿಸು ngx_ಗಣ_connection_log(c, l)                                         \
                                                                             \
    c->ಲಘುಗಣಕ->file = l->file;                                                  \
    c->ಲಘುಗಣಕ->ಮುಂದಿನ = l->ಮುಂದಿನ;                                                  \
    c->ಲಘುಗಣಕ->writer = l->writer;                                              \
    c->ಲಘುಗಣಕ->wdata = l->wdata;                                                \
    ಆದರೆ (!(c->ಲಘುಗಣಕ->log_level & NGX_LOG_DEBUG_CONNECTION)) {                   \
        c->ಲಘುಗಣಕ->log_level = l->log_level;                                    \
    }


ngx_listening_t *ngx_create_listening(ngx_conf_t *cf, ರಚನೆ sockaddr *sockaddr,
    socklen_t socklen);
ngx_int_t ngx_clone_listening(ngx_ಚಕ್ರ_t *ಚಕ್ರ, ngx_listening_t *ls);
ngx_int_t ngx_ಗಣ_inherited_sockets(ngx_ಚಕ್ರ_t *ಚಕ್ರ);
ngx_int_t ngx_open_listening_sockets(ngx_ಚಕ್ರ_t *ಚಕ್ರ);
ಶೂನ್ಯ ngx_configure_listening_sockets(ngx_ಚಕ್ರ_t *ಚಕ್ರ);
ಶೂನ್ಯ ngx_close_listening_sockets(ngx_ಚಕ್ರ_t *ಚಕ್ರ);
ಶೂನ್ಯ ngx_close_connection(ngx_connection_t *c);
ಶೂನ್ಯ ngx_close_idle_connections(ngx_ಚಕ್ರ_t *ಚಕ್ರ);
ngx_int_t ngx_connection_local_sockaddr(ngx_connection_t *c, ngx_str_t *s,
    ngx_uint_t ಪೋರ್ಟ್);
ngx_int_t ngx_tcp_nodelay(ngx_connection_t *c);
ngx_int_t ngx_connection_error(ngx_connection_t *c, ngx_err_t err, ಅಕ್ಷರ *ಪಠ್ಯ);

ngx_connection_t *ngx_get_connection(ngx_socket_t s, ngx_log_t *ಲಘುಗಣಕ);
ಶೂನ್ಯ ngx_free_connection(ngx_connection_t *c);

ಶೂನ್ಯ ngx_reusable_connection(ngx_connection_t *c, ngx_uint_t reusable);

#ಆದರೆ_ಅಂತ್ಯ /* _NGX_CONNECTION_H_INCLUDED_ */
