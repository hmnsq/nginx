
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


#جے_بنا_تعریف _NGX_RESOLVER_H_INCLUDED_
#تعریف_کرو _NGX_RESOLVER_H_INCLUDED_


#تعریف_کرو NGX_RESOLVE_A         1
#تعریف_کرو NGX_RESOLVE_CNAME     5
#تعریف_کرو NGX_RESOLVE_PTR       12
#تعریف_کرو NGX_RESOLVE_MX        15
#تعریف_کرو NGX_RESOLVE_TXT       16
#إذا (NGX_HAVE_INET6)
#تعریف_کرو NGX_RESOLVE_AAAA      28
#جے_انت
#تعریف_کرو NGX_RESOLVE_SRV       33
#تعریف_کرو NGX_RESOLVE_DNAME     39

#تعریف_کرو NGX_RESOLVE_FORMERR   1
#تعریف_کرو NGX_RESOLVE_SERVFAIL  2
#تعریف_کرو NGX_RESOLVE_NXDOMAIN  3
#تعریف_کرو NGX_RESOLVE_NOTIMP    4
#تعریف_کرو NGX_RESOLVE_REFUSED   5
#تعریف_کرو NGX_RESOLVE_TIMEDOUT  NGX_ETIMEDOUT


#تعریف_کرو NGX_NO_RESOLVER       (فراغ *) -1

#تعریف_کرو NGX_RESOLVER_MAX_RECURSION    50


تعريفنوع بنية ngx_resolver_s  ngx_resolver_t;


تعريفنوع بنية {
    ngx_connection_t         *udp;
    ngx_connection_t         *tcp;
    بنية sockaddr          *sockaddr;
    socklen_t                 socklen;
    ngx_str_t                 server;
    ngx_log_t                 لوگارتھم;
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
    u_short                   ترجیح;
    u_short                   بھار;
} ngx_resolver_addr_t;


تعريفنوع بنية {
    ngx_str_t                 name;
    u_short                   ترجیح;
    u_short                   بھار;
    u_short                   پورٹ;
} ngx_resolver_srv_t;


تعريفنوع بنية {
    ngx_str_t                 name;
    u_short                   ترجیح;
    u_short                   بھار;
    u_short                   پورٹ;

    ngx_resolver_ctx_t       *ctx;
    ngx_int_t                 state;

    ngx_uint_t                naddrs;
    ngx_addr_t               *addrs;
} ngx_resolver_srv_name_t;


تعريفنوع بنية {
    ngx_rbtree_گرہ_t         گرہ;
    ngx_قطار_t               قطار;

    /* PTR: resolved name, A: name to resolve */
    u_char                   *name;

#إذا (NGX_HAVE_INET6)
    /* PTR: IPv6 address to resolve (IPv4 address is in rbtree node key) */
    بنية in6_addr           addr6;
#جے_انت

    u_short                   nlen;
    u_short                   qlen;

    u_char                   *query;
#إذا (NGX_HAVE_INET6)
    u_char                   *query6;
#جے_انت

    اتحاد {
        وچ_addr_t             addr;
        وچ_addr_t            *addrs;
        u_char               *cname;
        ngx_resolver_srv_t   *srvs;
    } u;

    u_char                    کوڈ;
    u_short                   naddrs;
    u_short                   nsrvs;
    u_short                   cnlen;

#إذا (NGX_HAVE_INET6)
    اتحاد {
        بنية in6_addr       addr6;
        بنية in6_addr      *addrs6;
    } u6;

    u_short                   naddrs6;
#جے_انت

    ویلا_t                    expire;
    ویلا_t                    valid;
    uint32_t                  ttl;

    بنانشانی                  tcp:1;
#إذا (NGX_HAVE_INET6)
    بنانشانی                  tcp6:1;
#جے_انت

    ngx_uint_t                آخری_connection;

    ngx_resolver_ctx_t       *اڈیک;
} ngx_resolver_گرہ_t;


بنية ngx_resolver_s {
    /* has to be pointer because of "incomplete type" */
    ngx_event_t              *event;
    فراغ                     *dummy;
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

    بنانشانی                  ipv4:1;

#إذا (NGX_HAVE_INET6)
    بنانشانی                  ipv6:1;
    ngx_rbtree_t              addr6_rbtree;
    ngx_rbtree_گرہ_t         addr6_sentinel;
    ngx_قطار_t               addr6_resend_قطار;
    ngx_قطار_t               addr6_expire_قطار;
#جے_انت

    ویلا_t                    resend_timeout;
    ویلا_t                    tcp_timeout;
    ویلا_t                    expire;
    ویلا_t                    valid;

    ngx_uint_t                log_level;
};


بنية ngx_resolver_ctx_s {
    ngx_resolver_ctx_t       *اگلا;
    ngx_resolver_t           *resolver;
    ngx_resolver_گرہ_t      *گرہ;

    /* event ident must be after 3 pointers as in ngx_connection_t */
    ngx_int_t                 ident;

    ngx_int_t                 state;
    ngx_str_t                 name;
    ngx_str_t                 service;

    ویلا_t                    valid;
    ngx_uint_t                naddrs;
    ngx_resolver_addr_t      *addrs;
    ngx_resolver_addr_t       addr;
    بنية sockaddr_وچ        جیب;

    ngx_uint_t                گنتی;
    ngx_uint_t                nsrvs;
    ngx_resolver_srv_name_t  *srvs;

    ngx_resolver_handler_pt   handler;
    فراغ                     *انکڑے;
    ngx_msec_t                timeout;

    بنانشانی                  تیز:1;
    بنانشانی                  async:1;
    بنانشانی                  cancelable:1;
    ngx_uint_t                recursion;
    ngx_event_t              *event;
};


ngx_resolver_t *ngx_resolver_create(ngx_conf_t *cf, ngx_str_t *ناں,
    ngx_uint_t n);
ngx_resolver_ctx_t *ngx_resolve_شروع(ngx_resolver_t *r,
    ngx_resolver_ctx_t *عارضی);
ngx_int_t ngx_resolve_name(ngx_resolver_ctx_t *ctx);
فراغ ngx_resolve_name_پورا(ngx_resolver_ctx_t *ctx);
ngx_int_t ngx_resolve_addr(ngx_resolver_ctx_t *ctx);
فراغ ngx_resolve_addr_پورا(ngx_resolver_ctx_t *ctx);
حرف *ngx_resolver_strerror(ngx_int_t err);


#جے_انت /* _NGX_RESOLVER_H_INCLUDED_ */
