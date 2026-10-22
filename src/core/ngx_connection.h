
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#اگر_غیر_تعریف_شدہ _NGX_CONNECTION_H_INCLUDED_
#تعریف_کرو _NGX_CONNECTION_H_INCLUDED_


#شامل <ngx_config.h>
#شامل <ngx_core.h>


قسمتعریف ساخت ngx_listening_s  ngx_listening_t;

ساخت ngx_listening_s {
    ngx_socket_t        fd;

    ساخت sockaddr    *sockaddr;
    socklen_t           socklen;    /* size of sockaddr */
    سائز_t              addr_متن_max_len;
    ngx_str_t           addr_متن;

    صحیح                 type;
    صحیح                 protocol;

    صحیح                 backlog;
    صحیح                 rcvbuf;
    صحیح                 sndbuf;
#اگر (NGX_HAVE_KEEPALIVE_TUNABLE)
    صحیح                 keepidle;
    صحیح                 keepintvl;
    صحیح                 keepcnt;
#اگر_ختم

    /* handler of accepted connection */
    ngx_connection_handler_pt   handler;

    خالی               *servers;  /* array of ngx_http_in_addr_t, for example */

    ngx_log_t           لوگارتھم;
    ngx_log_t          *logp;

    سائز_t              pool_سائز;

    ngx_listening_t    *پچھلا_2;
    ngx_connection_t   *connection;

    ngx_rbtree_t        rbtree;
    ngx_rbtree_گرہ_t   sentinel;

    ngx_uint_t          کارکن;

    غیرعلامتی            open:1;
    غیرعلامتی            remain:1;
    غیرعلامتی            ignore:1;

    غیرعلامتی            bound:1;       /* already bound */
    غیرعلامتی            inherited:1;   /* inherited from previous process */
    غیرعلامتی            nonblocking_accept:1;
    غیرعلامتی            listen:1;
    غیرعلامتی            nonblocking:1;
    غیرعلامتی            shared:1;    /* shared between threads or processes */
    غیرعلامتی            addr_ntop:1;
    غیرعلامتی            wildcard:1;

#اگر (NGX_HAVE_INET6)
    غیرعلامتی            ipv6only:1;
#اگر_ختم
    غیرعلامتی            reuseport:1;
    غیرعلامتی            جوڑو_reuseport:1;
    غیرعلامتی            keepalive:2;
    غیرعلامتی            quic:1;

    غیرعلامتی            change_protocol:1;

    غیرعلامتی            deferred_accept:1;
    غیرعلامتی            delete_deferred:1;
    غیرعلامتی            جوڑو_deferred:1;
#اگر (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
    حرف               *accept_filter;
#اگر_ختم
#اگر (NGX_HAVE_SETFIB)
    صحیح                 setfib;
#اگر_ختم

#اگر (NGX_HAVE_TCP_FASTOPEN)
    صحیح                 fastopen;
#اگر_ختم

};


قسمتعریف شمار {
    NGX_ERROR_ALERT = 0,
    NGX_ERROR_ERR,
    NGX_ERROR_INFO,
    NGX_ERROR_IGNORE_ECONNRESET,
    NGX_ERROR_IGNORE_EINVAL,
    NGX_ERROR_IGNORE_EMSGSIZE
} ngx_connection_log_error_e;


قسمتعریف شمار {
    NGX_TCP_NODELAY_UNSET = 0,
    NGX_TCP_NODELAY_سیٹ,
    NGX_TCP_NODELAY_DISABLED
} ngx_connection_tcp_nodelay_e;


قسمتعریف شمار {
    NGX_TCP_NOPUSH_UNSET = 0,
    NGX_TCP_NOPUSH_سیٹ,
    NGX_TCP_NOPUSH_DISABLED
} ngx_connection_tcp_nopush_e;


#تعریف_کرو NGX_LOWLEVEL_BUFFERED  0x0f
#تعریف_کرو NGX_SSL_BUFFERED       0x01
#تعریف_کرو NGX_HTTP_V2_BUFFERED   0x02


ساخت ngx_connection_s {
    خالی               *ڈیٹا;
    ngx_event_t        *اقرأ;
    ngx_event_t        *اكتب;

    ngx_socket_t        fd;

    ngx_recv_pt         recv;
    ngx_send_pt         send;
    ngx_recv_chain_pt   recv_chain;
    ngx_send_chain_pt   send_chain;

    ngx_listening_t    *listening;

    off_t               بھیجا;

    ngx_log_t          *لوگارتھم;

    ngx_pool_t         *pool;

    صحیح                 type;

    ساخت sockaddr    *sockaddr;
    socklen_t           socklen;
    ngx_str_t           addr_متن;

    ngx_proxy_protocol_t  *proxy_protocol;

#اگر (NGX_QUIC || NGX_COMPAT)
    ngx_quic_دھارا_t     *quic;
#اگر_ختم

#اگر (NGX_SSL || NGX_COMPAT)
    ngx_ssl_connection_t  *ssl;
#اگر_ختم

    ngx_udp_connection_t  *udp;

    ساخت sockaddr    *local_sockaddr;
    socklen_t           local_socklen;

    ngx_buf_t          *بفر;

    ngx_قطار_t         قطار;

    ngx_atomic_uint_t   نمبر;

    ngx_msec_t          آغاز_وقت;
    ngx_uint_t          درخواستیں;

    غیرعلامتی            buffered:8;

    غیرعلامتی            log_error:3;     /* ngx_connection_log_error_e */

    غیرعلامتی            timedout:1;
    غیرعلامتی            error:1;
    غیرعلامتی            destroyed:1;
    غیرعلامتی            pipeline:1;

    غیرعلامتی            idle:1;
    غیرعلامتی            reusable:1;
    غیرعلامتی            close:1;
    غیرعلامتی            shared:1;

    غیرعلامتی            sendfile:1;
    غیرعلامتی            sndlowat:1;
    غیرعلامتی            tcp_nodelay:2;   /* ngx_connection_tcp_nodelay_e */
    غیرعلامتی            tcp_nopush:2;    /* ngx_connection_tcp_nopush_e */

    غیرعلامتی            need_آخری_buf:1;
    غیرعلامتی            need_flush_buf:1;

#اگر (NGX_HAVE_SENDFILE_NODISKIO || NGX_COMPAT)
    غیرعلامتی            busy_گنتی:2;
#اگر_ختم

    غیرعلامتی            ktls_recv:1;

#اگر (NGX_دھاگے || NGX_COMPAT)
    ngx_thread_task_t  *sendfile_task;
#اگر_ختم
};


#تعریف_کرو ngx_سیٹ_connection_log(c, l)                                         \
                                                                             \
    c->لوگارتھم->file = l->file;                                                  \
    c->لوگارتھم->اگلا = l->اگلا;                                                  \
    c->لوگارتھم->writer = l->writer;                                              \
    c->لوگارتھم->wdata = l->wdata;                                                \
    اگر (!(c->لوگارتھم->log_level & NGX_LOG_DEBUG_CONNECTION)) {                   \
        c->لوگارتھم->log_level = l->log_level;                                    \
    }


ngx_listening_t *ngx_create_listening(ngx_conf_t *cf, ساخت sockaddr *sockaddr,
    socklen_t socklen);
ngx_int_t ngx_clone_listening(ngx_چکر_t *چکر, ngx_listening_t *ls);
ngx_int_t ngx_سیٹ_inherited_sockets(ngx_چکر_t *چکر);
ngx_int_t ngx_open_listening_sockets(ngx_چکر_t *چکر);
خالی ngx_configure_listening_sockets(ngx_چکر_t *چکر);
خالی ngx_close_listening_sockets(ngx_چکر_t *چکر);
خالی ngx_close_connection(ngx_connection_t *c);
خالی ngx_close_idle_connections(ngx_چکر_t *چکر);
ngx_int_t ngx_connection_local_sockaddr(ngx_connection_t *c, ngx_str_t *s,
    ngx_uint_t پورٹ);
ngx_int_t ngx_tcp_nodelay(ngx_connection_t *c);
ngx_int_t ngx_connection_error(ngx_connection_t *c, ngx_err_t err, حرف *متن);

ngx_connection_t *ngx_get_connection(ngx_socket_t s, ngx_log_t *لوگارتھم);
خالی ngx_free_connection(ngx_connection_t *c);

خالی ngx_reusable_connection(ngx_connection_t *c, ngx_uint_t reusable);

#اگر_ختم /* _NGX_CONNECTION_H_INCLUDED_ */
