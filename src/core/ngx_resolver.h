
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


#ifndef _NGX_RESOLVER_H_INCLUDED_
#define _NGX_RESOLVER_H_INCLUDED_


#define NGX_RESOLVE_A         1
#define NGX_RESOLVE_CNAME     5
#define NGX_RESOLVE_PTR       12
#define NGX_RESOLVE_MX        15
#define NGX_RESOLVE_TXT       16
#ಯದಿ (NGX_HAVE_INET6)
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


#define NGX_NO_RESOLVER       (ಶೂನ್ಯ *) -1

#define NGX_RESOLVER_MAX_RECURSION    50


ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ ngx_resolver_s  ngx_resolver_t;


ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    ngx_connection_t         *udp;
    ngx_connection_t         *tcp;
    ಸಂರಚನಾ sockaddr          *sockaddr;
    socklen_t                 socklen;
    ngx_str_t                 server;
    ngx_log_t                 log;
    ngx_buf_t                *read_buf;
    ngx_buf_t                *write_buf;
    ngx_resolver_t           *resolver;
} ngx_resolver_connection_t;


ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ ngx_resolver_ctx_s  ngx_resolver_ctx_t;

ಪ್ರಕಾರನಿರ್ದೇಶ ಶೂನ್ಯ (*ngx_resolver_handler_pt)(ngx_resolver_ctx_t *ctx);


ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    ಸಂರಚನಾ sockaddr          *sockaddr;
    socklen_t                 socklen;
    ngx_str_t                 name;
    u_short                   priority;
    u_short                   weight;
} ngx_resolver_addr_t;


ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    ngx_str_t                 name;
    u_short                   priority;
    u_short                   weight;
    u_short                   port;
} ngx_resolver_srv_t;


ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    ngx_str_t                 name;
    u_short                   priority;
    u_short                   weight;
    u_short                   port;

    ngx_resolver_ctx_t       *ctx;
    ngx_int_t                 state;

    ngx_uint_t                naddrs;
    ngx_addr_t               *addrs;
} ngx_resolver_srv_name_t;


ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    ngx_rbtree_ಗಂಟು_t         ಗಂಟು;
    ngx_queue_t               queue;

    /* PTR: resolved name, A: name to resolve */
    u_char                   *name;

#ಯದಿ (NGX_HAVE_INET6)
    /* PTR: IPv6 address to resolve (IPv4 address is in rbtree node key) */
    ಸಂರಚನಾ in6_addr           addr6;
#endif

    u_short                   nlen;
    u_short                   qlen;

    u_char                   *query;
#ಯದಿ (NGX_HAVE_INET6)
    u_char                   *query6;
#endif

    ಸಙ್ಘ {
        in_addr_t             addr;
        in_addr_t            *addrs;
        u_char               *cname;
        ngx_resolver_srv_t   *srvs;
    } u;

    u_char                    code;
    u_short                   naddrs;
    u_short                   nsrvs;
    u_short                   cnlen;

#ಯದಿ (NGX_HAVE_INET6)
    ಸಙ್ಘ {
        ಸಂರಚನಾ in6_addr       addr6;
        ಸಂರಚನಾ in6_addr      *addrs6;
    } u6;

    u_short                   naddrs6;
#endif

    time_t                    expire;
    time_t                    valid;
    uint32_t                  ttl;

    unsigned                  tcp:1;
#ಯದಿ (NGX_HAVE_INET6)
    unsigned                  tcp6:1;
#endif

    ngx_uint_t                last_connection;

    ngx_resolver_ctx_t       *waiting;
} ngx_resolver_ಗಂಟು_t;


ಸಂರಚನಾ ngx_resolver_s {
    /* has to be pointer because of "incomplete type" */
    ngx_event_t              *event;
    ಶೂನ್ಯ                     *dummy;
    ngx_log_t                *log;

    /* event ident must be after 3 pointers as in ngx_connection_t */
    ngx_int_t                 ident;

    /* simple round robin DNS peers balancer */
    ngx_array_t               connections;
    ngx_uint_t                last_connection;

    ngx_rbtree_t              name_rbtree;
    ngx_rbtree_ಗಂಟು_t         name_sentinel;

    ngx_rbtree_t              srv_rbtree;
    ngx_rbtree_ಗಂಟು_t         srv_sentinel;

    ngx_rbtree_t              addr_rbtree;
    ngx_rbtree_ಗಂಟು_t         addr_sentinel;

    ngx_queue_t               name_resend_queue;
    ngx_queue_t               srv_resend_queue;
    ngx_queue_t               addr_resend_queue;

    ngx_queue_t               name_expire_queue;
    ngx_queue_t               srv_expire_queue;
    ngx_queue_t               addr_expire_queue;

    unsigned                  ipv4:1;

#ಯದಿ (NGX_HAVE_INET6)
    unsigned                  ipv6:1;
    ngx_rbtree_t              addr6_rbtree;
    ngx_rbtree_ಗಂಟು_t         addr6_sentinel;
    ngx_queue_t               addr6_resend_queue;
    ngx_queue_t               addr6_expire_queue;
#endif

    time_t                    resend_timeout;
    time_t                    tcp_timeout;
    time_t                    expire;
    time_t                    valid;

    ngx_uint_t                log_level;
};


ಸಂರಚನಾ ngx_resolver_ctx_s {
    ngx_resolver_ctx_t       *next;
    ngx_resolver_t           *resolver;
    ngx_resolver_ಗಂಟು_t      *ಗಂಟು;

    /* event ident must be after 3 pointers as in ngx_connection_t */
    ngx_int_t                 ident;

    ngx_int_t                 state;
    ngx_str_t                 name;
    ngx_str_t                 service;

    time_t                    valid;
    ngx_uint_t                naddrs;
    ngx_resolver_addr_t      *addrs;
    ngx_resolver_addr_t       addr;
    ಸಂರಚನಾ sockaddr_in        sin;

    ngx_uint_t                ಎಣಿಕೆ;
    ngx_uint_t                nsrvs;
    ngx_resolver_srv_name_t  *srvs;

    ngx_resolver_handler_pt   handler;
    ಶೂನ್ಯ                     *data;
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
ಶೂನ್ಯ ngx_resolve_name_done(ngx_resolver_ctx_t *ctx);
ngx_int_t ngx_resolve_addr(ngx_resolver_ctx_t *ctx);
ಶೂನ್ಯ ngx_resolve_addr_done(ngx_resolver_ctx_t *ctx);
ವರ್ಣ *ngx_resolver_strerror(ngx_int_t err);


#endif /* _NGX_RESOLVER_H_INCLUDED_ */
