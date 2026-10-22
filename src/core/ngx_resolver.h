
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#شامل <ngx_config.h>
#شامل <ngx_core.h>


#اگر_غیر_تعریف_شدہ _NGX_RESOLVER_H_INCLUDED_
#تعریف_کرو _NGX_RESOLVER_H_INCLUDED_


#تعریف_کرو NGX_RESOLVE_A         1
#تعریف_کرو NGX_RESOLVE_CNAME     5
#تعریف_کرو NGX_RESOLVE_PTR       12
#تعریف_کرو NGX_RESOLVE_MX        15
#تعریف_کرو NGX_RESOLVE_TXT       16
#اگر (NGX_HAVE_INET6)
#تعریف_کرو NGX_RESOLVE_AAAA      28
#اگر_ختم
#تعریف_کرو NGX_RESOLVE_SRV       33
#تعریف_کرو NGX_RESOLVE_DNAME     39

#تعریف_کرو NGX_RESOLVE_FORMERR   1
#تعریف_کرو NGX_RESOLVE_SERVFAIL  2
#تعریف_کرو NGX_RESOLVE_NXDOMAIN  3
#تعریف_کرو NGX_RESOLVE_NOTIMP    4
#تعریف_کرو NGX_RESOLVE_REFUSED   5
#تعریف_کرو NGX_RESOLVE_TIMEDOUT  NGX_ETIMEDOUT


#تعریف_کرو NGX_NO_RESOLVER       (خالی *) -1

#تعریف_کرو NGX_RESOLVER_MAX_RECURSION    50


قسمتعریف ساخت ngx_resolver_s  ngx_resolver_t;


قسمتعریف ساخت {
    ngx_connection_t         *udp;
    ngx_connection_t         *tcp;
    ساخت sockaddr          *sockaddr;
    socklen_t                 socklen;
    ngx_str_t                 server;
    ngx_log_t                 لوگارتھم;
    ngx_buf_t                *read_buf;
    ngx_buf_t                *write_buf;
    ngx_resolver_t           *resolver;
} ngx_resolver_connection_t;


قسمتعریف ساخت ngx_resolver_ctx_s  ngx_resolver_ctx_t;

قسمتعریف خالی (*ngx_resolver_handler_pt)(ngx_resolver_ctx_t *ctx);


قسمتعریف ساخت {
    ساخت sockaddr          *sockaddr;
    socklen_t                 socklen;
    ngx_str_t                 name;
    u_short                   ترجیح;
    u_short                   وزن;
} ngx_resolver_addr_t;


قسمتعریف ساخت {
    ngx_str_t                 name;
    u_short                   ترجیح;
    u_short                   وزن;
    u_short                   پورٹ;
} ngx_resolver_srv_t;


قسمتعریف ساخت {
    ngx_str_t                 name;
    u_short                   ترجیح;
    u_short                   وزن;
    u_short                   پورٹ;

    ngx_resolver_ctx_t       *ctx;
    ngx_int_t                 state;

    ngx_uint_t                naddrs;
    ngx_addr_t               *addrs;
} ngx_resolver_srv_name_t;


قسمتعریف ساخت {
    ngx_rbtree_گرہ_t         گرہ;
    ngx_قطار_t               قطار;

    /* PTR: resolved name, A: name to resolve */
    u_char                   *name;

#اگر (NGX_HAVE_INET6)
    /* PTR: IPv6 address to resolve (IPv4 address is in rbtree node key) */
    ساخت in6_addr           addr6;
#اگر_ختم

    u_short                   nlen;
    u_short                   qlen;

    u_char                   *query;
#اگر (NGX_HAVE_INET6)
    u_char                   *query6;
#اگر_ختم

    اتحاد {
        اندر_addr_t             addr;
        اندر_addr_t            *addrs;
        u_char               *cname;
        ngx_resolver_srv_t   *srvs;
    } u;

    u_char                    رمز;
    u_short                   naddrs;
    u_short                   nsrvs;
    u_short                   cnlen;

#اگر (NGX_HAVE_INET6)
    اتحاد {
        ساخت in6_addr       addr6;
        ساخت in6_addr      *addrs6;
    } u6;

    u_short                   naddrs6;
#اگر_ختم

    وقت_t                    expire;
    وقت_t                    valid;
    uint32_t                  ttl;

    غیرعلامتی                  tcp:1;
#اگر (NGX_HAVE_INET6)
    غیرعلامتی                  tcp6:1;
#اگر_ختم

    ngx_uint_t                آخری_connection;

    ngx_resolver_ctx_t       *انتظار;
} ngx_resolver_گرہ_t;


ساخت ngx_resolver_s {
    /* has to be pointer because of "incomplete type" */
    ngx_event_t              *event;
    خالی                     *dummy;
    ngx_log_t                *لوگارتھم;

    /* event ident must be after 3 pointers as in ngx_connection_t */
    ngx_int_t                 ident;

    /* simple round robin DNS peers balancer */
    ngx_array_t               connections;
    ngx_uint_t                آخری_connection;

    ngx_rbtree_t              name_rbtree;
    ngx_rbtree_گرہ_t         name_sentinel;

    ngx_rbtree_t              srv_rbtree;
    ngx_rbtree_گرہ_t         srv_sentinel;

    ngx_rbtree_t              addr_rbtree;
    ngx_rbtree_گرہ_t         addr_sentinel;

    ngx_قطار_t               name_resend_قطار;
    ngx_قطار_t               srv_resend_قطار;
    ngx_قطار_t               addr_resend_قطار;

    ngx_قطار_t               name_expire_قطار;
    ngx_قطار_t               srv_expire_قطار;
    ngx_قطار_t               addr_expire_قطار;

    غیرعلامتی                  ipv4:1;

#اگر (NGX_HAVE_INET6)
    غیرعلامتی                  ipv6:1;
    ngx_rbtree_t              addr6_rbtree;
    ngx_rbtree_گرہ_t         addr6_sentinel;
    ngx_قطار_t               addr6_resend_قطار;
    ngx_قطار_t               addr6_expire_قطار;
#اگر_ختم

    وقت_t                    resend_timeout;
    وقت_t                    tcp_timeout;
    وقت_t                    expire;
    وقت_t                    valid;

    ngx_uint_t                log_level;
};


ساخت ngx_resolver_ctx_s {
    ngx_resolver_ctx_t       *اگلا;
    ngx_resolver_t           *resolver;
    ngx_resolver_گرہ_t      *گرہ;

    /* event ident must be after 3 pointers as in ngx_connection_t */
    ngx_int_t                 ident;

    ngx_int_t                 state;
    ngx_str_t                 name;
    ngx_str_t                 service;

    وقت_t                    valid;
    ngx_uint_t                naddrs;
    ngx_resolver_addr_t      *addrs;
    ngx_resolver_addr_t       addr;
    ساخت sockaddr_اندر        جیب;

    ngx_uint_t                گنتی;
    ngx_uint_t                nsrvs;
    ngx_resolver_srv_name_t  *srvs;

    ngx_resolver_handler_pt   handler;
    خالی                     *ڈیٹا;
    ngx_msec_t                timeout;

    غیرعلامتی                  تیز:1;
    غیرعلامتی                  async:1;
    غیرعلامتی                  cancelable:1;
    ngx_uint_t                recursion;
    ngx_event_t              *event;
};


ngx_resolver_t *ngx_resolver_create(ngx_conf_t *cf, ngx_str_t *نام,
    ngx_uint_t n);
ngx_resolver_ctx_t *ngx_resolve_آغاز(ngx_resolver_t *r,
    ngx_resolver_ctx_t *عارضی);
ngx_int_t ngx_resolve_name(ngx_resolver_ctx_t *ctx);
خالی ngx_resolve_name_مکمل(ngx_resolver_ctx_t *ctx);
ngx_int_t ngx_resolve_addr(ngx_resolver_ctx_t *ctx);
خالی ngx_resolve_addr_مکمل(ngx_resolver_ctx_t *ctx);
حرف *ngx_resolver_strerror(ngx_int_t err);


#اگر_ختم /* _NGX_RESOLVER_H_INCLUDED_ */
