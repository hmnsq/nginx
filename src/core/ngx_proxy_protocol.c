
/*
 * Copyright (C) Roman Arutyunyan
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


#परिभाषय NGX_PROXY_PROTOCOL_V2_MAX_शीर्षकम्    52

#परिभाषय NGX_PROXY_PROTOCOL_CMD_LOCAL        0
#परिभाषय NGX_PROXY_PROTOCOL_CMD_PROXY        1

#परिभाषय NGX_PROXY_PROTOCOL_AF_UNSPEC        0
#परिभाषय NGX_PROXY_PROTOCOL_AF_INET          1
#परिभाषय NGX_PROXY_PROTOCOL_AF_INET6         2

#परिभाषय NGX_PROXY_PROTOCOL_TYPE_UNSPEC      0
#परिभाषय NGX_PROXY_PROTOCOL_TYPE_प्रवाहः      1
#परिभाषय NGX_PROXY_PROTOCOL_TYPE_DGRAM       2

#परिभाषय NGX_PROXY_PROTOCOL_TLV_ALPN         0x01
#परिभाषय NGX_PROXY_PROTOCOL_TLV_AUTHORITY    0x02
#परिभाषय NGX_PROXY_PROTOCOL_TLV_चक्रीयपरीक्षा_32_C       0x03
#परिभाषय NGX_PROXY_PROTOCOL_TLV_UNIQUE_ID    0x05
#परिभाषय NGX_PROXY_PROTOCOL_TLV_SSL          0x20
#परिभाषय NGX_PROXY_PROTOCOL_TLV_SSL_संस्करणम्  0x21
#परिभाषय NGX_PROXY_PROTOCOL_TLV_SSL_CN       0x22
#परिभाषय NGX_PROXY_PROTOCOL_TLV_SSL_CIPHER   0x23
#परिभाषय NGX_PROXY_PROTOCOL_TLV_SSL_SIG_ALG  0x24
#परिभाषय NGX_PROXY_PROTOCOL_TLV_SSL_कुञ्चिका_ALG  0x25
#परिभाषय NGX_PROXY_PROTOCOL_TLV_NETNS        0x30

#परिभाषय NGX_PROXY_PROTOCOL_V2_CLIENT_SSL          0x01
#परिभाषय NGX_PROXY_PROTOCOL_V2_CLIENT_CERT_CONN    0x02
#परिभाषय NGX_PROXY_PROTOCOL_V2_CLIENT_CERT_SESS    0x04


#परिभाषय ngx_proxy_protocol_parse_uint16(p)                                    \
    ( ((uint16_t) (p)[0] << 8)                                                \
    + (           (p)[1]) )

#परिभाषय ngx_proxy_protocol_parse_uint32(p)                                    \
    ( ((uint32_t) (p)[0] << 24)                                               \
    + (           (p)[1] << 16)                                               \
    + (           (p)[2] << 8)                                                \
    + (           (p)[3]) )


प्रकारनिर्देश संरचना {
    u_char                                  signature[12];
    u_char                                  संस्करणम्_command;
    u_char                                  family_transport;
    u_char                                  len[2];
} ngx_proxy_protocol_शीर्षकम्_t;


प्रकारनिर्देश संरचना {
    u_char                                  src_addr[4];
    u_char                                  dst_addr[4];
    u_char                                  src_पत्तनम्[2];
    u_char                                  dst_पत्तनम्[2];
} ngx_proxy_protocol_inet_addrs_t;


प्रकारनिर्देश संरचना {
    u_char                                  src_addr[16];
    u_char                                  dst_addr[16];
    u_char                                  src_पत्तनम्[2];
    u_char                                  dst_पत्तनम्[2];
} ngx_proxy_protocol_inet6_addrs_t;


प्रकारनिर्देश संरचना {
    u_char                                  type;
    u_char                                  len[2];
} ngx_proxy_protocol_tlv_t;


प्रकारनिर्देश संरचना {
    u_char                                  client;
    u_char                                  verify[4];
} ngx_proxy_protocol_tlv_ssl_t;


प्रकारनिर्देश संरचना {
    ngx_str_t                               name;
    ngx_uint_t                              type;
} ngx_proxy_protocol_tlv_प्रविष्टिः_t;


प्रकारनिर्देश संरचना {
    ngx_uint_t                              type;
    ngx_str_t                               मानम्;
} ngx_proxy_protocol_tlv_मानम्_t;


स्थिर u_char *ngx_proxy_protocol_read_addr(ngx_connection_t *c, u_char *p,
    u_char *चरमः, ngx_str_t *addr);
स्थिर u_char *ngx_proxy_protocol_read_पत्तनम्(u_char *p, u_char *चरमः,
    अन्तर्_पत्तनम्_t *पत्तनम्, u_char sep);
स्थिर u_char *ngx_proxy_protocol_v2_read(ngx_connection_t *c, u_char *buf,
    u_char *चरमः);
स्थिर ngx_int_t ngx_proxy_protocol_lookup_tlv(ngx_connection_t *c,
    ngx_str_t *tlvs, ngx_uint_t type, ngx_str_t *मानम्);
#यदि (NGX_OPENSSL)
स्थिर ngx_int_t ngx_proxy_protocol_v2_eval_ssl(ngx_connection_t *c,
    ngx_array_t *tlvs, ngx_array_t *ssl_tlvs, ngx_uint_t *client,
    uint32_t *verify);
स्थिर ngx_int_t ngx_proxy_protocol_v2_authority(ngx_connection_t *c,
    ngx_str_t *बहिर्गमनम्);
स्थिर ngx_int_t ngx_proxy_protocol_v2_alpn(ngx_connection_t *c,
    ngx_str_t *बहिर्गमनम्);
स्थिर ngx_int_t ngx_proxy_protocol_v2_ssl_sub(ngx_connection_t *c,
    ngx_uint_t type, ngx_str_t *बहिर्गमनम्);
#यदि_अन्तः
स्थिर u_char *ngx_proxy_protocol_v2_write_शीर्षकम्(ngx_connection_t *c,
    u_char *buf, u_char *चरमः);
स्थिर u_char ngx_proxy_protocol_v2_family(ngx_uint_t family);
स्थिर शून्य ngx_proxy_protocol_v2_write_ipv4(संरचना sockaddr *sa, u_char *addr,
    u_char *पत्तनम्);
स्थिर शून्य ngx_proxy_protocol_v2_write_ipv6(संरचना sockaddr *sa, u_char *addr,
    u_char *पत्तनम्);
स्थिर u_char *ngx_proxy_protocol_v2_write_tlv(ngx_connection_t *c, u_char *p,
    u_char *चरमः, ngx_uint_t type, ngx_str_t *मानम्);
स्थिर u_char *ngx_proxy_protocol_v2_write_ssl(ngx_connection_t *c, u_char *p,
    u_char *चरमः, ngx_array_t *ssl_tlvs, ngx_uint_t client, uint32_t verify);
स्थिर शून्य ngx_proxy_protocol_v2_समुच्चयः_len(u_char *buf, u_char *p);
स्थिर u_char *ngx_proxy_protocol_v2_write_crc32c(ngx_connection_t *c,
    u_char *buf, u_char *p, u_char *चरमः);


स्थिर ngx_proxy_protocol_tlv_प्रविष्टिः_t  ngx_proxy_protocol_tlv_entries[] = {
    { ngx_string("alpn"),       NGX_PROXY_PROTOCOL_TLV_ALPN },
    { ngx_string("authority"),  NGX_PROXY_PROTOCOL_TLV_AUTHORITY },
    { ngx_string("unique_id"),  NGX_PROXY_PROTOCOL_TLV_UNIQUE_ID },
    { ngx_string("ssl"),        NGX_PROXY_PROTOCOL_TLV_SSL },
    { ngx_string("netns"),      NGX_PROXY_PROTOCOL_TLV_NETNS },
    { ngx_null_string,          0x00 }
};


स्थिर ngx_proxy_protocol_tlv_प्रविष्टिः_t  ngx_proxy_protocol_tlv_ssl_entries[] = {
    { ngx_string("version"),    NGX_PROXY_PROTOCOL_TLV_SSL_संस्करणम् },
    { ngx_string("cn"),         NGX_PROXY_PROTOCOL_TLV_SSL_CN },
    { ngx_string("cipher"),     NGX_PROXY_PROTOCOL_TLV_SSL_CIPHER },
    { ngx_string("sig_alg"),    NGX_PROXY_PROTOCOL_TLV_SSL_SIG_ALG },
    { ngx_string("key_alg"),    NGX_PROXY_PROTOCOL_TLV_SSL_कुञ्चिका_ALG },
    { ngx_null_string,          0x00 }
};


स्थिर ध्रुव u_char  ngx_proxy_protocol_signature[] = "\r\n\r\n\0\r\nQUIT\n";


u_char *
ngx_proxy_protocol_read(ngx_connection_t *c, u_char *buf, u_char *चरमः)
{
    आकार_t                 len;
    u_char                *p;
    ngx_proxy_protocol_t  *pp;

    p = buf;
    len = चरमः - buf;

    यदि (len >= परिमाण(ngx_proxy_protocol_शीर्षकम्_t)
        && ngx_memcmp(p, ngx_proxy_protocol_signature,
                      परिमाण(ngx_proxy_protocol_signature) - 1)
           == 0)
    {
        प्रत्यागम ngx_proxy_protocol_v2_read(c, buf, चरमः);
    }

    यदि (len < 8 || ngx_strncmp(p, "PROXY ", 6) != 0) {
        गच्छ invalid;
    }

    p += 6;
    len -= 6;

    यदि (len >= 7 && ngx_strncmp(p, "UNKNOWN", 7) == 0) {
        ngx_log_debug0(NGX_LOG_DEBUG_CORE, c->लघुगणकः, 0,
                       "PROXY protocol unknown protocol");
        p += 7;
        गच्छ skip;
    }

    यदि (len < 5 || ngx_strncmp(p, "TCP", 3) != 0
        || (p[3] != '4' && p[3] != '6') || p[4] != ' ')
    {
        गच्छ invalid;
    }

    p += 5;

    pp = ngx_pcalloc(c->pool, परिमाण(ngx_proxy_protocol_t));
    यदि (pp == रिक्त) {
        प्रत्यागम रिक्त;
    }

    p = ngx_proxy_protocol_read_addr(c, p, चरमः, &pp->src_addr);
    यदि (p == रिक्त) {
        गच्छ invalid;
    }

    p = ngx_proxy_protocol_read_addr(c, p, चरमः, &pp->dst_addr);
    यदि (p == रिक्त) {
        गच्छ invalid;
    }

    p = ngx_proxy_protocol_read_पत्तनम्(p, चरमः, &pp->src_पत्तनम्, ' ');
    यदि (p == रिक्त) {
        गच्छ invalid;
    }

    p = ngx_proxy_protocol_read_पत्तनम्(p, चरमः, &pp->dst_पत्तनम्, CR);
    यदि (p == रिक्त) {
        गच्छ invalid;
    }

    यदि (p == चरमः) {
        गच्छ invalid;
    }

    यदि (*p++ != LF) {
        गच्छ invalid;
    }

    ngx_log_debug4(NGX_LOG_DEBUG_CORE, c->लघुगणकः, 0,
                   "PROXY protocol src: %V %d, dst: %V %d",
                   &pp->src_addr, pp->src_पत्तनम्, &pp->dst_addr, pp->dst_पत्तनम्);

    c->proxy_protocol = pp;

    प्रत्यागम p;

skip:

    प्रति ( /* void */ ; p < चरमः - 1; p++) {
        यदि (p[0] == CR && p[1] == LF) {
            प्रत्यागम p + 2;
        }
    }

invalid:

    प्रति (p = buf; p < चरमः; p++) {
        यदि (*p == CR || *p == LF) {
            भङ्ग;
        }
    }

    ngx_log_error(NGX_LOG_ERR, c->लघुगणकः, 0,
                  "broken header: \"%*s\"", (आकार_t) (p - buf), buf);

    प्रत्यागम रिक्त;
}


स्थिर u_char *
ngx_proxy_protocol_read_addr(ngx_connection_t *c, u_char *p, u_char *चरमः,
    ngx_str_t *addr)
{
    आकार_t  len;
    u_char  ch, *pos;

    pos = p;

    प्रति ( ;; ) {
        यदि (p == चरमः) {
            प्रत्यागम रिक्त;
        }

        ch = *p++;

        यदि (ch == ' ') {
            भङ्ग;
        }

        यदि (ch != ':' && ch != '.'
            && (ch < 'a' || ch > 'f')
            && (ch < 'A' || ch > 'F')
            && (ch < '0' || ch > '9'))
        {
            प्रत्यागम रिक्त;
        }
    }

    len = p - pos - 1;

    addr->दत्त = ngx_pnalloc(c->pool, len);
    यदि (addr->दत्त == रिक्त) {
        प्रत्यागम रिक्त;
    }

    ngx_memcpy(addr->दत्त, pos, len);
    addr->len = len;

    प्रत्यागम p;
}


स्थिर u_char *
ngx_proxy_protocol_read_पत्तनम्(u_char *p, u_char *चरमः, अन्तर्_पत्तनम्_t *पत्तनम्,
    u_char sep)
{
    आकार_t      len;
    u_char     *pos;
    ngx_int_t   n;

    pos = p;

    प्रति ( ;; ) {
        यदि (p == चरमः) {
            प्रत्यागम रिक्त;
        }

        यदि (*p++ == sep) {
            भङ्ग;
        }
    }

    len = p - pos - 1;

    n = ngx_atoi(pos, len);
    यदि (n < 0 || n > 65535) {
        प्रत्यागम रिक्त;
    }

    *पत्तनम् = (अन्तर्_पत्तनम्_t) n;

    प्रत्यागम p;
}


u_char *
ngx_proxy_protocol_write(ngx_connection_t *c, u_char *buf, u_char *चरमः)
{
    ngx_uint_t  पत्तनम्, lport;

    यदि (चरमः - buf < NGX_PROXY_PROTOCOL_V1_MAX_शीर्षकम्) {
        ngx_log_error(NGX_LOG_ALERT, c->लघुगणकः, 0,
                      "too small buffer for PROXY protocol");
        प्रत्यागम रिक्त;
    }

    यदि (ngx_connection_local_sockaddr(c, रिक्त, 0) != NGX_OK) {
        प्रत्यागम रिक्त;
    }

    विकल्प (c->sockaddr->sa_family) {

    पक्ष AF_INET:
        buf = ngx_cpymem(buf, "PROXY TCP4 ", परिमाण("PROXY TCP4 ") - 1);
        भङ्ग;

#यदि (NGX_HAVE_INET6)
    पक्ष AF_INET6:
        buf = ngx_cpymem(buf, "PROXY TCP6 ", परिमाण("PROXY TCP6 ") - 1);
        भङ्ग;
#यदि_अन्तः

    स्वभाव:
        प्रत्यागम ngx_cpymem(buf, "PROXY UNKNOWN" CRLF,
                          परिमाण("PROXY UNKNOWN" CRLF) - 1);
    }

    buf += ngx_sock_ntop(c->sockaddr, c->socklen, buf, चरमः - buf, 0);

    *buf++ = ' ';

    buf += ngx_sock_ntop(c->local_sockaddr, c->local_socklen, buf, चरमः - buf,
                         0);

    पत्तनम् = ngx_inet_get_पत्तनम्(c->sockaddr);
    lport = ngx_inet_get_पत्तनम्(c->local_sockaddr);

    प्रत्यागम ngx_slprintf(buf, चरमः, " %ui %ui" CRLF, पत्तनम्, lport);
}


स्थिर u_char *
ngx_proxy_protocol_v2_read(ngx_connection_t *c, u_char *buf, u_char *चरमः)
{
    u_char                             *अन्तः;
    आकार_t                              len;
    socklen_t                           socklen;
    ngx_uint_t                          संस्करणम्, command, family, transport;
    ngx_sockaddr_t                      src_sockaddr, dst_sockaddr;
    ngx_proxy_protocol_t               *pp;
    ngx_proxy_protocol_शीर्षकम्_t        *शीर्षकम्;
    ngx_proxy_protocol_inet_addrs_t    *अन्तर्;
#यदि (NGX_HAVE_INET6)
    ngx_proxy_protocol_inet6_addrs_t   *in6;
#यदि_अन्तः

    शीर्षकम् = (ngx_proxy_protocol_शीर्षकम्_t *) buf;

    buf += परिमाण(ngx_proxy_protocol_शीर्षकम्_t);

    संस्करणम् = शीर्षकम्->संस्करणम्_command >> 4;

    यदि (संस्करणम् != 2) {
        ngx_log_error(NGX_LOG_ERR, c->लघुगणकः, 0,
                      "unknown PROXY protocol version: %ui", संस्करणम्);
        प्रत्यागम रिक्त;
    }

    len = ngx_proxy_protocol_parse_uint16(शीर्षकम्->len);

    यदि ((आकार_t) (चरमः - buf) < len) {
        ngx_log_error(NGX_LOG_ERR, c->लघुगणकः, 0, "header is too large");
        प्रत्यागम रिक्त;
    }

    अन्तः = buf + len;

    command = शीर्षकम्->संस्करणम्_command & 0x0f;

    /* only PROXY is supported */
    यदि (command != NGX_PROXY_PROTOCOL_CMD_PROXY) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->लघुगणकः, 0,
                       "PROXY protocol v2 unsupported command %ui", command);
        प्रत्यागम अन्तः;
    }

    transport = शीर्षकम्->family_transport & 0x0f;

    /* only STREAM is supported */
    यदि (transport != NGX_PROXY_PROTOCOL_TYPE_प्रवाहः) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->लघुगणकः, 0,
                       "PROXY protocol v2 unsupported transport %ui",
                       transport);
        प्रत्यागम अन्तः;
    }

    pp = ngx_pcalloc(c->pool, परिमाण(ngx_proxy_protocol_t));
    यदि (pp == रिक्त) {
        प्रत्यागम रिक्त;
    }

    family = शीर्षकम्->family_transport >> 4;

    विकल्प (family) {

    पक्ष NGX_PROXY_PROTOCOL_AF_INET:

        यदि ((आकार_t) (अन्तः - buf) < परिमाण(ngx_proxy_protocol_inet_addrs_t)) {
            प्रत्यागम रिक्त;
        }

        अन्तर् = (ngx_proxy_protocol_inet_addrs_t *) buf;

        src_sockaddr.sockaddr_अन्तर्.sin_family = AF_INET;
        src_sockaddr.sockaddr_अन्तर्.sin_पत्तनम् = 0;
        ngx_memcpy(&src_sockaddr.sockaddr_अन्तर्.sin_addr, अन्तर्->src_addr, 4);

        dst_sockaddr.sockaddr_अन्तर्.sin_family = AF_INET;
        dst_sockaddr.sockaddr_अन्तर्.sin_पत्तनम् = 0;
        ngx_memcpy(&dst_sockaddr.sockaddr_अन्तर्.sin_addr, अन्तर्->dst_addr, 4);

        pp->src_पत्तनम् = ngx_proxy_protocol_parse_uint16(अन्तर्->src_पत्तनम्);
        pp->dst_पत्तनम् = ngx_proxy_protocol_parse_uint16(अन्तर्->dst_पत्तनम्);

        socklen = परिमाण(संरचना sockaddr_अन्तर्);

        buf += परिमाण(ngx_proxy_protocol_inet_addrs_t);

        भङ्ग;

#यदि (NGX_HAVE_INET6)

    पक्ष NGX_PROXY_PROTOCOL_AF_INET6:

        यदि ((आकार_t) (अन्तः - buf) < परिमाण(ngx_proxy_protocol_inet6_addrs_t)) {
            प्रत्यागम रिक्त;
        }

        in6 = (ngx_proxy_protocol_inet6_addrs_t *) buf;

        src_sockaddr.sockaddr_in6.sin6_family = AF_INET6;
        src_sockaddr.sockaddr_in6.sin6_पत्तनम् = 0;
        ngx_memcpy(&src_sockaddr.sockaddr_in6.sin6_addr, in6->src_addr, 16);

        dst_sockaddr.sockaddr_in6.sin6_family = AF_INET6;
        dst_sockaddr.sockaddr_in6.sin6_पत्तनम् = 0;
        ngx_memcpy(&dst_sockaddr.sockaddr_in6.sin6_addr, in6->dst_addr, 16);

        pp->src_पत्तनम् = ngx_proxy_protocol_parse_uint16(in6->src_पत्तनम्);
        pp->dst_पत्तनम् = ngx_proxy_protocol_parse_uint16(in6->dst_पत्तनम्);

        socklen = परिमाण(संरचना sockaddr_in6);

        buf += परिमाण(ngx_proxy_protocol_inet6_addrs_t);

        भङ्ग;

#यदि_अन्तः

    स्वभाव:
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->लघुगणकः, 0,
                       "PROXY protocol v2 unsupported address family %ui",
                       family);
        प्रत्यागम अन्तः;
    }

    pp->src_addr.दत्त = ngx_pnalloc(c->pool, NGX_SOCKADDR_STRLEN);
    यदि (pp->src_addr.दत्त == रिक्त) {
        प्रत्यागम रिक्त;
    }

    pp->src_addr.len = ngx_sock_ntop(&src_sockaddr.sockaddr, socklen,
                                     pp->src_addr.दत्त, NGX_SOCKADDR_STRLEN, 0);

    pp->dst_addr.दत्त = ngx_pnalloc(c->pool, NGX_SOCKADDR_STRLEN);
    यदि (pp->dst_addr.दत्त == रिक्त) {
        प्रत्यागम रिक्त;
    }

    pp->dst_addr.len = ngx_sock_ntop(&dst_sockaddr.sockaddr, socklen,
                                     pp->dst_addr.दत्त, NGX_SOCKADDR_STRLEN, 0);

    ngx_log_debug4(NGX_LOG_DEBUG_CORE, c->लघुगणकः, 0,
                   "PROXY protocol v2 src: %V %d, dst: %V %d",
                   &pp->src_addr, pp->src_पत्तनम्, &pp->dst_addr, pp->dst_पत्तनम्);

    यदि (buf < अन्तः) {
        pp->tlvs.दत्त = ngx_pnalloc(c->pool, अन्तः - buf);
        यदि (pp->tlvs.दत्त == रिक्त) {
            प्रत्यागम रिक्त;
        }

        ngx_memcpy(pp->tlvs.दत्त, buf, अन्तः - buf);
        pp->tlvs.len = अन्तः - buf;
    }

    c->proxy_protocol = pp;

    प्रत्यागम अन्तः;
}


ngx_int_t
ngx_proxy_protocol_get_tlv(ngx_connection_t *c, ngx_str_t *name,
    ngx_str_t *मानम्)
{
    u_char                          *p;
    आकार_t                           n;
    uint32_t                         verify;
    ngx_str_t                        ssl, *tlvs;
    ngx_int_t                        rc, type;
    ngx_proxy_protocol_tlv_ssl_t    *tlv_ssl;
    ngx_proxy_protocol_tlv_प्रविष्टिः_t  *te;

    यदि (c->proxy_protocol == रिक्त) {
        प्रत्यागम NGX_DECLINED;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->लघुगणकः, 0,
                   "PROXY protocol v2 get tlv \"%V\"", name);

    te = ngx_proxy_protocol_tlv_entries;
    tlvs = &c->proxy_protocol->tlvs;

    p = name->दत्त;
    n = name->len;

    यदि (n >= 4 && p[0] == 's' && p[1] == 's' && p[2] == 'l' && p[3] == '_') {

        rc = ngx_proxy_protocol_lookup_tlv(c, tlvs,
                                           NGX_PROXY_PROTOCOL_TLV_SSL, &ssl);
        यदि (rc != NGX_OK) {
            प्रत्यागम rc;
        }

        यदि (ssl.len < परिमाण(ngx_proxy_protocol_tlv_ssl_t)) {
            प्रत्यागम NGX_ERROR;
        }

        p += 4;
        n -= 4;

        यदि (n == 6 && ngx_strncmp(p, "verify", 6) == 0) {

            tlv_ssl = (ngx_proxy_protocol_tlv_ssl_t *) ssl.दत्त;
            verify = ngx_proxy_protocol_parse_uint32(tlv_ssl->verify);

            मानम्->दत्त = ngx_pnalloc(c->pool, NGX_INT32_LEN);
            यदि (मानम्->दत्त == रिक्त) {
                प्रत्यागम NGX_ERROR;
            }

            मानम्->len = ngx_sprintf(मानम्->दत्त, "%uD", verify)
                         - मानम्->दत्त;
            प्रत्यागम NGX_OK;
        }

        ssl.दत्त += परिमाण(ngx_proxy_protocol_tlv_ssl_t);
        ssl.len -= परिमाण(ngx_proxy_protocol_tlv_ssl_t);

        te = ngx_proxy_protocol_tlv_ssl_entries;
        tlvs = &ssl;
    }

    यदि (n >= 2 && p[0] == '0' && p[1] == 'x') {

        type = ngx_hextoi(p + 2, n - 2);
        यदि (type == NGX_ERROR) {
            ngx_log_error(NGX_LOG_ERR, c->लघुगणकः, 0,
                          "invalid PROXY protocol TLV \"%V\"", name);
            प्रत्यागम NGX_ERROR;
        }

        प्रत्यागम ngx_proxy_protocol_lookup_tlv(c, tlvs, type, मानम्);
    }

    प्रति ( /* void */ ; te->type; te++) {
        यदि (te->name.len == n && ngx_strncmp(te->name.दत्त, p, n) == 0) {
            प्रत्यागम ngx_proxy_protocol_lookup_tlv(c, tlvs, te->type, मानम्);
        }
    }

    ngx_log_error(NGX_LOG_ERR, c->लघुगणकः, 0,
                  "unknown PROXY protocol TLV \"%V\"", name);

    प्रत्यागम NGX_DECLINED;
}


स्थिर ngx_int_t
ngx_proxy_protocol_lookup_tlv(ngx_connection_t *c, ngx_str_t *tlvs,
    ngx_uint_t type, ngx_str_t *मानम्)
{
    u_char                    *p;
    आकार_t                     n, len;
    ngx_proxy_protocol_tlv_t  *tlv;

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->लघुगणकः, 0,
                   "PROXY protocol v2 lookup tlv:%02xi", type);

    p = tlvs->दत्त;
    n = tlvs->len;

    यावत् (n) {
        यदि (n < परिमाण(ngx_proxy_protocol_tlv_t)) {
            ngx_log_error(NGX_LOG_ERR, c->लघुगणकः, 0, "broken PROXY protocol TLV");
            प्रत्यागम NGX_ERROR;
        }

        tlv = (ngx_proxy_protocol_tlv_t *) p;
        len = ngx_proxy_protocol_parse_uint16(tlv->len);

        p += परिमाण(ngx_proxy_protocol_tlv_t);
        n -= परिमाण(ngx_proxy_protocol_tlv_t);

        यदि (n < len) {
            ngx_log_error(NGX_LOG_ERR, c->लघुगणकः, 0, "broken PROXY protocol TLV");
            प्रत्यागम NGX_ERROR;
        }

        यदि (tlv->type == type) {
            मानम्->दत्त = p;
            मानम्->len = len;
            प्रत्यागम NGX_OK;
        }

        p += len;
        n -= len;
    }

    प्रत्यागम NGX_DECLINED;
}


u_char *
ngx_proxy_protocol_v2_write(ngx_connection_t *c, u_char *buf, u_char *चरमः,
    ngx_array_t *tlvs /* reserved */ )
{
    u_char                          *p;
    uint32_t                         verify;
    ngx_uint_t                       i, client;
    ngx_array_t                      आधारः_tlvs, ssl_tlvs;
    ngx_proxy_protocol_tlv_मानम्_t  *tlv;

    client = 0;
    verify = 0;

    ngx_memzero(&आधारः_tlvs, परिमाण(ngx_array_t));
    ngx_memzero(&ssl_tlvs, परिमाण(ngx_array_t));

#यदि (NGX_OPENSSL)

    यदि (c->ssl != रिक्त) {
        यदि (ngx_array_init(&आधारः_tlvs, c->pool, 2,
                           परिमाण(ngx_proxy_protocol_tlv_मानम्_t))
            != NGX_OK)
        {
            प्रत्यागम रिक्त;
        }

        यदि (ngx_array_init(&ssl_tlvs, c->pool, 5,
                           परिमाण(ngx_proxy_protocol_tlv_मानम्_t))
            != NGX_OK)
        {
            प्रत्यागम रिक्त;
        }

        यदि (ngx_proxy_protocol_v2_eval_ssl(c, &आधारः_tlvs, &ssl_tlvs, &client,
                                           &verify)
            != NGX_OK)
        {
            प्रत्यागम रिक्त;
        }
    }

#यदि_अन्तः

    p = ngx_proxy_protocol_v2_write_शीर्षकम्(c, buf, चरमः);
    यदि (p == रिक्त) {
        प्रत्यागम रिक्त;
    }

    tlv = आधारः_tlvs.elts;

    प्रति (i = 0; i < आधारः_tlvs.nelts; i++) {
        p = ngx_proxy_protocol_v2_write_tlv(c, p, चरमः, tlv[i].type,
                                           &tlv[i].मानम्);
        यदि (p == रिक्त) {
            प्रत्यागम रिक्त;
        }
    }

    यदि (client) {
        p = ngx_proxy_protocol_v2_write_ssl(c, p, चरमः, &ssl_tlvs, client,
                                            verify);
        यदि (p == रिक्त) {
            प्रत्यागम रिक्त;
        }
    }

    ngx_proxy_protocol_v2_समुच्चयः_len(buf,
                                  p + परिमाण(ngx_proxy_protocol_tlv_t) + 4);

    प्रत्यागम ngx_proxy_protocol_v2_write_crc32c(c, buf, p, चरमः);
}


#यदि (NGX_OPENSSL)

स्थिर ngx_int_t
ngx_proxy_protocol_v2_eval_ssl(ngx_connection_t *c, ngx_array_t *tlvs,
    ngx_array_t *ssl_tlvs, ngx_uint_t *client, uint32_t *verify)
{
    X509                            *cert;
    दीर्घ                             n;
    ngx_int_t                        rc;
    ngx_str_t                        मानम्;
    ngx_uint_t                       type;
    ध्रुव वर्ण                      *s;
    ngx_proxy_protocol_tlv_मानम्_t  *tlv;

    यदि (ngx_proxy_protocol_v2_authority(c, &मानम्) == NGX_OK) {
        tlv = ngx_array_प्रक्षिप(tlvs);
        यदि (tlv == रिक्त) {
            प्रत्यागम NGX_ERROR;
        }

        tlv->type = NGX_PROXY_PROTOCOL_TLV_AUTHORITY;
        tlv->मानम् = मानम्;
    }

    यदि (ngx_proxy_protocol_v2_alpn(c, &मानम्) == NGX_OK) {
        tlv = ngx_array_प्रक्षिप(tlvs);
        यदि (tlv == रिक्त) {
            प्रत्यागम NGX_ERROR;
        }

        tlv->type = NGX_PROXY_PROTOCOL_TLV_ALPN;
        tlv->मानम् = मानम्;
    }

    प्रति (type = NGX_PROXY_PROTOCOL_TLV_SSL_संस्करणम्;
         type <= NGX_PROXY_PROTOCOL_TLV_SSL_कुञ्चिका_ALG;
         type++)
    {
        rc = ngx_proxy_protocol_v2_ssl_sub(c, type, &मानम्);

        यदि (rc == NGX_ERROR) {
            प्रत्यागम NGX_ERROR;
        }

        यदि (rc != NGX_OK) {
            अनुवर्त;
        }

        tlv = ngx_array_प्रक्षिप(ssl_tlvs);
        यदि (tlv == रिक्त) {
            प्रत्यागम NGX_ERROR;
        }

        tlv->type = type;
        tlv->मानम् = मानम्;
    }

    *client = NGX_PROXY_PROTOCOL_V2_CLIENT_SSL;
    *verify = 1;  /* X509_V_ERR_UNSPECIFIED */

    cert = SSL_get_peer_certificate(c->ssl->connection);

    यदि (cert != रिक्त) {
        X509_free(cert);

        *client |= NGX_PROXY_PROTOCOL_V2_CLIENT_CERT_SESS;

        यदि (!SSL_session_reused(c->ssl->connection)) {
            *client |= NGX_PROXY_PROTOCOL_V2_CLIENT_CERT_CONN;
        }

        n = SSL_get_verify_परिणाम(c->ssl->connection);

        यदि (n == X509_V_OK) {

            यदि (ngx_ssl_ocsp_get_स्थितिः(c, &s) != NGX_OK) {
                n = X509_V_ERR_CERT_REVOKED;
            }
        }

        *verify = (uint32_t) n;
    }

    प्रत्यागम NGX_OK;
}


स्थिर ngx_int_t
ngx_proxy_protocol_v2_authority(ngx_connection_t *c, ngx_str_t *बहिर्गमनम्)
{
#यदि_परिभाषितम् SSL_CTRL_समुच्चयः_TLSEXT_HOSTNAME

    ध्रुव वर्ण  *sni;

    sni = SSL_get_servername(c->ssl->connection, TLSEXT_NAMETYPE_host_name);
    यदि (sni == रिक्त) {
        प्रत्यागम NGX_DECLINED;
    }

    बहिर्गमनम्->दत्त = (u_char *) sni;
    बहिर्गमनम्->len = ngx_strlen(sni);
    प्रत्यागम NGX_OK;

#अन्यथा
    प्रत्यागम NGX_DECLINED;
#यदि_अन्तः
}


स्थिर ngx_int_t
ngx_proxy_protocol_v2_alpn(ngx_connection_t *c, ngx_str_t *बहिर्गमनम्)
{
#यदि_परिभाषितम् TLSEXT_TYPE_application_layer_protocol_negotiation

    अचिह्नित पूर्णाङ्क          alpnlen;
    ध्रुव अचिह्नित वर्ण  *alpn;

    SSL_get0_alpn_selected(c->ssl->connection, &alpn, &alpnlen);
    यदि (alpn == रिक्त || alpnlen == 0) {
        प्रत्यागम NGX_DECLINED;
    }

    बहिर्गमनम्->दत्त = (u_char *) alpn;
    बहिर्गमनम्->len = alpnlen;
    प्रत्यागम NGX_OK;

#अन्यथा
    प्रत्यागम NGX_DECLINED;
#यदि_अन्तः
}


स्थिर ngx_int_t
ngx_proxy_protocol_v2_ssl_sub(ngx_connection_t *c, ngx_uint_t type,
    ngx_str_t *बहिर्गमनम्)
{
    पूर्णाङ्क               i, len;
    X509             *cert;
    u_char           *alg, *s;
    EVP_PKEY         *pkey;
#यदि (OPENSSL_संस्करणम्_सङ्ख्या >= 0x40000000L)
    ध्रुव
#यदि_अन्तः
    X509_NAME        *subject;
#यदि (OPENSSL_संस्करणम्_सङ्ख्या >= 0x40000000L)
    ध्रुव
#यदि_अन्तः
    X509_NAME_प्रविष्टिः  *प्रविष्टिः;

    विकल्प (type) {

    पक्ष NGX_PROXY_PROTOCOL_TLV_SSL_संस्करणम्:
        बहिर्गमनम्->दत्त = (u_char *) SSL_get_संस्करणम्(c->ssl->connection);
        बहिर्गमनम्->len = ngx_strlen(बहिर्गमनम्->दत्त);
        प्रत्यागम NGX_OK;

    पक्ष NGX_PROXY_PROTOCOL_TLV_SSL_CIPHER:
        बहिर्गमनम्->दत्त = (u_char *) SSL_get_cipher_name(c->ssl->connection);
        बहिर्गमनम्->len = ngx_strlen(बहिर्गमनम्->दत्त);
        प्रत्यागम NGX_OK;

    पक्ष NGX_PROXY_PROTOCOL_TLV_SSL_CN:
        cert = SSL_get_peer_certificate(c->ssl->connection);
        यदि (cert == रिक्त) {
            प्रत्यागम NGX_DECLINED;
        }

        subject = X509_get_subject_name(cert);
        यदि (subject == रिक्त) {
            X509_free(cert);
            प्रत्यागम NGX_ERROR;
        }

        i = X509_NAME_get_सूचकांक_by_NID(subject, NID_commonName, -1);
        यदि (i < 0) {
            X509_free(cert);
            प्रत्यागम NGX_DECLINED;
        }

        प्रविष्टिः = X509_NAME_get_प्रविष्टिः(subject, i);

        len = ASN1_STRING_गन्तव्यम्_UTF8(&s, X509_NAME_प्रविष्टिः_get_दत्त(प्रविष्टिः));
        यदि (len < 0) {
            ngx_ssl_error(NGX_LOG_ALERT, c->लघुगणकः, 0,
                          "ASN1_STRING_to_UTF8() failed");
            X509_free(cert);
            प्रत्यागम NGX_ERROR;
        }

        बहिर्गमनम्->len = len;
        बहिर्गमनम्->दत्त = ngx_pnalloc(c->pool, len);
        यदि (बहिर्गमनम्->दत्त == रिक्त) {
            OPENSSL_free(s);
            X509_free(cert);
            प्रत्यागम NGX_ERROR;
        }

        ngx_memcpy(बहिर्गमनम्->दत्त, s, len);

        OPENSSL_free(s);
        X509_free(cert);
        प्रत्यागम NGX_OK;

    पक्ष NGX_PROXY_PROTOCOL_TLV_SSL_SIG_ALG:
        cert = SSL_get_certificate(c->ssl->connection);

        बहिर्गमनम्->दत्त = (u_char *) OBJ_nid2sn(X509_get_signature_nid(cert));
        बहिर्गमनम्->len = ngx_strlen(बहिर्गमनम्->दत्त);
        प्रत्यागम NGX_OK;

    पक्ष NGX_PROXY_PROTOCOL_TLV_SSL_कुञ्चिका_ALG:
        cert = SSL_get_certificate(c->ssl->connection);

        pkey = X509_get_pubkey(cert);
        यदि (pkey == रिक्त) {
            प्रत्यागम NGX_ERROR;
        }

        विकल्प (EVP_PKEY_आधारः_id(pkey)) {

        पक्ष EVP_PKEY_RSA:
#यदि_परिभाषितम् EVP_PKEY_RSA_PSS
        पक्ष EVP_PKEY_RSA_PSS:
#यदि_अन्तः
            alg = (u_char *) "RSA";
            भङ्ग;

        पक्ष EVP_PKEY_EC:
            alg = (u_char *) "EC";
            भङ्ग;

        पक्ष EVP_PKEY_DSA:
            alg = (u_char *) "DSA";
            भङ्ग;

        स्वभाव:
            EVP_PKEY_free(pkey);
            प्रत्यागम NGX_DECLINED;
        }

        बहिर्गमनम्->len = परिमाण("RSA16384") - 1;
        बहिर्गमनम्->दत्त = ngx_pnalloc(c->pool, बहिर्गमनम्->len);
        यदि (बहिर्गमनम्->दत्त == रिक्त) {
            EVP_PKEY_free(pkey);
            प्रत्यागम NGX_ERROR;
        }

        बहिर्गमनम्->len = ngx_snprintf(बहिर्गमनम्->दत्त, बहिर्गमनम्->len, "%s%d",
                                alg, EVP_PKEY_द्वयङ्काः(pkey))
                   - बहिर्गमनम्->दत्त;

        EVP_PKEY_free(pkey);

        प्रत्यागम NGX_OK;
    }

    प्रत्यागम NGX_DECLINED;
}

#यदि_अन्तः


स्थिर u_char *
ngx_proxy_protocol_v2_write_शीर्षकम्(ngx_connection_t *c, u_char *buf,
    u_char *चरमः)
{
    u_char                            *p, src_af, dst_af,
                                       family, command, transport;
    ngx_uint_t                         src_fam, dst_fam;
    ngx_proxy_protocol_शीर्षकम्_t       *शीर्षकम्;
    ngx_proxy_protocol_inet_addrs_t   *अन्तर्;
#यदि (NGX_HAVE_INET6)
    ngx_uint_t                         पत्तनम्, lport;
    संरचना in6_addr                   *src6, *dst6;
    ngx_proxy_protocol_inet6_addrs_t  *in6;
#यदि_अन्तः

    यदि (चरमः - buf < NGX_PROXY_PROTOCOL_V2_MAX_शीर्षकम्) {
        ngx_log_error(NGX_LOG_ALERT, c->लघुगणकः, 0,
                      "too small buffer for PROXY protocol v2");
        प्रत्यागम रिक्त;
    }

    यदि (ngx_connection_local_sockaddr(c, रिक्त, 0) != NGX_OK) {
        प्रत्यागम रिक्त;
    }

    /* the length field is set by the caller once all TLVs are written */

    शीर्षकम् = (ngx_proxy_protocol_शीर्षकम्_t *) buf;

    ngx_memcpy(शीर्षकम्->signature, ngx_proxy_protocol_signature,
               परिमाण(ngx_proxy_protocol_signature) - 1);

    p = buf + परिमाण(ngx_proxy_protocol_शीर्षकम्_t);

    src_fam = c->sockaddr->sa_family;
    dst_fam = c->local_sockaddr->sa_family;

    src_af = ngx_proxy_protocol_v2_family(src_fam);
    dst_af = ngx_proxy_protocol_v2_family(dst_fam);

    /* promote to the highest address family present on either side */

    यदि (src_af == NGX_PROXY_PROTOCOL_AF_UNSPEC
        || dst_af == NGX_PROXY_PROTOCOL_AF_UNSPEC)
    {
        command = NGX_PROXY_PROTOCOL_CMD_LOCAL;
        family = NGX_PROXY_PROTOCOL_AF_UNSPEC;
        transport = NGX_PROXY_PROTOCOL_TYPE_UNSPEC;

    } अन्यथा {
        command = NGX_PROXY_PROTOCOL_CMD_PROXY;
        family = ngx_max(src_af, dst_af);

        विकल्प (c->type) {
        पक्ष SOCK_प्रवाहः:
            transport = NGX_PROXY_PROTOCOL_TYPE_प्रवाहः;
            भङ्ग;
        पक्ष SOCK_DGRAM:
            transport = NGX_PROXY_PROTOCOL_TYPE_DGRAM;
            भङ्ग;
        स्वभाव:
            transport = NGX_PROXY_PROTOCOL_TYPE_UNSPEC;
            भङ्ग;
        }
    }

    शीर्षकम्->संस्करणम्_command = 0x20 | command;
    शीर्षकम्->family_transport = (family << 4) | transport;

    विकल्प (family) {

    पक्ष NGX_PROXY_PROTOCOL_AF_INET:

        अन्तर् = (ngx_proxy_protocol_inet_addrs_t *) p;

        ngx_proxy_protocol_v2_write_ipv4(c->sockaddr,
                                         अन्तर्->src_addr, अन्तर्->src_पत्तनम्);
        ngx_proxy_protocol_v2_write_ipv4(c->local_sockaddr,
                                         अन्तर्->dst_addr, अन्तर्->dst_पत्तनम्);

        p += परिमाण(ngx_proxy_protocol_inet_addrs_t);

        भङ्ग;

#यदि (NGX_HAVE_INET6)

    पक्ष NGX_PROXY_PROTOCOL_AF_INET6:

        यदि (src_fam == AF_INET6 && dst_fam == AF_INET6) {
            src6 = &((संरचना sockaddr_in6 *) c->sockaddr)->sin6_addr;
            dst6 = &((संरचना sockaddr_in6 *) c->local_sockaddr)->sin6_addr;

            यदि (IN6_IS_ADDR_V4MAPPED(src6) && IN6_IS_ADDR_V4MAPPED(dst6)) {

                /* both v4-mapped: demote to AF_INET */

                शीर्षकम्->family_transport = (NGX_PROXY_PROTOCOL_AF_INET << 4)
                                           | transport;

                अन्तर् = (ngx_proxy_protocol_inet_addrs_t *) p;

                ngx_memcpy(अन्तर्->src_addr, src6->s6_addr + 12, 4);
                ngx_memcpy(अन्तर्->dst_addr, dst6->s6_addr + 12, 4);

                पत्तनम्  = ngx_inet_get_पत्तनम्(c->sockaddr);
                lport = ngx_inet_get_पत्तनम्(c->local_sockaddr);

                अन्तर्->src_पत्तनम्[0] = (u_char) (पत्तनम् >> 8);
                अन्तर्->src_पत्तनम्[1] = (u_char)  पत्तनम्;
                अन्तर्->dst_पत्तनम्[0] = (u_char) (lport >> 8);
                अन्तर्->dst_पत्तनम्[1] = (u_char)  lport;

                p += परिमाण(ngx_proxy_protocol_inet_addrs_t);
                भङ्ग;
            }
        }

        in6 = (ngx_proxy_protocol_inet6_addrs_t *) p;

        ngx_proxy_protocol_v2_write_ipv6(c->sockaddr,
                                         in6->src_addr, in6->src_पत्तनम्);
        ngx_proxy_protocol_v2_write_ipv6(c->local_sockaddr,
                                         in6->dst_addr, in6->dst_पत्तनम्);

        p += परिमाण(ngx_proxy_protocol_inet6_addrs_t);

        भङ्ग;

#यदि_अन्तः

    स्वभाव:  /* NGX_PROXY_PROTOCOL_AF_UNSPEC */
        भङ्ग;
    }

    प्रत्यागम p;
}


स्थिर u_char
ngx_proxy_protocol_v2_family(ngx_uint_t family)
{
    विकल्प (family) {

    पक्ष AF_INET:
        प्रत्यागम NGX_PROXY_PROTOCOL_AF_INET;

#यदि (NGX_HAVE_INET6)
    पक्ष AF_INET6:
        प्रत्यागम NGX_PROXY_PROTOCOL_AF_INET6;
#यदि_अन्तः

    स्वभाव:
        प्रत्यागम NGX_PROXY_PROTOCOL_AF_UNSPEC;
    }
}


स्थिर शून्य
ngx_proxy_protocol_v2_write_ipv4(संरचना sockaddr *sa, u_char *addr,
    u_char *पत्तनम्)
{
    संरचना sockaddr_अन्तर्  *ज्या;

    ज्या = (संरचना sockaddr_अन्तर् *) sa;
    ngx_memcpy(addr, &ज्या->sin_addr, 4);
    ngx_memcpy(पत्तनम्, &ज्या->sin_पत्तनम्, 2);
}


#यदि (NGX_HAVE_INET6)

स्थिर शून्य
ngx_proxy_protocol_v2_write_ipv6(संरचना sockaddr *sa, u_char *addr,
    u_char *पत्तनम्)
{
    संरचना sockaddr_अन्तर्   *ज्या;
    संरचना sockaddr_in6  *sin6;

    /* IPv4-mapped prefix: 80 zero bits + 16 one bits */
    स्थिर ध्रुव u_char  mapped[]   = { 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0xff, 0xff };

    यदि (sa->sa_family == AF_INET6) {
        sin6 = (संरचना sockaddr_in6 *) sa;
        ngx_memcpy(addr, &sin6->sin6_addr, 16);
        ngx_memcpy(पत्तनम्, &sin6->sin6_पत्तनम्, 2);

    } अन्यथा {
        /* promote AF_INET to ::ffff:a.b.c.d */
        ज्या = (संरचना sockaddr_अन्तर् *) sa;
        ngx_memcpy(addr, mapped, परिमाण(mapped));
        ngx_memcpy(addr + परिमाण(mapped), &ज्या->sin_addr, 4);
        ngx_memcpy(पत्तनम्, &ज्या->sin_पत्तनम्, 2);
    }
}

#यदि_अन्तः


स्थिर u_char *
ngx_proxy_protocol_v2_write_tlv(ngx_connection_t *c, u_char *p, u_char *चरमः,
    ngx_uint_t type, ngx_str_t *मानम्)
{
    ngx_proxy_protocol_tlv_t  *tlv;

    यदि ((आकार_t) (चरमः - p) < परिमाण(ngx_proxy_protocol_tlv_t) + मानम्->len) {
        ngx_log_error(NGX_LOG_ALERT, c->लघुगणकः, 0,
                      "too small buffer for PROXY protocol v2 TLV");
        प्रत्यागम रिक्त;
    }

    tlv = (ngx_proxy_protocol_tlv_t *) p;
    tlv->type = (u_char) type;
    tlv->len[0] = (u_char) (मानम्->len >> 8);
    tlv->len[1] = (u_char)  मानम्->len;

    p += परिमाण(ngx_proxy_protocol_tlv_t);

    प्रत्यागम ngx_cpymem(p, मानम्->दत्त, मानम्->len);
}


स्थिर u_char *
ngx_proxy_protocol_v2_write_ssl(ngx_connection_t *c, u_char *p, u_char *चरमः,
    ngx_array_t *ssl_tlvs, ngx_uint_t client, uint32_t verify)
{
    u_char                          *start;
    uint16_t                         len;
    ngx_uint_t                       i;
    ngx_proxy_protocol_tlv_t        *tlv;
    ngx_proxy_protocol_tlv_ssl_t    *tlv_ssl;
    ngx_proxy_protocol_tlv_मानम्_t  *sub;

    यदि ((आकार_t) (चरमः - p) < परिमाण(ngx_proxy_protocol_tlv_t)
                              + परिमाण(ngx_proxy_protocol_tlv_ssl_t))
    {
        ngx_log_error(NGX_LOG_ALERT, c->लघुगणकः, 0,
                      "too small buffer for PROXY protocol v2 SSL TLV");
        प्रत्यागम रिक्त;
    }

    start = p;
    p += परिमाण(ngx_proxy_protocol_tlv_t);

    tlv_ssl = (ngx_proxy_protocol_tlv_ssl_t *) p;
    tlv_ssl->client = (u_char) client;
    tlv_ssl->verify[0] = (u_char) (verify >> 24);
    tlv_ssl->verify[1] = (u_char) (verify >> 16);
    tlv_ssl->verify[2] = (u_char) (verify >> 8);
    tlv_ssl->verify[3] = (u_char)  verify;

    p += परिमाण(ngx_proxy_protocol_tlv_ssl_t);

    sub = ssl_tlvs->elts;

    प्रति (i = 0; i < ssl_tlvs->nelts; i++) {
        p = ngx_proxy_protocol_v2_write_tlv(c, p, चरमः, sub[i].type,
                                            &sub[i].मानम्);
        यदि (p == रिक्त) {
            प्रत्यागम रिक्त;
        }
    }

    len = (uint16_t) (p - start - परिमाण(ngx_proxy_protocol_tlv_t));

    tlv = (ngx_proxy_protocol_tlv_t *) start;
    tlv->type = NGX_PROXY_PROTOCOL_TLV_SSL;
    tlv->len[0] = (u_char) (len >> 8);
    tlv->len[1] = (u_char)  len;

    प्रत्यागम p;
}


स्थिर शून्य
ngx_proxy_protocol_v2_समुच्चयः_len(u_char *buf, u_char *p)
{
    uint16_t                      len;
    ngx_proxy_protocol_शीर्षकम्_t  *शीर्षकम्;

    len = (uint16_t) (p - buf - परिमाण(ngx_proxy_protocol_शीर्षकम्_t));

    शीर्षकम् = (ngx_proxy_protocol_शीर्षकम्_t *) buf;
    शीर्षकम्->len[0] = (u_char) (len >> 8);
    शीर्षकम्->len[1] = (u_char) len;
}


स्थिर u_char *
ngx_proxy_protocol_v2_write_crc32c(ngx_connection_t *c, u_char *buf,
    u_char *p, u_char *चरमः)
{
    u_char                    *मानम्;
    uint32_t                   crc;
    ngx_proxy_protocol_tlv_t  *tlv;

    यदि ((आकार_t) (चरमः - p) < परिमाण(ngx_proxy_protocol_tlv_t) + 4) {
        ngx_log_error(NGX_LOG_ALERT, c->लघुगणकः, 0,
                      "too small buffer for PROXY protocol v2 CRC32c TLV");
        प्रत्यागम रिक्त;
    }

    /* append the CRC32c TLV with a zeroed value field */

    tlv = (ngx_proxy_protocol_tlv_t *) p;
    tlv->type = NGX_PROXY_PROTOCOL_TLV_चक्रीयपरीक्षा_32_C;
    tlv->len[0] = 0;
    tlv->len[1] = 4;

    मानम् = p + परिमाण(ngx_proxy_protocol_tlv_t);

    ngx_memzero(मानम्, 4);

    p = मानम् + 4;

    /* the checksum covers the entire header with the zeroed value */

    crc = ngx_crc32c_long(buf, p - buf);

    मानम्[0] = (u_char) (crc >> 24);
    मानम्[1] = (u_char) (crc >> 16);
    मानम्[2] = (u_char) (crc >> 8);
    मानम्[3] = (u_char) crc;

    प्रत्यागम p;
}
