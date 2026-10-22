
/*
 * Copyright (C) Roman Arutyunyan
 * Copyright (C) Nginx, Inc.
 */


#અન્તર્ભાવ <ngx_config.h>
#અન્તર્ભાવ <ngx_core.h>


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
#define NGX_PROXY_PROTOCOL_TLV_SSL_ચાવી_ALG  0x25
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


પ્રકારવ્યાખ્યા સંરચના {
    u_char                                  signature[12];
    u_char                                  version_command;
    u_char                                  family_transport;
    u_char                                  len[2];
} ngx_proxy_protocol_header_t;


પ્રકારવ્યાખ્યા સંરચના {
    u_char                                  src_addr[4];
    u_char                                  dst_addr[4];
    u_char                                  src_port[2];
    u_char                                  dst_port[2];
} ngx_proxy_protocol_inet_addrs_t;


પ્રકારવ્યાખ્યા સંરચના {
    u_char                                  src_addr[16];
    u_char                                  dst_addr[16];
    u_char                                  src_port[2];
    u_char                                  dst_port[2];
} ngx_proxy_protocol_inet6_addrs_t;


પ્રકારવ્યાખ્યા સંરચના {
    u_char                                  type;
    u_char                                  len[2];
} ngx_proxy_protocol_tlv_t;


પ્રકારવ્યાખ્યા સંરચના {
    u_char                                  client;
    u_char                                  verify[4];
} ngx_proxy_protocol_tlv_ssl_t;


પ્રકારવ્યાખ્યા સંરચના {
    ngx_str_t                               name;
    ngx_uint_t                              type;
} ngx_proxy_protocol_tlv_entry_t;


પ્રકારવ્યાખ્યા સંરચના {
    ngx_uint_t                              type;
    ngx_str_t                               કિંમત;
} ngx_proxy_protocol_tlv_કિંમત_t;


સ્થિર u_char *ngx_proxy_protocol_read_addr(ngx_connection_t *c, u_char *p,
    u_char *last, ngx_str_t *addr);
સ્થિર u_char *ngx_proxy_protocol_read_port(u_char *p, u_char *last,
    in_port_t *port, u_char sep);
સ્થિર u_char *ngx_proxy_protocol_v2_read(ngx_connection_t *c, u_char *buf,
    u_char *last);
સ્થિર ngx_int_t ngx_proxy_protocol_lookup_tlv(ngx_connection_t *c,
    ngx_str_t *tlvs, ngx_uint_t type, ngx_str_t *કિંમત);
#જો (NGX_OPENSSL)
સ્થિર ngx_int_t ngx_proxy_protocol_v2_eval_ssl(ngx_connection_t *c,
    ngx_array_t *tlvs, ngx_array_t *ssl_tlvs, ngx_uint_t *client,
    uint32_t *verify);
સ્થિર ngx_int_t ngx_proxy_protocol_v2_authority(ngx_connection_t *c,
    ngx_str_t *out);
સ્થિર ngx_int_t ngx_proxy_protocol_v2_alpn(ngx_connection_t *c,
    ngx_str_t *out);
સ્થિર ngx_int_t ngx_proxy_protocol_v2_ssl_sub(ngx_connection_t *c,
    ngx_uint_t type, ngx_str_t *out);
#endif
સ્થિર u_char *ngx_proxy_protocol_v2_write_header(ngx_connection_t *c,
    u_char *buf, u_char *last);
સ્થિર u_char ngx_proxy_protocol_v2_family(ngx_uint_t family);
સ્થિર શૂન્ય ngx_proxy_protocol_v2_write_ipv4(સંરચના sockaddr *sa, u_char *addr,
    u_char *port);
સ્થિર શૂન્ય ngx_proxy_protocol_v2_write_ipv6(સંરચના sockaddr *sa, u_char *addr,
    u_char *port);
સ્થિર u_char *ngx_proxy_protocol_v2_write_tlv(ngx_connection_t *c, u_char *p,
    u_char *last, ngx_uint_t type, ngx_str_t *કિંમત);
સ્થિર u_char *ngx_proxy_protocol_v2_write_ssl(ngx_connection_t *c, u_char *p,
    u_char *last, ngx_array_t *ssl_tlvs, ngx_uint_t client, uint32_t verify);
સ્થિર શૂન્ય ngx_proxy_protocol_v2_set_len(u_char *buf, u_char *p);
સ્થિર u_char *ngx_proxy_protocol_v2_write_crc32c(ngx_connection_t *c,
    u_char *buf, u_char *p, u_char *last);


સ્થિર ngx_proxy_protocol_tlv_entry_t  ngx_proxy_protocol_tlv_entries[] = {
    { ngx_string("alpn"),       NGX_PROXY_PROTOCOL_TLV_ALPN },
    { ngx_string("authority"),  NGX_PROXY_PROTOCOL_TLV_AUTHORITY },
    { ngx_string("unique_id"),  NGX_PROXY_PROTOCOL_TLV_UNIQUE_ID },
    { ngx_string("ssl"),        NGX_PROXY_PROTOCOL_TLV_SSL },
    { ngx_string("netns"),      NGX_PROXY_PROTOCOL_TLV_NETNS },
    { ngx_null_string,          0x00 }
};


સ્થિર ngx_proxy_protocol_tlv_entry_t  ngx_proxy_protocol_tlv_ssl_entries[] = {
    { ngx_string("version"),    NGX_PROXY_PROTOCOL_TLV_SSL_VERSION },
    { ngx_string("cn"),         NGX_PROXY_PROTOCOL_TLV_SSL_CN },
    { ngx_string("cipher"),     NGX_PROXY_PROTOCOL_TLV_SSL_CIPHER },
    { ngx_string("sig_alg"),    NGX_PROXY_PROTOCOL_TLV_SSL_SIG_ALG },
    { ngx_string("key_alg"),    NGX_PROXY_PROTOCOL_TLV_SSL_ચાવી_ALG },
    { ngx_null_string,          0x00 }
};


સ્થિર અચલ u_char  ngx_proxy_protocol_signature[] = "\r\n\r\n\0\r\nQUIT\n";


u_char *
ngx_proxy_protocol_read(ngx_connection_t *c, u_char *buf, u_char *last)
{
    size_t                 len;
    u_char                *p;
    ngx_proxy_protocol_t  *pp;

    p = buf;
    len = last - buf;

    જો (len >= કદ(ngx_proxy_protocol_header_t)
        && ngx_memcmp(p, ngx_proxy_protocol_signature,
                      કદ(ngx_proxy_protocol_signature) - 1)
           == 0)
    {
        પાછા ngx_proxy_protocol_v2_read(c, buf, last);
    }

    જો (len < 8 || ngx_strncmp(p, "PROXY ", 6) != 0) {
        જાઓ invalid;
    }

    p += 6;
    len -= 6;

    જો (len >= 7 && ngx_strncmp(p, "UNKNOWN", 7) == 0) {
        ngx_log_debug0(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "PROXY protocol unknown protocol");
        p += 7;
        જાઓ skip;
    }

    જો (len < 5 || ngx_strncmp(p, "TCP", 3) != 0
        || (p[3] != '4' && p[3] != '6') || p[4] != ' ')
    {
        જાઓ invalid;
    }

    p += 5;

    pp = ngx_pcalloc(c->pool, કદ(ngx_proxy_protocol_t));
    જો (pp == ખાલી) {
        પાછા ખાલી;
    }

    p = ngx_proxy_protocol_read_addr(c, p, last, &pp->src_addr);
    જો (p == ખાલી) {
        જાઓ invalid;
    }

    p = ngx_proxy_protocol_read_addr(c, p, last, &pp->dst_addr);
    જો (p == ખાલી) {
        જાઓ invalid;
    }

    p = ngx_proxy_protocol_read_port(p, last, &pp->src_port, ' ');
    જો (p == ખાલી) {
        જાઓ invalid;
    }

    p = ngx_proxy_protocol_read_port(p, last, &pp->dst_port, CR);
    જો (p == ખાલી) {
        જાઓ invalid;
    }

    જો (p == last) {
        જાઓ invalid;
    }

    જો (*p++ != LF) {
        જાઓ invalid;
    }

    ngx_log_debug4(NGX_LOG_DEBUG_CORE, c->log, 0,
                   "PROXY protocol src: %V %d, dst: %V %d",
                   &pp->src_addr, pp->src_port, &pp->dst_addr, pp->dst_port);

    c->proxy_protocol = pp;

    પાછા p;

skip:

    માટે ( /* void */ ; p < last - 1; p++) {
        જો (p[0] == CR && p[1] == LF) {
            પાછા p + 2;
        }
    }

invalid:

    માટે (p = buf; p < last; p++) {
        જો (*p == CR || *p == LF) {
            તોડો;
        }
    }

    ngx_log_error(NGX_LOG_ERR, c->log, 0,
                  "broken header: \"%*s\"", (size_t) (p - buf), buf);

    પાછા ખાલી;
}


સ્થિર u_char *
ngx_proxy_protocol_read_addr(ngx_connection_t *c, u_char *p, u_char *last,
    ngx_str_t *addr)
{
    size_t  len;
    u_char  ch, *pos;

    pos = p;

    માટે ( ;; ) {
        જો (p == last) {
            પાછા ખાલી;
        }

        ch = *p++;

        જો (ch == ' ') {
            તોડો;
        }

        જો (ch != ':' && ch != '.'
            && (ch < 'a' || ch > 'f')
            && (ch < 'A' || ch > 'F')
            && (ch < '0' || ch > '9'))
        {
            પાછા ખાલી;
        }
    }

    len = p - pos - 1;

    addr->data = ngx_pnalloc(c->pool, len);
    જો (addr->data == ખાલી) {
        પાછા ખાલી;
    }

    ngx_memcpy(addr->data, pos, len);
    addr->len = len;

    પાછા p;
}


સ્થિર u_char *
ngx_proxy_protocol_read_port(u_char *p, u_char *last, in_port_t *port,
    u_char sep)
{
    size_t      len;
    u_char     *pos;
    ngx_int_t   n;

    pos = p;

    માટે ( ;; ) {
        જો (p == last) {
            પાછા ખાલી;
        }

        જો (*p++ == sep) {
            તોડો;
        }
    }

    len = p - pos - 1;

    n = ngx_atoi(pos, len);
    જો (n < 0 || n > 65535) {
        પાછા ખાલી;
    }

    *port = (in_port_t) n;

    પાછા p;
}


u_char *
ngx_proxy_protocol_write(ngx_connection_t *c, u_char *buf, u_char *last)
{
    ngx_uint_t  port, lport;

    જો (last - buf < NGX_PROXY_PROTOCOL_V1_MAX_HEADER) {
        ngx_log_error(NGX_LOG_ALERT, c->log, 0,
                      "too small buffer for PROXY protocol");
        પાછા ખાલી;
    }

    જો (ngx_connection_local_sockaddr(c, ખાલી, 0) != NGX_OK) {
        પાછા ખાલી;
    }

    પસંદગી (c->sockaddr->sa_family) {

    સ્થિતિ AF_INET:
        buf = ngx_cpymem(buf, "PROXY TCP4 ", કદ("PROXY TCP4 ") - 1);
        તોડો;

#જો (NGX_HAVE_INET6)
    સ્થિતિ AF_INET6:
        buf = ngx_cpymem(buf, "PROXY TCP6 ", કદ("PROXY TCP6 ") - 1);
        તોડો;
#endif

    મૂળભૂત:
        પાછા ngx_cpymem(buf, "PROXY UNKNOWN" CRLF,
                          કદ("PROXY UNKNOWN" CRLF) - 1);
    }

    buf += ngx_sock_ntop(c->sockaddr, c->socklen, buf, last - buf, 0);

    *buf++ = ' ';

    buf += ngx_sock_ntop(c->local_sockaddr, c->local_socklen, buf, last - buf,
                         0);

    port = ngx_inet_get_port(c->sockaddr);
    lport = ngx_inet_get_port(c->local_sockaddr);

    પાછા ngx_slprintf(buf, last, " %ui %ui" CRLF, port, lport);
}


સ્થિર u_char *
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
#જો (NGX_HAVE_INET6)
    ngx_proxy_protocol_inet6_addrs_t   *in6;
#endif

    header = (ngx_proxy_protocol_header_t *) buf;

    buf += કદ(ngx_proxy_protocol_header_t);

    version = header->version_command >> 4;

    જો (version != 2) {
        ngx_log_error(NGX_LOG_ERR, c->log, 0,
                      "unknown PROXY protocol version: %ui", version);
        પાછા ખાલી;
    }

    len = ngx_proxy_protocol_parse_uint16(header->len);

    જો ((size_t) (last - buf) < len) {
        ngx_log_error(NGX_LOG_ERR, c->log, 0, "header is too large");
        પાછા ખાલી;
    }

    end = buf + len;

    command = header->version_command & 0x0f;

    /* only PROXY is supported */
    જો (command != NGX_PROXY_PROTOCOL_CMD_PROXY) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "PROXY protocol v2 unsupported command %ui", command);
        પાછા end;
    }

    transport = header->family_transport & 0x0f;

    /* only STREAM is supported */
    જો (transport != NGX_PROXY_PROTOCOL_TYPE_STREAM) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "PROXY protocol v2 unsupported transport %ui",
                       transport);
        પાછા end;
    }

    pp = ngx_pcalloc(c->pool, કદ(ngx_proxy_protocol_t));
    જો (pp == ખાલી) {
        પાછા ખાલી;
    }

    family = header->family_transport >> 4;

    પસંદગી (family) {

    સ્થિતિ NGX_PROXY_PROTOCOL_AF_INET:

        જો ((size_t) (end - buf) < કદ(ngx_proxy_protocol_inet_addrs_t)) {
            પાછા ખાલી;
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

        socklen = કદ(સંરચના sockaddr_in);

        buf += કદ(ngx_proxy_protocol_inet_addrs_t);

        તોડો;

#જો (NGX_HAVE_INET6)

    સ્થિતિ NGX_PROXY_PROTOCOL_AF_INET6:

        જો ((size_t) (end - buf) < કદ(ngx_proxy_protocol_inet6_addrs_t)) {
            પાછા ખાલી;
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

        socklen = કદ(સંરચના sockaddr_in6);

        buf += કદ(ngx_proxy_protocol_inet6_addrs_t);

        તોડો;

#endif

    મૂળભૂત:
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "PROXY protocol v2 unsupported address family %ui",
                       family);
        પાછા end;
    }

    pp->src_addr.data = ngx_pnalloc(c->pool, NGX_SOCKADDR_STRLEN);
    જો (pp->src_addr.data == ખાલી) {
        પાછા ખાલી;
    }

    pp->src_addr.len = ngx_sock_ntop(&src_sockaddr.sockaddr, socklen,
                                     pp->src_addr.data, NGX_SOCKADDR_STRLEN, 0);

    pp->dst_addr.data = ngx_pnalloc(c->pool, NGX_SOCKADDR_STRLEN);
    જો (pp->dst_addr.data == ખાલી) {
        પાછા ખાલી;
    }

    pp->dst_addr.len = ngx_sock_ntop(&dst_sockaddr.sockaddr, socklen,
                                     pp->dst_addr.data, NGX_SOCKADDR_STRLEN, 0);

    ngx_log_debug4(NGX_LOG_DEBUG_CORE, c->log, 0,
                   "PROXY protocol v2 src: %V %d, dst: %V %d",
                   &pp->src_addr, pp->src_port, &pp->dst_addr, pp->dst_port);

    જો (buf < end) {
        pp->tlvs.data = ngx_pnalloc(c->pool, end - buf);
        જો (pp->tlvs.data == ખાલી) {
            પાછા ખાલી;
        }

        ngx_memcpy(pp->tlvs.data, buf, end - buf);
        pp->tlvs.len = end - buf;
    }

    c->proxy_protocol = pp;

    પાછા end;
}


ngx_int_t
ngx_proxy_protocol_get_tlv(ngx_connection_t *c, ngx_str_t *name,
    ngx_str_t *કિંમત)
{
    u_char                          *p;
    size_t                           n;
    uint32_t                         verify;
    ngx_str_t                        ssl, *tlvs;
    ngx_int_t                        rc, type;
    ngx_proxy_protocol_tlv_ssl_t    *tlv_ssl;
    ngx_proxy_protocol_tlv_entry_t  *te;

    જો (c->proxy_protocol == ખાલી) {
        પાછા NGX_DECLINED;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                   "PROXY protocol v2 get tlv \"%V\"", name);

    te = ngx_proxy_protocol_tlv_entries;
    tlvs = &c->proxy_protocol->tlvs;

    p = name->data;
    n = name->len;

    જો (n >= 4 && p[0] == 's' && p[1] == 's' && p[2] == 'l' && p[3] == '_') {

        rc = ngx_proxy_protocol_lookup_tlv(c, tlvs,
                                           NGX_PROXY_PROTOCOL_TLV_SSL, &ssl);
        જો (rc != NGX_OK) {
            પાછા rc;
        }

        જો (ssl.len < કદ(ngx_proxy_protocol_tlv_ssl_t)) {
            પાછા NGX_ERROR;
        }

        p += 4;
        n -= 4;

        જો (n == 6 && ngx_strncmp(p, "verify", 6) == 0) {

            tlv_ssl = (ngx_proxy_protocol_tlv_ssl_t *) ssl.data;
            verify = ngx_proxy_protocol_parse_uint32(tlv_ssl->verify);

            કિંમત->data = ngx_pnalloc(c->pool, NGX_INT32_LEN);
            જો (કિંમત->data == ખાલી) {
                પાછા NGX_ERROR;
            }

            કિંમત->len = ngx_sprintf(કિંમત->data, "%uD", verify)
                         - કિંમત->data;
            પાછા NGX_OK;
        }

        ssl.data += કદ(ngx_proxy_protocol_tlv_ssl_t);
        ssl.len -= કદ(ngx_proxy_protocol_tlv_ssl_t);

        te = ngx_proxy_protocol_tlv_ssl_entries;
        tlvs = &ssl;
    }

    જો (n >= 2 && p[0] == '0' && p[1] == 'x') {

        type = ngx_hextoi(p + 2, n - 2);
        જો (type == NGX_ERROR) {
            ngx_log_error(NGX_LOG_ERR, c->log, 0,
                          "invalid PROXY protocol TLV \"%V\"", name);
            પાછા NGX_ERROR;
        }

        પાછા ngx_proxy_protocol_lookup_tlv(c, tlvs, type, કિંમત);
    }

    માટે ( /* void */ ; te->type; te++) {
        જો (te->name.len == n && ngx_strncmp(te->name.data, p, n) == 0) {
            પાછા ngx_proxy_protocol_lookup_tlv(c, tlvs, te->type, કિંમત);
        }
    }

    ngx_log_error(NGX_LOG_ERR, c->log, 0,
                  "unknown PROXY protocol TLV \"%V\"", name);

    પાછા NGX_DECLINED;
}


સ્થિર ngx_int_t
ngx_proxy_protocol_lookup_tlv(ngx_connection_t *c, ngx_str_t *tlvs,
    ngx_uint_t type, ngx_str_t *કિંમત)
{
    u_char                    *p;
    size_t                     n, len;
    ngx_proxy_protocol_tlv_t  *tlv;

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                   "PROXY protocol v2 lookup tlv:%02xi", type);

    p = tlvs->data;
    n = tlvs->len;

    જ્યારે (n) {
        જો (n < કદ(ngx_proxy_protocol_tlv_t)) {
            ngx_log_error(NGX_LOG_ERR, c->log, 0, "broken PROXY protocol TLV");
            પાછા NGX_ERROR;
        }

        tlv = (ngx_proxy_protocol_tlv_t *) p;
        len = ngx_proxy_protocol_parse_uint16(tlv->len);

        p += કદ(ngx_proxy_protocol_tlv_t);
        n -= કદ(ngx_proxy_protocol_tlv_t);

        જો (n < len) {
            ngx_log_error(NGX_LOG_ERR, c->log, 0, "broken PROXY protocol TLV");
            પાછા NGX_ERROR;
        }

        જો (tlv->type == type) {
            કિંમત->data = p;
            કિંમત->len = len;
            પાછા NGX_OK;
        }

        p += len;
        n -= len;
    }

    પાછા NGX_DECLINED;
}


u_char *
ngx_proxy_protocol_v2_write(ngx_connection_t *c, u_char *buf, u_char *last,
    ngx_array_t *tlvs /* reserved */ )
{
    u_char                          *p;
    uint32_t                         verify;
    ngx_uint_t                       i, client;
    ngx_array_t                      base_tlvs, ssl_tlvs;
    ngx_proxy_protocol_tlv_કિંમત_t  *tlv;

    client = 0;
    verify = 0;

    ngx_memzero(&base_tlvs, કદ(ngx_array_t));
    ngx_memzero(&ssl_tlvs, કદ(ngx_array_t));

#જો (NGX_OPENSSL)

    જો (c->ssl != ખાલી) {
        જો (ngx_array_init(&base_tlvs, c->pool, 2,
                           કદ(ngx_proxy_protocol_tlv_કિંમત_t))
            != NGX_OK)
        {
            પાછા ખાલી;
        }

        જો (ngx_array_init(&ssl_tlvs, c->pool, 5,
                           કદ(ngx_proxy_protocol_tlv_કિંમત_t))
            != NGX_OK)
        {
            પાછા ખાલી;
        }

        જો (ngx_proxy_protocol_v2_eval_ssl(c, &base_tlvs, &ssl_tlvs, &client,
                                           &verify)
            != NGX_OK)
        {
            પાછા ખાલી;
        }
    }

#endif

    p = ngx_proxy_protocol_v2_write_header(c, buf, last);
    જો (p == ખાલી) {
        પાછા ખાલી;
    }

    tlv = base_tlvs.elts;

    માટે (i = 0; i < base_tlvs.nelts; i++) {
        p = ngx_proxy_protocol_v2_write_tlv(c, p, last, tlv[i].type,
                                           &tlv[i].કિંમત);
        જો (p == ખાલી) {
            પાછા ખાલી;
        }
    }

    જો (client) {
        p = ngx_proxy_protocol_v2_write_ssl(c, p, last, &ssl_tlvs, client,
                                            verify);
        જો (p == ખાલી) {
            પાછા ખાલી;
        }
    }

    ngx_proxy_protocol_v2_set_len(buf,
                                  p + કદ(ngx_proxy_protocol_tlv_t) + 4);

    પાછા ngx_proxy_protocol_v2_write_crc32c(c, buf, p, last);
}


#જો (NGX_OPENSSL)

સ્થિર ngx_int_t
ngx_proxy_protocol_v2_eval_ssl(ngx_connection_t *c, ngx_array_t *tlvs,
    ngx_array_t *ssl_tlvs, ngx_uint_t *client, uint32_t *verify)
{
    X509                            *cert;
    long                             n;
    ngx_int_t                        rc;
    ngx_str_t                        કિંમત;
    ngx_uint_t                       type;
    અચલ અક્ષર                      *s;
    ngx_proxy_protocol_tlv_કિંમત_t  *tlv;

    જો (ngx_proxy_protocol_v2_authority(c, &કિંમત) == NGX_OK) {
        tlv = ngx_array_ધકેલો(tlvs);
        જો (tlv == ખાલી) {
            પાછા NGX_ERROR;
        }

        tlv->type = NGX_PROXY_PROTOCOL_TLV_AUTHORITY;
        tlv->કિંમત = કિંમત;
    }

    જો (ngx_proxy_protocol_v2_alpn(c, &કિંમત) == NGX_OK) {
        tlv = ngx_array_ધકેલો(tlvs);
        જો (tlv == ખાલી) {
            પાછા NGX_ERROR;
        }

        tlv->type = NGX_PROXY_PROTOCOL_TLV_ALPN;
        tlv->કિંમત = કિંમત;
    }

    માટે (type = NGX_PROXY_PROTOCOL_TLV_SSL_VERSION;
         type <= NGX_PROXY_PROTOCOL_TLV_SSL_ચાવી_ALG;
         type++)
    {
        rc = ngx_proxy_protocol_v2_ssl_sub(c, type, &કિંમત);

        જો (rc == NGX_ERROR) {
            પાછા NGX_ERROR;
        }

        જો (rc != NGX_OK) {
            ચાલુ;
        }

        tlv = ngx_array_ધકેલો(ssl_tlvs);
        જો (tlv == ખાલી) {
            પાછા NGX_ERROR;
        }

        tlv->type = type;
        tlv->કિંમત = કિંમત;
    }

    *client = NGX_PROXY_PROTOCOL_V2_CLIENT_SSL;
    *verify = 1;  /* X509_V_ERR_UNSPECIFIED */

    cert = SSL_get_peer_certificate(c->ssl->connection);

    જો (cert != ખાલી) {
        X509_free(cert);

        *client |= NGX_PROXY_PROTOCOL_V2_CLIENT_CERT_SESS;

        જો (!SSL_session_reused(c->ssl->connection)) {
            *client |= NGX_PROXY_PROTOCOL_V2_CLIENT_CERT_CONN;
        }

        n = SSL_get_verify_result(c->ssl->connection);

        જો (n == X509_V_OK) {

            જો (ngx_ssl_ocsp_get_status(c, &s) != NGX_OK) {
                n = X509_V_ERR_CERT_REVOKED;
            }
        }

        *verify = (uint32_t) n;
    }

    પાછા NGX_OK;
}


સ્થિર ngx_int_t
ngx_proxy_protocol_v2_authority(ngx_connection_t *c, ngx_str_t *out)
{
#ifdef SSL_CTRL_SET_TLSEXT_HOSTNAME

    અચલ અક્ષર  *sni;

    sni = SSL_get_servername(c->ssl->connection, TLSEXT_NAMETYPE_host_name);
    જો (sni == ખાલી) {
        પાછા NGX_DECLINED;
    }

    out->data = (u_char *) sni;
    out->len = ngx_strlen(sni);
    પાછા NGX_OK;

#નહીંતર
    પાછા NGX_DECLINED;
#endif
}


સ્થિર ngx_int_t
ngx_proxy_protocol_v2_alpn(ngx_connection_t *c, ngx_str_t *out)
{
#ifdef TLSEXT_TYPE_application_layer_protocol_negotiation

    unsigned પૂર્ણાંક          alpnlen;
    અચલ unsigned અક્ષર  *alpn;

    SSL_get0_alpn_selected(c->ssl->connection, &alpn, &alpnlen);
    જો (alpn == ખાલી || alpnlen == 0) {
        પાછા NGX_DECLINED;
    }

    out->data = (u_char *) alpn;
    out->len = alpnlen;
    પાછા NGX_OK;

#નહીંતર
    પાછા NGX_DECLINED;
#endif
}


સ્થિર ngx_int_t
ngx_proxy_protocol_v2_ssl_sub(ngx_connection_t *c, ngx_uint_t type,
    ngx_str_t *out)
{
    પૂર્ણાંક               i, len;
    X509             *cert;
    u_char           *alg, *s;
    EVP_PKEY         *pkey;
#જો (OPENSSL_VERSION_સંખ્યા >= 0x40000000L)
    અચલ
#endif
    X509_NAME        *subject;
#જો (OPENSSL_VERSION_સંખ્યા >= 0x40000000L)
    અચલ
#endif
    X509_NAME_ENTRY  *entry;

    પસંદગી (type) {

    સ્થિતિ NGX_PROXY_PROTOCOL_TLV_SSL_VERSION:
        out->data = (u_char *) SSL_get_version(c->ssl->connection);
        out->len = ngx_strlen(out->data);
        પાછા NGX_OK;

    સ્થિતિ NGX_PROXY_PROTOCOL_TLV_SSL_CIPHER:
        out->data = (u_char *) SSL_get_cipher_name(c->ssl->connection);
        out->len = ngx_strlen(out->data);
        પાછા NGX_OK;

    સ્થિતિ NGX_PROXY_PROTOCOL_TLV_SSL_CN:
        cert = SSL_get_peer_certificate(c->ssl->connection);
        જો (cert == ખાલી) {
            પાછા NGX_DECLINED;
        }

        subject = X509_get_subject_name(cert);
        જો (subject == ખાલી) {
            X509_free(cert);
            પાછા NGX_ERROR;
        }

        i = X509_NAME_get_index_by_NID(subject, NID_commonName, -1);
        જો (i < 0) {
            X509_free(cert);
            પાછા NGX_DECLINED;
        }

        entry = X509_NAME_get_entry(subject, i);

        len = ASN1_STRING_to_UTF8(&s, X509_NAME_ENTRY_get_data(entry));
        જો (len < 0) {
            ngx_ssl_error(NGX_LOG_ALERT, c->log, 0,
                          "ASN1_STRING_to_UTF8() failed");
            X509_free(cert);
            પાછા NGX_ERROR;
        }

        out->len = len;
        out->data = ngx_pnalloc(c->pool, len);
        જો (out->data == ખાલી) {
            OPENSSL_free(s);
            X509_free(cert);
            પાછા NGX_ERROR;
        }

        ngx_memcpy(out->data, s, len);

        OPENSSL_free(s);
        X509_free(cert);
        પાછા NGX_OK;

    સ્થિતિ NGX_PROXY_PROTOCOL_TLV_SSL_SIG_ALG:
        cert = SSL_get_certificate(c->ssl->connection);

        out->data = (u_char *) OBJ_nid2sn(X509_get_signature_nid(cert));
        out->len = ngx_strlen(out->data);
        પાછા NGX_OK;

    સ્થિતિ NGX_PROXY_PROTOCOL_TLV_SSL_ચાવી_ALG:
        cert = SSL_get_certificate(c->ssl->connection);

        pkey = X509_get_pubkey(cert);
        જો (pkey == ખાલી) {
            પાછા NGX_ERROR;
        }

        પસંદગી (EVP_PKEY_base_id(pkey)) {

        સ્થિતિ EVP_PKEY_RSA:
#ifdef EVP_PKEY_RSA_PSS
        સ્થિતિ EVP_PKEY_RSA_PSS:
#endif
            alg = (u_char *) "RSA";
            તોડો;

        સ્થિતિ EVP_PKEY_EC:
            alg = (u_char *) "EC";
            તોડો;

        સ્થિતિ EVP_PKEY_DSA:
            alg = (u_char *) "DSA";
            તોડો;

        મૂળભૂત:
            EVP_PKEY_free(pkey);
            પાછા NGX_DECLINED;
        }

        out->len = કદ("RSA16384") - 1;
        out->data = ngx_pnalloc(c->pool, out->len);
        જો (out->data == ખાલી) {
            EVP_PKEY_free(pkey);
            પાછા NGX_ERROR;
        }

        out->len = ngx_snprintf(out->data, out->len, "%s%d",
                                alg, EVP_PKEY_bits(pkey))
                   - out->data;

        EVP_PKEY_free(pkey);

        પાછા NGX_OK;
    }

    પાછા NGX_DECLINED;
}

#endif


સ્થિર u_char *
ngx_proxy_protocol_v2_write_header(ngx_connection_t *c, u_char *buf,
    u_char *last)
{
    u_char                            *p, src_af, dst_af,
                                       family, command, transport;
    ngx_uint_t                         src_fam, dst_fam;
    ngx_proxy_protocol_header_t       *header;
    ngx_proxy_protocol_inet_addrs_t   *in;
#જો (NGX_HAVE_INET6)
    ngx_uint_t                         port, lport;
    સંરચના in6_addr                   *src6, *dst6;
    ngx_proxy_protocol_inet6_addrs_t  *in6;
#endif

    જો (last - buf < NGX_PROXY_PROTOCOL_V2_MAX_HEADER) {
        ngx_log_error(NGX_LOG_ALERT, c->log, 0,
                      "too small buffer for PROXY protocol v2");
        પાછા ખાલી;
    }

    જો (ngx_connection_local_sockaddr(c, ખાલી, 0) != NGX_OK) {
        પાછા ખાલી;
    }

    /* the length field is set by the caller once all TLVs are written */

    header = (ngx_proxy_protocol_header_t *) buf;

    ngx_memcpy(header->signature, ngx_proxy_protocol_signature,
               કદ(ngx_proxy_protocol_signature) - 1);

    p = buf + કદ(ngx_proxy_protocol_header_t);

    src_fam = c->sockaddr->sa_family;
    dst_fam = c->local_sockaddr->sa_family;

    src_af = ngx_proxy_protocol_v2_family(src_fam);
    dst_af = ngx_proxy_protocol_v2_family(dst_fam);

    /* promote to the highest address family present on either side */

    જો (src_af == NGX_PROXY_PROTOCOL_AF_UNSPEC
        || dst_af == NGX_PROXY_PROTOCOL_AF_UNSPEC)
    {
        command = NGX_PROXY_PROTOCOL_CMD_LOCAL;
        family = NGX_PROXY_PROTOCOL_AF_UNSPEC;
        transport = NGX_PROXY_PROTOCOL_TYPE_UNSPEC;

    } નહીંતર {
        command = NGX_PROXY_PROTOCOL_CMD_PROXY;
        family = ngx_max(src_af, dst_af);

        પસંદગી (c->type) {
        સ્થિતિ SOCK_STREAM:
            transport = NGX_PROXY_PROTOCOL_TYPE_STREAM;
            તોડો;
        સ્થિતિ SOCK_DGRAM:
            transport = NGX_PROXY_PROTOCOL_TYPE_DGRAM;
            તોડો;
        મૂળભૂત:
            transport = NGX_PROXY_PROTOCOL_TYPE_UNSPEC;
            તોડો;
        }
    }

    header->version_command = 0x20 | command;
    header->family_transport = (family << 4) | transport;

    પસંદગી (family) {

    સ્થિતિ NGX_PROXY_PROTOCOL_AF_INET:

        in = (ngx_proxy_protocol_inet_addrs_t *) p;

        ngx_proxy_protocol_v2_write_ipv4(c->sockaddr,
                                         in->src_addr, in->src_port);
        ngx_proxy_protocol_v2_write_ipv4(c->local_sockaddr,
                                         in->dst_addr, in->dst_port);

        p += કદ(ngx_proxy_protocol_inet_addrs_t);

        તોડો;

#જો (NGX_HAVE_INET6)

    સ્થિતિ NGX_PROXY_PROTOCOL_AF_INET6:

        જો (src_fam == AF_INET6 && dst_fam == AF_INET6) {
            src6 = &((સંરચના sockaddr_in6 *) c->sockaddr)->sin6_addr;
            dst6 = &((સંરચના sockaddr_in6 *) c->local_sockaddr)->sin6_addr;

            જો (IN6_IS_ADDR_V4MAPPED(src6) && IN6_IS_ADDR_V4MAPPED(dst6)) {

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

                p += કદ(ngx_proxy_protocol_inet_addrs_t);
                તોડો;
            }
        }

        in6 = (ngx_proxy_protocol_inet6_addrs_t *) p;

        ngx_proxy_protocol_v2_write_ipv6(c->sockaddr,
                                         in6->src_addr, in6->src_port);
        ngx_proxy_protocol_v2_write_ipv6(c->local_sockaddr,
                                         in6->dst_addr, in6->dst_port);

        p += કદ(ngx_proxy_protocol_inet6_addrs_t);

        તોડો;

#endif

    મૂળભૂત:  /* NGX_PROXY_PROTOCOL_AF_UNSPEC */
        તોડો;
    }

    પાછા p;
}


સ્થિર u_char
ngx_proxy_protocol_v2_family(ngx_uint_t family)
{
    પસંદગી (family) {

    સ્થિતિ AF_INET:
        પાછા NGX_PROXY_PROTOCOL_AF_INET;

#જો (NGX_HAVE_INET6)
    સ્થિતિ AF_INET6:
        પાછા NGX_PROXY_PROTOCOL_AF_INET6;
#endif

    મૂળભૂત:
        પાછા NGX_PROXY_PROTOCOL_AF_UNSPEC;
    }
}


સ્થિર શૂન્ય
ngx_proxy_protocol_v2_write_ipv4(સંરચના sockaddr *sa, u_char *addr,
    u_char *port)
{
    સંરચના sockaddr_in  *sin;

    sin = (સંરચના sockaddr_in *) sa;
    ngx_memcpy(addr, &sin->sin_addr, 4);
    ngx_memcpy(port, &sin->sin_port, 2);
}


#જો (NGX_HAVE_INET6)

સ્થિર શૂન્ય
ngx_proxy_protocol_v2_write_ipv6(સંરચના sockaddr *sa, u_char *addr,
    u_char *port)
{
    સંરચના sockaddr_in   *sin;
    સંરચના sockaddr_in6  *sin6;

    /* IPv4-mapped prefix: 80 zero bits + 16 one bits */
    સ્થિર અચલ u_char  mapped[]   = { 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0xff, 0xff };

    જો (sa->sa_family == AF_INET6) {
        sin6 = (સંરચના sockaddr_in6 *) sa;
        ngx_memcpy(addr, &sin6->sin6_addr, 16);
        ngx_memcpy(port, &sin6->sin6_port, 2);

    } નહીંતર {
        /* promote AF_INET to ::ffff:a.b.c.d */
        sin = (સંરચના sockaddr_in *) sa;
        ngx_memcpy(addr, mapped, કદ(mapped));
        ngx_memcpy(addr + કદ(mapped), &sin->sin_addr, 4);
        ngx_memcpy(port, &sin->sin_port, 2);
    }
}

#endif


સ્થિર u_char *
ngx_proxy_protocol_v2_write_tlv(ngx_connection_t *c, u_char *p, u_char *last,
    ngx_uint_t type, ngx_str_t *કિંમત)
{
    ngx_proxy_protocol_tlv_t  *tlv;

    જો ((size_t) (last - p) < કદ(ngx_proxy_protocol_tlv_t) + કિંમત->len) {
        ngx_log_error(NGX_LOG_ALERT, c->log, 0,
                      "too small buffer for PROXY protocol v2 TLV");
        પાછા ખાલી;
    }

    tlv = (ngx_proxy_protocol_tlv_t *) p;
    tlv->type = (u_char) type;
    tlv->len[0] = (u_char) (કિંમત->len >> 8);
    tlv->len[1] = (u_char)  કિંમત->len;

    p += કદ(ngx_proxy_protocol_tlv_t);

    પાછા ngx_cpymem(p, કિંમત->data, કિંમત->len);
}


સ્થિર u_char *
ngx_proxy_protocol_v2_write_ssl(ngx_connection_t *c, u_char *p, u_char *last,
    ngx_array_t *ssl_tlvs, ngx_uint_t client, uint32_t verify)
{
    u_char                          *start;
    uint16_t                         len;
    ngx_uint_t                       i;
    ngx_proxy_protocol_tlv_t        *tlv;
    ngx_proxy_protocol_tlv_ssl_t    *tlv_ssl;
    ngx_proxy_protocol_tlv_કિંમત_t  *sub;

    જો ((size_t) (last - p) < કદ(ngx_proxy_protocol_tlv_t)
                              + કદ(ngx_proxy_protocol_tlv_ssl_t))
    {
        ngx_log_error(NGX_LOG_ALERT, c->log, 0,
                      "too small buffer for PROXY protocol v2 SSL TLV");
        પાછા ખાલી;
    }

    start = p;
    p += કદ(ngx_proxy_protocol_tlv_t);

    tlv_ssl = (ngx_proxy_protocol_tlv_ssl_t *) p;
    tlv_ssl->client = (u_char) client;
    tlv_ssl->verify[0] = (u_char) (verify >> 24);
    tlv_ssl->verify[1] = (u_char) (verify >> 16);
    tlv_ssl->verify[2] = (u_char) (verify >> 8);
    tlv_ssl->verify[3] = (u_char)  verify;

    p += કદ(ngx_proxy_protocol_tlv_ssl_t);

    sub = ssl_tlvs->elts;

    માટે (i = 0; i < ssl_tlvs->nelts; i++) {
        p = ngx_proxy_protocol_v2_write_tlv(c, p, last, sub[i].type,
                                            &sub[i].કિંમત);
        જો (p == ખાલી) {
            પાછા ખાલી;
        }
    }

    len = (uint16_t) (p - start - કદ(ngx_proxy_protocol_tlv_t));

    tlv = (ngx_proxy_protocol_tlv_t *) start;
    tlv->type = NGX_PROXY_PROTOCOL_TLV_SSL;
    tlv->len[0] = (u_char) (len >> 8);
    tlv->len[1] = (u_char)  len;

    પાછા p;
}


સ્થિર શૂન્ય
ngx_proxy_protocol_v2_set_len(u_char *buf, u_char *p)
{
    uint16_t                      len;
    ngx_proxy_protocol_header_t  *header;

    len = (uint16_t) (p - buf - કદ(ngx_proxy_protocol_header_t));

    header = (ngx_proxy_protocol_header_t *) buf;
    header->len[0] = (u_char) (len >> 8);
    header->len[1] = (u_char) len;
}


સ્થિર u_char *
ngx_proxy_protocol_v2_write_crc32c(ngx_connection_t *c, u_char *buf,
    u_char *p, u_char *last)
{
    u_char                    *કિંમત;
    uint32_t                   crc;
    ngx_proxy_protocol_tlv_t  *tlv;

    જો ((size_t) (last - p) < કદ(ngx_proxy_protocol_tlv_t) + 4) {
        ngx_log_error(NGX_LOG_ALERT, c->log, 0,
                      "too small buffer for PROXY protocol v2 CRC32c TLV");
        પાછા ખાલી;
    }

    /* append the CRC32c TLV with a zeroed value field */

    tlv = (ngx_proxy_protocol_tlv_t *) p;
    tlv->type = NGX_PROXY_PROTOCOL_TLV_CRC32C;
    tlv->len[0] = 0;
    tlv->len[1] = 4;

    કિંમત = p + કદ(ngx_proxy_protocol_tlv_t);

    ngx_memzero(કિંમત, 4);

    p = કિંમત + 4;

    /* the checksum covers the entire header with the zeroed value */

    crc = ngx_crc32c_long(buf, p - buf);

    કિંમત[0] = (u_char) (crc >> 24);
    કિંમત[1] = (u_char) (crc >> 16);
    કિંમત[2] = (u_char) (crc >> 8);
    કિંમત[3] = (u_char) crc;

    પાછા p;
}
