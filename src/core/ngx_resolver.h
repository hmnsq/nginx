
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


#यदि_अपरिभाषितम् _NGX_RESOLVER_H_INCLUDED_
#परिभाषय _NGX_RESOLVER_H_INCLUDED_


#परिभाषय NGX_RESOLVE_A         1
#परिभाषय NGX_RESOLVE_CNAME     5
#परिभाषय NGX_RESOLVE_PTR       12
#परिभाषय NGX_RESOLVE_MX        15
#परिभाषय NGX_RESOLVE_TXT       16
#यदि (NGX_HAVE_INET6)
#परिभाषय NGX_RESOLVE_AAAA      28
#यदि_अन्तः
#परिभाषय NGX_RESOLVE_SRV       33
#परिभाषय NGX_RESOLVE_DNAME     39

#परिभाषय NGX_RESOLVE_FORMERR   1
#परिभाषय NGX_RESOLVE_SERVFAIL  2
#परिभाषय NGX_RESOLVE_NXDOMAIN  3
#परिभाषय NGX_RESOLVE_NOTIMP    4
#परिभाषय NGX_RESOLVE_REFUSED   5
#परिभाषय NGX_RESOLVE_TIMEDOUT  NGX_ETIMEDOUT


#परिभाषय NGX_NO_RESOLVER       (शून्य *) -1

#परिभाषय NGX_RESOLVER_MAX_RECURSION    50


प्रकारनिर्देश संरचना ngx_resolver_s  ngx_resolver_t;


प्रकारनिर्देश संरचना {
    ngx_connection_t         *udp;
    ngx_connection_t         *tcp;
    संरचना sockaddr          *sockaddr;
    socklen_t                 socklen;
    ngx_str_t                 server;
    ngx_log_t                 लघुगणकः;
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
    u_short                   प्राथमिकता;
    u_short                   weight;
} ngx_resolver_addr_t;


प्रकारनिर्देश संरचना {
    ngx_str_t                 name;
    u_short                   प्राथमिकता;
    u_short                   weight;
    u_short                   पत्तनम्;
} ngx_resolver_srv_t;


प्रकारनिर्देश संरचना {
    ngx_str_t                 name;
    u_short                   प्राथमिकता;
    u_short                   weight;
    u_short                   पत्तनम्;

    ngx_resolver_ctx_t       *ctx;
    ngx_int_t                 state;

    ngx_uint_t                naddrs;
    ngx_addr_t               *addrs;
} ngx_resolver_srv_name_t;


प्रकारनिर्देश संरचना {
    ngx_rbtree_ग्रन्थिः_t         ग्रन्थिः;
    ngx_queue_t               queue;

    /* PTR: resolved name, A: name to resolve */
    u_char                   *name;

#यदि (NGX_HAVE_INET6)
    /* PTR: IPv6 address to resolve (IPv4 address is in rbtree node key) */
    संरचना in6_addr           addr6;
#यदि_अन्तः

    u_short                   nlen;
    u_short                   qlen;

    u_char                   *query;
#यदि (NGX_HAVE_INET6)
    u_char                   *query6;
#यदि_अन्तः

    सङ्घ {
        अन्तर्_addr_t             addr;
        अन्तर्_addr_t            *addrs;
        u_char               *cname;
        ngx_resolver_srv_t   *srvs;
    } u;

    u_char                    संहिता;
    u_short                   naddrs;
    u_short                   nsrvs;
    u_short                   cnlen;

#यदि (NGX_HAVE_INET6)
    सङ्घ {
        संरचना in6_addr       addr6;
        संरचना in6_addr      *addrs6;
    } u6;

    u_short                   naddrs6;
#यदि_अन्तः

    time_t                    expire;
    time_t                    valid;
    uint32_t                  ttl;

    अचिह्नित                  tcp:1;
#यदि (NGX_HAVE_INET6)
    अचिह्नित                  tcp6:1;
#यदि_अन्तः

    ngx_uint_t                चरमः_connection;

    ngx_resolver_ctx_t       *प्रतीक्षा;
} ngx_resolver_ग्रन्थिः_t;


संरचना ngx_resolver_s {
    /* has to be pointer because of "incomplete type" */
    ngx_event_t              *event;
    शून्य                     *dummy;
    ngx_log_t                *लघुगणकः;

    /* event ident must be after 3 pointers as in ngx_connection_t */
    ngx_int_t                 ident;

    /* simple round robin DNS peers balancer */
    ngx_array_t               connections;
    ngx_uint_t                चरमः_connection;

    ngx_rbtree_t              name_rbtree;
    ngx_rbtree_ग्रन्थिः_t         name_sentinel;

    ngx_rbtree_t              srv_rbtree;
    ngx_rbtree_ग्रन्थिः_t         srv_sentinel;

    ngx_rbtree_t              addr_rbtree;
    ngx_rbtree_ग्रन्थिः_t         addr_sentinel;

    ngx_queue_t               name_resend_queue;
    ngx_queue_t               srv_resend_queue;
    ngx_queue_t               addr_resend_queue;

    ngx_queue_t               name_expire_queue;
    ngx_queue_t               srv_expire_queue;
    ngx_queue_t               addr_expire_queue;

    अचिह्नित                  ipv4:1;

#यदि (NGX_HAVE_INET6)
    अचिह्नित                  ipv6:1;
    ngx_rbtree_t              addr6_rbtree;
    ngx_rbtree_ग्रन्थिः_t         addr6_sentinel;
    ngx_queue_t               addr6_resend_queue;
    ngx_queue_t               addr6_expire_queue;
#यदि_अन्तः

    time_t                    resend_timeout;
    time_t                    tcp_timeout;
    time_t                    expire;
    time_t                    valid;

    ngx_uint_t                log_level;
};


संरचना ngx_resolver_ctx_s {
    ngx_resolver_ctx_t       *अग्र;
    ngx_resolver_t           *resolver;
    ngx_resolver_ग्रन्थिः_t      *ग्रन्थिः;

    /* event ident must be after 3 pointers as in ngx_connection_t */
    ngx_int_t                 ident;

    ngx_int_t                 state;
    ngx_str_t                 name;
    ngx_str_t                 service;

    time_t                    valid;
    ngx_uint_t                naddrs;
    ngx_resolver_addr_t      *addrs;
    ngx_resolver_addr_t       addr;
    संरचना sockaddr_अन्तर्        ज्या;

    ngx_uint_t                गणनम्;
    ngx_uint_t                nsrvs;
    ngx_resolver_srv_name_t  *srvs;

    ngx_resolver_handler_pt   handler;
    शून्य                     *दत्त;
    ngx_msec_t                timeout;

    अचिह्नित                  शीघ्र:1;
    अचिह्नित                  async:1;
    अचिह्नित                  cancelable:1;
    ngx_uint_t                recursion;
    ngx_event_t              *event;
};


ngx_resolver_t *ngx_resolver_create(ngx_conf_t *cf, ngx_str_t *नामानि,
    ngx_uint_t n);
ngx_resolver_ctx_t *ngx_resolve_start(ngx_resolver_t *r,
    ngx_resolver_ctx_t *अस्थायी);
ngx_int_t ngx_resolve_name(ngx_resolver_ctx_t *ctx);
शून्य ngx_resolve_name_समाप्तम्(ngx_resolver_ctx_t *ctx);
ngx_int_t ngx_resolve_addr(ngx_resolver_ctx_t *ctx);
शून्य ngx_resolve_addr_समाप्तम्(ngx_resolver_ctx_t *ctx);
वर्ण *ngx_resolver_strerror(ngx_int_t err);


#यदि_अन्तः /* _NGX_RESOLVER_H_INCLUDED_ */
