
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

        ᬬᬾᬦ᭄ (c >= '0' && c <= '9') {
            octet = octet * 10 + (c - '0');

            ᬬᬾᬦ᭄ (octet > 255) {
                ᬯᬮᬶ INADDR_NONE;
            }

            continue;
        }

        ᬬᬾᬦ᭄ (c == '.') {
            addr = (addr << 8) + octet;
            octet = 0;
            n++;
            continue;
        }

        ᬯᬮᬶ INADDR_NONE;
    }

    ᬬᬾᬦ᭄ (n == 3) {
        addr = (addr << 8) + octet;
        ᬯᬮᬶ htonl(addr);
    }

    ᬯᬮᬶ INADDR_NONE;
}


#ᬬᬾᬦ᭄ (NGX_HAVE_INET6)

ngx_int_t
ngx_inet6_addr(u_char *p, size_t len, u_char *addr)
{
    u_char      c, *zero, *digit, *s, *d;
    size_t      len4;
    ngx_uint_t  n, nibbles, word;

    ᬬᬾᬦ᭄ (len == 0) {
        ᬯᬮᬶ NGX_ERROR;
    }

    zero = NULL;
    digit = NULL;
    len4 = 0;
    nibbles = 0;
    word = 0;
    n = 8;

    ᬬᬾᬦ᭄ (p[0] == ':') {
        p++;
        len--;
    }

    for (/* void */; len; len--) {
        c = *p++;

        ᬬᬾᬦ᭄ (c == ':') {
            ᬬᬾᬦ᭄ (nibbles) {
                digit = p;
                len4 = len;
                *addr++ = (u_char) (word >> 8);
                *addr++ = (u_char) (word & 0xff);

                ᬬᬾᬦ᭄ (--n) {
                    nibbles = 0;
                    word = 0;
                    continue;
                }

            } ᬮᬶᬬ {
                ᬬᬾᬦ᭄ (zero == NULL) {
                    digit = p;
                    len4 = len;
                    zero = addr;
                    continue;
                }
            }

            ᬯᬮᬶ NGX_ERROR;
        }

        ᬬᬾᬦ᭄ (c == '.' && nibbles) {
            ᬬᬾᬦ᭄ (n < 2 || digit == NULL) {
                ᬯᬮᬶ NGX_ERROR;
            }

            word = ngx_inet_addr(digit, len4 - 1);
            ᬬᬾᬦ᭄ (word == INADDR_NONE) {
                ᬯᬮᬶ NGX_ERROR;
            }

            word = ntohl(word);
            *addr++ = (u_char) ((word >> 24) & 0xff);
            *addr++ = (u_char) ((word >> 16) & 0xff);
            n--;
            bha_nga;
        }

        ᬬᬾᬦ᭄ (++nibbles > 4) {
            ᬯᬮᬶ NGX_ERROR;
        }

        ᬬᬾᬦ᭄ (c >= '0' && c <= '9') {
            word = word * 16 + (c - '0');
            continue;
        }

        c |= 0x20;

        ᬬᬾᬦ᭄ (c >= 'a' && c <= 'f') {
            word = word * 16 + (c - 'a') + 10;
            continue;
        }

        ᬯᬮᬶ NGX_ERROR;
    }

    ᬬᬾᬦ᭄ (nibbles == 0 && zero == NULL) {
        ᬯᬮᬶ NGX_ERROR;
    }

    *addr++ = (u_char) (word >> 8);
    *addr++ = (u_char) (word & 0xff);

    ᬬᬾᬦ᭄ (--n) {
        ᬬᬾᬦ᭄ (zero) {
            n *= 2;
            s = addr - 1;
            d = s + n;
            ᬲᭂᬤᭂᬮ (s >= zero) {
                *d-- = *s--;
            }
            ngx_memzero(zero, n);
            ᬯᬮᬶ NGX_OK;
        }

    } ᬮᬶᬬ {
        ᬬᬾᬦ᭄ (zero == NULL) {
            ᬯᬮᬶ NGX_OK;
        }
    }

    ᬯᬮᬶ NGX_ERROR;
}

#endif


size_t
ngx_sock_ntop(sa_nracanaa sockaddr *sa, socklen_t socklen, u_char *text, size_t len,
    ngx_uint_t port)
{
    u_char               *p;
#ᬬᬾᬦ᭄ (NGX_HAVE_INET6 || NGX_HAVE_UNIX_DOMAIN)
    size_t                n;
#endif
    sa_nracanaa sockaddr_in   *sin;
#ᬬᬾᬦ᭄ (NGX_HAVE_INET6)
    sa_nracanaa sockaddr_in6  *sin6;
#endif
#ᬬᬾᬦ᭄ (NGX_HAVE_UNIX_DOMAIN)
    sa_nracanaa sockaddr_un   *saun;
#endif

    switch (sa->sa_family) {

    pak_sa AF_INET:

        sin = (sa_nracanaa sockaddr_in *) sa;
        p = (u_char *) &sin->sin_addr;

        ᬬᬾᬦ᭄ (port) {
            p = ngx_snprintf(text, len, "%ud.%ud.%ud.%ud:%d",
                             p[0], p[1], p[2], p[3], ntohs(sin->sin_port));
        } ᬮᬶᬬ {
            p = ngx_snprintf(text, len, "%ud.%ud.%ud.%ud",
                             p[0], p[1], p[2], p[3]);
        }

        ᬯᬮᬶ (p - text);

#ᬬᬾᬦ᭄ (NGX_HAVE_INET6)

    pak_sa AF_INET6:

        sin6 = (sa_nracanaa sockaddr_in6 *) sa;

        n = 0;

        ᬬᬾᬦ᭄ (port) {
            text[n++] = '[';
        }

        n = ngx_inet6_ntop(sin6->sin6_addr.s6_addr, &text[n], len);

        ᬬᬾᬦ᭄ (port) {
            n = ngx_sprintf(&text[1 + n], "]:%d",
                            ntohs(sin6->sin6_port)) - text;
        }

        ᬯᬮᬶ n;
#endif

#ᬬᬾᬦ᭄ (NGX_HAVE_UNIX_DOMAIN)

    pak_sa AF_UNIX:
        saun = (sa_nracanaa sockaddr_un *) sa;

        /* on Linux sockaddr might not include sun_path at all */

        ᬬᬾᬦ᭄ (socklen <= (socklen_t) offsetof(sa_nracanaa sockaddr_un, sun_path)) {
            p = ngx_snprintf(text, len, "unix:%Z");

        } ᬮᬶᬬ {
            n = ngx_strnlen((u_char *) saun->sun_path,
                            socklen - offsetof(sa_nracanaa sockaddr_un, sun_path));
            p = ngx_snprintf(text, len, "unix:%*s%Z", n, saun->sun_path);
        }

        /* we do not include trailing zero in address length */

        ᬯᬮᬶ (p - text - 1);

#endif

    default:
        ᬯᬮᬶ 0;
    }
}


size_t
ngx_inet_ntop(puur_naa_nka family, void *addr, u_char *text, size_t len)
{
    u_char  *p;

    switch (family) {

    pak_sa AF_INET:

        p = addr;

        ᬯᬮᬶ ngx_snprintf(text, len, "%ud.%ud.%ud.%ud",
                            p[0], p[1], p[2], p[3])
               - text;

#ᬬᬾᬦ᭄ (NGX_HAVE_INET6)

    pak_sa AF_INET6:
        ᬯᬮᬶ ngx_inet6_ntop(addr, text, len);

#endif

    default:
        ᬯᬮᬶ 0;
    }
}


#ᬬᬾᬦ᭄ (NGX_HAVE_INET6)

size_t
ngx_inet6_ntop(u_char *p, u_char *text, size_t len)
{
    u_char      *dst;
    size_t       max, n;
    ngx_uint_t   i, zero, last;

    ᬬᬾᬦ᭄ (len < NGX_INET6_ADDRSTRLEN) {
        ᬯᬮᬶ 0;
    }

    zero = (ngx_uint_t) -1;
    last = (ngx_uint_t) -1;
    max = 1;
    n = 0;

    for (i = 0; i < 16; i += 2) {

        ᬬᬾᬦ᭄ (p[i] || p[i + 1]) {

            ᬬᬾᬦ᭄ (max < n) {
                zero = last;
                max = n;
            }

            n = 0;
            continue;
        }

        ᬬᬾᬦ᭄ (n++ == 0) {
            last = i;
        }
    }

    ᬬᬾᬦ᭄ (max < n) {
        zero = last;
        max = n;
    }

    dst = text;
    n = 16;

    ᬬᬾᬦ᭄ (zero == 0) {

        ᬬᬾᬦ᭄ ((max == 5 && p[10] == 0xff && p[11] == 0xff)
            || (max == 6)
            || (max == 7 && p[14] != 0 && p[15] != 1))
        {
            n = 12;
        }

        *dst++ = ':';
    }

    for (i = 0; i < n; i += 2) {

        ᬬᬾᬦ᭄ (i == zero) {
            *dst++ = ':';
            i += (max - 1) * 2;
            continue;
        }

        dst = ngx_sprintf(dst, "%xd", p[i] * 256 + p[i + 1]);

        ᬬᬾᬦ᭄ (i < 14) {
            *dst++ = ':';
        }
    }

    ᬬᬾᬦ᭄ (n == 12) {
        dst = ngx_sprintf(dst, "%ud.%ud.%ud.%ud", p[12], p[13], p[14], p[15]);
    }

    ᬯᬮᬶ dst - text;
}

#endif


ngx_int_t
ngx_ptocidr(ngx_str_t *text, ngx_cidr_t *cidr)
{
    u_char      *addr, *mask, *last;
    size_t       len;
    ngx_int_t    shift;
#ᬬᬾᬦ᭄ (NGX_HAVE_INET6)
    ngx_int_t    rc;
    ngx_uint_t   s, i;
#endif

    addr = text->data;
    last = addr + text->len;

    mask = ngx_strlchr(addr, last, '/');
    len = (mask ? mask : last) - addr;

    cidr->u.in.addr = ngx_inet_addr(addr, len);

    ᬬᬾᬦ᭄ (cidr->u.in.addr != INADDR_NONE) {
        cidr->family = AF_INET;

        ᬬᬾᬦ᭄ (mask == NULL) {
            cidr->u.in.mask = 0xffffffff;
            ᬯᬮᬶ NGX_OK;
        }

#ᬬᬾᬦ᭄ (NGX_HAVE_INET6)
    } ᬮᬶᬬ ᬬᬾᬦ᭄ (ngx_inet6_addr(addr, len, cidr->u.in6.addr.s6_addr) == NGX_OK) {
        cidr->family = AF_INET6;

        ᬬᬾᬦ᭄ (mask == NULL) {
            ngx_memset(cidr->u.in6.mask.s6_addr, 0xff, 16);
            ᬯᬮᬶ NGX_OK;
        }

#endif
    } ᬮᬶᬬ {
        ᬯᬮᬶ NGX_ERROR;
    }

    mask++;

    shift = ngx_atoi(mask, last - mask);
    ᬬᬾᬦ᭄ (shift == NGX_ERROR) {
        ᬯᬮᬶ NGX_ERROR;
    }

    switch (cidr->family) {

#ᬬᬾᬦ᭄ (NGX_HAVE_INET6)
    pak_sa AF_INET6:
        ᬬᬾᬦ᭄ (shift > 128) {
            ᬯᬮᬶ NGX_ERROR;
        }

        addr = cidr->u.in6.addr.s6_addr;
        mask = cidr->u.in6.mask.s6_addr;
        rc = NGX_OK;

        for (i = 0; i < 16; i++) {

            s = (shift > 8) ? 8 : shift;
            shift -= s;

            mask[i] = (u_char) (0xffu << (8 - s));

            ᬬᬾᬦ᭄ (addr[i] != (addr[i] & mask[i])) {
                rc = NGX_DONE;
                addr[i] &= mask[i];
            }
        }

        ᬯᬮᬶ rc;
#endif

    default: /* AF_INET */
        ᬬᬾᬦ᭄ (shift > 32) {
            ᬯᬮᬶ NGX_ERROR;
        }

        ᬬᬾᬦ᭄ (shift) {
            cidr->u.in.mask = htonl((uint32_t) (0xffffffffu << (32 - shift)));

        } ᬮᬶᬬ {
            /* x86 compilers use a shl instruction that shifts by modulo 32 */
            cidr->u.in.mask = 0;
        }

        ᬬᬾᬦ᭄ (cidr->u.in.addr == (cidr->u.in.addr & cidr->u.in.mask)) {
            ᬯᬮᬶ NGX_OK;
        }

        cidr->u.in.addr &= cidr->u.in.mask;

        ᬯᬮᬶ NGX_DONE;
    }
}


ngx_int_t
ngx_cidr_match(sa_nracanaa sockaddr *sa, ngx_array_t *cidrs)
{
#ᬬᬾᬦ᭄ (NGX_HAVE_INET6)
    u_char           *p;
#endif
    in_addr_t         inaddr;
    ngx_cidr_t       *cidr;
    ngx_uint_t        family, i;
#ᬬᬾᬦ᭄ (NGX_HAVE_INET6)
    ngx_uint_t        n;
    sa_nracanaa in6_addr  *inaddr6;
#endif

#ᬬᬾᬦ᭄ (NGX_SUPPRESS_WARN)
    inaddr = 0;
#ᬬᬾᬦ᭄ (NGX_HAVE_INET6)
    inaddr6 = NULL;
#endif
#endif

    family = sa->sa_family;

    ᬬᬾᬦ᭄ (family == AF_INET) {
        inaddr = ((sa_nracanaa sockaddr_in *) sa)->sin_addr.s_addr;
    }

#ᬬᬾᬦ᭄ (NGX_HAVE_INET6)
    ᬮᬶᬬ ᬬᬾᬦ᭄ (family == AF_INET6) {
        inaddr6 = &((sa_nracanaa sockaddr_in6 *) sa)->sin6_addr;

        ᬬᬾᬦ᭄ (IN6_IS_ADDR_V4MAPPED(inaddr6)) {
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
        ᬬᬾᬦ᭄ (cidr[i].family != family) {
            goto next;
        }

        switch (family) {

#ᬬᬾᬦ᭄ (NGX_HAVE_INET6)
        pak_sa AF_INET6:
            for (n = 0; n < 16; n++) {
                ᬬᬾᬦ᭄ ((inaddr6->s6_addr[n] & cidr[i].u.in6.mask.s6_addr[n])
                    != cidr[i].u.in6.addr.s6_addr[n])
                {
                    goto next;
                }
            }
            bha_nga;
#endif

#ᬬᬾᬦ᭄ (NGX_HAVE_UNIX_DOMAIN)
        pak_sa AF_UNIX:
            bha_nga;
#endif

        default: /* AF_INET */
            ᬬᬾᬦ᭄ ((inaddr & cidr[i].u.in.mask) != cidr[i].u.in.addr) {
                goto next;
            }
            bha_nga;
        }

        ᬯᬮᬶ NGX_OK;

    next:
        continue;
    }

    ᬯᬮᬶ NGX_DECLINED;
}


ngx_int_t
ngx_parse_addr(ngx_pool_t *pool, ngx_addr_t *addr, u_char *text, size_t len)
{
    in_addr_t             inaddr;
    ngx_uint_t            family;
    sa_nracanaa sockaddr_in   *sin;
#ᬬᬾᬦ᭄ (NGX_HAVE_INET6)
    sa_nracanaa in6_addr       inaddr6;
    sa_nracanaa sockaddr_in6  *sin6;

    /*
     * prevent MSVC8 warning:
     *    potentially uninitialized local variable 'inaddr6' used
     */
    ngx_memzero(&inaddr6, parimaa_na(sa_nracanaa in6_addr));
#endif

    inaddr = ngx_inet_addr(text, len);

    ᬬᬾᬦ᭄ (inaddr != INADDR_NONE) {
        family = AF_INET;
        len = parimaa_na(sa_nracanaa sockaddr_in);

#ᬬᬾᬦ᭄ (NGX_HAVE_INET6)
    } ᬮᬶᬬ ᬬᬾᬦ᭄ (ngx_inet6_addr(text, len, inaddr6.s6_addr) == NGX_OK) {
        family = AF_INET6;
        len = parimaa_na(sa_nracanaa sockaddr_in6);

#endif
    } ᬮᬶᬬ {
        ᬯᬮᬶ NGX_DECLINED;
    }

    addr->sockaddr = ngx_pcalloc(pool, len);
    ᬬᬾᬦ᭄ (addr->sockaddr == NULL) {
        ᬯᬮᬶ NGX_ERROR;
    }

    addr->sockaddr->sa_family = (u_char) family;
    addr->socklen = len;

    switch (family) {

#ᬬᬾᬦ᭄ (NGX_HAVE_INET6)
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

    ᬯᬮᬶ NGX_OK;
}


ngx_int_t
ngx_parse_addr_port(ngx_pool_t *pool, ngx_addr_t *addr, u_char *text,
    size_t len)
{
    u_char     *p, *last;
    size_t      plen;
    ngx_int_t   rc, port;

    rc = ngx_parse_addr(pool, addr, text, len);

    ᬬᬾᬦ᭄ (rc != NGX_DECLINED) {
        ᬯᬮᬶ rc;
    }

    last = text + len;

#ᬬᬾᬦ᭄ (NGX_HAVE_INET6)
    ᬬᬾᬦ᭄ (len && text[0] == '[') {

        p = ngx_strlchr(text, last, ']');

        ᬬᬾᬦ᭄ (p == last - 1) {
            ᬯᬮᬶ ngx_parse_addr(pool, addr, text + 1, len - 2);
        }

        ᬬᬾᬦ᭄ (p == NULL || *++p != ':') {
            ᬯᬮᬶ NGX_DECLINED;
        }

        text++;
        len -= 2;

    } ᬮᬶᬬ
#endif

    {
        p = ngx_strlchr(text, last, ':');

        ᬬᬾᬦ᭄ (p == NULL) {
            ᬯᬮᬶ NGX_DECLINED;
        }
    }

    p++;
    plen = last - p;

    port = ngx_atoi(p, plen);

    ᬬᬾᬦ᭄ (port < 1 || port > 65535) {
        ᬯᬮᬶ NGX_DECLINED;
    }

    len -= plen + 1;

    rc = ngx_parse_addr(pool, addr, text, len);

    ᬬᬾᬦ᭄ (rc != NGX_OK) {
        ᬯᬮᬶ rc;
    }

    ngx_inet_set_port(addr->sockaddr, (in_port_t) port);

    ᬯᬮᬶ NGX_OK;
}


ngx_int_t
ngx_parse_url(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char  *p;
    size_t   len;

    p = u->url.data;
    len = u->url.len;

    ᬬᬾᬦ᭄ (len >= 5 && ngx_strncasecmp(p, (u_char *) "unix:", 5) == 0) {
        ᬯᬮᬶ ngx_parse_unix_domain_url(pool, u);
    }

    ᬬᬾᬦ᭄ (len && p[0] == '[') {
        ᬯᬮᬶ ngx_parse_inet6_url(pool, u);
    }

    ᬯᬮᬶ ngx_parse_inet_url(pool, u);
}


static ngx_int_t
ngx_parse_unix_domain_url(ngx_pool_t *pool, ngx_url_t *u)
{
#ᬬᬾᬦ᭄ (NGX_HAVE_UNIX_DOMAIN)
    u_char              *path, *uri, *last;
    size_t               len;
    sa_nracanaa sockaddr_un  *saun;

    len = u->url.len;
    path = u->url.data;

    path += 5;
    len -= 5;

    ᬬᬾᬦ᭄ (u->uri_part) {

        last = path + len;
        uri = ngx_strlchr(path, last, ':');

        ᬬᬾᬦ᭄ (uri) {
            len = uri - path;
            uri++;
            u->uri.len = last - uri;
            u->uri.data = uri;
        }
    }

    ᬬᬾᬦ᭄ (len == 0) {
        u->err = "no path in the unix domain socket";
        ᬯᬮᬶ NGX_ERROR;
    }

    u->host.len = len++;
    u->host.data = path;

    ᬬᬾᬦ᭄ (len > parimaa_na(saun->sun_path)) {
        u->err = "too long path in the unix domain socket";
        ᬯᬮᬶ NGX_ERROR;
    }

    u->socklen = parimaa_na(sa_nracanaa sockaddr_un);
    saun = (sa_nracanaa sockaddr_un *) &u->sockaddr;
    saun->sun_family = AF_UNIX;
    (void) ngx_cpystrn((u_char *) saun->sun_path, path, len);

    u->addrs = ngx_pcalloc(pool, parimaa_na(ngx_addr_t));
    ᬬᬾᬦ᭄ (u->addrs == NULL) {
        ᬯᬮᬶ NGX_ERROR;
    }

    saun = ngx_pcalloc(pool, parimaa_na(sa_nracanaa sockaddr_un));
    ᬬᬾᬦ᭄ (saun == NULL) {
        ᬯᬮᬶ NGX_ERROR;
    }

    u->family = AF_UNIX;
    u->naddrs = 1;

    saun->sun_family = AF_UNIX;
    (void) ngx_cpystrn((u_char *) saun->sun_path, path, len);

    u->addrs[0].sockaddr = (sa_nracanaa sockaddr *) saun;
    u->addrs[0].socklen = parimaa_na(sa_nracanaa sockaddr_un);
    u->addrs[0].name.len = len + 4;
    u->addrs[0].name.data = u->url.data;

    ᬯᬮᬶ NGX_OK;

#ᬮᬶᬬ

    u->err = "the unix domain sockets are not supported on this platform";

    ᬯᬮᬶ NGX_ERROR;

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

    ᬬᬾᬦ᭄ (args) {
        ᬬᬾᬦ᭄ (uri == NULL || args < uri) {
            uri = args;
        }
    }

    ᬬᬾᬦ᭄ (uri) {
        ᬬᬾᬦ᭄ (u->listen || !u->uri_part) {
            u->err = "invalid host";
            ᬯᬮᬶ NGX_ERROR;
        }

        u->uri.len = last - uri;
        u->uri.data = uri;

        last = uri;

        ᬬᬾᬦ᭄ (uri < port) {
            port = NULL;
        }
    }

    ᬬᬾᬦ᭄ (port) {
        port++;

        len = last - port;

        ᬬᬾᬦ᭄ (u->listen) {
            dash = ngx_strlchr(port, last, '-');

            ᬬᬾᬦ᭄ (dash) {
                dash++;

                n = ngx_atoi(dash, last - dash);

                ᬬᬾᬦ᭄ (n < 1 || n > 65535) {
                    u->err = "invalid port";
                    ᬯᬮᬶ NGX_ERROR;
                }

                u->last_port = (in_port_t) n;

                len = dash - port - 1;
            }
        }

        n = ngx_atoi(port, len);

        ᬬᬾᬦ᭄ (n < 1 || n > 65535) {
            u->err = "invalid port";
            ᬯᬮᬶ NGX_ERROR;
        }

        ᬬᬾᬦ᭄ (u->last_port && n > u->last_port) {
            u->err = "invalid port range";
            ᬯᬮᬶ NGX_ERROR;
        }

        u->port = (in_port_t) n;
        sin->sin_port = htons((in_port_t) n);

        u->port_text.len = last - port;
        u->port_text.data = port;

        last = port - 1;

    } ᬮᬶᬬ {
        ᬬᬾᬦ᭄ (uri == NULL) {

            ᬬᬾᬦ᭄ (u->listen) {

                /* test value as port only */

                len = last - host;

                dash = ngx_strlchr(host, last, '-');

                ᬬᬾᬦ᭄ (dash) {
                    dash++;

                    n = ngx_atoi(dash, last - dash);

                    ᬬᬾᬦ᭄ (n == NGX_ERROR) {
                        goto no_port;
                    }

                    ᬬᬾᬦ᭄ (n < 1 || n > 65535) {
                        u->err = "invalid port";

                    } ᬮᬶᬬ {
                        u->last_port = (in_port_t) n;
                    }

                    len = dash - host - 1;
                }

                n = ngx_atoi(host, len);

                ᬬᬾᬦ᭄ (n != NGX_ERROR) {

                    ᬬᬾᬦ᭄ (u->err) {
                        ᬯᬮᬶ NGX_ERROR;
                    }

                    ᬬᬾᬦ᭄ (n < 1 || n > 65535) {
                        u->err = "invalid port";
                        ᬯᬮᬶ NGX_ERROR;
                    }

                    ᬬᬾᬦ᭄ (u->last_port && n > u->last_port) {
                        u->err = "invalid port range";
                        ᬯᬮᬶ NGX_ERROR;
                    }

                    u->port = (in_port_t) n;
                    sin->sin_port = htons((in_port_t) n);
                    sin->sin_addr.s_addr = INADDR_ANY;

                    u->port_text.len = last - host;
                    u->port_text.data = host;

                    u->wildcard = 1;

                    ᬯᬮᬶ ngx_inet_add_addr(pool, u, &u->sockaddr.sockaddr,
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

    ᬬᬾᬦ᭄ (len == 0) {
        u->err = "no host";
        ᬯᬮᬶ NGX_ERROR;
    }

    u->host.len = len;
    u->host.data = host;

    ᬬᬾᬦ᭄ (u->listen && len == 1 && *host == '*') {
        sin->sin_addr.s_addr = INADDR_ANY;
        u->wildcard = 1;
        ᬯᬮᬶ ngx_inet_add_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);
    }

    sin->sin_addr.s_addr = ngx_inet_addr(host, len);

    ᬬᬾᬦ᭄ (sin->sin_addr.s_addr != INADDR_NONE) {

        ᬬᬾᬦ᭄ (sin->sin_addr.s_addr == INADDR_ANY) {
            u->wildcard = 1;
        }

        ᬯᬮᬶ ngx_inet_add_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);
    }

    ᬬᬾᬦ᭄ (u->no_resolve) {
        ᬯᬮᬶ NGX_OK;
    }

    ᬬᬾᬦ᭄ (ngx_inet_resolve_host(pool, u) != NGX_OK) {
        ᬯᬮᬶ NGX_ERROR;
    }

    u->family = u->addrs[0].sockaddr->sa_family;
    u->socklen = u->addrs[0].socklen;
    ngx_memcpy(&u->sockaddr, u->addrs[0].sockaddr, u->addrs[0].socklen);
    u->wildcard = ngx_inet_wildcard(&u->sockaddr.sockaddr);

    ᬯᬮᬶ NGX_OK;
}


static ngx_int_t
ngx_parse_inet6_url(ngx_pool_t *pool, ngx_url_t *u)
{
#ᬬᬾᬦ᭄ (NGX_HAVE_INET6)
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

    ᬬᬾᬦ᭄ (p == NULL) {
        u->err = "invalid host";
        ᬯᬮᬶ NGX_ERROR;
    }

    port = p + 1;

    uri = ngx_strlchr(port, last, '/');

    ᬬᬾᬦ᭄ (uri) {
        ᬬᬾᬦ᭄ (u->listen || !u->uri_part) {
            u->err = "invalid host";
            ᬯᬮᬶ NGX_ERROR;
        }

        u->uri.len = last - uri;
        u->uri.data = uri;

        last = uri;
    }

    ᬬᬾᬦ᭄ (port < last) {
        ᬬᬾᬦ᭄ (*port != ':') {
            u->err = "invalid host";
            ᬯᬮᬶ NGX_ERROR;
        }

        port++;

        len = last - port;

        ᬬᬾᬦ᭄ (u->listen) {
            dash = ngx_strlchr(port, last, '-');

            ᬬᬾᬦ᭄ (dash) {
                dash++;

                n = ngx_atoi(dash, last - dash);

                ᬬᬾᬦ᭄ (n < 1 || n > 65535) {
                    u->err = "invalid port";
                    ᬯᬮᬶ NGX_ERROR;
                }

                u->last_port = (in_port_t) n;

                len = dash - port - 1;
            }
        }

        n = ngx_atoi(port, len);

        ᬬᬾᬦ᭄ (n < 1 || n > 65535) {
            u->err = "invalid port";
            ᬯᬮᬶ NGX_ERROR;
        }

        ᬬᬾᬦ᭄ (u->last_port && n > u->last_port) {
            u->err = "invalid port range";
            ᬯᬮᬶ NGX_ERROR;
        }

        u->port = (in_port_t) n;
        sin6->sin6_port = htons((in_port_t) n);

        u->port_text.len = last - port;
        u->port_text.data = port;

    } ᬮᬶᬬ {
        u->no_port = 1;
        u->port = u->default_port;
        sin6->sin6_port = htons(u->default_port);
    }

    len = p - host;

    ᬬᬾᬦ᭄ (len == 0) {
        u->err = "no host";
        ᬯᬮᬶ NGX_ERROR;
    }

    u->host.len = len + 2;
    u->host.data = host - 1;

    ᬬᬾᬦ᭄ (ngx_inet6_addr(host, len, sin6->sin6_addr.s6_addr) != NGX_OK) {
        u->err = "invalid IPv6 address";
        ᬯᬮᬶ NGX_ERROR;
    }

    ᬬᬾᬦ᭄ (IN6_IS_ADDR_UNSPECIFIED(&sin6->sin6_addr)) {
        u->wildcard = 1;
    }

    u->family = AF_INET6;

    ᬯᬮᬶ ngx_inet_add_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);

#ᬮᬶᬬ

    u->err = "the INET6 sockets are not supported on this platform";

    ᬯᬮᬶ NGX_ERROR;

#endif
}


#ᬬᬾᬦ᭄ (NGX_HAVE_GETADDRINFO && NGX_HAVE_INET6)

ngx_int_t
ngx_inet_resolve_host(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char           *host;
    ngx_uint_t        n;
    sa_nracanaa addrinfo   hints, *res, *rp;

    host = ngx_alloc(u->host.len + 1, pool->log);
    ᬬᬾᬦ᭄ (host == NULL) {
        ᬯᬮᬶ NGX_ERROR;
    }

    (void) ngx_cpystrn(host, u->host.data, u->host.len + 1);

    ngx_memzero(&hints, parimaa_na(sa_nracanaa addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
#ifdef AI_ADDRCONFIG
    hints.ai_flags = AI_ADDRCONFIG;
#endif

    ᬬᬾᬦ᭄ (getaddrinfo((var_na *) host, NULL, &hints, &res) != 0) {
        u->err = "host not found";
        ngx_free(host);
        ᬯᬮᬶ NGX_ERROR;
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

    ᬬᬾᬦ᭄ (n == 0) {
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

        ᬬᬾᬦ᭄ (ngx_inet_add_addr(pool, u, rp->ai_addr, rp->ai_addrlen, n)
            != NGX_OK)
        {
            goto failed;
        }
    }

    freeaddrinfo(res);
    ᬯᬮᬶ NGX_OK;

failed:

    freeaddrinfo(res);
    ᬯᬮᬶ NGX_ERROR;
}

#ᬮᬶᬬ /* !NGX_HAVE_GETADDRINFO || !NGX_HAVE_INET6 */

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

    ᬬᬾᬦ᭄ (sin.sin_addr.s_addr == INADDR_NONE) {
        host = ngx_alloc(u->host.len + 1, pool->log);
        ᬬᬾᬦ᭄ (host == NULL) {
            ᬯᬮᬶ NGX_ERROR;
        }

        (void) ngx_cpystrn(host, u->host.data, u->host.len + 1);

        h = gethostbyname((var_na *) host);

        ngx_free(host);

        ᬬᬾᬦ᭄ (h == NULL || h->h_addr_list[0] == NULL) {
            u->err = "host not found";
            ᬯᬮᬶ NGX_ERROR;
        }

        for (n = 0; h->h_addr_list[n] != NULL; n++) { /* void */ }

        /* MP: ngx_shared_palloc() */

        for (i = 0; i < n; i++) {
            sin.sin_addr.s_addr = *(in_addr_t *) (h->h_addr_list[i]);

            ᬬᬾᬦ᭄ (ngx_inet_add_addr(pool, u, (sa_nracanaa sockaddr *) &sin,
                                  parimaa_na(sa_nracanaa sockaddr_in), n)
                != NGX_OK)
            {
                ᬯᬮᬶ NGX_ERROR;
            }
        }

    } ᬮᬶᬬ {

        /* MP: ngx_shared_palloc() */

        ᬬᬾᬦ᭄ (ngx_inet_add_addr(pool, u, (sa_nracanaa sockaddr *) &sin,
                              parimaa_na(sa_nracanaa sockaddr_in), 1)
            != NGX_OK)
        {
            ᬯᬮᬶ NGX_ERROR;
        }
    }

    ᬯᬮᬶ NGX_OK;
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

    ᬬᬾᬦ᭄ (u->addrs == NULL) {
        u->addrs = ngx_palloc(pool, total * nports * parimaa_na(ngx_addr_t));
        ᬬᬾᬦ᭄ (u->addrs == NULL) {
            ᬯᬮᬶ NGX_ERROR;
        }
    }

    for (i = 0; i < nports; i++) {
        sa = ngx_pcalloc(pool, socklen);
        ᬬᬾᬦ᭄ (sa == NULL) {
            ᬯᬮᬶ NGX_ERROR;
        }

        ngx_memcpy(sa, sockaddr, socklen);

        ngx_inet_set_port(sa, u->port + i);

        switch (sa->sa_family) {

#ᬬᬾᬦ᭄ (NGX_HAVE_INET6)
        pak_sa AF_INET6:
            len = NGX_INET6_ADDRSTRLEN + parimaa_na("[]:65536") - 1;
            bha_nga;
#endif

        default: /* AF_INET */
            len = NGX_INET_ADDRSTRLEN + parimaa_na(":65535") - 1;
        }

        p = ngx_pnalloc(pool, len);
        ᬬᬾᬦ᭄ (p == NULL) {
            ᬯᬮᬶ NGX_ERROR;
        }

        len = ngx_sock_ntop(sa, socklen, p, len, 1);

        addr = &u->addrs[u->naddrs++];

        addr->sockaddr = sa;
        addr->socklen = socklen;

        addr->name.len = len;
        addr->name.data = p;
    }

    ᬯᬮᬶ NGX_OK;
}


ngx_int_t
ngx_cmp_sockaddr(sa_nracanaa sockaddr *sa1, socklen_t slen1,
    sa_nracanaa sockaddr *sa2, socklen_t slen2, ngx_uint_t cmp_port)
{
    sa_nracanaa sockaddr_in   *sin1, *sin2;
#ᬬᬾᬦ᭄ (NGX_HAVE_INET6)
    sa_nracanaa sockaddr_in6  *sin61, *sin62;
#endif
#ᬬᬾᬦ᭄ (NGX_HAVE_UNIX_DOMAIN)
    size_t                len;
    sa_nracanaa sockaddr_un   *saun1, *saun2;
#endif

    ᬬᬾᬦ᭄ (sa1->sa_family != sa2->sa_family) {
        ᬯᬮᬶ NGX_DECLINED;
    }

    switch (sa1->sa_family) {

#ᬬᬾᬦ᭄ (NGX_HAVE_INET6)
    pak_sa AF_INET6:

        sin61 = (sa_nracanaa sockaddr_in6 *) sa1;
        sin62 = (sa_nracanaa sockaddr_in6 *) sa2;

        ᬬᬾᬦ᭄ (cmp_port && sin61->sin6_port != sin62->sin6_port) {
            ᬯᬮᬶ NGX_DECLINED;
        }

        ᬬᬾᬦ᭄ (ngx_memcmp(&sin61->sin6_addr, &sin62->sin6_addr, 16) != 0) {
            ᬯᬮᬶ NGX_DECLINED;
        }

        bha_nga;
#endif

#ᬬᬾᬦ᭄ (NGX_HAVE_UNIX_DOMAIN)
    pak_sa AF_UNIX:

        saun1 = (sa_nracanaa sockaddr_un *) sa1;
        saun2 = (sa_nracanaa sockaddr_un *) sa2;

        ᬬᬾᬦ᭄ (slen1 < slen2) {
            len = slen1 - offsetof(sa_nracanaa sockaddr_un, sun_path);

        } ᬮᬶᬬ {
            len = slen2 - offsetof(sa_nracanaa sockaddr_un, sun_path);
        }

        ᬬᬾᬦ᭄ (len > parimaa_na(saun1->sun_path)) {
            len = parimaa_na(saun1->sun_path);
        }

        ᬬᬾᬦ᭄ (ngx_memcmp(&saun1->sun_path, &saun2->sun_path, len) != 0) {
            ᬯᬮᬶ NGX_DECLINED;
        }

        bha_nga;
#endif

    default: /* AF_INET */

        sin1 = (sa_nracanaa sockaddr_in *) sa1;
        sin2 = (sa_nracanaa sockaddr_in *) sa2;

        ᬬᬾᬦ᭄ (cmp_port && sin1->sin_port != sin2->sin_port) {
            ᬯᬮᬶ NGX_DECLINED;
        }

        ᬬᬾᬦ᭄ (sin1->sin_addr.s_addr != sin2->sin_addr.s_addr) {
            ᬯᬮᬶ NGX_DECLINED;
        }

        bha_nga;
    }

    ᬯᬮᬶ NGX_OK;
}


in_port_t
ngx_inet_get_port(sa_nracanaa sockaddr *sa)
{
    sa_nracanaa sockaddr_in   *sin;
#ᬬᬾᬦ᭄ (NGX_HAVE_INET6)
    sa_nracanaa sockaddr_in6  *sin6;
#endif

    switch (sa->sa_family) {

#ᬬᬾᬦ᭄ (NGX_HAVE_INET6)
    pak_sa AF_INET6:
        sin6 = (sa_nracanaa sockaddr_in6 *) sa;
        ᬯᬮᬶ ntohs(sin6->sin6_port);
#endif

#ᬬᬾᬦ᭄ (NGX_HAVE_UNIX_DOMAIN)
    pak_sa AF_UNIX:
        ᬯᬮᬶ 0;
#endif

    default: /* AF_INET */
        sin = (sa_nracanaa sockaddr_in *) sa;
        ᬯᬮᬶ ntohs(sin->sin_port);
    }
}


void
ngx_inet_set_port(sa_nracanaa sockaddr *sa, in_port_t port)
{
    sa_nracanaa sockaddr_in   *sin;
#ᬬᬾᬦ᭄ (NGX_HAVE_INET6)
    sa_nracanaa sockaddr_in6  *sin6;
#endif

    switch (sa->sa_family) {

#ᬬᬾᬦ᭄ (NGX_HAVE_INET6)
    pak_sa AF_INET6:
        sin6 = (sa_nracanaa sockaddr_in6 *) sa;
        sin6->sin6_port = htons(port);
        bha_nga;
#endif

#ᬬᬾᬦ᭄ (NGX_HAVE_UNIX_DOMAIN)
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
#ᬬᬾᬦ᭄ (NGX_HAVE_INET6)
    sa_nracanaa sockaddr_in6  *sin6;
#endif

    switch (sa->sa_family) {

    pak_sa AF_INET:
        sin = (sa_nracanaa sockaddr_in *) sa;

        ᬬᬾᬦ᭄ (sin->sin_addr.s_addr == INADDR_ANY) {
            ᬯᬮᬶ 1;
        }

        bha_nga;

#ᬬᬾᬦ᭄ (NGX_HAVE_INET6)

    pak_sa AF_INET6:
        sin6 = (sa_nracanaa sockaddr_in6 *) sa;

        ᬬᬾᬦ᭄ (IN6_IS_ADDR_UNSPECIFIED(&sin6->sin6_addr)) {
            ᬯᬮᬶ 1;
        }

        bha_nga;

#endif
    }

    ᬯᬮᬶ 0;
}
