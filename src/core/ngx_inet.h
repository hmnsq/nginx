
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#اگر_غیر_تعریف_شدہ _NGX_INET_H_INCLUDED_
#تعریف_کرو _NGX_INET_H_INCLUDED_


#شامل <ngx_config.h>
#شامل <ngx_core.h>


#تعریف_کرو NGX_INET_ADDRSTRLEN   (حجم("255.255.255.255") - 1)
#تعریف_کرو NGX_INET6_ADDRSTRLEN                                                 \
    (حجم("ffff:ffff:ffff:ffff:ffff:ffff:255.255.255.255") - 1)
#تعریف_کرو NGX_UNIX_ADDRSTRLEN                                                  \
    (حجم("unix:") - 1 +                                                   \
     حجم(ساخت sockaddr_un) - offsetof(ساخت sockaddr_un, sun_راستہ))

#اگر (NGX_HAVE_UNIX_DOMAIN)
#تعریف_کرو NGX_SOCKADDR_STRLEN   NGX_UNIX_ADDRSTRLEN
#وگرنہ_اگر (NGX_HAVE_INET6)
#تعریف_کرو NGX_SOCKADDR_STRLEN   (NGX_INET6_ADDRSTRLEN + حجم("[]:65535") - 1)
#ورنہ
#تعریف_کرو NGX_SOCKADDR_STRLEN   (NGX_INET_ADDRSTRLEN + حجم(":65535") - 1)
#اگر_ختم

/* compatibility */
#تعریف_کرو NGX_SOCKADDRLEN       حجم(ngx_sockaddr_t)


قسمتعریف اتحاد {
    ساخت sockaddr           sockaddr;
    ساخت sockaddr_اندر        sockaddr_اندر;
#اگر (NGX_HAVE_INET6)
    ساخت sockaddr_in6       sockaddr_in6;
#اگر_ختم
#اگر (NGX_HAVE_UNIX_DOMAIN)
    ساخت sockaddr_un        sockaddr_un;
#اگر_ختم
} ngx_sockaddr_t;


قسمتعریف ساخت {
    اندر_addr_t                 addr;
    اندر_addr_t                 نقاب;
} ngx_اندر_cidr_t;


#اگر (NGX_HAVE_INET6)

قسمتعریف ساخت {
    ساخت in6_addr           addr;
    ساخت in6_addr           نقاب;
} ngx_in6_cidr_t;

#اگر_ختم


قسمتعریف ساخت {
    ngx_uint_t                family;
    اتحاد {
        ngx_اندر_cidr_t         اندر;
#اگر (NGX_HAVE_INET6)
        ngx_in6_cidr_t        in6;
#اگر_ختم
    } u;
} ngx_cidr_t;


قسمتعریف ساخت {
    ساخت sockaddr          *sockaddr;
    socklen_t                 socklen;
    ngx_str_t                 name;
} ngx_addr_t;


قسمتعریف ساخت {
    ngx_str_t                 url;
    ngx_str_t                 host;
    ngx_str_t                 پورٹ_متن;
    ngx_str_t                 uri;

    اندر_پورٹ_t                 پورٹ;
    اندر_پورٹ_t                 default_پورٹ;
    اندر_پورٹ_t                 آخری_پورٹ;
    صحیح                       family;

    غیرعلامتی                  listen:1;
    غیرعلامتی                  uri_part:1;
    غیرعلامتی                  no_resolve:1;

    غیرعلامتی                  no_پورٹ:1;
    غیرعلامتی                  wildcard:1;

    socklen_t                 socklen;
    ngx_sockaddr_t            sockaddr;

    ngx_addr_t               *addrs;
    ngx_uint_t                naddrs;

    حرف                     *err;
} ngx_url_t;


اندر_addr_t ngx_inet_addr(u_char *متن, سائز_t len);
#اگر (NGX_HAVE_INET6)
ngx_int_t ngx_inet6_addr(u_char *p, سائز_t len, u_char *addr);
سائز_t ngx_inet6_ntop(u_char *p, u_char *متن, سائز_t len);
#اگر_ختم
سائز_t ngx_sock_ntop(ساخت sockaddr *sa, socklen_t socklen, u_char *متن,
    سائز_t len, ngx_uint_t پورٹ);
سائز_t ngx_inet_ntop(صحیح family, خالی *addr, u_char *متن, سائز_t len);
ngx_int_t ngx_ptocidr(ngx_str_t *متن, ngx_cidr_t *cidr);
ngx_int_t ngx_cidr_مطابقت(ساخت sockaddr *sa, ngx_array_t *cidrs);
ngx_int_t ngx_parse_addr(ngx_pool_t *pool, ngx_addr_t *addr, u_char *متن,
    سائز_t len);
ngx_int_t ngx_parse_addr_پورٹ(ngx_pool_t *pool, ngx_addr_t *addr,
    u_char *متن, سائز_t len);
ngx_int_t ngx_parse_url(ngx_pool_t *pool, ngx_url_t *u);
ngx_int_t ngx_inet_resolve_host(ngx_pool_t *pool, ngx_url_t *u);
ngx_int_t ngx_cmp_sockaddr(ساخت sockaddr *sa1, socklen_t slen1,
    ساخت sockaddr *sa2, socklen_t slen2, ngx_uint_t cmp_پورٹ);
اندر_پورٹ_t ngx_inet_get_پورٹ(ساخت sockaddr *sa);
خالی ngx_inet_سیٹ_پورٹ(ساخت sockaddr *sa, اندر_پورٹ_t پورٹ);
ngx_uint_t ngx_inet_wildcard(ساخت sockaddr *sa);


#اگر_ختم /* _NGX_INET_H_INCLUDED_ */
