
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_INET_H_INCLUDED_
#define _NGX_INET_H_INCLUDED_


#شامل <ngx_config.h>
#شامل <ngx_core.h>


#define NGX_INET_ADDRSTRLEN   (اندازه("255.255.255.255") - 1)
#define NGX_INET6_ADDRSTRLEN                                                 \
    (اندازه("ffff:ffff:ffff:ffff:ffff:ffff:255.255.255.255") - 1)
#define NGX_UNIX_ADDRSTRLEN                                                  \
    (اندازه("unix:") - 1 +                                                   \
     اندازه(ساختار sockaddr_un) - offsetof(ساختار sockaddr_un, sun_path))

#اگر (NGX_HAVE_UNIX_DOMAIN)
#define NGX_SOCKADDR_STRLEN   NGX_UNIX_ADDRSTRLEN
#وگرنهاگر (NGX_HAVE_INET6)
#define NGX_SOCKADDR_STRLEN   (NGX_INET6_ADDRSTRLEN + اندازه("[]:65535") - 1)
#وگرنه
#define NGX_SOCKADDR_STRLEN   (NGX_INET_ADDRSTRLEN + اندازه(":65535") - 1)
#endif

/* compatibility */
#define NGX_SOCKADDRLEN       اندازه(ngx_sockaddr_t)


تعریفنوع اجتماع {
    ساختار sockaddr           sockaddr;
    ساختار sockaddr_in        sockaddr_in;
#اگر (NGX_HAVE_INET6)
    ساختار sockaddr_in6       sockaddr_in6;
#endif
#اگر (NGX_HAVE_UNIX_DOMAIN)
    ساختار sockaddr_un        sockaddr_un;
#endif
} ngx_sockaddr_t;


تعریفنوع ساختار {
    in_addr_t                 addr;
    in_addr_t                 mask;
} ngx_in_cidr_t;


#اگر (NGX_HAVE_INET6)

تعریفنوع ساختار {
    ساختار in6_addr           addr;
    ساختار in6_addr           mask;
} ngx_in6_cidr_t;

#endif


تعریفنوع ساختار {
    ngx_uint_t                family;
    اجتماع {
        ngx_in_cidr_t         in;
#اگر (NGX_HAVE_INET6)
        ngx_in6_cidr_t        in6;
#endif
    } u;
} ngx_cidr_t;


تعریفنوع ساختار {
    ساختار sockaddr          *sockaddr;
    socklen_t                 socklen;
    ngx_str_t                 name;
} ngx_addr_t;


تعریفنوع ساختار {
    ngx_str_t                 url;
    ngx_str_t                 host;
    ngx_str_t                 port_text;
    ngx_str_t                 uri;

    in_port_t                 port;
    in_port_t                 default_port;
    in_port_t                 last_port;
    صحیح                       family;

    unsigned                  listen:1;
    unsigned                  uri_part:1;
    unsigned                  no_resolve:1;

    unsigned                  no_port:1;
    unsigned                  wildcard:1;

    socklen_t                 socklen;
    ngx_sockaddr_t            sockaddr;

    ngx_addr_t               *addrs;
    ngx_uint_t                naddrs;

    نویسه                     *err;
} ngx_url_t;


in_addr_t ngx_inet_addr(u_char *text, size_t len);
#اگر (NGX_HAVE_INET6)
ngx_int_t ngx_inet6_addr(u_char *p, size_t len, u_char *addr);
size_t ngx_inet6_ntop(u_char *p, u_char *text, size_t len);
#endif
size_t ngx_sock_ntop(ساختار sockaddr *sa, socklen_t socklen, u_char *text,
    size_t len, ngx_uint_t port);
size_t ngx_inet_ntop(صحیح family, تهی *addr, u_char *text, size_t len);
ngx_int_t ngx_ptocidr(ngx_str_t *text, ngx_cidr_t *cidr);
ngx_int_t ngx_cidr_match(ساختار sockaddr *sa, ngx_array_t *cidrs);
ngx_int_t ngx_parse_addr(ngx_pool_t *pool, ngx_addr_t *addr, u_char *text,
    size_t len);
ngx_int_t ngx_parse_addr_port(ngx_pool_t *pool, ngx_addr_t *addr,
    u_char *text, size_t len);
ngx_int_t ngx_parse_url(ngx_pool_t *pool, ngx_url_t *u);
ngx_int_t ngx_inet_resolve_host(ngx_pool_t *pool, ngx_url_t *u);
ngx_int_t ngx_cmp_sockaddr(ساختار sockaddr *sa1, socklen_t slen1,
    ساختار sockaddr *sa2, socklen_t slen2, ngx_uint_t cmp_port);
in_port_t ngx_inet_get_port(ساختار sockaddr *sa);
تهی ngx_inet_set_port(ساختار sockaddr *sa, in_port_t port);
ngx_uint_t ngx_inet_wildcard(ساختار sockaddr *sa);


#endif /* _NGX_INET_H_INCLUDED_ */
