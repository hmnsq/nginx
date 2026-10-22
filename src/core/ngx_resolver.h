
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


#ifndef _NGX_RESOLVER_H_INCLUDED_
#define _NGX_RESOLVER_H_INCLUDED_


#define NGX_RESOLVE_A         1
#define NGX_RESOLVE_CNAME     5
#define NGX_RESOLVE_PTR       12
#define NGX_RESOLVE_MX        15
#define NGX_RESOLVE_TXT       16
#यदि (NGX_HAVE_INET6)
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


#define NGX_NO_RESOLVER       (शून्य *) -1

#define NGX_RESOLVER_MAX_RECURSION    50


प्रकारनिर्देश संरचना ngx_resolver_s  ngx_resolver_t;


प्रकारनिर्देश संरचना {
    ngx_connection_t         *udp;
    ngx_connection_t         *tcp;
    संरचना sockaddr          *sockaddr;
    socklen_t                 socklen;
    ngx_str_t                 server;
    ngx_log_t                 log;
    ngx_buf_t                *read_buf;
    ngx_buf_t                *write_buf;
    ngx_resolver_t           *resolver;
} ngx_resolver_connection_t;


प्रकारनिर्देश संरचना ngx_resolver_ctx_s  ngx_resolver_ctx_t;

प्रकारनिर्देश शून्य (*ngx_resolver_handler_pt)(ngx_resolver_ctx_t *ctx);


प्रकारनिर्देश संरचना {
    संरचना sockaddr          *sockaddr;
    socklen_t                 socklen;
    ngx_str_t                 name;
    u_short                   priority;
    u_short                   weight;
} ngx_resolver_addr_t;


प्रकारनिर्देश संरचना {
    ngx_str_t                 name;
    u_short                   priority;
    u_short                   weight;
    u_short                   port;
} ngx_resolver_srv_t;


प्रकारनिर्देश संरचना {
    ngx_str_t                 name;
    u_short                   priority;
    u_short                   weight;
    u_short                   port;

    ngx_resolver_ctx_t       *ctx;
    ngx_int_t                 state;

    ngx_uint_t                naddrs;
    ngx_addr_t               *addrs;
} ngx_resolver_srv_name_t;


प्रकारनिर्देश संरचना {
    ngx_rbtree_गाँठो_t         गाँठो;
    ngx_queue_t               queue;

    /* PTR: resolved name, A: name to resolve */
    u_char                   *name;

#यदि (NGX_HAVE_INET6)
    /* PTR: IPv6 address to resolve (IPv4 address is in rbtree node key) */
    संरचना in6_addr           addr6;
#endif

    u_short                   nlen;
    u_short                   qlen;

    u_char                   *query;
#यदि (NGX_HAVE_INET6)
    u_char                   *query6;
#endif

    सङ्घ {
        in_addr_t             addr;
        in_addr_t            *addrs;
        u_char               *cname;
        ngx_resolver_srv_t   *srvs;
    } u;

    u_char                    code;
    u_short                   naddrs;
    u_short                   nsrvs;
    u_short                   cnlen;

#यदि (NGX_HAVE_INET6)
    सङ्घ {
        संरचना in6_addr       addr6;
        संरचना in6_addr      *addrs6;
    } u6;

    u_short                   naddrs6;
#endif

    समय_प्रकार                    expire;
    समय_प्रकार                    valid;
    uint32_t                  ttl;

    अचिन्हित                  tcp:1;
#यदि (NGX_HAVE_INET6)
    अचिन्हित                  tcp6:1;
#endif

    ngx_uint_t                last_connection;

    ngx_resolver_ctx_t       *waiting;
} ngx_resolver_गाँठो_t;


संरचना ngx_resolver_s {
    /* has to be pointer because of "incomplete type" */
    ngx_event_t              *event;
    शून्य                     *dummy;
    ngx_log_t                *log;

    /* event ident must be after 3 pointers as in ngx_connection_t */
    ngx_int_t                 ident;

    /* simple round robin DNS peers balancer */
    ngx_array_t               connections;
    ngx_uint_t                last_connection;

    ngx_rbtree_t              name_rbtree;
    ngx_rbtree_गाँठो_t         name_sentinel;

    ngx_rbtree_t              srv_rbtree;
    ngx_rbtree_गाँठो_t         srv_sentinel;

    ngx_rbtree_t              addr_rbtree;
    ngx_rbtree_गाँठो_t         addr_sentinel;

    ngx_queue_t               name_resend_queue;
    ngx_queue_t               srv_resend_queue;
    ngx_queue_t               addr_resend_queue;

    ngx_queue_t               name_expire_queue;
    ngx_queue_t               srv_expire_queue;
    ngx_queue_t               addr_expire_queue;

    अचिन्हित                  ipv4:1;

#यदि (NGX_HAVE_INET6)
    अचिन्हित                  ipv6:1;
    ngx_rbtree_t              addr6_rbtree;
    ngx_rbtree_गाँठो_t         addr6_sentinel;
    ngx_queue_t               addr6_resend_queue;
    ngx_queue_t               addr6_expire_queue;
#endif

    समय_प्रकार                    resend_timeout;
    समय_प्रकार                    tcp_timeout;
    समय_प्रकार                    expire;
    समय_प्रकार                    valid;

    ngx_uint_t                log_level;
};


संरचना ngx_resolver_ctx_s {
    ngx_resolver_ctx_t       *next;
    ngx_resolver_t           *resolver;
    ngx_resolver_गाँठो_t      *गाँठो;

    /* event ident must be after 3 pointers as in ngx_connection_t */
    ngx_int_t                 ident;

    ngx_int_t                 state;
    ngx_str_t                 name;
    ngx_str_t                 service;

    समय_प्रकार                    valid;
    ngx_uint_t                naddrs;
    ngx_resolver_addr_t      *addrs;
    ngx_resolver_addr_t       addr;
    संरचना sockaddr_in        sin;

    ngx_uint_t                गन्ती;
    ngx_uint_t                nsrvs;
    ngx_resolver_srv_name_t  *srvs;

    ngx_resolver_handler_pt   handler;
    शून्य                     *data;
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
    ngx_resolver_ctx_t *temp);
ngx_int_t ngx_resolve_name(ngx_resolver_ctx_t *ctx);
शून्य ngx_resolve_name_done(ngx_resolver_ctx_t *ctx);
ngx_int_t ngx_resolve_addr(ngx_resolver_ctx_t *ctx);
शून्य ngx_resolve_addr_done(ngx_resolver_ctx_t *ctx);
वर्ण *ngx_resolver_strerror(ngx_int_t err);


#endif /* _NGX_RESOLVER_H_INCLUDED_ */
