
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#અન્તર્ભાવ <ngx_config.h>
#અન્તર્ભાવ <ngx_core.h>


સ્થિર ngx_int_t ngx_parse_unix_domain_url(ngx_pool_t *pool, ngx_url_t *u);
સ્થિર ngx_int_t ngx_parse_inet_url(ngx_pool_t *pool, ngx_url_t *u);
સ્થિર ngx_int_t ngx_parse_inet6_url(ngx_pool_t *pool, ngx_url_t *u);
સ્થિર ngx_int_t ngx_inet_add_addr(ngx_pool_t *pool, ngx_url_t *u,
    સંરચના sockaddr *sockaddr, socklen_t socklen, ngx_uint_t total);


in_addr_t
ngx_inet_addr(u_char *text, size_t len)
{
    u_char      *p, c;
    in_addr_t    addr;
    ngx_uint_t   octet, n;

    addr = 0;
    octet = 0;
    n = 0;

    માટે (p = text; p < text + len; p++) {
        c = *p;

        જો (c >= '0' && c <= '9') {
            octet = octet * 10 + (c - '0');

            જો (octet > 255) {
                પાછા INADDR_NONE;
            }

            ચાલુ;
        }

        જો (c == '.') {
            addr = (addr << 8) + octet;
            octet = 0;
            n++;
            ચાલુ;
        }

        પાછા INADDR_NONE;
    }

    જો (n == 3) {
        addr = (addr << 8) + octet;
        પાછા htonl(addr);
    }

    પાછા INADDR_NONE;
}


#જો (NGX_HAVE_INET6)

ngx_int_t
ngx_inet6_addr(u_char *p, size_t len, u_char *addr)
{
    u_char      c, *zero, *digit, *s, *d;
    size_t      len4;
    ngx_uint_t  n, nibbles, word;

    જો (len == 0) {
        પાછા NGX_ERROR;
    }

    zero = ખાલી;
    digit = ખાલી;
    len4 = 0;
    nibbles = 0;
    word = 0;
    n = 8;

    જો (p[0] == ':') {
        p++;
        len--;
    }

    માટે (/* void */; len; len--) {
        c = *p++;

        જો (c == ':') {
            જો (nibbles) {
                digit = p;
                len4 = len;
                *addr++ = (u_char) (word >> 8);
                *addr++ = (u_char) (word & 0xff);

                જો (--n) {
                    nibbles = 0;
                    word = 0;
                    ચાલુ;
                }

            } નહીંતર {
                જો (zero == ખાલી) {
                    digit = p;
                    len4 = len;
                    zero = addr;
                    ચાલુ;
                }
            }

            પાછા NGX_ERROR;
        }

        જો (c == '.' && nibbles) {
            જો (n < 2 || digit == ખાલી) {
                પાછા NGX_ERROR;
            }

            word = ngx_inet_addr(digit, len4 - 1);
            જો (word == INADDR_NONE) {
                પાછા NGX_ERROR;
            }

            word = ntohl(word);
            *addr++ = (u_char) ((word >> 24) & 0xff);
            *addr++ = (u_char) ((word >> 16) & 0xff);
            n--;
            તોડો;
        }

        જો (++nibbles > 4) {
            પાછા NGX_ERROR;
        }

        જો (c >= '0' && c <= '9') {
            word = word * 16 + (c - '0');
            ચાલુ;
        }

        c |= 0x20;

        જો (c >= 'a' && c <= 'f') {
            word = word * 16 + (c - 'a') + 10;
            ચાલુ;
        }

        પાછા NGX_ERROR;
    }

    જો (nibbles == 0 && zero == ખાલી) {
        પાછા NGX_ERROR;
    }

    *addr++ = (u_char) (word >> 8);
    *addr++ = (u_char) (word & 0xff);

    જો (--n) {
        જો (zero) {
            n *= 2;
            s = addr - 1;
            d = s + n;
            જ્યારે (s >= zero) {
                *d-- = *s--;
            }
            ngx_memzero(zero, n);
            પાછા NGX_OK;
        }

    } નહીંતર {
        જો (zero == ખાલી) {
            પાછા NGX_OK;
        }
    }

    પાછા NGX_ERROR;
}

#endif


size_t
ngx_sock_ntop(સંરચના sockaddr *sa, socklen_t socklen, u_char *text, size_t len,
    ngx_uint_t port)
{
    u_char               *p;
#જો (NGX_HAVE_INET6 || NGX_HAVE_UNIX_DOMAIN)
    size_t                n;
#endif
    સંરચના sockaddr_in   *sin;
#જો (NGX_HAVE_INET6)
    સંરચના sockaddr_in6  *sin6;
#endif
#જો (NGX_HAVE_UNIX_DOMAIN)
    સંરચના sockaddr_un   *saun;
#endif

    પસંદગી (sa->sa_family) {

    સ્થિતિ AF_INET:

        sin = (સંરચના sockaddr_in *) sa;
        p = (u_char *) &sin->sin_addr;

        જો (port) {
            p = ngx_snprintf(text, len, "%ud.%ud.%ud.%ud:%d",
                             p[0], p[1], p[2], p[3], ntohs(sin->sin_port));
        } નહીંતર {
            p = ngx_snprintf(text, len, "%ud.%ud.%ud.%ud",
                             p[0], p[1], p[2], p[3]);
        }

        પાછા (p - text);

#જો (NGX_HAVE_INET6)

    સ્થિતિ AF_INET6:

        sin6 = (સંરચના sockaddr_in6 *) sa;

        n = 0;

        જો (port) {
            text[n++] = '[';
        }

        n = ngx_inet6_ntop(sin6->sin6_addr.s6_addr, &text[n], len);

        જો (port) {
            n = ngx_sprintf(&text[1 + n], "]:%d",
                            ntohs(sin6->sin6_port)) - text;
        }

        પાછા n;
#endif

#જો (NGX_HAVE_UNIX_DOMAIN)

    સ્થિતિ AF_UNIX:
        saun = (સંરચના sockaddr_un *) sa;

        /* on Linux sockaddr might not include sun_path at all */

        જો (socklen <= (socklen_t) offsetof(સંરચના sockaddr_un, sun_path)) {
            p = ngx_snprintf(text, len, "unix:%Z");

        } નહીંતર {
            n = ngx_strnlen((u_char *) saun->sun_path,
                            socklen - offsetof(સંરચના sockaddr_un, sun_path));
            p = ngx_snprintf(text, len, "unix:%*s%Z", n, saun->sun_path);
        }

        /* we do not include trailing zero in address length */

        પાછા (p - text - 1);

#endif

    મૂળભૂત:
        પાછા 0;
    }
}


size_t
ngx_inet_ntop(પૂર્ણાંક family, શૂન્ય *addr, u_char *text, size_t len)
{
    u_char  *p;

    પસંદગી (family) {

    સ્થિતિ AF_INET:

        p = addr;

        પાછા ngx_snprintf(text, len, "%ud.%ud.%ud.%ud",
                            p[0], p[1], p[2], p[3])
               - text;

#જો (NGX_HAVE_INET6)

    સ્થિતિ AF_INET6:
        પાછા ngx_inet6_ntop(addr, text, len);

#endif

    મૂળભૂત:
        પાછા 0;
    }
}


#જો (NGX_HAVE_INET6)

size_t
ngx_inet6_ntop(u_char *p, u_char *text, size_t len)
{
    u_char      *dst;
    size_t       max, n;
    ngx_uint_t   i, zero, last;

    જો (len < NGX_INET6_ADDRSTRLEN) {
        પાછા 0;
    }

    zero = (ngx_uint_t) -1;
    last = (ngx_uint_t) -1;
    max = 1;
    n = 0;

    માટે (i = 0; i < 16; i += 2) {

        જો (p[i] || p[i + 1]) {

            જો (max < n) {
                zero = last;
                max = n;
            }

            n = 0;
            ચાલુ;
        }

        જો (n++ == 0) {
            last = i;
        }
    }

    જો (max < n) {
        zero = last;
        max = n;
    }

    dst = text;
    n = 16;

    જો (zero == 0) {

        જો ((max == 5 && p[10] == 0xff && p[11] == 0xff)
            || (max == 6)
            || (max == 7 && p[14] != 0 && p[15] != 1))
        {
            n = 12;
        }

        *dst++ = ':';
    }

    માટે (i = 0; i < n; i += 2) {

        જો (i == zero) {
            *dst++ = ':';
            i += (max - 1) * 2;
            ચાલુ;
        }

        dst = ngx_sprintf(dst, "%xd", p[i] * 256 + p[i + 1]);

        જો (i < 14) {
            *dst++ = ':';
        }
    }

    જો (n == 12) {
        dst = ngx_sprintf(dst, "%ud.%ud.%ud.%ud", p[12], p[13], p[14], p[15]);
    }

    પાછા dst - text;
}

#endif


ngx_int_t
ngx_ptocidr(ngx_str_t *text, ngx_cidr_t *cidr)
{
    u_char      *addr, *mask, *last;
    size_t       len;
    ngx_int_t    shift;
#જો (NGX_HAVE_INET6)
    ngx_int_t    rc;
    ngx_uint_t   s, i;
#endif

    addr = text->data;
    last = addr + text->len;

    mask = ngx_strlchr(addr, last, '/');
    len = (mask ? mask : last) - addr;

    cidr->u.in.addr = ngx_inet_addr(addr, len);

    જો (cidr->u.in.addr != INADDR_NONE) {
        cidr->family = AF_INET;

        જો (mask == ખાલી) {
            cidr->u.in.mask = 0xffffffff;
            પાછા NGX_OK;
        }

#જો (NGX_HAVE_INET6)
    } નહીંતર જો (ngx_inet6_addr(addr, len, cidr->u.in6.addr.s6_addr) == NGX_OK) {
        cidr->family = AF_INET6;

        જો (mask == ખાલી) {
            ngx_memset(cidr->u.in6.mask.s6_addr, 0xff, 16);
            પાછા NGX_OK;
        }

#endif
    } નહીંતર {
        પાછા NGX_ERROR;
    }

    mask++;

    shift = ngx_atoi(mask, last - mask);
    જો (shift == NGX_ERROR) {
        પાછા NGX_ERROR;
    }

    પસંદગી (cidr->family) {

#જો (NGX_HAVE_INET6)
    સ્થિતિ AF_INET6:
        જો (shift > 128) {
            પાછા NGX_ERROR;
        }

        addr = cidr->u.in6.addr.s6_addr;
        mask = cidr->u.in6.mask.s6_addr;
        rc = NGX_OK;

        માટે (i = 0; i < 16; i++) {

            s = (shift > 8) ? 8 : shift;
            shift -= s;

            mask[i] = (u_char) (0xffu << (8 - s));

            જો (addr[i] != (addr[i] & mask[i])) {
                rc = NGX_DONE;
                addr[i] &= mask[i];
            }
        }

        પાછા rc;
#endif

    મૂળભૂત: /* AF_INET */
        જો (shift > 32) {
            પાછા NGX_ERROR;
        }

        જો (shift) {
            cidr->u.in.mask = htonl((uint32_t) (0xffffffffu << (32 - shift)));

        } નહીંતર {
            /* x86 compilers use a shl instruction that shifts by modulo 32 */
            cidr->u.in.mask = 0;
        }

        જો (cidr->u.in.addr == (cidr->u.in.addr & cidr->u.in.mask)) {
            પાછા NGX_OK;
        }

        cidr->u.in.addr &= cidr->u.in.mask;

        પાછા NGX_DONE;
    }
}


ngx_int_t
ngx_cidr_match(સંરચના sockaddr *sa, ngx_array_t *cidrs)
{
#જો (NGX_HAVE_INET6)
    u_char           *p;
#endif
    in_addr_t         inaddr;
    ngx_cidr_t       *cidr;
    ngx_uint_t        family, i;
#જો (NGX_HAVE_INET6)
    ngx_uint_t        n;
    સંરચના in6_addr  *inaddr6;
#endif

#જો (NGX_SUPPRESS_WARN)
    inaddr = 0;
#જો (NGX_HAVE_INET6)
    inaddr6 = ખાલી;
#endif
#endif

    family = sa->sa_family;

    જો (family == AF_INET) {
        inaddr = ((સંરચના sockaddr_in *) sa)->sin_addr.s_addr;
    }

#જો (NGX_HAVE_INET6)
    નહીંતર જો (family == AF_INET6) {
        inaddr6 = &((સંરચના sockaddr_in6 *) sa)->sin6_addr;

        જો (IN6_IS_ADDR_V4MAPPED(inaddr6)) {
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

    માટે (cidr = cidrs->elts, i = 0; i < cidrs->nelts; i++) {
        જો (cidr[i].family != family) {
            જાઓ next;
        }

        પસંદગી (family) {

#જો (NGX_HAVE_INET6)
        સ્થિતિ AF_INET6:
            માટે (n = 0; n < 16; n++) {
                જો ((inaddr6->s6_addr[n] & cidr[i].u.in6.mask.s6_addr[n])
                    != cidr[i].u.in6.addr.s6_addr[n])
                {
                    જાઓ next;
                }
            }
            તોડો;
#endif

#જો (NGX_HAVE_UNIX_DOMAIN)
        સ્થિતિ AF_UNIX:
            તોડો;
#endif

        મૂળભૂત: /* AF_INET */
            જો ((inaddr & cidr[i].u.in.mask) != cidr[i].u.in.addr) {
                જાઓ next;
            }
            તોડો;
        }

        પાછા NGX_OK;

    next:
        ચાલુ;
    }

    પાછા NGX_DECLINED;
}


ngx_int_t
ngx_parse_addr(ngx_pool_t *pool, ngx_addr_t *addr, u_char *text, size_t len)
{
    in_addr_t             inaddr;
    ngx_uint_t            family;
    સંરચના sockaddr_in   *sin;
#જો (NGX_HAVE_INET6)
    સંરચના in6_addr       inaddr6;
    સંરચના sockaddr_in6  *sin6;

    /*
     * prevent MSVC8 warning:
     *    potentially uninitialized local variable 'inaddr6' used
     */
    ngx_memzero(&inaddr6, કદ(સંરચના in6_addr));
#endif

    inaddr = ngx_inet_addr(text, len);

    જો (inaddr != INADDR_NONE) {
        family = AF_INET;
        len = કદ(સંરચના sockaddr_in);

#જો (NGX_HAVE_INET6)
    } નહીંતર જો (ngx_inet6_addr(text, len, inaddr6.s6_addr) == NGX_OK) {
        family = AF_INET6;
        len = કદ(સંરચના sockaddr_in6);

#endif
    } નહીંતર {
        પાછા NGX_DECLINED;
    }

    addr->sockaddr = ngx_pcalloc(pool, len);
    જો (addr->sockaddr == ખાલી) {
        પાછા NGX_ERROR;
    }

    addr->sockaddr->sa_family = (u_char) family;
    addr->socklen = len;

    પસંદગી (family) {

#જો (NGX_HAVE_INET6)
    સ્થિતિ AF_INET6:
        sin6 = (સંરચના sockaddr_in6 *) addr->sockaddr;
        ngx_memcpy(sin6->sin6_addr.s6_addr, inaddr6.s6_addr, 16);
        તોડો;
#endif

    મૂળભૂત: /* AF_INET */
        sin = (સંરચના sockaddr_in *) addr->sockaddr;
        sin->sin_addr.s_addr = inaddr;
        તોડો;
    }

    પાછા NGX_OK;
}


ngx_int_t
ngx_parse_addr_port(ngx_pool_t *pool, ngx_addr_t *addr, u_char *text,
    size_t len)
{
    u_char     *p, *last;
    size_t      plen;
    ngx_int_t   rc, port;

    rc = ngx_parse_addr(pool, addr, text, len);

    જો (rc != NGX_DECLINED) {
        પાછા rc;
    }

    last = text + len;

#જો (NGX_HAVE_INET6)
    જો (len && text[0] == '[') {

        p = ngx_strlchr(text, last, ']');

        જો (p == last - 1) {
            પાછા ngx_parse_addr(pool, addr, text + 1, len - 2);
        }

        જો (p == ખાલી || *++p != ':') {
            પાછા NGX_DECLINED;
        }

        text++;
        len -= 2;

    } નહીંતર
#endif

    {
        p = ngx_strlchr(text, last, ':');

        જો (p == ખાલી) {
            પાછા NGX_DECLINED;
        }
    }

    p++;
    plen = last - p;

    port = ngx_atoi(p, plen);

    જો (port < 1 || port > 65535) {
        પાછા NGX_DECLINED;
    }

    len -= plen + 1;

    rc = ngx_parse_addr(pool, addr, text, len);

    જો (rc != NGX_OK) {
        પાછા rc;
    }

    ngx_inet_set_port(addr->sockaddr, (in_port_t) port);

    પાછા NGX_OK;
}


ngx_int_t
ngx_parse_url(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char  *p;
    size_t   len;

    p = u->url.data;
    len = u->url.len;

    જો (len >= 5 && ngx_strncasecmp(p, (u_char *) "unix:", 5) == 0) {
        પાછા ngx_parse_unix_domain_url(pool, u);
    }

    જો (len && p[0] == '[') {
        પાછા ngx_parse_inet6_url(pool, u);
    }

    પાછા ngx_parse_inet_url(pool, u);
}


સ્થિર ngx_int_t
ngx_parse_unix_domain_url(ngx_pool_t *pool, ngx_url_t *u)
{
#જો (NGX_HAVE_UNIX_DOMAIN)
    u_char              *path, *uri, *last;
    size_t               len;
    સંરચના sockaddr_un  *saun;

    len = u->url.len;
    path = u->url.data;

    path += 5;
    len -= 5;

    જો (u->uri_part) {

        last = path + len;
        uri = ngx_strlchr(path, last, ':');

        જો (uri) {
            len = uri - path;
            uri++;
            u->uri.len = last - uri;
            u->uri.data = uri;
        }
    }

    જો (len == 0) {
        u->err = "no path in the unix domain socket";
        પાછા NGX_ERROR;
    }

    u->host.len = len++;
    u->host.data = path;

    જો (len > કદ(saun->sun_path)) {
        u->err = "too long path in the unix domain socket";
        પાછા NGX_ERROR;
    }

    u->socklen = કદ(સંરચના sockaddr_un);
    saun = (સંરચના sockaddr_un *) &u->sockaddr;
    saun->sun_family = AF_UNIX;
    (શૂન્ય) ngx_cpystrn((u_char *) saun->sun_path, path, len);

    u->addrs = ngx_pcalloc(pool, કદ(ngx_addr_t));
    જો (u->addrs == ખાલી) {
        પાછા NGX_ERROR;
    }

    saun = ngx_pcalloc(pool, કદ(સંરચના sockaddr_un));
    જો (saun == ખાલી) {
        પાછા NGX_ERROR;
    }

    u->family = AF_UNIX;
    u->naddrs = 1;

    saun->sun_family = AF_UNIX;
    (શૂન્ય) ngx_cpystrn((u_char *) saun->sun_path, path, len);

    u->addrs[0].sockaddr = (સંરચના sockaddr *) saun;
    u->addrs[0].socklen = કદ(સંરચના sockaddr_un);
    u->addrs[0].name.len = len + 4;
    u->addrs[0].name.data = u->url.data;

    પાછા NGX_OK;

#નહીંતર

    u->err = "the unix domain sockets are not supported on this platform";

    પાછા NGX_ERROR;

#endif
}


સ્થિર ngx_int_t
ngx_parse_inet_url(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char              *host, *port, *last, *uri, *args, *dash;
    size_t               len;
    ngx_int_t            n;
    સંરચના sockaddr_in  *sin;

    u->socklen = કદ(સંરચના sockaddr_in);
    sin = (સંરચના sockaddr_in *) &u->sockaddr;
    sin->sin_family = AF_INET;

    u->family = AF_INET;

    host = u->url.data;

    last = host + u->url.len;

    port = ngx_strlchr(host, last, ':');

    uri = ngx_strlchr(host, last, '/');

    args = ngx_strlchr(host, last, '?');

    જો (args) {
        જો (uri == ખાલી || args < uri) {
            uri = args;
        }
    }

    જો (uri) {
        જો (u->listen || !u->uri_part) {
            u->err = "invalid host";
            પાછા NGX_ERROR;
        }

        u->uri.len = last - uri;
        u->uri.data = uri;

        last = uri;

        જો (uri < port) {
            port = ખાલી;
        }
    }

    જો (port) {
        port++;

        len = last - port;

        જો (u->listen) {
            dash = ngx_strlchr(port, last, '-');

            જો (dash) {
                dash++;

                n = ngx_atoi(dash, last - dash);

                જો (n < 1 || n > 65535) {
                    u->err = "invalid port";
                    પાછા NGX_ERROR;
                }

                u->last_port = (in_port_t) n;

                len = dash - port - 1;
            }
        }

        n = ngx_atoi(port, len);

        જો (n < 1 || n > 65535) {
            u->err = "invalid port";
            પાછા NGX_ERROR;
        }

        જો (u->last_port && n > u->last_port) {
            u->err = "invalid port range";
            પાછા NGX_ERROR;
        }

        u->port = (in_port_t) n;
        sin->sin_port = htons((in_port_t) n);

        u->port_text.len = last - port;
        u->port_text.data = port;

        last = port - 1;

    } નહીંતર {
        જો (uri == ખાલી) {

            જો (u->listen) {

                /* test value as port only */

                len = last - host;

                dash = ngx_strlchr(host, last, '-');

                જો (dash) {
                    dash++;

                    n = ngx_atoi(dash, last - dash);

                    જો (n == NGX_ERROR) {
                        જાઓ no_port;
                    }

                    જો (n < 1 || n > 65535) {
                        u->err = "invalid port";

                    } નહીંતર {
                        u->last_port = (in_port_t) n;
                    }

                    len = dash - host - 1;
                }

                n = ngx_atoi(host, len);

                જો (n != NGX_ERROR) {

                    જો (u->err) {
                        પાછા NGX_ERROR;
                    }

                    જો (n < 1 || n > 65535) {
                        u->err = "invalid port";
                        પાછા NGX_ERROR;
                    }

                    જો (u->last_port && n > u->last_port) {
                        u->err = "invalid port range";
                        પાછા NGX_ERROR;
                    }

                    u->port = (in_port_t) n;
                    sin->sin_port = htons((in_port_t) n);
                    sin->sin_addr.s_addr = INADDR_ANY;

                    u->port_text.len = last - host;
                    u->port_text.data = host;

                    u->wildcard = 1;

                    પાછા ngx_inet_add_addr(pool, u, &u->sockaddr.sockaddr,
                                             u->socklen, 1);
                }
            }
        }

no_port:

        u->err = ખાલી;
        u->no_port = 1;
        u->port = u->default_port;
        sin->sin_port = htons(u->default_port);
        u->last_port = 0;
    }

    len = last - host;

    જો (len == 0) {
        u->err = "no host";
        પાછા NGX_ERROR;
    }

    u->host.len = len;
    u->host.data = host;

    જો (u->listen && len == 1 && *host == '*') {
        sin->sin_addr.s_addr = INADDR_ANY;
        u->wildcard = 1;
        પાછા ngx_inet_add_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);
    }

    sin->sin_addr.s_addr = ngx_inet_addr(host, len);

    જો (sin->sin_addr.s_addr != INADDR_NONE) {

        જો (sin->sin_addr.s_addr == INADDR_ANY) {
            u->wildcard = 1;
        }

        પાછા ngx_inet_add_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);
    }

    જો (u->no_resolve) {
        પાછા NGX_OK;
    }

    જો (ngx_inet_resolve_host(pool, u) != NGX_OK) {
        પાછા NGX_ERROR;
    }

    u->family = u->addrs[0].sockaddr->sa_family;
    u->socklen = u->addrs[0].socklen;
    ngx_memcpy(&u->sockaddr, u->addrs[0].sockaddr, u->addrs[0].socklen);
    u->wildcard = ngx_inet_wildcard(&u->sockaddr.sockaddr);

    પાછા NGX_OK;
}


સ્થિર ngx_int_t
ngx_parse_inet6_url(ngx_pool_t *pool, ngx_url_t *u)
{
#જો (NGX_HAVE_INET6)
    u_char               *p, *host, *port, *last, *uri, *dash;
    size_t                len;
    ngx_int_t             n;
    સંરચના sockaddr_in6  *sin6;

    u->socklen = કદ(સંરચના sockaddr_in6);
    sin6 = (સંરચના sockaddr_in6 *) &u->sockaddr;
    sin6->sin6_family = AF_INET6;

    host = u->url.data + 1;

    last = u->url.data + u->url.len;

    p = ngx_strlchr(host, last, ']');

    જો (p == ખાલી) {
        u->err = "invalid host";
        પાછા NGX_ERROR;
    }

    port = p + 1;

    uri = ngx_strlchr(port, last, '/');

    જો (uri) {
        જો (u->listen || !u->uri_part) {
            u->err = "invalid host";
            પાછા NGX_ERROR;
        }

        u->uri.len = last - uri;
        u->uri.data = uri;

        last = uri;
    }

    જો (port < last) {
        જો (*port != ':') {
            u->err = "invalid host";
            પાછા NGX_ERROR;
        }

        port++;

        len = last - port;

        જો (u->listen) {
            dash = ngx_strlchr(port, last, '-');

            જો (dash) {
                dash++;

                n = ngx_atoi(dash, last - dash);

                જો (n < 1 || n > 65535) {
                    u->err = "invalid port";
                    પાછા NGX_ERROR;
                }

                u->last_port = (in_port_t) n;

                len = dash - port - 1;
            }
        }

        n = ngx_atoi(port, len);

        જો (n < 1 || n > 65535) {
            u->err = "invalid port";
            પાછા NGX_ERROR;
        }

        જો (u->last_port && n > u->last_port) {
            u->err = "invalid port range";
            પાછા NGX_ERROR;
        }

        u->port = (in_port_t) n;
        sin6->sin6_port = htons((in_port_t) n);

        u->port_text.len = last - port;
        u->port_text.data = port;

    } નહીંતર {
        u->no_port = 1;
        u->port = u->default_port;
        sin6->sin6_port = htons(u->default_port);
    }

    len = p - host;

    જો (len == 0) {
        u->err = "no host";
        પાછા NGX_ERROR;
    }

    u->host.len = len + 2;
    u->host.data = host - 1;

    જો (ngx_inet6_addr(host, len, sin6->sin6_addr.s6_addr) != NGX_OK) {
        u->err = "invalid IPv6 address";
        પાછા NGX_ERROR;
    }

    જો (IN6_IS_ADDR_UNSPECIFIED(&sin6->sin6_addr)) {
        u->wildcard = 1;
    }

    u->family = AF_INET6;

    પાછા ngx_inet_add_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);

#નહીંતર

    u->err = "the INET6 sockets are not supported on this platform";

    પાછા NGX_ERROR;

#endif
}


#જો (NGX_HAVE_GETADDRINFO && NGX_HAVE_INET6)

ngx_int_t
ngx_inet_resolve_host(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char           *host;
    ngx_uint_t        n;
    સંરચના addrinfo   hints, *res, *rp;

    host = ngx_alloc(u->host.len + 1, pool->log);
    જો (host == ખાલી) {
        પાછા NGX_ERROR;
    }

    (શૂન્ય) ngx_cpystrn(host, u->host.data, u->host.len + 1);

    ngx_memzero(&hints, કદ(સંરચના addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
#ifdef AI_ADDRCONFIG
    hints.ai_flags = AI_ADDRCONFIG;
#endif

    જો (getaddrinfo((અક્ષર *) host, ખાલી, &hints, &res) != 0) {
        u->err = "host not found";
        ngx_free(host);
        પાછા NGX_ERROR;
    }

    ngx_free(host);

    માટે (n = 0, rp = res; rp != ખાલી; rp = rp->ai_next) {

        પસંદગી (rp->ai_family) {

        સ્થિતિ AF_INET:
        સ્થિતિ AF_INET6:
            તોડો;

        મૂળભૂત:
            ચાલુ;
        }

        n++;
    }

    જો (n == 0) {
        u->err = "host not found";
        જાઓ failed;
    }

    /* MP: ngx_shared_palloc() */

    માટે (rp = res; rp != ખાલી; rp = rp->ai_next) {

        પસંદગી (rp->ai_family) {

        સ્થિતિ AF_INET:
        સ્થિતિ AF_INET6:
            તોડો;

        મૂળભૂત:
            ચાલુ;
        }

        જો (ngx_inet_add_addr(pool, u, rp->ai_addr, rp->ai_addrlen, n)
            != NGX_OK)
        {
            જાઓ failed;
        }
    }

    freeaddrinfo(res);
    પાછા NGX_OK;

failed:

    freeaddrinfo(res);
    પાછા NGX_ERROR;
}

#નહીંતર /* !NGX_HAVE_GETADDRINFO || !NGX_HAVE_INET6 */

ngx_int_t
ngx_inet_resolve_host(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char              *host;
    ngx_uint_t           i, n;
    સંરચના hostent      *h;
    સંરચના sockaddr_in   sin;

    /* AF_INET only */

    ngx_memzero(&sin, કદ(સંરચના sockaddr_in));

    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = ngx_inet_addr(u->host.data, u->host.len);

    જો (sin.sin_addr.s_addr == INADDR_NONE) {
        host = ngx_alloc(u->host.len + 1, pool->log);
        જો (host == ખાલી) {
            પાછા NGX_ERROR;
        }

        (શૂન્ય) ngx_cpystrn(host, u->host.data, u->host.len + 1);

        h = gethostbyname((અક્ષર *) host);

        ngx_free(host);

        જો (h == ખાલી || h->h_addr_list[0] == ખાલી) {
            u->err = "host not found";
            પાછા NGX_ERROR;
        }

        માટે (n = 0; h->h_addr_list[n] != ખાલી; n++) { /* void */ }

        /* MP: ngx_shared_palloc() */

        માટે (i = 0; i < n; i++) {
            sin.sin_addr.s_addr = *(in_addr_t *) (h->h_addr_list[i]);

            જો (ngx_inet_add_addr(pool, u, (સંરચના sockaddr *) &sin,
                                  કદ(સંરચના sockaddr_in), n)
                != NGX_OK)
            {
                પાછા NGX_ERROR;
            }
        }

    } નહીંતર {

        /* MP: ngx_shared_palloc() */

        જો (ngx_inet_add_addr(pool, u, (સંરચના sockaddr *) &sin,
                              કદ(સંરચના sockaddr_in), 1)
            != NGX_OK)
        {
            પાછા NGX_ERROR;
        }
    }

    પાછા NGX_OK;
}

#endif /* NGX_HAVE_GETADDRINFO && NGX_HAVE_INET6 */


સ્થિર ngx_int_t
ngx_inet_add_addr(ngx_pool_t *pool, ngx_url_t *u, સંરચના sockaddr *sockaddr,
    socklen_t socklen, ngx_uint_t total)
{
    u_char           *p;
    size_t            len;
    ngx_uint_t        i, nports;
    ngx_addr_t       *addr;
    સંરચના sockaddr  *sa;

    nports = u->last_port ? u->last_port - u->port + 1 : 1;

    જો (u->addrs == ખાલી) {
        u->addrs = ngx_palloc(pool, total * nports * કદ(ngx_addr_t));
        જો (u->addrs == ખાલી) {
            પાછા NGX_ERROR;
        }
    }

    માટે (i = 0; i < nports; i++) {
        sa = ngx_pcalloc(pool, socklen);
        જો (sa == ખાલી) {
            પાછા NGX_ERROR;
        }

        ngx_memcpy(sa, sockaddr, socklen);

        ngx_inet_set_port(sa, u->port + i);

        પસંદગી (sa->sa_family) {

#જો (NGX_HAVE_INET6)
        સ્થિતિ AF_INET6:
            len = NGX_INET6_ADDRSTRLEN + કદ("[]:65536") - 1;
            તોડો;
#endif

        મૂળભૂત: /* AF_INET */
            len = NGX_INET_ADDRSTRLEN + કદ(":65535") - 1;
        }

        p = ngx_pnalloc(pool, len);
        જો (p == ખાલી) {
            પાછા NGX_ERROR;
        }

        len = ngx_sock_ntop(sa, socklen, p, len, 1);

        addr = &u->addrs[u->naddrs++];

        addr->sockaddr = sa;
        addr->socklen = socklen;

        addr->name.len = len;
        addr->name.data = p;
    }

    પાછા NGX_OK;
}


ngx_int_t
ngx_cmp_sockaddr(સંરચના sockaddr *sa1, socklen_t slen1,
    સંરચના sockaddr *sa2, socklen_t slen2, ngx_uint_t cmp_port)
{
    સંરચના sockaddr_in   *sin1, *sin2;
#જો (NGX_HAVE_INET6)
    સંરચના sockaddr_in6  *sin61, *sin62;
#endif
#જો (NGX_HAVE_UNIX_DOMAIN)
    size_t                len;
    સંરચના sockaddr_un   *saun1, *saun2;
#endif

    જો (sa1->sa_family != sa2->sa_family) {
        પાછા NGX_DECLINED;
    }

    પસંદગી (sa1->sa_family) {

#જો (NGX_HAVE_INET6)
    સ્થિતિ AF_INET6:

        sin61 = (સંરચના sockaddr_in6 *) sa1;
        sin62 = (સંરચના sockaddr_in6 *) sa2;

        જો (cmp_port && sin61->sin6_port != sin62->sin6_port) {
            પાછા NGX_DECLINED;
        }

        જો (ngx_memcmp(&sin61->sin6_addr, &sin62->sin6_addr, 16) != 0) {
            પાછા NGX_DECLINED;
        }

        તોડો;
#endif

#જો (NGX_HAVE_UNIX_DOMAIN)
    સ્થિતિ AF_UNIX:

        saun1 = (સંરચના sockaddr_un *) sa1;
        saun2 = (સંરચના sockaddr_un *) sa2;

        જો (slen1 < slen2) {
            len = slen1 - offsetof(સંરચના sockaddr_un, sun_path);

        } નહીંતર {
            len = slen2 - offsetof(સંરચના sockaddr_un, sun_path);
        }

        જો (len > કદ(saun1->sun_path)) {
            len = કદ(saun1->sun_path);
        }

        જો (ngx_memcmp(&saun1->sun_path, &saun2->sun_path, len) != 0) {
            પાછા NGX_DECLINED;
        }

        તોડો;
#endif

    મૂળભૂત: /* AF_INET */

        sin1 = (સંરચના sockaddr_in *) sa1;
        sin2 = (સંરચના sockaddr_in *) sa2;

        જો (cmp_port && sin1->sin_port != sin2->sin_port) {
            પાછા NGX_DECLINED;
        }

        જો (sin1->sin_addr.s_addr != sin2->sin_addr.s_addr) {
            પાછા NGX_DECLINED;
        }

        તોડો;
    }

    પાછા NGX_OK;
}


in_port_t
ngx_inet_get_port(સંરચના sockaddr *sa)
{
    સંરચના sockaddr_in   *sin;
#જો (NGX_HAVE_INET6)
    સંરચના sockaddr_in6  *sin6;
#endif

    પસંદગી (sa->sa_family) {

#જો (NGX_HAVE_INET6)
    સ્થિતિ AF_INET6:
        sin6 = (સંરચના sockaddr_in6 *) sa;
        પાછા ntohs(sin6->sin6_port);
#endif

#જો (NGX_HAVE_UNIX_DOMAIN)
    સ્થિતિ AF_UNIX:
        પાછા 0;
#endif

    મૂળભૂત: /* AF_INET */
        sin = (સંરચના sockaddr_in *) sa;
        પાછા ntohs(sin->sin_port);
    }
}


શૂન્ય
ngx_inet_set_port(સંરચના sockaddr *sa, in_port_t port)
{
    સંરચના sockaddr_in   *sin;
#જો (NGX_HAVE_INET6)
    સંરચના sockaddr_in6  *sin6;
#endif

    પસંદગી (sa->sa_family) {

#જો (NGX_HAVE_INET6)
    સ્થિતિ AF_INET6:
        sin6 = (સંરચના sockaddr_in6 *) sa;
        sin6->sin6_port = htons(port);
        તોડો;
#endif

#જો (NGX_HAVE_UNIX_DOMAIN)
    સ્થિતિ AF_UNIX:
        તોડો;
#endif

    મૂળભૂત: /* AF_INET */
        sin = (સંરચના sockaddr_in *) sa;
        sin->sin_port = htons(port);
        તોડો;
    }
}


ngx_uint_t
ngx_inet_wildcard(સંરચના sockaddr *sa)
{
    સંરચના sockaddr_in   *sin;
#જો (NGX_HAVE_INET6)
    સંરચના sockaddr_in6  *sin6;
#endif

    પસંદગી (sa->sa_family) {

    સ્થિતિ AF_INET:
        sin = (સંરચના sockaddr_in *) sa;

        જો (sin->sin_addr.s_addr == INADDR_ANY) {
            પાછા 1;
        }

        તોડો;

#જો (NGX_HAVE_INET6)

    સ્થિતિ AF_INET6:
        sin6 = (સંરચના sockaddr_in6 *) sa;

        જો (IN6_IS_ADDR_UNSPECIFIED(&sin6->sin6_addr)) {
            પાછા 1;
        }

        તોડો;

#endif
    }

    પાછા 0;
}
