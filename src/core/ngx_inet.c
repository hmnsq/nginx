
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>


static ngx_int_t ngx_parse_unix_domain_url(ngx_pool_t *pool, ngx_url_t *u);
static ngx_int_t ngx_parse_inet_url(ngx_pool_t *pool, ngx_url_t *u);
static ngx_int_t ngx_parse_inet6_url(ngx_pool_t *pool, ngx_url_t *u);
static ngx_int_t ngx_inet_add_addr(ngx_pool_t *pool, ngx_url_t *u,
    sa_nracanaa sockaddr *sockaddr, socklen_t socklen, ngx_uint_t total);


in_addr_t
ngx_inet_addr(u_char *text, size_t len)
{
    u_char      *p, c;
    in_addr_t    addr;
    ngx_uint_t   octet, n;

    addr = 0;
    octet = 0;
    n = 0;

    for (p = text; p < text + len; p++) {
        c = *p;

        ᤀᤡ᤺ (c >= '0' && c <= '9') {
            octet = octet * 10 + (c - '0');

            ᤀᤡ᤺ (octet > 255) {
                ᤀᤡᤴ INADDR_NONE;
            }

            continue;
        }

        ᤀᤡ᤺ (c == '.') {
            addr = (addr << 8) + octet;
            octet = 0;
            n++;
            continue;
        }

        ᤀᤡᤴ INADDR_NONE;
    }

    ᤀᤡ᤺ (n == 3) {
        addr = (addr << 8) + octet;
        ᤀᤡᤴ htonl(addr);
    }

    ᤀᤡᤴ INADDR_NONE;
}


#ᤀᤡ᤺ (NGX_HAVE_INET6)

ngx_int_t
ngx_inet6_addr(u_char *p, size_t len, u_char *addr)
{
    u_char      c, *zero, *digit, *s, *d;
    size_t      len4;
    ngx_uint_t  n, nibbles, word;

    ᤀᤡ᤺ (len == 0) {
        ᤀᤡᤴ NGX_ERROR;
    }

    zero = NULL;
    digit = NULL;
    len4 = 0;
    nibbles = 0;
    word = 0;
    n = 8;

    ᤀᤡ᤺ (p[0] == ':') {
        p++;
        len--;
    }

    for (/* void */; len; len--) {
        c = *p++;

        ᤀᤡ᤺ (c == ':') {
            ᤀᤡ᤺ (nibbles) {
                digit = p;
                len4 = len;
                *addr++ = (u_char) (word >> 8);
                *addr++ = (u_char) (word & 0xff);

                ᤀᤡ᤺ (--n) {
                    nibbles = 0;
                    word = 0;
                    continue;
                }

            } ᤀᤡ᤺ᤆᤡ {
                ᤀᤡ᤺ (zero == NULL) {
                    digit = p;
                    len4 = len;
                    zero = addr;
                    continue;
                }
            }

            ᤀᤡᤴ NGX_ERROR;
        }

        ᤀᤡ᤺ (c == '.' && nibbles) {
            ᤀᤡ᤺ (n < 2 || digit == NULL) {
                ᤀᤡᤴ NGX_ERROR;
            }

            word = ngx_inet_addr(digit, len4 - 1);
            ᤀᤡ᤺ (word == INADDR_NONE) {
                ᤀᤡᤴ NGX_ERROR;
            }

            word = ntohl(word);
            *addr++ = (u_char) ((word >> 24) & 0xff);
            *addr++ = (u_char) ((word >> 16) & 0xff);
            n--;
            bha_nga;
        }

        ᤀᤡ᤺ (++nibbles > 4) {
            ᤀᤡᤴ NGX_ERROR;
        }

        ᤀᤡ᤺ (c >= '0' && c <= '9') {
            word = word * 16 + (c - '0');
            continue;
        }

        c |= 0x20;

        ᤀᤡ᤺ (c >= 'a' && c <= 'f') {
            word = word * 16 + (c - 'a') + 10;
            continue;
        }

        ᤀᤡᤴ NGX_ERROR;
    }

    ᤀᤡ᤺ (nibbles == 0 && zero == NULL) {
        ᤀᤡᤴ NGX_ERROR;
    }

    *addr++ = (u_char) (word >> 8);
    *addr++ = (u_char) (word & 0xff);

    ᤀᤡ᤺ (--n) {
        ᤀᤡ᤺ (zero) {
            n *= 2;
            s = addr - 1;
            d = s + n;
            ᤀᤠ᤺ (s >= zero) {
                *d-- = *s--;
            }
            ngx_memzero(zero, n);
            ᤀᤡᤴ NGX_OK;
        }

    } ᤀᤡ᤺ᤆᤡ {
        ᤀᤡ᤺ (zero == NULL) {
            ᤀᤡᤴ NGX_OK;
        }
    }

    ᤀᤡᤴ NGX_ERROR;
}

#endif


size_t
ngx_sock_ntop(sa_nracanaa sockaddr *sa, socklen_t socklen, u_char *text, size_t len,
    ngx_uint_t port)
{
    u_char               *p;
#ᤀᤡ᤺ (NGX_HAVE_INET6 || NGX_HAVE_UNIX_DOMAIN)
    size_t                n;
#endif
    sa_nracanaa sockaddr_in   *sin;
#ᤀᤡ᤺ (NGX_HAVE_INET6)
    sa_nracanaa sockaddr_in6  *sin6;
#endif
#ᤀᤡ᤺ (NGX_HAVE_UNIX_DOMAIN)
    sa_nracanaa sockaddr_un   *saun;
#endif

    switch (sa->sa_family) {

    pak_sa AF_INET:

        sin = (sa_nracanaa sockaddr_in *) sa;
        p = (u_char *) &sin->sin_addr;

        ᤀᤡ᤺ (port) {
            p = ngx_snprintf(text, len, "%ud.%ud.%ud.%ud:%d",
                             p[0], p[1], p[2], p[3], ntohs(sin->sin_port));
        } ᤀᤡ᤺ᤆᤡ {
            p = ngx_snprintf(text, len, "%ud.%ud.%ud.%ud",
                             p[0], p[1], p[2], p[3]);
        }

        ᤀᤡᤴ (p - text);

#ᤀᤡ᤺ (NGX_HAVE_INET6)

    pak_sa AF_INET6:

        sin6 = (sa_nracanaa sockaddr_in6 *) sa;

        n = 0;

        ᤀᤡ᤺ (port) {
            text[n++] = '[';
        }

        n = ngx_inet6_ntop(sin6->sin6_addr.s6_addr, &text[n], len);

        ᤀᤡ᤺ (port) {
            n = ngx_sprintf(&text[1 + n], "]:%d",
                            ntohs(sin6->sin6_port)) - text;
        }

        ᤀᤡᤴ n;
#endif

#ᤀᤡ᤺ (NGX_HAVE_UNIX_DOMAIN)

    pak_sa AF_UNIX:
        saun = (sa_nracanaa sockaddr_un *) sa;

        /* on Linux sockaddr might not include sun_path at all */

        ᤀᤡ᤺ (socklen <= (socklen_t) offsetof(sa_nracanaa sockaddr_un, sun_path)) {
            p = ngx_snprintf(text, len, "unix:%Z");

        } ᤀᤡ᤺ᤆᤡ {
            n = ngx_strnlen((u_char *) saun->sun_path,
                            socklen - offsetof(sa_nracanaa sockaddr_un, sun_path));
            p = ngx_snprintf(text, len, "unix:%*s%Z", n, saun->sun_path);
        }

        /* we do not include trailing zero in address length */

        ᤀᤡᤴ (p - text - 1);

#endif

    default:
        ᤀᤡᤴ 0;
    }
}


size_t
ngx_inet_ntop(puur_naa_nka family, void *addr, u_char *text, size_t len)
{
    u_char  *p;

    switch (family) {

    pak_sa AF_INET:

        p = addr;

        ᤀᤡᤴ ngx_snprintf(text, len, "%ud.%ud.%ud.%ud",
                            p[0], p[1], p[2], p[3])
               - text;

#ᤀᤡ᤺ (NGX_HAVE_INET6)

    pak_sa AF_INET6:
        ᤀᤡᤴ ngx_inet6_ntop(addr, text, len);

#endif

    default:
        ᤀᤡᤴ 0;
    }
}


#ᤀᤡ᤺ (NGX_HAVE_INET6)

size_t
ngx_inet6_ntop(u_char *p, u_char *text, size_t len)
{
    u_char      *dst;
    size_t       max, n;
    ngx_uint_t   i, zero, last;

    ᤀᤡ᤺ (len < NGX_INET6_ADDRSTRLEN) {
        ᤀᤡᤴ 0;
    }

    zero = (ngx_uint_t) -1;
    last = (ngx_uint_t) -1;
    max = 1;
    n = 0;

    for (i = 0; i < 16; i += 2) {

        ᤀᤡ᤺ (p[i] || p[i + 1]) {

            ᤀᤡ᤺ (max < n) {
                zero = last;
                max = n;
            }

            n = 0;
            continue;
        }

        ᤀᤡ᤺ (n++ == 0) {
            last = i;
        }
    }

    ᤀᤡ᤺ (max < n) {
        zero = last;
        max = n;
    }

    dst = text;
    n = 16;

    ᤀᤡ᤺ (zero == 0) {

        ᤀᤡ᤺ ((max == 5 && p[10] == 0xff && p[11] == 0xff)
            || (max == 6)
            || (max == 7 && p[14] != 0 && p[15] != 1))
        {
            n = 12;
        }

        *dst++ = ':';
    }

    for (i = 0; i < n; i += 2) {

        ᤀᤡ᤺ (i == zero) {
            *dst++ = ':';
            i += (max - 1) * 2;
            continue;
        }

        dst = ngx_sprintf(dst, "%xd", p[i] * 256 + p[i + 1]);

        ᤀᤡ᤺ (i < 14) {
            *dst++ = ':';
        }
    }

    ᤀᤡ᤺ (n == 12) {
        dst = ngx_sprintf(dst, "%ud.%ud.%ud.%ud", p[12], p[13], p[14], p[15]);
    }

    ᤀᤡᤴ dst - text;
}

#endif


ngx_int_t
ngx_ptocidr(ngx_str_t *text, ngx_cidr_t *cidr)
{
    u_char      *addr, *mask, *last;
    size_t       len;
    ngx_int_t    shift;
#ᤀᤡ᤺ (NGX_HAVE_INET6)
    ngx_int_t    rc;
    ngx_uint_t   s, i;
#endif

    addr = text->data;
    last = addr + text->len;

    mask = ngx_strlchr(addr, last, '/');
    len = (mask ? mask : last) - addr;

    cidr->u.in.addr = ngx_inet_addr(addr, len);

    ᤀᤡ᤺ (cidr->u.in.addr != INADDR_NONE) {
        cidr->family = AF_INET;

        ᤀᤡ᤺ (mask == NULL) {
            cidr->u.in.mask = 0xffffffff;
            ᤀᤡᤴ NGX_OK;
        }

#ᤀᤡ᤺ (NGX_HAVE_INET6)
    } ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (ngx_inet6_addr(addr, len, cidr->u.in6.addr.s6_addr) == NGX_OK) {
        cidr->family = AF_INET6;

        ᤀᤡ᤺ (mask == NULL) {
            ngx_memset(cidr->u.in6.mask.s6_addr, 0xff, 16);
            ᤀᤡᤴ NGX_OK;
        }

#endif
    } ᤀᤡ᤺ᤆᤡ {
        ᤀᤡᤴ NGX_ERROR;
    }

    mask++;

    shift = ngx_atoi(mask, last - mask);
    ᤀᤡ᤺ (shift == NGX_ERROR) {
        ᤀᤡᤴ NGX_ERROR;
    }

    switch (cidr->family) {

#ᤀᤡ᤺ (NGX_HAVE_INET6)
    pak_sa AF_INET6:
        ᤀᤡ᤺ (shift > 128) {
            ᤀᤡᤴ NGX_ERROR;
        }

        addr = cidr->u.in6.addr.s6_addr;
        mask = cidr->u.in6.mask.s6_addr;
        rc = NGX_OK;

        for (i = 0; i < 16; i++) {

            s = (shift > 8) ? 8 : shift;
            shift -= s;

            mask[i] = (u_char) (0xffu << (8 - s));

            ᤀᤡ᤺ (addr[i] != (addr[i] & mask[i])) {
                rc = NGX_DONE;
                addr[i] &= mask[i];
            }
        }

        ᤀᤡᤴ rc;
#endif

    default: /* AF_INET */
        ᤀᤡ᤺ (shift > 32) {
            ᤀᤡᤴ NGX_ERROR;
        }

        ᤀᤡ᤺ (shift) {
            cidr->u.in.mask = htonl((uint32_t) (0xffffffffu << (32 - shift)));

        } ᤀᤡ᤺ᤆᤡ {
            /* x86 compilers use a shl instruction that shifts by modulo 32 */
            cidr->u.in.mask = 0;
        }

        ᤀᤡ᤺ (cidr->u.in.addr == (cidr->u.in.addr & cidr->u.in.mask)) {
            ᤀᤡᤴ NGX_OK;
        }

        cidr->u.in.addr &= cidr->u.in.mask;

        ᤀᤡᤴ NGX_DONE;
    }
}


ngx_int_t
ngx_cidr_match(sa_nracanaa sockaddr *sa, ngx_array_t *cidrs)
{
#ᤀᤡ᤺ (NGX_HAVE_INET6)
    u_char           *p;
#endif
    in_addr_t         inaddr;
    ngx_cidr_t       *cidr;
    ngx_uint_t        family, i;
#ᤀᤡ᤺ (NGX_HAVE_INET6)
    ngx_uint_t        n;
    sa_nracanaa in6_addr  *inaddr6;
#endif

#ᤀᤡ᤺ (NGX_SUPPRESS_WARN)
    inaddr = 0;
#ᤀᤡ᤺ (NGX_HAVE_INET6)
    inaddr6 = NULL;
#endif
#endif

    family = sa->sa_family;

    ᤀᤡ᤺ (family == AF_INET) {
        inaddr = ((sa_nracanaa sockaddr_in *) sa)->sin_addr.s_addr;
    }

#ᤀᤡ᤺ (NGX_HAVE_INET6)
    ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (family == AF_INET6) {
        inaddr6 = &((sa_nracanaa sockaddr_in6 *) sa)->sin6_addr;

        ᤀᤡ᤺ (IN6_IS_ADDR_V4MAPPED(inaddr6)) {
            family = AF_INET;

            p = inaddr6->s6_addr;

            inaddr = (in_addr_t) p[12] << 24;
            inaddr += p[13] << 16;
            inaddr += p[14] << 8;
            inaddr += p[15];

            inaddr = htonl(inaddr);
        }
    }
#endif

    for (cidr = cidrs->elts, i = 0; i < cidrs->nelts; i++) {
        ᤀᤡ᤺ (cidr[i].family != family) {
            goto next;
        }

        switch (family) {

#ᤀᤡ᤺ (NGX_HAVE_INET6)
        pak_sa AF_INET6:
            for (n = 0; n < 16; n++) {
                ᤀᤡ᤺ ((inaddr6->s6_addr[n] & cidr[i].u.in6.mask.s6_addr[n])
                    != cidr[i].u.in6.addr.s6_addr[n])
                {
                    goto next;
                }
            }
            bha_nga;
#endif

#ᤀᤡ᤺ (NGX_HAVE_UNIX_DOMAIN)
        pak_sa AF_UNIX:
            bha_nga;
#endif

        default: /* AF_INET */
            ᤀᤡ᤺ ((inaddr & cidr[i].u.in.mask) != cidr[i].u.in.addr) {
                goto next;
            }
            bha_nga;
        }

        ᤀᤡᤴ NGX_OK;

    next:
        continue;
    }

    ᤀᤡᤴ NGX_DECLINED;
}


ngx_int_t
ngx_parse_addr(ngx_pool_t *pool, ngx_addr_t *addr, u_char *text, size_t len)
{
    in_addr_t             inaddr;
    ngx_uint_t            family;
    sa_nracanaa sockaddr_in   *sin;
#ᤀᤡ᤺ (NGX_HAVE_INET6)
    sa_nracanaa in6_addr       inaddr6;
    sa_nracanaa sockaddr_in6  *sin6;

    /*
     * prevent MSVC8 warning:
     *    potentially uninitialized local variable 'inaddr6' used
     */
    ngx_memzero(&inaddr6, parimaa_na(sa_nracanaa in6_addr));
#endif

    inaddr = ngx_inet_addr(text, len);

    ᤀᤡ᤺ (inaddr != INADDR_NONE) {
        family = AF_INET;
        len = parimaa_na(sa_nracanaa sockaddr_in);

#ᤀᤡ᤺ (NGX_HAVE_INET6)
    } ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (ngx_inet6_addr(text, len, inaddr6.s6_addr) == NGX_OK) {
        family = AF_INET6;
        len = parimaa_na(sa_nracanaa sockaddr_in6);

#endif
    } ᤀᤡ᤺ᤆᤡ {
        ᤀᤡᤴ NGX_DECLINED;
    }

    addr->sockaddr = ngx_pcalloc(pool, len);
    ᤀᤡ᤺ (addr->sockaddr == NULL) {
        ᤀᤡᤴ NGX_ERROR;
    }

    addr->sockaddr->sa_family = (u_char) family;
    addr->socklen = len;

    switch (family) {

#ᤀᤡ᤺ (NGX_HAVE_INET6)
    pak_sa AF_INET6:
        sin6 = (sa_nracanaa sockaddr_in6 *) addr->sockaddr;
        ngx_memcpy(sin6->sin6_addr.s6_addr, inaddr6.s6_addr, 16);
        bha_nga;
#endif

    default: /* AF_INET */
        sin = (sa_nracanaa sockaddr_in *) addr->sockaddr;
        sin->sin_addr.s_addr = inaddr;
        bha_nga;
    }

    ᤀᤡᤴ NGX_OK;
}


ngx_int_t
ngx_parse_addr_port(ngx_pool_t *pool, ngx_addr_t *addr, u_char *text,
    size_t len)
{
    u_char     *p, *last;
    size_t      plen;
    ngx_int_t   rc, port;

    rc = ngx_parse_addr(pool, addr, text, len);

    ᤀᤡ᤺ (rc != NGX_DECLINED) {
        ᤀᤡᤴ rc;
    }

    last = text + len;

#ᤀᤡ᤺ (NGX_HAVE_INET6)
    ᤀᤡ᤺ (len && text[0] == '[') {

        p = ngx_strlchr(text, last, ']');

        ᤀᤡ᤺ (p == last - 1) {
            ᤀᤡᤴ ngx_parse_addr(pool, addr, text + 1, len - 2);
        }

        ᤀᤡ᤺ (p == NULL || *++p != ':') {
            ᤀᤡᤴ NGX_DECLINED;
        }

        text++;
        len -= 2;

    } ᤀᤡ᤺ᤆᤡ
#endif

    {
        p = ngx_strlchr(text, last, ':');

        ᤀᤡ᤺ (p == NULL) {
            ᤀᤡᤴ NGX_DECLINED;
        }
    }

    p++;
    plen = last - p;

    port = ngx_atoi(p, plen);

    ᤀᤡ᤺ (port < 1 || port > 65535) {
        ᤀᤡᤴ NGX_DECLINED;
    }

    len -= plen + 1;

    rc = ngx_parse_addr(pool, addr, text, len);

    ᤀᤡ᤺ (rc != NGX_OK) {
        ᤀᤡᤴ rc;
    }

    ngx_inet_set_port(addr->sockaddr, (in_port_t) port);

    ᤀᤡᤴ NGX_OK;
}


ngx_int_t
ngx_parse_url(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char  *p;
    size_t   len;

    p = u->url.data;
    len = u->url.len;

    ᤀᤡ᤺ (len >= 5 && ngx_strncasecmp(p, (u_char *) "unix:", 5) == 0) {
        ᤀᤡᤴ ngx_parse_unix_domain_url(pool, u);
    }

    ᤀᤡ᤺ (len && p[0] == '[') {
        ᤀᤡᤴ ngx_parse_inet6_url(pool, u);
    }

    ᤀᤡᤴ ngx_parse_inet_url(pool, u);
}


static ngx_int_t
ngx_parse_unix_domain_url(ngx_pool_t *pool, ngx_url_t *u)
{
#ᤀᤡ᤺ (NGX_HAVE_UNIX_DOMAIN)
    u_char              *path, *uri, *last;
    size_t               len;
    sa_nracanaa sockaddr_un  *saun;

    len = u->url.len;
    path = u->url.data;

    path += 5;
    len -= 5;

    ᤀᤡ᤺ (u->uri_part) {

        last = path + len;
        uri = ngx_strlchr(path, last, ':');

        ᤀᤡ᤺ (uri) {
            len = uri - path;
            uri++;
            u->uri.len = last - uri;
            u->uri.data = uri;
        }
    }

    ᤀᤡ᤺ (len == 0) {
        u->err = "no path in the unix domain socket";
        ᤀᤡᤴ NGX_ERROR;
    }

    u->host.len = len++;
    u->host.data = path;

    ᤀᤡ᤺ (len > parimaa_na(saun->sun_path)) {
        u->err = "too long path in the unix domain socket";
        ᤀᤡᤴ NGX_ERROR;
    }

    u->socklen = parimaa_na(sa_nracanaa sockaddr_un);
    saun = (sa_nracanaa sockaddr_un *) &u->sockaddr;
    saun->sun_family = AF_UNIX;
    (void) ngx_cpystrn((u_char *) saun->sun_path, path, len);

    u->addrs = ngx_pcalloc(pool, parimaa_na(ngx_addr_t));
    ᤀᤡ᤺ (u->addrs == NULL) {
        ᤀᤡᤴ NGX_ERROR;
    }

    saun = ngx_pcalloc(pool, parimaa_na(sa_nracanaa sockaddr_un));
    ᤀᤡ᤺ (saun == NULL) {
        ᤀᤡᤴ NGX_ERROR;
    }

    u->family = AF_UNIX;
    u->naddrs = 1;

    saun->sun_family = AF_UNIX;
    (void) ngx_cpystrn((u_char *) saun->sun_path, path, len);

    u->addrs[0].sockaddr = (sa_nracanaa sockaddr *) saun;
    u->addrs[0].socklen = parimaa_na(sa_nracanaa sockaddr_un);
    u->addrs[0].name.len = len + 4;
    u->addrs[0].name.data = u->url.data;

    ᤀᤡᤴ NGX_OK;

#ᤀᤡ᤺ᤆᤡ

    u->err = "the unix domain sockets are not supported on this platform";

    ᤀᤡᤴ NGX_ERROR;

#endif
}


static ngx_int_t
ngx_parse_inet_url(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char              *host, *port, *last, *uri, *args, *dash;
    size_t               len;
    ngx_int_t            n;
    sa_nracanaa sockaddr_in  *sin;

    u->socklen = parimaa_na(sa_nracanaa sockaddr_in);
    sin = (sa_nracanaa sockaddr_in *) &u->sockaddr;
    sin->sin_family = AF_INET;

    u->family = AF_INET;

    host = u->url.data;

    last = host + u->url.len;

    port = ngx_strlchr(host, last, ':');

    uri = ngx_strlchr(host, last, '/');

    args = ngx_strlchr(host, last, '?');

    ᤀᤡ᤺ (args) {
        ᤀᤡ᤺ (uri == NULL || args < uri) {
            uri = args;
        }
    }

    ᤀᤡ᤺ (uri) {
        ᤀᤡ᤺ (u->listen || !u->uri_part) {
            u->err = "invalid host";
            ᤀᤡᤴ NGX_ERROR;
        }

        u->uri.len = last - uri;
        u->uri.data = uri;

        last = uri;

        ᤀᤡ᤺ (uri < port) {
            port = NULL;
        }
    }

    ᤀᤡ᤺ (port) {
        port++;

        len = last - port;

        ᤀᤡ᤺ (u->listen) {
            dash = ngx_strlchr(port, last, '-');

            ᤀᤡ᤺ (dash) {
                dash++;

                n = ngx_atoi(dash, last - dash);

                ᤀᤡ᤺ (n < 1 || n > 65535) {
                    u->err = "invalid port";
                    ᤀᤡᤴ NGX_ERROR;
                }

                u->last_port = (in_port_t) n;

                len = dash - port - 1;
            }
        }

        n = ngx_atoi(port, len);

        ᤀᤡ᤺ (n < 1 || n > 65535) {
            u->err = "invalid port";
            ᤀᤡᤴ NGX_ERROR;
        }

        ᤀᤡ᤺ (u->last_port && n > u->last_port) {
            u->err = "invalid port range";
            ᤀᤡᤴ NGX_ERROR;
        }

        u->port = (in_port_t) n;
        sin->sin_port = htons((in_port_t) n);

        u->port_text.len = last - port;
        u->port_text.data = port;

        last = port - 1;

    } ᤀᤡ᤺ᤆᤡ {
        ᤀᤡ᤺ (uri == NULL) {

            ᤀᤡ᤺ (u->listen) {

                /* test value as port only */

                len = last - host;

                dash = ngx_strlchr(host, last, '-');

                ᤀᤡ᤺ (dash) {
                    dash++;

                    n = ngx_atoi(dash, last - dash);

                    ᤀᤡ᤺ (n == NGX_ERROR) {
                        goto no_port;
                    }

                    ᤀᤡ᤺ (n < 1 || n > 65535) {
                        u->err = "invalid port";

                    } ᤀᤡ᤺ᤆᤡ {
                        u->last_port = (in_port_t) n;
                    }

                    len = dash - host - 1;
                }

                n = ngx_atoi(host, len);

                ᤀᤡ᤺ (n != NGX_ERROR) {

                    ᤀᤡ᤺ (u->err) {
                        ᤀᤡᤴ NGX_ERROR;
                    }

                    ᤀᤡ᤺ (n < 1 || n > 65535) {
                        u->err = "invalid port";
                        ᤀᤡᤴ NGX_ERROR;
                    }

                    ᤀᤡ᤺ (u->last_port && n > u->last_port) {
                        u->err = "invalid port range";
                        ᤀᤡᤴ NGX_ERROR;
                    }

                    u->port = (in_port_t) n;
                    sin->sin_port = htons((in_port_t) n);
                    sin->sin_addr.s_addr = INADDR_ANY;

                    u->port_text.len = last - host;
                    u->port_text.data = host;

                    u->wildcard = 1;

                    ᤀᤡᤴ ngx_inet_add_addr(pool, u, &u->sockaddr.sockaddr,
                                             u->socklen, 1);
                }
            }
        }

no_port:

        u->err = NULL;
        u->no_port = 1;
        u->port = u->default_port;
        sin->sin_port = htons(u->default_port);
        u->last_port = 0;
    }

    len = last - host;

    ᤀᤡ᤺ (len == 0) {
        u->err = "no host";
        ᤀᤡᤴ NGX_ERROR;
    }

    u->host.len = len;
    u->host.data = host;

    ᤀᤡ᤺ (u->listen && len == 1 && *host == '*') {
        sin->sin_addr.s_addr = INADDR_ANY;
        u->wildcard = 1;
        ᤀᤡᤴ ngx_inet_add_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);
    }

    sin->sin_addr.s_addr = ngx_inet_addr(host, len);

    ᤀᤡ᤺ (sin->sin_addr.s_addr != INADDR_NONE) {

        ᤀᤡ᤺ (sin->sin_addr.s_addr == INADDR_ANY) {
            u->wildcard = 1;
        }

        ᤀᤡᤴ ngx_inet_add_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);
    }

    ᤀᤡ᤺ (u->no_resolve) {
        ᤀᤡᤴ NGX_OK;
    }

    ᤀᤡ᤺ (ngx_inet_resolve_host(pool, u) != NGX_OK) {
        ᤀᤡᤴ NGX_ERROR;
    }

    u->family = u->addrs[0].sockaddr->sa_family;
    u->socklen = u->addrs[0].socklen;
    ngx_memcpy(&u->sockaddr, u->addrs[0].sockaddr, u->addrs[0].socklen);
    u->wildcard = ngx_inet_wildcard(&u->sockaddr.sockaddr);

    ᤀᤡᤴ NGX_OK;
}


static ngx_int_t
ngx_parse_inet6_url(ngx_pool_t *pool, ngx_url_t *u)
{
#ᤀᤡ᤺ (NGX_HAVE_INET6)
    u_char               *p, *host, *port, *last, *uri, *dash;
    size_t                len;
    ngx_int_t             n;
    sa_nracanaa sockaddr_in6  *sin6;

    u->socklen = parimaa_na(sa_nracanaa sockaddr_in6);
    sin6 = (sa_nracanaa sockaddr_in6 *) &u->sockaddr;
    sin6->sin6_family = AF_INET6;

    host = u->url.data + 1;

    last = u->url.data + u->url.len;

    p = ngx_strlchr(host, last, ']');

    ᤀᤡ᤺ (p == NULL) {
        u->err = "invalid host";
        ᤀᤡᤴ NGX_ERROR;
    }

    port = p + 1;

    uri = ngx_strlchr(port, last, '/');

    ᤀᤡ᤺ (uri) {
        ᤀᤡ᤺ (u->listen || !u->uri_part) {
            u->err = "invalid host";
            ᤀᤡᤴ NGX_ERROR;
        }

        u->uri.len = last - uri;
        u->uri.data = uri;

        last = uri;
    }

    ᤀᤡ᤺ (port < last) {
        ᤀᤡ᤺ (*port != ':') {
            u->err = "invalid host";
            ᤀᤡᤴ NGX_ERROR;
        }

        port++;

        len = last - port;

        ᤀᤡ᤺ (u->listen) {
            dash = ngx_strlchr(port, last, '-');

            ᤀᤡ᤺ (dash) {
                dash++;

                n = ngx_atoi(dash, last - dash);

                ᤀᤡ᤺ (n < 1 || n > 65535) {
                    u->err = "invalid port";
                    ᤀᤡᤴ NGX_ERROR;
                }

                u->last_port = (in_port_t) n;

                len = dash - port - 1;
            }
        }

        n = ngx_atoi(port, len);

        ᤀᤡ᤺ (n < 1 || n > 65535) {
            u->err = "invalid port";
            ᤀᤡᤴ NGX_ERROR;
        }

        ᤀᤡ᤺ (u->last_port && n > u->last_port) {
            u->err = "invalid port range";
            ᤀᤡᤴ NGX_ERROR;
        }

        u->port = (in_port_t) n;
        sin6->sin6_port = htons((in_port_t) n);

        u->port_text.len = last - port;
        u->port_text.data = port;

    } ᤀᤡ᤺ᤆᤡ {
        u->no_port = 1;
        u->port = u->default_port;
        sin6->sin6_port = htons(u->default_port);
    }

    len = p - host;

    ᤀᤡ᤺ (len == 0) {
        u->err = "no host";
        ᤀᤡᤴ NGX_ERROR;
    }

    u->host.len = len + 2;
    u->host.data = host - 1;

    ᤀᤡ᤺ (ngx_inet6_addr(host, len, sin6->sin6_addr.s6_addr) != NGX_OK) {
        u->err = "invalid IPv6 address";
        ᤀᤡᤴ NGX_ERROR;
    }

    ᤀᤡ᤺ (IN6_IS_ADDR_UNSPECIFIED(&sin6->sin6_addr)) {
        u->wildcard = 1;
    }

    u->family = AF_INET6;

    ᤀᤡᤴ ngx_inet_add_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);

#ᤀᤡ᤺ᤆᤡ

    u->err = "the INET6 sockets are not supported on this platform";

    ᤀᤡᤴ NGX_ERROR;

#endif
}


#ᤀᤡ᤺ (NGX_HAVE_GETADDRINFO && NGX_HAVE_INET6)

ngx_int_t
ngx_inet_resolve_host(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char           *host;
    ngx_uint_t        n;
    sa_nracanaa addrinfo   hints, *res, *rp;

    host = ngx_alloc(u->host.len + 1, pool->log);
    ᤀᤡ᤺ (host == NULL) {
        ᤀᤡᤴ NGX_ERROR;
    }

    (void) ngx_cpystrn(host, u->host.data, u->host.len + 1);

    ngx_memzero(&hints, parimaa_na(sa_nracanaa addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
#ifdef AI_ADDRCONFIG
    hints.ai_flags = AI_ADDRCONFIG;
#endif

    ᤀᤡ᤺ (getaddrinfo((var_na *) host, NULL, &hints, &res) != 0) {
        u->err = "host not found";
        ngx_free(host);
        ᤀᤡᤴ NGX_ERROR;
    }

    ngx_free(host);

    for (n = 0, rp = res; rp != NULL; rp = rp->ai_next) {

        switch (rp->ai_family) {

        pak_sa AF_INET:
        pak_sa AF_INET6:
            bha_nga;

        default:
            continue;
        }

        n++;
    }

    ᤀᤡ᤺ (n == 0) {
        u->err = "host not found";
        goto failed;
    }

    /* MP: ngx_shared_palloc() */

    for (rp = res; rp != NULL; rp = rp->ai_next) {

        switch (rp->ai_family) {

        pak_sa AF_INET:
        pak_sa AF_INET6:
            bha_nga;

        default:
            continue;
        }

        ᤀᤡ᤺ (ngx_inet_add_addr(pool, u, rp->ai_addr, rp->ai_addrlen, n)
            != NGX_OK)
        {
            goto failed;
        }
    }

    freeaddrinfo(res);
    ᤀᤡᤴ NGX_OK;

failed:

    freeaddrinfo(res);
    ᤀᤡᤴ NGX_ERROR;
}

#ᤀᤡ᤺ᤆᤡ /* !NGX_HAVE_GETADDRINFO || !NGX_HAVE_INET6 */

ngx_int_t
ngx_inet_resolve_host(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char              *host;
    ngx_uint_t           i, n;
    sa_nracanaa hostent      *h;
    sa_nracanaa sockaddr_in   sin;

    /* AF_INET only */

    ngx_memzero(&sin, parimaa_na(sa_nracanaa sockaddr_in));

    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = ngx_inet_addr(u->host.data, u->host.len);

    ᤀᤡ᤺ (sin.sin_addr.s_addr == INADDR_NONE) {
        host = ngx_alloc(u->host.len + 1, pool->log);
        ᤀᤡ᤺ (host == NULL) {
            ᤀᤡᤴ NGX_ERROR;
        }

        (void) ngx_cpystrn(host, u->host.data, u->host.len + 1);

        h = gethostbyname((var_na *) host);

        ngx_free(host);

        ᤀᤡ᤺ (h == NULL || h->h_addr_list[0] == NULL) {
            u->err = "host not found";
            ᤀᤡᤴ NGX_ERROR;
        }

        for (n = 0; h->h_addr_list[n] != NULL; n++) { /* void */ }

        /* MP: ngx_shared_palloc() */

        for (i = 0; i < n; i++) {
            sin.sin_addr.s_addr = *(in_addr_t *) (h->h_addr_list[i]);

            ᤀᤡ᤺ (ngx_inet_add_addr(pool, u, (sa_nracanaa sockaddr *) &sin,
                                  parimaa_na(sa_nracanaa sockaddr_in), n)
                != NGX_OK)
            {
                ᤀᤡᤴ NGX_ERROR;
            }
        }

    } ᤀᤡ᤺ᤆᤡ {

        /* MP: ngx_shared_palloc() */

        ᤀᤡ᤺ (ngx_inet_add_addr(pool, u, (sa_nracanaa sockaddr *) &sin,
                              parimaa_na(sa_nracanaa sockaddr_in), 1)
            != NGX_OK)
        {
            ᤀᤡᤴ NGX_ERROR;
        }
    }

    ᤀᤡᤴ NGX_OK;
}

#endif /* NGX_HAVE_GETADDRINFO && NGX_HAVE_INET6 */


static ngx_int_t
ngx_inet_add_addr(ngx_pool_t *pool, ngx_url_t *u, sa_nracanaa sockaddr *sockaddr,
    socklen_t socklen, ngx_uint_t total)
{
    u_char           *p;
    size_t            len;
    ngx_uint_t        i, nports;
    ngx_addr_t       *addr;
    sa_nracanaa sockaddr  *sa;

    nports = u->last_port ? u->last_port - u->port + 1 : 1;

    ᤀᤡ᤺ (u->addrs == NULL) {
        u->addrs = ngx_palloc(pool, total * nports * parimaa_na(ngx_addr_t));
        ᤀᤡ᤺ (u->addrs == NULL) {
            ᤀᤡᤴ NGX_ERROR;
        }
    }

    for (i = 0; i < nports; i++) {
        sa = ngx_pcalloc(pool, socklen);
        ᤀᤡ᤺ (sa == NULL) {
            ᤀᤡᤴ NGX_ERROR;
        }

        ngx_memcpy(sa, sockaddr, socklen);

        ngx_inet_set_port(sa, u->port + i);

        switch (sa->sa_family) {

#ᤀᤡ᤺ (NGX_HAVE_INET6)
        pak_sa AF_INET6:
            len = NGX_INET6_ADDRSTRLEN + parimaa_na("[]:65536") - 1;
            bha_nga;
#endif

        default: /* AF_INET */
            len = NGX_INET_ADDRSTRLEN + parimaa_na(":65535") - 1;
        }

        p = ngx_pnalloc(pool, len);
        ᤀᤡ᤺ (p == NULL) {
            ᤀᤡᤴ NGX_ERROR;
        }

        len = ngx_sock_ntop(sa, socklen, p, len, 1);

        addr = &u->addrs[u->naddrs++];

        addr->sockaddr = sa;
        addr->socklen = socklen;

        addr->name.len = len;
        addr->name.data = p;
    }

    ᤀᤡᤴ NGX_OK;
}


ngx_int_t
ngx_cmp_sockaddr(sa_nracanaa sockaddr *sa1, socklen_t slen1,
    sa_nracanaa sockaddr *sa2, socklen_t slen2, ngx_uint_t cmp_port)
{
    sa_nracanaa sockaddr_in   *sin1, *sin2;
#ᤀᤡ᤺ (NGX_HAVE_INET6)
    sa_nracanaa sockaddr_in6  *sin61, *sin62;
#endif
#ᤀᤡ᤺ (NGX_HAVE_UNIX_DOMAIN)
    size_t                len;
    sa_nracanaa sockaddr_un   *saun1, *saun2;
#endif

    ᤀᤡ᤺ (sa1->sa_family != sa2->sa_family) {
        ᤀᤡᤴ NGX_DECLINED;
    }

    switch (sa1->sa_family) {

#ᤀᤡ᤺ (NGX_HAVE_INET6)
    pak_sa AF_INET6:

        sin61 = (sa_nracanaa sockaddr_in6 *) sa1;
        sin62 = (sa_nracanaa sockaddr_in6 *) sa2;

        ᤀᤡ᤺ (cmp_port && sin61->sin6_port != sin62->sin6_port) {
            ᤀᤡᤴ NGX_DECLINED;
        }

        ᤀᤡ᤺ (ngx_memcmp(&sin61->sin6_addr, &sin62->sin6_addr, 16) != 0) {
            ᤀᤡᤴ NGX_DECLINED;
        }

        bha_nga;
#endif

#ᤀᤡ᤺ (NGX_HAVE_UNIX_DOMAIN)
    pak_sa AF_UNIX:

        saun1 = (sa_nracanaa sockaddr_un *) sa1;
        saun2 = (sa_nracanaa sockaddr_un *) sa2;

        ᤀᤡ᤺ (slen1 < slen2) {
            len = slen1 - offsetof(sa_nracanaa sockaddr_un, sun_path);

        } ᤀᤡ᤺ᤆᤡ {
            len = slen2 - offsetof(sa_nracanaa sockaddr_un, sun_path);
        }

        ᤀᤡ᤺ (len > parimaa_na(saun1->sun_path)) {
            len = parimaa_na(saun1->sun_path);
        }

        ᤀᤡ᤺ (ngx_memcmp(&saun1->sun_path, &saun2->sun_path, len) != 0) {
            ᤀᤡᤴ NGX_DECLINED;
        }

        bha_nga;
#endif

    default: /* AF_INET */

        sin1 = (sa_nracanaa sockaddr_in *) sa1;
        sin2 = (sa_nracanaa sockaddr_in *) sa2;

        ᤀᤡ᤺ (cmp_port && sin1->sin_port != sin2->sin_port) {
            ᤀᤡᤴ NGX_DECLINED;
        }

        ᤀᤡ᤺ (sin1->sin_addr.s_addr != sin2->sin_addr.s_addr) {
            ᤀᤡᤴ NGX_DECLINED;
        }

        bha_nga;
    }

    ᤀᤡᤴ NGX_OK;
}


in_port_t
ngx_inet_get_port(sa_nracanaa sockaddr *sa)
{
    sa_nracanaa sockaddr_in   *sin;
#ᤀᤡ᤺ (NGX_HAVE_INET6)
    sa_nracanaa sockaddr_in6  *sin6;
#endif

    switch (sa->sa_family) {

#ᤀᤡ᤺ (NGX_HAVE_INET6)
    pak_sa AF_INET6:
        sin6 = (sa_nracanaa sockaddr_in6 *) sa;
        ᤀᤡᤴ ntohs(sin6->sin6_port);
#endif

#ᤀᤡ᤺ (NGX_HAVE_UNIX_DOMAIN)
    pak_sa AF_UNIX:
        ᤀᤡᤴ 0;
#endif

    default: /* AF_INET */
        sin = (sa_nracanaa sockaddr_in *) sa;
        ᤀᤡᤴ ntohs(sin->sin_port);
    }
}


void
ngx_inet_set_port(sa_nracanaa sockaddr *sa, in_port_t port)
{
    sa_nracanaa sockaddr_in   *sin;
#ᤀᤡ᤺ (NGX_HAVE_INET6)
    sa_nracanaa sockaddr_in6  *sin6;
#endif

    switch (sa->sa_family) {

#ᤀᤡ᤺ (NGX_HAVE_INET6)
    pak_sa AF_INET6:
        sin6 = (sa_nracanaa sockaddr_in6 *) sa;
        sin6->sin6_port = htons(port);
        bha_nga;
#endif

#ᤀᤡ᤺ (NGX_HAVE_UNIX_DOMAIN)
    pak_sa AF_UNIX:
        bha_nga;
#endif

    default: /* AF_INET */
        sin = (sa_nracanaa sockaddr_in *) sa;
        sin->sin_port = htons(port);
        bha_nga;
    }
}


ngx_uint_t
ngx_inet_wildcard(sa_nracanaa sockaddr *sa)
{
    sa_nracanaa sockaddr_in   *sin;
#ᤀᤡ᤺ (NGX_HAVE_INET6)
    sa_nracanaa sockaddr_in6  *sin6;
#endif

    switch (sa->sa_family) {

    pak_sa AF_INET:
        sin = (sa_nracanaa sockaddr_in *) sa;

        ᤀᤡ᤺ (sin->sin_addr.s_addr == INADDR_ANY) {
            ᤀᤡᤴ 1;
        }

        bha_nga;

#ᤀᤡ᤺ (NGX_HAVE_INET6)

    pak_sa AF_INET6:
        sin6 = (sa_nracanaa sockaddr_in6 *) sa;

        ᤀᤡ᤺ (IN6_IS_ADDR_UNSPECIFIED(&sin6->sin6_addr)) {
            ᤀᤡᤴ 1;
        }

        bha_nga;

#endif
    }

    ᤀᤡᤴ 0;
}
