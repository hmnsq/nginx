
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
ngx_inet_addr(u_char *text, माप_प्रकार len)
{
    u_char      *p, c;
    in_addr_t    addr;
    ngx_uint_t   octet, n;

    addr = 0;
    octet = 0;
    n = 0;

    साठी (p = text; p < text + len; p++) {
        c = *p;

        जर (c >= '0' && c <= '9') {
            octet = octet * 10 + (c - '0');

            जर (octet > 255) {
                परत INADDR_NONE;
            }

            सुरूठेवा;
        }

        जर (c == '.') {
            addr = (addr << 8) + octet;
            octet = 0;
            n++;
            सुरूठेवा;
        }

        परत INADDR_NONE;
    }

    जर (n == 3) {
        addr = (addr << 8) + octet;
        परत htonl(addr);
    }

    परत INADDR_NONE;
}


#जर (NGX_HAVE_INET6)

ngx_int_t
ngx_inet6_addr(u_char *p, माप_प्रकार len, u_char *addr)
{
    u_char      c, *zero, *digit, *s, *d;
    माप_प्रकार      len4;
    ngx_uint_t  n, nibbles, word;

    जर (len == 0) {
        परत NGX_ERROR;
    }

    zero = रिक्त;
    digit = रिक्त;
    len4 = 0;
    nibbles = 0;
    word = 0;
    n = 8;

    जर (p[0] == ':') {
        p++;
        len--;
    }

    साठी (/* void */; len; len--) {
        c = *p++;

        जर (c == ':') {
            जर (nibbles) {
                digit = p;
                len4 = len;
                *addr++ = (u_char) (word >> 8);
                *addr++ = (u_char) (word & 0xff);

                जर (--n) {
                    nibbles = 0;
                    word = 0;
                    सुरूठेवा;
                }

            } अन्यथा {
                जर (zero == रिक्त) {
                    digit = p;
                    len4 = len;
                    zero = addr;
                    सुरूठेवा;
                }
            }

            परत NGX_ERROR;
        }

        जर (c == '.' && nibbles) {
            जर (n < 2 || digit == रिक्त) {
                परत NGX_ERROR;
            }

            word = ngx_inet_addr(digit, len4 - 1);
            जर (word == INADDR_NONE) {
                परत NGX_ERROR;
            }

            word = ntohl(word);
            *addr++ = (u_char) ((word >> 24) & 0xff);
            *addr++ = (u_char) ((word >> 16) & 0xff);
            n--;
            थांबा;
        }

        जर (++nibbles > 4) {
            परत NGX_ERROR;
        }

        जर (c >= '0' && c <= '9') {
            word = word * 16 + (c - '0');
            सुरूठेवा;
        }

        c |= 0x20;

        जर (c >= 'a' && c <= 'f') {
            word = word * 16 + (c - 'a') + 10;
            सुरूठेवा;
        }

        परत NGX_ERROR;
    }

    जर (nibbles == 0 && zero == रिक्त) {
        परत NGX_ERROR;
    }

    *addr++ = (u_char) (word >> 8);
    *addr++ = (u_char) (word & 0xff);

    जर (--n) {
        जर (zero) {
            n *= 2;
            s = addr - 1;
            d = s + n;
            पर्यंत (s >= zero) {
                *d-- = *s--;
            }
            ngx_memzero(zero, n);
            परत NGX_OK;
        }

    } अन्यथा {
        जर (zero == रिक्त) {
            परत NGX_OK;
        }
    }

    परत NGX_ERROR;
}

#endif


माप_प्रकार
ngx_sock_ntop(संरचना sockaddr *sa, socklen_t socklen, u_char *text, माप_प्रकार len,
    ngx_uint_t port)
{
    u_char               *p;
#जर (NGX_HAVE_INET6 || NGX_HAVE_UNIX_DOMAIN)
    माप_प्रकार                n;
#endif
    संरचना sockaddr_in   *sin;
#जर (NGX_HAVE_INET6)
    संरचना sockaddr_in6  *sin6;
#endif
#जर (NGX_HAVE_UNIX_DOMAIN)
    संरचना sockaddr_un   *saun;
#endif

    निवड (sa->sa_family) {

    स्थिती AF_INET:

        sin = (संरचना sockaddr_in *) sa;
        p = (u_char *) &sin->sin_addr;

        जर (port) {
            p = ngx_snprintf(text, len, "%ud.%ud.%ud.%ud:%d",
                             p[0], p[1], p[2], p[3], ntohs(sin->sin_port));
        } अन्यथा {
            p = ngx_snprintf(text, len, "%ud.%ud.%ud.%ud",
                             p[0], p[1], p[2], p[3]);
        }

        परत (p - text);

#जर (NGX_HAVE_INET6)

    स्थिती AF_INET6:

        sin6 = (संरचना sockaddr_in6 *) sa;

        n = 0;

        जर (port) {
            text[n++] = '[';
        }

        n = ngx_inet6_ntop(sin6->sin6_addr.s6_addr, &text[n], len);

        जर (port) {
            n = ngx_sprintf(&text[1 + n], "]:%d",
                            ntohs(sin6->sin6_port)) - text;
        }

        परत n;
#endif

#जर (NGX_HAVE_UNIX_DOMAIN)

    स्थिती AF_UNIX:
        saun = (संरचना sockaddr_un *) sa;

        /* on Linux sockaddr might not include sun_path at all */

        जर (socklen <= (socklen_t) दुरत्व(संरचना sockaddr_un, sun_path)) {
            p = ngx_snprintf(text, len, "unix:%Z");

        } अन्यथा {
            n = ngx_strnlen((u_char *) saun->sun_path,
                            socklen - दुरत्व(संरचना sockaddr_un, sun_path));
            p = ngx_snprintf(text, len, "unix:%*s%Z", n, saun->sun_path);
        }

        /* we do not include trailing zero in address length */

        परत (p - text - 1);

#endif

    पूर्वनिर्धारित:
        परत 0;
    }
}


माप_प्रकार
ngx_inet_ntop(पूर्णांक family, शून्य *addr, u_char *text, माप_प्रकार len)
{
    u_char  *p;

    निवड (family) {

    स्थिती AF_INET:

        p = addr;

        परत ngx_snprintf(text, len, "%ud.%ud.%ud.%ud",
                            p[0], p[1], p[2], p[3])
               - text;

#जर (NGX_HAVE_INET6)

    स्थिती AF_INET6:
        परत ngx_inet6_ntop(addr, text, len);

#endif

    पूर्वनिर्धारित:
        परत 0;
    }
}


#जर (NGX_HAVE_INET6)

माप_प्रकार
ngx_inet6_ntop(u_char *p, u_char *text, माप_प्रकार len)
{
    u_char      *dst;
    माप_प्रकार       max, n;
    ngx_uint_t   i, zero, last;

    जर (len < NGX_INET6_ADDRSTRLEN) {
        परत 0;
    }

    zero = (ngx_uint_t) -1;
    last = (ngx_uint_t) -1;
    max = 1;
    n = 0;

    साठी (i = 0; i < 16; i += 2) {

        जर (p[i] || p[i + 1]) {

            जर (max < n) {
                zero = last;
                max = n;
            }

            n = 0;
            सुरूठेवा;
        }

        जर (n++ == 0) {
            last = i;
        }
    }

    जर (max < n) {
        zero = last;
        max = n;
    }

    dst = text;
    n = 16;

    जर (zero == 0) {

        जर ((max == 5 && p[10] == 0xff && p[11] == 0xff)
            || (max == 6)
            || (max == 7 && p[14] != 0 && p[15] != 1))
        {
            n = 12;
        }

        *dst++ = ':';
    }

    साठी (i = 0; i < n; i += 2) {

        जर (i == zero) {
            *dst++ = ':';
            i += (max - 1) * 2;
            सुरूठेवा;
        }

        dst = ngx_sprintf(dst, "%xd", p[i] * 256 + p[i + 1]);

        जर (i < 14) {
            *dst++ = ':';
        }
    }

    जर (n == 12) {
        dst = ngx_sprintf(dst, "%ud.%ud.%ud.%ud", p[12], p[13], p[14], p[15]);
    }

    परत dst - text;
}

#endif


ngx_int_t
ngx_ptocidr(ngx_str_t *text, ngx_cidr_t *cidr)
{
    u_char      *addr, *mask, *last;
    माप_प्रकार       len;
    ngx_int_t    shift;
#जर (NGX_HAVE_INET6)
    ngx_int_t    rc;
    ngx_uint_t   s, i;
#endif

    addr = text->data;
    last = addr + text->len;

    mask = ngx_strlchr(addr, last, '/');
    len = (mask ? mask : last) - addr;

    cidr->u.in.addr = ngx_inet_addr(addr, len);

    जर (cidr->u.in.addr != INADDR_NONE) {
        cidr->family = AF_INET;

        जर (mask == रिक्त) {
            cidr->u.in.mask = 0xffffffff;
            परत NGX_OK;
        }

#जर (NGX_HAVE_INET6)
    } अन्यथा जर (ngx_inet6_addr(addr, len, cidr->u.in6.addr.s6_addr) == NGX_OK) {
        cidr->family = AF_INET6;

        जर (mask == रिक्त) {
            ngx_memset(cidr->u.in6.mask.s6_addr, 0xff, 16);
            परत NGX_OK;
        }

#endif
    } अन्यथा {
        परत NGX_ERROR;
    }

    mask++;

    shift = ngx_atoi(mask, last - mask);
    जर (shift == NGX_ERROR) {
        परत NGX_ERROR;
    }

    निवड (cidr->family) {

#जर (NGX_HAVE_INET6)
    स्थिती AF_INET6:
        जर (shift > 128) {
            परत NGX_ERROR;
        }

        addr = cidr->u.in6.addr.s6_addr;
        mask = cidr->u.in6.mask.s6_addr;
        rc = NGX_OK;

        साठी (i = 0; i < 16; i++) {

            s = (shift > 8) ? 8 : shift;
            shift -= s;

            mask[i] = (u_char) (0xffu << (8 - s));

            जर (addr[i] != (addr[i] & mask[i])) {
                rc = NGX_DONE;
                addr[i] &= mask[i];
            }
        }

        परत rc;
#endif

    पूर्वनिर्धारित: /* AF_INET */
        जर (shift > 32) {
            परत NGX_ERROR;
        }

        जर (shift) {
            cidr->u.in.mask = htonl((uint32_t) (0xffffffffu << (32 - shift)));

        } अन्यथा {
            /* x86 compilers use a shl instruction that shifts by modulo 32 */
            cidr->u.in.mask = 0;
        }

        जर (cidr->u.in.addr == (cidr->u.in.addr & cidr->u.in.mask)) {
            परत NGX_OK;
        }

        cidr->u.in.addr &= cidr->u.in.mask;

        परत NGX_DONE;
    }
}


ngx_int_t
ngx_cidr_match(संरचना sockaddr *sa, ngx_array_t *cidrs)
{
#जर (NGX_HAVE_INET6)
    u_char           *p;
#endif
    in_addr_t         inaddr;
    ngx_cidr_t       *cidr;
    ngx_uint_t        family, i;
#जर (NGX_HAVE_INET6)
    ngx_uint_t        n;
    संरचना in6_addr  *inaddr6;
#endif

#जर (NGX_SUPPRESS_WARN)
    inaddr = 0;
#जर (NGX_HAVE_INET6)
    inaddr6 = रिक्त;
#endif
#endif

    family = sa->sa_family;

    जर (family == AF_INET) {
        inaddr = ((संरचना sockaddr_in *) sa)->sin_addr.s_addr;
    }

#जर (NGX_HAVE_INET6)
    अन्यथा जर (family == AF_INET6) {
        inaddr6 = &((संरचना sockaddr_in6 *) sa)->sin6_addr;

        जर (IN6_IS_ADDR_V4MAPPED(inaddr6)) {
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

    साठी (cidr = cidrs->elts, i = 0; i < cidrs->nelts; i++) {
        जर (cidr[i].family != family) {
            जा next;
        }

        निवड (family) {

#जर (NGX_HAVE_INET6)
        स्थिती AF_INET6:
            साठी (n = 0; n < 16; n++) {
                जर ((inaddr6->s6_addr[n] & cidr[i].u.in6.mask.s6_addr[n])
                    != cidr[i].u.in6.addr.s6_addr[n])
                {
                    जा next;
                }
            }
            थांबा;
#endif

#जर (NGX_HAVE_UNIX_DOMAIN)
        स्थिती AF_UNIX:
            थांबा;
#endif

        पूर्वनिर्धारित: /* AF_INET */
            जर ((inaddr & cidr[i].u.in.mask) != cidr[i].u.in.addr) {
                जा next;
            }
            थांबा;
        }

        परत NGX_OK;

    next:
        सुरूठेवा;
    }

    परत NGX_DECLINED;
}


ngx_int_t
ngx_parse_addr(ngx_pool_t *pool, ngx_addr_t *addr, u_char *text, माप_प्रकार len)
{
    in_addr_t             inaddr;
    ngx_uint_t            family;
    संरचना sockaddr_in   *sin;
#जर (NGX_HAVE_INET6)
    संरचना in6_addr       inaddr6;
    संरचना sockaddr_in6  *sin6;

    /*
     * prevent MSVC8 warning:
     *    potentially uninitialized local variable 'inaddr6' used
     */
    ngx_memzero(&inaddr6, आकार(संरचना in6_addr));
#endif

    inaddr = ngx_inet_addr(text, len);

    जर (inaddr != INADDR_NONE) {
        family = AF_INET;
        len = आकार(संरचना sockaddr_in);

#जर (NGX_HAVE_INET6)
    } अन्यथा जर (ngx_inet6_addr(text, len, inaddr6.s6_addr) == NGX_OK) {
        family = AF_INET6;
        len = आकार(संरचना sockaddr_in6);

#endif
    } अन्यथा {
        परत NGX_DECLINED;
    }

    addr->sockaddr = ngx_pcalloc(pool, len);
    जर (addr->sockaddr == रिक्त) {
        परत NGX_ERROR;
    }

    addr->sockaddr->sa_family = (u_char) family;
    addr->socklen = len;

    निवड (family) {

#जर (NGX_HAVE_INET6)
    स्थिती AF_INET6:
        sin6 = (संरचना sockaddr_in6 *) addr->sockaddr;
        ngx_memcpy(sin6->sin6_addr.s6_addr, inaddr6.s6_addr, 16);
        थांबा;
#endif

    पूर्वनिर्धारित: /* AF_INET */
        sin = (संरचना sockaddr_in *) addr->sockaddr;
        sin->sin_addr.s_addr = inaddr;
        थांबा;
    }

    परत NGX_OK;
}


ngx_int_t
ngx_parse_addr_port(ngx_pool_t *pool, ngx_addr_t *addr, u_char *text,
    माप_प्रकार len)
{
    u_char     *p, *last;
    माप_प्रकार      plen;
    ngx_int_t   rc, port;

    rc = ngx_parse_addr(pool, addr, text, len);

    जर (rc != NGX_DECLINED) {
        परत rc;
    }

    last = text + len;

#जर (NGX_HAVE_INET6)
    जर (len && text[0] == '[') {

        p = ngx_strlchr(text, last, ']');

        जर (p == last - 1) {
            परत ngx_parse_addr(pool, addr, text + 1, len - 2);
        }

        जर (p == रिक्त || *++p != ':') {
            परत NGX_DECLINED;
        }

        text++;
        len -= 2;

    } अन्यथा
#endif

    {
        p = ngx_strlchr(text, last, ':');

        जर (p == रिक्त) {
            परत NGX_DECLINED;
        }
    }

    p++;
    plen = last - p;

    port = ngx_atoi(p, plen);

    जर (port < 1 || port > 65535) {
        परत NGX_DECLINED;
    }

    len -= plen + 1;

    rc = ngx_parse_addr(pool, addr, text, len);

    जर (rc != NGX_OK) {
        परत rc;
    }

    ngx_inet_set_port(addr->sockaddr, (in_port_t) port);

    परत NGX_OK;
}


ngx_int_t
ngx_parse_url(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char  *p;
    माप_प्रकार   len;

    p = u->url.data;
    len = u->url.len;

    जर (len >= 5 && ngx_strncasecmp(p, (u_char *) "unix:", 5) == 0) {
        परत ngx_parse_unix_domain_url(pool, u);
    }

    जर (len && p[0] == '[') {
        परत ngx_parse_inet6_url(pool, u);
    }

    परत ngx_parse_inet_url(pool, u);
}


स्थिर ngx_int_t
ngx_parse_unix_domain_url(ngx_pool_t *pool, ngx_url_t *u)
{
#जर (NGX_HAVE_UNIX_DOMAIN)
    u_char              *path, *uri, *last;
    माप_प्रकार               len;
    संरचना sockaddr_un  *saun;

    len = u->url.len;
    path = u->url.data;

    path += 5;
    len -= 5;

    जर (u->uri_part) {

        last = path + len;
        uri = ngx_strlchr(path, last, ':');

        जर (uri) {
            len = uri - path;
            uri++;
            u->uri.len = last - uri;
            u->uri.data = uri;
        }
    }

    जर (len == 0) {
        u->err = "no path in the unix domain socket";
        परत NGX_ERROR;
    }

    u->host.len = len++;
    u->host.data = path;

    जर (len > आकार(saun->sun_path)) {
        u->err = "too long path in the unix domain socket";
        परत NGX_ERROR;
    }

    u->socklen = आकार(संरचना sockaddr_un);
    saun = (संरचना sockaddr_un *) &u->sockaddr;
    saun->sun_family = AF_UNIX;
    (शून्य) ngx_cpystrn((u_char *) saun->sun_path, path, len);

    u->addrs = ngx_pcalloc(pool, आकार(ngx_addr_t));
    जर (u->addrs == रिक्त) {
        परत NGX_ERROR;
    }

    saun = ngx_pcalloc(pool, आकार(संरचना sockaddr_un));
    जर (saun == रिक्त) {
        परत NGX_ERROR;
    }

    u->family = AF_UNIX;
    u->naddrs = 1;

    saun->sun_family = AF_UNIX;
    (शून्य) ngx_cpystrn((u_char *) saun->sun_path, path, len);

    u->addrs[0].sockaddr = (संरचना sockaddr *) saun;
    u->addrs[0].socklen = आकार(संरचना sockaddr_un);
    u->addrs[0].name.len = len + 4;
    u->addrs[0].name.data = u->url.data;

    परत NGX_OK;

#अन्यथा

    u->err = "the unix domain sockets are not supported on this platform";

    परत NGX_ERROR;

#endif
}


स्थिर ngx_int_t
ngx_parse_inet_url(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char              *host, *port, *last, *uri, *args, *dash;
    माप_प्रकार               len;
    ngx_int_t            n;
    संरचना sockaddr_in  *sin;

    u->socklen = आकार(संरचना sockaddr_in);
    sin = (संरचना sockaddr_in *) &u->sockaddr;
    sin->sin_family = AF_INET;

    u->family = AF_INET;

    host = u->url.data;

    last = host + u->url.len;

    port = ngx_strlchr(host, last, ':');

    uri = ngx_strlchr(host, last, '/');

    args = ngx_strlchr(host, last, '?');

    जर (args) {
        जर (uri == रिक्त || args < uri) {
            uri = args;
        }
    }

    जर (uri) {
        जर (u->listen || !u->uri_part) {
            u->err = "invalid host";
            परत NGX_ERROR;
        }

        u->uri.len = last - uri;
        u->uri.data = uri;

        last = uri;

        जर (uri < port) {
            port = रिक्त;
        }
    }

    जर (port) {
        port++;

        len = last - port;

        जर (u->listen) {
            dash = ngx_strlchr(port, last, '-');

            जर (dash) {
                dash++;

                n = ngx_atoi(dash, last - dash);

                जर (n < 1 || n > 65535) {
                    u->err = "invalid port";
                    परत NGX_ERROR;
                }

                u->last_port = (in_port_t) n;

                len = dash - port - 1;
            }
        }

        n = ngx_atoi(port, len);

        जर (n < 1 || n > 65535) {
            u->err = "invalid port";
            परत NGX_ERROR;
        }

        जर (u->last_port && n > u->last_port) {
            u->err = "invalid port range";
            परत NGX_ERROR;
        }

        u->port = (in_port_t) n;
        sin->sin_port = htons((in_port_t) n);

        u->port_text.len = last - port;
        u->port_text.data = port;

        last = port - 1;

    } अन्यथा {
        जर (uri == रिक्त) {

            जर (u->listen) {

                /* test value as port only */

                len = last - host;

                dash = ngx_strlchr(host, last, '-');

                जर (dash) {
                    dash++;

                    n = ngx_atoi(dash, last - dash);

                    जर (n == NGX_ERROR) {
                        जा no_port;
                    }

                    जर (n < 1 || n > 65535) {
                        u->err = "invalid port";

                    } अन्यथा {
                        u->last_port = (in_port_t) n;
                    }

                    len = dash - host - 1;
                }

                n = ngx_atoi(host, len);

                जर (n != NGX_ERROR) {

                    जर (u->err) {
                        परत NGX_ERROR;
                    }

                    जर (n < 1 || n > 65535) {
                        u->err = "invalid port";
                        परत NGX_ERROR;
                    }

                    जर (u->last_port && n > u->last_port) {
                        u->err = "invalid port range";
                        परत NGX_ERROR;
                    }

                    u->port = (in_port_t) n;
                    sin->sin_port = htons((in_port_t) n);
                    sin->sin_addr.s_addr = INADDR_ANY;

                    u->port_text.len = last - host;
                    u->port_text.data = host;

                    u->wildcard = 1;

                    परत ngx_inet_add_addr(pool, u, &u->sockaddr.sockaddr,
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

    जर (len == 0) {
        u->err = "no host";
        परत NGX_ERROR;
    }

    u->host.len = len;
    u->host.data = host;

    जर (u->listen && len == 1 && *host == '*') {
        sin->sin_addr.s_addr = INADDR_ANY;
        u->wildcard = 1;
        परत ngx_inet_add_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);
    }

    sin->sin_addr.s_addr = ngx_inet_addr(host, len);

    जर (sin->sin_addr.s_addr != INADDR_NONE) {

        जर (sin->sin_addr.s_addr == INADDR_ANY) {
            u->wildcard = 1;
        }

        परत ngx_inet_add_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);
    }

    जर (u->no_resolve) {
        परत NGX_OK;
    }

    जर (ngx_inet_resolve_host(pool, u) != NGX_OK) {
        परत NGX_ERROR;
    }

    u->family = u->addrs[0].sockaddr->sa_family;
    u->socklen = u->addrs[0].socklen;
    ngx_memcpy(&u->sockaddr, u->addrs[0].sockaddr, u->addrs[0].socklen);
    u->wildcard = ngx_inet_wildcard(&u->sockaddr.sockaddr);

    परत NGX_OK;
}


स्थिर ngx_int_t
ngx_parse_inet6_url(ngx_pool_t *pool, ngx_url_t *u)
{
#जर (NGX_HAVE_INET6)
    u_char               *p, *host, *port, *last, *uri, *dash;
    माप_प्रकार                len;
    ngx_int_t             n;
    संरचना sockaddr_in6  *sin6;

    u->socklen = आकार(संरचना sockaddr_in6);
    sin6 = (संरचना sockaddr_in6 *) &u->sockaddr;
    sin6->sin6_family = AF_INET6;

    host = u->url.data + 1;

    last = u->url.data + u->url.len;

    p = ngx_strlchr(host, last, ']');

    जर (p == रिक्त) {
        u->err = "invalid host";
        परत NGX_ERROR;
    }

    port = p + 1;

    uri = ngx_strlchr(port, last, '/');

    जर (uri) {
        जर (u->listen || !u->uri_part) {
            u->err = "invalid host";
            परत NGX_ERROR;
        }

        u->uri.len = last - uri;
        u->uri.data = uri;

        last = uri;
    }

    जर (port < last) {
        जर (*port != ':') {
            u->err = "invalid host";
            परत NGX_ERROR;
        }

        port++;

        len = last - port;

        जर (u->listen) {
            dash = ngx_strlchr(port, last, '-');

            जर (dash) {
                dash++;

                n = ngx_atoi(dash, last - dash);

                जर (n < 1 || n > 65535) {
                    u->err = "invalid port";
                    परत NGX_ERROR;
                }

                u->last_port = (in_port_t) n;

                len = dash - port - 1;
            }
        }

        n = ngx_atoi(port, len);

        जर (n < 1 || n > 65535) {
            u->err = "invalid port";
            परत NGX_ERROR;
        }

        जर (u->last_port && n > u->last_port) {
            u->err = "invalid port range";
            परत NGX_ERROR;
        }

        u->port = (in_port_t) n;
        sin6->sin6_port = htons((in_port_t) n);

        u->port_text.len = last - port;
        u->port_text.data = port;

    } अन्यथा {
        u->no_port = 1;
        u->port = u->default_port;
        sin6->sin6_port = htons(u->default_port);
    }

    len = p - host;

    जर (len == 0) {
        u->err = "no host";
        परत NGX_ERROR;
    }

    u->host.len = len + 2;
    u->host.data = host - 1;

    जर (ngx_inet6_addr(host, len, sin6->sin6_addr.s6_addr) != NGX_OK) {
        u->err = "invalid IPv6 address";
        परत NGX_ERROR;
    }

    जर (IN6_IS_ADDR_UNSPECIFIED(&sin6->sin6_addr)) {
        u->wildcard = 1;
    }

    u->family = AF_INET6;

    परत ngx_inet_add_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);

#अन्यथा

    u->err = "the INET6 sockets are not supported on this platform";

    परत NGX_ERROR;

#endif
}


#जर (NGX_HAVE_GETADDRINFO && NGX_HAVE_INET6)

ngx_int_t
ngx_inet_resolve_host(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char           *host;
    ngx_uint_t        n;
    संरचना addrinfo   hints, *res, *rp;

    host = ngx_alloc(u->host.len + 1, pool->log);
    जर (host == रिक्त) {
        परत NGX_ERROR;
    }

    (शून्य) ngx_cpystrn(host, u->host.data, u->host.len + 1);

    ngx_memzero(&hints, आकार(संरचना addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
#ifdef AI_ADDRCONFIG
    hints.ai_flags = AI_ADDRCONFIG;
#endif

    जर (getaddrinfo((अक्षर *) host, रिक्त, &hints, &res) != 0) {
        u->err = "host not found";
        ngx_free(host);
        परत NGX_ERROR;
    }

    ngx_free(host);

    साठी (n = 0, rp = res; rp != रिक्त; rp = rp->ai_next) {

        निवड (rp->ai_family) {

        स्थिती AF_INET:
        स्थिती AF_INET6:
            थांबा;

        पूर्वनिर्धारित:
            सुरूठेवा;
        }

        n++;
    }

    जर (n == 0) {
        u->err = "host not found";
        जा failed;
    }

    /* MP: ngx_shared_palloc() */

    साठी (rp = res; rp != रिक्त; rp = rp->ai_next) {

        निवड (rp->ai_family) {

        स्थिती AF_INET:
        स्थिती AF_INET6:
            थांबा;

        पूर्वनिर्धारित:
            सुरूठेवा;
        }

        जर (ngx_inet_add_addr(pool, u, rp->ai_addr, rp->ai_addrlen, n)
            != NGX_OK)
        {
            जा failed;
        }
    }

    freeaddrinfo(res);
    परत NGX_OK;

failed:

    freeaddrinfo(res);
    परत NGX_ERROR;
}

#अन्यथा /* !NGX_HAVE_GETADDRINFO || !NGX_HAVE_INET6 */

ngx_int_t
ngx_inet_resolve_host(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char              *host;
    ngx_uint_t           i, n;
    संरचना hostent      *h;
    संरचना sockaddr_in   sin;

    /* AF_INET only */

    ngx_memzero(&sin, आकार(संरचना sockaddr_in));

    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = ngx_inet_addr(u->host.data, u->host.len);

    जर (sin.sin_addr.s_addr == INADDR_NONE) {
        host = ngx_alloc(u->host.len + 1, pool->log);
        जर (host == रिक्त) {
            परत NGX_ERROR;
        }

        (शून्य) ngx_cpystrn(host, u->host.data, u->host.len + 1);

        h = gethostbyname((अक्षर *) host);

        ngx_free(host);

        जर (h == रिक्त || h->h_addr_list[0] == रिक्त) {
            u->err = "host not found";
            परत NGX_ERROR;
        }

        साठी (n = 0; h->h_addr_list[n] != रिक्त; n++) { /* void */ }

        /* MP: ngx_shared_palloc() */

        साठी (i = 0; i < n; i++) {
            sin.sin_addr.s_addr = *(in_addr_t *) (h->h_addr_list[i]);

            जर (ngx_inet_add_addr(pool, u, (संरचना sockaddr *) &sin,
                                  आकार(संरचना sockaddr_in), n)
                != NGX_OK)
            {
                परत NGX_ERROR;
            }
        }

    } अन्यथा {

        /* MP: ngx_shared_palloc() */

        जर (ngx_inet_add_addr(pool, u, (संरचना sockaddr *) &sin,
                              आकार(संरचना sockaddr_in), 1)
            != NGX_OK)
        {
            परत NGX_ERROR;
        }
    }

    परत NGX_OK;
}

#endif /* NGX_HAVE_GETADDRINFO && NGX_HAVE_INET6 */


स्थिर ngx_int_t
ngx_inet_add_addr(ngx_pool_t *pool, ngx_url_t *u, संरचना sockaddr *sockaddr,
    socklen_t socklen, ngx_uint_t total)
{
    u_char           *p;
    माप_प्रकार            len;
    ngx_uint_t        i, nports;
    ngx_addr_t       *addr;
    संरचना sockaddr  *sa;

    nports = u->last_port ? u->last_port - u->port + 1 : 1;

    जर (u->addrs == रिक्त) {
        u->addrs = ngx_palloc(pool, total * nports * आकार(ngx_addr_t));
        जर (u->addrs == रिक्त) {
            परत NGX_ERROR;
        }
    }

    साठी (i = 0; i < nports; i++) {
        sa = ngx_pcalloc(pool, socklen);
        जर (sa == रिक्त) {
            परत NGX_ERROR;
        }

        ngx_memcpy(sa, sockaddr, socklen);

        ngx_inet_set_port(sa, u->port + i);

        निवड (sa->sa_family) {

#जर (NGX_HAVE_INET6)
        स्थिती AF_INET6:
            len = NGX_INET6_ADDRSTRLEN + आकार("[]:65536") - 1;
            थांबा;
#endif

        पूर्वनिर्धारित: /* AF_INET */
            len = NGX_INET_ADDRSTRLEN + आकार(":65535") - 1;
        }

        p = ngx_pnalloc(pool, len);
        जर (p == रिक्त) {
            परत NGX_ERROR;
        }

        len = ngx_sock_ntop(sa, socklen, p, len, 1);

        addr = &u->addrs[u->naddrs++];

        addr->sockaddr = sa;
        addr->socklen = socklen;

        addr->name.len = len;
        addr->name.data = p;
    }

    परत NGX_OK;
}


ngx_int_t
ngx_cmp_sockaddr(संरचना sockaddr *sa1, socklen_t slen1,
    संरचना sockaddr *sa2, socklen_t slen2, ngx_uint_t cmp_port)
{
    संरचना sockaddr_in   *sin1, *sin2;
#जर (NGX_HAVE_INET6)
    संरचना sockaddr_in6  *sin61, *sin62;
#endif
#जर (NGX_HAVE_UNIX_DOMAIN)
    माप_प्रकार                len;
    संरचना sockaddr_un   *saun1, *saun2;
#endif

    जर (sa1->sa_family != sa2->sa_family) {
        परत NGX_DECLINED;
    }

    निवड (sa1->sa_family) {

#जर (NGX_HAVE_INET6)
    स्थिती AF_INET6:

        sin61 = (संरचना sockaddr_in6 *) sa1;
        sin62 = (संरचना sockaddr_in6 *) sa2;

        जर (cmp_port && sin61->sin6_port != sin62->sin6_port) {
            परत NGX_DECLINED;
        }

        जर (ngx_memcmp(&sin61->sin6_addr, &sin62->sin6_addr, 16) != 0) {
            परत NGX_DECLINED;
        }

        थांबा;
#endif

#जर (NGX_HAVE_UNIX_DOMAIN)
    स्थिती AF_UNIX:

        saun1 = (संरचना sockaddr_un *) sa1;
        saun2 = (संरचना sockaddr_un *) sa2;

        जर (slen1 < slen2) {
            len = slen1 - दुरत्व(संरचना sockaddr_un, sun_path);

        } अन्यथा {
            len = slen2 - दुरत्व(संरचना sockaddr_un, sun_path);
        }

        जर (len > आकार(saun1->sun_path)) {
            len = आकार(saun1->sun_path);
        }

        जर (ngx_memcmp(&saun1->sun_path, &saun2->sun_path, len) != 0) {
            परत NGX_DECLINED;
        }

        थांबा;
#endif

    पूर्वनिर्धारित: /* AF_INET */

        sin1 = (संरचना sockaddr_in *) sa1;
        sin2 = (संरचना sockaddr_in *) sa2;

        जर (cmp_port && sin1->sin_port != sin2->sin_port) {
            परत NGX_DECLINED;
        }

        जर (sin1->sin_addr.s_addr != sin2->sin_addr.s_addr) {
            परत NGX_DECLINED;
        }

        थांबा;
    }

    परत NGX_OK;
}


in_port_t
ngx_inet_get_port(संरचना sockaddr *sa)
{
    संरचना sockaddr_in   *sin;
#जर (NGX_HAVE_INET6)
    संरचना sockaddr_in6  *sin6;
#endif

    निवड (sa->sa_family) {

#जर (NGX_HAVE_INET6)
    स्थिती AF_INET6:
        sin6 = (संरचना sockaddr_in6 *) sa;
        परत ntohs(sin6->sin6_port);
#endif

#जर (NGX_HAVE_UNIX_DOMAIN)
    स्थिती AF_UNIX:
        परत 0;
#endif

    पूर्वनिर्धारित: /* AF_INET */
        sin = (संरचना sockaddr_in *) sa;
        परत ntohs(sin->sin_port);
    }
}


शून्य
ngx_inet_set_port(संरचना sockaddr *sa, in_port_t port)
{
    संरचना sockaddr_in   *sin;
#जर (NGX_HAVE_INET6)
    संरचना sockaddr_in6  *sin6;
#endif

    निवड (sa->sa_family) {

#जर (NGX_HAVE_INET6)
    स्थिती AF_INET6:
        sin6 = (संरचना sockaddr_in6 *) sa;
        sin6->sin6_port = htons(port);
        थांबा;
#endif

#जर (NGX_HAVE_UNIX_DOMAIN)
    स्थिती AF_UNIX:
        थांबा;
#endif

    पूर्वनिर्धारित: /* AF_INET */
        sin = (संरचना sockaddr_in *) sa;
        sin->sin_port = htons(port);
        थांबा;
    }
}


ngx_uint_t
ngx_inet_wildcard(संरचना sockaddr *sa)
{
    संरचना sockaddr_in   *sin;
#जर (NGX_HAVE_INET6)
    संरचना sockaddr_in6  *sin6;
#endif

    निवड (sa->sa_family) {

    स्थिती AF_INET:
        sin = (संरचना sockaddr_in *) sa;

        जर (sin->sin_addr.s_addr == INADDR_ANY) {
            परत 1;
        }

        थांबा;

#जर (NGX_HAVE_INET6)

    स्थिती AF_INET6:
        sin6 = (संरचना sockaddr_in6 *) sa;

        जर (IN6_IS_ADDR_UNSPECIFIED(&sin6->sin6_addr)) {
            परत 1;
        }

        थांबा;

#endif
    }

    परत 0;
}
