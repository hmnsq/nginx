
/*
 * Copyright (C) Roman Arutyunyan
 * Copyright (C) Nginx, Inc.
 */


#جے_بنا_تعریف _NGX_PROXY_PROTOCOL_H_INCLUDED_
#تعریف_کرو _NGX_PROXY_PROTOCOL_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


#تعریف_کرو NGX_PROXY_PROTOCOL_V1_MAX_سرخی  107
#تعریف_کرو NGX_PROXY_PROTOCOL_MAX_سرخی     4096


بنية ngx_proxy_protocol_s {
    ngx_str_t           src_addr;
    ngx_str_t           dst_addr;
    وچ_پورٹ_t           src_پورٹ;
    وچ_پورٹ_t           dst_پورٹ;
    ngx_str_t           tlvs;
};


u_char *ngx_proxy_protocol_read(ngx_connection_t *c, u_char *buf,
    u_char *آخری);
u_char *ngx_proxy_protocol_write(ngx_connection_t *c, u_char *buf,
    u_char *آخری);
u_char *ngx_proxy_protocol_v2_write(ngx_connection_t *c, u_char *buf,
    u_char *آخری, ngx_array_t *tlvs);
ngx_int_t ngx_proxy_protocol_get_tlv(ngx_connection_t *c, ngx_str_t *name,
    ngx_str_t *قدر);


#جے_انت /* _NGX_PROXY_PROTOCOL_H_INCLUDED_ */
