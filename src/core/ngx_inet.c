
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#அந்தர்பாவ <ngx_config.h>
#அந்தர்பாவ <ngx_core.h>


நிலையான ngx_int_t ngx_parse_unix_domain_url(ngx_pool_t *pool, ngx_url_t *u);
நிலையான ngx_int_t ngx_parse_inet_url(ngx_pool_t *pool, ngx_url_t *u);
நிலையான ngx_int_t ngx_parse_inet6_url(ngx_pool_t *pool, ngx_url_t *u);
நிலையான ngx_int_t ngx_inet_add_addr(ngx_pool_t *pool, ngx_url_t *u,
    கட்டமைப்பு sockaddr *sockaddr, socklen_t socklen, ngx_uint_t total);


in_addr_t
ngx_inet_addr(u_char *text, size_t len)
{
    u_char      *p, c;
    in_addr_t    addr;
    ngx_uint_t   octet, n;

    addr = 0;
    octet = 0;
    n = 0;

    க்கு (p = text; p < text + len; p++) {
        c = *p;

        எனில் (c >= '0' && c <= '9') {
            octet = octet * 10 + (c - '0');

            எனில் (octet > 255) {
                திரும்பு INADDR_NONE;
            }

            தொடர்;
        }

        எனில் (c == '.') {
            addr = (addr << 8) + octet;
            octet = 0;
            n++;
            தொடர்;
        }

        திரும்பு INADDR_NONE;
    }

    எனில் (n == 3) {
        addr = (addr << 8) + octet;
        திரும்பு htonl(addr);
    }

    திரும்பு INADDR_NONE;
}


#எனில் (NGX_HAVE_INET6)

ngx_int_t
ngx_inet6_addr(u_char *p, size_t len, u_char *addr)
{
    u_char      c, *zero, *digit, *s, *d;
    size_t      len4;
    ngx_uint_t  n, nibbles, word;

    எனில் (len == 0) {
        திரும்பு NGX_ERROR;
    }

    zero = வெற்று;
    digit = வெற்று;
    len4 = 0;
    nibbles = 0;
    word = 0;
    n = 8;

    எனில் (p[0] == ':') {
        p++;
        len--;
    }

    க்கு (/* void */; len; len--) {
        c = *p++;

        எனில் (c == ':') {
            எனில் (nibbles) {
                digit = p;
                len4 = len;
                *addr++ = (u_char) (word >> 8);
                *addr++ = (u_char) (word & 0xff);

                எனில் (--n) {
                    nibbles = 0;
                    word = 0;
                    தொடர்;
                }

            } இல்லையெனில் {
                எனில் (zero == வெற்று) {
                    digit = p;
                    len4 = len;
                    zero = addr;
                    தொடர்;
                }
            }

            திரும்பு NGX_ERROR;
        }

        எனில் (c == '.' && nibbles) {
            எனில் (n < 2 || digit == வெற்று) {
                திரும்பு NGX_ERROR;
            }

            word = ngx_inet_addr(digit, len4 - 1);
            எனில் (word == INADDR_NONE) {
                திரும்பு NGX_ERROR;
            }

            word = ntohl(word);
            *addr++ = (u_char) ((word >> 24) & 0xff);
            *addr++ = (u_char) ((word >> 16) & 0xff);
            n--;
            நிறுத்து;
        }

        எனில் (++nibbles > 4) {
            திரும்பு NGX_ERROR;
        }

        எனில் (c >= '0' && c <= '9') {
            word = word * 16 + (c - '0');
            தொடர்;
        }

        c |= 0x20;

        எனில் (c >= 'a' && c <= 'f') {
            word = word * 16 + (c - 'a') + 10;
            தொடர்;
        }

        திரும்பு NGX_ERROR;
    }

    எனில் (nibbles == 0 && zero == வெற்று) {
        திரும்பு NGX_ERROR;
    }

    *addr++ = (u_char) (word >> 8);
    *addr++ = (u_char) (word & 0xff);

    எனில் (--n) {
        எனில் (zero) {
            n *= 2;
            s = addr - 1;
            d = s + n;
            போது (s >= zero) {
                *d-- = *s--;
            }
            ngx_memzero(zero, n);
            திரும்பு NGX_OK;
        }

    } இல்லையெனில் {
        எனில் (zero == வெற்று) {
            திரும்பு NGX_OK;
        }
    }

    திரும்பு NGX_ERROR;
}

#endif


size_t
ngx_sock_ntop(கட்டமைப்பு sockaddr *sa, socklen_t socklen, u_char *text, size_t len,
    ngx_uint_t port)
{
    u_char               *p;
#எனில் (NGX_HAVE_INET6 || NGX_HAVE_UNIX_DOMAIN)
    size_t                n;
#endif
    கட்டமைப்பு sockaddr_in   *sin;
#எனில் (NGX_HAVE_INET6)
    கட்டமைப்பு sockaddr_in6  *sin6;
#endif
#எனில் (NGX_HAVE_UNIX_DOMAIN)
    கட்டமைப்பு sockaddr_un   *saun;
#endif

    தேர்வு (sa->sa_family) {

    நிலை AF_INET:

        sin = (கட்டமைப்பு sockaddr_in *) sa;
        p = (u_char *) &sin->sin_addr;

        எனில் (port) {
            p = ngx_snprintf(text, len, "%ud.%ud.%ud.%ud:%d",
                             p[0], p[1], p[2], p[3], ntohs(sin->sin_port));
        } இல்லையெனில் {
            p = ngx_snprintf(text, len, "%ud.%ud.%ud.%ud",
                             p[0], p[1], p[2], p[3]);
        }

        திரும்பு (p - text);

#எனில் (NGX_HAVE_INET6)

    நிலை AF_INET6:

        sin6 = (கட்டமைப்பு sockaddr_in6 *) sa;

        n = 0;

        எனில் (port) {
            text[n++] = '[';
        }

        n = ngx_inet6_ntop(sin6->sin6_addr.s6_addr, &text[n], len);

        எனில் (port) {
            n = ngx_sprintf(&text[1 + n], "]:%d",
                            ntohs(sin6->sin6_port)) - text;
        }

        திரும்பு n;
#endif

#எனில் (NGX_HAVE_UNIX_DOMAIN)

    நிலை AF_UNIX:
        saun = (கட்டமைப்பு sockaddr_un *) sa;

        /* on Linux sockaddr might not include sun_path at all */

        எனில் (socklen <= (socklen_t) offsetof(கட்டமைப்பு sockaddr_un, sun_path)) {
            p = ngx_snprintf(text, len, "unix:%Z");

        } இல்லையெனில் {
            n = ngx_strnlen((u_char *) saun->sun_path,
                            socklen - offsetof(கட்டமைப்பு sockaddr_un, sun_path));
            p = ngx_snprintf(text, len, "unix:%*s%Z", n, saun->sun_path);
        }

        /* we do not include trailing zero in address length */

        திரும்பு (p - text - 1);

#endif

    இயல்பு:
        திரும்பு 0;
    }
}


size_t
ngx_inet_ntop(முழு family, வெறுமை *addr, u_char *text, size_t len)
{
    u_char  *p;

    தேர்வு (family) {

    நிலை AF_INET:

        p = addr;

        திரும்பு ngx_snprintf(text, len, "%ud.%ud.%ud.%ud",
                            p[0], p[1], p[2], p[3])
               - text;

#எனில் (NGX_HAVE_INET6)

    நிலை AF_INET6:
        திரும்பு ngx_inet6_ntop(addr, text, len);

#endif

    இயல்பு:
        திரும்பு 0;
    }
}


#எனில் (NGX_HAVE_INET6)

size_t
ngx_inet6_ntop(u_char *p, u_char *text, size_t len)
{
    u_char      *dst;
    size_t       max, n;
    ngx_uint_t   i, zero, last;

    எனில் (len < NGX_INET6_ADDRSTRLEN) {
        திரும்பு 0;
    }

    zero = (ngx_uint_t) -1;
    last = (ngx_uint_t) -1;
    max = 1;
    n = 0;

    க்கு (i = 0; i < 16; i += 2) {

        எனில் (p[i] || p[i + 1]) {

            எனில் (max < n) {
                zero = last;
                max = n;
            }

            n = 0;
            தொடர்;
        }

        எனில் (n++ == 0) {
            last = i;
        }
    }

    எனில் (max < n) {
        zero = last;
        max = n;
    }

    dst = text;
    n = 16;

    எனில் (zero == 0) {

        எனில் ((max == 5 && p[10] == 0xff && p[11] == 0xff)
            || (max == 6)
            || (max == 7 && p[14] != 0 && p[15] != 1))
        {
            n = 12;
        }

        *dst++ = ':';
    }

    க்கு (i = 0; i < n; i += 2) {

        எனில் (i == zero) {
            *dst++ = ':';
            i += (max - 1) * 2;
            தொடர்;
        }

        dst = ngx_sprintf(dst, "%xd", p[i] * 256 + p[i + 1]);

        எனில் (i < 14) {
            *dst++ = ':';
        }
    }

    எனில் (n == 12) {
        dst = ngx_sprintf(dst, "%ud.%ud.%ud.%ud", p[12], p[13], p[14], p[15]);
    }

    திரும்பு dst - text;
}

#endif


ngx_int_t
ngx_ptocidr(ngx_str_t *text, ngx_cidr_t *cidr)
{
    u_char      *addr, *mask, *last;
    size_t       len;
    ngx_int_t    shift;
#எனில் (NGX_HAVE_INET6)
    ngx_int_t    rc;
    ngx_uint_t   s, i;
#endif

    addr = text->data;
    last = addr + text->len;

    mask = ngx_strlchr(addr, last, '/');
    len = (mask ? mask : last) - addr;

    cidr->u.in.addr = ngx_inet_addr(addr, len);

    எனில் (cidr->u.in.addr != INADDR_NONE) {
        cidr->family = AF_INET;

        எனில் (mask == வெற்று) {
            cidr->u.in.mask = 0xffffffff;
            திரும்பு NGX_OK;
        }

#எனில் (NGX_HAVE_INET6)
    } இல்லையெனில் எனில் (ngx_inet6_addr(addr, len, cidr->u.in6.addr.s6_addr) == NGX_OK) {
        cidr->family = AF_INET6;

        எனில் (mask == வெற்று) {
            ngx_memset(cidr->u.in6.mask.s6_addr, 0xff, 16);
            திரும்பு NGX_OK;
        }

#endif
    } இல்லையெனில் {
        திரும்பு NGX_ERROR;
    }

    mask++;

    shift = ngx_atoi(mask, last - mask);
    எனில் (shift == NGX_ERROR) {
        திரும்பு NGX_ERROR;
    }

    தேர்வு (cidr->family) {

#எனில் (NGX_HAVE_INET6)
    நிலை AF_INET6:
        எனில் (shift > 128) {
            திரும்பு NGX_ERROR;
        }

        addr = cidr->u.in6.addr.s6_addr;
        mask = cidr->u.in6.mask.s6_addr;
        rc = NGX_OK;

        க்கு (i = 0; i < 16; i++) {

            s = (shift > 8) ? 8 : shift;
            shift -= s;

            mask[i] = (u_char) (0xffu << (8 - s));

            எனில் (addr[i] != (addr[i] & mask[i])) {
                rc = NGX_DONE;
                addr[i] &= mask[i];
            }
        }

        திரும்பு rc;
#endif

    இயல்பு: /* AF_INET */
        எனில் (shift > 32) {
            திரும்பு NGX_ERROR;
        }

        எனில் (shift) {
            cidr->u.in.mask = htonl((uint32_t) (0xffffffffu << (32 - shift)));

        } இல்லையெனில் {
            /* x86 compilers use a shl instruction that shifts by modulo 32 */
            cidr->u.in.mask = 0;
        }

        எனில் (cidr->u.in.addr == (cidr->u.in.addr & cidr->u.in.mask)) {
            திரும்பு NGX_OK;
        }

        cidr->u.in.addr &= cidr->u.in.mask;

        திரும்பு NGX_DONE;
    }
}


ngx_int_t
ngx_cidr_match(கட்டமைப்பு sockaddr *sa, ngx_array_t *cidrs)
{
#எனில் (NGX_HAVE_INET6)
    u_char           *p;
#endif
    in_addr_t         inaddr;
    ngx_cidr_t       *cidr;
    ngx_uint_t        family, i;
#எனில் (NGX_HAVE_INET6)
    ngx_uint_t        n;
    கட்டமைப்பு in6_addr  *inaddr6;
#endif

#எனில் (NGX_SUPPRESS_WARN)
    inaddr = 0;
#எனில் (NGX_HAVE_INET6)
    inaddr6 = வெற்று;
#endif
#endif

    family = sa->sa_family;

    எனில் (family == AF_INET) {
        inaddr = ((கட்டமைப்பு sockaddr_in *) sa)->sin_addr.s_addr;
    }

#எனில் (NGX_HAVE_INET6)
    இல்லையெனில் எனில் (family == AF_INET6) {
        inaddr6 = &((கட்டமைப்பு sockaddr_in6 *) sa)->sin6_addr;

        எனில் (IN6_IS_ADDR_V4MAPPED(inaddr6)) {
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

    க்கு (cidr = cidrs->elts, i = 0; i < cidrs->nelts; i++) {
        எனில் (cidr[i].family != family) {
            செல் next;
        }

        தேர்வு (family) {

#எனில் (NGX_HAVE_INET6)
        நிலை AF_INET6:
            க்கு (n = 0; n < 16; n++) {
                எனில் ((inaddr6->s6_addr[n] & cidr[i].u.in6.mask.s6_addr[n])
                    != cidr[i].u.in6.addr.s6_addr[n])
                {
                    செல் next;
                }
            }
            நிறுத்து;
#endif

#எனில் (NGX_HAVE_UNIX_DOMAIN)
        நிலை AF_UNIX:
            நிறுத்து;
#endif

        இயல்பு: /* AF_INET */
            எனில் ((inaddr & cidr[i].u.in.mask) != cidr[i].u.in.addr) {
                செல் next;
            }
            நிறுத்து;
        }

        திரும்பு NGX_OK;

    next:
        தொடர்;
    }

    திரும்பு NGX_DECLINED;
}


ngx_int_t
ngx_parse_addr(ngx_pool_t *pool, ngx_addr_t *addr, u_char *text, size_t len)
{
    in_addr_t             inaddr;
    ngx_uint_t            family;
    கட்டமைப்பு sockaddr_in   *sin;
#எனில் (NGX_HAVE_INET6)
    கட்டமைப்பு in6_addr       inaddr6;
    கட்டமைப்பு sockaddr_in6  *sin6;

    /*
     * prevent MSVC8 warning:
     *    potentially uninitialized local variable 'inaddr6' used
     */
    ngx_memzero(&inaddr6, அளவு(கட்டமைப்பு in6_addr));
#endif

    inaddr = ngx_inet_addr(text, len);

    எனில் (inaddr != INADDR_NONE) {
        family = AF_INET;
        len = அளவு(கட்டமைப்பு sockaddr_in);

#எனில் (NGX_HAVE_INET6)
    } இல்லையெனில் எனில் (ngx_inet6_addr(text, len, inaddr6.s6_addr) == NGX_OK) {
        family = AF_INET6;
        len = அளவு(கட்டமைப்பு sockaddr_in6);

#endif
    } இல்லையெனில் {
        திரும்பு NGX_DECLINED;
    }

    addr->sockaddr = ngx_pcalloc(pool, len);
    எனில் (addr->sockaddr == வெற்று) {
        திரும்பு NGX_ERROR;
    }

    addr->sockaddr->sa_family = (u_char) family;
    addr->socklen = len;

    தேர்வு (family) {

#எனில் (NGX_HAVE_INET6)
    நிலை AF_INET6:
        sin6 = (கட்டமைப்பு sockaddr_in6 *) addr->sockaddr;
        ngx_memcpy(sin6->sin6_addr.s6_addr, inaddr6.s6_addr, 16);
        நிறுத்து;
#endif

    இயல்பு: /* AF_INET */
        sin = (கட்டமைப்பு sockaddr_in *) addr->sockaddr;
        sin->sin_addr.s_addr = inaddr;
        நிறுத்து;
    }

    திரும்பு NGX_OK;
}


ngx_int_t
ngx_parse_addr_port(ngx_pool_t *pool, ngx_addr_t *addr, u_char *text,
    size_t len)
{
    u_char     *p, *last;
    size_t      plen;
    ngx_int_t   rc, port;

    rc = ngx_parse_addr(pool, addr, text, len);

    எனில் (rc != NGX_DECLINED) {
        திரும்பு rc;
    }

    last = text + len;

#எனில் (NGX_HAVE_INET6)
    எனில் (len && text[0] == '[') {

        p = ngx_strlchr(text, last, ']');

        எனில் (p == last - 1) {
            திரும்பு ngx_parse_addr(pool, addr, text + 1, len - 2);
        }

        எனில் (p == வெற்று || *++p != ':') {
            திரும்பு NGX_DECLINED;
        }

        text++;
        len -= 2;

    } இல்லையெனில்
#endif

    {
        p = ngx_strlchr(text, last, ':');

        எனில் (p == வெற்று) {
            திரும்பு NGX_DECLINED;
        }
    }

    p++;
    plen = last - p;

    port = ngx_atoi(p, plen);

    எனில் (port < 1 || port > 65535) {
        திரும்பு NGX_DECLINED;
    }

    len -= plen + 1;

    rc = ngx_parse_addr(pool, addr, text, len);

    எனில் (rc != NGX_OK) {
        திரும்பு rc;
    }

    ngx_inet_set_port(addr->sockaddr, (in_port_t) port);

    திரும்பு NGX_OK;
}


ngx_int_t
ngx_parse_url(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char  *p;
    size_t   len;

    p = u->url.data;
    len = u->url.len;

    எனில் (len >= 5 && ngx_strncasecmp(p, (u_char *) "unix:", 5) == 0) {
        திரும்பு ngx_parse_unix_domain_url(pool, u);
    }

    எனில் (len && p[0] == '[') {
        திரும்பு ngx_parse_inet6_url(pool, u);
    }

    திரும்பு ngx_parse_inet_url(pool, u);
}


நிலையான ngx_int_t
ngx_parse_unix_domain_url(ngx_pool_t *pool, ngx_url_t *u)
{
#எனில் (NGX_HAVE_UNIX_DOMAIN)
    u_char              *path, *uri, *last;
    size_t               len;
    கட்டமைப்பு sockaddr_un  *saun;

    len = u->url.len;
    path = u->url.data;

    path += 5;
    len -= 5;

    எனில் (u->uri_part) {

        last = path + len;
        uri = ngx_strlchr(path, last, ':');

        எனில் (uri) {
            len = uri - path;
            uri++;
            u->uri.len = last - uri;
            u->uri.data = uri;
        }
    }

    எனில் (len == 0) {
        u->err = "no path in the unix domain socket";
        திரும்பு NGX_ERROR;
    }

    u->host.len = len++;
    u->host.data = path;

    எனில் (len > அளவு(saun->sun_path)) {
        u->err = "too long path in the unix domain socket";
        திரும்பு NGX_ERROR;
    }

    u->socklen = அளவு(கட்டமைப்பு sockaddr_un);
    saun = (கட்டமைப்பு sockaddr_un *) &u->sockaddr;
    saun->sun_family = AF_UNIX;
    (வெறுமை) ngx_cpystrn((u_char *) saun->sun_path, path, len);

    u->addrs = ngx_pcalloc(pool, அளவு(ngx_addr_t));
    எனில் (u->addrs == வெற்று) {
        திரும்பு NGX_ERROR;
    }

    saun = ngx_pcalloc(pool, அளவு(கட்டமைப்பு sockaddr_un));
    எனில் (saun == வெற்று) {
        திரும்பு NGX_ERROR;
    }

    u->family = AF_UNIX;
    u->naddrs = 1;

    saun->sun_family = AF_UNIX;
    (வெறுமை) ngx_cpystrn((u_char *) saun->sun_path, path, len);

    u->addrs[0].sockaddr = (கட்டமைப்பு sockaddr *) saun;
    u->addrs[0].socklen = அளவு(கட்டமைப்பு sockaddr_un);
    u->addrs[0].name.len = len + 4;
    u->addrs[0].name.data = u->url.data;

    திரும்பு NGX_OK;

#இல்லையெனில்

    u->err = "the unix domain sockets are not supported on this platform";

    திரும்பு NGX_ERROR;

#endif
}


நிலையான ngx_int_t
ngx_parse_inet_url(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char              *host, *port, *last, *uri, *args, *dash;
    size_t               len;
    ngx_int_t            n;
    கட்டமைப்பு sockaddr_in  *sin;

    u->socklen = அளவு(கட்டமைப்பு sockaddr_in);
    sin = (கட்டமைப்பு sockaddr_in *) &u->sockaddr;
    sin->sin_family = AF_INET;

    u->family = AF_INET;

    host = u->url.data;

    last = host + u->url.len;

    port = ngx_strlchr(host, last, ':');

    uri = ngx_strlchr(host, last, '/');

    args = ngx_strlchr(host, last, '?');

    எனில் (args) {
        எனில் (uri == வெற்று || args < uri) {
            uri = args;
        }
    }

    எனில் (uri) {
        எனில் (u->listen || !u->uri_part) {
            u->err = "invalid host";
            திரும்பு NGX_ERROR;
        }

        u->uri.len = last - uri;
        u->uri.data = uri;

        last = uri;

        எனில் (uri < port) {
            port = வெற்று;
        }
    }

    எனில் (port) {
        port++;

        len = last - port;

        எனில் (u->listen) {
            dash = ngx_strlchr(port, last, '-');

            எனில் (dash) {
                dash++;

                n = ngx_atoi(dash, last - dash);

                எனில் (n < 1 || n > 65535) {
                    u->err = "invalid port";
                    திரும்பு NGX_ERROR;
                }

                u->last_port = (in_port_t) n;

                len = dash - port - 1;
            }
        }

        n = ngx_atoi(port, len);

        எனில் (n < 1 || n > 65535) {
            u->err = "invalid port";
            திரும்பு NGX_ERROR;
        }

        எனில் (u->last_port && n > u->last_port) {
            u->err = "invalid port range";
            திரும்பு NGX_ERROR;
        }

        u->port = (in_port_t) n;
        sin->sin_port = htons((in_port_t) n);

        u->port_text.len = last - port;
        u->port_text.data = port;

        last = port - 1;

    } இல்லையெனில் {
        எனில் (uri == வெற்று) {

            எனில் (u->listen) {

                /* test value as port only */

                len = last - host;

                dash = ngx_strlchr(host, last, '-');

                எனில் (dash) {
                    dash++;

                    n = ngx_atoi(dash, last - dash);

                    எனில் (n == NGX_ERROR) {
                        செல் no_port;
                    }

                    எனில் (n < 1 || n > 65535) {
                        u->err = "invalid port";

                    } இல்லையெனில் {
                        u->last_port = (in_port_t) n;
                    }

                    len = dash - host - 1;
                }

                n = ngx_atoi(host, len);

                எனில் (n != NGX_ERROR) {

                    எனில் (u->err) {
                        திரும்பு NGX_ERROR;
                    }

                    எனில் (n < 1 || n > 65535) {
                        u->err = "invalid port";
                        திரும்பு NGX_ERROR;
                    }

                    எனில் (u->last_port && n > u->last_port) {
                        u->err = "invalid port range";
                        திரும்பு NGX_ERROR;
                    }

                    u->port = (in_port_t) n;
                    sin->sin_port = htons((in_port_t) n);
                    sin->sin_addr.s_addr = INADDR_ANY;

                    u->port_text.len = last - host;
                    u->port_text.data = host;

                    u->wildcard = 1;

                    திரும்பு ngx_inet_add_addr(pool, u, &u->sockaddr.sockaddr,
                                             u->socklen, 1);
                }
            }
        }

no_port:

        u->err = வெற்று;
        u->no_port = 1;
        u->port = u->default_port;
        sin->sin_port = htons(u->default_port);
        u->last_port = 0;
    }

    len = last - host;

    எனில் (len == 0) {
        u->err = "no host";
        திரும்பு NGX_ERROR;
    }

    u->host.len = len;
    u->host.data = host;

    எனில் (u->listen && len == 1 && *host == '*') {
        sin->sin_addr.s_addr = INADDR_ANY;
        u->wildcard = 1;
        திரும்பு ngx_inet_add_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);
    }

    sin->sin_addr.s_addr = ngx_inet_addr(host, len);

    எனில் (sin->sin_addr.s_addr != INADDR_NONE) {

        எனில் (sin->sin_addr.s_addr == INADDR_ANY) {
            u->wildcard = 1;
        }

        திரும்பு ngx_inet_add_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);
    }

    எனில் (u->no_resolve) {
        திரும்பு NGX_OK;
    }

    எனில் (ngx_inet_resolve_host(pool, u) != NGX_OK) {
        திரும்பு NGX_ERROR;
    }

    u->family = u->addrs[0].sockaddr->sa_family;
    u->socklen = u->addrs[0].socklen;
    ngx_memcpy(&u->sockaddr, u->addrs[0].sockaddr, u->addrs[0].socklen);
    u->wildcard = ngx_inet_wildcard(&u->sockaddr.sockaddr);

    திரும்பு NGX_OK;
}


நிலையான ngx_int_t
ngx_parse_inet6_url(ngx_pool_t *pool, ngx_url_t *u)
{
#எனில் (NGX_HAVE_INET6)
    u_char               *p, *host, *port, *last, *uri, *dash;
    size_t                len;
    ngx_int_t             n;
    கட்டமைப்பு sockaddr_in6  *sin6;

    u->socklen = அளவு(கட்டமைப்பு sockaddr_in6);
    sin6 = (கட்டமைப்பு sockaddr_in6 *) &u->sockaddr;
    sin6->sin6_family = AF_INET6;

    host = u->url.data + 1;

    last = u->url.data + u->url.len;

    p = ngx_strlchr(host, last, ']');

    எனில் (p == வெற்று) {
        u->err = "invalid host";
        திரும்பு NGX_ERROR;
    }

    port = p + 1;

    uri = ngx_strlchr(port, last, '/');

    எனில் (uri) {
        எனில் (u->listen || !u->uri_part) {
            u->err = "invalid host";
            திரும்பு NGX_ERROR;
        }

        u->uri.len = last - uri;
        u->uri.data = uri;

        last = uri;
    }

    எனில் (port < last) {
        எனில் (*port != ':') {
            u->err = "invalid host";
            திரும்பு NGX_ERROR;
        }

        port++;

        len = last - port;

        எனில் (u->listen) {
            dash = ngx_strlchr(port, last, '-');

            எனில் (dash) {
                dash++;

                n = ngx_atoi(dash, last - dash);

                எனில் (n < 1 || n > 65535) {
                    u->err = "invalid port";
                    திரும்பு NGX_ERROR;
                }

                u->last_port = (in_port_t) n;

                len = dash - port - 1;
            }
        }

        n = ngx_atoi(port, len);

        எனில் (n < 1 || n > 65535) {
            u->err = "invalid port";
            திரும்பு NGX_ERROR;
        }

        எனில் (u->last_port && n > u->last_port) {
            u->err = "invalid port range";
            திரும்பு NGX_ERROR;
        }

        u->port = (in_port_t) n;
        sin6->sin6_port = htons((in_port_t) n);

        u->port_text.len = last - port;
        u->port_text.data = port;

    } இல்லையெனில் {
        u->no_port = 1;
        u->port = u->default_port;
        sin6->sin6_port = htons(u->default_port);
    }

    len = p - host;

    எனில் (len == 0) {
        u->err = "no host";
        திரும்பு NGX_ERROR;
    }

    u->host.len = len + 2;
    u->host.data = host - 1;

    எனில் (ngx_inet6_addr(host, len, sin6->sin6_addr.s6_addr) != NGX_OK) {
        u->err = "invalid IPv6 address";
        திரும்பு NGX_ERROR;
    }

    எனில் (IN6_IS_ADDR_UNSPECIFIED(&sin6->sin6_addr)) {
        u->wildcard = 1;
    }

    u->family = AF_INET6;

    திரும்பு ngx_inet_add_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);

#இல்லையெனில்

    u->err = "the INET6 sockets are not supported on this platform";

    திரும்பு NGX_ERROR;

#endif
}


#எனில் (NGX_HAVE_GETADDRINFO && NGX_HAVE_INET6)

ngx_int_t
ngx_inet_resolve_host(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char           *host;
    ngx_uint_t        n;
    கட்டமைப்பு addrinfo   hints, *res, *rp;

    host = ngx_alloc(u->host.len + 1, pool->log);
    எனில் (host == வெற்று) {
        திரும்பு NGX_ERROR;
    }

    (வெறுமை) ngx_cpystrn(host, u->host.data, u->host.len + 1);

    ngx_memzero(&hints, அளவு(கட்டமைப்பு addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
#ifdef AI_ADDRCONFIG
    hints.ai_flags = AI_ADDRCONFIG;
#endif

    எனில் (getaddrinfo((எழுத்து *) host, வெற்று, &hints, &res) != 0) {
        u->err = "host not found";
        ngx_free(host);
        திரும்பு NGX_ERROR;
    }

    ngx_free(host);

    க்கு (n = 0, rp = res; rp != வெற்று; rp = rp->ai_next) {

        தேர்வு (rp->ai_family) {

        நிலை AF_INET:
        நிலை AF_INET6:
            நிறுத்து;

        இயல்பு:
            தொடர்;
        }

        n++;
    }

    எனில் (n == 0) {
        u->err = "host not found";
        செல் failed;
    }

    /* MP: ngx_shared_palloc() */

    க்கு (rp = res; rp != வெற்று; rp = rp->ai_next) {

        தேர்வு (rp->ai_family) {

        நிலை AF_INET:
        நிலை AF_INET6:
            நிறுத்து;

        இயல்பு:
            தொடர்;
        }

        எனில் (ngx_inet_add_addr(pool, u, rp->ai_addr, rp->ai_addrlen, n)
            != NGX_OK)
        {
            செல் failed;
        }
    }

    freeaddrinfo(res);
    திரும்பு NGX_OK;

failed:

    freeaddrinfo(res);
    திரும்பு NGX_ERROR;
}

#இல்லையெனில் /* !NGX_HAVE_GETADDRINFO || !NGX_HAVE_INET6 */

ngx_int_t
ngx_inet_resolve_host(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char              *host;
    ngx_uint_t           i, n;
    கட்டமைப்பு hostent      *h;
    கட்டமைப்பு sockaddr_in   sin;

    /* AF_INET only */

    ngx_memzero(&sin, அளவு(கட்டமைப்பு sockaddr_in));

    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = ngx_inet_addr(u->host.data, u->host.len);

    எனில் (sin.sin_addr.s_addr == INADDR_NONE) {
        host = ngx_alloc(u->host.len + 1, pool->log);
        எனில் (host == வெற்று) {
            திரும்பு NGX_ERROR;
        }

        (வெறுமை) ngx_cpystrn(host, u->host.data, u->host.len + 1);

        h = gethostbyname((எழுத்து *) host);

        ngx_free(host);

        எனில் (h == வெற்று || h->h_addr_list[0] == வெற்று) {
            u->err = "host not found";
            திரும்பு NGX_ERROR;
        }

        க்கு (n = 0; h->h_addr_list[n] != வெற்று; n++) { /* void */ }

        /* MP: ngx_shared_palloc() */

        க்கு (i = 0; i < n; i++) {
            sin.sin_addr.s_addr = *(in_addr_t *) (h->h_addr_list[i]);

            எனில் (ngx_inet_add_addr(pool, u, (கட்டமைப்பு sockaddr *) &sin,
                                  அளவு(கட்டமைப்பு sockaddr_in), n)
                != NGX_OK)
            {
                திரும்பு NGX_ERROR;
            }
        }

    } இல்லையெனில் {

        /* MP: ngx_shared_palloc() */

        எனில் (ngx_inet_add_addr(pool, u, (கட்டமைப்பு sockaddr *) &sin,
                              அளவு(கட்டமைப்பு sockaddr_in), 1)
            != NGX_OK)
        {
            திரும்பு NGX_ERROR;
        }
    }

    திரும்பு NGX_OK;
}

#endif /* NGX_HAVE_GETADDRINFO && NGX_HAVE_INET6 */


நிலையான ngx_int_t
ngx_inet_add_addr(ngx_pool_t *pool, ngx_url_t *u, கட்டமைப்பு sockaddr *sockaddr,
    socklen_t socklen, ngx_uint_t total)
{
    u_char           *p;
    size_t            len;
    ngx_uint_t        i, nports;
    ngx_addr_t       *addr;
    கட்டமைப்பு sockaddr  *sa;

    nports = u->last_port ? u->last_port - u->port + 1 : 1;

    எனில் (u->addrs == வெற்று) {
        u->addrs = ngx_palloc(pool, total * nports * அளவு(ngx_addr_t));
        எனில் (u->addrs == வெற்று) {
            திரும்பு NGX_ERROR;
        }
    }

    க்கு (i = 0; i < nports; i++) {
        sa = ngx_pcalloc(pool, socklen);
        எனில் (sa == வெற்று) {
            திரும்பு NGX_ERROR;
        }

        ngx_memcpy(sa, sockaddr, socklen);

        ngx_inet_set_port(sa, u->port + i);

        தேர்வு (sa->sa_family) {

#எனில் (NGX_HAVE_INET6)
        நிலை AF_INET6:
            len = NGX_INET6_ADDRSTRLEN + அளவு("[]:65536") - 1;
            நிறுத்து;
#endif

        இயல்பு: /* AF_INET */
            len = NGX_INET_ADDRSTRLEN + அளவு(":65535") - 1;
        }

        p = ngx_pnalloc(pool, len);
        எனில் (p == வெற்று) {
            திரும்பு NGX_ERROR;
        }

        len = ngx_sock_ntop(sa, socklen, p, len, 1);

        addr = &u->addrs[u->naddrs++];

        addr->sockaddr = sa;
        addr->socklen = socklen;

        addr->name.len = len;
        addr->name.data = p;
    }

    திரும்பு NGX_OK;
}


ngx_int_t
ngx_cmp_sockaddr(கட்டமைப்பு sockaddr *sa1, socklen_t slen1,
    கட்டமைப்பு sockaddr *sa2, socklen_t slen2, ngx_uint_t cmp_port)
{
    கட்டமைப்பு sockaddr_in   *sin1, *sin2;
#எனில் (NGX_HAVE_INET6)
    கட்டமைப்பு sockaddr_in6  *sin61, *sin62;
#endif
#எனில் (NGX_HAVE_UNIX_DOMAIN)
    size_t                len;
    கட்டமைப்பு sockaddr_un   *saun1, *saun2;
#endif

    எனில் (sa1->sa_family != sa2->sa_family) {
        திரும்பு NGX_DECLINED;
    }

    தேர்வு (sa1->sa_family) {

#எனில் (NGX_HAVE_INET6)
    நிலை AF_INET6:

        sin61 = (கட்டமைப்பு sockaddr_in6 *) sa1;
        sin62 = (கட்டமைப்பு sockaddr_in6 *) sa2;

        எனில் (cmp_port && sin61->sin6_port != sin62->sin6_port) {
            திரும்பு NGX_DECLINED;
        }

        எனில் (ngx_memcmp(&sin61->sin6_addr, &sin62->sin6_addr, 16) != 0) {
            திரும்பு NGX_DECLINED;
        }

        நிறுத்து;
#endif

#எனில் (NGX_HAVE_UNIX_DOMAIN)
    நிலை AF_UNIX:

        saun1 = (கட்டமைப்பு sockaddr_un *) sa1;
        saun2 = (கட்டமைப்பு sockaddr_un *) sa2;

        எனில் (slen1 < slen2) {
            len = slen1 - offsetof(கட்டமைப்பு sockaddr_un, sun_path);

        } இல்லையெனில் {
            len = slen2 - offsetof(கட்டமைப்பு sockaddr_un, sun_path);
        }

        எனில் (len > அளவு(saun1->sun_path)) {
            len = அளவு(saun1->sun_path);
        }

        எனில் (ngx_memcmp(&saun1->sun_path, &saun2->sun_path, len) != 0) {
            திரும்பு NGX_DECLINED;
        }

        நிறுத்து;
#endif

    இயல்பு: /* AF_INET */

        sin1 = (கட்டமைப்பு sockaddr_in *) sa1;
        sin2 = (கட்டமைப்பு sockaddr_in *) sa2;

        எனில் (cmp_port && sin1->sin_port != sin2->sin_port) {
            திரும்பு NGX_DECLINED;
        }

        எனில் (sin1->sin_addr.s_addr != sin2->sin_addr.s_addr) {
            திரும்பு NGX_DECLINED;
        }

        நிறுத்து;
    }

    திரும்பு NGX_OK;
}


in_port_t
ngx_inet_get_port(கட்டமைப்பு sockaddr *sa)
{
    கட்டமைப்பு sockaddr_in   *sin;
#எனில் (NGX_HAVE_INET6)
    கட்டமைப்பு sockaddr_in6  *sin6;
#endif

    தேர்வு (sa->sa_family) {

#எனில் (NGX_HAVE_INET6)
    நிலை AF_INET6:
        sin6 = (கட்டமைப்பு sockaddr_in6 *) sa;
        திரும்பு ntohs(sin6->sin6_port);
#endif

#எனில் (NGX_HAVE_UNIX_DOMAIN)
    நிலை AF_UNIX:
        திரும்பு 0;
#endif

    இயல்பு: /* AF_INET */
        sin = (கட்டமைப்பு sockaddr_in *) sa;
        திரும்பு ntohs(sin->sin_port);
    }
}


வெறுமை
ngx_inet_set_port(கட்டமைப்பு sockaddr *sa, in_port_t port)
{
    கட்டமைப்பு sockaddr_in   *sin;
#எனில் (NGX_HAVE_INET6)
    கட்டமைப்பு sockaddr_in6  *sin6;
#endif

    தேர்வு (sa->sa_family) {

#எனில் (NGX_HAVE_INET6)
    நிலை AF_INET6:
        sin6 = (கட்டமைப்பு sockaddr_in6 *) sa;
        sin6->sin6_port = htons(port);
        நிறுத்து;
#endif

#எனில் (NGX_HAVE_UNIX_DOMAIN)
    நிலை AF_UNIX:
        நிறுத்து;
#endif

    இயல்பு: /* AF_INET */
        sin = (கட்டமைப்பு sockaddr_in *) sa;
        sin->sin_port = htons(port);
        நிறுத்து;
    }
}


ngx_uint_t
ngx_inet_wildcard(கட்டமைப்பு sockaddr *sa)
{
    கட்டமைப்பு sockaddr_in   *sin;
#எனில் (NGX_HAVE_INET6)
    கட்டமைப்பு sockaddr_in6  *sin6;
#endif

    தேர்வு (sa->sa_family) {

    நிலை AF_INET:
        sin = (கட்டமைப்பு sockaddr_in *) sa;

        எனில் (sin->sin_addr.s_addr == INADDR_ANY) {
            திரும்பு 1;
        }

        நிறுத்து;

#எனில் (NGX_HAVE_INET6)

    நிலை AF_INET6:
        sin6 = (கட்டமைப்பு sockaddr_in6 *) sa;

        எனில் (IN6_IS_ADDR_UNSPECIFIED(&sin6->sin6_addr)) {
            திரும்பு 1;
        }

        நிறுத்து;

#endif
    }

    திரும்பு 0;
}
