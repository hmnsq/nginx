
/*
 * Copyright (C) Roman Arutyunyan
 * Copyright (C) Nginx, Inc.
 */


#اگر_تعریف_نشده _NGX_PROXY_PROTOCOL_H_INCLUDED_
#تعریف_کن _NGX_PROXY_PROTOCOL_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


#تعریف_کن NGX_PROXY_PROTOCOL_V1_MAX_سرآیند  107
#تعریف_کن NGX_PROXY_PROTOCOL_MAX_سرآیند     4096


بنية ngx_proxy_protocol_s {
    ngx_str_t           src_addr;
    ngx_str_t           dst_addr;
    درون_پورت_t           src_پورت;
    درون_پورت_t           dst_پورت;
    ngx_str_t           tlvs;
};


u_char *ngx_proxy_protocol_read(ngx_connection_t *c, u_char *buf,
    u_char *آخرین);
u_char *ngx_proxy_protocol_write(ngx_connection_t *c, u_char *buf,
    u_char *آخرین);
u_char *ngx_proxy_protocol_v2_write(ngx_connection_t *c, u_char *buf,
    u_char *آخرین, ngx_array_t *tlvs);
ngx_int_t ngx_proxy_protocol_get_tlv(ngx_connection_t *c, ngx_str_t *name,
    ngx_str_t *مقدار);


#پایان_اگر /* _NGX_PROXY_PROTOCOL_H_INCLUDED_ */
