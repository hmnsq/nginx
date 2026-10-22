
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#جيڪڏهن_وصف_نه_ٿيل _NGX_INET_H_INCLUDED_
#وصف_ڪريو _NGX_INET_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


#وصف_ڪريو NGX_INET_ADDRSTRLEN   (حجم("255.255.255.255") - 1)
#وصف_ڪريو NGX_INET6_ADDRSTRLEN                                                 \
    (حجم("ffff:ffff:ffff:ffff:ffff:ffff:255.255.255.255") - 1)
#وصف_ڪريو NGX_UNIX_ADDRSTRLEN                                                  \
    (حجم("unix:") - 1 +                                                   \
     حجم(بنية sockaddr_un) - offsetof(بنية sockaddr_un, sun_رستو))

#إذا (NGX_HAVE_UNIX_DOMAIN)
#وصف_ڪريو NGX_SOCKADDR_STRLEN   NGX_UNIX_ADDRSTRLEN
#وإلاإذا (NGX_HAVE_INET6)
#وصف_ڪريو NGX_SOCKADDR_STRLEN   (NGX_INET6_ADDRSTRLEN + حجم("[]:65535") - 1)
#وإلا
#وصف_ڪريو NGX_SOCKADDR_STRLEN   (NGX_INET_ADDRSTRLEN + حجم(":65535") - 1)
#جيڪڏهن_پورو

/* compatibility */
#وصف_ڪريو NGX_SOCKADDRLEN       حجم(ngx_sockaddr_t)


تعريفنوع اتحاد {
    بنية sockaddr           sockaddr;
    بنية sockaddr_اندريون        sockaddr_اندريون;
#إذا (NGX_HAVE_INET6)
    بنية sockaddr_in6       sockaddr_in6;
#جيڪڏهن_پورو
#إذا (NGX_HAVE_UNIX_DOMAIN)
    بنية sockaddr_un        sockaddr_un;
#جيڪڏهن_پورو
} ngx_sockaddr_t;


تعريفنوع بنية {
    اندريون_addr_t                 addr;
    اندريون_addr_t                 نقاب;
} ngx_اندريون_cidr_t;


#إذا (NGX_HAVE_INET6)

تعريفنوع بنية {
    بنية in6_addr           addr;
    بنية in6_addr           نقاب;
} ngx_in6_cidr_t;

#جيڪڏهن_پورو


تعريفنوع بنية {
    ngx_uint_t                family;
    اتحاد {
        ngx_اندريون_cidr_t         اندريون;
#إذا (NGX_HAVE_INET6)
        ngx_in6_cidr_t        in6;
#جيڪڏهن_پورو
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
    ngx_str_t                 پورٽ_متن;
    ngx_str_t                 uri;

    اندريون_پورٽ_t                 پورٽ;
    اندريون_پورٽ_t                 default_پورٽ;
    اندريون_پورٽ_t                 آخري_پورٽ;
    صحيح                       family;

    بنانشان                  listen:1;
    بنانشان                  uri_part:1;
    بنانشان                  no_resolve:1;

    بنانشان                  no_پورٽ:1;
    بنانشان                  wildcard:1;

    socklen_t                 socklen;
    ngx_sockaddr_t            sockaddr;

    ngx_addr_t               *addrs;
    ngx_uint_t                naddrs;

    حرف                     *err;
} ngx_url_t;


اندريون_addr_t ngx_inet_addr(u_char *متن, سائيز_t len);
#إذا (NGX_HAVE_INET6)
ngx_int_t ngx_inet6_addr(u_char *p, سائيز_t len, u_char *addr);
سائيز_t ngx_inet6_ntop(u_char *p, u_char *متن, سائيز_t len);
#جيڪڏهن_پورو
سائيز_t ngx_sock_ntop(بنية sockaddr *sa, socklen_t socklen, u_char *متن,
    سائيز_t len, ngx_uint_t پورٽ);
سائيز_t ngx_inet_ntop(صحيح family, فراغ *addr, u_char *متن, سائيز_t len);
ngx_int_t ngx_ptocidr(ngx_str_t *متن, ngx_cidr_t *cidr);
ngx_int_t ngx_cidr_ميلاپ(بنية sockaddr *sa, ngx_array_t *cidrs);
ngx_int_t ngx_parse_addr(ngx_pool_t *pool, ngx_addr_t *addr, u_char *متن,
    سائيز_t len);
ngx_int_t ngx_parse_addr_پورٽ(ngx_pool_t *pool, ngx_addr_t *addr,
    u_char *متن, سائيز_t len);
ngx_int_t ngx_parse_url(ngx_pool_t *pool, ngx_url_t *u);
ngx_int_t ngx_inet_resolve_host(ngx_pool_t *pool, ngx_url_t *u);
ngx_int_t ngx_cmp_sockaddr(بنية sockaddr *sa1, socklen_t slen1,
    بنية sockaddr *sa2, socklen_t slen2, ngx_uint_t cmp_پورٽ);
اندريون_پورٽ_t ngx_inet_get_پورٽ(بنية sockaddr *sa);
فراغ ngx_inet_سيٽ_پورٽ(بنية sockaddr *sa, اندريون_پورٽ_t پورٽ);
ngx_uint_t ngx_inet_wildcard(بنية sockaddr *sa);


#جيڪڏهن_پورو /* _NGX_INET_H_INCLUDED_ */
