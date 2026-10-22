
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


ಸ್ಥಿರ ngx_int_t ngx_parse_unix_domain_url(ngx_pool_t *pool, ngx_url_t *u);
ಸ್ಥಿರ ngx_int_t ngx_parse_inet_url(ngx_pool_t *pool, ngx_url_t *u);
ಸ್ಥಿರ ngx_int_t ngx_parse_inet6_url(ngx_pool_t *pool, ngx_url_t *u);
ಸ್ಥಿರ ngx_int_t ngx_inet_ಕೂಡಿಸು_addr(ngx_pool_t *pool, ngx_url_t *u,
    ರಚನೆ sockaddr *sockaddr, socklen_t socklen, ngx_uint_t ಒಟ್ಟು);


ಒಳ_addr_t
ngx_inet_addr(u_char *ಪಠ್ಯ, ಗಾತ್ರ_t len)
{
    u_char      *p, c;
    ಒಳ_addr_t    addr;
    ngx_uint_t   octet, n;

    addr = 0;
    octet = 0;
    n = 0;

    ಗಾಗಿ (p = ಪಠ್ಯ; p < ಪಠ್ಯ + len; p++) {
        c = *p;

        ಆದರೆ (c >= '0' && c <= '9') {
            octet = octet * 10 + (c - '0');

            ಆದರೆ (octet > 255) {
                ಹಿಂತಿರುಗಿ INADDR_NONE;
            }

            ಮುಂದುವರಿ;
        }

        ಆದರೆ (c == '.') {
            addr = (addr << 8) + octet;
            octet = 0;
            n++;
            ಮುಂದುವರಿ;
        }

        ಹಿಂತಿರುಗಿ INADDR_NONE;
    }

    ಆದರೆ (n == 3) {
        addr = (addr << 8) + octet;
        ಹಿಂತಿರುಗಿ htonl(addr);
    }

    ಹಿಂತಿರುಗಿ INADDR_NONE;
}


#ಆದರೆ (NGX_HAVE_INET6)

ngx_int_t
ngx_inet6_addr(u_char *p, ಗಾತ್ರ_t len, u_char *addr)
{
    u_char      c, *zero, *digit, *s, *d;
    ಗಾತ್ರ_t      len4;
    ngx_uint_t  n, nibbles, ಶಬ್ದ;

    ಆದರೆ (len == 0) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    zero = ಖಾಲಿ;
    digit = ಖಾಲಿ;
    len4 = 0;
    nibbles = 0;
    ಶಬ್ದ = 0;
    n = 8;

    ಆದರೆ (p[0] == ':') {
        p++;
        len--;
    }

    ಗಾಗಿ (/* void */; len; len--) {
        c = *p++;

        ಆದರೆ (c == ':') {
            ಆದರೆ (nibbles) {
                digit = p;
                len4 = len;
                *addr++ = (u_char) (ಶಬ್ದ >> 8);
                *addr++ = (u_char) (ಶಬ್ದ & 0xff);

                ಆದರೆ (--n) {
                    nibbles = 0;
                    ಶಬ್ದ = 0;
                    ಮುಂದುವರಿ;
                }

            } ಇಲ್ಲದಿದ್ದರೆ {
                ಆದರೆ (zero == ಖಾಲಿ) {
                    digit = p;
                    len4 = len;
                    zero = addr;
                    ಮುಂದುವರಿ;
                }
            }

            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        ಆದರೆ (c == '.' && nibbles) {
            ಆದರೆ (n < 2 || digit == ಖಾಲಿ) {
                ಹಿಂತಿರುಗಿ NGX_ERROR;
            }

            ಶಬ್ದ = ngx_inet_addr(digit, len4 - 1);
            ಆದರೆ (ಶಬ್ದ == INADDR_NONE) {
                ಹಿಂತಿರುಗಿ NGX_ERROR;
            }

            ಶಬ್ದ = ntohl(ಶಬ್ದ);
            *addr++ = (u_char) ((ಶಬ್ದ >> 24) & 0xff);
            *addr++ = (u_char) ((ಶಬ್ದ >> 16) & 0xff);
            n--;
            ನಿಲ್ಲಿಸು;
        }

        ಆದರೆ (++nibbles > 4) {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        ಆದರೆ (c >= '0' && c <= '9') {
            ಶಬ್ದ = ಶಬ್ದ * 16 + (c - '0');
            ಮುಂದುವರಿ;
        }

        c |= 0x20;

        ಆದರೆ (c >= 'a' && c <= 'f') {
            ಶಬ್ದ = ಶಬ್ದ * 16 + (c - 'a') + 10;
            ಮುಂದುವರಿ;
        }

        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    ಆದರೆ (nibbles == 0 && zero == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    *addr++ = (u_char) (ಶಬ್ದ >> 8);
    *addr++ = (u_char) (ಶಬ್ದ & 0xff);

    ಆದರೆ (--n) {
        ಆದರೆ (zero) {
            n *= 2;
            s = addr - 1;
            d = s + n;
            ಆಗ (s >= zero) {
                *d-- = *s--;
            }
            ngx_memzero(zero, n);
            ಹಿಂತಿರುಗಿ NGX_OK;
        }

    } ಇಲ್ಲದಿದ್ದರೆ {
        ಆದರೆ (zero == ಖಾಲಿ) {
            ಹಿಂತಿರುಗಿ NGX_OK;
        }
    }

    ಹಿಂತಿರುಗಿ NGX_ERROR;
}

#ಆದರೆ_ಅಂತ್ಯ


ಗಾತ್ರ_t
ngx_sock_ntop(ರಚನೆ sockaddr *sa, socklen_t socklen, u_char *ಪಠ್ಯ, ಗಾತ್ರ_t len,
    ngx_uint_t ಪೋರ್ಟ್)
{
    u_char               *p;
#ಆದರೆ (NGX_HAVE_INET6 || NGX_HAVE_UNIX_DOMAIN)
    ಗಾತ್ರ_t                n;
#ಆದರೆ_ಅಂತ್ಯ
    ರಚನೆ sockaddr_ಒಳ   *ಜ್ಯಾ;
#ಆದರೆ (NGX_HAVE_INET6)
    ರಚನೆ sockaddr_in6  *sin6;
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ (NGX_HAVE_UNIX_DOMAIN)
    ರಚನೆ sockaddr_un   *saun;
#ಆದರೆ_ಅಂತ್ಯ

    ಆಯ್ಕೆ (sa->sa_family) {

    ಸ್ಥಿತಿ AF_INET:

        ಜ್ಯಾ = (ರಚನೆ sockaddr_ಒಳ *) sa;
        p = (u_char *) &ಜ್ಯಾ->sin_addr;

        ಆದರೆ (ಪೋರ್ಟ್) {
            p = ngx_snprintf(ಪಠ್ಯ, len, "%ud.%ud.%ud.%ud:%d",
                             p[0], p[1], p[2], p[3], ntohs(ಜ್ಯಾ->sin_ಪೋರ್ಟ್));
        } ಇಲ್ಲದಿದ್ದರೆ {
            p = ngx_snprintf(ಪಠ್ಯ, len, "%ud.%ud.%ud.%ud",
                             p[0], p[1], p[2], p[3]);
        }

        ಹಿಂತಿರುಗಿ (p - ಪಠ್ಯ);

#ಆದರೆ (NGX_HAVE_INET6)

    ಸ್ಥಿತಿ AF_INET6:

        sin6 = (ರಚನೆ sockaddr_in6 *) sa;

        n = 0;

        ಆದರೆ (ಪೋರ್ಟ್) {
            ಪಠ್ಯ[n++] = '[';
        }

        n = ngx_inet6_ntop(sin6->sin6_addr.s6_addr, &ಪಠ್ಯ[n], len);

        ಆದರೆ (ಪೋರ್ಟ್) {
            n = ngx_sprintf(&ಪಠ್ಯ[1 + n], "]:%d",
                            ntohs(sin6->sin6_ಪೋರ್ಟ್)) - ಪಠ್ಯ;
        }

        ಹಿಂತಿರುಗಿ n;
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ (NGX_HAVE_UNIX_DOMAIN)

    ಸ್ಥಿತಿ AF_UNIX:
        saun = (ರಚನೆ sockaddr_un *) sa;

        /* on Linux sockaddr might not include sun_path at all */

        ಆದರೆ (socklen <= (socklen_t) offsetof(ರಚನೆ sockaddr_un, sun_ಮಾರ್ಗ)) {
            p = ngx_snprintf(ಪಠ್ಯ, len, "unix:%Z");

        } ಇಲ್ಲದಿದ್ದರೆ {
            n = ngx_strnlen((u_char *) saun->sun_ಮಾರ್ಗ,
                            socklen - offsetof(ರಚನೆ sockaddr_un, sun_ಮಾರ್ಗ));
            p = ngx_snprintf(ಪಠ್ಯ, len, "unix:%*s%Z", n, saun->sun_ಮಾರ್ಗ);
        }

        /* we do not include trailing zero in address length */

        ಹಿಂತಿರುಗಿ (p - ಪಠ್ಯ - 1);

#ಆದರೆ_ಅಂತ್ಯ

    ಪೂರ್ವನಿಯೋಜಿತ:
        ಹಿಂತಿರುಗಿ 0;
    }
}


ಗಾತ್ರ_t
ngx_inet_ntop(ಪೂರ್ಣಾಂಕ family, ಶೂನ್ಯ *addr, u_char *ಪಠ್ಯ, ಗಾತ್ರ_t len)
{
    u_char  *p;

    ಆಯ್ಕೆ (family) {

    ಸ್ಥಿತಿ AF_INET:

        p = addr;

        ಹಿಂತಿರುಗಿ ngx_snprintf(ಪಠ್ಯ, len, "%ud.%ud.%ud.%ud",
                            p[0], p[1], p[2], p[3])
               - ಪಠ್ಯ;

#ಆದರೆ (NGX_HAVE_INET6)

    ಸ್ಥಿತಿ AF_INET6:
        ಹಿಂತಿರುಗಿ ngx_inet6_ntop(addr, ಪಠ್ಯ, len);

#ಆದರೆ_ಅಂತ್ಯ

    ಪೂರ್ವನಿಯೋಜಿತ:
        ಹಿಂತಿರುಗಿ 0;
    }
}


#ಆದರೆ (NGX_HAVE_INET6)

ಗಾತ್ರ_t
ngx_inet6_ntop(u_char *p, u_char *ಪಠ್ಯ, ಗಾತ್ರ_t len)
{
    u_char      *dst;
    ಗಾತ್ರ_t       max, n;
    ngx_uint_t   i, zero, ಕೊನೆಯ;

    ಆದರೆ (len < NGX_INET6_ADDRSTRLEN) {
        ಹಿಂತಿರುಗಿ 0;
    }

    zero = (ngx_uint_t) -1;
    ಕೊನೆಯ = (ngx_uint_t) -1;
    max = 1;
    n = 0;

    ಗಾಗಿ (i = 0; i < 16; i += 2) {

        ಆದರೆ (p[i] || p[i + 1]) {

            ಆದರೆ (max < n) {
                zero = ಕೊನೆಯ;
                max = n;
            }

            n = 0;
            ಮುಂದುವರಿ;
        }

        ಆದರೆ (n++ == 0) {
            ಕೊನೆಯ = i;
        }
    }

    ಆದರೆ (max < n) {
        zero = ಕೊನೆಯ;
        max = n;
    }

    dst = ಪಠ್ಯ;
    n = 16;

    ಆದರೆ (zero == 0) {

        ಆದರೆ ((max == 5 && p[10] == 0xff && p[11] == 0xff)
            || (max == 6)
            || (max == 7 && p[14] != 0 && p[15] != 1))
        {
            n = 12;
        }

        *dst++ = ':';
    }

    ಗಾಗಿ (i = 0; i < n; i += 2) {

        ಆದರೆ (i == zero) {
            *dst++ = ':';
            i += (max - 1) * 2;
            ಮುಂದುವರಿ;
        }

        dst = ngx_sprintf(dst, "%xd", p[i] * 256 + p[i + 1]);

        ಆದರೆ (i < 14) {
            *dst++ = ':';
        }
    }

    ಆದರೆ (n == 12) {
        dst = ngx_sprintf(dst, "%ud.%ud.%ud.%ud", p[12], p[13], p[14], p[15]);
    }

    ಹಿಂತಿರುಗಿ dst - ಪಠ್ಯ;
}

#ಆದರೆ_ಅಂತ್ಯ


ngx_int_t
ngx_ptocidr(ngx_str_t *ಪಠ್ಯ, ngx_cidr_t *cidr)
{
    u_char      *addr, *ಮುಖವಾಡ, *ಕೊನೆಯ;
    ಗಾತ್ರ_t       len;
    ngx_int_t    ಸರಿಸು;
#ಆದರೆ (NGX_HAVE_INET6)
    ngx_int_t    rc;
    ngx_uint_t   s, i;
#ಆದರೆ_ಅಂತ್ಯ

    addr = ಪಠ್ಯ->ದತ್ತಾಂಶ;
    ಕೊನೆಯ = addr + ಪಠ್ಯ->len;

    ಮುಖವಾಡ = ngx_strlchr(addr, ಕೊನೆಯ, '/');
    len = (ಮುಖವಾಡ ? ಮುಖವಾಡ : ಕೊನೆಯ) - addr;

    cidr->u.ಒಳ.addr = ngx_inet_addr(addr, len);

    ಆದರೆ (cidr->u.ಒಳ.addr != INADDR_NONE) {
        cidr->family = AF_INET;

        ಆದರೆ (ಮುಖವಾಡ == ಖಾಲಿ) {
            cidr->u.ಒಳ.ಮುಖವಾಡ = 0xffffffff;
            ಹಿಂತಿರುಗಿ NGX_OK;
        }

#ಆದರೆ (NGX_HAVE_INET6)
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ngx_inet6_addr(addr, len, cidr->u.in6.addr.s6_addr) == NGX_OK) {
        cidr->family = AF_INET6;

        ಆದರೆ (ಮುಖವಾಡ == ಖಾಲಿ) {
            ngx_memset(cidr->u.in6.ಮುಖವಾಡ.s6_addr, 0xff, 16);
            ಹಿಂತಿರುಗಿ NGX_OK;
        }

#ಆದರೆ_ಅಂತ್ಯ
    } ಇಲ್ಲದಿದ್ದರೆ {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    ಮುಖವಾಡ++;

    ಸರಿಸು = ngx_atoi(ಮುಖವಾಡ, ಕೊನೆಯ - ಮುಖವಾಡ);
    ಆದರೆ (ಸರಿಸು == NGX_ERROR) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    ಆಯ್ಕೆ (cidr->family) {

#ಆದರೆ (NGX_HAVE_INET6)
    ಸ್ಥಿತಿ AF_INET6:
        ಆದರೆ (ಸರಿಸು > 128) {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        addr = cidr->u.in6.addr.s6_addr;
        ಮುಖವಾಡ = cidr->u.in6.ಮುಖವಾಡ.s6_addr;
        rc = NGX_OK;

        ಗಾಗಿ (i = 0; i < 16; i++) {

            s = (ಸರಿಸು > 8) ? 8 : ಸರಿಸು;
            ಸರಿಸು -= s;

            ಮುಖವಾಡ[i] = (u_char) (0xffu << (8 - s));

            ಆದರೆ (addr[i] != (addr[i] & ಮುಖವಾಡ[i])) {
                rc = NGX_ಮುಗಿದ;
                addr[i] &= ಮುಖವಾಡ[i];
            }
        }

        ಹಿಂತಿರುಗಿ rc;
#ಆದರೆ_ಅಂತ್ಯ

    ಪೂರ್ವನಿಯೋಜಿತ: /* AF_INET */
        ಆದರೆ (ಸರಿಸು > 32) {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        ಆದರೆ (ಸರಿಸು) {
            cidr->u.ಒಳ.ಮುಖವಾಡ = htonl((uint32_t) (0xffffffffu << (32 - ಸರಿಸು)));

        } ಇಲ್ಲದಿದ್ದರೆ {
            /* x86 compilers use a shl instruction that shifts by modulo 32 */
            cidr->u.ಒಳ.ಮುಖವಾಡ = 0;
        }

        ಆದರೆ (cidr->u.ಒಳ.addr == (cidr->u.ಒಳ.addr & cidr->u.ಒಳ.ಮುಖವಾಡ)) {
            ಹಿಂತಿರುಗಿ NGX_OK;
        }

        cidr->u.ಒಳ.addr &= cidr->u.ಒಳ.ಮುಖವಾಡ;

        ಹಿಂತಿರುಗಿ NGX_ಮುಗಿದ;
    }
}


ngx_int_t
ngx_cidr_ಹೊಂದಿಕೆ(ರಚನೆ sockaddr *sa, ngx_array_t *cidrs)
{
#ಆದರೆ (NGX_HAVE_INET6)
    u_char           *p;
#ಆದರೆ_ಅಂತ್ಯ
    ಒಳ_addr_t         inaddr;
    ngx_cidr_t       *cidr;
    ngx_uint_t        family, i;
#ಆದರೆ (NGX_HAVE_INET6)
    ngx_uint_t        n;
    ರಚನೆ in6_addr  *inaddr6;
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ (NGX_SUPPRESS_WARN)
    inaddr = 0;
#ಆದರೆ (NGX_HAVE_INET6)
    inaddr6 = ಖಾಲಿ;
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

    family = sa->sa_family;

    ಆದರೆ (family == AF_INET) {
        inaddr = ((ರಚನೆ sockaddr_ಒಳ *) sa)->sin_addr.s_addr;
    }

#ಆದರೆ (NGX_HAVE_INET6)
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (family == AF_INET6) {
        inaddr6 = &((ರಚನೆ sockaddr_in6 *) sa)->sin6_addr;

        ಆದರೆ (IN6_IS_ADDR_V4MAPPED(inaddr6)) {
            family = AF_INET;

            p = inaddr6->s6_addr;

            inaddr = (ಒಳ_addr_t) p[12] << 24;
            inaddr += p[13] << 16;
            inaddr += p[14] << 8;
            inaddr += p[15];

            inaddr = htonl(inaddr);
        }
    }
#ಆದರೆ_ಅಂತ್ಯ

    ಗಾಗಿ (cidr = cidrs->elts, i = 0; i < cidrs->nelts; i++) {
        ಆದರೆ (cidr[i].family != family) {
            ಹೋಗು ಮುಂದಿನ;
        }

        ಆಯ್ಕೆ (family) {

#ಆದರೆ (NGX_HAVE_INET6)
        ಸ್ಥಿತಿ AF_INET6:
            ಗಾಗಿ (n = 0; n < 16; n++) {
                ಆದರೆ ((inaddr6->s6_addr[n] & cidr[i].u.in6.ಮುಖವಾಡ.s6_addr[n])
                    != cidr[i].u.in6.addr.s6_addr[n])
                {
                    ಹೋಗು ಮುಂದಿನ;
                }
            }
            ನಿಲ್ಲಿಸು;
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ (NGX_HAVE_UNIX_DOMAIN)
        ಸ್ಥಿತಿ AF_UNIX:
            ನಿಲ್ಲಿಸು;
#ಆದರೆ_ಅಂತ್ಯ

        ಪೂರ್ವನಿಯೋಜಿತ: /* AF_INET */
            ಆದರೆ ((inaddr & cidr[i].u.ಒಳ.ಮುಖವಾಡ) != cidr[i].u.ಒಳ.addr) {
                ಹೋಗು ಮುಂದಿನ;
            }
            ನಿಲ್ಲಿಸು;
        }

        ಹಿಂತಿರುಗಿ NGX_OK;

    ಮುಂದಿನ:
        ಮುಂದುವರಿ;
    }

    ಹಿಂತಿರುಗಿ NGX_DECLINED;
}


ngx_int_t
ngx_parse_addr(ngx_pool_t *pool, ngx_addr_t *addr, u_char *ಪಠ್ಯ, ಗಾತ್ರ_t len)
{
    ಒಳ_addr_t             inaddr;
    ngx_uint_t            family;
    ರಚನೆ sockaddr_ಒಳ   *ಜ್ಯಾ;
#ಆದರೆ (NGX_HAVE_INET6)
    ರಚನೆ in6_addr       inaddr6;
    ರಚನೆ sockaddr_in6  *sin6;

    /*
     * prevent MSVC8 warning:
     *    potentially uninitialized local variable 'inaddr6' used
     */
    ngx_memzero(&inaddr6, ಗಾತ್ರ(ರಚನೆ in6_addr));
#ಆದರೆ_ಅಂತ್ಯ

    inaddr = ngx_inet_addr(ಪಠ್ಯ, len);

    ಆದರೆ (inaddr != INADDR_NONE) {
        family = AF_INET;
        len = ಗಾತ್ರ(ರಚನೆ sockaddr_ಒಳ);

#ಆದರೆ (NGX_HAVE_INET6)
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ngx_inet6_addr(ಪಠ್ಯ, len, inaddr6.s6_addr) == NGX_OK) {
        family = AF_INET6;
        len = ಗಾತ್ರ(ರಚನೆ sockaddr_in6);

#ಆದರೆ_ಅಂತ್ಯ
    } ಇಲ್ಲದಿದ್ದರೆ {
        ಹಿಂತಿರುಗಿ NGX_DECLINED;
    }

    addr->sockaddr = ngx_pcalloc(pool, len);
    ಆದರೆ (addr->sockaddr == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    addr->sockaddr->sa_family = (u_char) family;
    addr->socklen = len;

    ಆಯ್ಕೆ (family) {

#ಆದರೆ (NGX_HAVE_INET6)
    ಸ್ಥಿತಿ AF_INET6:
        sin6 = (ರಚನೆ sockaddr_in6 *) addr->sockaddr;
        ngx_memcpy(sin6->sin6_addr.s6_addr, inaddr6.s6_addr, 16);
        ನಿಲ್ಲಿಸು;
#ಆದರೆ_ಅಂತ್ಯ

    ಪೂರ್ವನಿಯೋಜಿತ: /* AF_INET */
        ಜ್ಯಾ = (ರಚನೆ sockaddr_ಒಳ *) addr->sockaddr;
        ಜ್ಯಾ->sin_addr.s_addr = inaddr;
        ನಿಲ್ಲಿಸು;
    }

    ಹಿಂತಿರುಗಿ NGX_OK;
}


ngx_int_t
ngx_parse_addr_ಪೋರ್ಟ್(ngx_pool_t *pool, ngx_addr_t *addr, u_char *ಪಠ್ಯ,
    ಗಾತ್ರ_t len)
{
    u_char     *p, *ಕೊನೆಯ;
    ಗಾತ್ರ_t      plen;
    ngx_int_t   rc, ಪೋರ್ಟ್;

    rc = ngx_parse_addr(pool, addr, ಪಠ್ಯ, len);

    ಆದರೆ (rc != NGX_DECLINED) {
        ಹಿಂತಿರುಗಿ rc;
    }

    ಕೊನೆಯ = ಪಠ್ಯ + len;

#ಆದರೆ (NGX_HAVE_INET6)
    ಆದರೆ (len && ಪಠ್ಯ[0] == '[') {

        p = ngx_strlchr(ಪಠ್ಯ, ಕೊನೆಯ, ']');

        ಆದರೆ (p == ಕೊನೆಯ - 1) {
            ಹಿಂತಿರುಗಿ ngx_parse_addr(pool, addr, ಪಠ್ಯ + 1, len - 2);
        }

        ಆದರೆ (p == ಖಾಲಿ || *++p != ':') {
            ಹಿಂತಿರುಗಿ NGX_DECLINED;
        }

        ಪಠ್ಯ++;
        len -= 2;

    } ಇಲ್ಲದಿದ್ದರೆ
#ಆದರೆ_ಅಂತ್ಯ

    {
        p = ngx_strlchr(ಪಠ್ಯ, ಕೊನೆಯ, ':');

        ಆದರೆ (p == ಖಾಲಿ) {
            ಹಿಂತಿರುಗಿ NGX_DECLINED;
        }
    }

    p++;
    plen = ಕೊನೆಯ - p;

    ಪೋರ್ಟ್ = ngx_atoi(p, plen);

    ಆದರೆ (ಪೋರ್ಟ್ < 1 || ಪೋರ್ಟ್ > 65535) {
        ಹಿಂತಿರುಗಿ NGX_DECLINED;
    }

    len -= plen + 1;

    rc = ngx_parse_addr(pool, addr, ಪಠ್ಯ, len);

    ಆದರೆ (rc != NGX_OK) {
        ಹಿಂತಿರುಗಿ rc;
    }

    ngx_inet_ಗಣ_ಪೋರ್ಟ್(addr->sockaddr, (ಒಳ_ಪೋರ್ಟ್_t) ಪೋರ್ಟ್);

    ಹಿಂತಿರುಗಿ NGX_OK;
}


ngx_int_t
ngx_parse_url(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char  *p;
    ಗಾತ್ರ_t   len;

    p = u->url.ದತ್ತಾಂಶ;
    len = u->url.len;

    ಆದರೆ (len >= 5 && ngx_strncasecmp(p, (u_char *) "unix:", 5) == 0) {
        ಹಿಂತಿರುಗಿ ngx_parse_unix_domain_url(pool, u);
    }

    ಆದರೆ (len && p[0] == '[') {
        ಹಿಂತಿರುಗಿ ngx_parse_inet6_url(pool, u);
    }

    ಹಿಂತಿರುಗಿ ngx_parse_inet_url(pool, u);
}


ಸ್ಥಿರ ngx_int_t
ngx_parse_unix_domain_url(ngx_pool_t *pool, ngx_url_t *u)
{
#ಆದರೆ (NGX_HAVE_UNIX_DOMAIN)
    u_char              *ಮಾರ್ಗ, *uri, *ಕೊನೆಯ;
    ಗಾತ್ರ_t               len;
    ರಚನೆ sockaddr_un  *saun;

    len = u->url.len;
    ಮಾರ್ಗ = u->url.ದತ್ತಾಂಶ;

    ಮಾರ್ಗ += 5;
    len -= 5;

    ಆದರೆ (u->uri_part) {

        ಕೊನೆಯ = ಮಾರ್ಗ + len;
        uri = ngx_strlchr(ಮಾರ್ಗ, ಕೊನೆಯ, ':');

        ಆದರೆ (uri) {
            len = uri - ಮಾರ್ಗ;
            uri++;
            u->uri.len = ಕೊನೆಯ - uri;
            u->uri.ದತ್ತಾಂಶ = uri;
        }
    }

    ಆದರೆ (len == 0) {
        u->err = "no path in the unix domain socket";
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    u->host.len = len++;
    u->host.ದತ್ತಾಂಶ = ಮಾರ್ಗ;

    ಆದರೆ (len > ಗಾತ್ರ(saun->sun_ಮಾರ್ಗ)) {
        u->err = "too long path in the unix domain socket";
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    u->socklen = ಗಾತ್ರ(ರಚನೆ sockaddr_un);
    saun = (ರಚನೆ sockaddr_un *) &u->sockaddr;
    saun->sun_family = AF_UNIX;
    (ಶೂನ್ಯ) ngx_cpystrn((u_char *) saun->sun_ಮಾರ್ಗ, ಮಾರ್ಗ, len);

    u->addrs = ngx_pcalloc(pool, ಗಾತ್ರ(ngx_addr_t));
    ಆದರೆ (u->addrs == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    saun = ngx_pcalloc(pool, ಗಾತ್ರ(ರಚನೆ sockaddr_un));
    ಆದರೆ (saun == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    u->family = AF_UNIX;
    u->naddrs = 1;

    saun->sun_family = AF_UNIX;
    (ಶೂನ್ಯ) ngx_cpystrn((u_char *) saun->sun_ಮಾರ್ಗ, ಮಾರ್ಗ, len);

    u->addrs[0].sockaddr = (ರಚನೆ sockaddr *) saun;
    u->addrs[0].socklen = ಗಾತ್ರ(ರಚನೆ sockaddr_un);
    u->addrs[0].name.len = len + 4;
    u->addrs[0].name.ದತ್ತಾಂಶ = u->url.ದತ್ತಾಂಶ;

    ಹಿಂತಿರುಗಿ NGX_OK;

#ಇಲ್ಲದಿದ್ದರೆ

    u->err = "the unix domain sockets are not supported on this platform";

    ಹಿಂತಿರುಗಿ NGX_ERROR;

#ಆದರೆ_ಅಂತ್ಯ
}


ಸ್ಥಿರ ngx_int_t
ngx_parse_inet_url(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char              *host, *ಪೋರ್ಟ್, *ಕೊನೆಯ, *uri, *ವಾದಗಳು, *dash;
    ಗಾತ್ರ_t               len;
    ngx_int_t            n;
    ರಚನೆ sockaddr_ಒಳ  *ಜ್ಯಾ;

    u->socklen = ಗಾತ್ರ(ರಚನೆ sockaddr_ಒಳ);
    ಜ್ಯಾ = (ರಚನೆ sockaddr_ಒಳ *) &u->sockaddr;
    ಜ್ಯಾ->sin_family = AF_INET;

    u->family = AF_INET;

    host = u->url.ದತ್ತಾಂಶ;

    ಕೊನೆಯ = host + u->url.len;

    ಪೋರ್ಟ್ = ngx_strlchr(host, ಕೊನೆಯ, ':');

    uri = ngx_strlchr(host, ಕೊನೆಯ, '/');

    ವಾದಗಳು = ngx_strlchr(host, ಕೊನೆಯ, '?');

    ಆದರೆ (ವಾದಗಳು) {
        ಆದರೆ (uri == ಖಾಲಿ || ವಾದಗಳು < uri) {
            uri = ವಾದಗಳು;
        }
    }

    ಆದರೆ (uri) {
        ಆದರೆ (u->listen || !u->uri_part) {
            u->err = "invalid host";
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        u->uri.len = ಕೊನೆಯ - uri;
        u->uri.ದತ್ತಾಂಶ = uri;

        ಕೊನೆಯ = uri;

        ಆದರೆ (uri < ಪೋರ್ಟ್) {
            ಪೋರ್ಟ್ = ಖಾಲಿ;
        }
    }

    ಆದರೆ (ಪೋರ್ಟ್) {
        ಪೋರ್ಟ್++;

        len = ಕೊನೆಯ - ಪೋರ್ಟ್;

        ಆದರೆ (u->listen) {
            dash = ngx_strlchr(ಪೋರ್ಟ್, ಕೊನೆಯ, '-');

            ಆದರೆ (dash) {
                dash++;

                n = ngx_atoi(dash, ಕೊನೆಯ - dash);

                ಆದರೆ (n < 1 || n > 65535) {
                    u->err = "invalid port";
                    ಹಿಂತಿರುಗಿ NGX_ERROR;
                }

                u->ಕೊನೆಯ_ಪೋರ್ಟ್ = (ಒಳ_ಪೋರ್ಟ್_t) n;

                len = dash - ಪೋರ್ಟ್ - 1;
            }
        }

        n = ngx_atoi(ಪೋರ್ಟ್, len);

        ಆದರೆ (n < 1 || n > 65535) {
            u->err = "invalid port";
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        ಆದರೆ (u->ಕೊನೆಯ_ಪೋರ್ಟ್ && n > u->ಕೊನೆಯ_ಪೋರ್ಟ್) {
            u->err = "invalid port range";
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        u->ಪೋರ್ಟ್ = (ಒಳ_ಪೋರ್ಟ್_t) n;
        ಜ್ಯಾ->sin_ಪೋರ್ಟ್ = htons((ಒಳ_ಪೋರ್ಟ್_t) n);

        u->ಪೋರ್ಟ್_ಪಠ್ಯ.len = ಕೊನೆಯ - ಪೋರ್ಟ್;
        u->ಪೋರ್ಟ್_ಪಠ್ಯ.ದತ್ತಾಂಶ = ಪೋರ್ಟ್;

        ಕೊನೆಯ = ಪೋರ್ಟ್ - 1;

    } ಇಲ್ಲದಿದ್ದರೆ {
        ಆದರೆ (uri == ಖಾಲಿ) {

            ಆದರೆ (u->listen) {

                /* test value as port only */

                len = ಕೊನೆಯ - host;

                dash = ngx_strlchr(host, ಕೊನೆಯ, '-');

                ಆದರೆ (dash) {
                    dash++;

                    n = ngx_atoi(dash, ಕೊನೆಯ - dash);

                    ಆದರೆ (n == NGX_ERROR) {
                        ಹೋಗು no_ಪೋರ್ಟ್;
                    }

                    ಆದರೆ (n < 1 || n > 65535) {
                        u->err = "invalid port";

                    } ಇಲ್ಲದಿದ್ದರೆ {
                        u->ಕೊನೆಯ_ಪೋರ್ಟ್ = (ಒಳ_ಪೋರ್ಟ್_t) n;
                    }

                    len = dash - host - 1;
                }

                n = ngx_atoi(host, len);

                ಆದರೆ (n != NGX_ERROR) {

                    ಆದರೆ (u->err) {
                        ಹಿಂತಿರುಗಿ NGX_ERROR;
                    }

                    ಆದರೆ (n < 1 || n > 65535) {
                        u->err = "invalid port";
                        ಹಿಂತಿರುಗಿ NGX_ERROR;
                    }

                    ಆದರೆ (u->ಕೊನೆಯ_ಪೋರ್ಟ್ && n > u->ಕೊನೆಯ_ಪೋರ್ಟ್) {
                        u->err = "invalid port range";
                        ಹಿಂತಿರುಗಿ NGX_ERROR;
                    }

                    u->ಪೋರ್ಟ್ = (ಒಳ_ಪೋರ್ಟ್_t) n;
                    ಜ್ಯಾ->sin_ಪೋರ್ಟ್ = htons((ಒಳ_ಪೋರ್ಟ್_t) n);
                    ಜ್ಯಾ->sin_addr.s_addr = INADDR_ANY;

                    u->ಪೋರ್ಟ್_ಪಠ್ಯ.len = ಕೊನೆಯ - host;
                    u->ಪೋರ್ಟ್_ಪಠ್ಯ.ದತ್ತಾಂಶ = host;

                    u->wildcard = 1;

                    ಹಿಂತಿರುಗಿ ngx_inet_ಕೂಡಿಸು_addr(pool, u, &u->sockaddr.sockaddr,
                                             u->socklen, 1);
                }
            }
        }

no_ಪೋರ್ಟ್:

        u->err = ಖಾಲಿ;
        u->no_ಪೋರ್ಟ್ = 1;
        u->ಪೋರ್ಟ್ = u->default_ಪೋರ್ಟ್;
        ಜ್ಯಾ->sin_ಪೋರ್ಟ್ = htons(u->default_ಪೋರ್ಟ್);
        u->ಕೊನೆಯ_ಪೋರ್ಟ್ = 0;
    }

    len = ಕೊನೆಯ - host;

    ಆದರೆ (len == 0) {
        u->err = "no host";
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    u->host.len = len;
    u->host.ದತ್ತಾಂಶ = host;

    ಆದರೆ (u->listen && len == 1 && *host == '*') {
        ಜ್ಯಾ->sin_addr.s_addr = INADDR_ANY;
        u->wildcard = 1;
        ಹಿಂತಿರುಗಿ ngx_inet_ಕೂಡಿಸು_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);
    }

    ಜ್ಯಾ->sin_addr.s_addr = ngx_inet_addr(host, len);

    ಆದರೆ (ಜ್ಯಾ->sin_addr.s_addr != INADDR_NONE) {

        ಆದರೆ (ಜ್ಯಾ->sin_addr.s_addr == INADDR_ANY) {
            u->wildcard = 1;
        }

        ಹಿಂತಿರುಗಿ ngx_inet_ಕೂಡಿಸು_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);
    }

    ಆದರೆ (u->no_resolve) {
        ಹಿಂತಿರುಗಿ NGX_OK;
    }

    ಆದರೆ (ngx_inet_resolve_host(pool, u) != NGX_OK) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    u->family = u->addrs[0].sockaddr->sa_family;
    u->socklen = u->addrs[0].socklen;
    ngx_memcpy(&u->sockaddr, u->addrs[0].sockaddr, u->addrs[0].socklen);
    u->wildcard = ngx_inet_wildcard(&u->sockaddr.sockaddr);

    ಹಿಂತಿರುಗಿ NGX_OK;
}


ಸ್ಥಿರ ngx_int_t
ngx_parse_inet6_url(ngx_pool_t *pool, ngx_url_t *u)
{
#ಆದರೆ (NGX_HAVE_INET6)
    u_char               *p, *host, *ಪೋರ್ಟ್, *ಕೊನೆಯ, *uri, *dash;
    ಗಾತ್ರ_t                len;
    ngx_int_t             n;
    ರಚನೆ sockaddr_in6  *sin6;

    u->socklen = ಗಾತ್ರ(ರಚನೆ sockaddr_in6);
    sin6 = (ರಚನೆ sockaddr_in6 *) &u->sockaddr;
    sin6->sin6_family = AF_INET6;

    host = u->url.ದತ್ತಾಂಶ + 1;

    ಕೊನೆಯ = u->url.ದತ್ತಾಂಶ + u->url.len;

    p = ngx_strlchr(host, ಕೊನೆಯ, ']');

    ಆದರೆ (p == ಖಾಲಿ) {
        u->err = "invalid host";
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    ಪೋರ್ಟ್ = p + 1;

    uri = ngx_strlchr(ಪೋರ್ಟ್, ಕೊನೆಯ, '/');

    ಆದರೆ (uri) {
        ಆದರೆ (u->listen || !u->uri_part) {
            u->err = "invalid host";
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        u->uri.len = ಕೊನೆಯ - uri;
        u->uri.ದತ್ತಾಂಶ = uri;

        ಕೊನೆಯ = uri;
    }

    ಆದರೆ (ಪೋರ್ಟ್ < ಕೊನೆಯ) {
        ಆದರೆ (*ಪೋರ್ಟ್ != ':') {
            u->err = "invalid host";
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        ಪೋರ್ಟ್++;

        len = ಕೊನೆಯ - ಪೋರ್ಟ್;

        ಆದರೆ (u->listen) {
            dash = ngx_strlchr(ಪೋರ್ಟ್, ಕೊನೆಯ, '-');

            ಆದರೆ (dash) {
                dash++;

                n = ngx_atoi(dash, ಕೊನೆಯ - dash);

                ಆದರೆ (n < 1 || n > 65535) {
                    u->err = "invalid port";
                    ಹಿಂತಿರುಗಿ NGX_ERROR;
                }

                u->ಕೊನೆಯ_ಪೋರ್ಟ್ = (ಒಳ_ಪೋರ್ಟ್_t) n;

                len = dash - ಪೋರ್ಟ್ - 1;
            }
        }

        n = ngx_atoi(ಪೋರ್ಟ್, len);

        ಆದರೆ (n < 1 || n > 65535) {
            u->err = "invalid port";
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        ಆದರೆ (u->ಕೊನೆಯ_ಪೋರ್ಟ್ && n > u->ಕೊನೆಯ_ಪೋರ್ಟ್) {
            u->err = "invalid port range";
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        u->ಪೋರ್ಟ್ = (ಒಳ_ಪೋರ್ಟ್_t) n;
        sin6->sin6_ಪೋರ್ಟ್ = htons((ಒಳ_ಪೋರ್ಟ್_t) n);

        u->ಪೋರ್ಟ್_ಪಠ್ಯ.len = ಕೊನೆಯ - ಪೋರ್ಟ್;
        u->ಪೋರ್ಟ್_ಪಠ್ಯ.ದತ್ತಾಂಶ = ಪೋರ್ಟ್;

    } ಇಲ್ಲದಿದ್ದರೆ {
        u->no_ಪೋರ್ಟ್ = 1;
        u->ಪೋರ್ಟ್ = u->default_ಪೋರ್ಟ್;
        sin6->sin6_ಪೋರ್ಟ್ = htons(u->default_ಪೋರ್ಟ್);
    }

    len = p - host;

    ಆದರೆ (len == 0) {
        u->err = "no host";
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    u->host.len = len + 2;
    u->host.ದತ್ತಾಂಶ = host - 1;

    ಆದರೆ (ngx_inet6_addr(host, len, sin6->sin6_addr.s6_addr) != NGX_OK) {
        u->err = "invalid IPv6 address";
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    ಆದರೆ (IN6_IS_ADDR_UNSPECIFIED(&sin6->sin6_addr)) {
        u->wildcard = 1;
    }

    u->family = AF_INET6;

    ಹಿಂತಿರುಗಿ ngx_inet_ಕೂಡಿಸು_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);

#ಇಲ್ಲದಿದ್ದರೆ

    u->err = "the INET6 sockets are not supported on this platform";

    ಹಿಂತಿರುಗಿ NGX_ERROR;

#ಆದರೆ_ಅಂತ್ಯ
}


#ಆದರೆ (NGX_HAVE_GETADDRINFO && NGX_HAVE_INET6)

ngx_int_t
ngx_inet_resolve_host(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char           *host;
    ngx_uint_t        n;
    ರಚನೆ addrinfo   hints, *res, *rp;

    host = ngx_alloc(u->host.len + 1, pool->ಲಘುಗಣಕ);
    ಆದರೆ (host == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    (ಶೂನ್ಯ) ngx_cpystrn(host, u->host.ದತ್ತಾಂಶ, u->host.len + 1);

    ngx_memzero(&hints, ಗಾತ್ರ(ರಚನೆ addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_ಹರಿವು;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ AI_ADDRCONFIG
    hints.ai_flags = AI_ADDRCONFIG;
#ಆದರೆ_ಅಂತ್ಯ

    ಆದರೆ (getaddrinfo((ಅಕ್ಷರ *) host, ಖಾಲಿ, &hints, &res) != 0) {
        u->err = "host not found";
        ngx_free(host);
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    ngx_free(host);

    ಗಾಗಿ (n = 0, rp = res; rp != ಖಾಲಿ; rp = rp->ai_ಮುಂದಿನ) {

        ಆಯ್ಕೆ (rp->ai_family) {

        ಸ್ಥಿತಿ AF_INET:
        ಸ್ಥಿತಿ AF_INET6:
            ನಿಲ್ಲಿಸು;

        ಪೂರ್ವನಿಯೋಜಿತ:
            ಮುಂದುವರಿ;
        }

        n++;
    }

    ಆದರೆ (n == 0) {
        u->err = "host not found";
        ಹೋಗು failed;
    }

    /* MP: ngx_shared_palloc() */

    ಗಾಗಿ (rp = res; rp != ಖಾಲಿ; rp = rp->ai_ಮುಂದಿನ) {

        ಆಯ್ಕೆ (rp->ai_family) {

        ಸ್ಥಿತಿ AF_INET:
        ಸ್ಥಿತಿ AF_INET6:
            ನಿಲ್ಲಿಸು;

        ಪೂರ್ವನಿಯೋಜಿತ:
            ಮುಂದುವರಿ;
        }

        ಆದರೆ (ngx_inet_ಕೂಡಿಸು_addr(pool, u, rp->ai_addr, rp->ai_addrlen, n)
            != NGX_OK)
        {
            ಹೋಗು failed;
        }
    }

    freeaddrinfo(res);
    ಹಿಂತಿರುಗಿ NGX_OK;

failed:

    freeaddrinfo(res);
    ಹಿಂತಿರುಗಿ NGX_ERROR;
}

#ಇಲ್ಲದಿದ್ದರೆ /* !NGX_HAVE_GETADDRINFO || !NGX_HAVE_INET6 */

ngx_int_t
ngx_inet_resolve_host(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char              *host;
    ngx_uint_t           i, n;
    ರಚನೆ hostent      *h;
    ರಚನೆ sockaddr_ಒಳ   ಜ್ಯಾ;

    /* AF_INET only */

    ngx_memzero(&ಜ್ಯಾ, ಗಾತ್ರ(ರಚನೆ sockaddr_ಒಳ));

    ಜ್ಯಾ.sin_family = AF_INET;
    ಜ್ಯಾ.sin_addr.s_addr = ngx_inet_addr(u->host.ದತ್ತಾಂಶ, u->host.len);

    ಆದರೆ (ಜ್ಯಾ.sin_addr.s_addr == INADDR_NONE) {
        host = ngx_alloc(u->host.len + 1, pool->ಲಘುಗಣಕ);
        ಆದರೆ (host == ಖಾಲಿ) {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        (ಶೂನ್ಯ) ngx_cpystrn(host, u->host.ದತ್ತಾಂಶ, u->host.len + 1);

        h = gethostbyname((ಅಕ್ಷರ *) host);

        ngx_free(host);

        ಆದರೆ (h == ಖಾಲಿ || h->h_addr_list[0] == ಖಾಲಿ) {
            u->err = "host not found";
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        ಗಾಗಿ (n = 0; h->h_addr_list[n] != ಖಾಲಿ; n++) { /* void */ }

        /* MP: ngx_shared_palloc() */

        ಗಾಗಿ (i = 0; i < n; i++) {
            ಜ್ಯಾ.sin_addr.s_addr = *(ಒಳ_addr_t *) (h->h_addr_list[i]);

            ಆದರೆ (ngx_inet_ಕೂಡಿಸು_addr(pool, u, (ರಚನೆ sockaddr *) &ಜ್ಯಾ,
                                  ಗಾತ್ರ(ರಚನೆ sockaddr_ಒಳ), n)
                != NGX_OK)
            {
                ಹಿಂತಿರುಗಿ NGX_ERROR;
            }
        }

    } ಇಲ್ಲದಿದ್ದರೆ {

        /* MP: ngx_shared_palloc() */

        ಆದರೆ (ngx_inet_ಕೂಡಿಸು_addr(pool, u, (ರಚನೆ sockaddr *) &ಜ್ಯಾ,
                              ಗಾತ್ರ(ರಚನೆ sockaddr_ಒಳ), 1)
            != NGX_OK)
        {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }
    }

    ಹಿಂತಿರುಗಿ NGX_OK;
}

#ಆದರೆ_ಅಂತ್ಯ /* NGX_HAVE_GETADDRINFO && NGX_HAVE_INET6 */


ಸ್ಥಿರ ngx_int_t
ngx_inet_ಕೂಡಿಸು_addr(ngx_pool_t *pool, ngx_url_t *u, ರಚನೆ sockaddr *sockaddr,
    socklen_t socklen, ngx_uint_t ಒಟ್ಟು)
{
    u_char           *p;
    ಗಾತ್ರ_t            len;
    ngx_uint_t        i, nports;
    ngx_addr_t       *addr;
    ರಚನೆ sockaddr  *sa;

    nports = u->ಕೊನೆಯ_ಪೋರ್ಟ್ ? u->ಕೊನೆಯ_ಪೋರ್ಟ್ - u->ಪೋರ್ಟ್ + 1 : 1;

    ಆದರೆ (u->addrs == ಖಾಲಿ) {
        u->addrs = ngx_palloc(pool, ಒಟ್ಟು * nports * ಗಾತ್ರ(ngx_addr_t));
        ಆದರೆ (u->addrs == ಖಾಲಿ) {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }
    }

    ಗಾಗಿ (i = 0; i < nports; i++) {
        sa = ngx_pcalloc(pool, socklen);
        ಆದರೆ (sa == ಖಾಲಿ) {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        ngx_memcpy(sa, sockaddr, socklen);

        ngx_inet_ಗಣ_ಪೋರ್ಟ್(sa, u->ಪೋರ್ಟ್ + i);

        ಆಯ್ಕೆ (sa->sa_family) {

#ಆದರೆ (NGX_HAVE_INET6)
        ಸ್ಥಿತಿ AF_INET6:
            len = NGX_INET6_ADDRSTRLEN + ಗಾತ್ರ("[]:65536") - 1;
            ನಿಲ್ಲಿಸು;
#ಆದರೆ_ಅಂತ್ಯ

        ಪೂರ್ವನಿಯೋಜಿತ: /* AF_INET */
            len = NGX_INET_ADDRSTRLEN + ಗಾತ್ರ(":65535") - 1;
        }

        p = ngx_pnalloc(pool, len);
        ಆದರೆ (p == ಖಾಲಿ) {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        len = ngx_sock_ntop(sa, socklen, p, len, 1);

        addr = &u->addrs[u->naddrs++];

        addr->sockaddr = sa;
        addr->socklen = socklen;

        addr->name.len = len;
        addr->name.ದತ್ತಾಂಶ = p;
    }

    ಹಿಂತಿರುಗಿ NGX_OK;
}


ngx_int_t
ngx_cmp_sockaddr(ರಚನೆ sockaddr *sa1, socklen_t slen1,
    ರಚನೆ sockaddr *sa2, socklen_t slen2, ngx_uint_t cmp_ಪೋರ್ಟ್)
{
    ರಚನೆ sockaddr_ಒಳ   *sin1, *sin2;
#ಆದರೆ (NGX_HAVE_INET6)
    ರಚನೆ sockaddr_in6  *sin61, *sin62;
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ (NGX_HAVE_UNIX_DOMAIN)
    ಗಾತ್ರ_t                len;
    ರಚನೆ sockaddr_un   *saun1, *saun2;
#ಆದರೆ_ಅಂತ್ಯ

    ಆದರೆ (sa1->sa_family != sa2->sa_family) {
        ಹಿಂತಿರುಗಿ NGX_DECLINED;
    }

    ಆಯ್ಕೆ (sa1->sa_family) {

#ಆದರೆ (NGX_HAVE_INET6)
    ಸ್ಥಿತಿ AF_INET6:

        sin61 = (ರಚನೆ sockaddr_in6 *) sa1;
        sin62 = (ರಚನೆ sockaddr_in6 *) sa2;

        ಆದರೆ (cmp_ಪೋರ್ಟ್ && sin61->sin6_ಪೋರ್ಟ್ != sin62->sin6_ಪೋರ್ಟ್) {
            ಹಿಂತಿರುಗಿ NGX_DECLINED;
        }

        ಆದರೆ (ngx_memcmp(&sin61->sin6_addr, &sin62->sin6_addr, 16) != 0) {
            ಹಿಂತಿರುಗಿ NGX_DECLINED;
        }

        ನಿಲ್ಲಿಸು;
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ (NGX_HAVE_UNIX_DOMAIN)
    ಸ್ಥಿತಿ AF_UNIX:

        saun1 = (ರಚನೆ sockaddr_un *) sa1;
        saun2 = (ರಚನೆ sockaddr_un *) sa2;

        ಆದರೆ (slen1 < slen2) {
            len = slen1 - offsetof(ರಚನೆ sockaddr_un, sun_ಮಾರ್ಗ);

        } ಇಲ್ಲದಿದ್ದರೆ {
            len = slen2 - offsetof(ರಚನೆ sockaddr_un, sun_ಮಾರ್ಗ);
        }

        ಆದರೆ (len > ಗಾತ್ರ(saun1->sun_ಮಾರ್ಗ)) {
            len = ಗಾತ್ರ(saun1->sun_ಮಾರ್ಗ);
        }

        ಆದರೆ (ngx_memcmp(&saun1->sun_ಮಾರ್ಗ, &saun2->sun_ಮಾರ್ಗ, len) != 0) {
            ಹಿಂತಿರುಗಿ NGX_DECLINED;
        }

        ನಿಲ್ಲಿಸು;
#ಆದರೆ_ಅಂತ್ಯ

    ಪೂರ್ವನಿಯೋಜಿತ: /* AF_INET */

        sin1 = (ರಚನೆ sockaddr_ಒಳ *) sa1;
        sin2 = (ರಚನೆ sockaddr_ಒಳ *) sa2;

        ಆದರೆ (cmp_ಪೋರ್ಟ್ && sin1->sin_ಪೋರ್ಟ್ != sin2->sin_ಪೋರ್ಟ್) {
            ಹಿಂತಿರುಗಿ NGX_DECLINED;
        }

        ಆದರೆ (sin1->sin_addr.s_addr != sin2->sin_addr.s_addr) {
            ಹಿಂತಿರುಗಿ NGX_DECLINED;
        }

        ನಿಲ್ಲಿಸು;
    }

    ಹಿಂತಿರುಗಿ NGX_OK;
}


ಒಳ_ಪೋರ್ಟ್_t
ngx_inet_get_ಪೋರ್ಟ್(ರಚನೆ sockaddr *sa)
{
    ರಚನೆ sockaddr_ಒಳ   *ಜ್ಯಾ;
#ಆದರೆ (NGX_HAVE_INET6)
    ರಚನೆ sockaddr_in6  *sin6;
#ಆದರೆ_ಅಂತ್ಯ

    ಆಯ್ಕೆ (sa->sa_family) {

#ಆದರೆ (NGX_HAVE_INET6)
    ಸ್ಥಿತಿ AF_INET6:
        sin6 = (ರಚನೆ sockaddr_in6 *) sa;
        ಹಿಂತಿರುಗಿ ntohs(sin6->sin6_ಪೋರ್ಟ್);
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ (NGX_HAVE_UNIX_DOMAIN)
    ಸ್ಥಿತಿ AF_UNIX:
        ಹಿಂತಿರುಗಿ 0;
#ಆದರೆ_ಅಂತ್ಯ

    ಪೂರ್ವನಿಯೋಜಿತ: /* AF_INET */
        ಜ್ಯಾ = (ರಚನೆ sockaddr_ಒಳ *) sa;
        ಹಿಂತಿರುಗಿ ntohs(ಜ್ಯಾ->sin_ಪೋರ್ಟ್);
    }
}


ಶೂನ್ಯ
ngx_inet_ಗಣ_ಪೋರ್ಟ್(ರಚನೆ sockaddr *sa, ಒಳ_ಪೋರ್ಟ್_t ಪೋರ್ಟ್)
{
    ರಚನೆ sockaddr_ಒಳ   *ಜ್ಯಾ;
#ಆದರೆ (NGX_HAVE_INET6)
    ರಚನೆ sockaddr_in6  *sin6;
#ಆದರೆ_ಅಂತ್ಯ

    ಆಯ್ಕೆ (sa->sa_family) {

#ಆದರೆ (NGX_HAVE_INET6)
    ಸ್ಥಿತಿ AF_INET6:
        sin6 = (ರಚನೆ sockaddr_in6 *) sa;
        sin6->sin6_ಪೋರ್ಟ್ = htons(ಪೋರ್ಟ್);
        ನಿಲ್ಲಿಸು;
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ (NGX_HAVE_UNIX_DOMAIN)
    ಸ್ಥಿತಿ AF_UNIX:
        ನಿಲ್ಲಿಸು;
#ಆದರೆ_ಅಂತ್ಯ

    ಪೂರ್ವನಿಯೋಜಿತ: /* AF_INET */
        ಜ್ಯಾ = (ರಚನೆ sockaddr_ಒಳ *) sa;
        ಜ್ಯಾ->sin_ಪೋರ್ಟ್ = htons(ಪೋರ್ಟ್);
        ನಿಲ್ಲಿಸು;
    }
}


ngx_uint_t
ngx_inet_wildcard(ರಚನೆ sockaddr *sa)
{
    ರಚನೆ sockaddr_ಒಳ   *ಜ್ಯಾ;
#ಆದರೆ (NGX_HAVE_INET6)
    ರಚನೆ sockaddr_in6  *sin6;
#ಆದರೆ_ಅಂತ್ಯ

    ಆಯ್ಕೆ (sa->sa_family) {

    ಸ್ಥಿತಿ AF_INET:
        ಜ್ಯಾ = (ರಚನೆ sockaddr_ಒಳ *) sa;

        ಆದರೆ (ಜ್ಯಾ->sin_addr.s_addr == INADDR_ANY) {
            ಹಿಂತಿರುಗಿ 1;
        }

        ನಿಲ್ಲಿಸು;

#ಆದರೆ (NGX_HAVE_INET6)

    ಸ್ಥಿತಿ AF_INET6:
        sin6 = (ರಚನೆ sockaddr_in6 *) sa;

        ಆದರೆ (IN6_IS_ADDR_UNSPECIFIED(&sin6->sin6_addr)) {
            ಹಿಂತಿರುಗಿ 1;
        }

        ನಿಲ್ಲಿಸು;

#ಆದರೆ_ಅಂತ್ಯ
    }

    ಹಿಂತಿರುಗಿ 0;
}
