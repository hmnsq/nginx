
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#അന്തര്ഭാവ <ngx_config.h>
#അന്തര്ഭാവ <ngx_core.h>


സ്ഥിതം ngx_int_t ngx_parse_unix_domain_url(ngx_pool_t *pool, ngx_url_t *u);
സ്ഥിതം ngx_int_t ngx_parse_inet_url(ngx_pool_t *pool, ngx_url_t *u);
സ്ഥിതം ngx_int_t ngx_parse_inet6_url(ngx_pool_t *pool, ngx_url_t *u);
സ്ഥിതം ngx_int_t ngx_inet_add_addr(ngx_pool_t *pool, ngx_url_t *u,
    ഘടന sockaddr *sockaddr, socklen_t socklen, ngx_uint_t total);


in_addr_t
ngx_inet_addr(u_char *text, size_t len)
{
    u_char      *p, c;
    in_addr_t    addr;
    ngx_uint_t   octet, n;

    addr = 0;
    octet = 0;
    n = 0;

    വേണ്ടി (p = text; p < text + len; p++) {
        c = *p;

        എങ്കിൽ (c >= '0' && c <= '9') {
            octet = octet * 10 + (c - '0');

            എങ്കിൽ (octet > 255) {
                തിരികെ INADDR_NONE;
            }

            തുടരുക;
        }

        എങ്കിൽ (c == '.') {
            addr = (addr << 8) + octet;
            octet = 0;
            n++;
            തുടരുക;
        }

        തിരികെ INADDR_NONE;
    }

    എങ്കിൽ (n == 3) {
        addr = (addr << 8) + octet;
        തിരികെ htonl(addr);
    }

    തിരികെ INADDR_NONE;
}


#എങ്കിൽ (NGX_HAVE_INET6)

ngx_int_t
ngx_inet6_addr(u_char *p, size_t len, u_char *addr)
{
    u_char      c, *zero, *digit, *s, *d;
    size_t      len4;
    ngx_uint_t  n, nibbles, word;

    എങ്കിൽ (len == 0) {
        തിരികെ NGX_ERROR;
    }

    zero = ശൂന്യ;
    digit = ശൂന്യ;
    len4 = 0;
    nibbles = 0;
    word = 0;
    n = 8;

    എങ്കിൽ (p[0] == ':') {
        p++;
        len--;
    }

    വേണ്ടി (/* void */; len; len--) {
        c = *p++;

        എങ്കിൽ (c == ':') {
            എങ്കിൽ (nibbles) {
                digit = p;
                len4 = len;
                *addr++ = (u_char) (word >> 8);
                *addr++ = (u_char) (word & 0xff);

                എങ്കിൽ (--n) {
                    nibbles = 0;
                    word = 0;
                    തുടരുക;
                }

            } അല്ലെങ്കിൽ {
                എങ്കിൽ (zero == ശൂന്യ) {
                    digit = p;
                    len4 = len;
                    zero = addr;
                    തുടരുക;
                }
            }

            തിരികെ NGX_ERROR;
        }

        എങ്കിൽ (c == '.' && nibbles) {
            എങ്കിൽ (n < 2 || digit == ശൂന്യ) {
                തിരികെ NGX_ERROR;
            }

            word = ngx_inet_addr(digit, len4 - 1);
            എങ്കിൽ (word == INADDR_NONE) {
                തിരികെ NGX_ERROR;
            }

            word = ntohl(word);
            *addr++ = (u_char) ((word >> 24) & 0xff);
            *addr++ = (u_char) ((word >> 16) & 0xff);
            n--;
            നിർത്തുക;
        }

        എങ്കിൽ (++nibbles > 4) {
            തിരികെ NGX_ERROR;
        }

        എങ്കിൽ (c >= '0' && c <= '9') {
            word = word * 16 + (c - '0');
            തുടരുക;
        }

        c |= 0x20;

        എങ്കിൽ (c >= 'a' && c <= 'f') {
            word = word * 16 + (c - 'a') + 10;
            തുടരുക;
        }

        തിരികെ NGX_ERROR;
    }

    എങ്കിൽ (nibbles == 0 && zero == ശൂന്യ) {
        തിരികെ NGX_ERROR;
    }

    *addr++ = (u_char) (word >> 8);
    *addr++ = (u_char) (word & 0xff);

    എങ്കിൽ (--n) {
        എങ്കിൽ (zero) {
            n *= 2;
            s = addr - 1;
            d = s + n;
            ആകുമ്പോൾ (s >= zero) {
                *d-- = *s--;
            }
            ngx_memzero(zero, n);
            തിരികെ NGX_OK;
        }

    } അല്ലെങ്കിൽ {
        എങ്കിൽ (zero == ശൂന്യ) {
            തിരികെ NGX_OK;
        }
    }

    തിരികെ NGX_ERROR;
}

#endif


size_t
ngx_sock_ntop(ഘടന sockaddr *sa, socklen_t socklen, u_char *text, size_t len,
    ngx_uint_t port)
{
    u_char               *p;
#എങ്കിൽ (NGX_HAVE_INET6 || NGX_HAVE_UNIX_DOMAIN)
    size_t                n;
#endif
    ഘടന sockaddr_in   *sin;
#എങ്കിൽ (NGX_HAVE_INET6)
    ഘടന sockaddr_in6  *sin6;
#endif
#എങ്കിൽ (NGX_HAVE_UNIX_DOMAIN)
    ഘടന sockaddr_un   *saun;
#endif

    തിരഞ്ഞെടുപ്പ് (sa->sa_family) {

    അവസ്ഥ AF_INET:

        sin = (ഘടന sockaddr_in *) sa;
        p = (u_char *) &sin->sin_addr;

        എങ്കിൽ (port) {
            p = ngx_snprintf(text, len, "%ud.%ud.%ud.%ud:%d",
                             p[0], p[1], p[2], p[3], ntohs(sin->sin_port));
        } അല്ലെങ്കിൽ {
            p = ngx_snprintf(text, len, "%ud.%ud.%ud.%ud",
                             p[0], p[1], p[2], p[3]);
        }

        തിരികെ (p - text);

#എങ്കിൽ (NGX_HAVE_INET6)

    അവസ്ഥ AF_INET6:

        sin6 = (ഘടന sockaddr_in6 *) sa;

        n = 0;

        എങ്കിൽ (port) {
            text[n++] = '[';
        }

        n = ngx_inet6_ntop(sin6->sin6_addr.s6_addr, &text[n], len);

        എങ്കിൽ (port) {
            n = ngx_sprintf(&text[1 + n], "]:%d",
                            ntohs(sin6->sin6_port)) - text;
        }

        തിരികെ n;
#endif

#എങ്കിൽ (NGX_HAVE_UNIX_DOMAIN)

    അവസ്ഥ AF_UNIX:
        saun = (ഘടന sockaddr_un *) sa;

        /* on Linux sockaddr might not include sun_path at all */

        എങ്കിൽ (socklen <= (socklen_t) offsetof(ഘടന sockaddr_un, sun_path)) {
            p = ngx_snprintf(text, len, "unix:%Z");

        } അല്ലെങ്കിൽ {
            n = ngx_strnlen((u_char *) saun->sun_path,
                            socklen - offsetof(ഘടന sockaddr_un, sun_path));
            p = ngx_snprintf(text, len, "unix:%*s%Z", n, saun->sun_path);
        }

        /* we do not include trailing zero in address length */

        തിരികെ (p - text - 1);

#endif

    സ്ഥിരസ്ഥിതി:
        തിരികെ 0;
    }
}


size_t
ngx_inet_ntop(പൂർണ്ണസംഖ്യ family, ശൂന്യം *addr, u_char *text, size_t len)
{
    u_char  *p;

    തിരഞ്ഞെടുപ്പ് (family) {

    അവസ്ഥ AF_INET:

        p = addr;

        തിരികെ ngx_snprintf(text, len, "%ud.%ud.%ud.%ud",
                            p[0], p[1], p[2], p[3])
               - text;

#എങ്കിൽ (NGX_HAVE_INET6)

    അവസ്ഥ AF_INET6:
        തിരികെ ngx_inet6_ntop(addr, text, len);

#endif

    സ്ഥിരസ്ഥിതി:
        തിരികെ 0;
    }
}


#എങ്കിൽ (NGX_HAVE_INET6)

size_t
ngx_inet6_ntop(u_char *p, u_char *text, size_t len)
{
    u_char      *dst;
    size_t       max, n;
    ngx_uint_t   i, zero, last;

    എങ്കിൽ (len < NGX_INET6_ADDRSTRLEN) {
        തിരികെ 0;
    }

    zero = (ngx_uint_t) -1;
    last = (ngx_uint_t) -1;
    max = 1;
    n = 0;

    വേണ്ടി (i = 0; i < 16; i += 2) {

        എങ്കിൽ (p[i] || p[i + 1]) {

            എങ്കിൽ (max < n) {
                zero = last;
                max = n;
            }

            n = 0;
            തുടരുക;
        }

        എങ്കിൽ (n++ == 0) {
            last = i;
        }
    }

    എങ്കിൽ (max < n) {
        zero = last;
        max = n;
    }

    dst = text;
    n = 16;

    എങ്കിൽ (zero == 0) {

        എങ്കിൽ ((max == 5 && p[10] == 0xff && p[11] == 0xff)
            || (max == 6)
            || (max == 7 && p[14] != 0 && p[15] != 1))
        {
            n = 12;
        }

        *dst++ = ':';
    }

    വേണ്ടി (i = 0; i < n; i += 2) {

        എങ്കിൽ (i == zero) {
            *dst++ = ':';
            i += (max - 1) * 2;
            തുടരുക;
        }

        dst = ngx_sprintf(dst, "%xd", p[i] * 256 + p[i + 1]);

        എങ്കിൽ (i < 14) {
            *dst++ = ':';
        }
    }

    എങ്കിൽ (n == 12) {
        dst = ngx_sprintf(dst, "%ud.%ud.%ud.%ud", p[12], p[13], p[14], p[15]);
    }

    തിരികെ dst - text;
}

#endif


ngx_int_t
ngx_ptocidr(ngx_str_t *text, ngx_cidr_t *cidr)
{
    u_char      *addr, *mask, *last;
    size_t       len;
    ngx_int_t    shift;
#എങ്കിൽ (NGX_HAVE_INET6)
    ngx_int_t    rc;
    ngx_uint_t   s, i;
#endif

    addr = text->data;
    last = addr + text->len;

    mask = ngx_strlchr(addr, last, '/');
    len = (mask ? mask : last) - addr;

    cidr->u.in.addr = ngx_inet_addr(addr, len);

    എങ്കിൽ (cidr->u.in.addr != INADDR_NONE) {
        cidr->family = AF_INET;

        എങ്കിൽ (mask == ശൂന്യ) {
            cidr->u.in.mask = 0xffffffff;
            തിരികെ NGX_OK;
        }

#എങ്കിൽ (NGX_HAVE_INET6)
    } അല്ലെങ്കിൽ എങ്കിൽ (ngx_inet6_addr(addr, len, cidr->u.in6.addr.s6_addr) == NGX_OK) {
        cidr->family = AF_INET6;

        എങ്കിൽ (mask == ശൂന്യ) {
            ngx_memset(cidr->u.in6.mask.s6_addr, 0xff, 16);
            തിരികെ NGX_OK;
        }

#endif
    } അല്ലെങ്കിൽ {
        തിരികെ NGX_ERROR;
    }

    mask++;

    shift = ngx_atoi(mask, last - mask);
    എങ്കിൽ (shift == NGX_ERROR) {
        തിരികെ NGX_ERROR;
    }

    തിരഞ്ഞെടുപ്പ് (cidr->family) {

#എങ്കിൽ (NGX_HAVE_INET6)
    അവസ്ഥ AF_INET6:
        എങ്കിൽ (shift > 128) {
            തിരികെ NGX_ERROR;
        }

        addr = cidr->u.in6.addr.s6_addr;
        mask = cidr->u.in6.mask.s6_addr;
        rc = NGX_OK;

        വേണ്ടി (i = 0; i < 16; i++) {

            s = (shift > 8) ? 8 : shift;
            shift -= s;

            mask[i] = (u_char) (0xffu << (8 - s));

            എങ്കിൽ (addr[i] != (addr[i] & mask[i])) {
                rc = NGX_DONE;
                addr[i] &= mask[i];
            }
        }

        തിരികെ rc;
#endif

    സ്ഥിരസ്ഥിതി: /* AF_INET */
        എങ്കിൽ (shift > 32) {
            തിരികെ NGX_ERROR;
        }

        എങ്കിൽ (shift) {
            cidr->u.in.mask = htonl((uint32_t) (0xffffffffu << (32 - shift)));

        } അല്ലെങ്കിൽ {
            /* x86 compilers use a shl instruction that shifts by modulo 32 */
            cidr->u.in.mask = 0;
        }

        എങ്കിൽ (cidr->u.in.addr == (cidr->u.in.addr & cidr->u.in.mask)) {
            തിരികെ NGX_OK;
        }

        cidr->u.in.addr &= cidr->u.in.mask;

        തിരികെ NGX_DONE;
    }
}


ngx_int_t
ngx_cidr_match(ഘടന sockaddr *sa, ngx_array_t *cidrs)
{
#എങ്കിൽ (NGX_HAVE_INET6)
    u_char           *p;
#endif
    in_addr_t         inaddr;
    ngx_cidr_t       *cidr;
    ngx_uint_t        family, i;
#എങ്കിൽ (NGX_HAVE_INET6)
    ngx_uint_t        n;
    ഘടന in6_addr  *inaddr6;
#endif

#എങ്കിൽ (NGX_SUPPRESS_WARN)
    inaddr = 0;
#എങ്കിൽ (NGX_HAVE_INET6)
    inaddr6 = ശൂന്യ;
#endif
#endif

    family = sa->sa_family;

    എങ്കിൽ (family == AF_INET) {
        inaddr = ((ഘടന sockaddr_in *) sa)->sin_addr.s_addr;
    }

#എങ്കിൽ (NGX_HAVE_INET6)
    അല്ലെങ്കിൽ എങ്കിൽ (family == AF_INET6) {
        inaddr6 = &((ഘടന sockaddr_in6 *) sa)->sin6_addr;

        എങ്കിൽ (IN6_IS_ADDR_V4MAPPED(inaddr6)) {
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

    വേണ്ടി (cidr = cidrs->elts, i = 0; i < cidrs->nelts; i++) {
        എങ്കിൽ (cidr[i].family != family) {
            പോകുക next;
        }

        തിരഞ്ഞെടുപ്പ് (family) {

#എങ്കിൽ (NGX_HAVE_INET6)
        അവസ്ഥ AF_INET6:
            വേണ്ടി (n = 0; n < 16; n++) {
                എങ്കിൽ ((inaddr6->s6_addr[n] & cidr[i].u.in6.mask.s6_addr[n])
                    != cidr[i].u.in6.addr.s6_addr[n])
                {
                    പോകുക next;
                }
            }
            നിർത്തുക;
#endif

#എങ്കിൽ (NGX_HAVE_UNIX_DOMAIN)
        അവസ്ഥ AF_UNIX:
            നിർത്തുക;
#endif

        സ്ഥിരസ്ഥിതി: /* AF_INET */
            എങ്കിൽ ((inaddr & cidr[i].u.in.mask) != cidr[i].u.in.addr) {
                പോകുക next;
            }
            നിർത്തുക;
        }

        തിരികെ NGX_OK;

    next:
        തുടരുക;
    }

    തിരികെ NGX_DECLINED;
}


ngx_int_t
ngx_parse_addr(ngx_pool_t *pool, ngx_addr_t *addr, u_char *text, size_t len)
{
    in_addr_t             inaddr;
    ngx_uint_t            family;
    ഘടന sockaddr_in   *sin;
#എങ്കിൽ (NGX_HAVE_INET6)
    ഘടന in6_addr       inaddr6;
    ഘടന sockaddr_in6  *sin6;

    /*
     * prevent MSVC8 warning:
     *    potentially uninitialized local variable 'inaddr6' used
     */
    ngx_memzero(&inaddr6, വലുപ്പം(ഘടന in6_addr));
#endif

    inaddr = ngx_inet_addr(text, len);

    എങ്കിൽ (inaddr != INADDR_NONE) {
        family = AF_INET;
        len = വലുപ്പം(ഘടന sockaddr_in);

#എങ്കിൽ (NGX_HAVE_INET6)
    } അല്ലെങ്കിൽ എങ്കിൽ (ngx_inet6_addr(text, len, inaddr6.s6_addr) == NGX_OK) {
        family = AF_INET6;
        len = വലുപ്പം(ഘടന sockaddr_in6);

#endif
    } അല്ലെങ്കിൽ {
        തിരികെ NGX_DECLINED;
    }

    addr->sockaddr = ngx_pcalloc(pool, len);
    എങ്കിൽ (addr->sockaddr == ശൂന്യ) {
        തിരികെ NGX_ERROR;
    }

    addr->sockaddr->sa_family = (u_char) family;
    addr->socklen = len;

    തിരഞ്ഞെടുപ്പ് (family) {

#എങ്കിൽ (NGX_HAVE_INET6)
    അവസ്ഥ AF_INET6:
        sin6 = (ഘടന sockaddr_in6 *) addr->sockaddr;
        ngx_memcpy(sin6->sin6_addr.s6_addr, inaddr6.s6_addr, 16);
        നിർത്തുക;
#endif

    സ്ഥിരസ്ഥിതി: /* AF_INET */
        sin = (ഘടന sockaddr_in *) addr->sockaddr;
        sin->sin_addr.s_addr = inaddr;
        നിർത്തുക;
    }

    തിരികെ NGX_OK;
}


ngx_int_t
ngx_parse_addr_port(ngx_pool_t *pool, ngx_addr_t *addr, u_char *text,
    size_t len)
{
    u_char     *p, *last;
    size_t      plen;
    ngx_int_t   rc, port;

    rc = ngx_parse_addr(pool, addr, text, len);

    എങ്കിൽ (rc != NGX_DECLINED) {
        തിരികെ rc;
    }

    last = text + len;

#എങ്കിൽ (NGX_HAVE_INET6)
    എങ്കിൽ (len && text[0] == '[') {

        p = ngx_strlchr(text, last, ']');

        എങ്കിൽ (p == last - 1) {
            തിരികെ ngx_parse_addr(pool, addr, text + 1, len - 2);
        }

        എങ്കിൽ (p == ശൂന്യ || *++p != ':') {
            തിരികെ NGX_DECLINED;
        }

        text++;
        len -= 2;

    } അല്ലെങ്കിൽ
#endif

    {
        p = ngx_strlchr(text, last, ':');

        എങ്കിൽ (p == ശൂന്യ) {
            തിരികെ NGX_DECLINED;
        }
    }

    p++;
    plen = last - p;

    port = ngx_atoi(p, plen);

    എങ്കിൽ (port < 1 || port > 65535) {
        തിരികെ NGX_DECLINED;
    }

    len -= plen + 1;

    rc = ngx_parse_addr(pool, addr, text, len);

    എങ്കിൽ (rc != NGX_OK) {
        തിരികെ rc;
    }

    ngx_inet_set_port(addr->sockaddr, (in_port_t) port);

    തിരികെ NGX_OK;
}


ngx_int_t
ngx_parse_url(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char  *p;
    size_t   len;

    p = u->url.data;
    len = u->url.len;

    എങ്കിൽ (len >= 5 && ngx_strncasecmp(p, (u_char *) "unix:", 5) == 0) {
        തിരികെ ngx_parse_unix_domain_url(pool, u);
    }

    എങ്കിൽ (len && p[0] == '[') {
        തിരികെ ngx_parse_inet6_url(pool, u);
    }

    തിരികെ ngx_parse_inet_url(pool, u);
}


സ്ഥിതം ngx_int_t
ngx_parse_unix_domain_url(ngx_pool_t *pool, ngx_url_t *u)
{
#എങ്കിൽ (NGX_HAVE_UNIX_DOMAIN)
    u_char              *path, *uri, *last;
    size_t               len;
    ഘടന sockaddr_un  *saun;

    len = u->url.len;
    path = u->url.data;

    path += 5;
    len -= 5;

    എങ്കിൽ (u->uri_part) {

        last = path + len;
        uri = ngx_strlchr(path, last, ':');

        എങ്കിൽ (uri) {
            len = uri - path;
            uri++;
            u->uri.len = last - uri;
            u->uri.data = uri;
        }
    }

    എങ്കിൽ (len == 0) {
        u->err = "no path in the unix domain socket";
        തിരികെ NGX_ERROR;
    }

    u->host.len = len++;
    u->host.data = path;

    എങ്കിൽ (len > വലുപ്പം(saun->sun_path)) {
        u->err = "too long path in the unix domain socket";
        തിരികെ NGX_ERROR;
    }

    u->socklen = വലുപ്പം(ഘടന sockaddr_un);
    saun = (ഘടന sockaddr_un *) &u->sockaddr;
    saun->sun_family = AF_UNIX;
    (ശൂന്യം) ngx_cpystrn((u_char *) saun->sun_path, path, len);

    u->addrs = ngx_pcalloc(pool, വലുപ്പം(ngx_addr_t));
    എങ്കിൽ (u->addrs == ശൂന്യ) {
        തിരികെ NGX_ERROR;
    }

    saun = ngx_pcalloc(pool, വലുപ്പം(ഘടന sockaddr_un));
    എങ്കിൽ (saun == ശൂന്യ) {
        തിരികെ NGX_ERROR;
    }

    u->family = AF_UNIX;
    u->naddrs = 1;

    saun->sun_family = AF_UNIX;
    (ശൂന്യം) ngx_cpystrn((u_char *) saun->sun_path, path, len);

    u->addrs[0].sockaddr = (ഘടന sockaddr *) saun;
    u->addrs[0].socklen = വലുപ്പം(ഘടന sockaddr_un);
    u->addrs[0].name.len = len + 4;
    u->addrs[0].name.data = u->url.data;

    തിരികെ NGX_OK;

#അല്ലെങ്കിൽ

    u->err = "the unix domain sockets are not supported on this platform";

    തിരികെ NGX_ERROR;

#endif
}


സ്ഥിതം ngx_int_t
ngx_parse_inet_url(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char              *host, *port, *last, *uri, *args, *dash;
    size_t               len;
    ngx_int_t            n;
    ഘടന sockaddr_in  *sin;

    u->socklen = വലുപ്പം(ഘടന sockaddr_in);
    sin = (ഘടന sockaddr_in *) &u->sockaddr;
    sin->sin_family = AF_INET;

    u->family = AF_INET;

    host = u->url.data;

    last = host + u->url.len;

    port = ngx_strlchr(host, last, ':');

    uri = ngx_strlchr(host, last, '/');

    args = ngx_strlchr(host, last, '?');

    എങ്കിൽ (args) {
        എങ്കിൽ (uri == ശൂന്യ || args < uri) {
            uri = args;
        }
    }

    എങ്കിൽ (uri) {
        എങ്കിൽ (u->listen || !u->uri_part) {
            u->err = "invalid host";
            തിരികെ NGX_ERROR;
        }

        u->uri.len = last - uri;
        u->uri.data = uri;

        last = uri;

        എങ്കിൽ (uri < port) {
            port = ശൂന്യ;
        }
    }

    എങ്കിൽ (port) {
        port++;

        len = last - port;

        എങ്കിൽ (u->listen) {
            dash = ngx_strlchr(port, last, '-');

            എങ്കിൽ (dash) {
                dash++;

                n = ngx_atoi(dash, last - dash);

                എങ്കിൽ (n < 1 || n > 65535) {
                    u->err = "invalid port";
                    തിരികെ NGX_ERROR;
                }

                u->last_port = (in_port_t) n;

                len = dash - port - 1;
            }
        }

        n = ngx_atoi(port, len);

        എങ്കിൽ (n < 1 || n > 65535) {
            u->err = "invalid port";
            തിരികെ NGX_ERROR;
        }

        എങ്കിൽ (u->last_port && n > u->last_port) {
            u->err = "invalid port range";
            തിരികെ NGX_ERROR;
        }

        u->port = (in_port_t) n;
        sin->sin_port = htons((in_port_t) n);

        u->port_text.len = last - port;
        u->port_text.data = port;

        last = port - 1;

    } അല്ലെങ്കിൽ {
        എങ്കിൽ (uri == ശൂന്യ) {

            എങ്കിൽ (u->listen) {

                /* test value as port only */

                len = last - host;

                dash = ngx_strlchr(host, last, '-');

                എങ്കിൽ (dash) {
                    dash++;

                    n = ngx_atoi(dash, last - dash);

                    എങ്കിൽ (n == NGX_ERROR) {
                        പോകുക no_port;
                    }

                    എങ്കിൽ (n < 1 || n > 65535) {
                        u->err = "invalid port";

                    } അല്ലെങ്കിൽ {
                        u->last_port = (in_port_t) n;
                    }

                    len = dash - host - 1;
                }

                n = ngx_atoi(host, len);

                എങ്കിൽ (n != NGX_ERROR) {

                    എങ്കിൽ (u->err) {
                        തിരികെ NGX_ERROR;
                    }

                    എങ്കിൽ (n < 1 || n > 65535) {
                        u->err = "invalid port";
                        തിരികെ NGX_ERROR;
                    }

                    എങ്കിൽ (u->last_port && n > u->last_port) {
                        u->err = "invalid port range";
                        തിരികെ NGX_ERROR;
                    }

                    u->port = (in_port_t) n;
                    sin->sin_port = htons((in_port_t) n);
                    sin->sin_addr.s_addr = INADDR_ANY;

                    u->port_text.len = last - host;
                    u->port_text.data = host;

                    u->wildcard = 1;

                    തിരികെ ngx_inet_add_addr(pool, u, &u->sockaddr.sockaddr,
                                             u->socklen, 1);
                }
            }
        }

no_port:

        u->err = ശൂന്യ;
        u->no_port = 1;
        u->port = u->default_port;
        sin->sin_port = htons(u->default_port);
        u->last_port = 0;
    }

    len = last - host;

    എങ്കിൽ (len == 0) {
        u->err = "no host";
        തിരികെ NGX_ERROR;
    }

    u->host.len = len;
    u->host.data = host;

    എങ്കിൽ (u->listen && len == 1 && *host == '*') {
        sin->sin_addr.s_addr = INADDR_ANY;
        u->wildcard = 1;
        തിരികെ ngx_inet_add_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);
    }

    sin->sin_addr.s_addr = ngx_inet_addr(host, len);

    എങ്കിൽ (sin->sin_addr.s_addr != INADDR_NONE) {

        എങ്കിൽ (sin->sin_addr.s_addr == INADDR_ANY) {
            u->wildcard = 1;
        }

        തിരികെ ngx_inet_add_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);
    }

    എങ്കിൽ (u->no_resolve) {
        തിരികെ NGX_OK;
    }

    എങ്കിൽ (ngx_inet_resolve_host(pool, u) != NGX_OK) {
        തിരികെ NGX_ERROR;
    }

    u->family = u->addrs[0].sockaddr->sa_family;
    u->socklen = u->addrs[0].socklen;
    ngx_memcpy(&u->sockaddr, u->addrs[0].sockaddr, u->addrs[0].socklen);
    u->wildcard = ngx_inet_wildcard(&u->sockaddr.sockaddr);

    തിരികെ NGX_OK;
}


സ്ഥിതം ngx_int_t
ngx_parse_inet6_url(ngx_pool_t *pool, ngx_url_t *u)
{
#എങ്കിൽ (NGX_HAVE_INET6)
    u_char               *p, *host, *port, *last, *uri, *dash;
    size_t                len;
    ngx_int_t             n;
    ഘടന sockaddr_in6  *sin6;

    u->socklen = വലുപ്പം(ഘടന sockaddr_in6);
    sin6 = (ഘടന sockaddr_in6 *) &u->sockaddr;
    sin6->sin6_family = AF_INET6;

    host = u->url.data + 1;

    last = u->url.data + u->url.len;

    p = ngx_strlchr(host, last, ']');

    എങ്കിൽ (p == ശൂന്യ) {
        u->err = "invalid host";
        തിരികെ NGX_ERROR;
    }

    port = p + 1;

    uri = ngx_strlchr(port, last, '/');

    എങ്കിൽ (uri) {
        എങ്കിൽ (u->listen || !u->uri_part) {
            u->err = "invalid host";
            തിരികെ NGX_ERROR;
        }

        u->uri.len = last - uri;
        u->uri.data = uri;

        last = uri;
    }

    എങ്കിൽ (port < last) {
        എങ്കിൽ (*port != ':') {
            u->err = "invalid host";
            തിരികെ NGX_ERROR;
        }

        port++;

        len = last - port;

        എങ്കിൽ (u->listen) {
            dash = ngx_strlchr(port, last, '-');

            എങ്കിൽ (dash) {
                dash++;

                n = ngx_atoi(dash, last - dash);

                എങ്കിൽ (n < 1 || n > 65535) {
                    u->err = "invalid port";
                    തിരികെ NGX_ERROR;
                }

                u->last_port = (in_port_t) n;

                len = dash - port - 1;
            }
        }

        n = ngx_atoi(port, len);

        എങ്കിൽ (n < 1 || n > 65535) {
            u->err = "invalid port";
            തിരികെ NGX_ERROR;
        }

        എങ്കിൽ (u->last_port && n > u->last_port) {
            u->err = "invalid port range";
            തിരികെ NGX_ERROR;
        }

        u->port = (in_port_t) n;
        sin6->sin6_port = htons((in_port_t) n);

        u->port_text.len = last - port;
        u->port_text.data = port;

    } അല്ലെങ്കിൽ {
        u->no_port = 1;
        u->port = u->default_port;
        sin6->sin6_port = htons(u->default_port);
    }

    len = p - host;

    എങ്കിൽ (len == 0) {
        u->err = "no host";
        തിരികെ NGX_ERROR;
    }

    u->host.len = len + 2;
    u->host.data = host - 1;

    എങ്കിൽ (ngx_inet6_addr(host, len, sin6->sin6_addr.s6_addr) != NGX_OK) {
        u->err = "invalid IPv6 address";
        തിരികെ NGX_ERROR;
    }

    എങ്കിൽ (IN6_IS_ADDR_UNSPECIFIED(&sin6->sin6_addr)) {
        u->wildcard = 1;
    }

    u->family = AF_INET6;

    തിരികെ ngx_inet_add_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);

#അല്ലെങ്കിൽ

    u->err = "the INET6 sockets are not supported on this platform";

    തിരികെ NGX_ERROR;

#endif
}


#എങ്കിൽ (NGX_HAVE_GETADDRINFO && NGX_HAVE_INET6)

ngx_int_t
ngx_inet_resolve_host(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char           *host;
    ngx_uint_t        n;
    ഘടന addrinfo   hints, *res, *rp;

    host = ngx_alloc(u->host.len + 1, pool->log);
    എങ്കിൽ (host == ശൂന്യ) {
        തിരികെ NGX_ERROR;
    }

    (ശൂന്യം) ngx_cpystrn(host, u->host.data, u->host.len + 1);

    ngx_memzero(&hints, വലുപ്പം(ഘടന addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
#ifdef AI_ADDRCONFIG
    hints.ai_flags = AI_ADDRCONFIG;
#endif

    എങ്കിൽ (getaddrinfo((അക്ഷരം *) host, ശൂന്യ, &hints, &res) != 0) {
        u->err = "host not found";
        ngx_free(host);
        തിരികെ NGX_ERROR;
    }

    ngx_free(host);

    വേണ്ടി (n = 0, rp = res; rp != ശൂന്യ; rp = rp->ai_next) {

        തിരഞ്ഞെടുപ്പ് (rp->ai_family) {

        അവസ്ഥ AF_INET:
        അവസ്ഥ AF_INET6:
            നിർത്തുക;

        സ്ഥിരസ്ഥിതി:
            തുടരുക;
        }

        n++;
    }

    എങ്കിൽ (n == 0) {
        u->err = "host not found";
        പോകുക failed;
    }

    /* MP: ngx_shared_palloc() */

    വേണ്ടി (rp = res; rp != ശൂന്യ; rp = rp->ai_next) {

        തിരഞ്ഞെടുപ്പ് (rp->ai_family) {

        അവസ്ഥ AF_INET:
        അവസ്ഥ AF_INET6:
            നിർത്തുക;

        സ്ഥിരസ്ഥിതി:
            തുടരുക;
        }

        എങ്കിൽ (ngx_inet_add_addr(pool, u, rp->ai_addr, rp->ai_addrlen, n)
            != NGX_OK)
        {
            പോകുക failed;
        }
    }

    freeaddrinfo(res);
    തിരികെ NGX_OK;

failed:

    freeaddrinfo(res);
    തിരികെ NGX_ERROR;
}

#അല്ലെങ്കിൽ /* !NGX_HAVE_GETADDRINFO || !NGX_HAVE_INET6 */

ngx_int_t
ngx_inet_resolve_host(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char              *host;
    ngx_uint_t           i, n;
    ഘടന hostent      *h;
    ഘടന sockaddr_in   sin;

    /* AF_INET only */

    ngx_memzero(&sin, വലുപ്പം(ഘടന sockaddr_in));

    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = ngx_inet_addr(u->host.data, u->host.len);

    എങ്കിൽ (sin.sin_addr.s_addr == INADDR_NONE) {
        host = ngx_alloc(u->host.len + 1, pool->log);
        എങ്കിൽ (host == ശൂന്യ) {
            തിരികെ NGX_ERROR;
        }

        (ശൂന്യം) ngx_cpystrn(host, u->host.data, u->host.len + 1);

        h = gethostbyname((അക്ഷരം *) host);

        ngx_free(host);

        എങ്കിൽ (h == ശൂന്യ || h->h_addr_list[0] == ശൂന്യ) {
            u->err = "host not found";
            തിരികെ NGX_ERROR;
        }

        വേണ്ടി (n = 0; h->h_addr_list[n] != ശൂന്യ; n++) { /* void */ }

        /* MP: ngx_shared_palloc() */

        വേണ്ടി (i = 0; i < n; i++) {
            sin.sin_addr.s_addr = *(in_addr_t *) (h->h_addr_list[i]);

            എങ്കിൽ (ngx_inet_add_addr(pool, u, (ഘടന sockaddr *) &sin,
                                  വലുപ്പം(ഘടന sockaddr_in), n)
                != NGX_OK)
            {
                തിരികെ NGX_ERROR;
            }
        }

    } അല്ലെങ്കിൽ {

        /* MP: ngx_shared_palloc() */

        എങ്കിൽ (ngx_inet_add_addr(pool, u, (ഘടന sockaddr *) &sin,
                              വലുപ്പം(ഘടന sockaddr_in), 1)
            != NGX_OK)
        {
            തിരികെ NGX_ERROR;
        }
    }

    തിരികെ NGX_OK;
}

#endif /* NGX_HAVE_GETADDRINFO && NGX_HAVE_INET6 */


സ്ഥിതം ngx_int_t
ngx_inet_add_addr(ngx_pool_t *pool, ngx_url_t *u, ഘടന sockaddr *sockaddr,
    socklen_t socklen, ngx_uint_t total)
{
    u_char           *p;
    size_t            len;
    ngx_uint_t        i, nports;
    ngx_addr_t       *addr;
    ഘടന sockaddr  *sa;

    nports = u->last_port ? u->last_port - u->port + 1 : 1;

    എങ്കിൽ (u->addrs == ശൂന്യ) {
        u->addrs = ngx_palloc(pool, total * nports * വലുപ്പം(ngx_addr_t));
        എങ്കിൽ (u->addrs == ശൂന്യ) {
            തിരികെ NGX_ERROR;
        }
    }

    വേണ്ടി (i = 0; i < nports; i++) {
        sa = ngx_pcalloc(pool, socklen);
        എങ്കിൽ (sa == ശൂന്യ) {
            തിരികെ NGX_ERROR;
        }

        ngx_memcpy(sa, sockaddr, socklen);

        ngx_inet_set_port(sa, u->port + i);

        തിരഞ്ഞെടുപ്പ് (sa->sa_family) {

#എങ്കിൽ (NGX_HAVE_INET6)
        അവസ്ഥ AF_INET6:
            len = NGX_INET6_ADDRSTRLEN + വലുപ്പം("[]:65536") - 1;
            നിർത്തുക;
#endif

        സ്ഥിരസ്ഥിതി: /* AF_INET */
            len = NGX_INET_ADDRSTRLEN + വലുപ്പം(":65535") - 1;
        }

        p = ngx_pnalloc(pool, len);
        എങ്കിൽ (p == ശൂന്യ) {
            തിരികെ NGX_ERROR;
        }

        len = ngx_sock_ntop(sa, socklen, p, len, 1);

        addr = &u->addrs[u->naddrs++];

        addr->sockaddr = sa;
        addr->socklen = socklen;

        addr->name.len = len;
        addr->name.data = p;
    }

    തിരികെ NGX_OK;
}


ngx_int_t
ngx_cmp_sockaddr(ഘടന sockaddr *sa1, socklen_t slen1,
    ഘടന sockaddr *sa2, socklen_t slen2, ngx_uint_t cmp_port)
{
    ഘടന sockaddr_in   *sin1, *sin2;
#എങ്കിൽ (NGX_HAVE_INET6)
    ഘടന sockaddr_in6  *sin61, *sin62;
#endif
#എങ്കിൽ (NGX_HAVE_UNIX_DOMAIN)
    size_t                len;
    ഘടന sockaddr_un   *saun1, *saun2;
#endif

    എങ്കിൽ (sa1->sa_family != sa2->sa_family) {
        തിരികെ NGX_DECLINED;
    }

    തിരഞ്ഞെടുപ്പ് (sa1->sa_family) {

#എങ്കിൽ (NGX_HAVE_INET6)
    അവസ്ഥ AF_INET6:

        sin61 = (ഘടന sockaddr_in6 *) sa1;
        sin62 = (ഘടന sockaddr_in6 *) sa2;

        എങ്കിൽ (cmp_port && sin61->sin6_port != sin62->sin6_port) {
            തിരികെ NGX_DECLINED;
        }

        എങ്കിൽ (ngx_memcmp(&sin61->sin6_addr, &sin62->sin6_addr, 16) != 0) {
            തിരികെ NGX_DECLINED;
        }

        നിർത്തുക;
#endif

#എങ്കിൽ (NGX_HAVE_UNIX_DOMAIN)
    അവസ്ഥ AF_UNIX:

        saun1 = (ഘടന sockaddr_un *) sa1;
        saun2 = (ഘടന sockaddr_un *) sa2;

        എങ്കിൽ (slen1 < slen2) {
            len = slen1 - offsetof(ഘടന sockaddr_un, sun_path);

        } അല്ലെങ്കിൽ {
            len = slen2 - offsetof(ഘടന sockaddr_un, sun_path);
        }

        എങ്കിൽ (len > വലുപ്പം(saun1->sun_path)) {
            len = വലുപ്പം(saun1->sun_path);
        }

        എങ്കിൽ (ngx_memcmp(&saun1->sun_path, &saun2->sun_path, len) != 0) {
            തിരികെ NGX_DECLINED;
        }

        നിർത്തുക;
#endif

    സ്ഥിരസ്ഥിതി: /* AF_INET */

        sin1 = (ഘടന sockaddr_in *) sa1;
        sin2 = (ഘടന sockaddr_in *) sa2;

        എങ്കിൽ (cmp_port && sin1->sin_port != sin2->sin_port) {
            തിരികെ NGX_DECLINED;
        }

        എങ്കിൽ (sin1->sin_addr.s_addr != sin2->sin_addr.s_addr) {
            തിരികെ NGX_DECLINED;
        }

        നിർത്തുക;
    }

    തിരികെ NGX_OK;
}


in_port_t
ngx_inet_get_port(ഘടന sockaddr *sa)
{
    ഘടന sockaddr_in   *sin;
#എങ്കിൽ (NGX_HAVE_INET6)
    ഘടന sockaddr_in6  *sin6;
#endif

    തിരഞ്ഞെടുപ്പ് (sa->sa_family) {

#എങ്കിൽ (NGX_HAVE_INET6)
    അവസ്ഥ AF_INET6:
        sin6 = (ഘടന sockaddr_in6 *) sa;
        തിരികെ ntohs(sin6->sin6_port);
#endif

#എങ്കിൽ (NGX_HAVE_UNIX_DOMAIN)
    അവസ്ഥ AF_UNIX:
        തിരികെ 0;
#endif

    സ്ഥിരസ്ഥിതി: /* AF_INET */
        sin = (ഘടന sockaddr_in *) sa;
        തിരികെ ntohs(sin->sin_port);
    }
}


ശൂന്യം
ngx_inet_set_port(ഘടന sockaddr *sa, in_port_t port)
{
    ഘടന sockaddr_in   *sin;
#എങ്കിൽ (NGX_HAVE_INET6)
    ഘടന sockaddr_in6  *sin6;
#endif

    തിരഞ്ഞെടുപ്പ് (sa->sa_family) {

#എങ്കിൽ (NGX_HAVE_INET6)
    അവസ്ഥ AF_INET6:
        sin6 = (ഘടന sockaddr_in6 *) sa;
        sin6->sin6_port = htons(port);
        നിർത്തുക;
#endif

#എങ്കിൽ (NGX_HAVE_UNIX_DOMAIN)
    അവസ്ഥ AF_UNIX:
        നിർത്തുക;
#endif

    സ്ഥിരസ്ഥിതി: /* AF_INET */
        sin = (ഘടന sockaddr_in *) sa;
        sin->sin_port = htons(port);
        നിർത്തുക;
    }
}


ngx_uint_t
ngx_inet_wildcard(ഘടന sockaddr *sa)
{
    ഘടന sockaddr_in   *sin;
#എങ്കിൽ (NGX_HAVE_INET6)
    ഘടന sockaddr_in6  *sin6;
#endif

    തിരഞ്ഞെടുപ്പ് (sa->sa_family) {

    അവസ്ഥ AF_INET:
        sin = (ഘടന sockaddr_in *) sa;

        എങ്കിൽ (sin->sin_addr.s_addr == INADDR_ANY) {
            തിരികെ 1;
        }

        നിർത്തുക;

#എങ്കിൽ (NGX_HAVE_INET6)

    അവസ്ഥ AF_INET6:
        sin6 = (ഘടന sockaddr_in6 *) sa;

        എങ്കിൽ (IN6_IS_ADDR_UNSPECIFIED(&sin6->sin6_addr)) {
            തിരികെ 1;
        }

        നിർത്തുക;

#endif
    }

    തിരികെ 0;
}
