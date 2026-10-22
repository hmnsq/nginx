
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_INET_H_INCLUDED_
#define _NGX_INET_H_INCLUDED_


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


#define NGX_INET_ADDRSTRLEN   (ಪರಿಮಾಣ("255.255.255.255") - 1)
#define NGX_INET6_ADDRSTRLEN                                                 \
    (ಪರಿಮಾಣ("ffff:ffff:ffff:ffff:ffff:ffff:255.255.255.255") - 1)
#define NGX_UNIX_ADDRSTRLEN                                                  \
    (ಪರಿಮಾಣ("unix:") - 1 +                                                   \
     ಪರಿಮಾಣ(ಸಂರಚನಾ sockaddr_un) - offsetof(ಸಂರಚನಾ sockaddr_un, sun_path))

#ಯದಿ (NGX_HAVE_UNIX_DOMAIN)
#define NGX_SOCKADDR_STRLEN   NGX_UNIX_ADDRSTRLEN
#ನೋಚೇದ್ಯದಿ (NGX_HAVE_INET6)
#define NGX_SOCKADDR_STRLEN   (NGX_INET6_ADDRSTRLEN + ಪರಿಮಾಣ("[]:65535") - 1)
#ಅನ್ಯಥಾ
#define NGX_SOCKADDR_STRLEN   (NGX_INET_ADDRSTRLEN + ಪರಿಮಾಣ(":65535") - 1)
#endif

/* compatibility */
#define NGX_SOCKADDRLEN       ಪರಿಮಾಣ(ngx_sockaddr_t)


ಪ್ರಕಾರನಿರ್ದೇಶ ಸಙ್ಘ {
    ಸಂರಚನಾ sockaddr           sockaddr;
    ಸಂರಚನಾ sockaddr_in        sockaddr_in;
#ಯದಿ (NGX_HAVE_INET6)
    ಸಂರಚನಾ sockaddr_in6       sockaddr_in6;
#endif
#ಯದಿ (NGX_HAVE_UNIX_DOMAIN)
    ಸಂರಚನಾ sockaddr_un        sockaddr_un;
#endif
} ngx_sockaddr_t;


ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    in_addr_t                 addr;
    in_addr_t                 mask;
} ngx_in_cidr_t;


#ಯದಿ (NGX_HAVE_INET6)

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    ಸಂರಚನಾ in6_addr           addr;
    ಸಂರಚನಾ in6_addr           mask;
} ngx_in6_cidr_t;

#endif


ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    ngx_uint_t                family;
    ಸಙ್ಘ {
        ngx_in_cidr_t         in;
#ಯದಿ (NGX_HAVE_INET6)
        ngx_in6_cidr_t        in6;
#endif
    } u;
} ngx_cidr_t;


ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    ಸಂರಚನಾ sockaddr          *sockaddr;
    socklen_t                 socklen;
    ngx_str_t                 name;
} ngx_addr_t;


ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    ngx_str_t                 url;
    ngx_str_t                 host;
    ngx_str_t                 port_text;
    ngx_str_t                 uri;

    in_port_t                 port;
    in_port_t                 default_port;
    in_port_t                 last_port;
    ಪೂರ್ಣಾಙ್ಕ                       family;

    unsigned                  listen:1;
    unsigned                  uri_part:1;
    unsigned                  no_resolve:1;

    unsigned                  no_port:1;
    unsigned                  wildcard:1;

    socklen_t                 socklen;
    ngx_sockaddr_t            sockaddr;

    ngx_addr_t               *addrs;
    ngx_uint_t                naddrs;

    ವರ್ಣ                     *err;
} ngx_url_t;


in_addr_t ngx_inet_addr(u_char *text, size_t len);
#ಯದಿ (NGX_HAVE_INET6)
ngx_int_t ngx_inet6_addr(u_char *p, size_t len, u_char *addr);
size_t ngx_inet6_ntop(u_char *p, u_char *text, size_t len);
#endif
size_t ngx_sock_ntop(ಸಂರಚನಾ sockaddr *sa, socklen_t socklen, u_char *text,
    size_t len, ngx_uint_t port);
size_t ngx_inet_ntop(ಪೂರ್ಣಾಙ್ಕ family, ಶೂನ್ಯ *addr, u_char *text, size_t len);
ngx_int_t ngx_ptocidr(ngx_str_t *text, ngx_cidr_t *cidr);
ngx_int_t ngx_cidr_match(ಸಂರಚನಾ sockaddr *sa, ngx_array_t *cidrs);
ngx_int_t ngx_parse_addr(ngx_pool_t *pool, ngx_addr_t *addr, u_char *text,
    size_t len);
ngx_int_t ngx_parse_addr_port(ngx_pool_t *pool, ngx_addr_t *addr,
    u_char *text, size_t len);
ngx_int_t ngx_parse_url(ngx_pool_t *pool, ngx_url_t *u);
ngx_int_t ngx_inet_resolve_host(ngx_pool_t *pool, ngx_url_t *u);
ngx_int_t ngx_cmp_sockaddr(ಸಂರಚನಾ sockaddr *sa1, socklen_t slen1,
    ಸಂರಚನಾ sockaddr *sa2, socklen_t slen2, ngx_uint_t cmp_port);
in_port_t ngx_inet_get_port(ಸಂರಚನಾ sockaddr *sa);
ಶೂನ್ಯ ngx_inet_set_port(ಸಂರಚನಾ sockaddr *sa, in_port_t port);
ngx_uint_t ngx_inet_wildcard(ಸಂರಚನಾ sockaddr *sa);


#endif /* _NGX_INET_H_INCLUDED_ */
