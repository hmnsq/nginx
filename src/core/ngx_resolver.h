
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ _NGX_RESOLVER_H_INCLUDED_
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _NGX_RESOLVER_H_INCLUDED_


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_RESOLVE_A         1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_RESOLVE_CNAME     5
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_RESOLVE_PTR       12
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_RESOLVE_MX        15
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_RESOLVE_TXT       16
#ਜੇ (NGX_HAVE_INET6)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_RESOLVE_AAAA      28
#ਜੇ_ਅੰਤ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_RESOLVE_SRV       33
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_RESOLVE_DNAME     39

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_RESOLVE_FORMERR   1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_RESOLVE_SERVFAIL  2
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_RESOLVE_NXDOMAIN  3
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_RESOLVE_NOTIMP    4
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_RESOLVE_REFUSED   5
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_RESOLVE_TIMEDOUT  NGX_ETIMEDOUT


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_NO_RESOLVER       (ਖਾਲੀ *) -1

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_RESOLVER_MAX_RECURSION    50


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ngx_resolver_s  ngx_resolver_t;


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ngx_connection_t         *udp;
    ngx_connection_t         *tcp;
    ਬਣਤਰ sockaddr          *sockaddr;
    socklen_t                 socklen;
    ngx_str_t                 server;
    ngx_log_t                 ਲਘੂਗਣਕ;
    ngx_buf_t                *read_buf;
    ngx_buf_t                *write_buf;
    ngx_resolver_t           *resolver;
} ngx_resolver_connection_t;


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ngx_resolver_ctx_s  ngx_resolver_ctx_t;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (*ngx_resolver_handler_pt)(ngx_resolver_ctx_t *ctx);


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ਬਣਤਰ sockaddr          *sockaddr;
    socklen_t                 socklen;
    ngx_str_t                 name;
    u_short                   ਤਰਜੀਹ;
    u_short                   ਭਾਰ;
} ngx_resolver_addr_t;


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ngx_str_t                 name;
    u_short                   ਤਰਜੀਹ;
    u_short                   ਭਾਰ;
    u_short                   ਪੋਰਟ;
} ngx_resolver_srv_t;


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ngx_str_t                 name;
    u_short                   ਤਰਜੀਹ;
    u_short                   ਭਾਰ;
    u_short                   ਪੋਰਟ;

    ngx_resolver_ctx_t       *ctx;
    ngx_int_t                 state;

    ngx_uint_t                naddrs;
    ngx_addr_t               *addrs;
} ngx_resolver_srv_name_t;


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ngx_rbtree_ਗੰਢ_t         ਗੰਢ;
    ngx_ਕਤਾਰ_t               ਕਤਾਰ;

    /* PTR: resolved name, A: name to resolve */
    u_char                   *name;

#ਜੇ (NGX_HAVE_INET6)
    /* PTR: IPv6 address to resolve (IPv4 address is in rbtree node key) */
    ਬਣਤਰ in6_addr           addr6;
#ਜੇ_ਅੰਤ

    u_short                   nlen;
    u_short                   qlen;

    u_char                   *query;
#ਜੇ (NGX_HAVE_INET6)
    u_char                   *query6;
#ਜੇ_ਅੰਤ

    ਸੰਘ {
        ਵਿੱਚ_addr_t             addr;
        ਵਿੱਚ_addr_t            *addrs;
        u_char               *cname;
        ngx_resolver_srv_t   *srvs;
    } u;

    u_char                    ਕੋਡ;
    u_short                   naddrs;
    u_short                   nsrvs;
    u_short                   cnlen;

#ਜੇ (NGX_HAVE_INET6)
    ਸੰਘ {
        ਬਣਤਰ in6_addr       addr6;
        ਬਣਤਰ in6_addr      *addrs6;
    } u6;

    u_short                   naddrs6;
#ਜੇ_ਅੰਤ

    ਸਮਾਂ_t                    expire;
    ਸਮਾਂ_t                    valid;
    uint32_t                  ttl;

    ਬਿਨਾਨਿਸ਼ਾਨੀ                  tcp:1;
#ਜੇ (NGX_HAVE_INET6)
    ਬਿਨਾਨਿਸ਼ਾਨੀ                  tcp6:1;
#ਜੇ_ਅੰਤ

    ngx_uint_t                ਆਖਰੀ_connection;

    ngx_resolver_ctx_t       *ਉਡੀਕ;
} ngx_resolver_ਗੰਢ_t;


ਬਣਤਰ ngx_resolver_s {
    /* has to be pointer because of "incomplete type" */
    ngx_event_t              *event;
    ਖਾਲੀ                     *dummy;
    ngx_log_t                *ਲਘੂਗਣਕ;

    /* event ident must be after 3 pointers as in ngx_connection_t */
    ngx_int_t                 ident;

    /* simple round robin DNS peers balancer */
    ngx_array_t               connections;
    ngx_uint_t                ਆਖਰੀ_connection;

    ngx_rbtree_t              name_rbtree;
    ngx_rbtree_ਗੰਢ_t         name_sentinel;

    ngx_rbtree_t              srv_rbtree;
    ngx_rbtree_ਗੰਢ_t         srv_sentinel;

    ngx_rbtree_t              addr_rbtree;
    ngx_rbtree_ਗੰਢ_t         addr_sentinel;

    ngx_ਕਤਾਰ_t               name_resend_ਕਤਾਰ;
    ngx_ਕਤਾਰ_t               srv_resend_ਕਤਾਰ;
    ngx_ਕਤਾਰ_t               addr_resend_ਕਤਾਰ;

    ngx_ਕਤਾਰ_t               name_expire_ਕਤਾਰ;
    ngx_ਕਤਾਰ_t               srv_expire_ਕਤਾਰ;
    ngx_ਕਤਾਰ_t               addr_expire_ਕਤਾਰ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ                  ipv4:1;

#ਜੇ (NGX_HAVE_INET6)
    ਬਿਨਾਨਿਸ਼ਾਨੀ                  ipv6:1;
    ngx_rbtree_t              addr6_rbtree;
    ngx_rbtree_ਗੰਢ_t         addr6_sentinel;
    ngx_ਕਤਾਰ_t               addr6_resend_ਕਤਾਰ;
    ngx_ਕਤਾਰ_t               addr6_expire_ਕਤਾਰ;
#ਜੇ_ਅੰਤ

    ਸਮਾਂ_t                    resend_timeout;
    ਸਮਾਂ_t                    tcp_timeout;
    ਸਮਾਂ_t                    expire;
    ਸਮਾਂ_t                    valid;

    ngx_uint_t                log_level;
};


ਬਣਤਰ ngx_resolver_ctx_s {
    ngx_resolver_ctx_t       *ਅਗਲਾ;
    ngx_resolver_t           *resolver;
    ngx_resolver_ਗੰਢ_t      *ਗੰਢ;

    /* event ident must be after 3 pointers as in ngx_connection_t */
    ngx_int_t                 ident;

    ngx_int_t                 state;
    ngx_str_t                 name;
    ngx_str_t                 service;

    ਸਮਾਂ_t                    valid;
    ngx_uint_t                naddrs;
    ngx_resolver_addr_t      *addrs;
    ngx_resolver_addr_t       addr;
    ਬਣਤਰ sockaddr_ਵਿੱਚ        ਜੀਵਾ;

    ngx_uint_t                ਗਿਣਤੀ_2;
    ngx_uint_t                nsrvs;
    ngx_resolver_srv_name_t  *srvs;

    ngx_resolver_handler_pt   handler;
    ਖਾਲੀ                     *ਅੰਕੜੇ;
    ngx_msec_t                timeout;

    ਬਿਨਾਨਿਸ਼ਾਨੀ                  ਤੇਜ਼:1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ                  async:1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ                  cancelable:1;
    ngx_uint_t                recursion;
    ngx_event_t              *event;
};


ngx_resolver_t *ngx_resolver_create(ngx_conf_t *cf, ngx_str_t *ਨਾਂ,
    ngx_uint_t n);
ngx_resolver_ctx_t *ngx_resolve_ਸ਼ੁਰੂ(ngx_resolver_t *r,
    ngx_resolver_ctx_t *ਆਰਜ਼ੀ);
ngx_int_t ngx_resolve_name(ngx_resolver_ctx_t *ctx);
ਖਾਲੀ ngx_resolve_name_ਪੂਰਾ(ngx_resolver_ctx_t *ctx);
ngx_int_t ngx_resolve_addr(ngx_resolver_ctx_t *ctx);
ਖਾਲੀ ngx_resolve_addr_ਪੂਰਾ(ngx_resolver_ctx_t *ctx);
ਅੱਖਰ *ngx_resolver_strerror(ngx_int_t err);


#ਜੇ_ਅੰਤ /* _NGX_RESOLVER_H_INCLUDED_ */
