
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_CONNECTION_H_INCLUDED_
#define _NGX_CONNECTION_H_INCLUDED_


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ ngx_listening_s  ngx_listening_t;

ಸಂರಚನಾ ngx_listening_s {
    ngx_socket_t        fd;

    ಸಂರಚನಾ sockaddr    *sockaddr;
    socklen_t           socklen;    /* size of sockaddr */
    size_t              addr_text_max_len;
    ngx_str_t           addr_text;

    ಪೂರ್ಣಾಙ್ಕ                 type;
    ಪೂರ್ಣಾಙ್ಕ                 protocol;

    ಪೂರ್ಣಾಙ್ಕ                 backlog;
    ಪೂರ್ಣಾಙ್ಕ                 rcvbuf;
    ಪೂರ್ಣಾಙ್ಕ                 sndbuf;
#ಯದಿ (NGX_HAVE_KEEPALIVE_TUNABLE)
    ಪೂರ್ಣಾಙ್ಕ                 keepidle;
    ಪೂರ್ಣಾಙ್ಕ                 keepintvl;
    ಪೂರ್ಣಾಙ್ಕ                 keepcnt;
#endif

    /* handler of accepted connection */
    ngx_connection_handler_pt   handler;

    ಶೂನ್ಯ               *servers;  /* array of ngx_http_in_addr_t, for example */

    ngx_log_t           log;
    ngx_log_t          *logp;

    size_t              pool_size;

    ngx_listening_t    *previous;
    ngx_connection_t   *connection;

    ngx_rbtree_t        rbtree;
    ngx_rbtree_ಗಂಟು_t   sentinel;

    ngx_uint_t          worker;

    unsigned            open:1;
    unsigned            remain:1;
    unsigned            ignore:1;

    unsigned            bound:1;       /* already bound */
    unsigned            inherited:1;   /* inherited from previous process */
    unsigned            nonblocking_accept:1;
    unsigned            listen:1;
    unsigned            nonblocking:1;
    unsigned            shared:1;    /* shared between threads or processes */
    unsigned            addr_ntop:1;
    unsigned            wildcard:1;

#ಯದಿ (NGX_HAVE_INET6)
    unsigned            ipv6only:1;
#endif
    unsigned            reuseport:1;
    unsigned            add_reuseport:1;
    unsigned            keepalive:2;
    unsigned            quic:1;

    unsigned            change_protocol:1;

    unsigned            deferred_accept:1;
    unsigned            delete_deferred:1;
    unsigned            add_deferred:1;
#ಯದಿ (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
    ವರ್ಣ               *accept_filter;
#endif
#ಯದಿ (NGX_HAVE_SETFIB)
    ಪೂರ್ಣಾಙ್ಕ                 setfib;
#endif

#ಯದಿ (NGX_HAVE_TCP_FASTOPEN)
    ಪೂರ್ಣಾಙ್ಕ                 fastopen;
#endif

};


ಪ್ರಕಾರನಿರ್ದೇಶ ಗಣನಾ {
    NGX_ERROR_ALERT = 0,
    NGX_ERROR_ERR,
    NGX_ERROR_INFO,
    NGX_ERROR_IGNORE_ECONNRESET,
    NGX_ERROR_IGNORE_EINVAL,
    NGX_ERROR_IGNORE_EMSGSIZE
} ngx_connection_log_error_e;


ಪ್ರಕಾರನಿರ್ದೇಶ ಗಣನಾ {
    NGX_TCP_NODELAY_UNSET = 0,
    NGX_TCP_NODELAY_SET,
    NGX_TCP_NODELAY_DISABLED
} ngx_connection_tcp_nodelay_e;


ಪ್ರಕಾರನಿರ್ದೇಶ ಗಣನಾ {
    NGX_TCP_NOPUSH_UNSET = 0,
    NGX_TCP_NOPUSH_SET,
    NGX_TCP_NOPUSH_DISABLED
} ngx_connection_tcp_nopush_e;


#define NGX_LOWLEVEL_BUFFERED  0x0f
#define NGX_SSL_BUFFERED       0x01
#define NGX_HTTP_V2_BUFFERED   0x02


ಸಂರಚನಾ ngx_connection_s {
    ಶೂನ್ಯ               *data;
    ngx_event_t        *ಪಠ;
    ngx_event_t        *ಲಿಖ;

    ngx_socket_t        fd;

    ngx_recv_pt         recv;
    ngx_send_pt         send;
    ngx_recv_chain_pt   recv_chain;
    ngx_send_chain_pt   send_chain;

    ngx_listening_t    *listening;

    off_t               sent;

    ngx_log_t          *log;

    ngx_pool_t         *pool;

    ಪೂರ್ಣಾಙ್ಕ                 type;

    ಸಂರಚನಾ sockaddr    *sockaddr;
    socklen_t           socklen;
    ngx_str_t           addr_text;

    ngx_proxy_protocol_t  *proxy_protocol;

#ಯದಿ (NGX_QUIC || NGX_COMPAT)
    ngx_quic_stream_t     *quic;
#endif

#ಯದಿ (NGX_SSL || NGX_COMPAT)
    ngx_ssl_connection_t  *ssl;
#endif

    ngx_udp_connection_t  *udp;

    ಸಂರಚನಾ sockaddr    *local_sockaddr;
    socklen_t           local_socklen;

    ngx_buf_t          *buffer;

    ngx_queue_t         queue;

    ngx_atomic_uint_t   ಸಂಖ್ಯೆ;

    ngx_msec_t          start_time;
    ngx_uint_t          requests;

    unsigned            buffered:8;

    unsigned            log_error:3;     /* ngx_connection_log_error_e */

    unsigned            timedout:1;
    unsigned            error:1;
    unsigned            destroyed:1;
    unsigned            pipeline:1;

    unsigned            idle:1;
    unsigned            reusable:1;
    unsigned            close:1;
    unsigned            shared:1;

    unsigned            sendfile:1;
    unsigned            sndlowat:1;
    unsigned            tcp_nodelay:2;   /* ngx_connection_tcp_nodelay_e */
    unsigned            tcp_nopush:2;    /* ngx_connection_tcp_nopush_e */

    unsigned            need_last_buf:1;
    unsigned            need_flush_buf:1;

#ಯದಿ (NGX_HAVE_SENDFILE_NODISKIO || NGX_COMPAT)
    unsigned            busy_ಎಣಿಕೆ:2;
#endif

    unsigned            ktls_recv:1;

#ಯದಿ (NGX_THREADS || NGX_COMPAT)
    ngx_thread_task_t  *sendfile_task;
#endif
};


#define ngx_set_connection_log(c, l)                                         \
                                                                             \
    c->log->file = l->file;                                                  \
    c->log->next = l->next;                                                  \
    c->log->writer = l->writer;                                              \
    c->log->wdata = l->wdata;                                                \
    ಯದಿ (!(c->log->log_level & NGX_LOG_DEBUG_CONNECTION)) {                   \
        c->log->log_level = l->log_level;                                    \
    }


ngx_listening_t *ngx_create_listening(ngx_conf_t *cf, ಸಂರಚನಾ sockaddr *sockaddr,
    socklen_t socklen);
ngx_int_t ngx_clone_listening(ngx_cycle_t *cycle, ngx_listening_t *ls);
ngx_int_t ngx_set_inherited_sockets(ngx_cycle_t *cycle);
ngx_int_t ngx_open_listening_sockets(ngx_cycle_t *cycle);
ಶೂನ್ಯ ngx_configure_listening_sockets(ngx_cycle_t *cycle);
ಶೂನ್ಯ ngx_close_listening_sockets(ngx_cycle_t *cycle);
ಶೂನ್ಯ ngx_close_connection(ngx_connection_t *c);
ಶೂನ್ಯ ngx_close_idle_connections(ngx_cycle_t *cycle);
ngx_int_t ngx_connection_local_sockaddr(ngx_connection_t *c, ngx_str_t *s,
    ngx_uint_t port);
ngx_int_t ngx_tcp_nodelay(ngx_connection_t *c);
ngx_int_t ngx_connection_error(ngx_connection_t *c, ngx_err_t err, ವರ್ಣ *text);

ngx_connection_t *ngx_get_connection(ngx_socket_t s, ngx_log_t *log);
ಶೂನ್ಯ ngx_free_connection(ngx_connection_t *c);

ಶೂನ್ಯ ngx_reusable_connection(ngx_connection_t *c, ngx_uint_t reusable);

#endif /* _NGX_CONNECTION_H_INCLUDED_ */
