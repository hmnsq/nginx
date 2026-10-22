
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#اگر_تعریف_نشده _NGX_CONNECTION_H_INCLUDED_
#تعریف_کن _NGX_CONNECTION_H_INCLUDED_


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
#پایان_اگر

    /* handler of accepted connection */
    ngx_connection_handler_pt   handler;

    فراغ               *servers;  /* array of ngx_http_in_addr_t, for example */

    ngx_log_t           لگاریتم;
    ngx_log_t          *logp;

    اندازه_t              pool_اندازه;

    ngx_listening_t    *قبلی;
    ngx_connection_t   *connection;

    ngx_rbtree_t        rbtree;
    ngx_rbtree_گره_t   sentinel;

    ngx_uint_t          کارگر;

    بیعلامت            open:1;
    بیعلامت            remain:1;
    بیعلامت            ignore:1;

    بیعلامت            bound:1;       /* already bound */
    بیعلامت            inherited:1;   /* inherited from previous process */
    بیعلامت            nonblocking_accept:1;
    بیعلامت            listen:1;
    بیعلامت            nonblocking:1;
    بیعلامت            shared:1;    /* shared between threads or processes */
    بیعلامت            addr_ntop:1;
    بیعلامت            wildcard:1;

#إذا (NGX_HAVE_INET6)
    بیعلامت            ipv6only:1;
#پایان_اگر
    بیعلامت            reuseport:1;
    بیعلامت            جمع_کن_reuseport:1;
    بیعلامت            keepalive:2;
    بیعلامت            quic:1;

    بیعلامت            change_protocol:1;

    بیعلامت            deferred_accept:1;
    بیعلامت            delete_deferred:1;
    بیعلامت            جمع_کن_deferred:1;
#إذا (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
    حرف               *accept_filter;
#پایان_اگر
#إذا (NGX_HAVE_SETFIB)
    صحيح                 setfib;
#پایان_اگر

#إذا (NGX_HAVE_TCP_FASTOPEN)
    صحيح                 fastopen;
#پایان_اگر

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
    NGX_TCP_NODELAY_مجموعه,
    NGX_TCP_NODELAY_DISABLED
} ngx_connection_tcp_nodelay_e;


تعريفنوع تعداد {
    NGX_TCP_NOPUSH_UNSET = 0,
    NGX_TCP_NOPUSH_مجموعه,
    NGX_TCP_NOPUSH_DISABLED
} ngx_connection_tcp_nopush_e;


#تعریف_کن NGX_LOWLEVEL_BUFFERED  0x0f
#تعریف_کن NGX_SSL_BUFFERED       0x01
#تعریف_کن NGX_HTTP_V2_BUFFERED   0x02


بنية ngx_connection_s {
    فراغ               *داده;
    ngx_event_t        *اقرأ;
    ngx_event_t        *اكتب;

    ngx_socket_t        fd;

    ngx_recv_pt         recv;
    ngx_send_pt         send;
    ngx_recv_chain_pt   recv_chain;
    ngx_send_chain_pt   send_chain;

    ngx_listening_t    *listening;

    off_t               فرستاده;

    ngx_log_t          *لگاریتم;

    ngx_pool_t         *pool;

    صحيح                 type;

    بنية sockaddr    *sockaddr;
    socklen_t           socklen;
    ngx_str_t           addr_متن;

    ngx_proxy_protocol_t  *proxy_protocol;

#إذا (NGX_QUIC || NGX_COMPAT)
    ngx_quic_جریان_t     *quic;
#پایان_اگر

#إذا (NGX_SSL || NGX_COMPAT)
    ngx_ssl_connection_t  *ssl;
#پایان_اگر

    ngx_udp_connection_t  *udp;

    بنية sockaddr    *local_sockaddr;
    socklen_t           local_socklen;

    ngx_buf_t          *میانگیر;

    ngx_صف_t         صف;

    ngx_atomic_uint_t   عدد;

    ngx_msec_t          شروع_زمان;
    ngx_uint_t          درخواستها;

    بیعلامت            buffered:8;

    بیعلامت            log_error:3;     /* ngx_connection_log_error_e */

    بیعلامت            timedout:1;
    بیعلامت            error:1;
    بیعلامت            destroyed:1;
    بیعلامت            pipeline:1;

    بیعلامت            idle:1;
    بیعلامت            reusable:1;
    بیعلامت            close:1;
    بیعلامت            shared:1;

    بیعلامت            sendfile:1;
    بیعلامت            sndlowat:1;
    بیعلامت            tcp_nodelay:2;   /* ngx_connection_tcp_nodelay_e */
    بیعلامت            tcp_nopush:2;    /* ngx_connection_tcp_nopush_e */

    بیعلامت            need_آخرین_buf:1;
    بیعلامت            need_flush_buf:1;

#إذا (NGX_HAVE_SENDFILE_NODISKIO || NGX_COMPAT)
    بیعلامت            busy_شمار:2;
#پایان_اگر

    بیعلامت            ktls_recv:1;

#إذا (NGX_نخها || NGX_COMPAT)
    ngx_thread_task_t  *sendfile_task;
#پایان_اگر
};


#تعریف_کن ngx_مجموعه_connection_log(c, l)                                         \
                                                                             \
    c->لگاریتم->file = l->file;                                                  \
    c->لگاریتم->بعدی = l->بعدی;                                                  \
    c->لگاریتم->writer = l->writer;                                              \
    c->لگاریتم->wdata = l->wdata;                                                \
    إذا (!(c->لگاریتم->log_level & NGX_LOG_DEBUG_CONNECTION)) {                   \
        c->لگاریتم->log_level = l->log_level;                                    \
    }


ngx_listening_t *ngx_create_listening(ngx_conf_t *cf, بنية sockaddr *sockaddr,
    socklen_t socklen);
ngx_int_t ngx_clone_listening(ngx_چرخه_t *چرخه, ngx_listening_t *ls);
ngx_int_t ngx_مجموعه_inherited_sockets(ngx_چرخه_t *چرخه);
ngx_int_t ngx_open_listening_sockets(ngx_چرخه_t *چرخه);
فراغ ngx_configure_listening_sockets(ngx_چرخه_t *چرخه);
فراغ ngx_close_listening_sockets(ngx_چرخه_t *چرخه);
فراغ ngx_close_connection(ngx_connection_t *c);
فراغ ngx_close_idle_connections(ngx_چرخه_t *چرخه);
ngx_int_t ngx_connection_local_sockaddr(ngx_connection_t *c, ngx_str_t *s,
    ngx_uint_t پورت);
ngx_int_t ngx_tcp_nodelay(ngx_connection_t *c);
ngx_int_t ngx_connection_error(ngx_connection_t *c, ngx_err_t err, حرف *متن);

ngx_connection_t *ngx_get_connection(ngx_socket_t s, ngx_log_t *لگاریتم);
فراغ ngx_free_connection(ngx_connection_t *c);

فراغ ngx_reusable_connection(ngx_connection_t *c, ngx_uint_t reusable);

#پایان_اگر /* _NGX_CONNECTION_H_INCLUDED_ */
