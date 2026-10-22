
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#جيڪڏهن_وصف_نه_ٿيل _NGX_CONNECTION_H_INCLUDED_
#وصف_ڪريو _NGX_CONNECTION_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


تعريفنوع بنية ngx_listening_s  ngx_listening_t;

بنية ngx_listening_s {
    ngx_socket_t        fd;

    بنية sockaddr    *sockaddr;
    socklen_t           socklen;    /* size of sockaddr */
    سائيز_t              addr_متن_max_len;
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
#جيڪڏهن_پورو

    /* handler of accepted connection */
    ngx_connection_handler_pt   handler;

    فراغ               *servers;  /* array of ngx_http_in_addr_t, for example */

    ngx_log_t           لاگرٿم;
    ngx_log_t          *logp;

    سائيز_t              pool_سائيز;

    ngx_listening_t    *اڳيون;
    ngx_connection_t   *connection;

    ngx_rbtree_t        rbtree;
    ngx_rbtree_ڳنڍ_t   sentinel;

    ngx_uint_t          ڪم_ڪندڙ;

    بنانشان            open:1;
    بنانشان            remain:1;
    بنانشان            ignore:1;

    بنانشان            bound:1;       /* already bound */
    بنانشان            inherited:1;   /* inherited from previous process */
    بنانشان            nonblocking_accept:1;
    بنانشان            listen:1;
    بنانشان            nonblocking:1;
    بنانشان            shared:1;    /* shared between threads or processes */
    بنانشان            addr_ntop:1;
    بنانشان            wildcard:1;

#إذا (NGX_HAVE_INET6)
    بنانشان            ipv6only:1;
#جيڪڏهن_پورو
    بنانشان            reuseport:1;
    بنانشان            جوڙيو_reuseport:1;
    بنانشان            keepalive:2;
    بنانشان            quic:1;

    بنانشان            change_protocol:1;

    بنانشان            deferred_accept:1;
    بنانشان            delete_deferred:1;
    بنانشان            جوڙيو_deferred:1;
#إذا (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
    حرف               *accept_filter;
#جيڪڏهن_پورو
#إذا (NGX_HAVE_SETFIB)
    صحيح                 setfib;
#جيڪڏهن_پورو

#إذا (NGX_HAVE_TCP_FASTOPEN)
    صحيح                 fastopen;
#جيڪڏهن_پورو

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
    NGX_TCP_NODELAY_سيٽ,
    NGX_TCP_NODELAY_DISABLED
} ngx_connection_tcp_nodelay_e;


تعريفنوع تعداد {
    NGX_TCP_NOPUSH_UNSET = 0,
    NGX_TCP_NOPUSH_سيٽ,
    NGX_TCP_NOPUSH_DISABLED
} ngx_connection_tcp_nopush_e;


#وصف_ڪريو NGX_LOWLEVEL_BUFFERED  0x0f
#وصف_ڪريو NGX_SSL_BUFFERED       0x01
#وصف_ڪريو NGX_HTTP_V2_BUFFERED   0x02


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

    off_t               موڪليل;

    ngx_log_t          *لاگرٿم;

    ngx_pool_t         *pool;

    صحيح                 type;

    بنية sockaddr    *sockaddr;
    socklen_t           socklen;
    ngx_str_t           addr_متن;

    ngx_proxy_protocol_t  *proxy_protocol;

#إذا (NGX_QUIC || NGX_COMPAT)
    ngx_quic_وهڪرو_t     *quic;
#جيڪڏهن_پورو

#إذا (NGX_SSL || NGX_COMPAT)
    ngx_ssl_connection_t  *ssl;
#جيڪڏهن_پورو

    ngx_udp_connection_t  *udp;

    بنية sockaddr    *local_sockaddr;
    socklen_t           local_socklen;

    ngx_buf_t          *بفر;

    ngx_قطار_t         قطار;

    ngx_atomic_uint_t   عدد;

    ngx_msec_t          شروعات_وقت;
    ngx_uint_t          درخواستون;

    بنانشان            buffered:8;

    بنانشان            log_error:3;     /* ngx_connection_log_error_e */

    بنانشان            timedout:1;
    بنانشان            error:1;
    بنانشان            destroyed:1;
    بنانشان            pipeline:1;

    بنانشان            idle:1;
    بنانشان            reusable:1;
    بنانشان            close:1;
    بنانشان            shared:1;

    بنانشان            sendfile:1;
    بنانشان            sndlowat:1;
    بنانشان            tcp_nodelay:2;   /* ngx_connection_tcp_nodelay_e */
    بنانشان            tcp_nopush:2;    /* ngx_connection_tcp_nopush_e */

    بنانشان            need_آخري_buf:1;
    بنانشان            need_flush_buf:1;

#إذا (NGX_HAVE_SENDFILE_NODISKIO || NGX_COMPAT)
    بنانشان            busy_ڳڻپ:2;
#جيڪڏهن_پورو

    بنانشان            ktls_recv:1;

#إذا (NGX_ڌاڳا || NGX_COMPAT)
    ngx_thread_task_t  *sendfile_task;
#جيڪڏهن_پورو
};


#وصف_ڪريو ngx_سيٽ_connection_log(c, l)                                         \
                                                                             \
    c->لاگرٿم->file = l->file;                                                  \
    c->لاگرٿم->ايندڙ = l->ايندڙ;                                                  \
    c->لاگرٿم->writer = l->writer;                                              \
    c->لاگرٿم->wdata = l->wdata;                                                \
    إذا (!(c->لاگرٿم->log_level & NGX_LOG_DEBUG_CONNECTION)) {                   \
        c->لاگرٿم->log_level = l->log_level;                                    \
    }


ngx_listening_t *ngx_create_listening(ngx_conf_t *cf, بنية sockaddr *sockaddr,
    socklen_t socklen);
ngx_int_t ngx_clone_listening(ngx_چڪر_t *چڪر, ngx_listening_t *ls);
ngx_int_t ngx_سيٽ_inherited_sockets(ngx_چڪر_t *چڪر);
ngx_int_t ngx_open_listening_sockets(ngx_چڪر_t *چڪر);
فراغ ngx_configure_listening_sockets(ngx_چڪر_t *چڪر);
فراغ ngx_close_listening_sockets(ngx_چڪر_t *چڪر);
فراغ ngx_close_connection(ngx_connection_t *c);
فراغ ngx_close_idle_connections(ngx_چڪر_t *چڪر);
ngx_int_t ngx_connection_local_sockaddr(ngx_connection_t *c, ngx_str_t *s,
    ngx_uint_t پورٽ);
ngx_int_t ngx_tcp_nodelay(ngx_connection_t *c);
ngx_int_t ngx_connection_error(ngx_connection_t *c, ngx_err_t err, حرف *متن);

ngx_connection_t *ngx_get_connection(ngx_socket_t s, ngx_log_t *لاگرٿم);
فراغ ngx_free_connection(ngx_connection_t *c);

فراغ ngx_reusable_connection(ngx_connection_t *c, ngx_uint_t reusable);

#جيڪڏهن_پورو /* _NGX_CONNECTION_H_INCLUDED_ */
