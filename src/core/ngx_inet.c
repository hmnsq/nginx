
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
    struct sockaddr *sockaddr, socklen_t socklen, ngx_uint_t total);


in_addr_t
ngx_inet_addr(u_char *text, size_t len)
{
    u_char      *p, c;
    in_addr_t    addr;
    ngx_uint_t   octet, n;

    addr = 0;
    octet = 0;
    n = 0;

    위해 (p = text; p < text + len; p++) {
        c = *p;

        만약 (c >= '0' && c <= '9') {
            octet = octet * 10 + (c - '0');

            만약 (octet > 255) {
                반환 INADDR_NONE;
            }

            continue;
        }

        만약 (c == '.') {
            addr = (addr << 8) + octet;
            octet = 0;
            n++;
            continue;
        }

        반환 INADDR_NONE;
    }

    만약 (n == 3) {
        addr = (addr << 8) + octet;
        반환 htonl(addr);
    }

    반환 INADDR_NONE;
}


#만약 (NGX_HAVE_INET6)

ngx_int_t
ngx_inet6_addr(u_char *p, size_t len, u_char *addr)
{
    u_char      c, *zero, *digit, *s, *d;
    size_t      len4;
    ngx_uint_t  n, nibbles, word;

    만약 (len == 0) {
        반환 NGX_ERROR;
    }

    zero = NULL;
    digit = NULL;
    len4 = 0;
    nibbles = 0;
    word = 0;
    n = 8;

    만약 (p[0] == ':') {
        p++;
        len--;
    }

    위해 (/* void */; len; len--) {
        c = *p++;

        만약 (c == ':') {
            만약 (nibbles) {
                digit = p;
                len4 = len;
                *addr++ = (u_char) (word >> 8);
                *addr++ = (u_char) (word & 0xff);

                만약 (--n) {
                    nibbles = 0;
                    word = 0;
                    continue;
                }

            } 아니면 {
                만약 (zero == NULL) {
                    digit = p;
                    len4 = len;
                    zero = addr;
                    continue;
                }
            }

            반환 NGX_ERROR;
        }

        만약 (c == '.' && nibbles) {
            만약 (n < 2 || digit == NULL) {
                반환 NGX_ERROR;
            }

            word = ngx_inet_addr(digit, len4 - 1);
            만약 (word == INADDR_NONE) {
                반환 NGX_ERROR;
            }

            word = ntohl(word);
            *addr++ = (u_char) ((word >> 24) & 0xff);
            *addr++ = (u_char) ((word >> 16) & 0xff);
            n--;
            break;
        }

        만약 (++nibbles > 4) {
            반환 NGX_ERROR;
        }

        만약 (c >= '0' && c <= '9') {
            word = word * 16 + (c - '0');
            continue;
        }

        c |= 0x20;

        만약 (c >= 'a' && c <= 'f') {
            word = word * 16 + (c - 'a') + 10;
            continue;
        }

        반환 NGX_ERROR;
    }

    만약 (nibbles == 0 && zero == NULL) {
        반환 NGX_ERROR;
    }

    *addr++ = (u_char) (word >> 8);
    *addr++ = (u_char) (word & 0xff);

    만약 (--n) {
        만약 (zero) {
            n *= 2;
            s = addr - 1;
            d = s + n;
            동안 (s >= zero) {
                *d-- = *s--;
            }
            ngx_memzero(zero, n);
            반환 NGX_OK;
        }

    } 아니면 {
        만약 (zero == NULL) {
            반환 NGX_OK;
        }
    }

    반환 NGX_ERROR;
}

#endif


size_t
ngx_sock_ntop(struct sockaddr *sa, socklen_t socklen, u_char *text, size_t len,
    ngx_uint_t port)
{
    u_char               *p;
#만약 (NGX_HAVE_INET6 || NGX_HAVE_UNIX_DOMAIN)
    size_t                n;
#endif
    struct sockaddr_in   *sin;
#만약 (NGX_HAVE_INET6)
    struct sockaddr_in6  *sin6;
#endif
#만약 (NGX_HAVE_UNIX_DOMAIN)
    struct sockaddr_un   *saun;
#endif

    switch (sa->sa_family) {

    case AF_INET:

        sin = (struct sockaddr_in *) sa;
        p = (u_char *) &sin->sin_addr;

        만약 (port) {
            p = ngx_snprintf(text, len, "%ud.%ud.%ud.%ud:%d",
                             p[0], p[1], p[2], p[3], ntohs(sin->sin_port));
        } 아니면 {
            p = ngx_snprintf(text, len, "%ud.%ud.%ud.%ud",
                             p[0], p[1], p[2], p[3]);
        }

        반환 (p - text);

#만약 (NGX_HAVE_INET6)

    case AF_INET6:

        sin6 = (struct sockaddr_in6 *) sa;

        n = 0;

        만약 (port) {
            text[n++] = '[';
        }

        n = ngx_inet6_ntop(sin6->sin6_addr.s6_addr, &text[n], len);

        만약 (port) {
            n = ngx_sprintf(&text[1 + n], "]:%d",
                            ntohs(sin6->sin6_port)) - text;
        }

        반환 n;
#endif

#만약 (NGX_HAVE_UNIX_DOMAIN)

    case AF_UNIX:
        saun = (struct sockaddr_un *) sa;

        /* on Linux sockaddr might not include sun_path at all */

        만약 (socklen <= (socklen_t) offsetof(struct sockaddr_un, sun_path)) {
            p = ngx_snprintf(text, len, "unix:%Z");

        } 아니면 {
            n = ngx_strnlen((u_char *) saun->sun_path,
                            socklen - offsetof(struct sockaddr_un, sun_path));
            p = ngx_snprintf(text, len, "unix:%*s%Z", n, saun->sun_path);
        }

        /* we do not include trailing zero in address length */

        반환 (p - text - 1);

#endif

    default:
        반환 0;
    }
}


size_t
ngx_inet_ntop(정수 family, void *addr, u_char *text, size_t len)
{
    u_char  *p;

    switch (family) {

    case AF_INET:

        p = addr;

        반환 ngx_snprintf(text, len, "%ud.%ud.%ud.%ud",
                            p[0], p[1], p[2], p[3])
               - text;

#만약 (NGX_HAVE_INET6)

    case AF_INET6:
        반환 ngx_inet6_ntop(addr, text, len);

#endif

    default:
        반환 0;
    }
}


#만약 (NGX_HAVE_INET6)

size_t
ngx_inet6_ntop(u_char *p, u_char *text, size_t len)
{
    u_char      *dst;
    size_t       max, n;
    ngx_uint_t   i, zero, last;

    만약 (len < NGX_INET6_ADDRSTRLEN) {
        반환 0;
    }

    zero = (ngx_uint_t) -1;
    last = (ngx_uint_t) -1;
    max = 1;
    n = 0;

    위해 (i = 0; i < 16; i += 2) {

        만약 (p[i] || p[i + 1]) {

            만약 (max < n) {
                zero = last;
                max = n;
            }

            n = 0;
            continue;
        }

        만약 (n++ == 0) {
            last = i;
        }
    }

    만약 (max < n) {
        zero = last;
        max = n;
    }

    dst = text;
    n = 16;

    만약 (zero == 0) {

        만약 ((max == 5 && p[10] == 0xff && p[11] == 0xff)
            || (max == 6)
            || (max == 7 && p[14] != 0 && p[15] != 1))
        {
            n = 12;
        }

        *dst++ = ':';
    }

    위해 (i = 0; i < n; i += 2) {

        만약 (i == zero) {
            *dst++ = ':';
            i += (max - 1) * 2;
            continue;
        }

        dst = ngx_sprintf(dst, "%xd", p[i] * 256 + p[i + 1]);

        만약 (i < 14) {
            *dst++ = ':';
        }
    }

    만약 (n == 12) {
        dst = ngx_sprintf(dst, "%ud.%ud.%ud.%ud", p[12], p[13], p[14], p[15]);
    }

    반환 dst - text;
}

#endif


ngx_int_t
ngx_ptocidr(ngx_str_t *text, ngx_cidr_t *cidr)
{
    u_char      *addr, *mask, *last;
    size_t       len;
    ngx_int_t    shift;
#만약 (NGX_HAVE_INET6)
    ngx_int_t    rc;
    ngx_uint_t   s, i;
#endif

    addr = text->data;
    last = addr + text->len;

    mask = ngx_strlchr(addr, last, '/');
    len = (mask ? mask : last) - addr;

    cidr->u.in.addr = ngx_inet_addr(addr, len);

    만약 (cidr->u.in.addr != INADDR_NONE) {
        cidr->family = AF_INET;

        만약 (mask == NULL) {
            cidr->u.in.mask = 0xffffffff;
            반환 NGX_OK;
        }

#만약 (NGX_HAVE_INET6)
    } 아니면 만약 (ngx_inet6_addr(addr, len, cidr->u.in6.addr.s6_addr) == NGX_OK) {
        cidr->family = AF_INET6;

        만약 (mask == NULL) {
            ngx_memset(cidr->u.in6.mask.s6_addr, 0xff, 16);
            반환 NGX_OK;
        }

#endif
    } 아니면 {
        반환 NGX_ERROR;
    }

    mask++;

    shift = ngx_atoi(mask, last - mask);
    만약 (shift == NGX_ERROR) {
        반환 NGX_ERROR;
    }

    switch (cidr->family) {

#만약 (NGX_HAVE_INET6)
    case AF_INET6:
        만약 (shift > 128) {
            반환 NGX_ERROR;
        }

        addr = cidr->u.in6.addr.s6_addr;
        mask = cidr->u.in6.mask.s6_addr;
        rc = NGX_OK;

        위해 (i = 0; i < 16; i++) {

            s = (shift > 8) ? 8 : shift;
            shift -= s;

            mask[i] = (u_char) (0xffu << (8 - s));

            만약 (addr[i] != (addr[i] & mask[i])) {
                rc = NGX_DONE;
                addr[i] &= mask[i];
            }
        }

        반환 rc;
#endif

    default: /* AF_INET */
        만약 (shift > 32) {
            반환 NGX_ERROR;
        }

        만약 (shift) {
            cidr->u.in.mask = htonl((uint32_t) (0xffffffffu << (32 - shift)));

        } 아니면 {
            /* x86 compilers use a shl instruction that shifts by modulo 32 */
            cidr->u.in.mask = 0;
        }

        만약 (cidr->u.in.addr == (cidr->u.in.addr & cidr->u.in.mask)) {
            반환 NGX_OK;
        }

        cidr->u.in.addr &= cidr->u.in.mask;

        반환 NGX_DONE;
    }
}


ngx_int_t
ngx_cidr_match(struct sockaddr *sa, ngx_array_t *cidrs)
{
#만약 (NGX_HAVE_INET6)
    u_char           *p;
#endif
    in_addr_t         inaddr;
    ngx_cidr_t       *cidr;
    ngx_uint_t        family, i;
#만약 (NGX_HAVE_INET6)
    ngx_uint_t        n;
    struct in6_addr  *inaddr6;
#endif

#만약 (NGX_SUPPRESS_WARN)
    inaddr = 0;
#만약 (NGX_HAVE_INET6)
    inaddr6 = NULL;
#endif
#endif

    family = sa->sa_family;

    만약 (family == AF_INET) {
        inaddr = ((struct sockaddr_in *) sa)->sin_addr.s_addr;
    }

#만약 (NGX_HAVE_INET6)
    아니면 만약 (family == AF_INET6) {
        inaddr6 = &((struct sockaddr_in6 *) sa)->sin6_addr;

        만약 (IN6_IS_ADDR_V4MAPPED(inaddr6)) {
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

    위해 (cidr = cidrs->elts, i = 0; i < cidrs->nelts; i++) {
        만약 (cidr[i].family != family) {
            goto next;
        }

        switch (family) {

#만약 (NGX_HAVE_INET6)
        case AF_INET6:
            위해 (n = 0; n < 16; n++) {
                만약 ((inaddr6->s6_addr[n] & cidr[i].u.in6.mask.s6_addr[n])
                    != cidr[i].u.in6.addr.s6_addr[n])
                {
                    goto next;
                }
            }
            break;
#endif

#만약 (NGX_HAVE_UNIX_DOMAIN)
        case AF_UNIX:
            break;
#endif

        default: /* AF_INET */
            만약 ((inaddr & cidr[i].u.in.mask) != cidr[i].u.in.addr) {
                goto next;
            }
            break;
        }

        반환 NGX_OK;

    next:
        continue;
    }

    반환 NGX_DECLINED;
}


ngx_int_t
ngx_parse_addr(ngx_pool_t *pool, ngx_addr_t *addr, u_char *text, size_t len)
{
    in_addr_t             inaddr;
    ngx_uint_t            family;
    struct sockaddr_in   *sin;
#만약 (NGX_HAVE_INET6)
    struct in6_addr       inaddr6;
    struct sockaddr_in6  *sin6;

    /*
     * prevent MSVC8 warning:
     *    potentially uninitialized local variable 'inaddr6' used
     */
    ngx_memzero(&inaddr6, sizeof(struct in6_addr));
#endif

    inaddr = ngx_inet_addr(text, len);

    만약 (inaddr != INADDR_NONE) {
        family = AF_INET;
        len = sizeof(struct sockaddr_in);

#만약 (NGX_HAVE_INET6)
    } 아니면 만약 (ngx_inet6_addr(text, len, inaddr6.s6_addr) == NGX_OK) {
        family = AF_INET6;
        len = sizeof(struct sockaddr_in6);

#endif
    } 아니면 {
        반환 NGX_DECLINED;
    }

    addr->sockaddr = ngx_pcalloc(pool, len);
    만약 (addr->sockaddr == NULL) {
        반환 NGX_ERROR;
    }

    addr->sockaddr->sa_family = (u_char) family;
    addr->socklen = len;

    switch (family) {

#만약 (NGX_HAVE_INET6)
    case AF_INET6:
        sin6 = (struct sockaddr_in6 *) addr->sockaddr;
        ngx_memcpy(sin6->sin6_addr.s6_addr, inaddr6.s6_addr, 16);
        break;
#endif

    default: /* AF_INET */
        sin = (struct sockaddr_in *) addr->sockaddr;
        sin->sin_addr.s_addr = inaddr;
        break;
    }

    반환 NGX_OK;
}


ngx_int_t
ngx_parse_addr_port(ngx_pool_t *pool, ngx_addr_t *addr, u_char *text,
    size_t len)
{
    u_char     *p, *last;
    size_t      plen;
    ngx_int_t   rc, port;

    rc = ngx_parse_addr(pool, addr, text, len);

    만약 (rc != NGX_DECLINED) {
        반환 rc;
    }

    last = text + len;

#만약 (NGX_HAVE_INET6)
    만약 (len && text[0] == '[') {

        p = ngx_strlchr(text, last, ']');

        만약 (p == last - 1) {
            반환 ngx_parse_addr(pool, addr, text + 1, len - 2);
        }

        만약 (p == NULL || *++p != ':') {
            반환 NGX_DECLINED;
        }

        text++;
        len -= 2;

    } 아니면
#endif

    {
        p = ngx_strlchr(text, last, ':');

        만약 (p == NULL) {
            반환 NGX_DECLINED;
        }
    }

    p++;
    plen = last - p;

    port = ngx_atoi(p, plen);

    만약 (port < 1 || port > 65535) {
        반환 NGX_DECLINED;
    }

    len -= plen + 1;

    rc = ngx_parse_addr(pool, addr, text, len);

    만약 (rc != NGX_OK) {
        반환 rc;
    }

    ngx_inet_set_port(addr->sockaddr, (in_port_t) port);

    반환 NGX_OK;
}


ngx_int_t
ngx_parse_url(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char  *p;
    size_t   len;

    p = u->url.data;
    len = u->url.len;

    만약 (len >= 5 && ngx_strncasecmp(p, (u_char *) "unix:", 5) == 0) {
        반환 ngx_parse_unix_domain_url(pool, u);
    }

    만약 (len && p[0] == '[') {
        반환 ngx_parse_inet6_url(pool, u);
    }

    반환 ngx_parse_inet_url(pool, u);
}


static ngx_int_t
ngx_parse_unix_domain_url(ngx_pool_t *pool, ngx_url_t *u)
{
#만약 (NGX_HAVE_UNIX_DOMAIN)
    u_char              *path, *uri, *last;
    size_t               len;
    struct sockaddr_un  *saun;

    len = u->url.len;
    path = u->url.data;

    path += 5;
    len -= 5;

    만약 (u->uri_part) {

        last = path + len;
        uri = ngx_strlchr(path, last, ':');

        만약 (uri) {
            len = uri - path;
            uri++;
            u->uri.len = last - uri;
            u->uri.data = uri;
        }
    }

    만약 (len == 0) {
        u->err = "no path in the unix domain socket";
        반환 NGX_ERROR;
    }

    u->host.len = len++;
    u->host.data = path;

    만약 (len > sizeof(saun->sun_path)) {
        u->err = "too long path in the unix domain socket";
        반환 NGX_ERROR;
    }

    u->socklen = sizeof(struct sockaddr_un);
    saun = (struct sockaddr_un *) &u->sockaddr;
    saun->sun_family = AF_UNIX;
    (void) ngx_cpystrn((u_char *) saun->sun_path, path, len);

    u->addrs = ngx_pcalloc(pool, sizeof(ngx_addr_t));
    만약 (u->addrs == NULL) {
        반환 NGX_ERROR;
    }

    saun = ngx_pcalloc(pool, sizeof(struct sockaddr_un));
    만약 (saun == NULL) {
        반환 NGX_ERROR;
    }

    u->family = AF_UNIX;
    u->naddrs = 1;

    saun->sun_family = AF_UNIX;
    (void) ngx_cpystrn((u_char *) saun->sun_path, path, len);

    u->addrs[0].sockaddr = (struct sockaddr *) saun;
    u->addrs[0].socklen = sizeof(struct sockaddr_un);
    u->addrs[0].name.len = len + 4;
    u->addrs[0].name.data = u->url.data;

    반환 NGX_OK;

#아니면

    u->err = "the unix domain sockets are not supported on this platform";

    반환 NGX_ERROR;

#endif
}


static ngx_int_t
ngx_parse_inet_url(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char              *host, *port, *last, *uri, *args, *dash;
    size_t               len;
    ngx_int_t            n;
    struct sockaddr_in  *sin;

    u->socklen = sizeof(struct sockaddr_in);
    sin = (struct sockaddr_in *) &u->sockaddr;
    sin->sin_family = AF_INET;

    u->family = AF_INET;

    host = u->url.data;

    last = host + u->url.len;

    port = ngx_strlchr(host, last, ':');

    uri = ngx_strlchr(host, last, '/');

    args = ngx_strlchr(host, last, '?');

    만약 (args) {
        만약 (uri == NULL || args < uri) {
            uri = args;
        }
    }

    만약 (uri) {
        만약 (u->listen || !u->uri_part) {
            u->err = "invalid host";
            반환 NGX_ERROR;
        }

        u->uri.len = last - uri;
        u->uri.data = uri;

        last = uri;

        만약 (uri < port) {
            port = NULL;
        }
    }

    만약 (port) {
        port++;

        len = last - port;

        만약 (u->listen) {
            dash = ngx_strlchr(port, last, '-');

            만약 (dash) {
                dash++;

                n = ngx_atoi(dash, last - dash);

                만약 (n < 1 || n > 65535) {
                    u->err = "invalid port";
                    반환 NGX_ERROR;
                }

                u->last_port = (in_port_t) n;

                len = dash - port - 1;
            }
        }

        n = ngx_atoi(port, len);

        만약 (n < 1 || n > 65535) {
            u->err = "invalid port";
            반환 NGX_ERROR;
        }

        만약 (u->last_port && n > u->last_port) {
            u->err = "invalid port range";
            반환 NGX_ERROR;
        }

        u->port = (in_port_t) n;
        sin->sin_port = htons((in_port_t) n);

        u->port_text.len = last - port;
        u->port_text.data = port;

        last = port - 1;

    } 아니면 {
        만약 (uri == NULL) {

            만약 (u->listen) {

                /* test value as port only */

                len = last - host;

                dash = ngx_strlchr(host, last, '-');

                만약 (dash) {
                    dash++;

                    n = ngx_atoi(dash, last - dash);

                    만약 (n == NGX_ERROR) {
                        goto no_port;
                    }

                    만약 (n < 1 || n > 65535) {
                        u->err = "invalid port";

                    } 아니면 {
                        u->last_port = (in_port_t) n;
                    }

                    len = dash - host - 1;
                }

                n = ngx_atoi(host, len);

                만약 (n != NGX_ERROR) {

                    만약 (u->err) {
                        반환 NGX_ERROR;
                    }

                    만약 (n < 1 || n > 65535) {
                        u->err = "invalid port";
                        반환 NGX_ERROR;
                    }

                    만약 (u->last_port && n > u->last_port) {
                        u->err = "invalid port range";
                        반환 NGX_ERROR;
                    }

                    u->port = (in_port_t) n;
                    sin->sin_port = htons((in_port_t) n);
                    sin->sin_addr.s_addr = INADDR_ANY;

                    u->port_text.len = last - host;
                    u->port_text.data = host;

                    u->wildcard = 1;

                    반환 ngx_inet_add_addr(pool, u, &u->sockaddr.sockaddr,
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

    만약 (len == 0) {
        u->err = "no host";
        반환 NGX_ERROR;
    }

    u->host.len = len;
    u->host.data = host;

    만약 (u->listen && len == 1 && *host == '*') {
        sin->sin_addr.s_addr = INADDR_ANY;
        u->wildcard = 1;
        반환 ngx_inet_add_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);
    }

    sin->sin_addr.s_addr = ngx_inet_addr(host, len);

    만약 (sin->sin_addr.s_addr != INADDR_NONE) {

        만약 (sin->sin_addr.s_addr == INADDR_ANY) {
            u->wildcard = 1;
        }

        반환 ngx_inet_add_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);
    }

    만약 (u->no_resolve) {
        반환 NGX_OK;
    }

    만약 (ngx_inet_resolve_host(pool, u) != NGX_OK) {
        반환 NGX_ERROR;
    }

    u->family = u->addrs[0].sockaddr->sa_family;
    u->socklen = u->addrs[0].socklen;
    ngx_memcpy(&u->sockaddr, u->addrs[0].sockaddr, u->addrs[0].socklen);
    u->wildcard = ngx_inet_wildcard(&u->sockaddr.sockaddr);

    반환 NGX_OK;
}


static ngx_int_t
ngx_parse_inet6_url(ngx_pool_t *pool, ngx_url_t *u)
{
#만약 (NGX_HAVE_INET6)
    u_char               *p, *host, *port, *last, *uri, *dash;
    size_t                len;
    ngx_int_t             n;
    struct sockaddr_in6  *sin6;

    u->socklen = sizeof(struct sockaddr_in6);
    sin6 = (struct sockaddr_in6 *) &u->sockaddr;
    sin6->sin6_family = AF_INET6;

    host = u->url.data + 1;

    last = u->url.data + u->url.len;

    p = ngx_strlchr(host, last, ']');

    만약 (p == NULL) {
        u->err = "invalid host";
        반환 NGX_ERROR;
    }

    port = p + 1;

    uri = ngx_strlchr(port, last, '/');

    만약 (uri) {
        만약 (u->listen || !u->uri_part) {
            u->err = "invalid host";
            반환 NGX_ERROR;
        }

        u->uri.len = last - uri;
        u->uri.data = uri;

        last = uri;
    }

    만약 (port < last) {
        만약 (*port != ':') {
            u->err = "invalid host";
            반환 NGX_ERROR;
        }

        port++;

        len = last - port;

        만약 (u->listen) {
            dash = ngx_strlchr(port, last, '-');

            만약 (dash) {
                dash++;

                n = ngx_atoi(dash, last - dash);

                만약 (n < 1 || n > 65535) {
                    u->err = "invalid port";
                    반환 NGX_ERROR;
                }

                u->last_port = (in_port_t) n;

                len = dash - port - 1;
            }
        }

        n = ngx_atoi(port, len);

        만약 (n < 1 || n > 65535) {
            u->err = "invalid port";
            반환 NGX_ERROR;
        }

        만약 (u->last_port && n > u->last_port) {
            u->err = "invalid port range";
            반환 NGX_ERROR;
        }

        u->port = (in_port_t) n;
        sin6->sin6_port = htons((in_port_t) n);

        u->port_text.len = last - port;
        u->port_text.data = port;

    } 아니면 {
        u->no_port = 1;
        u->port = u->default_port;
        sin6->sin6_port = htons(u->default_port);
    }

    len = p - host;

    만약 (len == 0) {
        u->err = "no host";
        반환 NGX_ERROR;
    }

    u->host.len = len + 2;
    u->host.data = host - 1;

    만약 (ngx_inet6_addr(host, len, sin6->sin6_addr.s6_addr) != NGX_OK) {
        u->err = "invalid IPv6 address";
        반환 NGX_ERROR;
    }

    만약 (IN6_IS_ADDR_UNSPECIFIED(&sin6->sin6_addr)) {
        u->wildcard = 1;
    }

    u->family = AF_INET6;

    반환 ngx_inet_add_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);

#아니면

    u->err = "the INET6 sockets are not supported on this platform";

    반환 NGX_ERROR;

#endif
}


#만약 (NGX_HAVE_GETADDRINFO && NGX_HAVE_INET6)

ngx_int_t
ngx_inet_resolve_host(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char           *host;
    ngx_uint_t        n;
    struct addrinfo   hints, *res, *rp;

    host = ngx_alloc(u->host.len + 1, pool->log);
    만약 (host == NULL) {
        반환 NGX_ERROR;
    }

    (void) ngx_cpystrn(host, u->host.data, u->host.len + 1);

    ngx_memzero(&hints, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
#ifdef AI_ADDRCONFIG
    hints.ai_flags = AI_ADDRCONFIG;
#endif

    만약 (getaddrinfo((char *) host, NULL, &hints, &res) != 0) {
        u->err = "host not found";
        ngx_free(host);
        반환 NGX_ERROR;
    }

    ngx_free(host);

    위해 (n = 0, rp = res; rp != NULL; rp = rp->ai_next) {

        switch (rp->ai_family) {

        case AF_INET:
        case AF_INET6:
            break;

        default:
            continue;
        }

        n++;
    }

    만약 (n == 0) {
        u->err = "host not found";
        goto failed;
    }

    /* MP: ngx_shared_palloc() */

    위해 (rp = res; rp != NULL; rp = rp->ai_next) {

        switch (rp->ai_family) {

        case AF_INET:
        case AF_INET6:
            break;

        default:
            continue;
        }

        만약 (ngx_inet_add_addr(pool, u, rp->ai_addr, rp->ai_addrlen, n)
            != NGX_OK)
        {
            goto failed;
        }
    }

    freeaddrinfo(res);
    반환 NGX_OK;

failed:

    freeaddrinfo(res);
    반환 NGX_ERROR;
}

#아니면 /* !NGX_HAVE_GETADDRINFO || !NGX_HAVE_INET6 */

ngx_int_t
ngx_inet_resolve_host(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char              *host;
    ngx_uint_t           i, n;
    struct hostent      *h;
    struct sockaddr_in   sin;

    /* AF_INET only */

    ngx_memzero(&sin, sizeof(struct sockaddr_in));

    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = ngx_inet_addr(u->host.data, u->host.len);

    만약 (sin.sin_addr.s_addr == INADDR_NONE) {
        host = ngx_alloc(u->host.len + 1, pool->log);
        만약 (host == NULL) {
            반환 NGX_ERROR;
        }

        (void) ngx_cpystrn(host, u->host.data, u->host.len + 1);

        h = gethostbyname((char *) host);

        ngx_free(host);

        만약 (h == NULL || h->h_addr_list[0] == NULL) {
            u->err = "host not found";
            반환 NGX_ERROR;
        }

        위해 (n = 0; h->h_addr_list[n] != NULL; n++) { /* void */ }

        /* MP: ngx_shared_palloc() */

        위해 (i = 0; i < n; i++) {
            sin.sin_addr.s_addr = *(in_addr_t *) (h->h_addr_list[i]);

            만약 (ngx_inet_add_addr(pool, u, (struct sockaddr *) &sin,
                                  sizeof(struct sockaddr_in), n)
                != NGX_OK)
            {
                반환 NGX_ERROR;
            }
        }

    } 아니면 {

        /* MP: ngx_shared_palloc() */

        만약 (ngx_inet_add_addr(pool, u, (struct sockaddr *) &sin,
                              sizeof(struct sockaddr_in), 1)
            != NGX_OK)
        {
            반환 NGX_ERROR;
        }
    }

    반환 NGX_OK;
}

#endif /* NGX_HAVE_GETADDRINFO && NGX_HAVE_INET6 */


static ngx_int_t
ngx_inet_add_addr(ngx_pool_t *pool, ngx_url_t *u, struct sockaddr *sockaddr,
    socklen_t socklen, ngx_uint_t total)
{
    u_char           *p;
    size_t            len;
    ngx_uint_t        i, nports;
    ngx_addr_t       *addr;
    struct sockaddr  *sa;

    nports = u->last_port ? u->last_port - u->port + 1 : 1;

    만약 (u->addrs == NULL) {
        u->addrs = ngx_palloc(pool, total * nports * sizeof(ngx_addr_t));
        만약 (u->addrs == NULL) {
            반환 NGX_ERROR;
        }
    }

    위해 (i = 0; i < nports; i++) {
        sa = ngx_pcalloc(pool, socklen);
        만약 (sa == NULL) {
            반환 NGX_ERROR;
        }

        ngx_memcpy(sa, sockaddr, socklen);

        ngx_inet_set_port(sa, u->port + i);

        switch (sa->sa_family) {

#만약 (NGX_HAVE_INET6)
        case AF_INET6:
            len = NGX_INET6_ADDRSTRLEN + sizeof("[]:65536") - 1;
            break;
#endif

        default: /* AF_INET */
            len = NGX_INET_ADDRSTRLEN + sizeof(":65535") - 1;
        }

        p = ngx_pnalloc(pool, len);
        만약 (p == NULL) {
            반환 NGX_ERROR;
        }

        len = ngx_sock_ntop(sa, socklen, p, len, 1);

        addr = &u->addrs[u->naddrs++];

        addr->sockaddr = sa;
        addr->socklen = socklen;

        addr->name.len = len;
        addr->name.data = p;
    }

    반환 NGX_OK;
}


ngx_int_t
ngx_cmp_sockaddr(struct sockaddr *sa1, socklen_t slen1,
    struct sockaddr *sa2, socklen_t slen2, ngx_uint_t cmp_port)
{
    struct sockaddr_in   *sin1, *sin2;
#만약 (NGX_HAVE_INET6)
    struct sockaddr_in6  *sin61, *sin62;
#endif
#만약 (NGX_HAVE_UNIX_DOMAIN)
    size_t                len;
    struct sockaddr_un   *saun1, *saun2;
#endif

    만약 (sa1->sa_family != sa2->sa_family) {
        반환 NGX_DECLINED;
    }

    switch (sa1->sa_family) {

#만약 (NGX_HAVE_INET6)
    case AF_INET6:

        sin61 = (struct sockaddr_in6 *) sa1;
        sin62 = (struct sockaddr_in6 *) sa2;

        만약 (cmp_port && sin61->sin6_port != sin62->sin6_port) {
            반환 NGX_DECLINED;
        }

        만약 (ngx_memcmp(&sin61->sin6_addr, &sin62->sin6_addr, 16) != 0) {
            반환 NGX_DECLINED;
        }

        break;
#endif

#만약 (NGX_HAVE_UNIX_DOMAIN)
    case AF_UNIX:

        saun1 = (struct sockaddr_un *) sa1;
        saun2 = (struct sockaddr_un *) sa2;

        만약 (slen1 < slen2) {
            len = slen1 - offsetof(struct sockaddr_un, sun_path);

        } 아니면 {
            len = slen2 - offsetof(struct sockaddr_un, sun_path);
        }

        만약 (len > sizeof(saun1->sun_path)) {
            len = sizeof(saun1->sun_path);
        }

        만약 (ngx_memcmp(&saun1->sun_path, &saun2->sun_path, len) != 0) {
            반환 NGX_DECLINED;
        }

        break;
#endif

    default: /* AF_INET */

        sin1 = (struct sockaddr_in *) sa1;
        sin2 = (struct sockaddr_in *) sa2;

        만약 (cmp_port && sin1->sin_port != sin2->sin_port) {
            반환 NGX_DECLINED;
        }

        만약 (sin1->sin_addr.s_addr != sin2->sin_addr.s_addr) {
            반환 NGX_DECLINED;
        }

        break;
    }

    반환 NGX_OK;
}


in_port_t
ngx_inet_get_port(struct sockaddr *sa)
{
    struct sockaddr_in   *sin;
#만약 (NGX_HAVE_INET6)
    struct sockaddr_in6  *sin6;
#endif

    switch (sa->sa_family) {

#만약 (NGX_HAVE_INET6)
    case AF_INET6:
        sin6 = (struct sockaddr_in6 *) sa;
        반환 ntohs(sin6->sin6_port);
#endif

#만약 (NGX_HAVE_UNIX_DOMAIN)
    case AF_UNIX:
        반환 0;
#endif

    default: /* AF_INET */
        sin = (struct sockaddr_in *) sa;
        반환 ntohs(sin->sin_port);
    }
}


void
ngx_inet_set_port(struct sockaddr *sa, in_port_t port)
{
    struct sockaddr_in   *sin;
#만약 (NGX_HAVE_INET6)
    struct sockaddr_in6  *sin6;
#endif

    switch (sa->sa_family) {

#만약 (NGX_HAVE_INET6)
    case AF_INET6:
        sin6 = (struct sockaddr_in6 *) sa;
        sin6->sin6_port = htons(port);
        break;
#endif

#만약 (NGX_HAVE_UNIX_DOMAIN)
    case AF_UNIX:
        break;
#endif

    default: /* AF_INET */
        sin = (struct sockaddr_in *) sa;
        sin->sin_port = htons(port);
        break;
    }
}


ngx_uint_t
ngx_inet_wildcard(struct sockaddr *sa)
{
    struct sockaddr_in   *sin;
#만약 (NGX_HAVE_INET6)
    struct sockaddr_in6  *sin6;
#endif

    switch (sa->sa_family) {

    case AF_INET:
        sin = (struct sockaddr_in *) sa;

        만약 (sin->sin_addr.s_addr == INADDR_ANY) {
            반환 1;
        }

        break;

#만약 (NGX_HAVE_INET6)

    case AF_INET6:
        sin6 = (struct sockaddr_in6 *) sa;

        만약 (IN6_IS_ADDR_UNSPECIFIED(&sin6->sin6_addr)) {
            반환 1;
        }

        break;

#endif
    }

    반환 0;
}
