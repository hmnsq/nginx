
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ଅନ୍ତର୍ଭାଵ <ngx_config.h>
#ଅନ୍ତର୍ଭାଵ <ngx_core.h>


#ifndef _NGX_RESOLVER_H_INCLUDED_
#define _NGX_RESOLVER_H_INCLUDED_


#define NGX_RESOLVE_A         1
#define NGX_RESOLVE_CNAME     5
#define NGX_RESOLVE_PTR       12
#define NGX_RESOLVE_MX        15
#define NGX_RESOLVE_TXT       16
#ଯଦି (NGX_HAVE_INET6)
#define NGX_RESOLVE_AAAA      28
#endif
#define NGX_RESOLVE_SRV       33
#define NGX_RESOLVE_DNAME     39

#define NGX_RESOLVE_FORMERR   1
#define NGX_RESOLVE_SERVFAIL  2
#define NGX_RESOLVE_NXDOMAIN  3
#define NGX_RESOLVE_NOTIMP    4
#define NGX_RESOLVE_REFUSED   5
#define NGX_RESOLVE_TIMEDOUT  NGX_ETIMEDOUT


#define NGX_NO_RESOLVER       (ଶୂନ୍ୟ *) -1

#define NGX_RESOLVER_MAX_RECURSION    50


ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା ngx_resolver_s  ngx_resolver_t;


ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ngx_connection_t         *udp;
    ngx_connection_t         *tcp;
    ସଂରଚନା sockaddr          *sockaddr;
    socklen_t                 socklen;
    ngx_str_t                 server;
    ngx_log_t                 log;
    ngx_buf_t                *read_buf;
    ngx_buf_t                *write_buf;
    ngx_resolver_t           *resolver;
} ngx_resolver_connection_t;


ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା ngx_resolver_ctx_s  ngx_resolver_ctx_t;

ପ୍ରକାରବ୍ୟାଖ୍ୟା ଶୂନ୍ୟ (*ngx_resolver_handler_pt)(ngx_resolver_ctx_t *ctx);


ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ସଂରଚନା sockaddr          *sockaddr;
    socklen_t                 socklen;
    ngx_str_t                 name;
    u_short                   priority;
    u_short                   weight;
} ngx_resolver_addr_t;


ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ngx_str_t                 name;
    u_short                   priority;
    u_short                   weight;
    u_short                   port;
} ngx_resolver_srv_t;


ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ngx_str_t                 name;
    u_short                   priority;
    u_short                   weight;
    u_short                   port;

    ngx_resolver_ctx_t       *ctx;
    ngx_int_t                 state;

    ngx_uint_t                naddrs;
    ngx_addr_t               *addrs;
} ngx_resolver_srv_name_t;


ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ngx_rbtree_ଗଣ୍ଠି_t         ଗଣ୍ଠି;
    ngx_queue_t               queue;

    /* PTR: resolved name, A: name to resolve */
    u_char                   *name;

#ଯଦି (NGX_HAVE_INET6)
    /* PTR: IPv6 address to resolve (IPv4 address is in rbtree node key) */
    ସଂରଚନା in6_addr           addr6;
#endif

    u_short                   nlen;
    u_short                   qlen;

    u_char                   *query;
#ଯଦି (NGX_HAVE_INET6)
    u_char                   *query6;
#endif

    ସଂଘ {
        in_addr_t             addr;
        in_addr_t            *addrs;
        u_char               *cname;
        ngx_resolver_srv_t   *srvs;
    } u;

    u_char                    code;
    u_short                   naddrs;
    u_short                   nsrvs;
    u_short                   cnlen;

#ଯଦି (NGX_HAVE_INET6)
    ସଂଘ {
        ସଂରଚନା in6_addr       addr6;
        ସଂରଚନା in6_addr      *addrs6;
    } u6;

    u_short                   naddrs6;
#endif

    time_t                    expire;
    time_t                    valid;
    uint32_t                  ttl;

    unsigned                  tcp:1;
#ଯଦି (NGX_HAVE_INET6)
    unsigned                  tcp6:1;
#endif

    ngx_uint_t                last_connection;

    ngx_resolver_ctx_t       *waiting;
} ngx_resolver_ଗଣ୍ଠି_t;


ସଂରଚନା ngx_resolver_s {
    /* has to be pointer because of "incomplete type" */
    ngx_event_t              *event;
    ଶୂନ୍ୟ                     *dummy;
    ngx_log_t                *log;

    /* event ident must be after 3 pointers as in ngx_connection_t */
    ngx_int_t                 ident;

    /* simple round robin DNS peers balancer */
    ngx_array_t               connections;
    ngx_uint_t                last_connection;

    ngx_rbtree_t              name_rbtree;
    ngx_rbtree_ଗଣ୍ଠି_t         name_sentinel;

    ngx_rbtree_t              srv_rbtree;
    ngx_rbtree_ଗଣ୍ଠି_t         srv_sentinel;

    ngx_rbtree_t              addr_rbtree;
    ngx_rbtree_ଗଣ୍ଠି_t         addr_sentinel;

    ngx_queue_t               name_resend_queue;
    ngx_queue_t               srv_resend_queue;
    ngx_queue_t               addr_resend_queue;

    ngx_queue_t               name_expire_queue;
    ngx_queue_t               srv_expire_queue;
    ngx_queue_t               addr_expire_queue;

    unsigned                  ipv4:1;

#ଯଦି (NGX_HAVE_INET6)
    unsigned                  ipv6:1;
    ngx_rbtree_t              addr6_rbtree;
    ngx_rbtree_ଗଣ୍ଠି_t         addr6_sentinel;
    ngx_queue_t               addr6_resend_queue;
    ngx_queue_t               addr6_expire_queue;
#endif

    time_t                    resend_timeout;
    time_t                    tcp_timeout;
    time_t                    expire;
    time_t                    valid;

    ngx_uint_t                log_level;
};


ସଂରଚନା ngx_resolver_ctx_s {
    ngx_resolver_ctx_t       *next;
    ngx_resolver_t           *resolver;
    ngx_resolver_ଗଣ୍ଠି_t      *ଗଣ୍ଠି;

    /* event ident must be after 3 pointers as in ngx_connection_t */
    ngx_int_t                 ident;

    ngx_int_t                 state;
    ngx_str_t                 name;
    ngx_str_t                 service;

    time_t                    valid;
    ngx_uint_t                naddrs;
    ngx_resolver_addr_t      *addrs;
    ngx_resolver_addr_t       addr;
    ସଂରଚନା sockaddr_in        sin;

    ngx_uint_t                ଗଣନା_2;
    ngx_uint_t                nsrvs;
    ngx_resolver_srv_name_t  *srvs;

    ngx_resolver_handler_pt   handler;
    ଶୂନ୍ୟ                     *data;
    ngx_msec_t                timeout;

    unsigned                  quick:1;
    unsigned                  async:1;
    unsigned                  cancelable:1;
    ngx_uint_t                recursion;
    ngx_event_t              *event;
};


ngx_resolver_t *ngx_resolver_create(ngx_conf_t *cf, ngx_str_t *names,
    ngx_uint_t n);
ngx_resolver_ctx_t *ngx_resolve_start(ngx_resolver_t *r,
    ngx_resolver_ctx_t *temp);
ngx_int_t ngx_resolve_name(ngx_resolver_ctx_t *ctx);
ଶୂନ୍ୟ ngx_resolve_name_done(ngx_resolver_ctx_t *ctx);
ngx_int_t ngx_resolve_addr(ngx_resolver_ctx_t *ctx);
ଶୂନ୍ୟ ngx_resolve_addr_done(ngx_resolver_ctx_t *ctx);
ଅକ୍ଷର *ngx_resolver_strerror(ngx_int_t err);


#endif /* _NGX_RESOLVER_H_INCLUDED_ */
