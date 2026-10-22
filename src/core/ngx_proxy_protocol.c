
/*
 * Copyright (C) Roman Arutyunyan
 * Copyright (C) Nginx, Inc.
 */


#অন্তর্ভাব <ngx_config.h>
#অন্তর্ভাব <ngx_core.h>


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
#define NGX_PROXY_PROTOCOL_TLV_SSL_চাবি_ALG  0x25
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


প্রকারলেখ কাঠামো {
    u_char                                  signature[12];
    u_char                                  version_command;
    u_char                                  family_transport;
    u_char                                  len[2];
} ngx_proxy_protocol_header_t;


প্রকারলেখ কাঠামো {
    u_char                                  src_addr[4];
    u_char                                  dst_addr[4];
    u_char                                  src_port[2];
    u_char                                  dst_port[2];
} ngx_proxy_protocol_inet_addrs_t;


প্রকারলেখ কাঠামো {
    u_char                                  src_addr[16];
    u_char                                  dst_addr[16];
    u_char                                  src_port[2];
    u_char                                  dst_port[2];
} ngx_proxy_protocol_inet6_addrs_t;


প্রকারলেখ কাঠামো {
    u_char                                  type;
    u_char                                  len[2];
} ngx_proxy_protocol_tlv_t;


প্রকারলেখ কাঠামো {
    u_char                                  client;
    u_char                                  verify[4];
} ngx_proxy_protocol_tlv_ssl_t;


প্রকারলেখ কাঠামো {
    ngx_str_t                               name;
    ngx_uint_t                              type;
} ngx_proxy_protocol_tlv_entry_t;


প্রকারলেখ কাঠামো {
    ngx_uint_t                              type;
    ngx_str_t                               মান;
} ngx_proxy_protocol_tlv_মান_t;


স্থির u_char *ngx_proxy_protocol_read_addr(ngx_connection_t *c, u_char *p,
    u_char *last, ngx_str_t *addr);
স্থির u_char *ngx_proxy_protocol_read_port(u_char *p, u_char *last,
    in_port_t *port, u_char sep);
স্থির u_char *ngx_proxy_protocol_v2_read(ngx_connection_t *c, u_char *buf,
    u_char *last);
স্থির ngx_int_t ngx_proxy_protocol_lookup_tlv(ngx_connection_t *c,
    ngx_str_t *tlvs, ngx_uint_t type, ngx_str_t *মান);
#যদি (NGX_OPENSSL)
স্থির ngx_int_t ngx_proxy_protocol_v2_eval_ssl(ngx_connection_t *c,
    ngx_array_t *tlvs, ngx_array_t *ssl_tlvs, ngx_uint_t *client,
    uint32_t *verify);
স্থির ngx_int_t ngx_proxy_protocol_v2_authority(ngx_connection_t *c,
    ngx_str_t *out);
স্থির ngx_int_t ngx_proxy_protocol_v2_alpn(ngx_connection_t *c,
    ngx_str_t *out);
স্থির ngx_int_t ngx_proxy_protocol_v2_ssl_sub(ngx_connection_t *c,
    ngx_uint_t type, ngx_str_t *out);
#endif
স্থির u_char *ngx_proxy_protocol_v2_write_header(ngx_connection_t *c,
    u_char *buf, u_char *last);
স্থির u_char ngx_proxy_protocol_v2_family(ngx_uint_t family);
স্থির শূন্য ngx_proxy_protocol_v2_write_ipv4(কাঠামো sockaddr *sa, u_char *addr,
    u_char *port);
স্থির শূন্য ngx_proxy_protocol_v2_write_ipv6(কাঠামো sockaddr *sa, u_char *addr,
    u_char *port);
স্থির u_char *ngx_proxy_protocol_v2_write_tlv(ngx_connection_t *c, u_char *p,
    u_char *last, ngx_uint_t type, ngx_str_t *মান);
স্থির u_char *ngx_proxy_protocol_v2_write_ssl(ngx_connection_t *c, u_char *p,
    u_char *last, ngx_array_t *ssl_tlvs, ngx_uint_t client, uint32_t verify);
স্থির শূন্য ngx_proxy_protocol_v2_set_len(u_char *buf, u_char *p);
স্থির u_char *ngx_proxy_protocol_v2_write_crc32c(ngx_connection_t *c,
    u_char *buf, u_char *p, u_char *last);


স্থির ngx_proxy_protocol_tlv_entry_t  ngx_proxy_protocol_tlv_entries[] = {
    { ngx_string("alpn"),       NGX_PROXY_PROTOCOL_TLV_ALPN },
    { ngx_string("authority"),  NGX_PROXY_PROTOCOL_TLV_AUTHORITY },
    { ngx_string("unique_id"),  NGX_PROXY_PROTOCOL_TLV_UNIQUE_ID },
    { ngx_string("ssl"),        NGX_PROXY_PROTOCOL_TLV_SSL },
    { ngx_string("netns"),      NGX_PROXY_PROTOCOL_TLV_NETNS },
    { ngx_null_string,          0x00 }
};


স্থির ngx_proxy_protocol_tlv_entry_t  ngx_proxy_protocol_tlv_ssl_entries[] = {
    { ngx_string("version"),    NGX_PROXY_PROTOCOL_TLV_SSL_VERSION },
    { ngx_string("cn"),         NGX_PROXY_PROTOCOL_TLV_SSL_CN },
    { ngx_string("cipher"),     NGX_PROXY_PROTOCOL_TLV_SSL_CIPHER },
    { ngx_string("sig_alg"),    NGX_PROXY_PROTOCOL_TLV_SSL_SIG_ALG },
    { ngx_string("key_alg"),    NGX_PROXY_PROTOCOL_TLV_SSL_চাবি_ALG },
    { ngx_null_string,          0x00 }
};


স্থির ধ্রুবক u_char  ngx_proxy_protocol_signature[] = "\r\n\r\n\0\r\nQUIT\n";


u_char *
ngx_proxy_protocol_read(ngx_connection_t *c, u_char *buf, u_char *last)
{
    আকার_t                 len;
    u_char                *p;
    ngx_proxy_protocol_t  *pp;

    p = buf;
    len = last - buf;

    যদি (len >= আকার(ngx_proxy_protocol_header_t)
        && ngx_memcmp(p, ngx_proxy_protocol_signature,
                      আকার(ngx_proxy_protocol_signature) - 1)
           == 0)
    {
        ফেরত ngx_proxy_protocol_v2_read(c, buf, last);
    }

    যদি (len < 8 || ngx_strncmp(p, "PROXY ", 6) != 0) {
        যাও invalid;
    }

    p += 6;
    len -= 6;

    যদি (len >= 7 && ngx_strncmp(p, "UNKNOWN", 7) == 0) {
        ngx_log_debug0(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "PROXY protocol unknown protocol");
        p += 7;
        যাও skip;
    }

    যদি (len < 5 || ngx_strncmp(p, "TCP", 3) != 0
        || (p[3] != '4' && p[3] != '6') || p[4] != ' ')
    {
        যাও invalid;
    }

    p += 5;

    pp = ngx_pcalloc(c->pool, আকার(ngx_proxy_protocol_t));
    যদি (pp == ফাঁকা) {
        ফেরত ফাঁকা;
    }

    p = ngx_proxy_protocol_read_addr(c, p, last, &pp->src_addr);
    যদি (p == ফাঁকা) {
        যাও invalid;
    }

    p = ngx_proxy_protocol_read_addr(c, p, last, &pp->dst_addr);
    যদি (p == ফাঁকা) {
        যাও invalid;
    }

    p = ngx_proxy_protocol_read_port(p, last, &pp->src_port, ' ');
    যদি (p == ফাঁকা) {
        যাও invalid;
    }

    p = ngx_proxy_protocol_read_port(p, last, &pp->dst_port, CR);
    যদি (p == ফাঁকা) {
        যাও invalid;
    }

    যদি (p == last) {
        যাও invalid;
    }

    যদি (*p++ != LF) {
        যাও invalid;
    }

    ngx_log_debug4(NGX_LOG_DEBUG_CORE, c->log, 0,
                   "PROXY protocol src: %V %d, dst: %V %d",
                   &pp->src_addr, pp->src_port, &pp->dst_addr, pp->dst_port);

    c->proxy_protocol = pp;

    ফেরত p;

skip:

    জন্য ( /* void */ ; p < last - 1; p++) {
        যদি (p[0] == CR && p[1] == LF) {
            ফেরত p + 2;
        }
    }

invalid:

    জন্য (p = buf; p < last; p++) {
        যদি (*p == CR || *p == LF) {
            বিরতি;
        }
    }

    ngx_log_error(NGX_LOG_ERR, c->log, 0,
                  "broken header: \"%*s\"", (আকার_t) (p - buf), buf);

    ফেরত ফাঁকা;
}


স্থির u_char *
ngx_proxy_protocol_read_addr(ngx_connection_t *c, u_char *p, u_char *last,
    ngx_str_t *addr)
{
    আকার_t  len;
    u_char  ch, *pos;

    pos = p;

    জন্য ( ;; ) {
        যদি (p == last) {
            ফেরত ফাঁকা;
        }

        ch = *p++;

        যদি (ch == ' ') {
            বিরতি;
        }

        যদি (ch != ':' && ch != '.'
            && (ch < 'a' || ch > 'f')
            && (ch < 'A' || ch > 'F')
            && (ch < '0' || ch > '9'))
        {
            ফেরত ফাঁকা;
        }
    }

    len = p - pos - 1;

    addr->তথ্য = ngx_pnalloc(c->pool, len);
    যদি (addr->তথ্য == ফাঁকা) {
        ফেরত ফাঁকা;
    }

    ngx_memcpy(addr->তথ্য, pos, len);
    addr->len = len;

    ফেরত p;
}


স্থির u_char *
ngx_proxy_protocol_read_port(u_char *p, u_char *last, in_port_t *port,
    u_char sep)
{
    আকার_t      len;
    u_char     *pos;
    ngx_int_t   n;

    pos = p;

    জন্য ( ;; ) {
        যদি (p == last) {
            ফেরত ফাঁকা;
        }

        যদি (*p++ == sep) {
            বিরতি;
        }
    }

    len = p - pos - 1;

    n = ngx_atoi(pos, len);
    যদি (n < 0 || n > 65535) {
        ফেরত ফাঁকা;
    }

    *port = (in_port_t) n;

    ফেরত p;
}


u_char *
ngx_proxy_protocol_write(ngx_connection_t *c, u_char *buf, u_char *last)
{
    ngx_uint_t  port, lport;

    যদি (last - buf < NGX_PROXY_PROTOCOL_V1_MAX_HEADER) {
        ngx_log_error(NGX_LOG_ALERT, c->log, 0,
                      "too small buffer for PROXY protocol");
        ফেরত ফাঁকা;
    }

    যদি (ngx_connection_local_sockaddr(c, ফাঁকা, 0) != NGX_OK) {
        ফেরত ফাঁকা;
    }

    নির্বাচন (c->sockaddr->sa_family) {

    ক্ষেত্র AF_INET:
        buf = ngx_cpymem(buf, "PROXY TCP4 ", আকার("PROXY TCP4 ") - 1);
        বিরতি;

#যদি (NGX_HAVE_INET6)
    ক্ষেত্র AF_INET6:
        buf = ngx_cpymem(buf, "PROXY TCP6 ", আকার("PROXY TCP6 ") - 1);
        বিরতি;
#endif

    পূর্বনির্ধারিত:
        ফেরত ngx_cpymem(buf, "PROXY UNKNOWN" CRLF,
                          আকার("PROXY UNKNOWN" CRLF) - 1);
    }

    buf += ngx_sock_ntop(c->sockaddr, c->socklen, buf, last - buf, 0);

    *buf++ = ' ';

    buf += ngx_sock_ntop(c->local_sockaddr, c->local_socklen, buf, last - buf,
                         0);

    port = ngx_inet_get_port(c->sockaddr);
    lport = ngx_inet_get_port(c->local_sockaddr);

    ফেরত ngx_slprintf(buf, last, " %ui %ui" CRLF, port, lport);
}


স্থির u_char *
ngx_proxy_protocol_v2_read(ngx_connection_t *c, u_char *buf, u_char *last)
{
    u_char                             *end;
    আকার_t                              len;
    socklen_t                           socklen;
    ngx_uint_t                          version, command, family, transport;
    ngx_sockaddr_t                      src_sockaddr, dst_sockaddr;
    ngx_proxy_protocol_t               *pp;
    ngx_proxy_protocol_header_t        *header;
    ngx_proxy_protocol_inet_addrs_t    *in;
#যদি (NGX_HAVE_INET6)
    ngx_proxy_protocol_inet6_addrs_t   *in6;
#endif

    header = (ngx_proxy_protocol_header_t *) buf;

    buf += আকার(ngx_proxy_protocol_header_t);

    version = header->version_command >> 4;

    যদি (version != 2) {
        ngx_log_error(NGX_LOG_ERR, c->log, 0,
                      "unknown PROXY protocol version: %ui", version);
        ফেরত ফাঁকা;
    }

    len = ngx_proxy_protocol_parse_uint16(header->len);

    যদি ((আকার_t) (last - buf) < len) {
        ngx_log_error(NGX_LOG_ERR, c->log, 0, "header is too large");
        ফেরত ফাঁকা;
    }

    end = buf + len;

    command = header->version_command & 0x0f;

    /* only PROXY is supported */
    যদি (command != NGX_PROXY_PROTOCOL_CMD_PROXY) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "PROXY protocol v2 unsupported command %ui", command);
        ফেরত end;
    }

    transport = header->family_transport & 0x0f;

    /* only STREAM is supported */
    যদি (transport != NGX_PROXY_PROTOCOL_TYPE_STREAM) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "PROXY protocol v2 unsupported transport %ui",
                       transport);
        ফেরত end;
    }

    pp = ngx_pcalloc(c->pool, আকার(ngx_proxy_protocol_t));
    যদি (pp == ফাঁকা) {
        ফেরত ফাঁকা;
    }

    family = header->family_transport >> 4;

    নির্বাচন (family) {

    ক্ষেত্র NGX_PROXY_PROTOCOL_AF_INET:

        যদি ((আকার_t) (end - buf) < আকার(ngx_proxy_protocol_inet_addrs_t)) {
            ফেরত ফাঁকা;
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

        socklen = আকার(কাঠামো sockaddr_in);

        buf += আকার(ngx_proxy_protocol_inet_addrs_t);

        বিরতি;

#যদি (NGX_HAVE_INET6)

    ক্ষেত্র NGX_PROXY_PROTOCOL_AF_INET6:

        যদি ((আকার_t) (end - buf) < আকার(ngx_proxy_protocol_inet6_addrs_t)) {
            ফেরত ফাঁকা;
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

        socklen = আকার(কাঠামো sockaddr_in6);

        buf += আকার(ngx_proxy_protocol_inet6_addrs_t);

        বিরতি;

#endif

    পূর্বনির্ধারিত:
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "PROXY protocol v2 unsupported address family %ui",
                       family);
        ফেরত end;
    }

    pp->src_addr.তথ্য = ngx_pnalloc(c->pool, NGX_SOCKADDR_STRLEN);
    যদি (pp->src_addr.তথ্য == ফাঁকা) {
        ফেরত ফাঁকা;
    }

    pp->src_addr.len = ngx_sock_ntop(&src_sockaddr.sockaddr, socklen,
                                     pp->src_addr.তথ্য, NGX_SOCKADDR_STRLEN, 0);

    pp->dst_addr.তথ্য = ngx_pnalloc(c->pool, NGX_SOCKADDR_STRLEN);
    যদি (pp->dst_addr.তথ্য == ফাঁকা) {
        ফেরত ফাঁকা;
    }

    pp->dst_addr.len = ngx_sock_ntop(&dst_sockaddr.sockaddr, socklen,
                                     pp->dst_addr.তথ্য, NGX_SOCKADDR_STRLEN, 0);

    ngx_log_debug4(NGX_LOG_DEBUG_CORE, c->log, 0,
                   "PROXY protocol v2 src: %V %d, dst: %V %d",
                   &pp->src_addr, pp->src_port, &pp->dst_addr, pp->dst_port);

    যদি (buf < end) {
        pp->tlvs.তথ্য = ngx_pnalloc(c->pool, end - buf);
        যদি (pp->tlvs.তথ্য == ফাঁকা) {
            ফেরত ফাঁকা;
        }

        ngx_memcpy(pp->tlvs.তথ্য, buf, end - buf);
        pp->tlvs.len = end - buf;
    }

    c->proxy_protocol = pp;

    ফেরত end;
}


ngx_int_t
ngx_proxy_protocol_get_tlv(ngx_connection_t *c, ngx_str_t *name,
    ngx_str_t *মান)
{
    u_char                          *p;
    আকার_t                           n;
    uint32_t                         verify;
    ngx_str_t                        ssl, *tlvs;
    ngx_int_t                        rc, type;
    ngx_proxy_protocol_tlv_ssl_t    *tlv_ssl;
    ngx_proxy_protocol_tlv_entry_t  *te;

    যদি (c->proxy_protocol == ফাঁকা) {
        ফেরত NGX_DECLINED;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                   "PROXY protocol v2 get tlv \"%V\"", name);

    te = ngx_proxy_protocol_tlv_entries;
    tlvs = &c->proxy_protocol->tlvs;

    p = name->তথ্য;
    n = name->len;

    যদি (n >= 4 && p[0] == 's' && p[1] == 's' && p[2] == 'l' && p[3] == '_') {

        rc = ngx_proxy_protocol_lookup_tlv(c, tlvs,
                                           NGX_PROXY_PROTOCOL_TLV_SSL, &ssl);
        যদি (rc != NGX_OK) {
            ফেরত rc;
        }

        যদি (ssl.len < আকার(ngx_proxy_protocol_tlv_ssl_t)) {
            ফেরত NGX_ERROR;
        }

        p += 4;
        n -= 4;

        যদি (n == 6 && ngx_strncmp(p, "verify", 6) == 0) {

            tlv_ssl = (ngx_proxy_protocol_tlv_ssl_t *) ssl.তথ্য;
            verify = ngx_proxy_protocol_parse_uint32(tlv_ssl->verify);

            মান->তথ্য = ngx_pnalloc(c->pool, NGX_INT32_LEN);
            যদি (মান->তথ্য == ফাঁকা) {
                ফেরত NGX_ERROR;
            }

            মান->len = ngx_sprintf(মান->তথ্য, "%uD", verify)
                         - মান->তথ্য;
            ফেরত NGX_OK;
        }

        ssl.তথ্য += আকার(ngx_proxy_protocol_tlv_ssl_t);
        ssl.len -= আকার(ngx_proxy_protocol_tlv_ssl_t);

        te = ngx_proxy_protocol_tlv_ssl_entries;
        tlvs = &ssl;
    }

    যদি (n >= 2 && p[0] == '0' && p[1] == 'x') {

        type = ngx_hextoi(p + 2, n - 2);
        যদি (type == NGX_ERROR) {
            ngx_log_error(NGX_LOG_ERR, c->log, 0,
                          "invalid PROXY protocol TLV \"%V\"", name);
            ফেরত NGX_ERROR;
        }

        ফেরত ngx_proxy_protocol_lookup_tlv(c, tlvs, type, মান);
    }

    জন্য ( /* void */ ; te->type; te++) {
        যদি (te->name.len == n && ngx_strncmp(te->name.তথ্য, p, n) == 0) {
            ফেরত ngx_proxy_protocol_lookup_tlv(c, tlvs, te->type, মান);
        }
    }

    ngx_log_error(NGX_LOG_ERR, c->log, 0,
                  "unknown PROXY protocol TLV \"%V\"", name);

    ফেরত NGX_DECLINED;
}


স্থির ngx_int_t
ngx_proxy_protocol_lookup_tlv(ngx_connection_t *c, ngx_str_t *tlvs,
    ngx_uint_t type, ngx_str_t *মান)
{
    u_char                    *p;
    আকার_t                     n, len;
    ngx_proxy_protocol_tlv_t  *tlv;

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                   "PROXY protocol v2 lookup tlv:%02xi", type);

    p = tlvs->তথ্য;
    n = tlvs->len;

    যখন (n) {
        যদি (n < আকার(ngx_proxy_protocol_tlv_t)) {
            ngx_log_error(NGX_LOG_ERR, c->log, 0, "broken PROXY protocol TLV");
            ফেরত NGX_ERROR;
        }

        tlv = (ngx_proxy_protocol_tlv_t *) p;
        len = ngx_proxy_protocol_parse_uint16(tlv->len);

        p += আকার(ngx_proxy_protocol_tlv_t);
        n -= আকার(ngx_proxy_protocol_tlv_t);

        যদি (n < len) {
            ngx_log_error(NGX_LOG_ERR, c->log, 0, "broken PROXY protocol TLV");
            ফেরত NGX_ERROR;
        }

        যদি (tlv->type == type) {
            মান->তথ্য = p;
            মান->len = len;
            ফেরত NGX_OK;
        }

        p += len;
        n -= len;
    }

    ফেরত NGX_DECLINED;
}


u_char *
ngx_proxy_protocol_v2_write(ngx_connection_t *c, u_char *buf, u_char *last,
    ngx_array_t *tlvs /* reserved */ )
{
    u_char                          *p;
    uint32_t                         verify;
    ngx_uint_t                       i, client;
    ngx_array_t                      base_tlvs, ssl_tlvs;
    ngx_proxy_protocol_tlv_মান_t  *tlv;

    client = 0;
    verify = 0;

    ngx_memzero(&base_tlvs, আকার(ngx_array_t));
    ngx_memzero(&ssl_tlvs, আকার(ngx_array_t));

#যদি (NGX_OPENSSL)

    যদি (c->ssl != ফাঁকা) {
        যদি (ngx_array_init(&base_tlvs, c->pool, 2,
                           আকার(ngx_proxy_protocol_tlv_মান_t))
            != NGX_OK)
        {
            ফেরত ফাঁকা;
        }

        যদি (ngx_array_init(&ssl_tlvs, c->pool, 5,
                           আকার(ngx_proxy_protocol_tlv_মান_t))
            != NGX_OK)
        {
            ফেরত ফাঁকা;
        }

        যদি (ngx_proxy_protocol_v2_eval_ssl(c, &base_tlvs, &ssl_tlvs, &client,
                                           &verify)
            != NGX_OK)
        {
            ফেরত ফাঁকা;
        }
    }

#endif

    p = ngx_proxy_protocol_v2_write_header(c, buf, last);
    যদি (p == ফাঁকা) {
        ফেরত ফাঁকা;
    }

    tlv = base_tlvs.elts;

    জন্য (i = 0; i < base_tlvs.nelts; i++) {
        p = ngx_proxy_protocol_v2_write_tlv(c, p, last, tlv[i].type,
                                           &tlv[i].মান);
        যদি (p == ফাঁকা) {
            ফেরত ফাঁকা;
        }
    }

    যদি (client) {
        p = ngx_proxy_protocol_v2_write_ssl(c, p, last, &ssl_tlvs, client,
                                            verify);
        যদি (p == ফাঁকা) {
            ফেরত ফাঁকা;
        }
    }

    ngx_proxy_protocol_v2_set_len(buf,
                                  p + আকার(ngx_proxy_protocol_tlv_t) + 4);

    ফেরত ngx_proxy_protocol_v2_write_crc32c(c, buf, p, last);
}


#যদি (NGX_OPENSSL)

স্থির ngx_int_t
ngx_proxy_protocol_v2_eval_ssl(ngx_connection_t *c, ngx_array_t *tlvs,
    ngx_array_t *ssl_tlvs, ngx_uint_t *client, uint32_t *verify)
{
    X509                            *cert;
    long                             n;
    ngx_int_t                        rc;
    ngx_str_t                        মান;
    ngx_uint_t                       type;
    ধ্রুবক অক্ষর                      *s;
    ngx_proxy_protocol_tlv_মান_t  *tlv;

    যদি (ngx_proxy_protocol_v2_authority(c, &মান) == NGX_OK) {
        tlv = ngx_array_ঠেলো(tlvs);
        যদি (tlv == ফাঁকা) {
            ফেরত NGX_ERROR;
        }

        tlv->type = NGX_PROXY_PROTOCOL_TLV_AUTHORITY;
        tlv->মান = মান;
    }

    যদি (ngx_proxy_protocol_v2_alpn(c, &মান) == NGX_OK) {
        tlv = ngx_array_ঠেলো(tlvs);
        যদি (tlv == ফাঁকা) {
            ফেরত NGX_ERROR;
        }

        tlv->type = NGX_PROXY_PROTOCOL_TLV_ALPN;
        tlv->মান = মান;
    }

    জন্য (type = NGX_PROXY_PROTOCOL_TLV_SSL_VERSION;
         type <= NGX_PROXY_PROTOCOL_TLV_SSL_চাবি_ALG;
         type++)
    {
        rc = ngx_proxy_protocol_v2_ssl_sub(c, type, &মান);

        যদি (rc == NGX_ERROR) {
            ফেরত NGX_ERROR;
        }

        যদি (rc != NGX_OK) {
            চালু;
        }

        tlv = ngx_array_ঠেলো(ssl_tlvs);
        যদি (tlv == ফাঁকা) {
            ফেরত NGX_ERROR;
        }

        tlv->type = type;
        tlv->মান = মান;
    }

    *client = NGX_PROXY_PROTOCOL_V2_CLIENT_SSL;
    *verify = 1;  /* X509_V_ERR_UNSPECIFIED */

    cert = SSL_get_peer_certificate(c->ssl->connection);

    যদি (cert != ফাঁকা) {
        X509_free(cert);

        *client |= NGX_PROXY_PROTOCOL_V2_CLIENT_CERT_SESS;

        যদি (!SSL_session_reused(c->ssl->connection)) {
            *client |= NGX_PROXY_PROTOCOL_V2_CLIENT_CERT_CONN;
        }

        n = SSL_get_verify_ফলাফল(c->ssl->connection);

        যদি (n == X509_V_OK) {

            যদি (ngx_ssl_ocsp_get_status(c, &s) != NGX_OK) {
                n = X509_V_ERR_CERT_REVOKED;
            }
        }

        *verify = (uint32_t) n;
    }

    ফেরত NGX_OK;
}


স্থির ngx_int_t
ngx_proxy_protocol_v2_authority(ngx_connection_t *c, ngx_str_t *out)
{
#ifdef SSL_CTRL_SET_TLSEXT_HOSTNAME

    ধ্রুবক অক্ষর  *sni;

    sni = SSL_get_servername(c->ssl->connection, TLSEXT_NAMETYPE_host_name);
    যদি (sni == ফাঁকা) {
        ফেরত NGX_DECLINED;
    }

    out->তথ্য = (u_char *) sni;
    out->len = ngx_strlen(sni);
    ফেরত NGX_OK;

#অন্যথা
    ফেরত NGX_DECLINED;
#endif
}


স্থির ngx_int_t
ngx_proxy_protocol_v2_alpn(ngx_connection_t *c, ngx_str_t *out)
{
#ifdef TLSEXT_TYPE_application_layer_protocol_negotiation

    unsigned পূর্ণসংখ্যা          alpnlen;
    ধ্রুবক unsigned অক্ষর  *alpn;

    SSL_get0_alpn_selected(c->ssl->connection, &alpn, &alpnlen);
    যদি (alpn == ফাঁকা || alpnlen == 0) {
        ফেরত NGX_DECLINED;
    }

    out->তথ্য = (u_char *) alpn;
    out->len = alpnlen;
    ফেরত NGX_OK;

#অন্যথা
    ফেরত NGX_DECLINED;
#endif
}


স্থির ngx_int_t
ngx_proxy_protocol_v2_ssl_sub(ngx_connection_t *c, ngx_uint_t type,
    ngx_str_t *out)
{
    পূর্ণসংখ্যা               i, len;
    X509             *cert;
    u_char           *alg, *s;
    EVP_PKEY         *pkey;
#যদি (OPENSSL_VERSION_সংখ্যা >= 0x40000000L)
    ধ্রুবক
#endif
    X509_NAME        *subject;
#যদি (OPENSSL_VERSION_সংখ্যা >= 0x40000000L)
    ধ্রুবক
#endif
    X509_NAME_ENTRY  *entry;

    নির্বাচন (type) {

    ক্ষেত্র NGX_PROXY_PROTOCOL_TLV_SSL_VERSION:
        out->তথ্য = (u_char *) SSL_get_version(c->ssl->connection);
        out->len = ngx_strlen(out->তথ্য);
        ফেরত NGX_OK;

    ক্ষেত্র NGX_PROXY_PROTOCOL_TLV_SSL_CIPHER:
        out->তথ্য = (u_char *) SSL_get_cipher_name(c->ssl->connection);
        out->len = ngx_strlen(out->তথ্য);
        ফেরত NGX_OK;

    ক্ষেত্র NGX_PROXY_PROTOCOL_TLV_SSL_CN:
        cert = SSL_get_peer_certificate(c->ssl->connection);
        যদি (cert == ফাঁকা) {
            ফেরত NGX_DECLINED;
        }

        subject = X509_get_subject_name(cert);
        যদি (subject == ফাঁকা) {
            X509_free(cert);
            ফেরত NGX_ERROR;
        }

        i = X509_NAME_get_সূচক_by_NID(subject, NID_commonName, -1);
        যদি (i < 0) {
            X509_free(cert);
            ফেরত NGX_DECLINED;
        }

        entry = X509_NAME_get_entry(subject, i);

        len = ASN1_STRING_to_UTF8(&s, X509_NAME_ENTRY_get_তথ্য(entry));
        যদি (len < 0) {
            ngx_ssl_error(NGX_LOG_ALERT, c->log, 0,
                          "ASN1_STRING_to_UTF8() failed");
            X509_free(cert);
            ফেরত NGX_ERROR;
        }

        out->len = len;
        out->তথ্য = ngx_pnalloc(c->pool, len);
        যদি (out->তথ্য == ফাঁকা) {
            OPENSSL_free(s);
            X509_free(cert);
            ফেরত NGX_ERROR;
        }

        ngx_memcpy(out->তথ্য, s, len);

        OPENSSL_free(s);
        X509_free(cert);
        ফেরত NGX_OK;

    ক্ষেত্র NGX_PROXY_PROTOCOL_TLV_SSL_SIG_ALG:
        cert = SSL_get_certificate(c->ssl->connection);

        out->তথ্য = (u_char *) OBJ_nid2sn(X509_get_signature_nid(cert));
        out->len = ngx_strlen(out->তথ্য);
        ফেরত NGX_OK;

    ক্ষেত্র NGX_PROXY_PROTOCOL_TLV_SSL_চাবি_ALG:
        cert = SSL_get_certificate(c->ssl->connection);

        pkey = X509_get_pubkey(cert);
        যদি (pkey == ফাঁকা) {
            ফেরত NGX_ERROR;
        }

        নির্বাচন (EVP_PKEY_base_id(pkey)) {

        ক্ষেত্র EVP_PKEY_RSA:
#ifdef EVP_PKEY_RSA_PSS
        ক্ষেত্র EVP_PKEY_RSA_PSS:
#endif
            alg = (u_char *) "RSA";
            বিরতি;

        ক্ষেত্র EVP_PKEY_EC:
            alg = (u_char *) "EC";
            বিরতি;

        ক্ষেত্র EVP_PKEY_DSA:
            alg = (u_char *) "DSA";
            বিরতি;

        পূর্বনির্ধারিত:
            EVP_PKEY_free(pkey);
            ফেরত NGX_DECLINED;
        }

        out->len = আকার("RSA16384") - 1;
        out->তথ্য = ngx_pnalloc(c->pool, out->len);
        যদি (out->তথ্য == ফাঁকা) {
            EVP_PKEY_free(pkey);
            ফেরত NGX_ERROR;
        }

        out->len = ngx_snprintf(out->তথ্য, out->len, "%s%d",
                                alg, EVP_PKEY_bits(pkey))
                   - out->তথ্য;

        EVP_PKEY_free(pkey);

        ফেরত NGX_OK;
    }

    ফেরত NGX_DECLINED;
}

#endif


স্থির u_char *
ngx_proxy_protocol_v2_write_header(ngx_connection_t *c, u_char *buf,
    u_char *last)
{
    u_char                            *p, src_af, dst_af,
                                       family, command, transport;
    ngx_uint_t                         src_fam, dst_fam;
    ngx_proxy_protocol_header_t       *header;
    ngx_proxy_protocol_inet_addrs_t   *in;
#যদি (NGX_HAVE_INET6)
    ngx_uint_t                         port, lport;
    কাঠামো in6_addr                   *src6, *dst6;
    ngx_proxy_protocol_inet6_addrs_t  *in6;
#endif

    যদি (last - buf < NGX_PROXY_PROTOCOL_V2_MAX_HEADER) {
        ngx_log_error(NGX_LOG_ALERT, c->log, 0,
                      "too small buffer for PROXY protocol v2");
        ফেরত ফাঁকা;
    }

    যদি (ngx_connection_local_sockaddr(c, ফাঁকা, 0) != NGX_OK) {
        ফেরত ফাঁকা;
    }

    /* the length field is set by the caller once all TLVs are written */

    header = (ngx_proxy_protocol_header_t *) buf;

    ngx_memcpy(header->signature, ngx_proxy_protocol_signature,
               আকার(ngx_proxy_protocol_signature) - 1);

    p = buf + আকার(ngx_proxy_protocol_header_t);

    src_fam = c->sockaddr->sa_family;
    dst_fam = c->local_sockaddr->sa_family;

    src_af = ngx_proxy_protocol_v2_family(src_fam);
    dst_af = ngx_proxy_protocol_v2_family(dst_fam);

    /* promote to the highest address family present on either side */

    যদি (src_af == NGX_PROXY_PROTOCOL_AF_UNSPEC
        || dst_af == NGX_PROXY_PROTOCOL_AF_UNSPEC)
    {
        command = NGX_PROXY_PROTOCOL_CMD_LOCAL;
        family = NGX_PROXY_PROTOCOL_AF_UNSPEC;
        transport = NGX_PROXY_PROTOCOL_TYPE_UNSPEC;

    } অন্যথা {
        command = NGX_PROXY_PROTOCOL_CMD_PROXY;
        family = ngx_max(src_af, dst_af);

        নির্বাচন (c->type) {
        ক্ষেত্র SOCK_STREAM:
            transport = NGX_PROXY_PROTOCOL_TYPE_STREAM;
            বিরতি;
        ক্ষেত্র SOCK_DGRAM:
            transport = NGX_PROXY_PROTOCOL_TYPE_DGRAM;
            বিরতি;
        পূর্বনির্ধারিত:
            transport = NGX_PROXY_PROTOCOL_TYPE_UNSPEC;
            বিরতি;
        }
    }

    header->version_command = 0x20 | command;
    header->family_transport = (family << 4) | transport;

    নির্বাচন (family) {

    ক্ষেত্র NGX_PROXY_PROTOCOL_AF_INET:

        in = (ngx_proxy_protocol_inet_addrs_t *) p;

        ngx_proxy_protocol_v2_write_ipv4(c->sockaddr,
                                         in->src_addr, in->src_port);
        ngx_proxy_protocol_v2_write_ipv4(c->local_sockaddr,
                                         in->dst_addr, in->dst_port);

        p += আকার(ngx_proxy_protocol_inet_addrs_t);

        বিরতি;

#যদি (NGX_HAVE_INET6)

    ক্ষেত্র NGX_PROXY_PROTOCOL_AF_INET6:

        যদি (src_fam == AF_INET6 && dst_fam == AF_INET6) {
            src6 = &((কাঠামো sockaddr_in6 *) c->sockaddr)->sin6_addr;
            dst6 = &((কাঠামো sockaddr_in6 *) c->local_sockaddr)->sin6_addr;

            যদি (IN6_IS_ADDR_V4MAPPED(src6) && IN6_IS_ADDR_V4MAPPED(dst6)) {

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

                p += আকার(ngx_proxy_protocol_inet_addrs_t);
                বিরতি;
            }
        }

        in6 = (ngx_proxy_protocol_inet6_addrs_t *) p;

        ngx_proxy_protocol_v2_write_ipv6(c->sockaddr,
                                         in6->src_addr, in6->src_port);
        ngx_proxy_protocol_v2_write_ipv6(c->local_sockaddr,
                                         in6->dst_addr, in6->dst_port);

        p += আকার(ngx_proxy_protocol_inet6_addrs_t);

        বিরতি;

#endif

    পূর্বনির্ধারিত:  /* NGX_PROXY_PROTOCOL_AF_UNSPEC */
        বিরতি;
    }

    ফেরত p;
}


স্থির u_char
ngx_proxy_protocol_v2_family(ngx_uint_t family)
{
    নির্বাচন (family) {

    ক্ষেত্র AF_INET:
        ফেরত NGX_PROXY_PROTOCOL_AF_INET;

#যদি (NGX_HAVE_INET6)
    ক্ষেত্র AF_INET6:
        ফেরত NGX_PROXY_PROTOCOL_AF_INET6;
#endif

    পূর্বনির্ধারিত:
        ফেরত NGX_PROXY_PROTOCOL_AF_UNSPEC;
    }
}


স্থির শূন্য
ngx_proxy_protocol_v2_write_ipv4(কাঠামো sockaddr *sa, u_char *addr,
    u_char *port)
{
    কাঠামো sockaddr_in  *sin;

    sin = (কাঠামো sockaddr_in *) sa;
    ngx_memcpy(addr, &sin->sin_addr, 4);
    ngx_memcpy(port, &sin->sin_port, 2);
}


#যদি (NGX_HAVE_INET6)

স্থির শূন্য
ngx_proxy_protocol_v2_write_ipv6(কাঠামো sockaddr *sa, u_char *addr,
    u_char *port)
{
    কাঠামো sockaddr_in   *sin;
    কাঠামো sockaddr_in6  *sin6;

    /* IPv4-mapped prefix: 80 zero bits + 16 one bits */
    স্থির ধ্রুবক u_char  mapped[]   = { 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0xff, 0xff };

    যদি (sa->sa_family == AF_INET6) {
        sin6 = (কাঠামো sockaddr_in6 *) sa;
        ngx_memcpy(addr, &sin6->sin6_addr, 16);
        ngx_memcpy(port, &sin6->sin6_port, 2);

    } অন্যথা {
        /* promote AF_INET to ::ffff:a.b.c.d */
        sin = (কাঠামো sockaddr_in *) sa;
        ngx_memcpy(addr, mapped, আকার(mapped));
        ngx_memcpy(addr + আকার(mapped), &sin->sin_addr, 4);
        ngx_memcpy(port, &sin->sin_port, 2);
    }
}

#endif


স্থির u_char *
ngx_proxy_protocol_v2_write_tlv(ngx_connection_t *c, u_char *p, u_char *last,
    ngx_uint_t type, ngx_str_t *মান)
{
    ngx_proxy_protocol_tlv_t  *tlv;

    যদি ((আকার_t) (last - p) < আকার(ngx_proxy_protocol_tlv_t) + মান->len) {
        ngx_log_error(NGX_LOG_ALERT, c->log, 0,
                      "too small buffer for PROXY protocol v2 TLV");
        ফেরত ফাঁকা;
    }

    tlv = (ngx_proxy_protocol_tlv_t *) p;
    tlv->type = (u_char) type;
    tlv->len[0] = (u_char) (মান->len >> 8);
    tlv->len[1] = (u_char)  মান->len;

    p += আকার(ngx_proxy_protocol_tlv_t);

    ফেরত ngx_cpymem(p, মান->তথ্য, মান->len);
}


স্থির u_char *
ngx_proxy_protocol_v2_write_ssl(ngx_connection_t *c, u_char *p, u_char *last,
    ngx_array_t *ssl_tlvs, ngx_uint_t client, uint32_t verify)
{
    u_char                          *start;
    uint16_t                         len;
    ngx_uint_t                       i;
    ngx_proxy_protocol_tlv_t        *tlv;
    ngx_proxy_protocol_tlv_ssl_t    *tlv_ssl;
    ngx_proxy_protocol_tlv_মান_t  *sub;

    যদি ((আকার_t) (last - p) < আকার(ngx_proxy_protocol_tlv_t)
                              + আকার(ngx_proxy_protocol_tlv_ssl_t))
    {
        ngx_log_error(NGX_LOG_ALERT, c->log, 0,
                      "too small buffer for PROXY protocol v2 SSL TLV");
        ফেরত ফাঁকা;
    }

    start = p;
    p += আকার(ngx_proxy_protocol_tlv_t);

    tlv_ssl = (ngx_proxy_protocol_tlv_ssl_t *) p;
    tlv_ssl->client = (u_char) client;
    tlv_ssl->verify[0] = (u_char) (verify >> 24);
    tlv_ssl->verify[1] = (u_char) (verify >> 16);
    tlv_ssl->verify[2] = (u_char) (verify >> 8);
    tlv_ssl->verify[3] = (u_char)  verify;

    p += আকার(ngx_proxy_protocol_tlv_ssl_t);

    sub = ssl_tlvs->elts;

    জন্য (i = 0; i < ssl_tlvs->nelts; i++) {
        p = ngx_proxy_protocol_v2_write_tlv(c, p, last, sub[i].type,
                                            &sub[i].মান);
        যদি (p == ফাঁকা) {
            ফেরত ফাঁকা;
        }
    }

    len = (uint16_t) (p - start - আকার(ngx_proxy_protocol_tlv_t));

    tlv = (ngx_proxy_protocol_tlv_t *) start;
    tlv->type = NGX_PROXY_PROTOCOL_TLV_SSL;
    tlv->len[0] = (u_char) (len >> 8);
    tlv->len[1] = (u_char)  len;

    ফেরত p;
}


স্থির শূন্য
ngx_proxy_protocol_v2_set_len(u_char *buf, u_char *p)
{
    uint16_t                      len;
    ngx_proxy_protocol_header_t  *header;

    len = (uint16_t) (p - buf - আকার(ngx_proxy_protocol_header_t));

    header = (ngx_proxy_protocol_header_t *) buf;
    header->len[0] = (u_char) (len >> 8);
    header->len[1] = (u_char) len;
}


স্থির u_char *
ngx_proxy_protocol_v2_write_crc32c(ngx_connection_t *c, u_char *buf,
    u_char *p, u_char *last)
{
    u_char                    *মান;
    uint32_t                   crc;
    ngx_proxy_protocol_tlv_t  *tlv;

    যদি ((আকার_t) (last - p) < আকার(ngx_proxy_protocol_tlv_t) + 4) {
        ngx_log_error(NGX_LOG_ALERT, c->log, 0,
                      "too small buffer for PROXY protocol v2 CRC32c TLV");
        ফেরত ফাঁকা;
    }

    /* append the CRC32c TLV with a zeroed value field */

    tlv = (ngx_proxy_protocol_tlv_t *) p;
    tlv->type = NGX_PROXY_PROTOCOL_TLV_CRC32C;
    tlv->len[0] = 0;
    tlv->len[1] = 4;

    মান = p + আকার(ngx_proxy_protocol_tlv_t);

    ngx_memzero(মান, 4);

    p = মান + 4;

    /* the checksum covers the entire header with the zeroed value */

    crc = ngx_crc32c_long(buf, p - buf);

    মান[0] = (u_char) (crc >> 24);
    মান[1] = (u_char) (crc >> 16);
    মান[2] = (u_char) (crc >> 8);
    মান[3] = (u_char) crc;

    ফেরত p;
}
