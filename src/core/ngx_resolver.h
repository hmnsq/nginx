
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#समावेश <ngx_config.h>
#समावेश <ngx_core.h>


#अगर_अघोषित _NGX_RESOLVER_H_INCLUDED_
#घोषणा _NGX_RESOLVER_H_INCLUDED_


#घोषणा NGX_RESOLVE_A         1
#घोषणा NGX_RESOLVE_CNAME     5
#घोषणा NGX_RESOLVE_PTR       12
#घोषणा NGX_RESOLVE_MX        15
#घोषणा NGX_RESOLVE_TXT       16
#अगर (NGX_HAVE_INET6)
#घोषणा NGX_RESOLVE_AAAA      28
#पूर्ण_अगर
#घोषणा NGX_RESOLVE_SRV       33
#घोषणा NGX_RESOLVE_DNAME     39

#घोषणा NGX_RESOLVE_FORMERR   1
#घोषणा NGX_RESOLVE_SERVFAIL  2
#घोषणा NGX_RESOLVE_NXDOMAIN  3
#घोषणा NGX_RESOLVE_NOTIMP    4
#घोषणा NGX_RESOLVE_REFUSED   5
#घोषणा NGX_RESOLVE_TIMEDOUT  NGX_ETIMEDOUT


#घोषणा NGX_NO_RESOLVER       (व्योम *) -1

#घोषणा NGX_RESOLVER_MAX_RECURSION    50


प्रकार काष्ठा ngx_resolver_s  ngx_resolver_t;


प्रकार काष्ठा {
    ngx_connection_t         *udp;
    ngx_connection_t         *tcp;
    काष्ठा sockaddr          *sockaddr;
    socklen_t                 socklen;
    ngx_str_t                 server;
    ngx_log_t                 log;
    ngx_buf_t                *read_buf;
    ngx_buf_t                *write_buf;
    ngx_resolver_t           *resolver;
} ngx_resolver_connection_t;


प्रकार काष्ठा ngx_resolver_ctx_s  ngx_resolver_ctx_t;

प्रकार व्योम (*ngx_resolver_handler_pt)(ngx_resolver_ctx_t *ctx);


प्रकार काष्ठा {
    काष्ठा sockaddr          *sockaddr;
    socklen_t                 socklen;
    ngx_str_t                 name;
    u_short                   priority;
    u_short                   weight;
} ngx_resolver_addr_t;


प्रकार काष्ठा {
    ngx_str_t                 name;
    u_short                   priority;
    u_short                   weight;
    u_short                   port;
} ngx_resolver_srv_t;


प्रकार काष्ठा {
    ngx_str_t                 name;
    u_short                   priority;
    u_short                   weight;
    u_short                   port;

    ngx_resolver_ctx_t       *ctx;
    ngx_int_t                 state;

    ngx_uint_t                naddrs;
    ngx_addr_t               *addrs;
} ngx_resolver_srv_name_t;


प्रकार काष्ठा {
    ngx_rbtree_गाँठ_t         गाँठ;
    ngx_queue_t               queue;

    /* PTR: resolved name, A: name to resolve */
    u_char                   *name;

#अगर (NGX_HAVE_INET6)
    /* PTR: IPv6 address to resolve (IPv4 address is in rbtree node key) */
    काष्ठा in6_addr           addr6;
#पूर्ण_अगर

    u_short                   nlen;
    u_short                   qlen;

    u_char                   *query;
#अगर (NGX_HAVE_INET6)
    u_char                   *query6;
#पूर्ण_अगर

    जोड़ {
        in_addr_t             addr;
        in_addr_t            *addrs;
        u_char               *cname;
        ngx_resolver_srv_t   *srvs;
    } u;

    u_char                    code;
    u_short                   naddrs;
    u_short                   nsrvs;
    u_short                   cnlen;

#अगर (NGX_HAVE_INET6)
    जोड़ {
        काष्ठा in6_addr       addr6;
        काष्ठा in6_addr      *addrs6;
    } u6;

    u_short                   naddrs6;
#पूर्ण_अगर

    समय_प्रकार                    expire;
    समय_प्रकार                    valid;
    uint32_t                  ttl;

    अचिन्हित                  tcp:1;
#अगर (NGX_HAVE_INET6)
    अचिन्हित                  tcp6:1;
#पूर्ण_अगर

    ngx_uint_t                last_connection;

    ngx_resolver_ctx_t       *waiting;
} ngx_resolver_गाँठ_t;


काष्ठा ngx_resolver_s {
    /* has to be pointer because of "incomplete type" */
    ngx_event_t              *event;
    व्योम                     *dummy;
    ngx_log_t                *log;

    /* event ident must be after 3 pointers as in ngx_connection_t */
    ngx_int_t                 ident;

    /* simple round robin DNS peers balancer */
    ngx_array_t               connections;
    ngx_uint_t                last_connection;

    ngx_rbtree_t              name_rbtree;
    ngx_rbtree_गाँठ_t         name_sentinel;

    ngx_rbtree_t              srv_rbtree;
    ngx_rbtree_गाँठ_t         srv_sentinel;

    ngx_rbtree_t              addr_rbtree;
    ngx_rbtree_गाँठ_t         addr_sentinel;

    ngx_queue_t               name_resend_queue;
    ngx_queue_t               srv_resend_queue;
    ngx_queue_t               addr_resend_queue;

    ngx_queue_t               name_expire_queue;
    ngx_queue_t               srv_expire_queue;
    ngx_queue_t               addr_expire_queue;

    अचिन्हित                  ipv4:1;

#अगर (NGX_HAVE_INET6)
    अचिन्हित                  ipv6:1;
    ngx_rbtree_t              addr6_rbtree;
    ngx_rbtree_गाँठ_t         addr6_sentinel;
    ngx_queue_t               addr6_resend_queue;
    ngx_queue_t               addr6_expire_queue;
#पूर्ण_अगर

    समय_प्रकार                    resend_timeout;
    समय_प्रकार                    tcp_timeout;
    समय_प्रकार                    expire;
    समय_प्रकार                    valid;

    ngx_uint_t                log_level;
};


काष्ठा ngx_resolver_ctx_s {
    ngx_resolver_ctx_t       *अग्र;
    ngx_resolver_t           *resolver;
    ngx_resolver_गाँठ_t      *गाँठ;

    /* event ident must be after 3 pointers as in ngx_connection_t */
    ngx_int_t                 ident;

    ngx_int_t                 state;
    ngx_str_t                 name;
    ngx_str_t                 service;

    समय_प्रकार                    valid;
    ngx_uint_t                naddrs;
    ngx_resolver_addr_t      *addrs;
    ngx_resolver_addr_t       addr;
    काष्ठा sockaddr_in        sin;

    ngx_uint_t                गिनती;
    ngx_uint_t                nsrvs;
    ngx_resolver_srv_name_t  *srvs;

    ngx_resolver_handler_pt   handler;
    व्योम                     *तथ्य;
    ngx_msec_t                timeout;

    अचिन्हित                  quick:1;
    अचिन्हित                  async:1;
    अचिन्हित                  cancelable:1;
    ngx_uint_t                recursion;
    ngx_event_t              *event;
};


ngx_resolver_t *ngx_resolver_create(ngx_conf_t *cf, ngx_str_t *names,
    ngx_uint_t n);
ngx_resolver_ctx_t *ngx_resolve_start(ngx_resolver_t *r,
    ngx_resolver_ctx_t *अस्थायी);
ngx_int_t ngx_resolve_name(ngx_resolver_ctx_t *ctx);
व्योम ngx_resolve_name_done(ngx_resolver_ctx_t *ctx);
ngx_int_t ngx_resolve_addr(ngx_resolver_ctx_t *ctx);
व्योम ngx_resolve_addr_done(ngx_resolver_ctx_t *ctx);
अक्षर *ngx_resolver_strerror(ngx_int_t err);


#पूर्ण_अगर /* _NGX_RESOLVER_H_INCLUDED_ */
