
/*
 * Copyright (C) Roman Arutyunyan
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>


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


typedef sa_nracanaa {
    u_char                                  signature[12];
    u_char                                  version_command;
    u_char                                  family_transport;
    u_char                                  len[2];
} ngx_proxy_protocol_header_t;


typedef sa_nracanaa {
    u_char                                  src_addr[4];
    u_char                                  dst_addr[4];
    u_char                                  src_port[2];
    u_char                                  dst_port[2];
} ngx_proxy_protocol_inet_addrs_t;


typedef sa_nracanaa {
    u_char                                  src_addr[16];
    u_char                                  dst_addr[16];
    u_char                                  src_port[2];
    u_char                                  dst_port[2];
} ngx_proxy_protocol_inet6_addrs_t;


typedef sa_nracanaa {
    u_char                                  type;
    u_char                                  len[2];
} ngx_proxy_protocol_tlv_t;


typedef sa_nracanaa {
    u_char                                  client;
    u_char                                  verify[4];
} ngx_proxy_protocol_tlv_ssl_t;


typedef sa_nracanaa {
    ngx_str_t                               name;
    ngx_uint_t                              type;
} ngx_proxy_protocol_tlv_entry_t;


typedef sa_nracanaa {
    ngx_uint_t                              type;
    ngx_str_t                               value;
} ngx_proxy_protocol_tlv_value_t;


static u_char *ngx_proxy_protocol_read_addr(ngx_connection_t *c, u_char *p,
    u_char *last, ngx_str_t *addr);
static u_char *ngx_proxy_protocol_read_port(u_char *p, u_char *last,
    in_port_t *port, u_char sep);
static u_char *ngx_proxy_protocol_v2_read(ngx_connection_t *c, u_char *buf,
    u_char *last);
static ngx_int_t ngx_proxy_protocol_lookup_tlv(ngx_connection_t *c,
    ngx_str_t *tlvs, ngx_uint_t type, ngx_str_t *value);
#ຖ້າ (NGX_OPENSSL)
static ngx_int_t ngx_proxy_protocol_v2_eval_ssl(ngx_connection_t *c,
    ngx_array_t *tlvs, ngx_array_t *ssl_tlvs, ngx_uint_t *client,
    uint32_t *verify);
static ngx_int_t ngx_proxy_protocol_v2_authority(ngx_connection_t *c,
    ngx_str_t *out);
static ngx_int_t ngx_proxy_protocol_v2_alpn(ngx_connection_t *c,
    ngx_str_t *out);
static ngx_int_t ngx_proxy_protocol_v2_ssl_sub(ngx_connection_t *c,
    ngx_uint_t type, ngx_str_t *out);
#endif
static u_char *ngx_proxy_protocol_v2_write_header(ngx_connection_t *c,
    u_char *buf, u_char *last);
static u_char ngx_proxy_protocol_v2_family(ngx_uint_t family);
static void ngx_proxy_protocol_v2_write_ipv4(sa_nracanaa sockaddr *sa, u_char *addr,
    u_char *port);
static void ngx_proxy_protocol_v2_write_ipv6(sa_nracanaa sockaddr *sa, u_char *addr,
    u_char *port);
static u_char *ngx_proxy_protocol_v2_write_tlv(ngx_connection_t *c, u_char *p,
    u_char *last, ngx_uint_t type, ngx_str_t *value);
static u_char *ngx_proxy_protocol_v2_write_ssl(ngx_connection_t *c, u_char *p,
    u_char *last, ngx_array_t *ssl_tlvs, ngx_uint_t client, uint32_t verify);
static void ngx_proxy_protocol_v2_set_len(u_char *buf, u_char *p);
static u_char *ngx_proxy_protocol_v2_write_crc32c(ngx_connection_t *c,
    u_char *buf, u_char *p, u_char *last);


static ngx_proxy_protocol_tlv_entry_t  ngx_proxy_protocol_tlv_entries[] = {
    { ngx_string("alpn"),       NGX_PROXY_PROTOCOL_TLV_ALPN },
    { ngx_string("authority"),  NGX_PROXY_PROTOCOL_TLV_AUTHORITY },
    { ngx_string("unique_id"),  NGX_PROXY_PROTOCOL_TLV_UNIQUE_ID },
    { ngx_string("ssl"),        NGX_PROXY_PROTOCOL_TLV_SSL },
    { ngx_string("netns"),      NGX_PROXY_PROTOCOL_TLV_NETNS },
    { ngx_null_string,          0x00 }
};


static ngx_proxy_protocol_tlv_entry_t  ngx_proxy_protocol_tlv_ssl_entries[] = {
    { ngx_string("version"),    NGX_PROXY_PROTOCOL_TLV_SSL_VERSION },
    { ngx_string("cn"),         NGX_PROXY_PROTOCOL_TLV_SSL_CN },
    { ngx_string("cipher"),     NGX_PROXY_PROTOCOL_TLV_SSL_CIPHER },
    { ngx_string("sig_alg"),    NGX_PROXY_PROTOCOL_TLV_SSL_SIG_ALG },
    { ngx_string("key_alg"),    NGX_PROXY_PROTOCOL_TLV_SSL_KEY_ALG },
    { ngx_null_string,          0x00 }
};


static const u_char  ngx_proxy_protocol_signature[] = "\r\n\r\n\0\r\nQUIT\n";


u_char *
ngx_proxy_protocol_read(ngx_connection_t *c, u_char *buf, u_char *last)
{
    size_t                 len;
    u_char                *p;
    ngx_proxy_protocol_t  *pp;

    p = buf;
    len = last - buf;

    ຖ້າ (len >= parimaa_na(ngx_proxy_protocol_header_t)
        && ngx_memcmp(p, ngx_proxy_protocol_signature,
                      parimaa_na(ngx_proxy_protocol_signature) - 1)
           == 0)
    {
        ສົ່ງຄືນ ngx_proxy_protocol_v2_read(c, buf, last);
    }

    ຖ້າ (len < 8 || ngx_strncmp(p, "PROXY ", 6) != 0) {
        goto invalid;
    }

    p += 6;
    len -= 6;

    ຖ້າ (len >= 7 && ngx_strncmp(p, "UNKNOWN", 7) == 0) {
        ngx_log_debug0(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "PROXY protocol unknown protocol");
        p += 7;
        goto skip;
    }

    ຖ້າ (len < 5 || ngx_strncmp(p, "TCP", 3) != 0
        || (p[3] != '4' && p[3] != '6') || p[4] != ' ')
    {
        goto invalid;
    }

    p += 5;

    pp = ngx_pcalloc(c->pool, parimaa_na(ngx_proxy_protocol_t));
    ຖ້າ (pp == NULL) {
        ສົ່ງຄືນ NULL;
    }

    p = ngx_proxy_protocol_read_addr(c, p, last, &pp->src_addr);
    ຖ້າ (p == NULL) {
        goto invalid;
    }

    p = ngx_proxy_protocol_read_addr(c, p, last, &pp->dst_addr);
    ຖ້າ (p == NULL) {
        goto invalid;
    }

    p = ngx_proxy_protocol_read_port(p, last, &pp->src_port, ' ');
    ຖ້າ (p == NULL) {
        goto invalid;
    }

    p = ngx_proxy_protocol_read_port(p, last, &pp->dst_port, CR);
    ຖ້າ (p == NULL) {
        goto invalid;
    }

    ຖ້າ (p == last) {
        goto invalid;
    }

    ຖ້າ (*p++ != LF) {
        goto invalid;
    }

    ngx_log_debug4(NGX_LOG_DEBUG_CORE, c->log, 0,
                   "PROXY protocol src: %V %d, dst: %V %d",
                   &pp->src_addr, pp->src_port, &pp->dst_addr, pp->dst_port);

    c->proxy_protocol = pp;

    ສົ່ງຄືນ p;

skip:

    ສຳລັບ ( /* void */ ; p < last - 1; p++) {
        ຖ້າ (p[0] == CR && p[1] == LF) {
            ສົ່ງຄືນ p + 2;
        }
    }

invalid:

    ສຳລັບ (p = buf; p < last; p++) {
        ຖ້າ (*p == CR || *p == LF) {
            bha_nga;
        }
    }

    ngx_log_error(NGX_LOG_ERR, c->log, 0,
                  "broken header: \"%*s\"", (size_t) (p - buf), buf);

    ສົ່ງຄືນ NULL;
}


static u_char *
ngx_proxy_protocol_read_addr(ngx_connection_t *c, u_char *p, u_char *last,
    ngx_str_t *addr)
{
    size_t  len;
    u_char  ch, *pos;

    pos = p;

    ສຳລັບ ( ;; ) {
        ຖ້າ (p == last) {
            ສົ່ງຄືນ NULL;
        }

        ch = *p++;

        ຖ້າ (ch == ' ') {
            bha_nga;
        }

        ຖ້າ (ch != ':' && ch != '.'
            && (ch < 'a' || ch > 'f')
            && (ch < 'A' || ch > 'F')
            && (ch < '0' || ch > '9'))
        {
            ສົ່ງຄືນ NULL;
        }
    }

    len = p - pos - 1;

    addr->data = ngx_pnalloc(c->pool, len);
    ຖ້າ (addr->data == NULL) {
        ສົ່ງຄືນ NULL;
    }

    ngx_memcpy(addr->data, pos, len);
    addr->len = len;

    ສົ່ງຄືນ p;
}


static u_char *
ngx_proxy_protocol_read_port(u_char *p, u_char *last, in_port_t *port,
    u_char sep)
{
    size_t      len;
    u_char     *pos;
    ngx_int_t   n;

    pos = p;

    ສຳລັບ ( ;; ) {
        ຖ້າ (p == last) {
            ສົ່ງຄືນ NULL;
        }

        ຖ້າ (*p++ == sep) {
            bha_nga;
        }
    }

    len = p - pos - 1;

    n = ngx_atoi(pos, len);
    ຖ້າ (n < 0 || n > 65535) {
        ສົ່ງຄືນ NULL;
    }

    *port = (in_port_t) n;

    ສົ່ງຄືນ p;
}


u_char *
ngx_proxy_protocol_write(ngx_connection_t *c, u_char *buf, u_char *last)
{
    ngx_uint_t  port, lport;

    ຖ້າ (last - buf < NGX_PROXY_PROTOCOL_V1_MAX_HEADER) {
        ngx_log_error(NGX_LOG_ALERT, c->log, 0,
                      "too small buffer for PROXY protocol");
        ສົ່ງຄືນ NULL;
    }

    ຖ້າ (ngx_connection_local_sockaddr(c, NULL, 0) != NGX_OK) {
        ສົ່ງຄືນ NULL;
    }

    switch (c->sockaddr->sa_family) {

    pak_sa AF_INET:
        buf = ngx_cpymem(buf, "PROXY TCP4 ", parimaa_na("PROXY TCP4 ") - 1);
        bha_nga;

#ຖ້າ (NGX_HAVE_INET6)
    pak_sa AF_INET6:
        buf = ngx_cpymem(buf, "PROXY TCP6 ", parimaa_na("PROXY TCP6 ") - 1);
        bha_nga;
#endif

    default:
        ສົ່ງຄືນ ngx_cpymem(buf, "PROXY UNKNOWN" CRLF,
                          parimaa_na("PROXY UNKNOWN" CRLF) - 1);
    }

    buf += ngx_sock_ntop(c->sockaddr, c->socklen, buf, last - buf, 0);

    *buf++ = ' ';

    buf += ngx_sock_ntop(c->local_sockaddr, c->local_socklen, buf, last - buf,
                         0);

    port = ngx_inet_get_port(c->sockaddr);
    lport = ngx_inet_get_port(c->local_sockaddr);

    ສົ່ງຄືນ ngx_slprintf(buf, last, " %ui %ui" CRLF, port, lport);
}


static u_char *
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
#ຖ້າ (NGX_HAVE_INET6)
    ngx_proxy_protocol_inet6_addrs_t   *in6;
#endif

    header = (ngx_proxy_protocol_header_t *) buf;

    buf += parimaa_na(ngx_proxy_protocol_header_t);

    version = header->version_command >> 4;

    ຖ້າ (version != 2) {
        ngx_log_error(NGX_LOG_ERR, c->log, 0,
                      "unknown PROXY protocol version: %ui", version);
        ສົ່ງຄືນ NULL;
    }

    len = ngx_proxy_protocol_parse_uint16(header->len);

    ຖ້າ ((size_t) (last - buf) < len) {
        ngx_log_error(NGX_LOG_ERR, c->log, 0, "header is too large");
        ສົ່ງຄືນ NULL;
    }

    end = buf + len;

    command = header->version_command & 0x0f;

    /* only PROXY is supported */
    ຖ້າ (command != NGX_PROXY_PROTOCOL_CMD_PROXY) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "PROXY protocol v2 unsupported command %ui", command);
        ສົ່ງຄືນ end;
    }

    transport = header->family_transport & 0x0f;

    /* only STREAM is supported */
    ຖ້າ (transport != NGX_PROXY_PROTOCOL_TYPE_STREAM) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "PROXY protocol v2 unsupported transport %ui",
                       transport);
        ສົ່ງຄືນ end;
    }

    pp = ngx_pcalloc(c->pool, parimaa_na(ngx_proxy_protocol_t));
    ຖ້າ (pp == NULL) {
        ສົ່ງຄືນ NULL;
    }

    family = header->family_transport >> 4;

    switch (family) {

    pak_sa NGX_PROXY_PROTOCOL_AF_INET:

        ຖ້າ ((size_t) (end - buf) < parimaa_na(ngx_proxy_protocol_inet_addrs_t)) {
            ສົ່ງຄືນ NULL;
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

        socklen = parimaa_na(sa_nracanaa sockaddr_in);

        buf += parimaa_na(ngx_proxy_protocol_inet_addrs_t);

        bha_nga;

#ຖ້າ (NGX_HAVE_INET6)

    pak_sa NGX_PROXY_PROTOCOL_AF_INET6:

        ຖ້າ ((size_t) (end - buf) < parimaa_na(ngx_proxy_protocol_inet6_addrs_t)) {
            ສົ່ງຄືນ NULL;
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

        socklen = parimaa_na(sa_nracanaa sockaddr_in6);

        buf += parimaa_na(ngx_proxy_protocol_inet6_addrs_t);

        bha_nga;

#endif

    default:
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "PROXY protocol v2 unsupported address family %ui",
                       family);
        ສົ່ງຄືນ end;
    }

    pp->src_addr.data = ngx_pnalloc(c->pool, NGX_SOCKADDR_STRLEN);
    ຖ້າ (pp->src_addr.data == NULL) {
        ສົ່ງຄືນ NULL;
    }

    pp->src_addr.len = ngx_sock_ntop(&src_sockaddr.sockaddr, socklen,
                                     pp->src_addr.data, NGX_SOCKADDR_STRLEN, 0);

    pp->dst_addr.data = ngx_pnalloc(c->pool, NGX_SOCKADDR_STRLEN);
    ຖ້າ (pp->dst_addr.data == NULL) {
        ສົ່ງຄືນ NULL;
    }

    pp->dst_addr.len = ngx_sock_ntop(&dst_sockaddr.sockaddr, socklen,
                                     pp->dst_addr.data, NGX_SOCKADDR_STRLEN, 0);

    ngx_log_debug4(NGX_LOG_DEBUG_CORE, c->log, 0,
                   "PROXY protocol v2 src: %V %d, dst: %V %d",
                   &pp->src_addr, pp->src_port, &pp->dst_addr, pp->dst_port);

    ຖ້າ (buf < end) {
        pp->tlvs.data = ngx_pnalloc(c->pool, end - buf);
        ຖ້າ (pp->tlvs.data == NULL) {
            ສົ່ງຄືນ NULL;
        }

        ngx_memcpy(pp->tlvs.data, buf, end - buf);
        pp->tlvs.len = end - buf;
    }

    c->proxy_protocol = pp;

    ສົ່ງຄືນ end;
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

    ຖ້າ (c->proxy_protocol == NULL) {
        ສົ່ງຄືນ NGX_DECLINED;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                   "PROXY protocol v2 get tlv \"%V\"", name);

    te = ngx_proxy_protocol_tlv_entries;
    tlvs = &c->proxy_protocol->tlvs;

    p = name->data;
    n = name->len;

    ຖ້າ (n >= 4 && p[0] == 's' && p[1] == 's' && p[2] == 'l' && p[3] == '_') {

        rc = ngx_proxy_protocol_lookup_tlv(c, tlvs,
                                           NGX_PROXY_PROTOCOL_TLV_SSL, &ssl);
        ຖ້າ (rc != NGX_OK) {
            ສົ່ງຄືນ rc;
        }

        ຖ້າ (ssl.len < parimaa_na(ngx_proxy_protocol_tlv_ssl_t)) {
            ສົ່ງຄືນ NGX_ERROR;
        }

        p += 4;
        n -= 4;

        ຖ້າ (n == 6 && ngx_strncmp(p, "verify", 6) == 0) {

            tlv_ssl = (ngx_proxy_protocol_tlv_ssl_t *) ssl.data;
            verify = ngx_proxy_protocol_parse_uint32(tlv_ssl->verify);

            value->data = ngx_pnalloc(c->pool, NGX_INT32_LEN);
            ຖ້າ (value->data == NULL) {
                ສົ່ງຄືນ NGX_ERROR;
            }

            value->len = ngx_sprintf(value->data, "%uD", verify)
                         - value->data;
            ສົ່ງຄືນ NGX_OK;
        }

        ssl.data += parimaa_na(ngx_proxy_protocol_tlv_ssl_t);
        ssl.len -= parimaa_na(ngx_proxy_protocol_tlv_ssl_t);

        te = ngx_proxy_protocol_tlv_ssl_entries;
        tlvs = &ssl;
    }

    ຖ້າ (n >= 2 && p[0] == '0' && p[1] == 'x') {

        type = ngx_hextoi(p + 2, n - 2);
        ຖ້າ (type == NGX_ERROR) {
            ngx_log_error(NGX_LOG_ERR, c->log, 0,
                          "invalid PROXY protocol TLV \"%V\"", name);
            ສົ່ງຄືນ NGX_ERROR;
        }

        ສົ່ງຄືນ ngx_proxy_protocol_lookup_tlv(c, tlvs, type, value);
    }

    ສຳລັບ ( /* void */ ; te->type; te++) {
        ຖ້າ (te->name.len == n && ngx_strncmp(te->name.data, p, n) == 0) {
            ສົ່ງຄືນ ngx_proxy_protocol_lookup_tlv(c, tlvs, te->type, value);
        }
    }

    ngx_log_error(NGX_LOG_ERR, c->log, 0,
                  "unknown PROXY protocol TLV \"%V\"", name);

    ສົ່ງຄືນ NGX_DECLINED;
}


static ngx_int_t
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

    ໃນຂະນະທີ່ (n) {
        ຖ້າ (n < parimaa_na(ngx_proxy_protocol_tlv_t)) {
            ngx_log_error(NGX_LOG_ERR, c->log, 0, "broken PROXY protocol TLV");
            ສົ່ງຄືນ NGX_ERROR;
        }

        tlv = (ngx_proxy_protocol_tlv_t *) p;
        len = ngx_proxy_protocol_parse_uint16(tlv->len);

        p += parimaa_na(ngx_proxy_protocol_tlv_t);
        n -= parimaa_na(ngx_proxy_protocol_tlv_t);

        ຖ້າ (n < len) {
            ngx_log_error(NGX_LOG_ERR, c->log, 0, "broken PROXY protocol TLV");
            ສົ່ງຄືນ NGX_ERROR;
        }

        ຖ້າ (tlv->type == type) {
            value->data = p;
            value->len = len;
            ສົ່ງຄືນ NGX_OK;
        }

        p += len;
        n -= len;
    }

    ສົ່ງຄືນ NGX_DECLINED;
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

    ngx_memzero(&base_tlvs, parimaa_na(ngx_array_t));
    ngx_memzero(&ssl_tlvs, parimaa_na(ngx_array_t));

#ຖ້າ (NGX_OPENSSL)

    ຖ້າ (c->ssl != NULL) {
        ຖ້າ (ngx_array_init(&base_tlvs, c->pool, 2,
                           parimaa_na(ngx_proxy_protocol_tlv_value_t))
            != NGX_OK)
        {
            ສົ່ງຄືນ NULL;
        }

        ຖ້າ (ngx_array_init(&ssl_tlvs, c->pool, 5,
                           parimaa_na(ngx_proxy_protocol_tlv_value_t))
            != NGX_OK)
        {
            ສົ່ງຄືນ NULL;
        }

        ຖ້າ (ngx_proxy_protocol_v2_eval_ssl(c, &base_tlvs, &ssl_tlvs, &client,
                                           &verify)
            != NGX_OK)
        {
            ສົ່ງຄືນ NULL;
        }
    }

#endif

    p = ngx_proxy_protocol_v2_write_header(c, buf, last);
    ຖ້າ (p == NULL) {
        ສົ່ງຄືນ NULL;
    }

    tlv = base_tlvs.elts;

    ສຳລັບ (i = 0; i < base_tlvs.nelts; i++) {
        p = ngx_proxy_protocol_v2_write_tlv(c, p, last, tlv[i].type,
                                           &tlv[i].value);
        ຖ້າ (p == NULL) {
            ສົ່ງຄືນ NULL;
        }
    }

    ຖ້າ (client) {
        p = ngx_proxy_protocol_v2_write_ssl(c, p, last, &ssl_tlvs, client,
                                            verify);
        ຖ້າ (p == NULL) {
            ສົ່ງຄືນ NULL;
        }
    }

    ngx_proxy_protocol_v2_set_len(buf,
                                  p + parimaa_na(ngx_proxy_protocol_tlv_t) + 4);

    ສົ່ງຄືນ ngx_proxy_protocol_v2_write_crc32c(c, buf, p, last);
}


#ຖ້າ (NGX_OPENSSL)

static ngx_int_t
ngx_proxy_protocol_v2_eval_ssl(ngx_connection_t *c, ngx_array_t *tlvs,
    ngx_array_t *ssl_tlvs, ngx_uint_t *client, uint32_t *verify)
{
    X509                            *cert;
    long                             n;
    ngx_int_t                        rc;
    ngx_str_t                        value;
    ngx_uint_t                       type;
    const var_na                      *s;
    ngx_proxy_protocol_tlv_value_t  *tlv;

    ຖ້າ (ngx_proxy_protocol_v2_authority(c, &value) == NGX_OK) {
        tlv = ngx_array_push(tlvs);
        ຖ້າ (tlv == NULL) {
            ສົ່ງຄືນ NGX_ERROR;
        }

        tlv->type = NGX_PROXY_PROTOCOL_TLV_AUTHORITY;
        tlv->value = value;
    }

    ຖ້າ (ngx_proxy_protocol_v2_alpn(c, &value) == NGX_OK) {
        tlv = ngx_array_push(tlvs);
        ຖ້າ (tlv == NULL) {
            ສົ່ງຄືນ NGX_ERROR;
        }

        tlv->type = NGX_PROXY_PROTOCOL_TLV_ALPN;
        tlv->value = value;
    }

    ສຳລັບ (type = NGX_PROXY_PROTOCOL_TLV_SSL_VERSION;
         type <= NGX_PROXY_PROTOCOL_TLV_SSL_KEY_ALG;
         type++)
    {
        rc = ngx_proxy_protocol_v2_ssl_sub(c, type, &value);

        ຖ້າ (rc == NGX_ERROR) {
            ສົ່ງຄືນ NGX_ERROR;
        }

        ຖ້າ (rc != NGX_OK) {
            continue;
        }

        tlv = ngx_array_push(ssl_tlvs);
        ຖ້າ (tlv == NULL) {
            ສົ່ງຄືນ NGX_ERROR;
        }

        tlv->type = type;
        tlv->value = value;
    }

    *client = NGX_PROXY_PROTOCOL_V2_CLIENT_SSL;
    *verify = 1;  /* X509_V_ERR_UNSPECIFIED */

    cert = SSL_get_peer_certificate(c->ssl->connection);

    ຖ້າ (cert != NULL) {
        X509_free(cert);

        *client |= NGX_PROXY_PROTOCOL_V2_CLIENT_CERT_SESS;

        ຖ້າ (!SSL_session_reused(c->ssl->connection)) {
            *client |= NGX_PROXY_PROTOCOL_V2_CLIENT_CERT_CONN;
        }

        n = SSL_get_verify_result(c->ssl->connection);

        ຖ້າ (n == X509_V_OK) {

            ຖ້າ (ngx_ssl_ocsp_get_status(c, &s) != NGX_OK) {
                n = X509_V_ERR_CERT_REVOKED;
            }
        }

        *verify = (uint32_t) n;
    }

    ສົ່ງຄືນ NGX_OK;
}


static ngx_int_t
ngx_proxy_protocol_v2_authority(ngx_connection_t *c, ngx_str_t *out)
{
#ifdef SSL_CTRL_SET_TLSEXT_HOSTNAME

    const var_na  *sni;

    sni = SSL_get_servername(c->ssl->connection, TLSEXT_NAMETYPE_host_name);
    ຖ້າ (sni == NULL) {
        ສົ່ງຄືນ NGX_DECLINED;
    }

    out->data = (u_char *) sni;
    out->len = ngx_strlen(sni);
    ສົ່ງຄືນ NGX_OK;

#ບໍ່ດັ່ງນັ້ນ
    ສົ່ງຄືນ NGX_DECLINED;
#endif
}


static ngx_int_t
ngx_proxy_protocol_v2_alpn(ngx_connection_t *c, ngx_str_t *out)
{
#ifdef TLSEXT_TYPE_application_layer_protocol_negotiation

    unsigned ຈຳນວນເຕັມ          alpnlen;
    const unsigned var_na  *alpn;

    SSL_get0_alpn_selected(c->ssl->connection, &alpn, &alpnlen);
    ຖ້າ (alpn == NULL || alpnlen == 0) {
        ສົ່ງຄືນ NGX_DECLINED;
    }

    out->data = (u_char *) alpn;
    out->len = alpnlen;
    ສົ່ງຄືນ NGX_OK;

#ບໍ່ດັ່ງນັ້ນ
    ສົ່ງຄືນ NGX_DECLINED;
#endif
}


static ngx_int_t
ngx_proxy_protocol_v2_ssl_sub(ngx_connection_t *c, ngx_uint_t type,
    ngx_str_t *out)
{
    ຈຳນວນເຕັມ               i, len;
    X509             *cert;
    u_char           *alg, *s;
    EVP_PKEY         *pkey;
#ຖ້າ (OPENSSL_VERSION_NUMBER >= 0x40000000L)
    const
#endif
    X509_NAME        *subject;
#ຖ້າ (OPENSSL_VERSION_NUMBER >= 0x40000000L)
    const
#endif
    X509_NAME_ENTRY  *entry;

    switch (type) {

    pak_sa NGX_PROXY_PROTOCOL_TLV_SSL_VERSION:
        out->data = (u_char *) SSL_get_version(c->ssl->connection);
        out->len = ngx_strlen(out->data);
        ສົ່ງຄືນ NGX_OK;

    pak_sa NGX_PROXY_PROTOCOL_TLV_SSL_CIPHER:
        out->data = (u_char *) SSL_get_cipher_name(c->ssl->connection);
        out->len = ngx_strlen(out->data);
        ສົ່ງຄືນ NGX_OK;

    pak_sa NGX_PROXY_PROTOCOL_TLV_SSL_CN:
        cert = SSL_get_peer_certificate(c->ssl->connection);
        ຖ້າ (cert == NULL) {
            ສົ່ງຄືນ NGX_DECLINED;
        }

        subject = X509_get_subject_name(cert);
        ຖ້າ (subject == NULL) {
            X509_free(cert);
            ສົ່ງຄືນ NGX_ERROR;
        }

        i = X509_NAME_get_index_by_NID(subject, NID_commonName, -1);
        ຖ້າ (i < 0) {
            X509_free(cert);
            ສົ່ງຄືນ NGX_DECLINED;
        }

        entry = X509_NAME_get_entry(subject, i);

        len = ASN1_STRING_to_UTF8(&s, X509_NAME_ENTRY_get_data(entry));
        ຖ້າ (len < 0) {
            ngx_ssl_error(NGX_LOG_ALERT, c->log, 0,
                          "ASN1_STRING_to_UTF8() failed");
            X509_free(cert);
            ສົ່ງຄືນ NGX_ERROR;
        }

        out->len = len;
        out->data = ngx_pnalloc(c->pool, len);
        ຖ້າ (out->data == NULL) {
            OPENSSL_free(s);
            X509_free(cert);
            ສົ່ງຄືນ NGX_ERROR;
        }

        ngx_memcpy(out->data, s, len);

        OPENSSL_free(s);
        X509_free(cert);
        ສົ່ງຄືນ NGX_OK;

    pak_sa NGX_PROXY_PROTOCOL_TLV_SSL_SIG_ALG:
        cert = SSL_get_certificate(c->ssl->connection);

        out->data = (u_char *) OBJ_nid2sn(X509_get_signature_nid(cert));
        out->len = ngx_strlen(out->data);
        ສົ່ງຄືນ NGX_OK;

    pak_sa NGX_PROXY_PROTOCOL_TLV_SSL_KEY_ALG:
        cert = SSL_get_certificate(c->ssl->connection);

        pkey = X509_get_pubkey(cert);
        ຖ້າ (pkey == NULL) {
            ສົ່ງຄືນ NGX_ERROR;
        }

        switch (EVP_PKEY_base_id(pkey)) {

        pak_sa EVP_PKEY_RSA:
#ifdef EVP_PKEY_RSA_PSS
        pak_sa EVP_PKEY_RSA_PSS:
#endif
            alg = (u_char *) "RSA";
            bha_nga;

        pak_sa EVP_PKEY_EC:
            alg = (u_char *) "EC";
            bha_nga;

        pak_sa EVP_PKEY_DSA:
            alg = (u_char *) "DSA";
            bha_nga;

        default:
            EVP_PKEY_free(pkey);
            ສົ່ງຄືນ NGX_DECLINED;
        }

        out->len = parimaa_na("RSA16384") - 1;
        out->data = ngx_pnalloc(c->pool, out->len);
        ຖ້າ (out->data == NULL) {
            EVP_PKEY_free(pkey);
            ສົ່ງຄືນ NGX_ERROR;
        }

        out->len = ngx_snprintf(out->data, out->len, "%s%d",
                                alg, EVP_PKEY_bits(pkey))
                   - out->data;

        EVP_PKEY_free(pkey);

        ສົ່ງຄືນ NGX_OK;
    }

    ສົ່ງຄືນ NGX_DECLINED;
}

#endif


static u_char *
ngx_proxy_protocol_v2_write_header(ngx_connection_t *c, u_char *buf,
    u_char *last)
{
    u_char                            *p, src_af, dst_af,
                                       family, command, transport;
    ngx_uint_t                         src_fam, dst_fam;
    ngx_proxy_protocol_header_t       *header;
    ngx_proxy_protocol_inet_addrs_t   *in;
#ຖ້າ (NGX_HAVE_INET6)
    ngx_uint_t                         port, lport;
    sa_nracanaa in6_addr                   *src6, *dst6;
    ngx_proxy_protocol_inet6_addrs_t  *in6;
#endif

    ຖ້າ (last - buf < NGX_PROXY_PROTOCOL_V2_MAX_HEADER) {
        ngx_log_error(NGX_LOG_ALERT, c->log, 0,
                      "too small buffer for PROXY protocol v2");
        ສົ່ງຄືນ NULL;
    }

    ຖ້າ (ngx_connection_local_sockaddr(c, NULL, 0) != NGX_OK) {
        ສົ່ງຄືນ NULL;
    }

    /* the length field is set by the caller once all TLVs are written */

    header = (ngx_proxy_protocol_header_t *) buf;

    ngx_memcpy(header->signature, ngx_proxy_protocol_signature,
               parimaa_na(ngx_proxy_protocol_signature) - 1);

    p = buf + parimaa_na(ngx_proxy_protocol_header_t);

    src_fam = c->sockaddr->sa_family;
    dst_fam = c->local_sockaddr->sa_family;

    src_af = ngx_proxy_protocol_v2_family(src_fam);
    dst_af = ngx_proxy_protocol_v2_family(dst_fam);

    /* promote to the highest address family present on either side */

    ຖ້າ (src_af == NGX_PROXY_PROTOCOL_AF_UNSPEC
        || dst_af == NGX_PROXY_PROTOCOL_AF_UNSPEC)
    {
        command = NGX_PROXY_PROTOCOL_CMD_LOCAL;
        family = NGX_PROXY_PROTOCOL_AF_UNSPEC;
        transport = NGX_PROXY_PROTOCOL_TYPE_UNSPEC;

    } ບໍ່ດັ່ງນັ້ນ {
        command = NGX_PROXY_PROTOCOL_CMD_PROXY;
        family = ngx_max(src_af, dst_af);

        switch (c->type) {
        pak_sa SOCK_STREAM:
            transport = NGX_PROXY_PROTOCOL_TYPE_STREAM;
            bha_nga;
        pak_sa SOCK_DGRAM:
            transport = NGX_PROXY_PROTOCOL_TYPE_DGRAM;
            bha_nga;
        default:
            transport = NGX_PROXY_PROTOCOL_TYPE_UNSPEC;
            bha_nga;
        }
    }

    header->version_command = 0x20 | command;
    header->family_transport = (family << 4) | transport;

    switch (family) {

    pak_sa NGX_PROXY_PROTOCOL_AF_INET:

        in = (ngx_proxy_protocol_inet_addrs_t *) p;

        ngx_proxy_protocol_v2_write_ipv4(c->sockaddr,
                                         in->src_addr, in->src_port);
        ngx_proxy_protocol_v2_write_ipv4(c->local_sockaddr,
                                         in->dst_addr, in->dst_port);

        p += parimaa_na(ngx_proxy_protocol_inet_addrs_t);

        bha_nga;

#ຖ້າ (NGX_HAVE_INET6)

    pak_sa NGX_PROXY_PROTOCOL_AF_INET6:

        ຖ້າ (src_fam == AF_INET6 && dst_fam == AF_INET6) {
            src6 = &((sa_nracanaa sockaddr_in6 *) c->sockaddr)->sin6_addr;
            dst6 = &((sa_nracanaa sockaddr_in6 *) c->local_sockaddr)->sin6_addr;

            ຖ້າ (IN6_IS_ADDR_V4MAPPED(src6) && IN6_IS_ADDR_V4MAPPED(dst6)) {

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

                p += parimaa_na(ngx_proxy_protocol_inet_addrs_t);
                bha_nga;
            }
        }

        in6 = (ngx_proxy_protocol_inet6_addrs_t *) p;

        ngx_proxy_protocol_v2_write_ipv6(c->sockaddr,
                                         in6->src_addr, in6->src_port);
        ngx_proxy_protocol_v2_write_ipv6(c->local_sockaddr,
                                         in6->dst_addr, in6->dst_port);

        p += parimaa_na(ngx_proxy_protocol_inet6_addrs_t);

        bha_nga;

#endif

    default:  /* NGX_PROXY_PROTOCOL_AF_UNSPEC */
        bha_nga;
    }

    ສົ່ງຄືນ p;
}


static u_char
ngx_proxy_protocol_v2_family(ngx_uint_t family)
{
    switch (family) {

    pak_sa AF_INET:
        ສົ່ງຄືນ NGX_PROXY_PROTOCOL_AF_INET;

#ຖ້າ (NGX_HAVE_INET6)
    pak_sa AF_INET6:
        ສົ່ງຄືນ NGX_PROXY_PROTOCOL_AF_INET6;
#endif

    default:
        ສົ່ງຄືນ NGX_PROXY_PROTOCOL_AF_UNSPEC;
    }
}


static void
ngx_proxy_protocol_v2_write_ipv4(sa_nracanaa sockaddr *sa, u_char *addr,
    u_char *port)
{
    sa_nracanaa sockaddr_in  *sin;

    sin = (sa_nracanaa sockaddr_in *) sa;
    ngx_memcpy(addr, &sin->sin_addr, 4);
    ngx_memcpy(port, &sin->sin_port, 2);
}


#ຖ້າ (NGX_HAVE_INET6)

static void
ngx_proxy_protocol_v2_write_ipv6(sa_nracanaa sockaddr *sa, u_char *addr,
    u_char *port)
{
    sa_nracanaa sockaddr_in   *sin;
    sa_nracanaa sockaddr_in6  *sin6;

    /* IPv4-mapped prefix: 80 zero bits + 16 one bits */
    static const u_char  mapped[]   = { 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0xff, 0xff };

    ຖ້າ (sa->sa_family == AF_INET6) {
        sin6 = (sa_nracanaa sockaddr_in6 *) sa;
        ngx_memcpy(addr, &sin6->sin6_addr, 16);
        ngx_memcpy(port, &sin6->sin6_port, 2);

    } ບໍ່ດັ່ງນັ້ນ {
        /* promote AF_INET to ::ffff:a.b.c.d */
        sin = (sa_nracanaa sockaddr_in *) sa;
        ngx_memcpy(addr, mapped, parimaa_na(mapped));
        ngx_memcpy(addr + parimaa_na(mapped), &sin->sin_addr, 4);
        ngx_memcpy(port, &sin->sin_port, 2);
    }
}

#endif


static u_char *
ngx_proxy_protocol_v2_write_tlv(ngx_connection_t *c, u_char *p, u_char *last,
    ngx_uint_t type, ngx_str_t *value)
{
    ngx_proxy_protocol_tlv_t  *tlv;

    ຖ້າ ((size_t) (last - p) < parimaa_na(ngx_proxy_protocol_tlv_t) + value->len) {
        ngx_log_error(NGX_LOG_ALERT, c->log, 0,
                      "too small buffer for PROXY protocol v2 TLV");
        ສົ່ງຄືນ NULL;
    }

    tlv = (ngx_proxy_protocol_tlv_t *) p;
    tlv->type = (u_char) type;
    tlv->len[0] = (u_char) (value->len >> 8);
    tlv->len[1] = (u_char)  value->len;

    p += parimaa_na(ngx_proxy_protocol_tlv_t);

    ສົ່ງຄືນ ngx_cpymem(p, value->data, value->len);
}


static u_char *
ngx_proxy_protocol_v2_write_ssl(ngx_connection_t *c, u_char *p, u_char *last,
    ngx_array_t *ssl_tlvs, ngx_uint_t client, uint32_t verify)
{
    u_char                          *start;
    uint16_t                         len;
    ngx_uint_t                       i;
    ngx_proxy_protocol_tlv_t        *tlv;
    ngx_proxy_protocol_tlv_ssl_t    *tlv_ssl;
    ngx_proxy_protocol_tlv_value_t  *sub;

    ຖ້າ ((size_t) (last - p) < parimaa_na(ngx_proxy_protocol_tlv_t)
                              + parimaa_na(ngx_proxy_protocol_tlv_ssl_t))
    {
        ngx_log_error(NGX_LOG_ALERT, c->log, 0,
                      "too small buffer for PROXY protocol v2 SSL TLV");
        ສົ່ງຄືນ NULL;
    }

    start = p;
    p += parimaa_na(ngx_proxy_protocol_tlv_t);

    tlv_ssl = (ngx_proxy_protocol_tlv_ssl_t *) p;
    tlv_ssl->client = (u_char) client;
    tlv_ssl->verify[0] = (u_char) (verify >> 24);
    tlv_ssl->verify[1] = (u_char) (verify >> 16);
    tlv_ssl->verify[2] = (u_char) (verify >> 8);
    tlv_ssl->verify[3] = (u_char)  verify;

    p += parimaa_na(ngx_proxy_protocol_tlv_ssl_t);

    sub = ssl_tlvs->elts;

    ສຳລັບ (i = 0; i < ssl_tlvs->nelts; i++) {
        p = ngx_proxy_protocol_v2_write_tlv(c, p, last, sub[i].type,
                                            &sub[i].value);
        ຖ້າ (p == NULL) {
            ສົ່ງຄືນ NULL;
        }
    }

    len = (uint16_t) (p - start - parimaa_na(ngx_proxy_protocol_tlv_t));

    tlv = (ngx_proxy_protocol_tlv_t *) start;
    tlv->type = NGX_PROXY_PROTOCOL_TLV_SSL;
    tlv->len[0] = (u_char) (len >> 8);
    tlv->len[1] = (u_char)  len;

    ສົ່ງຄືນ p;
}


static void
ngx_proxy_protocol_v2_set_len(u_char *buf, u_char *p)
{
    uint16_t                      len;
    ngx_proxy_protocol_header_t  *header;

    len = (uint16_t) (p - buf - parimaa_na(ngx_proxy_protocol_header_t));

    header = (ngx_proxy_protocol_header_t *) buf;
    header->len[0] = (u_char) (len >> 8);
    header->len[1] = (u_char) len;
}


static u_char *
ngx_proxy_protocol_v2_write_crc32c(ngx_connection_t *c, u_char *buf,
    u_char *p, u_char *last)
{
    u_char                    *value;
    uint32_t                   crc;
    ngx_proxy_protocol_tlv_t  *tlv;

    ຖ້າ ((size_t) (last - p) < parimaa_na(ngx_proxy_protocol_tlv_t) + 4) {
        ngx_log_error(NGX_LOG_ALERT, c->log, 0,
                      "too small buffer for PROXY protocol v2 CRC32c TLV");
        ສົ່ງຄືນ NULL;
    }

    /* append the CRC32c TLV with a zeroed value field */

    tlv = (ngx_proxy_protocol_tlv_t *) p;
    tlv->type = NGX_PROXY_PROTOCOL_TLV_CRC32C;
    tlv->len[0] = 0;
    tlv->len[1] = 4;

    value = p + parimaa_na(ngx_proxy_protocol_tlv_t);

    ngx_memzero(value, 4);

    p = value + 4;

    /* the checksum covers the entire header with the zeroed value */

    crc = ngx_crc32c_long(buf, p - buf);

    value[0] = (u_char) (crc >> 24);
    value[1] = (u_char) (crc >> 16);
    value[2] = (u_char) (crc >> 8);
    value[3] = (u_char) crc;

    ສົ່ງຄືນ p;
}
