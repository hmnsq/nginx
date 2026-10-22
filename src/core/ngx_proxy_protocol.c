
/*
 * Copyright (C) Roman Arutyunyan
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


#تعریف_کن NGX_PROXY_PROTOCOL_V2_MAX_سرآیند    52

#تعریف_کن NGX_PROXY_PROTOCOL_CMD_LOCAL        0
#تعریف_کن NGX_PROXY_PROTOCOL_CMD_PROXY        1

#تعریف_کن NGX_PROXY_PROTOCOL_AF_UNSPEC        0
#تعریف_کن NGX_PROXY_PROTOCOL_AF_INET          1
#تعریف_کن NGX_PROXY_PROTOCOL_AF_INET6         2

#تعریف_کن NGX_PROXY_PROTOCOL_TYPE_UNSPEC      0
#تعریف_کن NGX_PROXY_PROTOCOL_TYPE_جریان      1
#تعریف_کن NGX_PROXY_PROTOCOL_TYPE_DGRAM       2

#تعریف_کن NGX_PROXY_PROTOCOL_TLV_ALPN         0x01
#تعریف_کن NGX_PROXY_PROTOCOL_TLV_AUTHORITY    0x02
#تعریف_کن NGX_PROXY_PROTOCOL_TLV_بررسی_چرخهای_32_C       0x03
#تعریف_کن NGX_PROXY_PROTOCOL_TLV_UNIQUE_ID    0x05
#تعریف_کن NGX_PROXY_PROTOCOL_TLV_SSL          0x20
#تعریف_کن NGX_PROXY_PROTOCOL_TLV_SSL_نسخه  0x21
#تعریف_کن NGX_PROXY_PROTOCOL_TLV_SSL_CN       0x22
#تعریف_کن NGX_PROXY_PROTOCOL_TLV_SSL_CIPHER   0x23
#تعریف_کن NGX_PROXY_PROTOCOL_TLV_SSL_SIG_ALG  0x24
#تعریف_کن NGX_PROXY_PROTOCOL_TLV_SSL_کلید_ALG  0x25
#تعریف_کن NGX_PROXY_PROTOCOL_TLV_NETNS        0x30

#تعریف_کن NGX_PROXY_PROTOCOL_V2_CLIENT_SSL          0x01
#تعریف_کن NGX_PROXY_PROTOCOL_V2_CLIENT_CERT_CONN    0x02
#تعریف_کن NGX_PROXY_PROTOCOL_V2_CLIENT_CERT_SESS    0x04


#تعریف_کن ngx_proxy_protocol_parse_uint16(p)                                    \
    ( ((uint16_t) (p)[0] << 8)                                                \
    + (           (p)[1]) )

#تعریف_کن ngx_proxy_protocol_parse_uint32(p)                                    \
    ( ((uint32_t) (p)[0] << 24)                                               \
    + (           (p)[1] << 16)                                               \
    + (           (p)[2] << 8)                                                \
    + (           (p)[3]) )


تعريفنوع بنية {
    u_char                                  signature[12];
    u_char                                  نسخه_command;
    u_char                                  family_transport;
    u_char                                  len[2];
} ngx_proxy_protocol_سرآیند_t;


تعريفنوع بنية {
    u_char                                  src_addr[4];
    u_char                                  dst_addr[4];
    u_char                                  src_پورت[2];
    u_char                                  dst_پورت[2];
} ngx_proxy_protocol_inet_addrs_t;


تعريفنوع بنية {
    u_char                                  src_addr[16];
    u_char                                  dst_addr[16];
    u_char                                  src_پورت[2];
    u_char                                  dst_پورت[2];
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
} ngx_proxy_protocol_tlv_مدخل_t;


تعريفنوع بنية {
    ngx_uint_t                              type;
    ngx_str_t                               مقدار;
} ngx_proxy_protocol_tlv_مقدار_t;


ساكن u_char *ngx_proxy_protocol_read_addr(ngx_connection_t *c, u_char *p,
    u_char *آخرین, ngx_str_t *addr);
ساكن u_char *ngx_proxy_protocol_read_پورت(u_char *p, u_char *آخرین,
    درون_پورت_t *پورت, u_char sep);
ساكن u_char *ngx_proxy_protocol_v2_read(ngx_connection_t *c, u_char *buf,
    u_char *آخرین);
ساكن ngx_int_t ngx_proxy_protocol_lookup_tlv(ngx_connection_t *c,
    ngx_str_t *tlvs, ngx_uint_t type, ngx_str_t *مقدار);
#إذا (NGX_OPENSSL)
ساكن ngx_int_t ngx_proxy_protocol_v2_eval_ssl(ngx_connection_t *c,
    ngx_array_t *tlvs, ngx_array_t *ssl_tlvs, ngx_uint_t *client,
    uint32_t *verify);
ساكن ngx_int_t ngx_proxy_protocol_v2_authority(ngx_connection_t *c,
    ngx_str_t *بیرون);
ساكن ngx_int_t ngx_proxy_protocol_v2_alpn(ngx_connection_t *c,
    ngx_str_t *بیرون);
ساكن ngx_int_t ngx_proxy_protocol_v2_ssl_sub(ngx_connection_t *c,
    ngx_uint_t type, ngx_str_t *بیرون);
#پایان_اگر
ساكن u_char *ngx_proxy_protocol_v2_write_سرآیند(ngx_connection_t *c,
    u_char *buf, u_char *آخرین);
ساكن u_char ngx_proxy_protocol_v2_family(ngx_uint_t family);
ساكن فراغ ngx_proxy_protocol_v2_write_ipv4(بنية sockaddr *sa, u_char *addr,
    u_char *پورت);
ساكن فراغ ngx_proxy_protocol_v2_write_ipv6(بنية sockaddr *sa, u_char *addr,
    u_char *پورت);
ساكن u_char *ngx_proxy_protocol_v2_write_tlv(ngx_connection_t *c, u_char *p,
    u_char *آخرین, ngx_uint_t type, ngx_str_t *مقدار);
ساكن u_char *ngx_proxy_protocol_v2_write_ssl(ngx_connection_t *c, u_char *p,
    u_char *آخرین, ngx_array_t *ssl_tlvs, ngx_uint_t client, uint32_t verify);
ساكن فراغ ngx_proxy_protocol_v2_مجموعه_len(u_char *buf, u_char *p);
ساكن u_char *ngx_proxy_protocol_v2_write_crc32c(ngx_connection_t *c,
    u_char *buf, u_char *p, u_char *آخرین);


ساكن ngx_proxy_protocol_tlv_مدخل_t  ngx_proxy_protocol_tlv_entries[] = {
    { ngx_string("alpn"),       NGX_PROXY_PROTOCOL_TLV_ALPN },
    { ngx_string("authority"),  NGX_PROXY_PROTOCOL_TLV_AUTHORITY },
    { ngx_string("unique_id"),  NGX_PROXY_PROTOCOL_TLV_UNIQUE_ID },
    { ngx_string("ssl"),        NGX_PROXY_PROTOCOL_TLV_SSL },
    { ngx_string("netns"),      NGX_PROXY_PROTOCOL_TLV_NETNS },
    { ngx_null_string,          0x00 }
};


ساكن ngx_proxy_protocol_tlv_مدخل_t  ngx_proxy_protocol_tlv_ssl_entries[] = {
    { ngx_string("version"),    NGX_PROXY_PROTOCOL_TLV_SSL_نسخه },
    { ngx_string("cn"),         NGX_PROXY_PROTOCOL_TLV_SSL_CN },
    { ngx_string("cipher"),     NGX_PROXY_PROTOCOL_TLV_SSL_CIPHER },
    { ngx_string("sig_alg"),    NGX_PROXY_PROTOCOL_TLV_SSL_SIG_ALG },
    { ngx_string("key_alg"),    NGX_PROXY_PROTOCOL_TLV_SSL_کلید_ALG },
    { ngx_null_string,          0x00 }
};


ساكن ثابت u_char  ngx_proxy_protocol_signature[] = "\r\n\r\n\0\r\nQUIT\n";


u_char *
ngx_proxy_protocol_read(ngx_connection_t *c, u_char *buf, u_char *آخرین)
{
    اندازه_t                 len;
    u_char                *p;
    ngx_proxy_protocol_t  *pp;

    p = buf;
    len = آخرین - buf;

    إذا (len >= حجم(ngx_proxy_protocol_سرآیند_t)
        && ngx_memcmp(p, ngx_proxy_protocol_signature,
                      حجم(ngx_proxy_protocol_signature) - 1)
           == 0)
    {
        أرجع ngx_proxy_protocol_v2_read(c, buf, آخرین);
    }

    إذا (len < 8 || ngx_strncmp(p, "PROXY ", 6) != 0) {
        اذهب invalid;
    }

    p += 6;
    len -= 6;

    إذا (len >= 7 && ngx_strncmp(p, "UNKNOWN", 7) == 0) {
        ngx_log_debug0(NGX_LOG_DEBUG_CORE, c->لگاریتم, 0,
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

    p = ngx_proxy_protocol_read_addr(c, p, آخرین, &pp->src_addr);
    إذا (p == عدم) {
        اذهب invalid;
    }

    p = ngx_proxy_protocol_read_addr(c, p, آخرین, &pp->dst_addr);
    إذا (p == عدم) {
        اذهب invalid;
    }

    p = ngx_proxy_protocol_read_پورت(p, آخرین, &pp->src_پورت, ' ');
    إذا (p == عدم) {
        اذهب invalid;
    }

    p = ngx_proxy_protocol_read_پورت(p, آخرین, &pp->dst_پورت, CR);
    إذا (p == عدم) {
        اذهب invalid;
    }

    إذا (p == آخرین) {
        اذهب invalid;
    }

    إذا (*p++ != LF) {
        اذهب invalid;
    }

    ngx_log_debug4(NGX_LOG_DEBUG_CORE, c->لگاریتم, 0,
                   "PROXY protocol src: %V %d, dst: %V %d",
                   &pp->src_addr, pp->src_پورت, &pp->dst_addr, pp->dst_پورت);

    c->proxy_protocol = pp;

    أرجع p;

skip:

    لكل ( /* void */ ; p < آخرین - 1; p++) {
        إذا (p[0] == CR && p[1] == LF) {
            أرجع p + 2;
        }
    }

invalid:

    لكل (p = buf; p < آخرین; p++) {
        إذا (*p == CR || *p == LF) {
            اكسر;
        }
    }

    ngx_log_error(NGX_LOG_ERR, c->لگاریتم, 0,
                  "broken header: \"%*s\"", (اندازه_t) (p - buf), buf);

    أرجع عدم;
}


ساكن u_char *
ngx_proxy_protocol_read_addr(ngx_connection_t *c, u_char *p, u_char *آخرین,
    ngx_str_t *addr)
{
    اندازه_t  len;
    u_char  ch, *pos;

    pos = p;

    لكل ( ;; ) {
        إذا (p == آخرین) {
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

    addr->داده = ngx_pnalloc(c->pool, len);
    إذا (addr->داده == عدم) {
        أرجع عدم;
    }

    ngx_memcpy(addr->داده, pos, len);
    addr->len = len;

    أرجع p;
}


ساكن u_char *
ngx_proxy_protocol_read_پورت(u_char *p, u_char *آخرین, درون_پورت_t *پورت,
    u_char sep)
{
    اندازه_t      len;
    u_char     *pos;
    ngx_int_t   n;

    pos = p;

    لكل ( ;; ) {
        إذا (p == آخرین) {
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

    *پورت = (درون_پورت_t) n;

    أرجع p;
}


u_char *
ngx_proxy_protocol_write(ngx_connection_t *c, u_char *buf, u_char *آخرین)
{
    ngx_uint_t  پورت, lport;

    إذا (آخرین - buf < NGX_PROXY_PROTOCOL_V1_MAX_سرآیند) {
        ngx_log_error(NGX_LOG_ALERT, c->لگاریتم, 0,
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
#پایان_اگر

    افتراضي:
        أرجع ngx_cpymem(buf, "PROXY UNKNOWN" CRLF,
                          حجم("PROXY UNKNOWN" CRLF) - 1);
    }

    buf += ngx_sock_ntop(c->sockaddr, c->socklen, buf, آخرین - buf, 0);

    *buf++ = ' ';

    buf += ngx_sock_ntop(c->local_sockaddr, c->local_socklen, buf, آخرین - buf,
                         0);

    پورت = ngx_inet_get_پورت(c->sockaddr);
    lport = ngx_inet_get_پورت(c->local_sockaddr);

    أرجع ngx_slprintf(buf, آخرین, " %ui %ui" CRLF, پورت, lport);
}


ساكن u_char *
ngx_proxy_protocol_v2_read(ngx_connection_t *c, u_char *buf, u_char *آخرین)
{
    u_char                             *انتها;
    اندازه_t                              len;
    socklen_t                           socklen;
    ngx_uint_t                          نسخه, command, family, transport;
    ngx_sockaddr_t                      src_sockaddr, dst_sockaddr;
    ngx_proxy_protocol_t               *pp;
    ngx_proxy_protocol_سرآیند_t        *سرآیند;
    ngx_proxy_protocol_inet_addrs_t    *درون;
#إذا (NGX_HAVE_INET6)
    ngx_proxy_protocol_inet6_addrs_t   *in6;
#پایان_اگر

    سرآیند = (ngx_proxy_protocol_سرآیند_t *) buf;

    buf += حجم(ngx_proxy_protocol_سرآیند_t);

    نسخه = سرآیند->نسخه_command >> 4;

    إذا (نسخه != 2) {
        ngx_log_error(NGX_LOG_ERR, c->لگاریتم, 0,
                      "unknown PROXY protocol version: %ui", نسخه);
        أرجع عدم;
    }

    len = ngx_proxy_protocol_parse_uint16(سرآیند->len);

    إذا ((اندازه_t) (آخرین - buf) < len) {
        ngx_log_error(NGX_LOG_ERR, c->لگاریتم, 0, "header is too large");
        أرجع عدم;
    }

    انتها = buf + len;

    command = سرآیند->نسخه_command & 0x0f;

    /* only PROXY is supported */
    إذا (command != NGX_PROXY_PROTOCOL_CMD_PROXY) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->لگاریتم, 0,
                       "PROXY protocol v2 unsupported command %ui", command);
        أرجع انتها;
    }

    transport = سرآیند->family_transport & 0x0f;

    /* only STREAM is supported */
    إذا (transport != NGX_PROXY_PROTOCOL_TYPE_جریان) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->لگاریتم, 0,
                       "PROXY protocol v2 unsupported transport %ui",
                       transport);
        أرجع انتها;
    }

    pp = ngx_pcalloc(c->pool, حجم(ngx_proxy_protocol_t));
    إذا (pp == عدم) {
        أرجع عدم;
    }

    family = سرآیند->family_transport >> 4;

    بدل (family) {

    حالة NGX_PROXY_PROTOCOL_AF_INET:

        إذا ((اندازه_t) (انتها - buf) < حجم(ngx_proxy_protocol_inet_addrs_t)) {
            أرجع عدم;
        }

        درون = (ngx_proxy_protocol_inet_addrs_t *) buf;

        src_sockaddr.sockaddr_درون.sin_family = AF_INET;
        src_sockaddr.sockaddr_درون.sin_پورت = 0;
        ngx_memcpy(&src_sockaddr.sockaddr_درون.sin_addr, درون->src_addr, 4);

        dst_sockaddr.sockaddr_درون.sin_family = AF_INET;
        dst_sockaddr.sockaddr_درون.sin_پورت = 0;
        ngx_memcpy(&dst_sockaddr.sockaddr_درون.sin_addr, درون->dst_addr, 4);

        pp->src_پورت = ngx_proxy_protocol_parse_uint16(درون->src_پورت);
        pp->dst_پورت = ngx_proxy_protocol_parse_uint16(درون->dst_پورت);

        socklen = حجم(بنية sockaddr_درون);

        buf += حجم(ngx_proxy_protocol_inet_addrs_t);

        اكسر;

#إذا (NGX_HAVE_INET6)

    حالة NGX_PROXY_PROTOCOL_AF_INET6:

        إذا ((اندازه_t) (انتها - buf) < حجم(ngx_proxy_protocol_inet6_addrs_t)) {
            أرجع عدم;
        }

        in6 = (ngx_proxy_protocol_inet6_addrs_t *) buf;

        src_sockaddr.sockaddr_in6.sin6_family = AF_INET6;
        src_sockaddr.sockaddr_in6.sin6_پورت = 0;
        ngx_memcpy(&src_sockaddr.sockaddr_in6.sin6_addr, in6->src_addr, 16);

        dst_sockaddr.sockaddr_in6.sin6_family = AF_INET6;
        dst_sockaddr.sockaddr_in6.sin6_پورت = 0;
        ngx_memcpy(&dst_sockaddr.sockaddr_in6.sin6_addr, in6->dst_addr, 16);

        pp->src_پورت = ngx_proxy_protocol_parse_uint16(in6->src_پورت);
        pp->dst_پورت = ngx_proxy_protocol_parse_uint16(in6->dst_پورت);

        socklen = حجم(بنية sockaddr_in6);

        buf += حجم(ngx_proxy_protocol_inet6_addrs_t);

        اكسر;

#پایان_اگر

    افتراضي:
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->لگاریتم, 0,
                       "PROXY protocol v2 unsupported address family %ui",
                       family);
        أرجع انتها;
    }

    pp->src_addr.داده = ngx_pnalloc(c->pool, NGX_SOCKADDR_STRLEN);
    إذا (pp->src_addr.داده == عدم) {
        أرجع عدم;
    }

    pp->src_addr.len = ngx_sock_ntop(&src_sockaddr.sockaddr, socklen,
                                     pp->src_addr.داده, NGX_SOCKADDR_STRLEN, 0);

    pp->dst_addr.داده = ngx_pnalloc(c->pool, NGX_SOCKADDR_STRLEN);
    إذا (pp->dst_addr.داده == عدم) {
        أرجع عدم;
    }

    pp->dst_addr.len = ngx_sock_ntop(&dst_sockaddr.sockaddr, socklen,
                                     pp->dst_addr.داده, NGX_SOCKADDR_STRLEN, 0);

    ngx_log_debug4(NGX_LOG_DEBUG_CORE, c->لگاریتم, 0,
                   "PROXY protocol v2 src: %V %d, dst: %V %d",
                   &pp->src_addr, pp->src_پورت, &pp->dst_addr, pp->dst_پورت);

    إذا (buf < انتها) {
        pp->tlvs.داده = ngx_pnalloc(c->pool, انتها - buf);
        إذا (pp->tlvs.داده == عدم) {
            أرجع عدم;
        }

        ngx_memcpy(pp->tlvs.داده, buf, انتها - buf);
        pp->tlvs.len = انتها - buf;
    }

    c->proxy_protocol = pp;

    أرجع انتها;
}


ngx_int_t
ngx_proxy_protocol_get_tlv(ngx_connection_t *c, ngx_str_t *name,
    ngx_str_t *مقدار)
{
    u_char                          *p;
    اندازه_t                           n;
    uint32_t                         verify;
    ngx_str_t                        ssl, *tlvs;
    ngx_int_t                        rc, type;
    ngx_proxy_protocol_tlv_ssl_t    *tlv_ssl;
    ngx_proxy_protocol_tlv_مدخل_t  *te;

    إذا (c->proxy_protocol == عدم) {
        أرجع NGX_DECLINED;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->لگاریتم, 0,
                   "PROXY protocol v2 get tlv \"%V\"", name);

    te = ngx_proxy_protocol_tlv_entries;
    tlvs = &c->proxy_protocol->tlvs;

    p = name->داده;
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

            tlv_ssl = (ngx_proxy_protocol_tlv_ssl_t *) ssl.داده;
            verify = ngx_proxy_protocol_parse_uint32(tlv_ssl->verify);

            مقدار->داده = ngx_pnalloc(c->pool, NGX_INT32_LEN);
            إذا (مقدار->داده == عدم) {
                أرجع NGX_ERROR;
            }

            مقدار->len = ngx_sprintf(مقدار->داده, "%uD", verify)
                         - مقدار->داده;
            أرجع NGX_OK;
        }

        ssl.داده += حجم(ngx_proxy_protocol_tlv_ssl_t);
        ssl.len -= حجم(ngx_proxy_protocol_tlv_ssl_t);

        te = ngx_proxy_protocol_tlv_ssl_entries;
        tlvs = &ssl;
    }

    إذا (n >= 2 && p[0] == '0' && p[1] == 'x') {

        type = ngx_hextoi(p + 2, n - 2);
        إذا (type == NGX_ERROR) {
            ngx_log_error(NGX_LOG_ERR, c->لگاریتم, 0,
                          "invalid PROXY protocol TLV \"%V\"", name);
            أرجع NGX_ERROR;
        }

        أرجع ngx_proxy_protocol_lookup_tlv(c, tlvs, type, مقدار);
    }

    لكل ( /* void */ ; te->type; te++) {
        إذا (te->name.len == n && ngx_strncmp(te->name.داده, p, n) == 0) {
            أرجع ngx_proxy_protocol_lookup_tlv(c, tlvs, te->type, مقدار);
        }
    }

    ngx_log_error(NGX_LOG_ERR, c->لگاریتم, 0,
                  "unknown PROXY protocol TLV \"%V\"", name);

    أرجع NGX_DECLINED;
}


ساكن ngx_int_t
ngx_proxy_protocol_lookup_tlv(ngx_connection_t *c, ngx_str_t *tlvs,
    ngx_uint_t type, ngx_str_t *مقدار)
{
    u_char                    *p;
    اندازه_t                     n, len;
    ngx_proxy_protocol_tlv_t  *tlv;

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->لگاریتم, 0,
                   "PROXY protocol v2 lookup tlv:%02xi", type);

    p = tlvs->داده;
    n = tlvs->len;

    طالما (n) {
        إذا (n < حجم(ngx_proxy_protocol_tlv_t)) {
            ngx_log_error(NGX_LOG_ERR, c->لگاریتم, 0, "broken PROXY protocol TLV");
            أرجع NGX_ERROR;
        }

        tlv = (ngx_proxy_protocol_tlv_t *) p;
        len = ngx_proxy_protocol_parse_uint16(tlv->len);

        p += حجم(ngx_proxy_protocol_tlv_t);
        n -= حجم(ngx_proxy_protocol_tlv_t);

        إذا (n < len) {
            ngx_log_error(NGX_LOG_ERR, c->لگاریتم, 0, "broken PROXY protocol TLV");
            أرجع NGX_ERROR;
        }

        إذا (tlv->type == type) {
            مقدار->داده = p;
            مقدار->len = len;
            أرجع NGX_OK;
        }

        p += len;
        n -= len;
    }

    أرجع NGX_DECLINED;
}


u_char *
ngx_proxy_protocol_v2_write(ngx_connection_t *c, u_char *buf, u_char *آخرین,
    ngx_array_t *tlvs /* reserved */ )
{
    u_char                          *p;
    uint32_t                         verify;
    ngx_uint_t                       i, client;
    ngx_array_t                      پایه_tlvs, ssl_tlvs;
    ngx_proxy_protocol_tlv_مقدار_t  *tlv;

    client = 0;
    verify = 0;

    ngx_memzero(&پایه_tlvs, حجم(ngx_array_t));
    ngx_memzero(&ssl_tlvs, حجم(ngx_array_t));

#إذا (NGX_OPENSSL)

    إذا (c->ssl != عدم) {
        إذا (ngx_array_init(&پایه_tlvs, c->pool, 2,
                           حجم(ngx_proxy_protocol_tlv_مقدار_t))
            != NGX_OK)
        {
            أرجع عدم;
        }

        إذا (ngx_array_init(&ssl_tlvs, c->pool, 5,
                           حجم(ngx_proxy_protocol_tlv_مقدار_t))
            != NGX_OK)
        {
            أرجع عدم;
        }

        إذا (ngx_proxy_protocol_v2_eval_ssl(c, &پایه_tlvs, &ssl_tlvs, &client,
                                           &verify)
            != NGX_OK)
        {
            أرجع عدم;
        }
    }

#پایان_اگر

    p = ngx_proxy_protocol_v2_write_سرآیند(c, buf, آخرین);
    إذا (p == عدم) {
        أرجع عدم;
    }

    tlv = پایه_tlvs.elts;

    لكل (i = 0; i < پایه_tlvs.nelts; i++) {
        p = ngx_proxy_protocol_v2_write_tlv(c, p, آخرین, tlv[i].type,
                                           &tlv[i].مقدار);
        إذا (p == عدم) {
            أرجع عدم;
        }
    }

    إذا (client) {
        p = ngx_proxy_protocol_v2_write_ssl(c, p, آخرین, &ssl_tlvs, client,
                                            verify);
        إذا (p == عدم) {
            أرجع عدم;
        }
    }

    ngx_proxy_protocol_v2_مجموعه_len(buf,
                                  p + حجم(ngx_proxy_protocol_tlv_t) + 4);

    أرجع ngx_proxy_protocol_v2_write_crc32c(c, buf, p, آخرین);
}


#إذا (NGX_OPENSSL)

ساكن ngx_int_t
ngx_proxy_protocol_v2_eval_ssl(ngx_connection_t *c, ngx_array_t *tlvs,
    ngx_array_t *ssl_tlvs, ngx_uint_t *client, uint32_t *verify)
{
    X509                            *cert;
    دراز                             n;
    ngx_int_t                        rc;
    ngx_str_t                        مقدار;
    ngx_uint_t                       type;
    ثابت حرف                      *s;
    ngx_proxy_protocol_tlv_مقدار_t  *tlv;

    إذا (ngx_proxy_protocol_v2_authority(c, &مقدار) == NGX_OK) {
        tlv = ngx_array_افزودن(tlvs);
        إذا (tlv == عدم) {
            أرجع NGX_ERROR;
        }

        tlv->type = NGX_PROXY_PROTOCOL_TLV_AUTHORITY;
        tlv->مقدار = مقدار;
    }

    إذا (ngx_proxy_protocol_v2_alpn(c, &مقدار) == NGX_OK) {
        tlv = ngx_array_افزودن(tlvs);
        إذا (tlv == عدم) {
            أرجع NGX_ERROR;
        }

        tlv->type = NGX_PROXY_PROTOCOL_TLV_ALPN;
        tlv->مقدار = مقدار;
    }

    لكل (type = NGX_PROXY_PROTOCOL_TLV_SSL_نسخه;
         type <= NGX_PROXY_PROTOCOL_TLV_SSL_کلید_ALG;
         type++)
    {
        rc = ngx_proxy_protocol_v2_ssl_sub(c, type, &مقدار);

        إذا (rc == NGX_ERROR) {
            أرجع NGX_ERROR;
        }

        إذا (rc != NGX_OK) {
            استمر;
        }

        tlv = ngx_array_افزودن(ssl_tlvs);
        إذا (tlv == عدم) {
            أرجع NGX_ERROR;
        }

        tlv->type = type;
        tlv->مقدار = مقدار;
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

        n = SSL_get_verify_نتیجه(c->ssl->connection);

        إذا (n == X509_V_OK) {

            إذا (ngx_ssl_ocsp_get_وضعیت(c, &s) != NGX_OK) {
                n = X509_V_ERR_CERT_REVOKED;
            }
        }

        *verify = (uint32_t) n;
    }

    أرجع NGX_OK;
}


ساكن ngx_int_t
ngx_proxy_protocol_v2_authority(ngx_connection_t *c, ngx_str_t *بیرون)
{
#اگر_تعریف_شده SSL_CTRL_مجموعه_TLSEXT_HOSTNAME

    ثابت حرف  *sni;

    sni = SSL_get_servername(c->ssl->connection, TLSEXT_NAMETYPE_host_name);
    إذا (sni == عدم) {
        أرجع NGX_DECLINED;
    }

    بیرون->داده = (u_char *) sni;
    بیرون->len = ngx_strlen(sni);
    أرجع NGX_OK;

#وإلا
    أرجع NGX_DECLINED;
#پایان_اگر
}


ساكن ngx_int_t
ngx_proxy_protocol_v2_alpn(ngx_connection_t *c, ngx_str_t *بیرون)
{
#اگر_تعریف_شده TLSEXT_TYPE_application_layer_protocol_negotiation

    بیعلامت صحيح          alpnlen;
    ثابت بیعلامت حرف  *alpn;

    SSL_get0_alpn_selected(c->ssl->connection, &alpn, &alpnlen);
    إذا (alpn == عدم || alpnlen == 0) {
        أرجع NGX_DECLINED;
    }

    بیرون->داده = (u_char *) alpn;
    بیرون->len = alpnlen;
    أرجع NGX_OK;

#وإلا
    أرجع NGX_DECLINED;
#پایان_اگر
}


ساكن ngx_int_t
ngx_proxy_protocol_v2_ssl_sub(ngx_connection_t *c, ngx_uint_t type,
    ngx_str_t *بیرون)
{
    صحيح               i, len;
    X509             *cert;
    u_char           *alg, *s;
    EVP_PKEY         *pkey;
#إذا (OPENSSL_نسخه_عدد >= 0x40000000L)
    ثابت
#پایان_اگر
    X509_NAME        *subject;
#إذا (OPENSSL_نسخه_عدد >= 0x40000000L)
    ثابت
#پایان_اگر
    X509_NAME_مدخل  *مدخل;

    بدل (type) {

    حالة NGX_PROXY_PROTOCOL_TLV_SSL_نسخه:
        بیرون->داده = (u_char *) SSL_get_نسخه(c->ssl->connection);
        بیرون->len = ngx_strlen(بیرون->داده);
        أرجع NGX_OK;

    حالة NGX_PROXY_PROTOCOL_TLV_SSL_CIPHER:
        بیرون->داده = (u_char *) SSL_get_cipher_name(c->ssl->connection);
        بیرون->len = ngx_strlen(بیرون->داده);
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

        i = X509_NAME_get_شاخص_by_NID(subject, NID_commonName, -1);
        إذا (i < 0) {
            X509_free(cert);
            أرجع NGX_DECLINED;
        }

        مدخل = X509_NAME_get_مدخل(subject, i);

        len = ASN1_STRING_مقصد_UTF8(&s, X509_NAME_مدخل_get_داده(مدخل));
        إذا (len < 0) {
            ngx_ssl_error(NGX_LOG_ALERT, c->لگاریتم, 0,
                          "ASN1_STRING_to_UTF8() failed");
            X509_free(cert);
            أرجع NGX_ERROR;
        }

        بیرون->len = len;
        بیرون->داده = ngx_pnalloc(c->pool, len);
        إذا (بیرون->داده == عدم) {
            OPENSSL_free(s);
            X509_free(cert);
            أرجع NGX_ERROR;
        }

        ngx_memcpy(بیرون->داده, s, len);

        OPENSSL_free(s);
        X509_free(cert);
        أرجع NGX_OK;

    حالة NGX_PROXY_PROTOCOL_TLV_SSL_SIG_ALG:
        cert = SSL_get_certificate(c->ssl->connection);

        بیرون->داده = (u_char *) OBJ_nid2sn(X509_get_signature_nid(cert));
        بیرون->len = ngx_strlen(بیرون->داده);
        أرجع NGX_OK;

    حالة NGX_PROXY_PROTOCOL_TLV_SSL_کلید_ALG:
        cert = SSL_get_certificate(c->ssl->connection);

        pkey = X509_get_pubkey(cert);
        إذا (pkey == عدم) {
            أرجع NGX_ERROR;
        }

        بدل (EVP_PKEY_پایه_id(pkey)) {

        حالة EVP_PKEY_RSA:
#اگر_تعریف_شده EVP_PKEY_RSA_PSS
        حالة EVP_PKEY_RSA_PSS:
#پایان_اگر
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

        بیرون->len = حجم("RSA16384") - 1;
        بیرون->داده = ngx_pnalloc(c->pool, بیرون->len);
        إذا (بیرون->داده == عدم) {
            EVP_PKEY_free(pkey);
            أرجع NGX_ERROR;
        }

        بیرون->len = ngx_snprintf(بیرون->داده, بیرون->len, "%s%d",
                                alg, EVP_PKEY_بیتها(pkey))
                   - بیرون->داده;

        EVP_PKEY_free(pkey);

        أرجع NGX_OK;
    }

    أرجع NGX_DECLINED;
}

#پایان_اگر


ساكن u_char *
ngx_proxy_protocol_v2_write_سرآیند(ngx_connection_t *c, u_char *buf,
    u_char *آخرین)
{
    u_char                            *p, src_af, dst_af,
                                       family, command, transport;
    ngx_uint_t                         src_fam, dst_fam;
    ngx_proxy_protocol_سرآیند_t       *سرآیند;
    ngx_proxy_protocol_inet_addrs_t   *درون;
#إذا (NGX_HAVE_INET6)
    ngx_uint_t                         پورت, lport;
    بنية in6_addr                   *src6, *dst6;
    ngx_proxy_protocol_inet6_addrs_t  *in6;
#پایان_اگر

    إذا (آخرین - buf < NGX_PROXY_PROTOCOL_V2_MAX_سرآیند) {
        ngx_log_error(NGX_LOG_ALERT, c->لگاریتم, 0,
                      "too small buffer for PROXY protocol v2");
        أرجع عدم;
    }

    إذا (ngx_connection_local_sockaddr(c, عدم, 0) != NGX_OK) {
        أرجع عدم;
    }

    /* the length field is set by the caller once all TLVs are written */

    سرآیند = (ngx_proxy_protocol_سرآیند_t *) buf;

    ngx_memcpy(سرآیند->signature, ngx_proxy_protocol_signature,
               حجم(ngx_proxy_protocol_signature) - 1);

    p = buf + حجم(ngx_proxy_protocol_سرآیند_t);

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
        حالة SOCK_جریان:
            transport = NGX_PROXY_PROTOCOL_TYPE_جریان;
            اكسر;
        حالة SOCK_DGRAM:
            transport = NGX_PROXY_PROTOCOL_TYPE_DGRAM;
            اكسر;
        افتراضي:
            transport = NGX_PROXY_PROTOCOL_TYPE_UNSPEC;
            اكسر;
        }
    }

    سرآیند->نسخه_command = 0x20 | command;
    سرآیند->family_transport = (family << 4) | transport;

    بدل (family) {

    حالة NGX_PROXY_PROTOCOL_AF_INET:

        درون = (ngx_proxy_protocol_inet_addrs_t *) p;

        ngx_proxy_protocol_v2_write_ipv4(c->sockaddr,
                                         درون->src_addr, درون->src_پورت);
        ngx_proxy_protocol_v2_write_ipv4(c->local_sockaddr,
                                         درون->dst_addr, درون->dst_پورت);

        p += حجم(ngx_proxy_protocol_inet_addrs_t);

        اكسر;

#إذا (NGX_HAVE_INET6)

    حالة NGX_PROXY_PROTOCOL_AF_INET6:

        إذا (src_fam == AF_INET6 && dst_fam == AF_INET6) {
            src6 = &((بنية sockaddr_in6 *) c->sockaddr)->sin6_addr;
            dst6 = &((بنية sockaddr_in6 *) c->local_sockaddr)->sin6_addr;

            إذا (IN6_IS_ADDR_V4MAPPED(src6) && IN6_IS_ADDR_V4MAPPED(dst6)) {

                /* both v4-mapped: demote to AF_INET */

                سرآیند->family_transport = (NGX_PROXY_PROTOCOL_AF_INET << 4)
                                           | transport;

                درون = (ngx_proxy_protocol_inet_addrs_t *) p;

                ngx_memcpy(درون->src_addr, src6->s6_addr + 12, 4);
                ngx_memcpy(درون->dst_addr, dst6->s6_addr + 12, 4);

                پورت  = ngx_inet_get_پورت(c->sockaddr);
                lport = ngx_inet_get_پورت(c->local_sockaddr);

                درون->src_پورت[0] = (u_char) (پورت >> 8);
                درون->src_پورت[1] = (u_char)  پورت;
                درون->dst_پورت[0] = (u_char) (lport >> 8);
                درون->dst_پورت[1] = (u_char)  lport;

                p += حجم(ngx_proxy_protocol_inet_addrs_t);
                اكسر;
            }
        }

        in6 = (ngx_proxy_protocol_inet6_addrs_t *) p;

        ngx_proxy_protocol_v2_write_ipv6(c->sockaddr,
                                         in6->src_addr, in6->src_پورت);
        ngx_proxy_protocol_v2_write_ipv6(c->local_sockaddr,
                                         in6->dst_addr, in6->dst_پورت);

        p += حجم(ngx_proxy_protocol_inet6_addrs_t);

        اكسر;

#پایان_اگر

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
#پایان_اگر

    افتراضي:
        أرجع NGX_PROXY_PROTOCOL_AF_UNSPEC;
    }
}


ساكن فراغ
ngx_proxy_protocol_v2_write_ipv4(بنية sockaddr *sa, u_char *addr,
    u_char *پورت)
{
    بنية sockaddr_درون  *سینوس;

    سینوس = (بنية sockaddr_درون *) sa;
    ngx_memcpy(addr, &سینوس->sin_addr, 4);
    ngx_memcpy(پورت, &سینوس->sin_پورت, 2);
}


#إذا (NGX_HAVE_INET6)

ساكن فراغ
ngx_proxy_protocol_v2_write_ipv6(بنية sockaddr *sa, u_char *addr,
    u_char *پورت)
{
    بنية sockaddr_درون   *سینوس;
    بنية sockaddr_in6  *sin6;

    /* IPv4-mapped prefix: 80 zero bits + 16 one bits */
    ساكن ثابت u_char  mapped[]   = { 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0xff, 0xff };

    إذا (sa->sa_family == AF_INET6) {
        sin6 = (بنية sockaddr_in6 *) sa;
        ngx_memcpy(addr, &sin6->sin6_addr, 16);
        ngx_memcpy(پورت, &sin6->sin6_پورت, 2);

    } وإلا {
        /* promote AF_INET to ::ffff:a.b.c.d */
        سینوس = (بنية sockaddr_درون *) sa;
        ngx_memcpy(addr, mapped, حجم(mapped));
        ngx_memcpy(addr + حجم(mapped), &سینوس->sin_addr, 4);
        ngx_memcpy(پورت, &سینوس->sin_پورت, 2);
    }
}

#پایان_اگر


ساكن u_char *
ngx_proxy_protocol_v2_write_tlv(ngx_connection_t *c, u_char *p, u_char *آخرین,
    ngx_uint_t type, ngx_str_t *مقدار)
{
    ngx_proxy_protocol_tlv_t  *tlv;

    إذا ((اندازه_t) (آخرین - p) < حجم(ngx_proxy_protocol_tlv_t) + مقدار->len) {
        ngx_log_error(NGX_LOG_ALERT, c->لگاریتم, 0,
                      "too small buffer for PROXY protocol v2 TLV");
        أرجع عدم;
    }

    tlv = (ngx_proxy_protocol_tlv_t *) p;
    tlv->type = (u_char) type;
    tlv->len[0] = (u_char) (مقدار->len >> 8);
    tlv->len[1] = (u_char)  مقدار->len;

    p += حجم(ngx_proxy_protocol_tlv_t);

    أرجع ngx_cpymem(p, مقدار->داده, مقدار->len);
}


ساكن u_char *
ngx_proxy_protocol_v2_write_ssl(ngx_connection_t *c, u_char *p, u_char *آخرین,
    ngx_array_t *ssl_tlvs, ngx_uint_t client, uint32_t verify)
{
    u_char                          *شروع;
    uint16_t                         len;
    ngx_uint_t                       i;
    ngx_proxy_protocol_tlv_t        *tlv;
    ngx_proxy_protocol_tlv_ssl_t    *tlv_ssl;
    ngx_proxy_protocol_tlv_مقدار_t  *sub;

    إذا ((اندازه_t) (آخرین - p) < حجم(ngx_proxy_protocol_tlv_t)
                              + حجم(ngx_proxy_protocol_tlv_ssl_t))
    {
        ngx_log_error(NGX_LOG_ALERT, c->لگاریتم, 0,
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
        p = ngx_proxy_protocol_v2_write_tlv(c, p, آخرین, sub[i].type,
                                            &sub[i].مقدار);
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
ngx_proxy_protocol_v2_مجموعه_len(u_char *buf, u_char *p)
{
    uint16_t                      len;
    ngx_proxy_protocol_سرآیند_t  *سرآیند;

    len = (uint16_t) (p - buf - حجم(ngx_proxy_protocol_سرآیند_t));

    سرآیند = (ngx_proxy_protocol_سرآیند_t *) buf;
    سرآیند->len[0] = (u_char) (len >> 8);
    سرآیند->len[1] = (u_char) len;
}


ساكن u_char *
ngx_proxy_protocol_v2_write_crc32c(ngx_connection_t *c, u_char *buf,
    u_char *p, u_char *آخرین)
{
    u_char                    *مقدار;
    uint32_t                   بررسی_چرخهای;
    ngx_proxy_protocol_tlv_t  *tlv;

    إذا ((اندازه_t) (آخرین - p) < حجم(ngx_proxy_protocol_tlv_t) + 4) {
        ngx_log_error(NGX_LOG_ALERT, c->لگاریتم, 0,
                      "too small buffer for PROXY protocol v2 CRC32c TLV");
        أرجع عدم;
    }

    /* append the CRC32c TLV with a zeroed value field */

    tlv = (ngx_proxy_protocol_tlv_t *) p;
    tlv->type = NGX_PROXY_PROTOCOL_TLV_بررسی_چرخهای_32_C;
    tlv->len[0] = 0;
    tlv->len[1] = 4;

    مقدار = p + حجم(ngx_proxy_protocol_tlv_t);

    ngx_memzero(مقدار, 4);

    p = مقدار + 4;

    /* the checksum covers the entire header with the zeroed value */

    بررسی_چرخهای = ngx_crc32c_long(buf, p - buf);

    مقدار[0] = (u_char) (بررسی_چرخهای >> 24);
    مقدار[1] = (u_char) (بررسی_چرخهای >> 16);
    مقدار[2] = (u_char) (بررسی_چرخهای >> 8);
    مقدار[3] = (u_char) بررسی_چرخهای;

    أرجع p;
}
