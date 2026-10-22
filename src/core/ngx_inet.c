
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


ساكن ngx_int_t ngx_parse_unix_domain_url(ngx_pool_t *pool, ngx_url_t *u);
ساكن ngx_int_t ngx_parse_inet_url(ngx_pool_t *pool, ngx_url_t *u);
ساكن ngx_int_t ngx_parse_inet6_url(ngx_pool_t *pool, ngx_url_t *u);
ساكن ngx_int_t ngx_inet_add_addr(ngx_pool_t *pool, ngx_url_t *u,
    بنية sockaddr *sockaddr, socklen_t socklen, ngx_uint_t total);


in_addr_t
ngx_inet_addr(u_char *text, size_t len)
{
    u_char      *p, c;
    in_addr_t    addr;
    ngx_uint_t   octet, n;

    addr = 0;
    octet = 0;
    n = 0;

    لكل (p = text; p < text + len; p++) {
        c = *p;

        إذا (c >= '0' && c <= '9') {
            octet = octet * 10 + (c - '0');

            إذا (octet > 255) {
                أرجع INADDR_NONE;
            }

            استمر;
        }

        إذا (c == '.') {
            addr = (addr << 8) + octet;
            octet = 0;
            n++;
            استمر;
        }

        أرجع INADDR_NONE;
    }

    إذا (n == 3) {
        addr = (addr << 8) + octet;
        أرجع htonl(addr);
    }

    أرجع INADDR_NONE;
}


#إذا (NGX_HAVE_INET6)

ngx_int_t
ngx_inet6_addr(u_char *p, size_t len, u_char *addr)
{
    u_char      c, *zero, *digit, *s, *d;
    size_t      len4;
    ngx_uint_t  n, nibbles, word;

    إذا (len == 0) {
        أرجع NGX_ERROR;
    }

    zero = عدم;
    digit = عدم;
    len4 = 0;
    nibbles = 0;
    word = 0;
    n = 8;

    إذا (p[0] == ':') {
        p++;
        len--;
    }

    لكل (/* void */; len; len--) {
        c = *p++;

        إذا (c == ':') {
            إذا (nibbles) {
                digit = p;
                len4 = len;
                *addr++ = (u_char) (word >> 8);
                *addr++ = (u_char) (word & 0xff);

                إذا (--n) {
                    nibbles = 0;
                    word = 0;
                    استمر;
                }

            } وإلا {
                إذا (zero == عدم) {
                    digit = p;
                    len4 = len;
                    zero = addr;
                    استمر;
                }
            }

            أرجع NGX_ERROR;
        }

        إذا (c == '.' && nibbles) {
            إذا (n < 2 || digit == عدم) {
                أرجع NGX_ERROR;
            }

            word = ngx_inet_addr(digit, len4 - 1);
            إذا (word == INADDR_NONE) {
                أرجع NGX_ERROR;
            }

            word = ntohl(word);
            *addr++ = (u_char) ((word >> 24) & 0xff);
            *addr++ = (u_char) ((word >> 16) & 0xff);
            n--;
            اكسر;
        }

        إذا (++nibbles > 4) {
            أرجع NGX_ERROR;
        }

        إذا (c >= '0' && c <= '9') {
            word = word * 16 + (c - '0');
            استمر;
        }

        c |= 0x20;

        إذا (c >= 'a' && c <= 'f') {
            word = word * 16 + (c - 'a') + 10;
            استمر;
        }

        أرجع NGX_ERROR;
    }

    إذا (nibbles == 0 && zero == عدم) {
        أرجع NGX_ERROR;
    }

    *addr++ = (u_char) (word >> 8);
    *addr++ = (u_char) (word & 0xff);

    إذا (--n) {
        إذا (zero) {
            n *= 2;
            s = addr - 1;
            d = s + n;
            طالما (s >= zero) {
                *d-- = *s--;
            }
            ngx_memzero(zero, n);
            أرجع NGX_OK;
        }

    } وإلا {
        إذا (zero == عدم) {
            أرجع NGX_OK;
        }
    }

    أرجع NGX_ERROR;
}

#endif


size_t
ngx_sock_ntop(بنية sockaddr *sa, socklen_t socklen, u_char *text, size_t len,
    ngx_uint_t port)
{
    u_char               *p;
#إذا (NGX_HAVE_INET6 || NGX_HAVE_UNIX_DOMAIN)
    size_t                n;
#endif
    بنية sockaddr_in   *sin;
#إذا (NGX_HAVE_INET6)
    بنية sockaddr_in6  *sin6;
#endif
#إذا (NGX_HAVE_UNIX_DOMAIN)
    بنية sockaddr_un   *saun;
#endif

    بدل (sa->sa_family) {

    حالة AF_INET:

        sin = (بنية sockaddr_in *) sa;
        p = (u_char *) &sin->sin_addr;

        إذا (port) {
            p = ngx_snprintf(text, len, "%ud.%ud.%ud.%ud:%d",
                             p[0], p[1], p[2], p[3], ntohs(sin->sin_port));
        } وإلا {
            p = ngx_snprintf(text, len, "%ud.%ud.%ud.%ud",
                             p[0], p[1], p[2], p[3]);
        }

        أرجع (p - text);

#إذا (NGX_HAVE_INET6)

    حالة AF_INET6:

        sin6 = (بنية sockaddr_in6 *) sa;

        n = 0;

        إذا (port) {
            text[n++] = '[';
        }

        n = ngx_inet6_ntop(sin6->sin6_addr.s6_addr, &text[n], len);

        إذا (port) {
            n = ngx_sprintf(&text[1 + n], "]:%d",
                            ntohs(sin6->sin6_port)) - text;
        }

        أرجع n;
#endif

#إذا (NGX_HAVE_UNIX_DOMAIN)

    حالة AF_UNIX:
        saun = (بنية sockaddr_un *) sa;

        /* on Linux sockaddr might not include sun_path at all */

        إذا (socklen <= (socklen_t) offsetof(بنية sockaddr_un, sun_path)) {
            p = ngx_snprintf(text, len, "unix:%Z");

        } وإلا {
            n = ngx_strnlen((u_char *) saun->sun_path,
                            socklen - offsetof(بنية sockaddr_un, sun_path));
            p = ngx_snprintf(text, len, "unix:%*s%Z", n, saun->sun_path);
        }

        /* we do not include trailing zero in address length */

        أرجع (p - text - 1);

#endif

    افتراضي:
        أرجع 0;
    }
}


size_t
ngx_inet_ntop(صحيح family, فراغ *addr, u_char *text, size_t len)
{
    u_char  *p;

    بدل (family) {

    حالة AF_INET:

        p = addr;

        أرجع ngx_snprintf(text, len, "%ud.%ud.%ud.%ud",
                            p[0], p[1], p[2], p[3])
               - text;

#إذا (NGX_HAVE_INET6)

    حالة AF_INET6:
        أرجع ngx_inet6_ntop(addr, text, len);

#endif

    افتراضي:
        أرجع 0;
    }
}


#إذا (NGX_HAVE_INET6)

size_t
ngx_inet6_ntop(u_char *p, u_char *text, size_t len)
{
    u_char      *dst;
    size_t       max, n;
    ngx_uint_t   i, zero, last;

    إذا (len < NGX_INET6_ADDRSTRLEN) {
        أرجع 0;
    }

    zero = (ngx_uint_t) -1;
    last = (ngx_uint_t) -1;
    max = 1;
    n = 0;

    لكل (i = 0; i < 16; i += 2) {

        إذا (p[i] || p[i + 1]) {

            إذا (max < n) {
                zero = last;
                max = n;
            }

            n = 0;
            استمر;
        }

        إذا (n++ == 0) {
            last = i;
        }
    }

    إذا (max < n) {
        zero = last;
        max = n;
    }

    dst = text;
    n = 16;

    إذا (zero == 0) {

        إذا ((max == 5 && p[10] == 0xff && p[11] == 0xff)
            || (max == 6)
            || (max == 7 && p[14] != 0 && p[15] != 1))
        {
            n = 12;
        }

        *dst++ = ':';
    }

    لكل (i = 0; i < n; i += 2) {

        إذا (i == zero) {
            *dst++ = ':';
            i += (max - 1) * 2;
            استمر;
        }

        dst = ngx_sprintf(dst, "%xd", p[i] * 256 + p[i + 1]);

        إذا (i < 14) {
            *dst++ = ':';
        }
    }

    إذا (n == 12) {
        dst = ngx_sprintf(dst, "%ud.%ud.%ud.%ud", p[12], p[13], p[14], p[15]);
    }

    أرجع dst - text;
}

#endif


ngx_int_t
ngx_ptocidr(ngx_str_t *text, ngx_cidr_t *cidr)
{
    u_char      *addr, *mask, *last;
    size_t       len;
    ngx_int_t    shift;
#إذا (NGX_HAVE_INET6)
    ngx_int_t    rc;
    ngx_uint_t   s, i;
#endif

    addr = text->data;
    last = addr + text->len;

    mask = ngx_strlchr(addr, last, '/');
    len = (mask ? mask : last) - addr;

    cidr->u.in.addr = ngx_inet_addr(addr, len);

    إذا (cidr->u.in.addr != INADDR_NONE) {
        cidr->family = AF_INET;

        إذا (mask == عدم) {
            cidr->u.in.mask = 0xffffffff;
            أرجع NGX_OK;
        }

#إذا (NGX_HAVE_INET6)
    } وإلا إذا (ngx_inet6_addr(addr, len, cidr->u.in6.addr.s6_addr) == NGX_OK) {
        cidr->family = AF_INET6;

        إذا (mask == عدم) {
            ngx_memset(cidr->u.in6.mask.s6_addr, 0xff, 16);
            أرجع NGX_OK;
        }

#endif
    } وإلا {
        أرجع NGX_ERROR;
    }

    mask++;

    shift = ngx_atoi(mask, last - mask);
    إذا (shift == NGX_ERROR) {
        أرجع NGX_ERROR;
    }

    بدل (cidr->family) {

#إذا (NGX_HAVE_INET6)
    حالة AF_INET6:
        إذا (shift > 128) {
            أرجع NGX_ERROR;
        }

        addr = cidr->u.in6.addr.s6_addr;
        mask = cidr->u.in6.mask.s6_addr;
        rc = NGX_OK;

        لكل (i = 0; i < 16; i++) {

            s = (shift > 8) ? 8 : shift;
            shift -= s;

            mask[i] = (u_char) (0xffu << (8 - s));

            إذا (addr[i] != (addr[i] & mask[i])) {
                rc = NGX_DONE;
                addr[i] &= mask[i];
            }
        }

        أرجع rc;
#endif

    افتراضي: /* AF_INET */
        إذا (shift > 32) {
            أرجع NGX_ERROR;
        }

        إذا (shift) {
            cidr->u.in.mask = htonl((uint32_t) (0xffffffffu << (32 - shift)));

        } وإلا {
            /* x86 compilers use a shl instruction that shifts by modulo 32 */
            cidr->u.in.mask = 0;
        }

        إذا (cidr->u.in.addr == (cidr->u.in.addr & cidr->u.in.mask)) {
            أرجع NGX_OK;
        }

        cidr->u.in.addr &= cidr->u.in.mask;

        أرجع NGX_DONE;
    }
}


ngx_int_t
ngx_cidr_match(بنية sockaddr *sa, ngx_array_t *cidrs)
{
#إذا (NGX_HAVE_INET6)
    u_char           *p;
#endif
    in_addr_t         inaddr;
    ngx_cidr_t       *cidr;
    ngx_uint_t        family, i;
#إذا (NGX_HAVE_INET6)
    ngx_uint_t        n;
    بنية in6_addr  *inaddr6;
#endif

#إذا (NGX_SUPPRESS_WARN)
    inaddr = 0;
#إذا (NGX_HAVE_INET6)
    inaddr6 = عدم;
#endif
#endif

    family = sa->sa_family;

    إذا (family == AF_INET) {
        inaddr = ((بنية sockaddr_in *) sa)->sin_addr.s_addr;
    }

#إذا (NGX_HAVE_INET6)
    وإلا إذا (family == AF_INET6) {
        inaddr6 = &((بنية sockaddr_in6 *) sa)->sin6_addr;

        إذا (IN6_IS_ADDR_V4MAPPED(inaddr6)) {
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

    لكل (cidr = cidrs->elts, i = 0; i < cidrs->nelts; i++) {
        إذا (cidr[i].family != family) {
            اذهب next;
        }

        بدل (family) {

#إذا (NGX_HAVE_INET6)
        حالة AF_INET6:
            لكل (n = 0; n < 16; n++) {
                إذا ((inaddr6->s6_addr[n] & cidr[i].u.in6.mask.s6_addr[n])
                    != cidr[i].u.in6.addr.s6_addr[n])
                {
                    اذهب next;
                }
            }
            اكسر;
#endif

#إذا (NGX_HAVE_UNIX_DOMAIN)
        حالة AF_UNIX:
            اكسر;
#endif

        افتراضي: /* AF_INET */
            إذا ((inaddr & cidr[i].u.in.mask) != cidr[i].u.in.addr) {
                اذهب next;
            }
            اكسر;
        }

        أرجع NGX_OK;

    next:
        استمر;
    }

    أرجع NGX_DECLINED;
}


ngx_int_t
ngx_parse_addr(ngx_pool_t *pool, ngx_addr_t *addr, u_char *text, size_t len)
{
    in_addr_t             inaddr;
    ngx_uint_t            family;
    بنية sockaddr_in   *sin;
#إذا (NGX_HAVE_INET6)
    بنية in6_addr       inaddr6;
    بنية sockaddr_in6  *sin6;

    /*
     * prevent MSVC8 warning:
     *    potentially uninitialized local variable 'inaddr6' used
     */
    ngx_memzero(&inaddr6, حجم(بنية in6_addr));
#endif

    inaddr = ngx_inet_addr(text, len);

    إذا (inaddr != INADDR_NONE) {
        family = AF_INET;
        len = حجم(بنية sockaddr_in);

#إذا (NGX_HAVE_INET6)
    } وإلا إذا (ngx_inet6_addr(text, len, inaddr6.s6_addr) == NGX_OK) {
        family = AF_INET6;
        len = حجم(بنية sockaddr_in6);

#endif
    } وإلا {
        أرجع NGX_DECLINED;
    }

    addr->sockaddr = ngx_pcalloc(pool, len);
    إذا (addr->sockaddr == عدم) {
        أرجع NGX_ERROR;
    }

    addr->sockaddr->sa_family = (u_char) family;
    addr->socklen = len;

    بدل (family) {

#إذا (NGX_HAVE_INET6)
    حالة AF_INET6:
        sin6 = (بنية sockaddr_in6 *) addr->sockaddr;
        ngx_memcpy(sin6->sin6_addr.s6_addr, inaddr6.s6_addr, 16);
        اكسر;
#endif

    افتراضي: /* AF_INET */
        sin = (بنية sockaddr_in *) addr->sockaddr;
        sin->sin_addr.s_addr = inaddr;
        اكسر;
    }

    أرجع NGX_OK;
}


ngx_int_t
ngx_parse_addr_port(ngx_pool_t *pool, ngx_addr_t *addr, u_char *text,
    size_t len)
{
    u_char     *p, *last;
    size_t      plen;
    ngx_int_t   rc, port;

    rc = ngx_parse_addr(pool, addr, text, len);

    إذا (rc != NGX_DECLINED) {
        أرجع rc;
    }

    last = text + len;

#إذا (NGX_HAVE_INET6)
    إذا (len && text[0] == '[') {

        p = ngx_strlchr(text, last, ']');

        إذا (p == last - 1) {
            أرجع ngx_parse_addr(pool, addr, text + 1, len - 2);
        }

        إذا (p == عدم || *++p != ':') {
            أرجع NGX_DECLINED;
        }

        text++;
        len -= 2;

    } وإلا
#endif

    {
        p = ngx_strlchr(text, last, ':');

        إذا (p == عدم) {
            أرجع NGX_DECLINED;
        }
    }

    p++;
    plen = last - p;

    port = ngx_atoi(p, plen);

    إذا (port < 1 || port > 65535) {
        أرجع NGX_DECLINED;
    }

    len -= plen + 1;

    rc = ngx_parse_addr(pool, addr, text, len);

    إذا (rc != NGX_OK) {
        أرجع rc;
    }

    ngx_inet_set_port(addr->sockaddr, (in_port_t) port);

    أرجع NGX_OK;
}


ngx_int_t
ngx_parse_url(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char  *p;
    size_t   len;

    p = u->url.data;
    len = u->url.len;

    إذا (len >= 5 && ngx_strncasecmp(p, (u_char *) "unix:", 5) == 0) {
        أرجع ngx_parse_unix_domain_url(pool, u);
    }

    إذا (len && p[0] == '[') {
        أرجع ngx_parse_inet6_url(pool, u);
    }

    أرجع ngx_parse_inet_url(pool, u);
}


ساكن ngx_int_t
ngx_parse_unix_domain_url(ngx_pool_t *pool, ngx_url_t *u)
{
#إذا (NGX_HAVE_UNIX_DOMAIN)
    u_char              *path, *uri, *last;
    size_t               len;
    بنية sockaddr_un  *saun;

    len = u->url.len;
    path = u->url.data;

    path += 5;
    len -= 5;

    إذا (u->uri_part) {

        last = path + len;
        uri = ngx_strlchr(path, last, ':');

        إذا (uri) {
            len = uri - path;
            uri++;
            u->uri.len = last - uri;
            u->uri.data = uri;
        }
    }

    إذا (len == 0) {
        u->err = "no path in the unix domain socket";
        أرجع NGX_ERROR;
    }

    u->host.len = len++;
    u->host.data = path;

    إذا (len > حجم(saun->sun_path)) {
        u->err = "too long path in the unix domain socket";
        أرجع NGX_ERROR;
    }

    u->socklen = حجم(بنية sockaddr_un);
    saun = (بنية sockaddr_un *) &u->sockaddr;
    saun->sun_family = AF_UNIX;
    (فراغ) ngx_cpystrn((u_char *) saun->sun_path, path, len);

    u->addrs = ngx_pcalloc(pool, حجم(ngx_addr_t));
    إذا (u->addrs == عدم) {
        أرجع NGX_ERROR;
    }

    saun = ngx_pcalloc(pool, حجم(بنية sockaddr_un));
    إذا (saun == عدم) {
        أرجع NGX_ERROR;
    }

    u->family = AF_UNIX;
    u->naddrs = 1;

    saun->sun_family = AF_UNIX;
    (فراغ) ngx_cpystrn((u_char *) saun->sun_path, path, len);

    u->addrs[0].sockaddr = (بنية sockaddr *) saun;
    u->addrs[0].socklen = حجم(بنية sockaddr_un);
    u->addrs[0].name.len = len + 4;
    u->addrs[0].name.data = u->url.data;

    أرجع NGX_OK;

#وإلا

    u->err = "the unix domain sockets are not supported on this platform";

    أرجع NGX_ERROR;

#endif
}


ساكن ngx_int_t
ngx_parse_inet_url(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char              *host, *port, *last, *uri, *وسائط, *dash;
    size_t               len;
    ngx_int_t            n;
    بنية sockaddr_in  *sin;

    u->socklen = حجم(بنية sockaddr_in);
    sin = (بنية sockaddr_in *) &u->sockaddr;
    sin->sin_family = AF_INET;

    u->family = AF_INET;

    host = u->url.data;

    last = host + u->url.len;

    port = ngx_strlchr(host, last, ':');

    uri = ngx_strlchr(host, last, '/');

    وسائط = ngx_strlchr(host, last, '?');

    إذا (وسائط) {
        إذا (uri == عدم || وسائط < uri) {
            uri = وسائط;
        }
    }

    إذا (uri) {
        إذا (u->listen || !u->uri_part) {
            u->err = "invalid host";
            أرجع NGX_ERROR;
        }

        u->uri.len = last - uri;
        u->uri.data = uri;

        last = uri;

        إذا (uri < port) {
            port = عدم;
        }
    }

    إذا (port) {
        port++;

        len = last - port;

        إذا (u->listen) {
            dash = ngx_strlchr(port, last, '-');

            إذا (dash) {
                dash++;

                n = ngx_atoi(dash, last - dash);

                إذا (n < 1 || n > 65535) {
                    u->err = "invalid port";
                    أرجع NGX_ERROR;
                }

                u->last_port = (in_port_t) n;

                len = dash - port - 1;
            }
        }

        n = ngx_atoi(port, len);

        إذا (n < 1 || n > 65535) {
            u->err = "invalid port";
            أرجع NGX_ERROR;
        }

        إذا (u->last_port && n > u->last_port) {
            u->err = "invalid port range";
            أرجع NGX_ERROR;
        }

        u->port = (in_port_t) n;
        sin->sin_port = htons((in_port_t) n);

        u->port_text.len = last - port;
        u->port_text.data = port;

        last = port - 1;

    } وإلا {
        إذا (uri == عدم) {

            إذا (u->listen) {

                /* test value as port only */

                len = last - host;

                dash = ngx_strlchr(host, last, '-');

                إذا (dash) {
                    dash++;

                    n = ngx_atoi(dash, last - dash);

                    إذا (n == NGX_ERROR) {
                        اذهب no_port;
                    }

                    إذا (n < 1 || n > 65535) {
                        u->err = "invalid port";

                    } وإلا {
                        u->last_port = (in_port_t) n;
                    }

                    len = dash - host - 1;
                }

                n = ngx_atoi(host, len);

                إذا (n != NGX_ERROR) {

                    إذا (u->err) {
                        أرجع NGX_ERROR;
                    }

                    إذا (n < 1 || n > 65535) {
                        u->err = "invalid port";
                        أرجع NGX_ERROR;
                    }

                    إذا (u->last_port && n > u->last_port) {
                        u->err = "invalid port range";
                        أرجع NGX_ERROR;
                    }

                    u->port = (in_port_t) n;
                    sin->sin_port = htons((in_port_t) n);
                    sin->sin_addr.s_addr = INADDR_ANY;

                    u->port_text.len = last - host;
                    u->port_text.data = host;

                    u->wildcard = 1;

                    أرجع ngx_inet_add_addr(pool, u, &u->sockaddr.sockaddr,
                                             u->socklen, 1);
                }
            }
        }

no_port:

        u->err = عدم;
        u->no_port = 1;
        u->port = u->default_port;
        sin->sin_port = htons(u->default_port);
        u->last_port = 0;
    }

    len = last - host;

    إذا (len == 0) {
        u->err = "no host";
        أرجع NGX_ERROR;
    }

    u->host.len = len;
    u->host.data = host;

    إذا (u->listen && len == 1 && *host == '*') {
        sin->sin_addr.s_addr = INADDR_ANY;
        u->wildcard = 1;
        أرجع ngx_inet_add_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);
    }

    sin->sin_addr.s_addr = ngx_inet_addr(host, len);

    إذا (sin->sin_addr.s_addr != INADDR_NONE) {

        إذا (sin->sin_addr.s_addr == INADDR_ANY) {
            u->wildcard = 1;
        }

        أرجع ngx_inet_add_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);
    }

    إذا (u->no_resolve) {
        أرجع NGX_OK;
    }

    إذا (ngx_inet_resolve_host(pool, u) != NGX_OK) {
        أرجع NGX_ERROR;
    }

    u->family = u->addrs[0].sockaddr->sa_family;
    u->socklen = u->addrs[0].socklen;
    ngx_memcpy(&u->sockaddr, u->addrs[0].sockaddr, u->addrs[0].socklen);
    u->wildcard = ngx_inet_wildcard(&u->sockaddr.sockaddr);

    أرجع NGX_OK;
}


ساكن ngx_int_t
ngx_parse_inet6_url(ngx_pool_t *pool, ngx_url_t *u)
{
#إذا (NGX_HAVE_INET6)
    u_char               *p, *host, *port, *last, *uri, *dash;
    size_t                len;
    ngx_int_t             n;
    بنية sockaddr_in6  *sin6;

    u->socklen = حجم(بنية sockaddr_in6);
    sin6 = (بنية sockaddr_in6 *) &u->sockaddr;
    sin6->sin6_family = AF_INET6;

    host = u->url.data + 1;

    last = u->url.data + u->url.len;

    p = ngx_strlchr(host, last, ']');

    إذا (p == عدم) {
        u->err = "invalid host";
        أرجع NGX_ERROR;
    }

    port = p + 1;

    uri = ngx_strlchr(port, last, '/');

    إذا (uri) {
        إذا (u->listen || !u->uri_part) {
            u->err = "invalid host";
            أرجع NGX_ERROR;
        }

        u->uri.len = last - uri;
        u->uri.data = uri;

        last = uri;
    }

    إذا (port < last) {
        إذا (*port != ':') {
            u->err = "invalid host";
            أرجع NGX_ERROR;
        }

        port++;

        len = last - port;

        إذا (u->listen) {
            dash = ngx_strlchr(port, last, '-');

            إذا (dash) {
                dash++;

                n = ngx_atoi(dash, last - dash);

                إذا (n < 1 || n > 65535) {
                    u->err = "invalid port";
                    أرجع NGX_ERROR;
                }

                u->last_port = (in_port_t) n;

                len = dash - port - 1;
            }
        }

        n = ngx_atoi(port, len);

        إذا (n < 1 || n > 65535) {
            u->err = "invalid port";
            أرجع NGX_ERROR;
        }

        إذا (u->last_port && n > u->last_port) {
            u->err = "invalid port range";
            أرجع NGX_ERROR;
        }

        u->port = (in_port_t) n;
        sin6->sin6_port = htons((in_port_t) n);

        u->port_text.len = last - port;
        u->port_text.data = port;

    } وإلا {
        u->no_port = 1;
        u->port = u->default_port;
        sin6->sin6_port = htons(u->default_port);
    }

    len = p - host;

    إذا (len == 0) {
        u->err = "no host";
        أرجع NGX_ERROR;
    }

    u->host.len = len + 2;
    u->host.data = host - 1;

    إذا (ngx_inet6_addr(host, len, sin6->sin6_addr.s6_addr) != NGX_OK) {
        u->err = "invalid IPv6 address";
        أرجع NGX_ERROR;
    }

    إذا (IN6_IS_ADDR_UNSPECIFIED(&sin6->sin6_addr)) {
        u->wildcard = 1;
    }

    u->family = AF_INET6;

    أرجع ngx_inet_add_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);

#وإلا

    u->err = "the INET6 sockets are not supported on this platform";

    أرجع NGX_ERROR;

#endif
}


#إذا (NGX_HAVE_GETADDRINFO && NGX_HAVE_INET6)

ngx_int_t
ngx_inet_resolve_host(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char           *host;
    ngx_uint_t        n;
    بنية addrinfo   hints, *res, *rp;

    host = ngx_alloc(u->host.len + 1, pool->log);
    إذا (host == عدم) {
        أرجع NGX_ERROR;
    }

    (فراغ) ngx_cpystrn(host, u->host.data, u->host.len + 1);

    ngx_memzero(&hints, حجم(بنية addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
#ifdef AI_ADDRCONFIG
    hints.ai_flags = AI_ADDRCONFIG;
#endif

    إذا (getaddrinfo((حرف *) host, عدم, &hints, &res) != 0) {
        u->err = "host not found";
        ngx_free(host);
        أرجع NGX_ERROR;
    }

    ngx_free(host);

    لكل (n = 0, rp = res; rp != عدم; rp = rp->ai_next) {

        بدل (rp->ai_family) {

        حالة AF_INET:
        حالة AF_INET6:
            اكسر;

        افتراضي:
            استمر;
        }

        n++;
    }

    إذا (n == 0) {
        u->err = "host not found";
        اذهب failed;
    }

    /* MP: ngx_shared_palloc() */

    لكل (rp = res; rp != عدم; rp = rp->ai_next) {

        بدل (rp->ai_family) {

        حالة AF_INET:
        حالة AF_INET6:
            اكسر;

        افتراضي:
            استمر;
        }

        إذا (ngx_inet_add_addr(pool, u, rp->ai_addr, rp->ai_addrlen, n)
            != NGX_OK)
        {
            اذهب failed;
        }
    }

    freeaddrinfo(res);
    أرجع NGX_OK;

failed:

    freeaddrinfo(res);
    أرجع NGX_ERROR;
}

#وإلا /* !NGX_HAVE_GETADDRINFO || !NGX_HAVE_INET6 */

ngx_int_t
ngx_inet_resolve_host(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char              *host;
    ngx_uint_t           i, n;
    بنية hostent      *h;
    بنية sockaddr_in   sin;

    /* AF_INET only */

    ngx_memzero(&sin, حجم(بنية sockaddr_in));

    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = ngx_inet_addr(u->host.data, u->host.len);

    إذا (sin.sin_addr.s_addr == INADDR_NONE) {
        host = ngx_alloc(u->host.len + 1, pool->log);
        إذا (host == عدم) {
            أرجع NGX_ERROR;
        }

        (فراغ) ngx_cpystrn(host, u->host.data, u->host.len + 1);

        h = gethostbyname((حرف *) host);

        ngx_free(host);

        إذا (h == عدم || h->h_addr_list[0] == عدم) {
            u->err = "host not found";
            أرجع NGX_ERROR;
        }

        لكل (n = 0; h->h_addr_list[n] != عدم; n++) { /* void */ }

        /* MP: ngx_shared_palloc() */

        لكل (i = 0; i < n; i++) {
            sin.sin_addr.s_addr = *(in_addr_t *) (h->h_addr_list[i]);

            إذا (ngx_inet_add_addr(pool, u, (بنية sockaddr *) &sin,
                                  حجم(بنية sockaddr_in), n)
                != NGX_OK)
            {
                أرجع NGX_ERROR;
            }
        }

    } وإلا {

        /* MP: ngx_shared_palloc() */

        إذا (ngx_inet_add_addr(pool, u, (بنية sockaddr *) &sin,
                              حجم(بنية sockaddr_in), 1)
            != NGX_OK)
        {
            أرجع NGX_ERROR;
        }
    }

    أرجع NGX_OK;
}

#endif /* NGX_HAVE_GETADDRINFO && NGX_HAVE_INET6 */


ساكن ngx_int_t
ngx_inet_add_addr(ngx_pool_t *pool, ngx_url_t *u, بنية sockaddr *sockaddr,
    socklen_t socklen, ngx_uint_t total)
{
    u_char           *p;
    size_t            len;
    ngx_uint_t        i, nports;
    ngx_addr_t       *addr;
    بنية sockaddr  *sa;

    nports = u->last_port ? u->last_port - u->port + 1 : 1;

    إذا (u->addrs == عدم) {
        u->addrs = ngx_palloc(pool, total * nports * حجم(ngx_addr_t));
        إذا (u->addrs == عدم) {
            أرجع NGX_ERROR;
        }
    }

    لكل (i = 0; i < nports; i++) {
        sa = ngx_pcalloc(pool, socklen);
        إذا (sa == عدم) {
            أرجع NGX_ERROR;
        }

        ngx_memcpy(sa, sockaddr, socklen);

        ngx_inet_set_port(sa, u->port + i);

        بدل (sa->sa_family) {

#إذا (NGX_HAVE_INET6)
        حالة AF_INET6:
            len = NGX_INET6_ADDRSTRLEN + حجم("[]:65536") - 1;
            اكسر;
#endif

        افتراضي: /* AF_INET */
            len = NGX_INET_ADDRSTRLEN + حجم(":65535") - 1;
        }

        p = ngx_pnalloc(pool, len);
        إذا (p == عدم) {
            أرجع NGX_ERROR;
        }

        len = ngx_sock_ntop(sa, socklen, p, len, 1);

        addr = &u->addrs[u->naddrs++];

        addr->sockaddr = sa;
        addr->socklen = socklen;

        addr->name.len = len;
        addr->name.data = p;
    }

    أرجع NGX_OK;
}


ngx_int_t
ngx_cmp_sockaddr(بنية sockaddr *sa1, socklen_t slen1,
    بنية sockaddr *sa2, socklen_t slen2, ngx_uint_t cmp_port)
{
    بنية sockaddr_in   *sin1, *sin2;
#إذا (NGX_HAVE_INET6)
    بنية sockaddr_in6  *sin61, *sin62;
#endif
#إذا (NGX_HAVE_UNIX_DOMAIN)
    size_t                len;
    بنية sockaddr_un   *saun1, *saun2;
#endif

    إذا (sa1->sa_family != sa2->sa_family) {
        أرجع NGX_DECLINED;
    }

    بدل (sa1->sa_family) {

#إذا (NGX_HAVE_INET6)
    حالة AF_INET6:

        sin61 = (بنية sockaddr_in6 *) sa1;
        sin62 = (بنية sockaddr_in6 *) sa2;

        إذا (cmp_port && sin61->sin6_port != sin62->sin6_port) {
            أرجع NGX_DECLINED;
        }

        إذا (ngx_memcmp(&sin61->sin6_addr, &sin62->sin6_addr, 16) != 0) {
            أرجع NGX_DECLINED;
        }

        اكسر;
#endif

#إذا (NGX_HAVE_UNIX_DOMAIN)
    حالة AF_UNIX:

        saun1 = (بنية sockaddr_un *) sa1;
        saun2 = (بنية sockaddr_un *) sa2;

        إذا (slen1 < slen2) {
            len = slen1 - offsetof(بنية sockaddr_un, sun_path);

        } وإلا {
            len = slen2 - offsetof(بنية sockaddr_un, sun_path);
        }

        إذا (len > حجم(saun1->sun_path)) {
            len = حجم(saun1->sun_path);
        }

        إذا (ngx_memcmp(&saun1->sun_path, &saun2->sun_path, len) != 0) {
            أرجع NGX_DECLINED;
        }

        اكسر;
#endif

    افتراضي: /* AF_INET */

        sin1 = (بنية sockaddr_in *) sa1;
        sin2 = (بنية sockaddr_in *) sa2;

        إذا (cmp_port && sin1->sin_port != sin2->sin_port) {
            أرجع NGX_DECLINED;
        }

        إذا (sin1->sin_addr.s_addr != sin2->sin_addr.s_addr) {
            أرجع NGX_DECLINED;
        }

        اكسر;
    }

    أرجع NGX_OK;
}


in_port_t
ngx_inet_get_port(بنية sockaddr *sa)
{
    بنية sockaddr_in   *sin;
#إذا (NGX_HAVE_INET6)
    بنية sockaddr_in6  *sin6;
#endif

    بدل (sa->sa_family) {

#إذا (NGX_HAVE_INET6)
    حالة AF_INET6:
        sin6 = (بنية sockaddr_in6 *) sa;
        أرجع ntohs(sin6->sin6_port);
#endif

#إذا (NGX_HAVE_UNIX_DOMAIN)
    حالة AF_UNIX:
        أرجع 0;
#endif

    افتراضي: /* AF_INET */
        sin = (بنية sockaddr_in *) sa;
        أرجع ntohs(sin->sin_port);
    }
}


فراغ
ngx_inet_set_port(بنية sockaddr *sa, in_port_t port)
{
    بنية sockaddr_in   *sin;
#إذا (NGX_HAVE_INET6)
    بنية sockaddr_in6  *sin6;
#endif

    بدل (sa->sa_family) {

#إذا (NGX_HAVE_INET6)
    حالة AF_INET6:
        sin6 = (بنية sockaddr_in6 *) sa;
        sin6->sin6_port = htons(port);
        اكسر;
#endif

#إذا (NGX_HAVE_UNIX_DOMAIN)
    حالة AF_UNIX:
        اكسر;
#endif

    افتراضي: /* AF_INET */
        sin = (بنية sockaddr_in *) sa;
        sin->sin_port = htons(port);
        اكسر;
    }
}


ngx_uint_t
ngx_inet_wildcard(بنية sockaddr *sa)
{
    بنية sockaddr_in   *sin;
#إذا (NGX_HAVE_INET6)
    بنية sockaddr_in6  *sin6;
#endif

    بدل (sa->sa_family) {

    حالة AF_INET:
        sin = (بنية sockaddr_in *) sa;

        إذا (sin->sin_addr.s_addr == INADDR_ANY) {
            أرجع 1;
        }

        اكسر;

#إذا (NGX_HAVE_INET6)

    حالة AF_INET6:
        sin6 = (بنية sockaddr_in6 *) sa;

        إذا (IN6_IS_ADDR_UNSPECIFIED(&sin6->sin6_addr)) {
            أرجع 1;
        }

        اكسر;

#endif
    }

    أرجع 0;
}
