
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ _NGX_INET_H_INCLUDED_
#ವ್ಯಾಖ್ಯಾನಿಸು _NGX_INET_H_INCLUDED_


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


#ವ್ಯಾಖ್ಯಾನಿಸು NGX_INET_ADDRSTRLEN   (ಗಾತ್ರ("255.255.255.255") - 1)
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_INET6_ADDRSTRLEN                                                 \
    (ಗಾತ್ರ("ffff:ffff:ffff:ffff:ffff:ffff:255.255.255.255") - 1)
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_UNIX_ADDRSTRLEN                                                  \
    (ಗಾತ್ರ("unix:") - 1 +                                                   \
     ಗಾತ್ರ(ರಚನೆ sockaddr_un) - offsetof(ರಚನೆ sockaddr_un, sun_ಮಾರ್ಗ))

#ಆದರೆ (NGX_HAVE_UNIX_DOMAIN)
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_SOCKADDR_STRLEN   NGX_UNIX_ADDRSTRLEN
#ಅಥವಾಆದರೆ (NGX_HAVE_INET6)
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_SOCKADDR_STRLEN   (NGX_INET6_ADDRSTRLEN + ಗಾತ್ರ("[]:65535") - 1)
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_SOCKADDR_STRLEN   (NGX_INET_ADDRSTRLEN + ಗಾತ್ರ(":65535") - 1)
#ಆದರೆ_ಅಂತ್ಯ

/* compatibility */
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_SOCKADDRLEN       ಗಾತ್ರ(ngx_sockaddr_t)


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಒಕ್ಕೂಟ {
    ರಚನೆ sockaddr           sockaddr;
    ರಚನೆ sockaddr_ಒಳ        sockaddr_ಒಳ;
#ಆದರೆ (NGX_HAVE_INET6)
    ರಚನೆ sockaddr_in6       sockaddr_in6;
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ (NGX_HAVE_UNIX_DOMAIN)
    ರಚನೆ sockaddr_un        sockaddr_un;
#ಆದರೆ_ಅಂತ್ಯ
} ngx_sockaddr_t;


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ಒಳ_addr_t                 addr;
    ಒಳ_addr_t                 ಮುಖವಾಡ;
} ngx_ಒಳ_cidr_t;


#ಆದರೆ (NGX_HAVE_INET6)

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ರಚನೆ in6_addr           addr;
    ರಚನೆ in6_addr           ಮುಖವಾಡ;
} ngx_in6_cidr_t;

#ಆದರೆ_ಅಂತ್ಯ


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ngx_uint_t                family;
    ಒಕ್ಕೂಟ {
        ngx_ಒಳ_cidr_t         ಒಳ;
#ಆದರೆ (NGX_HAVE_INET6)
        ngx_in6_cidr_t        in6;
#ಆದರೆ_ಅಂತ್ಯ
    } u;
} ngx_cidr_t;


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ರಚನೆ sockaddr          *sockaddr;
    socklen_t                 socklen;
    ngx_str_t                 name;
} ngx_addr_t;


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ngx_str_t                 url;
    ngx_str_t                 host;
    ngx_str_t                 ಪೋರ್ಟ್_ಪಠ್ಯ;
    ngx_str_t                 uri;

    ಒಳ_ಪೋರ್ಟ್_t                 ಪೋರ್ಟ್;
    ಒಳ_ಪೋರ್ಟ್_t                 default_ಪೋರ್ಟ್;
    ಒಳ_ಪೋರ್ಟ್_t                 ಕೊನೆಯ_ಪೋರ್ಟ್;
    ಪೂರ್ಣಾಂಕ                       family;

    ಚಿಹ್ನೆರಹಿತ                  listen:1;
    ಚಿಹ್ನೆರಹಿತ                  uri_part:1;
    ಚಿಹ್ನೆರಹಿತ                  no_resolve:1;

    ಚಿಹ್ನೆರಹಿತ                  no_ಪೋರ್ಟ್:1;
    ಚಿಹ್ನೆರಹಿತ                  wildcard:1;

    socklen_t                 socklen;
    ngx_sockaddr_t            sockaddr;

    ngx_addr_t               *addrs;
    ngx_uint_t                naddrs;

    ಅಕ್ಷರ                     *err;
} ngx_url_t;


ಒಳ_addr_t ngx_inet_addr(u_char *ಪಠ್ಯ, ಗಾತ್ರ_t len);
#ಆದರೆ (NGX_HAVE_INET6)
ngx_int_t ngx_inet6_addr(u_char *p, ಗಾತ್ರ_t len, u_char *addr);
ಗಾತ್ರ_t ngx_inet6_ntop(u_char *p, u_char *ಪಠ್ಯ, ಗಾತ್ರ_t len);
#ಆದರೆ_ಅಂತ್ಯ
ಗಾತ್ರ_t ngx_sock_ntop(ರಚನೆ sockaddr *sa, socklen_t socklen, u_char *ಪಠ್ಯ,
    ಗಾತ್ರ_t len, ngx_uint_t ಪೋರ್ಟ್);
ಗಾತ್ರ_t ngx_inet_ntop(ಪೂರ್ಣಾಂಕ family, ಶೂನ್ಯ *addr, u_char *ಪಠ್ಯ, ಗಾತ್ರ_t len);
ngx_int_t ngx_ptocidr(ngx_str_t *ಪಠ್ಯ, ngx_cidr_t *cidr);
ngx_int_t ngx_cidr_ಹೊಂದಿಕೆ(ರಚನೆ sockaddr *sa, ngx_array_t *cidrs);
ngx_int_t ngx_parse_addr(ngx_pool_t *pool, ngx_addr_t *addr, u_char *ಪಠ್ಯ,
    ಗಾತ್ರ_t len);
ngx_int_t ngx_parse_addr_ಪೋರ್ಟ್(ngx_pool_t *pool, ngx_addr_t *addr,
    u_char *ಪಠ್ಯ, ಗಾತ್ರ_t len);
ngx_int_t ngx_parse_url(ngx_pool_t *pool, ngx_url_t *u);
ngx_int_t ngx_inet_resolve_host(ngx_pool_t *pool, ngx_url_t *u);
ngx_int_t ngx_cmp_sockaddr(ರಚನೆ sockaddr *sa1, socklen_t slen1,
    ರಚನೆ sockaddr *sa2, socklen_t slen2, ngx_uint_t cmp_ಪೋರ್ಟ್);
ಒಳ_ಪೋರ್ಟ್_t ngx_inet_get_ಪೋರ್ಟ್(ರಚನೆ sockaddr *sa);
ಶೂನ್ಯ ngx_inet_ಗಣ_ಪೋರ್ಟ್(ರಚನೆ sockaddr *sa, ಒಳ_ಪೋರ್ಟ್_t ಪೋರ್ಟ್);
ngx_uint_t ngx_inet_wildcard(ರಚನೆ sockaddr *sa);


#ಆದರೆ_ಅಂತ್ಯ /* _NGX_INET_H_INCLUDED_ */
