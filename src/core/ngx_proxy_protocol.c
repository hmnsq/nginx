
/*
 * Copyright (C) Roman Arutyunyan
 * Copyright (C) Nginx, Inc.
 */


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>


#నిర్వచించు NGX_PROXY_PROTOCOL_V2_MAX_శీర్షిక    52

#నిర్వచించు NGX_PROXY_PROTOCOL_CMD_LOCAL        0
#నిర్వచించు NGX_PROXY_PROTOCOL_CMD_PROXY        1

#నిర్వచించు NGX_PROXY_PROTOCOL_AF_UNSPEC        0
#నిర్వచించు NGX_PROXY_PROTOCOL_AF_INET          1
#నిర్వచించు NGX_PROXY_PROTOCOL_AF_INET6         2

#నిర్వచించు NGX_PROXY_PROTOCOL_TYPE_UNSPEC      0
#నిర్వచించు NGX_PROXY_PROTOCOL_TYPE_ప్రవాహం      1
#నిర్వచించు NGX_PROXY_PROTOCOL_TYPE_DGRAM       2

#నిర్వచించు NGX_PROXY_PROTOCOL_TLV_ALPN         0x01
#నిర్వచించు NGX_PROXY_PROTOCOL_TLV_AUTHORITY    0x02
#నిర్వచించు NGX_PROXY_PROTOCOL_TLV_చక్రీయ_తనిఖీ_32_C       0x03
#నిర్వచించు NGX_PROXY_PROTOCOL_TLV_UNIQUE_ID    0x05
#నిర్వచించు NGX_PROXY_PROTOCOL_TLV_SSL          0x20
#నిర్వచించు NGX_PROXY_PROTOCOL_TLV_SSL_సంస్కరణ  0x21
#నిర్వచించు NGX_PROXY_PROTOCOL_TLV_SSL_CN       0x22
#నిర్వచించు NGX_PROXY_PROTOCOL_TLV_SSL_CIPHER   0x23
#నిర్వచించు NGX_PROXY_PROTOCOL_TLV_SSL_SIG_ALG  0x24
#నిర్వచించు NGX_PROXY_PROTOCOL_TLV_SSL_కీలకం_ALG  0x25
#నిర్వచించు NGX_PROXY_PROTOCOL_TLV_NETNS        0x30

#నిర్వచించు NGX_PROXY_PROTOCOL_V2_CLIENT_SSL          0x01
#నిర్వచించు NGX_PROXY_PROTOCOL_V2_CLIENT_CERT_CONN    0x02
#నిర్వచించు NGX_PROXY_PROTOCOL_V2_CLIENT_CERT_SESS    0x04


#నిర్వచించు ngx_proxy_protocol_parse_uint16(p)                                    \
    ( ((uint16_t) (p)[0] << 8)                                                \
    + (           (p)[1]) )

#నిర్వచించు ngx_proxy_protocol_parse_uint32(p)                                    \
    ( ((uint32_t) (p)[0] << 24)                                               \
    + (           (p)[1] << 16)                                               \
    + (           (p)[2] << 8)                                                \
    + (           (p)[3]) )


రకనిర్వచనం నిర్మాణం {
    u_char                                  signature[12];
    u_char                                  సంస్కరణ_command;
    u_char                                  family_transport;
    u_char                                  len[2];
} ngx_proxy_protocol_శీర్షిక_t;


రకనిర్వచనం నిర్మాణం {
    u_char                                  src_addr[4];
    u_char                                  dst_addr[4];
    u_char                                  src_పోర్టు[2];
    u_char                                  dst_పోర్టు[2];
} ngx_proxy_protocol_inet_addrs_t;


రకనిర్వచనం నిర్మాణం {
    u_char                                  src_addr[16];
    u_char                                  dst_addr[16];
    u_char                                  src_పోర్టు[2];
    u_char                                  dst_పోర్టు[2];
} ngx_proxy_protocol_inet6_addrs_t;


రకనిర్వచనం నిర్మాణం {
    u_char                                  type;
    u_char                                  len[2];
} ngx_proxy_protocol_tlv_t;


రకనిర్వచనం నిర్మాణం {
    u_char                                  client;
    u_char                                  verify[4];
} ngx_proxy_protocol_tlv_ssl_t;


రకనిర్వచనం నిర్మాణం {
    ngx_str_t                               name;
    ngx_uint_t                              type;
} ngx_proxy_protocol_tlv_నమోదు_t;


రకనిర్వచనం నిర్మాణం {
    ngx_uint_t                              type;
    ngx_str_t                               విలువ;
} ngx_proxy_protocol_tlv_విలువ_t;


స్థిర u_char *ngx_proxy_protocol_read_addr(ngx_connection_t *c, u_char *p,
    u_char *చివరి, ngx_str_t *addr);
స్థిర u_char *ngx_proxy_protocol_read_పోర్టు(u_char *p, u_char *చివరి,
    లోపల_పోర్టు_t *పోర్టు, u_char sep);
స్థిర u_char *ngx_proxy_protocol_v2_read(ngx_connection_t *c, u_char *buf,
    u_char *చివరి);
స్థిర ngx_int_t ngx_proxy_protocol_lookup_tlv(ngx_connection_t *c,
    ngx_str_t *tlvs, ngx_uint_t type, ngx_str_t *విలువ);
#ఒకవేళ (NGX_OPENSSL)
స్థిర ngx_int_t ngx_proxy_protocol_v2_eval_ssl(ngx_connection_t *c,
    ngx_array_t *tlvs, ngx_array_t *ssl_tlvs, ngx_uint_t *client,
    uint32_t *verify);
స్థిర ngx_int_t ngx_proxy_protocol_v2_authority(ngx_connection_t *c,
    ngx_str_t *బహిర్గతం);
స్థిర ngx_int_t ngx_proxy_protocol_v2_alpn(ngx_connection_t *c,
    ngx_str_t *బహిర్గతం);
స్థిర ngx_int_t ngx_proxy_protocol_v2_ssl_sub(ngx_connection_t *c,
    ngx_uint_t type, ngx_str_t *బహిర్గతం);
#అయితే_ముగింపు
స్థిర u_char *ngx_proxy_protocol_v2_write_శీర్షిక(ngx_connection_t *c,
    u_char *buf, u_char *చివరి);
స్థిర u_char ngx_proxy_protocol_v2_family(ngx_uint_t family);
స్థిర శూన్య ngx_proxy_protocol_v2_write_ipv4(నిర్మాణం sockaddr *sa, u_char *addr,
    u_char *పోర్టు);
స్థిర శూన్య ngx_proxy_protocol_v2_write_ipv6(నిర్మాణం sockaddr *sa, u_char *addr,
    u_char *పోర్టు);
స్థిర u_char *ngx_proxy_protocol_v2_write_tlv(ngx_connection_t *c, u_char *p,
    u_char *చివరి, ngx_uint_t type, ngx_str_t *విలువ);
స్థిర u_char *ngx_proxy_protocol_v2_write_ssl(ngx_connection_t *c, u_char *p,
    u_char *చివరి, ngx_array_t *ssl_tlvs, ngx_uint_t client, uint32_t verify);
స్థిర శూన్య ngx_proxy_protocol_v2_సమితి_len(u_char *buf, u_char *p);
స్థిర u_char *ngx_proxy_protocol_v2_write_crc32c(ngx_connection_t *c,
    u_char *buf, u_char *p, u_char *చివరి);


స్థిర ngx_proxy_protocol_tlv_నమోదు_t  ngx_proxy_protocol_tlv_entries[] = {
    { ngx_string("alpn"),       NGX_PROXY_PROTOCOL_TLV_ALPN },
    { ngx_string("authority"),  NGX_PROXY_PROTOCOL_TLV_AUTHORITY },
    { ngx_string("unique_id"),  NGX_PROXY_PROTOCOL_TLV_UNIQUE_ID },
    { ngx_string("ssl"),        NGX_PROXY_PROTOCOL_TLV_SSL },
    { ngx_string("netns"),      NGX_PROXY_PROTOCOL_TLV_NETNS },
    { ngx_null_string,          0x00 }
};


స్థిర ngx_proxy_protocol_tlv_నమోదు_t  ngx_proxy_protocol_tlv_ssl_entries[] = {
    { ngx_string("version"),    NGX_PROXY_PROTOCOL_TLV_SSL_సంస్కరణ },
    { ngx_string("cn"),         NGX_PROXY_PROTOCOL_TLV_SSL_CN },
    { ngx_string("cipher"),     NGX_PROXY_PROTOCOL_TLV_SSL_CIPHER },
    { ngx_string("sig_alg"),    NGX_PROXY_PROTOCOL_TLV_SSL_SIG_ALG },
    { ngx_string("key_alg"),    NGX_PROXY_PROTOCOL_TLV_SSL_కీలకం_ALG },
    { ngx_null_string,          0x00 }
};


స్థిర స్థిరరాశి u_char  ngx_proxy_protocol_signature[] = "\r\n\r\n\0\r\nQUIT\n";


u_char *
ngx_proxy_protocol_read(ngx_connection_t *c, u_char *buf, u_char *చివరి)
{
    కొలత_t                 len;
    u_char                *p;
    ngx_proxy_protocol_t  *pp;

    p = buf;
    len = చివరి - buf;

    ఒకవేళ (len >= పరిమాణం(ngx_proxy_protocol_శీర్షిక_t)
        && ngx_memcmp(p, ngx_proxy_protocol_signature,
                      పరిమాణం(ngx_proxy_protocol_signature) - 1)
           == 0)
    {
        తిరిగి ngx_proxy_protocol_v2_read(c, buf, చివరి);
    }

    ఒకవేళ (len < 8 || ngx_strncmp(p, "PROXY ", 6) != 0) {
        వెళ్ళు invalid;
    }

    p += 6;
    len -= 6;

    ఒకవేళ (len >= 7 && ngx_strncmp(p, "UNKNOWN", 7) == 0) {
        ngx_log_debug0(NGX_LOG_DEBUG_CORE, c->సంవర్గమానం, 0,
                       "PROXY protocol unknown protocol");
        p += 7;
        వెళ్ళు skip;
    }

    ఒకవేళ (len < 5 || ngx_strncmp(p, "TCP", 3) != 0
        || (p[3] != '4' && p[3] != '6') || p[4] != ' ')
    {
        వెళ్ళు invalid;
    }

    p += 5;

    pp = ngx_pcalloc(c->pool, పరిమాణం(ngx_proxy_protocol_t));
    ఒకవేళ (pp == ఖాళీ) {
        తిరిగి ఖాళీ;
    }

    p = ngx_proxy_protocol_read_addr(c, p, చివరి, &pp->src_addr);
    ఒకవేళ (p == ఖాళీ) {
        వెళ్ళు invalid;
    }

    p = ngx_proxy_protocol_read_addr(c, p, చివరి, &pp->dst_addr);
    ఒకవేళ (p == ఖాళీ) {
        వెళ్ళు invalid;
    }

    p = ngx_proxy_protocol_read_పోర్టు(p, చివరి, &pp->src_పోర్టు, ' ');
    ఒకవేళ (p == ఖాళీ) {
        వెళ్ళు invalid;
    }

    p = ngx_proxy_protocol_read_పోర్టు(p, చివరి, &pp->dst_పోర్టు, CR);
    ఒకవేళ (p == ఖాళీ) {
        వెళ్ళు invalid;
    }

    ఒకవేళ (p == చివరి) {
        వెళ్ళు invalid;
    }

    ఒకవేళ (*p++ != LF) {
        వెళ్ళు invalid;
    }

    ngx_log_debug4(NGX_LOG_DEBUG_CORE, c->సంవర్గమానం, 0,
                   "PROXY protocol src: %V %d, dst: %V %d",
                   &pp->src_addr, pp->src_పోర్టు, &pp->dst_addr, pp->dst_పోర్టు);

    c->proxy_protocol = pp;

    తిరిగి p;

skip:

    కొరకు ( /* void */ ; p < చివరి - 1; p++) {
        ఒకవేళ (p[0] == CR && p[1] == LF) {
            తిరిగి p + 2;
        }
    }

invalid:

    కొరకు (p = buf; p < చివరి; p++) {
        ఒకవేళ (*p == CR || *p == LF) {
            ఆపు;
        }
    }

    ngx_log_error(NGX_LOG_ERR, c->సంవర్గమానం, 0,
                  "broken header: \"%*s\"", (కొలత_t) (p - buf), buf);

    తిరిగి ఖాళీ;
}


స్థిర u_char *
ngx_proxy_protocol_read_addr(ngx_connection_t *c, u_char *p, u_char *చివరి,
    ngx_str_t *addr)
{
    కొలత_t  len;
    u_char  ch, *pos;

    pos = p;

    కొరకు ( ;; ) {
        ఒకవేళ (p == చివరి) {
            తిరిగి ఖాళీ;
        }

        ch = *p++;

        ఒకవేళ (ch == ' ') {
            ఆపు;
        }

        ఒకవేళ (ch != ':' && ch != '.'
            && (ch < 'a' || ch > 'f')
            && (ch < 'A' || ch > 'F')
            && (ch < '0' || ch > '9'))
        {
            తిరిగి ఖాళీ;
        }
    }

    len = p - pos - 1;

    addr->దత్తాంశం = ngx_pnalloc(c->pool, len);
    ఒకవేళ (addr->దత్తాంశం == ఖాళీ) {
        తిరిగి ఖాళీ;
    }

    ngx_memcpy(addr->దత్తాంశం, pos, len);
    addr->len = len;

    తిరిగి p;
}


స్థిర u_char *
ngx_proxy_protocol_read_పోర్టు(u_char *p, u_char *చివరి, లోపల_పోర్టు_t *పోర్టు,
    u_char sep)
{
    కొలత_t      len;
    u_char     *pos;
    ngx_int_t   n;

    pos = p;

    కొరకు ( ;; ) {
        ఒకవేళ (p == చివరి) {
            తిరిగి ఖాళీ;
        }

        ఒకవేళ (*p++ == sep) {
            ఆపు;
        }
    }

    len = p - pos - 1;

    n = ngx_atoi(pos, len);
    ఒకవేళ (n < 0 || n > 65535) {
        తిరిగి ఖాళీ;
    }

    *పోర్టు = (లోపల_పోర్టు_t) n;

    తిరిగి p;
}


u_char *
ngx_proxy_protocol_write(ngx_connection_t *c, u_char *buf, u_char *చివరి)
{
    ngx_uint_t  పోర్టు, lport;

    ఒకవేళ (చివరి - buf < NGX_PROXY_PROTOCOL_V1_MAX_శీర్షిక) {
        ngx_log_error(NGX_LOG_ALERT, c->సంవర్గమానం, 0,
                      "too small buffer for PROXY protocol");
        తిరిగి ఖాళీ;
    }

    ఒకవేళ (ngx_connection_local_sockaddr(c, ఖాళీ, 0) != NGX_OK) {
        తిరిగి ఖాళీ;
    }

    ఎంపిక (c->sockaddr->sa_family) {

    స్థితి AF_INET:
        buf = ngx_cpymem(buf, "PROXY TCP4 ", పరిమాణం("PROXY TCP4 ") - 1);
        ఆపు;

#ఒకవేళ (NGX_HAVE_INET6)
    స్థితి AF_INET6:
        buf = ngx_cpymem(buf, "PROXY TCP6 ", పరిమాణం("PROXY TCP6 ") - 1);
        ఆపు;
#అయితే_ముగింపు

    అప్రమేయ:
        తిరిగి ngx_cpymem(buf, "PROXY UNKNOWN" CRLF,
                          పరిమాణం("PROXY UNKNOWN" CRLF) - 1);
    }

    buf += ngx_sock_ntop(c->sockaddr, c->socklen, buf, చివరి - buf, 0);

    *buf++ = ' ';

    buf += ngx_sock_ntop(c->local_sockaddr, c->local_socklen, buf, చివరి - buf,
                         0);

    పోర్టు = ngx_inet_get_పోర్టు(c->sockaddr);
    lport = ngx_inet_get_పోర్టు(c->local_sockaddr);

    తిరిగి ngx_slprintf(buf, చివరి, " %ui %ui" CRLF, పోర్టు, lport);
}


స్థిర u_char *
ngx_proxy_protocol_v2_read(ngx_connection_t *c, u_char *buf, u_char *చివరి)
{
    u_char                             *చివర;
    కొలత_t                              len;
    socklen_t                           socklen;
    ngx_uint_t                          సంస్కరణ, command, family, transport;
    ngx_sockaddr_t                      src_sockaddr, dst_sockaddr;
    ngx_proxy_protocol_t               *pp;
    ngx_proxy_protocol_శీర్షిక_t        *శీర్షిక;
    ngx_proxy_protocol_inet_addrs_t    *లోపల;
#ఒకవేళ (NGX_HAVE_INET6)
    ngx_proxy_protocol_inet6_addrs_t   *in6;
#అయితే_ముగింపు

    శీర్షిక = (ngx_proxy_protocol_శీర్షిక_t *) buf;

    buf += పరిమాణం(ngx_proxy_protocol_శీర్షిక_t);

    సంస్కరణ = శీర్షిక->సంస్కరణ_command >> 4;

    ఒకవేళ (సంస్కరణ != 2) {
        ngx_log_error(NGX_LOG_ERR, c->సంవర్గమానం, 0,
                      "unknown PROXY protocol version: %ui", సంస్కరణ);
        తిరిగి ఖాళీ;
    }

    len = ngx_proxy_protocol_parse_uint16(శీర్షిక->len);

    ఒకవేళ ((కొలత_t) (చివరి - buf) < len) {
        ngx_log_error(NGX_LOG_ERR, c->సంవర్గమానం, 0, "header is too large");
        తిరిగి ఖాళీ;
    }

    చివర = buf + len;

    command = శీర్షిక->సంస్కరణ_command & 0x0f;

    /* only PROXY is supported */
    ఒకవేళ (command != NGX_PROXY_PROTOCOL_CMD_PROXY) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->సంవర్గమానం, 0,
                       "PROXY protocol v2 unsupported command %ui", command);
        తిరిగి చివర;
    }

    transport = శీర్షిక->family_transport & 0x0f;

    /* only STREAM is supported */
    ఒకవేళ (transport != NGX_PROXY_PROTOCOL_TYPE_ప్రవాహం) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->సంవర్గమానం, 0,
                       "PROXY protocol v2 unsupported transport %ui",
                       transport);
        తిరిగి చివర;
    }

    pp = ngx_pcalloc(c->pool, పరిమాణం(ngx_proxy_protocol_t));
    ఒకవేళ (pp == ఖాళీ) {
        తిరిగి ఖాళీ;
    }

    family = శీర్షిక->family_transport >> 4;

    ఎంపిక (family) {

    స్థితి NGX_PROXY_PROTOCOL_AF_INET:

        ఒకవేళ ((కొలత_t) (చివర - buf) < పరిమాణం(ngx_proxy_protocol_inet_addrs_t)) {
            తిరిగి ఖాళీ;
        }

        లోపల = (ngx_proxy_protocol_inet_addrs_t *) buf;

        src_sockaddr.sockaddr_లోపల.sin_family = AF_INET;
        src_sockaddr.sockaddr_లోపల.sin_పోర్టు = 0;
        ngx_memcpy(&src_sockaddr.sockaddr_లోపల.sin_addr, లోపల->src_addr, 4);

        dst_sockaddr.sockaddr_లోపల.sin_family = AF_INET;
        dst_sockaddr.sockaddr_లోపల.sin_పోర్టు = 0;
        ngx_memcpy(&dst_sockaddr.sockaddr_లోపల.sin_addr, లోపల->dst_addr, 4);

        pp->src_పోర్టు = ngx_proxy_protocol_parse_uint16(లోపల->src_పోర్టు);
        pp->dst_పోర్టు = ngx_proxy_protocol_parse_uint16(లోపల->dst_పోర్టు);

        socklen = పరిమాణం(నిర్మాణం sockaddr_లోపల);

        buf += పరిమాణం(ngx_proxy_protocol_inet_addrs_t);

        ఆపు;

#ఒకవేళ (NGX_HAVE_INET6)

    స్థితి NGX_PROXY_PROTOCOL_AF_INET6:

        ఒకవేళ ((కొలత_t) (చివర - buf) < పరిమాణం(ngx_proxy_protocol_inet6_addrs_t)) {
            తిరిగి ఖాళీ;
        }

        in6 = (ngx_proxy_protocol_inet6_addrs_t *) buf;

        src_sockaddr.sockaddr_in6.sin6_family = AF_INET6;
        src_sockaddr.sockaddr_in6.sin6_పోర్టు = 0;
        ngx_memcpy(&src_sockaddr.sockaddr_in6.sin6_addr, in6->src_addr, 16);

        dst_sockaddr.sockaddr_in6.sin6_family = AF_INET6;
        dst_sockaddr.sockaddr_in6.sin6_పోర్టు = 0;
        ngx_memcpy(&dst_sockaddr.sockaddr_in6.sin6_addr, in6->dst_addr, 16);

        pp->src_పోర్టు = ngx_proxy_protocol_parse_uint16(in6->src_పోర్టు);
        pp->dst_పోర్టు = ngx_proxy_protocol_parse_uint16(in6->dst_పోర్టు);

        socklen = పరిమాణం(నిర్మాణం sockaddr_in6);

        buf += పరిమాణం(ngx_proxy_protocol_inet6_addrs_t);

        ఆపు;

#అయితే_ముగింపు

    అప్రమేయ:
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->సంవర్గమానం, 0,
                       "PROXY protocol v2 unsupported address family %ui",
                       family);
        తిరిగి చివర;
    }

    pp->src_addr.దత్తాంశం = ngx_pnalloc(c->pool, NGX_SOCKADDR_STRLEN);
    ఒకవేళ (pp->src_addr.దత్తాంశం == ఖాళీ) {
        తిరిగి ఖాళీ;
    }

    pp->src_addr.len = ngx_sock_ntop(&src_sockaddr.sockaddr, socklen,
                                     pp->src_addr.దత్తాంశం, NGX_SOCKADDR_STRLEN, 0);

    pp->dst_addr.దత్తాంశం = ngx_pnalloc(c->pool, NGX_SOCKADDR_STRLEN);
    ఒకవేళ (pp->dst_addr.దత్తాంశం == ఖాళీ) {
        తిరిగి ఖాళీ;
    }

    pp->dst_addr.len = ngx_sock_ntop(&dst_sockaddr.sockaddr, socklen,
                                     pp->dst_addr.దత్తాంశం, NGX_SOCKADDR_STRLEN, 0);

    ngx_log_debug4(NGX_LOG_DEBUG_CORE, c->సంవర్గమానం, 0,
                   "PROXY protocol v2 src: %V %d, dst: %V %d",
                   &pp->src_addr, pp->src_పోర్టు, &pp->dst_addr, pp->dst_పోర్టు);

    ఒకవేళ (buf < చివర) {
        pp->tlvs.దత్తాంశం = ngx_pnalloc(c->pool, చివర - buf);
        ఒకవేళ (pp->tlvs.దత్తాంశం == ఖాళీ) {
            తిరిగి ఖాళీ;
        }

        ngx_memcpy(pp->tlvs.దత్తాంశం, buf, చివర - buf);
        pp->tlvs.len = చివర - buf;
    }

    c->proxy_protocol = pp;

    తిరిగి చివర;
}


ngx_int_t
ngx_proxy_protocol_get_tlv(ngx_connection_t *c, ngx_str_t *name,
    ngx_str_t *విలువ)
{
    u_char                          *p;
    కొలత_t                           n;
    uint32_t                         verify;
    ngx_str_t                        ssl, *tlvs;
    ngx_int_t                        rc, type;
    ngx_proxy_protocol_tlv_ssl_t    *tlv_ssl;
    ngx_proxy_protocol_tlv_నమోదు_t  *te;

    ఒకవేళ (c->proxy_protocol == ఖాళీ) {
        తిరిగి NGX_DECLINED;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->సంవర్గమానం, 0,
                   "PROXY protocol v2 get tlv \"%V\"", name);

    te = ngx_proxy_protocol_tlv_entries;
    tlvs = &c->proxy_protocol->tlvs;

    p = name->దత్తాంశం;
    n = name->len;

    ఒకవేళ (n >= 4 && p[0] == 's' && p[1] == 's' && p[2] == 'l' && p[3] == '_') {

        rc = ngx_proxy_protocol_lookup_tlv(c, tlvs,
                                           NGX_PROXY_PROTOCOL_TLV_SSL, &ssl);
        ఒకవేళ (rc != NGX_OK) {
            తిరిగి rc;
        }

        ఒకవేళ (ssl.len < పరిమాణం(ngx_proxy_protocol_tlv_ssl_t)) {
            తిరిగి NGX_ERROR;
        }

        p += 4;
        n -= 4;

        ఒకవేళ (n == 6 && ngx_strncmp(p, "verify", 6) == 0) {

            tlv_ssl = (ngx_proxy_protocol_tlv_ssl_t *) ssl.దత్తాంశం;
            verify = ngx_proxy_protocol_parse_uint32(tlv_ssl->verify);

            విలువ->దత్తాంశం = ngx_pnalloc(c->pool, NGX_INT32_LEN);
            ఒకవేళ (విలువ->దత్తాంశం == ఖాళీ) {
                తిరిగి NGX_ERROR;
            }

            విలువ->len = ngx_sprintf(విలువ->దత్తాంశం, "%uD", verify)
                         - విలువ->దత్తాంశం;
            తిరిగి NGX_OK;
        }

        ssl.దత్తాంశం += పరిమాణం(ngx_proxy_protocol_tlv_ssl_t);
        ssl.len -= పరిమాణం(ngx_proxy_protocol_tlv_ssl_t);

        te = ngx_proxy_protocol_tlv_ssl_entries;
        tlvs = &ssl;
    }

    ఒకవేళ (n >= 2 && p[0] == '0' && p[1] == 'x') {

        type = ngx_hextoi(p + 2, n - 2);
        ఒకవేళ (type == NGX_ERROR) {
            ngx_log_error(NGX_LOG_ERR, c->సంవర్గమానం, 0,
                          "invalid PROXY protocol TLV \"%V\"", name);
            తిరిగి NGX_ERROR;
        }

        తిరిగి ngx_proxy_protocol_lookup_tlv(c, tlvs, type, విలువ);
    }

    కొరకు ( /* void */ ; te->type; te++) {
        ఒకవేళ (te->name.len == n && ngx_strncmp(te->name.దత్తాంశం, p, n) == 0) {
            తిరిగి ngx_proxy_protocol_lookup_tlv(c, tlvs, te->type, విలువ);
        }
    }

    ngx_log_error(NGX_LOG_ERR, c->సంవర్గమానం, 0,
                  "unknown PROXY protocol TLV \"%V\"", name);

    తిరిగి NGX_DECLINED;
}


స్థిర ngx_int_t
ngx_proxy_protocol_lookup_tlv(ngx_connection_t *c, ngx_str_t *tlvs,
    ngx_uint_t type, ngx_str_t *విలువ)
{
    u_char                    *p;
    కొలత_t                     n, len;
    ngx_proxy_protocol_tlv_t  *tlv;

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->సంవర్గమానం, 0,
                   "PROXY protocol v2 lookup tlv:%02xi", type);

    p = tlvs->దత్తాంశం;
    n = tlvs->len;

    అయితే (n) {
        ఒకవేళ (n < పరిమాణం(ngx_proxy_protocol_tlv_t)) {
            ngx_log_error(NGX_LOG_ERR, c->సంవర్గమానం, 0, "broken PROXY protocol TLV");
            తిరిగి NGX_ERROR;
        }

        tlv = (ngx_proxy_protocol_tlv_t *) p;
        len = ngx_proxy_protocol_parse_uint16(tlv->len);

        p += పరిమాణం(ngx_proxy_protocol_tlv_t);
        n -= పరిమాణం(ngx_proxy_protocol_tlv_t);

        ఒకవేళ (n < len) {
            ngx_log_error(NGX_LOG_ERR, c->సంవర్గమానం, 0, "broken PROXY protocol TLV");
            తిరిగి NGX_ERROR;
        }

        ఒకవేళ (tlv->type == type) {
            విలువ->దత్తాంశం = p;
            విలువ->len = len;
            తిరిగి NGX_OK;
        }

        p += len;
        n -= len;
    }

    తిరిగి NGX_DECLINED;
}


u_char *
ngx_proxy_protocol_v2_write(ngx_connection_t *c, u_char *buf, u_char *చివరి,
    ngx_array_t *tlvs /* reserved */ )
{
    u_char                          *p;
    uint32_t                         verify;
    ngx_uint_t                       i, client;
    ngx_array_t                      పునాది_tlvs, ssl_tlvs;
    ngx_proxy_protocol_tlv_విలువ_t  *tlv;

    client = 0;
    verify = 0;

    ngx_memzero(&పునాది_tlvs, పరిమాణం(ngx_array_t));
    ngx_memzero(&ssl_tlvs, పరిమాణం(ngx_array_t));

#ఒకవేళ (NGX_OPENSSL)

    ఒకవేళ (c->ssl != ఖాళీ) {
        ఒకవేళ (ngx_array_init(&పునాది_tlvs, c->pool, 2,
                           పరిమాణం(ngx_proxy_protocol_tlv_విలువ_t))
            != NGX_OK)
        {
            తిరిగి ఖాళీ;
        }

        ఒకవేళ (ngx_array_init(&ssl_tlvs, c->pool, 5,
                           పరిమాణం(ngx_proxy_protocol_tlv_విలువ_t))
            != NGX_OK)
        {
            తిరిగి ఖాళీ;
        }

        ఒకవేళ (ngx_proxy_protocol_v2_eval_ssl(c, &పునాది_tlvs, &ssl_tlvs, &client,
                                           &verify)
            != NGX_OK)
        {
            తిరిగి ఖాళీ;
        }
    }

#అయితే_ముగింపు

    p = ngx_proxy_protocol_v2_write_శీర్షిక(c, buf, చివరి);
    ఒకవేళ (p == ఖాళీ) {
        తిరిగి ఖాళీ;
    }

    tlv = పునాది_tlvs.elts;

    కొరకు (i = 0; i < పునాది_tlvs.nelts; i++) {
        p = ngx_proxy_protocol_v2_write_tlv(c, p, చివరి, tlv[i].type,
                                           &tlv[i].విలువ);
        ఒకవేళ (p == ఖాళీ) {
            తిరిగి ఖాళీ;
        }
    }

    ఒకవేళ (client) {
        p = ngx_proxy_protocol_v2_write_ssl(c, p, చివరి, &ssl_tlvs, client,
                                            verify);
        ఒకవేళ (p == ఖాళీ) {
            తిరిగి ఖాళీ;
        }
    }

    ngx_proxy_protocol_v2_సమితి_len(buf,
                                  p + పరిమాణం(ngx_proxy_protocol_tlv_t) + 4);

    తిరిగి ngx_proxy_protocol_v2_write_crc32c(c, buf, p, చివరి);
}


#ఒకవేళ (NGX_OPENSSL)

స్థిర ngx_int_t
ngx_proxy_protocol_v2_eval_ssl(ngx_connection_t *c, ngx_array_t *tlvs,
    ngx_array_t *ssl_tlvs, ngx_uint_t *client, uint32_t *verify)
{
    X509                            *cert;
    దీర్ఘ                             n;
    ngx_int_t                        rc;
    ngx_str_t                        విలువ;
    ngx_uint_t                       type;
    స్థిరరాశి అక్షరం                      *s;
    ngx_proxy_protocol_tlv_విలువ_t  *tlv;

    ఒకవేళ (ngx_proxy_protocol_v2_authority(c, &విలువ) == NGX_OK) {
        tlv = ngx_array_నెట్టు(tlvs);
        ఒకవేళ (tlv == ఖాళీ) {
            తిరిగి NGX_ERROR;
        }

        tlv->type = NGX_PROXY_PROTOCOL_TLV_AUTHORITY;
        tlv->విలువ = విలువ;
    }

    ఒకవేళ (ngx_proxy_protocol_v2_alpn(c, &విలువ) == NGX_OK) {
        tlv = ngx_array_నెట్టు(tlvs);
        ఒకవేళ (tlv == ఖాళీ) {
            తిరిగి NGX_ERROR;
        }

        tlv->type = NGX_PROXY_PROTOCOL_TLV_ALPN;
        tlv->విలువ = విలువ;
    }

    కొరకు (type = NGX_PROXY_PROTOCOL_TLV_SSL_సంస్కరణ;
         type <= NGX_PROXY_PROTOCOL_TLV_SSL_కీలకం_ALG;
         type++)
    {
        rc = ngx_proxy_protocol_v2_ssl_sub(c, type, &విలువ);

        ఒకవేళ (rc == NGX_ERROR) {
            తిరిగి NGX_ERROR;
        }

        ఒకవేళ (rc != NGX_OK) {
            కొనసాగు;
        }

        tlv = ngx_array_నెట్టు(ssl_tlvs);
        ఒకవేళ (tlv == ఖాళీ) {
            తిరిగి NGX_ERROR;
        }

        tlv->type = type;
        tlv->విలువ = విలువ;
    }

    *client = NGX_PROXY_PROTOCOL_V2_CLIENT_SSL;
    *verify = 1;  /* X509_V_ERR_UNSPECIFIED */

    cert = SSL_get_peer_certificate(c->ssl->connection);

    ఒకవేళ (cert != ఖాళీ) {
        X509_free(cert);

        *client |= NGX_PROXY_PROTOCOL_V2_CLIENT_CERT_SESS;

        ఒకవేళ (!SSL_session_reused(c->ssl->connection)) {
            *client |= NGX_PROXY_PROTOCOL_V2_CLIENT_CERT_CONN;
        }

        n = SSL_get_verify_ఫలితం(c->ssl->connection);

        ఒకవేళ (n == X509_V_OK) {

            ఒకవేళ (ngx_ssl_ocsp_get_స్థితి(c, &s) != NGX_OK) {
                n = X509_V_ERR_CERT_REVOKED;
            }
        }

        *verify = (uint32_t) n;
    }

    తిరిగి NGX_OK;
}


స్థిర ngx_int_t
ngx_proxy_protocol_v2_authority(ngx_connection_t *c, ngx_str_t *బహిర్గతం)
{
#నిర్వచితమైతే SSL_CTRL_సమితి_TLSEXT_HOSTNAME

    స్థిరరాశి అక్షరం  *sni;

    sni = SSL_get_servername(c->ssl->connection, TLSEXT_NAMETYPE_host_name);
    ఒకవేళ (sni == ఖాళీ) {
        తిరిగి NGX_DECLINED;
    }

    బహిర్గతం->దత్తాంశం = (u_char *) sni;
    బహిర్గతం->len = ngx_strlen(sni);
    తిరిగి NGX_OK;

#లేకపోతే
    తిరిగి NGX_DECLINED;
#అయితే_ముగింపు
}


స్థిర ngx_int_t
ngx_proxy_protocol_v2_alpn(ngx_connection_t *c, ngx_str_t *బహిర్గతం)
{
#నిర్వచితమైతే TLSEXT_TYPE_application_layer_protocol_negotiation

    గుర్తులేని పూర్ణాంకం          alpnlen;
    స్థిరరాశి గుర్తులేని అక్షరం  *alpn;

    SSL_get0_alpn_selected(c->ssl->connection, &alpn, &alpnlen);
    ఒకవేళ (alpn == ఖాళీ || alpnlen == 0) {
        తిరిగి NGX_DECLINED;
    }

    బహిర్గతం->దత్తాంశం = (u_char *) alpn;
    బహిర్గతం->len = alpnlen;
    తిరిగి NGX_OK;

#లేకపోతే
    తిరిగి NGX_DECLINED;
#అయితే_ముగింపు
}


స్థిర ngx_int_t
ngx_proxy_protocol_v2_ssl_sub(ngx_connection_t *c, ngx_uint_t type,
    ngx_str_t *బహిర్గతం)
{
    పూర్ణాంకం               i, len;
    X509             *cert;
    u_char           *alg, *s;
    EVP_PKEY         *pkey;
#ఒకవేళ (OPENSSL_సంస్కరణ_సంఖ్య >= 0x40000000L)
    స్థిరరాశి
#అయితే_ముగింపు
    X509_NAME        *subject;
#ఒకవేళ (OPENSSL_సంస్కరణ_సంఖ్య >= 0x40000000L)
    స్థిరరాశి
#అయితే_ముగింపు
    X509_NAME_నమోదు  *నమోదు_2;

    ఎంపిక (type) {

    స్థితి NGX_PROXY_PROTOCOL_TLV_SSL_సంస్కరణ:
        బహిర్గతం->దత్తాంశం = (u_char *) SSL_get_సంస్కరణ(c->ssl->connection);
        బహిర్గతం->len = ngx_strlen(బహిర్గతం->దత్తాంశం);
        తిరిగి NGX_OK;

    స్థితి NGX_PROXY_PROTOCOL_TLV_SSL_CIPHER:
        బహిర్గతం->దత్తాంశం = (u_char *) SSL_get_cipher_name(c->ssl->connection);
        బహిర్గతం->len = ngx_strlen(బహిర్గతం->దత్తాంశం);
        తిరిగి NGX_OK;

    స్థితి NGX_PROXY_PROTOCOL_TLV_SSL_CN:
        cert = SSL_get_peer_certificate(c->ssl->connection);
        ఒకవేళ (cert == ఖాళీ) {
            తిరిగి NGX_DECLINED;
        }

        subject = X509_get_subject_name(cert);
        ఒకవేళ (subject == ఖాళీ) {
            X509_free(cert);
            తిరిగి NGX_ERROR;
        }

        i = X509_NAME_get_సూచీ_by_NID(subject, NID_commonName, -1);
        ఒకవేళ (i < 0) {
            X509_free(cert);
            తిరిగి NGX_DECLINED;
        }

        నమోదు_2 = X509_NAME_get_నమోదు(subject, i);

        len = ASN1_STRING_గమ్యం_UTF8(&s, X509_NAME_నమోదు_get_దత్తాంశం(నమోదు_2));
        ఒకవేళ (len < 0) {
            ngx_ssl_error(NGX_LOG_ALERT, c->సంవర్గమానం, 0,
                          "ASN1_STRING_to_UTF8() failed");
            X509_free(cert);
            తిరిగి NGX_ERROR;
        }

        బహిర్గతం->len = len;
        బహిర్గతం->దత్తాంశం = ngx_pnalloc(c->pool, len);
        ఒకవేళ (బహిర్గతం->దత్తాంశం == ఖాళీ) {
            OPENSSL_free(s);
            X509_free(cert);
            తిరిగి NGX_ERROR;
        }

        ngx_memcpy(బహిర్గతం->దత్తాంశం, s, len);

        OPENSSL_free(s);
        X509_free(cert);
        తిరిగి NGX_OK;

    స్థితి NGX_PROXY_PROTOCOL_TLV_SSL_SIG_ALG:
        cert = SSL_get_certificate(c->ssl->connection);

        బహిర్గతం->దత్తాంశం = (u_char *) OBJ_nid2sn(X509_get_signature_nid(cert));
        బహిర్గతం->len = ngx_strlen(బహిర్గతం->దత్తాంశం);
        తిరిగి NGX_OK;

    స్థితి NGX_PROXY_PROTOCOL_TLV_SSL_కీలకం_ALG:
        cert = SSL_get_certificate(c->ssl->connection);

        pkey = X509_get_pubkey(cert);
        ఒకవేళ (pkey == ఖాళీ) {
            తిరిగి NGX_ERROR;
        }

        ఎంపిక (EVP_PKEY_పునాది_id(pkey)) {

        స్థితి EVP_PKEY_RSA:
#నిర్వచితమైతే EVP_PKEY_RSA_PSS
        స్థితి EVP_PKEY_RSA_PSS:
#అయితే_ముగింపు
            alg = (u_char *) "RSA";
            ఆపు;

        స్థితి EVP_PKEY_EC:
            alg = (u_char *) "EC";
            ఆపు;

        స్థితి EVP_PKEY_DSA:
            alg = (u_char *) "DSA";
            ఆపు;

        అప్రమేయ:
            EVP_PKEY_free(pkey);
            తిరిగి NGX_DECLINED;
        }

        బహిర్గతం->len = పరిమాణం("RSA16384") - 1;
        బహిర్గతం->దత్తాంశం = ngx_pnalloc(c->pool, బహిర్గతం->len);
        ఒకవేళ (బహిర్గతం->దత్తాంశం == ఖాళీ) {
            EVP_PKEY_free(pkey);
            తిరిగి NGX_ERROR;
        }

        బహిర్గతం->len = ngx_snprintf(బహిర్గతం->దత్తాంశం, బహిర్గతం->len, "%s%d",
                                alg, EVP_PKEY_బిట్లు(pkey))
                   - బహిర్గతం->దత్తాంశం;

        EVP_PKEY_free(pkey);

        తిరిగి NGX_OK;
    }

    తిరిగి NGX_DECLINED;
}

#అయితే_ముగింపు


స్థిర u_char *
ngx_proxy_protocol_v2_write_శీర్షిక(ngx_connection_t *c, u_char *buf,
    u_char *చివరి)
{
    u_char                            *p, src_af, dst_af,
                                       family, command, transport;
    ngx_uint_t                         src_fam, dst_fam;
    ngx_proxy_protocol_శీర్షిక_t       *శీర్షిక;
    ngx_proxy_protocol_inet_addrs_t   *లోపల;
#ఒకవేళ (NGX_HAVE_INET6)
    ngx_uint_t                         పోర్టు, lport;
    నిర్మాణం in6_addr                   *src6, *dst6;
    ngx_proxy_protocol_inet6_addrs_t  *in6;
#అయితే_ముగింపు

    ఒకవేళ (చివరి - buf < NGX_PROXY_PROTOCOL_V2_MAX_శీర్షిక) {
        ngx_log_error(NGX_LOG_ALERT, c->సంవర్గమానం, 0,
                      "too small buffer for PROXY protocol v2");
        తిరిగి ఖాళీ;
    }

    ఒకవేళ (ngx_connection_local_sockaddr(c, ఖాళీ, 0) != NGX_OK) {
        తిరిగి ఖాళీ;
    }

    /* the length field is set by the caller once all TLVs are written */

    శీర్షిక = (ngx_proxy_protocol_శీర్షిక_t *) buf;

    ngx_memcpy(శీర్షిక->signature, ngx_proxy_protocol_signature,
               పరిమాణం(ngx_proxy_protocol_signature) - 1);

    p = buf + పరిమాణం(ngx_proxy_protocol_శీర్షిక_t);

    src_fam = c->sockaddr->sa_family;
    dst_fam = c->local_sockaddr->sa_family;

    src_af = ngx_proxy_protocol_v2_family(src_fam);
    dst_af = ngx_proxy_protocol_v2_family(dst_fam);

    /* promote to the highest address family present on either side */

    ఒకవేళ (src_af == NGX_PROXY_PROTOCOL_AF_UNSPEC
        || dst_af == NGX_PROXY_PROTOCOL_AF_UNSPEC)
    {
        command = NGX_PROXY_PROTOCOL_CMD_LOCAL;
        family = NGX_PROXY_PROTOCOL_AF_UNSPEC;
        transport = NGX_PROXY_PROTOCOL_TYPE_UNSPEC;

    } లేకపోతే {
        command = NGX_PROXY_PROTOCOL_CMD_PROXY;
        family = ngx_max(src_af, dst_af);

        ఎంపిక (c->type) {
        స్థితి SOCK_ప్రవాహం:
            transport = NGX_PROXY_PROTOCOL_TYPE_ప్రవాహం;
            ఆపు;
        స్థితి SOCK_DGRAM:
            transport = NGX_PROXY_PROTOCOL_TYPE_DGRAM;
            ఆపు;
        అప్రమేయ:
            transport = NGX_PROXY_PROTOCOL_TYPE_UNSPEC;
            ఆపు;
        }
    }

    శీర్షిక->సంస్కరణ_command = 0x20 | command;
    శీర్షిక->family_transport = (family << 4) | transport;

    ఎంపిక (family) {

    స్థితి NGX_PROXY_PROTOCOL_AF_INET:

        లోపల = (ngx_proxy_protocol_inet_addrs_t *) p;

        ngx_proxy_protocol_v2_write_ipv4(c->sockaddr,
                                         లోపల->src_addr, లోపల->src_పోర్టు);
        ngx_proxy_protocol_v2_write_ipv4(c->local_sockaddr,
                                         లోపల->dst_addr, లోపల->dst_పోర్టు);

        p += పరిమాణం(ngx_proxy_protocol_inet_addrs_t);

        ఆపు;

#ఒకవేళ (NGX_HAVE_INET6)

    స్థితి NGX_PROXY_PROTOCOL_AF_INET6:

        ఒకవేళ (src_fam == AF_INET6 && dst_fam == AF_INET6) {
            src6 = &((నిర్మాణం sockaddr_in6 *) c->sockaddr)->sin6_addr;
            dst6 = &((నిర్మాణం sockaddr_in6 *) c->local_sockaddr)->sin6_addr;

            ఒకవేళ (IN6_IS_ADDR_V4MAPPED(src6) && IN6_IS_ADDR_V4MAPPED(dst6)) {

                /* both v4-mapped: demote to AF_INET */

                శీర్షిక->family_transport = (NGX_PROXY_PROTOCOL_AF_INET << 4)
                                           | transport;

                లోపల = (ngx_proxy_protocol_inet_addrs_t *) p;

                ngx_memcpy(లోపల->src_addr, src6->s6_addr + 12, 4);
                ngx_memcpy(లోపల->dst_addr, dst6->s6_addr + 12, 4);

                పోర్టు  = ngx_inet_get_పోర్టు(c->sockaddr);
                lport = ngx_inet_get_పోర్టు(c->local_sockaddr);

                లోపల->src_పోర్టు[0] = (u_char) (పోర్టు >> 8);
                లోపల->src_పోర్టు[1] = (u_char)  పోర్టు;
                లోపల->dst_పోర్టు[0] = (u_char) (lport >> 8);
                లోపల->dst_పోర్టు[1] = (u_char)  lport;

                p += పరిమాణం(ngx_proxy_protocol_inet_addrs_t);
                ఆపు;
            }
        }

        in6 = (ngx_proxy_protocol_inet6_addrs_t *) p;

        ngx_proxy_protocol_v2_write_ipv6(c->sockaddr,
                                         in6->src_addr, in6->src_పోర్టు);
        ngx_proxy_protocol_v2_write_ipv6(c->local_sockaddr,
                                         in6->dst_addr, in6->dst_పోర్టు);

        p += పరిమాణం(ngx_proxy_protocol_inet6_addrs_t);

        ఆపు;

#అయితే_ముగింపు

    అప్రమేయ:  /* NGX_PROXY_PROTOCOL_AF_UNSPEC */
        ఆపు;
    }

    తిరిగి p;
}


స్థిర u_char
ngx_proxy_protocol_v2_family(ngx_uint_t family)
{
    ఎంపిక (family) {

    స్థితి AF_INET:
        తిరిగి NGX_PROXY_PROTOCOL_AF_INET;

#ఒకవేళ (NGX_HAVE_INET6)
    స్థితి AF_INET6:
        తిరిగి NGX_PROXY_PROTOCOL_AF_INET6;
#అయితే_ముగింపు

    అప్రమేయ:
        తిరిగి NGX_PROXY_PROTOCOL_AF_UNSPEC;
    }
}


స్థిర శూన్య
ngx_proxy_protocol_v2_write_ipv4(నిర్మాణం sockaddr *sa, u_char *addr,
    u_char *పోర్టు)
{
    నిర్మాణం sockaddr_లోపల  *జ్యా;

    జ్యా = (నిర్మాణం sockaddr_లోపల *) sa;
    ngx_memcpy(addr, &జ్యా->sin_addr, 4);
    ngx_memcpy(పోర్టు, &జ్యా->sin_పోర్టు, 2);
}


#ఒకవేళ (NGX_HAVE_INET6)

స్థిర శూన్య
ngx_proxy_protocol_v2_write_ipv6(నిర్మాణం sockaddr *sa, u_char *addr,
    u_char *పోర్టు)
{
    నిర్మాణం sockaddr_లోపల   *జ్యా;
    నిర్మాణం sockaddr_in6  *sin6;

    /* IPv4-mapped prefix: 80 zero bits + 16 one bits */
    స్థిర స్థిరరాశి u_char  mapped[]   = { 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0xff, 0xff };

    ఒకవేళ (sa->sa_family == AF_INET6) {
        sin6 = (నిర్మాణం sockaddr_in6 *) sa;
        ngx_memcpy(addr, &sin6->sin6_addr, 16);
        ngx_memcpy(పోర్టు, &sin6->sin6_పోర్టు, 2);

    } లేకపోతే {
        /* promote AF_INET to ::ffff:a.b.c.d */
        జ్యా = (నిర్మాణం sockaddr_లోపల *) sa;
        ngx_memcpy(addr, mapped, పరిమాణం(mapped));
        ngx_memcpy(addr + పరిమాణం(mapped), &జ్యా->sin_addr, 4);
        ngx_memcpy(పోర్టు, &జ్యా->sin_పోర్టు, 2);
    }
}

#అయితే_ముగింపు


స్థిర u_char *
ngx_proxy_protocol_v2_write_tlv(ngx_connection_t *c, u_char *p, u_char *చివరి,
    ngx_uint_t type, ngx_str_t *విలువ)
{
    ngx_proxy_protocol_tlv_t  *tlv;

    ఒకవేళ ((కొలత_t) (చివరి - p) < పరిమాణం(ngx_proxy_protocol_tlv_t) + విలువ->len) {
        ngx_log_error(NGX_LOG_ALERT, c->సంవర్గమానం, 0,
                      "too small buffer for PROXY protocol v2 TLV");
        తిరిగి ఖాళీ;
    }

    tlv = (ngx_proxy_protocol_tlv_t *) p;
    tlv->type = (u_char) type;
    tlv->len[0] = (u_char) (విలువ->len >> 8);
    tlv->len[1] = (u_char)  విలువ->len;

    p += పరిమాణం(ngx_proxy_protocol_tlv_t);

    తిరిగి ngx_cpymem(p, విలువ->దత్తాంశం, విలువ->len);
}


స్థిర u_char *
ngx_proxy_protocol_v2_write_ssl(ngx_connection_t *c, u_char *p, u_char *చివరి,
    ngx_array_t *ssl_tlvs, ngx_uint_t client, uint32_t verify)
{
    u_char                          *ప్రారంభం;
    uint16_t                         len;
    ngx_uint_t                       i;
    ngx_proxy_protocol_tlv_t        *tlv;
    ngx_proxy_protocol_tlv_ssl_t    *tlv_ssl;
    ngx_proxy_protocol_tlv_విలువ_t  *sub;

    ఒకవేళ ((కొలత_t) (చివరి - p) < పరిమాణం(ngx_proxy_protocol_tlv_t)
                              + పరిమాణం(ngx_proxy_protocol_tlv_ssl_t))
    {
        ngx_log_error(NGX_LOG_ALERT, c->సంవర్గమానం, 0,
                      "too small buffer for PROXY protocol v2 SSL TLV");
        తిరిగి ఖాళీ;
    }

    ప్రారంభం = p;
    p += పరిమాణం(ngx_proxy_protocol_tlv_t);

    tlv_ssl = (ngx_proxy_protocol_tlv_ssl_t *) p;
    tlv_ssl->client = (u_char) client;
    tlv_ssl->verify[0] = (u_char) (verify >> 24);
    tlv_ssl->verify[1] = (u_char) (verify >> 16);
    tlv_ssl->verify[2] = (u_char) (verify >> 8);
    tlv_ssl->verify[3] = (u_char)  verify;

    p += పరిమాణం(ngx_proxy_protocol_tlv_ssl_t);

    sub = ssl_tlvs->elts;

    కొరకు (i = 0; i < ssl_tlvs->nelts; i++) {
        p = ngx_proxy_protocol_v2_write_tlv(c, p, చివరి, sub[i].type,
                                            &sub[i].విలువ);
        ఒకవేళ (p == ఖాళీ) {
            తిరిగి ఖాళీ;
        }
    }

    len = (uint16_t) (p - ప్రారంభం - పరిమాణం(ngx_proxy_protocol_tlv_t));

    tlv = (ngx_proxy_protocol_tlv_t *) ప్రారంభం;
    tlv->type = NGX_PROXY_PROTOCOL_TLV_SSL;
    tlv->len[0] = (u_char) (len >> 8);
    tlv->len[1] = (u_char)  len;

    తిరిగి p;
}


స్థిర శూన్య
ngx_proxy_protocol_v2_సమితి_len(u_char *buf, u_char *p)
{
    uint16_t                      len;
    ngx_proxy_protocol_శీర్షిక_t  *శీర్షిక;

    len = (uint16_t) (p - buf - పరిమాణం(ngx_proxy_protocol_శీర్షిక_t));

    శీర్షిక = (ngx_proxy_protocol_శీర్షిక_t *) buf;
    శీర్షిక->len[0] = (u_char) (len >> 8);
    శీర్షిక->len[1] = (u_char) len;
}


స్థిర u_char *
ngx_proxy_protocol_v2_write_crc32c(ngx_connection_t *c, u_char *buf,
    u_char *p, u_char *చివరి)
{
    u_char                    *విలువ;
    uint32_t                   చక్రీయ_తనిఖీ;
    ngx_proxy_protocol_tlv_t  *tlv;

    ఒకవేళ ((కొలత_t) (చివరి - p) < పరిమాణం(ngx_proxy_protocol_tlv_t) + 4) {
        ngx_log_error(NGX_LOG_ALERT, c->సంవర్గమానం, 0,
                      "too small buffer for PROXY protocol v2 CRC32c TLV");
        తిరిగి ఖాళీ;
    }

    /* append the CRC32c TLV with a zeroed value field */

    tlv = (ngx_proxy_protocol_tlv_t *) p;
    tlv->type = NGX_PROXY_PROTOCOL_TLV_చక్రీయ_తనిఖీ_32_C;
    tlv->len[0] = 0;
    tlv->len[1] = 4;

    విలువ = p + పరిమాణం(ngx_proxy_protocol_tlv_t);

    ngx_memzero(విలువ, 4);

    p = విలువ + 4;

    /* the checksum covers the entire header with the zeroed value */

    చక్రీయ_తనిఖీ = ngx_crc32c_long(buf, p - buf);

    విలువ[0] = (u_char) (చక్రీయ_తనిఖీ >> 24);
    విలువ[1] = (u_char) (చక్రీయ_తనిఖీ >> 16);
    విలువ[2] = (u_char) (చక్రీయ_తనిఖీ >> 8);
    విలువ[3] = (u_char) చక్రీయ_తనిఖీ;

    తిరిగి p;
}
