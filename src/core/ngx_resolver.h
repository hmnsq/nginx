
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


#جيڪڏهن_وصف_نه_ٿيل _NGX_RESOLVER_H_INCLUDED_
#وصف_ڪريو _NGX_RESOLVER_H_INCLUDED_


#وصف_ڪريو NGX_RESOLVE_A         1
#وصف_ڪريو NGX_RESOLVE_CNAME     5
#وصف_ڪريو NGX_RESOLVE_PTR       12
#وصف_ڪريو NGX_RESOLVE_MX        15
#وصف_ڪريو NGX_RESOLVE_TXT       16
#إذا (NGX_HAVE_INET6)
#وصف_ڪريو NGX_RESOLVE_AAAA      28
#جيڪڏهن_پورو
#وصف_ڪريو NGX_RESOLVE_SRV       33
#وصف_ڪريو NGX_RESOLVE_DNAME     39

#وصف_ڪريو NGX_RESOLVE_FORMERR   1
#وصف_ڪريو NGX_RESOLVE_SERVFAIL  2
#وصف_ڪريو NGX_RESOLVE_NXDOMAIN  3
#وصف_ڪريو NGX_RESOLVE_NOTIMP    4
#وصف_ڪريو NGX_RESOLVE_REFUSED   5
#وصف_ڪريو NGX_RESOLVE_TIMEDOUT  NGX_ETIMEDOUT


#وصف_ڪريو NGX_NO_RESOLVER       (فراغ *) -1

#وصف_ڪريو NGX_RESOLVER_MAX_RECURSION    50


تعريفنوع بنية ngx_resolver_s  ngx_resolver_t;


تعريفنوع بنية {
    ngx_connection_t         *udp;
    ngx_connection_t         *tcp;
    بنية sockaddr          *sockaddr;
    socklen_t                 socklen;
    ngx_str_t                 server;
    ngx_log_t                 لاگرٿم;
    ngx_buf_t                *read_buf;
    ngx_buf_t                *write_buf;
    ngx_resolver_t           *resolver;
} ngx_resolver_connection_t;


تعريفنوع بنية ngx_resolver_ctx_s  ngx_resolver_ctx_t;

تعريفنوع فراغ (*ngx_resolver_handler_pt)(ngx_resolver_ctx_t *ctx);


تعريفنوع بنية {
    بنية sockaddr          *sockaddr;
    socklen_t                 socklen;
    ngx_str_t                 name;
    u_short                   ترجيح;
    u_short                   وزن;
} ngx_resolver_addr_t;


تعريفنوع بنية {
    ngx_str_t                 name;
    u_short                   ترجيح;
    u_short                   وزن;
    u_short                   پورٽ;
} ngx_resolver_srv_t;


تعريفنوع بنية {
    ngx_str_t                 name;
    u_short                   ترجيح;
    u_short                   وزن;
    u_short                   پورٽ;

    ngx_resolver_ctx_t       *ctx;
    ngx_int_t                 state;

    ngx_uint_t                naddrs;
    ngx_addr_t               *addrs;
} ngx_resolver_srv_name_t;


تعريفنوع بنية {
    ngx_rbtree_ڳنڍ_t         ڳنڍ;
    ngx_قطار_t               قطار;

    /* PTR: resolved name, A: name to resolve */
    u_char                   *name;

#إذا (NGX_HAVE_INET6)
    /* PTR: IPv6 address to resolve (IPv4 address is in rbtree node key) */
    بنية in6_addr           addr6;
#جيڪڏهن_پورو

    u_short                   nlen;
    u_short                   qlen;

    u_char                   *query;
#إذا (NGX_HAVE_INET6)
    u_char                   *query6;
#جيڪڏهن_پورو

    اتحاد {
        اندريون_addr_t             addr;
        اندريون_addr_t            *addrs;
        u_char               *cname;
        ngx_resolver_srv_t   *srvs;
    } u;

    u_char                    ڪوڊ;
    u_short                   naddrs;
    u_short                   nsrvs;
    u_short                   cnlen;

#إذا (NGX_HAVE_INET6)
    اتحاد {
        بنية in6_addr       addr6;
        بنية in6_addr      *addrs6;
    } u6;

    u_short                   naddrs6;
#جيڪڏهن_پورو

    وقت_t                    expire;
    وقت_t                    valid;
    uint32_t                  ttl;

    بنانشان                  tcp:1;
#إذا (NGX_HAVE_INET6)
    بنانشان                  tcp6:1;
#جيڪڏهن_پورو

    ngx_uint_t                آخري_connection;

    ngx_resolver_ctx_t       *انتظار;
} ngx_resolver_ڳنڍ_t;


بنية ngx_resolver_s {
    /* has to be pointer because of "incomplete type" */
    ngx_event_t              *event;
    فراغ                     *dummy;
    ngx_log_t                *لاگرٿم;

    /* event ident must be after 3 pointers as in ngx_connection_t */
    ngx_int_t                 ident;

    /* simple round robin DNS peers balancer */
    ngx_array_t               connections;
    ngx_uint_t                آخري_connection;

    ngx_rbtree_t              name_rbtree;
    ngx_rbtree_ڳنڍ_t         name_sentinel;

    ngx_rbtree_t              srv_rbtree;
    ngx_rbtree_ڳنڍ_t         srv_sentinel;

    ngx_rbtree_t              addr_rbtree;
    ngx_rbtree_ڳنڍ_t         addr_sentinel;

    ngx_قطار_t               name_resend_قطار;
    ngx_قطار_t               srv_resend_قطار;
    ngx_قطار_t               addr_resend_قطار;

    ngx_قطار_t               name_expire_قطار;
    ngx_قطار_t               srv_expire_قطار;
    ngx_قطار_t               addr_expire_قطار;

    بنانشان                  ipv4:1;

#إذا (NGX_HAVE_INET6)
    بنانشان                  ipv6:1;
    ngx_rbtree_t              addr6_rbtree;
    ngx_rbtree_ڳنڍ_t         addr6_sentinel;
    ngx_قطار_t               addr6_resend_قطار;
    ngx_قطار_t               addr6_expire_قطار;
#جيڪڏهن_پورو

    وقت_t                    resend_timeout;
    وقت_t                    tcp_timeout;
    وقت_t                    expire;
    وقت_t                    valid;

    ngx_uint_t                log_level;
};


بنية ngx_resolver_ctx_s {
    ngx_resolver_ctx_t       *ايندڙ;
    ngx_resolver_t           *resolver;
    ngx_resolver_ڳنڍ_t      *ڳنڍ;

    /* event ident must be after 3 pointers as in ngx_connection_t */
    ngx_int_t                 ident;

    ngx_int_t                 state;
    ngx_str_t                 name;
    ngx_str_t                 service;

    وقت_t                    valid;
    ngx_uint_t                naddrs;
    ngx_resolver_addr_t      *addrs;
    ngx_resolver_addr_t       addr;
    بنية sockaddr_اندريون        جيب;

    ngx_uint_t                ڳڻپ;
    ngx_uint_t                nsrvs;
    ngx_resolver_srv_name_t  *srvs;

    ngx_resolver_handler_pt   handler;
    فراغ                     *معلومات;
    ngx_msec_t                timeout;

    بنانشان                  تيز:1;
    بنانشان                  async:1;
    بنانشان                  cancelable:1;
    ngx_uint_t                recursion;
    ngx_event_t              *event;
};


ngx_resolver_t *ngx_resolver_create(ngx_conf_t *cf, ngx_str_t *نالا,
    ngx_uint_t n);
ngx_resolver_ctx_t *ngx_resolve_شروعات(ngx_resolver_t *r,
    ngx_resolver_ctx_t *عارضي);
ngx_int_t ngx_resolve_name(ngx_resolver_ctx_t *ctx);
فراغ ngx_resolve_name_پورو(ngx_resolver_ctx_t *ctx);
ngx_int_t ngx_resolve_addr(ngx_resolver_ctx_t *ctx);
فراغ ngx_resolve_addr_پورو(ngx_resolver_ctx_t *ctx);
حرف *ngx_resolver_strerror(ngx_int_t err);


#جيڪڏهن_پورو /* _NGX_RESOLVER_H_INCLUDED_ */
