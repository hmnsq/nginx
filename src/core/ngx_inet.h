
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_INET_H_INCLUDED_
#define _NGX_INET_H_INCLUDED_


#અન્તર્ભાવ <ngx_config.h>
#અન્તર્ભાવ <ngx_core.h>


#define NGX_INET_ADDRSTRLEN   (કદ("255.255.255.255") - 1)
#define NGX_INET6_ADDRSTRLEN                                                 \
    (કદ("ffff:ffff:ffff:ffff:ffff:ffff:255.255.255.255") - 1)
#define NGX_UNIX_ADDRSTRLEN                                                  \
    (કદ("unix:") - 1 +                                                   \
     કદ(સંરચના sockaddr_un) - offsetof(સંરચના sockaddr_un, sun_path))

#જો (NGX_HAVE_UNIX_DOMAIN)
#define NGX_SOCKADDR_STRLEN   NGX_UNIX_ADDRSTRLEN
#અથવાજો (NGX_HAVE_INET6)
#define NGX_SOCKADDR_STRLEN   (NGX_INET6_ADDRSTRLEN + કદ("[]:65535") - 1)
#નહીંતર
#define NGX_SOCKADDR_STRLEN   (NGX_INET_ADDRSTRLEN + કદ(":65535") - 1)
#endif

/* compatibility */
#define NGX_SOCKADDRLEN       કદ(ngx_sockaddr_t)


પ્રકારવ્યાખ્યા સંઘ {
    સંરચના sockaddr           sockaddr;
    સંરચના sockaddr_in        sockaddr_in;
#જો (NGX_HAVE_INET6)
    સંરચના sockaddr_in6       sockaddr_in6;
#endif
#જો (NGX_HAVE_UNIX_DOMAIN)
    સંરચના sockaddr_un        sockaddr_un;
#endif
} ngx_sockaddr_t;


પ્રકારવ્યાખ્યા સંરચના {
    in_addr_t                 addr;
    in_addr_t                 mask;
} ngx_in_cidr_t;


#જો (NGX_HAVE_INET6)

પ્રકારવ્યાખ્યા સંરચના {
    સંરચના in6_addr           addr;
    સંરચના in6_addr           mask;
} ngx_in6_cidr_t;

#endif


પ્રકારવ્યાખ્યા સંરચના {
    ngx_uint_t                family;
    સંઘ {
        ngx_in_cidr_t         in;
#જો (NGX_HAVE_INET6)
        ngx_in6_cidr_t        in6;
#endif
    } u;
} ngx_cidr_t;


પ્રકારવ્યાખ્યા સંરચના {
    સંરચના sockaddr          *sockaddr;
    socklen_t                 socklen;
    ngx_str_t                 name;
} ngx_addr_t;


પ્રકારવ્યાખ્યા સંરચના {
    ngx_str_t                 url;
    ngx_str_t                 host;
    ngx_str_t                 port_text;
    ngx_str_t                 uri;

    in_port_t                 port;
    in_port_t                 default_port;
    in_port_t                 last_port;
    પૂર્ણાંક                       family;

    unsigned                  listen:1;
    unsigned                  uri_part:1;
    unsigned                  no_resolve:1;

    unsigned                  no_port:1;
    unsigned                  wildcard:1;

    socklen_t                 socklen;
    ngx_sockaddr_t            sockaddr;

    ngx_addr_t               *addrs;
    ngx_uint_t                naddrs;

    અક્ષર                     *err;
} ngx_url_t;


in_addr_t ngx_inet_addr(u_char *text, size_t len);
#જો (NGX_HAVE_INET6)
ngx_int_t ngx_inet6_addr(u_char *p, size_t len, u_char *addr);
size_t ngx_inet6_ntop(u_char *p, u_char *text, size_t len);
#endif
size_t ngx_sock_ntop(સંરચના sockaddr *sa, socklen_t socklen, u_char *text,
    size_t len, ngx_uint_t port);
size_t ngx_inet_ntop(પૂર્ણાંક family, શૂન્ય *addr, u_char *text, size_t len);
ngx_int_t ngx_ptocidr(ngx_str_t *text, ngx_cidr_t *cidr);
ngx_int_t ngx_cidr_match(સંરચના sockaddr *sa, ngx_array_t *cidrs);
ngx_int_t ngx_parse_addr(ngx_pool_t *pool, ngx_addr_t *addr, u_char *text,
    size_t len);
ngx_int_t ngx_parse_addr_port(ngx_pool_t *pool, ngx_addr_t *addr,
    u_char *text, size_t len);
ngx_int_t ngx_parse_url(ngx_pool_t *pool, ngx_url_t *u);
ngx_int_t ngx_inet_resolve_host(ngx_pool_t *pool, ngx_url_t *u);
ngx_int_t ngx_cmp_sockaddr(સંરચના sockaddr *sa1, socklen_t slen1,
    સંરચના sockaddr *sa2, socklen_t slen2, ngx_uint_t cmp_port);
in_port_t ngx_inet_get_port(સંરચના sockaddr *sa);
શૂન્ય ngx_inet_set_port(સંરચના sockaddr *sa, in_port_t port);
ngx_uint_t ngx_inet_wildcard(સંરચના sockaddr *sa);


#endif /* _NGX_INET_H_INCLUDED_ */
