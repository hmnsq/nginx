
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#که_نه_تعریف_شوی _NGX_INET_H_INCLUDED_
#تعریف_کړه _NGX_INET_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


#تعریف_کړه NGX_INET_ADDRSTRLEN   (حجم("255.255.255.255") - 1)
#تعریف_کړه NGX_INET6_ADDRSTRLEN                                                 \
    (حجم("ffff:ffff:ffff:ffff:ffff:ffff:255.255.255.255") - 1)
#تعریف_کړه NGX_UNIX_ADDRSTRLEN                                                  \
    (حجم("unix:") - 1 +                                                   \
     حجم(بنية sockaddr_un) - offsetof(بنية sockaddr_un, sun_لار))

#إذا (NGX_HAVE_UNIX_DOMAIN)
#تعریف_کړه NGX_SOCKADDR_STRLEN   NGX_UNIX_ADDRSTRLEN
#وإلاإذا (NGX_HAVE_INET6)
#تعریف_کړه NGX_SOCKADDR_STRLEN   (NGX_INET6_ADDRSTRLEN + حجم("[]:65535") - 1)
#وإلا
#تعریف_کړه NGX_SOCKADDR_STRLEN   (NGX_INET_ADDRSTRLEN + حجم(":65535") - 1)
#د_که_پای

/* compatibility */
#تعریف_کړه NGX_SOCKADDRLEN       حجم(ngx_sockaddr_t)


تعريفنوع اتحاد {
    بنية sockaddr           sockaddr;
    بنية sockaddr_دننه        sockaddr_دننه;
#إذا (NGX_HAVE_INET6)
    بنية sockaddr_in6       sockaddr_in6;
#د_که_پای
#إذا (NGX_HAVE_UNIX_DOMAIN)
    بنية sockaddr_un        sockaddr_un;
#د_که_پای
} ngx_sockaddr_t;


تعريفنوع بنية {
    دننه_addr_t                 addr;
    دننه_addr_t                 ماسک;
} ngx_دننه_cidr_t;


#إذا (NGX_HAVE_INET6)

تعريفنوع بنية {
    بنية in6_addr           addr;
    بنية in6_addr           ماسک;
} ngx_in6_cidr_t;

#د_که_پای


تعريفنوع بنية {
    ngx_uint_t                family;
    اتحاد {
        ngx_دننه_cidr_t         دننه;
#إذا (NGX_HAVE_INET6)
        ngx_in6_cidr_t        in6;
#د_که_پای
    } u;
} ngx_cidr_t;


تعريفنوع بنية {
    بنية sockaddr          *sockaddr;
    socklen_t                 socklen;
    ngx_str_t                 name;
} ngx_addr_t;


تعريفنوع بنية {
    ngx_str_t                 url;
    ngx_str_t                 host;
    ngx_str_t                 پورټ_متن;
    ngx_str_t                 uri;

    دننه_پورټ_t                 پورټ;
    دننه_پورټ_t                 default_پورټ;
    دننه_پورټ_t                 وروستی_پورټ;
    صحيح                       family;

    بینښه                  listen:1;
    بینښه                  uri_part:1;
    بینښه                  no_resolve:1;

    بینښه                  no_پورټ:1;
    بینښه                  wildcard:1;

    socklen_t                 socklen;
    ngx_sockaddr_t            sockaddr;

    ngx_addr_t               *addrs;
    ngx_uint_t                naddrs;

    حرف                     *err;
} ngx_url_t;


دننه_addr_t ngx_inet_addr(u_char *متن, اندازه_t len);
#إذا (NGX_HAVE_INET6)
ngx_int_t ngx_inet6_addr(u_char *p, اندازه_t len, u_char *addr);
اندازه_t ngx_inet6_ntop(u_char *p, u_char *متن, اندازه_t len);
#د_که_پای
اندازه_t ngx_sock_ntop(بنية sockaddr *sa, socklen_t socklen, u_char *متن,
    اندازه_t len, ngx_uint_t پورټ);
اندازه_t ngx_inet_ntop(صحيح family, فراغ *addr, u_char *متن, اندازه_t len);
ngx_int_t ngx_ptocidr(ngx_str_t *متن, ngx_cidr_t *cidr);
ngx_int_t ngx_cidr_سمون(بنية sockaddr *sa, ngx_array_t *cidrs);
ngx_int_t ngx_parse_addr(ngx_pool_t *pool, ngx_addr_t *addr, u_char *متن,
    اندازه_t len);
ngx_int_t ngx_parse_addr_پورټ(ngx_pool_t *pool, ngx_addr_t *addr,
    u_char *متن, اندازه_t len);
ngx_int_t ngx_parse_url(ngx_pool_t *pool, ngx_url_t *u);
ngx_int_t ngx_inet_resolve_host(ngx_pool_t *pool, ngx_url_t *u);
ngx_int_t ngx_cmp_sockaddr(بنية sockaddr *sa1, socklen_t slen1,
    بنية sockaddr *sa2, socklen_t slen2, ngx_uint_t cmp_پورټ);
دننه_پورټ_t ngx_inet_get_پورټ(بنية sockaddr *sa);
فراغ ngx_inet_سېټ_پورټ(بنية sockaddr *sa, دننه_پورټ_t پورټ);
ngx_uint_t ngx_inet_wildcard(بنية sockaddr *sa);


#د_که_پای /* _NGX_INET_H_INCLUDED_ */
