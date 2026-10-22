
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ଅନ୍ତର୍ଭାଵ <ngx_config.h>
#ଅନ୍ତର୍ଭାଵ <ngx_core.h>


ସ୍ଥିର ngx_int_t ngx_parse_unix_domain_url(ngx_pool_t *pool, ngx_url_t *u);
ସ୍ଥିର ngx_int_t ngx_parse_inet_url(ngx_pool_t *pool, ngx_url_t *u);
ସ୍ଥିର ngx_int_t ngx_parse_inet6_url(ngx_pool_t *pool, ngx_url_t *u);
ସ୍ଥିର ngx_int_t ngx_inet_add_addr(ngx_pool_t *pool, ngx_url_t *u,
    ସଂରଚନା sockaddr *sockaddr, socklen_t socklen, ngx_uint_t total);


in_addr_t
ngx_inet_addr(u_char *text, size_t len)
{
    u_char      *p, c;
    in_addr_t    addr;
    ngx_uint_t   octet, n;

    addr = 0;
    octet = 0;
    n = 0;

    ପାଇଁ (p = text; p < text + len; p++) {
        c = *p;

        ଯଦି (c >= '0' && c <= '9') {
            octet = octet * 10 + (c - '0');

            ଯଦି (octet > 255) {
                ଫେରନ୍ତୁ INADDR_NONE;
            }

            ଜାରି;
        }

        ଯଦି (c == '.') {
            addr = (addr << 8) + octet;
            octet = 0;
            n++;
            ଜାରି;
        }

        ଫେରନ୍ତୁ INADDR_NONE;
    }

    ଯଦି (n == 3) {
        addr = (addr << 8) + octet;
        ଫେରନ୍ତୁ htonl(addr);
    }

    ଫେରନ୍ତୁ INADDR_NONE;
}


#ଯଦି (NGX_HAVE_INET6)

ngx_int_t
ngx_inet6_addr(u_char *p, size_t len, u_char *addr)
{
    u_char      c, *zero, *digit, *s, *d;
    size_t      len4;
    ngx_uint_t  n, nibbles, word;

    ଯଦି (len == 0) {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    zero = ଖାଲି;
    digit = ଖାଲି;
    len4 = 0;
    nibbles = 0;
    word = 0;
    n = 8;

    ଯଦି (p[0] == ':') {
        p++;
        len--;
    }

    ପାଇଁ (/* void */; len; len--) {
        c = *p++;

        ଯଦି (c == ':') {
            ଯଦି (nibbles) {
                digit = p;
                len4 = len;
                *addr++ = (u_char) (word >> 8);
                *addr++ = (u_char) (word & 0xff);

                ଯଦି (--n) {
                    nibbles = 0;
                    word = 0;
                    ଜାରି;
                }

            } ଅନ୍ୟଥା {
                ଯଦି (zero == ଖାଲି) {
                    digit = p;
                    len4 = len;
                    zero = addr;
                    ଜାରି;
                }
            }

            ଫେରନ୍ତୁ NGX_ERROR;
        }

        ଯଦି (c == '.' && nibbles) {
            ଯଦି (n < 2 || digit == ଖାଲି) {
                ଫେରନ୍ତୁ NGX_ERROR;
            }

            word = ngx_inet_addr(digit, len4 - 1);
            ଯଦି (word == INADDR_NONE) {
                ଫେରନ୍ତୁ NGX_ERROR;
            }

            word = ntohl(word);
            *addr++ = (u_char) ((word >> 24) & 0xff);
            *addr++ = (u_char) ((word >> 16) & 0xff);
            n--;
            ଭାଙ୍ଗ;
        }

        ଯଦି (++nibbles > 4) {
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        ଯଦି (c >= '0' && c <= '9') {
            word = word * 16 + (c - '0');
            ଜାରି;
        }

        c |= 0x20;

        ଯଦି (c >= 'a' && c <= 'f') {
            word = word * 16 + (c - 'a') + 10;
            ଜାରି;
        }

        ଫେରନ୍ତୁ NGX_ERROR;
    }

    ଯଦି (nibbles == 0 && zero == ଖାଲି) {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    *addr++ = (u_char) (word >> 8);
    *addr++ = (u_char) (word & 0xff);

    ଯଦି (--n) {
        ଯଦି (zero) {
            n *= 2;
            s = addr - 1;
            d = s + n;
            ଯେତେବେଳେ (s >= zero) {
                *d-- = *s--;
            }
            ngx_memzero(zero, n);
            ଫେରନ୍ତୁ NGX_OK;
        }

    } ଅନ୍ୟଥା {
        ଯଦି (zero == ଖାଲି) {
            ଫେରନ୍ତୁ NGX_OK;
        }
    }

    ଫେରନ୍ତୁ NGX_ERROR;
}

#endif


size_t
ngx_sock_ntop(ସଂରଚନା sockaddr *sa, socklen_t socklen, u_char *text, size_t len,
    ngx_uint_t port)
{
    u_char               *p;
#ଯଦି (NGX_HAVE_INET6 || NGX_HAVE_UNIX_DOMAIN)
    size_t                n;
#endif
    ସଂରଚନା sockaddr_in   *sin;
#ଯଦି (NGX_HAVE_INET6)
    ସଂରଚନା sockaddr_in6  *sin6;
#endif
#ଯଦି (NGX_HAVE_UNIX_DOMAIN)
    ସଂରଚନା sockaddr_un   *saun;
#endif

    ବାଛ (sa->sa_family) {

    ସ୍ଥିତି AF_INET:

        sin = (ସଂରଚନା sockaddr_in *) sa;
        p = (u_char *) &sin->sin_addr;

        ଯଦି (port) {
            p = ngx_snprintf(text, len, "%ud.%ud.%ud.%ud:%d",
                             p[0], p[1], p[2], p[3], ntohs(sin->sin_port));
        } ଅନ୍ୟଥା {
            p = ngx_snprintf(text, len, "%ud.%ud.%ud.%ud",
                             p[0], p[1], p[2], p[3]);
        }

        ଫେରନ୍ତୁ (p - text);

#ଯଦି (NGX_HAVE_INET6)

    ସ୍ଥିତି AF_INET6:

        sin6 = (ସଂରଚନା sockaddr_in6 *) sa;

        n = 0;

        ଯଦି (port) {
            text[n++] = '[';
        }

        n = ngx_inet6_ntop(sin6->sin6_addr.s6_addr, &text[n], len);

        ଯଦି (port) {
            n = ngx_sprintf(&text[1 + n], "]:%d",
                            ntohs(sin6->sin6_port)) - text;
        }

        ଫେରନ୍ତୁ n;
#endif

#ଯଦି (NGX_HAVE_UNIX_DOMAIN)

    ସ୍ଥିତି AF_UNIX:
        saun = (ସଂରଚନା sockaddr_un *) sa;

        /* on Linux sockaddr might not include sun_path at all */

        ଯଦି (socklen <= (socklen_t) offsetof(ସଂରଚନା sockaddr_un, sun_path)) {
            p = ngx_snprintf(text, len, "unix:%Z");

        } ଅନ୍ୟଥା {
            n = ngx_strnlen((u_char *) saun->sun_path,
                            socklen - offsetof(ସଂରଚନା sockaddr_un, sun_path));
            p = ngx_snprintf(text, len, "unix:%*s%Z", n, saun->sun_path);
        }

        /* we do not include trailing zero in address length */

        ଫେରନ୍ତୁ (p - text - 1);

#endif

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        ଫେରନ୍ତୁ 0;
    }
}


size_t
ngx_inet_ntop(ପୂର୍ଣ୍ଣସଂଖ୍ୟା family, ଶୂନ୍ୟ *addr, u_char *text, size_t len)
{
    u_char  *p;

    ବାଛ (family) {

    ସ୍ଥିତି AF_INET:

        p = addr;

        ଫେରନ୍ତୁ ngx_snprintf(text, len, "%ud.%ud.%ud.%ud",
                            p[0], p[1], p[2], p[3])
               - text;

#ଯଦି (NGX_HAVE_INET6)

    ସ୍ଥିତି AF_INET6:
        ଫେରନ୍ତୁ ngx_inet6_ntop(addr, text, len);

#endif

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        ଫେରନ୍ତୁ 0;
    }
}


#ଯଦି (NGX_HAVE_INET6)

size_t
ngx_inet6_ntop(u_char *p, u_char *text, size_t len)
{
    u_char      *dst;
    size_t       max, n;
    ngx_uint_t   i, zero, last;

    ଯଦି (len < NGX_INET6_ADDRSTRLEN) {
        ଫେରନ୍ତୁ 0;
    }

    zero = (ngx_uint_t) -1;
    last = (ngx_uint_t) -1;
    max = 1;
    n = 0;

    ପାଇଁ (i = 0; i < 16; i += 2) {

        ଯଦି (p[i] || p[i + 1]) {

            ଯଦି (max < n) {
                zero = last;
                max = n;
            }

            n = 0;
            ଜାରି;
        }

        ଯଦି (n++ == 0) {
            last = i;
        }
    }

    ଯଦି (max < n) {
        zero = last;
        max = n;
    }

    dst = text;
    n = 16;

    ଯଦି (zero == 0) {

        ଯଦି ((max == 5 && p[10] == 0xff && p[11] == 0xff)
            || (max == 6)
            || (max == 7 && p[14] != 0 && p[15] != 1))
        {
            n = 12;
        }

        *dst++ = ':';
    }

    ପାଇଁ (i = 0; i < n; i += 2) {

        ଯଦି (i == zero) {
            *dst++ = ':';
            i += (max - 1) * 2;
            ଜାରି;
        }

        dst = ngx_sprintf(dst, "%xd", p[i] * 256 + p[i + 1]);

        ଯଦି (i < 14) {
            *dst++ = ':';
        }
    }

    ଯଦି (n == 12) {
        dst = ngx_sprintf(dst, "%ud.%ud.%ud.%ud", p[12], p[13], p[14], p[15]);
    }

    ଫେରନ୍ତୁ dst - text;
}

#endif


ngx_int_t
ngx_ptocidr(ngx_str_t *text, ngx_cidr_t *cidr)
{
    u_char      *addr, *mask, *last;
    size_t       len;
    ngx_int_t    shift;
#ଯଦି (NGX_HAVE_INET6)
    ngx_int_t    rc;
    ngx_uint_t   s, i;
#endif

    addr = text->data;
    last = addr + text->len;

    mask = ngx_strlchr(addr, last, '/');
    len = (mask ? mask : last) - addr;

    cidr->u.in.addr = ngx_inet_addr(addr, len);

    ଯଦି (cidr->u.in.addr != INADDR_NONE) {
        cidr->family = AF_INET;

        ଯଦି (mask == ଖାଲି) {
            cidr->u.in.mask = 0xffffffff;
            ଫେରନ୍ତୁ NGX_OK;
        }

#ଯଦି (NGX_HAVE_INET6)
    } ଅନ୍ୟଥା ଯଦି (ngx_inet6_addr(addr, len, cidr->u.in6.addr.s6_addr) == NGX_OK) {
        cidr->family = AF_INET6;

        ଯଦି (mask == ଖାଲି) {
            ngx_memset(cidr->u.in6.mask.s6_addr, 0xff, 16);
            ଫେରନ୍ତୁ NGX_OK;
        }

#endif
    } ଅନ୍ୟଥା {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    mask++;

    shift = ngx_atoi(mask, last - mask);
    ଯଦି (shift == NGX_ERROR) {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    ବାଛ (cidr->family) {

#ଯଦି (NGX_HAVE_INET6)
    ସ୍ଥିତି AF_INET6:
        ଯଦି (shift > 128) {
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        addr = cidr->u.in6.addr.s6_addr;
        mask = cidr->u.in6.mask.s6_addr;
        rc = NGX_OK;

        ପାଇଁ (i = 0; i < 16; i++) {

            s = (shift > 8) ? 8 : shift;
            shift -= s;

            mask[i] = (u_char) (0xffu << (8 - s));

            ଯଦି (addr[i] != (addr[i] & mask[i])) {
                rc = NGX_DONE;
                addr[i] &= mask[i];
            }
        }

        ଫେରନ୍ତୁ rc;
#endif

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ: /* AF_INET */
        ଯଦି (shift > 32) {
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        ଯଦି (shift) {
            cidr->u.in.mask = htonl((uint32_t) (0xffffffffu << (32 - shift)));

        } ଅନ୍ୟଥା {
            /* x86 compilers use a shl instruction that shifts by modulo 32 */
            cidr->u.in.mask = 0;
        }

        ଯଦି (cidr->u.in.addr == (cidr->u.in.addr & cidr->u.in.mask)) {
            ଫେରନ୍ତୁ NGX_OK;
        }

        cidr->u.in.addr &= cidr->u.in.mask;

        ଫେରନ୍ତୁ NGX_DONE;
    }
}


ngx_int_t
ngx_cidr_match(ସଂରଚନା sockaddr *sa, ngx_array_t *cidrs)
{
#ଯଦି (NGX_HAVE_INET6)
    u_char           *p;
#endif
    in_addr_t         inaddr;
    ngx_cidr_t       *cidr;
    ngx_uint_t        family, i;
#ଯଦି (NGX_HAVE_INET6)
    ngx_uint_t        n;
    ସଂରଚନା in6_addr  *inaddr6;
#endif

#ଯଦି (NGX_SUPPRESS_WARN)
    inaddr = 0;
#ଯଦି (NGX_HAVE_INET6)
    inaddr6 = ଖାଲି;
#endif
#endif

    family = sa->sa_family;

    ଯଦି (family == AF_INET) {
        inaddr = ((ସଂରଚନା sockaddr_in *) sa)->sin_addr.s_addr;
    }

#ଯଦି (NGX_HAVE_INET6)
    ଅନ୍ୟଥା ଯଦି (family == AF_INET6) {
        inaddr6 = &((ସଂରଚନା sockaddr_in6 *) sa)->sin6_addr;

        ଯଦି (IN6_IS_ADDR_V4MAPPED(inaddr6)) {
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

    ପାଇଁ (cidr = cidrs->elts, i = 0; i < cidrs->nelts; i++) {
        ଯଦି (cidr[i].family != family) {
            ଯାଅ next;
        }

        ବାଛ (family) {

#ଯଦି (NGX_HAVE_INET6)
        ସ୍ଥିତି AF_INET6:
            ପାଇଁ (n = 0; n < 16; n++) {
                ଯଦି ((inaddr6->s6_addr[n] & cidr[i].u.in6.mask.s6_addr[n])
                    != cidr[i].u.in6.addr.s6_addr[n])
                {
                    ଯାଅ next;
                }
            }
            ଭାଙ୍ଗ;
#endif

#ଯଦି (NGX_HAVE_UNIX_DOMAIN)
        ସ୍ଥିତି AF_UNIX:
            ଭାଙ୍ଗ;
#endif

        ପୂର୍ବନିର୍ଦ୍ଧାରିତ: /* AF_INET */
            ଯଦି ((inaddr & cidr[i].u.in.mask) != cidr[i].u.in.addr) {
                ଯାଅ next;
            }
            ଭାଙ୍ଗ;
        }

        ଫେରନ୍ତୁ NGX_OK;

    next:
        ଜାରି;
    }

    ଫେରନ୍ତୁ NGX_DECLINED;
}


ngx_int_t
ngx_parse_addr(ngx_pool_t *pool, ngx_addr_t *addr, u_char *text, size_t len)
{
    in_addr_t             inaddr;
    ngx_uint_t            family;
    ସଂରଚନା sockaddr_in   *sin;
#ଯଦି (NGX_HAVE_INET6)
    ସଂରଚନା in6_addr       inaddr6;
    ସଂରଚନା sockaddr_in6  *sin6;

    /*
     * prevent MSVC8 warning:
     *    potentially uninitialized local variable 'inaddr6' used
     */
    ngx_memzero(&inaddr6, ଆକାର(ସଂରଚନା in6_addr));
#endif

    inaddr = ngx_inet_addr(text, len);

    ଯଦି (inaddr != INADDR_NONE) {
        family = AF_INET;
        len = ଆକାର(ସଂରଚନା sockaddr_in);

#ଯଦି (NGX_HAVE_INET6)
    } ଅନ୍ୟଥା ଯଦି (ngx_inet6_addr(text, len, inaddr6.s6_addr) == NGX_OK) {
        family = AF_INET6;
        len = ଆକାର(ସଂରଚନା sockaddr_in6);

#endif
    } ଅନ୍ୟଥା {
        ଫେରନ୍ତୁ NGX_DECLINED;
    }

    addr->sockaddr = ngx_pcalloc(pool, len);
    ଯଦି (addr->sockaddr == ଖାଲି) {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    addr->sockaddr->sa_family = (u_char) family;
    addr->socklen = len;

    ବାଛ (family) {

#ଯଦି (NGX_HAVE_INET6)
    ସ୍ଥିତି AF_INET6:
        sin6 = (ସଂରଚନା sockaddr_in6 *) addr->sockaddr;
        ngx_memcpy(sin6->sin6_addr.s6_addr, inaddr6.s6_addr, 16);
        ଭାଙ୍ଗ;
#endif

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ: /* AF_INET */
        sin = (ସଂରଚନା sockaddr_in *) addr->sockaddr;
        sin->sin_addr.s_addr = inaddr;
        ଭାଙ୍ଗ;
    }

    ଫେରନ୍ତୁ NGX_OK;
}


ngx_int_t
ngx_parse_addr_port(ngx_pool_t *pool, ngx_addr_t *addr, u_char *text,
    size_t len)
{
    u_char     *p, *last;
    size_t      plen;
    ngx_int_t   rc, port;

    rc = ngx_parse_addr(pool, addr, text, len);

    ଯଦି (rc != NGX_DECLINED) {
        ଫେରନ୍ତୁ rc;
    }

    last = text + len;

#ଯଦି (NGX_HAVE_INET6)
    ଯଦି (len && text[0] == '[') {

        p = ngx_strlchr(text, last, ']');

        ଯଦି (p == last - 1) {
            ଫେରନ୍ତୁ ngx_parse_addr(pool, addr, text + 1, len - 2);
        }

        ଯଦି (p == ଖାଲି || *++p != ':') {
            ଫେରନ୍ତୁ NGX_DECLINED;
        }

        text++;
        len -= 2;

    } ଅନ୍ୟଥା
#endif

    {
        p = ngx_strlchr(text, last, ':');

        ଯଦି (p == ଖାଲି) {
            ଫେରନ୍ତୁ NGX_DECLINED;
        }
    }

    p++;
    plen = last - p;

    port = ngx_atoi(p, plen);

    ଯଦି (port < 1 || port > 65535) {
        ଫେରନ୍ତୁ NGX_DECLINED;
    }

    len -= plen + 1;

    rc = ngx_parse_addr(pool, addr, text, len);

    ଯଦି (rc != NGX_OK) {
        ଫେରନ୍ତୁ rc;
    }

    ngx_inet_set_port(addr->sockaddr, (in_port_t) port);

    ଫେରନ୍ତୁ NGX_OK;
}


ngx_int_t
ngx_parse_url(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char  *p;
    size_t   len;

    p = u->url.data;
    len = u->url.len;

    ଯଦି (len >= 5 && ngx_strncasecmp(p, (u_char *) "unix:", 5) == 0) {
        ଫେରନ୍ତୁ ngx_parse_unix_domain_url(pool, u);
    }

    ଯଦି (len && p[0] == '[') {
        ଫେରନ୍ତୁ ngx_parse_inet6_url(pool, u);
    }

    ଫେରନ୍ତୁ ngx_parse_inet_url(pool, u);
}


ସ୍ଥିର ngx_int_t
ngx_parse_unix_domain_url(ngx_pool_t *pool, ngx_url_t *u)
{
#ଯଦି (NGX_HAVE_UNIX_DOMAIN)
    u_char              *path, *uri, *last;
    size_t               len;
    ସଂରଚନା sockaddr_un  *saun;

    len = u->url.len;
    path = u->url.data;

    path += 5;
    len -= 5;

    ଯଦି (u->uri_part) {

        last = path + len;
        uri = ngx_strlchr(path, last, ':');

        ଯଦି (uri) {
            len = uri - path;
            uri++;
            u->uri.len = last - uri;
            u->uri.data = uri;
        }
    }

    ଯଦି (len == 0) {
        u->err = "no path in the unix domain socket";
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    u->host.len = len++;
    u->host.data = path;

    ଯଦି (len > ଆକାର(saun->sun_path)) {
        u->err = "too long path in the unix domain socket";
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    u->socklen = ଆକାର(ସଂରଚନା sockaddr_un);
    saun = (ସଂରଚନା sockaddr_un *) &u->sockaddr;
    saun->sun_family = AF_UNIX;
    (ଶୂନ୍ୟ) ngx_cpystrn((u_char *) saun->sun_path, path, len);

    u->addrs = ngx_pcalloc(pool, ଆକାର(ngx_addr_t));
    ଯଦି (u->addrs == ଖାଲି) {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    saun = ngx_pcalloc(pool, ଆକାର(ସଂରଚନା sockaddr_un));
    ଯଦି (saun == ଖାଲି) {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    u->family = AF_UNIX;
    u->naddrs = 1;

    saun->sun_family = AF_UNIX;
    (ଶୂନ୍ୟ) ngx_cpystrn((u_char *) saun->sun_path, path, len);

    u->addrs[0].sockaddr = (ସଂରଚନା sockaddr *) saun;
    u->addrs[0].socklen = ଆକାର(ସଂରଚନା sockaddr_un);
    u->addrs[0].name.len = len + 4;
    u->addrs[0].name.data = u->url.data;

    ଫେରନ୍ତୁ NGX_OK;

#ଅନ୍ୟଥା

    u->err = "the unix domain sockets are not supported on this platform";

    ଫେରନ୍ତୁ NGX_ERROR;

#endif
}


ସ୍ଥିର ngx_int_t
ngx_parse_inet_url(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char              *host, *port, *last, *uri, *args, *dash;
    size_t               len;
    ngx_int_t            n;
    ସଂରଚନା sockaddr_in  *sin;

    u->socklen = ଆକାର(ସଂରଚନା sockaddr_in);
    sin = (ସଂରଚନା sockaddr_in *) &u->sockaddr;
    sin->sin_family = AF_INET;

    u->family = AF_INET;

    host = u->url.data;

    last = host + u->url.len;

    port = ngx_strlchr(host, last, ':');

    uri = ngx_strlchr(host, last, '/');

    args = ngx_strlchr(host, last, '?');

    ଯଦି (args) {
        ଯଦି (uri == ଖାଲି || args < uri) {
            uri = args;
        }
    }

    ଯଦି (uri) {
        ଯଦି (u->listen || !u->uri_part) {
            u->err = "invalid host";
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        u->uri.len = last - uri;
        u->uri.data = uri;

        last = uri;

        ଯଦି (uri < port) {
            port = ଖାଲି;
        }
    }

    ଯଦି (port) {
        port++;

        len = last - port;

        ଯଦି (u->listen) {
            dash = ngx_strlchr(port, last, '-');

            ଯଦି (dash) {
                dash++;

                n = ngx_atoi(dash, last - dash);

                ଯଦି (n < 1 || n > 65535) {
                    u->err = "invalid port";
                    ଫେରନ୍ତୁ NGX_ERROR;
                }

                u->last_port = (in_port_t) n;

                len = dash - port - 1;
            }
        }

        n = ngx_atoi(port, len);

        ଯଦି (n < 1 || n > 65535) {
            u->err = "invalid port";
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        ଯଦି (u->last_port && n > u->last_port) {
            u->err = "invalid port range";
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        u->port = (in_port_t) n;
        sin->sin_port = htons((in_port_t) n);

        u->port_text.len = last - port;
        u->port_text.data = port;

        last = port - 1;

    } ଅନ୍ୟଥା {
        ଯଦି (uri == ଖାଲି) {

            ଯଦି (u->listen) {

                /* test value as port only */

                len = last - host;

                dash = ngx_strlchr(host, last, '-');

                ଯଦି (dash) {
                    dash++;

                    n = ngx_atoi(dash, last - dash);

                    ଯଦି (n == NGX_ERROR) {
                        ଯାଅ no_port;
                    }

                    ଯଦି (n < 1 || n > 65535) {
                        u->err = "invalid port";

                    } ଅନ୍ୟଥା {
                        u->last_port = (in_port_t) n;
                    }

                    len = dash - host - 1;
                }

                n = ngx_atoi(host, len);

                ଯଦି (n != NGX_ERROR) {

                    ଯଦି (u->err) {
                        ଫେରନ୍ତୁ NGX_ERROR;
                    }

                    ଯଦି (n < 1 || n > 65535) {
                        u->err = "invalid port";
                        ଫେରନ୍ତୁ NGX_ERROR;
                    }

                    ଯଦି (u->last_port && n > u->last_port) {
                        u->err = "invalid port range";
                        ଫେରନ୍ତୁ NGX_ERROR;
                    }

                    u->port = (in_port_t) n;
                    sin->sin_port = htons((in_port_t) n);
                    sin->sin_addr.s_addr = INADDR_ANY;

                    u->port_text.len = last - host;
                    u->port_text.data = host;

                    u->wildcard = 1;

                    ଫେରନ୍ତୁ ngx_inet_add_addr(pool, u, &u->sockaddr.sockaddr,
                                             u->socklen, 1);
                }
            }
        }

no_port:

        u->err = ଖାଲି;
        u->no_port = 1;
        u->port = u->default_port;
        sin->sin_port = htons(u->default_port);
        u->last_port = 0;
    }

    len = last - host;

    ଯଦି (len == 0) {
        u->err = "no host";
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    u->host.len = len;
    u->host.data = host;

    ଯଦି (u->listen && len == 1 && *host == '*') {
        sin->sin_addr.s_addr = INADDR_ANY;
        u->wildcard = 1;
        ଫେରନ୍ତୁ ngx_inet_add_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);
    }

    sin->sin_addr.s_addr = ngx_inet_addr(host, len);

    ଯଦି (sin->sin_addr.s_addr != INADDR_NONE) {

        ଯଦି (sin->sin_addr.s_addr == INADDR_ANY) {
            u->wildcard = 1;
        }

        ଫେରନ୍ତୁ ngx_inet_add_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);
    }

    ଯଦି (u->no_resolve) {
        ଫେରନ୍ତୁ NGX_OK;
    }

    ଯଦି (ngx_inet_resolve_host(pool, u) != NGX_OK) {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    u->family = u->addrs[0].sockaddr->sa_family;
    u->socklen = u->addrs[0].socklen;
    ngx_memcpy(&u->sockaddr, u->addrs[0].sockaddr, u->addrs[0].socklen);
    u->wildcard = ngx_inet_wildcard(&u->sockaddr.sockaddr);

    ଫେରନ୍ତୁ NGX_OK;
}


ସ୍ଥିର ngx_int_t
ngx_parse_inet6_url(ngx_pool_t *pool, ngx_url_t *u)
{
#ଯଦି (NGX_HAVE_INET6)
    u_char               *p, *host, *port, *last, *uri, *dash;
    size_t                len;
    ngx_int_t             n;
    ସଂରଚନା sockaddr_in6  *sin6;

    u->socklen = ଆକାର(ସଂରଚନା sockaddr_in6);
    sin6 = (ସଂରଚନା sockaddr_in6 *) &u->sockaddr;
    sin6->sin6_family = AF_INET6;

    host = u->url.data + 1;

    last = u->url.data + u->url.len;

    p = ngx_strlchr(host, last, ']');

    ଯଦି (p == ଖାଲି) {
        u->err = "invalid host";
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    port = p + 1;

    uri = ngx_strlchr(port, last, '/');

    ଯଦି (uri) {
        ଯଦି (u->listen || !u->uri_part) {
            u->err = "invalid host";
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        u->uri.len = last - uri;
        u->uri.data = uri;

        last = uri;
    }

    ଯଦି (port < last) {
        ଯଦି (*port != ':') {
            u->err = "invalid host";
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        port++;

        len = last - port;

        ଯଦି (u->listen) {
            dash = ngx_strlchr(port, last, '-');

            ଯଦି (dash) {
                dash++;

                n = ngx_atoi(dash, last - dash);

                ଯଦି (n < 1 || n > 65535) {
                    u->err = "invalid port";
                    ଫେରନ୍ତୁ NGX_ERROR;
                }

                u->last_port = (in_port_t) n;

                len = dash - port - 1;
            }
        }

        n = ngx_atoi(port, len);

        ଯଦି (n < 1 || n > 65535) {
            u->err = "invalid port";
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        ଯଦି (u->last_port && n > u->last_port) {
            u->err = "invalid port range";
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        u->port = (in_port_t) n;
        sin6->sin6_port = htons((in_port_t) n);

        u->port_text.len = last - port;
        u->port_text.data = port;

    } ଅନ୍ୟଥା {
        u->no_port = 1;
        u->port = u->default_port;
        sin6->sin6_port = htons(u->default_port);
    }

    len = p - host;

    ଯଦି (len == 0) {
        u->err = "no host";
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    u->host.len = len + 2;
    u->host.data = host - 1;

    ଯଦି (ngx_inet6_addr(host, len, sin6->sin6_addr.s6_addr) != NGX_OK) {
        u->err = "invalid IPv6 address";
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    ଯଦି (IN6_IS_ADDR_UNSPECIFIED(&sin6->sin6_addr)) {
        u->wildcard = 1;
    }

    u->family = AF_INET6;

    ଫେରନ୍ତୁ ngx_inet_add_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);

#ଅନ୍ୟଥା

    u->err = "the INET6 sockets are not supported on this platform";

    ଫେରନ୍ତୁ NGX_ERROR;

#endif
}


#ଯଦି (NGX_HAVE_GETADDRINFO && NGX_HAVE_INET6)

ngx_int_t
ngx_inet_resolve_host(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char           *host;
    ngx_uint_t        n;
    ସଂରଚନା addrinfo   hints, *res, *rp;

    host = ngx_alloc(u->host.len + 1, pool->log);
    ଯଦି (host == ଖାଲି) {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    (ଶୂନ୍ୟ) ngx_cpystrn(host, u->host.data, u->host.len + 1);

    ngx_memzero(&hints, ଆକାର(ସଂରଚନା addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
#ifdef AI_ADDRCONFIG
    hints.ai_flags = AI_ADDRCONFIG;
#endif

    ଯଦି (getaddrinfo((ଅକ୍ଷର *) host, ଖାଲି, &hints, &res) != 0) {
        u->err = "host not found";
        ngx_free(host);
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    ngx_free(host);

    ପାଇଁ (n = 0, rp = res; rp != ଖାଲି; rp = rp->ai_next) {

        ବାଛ (rp->ai_family) {

        ସ୍ଥିତି AF_INET:
        ସ୍ଥିତି AF_INET6:
            ଭାଙ୍ଗ;

        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
            ଜାରି;
        }

        n++;
    }

    ଯଦି (n == 0) {
        u->err = "host not found";
        ଯାଅ failed;
    }

    /* MP: ngx_shared_palloc() */

    ପାଇଁ (rp = res; rp != ଖାଲି; rp = rp->ai_next) {

        ବାଛ (rp->ai_family) {

        ସ୍ଥିତି AF_INET:
        ସ୍ଥିତି AF_INET6:
            ଭାଙ୍ଗ;

        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
            ଜାରି;
        }

        ଯଦି (ngx_inet_add_addr(pool, u, rp->ai_addr, rp->ai_addrlen, n)
            != NGX_OK)
        {
            ଯାଅ failed;
        }
    }

    freeaddrinfo(res);
    ଫେରନ୍ତୁ NGX_OK;

failed:

    freeaddrinfo(res);
    ଫେରନ୍ତୁ NGX_ERROR;
}

#ଅନ୍ୟଥା /* !NGX_HAVE_GETADDRINFO || !NGX_HAVE_INET6 */

ngx_int_t
ngx_inet_resolve_host(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char              *host;
    ngx_uint_t           i, n;
    ସଂରଚନା hostent      *h;
    ସଂରଚନା sockaddr_in   sin;

    /* AF_INET only */

    ngx_memzero(&sin, ଆକାର(ସଂରଚନା sockaddr_in));

    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = ngx_inet_addr(u->host.data, u->host.len);

    ଯଦି (sin.sin_addr.s_addr == INADDR_NONE) {
        host = ngx_alloc(u->host.len + 1, pool->log);
        ଯଦି (host == ଖାଲି) {
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        (ଶୂନ୍ୟ) ngx_cpystrn(host, u->host.data, u->host.len + 1);

        h = gethostbyname((ଅକ୍ଷର *) host);

        ngx_free(host);

        ଯଦି (h == ଖାଲି || h->h_addr_list[0] == ଖାଲି) {
            u->err = "host not found";
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        ପାଇଁ (n = 0; h->h_addr_list[n] != ଖାଲି; n++) { /* void */ }

        /* MP: ngx_shared_palloc() */

        ପାଇଁ (i = 0; i < n; i++) {
            sin.sin_addr.s_addr = *(in_addr_t *) (h->h_addr_list[i]);

            ଯଦି (ngx_inet_add_addr(pool, u, (ସଂରଚନା sockaddr *) &sin,
                                  ଆକାର(ସଂରଚନା sockaddr_in), n)
                != NGX_OK)
            {
                ଫେରନ୍ତୁ NGX_ERROR;
            }
        }

    } ଅନ୍ୟଥା {

        /* MP: ngx_shared_palloc() */

        ଯଦି (ngx_inet_add_addr(pool, u, (ସଂରଚନା sockaddr *) &sin,
                              ଆକାର(ସଂରଚନା sockaddr_in), 1)
            != NGX_OK)
        {
            ଫେରନ୍ତୁ NGX_ERROR;
        }
    }

    ଫେରନ୍ତୁ NGX_OK;
}

#endif /* NGX_HAVE_GETADDRINFO && NGX_HAVE_INET6 */


ସ୍ଥିର ngx_int_t
ngx_inet_add_addr(ngx_pool_t *pool, ngx_url_t *u, ସଂରଚନା sockaddr *sockaddr,
    socklen_t socklen, ngx_uint_t total)
{
    u_char           *p;
    size_t            len;
    ngx_uint_t        i, nports;
    ngx_addr_t       *addr;
    ସଂରଚନା sockaddr  *sa;

    nports = u->last_port ? u->last_port - u->port + 1 : 1;

    ଯଦି (u->addrs == ଖାଲି) {
        u->addrs = ngx_palloc(pool, total * nports * ଆକାର(ngx_addr_t));
        ଯଦି (u->addrs == ଖାଲି) {
            ଫେରନ୍ତୁ NGX_ERROR;
        }
    }

    ପାଇଁ (i = 0; i < nports; i++) {
        sa = ngx_pcalloc(pool, socklen);
        ଯଦି (sa == ଖାଲି) {
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        ngx_memcpy(sa, sockaddr, socklen);

        ngx_inet_set_port(sa, u->port + i);

        ବାଛ (sa->sa_family) {

#ଯଦି (NGX_HAVE_INET6)
        ସ୍ଥିତି AF_INET6:
            len = NGX_INET6_ADDRSTRLEN + ଆକାର("[]:65536") - 1;
            ଭାଙ୍ଗ;
#endif

        ପୂର୍ବନିର୍ଦ୍ଧାରିତ: /* AF_INET */
            len = NGX_INET_ADDRSTRLEN + ଆକାର(":65535") - 1;
        }

        p = ngx_pnalloc(pool, len);
        ଯଦି (p == ଖାଲି) {
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        len = ngx_sock_ntop(sa, socklen, p, len, 1);

        addr = &u->addrs[u->naddrs++];

        addr->sockaddr = sa;
        addr->socklen = socklen;

        addr->name.len = len;
        addr->name.data = p;
    }

    ଫେରନ୍ତୁ NGX_OK;
}


ngx_int_t
ngx_cmp_sockaddr(ସଂରଚନା sockaddr *sa1, socklen_t slen1,
    ସଂରଚନା sockaddr *sa2, socklen_t slen2, ngx_uint_t cmp_port)
{
    ସଂରଚନା sockaddr_in   *sin1, *sin2;
#ଯଦି (NGX_HAVE_INET6)
    ସଂରଚନା sockaddr_in6  *sin61, *sin62;
#endif
#ଯଦି (NGX_HAVE_UNIX_DOMAIN)
    size_t                len;
    ସଂରଚନା sockaddr_un   *saun1, *saun2;
#endif

    ଯଦି (sa1->sa_family != sa2->sa_family) {
        ଫେରନ୍ତୁ NGX_DECLINED;
    }

    ବାଛ (sa1->sa_family) {

#ଯଦି (NGX_HAVE_INET6)
    ସ୍ଥିତି AF_INET6:

        sin61 = (ସଂରଚନା sockaddr_in6 *) sa1;
        sin62 = (ସଂରଚନା sockaddr_in6 *) sa2;

        ଯଦି (cmp_port && sin61->sin6_port != sin62->sin6_port) {
            ଫେରନ୍ତୁ NGX_DECLINED;
        }

        ଯଦି (ngx_memcmp(&sin61->sin6_addr, &sin62->sin6_addr, 16) != 0) {
            ଫେରନ୍ତୁ NGX_DECLINED;
        }

        ଭାଙ୍ଗ;
#endif

#ଯଦି (NGX_HAVE_UNIX_DOMAIN)
    ସ୍ଥିତି AF_UNIX:

        saun1 = (ସଂରଚନା sockaddr_un *) sa1;
        saun2 = (ସଂରଚନା sockaddr_un *) sa2;

        ଯଦି (slen1 < slen2) {
            len = slen1 - offsetof(ସଂରଚନା sockaddr_un, sun_path);

        } ଅନ୍ୟଥା {
            len = slen2 - offsetof(ସଂରଚନା sockaddr_un, sun_path);
        }

        ଯଦି (len > ଆକାର(saun1->sun_path)) {
            len = ଆକାର(saun1->sun_path);
        }

        ଯଦି (ngx_memcmp(&saun1->sun_path, &saun2->sun_path, len) != 0) {
            ଫେରନ୍ତୁ NGX_DECLINED;
        }

        ଭାଙ୍ଗ;
#endif

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ: /* AF_INET */

        sin1 = (ସଂରଚନା sockaddr_in *) sa1;
        sin2 = (ସଂରଚନା sockaddr_in *) sa2;

        ଯଦି (cmp_port && sin1->sin_port != sin2->sin_port) {
            ଫେରନ୍ତୁ NGX_DECLINED;
        }

        ଯଦି (sin1->sin_addr.s_addr != sin2->sin_addr.s_addr) {
            ଫେରନ୍ତୁ NGX_DECLINED;
        }

        ଭାଙ୍ଗ;
    }

    ଫେରନ୍ତୁ NGX_OK;
}


in_port_t
ngx_inet_get_port(ସଂରଚନା sockaddr *sa)
{
    ସଂରଚନା sockaddr_in   *sin;
#ଯଦି (NGX_HAVE_INET6)
    ସଂରଚନା sockaddr_in6  *sin6;
#endif

    ବାଛ (sa->sa_family) {

#ଯଦି (NGX_HAVE_INET6)
    ସ୍ଥିତି AF_INET6:
        sin6 = (ସଂରଚନା sockaddr_in6 *) sa;
        ଫେରନ୍ତୁ ntohs(sin6->sin6_port);
#endif

#ଯଦି (NGX_HAVE_UNIX_DOMAIN)
    ସ୍ଥିତି AF_UNIX:
        ଫେରନ୍ତୁ 0;
#endif

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ: /* AF_INET */
        sin = (ସଂରଚନା sockaddr_in *) sa;
        ଫେରନ୍ତୁ ntohs(sin->sin_port);
    }
}


ଶୂନ୍ୟ
ngx_inet_set_port(ସଂରଚନା sockaddr *sa, in_port_t port)
{
    ସଂରଚନା sockaddr_in   *sin;
#ଯଦି (NGX_HAVE_INET6)
    ସଂରଚନା sockaddr_in6  *sin6;
#endif

    ବାଛ (sa->sa_family) {

#ଯଦି (NGX_HAVE_INET6)
    ସ୍ଥିତି AF_INET6:
        sin6 = (ସଂରଚନା sockaddr_in6 *) sa;
        sin6->sin6_port = htons(port);
        ଭାଙ୍ଗ;
#endif

#ଯଦି (NGX_HAVE_UNIX_DOMAIN)
    ସ୍ଥିତି AF_UNIX:
        ଭାଙ୍ଗ;
#endif

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ: /* AF_INET */
        sin = (ସଂରଚନା sockaddr_in *) sa;
        sin->sin_port = htons(port);
        ଭାଙ୍ଗ;
    }
}


ngx_uint_t
ngx_inet_wildcard(ସଂରଚନା sockaddr *sa)
{
    ସଂରଚନା sockaddr_in   *sin;
#ଯଦି (NGX_HAVE_INET6)
    ସଂରଚନା sockaddr_in6  *sin6;
#endif

    ବାଛ (sa->sa_family) {

    ସ୍ଥିତି AF_INET:
        sin = (ସଂରଚନା sockaddr_in *) sa;

        ଯଦି (sin->sin_addr.s_addr == INADDR_ANY) {
            ଫେରନ୍ତୁ 1;
        }

        ଭାଙ୍ଗ;

#ଯଦି (NGX_HAVE_INET6)

    ସ୍ଥିତି AF_INET6:
        sin6 = (ସଂରଚନା sockaddr_in6 *) sa;

        ଯଦି (IN6_IS_ADDR_UNSPECIFIED(&sin6->sin6_addr)) {
            ଫେରନ୍ତୁ 1;
        }

        ଭାଙ୍ଗ;

#endif
    }

    ଫେରନ୍ତୁ 0;
}
