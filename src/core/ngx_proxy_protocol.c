
/*
 * Copyright (C) Roman Arutyunyan
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


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
#define NGX_PROXY_PROTOCOL_TLV_SSL_KEY_ALG  0x25
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


تعريفنوع بنية {
    u_char                                  signature[12];
    u_char                                  version_command;
    u_char                                  family_transport;
    u_char                                  len[2];
} ngx_proxy_protocol_header_t;


تعريفنوع بنية {
    u_char                                  src_addr[4];
    u_char                                  dst_addr[4];
    u_char                                  src_port[2];
    u_char                                  dst_port[2];
} ngx_proxy_protocol_inet_addrs_t;


تعريفنوع بنية {
    u_char                                  src_addr[16];
    u_char                                  dst_addr[16];
    u_char                                  src_port[2];
    u_char                                  dst_port[2];
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
} ngx_proxy_protocol_tlv_entry_t;


تعريفنوع بنية {
    ngx_uint_t                              type;
    ngx_str_t                               value;
} ngx_proxy_protocol_tlv_value_t;


ساكن u_char *ngx_proxy_protocol_read_addr(ngx_connection_t *c, u_char *p,
    u_char *last, ngx_str_t *addr);
ساكن u_char *ngx_proxy_protocol_read_port(u_char *p, u_char *last,
    in_port_t *port, u_char sep);
ساكن u_char *ngx_proxy_protocol_v2_read(ngx_connection_t *c, u_char *buf,
    u_char *last);
ساكن ngx_int_t ngx_proxy_protocol_lookup_tlv(ngx_connection_t *c,
    ngx_str_t *tlvs, ngx_uint_t type, ngx_str_t *value);
#אם (NGX_OPENSSL)
ساكن ngx_int_t ngx_proxy_protocol_v2_eval_ssl(ngx_connection_t *c,
    ngx_array_t *tlvs, ngx_array_t *ssl_tlvs, ngx_uint_t *client,
    uint32_t *verify);
ساكن ngx_int_t ngx_proxy_protocol_v2_authority(ngx_connection_t *c,
    ngx_str_t *out);
ساكن ngx_int_t ngx_proxy_protocol_v2_alpn(ngx_connection_t *c,
    ngx_str_t *out);
ساكن ngx_int_t ngx_proxy_protocol_v2_ssl_sub(ngx_connection_t *c,
    ngx_uint_t type, ngx_str_t *out);
#endif
ساكن u_char *ngx_proxy_protocol_v2_write_header(ngx_connection_t *c,
    u_char *buf, u_char *last);
ساكن u_char ngx_proxy_protocol_v2_family(ngx_uint_t family);
ساكن فراغ ngx_proxy_protocol_v2_write_ipv4(بنية sockaddr *sa, u_char *addr,
    u_char *port);
ساكن فراغ ngx_proxy_protocol_v2_write_ipv6(بنية sockaddr *sa, u_char *addr,
    u_char *port);
ساكن u_char *ngx_proxy_protocol_v2_write_tlv(ngx_connection_t *c, u_char *p,
    u_char *last, ngx_uint_t type, ngx_str_t *value);
ساكن u_char *ngx_proxy_protocol_v2_write_ssl(ngx_connection_t *c, u_char *p,
    u_char *last, ngx_array_t *ssl_tlvs, ngx_uint_t client, uint32_t verify);
ساكن فراغ ngx_proxy_protocol_v2_set_len(u_char *buf, u_char *p);
ساكن u_char *ngx_proxy_protocol_v2_write_crc32c(ngx_connection_t *c,
    u_char *buf, u_char *p, u_char *last);


ساكن ngx_proxy_protocol_tlv_entry_t  ngx_proxy_protocol_tlv_entries[] = {
    { ngx_string("alpn"),       NGX_PROXY_PROTOCOL_TLV_ALPN },
    { ngx_string("authority"),  NGX_PROXY_PROTOCOL_TLV_AUTHORITY },
    { ngx_string("unique_id"),  NGX_PROXY_PROTOCOL_TLV_UNIQUE_ID },
    { ngx_string("ssl"),        NGX_PROXY_PROTOCOL_TLV_SSL },
    { ngx_string("netns"),      NGX_PROXY_PROTOCOL_TLV_NETNS },
    { ngx_null_string,          0x00 }
};


ساكن ngx_proxy_protocol_tlv_entry_t  ngx_proxy_protocol_tlv_ssl_entries[] = {
    { ngx_string("version"),    NGX_PROXY_PROTOCOL_TLV_SSL_VERSION },
    { ngx_string("cn"),         NGX_PROXY_PROTOCOL_TLV_SSL_CN },
    { ngx_string("cipher"),     NGX_PROXY_PROTOCOL_TLV_SSL_CIPHER },
    { ngx_string("sig_alg"),    NGX_PROXY_PROTOCOL_TLV_SSL_SIG_ALG },
    { ngx_string("key_alg"),    NGX_PROXY_PROTOCOL_TLV_SSL_KEY_ALG },
    { ngx_null_string,          0x00 }
};


ساكن ثابت u_char  ngx_proxy_protocol_signature[] = "\r\n\r\n\0\r\nQUIT\n";


u_char *
ngx_proxy_protocol_read(ngx_connection_t *c, u_char *buf, u_char *last)
{
    size_t                 len;
    u_char                *p;
    ngx_proxy_protocol_t  *pp;

    p = buf;
    len = last - buf;

    אם (len >= حجم(ngx_proxy_protocol_header_t)
        && ngx_memcmp(p, ngx_proxy_protocol_signature,
                      حجم(ngx_proxy_protocol_signature) - 1)
           == 0)
    {
        החזר ngx_proxy_protocol_v2_read(c, buf, last);
    }

    אם (len < 8 || ngx_strncmp(p, "PROXY ", 6) != 0) {
        اذهب invalid;
    }

    p += 6;
    len -= 6;

    אם (len >= 7 && ngx_strncmp(p, "UNKNOWN", 7) == 0) {
        ngx_log_debug0(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "PROXY protocol unknown protocol");
        p += 7;
        اذهب skip;
    }

    אם (len < 5 || ngx_strncmp(p, "TCP", 3) != 0
        || (p[3] != '4' && p[3] != '6') || p[4] != ' ')
    {
        اذهب invalid;
    }

    p += 5;

    pp = ngx_pcalloc(c->pool, حجم(ngx_proxy_protocol_t));
    אם (pp == عدم) {
        החזר عدم;
    }

    p = ngx_proxy_protocol_read_addr(c, p, last, &pp->src_addr);
    אם (p == عدم) {
        اذهب invalid;
    }

    p = ngx_proxy_protocol_read_addr(c, p, last, &pp->dst_addr);
    אם (p == عدم) {
        اذهب invalid;
    }

    p = ngx_proxy_protocol_read_port(p, last, &pp->src_port, ' ');
    אם (p == عدم) {
        اذهب invalid;
    }

    p = ngx_proxy_protocol_read_port(p, last, &pp->dst_port, CR);
    אם (p == عدم) {
        اذهب invalid;
    }

    אם (p == last) {
        اذهب invalid;
    }

    אם (*p++ != LF) {
        اذهب invalid;
    }

    ngx_log_debug4(NGX_LOG_DEBUG_CORE, c->log, 0,
                   "PROXY protocol src: %V %d, dst: %V %d",
                   &pp->src_addr, pp->src_port, &pp->dst_addr, pp->dst_port);

    c->proxy_protocol = pp;

    החזר p;

skip:

    עבור ( /* void */ ; p < last - 1; p++) {
        אם (p[0] == CR && p[1] == LF) {
            החזר p + 2;
        }
    }

invalid:

    עבור (p = buf; p < last; p++) {
        אם (*p == CR || *p == LF) {
            اكسر;
        }
    }

    ngx_log_error(NGX_LOG_ERR, c->log, 0,
                  "broken header: \"%*s\"", (size_t) (p - buf), buf);

    החזר عدم;
}


ساكن u_char *
ngx_proxy_protocol_read_addr(ngx_connection_t *c, u_char *p, u_char *last,
    ngx_str_t *addr)
{
    size_t  len;
    u_char  ch, *pos;

    pos = p;

    עבור ( ;; ) {
        אם (p == last) {
            החזר عدم;
        }

        ch = *p++;

        אם (ch == ' ') {
            اكسر;
        }

        אם (ch != ':' && ch != '.'
            && (ch < 'a' || ch > 'f')
            && (ch < 'A' || ch > 'F')
            && (ch < '0' || ch > '9'))
        {
            החזר عدم;
        }
    }

    len = p - pos - 1;

    addr->data = ngx_pnalloc(c->pool, len);
    אם (addr->data == عدم) {
        החזר عدم;
    }

    ngx_memcpy(addr->data, pos, len);
    addr->len = len;

    החזר p;
}


ساكن u_char *
ngx_proxy_protocol_read_port(u_char *p, u_char *last, in_port_t *port,
    u_char sep)
{
    size_t      len;
    u_char     *pos;
    ngx_int_t   n;

    pos = p;

    עבור ( ;; ) {
        אם (p == last) {
            החזר عدم;
        }

        אם (*p++ == sep) {
            اكسر;
        }
    }

    len = p - pos - 1;

    n = ngx_atoi(pos, len);
    אם (n < 0 || n > 65535) {
        החזר عدم;
    }

    *port = (in_port_t) n;

    החזר p;
}


u_char *
ngx_proxy_protocol_write(ngx_connection_t *c, u_char *buf, u_char *last)
{
    ngx_uint_t  port, lport;

    אם (last - buf < NGX_PROXY_PROTOCOL_V1_MAX_HEADER) {
        ngx_log_error(NGX_LOG_ALERT, c->log, 0,
                      "too small buffer for PROXY protocol");
        החזר عدم;
    }

    אם (ngx_connection_local_sockaddr(c, عدم, 0) != NGX_OK) {
        החזר عدم;
    }

    بدل (c->sockaddr->sa_family) {

    حالة AF_INET:
        buf = ngx_cpymem(buf, "PROXY TCP4 ", حجم("PROXY TCP4 ") - 1);
        اكسر;

#אם (NGX_HAVE_INET6)
    حالة AF_INET6:
        buf = ngx_cpymem(buf, "PROXY TCP6 ", حجم("PROXY TCP6 ") - 1);
        اكسر;
#endif

    افتراضي:
        החזר ngx_cpymem(buf, "PROXY UNKNOWN" CRLF,
                          حجم("PROXY UNKNOWN" CRLF) - 1);
    }

    buf += ngx_sock_ntop(c->sockaddr, c->socklen, buf, last - buf, 0);

    *buf++ = ' ';

    buf += ngx_sock_ntop(c->local_sockaddr, c->local_socklen, buf, last - buf,
                         0);

    port = ngx_inet_get_port(c->sockaddr);
    lport = ngx_inet_get_port(c->local_sockaddr);

    החזר ngx_slprintf(buf, last, " %ui %ui" CRLF, port, lport);
}


ساكن u_char *
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
#אם (NGX_HAVE_INET6)
    ngx_proxy_protocol_inet6_addrs_t   *in6;
#endif

    header = (ngx_proxy_protocol_header_t *) buf;

    buf += حجم(ngx_proxy_protocol_header_t);

    version = header->version_command >> 4;

    אם (version != 2) {
        ngx_log_error(NGX_LOG_ERR, c->log, 0,
                      "unknown PROXY protocol version: %ui", version);
        החזר عدم;
    }

    len = ngx_proxy_protocol_parse_uint16(header->len);

    אם ((size_t) (last - buf) < len) {
        ngx_log_error(NGX_LOG_ERR, c->log, 0, "header is too large");
        החזר عدم;
    }

    end = buf + len;

    command = header->version_command & 0x0f;

    /* only PROXY is supported */
    אם (command != NGX_PROXY_PROTOCOL_CMD_PROXY) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "PROXY protocol v2 unsupported command %ui", command);
        החזר end;
    }

    transport = header->family_transport & 0x0f;

    /* only STREAM is supported */
    אם (transport != NGX_PROXY_PROTOCOL_TYPE_STREAM) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "PROXY protocol v2 unsupported transport %ui",
                       transport);
        החזר end;
    }

    pp = ngx_pcalloc(c->pool, حجم(ngx_proxy_protocol_t));
    אם (pp == عدم) {
        החזר عدم;
    }

    family = header->family_transport >> 4;

    بدل (family) {

    حالة NGX_PROXY_PROTOCOL_AF_INET:

        אם ((size_t) (end - buf) < حجم(ngx_proxy_protocol_inet_addrs_t)) {
            החזר عدم;
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

        socklen = حجم(بنية sockaddr_in);

        buf += حجم(ngx_proxy_protocol_inet_addrs_t);

        اكسر;

#אם (NGX_HAVE_INET6)

    حالة NGX_PROXY_PROTOCOL_AF_INET6:

        אם ((size_t) (end - buf) < حجم(ngx_proxy_protocol_inet6_addrs_t)) {
            החזר عدم;
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

        socklen = حجم(بنية sockaddr_in6);

        buf += حجم(ngx_proxy_protocol_inet6_addrs_t);

        اكسر;

#endif

    افتراضي:
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "PROXY protocol v2 unsupported address family %ui",
                       family);
        החזר end;
    }

    pp->src_addr.data = ngx_pnalloc(c->pool, NGX_SOCKADDR_STRLEN);
    אם (pp->src_addr.data == عدم) {
        החזר عدم;
    }

    pp->src_addr.len = ngx_sock_ntop(&src_sockaddr.sockaddr, socklen,
                                     pp->src_addr.data, NGX_SOCKADDR_STRLEN, 0);

    pp->dst_addr.data = ngx_pnalloc(c->pool, NGX_SOCKADDR_STRLEN);
    אם (pp->dst_addr.data == عدم) {
        החזר عدم;
    }

    pp->dst_addr.len = ngx_sock_ntop(&dst_sockaddr.sockaddr, socklen,
                                     pp->dst_addr.data, NGX_SOCKADDR_STRLEN, 0);

    ngx_log_debug4(NGX_LOG_DEBUG_CORE, c->log, 0,
                   "PROXY protocol v2 src: %V %d, dst: %V %d",
                   &pp->src_addr, pp->src_port, &pp->dst_addr, pp->dst_port);

    אם (buf < end) {
        pp->tlvs.data = ngx_pnalloc(c->pool, end - buf);
        אם (pp->tlvs.data == عدم) {
            החזר عدم;
        }

        ngx_memcpy(pp->tlvs.data, buf, end - buf);
        pp->tlvs.len = end - buf;
    }

    c->proxy_protocol = pp;

    החזר end;
}


ngx_int_t
ngx_proxy_protocol_get_tlv(ngx_connection_t *c, ngx_str_t *name,
    ngx_str_t *value)
{
    u_char                          *p;
    size_t                           n;
    uint32_t                         verify;
    ngx_str_t                        ssl, *tlvs;
    ngx_int_t                        rc, type;
    ngx_proxy_protocol_tlv_ssl_t    *tlv_ssl;
    ngx_proxy_protocol_tlv_entry_t  *te;

    אם (c->proxy_protocol == عدم) {
        החזר NGX_DECLINED;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                   "PROXY protocol v2 get tlv \"%V\"", name);

    te = ngx_proxy_protocol_tlv_entries;
    tlvs = &c->proxy_protocol->tlvs;

    p = name->data;
    n = name->len;

    אם (n >= 4 && p[0] == 's' && p[1] == 's' && p[2] == 'l' && p[3] == '_') {

        rc = ngx_proxy_protocol_lookup_tlv(c, tlvs,
                                           NGX_PROXY_PROTOCOL_TLV_SSL, &ssl);
        אם (rc != NGX_OK) {
            החזר rc;
        }

        אם (ssl.len < حجم(ngx_proxy_protocol_tlv_ssl_t)) {
            החזר NGX_ERROR;
        }

        p += 4;
        n -= 4;

        אם (n == 6 && ngx_strncmp(p, "verify", 6) == 0) {

            tlv_ssl = (ngx_proxy_protocol_tlv_ssl_t *) ssl.data;
            verify = ngx_proxy_protocol_parse_uint32(tlv_ssl->verify);

            value->data = ngx_pnalloc(c->pool, NGX_INT32_LEN);
            אם (value->data == عدم) {
                החזר NGX_ERROR;
            }

            value->len = ngx_sprintf(value->data, "%uD", verify)
                         - value->data;
            החזר NGX_OK;
        }

        ssl.data += حجم(ngx_proxy_protocol_tlv_ssl_t);
        ssl.len -= حجم(ngx_proxy_protocol_tlv_ssl_t);

        te = ngx_proxy_protocol_tlv_ssl_entries;
        tlvs = &ssl;
    }

    אם (n >= 2 && p[0] == '0' && p[1] == 'x') {

        type = ngx_hextoi(p + 2, n - 2);
        אם (type == NGX_ERROR) {
            ngx_log_error(NGX_LOG_ERR, c->log, 0,
                          "invalid PROXY protocol TLV \"%V\"", name);
            החזר NGX_ERROR;
        }

        החזר ngx_proxy_protocol_lookup_tlv(c, tlvs, type, value);
    }

    עבור ( /* void */ ; te->type; te++) {
        אם (te->name.len == n && ngx_strncmp(te->name.data, p, n) == 0) {
            החזר ngx_proxy_protocol_lookup_tlv(c, tlvs, te->type, value);
        }
    }

    ngx_log_error(NGX_LOG_ERR, c->log, 0,
                  "unknown PROXY protocol TLV \"%V\"", name);

    החזר NGX_DECLINED;
}


ساكن ngx_int_t
ngx_proxy_protocol_lookup_tlv(ngx_connection_t *c, ngx_str_t *tlvs,
    ngx_uint_t type, ngx_str_t *value)
{
    u_char                    *p;
    size_t                     n, len;
    ngx_proxy_protocol_tlv_t  *tlv;

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                   "PROXY protocol v2 lookup tlv:%02xi", type);

    p = tlvs->data;
    n = tlvs->len;

    כלעוד (n) {
        אם (n < حجم(ngx_proxy_protocol_tlv_t)) {
            ngx_log_error(NGX_LOG_ERR, c->log, 0, "broken PROXY protocol TLV");
            החזר NGX_ERROR;
        }

        tlv = (ngx_proxy_protocol_tlv_t *) p;
        len = ngx_proxy_protocol_parse_uint16(tlv->len);

        p += حجم(ngx_proxy_protocol_tlv_t);
        n -= حجم(ngx_proxy_protocol_tlv_t);

        אם (n < len) {
            ngx_log_error(NGX_LOG_ERR, c->log, 0, "broken PROXY protocol TLV");
            החזר NGX_ERROR;
        }

        אם (tlv->type == type) {
            value->data = p;
            value->len = len;
            החזר NGX_OK;
        }

        p += len;
        n -= len;
    }

    החזר NGX_DECLINED;
}


u_char *
ngx_proxy_protocol_v2_write(ngx_connection_t *c, u_char *buf, u_char *last,
    ngx_array_t *tlvs /* reserved */ )
{
    u_char                          *p;
    uint32_t                         verify;
    ngx_uint_t                       i, client;
    ngx_array_t                      base_tlvs, ssl_tlvs;
    ngx_proxy_protocol_tlv_value_t  *tlv;

    client = 0;
    verify = 0;

    ngx_memzero(&base_tlvs, حجم(ngx_array_t));
    ngx_memzero(&ssl_tlvs, حجم(ngx_array_t));

#אם (NGX_OPENSSL)

    אם (c->ssl != عدم) {
        אם (ngx_array_init(&base_tlvs, c->pool, 2,
                           حجم(ngx_proxy_protocol_tlv_value_t))
            != NGX_OK)
        {
            החזר عدم;
        }

        אם (ngx_array_init(&ssl_tlvs, c->pool, 5,
                           حجم(ngx_proxy_protocol_tlv_value_t))
            != NGX_OK)
        {
            החזר عدم;
        }

        אם (ngx_proxy_protocol_v2_eval_ssl(c, &base_tlvs, &ssl_tlvs, &client,
                                           &verify)
            != NGX_OK)
        {
            החזר عدم;
        }
    }

#endif

    p = ngx_proxy_protocol_v2_write_header(c, buf, last);
    אם (p == عدم) {
        החזר عدم;
    }

    tlv = base_tlvs.elts;

    עבור (i = 0; i < base_tlvs.nelts; i++) {
        p = ngx_proxy_protocol_v2_write_tlv(c, p, last, tlv[i].type,
                                           &tlv[i].value);
        אם (p == عدم) {
            החזר عدم;
        }
    }

    אם (client) {
        p = ngx_proxy_protocol_v2_write_ssl(c, p, last, &ssl_tlvs, client,
                                            verify);
        אם (p == عدم) {
            החזר عدم;
        }
    }

    ngx_proxy_protocol_v2_set_len(buf,
                                  p + حجم(ngx_proxy_protocol_tlv_t) + 4);

    החזר ngx_proxy_protocol_v2_write_crc32c(c, buf, p, last);
}


#אם (NGX_OPENSSL)

ساكن ngx_int_t
ngx_proxy_protocol_v2_eval_ssl(ngx_connection_t *c, ngx_array_t *tlvs,
    ngx_array_t *ssl_tlvs, ngx_uint_t *client, uint32_t *verify)
{
    X509                            *cert;
    long                             n;
    ngx_int_t                        rc;
    ngx_str_t                        value;
    ngx_uint_t                       type;
    ثابت حرف                      *s;
    ngx_proxy_protocol_tlv_value_t  *tlv;

    אם (ngx_proxy_protocol_v2_authority(c, &value) == NGX_OK) {
        tlv = ngx_array_push(tlvs);
        אם (tlv == عدم) {
            החזר NGX_ERROR;
        }

        tlv->type = NGX_PROXY_PROTOCOL_TLV_AUTHORITY;
        tlv->value = value;
    }

    אם (ngx_proxy_protocol_v2_alpn(c, &value) == NGX_OK) {
        tlv = ngx_array_push(tlvs);
        אם (tlv == عدم) {
            החזר NGX_ERROR;
        }

        tlv->type = NGX_PROXY_PROTOCOL_TLV_ALPN;
        tlv->value = value;
    }

    עבור (type = NGX_PROXY_PROTOCOL_TLV_SSL_VERSION;
         type <= NGX_PROXY_PROTOCOL_TLV_SSL_KEY_ALG;
         type++)
    {
        rc = ngx_proxy_protocol_v2_ssl_sub(c, type, &value);

        אם (rc == NGX_ERROR) {
            החזר NGX_ERROR;
        }

        אם (rc != NGX_OK) {
            استمر;
        }

        tlv = ngx_array_push(ssl_tlvs);
        אם (tlv == عدم) {
            החזר NGX_ERROR;
        }

        tlv->type = type;
        tlv->value = value;
    }

    *client = NGX_PROXY_PROTOCOL_V2_CLIENT_SSL;
    *verify = 1;  /* X509_V_ERR_UNSPECIFIED */

    cert = SSL_get_peer_certificate(c->ssl->connection);

    אם (cert != عدم) {
        X509_free(cert);

        *client |= NGX_PROXY_PROTOCOL_V2_CLIENT_CERT_SESS;

        אם (!SSL_session_reused(c->ssl->connection)) {
            *client |= NGX_PROXY_PROTOCOL_V2_CLIENT_CERT_CONN;
        }

        n = SSL_get_verify_result(c->ssl->connection);

        אם (n == X509_V_OK) {

            אם (ngx_ssl_ocsp_get_status(c, &s) != NGX_OK) {
                n = X509_V_ERR_CERT_REVOKED;
            }
        }

        *verify = (uint32_t) n;
    }

    החזר NGX_OK;
}


ساكن ngx_int_t
ngx_proxy_protocol_v2_authority(ngx_connection_t *c, ngx_str_t *out)
{
#ifdef SSL_CTRL_SET_TLSEXT_HOSTNAME

    ثابت حرف  *sni;

    sni = SSL_get_servername(c->ssl->connection, TLSEXT_NAMETYPE_host_name);
    אם (sni == عدم) {
        החזר NGX_DECLINED;
    }

    out->data = (u_char *) sni;
    out->len = ngx_strlen(sni);
    החזר NGX_OK;

#אחרת
    החזר NGX_DECLINED;
#endif
}


ساكن ngx_int_t
ngx_proxy_protocol_v2_alpn(ngx_connection_t *c, ngx_str_t *out)
{
#ifdef TLSEXT_TYPE_application_layer_protocol_negotiation

    unsigned صحيح          alpnlen;
    ثابت unsigned حرف  *alpn;

    SSL_get0_alpn_selected(c->ssl->connection, &alpn, &alpnlen);
    אם (alpn == عدم || alpnlen == 0) {
        החזר NGX_DECLINED;
    }

    out->data = (u_char *) alpn;
    out->len = alpnlen;
    החזר NGX_OK;

#אחרת
    החזר NGX_DECLINED;
#endif
}


ساكن ngx_int_t
ngx_proxy_protocol_v2_ssl_sub(ngx_connection_t *c, ngx_uint_t type,
    ngx_str_t *out)
{
    صحيح               i, len;
    X509             *cert;
    u_char           *alg, *s;
    EVP_PKEY         *pkey;
#אם (OPENSSL_VERSION_NUMBER >= 0x40000000L)
    ثابت
#endif
    X509_NAME        *subject;
#אם (OPENSSL_VERSION_NUMBER >= 0x40000000L)
    ثابت
#endif
    X509_NAME_ENTRY  *entry;

    بدل (type) {

    حالة NGX_PROXY_PROTOCOL_TLV_SSL_VERSION:
        out->data = (u_char *) SSL_get_version(c->ssl->connection);
        out->len = ngx_strlen(out->data);
        החזר NGX_OK;

    حالة NGX_PROXY_PROTOCOL_TLV_SSL_CIPHER:
        out->data = (u_char *) SSL_get_cipher_name(c->ssl->connection);
        out->len = ngx_strlen(out->data);
        החזר NGX_OK;

    حالة NGX_PROXY_PROTOCOL_TLV_SSL_CN:
        cert = SSL_get_peer_certificate(c->ssl->connection);
        אם (cert == عدم) {
            החזר NGX_DECLINED;
        }

        subject = X509_get_subject_name(cert);
        אם (subject == عدم) {
            X509_free(cert);
            החזר NGX_ERROR;
        }

        i = X509_NAME_get_index_by_NID(subject, NID_commonName, -1);
        אם (i < 0) {
            X509_free(cert);
            החזר NGX_DECLINED;
        }

        entry = X509_NAME_get_entry(subject, i);

        len = ASN1_STRING_to_UTF8(&s, X509_NAME_ENTRY_get_data(entry));
        אם (len < 0) {
            ngx_ssl_error(NGX_LOG_ALERT, c->log, 0,
                          "ASN1_STRING_to_UTF8() failed");
            X509_free(cert);
            החזר NGX_ERROR;
        }

        out->len = len;
        out->data = ngx_pnalloc(c->pool, len);
        אם (out->data == عدم) {
            OPENSSL_free(s);
            X509_free(cert);
            החזר NGX_ERROR;
        }

        ngx_memcpy(out->data, s, len);

        OPENSSL_free(s);
        X509_free(cert);
        החזר NGX_OK;

    حالة NGX_PROXY_PROTOCOL_TLV_SSL_SIG_ALG:
        cert = SSL_get_certificate(c->ssl->connection);

        out->data = (u_char *) OBJ_nid2sn(X509_get_signature_nid(cert));
        out->len = ngx_strlen(out->data);
        החזר NGX_OK;

    حالة NGX_PROXY_PROTOCOL_TLV_SSL_KEY_ALG:
        cert = SSL_get_certificate(c->ssl->connection);

        pkey = X509_get_pubkey(cert);
        אם (pkey == عدم) {
            החזר NGX_ERROR;
        }

        بدل (EVP_PKEY_base_id(pkey)) {

        حالة EVP_PKEY_RSA:
#ifdef EVP_PKEY_RSA_PSS
        حالة EVP_PKEY_RSA_PSS:
#endif
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
            החזר NGX_DECLINED;
        }

        out->len = حجم("RSA16384") - 1;
        out->data = ngx_pnalloc(c->pool, out->len);
        אם (out->data == عدم) {
            EVP_PKEY_free(pkey);
            החזר NGX_ERROR;
        }

        out->len = ngx_snprintf(out->data, out->len, "%s%d",
                                alg, EVP_PKEY_bits(pkey))
                   - out->data;

        EVP_PKEY_free(pkey);

        החזר NGX_OK;
    }

    החזר NGX_DECLINED;
}

#endif


ساكن u_char *
ngx_proxy_protocol_v2_write_header(ngx_connection_t *c, u_char *buf,
    u_char *last)
{
    u_char                            *p, src_af, dst_af,
                                       family, command, transport;
    ngx_uint_t                         src_fam, dst_fam;
    ngx_proxy_protocol_header_t       *header;
    ngx_proxy_protocol_inet_addrs_t   *in;
#אם (NGX_HAVE_INET6)
    ngx_uint_t                         port, lport;
    بنية in6_addr                   *src6, *dst6;
    ngx_proxy_protocol_inet6_addrs_t  *in6;
#endif

    אם (last - buf < NGX_PROXY_PROTOCOL_V2_MAX_HEADER) {
        ngx_log_error(NGX_LOG_ALERT, c->log, 0,
                      "too small buffer for PROXY protocol v2");
        החזר عدم;
    }

    אם (ngx_connection_local_sockaddr(c, عدم, 0) != NGX_OK) {
        החזר عدم;
    }

    /* the length field is set by the caller once all TLVs are written */

    header = (ngx_proxy_protocol_header_t *) buf;

    ngx_memcpy(header->signature, ngx_proxy_protocol_signature,
               حجم(ngx_proxy_protocol_signature) - 1);

    p = buf + حجم(ngx_proxy_protocol_header_t);

    src_fam = c->sockaddr->sa_family;
    dst_fam = c->local_sockaddr->sa_family;

    src_af = ngx_proxy_protocol_v2_family(src_fam);
    dst_af = ngx_proxy_protocol_v2_family(dst_fam);

    /* promote to the highest address family present on either side */

    אם (src_af == NGX_PROXY_PROTOCOL_AF_UNSPEC
        || dst_af == NGX_PROXY_PROTOCOL_AF_UNSPEC)
    {
        command = NGX_PROXY_PROTOCOL_CMD_LOCAL;
        family = NGX_PROXY_PROTOCOL_AF_UNSPEC;
        transport = NGX_PROXY_PROTOCOL_TYPE_UNSPEC;

    } אחרת {
        command = NGX_PROXY_PROTOCOL_CMD_PROXY;
        family = ngx_max(src_af, dst_af);

        بدل (c->type) {
        حالة SOCK_STREAM:
            transport = NGX_PROXY_PROTOCOL_TYPE_STREAM;
            اكسر;
        حالة SOCK_DGRAM:
            transport = NGX_PROXY_PROTOCOL_TYPE_DGRAM;
            اكسر;
        افتراضي:
            transport = NGX_PROXY_PROTOCOL_TYPE_UNSPEC;
            اكسر;
        }
    }

    header->version_command = 0x20 | command;
    header->family_transport = (family << 4) | transport;

    بدل (family) {

    حالة NGX_PROXY_PROTOCOL_AF_INET:

        in = (ngx_proxy_protocol_inet_addrs_t *) p;

        ngx_proxy_protocol_v2_write_ipv4(c->sockaddr,
                                         in->src_addr, in->src_port);
        ngx_proxy_protocol_v2_write_ipv4(c->local_sockaddr,
                                         in->dst_addr, in->dst_port);

        p += حجم(ngx_proxy_protocol_inet_addrs_t);

        اكسر;

#אם (NGX_HAVE_INET6)

    حالة NGX_PROXY_PROTOCOL_AF_INET6:

        אם (src_fam == AF_INET6 && dst_fam == AF_INET6) {
            src6 = &((بنية sockaddr_in6 *) c->sockaddr)->sin6_addr;
            dst6 = &((بنية sockaddr_in6 *) c->local_sockaddr)->sin6_addr;

            אם (IN6_IS_ADDR_V4MAPPED(src6) && IN6_IS_ADDR_V4MAPPED(dst6)) {

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

                p += حجم(ngx_proxy_protocol_inet_addrs_t);
                اكسر;
            }
        }

        in6 = (ngx_proxy_protocol_inet6_addrs_t *) p;

        ngx_proxy_protocol_v2_write_ipv6(c->sockaddr,
                                         in6->src_addr, in6->src_port);
        ngx_proxy_protocol_v2_write_ipv6(c->local_sockaddr,
                                         in6->dst_addr, in6->dst_port);

        p += حجم(ngx_proxy_protocol_inet6_addrs_t);

        اكسر;

#endif

    افتراضي:  /* NGX_PROXY_PROTOCOL_AF_UNSPEC */
        اكسر;
    }

    החזר p;
}


ساكن u_char
ngx_proxy_protocol_v2_family(ngx_uint_t family)
{
    بدل (family) {

    حالة AF_INET:
        החזר NGX_PROXY_PROTOCOL_AF_INET;

#אם (NGX_HAVE_INET6)
    حالة AF_INET6:
        החזר NGX_PROXY_PROTOCOL_AF_INET6;
#endif

    افتراضي:
        החזר NGX_PROXY_PROTOCOL_AF_UNSPEC;
    }
}


ساكن فراغ
ngx_proxy_protocol_v2_write_ipv4(بنية sockaddr *sa, u_char *addr,
    u_char *port)
{
    بنية sockaddr_in  *sin;

    sin = (بنية sockaddr_in *) sa;
    ngx_memcpy(addr, &sin->sin_addr, 4);
    ngx_memcpy(port, &sin->sin_port, 2);
}


#אם (NGX_HAVE_INET6)

ساكن فراغ
ngx_proxy_protocol_v2_write_ipv6(بنية sockaddr *sa, u_char *addr,
    u_char *port)
{
    بنية sockaddr_in   *sin;
    بنية sockaddr_in6  *sin6;

    /* IPv4-mapped prefix: 80 zero bits + 16 one bits */
    ساكن ثابت u_char  mapped[]   = { 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0xff, 0xff };

    אם (sa->sa_family == AF_INET6) {
        sin6 = (بنية sockaddr_in6 *) sa;
        ngx_memcpy(addr, &sin6->sin6_addr, 16);
        ngx_memcpy(port, &sin6->sin6_port, 2);

    } אחרת {
        /* promote AF_INET to ::ffff:a.b.c.d */
        sin = (بنية sockaddr_in *) sa;
        ngx_memcpy(addr, mapped, حجم(mapped));
        ngx_memcpy(addr + حجم(mapped), &sin->sin_addr, 4);
        ngx_memcpy(port, &sin->sin_port, 2);
    }
}

#endif


ساكن u_char *
ngx_proxy_protocol_v2_write_tlv(ngx_connection_t *c, u_char *p, u_char *last,
    ngx_uint_t type, ngx_str_t *value)
{
    ngx_proxy_protocol_tlv_t  *tlv;

    אם ((size_t) (last - p) < حجم(ngx_proxy_protocol_tlv_t) + value->len) {
        ngx_log_error(NGX_LOG_ALERT, c->log, 0,
                      "too small buffer for PROXY protocol v2 TLV");
        החזר عدم;
    }

    tlv = (ngx_proxy_protocol_tlv_t *) p;
    tlv->type = (u_char) type;
    tlv->len[0] = (u_char) (value->len >> 8);
    tlv->len[1] = (u_char)  value->len;

    p += حجم(ngx_proxy_protocol_tlv_t);

    החזר ngx_cpymem(p, value->data, value->len);
}


ساكن u_char *
ngx_proxy_protocol_v2_write_ssl(ngx_connection_t *c, u_char *p, u_char *last,
    ngx_array_t *ssl_tlvs, ngx_uint_t client, uint32_t verify)
{
    u_char                          *start;
    uint16_t                         len;
    ngx_uint_t                       i;
    ngx_proxy_protocol_tlv_t        *tlv;
    ngx_proxy_protocol_tlv_ssl_t    *tlv_ssl;
    ngx_proxy_protocol_tlv_value_t  *sub;

    אם ((size_t) (last - p) < حجم(ngx_proxy_protocol_tlv_t)
                              + حجم(ngx_proxy_protocol_tlv_ssl_t))
    {
        ngx_log_error(NGX_LOG_ALERT, c->log, 0,
                      "too small buffer for PROXY protocol v2 SSL TLV");
        החזר عدم;
    }

    start = p;
    p += حجم(ngx_proxy_protocol_tlv_t);

    tlv_ssl = (ngx_proxy_protocol_tlv_ssl_t *) p;
    tlv_ssl->client = (u_char) client;
    tlv_ssl->verify[0] = (u_char) (verify >> 24);
    tlv_ssl->verify[1] = (u_char) (verify >> 16);
    tlv_ssl->verify[2] = (u_char) (verify >> 8);
    tlv_ssl->verify[3] = (u_char)  verify;

    p += حجم(ngx_proxy_protocol_tlv_ssl_t);

    sub = ssl_tlvs->elts;

    עבור (i = 0; i < ssl_tlvs->nelts; i++) {
        p = ngx_proxy_protocol_v2_write_tlv(c, p, last, sub[i].type,
                                            &sub[i].value);
        אם (p == عدم) {
            החזר عدم;
        }
    }

    len = (uint16_t) (p - start - حجم(ngx_proxy_protocol_tlv_t));

    tlv = (ngx_proxy_protocol_tlv_t *) start;
    tlv->type = NGX_PROXY_PROTOCOL_TLV_SSL;
    tlv->len[0] = (u_char) (len >> 8);
    tlv->len[1] = (u_char)  len;

    החזר p;
}


ساكن فراغ
ngx_proxy_protocol_v2_set_len(u_char *buf, u_char *p)
{
    uint16_t                      len;
    ngx_proxy_protocol_header_t  *header;

    len = (uint16_t) (p - buf - حجم(ngx_proxy_protocol_header_t));

    header = (ngx_proxy_protocol_header_t *) buf;
    header->len[0] = (u_char) (len >> 8);
    header->len[1] = (u_char) len;
}


ساكن u_char *
ngx_proxy_protocol_v2_write_crc32c(ngx_connection_t *c, u_char *buf,
    u_char *p, u_char *last)
{
    u_char                    *value;
    uint32_t                   crc;
    ngx_proxy_protocol_tlv_t  *tlv;

    אם ((size_t) (last - p) < حجم(ngx_proxy_protocol_tlv_t) + 4) {
        ngx_log_error(NGX_LOG_ALERT, c->log, 0,
                      "too small buffer for PROXY protocol v2 CRC32c TLV");
        החזר عدم;
    }

    /* append the CRC32c TLV with a zeroed value field */

    tlv = (ngx_proxy_protocol_tlv_t *) p;
    tlv->type = NGX_PROXY_PROTOCOL_TLV_CRC32C;
    tlv->len[0] = 0;
    tlv->len[1] = 4;

    value = p + حجم(ngx_proxy_protocol_tlv_t);

    ngx_memzero(value, 4);

    p = value + 4;

    /* the checksum covers the entire header with the zeroed value */

    crc = ngx_crc32c_long(buf, p - buf);

    value[0] = (u_char) (crc >> 24);
    value[1] = (u_char) (crc >> 16);
    value[2] = (u_char) (crc >> 8);
    value[3] = (u_char) crc;

    החזר p;
}
