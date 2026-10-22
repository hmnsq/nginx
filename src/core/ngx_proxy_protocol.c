
/*
 * Copyright (C) Roman Arutyunyan
 * Copyright (C) Nginx, Inc.
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


#ವ್ಯಾಖ್ಯಾನಿಸು NGX_PROXY_PROTOCOL_V2_MAX_ಶೀರ್ಷಿಕೆ    52

#ವ್ಯಾಖ್ಯಾನಿಸು NGX_PROXY_PROTOCOL_CMD_LOCAL        0
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_PROXY_PROTOCOL_CMD_PROXY        1

#ವ್ಯಾಖ್ಯಾನಿಸು NGX_PROXY_PROTOCOL_AF_UNSPEC        0
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_PROXY_PROTOCOL_AF_INET          1
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_PROXY_PROTOCOL_AF_INET6         2

#ವ್ಯಾಖ್ಯಾನಿಸು NGX_PROXY_PROTOCOL_TYPE_UNSPEC      0
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_PROXY_PROTOCOL_TYPE_ಹರಿವು      1
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_PROXY_PROTOCOL_TYPE_DGRAM       2

#ವ್ಯಾಖ್ಯಾನಿಸು NGX_PROXY_PROTOCOL_TLV_ALPN         0x01
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_PROXY_PROTOCOL_TLV_AUTHORITY    0x02
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_PROXY_PROTOCOL_TLV_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_32_C       0x03
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_PROXY_PROTOCOL_TLV_UNIQUE_ID    0x05
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_PROXY_PROTOCOL_TLV_SSL          0x20
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_PROXY_PROTOCOL_TLV_SSL_ಆವೃತ್ತಿ  0x21
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_PROXY_PROTOCOL_TLV_SSL_CN       0x22
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_PROXY_PROTOCOL_TLV_SSL_CIPHER   0x23
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_PROXY_PROTOCOL_TLV_SSL_SIG_ALG  0x24
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_PROXY_PROTOCOL_TLV_SSL_ಕೀಲಿ_ALG  0x25
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_PROXY_PROTOCOL_TLV_NETNS        0x30

#ವ್ಯಾಖ್ಯಾನಿಸು NGX_PROXY_PROTOCOL_V2_CLIENT_SSL          0x01
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_PROXY_PROTOCOL_V2_CLIENT_CERT_CONN    0x02
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_PROXY_PROTOCOL_V2_CLIENT_CERT_SESS    0x04


#ವ್ಯಾಖ್ಯಾನಿಸು ngx_proxy_protocol_parse_uint16(p)                                    \
    ( ((uint16_t) (p)[0] << 8)                                                \
    + (           (p)[1]) )

#ವ್ಯಾಖ್ಯಾನಿಸು ngx_proxy_protocol_parse_uint32(p)                                    \
    ( ((uint32_t) (p)[0] << 24)                                               \
    + (           (p)[1] << 16)                                               \
    + (           (p)[2] << 8)                                                \
    + (           (p)[3]) )


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    u_char                                  signature[12];
    u_char                                  ಆವೃತ್ತಿ_command;
    u_char                                  family_transport;
    u_char                                  len[2];
} ngx_proxy_protocol_ಶೀರ್ಷಿಕೆ_t;


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    u_char                                  src_addr[4];
    u_char                                  dst_addr[4];
    u_char                                  src_ಪೋರ್ಟ್[2];
    u_char                                  dst_ಪೋರ್ಟ್[2];
} ngx_proxy_protocol_inet_addrs_t;


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    u_char                                  src_addr[16];
    u_char                                  dst_addr[16];
    u_char                                  src_ಪೋರ್ಟ್[2];
    u_char                                  dst_ಪೋರ್ಟ್[2];
} ngx_proxy_protocol_inet6_addrs_t;


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    u_char                                  type;
    u_char                                  len[2];
} ngx_proxy_protocol_tlv_t;


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    u_char                                  client;
    u_char                                  verify[4];
} ngx_proxy_protocol_tlv_ssl_t;


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ngx_str_t                               name;
    ngx_uint_t                              type;
} ngx_proxy_protocol_tlv_ನಮೂದು_t;


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ngx_uint_t                              type;
    ngx_str_t                               ಮೌಲ್ಯ;
} ngx_proxy_protocol_tlv_ಮೌಲ್ಯ_t;


ಸ್ಥಿರ u_char *ngx_proxy_protocol_read_addr(ngx_connection_t *c, u_char *p,
    u_char *ಕೊನೆಯ, ngx_str_t *addr);
ಸ್ಥಿರ u_char *ngx_proxy_protocol_read_ಪೋರ್ಟ್(u_char *p, u_char *ಕೊನೆಯ,
    ಒಳ_ಪೋರ್ಟ್_t *ಪೋರ್ಟ್, u_char sep);
ಸ್ಥಿರ u_char *ngx_proxy_protocol_v2_read(ngx_connection_t *c, u_char *buf,
    u_char *ಕೊನೆಯ);
ಸ್ಥಿರ ngx_int_t ngx_proxy_protocol_lookup_tlv(ngx_connection_t *c,
    ngx_str_t *tlvs, ngx_uint_t type, ngx_str_t *ಮೌಲ್ಯ);
#ಆದರೆ (NGX_OPENSSL)
ಸ್ಥಿರ ngx_int_t ngx_proxy_protocol_v2_eval_ssl(ngx_connection_t *c,
    ngx_array_t *tlvs, ngx_array_t *ssl_tlvs, ngx_uint_t *client,
    uint32_t *verify);
ಸ್ಥಿರ ngx_int_t ngx_proxy_protocol_v2_authority(ngx_connection_t *c,
    ngx_str_t *ಹೊರಹೋಗು);
ಸ್ಥಿರ ngx_int_t ngx_proxy_protocol_v2_alpn(ngx_connection_t *c,
    ngx_str_t *ಹೊರಹೋಗು);
ಸ್ಥಿರ ngx_int_t ngx_proxy_protocol_v2_ssl_sub(ngx_connection_t *c,
    ngx_uint_t type, ngx_str_t *ಹೊರಹೋಗು);
#ಆದರೆ_ಅಂತ್ಯ
ಸ್ಥಿರ u_char *ngx_proxy_protocol_v2_write_ಶೀರ್ಷಿಕೆ(ngx_connection_t *c,
    u_char *buf, u_char *ಕೊನೆಯ);
ಸ್ಥಿರ u_char ngx_proxy_protocol_v2_family(ngx_uint_t family);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_proxy_protocol_v2_write_ipv4(ರಚನೆ sockaddr *sa, u_char *addr,
    u_char *ಪೋರ್ಟ್);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_proxy_protocol_v2_write_ipv6(ರಚನೆ sockaddr *sa, u_char *addr,
    u_char *ಪೋರ್ಟ್);
ಸ್ಥಿರ u_char *ngx_proxy_protocol_v2_write_tlv(ngx_connection_t *c, u_char *p,
    u_char *ಕೊನೆಯ, ngx_uint_t type, ngx_str_t *ಮೌಲ್ಯ);
ಸ್ಥಿರ u_char *ngx_proxy_protocol_v2_write_ssl(ngx_connection_t *c, u_char *p,
    u_char *ಕೊನೆಯ, ngx_array_t *ssl_tlvs, ngx_uint_t client, uint32_t verify);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_proxy_protocol_v2_ಗಣ_len(u_char *buf, u_char *p);
ಸ್ಥಿರ u_char *ngx_proxy_protocol_v2_write_crc32c(ngx_connection_t *c,
    u_char *buf, u_char *p, u_char *ಕೊನೆಯ);


ಸ್ಥಿರ ngx_proxy_protocol_tlv_ನಮೂದು_t  ngx_proxy_protocol_tlv_entries[] = {
    { ngx_string("alpn"),       NGX_PROXY_PROTOCOL_TLV_ALPN },
    { ngx_string("authority"),  NGX_PROXY_PROTOCOL_TLV_AUTHORITY },
    { ngx_string("unique_id"),  NGX_PROXY_PROTOCOL_TLV_UNIQUE_ID },
    { ngx_string("ssl"),        NGX_PROXY_PROTOCOL_TLV_SSL },
    { ngx_string("netns"),      NGX_PROXY_PROTOCOL_TLV_NETNS },
    { ngx_null_string,          0x00 }
};


ಸ್ಥಿರ ngx_proxy_protocol_tlv_ನಮೂದು_t  ngx_proxy_protocol_tlv_ssl_entries[] = {
    { ngx_string("version"),    NGX_PROXY_PROTOCOL_TLV_SSL_ಆವೃತ್ತಿ },
    { ngx_string("cn"),         NGX_PROXY_PROTOCOL_TLV_SSL_CN },
    { ngx_string("cipher"),     NGX_PROXY_PROTOCOL_TLV_SSL_CIPHER },
    { ngx_string("sig_alg"),    NGX_PROXY_PROTOCOL_TLV_SSL_SIG_ALG },
    { ngx_string("key_alg"),    NGX_PROXY_PROTOCOL_TLV_SSL_ಕೀಲಿ_ALG },
    { ngx_null_string,          0x00 }
};


ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ u_char  ngx_proxy_protocol_signature[] = "\r\n\r\n\0\r\nQUIT\n";


u_char *
ngx_proxy_protocol_read(ngx_connection_t *c, u_char *buf, u_char *ಕೊನೆಯ)
{
    ಗಾತ್ರ_t                 len;
    u_char                *p;
    ngx_proxy_protocol_t  *pp;

    p = buf;
    len = ಕೊನೆಯ - buf;

    ಆದರೆ (len >= ಗಾತ್ರ(ngx_proxy_protocol_ಶೀರ್ಷಿಕೆ_t)
        && ngx_memcmp(p, ngx_proxy_protocol_signature,
                      ಗಾತ್ರ(ngx_proxy_protocol_signature) - 1)
           == 0)
    {
        ಹಿಂತಿರುಗಿ ngx_proxy_protocol_v2_read(c, buf, ಕೊನೆಯ);
    }

    ಆದರೆ (len < 8 || ngx_strncmp(p, "PROXY ", 6) != 0) {
        ಹೋಗು invalid;
    }

    p += 6;
    len -= 6;

    ಆದರೆ (len >= 7 && ngx_strncmp(p, "UNKNOWN", 7) == 0) {
        ngx_log_debug0(NGX_LOG_DEBUG_CORE, c->ಲಘುಗಣಕ, 0,
                       "PROXY protocol unknown protocol");
        p += 7;
        ಹೋಗು skip;
    }

    ಆದರೆ (len < 5 || ngx_strncmp(p, "TCP", 3) != 0
        || (p[3] != '4' && p[3] != '6') || p[4] != ' ')
    {
        ಹೋಗು invalid;
    }

    p += 5;

    pp = ngx_pcalloc(c->pool, ಗಾತ್ರ(ngx_proxy_protocol_t));
    ಆದರೆ (pp == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    p = ngx_proxy_protocol_read_addr(c, p, ಕೊನೆಯ, &pp->src_addr);
    ಆದರೆ (p == ಖಾಲಿ) {
        ಹೋಗು invalid;
    }

    p = ngx_proxy_protocol_read_addr(c, p, ಕೊನೆಯ, &pp->dst_addr);
    ಆದರೆ (p == ಖಾಲಿ) {
        ಹೋಗು invalid;
    }

    p = ngx_proxy_protocol_read_ಪೋರ್ಟ್(p, ಕೊನೆಯ, &pp->src_ಪೋರ್ಟ್, ' ');
    ಆದರೆ (p == ಖಾಲಿ) {
        ಹೋಗು invalid;
    }

    p = ngx_proxy_protocol_read_ಪೋರ್ಟ್(p, ಕೊನೆಯ, &pp->dst_ಪೋರ್ಟ್, CR);
    ಆದರೆ (p == ಖಾಲಿ) {
        ಹೋಗು invalid;
    }

    ಆದರೆ (p == ಕೊನೆಯ) {
        ಹೋಗು invalid;
    }

    ಆದರೆ (*p++ != LF) {
        ಹೋಗು invalid;
    }

    ngx_log_debug4(NGX_LOG_DEBUG_CORE, c->ಲಘುಗಣಕ, 0,
                   "PROXY protocol src: %V %d, dst: %V %d",
                   &pp->src_addr, pp->src_ಪೋರ್ಟ್, &pp->dst_addr, pp->dst_ಪೋರ್ಟ್);

    c->proxy_protocol = pp;

    ಹಿಂತಿರುಗಿ p;

skip:

    ಗಾಗಿ ( /* void */ ; p < ಕೊನೆಯ - 1; p++) {
        ಆದರೆ (p[0] == CR && p[1] == LF) {
            ಹಿಂತಿರುಗಿ p + 2;
        }
    }

invalid:

    ಗಾಗಿ (p = buf; p < ಕೊನೆಯ; p++) {
        ಆದರೆ (*p == CR || *p == LF) {
            ನಿಲ್ಲಿಸು;
        }
    }

    ngx_log_error(NGX_LOG_ERR, c->ಲಘುಗಣಕ, 0,
                  "broken header: \"%*s\"", (ಗಾತ್ರ_t) (p - buf), buf);

    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}


ಸ್ಥಿರ u_char *
ngx_proxy_protocol_read_addr(ngx_connection_t *c, u_char *p, u_char *ಕೊನೆಯ,
    ngx_str_t *addr)
{
    ಗಾತ್ರ_t  len;
    u_char  ch, *pos;

    pos = p;

    ಗಾಗಿ ( ;; ) {
        ಆದರೆ (p == ಕೊನೆಯ) {
            ಹಿಂತಿರುಗಿ ಖಾಲಿ;
        }

        ch = *p++;

        ಆದರೆ (ch == ' ') {
            ನಿಲ್ಲಿಸು;
        }

        ಆದರೆ (ch != ':' && ch != '.'
            && (ch < 'a' || ch > 'f')
            && (ch < 'A' || ch > 'F')
            && (ch < '0' || ch > '9'))
        {
            ಹಿಂತಿರುಗಿ ಖಾಲಿ;
        }
    }

    len = p - pos - 1;

    addr->ದತ್ತಾಂಶ = ngx_pnalloc(c->pool, len);
    ಆದರೆ (addr->ದತ್ತಾಂಶ == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ngx_memcpy(addr->ದತ್ತಾಂಶ, pos, len);
    addr->len = len;

    ಹಿಂತಿರುಗಿ p;
}


ಸ್ಥಿರ u_char *
ngx_proxy_protocol_read_ಪೋರ್ಟ್(u_char *p, u_char *ಕೊನೆಯ, ಒಳ_ಪೋರ್ಟ್_t *ಪೋರ್ಟ್,
    u_char sep)
{
    ಗಾತ್ರ_t      len;
    u_char     *pos;
    ngx_int_t   n;

    pos = p;

    ಗಾಗಿ ( ;; ) {
        ಆದರೆ (p == ಕೊನೆಯ) {
            ಹಿಂತಿರುಗಿ ಖಾಲಿ;
        }

        ಆದರೆ (*p++ == sep) {
            ನಿಲ್ಲಿಸು;
        }
    }

    len = p - pos - 1;

    n = ngx_atoi(pos, len);
    ಆದರೆ (n < 0 || n > 65535) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    *ಪೋರ್ಟ್ = (ಒಳ_ಪೋರ್ಟ್_t) n;

    ಹಿಂತಿರುಗಿ p;
}


u_char *
ngx_proxy_protocol_write(ngx_connection_t *c, u_char *buf, u_char *ಕೊನೆಯ)
{
    ngx_uint_t  ಪೋರ್ಟ್, lport;

    ಆದರೆ (ಕೊನೆಯ - buf < NGX_PROXY_PROTOCOL_V1_MAX_ಶೀರ್ಷಿಕೆ) {
        ngx_log_error(NGX_LOG_ALERT, c->ಲಘುಗಣಕ, 0,
                      "too small buffer for PROXY protocol");
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ಆದರೆ (ngx_connection_local_sockaddr(c, ಖಾಲಿ, 0) != NGX_OK) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ಆಯ್ಕೆ (c->sockaddr->sa_family) {

    ಸ್ಥಿತಿ AF_INET:
        buf = ngx_cpymem(buf, "PROXY TCP4 ", ಗಾತ್ರ("PROXY TCP4 ") - 1);
        ನಿಲ್ಲಿಸು;

#ಆದರೆ (NGX_HAVE_INET6)
    ಸ್ಥಿತಿ AF_INET6:
        buf = ngx_cpymem(buf, "PROXY TCP6 ", ಗಾತ್ರ("PROXY TCP6 ") - 1);
        ನಿಲ್ಲಿಸು;
#ಆದರೆ_ಅಂತ್ಯ

    ಪೂರ್ವನಿಯೋಜಿತ:
        ಹಿಂತಿರುಗಿ ngx_cpymem(buf, "PROXY UNKNOWN" CRLF,
                          ಗಾತ್ರ("PROXY UNKNOWN" CRLF) - 1);
    }

    buf += ngx_sock_ntop(c->sockaddr, c->socklen, buf, ಕೊನೆಯ - buf, 0);

    *buf++ = ' ';

    buf += ngx_sock_ntop(c->local_sockaddr, c->local_socklen, buf, ಕೊನೆಯ - buf,
                         0);

    ಪೋರ್ಟ್ = ngx_inet_get_ಪೋರ್ಟ್(c->sockaddr);
    lport = ngx_inet_get_ಪೋರ್ಟ್(c->local_sockaddr);

    ಹಿಂತಿರುಗಿ ngx_slprintf(buf, ಕೊನೆಯ, " %ui %ui" CRLF, ಪೋರ್ಟ್, lport);
}


ಸ್ಥಿರ u_char *
ngx_proxy_protocol_v2_read(ngx_connection_t *c, u_char *buf, u_char *ಕೊನೆಯ)
{
    u_char                             *ಕೊನೆ;
    ಗಾತ್ರ_t                              len;
    socklen_t                           socklen;
    ngx_uint_t                          ಆವೃತ್ತಿ, command, family, transport;
    ngx_sockaddr_t                      src_sockaddr, dst_sockaddr;
    ngx_proxy_protocol_t               *pp;
    ngx_proxy_protocol_ಶೀರ್ಷಿಕೆ_t        *ಶೀರ್ಷಿಕೆ;
    ngx_proxy_protocol_inet_addrs_t    *ಒಳ;
#ಆದರೆ (NGX_HAVE_INET6)
    ngx_proxy_protocol_inet6_addrs_t   *in6;
#ಆದರೆ_ಅಂತ್ಯ

    ಶೀರ್ಷಿಕೆ = (ngx_proxy_protocol_ಶೀರ್ಷಿಕೆ_t *) buf;

    buf += ಗಾತ್ರ(ngx_proxy_protocol_ಶೀರ್ಷಿಕೆ_t);

    ಆವೃತ್ತಿ = ಶೀರ್ಷಿಕೆ->ಆವೃತ್ತಿ_command >> 4;

    ಆದರೆ (ಆವೃತ್ತಿ != 2) {
        ngx_log_error(NGX_LOG_ERR, c->ಲಘುಗಣಕ, 0,
                      "unknown PROXY protocol version: %ui", ಆವೃತ್ತಿ);
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    len = ngx_proxy_protocol_parse_uint16(ಶೀರ್ಷಿಕೆ->len);

    ಆದರೆ ((ಗಾತ್ರ_t) (ಕೊನೆಯ - buf) < len) {
        ngx_log_error(NGX_LOG_ERR, c->ಲಘುಗಣಕ, 0, "header is too large");
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ಕೊನೆ = buf + len;

    command = ಶೀರ್ಷಿಕೆ->ಆವೃತ್ತಿ_command & 0x0f;

    /* only PROXY is supported */
    ಆದರೆ (command != NGX_PROXY_PROTOCOL_CMD_PROXY) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->ಲಘುಗಣಕ, 0,
                       "PROXY protocol v2 unsupported command %ui", command);
        ಹಿಂತಿರುಗಿ ಕೊನೆ;
    }

    transport = ಶೀರ್ಷಿಕೆ->family_transport & 0x0f;

    /* only STREAM is supported */
    ಆದರೆ (transport != NGX_PROXY_PROTOCOL_TYPE_ಹರಿವು) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->ಲಘುಗಣಕ, 0,
                       "PROXY protocol v2 unsupported transport %ui",
                       transport);
        ಹಿಂತಿರುಗಿ ಕೊನೆ;
    }

    pp = ngx_pcalloc(c->pool, ಗಾತ್ರ(ngx_proxy_protocol_t));
    ಆದರೆ (pp == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    family = ಶೀರ್ಷಿಕೆ->family_transport >> 4;

    ಆಯ್ಕೆ (family) {

    ಸ್ಥಿತಿ NGX_PROXY_PROTOCOL_AF_INET:

        ಆದರೆ ((ಗಾತ್ರ_t) (ಕೊನೆ - buf) < ಗಾತ್ರ(ngx_proxy_protocol_inet_addrs_t)) {
            ಹಿಂತಿರುಗಿ ಖಾಲಿ;
        }

        ಒಳ = (ngx_proxy_protocol_inet_addrs_t *) buf;

        src_sockaddr.sockaddr_ಒಳ.sin_family = AF_INET;
        src_sockaddr.sockaddr_ಒಳ.sin_ಪೋರ್ಟ್ = 0;
        ngx_memcpy(&src_sockaddr.sockaddr_ಒಳ.sin_addr, ಒಳ->src_addr, 4);

        dst_sockaddr.sockaddr_ಒಳ.sin_family = AF_INET;
        dst_sockaddr.sockaddr_ಒಳ.sin_ಪೋರ್ಟ್ = 0;
        ngx_memcpy(&dst_sockaddr.sockaddr_ಒಳ.sin_addr, ಒಳ->dst_addr, 4);

        pp->src_ಪೋರ್ಟ್ = ngx_proxy_protocol_parse_uint16(ಒಳ->src_ಪೋರ್ಟ್);
        pp->dst_ಪೋರ್ಟ್ = ngx_proxy_protocol_parse_uint16(ಒಳ->dst_ಪೋರ್ಟ್);

        socklen = ಗಾತ್ರ(ರಚನೆ sockaddr_ಒಳ);

        buf += ಗಾತ್ರ(ngx_proxy_protocol_inet_addrs_t);

        ನಿಲ್ಲಿಸು;

#ಆದರೆ (NGX_HAVE_INET6)

    ಸ್ಥಿತಿ NGX_PROXY_PROTOCOL_AF_INET6:

        ಆದರೆ ((ಗಾತ್ರ_t) (ಕೊನೆ - buf) < ಗಾತ್ರ(ngx_proxy_protocol_inet6_addrs_t)) {
            ಹಿಂತಿರುಗಿ ಖಾಲಿ;
        }

        in6 = (ngx_proxy_protocol_inet6_addrs_t *) buf;

        src_sockaddr.sockaddr_in6.sin6_family = AF_INET6;
        src_sockaddr.sockaddr_in6.sin6_ಪೋರ್ಟ್ = 0;
        ngx_memcpy(&src_sockaddr.sockaddr_in6.sin6_addr, in6->src_addr, 16);

        dst_sockaddr.sockaddr_in6.sin6_family = AF_INET6;
        dst_sockaddr.sockaddr_in6.sin6_ಪೋರ್ಟ್ = 0;
        ngx_memcpy(&dst_sockaddr.sockaddr_in6.sin6_addr, in6->dst_addr, 16);

        pp->src_ಪೋರ್ಟ್ = ngx_proxy_protocol_parse_uint16(in6->src_ಪೋರ್ಟ್);
        pp->dst_ಪೋರ್ಟ್ = ngx_proxy_protocol_parse_uint16(in6->dst_ಪೋರ್ಟ್);

        socklen = ಗಾತ್ರ(ರಚನೆ sockaddr_in6);

        buf += ಗಾತ್ರ(ngx_proxy_protocol_inet6_addrs_t);

        ನಿಲ್ಲಿಸು;

#ಆದರೆ_ಅಂತ್ಯ

    ಪೂರ್ವನಿಯೋಜಿತ:
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->ಲಘುಗಣಕ, 0,
                       "PROXY protocol v2 unsupported address family %ui",
                       family);
        ಹಿಂತಿರುಗಿ ಕೊನೆ;
    }

    pp->src_addr.ದತ್ತಾಂಶ = ngx_pnalloc(c->pool, NGX_SOCKADDR_STRLEN);
    ಆದರೆ (pp->src_addr.ದತ್ತಾಂಶ == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    pp->src_addr.len = ngx_sock_ntop(&src_sockaddr.sockaddr, socklen,
                                     pp->src_addr.ದತ್ತಾಂಶ, NGX_SOCKADDR_STRLEN, 0);

    pp->dst_addr.ದತ್ತಾಂಶ = ngx_pnalloc(c->pool, NGX_SOCKADDR_STRLEN);
    ಆದರೆ (pp->dst_addr.ದತ್ತಾಂಶ == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    pp->dst_addr.len = ngx_sock_ntop(&dst_sockaddr.sockaddr, socklen,
                                     pp->dst_addr.ದತ್ತಾಂಶ, NGX_SOCKADDR_STRLEN, 0);

    ngx_log_debug4(NGX_LOG_DEBUG_CORE, c->ಲಘುಗಣಕ, 0,
                   "PROXY protocol v2 src: %V %d, dst: %V %d",
                   &pp->src_addr, pp->src_ಪೋರ್ಟ್, &pp->dst_addr, pp->dst_ಪೋರ್ಟ್);

    ಆದರೆ (buf < ಕೊನೆ) {
        pp->tlvs.ದತ್ತಾಂಶ = ngx_pnalloc(c->pool, ಕೊನೆ - buf);
        ಆದರೆ (pp->tlvs.ದತ್ತಾಂಶ == ಖಾಲಿ) {
            ಹಿಂತಿರುಗಿ ಖಾಲಿ;
        }

        ngx_memcpy(pp->tlvs.ದತ್ತಾಂಶ, buf, ಕೊನೆ - buf);
        pp->tlvs.len = ಕೊನೆ - buf;
    }

    c->proxy_protocol = pp;

    ಹಿಂತಿರುಗಿ ಕೊನೆ;
}


ngx_int_t
ngx_proxy_protocol_get_tlv(ngx_connection_t *c, ngx_str_t *name,
    ngx_str_t *ಮೌಲ್ಯ)
{
    u_char                          *p;
    ಗಾತ್ರ_t                           n;
    uint32_t                         verify;
    ngx_str_t                        ssl, *tlvs;
    ngx_int_t                        rc, type;
    ngx_proxy_protocol_tlv_ssl_t    *tlv_ssl;
    ngx_proxy_protocol_tlv_ನಮೂದು_t  *te;

    ಆದರೆ (c->proxy_protocol == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ NGX_DECLINED;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->ಲಘುಗಣಕ, 0,
                   "PROXY protocol v2 get tlv \"%V\"", name);

    te = ngx_proxy_protocol_tlv_entries;
    tlvs = &c->proxy_protocol->tlvs;

    p = name->ದತ್ತಾಂಶ;
    n = name->len;

    ಆದರೆ (n >= 4 && p[0] == 's' && p[1] == 's' && p[2] == 'l' && p[3] == '_') {

        rc = ngx_proxy_protocol_lookup_tlv(c, tlvs,
                                           NGX_PROXY_PROTOCOL_TLV_SSL, &ssl);
        ಆದರೆ (rc != NGX_OK) {
            ಹಿಂತಿರುಗಿ rc;
        }

        ಆದರೆ (ssl.len < ಗಾತ್ರ(ngx_proxy_protocol_tlv_ssl_t)) {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        p += 4;
        n -= 4;

        ಆದರೆ (n == 6 && ngx_strncmp(p, "verify", 6) == 0) {

            tlv_ssl = (ngx_proxy_protocol_tlv_ssl_t *) ssl.ದತ್ತಾಂಶ;
            verify = ngx_proxy_protocol_parse_uint32(tlv_ssl->verify);

            ಮೌಲ್ಯ->ದತ್ತಾಂಶ = ngx_pnalloc(c->pool, NGX_INT32_LEN);
            ಆದರೆ (ಮೌಲ್ಯ->ದತ್ತಾಂಶ == ಖಾಲಿ) {
                ಹಿಂತಿರುಗಿ NGX_ERROR;
            }

            ಮೌಲ್ಯ->len = ngx_sprintf(ಮೌಲ್ಯ->ದತ್ತಾಂಶ, "%uD", verify)
                         - ಮೌಲ್ಯ->ದತ್ತಾಂಶ;
            ಹಿಂತಿರುಗಿ NGX_OK;
        }

        ssl.ದತ್ತಾಂಶ += ಗಾತ್ರ(ngx_proxy_protocol_tlv_ssl_t);
        ssl.len -= ಗಾತ್ರ(ngx_proxy_protocol_tlv_ssl_t);

        te = ngx_proxy_protocol_tlv_ssl_entries;
        tlvs = &ssl;
    }

    ಆದರೆ (n >= 2 && p[0] == '0' && p[1] == 'x') {

        type = ngx_hextoi(p + 2, n - 2);
        ಆದರೆ (type == NGX_ERROR) {
            ngx_log_error(NGX_LOG_ERR, c->ಲಘುಗಣಕ, 0,
                          "invalid PROXY protocol TLV \"%V\"", name);
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        ಹಿಂತಿರುಗಿ ngx_proxy_protocol_lookup_tlv(c, tlvs, type, ಮೌಲ್ಯ);
    }

    ಗಾಗಿ ( /* void */ ; te->type; te++) {
        ಆದರೆ (te->name.len == n && ngx_strncmp(te->name.ದತ್ತಾಂಶ, p, n) == 0) {
            ಹಿಂತಿರುಗಿ ngx_proxy_protocol_lookup_tlv(c, tlvs, te->type, ಮೌಲ್ಯ);
        }
    }

    ngx_log_error(NGX_LOG_ERR, c->ಲಘುಗಣಕ, 0,
                  "unknown PROXY protocol TLV \"%V\"", name);

    ಹಿಂತಿರುಗಿ NGX_DECLINED;
}


ಸ್ಥಿರ ngx_int_t
ngx_proxy_protocol_lookup_tlv(ngx_connection_t *c, ngx_str_t *tlvs,
    ngx_uint_t type, ngx_str_t *ಮೌಲ್ಯ)
{
    u_char                    *p;
    ಗಾತ್ರ_t                     n, len;
    ngx_proxy_protocol_tlv_t  *tlv;

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->ಲಘುಗಣಕ, 0,
                   "PROXY protocol v2 lookup tlv:%02xi", type);

    p = tlvs->ದತ್ತಾಂಶ;
    n = tlvs->len;

    ಆಗ (n) {
        ಆದರೆ (n < ಗಾತ್ರ(ngx_proxy_protocol_tlv_t)) {
            ngx_log_error(NGX_LOG_ERR, c->ಲಘುಗಣಕ, 0, "broken PROXY protocol TLV");
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        tlv = (ngx_proxy_protocol_tlv_t *) p;
        len = ngx_proxy_protocol_parse_uint16(tlv->len);

        p += ಗಾತ್ರ(ngx_proxy_protocol_tlv_t);
        n -= ಗಾತ್ರ(ngx_proxy_protocol_tlv_t);

        ಆದರೆ (n < len) {
            ngx_log_error(NGX_LOG_ERR, c->ಲಘುಗಣಕ, 0, "broken PROXY protocol TLV");
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        ಆದರೆ (tlv->type == type) {
            ಮೌಲ್ಯ->ದತ್ತಾಂಶ = p;
            ಮೌಲ್ಯ->len = len;
            ಹಿಂತಿರುಗಿ NGX_OK;
        }

        p += len;
        n -= len;
    }

    ಹಿಂತಿರುಗಿ NGX_DECLINED;
}


u_char *
ngx_proxy_protocol_v2_write(ngx_connection_t *c, u_char *buf, u_char *ಕೊನೆಯ,
    ngx_array_t *tlvs /* reserved */ )
{
    u_char                          *p;
    uint32_t                         verify;
    ngx_uint_t                       i, client;
    ngx_array_t                      ತಳ_tlvs, ssl_tlvs;
    ngx_proxy_protocol_tlv_ಮೌಲ್ಯ_t  *tlv;

    client = 0;
    verify = 0;

    ngx_memzero(&ತಳ_tlvs, ಗಾತ್ರ(ngx_array_t));
    ngx_memzero(&ssl_tlvs, ಗಾತ್ರ(ngx_array_t));

#ಆದರೆ (NGX_OPENSSL)

    ಆದರೆ (c->ssl != ಖಾಲಿ) {
        ಆದರೆ (ngx_array_init(&ತಳ_tlvs, c->pool, 2,
                           ಗಾತ್ರ(ngx_proxy_protocol_tlv_ಮೌಲ್ಯ_t))
            != NGX_OK)
        {
            ಹಿಂತಿರುಗಿ ಖಾಲಿ;
        }

        ಆದರೆ (ngx_array_init(&ssl_tlvs, c->pool, 5,
                           ಗಾತ್ರ(ngx_proxy_protocol_tlv_ಮೌಲ್ಯ_t))
            != NGX_OK)
        {
            ಹಿಂತಿರುಗಿ ಖಾಲಿ;
        }

        ಆದರೆ (ngx_proxy_protocol_v2_eval_ssl(c, &ತಳ_tlvs, &ssl_tlvs, &client,
                                           &verify)
            != NGX_OK)
        {
            ಹಿಂತಿರುಗಿ ಖಾಲಿ;
        }
    }

#ಆದರೆ_ಅಂತ್ಯ

    p = ngx_proxy_protocol_v2_write_ಶೀರ್ಷಿಕೆ(c, buf, ಕೊನೆಯ);
    ಆದರೆ (p == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    tlv = ತಳ_tlvs.elts;

    ಗಾಗಿ (i = 0; i < ತಳ_tlvs.nelts; i++) {
        p = ngx_proxy_protocol_v2_write_tlv(c, p, ಕೊನೆಯ, tlv[i].type,
                                           &tlv[i].ಮೌಲ್ಯ);
        ಆದರೆ (p == ಖಾಲಿ) {
            ಹಿಂತಿರುಗಿ ಖಾಲಿ;
        }
    }

    ಆದರೆ (client) {
        p = ngx_proxy_protocol_v2_write_ssl(c, p, ಕೊನೆಯ, &ssl_tlvs, client,
                                            verify);
        ಆದರೆ (p == ಖಾಲಿ) {
            ಹಿಂತಿರುಗಿ ಖಾಲಿ;
        }
    }

    ngx_proxy_protocol_v2_ಗಣ_len(buf,
                                  p + ಗಾತ್ರ(ngx_proxy_protocol_tlv_t) + 4);

    ಹಿಂತಿರುಗಿ ngx_proxy_protocol_v2_write_crc32c(c, buf, p, ಕೊನೆಯ);
}


#ಆದರೆ (NGX_OPENSSL)

ಸ್ಥಿರ ngx_int_t
ngx_proxy_protocol_v2_eval_ssl(ngx_connection_t *c, ngx_array_t *tlvs,
    ngx_array_t *ssl_tlvs, ngx_uint_t *client, uint32_t *verify)
{
    X509                            *cert;
    ದೀರ್ಘ                             n;
    ngx_int_t                        rc;
    ngx_str_t                        ಮೌಲ್ಯ;
    ngx_uint_t                       type;
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ                      *s;
    ngx_proxy_protocol_tlv_ಮೌಲ್ಯ_t  *tlv;

    ಆದರೆ (ngx_proxy_protocol_v2_authority(c, &ಮೌಲ್ಯ) == NGX_OK) {
        tlv = ngx_array_ತಳ್ಳು(tlvs);
        ಆದರೆ (tlv == ಖಾಲಿ) {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        tlv->type = NGX_PROXY_PROTOCOL_TLV_AUTHORITY;
        tlv->ಮೌಲ್ಯ = ಮೌಲ್ಯ;
    }

    ಆದರೆ (ngx_proxy_protocol_v2_alpn(c, &ಮೌಲ್ಯ) == NGX_OK) {
        tlv = ngx_array_ತಳ್ಳು(tlvs);
        ಆದರೆ (tlv == ಖಾಲಿ) {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        tlv->type = NGX_PROXY_PROTOCOL_TLV_ALPN;
        tlv->ಮೌಲ್ಯ = ಮೌಲ್ಯ;
    }

    ಗಾಗಿ (type = NGX_PROXY_PROTOCOL_TLV_SSL_ಆವೃತ್ತಿ;
         type <= NGX_PROXY_PROTOCOL_TLV_SSL_ಕೀಲಿ_ALG;
         type++)
    {
        rc = ngx_proxy_protocol_v2_ssl_sub(c, type, &ಮೌಲ್ಯ);

        ಆದರೆ (rc == NGX_ERROR) {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        ಆದರೆ (rc != NGX_OK) {
            ಮುಂದುವರಿ;
        }

        tlv = ngx_array_ತಳ್ಳು(ssl_tlvs);
        ಆದರೆ (tlv == ಖಾಲಿ) {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        tlv->type = type;
        tlv->ಮೌಲ್ಯ = ಮೌಲ್ಯ;
    }

    *client = NGX_PROXY_PROTOCOL_V2_CLIENT_SSL;
    *verify = 1;  /* X509_V_ERR_UNSPECIFIED */

    cert = SSL_get_peer_certificate(c->ssl->connection);

    ಆದರೆ (cert != ಖಾಲಿ) {
        X509_free(cert);

        *client |= NGX_PROXY_PROTOCOL_V2_CLIENT_CERT_SESS;

        ಆದರೆ (!SSL_session_reused(c->ssl->connection)) {
            *client |= NGX_PROXY_PROTOCOL_V2_CLIENT_CERT_CONN;
        }

        n = SSL_get_verify_ಫಲಿತಾಂಶ(c->ssl->connection);

        ಆದರೆ (n == X509_V_OK) {

            ಆದರೆ (ngx_ssl_ocsp_get_ಸ್ಥಿತಿ(c, &s) != NGX_OK) {
                n = X509_V_ERR_CERT_REVOKED;
            }
        }

        *verify = (uint32_t) n;
    }

    ಹಿಂತಿರುಗಿ NGX_OK;
}


ಸ್ಥಿರ ngx_int_t
ngx_proxy_protocol_v2_authority(ngx_connection_t *c, ngx_str_t *ಹೊರಹೋಗು)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ SSL_CTRL_ಗಣ_TLSEXT_HOSTNAME

    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ  *sni;

    sni = SSL_get_servername(c->ssl->connection, TLSEXT_NAMETYPE_host_name);
    ಆದರೆ (sni == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ NGX_DECLINED;
    }

    ಹೊರಹೋಗು->ದತ್ತಾಂಶ = (u_char *) sni;
    ಹೊರಹೋಗು->len = ngx_strlen(sni);
    ಹಿಂತಿರುಗಿ NGX_OK;

#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ NGX_DECLINED;
#ಆದರೆ_ಅಂತ್ಯ
}


ಸ್ಥಿರ ngx_int_t
ngx_proxy_protocol_v2_alpn(ngx_connection_t *c, ngx_str_t *ಹೊರಹೋಗು)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ TLSEXT_TYPE_application_layer_protocol_negotiation

    ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ          alpnlen;
    ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ  *alpn;

    SSL_get0_alpn_selected(c->ssl->connection, &alpn, &alpnlen);
    ಆದರೆ (alpn == ಖಾಲಿ || alpnlen == 0) {
        ಹಿಂತಿರುಗಿ NGX_DECLINED;
    }

    ಹೊರಹೋಗು->ದತ್ತಾಂಶ = (u_char *) alpn;
    ಹೊರಹೋಗು->len = alpnlen;
    ಹಿಂತಿರುಗಿ NGX_OK;

#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ NGX_DECLINED;
#ಆದರೆ_ಅಂತ್ಯ
}


ಸ್ಥಿರ ngx_int_t
ngx_proxy_protocol_v2_ssl_sub(ngx_connection_t *c, ngx_uint_t type,
    ngx_str_t *ಹೊರಹೋಗು)
{
    ಪೂರ್ಣಾಂಕ               i, len;
    X509             *cert;
    u_char           *alg, *s;
    EVP_PKEY         *pkey;
#ಆದರೆ (OPENSSL_ಆವೃತ್ತಿ_ಸಂಖ್ಯೆ >= 0x40000000L)
    ಸ್ಥಿರಾಂಕ
#ಆದರೆ_ಅಂತ್ಯ
    X509_NAME        *subject;
#ಆದರೆ (OPENSSL_ಆವೃತ್ತಿ_ಸಂಖ್ಯೆ >= 0x40000000L)
    ಸ್ಥಿರಾಂಕ
#ಆದರೆ_ಅಂತ್ಯ
    X509_NAME_ನಮೂದು  *ನಮೂದು;

    ಆಯ್ಕೆ (type) {

    ಸ್ಥಿತಿ NGX_PROXY_PROTOCOL_TLV_SSL_ಆವೃತ್ತಿ:
        ಹೊರಹೋಗು->ದತ್ತಾಂಶ = (u_char *) SSL_get_ಆವೃತ್ತಿ(c->ssl->connection);
        ಹೊರಹೋಗು->len = ngx_strlen(ಹೊರಹೋಗು->ದತ್ತಾಂಶ);
        ಹಿಂತಿರುಗಿ NGX_OK;

    ಸ್ಥಿತಿ NGX_PROXY_PROTOCOL_TLV_SSL_CIPHER:
        ಹೊರಹೋಗು->ದತ್ತಾಂಶ = (u_char *) SSL_get_cipher_name(c->ssl->connection);
        ಹೊರಹೋಗು->len = ngx_strlen(ಹೊರಹೋಗು->ದತ್ತಾಂಶ);
        ಹಿಂತಿರುಗಿ NGX_OK;

    ಸ್ಥಿತಿ NGX_PROXY_PROTOCOL_TLV_SSL_CN:
        cert = SSL_get_peer_certificate(c->ssl->connection);
        ಆದರೆ (cert == ಖಾಲಿ) {
            ಹಿಂತಿರುಗಿ NGX_DECLINED;
        }

        subject = X509_get_subject_name(cert);
        ಆದರೆ (subject == ಖಾಲಿ) {
            X509_free(cert);
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        i = X509_NAME_get_ಸೂಚ್ಯಂಕ_by_NID(subject, NID_commonName, -1);
        ಆದರೆ (i < 0) {
            X509_free(cert);
            ಹಿಂತಿರುಗಿ NGX_DECLINED;
        }

        ನಮೂದು = X509_NAME_get_ನಮೂದು(subject, i);

        len = ASN1_STRING_ಗಮ್ಯ_UTF8(&s, X509_NAME_ನಮೂದು_get_ದತ್ತಾಂಶ(ನಮೂದು));
        ಆದರೆ (len < 0) {
            ngx_ssl_error(NGX_LOG_ALERT, c->ಲಘುಗಣಕ, 0,
                          "ASN1_STRING_to_UTF8() failed");
            X509_free(cert);
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        ಹೊರಹೋಗು->len = len;
        ಹೊರಹೋಗು->ದತ್ತಾಂಶ = ngx_pnalloc(c->pool, len);
        ಆದರೆ (ಹೊರಹೋಗು->ದತ್ತಾಂಶ == ಖಾಲಿ) {
            OPENSSL_free(s);
            X509_free(cert);
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        ngx_memcpy(ಹೊರಹೋಗು->ದತ್ತಾಂಶ, s, len);

        OPENSSL_free(s);
        X509_free(cert);
        ಹಿಂತಿರುಗಿ NGX_OK;

    ಸ್ಥಿತಿ NGX_PROXY_PROTOCOL_TLV_SSL_SIG_ALG:
        cert = SSL_get_certificate(c->ssl->connection);

        ಹೊರಹೋಗು->ದತ್ತಾಂಶ = (u_char *) OBJ_nid2sn(X509_get_signature_nid(cert));
        ಹೊರಹೋಗು->len = ngx_strlen(ಹೊರಹೋಗು->ದತ್ತಾಂಶ);
        ಹಿಂತಿರುಗಿ NGX_OK;

    ಸ್ಥಿತಿ NGX_PROXY_PROTOCOL_TLV_SSL_ಕೀಲಿ_ALG:
        cert = SSL_get_certificate(c->ssl->connection);

        pkey = X509_get_pubkey(cert);
        ಆದರೆ (pkey == ಖಾಲಿ) {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        ಆಯ್ಕೆ (EVP_PKEY_ತಳ_id(pkey)) {

        ಸ್ಥಿತಿ EVP_PKEY_RSA:
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ EVP_PKEY_RSA_PSS
        ಸ್ಥಿತಿ EVP_PKEY_RSA_PSS:
#ಆದರೆ_ಅಂತ್ಯ
            alg = (u_char *) "RSA";
            ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ EVP_PKEY_EC:
            alg = (u_char *) "EC";
            ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ EVP_PKEY_DSA:
            alg = (u_char *) "DSA";
            ನಿಲ್ಲಿಸು;

        ಪೂರ್ವನಿಯೋಜಿತ:
            EVP_PKEY_free(pkey);
            ಹಿಂತಿರುಗಿ NGX_DECLINED;
        }

        ಹೊರಹೋಗು->len = ಗಾತ್ರ("RSA16384") - 1;
        ಹೊರಹೋಗು->ದತ್ತಾಂಶ = ngx_pnalloc(c->pool, ಹೊರಹೋಗು->len);
        ಆದರೆ (ಹೊರಹೋಗು->ದತ್ತಾಂಶ == ಖಾಲಿ) {
            EVP_PKEY_free(pkey);
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        ಹೊರಹೋಗು->len = ngx_snprintf(ಹೊರಹೋಗು->ದತ್ತಾಂಶ, ಹೊರಹೋಗು->len, "%s%d",
                                alg, EVP_PKEY_ಬಿಟ್ಟುಗಳು(pkey))
                   - ಹೊರಹೋಗು->ದತ್ತಾಂಶ;

        EVP_PKEY_free(pkey);

        ಹಿಂತಿರುಗಿ NGX_OK;
    }

    ಹಿಂತಿರುಗಿ NGX_DECLINED;
}

#ಆದರೆ_ಅಂತ್ಯ


ಸ್ಥಿರ u_char *
ngx_proxy_protocol_v2_write_ಶೀರ್ಷಿಕೆ(ngx_connection_t *c, u_char *buf,
    u_char *ಕೊನೆಯ)
{
    u_char                            *p, src_af, dst_af,
                                       family, command, transport;
    ngx_uint_t                         src_fam, dst_fam;
    ngx_proxy_protocol_ಶೀರ್ಷಿಕೆ_t       *ಶೀರ್ಷಿಕೆ;
    ngx_proxy_protocol_inet_addrs_t   *ಒಳ;
#ಆದರೆ (NGX_HAVE_INET6)
    ngx_uint_t                         ಪೋರ್ಟ್, lport;
    ರಚನೆ in6_addr                   *src6, *dst6;
    ngx_proxy_protocol_inet6_addrs_t  *in6;
#ಆದರೆ_ಅಂತ್ಯ

    ಆದರೆ (ಕೊನೆಯ - buf < NGX_PROXY_PROTOCOL_V2_MAX_ಶೀರ್ಷಿಕೆ) {
        ngx_log_error(NGX_LOG_ALERT, c->ಲಘುಗಣಕ, 0,
                      "too small buffer for PROXY protocol v2");
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ಆದರೆ (ngx_connection_local_sockaddr(c, ಖಾಲಿ, 0) != NGX_OK) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    /* the length field is set by the caller once all TLVs are written */

    ಶೀರ್ಷಿಕೆ = (ngx_proxy_protocol_ಶೀರ್ಷಿಕೆ_t *) buf;

    ngx_memcpy(ಶೀರ್ಷಿಕೆ->signature, ngx_proxy_protocol_signature,
               ಗಾತ್ರ(ngx_proxy_protocol_signature) - 1);

    p = buf + ಗಾತ್ರ(ngx_proxy_protocol_ಶೀರ್ಷಿಕೆ_t);

    src_fam = c->sockaddr->sa_family;
    dst_fam = c->local_sockaddr->sa_family;

    src_af = ngx_proxy_protocol_v2_family(src_fam);
    dst_af = ngx_proxy_protocol_v2_family(dst_fam);

    /* promote to the highest address family present on either side */

    ಆದರೆ (src_af == NGX_PROXY_PROTOCOL_AF_UNSPEC
        || dst_af == NGX_PROXY_PROTOCOL_AF_UNSPEC)
    {
        command = NGX_PROXY_PROTOCOL_CMD_LOCAL;
        family = NGX_PROXY_PROTOCOL_AF_UNSPEC;
        transport = NGX_PROXY_PROTOCOL_TYPE_UNSPEC;

    } ಇಲ್ಲದಿದ್ದರೆ {
        command = NGX_PROXY_PROTOCOL_CMD_PROXY;
        family = ngx_max(src_af, dst_af);

        ಆಯ್ಕೆ (c->type) {
        ಸ್ಥಿತಿ SOCK_ಹರಿವು:
            transport = NGX_PROXY_PROTOCOL_TYPE_ಹರಿವು;
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ SOCK_DGRAM:
            transport = NGX_PROXY_PROTOCOL_TYPE_DGRAM;
            ನಿಲ್ಲಿಸು;
        ಪೂರ್ವನಿಯೋಜಿತ:
            transport = NGX_PROXY_PROTOCOL_TYPE_UNSPEC;
            ನಿಲ್ಲಿಸು;
        }
    }

    ಶೀರ್ಷಿಕೆ->ಆವೃತ್ತಿ_command = 0x20 | command;
    ಶೀರ್ಷಿಕೆ->family_transport = (family << 4) | transport;

    ಆಯ್ಕೆ (family) {

    ಸ್ಥಿತಿ NGX_PROXY_PROTOCOL_AF_INET:

        ಒಳ = (ngx_proxy_protocol_inet_addrs_t *) p;

        ngx_proxy_protocol_v2_write_ipv4(c->sockaddr,
                                         ಒಳ->src_addr, ಒಳ->src_ಪೋರ್ಟ್);
        ngx_proxy_protocol_v2_write_ipv4(c->local_sockaddr,
                                         ಒಳ->dst_addr, ಒಳ->dst_ಪೋರ್ಟ್);

        p += ಗಾತ್ರ(ngx_proxy_protocol_inet_addrs_t);

        ನಿಲ್ಲಿಸು;

#ಆದರೆ (NGX_HAVE_INET6)

    ಸ್ಥಿತಿ NGX_PROXY_PROTOCOL_AF_INET6:

        ಆದರೆ (src_fam == AF_INET6 && dst_fam == AF_INET6) {
            src6 = &((ರಚನೆ sockaddr_in6 *) c->sockaddr)->sin6_addr;
            dst6 = &((ರಚನೆ sockaddr_in6 *) c->local_sockaddr)->sin6_addr;

            ಆದರೆ (IN6_IS_ADDR_V4MAPPED(src6) && IN6_IS_ADDR_V4MAPPED(dst6)) {

                /* both v4-mapped: demote to AF_INET */

                ಶೀರ್ಷಿಕೆ->family_transport = (NGX_PROXY_PROTOCOL_AF_INET << 4)
                                           | transport;

                ಒಳ = (ngx_proxy_protocol_inet_addrs_t *) p;

                ngx_memcpy(ಒಳ->src_addr, src6->s6_addr + 12, 4);
                ngx_memcpy(ಒಳ->dst_addr, dst6->s6_addr + 12, 4);

                ಪೋರ್ಟ್  = ngx_inet_get_ಪೋರ್ಟ್(c->sockaddr);
                lport = ngx_inet_get_ಪೋರ್ಟ್(c->local_sockaddr);

                ಒಳ->src_ಪೋರ್ಟ್[0] = (u_char) (ಪೋರ್ಟ್ >> 8);
                ಒಳ->src_ಪೋರ್ಟ್[1] = (u_char)  ಪೋರ್ಟ್;
                ಒಳ->dst_ಪೋರ್ಟ್[0] = (u_char) (lport >> 8);
                ಒಳ->dst_ಪೋರ್ಟ್[1] = (u_char)  lport;

                p += ಗಾತ್ರ(ngx_proxy_protocol_inet_addrs_t);
                ನಿಲ್ಲಿಸು;
            }
        }

        in6 = (ngx_proxy_protocol_inet6_addrs_t *) p;

        ngx_proxy_protocol_v2_write_ipv6(c->sockaddr,
                                         in6->src_addr, in6->src_ಪೋರ್ಟ್);
        ngx_proxy_protocol_v2_write_ipv6(c->local_sockaddr,
                                         in6->dst_addr, in6->dst_ಪೋರ್ಟ್);

        p += ಗಾತ್ರ(ngx_proxy_protocol_inet6_addrs_t);

        ನಿಲ್ಲಿಸು;

#ಆದರೆ_ಅಂತ್ಯ

    ಪೂರ್ವನಿಯೋಜಿತ:  /* NGX_PROXY_PROTOCOL_AF_UNSPEC */
        ನಿಲ್ಲಿಸು;
    }

    ಹಿಂತಿರುಗಿ p;
}


ಸ್ಥಿರ u_char
ngx_proxy_protocol_v2_family(ngx_uint_t family)
{
    ಆಯ್ಕೆ (family) {

    ಸ್ಥಿತಿ AF_INET:
        ಹಿಂತಿರುಗಿ NGX_PROXY_PROTOCOL_AF_INET;

#ಆದರೆ (NGX_HAVE_INET6)
    ಸ್ಥಿತಿ AF_INET6:
        ಹಿಂತಿರುಗಿ NGX_PROXY_PROTOCOL_AF_INET6;
#ಆದರೆ_ಅಂತ್ಯ

    ಪೂರ್ವನಿಯೋಜಿತ:
        ಹಿಂತಿರುಗಿ NGX_PROXY_PROTOCOL_AF_UNSPEC;
    }
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_proxy_protocol_v2_write_ipv4(ರಚನೆ sockaddr *sa, u_char *addr,
    u_char *ಪೋರ್ಟ್)
{
    ರಚನೆ sockaddr_ಒಳ  *ಜ್ಯಾ;

    ಜ್ಯಾ = (ರಚನೆ sockaddr_ಒಳ *) sa;
    ngx_memcpy(addr, &ಜ್ಯಾ->sin_addr, 4);
    ngx_memcpy(ಪೋರ್ಟ್, &ಜ್ಯಾ->sin_ಪೋರ್ಟ್, 2);
}


#ಆದರೆ (NGX_HAVE_INET6)

ಸ್ಥಿರ ಶೂನ್ಯ
ngx_proxy_protocol_v2_write_ipv6(ರಚನೆ sockaddr *sa, u_char *addr,
    u_char *ಪೋರ್ಟ್)
{
    ರಚನೆ sockaddr_ಒಳ   *ಜ್ಯಾ;
    ರಚನೆ sockaddr_in6  *sin6;

    /* IPv4-mapped prefix: 80 zero bits + 16 one bits */
    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ u_char  mapped[]   = { 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0xff, 0xff };

    ಆದರೆ (sa->sa_family == AF_INET6) {
        sin6 = (ರಚನೆ sockaddr_in6 *) sa;
        ngx_memcpy(addr, &sin6->sin6_addr, 16);
        ngx_memcpy(ಪೋರ್ಟ್, &sin6->sin6_ಪೋರ್ಟ್, 2);

    } ಇಲ್ಲದಿದ್ದರೆ {
        /* promote AF_INET to ::ffff:a.b.c.d */
        ಜ್ಯಾ = (ರಚನೆ sockaddr_ಒಳ *) sa;
        ngx_memcpy(addr, mapped, ಗಾತ್ರ(mapped));
        ngx_memcpy(addr + ಗಾತ್ರ(mapped), &ಜ್ಯಾ->sin_addr, 4);
        ngx_memcpy(ಪೋರ್ಟ್, &ಜ್ಯಾ->sin_ಪೋರ್ಟ್, 2);
    }
}

#ಆದರೆ_ಅಂತ್ಯ


ಸ್ಥಿರ u_char *
ngx_proxy_protocol_v2_write_tlv(ngx_connection_t *c, u_char *p, u_char *ಕೊನೆಯ,
    ngx_uint_t type, ngx_str_t *ಮೌಲ್ಯ)
{
    ngx_proxy_protocol_tlv_t  *tlv;

    ಆದರೆ ((ಗಾತ್ರ_t) (ಕೊನೆಯ - p) < ಗಾತ್ರ(ngx_proxy_protocol_tlv_t) + ಮೌಲ್ಯ->len) {
        ngx_log_error(NGX_LOG_ALERT, c->ಲಘುಗಣಕ, 0,
                      "too small buffer for PROXY protocol v2 TLV");
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    tlv = (ngx_proxy_protocol_tlv_t *) p;
    tlv->type = (u_char) type;
    tlv->len[0] = (u_char) (ಮೌಲ್ಯ->len >> 8);
    tlv->len[1] = (u_char)  ಮೌಲ್ಯ->len;

    p += ಗಾತ್ರ(ngx_proxy_protocol_tlv_t);

    ಹಿಂತಿರುಗಿ ngx_cpymem(p, ಮೌಲ್ಯ->ದತ್ತಾಂಶ, ಮೌಲ್ಯ->len);
}


ಸ್ಥಿರ u_char *
ngx_proxy_protocol_v2_write_ssl(ngx_connection_t *c, u_char *p, u_char *ಕೊನೆಯ,
    ngx_array_t *ssl_tlvs, ngx_uint_t client, uint32_t verify)
{
    u_char                          *ಆರಂಭ;
    uint16_t                         len;
    ngx_uint_t                       i;
    ngx_proxy_protocol_tlv_t        *tlv;
    ngx_proxy_protocol_tlv_ssl_t    *tlv_ssl;
    ngx_proxy_protocol_tlv_ಮೌಲ್ಯ_t  *sub;

    ಆದರೆ ((ಗಾತ್ರ_t) (ಕೊನೆಯ - p) < ಗಾತ್ರ(ngx_proxy_protocol_tlv_t)
                              + ಗಾತ್ರ(ngx_proxy_protocol_tlv_ssl_t))
    {
        ngx_log_error(NGX_LOG_ALERT, c->ಲಘುಗಣಕ, 0,
                      "too small buffer for PROXY protocol v2 SSL TLV");
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ಆರಂಭ = p;
    p += ಗಾತ್ರ(ngx_proxy_protocol_tlv_t);

    tlv_ssl = (ngx_proxy_protocol_tlv_ssl_t *) p;
    tlv_ssl->client = (u_char) client;
    tlv_ssl->verify[0] = (u_char) (verify >> 24);
    tlv_ssl->verify[1] = (u_char) (verify >> 16);
    tlv_ssl->verify[2] = (u_char) (verify >> 8);
    tlv_ssl->verify[3] = (u_char)  verify;

    p += ಗಾತ್ರ(ngx_proxy_protocol_tlv_ssl_t);

    sub = ssl_tlvs->elts;

    ಗಾಗಿ (i = 0; i < ssl_tlvs->nelts; i++) {
        p = ngx_proxy_protocol_v2_write_tlv(c, p, ಕೊನೆಯ, sub[i].type,
                                            &sub[i].ಮೌಲ್ಯ);
        ಆದರೆ (p == ಖಾಲಿ) {
            ಹಿಂತಿರುಗಿ ಖಾಲಿ;
        }
    }

    len = (uint16_t) (p - ಆರಂಭ - ಗಾತ್ರ(ngx_proxy_protocol_tlv_t));

    tlv = (ngx_proxy_protocol_tlv_t *) ಆರಂಭ;
    tlv->type = NGX_PROXY_PROTOCOL_TLV_SSL;
    tlv->len[0] = (u_char) (len >> 8);
    tlv->len[1] = (u_char)  len;

    ಹಿಂತಿರುಗಿ p;
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_proxy_protocol_v2_ಗಣ_len(u_char *buf, u_char *p)
{
    uint16_t                      len;
    ngx_proxy_protocol_ಶೀರ್ಷಿಕೆ_t  *ಶೀರ್ಷಿಕೆ;

    len = (uint16_t) (p - buf - ಗಾತ್ರ(ngx_proxy_protocol_ಶೀರ್ಷಿಕೆ_t));

    ಶೀರ್ಷಿಕೆ = (ngx_proxy_protocol_ಶೀರ್ಷಿಕೆ_t *) buf;
    ಶೀರ್ಷಿಕೆ->len[0] = (u_char) (len >> 8);
    ಶೀರ್ಷಿಕೆ->len[1] = (u_char) len;
}


ಸ್ಥಿರ u_char *
ngx_proxy_protocol_v2_write_crc32c(ngx_connection_t *c, u_char *buf,
    u_char *p, u_char *ಕೊನೆಯ)
{
    u_char                    *ಮೌಲ್ಯ;
    uint32_t                   ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ;
    ngx_proxy_protocol_tlv_t  *tlv;

    ಆದರೆ ((ಗಾತ್ರ_t) (ಕೊನೆಯ - p) < ಗಾತ್ರ(ngx_proxy_protocol_tlv_t) + 4) {
        ngx_log_error(NGX_LOG_ALERT, c->ಲಘುಗಣಕ, 0,
                      "too small buffer for PROXY protocol v2 CRC32c TLV");
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    /* append the CRC32c TLV with a zeroed value field */

    tlv = (ngx_proxy_protocol_tlv_t *) p;
    tlv->type = NGX_PROXY_PROTOCOL_TLV_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_32_C;
    tlv->len[0] = 0;
    tlv->len[1] = 4;

    ಮೌಲ್ಯ = p + ಗಾತ್ರ(ngx_proxy_protocol_tlv_t);

    ngx_memzero(ಮೌಲ್ಯ, 4);

    p = ಮೌಲ್ಯ + 4;

    /* the checksum covers the entire header with the zeroed value */

    ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = ngx_crc32c_long(buf, p - buf);

    ಮೌಲ್ಯ[0] = (u_char) (ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ >> 24);
    ಮೌಲ್ಯ[1] = (u_char) (ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ >> 16);
    ಮೌಲ್ಯ[2] = (u_char) (ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ >> 8);
    ಮೌಲ್ಯ[3] = (u_char) ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ;

    ಹಿಂತಿರುಗಿ p;
}
