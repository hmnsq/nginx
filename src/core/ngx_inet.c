
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


ساكن ngx_int_t ngx_parse_unix_domain_url(ngx_pool_t *pool, ngx_url_t *u);
ساكن ngx_int_t ngx_parse_inet_url(ngx_pool_t *pool, ngx_url_t *u);
ساكن ngx_int_t ngx_parse_inet6_url(ngx_pool_t *pool, ngx_url_t *u);
ساكن ngx_int_t ngx_inet_جوڙيو_addr(ngx_pool_t *pool, ngx_url_t *u,
    بنية sockaddr *sockaddr, socklen_t socklen, ngx_uint_t ڪل);


اندريون_addr_t
ngx_inet_addr(u_char *متن, سائيز_t len)
{
    u_char      *p, c;
    اندريون_addr_t    addr;
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
ngx_inet6_addr(u_char *p, سائيز_t len, u_char *addr)
{
    u_char      c, *zero, *digit, *s, *d;
    سائيز_t      len4;
    ngx_uint_t  n, nibbles, لفظ;

    إذا (len == 0) {
        أرجع NGX_ERROR;
    }

    zero = عدم;
    digit = عدم;
    len4 = 0;
    nibbles = 0;
    لفظ = 0;
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
                *addr++ = (u_char) (لفظ >> 8);
                *addr++ = (u_char) (لفظ & 0xff);

                إذا (--n) {
                    nibbles = 0;
                    لفظ = 0;
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

            لفظ = ngx_inet_addr(digit, len4 - 1);
            إذا (لفظ == INADDR_NONE) {
                أرجع NGX_ERROR;
            }

            لفظ = ntohl(لفظ);
            *addr++ = (u_char) ((لفظ >> 24) & 0xff);
            *addr++ = (u_char) ((لفظ >> 16) & 0xff);
            n--;
            اكسر;
        }

        إذا (++nibbles > 4) {
            أرجع NGX_ERROR;
        }

        إذا (c >= '0' && c <= '9') {
            لفظ = لفظ * 16 + (c - '0');
            استمر;
        }

        c |= 0x20;

        إذا (c >= 'a' && c <= 'f') {
            لفظ = لفظ * 16 + (c - 'a') + 10;
            استمر;
        }

        أرجع NGX_ERROR;
    }

    إذا (nibbles == 0 && zero == عدم) {
        أرجع NGX_ERROR;
    }

    *addr++ = (u_char) (لفظ >> 8);
    *addr++ = (u_char) (لفظ & 0xff);

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

#جيڪڏهن_پورو


سائيز_t
ngx_sock_ntop(بنية sockaddr *sa, socklen_t socklen, u_char *متن, سائيز_t len,
    ngx_uint_t پورٽ)
{
    u_char               *p;
#إذا (NGX_HAVE_INET6 || NGX_HAVE_UNIX_DOMAIN)
    سائيز_t                n;
#جيڪڏهن_پورو
    بنية sockaddr_اندريون   *جيب;
#إذا (NGX_HAVE_INET6)
    بنية sockaddr_in6  *sin6;
#جيڪڏهن_پورو
#إذا (NGX_HAVE_UNIX_DOMAIN)
    بنية sockaddr_un   *saun;
#جيڪڏهن_پورو

    بدل (sa->sa_family) {

    حالة AF_INET:

        جيب = (بنية sockaddr_اندريون *) sa;
        p = (u_char *) &جيب->sin_addr;

        إذا (پورٽ) {
            p = ngx_snprintf(متن, len, "%ud.%ud.%ud.%ud:%d",
                             p[0], p[1], p[2], p[3], ntohs(جيب->sin_پورٽ));
        } وإلا {
            p = ngx_snprintf(متن, len, "%ud.%ud.%ud.%ud",
                             p[0], p[1], p[2], p[3]);
        }

        أرجع (p - متن);

#إذا (NGX_HAVE_INET6)

    حالة AF_INET6:

        sin6 = (بنية sockaddr_in6 *) sa;

        n = 0;

        إذا (پورٽ) {
            متن[n++] = '[';
        }

        n = ngx_inet6_ntop(sin6->sin6_addr.s6_addr, &متن[n], len);

        إذا (پورٽ) {
            n = ngx_sprintf(&متن[1 + n], "]:%d",
                            ntohs(sin6->sin6_پورٽ)) - متن;
        }

        أرجع n;
#جيڪڏهن_پورو

#إذا (NGX_HAVE_UNIX_DOMAIN)

    حالة AF_UNIX:
        saun = (بنية sockaddr_un *) sa;

        /* on Linux sockaddr might not include sun_path at all */

        إذا (socklen <= (socklen_t) offsetof(بنية sockaddr_un, sun_رستو)) {
            p = ngx_snprintf(متن, len, "unix:%Z");

        } وإلا {
            n = ngx_strnlen((u_char *) saun->sun_رستو,
                            socklen - offsetof(بنية sockaddr_un, sun_رستو));
            p = ngx_snprintf(متن, len, "unix:%*s%Z", n, saun->sun_رستو);
        }

        /* we do not include trailing zero in address length */

        أرجع (p - متن - 1);

#جيڪڏهن_پورو

    افتراضي:
        أرجع 0;
    }
}


سائيز_t
ngx_inet_ntop(صحيح family, فراغ *addr, u_char *متن, سائيز_t len)
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

#جيڪڏهن_پورو

    افتراضي:
        أرجع 0;
    }
}


#إذا (NGX_HAVE_INET6)

سائيز_t
ngx_inet6_ntop(u_char *p, u_char *متن, سائيز_t len)
{
    u_char      *dst;
    سائيز_t       max, n;
    ngx_uint_t   i, zero, آخري;

    إذا (len < NGX_INET6_ADDRSTRLEN) {
        أرجع 0;
    }

    zero = (ngx_uint_t) -1;
    آخري = (ngx_uint_t) -1;
    max = 1;
    n = 0;

    لكل (i = 0; i < 16; i += 2) {

        إذا (p[i] || p[i + 1]) {

            إذا (max < n) {
                zero = آخري;
                max = n;
            }

            n = 0;
            استمر;
        }

        إذا (n++ == 0) {
            آخري = i;
        }
    }

    إذا (max < n) {
        zero = آخري;
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

#جيڪڏهن_پورو


ngx_int_t
ngx_ptocidr(ngx_str_t *متن, ngx_cidr_t *cidr)
{
    u_char      *addr, *نقاب, *آخري;
    سائيز_t       len;
    ngx_int_t    سرڪايو;
#إذا (NGX_HAVE_INET6)
    ngx_int_t    rc;
    ngx_uint_t   s, i;
#جيڪڏهن_پورو

    addr = متن->معلومات;
    آخري = addr + متن->len;

    نقاب = ngx_strlchr(addr, آخري, '/');
    len = (نقاب ? نقاب : آخري) - addr;

    cidr->u.اندريون.addr = ngx_inet_addr(addr, len);

    إذا (cidr->u.اندريون.addr != INADDR_NONE) {
        cidr->family = AF_INET;

        إذا (نقاب == عدم) {
            cidr->u.اندريون.نقاب = 0xffffffff;
            أرجع NGX_OK;
        }

#إذا (NGX_HAVE_INET6)
    } وإلا إذا (ngx_inet6_addr(addr, len, cidr->u.in6.addr.s6_addr) == NGX_OK) {
        cidr->family = AF_INET6;

        إذا (نقاب == عدم) {
            ngx_memset(cidr->u.in6.نقاب.s6_addr, 0xff, 16);
            أرجع NGX_OK;
        }

#جيڪڏهن_پورو
    } وإلا {
        أرجع NGX_ERROR;
    }

    نقاب++;

    سرڪايو = ngx_atoi(نقاب, آخري - نقاب);
    إذا (سرڪايو == NGX_ERROR) {
        أرجع NGX_ERROR;
    }

    بدل (cidr->family) {

#إذا (NGX_HAVE_INET6)
    حالة AF_INET6:
        إذا (سرڪايو > 128) {
            أرجع NGX_ERROR;
        }

        addr = cidr->u.in6.addr.s6_addr;
        نقاب = cidr->u.in6.نقاب.s6_addr;
        rc = NGX_OK;

        لكل (i = 0; i < 16; i++) {

            s = (سرڪايو > 8) ? 8 : سرڪايو;
            سرڪايو -= s;

            نقاب[i] = (u_char) (0xffu << (8 - s));

            إذا (addr[i] != (addr[i] & نقاب[i])) {
                rc = NGX_پورو;
                addr[i] &= نقاب[i];
            }
        }

        أرجع rc;
#جيڪڏهن_پورو

    افتراضي: /* AF_INET */
        إذا (سرڪايو > 32) {
            أرجع NGX_ERROR;
        }

        إذا (سرڪايو) {
            cidr->u.اندريون.نقاب = htonl((uint32_t) (0xffffffffu << (32 - سرڪايو)));

        } وإلا {
            /* x86 compilers use a shl instruction that shifts by modulo 32 */
            cidr->u.اندريون.نقاب = 0;
        }

        إذا (cidr->u.اندريون.addr == (cidr->u.اندريون.addr & cidr->u.اندريون.نقاب)) {
            أرجع NGX_OK;
        }

        cidr->u.اندريون.addr &= cidr->u.اندريون.نقاب;

        أرجع NGX_پورو;
    }
}


ngx_int_t
ngx_cidr_ميلاپ(بنية sockaddr *sa, ngx_array_t *cidrs)
{
#إذا (NGX_HAVE_INET6)
    u_char           *p;
#جيڪڏهن_پورو
    اندريون_addr_t         inaddr;
    ngx_cidr_t       *cidr;
    ngx_uint_t        family, i;
#إذا (NGX_HAVE_INET6)
    ngx_uint_t        n;
    بنية in6_addr  *inaddr6;
#جيڪڏهن_پورو

#إذا (NGX_SUPPRESS_WARN)
    inaddr = 0;
#إذا (NGX_HAVE_INET6)
    inaddr6 = عدم;
#جيڪڏهن_پورو
#جيڪڏهن_پورو

    family = sa->sa_family;

    إذا (family == AF_INET) {
        inaddr = ((بنية sockaddr_اندريون *) sa)->sin_addr.s_addr;
    }

#إذا (NGX_HAVE_INET6)
    وإلا إذا (family == AF_INET6) {
        inaddr6 = &((بنية sockaddr_in6 *) sa)->sin6_addr;

        إذا (IN6_IS_ADDR_V4MAPPED(inaddr6)) {
            family = AF_INET;

            p = inaddr6->s6_addr;

            inaddr = (اندريون_addr_t) p[12] << 24;
            inaddr += p[13] << 16;
            inaddr += p[14] << 8;
            inaddr += p[15];

            inaddr = htonl(inaddr);
        }
    }
#جيڪڏهن_پورو

    لكل (cidr = cidrs->elts, i = 0; i < cidrs->nelts; i++) {
        إذا (cidr[i].family != family) {
            اذهب ايندڙ;
        }

        بدل (family) {

#إذا (NGX_HAVE_INET6)
        حالة AF_INET6:
            لكل (n = 0; n < 16; n++) {
                إذا ((inaddr6->s6_addr[n] & cidr[i].u.in6.نقاب.s6_addr[n])
                    != cidr[i].u.in6.addr.s6_addr[n])
                {
                    اذهب ايندڙ;
                }
            }
            اكسر;
#جيڪڏهن_پورو

#إذا (NGX_HAVE_UNIX_DOMAIN)
        حالة AF_UNIX:
            اكسر;
#جيڪڏهن_پورو

        افتراضي: /* AF_INET */
            إذا ((inaddr & cidr[i].u.اندريون.نقاب) != cidr[i].u.اندريون.addr) {
                اذهب ايندڙ;
            }
            اكسر;
        }

        أرجع NGX_OK;

    ايندڙ:
        استمر;
    }

    أرجع NGX_DECLINED;
}


ngx_int_t
ngx_parse_addr(ngx_pool_t *pool, ngx_addr_t *addr, u_char *متن, سائيز_t len)
{
    اندريون_addr_t             inaddr;
    ngx_uint_t            family;
    بنية sockaddr_اندريون   *جيب;
#إذا (NGX_HAVE_INET6)
    بنية in6_addr       inaddr6;
    بنية sockaddr_in6  *sin6;

    /*
     * prevent MSVC8 warning:
     *    potentially uninitialized local variable 'inaddr6' used
     */
    ngx_memzero(&inaddr6, حجم(بنية in6_addr));
#جيڪڏهن_پورو

    inaddr = ngx_inet_addr(متن, len);

    إذا (inaddr != INADDR_NONE) {
        family = AF_INET;
        len = حجم(بنية sockaddr_اندريون);

#إذا (NGX_HAVE_INET6)
    } وإلا إذا (ngx_inet6_addr(متن, len, inaddr6.s6_addr) == NGX_OK) {
        family = AF_INET6;
        len = حجم(بنية sockaddr_in6);

#جيڪڏهن_پورو
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
#جيڪڏهن_پورو

    افتراضي: /* AF_INET */
        جيب = (بنية sockaddr_اندريون *) addr->sockaddr;
        جيب->sin_addr.s_addr = inaddr;
        اكسر;
    }

    أرجع NGX_OK;
}


ngx_int_t
ngx_parse_addr_پورٽ(ngx_pool_t *pool, ngx_addr_t *addr, u_char *متن,
    سائيز_t len)
{
    u_char     *p, *آخري;
    سائيز_t      plen;
    ngx_int_t   rc, پورٽ;

    rc = ngx_parse_addr(pool, addr, متن, len);

    إذا (rc != NGX_DECLINED) {
        أرجع rc;
    }

    آخري = متن + len;

#إذا (NGX_HAVE_INET6)
    إذا (len && متن[0] == '[') {

        p = ngx_strlchr(متن, آخري, ']');

        إذا (p == آخري - 1) {
            أرجع ngx_parse_addr(pool, addr, متن + 1, len - 2);
        }

        إذا (p == عدم || *++p != ':') {
            أرجع NGX_DECLINED;
        }

        متن++;
        len -= 2;

    } وإلا
#جيڪڏهن_پورو

    {
        p = ngx_strlchr(متن, آخري, ':');

        إذا (p == عدم) {
            أرجع NGX_DECLINED;
        }
    }

    p++;
    plen = آخري - p;

    پورٽ = ngx_atoi(p, plen);

    إذا (پورٽ < 1 || پورٽ > 65535) {
        أرجع NGX_DECLINED;
    }

    len -= plen + 1;

    rc = ngx_parse_addr(pool, addr, متن, len);

    إذا (rc != NGX_OK) {
        أرجع rc;
    }

    ngx_inet_سيٽ_پورٽ(addr->sockaddr, (اندريون_پورٽ_t) پورٽ);

    أرجع NGX_OK;
}


ngx_int_t
ngx_parse_url(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char  *p;
    سائيز_t   len;

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
    u_char              *رستو, *uri, *آخري;
    سائيز_t               len;
    بنية sockaddr_un  *saun;

    len = u->url.len;
    رستو = u->url.معلومات;

    رستو += 5;
    len -= 5;

    إذا (u->uri_part) {

        آخري = رستو + len;
        uri = ngx_strlchr(رستو, آخري, ':');

        إذا (uri) {
            len = uri - رستو;
            uri++;
            u->uri.len = آخري - uri;
            u->uri.معلومات = uri;
        }
    }

    إذا (len == 0) {
        u->err = "no path in the unix domain socket";
        أرجع NGX_ERROR;
    }

    u->host.len = len++;
    u->host.معلومات = رستو;

    إذا (len > حجم(saun->sun_رستو)) {
        u->err = "too long path in the unix domain socket";
        أرجع NGX_ERROR;
    }

    u->socklen = حجم(بنية sockaddr_un);
    saun = (بنية sockaddr_un *) &u->sockaddr;
    saun->sun_family = AF_UNIX;
    (فراغ) ngx_cpystrn((u_char *) saun->sun_رستو, رستو, len);

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
    (فراغ) ngx_cpystrn((u_char *) saun->sun_رستو, رستو, len);

    u->addrs[0].sockaddr = (بنية sockaddr *) saun;
    u->addrs[0].socklen = حجم(بنية sockaddr_un);
    u->addrs[0].name.len = len + 4;
    u->addrs[0].name.معلومات = u->url.معلومات;

    أرجع NGX_OK;

#وإلا

    u->err = "the unix domain sockets are not supported on this platform";

    أرجع NGX_ERROR;

#جيڪڏهن_پورو
}


ساكن ngx_int_t
ngx_parse_inet_url(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char              *host, *پورٽ, *آخري, *uri, *دليلون, *dash;
    سائيز_t               len;
    ngx_int_t            n;
    بنية sockaddr_اندريون  *جيب;

    u->socklen = حجم(بنية sockaddr_اندريون);
    جيب = (بنية sockaddr_اندريون *) &u->sockaddr;
    جيب->sin_family = AF_INET;

    u->family = AF_INET;

    host = u->url.معلومات;

    آخري = host + u->url.len;

    پورٽ = ngx_strlchr(host, آخري, ':');

    uri = ngx_strlchr(host, آخري, '/');

    دليلون = ngx_strlchr(host, آخري, '?');

    إذا (دليلون) {
        إذا (uri == عدم || دليلون < uri) {
            uri = دليلون;
        }
    }

    إذا (uri) {
        إذا (u->listen || !u->uri_part) {
            u->err = "invalid host";
            أرجع NGX_ERROR;
        }

        u->uri.len = آخري - uri;
        u->uri.معلومات = uri;

        آخري = uri;

        إذا (uri < پورٽ) {
            پورٽ = عدم;
        }
    }

    إذا (پورٽ) {
        پورٽ++;

        len = آخري - پورٽ;

        إذا (u->listen) {
            dash = ngx_strlchr(پورٽ, آخري, '-');

            إذا (dash) {
                dash++;

                n = ngx_atoi(dash, آخري - dash);

                إذا (n < 1 || n > 65535) {
                    u->err = "invalid port";
                    أرجع NGX_ERROR;
                }

                u->آخري_پورٽ = (اندريون_پورٽ_t) n;

                len = dash - پورٽ - 1;
            }
        }

        n = ngx_atoi(پورٽ, len);

        إذا (n < 1 || n > 65535) {
            u->err = "invalid port";
            أرجع NGX_ERROR;
        }

        إذا (u->آخري_پورٽ && n > u->آخري_پورٽ) {
            u->err = "invalid port range";
            أرجع NGX_ERROR;
        }

        u->پورٽ = (اندريون_پورٽ_t) n;
        جيب->sin_پورٽ = htons((اندريون_پورٽ_t) n);

        u->پورٽ_متن.len = آخري - پورٽ;
        u->پورٽ_متن.معلومات = پورٽ;

        آخري = پورٽ - 1;

    } وإلا {
        إذا (uri == عدم) {

            إذا (u->listen) {

                /* test value as port only */

                len = آخري - host;

                dash = ngx_strlchr(host, آخري, '-');

                إذا (dash) {
                    dash++;

                    n = ngx_atoi(dash, آخري - dash);

                    إذا (n == NGX_ERROR) {
                        اذهب no_پورٽ;
                    }

                    إذا (n < 1 || n > 65535) {
                        u->err = "invalid port";

                    } وإلا {
                        u->آخري_پورٽ = (اندريون_پورٽ_t) n;
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

                    إذا (u->آخري_پورٽ && n > u->آخري_پورٽ) {
                        u->err = "invalid port range";
                        أرجع NGX_ERROR;
                    }

                    u->پورٽ = (اندريون_پورٽ_t) n;
                    جيب->sin_پورٽ = htons((اندريون_پورٽ_t) n);
                    جيب->sin_addr.s_addr = INADDR_ANY;

                    u->پورٽ_متن.len = آخري - host;
                    u->پورٽ_متن.معلومات = host;

                    u->wildcard = 1;

                    أرجع ngx_inet_جوڙيو_addr(pool, u, &u->sockaddr.sockaddr,
                                             u->socklen, 1);
                }
            }
        }

no_پورٽ:

        u->err = عدم;
        u->no_پورٽ = 1;
        u->پورٽ = u->default_پورٽ;
        جيب->sin_پورٽ = htons(u->default_پورٽ);
        u->آخري_پورٽ = 0;
    }

    len = آخري - host;

    إذا (len == 0) {
        u->err = "no host";
        أرجع NGX_ERROR;
    }

    u->host.len = len;
    u->host.معلومات = host;

    إذا (u->listen && len == 1 && *host == '*') {
        جيب->sin_addr.s_addr = INADDR_ANY;
        u->wildcard = 1;
        أرجع ngx_inet_جوڙيو_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);
    }

    جيب->sin_addr.s_addr = ngx_inet_addr(host, len);

    إذا (جيب->sin_addr.s_addr != INADDR_NONE) {

        إذا (جيب->sin_addr.s_addr == INADDR_ANY) {
            u->wildcard = 1;
        }

        أرجع ngx_inet_جوڙيو_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);
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
    u_char               *p, *host, *پورٽ, *آخري, *uri, *dash;
    سائيز_t                len;
    ngx_int_t             n;
    بنية sockaddr_in6  *sin6;

    u->socklen = حجم(بنية sockaddr_in6);
    sin6 = (بنية sockaddr_in6 *) &u->sockaddr;
    sin6->sin6_family = AF_INET6;

    host = u->url.معلومات + 1;

    آخري = u->url.معلومات + u->url.len;

    p = ngx_strlchr(host, آخري, ']');

    إذا (p == عدم) {
        u->err = "invalid host";
        أرجع NGX_ERROR;
    }

    پورٽ = p + 1;

    uri = ngx_strlchr(پورٽ, آخري, '/');

    إذا (uri) {
        إذا (u->listen || !u->uri_part) {
            u->err = "invalid host";
            أرجع NGX_ERROR;
        }

        u->uri.len = آخري - uri;
        u->uri.معلومات = uri;

        آخري = uri;
    }

    إذا (پورٽ < آخري) {
        إذا (*پورٽ != ':') {
            u->err = "invalid host";
            أرجع NGX_ERROR;
        }

        پورٽ++;

        len = آخري - پورٽ;

        إذا (u->listen) {
            dash = ngx_strlchr(پورٽ, آخري, '-');

            إذا (dash) {
                dash++;

                n = ngx_atoi(dash, آخري - dash);

                إذا (n < 1 || n > 65535) {
                    u->err = "invalid port";
                    أرجع NGX_ERROR;
                }

                u->آخري_پورٽ = (اندريون_پورٽ_t) n;

                len = dash - پورٽ - 1;
            }
        }

        n = ngx_atoi(پورٽ, len);

        إذا (n < 1 || n > 65535) {
            u->err = "invalid port";
            أرجع NGX_ERROR;
        }

        إذا (u->آخري_پورٽ && n > u->آخري_پورٽ) {
            u->err = "invalid port range";
            أرجع NGX_ERROR;
        }

        u->پورٽ = (اندريون_پورٽ_t) n;
        sin6->sin6_پورٽ = htons((اندريون_پورٽ_t) n);

        u->پورٽ_متن.len = آخري - پورٽ;
        u->پورٽ_متن.معلومات = پورٽ;

    } وإلا {
        u->no_پورٽ = 1;
        u->پورٽ = u->default_پورٽ;
        sin6->sin6_پورٽ = htons(u->default_پورٽ);
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

    أرجع ngx_inet_جوڙيو_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);

#وإلا

    u->err = "the INET6 sockets are not supported on this platform";

    أرجع NGX_ERROR;

#جيڪڏهن_پورو
}


#إذا (NGX_HAVE_GETADDRINFO && NGX_HAVE_INET6)

ngx_int_t
ngx_inet_resolve_host(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char           *host;
    ngx_uint_t        n;
    بنية addrinfo   hints, *res, *rp;

    host = ngx_alloc(u->host.len + 1, pool->لاگرٿم);
    إذا (host == عدم) {
        أرجع NGX_ERROR;
    }

    (فراغ) ngx_cpystrn(host, u->host.معلومات, u->host.len + 1);

    ngx_memzero(&hints, حجم(بنية addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_وهڪرو;
#جيڪڏهن_وصف_ٿيل AI_ADDRCONFIG
    hints.ai_flags = AI_ADDRCONFIG;
#جيڪڏهن_پورو

    إذا (getaddrinfo((حرف *) host, عدم, &hints, &res) != 0) {
        u->err = "host not found";
        ngx_free(host);
        أرجع NGX_ERROR;
    }

    ngx_free(host);

    لكل (n = 0, rp = res; rp != عدم; rp = rp->ai_ايندڙ) {

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

    لكل (rp = res; rp != عدم; rp = rp->ai_ايندڙ) {

        بدل (rp->ai_family) {

        حالة AF_INET:
        حالة AF_INET6:
            اكسر;

        افتراضي:
            استمر;
        }

        إذا (ngx_inet_جوڙيو_addr(pool, u, rp->ai_addr, rp->ai_addrlen, n)
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
    بنية sockaddr_اندريون   جيب;

    /* AF_INET only */

    ngx_memzero(&جيب, حجم(بنية sockaddr_اندريون));

    جيب.sin_family = AF_INET;
    جيب.sin_addr.s_addr = ngx_inet_addr(u->host.معلومات, u->host.len);

    إذا (جيب.sin_addr.s_addr == INADDR_NONE) {
        host = ngx_alloc(u->host.len + 1, pool->لاگرٿم);
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
            جيب.sin_addr.s_addr = *(اندريون_addr_t *) (h->h_addr_list[i]);

            إذا (ngx_inet_جوڙيو_addr(pool, u, (بنية sockaddr *) &جيب,
                                  حجم(بنية sockaddr_اندريون), n)
                != NGX_OK)
            {
                أرجع NGX_ERROR;
            }
        }

    } وإلا {

        /* MP: ngx_shared_palloc() */

        إذا (ngx_inet_جوڙيو_addr(pool, u, (بنية sockaddr *) &جيب,
                              حجم(بنية sockaddr_اندريون), 1)
            != NGX_OK)
        {
            أرجع NGX_ERROR;
        }
    }

    أرجع NGX_OK;
}

#جيڪڏهن_پورو /* NGX_HAVE_GETADDRINFO && NGX_HAVE_INET6 */


ساكن ngx_int_t
ngx_inet_جوڙيو_addr(ngx_pool_t *pool, ngx_url_t *u, بنية sockaddr *sockaddr,
    socklen_t socklen, ngx_uint_t ڪل)
{
    u_char           *p;
    سائيز_t            len;
    ngx_uint_t        i, nports;
    ngx_addr_t       *addr;
    بنية sockaddr  *sa;

    nports = u->آخري_پورٽ ? u->آخري_پورٽ - u->پورٽ + 1 : 1;

    إذا (u->addrs == عدم) {
        u->addrs = ngx_palloc(pool, ڪل * nports * حجم(ngx_addr_t));
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

        ngx_inet_سيٽ_پورٽ(sa, u->پورٽ + i);

        بدل (sa->sa_family) {

#إذا (NGX_HAVE_INET6)
        حالة AF_INET6:
            len = NGX_INET6_ADDRSTRLEN + حجم("[]:65536") - 1;
            اكسر;
#جيڪڏهن_پورو

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
    بنية sockaddr *sa2, socklen_t slen2, ngx_uint_t cmp_پورٽ)
{
    بنية sockaddr_اندريون   *sin1, *sin2;
#إذا (NGX_HAVE_INET6)
    بنية sockaddr_in6  *sin61, *sin62;
#جيڪڏهن_پورو
#إذا (NGX_HAVE_UNIX_DOMAIN)
    سائيز_t                len;
    بنية sockaddr_un   *saun1, *saun2;
#جيڪڏهن_پورو

    إذا (sa1->sa_family != sa2->sa_family) {
        أرجع NGX_DECLINED;
    }

    بدل (sa1->sa_family) {

#إذا (NGX_HAVE_INET6)
    حالة AF_INET6:

        sin61 = (بنية sockaddr_in6 *) sa1;
        sin62 = (بنية sockaddr_in6 *) sa2;

        إذا (cmp_پورٽ && sin61->sin6_پورٽ != sin62->sin6_پورٽ) {
            أرجع NGX_DECLINED;
        }

        إذا (ngx_memcmp(&sin61->sin6_addr, &sin62->sin6_addr, 16) != 0) {
            أرجع NGX_DECLINED;
        }

        اكسر;
#جيڪڏهن_پورو

#إذا (NGX_HAVE_UNIX_DOMAIN)
    حالة AF_UNIX:

        saun1 = (بنية sockaddr_un *) sa1;
        saun2 = (بنية sockaddr_un *) sa2;

        إذا (slen1 < slen2) {
            len = slen1 - offsetof(بنية sockaddr_un, sun_رستو);

        } وإلا {
            len = slen2 - offsetof(بنية sockaddr_un, sun_رستو);
        }

        إذا (len > حجم(saun1->sun_رستو)) {
            len = حجم(saun1->sun_رستو);
        }

        إذا (ngx_memcmp(&saun1->sun_رستو, &saun2->sun_رستو, len) != 0) {
            أرجع NGX_DECLINED;
        }

        اكسر;
#جيڪڏهن_پورو

    افتراضي: /* AF_INET */

        sin1 = (بنية sockaddr_اندريون *) sa1;
        sin2 = (بنية sockaddr_اندريون *) sa2;

        إذا (cmp_پورٽ && sin1->sin_پورٽ != sin2->sin_پورٽ) {
            أرجع NGX_DECLINED;
        }

        إذا (sin1->sin_addr.s_addr != sin2->sin_addr.s_addr) {
            أرجع NGX_DECLINED;
        }

        اكسر;
    }

    أرجع NGX_OK;
}


اندريون_پورٽ_t
ngx_inet_get_پورٽ(بنية sockaddr *sa)
{
    بنية sockaddr_اندريون   *جيب;
#إذا (NGX_HAVE_INET6)
    بنية sockaddr_in6  *sin6;
#جيڪڏهن_پورو

    بدل (sa->sa_family) {

#إذا (NGX_HAVE_INET6)
    حالة AF_INET6:
        sin6 = (بنية sockaddr_in6 *) sa;
        أرجع ntohs(sin6->sin6_پورٽ);
#جيڪڏهن_پورو

#إذا (NGX_HAVE_UNIX_DOMAIN)
    حالة AF_UNIX:
        أرجع 0;
#جيڪڏهن_پورو

    افتراضي: /* AF_INET */
        جيب = (بنية sockaddr_اندريون *) sa;
        أرجع ntohs(جيب->sin_پورٽ);
    }
}


فراغ
ngx_inet_سيٽ_پورٽ(بنية sockaddr *sa, اندريون_پورٽ_t پورٽ)
{
    بنية sockaddr_اندريون   *جيب;
#إذا (NGX_HAVE_INET6)
    بنية sockaddr_in6  *sin6;
#جيڪڏهن_پورو

    بدل (sa->sa_family) {

#إذا (NGX_HAVE_INET6)
    حالة AF_INET6:
        sin6 = (بنية sockaddr_in6 *) sa;
        sin6->sin6_پورٽ = htons(پورٽ);
        اكسر;
#جيڪڏهن_پورو

#إذا (NGX_HAVE_UNIX_DOMAIN)
    حالة AF_UNIX:
        اكسر;
#جيڪڏهن_پورو

    افتراضي: /* AF_INET */
        جيب = (بنية sockaddr_اندريون *) sa;
        جيب->sin_پورٽ = htons(پورٽ);
        اكسر;
    }
}


ngx_uint_t
ngx_inet_wildcard(بنية sockaddr *sa)
{
    بنية sockaddr_اندريون   *جيب;
#إذا (NGX_HAVE_INET6)
    بنية sockaddr_in6  *sin6;
#جيڪڏهن_پورو

    بدل (sa->sa_family) {

    حالة AF_INET:
        جيب = (بنية sockaddr_اندريون *) sa;

        إذا (جيب->sin_addr.s_addr == INADDR_ANY) {
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

#جيڪڏهن_پورو
    }

    أرجع 0;
}
