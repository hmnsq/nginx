
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ _NGX_RESOLVER_H_INCLUDED_
#ವ್ಯಾಖ್ಯಾನಿಸು _NGX_RESOLVER_H_INCLUDED_


#ವ್ಯಾಖ್ಯಾನಿಸು NGX_RESOLVE_A         1
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_RESOLVE_CNAME     5
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_RESOLVE_PTR       12
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_RESOLVE_MX        15
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_RESOLVE_TXT       16
#ಆದರೆ (NGX_HAVE_INET6)
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_RESOLVE_AAAA      28
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_RESOLVE_SRV       33
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_RESOLVE_DNAME     39

#ವ್ಯಾಖ್ಯಾನಿಸು NGX_RESOLVE_FORMERR   1
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_RESOLVE_SERVFAIL  2
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_RESOLVE_NXDOMAIN  3
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_RESOLVE_NOTIMP    4
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_RESOLVE_REFUSED   5
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_RESOLVE_TIMEDOUT  NGX_ETIMEDOUT


#ವ್ಯಾಖ್ಯಾನಿಸು NGX_NO_RESOLVER       (ಶೂನ್ಯ *) -1

#ವ್ಯಾಖ್ಯಾನಿಸು NGX_RESOLVER_MAX_RECURSION    50


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ ngx_resolver_s  ngx_resolver_t;


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ngx_connection_t         *udp;
    ngx_connection_t         *tcp;
    ರಚನೆ sockaddr          *sockaddr;
    socklen_t                 socklen;
    ngx_str_t                 server;
    ngx_log_t                 ಲಘುಗಣಕ;
    ngx_buf_t                *read_buf;
    ngx_buf_t                *write_buf;
    ngx_resolver_t           *resolver;
} ngx_resolver_connection_t;


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ ngx_resolver_ctx_s  ngx_resolver_ctx_t;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (*ngx_resolver_handler_pt)(ngx_resolver_ctx_t *ctx);


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ರಚನೆ sockaddr          *sockaddr;
    socklen_t                 socklen;
    ngx_str_t                 name;
    u_short                   ಆದ್ಯತೆ;
    u_short                   ತೂಕ;
} ngx_resolver_addr_t;


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ngx_str_t                 name;
    u_short                   ಆದ್ಯತೆ;
    u_short                   ತೂಕ;
    u_short                   ಪೋರ್ಟ್;
} ngx_resolver_srv_t;


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ngx_str_t                 name;
    u_short                   ಆದ್ಯತೆ;
    u_short                   ತೂಕ;
    u_short                   ಪೋರ್ಟ್;

    ngx_resolver_ctx_t       *ctx;
    ngx_int_t                 state;

    ngx_uint_t                naddrs;
    ngx_addr_t               *addrs;
} ngx_resolver_srv_name_t;


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ngx_rbtree_ಗಂಟು_t         ಗಂಟು;
    ngx_ಸರತಿ_t               ಸರತಿ;

    /* PTR: resolved name, A: name to resolve */
    u_char                   *name;

#ಆದರೆ (NGX_HAVE_INET6)
    /* PTR: IPv6 address to resolve (IPv4 address is in rbtree node key) */
    ರಚನೆ in6_addr           addr6;
#ಆದರೆ_ಅಂತ್ಯ

    u_short                   nlen;
    u_short                   qlen;

    u_char                   *query;
#ಆದರೆ (NGX_HAVE_INET6)
    u_char                   *query6;
#ಆದರೆ_ಅಂತ್ಯ

    ಒಕ್ಕೂಟ {
        ಒಳ_addr_t             addr;
        ಒಳ_addr_t            *addrs;
        u_char               *cname;
        ngx_resolver_srv_t   *srvs;
    } u;

    u_char                    ಸಂಕೇತ_2;
    u_short                   naddrs;
    u_short                   nsrvs;
    u_short                   cnlen;

#ಆದರೆ (NGX_HAVE_INET6)
    ಒಕ್ಕೂಟ {
        ರಚನೆ in6_addr       addr6;
        ರಚನೆ in6_addr      *addrs6;
    } u6;

    u_short                   naddrs6;
#ಆದರೆ_ಅಂತ್ಯ

    ಸಮಯ_t                    expire;
    ಸಮಯ_t                    valid;
    uint32_t                  ttl;

    ಚಿಹ್ನೆರಹಿತ                  tcp:1;
#ಆದರೆ (NGX_HAVE_INET6)
    ಚಿಹ್ನೆರಹಿತ                  tcp6:1;
#ಆದರೆ_ಅಂತ್ಯ

    ngx_uint_t                ಕೊನೆಯ_connection;

    ngx_resolver_ctx_t       *ಕಾಯುವಿಕೆ;
} ngx_resolver_ಗಂಟು_t;


ರಚನೆ ngx_resolver_s {
    /* has to be pointer because of "incomplete type" */
    ngx_event_t              *event;
    ಶೂನ್ಯ                     *dummy;
    ngx_log_t                *ಲಘುಗಣಕ;

    /* event ident must be after 3 pointers as in ngx_connection_t */
    ngx_int_t                 ident;

    /* simple round robin DNS peers balancer */
    ngx_array_t               connections;
    ngx_uint_t                ಕೊನೆಯ_connection;

    ngx_rbtree_t              name_rbtree;
    ngx_rbtree_ಗಂಟು_t         name_sentinel;

    ngx_rbtree_t              srv_rbtree;
    ngx_rbtree_ಗಂಟು_t         srv_sentinel;

    ngx_rbtree_t              addr_rbtree;
    ngx_rbtree_ಗಂಟು_t         addr_sentinel;

    ngx_ಸರತಿ_t               name_resend_ಸರತಿ;
    ngx_ಸರತಿ_t               srv_resend_ಸರತಿ;
    ngx_ಸರತಿ_t               addr_resend_ಸರತಿ;

    ngx_ಸರತಿ_t               name_expire_ಸರತಿ;
    ngx_ಸರತಿ_t               srv_expire_ಸರತಿ;
    ngx_ಸರತಿ_t               addr_expire_ಸರತಿ;

    ಚಿಹ್ನೆರಹಿತ                  ipv4:1;

#ಆದರೆ (NGX_HAVE_INET6)
    ಚಿಹ್ನೆರಹಿತ                  ipv6:1;
    ngx_rbtree_t              addr6_rbtree;
    ngx_rbtree_ಗಂಟು_t         addr6_sentinel;
    ngx_ಸರತಿ_t               addr6_resend_ಸರತಿ;
    ngx_ಸರತಿ_t               addr6_expire_ಸರತಿ;
#ಆದರೆ_ಅಂತ್ಯ

    ಸಮಯ_t                    resend_timeout;
    ಸಮಯ_t                    tcp_timeout;
    ಸಮಯ_t                    expire;
    ಸಮಯ_t                    valid;

    ngx_uint_t                log_level;
};


ರಚನೆ ngx_resolver_ctx_s {
    ngx_resolver_ctx_t       *ಮುಂದಿನ;
    ngx_resolver_t           *resolver;
    ngx_resolver_ಗಂಟು_t      *ಗಂಟು;

    /* event ident must be after 3 pointers as in ngx_connection_t */
    ngx_int_t                 ident;

    ngx_int_t                 state;
    ngx_str_t                 name;
    ngx_str_t                 service;

    ಸಮಯ_t                    valid;
    ngx_uint_t                naddrs;
    ngx_resolver_addr_t      *addrs;
    ngx_resolver_addr_t       addr;
    ರಚನೆ sockaddr_ಒಳ        ಜ್ಯಾ;

    ngx_uint_t                ಎಣಿಕೆ_2;
    ngx_uint_t                nsrvs;
    ngx_resolver_srv_name_t  *srvs;

    ngx_resolver_handler_pt   handler;
    ಶೂನ್ಯ                     *ದತ್ತಾಂಶ;
    ngx_msec_t                timeout;

    ಚಿಹ್ನೆರಹಿತ                  ಶೀಘ್ರ:1;
    ಚಿಹ್ನೆರಹಿತ                  async:1;
    ಚಿಹ್ನೆರಹಿತ                  cancelable:1;
    ngx_uint_t                recursion;
    ngx_event_t              *event;
};


ngx_resolver_t *ngx_resolver_create(ngx_conf_t *cf, ngx_str_t *ಹೆಸರುಗಳು,
    ngx_uint_t n);
ngx_resolver_ctx_t *ngx_resolve_ಆರಂಭ(ngx_resolver_t *r,
    ngx_resolver_ctx_t *ತಾತ್ಕಾಲಿಕ);
ngx_int_t ngx_resolve_name(ngx_resolver_ctx_t *ctx);
ಶೂನ್ಯ ngx_resolve_name_ಮುಗಿದ(ngx_resolver_ctx_t *ctx);
ngx_int_t ngx_resolve_addr(ngx_resolver_ctx_t *ctx);
ಶೂನ್ಯ ngx_resolve_addr_ಮುಗಿದ(ngx_resolver_ctx_t *ctx);
ಅಕ್ಷರ *ngx_resolver_strerror(ngx_int_t err);


#ಆದರೆ_ಅಂತ್ಯ /* _NGX_RESOLVER_H_INCLUDED_ */
