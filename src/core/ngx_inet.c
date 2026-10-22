
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>


స్థిర ngx_int_t ngx_parse_unix_domain_url(ngx_pool_t *pool, ngx_url_t *u);
స్థిర ngx_int_t ngx_parse_inet_url(ngx_pool_t *pool, ngx_url_t *u);
స్థిర ngx_int_t ngx_parse_inet6_url(ngx_pool_t *pool, ngx_url_t *u);
స్థిర ngx_int_t ngx_inet_కూడు_addr(ngx_pool_t *pool, ngx_url_t *u,
    నిర్మాణం sockaddr *sockaddr, socklen_t socklen, ngx_uint_t మొత్తం);


లోపల_addr_t
ngx_inet_addr(u_char *పాఠ్యం, కొలత_t len)
{
    u_char      *p, c;
    లోపల_addr_t    addr;
    ngx_uint_t   octet, n;

    addr = 0;
    octet = 0;
    n = 0;

    కొరకు (p = పాఠ్యం; p < పాఠ్యం + len; p++) {
        c = *p;

        ఒకవేళ (c >= '0' && c <= '9') {
            octet = octet * 10 + (c - '0');

            ఒకవేళ (octet > 255) {
                తిరిగి INADDR_NONE;
            }

            కొనసాగు;
        }

        ఒకవేళ (c == '.') {
            addr = (addr << 8) + octet;
            octet = 0;
            n++;
            కొనసాగు;
        }

        తిరిగి INADDR_NONE;
    }

    ఒకవేళ (n == 3) {
        addr = (addr << 8) + octet;
        తిరిగి htonl(addr);
    }

    తిరిగి INADDR_NONE;
}


#ఒకవేళ (NGX_HAVE_INET6)

ngx_int_t
ngx_inet6_addr(u_char *p, కొలత_t len, u_char *addr)
{
    u_char      c, *zero, *digit, *s, *d;
    కొలత_t      len4;
    ngx_uint_t  n, nibbles, మాట;

    ఒకవేళ (len == 0) {
        తిరిగి NGX_ERROR;
    }

    zero = ఖాళీ;
    digit = ఖాళీ;
    len4 = 0;
    nibbles = 0;
    మాట = 0;
    n = 8;

    ఒకవేళ (p[0] == ':') {
        p++;
        len--;
    }

    కొరకు (/* void */; len; len--) {
        c = *p++;

        ఒకవేళ (c == ':') {
            ఒకవేళ (nibbles) {
                digit = p;
                len4 = len;
                *addr++ = (u_char) (మాట >> 8);
                *addr++ = (u_char) (మాట & 0xff);

                ఒకవేళ (--n) {
                    nibbles = 0;
                    మాట = 0;
                    కొనసాగు;
                }

            } లేకపోతే {
                ఒకవేళ (zero == ఖాళీ) {
                    digit = p;
                    len4 = len;
                    zero = addr;
                    కొనసాగు;
                }
            }

            తిరిగి NGX_ERROR;
        }

        ఒకవేళ (c == '.' && nibbles) {
            ఒకవేళ (n < 2 || digit == ఖాళీ) {
                తిరిగి NGX_ERROR;
            }

            మాట = ngx_inet_addr(digit, len4 - 1);
            ఒకవేళ (మాట == INADDR_NONE) {
                తిరిగి NGX_ERROR;
            }

            మాట = ntohl(మాట);
            *addr++ = (u_char) ((మాట >> 24) & 0xff);
            *addr++ = (u_char) ((మాట >> 16) & 0xff);
            n--;
            ఆపు;
        }

        ఒకవేళ (++nibbles > 4) {
            తిరిగి NGX_ERROR;
        }

        ఒకవేళ (c >= '0' && c <= '9') {
            మాట = మాట * 16 + (c - '0');
            కొనసాగు;
        }

        c |= 0x20;

        ఒకవేళ (c >= 'a' && c <= 'f') {
            మాట = మాట * 16 + (c - 'a') + 10;
            కొనసాగు;
        }

        తిరిగి NGX_ERROR;
    }

    ఒకవేళ (nibbles == 0 && zero == ఖాళీ) {
        తిరిగి NGX_ERROR;
    }

    *addr++ = (u_char) (మాట >> 8);
    *addr++ = (u_char) (మాట & 0xff);

    ఒకవేళ (--n) {
        ఒకవేళ (zero) {
            n *= 2;
            s = addr - 1;
            d = s + n;
            అయితే (s >= zero) {
                *d-- = *s--;
            }
            ngx_memzero(zero, n);
            తిరిగి NGX_OK;
        }

    } లేకపోతే {
        ఒకవేళ (zero == ఖాళీ) {
            తిరిగి NGX_OK;
        }
    }

    తిరిగి NGX_ERROR;
}

#అయితే_ముగింపు


కొలత_t
ngx_sock_ntop(నిర్మాణం sockaddr *sa, socklen_t socklen, u_char *పాఠ్యం, కొలత_t len,
    ngx_uint_t పోర్టు)
{
    u_char               *p;
#ఒకవేళ (NGX_HAVE_INET6 || NGX_HAVE_UNIX_DOMAIN)
    కొలత_t                n;
#అయితే_ముగింపు
    నిర్మాణం sockaddr_లోపల   *జ్యా;
#ఒకవేళ (NGX_HAVE_INET6)
    నిర్మాణం sockaddr_in6  *sin6;
#అయితే_ముగింపు
#ఒకవేళ (NGX_HAVE_UNIX_DOMAIN)
    నిర్మాణం sockaddr_un   *saun;
#అయితే_ముగింపు

    ఎంపిక (sa->sa_family) {

    స్థితి AF_INET:

        జ్యా = (నిర్మాణం sockaddr_లోపల *) sa;
        p = (u_char *) &జ్యా->sin_addr;

        ఒకవేళ (పోర్టు) {
            p = ngx_snprintf(పాఠ్యం, len, "%ud.%ud.%ud.%ud:%d",
                             p[0], p[1], p[2], p[3], ntohs(జ్యా->sin_పోర్టు));
        } లేకపోతే {
            p = ngx_snprintf(పాఠ్యం, len, "%ud.%ud.%ud.%ud",
                             p[0], p[1], p[2], p[3]);
        }

        తిరిగి (p - పాఠ్యం);

#ఒకవేళ (NGX_HAVE_INET6)

    స్థితి AF_INET6:

        sin6 = (నిర్మాణం sockaddr_in6 *) sa;

        n = 0;

        ఒకవేళ (పోర్టు) {
            పాఠ్యం[n++] = '[';
        }

        n = ngx_inet6_ntop(sin6->sin6_addr.s6_addr, &పాఠ్యం[n], len);

        ఒకవేళ (పోర్టు) {
            n = ngx_sprintf(&పాఠ్యం[1 + n], "]:%d",
                            ntohs(sin6->sin6_పోర్టు)) - పాఠ్యం;
        }

        తిరిగి n;
#అయితే_ముగింపు

#ఒకవేళ (NGX_HAVE_UNIX_DOMAIN)

    స్థితి AF_UNIX:
        saun = (నిర్మాణం sockaddr_un *) sa;

        /* on Linux sockaddr might not include sun_path at all */

        ఒకవేళ (socklen <= (socklen_t) offsetof(నిర్మాణం sockaddr_un, sun_మార్గం)) {
            p = ngx_snprintf(పాఠ్యం, len, "unix:%Z");

        } లేకపోతే {
            n = ngx_strnlen((u_char *) saun->sun_మార్గం,
                            socklen - offsetof(నిర్మాణం sockaddr_un, sun_మార్గం));
            p = ngx_snprintf(పాఠ్యం, len, "unix:%*s%Z", n, saun->sun_మార్గం);
        }

        /* we do not include trailing zero in address length */

        తిరిగి (p - పాఠ్యం - 1);

#అయితే_ముగింపు

    అప్రమేయ:
        తిరిగి 0;
    }
}


కొలత_t
ngx_inet_ntop(పూర్ణాంకం family, శూన్య *addr, u_char *పాఠ్యం, కొలత_t len)
{
    u_char  *p;

    ఎంపిక (family) {

    స్థితి AF_INET:

        p = addr;

        తిరిగి ngx_snprintf(పాఠ్యం, len, "%ud.%ud.%ud.%ud",
                            p[0], p[1], p[2], p[3])
               - పాఠ్యం;

#ఒకవేళ (NGX_HAVE_INET6)

    స్థితి AF_INET6:
        తిరిగి ngx_inet6_ntop(addr, పాఠ్యం, len);

#అయితే_ముగింపు

    అప్రమేయ:
        తిరిగి 0;
    }
}


#ఒకవేళ (NGX_HAVE_INET6)

కొలత_t
ngx_inet6_ntop(u_char *p, u_char *పాఠ్యం, కొలత_t len)
{
    u_char      *dst;
    కొలత_t       max, n;
    ngx_uint_t   i, zero, చివరి;

    ఒకవేళ (len < NGX_INET6_ADDRSTRLEN) {
        తిరిగి 0;
    }

    zero = (ngx_uint_t) -1;
    చివరి = (ngx_uint_t) -1;
    max = 1;
    n = 0;

    కొరకు (i = 0; i < 16; i += 2) {

        ఒకవేళ (p[i] || p[i + 1]) {

            ఒకవేళ (max < n) {
                zero = చివరి;
                max = n;
            }

            n = 0;
            కొనసాగు;
        }

        ఒకవేళ (n++ == 0) {
            చివరి = i;
        }
    }

    ఒకవేళ (max < n) {
        zero = చివరి;
        max = n;
    }

    dst = పాఠ్యం;
    n = 16;

    ఒకవేళ (zero == 0) {

        ఒకవేళ ((max == 5 && p[10] == 0xff && p[11] == 0xff)
            || (max == 6)
            || (max == 7 && p[14] != 0 && p[15] != 1))
        {
            n = 12;
        }

        *dst++ = ':';
    }

    కొరకు (i = 0; i < n; i += 2) {

        ఒకవేళ (i == zero) {
            *dst++ = ':';
            i += (max - 1) * 2;
            కొనసాగు;
        }

        dst = ngx_sprintf(dst, "%xd", p[i] * 256 + p[i + 1]);

        ఒకవేళ (i < 14) {
            *dst++ = ':';
        }
    }

    ఒకవేళ (n == 12) {
        dst = ngx_sprintf(dst, "%ud.%ud.%ud.%ud", p[12], p[13], p[14], p[15]);
    }

    తిరిగి dst - పాఠ్యం;
}

#అయితే_ముగింపు


ngx_int_t
ngx_ptocidr(ngx_str_t *పాఠ్యం, ngx_cidr_t *cidr)
{
    u_char      *addr, *ముసుగు, *చివరి;
    కొలత_t       len;
    ngx_int_t    జరుపు;
#ఒకవేళ (NGX_HAVE_INET6)
    ngx_int_t    rc;
    ngx_uint_t   s, i;
#అయితే_ముగింపు

    addr = పాఠ్యం->దత్తాంశం;
    చివరి = addr + పాఠ్యం->len;

    ముసుగు = ngx_strlchr(addr, చివరి, '/');
    len = (ముసుగు ? ముసుగు : చివరి) - addr;

    cidr->u.లోపల.addr = ngx_inet_addr(addr, len);

    ఒకవేళ (cidr->u.లోపల.addr != INADDR_NONE) {
        cidr->family = AF_INET;

        ఒకవేళ (ముసుగు == ఖాళీ) {
            cidr->u.లోపల.ముసుగు = 0xffffffff;
            తిరిగి NGX_OK;
        }

#ఒకవేళ (NGX_HAVE_INET6)
    } లేకపోతే ఒకవేళ (ngx_inet6_addr(addr, len, cidr->u.in6.addr.s6_addr) == NGX_OK) {
        cidr->family = AF_INET6;

        ఒకవేళ (ముసుగు == ఖాళీ) {
            ngx_memset(cidr->u.in6.ముసుగు.s6_addr, 0xff, 16);
            తిరిగి NGX_OK;
        }

#అయితే_ముగింపు
    } లేకపోతే {
        తిరిగి NGX_ERROR;
    }

    ముసుగు++;

    జరుపు = ngx_atoi(ముసుగు, చివరి - ముసుగు);
    ఒకవేళ (జరుపు == NGX_ERROR) {
        తిరిగి NGX_ERROR;
    }

    ఎంపిక (cidr->family) {

#ఒకవేళ (NGX_HAVE_INET6)
    స్థితి AF_INET6:
        ఒకవేళ (జరుపు > 128) {
            తిరిగి NGX_ERROR;
        }

        addr = cidr->u.in6.addr.s6_addr;
        ముసుగు = cidr->u.in6.ముసుగు.s6_addr;
        rc = NGX_OK;

        కొరకు (i = 0; i < 16; i++) {

            s = (జరుపు > 8) ? 8 : జరుపు;
            జరుపు -= s;

            ముసుగు[i] = (u_char) (0xffu << (8 - s));

            ఒకవేళ (addr[i] != (addr[i] & ముసుగు[i])) {
                rc = NGX_పూర్తి;
                addr[i] &= ముసుగు[i];
            }
        }

        తిరిగి rc;
#అయితే_ముగింపు

    అప్రమేయ: /* AF_INET */
        ఒకవేళ (జరుపు > 32) {
            తిరిగి NGX_ERROR;
        }

        ఒకవేళ (జరుపు) {
            cidr->u.లోపల.ముసుగు = htonl((uint32_t) (0xffffffffu << (32 - జరుపు)));

        } లేకపోతే {
            /* x86 compilers use a shl instruction that shifts by modulo 32 */
            cidr->u.లోపల.ముసుగు = 0;
        }

        ఒకవేళ (cidr->u.లోపల.addr == (cidr->u.లోపల.addr & cidr->u.లోపల.ముసుగు)) {
            తిరిగి NGX_OK;
        }

        cidr->u.లోపల.addr &= cidr->u.లోపల.ముసుగు;

        తిరిగి NGX_పూర్తి;
    }
}


ngx_int_t
ngx_cidr_సరిపోలిక(నిర్మాణం sockaddr *sa, ngx_array_t *cidrs)
{
#ఒకవేళ (NGX_HAVE_INET6)
    u_char           *p;
#అయితే_ముగింపు
    లోపల_addr_t         inaddr;
    ngx_cidr_t       *cidr;
    ngx_uint_t        family, i;
#ఒకవేళ (NGX_HAVE_INET6)
    ngx_uint_t        n;
    నిర్మాణం in6_addr  *inaddr6;
#అయితే_ముగింపు

#ఒకవేళ (NGX_SUPPRESS_WARN)
    inaddr = 0;
#ఒకవేళ (NGX_HAVE_INET6)
    inaddr6 = ఖాళీ;
#అయితే_ముగింపు
#అయితే_ముగింపు

    family = sa->sa_family;

    ఒకవేళ (family == AF_INET) {
        inaddr = ((నిర్మాణం sockaddr_లోపల *) sa)->sin_addr.s_addr;
    }

#ఒకవేళ (NGX_HAVE_INET6)
    లేకపోతే ఒకవేళ (family == AF_INET6) {
        inaddr6 = &((నిర్మాణం sockaddr_in6 *) sa)->sin6_addr;

        ఒకవేళ (IN6_IS_ADDR_V4MAPPED(inaddr6)) {
            family = AF_INET;

            p = inaddr6->s6_addr;

            inaddr = (లోపల_addr_t) p[12] << 24;
            inaddr += p[13] << 16;
            inaddr += p[14] << 8;
            inaddr += p[15];

            inaddr = htonl(inaddr);
        }
    }
#అయితే_ముగింపు

    కొరకు (cidr = cidrs->elts, i = 0; i < cidrs->nelts; i++) {
        ఒకవేళ (cidr[i].family != family) {
            వెళ్ళు తదుపరి;
        }

        ఎంపిక (family) {

#ఒకవేళ (NGX_HAVE_INET6)
        స్థితి AF_INET6:
            కొరకు (n = 0; n < 16; n++) {
                ఒకవేళ ((inaddr6->s6_addr[n] & cidr[i].u.in6.ముసుగు.s6_addr[n])
                    != cidr[i].u.in6.addr.s6_addr[n])
                {
                    వెళ్ళు తదుపరి;
                }
            }
            ఆపు;
#అయితే_ముగింపు

#ఒకవేళ (NGX_HAVE_UNIX_DOMAIN)
        స్థితి AF_UNIX:
            ఆపు;
#అయితే_ముగింపు

        అప్రమేయ: /* AF_INET */
            ఒకవేళ ((inaddr & cidr[i].u.లోపల.ముసుగు) != cidr[i].u.లోపల.addr) {
                వెళ్ళు తదుపరి;
            }
            ఆపు;
        }

        తిరిగి NGX_OK;

    తదుపరి:
        కొనసాగు;
    }

    తిరిగి NGX_DECLINED;
}


ngx_int_t
ngx_parse_addr(ngx_pool_t *pool, ngx_addr_t *addr, u_char *పాఠ్యం, కొలత_t len)
{
    లోపల_addr_t             inaddr;
    ngx_uint_t            family;
    నిర్మాణం sockaddr_లోపల   *జ్యా;
#ఒకవేళ (NGX_HAVE_INET6)
    నిర్మాణం in6_addr       inaddr6;
    నిర్మాణం sockaddr_in6  *sin6;

    /*
     * prevent MSVC8 warning:
     *    potentially uninitialized local variable 'inaddr6' used
     */
    ngx_memzero(&inaddr6, పరిమాణం(నిర్మాణం in6_addr));
#అయితే_ముగింపు

    inaddr = ngx_inet_addr(పాఠ్యం, len);

    ఒకవేళ (inaddr != INADDR_NONE) {
        family = AF_INET;
        len = పరిమాణం(నిర్మాణం sockaddr_లోపల);

#ఒకవేళ (NGX_HAVE_INET6)
    } లేకపోతే ఒకవేళ (ngx_inet6_addr(పాఠ్యం, len, inaddr6.s6_addr) == NGX_OK) {
        family = AF_INET6;
        len = పరిమాణం(నిర్మాణం sockaddr_in6);

#అయితే_ముగింపు
    } లేకపోతే {
        తిరిగి NGX_DECLINED;
    }

    addr->sockaddr = ngx_pcalloc(pool, len);
    ఒకవేళ (addr->sockaddr == ఖాళీ) {
        తిరిగి NGX_ERROR;
    }

    addr->sockaddr->sa_family = (u_char) family;
    addr->socklen = len;

    ఎంపిక (family) {

#ఒకవేళ (NGX_HAVE_INET6)
    స్థితి AF_INET6:
        sin6 = (నిర్మాణం sockaddr_in6 *) addr->sockaddr;
        ngx_memcpy(sin6->sin6_addr.s6_addr, inaddr6.s6_addr, 16);
        ఆపు;
#అయితే_ముగింపు

    అప్రమేయ: /* AF_INET */
        జ్యా = (నిర్మాణం sockaddr_లోపల *) addr->sockaddr;
        జ్యా->sin_addr.s_addr = inaddr;
        ఆపు;
    }

    తిరిగి NGX_OK;
}


ngx_int_t
ngx_parse_addr_పోర్టు(ngx_pool_t *pool, ngx_addr_t *addr, u_char *పాఠ్యం,
    కొలత_t len)
{
    u_char     *p, *చివరి;
    కొలత_t      plen;
    ngx_int_t   rc, పోర్టు;

    rc = ngx_parse_addr(pool, addr, పాఠ్యం, len);

    ఒకవేళ (rc != NGX_DECLINED) {
        తిరిగి rc;
    }

    చివరి = పాఠ్యం + len;

#ఒకవేళ (NGX_HAVE_INET6)
    ఒకవేళ (len && పాఠ్యం[0] == '[') {

        p = ngx_strlchr(పాఠ్యం, చివరి, ']');

        ఒకవేళ (p == చివరి - 1) {
            తిరిగి ngx_parse_addr(pool, addr, పాఠ్యం + 1, len - 2);
        }

        ఒకవేళ (p == ఖాళీ || *++p != ':') {
            తిరిగి NGX_DECLINED;
        }

        పాఠ్యం++;
        len -= 2;

    } లేకపోతే
#అయితే_ముగింపు

    {
        p = ngx_strlchr(పాఠ్యం, చివరి, ':');

        ఒకవేళ (p == ఖాళీ) {
            తిరిగి NGX_DECLINED;
        }
    }

    p++;
    plen = చివరి - p;

    పోర్టు = ngx_atoi(p, plen);

    ఒకవేళ (పోర్టు < 1 || పోర్టు > 65535) {
        తిరిగి NGX_DECLINED;
    }

    len -= plen + 1;

    rc = ngx_parse_addr(pool, addr, పాఠ్యం, len);

    ఒకవేళ (rc != NGX_OK) {
        తిరిగి rc;
    }

    ngx_inet_సమితి_పోర్టు(addr->sockaddr, (లోపల_పోర్టు_t) పోర్టు);

    తిరిగి NGX_OK;
}


ngx_int_t
ngx_parse_url(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char  *p;
    కొలత_t   len;

    p = u->url.దత్తాంశం;
    len = u->url.len;

    ఒకవేళ (len >= 5 && ngx_strncasecmp(p, (u_char *) "unix:", 5) == 0) {
        తిరిగి ngx_parse_unix_domain_url(pool, u);
    }

    ఒకవేళ (len && p[0] == '[') {
        తిరిగి ngx_parse_inet6_url(pool, u);
    }

    తిరిగి ngx_parse_inet_url(pool, u);
}


స్థిర ngx_int_t
ngx_parse_unix_domain_url(ngx_pool_t *pool, ngx_url_t *u)
{
#ఒకవేళ (NGX_HAVE_UNIX_DOMAIN)
    u_char              *మార్గం, *uri, *చివరి;
    కొలత_t               len;
    నిర్మాణం sockaddr_un  *saun;

    len = u->url.len;
    మార్గం = u->url.దత్తాంశం;

    మార్గం += 5;
    len -= 5;

    ఒకవేళ (u->uri_part) {

        చివరి = మార్గం + len;
        uri = ngx_strlchr(మార్గం, చివరి, ':');

        ఒకవేళ (uri) {
            len = uri - మార్గం;
            uri++;
            u->uri.len = చివరి - uri;
            u->uri.దత్తాంశం = uri;
        }
    }

    ఒకవేళ (len == 0) {
        u->err = "no path in the unix domain socket";
        తిరిగి NGX_ERROR;
    }

    u->host.len = len++;
    u->host.దత్తాంశం = మార్గం;

    ఒకవేళ (len > పరిమాణం(saun->sun_మార్గం)) {
        u->err = "too long path in the unix domain socket";
        తిరిగి NGX_ERROR;
    }

    u->socklen = పరిమాణం(నిర్మాణం sockaddr_un);
    saun = (నిర్మాణం sockaddr_un *) &u->sockaddr;
    saun->sun_family = AF_UNIX;
    (శూన్య) ngx_cpystrn((u_char *) saun->sun_మార్గం, మార్గం, len);

    u->addrs = ngx_pcalloc(pool, పరిమాణం(ngx_addr_t));
    ఒకవేళ (u->addrs == ఖాళీ) {
        తిరిగి NGX_ERROR;
    }

    saun = ngx_pcalloc(pool, పరిమాణం(నిర్మాణం sockaddr_un));
    ఒకవేళ (saun == ఖాళీ) {
        తిరిగి NGX_ERROR;
    }

    u->family = AF_UNIX;
    u->naddrs = 1;

    saun->sun_family = AF_UNIX;
    (శూన్య) ngx_cpystrn((u_char *) saun->sun_మార్గం, మార్గం, len);

    u->addrs[0].sockaddr = (నిర్మాణం sockaddr *) saun;
    u->addrs[0].socklen = పరిమాణం(నిర్మాణం sockaddr_un);
    u->addrs[0].name.len = len + 4;
    u->addrs[0].name.దత్తాంశం = u->url.దత్తాంశం;

    తిరిగి NGX_OK;

#లేకపోతే

    u->err = "the unix domain sockets are not supported on this platform";

    తిరిగి NGX_ERROR;

#అయితే_ముగింపు
}


స్థిర ngx_int_t
ngx_parse_inet_url(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char              *host, *పోర్టు, *చివరి, *uri, *వాదనలు, *dash;
    కొలత_t               len;
    ngx_int_t            n;
    నిర్మాణం sockaddr_లోపల  *జ్యా;

    u->socklen = పరిమాణం(నిర్మాణం sockaddr_లోపల);
    జ్యా = (నిర్మాణం sockaddr_లోపల *) &u->sockaddr;
    జ్యా->sin_family = AF_INET;

    u->family = AF_INET;

    host = u->url.దత్తాంశం;

    చివరి = host + u->url.len;

    పోర్టు = ngx_strlchr(host, చివరి, ':');

    uri = ngx_strlchr(host, చివరి, '/');

    వాదనలు = ngx_strlchr(host, చివరి, '?');

    ఒకవేళ (వాదనలు) {
        ఒకవేళ (uri == ఖాళీ || వాదనలు < uri) {
            uri = వాదనలు;
        }
    }

    ఒకవేళ (uri) {
        ఒకవేళ (u->listen || !u->uri_part) {
            u->err = "invalid host";
            తిరిగి NGX_ERROR;
        }

        u->uri.len = చివరి - uri;
        u->uri.దత్తాంశం = uri;

        చివరి = uri;

        ఒకవేళ (uri < పోర్టు) {
            పోర్టు = ఖాళీ;
        }
    }

    ఒకవేళ (పోర్టు) {
        పోర్టు++;

        len = చివరి - పోర్టు;

        ఒకవేళ (u->listen) {
            dash = ngx_strlchr(పోర్టు, చివరి, '-');

            ఒకవేళ (dash) {
                dash++;

                n = ngx_atoi(dash, చివరి - dash);

                ఒకవేళ (n < 1 || n > 65535) {
                    u->err = "invalid port";
                    తిరిగి NGX_ERROR;
                }

                u->చివరి_పోర్టు = (లోపల_పోర్టు_t) n;

                len = dash - పోర్టు - 1;
            }
        }

        n = ngx_atoi(పోర్టు, len);

        ఒకవేళ (n < 1 || n > 65535) {
            u->err = "invalid port";
            తిరిగి NGX_ERROR;
        }

        ఒకవేళ (u->చివరి_పోర్టు && n > u->చివరి_పోర్టు) {
            u->err = "invalid port range";
            తిరిగి NGX_ERROR;
        }

        u->పోర్టు = (లోపల_పోర్టు_t) n;
        జ్యా->sin_పోర్టు = htons((లోపల_పోర్టు_t) n);

        u->పోర్టు_పాఠ్యం.len = చివరి - పోర్టు;
        u->పోర్టు_పాఠ్యం.దత్తాంశం = పోర్టు;

        చివరి = పోర్టు - 1;

    } లేకపోతే {
        ఒకవేళ (uri == ఖాళీ) {

            ఒకవేళ (u->listen) {

                /* test value as port only */

                len = చివరి - host;

                dash = ngx_strlchr(host, చివరి, '-');

                ఒకవేళ (dash) {
                    dash++;

                    n = ngx_atoi(dash, చివరి - dash);

                    ఒకవేళ (n == NGX_ERROR) {
                        వెళ్ళు no_పోర్టు;
                    }

                    ఒకవేళ (n < 1 || n > 65535) {
                        u->err = "invalid port";

                    } లేకపోతే {
                        u->చివరి_పోర్టు = (లోపల_పోర్టు_t) n;
                    }

                    len = dash - host - 1;
                }

                n = ngx_atoi(host, len);

                ఒకవేళ (n != NGX_ERROR) {

                    ఒకవేళ (u->err) {
                        తిరిగి NGX_ERROR;
                    }

                    ఒకవేళ (n < 1 || n > 65535) {
                        u->err = "invalid port";
                        తిరిగి NGX_ERROR;
                    }

                    ఒకవేళ (u->చివరి_పోర్టు && n > u->చివరి_పోర్టు) {
                        u->err = "invalid port range";
                        తిరిగి NGX_ERROR;
                    }

                    u->పోర్టు = (లోపల_పోర్టు_t) n;
                    జ్యా->sin_పోర్టు = htons((లోపల_పోర్టు_t) n);
                    జ్యా->sin_addr.s_addr = INADDR_ANY;

                    u->పోర్టు_పాఠ్యం.len = చివరి - host;
                    u->పోర్టు_పాఠ్యం.దత్తాంశం = host;

                    u->wildcard = 1;

                    తిరిగి ngx_inet_కూడు_addr(pool, u, &u->sockaddr.sockaddr,
                                             u->socklen, 1);
                }
            }
        }

no_పోర్టు:

        u->err = ఖాళీ;
        u->no_పోర్టు = 1;
        u->పోర్టు = u->default_పోర్టు;
        జ్యా->sin_పోర్టు = htons(u->default_పోర్టు);
        u->చివరి_పోర్టు = 0;
    }

    len = చివరి - host;

    ఒకవేళ (len == 0) {
        u->err = "no host";
        తిరిగి NGX_ERROR;
    }

    u->host.len = len;
    u->host.దత్తాంశం = host;

    ఒకవేళ (u->listen && len == 1 && *host == '*') {
        జ్యా->sin_addr.s_addr = INADDR_ANY;
        u->wildcard = 1;
        తిరిగి ngx_inet_కూడు_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);
    }

    జ్యా->sin_addr.s_addr = ngx_inet_addr(host, len);

    ఒకవేళ (జ్యా->sin_addr.s_addr != INADDR_NONE) {

        ఒకవేళ (జ్యా->sin_addr.s_addr == INADDR_ANY) {
            u->wildcard = 1;
        }

        తిరిగి ngx_inet_కూడు_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);
    }

    ఒకవేళ (u->no_resolve) {
        తిరిగి NGX_OK;
    }

    ఒకవేళ (ngx_inet_resolve_host(pool, u) != NGX_OK) {
        తిరిగి NGX_ERROR;
    }

    u->family = u->addrs[0].sockaddr->sa_family;
    u->socklen = u->addrs[0].socklen;
    ngx_memcpy(&u->sockaddr, u->addrs[0].sockaddr, u->addrs[0].socklen);
    u->wildcard = ngx_inet_wildcard(&u->sockaddr.sockaddr);

    తిరిగి NGX_OK;
}


స్థిర ngx_int_t
ngx_parse_inet6_url(ngx_pool_t *pool, ngx_url_t *u)
{
#ఒకవేళ (NGX_HAVE_INET6)
    u_char               *p, *host, *పోర్టు, *చివరి, *uri, *dash;
    కొలత_t                len;
    ngx_int_t             n;
    నిర్మాణం sockaddr_in6  *sin6;

    u->socklen = పరిమాణం(నిర్మాణం sockaddr_in6);
    sin6 = (నిర్మాణం sockaddr_in6 *) &u->sockaddr;
    sin6->sin6_family = AF_INET6;

    host = u->url.దత్తాంశం + 1;

    చివరి = u->url.దత్తాంశం + u->url.len;

    p = ngx_strlchr(host, చివరి, ']');

    ఒకవేళ (p == ఖాళీ) {
        u->err = "invalid host";
        తిరిగి NGX_ERROR;
    }

    పోర్టు = p + 1;

    uri = ngx_strlchr(పోర్టు, చివరి, '/');

    ఒకవేళ (uri) {
        ఒకవేళ (u->listen || !u->uri_part) {
            u->err = "invalid host";
            తిరిగి NGX_ERROR;
        }

        u->uri.len = చివరి - uri;
        u->uri.దత్తాంశం = uri;

        చివరి = uri;
    }

    ఒకవేళ (పోర్టు < చివరి) {
        ఒకవేళ (*పోర్టు != ':') {
            u->err = "invalid host";
            తిరిగి NGX_ERROR;
        }

        పోర్టు++;

        len = చివరి - పోర్టు;

        ఒకవేళ (u->listen) {
            dash = ngx_strlchr(పోర్టు, చివరి, '-');

            ఒకవేళ (dash) {
                dash++;

                n = ngx_atoi(dash, చివరి - dash);

                ఒకవేళ (n < 1 || n > 65535) {
                    u->err = "invalid port";
                    తిరిగి NGX_ERROR;
                }

                u->చివరి_పోర్టు = (లోపల_పోర్టు_t) n;

                len = dash - పోర్టు - 1;
            }
        }

        n = ngx_atoi(పోర్టు, len);

        ఒకవేళ (n < 1 || n > 65535) {
            u->err = "invalid port";
            తిరిగి NGX_ERROR;
        }

        ఒకవేళ (u->చివరి_పోర్టు && n > u->చివరి_పోర్టు) {
            u->err = "invalid port range";
            తిరిగి NGX_ERROR;
        }

        u->పోర్టు = (లోపల_పోర్టు_t) n;
        sin6->sin6_పోర్టు = htons((లోపల_పోర్టు_t) n);

        u->పోర్టు_పాఠ్యం.len = చివరి - పోర్టు;
        u->పోర్టు_పాఠ్యం.దత్తాంశం = పోర్టు;

    } లేకపోతే {
        u->no_పోర్టు = 1;
        u->పోర్టు = u->default_పోర్టు;
        sin6->sin6_పోర్టు = htons(u->default_పోర్టు);
    }

    len = p - host;

    ఒకవేళ (len == 0) {
        u->err = "no host";
        తిరిగి NGX_ERROR;
    }

    u->host.len = len + 2;
    u->host.దత్తాంశం = host - 1;

    ఒకవేళ (ngx_inet6_addr(host, len, sin6->sin6_addr.s6_addr) != NGX_OK) {
        u->err = "invalid IPv6 address";
        తిరిగి NGX_ERROR;
    }

    ఒకవేళ (IN6_IS_ADDR_UNSPECIFIED(&sin6->sin6_addr)) {
        u->wildcard = 1;
    }

    u->family = AF_INET6;

    తిరిగి ngx_inet_కూడు_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);

#లేకపోతే

    u->err = "the INET6 sockets are not supported on this platform";

    తిరిగి NGX_ERROR;

#అయితే_ముగింపు
}


#ఒకవేళ (NGX_HAVE_GETADDRINFO && NGX_HAVE_INET6)

ngx_int_t
ngx_inet_resolve_host(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char           *host;
    ngx_uint_t        n;
    నిర్మాణం addrinfo   hints, *res, *rp;

    host = ngx_alloc(u->host.len + 1, pool->సంవర్గమానం);
    ఒకవేళ (host == ఖాళీ) {
        తిరిగి NGX_ERROR;
    }

    (శూన్య) ngx_cpystrn(host, u->host.దత్తాంశం, u->host.len + 1);

    ngx_memzero(&hints, పరిమాణం(నిర్మాణం addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_ప్రవాహం;
#నిర్వచితమైతే AI_ADDRCONFIG
    hints.ai_flags = AI_ADDRCONFIG;
#అయితే_ముగింపు

    ఒకవేళ (getaddrinfo((అక్షరం *) host, ఖాళీ, &hints, &res) != 0) {
        u->err = "host not found";
        ngx_free(host);
        తిరిగి NGX_ERROR;
    }

    ngx_free(host);

    కొరకు (n = 0, rp = res; rp != ఖాళీ; rp = rp->ai_తదుపరి) {

        ఎంపిక (rp->ai_family) {

        స్థితి AF_INET:
        స్థితి AF_INET6:
            ఆపు;

        అప్రమేయ:
            కొనసాగు;
        }

        n++;
    }

    ఒకవేళ (n == 0) {
        u->err = "host not found";
        వెళ్ళు failed;
    }

    /* MP: ngx_shared_palloc() */

    కొరకు (rp = res; rp != ఖాళీ; rp = rp->ai_తదుపరి) {

        ఎంపిక (rp->ai_family) {

        స్థితి AF_INET:
        స్థితి AF_INET6:
            ఆపు;

        అప్రమేయ:
            కొనసాగు;
        }

        ఒకవేళ (ngx_inet_కూడు_addr(pool, u, rp->ai_addr, rp->ai_addrlen, n)
            != NGX_OK)
        {
            వెళ్ళు failed;
        }
    }

    freeaddrinfo(res);
    తిరిగి NGX_OK;

failed:

    freeaddrinfo(res);
    తిరిగి NGX_ERROR;
}

#లేకపోతే /* !NGX_HAVE_GETADDRINFO || !NGX_HAVE_INET6 */

ngx_int_t
ngx_inet_resolve_host(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char              *host;
    ngx_uint_t           i, n;
    నిర్మాణం hostent      *h;
    నిర్మాణం sockaddr_లోపల   జ్యా;

    /* AF_INET only */

    ngx_memzero(&జ్యా, పరిమాణం(నిర్మాణం sockaddr_లోపల));

    జ్యా.sin_family = AF_INET;
    జ్యా.sin_addr.s_addr = ngx_inet_addr(u->host.దత్తాంశం, u->host.len);

    ఒకవేళ (జ్యా.sin_addr.s_addr == INADDR_NONE) {
        host = ngx_alloc(u->host.len + 1, pool->సంవర్గమానం);
        ఒకవేళ (host == ఖాళీ) {
            తిరిగి NGX_ERROR;
        }

        (శూన్య) ngx_cpystrn(host, u->host.దత్తాంశం, u->host.len + 1);

        h = gethostbyname((అక్షరం *) host);

        ngx_free(host);

        ఒకవేళ (h == ఖాళీ || h->h_addr_list[0] == ఖాళీ) {
            u->err = "host not found";
            తిరిగి NGX_ERROR;
        }

        కొరకు (n = 0; h->h_addr_list[n] != ఖాళీ; n++) { /* void */ }

        /* MP: ngx_shared_palloc() */

        కొరకు (i = 0; i < n; i++) {
            జ్యా.sin_addr.s_addr = *(లోపల_addr_t *) (h->h_addr_list[i]);

            ఒకవేళ (ngx_inet_కూడు_addr(pool, u, (నిర్మాణం sockaddr *) &జ్యా,
                                  పరిమాణం(నిర్మాణం sockaddr_లోపల), n)
                != NGX_OK)
            {
                తిరిగి NGX_ERROR;
            }
        }

    } లేకపోతే {

        /* MP: ngx_shared_palloc() */

        ఒకవేళ (ngx_inet_కూడు_addr(pool, u, (నిర్మాణం sockaddr *) &జ్యా,
                              పరిమాణం(నిర్మాణం sockaddr_లోపల), 1)
            != NGX_OK)
        {
            తిరిగి NGX_ERROR;
        }
    }

    తిరిగి NGX_OK;
}

#అయితే_ముగింపు /* NGX_HAVE_GETADDRINFO && NGX_HAVE_INET6 */


స్థిర ngx_int_t
ngx_inet_కూడు_addr(ngx_pool_t *pool, ngx_url_t *u, నిర్మాణం sockaddr *sockaddr,
    socklen_t socklen, ngx_uint_t మొత్తం)
{
    u_char           *p;
    కొలత_t            len;
    ngx_uint_t        i, nports;
    ngx_addr_t       *addr;
    నిర్మాణం sockaddr  *sa;

    nports = u->చివరి_పోర్టు ? u->చివరి_పోర్టు - u->పోర్టు + 1 : 1;

    ఒకవేళ (u->addrs == ఖాళీ) {
        u->addrs = ngx_palloc(pool, మొత్తం * nports * పరిమాణం(ngx_addr_t));
        ఒకవేళ (u->addrs == ఖాళీ) {
            తిరిగి NGX_ERROR;
        }
    }

    కొరకు (i = 0; i < nports; i++) {
        sa = ngx_pcalloc(pool, socklen);
        ఒకవేళ (sa == ఖాళీ) {
            తిరిగి NGX_ERROR;
        }

        ngx_memcpy(sa, sockaddr, socklen);

        ngx_inet_సమితి_పోర్టు(sa, u->పోర్టు + i);

        ఎంపిక (sa->sa_family) {

#ఒకవేళ (NGX_HAVE_INET6)
        స్థితి AF_INET6:
            len = NGX_INET6_ADDRSTRLEN + పరిమాణం("[]:65536") - 1;
            ఆపు;
#అయితే_ముగింపు

        అప్రమేయ: /* AF_INET */
            len = NGX_INET_ADDRSTRLEN + పరిమాణం(":65535") - 1;
        }

        p = ngx_pnalloc(pool, len);
        ఒకవేళ (p == ఖాళీ) {
            తిరిగి NGX_ERROR;
        }

        len = ngx_sock_ntop(sa, socklen, p, len, 1);

        addr = &u->addrs[u->naddrs++];

        addr->sockaddr = sa;
        addr->socklen = socklen;

        addr->name.len = len;
        addr->name.దత్తాంశం = p;
    }

    తిరిగి NGX_OK;
}


ngx_int_t
ngx_cmp_sockaddr(నిర్మాణం sockaddr *sa1, socklen_t slen1,
    నిర్మాణం sockaddr *sa2, socklen_t slen2, ngx_uint_t cmp_పోర్టు)
{
    నిర్మాణం sockaddr_లోపల   *sin1, *sin2;
#ఒకవేళ (NGX_HAVE_INET6)
    నిర్మాణం sockaddr_in6  *sin61, *sin62;
#అయితే_ముగింపు
#ఒకవేళ (NGX_HAVE_UNIX_DOMAIN)
    కొలత_t                len;
    నిర్మాణం sockaddr_un   *saun1, *saun2;
#అయితే_ముగింపు

    ఒకవేళ (sa1->sa_family != sa2->sa_family) {
        తిరిగి NGX_DECLINED;
    }

    ఎంపిక (sa1->sa_family) {

#ఒకవేళ (NGX_HAVE_INET6)
    స్థితి AF_INET6:

        sin61 = (నిర్మాణం sockaddr_in6 *) sa1;
        sin62 = (నిర్మాణం sockaddr_in6 *) sa2;

        ఒకవేళ (cmp_పోర్టు && sin61->sin6_పోర్టు != sin62->sin6_పోర్టు) {
            తిరిగి NGX_DECLINED;
        }

        ఒకవేళ (ngx_memcmp(&sin61->sin6_addr, &sin62->sin6_addr, 16) != 0) {
            తిరిగి NGX_DECLINED;
        }

        ఆపు;
#అయితే_ముగింపు

#ఒకవేళ (NGX_HAVE_UNIX_DOMAIN)
    స్థితి AF_UNIX:

        saun1 = (నిర్మాణం sockaddr_un *) sa1;
        saun2 = (నిర్మాణం sockaddr_un *) sa2;

        ఒకవేళ (slen1 < slen2) {
            len = slen1 - offsetof(నిర్మాణం sockaddr_un, sun_మార్గం);

        } లేకపోతే {
            len = slen2 - offsetof(నిర్మాణం sockaddr_un, sun_మార్గం);
        }

        ఒకవేళ (len > పరిమాణం(saun1->sun_మార్గం)) {
            len = పరిమాణం(saun1->sun_మార్గం);
        }

        ఒకవేళ (ngx_memcmp(&saun1->sun_మార్గం, &saun2->sun_మార్గం, len) != 0) {
            తిరిగి NGX_DECLINED;
        }

        ఆపు;
#అయితే_ముగింపు

    అప్రమేయ: /* AF_INET */

        sin1 = (నిర్మాణం sockaddr_లోపల *) sa1;
        sin2 = (నిర్మాణం sockaddr_లోపల *) sa2;

        ఒకవేళ (cmp_పోర్టు && sin1->sin_పోర్టు != sin2->sin_పోర్టు) {
            తిరిగి NGX_DECLINED;
        }

        ఒకవేళ (sin1->sin_addr.s_addr != sin2->sin_addr.s_addr) {
            తిరిగి NGX_DECLINED;
        }

        ఆపు;
    }

    తిరిగి NGX_OK;
}


లోపల_పోర్టు_t
ngx_inet_get_పోర్టు(నిర్మాణం sockaddr *sa)
{
    నిర్మాణం sockaddr_లోపల   *జ్యా;
#ఒకవేళ (NGX_HAVE_INET6)
    నిర్మాణం sockaddr_in6  *sin6;
#అయితే_ముగింపు

    ఎంపిక (sa->sa_family) {

#ఒకవేళ (NGX_HAVE_INET6)
    స్థితి AF_INET6:
        sin6 = (నిర్మాణం sockaddr_in6 *) sa;
        తిరిగి ntohs(sin6->sin6_పోర్టు);
#అయితే_ముగింపు

#ఒకవేళ (NGX_HAVE_UNIX_DOMAIN)
    స్థితి AF_UNIX:
        తిరిగి 0;
#అయితే_ముగింపు

    అప్రమేయ: /* AF_INET */
        జ్యా = (నిర్మాణం sockaddr_లోపల *) sa;
        తిరిగి ntohs(జ్యా->sin_పోర్టు);
    }
}


శూన్య
ngx_inet_సమితి_పోర్టు(నిర్మాణం sockaddr *sa, లోపల_పోర్టు_t పోర్టు)
{
    నిర్మాణం sockaddr_లోపల   *జ్యా;
#ఒకవేళ (NGX_HAVE_INET6)
    నిర్మాణం sockaddr_in6  *sin6;
#అయితే_ముగింపు

    ఎంపిక (sa->sa_family) {

#ఒకవేళ (NGX_HAVE_INET6)
    స్థితి AF_INET6:
        sin6 = (నిర్మాణం sockaddr_in6 *) sa;
        sin6->sin6_పోర్టు = htons(పోర్టు);
        ఆపు;
#అయితే_ముగింపు

#ఒకవేళ (NGX_HAVE_UNIX_DOMAIN)
    స్థితి AF_UNIX:
        ఆపు;
#అయితే_ముగింపు

    అప్రమేయ: /* AF_INET */
        జ్యా = (నిర్మాణం sockaddr_లోపల *) sa;
        జ్యా->sin_పోర్టు = htons(పోర్టు);
        ఆపు;
    }
}


ngx_uint_t
ngx_inet_wildcard(నిర్మాణం sockaddr *sa)
{
    నిర్మాణం sockaddr_లోపల   *జ్యా;
#ఒకవేళ (NGX_HAVE_INET6)
    నిర్మాణం sockaddr_in6  *sin6;
#అయితే_ముగింపు

    ఎంపిక (sa->sa_family) {

    స్థితి AF_INET:
        జ్యా = (నిర్మాణం sockaddr_లోపల *) sa;

        ఒకవేళ (జ్యా->sin_addr.s_addr == INADDR_ANY) {
            తిరిగి 1;
        }

        ఆపు;

#ఒకవేళ (NGX_HAVE_INET6)

    స్థితి AF_INET6:
        sin6 = (నిర్మాణం sockaddr_in6 *) sa;

        ఒకవేళ (IN6_IS_ADDR_UNSPECIFIED(&sin6->sin6_addr)) {
            తిరిగి 1;
        }

        ఆపు;

#అయితే_ముగింపు
    }

    తిరిగి 0;
}
