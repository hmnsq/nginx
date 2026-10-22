
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


स्थिर ngx_int_t ngx_parse_unix_domain_url(ngx_pool_t *pool, ngx_url_t *u);
स्थिर ngx_int_t ngx_parse_inet_url(ngx_pool_t *pool, ngx_url_t *u);
स्थिर ngx_int_t ngx_parse_inet6_url(ngx_pool_t *pool, ngx_url_t *u);
स्थिर ngx_int_t ngx_inet_सङ्कलय_addr(ngx_pool_t *pool, ngx_url_t *u,
    संरचना sockaddr *sockaddr, socklen_t socklen, ngx_uint_t योग);


अन्तर्_addr_t
ngx_inet_addr(u_char *text, आकार_t len)
{
    u_char      *p, c;
    अन्तर्_addr_t    addr;
    ngx_uint_t   octet, n;

    addr = 0;
    octet = 0;
    n = 0;

    प्रति (p = text; p < text + len; p++) {
        c = *p;

        यदि (c >= '0' && c <= '9') {
            octet = octet * 10 + (c - '0');

            यदि (octet > 255) {
                प्रत्यागम INADDR_NONE;
            }

            अनुवर्त;
        }

        यदि (c == '.') {
            addr = (addr << 8) + octet;
            octet = 0;
            n++;
            अनुवर्त;
        }

        प्रत्यागम INADDR_NONE;
    }

    यदि (n == 3) {
        addr = (addr << 8) + octet;
        प्रत्यागम htonl(addr);
    }

    प्रत्यागम INADDR_NONE;
}


#यदि (NGX_HAVE_INET6)

ngx_int_t
ngx_inet6_addr(u_char *p, आकार_t len, u_char *addr)
{
    u_char      c, *zero, *digit, *s, *d;
    आकार_t      len4;
    ngx_uint_t  n, nibbles, word;

    यदि (len == 0) {
        प्रत्यागम NGX_ERROR;
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

    प्रति (/* void */; len; len--) {
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
                    अनुवर्त;
                }

            } अन्यथा {
                यदि (zero == रिक्त) {
                    digit = p;
                    len4 = len;
                    zero = addr;
                    अनुवर्त;
                }
            }

            प्रत्यागम NGX_ERROR;
        }

        यदि (c == '.' && nibbles) {
            यदि (n < 2 || digit == रिक्त) {
                प्रत्यागम NGX_ERROR;
            }

            word = ngx_inet_addr(digit, len4 - 1);
            यदि (word == INADDR_NONE) {
                प्रत्यागम NGX_ERROR;
            }

            word = ntohl(word);
            *addr++ = (u_char) ((word >> 24) & 0xff);
            *addr++ = (u_char) ((word >> 16) & 0xff);
            n--;
            भङ्ग;
        }

        यदि (++nibbles > 4) {
            प्रत्यागम NGX_ERROR;
        }

        यदि (c >= '0' && c <= '9') {
            word = word * 16 + (c - '0');
            अनुवर्त;
        }

        c |= 0x20;

        यदि (c >= 'a' && c <= 'f') {
            word = word * 16 + (c - 'a') + 10;
            अनुवर्त;
        }

        प्रत्यागम NGX_ERROR;
    }

    यदि (nibbles == 0 && zero == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

    *addr++ = (u_char) (word >> 8);
    *addr++ = (u_char) (word & 0xff);

    यदि (--n) {
        यदि (zero) {
            n *= 2;
            s = addr - 1;
            d = s + n;
            यावत् (s >= zero) {
                *d-- = *s--;
            }
            ngx_memzero(zero, n);
            प्रत्यागम NGX_OK;
        }

    } अन्यथा {
        यदि (zero == रिक्त) {
            प्रत्यागम NGX_OK;
        }
    }

    प्रत्यागम NGX_ERROR;
}

#यदि_अन्तः


आकार_t
ngx_sock_ntop(संरचना sockaddr *sa, socklen_t socklen, u_char *text, आकार_t len,
    ngx_uint_t पत्तनम्)
{
    u_char               *p;
#यदि (NGX_HAVE_INET6 || NGX_HAVE_UNIX_DOMAIN)
    आकार_t                n;
#यदि_अन्तः
    संरचना sockaddr_अन्तर्   *ज्या;
#यदि (NGX_HAVE_INET6)
    संरचना sockaddr_in6  *sin6;
#यदि_अन्तः
#यदि (NGX_HAVE_UNIX_DOMAIN)
    संरचना sockaddr_un   *saun;
#यदि_अन्तः

    विकल्प (sa->sa_family) {

    पक्ष AF_INET:

        ज्या = (संरचना sockaddr_अन्तर् *) sa;
        p = (u_char *) &ज्या->sin_addr;

        यदि (पत्तनम्) {
            p = ngx_snprintf(text, len, "%ud.%ud.%ud.%ud:%d",
                             p[0], p[1], p[2], p[3], ntohs(ज्या->sin_पत्तनम्));
        } अन्यथा {
            p = ngx_snprintf(text, len, "%ud.%ud.%ud.%ud",
                             p[0], p[1], p[2], p[3]);
        }

        प्रत्यागम (p - text);

#यदि (NGX_HAVE_INET6)

    पक्ष AF_INET6:

        sin6 = (संरचना sockaddr_in6 *) sa;

        n = 0;

        यदि (पत्तनम्) {
            text[n++] = '[';
        }

        n = ngx_inet6_ntop(sin6->sin6_addr.s6_addr, &text[n], len);

        यदि (पत्तनम्) {
            n = ngx_sprintf(&text[1 + n], "]:%d",
                            ntohs(sin6->sin6_पत्तनम्)) - text;
        }

        प्रत्यागम n;
#यदि_अन्तः

#यदि (NGX_HAVE_UNIX_DOMAIN)

    पक्ष AF_UNIX:
        saun = (संरचना sockaddr_un *) sa;

        /* on Linux sockaddr might not include sun_path at all */

        यदि (socklen <= (socklen_t) offsetof(संरचना sockaddr_un, sun_मार्गः)) {
            p = ngx_snprintf(text, len, "unix:%Z");

        } अन्यथा {
            n = ngx_strnlen((u_char *) saun->sun_मार्गः,
                            socklen - offsetof(संरचना sockaddr_un, sun_मार्गः));
            p = ngx_snprintf(text, len, "unix:%*s%Z", n, saun->sun_मार्गः);
        }

        /* we do not include trailing zero in address length */

        प्रत्यागम (p - text - 1);

#यदि_अन्तः

    स्वभाव:
        प्रत्यागम 0;
    }
}


आकार_t
ngx_inet_ntop(पूर्णाङ्क family, शून्य *addr, u_char *text, आकार_t len)
{
    u_char  *p;

    विकल्प (family) {

    पक्ष AF_INET:

        p = addr;

        प्रत्यागम ngx_snprintf(text, len, "%ud.%ud.%ud.%ud",
                            p[0], p[1], p[2], p[3])
               - text;

#यदि (NGX_HAVE_INET6)

    पक्ष AF_INET6:
        प्रत्यागम ngx_inet6_ntop(addr, text, len);

#यदि_अन्तः

    स्वभाव:
        प्रत्यागम 0;
    }
}


#यदि (NGX_HAVE_INET6)

आकार_t
ngx_inet6_ntop(u_char *p, u_char *text, आकार_t len)
{
    u_char      *dst;
    आकार_t       max, n;
    ngx_uint_t   i, zero, चरमः;

    यदि (len < NGX_INET6_ADDRSTRLEN) {
        प्रत्यागम 0;
    }

    zero = (ngx_uint_t) -1;
    चरमः = (ngx_uint_t) -1;
    max = 1;
    n = 0;

    प्रति (i = 0; i < 16; i += 2) {

        यदि (p[i] || p[i + 1]) {

            यदि (max < n) {
                zero = चरमः;
                max = n;
            }

            n = 0;
            अनुवर्त;
        }

        यदि (n++ == 0) {
            चरमः = i;
        }
    }

    यदि (max < n) {
        zero = चरमः;
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

    प्रति (i = 0; i < n; i += 2) {

        यदि (i == zero) {
            *dst++ = ':';
            i += (max - 1) * 2;
            अनुवर्त;
        }

        dst = ngx_sprintf(dst, "%xd", p[i] * 256 + p[i + 1]);

        यदि (i < 14) {
            *dst++ = ':';
        }
    }

    यदि (n == 12) {
        dst = ngx_sprintf(dst, "%ud.%ud.%ud.%ud", p[12], p[13], p[14], p[15]);
    }

    प्रत्यागम dst - text;
}

#यदि_अन्तः


ngx_int_t
ngx_ptocidr(ngx_str_t *text, ngx_cidr_t *cidr)
{
    u_char      *addr, *आवरणम्, *चरमः;
    आकार_t       len;
    ngx_int_t    सरणम्;
#यदि (NGX_HAVE_INET6)
    ngx_int_t    rc;
    ngx_uint_t   s, i;
#यदि_अन्तः

    addr = text->दत्त;
    चरमः = addr + text->len;

    आवरणम् = ngx_strlchr(addr, चरमः, '/');
    len = (आवरणम् ? आवरणम् : चरमः) - addr;

    cidr->u.अन्तर्.addr = ngx_inet_addr(addr, len);

    यदि (cidr->u.अन्तर्.addr != INADDR_NONE) {
        cidr->family = AF_INET;

        यदि (आवरणम् == रिक्त) {
            cidr->u.अन्तर्.आवरणम् = 0xffffffff;
            प्रत्यागम NGX_OK;
        }

#यदि (NGX_HAVE_INET6)
    } अन्यथा यदि (ngx_inet6_addr(addr, len, cidr->u.in6.addr.s6_addr) == NGX_OK) {
        cidr->family = AF_INET6;

        यदि (आवरणम् == रिक्त) {
            ngx_memset(cidr->u.in6.आवरणम्.s6_addr, 0xff, 16);
            प्रत्यागम NGX_OK;
        }

#यदि_अन्तः
    } अन्यथा {
        प्रत्यागम NGX_ERROR;
    }

    आवरणम्++;

    सरणम् = ngx_atoi(आवरणम्, चरमः - आवरणम्);
    यदि (सरणम् == NGX_ERROR) {
        प्रत्यागम NGX_ERROR;
    }

    विकल्प (cidr->family) {

#यदि (NGX_HAVE_INET6)
    पक्ष AF_INET6:
        यदि (सरणम् > 128) {
            प्रत्यागम NGX_ERROR;
        }

        addr = cidr->u.in6.addr.s6_addr;
        आवरणम् = cidr->u.in6.आवरणम्.s6_addr;
        rc = NGX_OK;

        प्रति (i = 0; i < 16; i++) {

            s = (सरणम् > 8) ? 8 : सरणम्;
            सरणम् -= s;

            आवरणम्[i] = (u_char) (0xffu << (8 - s));

            यदि (addr[i] != (addr[i] & आवरणम्[i])) {
                rc = NGX_समाप्तम्;
                addr[i] &= आवरणम्[i];
            }
        }

        प्रत्यागम rc;
#यदि_अन्तः

    स्वभाव: /* AF_INET */
        यदि (सरणम् > 32) {
            प्रत्यागम NGX_ERROR;
        }

        यदि (सरणम्) {
            cidr->u.अन्तर्.आवरणम् = htonl((uint32_t) (0xffffffffu << (32 - सरणम्)));

        } अन्यथा {
            /* x86 compilers use a shl instruction that shifts by modulo 32 */
            cidr->u.अन्तर्.आवरणम् = 0;
        }

        यदि (cidr->u.अन्तर्.addr == (cidr->u.अन्तर्.addr & cidr->u.अन्तर्.आवरणम्)) {
            प्रत्यागम NGX_OK;
        }

        cidr->u.अन्तर्.addr &= cidr->u.अन्तर्.आवरणम्;

        प्रत्यागम NGX_समाप्तम्;
    }
}


ngx_int_t
ngx_cidr_मेलनम्(संरचना sockaddr *sa, ngx_array_t *cidrs)
{
#यदि (NGX_HAVE_INET6)
    u_char           *p;
#यदि_अन्तः
    अन्तर्_addr_t         inaddr;
    ngx_cidr_t       *cidr;
    ngx_uint_t        family, i;
#यदि (NGX_HAVE_INET6)
    ngx_uint_t        n;
    संरचना in6_addr  *inaddr6;
#यदि_अन्तः

#यदि (NGX_SUPPRESS_WARN)
    inaddr = 0;
#यदि (NGX_HAVE_INET6)
    inaddr6 = रिक्त;
#यदि_अन्तः
#यदि_अन्तः

    family = sa->sa_family;

    यदि (family == AF_INET) {
        inaddr = ((संरचना sockaddr_अन्तर् *) sa)->sin_addr.s_addr;
    }

#यदि (NGX_HAVE_INET6)
    अन्यथा यदि (family == AF_INET6) {
        inaddr6 = &((संरचना sockaddr_in6 *) sa)->sin6_addr;

        यदि (IN6_IS_ADDR_V4MAPPED(inaddr6)) {
            family = AF_INET;

            p = inaddr6->s6_addr;

            inaddr = (अन्तर्_addr_t) p[12] << 24;
            inaddr += p[13] << 16;
            inaddr += p[14] << 8;
            inaddr += p[15];

            inaddr = htonl(inaddr);
        }
    }
#यदि_अन्तः

    प्रति (cidr = cidrs->elts, i = 0; i < cidrs->nelts; i++) {
        यदि (cidr[i].family != family) {
            गच्छ अग्र;
        }

        विकल्प (family) {

#यदि (NGX_HAVE_INET6)
        पक्ष AF_INET6:
            प्रति (n = 0; n < 16; n++) {
                यदि ((inaddr6->s6_addr[n] & cidr[i].u.in6.आवरणम्.s6_addr[n])
                    != cidr[i].u.in6.addr.s6_addr[n])
                {
                    गच्छ अग्र;
                }
            }
            भङ्ग;
#यदि_अन्तः

#यदि (NGX_HAVE_UNIX_DOMAIN)
        पक्ष AF_UNIX:
            भङ्ग;
#यदि_अन्तः

        स्वभाव: /* AF_INET */
            यदि ((inaddr & cidr[i].u.अन्तर्.आवरणम्) != cidr[i].u.अन्तर्.addr) {
                गच्छ अग्र;
            }
            भङ्ग;
        }

        प्रत्यागम NGX_OK;

    अग्र:
        अनुवर्त;
    }

    प्रत्यागम NGX_DECLINED;
}


ngx_int_t
ngx_parse_addr(ngx_pool_t *pool, ngx_addr_t *addr, u_char *text, आकार_t len)
{
    अन्तर्_addr_t             inaddr;
    ngx_uint_t            family;
    संरचना sockaddr_अन्तर्   *ज्या;
#यदि (NGX_HAVE_INET6)
    संरचना in6_addr       inaddr6;
    संरचना sockaddr_in6  *sin6;

    /*
     * prevent MSVC8 warning:
     *    potentially uninitialized local variable 'inaddr6' used
     */
    ngx_memzero(&inaddr6, परिमाण(संरचना in6_addr));
#यदि_अन्तः

    inaddr = ngx_inet_addr(text, len);

    यदि (inaddr != INADDR_NONE) {
        family = AF_INET;
        len = परिमाण(संरचना sockaddr_अन्तर्);

#यदि (NGX_HAVE_INET6)
    } अन्यथा यदि (ngx_inet6_addr(text, len, inaddr6.s6_addr) == NGX_OK) {
        family = AF_INET6;
        len = परिमाण(संरचना sockaddr_in6);

#यदि_अन्तः
    } अन्यथा {
        प्रत्यागम NGX_DECLINED;
    }

    addr->sockaddr = ngx_pcalloc(pool, len);
    यदि (addr->sockaddr == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

    addr->sockaddr->sa_family = (u_char) family;
    addr->socklen = len;

    विकल्प (family) {

#यदि (NGX_HAVE_INET6)
    पक्ष AF_INET6:
        sin6 = (संरचना sockaddr_in6 *) addr->sockaddr;
        ngx_memcpy(sin6->sin6_addr.s6_addr, inaddr6.s6_addr, 16);
        भङ्ग;
#यदि_अन्तः

    स्वभाव: /* AF_INET */
        ज्या = (संरचना sockaddr_अन्तर् *) addr->sockaddr;
        ज्या->sin_addr.s_addr = inaddr;
        भङ्ग;
    }

    प्रत्यागम NGX_OK;
}


ngx_int_t
ngx_parse_addr_पत्तनम्(ngx_pool_t *pool, ngx_addr_t *addr, u_char *text,
    आकार_t len)
{
    u_char     *p, *चरमः;
    आकार_t      plen;
    ngx_int_t   rc, पत्तनम्;

    rc = ngx_parse_addr(pool, addr, text, len);

    यदि (rc != NGX_DECLINED) {
        प्रत्यागम rc;
    }

    चरमः = text + len;

#यदि (NGX_HAVE_INET6)
    यदि (len && text[0] == '[') {

        p = ngx_strlchr(text, चरमः, ']');

        यदि (p == चरमः - 1) {
            प्रत्यागम ngx_parse_addr(pool, addr, text + 1, len - 2);
        }

        यदि (p == रिक्त || *++p != ':') {
            प्रत्यागम NGX_DECLINED;
        }

        text++;
        len -= 2;

    } अन्यथा
#यदि_अन्तः

    {
        p = ngx_strlchr(text, चरमः, ':');

        यदि (p == रिक्त) {
            प्रत्यागम NGX_DECLINED;
        }
    }

    p++;
    plen = चरमः - p;

    पत्तनम् = ngx_atoi(p, plen);

    यदि (पत्तनम् < 1 || पत्तनम् > 65535) {
        प्रत्यागम NGX_DECLINED;
    }

    len -= plen + 1;

    rc = ngx_parse_addr(pool, addr, text, len);

    यदि (rc != NGX_OK) {
        प्रत्यागम rc;
    }

    ngx_inet_समुच्चयः_पत्तनम्(addr->sockaddr, (अन्तर्_पत्तनम्_t) पत्तनम्);

    प्रत्यागम NGX_OK;
}


ngx_int_t
ngx_parse_url(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char  *p;
    आकार_t   len;

    p = u->url.दत्त;
    len = u->url.len;

    यदि (len >= 5 && ngx_strncasecmp(p, (u_char *) "unix:", 5) == 0) {
        प्रत्यागम ngx_parse_unix_domain_url(pool, u);
    }

    यदि (len && p[0] == '[') {
        प्रत्यागम ngx_parse_inet6_url(pool, u);
    }

    प्रत्यागम ngx_parse_inet_url(pool, u);
}


स्थिर ngx_int_t
ngx_parse_unix_domain_url(ngx_pool_t *pool, ngx_url_t *u)
{
#यदि (NGX_HAVE_UNIX_DOMAIN)
    u_char              *मार्गः, *uri, *चरमः;
    आकार_t               len;
    संरचना sockaddr_un  *saun;

    len = u->url.len;
    मार्गः = u->url.दत्त;

    मार्गः += 5;
    len -= 5;

    यदि (u->uri_part) {

        चरमः = मार्गः + len;
        uri = ngx_strlchr(मार्गः, चरमः, ':');

        यदि (uri) {
            len = uri - मार्गः;
            uri++;
            u->uri.len = चरमः - uri;
            u->uri.दत्त = uri;
        }
    }

    यदि (len == 0) {
        u->err = "no path in the unix domain socket";
        प्रत्यागम NGX_ERROR;
    }

    u->host.len = len++;
    u->host.दत्त = मार्गः;

    यदि (len > परिमाण(saun->sun_मार्गः)) {
        u->err = "too long path in the unix domain socket";
        प्रत्यागम NGX_ERROR;
    }

    u->socklen = परिमाण(संरचना sockaddr_un);
    saun = (संरचना sockaddr_un *) &u->sockaddr;
    saun->sun_family = AF_UNIX;
    (शून्य) ngx_cpystrn((u_char *) saun->sun_मार्गः, मार्गः, len);

    u->addrs = ngx_pcalloc(pool, परिमाण(ngx_addr_t));
    यदि (u->addrs == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

    saun = ngx_pcalloc(pool, परिमाण(संरचना sockaddr_un));
    यदि (saun == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

    u->family = AF_UNIX;
    u->naddrs = 1;

    saun->sun_family = AF_UNIX;
    (शून्य) ngx_cpystrn((u_char *) saun->sun_मार्गः, मार्गः, len);

    u->addrs[0].sockaddr = (संरचना sockaddr *) saun;
    u->addrs[0].socklen = परिमाण(संरचना sockaddr_un);
    u->addrs[0].name.len = len + 4;
    u->addrs[0].name.दत्त = u->url.दत्त;

    प्रत्यागम NGX_OK;

#अन्यथा

    u->err = "the unix domain sockets are not supported on this platform";

    प्रत्यागम NGX_ERROR;

#यदि_अन्तः
}


स्थिर ngx_int_t
ngx_parse_inet_url(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char              *host, *पत्तनम्, *चरमः, *uri, *तर्काः, *dash;
    आकार_t               len;
    ngx_int_t            n;
    संरचना sockaddr_अन्तर्  *ज्या;

    u->socklen = परिमाण(संरचना sockaddr_अन्तर्);
    ज्या = (संरचना sockaddr_अन्तर् *) &u->sockaddr;
    ज्या->sin_family = AF_INET;

    u->family = AF_INET;

    host = u->url.दत्त;

    चरमः = host + u->url.len;

    पत्तनम् = ngx_strlchr(host, चरमः, ':');

    uri = ngx_strlchr(host, चरमः, '/');

    तर्काः = ngx_strlchr(host, चरमः, '?');

    यदि (तर्काः) {
        यदि (uri == रिक्त || तर्काः < uri) {
            uri = तर्काः;
        }
    }

    यदि (uri) {
        यदि (u->listen || !u->uri_part) {
            u->err = "invalid host";
            प्रत्यागम NGX_ERROR;
        }

        u->uri.len = चरमः - uri;
        u->uri.दत्त = uri;

        चरमः = uri;

        यदि (uri < पत्तनम्) {
            पत्तनम् = रिक्त;
        }
    }

    यदि (पत्तनम्) {
        पत्तनम्++;

        len = चरमः - पत्तनम्;

        यदि (u->listen) {
            dash = ngx_strlchr(पत्तनम्, चरमः, '-');

            यदि (dash) {
                dash++;

                n = ngx_atoi(dash, चरमः - dash);

                यदि (n < 1 || n > 65535) {
                    u->err = "invalid port";
                    प्रत्यागम NGX_ERROR;
                }

                u->चरमः_पत्तनम् = (अन्तर्_पत्तनम्_t) n;

                len = dash - पत्तनम् - 1;
            }
        }

        n = ngx_atoi(पत्तनम्, len);

        यदि (n < 1 || n > 65535) {
            u->err = "invalid port";
            प्रत्यागम NGX_ERROR;
        }

        यदि (u->चरमः_पत्तनम् && n > u->चरमः_पत्तनम्) {
            u->err = "invalid port range";
            प्रत्यागम NGX_ERROR;
        }

        u->पत्तनम् = (अन्तर्_पत्तनम्_t) n;
        ज्या->sin_पत्तनम् = htons((अन्तर्_पत्तनम्_t) n);

        u->पत्तनम्_text.len = चरमः - पत्तनम्;
        u->पत्तनम्_text.दत्त = पत्तनम्;

        चरमः = पत्तनम् - 1;

    } अन्यथा {
        यदि (uri == रिक्त) {

            यदि (u->listen) {

                /* test value as port only */

                len = चरमः - host;

                dash = ngx_strlchr(host, चरमः, '-');

                यदि (dash) {
                    dash++;

                    n = ngx_atoi(dash, चरमः - dash);

                    यदि (n == NGX_ERROR) {
                        गच्छ no_पत्तनम्;
                    }

                    यदि (n < 1 || n > 65535) {
                        u->err = "invalid port";

                    } अन्यथा {
                        u->चरमः_पत्तनम् = (अन्तर्_पत्तनम्_t) n;
                    }

                    len = dash - host - 1;
                }

                n = ngx_atoi(host, len);

                यदि (n != NGX_ERROR) {

                    यदि (u->err) {
                        प्रत्यागम NGX_ERROR;
                    }

                    यदि (n < 1 || n > 65535) {
                        u->err = "invalid port";
                        प्रत्यागम NGX_ERROR;
                    }

                    यदि (u->चरमः_पत्तनम् && n > u->चरमः_पत्तनम्) {
                        u->err = "invalid port range";
                        प्रत्यागम NGX_ERROR;
                    }

                    u->पत्तनम् = (अन्तर्_पत्तनम्_t) n;
                    ज्या->sin_पत्तनम् = htons((अन्तर्_पत्तनम्_t) n);
                    ज्या->sin_addr.s_addr = INADDR_ANY;

                    u->पत्तनम्_text.len = चरमः - host;
                    u->पत्तनम्_text.दत्त = host;

                    u->wildcard = 1;

                    प्रत्यागम ngx_inet_सङ्कलय_addr(pool, u, &u->sockaddr.sockaddr,
                                             u->socklen, 1);
                }
            }
        }

no_पत्तनम्:

        u->err = रिक्त;
        u->no_पत्तनम् = 1;
        u->पत्तनम् = u->default_पत्तनम्;
        ज्या->sin_पत्तनम् = htons(u->default_पत्तनम्);
        u->चरमः_पत्तनम् = 0;
    }

    len = चरमः - host;

    यदि (len == 0) {
        u->err = "no host";
        प्रत्यागम NGX_ERROR;
    }

    u->host.len = len;
    u->host.दत्त = host;

    यदि (u->listen && len == 1 && *host == '*') {
        ज्या->sin_addr.s_addr = INADDR_ANY;
        u->wildcard = 1;
        प्रत्यागम ngx_inet_सङ्कलय_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);
    }

    ज्या->sin_addr.s_addr = ngx_inet_addr(host, len);

    यदि (ज्या->sin_addr.s_addr != INADDR_NONE) {

        यदि (ज्या->sin_addr.s_addr == INADDR_ANY) {
            u->wildcard = 1;
        }

        प्रत्यागम ngx_inet_सङ्कलय_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);
    }

    यदि (u->no_resolve) {
        प्रत्यागम NGX_OK;
    }

    यदि (ngx_inet_resolve_host(pool, u) != NGX_OK) {
        प्रत्यागम NGX_ERROR;
    }

    u->family = u->addrs[0].sockaddr->sa_family;
    u->socklen = u->addrs[0].socklen;
    ngx_memcpy(&u->sockaddr, u->addrs[0].sockaddr, u->addrs[0].socklen);
    u->wildcard = ngx_inet_wildcard(&u->sockaddr.sockaddr);

    प्रत्यागम NGX_OK;
}


स्थिर ngx_int_t
ngx_parse_inet6_url(ngx_pool_t *pool, ngx_url_t *u)
{
#यदि (NGX_HAVE_INET6)
    u_char               *p, *host, *पत्तनम्, *चरमः, *uri, *dash;
    आकार_t                len;
    ngx_int_t             n;
    संरचना sockaddr_in6  *sin6;

    u->socklen = परिमाण(संरचना sockaddr_in6);
    sin6 = (संरचना sockaddr_in6 *) &u->sockaddr;
    sin6->sin6_family = AF_INET6;

    host = u->url.दत्त + 1;

    चरमः = u->url.दत्त + u->url.len;

    p = ngx_strlchr(host, चरमः, ']');

    यदि (p == रिक्त) {
        u->err = "invalid host";
        प्रत्यागम NGX_ERROR;
    }

    पत्तनम् = p + 1;

    uri = ngx_strlchr(पत्तनम्, चरमः, '/');

    यदि (uri) {
        यदि (u->listen || !u->uri_part) {
            u->err = "invalid host";
            प्रत्यागम NGX_ERROR;
        }

        u->uri.len = चरमः - uri;
        u->uri.दत्त = uri;

        चरमः = uri;
    }

    यदि (पत्तनम् < चरमः) {
        यदि (*पत्तनम् != ':') {
            u->err = "invalid host";
            प्रत्यागम NGX_ERROR;
        }

        पत्तनम्++;

        len = चरमः - पत्तनम्;

        यदि (u->listen) {
            dash = ngx_strlchr(पत्तनम्, चरमः, '-');

            यदि (dash) {
                dash++;

                n = ngx_atoi(dash, चरमः - dash);

                यदि (n < 1 || n > 65535) {
                    u->err = "invalid port";
                    प्रत्यागम NGX_ERROR;
                }

                u->चरमः_पत्तनम् = (अन्तर्_पत्तनम्_t) n;

                len = dash - पत्तनम् - 1;
            }
        }

        n = ngx_atoi(पत्तनम्, len);

        यदि (n < 1 || n > 65535) {
            u->err = "invalid port";
            प्रत्यागम NGX_ERROR;
        }

        यदि (u->चरमः_पत्तनम् && n > u->चरमः_पत्तनम्) {
            u->err = "invalid port range";
            प्रत्यागम NGX_ERROR;
        }

        u->पत्तनम् = (अन्तर्_पत्तनम्_t) n;
        sin6->sin6_पत्तनम् = htons((अन्तर्_पत्तनम्_t) n);

        u->पत्तनम्_text.len = चरमः - पत्तनम्;
        u->पत्तनम्_text.दत्त = पत्तनम्;

    } अन्यथा {
        u->no_पत्तनम् = 1;
        u->पत्तनम् = u->default_पत्तनम्;
        sin6->sin6_पत्तनम् = htons(u->default_पत्तनम्);
    }

    len = p - host;

    यदि (len == 0) {
        u->err = "no host";
        प्रत्यागम NGX_ERROR;
    }

    u->host.len = len + 2;
    u->host.दत्त = host - 1;

    यदि (ngx_inet6_addr(host, len, sin6->sin6_addr.s6_addr) != NGX_OK) {
        u->err = "invalid IPv6 address";
        प्रत्यागम NGX_ERROR;
    }

    यदि (IN6_IS_ADDR_UNSPECIFIED(&sin6->sin6_addr)) {
        u->wildcard = 1;
    }

    u->family = AF_INET6;

    प्रत्यागम ngx_inet_सङ्कलय_addr(pool, u, &u->sockaddr.sockaddr, u->socklen, 1);

#अन्यथा

    u->err = "the INET6 sockets are not supported on this platform";

    प्रत्यागम NGX_ERROR;

#यदि_अन्तः
}


#यदि (NGX_HAVE_GETADDRINFO && NGX_HAVE_INET6)

ngx_int_t
ngx_inet_resolve_host(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char           *host;
    ngx_uint_t        n;
    संरचना addrinfo   hints, *res, *rp;

    host = ngx_alloc(u->host.len + 1, pool->लघुगणकः);
    यदि (host == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

    (शून्य) ngx_cpystrn(host, u->host.दत्त, u->host.len + 1);

    ngx_memzero(&hints, परिमाण(संरचना addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_प्रवाहः;
#यदि_परिभाषितम् AI_ADDRCONFIG
    hints.ai_flags = AI_ADDRCONFIG;
#यदि_अन्तः

    यदि (getaddrinfo((वर्ण *) host, रिक्त, &hints, &res) != 0) {
        u->err = "host not found";
        ngx_free(host);
        प्रत्यागम NGX_ERROR;
    }

    ngx_free(host);

    प्रति (n = 0, rp = res; rp != रिक्त; rp = rp->ai_अग्र) {

        विकल्प (rp->ai_family) {

        पक्ष AF_INET:
        पक्ष AF_INET6:
            भङ्ग;

        स्वभाव:
            अनुवर्त;
        }

        n++;
    }

    यदि (n == 0) {
        u->err = "host not found";
        गच्छ failed;
    }

    /* MP: ngx_shared_palloc() */

    प्रति (rp = res; rp != रिक्त; rp = rp->ai_अग्र) {

        विकल्प (rp->ai_family) {

        पक्ष AF_INET:
        पक्ष AF_INET6:
            भङ्ग;

        स्वभाव:
            अनुवर्त;
        }

        यदि (ngx_inet_सङ्कलय_addr(pool, u, rp->ai_addr, rp->ai_addrlen, n)
            != NGX_OK)
        {
            गच्छ failed;
        }
    }

    freeaddrinfo(res);
    प्रत्यागम NGX_OK;

failed:

    freeaddrinfo(res);
    प्रत्यागम NGX_ERROR;
}

#अन्यथा /* !NGX_HAVE_GETADDRINFO || !NGX_HAVE_INET6 */

ngx_int_t
ngx_inet_resolve_host(ngx_pool_t *pool, ngx_url_t *u)
{
    u_char              *host;
    ngx_uint_t           i, n;
    संरचना hostent      *h;
    संरचना sockaddr_अन्तर्   ज्या;

    /* AF_INET only */

    ngx_memzero(&ज्या, परिमाण(संरचना sockaddr_अन्तर्));

    ज्या.sin_family = AF_INET;
    ज्या.sin_addr.s_addr = ngx_inet_addr(u->host.दत्त, u->host.len);

    यदि (ज्या.sin_addr.s_addr == INADDR_NONE) {
        host = ngx_alloc(u->host.len + 1, pool->लघुगणकः);
        यदि (host == रिक्त) {
            प्रत्यागम NGX_ERROR;
        }

        (शून्य) ngx_cpystrn(host, u->host.दत्त, u->host.len + 1);

        h = gethostbyname((वर्ण *) host);

        ngx_free(host);

        यदि (h == रिक्त || h->h_addr_list[0] == रिक्त) {
            u->err = "host not found";
            प्रत्यागम NGX_ERROR;
        }

        प्रति (n = 0; h->h_addr_list[n] != रिक्त; n++) { /* void */ }

        /* MP: ngx_shared_palloc() */

        प्रति (i = 0; i < n; i++) {
            ज्या.sin_addr.s_addr = *(अन्तर्_addr_t *) (h->h_addr_list[i]);

            यदि (ngx_inet_सङ्कलय_addr(pool, u, (संरचना sockaddr *) &ज्या,
                                  परिमाण(संरचना sockaddr_अन्तर्), n)
                != NGX_OK)
            {
                प्रत्यागम NGX_ERROR;
            }
        }

    } अन्यथा {

        /* MP: ngx_shared_palloc() */

        यदि (ngx_inet_सङ्कलय_addr(pool, u, (संरचना sockaddr *) &ज्या,
                              परिमाण(संरचना sockaddr_अन्तर्), 1)
            != NGX_OK)
        {
            प्रत्यागम NGX_ERROR;
        }
    }

    प्रत्यागम NGX_OK;
}

#यदि_अन्तः /* NGX_HAVE_GETADDRINFO && NGX_HAVE_INET6 */


स्थिर ngx_int_t
ngx_inet_सङ्कलय_addr(ngx_pool_t *pool, ngx_url_t *u, संरचना sockaddr *sockaddr,
    socklen_t socklen, ngx_uint_t योग)
{
    u_char           *p;
    आकार_t            len;
    ngx_uint_t        i, nports;
    ngx_addr_t       *addr;
    संरचना sockaddr  *sa;

    nports = u->चरमः_पत्तनम् ? u->चरमः_पत्तनम् - u->पत्तनम् + 1 : 1;

    यदि (u->addrs == रिक्त) {
        u->addrs = ngx_palloc(pool, योग * nports * परिमाण(ngx_addr_t));
        यदि (u->addrs == रिक्त) {
            प्रत्यागम NGX_ERROR;
        }
    }

    प्रति (i = 0; i < nports; i++) {
        sa = ngx_pcalloc(pool, socklen);
        यदि (sa == रिक्त) {
            प्रत्यागम NGX_ERROR;
        }

        ngx_memcpy(sa, sockaddr, socklen);

        ngx_inet_समुच्चयः_पत्तनम्(sa, u->पत्तनम् + i);

        विकल्प (sa->sa_family) {

#यदि (NGX_HAVE_INET6)
        पक्ष AF_INET6:
            len = NGX_INET6_ADDRSTRLEN + परिमाण("[]:65536") - 1;
            भङ्ग;
#यदि_अन्तः

        स्वभाव: /* AF_INET */
            len = NGX_INET_ADDRSTRLEN + परिमाण(":65535") - 1;
        }

        p = ngx_pnalloc(pool, len);
        यदि (p == रिक्त) {
            प्रत्यागम NGX_ERROR;
        }

        len = ngx_sock_ntop(sa, socklen, p, len, 1);

        addr = &u->addrs[u->naddrs++];

        addr->sockaddr = sa;
        addr->socklen = socklen;

        addr->name.len = len;
        addr->name.दत्त = p;
    }

    प्रत्यागम NGX_OK;
}


ngx_int_t
ngx_cmp_sockaddr(संरचना sockaddr *sa1, socklen_t slen1,
    संरचना sockaddr *sa2, socklen_t slen2, ngx_uint_t cmp_पत्तनम्)
{
    संरचना sockaddr_अन्तर्   *sin1, *sin2;
#यदि (NGX_HAVE_INET6)
    संरचना sockaddr_in6  *sin61, *sin62;
#यदि_अन्तः
#यदि (NGX_HAVE_UNIX_DOMAIN)
    आकार_t                len;
    संरचना sockaddr_un   *saun1, *saun2;
#यदि_अन्तः

    यदि (sa1->sa_family != sa2->sa_family) {
        प्रत्यागम NGX_DECLINED;
    }

    विकल्प (sa1->sa_family) {

#यदि (NGX_HAVE_INET6)
    पक्ष AF_INET6:

        sin61 = (संरचना sockaddr_in6 *) sa1;
        sin62 = (संरचना sockaddr_in6 *) sa2;

        यदि (cmp_पत्तनम् && sin61->sin6_पत्तनम् != sin62->sin6_पत्तनम्) {
            प्रत्यागम NGX_DECLINED;
        }

        यदि (ngx_memcmp(&sin61->sin6_addr, &sin62->sin6_addr, 16) != 0) {
            प्रत्यागम NGX_DECLINED;
        }

        भङ्ग;
#यदि_अन्तः

#यदि (NGX_HAVE_UNIX_DOMAIN)
    पक्ष AF_UNIX:

        saun1 = (संरचना sockaddr_un *) sa1;
        saun2 = (संरचना sockaddr_un *) sa2;

        यदि (slen1 < slen2) {
            len = slen1 - offsetof(संरचना sockaddr_un, sun_मार्गः);

        } अन्यथा {
            len = slen2 - offsetof(संरचना sockaddr_un, sun_मार्गः);
        }

        यदि (len > परिमाण(saun1->sun_मार्गः)) {
            len = परिमाण(saun1->sun_मार्गः);
        }

        यदि (ngx_memcmp(&saun1->sun_मार्गः, &saun2->sun_मार्गः, len) != 0) {
            प्रत्यागम NGX_DECLINED;
        }

        भङ्ग;
#यदि_अन्तः

    स्वभाव: /* AF_INET */

        sin1 = (संरचना sockaddr_अन्तर् *) sa1;
        sin2 = (संरचना sockaddr_अन्तर् *) sa2;

        यदि (cmp_पत्तनम् && sin1->sin_पत्तनम् != sin2->sin_पत्तनम्) {
            प्रत्यागम NGX_DECLINED;
        }

        यदि (sin1->sin_addr.s_addr != sin2->sin_addr.s_addr) {
            प्रत्यागम NGX_DECLINED;
        }

        भङ्ग;
    }

    प्रत्यागम NGX_OK;
}


अन्तर्_पत्तनम्_t
ngx_inet_get_पत्तनम्(संरचना sockaddr *sa)
{
    संरचना sockaddr_अन्तर्   *ज्या;
#यदि (NGX_HAVE_INET6)
    संरचना sockaddr_in6  *sin6;
#यदि_अन्तः

    विकल्प (sa->sa_family) {

#यदि (NGX_HAVE_INET6)
    पक्ष AF_INET6:
        sin6 = (संरचना sockaddr_in6 *) sa;
        प्रत्यागम ntohs(sin6->sin6_पत्तनम्);
#यदि_अन्तः

#यदि (NGX_HAVE_UNIX_DOMAIN)
    पक्ष AF_UNIX:
        प्रत्यागम 0;
#यदि_अन्तः

    स्वभाव: /* AF_INET */
        ज्या = (संरचना sockaddr_अन्तर् *) sa;
        प्रत्यागम ntohs(ज्या->sin_पत्तनम्);
    }
}


शून्य
ngx_inet_समुच्चयः_पत्तनम्(संरचना sockaddr *sa, अन्तर्_पत्तनम्_t पत्तनम्)
{
    संरचना sockaddr_अन्तर्   *ज्या;
#यदि (NGX_HAVE_INET6)
    संरचना sockaddr_in6  *sin6;
#यदि_अन्तः

    विकल्प (sa->sa_family) {

#यदि (NGX_HAVE_INET6)
    पक्ष AF_INET6:
        sin6 = (संरचना sockaddr_in6 *) sa;
        sin6->sin6_पत्तनम् = htons(पत्तनम्);
        भङ्ग;
#यदि_अन्तः

#यदि (NGX_HAVE_UNIX_DOMAIN)
    पक्ष AF_UNIX:
        भङ्ग;
#यदि_अन्तः

    स्वभाव: /* AF_INET */
        ज्या = (संरचना sockaddr_अन्तर् *) sa;
        ज्या->sin_पत्तनम् = htons(पत्तनम्);
        भङ्ग;
    }
}


ngx_uint_t
ngx_inet_wildcard(संरचना sockaddr *sa)
{
    संरचना sockaddr_अन्तर्   *ज्या;
#यदि (NGX_HAVE_INET6)
    संरचना sockaddr_in6  *sin6;
#यदि_अन्तः

    विकल्प (sa->sa_family) {

    पक्ष AF_INET:
        ज्या = (संरचना sockaddr_अन्तर् *) sa;

        यदि (ज्या->sin_addr.s_addr == INADDR_ANY) {
            प्रत्यागम 1;
        }

        भङ्ग;

#यदि (NGX_HAVE_INET6)

    पक्ष AF_INET6:
        sin6 = (संरचना sockaddr_in6 *) sa;

        यदि (IN6_IS_ADDR_UNSPECIFIED(&sin6->sin6_addr)) {
            प्रत्यागम 1;
        }

        भङ्ग;

#यदि_अन्तः
    }

    प्रत्यागम 0;
}
