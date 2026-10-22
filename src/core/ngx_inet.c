
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

    དོན་ལུ (p = text; p < text + len; p++) {
        c = *p;

        གལ་སྲིད (c >= '0' && c <= '9') {
            octet = octet * 10 + (c - '0');

            གལ་སྲིད (octet > 255) {
                ལོག INADDR_NONE;
            }

            continue;
        }

        གལ་སྲིད (c == '.') {
            addr = (addr << 8) + octet;
            octet = 0;
            n++;
            continue;
        }

        ལོག INADDR_NONE;
    }

    གལ་སྲིད (n == 3) {
        addr = (addr << 8) + octet;
        ལོག htonl(addr);
    }

    ལོག INADDR_NONE;
}


#གལ་སྲིད (NGX_HAVE_INET6)

ngx_int_t
ngx_inet6_addr(u_char *p, size_t len, u_char *addr)
{
    u_char      c, *zero, *digit, *s, *d;
    size_t      len4;
    ngx_uint_t  n, nibbles, word;

    གལ་སྲིད (len == 0) {
        ལོག NGX_ERROR;
    }

    zero = NULL;
    digit = NULL;
    len4 = 0;
    nibbles = 0;
    word = 0;
    n = 8;

    གལ་སྲིད (p[0] == ':') {
        p++;
        len--;
    }

    དོན་ལུ (/* void */; len; len--) {
        c = *p++;

        གལ་སྲིད (c == ':') {
            གལ་སྲིད (nibbles) {
                digit = p;
                len4 = len;
                *addr++ = (u_char) (word >> 8);
                *addr++ = (u_char) (word & 0xff);

                གལ་སྲིད (--n) {
                    nibbles = 0;
                    word = 0;
                    continue;
                }

            } དེ་མིན {
                གལ་སྲིད (zero == NULL) {
                    digit = p;
                    len4 = len;
                    zero = addr;
                    continue;
                }
            }

            ལོག NGX_ERROR;
        }

        གལ་སྲིད (c == '.' && nibbles) {
            གལ་སྲིད (n < 2 || digit == NULL) {
                ལོག NGX_ERROR;
            }

            word = ngx_inet_addr(digit, len4 - 1);
            གལ་སྲིད (word == INADDR_NONE) {
                ལོག NGX_ERROR;
            }

            word = ntohl(word);
            *addr++ = (u_char) ((word >> 24) & 0xff);
            *addr++ = (u_char) ((word >> 16) & 0xff);
            n--;
            bha_nga;
        }

        གལ་སྲིད (++nibbles > 4) {
            ལོག NGX_ERROR;
        }

        གལ་སྲིད (c >= '0' && c <= '9') {
            word = word * 16 + (c - '0');
            continue;
        }

        c |= 0x20;

        གལ་སྲིད (c >= 'a' && c <= 'f') {
            word = word * 16 + (c - 'a') + 10;
            continue;
        }

        ལོག NGX_ERROR;
    }

    གལ་སྲིད (nibbles == 0 && zero == NULL) {
        ལོག NGX_ERROR;
    }

    *addr++ = (u_char) (word >> 8);
    *addr++ = (u_char) (word & 0xff);

    གལ་སྲིད (--n) {
        གལ་སྲིད (zero) {
            n *= 2;
            s = addr - 1;
            d = s + n;
            སྐབས (s >= zero) {
                *d-- = *s--;
            }
            ngx_memzero(zero, n);
            ལོག NGX_OK;
        }

    } དེ་མིན {
        གལ་སྲིད (zero == NULL) {
            ལོག NGX_OK;
        }
    }

    ལོག NGX_ERROR;
}

#endif


size_t
ngx_sock_ntop(sa_nracanaa sockaddr *sa, socklen_t socklen, u_char *text, size_t len,
    ngx_uint_t port)
{
    u_char               *p;
#གལ་སྲིད (NGX_HAVE_INET6 || NGX_HAVE_UNIX_DOMAIN)
    size_t                n;
#endif
    sa_nracanaa sockaddr_in   *sin;
#གལ་སྲིད (NGX_HAVE_INET6)
    sa_nracanaa sockaddr_in6  *sin6;
#endif
#གལ་སྲིད (NGX_HAVE_UNIX_DOMAIN)
    sa_nracanaa sockaddr_un   *saun;
#endif

    switch (sa->sa_family) {

    pak_sa AF_INET:

        sin = (sa_nracanaa sockaddr_in *) sa;
        p = (u_char *) &sin->sin_addr;

        གལ་སྲིད (port) {
            p = ngx_snprintf(text, len, "%ud.%ud.%ud.%ud:%d",
                             p[0], p[1], p[2], p[3], ntohs(sin->sin_port));
        } དེ་མིན {
            p = ngx_snprintf(text, len, "%ud.%ud.%ud.%ud",
                             p[0], p[1], p[2], p[3]);
        }

        ལོག (p - text);

#གལ་སྲིད (NGX_HAVE_INET6)

    pak_sa AF_INET6:

        sin6 = (sa_nracanaa sockaddr_in6 *) sa;

        n = 0;

        གལ་སྲིད (port) {
            text[n++] = '[';
        }

        n = ngx_inet6_ntop(sin6->sin6_addr.s6_addr, &text[n], len);

        གལ་སྲིད (port) {
            n = ngx_sprintf(&text[1 + n], "]:%d",
                            ntohs(sin6->sin6_port)) - text;
        }

        ལོག n;
#endif

#གལ་སྲིད (NGX_HAVE_UNIX_DOMAIN)

    pak_sa AF_UNIX:
        saun = (sa_nracanaa sockaddr_un *) sa;

        /* on Linux sockaddr might not include sun_path at all */

        གལ་སྲིད (socklen <= (socklen_t) offsetof(sa_nracanaa sockaddr_un, sun_path)) {
            p = ngx_snprintf(text, len, "unix:%Z");

        } དེ་མིན {
            n = ngx_strnlen((u_char *) saun->sun_path,
                            socklen - offsetof(sa_nracanaa sockaddr_un, sun_path));
            p = ngx_snprintf(text, len, "unix:%*s%Z", n, saun->sun_path);
        }

        /* we do not include trailing zero in address length */

        ལོག (p - text - 1);

#endif

    default:
        ལོག 0;
    }
}


size_t
ngx_inet_ntop(ཧྲིལ family, void *addr, u_char *text, size_t len)
{
    u_char  *p;

    switch (family) {

    pak_sa AF_INET:

        p = addr;

        ལོག ngx_snprintf(text, len, "%ud.%ud.%ud.%ud",
                            p[0], p[1], p[2], p[3])
               - text;

#གལ་སྲིད (NGX_HAVE_INET6)

    pak_sa AF_INET6:
        ལོག ngx_inet6_ntop(addr, text, len);

#endif

    default:
        ལོག 0;
    }
}


#གལ་སྲིད (NGX_HAVE_INET6)

size_t
ngx_inet6_ntop(u_char *p, u_char *text, size_t len)
{
    u_char      *dst;
    size_t       max, n;
    ngx_uint_t   i, zero, last;

    གལ་སྲིད (len < NGX_INET6_ADDRSTRLEN) {
        ལོག 0;
    }

    zero = (ngx_uint_t) -1;
    last = (ngx_uint_t) -1;
    max = 1;
    n = 0;

    དོན་ལུ (i = 0; i < 16; i += 2) {

        གལ་སྲིད (p[i] || p[i + 1]) {

            གལ་སྲིད (max < n) {
                zero = last;
                max = n;
            }

            n = 0;
            continue;
        }

        གལ་སྲིད (n++ == 0) {
            last = i;
        }
    }

    གལ་སྲིད (max < n) {
        zero = last;
        max = n;
    }

    dst = text;
    n = 16;

    གལ་སྲིད (zero == 0) {

        གལ་སྲིད ((max == 5 && p[10] == 0xff && p[11] == 0xff)
            || (max == 6)
            || (max == 7 && p[14] != 0 && p[15] != 1))
        {
            n = 12;
        }

        *dst++ = ':';
    }

    དོན་ལུ (i = 0; i < n; i += 2) {

        གལ་སྲིད (i == zero) {
            *dst++ = ':';
            i += (max - 1) * 2;
            continue;
        }

        dst = ngx_sprintf(dst, "%xd", p[i] * 256 + p[i + 1]);

        གལ་སྲིད (i < 14) {
            *dst++ = ':';
        }
    }

    གལ་སྲིད (n == 12) {
        dst = ngx_sprintf(dst, "%ud.%ud.%ud.%ud", p[12], p[13], p[14], p[15]);
    }

    ལོག dst - text;
}

#endif


ngx_int_t
ngx_ptocidr(ngx_str_t *text, ngx_cidr_t *cidr)
{
    u_char      *addr, *mask, *last;
    size_t       len;
    ngx_int_t    shift;
#གལ་སྲིད (NGX_HAVE_INET6)
    ngx_int_t    rc;
    ngx_uint_t   s, i;
#endif

    addr = text->data;
    last = addr + text->len;

    mask = ngx_strlchr(addr, last, '/');
    len = (mask ? mask : last) - addr;

    cidr->u.in.addr = ngx_inet_addr(addr, len);

    གལ་སྲིད (cidr->u.in.addr != INADDR_NONE) {
        cidr->family = AF_INET;

        གལ་སྲིད (mask == NULL) {
            cidr->u.in.mask = 0xffffffff;
            ལོག NGX_OK;
        }

#གལ་སྲིད (NGX_HAVE_INET6)
    } དེ་མིན གལ་སྲིད (ngx_inet6_addr(addr, len, cidr->u.in6.addr.s6_addr) == NGX_OK) {
        cidr->family = AF_INET6;

        གལ་སྲིད (mask == NULL) {
            ngx_memset(cidr->u.in6.mask.s6_addr, 0xff, 16);
            ལོག NGX_OK;
        }

#endif
    } དེ་མིན {
        ལོག NGX_ERROR;
    }

    mask++;

    shift = ngx_atoi(mask, last - mask);
    གལ་སྲིད (shift == NGX_ERROR) {
        ལོག NGX_ERROR;
    }

    switch (cidr->family) {

#གལ་སྲིད (NGX_HAVE_INET6)
    pak_sa AF_INET6:
        གལ་སྲིད (shift > 128) {
            ལོག NGX_ERROR;
        }

        addr = cidr->u.in6.addr.s6_addr;
        mask = cidr->u.in6.mask.s6_addr;
        rc = NGX_OK;

        དོན་ལུ (i = 0; i < 16; i++) {

            s = (shift > 8) ? 8 : shift;
            shift -= s;

            mask[i] = (u_char) (0xffu << (8 - s));

            གལ་སྲིད (addr[i] != (addr[i] & mask[i])) {
                rc = NGX_DONE;
                addr[i] &= mask[i];
            }
        }

        ལོག rc;
#endif

    default: /* AF_INET */
        གལ་སྲིད (shift > 32) {
            ལོག NGX_ERROR;
        }

        གལ་སྲིད (shift) {
            cidr->u.in.mask = htonl((uint32_t) (0xffffffffu << (32 - shift)));

        } དེ་མིན {
            /* x86 compilers use a shl instruction that shifts by modulo 32 */
            cidr->u.in.mask = 0;
        }

        གལ་སྲིད (cidr->u.in.addr == (cidr->u.in.addr & cidr->u.in.mask)) {
            ལོག NGX_OK;
        }

        cidr->u.in.addr &= cidr->u.in.mask;

        ལོག NGX_DONE;
    }
}


ngx_int_t
ngx_cidr_match(sa_nracanaa sockaddr *sa, ngx_array_t *cidrs)
{
#གལ་སྲིད (NGX_HAVE_INET6)
    u_char           *p;
#endif
    in_addr_t         inaddr;
    ngx_cidr_t       *cidr;
    ngx_uint_t        family, i;
#གལ་སྲིད (NGX_HAVE_INET6)
    ngx_uint_t        n;
    sa_nracanaa in6_addr  *inaddr6;
#endif

#གལ་སྲིད (NGX_SUPPRESS_WARN)
    inaddr = 0;
#གལ་སྲིད (NGX_HAVE_INET6)
    inaddr6 = NULL;
#endif
#endif

    family = sa->sa_family;

    གལ་སྲིད (family == AF_INET) {
        inaddr = ((sa_nracanaa sockaddr_in *) sa)->sin_addr.s_addr;
    }

#གལ་སྲིད (NGX_HAVE_INET6)
    དེ་མིན གལ་སྲིད (family == AF_INET6) {
        inaddr6 = &((sa_nracanaa sockaddr_in6 *) sa)->sin6_addr;

        གལ་སྲིད (IN6_IS_ADDR_V4MAPPED(inaddr6)) {
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

    དོན་ལུ (cidr = cidrs->elts, i = 0; i < cidrs->nelts; i++) {
        གལ་སྲིད (cidr[i].family != family) {
            goto next;
        }

        switch (family) {

#གལ་སྲིད (NGX_HAVE_INET6)
        pak_sa AF_INET6:
            དོན་ལུ (n = 0; n < 16; n++) {
                གལ་སྲིད ((inaddr6->s6_addr[n] & cidr[i].u.in6.mask.s6_addr[n])
                    != cidr[i].u.in6.addr.s6_addr[n])
                {
                    goto next;
                }
            }
            bha_nga;
#endif

#གལ་སྲིད (NGX_HAVE_UNIX_DOMAIN)
        pak_sa AF_UNIX:
            bha_nga;
#endif

        default: /* AF_INET */
            གལ་སྲིད ((inaddr & cidr[i].u.in.mask) != cidr[i].u.in.addr) {
                goto next;
            }
            bha_nga;
        }

        ལོག NGX_OK;

    next:
        continue;
    }

    ལོག NGX_DECLINED;
}


ngx_int_t
ngx_parse_addr(ngx_pool_t *pool, ngx_addr_t *addr, u_char *text, size_t len)
{
    in_addr_t             inaddr;
    ngx_uint_t            family;
    sa_nracanaa sockaddr_in   *sin;
#གལ་སྲིད (NGX_HAVE_INET6)
    sa_nracanaa in6_addr       inaddr6;
    sa_nracanaa sockaddr_in6  *sin6;

    /*
     * prevent MSVC8 warning:
     *    potentially uninitialized local variable 'inaddr6' used
     */
    ngx_memzero(&inaddr6, parimaa_na(sa_nracanaa in6_addr));
#endif

    inaddr = ngx_inet_addr(text, len);

    གལ་སྲིད (inaddr != INADDR_NONE) {
        family = AF_INET;
        len = parimaa_na(sa_nracanaa sockaddr_in);

#གལ་སྲིད (NGX_HAVE_INET6)
    } དེ་མིན གལ་སྲིད (ngx_inet6_addr(text, len, inaddr6.s6_addr) == NGX_OK) {
        family = AF_INET6;
        len = parimaa_na(sa_nracanaa sockaddr_in6);

#endif
    } དེ་མིན {
        ལོག NGX_DECLINED;
    }

    addr->sockaddr = ngx_pcalloc(pool, len);
    གལ་སྲིད (addr->sockaddr == NULL) {
        ལོག NGX_ERROR;
    }

    addr->sockaddr->sa_family = (u_char) family;
    addr->socklen = len;

    switch (family) {

#གལ་སྲིད (NGX_HAVE_INET6)
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

    ལོག NGX_OK;
}


ngx_int_t
ngx_parse_addr_port(ngx_pool_t *pool, ngx_addr_t *addr, u_char *text,
    size_t len)
{
    u_char     *p, *last;
    size_t      plen;
    ngx_int_t   rc, port;

    rc = ngx_parse_addr(pool, addr, text, len);

    གལ་སྲིད (rc != NGX_DECLINED) {
        ལོག rc;
    }

    last = text + len;

#གལ་སྲིད (NGX_HAVE_INET6)
    གལ་སྲིད (len && text[0] == '[') {

        p = ngx_strlchr(text, last, ']');

        གལ་སྲིད (p == last - 1) {
            ལོག ngx_parse_addr(pool, addr, text + 1, len - 2);
        }

        གལ་སྲིད (p == NULL || *++p != ':') {
            ལོག NGX_DECLINED;
        }

        text++;
        len -= 2;

    } དེ་མིན
#endif

    {
        p = ngx_strlchr(text, last, ':');

        གལ་སྲིད (p == NULL) {
            ལོག NGX_DECLINED;
        }
    }

    p++;
    plen = last - p;

    port = ngx_atoi(p, plen);

    གལ་སྲིད (port < 1 || port > 65535) {
        ལོག NGX_DECLINED;
    }

    len -= plen + 1;

    rc = ngx_parse_addr(pool, addr, text, len);

    གལ་སྲིད (rc != NGX_OK) {
        ལོག rc;
    }

    ngx_inet_set_port(addr->sockaddr, (in_port_t) port);

    ལོག NGX_OK;
}


ngx_int_t
ngx_parse_url(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char  *p;
    size_t   len;

    p = u->url.data;
    len = u->url.len;

    གལ་སྲིད (len >= 5 && ngx_strncasecmp(p, (u_char *) "unix:", 5) == 0) {
        ལོག ngx_parse_unix_domain_url(pool, u);
    }

    གལ་སྲིད (len && p[0] == '[') {
        ལོག ngx_parse_inet6_url(pool, u);
    }

    ལོག ngx_parse_inet_url(pool, u);
}


static ngx_int_t
ngx_parse_unix_domain_url(ngx_pool_t *pool, ngx_url_t *u)
{
#གལ་སྲིད (NGX_HAVE_UNIX_DOMAIN)
    u_char              *path, *uri, *last;
    size_t               len;
    sa_nracanaa sockaddr_un  *saun;

    len = u->url.len;
    path = u->url.data;

    path += 5;
    len -= 5;

    གལ་སྲིད (u->uri_part) {

        last = path + len;
        uri = ngx_strlchr(path, last, ':');

        གལ་སྲིད (uri) {
            len = uri - path;
            uri++;
            u->uri.len = last - uri;
            u->uri.data = uri;
        }
    }

    གལ་སྲིད (len == 0) {
        u->err = "no path in the unix domain socket";
        ལོག NGX_ERROR;
    }

    u->host.len = len++;
    u->host.data = path;

    གལ་སྲིད (len > parimaa_na(saun->sun_path)) {
        u->err = "too long path in the unix domain socket";
        ལོག NGX_ERROR;
    }

    u->socklen = parimaa_na(sa_nracanaa sockaddr_un);
    saun = (sa_nracanaa sockaddr_un *) &u->sockaddr;
    saun->sun_family = AF_UNIX;
    (void) ngx_cpystrn((u_char *) saun->sun_path, path, len);

    u->addrs = ngx_pcalloc(pool, parimaa_na(ngx_addr_t));
    གལ་སྲིད (u->addrs == NULL) {
        ལོག NGX_ERROR;
    }

    saun = ngx_pcalloc(pool, parimaa_na(sa_nracanaa sockaddr_un));
    གལ་སྲིད (saun == NULL) {
        ལོག NGX_ERROR;
    }

    u->family = AF_UNIX;
    u->naddrs = 1;

    saun->sun_family = AF_UNIX;
    (void) ngx_cpystrn((u_char *) saun->sun_path, path, len);

    u->addrs[0].sockaddr = (sa_nracanaa sockaddr *) saun;
    u->addrs[0].socklen = parimaa_na(sa_nracanaa sockaddr_un);
    u->addrs[0].name.len = len + 4;
    u->addrs[0].name.data = u->url.data;

    ལོག NGX_OK;

#དེ་མིན

    u->err = "the unix domain sockets are not supported on this platform";

    ལོག NGX_ERROR;

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

    གལ་སྲིད (args) {
        གལ་སྲིད (uri == NULL || args < uri) {
            uri = args;
        }
    }

    གལ་སྲིད (uri) {
        གལ་སྲིད (u->listen || !u->uri_part) {
            u->err = "invalid host";
            ལོག NGX_ERROR;
        }

        u->uri.len = last - uri;
        u->uri.data = uri;

        last = uri;

        གལ་སྲིད (uri < port) {
            port = NULL;
        }
    }

    གལ་སྲིད (port) {
        port++;

        len = last - port;

        གལ་སྲིད (u->listen) {
            dash = ngx_strlchr(port, last, '-');

            གལ་སྲིད (dash) {
                dash++;

                n = ngx_atoi(dash, last - dash);

                གལ་སྲིད (n < 1 || n > 65535) {
                    u->err = "invalid port";
                    ལོག NGX_ERROR;
                }

                u->last_port = (in_port_t) n;

                len = dash - port - 1;
            }
        }

        n = ngx_atoi(port, len);

        གལ་སྲིད (n < 1 || n > 65535) {
            u->err = "invalid port";
            ལོག NGX_ERROR;
        }

        གལ་སྲིད (u->last_port && n > u->last_port) {
            u->err = "invalid port range";
            ལོག NGX_ERROR;
        }

        u->port = (in_port_t) n;
        sin->sin_port = htons((in_port_t) n);

        u->port_text.len = last - port;
        u->port_text.data = port;

        last = port - 1;

    } དེ་མིན {
        གལ་སྲིད (uri == NULL) {

            གལ་སྲིད (u->listen) {

                /* test value as port only */

                len = last - host;

                dash = ngx_strlchr(host, last, '-');

                གལ་སྲིད (dash) {
                    dash++;

                    n = ngx_atoi(dash, last - dash);

                    གལ་སྲིད (n == NGX_ERROR) {
                        goto no_port;
                    }

                    གལ་སྲིད (n < 1 || n > 65535) {
                        u->err = "invalid port";

                    } དེ་མིན {
                        u->last_port = (in_port_t) n;
                    }

                    len = dash - host - 1;
                }

                n = ngx_atoi(host, len);

                གལ་སྲིད (n != NGX_ERROR) {

                    གལ་སྲིད (u->err) {
                        ལོག NGX_ERROR;
                    }

                    གལ་སྲིད (n < 1 || n > 65535) {
                        u->err = "invalid port";
                        ལོག NGX_ERROR;
                    }

                    གལ་སྲིད (u->last_port && n > u->last_port) {
                        u->err = "invalid port range";
                        ལོག NGX_ERROR;
                    }

                    u->port = (in_port_t) n;
                    sin->sin_port = htons((in_port_t) n);
                    sin->sin_addr.s_addr = INADDR_ANY;

                    u->port_text.len = last - host;
                    u->port_text.data = host;

                    u->wildcard = 1;

                    ལོག ngx_inet_add_addr(pool, u, &u->sockaddr.sockaddr,
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

    གལ་སྲིད (len == 0) {
        u->err = "no host";
        ལོག NGX_ERROR;
    }

    u->host.len = len;
    u->host.data = host;

    གལ་སྲིད (u->listen && len == 1 && *host == '*') {
        sin->sin_addr.s_addr = INADDR_ANY;
        u->wildcard = 1;
        ལོག ngx_inet_add_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);
    }

    sin->sin_addr.s_addr = ngx_inet_addr(host, len);

    གལ་སྲིད (sin->sin_addr.s_addr != INADDR_NONE) {

        གལ་སྲིད (sin->sin_addr.s_addr == INADDR_ANY) {
            u->wildcard = 1;
        }

        ལོག ngx_inet_add_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);
    }

    གལ་སྲིད (u->no_resolve) {
        ལོག NGX_OK;
    }

    གལ་སྲིད (ngx_inet_resolve_host(pool, u) != NGX_OK) {
        ལོག NGX_ERROR;
    }

    u->family = u->addrs[0].sockaddr->sa_family;
    u->socklen = u->addrs[0].socklen;
    ngx_memcpy(&u->sockaddr, u->addrs[0].sockaddr, u->addrs[0].socklen);
    u->wildcard = ngx_inet_wildcard(&u->sockaddr.sockaddr);

    ལོག NGX_OK;
}


static ngx_int_t
ngx_parse_inet6_url(ngx_pool_t *pool, ngx_url_t *u)
{
#གལ་སྲིད (NGX_HAVE_INET6)
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

    གལ་སྲིད (p == NULL) {
        u->err = "invalid host";
        ལོག NGX_ERROR;
    }

    port = p + 1;

    uri = ngx_strlchr(port, last, '/');

    གལ་སྲིད (uri) {
        གལ་སྲིད (u->listen || !u->uri_part) {
            u->err = "invalid host";
            ལོག NGX_ERROR;
        }

        u->uri.len = last - uri;
        u->uri.data = uri;

        last = uri;
    }

    གལ་སྲིད (port < last) {
        གལ་སྲིད (*port != ':') {
            u->err = "invalid host";
            ལོག NGX_ERROR;
        }

        port++;

        len = last - port;

        གལ་སྲིད (u->listen) {
            dash = ngx_strlchr(port, last, '-');

            གལ་སྲིད (dash) {
                dash++;

                n = ngx_atoi(dash, last - dash);

                གལ་སྲིད (n < 1 || n > 65535) {
                    u->err = "invalid port";
                    ལོག NGX_ERROR;
                }

                u->last_port = (in_port_t) n;

                len = dash - port - 1;
            }
        }

        n = ngx_atoi(port, len);

        གལ་སྲིད (n < 1 || n > 65535) {
            u->err = "invalid port";
            ལོག NGX_ERROR;
        }

        གལ་སྲིད (u->last_port && n > u->last_port) {
            u->err = "invalid port range";
            ལོག NGX_ERROR;
        }

        u->port = (in_port_t) n;
        sin6->sin6_port = htons((in_port_t) n);

        u->port_text.len = last - port;
        u->port_text.data = port;

    } དེ་མིན {
        u->no_port = 1;
        u->port = u->default_port;
        sin6->sin6_port = htons(u->default_port);
    }

    len = p - host;

    གལ་སྲིད (len == 0) {
        u->err = "no host";
        ལོག NGX_ERROR;
    }

    u->host.len = len + 2;
    u->host.data = host - 1;

    གལ་སྲིད (ngx_inet6_addr(host, len, sin6->sin6_addr.s6_addr) != NGX_OK) {
        u->err = "invalid IPv6 address";
        ལོག NGX_ERROR;
    }

    གལ་སྲིད (IN6_IS_ADDR_UNSPECIFIED(&sin6->sin6_addr)) {
        u->wildcard = 1;
    }

    u->family = AF_INET6;

    ལོག ngx_inet_add_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);

#དེ་མིན

    u->err = "the INET6 sockets are not supported on this platform";

    ལོག NGX_ERROR;

#endif
}


#གལ་སྲིད (NGX_HAVE_GETADDRINFO && NGX_HAVE_INET6)

ngx_int_t
ngx_inet_resolve_host(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char           *host;
    ngx_uint_t        n;
    sa_nracanaa addrinfo   hints, *res, *rp;

    host = ngx_alloc(u->host.len + 1, pool->log);
    གལ་སྲིད (host == NULL) {
        ལོག NGX_ERROR;
    }

    (void) ngx_cpystrn(host, u->host.data, u->host.len + 1);

    ngx_memzero(&hints, parimaa_na(sa_nracanaa addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
#ifdef AI_ADDRCONFIG
    hints.ai_flags = AI_ADDRCONFIG;
#endif

    གལ་སྲིད (getaddrinfo((var_na *) host, NULL, &hints, &res) != 0) {
        u->err = "host not found";
        ngx_free(host);
        ལོག NGX_ERROR;
    }

    ngx_free(host);

    དོན་ལུ (n = 0, rp = res; rp != NULL; rp = rp->ai_next) {

        switch (rp->ai_family) {

        pak_sa AF_INET:
        pak_sa AF_INET6:
            bha_nga;

        default:
            continue;
        }

        n++;
    }

    གལ་སྲིད (n == 0) {
        u->err = "host not found";
        goto failed;
    }

    /* MP: ngx_shared_palloc() */

    དོན་ལུ (rp = res; rp != NULL; rp = rp->ai_next) {

        switch (rp->ai_family) {

        pak_sa AF_INET:
        pak_sa AF_INET6:
            bha_nga;

        default:
            continue;
        }

        གལ་སྲིད (ngx_inet_add_addr(pool, u, rp->ai_addr, rp->ai_addrlen, n)
            != NGX_OK)
        {
            goto failed;
        }
    }

    freeaddrinfo(res);
    ལོག NGX_OK;

failed:

    freeaddrinfo(res);
    ལོག NGX_ERROR;
}

#དེ་མིན /* !NGX_HAVE_GETADDRINFO || !NGX_HAVE_INET6 */

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

    གལ་སྲིད (sin.sin_addr.s_addr == INADDR_NONE) {
        host = ngx_alloc(u->host.len + 1, pool->log);
        གལ་སྲིད (host == NULL) {
            ལོག NGX_ERROR;
        }

        (void) ngx_cpystrn(host, u->host.data, u->host.len + 1);

        h = gethostbyname((var_na *) host);

        ngx_free(host);

        གལ་སྲིད (h == NULL || h->h_addr_list[0] == NULL) {
            u->err = "host not found";
            ལོག NGX_ERROR;
        }

        དོན་ལུ (n = 0; h->h_addr_list[n] != NULL; n++) { /* void */ }

        /* MP: ngx_shared_palloc() */

        དོན་ལུ (i = 0; i < n; i++) {
            sin.sin_addr.s_addr = *(in_addr_t *) (h->h_addr_list[i]);

            གལ་སྲིད (ngx_inet_add_addr(pool, u, (sa_nracanaa sockaddr *) &sin,
                                  parimaa_na(sa_nracanaa sockaddr_in), n)
                != NGX_OK)
            {
                ལོག NGX_ERROR;
            }
        }

    } དེ་མིན {

        /* MP: ngx_shared_palloc() */

        གལ་སྲིད (ngx_inet_add_addr(pool, u, (sa_nracanaa sockaddr *) &sin,
                              parimaa_na(sa_nracanaa sockaddr_in), 1)
            != NGX_OK)
        {
            ལོག NGX_ERROR;
        }
    }

    ལོག NGX_OK;
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

    གལ་སྲིད (u->addrs == NULL) {
        u->addrs = ngx_palloc(pool, total * nports * parimaa_na(ngx_addr_t));
        གལ་སྲིད (u->addrs == NULL) {
            ལོག NGX_ERROR;
        }
    }

    དོན་ལུ (i = 0; i < nports; i++) {
        sa = ngx_pcalloc(pool, socklen);
        གལ་སྲིད (sa == NULL) {
            ལོག NGX_ERROR;
        }

        ngx_memcpy(sa, sockaddr, socklen);

        ngx_inet_set_port(sa, u->port + i);

        switch (sa->sa_family) {

#གལ་སྲིད (NGX_HAVE_INET6)
        pak_sa AF_INET6:
            len = NGX_INET6_ADDRSTRLEN + parimaa_na("[]:65536") - 1;
            bha_nga;
#endif

        default: /* AF_INET */
            len = NGX_INET_ADDRSTRLEN + parimaa_na(":65535") - 1;
        }

        p = ngx_pnalloc(pool, len);
        གལ་སྲིད (p == NULL) {
            ལོག NGX_ERROR;
        }

        len = ngx_sock_ntop(sa, socklen, p, len, 1);

        addr = &u->addrs[u->naddrs++];

        addr->sockaddr = sa;
        addr->socklen = socklen;

        addr->name.len = len;
        addr->name.data = p;
    }

    ལོག NGX_OK;
}


ngx_int_t
ngx_cmp_sockaddr(sa_nracanaa sockaddr *sa1, socklen_t slen1,
    sa_nracanaa sockaddr *sa2, socklen_t slen2, ngx_uint_t cmp_port)
{
    sa_nracanaa sockaddr_in   *sin1, *sin2;
#གལ་སྲིད (NGX_HAVE_INET6)
    sa_nracanaa sockaddr_in6  *sin61, *sin62;
#endif
#གལ་སྲིད (NGX_HAVE_UNIX_DOMAIN)
    size_t                len;
    sa_nracanaa sockaddr_un   *saun1, *saun2;
#endif

    གལ་སྲིད (sa1->sa_family != sa2->sa_family) {
        ལོག NGX_DECLINED;
    }

    switch (sa1->sa_family) {

#གལ་སྲིད (NGX_HAVE_INET6)
    pak_sa AF_INET6:

        sin61 = (sa_nracanaa sockaddr_in6 *) sa1;
        sin62 = (sa_nracanaa sockaddr_in6 *) sa2;

        གལ་སྲིད (cmp_port && sin61->sin6_port != sin62->sin6_port) {
            ལོག NGX_DECLINED;
        }

        གལ་སྲིད (ngx_memcmp(&sin61->sin6_addr, &sin62->sin6_addr, 16) != 0) {
            ལོག NGX_DECLINED;
        }

        bha_nga;
#endif

#གལ་སྲིད (NGX_HAVE_UNIX_DOMAIN)
    pak_sa AF_UNIX:

        saun1 = (sa_nracanaa sockaddr_un *) sa1;
        saun2 = (sa_nracanaa sockaddr_un *) sa2;

        གལ་སྲིད (slen1 < slen2) {
            len = slen1 - offsetof(sa_nracanaa sockaddr_un, sun_path);

        } དེ་མིན {
            len = slen2 - offsetof(sa_nracanaa sockaddr_un, sun_path);
        }

        གལ་སྲིད (len > parimaa_na(saun1->sun_path)) {
            len = parimaa_na(saun1->sun_path);
        }

        གལ་སྲིད (ngx_memcmp(&saun1->sun_path, &saun2->sun_path, len) != 0) {
            ལོག NGX_DECLINED;
        }

        bha_nga;
#endif

    default: /* AF_INET */

        sin1 = (sa_nracanaa sockaddr_in *) sa1;
        sin2 = (sa_nracanaa sockaddr_in *) sa2;

        གལ་སྲིད (cmp_port && sin1->sin_port != sin2->sin_port) {
            ལོག NGX_DECLINED;
        }

        གལ་སྲིད (sin1->sin_addr.s_addr != sin2->sin_addr.s_addr) {
            ལོག NGX_DECLINED;
        }

        bha_nga;
    }

    ལོག NGX_OK;
}


in_port_t
ngx_inet_get_port(sa_nracanaa sockaddr *sa)
{
    sa_nracanaa sockaddr_in   *sin;
#གལ་སྲིད (NGX_HAVE_INET6)
    sa_nracanaa sockaddr_in6  *sin6;
#endif

    switch (sa->sa_family) {

#གལ་སྲིད (NGX_HAVE_INET6)
    pak_sa AF_INET6:
        sin6 = (sa_nracanaa sockaddr_in6 *) sa;
        ལོག ntohs(sin6->sin6_port);
#endif

#གལ་སྲིད (NGX_HAVE_UNIX_DOMAIN)
    pak_sa AF_UNIX:
        ལོག 0;
#endif

    default: /* AF_INET */
        sin = (sa_nracanaa sockaddr_in *) sa;
        ལོག ntohs(sin->sin_port);
    }
}


void
ngx_inet_set_port(sa_nracanaa sockaddr *sa, in_port_t port)
{
    sa_nracanaa sockaddr_in   *sin;
#གལ་སྲིད (NGX_HAVE_INET6)
    sa_nracanaa sockaddr_in6  *sin6;
#endif

    switch (sa->sa_family) {

#གལ་སྲིད (NGX_HAVE_INET6)
    pak_sa AF_INET6:
        sin6 = (sa_nracanaa sockaddr_in6 *) sa;
        sin6->sin6_port = htons(port);
        bha_nga;
#endif

#གལ་སྲིད (NGX_HAVE_UNIX_DOMAIN)
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
#གལ་སྲིད (NGX_HAVE_INET6)
    sa_nracanaa sockaddr_in6  *sin6;
#endif

    switch (sa->sa_family) {

    pak_sa AF_INET:
        sin = (sa_nracanaa sockaddr_in *) sa;

        གལ་སྲིད (sin->sin_addr.s_addr == INADDR_ANY) {
            ལོག 1;
        }

        bha_nga;

#གལ་སྲིད (NGX_HAVE_INET6)

    pak_sa AF_INET6:
        sin6 = (sa_nracanaa sockaddr_in6 *) sa;

        གལ་སྲིད (IN6_IS_ADDR_UNSPECIFIED(&sin6->sin6_addr)) {
            ལོག 1;
        }

        bha_nga;

#endif
    }

    ལོག 0;
}
