
/*
 * Copyright (C) Roman Arutyunyan
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


#تعریف_کړه NGX_PROXY_PROTOCOL_V2_MAX_سرلیک    52

#تعریف_کړه NGX_PROXY_PROTOCOL_CMD_LOCAL        0
#تعریف_کړه NGX_PROXY_PROTOCOL_CMD_PROXY        1

#تعریف_کړه NGX_PROXY_PROTOCOL_AF_UNSPEC        0
#تعریف_کړه NGX_PROXY_PROTOCOL_AF_INET          1
#تعریف_کړه NGX_PROXY_PROTOCOL_AF_INET6         2

#تعریف_کړه NGX_PROXY_PROTOCOL_TYPE_UNSPEC      0
#تعریف_کړه NGX_PROXY_PROTOCOL_TYPE_بهیر      1
#تعریف_کړه NGX_PROXY_PROTOCOL_TYPE_DGRAM       2

#تعریف_کړه NGX_PROXY_PROTOCOL_TLV_ALPN         0x01
#تعریف_کړه NGX_PROXY_PROTOCOL_TLV_AUTHORITY    0x02
#تعریف_کړه NGX_PROXY_PROTOCOL_TLV_دوراني_ازموینه_32_C       0x03
#تعریف_کړه NGX_PROXY_PROTOCOL_TLV_UNIQUE_ID    0x05
#تعریف_کړه NGX_PROXY_PROTOCOL_TLV_SSL          0x20
#تعریف_کړه NGX_PROXY_PROTOCOL_TLV_SSL_نسخه  0x21
#تعریف_کړه NGX_PROXY_PROTOCOL_TLV_SSL_CN       0x22
#تعریف_کړه NGX_PROXY_PROTOCOL_TLV_SSL_CIPHER   0x23
#تعریف_کړه NGX_PROXY_PROTOCOL_TLV_SSL_SIG_ALG  0x24
#تعریف_کړه NGX_PROXY_PROTOCOL_TLV_SSL_کلی_ALG  0x25
#تعریف_کړه NGX_PROXY_PROTOCOL_TLV_NETNS        0x30

#تعریف_کړه NGX_PROXY_PROTOCOL_V2_CLIENT_SSL          0x01
#تعریف_کړه NGX_PROXY_PROTOCOL_V2_CLIENT_CERT_CONN    0x02
#تعریف_کړه NGX_PROXY_PROTOCOL_V2_CLIENT_CERT_SESS    0x04


#تعریف_کړه ngx_proxy_protocol_parse_uint16(p)                                    \
    ( ((uint16_t) (p)[0] << 8)                                                \
    + (           (p)[1]) )

#تعریف_کړه ngx_proxy_protocol_parse_uint32(p)                                    \
    ( ((uint32_t) (p)[0] << 24)                                               \
    + (           (p)[1] << 16)                                               \
    + (           (p)[2] << 8)                                                \
    + (           (p)[3]) )


تعريفنوع بنية {
    u_char                                  signature[12];
    u_char                                  نسخه_command;
    u_char                                  family_transport;
    u_char                                  len[2];
} ngx_proxy_protocol_سرلیک_t;


تعريفنوع بنية {
    u_char                                  src_addr[4];
    u_char                                  dst_addr[4];
    u_char                                  src_پورټ[2];
    u_char                                  dst_پورټ[2];
} ngx_proxy_protocol_inet_addrs_t;


تعريفنوع بنية {
    u_char                                  src_addr[16];
    u_char                                  dst_addr[16];
    u_char                                  src_پورټ[2];
    u_char                                  dst_پورټ[2];
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
} ngx_proxy_protocol_tlv_ثبت_t;


تعريفنوع بنية {
    ngx_uint_t                              type;
    ngx_str_t                               ارزښت;
} ngx_proxy_protocol_tlv_ارزښت_t;


ساكن u_char *ngx_proxy_protocol_read_addr(ngx_connection_t *c, u_char *p,
    u_char *وروستی, ngx_str_t *addr);
ساكن u_char *ngx_proxy_protocol_read_پورټ(u_char *p, u_char *وروستی,
    دننه_پورټ_t *پورټ, u_char sep);
ساكن u_char *ngx_proxy_protocol_v2_read(ngx_connection_t *c, u_char *buf,
    u_char *وروستی);
ساكن ngx_int_t ngx_proxy_protocol_lookup_tlv(ngx_connection_t *c,
    ngx_str_t *tlvs, ngx_uint_t type, ngx_str_t *ارزښت);
#إذا (NGX_OPENSSL)
ساكن ngx_int_t ngx_proxy_protocol_v2_eval_ssl(ngx_connection_t *c,
    ngx_array_t *tlvs, ngx_array_t *ssl_tlvs, ngx_uint_t *client,
    uint32_t *verify);
ساكن ngx_int_t ngx_proxy_protocol_v2_authority(ngx_connection_t *c,
    ngx_str_t *بهر);
ساكن ngx_int_t ngx_proxy_protocol_v2_alpn(ngx_connection_t *c,
    ngx_str_t *بهر);
ساكن ngx_int_t ngx_proxy_protocol_v2_ssl_sub(ngx_connection_t *c,
    ngx_uint_t type, ngx_str_t *بهر);
#د_که_پای
ساكن u_char *ngx_proxy_protocol_v2_write_سرلیک(ngx_connection_t *c,
    u_char *buf, u_char *وروستی);
ساكن u_char ngx_proxy_protocol_v2_family(ngx_uint_t family);
ساكن فراغ ngx_proxy_protocol_v2_write_ipv4(بنية sockaddr *sa, u_char *addr,
    u_char *پورټ);
ساكن فراغ ngx_proxy_protocol_v2_write_ipv6(بنية sockaddr *sa, u_char *addr,
    u_char *پورټ);
ساكن u_char *ngx_proxy_protocol_v2_write_tlv(ngx_connection_t *c, u_char *p,
    u_char *وروستی, ngx_uint_t type, ngx_str_t *ارزښت);
ساكن u_char *ngx_proxy_protocol_v2_write_ssl(ngx_connection_t *c, u_char *p,
    u_char *وروستی, ngx_array_t *ssl_tlvs, ngx_uint_t client, uint32_t verify);
ساكن فراغ ngx_proxy_protocol_v2_سېټ_len(u_char *buf, u_char *p);
ساكن u_char *ngx_proxy_protocol_v2_write_crc32c(ngx_connection_t *c,
    u_char *buf, u_char *p, u_char *وروستی);


ساكن ngx_proxy_protocol_tlv_ثبت_t  ngx_proxy_protocol_tlv_entries[] = {
    { ngx_string("alpn"),       NGX_PROXY_PROTOCOL_TLV_ALPN },
    { ngx_string("authority"),  NGX_PROXY_PROTOCOL_TLV_AUTHORITY },
    { ngx_string("unique_id"),  NGX_PROXY_PROTOCOL_TLV_UNIQUE_ID },
    { ngx_string("ssl"),        NGX_PROXY_PROTOCOL_TLV_SSL },
    { ngx_string("netns"),      NGX_PROXY_PROTOCOL_TLV_NETNS },
    { ngx_null_string,          0x00 }
};


ساكن ngx_proxy_protocol_tlv_ثبت_t  ngx_proxy_protocol_tlv_ssl_entries[] = {
    { ngx_string("version"),    NGX_PROXY_PROTOCOL_TLV_SSL_نسخه },
    { ngx_string("cn"),         NGX_PROXY_PROTOCOL_TLV_SSL_CN },
    { ngx_string("cipher"),     NGX_PROXY_PROTOCOL_TLV_SSL_CIPHER },
    { ngx_string("sig_alg"),    NGX_PROXY_PROTOCOL_TLV_SSL_SIG_ALG },
    { ngx_string("key_alg"),    NGX_PROXY_PROTOCOL_TLV_SSL_کلی_ALG },
    { ngx_null_string,          0x00 }
};


ساكن ثابت u_char  ngx_proxy_protocol_signature[] = "\r\n\r\n\0\r\nQUIT\n";


u_char *
ngx_proxy_protocol_read(ngx_connection_t *c, u_char *buf, u_char *وروستی)
{
    اندازه_t                 len;
    u_char                *p;
    ngx_proxy_protocol_t  *pp;

    p = buf;
    len = وروستی - buf;

    إذا (len >= حجم(ngx_proxy_protocol_سرلیک_t)
        && ngx_memcmp(p, ngx_proxy_protocol_signature,
                      حجم(ngx_proxy_protocol_signature) - 1)
           == 0)
    {
        أرجع ngx_proxy_protocol_v2_read(c, buf, وروستی);
    }

    إذا (len < 8 || ngx_strncmp(p, "PROXY ", 6) != 0) {
        اذهب invalid;
    }

    p += 6;
    len -= 6;

    إذا (len >= 7 && ngx_strncmp(p, "UNKNOWN", 7) == 0) {
        ngx_log_debug0(NGX_LOG_DEBUG_CORE, c->لوګاریتم, 0,
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

    p = ngx_proxy_protocol_read_addr(c, p, وروستی, &pp->src_addr);
    إذا (p == عدم) {
        اذهب invalid;
    }

    p = ngx_proxy_protocol_read_addr(c, p, وروستی, &pp->dst_addr);
    إذا (p == عدم) {
        اذهب invalid;
    }

    p = ngx_proxy_protocol_read_پورټ(p, وروستی, &pp->src_پورټ, ' ');
    إذا (p == عدم) {
        اذهب invalid;
    }

    p = ngx_proxy_protocol_read_پورټ(p, وروستی, &pp->dst_پورټ, CR);
    إذا (p == عدم) {
        اذهب invalid;
    }

    إذا (p == وروستی) {
        اذهب invalid;
    }

    إذا (*p++ != LF) {
        اذهب invalid;
    }

    ngx_log_debug4(NGX_LOG_DEBUG_CORE, c->لوګاریتم, 0,
                   "PROXY protocol src: %V %d, dst: %V %d",
                   &pp->src_addr, pp->src_پورټ, &pp->dst_addr, pp->dst_پورټ);

    c->proxy_protocol = pp;

    أرجع p;

skip:

    لكل ( /* void */ ; p < وروستی - 1; p++) {
        إذا (p[0] == CR && p[1] == LF) {
            أرجع p + 2;
        }
    }

invalid:

    لكل (p = buf; p < وروستی; p++) {
        إذا (*p == CR || *p == LF) {
            اكسر;
        }
    }

    ngx_log_error(NGX_LOG_ERR, c->لوګاریتم, 0,
                  "broken header: \"%*s\"", (اندازه_t) (p - buf), buf);

    أرجع عدم;
}


ساكن u_char *
ngx_proxy_protocol_read_addr(ngx_connection_t *c, u_char *p, u_char *وروستی,
    ngx_str_t *addr)
{
    اندازه_t  len;
    u_char  ch, *pos;

    pos = p;

    لكل ( ;; ) {
        إذا (p == وروستی) {
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

    addr->معلومات = ngx_pnalloc(c->pool, len);
    إذا (addr->معلومات == عدم) {
        أرجع عدم;
    }

    ngx_memcpy(addr->معلومات, pos, len);
    addr->len = len;

    أرجع p;
}


ساكن u_char *
ngx_proxy_protocol_read_پورټ(u_char *p, u_char *وروستی, دننه_پورټ_t *پورټ,
    u_char sep)
{
    اندازه_t      len;
    u_char     *pos;
    ngx_int_t   n;

    pos = p;

    لكل ( ;; ) {
        إذا (p == وروستی) {
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

    *پورټ = (دننه_پورټ_t) n;

    أرجع p;
}


u_char *
ngx_proxy_protocol_write(ngx_connection_t *c, u_char *buf, u_char *وروستی)
{
    ngx_uint_t  پورټ, lport;

    إذا (وروستی - buf < NGX_PROXY_PROTOCOL_V1_MAX_سرلیک) {
        ngx_log_error(NGX_LOG_ALERT, c->لوګاریتم, 0,
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
#د_که_پای

    افتراضي:
        أرجع ngx_cpymem(buf, "PROXY UNKNOWN" CRLF,
                          حجم("PROXY UNKNOWN" CRLF) - 1);
    }

    buf += ngx_sock_ntop(c->sockaddr, c->socklen, buf, وروستی - buf, 0);

    *buf++ = ' ';

    buf += ngx_sock_ntop(c->local_sockaddr, c->local_socklen, buf, وروستی - buf,
                         0);

    پورټ = ngx_inet_get_پورټ(c->sockaddr);
    lport = ngx_inet_get_پورټ(c->local_sockaddr);

    أرجع ngx_slprintf(buf, وروستی, " %ui %ui" CRLF, پورټ, lport);
}


ساكن u_char *
ngx_proxy_protocol_v2_read(ngx_connection_t *c, u_char *buf, u_char *وروستی)
{
    u_char                             *پای_ټکی;
    اندازه_t                              len;
    socklen_t                           socklen;
    ngx_uint_t                          نسخه, command, family, transport;
    ngx_sockaddr_t                      src_sockaddr, dst_sockaddr;
    ngx_proxy_protocol_t               *pp;
    ngx_proxy_protocol_سرلیک_t        *سرلیک;
    ngx_proxy_protocol_inet_addrs_t    *دننه;
#إذا (NGX_HAVE_INET6)
    ngx_proxy_protocol_inet6_addrs_t   *in6;
#د_که_پای

    سرلیک = (ngx_proxy_protocol_سرلیک_t *) buf;

    buf += حجم(ngx_proxy_protocol_سرلیک_t);

    نسخه = سرلیک->نسخه_command >> 4;

    إذا (نسخه != 2) {
        ngx_log_error(NGX_LOG_ERR, c->لوګاریتم, 0,
                      "unknown PROXY protocol version: %ui", نسخه);
        أرجع عدم;
    }

    len = ngx_proxy_protocol_parse_uint16(سرلیک->len);

    إذا ((اندازه_t) (وروستی - buf) < len) {
        ngx_log_error(NGX_LOG_ERR, c->لوګاریتم, 0, "header is too large");
        أرجع عدم;
    }

    پای_ټکی = buf + len;

    command = سرلیک->نسخه_command & 0x0f;

    /* only PROXY is supported */
    إذا (command != NGX_PROXY_PROTOCOL_CMD_PROXY) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->لوګاریتم, 0,
                       "PROXY protocol v2 unsupported command %ui", command);
        أرجع پای_ټکی;
    }

    transport = سرلیک->family_transport & 0x0f;

    /* only STREAM is supported */
    إذا (transport != NGX_PROXY_PROTOCOL_TYPE_بهیر) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->لوګاریتم, 0,
                       "PROXY protocol v2 unsupported transport %ui",
                       transport);
        أرجع پای_ټکی;
    }

    pp = ngx_pcalloc(c->pool, حجم(ngx_proxy_protocol_t));
    إذا (pp == عدم) {
        أرجع عدم;
    }

    family = سرلیک->family_transport >> 4;

    بدل (family) {

    حالة NGX_PROXY_PROTOCOL_AF_INET:

        إذا ((اندازه_t) (پای_ټکی - buf) < حجم(ngx_proxy_protocol_inet_addrs_t)) {
            أرجع عدم;
        }

        دننه = (ngx_proxy_protocol_inet_addrs_t *) buf;

        src_sockaddr.sockaddr_دننه.sin_family = AF_INET;
        src_sockaddr.sockaddr_دننه.sin_پورټ = 0;
        ngx_memcpy(&src_sockaddr.sockaddr_دننه.sin_addr, دننه->src_addr, 4);

        dst_sockaddr.sockaddr_دننه.sin_family = AF_INET;
        dst_sockaddr.sockaddr_دننه.sin_پورټ = 0;
        ngx_memcpy(&dst_sockaddr.sockaddr_دننه.sin_addr, دننه->dst_addr, 4);

        pp->src_پورټ = ngx_proxy_protocol_parse_uint16(دننه->src_پورټ);
        pp->dst_پورټ = ngx_proxy_protocol_parse_uint16(دننه->dst_پورټ);

        socklen = حجم(بنية sockaddr_دننه);

        buf += حجم(ngx_proxy_protocol_inet_addrs_t);

        اكسر;

#إذا (NGX_HAVE_INET6)

    حالة NGX_PROXY_PROTOCOL_AF_INET6:

        إذا ((اندازه_t) (پای_ټکی - buf) < حجم(ngx_proxy_protocol_inet6_addrs_t)) {
            أرجع عدم;
        }

        in6 = (ngx_proxy_protocol_inet6_addrs_t *) buf;

        src_sockaddr.sockaddr_in6.sin6_family = AF_INET6;
        src_sockaddr.sockaddr_in6.sin6_پورټ = 0;
        ngx_memcpy(&src_sockaddr.sockaddr_in6.sin6_addr, in6->src_addr, 16);

        dst_sockaddr.sockaddr_in6.sin6_family = AF_INET6;
        dst_sockaddr.sockaddr_in6.sin6_پورټ = 0;
        ngx_memcpy(&dst_sockaddr.sockaddr_in6.sin6_addr, in6->dst_addr, 16);

        pp->src_پورټ = ngx_proxy_protocol_parse_uint16(in6->src_پورټ);
        pp->dst_پورټ = ngx_proxy_protocol_parse_uint16(in6->dst_پورټ);

        socklen = حجم(بنية sockaddr_in6);

        buf += حجم(ngx_proxy_protocol_inet6_addrs_t);

        اكسر;

#د_که_پای

    افتراضي:
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->لوګاریتم, 0,
                       "PROXY protocol v2 unsupported address family %ui",
                       family);
        أرجع پای_ټکی;
    }

    pp->src_addr.معلومات = ngx_pnalloc(c->pool, NGX_SOCKADDR_STRLEN);
    إذا (pp->src_addr.معلومات == عدم) {
        أرجع عدم;
    }

    pp->src_addr.len = ngx_sock_ntop(&src_sockaddr.sockaddr, socklen,
                                     pp->src_addr.معلومات, NGX_SOCKADDR_STRLEN, 0);

    pp->dst_addr.معلومات = ngx_pnalloc(c->pool, NGX_SOCKADDR_STRLEN);
    إذا (pp->dst_addr.معلومات == عدم) {
        أرجع عدم;
    }

    pp->dst_addr.len = ngx_sock_ntop(&dst_sockaddr.sockaddr, socklen,
                                     pp->dst_addr.معلومات, NGX_SOCKADDR_STRLEN, 0);

    ngx_log_debug4(NGX_LOG_DEBUG_CORE, c->لوګاریتم, 0,
                   "PROXY protocol v2 src: %V %d, dst: %V %d",
                   &pp->src_addr, pp->src_پورټ, &pp->dst_addr, pp->dst_پورټ);

    إذا (buf < پای_ټکی) {
        pp->tlvs.معلومات = ngx_pnalloc(c->pool, پای_ټکی - buf);
        إذا (pp->tlvs.معلومات == عدم) {
            أرجع عدم;
        }

        ngx_memcpy(pp->tlvs.معلومات, buf, پای_ټکی - buf);
        pp->tlvs.len = پای_ټکی - buf;
    }

    c->proxy_protocol = pp;

    أرجع پای_ټکی;
}


ngx_int_t
ngx_proxy_protocol_get_tlv(ngx_connection_t *c, ngx_str_t *name,
    ngx_str_t *ارزښت)
{
    u_char                          *p;
    اندازه_t                           n;
    uint32_t                         verify;
    ngx_str_t                        ssl, *tlvs;
    ngx_int_t                        rc, type;
    ngx_proxy_protocol_tlv_ssl_t    *tlv_ssl;
    ngx_proxy_protocol_tlv_ثبت_t  *te;

    إذا (c->proxy_protocol == عدم) {
        أرجع NGX_DECLINED;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->لوګاریتم, 0,
                   "PROXY protocol v2 get tlv \"%V\"", name);

    te = ngx_proxy_protocol_tlv_entries;
    tlvs = &c->proxy_protocol->tlvs;

    p = name->معلومات;
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

            tlv_ssl = (ngx_proxy_protocol_tlv_ssl_t *) ssl.معلومات;
            verify = ngx_proxy_protocol_parse_uint32(tlv_ssl->verify);

            ارزښت->معلومات = ngx_pnalloc(c->pool, NGX_INT32_LEN);
            إذا (ارزښت->معلومات == عدم) {
                أرجع NGX_ERROR;
            }

            ارزښت->len = ngx_sprintf(ارزښت->معلومات, "%uD", verify)
                         - ارزښت->معلومات;
            أرجع NGX_OK;
        }

        ssl.معلومات += حجم(ngx_proxy_protocol_tlv_ssl_t);
        ssl.len -= حجم(ngx_proxy_protocol_tlv_ssl_t);

        te = ngx_proxy_protocol_tlv_ssl_entries;
        tlvs = &ssl;
    }

    إذا (n >= 2 && p[0] == '0' && p[1] == 'x') {

        type = ngx_hextoi(p + 2, n - 2);
        إذا (type == NGX_ERROR) {
            ngx_log_error(NGX_LOG_ERR, c->لوګاریتم, 0,
                          "invalid PROXY protocol TLV \"%V\"", name);
            أرجع NGX_ERROR;
        }

        أرجع ngx_proxy_protocol_lookup_tlv(c, tlvs, type, ارزښت);
    }

    لكل ( /* void */ ; te->type; te++) {
        إذا (te->name.len == n && ngx_strncmp(te->name.معلومات, p, n) == 0) {
            أرجع ngx_proxy_protocol_lookup_tlv(c, tlvs, te->type, ارزښت);
        }
    }

    ngx_log_error(NGX_LOG_ERR, c->لوګاریتم, 0,
                  "unknown PROXY protocol TLV \"%V\"", name);

    أرجع NGX_DECLINED;
}


ساكن ngx_int_t
ngx_proxy_protocol_lookup_tlv(ngx_connection_t *c, ngx_str_t *tlvs,
    ngx_uint_t type, ngx_str_t *ارزښت)
{
    u_char                    *p;
    اندازه_t                     n, len;
    ngx_proxy_protocol_tlv_t  *tlv;

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->لوګاریتم, 0,
                   "PROXY protocol v2 lookup tlv:%02xi", type);

    p = tlvs->معلومات;
    n = tlvs->len;

    طالما (n) {
        إذا (n < حجم(ngx_proxy_protocol_tlv_t)) {
            ngx_log_error(NGX_LOG_ERR, c->لوګاریتم, 0, "broken PROXY protocol TLV");
            أرجع NGX_ERROR;
        }

        tlv = (ngx_proxy_protocol_tlv_t *) p;
        len = ngx_proxy_protocol_parse_uint16(tlv->len);

        p += حجم(ngx_proxy_protocol_tlv_t);
        n -= حجم(ngx_proxy_protocol_tlv_t);

        إذا (n < len) {
            ngx_log_error(NGX_LOG_ERR, c->لوګاریتم, 0, "broken PROXY protocol TLV");
            أرجع NGX_ERROR;
        }

        إذا (tlv->type == type) {
            ارزښت->معلومات = p;
            ارزښت->len = len;
            أرجع NGX_OK;
        }

        p += len;
        n -= len;
    }

    أرجع NGX_DECLINED;
}


u_char *
ngx_proxy_protocol_v2_write(ngx_connection_t *c, u_char *buf, u_char *وروستی,
    ngx_array_t *tlvs /* reserved */ )
{
    u_char                          *p;
    uint32_t                         verify;
    ngx_uint_t                       i, client;
    ngx_array_t                      بنسټ_tlvs, ssl_tlvs;
    ngx_proxy_protocol_tlv_ارزښت_t  *tlv;

    client = 0;
    verify = 0;

    ngx_memzero(&بنسټ_tlvs, حجم(ngx_array_t));
    ngx_memzero(&ssl_tlvs, حجم(ngx_array_t));

#إذا (NGX_OPENSSL)

    إذا (c->ssl != عدم) {
        إذا (ngx_array_init(&بنسټ_tlvs, c->pool, 2,
                           حجم(ngx_proxy_protocol_tlv_ارزښت_t))
            != NGX_OK)
        {
            أرجع عدم;
        }

        إذا (ngx_array_init(&ssl_tlvs, c->pool, 5,
                           حجم(ngx_proxy_protocol_tlv_ارزښت_t))
            != NGX_OK)
        {
            أرجع عدم;
        }

        إذا (ngx_proxy_protocol_v2_eval_ssl(c, &بنسټ_tlvs, &ssl_tlvs, &client,
                                           &verify)
            != NGX_OK)
        {
            أرجع عدم;
        }
    }

#د_که_پای

    p = ngx_proxy_protocol_v2_write_سرلیک(c, buf, وروستی);
    إذا (p == عدم) {
        أرجع عدم;
    }

    tlv = بنسټ_tlvs.elts;

    لكل (i = 0; i < بنسټ_tlvs.nelts; i++) {
        p = ngx_proxy_protocol_v2_write_tlv(c, p, وروستی, tlv[i].type,
                                           &tlv[i].ارزښت);
        إذا (p == عدم) {
            أرجع عدم;
        }
    }

    إذا (client) {
        p = ngx_proxy_protocol_v2_write_ssl(c, p, وروستی, &ssl_tlvs, client,
                                            verify);
        إذا (p == عدم) {
            أرجع عدم;
        }
    }

    ngx_proxy_protocol_v2_سېټ_len(buf,
                                  p + حجم(ngx_proxy_protocol_tlv_t) + 4);

    أرجع ngx_proxy_protocol_v2_write_crc32c(c, buf, p, وروستی);
}


#إذا (NGX_OPENSSL)

ساكن ngx_int_t
ngx_proxy_protocol_v2_eval_ssl(ngx_connection_t *c, ngx_array_t *tlvs,
    ngx_array_t *ssl_tlvs, ngx_uint_t *client, uint32_t *verify)
{
    X509                            *cert;
    اوږد                             n;
    ngx_int_t                        rc;
    ngx_str_t                        ارزښت;
    ngx_uint_t                       type;
    ثابت حرف                      *s;
    ngx_proxy_protocol_tlv_ارزښت_t  *tlv;

    إذا (ngx_proxy_protocol_v2_authority(c, &ارزښت) == NGX_OK) {
        tlv = ngx_array_ټېل(tlvs);
        إذا (tlv == عدم) {
            أرجع NGX_ERROR;
        }

        tlv->type = NGX_PROXY_PROTOCOL_TLV_AUTHORITY;
        tlv->ارزښت = ارزښت;
    }

    إذا (ngx_proxy_protocol_v2_alpn(c, &ارزښت) == NGX_OK) {
        tlv = ngx_array_ټېل(tlvs);
        إذا (tlv == عدم) {
            أرجع NGX_ERROR;
        }

        tlv->type = NGX_PROXY_PROTOCOL_TLV_ALPN;
        tlv->ارزښت = ارزښت;
    }

    لكل (type = NGX_PROXY_PROTOCOL_TLV_SSL_نسخه;
         type <= NGX_PROXY_PROTOCOL_TLV_SSL_کلی_ALG;
         type++)
    {
        rc = ngx_proxy_protocol_v2_ssl_sub(c, type, &ارزښت);

        إذا (rc == NGX_ERROR) {
            أرجع NGX_ERROR;
        }

        إذا (rc != NGX_OK) {
            استمر;
        }

        tlv = ngx_array_ټېل(ssl_tlvs);
        إذا (tlv == عدم) {
            أرجع NGX_ERROR;
        }

        tlv->type = type;
        tlv->ارزښت = ارزښت;
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

        n = SSL_get_verify_پایله(c->ssl->connection);

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
ngx_proxy_protocol_v2_authority(ngx_connection_t *c, ngx_str_t *بهر)
{
#که_تعریف_شوی SSL_CTRL_سېټ_TLSEXT_HOSTNAME

    ثابت حرف  *sni;

    sni = SSL_get_servername(c->ssl->connection, TLSEXT_NAMETYPE_host_name);
    إذا (sni == عدم) {
        أرجع NGX_DECLINED;
    }

    بهر->معلومات = (u_char *) sni;
    بهر->len = ngx_strlen(sni);
    أرجع NGX_OK;

#وإلا
    أرجع NGX_DECLINED;
#د_که_پای
}


ساكن ngx_int_t
ngx_proxy_protocol_v2_alpn(ngx_connection_t *c, ngx_str_t *بهر)
{
#که_تعریف_شوی TLSEXT_TYPE_application_layer_protocol_negotiation

    بینښه صحيح          alpnlen;
    ثابت بینښه حرف  *alpn;

    SSL_get0_alpn_selected(c->ssl->connection, &alpn, &alpnlen);
    إذا (alpn == عدم || alpnlen == 0) {
        أرجع NGX_DECLINED;
    }

    بهر->معلومات = (u_char *) alpn;
    بهر->len = alpnlen;
    أرجع NGX_OK;

#وإلا
    أرجع NGX_DECLINED;
#د_که_پای
}


ساكن ngx_int_t
ngx_proxy_protocol_v2_ssl_sub(ngx_connection_t *c, ngx_uint_t type,
    ngx_str_t *بهر)
{
    صحيح               i, len;
    X509             *cert;
    u_char           *alg, *s;
    EVP_PKEY         *pkey;
#إذا (OPENSSL_نسخه_شمېره >= 0x40000000L)
    ثابت
#د_که_پای
    X509_NAME        *subject;
#إذا (OPENSSL_نسخه_شمېره >= 0x40000000L)
    ثابت
#د_که_پای
    X509_NAME_ثبت  *ثبت;

    بدل (type) {

    حالة NGX_PROXY_PROTOCOL_TLV_SSL_نسخه:
        بهر->معلومات = (u_char *) SSL_get_نسخه(c->ssl->connection);
        بهر->len = ngx_strlen(بهر->معلومات);
        أرجع NGX_OK;

    حالة NGX_PROXY_PROTOCOL_TLV_SSL_CIPHER:
        بهر->معلومات = (u_char *) SSL_get_cipher_name(c->ssl->connection);
        بهر->len = ngx_strlen(بهر->معلومات);
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

        ثبت = X509_NAME_get_ثبت(subject, i);

        len = ASN1_STRING_منزل_UTF8(&s, X509_NAME_ثبت_get_معلومات(ثبت));
        إذا (len < 0) {
            ngx_ssl_error(NGX_LOG_ALERT, c->لوګاریتم, 0,
                          "ASN1_STRING_to_UTF8() failed");
            X509_free(cert);
            أرجع NGX_ERROR;
        }

        بهر->len = len;
        بهر->معلومات = ngx_pnalloc(c->pool, len);
        إذا (بهر->معلومات == عدم) {
            OPENSSL_free(s);
            X509_free(cert);
            أرجع NGX_ERROR;
        }

        ngx_memcpy(بهر->معلومات, s, len);

        OPENSSL_free(s);
        X509_free(cert);
        أرجع NGX_OK;

    حالة NGX_PROXY_PROTOCOL_TLV_SSL_SIG_ALG:
        cert = SSL_get_certificate(c->ssl->connection);

        بهر->معلومات = (u_char *) OBJ_nid2sn(X509_get_signature_nid(cert));
        بهر->len = ngx_strlen(بهر->معلومات);
        أرجع NGX_OK;

    حالة NGX_PROXY_PROTOCOL_TLV_SSL_کلی_ALG:
        cert = SSL_get_certificate(c->ssl->connection);

        pkey = X509_get_pubkey(cert);
        إذا (pkey == عدم) {
            أرجع NGX_ERROR;
        }

        بدل (EVP_PKEY_بنسټ_id(pkey)) {

        حالة EVP_PKEY_RSA:
#که_تعریف_شوی EVP_PKEY_RSA_PSS
        حالة EVP_PKEY_RSA_PSS:
#د_که_پای
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

        بهر->len = حجم("RSA16384") - 1;
        بهر->معلومات = ngx_pnalloc(c->pool, بهر->len);
        إذا (بهر->معلومات == عدم) {
            EVP_PKEY_free(pkey);
            أرجع NGX_ERROR;
        }

        بهر->len = ngx_snprintf(بهر->معلومات, بهر->len, "%s%d",
                                alg, EVP_PKEY_بټونه(pkey))
                   - بهر->معلومات;

        EVP_PKEY_free(pkey);

        أرجع NGX_OK;
    }

    أرجع NGX_DECLINED;
}

#د_که_پای


ساكن u_char *
ngx_proxy_protocol_v2_write_سرلیک(ngx_connection_t *c, u_char *buf,
    u_char *وروستی)
{
    u_char                            *p, src_af, dst_af,
                                       family, command, transport;
    ngx_uint_t                         src_fam, dst_fam;
    ngx_proxy_protocol_سرلیک_t       *سرلیک;
    ngx_proxy_protocol_inet_addrs_t   *دننه;
#إذا (NGX_HAVE_INET6)
    ngx_uint_t                         پورټ, lport;
    بنية in6_addr                   *src6, *dst6;
    ngx_proxy_protocol_inet6_addrs_t  *in6;
#د_که_پای

    إذا (وروستی - buf < NGX_PROXY_PROTOCOL_V2_MAX_سرلیک) {
        ngx_log_error(NGX_LOG_ALERT, c->لوګاریتم, 0,
                      "too small buffer for PROXY protocol v2");
        أرجع عدم;
    }

    إذا (ngx_connection_local_sockaddr(c, عدم, 0) != NGX_OK) {
        أرجع عدم;
    }

    /* the length field is set by the caller once all TLVs are written */

    سرلیک = (ngx_proxy_protocol_سرلیک_t *) buf;

    ngx_memcpy(سرلیک->signature, ngx_proxy_protocol_signature,
               حجم(ngx_proxy_protocol_signature) - 1);

    p = buf + حجم(ngx_proxy_protocol_سرلیک_t);

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
        حالة SOCK_بهیر:
            transport = NGX_PROXY_PROTOCOL_TYPE_بهیر;
            اكسر;
        حالة SOCK_DGRAM:
            transport = NGX_PROXY_PROTOCOL_TYPE_DGRAM;
            اكسر;
        افتراضي:
            transport = NGX_PROXY_PROTOCOL_TYPE_UNSPEC;
            اكسر;
        }
    }

    سرلیک->نسخه_command = 0x20 | command;
    سرلیک->family_transport = (family << 4) | transport;

    بدل (family) {

    حالة NGX_PROXY_PROTOCOL_AF_INET:

        دننه = (ngx_proxy_protocol_inet_addrs_t *) p;

        ngx_proxy_protocol_v2_write_ipv4(c->sockaddr,
                                         دننه->src_addr, دننه->src_پورټ);
        ngx_proxy_protocol_v2_write_ipv4(c->local_sockaddr,
                                         دننه->dst_addr, دننه->dst_پورټ);

        p += حجم(ngx_proxy_protocol_inet_addrs_t);

        اكسر;

#إذا (NGX_HAVE_INET6)

    حالة NGX_PROXY_PROTOCOL_AF_INET6:

        إذا (src_fam == AF_INET6 && dst_fam == AF_INET6) {
            src6 = &((بنية sockaddr_in6 *) c->sockaddr)->sin6_addr;
            dst6 = &((بنية sockaddr_in6 *) c->local_sockaddr)->sin6_addr;

            إذا (IN6_IS_ADDR_V4MAPPED(src6) && IN6_IS_ADDR_V4MAPPED(dst6)) {

                /* both v4-mapped: demote to AF_INET */

                سرلیک->family_transport = (NGX_PROXY_PROTOCOL_AF_INET << 4)
                                           | transport;

                دننه = (ngx_proxy_protocol_inet_addrs_t *) p;

                ngx_memcpy(دننه->src_addr, src6->s6_addr + 12, 4);
                ngx_memcpy(دننه->dst_addr, dst6->s6_addr + 12, 4);

                پورټ  = ngx_inet_get_پورټ(c->sockaddr);
                lport = ngx_inet_get_پورټ(c->local_sockaddr);

                دننه->src_پورټ[0] = (u_char) (پورټ >> 8);
                دننه->src_پورټ[1] = (u_char)  پورټ;
                دننه->dst_پورټ[0] = (u_char) (lport >> 8);
                دننه->dst_پورټ[1] = (u_char)  lport;

                p += حجم(ngx_proxy_protocol_inet_addrs_t);
                اكسر;
            }
        }

        in6 = (ngx_proxy_protocol_inet6_addrs_t *) p;

        ngx_proxy_protocol_v2_write_ipv6(c->sockaddr,
                                         in6->src_addr, in6->src_پورټ);
        ngx_proxy_protocol_v2_write_ipv6(c->local_sockaddr,
                                         in6->dst_addr, in6->dst_پورټ);

        p += حجم(ngx_proxy_protocol_inet6_addrs_t);

        اكسر;

#د_که_پای

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
#د_که_پای

    افتراضي:
        أرجع NGX_PROXY_PROTOCOL_AF_UNSPEC;
    }
}


ساكن فراغ
ngx_proxy_protocol_v2_write_ipv4(بنية sockaddr *sa, u_char *addr,
    u_char *پورټ)
{
    بنية sockaddr_دننه  *ساین;

    ساین = (بنية sockaddr_دننه *) sa;
    ngx_memcpy(addr, &ساین->sin_addr, 4);
    ngx_memcpy(پورټ, &ساین->sin_پورټ, 2);
}


#إذا (NGX_HAVE_INET6)

ساكن فراغ
ngx_proxy_protocol_v2_write_ipv6(بنية sockaddr *sa, u_char *addr,
    u_char *پورټ)
{
    بنية sockaddr_دننه   *ساین;
    بنية sockaddr_in6  *sin6;

    /* IPv4-mapped prefix: 80 zero bits + 16 one bits */
    ساكن ثابت u_char  mapped[]   = { 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0xff, 0xff };

    إذا (sa->sa_family == AF_INET6) {
        sin6 = (بنية sockaddr_in6 *) sa;
        ngx_memcpy(addr, &sin6->sin6_addr, 16);
        ngx_memcpy(پورټ, &sin6->sin6_پورټ, 2);

    } وإلا {
        /* promote AF_INET to ::ffff:a.b.c.d */
        ساین = (بنية sockaddr_دننه *) sa;
        ngx_memcpy(addr, mapped, حجم(mapped));
        ngx_memcpy(addr + حجم(mapped), &ساین->sin_addr, 4);
        ngx_memcpy(پورټ, &ساین->sin_پورټ, 2);
    }
}

#د_که_پای


ساكن u_char *
ngx_proxy_protocol_v2_write_tlv(ngx_connection_t *c, u_char *p, u_char *وروستی,
    ngx_uint_t type, ngx_str_t *ارزښت)
{
    ngx_proxy_protocol_tlv_t  *tlv;

    إذا ((اندازه_t) (وروستی - p) < حجم(ngx_proxy_protocol_tlv_t) + ارزښت->len) {
        ngx_log_error(NGX_LOG_ALERT, c->لوګاریتم, 0,
                      "too small buffer for PROXY protocol v2 TLV");
        أرجع عدم;
    }

    tlv = (ngx_proxy_protocol_tlv_t *) p;
    tlv->type = (u_char) type;
    tlv->len[0] = (u_char) (ارزښت->len >> 8);
    tlv->len[1] = (u_char)  ارزښت->len;

    p += حجم(ngx_proxy_protocol_tlv_t);

    أرجع ngx_cpymem(p, ارزښت->معلومات, ارزښت->len);
}


ساكن u_char *
ngx_proxy_protocol_v2_write_ssl(ngx_connection_t *c, u_char *p, u_char *وروستی,
    ngx_array_t *ssl_tlvs, ngx_uint_t client, uint32_t verify)
{
    u_char                          *پیل;
    uint16_t                         len;
    ngx_uint_t                       i;
    ngx_proxy_protocol_tlv_t        *tlv;
    ngx_proxy_protocol_tlv_ssl_t    *tlv_ssl;
    ngx_proxy_protocol_tlv_ارزښت_t  *sub;

    إذا ((اندازه_t) (وروستی - p) < حجم(ngx_proxy_protocol_tlv_t)
                              + حجم(ngx_proxy_protocol_tlv_ssl_t))
    {
        ngx_log_error(NGX_LOG_ALERT, c->لوګاریتم, 0,
                      "too small buffer for PROXY protocol v2 SSL TLV");
        أرجع عدم;
    }

    پیل = p;
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
        p = ngx_proxy_protocol_v2_write_tlv(c, p, وروستی, sub[i].type,
                                            &sub[i].ارزښت);
        إذا (p == عدم) {
            أرجع عدم;
        }
    }

    len = (uint16_t) (p - پیل - حجم(ngx_proxy_protocol_tlv_t));

    tlv = (ngx_proxy_protocol_tlv_t *) پیل;
    tlv->type = NGX_PROXY_PROTOCOL_TLV_SSL;
    tlv->len[0] = (u_char) (len >> 8);
    tlv->len[1] = (u_char)  len;

    أرجع p;
}


ساكن فراغ
ngx_proxy_protocol_v2_سېټ_len(u_char *buf, u_char *p)
{
    uint16_t                      len;
    ngx_proxy_protocol_سرلیک_t  *سرلیک;

    len = (uint16_t) (p - buf - حجم(ngx_proxy_protocol_سرلیک_t));

    سرلیک = (ngx_proxy_protocol_سرلیک_t *) buf;
    سرلیک->len[0] = (u_char) (len >> 8);
    سرلیک->len[1] = (u_char) len;
}


ساكن u_char *
ngx_proxy_protocol_v2_write_crc32c(ngx_connection_t *c, u_char *buf,
    u_char *p, u_char *وروستی)
{
    u_char                    *ارزښت;
    uint32_t                   دوراني_ازموینه;
    ngx_proxy_protocol_tlv_t  *tlv;

    إذا ((اندازه_t) (وروستی - p) < حجم(ngx_proxy_protocol_tlv_t) + 4) {
        ngx_log_error(NGX_LOG_ALERT, c->لوګاریتم, 0,
                      "too small buffer for PROXY protocol v2 CRC32c TLV");
        أرجع عدم;
    }

    /* append the CRC32c TLV with a zeroed value field */

    tlv = (ngx_proxy_protocol_tlv_t *) p;
    tlv->type = NGX_PROXY_PROTOCOL_TLV_دوراني_ازموینه_32_C;
    tlv->len[0] = 0;
    tlv->len[1] = 4;

    ارزښت = p + حجم(ngx_proxy_protocol_tlv_t);

    ngx_memzero(ارزښت, 4);

    p = ارزښت + 4;

    /* the checksum covers the entire header with the zeroed value */

    دوراني_ازموینه = ngx_crc32c_long(buf, p - buf);

    ارزښت[0] = (u_char) (دوراني_ازموینه >> 24);
    ارزښت[1] = (u_char) (دوراني_ازموینه >> 16);
    ارزښت[2] = (u_char) (دوراني_ازموینه >> 8);
    ارزښت[3] = (u_char) دوراني_ازموینه;

    أرجع p;
}
