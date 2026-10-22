
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_INET_H_INCLUDED_
#define _NGX_INET_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


#define NGX_INET_ADDRSTRLEN   (حجم("255.255.255.255") - 1)
#define NGX_INET6_ADDRSTRLEN                                                 \
    (حجم("ffff:ffff:ffff:ffff:ffff:ffff:255.255.255.255") - 1)
#define NGX_UNIX_ADDRSTRLEN                                                  \
    (حجم("unix:") - 1 +                                                   \
     حجم(بنية sockaddr_un) - offsetof(بنية sockaddr_un, sun_path))

#إذا (NGX_HAVE_UNIX_DOMAIN)
#define NGX_SOCKADDR_STRLEN   NGX_UNIX_ADDRSTRLEN
#وإلاإذا (NGX_HAVE_INET6)
#define NGX_SOCKADDR_STRLEN   (NGX_INET6_ADDRSTRLEN + حجم("[]:65535") - 1)
#وإلا
#define NGX_SOCKADDR_STRLEN   (NGX_INET_ADDRSTRLEN + حجم(":65535") - 1)
#endif

/* compatibility */
#define NGX_SOCKADDRLEN       حجم(ngx_sockaddr_t)


تعريفنوع اتحاد {
    بنية sockaddr           sockaddr;
    بنية sockaddr_in        sockaddr_in;
#إذا (NGX_HAVE_INET6)
    بنية sockaddr_in6       sockaddr_in6;
#endif
#إذا (NGX_HAVE_UNIX_DOMAIN)
    بنية sockaddr_un        sockaddr_un;
#endif
} ngx_sockaddr_t;


تعريفنوع بنية {
    in_addr_t                 addr;
    in_addr_t                 mask;
} ngx_in_cidr_t;


#إذا (NGX_HAVE_INET6)

تعريفنوع بنية {
    بنية in6_addr           addr;
    بنية in6_addr           mask;
} ngx_in6_cidr_t;

#endif


تعريفنوع بنية {
    ngx_uint_t                family;
    اتحاد {
        ngx_in_cidr_t         in;
#إذا (NGX_HAVE_INET6)
        ngx_in6_cidr_t        in6;
#endif
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
    ngx_str_t                 port_text;
    ngx_str_t                 uri;

    in_port_t                 port;
    in_port_t                 default_port;
    in_port_t                 last_port;
    صحيح                       family;

    unsigned                  listen:1;
    unsigned                  uri_part:1;
    unsigned                  no_resolve:1;

    unsigned                  no_port:1;
    unsigned                  wildcard:1;

    socklen_t                 socklen;
    ngx_sockaddr_t            sockaddr;

    ngx_addr_t               *addrs;
    ngx_uint_t                naddrs;

    حرف                     *err;
} ngx_url_t;


in_addr_t ngx_inet_addr(u_char *text, size_t len);
#إذا (NGX_HAVE_INET6)
ngx_int_t ngx_inet6_addr(u_char *p, size_t len, u_char *addr);
size_t ngx_inet6_ntop(u_char *p, u_char *text, size_t len);
#endif
size_t ngx_sock_ntop(بنية sockaddr *sa, socklen_t socklen, u_char *text,
    size_t len, ngx_uint_t port);
size_t ngx_inet_ntop(صحيح family, فراغ *addr, u_char *text, size_t len);
ngx_int_t ngx_ptocidr(ngx_str_t *text, ngx_cidr_t *cidr);
ngx_int_t ngx_cidr_match(بنية sockaddr *sa, ngx_array_t *cidrs);
ngx_int_t ngx_parse_addr(ngx_pool_t *pool, ngx_addr_t *addr, u_char *text,
    size_t len);
ngx_int_t ngx_parse_addr_port(ngx_pool_t *pool, ngx_addr_t *addr,
    u_char *text, size_t len);
ngx_int_t ngx_parse_url(ngx_pool_t *pool, ngx_url_t *u);
ngx_int_t ngx_inet_resolve_host(ngx_pool_t *pool, ngx_url_t *u);
ngx_int_t ngx_cmp_sockaddr(بنية sockaddr *sa1, socklen_t slen1,
    بنية sockaddr *sa2, socklen_t slen2, ngx_uint_t cmp_port);
in_port_t ngx_inet_get_port(بنية sockaddr *sa);
فراغ ngx_inet_set_port(بنية sockaddr *sa, in_port_t port);
ngx_uint_t ngx_inet_wildcard(بنية sockaddr *sa);


#endif /* _NGX_INET_H_INCLUDED_ */
