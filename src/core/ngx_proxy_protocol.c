
/*
 * Copyright (C) Roman Arutyunyan
 * Copyright (C) Nginx, Inc.
 */


#ଅନ୍ତର୍ଭାଵ <ngx_config.h>
#ଅନ୍ତର୍ଭାଵ <ngx_core.h>


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
#define NGX_PROXY_PROTOCOL_TLV_SSL_ଚାବି_ALG  0x25
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


ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    u_char                                  signature[12];
    u_char                                  version_command;
    u_char                                  family_transport;
    u_char                                  len[2];
} ngx_proxy_protocol_header_t;


ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    u_char                                  src_addr[4];
    u_char                                  dst_addr[4];
    u_char                                  src_port[2];
    u_char                                  dst_port[2];
} ngx_proxy_protocol_inet_addrs_t;


ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    u_char                                  src_addr[16];
    u_char                                  dst_addr[16];
    u_char                                  src_port[2];
    u_char                                  dst_port[2];
} ngx_proxy_protocol_inet6_addrs_t;


ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    u_char                                  type;
    u_char                                  len[2];
} ngx_proxy_protocol_tlv_t;


ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    u_char                                  client;
    u_char                                  verify[4];
} ngx_proxy_protocol_tlv_ssl_t;


ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ngx_str_t                               name;
    ngx_uint_t                              type;
} ngx_proxy_protocol_tlv_entry_t;


ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ngx_uint_t                              type;
    ngx_str_t                               ମୂଲ୍ୟ;
} ngx_proxy_protocol_tlv_ମୂଲ୍ୟ_t;


ସ୍ଥିର u_char *ngx_proxy_protocol_read_addr(ngx_connection_t *c, u_char *p,
    u_char *last, ngx_str_t *addr);
ସ୍ଥିର u_char *ngx_proxy_protocol_read_port(u_char *p, u_char *last,
    in_port_t *port, u_char sep);
ସ୍ଥିର u_char *ngx_proxy_protocol_v2_read(ngx_connection_t *c, u_char *buf,
    u_char *last);
ସ୍ଥିର ngx_int_t ngx_proxy_protocol_lookup_tlv(ngx_connection_t *c,
    ngx_str_t *tlvs, ngx_uint_t type, ngx_str_t *ମୂଲ୍ୟ);
#ଯଦି (NGX_OPENSSL)
ସ୍ଥିର ngx_int_t ngx_proxy_protocol_v2_eval_ssl(ngx_connection_t *c,
    ngx_array_t *tlvs, ngx_array_t *ssl_tlvs, ngx_uint_t *client,
    uint32_t *verify);
ସ୍ଥିର ngx_int_t ngx_proxy_protocol_v2_authority(ngx_connection_t *c,
    ngx_str_t *out);
ସ୍ଥିର ngx_int_t ngx_proxy_protocol_v2_alpn(ngx_connection_t *c,
    ngx_str_t *out);
ସ୍ଥିର ngx_int_t ngx_proxy_protocol_v2_ssl_sub(ngx_connection_t *c,
    ngx_uint_t type, ngx_str_t *out);
#endif
ସ୍ଥିର u_char *ngx_proxy_protocol_v2_write_header(ngx_connection_t *c,
    u_char *buf, u_char *last);
ସ୍ଥିର u_char ngx_proxy_protocol_v2_family(ngx_uint_t family);
ସ୍ଥିର ଶୂନ୍ୟ ngx_proxy_protocol_v2_write_ipv4(ସଂରଚନା sockaddr *sa, u_char *addr,
    u_char *port);
ସ୍ଥିର ଶୂନ୍ୟ ngx_proxy_protocol_v2_write_ipv6(ସଂରଚନା sockaddr *sa, u_char *addr,
    u_char *port);
ସ୍ଥିର u_char *ngx_proxy_protocol_v2_write_tlv(ngx_connection_t *c, u_char *p,
    u_char *last, ngx_uint_t type, ngx_str_t *ମୂଲ୍ୟ);
ସ୍ଥିର u_char *ngx_proxy_protocol_v2_write_ssl(ngx_connection_t *c, u_char *p,
    u_char *last, ngx_array_t *ssl_tlvs, ngx_uint_t client, uint32_t verify);
ସ୍ଥିର ଶୂନ୍ୟ ngx_proxy_protocol_v2_set_len(u_char *buf, u_char *p);
ସ୍ଥିର u_char *ngx_proxy_protocol_v2_write_crc32c(ngx_connection_t *c,
    u_char *buf, u_char *p, u_char *last);


ସ୍ଥିର ngx_proxy_protocol_tlv_entry_t  ngx_proxy_protocol_tlv_entries[] = {
    { ngx_string("alpn"),       NGX_PROXY_PROTOCOL_TLV_ALPN },
    { ngx_string("authority"),  NGX_PROXY_PROTOCOL_TLV_AUTHORITY },
    { ngx_string("unique_id"),  NGX_PROXY_PROTOCOL_TLV_UNIQUE_ID },
    { ngx_string("ssl"),        NGX_PROXY_PROTOCOL_TLV_SSL },
    { ngx_string("netns"),      NGX_PROXY_PROTOCOL_TLV_NETNS },
    { ngx_null_string,          0x00 }
};


ସ୍ଥିର ngx_proxy_protocol_tlv_entry_t  ngx_proxy_protocol_tlv_ssl_entries[] = {
    { ngx_string("version"),    NGX_PROXY_PROTOCOL_TLV_SSL_VERSION },
    { ngx_string("cn"),         NGX_PROXY_PROTOCOL_TLV_SSL_CN },
    { ngx_string("cipher"),     NGX_PROXY_PROTOCOL_TLV_SSL_CIPHER },
    { ngx_string("sig_alg"),    NGX_PROXY_PROTOCOL_TLV_SSL_SIG_ALG },
    { ngx_string("key_alg"),    NGX_PROXY_PROTOCOL_TLV_SSL_ଚାବି_ALG },
    { ngx_null_string,          0x00 }
};


ସ୍ଥିର ଅଚଳ u_char  ngx_proxy_protocol_signature[] = "\r\n\r\n\0\r\nQUIT\n";


u_char *
ngx_proxy_protocol_read(ngx_connection_t *c, u_char *buf, u_char *last)
{
    size_t                 len;
    u_char                *p;
    ngx_proxy_protocol_t  *pp;

    p = buf;
    len = last - buf;

    ଯଦି (len >= ଆକାର(ngx_proxy_protocol_header_t)
        && ngx_memcmp(p, ngx_proxy_protocol_signature,
                      ଆକାର(ngx_proxy_protocol_signature) - 1)
           == 0)
    {
        ଫେରନ୍ତୁ ngx_proxy_protocol_v2_read(c, buf, last);
    }

    ଯଦି (len < 8 || ngx_strncmp(p, "PROXY ", 6) != 0) {
        ଯାଅ invalid;
    }

    p += 6;
    len -= 6;

    ଯଦି (len >= 7 && ngx_strncmp(p, "UNKNOWN", 7) == 0) {
        ngx_log_debug0(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "PROXY protocol unknown protocol");
        p += 7;
        ଯାଅ skip;
    }

    ଯଦି (len < 5 || ngx_strncmp(p, "TCP", 3) != 0
        || (p[3] != '4' && p[3] != '6') || p[4] != ' ')
    {
        ଯାଅ invalid;
    }

    p += 5;

    pp = ngx_pcalloc(c->pool, ଆକାର(ngx_proxy_protocol_t));
    ଯଦି (pp == ଖାଲି) {
        ଫେରନ୍ତୁ ଖାଲି;
    }

    p = ngx_proxy_protocol_read_addr(c, p, last, &pp->src_addr);
    ଯଦି (p == ଖାଲି) {
        ଯାଅ invalid;
    }

    p = ngx_proxy_protocol_read_addr(c, p, last, &pp->dst_addr);
    ଯଦି (p == ଖାଲି) {
        ଯାଅ invalid;
    }

    p = ngx_proxy_protocol_read_port(p, last, &pp->src_port, ' ');
    ଯଦି (p == ଖାଲି) {
        ଯାଅ invalid;
    }

    p = ngx_proxy_protocol_read_port(p, last, &pp->dst_port, CR);
    ଯଦି (p == ଖାଲି) {
        ଯାଅ invalid;
    }

    ଯଦି (p == last) {
        ଯାଅ invalid;
    }

    ଯଦି (*p++ != LF) {
        ଯାଅ invalid;
    }

    ngx_log_debug4(NGX_LOG_DEBUG_CORE, c->log, 0,
                   "PROXY protocol src: %V %d, dst: %V %d",
                   &pp->src_addr, pp->src_port, &pp->dst_addr, pp->dst_port);

    c->proxy_protocol = pp;

    ଫେରନ୍ତୁ p;

skip:

    ପାଇଁ ( /* void */ ; p < last - 1; p++) {
        ଯଦି (p[0] == CR && p[1] == LF) {
            ଫେରନ୍ତୁ p + 2;
        }
    }

invalid:

    ପାଇଁ (p = buf; p < last; p++) {
        ଯଦି (*p == CR || *p == LF) {
            ଭାଙ୍ଗ;
        }
    }

    ngx_log_error(NGX_LOG_ERR, c->log, 0,
                  "broken header: \"%*s\"", (size_t) (p - buf), buf);

    ଫେରନ୍ତୁ ଖାଲି;
}


ସ୍ଥିର u_char *
ngx_proxy_protocol_read_addr(ngx_connection_t *c, u_char *p, u_char *last,
    ngx_str_t *addr)
{
    size_t  len;
    u_char  ch, *pos;

    pos = p;

    ପାଇଁ ( ;; ) {
        ଯଦି (p == last) {
            ଫେରନ୍ତୁ ଖାଲି;
        }

        ch = *p++;

        ଯଦି (ch == ' ') {
            ଭାଙ୍ଗ;
        }

        ଯଦି (ch != ':' && ch != '.'
            && (ch < 'a' || ch > 'f')
            && (ch < 'A' || ch > 'F')
            && (ch < '0' || ch > '9'))
        {
            ଫେରନ୍ତୁ ଖାଲି;
        }
    }

    len = p - pos - 1;

    addr->data = ngx_pnalloc(c->pool, len);
    ଯଦି (addr->data == ଖାଲି) {
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ngx_memcpy(addr->data, pos, len);
    addr->len = len;

    ଫେରନ୍ତୁ p;
}


ସ୍ଥିର u_char *
ngx_proxy_protocol_read_port(u_char *p, u_char *last, in_port_t *port,
    u_char sep)
{
    size_t      len;
    u_char     *pos;
    ngx_int_t   n;

    pos = p;

    ପାଇଁ ( ;; ) {
        ଯଦି (p == last) {
            ଫେରନ୍ତୁ ଖାଲି;
        }

        ଯଦି (*p++ == sep) {
            ଭାଙ୍ଗ;
        }
    }

    len = p - pos - 1;

    n = ngx_atoi(pos, len);
    ଯଦି (n < 0 || n > 65535) {
        ଫେରନ୍ତୁ ଖାଲି;
    }

    *port = (in_port_t) n;

    ଫେରନ୍ତୁ p;
}


u_char *
ngx_proxy_protocol_write(ngx_connection_t *c, u_char *buf, u_char *last)
{
    ngx_uint_t  port, lport;

    ଯଦି (last - buf < NGX_PROXY_PROTOCOL_V1_MAX_HEADER) {
        ngx_log_error(NGX_LOG_ALERT, c->log, 0,
                      "too small buffer for PROXY protocol");
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ଯଦି (ngx_connection_local_sockaddr(c, ଖାଲି, 0) != NGX_OK) {
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ବାଛ (c->sockaddr->sa_family) {

    ସ୍ଥିତି AF_INET:
        buf = ngx_cpymem(buf, "PROXY TCP4 ", ଆକାର("PROXY TCP4 ") - 1);
        ଭାଙ୍ଗ;

#ଯଦି (NGX_HAVE_INET6)
    ସ୍ଥିତି AF_INET6:
        buf = ngx_cpymem(buf, "PROXY TCP6 ", ଆକାର("PROXY TCP6 ") - 1);
        ଭାଙ୍ଗ;
#endif

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        ଫେରନ୍ତୁ ngx_cpymem(buf, "PROXY UNKNOWN" CRLF,
                          ଆକାର("PROXY UNKNOWN" CRLF) - 1);
    }

    buf += ngx_sock_ntop(c->sockaddr, c->socklen, buf, last - buf, 0);

    *buf++ = ' ';

    buf += ngx_sock_ntop(c->local_sockaddr, c->local_socklen, buf, last - buf,
                         0);

    port = ngx_inet_get_port(c->sockaddr);
    lport = ngx_inet_get_port(c->local_sockaddr);

    ଫେରନ୍ତୁ ngx_slprintf(buf, last, " %ui %ui" CRLF, port, lport);
}


ସ୍ଥିର u_char *
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
#ଯଦି (NGX_HAVE_INET6)
    ngx_proxy_protocol_inet6_addrs_t   *in6;
#endif

    header = (ngx_proxy_protocol_header_t *) buf;

    buf += ଆକାର(ngx_proxy_protocol_header_t);

    version = header->version_command >> 4;

    ଯଦି (version != 2) {
        ngx_log_error(NGX_LOG_ERR, c->log, 0,
                      "unknown PROXY protocol version: %ui", version);
        ଫେରନ୍ତୁ ଖାଲି;
    }

    len = ngx_proxy_protocol_parse_uint16(header->len);

    ଯଦି ((size_t) (last - buf) < len) {
        ngx_log_error(NGX_LOG_ERR, c->log, 0, "header is too large");
        ଫେରନ୍ତୁ ଖାଲି;
    }

    end = buf + len;

    command = header->version_command & 0x0f;

    /* only PROXY is supported */
    ଯଦି (command != NGX_PROXY_PROTOCOL_CMD_PROXY) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "PROXY protocol v2 unsupported command %ui", command);
        ଫେରନ୍ତୁ end;
    }

    transport = header->family_transport & 0x0f;

    /* only STREAM is supported */
    ଯଦି (transport != NGX_PROXY_PROTOCOL_TYPE_STREAM) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "PROXY protocol v2 unsupported transport %ui",
                       transport);
        ଫେରନ୍ତୁ end;
    }

    pp = ngx_pcalloc(c->pool, ଆକାର(ngx_proxy_protocol_t));
    ଯଦି (pp == ଖାଲି) {
        ଫେରନ୍ତୁ ଖାଲି;
    }

    family = header->family_transport >> 4;

    ବାଛ (family) {

    ସ୍ଥିତି NGX_PROXY_PROTOCOL_AF_INET:

        ଯଦି ((size_t) (end - buf) < ଆକାର(ngx_proxy_protocol_inet_addrs_t)) {
            ଫେରନ୍ତୁ ଖାଲି;
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

        socklen = ଆକାର(ସଂରଚନା sockaddr_in);

        buf += ଆକାର(ngx_proxy_protocol_inet_addrs_t);

        ଭାଙ୍ଗ;

#ଯଦି (NGX_HAVE_INET6)

    ସ୍ଥିତି NGX_PROXY_PROTOCOL_AF_INET6:

        ଯଦି ((size_t) (end - buf) < ଆକାର(ngx_proxy_protocol_inet6_addrs_t)) {
            ଫେରନ୍ତୁ ଖାଲି;
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

        socklen = ଆକାର(ସଂରଚନା sockaddr_in6);

        buf += ଆକାର(ngx_proxy_protocol_inet6_addrs_t);

        ଭାଙ୍ଗ;

#endif

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "PROXY protocol v2 unsupported address family %ui",
                       family);
        ଫେରନ୍ତୁ end;
    }

    pp->src_addr.data = ngx_pnalloc(c->pool, NGX_SOCKADDR_STRLEN);
    ଯଦି (pp->src_addr.data == ଖାଲି) {
        ଫେରନ୍ତୁ ଖାଲି;
    }

    pp->src_addr.len = ngx_sock_ntop(&src_sockaddr.sockaddr, socklen,
                                     pp->src_addr.data, NGX_SOCKADDR_STRLEN, 0);

    pp->dst_addr.data = ngx_pnalloc(c->pool, NGX_SOCKADDR_STRLEN);
    ଯଦି (pp->dst_addr.data == ଖାଲି) {
        ଫେରନ୍ତୁ ଖାଲି;
    }

    pp->dst_addr.len = ngx_sock_ntop(&dst_sockaddr.sockaddr, socklen,
                                     pp->dst_addr.data, NGX_SOCKADDR_STRLEN, 0);

    ngx_log_debug4(NGX_LOG_DEBUG_CORE, c->log, 0,
                   "PROXY protocol v2 src: %V %d, dst: %V %d",
                   &pp->src_addr, pp->src_port, &pp->dst_addr, pp->dst_port);

    ଯଦି (buf < end) {
        pp->tlvs.data = ngx_pnalloc(c->pool, end - buf);
        ଯଦି (pp->tlvs.data == ଖାଲି) {
            ଫେରନ୍ତୁ ଖାଲି;
        }

        ngx_memcpy(pp->tlvs.data, buf, end - buf);
        pp->tlvs.len = end - buf;
    }

    c->proxy_protocol = pp;

    ଫେରନ୍ତୁ end;
}


ngx_int_t
ngx_proxy_protocol_get_tlv(ngx_connection_t *c, ngx_str_t *name,
    ngx_str_t *ମୂଲ୍ୟ)
{
    u_char                          *p;
    size_t                           n;
    uint32_t                         verify;
    ngx_str_t                        ssl, *tlvs;
    ngx_int_t                        rc, type;
    ngx_proxy_protocol_tlv_ssl_t    *tlv_ssl;
    ngx_proxy_protocol_tlv_entry_t  *te;

    ଯଦି (c->proxy_protocol == ଖାଲି) {
        ଫେରନ୍ତୁ NGX_DECLINED;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                   "PROXY protocol v2 get tlv \"%V\"", name);

    te = ngx_proxy_protocol_tlv_entries;
    tlvs = &c->proxy_protocol->tlvs;

    p = name->data;
    n = name->len;

    ଯଦି (n >= 4 && p[0] == 's' && p[1] == 's' && p[2] == 'l' && p[3] == '_') {

        rc = ngx_proxy_protocol_lookup_tlv(c, tlvs,
                                           NGX_PROXY_PROTOCOL_TLV_SSL, &ssl);
        ଯଦି (rc != NGX_OK) {
            ଫେରନ୍ତୁ rc;
        }

        ଯଦି (ssl.len < ଆକାର(ngx_proxy_protocol_tlv_ssl_t)) {
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        p += 4;
        n -= 4;

        ଯଦି (n == 6 && ngx_strncmp(p, "verify", 6) == 0) {

            tlv_ssl = (ngx_proxy_protocol_tlv_ssl_t *) ssl.data;
            verify = ngx_proxy_protocol_parse_uint32(tlv_ssl->verify);

            ମୂଲ୍ୟ->data = ngx_pnalloc(c->pool, NGX_INT32_LEN);
            ଯଦି (ମୂଲ୍ୟ->data == ଖାଲି) {
                ଫେରନ୍ତୁ NGX_ERROR;
            }

            ମୂଲ୍ୟ->len = ngx_sprintf(ମୂଲ୍ୟ->data, "%uD", verify)
                         - ମୂଲ୍ୟ->data;
            ଫେରନ୍ତୁ NGX_OK;
        }

        ssl.data += ଆକାର(ngx_proxy_protocol_tlv_ssl_t);
        ssl.len -= ଆକାର(ngx_proxy_protocol_tlv_ssl_t);

        te = ngx_proxy_protocol_tlv_ssl_entries;
        tlvs = &ssl;
    }

    ଯଦି (n >= 2 && p[0] == '0' && p[1] == 'x') {

        type = ngx_hextoi(p + 2, n - 2);
        ଯଦି (type == NGX_ERROR) {
            ngx_log_error(NGX_LOG_ERR, c->log, 0,
                          "invalid PROXY protocol TLV \"%V\"", name);
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        ଫେରନ୍ତୁ ngx_proxy_protocol_lookup_tlv(c, tlvs, type, ମୂଲ୍ୟ);
    }

    ପାଇଁ ( /* void */ ; te->type; te++) {
        ଯଦି (te->name.len == n && ngx_strncmp(te->name.data, p, n) == 0) {
            ଫେରନ୍ତୁ ngx_proxy_protocol_lookup_tlv(c, tlvs, te->type, ମୂଲ୍ୟ);
        }
    }

    ngx_log_error(NGX_LOG_ERR, c->log, 0,
                  "unknown PROXY protocol TLV \"%V\"", name);

    ଫେରନ୍ତୁ NGX_DECLINED;
}


ସ୍ଥିର ngx_int_t
ngx_proxy_protocol_lookup_tlv(ngx_connection_t *c, ngx_str_t *tlvs,
    ngx_uint_t type, ngx_str_t *ମୂଲ୍ୟ)
{
    u_char                    *p;
    size_t                     n, len;
    ngx_proxy_protocol_tlv_t  *tlv;

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                   "PROXY protocol v2 lookup tlv:%02xi", type);

    p = tlvs->data;
    n = tlvs->len;

    ଯେତେବେଳେ (n) {
        ଯଦି (n < ଆକାର(ngx_proxy_protocol_tlv_t)) {
            ngx_log_error(NGX_LOG_ERR, c->log, 0, "broken PROXY protocol TLV");
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        tlv = (ngx_proxy_protocol_tlv_t *) p;
        len = ngx_proxy_protocol_parse_uint16(tlv->len);

        p += ଆକାର(ngx_proxy_protocol_tlv_t);
        n -= ଆକାର(ngx_proxy_protocol_tlv_t);

        ଯଦି (n < len) {
            ngx_log_error(NGX_LOG_ERR, c->log, 0, "broken PROXY protocol TLV");
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        ଯଦି (tlv->type == type) {
            ମୂଲ୍ୟ->data = p;
            ମୂଲ୍ୟ->len = len;
            ଫେରନ୍ତୁ NGX_OK;
        }

        p += len;
        n -= len;
    }

    ଫେରନ୍ତୁ NGX_DECLINED;
}


u_char *
ngx_proxy_protocol_v2_write(ngx_connection_t *c, u_char *buf, u_char *last,
    ngx_array_t *tlvs /* reserved */ )
{
    u_char                          *p;
    uint32_t                         verify;
    ngx_uint_t                       i, client;
    ngx_array_t                      base_tlvs, ssl_tlvs;
    ngx_proxy_protocol_tlv_ମୂଲ୍ୟ_t  *tlv;

    client = 0;
    verify = 0;

    ngx_memzero(&base_tlvs, ଆକାର(ngx_array_t));
    ngx_memzero(&ssl_tlvs, ଆକାର(ngx_array_t));

#ଯଦି (NGX_OPENSSL)

    ଯଦି (c->ssl != ଖାଲି) {
        ଯଦି (ngx_array_init(&base_tlvs, c->pool, 2,
                           ଆକାର(ngx_proxy_protocol_tlv_ମୂଲ୍ୟ_t))
            != NGX_OK)
        {
            ଫେରନ୍ତୁ ଖାଲି;
        }

        ଯଦି (ngx_array_init(&ssl_tlvs, c->pool, 5,
                           ଆକାର(ngx_proxy_protocol_tlv_ମୂଲ୍ୟ_t))
            != NGX_OK)
        {
            ଫେରନ୍ତୁ ଖାଲି;
        }

        ଯଦି (ngx_proxy_protocol_v2_eval_ssl(c, &base_tlvs, &ssl_tlvs, &client,
                                           &verify)
            != NGX_OK)
        {
            ଫେରନ୍ତୁ ଖାଲି;
        }
    }

#endif

    p = ngx_proxy_protocol_v2_write_header(c, buf, last);
    ଯଦି (p == ଖାଲି) {
        ଫେରନ୍ତୁ ଖାଲି;
    }

    tlv = base_tlvs.elts;

    ପାଇଁ (i = 0; i < base_tlvs.nelts; i++) {
        p = ngx_proxy_protocol_v2_write_tlv(c, p, last, tlv[i].type,
                                           &tlv[i].ମୂଲ୍ୟ);
        ଯଦି (p == ଖାଲି) {
            ଫେରନ୍ତୁ ଖାଲି;
        }
    }

    ଯଦି (client) {
        p = ngx_proxy_protocol_v2_write_ssl(c, p, last, &ssl_tlvs, client,
                                            verify);
        ଯଦି (p == ଖାଲି) {
            ଫେରନ୍ତୁ ଖାଲି;
        }
    }

    ngx_proxy_protocol_v2_set_len(buf,
                                  p + ଆକାର(ngx_proxy_protocol_tlv_t) + 4);

    ଫେରନ୍ତୁ ngx_proxy_protocol_v2_write_crc32c(c, buf, p, last);
}


#ଯଦି (NGX_OPENSSL)

ସ୍ଥିର ngx_int_t
ngx_proxy_protocol_v2_eval_ssl(ngx_connection_t *c, ngx_array_t *tlvs,
    ngx_array_t *ssl_tlvs, ngx_uint_t *client, uint32_t *verify)
{
    X509                            *cert;
    long                             n;
    ngx_int_t                        rc;
    ngx_str_t                        ମୂଲ୍ୟ;
    ngx_uint_t                       type;
    ଅଚଳ ଅକ୍ଷର                      *s;
    ngx_proxy_protocol_tlv_ମୂଲ୍ୟ_t  *tlv;

    ଯଦି (ngx_proxy_protocol_v2_authority(c, &ମୂଲ୍ୟ) == NGX_OK) {
        tlv = ngx_array_ଠେଲ(tlvs);
        ଯଦି (tlv == ଖାଲି) {
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        tlv->type = NGX_PROXY_PROTOCOL_TLV_AUTHORITY;
        tlv->ମୂଲ୍ୟ = ମୂଲ୍ୟ;
    }

    ଯଦି (ngx_proxy_protocol_v2_alpn(c, &ମୂଲ୍ୟ) == NGX_OK) {
        tlv = ngx_array_ଠେଲ(tlvs);
        ଯଦି (tlv == ଖାଲି) {
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        tlv->type = NGX_PROXY_PROTOCOL_TLV_ALPN;
        tlv->ମୂଲ୍ୟ = ମୂଲ୍ୟ;
    }

    ପାଇଁ (type = NGX_PROXY_PROTOCOL_TLV_SSL_VERSION;
         type <= NGX_PROXY_PROTOCOL_TLV_SSL_ଚାବି_ALG;
         type++)
    {
        rc = ngx_proxy_protocol_v2_ssl_sub(c, type, &ମୂଲ୍ୟ);

        ଯଦି (rc == NGX_ERROR) {
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        ଯଦି (rc != NGX_OK) {
            ଜାରି;
        }

        tlv = ngx_array_ଠେଲ(ssl_tlvs);
        ଯଦି (tlv == ଖାଲି) {
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        tlv->type = type;
        tlv->ମୂଲ୍ୟ = ମୂଲ୍ୟ;
    }

    *client = NGX_PROXY_PROTOCOL_V2_CLIENT_SSL;
    *verify = 1;  /* X509_V_ERR_UNSPECIFIED */

    cert = SSL_get_peer_certificate(c->ssl->connection);

    ଯଦି (cert != ଖାଲି) {
        X509_free(cert);

        *client |= NGX_PROXY_PROTOCOL_V2_CLIENT_CERT_SESS;

        ଯଦି (!SSL_session_reused(c->ssl->connection)) {
            *client |= NGX_PROXY_PROTOCOL_V2_CLIENT_CERT_CONN;
        }

        n = SSL_get_verify_result(c->ssl->connection);

        ଯଦି (n == X509_V_OK) {

            ଯଦି (ngx_ssl_ocsp_get_status(c, &s) != NGX_OK) {
                n = X509_V_ERR_CERT_REVOKED;
            }
        }

        *verify = (uint32_t) n;
    }

    ଫେରନ୍ତୁ NGX_OK;
}


ସ୍ଥିର ngx_int_t
ngx_proxy_protocol_v2_authority(ngx_connection_t *c, ngx_str_t *out)
{
#ifdef SSL_CTRL_SET_TLSEXT_HOSTNAME

    ଅଚଳ ଅକ୍ଷର  *sni;

    sni = SSL_get_servername(c->ssl->connection, TLSEXT_NAMETYPE_host_name);
    ଯଦି (sni == ଖାଲି) {
        ଫେରନ୍ତୁ NGX_DECLINED;
    }

    out->data = (u_char *) sni;
    out->len = ngx_strlen(sni);
    ଫେରନ୍ତୁ NGX_OK;

#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ NGX_DECLINED;
#endif
}


ସ୍ଥିର ngx_int_t
ngx_proxy_protocol_v2_alpn(ngx_connection_t *c, ngx_str_t *out)
{
#ifdef TLSEXT_TYPE_application_layer_protocol_negotiation

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା          alpnlen;
    ଅଚଳ unsigned ଅକ୍ଷର  *alpn;

    SSL_get0_alpn_selected(c->ssl->connection, &alpn, &alpnlen);
    ଯଦି (alpn == ଖାଲି || alpnlen == 0) {
        ଫେରନ୍ତୁ NGX_DECLINED;
    }

    out->data = (u_char *) alpn;
    out->len = alpnlen;
    ଫେରନ୍ତୁ NGX_OK;

#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ NGX_DECLINED;
#endif
}


ସ୍ଥିର ngx_int_t
ngx_proxy_protocol_v2_ssl_sub(ngx_connection_t *c, ngx_uint_t type,
    ngx_str_t *out)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା               i, len;
    X509             *cert;
    u_char           *alg, *s;
    EVP_PKEY         *pkey;
#ଯଦି (OPENSSL_VERSION_ସଂଖ୍ୟା >= 0x40000000L)
    ଅଚଳ
#endif
    X509_NAME        *subject;
#ଯଦି (OPENSSL_VERSION_ସଂଖ୍ୟା >= 0x40000000L)
    ଅଚଳ
#endif
    X509_NAME_ENTRY  *entry;

    ବାଛ (type) {

    ସ୍ଥିତି NGX_PROXY_PROTOCOL_TLV_SSL_VERSION:
        out->data = (u_char *) SSL_get_version(c->ssl->connection);
        out->len = ngx_strlen(out->data);
        ଫେରନ୍ତୁ NGX_OK;

    ସ୍ଥିତି NGX_PROXY_PROTOCOL_TLV_SSL_CIPHER:
        out->data = (u_char *) SSL_get_cipher_name(c->ssl->connection);
        out->len = ngx_strlen(out->data);
        ଫେରନ୍ତୁ NGX_OK;

    ସ୍ଥିତି NGX_PROXY_PROTOCOL_TLV_SSL_CN:
        cert = SSL_get_peer_certificate(c->ssl->connection);
        ଯଦି (cert == ଖାଲି) {
            ଫେରନ୍ତୁ NGX_DECLINED;
        }

        subject = X509_get_subject_name(cert);
        ଯଦି (subject == ଖାଲି) {
            X509_free(cert);
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        i = X509_NAME_get_index_by_NID(subject, NID_commonName, -1);
        ଯଦି (i < 0) {
            X509_free(cert);
            ଫେରନ୍ତୁ NGX_DECLINED;
        }

        entry = X509_NAME_get_entry(subject, i);

        len = ASN1_STRING_to_UTF8(&s, X509_NAME_ENTRY_get_data(entry));
        ଯଦି (len < 0) {
            ngx_ssl_error(NGX_LOG_ALERT, c->log, 0,
                          "ASN1_STRING_to_UTF8() failed");
            X509_free(cert);
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        out->len = len;
        out->data = ngx_pnalloc(c->pool, len);
        ଯଦି (out->data == ଖାଲି) {
            OPENSSL_free(s);
            X509_free(cert);
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        ngx_memcpy(out->data, s, len);

        OPENSSL_free(s);
        X509_free(cert);
        ଫେରନ୍ତୁ NGX_OK;

    ସ୍ଥିତି NGX_PROXY_PROTOCOL_TLV_SSL_SIG_ALG:
        cert = SSL_get_certificate(c->ssl->connection);

        out->data = (u_char *) OBJ_nid2sn(X509_get_signature_nid(cert));
        out->len = ngx_strlen(out->data);
        ଫେରନ୍ତୁ NGX_OK;

    ସ୍ଥିତି NGX_PROXY_PROTOCOL_TLV_SSL_ଚାବି_ALG:
        cert = SSL_get_certificate(c->ssl->connection);

        pkey = X509_get_pubkey(cert);
        ଯଦି (pkey == ଖାଲି) {
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        ବାଛ (EVP_PKEY_base_id(pkey)) {

        ସ୍ଥିତି EVP_PKEY_RSA:
#ifdef EVP_PKEY_RSA_PSS
        ସ୍ଥିତି EVP_PKEY_RSA_PSS:
#endif
            alg = (u_char *) "RSA";
            ଭାଙ୍ଗ;

        ସ୍ଥିତି EVP_PKEY_EC:
            alg = (u_char *) "EC";
            ଭାଙ୍ଗ;

        ସ୍ଥିତି EVP_PKEY_DSA:
            alg = (u_char *) "DSA";
            ଭାଙ୍ଗ;

        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
            EVP_PKEY_free(pkey);
            ଫେରନ୍ତୁ NGX_DECLINED;
        }

        out->len = ଆକାର("RSA16384") - 1;
        out->data = ngx_pnalloc(c->pool, out->len);
        ଯଦି (out->data == ଖାଲି) {
            EVP_PKEY_free(pkey);
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        out->len = ngx_snprintf(out->data, out->len, "%s%d",
                                alg, EVP_PKEY_bits(pkey))
                   - out->data;

        EVP_PKEY_free(pkey);

        ଫେରନ୍ତୁ NGX_OK;
    }

    ଫେରନ୍ତୁ NGX_DECLINED;
}

#endif


ସ୍ଥିର u_char *
ngx_proxy_protocol_v2_write_header(ngx_connection_t *c, u_char *buf,
    u_char *last)
{
    u_char                            *p, src_af, dst_af,
                                       family, command, transport;
    ngx_uint_t                         src_fam, dst_fam;
    ngx_proxy_protocol_header_t       *header;
    ngx_proxy_protocol_inet_addrs_t   *in;
#ଯଦି (NGX_HAVE_INET6)
    ngx_uint_t                         port, lport;
    ସଂରଚନା in6_addr                   *src6, *dst6;
    ngx_proxy_protocol_inet6_addrs_t  *in6;
#endif

    ଯଦି (last - buf < NGX_PROXY_PROTOCOL_V2_MAX_HEADER) {
        ngx_log_error(NGX_LOG_ALERT, c->log, 0,
                      "too small buffer for PROXY protocol v2");
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ଯଦି (ngx_connection_local_sockaddr(c, ଖାଲି, 0) != NGX_OK) {
        ଫେରନ୍ତୁ ଖାଲି;
    }

    /* the length field is set by the caller once all TLVs are written */

    header = (ngx_proxy_protocol_header_t *) buf;

    ngx_memcpy(header->signature, ngx_proxy_protocol_signature,
               ଆକାର(ngx_proxy_protocol_signature) - 1);

    p = buf + ଆକାର(ngx_proxy_protocol_header_t);

    src_fam = c->sockaddr->sa_family;
    dst_fam = c->local_sockaddr->sa_family;

    src_af = ngx_proxy_protocol_v2_family(src_fam);
    dst_af = ngx_proxy_protocol_v2_family(dst_fam);

    /* promote to the highest address family present on either side */

    ଯଦି (src_af == NGX_PROXY_PROTOCOL_AF_UNSPEC
        || dst_af == NGX_PROXY_PROTOCOL_AF_UNSPEC)
    {
        command = NGX_PROXY_PROTOCOL_CMD_LOCAL;
        family = NGX_PROXY_PROTOCOL_AF_UNSPEC;
        transport = NGX_PROXY_PROTOCOL_TYPE_UNSPEC;

    } ଅନ୍ୟଥା {
        command = NGX_PROXY_PROTOCOL_CMD_PROXY;
        family = ngx_max(src_af, dst_af);

        ବାଛ (c->type) {
        ସ୍ଥିତି SOCK_STREAM:
            transport = NGX_PROXY_PROTOCOL_TYPE_STREAM;
            ଭାଙ୍ଗ;
        ସ୍ଥିତି SOCK_DGRAM:
            transport = NGX_PROXY_PROTOCOL_TYPE_DGRAM;
            ଭାଙ୍ଗ;
        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
            transport = NGX_PROXY_PROTOCOL_TYPE_UNSPEC;
            ଭାଙ୍ଗ;
        }
    }

    header->version_command = 0x20 | command;
    header->family_transport = (family << 4) | transport;

    ବାଛ (family) {

    ସ୍ଥିତି NGX_PROXY_PROTOCOL_AF_INET:

        in = (ngx_proxy_protocol_inet_addrs_t *) p;

        ngx_proxy_protocol_v2_write_ipv4(c->sockaddr,
                                         in->src_addr, in->src_port);
        ngx_proxy_protocol_v2_write_ipv4(c->local_sockaddr,
                                         in->dst_addr, in->dst_port);

        p += ଆକାର(ngx_proxy_protocol_inet_addrs_t);

        ଭାଙ୍ଗ;

#ଯଦି (NGX_HAVE_INET6)

    ସ୍ଥିତି NGX_PROXY_PROTOCOL_AF_INET6:

        ଯଦି (src_fam == AF_INET6 && dst_fam == AF_INET6) {
            src6 = &((ସଂରଚନା sockaddr_in6 *) c->sockaddr)->sin6_addr;
            dst6 = &((ସଂରଚନା sockaddr_in6 *) c->local_sockaddr)->sin6_addr;

            ଯଦି (IN6_IS_ADDR_V4MAPPED(src6) && IN6_IS_ADDR_V4MAPPED(dst6)) {

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

                p += ଆକାର(ngx_proxy_protocol_inet_addrs_t);
                ଭାଙ୍ଗ;
            }
        }

        in6 = (ngx_proxy_protocol_inet6_addrs_t *) p;

        ngx_proxy_protocol_v2_write_ipv6(c->sockaddr,
                                         in6->src_addr, in6->src_port);
        ngx_proxy_protocol_v2_write_ipv6(c->local_sockaddr,
                                         in6->dst_addr, in6->dst_port);

        p += ଆକାର(ngx_proxy_protocol_inet6_addrs_t);

        ଭାଙ୍ଗ;

#endif

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:  /* NGX_PROXY_PROTOCOL_AF_UNSPEC */
        ଭାଙ୍ଗ;
    }

    ଫେରନ୍ତୁ p;
}


ସ୍ଥିର u_char
ngx_proxy_protocol_v2_family(ngx_uint_t family)
{
    ବାଛ (family) {

    ସ୍ଥିତି AF_INET:
        ଫେରନ୍ତୁ NGX_PROXY_PROTOCOL_AF_INET;

#ଯଦି (NGX_HAVE_INET6)
    ସ୍ଥିତି AF_INET6:
        ଫେରନ୍ତୁ NGX_PROXY_PROTOCOL_AF_INET6;
#endif

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        ଫେରନ୍ତୁ NGX_PROXY_PROTOCOL_AF_UNSPEC;
    }
}


ସ୍ଥିର ଶୂନ୍ୟ
ngx_proxy_protocol_v2_write_ipv4(ସଂରଚନା sockaddr *sa, u_char *addr,
    u_char *port)
{
    ସଂରଚନା sockaddr_in  *sin;

    sin = (ସଂରଚନା sockaddr_in *) sa;
    ngx_memcpy(addr, &sin->sin_addr, 4);
    ngx_memcpy(port, &sin->sin_port, 2);
}


#ଯଦି (NGX_HAVE_INET6)

ସ୍ଥିର ଶୂନ୍ୟ
ngx_proxy_protocol_v2_write_ipv6(ସଂରଚନା sockaddr *sa, u_char *addr,
    u_char *port)
{
    ସଂରଚନା sockaddr_in   *sin;
    ସଂରଚନା sockaddr_in6  *sin6;

    /* IPv4-mapped prefix: 80 zero bits + 16 one bits */
    ସ୍ଥିର ଅଚଳ u_char  mapped[]   = { 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0xff, 0xff };

    ଯଦି (sa->sa_family == AF_INET6) {
        sin6 = (ସଂରଚନା sockaddr_in6 *) sa;
        ngx_memcpy(addr, &sin6->sin6_addr, 16);
        ngx_memcpy(port, &sin6->sin6_port, 2);

    } ଅନ୍ୟଥା {
        /* promote AF_INET to ::ffff:a.b.c.d */
        sin = (ସଂରଚନା sockaddr_in *) sa;
        ngx_memcpy(addr, mapped, ଆକାର(mapped));
        ngx_memcpy(addr + ଆକାର(mapped), &sin->sin_addr, 4);
        ngx_memcpy(port, &sin->sin_port, 2);
    }
}

#endif


ସ୍ଥିର u_char *
ngx_proxy_protocol_v2_write_tlv(ngx_connection_t *c, u_char *p, u_char *last,
    ngx_uint_t type, ngx_str_t *ମୂଲ୍ୟ)
{
    ngx_proxy_protocol_tlv_t  *tlv;

    ଯଦି ((size_t) (last - p) < ଆକାର(ngx_proxy_protocol_tlv_t) + ମୂଲ୍ୟ->len) {
        ngx_log_error(NGX_LOG_ALERT, c->log, 0,
                      "too small buffer for PROXY protocol v2 TLV");
        ଫେରନ୍ତୁ ଖାଲି;
    }

    tlv = (ngx_proxy_protocol_tlv_t *) p;
    tlv->type = (u_char) type;
    tlv->len[0] = (u_char) (ମୂଲ୍ୟ->len >> 8);
    tlv->len[1] = (u_char)  ମୂଲ୍ୟ->len;

    p += ଆକାର(ngx_proxy_protocol_tlv_t);

    ଫେରନ୍ତୁ ngx_cpymem(p, ମୂଲ୍ୟ->data, ମୂଲ୍ୟ->len);
}


ସ୍ଥିର u_char *
ngx_proxy_protocol_v2_write_ssl(ngx_connection_t *c, u_char *p, u_char *last,
    ngx_array_t *ssl_tlvs, ngx_uint_t client, uint32_t verify)
{
    u_char                          *start;
    uint16_t                         len;
    ngx_uint_t                       i;
    ngx_proxy_protocol_tlv_t        *tlv;
    ngx_proxy_protocol_tlv_ssl_t    *tlv_ssl;
    ngx_proxy_protocol_tlv_ମୂଲ୍ୟ_t  *sub;

    ଯଦି ((size_t) (last - p) < ଆକାର(ngx_proxy_protocol_tlv_t)
                              + ଆକାର(ngx_proxy_protocol_tlv_ssl_t))
    {
        ngx_log_error(NGX_LOG_ALERT, c->log, 0,
                      "too small buffer for PROXY protocol v2 SSL TLV");
        ଫେରନ୍ତୁ ଖାଲି;
    }

    start = p;
    p += ଆକାର(ngx_proxy_protocol_tlv_t);

    tlv_ssl = (ngx_proxy_protocol_tlv_ssl_t *) p;
    tlv_ssl->client = (u_char) client;
    tlv_ssl->verify[0] = (u_char) (verify >> 24);
    tlv_ssl->verify[1] = (u_char) (verify >> 16);
    tlv_ssl->verify[2] = (u_char) (verify >> 8);
    tlv_ssl->verify[3] = (u_char)  verify;

    p += ଆକାର(ngx_proxy_protocol_tlv_ssl_t);

    sub = ssl_tlvs->elts;

    ପାଇଁ (i = 0; i < ssl_tlvs->nelts; i++) {
        p = ngx_proxy_protocol_v2_write_tlv(c, p, last, sub[i].type,
                                            &sub[i].ମୂଲ୍ୟ);
        ଯଦି (p == ଖାଲି) {
            ଫେରନ୍ତୁ ଖାଲି;
        }
    }

    len = (uint16_t) (p - start - ଆକାର(ngx_proxy_protocol_tlv_t));

    tlv = (ngx_proxy_protocol_tlv_t *) start;
    tlv->type = NGX_PROXY_PROTOCOL_TLV_SSL;
    tlv->len[0] = (u_char) (len >> 8);
    tlv->len[1] = (u_char)  len;

    ଫେରନ୍ତୁ p;
}


ସ୍ଥିର ଶୂନ୍ୟ
ngx_proxy_protocol_v2_set_len(u_char *buf, u_char *p)
{
    uint16_t                      len;
    ngx_proxy_protocol_header_t  *header;

    len = (uint16_t) (p - buf - ଆକାର(ngx_proxy_protocol_header_t));

    header = (ngx_proxy_protocol_header_t *) buf;
    header->len[0] = (u_char) (len >> 8);
    header->len[1] = (u_char) len;
}


ସ୍ଥିର u_char *
ngx_proxy_protocol_v2_write_crc32c(ngx_connection_t *c, u_char *buf,
    u_char *p, u_char *last)
{
    u_char                    *ମୂଲ୍ୟ;
    uint32_t                   crc;
    ngx_proxy_protocol_tlv_t  *tlv;

    ଯଦି ((size_t) (last - p) < ଆକାର(ngx_proxy_protocol_tlv_t) + 4) {
        ngx_log_error(NGX_LOG_ALERT, c->log, 0,
                      "too small buffer for PROXY protocol v2 CRC32c TLV");
        ଫେରନ୍ତୁ ଖାଲି;
    }

    /* append the CRC32c TLV with a zeroed value field */

    tlv = (ngx_proxy_protocol_tlv_t *) p;
    tlv->type = NGX_PROXY_PROTOCOL_TLV_CRC32C;
    tlv->len[0] = 0;
    tlv->len[1] = 4;

    ମୂଲ୍ୟ = p + ଆକାର(ngx_proxy_protocol_tlv_t);

    ngx_memzero(ମୂଲ୍ୟ, 4);

    p = ମୂଲ୍ୟ + 4;

    /* the checksum covers the entire header with the zeroed value */

    crc = ngx_crc32c_long(buf, p - buf);

    ମୂଲ୍ୟ[0] = (u_char) (crc >> 24);
    ମୂଲ୍ୟ[1] = (u_char) (crc >> 16);
    ମୂଲ୍ୟ[2] = (u_char) (crc >> 8);
    ମୂଲ୍ୟ[3] = (u_char) crc;

    ଫେରନ୍ତୁ p;
}
