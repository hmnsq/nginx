
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_INET_H_INCLUDED_
#define _NGX_INET_H_INCLUDED_


#அந்தர்பாவ <ngx_config.h>
#அந்தர்பாவ <ngx_core.h>


#define NGX_INET_ADDRSTRLEN   (அளவு("255.255.255.255") - 1)
#define NGX_INET6_ADDRSTRLEN                                                 \
    (அளவு("ffff:ffff:ffff:ffff:ffff:ffff:255.255.255.255") - 1)
#define NGX_UNIX_ADDRSTRLEN                                                  \
    (அளவு("unix:") - 1 +                                                   \
     அளவு(கட்டமைப்பு sockaddr_un) - offsetof(கட்டமைப்பு sockaddr_un, sun_path))

#எனில் (NGX_HAVE_UNIX_DOMAIN)
#define NGX_SOCKADDR_STRLEN   NGX_UNIX_ADDRSTRLEN
#அல்லதெனில் (NGX_HAVE_INET6)
#define NGX_SOCKADDR_STRLEN   (NGX_INET6_ADDRSTRLEN + அளவு("[]:65535") - 1)
#இல்லையெனில்
#define NGX_SOCKADDR_STRLEN   (NGX_INET_ADDRSTRLEN + அளவு(":65535") - 1)
#endif

/* compatibility */
#define NGX_SOCKADDRLEN       அளவு(ngx_sockaddr_t)


வகைவரை ஒன்றிப்பு {
    கட்டமைப்பு sockaddr           sockaddr;
    கட்டமைப்பு sockaddr_in        sockaddr_in;
#எனில் (NGX_HAVE_INET6)
    கட்டமைப்பு sockaddr_in6       sockaddr_in6;
#endif
#எனில் (NGX_HAVE_UNIX_DOMAIN)
    கட்டமைப்பு sockaddr_un        sockaddr_un;
#endif
} ngx_sockaddr_t;


வகைவரை கட்டமைப்பு {
    in_addr_t                 addr;
    in_addr_t                 mask;
} ngx_in_cidr_t;


#எனில் (NGX_HAVE_INET6)

வகைவரை கட்டமைப்பு {
    கட்டமைப்பு in6_addr           addr;
    கட்டமைப்பு in6_addr           mask;
} ngx_in6_cidr_t;

#endif


வகைவரை கட்டமைப்பு {
    ngx_uint_t                family;
    ஒன்றிப்பு {
        ngx_in_cidr_t         in;
#எனில் (NGX_HAVE_INET6)
        ngx_in6_cidr_t        in6;
#endif
    } u;
} ngx_cidr_t;


வகைவரை கட்டமைப்பு {
    கட்டமைப்பு sockaddr          *sockaddr;
    socklen_t                 socklen;
    ngx_str_t                 name;
} ngx_addr_t;


வகைவரை கட்டமைப்பு {
    ngx_str_t                 url;
    ngx_str_t                 host;
    ngx_str_t                 port_text;
    ngx_str_t                 uri;

    in_port_t                 port;
    in_port_t                 default_port;
    in_port_t                 last_port;
    முழு                       family;

    unsigned                  listen:1;
    unsigned                  uri_part:1;
    unsigned                  no_resolve:1;

    unsigned                  no_port:1;
    unsigned                  wildcard:1;

    socklen_t                 socklen;
    ngx_sockaddr_t            sockaddr;

    ngx_addr_t               *addrs;
    ngx_uint_t                naddrs;

    எழுத்து                     *err;
} ngx_url_t;


in_addr_t ngx_inet_addr(u_char *text, size_t len);
#எனில் (NGX_HAVE_INET6)
ngx_int_t ngx_inet6_addr(u_char *p, size_t len, u_char *addr);
size_t ngx_inet6_ntop(u_char *p, u_char *text, size_t len);
#endif
size_t ngx_sock_ntop(கட்டமைப்பு sockaddr *sa, socklen_t socklen, u_char *text,
    size_t len, ngx_uint_t port);
size_t ngx_inet_ntop(முழு family, வெறுமை *addr, u_char *text, size_t len);
ngx_int_t ngx_ptocidr(ngx_str_t *text, ngx_cidr_t *cidr);
ngx_int_t ngx_cidr_match(கட்டமைப்பு sockaddr *sa, ngx_array_t *cidrs);
ngx_int_t ngx_parse_addr(ngx_pool_t *pool, ngx_addr_t *addr, u_char *text,
    size_t len);
ngx_int_t ngx_parse_addr_port(ngx_pool_t *pool, ngx_addr_t *addr,
    u_char *text, size_t len);
ngx_int_t ngx_parse_url(ngx_pool_t *pool, ngx_url_t *u);
ngx_int_t ngx_inet_resolve_host(ngx_pool_t *pool, ngx_url_t *u);
ngx_int_t ngx_cmp_sockaddr(கட்டமைப்பு sockaddr *sa1, socklen_t slen1,
    கட்டமைப்பு sockaddr *sa2, socklen_t slen2, ngx_uint_t cmp_port);
in_port_t ngx_inet_get_port(கட்டமைப்பு sockaddr *sa);
வெறுமை ngx_inet_set_port(கட்டமைப்பு sockaddr *sa, in_port_t port);
ngx_uint_t ngx_inet_wildcard(கட்டமைப்பு sockaddr *sa);


#endif /* _NGX_INET_H_INCLUDED_ */
