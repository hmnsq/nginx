
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#समावेश <ngx_config.h>
#समावेश <ngx_core.h>


अटल ngx_int_t ngx_parse_unix_domain_url(ngx_pool_t *pool, ngx_url_t *u);
अटल ngx_int_t ngx_parse_inet_url(ngx_pool_t *pool, ngx_url_t *u);
अटल ngx_int_t ngx_parse_inet6_url(ngx_pool_t *pool, ngx_url_t *u);
अटल ngx_int_t ngx_inet_add_addr(ngx_pool_t *pool, ngx_url_t *u,
    काष्ठा sockaddr *sockaddr, socklen_t socklen, ngx_uint_t योग);


in_addr_t
ngx_inet_addr(u_char *text, माप_प्रकार len)
{
    u_char      *p, c;
    in_addr_t    addr;
    ngx_uint_t   octet, n;

    addr = 0;
    octet = 0;
    n = 0;

    क्रम (p = text; p < text + len; p++) {
        c = *p;

        अगर (c >= '0' && c <= '9') {
            octet = octet * 10 + (c - '0');

            अगर (octet > 255) {
                वापस INADDR_NONE;
            }

            जारी;
        }

        अगर (c == '.') {
            addr = (addr << 8) + octet;
            octet = 0;
            n++;
            जारी;
        }

        वापस INADDR_NONE;
    }

    अगर (n == 3) {
        addr = (addr << 8) + octet;
        वापस htonl(addr);
    }

    वापस INADDR_NONE;
}


#अगर (NGX_HAVE_INET6)

ngx_int_t
ngx_inet6_addr(u_char *p, माप_प्रकार len, u_char *addr)
{
    u_char      c, *zero, *digit, *s, *d;
    माप_प्रकार      len4;
    ngx_uint_t  n, nibbles, word;

    अगर (len == 0) {
        वापस NGX_ERROR;
    }

    zero = शून्य;
    digit = शून्य;
    len4 = 0;
    nibbles = 0;
    word = 0;
    n = 8;

    अगर (p[0] == ':') {
        p++;
        len--;
    }

    क्रम (/* void */; len; len--) {
        c = *p++;

        अगर (c == ':') {
            अगर (nibbles) {
                digit = p;
                len4 = len;
                *addr++ = (u_char) (word >> 8);
                *addr++ = (u_char) (word & 0xff);

                अगर (--n) {
                    nibbles = 0;
                    word = 0;
                    जारी;
                }

            } अन्यथा {
                अगर (zero == शून्य) {
                    digit = p;
                    len4 = len;
                    zero = addr;
                    जारी;
                }
            }

            वापस NGX_ERROR;
        }

        अगर (c == '.' && nibbles) {
            अगर (n < 2 || digit == शून्य) {
                वापस NGX_ERROR;
            }

            word = ngx_inet_addr(digit, len4 - 1);
            अगर (word == INADDR_NONE) {
                वापस NGX_ERROR;
            }

            word = ntohl(word);
            *addr++ = (u_char) ((word >> 24) & 0xff);
            *addr++ = (u_char) ((word >> 16) & 0xff);
            n--;
            अवरोध;
        }

        अगर (++nibbles > 4) {
            वापस NGX_ERROR;
        }

        अगर (c >= '0' && c <= '9') {
            word = word * 16 + (c - '0');
            जारी;
        }

        c |= 0x20;

        अगर (c >= 'a' && c <= 'f') {
            word = word * 16 + (c - 'a') + 10;
            जारी;
        }

        वापस NGX_ERROR;
    }

    अगर (nibbles == 0 && zero == शून्य) {
        वापस NGX_ERROR;
    }

    *addr++ = (u_char) (word >> 8);
    *addr++ = (u_char) (word & 0xff);

    अगर (--n) {
        अगर (zero) {
            n *= 2;
            s = addr - 1;
            d = s + n;
            जबतक (s >= zero) {
                *d-- = *s--;
            }
            ngx_memzero(zero, n);
            वापस NGX_OK;
        }

    } अन्यथा {
        अगर (zero == शून्य) {
            वापस NGX_OK;
        }
    }

    वापस NGX_ERROR;
}

#पूर्ण_अगर


माप_प्रकार
ngx_sock_ntop(काष्ठा sockaddr *sa, socklen_t socklen, u_char *text, माप_प्रकार len,
    ngx_uint_t port)
{
    u_char               *p;
#अगर (NGX_HAVE_INET6 || NGX_HAVE_UNIX_DOMAIN)
    माप_प्रकार                n;
#पूर्ण_अगर
    काष्ठा sockaddr_in   *sin;
#अगर (NGX_HAVE_INET6)
    काष्ठा sockaddr_in6  *sin6;
#पूर्ण_अगर
#अगर (NGX_HAVE_UNIX_DOMAIN)
    काष्ठा sockaddr_un   *saun;
#पूर्ण_अगर

    चयन (sa->sa_family) {

    हाल AF_INET:

        sin = (काष्ठा sockaddr_in *) sa;
        p = (u_char *) &sin->sin_addr;

        अगर (port) {
            p = ngx_snprintf(text, len, "%ud.%ud.%ud.%ud:%d",
                             p[0], p[1], p[2], p[3], ntohs(sin->sin_port));
        } अन्यथा {
            p = ngx_snprintf(text, len, "%ud.%ud.%ud.%ud",
                             p[0], p[1], p[2], p[3]);
        }

        वापस (p - text);

#अगर (NGX_HAVE_INET6)

    हाल AF_INET6:

        sin6 = (काष्ठा sockaddr_in6 *) sa;

        n = 0;

        अगर (port) {
            text[n++] = '[';
        }

        n = ngx_inet6_ntop(sin6->sin6_addr.s6_addr, &text[n], len);

        अगर (port) {
            n = ngx_sprintf(&text[1 + n], "]:%d",
                            ntohs(sin6->sin6_port)) - text;
        }

        वापस n;
#पूर्ण_अगर

#अगर (NGX_HAVE_UNIX_DOMAIN)

    हाल AF_UNIX:
        saun = (काष्ठा sockaddr_un *) sa;

        /* on Linux sockaddr might not include sun_path at all */

        अगर (socklen <= (socklen_t) दुरत्व(काष्ठा sockaddr_un, sun_path)) {
            p = ngx_snprintf(text, len, "unix:%Z");

        } अन्यथा {
            n = ngx_strnlen((u_char *) saun->sun_path,
                            socklen - दुरत्व(काष्ठा sockaddr_un, sun_path));
            p = ngx_snprintf(text, len, "unix:%*s%Z", n, saun->sun_path);
        }

        /* we do not include trailing zero in address length */

        वापस (p - text - 1);

#पूर्ण_अगर

    शेष:
        वापस 0;
    }
}


माप_प्रकार
ngx_inet_ntop(पूर्णांक family, व्योम *addr, u_char *text, माप_प्रकार len)
{
    u_char  *p;

    चयन (family) {

    हाल AF_INET:

        p = addr;

        वापस ngx_snprintf(text, len, "%ud.%ud.%ud.%ud",
                            p[0], p[1], p[2], p[3])
               - text;

#अगर (NGX_HAVE_INET6)

    हाल AF_INET6:
        वापस ngx_inet6_ntop(addr, text, len);

#पूर्ण_अगर

    शेष:
        वापस 0;
    }
}


#अगर (NGX_HAVE_INET6)

माप_प्रकार
ngx_inet6_ntop(u_char *p, u_char *text, माप_प्रकार len)
{
    u_char      *dst;
    माप_प्रकार       max, n;
    ngx_uint_t   i, zero, last;

    अगर (len < NGX_INET6_ADDRSTRLEN) {
        वापस 0;
    }

    zero = (ngx_uint_t) -1;
    last = (ngx_uint_t) -1;
    max = 1;
    n = 0;

    क्रम (i = 0; i < 16; i += 2) {

        अगर (p[i] || p[i + 1]) {

            अगर (max < n) {
                zero = last;
                max = n;
            }

            n = 0;
            जारी;
        }

        अगर (n++ == 0) {
            last = i;
        }
    }

    अगर (max < n) {
        zero = last;
        max = n;
    }

    dst = text;
    n = 16;

    अगर (zero == 0) {

        अगर ((max == 5 && p[10] == 0xff && p[11] == 0xff)
            || (max == 6)
            || (max == 7 && p[14] != 0 && p[15] != 1))
        {
            n = 12;
        }

        *dst++ = ':';
    }

    क्रम (i = 0; i < n; i += 2) {

        अगर (i == zero) {
            *dst++ = ':';
            i += (max - 1) * 2;
            जारी;
        }

        dst = ngx_sprintf(dst, "%xd", p[i] * 256 + p[i + 1]);

        अगर (i < 14) {
            *dst++ = ':';
        }
    }

    अगर (n == 12) {
        dst = ngx_sprintf(dst, "%ud.%ud.%ud.%ud", p[12], p[13], p[14], p[15]);
    }

    वापस dst - text;
}

#पूर्ण_अगर


ngx_int_t
ngx_ptocidr(ngx_str_t *text, ngx_cidr_t *cidr)
{
    u_char      *addr, *mask, *last;
    माप_प्रकार       len;
    ngx_int_t    shift;
#अगर (NGX_HAVE_INET6)
    ngx_int_t    rc;
    ngx_uint_t   s, i;
#पूर्ण_अगर

    addr = text->तथ्य;
    last = addr + text->len;

    mask = ngx_strlchr(addr, last, '/');
    len = (mask ? mask : last) - addr;

    cidr->u.in.addr = ngx_inet_addr(addr, len);

    अगर (cidr->u.in.addr != INADDR_NONE) {
        cidr->family = AF_INET;

        अगर (mask == शून्य) {
            cidr->u.in.mask = 0xffffffff;
            वापस NGX_OK;
        }

#अगर (NGX_HAVE_INET6)
    } अन्यथा अगर (ngx_inet6_addr(addr, len, cidr->u.in6.addr.s6_addr) == NGX_OK) {
        cidr->family = AF_INET6;

        अगर (mask == शून्य) {
            ngx_memset(cidr->u.in6.mask.s6_addr, 0xff, 16);
            वापस NGX_OK;
        }

#पूर्ण_अगर
    } अन्यथा {
        वापस NGX_ERROR;
    }

    mask++;

    shift = ngx_atoi(mask, last - mask);
    अगर (shift == NGX_ERROR) {
        वापस NGX_ERROR;
    }

    चयन (cidr->family) {

#अगर (NGX_HAVE_INET6)
    हाल AF_INET6:
        अगर (shift > 128) {
            वापस NGX_ERROR;
        }

        addr = cidr->u.in6.addr.s6_addr;
        mask = cidr->u.in6.mask.s6_addr;
        rc = NGX_OK;

        क्रम (i = 0; i < 16; i++) {

            s = (shift > 8) ? 8 : shift;
            shift -= s;

            mask[i] = (u_char) (0xffu << (8 - s));

            अगर (addr[i] != (addr[i] & mask[i])) {
                rc = NGX_DONE;
                addr[i] &= mask[i];
            }
        }

        वापस rc;
#पूर्ण_अगर

    शेष: /* AF_INET */
        अगर (shift > 32) {
            वापस NGX_ERROR;
        }

        अगर (shift) {
            cidr->u.in.mask = htonl((uint32_t) (0xffffffffu << (32 - shift)));

        } अन्यथा {
            /* x86 compilers use a shl instruction that shifts by modulo 32 */
            cidr->u.in.mask = 0;
        }

        अगर (cidr->u.in.addr == (cidr->u.in.addr & cidr->u.in.mask)) {
            वापस NGX_OK;
        }

        cidr->u.in.addr &= cidr->u.in.mask;

        वापस NGX_DONE;
    }
}


ngx_int_t
ngx_cidr_match(काष्ठा sockaddr *sa, ngx_array_t *cidrs)
{
#अगर (NGX_HAVE_INET6)
    u_char           *p;
#पूर्ण_अगर
    in_addr_t         inaddr;
    ngx_cidr_t       *cidr;
    ngx_uint_t        family, i;
#अगर (NGX_HAVE_INET6)
    ngx_uint_t        n;
    काष्ठा in6_addr  *inaddr6;
#पूर्ण_अगर

#अगर (NGX_SUPPRESS_WARN)
    inaddr = 0;
#अगर (NGX_HAVE_INET6)
    inaddr6 = शून्य;
#पूर्ण_अगर
#पूर्ण_अगर

    family = sa->sa_family;

    अगर (family == AF_INET) {
        inaddr = ((काष्ठा sockaddr_in *) sa)->sin_addr.s_addr;
    }

#अगर (NGX_HAVE_INET6)
    अन्यथा अगर (family == AF_INET6) {
        inaddr6 = &((काष्ठा sockaddr_in6 *) sa)->sin6_addr;

        अगर (IN6_IS_ADDR_V4MAPPED(inaddr6)) {
            family = AF_INET;

            p = inaddr6->s6_addr;

            inaddr = (in_addr_t) p[12] << 24;
            inaddr += p[13] << 16;
            inaddr += p[14] << 8;
            inaddr += p[15];

            inaddr = htonl(inaddr);
        }
    }
#पूर्ण_अगर

    क्रम (cidr = cidrs->elts, i = 0; i < cidrs->nelts; i++) {
        अगर (cidr[i].family != family) {
            जाओ अग्र;
        }

        चयन (family) {

#अगर (NGX_HAVE_INET6)
        हाल AF_INET6:
            क्रम (n = 0; n < 16; n++) {
                अगर ((inaddr6->s6_addr[n] & cidr[i].u.in6.mask.s6_addr[n])
                    != cidr[i].u.in6.addr.s6_addr[n])
                {
                    जाओ अग्र;
                }
            }
            अवरोध;
#पूर्ण_अगर

#अगर (NGX_HAVE_UNIX_DOMAIN)
        हाल AF_UNIX:
            अवरोध;
#पूर्ण_अगर

        शेष: /* AF_INET */
            अगर ((inaddr & cidr[i].u.in.mask) != cidr[i].u.in.addr) {
                जाओ अग्र;
            }
            अवरोध;
        }

        वापस NGX_OK;

    अग्र:
        जारी;
    }

    वापस NGX_DECLINED;
}


ngx_int_t
ngx_parse_addr(ngx_pool_t *pool, ngx_addr_t *addr, u_char *text, माप_प्रकार len)
{
    in_addr_t             inaddr;
    ngx_uint_t            family;
    काष्ठा sockaddr_in   *sin;
#अगर (NGX_HAVE_INET6)
    काष्ठा in6_addr       inaddr6;
    काष्ठा sockaddr_in6  *sin6;

    /*
     * prevent MSVC8 warning:
     *    potentially uninitialized local variable 'inaddr6' used
     */
    ngx_memzero(&inaddr6, माप(काष्ठा in6_addr));
#पूर्ण_अगर

    inaddr = ngx_inet_addr(text, len);

    अगर (inaddr != INADDR_NONE) {
        family = AF_INET;
        len = माप(काष्ठा sockaddr_in);

#अगर (NGX_HAVE_INET6)
    } अन्यथा अगर (ngx_inet6_addr(text, len, inaddr6.s6_addr) == NGX_OK) {
        family = AF_INET6;
        len = माप(काष्ठा sockaddr_in6);

#पूर्ण_अगर
    } अन्यथा {
        वापस NGX_DECLINED;
    }

    addr->sockaddr = ngx_pcalloc(pool, len);
    अगर (addr->sockaddr == शून्य) {
        वापस NGX_ERROR;
    }

    addr->sockaddr->sa_family = (u_char) family;
    addr->socklen = len;

    चयन (family) {

#अगर (NGX_HAVE_INET6)
    हाल AF_INET6:
        sin6 = (काष्ठा sockaddr_in6 *) addr->sockaddr;
        ngx_memcpy(sin6->sin6_addr.s6_addr, inaddr6.s6_addr, 16);
        अवरोध;
#पूर्ण_अगर

    शेष: /* AF_INET */
        sin = (काष्ठा sockaddr_in *) addr->sockaddr;
        sin->sin_addr.s_addr = inaddr;
        अवरोध;
    }

    वापस NGX_OK;
}


ngx_int_t
ngx_parse_addr_port(ngx_pool_t *pool, ngx_addr_t *addr, u_char *text,
    माप_प्रकार len)
{
    u_char     *p, *last;
    माप_प्रकार      plen;
    ngx_int_t   rc, port;

    rc = ngx_parse_addr(pool, addr, text, len);

    अगर (rc != NGX_DECLINED) {
        वापस rc;
    }

    last = text + len;

#अगर (NGX_HAVE_INET6)
    अगर (len && text[0] == '[') {

        p = ngx_strlchr(text, last, ']');

        अगर (p == last - 1) {
            वापस ngx_parse_addr(pool, addr, text + 1, len - 2);
        }

        अगर (p == शून्य || *++p != ':') {
            वापस NGX_DECLINED;
        }

        text++;
        len -= 2;

    } अन्यथा
#पूर्ण_अगर

    {
        p = ngx_strlchr(text, last, ':');

        अगर (p == शून्य) {
            वापस NGX_DECLINED;
        }
    }

    p++;
    plen = last - p;

    port = ngx_atoi(p, plen);

    अगर (port < 1 || port > 65535) {
        वापस NGX_DECLINED;
    }

    len -= plen + 1;

    rc = ngx_parse_addr(pool, addr, text, len);

    अगर (rc != NGX_OK) {
        वापस rc;
    }

    ngx_inet_set_port(addr->sockaddr, (in_port_t) port);

    वापस NGX_OK;
}


ngx_int_t
ngx_parse_url(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char  *p;
    माप_प्रकार   len;

    p = u->url.तथ्य;
    len = u->url.len;

    अगर (len >= 5 && ngx_strncasecmp(p, (u_char *) "unix:", 5) == 0) {
        वापस ngx_parse_unix_domain_url(pool, u);
    }

    अगर (len && p[0] == '[') {
        वापस ngx_parse_inet6_url(pool, u);
    }

    वापस ngx_parse_inet_url(pool, u);
}


अटल ngx_int_t
ngx_parse_unix_domain_url(ngx_pool_t *pool, ngx_url_t *u)
{
#अगर (NGX_HAVE_UNIX_DOMAIN)
    u_char              *path, *uri, *last;
    माप_प्रकार               len;
    काष्ठा sockaddr_un  *saun;

    len = u->url.len;
    path = u->url.तथ्य;

    path += 5;
    len -= 5;

    अगर (u->uri_part) {

        last = path + len;
        uri = ngx_strlchr(path, last, ':');

        अगर (uri) {
            len = uri - path;
            uri++;
            u->uri.len = last - uri;
            u->uri.तथ्य = uri;
        }
    }

    अगर (len == 0) {
        u->err = "no path in the unix domain socket";
        वापस NGX_ERROR;
    }

    u->host.len = len++;
    u->host.तथ्य = path;

    अगर (len > माप(saun->sun_path)) {
        u->err = "too long path in the unix domain socket";
        वापस NGX_ERROR;
    }

    u->socklen = माप(काष्ठा sockaddr_un);
    saun = (काष्ठा sockaddr_un *) &u->sockaddr;
    saun->sun_family = AF_UNIX;
    (व्योम) ngx_cpystrn((u_char *) saun->sun_path, path, len);

    u->addrs = ngx_pcalloc(pool, माप(ngx_addr_t));
    अगर (u->addrs == शून्य) {
        वापस NGX_ERROR;
    }

    saun = ngx_pcalloc(pool, माप(काष्ठा sockaddr_un));
    अगर (saun == शून्य) {
        वापस NGX_ERROR;
    }

    u->family = AF_UNIX;
    u->naddrs = 1;

    saun->sun_family = AF_UNIX;
    (व्योम) ngx_cpystrn((u_char *) saun->sun_path, path, len);

    u->addrs[0].sockaddr = (काष्ठा sockaddr *) saun;
    u->addrs[0].socklen = माप(काष्ठा sockaddr_un);
    u->addrs[0].name.len = len + 4;
    u->addrs[0].name.तथ्य = u->url.तथ्य;

    वापस NGX_OK;

#अन्यथा

    u->err = "the unix domain sockets are not supported on this platform";

    वापस NGX_ERROR;

#पूर्ण_अगर
}


अटल ngx_int_t
ngx_parse_inet_url(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char              *host, *port, *last, *uri, *args, *dash;
    माप_प्रकार               len;
    ngx_int_t            n;
    काष्ठा sockaddr_in  *sin;

    u->socklen = माप(काष्ठा sockaddr_in);
    sin = (काष्ठा sockaddr_in *) &u->sockaddr;
    sin->sin_family = AF_INET;

    u->family = AF_INET;

    host = u->url.तथ्य;

    last = host + u->url.len;

    port = ngx_strlchr(host, last, ':');

    uri = ngx_strlchr(host, last, '/');

    args = ngx_strlchr(host, last, '?');

    अगर (args) {
        अगर (uri == शून्य || args < uri) {
            uri = args;
        }
    }

    अगर (uri) {
        अगर (u->listen || !u->uri_part) {
            u->err = "invalid host";
            वापस NGX_ERROR;
        }

        u->uri.len = last - uri;
        u->uri.तथ्य = uri;

        last = uri;

        अगर (uri < port) {
            port = शून्य;
        }
    }

    अगर (port) {
        port++;

        len = last - port;

        अगर (u->listen) {
            dash = ngx_strlchr(port, last, '-');

            अगर (dash) {
                dash++;

                n = ngx_atoi(dash, last - dash);

                अगर (n < 1 || n > 65535) {
                    u->err = "invalid port";
                    वापस NGX_ERROR;
                }

                u->last_port = (in_port_t) n;

                len = dash - port - 1;
            }
        }

        n = ngx_atoi(port, len);

        अगर (n < 1 || n > 65535) {
            u->err = "invalid port";
            वापस NGX_ERROR;
        }

        अगर (u->last_port && n > u->last_port) {
            u->err = "invalid port range";
            वापस NGX_ERROR;
        }

        u->port = (in_port_t) n;
        sin->sin_port = htons((in_port_t) n);

        u->port_text.len = last - port;
        u->port_text.तथ्य = port;

        last = port - 1;

    } अन्यथा {
        अगर (uri == शून्य) {

            अगर (u->listen) {

                /* test value as port only */

                len = last - host;

                dash = ngx_strlchr(host, last, '-');

                अगर (dash) {
                    dash++;

                    n = ngx_atoi(dash, last - dash);

                    अगर (n == NGX_ERROR) {
                        जाओ no_port;
                    }

                    अगर (n < 1 || n > 65535) {
                        u->err = "invalid port";

                    } अन्यथा {
                        u->last_port = (in_port_t) n;
                    }

                    len = dash - host - 1;
                }

                n = ngx_atoi(host, len);

                अगर (n != NGX_ERROR) {

                    अगर (u->err) {
                        वापस NGX_ERROR;
                    }

                    अगर (n < 1 || n > 65535) {
                        u->err = "invalid port";
                        वापस NGX_ERROR;
                    }

                    अगर (u->last_port && n > u->last_port) {
                        u->err = "invalid port range";
                        वापस NGX_ERROR;
                    }

                    u->port = (in_port_t) n;
                    sin->sin_port = htons((in_port_t) n);
                    sin->sin_addr.s_addr = INADDR_ANY;

                    u->port_text.len = last - host;
                    u->port_text.तथ्य = host;

                    u->wildcard = 1;

                    वापस ngx_inet_add_addr(pool, u, &u->sockaddr.sockaddr,
                                             u->socklen, 1);
                }
            }
        }

no_port:

        u->err = शून्य;
        u->no_port = 1;
        u->port = u->default_port;
        sin->sin_port = htons(u->default_port);
        u->last_port = 0;
    }

    len = last - host;

    अगर (len == 0) {
        u->err = "no host";
        वापस NGX_ERROR;
    }

    u->host.len = len;
    u->host.तथ्य = host;

    अगर (u->listen && len == 1 && *host == '*') {
        sin->sin_addr.s_addr = INADDR_ANY;
        u->wildcard = 1;
        वापस ngx_inet_add_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);
    }

    sin->sin_addr.s_addr = ngx_inet_addr(host, len);

    अगर (sin->sin_addr.s_addr != INADDR_NONE) {

        अगर (sin->sin_addr.s_addr == INADDR_ANY) {
            u->wildcard = 1;
        }

        वापस ngx_inet_add_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);
    }

    अगर (u->no_resolve) {
        वापस NGX_OK;
    }

    अगर (ngx_inet_resolve_host(pool, u) != NGX_OK) {
        वापस NGX_ERROR;
    }

    u->family = u->addrs[0].sockaddr->sa_family;
    u->socklen = u->addrs[0].socklen;
    ngx_memcpy(&u->sockaddr, u->addrs[0].sockaddr, u->addrs[0].socklen);
    u->wildcard = ngx_inet_wildcard(&u->sockaddr.sockaddr);

    वापस NGX_OK;
}


अटल ngx_int_t
ngx_parse_inet6_url(ngx_pool_t *pool, ngx_url_t *u)
{
#अगर (NGX_HAVE_INET6)
    u_char               *p, *host, *port, *last, *uri, *dash;
    माप_प्रकार                len;
    ngx_int_t             n;
    काष्ठा sockaddr_in6  *sin6;

    u->socklen = माप(काष्ठा sockaddr_in6);
    sin6 = (काष्ठा sockaddr_in6 *) &u->sockaddr;
    sin6->sin6_family = AF_INET6;

    host = u->url.तथ्य + 1;

    last = u->url.तथ्य + u->url.len;

    p = ngx_strlchr(host, last, ']');

    अगर (p == शून्य) {
        u->err = "invalid host";
        वापस NGX_ERROR;
    }

    port = p + 1;

    uri = ngx_strlchr(port, last, '/');

    अगर (uri) {
        अगर (u->listen || !u->uri_part) {
            u->err = "invalid host";
            वापस NGX_ERROR;
        }

        u->uri.len = last - uri;
        u->uri.तथ्य = uri;

        last = uri;
    }

    अगर (port < last) {
        अगर (*port != ':') {
            u->err = "invalid host";
            वापस NGX_ERROR;
        }

        port++;

        len = last - port;

        अगर (u->listen) {
            dash = ngx_strlchr(port, last, '-');

            अगर (dash) {
                dash++;

                n = ngx_atoi(dash, last - dash);

                अगर (n < 1 || n > 65535) {
                    u->err = "invalid port";
                    वापस NGX_ERROR;
                }

                u->last_port = (in_port_t) n;

                len = dash - port - 1;
            }
        }

        n = ngx_atoi(port, len);

        अगर (n < 1 || n > 65535) {
            u->err = "invalid port";
            वापस NGX_ERROR;
        }

        अगर (u->last_port && n > u->last_port) {
            u->err = "invalid port range";
            वापस NGX_ERROR;
        }

        u->port = (in_port_t) n;
        sin6->sin6_port = htons((in_port_t) n);

        u->port_text.len = last - port;
        u->port_text.तथ्य = port;

    } अन्यथा {
        u->no_port = 1;
        u->port = u->default_port;
        sin6->sin6_port = htons(u->default_port);
    }

    len = p - host;

    अगर (len == 0) {
        u->err = "no host";
        वापस NGX_ERROR;
    }

    u->host.len = len + 2;
    u->host.तथ्य = host - 1;

    अगर (ngx_inet6_addr(host, len, sin6->sin6_addr.s6_addr) != NGX_OK) {
        u->err = "invalid IPv6 address";
        वापस NGX_ERROR;
    }

    अगर (IN6_IS_ADDR_UNSPECIFIED(&sin6->sin6_addr)) {
        u->wildcard = 1;
    }

    u->family = AF_INET6;

    वापस ngx_inet_add_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);

#अन्यथा

    u->err = "the INET6 sockets are not supported on this platform";

    वापस NGX_ERROR;

#पूर्ण_अगर
}


#अगर (NGX_HAVE_GETADDRINFO && NGX_HAVE_INET6)

ngx_int_t
ngx_inet_resolve_host(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char           *host;
    ngx_uint_t        n;
    काष्ठा addrinfo   hints, *res, *rp;

    host = ngx_alloc(u->host.len + 1, pool->log);
    अगर (host == शून्य) {
        वापस NGX_ERROR;
    }

    (व्योम) ngx_cpystrn(host, u->host.तथ्य, u->host.len + 1);

    ngx_memzero(&hints, माप(काष्ठा addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
#अगर_घोषित AI_ADDRCONFIG
    hints.ai_flags = AI_ADDRCONFIG;
#पूर्ण_अगर

    अगर (getaddrinfo((अक्षर *) host, शून्य, &hints, &res) != 0) {
        u->err = "host not found";
        ngx_free(host);
        वापस NGX_ERROR;
    }

    ngx_free(host);

    क्रम (n = 0, rp = res; rp != शून्य; rp = rp->ai_अग्र) {

        चयन (rp->ai_family) {

        हाल AF_INET:
        हाल AF_INET6:
            अवरोध;

        शेष:
            जारी;
        }

        n++;
    }

    अगर (n == 0) {
        u->err = "host not found";
        जाओ failed;
    }

    /* MP: ngx_shared_palloc() */

    क्रम (rp = res; rp != शून्य; rp = rp->ai_अग्र) {

        चयन (rp->ai_family) {

        हाल AF_INET:
        हाल AF_INET6:
            अवरोध;

        शेष:
            जारी;
        }

        अगर (ngx_inet_add_addr(pool, u, rp->ai_addr, rp->ai_addrlen, n)
            != NGX_OK)
        {
            जाओ failed;
        }
    }

    freeaddrinfo(res);
    वापस NGX_OK;

failed:

    freeaddrinfo(res);
    वापस NGX_ERROR;
}

#अन्यथा /* !NGX_HAVE_GETADDRINFO || !NGX_HAVE_INET6 */

ngx_int_t
ngx_inet_resolve_host(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char              *host;
    ngx_uint_t           i, n;
    काष्ठा hostent      *h;
    काष्ठा sockaddr_in   sin;

    /* AF_INET only */

    ngx_memzero(&sin, माप(काष्ठा sockaddr_in));

    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = ngx_inet_addr(u->host.तथ्य, u->host.len);

    अगर (sin.sin_addr.s_addr == INADDR_NONE) {
        host = ngx_alloc(u->host.len + 1, pool->log);
        अगर (host == शून्य) {
            वापस NGX_ERROR;
        }

        (व्योम) ngx_cpystrn(host, u->host.तथ्य, u->host.len + 1);

        h = gethostbyname((अक्षर *) host);

        ngx_free(host);

        अगर (h == शून्य || h->h_addr_list[0] == शून्य) {
            u->err = "host not found";
            वापस NGX_ERROR;
        }

        क्रम (n = 0; h->h_addr_list[n] != शून्य; n++) { /* void */ }

        /* MP: ngx_shared_palloc() */

        क्रम (i = 0; i < n; i++) {
            sin.sin_addr.s_addr = *(in_addr_t *) (h->h_addr_list[i]);

            अगर (ngx_inet_add_addr(pool, u, (काष्ठा sockaddr *) &sin,
                                  माप(काष्ठा sockaddr_in), n)
                != NGX_OK)
            {
                वापस NGX_ERROR;
            }
        }

    } अन्यथा {

        /* MP: ngx_shared_palloc() */

        अगर (ngx_inet_add_addr(pool, u, (काष्ठा sockaddr *) &sin,
                              माप(काष्ठा sockaddr_in), 1)
            != NGX_OK)
        {
            वापस NGX_ERROR;
        }
    }

    वापस NGX_OK;
}

#पूर्ण_अगर /* NGX_HAVE_GETADDRINFO && NGX_HAVE_INET6 */


अटल ngx_int_t
ngx_inet_add_addr(ngx_pool_t *pool, ngx_url_t *u, काष्ठा sockaddr *sockaddr,
    socklen_t socklen, ngx_uint_t योग)
{
    u_char           *p;
    माप_प्रकार            len;
    ngx_uint_t        i, nports;
    ngx_addr_t       *addr;
    काष्ठा sockaddr  *sa;

    nports = u->last_port ? u->last_port - u->port + 1 : 1;

    अगर (u->addrs == शून्य) {
        u->addrs = ngx_palloc(pool, योग * nports * माप(ngx_addr_t));
        अगर (u->addrs == शून्य) {
            वापस NGX_ERROR;
        }
    }

    क्रम (i = 0; i < nports; i++) {
        sa = ngx_pcalloc(pool, socklen);
        अगर (sa == शून्य) {
            वापस NGX_ERROR;
        }

        ngx_memcpy(sa, sockaddr, socklen);

        ngx_inet_set_port(sa, u->port + i);

        चयन (sa->sa_family) {

#अगर (NGX_HAVE_INET6)
        हाल AF_INET6:
            len = NGX_INET6_ADDRSTRLEN + माप("[]:65536") - 1;
            अवरोध;
#पूर्ण_अगर

        शेष: /* AF_INET */
            len = NGX_INET_ADDRSTRLEN + माप(":65535") - 1;
        }

        p = ngx_pnalloc(pool, len);
        अगर (p == शून्य) {
            वापस NGX_ERROR;
        }

        len = ngx_sock_ntop(sa, socklen, p, len, 1);

        addr = &u->addrs[u->naddrs++];

        addr->sockaddr = sa;
        addr->socklen = socklen;

        addr->name.len = len;
        addr->name.तथ्य = p;
    }

    वापस NGX_OK;
}


ngx_int_t
ngx_cmp_sockaddr(काष्ठा sockaddr *sa1, socklen_t slen1,
    काष्ठा sockaddr *sa2, socklen_t slen2, ngx_uint_t cmp_port)
{
    काष्ठा sockaddr_in   *sin1, *sin2;
#अगर (NGX_HAVE_INET6)
    काष्ठा sockaddr_in6  *sin61, *sin62;
#पूर्ण_अगर
#अगर (NGX_HAVE_UNIX_DOMAIN)
    माप_प्रकार                len;
    काष्ठा sockaddr_un   *saun1, *saun2;
#पूर्ण_अगर

    अगर (sa1->sa_family != sa2->sa_family) {
        वापस NGX_DECLINED;
    }

    चयन (sa1->sa_family) {

#अगर (NGX_HAVE_INET6)
    हाल AF_INET6:

        sin61 = (काष्ठा sockaddr_in6 *) sa1;
        sin62 = (काष्ठा sockaddr_in6 *) sa2;

        अगर (cmp_port && sin61->sin6_port != sin62->sin6_port) {
            वापस NGX_DECLINED;
        }

        अगर (ngx_memcmp(&sin61->sin6_addr, &sin62->sin6_addr, 16) != 0) {
            वापस NGX_DECLINED;
        }

        अवरोध;
#पूर्ण_अगर

#अगर (NGX_HAVE_UNIX_DOMAIN)
    हाल AF_UNIX:

        saun1 = (काष्ठा sockaddr_un *) sa1;
        saun2 = (काष्ठा sockaddr_un *) sa2;

        अगर (slen1 < slen2) {
            len = slen1 - दुरत्व(काष्ठा sockaddr_un, sun_path);

        } अन्यथा {
            len = slen2 - दुरत्व(काष्ठा sockaddr_un, sun_path);
        }

        अगर (len > माप(saun1->sun_path)) {
            len = माप(saun1->sun_path);
        }

        अगर (ngx_memcmp(&saun1->sun_path, &saun2->sun_path, len) != 0) {
            वापस NGX_DECLINED;
        }

        अवरोध;
#पूर्ण_अगर

    शेष: /* AF_INET */

        sin1 = (काष्ठा sockaddr_in *) sa1;
        sin2 = (काष्ठा sockaddr_in *) sa2;

        अगर (cmp_port && sin1->sin_port != sin2->sin_port) {
            वापस NGX_DECLINED;
        }

        अगर (sin1->sin_addr.s_addr != sin2->sin_addr.s_addr) {
            वापस NGX_DECLINED;
        }

        अवरोध;
    }

    वापस NGX_OK;
}


in_port_t
ngx_inet_get_port(काष्ठा sockaddr *sa)
{
    काष्ठा sockaddr_in   *sin;
#अगर (NGX_HAVE_INET6)
    काष्ठा sockaddr_in6  *sin6;
#पूर्ण_अगर

    चयन (sa->sa_family) {

#अगर (NGX_HAVE_INET6)
    हाल AF_INET6:
        sin6 = (काष्ठा sockaddr_in6 *) sa;
        वापस ntohs(sin6->sin6_port);
#पूर्ण_अगर

#अगर (NGX_HAVE_UNIX_DOMAIN)
    हाल AF_UNIX:
        वापस 0;
#पूर्ण_अगर

    शेष: /* AF_INET */
        sin = (काष्ठा sockaddr_in *) sa;
        वापस ntohs(sin->sin_port);
    }
}


व्योम
ngx_inet_set_port(काष्ठा sockaddr *sa, in_port_t port)
{
    काष्ठा sockaddr_in   *sin;
#अगर (NGX_HAVE_INET6)
    काष्ठा sockaddr_in6  *sin6;
#पूर्ण_अगर

    चयन (sa->sa_family) {

#अगर (NGX_HAVE_INET6)
    हाल AF_INET6:
        sin6 = (काष्ठा sockaddr_in6 *) sa;
        sin6->sin6_port = htons(port);
        अवरोध;
#पूर्ण_अगर

#अगर (NGX_HAVE_UNIX_DOMAIN)
    हाल AF_UNIX:
        अवरोध;
#पूर्ण_अगर

    शेष: /* AF_INET */
        sin = (काष्ठा sockaddr_in *) sa;
        sin->sin_port = htons(port);
        अवरोध;
    }
}


ngx_uint_t
ngx_inet_wildcard(काष्ठा sockaddr *sa)
{
    काष्ठा sockaddr_in   *sin;
#अगर (NGX_HAVE_INET6)
    काष्ठा sockaddr_in6  *sin6;
#पूर्ण_अगर

    चयन (sa->sa_family) {

    हाल AF_INET:
        sin = (काष्ठा sockaddr_in *) sa;

        अगर (sin->sin_addr.s_addr == INADDR_ANY) {
            वापस 1;
        }

        अवरोध;

#अगर (NGX_HAVE_INET6)

    हाल AF_INET6:
        sin6 = (काष्ठा sockaddr_in6 *) sa;

        अगर (IN6_IS_ADDR_UNSPECIFIED(&sin6->sin6_addr)) {
            वापस 1;
        }

        अवरोध;

#पूर्ण_अगर
    }

    वापस 0;
}
