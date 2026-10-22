
/*
 * Copyright (C) Roman Arutyunyan
 * Copyright (C) Nginx, Inc.
 */


#شامل <ngx_config.h>
#شامل <ngx_core.h>


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


قسمتعریف ساخت {
    u_char                                  signature[12];
    u_char                                  نسخہ_command;
    u_char                                  family_transport;
    u_char                                  len[2];
} ngx_proxy_protocol_سرخی_t;


قسمتعریف ساخت {
    u_char                                  src_addr[4];
    u_char                                  dst_addr[4];
    u_char                                  src_پورٹ[2];
    u_char                                  dst_پورٹ[2];
} ngx_proxy_protocol_inet_addrs_t;


قسمتعریف ساخت {
    u_char                                  src_addr[16];
    u_char                                  dst_addr[16];
    u_char                                  src_پورٹ[2];
    u_char                                  dst_پورٹ[2];
} ngx_proxy_protocol_inet6_addrs_t;


قسمتعریف ساخت {
    u_char                                  type;
    u_char                                  len[2];
} ngx_proxy_protocol_tlv_t;


قسمتعریف ساخت {
    u_char                                  client;
    u_char                                  verify[4];
} ngx_proxy_protocol_tlv_ssl_t;


قسمتعریف ساخت {
    ngx_str_t                               name;
    ngx_uint_t                              type;
} ngx_proxy_protocol_tlv_اندراج_t;


قسمتعریف ساخت {
    ngx_uint_t                              type;
    ngx_str_t                               قدر;
} ngx_proxy_protocol_tlv_قدر_t;


ساکن u_char *ngx_proxy_protocol_read_addr(ngx_connection_t *c, u_char *p,
    u_char *آخری, ngx_str_t *addr);
ساکن u_char *ngx_proxy_protocol_read_پورٹ(u_char *p, u_char *آخری,
    اندر_پورٹ_t *پورٹ, u_char sep);
ساکن u_char *ngx_proxy_protocol_v2_read(ngx_connection_t *c, u_char *buf,
    u_char *آخری);
ساکن ngx_int_t ngx_proxy_protocol_lookup_tlv(ngx_connection_t *c,
    ngx_str_t *tlvs, ngx_uint_t type, ngx_str_t *قدر);
#اگر (NGX_OPENSSL)
ساکن ngx_int_t ngx_proxy_protocol_v2_eval_ssl(ngx_connection_t *c,
    ngx_array_t *tlvs, ngx_array_t *ssl_tlvs, ngx_uint_t *client,
    uint32_t *verify);
ساکن ngx_int_t ngx_proxy_protocol_v2_authority(ngx_connection_t *c,
    ngx_str_t *برآمد);
ساکن ngx_int_t ngx_proxy_protocol_v2_alpn(ngx_connection_t *c,
    ngx_str_t *برآمد);
ساکن ngx_int_t ngx_proxy_protocol_v2_ssl_sub(ngx_connection_t *c,
    ngx_uint_t type, ngx_str_t *برآمد);
#اگر_ختم
ساکن u_char *ngx_proxy_protocol_v2_write_سرخی(ngx_connection_t *c,
    u_char *buf, u_char *آخری);
ساکن u_char ngx_proxy_protocol_v2_family(ngx_uint_t family);
ساکن خالی ngx_proxy_protocol_v2_write_ipv4(ساخت sockaddr *sa, u_char *addr,
    u_char *پورٹ);
ساکن خالی ngx_proxy_protocol_v2_write_ipv6(ساخت sockaddr *sa, u_char *addr,
    u_char *پورٹ);
ساکن u_char *ngx_proxy_protocol_v2_write_tlv(ngx_connection_t *c, u_char *p,
    u_char *آخری, ngx_uint_t type, ngx_str_t *قدر);
ساکن u_char *ngx_proxy_protocol_v2_write_ssl(ngx_connection_t *c, u_char *p,
    u_char *آخری, ngx_array_t *ssl_tlvs, ngx_uint_t client, uint32_t verify);
ساکن خالی ngx_proxy_protocol_v2_سیٹ_len(u_char *buf, u_char *p);
ساکن u_char *ngx_proxy_protocol_v2_write_crc32c(ngx_connection_t *c,
    u_char *buf, u_char *p, u_char *آخری);


ساکن ngx_proxy_protocol_tlv_اندراج_t  ngx_proxy_protocol_tlv_entries[] = {
    { ngx_string("alpn"),       NGX_PROXY_PROTOCOL_TLV_ALPN },
    { ngx_string("authority"),  NGX_PROXY_PROTOCOL_TLV_AUTHORITY },
    { ngx_string("unique_id"),  NGX_PROXY_PROTOCOL_TLV_UNIQUE_ID },
    { ngx_string("ssl"),        NGX_PROXY_PROTOCOL_TLV_SSL },
    { ngx_string("netns"),      NGX_PROXY_PROTOCOL_TLV_NETNS },
    { ngx_null_string,          0x00 }
};


ساکن ngx_proxy_protocol_tlv_اندراج_t  ngx_proxy_protocol_tlv_ssl_entries[] = {
    { ngx_string("version"),    NGX_PROXY_PROTOCOL_TLV_SSL_نسخہ },
    { ngx_string("cn"),         NGX_PROXY_PROTOCOL_TLV_SSL_CN },
    { ngx_string("cipher"),     NGX_PROXY_PROTOCOL_TLV_SSL_CIPHER },
    { ngx_string("sig_alg"),    NGX_PROXY_PROTOCOL_TLV_SSL_SIG_ALG },
    { ngx_string("key_alg"),    NGX_PROXY_PROTOCOL_TLV_SSL_کلید_ALG },
    { ngx_null_string,          0x00 }
};


ساکن ثابت u_char  ngx_proxy_protocol_signature[] = "\r\n\r\n\0\r\nQUIT\n";


u_char *
ngx_proxy_protocol_read(ngx_connection_t *c, u_char *buf, u_char *آخری)
{
    سائز_t                 len;
    u_char                *p;
    ngx_proxy_protocol_t  *pp;

    p = buf;
    len = آخری - buf;

    اگر (len >= حجم(ngx_proxy_protocol_سرخی_t)
        && ngx_memcmp(p, ngx_proxy_protocol_signature,
                      حجم(ngx_proxy_protocol_signature) - 1)
           == 0)
    {
        واپس ngx_proxy_protocol_v2_read(c, buf, آخری);
    }

    اگر (len < 8 || ngx_strncmp(p, "PROXY ", 6) != 0) {
        جاؤ invalid;
    }

    p += 6;
    len -= 6;

    اگر (len >= 7 && ngx_strncmp(p, "UNKNOWN", 7) == 0) {
        ngx_log_debug0(NGX_LOG_DEBUG_CORE, c->لوگارتھم, 0,
                       "PROXY protocol unknown protocol");
        p += 7;
        جاؤ skip;
    }

    اگر (len < 5 || ngx_strncmp(p, "TCP", 3) != 0
        || (p[3] != '4' && p[3] != '6') || p[4] != ' ')
    {
        جاؤ invalid;
    }

    p += 5;

    pp = ngx_pcalloc(c->pool, حجم(ngx_proxy_protocol_t));
    اگر (pp == تہی) {
        واپس تہی;
    }

    p = ngx_proxy_protocol_read_addr(c, p, آخری, &pp->src_addr);
    اگر (p == تہی) {
        جاؤ invalid;
    }

    p = ngx_proxy_protocol_read_addr(c, p, آخری, &pp->dst_addr);
    اگر (p == تہی) {
        جاؤ invalid;
    }

    p = ngx_proxy_protocol_read_پورٹ(p, آخری, &pp->src_پورٹ, ' ');
    اگر (p == تہی) {
        جاؤ invalid;
    }

    p = ngx_proxy_protocol_read_پورٹ(p, آخری, &pp->dst_پورٹ, CR);
    اگر (p == تہی) {
        جاؤ invalid;
    }

    اگر (p == آخری) {
        جاؤ invalid;
    }

    اگر (*p++ != LF) {
        جاؤ invalid;
    }

    ngx_log_debug4(NGX_LOG_DEBUG_CORE, c->لوگارتھم, 0,
                   "PROXY protocol src: %V %d, dst: %V %d",
                   &pp->src_addr, pp->src_پورٹ, &pp->dst_addr, pp->dst_پورٹ);

    c->proxy_protocol = pp;

    واپس p;

skip:

    برائے ( /* void */ ; p < آخری - 1; p++) {
        اگر (p[0] == CR && p[1] == LF) {
            واپس p + 2;
        }
    }

invalid:

    برائے (p = buf; p < آخری; p++) {
        اگر (*p == CR || *p == LF) {
            توڑو;
        }
    }

    ngx_log_error(NGX_LOG_ERR, c->لوگارتھم, 0,
                  "broken header: \"%*s\"", (سائز_t) (p - buf), buf);

    واپس تہی;
}


ساکن u_char *
ngx_proxy_protocol_read_addr(ngx_connection_t *c, u_char *p, u_char *آخری,
    ngx_str_t *addr)
{
    سائز_t  len;
    u_char  ch, *pos;

    pos = p;

    برائے ( ;; ) {
        اگر (p == آخری) {
            واپس تہی;
        }

        ch = *p++;

        اگر (ch == ' ') {
            توڑو;
        }

        اگر (ch != ':' && ch != '.'
            && (ch < 'a' || ch > 'f')
            && (ch < 'A' || ch > 'F')
            && (ch < '0' || ch > '9'))
        {
            واپس تہی;
        }
    }

    len = p - pos - 1;

    addr->ڈیٹا = ngx_pnalloc(c->pool, len);
    اگر (addr->ڈیٹا == تہی) {
        واپس تہی;
    }

    ngx_memcpy(addr->ڈیٹا, pos, len);
    addr->len = len;

    واپس p;
}


ساکن u_char *
ngx_proxy_protocol_read_پورٹ(u_char *p, u_char *آخری, اندر_پورٹ_t *پورٹ,
    u_char sep)
{
    سائز_t      len;
    u_char     *pos;
    ngx_int_t   n;

    pos = p;

    برائے ( ;; ) {
        اگر (p == آخری) {
            واپس تہی;
        }

        اگر (*p++ == sep) {
            توڑو;
        }
    }

    len = p - pos - 1;

    n = ngx_atoi(pos, len);
    اگر (n < 0 || n > 65535) {
        واپس تہی;
    }

    *پورٹ = (اندر_پورٹ_t) n;

    واپس p;
}


u_char *
ngx_proxy_protocol_write(ngx_connection_t *c, u_char *buf, u_char *آخری)
{
    ngx_uint_t  پورٹ, lport;

    اگر (آخری - buf < NGX_PROXY_PROTOCOL_V1_MAX_سرخی) {
        ngx_log_error(NGX_LOG_ALERT, c->لوگارتھم, 0,
                      "too small buffer for PROXY protocol");
        واپس تہی;
    }

    اگر (ngx_connection_local_sockaddr(c, تہی, 0) != NGX_OK) {
        واپس تہی;
    }

    انتخاب (c->sockaddr->sa_family) {

    حالت AF_INET:
        buf = ngx_cpymem(buf, "PROXY TCP4 ", حجم("PROXY TCP4 ") - 1);
        توڑو;

#اگر (NGX_HAVE_INET6)
    حالت AF_INET6:
        buf = ngx_cpymem(buf, "PROXY TCP6 ", حجم("PROXY TCP6 ") - 1);
        توڑو;
#اگر_ختم

    طےشدہ:
        واپس ngx_cpymem(buf, "PROXY UNKNOWN" CRLF,
                          حجم("PROXY UNKNOWN" CRLF) - 1);
    }

    buf += ngx_sock_ntop(c->sockaddr, c->socklen, buf, آخری - buf, 0);

    *buf++ = ' ';

    buf += ngx_sock_ntop(c->local_sockaddr, c->local_socklen, buf, آخری - buf,
                         0);

    پورٹ = ngx_inet_get_پورٹ(c->sockaddr);
    lport = ngx_inet_get_پورٹ(c->local_sockaddr);

    واپس ngx_slprintf(buf, آخری, " %ui %ui" CRLF, پورٹ, lport);
}


ساکن u_char *
ngx_proxy_protocol_v2_read(ngx_connection_t *c, u_char *buf, u_char *آخری)
{
    u_char                             *انتہا;
    سائز_t                              len;
    socklen_t                           socklen;
    ngx_uint_t                          نسخہ, command, family, transport;
    ngx_sockaddr_t                      src_sockaddr, dst_sockaddr;
    ngx_proxy_protocol_t               *pp;
    ngx_proxy_protocol_سرخی_t        *سرخی;
    ngx_proxy_protocol_inet_addrs_t    *اندر;
#اگر (NGX_HAVE_INET6)
    ngx_proxy_protocol_inet6_addrs_t   *in6;
#اگر_ختم

    سرخی = (ngx_proxy_protocol_سرخی_t *) buf;

    buf += حجم(ngx_proxy_protocol_سرخی_t);

    نسخہ = سرخی->نسخہ_command >> 4;

    اگر (نسخہ != 2) {
        ngx_log_error(NGX_LOG_ERR, c->لوگارتھم, 0,
                      "unknown PROXY protocol version: %ui", نسخہ);
        واپس تہی;
    }

    len = ngx_proxy_protocol_parse_uint16(سرخی->len);

    اگر ((سائز_t) (آخری - buf) < len) {
        ngx_log_error(NGX_LOG_ERR, c->لوگارتھم, 0, "header is too large");
        واپس تہی;
    }

    انتہا = buf + len;

    command = سرخی->نسخہ_command & 0x0f;

    /* only PROXY is supported */
    اگر (command != NGX_PROXY_PROTOCOL_CMD_PROXY) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->لوگارتھم, 0,
                       "PROXY protocol v2 unsupported command %ui", command);
        واپس انتہا;
    }

    transport = سرخی->family_transport & 0x0f;

    /* only STREAM is supported */
    اگر (transport != NGX_PROXY_PROTOCOL_TYPE_دھارا) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->لوگارتھم, 0,
                       "PROXY protocol v2 unsupported transport %ui",
                       transport);
        واپس انتہا;
    }

    pp = ngx_pcalloc(c->pool, حجم(ngx_proxy_protocol_t));
    اگر (pp == تہی) {
        واپس تہی;
    }

    family = سرخی->family_transport >> 4;

    انتخاب (family) {

    حالت NGX_PROXY_PROTOCOL_AF_INET:

        اگر ((سائز_t) (انتہا - buf) < حجم(ngx_proxy_protocol_inet_addrs_t)) {
            واپس تہی;
        }

        اندر = (ngx_proxy_protocol_inet_addrs_t *) buf;

        src_sockaddr.sockaddr_اندر.sin_family = AF_INET;
        src_sockaddr.sockaddr_اندر.sin_پورٹ = 0;
        ngx_memcpy(&src_sockaddr.sockaddr_اندر.sin_addr, اندر->src_addr, 4);

        dst_sockaddr.sockaddr_اندر.sin_family = AF_INET;
        dst_sockaddr.sockaddr_اندر.sin_پورٹ = 0;
        ngx_memcpy(&dst_sockaddr.sockaddr_اندر.sin_addr, اندر->dst_addr, 4);

        pp->src_پورٹ = ngx_proxy_protocol_parse_uint16(اندر->src_پورٹ);
        pp->dst_پورٹ = ngx_proxy_protocol_parse_uint16(اندر->dst_پورٹ);

        socklen = حجم(ساخت sockaddr_اندر);

        buf += حجم(ngx_proxy_protocol_inet_addrs_t);

        توڑو;

#اگر (NGX_HAVE_INET6)

    حالت NGX_PROXY_PROTOCOL_AF_INET6:

        اگر ((سائز_t) (انتہا - buf) < حجم(ngx_proxy_protocol_inet6_addrs_t)) {
            واپس تہی;
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

        socklen = حجم(ساخت sockaddr_in6);

        buf += حجم(ngx_proxy_protocol_inet6_addrs_t);

        توڑو;

#اگر_ختم

    طےشدہ:
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->لوگارتھم, 0,
                       "PROXY protocol v2 unsupported address family %ui",
                       family);
        واپس انتہا;
    }

    pp->src_addr.ڈیٹا = ngx_pnalloc(c->pool, NGX_SOCKADDR_STRLEN);
    اگر (pp->src_addr.ڈیٹا == تہی) {
        واپس تہی;
    }

    pp->src_addr.len = ngx_sock_ntop(&src_sockaddr.sockaddr, socklen,
                                     pp->src_addr.ڈیٹا, NGX_SOCKADDR_STRLEN, 0);

    pp->dst_addr.ڈیٹا = ngx_pnalloc(c->pool, NGX_SOCKADDR_STRLEN);
    اگر (pp->dst_addr.ڈیٹا == تہی) {
        واپس تہی;
    }

    pp->dst_addr.len = ngx_sock_ntop(&dst_sockaddr.sockaddr, socklen,
                                     pp->dst_addr.ڈیٹا, NGX_SOCKADDR_STRLEN, 0);

    ngx_log_debug4(NGX_LOG_DEBUG_CORE, c->لوگارتھم, 0,
                   "PROXY protocol v2 src: %V %d, dst: %V %d",
                   &pp->src_addr, pp->src_پورٹ, &pp->dst_addr, pp->dst_پورٹ);

    اگر (buf < انتہا) {
        pp->tlvs.ڈیٹا = ngx_pnalloc(c->pool, انتہا - buf);
        اگر (pp->tlvs.ڈیٹا == تہی) {
            واپس تہی;
        }

        ngx_memcpy(pp->tlvs.ڈیٹا, buf, انتہا - buf);
        pp->tlvs.len = انتہا - buf;
    }

    c->proxy_protocol = pp;

    واپس انتہا;
}


ngx_int_t
ngx_proxy_protocol_get_tlv(ngx_connection_t *c, ngx_str_t *name,
    ngx_str_t *قدر)
{
    u_char                          *p;
    سائز_t                           n;
    uint32_t                         verify;
    ngx_str_t                        ssl, *tlvs;
    ngx_int_t                        rc, type;
    ngx_proxy_protocol_tlv_ssl_t    *tlv_ssl;
    ngx_proxy_protocol_tlv_اندراج_t  *te;

    اگر (c->proxy_protocol == تہی) {
        واپس NGX_DECLINED;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->لوگارتھم, 0,
                   "PROXY protocol v2 get tlv \"%V\"", name);

    te = ngx_proxy_protocol_tlv_entries;
    tlvs = &c->proxy_protocol->tlvs;

    p = name->ڈیٹا;
    n = name->len;

    اگر (n >= 4 && p[0] == 's' && p[1] == 's' && p[2] == 'l' && p[3] == '_') {

        rc = ngx_proxy_protocol_lookup_tlv(c, tlvs,
                                           NGX_PROXY_PROTOCOL_TLV_SSL, &ssl);
        اگر (rc != NGX_OK) {
            واپس rc;
        }

        اگر (ssl.len < حجم(ngx_proxy_protocol_tlv_ssl_t)) {
            واپس NGX_ERROR;
        }

        p += 4;
        n -= 4;

        اگر (n == 6 && ngx_strncmp(p, "verify", 6) == 0) {

            tlv_ssl = (ngx_proxy_protocol_tlv_ssl_t *) ssl.ڈیٹا;
            verify = ngx_proxy_protocol_parse_uint32(tlv_ssl->verify);

            قدر->ڈیٹا = ngx_pnalloc(c->pool, NGX_INT32_LEN);
            اگر (قدر->ڈیٹا == تہی) {
                واپس NGX_ERROR;
            }

            قدر->len = ngx_sprintf(قدر->ڈیٹا, "%uD", verify)
                         - قدر->ڈیٹا;
            واپس NGX_OK;
        }

        ssl.ڈیٹا += حجم(ngx_proxy_protocol_tlv_ssl_t);
        ssl.len -= حجم(ngx_proxy_protocol_tlv_ssl_t);

        te = ngx_proxy_protocol_tlv_ssl_entries;
        tlvs = &ssl;
    }

    اگر (n >= 2 && p[0] == '0' && p[1] == 'x') {

        type = ngx_hextoi(p + 2, n - 2);
        اگر (type == NGX_ERROR) {
            ngx_log_error(NGX_LOG_ERR, c->لوگارتھم, 0,
                          "invalid PROXY protocol TLV \"%V\"", name);
            واپس NGX_ERROR;
        }

        واپس ngx_proxy_protocol_lookup_tlv(c, tlvs, type, قدر);
    }

    برائے ( /* void */ ; te->type; te++) {
        اگر (te->name.len == n && ngx_strncmp(te->name.ڈیٹا, p, n) == 0) {
            واپس ngx_proxy_protocol_lookup_tlv(c, tlvs, te->type, قدر);
        }
    }

    ngx_log_error(NGX_LOG_ERR, c->لوگارتھم, 0,
                  "unknown PROXY protocol TLV \"%V\"", name);

    واپس NGX_DECLINED;
}


ساکن ngx_int_t
ngx_proxy_protocol_lookup_tlv(ngx_connection_t *c, ngx_str_t *tlvs,
    ngx_uint_t type, ngx_str_t *قدر)
{
    u_char                    *p;
    سائز_t                     n, len;
    ngx_proxy_protocol_tlv_t  *tlv;

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->لوگارتھم, 0,
                   "PROXY protocol v2 lookup tlv:%02xi", type);

    p = tlvs->ڈیٹا;
    n = tlvs->len;

    جب (n) {
        اگر (n < حجم(ngx_proxy_protocol_tlv_t)) {
            ngx_log_error(NGX_LOG_ERR, c->لوگارتھم, 0, "broken PROXY protocol TLV");
            واپس NGX_ERROR;
        }

        tlv = (ngx_proxy_protocol_tlv_t *) p;
        len = ngx_proxy_protocol_parse_uint16(tlv->len);

        p += حجم(ngx_proxy_protocol_tlv_t);
        n -= حجم(ngx_proxy_protocol_tlv_t);

        اگر (n < len) {
            ngx_log_error(NGX_LOG_ERR, c->لوگارتھم, 0, "broken PROXY protocol TLV");
            واپس NGX_ERROR;
        }

        اگر (tlv->type == type) {
            قدر->ڈیٹا = p;
            قدر->len = len;
            واپس NGX_OK;
        }

        p += len;
        n -= len;
    }

    واپس NGX_DECLINED;
}


u_char *
ngx_proxy_protocol_v2_write(ngx_connection_t *c, u_char *buf, u_char *آخری,
    ngx_array_t *tlvs /* reserved */ )
{
    u_char                          *p;
    uint32_t                         verify;
    ngx_uint_t                       i, client;
    ngx_array_t                      بنیاد_tlvs, ssl_tlvs;
    ngx_proxy_protocol_tlv_قدر_t  *tlv;

    client = 0;
    verify = 0;

    ngx_memzero(&بنیاد_tlvs, حجم(ngx_array_t));
    ngx_memzero(&ssl_tlvs, حجم(ngx_array_t));

#اگر (NGX_OPENSSL)

    اگر (c->ssl != تہی) {
        اگر (ngx_array_init(&بنیاد_tlvs, c->pool, 2,
                           حجم(ngx_proxy_protocol_tlv_قدر_t))
            != NGX_OK)
        {
            واپس تہی;
        }

        اگر (ngx_array_init(&ssl_tlvs, c->pool, 5,
                           حجم(ngx_proxy_protocol_tlv_قدر_t))
            != NGX_OK)
        {
            واپس تہی;
        }

        اگر (ngx_proxy_protocol_v2_eval_ssl(c, &بنیاد_tlvs, &ssl_tlvs, &client,
                                           &verify)
            != NGX_OK)
        {
            واپس تہی;
        }
    }

#اگر_ختم

    p = ngx_proxy_protocol_v2_write_سرخی(c, buf, آخری);
    اگر (p == تہی) {
        واپس تہی;
    }

    tlv = بنیاد_tlvs.elts;

    برائے (i = 0; i < بنیاد_tlvs.nelts; i++) {
        p = ngx_proxy_protocol_v2_write_tlv(c, p, آخری, tlv[i].type,
                                           &tlv[i].قدر);
        اگر (p == تہی) {
            واپس تہی;
        }
    }

    اگر (client) {
        p = ngx_proxy_protocol_v2_write_ssl(c, p, آخری, &ssl_tlvs, client,
                                            verify);
        اگر (p == تہی) {
            واپس تہی;
        }
    }

    ngx_proxy_protocol_v2_سیٹ_len(buf,
                                  p + حجم(ngx_proxy_protocol_tlv_t) + 4);

    واپس ngx_proxy_protocol_v2_write_crc32c(c, buf, p, آخری);
}


#اگر (NGX_OPENSSL)

ساکن ngx_int_t
ngx_proxy_protocol_v2_eval_ssl(ngx_connection_t *c, ngx_array_t *tlvs,
    ngx_array_t *ssl_tlvs, ngx_uint_t *client, uint32_t *verify)
{
    X509                            *cert;
    طویل                             n;
    ngx_int_t                        rc;
    ngx_str_t                        قدر;
    ngx_uint_t                       type;
    ثابت حرف                      *s;
    ngx_proxy_protocol_tlv_قدر_t  *tlv;

    اگر (ngx_proxy_protocol_v2_authority(c, &قدر) == NGX_OK) {
        tlv = ngx_array_ڈالو(tlvs);
        اگر (tlv == تہی) {
            واپس NGX_ERROR;
        }

        tlv->type = NGX_PROXY_PROTOCOL_TLV_AUTHORITY;
        tlv->قدر = قدر;
    }

    اگر (ngx_proxy_protocol_v2_alpn(c, &قدر) == NGX_OK) {
        tlv = ngx_array_ڈالو(tlvs);
        اگر (tlv == تہی) {
            واپس NGX_ERROR;
        }

        tlv->type = NGX_PROXY_PROTOCOL_TLV_ALPN;
        tlv->قدر = قدر;
    }

    برائے (type = NGX_PROXY_PROTOCOL_TLV_SSL_نسخہ;
         type <= NGX_PROXY_PROTOCOL_TLV_SSL_کلید_ALG;
         type++)
    {
        rc = ngx_proxy_protocol_v2_ssl_sub(c, type, &قدر);

        اگر (rc == NGX_ERROR) {
            واپس NGX_ERROR;
        }

        اگر (rc != NGX_OK) {
            جاری;
        }

        tlv = ngx_array_ڈالو(ssl_tlvs);
        اگر (tlv == تہی) {
            واپس NGX_ERROR;
        }

        tlv->type = type;
        tlv->قدر = قدر;
    }

    *client = NGX_PROXY_PROTOCOL_V2_CLIENT_SSL;
    *verify = 1;  /* X509_V_ERR_UNSPECIFIED */

    cert = SSL_get_peer_certificate(c->ssl->connection);

    اگر (cert != تہی) {
        X509_free(cert);

        *client |= NGX_PROXY_PROTOCOL_V2_CLIENT_CERT_SESS;

        اگر (!SSL_session_reused(c->ssl->connection)) {
            *client |= NGX_PROXY_PROTOCOL_V2_CLIENT_CERT_CONN;
        }

        n = SSL_get_verify_نتیجہ(c->ssl->connection);

        اگر (n == X509_V_OK) {

            اگر (ngx_ssl_ocsp_get_حالت(c, &s) != NGX_OK) {
                n = X509_V_ERR_CERT_REVOKED;
            }
        }

        *verify = (uint32_t) n;
    }

    واپس NGX_OK;
}


ساکن ngx_int_t
ngx_proxy_protocol_v2_authority(ngx_connection_t *c, ngx_str_t *برآمد)
{
#اگر_تعریف_شدہ SSL_CTRL_سیٹ_TLSEXT_HOSTNAME

    ثابت حرف  *sni;

    sni = SSL_get_servername(c->ssl->connection, TLSEXT_NAMETYPE_host_name);
    اگر (sni == تہی) {
        واپس NGX_DECLINED;
    }

    برآمد->ڈیٹا = (u_char *) sni;
    برآمد->len = ngx_strlen(sni);
    واپس NGX_OK;

#ورنہ
    واپس NGX_DECLINED;
#اگر_ختم
}


ساکن ngx_int_t
ngx_proxy_protocol_v2_alpn(ngx_connection_t *c, ngx_str_t *برآمد)
{
#اگر_تعریف_شدہ TLSEXT_TYPE_application_layer_protocol_negotiation

    غیرعلامتی صحیح          alpnlen;
    ثابت غیرعلامتی حرف  *alpn;

    SSL_get0_alpn_selected(c->ssl->connection, &alpn, &alpnlen);
    اگر (alpn == تہی || alpnlen == 0) {
        واپس NGX_DECLINED;
    }

    برآمد->ڈیٹا = (u_char *) alpn;
    برآمد->len = alpnlen;
    واپس NGX_OK;

#ورنہ
    واپس NGX_DECLINED;
#اگر_ختم
}


ساکن ngx_int_t
ngx_proxy_protocol_v2_ssl_sub(ngx_connection_t *c, ngx_uint_t type,
    ngx_str_t *برآمد)
{
    صحیح               i, len;
    X509             *cert;
    u_char           *alg, *s;
    EVP_PKEY         *pkey;
#اگر (OPENSSL_نسخہ_نمبر >= 0x40000000L)
    ثابت
#اگر_ختم
    X509_NAME        *subject;
#اگر (OPENSSL_نسخہ_نمبر >= 0x40000000L)
    ثابت
#اگر_ختم
    X509_NAME_اندراج  *اندراج_2;

    انتخاب (type) {

    حالت NGX_PROXY_PROTOCOL_TLV_SSL_نسخہ:
        برآمد->ڈیٹا = (u_char *) SSL_get_نسخہ(c->ssl->connection);
        برآمد->len = ngx_strlen(برآمد->ڈیٹا);
        واپس NGX_OK;

    حالت NGX_PROXY_PROTOCOL_TLV_SSL_CIPHER:
        برآمد->ڈیٹا = (u_char *) SSL_get_cipher_name(c->ssl->connection);
        برآمد->len = ngx_strlen(برآمد->ڈیٹا);
        واپس NGX_OK;

    حالت NGX_PROXY_PROTOCOL_TLV_SSL_CN:
        cert = SSL_get_peer_certificate(c->ssl->connection);
        اگر (cert == تہی) {
            واپس NGX_DECLINED;
        }

        subject = X509_get_subject_name(cert);
        اگر (subject == تہی) {
            X509_free(cert);
            واپس NGX_ERROR;
        }

        i = X509_NAME_get_اشاریہ_by_NID(subject, NID_commonName, -1);
        اگر (i < 0) {
            X509_free(cert);
            واپس NGX_DECLINED;
        }

        اندراج_2 = X509_NAME_get_اندراج(subject, i);

        len = ASN1_STRING_مقصد_UTF8(&s, X509_NAME_اندراج_get_ڈیٹا(اندراج_2));
        اگر (len < 0) {
            ngx_ssl_error(NGX_LOG_ALERT, c->لوگارتھم, 0,
                          "ASN1_STRING_to_UTF8() failed");
            X509_free(cert);
            واپس NGX_ERROR;
        }

        برآمد->len = len;
        برآمد->ڈیٹا = ngx_pnalloc(c->pool, len);
        اگر (برآمد->ڈیٹا == تہی) {
            OPENSSL_free(s);
            X509_free(cert);
            واپس NGX_ERROR;
        }

        ngx_memcpy(برآمد->ڈیٹا, s, len);

        OPENSSL_free(s);
        X509_free(cert);
        واپس NGX_OK;

    حالت NGX_PROXY_PROTOCOL_TLV_SSL_SIG_ALG:
        cert = SSL_get_certificate(c->ssl->connection);

        برآمد->ڈیٹا = (u_char *) OBJ_nid2sn(X509_get_signature_nid(cert));
        برآمد->len = ngx_strlen(برآمد->ڈیٹا);
        واپس NGX_OK;

    حالت NGX_PROXY_PROTOCOL_TLV_SSL_کلید_ALG:
        cert = SSL_get_certificate(c->ssl->connection);

        pkey = X509_get_pubkey(cert);
        اگر (pkey == تہی) {
            واپس NGX_ERROR;
        }

        انتخاب (EVP_PKEY_بنیاد_id(pkey)) {

        حالت EVP_PKEY_RSA:
#اگر_تعریف_شدہ EVP_PKEY_RSA_PSS
        حالت EVP_PKEY_RSA_PSS:
#اگر_ختم
            alg = (u_char *) "RSA";
            توڑو;

        حالت EVP_PKEY_EC:
            alg = (u_char *) "EC";
            توڑو;

        حالت EVP_PKEY_DSA:
            alg = (u_char *) "DSA";
            توڑو;

        طےشدہ:
            EVP_PKEY_free(pkey);
            واپس NGX_DECLINED;
        }

        برآمد->len = حجم("RSA16384") - 1;
        برآمد->ڈیٹا = ngx_pnalloc(c->pool, برآمد->len);
        اگر (برآمد->ڈیٹا == تہی) {
            EVP_PKEY_free(pkey);
            واپس NGX_ERROR;
        }

        برآمد->len = ngx_snprintf(برآمد->ڈیٹا, برآمد->len, "%s%d",
                                alg, EVP_PKEY_بٹ(pkey))
                   - برآمد->ڈیٹا;

        EVP_PKEY_free(pkey);

        واپس NGX_OK;
    }

    واپس NGX_DECLINED;
}

#اگر_ختم


ساکن u_char *
ngx_proxy_protocol_v2_write_سرخی(ngx_connection_t *c, u_char *buf,
    u_char *آخری)
{
    u_char                            *p, src_af, dst_af,
                                       family, command, transport;
    ngx_uint_t                         src_fam, dst_fam;
    ngx_proxy_protocol_سرخی_t       *سرخی;
    ngx_proxy_protocol_inet_addrs_t   *اندر;
#اگر (NGX_HAVE_INET6)
    ngx_uint_t                         پورٹ, lport;
    ساخت in6_addr                   *src6, *dst6;
    ngx_proxy_protocol_inet6_addrs_t  *in6;
#اگر_ختم

    اگر (آخری - buf < NGX_PROXY_PROTOCOL_V2_MAX_سرخی) {
        ngx_log_error(NGX_LOG_ALERT, c->لوگارتھم, 0,
                      "too small buffer for PROXY protocol v2");
        واپس تہی;
    }

    اگر (ngx_connection_local_sockaddr(c, تہی, 0) != NGX_OK) {
        واپس تہی;
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

    اگر (src_af == NGX_PROXY_PROTOCOL_AF_UNSPEC
        || dst_af == NGX_PROXY_PROTOCOL_AF_UNSPEC)
    {
        command = NGX_PROXY_PROTOCOL_CMD_LOCAL;
        family = NGX_PROXY_PROTOCOL_AF_UNSPEC;
        transport = NGX_PROXY_PROTOCOL_TYPE_UNSPEC;

    } ورنہ {
        command = NGX_PROXY_PROTOCOL_CMD_PROXY;
        family = ngx_max(src_af, dst_af);

        انتخاب (c->type) {
        حالت SOCK_دھارا:
            transport = NGX_PROXY_PROTOCOL_TYPE_دھارا;
            توڑو;
        حالت SOCK_DGRAM:
            transport = NGX_PROXY_PROTOCOL_TYPE_DGRAM;
            توڑو;
        طےشدہ:
            transport = NGX_PROXY_PROTOCOL_TYPE_UNSPEC;
            توڑو;
        }
    }

    سرخی->نسخہ_command = 0x20 | command;
    سرخی->family_transport = (family << 4) | transport;

    انتخاب (family) {

    حالت NGX_PROXY_PROTOCOL_AF_INET:

        اندر = (ngx_proxy_protocol_inet_addrs_t *) p;

        ngx_proxy_protocol_v2_write_ipv4(c->sockaddr,
                                         اندر->src_addr, اندر->src_پورٹ);
        ngx_proxy_protocol_v2_write_ipv4(c->local_sockaddr,
                                         اندر->dst_addr, اندر->dst_پورٹ);

        p += حجم(ngx_proxy_protocol_inet_addrs_t);

        توڑو;

#اگر (NGX_HAVE_INET6)

    حالت NGX_PROXY_PROTOCOL_AF_INET6:

        اگر (src_fam == AF_INET6 && dst_fam == AF_INET6) {
            src6 = &((ساخت sockaddr_in6 *) c->sockaddr)->sin6_addr;
            dst6 = &((ساخت sockaddr_in6 *) c->local_sockaddr)->sin6_addr;

            اگر (IN6_IS_ADDR_V4MAPPED(src6) && IN6_IS_ADDR_V4MAPPED(dst6)) {

                /* both v4-mapped: demote to AF_INET */

                سرخی->family_transport = (NGX_PROXY_PROTOCOL_AF_INET << 4)
                                           | transport;

                اندر = (ngx_proxy_protocol_inet_addrs_t *) p;

                ngx_memcpy(اندر->src_addr, src6->s6_addr + 12, 4);
                ngx_memcpy(اندر->dst_addr, dst6->s6_addr + 12, 4);

                پورٹ  = ngx_inet_get_پورٹ(c->sockaddr);
                lport = ngx_inet_get_پورٹ(c->local_sockaddr);

                اندر->src_پورٹ[0] = (u_char) (پورٹ >> 8);
                اندر->src_پورٹ[1] = (u_char)  پورٹ;
                اندر->dst_پورٹ[0] = (u_char) (lport >> 8);
                اندر->dst_پورٹ[1] = (u_char)  lport;

                p += حجم(ngx_proxy_protocol_inet_addrs_t);
                توڑو;
            }
        }

        in6 = (ngx_proxy_protocol_inet6_addrs_t *) p;

        ngx_proxy_protocol_v2_write_ipv6(c->sockaddr,
                                         in6->src_addr, in6->src_پورٹ);
        ngx_proxy_protocol_v2_write_ipv6(c->local_sockaddr,
                                         in6->dst_addr, in6->dst_پورٹ);

        p += حجم(ngx_proxy_protocol_inet6_addrs_t);

        توڑو;

#اگر_ختم

    طےشدہ:  /* NGX_PROXY_PROTOCOL_AF_UNSPEC */
        توڑو;
    }

    واپس p;
}


ساکن u_char
ngx_proxy_protocol_v2_family(ngx_uint_t family)
{
    انتخاب (family) {

    حالت AF_INET:
        واپس NGX_PROXY_PROTOCOL_AF_INET;

#اگر (NGX_HAVE_INET6)
    حالت AF_INET6:
        واپس NGX_PROXY_PROTOCOL_AF_INET6;
#اگر_ختم

    طےشدہ:
        واپس NGX_PROXY_PROTOCOL_AF_UNSPEC;
    }
}


ساکن خالی
ngx_proxy_protocol_v2_write_ipv4(ساخت sockaddr *sa, u_char *addr,
    u_char *پورٹ)
{
    ساخت sockaddr_اندر  *جیب;

    جیب = (ساخت sockaddr_اندر *) sa;
    ngx_memcpy(addr, &جیب->sin_addr, 4);
    ngx_memcpy(پورٹ, &جیب->sin_پورٹ, 2);
}


#اگر (NGX_HAVE_INET6)

ساکن خالی
ngx_proxy_protocol_v2_write_ipv6(ساخت sockaddr *sa, u_char *addr,
    u_char *پورٹ)
{
    ساخت sockaddr_اندر   *جیب;
    ساخت sockaddr_in6  *sin6;

    /* IPv4-mapped prefix: 80 zero bits + 16 one bits */
    ساکن ثابت u_char  mapped[]   = { 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0xff, 0xff };

    اگر (sa->sa_family == AF_INET6) {
        sin6 = (ساخت sockaddr_in6 *) sa;
        ngx_memcpy(addr, &sin6->sin6_addr, 16);
        ngx_memcpy(پورٹ, &sin6->sin6_پورٹ, 2);

    } ورنہ {
        /* promote AF_INET to ::ffff:a.b.c.d */
        جیب = (ساخت sockaddr_اندر *) sa;
        ngx_memcpy(addr, mapped, حجم(mapped));
        ngx_memcpy(addr + حجم(mapped), &جیب->sin_addr, 4);
        ngx_memcpy(پورٹ, &جیب->sin_پورٹ, 2);
    }
}

#اگر_ختم


ساکن u_char *
ngx_proxy_protocol_v2_write_tlv(ngx_connection_t *c, u_char *p, u_char *آخری,
    ngx_uint_t type, ngx_str_t *قدر)
{
    ngx_proxy_protocol_tlv_t  *tlv;

    اگر ((سائز_t) (آخری - p) < حجم(ngx_proxy_protocol_tlv_t) + قدر->len) {
        ngx_log_error(NGX_LOG_ALERT, c->لوگارتھم, 0,
                      "too small buffer for PROXY protocol v2 TLV");
        واپس تہی;
    }

    tlv = (ngx_proxy_protocol_tlv_t *) p;
    tlv->type = (u_char) type;
    tlv->len[0] = (u_char) (قدر->len >> 8);
    tlv->len[1] = (u_char)  قدر->len;

    p += حجم(ngx_proxy_protocol_tlv_t);

    واپس ngx_cpymem(p, قدر->ڈیٹا, قدر->len);
}


ساکن u_char *
ngx_proxy_protocol_v2_write_ssl(ngx_connection_t *c, u_char *p, u_char *آخری,
    ngx_array_t *ssl_tlvs, ngx_uint_t client, uint32_t verify)
{
    u_char                          *آغاز;
    uint16_t                         len;
    ngx_uint_t                       i;
    ngx_proxy_protocol_tlv_t        *tlv;
    ngx_proxy_protocol_tlv_ssl_t    *tlv_ssl;
    ngx_proxy_protocol_tlv_قدر_t  *sub;

    اگر ((سائز_t) (آخری - p) < حجم(ngx_proxy_protocol_tlv_t)
                              + حجم(ngx_proxy_protocol_tlv_ssl_t))
    {
        ngx_log_error(NGX_LOG_ALERT, c->لوگارتھم, 0,
                      "too small buffer for PROXY protocol v2 SSL TLV");
        واپس تہی;
    }

    آغاز = p;
    p += حجم(ngx_proxy_protocol_tlv_t);

    tlv_ssl = (ngx_proxy_protocol_tlv_ssl_t *) p;
    tlv_ssl->client = (u_char) client;
    tlv_ssl->verify[0] = (u_char) (verify >> 24);
    tlv_ssl->verify[1] = (u_char) (verify >> 16);
    tlv_ssl->verify[2] = (u_char) (verify >> 8);
    tlv_ssl->verify[3] = (u_char)  verify;

    p += حجم(ngx_proxy_protocol_tlv_ssl_t);

    sub = ssl_tlvs->elts;

    برائے (i = 0; i < ssl_tlvs->nelts; i++) {
        p = ngx_proxy_protocol_v2_write_tlv(c, p, آخری, sub[i].type,
                                            &sub[i].قدر);
        اگر (p == تہی) {
            واپس تہی;
        }
    }

    len = (uint16_t) (p - آغاز - حجم(ngx_proxy_protocol_tlv_t));

    tlv = (ngx_proxy_protocol_tlv_t *) آغاز;
    tlv->type = NGX_PROXY_PROTOCOL_TLV_SSL;
    tlv->len[0] = (u_char) (len >> 8);
    tlv->len[1] = (u_char)  len;

    واپس p;
}


ساکن خالی
ngx_proxy_protocol_v2_سیٹ_len(u_char *buf, u_char *p)
{
    uint16_t                      len;
    ngx_proxy_protocol_سرخی_t  *سرخی;

    len = (uint16_t) (p - buf - حجم(ngx_proxy_protocol_سرخی_t));

    سرخی = (ngx_proxy_protocol_سرخی_t *) buf;
    سرخی->len[0] = (u_char) (len >> 8);
    سرخی->len[1] = (u_char) len;
}


ساکن u_char *
ngx_proxy_protocol_v2_write_crc32c(ngx_connection_t *c, u_char *buf,
    u_char *p, u_char *آخری)
{
    u_char                    *قدر;
    uint32_t                   چکری_جانچ;
    ngx_proxy_protocol_tlv_t  *tlv;

    اگر ((سائز_t) (آخری - p) < حجم(ngx_proxy_protocol_tlv_t) + 4) {
        ngx_log_error(NGX_LOG_ALERT, c->لوگارتھم, 0,
                      "too small buffer for PROXY protocol v2 CRC32c TLV");
        واپس تہی;
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

    واپس p;
}
