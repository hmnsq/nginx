
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#جے_بنا_تعریف _NGX_INET_H_INCLUDED_
#تعریف_کرو _NGX_INET_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


#تعریف_کرو NGX_INET_ADDRSTRLEN   (حجم("255.255.255.255") - 1)
#تعریف_کرو NGX_INET6_ADDRSTRLEN                                                 \
    (حجم("ffff:ffff:ffff:ffff:ffff:ffff:255.255.255.255") - 1)
#تعریف_کرو NGX_UNIX_ADDRSTRLEN                                                  \
    (حجم("unix:") - 1 +                                                   \
     حجم(بنية sockaddr_un) - offsetof(بنية sockaddr_un, sun_رستہ))

#إذا (NGX_HAVE_UNIX_DOMAIN)
#تعریف_کرو NGX_SOCKADDR_STRLEN   NGX_UNIX_ADDRSTRLEN
#وإلاإذا (NGX_HAVE_INET6)
#تعریف_کرو NGX_SOCKADDR_STRLEN   (NGX_INET6_ADDRSTRLEN + حجم("[]:65535") - 1)
#وإلا
#تعریف_کرو NGX_SOCKADDR_STRLEN   (NGX_INET_ADDRSTRLEN + حجم(":65535") - 1)
#جے_انت

/* compatibility */
#تعریف_کرو NGX_SOCKADDRLEN       حجم(ngx_sockaddr_t)


تعريفنوع اتحاد {
    بنية sockaddr           sockaddr;
    بنية sockaddr_وچ        sockaddr_وچ;
#إذا (NGX_HAVE_INET6)
    بنية sockaddr_in6       sockaddr_in6;
#جے_انت
#إذا (NGX_HAVE_UNIX_DOMAIN)
    بنية sockaddr_un        sockaddr_un;
#جے_انت
} ngx_sockaddr_t;


تعريفنوع بنية {
    وچ_addr_t                 addr;
    وچ_addr_t                 نقاب;
} ngx_وچ_cidr_t;


#إذا (NGX_HAVE_INET6)

تعريفنوع بنية {
    بنية in6_addr           addr;
    بنية in6_addr           نقاب;
} ngx_in6_cidr_t;

#جے_انت


تعريفنوع بنية {
    ngx_uint_t                family;
    اتحاد {
        ngx_وچ_cidr_t         وچ;
#إذا (NGX_HAVE_INET6)
        ngx_in6_cidr_t        in6;
#جے_انت
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
    ngx_str_t                 پورٹ_متن;
    ngx_str_t                 uri;

    وچ_پورٹ_t                 پورٹ;
    وچ_پورٹ_t                 default_پورٹ;
    وچ_پورٹ_t                 آخری_پورٹ;
    صحيح                       family;

    بنانشانی                  listen:1;
    بنانشانی                  uri_part:1;
    بنانشانی                  no_resolve:1;

    بنانشانی                  no_پورٹ:1;
    بنانشانی                  wildcard:1;

    socklen_t                 socklen;
    ngx_sockaddr_t            sockaddr;

    ngx_addr_t               *addrs;
    ngx_uint_t                naddrs;

    حرف                     *err;
} ngx_url_t;


وچ_addr_t ngx_inet_addr(u_char *متن, ناپ_t len);
#إذا (NGX_HAVE_INET6)
ngx_int_t ngx_inet6_addr(u_char *p, ناپ_t len, u_char *addr);
ناپ_t ngx_inet6_ntop(u_char *p, u_char *متن, ناپ_t len);
#جے_انت
ناپ_t ngx_sock_ntop(بنية sockaddr *sa, socklen_t socklen, u_char *متن,
    ناپ_t len, ngx_uint_t پورٹ);
ناپ_t ngx_inet_ntop(صحيح family, فراغ *addr, u_char *متن, ناپ_t len);
ngx_int_t ngx_ptocidr(ngx_str_t *متن, ngx_cidr_t *cidr);
ngx_int_t ngx_cidr_میل(بنية sockaddr *sa, ngx_array_t *cidrs);
ngx_int_t ngx_parse_addr(ngx_pool_t *pool, ngx_addr_t *addr, u_char *متن,
    ناپ_t len);
ngx_int_t ngx_parse_addr_پورٹ(ngx_pool_t *pool, ngx_addr_t *addr,
    u_char *متن, ناپ_t len);
ngx_int_t ngx_parse_url(ngx_pool_t *pool, ngx_url_t *u);
ngx_int_t ngx_inet_resolve_host(ngx_pool_t *pool, ngx_url_t *u);
ngx_int_t ngx_cmp_sockaddr(بنية sockaddr *sa1, socklen_t slen1,
    بنية sockaddr *sa2, socklen_t slen2, ngx_uint_t cmp_پورٹ);
وچ_پورٹ_t ngx_inet_get_پورٹ(بنية sockaddr *sa);
فراغ ngx_inet_سیٹ_پورٹ(بنية sockaddr *sa, وچ_پورٹ_t پورٹ);
ngx_uint_t ngx_inet_wildcard(بنية sockaddr *sa);


#جے_انت /* _NGX_INET_H_INCLUDED_ */
