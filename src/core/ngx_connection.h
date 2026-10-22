
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#నిర్వచితం_కాకపోతే _NGX_CONNECTION_H_INCLUDED_
#నిర్వచించు _NGX_CONNECTION_H_INCLUDED_


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>


రకనిర్వచనం నిర్మాణం ngx_listening_s  ngx_listening_t;

నిర్మాణం ngx_listening_s {
    ngx_socket_t        fd;

    నిర్మాణం sockaddr    *sockaddr;
    socklen_t           socklen;    /* size of sockaddr */
    కొలత_t              addr_పాఠ్యం_max_len;
    ngx_str_t           addr_పాఠ్యం;

    పూర్ణాంకం                 type;
    పూర్ణాంకం                 protocol;

    పూర్ణాంకం                 backlog;
    పూర్ణాంకం                 rcvbuf;
    పూర్ణాంకం                 sndbuf;
#ఒకవేళ (NGX_HAVE_KEEPALIVE_TUNABLE)
    పూర్ణాంకం                 keepidle;
    పూర్ణాంకం                 keepintvl;
    పూర్ణాంకం                 keepcnt;
#అయితే_ముగింపు

    /* handler of accepted connection */
    ngx_connection_handler_pt   handler;

    శూన్య               *servers;  /* array of ngx_http_in_addr_t, for example */

    ngx_log_t           సంవర్గమానం;
    ngx_log_t          *logp;

    కొలత_t              pool_కొలత;

    ngx_listening_t    *మునుపటి;
    ngx_connection_t   *connection;

    ngx_rbtree_t        rbtree;
    ngx_rbtree_కణుపు_t   sentinel;

    ngx_uint_t          కార్మికుడు;

    గుర్తులేని            open:1;
    గుర్తులేని            remain:1;
    గుర్తులేని            ignore:1;

    గుర్తులేని            bound:1;       /* already bound */
    గుర్తులేని            inherited:1;   /* inherited from previous process */
    గుర్తులేని            nonblocking_accept:1;
    గుర్తులేని            listen:1;
    గుర్తులేని            nonblocking:1;
    గుర్తులేని            shared:1;    /* shared between threads or processes */
    గుర్తులేని            addr_ntop:1;
    గుర్తులేని            wildcard:1;

#ఒకవేళ (NGX_HAVE_INET6)
    గుర్తులేని            ipv6only:1;
#అయితే_ముగింపు
    గుర్తులేని            reuseport:1;
    గుర్తులేని            కూడు_reuseport:1;
    గుర్తులేని            keepalive:2;
    గుర్తులేని            quic:1;

    గుర్తులేని            change_protocol:1;

    గుర్తులేని            deferred_accept:1;
    గుర్తులేని            delete_deferred:1;
    గుర్తులేని            కూడు_deferred:1;
#ఒకవేళ (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
    అక్షరం               *accept_filter;
#అయితే_ముగింపు
#ఒకవేళ (NGX_HAVE_SETFIB)
    పూర్ణాంకం                 setfib;
#అయితే_ముగింపు

#ఒకవేళ (NGX_HAVE_TCP_FASTOPEN)
    పూర్ణాంకం                 fastopen;
#అయితే_ముగింపు

};


రకనిర్వచనం గణన {
    NGX_ERROR_ALERT = 0,
    NGX_ERROR_ERR,
    NGX_ERROR_INFO,
    NGX_ERROR_IGNORE_ECONNRESET,
    NGX_ERROR_IGNORE_EINVAL,
    NGX_ERROR_IGNORE_EMSGSIZE
} ngx_connection_log_error_e;


రకనిర్వచనం గణన {
    NGX_TCP_NODELAY_UNSET = 0,
    NGX_TCP_NODELAY_సమితి,
    NGX_TCP_NODELAY_DISABLED
} ngx_connection_tcp_nodelay_e;


రకనిర్వచనం గణన {
    NGX_TCP_NOPUSH_UNSET = 0,
    NGX_TCP_NOPUSH_సమితి,
    NGX_TCP_NOPUSH_DISABLED
} ngx_connection_tcp_nopush_e;


#నిర్వచించు NGX_LOWLEVEL_BUFFERED  0x0f
#నిర్వచించు NGX_SSL_BUFFERED       0x01
#నిర్వచించు NGX_HTTP_V2_BUFFERED   0x02


నిర్మాణం ngx_connection_s {
    శూన్య               *దత్తాంశం;
    ngx_event_t        *పఠ;
    ngx_event_t        *లిఖ;

    ngx_socket_t        fd;

    ngx_recv_pt         recv;
    ngx_send_pt         send;
    ngx_recv_chain_pt   recv_chain;
    ngx_send_chain_pt   send_chain;

    ngx_listening_t    *listening;

    off_t               పంపిన;

    ngx_log_t          *సంవర్గమానం;

    ngx_pool_t         *pool;

    పూర్ణాంకం                 type;

    నిర్మాణం sockaddr    *sockaddr;
    socklen_t           socklen;
    ngx_str_t           addr_పాఠ్యం;

    ngx_proxy_protocol_t  *proxy_protocol;

#ఒకవేళ (NGX_QUIC || NGX_COMPAT)
    ngx_quic_ప్రవాహం_t     *quic;
#అయితే_ముగింపు

#ఒకవేళ (NGX_SSL || NGX_COMPAT)
    ngx_ssl_connection_t  *ssl;
#అయితే_ముగింపు

    ngx_udp_connection_t  *udp;

    నిర్మాణం sockaddr    *local_sockaddr;
    socklen_t           local_socklen;

    ngx_buf_t          *బఫరు;

    ngx_వరుస_t         వరుస;

    ngx_atomic_uint_t   సంఖ్య;

    ngx_msec_t          ప్రారంభం_సమయం;
    ngx_uint_t          అభ్యర్థనలు;

    గుర్తులేని            buffered:8;

    గుర్తులేని            log_error:3;     /* ngx_connection_log_error_e */

    గుర్తులేని            timedout:1;
    గుర్తులేని            error:1;
    గుర్తులేని            destroyed:1;
    గుర్తులేని            pipeline:1;

    గుర్తులేని            idle:1;
    గుర్తులేని            reusable:1;
    గుర్తులేని            close:1;
    గుర్తులేని            shared:1;

    గుర్తులేని            sendfile:1;
    గుర్తులేని            sndlowat:1;
    గుర్తులేని            tcp_nodelay:2;   /* ngx_connection_tcp_nodelay_e */
    గుర్తులేని            tcp_nopush:2;    /* ngx_connection_tcp_nopush_e */

    గుర్తులేని            need_చివరి_buf:1;
    గుర్తులేని            need_flush_buf:1;

#ఒకవేళ (NGX_HAVE_SENDFILE_NODISKIO || NGX_COMPAT)
    గుర్తులేని            busy_లెక్క:2;
#అయితే_ముగింపు

    గుర్తులేని            ktls_recv:1;

#ఒకవేళ (NGX_దారాలు || NGX_COMPAT)
    ngx_thread_task_t  *sendfile_task;
#అయితే_ముగింపు
};


#నిర్వచించు ngx_సమితి_connection_log(c, l)                                         \
                                                                             \
    c->సంవర్గమానం->file = l->file;                                                  \
    c->సంవర్గమానం->తదుపరి = l->తదుపరి;                                                  \
    c->సంవర్గమానం->writer = l->writer;                                              \
    c->సంవర్గమానం->wdata = l->wdata;                                                \
    ఒకవేళ (!(c->సంవర్గమానం->log_level & NGX_LOG_DEBUG_CONNECTION)) {                   \
        c->సంవర్గమానం->log_level = l->log_level;                                    \
    }


ngx_listening_t *ngx_create_listening(ngx_conf_t *cf, నిర్మాణం sockaddr *sockaddr,
    socklen_t socklen);
ngx_int_t ngx_clone_listening(ngx_చక్రం_t *చక్రం, ngx_listening_t *ls);
ngx_int_t ngx_సమితి_inherited_sockets(ngx_చక్రం_t *చక్రం);
ngx_int_t ngx_open_listening_sockets(ngx_చక్రం_t *చక్రం);
శూన్య ngx_configure_listening_sockets(ngx_చక్రం_t *చక్రం);
శూన్య ngx_close_listening_sockets(ngx_చక్రం_t *చక్రం);
శూన్య ngx_close_connection(ngx_connection_t *c);
శూన్య ngx_close_idle_connections(ngx_చక్రం_t *చక్రం);
ngx_int_t ngx_connection_local_sockaddr(ngx_connection_t *c, ngx_str_t *s,
    ngx_uint_t పోర్టు);
ngx_int_t ngx_tcp_nodelay(ngx_connection_t *c);
ngx_int_t ngx_connection_error(ngx_connection_t *c, ngx_err_t err, అక్షరం *పాఠ్యం);

ngx_connection_t *ngx_get_connection(ngx_socket_t s, ngx_log_t *సంవర్గమానం);
శూన్య ngx_free_connection(ngx_connection_t *c);

శూన్య ngx_reusable_connection(ngx_connection_t *c, ngx_uint_t reusable);

#అయితే_ముగింపు /* _NGX_CONNECTION_H_INCLUDED_ */
