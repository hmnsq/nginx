
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#অন্তর্ভাব <ngx_config.h>
#অন্তর্ভাব <ngx_core.h>


স্থির ngx_int_t ngx_parse_unix_domain_url(ngx_pool_t *pool, ngx_url_t *u);
স্থির ngx_int_t ngx_parse_inet_url(ngx_pool_t *pool, ngx_url_t *u);
স্থির ngx_int_t ngx_parse_inet6_url(ngx_pool_t *pool, ngx_url_t *u);
স্থির ngx_int_t ngx_inet_add_addr(ngx_pool_t *pool, ngx_url_t *u,
    সংরচনা sockaddr *sockaddr, socklen_t socklen, ngx_uint_t total);


in_addr_t
ngx_inet_addr(u_char *text, size_t len)
{
    u_char      *p, c;
    in_addr_t    addr;
    ngx_uint_t   octet, n;

    addr = 0;
    octet = 0;
    n = 0;

    প্রতি (p = text; p < text + len; p++) {
        c = *p;

        যদি (c >= '0' && c <= '9') {
            octet = octet * 10 + (c - '0');

            যদি (octet > 255) {
                প্রত্যাগম INADDR_NONE;
            }

            অনুবর্ত;
        }

        যদি (c == '.') {
            addr = (addr << 8) + octet;
            octet = 0;
            n++;
            অনুবর্ত;
        }

        প্রত্যাগম INADDR_NONE;
    }

    যদি (n == 3) {
        addr = (addr << 8) + octet;
        প্রত্যাগম htonl(addr);
    }

    প্রত্যাগম INADDR_NONE;
}


#যদি (NGX_HAVE_INET6)

ngx_int_t
ngx_inet6_addr(u_char *p, size_t len, u_char *addr)
{
    u_char      c, *zero, *digit, *s, *d;
    size_t      len4;
    ngx_uint_t  n, nibbles, word;

    যদি (len == 0) {
        প্রত্যাগম NGX_ERROR;
    }

    zero = রিক্ত;
    digit = রিক্ত;
    len4 = 0;
    nibbles = 0;
    word = 0;
    n = 8;

    যদি (p[0] == ':') {
        p++;
        len--;
    }

    প্রতি (/* void */; len; len--) {
        c = *p++;

        যদি (c == ':') {
            যদি (nibbles) {
                digit = p;
                len4 = len;
                *addr++ = (u_char) (word >> 8);
                *addr++ = (u_char) (word & 0xff);

                যদি (--n) {
                    nibbles = 0;
                    word = 0;
                    অনুবর্ত;
                }

            } অন্যথা {
                যদি (zero == রিক্ত) {
                    digit = p;
                    len4 = len;
                    zero = addr;
                    অনুবর্ত;
                }
            }

            প্রত্যাগম NGX_ERROR;
        }

        যদি (c == '.' && nibbles) {
            যদি (n < 2 || digit == রিক্ত) {
                প্রত্যাগম NGX_ERROR;
            }

            word = ngx_inet_addr(digit, len4 - 1);
            যদি (word == INADDR_NONE) {
                প্রত্যাগম NGX_ERROR;
            }

            word = ntohl(word);
            *addr++ = (u_char) ((word >> 24) & 0xff);
            *addr++ = (u_char) ((word >> 16) & 0xff);
            n--;
            ভঙ্গ;
        }

        যদি (++nibbles > 4) {
            প্রত্যাগম NGX_ERROR;
        }

        যদি (c >= '0' && c <= '9') {
            word = word * 16 + (c - '0');
            অনুবর্ত;
        }

        c |= 0x20;

        যদি (c >= 'a' && c <= 'f') {
            word = word * 16 + (c - 'a') + 10;
            অনুবর্ত;
        }

        প্রত্যাগম NGX_ERROR;
    }

    যদি (nibbles == 0 && zero == রিক্ত) {
        প্রত্যাগম NGX_ERROR;
    }

    *addr++ = (u_char) (word >> 8);
    *addr++ = (u_char) (word & 0xff);

    যদি (--n) {
        যদি (zero) {
            n *= 2;
            s = addr - 1;
            d = s + n;
            যাবত্ (s >= zero) {
                *d-- = *s--;
            }
            ngx_memzero(zero, n);
            প্রত্যাগম NGX_OK;
        }

    } অন্যথা {
        যদি (zero == রিক্ত) {
            প্রত্যাগম NGX_OK;
        }
    }

    প্রত্যাগম NGX_ERROR;
}

#endif


size_t
ngx_sock_ntop(সংরচনা sockaddr *sa, socklen_t socklen, u_char *text, size_t len,
    ngx_uint_t port)
{
    u_char               *p;
#যদি (NGX_HAVE_INET6 || NGX_HAVE_UNIX_DOMAIN)
    size_t                n;
#endif
    সংরচনা sockaddr_in   *sin;
#যদি (NGX_HAVE_INET6)
    সংরচনা sockaddr_in6  *sin6;
#endif
#যদি (NGX_HAVE_UNIX_DOMAIN)
    সংরচনা sockaddr_un   *saun;
#endif

    বিকল্প (sa->sa_family) {

    পক্ষ AF_INET:

        sin = (সংরচনা sockaddr_in *) sa;
        p = (u_char *) &sin->sin_addr;

        যদি (port) {
            p = ngx_snprintf(text, len, "%ud.%ud.%ud.%ud:%d",
                             p[0], p[1], p[2], p[3], ntohs(sin->sin_port));
        } অন্যথা {
            p = ngx_snprintf(text, len, "%ud.%ud.%ud.%ud",
                             p[0], p[1], p[2], p[3]);
        }

        প্রত্যাগম (p - text);

#যদি (NGX_HAVE_INET6)

    পক্ষ AF_INET6:

        sin6 = (সংরচনা sockaddr_in6 *) sa;

        n = 0;

        যদি (port) {
            text[n++] = '[';
        }

        n = ngx_inet6_ntop(sin6->sin6_addr.s6_addr, &text[n], len);

        যদি (port) {
            n = ngx_sprintf(&text[1 + n], "]:%d",
                            ntohs(sin6->sin6_port)) - text;
        }

        প্রত্যাগম n;
#endif

#যদি (NGX_HAVE_UNIX_DOMAIN)

    পক্ষ AF_UNIX:
        saun = (সংরচনা sockaddr_un *) sa;

        /* on Linux sockaddr might not include sun_path at all */

        যদি (socklen <= (socklen_t) offsetof(সংরচনা sockaddr_un, sun_path)) {
            p = ngx_snprintf(text, len, "unix:%Z");

        } অন্যথা {
            n = ngx_strnlen((u_char *) saun->sun_path,
                            socklen - offsetof(সংরচনা sockaddr_un, sun_path));
            p = ngx_snprintf(text, len, "unix:%*s%Z", n, saun->sun_path);
        }

        /* we do not include trailing zero in address length */

        প্রত্যাগম (p - text - 1);

#endif

    স্বভাব:
        প্রত্যাগম 0;
    }
}


size_t
ngx_inet_ntop(পূর্ণাঙ্ক family, শূন্য *addr, u_char *text, size_t len)
{
    u_char  *p;

    বিকল্প (family) {

    পক্ষ AF_INET:

        p = addr;

        প্রত্যাগম ngx_snprintf(text, len, "%ud.%ud.%ud.%ud",
                            p[0], p[1], p[2], p[3])
               - text;

#যদি (NGX_HAVE_INET6)

    পক্ষ AF_INET6:
        প্রত্যাগম ngx_inet6_ntop(addr, text, len);

#endif

    স্বভাব:
        প্রত্যাগম 0;
    }
}


#যদি (NGX_HAVE_INET6)

size_t
ngx_inet6_ntop(u_char *p, u_char *text, size_t len)
{
    u_char      *dst;
    size_t       max, n;
    ngx_uint_t   i, zero, last;

    যদি (len < NGX_INET6_ADDRSTRLEN) {
        প্রত্যাগম 0;
    }

    zero = (ngx_uint_t) -1;
    last = (ngx_uint_t) -1;
    max = 1;
    n = 0;

    প্রতি (i = 0; i < 16; i += 2) {

        যদি (p[i] || p[i + 1]) {

            যদি (max < n) {
                zero = last;
                max = n;
            }

            n = 0;
            অনুবর্ত;
        }

        যদি (n++ == 0) {
            last = i;
        }
    }

    যদি (max < n) {
        zero = last;
        max = n;
    }

    dst = text;
    n = 16;

    যদি (zero == 0) {

        যদি ((max == 5 && p[10] == 0xff && p[11] == 0xff)
            || (max == 6)
            || (max == 7 && p[14] != 0 && p[15] != 1))
        {
            n = 12;
        }

        *dst++ = ':';
    }

    প্রতি (i = 0; i < n; i += 2) {

        যদি (i == zero) {
            *dst++ = ':';
            i += (max - 1) * 2;
            অনুবর্ত;
        }

        dst = ngx_sprintf(dst, "%xd", p[i] * 256 + p[i + 1]);

        যদি (i < 14) {
            *dst++ = ':';
        }
    }

    যদি (n == 12) {
        dst = ngx_sprintf(dst, "%ud.%ud.%ud.%ud", p[12], p[13], p[14], p[15]);
    }

    প্রত্যাগম dst - text;
}

#endif


ngx_int_t
ngx_ptocidr(ngx_str_t *text, ngx_cidr_t *cidr)
{
    u_char      *addr, *mask, *last;
    size_t       len;
    ngx_int_t    shift;
#যদি (NGX_HAVE_INET6)
    ngx_int_t    rc;
    ngx_uint_t   s, i;
#endif

    addr = text->data;
    last = addr + text->len;

    mask = ngx_strlchr(addr, last, '/');
    len = (mask ? mask : last) - addr;

    cidr->u.in.addr = ngx_inet_addr(addr, len);

    যদি (cidr->u.in.addr != INADDR_NONE) {
        cidr->family = AF_INET;

        যদি (mask == রিক্ত) {
            cidr->u.in.mask = 0xffffffff;
            প্রত্যাগম NGX_OK;
        }

#যদি (NGX_HAVE_INET6)
    } অন্যথা যদি (ngx_inet6_addr(addr, len, cidr->u.in6.addr.s6_addr) == NGX_OK) {
        cidr->family = AF_INET6;

        যদি (mask == রিক্ত) {
            ngx_memset(cidr->u.in6.mask.s6_addr, 0xff, 16);
            প্রত্যাগম NGX_OK;
        }

#endif
    } অন্যথা {
        প্রত্যাগম NGX_ERROR;
    }

    mask++;

    shift = ngx_atoi(mask, last - mask);
    যদি (shift == NGX_ERROR) {
        প্রত্যাগম NGX_ERROR;
    }

    বিকল্প (cidr->family) {

#যদি (NGX_HAVE_INET6)
    পক্ষ AF_INET6:
        যদি (shift > 128) {
            প্রত্যাগম NGX_ERROR;
        }

        addr = cidr->u.in6.addr.s6_addr;
        mask = cidr->u.in6.mask.s6_addr;
        rc = NGX_OK;

        প্রতি (i = 0; i < 16; i++) {

            s = (shift > 8) ? 8 : shift;
            shift -= s;

            mask[i] = (u_char) (0xffu << (8 - s));

            যদি (addr[i] != (addr[i] & mask[i])) {
                rc = NGX_DONE;
                addr[i] &= mask[i];
            }
        }

        প্রত্যাগম rc;
#endif

    স্বভাব: /* AF_INET */
        যদি (shift > 32) {
            প্রত্যাগম NGX_ERROR;
        }

        যদি (shift) {
            cidr->u.in.mask = htonl((uint32_t) (0xffffffffu << (32 - shift)));

        } অন্যথা {
            /* x86 compilers use a shl instruction that shifts by modulo 32 */
            cidr->u.in.mask = 0;
        }

        যদি (cidr->u.in.addr == (cidr->u.in.addr & cidr->u.in.mask)) {
            প্রত্যাগম NGX_OK;
        }

        cidr->u.in.addr &= cidr->u.in.mask;

        প্রত্যাগম NGX_DONE;
    }
}


ngx_int_t
ngx_cidr_match(সংরচনা sockaddr *sa, ngx_array_t *cidrs)
{
#যদি (NGX_HAVE_INET6)
    u_char           *p;
#endif
    in_addr_t         inaddr;
    ngx_cidr_t       *cidr;
    ngx_uint_t        family, i;
#যদি (NGX_HAVE_INET6)
    ngx_uint_t        n;
    সংরচনা in6_addr  *inaddr6;
#endif

#যদি (NGX_SUPPRESS_WARN)
    inaddr = 0;
#যদি (NGX_HAVE_INET6)
    inaddr6 = রিক্ত;
#endif
#endif

    family = sa->sa_family;

    যদি (family == AF_INET) {
        inaddr = ((সংরচনা sockaddr_in *) sa)->sin_addr.s_addr;
    }

#যদি (NGX_HAVE_INET6)
    অন্যথা যদি (family == AF_INET6) {
        inaddr6 = &((সংরচনা sockaddr_in6 *) sa)->sin6_addr;

        যদি (IN6_IS_ADDR_V4MAPPED(inaddr6)) {
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

    প্রতি (cidr = cidrs->elts, i = 0; i < cidrs->nelts; i++) {
        যদি (cidr[i].family != family) {
            গচ্ছ next;
        }

        বিকল্প (family) {

#যদি (NGX_HAVE_INET6)
        পক্ষ AF_INET6:
            প্রতি (n = 0; n < 16; n++) {
                যদি ((inaddr6->s6_addr[n] & cidr[i].u.in6.mask.s6_addr[n])
                    != cidr[i].u.in6.addr.s6_addr[n])
                {
                    গচ্ছ next;
                }
            }
            ভঙ্গ;
#endif

#যদি (NGX_HAVE_UNIX_DOMAIN)
        পক্ষ AF_UNIX:
            ভঙ্গ;
#endif

        স্বভাব: /* AF_INET */
            যদি ((inaddr & cidr[i].u.in.mask) != cidr[i].u.in.addr) {
                গচ্ছ next;
            }
            ভঙ্গ;
        }

        প্রত্যাগম NGX_OK;

    next:
        অনুবর্ত;
    }

    প্রত্যাগম NGX_DECLINED;
}


ngx_int_t
ngx_parse_addr(ngx_pool_t *pool, ngx_addr_t *addr, u_char *text, size_t len)
{
    in_addr_t             inaddr;
    ngx_uint_t            family;
    সংরচনা sockaddr_in   *sin;
#যদি (NGX_HAVE_INET6)
    সংরচনা in6_addr       inaddr6;
    সংরচনা sockaddr_in6  *sin6;

    /*
     * prevent MSVC8 warning:
     *    potentially uninitialized local variable 'inaddr6' used
     */
    ngx_memzero(&inaddr6, পরিমাণ(সংরচনা in6_addr));
#endif

    inaddr = ngx_inet_addr(text, len);

    যদি (inaddr != INADDR_NONE) {
        family = AF_INET;
        len = পরিমাণ(সংরচনা sockaddr_in);

#যদি (NGX_HAVE_INET6)
    } অন্যথা যদি (ngx_inet6_addr(text, len, inaddr6.s6_addr) == NGX_OK) {
        family = AF_INET6;
        len = পরিমাণ(সংরচনা sockaddr_in6);

#endif
    } অন্যথা {
        প্রত্যাগম NGX_DECLINED;
    }

    addr->sockaddr = ngx_pcalloc(pool, len);
    যদি (addr->sockaddr == রিক্ত) {
        প্রত্যাগম NGX_ERROR;
    }

    addr->sockaddr->sa_family = (u_char) family;
    addr->socklen = len;

    বিকল্প (family) {

#যদি (NGX_HAVE_INET6)
    পক্ষ AF_INET6:
        sin6 = (সংরচনা sockaddr_in6 *) addr->sockaddr;
        ngx_memcpy(sin6->sin6_addr.s6_addr, inaddr6.s6_addr, 16);
        ভঙ্গ;
#endif

    স্বভাব: /* AF_INET */
        sin = (সংরচনা sockaddr_in *) addr->sockaddr;
        sin->sin_addr.s_addr = inaddr;
        ভঙ্গ;
    }

    প্রত্যাগম NGX_OK;
}


ngx_int_t
ngx_parse_addr_port(ngx_pool_t *pool, ngx_addr_t *addr, u_char *text,
    size_t len)
{
    u_char     *p, *last;
    size_t      plen;
    ngx_int_t   rc, port;

    rc = ngx_parse_addr(pool, addr, text, len);

    যদি (rc != NGX_DECLINED) {
        প্রত্যাগম rc;
    }

    last = text + len;

#যদি (NGX_HAVE_INET6)
    যদি (len && text[0] == '[') {

        p = ngx_strlchr(text, last, ']');

        যদি (p == last - 1) {
            প্রত্যাগম ngx_parse_addr(pool, addr, text + 1, len - 2);
        }

        যদি (p == রিক্ত || *++p != ':') {
            প্রত্যাগম NGX_DECLINED;
        }

        text++;
        len -= 2;

    } অন্যথা
#endif

    {
        p = ngx_strlchr(text, last, ':');

        যদি (p == রিক্ত) {
            প্রত্যাগম NGX_DECLINED;
        }
    }

    p++;
    plen = last - p;

    port = ngx_atoi(p, plen);

    যদি (port < 1 || port > 65535) {
        প্রত্যাগম NGX_DECLINED;
    }

    len -= plen + 1;

    rc = ngx_parse_addr(pool, addr, text, len);

    যদি (rc != NGX_OK) {
        প্রত্যাগম rc;
    }

    ngx_inet_set_port(addr->sockaddr, (in_port_t) port);

    প্রত্যাগম NGX_OK;
}


ngx_int_t
ngx_parse_url(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char  *p;
    size_t   len;

    p = u->url.data;
    len = u->url.len;

    যদি (len >= 5 && ngx_strncasecmp(p, (u_char *) "unix:", 5) == 0) {
        প্রত্যাগম ngx_parse_unix_domain_url(pool, u);
    }

    যদি (len && p[0] == '[') {
        প্রত্যাগম ngx_parse_inet6_url(pool, u);
    }

    প্রত্যাগম ngx_parse_inet_url(pool, u);
}


স্থির ngx_int_t
ngx_parse_unix_domain_url(ngx_pool_t *pool, ngx_url_t *u)
{
#যদি (NGX_HAVE_UNIX_DOMAIN)
    u_char              *path, *uri, *last;
    size_t               len;
    সংরচনা sockaddr_un  *saun;

    len = u->url.len;
    path = u->url.data;

    path += 5;
    len -= 5;

    যদি (u->uri_part) {

        last = path + len;
        uri = ngx_strlchr(path, last, ':');

        যদি (uri) {
            len = uri - path;
            uri++;
            u->uri.len = last - uri;
            u->uri.data = uri;
        }
    }

    যদি (len == 0) {
        u->err = "no path in the unix domain socket";
        প্রত্যাগম NGX_ERROR;
    }

    u->host.len = len++;
    u->host.data = path;

    যদি (len > পরিমাণ(saun->sun_path)) {
        u->err = "too long path in the unix domain socket";
        প্রত্যাগম NGX_ERROR;
    }

    u->socklen = পরিমাণ(সংরচনা sockaddr_un);
    saun = (সংরচনা sockaddr_un *) &u->sockaddr;
    saun->sun_family = AF_UNIX;
    (শূন্য) ngx_cpystrn((u_char *) saun->sun_path, path, len);

    u->addrs = ngx_pcalloc(pool, পরিমাণ(ngx_addr_t));
    যদি (u->addrs == রিক্ত) {
        প্রত্যাগম NGX_ERROR;
    }

    saun = ngx_pcalloc(pool, পরিমাণ(সংরচনা sockaddr_un));
    যদি (saun == রিক্ত) {
        প্রত্যাগম NGX_ERROR;
    }

    u->family = AF_UNIX;
    u->naddrs = 1;

    saun->sun_family = AF_UNIX;
    (শূন্য) ngx_cpystrn((u_char *) saun->sun_path, path, len);

    u->addrs[0].sockaddr = (সংরচনা sockaddr *) saun;
    u->addrs[0].socklen = পরিমাণ(সংরচনা sockaddr_un);
    u->addrs[0].name.len = len + 4;
    u->addrs[0].name.data = u->url.data;

    প্রত্যাগম NGX_OK;

#অন্যথা

    u->err = "the unix domain sockets are not supported on this platform";

    প্রত্যাগম NGX_ERROR;

#endif
}


স্থির ngx_int_t
ngx_parse_inet_url(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char              *host, *port, *last, *uri, *args, *dash;
    size_t               len;
    ngx_int_t            n;
    সংরচনা sockaddr_in  *sin;

    u->socklen = পরিমাণ(সংরচনা sockaddr_in);
    sin = (সংরচনা sockaddr_in *) &u->sockaddr;
    sin->sin_family = AF_INET;

    u->family = AF_INET;

    host = u->url.data;

    last = host + u->url.len;

    port = ngx_strlchr(host, last, ':');

    uri = ngx_strlchr(host, last, '/');

    args = ngx_strlchr(host, last, '?');

    যদি (args) {
        যদি (uri == রিক্ত || args < uri) {
            uri = args;
        }
    }

    যদি (uri) {
        যদি (u->listen || !u->uri_part) {
            u->err = "invalid host";
            প্রত্যাগম NGX_ERROR;
        }

        u->uri.len = last - uri;
        u->uri.data = uri;

        last = uri;

        যদি (uri < port) {
            port = রিক্ত;
        }
    }

    যদি (port) {
        port++;

        len = last - port;

        যদি (u->listen) {
            dash = ngx_strlchr(port, last, '-');

            যদি (dash) {
                dash++;

                n = ngx_atoi(dash, last - dash);

                যদি (n < 1 || n > 65535) {
                    u->err = "invalid port";
                    প্রত্যাগম NGX_ERROR;
                }

                u->last_port = (in_port_t) n;

                len = dash - port - 1;
            }
        }

        n = ngx_atoi(port, len);

        যদি (n < 1 || n > 65535) {
            u->err = "invalid port";
            প্রত্যাগম NGX_ERROR;
        }

        যদি (u->last_port && n > u->last_port) {
            u->err = "invalid port range";
            প্রত্যাগম NGX_ERROR;
        }

        u->port = (in_port_t) n;
        sin->sin_port = htons((in_port_t) n);

        u->port_text.len = last - port;
        u->port_text.data = port;

        last = port - 1;

    } অন্যথা {
        যদি (uri == রিক্ত) {

            যদি (u->listen) {

                /* test value as port only */

                len = last - host;

                dash = ngx_strlchr(host, last, '-');

                যদি (dash) {
                    dash++;

                    n = ngx_atoi(dash, last - dash);

                    যদি (n == NGX_ERROR) {
                        গচ্ছ no_port;
                    }

                    যদি (n < 1 || n > 65535) {
                        u->err = "invalid port";

                    } অন্যথা {
                        u->last_port = (in_port_t) n;
                    }

                    len = dash - host - 1;
                }

                n = ngx_atoi(host, len);

                যদি (n != NGX_ERROR) {

                    যদি (u->err) {
                        প্রত্যাগম NGX_ERROR;
                    }

                    যদি (n < 1 || n > 65535) {
                        u->err = "invalid port";
                        প্রত্যাগম NGX_ERROR;
                    }

                    যদি (u->last_port && n > u->last_port) {
                        u->err = "invalid port range";
                        প্রত্যাগম NGX_ERROR;
                    }

                    u->port = (in_port_t) n;
                    sin->sin_port = htons((in_port_t) n);
                    sin->sin_addr.s_addr = INADDR_ANY;

                    u->port_text.len = last - host;
                    u->port_text.data = host;

                    u->wildcard = 1;

                    প্রত্যাগম ngx_inet_add_addr(pool, u, &u->sockaddr.sockaddr,
                                             u->socklen, 1);
                }
            }
        }

no_port:

        u->err = রিক্ত;
        u->no_port = 1;
        u->port = u->default_port;
        sin->sin_port = htons(u->default_port);
        u->last_port = 0;
    }

    len = last - host;

    যদি (len == 0) {
        u->err = "no host";
        প্রত্যাগম NGX_ERROR;
    }

    u->host.len = len;
    u->host.data = host;

    যদি (u->listen && len == 1 && *host == '*') {
        sin->sin_addr.s_addr = INADDR_ANY;
        u->wildcard = 1;
        প্রত্যাগম ngx_inet_add_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);
    }

    sin->sin_addr.s_addr = ngx_inet_addr(host, len);

    যদি (sin->sin_addr.s_addr != INADDR_NONE) {

        যদি (sin->sin_addr.s_addr == INADDR_ANY) {
            u->wildcard = 1;
        }

        প্রত্যাগম ngx_inet_add_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);
    }

    যদি (u->no_resolve) {
        প্রত্যাগম NGX_OK;
    }

    যদি (ngx_inet_resolve_host(pool, u) != NGX_OK) {
        প্রত্যাগম NGX_ERROR;
    }

    u->family = u->addrs[0].sockaddr->sa_family;
    u->socklen = u->addrs[0].socklen;
    ngx_memcpy(&u->sockaddr, u->addrs[0].sockaddr, u->addrs[0].socklen);
    u->wildcard = ngx_inet_wildcard(&u->sockaddr.sockaddr);

    প্রত্যাগম NGX_OK;
}


স্থির ngx_int_t
ngx_parse_inet6_url(ngx_pool_t *pool, ngx_url_t *u)
{
#যদি (NGX_HAVE_INET6)
    u_char               *p, *host, *port, *last, *uri, *dash;
    size_t                len;
    ngx_int_t             n;
    সংরচনা sockaddr_in6  *sin6;

    u->socklen = পরিমাণ(সংরচনা sockaddr_in6);
    sin6 = (সংরচনা sockaddr_in6 *) &u->sockaddr;
    sin6->sin6_family = AF_INET6;

    host = u->url.data + 1;

    last = u->url.data + u->url.len;

    p = ngx_strlchr(host, last, ']');

    যদি (p == রিক্ত) {
        u->err = "invalid host";
        প্রত্যাগম NGX_ERROR;
    }

    port = p + 1;

    uri = ngx_strlchr(port, last, '/');

    যদি (uri) {
        যদি (u->listen || !u->uri_part) {
            u->err = "invalid host";
            প্রত্যাগম NGX_ERROR;
        }

        u->uri.len = last - uri;
        u->uri.data = uri;

        last = uri;
    }

    যদি (port < last) {
        যদি (*port != ':') {
            u->err = "invalid host";
            প্রত্যাগম NGX_ERROR;
        }

        port++;

        len = last - port;

        যদি (u->listen) {
            dash = ngx_strlchr(port, last, '-');

            যদি (dash) {
                dash++;

                n = ngx_atoi(dash, last - dash);

                যদি (n < 1 || n > 65535) {
                    u->err = "invalid port";
                    প্রত্যাগম NGX_ERROR;
                }

                u->last_port = (in_port_t) n;

                len = dash - port - 1;
            }
        }

        n = ngx_atoi(port, len);

        যদি (n < 1 || n > 65535) {
            u->err = "invalid port";
            প্রত্যাগম NGX_ERROR;
        }

        যদি (u->last_port && n > u->last_port) {
            u->err = "invalid port range";
            প্রত্যাগম NGX_ERROR;
        }

        u->port = (in_port_t) n;
        sin6->sin6_port = htons((in_port_t) n);

        u->port_text.len = last - port;
        u->port_text.data = port;

    } অন্যথা {
        u->no_port = 1;
        u->port = u->default_port;
        sin6->sin6_port = htons(u->default_port);
    }

    len = p - host;

    যদি (len == 0) {
        u->err = "no host";
        প্রত্যাগম NGX_ERROR;
    }

    u->host.len = len + 2;
    u->host.data = host - 1;

    যদি (ngx_inet6_addr(host, len, sin6->sin6_addr.s6_addr) != NGX_OK) {
        u->err = "invalid IPv6 address";
        প্রত্যাগম NGX_ERROR;
    }

    যদি (IN6_IS_ADDR_UNSPECIFIED(&sin6->sin6_addr)) {
        u->wildcard = 1;
    }

    u->family = AF_INET6;

    প্রত্যাগম ngx_inet_add_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);

#অন্যথা

    u->err = "the INET6 sockets are not supported on this platform";

    প্রত্যাগম NGX_ERROR;

#endif
}


#যদি (NGX_HAVE_GETADDRINFO && NGX_HAVE_INET6)

ngx_int_t
ngx_inet_resolve_host(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char           *host;
    ngx_uint_t        n;
    সংরচনা addrinfo   hints, *res, *rp;

    host = ngx_alloc(u->host.len + 1, pool->log);
    যদি (host == রিক্ত) {
        প্রত্যাগম NGX_ERROR;
    }

    (শূন্য) ngx_cpystrn(host, u->host.data, u->host.len + 1);

    ngx_memzero(&hints, পরিমাণ(সংরচনা addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
#ifdef AI_ADDRCONFIG
    hints.ai_flags = AI_ADDRCONFIG;
#endif

    যদি (getaddrinfo((বর্ণ *) host, রিক্ত, &hints, &res) != 0) {
        u->err = "host not found";
        ngx_free(host);
        প্রত্যাগম NGX_ERROR;
    }

    ngx_free(host);

    প্রতি (n = 0, rp = res; rp != রিক্ত; rp = rp->ai_next) {

        বিকল্প (rp->ai_family) {

        পক্ষ AF_INET:
        পক্ষ AF_INET6:
            ভঙ্গ;

        স্বভাব:
            অনুবর্ত;
        }

        n++;
    }

    যদি (n == 0) {
        u->err = "host not found";
        গচ্ছ failed;
    }

    /* MP: ngx_shared_palloc() */

    প্রতি (rp = res; rp != রিক্ত; rp = rp->ai_next) {

        বিকল্প (rp->ai_family) {

        পক্ষ AF_INET:
        পক্ষ AF_INET6:
            ভঙ্গ;

        স্বভাব:
            অনুবর্ত;
        }

        যদি (ngx_inet_add_addr(pool, u, rp->ai_addr, rp->ai_addrlen, n)
            != NGX_OK)
        {
            গচ্ছ failed;
        }
    }

    freeaddrinfo(res);
    প্রত্যাগম NGX_OK;

failed:

    freeaddrinfo(res);
    প্রত্যাগম NGX_ERROR;
}

#অন্যথা /* !NGX_HAVE_GETADDRINFO || !NGX_HAVE_INET6 */

ngx_int_t
ngx_inet_resolve_host(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char              *host;
    ngx_uint_t           i, n;
    সংরচনা hostent      *h;
    সংরচনা sockaddr_in   sin;

    /* AF_INET only */

    ngx_memzero(&sin, পরিমাণ(সংরচনা sockaddr_in));

    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = ngx_inet_addr(u->host.data, u->host.len);

    যদি (sin.sin_addr.s_addr == INADDR_NONE) {
        host = ngx_alloc(u->host.len + 1, pool->log);
        যদি (host == রিক্ত) {
            প্রত্যাগম NGX_ERROR;
        }

        (শূন্য) ngx_cpystrn(host, u->host.data, u->host.len + 1);

        h = gethostbyname((বর্ণ *) host);

        ngx_free(host);

        যদি (h == রিক্ত || h->h_addr_list[0] == রিক্ত) {
            u->err = "host not found";
            প্রত্যাগম NGX_ERROR;
        }

        প্রতি (n = 0; h->h_addr_list[n] != রিক্ত; n++) { /* void */ }

        /* MP: ngx_shared_palloc() */

        প্রতি (i = 0; i < n; i++) {
            sin.sin_addr.s_addr = *(in_addr_t *) (h->h_addr_list[i]);

            যদি (ngx_inet_add_addr(pool, u, (সংরচনা sockaddr *) &sin,
                                  পরিমাণ(সংরচনা sockaddr_in), n)
                != NGX_OK)
            {
                প্রত্যাগম NGX_ERROR;
            }
        }

    } অন্যথা {

        /* MP: ngx_shared_palloc() */

        যদি (ngx_inet_add_addr(pool, u, (সংরচনা sockaddr *) &sin,
                              পরিমাণ(সংরচনা sockaddr_in), 1)
            != NGX_OK)
        {
            প্রত্যাগম NGX_ERROR;
        }
    }

    প্রত্যাগম NGX_OK;
}

#endif /* NGX_HAVE_GETADDRINFO && NGX_HAVE_INET6 */


স্থির ngx_int_t
ngx_inet_add_addr(ngx_pool_t *pool, ngx_url_t *u, সংরচনা sockaddr *sockaddr,
    socklen_t socklen, ngx_uint_t total)
{
    u_char           *p;
    size_t            len;
    ngx_uint_t        i, nports;
    ngx_addr_t       *addr;
    সংরচনা sockaddr  *sa;

    nports = u->last_port ? u->last_port - u->port + 1 : 1;

    যদি (u->addrs == রিক্ত) {
        u->addrs = ngx_palloc(pool, total * nports * পরিমাণ(ngx_addr_t));
        যদি (u->addrs == রিক্ত) {
            প্রত্যাগম NGX_ERROR;
        }
    }

    প্রতি (i = 0; i < nports; i++) {
        sa = ngx_pcalloc(pool, socklen);
        যদি (sa == রিক্ত) {
            প্রত্যাগম NGX_ERROR;
        }

        ngx_memcpy(sa, sockaddr, socklen);

        ngx_inet_set_port(sa, u->port + i);

        বিকল্প (sa->sa_family) {

#যদি (NGX_HAVE_INET6)
        পক্ষ AF_INET6:
            len = NGX_INET6_ADDRSTRLEN + পরিমাণ("[]:65536") - 1;
            ভঙ্গ;
#endif

        স্বভাব: /* AF_INET */
            len = NGX_INET_ADDRSTRLEN + পরিমাণ(":65535") - 1;
        }

        p = ngx_pnalloc(pool, len);
        যদি (p == রিক্ত) {
            প্রত্যাগম NGX_ERROR;
        }

        len = ngx_sock_ntop(sa, socklen, p, len, 1);

        addr = &u->addrs[u->naddrs++];

        addr->sockaddr = sa;
        addr->socklen = socklen;

        addr->name.len = len;
        addr->name.data = p;
    }

    প্রত্যাগম NGX_OK;
}


ngx_int_t
ngx_cmp_sockaddr(সংরচনা sockaddr *sa1, socklen_t slen1,
    সংরচনা sockaddr *sa2, socklen_t slen2, ngx_uint_t cmp_port)
{
    সংরচনা sockaddr_in   *sin1, *sin2;
#যদি (NGX_HAVE_INET6)
    সংরচনা sockaddr_in6  *sin61, *sin62;
#endif
#যদি (NGX_HAVE_UNIX_DOMAIN)
    size_t                len;
    সংরচনা sockaddr_un   *saun1, *saun2;
#endif

    যদি (sa1->sa_family != sa2->sa_family) {
        প্রত্যাগম NGX_DECLINED;
    }

    বিকল্প (sa1->sa_family) {

#যদি (NGX_HAVE_INET6)
    পক্ষ AF_INET6:

        sin61 = (সংরচনা sockaddr_in6 *) sa1;
        sin62 = (সংরচনা sockaddr_in6 *) sa2;

        যদি (cmp_port && sin61->sin6_port != sin62->sin6_port) {
            প্রত্যাগম NGX_DECLINED;
        }

        যদি (ngx_memcmp(&sin61->sin6_addr, &sin62->sin6_addr, 16) != 0) {
            প্রত্যাগম NGX_DECLINED;
        }

        ভঙ্গ;
#endif

#যদি (NGX_HAVE_UNIX_DOMAIN)
    পক্ষ AF_UNIX:

        saun1 = (সংরচনা sockaddr_un *) sa1;
        saun2 = (সংরচনা sockaddr_un *) sa2;

        যদি (slen1 < slen2) {
            len = slen1 - offsetof(সংরচনা sockaddr_un, sun_path);

        } অন্যথা {
            len = slen2 - offsetof(সংরচনা sockaddr_un, sun_path);
        }

        যদি (len > পরিমাণ(saun1->sun_path)) {
            len = পরিমাণ(saun1->sun_path);
        }

        যদি (ngx_memcmp(&saun1->sun_path, &saun2->sun_path, len) != 0) {
            প্রত্যাগম NGX_DECLINED;
        }

        ভঙ্গ;
#endif

    স্বভাব: /* AF_INET */

        sin1 = (সংরচনা sockaddr_in *) sa1;
        sin2 = (সংরচনা sockaddr_in *) sa2;

        যদি (cmp_port && sin1->sin_port != sin2->sin_port) {
            প্রত্যাগম NGX_DECLINED;
        }

        যদি (sin1->sin_addr.s_addr != sin2->sin_addr.s_addr) {
            প্রত্যাগম NGX_DECLINED;
        }

        ভঙ্গ;
    }

    প্রত্যাগম NGX_OK;
}


in_port_t
ngx_inet_get_port(সংরচনা sockaddr *sa)
{
    সংরচনা sockaddr_in   *sin;
#যদি (NGX_HAVE_INET6)
    সংরচনা sockaddr_in6  *sin6;
#endif

    বিকল্প (sa->sa_family) {

#যদি (NGX_HAVE_INET6)
    পক্ষ AF_INET6:
        sin6 = (সংরচনা sockaddr_in6 *) sa;
        প্রত্যাগম ntohs(sin6->sin6_port);
#endif

#যদি (NGX_HAVE_UNIX_DOMAIN)
    পক্ষ AF_UNIX:
        প্রত্যাগম 0;
#endif

    স্বভাব: /* AF_INET */
        sin = (সংরচনা sockaddr_in *) sa;
        প্রত্যাগম ntohs(sin->sin_port);
    }
}


শূন্য
ngx_inet_set_port(সংরচনা sockaddr *sa, in_port_t port)
{
    সংরচনা sockaddr_in   *sin;
#যদি (NGX_HAVE_INET6)
    সংরচনা sockaddr_in6  *sin6;
#endif

    বিকল্প (sa->sa_family) {

#যদি (NGX_HAVE_INET6)
    পক্ষ AF_INET6:
        sin6 = (সংরচনা sockaddr_in6 *) sa;
        sin6->sin6_port = htons(port);
        ভঙ্গ;
#endif

#যদি (NGX_HAVE_UNIX_DOMAIN)
    পক্ষ AF_UNIX:
        ভঙ্গ;
#endif

    স্বভাব: /* AF_INET */
        sin = (সংরচনা sockaddr_in *) sa;
        sin->sin_port = htons(port);
        ভঙ্গ;
    }
}


ngx_uint_t
ngx_inet_wildcard(সংরচনা sockaddr *sa)
{
    সংরচনা sockaddr_in   *sin;
#যদি (NGX_HAVE_INET6)
    সংরচনা sockaddr_in6  *sin6;
#endif

    বিকল্প (sa->sa_family) {

    পক্ষ AF_INET:
        sin = (সংরচনা sockaddr_in *) sa;

        যদি (sin->sin_addr.s_addr == INADDR_ANY) {
            প্রত্যাগম 1;
        }

        ভঙ্গ;

#যদি (NGX_HAVE_INET6)

    পক্ষ AF_INET6:
        sin6 = (সংরচনা sockaddr_in6 *) sa;

        যদি (IN6_IS_ADDR_UNSPECIFIED(&sin6->sin6_addr)) {
            প্রত্যাগম 1;
        }

        ভঙ্গ;

#endif
    }

    প্রত্যাগম 0;
}
