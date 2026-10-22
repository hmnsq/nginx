
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अगर_अघोषित _NGX_INET_H_INCLUDED_
#घोषणा _NGX_INET_H_INCLUDED_


#समावेश <ngx_config.h>
#समावेश <ngx_core.h>


#घोषणा NGX_INET_ADDRSTRLEN   (माप("255.255.255.255") - 1)
#घोषणा NGX_INET6_ADDRSTRLEN                                                 \
    (माप("ffff:ffff:ffff:ffff:ffff:ffff:255.255.255.255") - 1)
#घोषणा NGX_UNIX_ADDRSTRLEN                                                  \
    (माप("unix:") - 1 +                                                   \
     माप(काष्ठा sockaddr_un) - दुरत्व(काष्ठा sockaddr_un, sun_path))

#अगर (NGX_HAVE_UNIX_DOMAIN)
#घोषणा NGX_SOCKADDR_STRLEN   NGX_UNIX_ADDRSTRLEN
#या_अगर (NGX_HAVE_INET6)
#घोषणा NGX_SOCKADDR_STRLEN   (NGX_INET6_ADDRSTRLEN + माप("[]:65535") - 1)
#अन्यथा
#घोषणा NGX_SOCKADDR_STRLEN   (NGX_INET_ADDRSTRLEN + माप(":65535") - 1)
#पूर्ण_अगर

/* compatibility */
#घोषणा NGX_SOCKADDRLEN       माप(ngx_sockaddr_t)


प्रकार जोड़ {
    काष्ठा sockaddr           sockaddr;
    काष्ठा sockaddr_in        sockaddr_in;
#अगर (NGX_HAVE_INET6)
    काष्ठा sockaddr_in6       sockaddr_in6;
#पूर्ण_अगर
#अगर (NGX_HAVE_UNIX_DOMAIN)
    काष्ठा sockaddr_un        sockaddr_un;
#पूर्ण_अगर
} ngx_sockaddr_t;


प्रकार काष्ठा {
    in_addr_t                 addr;
    in_addr_t                 mask;
} ngx_in_cidr_t;


#अगर (NGX_HAVE_INET6)

प्रकार काष्ठा {
    काष्ठा in6_addr           addr;
    काष्ठा in6_addr           mask;
} ngx_in6_cidr_t;

#पूर्ण_अगर


प्रकार काष्ठा {
    ngx_uint_t                family;
    जोड़ {
        ngx_in_cidr_t         in;
#अगर (NGX_HAVE_INET6)
        ngx_in6_cidr_t        in6;
#पूर्ण_अगर
    } u;
} ngx_cidr_t;


प्रकार काष्ठा {
    काष्ठा sockaddr          *sockaddr;
    socklen_t                 socklen;
    ngx_str_t                 name;
} ngx_addr_t;


प्रकार काष्ठा {
    ngx_str_t                 url;
    ngx_str_t                 host;
    ngx_str_t                 port_text;
    ngx_str_t                 uri;

    in_port_t                 port;
    in_port_t                 default_port;
    in_port_t                 last_port;
    पूर्णांक                       family;

    अचिन्हित                  listen:1;
    अचिन्हित                  uri_part:1;
    अचिन्हित                  no_resolve:1;

    अचिन्हित                  no_port:1;
    अचिन्हित                  wildcard:1;

    socklen_t                 socklen;
    ngx_sockaddr_t            sockaddr;

    ngx_addr_t               *addrs;
    ngx_uint_t                naddrs;

    अक्षर                     *err;
} ngx_url_t;


in_addr_t ngx_inet_addr(u_char *text, माप_प्रकार len);
#अगर (NGX_HAVE_INET6)
ngx_int_t ngx_inet6_addr(u_char *p, माप_प्रकार len, u_char *addr);
माप_प्रकार ngx_inet6_ntop(u_char *p, u_char *text, माप_प्रकार len);
#पूर्ण_अगर
माप_प्रकार ngx_sock_ntop(काष्ठा sockaddr *sa, socklen_t socklen, u_char *text,
    माप_प्रकार len, ngx_uint_t port);
माप_प्रकार ngx_inet_ntop(पूर्णांक family, व्योम *addr, u_char *text, माप_प्रकार len);
ngx_int_t ngx_ptocidr(ngx_str_t *text, ngx_cidr_t *cidr);
ngx_int_t ngx_cidr_match(काष्ठा sockaddr *sa, ngx_array_t *cidrs);
ngx_int_t ngx_parse_addr(ngx_pool_t *pool, ngx_addr_t *addr, u_char *text,
    माप_प्रकार len);
ngx_int_t ngx_parse_addr_port(ngx_pool_t *pool, ngx_addr_t *addr,
    u_char *text, माप_प्रकार len);
ngx_int_t ngx_parse_url(ngx_pool_t *pool, ngx_url_t *u);
ngx_int_t ngx_inet_resolve_host(ngx_pool_t *pool, ngx_url_t *u);
ngx_int_t ngx_cmp_sockaddr(काष्ठा sockaddr *sa1, socklen_t slen1,
    काष्ठा sockaddr *sa2, socklen_t slen2, ngx_uint_t cmp_port);
in_port_t ngx_inet_get_port(काष्ठा sockaddr *sa);
व्योम ngx_inet_set_port(काष्ठा sockaddr *sa, in_port_t port);
ngx_uint_t ngx_inet_wildcard(काष्ठा sockaddr *sa);


#पूर्ण_अगर /* _NGX_INET_H_INCLUDED_ */
