
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#شامل <ngx_config.h>
#شامل <ngx_core.h>


ایستا ngx_int_t ngx_parse_unix_domain_url(ngx_pool_t *pool, ngx_url_t *u);
ایستا ngx_int_t ngx_parse_inet_url(ngx_pool_t *pool, ngx_url_t *u);
ایستا ngx_int_t ngx_parse_inet6_url(ngx_pool_t *pool, ngx_url_t *u);
ایستا ngx_int_t ngx_inet_add_addr(ngx_pool_t *pool, ngx_url_t *u,
    ساختار sockaddr *sockaddr, socklen_t socklen, ngx_uint_t total);


in_addr_t
ngx_inet_addr(u_char *text, size_t len)
{
    u_char      *p, c;
    in_addr_t    addr;
    ngx_uint_t   octet, n;

    addr = 0;
    octet = 0;
    n = 0;

    برای (p = text; p < text + len; p++) {
        c = *p;

        اگر (c >= '0' && c <= '9') {
            octet = octet * 10 + (c - '0');

            اگر (octet > 255) {
                بازگشت INADDR_NONE;
            }

            ادامه;
        }

        اگر (c == '.') {
            addr = (addr << 8) + octet;
            octet = 0;
            n++;
            ادامه;
        }

        بازگشت INADDR_NONE;
    }

    اگر (n == 3) {
        addr = (addr << 8) + octet;
        بازگشت htonl(addr);
    }

    بازگشت INADDR_NONE;
}


#اگر (NGX_HAVE_INET6)

ngx_int_t
ngx_inet6_addr(u_char *p, size_t len, u_char *addr)
{
    u_char      c, *zero, *digit, *s, *d;
    size_t      len4;
    ngx_uint_t  n, nibbles, word;

    اگر (len == 0) {
        بازگشت NGX_ERROR;
    }

    zero = پوچ;
    digit = پوچ;
    len4 = 0;
    nibbles = 0;
    word = 0;
    n = 8;

    اگر (p[0] == ':') {
        p++;
        len--;
    }

    برای (/* void */; len; len--) {
        c = *p++;

        اگر (c == ':') {
            اگر (nibbles) {
                digit = p;
                len4 = len;
                *addr++ = (u_char) (word >> 8);
                *addr++ = (u_char) (word & 0xff);

                اگر (--n) {
                    nibbles = 0;
                    word = 0;
                    ادامه;
                }

            } وگرنه {
                اگر (zero == پوچ) {
                    digit = p;
                    len4 = len;
                    zero = addr;
                    ادامه;
                }
            }

            بازگشت NGX_ERROR;
        }

        اگر (c == '.' && nibbles) {
            اگر (n < 2 || digit == پوچ) {
                بازگشت NGX_ERROR;
            }

            word = ngx_inet_addr(digit, len4 - 1);
            اگر (word == INADDR_NONE) {
                بازگشت NGX_ERROR;
            }

            word = ntohl(word);
            *addr++ = (u_char) ((word >> 24) & 0xff);
            *addr++ = (u_char) ((word >> 16) & 0xff);
            n--;
            بشکن;
        }

        اگر (++nibbles > 4) {
            بازگشت NGX_ERROR;
        }

        اگر (c >= '0' && c <= '9') {
            word = word * 16 + (c - '0');
            ادامه;
        }

        c |= 0x20;

        اگر (c >= 'a' && c <= 'f') {
            word = word * 16 + (c - 'a') + 10;
            ادامه;
        }

        بازگشت NGX_ERROR;
    }

    اگر (nibbles == 0 && zero == پوچ) {
        بازگشت NGX_ERROR;
    }

    *addr++ = (u_char) (word >> 8);
    *addr++ = (u_char) (word & 0xff);

    اگر (--n) {
        اگر (zero) {
            n *= 2;
            s = addr - 1;
            d = s + n;
            وقتی (s >= zero) {
                *d-- = *s--;
            }
            ngx_memzero(zero, n);
            بازگشت NGX_OK;
        }

    } وگرنه {
        اگر (zero == پوچ) {
            بازگشت NGX_OK;
        }
    }

    بازگشت NGX_ERROR;
}

#endif


size_t
ngx_sock_ntop(ساختار sockaddr *sa, socklen_t socklen, u_char *text, size_t len,
    ngx_uint_t port)
{
    u_char               *p;
#اگر (NGX_HAVE_INET6 || NGX_HAVE_UNIX_DOMAIN)
    size_t                n;
#endif
    ساختار sockaddr_in   *sin;
#اگر (NGX_HAVE_INET6)
    ساختار sockaddr_in6  *sin6;
#endif
#اگر (NGX_HAVE_UNIX_DOMAIN)
    ساختار sockaddr_un   *saun;
#endif

    انتخاب (sa->sa_family) {

    حالت AF_INET:

        sin = (ساختار sockaddr_in *) sa;
        p = (u_char *) &sin->sin_addr;

        اگر (port) {
            p = ngx_snprintf(text, len, "%ud.%ud.%ud.%ud:%d",
                             p[0], p[1], p[2], p[3], ntohs(sin->sin_port));
        } وگرنه {
            p = ngx_snprintf(text, len, "%ud.%ud.%ud.%ud",
                             p[0], p[1], p[2], p[3]);
        }

        بازگشت (p - text);

#اگر (NGX_HAVE_INET6)

    حالت AF_INET6:

        sin6 = (ساختار sockaddr_in6 *) sa;

        n = 0;

        اگر (port) {
            text[n++] = '[';
        }

        n = ngx_inet6_ntop(sin6->sin6_addr.s6_addr, &text[n], len);

        اگر (port) {
            n = ngx_sprintf(&text[1 + n], "]:%d",
                            ntohs(sin6->sin6_port)) - text;
        }

        بازگشت n;
#endif

#اگر (NGX_HAVE_UNIX_DOMAIN)

    حالت AF_UNIX:
        saun = (ساختار sockaddr_un *) sa;

        /* on Linux sockaddr might not include sun_path at all */

        اگر (socklen <= (socklen_t) offsetof(ساختار sockaddr_un, sun_path)) {
            p = ngx_snprintf(text, len, "unix:%Z");

        } وگرنه {
            n = ngx_strnlen((u_char *) saun->sun_path,
                            socklen - offsetof(ساختار sockaddr_un, sun_path));
            p = ngx_snprintf(text, len, "unix:%*s%Z", n, saun->sun_path);
        }

        /* we do not include trailing zero in address length */

        بازگشت (p - text - 1);

#endif

    پیشفرض:
        بازگشت 0;
    }
}


size_t
ngx_inet_ntop(صحیح family, تهی *addr, u_char *text, size_t len)
{
    u_char  *p;

    انتخاب (family) {

    حالت AF_INET:

        p = addr;

        بازگشت ngx_snprintf(text, len, "%ud.%ud.%ud.%ud",
                            p[0], p[1], p[2], p[3])
               - text;

#اگر (NGX_HAVE_INET6)

    حالت AF_INET6:
        بازگشت ngx_inet6_ntop(addr, text, len);

#endif

    پیشفرض:
        بازگشت 0;
    }
}


#اگر (NGX_HAVE_INET6)

size_t
ngx_inet6_ntop(u_char *p, u_char *text, size_t len)
{
    u_char      *dst;
    size_t       max, n;
    ngx_uint_t   i, zero, last;

    اگر (len < NGX_INET6_ADDRSTRLEN) {
        بازگشت 0;
    }

    zero = (ngx_uint_t) -1;
    last = (ngx_uint_t) -1;
    max = 1;
    n = 0;

    برای (i = 0; i < 16; i += 2) {

        اگر (p[i] || p[i + 1]) {

            اگر (max < n) {
                zero = last;
                max = n;
            }

            n = 0;
            ادامه;
        }

        اگر (n++ == 0) {
            last = i;
        }
    }

    اگر (max < n) {
        zero = last;
        max = n;
    }

    dst = text;
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

    برای (i = 0; i < n; i += 2) {

        اگر (i == zero) {
            *dst++ = ':';
            i += (max - 1) * 2;
            ادامه;
        }

        dst = ngx_sprintf(dst, "%xd", p[i] * 256 + p[i + 1]);

        اگر (i < 14) {
            *dst++ = ':';
        }
    }

    اگر (n == 12) {
        dst = ngx_sprintf(dst, "%ud.%ud.%ud.%ud", p[12], p[13], p[14], p[15]);
    }

    بازگشت dst - text;
}

#endif


ngx_int_t
ngx_ptocidr(ngx_str_t *text, ngx_cidr_t *cidr)
{
    u_char      *addr, *mask, *last;
    size_t       len;
    ngx_int_t    shift;
#اگر (NGX_HAVE_INET6)
    ngx_int_t    rc;
    ngx_uint_t   s, i;
#endif

    addr = text->data;
    last = addr + text->len;

    mask = ngx_strlchr(addr, last, '/');
    len = (mask ? mask : last) - addr;

    cidr->u.in.addr = ngx_inet_addr(addr, len);

    اگر (cidr->u.in.addr != INADDR_NONE) {
        cidr->family = AF_INET;

        اگر (mask == پوچ) {
            cidr->u.in.mask = 0xffffffff;
            بازگشت NGX_OK;
        }

#اگر (NGX_HAVE_INET6)
    } وگرنه اگر (ngx_inet6_addr(addr, len, cidr->u.in6.addr.s6_addr) == NGX_OK) {
        cidr->family = AF_INET6;

        اگر (mask == پوچ) {
            ngx_memset(cidr->u.in6.mask.s6_addr, 0xff, 16);
            بازگشت NGX_OK;
        }

#endif
    } وگرنه {
        بازگشت NGX_ERROR;
    }

    mask++;

    shift = ngx_atoi(mask, last - mask);
    اگر (shift == NGX_ERROR) {
        بازگشت NGX_ERROR;
    }

    انتخاب (cidr->family) {

#اگر (NGX_HAVE_INET6)
    حالت AF_INET6:
        اگر (shift > 128) {
            بازگشت NGX_ERROR;
        }

        addr = cidr->u.in6.addr.s6_addr;
        mask = cidr->u.in6.mask.s6_addr;
        rc = NGX_OK;

        برای (i = 0; i < 16; i++) {

            s = (shift > 8) ? 8 : shift;
            shift -= s;

            mask[i] = (u_char) (0xffu << (8 - s));

            اگر (addr[i] != (addr[i] & mask[i])) {
                rc = NGX_DONE;
                addr[i] &= mask[i];
            }
        }

        بازگشت rc;
#endif

    پیشفرض: /* AF_INET */
        اگر (shift > 32) {
            بازگشت NGX_ERROR;
        }

        اگر (shift) {
            cidr->u.in.mask = htonl((uint32_t) (0xffffffffu << (32 - shift)));

        } وگرنه {
            /* x86 compilers use a shl instruction that shifts by modulo 32 */
            cidr->u.in.mask = 0;
        }

        اگر (cidr->u.in.addr == (cidr->u.in.addr & cidr->u.in.mask)) {
            بازگشت NGX_OK;
        }

        cidr->u.in.addr &= cidr->u.in.mask;

        بازگشت NGX_DONE;
    }
}


ngx_int_t
ngx_cidr_match(ساختار sockaddr *sa, ngx_array_t *cidrs)
{
#اگر (NGX_HAVE_INET6)
    u_char           *p;
#endif
    in_addr_t         inaddr;
    ngx_cidr_t       *cidr;
    ngx_uint_t        family, i;
#اگر (NGX_HAVE_INET6)
    ngx_uint_t        n;
    ساختار in6_addr  *inaddr6;
#endif

#اگر (NGX_SUPPRESS_WARN)
    inaddr = 0;
#اگر (NGX_HAVE_INET6)
    inaddr6 = پوچ;
#endif
#endif

    family = sa->sa_family;

    اگر (family == AF_INET) {
        inaddr = ((ساختار sockaddr_in *) sa)->sin_addr.s_addr;
    }

#اگر (NGX_HAVE_INET6)
    وگرنه اگر (family == AF_INET6) {
        inaddr6 = &((ساختار sockaddr_in6 *) sa)->sin6_addr;

        اگر (IN6_IS_ADDR_V4MAPPED(inaddr6)) {
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

    برای (cidr = cidrs->elts, i = 0; i < cidrs->nelts; i++) {
        اگر (cidr[i].family != family) {
            برو next;
        }

        انتخاب (family) {

#اگر (NGX_HAVE_INET6)
        حالت AF_INET6:
            برای (n = 0; n < 16; n++) {
                اگر ((inaddr6->s6_addr[n] & cidr[i].u.in6.mask.s6_addr[n])
                    != cidr[i].u.in6.addr.s6_addr[n])
                {
                    برو next;
                }
            }
            بشکن;
#endif

#اگر (NGX_HAVE_UNIX_DOMAIN)
        حالت AF_UNIX:
            بشکن;
#endif

        پیشفرض: /* AF_INET */
            اگر ((inaddr & cidr[i].u.in.mask) != cidr[i].u.in.addr) {
                برو next;
            }
            بشکن;
        }

        بازگشت NGX_OK;

    next:
        ادامه;
    }

    بازگشت NGX_DECLINED;
}


ngx_int_t
ngx_parse_addr(ngx_pool_t *pool, ngx_addr_t *addr, u_char *text, size_t len)
{
    in_addr_t             inaddr;
    ngx_uint_t            family;
    ساختار sockaddr_in   *sin;
#اگر (NGX_HAVE_INET6)
    ساختار in6_addr       inaddr6;
    ساختار sockaddr_in6  *sin6;

    /*
     * prevent MSVC8 warning:
     *    potentially uninitialized local variable 'inaddr6' used
     */
    ngx_memzero(&inaddr6, اندازه(ساختار in6_addr));
#endif

    inaddr = ngx_inet_addr(text, len);

    اگر (inaddr != INADDR_NONE) {
        family = AF_INET;
        len = اندازه(ساختار sockaddr_in);

#اگر (NGX_HAVE_INET6)
    } وگرنه اگر (ngx_inet6_addr(text, len, inaddr6.s6_addr) == NGX_OK) {
        family = AF_INET6;
        len = اندازه(ساختار sockaddr_in6);

#endif
    } وگرنه {
        بازگشت NGX_DECLINED;
    }

    addr->sockaddr = ngx_pcalloc(pool, len);
    اگر (addr->sockaddr == پوچ) {
        بازگشت NGX_ERROR;
    }

    addr->sockaddr->sa_family = (u_char) family;
    addr->socklen = len;

    انتخاب (family) {

#اگر (NGX_HAVE_INET6)
    حالت AF_INET6:
        sin6 = (ساختار sockaddr_in6 *) addr->sockaddr;
        ngx_memcpy(sin6->sin6_addr.s6_addr, inaddr6.s6_addr, 16);
        بشکن;
#endif

    پیشفرض: /* AF_INET */
        sin = (ساختار sockaddr_in *) addr->sockaddr;
        sin->sin_addr.s_addr = inaddr;
        بشکن;
    }

    بازگشت NGX_OK;
}


ngx_int_t
ngx_parse_addr_port(ngx_pool_t *pool, ngx_addr_t *addr, u_char *text,
    size_t len)
{
    u_char     *p, *last;
    size_t      plen;
    ngx_int_t   rc, port;

    rc = ngx_parse_addr(pool, addr, text, len);

    اگر (rc != NGX_DECLINED) {
        بازگشت rc;
    }

    last = text + len;

#اگر (NGX_HAVE_INET6)
    اگر (len && text[0] == '[') {

        p = ngx_strlchr(text, last, ']');

        اگر (p == last - 1) {
            بازگشت ngx_parse_addr(pool, addr, text + 1, len - 2);
        }

        اگر (p == پوچ || *++p != ':') {
            بازگشت NGX_DECLINED;
        }

        text++;
        len -= 2;

    } وگرنه
#endif

    {
        p = ngx_strlchr(text, last, ':');

        اگر (p == پوچ) {
            بازگشت NGX_DECLINED;
        }
    }

    p++;
    plen = last - p;

    port = ngx_atoi(p, plen);

    اگر (port < 1 || port > 65535) {
        بازگشت NGX_DECLINED;
    }

    len -= plen + 1;

    rc = ngx_parse_addr(pool, addr, text, len);

    اگر (rc != NGX_OK) {
        بازگشت rc;
    }

    ngx_inet_set_port(addr->sockaddr, (in_port_t) port);

    بازگشت NGX_OK;
}


ngx_int_t
ngx_parse_url(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char  *p;
    size_t   len;

    p = u->url.data;
    len = u->url.len;

    اگر (len >= 5 && ngx_strncasecmp(p, (u_char *) "unix:", 5) == 0) {
        بازگشت ngx_parse_unix_domain_url(pool, u);
    }

    اگر (len && p[0] == '[') {
        بازگشت ngx_parse_inet6_url(pool, u);
    }

    بازگشت ngx_parse_inet_url(pool, u);
}


ایستا ngx_int_t
ngx_parse_unix_domain_url(ngx_pool_t *pool, ngx_url_t *u)
{
#اگر (NGX_HAVE_UNIX_DOMAIN)
    u_char              *path, *uri, *last;
    size_t               len;
    ساختار sockaddr_un  *saun;

    len = u->url.len;
    path = u->url.data;

    path += 5;
    len -= 5;

    اگر (u->uri_part) {

        last = path + len;
        uri = ngx_strlchr(path, last, ':');

        اگر (uri) {
            len = uri - path;
            uri++;
            u->uri.len = last - uri;
            u->uri.data = uri;
        }
    }

    اگر (len == 0) {
        u->err = "no path in the unix domain socket";
        بازگشت NGX_ERROR;
    }

    u->host.len = len++;
    u->host.data = path;

    اگر (len > اندازه(saun->sun_path)) {
        u->err = "too long path in the unix domain socket";
        بازگشت NGX_ERROR;
    }

    u->socklen = اندازه(ساختار sockaddr_un);
    saun = (ساختار sockaddr_un *) &u->sockaddr;
    saun->sun_family = AF_UNIX;
    (تهی) ngx_cpystrn((u_char *) saun->sun_path, path, len);

    u->addrs = ngx_pcalloc(pool, اندازه(ngx_addr_t));
    اگر (u->addrs == پوچ) {
        بازگشت NGX_ERROR;
    }

    saun = ngx_pcalloc(pool, اندازه(ساختار sockaddr_un));
    اگر (saun == پوچ) {
        بازگشت NGX_ERROR;
    }

    u->family = AF_UNIX;
    u->naddrs = 1;

    saun->sun_family = AF_UNIX;
    (تهی) ngx_cpystrn((u_char *) saun->sun_path, path, len);

    u->addrs[0].sockaddr = (ساختار sockaddr *) saun;
    u->addrs[0].socklen = اندازه(ساختار sockaddr_un);
    u->addrs[0].name.len = len + 4;
    u->addrs[0].name.data = u->url.data;

    بازگشت NGX_OK;

#وگرنه

    u->err = "the unix domain sockets are not supported on this platform";

    بازگشت NGX_ERROR;

#endif
}


ایستا ngx_int_t
ngx_parse_inet_url(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char              *host, *port, *last, *uri, *args, *dash;
    size_t               len;
    ngx_int_t            n;
    ساختار sockaddr_in  *sin;

    u->socklen = اندازه(ساختار sockaddr_in);
    sin = (ساختار sockaddr_in *) &u->sockaddr;
    sin->sin_family = AF_INET;

    u->family = AF_INET;

    host = u->url.data;

    last = host + u->url.len;

    port = ngx_strlchr(host, last, ':');

    uri = ngx_strlchr(host, last, '/');

    args = ngx_strlchr(host, last, '?');

    اگر (args) {
        اگر (uri == پوچ || args < uri) {
            uri = args;
        }
    }

    اگر (uri) {
        اگر (u->listen || !u->uri_part) {
            u->err = "invalid host";
            بازگشت NGX_ERROR;
        }

        u->uri.len = last - uri;
        u->uri.data = uri;

        last = uri;

        اگر (uri < port) {
            port = پوچ;
        }
    }

    اگر (port) {
        port++;

        len = last - port;

        اگر (u->listen) {
            dash = ngx_strlchr(port, last, '-');

            اگر (dash) {
                dash++;

                n = ngx_atoi(dash, last - dash);

                اگر (n < 1 || n > 65535) {
                    u->err = "invalid port";
                    بازگشت NGX_ERROR;
                }

                u->last_port = (in_port_t) n;

                len = dash - port - 1;
            }
        }

        n = ngx_atoi(port, len);

        اگر (n < 1 || n > 65535) {
            u->err = "invalid port";
            بازگشت NGX_ERROR;
        }

        اگر (u->last_port && n > u->last_port) {
            u->err = "invalid port range";
            بازگشت NGX_ERROR;
        }

        u->port = (in_port_t) n;
        sin->sin_port = htons((in_port_t) n);

        u->port_text.len = last - port;
        u->port_text.data = port;

        last = port - 1;

    } وگرنه {
        اگر (uri == پوچ) {

            اگر (u->listen) {

                /* test value as port only */

                len = last - host;

                dash = ngx_strlchr(host, last, '-');

                اگر (dash) {
                    dash++;

                    n = ngx_atoi(dash, last - dash);

                    اگر (n == NGX_ERROR) {
                        برو no_port;
                    }

                    اگر (n < 1 || n > 65535) {
                        u->err = "invalid port";

                    } وگرنه {
                        u->last_port = (in_port_t) n;
                    }

                    len = dash - host - 1;
                }

                n = ngx_atoi(host, len);

                اگر (n != NGX_ERROR) {

                    اگر (u->err) {
                        بازگشت NGX_ERROR;
                    }

                    اگر (n < 1 || n > 65535) {
                        u->err = "invalid port";
                        بازگشت NGX_ERROR;
                    }

                    اگر (u->last_port && n > u->last_port) {
                        u->err = "invalid port range";
                        بازگشت NGX_ERROR;
                    }

                    u->port = (in_port_t) n;
                    sin->sin_port = htons((in_port_t) n);
                    sin->sin_addr.s_addr = INADDR_ANY;

                    u->port_text.len = last - host;
                    u->port_text.data = host;

                    u->wildcard = 1;

                    بازگشت ngx_inet_add_addr(pool, u, &u->sockaddr.sockaddr,
                                             u->socklen, 1);
                }
            }
        }

no_port:

        u->err = پوچ;
        u->no_port = 1;
        u->port = u->default_port;
        sin->sin_port = htons(u->default_port);
        u->last_port = 0;
    }

    len = last - host;

    اگر (len == 0) {
        u->err = "no host";
        بازگشت NGX_ERROR;
    }

    u->host.len = len;
    u->host.data = host;

    اگر (u->listen && len == 1 && *host == '*') {
        sin->sin_addr.s_addr = INADDR_ANY;
        u->wildcard = 1;
        بازگشت ngx_inet_add_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);
    }

    sin->sin_addr.s_addr = ngx_inet_addr(host, len);

    اگر (sin->sin_addr.s_addr != INADDR_NONE) {

        اگر (sin->sin_addr.s_addr == INADDR_ANY) {
            u->wildcard = 1;
        }

        بازگشت ngx_inet_add_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);
    }

    اگر (u->no_resolve) {
        بازگشت NGX_OK;
    }

    اگر (ngx_inet_resolve_host(pool, u) != NGX_OK) {
        بازگشت NGX_ERROR;
    }

    u->family = u->addrs[0].sockaddr->sa_family;
    u->socklen = u->addrs[0].socklen;
    ngx_memcpy(&u->sockaddr, u->addrs[0].sockaddr, u->addrs[0].socklen);
    u->wildcard = ngx_inet_wildcard(&u->sockaddr.sockaddr);

    بازگشت NGX_OK;
}


ایستا ngx_int_t
ngx_parse_inet6_url(ngx_pool_t *pool, ngx_url_t *u)
{
#اگر (NGX_HAVE_INET6)
    u_char               *p, *host, *port, *last, *uri, *dash;
    size_t                len;
    ngx_int_t             n;
    ساختار sockaddr_in6  *sin6;

    u->socklen = اندازه(ساختار sockaddr_in6);
    sin6 = (ساختار sockaddr_in6 *) &u->sockaddr;
    sin6->sin6_family = AF_INET6;

    host = u->url.data + 1;

    last = u->url.data + u->url.len;

    p = ngx_strlchr(host, last, ']');

    اگر (p == پوچ) {
        u->err = "invalid host";
        بازگشت NGX_ERROR;
    }

    port = p + 1;

    uri = ngx_strlchr(port, last, '/');

    اگر (uri) {
        اگر (u->listen || !u->uri_part) {
            u->err = "invalid host";
            بازگشت NGX_ERROR;
        }

        u->uri.len = last - uri;
        u->uri.data = uri;

        last = uri;
    }

    اگر (port < last) {
        اگر (*port != ':') {
            u->err = "invalid host";
            بازگشت NGX_ERROR;
        }

        port++;

        len = last - port;

        اگر (u->listen) {
            dash = ngx_strlchr(port, last, '-');

            اگر (dash) {
                dash++;

                n = ngx_atoi(dash, last - dash);

                اگر (n < 1 || n > 65535) {
                    u->err = "invalid port";
                    بازگشت NGX_ERROR;
                }

                u->last_port = (in_port_t) n;

                len = dash - port - 1;
            }
        }

        n = ngx_atoi(port, len);

        اگر (n < 1 || n > 65535) {
            u->err = "invalid port";
            بازگشت NGX_ERROR;
        }

        اگر (u->last_port && n > u->last_port) {
            u->err = "invalid port range";
            بازگشت NGX_ERROR;
        }

        u->port = (in_port_t) n;
        sin6->sin6_port = htons((in_port_t) n);

        u->port_text.len = last - port;
        u->port_text.data = port;

    } وگرنه {
        u->no_port = 1;
        u->port = u->default_port;
        sin6->sin6_port = htons(u->default_port);
    }

    len = p - host;

    اگر (len == 0) {
        u->err = "no host";
        بازگشت NGX_ERROR;
    }

    u->host.len = len + 2;
    u->host.data = host - 1;

    اگر (ngx_inet6_addr(host, len, sin6->sin6_addr.s6_addr) != NGX_OK) {
        u->err = "invalid IPv6 address";
        بازگشت NGX_ERROR;
    }

    اگر (IN6_IS_ADDR_UNSPECIFIED(&sin6->sin6_addr)) {
        u->wildcard = 1;
    }

    u->family = AF_INET6;

    بازگشت ngx_inet_add_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);

#وگرنه

    u->err = "the INET6 sockets are not supported on this platform";

    بازگشت NGX_ERROR;

#endif
}


#اگر (NGX_HAVE_GETADDRINFO && NGX_HAVE_INET6)

ngx_int_t
ngx_inet_resolve_host(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char           *host;
    ngx_uint_t        n;
    ساختار addrinfo   hints, *res, *rp;

    host = ngx_alloc(u->host.len + 1, pool->log);
    اگر (host == پوچ) {
        بازگشت NGX_ERROR;
    }

    (تهی) ngx_cpystrn(host, u->host.data, u->host.len + 1);

    ngx_memzero(&hints, اندازه(ساختار addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
#ifdef AI_ADDRCONFIG
    hints.ai_flags = AI_ADDRCONFIG;
#endif

    اگر (getaddrinfo((نویسه *) host, پوچ, &hints, &res) != 0) {
        u->err = "host not found";
        ngx_free(host);
        بازگشت NGX_ERROR;
    }

    ngx_free(host);

    برای (n = 0, rp = res; rp != پوچ; rp = rp->ai_next) {

        انتخاب (rp->ai_family) {

        حالت AF_INET:
        حالت AF_INET6:
            بشکن;

        پیشفرض:
            ادامه;
        }

        n++;
    }

    اگر (n == 0) {
        u->err = "host not found";
        برو failed;
    }

    /* MP: ngx_shared_palloc() */

    برای (rp = res; rp != پوچ; rp = rp->ai_next) {

        انتخاب (rp->ai_family) {

        حالت AF_INET:
        حالت AF_INET6:
            بشکن;

        پیشفرض:
            ادامه;
        }

        اگر (ngx_inet_add_addr(pool, u, rp->ai_addr, rp->ai_addrlen, n)
            != NGX_OK)
        {
            برو failed;
        }
    }

    freeaddrinfo(res);
    بازگشت NGX_OK;

failed:

    freeaddrinfo(res);
    بازگشت NGX_ERROR;
}

#وگرنه /* !NGX_HAVE_GETADDRINFO || !NGX_HAVE_INET6 */

ngx_int_t
ngx_inet_resolve_host(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char              *host;
    ngx_uint_t           i, n;
    ساختار hostent      *h;
    ساختار sockaddr_in   sin;

    /* AF_INET only */

    ngx_memzero(&sin, اندازه(ساختار sockaddr_in));

    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = ngx_inet_addr(u->host.data, u->host.len);

    اگر (sin.sin_addr.s_addr == INADDR_NONE) {
        host = ngx_alloc(u->host.len + 1, pool->log);
        اگر (host == پوچ) {
            بازگشت NGX_ERROR;
        }

        (تهی) ngx_cpystrn(host, u->host.data, u->host.len + 1);

        h = gethostbyname((نویسه *) host);

        ngx_free(host);

        اگر (h == پوچ || h->h_addr_list[0] == پوچ) {
            u->err = "host not found";
            بازگشت NGX_ERROR;
        }

        برای (n = 0; h->h_addr_list[n] != پوچ; n++) { /* void */ }

        /* MP: ngx_shared_palloc() */

        برای (i = 0; i < n; i++) {
            sin.sin_addr.s_addr = *(in_addr_t *) (h->h_addr_list[i]);

            اگر (ngx_inet_add_addr(pool, u, (ساختار sockaddr *) &sin,
                                  اندازه(ساختار sockaddr_in), n)
                != NGX_OK)
            {
                بازگشت NGX_ERROR;
            }
        }

    } وگرنه {

        /* MP: ngx_shared_palloc() */

        اگر (ngx_inet_add_addr(pool, u, (ساختار sockaddr *) &sin,
                              اندازه(ساختار sockaddr_in), 1)
            != NGX_OK)
        {
            بازگشت NGX_ERROR;
        }
    }

    بازگشت NGX_OK;
}

#endif /* NGX_HAVE_GETADDRINFO && NGX_HAVE_INET6 */


ایستا ngx_int_t
ngx_inet_add_addr(ngx_pool_t *pool, ngx_url_t *u, ساختار sockaddr *sockaddr,
    socklen_t socklen, ngx_uint_t total)
{
    u_char           *p;
    size_t            len;
    ngx_uint_t        i, nports;
    ngx_addr_t       *addr;
    ساختار sockaddr  *sa;

    nports = u->last_port ? u->last_port - u->port + 1 : 1;

    اگر (u->addrs == پوچ) {
        u->addrs = ngx_palloc(pool, total * nports * اندازه(ngx_addr_t));
        اگر (u->addrs == پوچ) {
            بازگشت NGX_ERROR;
        }
    }

    برای (i = 0; i < nports; i++) {
        sa = ngx_pcalloc(pool, socklen);
        اگر (sa == پوچ) {
            بازگشت NGX_ERROR;
        }

        ngx_memcpy(sa, sockaddr, socklen);

        ngx_inet_set_port(sa, u->port + i);

        انتخاب (sa->sa_family) {

#اگر (NGX_HAVE_INET6)
        حالت AF_INET6:
            len = NGX_INET6_ADDRSTRLEN + اندازه("[]:65536") - 1;
            بشکن;
#endif

        پیشفرض: /* AF_INET */
            len = NGX_INET_ADDRSTRLEN + اندازه(":65535") - 1;
        }

        p = ngx_pnalloc(pool, len);
        اگر (p == پوچ) {
            بازگشت NGX_ERROR;
        }

        len = ngx_sock_ntop(sa, socklen, p, len, 1);

        addr = &u->addrs[u->naddrs++];

        addr->sockaddr = sa;
        addr->socklen = socklen;

        addr->name.len = len;
        addr->name.data = p;
    }

    بازگشت NGX_OK;
}


ngx_int_t
ngx_cmp_sockaddr(ساختار sockaddr *sa1, socklen_t slen1,
    ساختار sockaddr *sa2, socklen_t slen2, ngx_uint_t cmp_port)
{
    ساختار sockaddr_in   *sin1, *sin2;
#اگر (NGX_HAVE_INET6)
    ساختار sockaddr_in6  *sin61, *sin62;
#endif
#اگر (NGX_HAVE_UNIX_DOMAIN)
    size_t                len;
    ساختار sockaddr_un   *saun1, *saun2;
#endif

    اگر (sa1->sa_family != sa2->sa_family) {
        بازگشت NGX_DECLINED;
    }

    انتخاب (sa1->sa_family) {

#اگر (NGX_HAVE_INET6)
    حالت AF_INET6:

        sin61 = (ساختار sockaddr_in6 *) sa1;
        sin62 = (ساختار sockaddr_in6 *) sa2;

        اگر (cmp_port && sin61->sin6_port != sin62->sin6_port) {
            بازگشت NGX_DECLINED;
        }

        اگر (ngx_memcmp(&sin61->sin6_addr, &sin62->sin6_addr, 16) != 0) {
            بازگشت NGX_DECLINED;
        }

        بشکن;
#endif

#اگر (NGX_HAVE_UNIX_DOMAIN)
    حالت AF_UNIX:

        saun1 = (ساختار sockaddr_un *) sa1;
        saun2 = (ساختار sockaddr_un *) sa2;

        اگر (slen1 < slen2) {
            len = slen1 - offsetof(ساختار sockaddr_un, sun_path);

        } وگرنه {
            len = slen2 - offsetof(ساختار sockaddr_un, sun_path);
        }

        اگر (len > اندازه(saun1->sun_path)) {
            len = اندازه(saun1->sun_path);
        }

        اگر (ngx_memcmp(&saun1->sun_path, &saun2->sun_path, len) != 0) {
            بازگشت NGX_DECLINED;
        }

        بشکن;
#endif

    پیشفرض: /* AF_INET */

        sin1 = (ساختار sockaddr_in *) sa1;
        sin2 = (ساختار sockaddr_in *) sa2;

        اگر (cmp_port && sin1->sin_port != sin2->sin_port) {
            بازگشت NGX_DECLINED;
        }

        اگر (sin1->sin_addr.s_addr != sin2->sin_addr.s_addr) {
            بازگشت NGX_DECLINED;
        }

        بشکن;
    }

    بازگشت NGX_OK;
}


in_port_t
ngx_inet_get_port(ساختار sockaddr *sa)
{
    ساختار sockaddr_in   *sin;
#اگر (NGX_HAVE_INET6)
    ساختار sockaddr_in6  *sin6;
#endif

    انتخاب (sa->sa_family) {

#اگر (NGX_HAVE_INET6)
    حالت AF_INET6:
        sin6 = (ساختار sockaddr_in6 *) sa;
        بازگشت ntohs(sin6->sin6_port);
#endif

#اگر (NGX_HAVE_UNIX_DOMAIN)
    حالت AF_UNIX:
        بازگشت 0;
#endif

    پیشفرض: /* AF_INET */
        sin = (ساختار sockaddr_in *) sa;
        بازگشت ntohs(sin->sin_port);
    }
}


تهی
ngx_inet_set_port(ساختار sockaddr *sa, in_port_t port)
{
    ساختار sockaddr_in   *sin;
#اگر (NGX_HAVE_INET6)
    ساختار sockaddr_in6  *sin6;
#endif

    انتخاب (sa->sa_family) {

#اگر (NGX_HAVE_INET6)
    حالت AF_INET6:
        sin6 = (ساختار sockaddr_in6 *) sa;
        sin6->sin6_port = htons(port);
        بشکن;
#endif

#اگر (NGX_HAVE_UNIX_DOMAIN)
    حالت AF_UNIX:
        بشکن;
#endif

    پیشفرض: /* AF_INET */
        sin = (ساختار sockaddr_in *) sa;
        sin->sin_port = htons(port);
        بشکن;
    }
}


ngx_uint_t
ngx_inet_wildcard(ساختار sockaddr *sa)
{
    ساختار sockaddr_in   *sin;
#اگر (NGX_HAVE_INET6)
    ساختار sockaddr_in6  *sin6;
#endif

    انتخاب (sa->sa_family) {

    حالت AF_INET:
        sin = (ساختار sockaddr_in *) sa;

        اگر (sin->sin_addr.s_addr == INADDR_ANY) {
            بازگشت 1;
        }

        بشکن;

#اگر (NGX_HAVE_INET6)

    حالت AF_INET6:
        sin6 = (ساختار sockaddr_in6 *) sa;

        اگر (IN6_IS_ADDR_UNSPECIFIED(&sin6->sin6_addr)) {
            بازگشت 1;
        }

        بشکن;

#endif
    }

    بازگشت 0;
}
