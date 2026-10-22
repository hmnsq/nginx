
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#شامل <ngx_config.h>
#شامل <ngx_core.h>


ساکن ngx_int_t ngx_parse_unix_domain_url(ngx_pool_t *pool, ngx_url_t *u);
ساکن ngx_int_t ngx_parse_inet_url(ngx_pool_t *pool, ngx_url_t *u);
ساکن ngx_int_t ngx_parse_inet6_url(ngx_pool_t *pool, ngx_url_t *u);
ساکن ngx_int_t ngx_inet_جوڑو_addr(ngx_pool_t *pool, ngx_url_t *u,
    ساخت sockaddr *sockaddr, socklen_t socklen, ngx_uint_t کل);


اندر_addr_t
ngx_inet_addr(u_char *متن, سائز_t len)
{
    u_char      *p, c;
    اندر_addr_t    addr;
    ngx_uint_t   octet, n;

    addr = 0;
    octet = 0;
    n = 0;

    برائے (p = متن; p < متن + len; p++) {
        c = *p;

        اگر (c >= '0' && c <= '9') {
            octet = octet * 10 + (c - '0');

            اگر (octet > 255) {
                واپس INADDR_NONE;
            }

            جاری;
        }

        اگر (c == '.') {
            addr = (addr << 8) + octet;
            octet = 0;
            n++;
            جاری;
        }

        واپس INADDR_NONE;
    }

    اگر (n == 3) {
        addr = (addr << 8) + octet;
        واپس htonl(addr);
    }

    واپس INADDR_NONE;
}


#اگر (NGX_HAVE_INET6)

ngx_int_t
ngx_inet6_addr(u_char *p, سائز_t len, u_char *addr)
{
    u_char      c, *zero, *digit, *s, *d;
    سائز_t      len4;
    ngx_uint_t  n, nibbles, لفظ;

    اگر (len == 0) {
        واپس NGX_ERROR;
    }

    zero = تہی;
    digit = تہی;
    len4 = 0;
    nibbles = 0;
    لفظ = 0;
    n = 8;

    اگر (p[0] == ':') {
        p++;
        len--;
    }

    برائے (/* void */; len; len--) {
        c = *p++;

        اگر (c == ':') {
            اگر (nibbles) {
                digit = p;
                len4 = len;
                *addr++ = (u_char) (لفظ >> 8);
                *addr++ = (u_char) (لفظ & 0xff);

                اگر (--n) {
                    nibbles = 0;
                    لفظ = 0;
                    جاری;
                }

            } ورنہ {
                اگر (zero == تہی) {
                    digit = p;
                    len4 = len;
                    zero = addr;
                    جاری;
                }
            }

            واپس NGX_ERROR;
        }

        اگر (c == '.' && nibbles) {
            اگر (n < 2 || digit == تہی) {
                واپس NGX_ERROR;
            }

            لفظ = ngx_inet_addr(digit, len4 - 1);
            اگر (لفظ == INADDR_NONE) {
                واپس NGX_ERROR;
            }

            لفظ = ntohl(لفظ);
            *addr++ = (u_char) ((لفظ >> 24) & 0xff);
            *addr++ = (u_char) ((لفظ >> 16) & 0xff);
            n--;
            توڑو;
        }

        اگر (++nibbles > 4) {
            واپس NGX_ERROR;
        }

        اگر (c >= '0' && c <= '9') {
            لفظ = لفظ * 16 + (c - '0');
            جاری;
        }

        c |= 0x20;

        اگر (c >= 'a' && c <= 'f') {
            لفظ = لفظ * 16 + (c - 'a') + 10;
            جاری;
        }

        واپس NGX_ERROR;
    }

    اگر (nibbles == 0 && zero == تہی) {
        واپس NGX_ERROR;
    }

    *addr++ = (u_char) (لفظ >> 8);
    *addr++ = (u_char) (لفظ & 0xff);

    اگر (--n) {
        اگر (zero) {
            n *= 2;
            s = addr - 1;
            d = s + n;
            جب (s >= zero) {
                *d-- = *s--;
            }
            ngx_memzero(zero, n);
            واپس NGX_OK;
        }

    } ورنہ {
        اگر (zero == تہی) {
            واپس NGX_OK;
        }
    }

    واپس NGX_ERROR;
}

#اگر_ختم


سائز_t
ngx_sock_ntop(ساخت sockaddr *sa, socklen_t socklen, u_char *متن, سائز_t len,
    ngx_uint_t پورٹ)
{
    u_char               *p;
#اگر (NGX_HAVE_INET6 || NGX_HAVE_UNIX_DOMAIN)
    سائز_t                n;
#اگر_ختم
    ساخت sockaddr_اندر   *جیب;
#اگر (NGX_HAVE_INET6)
    ساخت sockaddr_in6  *sin6;
#اگر_ختم
#اگر (NGX_HAVE_UNIX_DOMAIN)
    ساخت sockaddr_un   *saun;
#اگر_ختم

    انتخاب (sa->sa_family) {

    حالت AF_INET:

        جیب = (ساخت sockaddr_اندر *) sa;
        p = (u_char *) &جیب->sin_addr;

        اگر (پورٹ) {
            p = ngx_snprintf(متن, len, "%ud.%ud.%ud.%ud:%d",
                             p[0], p[1], p[2], p[3], ntohs(جیب->sin_پورٹ));
        } ورنہ {
            p = ngx_snprintf(متن, len, "%ud.%ud.%ud.%ud",
                             p[0], p[1], p[2], p[3]);
        }

        واپس (p - متن);

#اگر (NGX_HAVE_INET6)

    حالت AF_INET6:

        sin6 = (ساخت sockaddr_in6 *) sa;

        n = 0;

        اگر (پورٹ) {
            متن[n++] = '[';
        }

        n = ngx_inet6_ntop(sin6->sin6_addr.s6_addr, &متن[n], len);

        اگر (پورٹ) {
            n = ngx_sprintf(&متن[1 + n], "]:%d",
                            ntohs(sin6->sin6_پورٹ)) - متن;
        }

        واپس n;
#اگر_ختم

#اگر (NGX_HAVE_UNIX_DOMAIN)

    حالت AF_UNIX:
        saun = (ساخت sockaddr_un *) sa;

        /* on Linux sockaddr might not include sun_path at all */

        اگر (socklen <= (socklen_t) offsetof(ساخت sockaddr_un, sun_راستہ)) {
            p = ngx_snprintf(متن, len, "unix:%Z");

        } ورنہ {
            n = ngx_strnlen((u_char *) saun->sun_راستہ,
                            socklen - offsetof(ساخت sockaddr_un, sun_راستہ));
            p = ngx_snprintf(متن, len, "unix:%*s%Z", n, saun->sun_راستہ);
        }

        /* we do not include trailing zero in address length */

        واپس (p - متن - 1);

#اگر_ختم

    طےشدہ:
        واپس 0;
    }
}


سائز_t
ngx_inet_ntop(صحیح family, خالی *addr, u_char *متن, سائز_t len)
{
    u_char  *p;

    انتخاب (family) {

    حالت AF_INET:

        p = addr;

        واپس ngx_snprintf(متن, len, "%ud.%ud.%ud.%ud",
                            p[0], p[1], p[2], p[3])
               - متن;

#اگر (NGX_HAVE_INET6)

    حالت AF_INET6:
        واپس ngx_inet6_ntop(addr, متن, len);

#اگر_ختم

    طےشدہ:
        واپس 0;
    }
}


#اگر (NGX_HAVE_INET6)

سائز_t
ngx_inet6_ntop(u_char *p, u_char *متن, سائز_t len)
{
    u_char      *dst;
    سائز_t       max, n;
    ngx_uint_t   i, zero, آخری;

    اگر (len < NGX_INET6_ADDRSTRLEN) {
        واپس 0;
    }

    zero = (ngx_uint_t) -1;
    آخری = (ngx_uint_t) -1;
    max = 1;
    n = 0;

    برائے (i = 0; i < 16; i += 2) {

        اگر (p[i] || p[i + 1]) {

            اگر (max < n) {
                zero = آخری;
                max = n;
            }

            n = 0;
            جاری;
        }

        اگر (n++ == 0) {
            آخری = i;
        }
    }

    اگر (max < n) {
        zero = آخری;
        max = n;
    }

    dst = متن;
    n = 16;

    اگر (zero == 0) {

        اگر ((max == 5 && p[10] == 0xff && p[11] == 0xff)
            || (max == 6)
            || (max == 7 && p[14] != 0 && p[15] != 1))
        {
            n = 12;
        }

        *dst++ = ':';
    }

    برائے (i = 0; i < n; i += 2) {

        اگر (i == zero) {
            *dst++ = ':';
            i += (max - 1) * 2;
            جاری;
        }

        dst = ngx_sprintf(dst, "%xd", p[i] * 256 + p[i + 1]);

        اگر (i < 14) {
            *dst++ = ':';
        }
    }

    اگر (n == 12) {
        dst = ngx_sprintf(dst, "%ud.%ud.%ud.%ud", p[12], p[13], p[14], p[15]);
    }

    واپس dst - متن;
}

#اگر_ختم


ngx_int_t
ngx_ptocidr(ngx_str_t *متن, ngx_cidr_t *cidr)
{
    u_char      *addr, *نقاب, *آخری;
    سائز_t       len;
    ngx_int_t    سرکاؤ;
#اگر (NGX_HAVE_INET6)
    ngx_int_t    rc;
    ngx_uint_t   s, i;
#اگر_ختم

    addr = متن->ڈیٹا;
    آخری = addr + متن->len;

    نقاب = ngx_strlchr(addr, آخری, '/');
    len = (نقاب ? نقاب : آخری) - addr;

    cidr->u.اندر.addr = ngx_inet_addr(addr, len);

    اگر (cidr->u.اندر.addr != INADDR_NONE) {
        cidr->family = AF_INET;

        اگر (نقاب == تہی) {
            cidr->u.اندر.نقاب = 0xffffffff;
            واپس NGX_OK;
        }

#اگر (NGX_HAVE_INET6)
    } ورنہ اگر (ngx_inet6_addr(addr, len, cidr->u.in6.addr.s6_addr) == NGX_OK) {
        cidr->family = AF_INET6;

        اگر (نقاب == تہی) {
            ngx_memset(cidr->u.in6.نقاب.s6_addr, 0xff, 16);
            واپس NGX_OK;
        }

#اگر_ختم
    } ورنہ {
        واپس NGX_ERROR;
    }

    نقاب++;

    سرکاؤ = ngx_atoi(نقاب, آخری - نقاب);
    اگر (سرکاؤ == NGX_ERROR) {
        واپس NGX_ERROR;
    }

    انتخاب (cidr->family) {

#اگر (NGX_HAVE_INET6)
    حالت AF_INET6:
        اگر (سرکاؤ > 128) {
            واپس NGX_ERROR;
        }

        addr = cidr->u.in6.addr.s6_addr;
        نقاب = cidr->u.in6.نقاب.s6_addr;
        rc = NGX_OK;

        برائے (i = 0; i < 16; i++) {

            s = (سرکاؤ > 8) ? 8 : سرکاؤ;
            سرکاؤ -= s;

            نقاب[i] = (u_char) (0xffu << (8 - s));

            اگر (addr[i] != (addr[i] & نقاب[i])) {
                rc = NGX_مکمل;
                addr[i] &= نقاب[i];
            }
        }

        واپس rc;
#اگر_ختم

    طےشدہ: /* AF_INET */
        اگر (سرکاؤ > 32) {
            واپس NGX_ERROR;
        }

        اگر (سرکاؤ) {
            cidr->u.اندر.نقاب = htonl((uint32_t) (0xffffffffu << (32 - سرکاؤ)));

        } ورنہ {
            /* x86 compilers use a shl instruction that shifts by modulo 32 */
            cidr->u.اندر.نقاب = 0;
        }

        اگر (cidr->u.اندر.addr == (cidr->u.اندر.addr & cidr->u.اندر.نقاب)) {
            واپس NGX_OK;
        }

        cidr->u.اندر.addr &= cidr->u.اندر.نقاب;

        واپس NGX_مکمل;
    }
}


ngx_int_t
ngx_cidr_مطابقت(ساخت sockaddr *sa, ngx_array_t *cidrs)
{
#اگر (NGX_HAVE_INET6)
    u_char           *p;
#اگر_ختم
    اندر_addr_t         inaddr;
    ngx_cidr_t       *cidr;
    ngx_uint_t        family, i;
#اگر (NGX_HAVE_INET6)
    ngx_uint_t        n;
    ساخت in6_addr  *inaddr6;
#اگر_ختم

#اگر (NGX_SUPPRESS_WARN)
    inaddr = 0;
#اگر (NGX_HAVE_INET6)
    inaddr6 = تہی;
#اگر_ختم
#اگر_ختم

    family = sa->sa_family;

    اگر (family == AF_INET) {
        inaddr = ((ساخت sockaddr_اندر *) sa)->sin_addr.s_addr;
    }

#اگر (NGX_HAVE_INET6)
    ورنہ اگر (family == AF_INET6) {
        inaddr6 = &((ساخت sockaddr_in6 *) sa)->sin6_addr;

        اگر (IN6_IS_ADDR_V4MAPPED(inaddr6)) {
            family = AF_INET;

            p = inaddr6->s6_addr;

            inaddr = (اندر_addr_t) p[12] << 24;
            inaddr += p[13] << 16;
            inaddr += p[14] << 8;
            inaddr += p[15];

            inaddr = htonl(inaddr);
        }
    }
#اگر_ختم

    برائے (cidr = cidrs->elts, i = 0; i < cidrs->nelts; i++) {
        اگر (cidr[i].family != family) {
            جاؤ اگلا;
        }

        انتخاب (family) {

#اگر (NGX_HAVE_INET6)
        حالت AF_INET6:
            برائے (n = 0; n < 16; n++) {
                اگر ((inaddr6->s6_addr[n] & cidr[i].u.in6.نقاب.s6_addr[n])
                    != cidr[i].u.in6.addr.s6_addr[n])
                {
                    جاؤ اگلا;
                }
            }
            توڑو;
#اگر_ختم

#اگر (NGX_HAVE_UNIX_DOMAIN)
        حالت AF_UNIX:
            توڑو;
#اگر_ختم

        طےشدہ: /* AF_INET */
            اگر ((inaddr & cidr[i].u.اندر.نقاب) != cidr[i].u.اندر.addr) {
                جاؤ اگلا;
            }
            توڑو;
        }

        واپس NGX_OK;

    اگلا:
        جاری;
    }

    واپس NGX_DECLINED;
}


ngx_int_t
ngx_parse_addr(ngx_pool_t *pool, ngx_addr_t *addr, u_char *متن, سائز_t len)
{
    اندر_addr_t             inaddr;
    ngx_uint_t            family;
    ساخت sockaddr_اندر   *جیب;
#اگر (NGX_HAVE_INET6)
    ساخت in6_addr       inaddr6;
    ساخت sockaddr_in6  *sin6;

    /*
     * prevent MSVC8 warning:
     *    potentially uninitialized local variable 'inaddr6' used
     */
    ngx_memzero(&inaddr6, حجم(ساخت in6_addr));
#اگر_ختم

    inaddr = ngx_inet_addr(متن, len);

    اگر (inaddr != INADDR_NONE) {
        family = AF_INET;
        len = حجم(ساخت sockaddr_اندر);

#اگر (NGX_HAVE_INET6)
    } ورنہ اگر (ngx_inet6_addr(متن, len, inaddr6.s6_addr) == NGX_OK) {
        family = AF_INET6;
        len = حجم(ساخت sockaddr_in6);

#اگر_ختم
    } ورنہ {
        واپس NGX_DECLINED;
    }

    addr->sockaddr = ngx_pcalloc(pool, len);
    اگر (addr->sockaddr == تہی) {
        واپس NGX_ERROR;
    }

    addr->sockaddr->sa_family = (u_char) family;
    addr->socklen = len;

    انتخاب (family) {

#اگر (NGX_HAVE_INET6)
    حالت AF_INET6:
        sin6 = (ساخت sockaddr_in6 *) addr->sockaddr;
        ngx_memcpy(sin6->sin6_addr.s6_addr, inaddr6.s6_addr, 16);
        توڑو;
#اگر_ختم

    طےشدہ: /* AF_INET */
        جیب = (ساخت sockaddr_اندر *) addr->sockaddr;
        جیب->sin_addr.s_addr = inaddr;
        توڑو;
    }

    واپس NGX_OK;
}


ngx_int_t
ngx_parse_addr_پورٹ(ngx_pool_t *pool, ngx_addr_t *addr, u_char *متن,
    سائز_t len)
{
    u_char     *p, *آخری;
    سائز_t      plen;
    ngx_int_t   rc, پورٹ;

    rc = ngx_parse_addr(pool, addr, متن, len);

    اگر (rc != NGX_DECLINED) {
        واپس rc;
    }

    آخری = متن + len;

#اگر (NGX_HAVE_INET6)
    اگر (len && متن[0] == '[') {

        p = ngx_strlchr(متن, آخری, ']');

        اگر (p == آخری - 1) {
            واپس ngx_parse_addr(pool, addr, متن + 1, len - 2);
        }

        اگر (p == تہی || *++p != ':') {
            واپس NGX_DECLINED;
        }

        متن++;
        len -= 2;

    } ورنہ
#اگر_ختم

    {
        p = ngx_strlchr(متن, آخری, ':');

        اگر (p == تہی) {
            واپس NGX_DECLINED;
        }
    }

    p++;
    plen = آخری - p;

    پورٹ = ngx_atoi(p, plen);

    اگر (پورٹ < 1 || پورٹ > 65535) {
        واپس NGX_DECLINED;
    }

    len -= plen + 1;

    rc = ngx_parse_addr(pool, addr, متن, len);

    اگر (rc != NGX_OK) {
        واپس rc;
    }

    ngx_inet_سیٹ_پورٹ(addr->sockaddr, (اندر_پورٹ_t) پورٹ);

    واپس NGX_OK;
}


ngx_int_t
ngx_parse_url(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char  *p;
    سائز_t   len;

    p = u->url.ڈیٹا;
    len = u->url.len;

    اگر (len >= 5 && ngx_strncasecmp(p, (u_char *) "unix:", 5) == 0) {
        واپس ngx_parse_unix_domain_url(pool, u);
    }

    اگر (len && p[0] == '[') {
        واپس ngx_parse_inet6_url(pool, u);
    }

    واپس ngx_parse_inet_url(pool, u);
}


ساکن ngx_int_t
ngx_parse_unix_domain_url(ngx_pool_t *pool, ngx_url_t *u)
{
#اگر (NGX_HAVE_UNIX_DOMAIN)
    u_char              *راستہ, *uri, *آخری;
    سائز_t               len;
    ساخت sockaddr_un  *saun;

    len = u->url.len;
    راستہ = u->url.ڈیٹا;

    راستہ += 5;
    len -= 5;

    اگر (u->uri_part) {

        آخری = راستہ + len;
        uri = ngx_strlchr(راستہ, آخری, ':');

        اگر (uri) {
            len = uri - راستہ;
            uri++;
            u->uri.len = آخری - uri;
            u->uri.ڈیٹا = uri;
        }
    }

    اگر (len == 0) {
        u->err = "no path in the unix domain socket";
        واپس NGX_ERROR;
    }

    u->host.len = len++;
    u->host.ڈیٹا = راستہ;

    اگر (len > حجم(saun->sun_راستہ)) {
        u->err = "too long path in the unix domain socket";
        واپس NGX_ERROR;
    }

    u->socklen = حجم(ساخت sockaddr_un);
    saun = (ساخت sockaddr_un *) &u->sockaddr;
    saun->sun_family = AF_UNIX;
    (خالی) ngx_cpystrn((u_char *) saun->sun_راستہ, راستہ, len);

    u->addrs = ngx_pcalloc(pool, حجم(ngx_addr_t));
    اگر (u->addrs == تہی) {
        واپس NGX_ERROR;
    }

    saun = ngx_pcalloc(pool, حجم(ساخت sockaddr_un));
    اگر (saun == تہی) {
        واپس NGX_ERROR;
    }

    u->family = AF_UNIX;
    u->naddrs = 1;

    saun->sun_family = AF_UNIX;
    (خالی) ngx_cpystrn((u_char *) saun->sun_راستہ, راستہ, len);

    u->addrs[0].sockaddr = (ساخت sockaddr *) saun;
    u->addrs[0].socklen = حجم(ساخت sockaddr_un);
    u->addrs[0].name.len = len + 4;
    u->addrs[0].name.ڈیٹا = u->url.ڈیٹا;

    واپس NGX_OK;

#ورنہ

    u->err = "the unix domain sockets are not supported on this platform";

    واپس NGX_ERROR;

#اگر_ختم
}


ساکن ngx_int_t
ngx_parse_inet_url(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char              *host, *پورٹ, *آخری, *uri, *دلائل, *dash;
    سائز_t               len;
    ngx_int_t            n;
    ساخت sockaddr_اندر  *جیب;

    u->socklen = حجم(ساخت sockaddr_اندر);
    جیب = (ساخت sockaddr_اندر *) &u->sockaddr;
    جیب->sin_family = AF_INET;

    u->family = AF_INET;

    host = u->url.ڈیٹا;

    آخری = host + u->url.len;

    پورٹ = ngx_strlchr(host, آخری, ':');

    uri = ngx_strlchr(host, آخری, '/');

    دلائل = ngx_strlchr(host, آخری, '?');

    اگر (دلائل) {
        اگر (uri == تہی || دلائل < uri) {
            uri = دلائل;
        }
    }

    اگر (uri) {
        اگر (u->listen || !u->uri_part) {
            u->err = "invalid host";
            واپس NGX_ERROR;
        }

        u->uri.len = آخری - uri;
        u->uri.ڈیٹا = uri;

        آخری = uri;

        اگر (uri < پورٹ) {
            پورٹ = تہی;
        }
    }

    اگر (پورٹ) {
        پورٹ++;

        len = آخری - پورٹ;

        اگر (u->listen) {
            dash = ngx_strlchr(پورٹ, آخری, '-');

            اگر (dash) {
                dash++;

                n = ngx_atoi(dash, آخری - dash);

                اگر (n < 1 || n > 65535) {
                    u->err = "invalid port";
                    واپس NGX_ERROR;
                }

                u->آخری_پورٹ = (اندر_پورٹ_t) n;

                len = dash - پورٹ - 1;
            }
        }

        n = ngx_atoi(پورٹ, len);

        اگر (n < 1 || n > 65535) {
            u->err = "invalid port";
            واپس NGX_ERROR;
        }

        اگر (u->آخری_پورٹ && n > u->آخری_پورٹ) {
            u->err = "invalid port range";
            واپس NGX_ERROR;
        }

        u->پورٹ = (اندر_پورٹ_t) n;
        جیب->sin_پورٹ = htons((اندر_پورٹ_t) n);

        u->پورٹ_متن.len = آخری - پورٹ;
        u->پورٹ_متن.ڈیٹا = پورٹ;

        آخری = پورٹ - 1;

    } ورنہ {
        اگر (uri == تہی) {

            اگر (u->listen) {

                /* test value as port only */

                len = آخری - host;

                dash = ngx_strlchr(host, آخری, '-');

                اگر (dash) {
                    dash++;

                    n = ngx_atoi(dash, آخری - dash);

                    اگر (n == NGX_ERROR) {
                        جاؤ no_پورٹ;
                    }

                    اگر (n < 1 || n > 65535) {
                        u->err = "invalid port";

                    } ورنہ {
                        u->آخری_پورٹ = (اندر_پورٹ_t) n;
                    }

                    len = dash - host - 1;
                }

                n = ngx_atoi(host, len);

                اگر (n != NGX_ERROR) {

                    اگر (u->err) {
                        واپس NGX_ERROR;
                    }

                    اگر (n < 1 || n > 65535) {
                        u->err = "invalid port";
                        واپس NGX_ERROR;
                    }

                    اگر (u->آخری_پورٹ && n > u->آخری_پورٹ) {
                        u->err = "invalid port range";
                        واپس NGX_ERROR;
                    }

                    u->پورٹ = (اندر_پورٹ_t) n;
                    جیب->sin_پورٹ = htons((اندر_پورٹ_t) n);
                    جیب->sin_addr.s_addr = INADDR_ANY;

                    u->پورٹ_متن.len = آخری - host;
                    u->پورٹ_متن.ڈیٹا = host;

                    u->wildcard = 1;

                    واپس ngx_inet_جوڑو_addr(pool, u, &u->sockaddr.sockaddr,
                                             u->socklen, 1);
                }
            }
        }

no_پورٹ:

        u->err = تہی;
        u->no_پورٹ = 1;
        u->پورٹ = u->default_پورٹ;
        جیب->sin_پورٹ = htons(u->default_پورٹ);
        u->آخری_پورٹ = 0;
    }

    len = آخری - host;

    اگر (len == 0) {
        u->err = "no host";
        واپس NGX_ERROR;
    }

    u->host.len = len;
    u->host.ڈیٹا = host;

    اگر (u->listen && len == 1 && *host == '*') {
        جیب->sin_addr.s_addr = INADDR_ANY;
        u->wildcard = 1;
        واپس ngx_inet_جوڑو_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);
    }

    جیب->sin_addr.s_addr = ngx_inet_addr(host, len);

    اگر (جیب->sin_addr.s_addr != INADDR_NONE) {

        اگر (جیب->sin_addr.s_addr == INADDR_ANY) {
            u->wildcard = 1;
        }

        واپس ngx_inet_جوڑو_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);
    }

    اگر (u->no_resolve) {
        واپس NGX_OK;
    }

    اگر (ngx_inet_resolve_host(pool, u) != NGX_OK) {
        واپس NGX_ERROR;
    }

    u->family = u->addrs[0].sockaddr->sa_family;
    u->socklen = u->addrs[0].socklen;
    ngx_memcpy(&u->sockaddr, u->addrs[0].sockaddr, u->addrs[0].socklen);
    u->wildcard = ngx_inet_wildcard(&u->sockaddr.sockaddr);

    واپس NGX_OK;
}


ساکن ngx_int_t
ngx_parse_inet6_url(ngx_pool_t *pool, ngx_url_t *u)
{
#اگر (NGX_HAVE_INET6)
    u_char               *p, *host, *پورٹ, *آخری, *uri, *dash;
    سائز_t                len;
    ngx_int_t             n;
    ساخت sockaddr_in6  *sin6;

    u->socklen = حجم(ساخت sockaddr_in6);
    sin6 = (ساخت sockaddr_in6 *) &u->sockaddr;
    sin6->sin6_family = AF_INET6;

    host = u->url.ڈیٹا + 1;

    آخری = u->url.ڈیٹا + u->url.len;

    p = ngx_strlchr(host, آخری, ']');

    اگر (p == تہی) {
        u->err = "invalid host";
        واپس NGX_ERROR;
    }

    پورٹ = p + 1;

    uri = ngx_strlchr(پورٹ, آخری, '/');

    اگر (uri) {
        اگر (u->listen || !u->uri_part) {
            u->err = "invalid host";
            واپس NGX_ERROR;
        }

        u->uri.len = آخری - uri;
        u->uri.ڈیٹا = uri;

        آخری = uri;
    }

    اگر (پورٹ < آخری) {
        اگر (*پورٹ != ':') {
            u->err = "invalid host";
            واپس NGX_ERROR;
        }

        پورٹ++;

        len = آخری - پورٹ;

        اگر (u->listen) {
            dash = ngx_strlchr(پورٹ, آخری, '-');

            اگر (dash) {
                dash++;

                n = ngx_atoi(dash, آخری - dash);

                اگر (n < 1 || n > 65535) {
                    u->err = "invalid port";
                    واپس NGX_ERROR;
                }

                u->آخری_پورٹ = (اندر_پورٹ_t) n;

                len = dash - پورٹ - 1;
            }
        }

        n = ngx_atoi(پورٹ, len);

        اگر (n < 1 || n > 65535) {
            u->err = "invalid port";
            واپس NGX_ERROR;
        }

        اگر (u->آخری_پورٹ && n > u->آخری_پورٹ) {
            u->err = "invalid port range";
            واپس NGX_ERROR;
        }

        u->پورٹ = (اندر_پورٹ_t) n;
        sin6->sin6_پورٹ = htons((اندر_پورٹ_t) n);

        u->پورٹ_متن.len = آخری - پورٹ;
        u->پورٹ_متن.ڈیٹا = پورٹ;

    } ورنہ {
        u->no_پورٹ = 1;
        u->پورٹ = u->default_پورٹ;
        sin6->sin6_پورٹ = htons(u->default_پورٹ);
    }

    len = p - host;

    اگر (len == 0) {
        u->err = "no host";
        واپس NGX_ERROR;
    }

    u->host.len = len + 2;
    u->host.ڈیٹا = host - 1;

    اگر (ngx_inet6_addr(host, len, sin6->sin6_addr.s6_addr) != NGX_OK) {
        u->err = "invalid IPv6 address";
        واپس NGX_ERROR;
    }

    اگر (IN6_IS_ADDR_UNSPECIFIED(&sin6->sin6_addr)) {
        u->wildcard = 1;
    }

    u->family = AF_INET6;

    واپس ngx_inet_جوڑو_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);

#ورنہ

    u->err = "the INET6 sockets are not supported on this platform";

    واپس NGX_ERROR;

#اگر_ختم
}


#اگر (NGX_HAVE_GETADDRINFO && NGX_HAVE_INET6)

ngx_int_t
ngx_inet_resolve_host(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char           *host;
    ngx_uint_t        n;
    ساخت addrinfo   hints, *res, *rp;

    host = ngx_alloc(u->host.len + 1, pool->لوگارتھم);
    اگر (host == تہی) {
        واپس NGX_ERROR;
    }

    (خالی) ngx_cpystrn(host, u->host.ڈیٹا, u->host.len + 1);

    ngx_memzero(&hints, حجم(ساخت addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_دھارا;
#اگر_تعریف_شدہ AI_ADDRCONFIG
    hints.ai_flags = AI_ADDRCONFIG;
#اگر_ختم

    اگر (getaddrinfo((حرف *) host, تہی, &hints, &res) != 0) {
        u->err = "host not found";
        ngx_free(host);
        واپس NGX_ERROR;
    }

    ngx_free(host);

    برائے (n = 0, rp = res; rp != تہی; rp = rp->ai_اگلا) {

        انتخاب (rp->ai_family) {

        حالت AF_INET:
        حالت AF_INET6:
            توڑو;

        طےشدہ:
            جاری;
        }

        n++;
    }

    اگر (n == 0) {
        u->err = "host not found";
        جاؤ failed;
    }

    /* MP: ngx_shared_palloc() */

    برائے (rp = res; rp != تہی; rp = rp->ai_اگلا) {

        انتخاب (rp->ai_family) {

        حالت AF_INET:
        حالت AF_INET6:
            توڑو;

        طےشدہ:
            جاری;
        }

        اگر (ngx_inet_جوڑو_addr(pool, u, rp->ai_addr, rp->ai_addrlen, n)
            != NGX_OK)
        {
            جاؤ failed;
        }
    }

    freeaddrinfo(res);
    واپس NGX_OK;

failed:

    freeaddrinfo(res);
    واپس NGX_ERROR;
}

#ورنہ /* !NGX_HAVE_GETADDRINFO || !NGX_HAVE_INET6 */

ngx_int_t
ngx_inet_resolve_host(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char              *host;
    ngx_uint_t           i, n;
    ساخت hostent      *h;
    ساخت sockaddr_اندر   جیب;

    /* AF_INET only */

    ngx_memzero(&جیب, حجم(ساخت sockaddr_اندر));

    جیب.sin_family = AF_INET;
    جیب.sin_addr.s_addr = ngx_inet_addr(u->host.ڈیٹا, u->host.len);

    اگر (جیب.sin_addr.s_addr == INADDR_NONE) {
        host = ngx_alloc(u->host.len + 1, pool->لوگارتھم);
        اگر (host == تہی) {
            واپس NGX_ERROR;
        }

        (خالی) ngx_cpystrn(host, u->host.ڈیٹا, u->host.len + 1);

        h = gethostbyname((حرف *) host);

        ngx_free(host);

        اگر (h == تہی || h->h_addr_list[0] == تہی) {
            u->err = "host not found";
            واپس NGX_ERROR;
        }

        برائے (n = 0; h->h_addr_list[n] != تہی; n++) { /* void */ }

        /* MP: ngx_shared_palloc() */

        برائے (i = 0; i < n; i++) {
            جیب.sin_addr.s_addr = *(اندر_addr_t *) (h->h_addr_list[i]);

            اگر (ngx_inet_جوڑو_addr(pool, u, (ساخت sockaddr *) &جیب,
                                  حجم(ساخت sockaddr_اندر), n)
                != NGX_OK)
            {
                واپس NGX_ERROR;
            }
        }

    } ورنہ {

        /* MP: ngx_shared_palloc() */

        اگر (ngx_inet_جوڑو_addr(pool, u, (ساخت sockaddr *) &جیب,
                              حجم(ساخت sockaddr_اندر), 1)
            != NGX_OK)
        {
            واپس NGX_ERROR;
        }
    }

    واپس NGX_OK;
}

#اگر_ختم /* NGX_HAVE_GETADDRINFO && NGX_HAVE_INET6 */


ساکن ngx_int_t
ngx_inet_جوڑو_addr(ngx_pool_t *pool, ngx_url_t *u, ساخت sockaddr *sockaddr,
    socklen_t socklen, ngx_uint_t کل)
{
    u_char           *p;
    سائز_t            len;
    ngx_uint_t        i, nports;
    ngx_addr_t       *addr;
    ساخت sockaddr  *sa;

    nports = u->آخری_پورٹ ? u->آخری_پورٹ - u->پورٹ + 1 : 1;

    اگر (u->addrs == تہی) {
        u->addrs = ngx_palloc(pool, کل * nports * حجم(ngx_addr_t));
        اگر (u->addrs == تہی) {
            واپس NGX_ERROR;
        }
    }

    برائے (i = 0; i < nports; i++) {
        sa = ngx_pcalloc(pool, socklen);
        اگر (sa == تہی) {
            واپس NGX_ERROR;
        }

        ngx_memcpy(sa, sockaddr, socklen);

        ngx_inet_سیٹ_پورٹ(sa, u->پورٹ + i);

        انتخاب (sa->sa_family) {

#اگر (NGX_HAVE_INET6)
        حالت AF_INET6:
            len = NGX_INET6_ADDRSTRLEN + حجم("[]:65536") - 1;
            توڑو;
#اگر_ختم

        طےشدہ: /* AF_INET */
            len = NGX_INET_ADDRSTRLEN + حجم(":65535") - 1;
        }

        p = ngx_pnalloc(pool, len);
        اگر (p == تہی) {
            واپس NGX_ERROR;
        }

        len = ngx_sock_ntop(sa, socklen, p, len, 1);

        addr = &u->addrs[u->naddrs++];

        addr->sockaddr = sa;
        addr->socklen = socklen;

        addr->name.len = len;
        addr->name.ڈیٹا = p;
    }

    واپس NGX_OK;
}


ngx_int_t
ngx_cmp_sockaddr(ساخت sockaddr *sa1, socklen_t slen1,
    ساخت sockaddr *sa2, socklen_t slen2, ngx_uint_t cmp_پورٹ)
{
    ساخت sockaddr_اندر   *sin1, *sin2;
#اگر (NGX_HAVE_INET6)
    ساخت sockaddr_in6  *sin61, *sin62;
#اگر_ختم
#اگر (NGX_HAVE_UNIX_DOMAIN)
    سائز_t                len;
    ساخت sockaddr_un   *saun1, *saun2;
#اگر_ختم

    اگر (sa1->sa_family != sa2->sa_family) {
        واپس NGX_DECLINED;
    }

    انتخاب (sa1->sa_family) {

#اگر (NGX_HAVE_INET6)
    حالت AF_INET6:

        sin61 = (ساخت sockaddr_in6 *) sa1;
        sin62 = (ساخت sockaddr_in6 *) sa2;

        اگر (cmp_پورٹ && sin61->sin6_پورٹ != sin62->sin6_پورٹ) {
            واپس NGX_DECLINED;
        }

        اگر (ngx_memcmp(&sin61->sin6_addr, &sin62->sin6_addr, 16) != 0) {
            واپس NGX_DECLINED;
        }

        توڑو;
#اگر_ختم

#اگر (NGX_HAVE_UNIX_DOMAIN)
    حالت AF_UNIX:

        saun1 = (ساخت sockaddr_un *) sa1;
        saun2 = (ساخت sockaddr_un *) sa2;

        اگر (slen1 < slen2) {
            len = slen1 - offsetof(ساخت sockaddr_un, sun_راستہ);

        } ورنہ {
            len = slen2 - offsetof(ساخت sockaddr_un, sun_راستہ);
        }

        اگر (len > حجم(saun1->sun_راستہ)) {
            len = حجم(saun1->sun_راستہ);
        }

        اگر (ngx_memcmp(&saun1->sun_راستہ, &saun2->sun_راستہ, len) != 0) {
            واپس NGX_DECLINED;
        }

        توڑو;
#اگر_ختم

    طےشدہ: /* AF_INET */

        sin1 = (ساخت sockaddr_اندر *) sa1;
        sin2 = (ساخت sockaddr_اندر *) sa2;

        اگر (cmp_پورٹ && sin1->sin_پورٹ != sin2->sin_پورٹ) {
            واپس NGX_DECLINED;
        }

        اگر (sin1->sin_addr.s_addr != sin2->sin_addr.s_addr) {
            واپس NGX_DECLINED;
        }

        توڑو;
    }

    واپس NGX_OK;
}


اندر_پورٹ_t
ngx_inet_get_پورٹ(ساخت sockaddr *sa)
{
    ساخت sockaddr_اندر   *جیب;
#اگر (NGX_HAVE_INET6)
    ساخت sockaddr_in6  *sin6;
#اگر_ختم

    انتخاب (sa->sa_family) {

#اگر (NGX_HAVE_INET6)
    حالت AF_INET6:
        sin6 = (ساخت sockaddr_in6 *) sa;
        واپس ntohs(sin6->sin6_پورٹ);
#اگر_ختم

#اگر (NGX_HAVE_UNIX_DOMAIN)
    حالت AF_UNIX:
        واپس 0;
#اگر_ختم

    طےشدہ: /* AF_INET */
        جیب = (ساخت sockaddr_اندر *) sa;
        واپس ntohs(جیب->sin_پورٹ);
    }
}


خالی
ngx_inet_سیٹ_پورٹ(ساخت sockaddr *sa, اندر_پورٹ_t پورٹ)
{
    ساخت sockaddr_اندر   *جیب;
#اگر (NGX_HAVE_INET6)
    ساخت sockaddr_in6  *sin6;
#اگر_ختم

    انتخاب (sa->sa_family) {

#اگر (NGX_HAVE_INET6)
    حالت AF_INET6:
        sin6 = (ساخت sockaddr_in6 *) sa;
        sin6->sin6_پورٹ = htons(پورٹ);
        توڑو;
#اگر_ختم

#اگر (NGX_HAVE_UNIX_DOMAIN)
    حالت AF_UNIX:
        توڑو;
#اگر_ختم

    طےشدہ: /* AF_INET */
        جیب = (ساخت sockaddr_اندر *) sa;
        جیب->sin_پورٹ = htons(پورٹ);
        توڑو;
    }
}


ngx_uint_t
ngx_inet_wildcard(ساخت sockaddr *sa)
{
    ساخت sockaddr_اندر   *جیب;
#اگر (NGX_HAVE_INET6)
    ساخت sockaddr_in6  *sin6;
#اگر_ختم

    انتخاب (sa->sa_family) {

    حالت AF_INET:
        جیب = (ساخت sockaddr_اندر *) sa;

        اگر (جیب->sin_addr.s_addr == INADDR_ANY) {
            واپس 1;
        }

        توڑو;

#اگر (NGX_HAVE_INET6)

    حالت AF_INET6:
        sin6 = (ساخت sockaddr_in6 *) sa;

        اگر (IN6_IS_ADDR_UNSPECIFIED(&sin6->sin6_addr)) {
            واپس 1;
        }

        توڑو;

#اگر_ختم
    }

    واپس 0;
}
