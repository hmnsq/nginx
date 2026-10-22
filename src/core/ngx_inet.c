
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


स्थिर ngx_int_t ngx_parse_unix_domain_url(ngx_pool_t *pool, ngx_url_t *u);
स्थिर ngx_int_t ngx_parse_inet_url(ngx_pool_t *pool, ngx_url_t *u);
स्थिर ngx_int_t ngx_parse_inet6_url(ngx_pool_t *pool, ngx_url_t *u);
स्थिर ngx_int_t ngx_inet_add_addr(ngx_pool_t *pool, ngx_url_t *u,
    संरचना sockaddr *sockaddr, socklen_t socklen, ngx_uint_t total);


in_addr_t
ngx_inet_addr(u_char *text, size_t len)
{
    u_char      *p, c;
    in_addr_t    addr;
    ngx_uint_t   octet, n;

    addr = 0;
    octet = 0;
    n = 0;

    अत्थाय (p = text; p < text + len; p++) {
        c = *p;

        सचे (c >= '0' && c <= '9') {
            octet = octet * 10 + (c - '0');

            सचे (octet > 255) {
                पटिनिवत्त INADDR_NONE;
            }

            अनुवर्त;
        }

        सचे (c == '.') {
            addr = (addr << 8) + octet;
            octet = 0;
            n++;
            अनुवर्त;
        }

        पटिनिवत्त INADDR_NONE;
    }

    सचे (n == 3) {
        addr = (addr << 8) + octet;
        पटिनिवत्त htonl(addr);
    }

    पटिनिवत्त INADDR_NONE;
}


#सचे (NGX_HAVE_INET6)

ngx_int_t
ngx_inet6_addr(u_char *p, size_t len, u_char *addr)
{
    u_char      c, *zero, *digit, *s, *d;
    size_t      len4;
    ngx_uint_t  n, nibbles, word;

    सचे (len == 0) {
        पटिनिवत्त NGX_ERROR;
    }

    zero = रिक्त;
    digit = रिक्त;
    len4 = 0;
    nibbles = 0;
    word = 0;
    n = 8;

    सचे (p[0] == ':') {
        p++;
        len--;
    }

    अत्थाय (/* void */; len; len--) {
        c = *p++;

        सचे (c == ':') {
            सचे (nibbles) {
                digit = p;
                len4 = len;
                *addr++ = (u_char) (word >> 8);
                *addr++ = (u_char) (word & 0xff);

                सचे (--n) {
                    nibbles = 0;
                    word = 0;
                    अनुवर्त;
                }

            } अञ्ञथा {
                सचे (zero == रिक्त) {
                    digit = p;
                    len4 = len;
                    zero = addr;
                    अनुवर्त;
                }
            }

            पटिनिवत्त NGX_ERROR;
        }

        सचे (c == '.' && nibbles) {
            सचे (n < 2 || digit == रिक्त) {
                पटिनिवत्त NGX_ERROR;
            }

            word = ngx_inet_addr(digit, len4 - 1);
            सचे (word == INADDR_NONE) {
                पटिनिवत्त NGX_ERROR;
            }

            word = ntohl(word);
            *addr++ = (u_char) ((word >> 24) & 0xff);
            *addr++ = (u_char) ((word >> 16) & 0xff);
            n--;
            भङ्ग;
        }

        सचे (++nibbles > 4) {
            पटिनिवत्त NGX_ERROR;
        }

        सचे (c >= '0' && c <= '9') {
            word = word * 16 + (c - '0');
            अनुवर्त;
        }

        c |= 0x20;

        सचे (c >= 'a' && c <= 'f') {
            word = word * 16 + (c - 'a') + 10;
            अनुवर्त;
        }

        पटिनिवत्त NGX_ERROR;
    }

    सचे (nibbles == 0 && zero == रिक्त) {
        पटिनिवत्त NGX_ERROR;
    }

    *addr++ = (u_char) (word >> 8);
    *addr++ = (u_char) (word & 0xff);

    सचे (--n) {
        सचे (zero) {
            n *= 2;
            s = addr - 1;
            d = s + n;
            याव (s >= zero) {
                *d-- = *s--;
            }
            ngx_memzero(zero, n);
            पटिनिवत्त NGX_OK;
        }

    } अञ्ञथा {
        सचे (zero == रिक्त) {
            पटिनिवत्त NGX_OK;
        }
    }

    पटिनिवत्त NGX_ERROR;
}

#endif


size_t
ngx_sock_ntop(संरचना sockaddr *sa, socklen_t socklen, u_char *text, size_t len,
    ngx_uint_t port)
{
    u_char               *p;
#सचे (NGX_HAVE_INET6 || NGX_HAVE_UNIX_DOMAIN)
    size_t                n;
#endif
    संरचना sockaddr_in   *sin;
#सचे (NGX_HAVE_INET6)
    संरचना sockaddr_in6  *sin6;
#endif
#सचे (NGX_HAVE_UNIX_DOMAIN)
    संरचना sockaddr_un   *saun;
#endif

    विकल्प (sa->sa_family) {

    पक्ष AF_INET:

        sin = (संरचना sockaddr_in *) sa;
        p = (u_char *) &sin->sin_addr;

        सचे (port) {
            p = ngx_snprintf(text, len, "%ud.%ud.%ud.%ud:%d",
                             p[0], p[1], p[2], p[3], ntohs(sin->sin_port));
        } अञ्ञथा {
            p = ngx_snprintf(text, len, "%ud.%ud.%ud.%ud",
                             p[0], p[1], p[2], p[3]);
        }

        पटिनिवत्त (p - text);

#सचे (NGX_HAVE_INET6)

    पक्ष AF_INET6:

        sin6 = (संरचना sockaddr_in6 *) sa;

        n = 0;

        सचे (port) {
            text[n++] = '[';
        }

        n = ngx_inet6_ntop(sin6->sin6_addr.s6_addr, &text[n], len);

        सचे (port) {
            n = ngx_sprintf(&text[1 + n], "]:%d",
                            ntohs(sin6->sin6_port)) - text;
        }

        पटिनिवत्त n;
#endif

#सचे (NGX_HAVE_UNIX_DOMAIN)

    पक्ष AF_UNIX:
        saun = (संरचना sockaddr_un *) sa;

        /* on Linux sockaddr might not include sun_path at all */

        सचे (socklen <= (socklen_t) offsetof(संरचना sockaddr_un, sun_path)) {
            p = ngx_snprintf(text, len, "unix:%Z");

        } अञ्ञथा {
            n = ngx_strnlen((u_char *) saun->sun_path,
                            socklen - offsetof(संरचना sockaddr_un, sun_path));
            p = ngx_snprintf(text, len, "unix:%*s%Z", n, saun->sun_path);
        }

        /* we do not include trailing zero in address length */

        पटिनिवत्त (p - text - 1);

#endif

    स्वभाव:
        पटिनिवत्त 0;
    }
}


size_t
ngx_inet_ntop(पूर्णाङ्क family, शून्य *addr, u_char *text, size_t len)
{
    u_char  *p;

    विकल्प (family) {

    पक्ष AF_INET:

        p = addr;

        पटिनिवत्त ngx_snprintf(text, len, "%ud.%ud.%ud.%ud",
                            p[0], p[1], p[2], p[3])
               - text;

#सचे (NGX_HAVE_INET6)

    पक्ष AF_INET6:
        पटिनिवत्त ngx_inet6_ntop(addr, text, len);

#endif

    स्वभाव:
        पटिनिवत्त 0;
    }
}


#सचे (NGX_HAVE_INET6)

size_t
ngx_inet6_ntop(u_char *p, u_char *text, size_t len)
{
    u_char      *dst;
    size_t       max, n;
    ngx_uint_t   i, zero, last;

    सचे (len < NGX_INET6_ADDRSTRLEN) {
        पटिनिवत्त 0;
    }

    zero = (ngx_uint_t) -1;
    last = (ngx_uint_t) -1;
    max = 1;
    n = 0;

    अत्थाय (i = 0; i < 16; i += 2) {

        सचे (p[i] || p[i + 1]) {

            सचे (max < n) {
                zero = last;
                max = n;
            }

            n = 0;
            अनुवर्त;
        }

        सचे (n++ == 0) {
            last = i;
        }
    }

    सचे (max < n) {
        zero = last;
        max = n;
    }

    dst = text;
    n = 16;

    सचे (zero == 0) {

        सचे ((max == 5 && p[10] == 0xff && p[11] == 0xff)
            || (max == 6)
            || (max == 7 && p[14] != 0 && p[15] != 1))
        {
            n = 12;
        }

        *dst++ = ':';
    }

    अत्थाय (i = 0; i < n; i += 2) {

        सचे (i == zero) {
            *dst++ = ':';
            i += (max - 1) * 2;
            अनुवर्त;
        }

        dst = ngx_sprintf(dst, "%xd", p[i] * 256 + p[i + 1]);

        सचे (i < 14) {
            *dst++ = ':';
        }
    }

    सचे (n == 12) {
        dst = ngx_sprintf(dst, "%ud.%ud.%ud.%ud", p[12], p[13], p[14], p[15]);
    }

    पटिनिवत्त dst - text;
}

#endif


ngx_int_t
ngx_ptocidr(ngx_str_t *text, ngx_cidr_t *cidr)
{
    u_char      *addr, *mask, *last;
    size_t       len;
    ngx_int_t    shift;
#सचे (NGX_HAVE_INET6)
    ngx_int_t    rc;
    ngx_uint_t   s, i;
#endif

    addr = text->data;
    last = addr + text->len;

    mask = ngx_strlchr(addr, last, '/');
    len = (mask ? mask : last) - addr;

    cidr->u.in.addr = ngx_inet_addr(addr, len);

    सचे (cidr->u.in.addr != INADDR_NONE) {
        cidr->family = AF_INET;

        सचे (mask == रिक्त) {
            cidr->u.in.mask = 0xffffffff;
            पटिनिवत्त NGX_OK;
        }

#सचे (NGX_HAVE_INET6)
    } अञ्ञथा सचे (ngx_inet6_addr(addr, len, cidr->u.in6.addr.s6_addr) == NGX_OK) {
        cidr->family = AF_INET6;

        सचे (mask == रिक्त) {
            ngx_memset(cidr->u.in6.mask.s6_addr, 0xff, 16);
            पटिनिवत्त NGX_OK;
        }

#endif
    } अञ्ञथा {
        पटिनिवत्त NGX_ERROR;
    }

    mask++;

    shift = ngx_atoi(mask, last - mask);
    सचे (shift == NGX_ERROR) {
        पटिनिवत्त NGX_ERROR;
    }

    विकल्प (cidr->family) {

#सचे (NGX_HAVE_INET6)
    पक्ष AF_INET6:
        सचे (shift > 128) {
            पटिनिवत्त NGX_ERROR;
        }

        addr = cidr->u.in6.addr.s6_addr;
        mask = cidr->u.in6.mask.s6_addr;
        rc = NGX_OK;

        अत्थाय (i = 0; i < 16; i++) {

            s = (shift > 8) ? 8 : shift;
            shift -= s;

            mask[i] = (u_char) (0xffu << (8 - s));

            सचे (addr[i] != (addr[i] & mask[i])) {
                rc = NGX_DONE;
                addr[i] &= mask[i];
            }
        }

        पटिनिवत्त rc;
#endif

    स्वभाव: /* AF_INET */
        सचे (shift > 32) {
            पटिनिवत्त NGX_ERROR;
        }

        सचे (shift) {
            cidr->u.in.mask = htonl((uint32_t) (0xffffffffu << (32 - shift)));

        } अञ्ञथा {
            /* x86 compilers use a shl instruction that shifts by modulo 32 */
            cidr->u.in.mask = 0;
        }

        सचे (cidr->u.in.addr == (cidr->u.in.addr & cidr->u.in.mask)) {
            पटिनिवत्त NGX_OK;
        }

        cidr->u.in.addr &= cidr->u.in.mask;

        पटिनिवत्त NGX_DONE;
    }
}


ngx_int_t
ngx_cidr_match(संरचना sockaddr *sa, ngx_array_t *cidrs)
{
#सचे (NGX_HAVE_INET6)
    u_char           *p;
#endif
    in_addr_t         inaddr;
    ngx_cidr_t       *cidr;
    ngx_uint_t        family, i;
#सचे (NGX_HAVE_INET6)
    ngx_uint_t        n;
    संरचना in6_addr  *inaddr6;
#endif

#सचे (NGX_SUPPRESS_WARN)
    inaddr = 0;
#सचे (NGX_HAVE_INET6)
    inaddr6 = रिक्त;
#endif
#endif

    family = sa->sa_family;

    सचे (family == AF_INET) {
        inaddr = ((संरचना sockaddr_in *) sa)->sin_addr.s_addr;
    }

#सचे (NGX_HAVE_INET6)
    अञ्ञथा सचे (family == AF_INET6) {
        inaddr6 = &((संरचना sockaddr_in6 *) sa)->sin6_addr;

        सचे (IN6_IS_ADDR_V4MAPPED(inaddr6)) {
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

    अत्थाय (cidr = cidrs->elts, i = 0; i < cidrs->nelts; i++) {
        सचे (cidr[i].family != family) {
            गच्छ next;
        }

        विकल्प (family) {

#सचे (NGX_HAVE_INET6)
        पक्ष AF_INET6:
            अत्थाय (n = 0; n < 16; n++) {
                सचे ((inaddr6->s6_addr[n] & cidr[i].u.in6.mask.s6_addr[n])
                    != cidr[i].u.in6.addr.s6_addr[n])
                {
                    गच्छ next;
                }
            }
            भङ्ग;
#endif

#सचे (NGX_HAVE_UNIX_DOMAIN)
        पक्ष AF_UNIX:
            भङ्ग;
#endif

        स्वभाव: /* AF_INET */
            सचे ((inaddr & cidr[i].u.in.mask) != cidr[i].u.in.addr) {
                गच्छ next;
            }
            भङ्ग;
        }

        पटिनिवत्त NGX_OK;

    next:
        अनुवर्त;
    }

    पटिनिवत्त NGX_DECLINED;
}


ngx_int_t
ngx_parse_addr(ngx_pool_t *pool, ngx_addr_t *addr, u_char *text, size_t len)
{
    in_addr_t             inaddr;
    ngx_uint_t            family;
    संरचना sockaddr_in   *sin;
#सचे (NGX_HAVE_INET6)
    संरचना in6_addr       inaddr6;
    संरचना sockaddr_in6  *sin6;

    /*
     * prevent MSVC8 warning:
     *    potentially uninitialized local variable 'inaddr6' used
     */
    ngx_memzero(&inaddr6, परिमाण(संरचना in6_addr));
#endif

    inaddr = ngx_inet_addr(text, len);

    सचे (inaddr != INADDR_NONE) {
        family = AF_INET;
        len = परिमाण(संरचना sockaddr_in);

#सचे (NGX_HAVE_INET6)
    } अञ्ञथा सचे (ngx_inet6_addr(text, len, inaddr6.s6_addr) == NGX_OK) {
        family = AF_INET6;
        len = परिमाण(संरचना sockaddr_in6);

#endif
    } अञ्ञथा {
        पटिनिवत्त NGX_DECLINED;
    }

    addr->sockaddr = ngx_pcalloc(pool, len);
    सचे (addr->sockaddr == रिक्त) {
        पटिनिवत्त NGX_ERROR;
    }

    addr->sockaddr->sa_family = (u_char) family;
    addr->socklen = len;

    विकल्प (family) {

#सचे (NGX_HAVE_INET6)
    पक्ष AF_INET6:
        sin6 = (संरचना sockaddr_in6 *) addr->sockaddr;
        ngx_memcpy(sin6->sin6_addr.s6_addr, inaddr6.s6_addr, 16);
        भङ्ग;
#endif

    स्वभाव: /* AF_INET */
        sin = (संरचना sockaddr_in *) addr->sockaddr;
        sin->sin_addr.s_addr = inaddr;
        भङ्ग;
    }

    पटिनिवत्त NGX_OK;
}


ngx_int_t
ngx_parse_addr_port(ngx_pool_t *pool, ngx_addr_t *addr, u_char *text,
    size_t len)
{
    u_char     *p, *last;
    size_t      plen;
    ngx_int_t   rc, port;

    rc = ngx_parse_addr(pool, addr, text, len);

    सचे (rc != NGX_DECLINED) {
        पटिनिवत्त rc;
    }

    last = text + len;

#सचे (NGX_HAVE_INET6)
    सचे (len && text[0] == '[') {

        p = ngx_strlchr(text, last, ']');

        सचे (p == last - 1) {
            पटिनिवत्त ngx_parse_addr(pool, addr, text + 1, len - 2);
        }

        सचे (p == रिक्त || *++p != ':') {
            पटिनिवत्त NGX_DECLINED;
        }

        text++;
        len -= 2;

    } अञ्ञथा
#endif

    {
        p = ngx_strlchr(text, last, ':');

        सचे (p == रिक्त) {
            पटिनिवत्त NGX_DECLINED;
        }
    }

    p++;
    plen = last - p;

    port = ngx_atoi(p, plen);

    सचे (port < 1 || port > 65535) {
        पटिनिवत्त NGX_DECLINED;
    }

    len -= plen + 1;

    rc = ngx_parse_addr(pool, addr, text, len);

    सचे (rc != NGX_OK) {
        पटिनिवत्त rc;
    }

    ngx_inet_set_port(addr->sockaddr, (in_port_t) port);

    पटिनिवत्त NGX_OK;
}


ngx_int_t
ngx_parse_url(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char  *p;
    size_t   len;

    p = u->url.data;
    len = u->url.len;

    सचे (len >= 5 && ngx_strncasecmp(p, (u_char *) "unix:", 5) == 0) {
        पटिनिवत्त ngx_parse_unix_domain_url(pool, u);
    }

    सचे (len && p[0] == '[') {
        पटिनिवत्त ngx_parse_inet6_url(pool, u);
    }

    पटिनिवत्त ngx_parse_inet_url(pool, u);
}


स्थिर ngx_int_t
ngx_parse_unix_domain_url(ngx_pool_t *pool, ngx_url_t *u)
{
#सचे (NGX_HAVE_UNIX_DOMAIN)
    u_char              *path, *uri, *last;
    size_t               len;
    संरचना sockaddr_un  *saun;

    len = u->url.len;
    path = u->url.data;

    path += 5;
    len -= 5;

    सचे (u->uri_part) {

        last = path + len;
        uri = ngx_strlchr(path, last, ':');

        सचे (uri) {
            len = uri - path;
            uri++;
            u->uri.len = last - uri;
            u->uri.data = uri;
        }
    }

    सचे (len == 0) {
        u->err = "no path in the unix domain socket";
        पटिनिवत्त NGX_ERROR;
    }

    u->host.len = len++;
    u->host.data = path;

    सचे (len > परिमाण(saun->sun_path)) {
        u->err = "too long path in the unix domain socket";
        पटिनिवत्त NGX_ERROR;
    }

    u->socklen = परिमाण(संरचना sockaddr_un);
    saun = (संरचना sockaddr_un *) &u->sockaddr;
    saun->sun_family = AF_UNIX;
    (शून्य) ngx_cpystrn((u_char *) saun->sun_path, path, len);

    u->addrs = ngx_pcalloc(pool, परिमाण(ngx_addr_t));
    सचे (u->addrs == रिक्त) {
        पटिनिवत्त NGX_ERROR;
    }

    saun = ngx_pcalloc(pool, परिमाण(संरचना sockaddr_un));
    सचे (saun == रिक्त) {
        पटिनिवत्त NGX_ERROR;
    }

    u->family = AF_UNIX;
    u->naddrs = 1;

    saun->sun_family = AF_UNIX;
    (शून्य) ngx_cpystrn((u_char *) saun->sun_path, path, len);

    u->addrs[0].sockaddr = (संरचना sockaddr *) saun;
    u->addrs[0].socklen = परिमाण(संरचना sockaddr_un);
    u->addrs[0].name.len = len + 4;
    u->addrs[0].name.data = u->url.data;

    पटिनिवत्त NGX_OK;

#अञ्ञथा

    u->err = "the unix domain sockets are not supported on this platform";

    पटिनिवत्त NGX_ERROR;

#endif
}


स्थिर ngx_int_t
ngx_parse_inet_url(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char              *host, *port, *last, *uri, *args, *dash;
    size_t               len;
    ngx_int_t            n;
    संरचना sockaddr_in  *sin;

    u->socklen = परिमाण(संरचना sockaddr_in);
    sin = (संरचना sockaddr_in *) &u->sockaddr;
    sin->sin_family = AF_INET;

    u->family = AF_INET;

    host = u->url.data;

    last = host + u->url.len;

    port = ngx_strlchr(host, last, ':');

    uri = ngx_strlchr(host, last, '/');

    args = ngx_strlchr(host, last, '?');

    सचे (args) {
        सचे (uri == रिक्त || args < uri) {
            uri = args;
        }
    }

    सचे (uri) {
        सचे (u->listen || !u->uri_part) {
            u->err = "invalid host";
            पटिनिवत्त NGX_ERROR;
        }

        u->uri.len = last - uri;
        u->uri.data = uri;

        last = uri;

        सचे (uri < port) {
            port = रिक्त;
        }
    }

    सचे (port) {
        port++;

        len = last - port;

        सचे (u->listen) {
            dash = ngx_strlchr(port, last, '-');

            सचे (dash) {
                dash++;

                n = ngx_atoi(dash, last - dash);

                सचे (n < 1 || n > 65535) {
                    u->err = "invalid port";
                    पटिनिवत्त NGX_ERROR;
                }

                u->last_port = (in_port_t) n;

                len = dash - port - 1;
            }
        }

        n = ngx_atoi(port, len);

        सचे (n < 1 || n > 65535) {
            u->err = "invalid port";
            पटिनिवत्त NGX_ERROR;
        }

        सचे (u->last_port && n > u->last_port) {
            u->err = "invalid port range";
            पटिनिवत्त NGX_ERROR;
        }

        u->port = (in_port_t) n;
        sin->sin_port = htons((in_port_t) n);

        u->port_text.len = last - port;
        u->port_text.data = port;

        last = port - 1;

    } अञ्ञथा {
        सचे (uri == रिक्त) {

            सचे (u->listen) {

                /* test value as port only */

                len = last - host;

                dash = ngx_strlchr(host, last, '-');

                सचे (dash) {
                    dash++;

                    n = ngx_atoi(dash, last - dash);

                    सचे (n == NGX_ERROR) {
                        गच्छ no_port;
                    }

                    सचे (n < 1 || n > 65535) {
                        u->err = "invalid port";

                    } अञ्ञथा {
                        u->last_port = (in_port_t) n;
                    }

                    len = dash - host - 1;
                }

                n = ngx_atoi(host, len);

                सचे (n != NGX_ERROR) {

                    सचे (u->err) {
                        पटिनिवत्त NGX_ERROR;
                    }

                    सचे (n < 1 || n > 65535) {
                        u->err = "invalid port";
                        पटिनिवत्त NGX_ERROR;
                    }

                    सचे (u->last_port && n > u->last_port) {
                        u->err = "invalid port range";
                        पटिनिवत्त NGX_ERROR;
                    }

                    u->port = (in_port_t) n;
                    sin->sin_port = htons((in_port_t) n);
                    sin->sin_addr.s_addr = INADDR_ANY;

                    u->port_text.len = last - host;
                    u->port_text.data = host;

                    u->wildcard = 1;

                    पटिनिवत्त ngx_inet_add_addr(pool, u, &u->sockaddr.sockaddr,
                                             u->socklen, 1);
                }
            }
        }

no_port:

        u->err = रिक्त;
        u->no_port = 1;
        u->port = u->default_port;
        sin->sin_port = htons(u->default_port);
        u->last_port = 0;
    }

    len = last - host;

    सचे (len == 0) {
        u->err = "no host";
        पटिनिवत्त NGX_ERROR;
    }

    u->host.len = len;
    u->host.data = host;

    सचे (u->listen && len == 1 && *host == '*') {
        sin->sin_addr.s_addr = INADDR_ANY;
        u->wildcard = 1;
        पटिनिवत्त ngx_inet_add_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);
    }

    sin->sin_addr.s_addr = ngx_inet_addr(host, len);

    सचे (sin->sin_addr.s_addr != INADDR_NONE) {

        सचे (sin->sin_addr.s_addr == INADDR_ANY) {
            u->wildcard = 1;
        }

        पटिनिवत्त ngx_inet_add_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);
    }

    सचे (u->no_resolve) {
        पटिनिवत्त NGX_OK;
    }

    सचे (ngx_inet_resolve_host(pool, u) != NGX_OK) {
        पटिनिवत्त NGX_ERROR;
    }

    u->family = u->addrs[0].sockaddr->sa_family;
    u->socklen = u->addrs[0].socklen;
    ngx_memcpy(&u->sockaddr, u->addrs[0].sockaddr, u->addrs[0].socklen);
    u->wildcard = ngx_inet_wildcard(&u->sockaddr.sockaddr);

    पटिनिवत्त NGX_OK;
}


स्थिर ngx_int_t
ngx_parse_inet6_url(ngx_pool_t *pool, ngx_url_t *u)
{
#सचे (NGX_HAVE_INET6)
    u_char               *p, *host, *port, *last, *uri, *dash;
    size_t                len;
    ngx_int_t             n;
    संरचना sockaddr_in6  *sin6;

    u->socklen = परिमाण(संरचना sockaddr_in6);
    sin6 = (संरचना sockaddr_in6 *) &u->sockaddr;
    sin6->sin6_family = AF_INET6;

    host = u->url.data + 1;

    last = u->url.data + u->url.len;

    p = ngx_strlchr(host, last, ']');

    सचे (p == रिक्त) {
        u->err = "invalid host";
        पटिनिवत्त NGX_ERROR;
    }

    port = p + 1;

    uri = ngx_strlchr(port, last, '/');

    सचे (uri) {
        सचे (u->listen || !u->uri_part) {
            u->err = "invalid host";
            पटिनिवत्त NGX_ERROR;
        }

        u->uri.len = last - uri;
        u->uri.data = uri;

        last = uri;
    }

    सचे (port < last) {
        सचे (*port != ':') {
            u->err = "invalid host";
            पटिनिवत्त NGX_ERROR;
        }

        port++;

        len = last - port;

        सचे (u->listen) {
            dash = ngx_strlchr(port, last, '-');

            सचे (dash) {
                dash++;

                n = ngx_atoi(dash, last - dash);

                सचे (n < 1 || n > 65535) {
                    u->err = "invalid port";
                    पटिनिवत्त NGX_ERROR;
                }

                u->last_port = (in_port_t) n;

                len = dash - port - 1;
            }
        }

        n = ngx_atoi(port, len);

        सचे (n < 1 || n > 65535) {
            u->err = "invalid port";
            पटिनिवत्त NGX_ERROR;
        }

        सचे (u->last_port && n > u->last_port) {
            u->err = "invalid port range";
            पटिनिवत्त NGX_ERROR;
        }

        u->port = (in_port_t) n;
        sin6->sin6_port = htons((in_port_t) n);

        u->port_text.len = last - port;
        u->port_text.data = port;

    } अञ्ञथा {
        u->no_port = 1;
        u->port = u->default_port;
        sin6->sin6_port = htons(u->default_port);
    }

    len = p - host;

    सचे (len == 0) {
        u->err = "no host";
        पटिनिवत्त NGX_ERROR;
    }

    u->host.len = len + 2;
    u->host.data = host - 1;

    सचे (ngx_inet6_addr(host, len, sin6->sin6_addr.s6_addr) != NGX_OK) {
        u->err = "invalid IPv6 address";
        पटिनिवत्त NGX_ERROR;
    }

    सचे (IN6_IS_ADDR_UNSPECIFIED(&sin6->sin6_addr)) {
        u->wildcard = 1;
    }

    u->family = AF_INET6;

    पटिनिवत्त ngx_inet_add_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);

#अञ्ञथा

    u->err = "the INET6 sockets are not supported on this platform";

    पटिनिवत्त NGX_ERROR;

#endif
}


#सचे (NGX_HAVE_GETADDRINFO && NGX_HAVE_INET6)

ngx_int_t
ngx_inet_resolve_host(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char           *host;
    ngx_uint_t        n;
    संरचना addrinfo   hints, *res, *rp;

    host = ngx_alloc(u->host.len + 1, pool->log);
    सचे (host == रिक्त) {
        पटिनिवत्त NGX_ERROR;
    }

    (शून्य) ngx_cpystrn(host, u->host.data, u->host.len + 1);

    ngx_memzero(&hints, परिमाण(संरचना addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
#ifdef AI_ADDRCONFIG
    hints.ai_flags = AI_ADDRCONFIG;
#endif

    सचे (getaddrinfo((वर्ण *) host, रिक्त, &hints, &res) != 0) {
        u->err = "host not found";
        ngx_free(host);
        पटिनिवत्त NGX_ERROR;
    }

    ngx_free(host);

    अत्थाय (n = 0, rp = res; rp != रिक्त; rp = rp->ai_next) {

        विकल्प (rp->ai_family) {

        पक्ष AF_INET:
        पक्ष AF_INET6:
            भङ्ग;

        स्वभाव:
            अनुवर्त;
        }

        n++;
    }

    सचे (n == 0) {
        u->err = "host not found";
        गच्छ failed;
    }

    /* MP: ngx_shared_palloc() */

    अत्थाय (rp = res; rp != रिक्त; rp = rp->ai_next) {

        विकल्प (rp->ai_family) {

        पक्ष AF_INET:
        पक्ष AF_INET6:
            भङ्ग;

        स्वभाव:
            अनुवर्त;
        }

        सचे (ngx_inet_add_addr(pool, u, rp->ai_addr, rp->ai_addrlen, n)
            != NGX_OK)
        {
            गच्छ failed;
        }
    }

    freeaddrinfo(res);
    पटिनिवत्त NGX_OK;

failed:

    freeaddrinfo(res);
    पटिनिवत्त NGX_ERROR;
}

#अञ्ञथा /* !NGX_HAVE_GETADDRINFO || !NGX_HAVE_INET6 */

ngx_int_t
ngx_inet_resolve_host(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char              *host;
    ngx_uint_t           i, n;
    संरचना hostent      *h;
    संरचना sockaddr_in   sin;

    /* AF_INET only */

    ngx_memzero(&sin, परिमाण(संरचना sockaddr_in));

    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = ngx_inet_addr(u->host.data, u->host.len);

    सचे (sin.sin_addr.s_addr == INADDR_NONE) {
        host = ngx_alloc(u->host.len + 1, pool->log);
        सचे (host == रिक्त) {
            पटिनिवत्त NGX_ERROR;
        }

        (शून्य) ngx_cpystrn(host, u->host.data, u->host.len + 1);

        h = gethostbyname((वर्ण *) host);

        ngx_free(host);

        सचे (h == रिक्त || h->h_addr_list[0] == रिक्त) {
            u->err = "host not found";
            पटिनिवत्त NGX_ERROR;
        }

        अत्थाय (n = 0; h->h_addr_list[n] != रिक्त; n++) { /* void */ }

        /* MP: ngx_shared_palloc() */

        अत्थाय (i = 0; i < n; i++) {
            sin.sin_addr.s_addr = *(in_addr_t *) (h->h_addr_list[i]);

            सचे (ngx_inet_add_addr(pool, u, (संरचना sockaddr *) &sin,
                                  परिमाण(संरचना sockaddr_in), n)
                != NGX_OK)
            {
                पटिनिवत्त NGX_ERROR;
            }
        }

    } अञ्ञथा {

        /* MP: ngx_shared_palloc() */

        सचे (ngx_inet_add_addr(pool, u, (संरचना sockaddr *) &sin,
                              परिमाण(संरचना sockaddr_in), 1)
            != NGX_OK)
        {
            पटिनिवत्त NGX_ERROR;
        }
    }

    पटिनिवत्त NGX_OK;
}

#endif /* NGX_HAVE_GETADDRINFO && NGX_HAVE_INET6 */


स्थिर ngx_int_t
ngx_inet_add_addr(ngx_pool_t *pool, ngx_url_t *u, संरचना sockaddr *sockaddr,
    socklen_t socklen, ngx_uint_t total)
{
    u_char           *p;
    size_t            len;
    ngx_uint_t        i, nports;
    ngx_addr_t       *addr;
    संरचना sockaddr  *sa;

    nports = u->last_port ? u->last_port - u->port + 1 : 1;

    सचे (u->addrs == रिक्त) {
        u->addrs = ngx_palloc(pool, total * nports * परिमाण(ngx_addr_t));
        सचे (u->addrs == रिक्त) {
            पटिनिवत्त NGX_ERROR;
        }
    }

    अत्थाय (i = 0; i < nports; i++) {
        sa = ngx_pcalloc(pool, socklen);
        सचे (sa == रिक्त) {
            पटिनिवत्त NGX_ERROR;
        }

        ngx_memcpy(sa, sockaddr, socklen);

        ngx_inet_set_port(sa, u->port + i);

        विकल्प (sa->sa_family) {

#सचे (NGX_HAVE_INET6)
        पक्ष AF_INET6:
            len = NGX_INET6_ADDRSTRLEN + परिमाण("[]:65536") - 1;
            भङ्ग;
#endif

        स्वभाव: /* AF_INET */
            len = NGX_INET_ADDRSTRLEN + परिमाण(":65535") - 1;
        }

        p = ngx_pnalloc(pool, len);
        सचे (p == रिक्त) {
            पटिनिवत्त NGX_ERROR;
        }

        len = ngx_sock_ntop(sa, socklen, p, len, 1);

        addr = &u->addrs[u->naddrs++];

        addr->sockaddr = sa;
        addr->socklen = socklen;

        addr->name.len = len;
        addr->name.data = p;
    }

    पटिनिवत्त NGX_OK;
}


ngx_int_t
ngx_cmp_sockaddr(संरचना sockaddr *sa1, socklen_t slen1,
    संरचना sockaddr *sa2, socklen_t slen2, ngx_uint_t cmp_port)
{
    संरचना sockaddr_in   *sin1, *sin2;
#सचे (NGX_HAVE_INET6)
    संरचना sockaddr_in6  *sin61, *sin62;
#endif
#सचे (NGX_HAVE_UNIX_DOMAIN)
    size_t                len;
    संरचना sockaddr_un   *saun1, *saun2;
#endif

    सचे (sa1->sa_family != sa2->sa_family) {
        पटिनिवत्त NGX_DECLINED;
    }

    विकल्प (sa1->sa_family) {

#सचे (NGX_HAVE_INET6)
    पक्ष AF_INET6:

        sin61 = (संरचना sockaddr_in6 *) sa1;
        sin62 = (संरचना sockaddr_in6 *) sa2;

        सचे (cmp_port && sin61->sin6_port != sin62->sin6_port) {
            पटिनिवत्त NGX_DECLINED;
        }

        सचे (ngx_memcmp(&sin61->sin6_addr, &sin62->sin6_addr, 16) != 0) {
            पटिनिवत्त NGX_DECLINED;
        }

        भङ्ग;
#endif

#सचे (NGX_HAVE_UNIX_DOMAIN)
    पक्ष AF_UNIX:

        saun1 = (संरचना sockaddr_un *) sa1;
        saun2 = (संरचना sockaddr_un *) sa2;

        सचे (slen1 < slen2) {
            len = slen1 - offsetof(संरचना sockaddr_un, sun_path);

        } अञ्ञथा {
            len = slen2 - offsetof(संरचना sockaddr_un, sun_path);
        }

        सचे (len > परिमाण(saun1->sun_path)) {
            len = परिमाण(saun1->sun_path);
        }

        सचे (ngx_memcmp(&saun1->sun_path, &saun2->sun_path, len) != 0) {
            पटिनिवत्त NGX_DECLINED;
        }

        भङ्ग;
#endif

    स्वभाव: /* AF_INET */

        sin1 = (संरचना sockaddr_in *) sa1;
        sin2 = (संरचना sockaddr_in *) sa2;

        सचे (cmp_port && sin1->sin_port != sin2->sin_port) {
            पटिनिवत्त NGX_DECLINED;
        }

        सचे (sin1->sin_addr.s_addr != sin2->sin_addr.s_addr) {
            पटिनिवत्त NGX_DECLINED;
        }

        भङ्ग;
    }

    पटिनिवत्त NGX_OK;
}


in_port_t
ngx_inet_get_port(संरचना sockaddr *sa)
{
    संरचना sockaddr_in   *sin;
#सचे (NGX_HAVE_INET6)
    संरचना sockaddr_in6  *sin6;
#endif

    विकल्प (sa->sa_family) {

#सचे (NGX_HAVE_INET6)
    पक्ष AF_INET6:
        sin6 = (संरचना sockaddr_in6 *) sa;
        पटिनिवत्त ntohs(sin6->sin6_port);
#endif

#सचे (NGX_HAVE_UNIX_DOMAIN)
    पक्ष AF_UNIX:
        पटिनिवत्त 0;
#endif

    स्वभाव: /* AF_INET */
        sin = (संरचना sockaddr_in *) sa;
        पटिनिवत्त ntohs(sin->sin_port);
    }
}


शून्य
ngx_inet_set_port(संरचना sockaddr *sa, in_port_t port)
{
    संरचना sockaddr_in   *sin;
#सचे (NGX_HAVE_INET6)
    संरचना sockaddr_in6  *sin6;
#endif

    विकल्प (sa->sa_family) {

#सचे (NGX_HAVE_INET6)
    पक्ष AF_INET6:
        sin6 = (संरचना sockaddr_in6 *) sa;
        sin6->sin6_port = htons(port);
        भङ्ग;
#endif

#सचे (NGX_HAVE_UNIX_DOMAIN)
    पक्ष AF_UNIX:
        भङ्ग;
#endif

    स्वभाव: /* AF_INET */
        sin = (संरचना sockaddr_in *) sa;
        sin->sin_port = htons(port);
        भङ्ग;
    }
}


ngx_uint_t
ngx_inet_wildcard(संरचना sockaddr *sa)
{
    संरचना sockaddr_in   *sin;
#सचे (NGX_HAVE_INET6)
    संरचना sockaddr_in6  *sin6;
#endif

    विकल्प (sa->sa_family) {

    पक्ष AF_INET:
        sin = (संरचना sockaddr_in *) sa;

        सचे (sin->sin_addr.s_addr == INADDR_ANY) {
            पटिनिवत्त 1;
        }

        भङ्ग;

#सचे (NGX_HAVE_INET6)

    पक्ष AF_INET6:
        sin6 = (संरचना sockaddr_in6 *) sa;

        सचे (IN6_IS_ADDR_UNSPECIFIED(&sin6->sin6_addr)) {
            पटिनिवत्त 1;
        }

        भङ्ग;

#endif
    }

    पटिनिवत्त 0;
}
