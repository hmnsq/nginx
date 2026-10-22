
/*
 * Copyright (C) Roman Arutyunyan
 * Copyright (C) Nginx, Inc.
 */


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_PROXY_PROTOCOL_V2_MAX_ਸਿਰਲੇਖ    52

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_PROXY_PROTOCOL_CMD_LOCAL        0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_PROXY_PROTOCOL_CMD_PROXY        1

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_PROXY_PROTOCOL_AF_UNSPEC        0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_PROXY_PROTOCOL_AF_INET          1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_PROXY_PROTOCOL_AF_INET6         2

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_PROXY_PROTOCOL_TYPE_UNSPEC      0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_PROXY_PROTOCOL_TYPE_ਧਾਰਾ      1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_PROXY_PROTOCOL_TYPE_DGRAM       2

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_PROXY_PROTOCOL_TLV_ALPN         0x01
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_PROXY_PROTOCOL_TLV_AUTHORITY    0x02
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_PROXY_PROTOCOL_TLV_ਚੱਕਰੀ_ਜਾਂਚ_32_C       0x03
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_PROXY_PROTOCOL_TLV_UNIQUE_ID    0x05
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_PROXY_PROTOCOL_TLV_SSL          0x20
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_PROXY_PROTOCOL_TLV_SSL_ਸੰਸਕਰਣ  0x21
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_PROXY_PROTOCOL_TLV_SSL_CN       0x22
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_PROXY_PROTOCOL_TLV_SSL_CIPHER   0x23
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_PROXY_PROTOCOL_TLV_SSL_SIG_ALG  0x24
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_PROXY_PROTOCOL_TLV_SSL_ਕੁੰਜੀ_ALG  0x25
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_PROXY_PROTOCOL_TLV_NETNS        0x30

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_PROXY_PROTOCOL_V2_CLIENT_SSL          0x01
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_PROXY_PROTOCOL_V2_CLIENT_CERT_CONN    0x02
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_PROXY_PROTOCOL_V2_CLIENT_CERT_SESS    0x04


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_proxy_protocol_parse_uint16(p)                                    \
    ( ((uint16_t) (p)[0] << 8)                                                \
    + (           (p)[1]) )

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_proxy_protocol_parse_uint32(p)                                    \
    ( ((uint32_t) (p)[0] << 24)                                               \
    + (           (p)[1] << 16)                                               \
    + (           (p)[2] << 8)                                                \
    + (           (p)[3]) )


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    u_char                                  signature[12];
    u_char                                  ਸੰਸਕਰਣ_command;
    u_char                                  family_transport;
    u_char                                  len[2];
} ngx_proxy_protocol_ਸਿਰਲੇਖ_t;


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    u_char                                  src_addr[4];
    u_char                                  dst_addr[4];
    u_char                                  src_ਪੋਰਟ[2];
    u_char                                  dst_ਪੋਰਟ[2];
} ngx_proxy_protocol_inet_addrs_t;


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    u_char                                  src_addr[16];
    u_char                                  dst_addr[16];
    u_char                                  src_ਪੋਰਟ[2];
    u_char                                  dst_ਪੋਰਟ[2];
} ngx_proxy_protocol_inet6_addrs_t;


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    u_char                                  type;
    u_char                                  len[2];
} ngx_proxy_protocol_tlv_t;


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    u_char                                  client;
    u_char                                  verify[4];
} ngx_proxy_protocol_tlv_ssl_t;


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ngx_str_t                               name;
    ngx_uint_t                              type;
} ngx_proxy_protocol_tlv_ਇੰਦਰਾਜ_t;


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ngx_uint_t                              type;
    ngx_str_t                               ਮੁੱਲ;
} ngx_proxy_protocol_tlv_ਮੁੱਲ_t;


ਸਥਿਰ u_char *ngx_proxy_protocol_read_addr(ngx_connection_t *c, u_char *p,
    u_char *ਆਖਰੀ, ngx_str_t *addr);
ਸਥਿਰ u_char *ngx_proxy_protocol_read_ਪੋਰਟ(u_char *p, u_char *ਆਖਰੀ,
    ਵਿੱਚ_ਪੋਰਟ_t *ਪੋਰਟ, u_char sep);
ਸਥਿਰ u_char *ngx_proxy_protocol_v2_read(ngx_connection_t *c, u_char *buf,
    u_char *ਆਖਰੀ);
ਸਥਿਰ ngx_int_t ngx_proxy_protocol_lookup_tlv(ngx_connection_t *c,
    ngx_str_t *tlvs, ngx_uint_t type, ngx_str_t *ਮੁੱਲ);
#ਜੇ (NGX_OPENSSL)
ਸਥਿਰ ngx_int_t ngx_proxy_protocol_v2_eval_ssl(ngx_connection_t *c,
    ngx_array_t *tlvs, ngx_array_t *ssl_tlvs, ngx_uint_t *client,
    uint32_t *verify);
ਸਥਿਰ ngx_int_t ngx_proxy_protocol_v2_authority(ngx_connection_t *c,
    ngx_str_t *ਬਾਹਰਲਾ);
ਸਥਿਰ ngx_int_t ngx_proxy_protocol_v2_alpn(ngx_connection_t *c,
    ngx_str_t *ਬਾਹਰਲਾ);
ਸਥਿਰ ngx_int_t ngx_proxy_protocol_v2_ssl_sub(ngx_connection_t *c,
    ngx_uint_t type, ngx_str_t *ਬਾਹਰਲਾ);
#ਜੇ_ਅੰਤ
ਸਥਿਰ u_char *ngx_proxy_protocol_v2_write_ਸਿਰਲੇਖ(ngx_connection_t *c,
    u_char *buf, u_char *ਆਖਰੀ);
ਸਥਿਰ u_char ngx_proxy_protocol_v2_family(ngx_uint_t family);
ਸਥਿਰ ਖਾਲੀ ngx_proxy_protocol_v2_write_ipv4(ਬਣਤਰ sockaddr *sa, u_char *addr,
    u_char *ਪੋਰਟ);
ਸਥਿਰ ਖਾਲੀ ngx_proxy_protocol_v2_write_ipv6(ਬਣਤਰ sockaddr *sa, u_char *addr,
    u_char *ਪੋਰਟ);
ਸਥਿਰ u_char *ngx_proxy_protocol_v2_write_tlv(ngx_connection_t *c, u_char *p,
    u_char *ਆਖਰੀ, ngx_uint_t type, ngx_str_t *ਮੁੱਲ);
ਸਥਿਰ u_char *ngx_proxy_protocol_v2_write_ssl(ngx_connection_t *c, u_char *p,
    u_char *ਆਖਰੀ, ngx_array_t *ssl_tlvs, ngx_uint_t client, uint32_t verify);
ਸਥਿਰ ਖਾਲੀ ngx_proxy_protocol_v2_ਸੈੱਟ_len(u_char *buf, u_char *p);
ਸਥਿਰ u_char *ngx_proxy_protocol_v2_write_crc32c(ngx_connection_t *c,
    u_char *buf, u_char *p, u_char *ਆਖਰੀ);


ਸਥਿਰ ngx_proxy_protocol_tlv_ਇੰਦਰਾਜ_t  ngx_proxy_protocol_tlv_entries[] = {
    { ngx_string("alpn"),       NGX_PROXY_PROTOCOL_TLV_ALPN },
    { ngx_string("authority"),  NGX_PROXY_PROTOCOL_TLV_AUTHORITY },
    { ngx_string("unique_id"),  NGX_PROXY_PROTOCOL_TLV_UNIQUE_ID },
    { ngx_string("ssl"),        NGX_PROXY_PROTOCOL_TLV_SSL },
    { ngx_string("netns"),      NGX_PROXY_PROTOCOL_TLV_NETNS },
    { ngx_null_string,          0x00 }
};


ਸਥਿਰ ngx_proxy_protocol_tlv_ਇੰਦਰਾਜ_t  ngx_proxy_protocol_tlv_ssl_entries[] = {
    { ngx_string("version"),    NGX_PROXY_PROTOCOL_TLV_SSL_ਸੰਸਕਰਣ },
    { ngx_string("cn"),         NGX_PROXY_PROTOCOL_TLV_SSL_CN },
    { ngx_string("cipher"),     NGX_PROXY_PROTOCOL_TLV_SSL_CIPHER },
    { ngx_string("sig_alg"),    NGX_PROXY_PROTOCOL_TLV_SSL_SIG_ALG },
    { ngx_string("key_alg"),    NGX_PROXY_PROTOCOL_TLV_SSL_ਕੁੰਜੀ_ALG },
    { ngx_null_string,          0x00 }
};


ਸਥਿਰ ਅਚਲ u_char  ngx_proxy_protocol_signature[] = "\r\n\r\n\0\r\nQUIT\n";


u_char *
ngx_proxy_protocol_read(ngx_connection_t *c, u_char *buf, u_char *ਆਖਰੀ)
{
    ਮਾਪ_t                 len;
    u_char                *p;
    ngx_proxy_protocol_t  *pp;

    p = buf;
    len = ਆਖਰੀ - buf;

    ਜੇ (len >= ਆਕਾਰ(ngx_proxy_protocol_ਸਿਰਲੇਖ_t)
        && ngx_memcmp(p, ngx_proxy_protocol_signature,
                      ਆਕਾਰ(ngx_proxy_protocol_signature) - 1)
           == 0)
    {
        ਵਾਪਸ ngx_proxy_protocol_v2_read(c, buf, ਆਖਰੀ);
    }

    ਜੇ (len < 8 || ngx_strncmp(p, "PROXY ", 6) != 0) {
        ਜਾਓ invalid;
    }

    p += 6;
    len -= 6;

    ਜੇ (len >= 7 && ngx_strncmp(p, "UNKNOWN", 7) == 0) {
        ngx_log_debug0(NGX_LOG_DEBUG_CORE, c->ਲਘੂਗਣਕ, 0,
                       "PROXY protocol unknown protocol");
        p += 7;
        ਜਾਓ skip;
    }

    ਜੇ (len < 5 || ngx_strncmp(p, "TCP", 3) != 0
        || (p[3] != '4' && p[3] != '6') || p[4] != ' ')
    {
        ਜਾਓ invalid;
    }

    p += 5;

    pp = ngx_pcalloc(c->pool, ਆਕਾਰ(ngx_proxy_protocol_t));
    ਜੇ (pp == ਨਿਰਾ) {
        ਵਾਪਸ ਨਿਰਾ;
    }

    p = ngx_proxy_protocol_read_addr(c, p, ਆਖਰੀ, &pp->src_addr);
    ਜੇ (p == ਨਿਰਾ) {
        ਜਾਓ invalid;
    }

    p = ngx_proxy_protocol_read_addr(c, p, ਆਖਰੀ, &pp->dst_addr);
    ਜੇ (p == ਨਿਰਾ) {
        ਜਾਓ invalid;
    }

    p = ngx_proxy_protocol_read_ਪੋਰਟ(p, ਆਖਰੀ, &pp->src_ਪੋਰਟ, ' ');
    ਜੇ (p == ਨਿਰਾ) {
        ਜਾਓ invalid;
    }

    p = ngx_proxy_protocol_read_ਪੋਰਟ(p, ਆਖਰੀ, &pp->dst_ਪੋਰਟ, CR);
    ਜੇ (p == ਨਿਰਾ) {
        ਜਾਓ invalid;
    }

    ਜੇ (p == ਆਖਰੀ) {
        ਜਾਓ invalid;
    }

    ਜੇ (*p++ != LF) {
        ਜਾਓ invalid;
    }

    ngx_log_debug4(NGX_LOG_DEBUG_CORE, c->ਲਘੂਗਣਕ, 0,
                   "PROXY protocol src: %V %d, dst: %V %d",
                   &pp->src_addr, pp->src_ਪੋਰਟ, &pp->dst_addr, pp->dst_ਪੋਰਟ);

    c->proxy_protocol = pp;

    ਵਾਪਸ p;

skip:

    ਲਈ ( /* void */ ; p < ਆਖਰੀ - 1; p++) {
        ਜੇ (p[0] == CR && p[1] == LF) {
            ਵਾਪਸ p + 2;
        }
    }

invalid:

    ਲਈ (p = buf; p < ਆਖਰੀ; p++) {
        ਜੇ (*p == CR || *p == LF) {
            ਰੋਕੋ;
        }
    }

    ngx_log_error(NGX_LOG_ERR, c->ਲਘੂਗਣਕ, 0,
                  "broken header: \"%*s\"", (ਮਾਪ_t) (p - buf), buf);

    ਵਾਪਸ ਨਿਰਾ;
}


ਸਥਿਰ u_char *
ngx_proxy_protocol_read_addr(ngx_connection_t *c, u_char *p, u_char *ਆਖਰੀ,
    ngx_str_t *addr)
{
    ਮਾਪ_t  len;
    u_char  ch, *pos;

    pos = p;

    ਲਈ ( ;; ) {
        ਜੇ (p == ਆਖਰੀ) {
            ਵਾਪਸ ਨਿਰਾ;
        }

        ch = *p++;

        ਜੇ (ch == ' ') {
            ਰੋਕੋ;
        }

        ਜੇ (ch != ':' && ch != '.'
            && (ch < 'a' || ch > 'f')
            && (ch < 'A' || ch > 'F')
            && (ch < '0' || ch > '9'))
        {
            ਵਾਪਸ ਨਿਰਾ;
        }
    }

    len = p - pos - 1;

    addr->ਅੰਕੜੇ = ngx_pnalloc(c->pool, len);
    ਜੇ (addr->ਅੰਕੜੇ == ਨਿਰਾ) {
        ਵਾਪਸ ਨਿਰਾ;
    }

    ngx_memcpy(addr->ਅੰਕੜੇ, pos, len);
    addr->len = len;

    ਵਾਪਸ p;
}


ਸਥਿਰ u_char *
ngx_proxy_protocol_read_ਪੋਰਟ(u_char *p, u_char *ਆਖਰੀ, ਵਿੱਚ_ਪੋਰਟ_t *ਪੋਰਟ,
    u_char sep)
{
    ਮਾਪ_t      len;
    u_char     *pos;
    ngx_int_t   n;

    pos = p;

    ਲਈ ( ;; ) {
        ਜੇ (p == ਆਖਰੀ) {
            ਵਾਪਸ ਨਿਰਾ;
        }

        ਜੇ (*p++ == sep) {
            ਰੋਕੋ;
        }
    }

    len = p - pos - 1;

    n = ngx_atoi(pos, len);
    ਜੇ (n < 0 || n > 65535) {
        ਵਾਪਸ ਨਿਰਾ;
    }

    *ਪੋਰਟ = (ਵਿੱਚ_ਪੋਰਟ_t) n;

    ਵਾਪਸ p;
}


u_char *
ngx_proxy_protocol_write(ngx_connection_t *c, u_char *buf, u_char *ਆਖਰੀ)
{
    ngx_uint_t  ਪੋਰਟ, lport;

    ਜੇ (ਆਖਰੀ - buf < NGX_PROXY_PROTOCOL_V1_MAX_ਸਿਰਲੇਖ) {
        ngx_log_error(NGX_LOG_ALERT, c->ਲਘੂਗਣਕ, 0,
                      "too small buffer for PROXY protocol");
        ਵਾਪਸ ਨਿਰਾ;
    }

    ਜੇ (ngx_connection_local_sockaddr(c, ਨਿਰਾ, 0) != NGX_OK) {
        ਵਾਪਸ ਨਿਰਾ;
    }

    ਚੋਣ (c->sockaddr->sa_family) {

    ਹਾਲਤ AF_INET:
        buf = ngx_cpymem(buf, "PROXY TCP4 ", ਆਕਾਰ("PROXY TCP4 ") - 1);
        ਰੋਕੋ;

#ਜੇ (NGX_HAVE_INET6)
    ਹਾਲਤ AF_INET6:
        buf = ngx_cpymem(buf, "PROXY TCP6 ", ਆਕਾਰ("PROXY TCP6 ") - 1);
        ਰੋਕੋ;
#ਜੇ_ਅੰਤ

    ਮੂਲ:
        ਵਾਪਸ ngx_cpymem(buf, "PROXY UNKNOWN" CRLF,
                          ਆਕਾਰ("PROXY UNKNOWN" CRLF) - 1);
    }

    buf += ngx_sock_ntop(c->sockaddr, c->socklen, buf, ਆਖਰੀ - buf, 0);

    *buf++ = ' ';

    buf += ngx_sock_ntop(c->local_sockaddr, c->local_socklen, buf, ਆਖਰੀ - buf,
                         0);

    ਪੋਰਟ = ngx_inet_get_ਪੋਰਟ(c->sockaddr);
    lport = ngx_inet_get_ਪੋਰਟ(c->local_sockaddr);

    ਵਾਪਸ ngx_slprintf(buf, ਆਖਰੀ, " %ui %ui" CRLF, ਪੋਰਟ, lport);
}


ਸਥਿਰ u_char *
ngx_proxy_protocol_v2_read(ngx_connection_t *c, u_char *buf, u_char *ਆਖਰੀ)
{
    u_char                             *ਸਿਰਾ;
    ਮਾਪ_t                              len;
    socklen_t                           socklen;
    ngx_uint_t                          ਸੰਸਕਰਣ, command, family, transport;
    ngx_sockaddr_t                      src_sockaddr, dst_sockaddr;
    ngx_proxy_protocol_t               *pp;
    ngx_proxy_protocol_ਸਿਰਲੇਖ_t        *ਸਿਰਲੇਖ;
    ngx_proxy_protocol_inet_addrs_t    *ਵਿੱਚ;
#ਜੇ (NGX_HAVE_INET6)
    ngx_proxy_protocol_inet6_addrs_t   *in6;
#ਜੇ_ਅੰਤ

    ਸਿਰਲੇਖ = (ngx_proxy_protocol_ਸਿਰਲੇਖ_t *) buf;

    buf += ਆਕਾਰ(ngx_proxy_protocol_ਸਿਰਲੇਖ_t);

    ਸੰਸਕਰਣ = ਸਿਰਲੇਖ->ਸੰਸਕਰਣ_command >> 4;

    ਜੇ (ਸੰਸਕਰਣ != 2) {
        ngx_log_error(NGX_LOG_ERR, c->ਲਘੂਗਣਕ, 0,
                      "unknown PROXY protocol version: %ui", ਸੰਸਕਰਣ);
        ਵਾਪਸ ਨਿਰਾ;
    }

    len = ngx_proxy_protocol_parse_uint16(ਸਿਰਲੇਖ->len);

    ਜੇ ((ਮਾਪ_t) (ਆਖਰੀ - buf) < len) {
        ngx_log_error(NGX_LOG_ERR, c->ਲਘੂਗਣਕ, 0, "header is too large");
        ਵਾਪਸ ਨਿਰਾ;
    }

    ਸਿਰਾ = buf + len;

    command = ਸਿਰਲੇਖ->ਸੰਸਕਰਣ_command & 0x0f;

    /* only PROXY is supported */
    ਜੇ (command != NGX_PROXY_PROTOCOL_CMD_PROXY) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->ਲਘੂਗਣਕ, 0,
                       "PROXY protocol v2 unsupported command %ui", command);
        ਵਾਪਸ ਸਿਰਾ;
    }

    transport = ਸਿਰਲੇਖ->family_transport & 0x0f;

    /* only STREAM is supported */
    ਜੇ (transport != NGX_PROXY_PROTOCOL_TYPE_ਧਾਰਾ) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->ਲਘੂਗਣਕ, 0,
                       "PROXY protocol v2 unsupported transport %ui",
                       transport);
        ਵਾਪਸ ਸਿਰਾ;
    }

    pp = ngx_pcalloc(c->pool, ਆਕਾਰ(ngx_proxy_protocol_t));
    ਜੇ (pp == ਨਿਰਾ) {
        ਵਾਪਸ ਨਿਰਾ;
    }

    family = ਸਿਰਲੇਖ->family_transport >> 4;

    ਚੋਣ (family) {

    ਹਾਲਤ NGX_PROXY_PROTOCOL_AF_INET:

        ਜੇ ((ਮਾਪ_t) (ਸਿਰਾ - buf) < ਆਕਾਰ(ngx_proxy_protocol_inet_addrs_t)) {
            ਵਾਪਸ ਨਿਰਾ;
        }

        ਵਿੱਚ = (ngx_proxy_protocol_inet_addrs_t *) buf;

        src_sockaddr.sockaddr_ਵਿੱਚ.sin_family = AF_INET;
        src_sockaddr.sockaddr_ਵਿੱਚ.sin_ਪੋਰਟ = 0;
        ngx_memcpy(&src_sockaddr.sockaddr_ਵਿੱਚ.sin_addr, ਵਿੱਚ->src_addr, 4);

        dst_sockaddr.sockaddr_ਵਿੱਚ.sin_family = AF_INET;
        dst_sockaddr.sockaddr_ਵਿੱਚ.sin_ਪੋਰਟ = 0;
        ngx_memcpy(&dst_sockaddr.sockaddr_ਵਿੱਚ.sin_addr, ਵਿੱਚ->dst_addr, 4);

        pp->src_ਪੋਰਟ = ngx_proxy_protocol_parse_uint16(ਵਿੱਚ->src_ਪੋਰਟ);
        pp->dst_ਪੋਰਟ = ngx_proxy_protocol_parse_uint16(ਵਿੱਚ->dst_ਪੋਰਟ);

        socklen = ਆਕਾਰ(ਬਣਤਰ sockaddr_ਵਿੱਚ);

        buf += ਆਕਾਰ(ngx_proxy_protocol_inet_addrs_t);

        ਰੋਕੋ;

#ਜੇ (NGX_HAVE_INET6)

    ਹਾਲਤ NGX_PROXY_PROTOCOL_AF_INET6:

        ਜੇ ((ਮਾਪ_t) (ਸਿਰਾ - buf) < ਆਕਾਰ(ngx_proxy_protocol_inet6_addrs_t)) {
            ਵਾਪਸ ਨਿਰਾ;
        }

        in6 = (ngx_proxy_protocol_inet6_addrs_t *) buf;

        src_sockaddr.sockaddr_in6.sin6_family = AF_INET6;
        src_sockaddr.sockaddr_in6.sin6_ਪੋਰਟ = 0;
        ngx_memcpy(&src_sockaddr.sockaddr_in6.sin6_addr, in6->src_addr, 16);

        dst_sockaddr.sockaddr_in6.sin6_family = AF_INET6;
        dst_sockaddr.sockaddr_in6.sin6_ਪੋਰਟ = 0;
        ngx_memcpy(&dst_sockaddr.sockaddr_in6.sin6_addr, in6->dst_addr, 16);

        pp->src_ਪੋਰਟ = ngx_proxy_protocol_parse_uint16(in6->src_ਪੋਰਟ);
        pp->dst_ਪੋਰਟ = ngx_proxy_protocol_parse_uint16(in6->dst_ਪੋਰਟ);

        socklen = ਆਕਾਰ(ਬਣਤਰ sockaddr_in6);

        buf += ਆਕਾਰ(ngx_proxy_protocol_inet6_addrs_t);

        ਰੋਕੋ;

#ਜੇ_ਅੰਤ

    ਮੂਲ:
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->ਲਘੂਗਣਕ, 0,
                       "PROXY protocol v2 unsupported address family %ui",
                       family);
        ਵਾਪਸ ਸਿਰਾ;
    }

    pp->src_addr.ਅੰਕੜੇ = ngx_pnalloc(c->pool, NGX_SOCKADDR_STRLEN);
    ਜੇ (pp->src_addr.ਅੰਕੜੇ == ਨਿਰਾ) {
        ਵਾਪਸ ਨਿਰਾ;
    }

    pp->src_addr.len = ngx_sock_ntop(&src_sockaddr.sockaddr, socklen,
                                     pp->src_addr.ਅੰਕੜੇ, NGX_SOCKADDR_STRLEN, 0);

    pp->dst_addr.ਅੰਕੜੇ = ngx_pnalloc(c->pool, NGX_SOCKADDR_STRLEN);
    ਜੇ (pp->dst_addr.ਅੰਕੜੇ == ਨਿਰਾ) {
        ਵਾਪਸ ਨਿਰਾ;
    }

    pp->dst_addr.len = ngx_sock_ntop(&dst_sockaddr.sockaddr, socklen,
                                     pp->dst_addr.ਅੰਕੜੇ, NGX_SOCKADDR_STRLEN, 0);

    ngx_log_debug4(NGX_LOG_DEBUG_CORE, c->ਲਘੂਗਣਕ, 0,
                   "PROXY protocol v2 src: %V %d, dst: %V %d",
                   &pp->src_addr, pp->src_ਪੋਰਟ, &pp->dst_addr, pp->dst_ਪੋਰਟ);

    ਜੇ (buf < ਸਿਰਾ) {
        pp->tlvs.ਅੰਕੜੇ = ngx_pnalloc(c->pool, ਸਿਰਾ - buf);
        ਜੇ (pp->tlvs.ਅੰਕੜੇ == ਨਿਰਾ) {
            ਵਾਪਸ ਨਿਰਾ;
        }

        ngx_memcpy(pp->tlvs.ਅੰਕੜੇ, buf, ਸਿਰਾ - buf);
        pp->tlvs.len = ਸਿਰਾ - buf;
    }

    c->proxy_protocol = pp;

    ਵਾਪਸ ਸਿਰਾ;
}


ngx_int_t
ngx_proxy_protocol_get_tlv(ngx_connection_t *c, ngx_str_t *name,
    ngx_str_t *ਮੁੱਲ)
{
    u_char                          *p;
    ਮਾਪ_t                           n;
    uint32_t                         verify;
    ngx_str_t                        ssl, *tlvs;
    ngx_int_t                        rc, type;
    ngx_proxy_protocol_tlv_ssl_t    *tlv_ssl;
    ngx_proxy_protocol_tlv_ਇੰਦਰਾਜ_t  *te;

    ਜੇ (c->proxy_protocol == ਨਿਰਾ) {
        ਵਾਪਸ NGX_DECLINED;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->ਲਘੂਗਣਕ, 0,
                   "PROXY protocol v2 get tlv \"%V\"", name);

    te = ngx_proxy_protocol_tlv_entries;
    tlvs = &c->proxy_protocol->tlvs;

    p = name->ਅੰਕੜੇ;
    n = name->len;

    ਜੇ (n >= 4 && p[0] == 's' && p[1] == 's' && p[2] == 'l' && p[3] == '_') {

        rc = ngx_proxy_protocol_lookup_tlv(c, tlvs,
                                           NGX_PROXY_PROTOCOL_TLV_SSL, &ssl);
        ਜੇ (rc != NGX_OK) {
            ਵਾਪਸ rc;
        }

        ਜੇ (ssl.len < ਆਕਾਰ(ngx_proxy_protocol_tlv_ssl_t)) {
            ਵਾਪਸ NGX_ERROR;
        }

        p += 4;
        n -= 4;

        ਜੇ (n == 6 && ngx_strncmp(p, "verify", 6) == 0) {

            tlv_ssl = (ngx_proxy_protocol_tlv_ssl_t *) ssl.ਅੰਕੜੇ;
            verify = ngx_proxy_protocol_parse_uint32(tlv_ssl->verify);

            ਮੁੱਲ->ਅੰਕੜੇ = ngx_pnalloc(c->pool, NGX_INT32_LEN);
            ਜੇ (ਮੁੱਲ->ਅੰਕੜੇ == ਨਿਰਾ) {
                ਵਾਪਸ NGX_ERROR;
            }

            ਮੁੱਲ->len = ngx_sprintf(ਮੁੱਲ->ਅੰਕੜੇ, "%uD", verify)
                         - ਮੁੱਲ->ਅੰਕੜੇ;
            ਵਾਪਸ NGX_OK;
        }

        ssl.ਅੰਕੜੇ += ਆਕਾਰ(ngx_proxy_protocol_tlv_ssl_t);
        ssl.len -= ਆਕਾਰ(ngx_proxy_protocol_tlv_ssl_t);

        te = ngx_proxy_protocol_tlv_ssl_entries;
        tlvs = &ssl;
    }

    ਜੇ (n >= 2 && p[0] == '0' && p[1] == 'x') {

        type = ngx_hextoi(p + 2, n - 2);
        ਜੇ (type == NGX_ERROR) {
            ngx_log_error(NGX_LOG_ERR, c->ਲਘੂਗਣਕ, 0,
                          "invalid PROXY protocol TLV \"%V\"", name);
            ਵਾਪਸ NGX_ERROR;
        }

        ਵਾਪਸ ngx_proxy_protocol_lookup_tlv(c, tlvs, type, ਮੁੱਲ);
    }

    ਲਈ ( /* void */ ; te->type; te++) {
        ਜੇ (te->name.len == n && ngx_strncmp(te->name.ਅੰਕੜੇ, p, n) == 0) {
            ਵਾਪਸ ngx_proxy_protocol_lookup_tlv(c, tlvs, te->type, ਮੁੱਲ);
        }
    }

    ngx_log_error(NGX_LOG_ERR, c->ਲਘੂਗਣਕ, 0,
                  "unknown PROXY protocol TLV \"%V\"", name);

    ਵਾਪਸ NGX_DECLINED;
}


ਸਥਿਰ ngx_int_t
ngx_proxy_protocol_lookup_tlv(ngx_connection_t *c, ngx_str_t *tlvs,
    ngx_uint_t type, ngx_str_t *ਮੁੱਲ)
{
    u_char                    *p;
    ਮਾਪ_t                     n, len;
    ngx_proxy_protocol_tlv_t  *tlv;

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->ਲਘੂਗਣਕ, 0,
                   "PROXY protocol v2 lookup tlv:%02xi", type);

    p = tlvs->ਅੰਕੜੇ;
    n = tlvs->len;

    ਜਦੋਂ (n) {
        ਜੇ (n < ਆਕਾਰ(ngx_proxy_protocol_tlv_t)) {
            ngx_log_error(NGX_LOG_ERR, c->ਲਘੂਗਣਕ, 0, "broken PROXY protocol TLV");
            ਵਾਪਸ NGX_ERROR;
        }

        tlv = (ngx_proxy_protocol_tlv_t *) p;
        len = ngx_proxy_protocol_parse_uint16(tlv->len);

        p += ਆਕਾਰ(ngx_proxy_protocol_tlv_t);
        n -= ਆਕਾਰ(ngx_proxy_protocol_tlv_t);

        ਜੇ (n < len) {
            ngx_log_error(NGX_LOG_ERR, c->ਲਘੂਗਣਕ, 0, "broken PROXY protocol TLV");
            ਵਾਪਸ NGX_ERROR;
        }

        ਜੇ (tlv->type == type) {
            ਮੁੱਲ->ਅੰਕੜੇ = p;
            ਮੁੱਲ->len = len;
            ਵਾਪਸ NGX_OK;
        }

        p += len;
        n -= len;
    }

    ਵਾਪਸ NGX_DECLINED;
}


u_char *
ngx_proxy_protocol_v2_write(ngx_connection_t *c, u_char *buf, u_char *ਆਖਰੀ,
    ngx_array_t *tlvs /* reserved */ )
{
    u_char                          *p;
    uint32_t                         verify;
    ngx_uint_t                       i, client;
    ngx_array_t                      ਅਧਾਰ_tlvs, ssl_tlvs;
    ngx_proxy_protocol_tlv_ਮੁੱਲ_t  *tlv;

    client = 0;
    verify = 0;

    ngx_memzero(&ਅਧਾਰ_tlvs, ਆਕਾਰ(ngx_array_t));
    ngx_memzero(&ssl_tlvs, ਆਕਾਰ(ngx_array_t));

#ਜੇ (NGX_OPENSSL)

    ਜੇ (c->ssl != ਨਿਰਾ) {
        ਜੇ (ngx_array_init(&ਅਧਾਰ_tlvs, c->pool, 2,
                           ਆਕਾਰ(ngx_proxy_protocol_tlv_ਮੁੱਲ_t))
            != NGX_OK)
        {
            ਵਾਪਸ ਨਿਰਾ;
        }

        ਜੇ (ngx_array_init(&ssl_tlvs, c->pool, 5,
                           ਆਕਾਰ(ngx_proxy_protocol_tlv_ਮੁੱਲ_t))
            != NGX_OK)
        {
            ਵਾਪਸ ਨਿਰਾ;
        }

        ਜੇ (ngx_proxy_protocol_v2_eval_ssl(c, &ਅਧਾਰ_tlvs, &ssl_tlvs, &client,
                                           &verify)
            != NGX_OK)
        {
            ਵਾਪਸ ਨਿਰਾ;
        }
    }

#ਜੇ_ਅੰਤ

    p = ngx_proxy_protocol_v2_write_ਸਿਰਲੇਖ(c, buf, ਆਖਰੀ);
    ਜੇ (p == ਨਿਰਾ) {
        ਵਾਪਸ ਨਿਰਾ;
    }

    tlv = ਅਧਾਰ_tlvs.elts;

    ਲਈ (i = 0; i < ਅਧਾਰ_tlvs.nelts; i++) {
        p = ngx_proxy_protocol_v2_write_tlv(c, p, ਆਖਰੀ, tlv[i].type,
                                           &tlv[i].ਮੁੱਲ);
        ਜੇ (p == ਨਿਰਾ) {
            ਵਾਪਸ ਨਿਰਾ;
        }
    }

    ਜੇ (client) {
        p = ngx_proxy_protocol_v2_write_ssl(c, p, ਆਖਰੀ, &ssl_tlvs, client,
                                            verify);
        ਜੇ (p == ਨਿਰਾ) {
            ਵਾਪਸ ਨਿਰਾ;
        }
    }

    ngx_proxy_protocol_v2_ਸੈੱਟ_len(buf,
                                  p + ਆਕਾਰ(ngx_proxy_protocol_tlv_t) + 4);

    ਵਾਪਸ ngx_proxy_protocol_v2_write_crc32c(c, buf, p, ਆਖਰੀ);
}


#ਜੇ (NGX_OPENSSL)

ਸਥਿਰ ngx_int_t
ngx_proxy_protocol_v2_eval_ssl(ngx_connection_t *c, ngx_array_t *tlvs,
    ngx_array_t *ssl_tlvs, ngx_uint_t *client, uint32_t *verify)
{
    X509                            *cert;
    ਲੰਮਾ                             n;
    ngx_int_t                        rc;
    ngx_str_t                        ਮੁੱਲ;
    ngx_uint_t                       type;
    ਅਚਲ ਅੱਖਰ                      *s;
    ngx_proxy_protocol_tlv_ਮੁੱਲ_t  *tlv;

    ਜੇ (ngx_proxy_protocol_v2_authority(c, &ਮੁੱਲ) == NGX_OK) {
        tlv = ngx_array_ਧੱਕੋ(tlvs);
        ਜੇ (tlv == ਨਿਰਾ) {
            ਵਾਪਸ NGX_ERROR;
        }

        tlv->type = NGX_PROXY_PROTOCOL_TLV_AUTHORITY;
        tlv->ਮੁੱਲ = ਮੁੱਲ;
    }

    ਜੇ (ngx_proxy_protocol_v2_alpn(c, &ਮੁੱਲ) == NGX_OK) {
        tlv = ngx_array_ਧੱਕੋ(tlvs);
        ਜੇ (tlv == ਨਿਰਾ) {
            ਵਾਪਸ NGX_ERROR;
        }

        tlv->type = NGX_PROXY_PROTOCOL_TLV_ALPN;
        tlv->ਮੁੱਲ = ਮੁੱਲ;
    }

    ਲਈ (type = NGX_PROXY_PROTOCOL_TLV_SSL_ਸੰਸਕਰਣ;
         type <= NGX_PROXY_PROTOCOL_TLV_SSL_ਕੁੰਜੀ_ALG;
         type++)
    {
        rc = ngx_proxy_protocol_v2_ssl_sub(c, type, &ਮੁੱਲ);

        ਜੇ (rc == NGX_ERROR) {
            ਵਾਪਸ NGX_ERROR;
        }

        ਜੇ (rc != NGX_OK) {
            ਜਾਰੀ;
        }

        tlv = ngx_array_ਧੱਕੋ(ssl_tlvs);
        ਜੇ (tlv == ਨਿਰਾ) {
            ਵਾਪਸ NGX_ERROR;
        }

        tlv->type = type;
        tlv->ਮੁੱਲ = ਮੁੱਲ;
    }

    *client = NGX_PROXY_PROTOCOL_V2_CLIENT_SSL;
    *verify = 1;  /* X509_V_ERR_UNSPECIFIED */

    cert = SSL_get_peer_certificate(c->ssl->connection);

    ਜੇ (cert != ਨਿਰਾ) {
        X509_free(cert);

        *client |= NGX_PROXY_PROTOCOL_V2_CLIENT_CERT_SESS;

        ਜੇ (!SSL_session_reused(c->ssl->connection)) {
            *client |= NGX_PROXY_PROTOCOL_V2_CLIENT_CERT_CONN;
        }

        n = SSL_get_verify_ਨਤੀਜਾ(c->ssl->connection);

        ਜੇ (n == X509_V_OK) {

            ਜੇ (ngx_ssl_ocsp_get_ਸਥਿਤੀ(c, &s) != NGX_OK) {
                n = X509_V_ERR_CERT_REVOKED;
            }
        }

        *verify = (uint32_t) n;
    }

    ਵਾਪਸ NGX_OK;
}


ਸਥਿਰ ngx_int_t
ngx_proxy_protocol_v2_authority(ngx_connection_t *c, ngx_str_t *ਬਾਹਰਲਾ)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ SSL_CTRL_ਸੈੱਟ_TLSEXT_HOSTNAME

    ਅਚਲ ਅੱਖਰ  *sni;

    sni = SSL_get_servername(c->ssl->connection, TLSEXT_NAMETYPE_host_name);
    ਜੇ (sni == ਨਿਰਾ) {
        ਵਾਪਸ NGX_DECLINED;
    }

    ਬਾਹਰਲਾ->ਅੰਕੜੇ = (u_char *) sni;
    ਬਾਹਰਲਾ->len = ngx_strlen(sni);
    ਵਾਪਸ NGX_OK;

#ਨਹੀਂਤਾਂ
    ਵਾਪਸ NGX_DECLINED;
#ਜੇ_ਅੰਤ
}


ਸਥਿਰ ngx_int_t
ngx_proxy_protocol_v2_alpn(ngx_connection_t *c, ngx_str_t *ਬਾਹਰਲਾ)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ TLSEXT_TYPE_application_layer_protocol_negotiation

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ          alpnlen;
    ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ  *alpn;

    SSL_get0_alpn_selected(c->ssl->connection, &alpn, &alpnlen);
    ਜੇ (alpn == ਨਿਰਾ || alpnlen == 0) {
        ਵਾਪਸ NGX_DECLINED;
    }

    ਬਾਹਰਲਾ->ਅੰਕੜੇ = (u_char *) alpn;
    ਬਾਹਰਲਾ->len = alpnlen;
    ਵਾਪਸ NGX_OK;

#ਨਹੀਂਤਾਂ
    ਵਾਪਸ NGX_DECLINED;
#ਜੇ_ਅੰਤ
}


ਸਥਿਰ ngx_int_t
ngx_proxy_protocol_v2_ssl_sub(ngx_connection_t *c, ngx_uint_t type,
    ngx_str_t *ਬਾਹਰਲਾ)
{
    ਪੂਰਨ               i, len;
    X509             *cert;
    u_char           *alg, *s;
    EVP_PKEY         *pkey;
#ਜੇ (OPENSSL_ਸੰਸਕਰਣ_ਸੰਖਿਆ >= 0x40000000L)
    ਅਚਲ
#ਜੇ_ਅੰਤ
    X509_NAME        *subject;
#ਜੇ (OPENSSL_ਸੰਸਕਰਣ_ਸੰਖਿਆ >= 0x40000000L)
    ਅਚਲ
#ਜੇ_ਅੰਤ
    X509_NAME_ਇੰਦਰਾਜ  *ਇੰਦਰਾਜ;

    ਚੋਣ (type) {

    ਹਾਲਤ NGX_PROXY_PROTOCOL_TLV_SSL_ਸੰਸਕਰਣ:
        ਬਾਹਰਲਾ->ਅੰਕੜੇ = (u_char *) SSL_get_ਸੰਸਕਰਣ(c->ssl->connection);
        ਬਾਹਰਲਾ->len = ngx_strlen(ਬਾਹਰਲਾ->ਅੰਕੜੇ);
        ਵਾਪਸ NGX_OK;

    ਹਾਲਤ NGX_PROXY_PROTOCOL_TLV_SSL_CIPHER:
        ਬਾਹਰਲਾ->ਅੰਕੜੇ = (u_char *) SSL_get_cipher_name(c->ssl->connection);
        ਬਾਹਰਲਾ->len = ngx_strlen(ਬਾਹਰਲਾ->ਅੰਕੜੇ);
        ਵਾਪਸ NGX_OK;

    ਹਾਲਤ NGX_PROXY_PROTOCOL_TLV_SSL_CN:
        cert = SSL_get_peer_certificate(c->ssl->connection);
        ਜੇ (cert == ਨਿਰਾ) {
            ਵਾਪਸ NGX_DECLINED;
        }

        subject = X509_get_subject_name(cert);
        ਜੇ (subject == ਨਿਰਾ) {
            X509_free(cert);
            ਵਾਪਸ NGX_ERROR;
        }

        i = X509_NAME_get_ਸੂਚਕਾਂਕ_by_NID(subject, NID_commonName, -1);
        ਜੇ (i < 0) {
            X509_free(cert);
            ਵਾਪਸ NGX_DECLINED;
        }

        ਇੰਦਰਾਜ = X509_NAME_get_ਇੰਦਰਾਜ(subject, i);

        len = ASN1_STRING_ਮੰਜ਼ਿਲ_UTF8(&s, X509_NAME_ਇੰਦਰਾਜ_get_ਅੰਕੜੇ(ਇੰਦਰਾਜ));
        ਜੇ (len < 0) {
            ngx_ssl_error(NGX_LOG_ALERT, c->ਲਘੂਗਣਕ, 0,
                          "ASN1_STRING_to_UTF8() failed");
            X509_free(cert);
            ਵਾਪਸ NGX_ERROR;
        }

        ਬਾਹਰਲਾ->len = len;
        ਬਾਹਰਲਾ->ਅੰਕੜੇ = ngx_pnalloc(c->pool, len);
        ਜੇ (ਬਾਹਰਲਾ->ਅੰਕੜੇ == ਨਿਰਾ) {
            OPENSSL_free(s);
            X509_free(cert);
            ਵਾਪਸ NGX_ERROR;
        }

        ngx_memcpy(ਬਾਹਰਲਾ->ਅੰਕੜੇ, s, len);

        OPENSSL_free(s);
        X509_free(cert);
        ਵਾਪਸ NGX_OK;

    ਹਾਲਤ NGX_PROXY_PROTOCOL_TLV_SSL_SIG_ALG:
        cert = SSL_get_certificate(c->ssl->connection);

        ਬਾਹਰਲਾ->ਅੰਕੜੇ = (u_char *) OBJ_nid2sn(X509_get_signature_nid(cert));
        ਬਾਹਰਲਾ->len = ngx_strlen(ਬਾਹਰਲਾ->ਅੰਕੜੇ);
        ਵਾਪਸ NGX_OK;

    ਹਾਲਤ NGX_PROXY_PROTOCOL_TLV_SSL_ਕੁੰਜੀ_ALG:
        cert = SSL_get_certificate(c->ssl->connection);

        pkey = X509_get_pubkey(cert);
        ਜੇ (pkey == ਨਿਰਾ) {
            ਵਾਪਸ NGX_ERROR;
        }

        ਚੋਣ (EVP_PKEY_ਅਧਾਰ_id(pkey)) {

        ਹਾਲਤ EVP_PKEY_RSA:
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ EVP_PKEY_RSA_PSS
        ਹਾਲਤ EVP_PKEY_RSA_PSS:
#ਜੇ_ਅੰਤ
            alg = (u_char *) "RSA";
            ਰੋਕੋ;

        ਹਾਲਤ EVP_PKEY_EC:
            alg = (u_char *) "EC";
            ਰੋਕੋ;

        ਹਾਲਤ EVP_PKEY_DSA:
            alg = (u_char *) "DSA";
            ਰੋਕੋ;

        ਮੂਲ:
            EVP_PKEY_free(pkey);
            ਵਾਪਸ NGX_DECLINED;
        }

        ਬਾਹਰਲਾ->len = ਆਕਾਰ("RSA16384") - 1;
        ਬਾਹਰਲਾ->ਅੰਕੜੇ = ngx_pnalloc(c->pool, ਬਾਹਰਲਾ->len);
        ਜੇ (ਬਾਹਰਲਾ->ਅੰਕੜੇ == ਨਿਰਾ) {
            EVP_PKEY_free(pkey);
            ਵਾਪਸ NGX_ERROR;
        }

        ਬਾਹਰਲਾ->len = ngx_snprintf(ਬਾਹਰਲਾ->ਅੰਕੜੇ, ਬਾਹਰਲਾ->len, "%s%d",
                                alg, EVP_PKEY_ਬਿੱਟ(pkey))
                   - ਬਾਹਰਲਾ->ਅੰਕੜੇ;

        EVP_PKEY_free(pkey);

        ਵਾਪਸ NGX_OK;
    }

    ਵਾਪਸ NGX_DECLINED;
}

#ਜੇ_ਅੰਤ


ਸਥਿਰ u_char *
ngx_proxy_protocol_v2_write_ਸਿਰਲੇਖ(ngx_connection_t *c, u_char *buf,
    u_char *ਆਖਰੀ)
{
    u_char                            *p, src_af, dst_af,
                                       family, command, transport;
    ngx_uint_t                         src_fam, dst_fam;
    ngx_proxy_protocol_ਸਿਰਲੇਖ_t       *ਸਿਰਲੇਖ;
    ngx_proxy_protocol_inet_addrs_t   *ਵਿੱਚ;
#ਜੇ (NGX_HAVE_INET6)
    ngx_uint_t                         ਪੋਰਟ, lport;
    ਬਣਤਰ in6_addr                   *src6, *dst6;
    ngx_proxy_protocol_inet6_addrs_t  *in6;
#ਜੇ_ਅੰਤ

    ਜੇ (ਆਖਰੀ - buf < NGX_PROXY_PROTOCOL_V2_MAX_ਸਿਰਲੇਖ) {
        ngx_log_error(NGX_LOG_ALERT, c->ਲਘੂਗਣਕ, 0,
                      "too small buffer for PROXY protocol v2");
        ਵਾਪਸ ਨਿਰਾ;
    }

    ਜੇ (ngx_connection_local_sockaddr(c, ਨਿਰਾ, 0) != NGX_OK) {
        ਵਾਪਸ ਨਿਰਾ;
    }

    /* the length field is set by the caller once all TLVs are written */

    ਸਿਰਲੇਖ = (ngx_proxy_protocol_ਸਿਰਲੇਖ_t *) buf;

    ngx_memcpy(ਸਿਰਲੇਖ->signature, ngx_proxy_protocol_signature,
               ਆਕਾਰ(ngx_proxy_protocol_signature) - 1);

    p = buf + ਆਕਾਰ(ngx_proxy_protocol_ਸਿਰਲੇਖ_t);

    src_fam = c->sockaddr->sa_family;
    dst_fam = c->local_sockaddr->sa_family;

    src_af = ngx_proxy_protocol_v2_family(src_fam);
    dst_af = ngx_proxy_protocol_v2_family(dst_fam);

    /* promote to the highest address family present on either side */

    ਜੇ (src_af == NGX_PROXY_PROTOCOL_AF_UNSPEC
        || dst_af == NGX_PROXY_PROTOCOL_AF_UNSPEC)
    {
        command = NGX_PROXY_PROTOCOL_CMD_LOCAL;
        family = NGX_PROXY_PROTOCOL_AF_UNSPEC;
        transport = NGX_PROXY_PROTOCOL_TYPE_UNSPEC;

    } ਨਹੀਂਤਾਂ {
        command = NGX_PROXY_PROTOCOL_CMD_PROXY;
        family = ngx_max(src_af, dst_af);

        ਚੋਣ (c->type) {
        ਹਾਲਤ SOCK_ਧਾਰਾ:
            transport = NGX_PROXY_PROTOCOL_TYPE_ਧਾਰਾ;
            ਰੋਕੋ;
        ਹਾਲਤ SOCK_DGRAM:
            transport = NGX_PROXY_PROTOCOL_TYPE_DGRAM;
            ਰੋਕੋ;
        ਮੂਲ:
            transport = NGX_PROXY_PROTOCOL_TYPE_UNSPEC;
            ਰੋਕੋ;
        }
    }

    ਸਿਰਲੇਖ->ਸੰਸਕਰਣ_command = 0x20 | command;
    ਸਿਰਲੇਖ->family_transport = (family << 4) | transport;

    ਚੋਣ (family) {

    ਹਾਲਤ NGX_PROXY_PROTOCOL_AF_INET:

        ਵਿੱਚ = (ngx_proxy_protocol_inet_addrs_t *) p;

        ngx_proxy_protocol_v2_write_ipv4(c->sockaddr,
                                         ਵਿੱਚ->src_addr, ਵਿੱਚ->src_ਪੋਰਟ);
        ngx_proxy_protocol_v2_write_ipv4(c->local_sockaddr,
                                         ਵਿੱਚ->dst_addr, ਵਿੱਚ->dst_ਪੋਰਟ);

        p += ਆਕਾਰ(ngx_proxy_protocol_inet_addrs_t);

        ਰੋਕੋ;

#ਜੇ (NGX_HAVE_INET6)

    ਹਾਲਤ NGX_PROXY_PROTOCOL_AF_INET6:

        ਜੇ (src_fam == AF_INET6 && dst_fam == AF_INET6) {
            src6 = &((ਬਣਤਰ sockaddr_in6 *) c->sockaddr)->sin6_addr;
            dst6 = &((ਬਣਤਰ sockaddr_in6 *) c->local_sockaddr)->sin6_addr;

            ਜੇ (IN6_IS_ADDR_V4MAPPED(src6) && IN6_IS_ADDR_V4MAPPED(dst6)) {

                /* both v4-mapped: demote to AF_INET */

                ਸਿਰਲੇਖ->family_transport = (NGX_PROXY_PROTOCOL_AF_INET << 4)
                                           | transport;

                ਵਿੱਚ = (ngx_proxy_protocol_inet_addrs_t *) p;

                ngx_memcpy(ਵਿੱਚ->src_addr, src6->s6_addr + 12, 4);
                ngx_memcpy(ਵਿੱਚ->dst_addr, dst6->s6_addr + 12, 4);

                ਪੋਰਟ  = ngx_inet_get_ਪੋਰਟ(c->sockaddr);
                lport = ngx_inet_get_ਪੋਰਟ(c->local_sockaddr);

                ਵਿੱਚ->src_ਪੋਰਟ[0] = (u_char) (ਪੋਰਟ >> 8);
                ਵਿੱਚ->src_ਪੋਰਟ[1] = (u_char)  ਪੋਰਟ;
                ਵਿੱਚ->dst_ਪੋਰਟ[0] = (u_char) (lport >> 8);
                ਵਿੱਚ->dst_ਪੋਰਟ[1] = (u_char)  lport;

                p += ਆਕਾਰ(ngx_proxy_protocol_inet_addrs_t);
                ਰੋਕੋ;
            }
        }

        in6 = (ngx_proxy_protocol_inet6_addrs_t *) p;

        ngx_proxy_protocol_v2_write_ipv6(c->sockaddr,
                                         in6->src_addr, in6->src_ਪੋਰਟ);
        ngx_proxy_protocol_v2_write_ipv6(c->local_sockaddr,
                                         in6->dst_addr, in6->dst_ਪੋਰਟ);

        p += ਆਕਾਰ(ngx_proxy_protocol_inet6_addrs_t);

        ਰੋਕੋ;

#ਜੇ_ਅੰਤ

    ਮੂਲ:  /* NGX_PROXY_PROTOCOL_AF_UNSPEC */
        ਰੋਕੋ;
    }

    ਵਾਪਸ p;
}


ਸਥਿਰ u_char
ngx_proxy_protocol_v2_family(ngx_uint_t family)
{
    ਚੋਣ (family) {

    ਹਾਲਤ AF_INET:
        ਵਾਪਸ NGX_PROXY_PROTOCOL_AF_INET;

#ਜੇ (NGX_HAVE_INET6)
    ਹਾਲਤ AF_INET6:
        ਵਾਪਸ NGX_PROXY_PROTOCOL_AF_INET6;
#ਜੇ_ਅੰਤ

    ਮੂਲ:
        ਵਾਪਸ NGX_PROXY_PROTOCOL_AF_UNSPEC;
    }
}


ਸਥਿਰ ਖਾਲੀ
ngx_proxy_protocol_v2_write_ipv4(ਬਣਤਰ sockaddr *sa, u_char *addr,
    u_char *ਪੋਰਟ)
{
    ਬਣਤਰ sockaddr_ਵਿੱਚ  *ਜੀਵਾ;

    ਜੀਵਾ = (ਬਣਤਰ sockaddr_ਵਿੱਚ *) sa;
    ngx_memcpy(addr, &ਜੀਵਾ->sin_addr, 4);
    ngx_memcpy(ਪੋਰਟ, &ਜੀਵਾ->sin_ਪੋਰਟ, 2);
}


#ਜੇ (NGX_HAVE_INET6)

ਸਥਿਰ ਖਾਲੀ
ngx_proxy_protocol_v2_write_ipv6(ਬਣਤਰ sockaddr *sa, u_char *addr,
    u_char *ਪੋਰਟ)
{
    ਬਣਤਰ sockaddr_ਵਿੱਚ   *ਜੀਵਾ;
    ਬਣਤਰ sockaddr_in6  *sin6;

    /* IPv4-mapped prefix: 80 zero bits + 16 one bits */
    ਸਥਿਰ ਅਚਲ u_char  mapped[]   = { 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0xff, 0xff };

    ਜੇ (sa->sa_family == AF_INET6) {
        sin6 = (ਬਣਤਰ sockaddr_in6 *) sa;
        ngx_memcpy(addr, &sin6->sin6_addr, 16);
        ngx_memcpy(ਪੋਰਟ, &sin6->sin6_ਪੋਰਟ, 2);

    } ਨਹੀਂਤਾਂ {
        /* promote AF_INET to ::ffff:a.b.c.d */
        ਜੀਵਾ = (ਬਣਤਰ sockaddr_ਵਿੱਚ *) sa;
        ngx_memcpy(addr, mapped, ਆਕਾਰ(mapped));
        ngx_memcpy(addr + ਆਕਾਰ(mapped), &ਜੀਵਾ->sin_addr, 4);
        ngx_memcpy(ਪੋਰਟ, &ਜੀਵਾ->sin_ਪੋਰਟ, 2);
    }
}

#ਜੇ_ਅੰਤ


ਸਥਿਰ u_char *
ngx_proxy_protocol_v2_write_tlv(ngx_connection_t *c, u_char *p, u_char *ਆਖਰੀ,
    ngx_uint_t type, ngx_str_t *ਮੁੱਲ)
{
    ngx_proxy_protocol_tlv_t  *tlv;

    ਜੇ ((ਮਾਪ_t) (ਆਖਰੀ - p) < ਆਕਾਰ(ngx_proxy_protocol_tlv_t) + ਮੁੱਲ->len) {
        ngx_log_error(NGX_LOG_ALERT, c->ਲਘੂਗਣਕ, 0,
                      "too small buffer for PROXY protocol v2 TLV");
        ਵਾਪਸ ਨਿਰਾ;
    }

    tlv = (ngx_proxy_protocol_tlv_t *) p;
    tlv->type = (u_char) type;
    tlv->len[0] = (u_char) (ਮੁੱਲ->len >> 8);
    tlv->len[1] = (u_char)  ਮੁੱਲ->len;

    p += ਆਕਾਰ(ngx_proxy_protocol_tlv_t);

    ਵਾਪਸ ngx_cpymem(p, ਮੁੱਲ->ਅੰਕੜੇ, ਮੁੱਲ->len);
}


ਸਥਿਰ u_char *
ngx_proxy_protocol_v2_write_ssl(ngx_connection_t *c, u_char *p, u_char *ਆਖਰੀ,
    ngx_array_t *ssl_tlvs, ngx_uint_t client, uint32_t verify)
{
    u_char                          *ਸ਼ੁਰੂ;
    uint16_t                         len;
    ngx_uint_t                       i;
    ngx_proxy_protocol_tlv_t        *tlv;
    ngx_proxy_protocol_tlv_ssl_t    *tlv_ssl;
    ngx_proxy_protocol_tlv_ਮੁੱਲ_t  *sub;

    ਜੇ ((ਮਾਪ_t) (ਆਖਰੀ - p) < ਆਕਾਰ(ngx_proxy_protocol_tlv_t)
                              + ਆਕਾਰ(ngx_proxy_protocol_tlv_ssl_t))
    {
        ngx_log_error(NGX_LOG_ALERT, c->ਲਘੂਗਣਕ, 0,
                      "too small buffer for PROXY protocol v2 SSL TLV");
        ਵਾਪਸ ਨਿਰਾ;
    }

    ਸ਼ੁਰੂ = p;
    p += ਆਕਾਰ(ngx_proxy_protocol_tlv_t);

    tlv_ssl = (ngx_proxy_protocol_tlv_ssl_t *) p;
    tlv_ssl->client = (u_char) client;
    tlv_ssl->verify[0] = (u_char) (verify >> 24);
    tlv_ssl->verify[1] = (u_char) (verify >> 16);
    tlv_ssl->verify[2] = (u_char) (verify >> 8);
    tlv_ssl->verify[3] = (u_char)  verify;

    p += ਆਕਾਰ(ngx_proxy_protocol_tlv_ssl_t);

    sub = ssl_tlvs->elts;

    ਲਈ (i = 0; i < ssl_tlvs->nelts; i++) {
        p = ngx_proxy_protocol_v2_write_tlv(c, p, ਆਖਰੀ, sub[i].type,
                                            &sub[i].ਮੁੱਲ);
        ਜੇ (p == ਨਿਰਾ) {
            ਵਾਪਸ ਨਿਰਾ;
        }
    }

    len = (uint16_t) (p - ਸ਼ੁਰੂ - ਆਕਾਰ(ngx_proxy_protocol_tlv_t));

    tlv = (ngx_proxy_protocol_tlv_t *) ਸ਼ੁਰੂ;
    tlv->type = NGX_PROXY_PROTOCOL_TLV_SSL;
    tlv->len[0] = (u_char) (len >> 8);
    tlv->len[1] = (u_char)  len;

    ਵਾਪਸ p;
}


ਸਥਿਰ ਖਾਲੀ
ngx_proxy_protocol_v2_ਸੈੱਟ_len(u_char *buf, u_char *p)
{
    uint16_t                      len;
    ngx_proxy_protocol_ਸਿਰਲੇਖ_t  *ਸਿਰਲੇਖ;

    len = (uint16_t) (p - buf - ਆਕਾਰ(ngx_proxy_protocol_ਸਿਰਲੇਖ_t));

    ਸਿਰਲੇਖ = (ngx_proxy_protocol_ਸਿਰਲੇਖ_t *) buf;
    ਸਿਰਲੇਖ->len[0] = (u_char) (len >> 8);
    ਸਿਰਲੇਖ->len[1] = (u_char) len;
}


ਸਥਿਰ u_char *
ngx_proxy_protocol_v2_write_crc32c(ngx_connection_t *c, u_char *buf,
    u_char *p, u_char *ਆਖਰੀ)
{
    u_char                    *ਮੁੱਲ;
    uint32_t                   ਚੱਕਰੀ_ਜਾਂਚ;
    ngx_proxy_protocol_tlv_t  *tlv;

    ਜੇ ((ਮਾਪ_t) (ਆਖਰੀ - p) < ਆਕਾਰ(ngx_proxy_protocol_tlv_t) + 4) {
        ngx_log_error(NGX_LOG_ALERT, c->ਲਘੂਗਣਕ, 0,
                      "too small buffer for PROXY protocol v2 CRC32c TLV");
        ਵਾਪਸ ਨਿਰਾ;
    }

    /* append the CRC32c TLV with a zeroed value field */

    tlv = (ngx_proxy_protocol_tlv_t *) p;
    tlv->type = NGX_PROXY_PROTOCOL_TLV_ਚੱਕਰੀ_ਜਾਂਚ_32_C;
    tlv->len[0] = 0;
    tlv->len[1] = 4;

    ਮੁੱਲ = p + ਆਕਾਰ(ngx_proxy_protocol_tlv_t);

    ngx_memzero(ਮੁੱਲ, 4);

    p = ਮੁੱਲ + 4;

    /* the checksum covers the entire header with the zeroed value */

    ਚੱਕਰੀ_ਜਾਂਚ = ngx_crc32c_long(buf, p - buf);

    ਮੁੱਲ[0] = (u_char) (ਚੱਕਰੀ_ਜਾਂਚ >> 24);
    ਮੁੱਲ[1] = (u_char) (ਚੱਕਰੀ_ਜਾਂਚ >> 16);
    ਮੁੱਲ[2] = (u_char) (ਚੱਕਰੀ_ਜਾਂਚ >> 8);
    ਮੁੱਲ[3] = (u_char) ਚੱਕਰੀ_ਜਾਂਚ;

    ਵਾਪਸ p;
}
