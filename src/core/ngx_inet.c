
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>


ਸਥਿਰ ngx_int_t ngx_parse_unix_domain_url(ngx_pool_t *pool, ngx_url_t *u);
ਸਥਿਰ ngx_int_t ngx_parse_inet_url(ngx_pool_t *pool, ngx_url_t *u);
ਸਥਿਰ ngx_int_t ngx_parse_inet6_url(ngx_pool_t *pool, ngx_url_t *u);
ਸਥਿਰ ngx_int_t ngx_inet_ਜੋੜੋ_addr(ngx_pool_t *pool, ngx_url_t *u,
    ਬਣਤਰ sockaddr *sockaddr, socklen_t socklen, ngx_uint_t ਕੁੱਲ);


ਵਿੱਚ_addr_t
ngx_inet_addr(u_char *ਪਾਠ, ਮਾਪ_t len)
{
    u_char      *p, c;
    ਵਿੱਚ_addr_t    addr;
    ngx_uint_t   octet, n;

    addr = 0;
    octet = 0;
    n = 0;

    ਲਈ (p = ਪਾਠ; p < ਪਾਠ + len; p++) {
        c = *p;

        ਜੇ (c >= '0' && c <= '9') {
            octet = octet * 10 + (c - '0');

            ਜੇ (octet > 255) {
                ਵਾਪਸ INADDR_NONE;
            }

            ਜਾਰੀ;
        }

        ਜੇ (c == '.') {
            addr = (addr << 8) + octet;
            octet = 0;
            n++;
            ਜਾਰੀ;
        }

        ਵਾਪਸ INADDR_NONE;
    }

    ਜੇ (n == 3) {
        addr = (addr << 8) + octet;
        ਵਾਪਸ htonl(addr);
    }

    ਵਾਪਸ INADDR_NONE;
}


#ਜੇ (NGX_HAVE_INET6)

ngx_int_t
ngx_inet6_addr(u_char *p, ਮਾਪ_t len, u_char *addr)
{
    u_char      c, *zero, *digit, *s, *d;
    ਮਾਪ_t      len4;
    ngx_uint_t  n, nibbles, ਸ਼ਬਦ;

    ਜੇ (len == 0) {
        ਵਾਪਸ NGX_ERROR;
    }

    zero = ਨਿਰਾ;
    digit = ਨਿਰਾ;
    len4 = 0;
    nibbles = 0;
    ਸ਼ਬਦ = 0;
    n = 8;

    ਜੇ (p[0] == ':') {
        p++;
        len--;
    }

    ਲਈ (/* void */; len; len--) {
        c = *p++;

        ਜੇ (c == ':') {
            ਜੇ (nibbles) {
                digit = p;
                len4 = len;
                *addr++ = (u_char) (ਸ਼ਬਦ >> 8);
                *addr++ = (u_char) (ਸ਼ਬਦ & 0xff);

                ਜੇ (--n) {
                    nibbles = 0;
                    ਸ਼ਬਦ = 0;
                    ਜਾਰੀ;
                }

            } ਨਹੀਂਤਾਂ {
                ਜੇ (zero == ਨਿਰਾ) {
                    digit = p;
                    len4 = len;
                    zero = addr;
                    ਜਾਰੀ;
                }
            }

            ਵਾਪਸ NGX_ERROR;
        }

        ਜੇ (c == '.' && nibbles) {
            ਜੇ (n < 2 || digit == ਨਿਰਾ) {
                ਵਾਪਸ NGX_ERROR;
            }

            ਸ਼ਬਦ = ngx_inet_addr(digit, len4 - 1);
            ਜੇ (ਸ਼ਬਦ == INADDR_NONE) {
                ਵਾਪਸ NGX_ERROR;
            }

            ਸ਼ਬਦ = ntohl(ਸ਼ਬਦ);
            *addr++ = (u_char) ((ਸ਼ਬਦ >> 24) & 0xff);
            *addr++ = (u_char) ((ਸ਼ਬਦ >> 16) & 0xff);
            n--;
            ਰੋਕੋ;
        }

        ਜੇ (++nibbles > 4) {
            ਵਾਪਸ NGX_ERROR;
        }

        ਜੇ (c >= '0' && c <= '9') {
            ਸ਼ਬਦ = ਸ਼ਬਦ * 16 + (c - '0');
            ਜਾਰੀ;
        }

        c |= 0x20;

        ਜੇ (c >= 'a' && c <= 'f') {
            ਸ਼ਬਦ = ਸ਼ਬਦ * 16 + (c - 'a') + 10;
            ਜਾਰੀ;
        }

        ਵਾਪਸ NGX_ERROR;
    }

    ਜੇ (nibbles == 0 && zero == ਨਿਰਾ) {
        ਵਾਪਸ NGX_ERROR;
    }

    *addr++ = (u_char) (ਸ਼ਬਦ >> 8);
    *addr++ = (u_char) (ਸ਼ਬਦ & 0xff);

    ਜੇ (--n) {
        ਜੇ (zero) {
            n *= 2;
            s = addr - 1;
            d = s + n;
            ਜਦੋਂ (s >= zero) {
                *d-- = *s--;
            }
            ngx_memzero(zero, n);
            ਵਾਪਸ NGX_OK;
        }

    } ਨਹੀਂਤਾਂ {
        ਜੇ (zero == ਨਿਰਾ) {
            ਵਾਪਸ NGX_OK;
        }
    }

    ਵਾਪਸ NGX_ERROR;
}

#ਜੇ_ਅੰਤ


ਮਾਪ_t
ngx_sock_ntop(ਬਣਤਰ sockaddr *sa, socklen_t socklen, u_char *ਪਾਠ, ਮਾਪ_t len,
    ngx_uint_t ਪੋਰਟ)
{
    u_char               *p;
#ਜੇ (NGX_HAVE_INET6 || NGX_HAVE_UNIX_DOMAIN)
    ਮਾਪ_t                n;
#ਜੇ_ਅੰਤ
    ਬਣਤਰ sockaddr_ਵਿੱਚ   *ਜੀਵਾ;
#ਜੇ (NGX_HAVE_INET6)
    ਬਣਤਰ sockaddr_in6  *sin6;
#ਜੇ_ਅੰਤ
#ਜੇ (NGX_HAVE_UNIX_DOMAIN)
    ਬਣਤਰ sockaddr_un   *saun;
#ਜੇ_ਅੰਤ

    ਚੋਣ (sa->sa_family) {

    ਹਾਲਤ AF_INET:

        ਜੀਵਾ = (ਬਣਤਰ sockaddr_ਵਿੱਚ *) sa;
        p = (u_char *) &ਜੀਵਾ->sin_addr;

        ਜੇ (ਪੋਰਟ) {
            p = ngx_snprintf(ਪਾਠ, len, "%ud.%ud.%ud.%ud:%d",
                             p[0], p[1], p[2], p[3], ntohs(ਜੀਵਾ->sin_ਪੋਰਟ));
        } ਨਹੀਂਤਾਂ {
            p = ngx_snprintf(ਪਾਠ, len, "%ud.%ud.%ud.%ud",
                             p[0], p[1], p[2], p[3]);
        }

        ਵਾਪਸ (p - ਪਾਠ);

#ਜੇ (NGX_HAVE_INET6)

    ਹਾਲਤ AF_INET6:

        sin6 = (ਬਣਤਰ sockaddr_in6 *) sa;

        n = 0;

        ਜੇ (ਪੋਰਟ) {
            ਪਾਠ[n++] = '[';
        }

        n = ngx_inet6_ntop(sin6->sin6_addr.s6_addr, &ਪਾਠ[n], len);

        ਜੇ (ਪੋਰਟ) {
            n = ngx_sprintf(&ਪਾਠ[1 + n], "]:%d",
                            ntohs(sin6->sin6_ਪੋਰਟ)) - ਪਾਠ;
        }

        ਵਾਪਸ n;
#ਜੇ_ਅੰਤ

#ਜੇ (NGX_HAVE_UNIX_DOMAIN)

    ਹਾਲਤ AF_UNIX:
        saun = (ਬਣਤਰ sockaddr_un *) sa;

        /* on Linux sockaddr might not include sun_path at all */

        ਜੇ (socklen <= (socklen_t) offsetof(ਬਣਤਰ sockaddr_un, sun_ਰਸਤਾ)) {
            p = ngx_snprintf(ਪਾਠ, len, "unix:%Z");

        } ਨਹੀਂਤਾਂ {
            n = ngx_strnlen((u_char *) saun->sun_ਰਸਤਾ,
                            socklen - offsetof(ਬਣਤਰ sockaddr_un, sun_ਰਸਤਾ));
            p = ngx_snprintf(ਪਾਠ, len, "unix:%*s%Z", n, saun->sun_ਰਸਤਾ);
        }

        /* we do not include trailing zero in address length */

        ਵਾਪਸ (p - ਪਾਠ - 1);

#ਜੇ_ਅੰਤ

    ਮੂਲ:
        ਵਾਪਸ 0;
    }
}


ਮਾਪ_t
ngx_inet_ntop(ਪੂਰਨ family, ਖਾਲੀ *addr, u_char *ਪਾਠ, ਮਾਪ_t len)
{
    u_char  *p;

    ਚੋਣ (family) {

    ਹਾਲਤ AF_INET:

        p = addr;

        ਵਾਪਸ ngx_snprintf(ਪਾਠ, len, "%ud.%ud.%ud.%ud",
                            p[0], p[1], p[2], p[3])
               - ਪਾਠ;

#ਜੇ (NGX_HAVE_INET6)

    ਹਾਲਤ AF_INET6:
        ਵਾਪਸ ngx_inet6_ntop(addr, ਪਾਠ, len);

#ਜੇ_ਅੰਤ

    ਮੂਲ:
        ਵਾਪਸ 0;
    }
}


#ਜੇ (NGX_HAVE_INET6)

ਮਾਪ_t
ngx_inet6_ntop(u_char *p, u_char *ਪਾਠ, ਮਾਪ_t len)
{
    u_char      *dst;
    ਮਾਪ_t       max, n;
    ngx_uint_t   i, zero, ਆਖਰੀ;

    ਜੇ (len < NGX_INET6_ADDRSTRLEN) {
        ਵਾਪਸ 0;
    }

    zero = (ngx_uint_t) -1;
    ਆਖਰੀ = (ngx_uint_t) -1;
    max = 1;
    n = 0;

    ਲਈ (i = 0; i < 16; i += 2) {

        ਜੇ (p[i] || p[i + 1]) {

            ਜੇ (max < n) {
                zero = ਆਖਰੀ;
                max = n;
            }

            n = 0;
            ਜਾਰੀ;
        }

        ਜੇ (n++ == 0) {
            ਆਖਰੀ = i;
        }
    }

    ਜੇ (max < n) {
        zero = ਆਖਰੀ;
        max = n;
    }

    dst = ਪਾਠ;
    n = 16;

    ਜੇ (zero == 0) {

        ਜੇ ((max == 5 && p[10] == 0xff && p[11] == 0xff)
            || (max == 6)
            || (max == 7 && p[14] != 0 && p[15] != 1))
        {
            n = 12;
        }

        *dst++ = ':';
    }

    ਲਈ (i = 0; i < n; i += 2) {

        ਜੇ (i == zero) {
            *dst++ = ':';
            i += (max - 1) * 2;
            ਜਾਰੀ;
        }

        dst = ngx_sprintf(dst, "%xd", p[i] * 256 + p[i + 1]);

        ਜੇ (i < 14) {
            *dst++ = ':';
        }
    }

    ਜੇ (n == 12) {
        dst = ngx_sprintf(dst, "%ud.%ud.%ud.%ud", p[12], p[13], p[14], p[15]);
    }

    ਵਾਪਸ dst - ਪਾਠ;
}

#ਜੇ_ਅੰਤ


ngx_int_t
ngx_ptocidr(ngx_str_t *ਪਾਠ, ngx_cidr_t *cidr)
{
    u_char      *addr, *ਨਕਾਬ, *ਆਖਰੀ;
    ਮਾਪ_t       len;
    ngx_int_t    ਖਿਸਕਾਓ;
#ਜੇ (NGX_HAVE_INET6)
    ngx_int_t    rc;
    ngx_uint_t   s, i;
#ਜੇ_ਅੰਤ

    addr = ਪਾਠ->ਅੰਕੜੇ;
    ਆਖਰੀ = addr + ਪਾਠ->len;

    ਨਕਾਬ = ngx_strlchr(addr, ਆਖਰੀ, '/');
    len = (ਨਕਾਬ ? ਨਕਾਬ : ਆਖਰੀ) - addr;

    cidr->u.ਵਿੱਚ.addr = ngx_inet_addr(addr, len);

    ਜੇ (cidr->u.ਵਿੱਚ.addr != INADDR_NONE) {
        cidr->family = AF_INET;

        ਜੇ (ਨਕਾਬ == ਨਿਰਾ) {
            cidr->u.ਵਿੱਚ.ਨਕਾਬ = 0xffffffff;
            ਵਾਪਸ NGX_OK;
        }

#ਜੇ (NGX_HAVE_INET6)
    } ਨਹੀਂਤਾਂ ਜੇ (ngx_inet6_addr(addr, len, cidr->u.in6.addr.s6_addr) == NGX_OK) {
        cidr->family = AF_INET6;

        ਜੇ (ਨਕਾਬ == ਨਿਰਾ) {
            ngx_memset(cidr->u.in6.ਨਕਾਬ.s6_addr, 0xff, 16);
            ਵਾਪਸ NGX_OK;
        }

#ਜੇ_ਅੰਤ
    } ਨਹੀਂਤਾਂ {
        ਵਾਪਸ NGX_ERROR;
    }

    ਨਕਾਬ++;

    ਖਿਸਕਾਓ = ngx_atoi(ਨਕਾਬ, ਆਖਰੀ - ਨਕਾਬ);
    ਜੇ (ਖਿਸਕਾਓ == NGX_ERROR) {
        ਵਾਪਸ NGX_ERROR;
    }

    ਚੋਣ (cidr->family) {

#ਜੇ (NGX_HAVE_INET6)
    ਹਾਲਤ AF_INET6:
        ਜੇ (ਖਿਸਕਾਓ > 128) {
            ਵਾਪਸ NGX_ERROR;
        }

        addr = cidr->u.in6.addr.s6_addr;
        ਨਕਾਬ = cidr->u.in6.ਨਕਾਬ.s6_addr;
        rc = NGX_OK;

        ਲਈ (i = 0; i < 16; i++) {

            s = (ਖਿਸਕਾਓ > 8) ? 8 : ਖਿਸਕਾਓ;
            ਖਿਸਕਾਓ -= s;

            ਨਕਾਬ[i] = (u_char) (0xffu << (8 - s));

            ਜੇ (addr[i] != (addr[i] & ਨਕਾਬ[i])) {
                rc = NGX_ਪੂਰਾ;
                addr[i] &= ਨਕਾਬ[i];
            }
        }

        ਵਾਪਸ rc;
#ਜੇ_ਅੰਤ

    ਮੂਲ: /* AF_INET */
        ਜੇ (ਖਿਸਕਾਓ > 32) {
            ਵਾਪਸ NGX_ERROR;
        }

        ਜੇ (ਖਿਸਕਾਓ) {
            cidr->u.ਵਿੱਚ.ਨਕਾਬ = htonl((uint32_t) (0xffffffffu << (32 - ਖਿਸਕਾਓ)));

        } ਨਹੀਂਤਾਂ {
            /* x86 compilers use a shl instruction that shifts by modulo 32 */
            cidr->u.ਵਿੱਚ.ਨਕਾਬ = 0;
        }

        ਜੇ (cidr->u.ਵਿੱਚ.addr == (cidr->u.ਵਿੱਚ.addr & cidr->u.ਵਿੱਚ.ਨਕਾਬ)) {
            ਵਾਪਸ NGX_OK;
        }

        cidr->u.ਵਿੱਚ.addr &= cidr->u.ਵਿੱਚ.ਨਕਾਬ;

        ਵਾਪਸ NGX_ਪੂਰਾ;
    }
}


ngx_int_t
ngx_cidr_ਮੇਲ(ਬਣਤਰ sockaddr *sa, ngx_array_t *cidrs)
{
#ਜੇ (NGX_HAVE_INET6)
    u_char           *p;
#ਜੇ_ਅੰਤ
    ਵਿੱਚ_addr_t         inaddr;
    ngx_cidr_t       *cidr;
    ngx_uint_t        family, i;
#ਜੇ (NGX_HAVE_INET6)
    ngx_uint_t        n;
    ਬਣਤਰ in6_addr  *inaddr6;
#ਜੇ_ਅੰਤ

#ਜੇ (NGX_SUPPRESS_WARN)
    inaddr = 0;
#ਜੇ (NGX_HAVE_INET6)
    inaddr6 = ਨਿਰਾ;
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

    family = sa->sa_family;

    ਜੇ (family == AF_INET) {
        inaddr = ((ਬਣਤਰ sockaddr_ਵਿੱਚ *) sa)->sin_addr.s_addr;
    }

#ਜੇ (NGX_HAVE_INET6)
    ਨਹੀਂਤਾਂ ਜੇ (family == AF_INET6) {
        inaddr6 = &((ਬਣਤਰ sockaddr_in6 *) sa)->sin6_addr;

        ਜੇ (IN6_IS_ADDR_V4MAPPED(inaddr6)) {
            family = AF_INET;

            p = inaddr6->s6_addr;

            inaddr = (ਵਿੱਚ_addr_t) p[12] << 24;
            inaddr += p[13] << 16;
            inaddr += p[14] << 8;
            inaddr += p[15];

            inaddr = htonl(inaddr);
        }
    }
#ਜੇ_ਅੰਤ

    ਲਈ (cidr = cidrs->elts, i = 0; i < cidrs->nelts; i++) {
        ਜੇ (cidr[i].family != family) {
            ਜਾਓ ਅਗਲਾ;
        }

        ਚੋਣ (family) {

#ਜੇ (NGX_HAVE_INET6)
        ਹਾਲਤ AF_INET6:
            ਲਈ (n = 0; n < 16; n++) {
                ਜੇ ((inaddr6->s6_addr[n] & cidr[i].u.in6.ਨਕਾਬ.s6_addr[n])
                    != cidr[i].u.in6.addr.s6_addr[n])
                {
                    ਜਾਓ ਅਗਲਾ;
                }
            }
            ਰੋਕੋ;
#ਜੇ_ਅੰਤ

#ਜੇ (NGX_HAVE_UNIX_DOMAIN)
        ਹਾਲਤ AF_UNIX:
            ਰੋਕੋ;
#ਜੇ_ਅੰਤ

        ਮੂਲ: /* AF_INET */
            ਜੇ ((inaddr & cidr[i].u.ਵਿੱਚ.ਨਕਾਬ) != cidr[i].u.ਵਿੱਚ.addr) {
                ਜਾਓ ਅਗਲਾ;
            }
            ਰੋਕੋ;
        }

        ਵਾਪਸ NGX_OK;

    ਅਗਲਾ:
        ਜਾਰੀ;
    }

    ਵਾਪਸ NGX_DECLINED;
}


ngx_int_t
ngx_parse_addr(ngx_pool_t *pool, ngx_addr_t *addr, u_char *ਪਾਠ, ਮਾਪ_t len)
{
    ਵਿੱਚ_addr_t             inaddr;
    ngx_uint_t            family;
    ਬਣਤਰ sockaddr_ਵਿੱਚ   *ਜੀਵਾ;
#ਜੇ (NGX_HAVE_INET6)
    ਬਣਤਰ in6_addr       inaddr6;
    ਬਣਤਰ sockaddr_in6  *sin6;

    /*
     * prevent MSVC8 warning:
     *    potentially uninitialized local variable 'inaddr6' used
     */
    ngx_memzero(&inaddr6, ਆਕਾਰ(ਬਣਤਰ in6_addr));
#ਜੇ_ਅੰਤ

    inaddr = ngx_inet_addr(ਪਾਠ, len);

    ਜੇ (inaddr != INADDR_NONE) {
        family = AF_INET;
        len = ਆਕਾਰ(ਬਣਤਰ sockaddr_ਵਿੱਚ);

#ਜੇ (NGX_HAVE_INET6)
    } ਨਹੀਂਤਾਂ ਜੇ (ngx_inet6_addr(ਪਾਠ, len, inaddr6.s6_addr) == NGX_OK) {
        family = AF_INET6;
        len = ਆਕਾਰ(ਬਣਤਰ sockaddr_in6);

#ਜੇ_ਅੰਤ
    } ਨਹੀਂਤਾਂ {
        ਵਾਪਸ NGX_DECLINED;
    }

    addr->sockaddr = ngx_pcalloc(pool, len);
    ਜੇ (addr->sockaddr == ਨਿਰਾ) {
        ਵਾਪਸ NGX_ERROR;
    }

    addr->sockaddr->sa_family = (u_char) family;
    addr->socklen = len;

    ਚੋਣ (family) {

#ਜੇ (NGX_HAVE_INET6)
    ਹਾਲਤ AF_INET6:
        sin6 = (ਬਣਤਰ sockaddr_in6 *) addr->sockaddr;
        ngx_memcpy(sin6->sin6_addr.s6_addr, inaddr6.s6_addr, 16);
        ਰੋਕੋ;
#ਜੇ_ਅੰਤ

    ਮੂਲ: /* AF_INET */
        ਜੀਵਾ = (ਬਣਤਰ sockaddr_ਵਿੱਚ *) addr->sockaddr;
        ਜੀਵਾ->sin_addr.s_addr = inaddr;
        ਰੋਕੋ;
    }

    ਵਾਪਸ NGX_OK;
}


ngx_int_t
ngx_parse_addr_ਪੋਰਟ(ngx_pool_t *pool, ngx_addr_t *addr, u_char *ਪਾਠ,
    ਮਾਪ_t len)
{
    u_char     *p, *ਆਖਰੀ;
    ਮਾਪ_t      plen;
    ngx_int_t   rc, ਪੋਰਟ;

    rc = ngx_parse_addr(pool, addr, ਪਾਠ, len);

    ਜੇ (rc != NGX_DECLINED) {
        ਵਾਪਸ rc;
    }

    ਆਖਰੀ = ਪਾਠ + len;

#ਜੇ (NGX_HAVE_INET6)
    ਜੇ (len && ਪਾਠ[0] == '[') {

        p = ngx_strlchr(ਪਾਠ, ਆਖਰੀ, ']');

        ਜੇ (p == ਆਖਰੀ - 1) {
            ਵਾਪਸ ngx_parse_addr(pool, addr, ਪਾਠ + 1, len - 2);
        }

        ਜੇ (p == ਨਿਰਾ || *++p != ':') {
            ਵਾਪਸ NGX_DECLINED;
        }

        ਪਾਠ++;
        len -= 2;

    } ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ

    {
        p = ngx_strlchr(ਪਾਠ, ਆਖਰੀ, ':');

        ਜੇ (p == ਨਿਰਾ) {
            ਵਾਪਸ NGX_DECLINED;
        }
    }

    p++;
    plen = ਆਖਰੀ - p;

    ਪੋਰਟ = ngx_atoi(p, plen);

    ਜੇ (ਪੋਰਟ < 1 || ਪੋਰਟ > 65535) {
        ਵਾਪਸ NGX_DECLINED;
    }

    len -= plen + 1;

    rc = ngx_parse_addr(pool, addr, ਪਾਠ, len);

    ਜੇ (rc != NGX_OK) {
        ਵਾਪਸ rc;
    }

    ngx_inet_ਸੈੱਟ_ਪੋਰਟ(addr->sockaddr, (ਵਿੱਚ_ਪੋਰਟ_t) ਪੋਰਟ);

    ਵਾਪਸ NGX_OK;
}


ngx_int_t
ngx_parse_url(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char  *p;
    ਮਾਪ_t   len;

    p = u->url.ਅੰਕੜੇ;
    len = u->url.len;

    ਜੇ (len >= 5 && ngx_strncasecmp(p, (u_char *) "unix:", 5) == 0) {
        ਵਾਪਸ ngx_parse_unix_domain_url(pool, u);
    }

    ਜੇ (len && p[0] == '[') {
        ਵਾਪਸ ngx_parse_inet6_url(pool, u);
    }

    ਵਾਪਸ ngx_parse_inet_url(pool, u);
}


ਸਥਿਰ ngx_int_t
ngx_parse_unix_domain_url(ngx_pool_t *pool, ngx_url_t *u)
{
#ਜੇ (NGX_HAVE_UNIX_DOMAIN)
    u_char              *ਰਸਤਾ, *uri, *ਆਖਰੀ;
    ਮਾਪ_t               len;
    ਬਣਤਰ sockaddr_un  *saun;

    len = u->url.len;
    ਰਸਤਾ = u->url.ਅੰਕੜੇ;

    ਰਸਤਾ += 5;
    len -= 5;

    ਜੇ (u->uri_part) {

        ਆਖਰੀ = ਰਸਤਾ + len;
        uri = ngx_strlchr(ਰਸਤਾ, ਆਖਰੀ, ':');

        ਜੇ (uri) {
            len = uri - ਰਸਤਾ;
            uri++;
            u->uri.len = ਆਖਰੀ - uri;
            u->uri.ਅੰਕੜੇ = uri;
        }
    }

    ਜੇ (len == 0) {
        u->err = "no path in the unix domain socket";
        ਵਾਪਸ NGX_ERROR;
    }

    u->host.len = len++;
    u->host.ਅੰਕੜੇ = ਰਸਤਾ;

    ਜੇ (len > ਆਕਾਰ(saun->sun_ਰਸਤਾ)) {
        u->err = "too long path in the unix domain socket";
        ਵਾਪਸ NGX_ERROR;
    }

    u->socklen = ਆਕਾਰ(ਬਣਤਰ sockaddr_un);
    saun = (ਬਣਤਰ sockaddr_un *) &u->sockaddr;
    saun->sun_family = AF_UNIX;
    (ਖਾਲੀ) ngx_cpystrn((u_char *) saun->sun_ਰਸਤਾ, ਰਸਤਾ, len);

    u->addrs = ngx_pcalloc(pool, ਆਕਾਰ(ngx_addr_t));
    ਜੇ (u->addrs == ਨਿਰਾ) {
        ਵਾਪਸ NGX_ERROR;
    }

    saun = ngx_pcalloc(pool, ਆਕਾਰ(ਬਣਤਰ sockaddr_un));
    ਜੇ (saun == ਨਿਰਾ) {
        ਵਾਪਸ NGX_ERROR;
    }

    u->family = AF_UNIX;
    u->naddrs = 1;

    saun->sun_family = AF_UNIX;
    (ਖਾਲੀ) ngx_cpystrn((u_char *) saun->sun_ਰਸਤਾ, ਰਸਤਾ, len);

    u->addrs[0].sockaddr = (ਬਣਤਰ sockaddr *) saun;
    u->addrs[0].socklen = ਆਕਾਰ(ਬਣਤਰ sockaddr_un);
    u->addrs[0].name.len = len + 4;
    u->addrs[0].name.ਅੰਕੜੇ = u->url.ਅੰਕੜੇ;

    ਵਾਪਸ NGX_OK;

#ਨਹੀਂਤਾਂ

    u->err = "the unix domain sockets are not supported on this platform";

    ਵਾਪਸ NGX_ERROR;

#ਜੇ_ਅੰਤ
}


ਸਥਿਰ ngx_int_t
ngx_parse_inet_url(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char              *host, *ਪੋਰਟ, *ਆਖਰੀ, *uri, *ਦਲੀਲਾਂ, *dash;
    ਮਾਪ_t               len;
    ngx_int_t            n;
    ਬਣਤਰ sockaddr_ਵਿੱਚ  *ਜੀਵਾ;

    u->socklen = ਆਕਾਰ(ਬਣਤਰ sockaddr_ਵਿੱਚ);
    ਜੀਵਾ = (ਬਣਤਰ sockaddr_ਵਿੱਚ *) &u->sockaddr;
    ਜੀਵਾ->sin_family = AF_INET;

    u->family = AF_INET;

    host = u->url.ਅੰਕੜੇ;

    ਆਖਰੀ = host + u->url.len;

    ਪੋਰਟ = ngx_strlchr(host, ਆਖਰੀ, ':');

    uri = ngx_strlchr(host, ਆਖਰੀ, '/');

    ਦਲੀਲਾਂ = ngx_strlchr(host, ਆਖਰੀ, '?');

    ਜੇ (ਦਲੀਲਾਂ) {
        ਜੇ (uri == ਨਿਰਾ || ਦਲੀਲਾਂ < uri) {
            uri = ਦਲੀਲਾਂ;
        }
    }

    ਜੇ (uri) {
        ਜੇ (u->listen || !u->uri_part) {
            u->err = "invalid host";
            ਵਾਪਸ NGX_ERROR;
        }

        u->uri.len = ਆਖਰੀ - uri;
        u->uri.ਅੰਕੜੇ = uri;

        ਆਖਰੀ = uri;

        ਜੇ (uri < ਪੋਰਟ) {
            ਪੋਰਟ = ਨਿਰਾ;
        }
    }

    ਜੇ (ਪੋਰਟ) {
        ਪੋਰਟ++;

        len = ਆਖਰੀ - ਪੋਰਟ;

        ਜੇ (u->listen) {
            dash = ngx_strlchr(ਪੋਰਟ, ਆਖਰੀ, '-');

            ਜੇ (dash) {
                dash++;

                n = ngx_atoi(dash, ਆਖਰੀ - dash);

                ਜੇ (n < 1 || n > 65535) {
                    u->err = "invalid port";
                    ਵਾਪਸ NGX_ERROR;
                }

                u->ਆਖਰੀ_ਪੋਰਟ = (ਵਿੱਚ_ਪੋਰਟ_t) n;

                len = dash - ਪੋਰਟ - 1;
            }
        }

        n = ngx_atoi(ਪੋਰਟ, len);

        ਜੇ (n < 1 || n > 65535) {
            u->err = "invalid port";
            ਵਾਪਸ NGX_ERROR;
        }

        ਜੇ (u->ਆਖਰੀ_ਪੋਰਟ && n > u->ਆਖਰੀ_ਪੋਰਟ) {
            u->err = "invalid port range";
            ਵਾਪਸ NGX_ERROR;
        }

        u->ਪੋਰਟ = (ਵਿੱਚ_ਪੋਰਟ_t) n;
        ਜੀਵਾ->sin_ਪੋਰਟ = htons((ਵਿੱਚ_ਪੋਰਟ_t) n);

        u->ਪੋਰਟ_ਪਾਠ.len = ਆਖਰੀ - ਪੋਰਟ;
        u->ਪੋਰਟ_ਪਾਠ.ਅੰਕੜੇ = ਪੋਰਟ;

        ਆਖਰੀ = ਪੋਰਟ - 1;

    } ਨਹੀਂਤਾਂ {
        ਜੇ (uri == ਨਿਰਾ) {

            ਜੇ (u->listen) {

                /* test value as port only */

                len = ਆਖਰੀ - host;

                dash = ngx_strlchr(host, ਆਖਰੀ, '-');

                ਜੇ (dash) {
                    dash++;

                    n = ngx_atoi(dash, ਆਖਰੀ - dash);

                    ਜੇ (n == NGX_ERROR) {
                        ਜਾਓ no_ਪੋਰਟ;
                    }

                    ਜੇ (n < 1 || n > 65535) {
                        u->err = "invalid port";

                    } ਨਹੀਂਤਾਂ {
                        u->ਆਖਰੀ_ਪੋਰਟ = (ਵਿੱਚ_ਪੋਰਟ_t) n;
                    }

                    len = dash - host - 1;
                }

                n = ngx_atoi(host, len);

                ਜੇ (n != NGX_ERROR) {

                    ਜੇ (u->err) {
                        ਵਾਪਸ NGX_ERROR;
                    }

                    ਜੇ (n < 1 || n > 65535) {
                        u->err = "invalid port";
                        ਵਾਪਸ NGX_ERROR;
                    }

                    ਜੇ (u->ਆਖਰੀ_ਪੋਰਟ && n > u->ਆਖਰੀ_ਪੋਰਟ) {
                        u->err = "invalid port range";
                        ਵਾਪਸ NGX_ERROR;
                    }

                    u->ਪੋਰਟ = (ਵਿੱਚ_ਪੋਰਟ_t) n;
                    ਜੀਵਾ->sin_ਪੋਰਟ = htons((ਵਿੱਚ_ਪੋਰਟ_t) n);
                    ਜੀਵਾ->sin_addr.s_addr = INADDR_ANY;

                    u->ਪੋਰਟ_ਪਾਠ.len = ਆਖਰੀ - host;
                    u->ਪੋਰਟ_ਪਾਠ.ਅੰਕੜੇ = host;

                    u->wildcard = 1;

                    ਵਾਪਸ ngx_inet_ਜੋੜੋ_addr(pool, u, &u->sockaddr.sockaddr,
                                             u->socklen, 1);
                }
            }
        }

no_ਪੋਰਟ:

        u->err = ਨਿਰਾ;
        u->no_ਪੋਰਟ = 1;
        u->ਪੋਰਟ = u->default_ਪੋਰਟ;
        ਜੀਵਾ->sin_ਪੋਰਟ = htons(u->default_ਪੋਰਟ);
        u->ਆਖਰੀ_ਪੋਰਟ = 0;
    }

    len = ਆਖਰੀ - host;

    ਜੇ (len == 0) {
        u->err = "no host";
        ਵਾਪਸ NGX_ERROR;
    }

    u->host.len = len;
    u->host.ਅੰਕੜੇ = host;

    ਜੇ (u->listen && len == 1 && *host == '*') {
        ਜੀਵਾ->sin_addr.s_addr = INADDR_ANY;
        u->wildcard = 1;
        ਵਾਪਸ ngx_inet_ਜੋੜੋ_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);
    }

    ਜੀਵਾ->sin_addr.s_addr = ngx_inet_addr(host, len);

    ਜੇ (ਜੀਵਾ->sin_addr.s_addr != INADDR_NONE) {

        ਜੇ (ਜੀਵਾ->sin_addr.s_addr == INADDR_ANY) {
            u->wildcard = 1;
        }

        ਵਾਪਸ ngx_inet_ਜੋੜੋ_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);
    }

    ਜੇ (u->no_resolve) {
        ਵਾਪਸ NGX_OK;
    }

    ਜੇ (ngx_inet_resolve_host(pool, u) != NGX_OK) {
        ਵਾਪਸ NGX_ERROR;
    }

    u->family = u->addrs[0].sockaddr->sa_family;
    u->socklen = u->addrs[0].socklen;
    ngx_memcpy(&u->sockaddr, u->addrs[0].sockaddr, u->addrs[0].socklen);
    u->wildcard = ngx_inet_wildcard(&u->sockaddr.sockaddr);

    ਵਾਪਸ NGX_OK;
}


ਸਥਿਰ ngx_int_t
ngx_parse_inet6_url(ngx_pool_t *pool, ngx_url_t *u)
{
#ਜੇ (NGX_HAVE_INET6)
    u_char               *p, *host, *ਪੋਰਟ, *ਆਖਰੀ, *uri, *dash;
    ਮਾਪ_t                len;
    ngx_int_t             n;
    ਬਣਤਰ sockaddr_in6  *sin6;

    u->socklen = ਆਕਾਰ(ਬਣਤਰ sockaddr_in6);
    sin6 = (ਬਣਤਰ sockaddr_in6 *) &u->sockaddr;
    sin6->sin6_family = AF_INET6;

    host = u->url.ਅੰਕੜੇ + 1;

    ਆਖਰੀ = u->url.ਅੰਕੜੇ + u->url.len;

    p = ngx_strlchr(host, ਆਖਰੀ, ']');

    ਜੇ (p == ਨਿਰਾ) {
        u->err = "invalid host";
        ਵਾਪਸ NGX_ERROR;
    }

    ਪੋਰਟ = p + 1;

    uri = ngx_strlchr(ਪੋਰਟ, ਆਖਰੀ, '/');

    ਜੇ (uri) {
        ਜੇ (u->listen || !u->uri_part) {
            u->err = "invalid host";
            ਵਾਪਸ NGX_ERROR;
        }

        u->uri.len = ਆਖਰੀ - uri;
        u->uri.ਅੰਕੜੇ = uri;

        ਆਖਰੀ = uri;
    }

    ਜੇ (ਪੋਰਟ < ਆਖਰੀ) {
        ਜੇ (*ਪੋਰਟ != ':') {
            u->err = "invalid host";
            ਵਾਪਸ NGX_ERROR;
        }

        ਪੋਰਟ++;

        len = ਆਖਰੀ - ਪੋਰਟ;

        ਜੇ (u->listen) {
            dash = ngx_strlchr(ਪੋਰਟ, ਆਖਰੀ, '-');

            ਜੇ (dash) {
                dash++;

                n = ngx_atoi(dash, ਆਖਰੀ - dash);

                ਜੇ (n < 1 || n > 65535) {
                    u->err = "invalid port";
                    ਵਾਪਸ NGX_ERROR;
                }

                u->ਆਖਰੀ_ਪੋਰਟ = (ਵਿੱਚ_ਪੋਰਟ_t) n;

                len = dash - ਪੋਰਟ - 1;
            }
        }

        n = ngx_atoi(ਪੋਰਟ, len);

        ਜੇ (n < 1 || n > 65535) {
            u->err = "invalid port";
            ਵਾਪਸ NGX_ERROR;
        }

        ਜੇ (u->ਆਖਰੀ_ਪੋਰਟ && n > u->ਆਖਰੀ_ਪੋਰਟ) {
            u->err = "invalid port range";
            ਵਾਪਸ NGX_ERROR;
        }

        u->ਪੋਰਟ = (ਵਿੱਚ_ਪੋਰਟ_t) n;
        sin6->sin6_ਪੋਰਟ = htons((ਵਿੱਚ_ਪੋਰਟ_t) n);

        u->ਪੋਰਟ_ਪਾਠ.len = ਆਖਰੀ - ਪੋਰਟ;
        u->ਪੋਰਟ_ਪਾਠ.ਅੰਕੜੇ = ਪੋਰਟ;

    } ਨਹੀਂਤਾਂ {
        u->no_ਪੋਰਟ = 1;
        u->ਪੋਰਟ = u->default_ਪੋਰਟ;
        sin6->sin6_ਪੋਰਟ = htons(u->default_ਪੋਰਟ);
    }

    len = p - host;

    ਜੇ (len == 0) {
        u->err = "no host";
        ਵਾਪਸ NGX_ERROR;
    }

    u->host.len = len + 2;
    u->host.ਅੰਕੜੇ = host - 1;

    ਜੇ (ngx_inet6_addr(host, len, sin6->sin6_addr.s6_addr) != NGX_OK) {
        u->err = "invalid IPv6 address";
        ਵਾਪਸ NGX_ERROR;
    }

    ਜੇ (IN6_IS_ADDR_UNSPECIFIED(&sin6->sin6_addr)) {
        u->wildcard = 1;
    }

    u->family = AF_INET6;

    ਵਾਪਸ ngx_inet_ਜੋੜੋ_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);

#ਨਹੀਂਤਾਂ

    u->err = "the INET6 sockets are not supported on this platform";

    ਵਾਪਸ NGX_ERROR;

#ਜੇ_ਅੰਤ
}


#ਜੇ (NGX_HAVE_GETADDRINFO && NGX_HAVE_INET6)

ngx_int_t
ngx_inet_resolve_host(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char           *host;
    ngx_uint_t        n;
    ਬਣਤਰ addrinfo   hints, *res, *rp;

    host = ngx_alloc(u->host.len + 1, pool->ਲਘੂਗਣਕ);
    ਜੇ (host == ਨਿਰਾ) {
        ਵਾਪਸ NGX_ERROR;
    }

    (ਖਾਲੀ) ngx_cpystrn(host, u->host.ਅੰਕੜੇ, u->host.len + 1);

    ngx_memzero(&hints, ਆਕਾਰ(ਬਣਤਰ addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_ਧਾਰਾ;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ AI_ADDRCONFIG
    hints.ai_flags = AI_ADDRCONFIG;
#ਜੇ_ਅੰਤ

    ਜੇ (getaddrinfo((ਅੱਖਰ *) host, ਨਿਰਾ, &hints, &res) != 0) {
        u->err = "host not found";
        ngx_free(host);
        ਵਾਪਸ NGX_ERROR;
    }

    ngx_free(host);

    ਲਈ (n = 0, rp = res; rp != ਨਿਰਾ; rp = rp->ai_ਅਗਲਾ) {

        ਚੋਣ (rp->ai_family) {

        ਹਾਲਤ AF_INET:
        ਹਾਲਤ AF_INET6:
            ਰੋਕੋ;

        ਮੂਲ:
            ਜਾਰੀ;
        }

        n++;
    }

    ਜੇ (n == 0) {
        u->err = "host not found";
        ਜਾਓ failed;
    }

    /* MP: ngx_shared_palloc() */

    ਲਈ (rp = res; rp != ਨਿਰਾ; rp = rp->ai_ਅਗਲਾ) {

        ਚੋਣ (rp->ai_family) {

        ਹਾਲਤ AF_INET:
        ਹਾਲਤ AF_INET6:
            ਰੋਕੋ;

        ਮੂਲ:
            ਜਾਰੀ;
        }

        ਜੇ (ngx_inet_ਜੋੜੋ_addr(pool, u, rp->ai_addr, rp->ai_addrlen, n)
            != NGX_OK)
        {
            ਜਾਓ failed;
        }
    }

    freeaddrinfo(res);
    ਵਾਪਸ NGX_OK;

failed:

    freeaddrinfo(res);
    ਵਾਪਸ NGX_ERROR;
}

#ਨਹੀਂਤਾਂ /* !NGX_HAVE_GETADDRINFO || !NGX_HAVE_INET6 */

ngx_int_t
ngx_inet_resolve_host(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char              *host;
    ngx_uint_t           i, n;
    ਬਣਤਰ hostent      *h;
    ਬਣਤਰ sockaddr_ਵਿੱਚ   ਜੀਵਾ;

    /* AF_INET only */

    ngx_memzero(&ਜੀਵਾ, ਆਕਾਰ(ਬਣਤਰ sockaddr_ਵਿੱਚ));

    ਜੀਵਾ.sin_family = AF_INET;
    ਜੀਵਾ.sin_addr.s_addr = ngx_inet_addr(u->host.ਅੰਕੜੇ, u->host.len);

    ਜੇ (ਜੀਵਾ.sin_addr.s_addr == INADDR_NONE) {
        host = ngx_alloc(u->host.len + 1, pool->ਲਘੂਗਣਕ);
        ਜੇ (host == ਨਿਰਾ) {
            ਵਾਪਸ NGX_ERROR;
        }

        (ਖਾਲੀ) ngx_cpystrn(host, u->host.ਅੰਕੜੇ, u->host.len + 1);

        h = gethostbyname((ਅੱਖਰ *) host);

        ngx_free(host);

        ਜੇ (h == ਨਿਰਾ || h->h_addr_list[0] == ਨਿਰਾ) {
            u->err = "host not found";
            ਵਾਪਸ NGX_ERROR;
        }

        ਲਈ (n = 0; h->h_addr_list[n] != ਨਿਰਾ; n++) { /* void */ }

        /* MP: ngx_shared_palloc() */

        ਲਈ (i = 0; i < n; i++) {
            ਜੀਵਾ.sin_addr.s_addr = *(ਵਿੱਚ_addr_t *) (h->h_addr_list[i]);

            ਜੇ (ngx_inet_ਜੋੜੋ_addr(pool, u, (ਬਣਤਰ sockaddr *) &ਜੀਵਾ,
                                  ਆਕਾਰ(ਬਣਤਰ sockaddr_ਵਿੱਚ), n)
                != NGX_OK)
            {
                ਵਾਪਸ NGX_ERROR;
            }
        }

    } ਨਹੀਂਤਾਂ {

        /* MP: ngx_shared_palloc() */

        ਜੇ (ngx_inet_ਜੋੜੋ_addr(pool, u, (ਬਣਤਰ sockaddr *) &ਜੀਵਾ,
                              ਆਕਾਰ(ਬਣਤਰ sockaddr_ਵਿੱਚ), 1)
            != NGX_OK)
        {
            ਵਾਪਸ NGX_ERROR;
        }
    }

    ਵਾਪਸ NGX_OK;
}

#ਜੇ_ਅੰਤ /* NGX_HAVE_GETADDRINFO && NGX_HAVE_INET6 */


ਸਥਿਰ ngx_int_t
ngx_inet_ਜੋੜੋ_addr(ngx_pool_t *pool, ngx_url_t *u, ਬਣਤਰ sockaddr *sockaddr,
    socklen_t socklen, ngx_uint_t ਕੁੱਲ)
{
    u_char           *p;
    ਮਾਪ_t            len;
    ngx_uint_t        i, nports;
    ngx_addr_t       *addr;
    ਬਣਤਰ sockaddr  *sa;

    nports = u->ਆਖਰੀ_ਪੋਰਟ ? u->ਆਖਰੀ_ਪੋਰਟ - u->ਪੋਰਟ + 1 : 1;

    ਜੇ (u->addrs == ਨਿਰਾ) {
        u->addrs = ngx_palloc(pool, ਕੁੱਲ * nports * ਆਕਾਰ(ngx_addr_t));
        ਜੇ (u->addrs == ਨਿਰਾ) {
            ਵਾਪਸ NGX_ERROR;
        }
    }

    ਲਈ (i = 0; i < nports; i++) {
        sa = ngx_pcalloc(pool, socklen);
        ਜੇ (sa == ਨਿਰਾ) {
            ਵਾਪਸ NGX_ERROR;
        }

        ngx_memcpy(sa, sockaddr, socklen);

        ngx_inet_ਸੈੱਟ_ਪੋਰਟ(sa, u->ਪੋਰਟ + i);

        ਚੋਣ (sa->sa_family) {

#ਜੇ (NGX_HAVE_INET6)
        ਹਾਲਤ AF_INET6:
            len = NGX_INET6_ADDRSTRLEN + ਆਕਾਰ("[]:65536") - 1;
            ਰੋਕੋ;
#ਜੇ_ਅੰਤ

        ਮੂਲ: /* AF_INET */
            len = NGX_INET_ADDRSTRLEN + ਆਕਾਰ(":65535") - 1;
        }

        p = ngx_pnalloc(pool, len);
        ਜੇ (p == ਨਿਰਾ) {
            ਵਾਪਸ NGX_ERROR;
        }

        len = ngx_sock_ntop(sa, socklen, p, len, 1);

        addr = &u->addrs[u->naddrs++];

        addr->sockaddr = sa;
        addr->socklen = socklen;

        addr->name.len = len;
        addr->name.ਅੰਕੜੇ = p;
    }

    ਵਾਪਸ NGX_OK;
}


ngx_int_t
ngx_cmp_sockaddr(ਬਣਤਰ sockaddr *sa1, socklen_t slen1,
    ਬਣਤਰ sockaddr *sa2, socklen_t slen2, ngx_uint_t cmp_ਪੋਰਟ)
{
    ਬਣਤਰ sockaddr_ਵਿੱਚ   *sin1, *sin2;
#ਜੇ (NGX_HAVE_INET6)
    ਬਣਤਰ sockaddr_in6  *sin61, *sin62;
#ਜੇ_ਅੰਤ
#ਜੇ (NGX_HAVE_UNIX_DOMAIN)
    ਮਾਪ_t                len;
    ਬਣਤਰ sockaddr_un   *saun1, *saun2;
#ਜੇ_ਅੰਤ

    ਜੇ (sa1->sa_family != sa2->sa_family) {
        ਵਾਪਸ NGX_DECLINED;
    }

    ਚੋਣ (sa1->sa_family) {

#ਜੇ (NGX_HAVE_INET6)
    ਹਾਲਤ AF_INET6:

        sin61 = (ਬਣਤਰ sockaddr_in6 *) sa1;
        sin62 = (ਬਣਤਰ sockaddr_in6 *) sa2;

        ਜੇ (cmp_ਪੋਰਟ && sin61->sin6_ਪੋਰਟ != sin62->sin6_ਪੋਰਟ) {
            ਵਾਪਸ NGX_DECLINED;
        }

        ਜੇ (ngx_memcmp(&sin61->sin6_addr, &sin62->sin6_addr, 16) != 0) {
            ਵਾਪਸ NGX_DECLINED;
        }

        ਰੋਕੋ;
#ਜੇ_ਅੰਤ

#ਜੇ (NGX_HAVE_UNIX_DOMAIN)
    ਹਾਲਤ AF_UNIX:

        saun1 = (ਬਣਤਰ sockaddr_un *) sa1;
        saun2 = (ਬਣਤਰ sockaddr_un *) sa2;

        ਜੇ (slen1 < slen2) {
            len = slen1 - offsetof(ਬਣਤਰ sockaddr_un, sun_ਰਸਤਾ);

        } ਨਹੀਂਤਾਂ {
            len = slen2 - offsetof(ਬਣਤਰ sockaddr_un, sun_ਰਸਤਾ);
        }

        ਜੇ (len > ਆਕਾਰ(saun1->sun_ਰਸਤਾ)) {
            len = ਆਕਾਰ(saun1->sun_ਰਸਤਾ);
        }

        ਜੇ (ngx_memcmp(&saun1->sun_ਰਸਤਾ, &saun2->sun_ਰਸਤਾ, len) != 0) {
            ਵਾਪਸ NGX_DECLINED;
        }

        ਰੋਕੋ;
#ਜੇ_ਅੰਤ

    ਮੂਲ: /* AF_INET */

        sin1 = (ਬਣਤਰ sockaddr_ਵਿੱਚ *) sa1;
        sin2 = (ਬਣਤਰ sockaddr_ਵਿੱਚ *) sa2;

        ਜੇ (cmp_ਪੋਰਟ && sin1->sin_ਪੋਰਟ != sin2->sin_ਪੋਰਟ) {
            ਵਾਪਸ NGX_DECLINED;
        }

        ਜੇ (sin1->sin_addr.s_addr != sin2->sin_addr.s_addr) {
            ਵਾਪਸ NGX_DECLINED;
        }

        ਰੋਕੋ;
    }

    ਵਾਪਸ NGX_OK;
}


ਵਿੱਚ_ਪੋਰਟ_t
ngx_inet_get_ਪੋਰਟ(ਬਣਤਰ sockaddr *sa)
{
    ਬਣਤਰ sockaddr_ਵਿੱਚ   *ਜੀਵਾ;
#ਜੇ (NGX_HAVE_INET6)
    ਬਣਤਰ sockaddr_in6  *sin6;
#ਜੇ_ਅੰਤ

    ਚੋਣ (sa->sa_family) {

#ਜੇ (NGX_HAVE_INET6)
    ਹਾਲਤ AF_INET6:
        sin6 = (ਬਣਤਰ sockaddr_in6 *) sa;
        ਵਾਪਸ ntohs(sin6->sin6_ਪੋਰਟ);
#ਜੇ_ਅੰਤ

#ਜੇ (NGX_HAVE_UNIX_DOMAIN)
    ਹਾਲਤ AF_UNIX:
        ਵਾਪਸ 0;
#ਜੇ_ਅੰਤ

    ਮੂਲ: /* AF_INET */
        ਜੀਵਾ = (ਬਣਤਰ sockaddr_ਵਿੱਚ *) sa;
        ਵਾਪਸ ntohs(ਜੀਵਾ->sin_ਪੋਰਟ);
    }
}


ਖਾਲੀ
ngx_inet_ਸੈੱਟ_ਪੋਰਟ(ਬਣਤਰ sockaddr *sa, ਵਿੱਚ_ਪੋਰਟ_t ਪੋਰਟ)
{
    ਬਣਤਰ sockaddr_ਵਿੱਚ   *ਜੀਵਾ;
#ਜੇ (NGX_HAVE_INET6)
    ਬਣਤਰ sockaddr_in6  *sin6;
#ਜੇ_ਅੰਤ

    ਚੋਣ (sa->sa_family) {

#ਜੇ (NGX_HAVE_INET6)
    ਹਾਲਤ AF_INET6:
        sin6 = (ਬਣਤਰ sockaddr_in6 *) sa;
        sin6->sin6_ਪੋਰਟ = htons(ਪੋਰਟ);
        ਰੋਕੋ;
#ਜੇ_ਅੰਤ

#ਜੇ (NGX_HAVE_UNIX_DOMAIN)
    ਹਾਲਤ AF_UNIX:
        ਰੋਕੋ;
#ਜੇ_ਅੰਤ

    ਮੂਲ: /* AF_INET */
        ਜੀਵਾ = (ਬਣਤਰ sockaddr_ਵਿੱਚ *) sa;
        ਜੀਵਾ->sin_ਪੋਰਟ = htons(ਪੋਰਟ);
        ਰੋਕੋ;
    }
}


ngx_uint_t
ngx_inet_wildcard(ਬਣਤਰ sockaddr *sa)
{
    ਬਣਤਰ sockaddr_ਵਿੱਚ   *ਜੀਵਾ;
#ਜੇ (NGX_HAVE_INET6)
    ਬਣਤਰ sockaddr_in6  *sin6;
#ਜੇ_ਅੰਤ

    ਚੋਣ (sa->sa_family) {

    ਹਾਲਤ AF_INET:
        ਜੀਵਾ = (ਬਣਤਰ sockaddr_ਵਿੱਚ *) sa;

        ਜੇ (ਜੀਵਾ->sin_addr.s_addr == INADDR_ANY) {
            ਵਾਪਸ 1;
        }

        ਰੋਕੋ;

#ਜੇ (NGX_HAVE_INET6)

    ਹਾਲਤ AF_INET6:
        sin6 = (ਬਣਤਰ sockaddr_in6 *) sa;

        ਜੇ (IN6_IS_ADDR_UNSPECIFIED(&sin6->sin6_addr)) {
            ਵਾਪਸ 1;
        }

        ਰੋਕੋ;

#ਜੇ_ਅੰਤ
    }

    ਵਾਪਸ 0;
}
