
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>


#నిర్వచితం_కాకపోతే _NGX_RESOLVER_H_INCLUDED_
#నిర్వచించు _NGX_RESOLVER_H_INCLUDED_


#నిర్వచించు NGX_RESOLVE_A         1
#నిర్వచించు NGX_RESOLVE_CNAME     5
#నిర్వచించు NGX_RESOLVE_PTR       12
#నిర్వచించు NGX_RESOLVE_MX        15
#నిర్వచించు NGX_RESOLVE_TXT       16
#ఒకవేళ (NGX_HAVE_INET6)
#నిర్వచించు NGX_RESOLVE_AAAA      28
#అయితే_ముగింపు
#నిర్వచించు NGX_RESOLVE_SRV       33
#నిర్వచించు NGX_RESOLVE_DNAME     39

#నిర్వచించు NGX_RESOLVE_FORMERR   1
#నిర్వచించు NGX_RESOLVE_SERVFAIL  2
#నిర్వచించు NGX_RESOLVE_NXDOMAIN  3
#నిర్వచించు NGX_RESOLVE_NOTIMP    4
#నిర్వచించు NGX_RESOLVE_REFUSED   5
#నిర్వచించు NGX_RESOLVE_TIMEDOUT  NGX_ETIMEDOUT


#నిర్వచించు NGX_NO_RESOLVER       (శూన్య *) -1

#నిర్వచించు NGX_RESOLVER_MAX_RECURSION    50


రకనిర్వచనం నిర్మాణం ngx_resolver_s  ngx_resolver_t;


రకనిర్వచనం నిర్మాణం {
    ngx_connection_t         *udp;
    ngx_connection_t         *tcp;
    నిర్మాణం sockaddr          *sockaddr;
    socklen_t                 socklen;
    ngx_str_t                 server;
    ngx_log_t                 సంవర్గమానం;
    ngx_buf_t                *read_buf;
    ngx_buf_t                *write_buf;
    ngx_resolver_t           *resolver;
} ngx_resolver_connection_t;


రకనిర్వచనం నిర్మాణం ngx_resolver_ctx_s  ngx_resolver_ctx_t;

రకనిర్వచనం శూన్య (*ngx_resolver_handler_pt)(ngx_resolver_ctx_t *ctx);


రకనిర్వచనం నిర్మాణం {
    నిర్మాణం sockaddr          *sockaddr;
    socklen_t                 socklen;
    ngx_str_t                 name;
    u_short                   ప్రాధాన్యత;
    u_short                   బరువు;
} ngx_resolver_addr_t;


రకనిర్వచనం నిర్మాణం {
    ngx_str_t                 name;
    u_short                   ప్రాధాన్యత;
    u_short                   బరువు;
    u_short                   పోర్టు;
} ngx_resolver_srv_t;


రకనిర్వచనం నిర్మాణం {
    ngx_str_t                 name;
    u_short                   ప్రాధాన్యత;
    u_short                   బరువు;
    u_short                   పోర్టు;

    ngx_resolver_ctx_t       *ctx;
    ngx_int_t                 state;

    ngx_uint_t                naddrs;
    ngx_addr_t               *addrs;
} ngx_resolver_srv_name_t;


రకనిర్వచనం నిర్మాణం {
    ngx_rbtree_కణుపు_t         కణుపు;
    ngx_వరుస_t               వరుస;

    /* PTR: resolved name, A: name to resolve */
    u_char                   *name;

#ఒకవేళ (NGX_HAVE_INET6)
    /* PTR: IPv6 address to resolve (IPv4 address is in rbtree node key) */
    నిర్మాణం in6_addr           addr6;
#అయితే_ముగింపు

    u_short                   nlen;
    u_short                   qlen;

    u_char                   *query;
#ఒకవేళ (NGX_HAVE_INET6)
    u_char                   *query6;
#అయితే_ముగింపు

    సంఘం {
        లోపల_addr_t             addr;
        లోపల_addr_t            *addrs;
        u_char               *cname;
        ngx_resolver_srv_t   *srvs;
    } u;

    u_char                    సంకేతం_2;
    u_short                   naddrs;
    u_short                   nsrvs;
    u_short                   cnlen;

#ఒకవేళ (NGX_HAVE_INET6)
    సంఘం {
        నిర్మాణం in6_addr       addr6;
        నిర్మాణం in6_addr      *addrs6;
    } u6;

    u_short                   naddrs6;
#అయితే_ముగింపు

    సమయం_t                    expire;
    సమయం_t                    valid;
    uint32_t                  ttl;

    గుర్తులేని                  tcp:1;
#ఒకవేళ (NGX_HAVE_INET6)
    గుర్తులేని                  tcp6:1;
#అయితే_ముగింపు

    ngx_uint_t                చివరి_connection;

    ngx_resolver_ctx_t       *నిరీక్షణ;
} ngx_resolver_కణుపు_t;


నిర్మాణం ngx_resolver_s {
    /* has to be pointer because of "incomplete type" */
    ngx_event_t              *event;
    శూన్య                     *dummy;
    ngx_log_t                *సంవర్గమానం;

    /* event ident must be after 3 pointers as in ngx_connection_t */
    ngx_int_t                 ident;

    /* simple round robin DNS peers balancer */
    ngx_array_t               connections;
    ngx_uint_t                చివరి_connection;

    ngx_rbtree_t              name_rbtree;
    ngx_rbtree_కణుపు_t         name_sentinel;

    ngx_rbtree_t              srv_rbtree;
    ngx_rbtree_కణుపు_t         srv_sentinel;

    ngx_rbtree_t              addr_rbtree;
    ngx_rbtree_కణుపు_t         addr_sentinel;

    ngx_వరుస_t               name_resend_వరుస;
    ngx_వరుస_t               srv_resend_వరుస;
    ngx_వరుస_t               addr_resend_వరుస;

    ngx_వరుస_t               name_expire_వరుస;
    ngx_వరుస_t               srv_expire_వరుస;
    ngx_వరుస_t               addr_expire_వరుస;

    గుర్తులేని                  ipv4:1;

#ఒకవేళ (NGX_HAVE_INET6)
    గుర్తులేని                  ipv6:1;
    ngx_rbtree_t              addr6_rbtree;
    ngx_rbtree_కణుపు_t         addr6_sentinel;
    ngx_వరుస_t               addr6_resend_వరుస;
    ngx_వరుస_t               addr6_expire_వరుస;
#అయితే_ముగింపు

    సమయం_t                    resend_timeout;
    సమయం_t                    tcp_timeout;
    సమయం_t                    expire;
    సమయం_t                    valid;

    ngx_uint_t                log_level;
};


నిర్మాణం ngx_resolver_ctx_s {
    ngx_resolver_ctx_t       *తదుపరి;
    ngx_resolver_t           *resolver;
    ngx_resolver_కణుపు_t      *కణుపు;

    /* event ident must be after 3 pointers as in ngx_connection_t */
    ngx_int_t                 ident;

    ngx_int_t                 state;
    ngx_str_t                 name;
    ngx_str_t                 service;

    సమయం_t                    valid;
    ngx_uint_t                naddrs;
    ngx_resolver_addr_t      *addrs;
    ngx_resolver_addr_t       addr;
    నిర్మాణం sockaddr_లోపల        జ్యా;

    ngx_uint_t                లెక్క;
    ngx_uint_t                nsrvs;
    ngx_resolver_srv_name_t  *srvs;

    ngx_resolver_handler_pt   handler;
    శూన్య                     *దత్తాంశం;
    ngx_msec_t                timeout;

    గుర్తులేని                  శీఘ్ర:1;
    గుర్తులేని                  async:1;
    గుర్తులేని                  cancelable:1;
    ngx_uint_t                recursion;
    ngx_event_t              *event;
};


ngx_resolver_t *ngx_resolver_create(ngx_conf_t *cf, ngx_str_t *పేర్లు,
    ngx_uint_t n);
ngx_resolver_ctx_t *ngx_resolve_ప్రారంభం(ngx_resolver_t *r,
    ngx_resolver_ctx_t *తాత్కాలిక);
ngx_int_t ngx_resolve_name(ngx_resolver_ctx_t *ctx);
శూన్య ngx_resolve_name_పూర్తి(ngx_resolver_ctx_t *ctx);
ngx_int_t ngx_resolve_addr(ngx_resolver_ctx_t *ctx);
శూన్య ngx_resolve_addr_పూర్తి(ngx_resolver_ctx_t *ctx);
అక్షరం *ngx_resolver_strerror(ngx_int_t err);


#అయితే_ముగింపు /* _NGX_RESOLVER_H_INCLUDED_ */
