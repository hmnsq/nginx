
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_INET_H_INCLUDED_
#define _NGX_INET_H_INCLUDED_


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


#define NGX_INET_ADDRSTRLEN   (परिमाण("255.255.255.255") - 1)
#define NGX_INET6_ADDRSTRLEN                                                 \
    (परिमाण("ffff:ffff:ffff:ffff:ffff:ffff:255.255.255.255") - 1)
#define NGX_UNIX_ADDRSTRLEN                                                  \
    (परिमाण("unix:") - 1 +                                                   \
     परिमाण(संरचना sockaddr_un) - दुरत्व(संरचना sockaddr_un, sun_path))

#यदि (NGX_HAVE_UNIX_DOMAIN)
#define NGX_SOCKADDR_STRLEN   NGX_UNIX_ADDRSTRLEN
#अथवायदि (NGX_HAVE_INET6)
#define NGX_SOCKADDR_STRLEN   (NGX_INET6_ADDRSTRLEN + परिमाण("[]:65535") - 1)
#अन्यथा
#define NGX_SOCKADDR_STRLEN   (NGX_INET_ADDRSTRLEN + परिमाण(":65535") - 1)
#endif

/* compatibility */
#define NGX_SOCKADDRLEN       परिमाण(ngx_sockaddr_t)


प्रकारनिर्देश सङ्घ {
    संरचना sockaddr           sockaddr;
    संरचना sockaddr_in        sockaddr_in;
#यदि (NGX_HAVE_INET6)
    संरचना sockaddr_in6       sockaddr_in6;
#endif
#यदि (NGX_HAVE_UNIX_DOMAIN)
    संरचना sockaddr_un        sockaddr_un;
#endif
} ngx_sockaddr_t;


प्रकारनिर्देश संरचना {
    in_addr_t                 addr;
    in_addr_t                 mask;
} ngx_in_cidr_t;


#यदि (NGX_HAVE_INET6)

प्रकारनिर्देश संरचना {
    संरचना in6_addr           addr;
    संरचना in6_addr           mask;
} ngx_in6_cidr_t;

#endif


प्रकारनिर्देश संरचना {
    ngx_uint_t                family;
    सङ्घ {
        ngx_in_cidr_t         in;
#यदि (NGX_HAVE_INET6)
        ngx_in6_cidr_t        in6;
#endif
    } u;
} ngx_cidr_t;


प्रकारनिर्देश संरचना {
    संरचना sockaddr          *sockaddr;
    socklen_t                 socklen;
    ngx_str_t                 name;
} ngx_addr_t;


प्रकारनिर्देश संरचना {
    ngx_str_t                 url;
    ngx_str_t                 host;
    ngx_str_t                 port_text;
    ngx_str_t                 uri;

    in_port_t                 port;
    in_port_t                 default_port;
    in_port_t                 last_port;
    पूर्णाङ्क                       family;

    अचिन्हित                  listen:1;
    अचिन्हित                  uri_part:1;
    अचिन्हित                  no_resolve:1;

    अचिन्हित                  no_port:1;
    अचिन्हित                  wildcard:1;

    socklen_t                 socklen;
    ngx_sockaddr_t            sockaddr;

    ngx_addr_t               *addrs;
    ngx_uint_t                naddrs;

    वर्ण                     *err;
} ngx_url_t;


in_addr_t ngx_inet_addr(u_char *text, माप_प्रकार len);
#यदि (NGX_HAVE_INET6)
ngx_int_t ngx_inet6_addr(u_char *p, माप_प्रकार len, u_char *addr);
माप_प्रकार ngx_inet6_ntop(u_char *p, u_char *text, माप_प्रकार len);
#endif
माप_प्रकार ngx_sock_ntop(संरचना sockaddr *sa, socklen_t socklen, u_char *text,
    माप_प्रकार len, ngx_uint_t port);
माप_प्रकार ngx_inet_ntop(पूर्णाङ्क family, शून्य *addr, u_char *text, माप_प्रकार len);
ngx_int_t ngx_ptocidr(ngx_str_t *text, ngx_cidr_t *cidr);
ngx_int_t ngx_cidr_match(संरचना sockaddr *sa, ngx_array_t *cidrs);
ngx_int_t ngx_parse_addr(ngx_pool_t *pool, ngx_addr_t *addr, u_char *text,
    माप_प्रकार len);
ngx_int_t ngx_parse_addr_port(ngx_pool_t *pool, ngx_addr_t *addr,
    u_char *text, माप_प्रकार len);
ngx_int_t ngx_parse_url(ngx_pool_t *pool, ngx_url_t *u);
ngx_int_t ngx_inet_resolve_host(ngx_pool_t *pool, ngx_url_t *u);
ngx_int_t ngx_cmp_sockaddr(संरचना sockaddr *sa1, socklen_t slen1,
    संरचना sockaddr *sa2, socklen_t slen2, ngx_uint_t cmp_port);
in_port_t ngx_inet_get_port(संरचना sockaddr *sa);
शून्य ngx_inet_set_port(संरचना sockaddr *sa, in_port_t port);
ngx_uint_t ngx_inet_wildcard(संरचना sockaddr *sa);


#endif /* _NGX_INET_H_INCLUDED_ */
