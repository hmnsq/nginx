
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#جے_بنا_تعریف _NGX_CONNECTION_H_INCLUDED_
#تعریف_کرو _NGX_CONNECTION_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


تعريفنوع بنية ngx_listening_s  ngx_listening_t;

بنية ngx_listening_s {
    ngx_socket_t        fd;

    بنية sockaddr    *sockaddr;
    socklen_t           socklen;    /* size of sockaddr */
    ناپ_t              addr_متن_max_len;
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
#جے_انت

    /* handler of accepted connection */
    ngx_connection_handler_pt   handler;

    فراغ               *servers;  /* array of ngx_http_in_addr_t, for example */

    ngx_log_t           لوگارتھم;
    ngx_log_t          *logp;

    ناپ_t              pool_ناپ;

    ngx_listening_t    *پچھلا;
    ngx_connection_t   *connection;

    ngx_rbtree_t        rbtree;
    ngx_rbtree_گرہ_t   sentinel;

    ngx_uint_t          کامہ;

    بنانشانی            open:1;
    بنانشانی            remain:1;
    بنانشانی            ignore:1;

    بنانشانی            bound:1;       /* already bound */
    بنانشانی            inherited:1;   /* inherited from previous process */
    بنانشانی            nonblocking_accept:1;
    بنانشانی            listen:1;
    بنانشانی            nonblocking:1;
    بنانشانی            shared:1;    /* shared between threads or processes */
    بنانشانی            addr_ntop:1;
    بنانشانی            wildcard:1;

#إذا (NGX_HAVE_INET6)
    بنانشانی            ipv6only:1;
#جے_انت
    بنانشانی            reuseport:1;
    بنانشانی            جوڑو_reuseport:1;
    بنانشانی            keepalive:2;
    بنانشانی            quic:1;

    بنانشانی            change_protocol:1;

    بنانشانی            deferred_accept:1;
    بنانشانی            delete_deferred:1;
    بنانشانی            جوڑو_deferred:1;
#إذا (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
    حرف               *accept_filter;
#جے_انت
#إذا (NGX_HAVE_SETFIB)
    صحيح                 setfib;
#جے_انت

#إذا (NGX_HAVE_TCP_FASTOPEN)
    صحيح                 fastopen;
#جے_انت

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
    NGX_TCP_NODELAY_سیٹ,
    NGX_TCP_NODELAY_DISABLED
} ngx_connection_tcp_nodelay_e;


تعريفنوع تعداد {
    NGX_TCP_NOPUSH_UNSET = 0,
    NGX_TCP_NOPUSH_سیٹ,
    NGX_TCP_NOPUSH_DISABLED
} ngx_connection_tcp_nopush_e;


#تعریف_کرو NGX_LOWLEVEL_BUFFERED  0x0f
#تعریف_کرو NGX_SSL_BUFFERED       0x01
#تعریف_کرو NGX_HTTP_V2_BUFFERED   0x02


بنية ngx_connection_s {
    فراغ               *انکڑے;
    ngx_event_t        *اقرأ;
    ngx_event_t        *اكتب;

    ngx_socket_t        fd;

    ngx_recv_pt         recv;
    ngx_send_pt         send;
    ngx_recv_chain_pt   recv_chain;
    ngx_send_chain_pt   send_chain;

    ngx_listening_t    *listening;

    off_t               گھلیا;

    ngx_log_t          *لوگارتھم;

    ngx_pool_t         *pool;

    صحيح                 type;

    بنية sockaddr    *sockaddr;
    socklen_t           socklen;
    ngx_str_t           addr_متن;

    ngx_proxy_protocol_t  *proxy_protocol;

#إذا (NGX_QUIC || NGX_COMPAT)
    ngx_quic_دھارا_t     *quic;
#جے_انت

#إذا (NGX_SSL || NGX_COMPAT)
    ngx_ssl_connection_t  *ssl;
#جے_انت

    ngx_udp_connection_t  *udp;

    بنية sockaddr    *local_sockaddr;
    socklen_t           local_socklen;

    ngx_buf_t          *بفر;

    ngx_قطار_t         قطار;

    ngx_atomic_uint_t   نمبر;

    ngx_msec_t          شروع_ویلا;
    ngx_uint_t          بینتیاں;

    بنانشانی            buffered:8;

    بنانشانی            log_error:3;     /* ngx_connection_log_error_e */

    بنانشانی            timedout:1;
    بنانشانی            error:1;
    بنانشانی            destroyed:1;
    بنانشانی            pipeline:1;

    بنانشانی            idle:1;
    بنانشانی            reusable:1;
    بنانشانی            close:1;
    بنانشانی            shared:1;

    بنانشانی            sendfile:1;
    بنانشانی            sndlowat:1;
    بنانشانی            tcp_nodelay:2;   /* ngx_connection_tcp_nodelay_e */
    بنانشانی            tcp_nopush:2;    /* ngx_connection_tcp_nopush_e */

    بنانشانی            need_آخری_buf:1;
    بنانشانی            need_flush_buf:1;

#إذا (NGX_HAVE_SENDFILE_NODISKIO || NGX_COMPAT)
    بنانشانی            busy_گنتی:2;
#جے_انت

    بنانشانی            ktls_recv:1;

#إذا (NGX_دھاگے || NGX_COMPAT)
    ngx_thread_task_t  *sendfile_task;
#جے_انت
};


#تعریف_کرو ngx_سیٹ_connection_log(c, l)                                         \
                                                                             \
    c->لوگارتھم->file = l->file;                                                  \
    c->لوگارتھم->اگلا = l->اگلا;                                                  \
    c->لوگارتھم->writer = l->writer;                                              \
    c->لوگارتھم->wdata = l->wdata;                                                \
    إذا (!(c->لوگارتھم->log_level & NGX_LOG_DEBUG_CONNECTION)) {                   \
        c->لوگارتھم->log_level = l->log_level;                                    \
    }


ngx_listening_t *ngx_create_listening(ngx_conf_t *cf, بنية sockaddr *sockaddr,
    socklen_t socklen);
ngx_int_t ngx_clone_listening(ngx_چکر_t *چکر, ngx_listening_t *ls);
ngx_int_t ngx_سیٹ_inherited_sockets(ngx_چکر_t *چکر);
ngx_int_t ngx_open_listening_sockets(ngx_چکر_t *چکر);
فراغ ngx_configure_listening_sockets(ngx_چکر_t *چکر);
فراغ ngx_close_listening_sockets(ngx_چکر_t *چکر);
فراغ ngx_close_connection(ngx_connection_t *c);
فراغ ngx_close_idle_connections(ngx_چکر_t *چکر);
ngx_int_t ngx_connection_local_sockaddr(ngx_connection_t *c, ngx_str_t *s,
    ngx_uint_t پورٹ);
ngx_int_t ngx_tcp_nodelay(ngx_connection_t *c);
ngx_int_t ngx_connection_error(ngx_connection_t *c, ngx_err_t err, حرف *متن);

ngx_connection_t *ngx_get_connection(ngx_socket_t s, ngx_log_t *لوگارتھم);
فراغ ngx_free_connection(ngx_connection_t *c);

فراغ ngx_reusable_connection(ngx_connection_t *c, ngx_uint_t reusable);

#جے_انت /* _NGX_CONNECTION_H_INCLUDED_ */
