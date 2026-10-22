
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_INET_H_INCLUDED_
#define _NGX_INET_H_INCLUDED_


#অন্তর্ভাব <ngx_config.h>
#অন্তর্ভাব <ngx_core.h>


#define NGX_INET_ADDRSTRLEN   (আকার("255.255.255.255") - 1)
#define NGX_INET6_ADDRSTRLEN                                                 \
    (আকার("ffff:ffff:ffff:ffff:ffff:ffff:255.255.255.255") - 1)
#define NGX_UNIX_ADDRSTRLEN                                                  \
    (আকার("unix:") - 1 +                                                   \
     আকার(কাঠামো sockaddr_un) - offsetof(কাঠামো sockaddr_un, sun_path))

#যদি (NGX_HAVE_UNIX_DOMAIN)
#define NGX_SOCKADDR_STRLEN   NGX_UNIX_ADDRSTRLEN
#নতুবা_যদি (NGX_HAVE_INET6)
#define NGX_SOCKADDR_STRLEN   (NGX_INET6_ADDRSTRLEN + আকার("[]:65535") - 1)
#অন্যথা
#define NGX_SOCKADDR_STRLEN   (NGX_INET_ADDRSTRLEN + আকার(":65535") - 1)
#endif

/* compatibility */
#define NGX_SOCKADDRLEN       আকার(ngx_sockaddr_t)


প্রকারলেখ সংঘ {
    কাঠামো sockaddr           sockaddr;
    কাঠামো sockaddr_in        sockaddr_in;
#যদি (NGX_HAVE_INET6)
    কাঠামো sockaddr_in6       sockaddr_in6;
#endif
#যদি (NGX_HAVE_UNIX_DOMAIN)
    কাঠামো sockaddr_un        sockaddr_un;
#endif
} ngx_sockaddr_t;


প্রকারলেখ কাঠামো {
    in_addr_t                 addr;
    in_addr_t                 mask;
} ngx_in_cidr_t;


#যদি (NGX_HAVE_INET6)

প্রকারলেখ কাঠামো {
    কাঠামো in6_addr           addr;
    কাঠামো in6_addr           mask;
} ngx_in6_cidr_t;

#endif


প্রকারলেখ কাঠামো {
    ngx_uint_t                family;
    সংঘ {
        ngx_in_cidr_t         in;
#যদি (NGX_HAVE_INET6)
        ngx_in6_cidr_t        in6;
#endif
    } u;
} ngx_cidr_t;


প্রকারলেখ কাঠামো {
    কাঠামো sockaddr          *sockaddr;
    socklen_t                 socklen;
    ngx_str_t                 name;
} ngx_addr_t;


প্রকারলেখ কাঠামো {
    ngx_str_t                 url;
    ngx_str_t                 host;
    ngx_str_t                 port_text;
    ngx_str_t                 uri;

    in_port_t                 port;
    in_port_t                 default_port;
    in_port_t                 last_port;
    পূর্ণসংখ্যা                       family;

    unsigned                  listen:1;
    unsigned                  uri_part:1;
    unsigned                  no_resolve:1;

    unsigned                  no_port:1;
    unsigned                  wildcard:1;

    socklen_t                 socklen;
    ngx_sockaddr_t            sockaddr;

    ngx_addr_t               *addrs;
    ngx_uint_t                naddrs;

    অক্ষর                     *err;
} ngx_url_t;


in_addr_t ngx_inet_addr(u_char *text, আকার_t len);
#যদি (NGX_HAVE_INET6)
ngx_int_t ngx_inet6_addr(u_char *p, আকার_t len, u_char *addr);
আকার_t ngx_inet6_ntop(u_char *p, u_char *text, আকার_t len);
#endif
আকার_t ngx_sock_ntop(কাঠামো sockaddr *sa, socklen_t socklen, u_char *text,
    আকার_t len, ngx_uint_t port);
আকার_t ngx_inet_ntop(পূর্ণসংখ্যা family, শূন্য *addr, u_char *text, আকার_t len);
ngx_int_t ngx_ptocidr(ngx_str_t *text, ngx_cidr_t *cidr);
ngx_int_t ngx_cidr_match(কাঠামো sockaddr *sa, ngx_array_t *cidrs);
ngx_int_t ngx_parse_addr(ngx_pool_t *pool, ngx_addr_t *addr, u_char *text,
    আকার_t len);
ngx_int_t ngx_parse_addr_port(ngx_pool_t *pool, ngx_addr_t *addr,
    u_char *text, আকার_t len);
ngx_int_t ngx_parse_url(ngx_pool_t *pool, ngx_url_t *u);
ngx_int_t ngx_inet_resolve_host(ngx_pool_t *pool, ngx_url_t *u);
ngx_int_t ngx_cmp_sockaddr(কাঠামো sockaddr *sa1, socklen_t slen1,
    কাঠামো sockaddr *sa2, socklen_t slen2, ngx_uint_t cmp_port);
in_port_t ngx_inet_get_port(কাঠামো sockaddr *sa);
শূন্য ngx_inet_set_port(কাঠামো sockaddr *sa, in_port_t port);
ngx_uint_t ngx_inet_wildcard(কাঠামো sockaddr *sa);


#endif /* _NGX_INET_H_INCLUDED_ */
