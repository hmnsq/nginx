
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


ساكن ngx_int_t ngx_parse_unix_domain_url(ngx_pool_t *pool, ngx_url_t *u);
ساكن ngx_int_t ngx_parse_inet_url(ngx_pool_t *pool, ngx_url_t *u);
ساكن ngx_int_t ngx_parse_inet6_url(ngx_pool_t *pool, ngx_url_t *u);
ساكن ngx_int_t ngx_inet_جمع_کړه_addr(ngx_pool_t *pool, ngx_url_t *u,
    بنية sockaddr *sockaddr, socklen_t socklen, ngx_uint_t ټول);


دننه_addr_t
ngx_inet_addr(u_char *متن, اندازه_t len)
{
    u_char      *p, c;
    دننه_addr_t    addr;
    ngx_uint_t   octet, n;

    addr = 0;
    octet = 0;
    n = 0;

    لكل (p = متن; p < متن + len; p++) {
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
ngx_inet6_addr(u_char *p, اندازه_t len, u_char *addr)
{
    u_char      c, *zero, *digit, *s, *d;
    اندازه_t      len4;
    ngx_uint_t  n, nibbles, کلمه;

    إذا (len == 0) {
        أرجع NGX_ERROR;
    }

    zero = عدم;
    digit = عدم;
    len4 = 0;
    nibbles = 0;
    کلمه = 0;
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
                *addr++ = (u_char) (کلمه >> 8);
                *addr++ = (u_char) (کلمه & 0xff);

                إذا (--n) {
                    nibbles = 0;
                    کلمه = 0;
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

            کلمه = ngx_inet_addr(digit, len4 - 1);
            إذا (کلمه == INADDR_NONE) {
                أرجع NGX_ERROR;
            }

            کلمه = ntohl(کلمه);
            *addr++ = (u_char) ((کلمه >> 24) & 0xff);
            *addr++ = (u_char) ((کلمه >> 16) & 0xff);
            n--;
            اكسر;
        }

        إذا (++nibbles > 4) {
            أرجع NGX_ERROR;
        }

        إذا (c >= '0' && c <= '9') {
            کلمه = کلمه * 16 + (c - '0');
            استمر;
        }

        c |= 0x20;

        إذا (c >= 'a' && c <= 'f') {
            کلمه = کلمه * 16 + (c - 'a') + 10;
            استمر;
        }

        أرجع NGX_ERROR;
    }

    إذا (nibbles == 0 && zero == عدم) {
        أرجع NGX_ERROR;
    }

    *addr++ = (u_char) (کلمه >> 8);
    *addr++ = (u_char) (کلمه & 0xff);

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

#د_که_پای


اندازه_t
ngx_sock_ntop(بنية sockaddr *sa, socklen_t socklen, u_char *متن, اندازه_t len,
    ngx_uint_t پورټ)
{
    u_char               *p;
#إذا (NGX_HAVE_INET6 || NGX_HAVE_UNIX_DOMAIN)
    اندازه_t                n;
#د_که_پای
    بنية sockaddr_دننه   *ساین;
#إذا (NGX_HAVE_INET6)
    بنية sockaddr_in6  *sin6;
#د_که_پای
#إذا (NGX_HAVE_UNIX_DOMAIN)
    بنية sockaddr_un   *saun;
#د_که_پای

    بدل (sa->sa_family) {

    حالة AF_INET:

        ساین = (بنية sockaddr_دننه *) sa;
        p = (u_char *) &ساین->sin_addr;

        إذا (پورټ) {
            p = ngx_snprintf(متن, len, "%ud.%ud.%ud.%ud:%d",
                             p[0], p[1], p[2], p[3], ntohs(ساین->sin_پورټ));
        } وإلا {
            p = ngx_snprintf(متن, len, "%ud.%ud.%ud.%ud",
                             p[0], p[1], p[2], p[3]);
        }

        أرجع (p - متن);

#إذا (NGX_HAVE_INET6)

    حالة AF_INET6:

        sin6 = (بنية sockaddr_in6 *) sa;

        n = 0;

        إذا (پورټ) {
            متن[n++] = '[';
        }

        n = ngx_inet6_ntop(sin6->sin6_addr.s6_addr, &متن[n], len);

        إذا (پورټ) {
            n = ngx_sprintf(&متن[1 + n], "]:%d",
                            ntohs(sin6->sin6_پورټ)) - متن;
        }

        أرجع n;
#د_که_پای

#إذا (NGX_HAVE_UNIX_DOMAIN)

    حالة AF_UNIX:
        saun = (بنية sockaddr_un *) sa;

        /* on Linux sockaddr might not include sun_path at all */

        إذا (socklen <= (socklen_t) offsetof(بنية sockaddr_un, sun_لار)) {
            p = ngx_snprintf(متن, len, "unix:%Z");

        } وإلا {
            n = ngx_strnlen((u_char *) saun->sun_لار,
                            socklen - offsetof(بنية sockaddr_un, sun_لار));
            p = ngx_snprintf(متن, len, "unix:%*s%Z", n, saun->sun_لار);
        }

        /* we do not include trailing zero in address length */

        أرجع (p - متن - 1);

#د_که_پای

    افتراضي:
        أرجع 0;
    }
}


اندازه_t
ngx_inet_ntop(صحيح family, فراغ *addr, u_char *متن, اندازه_t len)
{
    u_char  *p;

    بدل (family) {

    حالة AF_INET:

        p = addr;

        أرجع ngx_snprintf(متن, len, "%ud.%ud.%ud.%ud",
                            p[0], p[1], p[2], p[3])
               - متن;

#إذا (NGX_HAVE_INET6)

    حالة AF_INET6:
        أرجع ngx_inet6_ntop(addr, متن, len);

#د_که_پای

    افتراضي:
        أرجع 0;
    }
}


#إذا (NGX_HAVE_INET6)

اندازه_t
ngx_inet6_ntop(u_char *p, u_char *متن, اندازه_t len)
{
    u_char      *dst;
    اندازه_t       max, n;
    ngx_uint_t   i, zero, وروستی;

    إذا (len < NGX_INET6_ADDRSTRLEN) {
        أرجع 0;
    }

    zero = (ngx_uint_t) -1;
    وروستی = (ngx_uint_t) -1;
    max = 1;
    n = 0;

    لكل (i = 0; i < 16; i += 2) {

        إذا (p[i] || p[i + 1]) {

            إذا (max < n) {
                zero = وروستی;
                max = n;
            }

            n = 0;
            استمر;
        }

        إذا (n++ == 0) {
            وروستی = i;
        }
    }

    إذا (max < n) {
        zero = وروستی;
        max = n;
    }

    dst = متن;
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

    أرجع dst - متن;
}

#د_که_پای


ngx_int_t
ngx_ptocidr(ngx_str_t *متن, ngx_cidr_t *cidr)
{
    u_char      *addr, *ماسک, *وروستی;
    اندازه_t       len;
    ngx_int_t    ښويدنه;
#إذا (NGX_HAVE_INET6)
    ngx_int_t    rc;
    ngx_uint_t   s, i;
#د_که_پای

    addr = متن->معلومات;
    وروستی = addr + متن->len;

    ماسک = ngx_strlchr(addr, وروستی, '/');
    len = (ماسک ? ماسک : وروستی) - addr;

    cidr->u.دننه.addr = ngx_inet_addr(addr, len);

    إذا (cidr->u.دننه.addr != INADDR_NONE) {
        cidr->family = AF_INET;

        إذا (ماسک == عدم) {
            cidr->u.دننه.ماسک = 0xffffffff;
            أرجع NGX_OK;
        }

#إذا (NGX_HAVE_INET6)
    } وإلا إذا (ngx_inet6_addr(addr, len, cidr->u.in6.addr.s6_addr) == NGX_OK) {
        cidr->family = AF_INET6;

        إذا (ماسک == عدم) {
            ngx_memset(cidr->u.in6.ماسک.s6_addr, 0xff, 16);
            أرجع NGX_OK;
        }

#د_که_پای
    } وإلا {
        أرجع NGX_ERROR;
    }

    ماسک++;

    ښويدنه = ngx_atoi(ماسک, وروستی - ماسک);
    إذا (ښويدنه == NGX_ERROR) {
        أرجع NGX_ERROR;
    }

    بدل (cidr->family) {

#إذا (NGX_HAVE_INET6)
    حالة AF_INET6:
        إذا (ښويدنه > 128) {
            أرجع NGX_ERROR;
        }

        addr = cidr->u.in6.addr.s6_addr;
        ماسک = cidr->u.in6.ماسک.s6_addr;
        rc = NGX_OK;

        لكل (i = 0; i < 16; i++) {

            s = (ښويدنه > 8) ? 8 : ښويدنه;
            ښويدنه -= s;

            ماسک[i] = (u_char) (0xffu << (8 - s));

            إذا (addr[i] != (addr[i] & ماسک[i])) {
                rc = NGX_بشپړ;
                addr[i] &= ماسک[i];
            }
        }

        أرجع rc;
#د_که_پای

    افتراضي: /* AF_INET */
        إذا (ښويدنه > 32) {
            أرجع NGX_ERROR;
        }

        إذا (ښويدنه) {
            cidr->u.دننه.ماسک = htonl((uint32_t) (0xffffffffu << (32 - ښويدنه)));

        } وإلا {
            /* x86 compilers use a shl instruction that shifts by modulo 32 */
            cidr->u.دننه.ماسک = 0;
        }

        إذا (cidr->u.دننه.addr == (cidr->u.دننه.addr & cidr->u.دننه.ماسک)) {
            أرجع NGX_OK;
        }

        cidr->u.دننه.addr &= cidr->u.دننه.ماسک;

        أرجع NGX_بشپړ;
    }
}


ngx_int_t
ngx_cidr_سمون(بنية sockaddr *sa, ngx_array_t *cidrs)
{
#إذا (NGX_HAVE_INET6)
    u_char           *p;
#د_که_پای
    دننه_addr_t         inaddr;
    ngx_cidr_t       *cidr;
    ngx_uint_t        family, i;
#إذا (NGX_HAVE_INET6)
    ngx_uint_t        n;
    بنية in6_addr  *inaddr6;
#د_که_پای

#إذا (NGX_SUPPRESS_WARN)
    inaddr = 0;
#إذا (NGX_HAVE_INET6)
    inaddr6 = عدم;
#د_که_پای
#د_که_پای

    family = sa->sa_family;

    إذا (family == AF_INET) {
        inaddr = ((بنية sockaddr_دننه *) sa)->sin_addr.s_addr;
    }

#إذا (NGX_HAVE_INET6)
    وإلا إذا (family == AF_INET6) {
        inaddr6 = &((بنية sockaddr_in6 *) sa)->sin6_addr;

        إذا (IN6_IS_ADDR_V4MAPPED(inaddr6)) {
            family = AF_INET;

            p = inaddr6->s6_addr;

            inaddr = (دننه_addr_t) p[12] << 24;
            inaddr += p[13] << 16;
            inaddr += p[14] << 8;
            inaddr += p[15];

            inaddr = htonl(inaddr);
        }
    }
#د_که_پای

    لكل (cidr = cidrs->elts, i = 0; i < cidrs->nelts; i++) {
        إذا (cidr[i].family != family) {
            اذهب بل;
        }

        بدل (family) {

#إذا (NGX_HAVE_INET6)
        حالة AF_INET6:
            لكل (n = 0; n < 16; n++) {
                إذا ((inaddr6->s6_addr[n] & cidr[i].u.in6.ماسک.s6_addr[n])
                    != cidr[i].u.in6.addr.s6_addr[n])
                {
                    اذهب بل;
                }
            }
            اكسر;
#د_که_پای

#إذا (NGX_HAVE_UNIX_DOMAIN)
        حالة AF_UNIX:
            اكسر;
#د_که_پای

        افتراضي: /* AF_INET */
            إذا ((inaddr & cidr[i].u.دننه.ماسک) != cidr[i].u.دننه.addr) {
                اذهب بل;
            }
            اكسر;
        }

        أرجع NGX_OK;

    بل:
        استمر;
    }

    أرجع NGX_DECLINED;
}


ngx_int_t
ngx_parse_addr(ngx_pool_t *pool, ngx_addr_t *addr, u_char *متن, اندازه_t len)
{
    دننه_addr_t             inaddr;
    ngx_uint_t            family;
    بنية sockaddr_دننه   *ساین;
#إذا (NGX_HAVE_INET6)
    بنية in6_addr       inaddr6;
    بنية sockaddr_in6  *sin6;

    /*
     * prevent MSVC8 warning:
     *    potentially uninitialized local variable 'inaddr6' used
     */
    ngx_memzero(&inaddr6, حجم(بنية in6_addr));
#د_که_پای

    inaddr = ngx_inet_addr(متن, len);

    إذا (inaddr != INADDR_NONE) {
        family = AF_INET;
        len = حجم(بنية sockaddr_دننه);

#إذا (NGX_HAVE_INET6)
    } وإلا إذا (ngx_inet6_addr(متن, len, inaddr6.s6_addr) == NGX_OK) {
        family = AF_INET6;
        len = حجم(بنية sockaddr_in6);

#د_که_پای
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
#د_که_پای

    افتراضي: /* AF_INET */
        ساین = (بنية sockaddr_دننه *) addr->sockaddr;
        ساین->sin_addr.s_addr = inaddr;
        اكسر;
    }

    أرجع NGX_OK;
}


ngx_int_t
ngx_parse_addr_پورټ(ngx_pool_t *pool, ngx_addr_t *addr, u_char *متن,
    اندازه_t len)
{
    u_char     *p, *وروستی;
    اندازه_t      plen;
    ngx_int_t   rc, پورټ;

    rc = ngx_parse_addr(pool, addr, متن, len);

    إذا (rc != NGX_DECLINED) {
        أرجع rc;
    }

    وروستی = متن + len;

#إذا (NGX_HAVE_INET6)
    إذا (len && متن[0] == '[') {

        p = ngx_strlchr(متن, وروستی, ']');

        إذا (p == وروستی - 1) {
            أرجع ngx_parse_addr(pool, addr, متن + 1, len - 2);
        }

        إذا (p == عدم || *++p != ':') {
            أرجع NGX_DECLINED;
        }

        متن++;
        len -= 2;

    } وإلا
#د_که_پای

    {
        p = ngx_strlchr(متن, وروستی, ':');

        إذا (p == عدم) {
            أرجع NGX_DECLINED;
        }
    }

    p++;
    plen = وروستی - p;

    پورټ = ngx_atoi(p, plen);

    إذا (پورټ < 1 || پورټ > 65535) {
        أرجع NGX_DECLINED;
    }

    len -= plen + 1;

    rc = ngx_parse_addr(pool, addr, متن, len);

    إذا (rc != NGX_OK) {
        أرجع rc;
    }

    ngx_inet_سېټ_پورټ(addr->sockaddr, (دننه_پورټ_t) پورټ);

    أرجع NGX_OK;
}


ngx_int_t
ngx_parse_url(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char  *p;
    اندازه_t   len;

    p = u->url.معلومات;
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
    u_char              *لار, *uri, *وروستی;
    اندازه_t               len;
    بنية sockaddr_un  *saun;

    len = u->url.len;
    لار = u->url.معلومات;

    لار += 5;
    len -= 5;

    إذا (u->uri_part) {

        وروستی = لار + len;
        uri = ngx_strlchr(لار, وروستی, ':');

        إذا (uri) {
            len = uri - لار;
            uri++;
            u->uri.len = وروستی - uri;
            u->uri.معلومات = uri;
        }
    }

    إذا (len == 0) {
        u->err = "no path in the unix domain socket";
        أرجع NGX_ERROR;
    }

    u->host.len = len++;
    u->host.معلومات = لار;

    إذا (len > حجم(saun->sun_لار)) {
        u->err = "too long path in the unix domain socket";
        أرجع NGX_ERROR;
    }

    u->socklen = حجم(بنية sockaddr_un);
    saun = (بنية sockaddr_un *) &u->sockaddr;
    saun->sun_family = AF_UNIX;
    (فراغ) ngx_cpystrn((u_char *) saun->sun_لار, لار, len);

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
    (فراغ) ngx_cpystrn((u_char *) saun->sun_لار, لار, len);

    u->addrs[0].sockaddr = (بنية sockaddr *) saun;
    u->addrs[0].socklen = حجم(بنية sockaddr_un);
    u->addrs[0].name.len = len + 4;
    u->addrs[0].name.معلومات = u->url.معلومات;

    أرجع NGX_OK;

#وإلا

    u->err = "the unix domain sockets are not supported on this platform";

    أرجع NGX_ERROR;

#د_که_پای
}


ساكن ngx_int_t
ngx_parse_inet_url(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char              *host, *پورټ, *وروستی, *uri, *دلیلونه, *dash;
    اندازه_t               len;
    ngx_int_t            n;
    بنية sockaddr_دننه  *ساین;

    u->socklen = حجم(بنية sockaddr_دننه);
    ساین = (بنية sockaddr_دننه *) &u->sockaddr;
    ساین->sin_family = AF_INET;

    u->family = AF_INET;

    host = u->url.معلومات;

    وروستی = host + u->url.len;

    پورټ = ngx_strlchr(host, وروستی, ':');

    uri = ngx_strlchr(host, وروستی, '/');

    دلیلونه = ngx_strlchr(host, وروستی, '?');

    إذا (دلیلونه) {
        إذا (uri == عدم || دلیلونه < uri) {
            uri = دلیلونه;
        }
    }

    إذا (uri) {
        إذا (u->listen || !u->uri_part) {
            u->err = "invalid host";
            أرجع NGX_ERROR;
        }

        u->uri.len = وروستی - uri;
        u->uri.معلومات = uri;

        وروستی = uri;

        إذا (uri < پورټ) {
            پورټ = عدم;
        }
    }

    إذا (پورټ) {
        پورټ++;

        len = وروستی - پورټ;

        إذا (u->listen) {
            dash = ngx_strlchr(پورټ, وروستی, '-');

            إذا (dash) {
                dash++;

                n = ngx_atoi(dash, وروستی - dash);

                إذا (n < 1 || n > 65535) {
                    u->err = "invalid port";
                    أرجع NGX_ERROR;
                }

                u->وروستی_پورټ = (دننه_پورټ_t) n;

                len = dash - پورټ - 1;
            }
        }

        n = ngx_atoi(پورټ, len);

        إذا (n < 1 || n > 65535) {
            u->err = "invalid port";
            أرجع NGX_ERROR;
        }

        إذا (u->وروستی_پورټ && n > u->وروستی_پورټ) {
            u->err = "invalid port range";
            أرجع NGX_ERROR;
        }

        u->پورټ = (دننه_پورټ_t) n;
        ساین->sin_پورټ = htons((دننه_پورټ_t) n);

        u->پورټ_متن.len = وروستی - پورټ;
        u->پورټ_متن.معلومات = پورټ;

        وروستی = پورټ - 1;

    } وإلا {
        إذا (uri == عدم) {

            إذا (u->listen) {

                /* test value as port only */

                len = وروستی - host;

                dash = ngx_strlchr(host, وروستی, '-');

                إذا (dash) {
                    dash++;

                    n = ngx_atoi(dash, وروستی - dash);

                    إذا (n == NGX_ERROR) {
                        اذهب no_پورټ;
                    }

                    إذا (n < 1 || n > 65535) {
                        u->err = "invalid port";

                    } وإلا {
                        u->وروستی_پورټ = (دننه_پورټ_t) n;
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

                    إذا (u->وروستی_پورټ && n > u->وروستی_پورټ) {
                        u->err = "invalid port range";
                        أرجع NGX_ERROR;
                    }

                    u->پورټ = (دننه_پورټ_t) n;
                    ساین->sin_پورټ = htons((دننه_پورټ_t) n);
                    ساین->sin_addr.s_addr = INADDR_ANY;

                    u->پورټ_متن.len = وروستی - host;
                    u->پورټ_متن.معلومات = host;

                    u->wildcard = 1;

                    أرجع ngx_inet_جمع_کړه_addr(pool, u, &u->sockaddr.sockaddr,
                                             u->socklen, 1);
                }
            }
        }

no_پورټ:

        u->err = عدم;
        u->no_پورټ = 1;
        u->پورټ = u->default_پورټ;
        ساین->sin_پورټ = htons(u->default_پورټ);
        u->وروستی_پورټ = 0;
    }

    len = وروستی - host;

    إذا (len == 0) {
        u->err = "no host";
        أرجع NGX_ERROR;
    }

    u->host.len = len;
    u->host.معلومات = host;

    إذا (u->listen && len == 1 && *host == '*') {
        ساین->sin_addr.s_addr = INADDR_ANY;
        u->wildcard = 1;
        أرجع ngx_inet_جمع_کړه_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);
    }

    ساین->sin_addr.s_addr = ngx_inet_addr(host, len);

    إذا (ساین->sin_addr.s_addr != INADDR_NONE) {

        إذا (ساین->sin_addr.s_addr == INADDR_ANY) {
            u->wildcard = 1;
        }

        أرجع ngx_inet_جمع_کړه_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);
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
    u_char               *p, *host, *پورټ, *وروستی, *uri, *dash;
    اندازه_t                len;
    ngx_int_t             n;
    بنية sockaddr_in6  *sin6;

    u->socklen = حجم(بنية sockaddr_in6);
    sin6 = (بنية sockaddr_in6 *) &u->sockaddr;
    sin6->sin6_family = AF_INET6;

    host = u->url.معلومات + 1;

    وروستی = u->url.معلومات + u->url.len;

    p = ngx_strlchr(host, وروستی, ']');

    إذا (p == عدم) {
        u->err = "invalid host";
        أرجع NGX_ERROR;
    }

    پورټ = p + 1;

    uri = ngx_strlchr(پورټ, وروستی, '/');

    إذا (uri) {
        إذا (u->listen || !u->uri_part) {
            u->err = "invalid host";
            أرجع NGX_ERROR;
        }

        u->uri.len = وروستی - uri;
        u->uri.معلومات = uri;

        وروستی = uri;
    }

    إذا (پورټ < وروستی) {
        إذا (*پورټ != ':') {
            u->err = "invalid host";
            أرجع NGX_ERROR;
        }

        پورټ++;

        len = وروستی - پورټ;

        إذا (u->listen) {
            dash = ngx_strlchr(پورټ, وروستی, '-');

            إذا (dash) {
                dash++;

                n = ngx_atoi(dash, وروستی - dash);

                إذا (n < 1 || n > 65535) {
                    u->err = "invalid port";
                    أرجع NGX_ERROR;
                }

                u->وروستی_پورټ = (دننه_پورټ_t) n;

                len = dash - پورټ - 1;
            }
        }

        n = ngx_atoi(پورټ, len);

        إذا (n < 1 || n > 65535) {
            u->err = "invalid port";
            أرجع NGX_ERROR;
        }

        إذا (u->وروستی_پورټ && n > u->وروستی_پورټ) {
            u->err = "invalid port range";
            أرجع NGX_ERROR;
        }

        u->پورټ = (دننه_پورټ_t) n;
        sin6->sin6_پورټ = htons((دننه_پورټ_t) n);

        u->پورټ_متن.len = وروستی - پورټ;
        u->پورټ_متن.معلومات = پورټ;

    } وإلا {
        u->no_پورټ = 1;
        u->پورټ = u->default_پورټ;
        sin6->sin6_پورټ = htons(u->default_پورټ);
    }

    len = p - host;

    إذا (len == 0) {
        u->err = "no host";
        أرجع NGX_ERROR;
    }

    u->host.len = len + 2;
    u->host.معلومات = host - 1;

    إذا (ngx_inet6_addr(host, len, sin6->sin6_addr.s6_addr) != NGX_OK) {
        u->err = "invalid IPv6 address";
        أرجع NGX_ERROR;
    }

    إذا (IN6_IS_ADDR_UNSPECIFIED(&sin6->sin6_addr)) {
        u->wildcard = 1;
    }

    u->family = AF_INET6;

    أرجع ngx_inet_جمع_کړه_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);

#وإلا

    u->err = "the INET6 sockets are not supported on this platform";

    أرجع NGX_ERROR;

#د_که_پای
}


#إذا (NGX_HAVE_GETADDRINFO && NGX_HAVE_INET6)

ngx_int_t
ngx_inet_resolve_host(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char           *host;
    ngx_uint_t        n;
    بنية addrinfo   hints, *res, *rp;

    host = ngx_alloc(u->host.len + 1, pool->لوګاریتم);
    إذا (host == عدم) {
        أرجع NGX_ERROR;
    }

    (فراغ) ngx_cpystrn(host, u->host.معلومات, u->host.len + 1);

    ngx_memzero(&hints, حجم(بنية addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_بهیر;
#که_تعریف_شوی AI_ADDRCONFIG
    hints.ai_flags = AI_ADDRCONFIG;
#د_که_پای

    إذا (getaddrinfo((حرف *) host, عدم, &hints, &res) != 0) {
        u->err = "host not found";
        ngx_free(host);
        أرجع NGX_ERROR;
    }

    ngx_free(host);

    لكل (n = 0, rp = res; rp != عدم; rp = rp->ai_بل) {

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

    لكل (rp = res; rp != عدم; rp = rp->ai_بل) {

        بدل (rp->ai_family) {

        حالة AF_INET:
        حالة AF_INET6:
            اكسر;

        افتراضي:
            استمر;
        }

        إذا (ngx_inet_جمع_کړه_addr(pool, u, rp->ai_addr, rp->ai_addrlen, n)
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
    بنية sockaddr_دننه   ساین;

    /* AF_INET only */

    ngx_memzero(&ساین, حجم(بنية sockaddr_دننه));

    ساین.sin_family = AF_INET;
    ساین.sin_addr.s_addr = ngx_inet_addr(u->host.معلومات, u->host.len);

    إذا (ساین.sin_addr.s_addr == INADDR_NONE) {
        host = ngx_alloc(u->host.len + 1, pool->لوګاریتم);
        إذا (host == عدم) {
            أرجع NGX_ERROR;
        }

        (فراغ) ngx_cpystrn(host, u->host.معلومات, u->host.len + 1);

        h = gethostbyname((حرف *) host);

        ngx_free(host);

        إذا (h == عدم || h->h_addr_list[0] == عدم) {
            u->err = "host not found";
            أرجع NGX_ERROR;
        }

        لكل (n = 0; h->h_addr_list[n] != عدم; n++) { /* void */ }

        /* MP: ngx_shared_palloc() */

        لكل (i = 0; i < n; i++) {
            ساین.sin_addr.s_addr = *(دننه_addr_t *) (h->h_addr_list[i]);

            إذا (ngx_inet_جمع_کړه_addr(pool, u, (بنية sockaddr *) &ساین,
                                  حجم(بنية sockaddr_دننه), n)
                != NGX_OK)
            {
                أرجع NGX_ERROR;
            }
        }

    } وإلا {

        /* MP: ngx_shared_palloc() */

        إذا (ngx_inet_جمع_کړه_addr(pool, u, (بنية sockaddr *) &ساین,
                              حجم(بنية sockaddr_دننه), 1)
            != NGX_OK)
        {
            أرجع NGX_ERROR;
        }
    }

    أرجع NGX_OK;
}

#د_که_پای /* NGX_HAVE_GETADDRINFO && NGX_HAVE_INET6 */


ساكن ngx_int_t
ngx_inet_جمع_کړه_addr(ngx_pool_t *pool, ngx_url_t *u, بنية sockaddr *sockaddr,
    socklen_t socklen, ngx_uint_t ټول)
{
    u_char           *p;
    اندازه_t            len;
    ngx_uint_t        i, nports;
    ngx_addr_t       *addr;
    بنية sockaddr  *sa;

    nports = u->وروستی_پورټ ? u->وروستی_پورټ - u->پورټ + 1 : 1;

    إذا (u->addrs == عدم) {
        u->addrs = ngx_palloc(pool, ټول * nports * حجم(ngx_addr_t));
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

        ngx_inet_سېټ_پورټ(sa, u->پورټ + i);

        بدل (sa->sa_family) {

#إذا (NGX_HAVE_INET6)
        حالة AF_INET6:
            len = NGX_INET6_ADDRSTRLEN + حجم("[]:65536") - 1;
            اكسر;
#د_که_پای

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
        addr->name.معلومات = p;
    }

    أرجع NGX_OK;
}


ngx_int_t
ngx_cmp_sockaddr(بنية sockaddr *sa1, socklen_t slen1,
    بنية sockaddr *sa2, socklen_t slen2, ngx_uint_t cmp_پورټ)
{
    بنية sockaddr_دننه   *sin1, *sin2;
#إذا (NGX_HAVE_INET6)
    بنية sockaddr_in6  *sin61, *sin62;
#د_که_پای
#إذا (NGX_HAVE_UNIX_DOMAIN)
    اندازه_t                len;
    بنية sockaddr_un   *saun1, *saun2;
#د_که_پای

    إذا (sa1->sa_family != sa2->sa_family) {
        أرجع NGX_DECLINED;
    }

    بدل (sa1->sa_family) {

#إذا (NGX_HAVE_INET6)
    حالة AF_INET6:

        sin61 = (بنية sockaddr_in6 *) sa1;
        sin62 = (بنية sockaddr_in6 *) sa2;

        إذا (cmp_پورټ && sin61->sin6_پورټ != sin62->sin6_پورټ) {
            أرجع NGX_DECLINED;
        }

        إذا (ngx_memcmp(&sin61->sin6_addr, &sin62->sin6_addr, 16) != 0) {
            أرجع NGX_DECLINED;
        }

        اكسر;
#د_که_پای

#إذا (NGX_HAVE_UNIX_DOMAIN)
    حالة AF_UNIX:

        saun1 = (بنية sockaddr_un *) sa1;
        saun2 = (بنية sockaddr_un *) sa2;

        إذا (slen1 < slen2) {
            len = slen1 - offsetof(بنية sockaddr_un, sun_لار);

        } وإلا {
            len = slen2 - offsetof(بنية sockaddr_un, sun_لار);
        }

        إذا (len > حجم(saun1->sun_لار)) {
            len = حجم(saun1->sun_لار);
        }

        إذا (ngx_memcmp(&saun1->sun_لار, &saun2->sun_لار, len) != 0) {
            أرجع NGX_DECLINED;
        }

        اكسر;
#د_که_پای

    افتراضي: /* AF_INET */

        sin1 = (بنية sockaddr_دننه *) sa1;
        sin2 = (بنية sockaddr_دننه *) sa2;

        إذا (cmp_پورټ && sin1->sin_پورټ != sin2->sin_پورټ) {
            أرجع NGX_DECLINED;
        }

        إذا (sin1->sin_addr.s_addr != sin2->sin_addr.s_addr) {
            أرجع NGX_DECLINED;
        }

        اكسر;
    }

    أرجع NGX_OK;
}


دننه_پورټ_t
ngx_inet_get_پورټ(بنية sockaddr *sa)
{
    بنية sockaddr_دننه   *ساین;
#إذا (NGX_HAVE_INET6)
    بنية sockaddr_in6  *sin6;
#د_که_پای

    بدل (sa->sa_family) {

#إذا (NGX_HAVE_INET6)
    حالة AF_INET6:
        sin6 = (بنية sockaddr_in6 *) sa;
        أرجع ntohs(sin6->sin6_پورټ);
#د_که_پای

#إذا (NGX_HAVE_UNIX_DOMAIN)
    حالة AF_UNIX:
        أرجع 0;
#د_که_پای

    افتراضي: /* AF_INET */
        ساین = (بنية sockaddr_دننه *) sa;
        أرجع ntohs(ساین->sin_پورټ);
    }
}


فراغ
ngx_inet_سېټ_پورټ(بنية sockaddr *sa, دننه_پورټ_t پورټ)
{
    بنية sockaddr_دننه   *ساین;
#إذا (NGX_HAVE_INET6)
    بنية sockaddr_in6  *sin6;
#د_که_پای

    بدل (sa->sa_family) {

#إذا (NGX_HAVE_INET6)
    حالة AF_INET6:
        sin6 = (بنية sockaddr_in6 *) sa;
        sin6->sin6_پورټ = htons(پورټ);
        اكسر;
#د_که_پای

#إذا (NGX_HAVE_UNIX_DOMAIN)
    حالة AF_UNIX:
        اكسر;
#د_که_پای

    افتراضي: /* AF_INET */
        ساین = (بنية sockaddr_دننه *) sa;
        ساین->sin_پورټ = htons(پورټ);
        اكسر;
    }
}


ngx_uint_t
ngx_inet_wildcard(بنية sockaddr *sa)
{
    بنية sockaddr_دننه   *ساین;
#إذا (NGX_HAVE_INET6)
    بنية sockaddr_in6  *sin6;
#د_که_پای

    بدل (sa->sa_family) {

    حالة AF_INET:
        ساین = (بنية sockaddr_دننه *) sa;

        إذا (ساین->sin_addr.s_addr == INADDR_ANY) {
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

#د_که_پای
    }

    أرجع 0;
}
