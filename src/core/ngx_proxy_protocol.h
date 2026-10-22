
/*
 * Copyright (C) Roman Arutyunyan
 * Copyright (C) Nginx, Inc.
 */


#నిర్వచితం_కాకపోతే _NGX_PROXY_PROTOCOL_H_INCLUDED_
#నిర్వచించు _NGX_PROXY_PROTOCOL_H_INCLUDED_


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>


#నిర్వచించు NGX_PROXY_PROTOCOL_V1_MAX_శీర్షిక  107
#నిర్వచించు NGX_PROXY_PROTOCOL_MAX_శీర్షిక     4096


నిర్మాణం ngx_proxy_protocol_s {
    ngx_str_t           src_addr;
    ngx_str_t           dst_addr;
    లోపల_పోర్టు_t           src_పోర్టు;
    లోపల_పోర్టు_t           dst_పోర్టు;
    ngx_str_t           tlvs;
};


u_char *ngx_proxy_protocol_read(ngx_connection_t *c, u_char *buf,
    u_char *చివరి);
u_char *ngx_proxy_protocol_write(ngx_connection_t *c, u_char *buf,
    u_char *చివరి);
u_char *ngx_proxy_protocol_v2_write(ngx_connection_t *c, u_char *buf,
    u_char *చివరి, ngx_array_t *tlvs);
ngx_int_t ngx_proxy_protocol_get_tlv(ngx_connection_t *c, ngx_str_t *name,
    ngx_str_t *విలువ);


#అయితే_ముగింపు /* _NGX_PROXY_PROTOCOL_H_INCLUDED_ */
