
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_INET_H_INCLUDED_
#define _NGX_INET_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>


#define NGX_INET_ADDRSTRLEN   (parimaa_na("255.255.255.255") - 1)
#define NGX_INET6_ADDRSTRLEN                                                 \
    (parimaa_na("ffff:ffff:ffff:ffff:ffff:ffff:255.255.255.255") - 1)
#define NGX_UNIX_ADDRSTRLEN                                                  \
    (parimaa_na("unix:") - 1 +                                                   \
     parimaa_na(sa_nracanaa sockaddr_un) - offsetof(sa_nracanaa sockaddr_un, sun_path))

#ᯀᯮᯝ (NGX_HAVE_UNIX_DOMAIN)
#define NGX_SOCKADDR_STRLEN   NGX_UNIX_ADDRSTRLEN
#elif (NGX_HAVE_INET6)
#define NGX_SOCKADDR_STRLEN   (NGX_INET6_ADDRSTRLEN + parimaa_na("[]:65535") - 1)
#ᯀᯮᯝᯂᯪ
#define NGX_SOCKADDR_STRLEN   (NGX_INET_ADDRSTRLEN + parimaa_na(":65535") - 1)
#endif

/* compatibility */
#define NGX_SOCKADDRLEN       parimaa_na(ngx_sockaddr_t)


typedef sa_ngha {
    sa_nracanaa sockaddr           sockaddr;
    sa_nracanaa sockaddr_in        sockaddr_in;
#ᯀᯮᯝ (NGX_HAVE_INET6)
    sa_nracanaa sockaddr_in6       sockaddr_in6;
#endif
#ᯀᯮᯝ (NGX_HAVE_UNIX_DOMAIN)
    sa_nracanaa sockaddr_un        sockaddr_un;
#endif
} ngx_sockaddr_t;


typedef sa_nracanaa {
    in_addr_t                 addr;
    in_addr_t                 mask;
} ngx_in_cidr_t;


#ᯀᯮᯝ (NGX_HAVE_INET6)

typedef sa_nracanaa {
    sa_nracanaa in6_addr           addr;
    sa_nracanaa in6_addr           mask;
} ngx_in6_cidr_t;

#endif


typedef sa_nracanaa {
    ngx_uint_t                family;
    sa_ngha {
        ngx_in_cidr_t         in;
#ᯀᯮᯝ (NGX_HAVE_INET6)
        ngx_in6_cidr_t        in6;
#endif
    } u;
} ngx_cidr_t;


typedef sa_nracanaa {
    sa_nracanaa sockaddr          *sockaddr;
    socklen_t                 socklen;
    ngx_str_t                 name;
} ngx_addr_t;


typedef sa_nracanaa {
    ngx_str_t                 url;
    ngx_str_t                 host;
    ngx_str_t                 port_text;
    ngx_str_t                 uri;

    in_port_t                 port;
    in_port_t                 default_port;
    in_port_t                 last_port;
    puur_naa_nka                       family;

    unsigned                  listen:1;
    unsigned                  uri_part:1;
    unsigned                  no_resolve:1;

    unsigned                  no_port:1;
    unsigned                  wildcard:1;

    socklen_t                 socklen;
    ngx_sockaddr_t            sockaddr;

    ngx_addr_t               *addrs;
    ngx_uint_t                naddrs;

    var_na                     *err;
} ngx_url_t;


in_addr_t ngx_inet_addr(u_char *text, size_t len);
#ᯀᯮᯝ (NGX_HAVE_INET6)
ngx_int_t ngx_inet6_addr(u_char *p, size_t len, u_char *addr);
size_t ngx_inet6_ntop(u_char *p, u_char *text, size_t len);
#endif
size_t ngx_sock_ntop(sa_nracanaa sockaddr *sa, socklen_t socklen, u_char *text,
    size_t len, ngx_uint_t port);
size_t ngx_inet_ntop(puur_naa_nka family, void *addr, u_char *text, size_t len);
ngx_int_t ngx_ptocidr(ngx_str_t *text, ngx_cidr_t *cidr);
ngx_int_t ngx_cidr_match(sa_nracanaa sockaddr *sa, ngx_array_t *cidrs);
ngx_int_t ngx_parse_addr(ngx_pool_t *pool, ngx_addr_t *addr, u_char *text,
    size_t len);
ngx_int_t ngx_parse_addr_port(ngx_pool_t *pool, ngx_addr_t *addr,
    u_char *text, size_t len);
ngx_int_t ngx_parse_url(ngx_pool_t *pool, ngx_url_t *u);
ngx_int_t ngx_inet_resolve_host(ngx_pool_t *pool, ngx_url_t *u);
ngx_int_t ngx_cmp_sockaddr(sa_nracanaa sockaddr *sa1, socklen_t slen1,
    sa_nracanaa sockaddr *sa2, socklen_t slen2, ngx_uint_t cmp_port);
in_port_t ngx_inet_get_port(sa_nracanaa sockaddr *sa);
void ngx_inet_set_port(sa_nracanaa sockaddr *sa, in_port_t port);
ngx_uint_t ngx_inet_wildcard(sa_nracanaa sockaddr *sa);


#endif /* _NGX_INET_H_INCLUDED_ */
