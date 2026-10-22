
/*
 * Copyright (C) Roman Arutyunyan
 * Copyright (C) Nginx, Inc.
 */


#அந்தர்பாவ <ngx_config.h>
#அந்தர்பாவ <ngx_core.h>


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
#define NGX_PROXY_PROTOCOL_TLV_SSL_சாவி_ALG  0x25
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


வகைவரை கட்டமைப்பு {
    u_char                                  signature[12];
    u_char                                  version_command;
    u_char                                  family_transport;
    u_char                                  len[2];
} ngx_proxy_protocol_header_t;


வகைவரை கட்டமைப்பு {
    u_char                                  src_addr[4];
    u_char                                  dst_addr[4];
    u_char                                  src_port[2];
    u_char                                  dst_port[2];
} ngx_proxy_protocol_inet_addrs_t;


வகைவரை கட்டமைப்பு {
    u_char                                  src_addr[16];
    u_char                                  dst_addr[16];
    u_char                                  src_port[2];
    u_char                                  dst_port[2];
} ngx_proxy_protocol_inet6_addrs_t;


வகைவரை கட்டமைப்பு {
    u_char                                  type;
    u_char                                  len[2];
} ngx_proxy_protocol_tlv_t;


வகைவரை கட்டமைப்பு {
    u_char                                  client;
    u_char                                  verify[4];
} ngx_proxy_protocol_tlv_ssl_t;


வகைவரை கட்டமைப்பு {
    ngx_str_t                               name;
    ngx_uint_t                              type;
} ngx_proxy_protocol_tlv_entry_t;


வகைவரை கட்டமைப்பு {
    ngx_uint_t                              type;
    ngx_str_t                               மதிப்பு;
} ngx_proxy_protocol_tlv_மதிப்பு_t;


நிலையான u_char *ngx_proxy_protocol_read_addr(ngx_connection_t *c, u_char *p,
    u_char *last, ngx_str_t *addr);
நிலையான u_char *ngx_proxy_protocol_read_port(u_char *p, u_char *last,
    in_port_t *port, u_char sep);
நிலையான u_char *ngx_proxy_protocol_v2_read(ngx_connection_t *c, u_char *buf,
    u_char *last);
நிலையான ngx_int_t ngx_proxy_protocol_lookup_tlv(ngx_connection_t *c,
    ngx_str_t *tlvs, ngx_uint_t type, ngx_str_t *மதிப்பு);
#எனில் (NGX_OPENSSL)
நிலையான ngx_int_t ngx_proxy_protocol_v2_eval_ssl(ngx_connection_t *c,
    ngx_array_t *tlvs, ngx_array_t *ssl_tlvs, ngx_uint_t *client,
    uint32_t *verify);
நிலையான ngx_int_t ngx_proxy_protocol_v2_authority(ngx_connection_t *c,
    ngx_str_t *out);
நிலையான ngx_int_t ngx_proxy_protocol_v2_alpn(ngx_connection_t *c,
    ngx_str_t *out);
நிலையான ngx_int_t ngx_proxy_protocol_v2_ssl_sub(ngx_connection_t *c,
    ngx_uint_t type, ngx_str_t *out);
#endif
நிலையான u_char *ngx_proxy_protocol_v2_write_header(ngx_connection_t *c,
    u_char *buf, u_char *last);
நிலையான u_char ngx_proxy_protocol_v2_family(ngx_uint_t family);
நிலையான வெறுமை ngx_proxy_protocol_v2_write_ipv4(கட்டமைப்பு sockaddr *sa, u_char *addr,
    u_char *port);
நிலையான வெறுமை ngx_proxy_protocol_v2_write_ipv6(கட்டமைப்பு sockaddr *sa, u_char *addr,
    u_char *port);
நிலையான u_char *ngx_proxy_protocol_v2_write_tlv(ngx_connection_t *c, u_char *p,
    u_char *last, ngx_uint_t type, ngx_str_t *மதிப்பு);
நிலையான u_char *ngx_proxy_protocol_v2_write_ssl(ngx_connection_t *c, u_char *p,
    u_char *last, ngx_array_t *ssl_tlvs, ngx_uint_t client, uint32_t verify);
நிலையான வெறுமை ngx_proxy_protocol_v2_set_len(u_char *buf, u_char *p);
நிலையான u_char *ngx_proxy_protocol_v2_write_crc32c(ngx_connection_t *c,
    u_char *buf, u_char *p, u_char *last);


நிலையான ngx_proxy_protocol_tlv_entry_t  ngx_proxy_protocol_tlv_entries[] = {
    { ngx_string("alpn"),       NGX_PROXY_PROTOCOL_TLV_ALPN },
    { ngx_string("authority"),  NGX_PROXY_PROTOCOL_TLV_AUTHORITY },
    { ngx_string("unique_id"),  NGX_PROXY_PROTOCOL_TLV_UNIQUE_ID },
    { ngx_string("ssl"),        NGX_PROXY_PROTOCOL_TLV_SSL },
    { ngx_string("netns"),      NGX_PROXY_PROTOCOL_TLV_NETNS },
    { ngx_null_string,          0x00 }
};


நிலையான ngx_proxy_protocol_tlv_entry_t  ngx_proxy_protocol_tlv_ssl_entries[] = {
    { ngx_string("version"),    NGX_PROXY_PROTOCOL_TLV_SSL_VERSION },
    { ngx_string("cn"),         NGX_PROXY_PROTOCOL_TLV_SSL_CN },
    { ngx_string("cipher"),     NGX_PROXY_PROTOCOL_TLV_SSL_CIPHER },
    { ngx_string("sig_alg"),    NGX_PROXY_PROTOCOL_TLV_SSL_SIG_ALG },
    { ngx_string("key_alg"),    NGX_PROXY_PROTOCOL_TLV_SSL_சாவி_ALG },
    { ngx_null_string,          0x00 }
};


நிலையான மாறிலி u_char  ngx_proxy_protocol_signature[] = "\r\n\r\n\0\r\nQUIT\n";


u_char *
ngx_proxy_protocol_read(ngx_connection_t *c, u_char *buf, u_char *last)
{
    size_t                 len;
    u_char                *p;
    ngx_proxy_protocol_t  *pp;

    p = buf;
    len = last - buf;

    எனில் (len >= அளவு(ngx_proxy_protocol_header_t)
        && ngx_memcmp(p, ngx_proxy_protocol_signature,
                      அளவு(ngx_proxy_protocol_signature) - 1)
           == 0)
    {
        திரும்பு ngx_proxy_protocol_v2_read(c, buf, last);
    }

    எனில் (len < 8 || ngx_strncmp(p, "PROXY ", 6) != 0) {
        செல் invalid;
    }

    p += 6;
    len -= 6;

    எனில் (len >= 7 && ngx_strncmp(p, "UNKNOWN", 7) == 0) {
        ngx_log_debug0(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "PROXY protocol unknown protocol");
        p += 7;
        செல் skip;
    }

    எனில் (len < 5 || ngx_strncmp(p, "TCP", 3) != 0
        || (p[3] != '4' && p[3] != '6') || p[4] != ' ')
    {
        செல் invalid;
    }

    p += 5;

    pp = ngx_pcalloc(c->pool, அளவு(ngx_proxy_protocol_t));
    எனில் (pp == வெற்று) {
        திரும்பு வெற்று;
    }

    p = ngx_proxy_protocol_read_addr(c, p, last, &pp->src_addr);
    எனில் (p == வெற்று) {
        செல் invalid;
    }

    p = ngx_proxy_protocol_read_addr(c, p, last, &pp->dst_addr);
    எனில் (p == வெற்று) {
        செல் invalid;
    }

    p = ngx_proxy_protocol_read_port(p, last, &pp->src_port, ' ');
    எனில் (p == வெற்று) {
        செல் invalid;
    }

    p = ngx_proxy_protocol_read_port(p, last, &pp->dst_port, CR);
    எனில் (p == வெற்று) {
        செல் invalid;
    }

    எனில் (p == last) {
        செல் invalid;
    }

    எனில் (*p++ != LF) {
        செல் invalid;
    }

    ngx_log_debug4(NGX_LOG_DEBUG_CORE, c->log, 0,
                   "PROXY protocol src: %V %d, dst: %V %d",
                   &pp->src_addr, pp->src_port, &pp->dst_addr, pp->dst_port);

    c->proxy_protocol = pp;

    திரும்பு p;

skip:

    க்கு ( /* void */ ; p < last - 1; p++) {
        எனில் (p[0] == CR && p[1] == LF) {
            திரும்பு p + 2;
        }
    }

invalid:

    க்கு (p = buf; p < last; p++) {
        எனில் (*p == CR || *p == LF) {
            நிறுத்து;
        }
    }

    ngx_log_error(NGX_LOG_ERR, c->log, 0,
                  "broken header: \"%*s\"", (size_t) (p - buf), buf);

    திரும்பு வெற்று;
}


நிலையான u_char *
ngx_proxy_protocol_read_addr(ngx_connection_t *c, u_char *p, u_char *last,
    ngx_str_t *addr)
{
    size_t  len;
    u_char  ch, *pos;

    pos = p;

    க்கு ( ;; ) {
        எனில் (p == last) {
            திரும்பு வெற்று;
        }

        ch = *p++;

        எனில் (ch == ' ') {
            நிறுத்து;
        }

        எனில் (ch != ':' && ch != '.'
            && (ch < 'a' || ch > 'f')
            && (ch < 'A' || ch > 'F')
            && (ch < '0' || ch > '9'))
        {
            திரும்பு வெற்று;
        }
    }

    len = p - pos - 1;

    addr->data = ngx_pnalloc(c->pool, len);
    எனில் (addr->data == வெற்று) {
        திரும்பு வெற்று;
    }

    ngx_memcpy(addr->data, pos, len);
    addr->len = len;

    திரும்பு p;
}


நிலையான u_char *
ngx_proxy_protocol_read_port(u_char *p, u_char *last, in_port_t *port,
    u_char sep)
{
    size_t      len;
    u_char     *pos;
    ngx_int_t   n;

    pos = p;

    க்கு ( ;; ) {
        எனில் (p == last) {
            திரும்பு வெற்று;
        }

        எனில் (*p++ == sep) {
            நிறுத்து;
        }
    }

    len = p - pos - 1;

    n = ngx_atoi(pos, len);
    எனில் (n < 0 || n > 65535) {
        திரும்பு வெற்று;
    }

    *port = (in_port_t) n;

    திரும்பு p;
}


u_char *
ngx_proxy_protocol_write(ngx_connection_t *c, u_char *buf, u_char *last)
{
    ngx_uint_t  port, lport;

    எனில் (last - buf < NGX_PROXY_PROTOCOL_V1_MAX_HEADER) {
        ngx_log_error(NGX_LOG_ALERT, c->log, 0,
                      "too small buffer for PROXY protocol");
        திரும்பு வெற்று;
    }

    எனில் (ngx_connection_local_sockaddr(c, வெற்று, 0) != NGX_OK) {
        திரும்பு வெற்று;
    }

    தேர்வு (c->sockaddr->sa_family) {

    நிலை AF_INET:
        buf = ngx_cpymem(buf, "PROXY TCP4 ", அளவு("PROXY TCP4 ") - 1);
        நிறுத்து;

#எனில் (NGX_HAVE_INET6)
    நிலை AF_INET6:
        buf = ngx_cpymem(buf, "PROXY TCP6 ", அளவு("PROXY TCP6 ") - 1);
        நிறுத்து;
#endif

    இயல்பு:
        திரும்பு ngx_cpymem(buf, "PROXY UNKNOWN" CRLF,
                          அளவு("PROXY UNKNOWN" CRLF) - 1);
    }

    buf += ngx_sock_ntop(c->sockaddr, c->socklen, buf, last - buf, 0);

    *buf++ = ' ';

    buf += ngx_sock_ntop(c->local_sockaddr, c->local_socklen, buf, last - buf,
                         0);

    port = ngx_inet_get_port(c->sockaddr);
    lport = ngx_inet_get_port(c->local_sockaddr);

    திரும்பு ngx_slprintf(buf, last, " %ui %ui" CRLF, port, lport);
}


நிலையான u_char *
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
#எனில் (NGX_HAVE_INET6)
    ngx_proxy_protocol_inet6_addrs_t   *in6;
#endif

    header = (ngx_proxy_protocol_header_t *) buf;

    buf += அளவு(ngx_proxy_protocol_header_t);

    version = header->version_command >> 4;

    எனில் (version != 2) {
        ngx_log_error(NGX_LOG_ERR, c->log, 0,
                      "unknown PROXY protocol version: %ui", version);
        திரும்பு வெற்று;
    }

    len = ngx_proxy_protocol_parse_uint16(header->len);

    எனில் ((size_t) (last - buf) < len) {
        ngx_log_error(NGX_LOG_ERR, c->log, 0, "header is too large");
        திரும்பு வெற்று;
    }

    end = buf + len;

    command = header->version_command & 0x0f;

    /* only PROXY is supported */
    எனில் (command != NGX_PROXY_PROTOCOL_CMD_PROXY) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "PROXY protocol v2 unsupported command %ui", command);
        திரும்பு end;
    }

    transport = header->family_transport & 0x0f;

    /* only STREAM is supported */
    எனில் (transport != NGX_PROXY_PROTOCOL_TYPE_STREAM) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "PROXY protocol v2 unsupported transport %ui",
                       transport);
        திரும்பு end;
    }

    pp = ngx_pcalloc(c->pool, அளவு(ngx_proxy_protocol_t));
    எனில் (pp == வெற்று) {
        திரும்பு வெற்று;
    }

    family = header->family_transport >> 4;

    தேர்வு (family) {

    நிலை NGX_PROXY_PROTOCOL_AF_INET:

        எனில் ((size_t) (end - buf) < அளவு(ngx_proxy_protocol_inet_addrs_t)) {
            திரும்பு வெற்று;
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

        socklen = அளவு(கட்டமைப்பு sockaddr_in);

        buf += அளவு(ngx_proxy_protocol_inet_addrs_t);

        நிறுத்து;

#எனில் (NGX_HAVE_INET6)

    நிலை NGX_PROXY_PROTOCOL_AF_INET6:

        எனில் ((size_t) (end - buf) < அளவு(ngx_proxy_protocol_inet6_addrs_t)) {
            திரும்பு வெற்று;
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

        socklen = அளவு(கட்டமைப்பு sockaddr_in6);

        buf += அளவு(ngx_proxy_protocol_inet6_addrs_t);

        நிறுத்து;

#endif

    இயல்பு:
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "PROXY protocol v2 unsupported address family %ui",
                       family);
        திரும்பு end;
    }

    pp->src_addr.data = ngx_pnalloc(c->pool, NGX_SOCKADDR_STRLEN);
    எனில் (pp->src_addr.data == வெற்று) {
        திரும்பு வெற்று;
    }

    pp->src_addr.len = ngx_sock_ntop(&src_sockaddr.sockaddr, socklen,
                                     pp->src_addr.data, NGX_SOCKADDR_STRLEN, 0);

    pp->dst_addr.data = ngx_pnalloc(c->pool, NGX_SOCKADDR_STRLEN);
    எனில் (pp->dst_addr.data == வெற்று) {
        திரும்பு வெற்று;
    }

    pp->dst_addr.len = ngx_sock_ntop(&dst_sockaddr.sockaddr, socklen,
                                     pp->dst_addr.data, NGX_SOCKADDR_STRLEN, 0);

    ngx_log_debug4(NGX_LOG_DEBUG_CORE, c->log, 0,
                   "PROXY protocol v2 src: %V %d, dst: %V %d",
                   &pp->src_addr, pp->src_port, &pp->dst_addr, pp->dst_port);

    எனில் (buf < end) {
        pp->tlvs.data = ngx_pnalloc(c->pool, end - buf);
        எனில் (pp->tlvs.data == வெற்று) {
            திரும்பு வெற்று;
        }

        ngx_memcpy(pp->tlvs.data, buf, end - buf);
        pp->tlvs.len = end - buf;
    }

    c->proxy_protocol = pp;

    திரும்பு end;
}


ngx_int_t
ngx_proxy_protocol_get_tlv(ngx_connection_t *c, ngx_str_t *name,
    ngx_str_t *மதிப்பு)
{
    u_char                          *p;
    size_t                           n;
    uint32_t                         verify;
    ngx_str_t                        ssl, *tlvs;
    ngx_int_t                        rc, type;
    ngx_proxy_protocol_tlv_ssl_t    *tlv_ssl;
    ngx_proxy_protocol_tlv_entry_t  *te;

    எனில் (c->proxy_protocol == வெற்று) {
        திரும்பு NGX_DECLINED;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                   "PROXY protocol v2 get tlv \"%V\"", name);

    te = ngx_proxy_protocol_tlv_entries;
    tlvs = &c->proxy_protocol->tlvs;

    p = name->data;
    n = name->len;

    எனில் (n >= 4 && p[0] == 's' && p[1] == 's' && p[2] == 'l' && p[3] == '_') {

        rc = ngx_proxy_protocol_lookup_tlv(c, tlvs,
                                           NGX_PROXY_PROTOCOL_TLV_SSL, &ssl);
        எனில் (rc != NGX_OK) {
            திரும்பு rc;
        }

        எனில் (ssl.len < அளவு(ngx_proxy_protocol_tlv_ssl_t)) {
            திரும்பு NGX_ERROR;
        }

        p += 4;
        n -= 4;

        எனில் (n == 6 && ngx_strncmp(p, "verify", 6) == 0) {

            tlv_ssl = (ngx_proxy_protocol_tlv_ssl_t *) ssl.data;
            verify = ngx_proxy_protocol_parse_uint32(tlv_ssl->verify);

            மதிப்பு->data = ngx_pnalloc(c->pool, NGX_INT32_LEN);
            எனில் (மதிப்பு->data == வெற்று) {
                திரும்பு NGX_ERROR;
            }

            மதிப்பு->len = ngx_sprintf(மதிப்பு->data, "%uD", verify)
                         - மதிப்பு->data;
            திரும்பு NGX_OK;
        }

        ssl.data += அளவு(ngx_proxy_protocol_tlv_ssl_t);
        ssl.len -= அளவு(ngx_proxy_protocol_tlv_ssl_t);

        te = ngx_proxy_protocol_tlv_ssl_entries;
        tlvs = &ssl;
    }

    எனில் (n >= 2 && p[0] == '0' && p[1] == 'x') {

        type = ngx_hextoi(p + 2, n - 2);
        எனில் (type == NGX_ERROR) {
            ngx_log_error(NGX_LOG_ERR, c->log, 0,
                          "invalid PROXY protocol TLV \"%V\"", name);
            திரும்பு NGX_ERROR;
        }

        திரும்பு ngx_proxy_protocol_lookup_tlv(c, tlvs, type, மதிப்பு);
    }

    க்கு ( /* void */ ; te->type; te++) {
        எனில் (te->name.len == n && ngx_strncmp(te->name.data, p, n) == 0) {
            திரும்பு ngx_proxy_protocol_lookup_tlv(c, tlvs, te->type, மதிப்பு);
        }
    }

    ngx_log_error(NGX_LOG_ERR, c->log, 0,
                  "unknown PROXY protocol TLV \"%V\"", name);

    திரும்பு NGX_DECLINED;
}


நிலையான ngx_int_t
ngx_proxy_protocol_lookup_tlv(ngx_connection_t *c, ngx_str_t *tlvs,
    ngx_uint_t type, ngx_str_t *மதிப்பு)
{
    u_char                    *p;
    size_t                     n, len;
    ngx_proxy_protocol_tlv_t  *tlv;

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                   "PROXY protocol v2 lookup tlv:%02xi", type);

    p = tlvs->data;
    n = tlvs->len;

    போது (n) {
        எனில் (n < அளவு(ngx_proxy_protocol_tlv_t)) {
            ngx_log_error(NGX_LOG_ERR, c->log, 0, "broken PROXY protocol TLV");
            திரும்பு NGX_ERROR;
        }

        tlv = (ngx_proxy_protocol_tlv_t *) p;
        len = ngx_proxy_protocol_parse_uint16(tlv->len);

        p += அளவு(ngx_proxy_protocol_tlv_t);
        n -= அளவு(ngx_proxy_protocol_tlv_t);

        எனில் (n < len) {
            ngx_log_error(NGX_LOG_ERR, c->log, 0, "broken PROXY protocol TLV");
            திரும்பு NGX_ERROR;
        }

        எனில் (tlv->type == type) {
            மதிப்பு->data = p;
            மதிப்பு->len = len;
            திரும்பு NGX_OK;
        }

        p += len;
        n -= len;
    }

    திரும்பு NGX_DECLINED;
}


u_char *
ngx_proxy_protocol_v2_write(ngx_connection_t *c, u_char *buf, u_char *last,
    ngx_array_t *tlvs /* reserved */ )
{
    u_char                          *p;
    uint32_t                         verify;
    ngx_uint_t                       i, client;
    ngx_array_t                      base_tlvs, ssl_tlvs;
    ngx_proxy_protocol_tlv_மதிப்பு_t  *tlv;

    client = 0;
    verify = 0;

    ngx_memzero(&base_tlvs, அளவு(ngx_array_t));
    ngx_memzero(&ssl_tlvs, அளவு(ngx_array_t));

#எனில் (NGX_OPENSSL)

    எனில் (c->ssl != வெற்று) {
        எனில் (ngx_array_init(&base_tlvs, c->pool, 2,
                           அளவு(ngx_proxy_protocol_tlv_மதிப்பு_t))
            != NGX_OK)
        {
            திரும்பு வெற்று;
        }

        எனில் (ngx_array_init(&ssl_tlvs, c->pool, 5,
                           அளவு(ngx_proxy_protocol_tlv_மதிப்பு_t))
            != NGX_OK)
        {
            திரும்பு வெற்று;
        }

        எனில் (ngx_proxy_protocol_v2_eval_ssl(c, &base_tlvs, &ssl_tlvs, &client,
                                           &verify)
            != NGX_OK)
        {
            திரும்பு வெற்று;
        }
    }

#endif

    p = ngx_proxy_protocol_v2_write_header(c, buf, last);
    எனில் (p == வெற்று) {
        திரும்பு வெற்று;
    }

    tlv = base_tlvs.elts;

    க்கு (i = 0; i < base_tlvs.nelts; i++) {
        p = ngx_proxy_protocol_v2_write_tlv(c, p, last, tlv[i].type,
                                           &tlv[i].மதிப்பு);
        எனில் (p == வெற்று) {
            திரும்பு வெற்று;
        }
    }

    எனில் (client) {
        p = ngx_proxy_protocol_v2_write_ssl(c, p, last, &ssl_tlvs, client,
                                            verify);
        எனில் (p == வெற்று) {
            திரும்பு வெற்று;
        }
    }

    ngx_proxy_protocol_v2_set_len(buf,
                                  p + அளவு(ngx_proxy_protocol_tlv_t) + 4);

    திரும்பு ngx_proxy_protocol_v2_write_crc32c(c, buf, p, last);
}


#எனில் (NGX_OPENSSL)

நிலையான ngx_int_t
ngx_proxy_protocol_v2_eval_ssl(ngx_connection_t *c, ngx_array_t *tlvs,
    ngx_array_t *ssl_tlvs, ngx_uint_t *client, uint32_t *verify)
{
    X509                            *cert;
    long                             n;
    ngx_int_t                        rc;
    ngx_str_t                        மதிப்பு;
    ngx_uint_t                       type;
    மாறிலி எழுத்து                      *s;
    ngx_proxy_protocol_tlv_மதிப்பு_t  *tlv;

    எனில் (ngx_proxy_protocol_v2_authority(c, &மதிப்பு) == NGX_OK) {
        tlv = ngx_array_தள்ளு(tlvs);
        எனில் (tlv == வெற்று) {
            திரும்பு NGX_ERROR;
        }

        tlv->type = NGX_PROXY_PROTOCOL_TLV_AUTHORITY;
        tlv->மதிப்பு = மதிப்பு;
    }

    எனில் (ngx_proxy_protocol_v2_alpn(c, &மதிப்பு) == NGX_OK) {
        tlv = ngx_array_தள்ளு(tlvs);
        எனில் (tlv == வெற்று) {
            திரும்பு NGX_ERROR;
        }

        tlv->type = NGX_PROXY_PROTOCOL_TLV_ALPN;
        tlv->மதிப்பு = மதிப்பு;
    }

    க்கு (type = NGX_PROXY_PROTOCOL_TLV_SSL_VERSION;
         type <= NGX_PROXY_PROTOCOL_TLV_SSL_சாவி_ALG;
         type++)
    {
        rc = ngx_proxy_protocol_v2_ssl_sub(c, type, &மதிப்பு);

        எனில் (rc == NGX_ERROR) {
            திரும்பு NGX_ERROR;
        }

        எனில் (rc != NGX_OK) {
            தொடர்;
        }

        tlv = ngx_array_தள்ளு(ssl_tlvs);
        எனில் (tlv == வெற்று) {
            திரும்பு NGX_ERROR;
        }

        tlv->type = type;
        tlv->மதிப்பு = மதிப்பு;
    }

    *client = NGX_PROXY_PROTOCOL_V2_CLIENT_SSL;
    *verify = 1;  /* X509_V_ERR_UNSPECIFIED */

    cert = SSL_get_peer_certificate(c->ssl->connection);

    எனில் (cert != வெற்று) {
        X509_free(cert);

        *client |= NGX_PROXY_PROTOCOL_V2_CLIENT_CERT_SESS;

        எனில் (!SSL_session_reused(c->ssl->connection)) {
            *client |= NGX_PROXY_PROTOCOL_V2_CLIENT_CERT_CONN;
        }

        n = SSL_get_verify_result(c->ssl->connection);

        எனில் (n == X509_V_OK) {

            எனில் (ngx_ssl_ocsp_get_status(c, &s) != NGX_OK) {
                n = X509_V_ERR_CERT_REVOKED;
            }
        }

        *verify = (uint32_t) n;
    }

    திரும்பு NGX_OK;
}


நிலையான ngx_int_t
ngx_proxy_protocol_v2_authority(ngx_connection_t *c, ngx_str_t *out)
{
#ifdef SSL_CTRL_SET_TLSEXT_HOSTNAME

    மாறிலி எழுத்து  *sni;

    sni = SSL_get_servername(c->ssl->connection, TLSEXT_NAMETYPE_host_name);
    எனில் (sni == வெற்று) {
        திரும்பு NGX_DECLINED;
    }

    out->data = (u_char *) sni;
    out->len = ngx_strlen(sni);
    திரும்பு NGX_OK;

#இல்லையெனில்
    திரும்பு NGX_DECLINED;
#endif
}


நிலையான ngx_int_t
ngx_proxy_protocol_v2_alpn(ngx_connection_t *c, ngx_str_t *out)
{
#ifdef TLSEXT_TYPE_application_layer_protocol_negotiation

    unsigned முழு          alpnlen;
    மாறிலி unsigned எழுத்து  *alpn;

    SSL_get0_alpn_selected(c->ssl->connection, &alpn, &alpnlen);
    எனில் (alpn == வெற்று || alpnlen == 0) {
        திரும்பு NGX_DECLINED;
    }

    out->data = (u_char *) alpn;
    out->len = alpnlen;
    திரும்பு NGX_OK;

#இல்லையெனில்
    திரும்பு NGX_DECLINED;
#endif
}


நிலையான ngx_int_t
ngx_proxy_protocol_v2_ssl_sub(ngx_connection_t *c, ngx_uint_t type,
    ngx_str_t *out)
{
    முழு               i, len;
    X509             *cert;
    u_char           *alg, *s;
    EVP_PKEY         *pkey;
#எனில் (OPENSSL_VERSION_எண் >= 0x40000000L)
    மாறிலி
#endif
    X509_NAME        *subject;
#எனில் (OPENSSL_VERSION_எண் >= 0x40000000L)
    மாறிலி
#endif
    X509_NAME_ENTRY  *entry;

    தேர்வு (type) {

    நிலை NGX_PROXY_PROTOCOL_TLV_SSL_VERSION:
        out->data = (u_char *) SSL_get_version(c->ssl->connection);
        out->len = ngx_strlen(out->data);
        திரும்பு NGX_OK;

    நிலை NGX_PROXY_PROTOCOL_TLV_SSL_CIPHER:
        out->data = (u_char *) SSL_get_cipher_name(c->ssl->connection);
        out->len = ngx_strlen(out->data);
        திரும்பு NGX_OK;

    நிலை NGX_PROXY_PROTOCOL_TLV_SSL_CN:
        cert = SSL_get_peer_certificate(c->ssl->connection);
        எனில் (cert == வெற்று) {
            திரும்பு NGX_DECLINED;
        }

        subject = X509_get_subject_name(cert);
        எனில் (subject == வெற்று) {
            X509_free(cert);
            திரும்பு NGX_ERROR;
        }

        i = X509_NAME_get_index_by_NID(subject, NID_commonName, -1);
        எனில் (i < 0) {
            X509_free(cert);
            திரும்பு NGX_DECLINED;
        }

        entry = X509_NAME_get_entry(subject, i);

        len = ASN1_STRING_to_UTF8(&s, X509_NAME_ENTRY_get_data(entry));
        எனில் (len < 0) {
            ngx_ssl_error(NGX_LOG_ALERT, c->log, 0,
                          "ASN1_STRING_to_UTF8() failed");
            X509_free(cert);
            திரும்பு NGX_ERROR;
        }

        out->len = len;
        out->data = ngx_pnalloc(c->pool, len);
        எனில் (out->data == வெற்று) {
            OPENSSL_free(s);
            X509_free(cert);
            திரும்பு NGX_ERROR;
        }

        ngx_memcpy(out->data, s, len);

        OPENSSL_free(s);
        X509_free(cert);
        திரும்பு NGX_OK;

    நிலை NGX_PROXY_PROTOCOL_TLV_SSL_SIG_ALG:
        cert = SSL_get_certificate(c->ssl->connection);

        out->data = (u_char *) OBJ_nid2sn(X509_get_signature_nid(cert));
        out->len = ngx_strlen(out->data);
        திரும்பு NGX_OK;

    நிலை NGX_PROXY_PROTOCOL_TLV_SSL_சாவி_ALG:
        cert = SSL_get_certificate(c->ssl->connection);

        pkey = X509_get_pubkey(cert);
        எனில் (pkey == வெற்று) {
            திரும்பு NGX_ERROR;
        }

        தேர்வு (EVP_PKEY_base_id(pkey)) {

        நிலை EVP_PKEY_RSA:
#ifdef EVP_PKEY_RSA_PSS
        நிலை EVP_PKEY_RSA_PSS:
#endif
            alg = (u_char *) "RSA";
            நிறுத்து;

        நிலை EVP_PKEY_EC:
            alg = (u_char *) "EC";
            நிறுத்து;

        நிலை EVP_PKEY_DSA:
            alg = (u_char *) "DSA";
            நிறுத்து;

        இயல்பு:
            EVP_PKEY_free(pkey);
            திரும்பு NGX_DECLINED;
        }

        out->len = அளவு("RSA16384") - 1;
        out->data = ngx_pnalloc(c->pool, out->len);
        எனில் (out->data == வெற்று) {
            EVP_PKEY_free(pkey);
            திரும்பு NGX_ERROR;
        }

        out->len = ngx_snprintf(out->data, out->len, "%s%d",
                                alg, EVP_PKEY_bits(pkey))
                   - out->data;

        EVP_PKEY_free(pkey);

        திரும்பு NGX_OK;
    }

    திரும்பு NGX_DECLINED;
}

#endif


நிலையான u_char *
ngx_proxy_protocol_v2_write_header(ngx_connection_t *c, u_char *buf,
    u_char *last)
{
    u_char                            *p, src_af, dst_af,
                                       family, command, transport;
    ngx_uint_t                         src_fam, dst_fam;
    ngx_proxy_protocol_header_t       *header;
    ngx_proxy_protocol_inet_addrs_t   *in;
#எனில் (NGX_HAVE_INET6)
    ngx_uint_t                         port, lport;
    கட்டமைப்பு in6_addr                   *src6, *dst6;
    ngx_proxy_protocol_inet6_addrs_t  *in6;
#endif

    எனில் (last - buf < NGX_PROXY_PROTOCOL_V2_MAX_HEADER) {
        ngx_log_error(NGX_LOG_ALERT, c->log, 0,
                      "too small buffer for PROXY protocol v2");
        திரும்பு வெற்று;
    }

    எனில் (ngx_connection_local_sockaddr(c, வெற்று, 0) != NGX_OK) {
        திரும்பு வெற்று;
    }

    /* the length field is set by the caller once all TLVs are written */

    header = (ngx_proxy_protocol_header_t *) buf;

    ngx_memcpy(header->signature, ngx_proxy_protocol_signature,
               அளவு(ngx_proxy_protocol_signature) - 1);

    p = buf + அளவு(ngx_proxy_protocol_header_t);

    src_fam = c->sockaddr->sa_family;
    dst_fam = c->local_sockaddr->sa_family;

    src_af = ngx_proxy_protocol_v2_family(src_fam);
    dst_af = ngx_proxy_protocol_v2_family(dst_fam);

    /* promote to the highest address family present on either side */

    எனில் (src_af == NGX_PROXY_PROTOCOL_AF_UNSPEC
        || dst_af == NGX_PROXY_PROTOCOL_AF_UNSPEC)
    {
        command = NGX_PROXY_PROTOCOL_CMD_LOCAL;
        family = NGX_PROXY_PROTOCOL_AF_UNSPEC;
        transport = NGX_PROXY_PROTOCOL_TYPE_UNSPEC;

    } இல்லையெனில் {
        command = NGX_PROXY_PROTOCOL_CMD_PROXY;
        family = ngx_max(src_af, dst_af);

        தேர்வு (c->type) {
        நிலை SOCK_STREAM:
            transport = NGX_PROXY_PROTOCOL_TYPE_STREAM;
            நிறுத்து;
        நிலை SOCK_DGRAM:
            transport = NGX_PROXY_PROTOCOL_TYPE_DGRAM;
            நிறுத்து;
        இயல்பு:
            transport = NGX_PROXY_PROTOCOL_TYPE_UNSPEC;
            நிறுத்து;
        }
    }

    header->version_command = 0x20 | command;
    header->family_transport = (family << 4) | transport;

    தேர்வு (family) {

    நிலை NGX_PROXY_PROTOCOL_AF_INET:

        in = (ngx_proxy_protocol_inet_addrs_t *) p;

        ngx_proxy_protocol_v2_write_ipv4(c->sockaddr,
                                         in->src_addr, in->src_port);
        ngx_proxy_protocol_v2_write_ipv4(c->local_sockaddr,
                                         in->dst_addr, in->dst_port);

        p += அளவு(ngx_proxy_protocol_inet_addrs_t);

        நிறுத்து;

#எனில் (NGX_HAVE_INET6)

    நிலை NGX_PROXY_PROTOCOL_AF_INET6:

        எனில் (src_fam == AF_INET6 && dst_fam == AF_INET6) {
            src6 = &((கட்டமைப்பு sockaddr_in6 *) c->sockaddr)->sin6_addr;
            dst6 = &((கட்டமைப்பு sockaddr_in6 *) c->local_sockaddr)->sin6_addr;

            எனில் (IN6_IS_ADDR_V4MAPPED(src6) && IN6_IS_ADDR_V4MAPPED(dst6)) {

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

                p += அளவு(ngx_proxy_protocol_inet_addrs_t);
                நிறுத்து;
            }
        }

        in6 = (ngx_proxy_protocol_inet6_addrs_t *) p;

        ngx_proxy_protocol_v2_write_ipv6(c->sockaddr,
                                         in6->src_addr, in6->src_port);
        ngx_proxy_protocol_v2_write_ipv6(c->local_sockaddr,
                                         in6->dst_addr, in6->dst_port);

        p += அளவு(ngx_proxy_protocol_inet6_addrs_t);

        நிறுத்து;

#endif

    இயல்பு:  /* NGX_PROXY_PROTOCOL_AF_UNSPEC */
        நிறுத்து;
    }

    திரும்பு p;
}


நிலையான u_char
ngx_proxy_protocol_v2_family(ngx_uint_t family)
{
    தேர்வு (family) {

    நிலை AF_INET:
        திரும்பு NGX_PROXY_PROTOCOL_AF_INET;

#எனில் (NGX_HAVE_INET6)
    நிலை AF_INET6:
        திரும்பு NGX_PROXY_PROTOCOL_AF_INET6;
#endif

    இயல்பு:
        திரும்பு NGX_PROXY_PROTOCOL_AF_UNSPEC;
    }
}


நிலையான வெறுமை
ngx_proxy_protocol_v2_write_ipv4(கட்டமைப்பு sockaddr *sa, u_char *addr,
    u_char *port)
{
    கட்டமைப்பு sockaddr_in  *sin;

    sin = (கட்டமைப்பு sockaddr_in *) sa;
    ngx_memcpy(addr, &sin->sin_addr, 4);
    ngx_memcpy(port, &sin->sin_port, 2);
}


#எனில் (NGX_HAVE_INET6)

நிலையான வெறுமை
ngx_proxy_protocol_v2_write_ipv6(கட்டமைப்பு sockaddr *sa, u_char *addr,
    u_char *port)
{
    கட்டமைப்பு sockaddr_in   *sin;
    கட்டமைப்பு sockaddr_in6  *sin6;

    /* IPv4-mapped prefix: 80 zero bits + 16 one bits */
    நிலையான மாறிலி u_char  mapped[]   = { 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0xff, 0xff };

    எனில் (sa->sa_family == AF_INET6) {
        sin6 = (கட்டமைப்பு sockaddr_in6 *) sa;
        ngx_memcpy(addr, &sin6->sin6_addr, 16);
        ngx_memcpy(port, &sin6->sin6_port, 2);

    } இல்லையெனில் {
        /* promote AF_INET to ::ffff:a.b.c.d */
        sin = (கட்டமைப்பு sockaddr_in *) sa;
        ngx_memcpy(addr, mapped, அளவு(mapped));
        ngx_memcpy(addr + அளவு(mapped), &sin->sin_addr, 4);
        ngx_memcpy(port, &sin->sin_port, 2);
    }
}

#endif


நிலையான u_char *
ngx_proxy_protocol_v2_write_tlv(ngx_connection_t *c, u_char *p, u_char *last,
    ngx_uint_t type, ngx_str_t *மதிப்பு)
{
    ngx_proxy_protocol_tlv_t  *tlv;

    எனில் ((size_t) (last - p) < அளவு(ngx_proxy_protocol_tlv_t) + மதிப்பு->len) {
        ngx_log_error(NGX_LOG_ALERT, c->log, 0,
                      "too small buffer for PROXY protocol v2 TLV");
        திரும்பு வெற்று;
    }

    tlv = (ngx_proxy_protocol_tlv_t *) p;
    tlv->type = (u_char) type;
    tlv->len[0] = (u_char) (மதிப்பு->len >> 8);
    tlv->len[1] = (u_char)  மதிப்பு->len;

    p += அளவு(ngx_proxy_protocol_tlv_t);

    திரும்பு ngx_cpymem(p, மதிப்பு->data, மதிப்பு->len);
}


நிலையான u_char *
ngx_proxy_protocol_v2_write_ssl(ngx_connection_t *c, u_char *p, u_char *last,
    ngx_array_t *ssl_tlvs, ngx_uint_t client, uint32_t verify)
{
    u_char                          *start;
    uint16_t                         len;
    ngx_uint_t                       i;
    ngx_proxy_protocol_tlv_t        *tlv;
    ngx_proxy_protocol_tlv_ssl_t    *tlv_ssl;
    ngx_proxy_protocol_tlv_மதிப்பு_t  *sub;

    எனில் ((size_t) (last - p) < அளவு(ngx_proxy_protocol_tlv_t)
                              + அளவு(ngx_proxy_protocol_tlv_ssl_t))
    {
        ngx_log_error(NGX_LOG_ALERT, c->log, 0,
                      "too small buffer for PROXY protocol v2 SSL TLV");
        திரும்பு வெற்று;
    }

    start = p;
    p += அளவு(ngx_proxy_protocol_tlv_t);

    tlv_ssl = (ngx_proxy_protocol_tlv_ssl_t *) p;
    tlv_ssl->client = (u_char) client;
    tlv_ssl->verify[0] = (u_char) (verify >> 24);
    tlv_ssl->verify[1] = (u_char) (verify >> 16);
    tlv_ssl->verify[2] = (u_char) (verify >> 8);
    tlv_ssl->verify[3] = (u_char)  verify;

    p += அளவு(ngx_proxy_protocol_tlv_ssl_t);

    sub = ssl_tlvs->elts;

    க்கு (i = 0; i < ssl_tlvs->nelts; i++) {
        p = ngx_proxy_protocol_v2_write_tlv(c, p, last, sub[i].type,
                                            &sub[i].மதிப்பு);
        எனில் (p == வெற்று) {
            திரும்பு வெற்று;
        }
    }

    len = (uint16_t) (p - start - அளவு(ngx_proxy_protocol_tlv_t));

    tlv = (ngx_proxy_protocol_tlv_t *) start;
    tlv->type = NGX_PROXY_PROTOCOL_TLV_SSL;
    tlv->len[0] = (u_char) (len >> 8);
    tlv->len[1] = (u_char)  len;

    திரும்பு p;
}


நிலையான வெறுமை
ngx_proxy_protocol_v2_set_len(u_char *buf, u_char *p)
{
    uint16_t                      len;
    ngx_proxy_protocol_header_t  *header;

    len = (uint16_t) (p - buf - அளவு(ngx_proxy_protocol_header_t));

    header = (ngx_proxy_protocol_header_t *) buf;
    header->len[0] = (u_char) (len >> 8);
    header->len[1] = (u_char) len;
}


நிலையான u_char *
ngx_proxy_protocol_v2_write_crc32c(ngx_connection_t *c, u_char *buf,
    u_char *p, u_char *last)
{
    u_char                    *மதிப்பு;
    uint32_t                   crc;
    ngx_proxy_protocol_tlv_t  *tlv;

    எனில் ((size_t) (last - p) < அளவு(ngx_proxy_protocol_tlv_t) + 4) {
        ngx_log_error(NGX_LOG_ALERT, c->log, 0,
                      "too small buffer for PROXY protocol v2 CRC32c TLV");
        திரும்பு வெற்று;
    }

    /* append the CRC32c TLV with a zeroed value field */

    tlv = (ngx_proxy_protocol_tlv_t *) p;
    tlv->type = NGX_PROXY_PROTOCOL_TLV_CRC32C;
    tlv->len[0] = 0;
    tlv->len[1] = 4;

    மதிப்பு = p + அளவு(ngx_proxy_protocol_tlv_t);

    ngx_memzero(மதிப்பு, 4);

    p = மதிப்பு + 4;

    /* the checksum covers the entire header with the zeroed value */

    crc = ngx_crc32c_long(buf, p - buf);

    மதிப்பு[0] = (u_char) (crc >> 24);
    மதிப்பு[1] = (u_char) (crc >> 16);
    மதிப்பு[2] = (u_char) (crc >> 8);
    மதிப்பு[3] = (u_char) crc;

    திரும்பு p;
}
