
/*
 * Copyright (C) Roman Arutyunyan
 * Copyright (C) Nginx, Inc.
 */


#جيڪڏهن_وصف_نه_ٿيل _NGX_PROXY_PROTOCOL_H_INCLUDED_
#وصف_ڪريو _NGX_PROXY_PROTOCOL_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


#وصف_ڪريو NGX_PROXY_PROTOCOL_V1_MAX_سرو  107
#وصف_ڪريو NGX_PROXY_PROTOCOL_MAX_سرو     4096


بنية ngx_proxy_protocol_s {
    ngx_str_t           src_addr;
    ngx_str_t           dst_addr;
    اندريون_پورٽ_t           src_پورٽ;
    اندريون_پورٽ_t           dst_پورٽ;
    ngx_str_t           tlvs;
};


u_char *ngx_proxy_protocol_read(ngx_connection_t *c, u_char *buf,
    u_char *آخري);
u_char *ngx_proxy_protocol_write(ngx_connection_t *c, u_char *buf,
    u_char *آخري);
u_char *ngx_proxy_protocol_v2_write(ngx_connection_t *c, u_char *buf,
    u_char *آخري, ngx_array_t *tlvs);
ngx_int_t ngx_proxy_protocol_get_tlv(ngx_connection_t *c, ngx_str_t *name,
    ngx_str_t *قدر);


#جيڪڏهن_پورو /* _NGX_PROXY_PROTOCOL_H_INCLUDED_ */
