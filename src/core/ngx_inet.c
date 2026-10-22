
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

    लागि (p = text; p < text + len; p++) {
        c = *p;

        यदि (c >= '0' && c <= '9') {
            octet = octet * 10 + (c - '0');

            यदि (octet > 255) {
                फर्काऊ INADDR_NONE;
            }

            जारी;
        }

        यदि (c == '.') {
            addr = (addr << 8) + octet;
            octet = 0;
            n++;
            जारी;
        }

        फर्काऊ INADDR_NONE;
    }

    यदि (n == 3) {
        addr = (addr << 8) + octet;
        फर्काऊ htonl(addr);
    }

    फर्काऊ INADDR_NONE;
}


#यदि (NGX_HAVE_INET6)

ngx_int_t
ngx_inet6_addr(u_char *p, माप_प्रकार len, u_char *addr)
{
    u_char      c, *zero, *digit, *s, *d;
    माप_प्रकार      len4;
    ngx_uint_t  n, nibbles, word;

    यदि (len == 0) {
        फर्काऊ NGX_ERROR;
    }

    zero = रिक्त;
    digit = रिक्त;
    len4 = 0;
    nibbles = 0;
    word = 0;
    n = 8;

    यदि (p[0] == ':') {
        p++;
        len--;
    }

    लागि (/* void */; len; len--) {
        c = *p++;

        यदि (c == ':') {
            यदि (nibbles) {
                digit = p;
                len4 = len;
                *addr++ = (u_char) (word >> 8);
                *addr++ = (u_char) (word & 0xff);

                यदि (--n) {
                    nibbles = 0;
                    word = 0;
                    जारी;
                }

            } अन्यथा {
                यदि (zero == रिक्त) {
                    digit = p;
                    len4 = len;
                    zero = addr;
                    जारी;
                }
            }

            फर्काऊ NGX_ERROR;
        }

        यदि (c == '.' && nibbles) {
            यदि (n < 2 || digit == रिक्त) {
                फर्काऊ NGX_ERROR;
            }

            word = ngx_inet_addr(digit, len4 - 1);
            यदि (word == INADDR_NONE) {
                फर्काऊ NGX_ERROR;
            }

            word = ntohl(word);
            *addr++ = (u_char) ((word >> 24) & 0xff);
            *addr++ = (u_char) ((word >> 16) & 0xff);
            n--;
            रोक;
        }

        यदि (++nibbles > 4) {
            फर्काऊ NGX_ERROR;
        }

        यदि (c >= '0' && c <= '9') {
            word = word * 16 + (c - '0');
            जारी;
        }

        c |= 0x20;

        यदि (c >= 'a' && c <= 'f') {
            word = word * 16 + (c - 'a') + 10;
            जारी;
        }

        फर्काऊ NGX_ERROR;
    }

    यदि (nibbles == 0 && zero == रिक्त) {
        फर्काऊ NGX_ERROR;
    }

    *addr++ = (u_char) (word >> 8);
    *addr++ = (u_char) (word & 0xff);

    यदि (--n) {
        यदि (zero) {
            n *= 2;
            s = addr - 1;
            d = s + n;
            जबसम्म (s >= zero) {
                *d-- = *s--;
            }
            ngx_memzero(zero, n);
            फर्काऊ NGX_OK;
        }

    } अन्यथा {
        यदि (zero == रिक्त) {
            फर्काऊ NGX_OK;
        }
    }

    फर्काऊ NGX_ERROR;
}

#endif


माप_प्रकार
ngx_sock_ntop(संरचना sockaddr *sa, socklen_t socklen, u_char *text, माप_प्रकार len,
    ngx_uint_t port)
{
    u_char               *p;
#यदि (NGX_HAVE_INET6 || NGX_HAVE_UNIX_DOMAIN)
    माप_प्रकार                n;
#endif
    संरचना sockaddr_in   *sin;
#यदि (NGX_HAVE_INET6)
    संरचना sockaddr_in6  *sin6;
#endif
#यदि (NGX_HAVE_UNIX_DOMAIN)
    संरचना sockaddr_un   *saun;
#endif

    विकल्प (sa->sa_family) {

    पक्ष AF_INET:

        sin = (संरचना sockaddr_in *) sa;
        p = (u_char *) &sin->sin_addr;

        यदि (port) {
            p = ngx_snprintf(text, len, "%ud.%ud.%ud.%ud:%d",
                             p[0], p[1], p[2], p[3], ntohs(sin->sin_port));
        } अन्यथा {
            p = ngx_snprintf(text, len, "%ud.%ud.%ud.%ud",
                             p[0], p[1], p[2], p[3]);
        }

        फर्काऊ (p - text);

#यदि (NGX_HAVE_INET6)

    पक्ष AF_INET6:

        sin6 = (संरचना sockaddr_in6 *) sa;

        n = 0;

        यदि (port) {
            text[n++] = '[';
        }

        n = ngx_inet6_ntop(sin6->sin6_addr.s6_addr, &text[n], len);

        यदि (port) {
            n = ngx_sprintf(&text[1 + n], "]:%d",
                            ntohs(sin6->sin6_port)) - text;
        }

        फर्काऊ n;
#endif

#यदि (NGX_HAVE_UNIX_DOMAIN)

    पक्ष AF_UNIX:
        saun = (संरचना sockaddr_un *) sa;

        /* on Linux sockaddr might not include sun_path at all */

        यदि (socklen <= (socklen_t) दुरत्व(संरचना sockaddr_un, sun_path)) {
            p = ngx_snprintf(text, len, "unix:%Z");

        } अन्यथा {
            n = ngx_strnlen((u_char *) saun->sun_path,
                            socklen - दुरत्व(संरचना sockaddr_un, sun_path));
            p = ngx_snprintf(text, len, "unix:%*s%Z", n, saun->sun_path);
        }

        /* we do not include trailing zero in address length */

        फर्काऊ (p - text - 1);

#endif

    स्वभाव:
        फर्काऊ 0;
    }
}


माप_प्रकार
ngx_inet_ntop(पूर्णाङ्क family, शून्य *addr, u_char *text, माप_प्रकार len)
{
    u_char  *p;

    विकल्प (family) {

    पक्ष AF_INET:

        p = addr;

        फर्काऊ ngx_snprintf(text, len, "%ud.%ud.%ud.%ud",
                            p[0], p[1], p[2], p[3])
               - text;

#यदि (NGX_HAVE_INET6)

    पक्ष AF_INET6:
        फर्काऊ ngx_inet6_ntop(addr, text, len);

#endif

    स्वभाव:
        फर्काऊ 0;
    }
}


#यदि (NGX_HAVE_INET6)

माप_प्रकार
ngx_inet6_ntop(u_char *p, u_char *text, माप_प्रकार len)
{
    u_char      *dst;
    माप_प्रकार       max, n;
    ngx_uint_t   i, zero, last;

    यदि (len < NGX_INET6_ADDRSTRLEN) {
        फर्काऊ 0;
    }

    zero = (ngx_uint_t) -1;
    last = (ngx_uint_t) -1;
    max = 1;
    n = 0;

    लागि (i = 0; i < 16; i += 2) {

        यदि (p[i] || p[i + 1]) {

            यदि (max < n) {
                zero = last;
                max = n;
            }

            n = 0;
            जारी;
        }

        यदि (n++ == 0) {
            last = i;
        }
    }

    यदि (max < n) {
        zero = last;
        max = n;
    }

    dst = text;
    n = 16;

    यदि (zero == 0) {

        यदि ((max == 5 && p[10] == 0xff && p[11] == 0xff)
            || (max == 6)
            || (max == 7 && p[14] != 0 && p[15] != 1))
        {
            n = 12;
        }

        *dst++ = ':';
    }

    लागि (i = 0; i < n; i += 2) {

        यदि (i == zero) {
            *dst++ = ':';
            i += (max - 1) * 2;
            जारी;
        }

        dst = ngx_sprintf(dst, "%xd", p[i] * 256 + p[i + 1]);

        यदि (i < 14) {
            *dst++ = ':';
        }
    }

    यदि (n == 12) {
        dst = ngx_sprintf(dst, "%ud.%ud.%ud.%ud", p[12], p[13], p[14], p[15]);
    }

    फर्काऊ dst - text;
}

#endif


ngx_int_t
ngx_ptocidr(ngx_str_t *text, ngx_cidr_t *cidr)
{
    u_char      *addr, *mask, *last;
    माप_प्रकार       len;
    ngx_int_t    shift;
#यदि (NGX_HAVE_INET6)
    ngx_int_t    rc;
    ngx_uint_t   s, i;
#endif

    addr = text->data;
    last = addr + text->len;

    mask = ngx_strlchr(addr, last, '/');
    len = (mask ? mask : last) - addr;

    cidr->u.in.addr = ngx_inet_addr(addr, len);

    यदि (cidr->u.in.addr != INADDR_NONE) {
        cidr->family = AF_INET;

        यदि (mask == रिक्त) {
            cidr->u.in.mask = 0xffffffff;
            फर्काऊ NGX_OK;
        }

#यदि (NGX_HAVE_INET6)
    } अन्यथा यदि (ngx_inet6_addr(addr, len, cidr->u.in6.addr.s6_addr) == NGX_OK) {
        cidr->family = AF_INET6;

        यदि (mask == रिक्त) {
            ngx_memset(cidr->u.in6.mask.s6_addr, 0xff, 16);
            फर्काऊ NGX_OK;
        }

#endif
    } अन्यथा {
        फर्काऊ NGX_ERROR;
    }

    mask++;

    shift = ngx_atoi(mask, last - mask);
    यदि (shift == NGX_ERROR) {
        फर्काऊ NGX_ERROR;
    }

    विकल्प (cidr->family) {

#यदि (NGX_HAVE_INET6)
    पक्ष AF_INET6:
        यदि (shift > 128) {
            फर्काऊ NGX_ERROR;
        }

        addr = cidr->u.in6.addr.s6_addr;
        mask = cidr->u.in6.mask.s6_addr;
        rc = NGX_OK;

        लागि (i = 0; i < 16; i++) {

            s = (shift > 8) ? 8 : shift;
            shift -= s;

            mask[i] = (u_char) (0xffu << (8 - s));

            यदि (addr[i] != (addr[i] & mask[i])) {
                rc = NGX_DONE;
                addr[i] &= mask[i];
            }
        }

        फर्काऊ rc;
#endif

    स्वभाव: /* AF_INET */
        यदि (shift > 32) {
            फर्काऊ NGX_ERROR;
        }

        यदि (shift) {
            cidr->u.in.mask = htonl((uint32_t) (0xffffffffu << (32 - shift)));

        } अन्यथा {
            /* x86 compilers use a shl instruction that shifts by modulo 32 */
            cidr->u.in.mask = 0;
        }

        यदि (cidr->u.in.addr == (cidr->u.in.addr & cidr->u.in.mask)) {
            फर्काऊ NGX_OK;
        }

        cidr->u.in.addr &= cidr->u.in.mask;

        फर्काऊ NGX_DONE;
    }
}


ngx_int_t
ngx_cidr_match(संरचना sockaddr *sa, ngx_array_t *cidrs)
{
#यदि (NGX_HAVE_INET6)
    u_char           *p;
#endif
    in_addr_t         inaddr;
    ngx_cidr_t       *cidr;
    ngx_uint_t        family, i;
#यदि (NGX_HAVE_INET6)
    ngx_uint_t        n;
    संरचना in6_addr  *inaddr6;
#endif

#यदि (NGX_SUPPRESS_WARN)
    inaddr = 0;
#यदि (NGX_HAVE_INET6)
    inaddr6 = रिक्त;
#endif
#endif

    family = sa->sa_family;

    यदि (family == AF_INET) {
        inaddr = ((संरचना sockaddr_in *) sa)->sin_addr.s_addr;
    }

#यदि (NGX_HAVE_INET6)
    अन्यथा यदि (family == AF_INET6) {
        inaddr6 = &((संरचना sockaddr_in6 *) sa)->sin6_addr;

        यदि (IN6_IS_ADDR_V4MAPPED(inaddr6)) {
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

    लागि (cidr = cidrs->elts, i = 0; i < cidrs->nelts; i++) {
        यदि (cidr[i].family != family) {
            गच्छ next;
        }

        विकल्प (family) {

#यदि (NGX_HAVE_INET6)
        पक्ष AF_INET6:
            लागि (n = 0; n < 16; n++) {
                यदि ((inaddr6->s6_addr[n] & cidr[i].u.in6.mask.s6_addr[n])
                    != cidr[i].u.in6.addr.s6_addr[n])
                {
                    गच्छ next;
                }
            }
            रोक;
#endif

#यदि (NGX_HAVE_UNIX_DOMAIN)
        पक्ष AF_UNIX:
            रोक;
#endif

        स्वभाव: /* AF_INET */
            यदि ((inaddr & cidr[i].u.in.mask) != cidr[i].u.in.addr) {
                गच्छ next;
            }
            रोक;
        }

        फर्काऊ NGX_OK;

    next:
        जारी;
    }

    फर्काऊ NGX_DECLINED;
}


ngx_int_t
ngx_parse_addr(ngx_pool_t *pool, ngx_addr_t *addr, u_char *text, माप_प्रकार len)
{
    in_addr_t             inaddr;
    ngx_uint_t            family;
    संरचना sockaddr_in   *sin;
#यदि (NGX_HAVE_INET6)
    संरचना in6_addr       inaddr6;
    संरचना sockaddr_in6  *sin6;

    /*
     * prevent MSVC8 warning:
     *    potentially uninitialized local variable 'inaddr6' used
     */
    ngx_memzero(&inaddr6, परिमाण(संरचना in6_addr));
#endif

    inaddr = ngx_inet_addr(text, len);

    यदि (inaddr != INADDR_NONE) {
        family = AF_INET;
        len = परिमाण(संरचना sockaddr_in);

#यदि (NGX_HAVE_INET6)
    } अन्यथा यदि (ngx_inet6_addr(text, len, inaddr6.s6_addr) == NGX_OK) {
        family = AF_INET6;
        len = परिमाण(संरचना sockaddr_in6);

#endif
    } अन्यथा {
        फर्काऊ NGX_DECLINED;
    }

    addr->sockaddr = ngx_pcalloc(pool, len);
    यदि (addr->sockaddr == रिक्त) {
        फर्काऊ NGX_ERROR;
    }

    addr->sockaddr->sa_family = (u_char) family;
    addr->socklen = len;

    विकल्प (family) {

#यदि (NGX_HAVE_INET6)
    पक्ष AF_INET6:
        sin6 = (संरचना sockaddr_in6 *) addr->sockaddr;
        ngx_memcpy(sin6->sin6_addr.s6_addr, inaddr6.s6_addr, 16);
        रोक;
#endif

    स्वभाव: /* AF_INET */
        sin = (संरचना sockaddr_in *) addr->sockaddr;
        sin->sin_addr.s_addr = inaddr;
        रोक;
    }

    फर्काऊ NGX_OK;
}


ngx_int_t
ngx_parse_addr_port(ngx_pool_t *pool, ngx_addr_t *addr, u_char *text,
    माप_प्रकार len)
{
    u_char     *p, *last;
    माप_प्रकार      plen;
    ngx_int_t   rc, port;

    rc = ngx_parse_addr(pool, addr, text, len);

    यदि (rc != NGX_DECLINED) {
        फर्काऊ rc;
    }

    last = text + len;

#यदि (NGX_HAVE_INET6)
    यदि (len && text[0] == '[') {

        p = ngx_strlchr(text, last, ']');

        यदि (p == last - 1) {
            फर्काऊ ngx_parse_addr(pool, addr, text + 1, len - 2);
        }

        यदि (p == रिक्त || *++p != ':') {
            फर्काऊ NGX_DECLINED;
        }

        text++;
        len -= 2;

    } अन्यथा
#endif

    {
        p = ngx_strlchr(text, last, ':');

        यदि (p == रिक्त) {
            फर्काऊ NGX_DECLINED;
        }
    }

    p++;
    plen = last - p;

    port = ngx_atoi(p, plen);

    यदि (port < 1 || port > 65535) {
        फर्काऊ NGX_DECLINED;
    }

    len -= plen + 1;

    rc = ngx_parse_addr(pool, addr, text, len);

    यदि (rc != NGX_OK) {
        फर्काऊ rc;
    }

    ngx_inet_set_port(addr->sockaddr, (in_port_t) port);

    फर्काऊ NGX_OK;
}


ngx_int_t
ngx_parse_url(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char  *p;
    माप_प्रकार   len;

    p = u->url.data;
    len = u->url.len;

    यदि (len >= 5 && ngx_strncasecmp(p, (u_char *) "unix:", 5) == 0) {
        फर्काऊ ngx_parse_unix_domain_url(pool, u);
    }

    यदि (len && p[0] == '[') {
        फर्काऊ ngx_parse_inet6_url(pool, u);
    }

    फर्काऊ ngx_parse_inet_url(pool, u);
}


स्थिर ngx_int_t
ngx_parse_unix_domain_url(ngx_pool_t *pool, ngx_url_t *u)
{
#यदि (NGX_HAVE_UNIX_DOMAIN)
    u_char              *path, *uri, *last;
    माप_प्रकार               len;
    संरचना sockaddr_un  *saun;

    len = u->url.len;
    path = u->url.data;

    path += 5;
    len -= 5;

    यदि (u->uri_part) {

        last = path + len;
        uri = ngx_strlchr(path, last, ':');

        यदि (uri) {
            len = uri - path;
            uri++;
            u->uri.len = last - uri;
            u->uri.data = uri;
        }
    }

    यदि (len == 0) {
        u->err = "no path in the unix domain socket";
        फर्काऊ NGX_ERROR;
    }

    u->host.len = len++;
    u->host.data = path;

    यदि (len > परिमाण(saun->sun_path)) {
        u->err = "too long path in the unix domain socket";
        फर्काऊ NGX_ERROR;
    }

    u->socklen = परिमाण(संरचना sockaddr_un);
    saun = (संरचना sockaddr_un *) &u->sockaddr;
    saun->sun_family = AF_UNIX;
    (शून्य) ngx_cpystrn((u_char *) saun->sun_path, path, len);

    u->addrs = ngx_pcalloc(pool, परिमाण(ngx_addr_t));
    यदि (u->addrs == रिक्त) {
        फर्काऊ NGX_ERROR;
    }

    saun = ngx_pcalloc(pool, परिमाण(संरचना sockaddr_un));
    यदि (saun == रिक्त) {
        फर्काऊ NGX_ERROR;
    }

    u->family = AF_UNIX;
    u->naddrs = 1;

    saun->sun_family = AF_UNIX;
    (शून्य) ngx_cpystrn((u_char *) saun->sun_path, path, len);

    u->addrs[0].sockaddr = (संरचना sockaddr *) saun;
    u->addrs[0].socklen = परिमाण(संरचना sockaddr_un);
    u->addrs[0].name.len = len + 4;
    u->addrs[0].name.data = u->url.data;

    फर्काऊ NGX_OK;

#अन्यथा

    u->err = "the unix domain sockets are not supported on this platform";

    फर्काऊ NGX_ERROR;

#endif
}


स्थिर ngx_int_t
ngx_parse_inet_url(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char              *host, *port, *last, *uri, *args, *dash;
    माप_प्रकार               len;
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

    यदि (args) {
        यदि (uri == रिक्त || args < uri) {
            uri = args;
        }
    }

    यदि (uri) {
        यदि (u->listen || !u->uri_part) {
            u->err = "invalid host";
            फर्काऊ NGX_ERROR;
        }

        u->uri.len = last - uri;
        u->uri.data = uri;

        last = uri;

        यदि (uri < port) {
            port = रिक्त;
        }
    }

    यदि (port) {
        port++;

        len = last - port;

        यदि (u->listen) {
            dash = ngx_strlchr(port, last, '-');

            यदि (dash) {
                dash++;

                n = ngx_atoi(dash, last - dash);

                यदि (n < 1 || n > 65535) {
                    u->err = "invalid port";
                    फर्काऊ NGX_ERROR;
                }

                u->last_port = (in_port_t) n;

                len = dash - port - 1;
            }
        }

        n = ngx_atoi(port, len);

        यदि (n < 1 || n > 65535) {
            u->err = "invalid port";
            फर्काऊ NGX_ERROR;
        }

        यदि (u->last_port && n > u->last_port) {
            u->err = "invalid port range";
            फर्काऊ NGX_ERROR;
        }

        u->port = (in_port_t) n;
        sin->sin_port = htons((in_port_t) n);

        u->port_text.len = last - port;
        u->port_text.data = port;

        last = port - 1;

    } अन्यथा {
        यदि (uri == रिक्त) {

            यदि (u->listen) {

                /* test value as port only */

                len = last - host;

                dash = ngx_strlchr(host, last, '-');

                यदि (dash) {
                    dash++;

                    n = ngx_atoi(dash, last - dash);

                    यदि (n == NGX_ERROR) {
                        गच्छ no_port;
                    }

                    यदि (n < 1 || n > 65535) {
                        u->err = "invalid port";

                    } अन्यथा {
                        u->last_port = (in_port_t) n;
                    }

                    len = dash - host - 1;
                }

                n = ngx_atoi(host, len);

                यदि (n != NGX_ERROR) {

                    यदि (u->err) {
                        फर्काऊ NGX_ERROR;
                    }

                    यदि (n < 1 || n > 65535) {
                        u->err = "invalid port";
                        फर्काऊ NGX_ERROR;
                    }

                    यदि (u->last_port && n > u->last_port) {
                        u->err = "invalid port range";
                        फर्काऊ NGX_ERROR;
                    }

                    u->port = (in_port_t) n;
                    sin->sin_port = htons((in_port_t) n);
                    sin->sin_addr.s_addr = INADDR_ANY;

                    u->port_text.len = last - host;
                    u->port_text.data = host;

                    u->wildcard = 1;

                    फर्काऊ ngx_inet_add_addr(pool, u, &u->sockaddr.sockaddr,
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

    यदि (len == 0) {
        u->err = "no host";
        फर्काऊ NGX_ERROR;
    }

    u->host.len = len;
    u->host.data = host;

    यदि (u->listen && len == 1 && *host == '*') {
        sin->sin_addr.s_addr = INADDR_ANY;
        u->wildcard = 1;
        फर्काऊ ngx_inet_add_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);
    }

    sin->sin_addr.s_addr = ngx_inet_addr(host, len);

    यदि (sin->sin_addr.s_addr != INADDR_NONE) {

        यदि (sin->sin_addr.s_addr == INADDR_ANY) {
            u->wildcard = 1;
        }

        फर्काऊ ngx_inet_add_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);
    }

    यदि (u->no_resolve) {
        फर्काऊ NGX_OK;
    }

    यदि (ngx_inet_resolve_host(pool, u) != NGX_OK) {
        फर्काऊ NGX_ERROR;
    }

    u->family = u->addrs[0].sockaddr->sa_family;
    u->socklen = u->addrs[0].socklen;
    ngx_memcpy(&u->sockaddr, u->addrs[0].sockaddr, u->addrs[0].socklen);
    u->wildcard = ngx_inet_wildcard(&u->sockaddr.sockaddr);

    फर्काऊ NGX_OK;
}


स्थिर ngx_int_t
ngx_parse_inet6_url(ngx_pool_t *pool, ngx_url_t *u)
{
#यदि (NGX_HAVE_INET6)
    u_char               *p, *host, *port, *last, *uri, *dash;
    माप_प्रकार                len;
    ngx_int_t             n;
    संरचना sockaddr_in6  *sin6;

    u->socklen = परिमाण(संरचना sockaddr_in6);
    sin6 = (संरचना sockaddr_in6 *) &u->sockaddr;
    sin6->sin6_family = AF_INET6;

    host = u->url.data + 1;

    last = u->url.data + u->url.len;

    p = ngx_strlchr(host, last, ']');

    यदि (p == रिक्त) {
        u->err = "invalid host";
        फर्काऊ NGX_ERROR;
    }

    port = p + 1;

    uri = ngx_strlchr(port, last, '/');

    यदि (uri) {
        यदि (u->listen || !u->uri_part) {
            u->err = "invalid host";
            फर्काऊ NGX_ERROR;
        }

        u->uri.len = last - uri;
        u->uri.data = uri;

        last = uri;
    }

    यदि (port < last) {
        यदि (*port != ':') {
            u->err = "invalid host";
            फर्काऊ NGX_ERROR;
        }

        port++;

        len = last - port;

        यदि (u->listen) {
            dash = ngx_strlchr(port, last, '-');

            यदि (dash) {
                dash++;

                n = ngx_atoi(dash, last - dash);

                यदि (n < 1 || n > 65535) {
                    u->err = "invalid port";
                    फर्काऊ NGX_ERROR;
                }

                u->last_port = (in_port_t) n;

                len = dash - port - 1;
            }
        }

        n = ngx_atoi(port, len);

        यदि (n < 1 || n > 65535) {
            u->err = "invalid port";
            फर्काऊ NGX_ERROR;
        }

        यदि (u->last_port && n > u->last_port) {
            u->err = "invalid port range";
            फर्काऊ NGX_ERROR;
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

    यदि (len == 0) {
        u->err = "no host";
        फर्काऊ NGX_ERROR;
    }

    u->host.len = len + 2;
    u->host.data = host - 1;

    यदि (ngx_inet6_addr(host, len, sin6->sin6_addr.s6_addr) != NGX_OK) {
        u->err = "invalid IPv6 address";
        फर्काऊ NGX_ERROR;
    }

    यदि (IN6_IS_ADDR_UNSPECIFIED(&sin6->sin6_addr)) {
        u->wildcard = 1;
    }

    u->family = AF_INET6;

    फर्काऊ ngx_inet_add_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);

#अन्यथा

    u->err = "the INET6 sockets are not supported on this platform";

    फर्काऊ NGX_ERROR;

#endif
}


#यदि (NGX_HAVE_GETADDRINFO && NGX_HAVE_INET6)

ngx_int_t
ngx_inet_resolve_host(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char           *host;
    ngx_uint_t        n;
    संरचना addrinfo   hints, *res, *rp;

    host = ngx_alloc(u->host.len + 1, pool->log);
    यदि (host == रिक्त) {
        फर्काऊ NGX_ERROR;
    }

    (शून्य) ngx_cpystrn(host, u->host.data, u->host.len + 1);

    ngx_memzero(&hints, परिमाण(संरचना addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
#ifdef AI_ADDRCONFIG
    hints.ai_flags = AI_ADDRCONFIG;
#endif

    यदि (getaddrinfo((वर्ण *) host, रिक्त, &hints, &res) != 0) {
        u->err = "host not found";
        ngx_free(host);
        फर्काऊ NGX_ERROR;
    }

    ngx_free(host);

    लागि (n = 0, rp = res; rp != रिक्त; rp = rp->ai_next) {

        विकल्प (rp->ai_family) {

        पक्ष AF_INET:
        पक्ष AF_INET6:
            रोक;

        स्वभाव:
            जारी;
        }

        n++;
    }

    यदि (n == 0) {
        u->err = "host not found";
        गच्छ failed;
    }

    /* MP: ngx_shared_palloc() */

    लागि (rp = res; rp != रिक्त; rp = rp->ai_next) {

        विकल्प (rp->ai_family) {

        पक्ष AF_INET:
        पक्ष AF_INET6:
            रोक;

        स्वभाव:
            जारी;
        }

        यदि (ngx_inet_add_addr(pool, u, rp->ai_addr, rp->ai_addrlen, n)
            != NGX_OK)
        {
            गच्छ failed;
        }
    }

    freeaddrinfo(res);
    फर्काऊ NGX_OK;

failed:

    freeaddrinfo(res);
    फर्काऊ NGX_ERROR;
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

    ngx_memzero(&sin, परिमाण(संरचना sockaddr_in));

    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = ngx_inet_addr(u->host.data, u->host.len);

    यदि (sin.sin_addr.s_addr == INADDR_NONE) {
        host = ngx_alloc(u->host.len + 1, pool->log);
        यदि (host == रिक्त) {
            फर्काऊ NGX_ERROR;
        }

        (शून्य) ngx_cpystrn(host, u->host.data, u->host.len + 1);

        h = gethostbyname((वर्ण *) host);

        ngx_free(host);

        यदि (h == रिक्त || h->h_addr_list[0] == रिक्त) {
            u->err = "host not found";
            फर्काऊ NGX_ERROR;
        }

        लागि (n = 0; h->h_addr_list[n] != रिक्त; n++) { /* void */ }

        /* MP: ngx_shared_palloc() */

        लागि (i = 0; i < n; i++) {
            sin.sin_addr.s_addr = *(in_addr_t *) (h->h_addr_list[i]);

            यदि (ngx_inet_add_addr(pool, u, (संरचना sockaddr *) &sin,
                                  परिमाण(संरचना sockaddr_in), n)
                != NGX_OK)
            {
                फर्काऊ NGX_ERROR;
            }
        }

    } अन्यथा {

        /* MP: ngx_shared_palloc() */

        यदि (ngx_inet_add_addr(pool, u, (संरचना sockaddr *) &sin,
                              परिमाण(संरचना sockaddr_in), 1)
            != NGX_OK)
        {
            फर्काऊ NGX_ERROR;
        }
    }

    फर्काऊ NGX_OK;
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

    यदि (u->addrs == रिक्त) {
        u->addrs = ngx_palloc(pool, total * nports * परिमाण(ngx_addr_t));
        यदि (u->addrs == रिक्त) {
            फर्काऊ NGX_ERROR;
        }
    }

    लागि (i = 0; i < nports; i++) {
        sa = ngx_pcalloc(pool, socklen);
        यदि (sa == रिक्त) {
            फर्काऊ NGX_ERROR;
        }

        ngx_memcpy(sa, sockaddr, socklen);

        ngx_inet_set_port(sa, u->port + i);

        विकल्प (sa->sa_family) {

#यदि (NGX_HAVE_INET6)
        पक्ष AF_INET6:
            len = NGX_INET6_ADDRSTRLEN + परिमाण("[]:65536") - 1;
            रोक;
#endif

        स्वभाव: /* AF_INET */
            len = NGX_INET_ADDRSTRLEN + परिमाण(":65535") - 1;
        }

        p = ngx_pnalloc(pool, len);
        यदि (p == रिक्त) {
            फर्काऊ NGX_ERROR;
        }

        len = ngx_sock_ntop(sa, socklen, p, len, 1);

        addr = &u->addrs[u->naddrs++];

        addr->sockaddr = sa;
        addr->socklen = socklen;

        addr->name.len = len;
        addr->name.data = p;
    }

    फर्काऊ NGX_OK;
}


ngx_int_t
ngx_cmp_sockaddr(संरचना sockaddr *sa1, socklen_t slen1,
    संरचना sockaddr *sa2, socklen_t slen2, ngx_uint_t cmp_port)
{
    संरचना sockaddr_in   *sin1, *sin2;
#यदि (NGX_HAVE_INET6)
    संरचना sockaddr_in6  *sin61, *sin62;
#endif
#यदि (NGX_HAVE_UNIX_DOMAIN)
    माप_प्रकार                len;
    संरचना sockaddr_un   *saun1, *saun2;
#endif

    यदि (sa1->sa_family != sa2->sa_family) {
        फर्काऊ NGX_DECLINED;
    }

    विकल्प (sa1->sa_family) {

#यदि (NGX_HAVE_INET6)
    पक्ष AF_INET6:

        sin61 = (संरचना sockaddr_in6 *) sa1;
        sin62 = (संरचना sockaddr_in6 *) sa2;

        यदि (cmp_port && sin61->sin6_port != sin62->sin6_port) {
            फर्काऊ NGX_DECLINED;
        }

        यदि (ngx_memcmp(&sin61->sin6_addr, &sin62->sin6_addr, 16) != 0) {
            फर्काऊ NGX_DECLINED;
        }

        रोक;
#endif

#यदि (NGX_HAVE_UNIX_DOMAIN)
    पक्ष AF_UNIX:

        saun1 = (संरचना sockaddr_un *) sa1;
        saun2 = (संरचना sockaddr_un *) sa2;

        यदि (slen1 < slen2) {
            len = slen1 - दुरत्व(संरचना sockaddr_un, sun_path);

        } अन्यथा {
            len = slen2 - दुरत्व(संरचना sockaddr_un, sun_path);
        }

        यदि (len > परिमाण(saun1->sun_path)) {
            len = परिमाण(saun1->sun_path);
        }

        यदि (ngx_memcmp(&saun1->sun_path, &saun2->sun_path, len) != 0) {
            फर्काऊ NGX_DECLINED;
        }

        रोक;
#endif

    स्वभाव: /* AF_INET */

        sin1 = (संरचना sockaddr_in *) sa1;
        sin2 = (संरचना sockaddr_in *) sa2;

        यदि (cmp_port && sin1->sin_port != sin2->sin_port) {
            फर्काऊ NGX_DECLINED;
        }

        यदि (sin1->sin_addr.s_addr != sin2->sin_addr.s_addr) {
            फर्काऊ NGX_DECLINED;
        }

        रोक;
    }

    फर्काऊ NGX_OK;
}


in_port_t
ngx_inet_get_port(संरचना sockaddr *sa)
{
    संरचना sockaddr_in   *sin;
#यदि (NGX_HAVE_INET6)
    संरचना sockaddr_in6  *sin6;
#endif

    विकल्प (sa->sa_family) {

#यदि (NGX_HAVE_INET6)
    पक्ष AF_INET6:
        sin6 = (संरचना sockaddr_in6 *) sa;
        फर्काऊ ntohs(sin6->sin6_port);
#endif

#यदि (NGX_HAVE_UNIX_DOMAIN)
    पक्ष AF_UNIX:
        फर्काऊ 0;
#endif

    स्वभाव: /* AF_INET */
        sin = (संरचना sockaddr_in *) sa;
        फर्काऊ ntohs(sin->sin_port);
    }
}


शून्य
ngx_inet_set_port(संरचना sockaddr *sa, in_port_t port)
{
    संरचना sockaddr_in   *sin;
#यदि (NGX_HAVE_INET6)
    संरचना sockaddr_in6  *sin6;
#endif

    विकल्प (sa->sa_family) {

#यदि (NGX_HAVE_INET6)
    पक्ष AF_INET6:
        sin6 = (संरचना sockaddr_in6 *) sa;
        sin6->sin6_port = htons(port);
        रोक;
#endif

#यदि (NGX_HAVE_UNIX_DOMAIN)
    पक्ष AF_UNIX:
        रोक;
#endif

    स्वभाव: /* AF_INET */
        sin = (संरचना sockaddr_in *) sa;
        sin->sin_port = htons(port);
        रोक;
    }
}


ngx_uint_t
ngx_inet_wildcard(संरचना sockaddr *sa)
{
    संरचना sockaddr_in   *sin;
#यदि (NGX_HAVE_INET6)
    संरचना sockaddr_in6  *sin6;
#endif

    विकल्प (sa->sa_family) {

    पक्ष AF_INET:
        sin = (संरचना sockaddr_in *) sa;

        यदि (sin->sin_addr.s_addr == INADDR_ANY) {
            फर्काऊ 1;
        }

        रोक;

#यदि (NGX_HAVE_INET6)

    पक्ष AF_INET6:
        sin6 = (संरचना sockaddr_in6 *) sa;

        यदि (IN6_IS_ADDR_UNSPECIFIED(&sin6->sin6_addr)) {
            फर्काऊ 1;
        }

        रोक;

#endif
    }

    फर्काऊ 0;
}
