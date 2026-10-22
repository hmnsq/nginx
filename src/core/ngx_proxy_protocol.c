
/*
 * Copyright (C) Roman Arutyunyan
 * Copyright (C) Nginx, Inc.
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


#define NGX_PROXY_PROTOCOL_V2_MAX_HEADER    52

#define NGX_PROXY_PROTOCOL_CMD_LOCAL        0
#define NGX_PROXY_PROTOCOL_CMD_PROXY        1

#define NGX_PROXY_PROTOCOL_AF_UNSPEC        0
#define NGX_PROXY_PROTOCOL_AF_INET          1
#define NGX_PROXY_PROTOCOL_AF_INET6         2

#define NGX_PROXY_PROTOCOL_TYPE_UNSPEC      0
#define NGX_PROXY_PROTOCOL_TYPE_STREAM      1
#define NGX_PROXY_PROTOCOL_TYPE_DGRAM       2

#define NGX_PROXY_PROTOCOL_TLV_ALPN         0x01
#define NGX_PROXY_PROTOCOL_TLV_AUTHORITY    0x02
#define NGX_PROXY_PROTOCOL_TLV_CRC32C       0x03
#define NGX_PROXY_PROTOCOL_TLV_UNIQUE_ID    0x05
#define NGX_PROXY_PROTOCOL_TLV_SSL          0x20
#define NGX_PROXY_PROTOCOL_TLV_SSL_VERSION  0x21
#define NGX_PROXY_PROTOCOL_TLV_SSL_CN       0x22
#define NGX_PROXY_PROTOCOL_TLV_SSL_CIPHER   0x23
#define NGX_PROXY_PROTOCOL_TLV_SSL_SIG_ALG  0x24
#define NGX_PROXY_PROTOCOL_TLV_SSL_ಕೀಲಿ_ALG  0x25
#define NGX_PROXY_PROTOCOL_TLV_NETNS        0x30

#define NGX_PROXY_PROTOCOL_V2_CLIENT_SSL          0x01
#define NGX_PROXY_PROTOCOL_V2_CLIENT_CERT_CONN    0x02
#define NGX_PROXY_PROTOCOL_V2_CLIENT_CERT_SESS    0x04


#define ngx_proxy_protocol_parse_uint16(p)                                    \
    ( ((uint16_t) (p)[0] << 8)                                                \
    + (           (p)[1]) )

#define ngx_proxy_protocol_parse_uint32(p)                                    \
    ( ((uint32_t) (p)[0] << 24)                                               \
    + (           (p)[1] << 16)                                               \
    + (           (p)[2] << 8)                                                \
    + (           (p)[3]) )


ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    u_char                                  signature[12];
    u_char                                  version_command;
    u_char                                  family_transport;
    u_char                                  len[2];
} ngx_proxy_protocol_header_t;


ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    u_char                                  src_addr[4];
    u_char                                  dst_addr[4];
    u_char                                  src_port[2];
    u_char                                  dst_port[2];
} ngx_proxy_protocol_inet_addrs_t;


ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    u_char                                  src_addr[16];
    u_char                                  dst_addr[16];
    u_char                                  src_port[2];
    u_char                                  dst_port[2];
} ngx_proxy_protocol_inet6_addrs_t;


ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    u_char                                  type;
    u_char                                  len[2];
} ngx_proxy_protocol_tlv_t;


ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    u_char                                  client;
    u_char                                  verify[4];
} ngx_proxy_protocol_tlv_ssl_t;


ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    ngx_str_t                               name;
    ngx_uint_t                              type;
} ngx_proxy_protocol_tlv_entry_t;


ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    ngx_uint_t                              type;
    ngx_str_t                               ಮೌಲ್ಯ;
} ngx_proxy_protocol_tlv_ಮೌಲ್ಯ_t;


ಸ್ಥಿರ u_char *ngx_proxy_protocol_read_addr(ngx_connection_t *c, u_char *p,
    u_char *last, ngx_str_t *addr);
ಸ್ಥಿರ u_char *ngx_proxy_protocol_read_port(u_char *p, u_char *last,
    in_port_t *port, u_char sep);
ಸ್ಥಿರ u_char *ngx_proxy_protocol_v2_read(ngx_connection_t *c, u_char *buf,
    u_char *last);
ಸ್ಥಿರ ngx_int_t ngx_proxy_protocol_lookup_tlv(ngx_connection_t *c,
    ngx_str_t *tlvs, ngx_uint_t type, ngx_str_t *ಮೌಲ್ಯ);
#ಯದಿ (NGX_OPENSSL)
ಸ್ಥಿರ ngx_int_t ngx_proxy_protocol_v2_eval_ssl(ngx_connection_t *c,
    ngx_array_t *tlvs, ngx_array_t *ssl_tlvs, ngx_uint_t *client,
    uint32_t *verify);
ಸ್ಥಿರ ngx_int_t ngx_proxy_protocol_v2_authority(ngx_connection_t *c,
    ngx_str_t *out);
ಸ್ಥಿರ ngx_int_t ngx_proxy_protocol_v2_alpn(ngx_connection_t *c,
    ngx_str_t *out);
ಸ್ಥಿರ ngx_int_t ngx_proxy_protocol_v2_ssl_sub(ngx_connection_t *c,
    ngx_uint_t type, ngx_str_t *out);
#endif
ಸ್ಥಿರ u_char *ngx_proxy_protocol_v2_write_header(ngx_connection_t *c,
    u_char *buf, u_char *last);
ಸ್ಥಿರ u_char ngx_proxy_protocol_v2_family(ngx_uint_t family);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_proxy_protocol_v2_write_ipv4(ಸಂರಚನಾ sockaddr *sa, u_char *addr,
    u_char *port);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_proxy_protocol_v2_write_ipv6(ಸಂರಚನಾ sockaddr *sa, u_char *addr,
    u_char *port);
ಸ್ಥಿರ u_char *ngx_proxy_protocol_v2_write_tlv(ngx_connection_t *c, u_char *p,
    u_char *last, ngx_uint_t type, ngx_str_t *ಮೌಲ್ಯ);
ಸ್ಥಿರ u_char *ngx_proxy_protocol_v2_write_ssl(ngx_connection_t *c, u_char *p,
    u_char *last, ngx_array_t *ssl_tlvs, ngx_uint_t client, uint32_t verify);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_proxy_protocol_v2_set_len(u_char *buf, u_char *p);
ಸ್ಥಿರ u_char *ngx_proxy_protocol_v2_write_crc32c(ngx_connection_t *c,
    u_char *buf, u_char *p, u_char *last);


ಸ್ಥಿರ ngx_proxy_protocol_tlv_entry_t  ngx_proxy_protocol_tlv_entries[] = {
    { ngx_string("alpn"),       NGX_PROXY_PROTOCOL_TLV_ALPN },
    { ngx_string("authority"),  NGX_PROXY_PROTOCOL_TLV_AUTHORITY },
    { ngx_string("unique_id"),  NGX_PROXY_PROTOCOL_TLV_UNIQUE_ID },
    { ngx_string("ssl"),        NGX_PROXY_PROTOCOL_TLV_SSL },
    { ngx_string("netns"),      NGX_PROXY_PROTOCOL_TLV_NETNS },
    { ngx_null_string,          0x00 }
};


ಸ್ಥಿರ ngx_proxy_protocol_tlv_entry_t  ngx_proxy_protocol_tlv_ssl_entries[] = {
    { ngx_string("version"),    NGX_PROXY_PROTOCOL_TLV_SSL_VERSION },
    { ngx_string("cn"),         NGX_PROXY_PROTOCOL_TLV_SSL_CN },
    { ngx_string("cipher"),     NGX_PROXY_PROTOCOL_TLV_SSL_CIPHER },
    { ngx_string("sig_alg"),    NGX_PROXY_PROTOCOL_TLV_SSL_SIG_ALG },
    { ngx_string("key_alg"),    NGX_PROXY_PROTOCOL_TLV_SSL_ಕೀಲಿ_ALG },
    { ngx_null_string,          0x00 }
};


ಸ್ಥಿರ ಧ್ರುವ u_char  ngx_proxy_protocol_signature[] = "\r\n\r\n\0\r\nQUIT\n";


u_char *
ngx_proxy_protocol_read(ngx_connection_t *c, u_char *buf, u_char *last)
{
    size_t                 len;
    u_char                *p;
    ngx_proxy_protocol_t  *pp;

    p = buf;
    len = last - buf;

    ಯದಿ (len >= ಪರಿಮಾಣ(ngx_proxy_protocol_header_t)
        && ngx_memcmp(p, ngx_proxy_protocol_signature,
                      ಪರಿಮಾಣ(ngx_proxy_protocol_signature) - 1)
           == 0)
    {
        ಪ್ರತ್ಯಾಗಮ ngx_proxy_protocol_v2_read(c, buf, last);
    }

    ಯದಿ (len < 8 || ngx_strncmp(p, "PROXY ", 6) != 0) {
        ಗಚ್ಛ invalid;
    }

    p += 6;
    len -= 6;

    ಯದಿ (len >= 7 && ngx_strncmp(p, "UNKNOWN", 7) == 0) {
        ngx_log_debug0(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "PROXY protocol unknown protocol");
        p += 7;
        ಗಚ್ಛ skip;
    }

    ಯದಿ (len < 5 || ngx_strncmp(p, "TCP", 3) != 0
        || (p[3] != '4' && p[3] != '6') || p[4] != ' ')
    {
        ಗಚ್ಛ invalid;
    }

    p += 5;

    pp = ngx_pcalloc(c->pool, ಪರಿಮಾಣ(ngx_proxy_protocol_t));
    ಯದಿ (pp == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    p = ngx_proxy_protocol_read_addr(c, p, last, &pp->src_addr);
    ಯದಿ (p == ರಿಕ್ತ) {
        ಗಚ್ಛ invalid;
    }

    p = ngx_proxy_protocol_read_addr(c, p, last, &pp->dst_addr);
    ಯದಿ (p == ರಿಕ್ತ) {
        ಗಚ್ಛ invalid;
    }

    p = ngx_proxy_protocol_read_port(p, last, &pp->src_port, ' ');
    ಯದಿ (p == ರಿಕ್ತ) {
        ಗಚ್ಛ invalid;
    }

    p = ngx_proxy_protocol_read_port(p, last, &pp->dst_port, CR);
    ಯದಿ (p == ರಿಕ್ತ) {
        ಗಚ್ಛ invalid;
    }

    ಯದಿ (p == last) {
        ಗಚ್ಛ invalid;
    }

    ಯದಿ (*p++ != LF) {
        ಗಚ್ಛ invalid;
    }

    ngx_log_debug4(NGX_LOG_DEBUG_CORE, c->log, 0,
                   "PROXY protocol src: %V %d, dst: %V %d",
                   &pp->src_addr, pp->src_port, &pp->dst_addr, pp->dst_port);

    c->proxy_protocol = pp;

    ಪ್ರತ್ಯಾಗಮ p;

skip:

    ಪ್ರತಿ ( /* void */ ; p < last - 1; p++) {
        ಯದಿ (p[0] == CR && p[1] == LF) {
            ಪ್ರತ್ಯಾಗಮ p + 2;
        }
    }

invalid:

    ಪ್ರತಿ (p = buf; p < last; p++) {
        ಯದಿ (*p == CR || *p == LF) {
            ಭಙ್ಗ;
        }
    }

    ngx_log_error(NGX_LOG_ERR, c->log, 0,
                  "broken header: \"%*s\"", (size_t) (p - buf), buf);

    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}


ಸ್ಥಿರ u_char *
ngx_proxy_protocol_read_addr(ngx_connection_t *c, u_char *p, u_char *last,
    ngx_str_t *addr)
{
    size_t  len;
    u_char  ch, *pos;

    pos = p;

    ಪ್ರತಿ ( ;; ) {
        ಯದಿ (p == last) {
            ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
        }

        ch = *p++;

        ಯದಿ (ch == ' ') {
            ಭಙ್ಗ;
        }

        ಯದಿ (ch != ':' && ch != '.'
            && (ch < 'a' || ch > 'f')
            && (ch < 'A' || ch > 'F')
            && (ch < '0' || ch > '9'))
        {
            ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
        }
    }

    len = p - pos - 1;

    addr->data = ngx_pnalloc(c->pool, len);
    ಯದಿ (addr->data == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ngx_memcpy(addr->data, pos, len);
    addr->len = len;

    ಪ್ರತ್ಯಾಗಮ p;
}


ಸ್ಥಿರ u_char *
ngx_proxy_protocol_read_port(u_char *p, u_char *last, in_port_t *port,
    u_char sep)
{
    size_t      len;
    u_char     *pos;
    ngx_int_t   n;

    pos = p;

    ಪ್ರತಿ ( ;; ) {
        ಯದಿ (p == last) {
            ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
        }

        ಯದಿ (*p++ == sep) {
            ಭಙ್ಗ;
        }
    }

    len = p - pos - 1;

    n = ngx_atoi(pos, len);
    ಯದಿ (n < 0 || n > 65535) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    *port = (in_port_t) n;

    ಪ್ರತ್ಯಾಗಮ p;
}


u_char *
ngx_proxy_protocol_write(ngx_connection_t *c, u_char *buf, u_char *last)
{
    ngx_uint_t  port, lport;

    ಯದಿ (last - buf < NGX_PROXY_PROTOCOL_V1_MAX_HEADER) {
        ngx_log_error(NGX_LOG_ALERT, c->log, 0,
                      "too small buffer for PROXY protocol");
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ಯದಿ (ngx_connection_local_sockaddr(c, ರಿಕ್ತ, 0) != NGX_OK) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ವಿಕಲ್ಪ (c->sockaddr->sa_family) {

    ಪಕ್ಷ AF_INET:
        buf = ngx_cpymem(buf, "PROXY TCP4 ", ಪರಿಮಾಣ("PROXY TCP4 ") - 1);
        ಭಙ್ಗ;

#ಯದಿ (NGX_HAVE_INET6)
    ಪಕ್ಷ AF_INET6:
        buf = ngx_cpymem(buf, "PROXY TCP6 ", ಪರಿಮಾಣ("PROXY TCP6 ") - 1);
        ಭಙ್ಗ;
#endif

    ಸ್ವಭಾವ:
        ಪ್ರತ್ಯಾಗಮ ngx_cpymem(buf, "PROXY UNKNOWN" CRLF,
                          ಪರಿಮಾಣ("PROXY UNKNOWN" CRLF) - 1);
    }

    buf += ngx_sock_ntop(c->sockaddr, c->socklen, buf, last - buf, 0);

    *buf++ = ' ';

    buf += ngx_sock_ntop(c->local_sockaddr, c->local_socklen, buf, last - buf,
                         0);

    port = ngx_inet_get_port(c->sockaddr);
    lport = ngx_inet_get_port(c->local_sockaddr);

    ಪ್ರತ್ಯಾಗಮ ngx_slprintf(buf, last, " %ui %ui" CRLF, port, lport);
}


ಸ್ಥಿರ u_char *
ngx_proxy_protocol_v2_read(ngx_connection_t *c, u_char *buf, u_char *last)
{
    u_char                             *end;
    size_t                              len;
    socklen_t                           socklen;
    ngx_uint_t                          version, command, family, transport;
    ngx_sockaddr_t                      src_sockaddr, dst_sockaddr;
    ngx_proxy_protocol_t               *pp;
    ngx_proxy_protocol_header_t        *header;
    ngx_proxy_protocol_inet_addrs_t    *in;
#ಯದಿ (NGX_HAVE_INET6)
    ngx_proxy_protocol_inet6_addrs_t   *in6;
#endif

    header = (ngx_proxy_protocol_header_t *) buf;

    buf += ಪರಿಮಾಣ(ngx_proxy_protocol_header_t);

    version = header->version_command >> 4;

    ಯದಿ (version != 2) {
        ngx_log_error(NGX_LOG_ERR, c->log, 0,
                      "unknown PROXY protocol version: %ui", version);
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    len = ngx_proxy_protocol_parse_uint16(header->len);

    ಯದಿ ((size_t) (last - buf) < len) {
        ngx_log_error(NGX_LOG_ERR, c->log, 0, "header is too large");
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    end = buf + len;

    command = header->version_command & 0x0f;

    /* only PROXY is supported */
    ಯದಿ (command != NGX_PROXY_PROTOCOL_CMD_PROXY) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "PROXY protocol v2 unsupported command %ui", command);
        ಪ್ರತ್ಯಾಗಮ end;
    }

    transport = header->family_transport & 0x0f;

    /* only STREAM is supported */
    ಯದಿ (transport != NGX_PROXY_PROTOCOL_TYPE_STREAM) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "PROXY protocol v2 unsupported transport %ui",
                       transport);
        ಪ್ರತ್ಯಾಗಮ end;
    }

    pp = ngx_pcalloc(c->pool, ಪರಿಮಾಣ(ngx_proxy_protocol_t));
    ಯದಿ (pp == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    family = header->family_transport >> 4;

    ವಿಕಲ್ಪ (family) {

    ಪಕ್ಷ NGX_PROXY_PROTOCOL_AF_INET:

        ಯದಿ ((size_t) (end - buf) < ಪರಿಮಾಣ(ngx_proxy_protocol_inet_addrs_t)) {
            ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
        }

        in = (ngx_proxy_protocol_inet_addrs_t *) buf;

        src_sockaddr.sockaddr_in.sin_family = AF_INET;
        src_sockaddr.sockaddr_in.sin_port = 0;
        ngx_memcpy(&src_sockaddr.sockaddr_in.sin_addr, in->src_addr, 4);

        dst_sockaddr.sockaddr_in.sin_family = AF_INET;
        dst_sockaddr.sockaddr_in.sin_port = 0;
        ngx_memcpy(&dst_sockaddr.sockaddr_in.sin_addr, in->dst_addr, 4);

        pp->src_port = ngx_proxy_protocol_parse_uint16(in->src_port);
        pp->dst_port = ngx_proxy_protocol_parse_uint16(in->dst_port);

        socklen = ಪರಿಮಾಣ(ಸಂರಚನಾ sockaddr_in);

        buf += ಪರಿಮಾಣ(ngx_proxy_protocol_inet_addrs_t);

        ಭಙ್ಗ;

#ಯದಿ (NGX_HAVE_INET6)

    ಪಕ್ಷ NGX_PROXY_PROTOCOL_AF_INET6:

        ಯದಿ ((size_t) (end - buf) < ಪರಿಮಾಣ(ngx_proxy_protocol_inet6_addrs_t)) {
            ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
        }

        in6 = (ngx_proxy_protocol_inet6_addrs_t *) buf;

        src_sockaddr.sockaddr_in6.sin6_family = AF_INET6;
        src_sockaddr.sockaddr_in6.sin6_port = 0;
        ngx_memcpy(&src_sockaddr.sockaddr_in6.sin6_addr, in6->src_addr, 16);

        dst_sockaddr.sockaddr_in6.sin6_family = AF_INET6;
        dst_sockaddr.sockaddr_in6.sin6_port = 0;
        ngx_memcpy(&dst_sockaddr.sockaddr_in6.sin6_addr, in6->dst_addr, 16);

        pp->src_port = ngx_proxy_protocol_parse_uint16(in6->src_port);
        pp->dst_port = ngx_proxy_protocol_parse_uint16(in6->dst_port);

        socklen = ಪರಿಮಾಣ(ಸಂರಚನಾ sockaddr_in6);

        buf += ಪರಿಮಾಣ(ngx_proxy_protocol_inet6_addrs_t);

        ಭಙ್ಗ;

#endif

    ಸ್ವಭಾವ:
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "PROXY protocol v2 unsupported address family %ui",
                       family);
        ಪ್ರತ್ಯಾಗಮ end;
    }

    pp->src_addr.data = ngx_pnalloc(c->pool, NGX_SOCKADDR_STRLEN);
    ಯದಿ (pp->src_addr.data == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    pp->src_addr.len = ngx_sock_ntop(&src_sockaddr.sockaddr, socklen,
                                     pp->src_addr.data, NGX_SOCKADDR_STRLEN, 0);

    pp->dst_addr.data = ngx_pnalloc(c->pool, NGX_SOCKADDR_STRLEN);
    ಯದಿ (pp->dst_addr.data == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    pp->dst_addr.len = ngx_sock_ntop(&dst_sockaddr.sockaddr, socklen,
                                     pp->dst_addr.data, NGX_SOCKADDR_STRLEN, 0);

    ngx_log_debug4(NGX_LOG_DEBUG_CORE, c->log, 0,
                   "PROXY protocol v2 src: %V %d, dst: %V %d",
                   &pp->src_addr, pp->src_port, &pp->dst_addr, pp->dst_port);

    ಯದಿ (buf < end) {
        pp->tlvs.data = ngx_pnalloc(c->pool, end - buf);
        ಯದಿ (pp->tlvs.data == ರಿಕ್ತ) {
            ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
        }

        ngx_memcpy(pp->tlvs.data, buf, end - buf);
        pp->tlvs.len = end - buf;
    }

    c->proxy_protocol = pp;

    ಪ್ರತ್ಯಾಗಮ end;
}


ngx_int_t
ngx_proxy_protocol_get_tlv(ngx_connection_t *c, ngx_str_t *name,
    ngx_str_t *ಮೌಲ್ಯ)
{
    u_char                          *p;
    size_t                           n;
    uint32_t                         verify;
    ngx_str_t                        ssl, *tlvs;
    ngx_int_t                        rc, type;
    ngx_proxy_protocol_tlv_ssl_t    *tlv_ssl;
    ngx_proxy_protocol_tlv_entry_t  *te;

    ಯದಿ (c->proxy_protocol == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                   "PROXY protocol v2 get tlv \"%V\"", name);

    te = ngx_proxy_protocol_tlv_entries;
    tlvs = &c->proxy_protocol->tlvs;

    p = name->data;
    n = name->len;

    ಯದಿ (n >= 4 && p[0] == 's' && p[1] == 's' && p[2] == 'l' && p[3] == '_') {

        rc = ngx_proxy_protocol_lookup_tlv(c, tlvs,
                                           NGX_PROXY_PROTOCOL_TLV_SSL, &ssl);
        ಯದಿ (rc != NGX_OK) {
            ಪ್ರತ್ಯಾಗಮ rc;
        }

        ಯದಿ (ssl.len < ಪರಿಮಾಣ(ngx_proxy_protocol_tlv_ssl_t)) {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        p += 4;
        n -= 4;

        ಯದಿ (n == 6 && ngx_strncmp(p, "verify", 6) == 0) {

            tlv_ssl = (ngx_proxy_protocol_tlv_ssl_t *) ssl.data;
            verify = ngx_proxy_protocol_parse_uint32(tlv_ssl->verify);

            ಮೌಲ್ಯ->data = ngx_pnalloc(c->pool, NGX_INT32_LEN);
            ಯದಿ (ಮೌಲ್ಯ->data == ರಿಕ್ತ) {
                ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
            }

            ಮೌಲ್ಯ->len = ngx_sprintf(ಮೌಲ್ಯ->data, "%uD", verify)
                         - ಮೌಲ್ಯ->data;
            ಪ್ರತ್ಯಾಗಮ NGX_OK;
        }

        ssl.data += ಪರಿಮಾಣ(ngx_proxy_protocol_tlv_ssl_t);
        ssl.len -= ಪರಿಮಾಣ(ngx_proxy_protocol_tlv_ssl_t);

        te = ngx_proxy_protocol_tlv_ssl_entries;
        tlvs = &ssl;
    }

    ಯದಿ (n >= 2 && p[0] == '0' && p[1] == 'x') {

        type = ngx_hextoi(p + 2, n - 2);
        ಯದಿ (type == NGX_ERROR) {
            ngx_log_error(NGX_LOG_ERR, c->log, 0,
                          "invalid PROXY protocol TLV \"%V\"", name);
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        ಪ್ರತ್ಯಾಗಮ ngx_proxy_protocol_lookup_tlv(c, tlvs, type, ಮೌಲ್ಯ);
    }

    ಪ್ರತಿ ( /* void */ ; te->type; te++) {
        ಯದಿ (te->name.len == n && ngx_strncmp(te->name.data, p, n) == 0) {
            ಪ್ರತ್ಯಾಗಮ ngx_proxy_protocol_lookup_tlv(c, tlvs, te->type, ಮೌಲ್ಯ);
        }
    }

    ngx_log_error(NGX_LOG_ERR, c->log, 0,
                  "unknown PROXY protocol TLV \"%V\"", name);

    ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;
}


ಸ್ಥಿರ ngx_int_t
ngx_proxy_protocol_lookup_tlv(ngx_connection_t *c, ngx_str_t *tlvs,
    ngx_uint_t type, ngx_str_t *ಮೌಲ್ಯ)
{
    u_char                    *p;
    size_t                     n, len;
    ngx_proxy_protocol_tlv_t  *tlv;

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                   "PROXY protocol v2 lookup tlv:%02xi", type);

    p = tlvs->data;
    n = tlvs->len;

    ಯಾವತ್ (n) {
        ಯದಿ (n < ಪರಿಮಾಣ(ngx_proxy_protocol_tlv_t)) {
            ngx_log_error(NGX_LOG_ERR, c->log, 0, "broken PROXY protocol TLV");
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        tlv = (ngx_proxy_protocol_tlv_t *) p;
        len = ngx_proxy_protocol_parse_uint16(tlv->len);

        p += ಪರಿಮಾಣ(ngx_proxy_protocol_tlv_t);
        n -= ಪರಿಮಾಣ(ngx_proxy_protocol_tlv_t);

        ಯದಿ (n < len) {
            ngx_log_error(NGX_LOG_ERR, c->log, 0, "broken PROXY protocol TLV");
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        ಯದಿ (tlv->type == type) {
            ಮೌಲ್ಯ->data = p;
            ಮೌಲ್ಯ->len = len;
            ಪ್ರತ್ಯಾಗಮ NGX_OK;
        }

        p += len;
        n -= len;
    }

    ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;
}


u_char *
ngx_proxy_protocol_v2_write(ngx_connection_t *c, u_char *buf, u_char *last,
    ngx_array_t *tlvs /* reserved */ )
{
    u_char                          *p;
    uint32_t                         verify;
    ngx_uint_t                       i, client;
    ngx_array_t                      base_tlvs, ssl_tlvs;
    ngx_proxy_protocol_tlv_ಮೌಲ್ಯ_t  *tlv;

    client = 0;
    verify = 0;

    ngx_memzero(&base_tlvs, ಪರಿಮಾಣ(ngx_array_t));
    ngx_memzero(&ssl_tlvs, ಪರಿಮಾಣ(ngx_array_t));

#ಯದಿ (NGX_OPENSSL)

    ಯದಿ (c->ssl != ರಿಕ್ತ) {
        ಯದಿ (ngx_array_init(&base_tlvs, c->pool, 2,
                           ಪರಿಮಾಣ(ngx_proxy_protocol_tlv_ಮೌಲ್ಯ_t))
            != NGX_OK)
        {
            ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
        }

        ಯದಿ (ngx_array_init(&ssl_tlvs, c->pool, 5,
                           ಪರಿಮಾಣ(ngx_proxy_protocol_tlv_ಮೌಲ್ಯ_t))
            != NGX_OK)
        {
            ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
        }

        ಯದಿ (ngx_proxy_protocol_v2_eval_ssl(c, &base_tlvs, &ssl_tlvs, &client,
                                           &verify)
            != NGX_OK)
        {
            ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
        }
    }

#endif

    p = ngx_proxy_protocol_v2_write_header(c, buf, last);
    ಯದಿ (p == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    tlv = base_tlvs.elts;

    ಪ್ರತಿ (i = 0; i < base_tlvs.nelts; i++) {
        p = ngx_proxy_protocol_v2_write_tlv(c, p, last, tlv[i].type,
                                           &tlv[i].ಮೌಲ್ಯ);
        ಯದಿ (p == ರಿಕ್ತ) {
            ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
        }
    }

    ಯದಿ (client) {
        p = ngx_proxy_protocol_v2_write_ssl(c, p, last, &ssl_tlvs, client,
                                            verify);
        ಯದಿ (p == ರಿಕ್ತ) {
            ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
        }
    }

    ngx_proxy_protocol_v2_set_len(buf,
                                  p + ಪರಿಮಾಣ(ngx_proxy_protocol_tlv_t) + 4);

    ಪ್ರತ್ಯಾಗಮ ngx_proxy_protocol_v2_write_crc32c(c, buf, p, last);
}


#ಯದಿ (NGX_OPENSSL)

ಸ್ಥಿರ ngx_int_t
ngx_proxy_protocol_v2_eval_ssl(ngx_connection_t *c, ngx_array_t *tlvs,
    ngx_array_t *ssl_tlvs, ngx_uint_t *client, uint32_t *verify)
{
    X509                            *cert;
    long                             n;
    ngx_int_t                        rc;
    ngx_str_t                        ಮೌಲ್ಯ;
    ngx_uint_t                       type;
    ಧ್ರುವ ವರ್ಣ                      *s;
    ngx_proxy_protocol_tlv_ಮೌಲ್ಯ_t  *tlv;

    ಯದಿ (ngx_proxy_protocol_v2_authority(c, &ಮೌಲ್ಯ) == NGX_OK) {
        tlv = ngx_array_ತಳ್ಳು(tlvs);
        ಯದಿ (tlv == ರಿಕ್ತ) {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        tlv->type = NGX_PROXY_PROTOCOL_TLV_AUTHORITY;
        tlv->ಮೌಲ್ಯ = ಮೌಲ್ಯ;
    }

    ಯದಿ (ngx_proxy_protocol_v2_alpn(c, &ಮೌಲ್ಯ) == NGX_OK) {
        tlv = ngx_array_ತಳ್ಳು(tlvs);
        ಯದಿ (tlv == ರಿಕ್ತ) {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        tlv->type = NGX_PROXY_PROTOCOL_TLV_ALPN;
        tlv->ಮೌಲ್ಯ = ಮೌಲ್ಯ;
    }

    ಪ್ರತಿ (type = NGX_PROXY_PROTOCOL_TLV_SSL_VERSION;
         type <= NGX_PROXY_PROTOCOL_TLV_SSL_ಕೀಲಿ_ALG;
         type++)
    {
        rc = ngx_proxy_protocol_v2_ssl_sub(c, type, &ಮೌಲ್ಯ);

        ಯದಿ (rc == NGX_ERROR) {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        ಯದಿ (rc != NGX_OK) {
            ಅನುವರ್ತ;
        }

        tlv = ngx_array_ತಳ್ಳು(ssl_tlvs);
        ಯದಿ (tlv == ರಿಕ್ತ) {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        tlv->type = type;
        tlv->ಮೌಲ್ಯ = ಮೌಲ್ಯ;
    }

    *client = NGX_PROXY_PROTOCOL_V2_CLIENT_SSL;
    *verify = 1;  /* X509_V_ERR_UNSPECIFIED */

    cert = SSL_get_peer_certificate(c->ssl->connection);

    ಯದಿ (cert != ರಿಕ್ತ) {
        X509_free(cert);

        *client |= NGX_PROXY_PROTOCOL_V2_CLIENT_CERT_SESS;

        ಯದಿ (!SSL_session_reused(c->ssl->connection)) {
            *client |= NGX_PROXY_PROTOCOL_V2_CLIENT_CERT_CONN;
        }

        n = SSL_get_verify_result(c->ssl->connection);

        ಯದಿ (n == X509_V_OK) {

            ಯದಿ (ngx_ssl_ocsp_get_status(c, &s) != NGX_OK) {
                n = X509_V_ERR_CERT_REVOKED;
            }
        }

        *verify = (uint32_t) n;
    }

    ಪ್ರತ್ಯಾಗಮ NGX_OK;
}


ಸ್ಥಿರ ngx_int_t
ngx_proxy_protocol_v2_authority(ngx_connection_t *c, ngx_str_t *out)
{
#ifdef SSL_CTRL_SET_TLSEXT_HOSTNAME

    ಧ್ರುವ ವರ್ಣ  *sni;

    sni = SSL_get_servername(c->ssl->connection, TLSEXT_NAMETYPE_host_name);
    ಯದಿ (sni == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;
    }

    out->data = (u_char *) sni;
    out->len = ngx_strlen(sni);
    ಪ್ರತ್ಯಾಗಮ NGX_OK;

#ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;
#endif
}


ಸ್ಥಿರ ngx_int_t
ngx_proxy_protocol_v2_alpn(ngx_connection_t *c, ngx_str_t *out)
{
#ifdef TLSEXT_TYPE_application_layer_protocol_negotiation

    unsigned ಪೂರ್ಣಾಙ್ಕ          alpnlen;
    ಧ್ರುವ unsigned ವರ್ಣ  *alpn;

    SSL_get0_alpn_selected(c->ssl->connection, &alpn, &alpnlen);
    ಯದಿ (alpn == ರಿಕ್ತ || alpnlen == 0) {
        ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;
    }

    out->data = (u_char *) alpn;
    out->len = alpnlen;
    ಪ್ರತ್ಯಾಗಮ NGX_OK;

#ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;
#endif
}


ಸ್ಥಿರ ngx_int_t
ngx_proxy_protocol_v2_ssl_sub(ngx_connection_t *c, ngx_uint_t type,
    ngx_str_t *out)
{
    ಪೂರ್ಣಾಙ್ಕ               i, len;
    X509             *cert;
    u_char           *alg, *s;
    EVP_PKEY         *pkey;
#ಯದಿ (OPENSSL_VERSION_ಸಂಖ್ಯೆ >= 0x40000000L)
    ಧ್ರುವ
#endif
    X509_NAME        *subject;
#ಯದಿ (OPENSSL_VERSION_ಸಂಖ್ಯೆ >= 0x40000000L)
    ಧ್ರುವ
#endif
    X509_NAME_ENTRY  *entry;

    ವಿಕಲ್ಪ (type) {

    ಪಕ್ಷ NGX_PROXY_PROTOCOL_TLV_SSL_VERSION:
        out->data = (u_char *) SSL_get_version(c->ssl->connection);
        out->len = ngx_strlen(out->data);
        ಪ್ರತ್ಯಾಗಮ NGX_OK;

    ಪಕ್ಷ NGX_PROXY_PROTOCOL_TLV_SSL_CIPHER:
        out->data = (u_char *) SSL_get_cipher_name(c->ssl->connection);
        out->len = ngx_strlen(out->data);
        ಪ್ರತ್ಯಾಗಮ NGX_OK;

    ಪಕ್ಷ NGX_PROXY_PROTOCOL_TLV_SSL_CN:
        cert = SSL_get_peer_certificate(c->ssl->connection);
        ಯದಿ (cert == ರಿಕ್ತ) {
            ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;
        }

        subject = X509_get_subject_name(cert);
        ಯದಿ (subject == ರಿಕ್ತ) {
            X509_free(cert);
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        i = X509_NAME_get_index_by_NID(subject, NID_commonName, -1);
        ಯದಿ (i < 0) {
            X509_free(cert);
            ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;
        }

        entry = X509_NAME_get_entry(subject, i);

        len = ASN1_STRING_to_UTF8(&s, X509_NAME_ENTRY_get_data(entry));
        ಯದಿ (len < 0) {
            ngx_ssl_error(NGX_LOG_ALERT, c->log, 0,
                          "ASN1_STRING_to_UTF8() failed");
            X509_free(cert);
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        out->len = len;
        out->data = ngx_pnalloc(c->pool, len);
        ಯದಿ (out->data == ರಿಕ್ತ) {
            OPENSSL_free(s);
            X509_free(cert);
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        ngx_memcpy(out->data, s, len);

        OPENSSL_free(s);
        X509_free(cert);
        ಪ್ರತ್ಯಾಗಮ NGX_OK;

    ಪಕ್ಷ NGX_PROXY_PROTOCOL_TLV_SSL_SIG_ALG:
        cert = SSL_get_certificate(c->ssl->connection);

        out->data = (u_char *) OBJ_nid2sn(X509_get_signature_nid(cert));
        out->len = ngx_strlen(out->data);
        ಪ್ರತ್ಯಾಗಮ NGX_OK;

    ಪಕ್ಷ NGX_PROXY_PROTOCOL_TLV_SSL_ಕೀಲಿ_ALG:
        cert = SSL_get_certificate(c->ssl->connection);

        pkey = X509_get_pubkey(cert);
        ಯದಿ (pkey == ರಿಕ್ತ) {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        ವಿಕಲ್ಪ (EVP_PKEY_base_id(pkey)) {

        ಪಕ್ಷ EVP_PKEY_RSA:
#ifdef EVP_PKEY_RSA_PSS
        ಪಕ್ಷ EVP_PKEY_RSA_PSS:
#endif
            alg = (u_char *) "RSA";
            ಭಙ್ಗ;

        ಪಕ್ಷ EVP_PKEY_EC:
            alg = (u_char *) "EC";
            ಭಙ್ಗ;

        ಪಕ್ಷ EVP_PKEY_DSA:
            alg = (u_char *) "DSA";
            ಭಙ್ಗ;

        ಸ್ವಭಾವ:
            EVP_PKEY_free(pkey);
            ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;
        }

        out->len = ಪರಿಮಾಣ("RSA16384") - 1;
        out->data = ngx_pnalloc(c->pool, out->len);
        ಯದಿ (out->data == ರಿಕ್ತ) {
            EVP_PKEY_free(pkey);
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        out->len = ngx_snprintf(out->data, out->len, "%s%d",
                                alg, EVP_PKEY_bits(pkey))
                   - out->data;

        EVP_PKEY_free(pkey);

        ಪ್ರತ್ಯಾಗಮ NGX_OK;
    }

    ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;
}

#endif


ಸ್ಥಿರ u_char *
ngx_proxy_protocol_v2_write_header(ngx_connection_t *c, u_char *buf,
    u_char *last)
{
    u_char                            *p, src_af, dst_af,
                                       family, command, transport;
    ngx_uint_t                         src_fam, dst_fam;
    ngx_proxy_protocol_header_t       *header;
    ngx_proxy_protocol_inet_addrs_t   *in;
#ಯದಿ (NGX_HAVE_INET6)
    ngx_uint_t                         port, lport;
    ಸಂರಚನಾ in6_addr                   *src6, *dst6;
    ngx_proxy_protocol_inet6_addrs_t  *in6;
#endif

    ಯದಿ (last - buf < NGX_PROXY_PROTOCOL_V2_MAX_HEADER) {
        ngx_log_error(NGX_LOG_ALERT, c->log, 0,
                      "too small buffer for PROXY protocol v2");
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ಯದಿ (ngx_connection_local_sockaddr(c, ರಿಕ್ತ, 0) != NGX_OK) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    /* the length field is set by the caller once all TLVs are written */

    header = (ngx_proxy_protocol_header_t *) buf;

    ngx_memcpy(header->signature, ngx_proxy_protocol_signature,
               ಪರಿಮಾಣ(ngx_proxy_protocol_signature) - 1);

    p = buf + ಪರಿಮಾಣ(ngx_proxy_protocol_header_t);

    src_fam = c->sockaddr->sa_family;
    dst_fam = c->local_sockaddr->sa_family;

    src_af = ngx_proxy_protocol_v2_family(src_fam);
    dst_af = ngx_proxy_protocol_v2_family(dst_fam);

    /* promote to the highest address family present on either side */

    ಯದಿ (src_af == NGX_PROXY_PROTOCOL_AF_UNSPEC
        || dst_af == NGX_PROXY_PROTOCOL_AF_UNSPEC)
    {
        command = NGX_PROXY_PROTOCOL_CMD_LOCAL;
        family = NGX_PROXY_PROTOCOL_AF_UNSPEC;
        transport = NGX_PROXY_PROTOCOL_TYPE_UNSPEC;

    } ಅನ್ಯಥಾ {
        command = NGX_PROXY_PROTOCOL_CMD_PROXY;
        family = ngx_max(src_af, dst_af);

        ವಿಕಲ್ಪ (c->type) {
        ಪಕ್ಷ SOCK_STREAM:
            transport = NGX_PROXY_PROTOCOL_TYPE_STREAM;
            ಭಙ್ಗ;
        ಪಕ್ಷ SOCK_DGRAM:
            transport = NGX_PROXY_PROTOCOL_TYPE_DGRAM;
            ಭಙ್ಗ;
        ಸ್ವಭಾವ:
            transport = NGX_PROXY_PROTOCOL_TYPE_UNSPEC;
            ಭಙ್ಗ;
        }
    }

    header->version_command = 0x20 | command;
    header->family_transport = (family << 4) | transport;

    ವಿಕಲ್ಪ (family) {

    ಪಕ್ಷ NGX_PROXY_PROTOCOL_AF_INET:

        in = (ngx_proxy_protocol_inet_addrs_t *) p;

        ngx_proxy_protocol_v2_write_ipv4(c->sockaddr,
                                         in->src_addr, in->src_port);
        ngx_proxy_protocol_v2_write_ipv4(c->local_sockaddr,
                                         in->dst_addr, in->dst_port);

        p += ಪರಿಮಾಣ(ngx_proxy_protocol_inet_addrs_t);

        ಭಙ್ಗ;

#ಯದಿ (NGX_HAVE_INET6)

    ಪಕ್ಷ NGX_PROXY_PROTOCOL_AF_INET6:

        ಯದಿ (src_fam == AF_INET6 && dst_fam == AF_INET6) {
            src6 = &((ಸಂರಚನಾ sockaddr_in6 *) c->sockaddr)->sin6_addr;
            dst6 = &((ಸಂರಚನಾ sockaddr_in6 *) c->local_sockaddr)->sin6_addr;

            ಯದಿ (IN6_IS_ADDR_V4MAPPED(src6) && IN6_IS_ADDR_V4MAPPED(dst6)) {

                /* both v4-mapped: demote to AF_INET */

                header->family_transport = (NGX_PROXY_PROTOCOL_AF_INET << 4)
                                           | transport;

                in = (ngx_proxy_protocol_inet_addrs_t *) p;

                ngx_memcpy(in->src_addr, src6->s6_addr + 12, 4);
                ngx_memcpy(in->dst_addr, dst6->s6_addr + 12, 4);

                port  = ngx_inet_get_port(c->sockaddr);
                lport = ngx_inet_get_port(c->local_sockaddr);

                in->src_port[0] = (u_char) (port >> 8);
                in->src_port[1] = (u_char)  port;
                in->dst_port[0] = (u_char) (lport >> 8);
                in->dst_port[1] = (u_char)  lport;

                p += ಪರಿಮಾಣ(ngx_proxy_protocol_inet_addrs_t);
                ಭಙ್ಗ;
            }
        }

        in6 = (ngx_proxy_protocol_inet6_addrs_t *) p;

        ngx_proxy_protocol_v2_write_ipv6(c->sockaddr,
                                         in6->src_addr, in6->src_port);
        ngx_proxy_protocol_v2_write_ipv6(c->local_sockaddr,
                                         in6->dst_addr, in6->dst_port);

        p += ಪರಿಮಾಣ(ngx_proxy_protocol_inet6_addrs_t);

        ಭಙ್ಗ;

#endif

    ಸ್ವಭಾವ:  /* NGX_PROXY_PROTOCOL_AF_UNSPEC */
        ಭಙ್ಗ;
    }

    ಪ್ರತ್ಯಾಗಮ p;
}


ಸ್ಥಿರ u_char
ngx_proxy_protocol_v2_family(ngx_uint_t family)
{
    ವಿಕಲ್ಪ (family) {

    ಪಕ್ಷ AF_INET:
        ಪ್ರತ್ಯಾಗಮ NGX_PROXY_PROTOCOL_AF_INET;

#ಯದಿ (NGX_HAVE_INET6)
    ಪಕ್ಷ AF_INET6:
        ಪ್ರತ್ಯಾಗಮ NGX_PROXY_PROTOCOL_AF_INET6;
#endif

    ಸ್ವಭಾವ:
        ಪ್ರತ್ಯಾಗಮ NGX_PROXY_PROTOCOL_AF_UNSPEC;
    }
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_proxy_protocol_v2_write_ipv4(ಸಂರಚನಾ sockaddr *sa, u_char *addr,
    u_char *port)
{
    ಸಂರಚನಾ sockaddr_in  *sin;

    sin = (ಸಂರಚನಾ sockaddr_in *) sa;
    ngx_memcpy(addr, &sin->sin_addr, 4);
    ngx_memcpy(port, &sin->sin_port, 2);
}


#ಯದಿ (NGX_HAVE_INET6)

ಸ್ಥಿರ ಶೂನ್ಯ
ngx_proxy_protocol_v2_write_ipv6(ಸಂರಚನಾ sockaddr *sa, u_char *addr,
    u_char *port)
{
    ಸಂರಚನಾ sockaddr_in   *sin;
    ಸಂರಚನಾ sockaddr_in6  *sin6;

    /* IPv4-mapped prefix: 80 zero bits + 16 one bits */
    ಸ್ಥಿರ ಧ್ರುವ u_char  mapped[]   = { 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0xff, 0xff };

    ಯದಿ (sa->sa_family == AF_INET6) {
        sin6 = (ಸಂರಚನಾ sockaddr_in6 *) sa;
        ngx_memcpy(addr, &sin6->sin6_addr, 16);
        ngx_memcpy(port, &sin6->sin6_port, 2);

    } ಅನ್ಯಥಾ {
        /* promote AF_INET to ::ffff:a.b.c.d */
        sin = (ಸಂರಚನಾ sockaddr_in *) sa;
        ngx_memcpy(addr, mapped, ಪರಿಮಾಣ(mapped));
        ngx_memcpy(addr + ಪರಿಮಾಣ(mapped), &sin->sin_addr, 4);
        ngx_memcpy(port, &sin->sin_port, 2);
    }
}

#endif


ಸ್ಥಿರ u_char *
ngx_proxy_protocol_v2_write_tlv(ngx_connection_t *c, u_char *p, u_char *last,
    ngx_uint_t type, ngx_str_t *ಮೌಲ್ಯ)
{
    ngx_proxy_protocol_tlv_t  *tlv;

    ಯದಿ ((size_t) (last - p) < ಪರಿಮಾಣ(ngx_proxy_protocol_tlv_t) + ಮೌಲ್ಯ->len) {
        ngx_log_error(NGX_LOG_ALERT, c->log, 0,
                      "too small buffer for PROXY protocol v2 TLV");
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    tlv = (ngx_proxy_protocol_tlv_t *) p;
    tlv->type = (u_char) type;
    tlv->len[0] = (u_char) (ಮೌಲ್ಯ->len >> 8);
    tlv->len[1] = (u_char)  ಮೌಲ್ಯ->len;

    p += ಪರಿಮಾಣ(ngx_proxy_protocol_tlv_t);

    ಪ್ರತ್ಯಾಗಮ ngx_cpymem(p, ಮೌಲ್ಯ->data, ಮೌಲ್ಯ->len);
}


ಸ್ಥಿರ u_char *
ngx_proxy_protocol_v2_write_ssl(ngx_connection_t *c, u_char *p, u_char *last,
    ngx_array_t *ssl_tlvs, ngx_uint_t client, uint32_t verify)
{
    u_char                          *start;
    uint16_t                         len;
    ngx_uint_t                       i;
    ngx_proxy_protocol_tlv_t        *tlv;
    ngx_proxy_protocol_tlv_ssl_t    *tlv_ssl;
    ngx_proxy_protocol_tlv_ಮೌಲ್ಯ_t  *sub;

    ಯದಿ ((size_t) (last - p) < ಪರಿಮಾಣ(ngx_proxy_protocol_tlv_t)
                              + ಪರಿಮಾಣ(ngx_proxy_protocol_tlv_ssl_t))
    {
        ngx_log_error(NGX_LOG_ALERT, c->log, 0,
                      "too small buffer for PROXY protocol v2 SSL TLV");
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    start = p;
    p += ಪರಿಮಾಣ(ngx_proxy_protocol_tlv_t);

    tlv_ssl = (ngx_proxy_protocol_tlv_ssl_t *) p;
    tlv_ssl->client = (u_char) client;
    tlv_ssl->verify[0] = (u_char) (verify >> 24);
    tlv_ssl->verify[1] = (u_char) (verify >> 16);
    tlv_ssl->verify[2] = (u_char) (verify >> 8);
    tlv_ssl->verify[3] = (u_char)  verify;

    p += ಪರಿಮಾಣ(ngx_proxy_protocol_tlv_ssl_t);

    sub = ssl_tlvs->elts;

    ಪ್ರತಿ (i = 0; i < ssl_tlvs->nelts; i++) {
        p = ngx_proxy_protocol_v2_write_tlv(c, p, last, sub[i].type,
                                            &sub[i].ಮೌಲ್ಯ);
        ಯದಿ (p == ರಿಕ್ತ) {
            ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
        }
    }

    len = (uint16_t) (p - start - ಪರಿಮಾಣ(ngx_proxy_protocol_tlv_t));

    tlv = (ngx_proxy_protocol_tlv_t *) start;
    tlv->type = NGX_PROXY_PROTOCOL_TLV_SSL;
    tlv->len[0] = (u_char) (len >> 8);
    tlv->len[1] = (u_char)  len;

    ಪ್ರತ್ಯಾಗಮ p;
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_proxy_protocol_v2_set_len(u_char *buf, u_char *p)
{
    uint16_t                      len;
    ngx_proxy_protocol_header_t  *header;

    len = (uint16_t) (p - buf - ಪರಿಮಾಣ(ngx_proxy_protocol_header_t));

    header = (ngx_proxy_protocol_header_t *) buf;
    header->len[0] = (u_char) (len >> 8);
    header->len[1] = (u_char) len;
}


ಸ್ಥಿರ u_char *
ngx_proxy_protocol_v2_write_crc32c(ngx_connection_t *c, u_char *buf,
    u_char *p, u_char *last)
{
    u_char                    *ಮೌಲ್ಯ;
    uint32_t                   crc;
    ngx_proxy_protocol_tlv_t  *tlv;

    ಯದಿ ((size_t) (last - p) < ಪರಿಮಾಣ(ngx_proxy_protocol_tlv_t) + 4) {
        ngx_log_error(NGX_LOG_ALERT, c->log, 0,
                      "too small buffer for PROXY protocol v2 CRC32c TLV");
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    /* append the CRC32c TLV with a zeroed value field */

    tlv = (ngx_proxy_protocol_tlv_t *) p;
    tlv->type = NGX_PROXY_PROTOCOL_TLV_CRC32C;
    tlv->len[0] = 0;
    tlv->len[1] = 4;

    ಮೌಲ್ಯ = p + ಪರಿಮಾಣ(ngx_proxy_protocol_tlv_t);

    ngx_memzero(ಮೌಲ್ಯ, 4);

    p = ಮೌಲ್ಯ + 4;

    /* the checksum covers the entire header with the zeroed value */

    crc = ngx_crc32c_long(buf, p - buf);

    ಮೌಲ್ಯ[0] = (u_char) (crc >> 24);
    ಮೌಲ್ಯ[1] = (u_char) (crc >> 16);
    ಮೌಲ್ಯ[2] = (u_char) (crc >> 8);
    ಮೌಲ್ಯ[3] = (u_char) crc;

    ಪ್ರತ್ಯಾಗಮ p;
}
