
/*
 * Copyright (C) Roman Arutyunyan
 * Copyright (C) Nginx, Inc.
 */


#അന്തര്ഭാവ <ngx_config.h>
#അന്തര്ഭാവ <ngx_core.h>


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
#define NGX_PROXY_PROTOCOL_TLV_SSL_താക്കോൽ_ALG  0x25
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


തരനിർവചനം ഘടന {
    u_char                                  signature[12];
    u_char                                  version_command;
    u_char                                  family_transport;
    u_char                                  len[2];
} ngx_proxy_protocol_header_t;


തരനിർവചനം ഘടന {
    u_char                                  src_addr[4];
    u_char                                  dst_addr[4];
    u_char                                  src_port[2];
    u_char                                  dst_port[2];
} ngx_proxy_protocol_inet_addrs_t;


തരനിർവചനം ഘടന {
    u_char                                  src_addr[16];
    u_char                                  dst_addr[16];
    u_char                                  src_port[2];
    u_char                                  dst_port[2];
} ngx_proxy_protocol_inet6_addrs_t;


തരനിർവചനം ഘടന {
    u_char                                  type;
    u_char                                  len[2];
} ngx_proxy_protocol_tlv_t;


തരനിർവചനം ഘടന {
    u_char                                  client;
    u_char                                  verify[4];
} ngx_proxy_protocol_tlv_ssl_t;


തരനിർവചനം ഘടന {
    ngx_str_t                               name;
    ngx_uint_t                              type;
} ngx_proxy_protocol_tlv_entry_t;


തരനിർവചനം ഘടന {
    ngx_uint_t                              type;
    ngx_str_t                               മൂല്യം;
} ngx_proxy_protocol_tlv_മൂല്യം_t;


സ്ഥിതം u_char *ngx_proxy_protocol_read_addr(ngx_connection_t *c, u_char *p,
    u_char *last, ngx_str_t *addr);
സ്ഥിതം u_char *ngx_proxy_protocol_read_port(u_char *p, u_char *last,
    in_port_t *port, u_char sep);
സ്ഥിതം u_char *ngx_proxy_protocol_v2_read(ngx_connection_t *c, u_char *buf,
    u_char *last);
സ്ഥിതം ngx_int_t ngx_proxy_protocol_lookup_tlv(ngx_connection_t *c,
    ngx_str_t *tlvs, ngx_uint_t type, ngx_str_t *മൂല്യം);
#എങ്കിൽ (NGX_OPENSSL)
സ്ഥിതം ngx_int_t ngx_proxy_protocol_v2_eval_ssl(ngx_connection_t *c,
    ngx_array_t *tlvs, ngx_array_t *ssl_tlvs, ngx_uint_t *client,
    uint32_t *verify);
സ്ഥിതം ngx_int_t ngx_proxy_protocol_v2_authority(ngx_connection_t *c,
    ngx_str_t *out);
സ്ഥിതം ngx_int_t ngx_proxy_protocol_v2_alpn(ngx_connection_t *c,
    ngx_str_t *out);
സ്ഥിതം ngx_int_t ngx_proxy_protocol_v2_ssl_sub(ngx_connection_t *c,
    ngx_uint_t type, ngx_str_t *out);
#endif
സ്ഥിതം u_char *ngx_proxy_protocol_v2_write_header(ngx_connection_t *c,
    u_char *buf, u_char *last);
സ്ഥിതം u_char ngx_proxy_protocol_v2_family(ngx_uint_t family);
സ്ഥിതം ശൂന്യം ngx_proxy_protocol_v2_write_ipv4(ഘടന sockaddr *sa, u_char *addr,
    u_char *port);
സ്ഥിതം ശൂന്യം ngx_proxy_protocol_v2_write_ipv6(ഘടന sockaddr *sa, u_char *addr,
    u_char *port);
സ്ഥിതം u_char *ngx_proxy_protocol_v2_write_tlv(ngx_connection_t *c, u_char *p,
    u_char *last, ngx_uint_t type, ngx_str_t *മൂല്യം);
സ്ഥിതം u_char *ngx_proxy_protocol_v2_write_ssl(ngx_connection_t *c, u_char *p,
    u_char *last, ngx_array_t *ssl_tlvs, ngx_uint_t client, uint32_t verify);
സ്ഥിതം ശൂന്യം ngx_proxy_protocol_v2_set_len(u_char *buf, u_char *p);
സ്ഥിതം u_char *ngx_proxy_protocol_v2_write_crc32c(ngx_connection_t *c,
    u_char *buf, u_char *p, u_char *last);


സ്ഥിതം ngx_proxy_protocol_tlv_entry_t  ngx_proxy_protocol_tlv_entries[] = {
    { ngx_string("alpn"),       NGX_PROXY_PROTOCOL_TLV_ALPN },
    { ngx_string("authority"),  NGX_PROXY_PROTOCOL_TLV_AUTHORITY },
    { ngx_string("unique_id"),  NGX_PROXY_PROTOCOL_TLV_UNIQUE_ID },
    { ngx_string("ssl"),        NGX_PROXY_PROTOCOL_TLV_SSL },
    { ngx_string("netns"),      NGX_PROXY_PROTOCOL_TLV_NETNS },
    { ngx_null_string,          0x00 }
};


സ്ഥിതം ngx_proxy_protocol_tlv_entry_t  ngx_proxy_protocol_tlv_ssl_entries[] = {
    { ngx_string("version"),    NGX_PROXY_PROTOCOL_TLV_SSL_VERSION },
    { ngx_string("cn"),         NGX_PROXY_PROTOCOL_TLV_SSL_CN },
    { ngx_string("cipher"),     NGX_PROXY_PROTOCOL_TLV_SSL_CIPHER },
    { ngx_string("sig_alg"),    NGX_PROXY_PROTOCOL_TLV_SSL_SIG_ALG },
    { ngx_string("key_alg"),    NGX_PROXY_PROTOCOL_TLV_SSL_താക്കോൽ_ALG },
    { ngx_null_string,          0x00 }
};


സ്ഥിതം സ്ഥിരം u_char  ngx_proxy_protocol_signature[] = "\r\n\r\n\0\r\nQUIT\n";


u_char *
ngx_proxy_protocol_read(ngx_connection_t *c, u_char *buf, u_char *last)
{
    size_t                 len;
    u_char                *p;
    ngx_proxy_protocol_t  *pp;

    p = buf;
    len = last - buf;

    എങ്കിൽ (len >= വലുപ്പം(ngx_proxy_protocol_header_t)
        && ngx_memcmp(p, ngx_proxy_protocol_signature,
                      വലുപ്പം(ngx_proxy_protocol_signature) - 1)
           == 0)
    {
        തിരികെ ngx_proxy_protocol_v2_read(c, buf, last);
    }

    എങ്കിൽ (len < 8 || ngx_strncmp(p, "PROXY ", 6) != 0) {
        പോകുക invalid;
    }

    p += 6;
    len -= 6;

    എങ്കിൽ (len >= 7 && ngx_strncmp(p, "UNKNOWN", 7) == 0) {
        ngx_log_debug0(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "PROXY protocol unknown protocol");
        p += 7;
        പോകുക skip;
    }

    എങ്കിൽ (len < 5 || ngx_strncmp(p, "TCP", 3) != 0
        || (p[3] != '4' && p[3] != '6') || p[4] != ' ')
    {
        പോകുക invalid;
    }

    p += 5;

    pp = ngx_pcalloc(c->pool, വലുപ്പം(ngx_proxy_protocol_t));
    എങ്കിൽ (pp == ശൂന്യ) {
        തിരികെ ശൂന്യ;
    }

    p = ngx_proxy_protocol_read_addr(c, p, last, &pp->src_addr);
    എങ്കിൽ (p == ശൂന്യ) {
        പോകുക invalid;
    }

    p = ngx_proxy_protocol_read_addr(c, p, last, &pp->dst_addr);
    എങ്കിൽ (p == ശൂന്യ) {
        പോകുക invalid;
    }

    p = ngx_proxy_protocol_read_port(p, last, &pp->src_port, ' ');
    എങ്കിൽ (p == ശൂന്യ) {
        പോകുക invalid;
    }

    p = ngx_proxy_protocol_read_port(p, last, &pp->dst_port, CR);
    എങ്കിൽ (p == ശൂന്യ) {
        പോകുക invalid;
    }

    എങ്കിൽ (p == last) {
        പോകുക invalid;
    }

    എങ്കിൽ (*p++ != LF) {
        പോകുക invalid;
    }

    ngx_log_debug4(NGX_LOG_DEBUG_CORE, c->log, 0,
                   "PROXY protocol src: %V %d, dst: %V %d",
                   &pp->src_addr, pp->src_port, &pp->dst_addr, pp->dst_port);

    c->proxy_protocol = pp;

    തിരികെ p;

skip:

    വേണ്ടി ( /* void */ ; p < last - 1; p++) {
        എങ്കിൽ (p[0] == CR && p[1] == LF) {
            തിരികെ p + 2;
        }
    }

invalid:

    വേണ്ടി (p = buf; p < last; p++) {
        എങ്കിൽ (*p == CR || *p == LF) {
            നിർത്തുക;
        }
    }

    ngx_log_error(NGX_LOG_ERR, c->log, 0,
                  "broken header: \"%*s\"", (size_t) (p - buf), buf);

    തിരികെ ശൂന്യ;
}


സ്ഥിതം u_char *
ngx_proxy_protocol_read_addr(ngx_connection_t *c, u_char *p, u_char *last,
    ngx_str_t *addr)
{
    size_t  len;
    u_char  ch, *pos;

    pos = p;

    വേണ്ടി ( ;; ) {
        എങ്കിൽ (p == last) {
            തിരികെ ശൂന്യ;
        }

        ch = *p++;

        എങ്കിൽ (ch == ' ') {
            നിർത്തുക;
        }

        എങ്കിൽ (ch != ':' && ch != '.'
            && (ch < 'a' || ch > 'f')
            && (ch < 'A' || ch > 'F')
            && (ch < '0' || ch > '9'))
        {
            തിരികെ ശൂന്യ;
        }
    }

    len = p - pos - 1;

    addr->data = ngx_pnalloc(c->pool, len);
    എങ്കിൽ (addr->data == ശൂന്യ) {
        തിരികെ ശൂന്യ;
    }

    ngx_memcpy(addr->data, pos, len);
    addr->len = len;

    തിരികെ p;
}


സ്ഥിതം u_char *
ngx_proxy_protocol_read_port(u_char *p, u_char *last, in_port_t *port,
    u_char sep)
{
    size_t      len;
    u_char     *pos;
    ngx_int_t   n;

    pos = p;

    വേണ്ടി ( ;; ) {
        എങ്കിൽ (p == last) {
            തിരികെ ശൂന്യ;
        }

        എങ്കിൽ (*p++ == sep) {
            നിർത്തുക;
        }
    }

    len = p - pos - 1;

    n = ngx_atoi(pos, len);
    എങ്കിൽ (n < 0 || n > 65535) {
        തിരികെ ശൂന്യ;
    }

    *port = (in_port_t) n;

    തിരികെ p;
}


u_char *
ngx_proxy_protocol_write(ngx_connection_t *c, u_char *buf, u_char *last)
{
    ngx_uint_t  port, lport;

    എങ്കിൽ (last - buf < NGX_PROXY_PROTOCOL_V1_MAX_HEADER) {
        ngx_log_error(NGX_LOG_ALERT, c->log, 0,
                      "too small buffer for PROXY protocol");
        തിരികെ ശൂന്യ;
    }

    എങ്കിൽ (ngx_connection_local_sockaddr(c, ശൂന്യ, 0) != NGX_OK) {
        തിരികെ ശൂന്യ;
    }

    തിരഞ്ഞെടുപ്പ് (c->sockaddr->sa_family) {

    അവസ്ഥ AF_INET:
        buf = ngx_cpymem(buf, "PROXY TCP4 ", വലുപ്പം("PROXY TCP4 ") - 1);
        നിർത്തുക;

#എങ്കിൽ (NGX_HAVE_INET6)
    അവസ്ഥ AF_INET6:
        buf = ngx_cpymem(buf, "PROXY TCP6 ", വലുപ്പം("PROXY TCP6 ") - 1);
        നിർത്തുക;
#endif

    സ്ഥിരസ്ഥിതി:
        തിരികെ ngx_cpymem(buf, "PROXY UNKNOWN" CRLF,
                          വലുപ്പം("PROXY UNKNOWN" CRLF) - 1);
    }

    buf += ngx_sock_ntop(c->sockaddr, c->socklen, buf, last - buf, 0);

    *buf++ = ' ';

    buf += ngx_sock_ntop(c->local_sockaddr, c->local_socklen, buf, last - buf,
                         0);

    port = ngx_inet_get_port(c->sockaddr);
    lport = ngx_inet_get_port(c->local_sockaddr);

    തിരികെ ngx_slprintf(buf, last, " %ui %ui" CRLF, port, lport);
}


സ്ഥിതം u_char *
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
#എങ്കിൽ (NGX_HAVE_INET6)
    ngx_proxy_protocol_inet6_addrs_t   *in6;
#endif

    header = (ngx_proxy_protocol_header_t *) buf;

    buf += വലുപ്പം(ngx_proxy_protocol_header_t);

    version = header->version_command >> 4;

    എങ്കിൽ (version != 2) {
        ngx_log_error(NGX_LOG_ERR, c->log, 0,
                      "unknown PROXY protocol version: %ui", version);
        തിരികെ ശൂന്യ;
    }

    len = ngx_proxy_protocol_parse_uint16(header->len);

    എങ്കിൽ ((size_t) (last - buf) < len) {
        ngx_log_error(NGX_LOG_ERR, c->log, 0, "header is too large");
        തിരികെ ശൂന്യ;
    }

    end = buf + len;

    command = header->version_command & 0x0f;

    /* only PROXY is supported */
    എങ്കിൽ (command != NGX_PROXY_PROTOCOL_CMD_PROXY) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "PROXY protocol v2 unsupported command %ui", command);
        തിരികെ end;
    }

    transport = header->family_transport & 0x0f;

    /* only STREAM is supported */
    എങ്കിൽ (transport != NGX_PROXY_PROTOCOL_TYPE_STREAM) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "PROXY protocol v2 unsupported transport %ui",
                       transport);
        തിരികെ end;
    }

    pp = ngx_pcalloc(c->pool, വലുപ്പം(ngx_proxy_protocol_t));
    എങ്കിൽ (pp == ശൂന്യ) {
        തിരികെ ശൂന്യ;
    }

    family = header->family_transport >> 4;

    തിരഞ്ഞെടുപ്പ് (family) {

    അവസ്ഥ NGX_PROXY_PROTOCOL_AF_INET:

        എങ്കിൽ ((size_t) (end - buf) < വലുപ്പം(ngx_proxy_protocol_inet_addrs_t)) {
            തിരികെ ശൂന്യ;
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

        socklen = വലുപ്പം(ഘടന sockaddr_in);

        buf += വലുപ്പം(ngx_proxy_protocol_inet_addrs_t);

        നിർത്തുക;

#എങ്കിൽ (NGX_HAVE_INET6)

    അവസ്ഥ NGX_PROXY_PROTOCOL_AF_INET6:

        എങ്കിൽ ((size_t) (end - buf) < വലുപ്പം(ngx_proxy_protocol_inet6_addrs_t)) {
            തിരികെ ശൂന്യ;
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

        socklen = വലുപ്പം(ഘടന sockaddr_in6);

        buf += വലുപ്പം(ngx_proxy_protocol_inet6_addrs_t);

        നിർത്തുക;

#endif

    സ്ഥിരസ്ഥിതി:
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "PROXY protocol v2 unsupported address family %ui",
                       family);
        തിരികെ end;
    }

    pp->src_addr.data = ngx_pnalloc(c->pool, NGX_SOCKADDR_STRLEN);
    എങ്കിൽ (pp->src_addr.data == ശൂന്യ) {
        തിരികെ ശൂന്യ;
    }

    pp->src_addr.len = ngx_sock_ntop(&src_sockaddr.sockaddr, socklen,
                                     pp->src_addr.data, NGX_SOCKADDR_STRLEN, 0);

    pp->dst_addr.data = ngx_pnalloc(c->pool, NGX_SOCKADDR_STRLEN);
    എങ്കിൽ (pp->dst_addr.data == ശൂന്യ) {
        തിരികെ ശൂന്യ;
    }

    pp->dst_addr.len = ngx_sock_ntop(&dst_sockaddr.sockaddr, socklen,
                                     pp->dst_addr.data, NGX_SOCKADDR_STRLEN, 0);

    ngx_log_debug4(NGX_LOG_DEBUG_CORE, c->log, 0,
                   "PROXY protocol v2 src: %V %d, dst: %V %d",
                   &pp->src_addr, pp->src_port, &pp->dst_addr, pp->dst_port);

    എങ്കിൽ (buf < end) {
        pp->tlvs.data = ngx_pnalloc(c->pool, end - buf);
        എങ്കിൽ (pp->tlvs.data == ശൂന്യ) {
            തിരികെ ശൂന്യ;
        }

        ngx_memcpy(pp->tlvs.data, buf, end - buf);
        pp->tlvs.len = end - buf;
    }

    c->proxy_protocol = pp;

    തിരികെ end;
}


ngx_int_t
ngx_proxy_protocol_get_tlv(ngx_connection_t *c, ngx_str_t *name,
    ngx_str_t *മൂല്യം)
{
    u_char                          *p;
    size_t                           n;
    uint32_t                         verify;
    ngx_str_t                        ssl, *tlvs;
    ngx_int_t                        rc, type;
    ngx_proxy_protocol_tlv_ssl_t    *tlv_ssl;
    ngx_proxy_protocol_tlv_entry_t  *te;

    എങ്കിൽ (c->proxy_protocol == ശൂന്യ) {
        തിരികെ NGX_DECLINED;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                   "PROXY protocol v2 get tlv \"%V\"", name);

    te = ngx_proxy_protocol_tlv_entries;
    tlvs = &c->proxy_protocol->tlvs;

    p = name->data;
    n = name->len;

    എങ്കിൽ (n >= 4 && p[0] == 's' && p[1] == 's' && p[2] == 'l' && p[3] == '_') {

        rc = ngx_proxy_protocol_lookup_tlv(c, tlvs,
                                           NGX_PROXY_PROTOCOL_TLV_SSL, &ssl);
        എങ്കിൽ (rc != NGX_OK) {
            തിരികെ rc;
        }

        എങ്കിൽ (ssl.len < വലുപ്പം(ngx_proxy_protocol_tlv_ssl_t)) {
            തിരികെ NGX_ERROR;
        }

        p += 4;
        n -= 4;

        എങ്കിൽ (n == 6 && ngx_strncmp(p, "verify", 6) == 0) {

            tlv_ssl = (ngx_proxy_protocol_tlv_ssl_t *) ssl.data;
            verify = ngx_proxy_protocol_parse_uint32(tlv_ssl->verify);

            മൂല്യം->data = ngx_pnalloc(c->pool, NGX_INT32_LEN);
            എങ്കിൽ (മൂല്യം->data == ശൂന്യ) {
                തിരികെ NGX_ERROR;
            }

            മൂല്യം->len = ngx_sprintf(മൂല്യം->data, "%uD", verify)
                         - മൂല്യം->data;
            തിരികെ NGX_OK;
        }

        ssl.data += വലുപ്പം(ngx_proxy_protocol_tlv_ssl_t);
        ssl.len -= വലുപ്പം(ngx_proxy_protocol_tlv_ssl_t);

        te = ngx_proxy_protocol_tlv_ssl_entries;
        tlvs = &ssl;
    }

    എങ്കിൽ (n >= 2 && p[0] == '0' && p[1] == 'x') {

        type = ngx_hextoi(p + 2, n - 2);
        എങ്കിൽ (type == NGX_ERROR) {
            ngx_log_error(NGX_LOG_ERR, c->log, 0,
                          "invalid PROXY protocol TLV \"%V\"", name);
            തിരികെ NGX_ERROR;
        }

        തിരികെ ngx_proxy_protocol_lookup_tlv(c, tlvs, type, മൂല്യം);
    }

    വേണ്ടി ( /* void */ ; te->type; te++) {
        എങ്കിൽ (te->name.len == n && ngx_strncmp(te->name.data, p, n) == 0) {
            തിരികെ ngx_proxy_protocol_lookup_tlv(c, tlvs, te->type, മൂല്യം);
        }
    }

    ngx_log_error(NGX_LOG_ERR, c->log, 0,
                  "unknown PROXY protocol TLV \"%V\"", name);

    തിരികെ NGX_DECLINED;
}


സ്ഥിതം ngx_int_t
ngx_proxy_protocol_lookup_tlv(ngx_connection_t *c, ngx_str_t *tlvs,
    ngx_uint_t type, ngx_str_t *മൂല്യം)
{
    u_char                    *p;
    size_t                     n, len;
    ngx_proxy_protocol_tlv_t  *tlv;

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                   "PROXY protocol v2 lookup tlv:%02xi", type);

    p = tlvs->data;
    n = tlvs->len;

    ആകുമ്പോൾ (n) {
        എങ്കിൽ (n < വലുപ്പം(ngx_proxy_protocol_tlv_t)) {
            ngx_log_error(NGX_LOG_ERR, c->log, 0, "broken PROXY protocol TLV");
            തിരികെ NGX_ERROR;
        }

        tlv = (ngx_proxy_protocol_tlv_t *) p;
        len = ngx_proxy_protocol_parse_uint16(tlv->len);

        p += വലുപ്പം(ngx_proxy_protocol_tlv_t);
        n -= വലുപ്പം(ngx_proxy_protocol_tlv_t);

        എങ്കിൽ (n < len) {
            ngx_log_error(NGX_LOG_ERR, c->log, 0, "broken PROXY protocol TLV");
            തിരികെ NGX_ERROR;
        }

        എങ്കിൽ (tlv->type == type) {
            മൂല്യം->data = p;
            മൂല്യം->len = len;
            തിരികെ NGX_OK;
        }

        p += len;
        n -= len;
    }

    തിരികെ NGX_DECLINED;
}


u_char *
ngx_proxy_protocol_v2_write(ngx_connection_t *c, u_char *buf, u_char *last,
    ngx_array_t *tlvs /* reserved */ )
{
    u_char                          *p;
    uint32_t                         verify;
    ngx_uint_t                       i, client;
    ngx_array_t                      base_tlvs, ssl_tlvs;
    ngx_proxy_protocol_tlv_മൂല്യം_t  *tlv;

    client = 0;
    verify = 0;

    ngx_memzero(&base_tlvs, വലുപ്പം(ngx_array_t));
    ngx_memzero(&ssl_tlvs, വലുപ്പം(ngx_array_t));

#എങ്കിൽ (NGX_OPENSSL)

    എങ്കിൽ (c->ssl != ശൂന്യ) {
        എങ്കിൽ (ngx_array_init(&base_tlvs, c->pool, 2,
                           വലുപ്പം(ngx_proxy_protocol_tlv_മൂല്യം_t))
            != NGX_OK)
        {
            തിരികെ ശൂന്യ;
        }

        എങ്കിൽ (ngx_array_init(&ssl_tlvs, c->pool, 5,
                           വലുപ്പം(ngx_proxy_protocol_tlv_മൂല്യം_t))
            != NGX_OK)
        {
            തിരികെ ശൂന്യ;
        }

        എങ്കിൽ (ngx_proxy_protocol_v2_eval_ssl(c, &base_tlvs, &ssl_tlvs, &client,
                                           &verify)
            != NGX_OK)
        {
            തിരികെ ശൂന്യ;
        }
    }

#endif

    p = ngx_proxy_protocol_v2_write_header(c, buf, last);
    എങ്കിൽ (p == ശൂന്യ) {
        തിരികെ ശൂന്യ;
    }

    tlv = base_tlvs.elts;

    വേണ്ടി (i = 0; i < base_tlvs.nelts; i++) {
        p = ngx_proxy_protocol_v2_write_tlv(c, p, last, tlv[i].type,
                                           &tlv[i].മൂല്യം);
        എങ്കിൽ (p == ശൂന്യ) {
            തിരികെ ശൂന്യ;
        }
    }

    എങ്കിൽ (client) {
        p = ngx_proxy_protocol_v2_write_ssl(c, p, last, &ssl_tlvs, client,
                                            verify);
        എങ്കിൽ (p == ശൂന്യ) {
            തിരികെ ശൂന്യ;
        }
    }

    ngx_proxy_protocol_v2_set_len(buf,
                                  p + വലുപ്പം(ngx_proxy_protocol_tlv_t) + 4);

    തിരികെ ngx_proxy_protocol_v2_write_crc32c(c, buf, p, last);
}


#എങ്കിൽ (NGX_OPENSSL)

സ്ഥിതം ngx_int_t
ngx_proxy_protocol_v2_eval_ssl(ngx_connection_t *c, ngx_array_t *tlvs,
    ngx_array_t *ssl_tlvs, ngx_uint_t *client, uint32_t *verify)
{
    X509                            *cert;
    long                             n;
    ngx_int_t                        rc;
    ngx_str_t                        മൂല്യം;
    ngx_uint_t                       type;
    സ്ഥിരം അക്ഷരം                      *s;
    ngx_proxy_protocol_tlv_മൂല്യം_t  *tlv;

    എങ്കിൽ (ngx_proxy_protocol_v2_authority(c, &മൂല്യം) == NGX_OK) {
        tlv = ngx_array_തള്ളുക(tlvs);
        എങ്കിൽ (tlv == ശൂന്യ) {
            തിരികെ NGX_ERROR;
        }

        tlv->type = NGX_PROXY_PROTOCOL_TLV_AUTHORITY;
        tlv->മൂല്യം = മൂല്യം;
    }

    എങ്കിൽ (ngx_proxy_protocol_v2_alpn(c, &മൂല്യം) == NGX_OK) {
        tlv = ngx_array_തള്ളുക(tlvs);
        എങ്കിൽ (tlv == ശൂന്യ) {
            തിരികെ NGX_ERROR;
        }

        tlv->type = NGX_PROXY_PROTOCOL_TLV_ALPN;
        tlv->മൂല്യം = മൂല്യം;
    }

    വേണ്ടി (type = NGX_PROXY_PROTOCOL_TLV_SSL_VERSION;
         type <= NGX_PROXY_PROTOCOL_TLV_SSL_താക്കോൽ_ALG;
         type++)
    {
        rc = ngx_proxy_protocol_v2_ssl_sub(c, type, &മൂല്യം);

        എങ്കിൽ (rc == NGX_ERROR) {
            തിരികെ NGX_ERROR;
        }

        എങ്കിൽ (rc != NGX_OK) {
            തുടരുക;
        }

        tlv = ngx_array_തള്ളുക(ssl_tlvs);
        എങ്കിൽ (tlv == ശൂന്യ) {
            തിരികെ NGX_ERROR;
        }

        tlv->type = type;
        tlv->മൂല്യം = മൂല്യം;
    }

    *client = NGX_PROXY_PROTOCOL_V2_CLIENT_SSL;
    *verify = 1;  /* X509_V_ERR_UNSPECIFIED */

    cert = SSL_get_peer_certificate(c->ssl->connection);

    എങ്കിൽ (cert != ശൂന്യ) {
        X509_free(cert);

        *client |= NGX_PROXY_PROTOCOL_V2_CLIENT_CERT_SESS;

        എങ്കിൽ (!SSL_session_reused(c->ssl->connection)) {
            *client |= NGX_PROXY_PROTOCOL_V2_CLIENT_CERT_CONN;
        }

        n = SSL_get_verify_result(c->ssl->connection);

        എങ്കിൽ (n == X509_V_OK) {

            എങ്കിൽ (ngx_ssl_ocsp_get_status(c, &s) != NGX_OK) {
                n = X509_V_ERR_CERT_REVOKED;
            }
        }

        *verify = (uint32_t) n;
    }

    തിരികെ NGX_OK;
}


സ്ഥിതം ngx_int_t
ngx_proxy_protocol_v2_authority(ngx_connection_t *c, ngx_str_t *out)
{
#ifdef SSL_CTRL_SET_TLSEXT_HOSTNAME

    സ്ഥിരം അക്ഷരം  *sni;

    sni = SSL_get_servername(c->ssl->connection, TLSEXT_NAMETYPE_host_name);
    എങ്കിൽ (sni == ശൂന്യ) {
        തിരികെ NGX_DECLINED;
    }

    out->data = (u_char *) sni;
    out->len = ngx_strlen(sni);
    തിരികെ NGX_OK;

#അല്ലെങ്കിൽ
    തിരികെ NGX_DECLINED;
#endif
}


സ്ഥിതം ngx_int_t
ngx_proxy_protocol_v2_alpn(ngx_connection_t *c, ngx_str_t *out)
{
#ifdef TLSEXT_TYPE_application_layer_protocol_negotiation

    unsigned പൂർണ്ണസംഖ്യ          alpnlen;
    സ്ഥിരം unsigned അക്ഷരം  *alpn;

    SSL_get0_alpn_selected(c->ssl->connection, &alpn, &alpnlen);
    എങ്കിൽ (alpn == ശൂന്യ || alpnlen == 0) {
        തിരികെ NGX_DECLINED;
    }

    out->data = (u_char *) alpn;
    out->len = alpnlen;
    തിരികെ NGX_OK;

#അല്ലെങ്കിൽ
    തിരികെ NGX_DECLINED;
#endif
}


സ്ഥിതം ngx_int_t
ngx_proxy_protocol_v2_ssl_sub(ngx_connection_t *c, ngx_uint_t type,
    ngx_str_t *out)
{
    പൂർണ്ണസംഖ്യ               i, len;
    X509             *cert;
    u_char           *alg, *s;
    EVP_PKEY         *pkey;
#എങ്കിൽ (OPENSSL_VERSION_സംഖ്യ >= 0x40000000L)
    സ്ഥിരം
#endif
    X509_NAME        *subject;
#എങ്കിൽ (OPENSSL_VERSION_സംഖ്യ >= 0x40000000L)
    സ്ഥിരം
#endif
    X509_NAME_ENTRY  *entry;

    തിരഞ്ഞെടുപ്പ് (type) {

    അവസ്ഥ NGX_PROXY_PROTOCOL_TLV_SSL_VERSION:
        out->data = (u_char *) SSL_get_version(c->ssl->connection);
        out->len = ngx_strlen(out->data);
        തിരികെ NGX_OK;

    അവസ്ഥ NGX_PROXY_PROTOCOL_TLV_SSL_CIPHER:
        out->data = (u_char *) SSL_get_cipher_name(c->ssl->connection);
        out->len = ngx_strlen(out->data);
        തിരികെ NGX_OK;

    അവസ്ഥ NGX_PROXY_PROTOCOL_TLV_SSL_CN:
        cert = SSL_get_peer_certificate(c->ssl->connection);
        എങ്കിൽ (cert == ശൂന്യ) {
            തിരികെ NGX_DECLINED;
        }

        subject = X509_get_subject_name(cert);
        എങ്കിൽ (subject == ശൂന്യ) {
            X509_free(cert);
            തിരികെ NGX_ERROR;
        }

        i = X509_NAME_get_index_by_NID(subject, NID_commonName, -1);
        എങ്കിൽ (i < 0) {
            X509_free(cert);
            തിരികെ NGX_DECLINED;
        }

        entry = X509_NAME_get_entry(subject, i);

        len = ASN1_STRING_to_UTF8(&s, X509_NAME_ENTRY_get_data(entry));
        എങ്കിൽ (len < 0) {
            ngx_ssl_error(NGX_LOG_ALERT, c->log, 0,
                          "ASN1_STRING_to_UTF8() failed");
            X509_free(cert);
            തിരികെ NGX_ERROR;
        }

        out->len = len;
        out->data = ngx_pnalloc(c->pool, len);
        എങ്കിൽ (out->data == ശൂന്യ) {
            OPENSSL_free(s);
            X509_free(cert);
            തിരികെ NGX_ERROR;
        }

        ngx_memcpy(out->data, s, len);

        OPENSSL_free(s);
        X509_free(cert);
        തിരികെ NGX_OK;

    അവസ്ഥ NGX_PROXY_PROTOCOL_TLV_SSL_SIG_ALG:
        cert = SSL_get_certificate(c->ssl->connection);

        out->data = (u_char *) OBJ_nid2sn(X509_get_signature_nid(cert));
        out->len = ngx_strlen(out->data);
        തിരികെ NGX_OK;

    അവസ്ഥ NGX_PROXY_PROTOCOL_TLV_SSL_താക്കോൽ_ALG:
        cert = SSL_get_certificate(c->ssl->connection);

        pkey = X509_get_pubkey(cert);
        എങ്കിൽ (pkey == ശൂന്യ) {
            തിരികെ NGX_ERROR;
        }

        തിരഞ്ഞെടുപ്പ് (EVP_PKEY_base_id(pkey)) {

        അവസ്ഥ EVP_PKEY_RSA:
#ifdef EVP_PKEY_RSA_PSS
        അവസ്ഥ EVP_PKEY_RSA_PSS:
#endif
            alg = (u_char *) "RSA";
            നിർത്തുക;

        അവസ്ഥ EVP_PKEY_EC:
            alg = (u_char *) "EC";
            നിർത്തുക;

        അവസ്ഥ EVP_PKEY_DSA:
            alg = (u_char *) "DSA";
            നിർത്തുക;

        സ്ഥിരസ്ഥിതി:
            EVP_PKEY_free(pkey);
            തിരികെ NGX_DECLINED;
        }

        out->len = വലുപ്പം("RSA16384") - 1;
        out->data = ngx_pnalloc(c->pool, out->len);
        എങ്കിൽ (out->data == ശൂന്യ) {
            EVP_PKEY_free(pkey);
            തിരികെ NGX_ERROR;
        }

        out->len = ngx_snprintf(out->data, out->len, "%s%d",
                                alg, EVP_PKEY_bits(pkey))
                   - out->data;

        EVP_PKEY_free(pkey);

        തിരികെ NGX_OK;
    }

    തിരികെ NGX_DECLINED;
}

#endif


സ്ഥിതം u_char *
ngx_proxy_protocol_v2_write_header(ngx_connection_t *c, u_char *buf,
    u_char *last)
{
    u_char                            *p, src_af, dst_af,
                                       family, command, transport;
    ngx_uint_t                         src_fam, dst_fam;
    ngx_proxy_protocol_header_t       *header;
    ngx_proxy_protocol_inet_addrs_t   *in;
#എങ്കിൽ (NGX_HAVE_INET6)
    ngx_uint_t                         port, lport;
    ഘടന in6_addr                   *src6, *dst6;
    ngx_proxy_protocol_inet6_addrs_t  *in6;
#endif

    എങ്കിൽ (last - buf < NGX_PROXY_PROTOCOL_V2_MAX_HEADER) {
        ngx_log_error(NGX_LOG_ALERT, c->log, 0,
                      "too small buffer for PROXY protocol v2");
        തിരികെ ശൂന്യ;
    }

    എങ്കിൽ (ngx_connection_local_sockaddr(c, ശൂന്യ, 0) != NGX_OK) {
        തിരികെ ശൂന്യ;
    }

    /* the length field is set by the caller once all TLVs are written */

    header = (ngx_proxy_protocol_header_t *) buf;

    ngx_memcpy(header->signature, ngx_proxy_protocol_signature,
               വലുപ്പം(ngx_proxy_protocol_signature) - 1);

    p = buf + വലുപ്പം(ngx_proxy_protocol_header_t);

    src_fam = c->sockaddr->sa_family;
    dst_fam = c->local_sockaddr->sa_family;

    src_af = ngx_proxy_protocol_v2_family(src_fam);
    dst_af = ngx_proxy_protocol_v2_family(dst_fam);

    /* promote to the highest address family present on either side */

    എങ്കിൽ (src_af == NGX_PROXY_PROTOCOL_AF_UNSPEC
        || dst_af == NGX_PROXY_PROTOCOL_AF_UNSPEC)
    {
        command = NGX_PROXY_PROTOCOL_CMD_LOCAL;
        family = NGX_PROXY_PROTOCOL_AF_UNSPEC;
        transport = NGX_PROXY_PROTOCOL_TYPE_UNSPEC;

    } അല്ലെങ്കിൽ {
        command = NGX_PROXY_PROTOCOL_CMD_PROXY;
        family = ngx_max(src_af, dst_af);

        തിരഞ്ഞെടുപ്പ് (c->type) {
        അവസ്ഥ SOCK_STREAM:
            transport = NGX_PROXY_PROTOCOL_TYPE_STREAM;
            നിർത്തുക;
        അവസ്ഥ SOCK_DGRAM:
            transport = NGX_PROXY_PROTOCOL_TYPE_DGRAM;
            നിർത്തുക;
        സ്ഥിരസ്ഥിതി:
            transport = NGX_PROXY_PROTOCOL_TYPE_UNSPEC;
            നിർത്തുക;
        }
    }

    header->version_command = 0x20 | command;
    header->family_transport = (family << 4) | transport;

    തിരഞ്ഞെടുപ്പ് (family) {

    അവസ്ഥ NGX_PROXY_PROTOCOL_AF_INET:

        in = (ngx_proxy_protocol_inet_addrs_t *) p;

        ngx_proxy_protocol_v2_write_ipv4(c->sockaddr,
                                         in->src_addr, in->src_port);
        ngx_proxy_protocol_v2_write_ipv4(c->local_sockaddr,
                                         in->dst_addr, in->dst_port);

        p += വലുപ്പം(ngx_proxy_protocol_inet_addrs_t);

        നിർത്തുക;

#എങ്കിൽ (NGX_HAVE_INET6)

    അവസ്ഥ NGX_PROXY_PROTOCOL_AF_INET6:

        എങ്കിൽ (src_fam == AF_INET6 && dst_fam == AF_INET6) {
            src6 = &((ഘടന sockaddr_in6 *) c->sockaddr)->sin6_addr;
            dst6 = &((ഘടന sockaddr_in6 *) c->local_sockaddr)->sin6_addr;

            എങ്കിൽ (IN6_IS_ADDR_V4MAPPED(src6) && IN6_IS_ADDR_V4MAPPED(dst6)) {

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

                p += വലുപ്പം(ngx_proxy_protocol_inet_addrs_t);
                നിർത്തുക;
            }
        }

        in6 = (ngx_proxy_protocol_inet6_addrs_t *) p;

        ngx_proxy_protocol_v2_write_ipv6(c->sockaddr,
                                         in6->src_addr, in6->src_port);
        ngx_proxy_protocol_v2_write_ipv6(c->local_sockaddr,
                                         in6->dst_addr, in6->dst_port);

        p += വലുപ്പം(ngx_proxy_protocol_inet6_addrs_t);

        നിർത്തുക;

#endif

    സ്ഥിരസ്ഥിതി:  /* NGX_PROXY_PROTOCOL_AF_UNSPEC */
        നിർത്തുക;
    }

    തിരികെ p;
}


സ്ഥിതം u_char
ngx_proxy_protocol_v2_family(ngx_uint_t family)
{
    തിരഞ്ഞെടുപ്പ് (family) {

    അവസ്ഥ AF_INET:
        തിരികെ NGX_PROXY_PROTOCOL_AF_INET;

#എങ്കിൽ (NGX_HAVE_INET6)
    അവസ്ഥ AF_INET6:
        തിരികെ NGX_PROXY_PROTOCOL_AF_INET6;
#endif

    സ്ഥിരസ്ഥിതി:
        തിരികെ NGX_PROXY_PROTOCOL_AF_UNSPEC;
    }
}


സ്ഥിതം ശൂന്യം
ngx_proxy_protocol_v2_write_ipv4(ഘടന sockaddr *sa, u_char *addr,
    u_char *port)
{
    ഘടന sockaddr_in  *sin;

    sin = (ഘടന sockaddr_in *) sa;
    ngx_memcpy(addr, &sin->sin_addr, 4);
    ngx_memcpy(port, &sin->sin_port, 2);
}


#എങ്കിൽ (NGX_HAVE_INET6)

സ്ഥിതം ശൂന്യം
ngx_proxy_protocol_v2_write_ipv6(ഘടന sockaddr *sa, u_char *addr,
    u_char *port)
{
    ഘടന sockaddr_in   *sin;
    ഘടന sockaddr_in6  *sin6;

    /* IPv4-mapped prefix: 80 zero bits + 16 one bits */
    സ്ഥിതം സ്ഥിരം u_char  mapped[]   = { 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0xff, 0xff };

    എങ്കിൽ (sa->sa_family == AF_INET6) {
        sin6 = (ഘടന sockaddr_in6 *) sa;
        ngx_memcpy(addr, &sin6->sin6_addr, 16);
        ngx_memcpy(port, &sin6->sin6_port, 2);

    } അല്ലെങ്കിൽ {
        /* promote AF_INET to ::ffff:a.b.c.d */
        sin = (ഘടന sockaddr_in *) sa;
        ngx_memcpy(addr, mapped, വലുപ്പം(mapped));
        ngx_memcpy(addr + വലുപ്പം(mapped), &sin->sin_addr, 4);
        ngx_memcpy(port, &sin->sin_port, 2);
    }
}

#endif


സ്ഥിതം u_char *
ngx_proxy_protocol_v2_write_tlv(ngx_connection_t *c, u_char *p, u_char *last,
    ngx_uint_t type, ngx_str_t *മൂല്യം)
{
    ngx_proxy_protocol_tlv_t  *tlv;

    എങ്കിൽ ((size_t) (last - p) < വലുപ്പം(ngx_proxy_protocol_tlv_t) + മൂല്യം->len) {
        ngx_log_error(NGX_LOG_ALERT, c->log, 0,
                      "too small buffer for PROXY protocol v2 TLV");
        തിരികെ ശൂന്യ;
    }

    tlv = (ngx_proxy_protocol_tlv_t *) p;
    tlv->type = (u_char) type;
    tlv->len[0] = (u_char) (മൂല്യം->len >> 8);
    tlv->len[1] = (u_char)  മൂല്യം->len;

    p += വലുപ്പം(ngx_proxy_protocol_tlv_t);

    തിരികെ ngx_cpymem(p, മൂല്യം->data, മൂല്യം->len);
}


സ്ഥിതം u_char *
ngx_proxy_protocol_v2_write_ssl(ngx_connection_t *c, u_char *p, u_char *last,
    ngx_array_t *ssl_tlvs, ngx_uint_t client, uint32_t verify)
{
    u_char                          *start;
    uint16_t                         len;
    ngx_uint_t                       i;
    ngx_proxy_protocol_tlv_t        *tlv;
    ngx_proxy_protocol_tlv_ssl_t    *tlv_ssl;
    ngx_proxy_protocol_tlv_മൂല്യം_t  *sub;

    എങ്കിൽ ((size_t) (last - p) < വലുപ്പം(ngx_proxy_protocol_tlv_t)
                              + വലുപ്പം(ngx_proxy_protocol_tlv_ssl_t))
    {
        ngx_log_error(NGX_LOG_ALERT, c->log, 0,
                      "too small buffer for PROXY protocol v2 SSL TLV");
        തിരികെ ശൂന്യ;
    }

    start = p;
    p += വലുപ്പം(ngx_proxy_protocol_tlv_t);

    tlv_ssl = (ngx_proxy_protocol_tlv_ssl_t *) p;
    tlv_ssl->client = (u_char) client;
    tlv_ssl->verify[0] = (u_char) (verify >> 24);
    tlv_ssl->verify[1] = (u_char) (verify >> 16);
    tlv_ssl->verify[2] = (u_char) (verify >> 8);
    tlv_ssl->verify[3] = (u_char)  verify;

    p += വലുപ്പം(ngx_proxy_protocol_tlv_ssl_t);

    sub = ssl_tlvs->elts;

    വേണ്ടി (i = 0; i < ssl_tlvs->nelts; i++) {
        p = ngx_proxy_protocol_v2_write_tlv(c, p, last, sub[i].type,
                                            &sub[i].മൂല്യം);
        എങ്കിൽ (p == ശൂന്യ) {
            തിരികെ ശൂന്യ;
        }
    }

    len = (uint16_t) (p - start - വലുപ്പം(ngx_proxy_protocol_tlv_t));

    tlv = (ngx_proxy_protocol_tlv_t *) start;
    tlv->type = NGX_PROXY_PROTOCOL_TLV_SSL;
    tlv->len[0] = (u_char) (len >> 8);
    tlv->len[1] = (u_char)  len;

    തിരികെ p;
}


സ്ഥിതം ശൂന്യം
ngx_proxy_protocol_v2_set_len(u_char *buf, u_char *p)
{
    uint16_t                      len;
    ngx_proxy_protocol_header_t  *header;

    len = (uint16_t) (p - buf - വലുപ്പം(ngx_proxy_protocol_header_t));

    header = (ngx_proxy_protocol_header_t *) buf;
    header->len[0] = (u_char) (len >> 8);
    header->len[1] = (u_char) len;
}


സ്ഥിതം u_char *
ngx_proxy_protocol_v2_write_crc32c(ngx_connection_t *c, u_char *buf,
    u_char *p, u_char *last)
{
    u_char                    *മൂല്യം;
    uint32_t                   crc;
    ngx_proxy_protocol_tlv_t  *tlv;

    എങ്കിൽ ((size_t) (last - p) < വലുപ്പം(ngx_proxy_protocol_tlv_t) + 4) {
        ngx_log_error(NGX_LOG_ALERT, c->log, 0,
                      "too small buffer for PROXY protocol v2 CRC32c TLV");
        തിരികെ ശൂന്യ;
    }

    /* append the CRC32c TLV with a zeroed value field */

    tlv = (ngx_proxy_protocol_tlv_t *) p;
    tlv->type = NGX_PROXY_PROTOCOL_TLV_CRC32C;
    tlv->len[0] = 0;
    tlv->len[1] = 4;

    മൂല്യം = p + വലുപ്പം(ngx_proxy_protocol_tlv_t);

    ngx_memzero(മൂല്യം, 4);

    p = മൂല്യം + 4;

    /* the checksum covers the entire header with the zeroed value */

    crc = ngx_crc32c_long(buf, p - buf);

    മൂല്യം[0] = (u_char) (crc >> 24);
    മൂല്യം[1] = (u_char) (crc >> 16);
    മൂല്യം[2] = (u_char) (crc >> 8);
    മൂല്യം[3] = (u_char) crc;

    തിരികെ p;
}
