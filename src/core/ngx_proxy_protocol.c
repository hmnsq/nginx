
/*
 * Copyright (C) Roman Arutyunyan
 * Copyright (C) Nginx, Inc.
 */


#समावेश <ngx_config.h>
#समावेश <ngx_core.h>


#घोषणा NGX_PROXY_PROTOCOL_V2_MAX_HEADER    52

#घोषणा NGX_PROXY_PROTOCOL_CMD_LOCAL        0
#घोषणा NGX_PROXY_PROTOCOL_CMD_PROXY        1

#घोषणा NGX_PROXY_PROTOCOL_AF_UNSPEC        0
#घोषणा NGX_PROXY_PROTOCOL_AF_INET          1
#घोषणा NGX_PROXY_PROTOCOL_AF_INET6         2

#घोषणा NGX_PROXY_PROTOCOL_TYPE_UNSPEC      0
#घोषणा NGX_PROXY_PROTOCOL_TYPE_STREAM      1
#घोषणा NGX_PROXY_PROTOCOL_TYPE_DGRAM       2

#घोषणा NGX_PROXY_PROTOCOL_TLV_ALPN         0x01
#घोषणा NGX_PROXY_PROTOCOL_TLV_AUTHORITY    0x02
#घोषणा NGX_PROXY_PROTOCOL_TLV_CRC32C       0x03
#घोषणा NGX_PROXY_PROTOCOL_TLV_UNIQUE_ID    0x05
#घोषणा NGX_PROXY_PROTOCOL_TLV_SSL          0x20
#घोषणा NGX_PROXY_PROTOCOL_TLV_SSL_VERSION  0x21
#घोषणा NGX_PROXY_PROTOCOL_TLV_SSL_CN       0x22
#घोषणा NGX_PROXY_PROTOCOL_TLV_SSL_CIPHER   0x23
#घोषणा NGX_PROXY_PROTOCOL_TLV_SSL_SIG_ALG  0x24
#घोषणा NGX_PROXY_PROTOCOL_TLV_SSL_कुंजी_ALG  0x25
#घोषणा NGX_PROXY_PROTOCOL_TLV_NETNS        0x30

#घोषणा NGX_PROXY_PROTOCOL_V2_CLIENT_SSL          0x01
#घोषणा NGX_PROXY_PROTOCOL_V2_CLIENT_CERT_CONN    0x02
#घोषणा NGX_PROXY_PROTOCOL_V2_CLIENT_CERT_SESS    0x04


#घोषणा ngx_proxy_protocol_parse_uint16(p)                                    \
    ( ((uint16_t) (p)[0] << 8)                                                \
    + (           (p)[1]) )

#घोषणा ngx_proxy_protocol_parse_uint32(p)                                    \
    ( ((uint32_t) (p)[0] << 24)                                               \
    + (           (p)[1] << 16)                                               \
    + (           (p)[2] << 8)                                                \
    + (           (p)[3]) )


प्रकार काष्ठा {
    u_char                                  signature[12];
    u_char                                  version_command;
    u_char                                  family_transport;
    u_char                                  len[2];
} ngx_proxy_protocol_header_t;


प्रकार काष्ठा {
    u_char                                  src_addr[4];
    u_char                                  dst_addr[4];
    u_char                                  src_port[2];
    u_char                                  dst_port[2];
} ngx_proxy_protocol_inet_addrs_t;


प्रकार काष्ठा {
    u_char                                  src_addr[16];
    u_char                                  dst_addr[16];
    u_char                                  src_port[2];
    u_char                                  dst_port[2];
} ngx_proxy_protocol_inet6_addrs_t;


प्रकार काष्ठा {
    u_char                                  type;
    u_char                                  len[2];
} ngx_proxy_protocol_tlv_t;


प्रकार काष्ठा {
    u_char                                  client;
    u_char                                  verify[4];
} ngx_proxy_protocol_tlv_ssl_t;


प्रकार काष्ठा {
    ngx_str_t                               name;
    ngx_uint_t                              type;
} ngx_proxy_protocol_tlv_entry_t;


प्रकार काष्ठा {
    ngx_uint_t                              type;
    ngx_str_t                               मूल्य;
} ngx_proxy_protocol_tlv_मूल्य_t;


अटल u_char *ngx_proxy_protocol_read_addr(ngx_connection_t *c, u_char *p,
    u_char *last, ngx_str_t *addr);
अटल u_char *ngx_proxy_protocol_read_port(u_char *p, u_char *last,
    in_port_t *port, u_char sep);
अटल u_char *ngx_proxy_protocol_v2_read(ngx_connection_t *c, u_char *buf,
    u_char *last);
अटल ngx_int_t ngx_proxy_protocol_lookup_tlv(ngx_connection_t *c,
    ngx_str_t *tlvs, ngx_uint_t type, ngx_str_t *मूल्य);
#अगर (NGX_OPENSSL)
अटल ngx_int_t ngx_proxy_protocol_v2_eval_ssl(ngx_connection_t *c,
    ngx_array_t *tlvs, ngx_array_t *ssl_tlvs, ngx_uint_t *client,
    uint32_t *verify);
अटल ngx_int_t ngx_proxy_protocol_v2_authority(ngx_connection_t *c,
    ngx_str_t *out);
अटल ngx_int_t ngx_proxy_protocol_v2_alpn(ngx_connection_t *c,
    ngx_str_t *out);
अटल ngx_int_t ngx_proxy_protocol_v2_ssl_sub(ngx_connection_t *c,
    ngx_uint_t type, ngx_str_t *out);
#पूर्ण_अगर
अटल u_char *ngx_proxy_protocol_v2_write_header(ngx_connection_t *c,
    u_char *buf, u_char *last);
अटल u_char ngx_proxy_protocol_v2_family(ngx_uint_t family);
अटल व्योम ngx_proxy_protocol_v2_write_ipv4(काष्ठा sockaddr *sa, u_char *addr,
    u_char *port);
अटल व्योम ngx_proxy_protocol_v2_write_ipv6(काष्ठा sockaddr *sa, u_char *addr,
    u_char *port);
अटल u_char *ngx_proxy_protocol_v2_write_tlv(ngx_connection_t *c, u_char *p,
    u_char *last, ngx_uint_t type, ngx_str_t *मूल्य);
अटल u_char *ngx_proxy_protocol_v2_write_ssl(ngx_connection_t *c, u_char *p,
    u_char *last, ngx_array_t *ssl_tlvs, ngx_uint_t client, uint32_t verify);
अटल व्योम ngx_proxy_protocol_v2_set_len(u_char *buf, u_char *p);
अटल u_char *ngx_proxy_protocol_v2_write_crc32c(ngx_connection_t *c,
    u_char *buf, u_char *p, u_char *last);


अटल ngx_proxy_protocol_tlv_entry_t  ngx_proxy_protocol_tlv_entries[] = {
    { ngx_string("alpn"),       NGX_PROXY_PROTOCOL_TLV_ALPN },
    { ngx_string("authority"),  NGX_PROXY_PROTOCOL_TLV_AUTHORITY },
    { ngx_string("unique_id"),  NGX_PROXY_PROTOCOL_TLV_UNIQUE_ID },
    { ngx_string("ssl"),        NGX_PROXY_PROTOCOL_TLV_SSL },
    { ngx_string("netns"),      NGX_PROXY_PROTOCOL_TLV_NETNS },
    { ngx_null_string,          0x00 }
};


अटल ngx_proxy_protocol_tlv_entry_t  ngx_proxy_protocol_tlv_ssl_entries[] = {
    { ngx_string("version"),    NGX_PROXY_PROTOCOL_TLV_SSL_VERSION },
    { ngx_string("cn"),         NGX_PROXY_PROTOCOL_TLV_SSL_CN },
    { ngx_string("cipher"),     NGX_PROXY_PROTOCOL_TLV_SSL_CIPHER },
    { ngx_string("sig_alg"),    NGX_PROXY_PROTOCOL_TLV_SSL_SIG_ALG },
    { ngx_string("key_alg"),    NGX_PROXY_PROTOCOL_TLV_SSL_कुंजी_ALG },
    { ngx_null_string,          0x00 }
};


अटल स्थिर u_char  ngx_proxy_protocol_signature[] = "\r\n\r\n\0\r\nQUIT\n";


u_char *
ngx_proxy_protocol_read(ngx_connection_t *c, u_char *buf, u_char *last)
{
    माप_प्रकार                 len;
    u_char                *p;
    ngx_proxy_protocol_t  *pp;

    p = buf;
    len = last - buf;

    अगर (len >= माप(ngx_proxy_protocol_header_t)
        && ngx_memcmp(p, ngx_proxy_protocol_signature,
                      माप(ngx_proxy_protocol_signature) - 1)
           == 0)
    {
        वापस ngx_proxy_protocol_v2_read(c, buf, last);
    }

    अगर (len < 8 || ngx_strncmp(p, "PROXY ", 6) != 0) {
        जाओ invalid;
    }

    p += 6;
    len -= 6;

    अगर (len >= 7 && ngx_strncmp(p, "UNKNOWN", 7) == 0) {
        ngx_log_debug0(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "PROXY protocol unknown protocol");
        p += 7;
        जाओ skip;
    }

    अगर (len < 5 || ngx_strncmp(p, "TCP", 3) != 0
        || (p[3] != '4' && p[3] != '6') || p[4] != ' ')
    {
        जाओ invalid;
    }

    p += 5;

    pp = ngx_pcalloc(c->pool, माप(ngx_proxy_protocol_t));
    अगर (pp == शून्य) {
        वापस शून्य;
    }

    p = ngx_proxy_protocol_read_addr(c, p, last, &pp->src_addr);
    अगर (p == शून्य) {
        जाओ invalid;
    }

    p = ngx_proxy_protocol_read_addr(c, p, last, &pp->dst_addr);
    अगर (p == शून्य) {
        जाओ invalid;
    }

    p = ngx_proxy_protocol_read_port(p, last, &pp->src_port, ' ');
    अगर (p == शून्य) {
        जाओ invalid;
    }

    p = ngx_proxy_protocol_read_port(p, last, &pp->dst_port, CR);
    अगर (p == शून्य) {
        जाओ invalid;
    }

    अगर (p == last) {
        जाओ invalid;
    }

    अगर (*p++ != LF) {
        जाओ invalid;
    }

    ngx_log_debug4(NGX_LOG_DEBUG_CORE, c->log, 0,
                   "PROXY protocol src: %V %d, dst: %V %d",
                   &pp->src_addr, pp->src_port, &pp->dst_addr, pp->dst_port);

    c->proxy_protocol = pp;

    वापस p;

skip:

    क्रम ( /* void */ ; p < last - 1; p++) {
        अगर (p[0] == CR && p[1] == LF) {
            वापस p + 2;
        }
    }

invalid:

    क्रम (p = buf; p < last; p++) {
        अगर (*p == CR || *p == LF) {
            अवरोध;
        }
    }

    ngx_log_error(NGX_LOG_ERR, c->log, 0,
                  "broken header: \"%*s\"", (माप_प्रकार) (p - buf), buf);

    वापस शून्य;
}


अटल u_char *
ngx_proxy_protocol_read_addr(ngx_connection_t *c, u_char *p, u_char *last,
    ngx_str_t *addr)
{
    माप_प्रकार  len;
    u_char  ch, *pos;

    pos = p;

    क्रम ( ;; ) {
        अगर (p == last) {
            वापस शून्य;
        }

        ch = *p++;

        अगर (ch == ' ') {
            अवरोध;
        }

        अगर (ch != ':' && ch != '.'
            && (ch < 'a' || ch > 'f')
            && (ch < 'A' || ch > 'F')
            && (ch < '0' || ch > '9'))
        {
            वापस शून्य;
        }
    }

    len = p - pos - 1;

    addr->तथ्य = ngx_pnalloc(c->pool, len);
    अगर (addr->तथ्य == शून्य) {
        वापस शून्य;
    }

    ngx_memcpy(addr->तथ्य, pos, len);
    addr->len = len;

    वापस p;
}


अटल u_char *
ngx_proxy_protocol_read_port(u_char *p, u_char *last, in_port_t *port,
    u_char sep)
{
    माप_प्रकार      len;
    u_char     *pos;
    ngx_int_t   n;

    pos = p;

    क्रम ( ;; ) {
        अगर (p == last) {
            वापस शून्य;
        }

        अगर (*p++ == sep) {
            अवरोध;
        }
    }

    len = p - pos - 1;

    n = ngx_atoi(pos, len);
    अगर (n < 0 || n > 65535) {
        वापस शून्य;
    }

    *port = (in_port_t) n;

    वापस p;
}


u_char *
ngx_proxy_protocol_write(ngx_connection_t *c, u_char *buf, u_char *last)
{
    ngx_uint_t  port, lport;

    अगर (last - buf < NGX_PROXY_PROTOCOL_V1_MAX_HEADER) {
        ngx_log_error(NGX_LOG_ALERT, c->log, 0,
                      "too small buffer for PROXY protocol");
        वापस शून्य;
    }

    अगर (ngx_connection_local_sockaddr(c, शून्य, 0) != NGX_OK) {
        वापस शून्य;
    }

    चयन (c->sockaddr->sa_family) {

    हाल AF_INET:
        buf = ngx_cpymem(buf, "PROXY TCP4 ", माप("PROXY TCP4 ") - 1);
        अवरोध;

#अगर (NGX_HAVE_INET6)
    हाल AF_INET6:
        buf = ngx_cpymem(buf, "PROXY TCP6 ", माप("PROXY TCP6 ") - 1);
        अवरोध;
#पूर्ण_अगर

    शेष:
        वापस ngx_cpymem(buf, "PROXY UNKNOWN" CRLF,
                          माप("PROXY UNKNOWN" CRLF) - 1);
    }

    buf += ngx_sock_ntop(c->sockaddr, c->socklen, buf, last - buf, 0);

    *buf++ = ' ';

    buf += ngx_sock_ntop(c->local_sockaddr, c->local_socklen, buf, last - buf,
                         0);

    port = ngx_inet_get_port(c->sockaddr);
    lport = ngx_inet_get_port(c->local_sockaddr);

    वापस ngx_slprintf(buf, last, " %ui %ui" CRLF, port, lport);
}


अटल u_char *
ngx_proxy_protocol_v2_read(ngx_connection_t *c, u_char *buf, u_char *last)
{
    u_char                             *end;
    माप_प्रकार                              len;
    socklen_t                           socklen;
    ngx_uint_t                          version, command, family, transport;
    ngx_sockaddr_t                      src_sockaddr, dst_sockaddr;
    ngx_proxy_protocol_t               *pp;
    ngx_proxy_protocol_header_t        *header;
    ngx_proxy_protocol_inet_addrs_t    *in;
#अगर (NGX_HAVE_INET6)
    ngx_proxy_protocol_inet6_addrs_t   *in6;
#पूर्ण_अगर

    header = (ngx_proxy_protocol_header_t *) buf;

    buf += माप(ngx_proxy_protocol_header_t);

    version = header->version_command >> 4;

    अगर (version != 2) {
        ngx_log_error(NGX_LOG_ERR, c->log, 0,
                      "unknown PROXY protocol version: %ui", version);
        वापस शून्य;
    }

    len = ngx_proxy_protocol_parse_uint16(header->len);

    अगर ((माप_प्रकार) (last - buf) < len) {
        ngx_log_error(NGX_LOG_ERR, c->log, 0, "header is too large");
        वापस शून्य;
    }

    end = buf + len;

    command = header->version_command & 0x0f;

    /* only PROXY is supported */
    अगर (command != NGX_PROXY_PROTOCOL_CMD_PROXY) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "PROXY protocol v2 unsupported command %ui", command);
        वापस end;
    }

    transport = header->family_transport & 0x0f;

    /* only STREAM is supported */
    अगर (transport != NGX_PROXY_PROTOCOL_TYPE_STREAM) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "PROXY protocol v2 unsupported transport %ui",
                       transport);
        वापस end;
    }

    pp = ngx_pcalloc(c->pool, माप(ngx_proxy_protocol_t));
    अगर (pp == शून्य) {
        वापस शून्य;
    }

    family = header->family_transport >> 4;

    चयन (family) {

    हाल NGX_PROXY_PROTOCOL_AF_INET:

        अगर ((माप_प्रकार) (end - buf) < माप(ngx_proxy_protocol_inet_addrs_t)) {
            वापस शून्य;
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

        socklen = माप(काष्ठा sockaddr_in);

        buf += माप(ngx_proxy_protocol_inet_addrs_t);

        अवरोध;

#अगर (NGX_HAVE_INET6)

    हाल NGX_PROXY_PROTOCOL_AF_INET6:

        अगर ((माप_प्रकार) (end - buf) < माप(ngx_proxy_protocol_inet6_addrs_t)) {
            वापस शून्य;
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

        socklen = माप(काष्ठा sockaddr_in6);

        buf += माप(ngx_proxy_protocol_inet6_addrs_t);

        अवरोध;

#पूर्ण_अगर

    शेष:
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "PROXY protocol v2 unsupported address family %ui",
                       family);
        वापस end;
    }

    pp->src_addr.तथ्य = ngx_pnalloc(c->pool, NGX_SOCKADDR_STRLEN);
    अगर (pp->src_addr.तथ्य == शून्य) {
        वापस शून्य;
    }

    pp->src_addr.len = ngx_sock_ntop(&src_sockaddr.sockaddr, socklen,
                                     pp->src_addr.तथ्य, NGX_SOCKADDR_STRLEN, 0);

    pp->dst_addr.तथ्य = ngx_pnalloc(c->pool, NGX_SOCKADDR_STRLEN);
    अगर (pp->dst_addr.तथ्य == शून्य) {
        वापस शून्य;
    }

    pp->dst_addr.len = ngx_sock_ntop(&dst_sockaddr.sockaddr, socklen,
                                     pp->dst_addr.तथ्य, NGX_SOCKADDR_STRLEN, 0);

    ngx_log_debug4(NGX_LOG_DEBUG_CORE, c->log, 0,
                   "PROXY protocol v2 src: %V %d, dst: %V %d",
                   &pp->src_addr, pp->src_port, &pp->dst_addr, pp->dst_port);

    अगर (buf < end) {
        pp->tlvs.तथ्य = ngx_pnalloc(c->pool, end - buf);
        अगर (pp->tlvs.तथ्य == शून्य) {
            वापस शून्य;
        }

        ngx_memcpy(pp->tlvs.तथ्य, buf, end - buf);
        pp->tlvs.len = end - buf;
    }

    c->proxy_protocol = pp;

    वापस end;
}


ngx_int_t
ngx_proxy_protocol_get_tlv(ngx_connection_t *c, ngx_str_t *name,
    ngx_str_t *मूल्य)
{
    u_char                          *p;
    माप_प्रकार                           n;
    uint32_t                         verify;
    ngx_str_t                        ssl, *tlvs;
    ngx_int_t                        rc, type;
    ngx_proxy_protocol_tlv_ssl_t    *tlv_ssl;
    ngx_proxy_protocol_tlv_entry_t  *te;

    अगर (c->proxy_protocol == शून्य) {
        वापस NGX_DECLINED;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                   "PROXY protocol v2 get tlv \"%V\"", name);

    te = ngx_proxy_protocol_tlv_entries;
    tlvs = &c->proxy_protocol->tlvs;

    p = name->तथ्य;
    n = name->len;

    अगर (n >= 4 && p[0] == 's' && p[1] == 's' && p[2] == 'l' && p[3] == '_') {

        rc = ngx_proxy_protocol_lookup_tlv(c, tlvs,
                                           NGX_PROXY_PROTOCOL_TLV_SSL, &ssl);
        अगर (rc != NGX_OK) {
            वापस rc;
        }

        अगर (ssl.len < माप(ngx_proxy_protocol_tlv_ssl_t)) {
            वापस NGX_ERROR;
        }

        p += 4;
        n -= 4;

        अगर (n == 6 && ngx_strncmp(p, "verify", 6) == 0) {

            tlv_ssl = (ngx_proxy_protocol_tlv_ssl_t *) ssl.तथ्य;
            verify = ngx_proxy_protocol_parse_uint32(tlv_ssl->verify);

            मूल्य->तथ्य = ngx_pnalloc(c->pool, NGX_INT32_LEN);
            अगर (मूल्य->तथ्य == शून्य) {
                वापस NGX_ERROR;
            }

            मूल्य->len = ngx_sprintf(मूल्य->तथ्य, "%uD", verify)
                         - मूल्य->तथ्य;
            वापस NGX_OK;
        }

        ssl.तथ्य += माप(ngx_proxy_protocol_tlv_ssl_t);
        ssl.len -= माप(ngx_proxy_protocol_tlv_ssl_t);

        te = ngx_proxy_protocol_tlv_ssl_entries;
        tlvs = &ssl;
    }

    अगर (n >= 2 && p[0] == '0' && p[1] == 'x') {

        type = ngx_hextoi(p + 2, n - 2);
        अगर (type == NGX_ERROR) {
            ngx_log_error(NGX_LOG_ERR, c->log, 0,
                          "invalid PROXY protocol TLV \"%V\"", name);
            वापस NGX_ERROR;
        }

        वापस ngx_proxy_protocol_lookup_tlv(c, tlvs, type, मूल्य);
    }

    क्रम ( /* void */ ; te->type; te++) {
        अगर (te->name.len == n && ngx_strncmp(te->name.तथ्य, p, n) == 0) {
            वापस ngx_proxy_protocol_lookup_tlv(c, tlvs, te->type, मूल्य);
        }
    }

    ngx_log_error(NGX_LOG_ERR, c->log, 0,
                  "unknown PROXY protocol TLV \"%V\"", name);

    वापस NGX_DECLINED;
}


अटल ngx_int_t
ngx_proxy_protocol_lookup_tlv(ngx_connection_t *c, ngx_str_t *tlvs,
    ngx_uint_t type, ngx_str_t *मूल्य)
{
    u_char                    *p;
    माप_प्रकार                     n, len;
    ngx_proxy_protocol_tlv_t  *tlv;

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                   "PROXY protocol v2 lookup tlv:%02xi", type);

    p = tlvs->तथ्य;
    n = tlvs->len;

    जबतक (n) {
        अगर (n < माप(ngx_proxy_protocol_tlv_t)) {
            ngx_log_error(NGX_LOG_ERR, c->log, 0, "broken PROXY protocol TLV");
            वापस NGX_ERROR;
        }

        tlv = (ngx_proxy_protocol_tlv_t *) p;
        len = ngx_proxy_protocol_parse_uint16(tlv->len);

        p += माप(ngx_proxy_protocol_tlv_t);
        n -= माप(ngx_proxy_protocol_tlv_t);

        अगर (n < len) {
            ngx_log_error(NGX_LOG_ERR, c->log, 0, "broken PROXY protocol TLV");
            वापस NGX_ERROR;
        }

        अगर (tlv->type == type) {
            मूल्य->तथ्य = p;
            मूल्य->len = len;
            वापस NGX_OK;
        }

        p += len;
        n -= len;
    }

    वापस NGX_DECLINED;
}


u_char *
ngx_proxy_protocol_v2_write(ngx_connection_t *c, u_char *buf, u_char *last,
    ngx_array_t *tlvs /* reserved */ )
{
    u_char                          *p;
    uint32_t                         verify;
    ngx_uint_t                       i, client;
    ngx_array_t                      base_tlvs, ssl_tlvs;
    ngx_proxy_protocol_tlv_मूल्य_t  *tlv;

    client = 0;
    verify = 0;

    ngx_memzero(&base_tlvs, माप(ngx_array_t));
    ngx_memzero(&ssl_tlvs, माप(ngx_array_t));

#अगर (NGX_OPENSSL)

    अगर (c->ssl != शून्य) {
        अगर (ngx_array_init(&base_tlvs, c->pool, 2,
                           माप(ngx_proxy_protocol_tlv_मूल्य_t))
            != NGX_OK)
        {
            वापस शून्य;
        }

        अगर (ngx_array_init(&ssl_tlvs, c->pool, 5,
                           माप(ngx_proxy_protocol_tlv_मूल्य_t))
            != NGX_OK)
        {
            वापस शून्य;
        }

        अगर (ngx_proxy_protocol_v2_eval_ssl(c, &base_tlvs, &ssl_tlvs, &client,
                                           &verify)
            != NGX_OK)
        {
            वापस शून्य;
        }
    }

#पूर्ण_अगर

    p = ngx_proxy_protocol_v2_write_header(c, buf, last);
    अगर (p == शून्य) {
        वापस शून्य;
    }

    tlv = base_tlvs.elts;

    क्रम (i = 0; i < base_tlvs.nelts; i++) {
        p = ngx_proxy_protocol_v2_write_tlv(c, p, last, tlv[i].type,
                                           &tlv[i].मूल्य);
        अगर (p == शून्य) {
            वापस शून्य;
        }
    }

    अगर (client) {
        p = ngx_proxy_protocol_v2_write_ssl(c, p, last, &ssl_tlvs, client,
                                            verify);
        अगर (p == शून्य) {
            वापस शून्य;
        }
    }

    ngx_proxy_protocol_v2_set_len(buf,
                                  p + माप(ngx_proxy_protocol_tlv_t) + 4);

    वापस ngx_proxy_protocol_v2_write_crc32c(c, buf, p, last);
}


#अगर (NGX_OPENSSL)

अटल ngx_int_t
ngx_proxy_protocol_v2_eval_ssl(ngx_connection_t *c, ngx_array_t *tlvs,
    ngx_array_t *ssl_tlvs, ngx_uint_t *client, uint32_t *verify)
{
    X509                            *cert;
    दीर्घ                             n;
    ngx_int_t                        rc;
    ngx_str_t                        मूल्य;
    ngx_uint_t                       type;
    स्थिर अक्षर                      *s;
    ngx_proxy_protocol_tlv_मूल्य_t  *tlv;

    अगर (ngx_proxy_protocol_v2_authority(c, &मूल्य) == NGX_OK) {
        tlv = ngx_array_धकेलो(tlvs);
        अगर (tlv == शून्य) {
            वापस NGX_ERROR;
        }

        tlv->type = NGX_PROXY_PROTOCOL_TLV_AUTHORITY;
        tlv->मूल्य = मूल्य;
    }

    अगर (ngx_proxy_protocol_v2_alpn(c, &मूल्य) == NGX_OK) {
        tlv = ngx_array_धकेलो(tlvs);
        अगर (tlv == शून्य) {
            वापस NGX_ERROR;
        }

        tlv->type = NGX_PROXY_PROTOCOL_TLV_ALPN;
        tlv->मूल्य = मूल्य;
    }

    क्रम (type = NGX_PROXY_PROTOCOL_TLV_SSL_VERSION;
         type <= NGX_PROXY_PROTOCOL_TLV_SSL_कुंजी_ALG;
         type++)
    {
        rc = ngx_proxy_protocol_v2_ssl_sub(c, type, &मूल्य);

        अगर (rc == NGX_ERROR) {
            वापस NGX_ERROR;
        }

        अगर (rc != NGX_OK) {
            जारी;
        }

        tlv = ngx_array_धकेलो(ssl_tlvs);
        अगर (tlv == शून्य) {
            वापस NGX_ERROR;
        }

        tlv->type = type;
        tlv->मूल्य = मूल्य;
    }

    *client = NGX_PROXY_PROTOCOL_V2_CLIENT_SSL;
    *verify = 1;  /* X509_V_ERR_UNSPECIFIED */

    cert = SSL_get_peer_certificate(c->ssl->connection);

    अगर (cert != शून्य) {
        X509_free(cert);

        *client |= NGX_PROXY_PROTOCOL_V2_CLIENT_CERT_SESS;

        अगर (!SSL_session_reused(c->ssl->connection)) {
            *client |= NGX_PROXY_PROTOCOL_V2_CLIENT_CERT_CONN;
        }

        n = SSL_get_verify_परिणाम(c->ssl->connection);

        अगर (n == X509_V_OK) {

            अगर (ngx_ssl_ocsp_get_status(c, &s) != NGX_OK) {
                n = X509_V_ERR_CERT_REVOKED;
            }
        }

        *verify = (uint32_t) n;
    }

    वापस NGX_OK;
}


अटल ngx_int_t
ngx_proxy_protocol_v2_authority(ngx_connection_t *c, ngx_str_t *out)
{
#अगर_घोषित SSL_CTRL_SET_TLSEXT_HOSTNAME

    स्थिर अक्षर  *sni;

    sni = SSL_get_servername(c->ssl->connection, TLSEXT_NAMETYPE_host_name);
    अगर (sni == शून्य) {
        वापस NGX_DECLINED;
    }

    out->तथ्य = (u_char *) sni;
    out->len = ngx_strlen(sni);
    वापस NGX_OK;

#अन्यथा
    वापस NGX_DECLINED;
#पूर्ण_अगर
}


अटल ngx_int_t
ngx_proxy_protocol_v2_alpn(ngx_connection_t *c, ngx_str_t *out)
{
#अगर_घोषित TLSEXT_TYPE_application_layer_protocol_negotiation

    अचिन्हित पूर्णांक          alpnlen;
    स्थिर अचिन्हित अक्षर  *alpn;

    SSL_get0_alpn_selected(c->ssl->connection, &alpn, &alpnlen);
    अगर (alpn == शून्य || alpnlen == 0) {
        वापस NGX_DECLINED;
    }

    out->तथ्य = (u_char *) alpn;
    out->len = alpnlen;
    वापस NGX_OK;

#अन्यथा
    वापस NGX_DECLINED;
#पूर्ण_अगर
}


अटल ngx_int_t
ngx_proxy_protocol_v2_ssl_sub(ngx_connection_t *c, ngx_uint_t type,
    ngx_str_t *out)
{
    पूर्णांक               i, len;
    X509             *cert;
    u_char           *alg, *s;
    EVP_PKEY         *pkey;
#अगर (OPENSSL_VERSION_संख्या >= 0x40000000L)
    स्थिर
#पूर्ण_अगर
    X509_NAME        *subject;
#अगर (OPENSSL_VERSION_संख्या >= 0x40000000L)
    स्थिर
#पूर्ण_अगर
    X509_NAME_ENTRY  *entry;

    चयन (type) {

    हाल NGX_PROXY_PROTOCOL_TLV_SSL_VERSION:
        out->तथ्य = (u_char *) SSL_get_version(c->ssl->connection);
        out->len = ngx_strlen(out->तथ्य);
        वापस NGX_OK;

    हाल NGX_PROXY_PROTOCOL_TLV_SSL_CIPHER:
        out->तथ्य = (u_char *) SSL_get_cipher_name(c->ssl->connection);
        out->len = ngx_strlen(out->तथ्य);
        वापस NGX_OK;

    हाल NGX_PROXY_PROTOCOL_TLV_SSL_CN:
        cert = SSL_get_peer_certificate(c->ssl->connection);
        अगर (cert == शून्य) {
            वापस NGX_DECLINED;
        }

        subject = X509_get_subject_name(cert);
        अगर (subject == शून्य) {
            X509_free(cert);
            वापस NGX_ERROR;
        }

        i = X509_NAME_get_सूचकांक_by_NID(subject, NID_commonName, -1);
        अगर (i < 0) {
            X509_free(cert);
            वापस NGX_DECLINED;
        }

        entry = X509_NAME_get_entry(subject, i);

        len = ASN1_STRING_to_UTF8(&s, X509_NAME_ENTRY_get_तथ्य(entry));
        अगर (len < 0) {
            ngx_ssl_error(NGX_LOG_ALERT, c->log, 0,
                          "ASN1_STRING_to_UTF8() failed");
            X509_free(cert);
            वापस NGX_ERROR;
        }

        out->len = len;
        out->तथ्य = ngx_pnalloc(c->pool, len);
        अगर (out->तथ्य == शून्य) {
            OPENSSL_free(s);
            X509_free(cert);
            वापस NGX_ERROR;
        }

        ngx_memcpy(out->तथ्य, s, len);

        OPENSSL_free(s);
        X509_free(cert);
        वापस NGX_OK;

    हाल NGX_PROXY_PROTOCOL_TLV_SSL_SIG_ALG:
        cert = SSL_get_certificate(c->ssl->connection);

        out->तथ्य = (u_char *) OBJ_nid2sn(X509_get_signature_nid(cert));
        out->len = ngx_strlen(out->तथ्य);
        वापस NGX_OK;

    हाल NGX_PROXY_PROTOCOL_TLV_SSL_कुंजी_ALG:
        cert = SSL_get_certificate(c->ssl->connection);

        pkey = X509_get_pubkey(cert);
        अगर (pkey == शून्य) {
            वापस NGX_ERROR;
        }

        चयन (EVP_PKEY_base_id(pkey)) {

        हाल EVP_PKEY_RSA:
#अगर_घोषित EVP_PKEY_RSA_PSS
        हाल EVP_PKEY_RSA_PSS:
#पूर्ण_अगर
            alg = (u_char *) "RSA";
            अवरोध;

        हाल EVP_PKEY_EC:
            alg = (u_char *) "EC";
            अवरोध;

        हाल EVP_PKEY_DSA:
            alg = (u_char *) "DSA";
            अवरोध;

        शेष:
            EVP_PKEY_free(pkey);
            वापस NGX_DECLINED;
        }

        out->len = माप("RSA16384") - 1;
        out->तथ्य = ngx_pnalloc(c->pool, out->len);
        अगर (out->तथ्य == शून्य) {
            EVP_PKEY_free(pkey);
            वापस NGX_ERROR;
        }

        out->len = ngx_snprintf(out->तथ्य, out->len, "%s%d",
                                alg, EVP_PKEY_bits(pkey))
                   - out->तथ्य;

        EVP_PKEY_free(pkey);

        वापस NGX_OK;
    }

    वापस NGX_DECLINED;
}

#पूर्ण_अगर


अटल u_char *
ngx_proxy_protocol_v2_write_header(ngx_connection_t *c, u_char *buf,
    u_char *last)
{
    u_char                            *p, src_af, dst_af,
                                       family, command, transport;
    ngx_uint_t                         src_fam, dst_fam;
    ngx_proxy_protocol_header_t       *header;
    ngx_proxy_protocol_inet_addrs_t   *in;
#अगर (NGX_HAVE_INET6)
    ngx_uint_t                         port, lport;
    काष्ठा in6_addr                   *src6, *dst6;
    ngx_proxy_protocol_inet6_addrs_t  *in6;
#पूर्ण_अगर

    अगर (last - buf < NGX_PROXY_PROTOCOL_V2_MAX_HEADER) {
        ngx_log_error(NGX_LOG_ALERT, c->log, 0,
                      "too small buffer for PROXY protocol v2");
        वापस शून्य;
    }

    अगर (ngx_connection_local_sockaddr(c, शून्य, 0) != NGX_OK) {
        वापस शून्य;
    }

    /* the length field is set by the caller once all TLVs are written */

    header = (ngx_proxy_protocol_header_t *) buf;

    ngx_memcpy(header->signature, ngx_proxy_protocol_signature,
               माप(ngx_proxy_protocol_signature) - 1);

    p = buf + माप(ngx_proxy_protocol_header_t);

    src_fam = c->sockaddr->sa_family;
    dst_fam = c->local_sockaddr->sa_family;

    src_af = ngx_proxy_protocol_v2_family(src_fam);
    dst_af = ngx_proxy_protocol_v2_family(dst_fam);

    /* promote to the highest address family present on either side */

    अगर (src_af == NGX_PROXY_PROTOCOL_AF_UNSPEC
        || dst_af == NGX_PROXY_PROTOCOL_AF_UNSPEC)
    {
        command = NGX_PROXY_PROTOCOL_CMD_LOCAL;
        family = NGX_PROXY_PROTOCOL_AF_UNSPEC;
        transport = NGX_PROXY_PROTOCOL_TYPE_UNSPEC;

    } अन्यथा {
        command = NGX_PROXY_PROTOCOL_CMD_PROXY;
        family = ngx_max(src_af, dst_af);

        चयन (c->type) {
        हाल SOCK_STREAM:
            transport = NGX_PROXY_PROTOCOL_TYPE_STREAM;
            अवरोध;
        हाल SOCK_DGRAM:
            transport = NGX_PROXY_PROTOCOL_TYPE_DGRAM;
            अवरोध;
        शेष:
            transport = NGX_PROXY_PROTOCOL_TYPE_UNSPEC;
            अवरोध;
        }
    }

    header->version_command = 0x20 | command;
    header->family_transport = (family << 4) | transport;

    चयन (family) {

    हाल NGX_PROXY_PROTOCOL_AF_INET:

        in = (ngx_proxy_protocol_inet_addrs_t *) p;

        ngx_proxy_protocol_v2_write_ipv4(c->sockaddr,
                                         in->src_addr, in->src_port);
        ngx_proxy_protocol_v2_write_ipv4(c->local_sockaddr,
                                         in->dst_addr, in->dst_port);

        p += माप(ngx_proxy_protocol_inet_addrs_t);

        अवरोध;

#अगर (NGX_HAVE_INET6)

    हाल NGX_PROXY_PROTOCOL_AF_INET6:

        अगर (src_fam == AF_INET6 && dst_fam == AF_INET6) {
            src6 = &((काष्ठा sockaddr_in6 *) c->sockaddr)->sin6_addr;
            dst6 = &((काष्ठा sockaddr_in6 *) c->local_sockaddr)->sin6_addr;

            अगर (IN6_IS_ADDR_V4MAPPED(src6) && IN6_IS_ADDR_V4MAPPED(dst6)) {

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

                p += माप(ngx_proxy_protocol_inet_addrs_t);
                अवरोध;
            }
        }

        in6 = (ngx_proxy_protocol_inet6_addrs_t *) p;

        ngx_proxy_protocol_v2_write_ipv6(c->sockaddr,
                                         in6->src_addr, in6->src_port);
        ngx_proxy_protocol_v2_write_ipv6(c->local_sockaddr,
                                         in6->dst_addr, in6->dst_port);

        p += माप(ngx_proxy_protocol_inet6_addrs_t);

        अवरोध;

#पूर्ण_अगर

    शेष:  /* NGX_PROXY_PROTOCOL_AF_UNSPEC */
        अवरोध;
    }

    वापस p;
}


अटल u_char
ngx_proxy_protocol_v2_family(ngx_uint_t family)
{
    चयन (family) {

    हाल AF_INET:
        वापस NGX_PROXY_PROTOCOL_AF_INET;

#अगर (NGX_HAVE_INET6)
    हाल AF_INET6:
        वापस NGX_PROXY_PROTOCOL_AF_INET6;
#पूर्ण_अगर

    शेष:
        वापस NGX_PROXY_PROTOCOL_AF_UNSPEC;
    }
}


अटल व्योम
ngx_proxy_protocol_v2_write_ipv4(काष्ठा sockaddr *sa, u_char *addr,
    u_char *port)
{
    काष्ठा sockaddr_in  *sin;

    sin = (काष्ठा sockaddr_in *) sa;
    ngx_memcpy(addr, &sin->sin_addr, 4);
    ngx_memcpy(port, &sin->sin_port, 2);
}


#अगर (NGX_HAVE_INET6)

अटल व्योम
ngx_proxy_protocol_v2_write_ipv6(काष्ठा sockaddr *sa, u_char *addr,
    u_char *port)
{
    काष्ठा sockaddr_in   *sin;
    काष्ठा sockaddr_in6  *sin6;

    /* IPv4-mapped prefix: 80 zero bits + 16 one bits */
    अटल स्थिर u_char  mapped[]   = { 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0xff, 0xff };

    अगर (sa->sa_family == AF_INET6) {
        sin6 = (काष्ठा sockaddr_in6 *) sa;
        ngx_memcpy(addr, &sin6->sin6_addr, 16);
        ngx_memcpy(port, &sin6->sin6_port, 2);

    } अन्यथा {
        /* promote AF_INET to ::ffff:a.b.c.d */
        sin = (काष्ठा sockaddr_in *) sa;
        ngx_memcpy(addr, mapped, माप(mapped));
        ngx_memcpy(addr + माप(mapped), &sin->sin_addr, 4);
        ngx_memcpy(port, &sin->sin_port, 2);
    }
}

#पूर्ण_अगर


अटल u_char *
ngx_proxy_protocol_v2_write_tlv(ngx_connection_t *c, u_char *p, u_char *last,
    ngx_uint_t type, ngx_str_t *मूल्य)
{
    ngx_proxy_protocol_tlv_t  *tlv;

    अगर ((माप_प्रकार) (last - p) < माप(ngx_proxy_protocol_tlv_t) + मूल्य->len) {
        ngx_log_error(NGX_LOG_ALERT, c->log, 0,
                      "too small buffer for PROXY protocol v2 TLV");
        वापस शून्य;
    }

    tlv = (ngx_proxy_protocol_tlv_t *) p;
    tlv->type = (u_char) type;
    tlv->len[0] = (u_char) (मूल्य->len >> 8);
    tlv->len[1] = (u_char)  मूल्य->len;

    p += माप(ngx_proxy_protocol_tlv_t);

    वापस ngx_cpymem(p, मूल्य->तथ्य, मूल्य->len);
}


अटल u_char *
ngx_proxy_protocol_v2_write_ssl(ngx_connection_t *c, u_char *p, u_char *last,
    ngx_array_t *ssl_tlvs, ngx_uint_t client, uint32_t verify)
{
    u_char                          *start;
    uint16_t                         len;
    ngx_uint_t                       i;
    ngx_proxy_protocol_tlv_t        *tlv;
    ngx_proxy_protocol_tlv_ssl_t    *tlv_ssl;
    ngx_proxy_protocol_tlv_मूल्य_t  *sub;

    अगर ((माप_प्रकार) (last - p) < माप(ngx_proxy_protocol_tlv_t)
                              + माप(ngx_proxy_protocol_tlv_ssl_t))
    {
        ngx_log_error(NGX_LOG_ALERT, c->log, 0,
                      "too small buffer for PROXY protocol v2 SSL TLV");
        वापस शून्य;
    }

    start = p;
    p += माप(ngx_proxy_protocol_tlv_t);

    tlv_ssl = (ngx_proxy_protocol_tlv_ssl_t *) p;
    tlv_ssl->client = (u_char) client;
    tlv_ssl->verify[0] = (u_char) (verify >> 24);
    tlv_ssl->verify[1] = (u_char) (verify >> 16);
    tlv_ssl->verify[2] = (u_char) (verify >> 8);
    tlv_ssl->verify[3] = (u_char)  verify;

    p += माप(ngx_proxy_protocol_tlv_ssl_t);

    sub = ssl_tlvs->elts;

    क्रम (i = 0; i < ssl_tlvs->nelts; i++) {
        p = ngx_proxy_protocol_v2_write_tlv(c, p, last, sub[i].type,
                                            &sub[i].मूल्य);
        अगर (p == शून्य) {
            वापस शून्य;
        }
    }

    len = (uint16_t) (p - start - माप(ngx_proxy_protocol_tlv_t));

    tlv = (ngx_proxy_protocol_tlv_t *) start;
    tlv->type = NGX_PROXY_PROTOCOL_TLV_SSL;
    tlv->len[0] = (u_char) (len >> 8);
    tlv->len[1] = (u_char)  len;

    वापस p;
}


अटल व्योम
ngx_proxy_protocol_v2_set_len(u_char *buf, u_char *p)
{
    uint16_t                      len;
    ngx_proxy_protocol_header_t  *header;

    len = (uint16_t) (p - buf - माप(ngx_proxy_protocol_header_t));

    header = (ngx_proxy_protocol_header_t *) buf;
    header->len[0] = (u_char) (len >> 8);
    header->len[1] = (u_char) len;
}


अटल u_char *
ngx_proxy_protocol_v2_write_crc32c(ngx_connection_t *c, u_char *buf,
    u_char *p, u_char *last)
{
    u_char                    *मूल्य;
    uint32_t                   crc;
    ngx_proxy_protocol_tlv_t  *tlv;

    अगर ((माप_प्रकार) (last - p) < माप(ngx_proxy_protocol_tlv_t) + 4) {
        ngx_log_error(NGX_LOG_ALERT, c->log, 0,
                      "too small buffer for PROXY protocol v2 CRC32c TLV");
        वापस शून्य;
    }

    /* append the CRC32c TLV with a zeroed value field */

    tlv = (ngx_proxy_protocol_tlv_t *) p;
    tlv->type = NGX_PROXY_PROTOCOL_TLV_CRC32C;
    tlv->len[0] = 0;
    tlv->len[1] = 4;

    मूल्य = p + माप(ngx_proxy_protocol_tlv_t);

    ngx_memzero(मूल्य, 4);

    p = मूल्य + 4;

    /* the checksum covers the entire header with the zeroed value */

    crc = ngx_crc32c_long(buf, p - buf);

    मूल्य[0] = (u_char) (crc >> 24);
    मूल्य[1] = (u_char) (crc >> 16);
    मूल्य[2] = (u_char) (crc >> 8);
    मूल्य[3] = (u_char) crc;

    वापस p;
}
