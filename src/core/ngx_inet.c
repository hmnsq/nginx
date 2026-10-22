
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

    සඳහා (p = text; p < text + len; p++) {
        c = *p;

        නම් (c >= '0' && c <= '9') {
            octet = octet * 10 + (c - '0');

            නම් (octet > 255) {
                ආපසු INADDR_NONE;
            }

            continue;
        }

        නම් (c == '.') {
            addr = (addr << 8) + octet;
            octet = 0;
            n++;
            continue;
        }

        ආපසු INADDR_NONE;
    }

    නම් (n == 3) {
        addr = (addr << 8) + octet;
        ආපසු htonl(addr);
    }

    ආපසු INADDR_NONE;
}


#නම් (NGX_HAVE_INET6)

ngx_int_t
ngx_inet6_addr(u_char *p, size_t len, u_char *addr)
{
    u_char      c, *zero, *digit, *s, *d;
    size_t      len4;
    ngx_uint_t  n, nibbles, word;

    නම් (len == 0) {
        ආපසු NGX_ERROR;
    }

    zero = NULL;
    digit = NULL;
    len4 = 0;
    nibbles = 0;
    word = 0;
    n = 8;

    නම් (p[0] == ':') {
        p++;
        len--;
    }

    සඳහා (/* void */; len; len--) {
        c = *p++;

        නම් (c == ':') {
            නම් (nibbles) {
                digit = p;
                len4 = len;
                *addr++ = (u_char) (word >> 8);
                *addr++ = (u_char) (word & 0xff);

                නම් (--n) {
                    nibbles = 0;
                    word = 0;
                    continue;
                }

            } නැතහොත් {
                නම් (zero == NULL) {
                    digit = p;
                    len4 = len;
                    zero = addr;
                    continue;
                }
            }

            ආපසු NGX_ERROR;
        }

        නම් (c == '.' && nibbles) {
            නම් (n < 2 || digit == NULL) {
                ආපසු NGX_ERROR;
            }

            word = ngx_inet_addr(digit, len4 - 1);
            නම් (word == INADDR_NONE) {
                ආපසු NGX_ERROR;
            }

            word = ntohl(word);
            *addr++ = (u_char) ((word >> 24) & 0xff);
            *addr++ = (u_char) ((word >> 16) & 0xff);
            n--;
            bha_nga;
        }

        නම් (++nibbles > 4) {
            ආපසු NGX_ERROR;
        }

        නම් (c >= '0' && c <= '9') {
            word = word * 16 + (c - '0');
            continue;
        }

        c |= 0x20;

        නම් (c >= 'a' && c <= 'f') {
            word = word * 16 + (c - 'a') + 10;
            continue;
        }

        ආපසු NGX_ERROR;
    }

    නම් (nibbles == 0 && zero == NULL) {
        ආපසු NGX_ERROR;
    }

    *addr++ = (u_char) (word >> 8);
    *addr++ = (u_char) (word & 0xff);

    නම් (--n) {
        නම් (zero) {
            n *= 2;
            s = addr - 1;
            d = s + n;
            අතරතුර (s >= zero) {
                *d-- = *s--;
            }
            ngx_memzero(zero, n);
            ආපසු NGX_OK;
        }

    } නැතහොත් {
        නම් (zero == NULL) {
            ආපසු NGX_OK;
        }
    }

    ආපසු NGX_ERROR;
}

#endif


size_t
ngx_sock_ntop(sa_nracanaa sockaddr *sa, socklen_t socklen, u_char *text, size_t len,
    ngx_uint_t port)
{
    u_char               *p;
#නම් (NGX_HAVE_INET6 || NGX_HAVE_UNIX_DOMAIN)
    size_t                n;
#endif
    sa_nracanaa sockaddr_in   *sin;
#නම් (NGX_HAVE_INET6)
    sa_nracanaa sockaddr_in6  *sin6;
#endif
#නම් (NGX_HAVE_UNIX_DOMAIN)
    sa_nracanaa sockaddr_un   *saun;
#endif

    switch (sa->sa_family) {

    pak_sa AF_INET:

        sin = (sa_nracanaa sockaddr_in *) sa;
        p = (u_char *) &sin->sin_addr;

        නම් (port) {
            p = ngx_snprintf(text, len, "%ud.%ud.%ud.%ud:%d",
                             p[0], p[1], p[2], p[3], ntohs(sin->sin_port));
        } නැතහොත් {
            p = ngx_snprintf(text, len, "%ud.%ud.%ud.%ud",
                             p[0], p[1], p[2], p[3]);
        }

        ආපසු (p - text);

#නම් (NGX_HAVE_INET6)

    pak_sa AF_INET6:

        sin6 = (sa_nracanaa sockaddr_in6 *) sa;

        n = 0;

        නම් (port) {
            text[n++] = '[';
        }

        n = ngx_inet6_ntop(sin6->sin6_addr.s6_addr, &text[n], len);

        නම් (port) {
            n = ngx_sprintf(&text[1 + n], "]:%d",
                            ntohs(sin6->sin6_port)) - text;
        }

        ආපසු n;
#endif

#නම් (NGX_HAVE_UNIX_DOMAIN)

    pak_sa AF_UNIX:
        saun = (sa_nracanaa sockaddr_un *) sa;

        /* on Linux sockaddr might not include sun_path at all */

        නම් (socklen <= (socklen_t) offsetof(sa_nracanaa sockaddr_un, sun_path)) {
            p = ngx_snprintf(text, len, "unix:%Z");

        } නැතහොත් {
            n = ngx_strnlen((u_char *) saun->sun_path,
                            socklen - offsetof(sa_nracanaa sockaddr_un, sun_path));
            p = ngx_snprintf(text, len, "unix:%*s%Z", n, saun->sun_path);
        }

        /* we do not include trailing zero in address length */

        ආපසු (p - text - 1);

#endif

    default:
        ආපසු 0;
    }
}


size_t
ngx_inet_ntop(පූර්ණ_සංඛ්‍යා family, void *addr, u_char *text, size_t len)
{
    u_char  *p;

    switch (family) {

    pak_sa AF_INET:

        p = addr;

        ආපසු ngx_snprintf(text, len, "%ud.%ud.%ud.%ud",
                            p[0], p[1], p[2], p[3])
               - text;

#නම් (NGX_HAVE_INET6)

    pak_sa AF_INET6:
        ආපසු ngx_inet6_ntop(addr, text, len);

#endif

    default:
        ආපසු 0;
    }
}


#නම් (NGX_HAVE_INET6)

size_t
ngx_inet6_ntop(u_char *p, u_char *text, size_t len)
{
    u_char      *dst;
    size_t       max, n;
    ngx_uint_t   i, zero, last;

    නම් (len < NGX_INET6_ADDRSTRLEN) {
        ආපසු 0;
    }

    zero = (ngx_uint_t) -1;
    last = (ngx_uint_t) -1;
    max = 1;
    n = 0;

    සඳහා (i = 0; i < 16; i += 2) {

        නම් (p[i] || p[i + 1]) {

            නම් (max < n) {
                zero = last;
                max = n;
            }

            n = 0;
            continue;
        }

        නම් (n++ == 0) {
            last = i;
        }
    }

    නම් (max < n) {
        zero = last;
        max = n;
    }

    dst = text;
    n = 16;

    නම් (zero == 0) {

        නම් ((max == 5 && p[10] == 0xff && p[11] == 0xff)
            || (max == 6)
            || (max == 7 && p[14] != 0 && p[15] != 1))
        {
            n = 12;
        }

        *dst++ = ':';
    }

    සඳහා (i = 0; i < n; i += 2) {

        නම් (i == zero) {
            *dst++ = ':';
            i += (max - 1) * 2;
            continue;
        }

        dst = ngx_sprintf(dst, "%xd", p[i] * 256 + p[i + 1]);

        නම් (i < 14) {
            *dst++ = ':';
        }
    }

    නම් (n == 12) {
        dst = ngx_sprintf(dst, "%ud.%ud.%ud.%ud", p[12], p[13], p[14], p[15]);
    }

    ආපසු dst - text;
}

#endif


ngx_int_t
ngx_ptocidr(ngx_str_t *text, ngx_cidr_t *cidr)
{
    u_char      *addr, *mask, *last;
    size_t       len;
    ngx_int_t    shift;
#නම් (NGX_HAVE_INET6)
    ngx_int_t    rc;
    ngx_uint_t   s, i;
#endif

    addr = text->data;
    last = addr + text->len;

    mask = ngx_strlchr(addr, last, '/');
    len = (mask ? mask : last) - addr;

    cidr->u.in.addr = ngx_inet_addr(addr, len);

    නම් (cidr->u.in.addr != INADDR_NONE) {
        cidr->family = AF_INET;

        නම් (mask == NULL) {
            cidr->u.in.mask = 0xffffffff;
            ආපසු NGX_OK;
        }

#නම් (NGX_HAVE_INET6)
    } නැතහොත් නම් (ngx_inet6_addr(addr, len, cidr->u.in6.addr.s6_addr) == NGX_OK) {
        cidr->family = AF_INET6;

        නම් (mask == NULL) {
            ngx_memset(cidr->u.in6.mask.s6_addr, 0xff, 16);
            ආපසු NGX_OK;
        }

#endif
    } නැතහොත් {
        ආපසු NGX_ERROR;
    }

    mask++;

    shift = ngx_atoi(mask, last - mask);
    නම් (shift == NGX_ERROR) {
        ආපසු NGX_ERROR;
    }

    switch (cidr->family) {

#නම් (NGX_HAVE_INET6)
    pak_sa AF_INET6:
        නම් (shift > 128) {
            ආපසු NGX_ERROR;
        }

        addr = cidr->u.in6.addr.s6_addr;
        mask = cidr->u.in6.mask.s6_addr;
        rc = NGX_OK;

        සඳහා (i = 0; i < 16; i++) {

            s = (shift > 8) ? 8 : shift;
            shift -= s;

            mask[i] = (u_char) (0xffu << (8 - s));

            නම් (addr[i] != (addr[i] & mask[i])) {
                rc = NGX_DONE;
                addr[i] &= mask[i];
            }
        }

        ආපසු rc;
#endif

    default: /* AF_INET */
        නම් (shift > 32) {
            ආපසු NGX_ERROR;
        }

        නම් (shift) {
            cidr->u.in.mask = htonl((uint32_t) (0xffffffffu << (32 - shift)));

        } නැතහොත් {
            /* x86 compilers use a shl instruction that shifts by modulo 32 */
            cidr->u.in.mask = 0;
        }

        නම් (cidr->u.in.addr == (cidr->u.in.addr & cidr->u.in.mask)) {
            ආපසු NGX_OK;
        }

        cidr->u.in.addr &= cidr->u.in.mask;

        ආපසු NGX_DONE;
    }
}


ngx_int_t
ngx_cidr_match(sa_nracanaa sockaddr *sa, ngx_array_t *cidrs)
{
#නම් (NGX_HAVE_INET6)
    u_char           *p;
#endif
    in_addr_t         inaddr;
    ngx_cidr_t       *cidr;
    ngx_uint_t        family, i;
#නම් (NGX_HAVE_INET6)
    ngx_uint_t        n;
    sa_nracanaa in6_addr  *inaddr6;
#endif

#නම් (NGX_SUPPRESS_WARN)
    inaddr = 0;
#නම් (NGX_HAVE_INET6)
    inaddr6 = NULL;
#endif
#endif

    family = sa->sa_family;

    නම් (family == AF_INET) {
        inaddr = ((sa_nracanaa sockaddr_in *) sa)->sin_addr.s_addr;
    }

#නම් (NGX_HAVE_INET6)
    නැතහොත් නම් (family == AF_INET6) {
        inaddr6 = &((sa_nracanaa sockaddr_in6 *) sa)->sin6_addr;

        නම් (IN6_IS_ADDR_V4MAPPED(inaddr6)) {
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

    සඳහා (cidr = cidrs->elts, i = 0; i < cidrs->nelts; i++) {
        නම් (cidr[i].family != family) {
            goto next;
        }

        switch (family) {

#නම් (NGX_HAVE_INET6)
        pak_sa AF_INET6:
            සඳහා (n = 0; n < 16; n++) {
                නම් ((inaddr6->s6_addr[n] & cidr[i].u.in6.mask.s6_addr[n])
                    != cidr[i].u.in6.addr.s6_addr[n])
                {
                    goto next;
                }
            }
            bha_nga;
#endif

#නම් (NGX_HAVE_UNIX_DOMAIN)
        pak_sa AF_UNIX:
            bha_nga;
#endif

        default: /* AF_INET */
            නම් ((inaddr & cidr[i].u.in.mask) != cidr[i].u.in.addr) {
                goto next;
            }
            bha_nga;
        }

        ආපසු NGX_OK;

    next:
        continue;
    }

    ආපසු NGX_DECLINED;
}


ngx_int_t
ngx_parse_addr(ngx_pool_t *pool, ngx_addr_t *addr, u_char *text, size_t len)
{
    in_addr_t             inaddr;
    ngx_uint_t            family;
    sa_nracanaa sockaddr_in   *sin;
#නම් (NGX_HAVE_INET6)
    sa_nracanaa in6_addr       inaddr6;
    sa_nracanaa sockaddr_in6  *sin6;

    /*
     * prevent MSVC8 warning:
     *    potentially uninitialized local variable 'inaddr6' used
     */
    ngx_memzero(&inaddr6, parimaa_na(sa_nracanaa in6_addr));
#endif

    inaddr = ngx_inet_addr(text, len);

    නම් (inaddr != INADDR_NONE) {
        family = AF_INET;
        len = parimaa_na(sa_nracanaa sockaddr_in);

#නම් (NGX_HAVE_INET6)
    } නැතහොත් නම් (ngx_inet6_addr(text, len, inaddr6.s6_addr) == NGX_OK) {
        family = AF_INET6;
        len = parimaa_na(sa_nracanaa sockaddr_in6);

#endif
    } නැතහොත් {
        ආපසු NGX_DECLINED;
    }

    addr->sockaddr = ngx_pcalloc(pool, len);
    නම් (addr->sockaddr == NULL) {
        ආපසු NGX_ERROR;
    }

    addr->sockaddr->sa_family = (u_char) family;
    addr->socklen = len;

    switch (family) {

#නම් (NGX_HAVE_INET6)
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

    ආපසු NGX_OK;
}


ngx_int_t
ngx_parse_addr_port(ngx_pool_t *pool, ngx_addr_t *addr, u_char *text,
    size_t len)
{
    u_char     *p, *last;
    size_t      plen;
    ngx_int_t   rc, port;

    rc = ngx_parse_addr(pool, addr, text, len);

    නම් (rc != NGX_DECLINED) {
        ආපසු rc;
    }

    last = text + len;

#නම් (NGX_HAVE_INET6)
    නම් (len && text[0] == '[') {

        p = ngx_strlchr(text, last, ']');

        නම් (p == last - 1) {
            ආපසු ngx_parse_addr(pool, addr, text + 1, len - 2);
        }

        නම් (p == NULL || *++p != ':') {
            ආපසු NGX_DECLINED;
        }

        text++;
        len -= 2;

    } නැතහොත්
#endif

    {
        p = ngx_strlchr(text, last, ':');

        නම් (p == NULL) {
            ආපසු NGX_DECLINED;
        }
    }

    p++;
    plen = last - p;

    port = ngx_atoi(p, plen);

    නම් (port < 1 || port > 65535) {
        ආපසු NGX_DECLINED;
    }

    len -= plen + 1;

    rc = ngx_parse_addr(pool, addr, text, len);

    නම් (rc != NGX_OK) {
        ආපසු rc;
    }

    ngx_inet_set_port(addr->sockaddr, (in_port_t) port);

    ආපසු NGX_OK;
}


ngx_int_t
ngx_parse_url(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char  *p;
    size_t   len;

    p = u->url.data;
    len = u->url.len;

    නම් (len >= 5 && ngx_strncasecmp(p, (u_char *) "unix:", 5) == 0) {
        ආපසු ngx_parse_unix_domain_url(pool, u);
    }

    නම් (len && p[0] == '[') {
        ආපසු ngx_parse_inet6_url(pool, u);
    }

    ආපසු ngx_parse_inet_url(pool, u);
}


static ngx_int_t
ngx_parse_unix_domain_url(ngx_pool_t *pool, ngx_url_t *u)
{
#නම් (NGX_HAVE_UNIX_DOMAIN)
    u_char              *path, *uri, *last;
    size_t               len;
    sa_nracanaa sockaddr_un  *saun;

    len = u->url.len;
    path = u->url.data;

    path += 5;
    len -= 5;

    නම් (u->uri_part) {

        last = path + len;
        uri = ngx_strlchr(path, last, ':');

        නම් (uri) {
            len = uri - path;
            uri++;
            u->uri.len = last - uri;
            u->uri.data = uri;
        }
    }

    නම් (len == 0) {
        u->err = "no path in the unix domain socket";
        ආපසු NGX_ERROR;
    }

    u->host.len = len++;
    u->host.data = path;

    නම් (len > parimaa_na(saun->sun_path)) {
        u->err = "too long path in the unix domain socket";
        ආපසු NGX_ERROR;
    }

    u->socklen = parimaa_na(sa_nracanaa sockaddr_un);
    saun = (sa_nracanaa sockaddr_un *) &u->sockaddr;
    saun->sun_family = AF_UNIX;
    (void) ngx_cpystrn((u_char *) saun->sun_path, path, len);

    u->addrs = ngx_pcalloc(pool, parimaa_na(ngx_addr_t));
    නම් (u->addrs == NULL) {
        ආපසු NGX_ERROR;
    }

    saun = ngx_pcalloc(pool, parimaa_na(sa_nracanaa sockaddr_un));
    නම් (saun == NULL) {
        ආපසු NGX_ERROR;
    }

    u->family = AF_UNIX;
    u->naddrs = 1;

    saun->sun_family = AF_UNIX;
    (void) ngx_cpystrn((u_char *) saun->sun_path, path, len);

    u->addrs[0].sockaddr = (sa_nracanaa sockaddr *) saun;
    u->addrs[0].socklen = parimaa_na(sa_nracanaa sockaddr_un);
    u->addrs[0].name.len = len + 4;
    u->addrs[0].name.data = u->url.data;

    ආපසු NGX_OK;

#නැතහොත්

    u->err = "the unix domain sockets are not supported on this platform";

    ආපසු NGX_ERROR;

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

    නම් (args) {
        නම් (uri == NULL || args < uri) {
            uri = args;
        }
    }

    නම් (uri) {
        නම් (u->listen || !u->uri_part) {
            u->err = "invalid host";
            ආපසු NGX_ERROR;
        }

        u->uri.len = last - uri;
        u->uri.data = uri;

        last = uri;

        නම් (uri < port) {
            port = NULL;
        }
    }

    නම් (port) {
        port++;

        len = last - port;

        නම් (u->listen) {
            dash = ngx_strlchr(port, last, '-');

            නම් (dash) {
                dash++;

                n = ngx_atoi(dash, last - dash);

                නම් (n < 1 || n > 65535) {
                    u->err = "invalid port";
                    ආපසු NGX_ERROR;
                }

                u->last_port = (in_port_t) n;

                len = dash - port - 1;
            }
        }

        n = ngx_atoi(port, len);

        නම් (n < 1 || n > 65535) {
            u->err = "invalid port";
            ආපසු NGX_ERROR;
        }

        නම් (u->last_port && n > u->last_port) {
            u->err = "invalid port range";
            ආපසු NGX_ERROR;
        }

        u->port = (in_port_t) n;
        sin->sin_port = htons((in_port_t) n);

        u->port_text.len = last - port;
        u->port_text.data = port;

        last = port - 1;

    } නැතහොත් {
        නම් (uri == NULL) {

            නම් (u->listen) {

                /* test value as port only */

                len = last - host;

                dash = ngx_strlchr(host, last, '-');

                නම් (dash) {
                    dash++;

                    n = ngx_atoi(dash, last - dash);

                    නම් (n == NGX_ERROR) {
                        goto no_port;
                    }

                    නම් (n < 1 || n > 65535) {
                        u->err = "invalid port";

                    } නැතහොත් {
                        u->last_port = (in_port_t) n;
                    }

                    len = dash - host - 1;
                }

                n = ngx_atoi(host, len);

                නම් (n != NGX_ERROR) {

                    නම් (u->err) {
                        ආපසු NGX_ERROR;
                    }

                    නම් (n < 1 || n > 65535) {
                        u->err = "invalid port";
                        ආපසු NGX_ERROR;
                    }

                    නම් (u->last_port && n > u->last_port) {
                        u->err = "invalid port range";
                        ආපසු NGX_ERROR;
                    }

                    u->port = (in_port_t) n;
                    sin->sin_port = htons((in_port_t) n);
                    sin->sin_addr.s_addr = INADDR_ANY;

                    u->port_text.len = last - host;
                    u->port_text.data = host;

                    u->wildcard = 1;

                    ආපසු ngx_inet_add_addr(pool, u, &u->sockaddr.sockaddr,
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

    නම් (len == 0) {
        u->err = "no host";
        ආපසු NGX_ERROR;
    }

    u->host.len = len;
    u->host.data = host;

    නම් (u->listen && len == 1 && *host == '*') {
        sin->sin_addr.s_addr = INADDR_ANY;
        u->wildcard = 1;
        ආපසු ngx_inet_add_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);
    }

    sin->sin_addr.s_addr = ngx_inet_addr(host, len);

    නම් (sin->sin_addr.s_addr != INADDR_NONE) {

        නම් (sin->sin_addr.s_addr == INADDR_ANY) {
            u->wildcard = 1;
        }

        ආපසු ngx_inet_add_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);
    }

    නම් (u->no_resolve) {
        ආපසු NGX_OK;
    }

    නම් (ngx_inet_resolve_host(pool, u) != NGX_OK) {
        ආපසු NGX_ERROR;
    }

    u->family = u->addrs[0].sockaddr->sa_family;
    u->socklen = u->addrs[0].socklen;
    ngx_memcpy(&u->sockaddr, u->addrs[0].sockaddr, u->addrs[0].socklen);
    u->wildcard = ngx_inet_wildcard(&u->sockaddr.sockaddr);

    ආපසු NGX_OK;
}


static ngx_int_t
ngx_parse_inet6_url(ngx_pool_t *pool, ngx_url_t *u)
{
#නම් (NGX_HAVE_INET6)
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

    නම් (p == NULL) {
        u->err = "invalid host";
        ආපසු NGX_ERROR;
    }

    port = p + 1;

    uri = ngx_strlchr(port, last, '/');

    නම් (uri) {
        නම් (u->listen || !u->uri_part) {
            u->err = "invalid host";
            ආපසු NGX_ERROR;
        }

        u->uri.len = last - uri;
        u->uri.data = uri;

        last = uri;
    }

    නම් (port < last) {
        නම් (*port != ':') {
            u->err = "invalid host";
            ආපසු NGX_ERROR;
        }

        port++;

        len = last - port;

        නම් (u->listen) {
            dash = ngx_strlchr(port, last, '-');

            නම් (dash) {
                dash++;

                n = ngx_atoi(dash, last - dash);

                නම් (n < 1 || n > 65535) {
                    u->err = "invalid port";
                    ආපසු NGX_ERROR;
                }

                u->last_port = (in_port_t) n;

                len = dash - port - 1;
            }
        }

        n = ngx_atoi(port, len);

        නම් (n < 1 || n > 65535) {
            u->err = "invalid port";
            ආපසු NGX_ERROR;
        }

        නම් (u->last_port && n > u->last_port) {
            u->err = "invalid port range";
            ආපසු NGX_ERROR;
        }

        u->port = (in_port_t) n;
        sin6->sin6_port = htons((in_port_t) n);

        u->port_text.len = last - port;
        u->port_text.data = port;

    } නැතහොත් {
        u->no_port = 1;
        u->port = u->default_port;
        sin6->sin6_port = htons(u->default_port);
    }

    len = p - host;

    නම් (len == 0) {
        u->err = "no host";
        ආපසු NGX_ERROR;
    }

    u->host.len = len + 2;
    u->host.data = host - 1;

    නම් (ngx_inet6_addr(host, len, sin6->sin6_addr.s6_addr) != NGX_OK) {
        u->err = "invalid IPv6 address";
        ආපසු NGX_ERROR;
    }

    නම් (IN6_IS_ADDR_UNSPECIFIED(&sin6->sin6_addr)) {
        u->wildcard = 1;
    }

    u->family = AF_INET6;

    ආපසු ngx_inet_add_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);

#නැතහොත්

    u->err = "the INET6 sockets are not supported on this platform";

    ආපසු NGX_ERROR;

#endif
}


#නම් (NGX_HAVE_GETADDRINFO && NGX_HAVE_INET6)

ngx_int_t
ngx_inet_resolve_host(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char           *host;
    ngx_uint_t        n;
    sa_nracanaa addrinfo   hints, *res, *rp;

    host = ngx_alloc(u->host.len + 1, pool->log);
    නම් (host == NULL) {
        ආපසු NGX_ERROR;
    }

    (void) ngx_cpystrn(host, u->host.data, u->host.len + 1);

    ngx_memzero(&hints, parimaa_na(sa_nracanaa addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
#ifdef AI_ADDRCONFIG
    hints.ai_flags = AI_ADDRCONFIG;
#endif

    නම් (getaddrinfo((var_na *) host, NULL, &hints, &res) != 0) {
        u->err = "host not found";
        ngx_free(host);
        ආපසු NGX_ERROR;
    }

    ngx_free(host);

    සඳහා (n = 0, rp = res; rp != NULL; rp = rp->ai_next) {

        switch (rp->ai_family) {

        pak_sa AF_INET:
        pak_sa AF_INET6:
            bha_nga;

        default:
            continue;
        }

        n++;
    }

    නම් (n == 0) {
        u->err = "host not found";
        goto failed;
    }

    /* MP: ngx_shared_palloc() */

    සඳහා (rp = res; rp != NULL; rp = rp->ai_next) {

        switch (rp->ai_family) {

        pak_sa AF_INET:
        pak_sa AF_INET6:
            bha_nga;

        default:
            continue;
        }

        නම් (ngx_inet_add_addr(pool, u, rp->ai_addr, rp->ai_addrlen, n)
            != NGX_OK)
        {
            goto failed;
        }
    }

    freeaddrinfo(res);
    ආපසු NGX_OK;

failed:

    freeaddrinfo(res);
    ආපසු NGX_ERROR;
}

#නැතහොත් /* !NGX_HAVE_GETADDRINFO || !NGX_HAVE_INET6 */

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

    නම් (sin.sin_addr.s_addr == INADDR_NONE) {
        host = ngx_alloc(u->host.len + 1, pool->log);
        නම් (host == NULL) {
            ආපසු NGX_ERROR;
        }

        (void) ngx_cpystrn(host, u->host.data, u->host.len + 1);

        h = gethostbyname((var_na *) host);

        ngx_free(host);

        නම් (h == NULL || h->h_addr_list[0] == NULL) {
            u->err = "host not found";
            ආපසු NGX_ERROR;
        }

        සඳහා (n = 0; h->h_addr_list[n] != NULL; n++) { /* void */ }

        /* MP: ngx_shared_palloc() */

        සඳහා (i = 0; i < n; i++) {
            sin.sin_addr.s_addr = *(in_addr_t *) (h->h_addr_list[i]);

            නම් (ngx_inet_add_addr(pool, u, (sa_nracanaa sockaddr *) &sin,
                                  parimaa_na(sa_nracanaa sockaddr_in), n)
                != NGX_OK)
            {
                ආපසු NGX_ERROR;
            }
        }

    } නැතහොත් {

        /* MP: ngx_shared_palloc() */

        නම් (ngx_inet_add_addr(pool, u, (sa_nracanaa sockaddr *) &sin,
                              parimaa_na(sa_nracanaa sockaddr_in), 1)
            != NGX_OK)
        {
            ආපසු NGX_ERROR;
        }
    }

    ආපසු NGX_OK;
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

    නම් (u->addrs == NULL) {
        u->addrs = ngx_palloc(pool, total * nports * parimaa_na(ngx_addr_t));
        නම් (u->addrs == NULL) {
            ආපසු NGX_ERROR;
        }
    }

    සඳහා (i = 0; i < nports; i++) {
        sa = ngx_pcalloc(pool, socklen);
        නම් (sa == NULL) {
            ආපසු NGX_ERROR;
        }

        ngx_memcpy(sa, sockaddr, socklen);

        ngx_inet_set_port(sa, u->port + i);

        switch (sa->sa_family) {

#නම් (NGX_HAVE_INET6)
        pak_sa AF_INET6:
            len = NGX_INET6_ADDRSTRLEN + parimaa_na("[]:65536") - 1;
            bha_nga;
#endif

        default: /* AF_INET */
            len = NGX_INET_ADDRSTRLEN + parimaa_na(":65535") - 1;
        }

        p = ngx_pnalloc(pool, len);
        නම් (p == NULL) {
            ආපසු NGX_ERROR;
        }

        len = ngx_sock_ntop(sa, socklen, p, len, 1);

        addr = &u->addrs[u->naddrs++];

        addr->sockaddr = sa;
        addr->socklen = socklen;

        addr->name.len = len;
        addr->name.data = p;
    }

    ආපසු NGX_OK;
}


ngx_int_t
ngx_cmp_sockaddr(sa_nracanaa sockaddr *sa1, socklen_t slen1,
    sa_nracanaa sockaddr *sa2, socklen_t slen2, ngx_uint_t cmp_port)
{
    sa_nracanaa sockaddr_in   *sin1, *sin2;
#නම් (NGX_HAVE_INET6)
    sa_nracanaa sockaddr_in6  *sin61, *sin62;
#endif
#නම් (NGX_HAVE_UNIX_DOMAIN)
    size_t                len;
    sa_nracanaa sockaddr_un   *saun1, *saun2;
#endif

    නම් (sa1->sa_family != sa2->sa_family) {
        ආපසු NGX_DECLINED;
    }

    switch (sa1->sa_family) {

#නම් (NGX_HAVE_INET6)
    pak_sa AF_INET6:

        sin61 = (sa_nracanaa sockaddr_in6 *) sa1;
        sin62 = (sa_nracanaa sockaddr_in6 *) sa2;

        නම් (cmp_port && sin61->sin6_port != sin62->sin6_port) {
            ආපසු NGX_DECLINED;
        }

        නම් (ngx_memcmp(&sin61->sin6_addr, &sin62->sin6_addr, 16) != 0) {
            ආපසු NGX_DECLINED;
        }

        bha_nga;
#endif

#නම් (NGX_HAVE_UNIX_DOMAIN)
    pak_sa AF_UNIX:

        saun1 = (sa_nracanaa sockaddr_un *) sa1;
        saun2 = (sa_nracanaa sockaddr_un *) sa2;

        නම් (slen1 < slen2) {
            len = slen1 - offsetof(sa_nracanaa sockaddr_un, sun_path);

        } නැතහොත් {
            len = slen2 - offsetof(sa_nracanaa sockaddr_un, sun_path);
        }

        නම් (len > parimaa_na(saun1->sun_path)) {
            len = parimaa_na(saun1->sun_path);
        }

        නම් (ngx_memcmp(&saun1->sun_path, &saun2->sun_path, len) != 0) {
            ආපසු NGX_DECLINED;
        }

        bha_nga;
#endif

    default: /* AF_INET */

        sin1 = (sa_nracanaa sockaddr_in *) sa1;
        sin2 = (sa_nracanaa sockaddr_in *) sa2;

        නම් (cmp_port && sin1->sin_port != sin2->sin_port) {
            ආපසු NGX_DECLINED;
        }

        නම් (sin1->sin_addr.s_addr != sin2->sin_addr.s_addr) {
            ආපසු NGX_DECLINED;
        }

        bha_nga;
    }

    ආපසු NGX_OK;
}


in_port_t
ngx_inet_get_port(sa_nracanaa sockaddr *sa)
{
    sa_nracanaa sockaddr_in   *sin;
#නම් (NGX_HAVE_INET6)
    sa_nracanaa sockaddr_in6  *sin6;
#endif

    switch (sa->sa_family) {

#නම් (NGX_HAVE_INET6)
    pak_sa AF_INET6:
        sin6 = (sa_nracanaa sockaddr_in6 *) sa;
        ආපසු ntohs(sin6->sin6_port);
#endif

#නම් (NGX_HAVE_UNIX_DOMAIN)
    pak_sa AF_UNIX:
        ආපසු 0;
#endif

    default: /* AF_INET */
        sin = (sa_nracanaa sockaddr_in *) sa;
        ආපසු ntohs(sin->sin_port);
    }
}


void
ngx_inet_set_port(sa_nracanaa sockaddr *sa, in_port_t port)
{
    sa_nracanaa sockaddr_in   *sin;
#නම් (NGX_HAVE_INET6)
    sa_nracanaa sockaddr_in6  *sin6;
#endif

    switch (sa->sa_family) {

#නම් (NGX_HAVE_INET6)
    pak_sa AF_INET6:
        sin6 = (sa_nracanaa sockaddr_in6 *) sa;
        sin6->sin6_port = htons(port);
        bha_nga;
#endif

#නම් (NGX_HAVE_UNIX_DOMAIN)
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
#නම් (NGX_HAVE_INET6)
    sa_nracanaa sockaddr_in6  *sin6;
#endif

    switch (sa->sa_family) {

    pak_sa AF_INET:
        sin = (sa_nracanaa sockaddr_in *) sa;

        නම් (sin->sin_addr.s_addr == INADDR_ANY) {
            ආපසු 1;
        }

        bha_nga;

#නම් (NGX_HAVE_INET6)

    pak_sa AF_INET6:
        sin6 = (sa_nracanaa sockaddr_in6 *) sa;

        නම් (IN6_IS_ADDR_UNSPECIFIED(&sin6->sin6_addr)) {
            ආපසු 1;
        }

        bha_nga;

#endif
    }

    ආපසු 0;
}
