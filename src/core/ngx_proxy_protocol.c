
/*
 * Copyright (C) Roman Arutyunyan
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


#تعریف_کرو NGX_PROXY_PROTOCOL_V2_MAX_سرخی    52

#تعریف_کرو NGX_PROXY_PROTOCOL_CMD_LOCAL        0
#تعریف_کرو NGX_PROXY_PROTOCOL_CMD_PROXY        1

#تعریف_کرو NGX_PROXY_PROTOCOL_AF_UNSPEC        0
#تعریف_کرو NGX_PROXY_PROTOCOL_AF_INET          1
#تعریف_کرو NGX_PROXY_PROTOCOL_AF_INET6         2

#تعریف_کرو NGX_PROXY_PROTOCOL_TYPE_UNSPEC      0
#تعریف_کرو NGX_PROXY_PROTOCOL_TYPE_دھارا      1
#تعریف_کرو NGX_PROXY_PROTOCOL_TYPE_DGRAM       2

#تعریف_کرو NGX_PROXY_PROTOCOL_TLV_ALPN         0x01
#تعریف_کرو NGX_PROXY_PROTOCOL_TLV_AUTHORITY    0x02
#تعریف_کرو NGX_PROXY_PROTOCOL_TLV_چکری_جانچ_32_C       0x03
#تعریف_کرو NGX_PROXY_PROTOCOL_TLV_UNIQUE_ID    0x05
#تعریف_کرو NGX_PROXY_PROTOCOL_TLV_SSL          0x20
#تعریف_کرو NGX_PROXY_PROTOCOL_TLV_SSL_نسخہ  0x21
#تعریف_کرو NGX_PROXY_PROTOCOL_TLV_SSL_CN       0x22
#تعریف_کرو NGX_PROXY_PROTOCOL_TLV_SSL_CIPHER   0x23
#تعریف_کرو NGX_PROXY_PROTOCOL_TLV_SSL_SIG_ALG  0x24
#تعریف_کرو NGX_PROXY_PROTOCOL_TLV_SSL_کلید_ALG  0x25
#تعریف_کرو NGX_PROXY_PROTOCOL_TLV_NETNS        0x30

#تعریف_کرو NGX_PROXY_PROTOCOL_V2_CLIENT_SSL          0x01
#تعریف_کرو NGX_PROXY_PROTOCOL_V2_CLIENT_CERT_CONN    0x02
#تعریف_کرو NGX_PROXY_PROTOCOL_V2_CLIENT_CERT_SESS    0x04


#تعریف_کرو ngx_proxy_protocol_parse_uint16(p)                                    \
    ( ((uint16_t) (p)[0] << 8)                                                \
    + (           (p)[1]) )

#تعریف_کرو ngx_proxy_protocol_parse_uint32(p)                                    \
    ( ((uint32_t) (p)[0] << 24)                                               \
    + (           (p)[1] << 16)                                               \
    + (           (p)[2] << 8)                                                \
    + (           (p)[3]) )


تعريفنوع بنية {
    u_char                                  signature[12];
    u_char                                  نسخہ_command;
    u_char                                  family_transport;
    u_char                                  len[2];
} ngx_proxy_protocol_سرخی_t;


تعريفنوع بنية {
    u_char                                  src_addr[4];
    u_char                                  dst_addr[4];
    u_char                                  src_پورٹ[2];
    u_char                                  dst_پورٹ[2];
} ngx_proxy_protocol_inet_addrs_t;


تعريفنوع بنية {
    u_char                                  src_addr[16];
    u_char                                  dst_addr[16];
    u_char                                  src_پورٹ[2];
    u_char                                  dst_پورٹ[2];
} ngx_proxy_protocol_inet6_addrs_t;


تعريفنوع بنية {
    u_char                                  type;
    u_char                                  len[2];
} ngx_proxy_protocol_tlv_t;


تعريفنوع بنية {
    u_char                                  client;
    u_char                                  verify[4];
} ngx_proxy_protocol_tlv_ssl_t;


تعريفنوع بنية {
    ngx_str_t                               name;
    ngx_uint_t                              type;
} ngx_proxy_protocol_tlv_اندراج_t;


تعريفنوع بنية {
    ngx_uint_t                              type;
    ngx_str_t                               قدر;
} ngx_proxy_protocol_tlv_قدر_t;


ساكن u_char *ngx_proxy_protocol_read_addr(ngx_connection_t *c, u_char *p,
    u_char *آخری, ngx_str_t *addr);
ساكن u_char *ngx_proxy_protocol_read_پورٹ(u_char *p, u_char *آخری,
    وچ_پورٹ_t *پورٹ, u_char sep);
ساكن u_char *ngx_proxy_protocol_v2_read(ngx_connection_t *c, u_char *buf,
    u_char *آخری);
ساكن ngx_int_t ngx_proxy_protocol_lookup_tlv(ngx_connection_t *c,
    ngx_str_t *tlvs, ngx_uint_t type, ngx_str_t *قدر);
#إذا (NGX_OPENSSL)
ساكن ngx_int_t ngx_proxy_protocol_v2_eval_ssl(ngx_connection_t *c,
    ngx_array_t *tlvs, ngx_array_t *ssl_tlvs, ngx_uint_t *client,
    uint32_t *verify);
ساكن ngx_int_t ngx_proxy_protocol_v2_authority(ngx_connection_t *c,
    ngx_str_t *باہرلا);
ساكن ngx_int_t ngx_proxy_protocol_v2_alpn(ngx_connection_t *c,
    ngx_str_t *باہرلا);
ساكن ngx_int_t ngx_proxy_protocol_v2_ssl_sub(ngx_connection_t *c,
    ngx_uint_t type, ngx_str_t *باہرلا);
#جے_انت
ساكن u_char *ngx_proxy_protocol_v2_write_سرخی(ngx_connection_t *c,
    u_char *buf, u_char *آخری);
ساكن u_char ngx_proxy_protocol_v2_family(ngx_uint_t family);
ساكن فراغ ngx_proxy_protocol_v2_write_ipv4(بنية sockaddr *sa, u_char *addr,
    u_char *پورٹ);
ساكن فراغ ngx_proxy_protocol_v2_write_ipv6(بنية sockaddr *sa, u_char *addr,
    u_char *پورٹ);
ساكن u_char *ngx_proxy_protocol_v2_write_tlv(ngx_connection_t *c, u_char *p,
    u_char *آخری, ngx_uint_t type, ngx_str_t *قدر);
ساكن u_char *ngx_proxy_protocol_v2_write_ssl(ngx_connection_t *c, u_char *p,
    u_char *آخری, ngx_array_t *ssl_tlvs, ngx_uint_t client, uint32_t verify);
ساكن فراغ ngx_proxy_protocol_v2_سیٹ_len(u_char *buf, u_char *p);
ساكن u_char *ngx_proxy_protocol_v2_write_crc32c(ngx_connection_t *c,
    u_char *buf, u_char *p, u_char *آخری);


ساكن ngx_proxy_protocol_tlv_اندراج_t  ngx_proxy_protocol_tlv_entries[] = {
    { ngx_string("alpn"),       NGX_PROXY_PROTOCOL_TLV_ALPN },
    { ngx_string("authority"),  NGX_PROXY_PROTOCOL_TLV_AUTHORITY },
    { ngx_string("unique_id"),  NGX_PROXY_PROTOCOL_TLV_UNIQUE_ID },
    { ngx_string("ssl"),        NGX_PROXY_PROTOCOL_TLV_SSL },
    { ngx_string("netns"),      NGX_PROXY_PROTOCOL_TLV_NETNS },
    { ngx_null_string,          0x00 }
};


ساكن ngx_proxy_protocol_tlv_اندراج_t  ngx_proxy_protocol_tlv_ssl_entries[] = {
    { ngx_string("version"),    NGX_PROXY_PROTOCOL_TLV_SSL_نسخہ },
    { ngx_string("cn"),         NGX_PROXY_PROTOCOL_TLV_SSL_CN },
    { ngx_string("cipher"),     NGX_PROXY_PROTOCOL_TLV_SSL_CIPHER },
    { ngx_string("sig_alg"),    NGX_PROXY_PROTOCOL_TLV_SSL_SIG_ALG },
    { ngx_string("key_alg"),    NGX_PROXY_PROTOCOL_TLV_SSL_کلید_ALG },
    { ngx_null_string,          0x00 }
};


ساكن ثابت u_char  ngx_proxy_protocol_signature[] = "\r\n\r\n\0\r\nQUIT\n";


u_char *
ngx_proxy_protocol_read(ngx_connection_t *c, u_char *buf, u_char *آخری)
{
    ناپ_t                 len;
    u_char                *p;
    ngx_proxy_protocol_t  *pp;

    p = buf;
    len = آخری - buf;

    إذا (len >= حجم(ngx_proxy_protocol_سرخی_t)
        && ngx_memcmp(p, ngx_proxy_protocol_signature,
                      حجم(ngx_proxy_protocol_signature) - 1)
           == 0)
    {
        أرجع ngx_proxy_protocol_v2_read(c, buf, آخری);
    }

    إذا (len < 8 || ngx_strncmp(p, "PROXY ", 6) != 0) {
        اذهب invalid;
    }

    p += 6;
    len -= 6;

    إذا (len >= 7 && ngx_strncmp(p, "UNKNOWN", 7) == 0) {
        ngx_log_debug0(NGX_LOG_DEBUG_CORE, c->لوگارتھم, 0,
                       "PROXY protocol unknown protocol");
        p += 7;
        اذهب skip;
    }

    إذا (len < 5 || ngx_strncmp(p, "TCP", 3) != 0
        || (p[3] != '4' && p[3] != '6') || p[4] != ' ')
    {
        اذهب invalid;
    }

    p += 5;

    pp = ngx_pcalloc(c->pool, حجم(ngx_proxy_protocol_t));
    إذا (pp == عدم) {
        أرجع عدم;
    }

    p = ngx_proxy_protocol_read_addr(c, p, آخری, &pp->src_addr);
    إذا (p == عدم) {
        اذهب invalid;
    }

    p = ngx_proxy_protocol_read_addr(c, p, آخری, &pp->dst_addr);
    إذا (p == عدم) {
        اذهب invalid;
    }

    p = ngx_proxy_protocol_read_پورٹ(p, آخری, &pp->src_پورٹ, ' ');
    إذا (p == عدم) {
        اذهب invalid;
    }

    p = ngx_proxy_protocol_read_پورٹ(p, آخری, &pp->dst_پورٹ, CR);
    إذا (p == عدم) {
        اذهب invalid;
    }

    إذا (p == آخری) {
        اذهب invalid;
    }

    إذا (*p++ != LF) {
        اذهب invalid;
    }

    ngx_log_debug4(NGX_LOG_DEBUG_CORE, c->لوگارتھم, 0,
                   "PROXY protocol src: %V %d, dst: %V %d",
                   &pp->src_addr, pp->src_پورٹ, &pp->dst_addr, pp->dst_پورٹ);

    c->proxy_protocol = pp;

    أرجع p;

skip:

    لكل ( /* void */ ; p < آخری - 1; p++) {
        إذا (p[0] == CR && p[1] == LF) {
            أرجع p + 2;
        }
    }

invalid:

    لكل (p = buf; p < آخری; p++) {
        إذا (*p == CR || *p == LF) {
            اكسر;
        }
    }

    ngx_log_error(NGX_LOG_ERR, c->لوگارتھم, 0,
                  "broken header: \"%*s\"", (ناپ_t) (p - buf), buf);

    أرجع عدم;
}


ساكن u_char *
ngx_proxy_protocol_read_addr(ngx_connection_t *c, u_char *p, u_char *آخری,
    ngx_str_t *addr)
{
    ناپ_t  len;
    u_char  ch, *pos;

    pos = p;

    لكل ( ;; ) {
        إذا (p == آخری) {
            أرجع عدم;
        }

        ch = *p++;

        إذا (ch == ' ') {
            اكسر;
        }

        إذا (ch != ':' && ch != '.'
            && (ch < 'a' || ch > 'f')
            && (ch < 'A' || ch > 'F')
            && (ch < '0' || ch > '9'))
        {
            أرجع عدم;
        }
    }

    len = p - pos - 1;

    addr->انکڑے = ngx_pnalloc(c->pool, len);
    إذا (addr->انکڑے == عدم) {
        أرجع عدم;
    }

    ngx_memcpy(addr->انکڑے, pos, len);
    addr->len = len;

    أرجع p;
}


ساكن u_char *
ngx_proxy_protocol_read_پورٹ(u_char *p, u_char *آخری, وچ_پورٹ_t *پورٹ,
    u_char sep)
{
    ناپ_t      len;
    u_char     *pos;
    ngx_int_t   n;

    pos = p;

    لكل ( ;; ) {
        إذا (p == آخری) {
            أرجع عدم;
        }

        إذا (*p++ == sep) {
            اكسر;
        }
    }

    len = p - pos - 1;

    n = ngx_atoi(pos, len);
    إذا (n < 0 || n > 65535) {
        أرجع عدم;
    }

    *پورٹ = (وچ_پورٹ_t) n;

    أرجع p;
}


u_char *
ngx_proxy_protocol_write(ngx_connection_t *c, u_char *buf, u_char *آخری)
{
    ngx_uint_t  پورٹ, lport;

    إذا (آخری - buf < NGX_PROXY_PROTOCOL_V1_MAX_سرخی) {
        ngx_log_error(NGX_LOG_ALERT, c->لوگارتھم, 0,
                      "too small buffer for PROXY protocol");
        أرجع عدم;
    }

    إذا (ngx_connection_local_sockaddr(c, عدم, 0) != NGX_OK) {
        أرجع عدم;
    }

    بدل (c->sockaddr->sa_family) {

    حالة AF_INET:
        buf = ngx_cpymem(buf, "PROXY TCP4 ", حجم("PROXY TCP4 ") - 1);
        اكسر;

#إذا (NGX_HAVE_INET6)
    حالة AF_INET6:
        buf = ngx_cpymem(buf, "PROXY TCP6 ", حجم("PROXY TCP6 ") - 1);
        اكسر;
#جے_انت

    افتراضي:
        أرجع ngx_cpymem(buf, "PROXY UNKNOWN" CRLF,
                          حجم("PROXY UNKNOWN" CRLF) - 1);
    }

    buf += ngx_sock_ntop(c->sockaddr, c->socklen, buf, آخری - buf, 0);

    *buf++ = ' ';

    buf += ngx_sock_ntop(c->local_sockaddr, c->local_socklen, buf, آخری - buf,
                         0);

    پورٹ = ngx_inet_get_پورٹ(c->sockaddr);
    lport = ngx_inet_get_پورٹ(c->local_sockaddr);

    أرجع ngx_slprintf(buf, آخری, " %ui %ui" CRLF, پورٹ, lport);
}


ساكن u_char *
ngx_proxy_protocol_v2_read(ngx_connection_t *c, u_char *buf, u_char *آخری)
{
    u_char                             *سرا;
    ناپ_t                              len;
    socklen_t                           socklen;
    ngx_uint_t                          نسخہ, command, family, transport;
    ngx_sockaddr_t                      src_sockaddr, dst_sockaddr;
    ngx_proxy_protocol_t               *pp;
    ngx_proxy_protocol_سرخی_t        *سرخی;
    ngx_proxy_protocol_inet_addrs_t    *وچ;
#إذا (NGX_HAVE_INET6)
    ngx_proxy_protocol_inet6_addrs_t   *in6;
#جے_انت

    سرخی = (ngx_proxy_protocol_سرخی_t *) buf;

    buf += حجم(ngx_proxy_protocol_سرخی_t);

    نسخہ = سرخی->نسخہ_command >> 4;

    إذا (نسخہ != 2) {
        ngx_log_error(NGX_LOG_ERR, c->لوگارتھم, 0,
                      "unknown PROXY protocol version: %ui", نسخہ);
        أرجع عدم;
    }

    len = ngx_proxy_protocol_parse_uint16(سرخی->len);

    إذا ((ناپ_t) (آخری - buf) < len) {
        ngx_log_error(NGX_LOG_ERR, c->لوگارتھم, 0, "header is too large");
        أرجع عدم;
    }

    سرا = buf + len;

    command = سرخی->نسخہ_command & 0x0f;

    /* only PROXY is supported */
    إذا (command != NGX_PROXY_PROTOCOL_CMD_PROXY) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->لوگارتھم, 0,
                       "PROXY protocol v2 unsupported command %ui", command);
        أرجع سرا;
    }

    transport = سرخی->family_transport & 0x0f;

    /* only STREAM is supported */
    إذا (transport != NGX_PROXY_PROTOCOL_TYPE_دھارا) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->لوگارتھم, 0,
                       "PROXY protocol v2 unsupported transport %ui",
                       transport);
        أرجع سرا;
    }

    pp = ngx_pcalloc(c->pool, حجم(ngx_proxy_protocol_t));
    إذا (pp == عدم) {
        أرجع عدم;
    }

    family = سرخی->family_transport >> 4;

    بدل (family) {

    حالة NGX_PROXY_PROTOCOL_AF_INET:

        إذا ((ناپ_t) (سرا - buf) < حجم(ngx_proxy_protocol_inet_addrs_t)) {
            أرجع عدم;
        }

        وچ = (ngx_proxy_protocol_inet_addrs_t *) buf;

        src_sockaddr.sockaddr_وچ.sin_family = AF_INET;
        src_sockaddr.sockaddr_وچ.sin_پورٹ = 0;
        ngx_memcpy(&src_sockaddr.sockaddr_وچ.sin_addr, وچ->src_addr, 4);

        dst_sockaddr.sockaddr_وچ.sin_family = AF_INET;
        dst_sockaddr.sockaddr_وچ.sin_پورٹ = 0;
        ngx_memcpy(&dst_sockaddr.sockaddr_وچ.sin_addr, وچ->dst_addr, 4);

        pp->src_پورٹ = ngx_proxy_protocol_parse_uint16(وچ->src_پورٹ);
        pp->dst_پورٹ = ngx_proxy_protocol_parse_uint16(وچ->dst_پورٹ);

        socklen = حجم(بنية sockaddr_وچ);

        buf += حجم(ngx_proxy_protocol_inet_addrs_t);

        اكسر;

#إذا (NGX_HAVE_INET6)

    حالة NGX_PROXY_PROTOCOL_AF_INET6:

        إذا ((ناپ_t) (سرا - buf) < حجم(ngx_proxy_protocol_inet6_addrs_t)) {
            أرجع عدم;
        }

        in6 = (ngx_proxy_protocol_inet6_addrs_t *) buf;

        src_sockaddr.sockaddr_in6.sin6_family = AF_INET6;
        src_sockaddr.sockaddr_in6.sin6_پورٹ = 0;
        ngx_memcpy(&src_sockaddr.sockaddr_in6.sin6_addr, in6->src_addr, 16);

        dst_sockaddr.sockaddr_in6.sin6_family = AF_INET6;
        dst_sockaddr.sockaddr_in6.sin6_پورٹ = 0;
        ngx_memcpy(&dst_sockaddr.sockaddr_in6.sin6_addr, in6->dst_addr, 16);

        pp->src_پورٹ = ngx_proxy_protocol_parse_uint16(in6->src_پورٹ);
        pp->dst_پورٹ = ngx_proxy_protocol_parse_uint16(in6->dst_پورٹ);

        socklen = حجم(بنية sockaddr_in6);

        buf += حجم(ngx_proxy_protocol_inet6_addrs_t);

        اكسر;

#جے_انت

    افتراضي:
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->لوگارتھم, 0,
                       "PROXY protocol v2 unsupported address family %ui",
                       family);
        أرجع سرا;
    }

    pp->src_addr.انکڑے = ngx_pnalloc(c->pool, NGX_SOCKADDR_STRLEN);
    إذا (pp->src_addr.انکڑے == عدم) {
        أرجع عدم;
    }

    pp->src_addr.len = ngx_sock_ntop(&src_sockaddr.sockaddr, socklen,
                                     pp->src_addr.انکڑے, NGX_SOCKADDR_STRLEN, 0);

    pp->dst_addr.انکڑے = ngx_pnalloc(c->pool, NGX_SOCKADDR_STRLEN);
    إذا (pp->dst_addr.انکڑے == عدم) {
        أرجع عدم;
    }

    pp->dst_addr.len = ngx_sock_ntop(&dst_sockaddr.sockaddr, socklen,
                                     pp->dst_addr.انکڑے, NGX_SOCKADDR_STRLEN, 0);

    ngx_log_debug4(NGX_LOG_DEBUG_CORE, c->لوگارتھم, 0,
                   "PROXY protocol v2 src: %V %d, dst: %V %d",
                   &pp->src_addr, pp->src_پورٹ, &pp->dst_addr, pp->dst_پورٹ);

    إذا (buf < سرا) {
        pp->tlvs.انکڑے = ngx_pnalloc(c->pool, سرا - buf);
        إذا (pp->tlvs.انکڑے == عدم) {
            أرجع عدم;
        }

        ngx_memcpy(pp->tlvs.انکڑے, buf, سرا - buf);
        pp->tlvs.len = سرا - buf;
    }

    c->proxy_protocol = pp;

    أرجع سرا;
}


ngx_int_t
ngx_proxy_protocol_get_tlv(ngx_connection_t *c, ngx_str_t *name,
    ngx_str_t *قدر)
{
    u_char                          *p;
    ناپ_t                           n;
    uint32_t                         verify;
    ngx_str_t                        ssl, *tlvs;
    ngx_int_t                        rc, type;
    ngx_proxy_protocol_tlv_ssl_t    *tlv_ssl;
    ngx_proxy_protocol_tlv_اندراج_t  *te;

    إذا (c->proxy_protocol == عدم) {
        أرجع NGX_DECLINED;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->لوگارتھم, 0,
                   "PROXY protocol v2 get tlv \"%V\"", name);

    te = ngx_proxy_protocol_tlv_entries;
    tlvs = &c->proxy_protocol->tlvs;

    p = name->انکڑے;
    n = name->len;

    إذا (n >= 4 && p[0] == 's' && p[1] == 's' && p[2] == 'l' && p[3] == '_') {

        rc = ngx_proxy_protocol_lookup_tlv(c, tlvs,
                                           NGX_PROXY_PROTOCOL_TLV_SSL, &ssl);
        إذا (rc != NGX_OK) {
            أرجع rc;
        }

        إذا (ssl.len < حجم(ngx_proxy_protocol_tlv_ssl_t)) {
            أرجع NGX_ERROR;
        }

        p += 4;
        n -= 4;

        إذا (n == 6 && ngx_strncmp(p, "verify", 6) == 0) {

            tlv_ssl = (ngx_proxy_protocol_tlv_ssl_t *) ssl.انکڑے;
            verify = ngx_proxy_protocol_parse_uint32(tlv_ssl->verify);

            قدر->انکڑے = ngx_pnalloc(c->pool, NGX_INT32_LEN);
            إذا (قدر->انکڑے == عدم) {
                أرجع NGX_ERROR;
            }

            قدر->len = ngx_sprintf(قدر->انکڑے, "%uD", verify)
                         - قدر->انکڑے;
            أرجع NGX_OK;
        }

        ssl.انکڑے += حجم(ngx_proxy_protocol_tlv_ssl_t);
        ssl.len -= حجم(ngx_proxy_protocol_tlv_ssl_t);

        te = ngx_proxy_protocol_tlv_ssl_entries;
        tlvs = &ssl;
    }

    إذا (n >= 2 && p[0] == '0' && p[1] == 'x') {

        type = ngx_hextoi(p + 2, n - 2);
        إذا (type == NGX_ERROR) {
            ngx_log_error(NGX_LOG_ERR, c->لوگارتھم, 0,
                          "invalid PROXY protocol TLV \"%V\"", name);
            أرجع NGX_ERROR;
        }

        أرجع ngx_proxy_protocol_lookup_tlv(c, tlvs, type, قدر);
    }

    لكل ( /* void */ ; te->type; te++) {
        إذا (te->name.len == n && ngx_strncmp(te->name.انکڑے, p, n) == 0) {
            أرجع ngx_proxy_protocol_lookup_tlv(c, tlvs, te->type, قدر);
        }
    }

    ngx_log_error(NGX_LOG_ERR, c->لوگارتھم, 0,
                  "unknown PROXY protocol TLV \"%V\"", name);

    أرجع NGX_DECLINED;
}


ساكن ngx_int_t
ngx_proxy_protocol_lookup_tlv(ngx_connection_t *c, ngx_str_t *tlvs,
    ngx_uint_t type, ngx_str_t *قدر)
{
    u_char                    *p;
    ناپ_t                     n, len;
    ngx_proxy_protocol_tlv_t  *tlv;

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->لوگارتھم, 0,
                   "PROXY protocol v2 lookup tlv:%02xi", type);

    p = tlvs->انکڑے;
    n = tlvs->len;

    طالما (n) {
        إذا (n < حجم(ngx_proxy_protocol_tlv_t)) {
            ngx_log_error(NGX_LOG_ERR, c->لوگارتھم, 0, "broken PROXY protocol TLV");
            أرجع NGX_ERROR;
        }

        tlv = (ngx_proxy_protocol_tlv_t *) p;
        len = ngx_proxy_protocol_parse_uint16(tlv->len);

        p += حجم(ngx_proxy_protocol_tlv_t);
        n -= حجم(ngx_proxy_protocol_tlv_t);

        إذا (n < len) {
            ngx_log_error(NGX_LOG_ERR, c->لوگارتھم, 0, "broken PROXY protocol TLV");
            أرجع NGX_ERROR;
        }

        إذا (tlv->type == type) {
            قدر->انکڑے = p;
            قدر->len = len;
            أرجع NGX_OK;
        }

        p += len;
        n -= len;
    }

    أرجع NGX_DECLINED;
}


u_char *
ngx_proxy_protocol_v2_write(ngx_connection_t *c, u_char *buf, u_char *آخری,
    ngx_array_t *tlvs /* reserved */ )
{
    u_char                          *p;
    uint32_t                         verify;
    ngx_uint_t                       i, client;
    ngx_array_t                      ادھار_tlvs, ssl_tlvs;
    ngx_proxy_protocol_tlv_قدر_t  *tlv;

    client = 0;
    verify = 0;

    ngx_memzero(&ادھار_tlvs, حجم(ngx_array_t));
    ngx_memzero(&ssl_tlvs, حجم(ngx_array_t));

#إذا (NGX_OPENSSL)

    إذا (c->ssl != عدم) {
        إذا (ngx_array_init(&ادھار_tlvs, c->pool, 2,
                           حجم(ngx_proxy_protocol_tlv_قدر_t))
            != NGX_OK)
        {
            أرجع عدم;
        }

        إذا (ngx_array_init(&ssl_tlvs, c->pool, 5,
                           حجم(ngx_proxy_protocol_tlv_قدر_t))
            != NGX_OK)
        {
            أرجع عدم;
        }

        إذا (ngx_proxy_protocol_v2_eval_ssl(c, &ادھار_tlvs, &ssl_tlvs, &client,
                                           &verify)
            != NGX_OK)
        {
            أرجع عدم;
        }
    }

#جے_انت

    p = ngx_proxy_protocol_v2_write_سرخی(c, buf, آخری);
    إذا (p == عدم) {
        أرجع عدم;
    }

    tlv = ادھار_tlvs.elts;

    لكل (i = 0; i < ادھار_tlvs.nelts; i++) {
        p = ngx_proxy_protocol_v2_write_tlv(c, p, آخری, tlv[i].type,
                                           &tlv[i].قدر);
        إذا (p == عدم) {
            أرجع عدم;
        }
    }

    إذا (client) {
        p = ngx_proxy_protocol_v2_write_ssl(c, p, آخری, &ssl_tlvs, client,
                                            verify);
        إذا (p == عدم) {
            أرجع عدم;
        }
    }

    ngx_proxy_protocol_v2_سیٹ_len(buf,
                                  p + حجم(ngx_proxy_protocol_tlv_t) + 4);

    أرجع ngx_proxy_protocol_v2_write_crc32c(c, buf, p, آخری);
}


#إذا (NGX_OPENSSL)

ساكن ngx_int_t
ngx_proxy_protocol_v2_eval_ssl(ngx_connection_t *c, ngx_array_t *tlvs,
    ngx_array_t *ssl_tlvs, ngx_uint_t *client, uint32_t *verify)
{
    X509                            *cert;
    لما                             n;
    ngx_int_t                        rc;
    ngx_str_t                        قدر;
    ngx_uint_t                       type;
    ثابت حرف                      *s;
    ngx_proxy_protocol_tlv_قدر_t  *tlv;

    إذا (ngx_proxy_protocol_v2_authority(c, &قدر) == NGX_OK) {
        tlv = ngx_array_ڈالو(tlvs);
        إذا (tlv == عدم) {
            أرجع NGX_ERROR;
        }

        tlv->type = NGX_PROXY_PROTOCOL_TLV_AUTHORITY;
        tlv->قدر = قدر;
    }

    إذا (ngx_proxy_protocol_v2_alpn(c, &قدر) == NGX_OK) {
        tlv = ngx_array_ڈالو(tlvs);
        إذا (tlv == عدم) {
            أرجع NGX_ERROR;
        }

        tlv->type = NGX_PROXY_PROTOCOL_TLV_ALPN;
        tlv->قدر = قدر;
    }

    لكل (type = NGX_PROXY_PROTOCOL_TLV_SSL_نسخہ;
         type <= NGX_PROXY_PROTOCOL_TLV_SSL_کلید_ALG;
         type++)
    {
        rc = ngx_proxy_protocol_v2_ssl_sub(c, type, &قدر);

        إذا (rc == NGX_ERROR) {
            أرجع NGX_ERROR;
        }

        إذا (rc != NGX_OK) {
            استمر;
        }

        tlv = ngx_array_ڈالو(ssl_tlvs);
        إذا (tlv == عدم) {
            أرجع NGX_ERROR;
        }

        tlv->type = type;
        tlv->قدر = قدر;
    }

    *client = NGX_PROXY_PROTOCOL_V2_CLIENT_SSL;
    *verify = 1;  /* X509_V_ERR_UNSPECIFIED */

    cert = SSL_get_peer_certificate(c->ssl->connection);

    إذا (cert != عدم) {
        X509_free(cert);

        *client |= NGX_PROXY_PROTOCOL_V2_CLIENT_CERT_SESS;

        إذا (!SSL_session_reused(c->ssl->connection)) {
            *client |= NGX_PROXY_PROTOCOL_V2_CLIENT_CERT_CONN;
        }

        n = SSL_get_verify_نتیجہ(c->ssl->connection);

        إذا (n == X509_V_OK) {

            إذا (ngx_ssl_ocsp_get_حالت(c, &s) != NGX_OK) {
                n = X509_V_ERR_CERT_REVOKED;
            }
        }

        *verify = (uint32_t) n;
    }

    أرجع NGX_OK;
}


ساكن ngx_int_t
ngx_proxy_protocol_v2_authority(ngx_connection_t *c, ngx_str_t *باہرلا)
{
#جے_تعریف_شدہ SSL_CTRL_سیٹ_TLSEXT_HOSTNAME

    ثابت حرف  *sni;

    sni = SSL_get_servername(c->ssl->connection, TLSEXT_NAMETYPE_host_name);
    إذا (sni == عدم) {
        أرجع NGX_DECLINED;
    }

    باہرلا->انکڑے = (u_char *) sni;
    باہرلا->len = ngx_strlen(sni);
    أرجع NGX_OK;

#وإلا
    أرجع NGX_DECLINED;
#جے_انت
}


ساكن ngx_int_t
ngx_proxy_protocol_v2_alpn(ngx_connection_t *c, ngx_str_t *باہرلا)
{
#جے_تعریف_شدہ TLSEXT_TYPE_application_layer_protocol_negotiation

    بنانشانی صحيح          alpnlen;
    ثابت بنانشانی حرف  *alpn;

    SSL_get0_alpn_selected(c->ssl->connection, &alpn, &alpnlen);
    إذا (alpn == عدم || alpnlen == 0) {
        أرجع NGX_DECLINED;
    }

    باہرلا->انکڑے = (u_char *) alpn;
    باہرلا->len = alpnlen;
    أرجع NGX_OK;

#وإلا
    أرجع NGX_DECLINED;
#جے_انت
}


ساكن ngx_int_t
ngx_proxy_protocol_v2_ssl_sub(ngx_connection_t *c, ngx_uint_t type,
    ngx_str_t *باہرلا)
{
    صحيح               i, len;
    X509             *cert;
    u_char           *alg, *s;
    EVP_PKEY         *pkey;
#إذا (OPENSSL_نسخہ_نمبر >= 0x40000000L)
    ثابت
#جے_انت
    X509_NAME        *subject;
#إذا (OPENSSL_نسخہ_نمبر >= 0x40000000L)
    ثابت
#جے_انت
    X509_NAME_اندراج  *اندراج;

    بدل (type) {

    حالة NGX_PROXY_PROTOCOL_TLV_SSL_نسخہ:
        باہرلا->انکڑے = (u_char *) SSL_get_نسخہ(c->ssl->connection);
        باہرلا->len = ngx_strlen(باہرلا->انکڑے);
        أرجع NGX_OK;

    حالة NGX_PROXY_PROTOCOL_TLV_SSL_CIPHER:
        باہرلا->انکڑے = (u_char *) SSL_get_cipher_name(c->ssl->connection);
        باہرلا->len = ngx_strlen(باہرلا->انکڑے);
        أرجع NGX_OK;

    حالة NGX_PROXY_PROTOCOL_TLV_SSL_CN:
        cert = SSL_get_peer_certificate(c->ssl->connection);
        إذا (cert == عدم) {
            أرجع NGX_DECLINED;
        }

        subject = X509_get_subject_name(cert);
        إذا (subject == عدم) {
            X509_free(cert);
            أرجع NGX_ERROR;
        }

        i = X509_NAME_get_اشاریہ_by_NID(subject, NID_commonName, -1);
        إذا (i < 0) {
            X509_free(cert);
            أرجع NGX_DECLINED;
        }

        اندراج = X509_NAME_get_اندراج(subject, i);

        len = ASN1_STRING_منزل_UTF8(&s, X509_NAME_اندراج_get_انکڑے(اندراج));
        إذا (len < 0) {
            ngx_ssl_error(NGX_LOG_ALERT, c->لوگارتھم, 0,
                          "ASN1_STRING_to_UTF8() failed");
            X509_free(cert);
            أرجع NGX_ERROR;
        }

        باہرلا->len = len;
        باہرلا->انکڑے = ngx_pnalloc(c->pool, len);
        إذا (باہرلا->انکڑے == عدم) {
            OPENSSL_free(s);
            X509_free(cert);
            أرجع NGX_ERROR;
        }

        ngx_memcpy(باہرلا->انکڑے, s, len);

        OPENSSL_free(s);
        X509_free(cert);
        أرجع NGX_OK;

    حالة NGX_PROXY_PROTOCOL_TLV_SSL_SIG_ALG:
        cert = SSL_get_certificate(c->ssl->connection);

        باہرلا->انکڑے = (u_char *) OBJ_nid2sn(X509_get_signature_nid(cert));
        باہرلا->len = ngx_strlen(باہرلا->انکڑے);
        أرجع NGX_OK;

    حالة NGX_PROXY_PROTOCOL_TLV_SSL_کلید_ALG:
        cert = SSL_get_certificate(c->ssl->connection);

        pkey = X509_get_pubkey(cert);
        إذا (pkey == عدم) {
            أرجع NGX_ERROR;
        }

        بدل (EVP_PKEY_ادھار_id(pkey)) {

        حالة EVP_PKEY_RSA:
#جے_تعریف_شدہ EVP_PKEY_RSA_PSS
        حالة EVP_PKEY_RSA_PSS:
#جے_انت
            alg = (u_char *) "RSA";
            اكسر;

        حالة EVP_PKEY_EC:
            alg = (u_char *) "EC";
            اكسر;

        حالة EVP_PKEY_DSA:
            alg = (u_char *) "DSA";
            اكسر;

        افتراضي:
            EVP_PKEY_free(pkey);
            أرجع NGX_DECLINED;
        }

        باہرلا->len = حجم("RSA16384") - 1;
        باہرلا->انکڑے = ngx_pnalloc(c->pool, باہرلا->len);
        إذا (باہرلا->انکڑے == عدم) {
            EVP_PKEY_free(pkey);
            أرجع NGX_ERROR;
        }

        باہرلا->len = ngx_snprintf(باہرلا->انکڑے, باہرلا->len, "%s%d",
                                alg, EVP_PKEY_بٹ(pkey))
                   - باہرلا->انکڑے;

        EVP_PKEY_free(pkey);

        أرجع NGX_OK;
    }

    أرجع NGX_DECLINED;
}

#جے_انت


ساكن u_char *
ngx_proxy_protocol_v2_write_سرخی(ngx_connection_t *c, u_char *buf,
    u_char *آخری)
{
    u_char                            *p, src_af, dst_af,
                                       family, command, transport;
    ngx_uint_t                         src_fam, dst_fam;
    ngx_proxy_protocol_سرخی_t       *سرخی;
    ngx_proxy_protocol_inet_addrs_t   *وچ;
#إذا (NGX_HAVE_INET6)
    ngx_uint_t                         پورٹ, lport;
    بنية in6_addr                   *src6, *dst6;
    ngx_proxy_protocol_inet6_addrs_t  *in6;
#جے_انت

    إذا (آخری - buf < NGX_PROXY_PROTOCOL_V2_MAX_سرخی) {
        ngx_log_error(NGX_LOG_ALERT, c->لوگارتھم, 0,
                      "too small buffer for PROXY protocol v2");
        أرجع عدم;
    }

    إذا (ngx_connection_local_sockaddr(c, عدم, 0) != NGX_OK) {
        أرجع عدم;
    }

    /* the length field is set by the caller once all TLVs are written */

    سرخی = (ngx_proxy_protocol_سرخی_t *) buf;

    ngx_memcpy(سرخی->signature, ngx_proxy_protocol_signature,
               حجم(ngx_proxy_protocol_signature) - 1);

    p = buf + حجم(ngx_proxy_protocol_سرخی_t);

    src_fam = c->sockaddr->sa_family;
    dst_fam = c->local_sockaddr->sa_family;

    src_af = ngx_proxy_protocol_v2_family(src_fam);
    dst_af = ngx_proxy_protocol_v2_family(dst_fam);

    /* promote to the highest address family present on either side */

    إذا (src_af == NGX_PROXY_PROTOCOL_AF_UNSPEC
        || dst_af == NGX_PROXY_PROTOCOL_AF_UNSPEC)
    {
        command = NGX_PROXY_PROTOCOL_CMD_LOCAL;
        family = NGX_PROXY_PROTOCOL_AF_UNSPEC;
        transport = NGX_PROXY_PROTOCOL_TYPE_UNSPEC;

    } وإلا {
        command = NGX_PROXY_PROTOCOL_CMD_PROXY;
        family = ngx_max(src_af, dst_af);

        بدل (c->type) {
        حالة SOCK_دھارا:
            transport = NGX_PROXY_PROTOCOL_TYPE_دھارا;
            اكسر;
        حالة SOCK_DGRAM:
            transport = NGX_PROXY_PROTOCOL_TYPE_DGRAM;
            اكسر;
        افتراضي:
            transport = NGX_PROXY_PROTOCOL_TYPE_UNSPEC;
            اكسر;
        }
    }

    سرخی->نسخہ_command = 0x20 | command;
    سرخی->family_transport = (family << 4) | transport;

    بدل (family) {

    حالة NGX_PROXY_PROTOCOL_AF_INET:

        وچ = (ngx_proxy_protocol_inet_addrs_t *) p;

        ngx_proxy_protocol_v2_write_ipv4(c->sockaddr,
                                         وچ->src_addr, وچ->src_پورٹ);
        ngx_proxy_protocol_v2_write_ipv4(c->local_sockaddr,
                                         وچ->dst_addr, وچ->dst_پورٹ);

        p += حجم(ngx_proxy_protocol_inet_addrs_t);

        اكسر;

#إذا (NGX_HAVE_INET6)

    حالة NGX_PROXY_PROTOCOL_AF_INET6:

        إذا (src_fam == AF_INET6 && dst_fam == AF_INET6) {
            src6 = &((بنية sockaddr_in6 *) c->sockaddr)->sin6_addr;
            dst6 = &((بنية sockaddr_in6 *) c->local_sockaddr)->sin6_addr;

            إذا (IN6_IS_ADDR_V4MAPPED(src6) && IN6_IS_ADDR_V4MAPPED(dst6)) {

                /* both v4-mapped: demote to AF_INET */

                سرخی->family_transport = (NGX_PROXY_PROTOCOL_AF_INET << 4)
                                           | transport;

                وچ = (ngx_proxy_protocol_inet_addrs_t *) p;

                ngx_memcpy(وچ->src_addr, src6->s6_addr + 12, 4);
                ngx_memcpy(وچ->dst_addr, dst6->s6_addr + 12, 4);

                پورٹ  = ngx_inet_get_پورٹ(c->sockaddr);
                lport = ngx_inet_get_پورٹ(c->local_sockaddr);

                وچ->src_پورٹ[0] = (u_char) (پورٹ >> 8);
                وچ->src_پورٹ[1] = (u_char)  پورٹ;
                وچ->dst_پورٹ[0] = (u_char) (lport >> 8);
                وچ->dst_پورٹ[1] = (u_char)  lport;

                p += حجم(ngx_proxy_protocol_inet_addrs_t);
                اكسر;
            }
        }

        in6 = (ngx_proxy_protocol_inet6_addrs_t *) p;

        ngx_proxy_protocol_v2_write_ipv6(c->sockaddr,
                                         in6->src_addr, in6->src_پورٹ);
        ngx_proxy_protocol_v2_write_ipv6(c->local_sockaddr,
                                         in6->dst_addr, in6->dst_پورٹ);

        p += حجم(ngx_proxy_protocol_inet6_addrs_t);

        اكسر;

#جے_انت

    افتراضي:  /* NGX_PROXY_PROTOCOL_AF_UNSPEC */
        اكسر;
    }

    أرجع p;
}


ساكن u_char
ngx_proxy_protocol_v2_family(ngx_uint_t family)
{
    بدل (family) {

    حالة AF_INET:
        أرجع NGX_PROXY_PROTOCOL_AF_INET;

#إذا (NGX_HAVE_INET6)
    حالة AF_INET6:
        أرجع NGX_PROXY_PROTOCOL_AF_INET6;
#جے_انت

    افتراضي:
        أرجع NGX_PROXY_PROTOCOL_AF_UNSPEC;
    }
}


ساكن فراغ
ngx_proxy_protocol_v2_write_ipv4(بنية sockaddr *sa, u_char *addr,
    u_char *پورٹ)
{
    بنية sockaddr_وچ  *جیب;

    جیب = (بنية sockaddr_وچ *) sa;
    ngx_memcpy(addr, &جیب->sin_addr, 4);
    ngx_memcpy(پورٹ, &جیب->sin_پورٹ, 2);
}


#إذا (NGX_HAVE_INET6)

ساكن فراغ
ngx_proxy_protocol_v2_write_ipv6(بنية sockaddr *sa, u_char *addr,
    u_char *پورٹ)
{
    بنية sockaddr_وچ   *جیب;
    بنية sockaddr_in6  *sin6;

    /* IPv4-mapped prefix: 80 zero bits + 16 one bits */
    ساكن ثابت u_char  mapped[]   = { 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0xff, 0xff };

    إذا (sa->sa_family == AF_INET6) {
        sin6 = (بنية sockaddr_in6 *) sa;
        ngx_memcpy(addr, &sin6->sin6_addr, 16);
        ngx_memcpy(پورٹ, &sin6->sin6_پورٹ, 2);

    } وإلا {
        /* promote AF_INET to ::ffff:a.b.c.d */
        جیب = (بنية sockaddr_وچ *) sa;
        ngx_memcpy(addr, mapped, حجم(mapped));
        ngx_memcpy(addr + حجم(mapped), &جیب->sin_addr, 4);
        ngx_memcpy(پورٹ, &جیب->sin_پورٹ, 2);
    }
}

#جے_انت


ساكن u_char *
ngx_proxy_protocol_v2_write_tlv(ngx_connection_t *c, u_char *p, u_char *آخری,
    ngx_uint_t type, ngx_str_t *قدر)
{
    ngx_proxy_protocol_tlv_t  *tlv;

    إذا ((ناپ_t) (آخری - p) < حجم(ngx_proxy_protocol_tlv_t) + قدر->len) {
        ngx_log_error(NGX_LOG_ALERT, c->لوگارتھم, 0,
                      "too small buffer for PROXY protocol v2 TLV");
        أرجع عدم;
    }

    tlv = (ngx_proxy_protocol_tlv_t *) p;
    tlv->type = (u_char) type;
    tlv->len[0] = (u_char) (قدر->len >> 8);
    tlv->len[1] = (u_char)  قدر->len;

    p += حجم(ngx_proxy_protocol_tlv_t);

    أرجع ngx_cpymem(p, قدر->انکڑے, قدر->len);
}


ساكن u_char *
ngx_proxy_protocol_v2_write_ssl(ngx_connection_t *c, u_char *p, u_char *آخری,
    ngx_array_t *ssl_tlvs, ngx_uint_t client, uint32_t verify)
{
    u_char                          *شروع;
    uint16_t                         len;
    ngx_uint_t                       i;
    ngx_proxy_protocol_tlv_t        *tlv;
    ngx_proxy_protocol_tlv_ssl_t    *tlv_ssl;
    ngx_proxy_protocol_tlv_قدر_t  *sub;

    إذا ((ناپ_t) (آخری - p) < حجم(ngx_proxy_protocol_tlv_t)
                              + حجم(ngx_proxy_protocol_tlv_ssl_t))
    {
        ngx_log_error(NGX_LOG_ALERT, c->لوگارتھم, 0,
                      "too small buffer for PROXY protocol v2 SSL TLV");
        أرجع عدم;
    }

    شروع = p;
    p += حجم(ngx_proxy_protocol_tlv_t);

    tlv_ssl = (ngx_proxy_protocol_tlv_ssl_t *) p;
    tlv_ssl->client = (u_char) client;
    tlv_ssl->verify[0] = (u_char) (verify >> 24);
    tlv_ssl->verify[1] = (u_char) (verify >> 16);
    tlv_ssl->verify[2] = (u_char) (verify >> 8);
    tlv_ssl->verify[3] = (u_char)  verify;

    p += حجم(ngx_proxy_protocol_tlv_ssl_t);

    sub = ssl_tlvs->elts;

    لكل (i = 0; i < ssl_tlvs->nelts; i++) {
        p = ngx_proxy_protocol_v2_write_tlv(c, p, آخری, sub[i].type,
                                            &sub[i].قدر);
        إذا (p == عدم) {
            أرجع عدم;
        }
    }

    len = (uint16_t) (p - شروع - حجم(ngx_proxy_protocol_tlv_t));

    tlv = (ngx_proxy_protocol_tlv_t *) شروع;
    tlv->type = NGX_PROXY_PROTOCOL_TLV_SSL;
    tlv->len[0] = (u_char) (len >> 8);
    tlv->len[1] = (u_char)  len;

    أرجع p;
}


ساكن فراغ
ngx_proxy_protocol_v2_سیٹ_len(u_char *buf, u_char *p)
{
    uint16_t                      len;
    ngx_proxy_protocol_سرخی_t  *سرخی;

    len = (uint16_t) (p - buf - حجم(ngx_proxy_protocol_سرخی_t));

    سرخی = (ngx_proxy_protocol_سرخی_t *) buf;
    سرخی->len[0] = (u_char) (len >> 8);
    سرخی->len[1] = (u_char) len;
}


ساكن u_char *
ngx_proxy_protocol_v2_write_crc32c(ngx_connection_t *c, u_char *buf,
    u_char *p, u_char *آخری)
{
    u_char                    *قدر;
    uint32_t                   چکری_جانچ;
    ngx_proxy_protocol_tlv_t  *tlv;

    إذا ((ناپ_t) (آخری - p) < حجم(ngx_proxy_protocol_tlv_t) + 4) {
        ngx_log_error(NGX_LOG_ALERT, c->لوگارتھم, 0,
                      "too small buffer for PROXY protocol v2 CRC32c TLV");
        أرجع عدم;
    }

    /* append the CRC32c TLV with a zeroed value field */

    tlv = (ngx_proxy_protocol_tlv_t *) p;
    tlv->type = NGX_PROXY_PROTOCOL_TLV_چکری_جانچ_32_C;
    tlv->len[0] = 0;
    tlv->len[1] = 4;

    قدر = p + حجم(ngx_proxy_protocol_tlv_t);

    ngx_memzero(قدر, 4);

    p = قدر + 4;

    /* the checksum covers the entire header with the zeroed value */

    چکری_جانچ = ngx_crc32c_long(buf, p - buf);

    قدر[0] = (u_char) (چکری_جانچ >> 24);
    قدر[1] = (u_char) (چکری_جانچ >> 16);
    قدر[2] = (u_char) (چکری_جانچ >> 8);
    قدر[3] = (u_char) چکری_جانچ;

    أرجع p;
}
