
/*
 * Copyright (C) Roman Arutyunyan
 * Copyright (C) Nginx, Inc.
 */


#यदि_अपरिभाषितम् _NGX_PROXY_PROTOCOL_H_INCLUDED_
#परिभाषय _NGX_PROXY_PROTOCOL_H_INCLUDED_


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


#परिभाषय NGX_PROXY_PROTOCOL_V1_MAX_शीर्षकम्  107
#परिभाषय NGX_PROXY_PROTOCOL_MAX_शीर्षकम्     4096


संरचना ngx_proxy_protocol_s {
    ngx_str_t           src_addr;
    ngx_str_t           dst_addr;
    अन्तर्_पत्तनम्_t           src_पत्तनम्;
    अन्तर्_पत्तनम्_t           dst_पत्तनम्;
    ngx_str_t           tlvs;
};


u_char *ngx_proxy_protocol_read(ngx_connection_t *c, u_char *buf,
    u_char *चरमः);
u_char *ngx_proxy_protocol_write(ngx_connection_t *c, u_char *buf,
    u_char *चरमः);
u_char *ngx_proxy_protocol_v2_write(ngx_connection_t *c, u_char *buf,
    u_char *चरमः, ngx_array_t *tlvs);
ngx_int_t ngx_proxy_protocol_get_tlv(ngx_connection_t *c, ngx_str_t *name,
    ngx_str_t *मानम्);


#यदि_अन्तः /* _NGX_PROXY_PROTOCOL_H_INCLUDED_ */
