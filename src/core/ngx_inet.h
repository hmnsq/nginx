
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#నిర్వచితం_కాకపోతే _NGX_INET_H_INCLUDED_
#నిర్వచించు _NGX_INET_H_INCLUDED_


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>


#నిర్వచించు NGX_INET_ADDRSTRLEN   (పరిమాణం("255.255.255.255") - 1)
#నిర్వచించు NGX_INET6_ADDRSTRLEN                                                 \
    (పరిమాణం("ffff:ffff:ffff:ffff:ffff:ffff:255.255.255.255") - 1)
#నిర్వచించు NGX_UNIX_ADDRSTRLEN                                                  \
    (పరిమాణం("unix:") - 1 +                                                   \
     పరిమాణం(నిర్మాణం sockaddr_un) - offsetof(నిర్మాణం sockaddr_un, sun_మార్గం))

#ఒకవేళ (NGX_HAVE_UNIX_DOMAIN)
#నిర్వచించు NGX_SOCKADDR_STRLEN   NGX_UNIX_ADDRSTRLEN
#లేదావేళ (NGX_HAVE_INET6)
#నిర్వచించు NGX_SOCKADDR_STRLEN   (NGX_INET6_ADDRSTRLEN + పరిమాణం("[]:65535") - 1)
#లేకపోతే
#నిర్వచించు NGX_SOCKADDR_STRLEN   (NGX_INET_ADDRSTRLEN + పరిమాణం(":65535") - 1)
#అయితే_ముగింపు

/* compatibility */
#నిర్వచించు NGX_SOCKADDRLEN       పరిమాణం(ngx_sockaddr_t)


రకనిర్వచనం సంఘం {
    నిర్మాణం sockaddr           sockaddr;
    నిర్మాణం sockaddr_లోపల        sockaddr_లోపల;
#ఒకవేళ (NGX_HAVE_INET6)
    నిర్మాణం sockaddr_in6       sockaddr_in6;
#అయితే_ముగింపు
#ఒకవేళ (NGX_HAVE_UNIX_DOMAIN)
    నిర్మాణం sockaddr_un        sockaddr_un;
#అయితే_ముగింపు
} ngx_sockaddr_t;


రకనిర్వచనం నిర్మాణం {
    లోపల_addr_t                 addr;
    లోపల_addr_t                 ముసుగు;
} ngx_లోపల_cidr_t;


#ఒకవేళ (NGX_HAVE_INET6)

రకనిర్వచనం నిర్మాణం {
    నిర్మాణం in6_addr           addr;
    నిర్మాణం in6_addr           ముసుగు;
} ngx_in6_cidr_t;

#అయితే_ముగింపు


రకనిర్వచనం నిర్మాణం {
    ngx_uint_t                family;
    సంఘం {
        ngx_లోపల_cidr_t         లోపల;
#ఒకవేళ (NGX_HAVE_INET6)
        ngx_in6_cidr_t        in6;
#అయితే_ముగింపు
    } u;
} ngx_cidr_t;


రకనిర్వచనం నిర్మాణం {
    నిర్మాణం sockaddr          *sockaddr;
    socklen_t                 socklen;
    ngx_str_t                 name;
} ngx_addr_t;


రకనిర్వచనం నిర్మాణం {
    ngx_str_t                 url;
    ngx_str_t                 host;
    ngx_str_t                 పోర్టు_పాఠ్యం;
    ngx_str_t                 uri;

    లోపల_పోర్టు_t                 పోర్టు;
    లోపల_పోర్టు_t                 default_పోర్టు;
    లోపల_పోర్టు_t                 చివరి_పోర్టు;
    పూర్ణాంకం                       family;

    గుర్తులేని                  listen:1;
    గుర్తులేని                  uri_part:1;
    గుర్తులేని                  no_resolve:1;

    గుర్తులేని                  no_పోర్టు:1;
    గుర్తులేని                  wildcard:1;

    socklen_t                 socklen;
    ngx_sockaddr_t            sockaddr;

    ngx_addr_t               *addrs;
    ngx_uint_t                naddrs;

    అక్షరం                     *err;
} ngx_url_t;


లోపల_addr_t ngx_inet_addr(u_char *పాఠ్యం, కొలత_t len);
#ఒకవేళ (NGX_HAVE_INET6)
ngx_int_t ngx_inet6_addr(u_char *p, కొలత_t len, u_char *addr);
కొలత_t ngx_inet6_ntop(u_char *p, u_char *పాఠ్యం, కొలత_t len);
#అయితే_ముగింపు
కొలత_t ngx_sock_ntop(నిర్మాణం sockaddr *sa, socklen_t socklen, u_char *పాఠ్యం,
    కొలత_t len, ngx_uint_t పోర్టు);
కొలత_t ngx_inet_ntop(పూర్ణాంకం family, శూన్య *addr, u_char *పాఠ్యం, కొలత_t len);
ngx_int_t ngx_ptocidr(ngx_str_t *పాఠ్యం, ngx_cidr_t *cidr);
ngx_int_t ngx_cidr_సరిపోలిక(నిర్మాణం sockaddr *sa, ngx_array_t *cidrs);
ngx_int_t ngx_parse_addr(ngx_pool_t *pool, ngx_addr_t *addr, u_char *పాఠ్యం,
    కొలత_t len);
ngx_int_t ngx_parse_addr_పోర్టు(ngx_pool_t *pool, ngx_addr_t *addr,
    u_char *పాఠ్యం, కొలత_t len);
ngx_int_t ngx_parse_url(ngx_pool_t *pool, ngx_url_t *u);
ngx_int_t ngx_inet_resolve_host(ngx_pool_t *pool, ngx_url_t *u);
ngx_int_t ngx_cmp_sockaddr(నిర్మాణం sockaddr *sa1, socklen_t slen1,
    నిర్మాణం sockaddr *sa2, socklen_t slen2, ngx_uint_t cmp_పోర్టు);
లోపల_పోర్టు_t ngx_inet_get_పోర్టు(నిర్మాణం sockaddr *sa);
శూన్య ngx_inet_సమితి_పోర్టు(నిర్మాణం sockaddr *sa, లోపల_పోర్టు_t పోర్టు);
ngx_uint_t ngx_inet_wildcard(నిర్మాణం sockaddr *sa);


#అయితే_ముగింపు /* _NGX_INET_H_INCLUDED_ */
