
/*
 * Copyright (C) Roman Arutyunyan
 * Copyright (C) Nginx, Inc.
 */


#که_نه_تعریف_شوی _NGX_PROXY_PROTOCOL_H_INCLUDED_
#تعریف_کړه _NGX_PROXY_PROTOCOL_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


#تعریف_کړه NGX_PROXY_PROTOCOL_V1_MAX_سرلیک  107
#تعریف_کړه NGX_PROXY_PROTOCOL_MAX_سرلیک     4096


بنية ngx_proxy_protocol_s {
    ngx_str_t           src_addr;
    ngx_str_t           dst_addr;
    دننه_پورټ_t           src_پورټ;
    دننه_پورټ_t           dst_پورټ;
    ngx_str_t           tlvs;
};


u_char *ngx_proxy_protocol_read(ngx_connection_t *c, u_char *buf,
    u_char *وروستی);
u_char *ngx_proxy_protocol_write(ngx_connection_t *c, u_char *buf,
    u_char *وروستی);
u_char *ngx_proxy_protocol_v2_write(ngx_connection_t *c, u_char *buf,
    u_char *وروستی, ngx_array_t *tlvs);
ngx_int_t ngx_proxy_protocol_get_tlv(ngx_connection_t *c, ngx_str_t *name,
    ngx_str_t *ارزښت);


#د_که_پای /* _NGX_PROXY_PROTOCOL_H_INCLUDED_ */
