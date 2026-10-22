
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#اگر_تعریف_نشده _NGX_INET_H_INCLUDED_
#تعریف_کن _NGX_INET_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


#تعریف_کن NGX_INET_ADDRSTRLEN   (حجم("255.255.255.255") - 1)
#تعریف_کن NGX_INET6_ADDRSTRLEN                                                 \
    (حجم("ffff:ffff:ffff:ffff:ffff:ffff:255.255.255.255") - 1)
#تعریف_کن NGX_UNIX_ADDRSTRLEN                                                  \
    (حجم("unix:") - 1 +                                                   \
     حجم(بنية sockaddr_un) - offsetof(بنية sockaddr_un, sun_مسیر))

#إذا (NGX_HAVE_UNIX_DOMAIN)
#تعریف_کن NGX_SOCKADDR_STRLEN   NGX_UNIX_ADDRSTRLEN
#وإلاإذا (NGX_HAVE_INET6)
#تعریف_کن NGX_SOCKADDR_STRLEN   (NGX_INET6_ADDRSTRLEN + حجم("[]:65535") - 1)
#وإلا
#تعریف_کن NGX_SOCKADDR_STRLEN   (NGX_INET_ADDRSTRLEN + حجم(":65535") - 1)
#پایان_اگر

/* compatibility */
#تعریف_کن NGX_SOCKADDRLEN       حجم(ngx_sockaddr_t)


تعريفنوع اتحاد {
    بنية sockaddr           sockaddr;
    بنية sockaddr_درون        sockaddr_درون;
#إذا (NGX_HAVE_INET6)
    بنية sockaddr_in6       sockaddr_in6;
#پایان_اگر
#إذا (NGX_HAVE_UNIX_DOMAIN)
    بنية sockaddr_un        sockaddr_un;
#پایان_اگر
} ngx_sockaddr_t;


تعريفنوع بنية {
    درون_addr_t                 addr;
    درون_addr_t                 ماسک;
} ngx_درون_cidr_t;


#إذا (NGX_HAVE_INET6)

تعريفنوع بنية {
    بنية in6_addr           addr;
    بنية in6_addr           ماسک;
} ngx_in6_cidr_t;

#پایان_اگر


تعريفنوع بنية {
    ngx_uint_t                family;
    اتحاد {
        ngx_درون_cidr_t         درون;
#إذا (NGX_HAVE_INET6)
        ngx_in6_cidr_t        in6;
#پایان_اگر
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
    ngx_str_t                 پورت_متن;
    ngx_str_t                 uri;

    درون_پورت_t                 پورت;
    درون_پورت_t                 default_پورت;
    درون_پورت_t                 آخرین_پورت;
    صحيح                       family;

    بیعلامت                  listen:1;
    بیعلامت                  uri_part:1;
    بیعلامت                  no_resolve:1;

    بیعلامت                  no_پورت:1;
    بیعلامت                  wildcard:1;

    socklen_t                 socklen;
    ngx_sockaddr_t            sockaddr;

    ngx_addr_t               *addrs;
    ngx_uint_t                naddrs;

    حرف                     *err;
} ngx_url_t;


درون_addr_t ngx_inet_addr(u_char *متن, اندازه_t len);
#إذا (NGX_HAVE_INET6)
ngx_int_t ngx_inet6_addr(u_char *p, اندازه_t len, u_char *addr);
اندازه_t ngx_inet6_ntop(u_char *p, u_char *متن, اندازه_t len);
#پایان_اگر
اندازه_t ngx_sock_ntop(بنية sockaddr *sa, socklen_t socklen, u_char *متن,
    اندازه_t len, ngx_uint_t پورت);
اندازه_t ngx_inet_ntop(صحيح family, فراغ *addr, u_char *متن, اندازه_t len);
ngx_int_t ngx_ptocidr(ngx_str_t *متن, ngx_cidr_t *cidr);
ngx_int_t ngx_cidr_تطابق(بنية sockaddr *sa, ngx_array_t *cidrs);
ngx_int_t ngx_parse_addr(ngx_pool_t *pool, ngx_addr_t *addr, u_char *متن,
    اندازه_t len);
ngx_int_t ngx_parse_addr_پورت(ngx_pool_t *pool, ngx_addr_t *addr,
    u_char *متن, اندازه_t len);
ngx_int_t ngx_parse_url(ngx_pool_t *pool, ngx_url_t *u);
ngx_int_t ngx_inet_resolve_host(ngx_pool_t *pool, ngx_url_t *u);
ngx_int_t ngx_cmp_sockaddr(بنية sockaddr *sa1, socklen_t slen1,
    بنية sockaddr *sa2, socklen_t slen2, ngx_uint_t cmp_پورت);
درون_پورت_t ngx_inet_get_پورت(بنية sockaddr *sa);
فراغ ngx_inet_مجموعه_پورت(بنية sockaddr *sa, درون_پورت_t پورت);
ngx_uint_t ngx_inet_wildcard(بنية sockaddr *sa);


#پایان_اگر /* _NGX_INET_H_INCLUDED_ */
