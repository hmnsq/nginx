
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ _NGX_INET_H_INCLUDED_
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _NGX_INET_H_INCLUDED_


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_INET_ADDRSTRLEN   (ਆਕਾਰ("255.255.255.255") - 1)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_INET6_ADDRSTRLEN                                                 \
    (ਆਕਾਰ("ffff:ffff:ffff:ffff:ffff:ffff:255.255.255.255") - 1)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_UNIX_ADDRSTRLEN                                                  \
    (ਆਕਾਰ("unix:") - 1 +                                                   \
     ਆਕਾਰ(ਬਣਤਰ sockaddr_un) - offsetof(ਬਣਤਰ sockaddr_un, sun_ਰਸਤਾ))

#ਜੇ (NGX_HAVE_UNIX_DOMAIN)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_SOCKADDR_STRLEN   NGX_UNIX_ADDRSTRLEN
#ਜਾਂਜੇ (NGX_HAVE_INET6)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_SOCKADDR_STRLEN   (NGX_INET6_ADDRSTRLEN + ਆਕਾਰ("[]:65535") - 1)
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_SOCKADDR_STRLEN   (NGX_INET_ADDRSTRLEN + ਆਕਾਰ(":65535") - 1)
#ਜੇ_ਅੰਤ

/* compatibility */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_SOCKADDRLEN       ਆਕਾਰ(ngx_sockaddr_t)


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਸੰਘ {
    ਬਣਤਰ sockaddr           sockaddr;
    ਬਣਤਰ sockaddr_ਵਿੱਚ        sockaddr_ਵਿੱਚ;
#ਜੇ (NGX_HAVE_INET6)
    ਬਣਤਰ sockaddr_in6       sockaddr_in6;
#ਜੇ_ਅੰਤ
#ਜੇ (NGX_HAVE_UNIX_DOMAIN)
    ਬਣਤਰ sockaddr_un        sockaddr_un;
#ਜੇ_ਅੰਤ
} ngx_sockaddr_t;


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ਵਿੱਚ_addr_t                 addr;
    ਵਿੱਚ_addr_t                 ਨਕਾਬ;
} ngx_ਵਿੱਚ_cidr_t;


#ਜੇ (NGX_HAVE_INET6)

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ਬਣਤਰ in6_addr           addr;
    ਬਣਤਰ in6_addr           ਨਕਾਬ;
} ngx_in6_cidr_t;

#ਜੇ_ਅੰਤ


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ngx_uint_t                family;
    ਸੰਘ {
        ngx_ਵਿੱਚ_cidr_t         ਵਿੱਚ;
#ਜੇ (NGX_HAVE_INET6)
        ngx_in6_cidr_t        in6;
#ਜੇ_ਅੰਤ
    } u;
} ngx_cidr_t;


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ਬਣਤਰ sockaddr          *sockaddr;
    socklen_t                 socklen;
    ngx_str_t                 name;
} ngx_addr_t;


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ngx_str_t                 url;
    ngx_str_t                 host;
    ngx_str_t                 ਪੋਰਟ_ਪਾਠ;
    ngx_str_t                 uri;

    ਵਿੱਚ_ਪੋਰਟ_t                 ਪੋਰਟ;
    ਵਿੱਚ_ਪੋਰਟ_t                 default_ਪੋਰਟ;
    ਵਿੱਚ_ਪੋਰਟ_t                 ਆਖਰੀ_ਪੋਰਟ;
    ਪੂਰਨ                       family;

    ਬਿਨਾਨਿਸ਼ਾਨੀ                  listen:1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ                  uri_part:1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ                  no_resolve:1;

    ਬਿਨਾਨਿਸ਼ਾਨੀ                  no_ਪੋਰਟ:1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ                  wildcard:1;

    socklen_t                 socklen;
    ngx_sockaddr_t            sockaddr;

    ngx_addr_t               *addrs;
    ngx_uint_t                naddrs;

    ਅੱਖਰ                     *err;
} ngx_url_t;


ਵਿੱਚ_addr_t ngx_inet_addr(u_char *ਪਾਠ, ਮਾਪ_t len);
#ਜੇ (NGX_HAVE_INET6)
ngx_int_t ngx_inet6_addr(u_char *p, ਮਾਪ_t len, u_char *addr);
ਮਾਪ_t ngx_inet6_ntop(u_char *p, u_char *ਪਾਠ, ਮਾਪ_t len);
#ਜੇ_ਅੰਤ
ਮਾਪ_t ngx_sock_ntop(ਬਣਤਰ sockaddr *sa, socklen_t socklen, u_char *ਪਾਠ,
    ਮਾਪ_t len, ngx_uint_t ਪੋਰਟ);
ਮਾਪ_t ngx_inet_ntop(ਪੂਰਨ family, ਖਾਲੀ *addr, u_char *ਪਾਠ, ਮਾਪ_t len);
ngx_int_t ngx_ptocidr(ngx_str_t *ਪਾਠ, ngx_cidr_t *cidr);
ngx_int_t ngx_cidr_ਮੇਲ(ਬਣਤਰ sockaddr *sa, ngx_array_t *cidrs);
ngx_int_t ngx_parse_addr(ngx_pool_t *pool, ngx_addr_t *addr, u_char *ਪਾਠ,
    ਮਾਪ_t len);
ngx_int_t ngx_parse_addr_ਪੋਰਟ(ngx_pool_t *pool, ngx_addr_t *addr,
    u_char *ਪਾਠ, ਮਾਪ_t len);
ngx_int_t ngx_parse_url(ngx_pool_t *pool, ngx_url_t *u);
ngx_int_t ngx_inet_resolve_host(ngx_pool_t *pool, ngx_url_t *u);
ngx_int_t ngx_cmp_sockaddr(ਬਣਤਰ sockaddr *sa1, socklen_t slen1,
    ਬਣਤਰ sockaddr *sa2, socklen_t slen2, ngx_uint_t cmp_ਪੋਰਟ);
ਵਿੱਚ_ਪੋਰਟ_t ngx_inet_get_ਪੋਰਟ(ਬਣਤਰ sockaddr *sa);
ਖਾਲੀ ngx_inet_ਸੈੱਟ_ਪੋਰਟ(ਬਣਤਰ sockaddr *sa, ਵਿੱਚ_ਪੋਰਟ_t ਪੋਰਟ);
ngx_uint_t ngx_inet_wildcard(ਬਣਤਰ sockaddr *sa);


#ਜੇ_ਅੰਤ /* _NGX_INET_H_INCLUDED_ */
