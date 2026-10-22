
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


ಸ್ಥಿರ ngx_int_t ngx_parse_unix_domain_url(ngx_pool_t *pool, ngx_url_t *u);
ಸ್ಥಿರ ngx_int_t ngx_parse_inet_url(ngx_pool_t *pool, ngx_url_t *u);
ಸ್ಥಿರ ngx_int_t ngx_parse_inet6_url(ngx_pool_t *pool, ngx_url_t *u);
ಸ್ಥಿರ ngx_int_t ngx_inet_add_addr(ngx_pool_t *pool, ngx_url_t *u,
    ಸಂರಚನಾ sockaddr *sockaddr, socklen_t socklen, ngx_uint_t total);


in_addr_t
ngx_inet_addr(u_char *text, size_t len)
{
    u_char      *p, c;
    in_addr_t    addr;
    ngx_uint_t   octet, n;

    addr = 0;
    octet = 0;
    n = 0;

    ಪ್ರತಿ (p = text; p < text + len; p++) {
        c = *p;

        ಯದಿ (c >= '0' && c <= '9') {
            octet = octet * 10 + (c - '0');

            ಯದಿ (octet > 255) {
                ಪ್ರತ್ಯಾಗಮ INADDR_NONE;
            }

            ಅನುವರ್ತ;
        }

        ಯದಿ (c == '.') {
            addr = (addr << 8) + octet;
            octet = 0;
            n++;
            ಅನುವರ್ತ;
        }

        ಪ್ರತ್ಯಾಗಮ INADDR_NONE;
    }

    ಯದಿ (n == 3) {
        addr = (addr << 8) + octet;
        ಪ್ರತ್ಯಾಗಮ htonl(addr);
    }

    ಪ್ರತ್ಯಾಗಮ INADDR_NONE;
}


#ಯದಿ (NGX_HAVE_INET6)

ngx_int_t
ngx_inet6_addr(u_char *p, size_t len, u_char *addr)
{
    u_char      c, *zero, *digit, *s, *d;
    size_t      len4;
    ngx_uint_t  n, nibbles, word;

    ಯದಿ (len == 0) {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    zero = ರಿಕ್ತ;
    digit = ರಿಕ್ತ;
    len4 = 0;
    nibbles = 0;
    word = 0;
    n = 8;

    ಯದಿ (p[0] == ':') {
        p++;
        len--;
    }

    ಪ್ರತಿ (/* void */; len; len--) {
        c = *p++;

        ಯದಿ (c == ':') {
            ಯದಿ (nibbles) {
                digit = p;
                len4 = len;
                *addr++ = (u_char) (word >> 8);
                *addr++ = (u_char) (word & 0xff);

                ಯದಿ (--n) {
                    nibbles = 0;
                    word = 0;
                    ಅನುವರ್ತ;
                }

            } ಅನ್ಯಥಾ {
                ಯದಿ (zero == ರಿಕ್ತ) {
                    digit = p;
                    len4 = len;
                    zero = addr;
                    ಅನುವರ್ತ;
                }
            }

            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        ಯದಿ (c == '.' && nibbles) {
            ಯದಿ (n < 2 || digit == ರಿಕ್ತ) {
                ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
            }

            word = ngx_inet_addr(digit, len4 - 1);
            ಯದಿ (word == INADDR_NONE) {
                ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
            }

            word = ntohl(word);
            *addr++ = (u_char) ((word >> 24) & 0xff);
            *addr++ = (u_char) ((word >> 16) & 0xff);
            n--;
            ಭಙ್ಗ;
        }

        ಯದಿ (++nibbles > 4) {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        ಯದಿ (c >= '0' && c <= '9') {
            word = word * 16 + (c - '0');
            ಅನುವರ್ತ;
        }

        c |= 0x20;

        ಯದಿ (c >= 'a' && c <= 'f') {
            word = word * 16 + (c - 'a') + 10;
            ಅನುವರ್ತ;
        }

        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    ಯದಿ (nibbles == 0 && zero == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    *addr++ = (u_char) (word >> 8);
    *addr++ = (u_char) (word & 0xff);

    ಯದಿ (--n) {
        ಯದಿ (zero) {
            n *= 2;
            s = addr - 1;
            d = s + n;
            ಯಾವತ್ (s >= zero) {
                *d-- = *s--;
            }
            ngx_memzero(zero, n);
            ಪ್ರತ್ಯಾಗಮ NGX_OK;
        }

    } ಅನ್ಯಥಾ {
        ಯದಿ (zero == ರಿಕ್ತ) {
            ಪ್ರತ್ಯಾಗಮ NGX_OK;
        }
    }

    ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
}

#endif


size_t
ngx_sock_ntop(ಸಂರಚನಾ sockaddr *sa, socklen_t socklen, u_char *text, size_t len,
    ngx_uint_t port)
{
    u_char               *p;
#ಯದಿ (NGX_HAVE_INET6 || NGX_HAVE_UNIX_DOMAIN)
    size_t                n;
#endif
    ಸಂರಚನಾ sockaddr_in   *sin;
#ಯದಿ (NGX_HAVE_INET6)
    ಸಂರಚನಾ sockaddr_in6  *sin6;
#endif
#ಯದಿ (NGX_HAVE_UNIX_DOMAIN)
    ಸಂರಚನಾ sockaddr_un   *saun;
#endif

    ವಿಕಲ್ಪ (sa->sa_family) {

    ಪಕ್ಷ AF_INET:

        sin = (ಸಂರಚನಾ sockaddr_in *) sa;
        p = (u_char *) &sin->sin_addr;

        ಯದಿ (port) {
            p = ngx_snprintf(text, len, "%ud.%ud.%ud.%ud:%d",
                             p[0], p[1], p[2], p[3], ntohs(sin->sin_port));
        } ಅನ್ಯಥಾ {
            p = ngx_snprintf(text, len, "%ud.%ud.%ud.%ud",
                             p[0], p[1], p[2], p[3]);
        }

        ಪ್ರತ್ಯಾಗಮ (p - text);

#ಯದಿ (NGX_HAVE_INET6)

    ಪಕ್ಷ AF_INET6:

        sin6 = (ಸಂರಚನಾ sockaddr_in6 *) sa;

        n = 0;

        ಯದಿ (port) {
            text[n++] = '[';
        }

        n = ngx_inet6_ntop(sin6->sin6_addr.s6_addr, &text[n], len);

        ಯದಿ (port) {
            n = ngx_sprintf(&text[1 + n], "]:%d",
                            ntohs(sin6->sin6_port)) - text;
        }

        ಪ್ರತ್ಯಾಗಮ n;
#endif

#ಯದಿ (NGX_HAVE_UNIX_DOMAIN)

    ಪಕ್ಷ AF_UNIX:
        saun = (ಸಂರಚನಾ sockaddr_un *) sa;

        /* on Linux sockaddr might not include sun_path at all */

        ಯದಿ (socklen <= (socklen_t) offsetof(ಸಂರಚನಾ sockaddr_un, sun_path)) {
            p = ngx_snprintf(text, len, "unix:%Z");

        } ಅನ್ಯಥಾ {
            n = ngx_strnlen((u_char *) saun->sun_path,
                            socklen - offsetof(ಸಂರಚನಾ sockaddr_un, sun_path));
            p = ngx_snprintf(text, len, "unix:%*s%Z", n, saun->sun_path);
        }

        /* we do not include trailing zero in address length */

        ಪ್ರತ್ಯಾಗಮ (p - text - 1);

#endif

    ಸ್ವಭಾವ:
        ಪ್ರತ್ಯಾಗಮ 0;
    }
}


size_t
ngx_inet_ntop(ಪೂರ್ಣಾಙ್ಕ family, ಶೂನ್ಯ *addr, u_char *text, size_t len)
{
    u_char  *p;

    ವಿಕಲ್ಪ (family) {

    ಪಕ್ಷ AF_INET:

        p = addr;

        ಪ್ರತ್ಯಾಗಮ ngx_snprintf(text, len, "%ud.%ud.%ud.%ud",
                            p[0], p[1], p[2], p[3])
               - text;

#ಯದಿ (NGX_HAVE_INET6)

    ಪಕ್ಷ AF_INET6:
        ಪ್ರತ್ಯಾಗಮ ngx_inet6_ntop(addr, text, len);

#endif

    ಸ್ವಭಾವ:
        ಪ್ರತ್ಯಾಗಮ 0;
    }
}


#ಯದಿ (NGX_HAVE_INET6)

size_t
ngx_inet6_ntop(u_char *p, u_char *text, size_t len)
{
    u_char      *dst;
    size_t       max, n;
    ngx_uint_t   i, zero, last;

    ಯದಿ (len < NGX_INET6_ADDRSTRLEN) {
        ಪ್ರತ್ಯಾಗಮ 0;
    }

    zero = (ngx_uint_t) -1;
    last = (ngx_uint_t) -1;
    max = 1;
    n = 0;

    ಪ್ರತಿ (i = 0; i < 16; i += 2) {

        ಯದಿ (p[i] || p[i + 1]) {

            ಯದಿ (max < n) {
                zero = last;
                max = n;
            }

            n = 0;
            ಅನುವರ್ತ;
        }

        ಯದಿ (n++ == 0) {
            last = i;
        }
    }

    ಯದಿ (max < n) {
        zero = last;
        max = n;
    }

    dst = text;
    n = 16;

    ಯದಿ (zero == 0) {

        ಯದಿ ((max == 5 && p[10] == 0xff && p[11] == 0xff)
            || (max == 6)
            || (max == 7 && p[14] != 0 && p[15] != 1))
        {
            n = 12;
        }

        *dst++ = ':';
    }

    ಪ್ರತಿ (i = 0; i < n; i += 2) {

        ಯದಿ (i == zero) {
            *dst++ = ':';
            i += (max - 1) * 2;
            ಅನುವರ್ತ;
        }

        dst = ngx_sprintf(dst, "%xd", p[i] * 256 + p[i + 1]);

        ಯದಿ (i < 14) {
            *dst++ = ':';
        }
    }

    ಯದಿ (n == 12) {
        dst = ngx_sprintf(dst, "%ud.%ud.%ud.%ud", p[12], p[13], p[14], p[15]);
    }

    ಪ್ರತ್ಯಾಗಮ dst - text;
}

#endif


ngx_int_t
ngx_ptocidr(ngx_str_t *text, ngx_cidr_t *cidr)
{
    u_char      *addr, *mask, *last;
    size_t       len;
    ngx_int_t    shift;
#ಯದಿ (NGX_HAVE_INET6)
    ngx_int_t    rc;
    ngx_uint_t   s, i;
#endif

    addr = text->data;
    last = addr + text->len;

    mask = ngx_strlchr(addr, last, '/');
    len = (mask ? mask : last) - addr;

    cidr->u.in.addr = ngx_inet_addr(addr, len);

    ಯದಿ (cidr->u.in.addr != INADDR_NONE) {
        cidr->family = AF_INET;

        ಯದಿ (mask == ರಿಕ್ತ) {
            cidr->u.in.mask = 0xffffffff;
            ಪ್ರತ್ಯಾಗಮ NGX_OK;
        }

#ಯದಿ (NGX_HAVE_INET6)
    } ಅನ್ಯಥಾ ಯದಿ (ngx_inet6_addr(addr, len, cidr->u.in6.addr.s6_addr) == NGX_OK) {
        cidr->family = AF_INET6;

        ಯದಿ (mask == ರಿಕ್ತ) {
            ngx_memset(cidr->u.in6.mask.s6_addr, 0xff, 16);
            ಪ್ರತ್ಯಾಗಮ NGX_OK;
        }

#endif
    } ಅನ್ಯಥಾ {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    mask++;

    shift = ngx_atoi(mask, last - mask);
    ಯದಿ (shift == NGX_ERROR) {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    ವಿಕಲ್ಪ (cidr->family) {

#ಯದಿ (NGX_HAVE_INET6)
    ಪಕ್ಷ AF_INET6:
        ಯದಿ (shift > 128) {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        addr = cidr->u.in6.addr.s6_addr;
        mask = cidr->u.in6.mask.s6_addr;
        rc = NGX_OK;

        ಪ್ರತಿ (i = 0; i < 16; i++) {

            s = (shift > 8) ? 8 : shift;
            shift -= s;

            mask[i] = (u_char) (0xffu << (8 - s));

            ಯದಿ (addr[i] != (addr[i] & mask[i])) {
                rc = NGX_DONE;
                addr[i] &= mask[i];
            }
        }

        ಪ್ರತ್ಯಾಗಮ rc;
#endif

    ಸ್ವಭಾವ: /* AF_INET */
        ಯದಿ (shift > 32) {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        ಯದಿ (shift) {
            cidr->u.in.mask = htonl((uint32_t) (0xffffffffu << (32 - shift)));

        } ಅನ್ಯಥಾ {
            /* x86 compilers use a shl instruction that shifts by modulo 32 */
            cidr->u.in.mask = 0;
        }

        ಯದಿ (cidr->u.in.addr == (cidr->u.in.addr & cidr->u.in.mask)) {
            ಪ್ರತ್ಯಾಗಮ NGX_OK;
        }

        cidr->u.in.addr &= cidr->u.in.mask;

        ಪ್ರತ್ಯಾಗಮ NGX_DONE;
    }
}


ngx_int_t
ngx_cidr_match(ಸಂರಚನಾ sockaddr *sa, ngx_array_t *cidrs)
{
#ಯದಿ (NGX_HAVE_INET6)
    u_char           *p;
#endif
    in_addr_t         inaddr;
    ngx_cidr_t       *cidr;
    ngx_uint_t        family, i;
#ಯದಿ (NGX_HAVE_INET6)
    ngx_uint_t        n;
    ಸಂರಚನಾ in6_addr  *inaddr6;
#endif

#ಯದಿ (NGX_SUPPRESS_WARN)
    inaddr = 0;
#ಯದಿ (NGX_HAVE_INET6)
    inaddr6 = ರಿಕ್ತ;
#endif
#endif

    family = sa->sa_family;

    ಯದಿ (family == AF_INET) {
        inaddr = ((ಸಂರಚನಾ sockaddr_in *) sa)->sin_addr.s_addr;
    }

#ಯದಿ (NGX_HAVE_INET6)
    ಅನ್ಯಥಾ ಯದಿ (family == AF_INET6) {
        inaddr6 = &((ಸಂರಚನಾ sockaddr_in6 *) sa)->sin6_addr;

        ಯದಿ (IN6_IS_ADDR_V4MAPPED(inaddr6)) {
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

    ಪ್ರತಿ (cidr = cidrs->elts, i = 0; i < cidrs->nelts; i++) {
        ಯದಿ (cidr[i].family != family) {
            ಗಚ್ಛ next;
        }

        ವಿಕಲ್ಪ (family) {

#ಯದಿ (NGX_HAVE_INET6)
        ಪಕ್ಷ AF_INET6:
            ಪ್ರತಿ (n = 0; n < 16; n++) {
                ಯದಿ ((inaddr6->s6_addr[n] & cidr[i].u.in6.mask.s6_addr[n])
                    != cidr[i].u.in6.addr.s6_addr[n])
                {
                    ಗಚ್ಛ next;
                }
            }
            ಭಙ್ಗ;
#endif

#ಯದಿ (NGX_HAVE_UNIX_DOMAIN)
        ಪಕ್ಷ AF_UNIX:
            ಭಙ್ಗ;
#endif

        ಸ್ವಭಾವ: /* AF_INET */
            ಯದಿ ((inaddr & cidr[i].u.in.mask) != cidr[i].u.in.addr) {
                ಗಚ್ಛ next;
            }
            ಭಙ್ಗ;
        }

        ಪ್ರತ್ಯಾಗಮ NGX_OK;

    next:
        ಅನುವರ್ತ;
    }

    ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;
}


ngx_int_t
ngx_parse_addr(ngx_pool_t *pool, ngx_addr_t *addr, u_char *text, size_t len)
{
    in_addr_t             inaddr;
    ngx_uint_t            family;
    ಸಂರಚನಾ sockaddr_in   *sin;
#ಯದಿ (NGX_HAVE_INET6)
    ಸಂರಚನಾ in6_addr       inaddr6;
    ಸಂರಚನಾ sockaddr_in6  *sin6;

    /*
     * prevent MSVC8 warning:
     *    potentially uninitialized local variable 'inaddr6' used
     */
    ngx_memzero(&inaddr6, ಪರಿಮಾಣ(ಸಂರಚನಾ in6_addr));
#endif

    inaddr = ngx_inet_addr(text, len);

    ಯದಿ (inaddr != INADDR_NONE) {
        family = AF_INET;
        len = ಪರಿಮಾಣ(ಸಂರಚನಾ sockaddr_in);

#ಯದಿ (NGX_HAVE_INET6)
    } ಅನ್ಯಥಾ ಯದಿ (ngx_inet6_addr(text, len, inaddr6.s6_addr) == NGX_OK) {
        family = AF_INET6;
        len = ಪರಿಮಾಣ(ಸಂರಚನಾ sockaddr_in6);

#endif
    } ಅನ್ಯಥಾ {
        ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;
    }

    addr->sockaddr = ngx_pcalloc(pool, len);
    ಯದಿ (addr->sockaddr == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    addr->sockaddr->sa_family = (u_char) family;
    addr->socklen = len;

    ವಿಕಲ್ಪ (family) {

#ಯದಿ (NGX_HAVE_INET6)
    ಪಕ್ಷ AF_INET6:
        sin6 = (ಸಂರಚನಾ sockaddr_in6 *) addr->sockaddr;
        ngx_memcpy(sin6->sin6_addr.s6_addr, inaddr6.s6_addr, 16);
        ಭಙ್ಗ;
#endif

    ಸ್ವಭಾವ: /* AF_INET */
        sin = (ಸಂರಚನಾ sockaddr_in *) addr->sockaddr;
        sin->sin_addr.s_addr = inaddr;
        ಭಙ್ಗ;
    }

    ಪ್ರತ್ಯಾಗಮ NGX_OK;
}


ngx_int_t
ngx_parse_addr_port(ngx_pool_t *pool, ngx_addr_t *addr, u_char *text,
    size_t len)
{
    u_char     *p, *last;
    size_t      plen;
    ngx_int_t   rc, port;

    rc = ngx_parse_addr(pool, addr, text, len);

    ಯದಿ (rc != NGX_DECLINED) {
        ಪ್ರತ್ಯಾಗಮ rc;
    }

    last = text + len;

#ಯದಿ (NGX_HAVE_INET6)
    ಯದಿ (len && text[0] == '[') {

        p = ngx_strlchr(text, last, ']');

        ಯದಿ (p == last - 1) {
            ಪ್ರತ್ಯಾಗಮ ngx_parse_addr(pool, addr, text + 1, len - 2);
        }

        ಯದಿ (p == ರಿಕ್ತ || *++p != ':') {
            ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;
        }

        text++;
        len -= 2;

    } ಅನ್ಯಥಾ
#endif

    {
        p = ngx_strlchr(text, last, ':');

        ಯದಿ (p == ರಿಕ್ತ) {
            ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;
        }
    }

    p++;
    plen = last - p;

    port = ngx_atoi(p, plen);

    ಯದಿ (port < 1 || port > 65535) {
        ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;
    }

    len -= plen + 1;

    rc = ngx_parse_addr(pool, addr, text, len);

    ಯದಿ (rc != NGX_OK) {
        ಪ್ರತ್ಯಾಗಮ rc;
    }

    ngx_inet_set_port(addr->sockaddr, (in_port_t) port);

    ಪ್ರತ್ಯಾಗಮ NGX_OK;
}


ngx_int_t
ngx_parse_url(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char  *p;
    size_t   len;

    p = u->url.data;
    len = u->url.len;

    ಯದಿ (len >= 5 && ngx_strncasecmp(p, (u_char *) "unix:", 5) == 0) {
        ಪ್ರತ್ಯಾಗಮ ngx_parse_unix_domain_url(pool, u);
    }

    ಯದಿ (len && p[0] == '[') {
        ಪ್ರತ್ಯಾಗಮ ngx_parse_inet6_url(pool, u);
    }

    ಪ್ರತ್ಯಾಗಮ ngx_parse_inet_url(pool, u);
}


ಸ್ಥಿರ ngx_int_t
ngx_parse_unix_domain_url(ngx_pool_t *pool, ngx_url_t *u)
{
#ಯದಿ (NGX_HAVE_UNIX_DOMAIN)
    u_char              *path, *uri, *last;
    size_t               len;
    ಸಂರಚನಾ sockaddr_un  *saun;

    len = u->url.len;
    path = u->url.data;

    path += 5;
    len -= 5;

    ಯದಿ (u->uri_part) {

        last = path + len;
        uri = ngx_strlchr(path, last, ':');

        ಯದಿ (uri) {
            len = uri - path;
            uri++;
            u->uri.len = last - uri;
            u->uri.data = uri;
        }
    }

    ಯದಿ (len == 0) {
        u->err = "no path in the unix domain socket";
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    u->host.len = len++;
    u->host.data = path;

    ಯದಿ (len > ಪರಿಮಾಣ(saun->sun_path)) {
        u->err = "too long path in the unix domain socket";
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    u->socklen = ಪರಿಮಾಣ(ಸಂರಚನಾ sockaddr_un);
    saun = (ಸಂರಚನಾ sockaddr_un *) &u->sockaddr;
    saun->sun_family = AF_UNIX;
    (ಶೂನ್ಯ) ngx_cpystrn((u_char *) saun->sun_path, path, len);

    u->addrs = ngx_pcalloc(pool, ಪರಿಮಾಣ(ngx_addr_t));
    ಯದಿ (u->addrs == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    saun = ngx_pcalloc(pool, ಪರಿಮಾಣ(ಸಂರಚನಾ sockaddr_un));
    ಯದಿ (saun == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    u->family = AF_UNIX;
    u->naddrs = 1;

    saun->sun_family = AF_UNIX;
    (ಶೂನ್ಯ) ngx_cpystrn((u_char *) saun->sun_path, path, len);

    u->addrs[0].sockaddr = (ಸಂರಚನಾ sockaddr *) saun;
    u->addrs[0].socklen = ಪರಿಮಾಣ(ಸಂರಚನಾ sockaddr_un);
    u->addrs[0].name.len = len + 4;
    u->addrs[0].name.data = u->url.data;

    ಪ್ರತ್ಯಾಗಮ NGX_OK;

#ಅನ್ಯಥಾ

    u->err = "the unix domain sockets are not supported on this platform";

    ಪ್ರತ್ಯಾಗಮ NGX_ERROR;

#endif
}


ಸ್ಥಿರ ngx_int_t
ngx_parse_inet_url(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char              *host, *port, *last, *uri, *args, *dash;
    size_t               len;
    ngx_int_t            n;
    ಸಂರಚನಾ sockaddr_in  *sin;

    u->socklen = ಪರಿಮಾಣ(ಸಂರಚನಾ sockaddr_in);
    sin = (ಸಂರಚನಾ sockaddr_in *) &u->sockaddr;
    sin->sin_family = AF_INET;

    u->family = AF_INET;

    host = u->url.data;

    last = host + u->url.len;

    port = ngx_strlchr(host, last, ':');

    uri = ngx_strlchr(host, last, '/');

    args = ngx_strlchr(host, last, '?');

    ಯದಿ (args) {
        ಯದಿ (uri == ರಿಕ್ತ || args < uri) {
            uri = args;
        }
    }

    ಯದಿ (uri) {
        ಯದಿ (u->listen || !u->uri_part) {
            u->err = "invalid host";
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        u->uri.len = last - uri;
        u->uri.data = uri;

        last = uri;

        ಯದಿ (uri < port) {
            port = ರಿಕ್ತ;
        }
    }

    ಯದಿ (port) {
        port++;

        len = last - port;

        ಯದಿ (u->listen) {
            dash = ngx_strlchr(port, last, '-');

            ಯದಿ (dash) {
                dash++;

                n = ngx_atoi(dash, last - dash);

                ಯದಿ (n < 1 || n > 65535) {
                    u->err = "invalid port";
                    ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
                }

                u->last_port = (in_port_t) n;

                len = dash - port - 1;
            }
        }

        n = ngx_atoi(port, len);

        ಯದಿ (n < 1 || n > 65535) {
            u->err = "invalid port";
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        ಯದಿ (u->last_port && n > u->last_port) {
            u->err = "invalid port range";
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        u->port = (in_port_t) n;
        sin->sin_port = htons((in_port_t) n);

        u->port_text.len = last - port;
        u->port_text.data = port;

        last = port - 1;

    } ಅನ್ಯಥಾ {
        ಯದಿ (uri == ರಿಕ್ತ) {

            ಯದಿ (u->listen) {

                /* test value as port only */

                len = last - host;

                dash = ngx_strlchr(host, last, '-');

                ಯದಿ (dash) {
                    dash++;

                    n = ngx_atoi(dash, last - dash);

                    ಯದಿ (n == NGX_ERROR) {
                        ಗಚ್ಛ no_port;
                    }

                    ಯದಿ (n < 1 || n > 65535) {
                        u->err = "invalid port";

                    } ಅನ್ಯಥಾ {
                        u->last_port = (in_port_t) n;
                    }

                    len = dash - host - 1;
                }

                n = ngx_atoi(host, len);

                ಯದಿ (n != NGX_ERROR) {

                    ಯದಿ (u->err) {
                        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
                    }

                    ಯದಿ (n < 1 || n > 65535) {
                        u->err = "invalid port";
                        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
                    }

                    ಯದಿ (u->last_port && n > u->last_port) {
                        u->err = "invalid port range";
                        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
                    }

                    u->port = (in_port_t) n;
                    sin->sin_port = htons((in_port_t) n);
                    sin->sin_addr.s_addr = INADDR_ANY;

                    u->port_text.len = last - host;
                    u->port_text.data = host;

                    u->wildcard = 1;

                    ಪ್ರತ್ಯಾಗಮ ngx_inet_add_addr(pool, u, &u->sockaddr.sockaddr,
                                             u->socklen, 1);
                }
            }
        }

no_port:

        u->err = ರಿಕ್ತ;
        u->no_port = 1;
        u->port = u->default_port;
        sin->sin_port = htons(u->default_port);
        u->last_port = 0;
    }

    len = last - host;

    ಯದಿ (len == 0) {
        u->err = "no host";
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    u->host.len = len;
    u->host.data = host;

    ಯದಿ (u->listen && len == 1 && *host == '*') {
        sin->sin_addr.s_addr = INADDR_ANY;
        u->wildcard = 1;
        ಪ್ರತ್ಯಾಗಮ ngx_inet_add_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);
    }

    sin->sin_addr.s_addr = ngx_inet_addr(host, len);

    ಯದಿ (sin->sin_addr.s_addr != INADDR_NONE) {

        ಯದಿ (sin->sin_addr.s_addr == INADDR_ANY) {
            u->wildcard = 1;
        }

        ಪ್ರತ್ಯಾಗಮ ngx_inet_add_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);
    }

    ಯದಿ (u->no_resolve) {
        ಪ್ರತ್ಯಾಗಮ NGX_OK;
    }

    ಯದಿ (ngx_inet_resolve_host(pool, u) != NGX_OK) {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    u->family = u->addrs[0].sockaddr->sa_family;
    u->socklen = u->addrs[0].socklen;
    ngx_memcpy(&u->sockaddr, u->addrs[0].sockaddr, u->addrs[0].socklen);
    u->wildcard = ngx_inet_wildcard(&u->sockaddr.sockaddr);

    ಪ್ರತ್ಯಾಗಮ NGX_OK;
}


ಸ್ಥಿರ ngx_int_t
ngx_parse_inet6_url(ngx_pool_t *pool, ngx_url_t *u)
{
#ಯದಿ (NGX_HAVE_INET6)
    u_char               *p, *host, *port, *last, *uri, *dash;
    size_t                len;
    ngx_int_t             n;
    ಸಂರಚನಾ sockaddr_in6  *sin6;

    u->socklen = ಪರಿಮಾಣ(ಸಂರಚನಾ sockaddr_in6);
    sin6 = (ಸಂರಚನಾ sockaddr_in6 *) &u->sockaddr;
    sin6->sin6_family = AF_INET6;

    host = u->url.data + 1;

    last = u->url.data + u->url.len;

    p = ngx_strlchr(host, last, ']');

    ಯದಿ (p == ರಿಕ್ತ) {
        u->err = "invalid host";
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    port = p + 1;

    uri = ngx_strlchr(port, last, '/');

    ಯದಿ (uri) {
        ಯದಿ (u->listen || !u->uri_part) {
            u->err = "invalid host";
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        u->uri.len = last - uri;
        u->uri.data = uri;

        last = uri;
    }

    ಯದಿ (port < last) {
        ಯದಿ (*port != ':') {
            u->err = "invalid host";
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        port++;

        len = last - port;

        ಯದಿ (u->listen) {
            dash = ngx_strlchr(port, last, '-');

            ಯದಿ (dash) {
                dash++;

                n = ngx_atoi(dash, last - dash);

                ಯದಿ (n < 1 || n > 65535) {
                    u->err = "invalid port";
                    ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
                }

                u->last_port = (in_port_t) n;

                len = dash - port - 1;
            }
        }

        n = ngx_atoi(port, len);

        ಯದಿ (n < 1 || n > 65535) {
            u->err = "invalid port";
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        ಯದಿ (u->last_port && n > u->last_port) {
            u->err = "invalid port range";
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        u->port = (in_port_t) n;
        sin6->sin6_port = htons((in_port_t) n);

        u->port_text.len = last - port;
        u->port_text.data = port;

    } ಅನ್ಯಥಾ {
        u->no_port = 1;
        u->port = u->default_port;
        sin6->sin6_port = htons(u->default_port);
    }

    len = p - host;

    ಯದಿ (len == 0) {
        u->err = "no host";
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    u->host.len = len + 2;
    u->host.data = host - 1;

    ಯದಿ (ngx_inet6_addr(host, len, sin6->sin6_addr.s6_addr) != NGX_OK) {
        u->err = "invalid IPv6 address";
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    ಯದಿ (IN6_IS_ADDR_UNSPECIFIED(&sin6->sin6_addr)) {
        u->wildcard = 1;
    }

    u->family = AF_INET6;

    ಪ್ರತ್ಯಾಗಮ ngx_inet_add_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);

#ಅನ್ಯಥಾ

    u->err = "the INET6 sockets are not supported on this platform";

    ಪ್ರತ್ಯಾಗಮ NGX_ERROR;

#endif
}


#ಯದಿ (NGX_HAVE_GETADDRINFO && NGX_HAVE_INET6)

ngx_int_t
ngx_inet_resolve_host(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char           *host;
    ngx_uint_t        n;
    ಸಂರಚನಾ addrinfo   hints, *res, *rp;

    host = ngx_alloc(u->host.len + 1, pool->log);
    ಯದಿ (host == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    (ಶೂನ್ಯ) ngx_cpystrn(host, u->host.data, u->host.len + 1);

    ngx_memzero(&hints, ಪರಿಮಾಣ(ಸಂರಚನಾ addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
#ifdef AI_ADDRCONFIG
    hints.ai_flags = AI_ADDRCONFIG;
#endif

    ಯದಿ (getaddrinfo((ವರ್ಣ *) host, ರಿಕ್ತ, &hints, &res) != 0) {
        u->err = "host not found";
        ngx_free(host);
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    ngx_free(host);

    ಪ್ರತಿ (n = 0, rp = res; rp != ರಿಕ್ತ; rp = rp->ai_next) {

        ವಿಕಲ್ಪ (rp->ai_family) {

        ಪಕ್ಷ AF_INET:
        ಪಕ್ಷ AF_INET6:
            ಭಙ್ಗ;

        ಸ್ವಭಾವ:
            ಅನುವರ್ತ;
        }

        n++;
    }

    ಯದಿ (n == 0) {
        u->err = "host not found";
        ಗಚ್ಛ failed;
    }

    /* MP: ngx_shared_palloc() */

    ಪ್ರತಿ (rp = res; rp != ರಿಕ್ತ; rp = rp->ai_next) {

        ವಿಕಲ್ಪ (rp->ai_family) {

        ಪಕ್ಷ AF_INET:
        ಪಕ್ಷ AF_INET6:
            ಭಙ್ಗ;

        ಸ್ವಭಾವ:
            ಅನುವರ್ತ;
        }

        ಯದಿ (ngx_inet_add_addr(pool, u, rp->ai_addr, rp->ai_addrlen, n)
            != NGX_OK)
        {
            ಗಚ್ಛ failed;
        }
    }

    freeaddrinfo(res);
    ಪ್ರತ್ಯಾಗಮ NGX_OK;

failed:

    freeaddrinfo(res);
    ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
}

#ಅನ್ಯಥಾ /* !NGX_HAVE_GETADDRINFO || !NGX_HAVE_INET6 */

ngx_int_t
ngx_inet_resolve_host(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char              *host;
    ngx_uint_t           i, n;
    ಸಂರಚನಾ hostent      *h;
    ಸಂರಚನಾ sockaddr_in   sin;

    /* AF_INET only */

    ngx_memzero(&sin, ಪರಿಮಾಣ(ಸಂರಚನಾ sockaddr_in));

    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = ngx_inet_addr(u->host.data, u->host.len);

    ಯದಿ (sin.sin_addr.s_addr == INADDR_NONE) {
        host = ngx_alloc(u->host.len + 1, pool->log);
        ಯದಿ (host == ರಿಕ್ತ) {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        (ಶೂನ್ಯ) ngx_cpystrn(host, u->host.data, u->host.len + 1);

        h = gethostbyname((ವರ್ಣ *) host);

        ngx_free(host);

        ಯದಿ (h == ರಿಕ್ತ || h->h_addr_list[0] == ರಿಕ್ತ) {
            u->err = "host not found";
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        ಪ್ರತಿ (n = 0; h->h_addr_list[n] != ರಿಕ್ತ; n++) { /* void */ }

        /* MP: ngx_shared_palloc() */

        ಪ್ರತಿ (i = 0; i < n; i++) {
            sin.sin_addr.s_addr = *(in_addr_t *) (h->h_addr_list[i]);

            ಯದಿ (ngx_inet_add_addr(pool, u, (ಸಂರಚನಾ sockaddr *) &sin,
                                  ಪರಿಮಾಣ(ಸಂರಚನಾ sockaddr_in), n)
                != NGX_OK)
            {
                ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
            }
        }

    } ಅನ್ಯಥಾ {

        /* MP: ngx_shared_palloc() */

        ಯದಿ (ngx_inet_add_addr(pool, u, (ಸಂರಚನಾ sockaddr *) &sin,
                              ಪರಿಮಾಣ(ಸಂರಚನಾ sockaddr_in), 1)
            != NGX_OK)
        {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }
    }

    ಪ್ರತ್ಯಾಗಮ NGX_OK;
}

#endif /* NGX_HAVE_GETADDRINFO && NGX_HAVE_INET6 */


ಸ್ಥಿರ ngx_int_t
ngx_inet_add_addr(ngx_pool_t *pool, ngx_url_t *u, ಸಂರಚನಾ sockaddr *sockaddr,
    socklen_t socklen, ngx_uint_t total)
{
    u_char           *p;
    size_t            len;
    ngx_uint_t        i, nports;
    ngx_addr_t       *addr;
    ಸಂರಚನಾ sockaddr  *sa;

    nports = u->last_port ? u->last_port - u->port + 1 : 1;

    ಯದಿ (u->addrs == ರಿಕ್ತ) {
        u->addrs = ngx_palloc(pool, total * nports * ಪರಿಮಾಣ(ngx_addr_t));
        ಯದಿ (u->addrs == ರಿಕ್ತ) {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }
    }

    ಪ್ರತಿ (i = 0; i < nports; i++) {
        sa = ngx_pcalloc(pool, socklen);
        ಯದಿ (sa == ರಿಕ್ತ) {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        ngx_memcpy(sa, sockaddr, socklen);

        ngx_inet_set_port(sa, u->port + i);

        ವಿಕಲ್ಪ (sa->sa_family) {

#ಯದಿ (NGX_HAVE_INET6)
        ಪಕ್ಷ AF_INET6:
            len = NGX_INET6_ADDRSTRLEN + ಪರಿಮಾಣ("[]:65536") - 1;
            ಭಙ್ಗ;
#endif

        ಸ್ವಭಾವ: /* AF_INET */
            len = NGX_INET_ADDRSTRLEN + ಪರಿಮಾಣ(":65535") - 1;
        }

        p = ngx_pnalloc(pool, len);
        ಯದಿ (p == ರಿಕ್ತ) {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        len = ngx_sock_ntop(sa, socklen, p, len, 1);

        addr = &u->addrs[u->naddrs++];

        addr->sockaddr = sa;
        addr->socklen = socklen;

        addr->name.len = len;
        addr->name.data = p;
    }

    ಪ್ರತ್ಯಾಗಮ NGX_OK;
}


ngx_int_t
ngx_cmp_sockaddr(ಸಂರಚನಾ sockaddr *sa1, socklen_t slen1,
    ಸಂರಚನಾ sockaddr *sa2, socklen_t slen2, ngx_uint_t cmp_port)
{
    ಸಂರಚನಾ sockaddr_in   *sin1, *sin2;
#ಯದಿ (NGX_HAVE_INET6)
    ಸಂರಚನಾ sockaddr_in6  *sin61, *sin62;
#endif
#ಯದಿ (NGX_HAVE_UNIX_DOMAIN)
    size_t                len;
    ಸಂರಚನಾ sockaddr_un   *saun1, *saun2;
#endif

    ಯದಿ (sa1->sa_family != sa2->sa_family) {
        ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;
    }

    ವಿಕಲ್ಪ (sa1->sa_family) {

#ಯದಿ (NGX_HAVE_INET6)
    ಪಕ್ಷ AF_INET6:

        sin61 = (ಸಂರಚನಾ sockaddr_in6 *) sa1;
        sin62 = (ಸಂರಚನಾ sockaddr_in6 *) sa2;

        ಯದಿ (cmp_port && sin61->sin6_port != sin62->sin6_port) {
            ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;
        }

        ಯದಿ (ngx_memcmp(&sin61->sin6_addr, &sin62->sin6_addr, 16) != 0) {
            ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;
        }

        ಭಙ್ಗ;
#endif

#ಯದಿ (NGX_HAVE_UNIX_DOMAIN)
    ಪಕ್ಷ AF_UNIX:

        saun1 = (ಸಂರಚನಾ sockaddr_un *) sa1;
        saun2 = (ಸಂರಚನಾ sockaddr_un *) sa2;

        ಯದಿ (slen1 < slen2) {
            len = slen1 - offsetof(ಸಂರಚನಾ sockaddr_un, sun_path);

        } ಅನ್ಯಥಾ {
            len = slen2 - offsetof(ಸಂರಚನಾ sockaddr_un, sun_path);
        }

        ಯದಿ (len > ಪರಿಮಾಣ(saun1->sun_path)) {
            len = ಪರಿಮಾಣ(saun1->sun_path);
        }

        ಯದಿ (ngx_memcmp(&saun1->sun_path, &saun2->sun_path, len) != 0) {
            ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;
        }

        ಭಙ್ಗ;
#endif

    ಸ್ವಭಾವ: /* AF_INET */

        sin1 = (ಸಂರಚನಾ sockaddr_in *) sa1;
        sin2 = (ಸಂರಚನಾ sockaddr_in *) sa2;

        ಯದಿ (cmp_port && sin1->sin_port != sin2->sin_port) {
            ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;
        }

        ಯದಿ (sin1->sin_addr.s_addr != sin2->sin_addr.s_addr) {
            ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;
        }

        ಭಙ್ಗ;
    }

    ಪ್ರತ್ಯಾಗಮ NGX_OK;
}


in_port_t
ngx_inet_get_port(ಸಂರಚನಾ sockaddr *sa)
{
    ಸಂರಚನಾ sockaddr_in   *sin;
#ಯದಿ (NGX_HAVE_INET6)
    ಸಂರಚನಾ sockaddr_in6  *sin6;
#endif

    ವಿಕಲ್ಪ (sa->sa_family) {

#ಯದಿ (NGX_HAVE_INET6)
    ಪಕ್ಷ AF_INET6:
        sin6 = (ಸಂರಚನಾ sockaddr_in6 *) sa;
        ಪ್ರತ್ಯಾಗಮ ntohs(sin6->sin6_port);
#endif

#ಯದಿ (NGX_HAVE_UNIX_DOMAIN)
    ಪಕ್ಷ AF_UNIX:
        ಪ್ರತ್ಯಾಗಮ 0;
#endif

    ಸ್ವಭಾವ: /* AF_INET */
        sin = (ಸಂರಚನಾ sockaddr_in *) sa;
        ಪ್ರತ್ಯಾಗಮ ntohs(sin->sin_port);
    }
}


ಶೂನ್ಯ
ngx_inet_set_port(ಸಂರಚನಾ sockaddr *sa, in_port_t port)
{
    ಸಂರಚನಾ sockaddr_in   *sin;
#ಯದಿ (NGX_HAVE_INET6)
    ಸಂರಚನಾ sockaddr_in6  *sin6;
#endif

    ವಿಕಲ್ಪ (sa->sa_family) {

#ಯದಿ (NGX_HAVE_INET6)
    ಪಕ್ಷ AF_INET6:
        sin6 = (ಸಂರಚನಾ sockaddr_in6 *) sa;
        sin6->sin6_port = htons(port);
        ಭಙ್ಗ;
#endif

#ಯದಿ (NGX_HAVE_UNIX_DOMAIN)
    ಪಕ್ಷ AF_UNIX:
        ಭಙ್ಗ;
#endif

    ಸ್ವಭಾವ: /* AF_INET */
        sin = (ಸಂರಚನಾ sockaddr_in *) sa;
        sin->sin_port = htons(port);
        ಭಙ್ಗ;
    }
}


ngx_uint_t
ngx_inet_wildcard(ಸಂರಚನಾ sockaddr *sa)
{
    ಸಂರಚನಾ sockaddr_in   *sin;
#ಯದಿ (NGX_HAVE_INET6)
    ಸಂರಚನಾ sockaddr_in6  *sin6;
#endif

    ವಿಕಲ್ಪ (sa->sa_family) {

    ಪಕ್ಷ AF_INET:
        sin = (ಸಂರಚನಾ sockaddr_in *) sa;

        ಯದಿ (sin->sin_addr.s_addr == INADDR_ANY) {
            ಪ್ರತ್ಯಾಗಮ 1;
        }

        ಭಙ್ಗ;

#ಯದಿ (NGX_HAVE_INET6)

    ಪಕ್ಷ AF_INET6:
        sin6 = (ಸಂರಚನಾ sockaddr_in6 *) sa;

        ಯದಿ (IN6_IS_ADDR_UNSPECIFIED(&sin6->sin6_addr)) {
            ಪ್ರತ್ಯಾಗಮ 1;
        }

        ಭಙ್ಗ;

#endif
    }

    ಪ್ರತ್ಯಾಗಮ 0;
}
