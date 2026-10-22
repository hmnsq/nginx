
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_INET_H_INCLUDED_
#define _NGX_INET_H_INCLUDED_


#അന്തര്ഭാവ <ngx_config.h>
#അന്തര്ഭാവ <ngx_core.h>


#define NGX_INET_ADDRSTRLEN   (വലുപ്പം("255.255.255.255") - 1)
#define NGX_INET6_ADDRSTRLEN                                                 \
    (വലുപ്പം("ffff:ffff:ffff:ffff:ffff:ffff:255.255.255.255") - 1)
#define NGX_UNIX_ADDRSTRLEN                                                  \
    (വലുപ്പം("unix:") - 1 +                                                   \
     വലുപ്പം(ഘടന sockaddr_un) - offsetof(ഘടന sockaddr_un, sun_path))

#എങ്കിൽ (NGX_HAVE_UNIX_DOMAIN)
#define NGX_SOCKADDR_STRLEN   NGX_UNIX_ADDRSTRLEN
#അല്ലെങ്കിലെങ്കിൽ (NGX_HAVE_INET6)
#define NGX_SOCKADDR_STRLEN   (NGX_INET6_ADDRSTRLEN + വലുപ്പം("[]:65535") - 1)
#അല്ലെങ്കിൽ
#define NGX_SOCKADDR_STRLEN   (NGX_INET_ADDRSTRLEN + വലുപ്പം(":65535") - 1)
#endif

/* compatibility */
#define NGX_SOCKADDRLEN       വലുപ്പം(ngx_sockaddr_t)


തരനിർവചനം സംഘം {
    ഘടന sockaddr           sockaddr;
    ഘടന sockaddr_in        sockaddr_in;
#എങ്കിൽ (NGX_HAVE_INET6)
    ഘടന sockaddr_in6       sockaddr_in6;
#endif
#എങ്കിൽ (NGX_HAVE_UNIX_DOMAIN)
    ഘടന sockaddr_un        sockaddr_un;
#endif
} ngx_sockaddr_t;


തരനിർവചനം ഘടന {
    in_addr_t                 addr;
    in_addr_t                 mask;
} ngx_in_cidr_t;


#എങ്കിൽ (NGX_HAVE_INET6)

തരനിർവചനം ഘടന {
    ഘടന in6_addr           addr;
    ഘടന in6_addr           mask;
} ngx_in6_cidr_t;

#endif


തരനിർവചനം ഘടന {
    ngx_uint_t                family;
    സംഘം {
        ngx_in_cidr_t         in;
#എങ്കിൽ (NGX_HAVE_INET6)
        ngx_in6_cidr_t        in6;
#endif
    } u;
} ngx_cidr_t;


തരനിർവചനം ഘടന {
    ഘടന sockaddr          *sockaddr;
    socklen_t                 socklen;
    ngx_str_t                 name;
} ngx_addr_t;


തരനിർവചനം ഘടന {
    ngx_str_t                 url;
    ngx_str_t                 host;
    ngx_str_t                 port_text;
    ngx_str_t                 uri;

    in_port_t                 port;
    in_port_t                 default_port;
    in_port_t                 last_port;
    പൂർണ്ണസംഖ്യ                       family;

    unsigned                  listen:1;
    unsigned                  uri_part:1;
    unsigned                  no_resolve:1;

    unsigned                  no_port:1;
    unsigned                  wildcard:1;

    socklen_t                 socklen;
    ngx_sockaddr_t            sockaddr;

    ngx_addr_t               *addrs;
    ngx_uint_t                naddrs;

    അക്ഷരം                     *err;
} ngx_url_t;


in_addr_t ngx_inet_addr(u_char *text, size_t len);
#എങ്കിൽ (NGX_HAVE_INET6)
ngx_int_t ngx_inet6_addr(u_char *p, size_t len, u_char *addr);
size_t ngx_inet6_ntop(u_char *p, u_char *text, size_t len);
#endif
size_t ngx_sock_ntop(ഘടന sockaddr *sa, socklen_t socklen, u_char *text,
    size_t len, ngx_uint_t port);
size_t ngx_inet_ntop(പൂർണ്ണസംഖ്യ family, ശൂന്യം *addr, u_char *text, size_t len);
ngx_int_t ngx_ptocidr(ngx_str_t *text, ngx_cidr_t *cidr);
ngx_int_t ngx_cidr_match(ഘടന sockaddr *sa, ngx_array_t *cidrs);
ngx_int_t ngx_parse_addr(ngx_pool_t *pool, ngx_addr_t *addr, u_char *text,
    size_t len);
ngx_int_t ngx_parse_addr_port(ngx_pool_t *pool, ngx_addr_t *addr,
    u_char *text, size_t len);
ngx_int_t ngx_parse_url(ngx_pool_t *pool, ngx_url_t *u);
ngx_int_t ngx_inet_resolve_host(ngx_pool_t *pool, ngx_url_t *u);
ngx_int_t ngx_cmp_sockaddr(ഘടന sockaddr *sa1, socklen_t slen1,
    ഘടന sockaddr *sa2, socklen_t slen2, ngx_uint_t cmp_port);
in_port_t ngx_inet_get_port(ഘടന sockaddr *sa);
ശൂന്യം ngx_inet_set_port(ഘടന sockaddr *sa, in_port_t port);
ngx_uint_t ngx_inet_wildcard(ഘടന sockaddr *sa);


#endif /* _NGX_INET_H_INCLUDED_ */
