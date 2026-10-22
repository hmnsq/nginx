
/*
 * Copyright (C) Roman Arutyunyan
 * Copyright (C) Nginx, Inc.
 */


#अगर_अघोषित _NGX_PROXY_PROTOCOL_H_INCLUDED_
#घोषणा _NGX_PROXY_PROTOCOL_H_INCLUDED_


#समावेश <ngx_config.h>
#समावेश <ngx_core.h>


#घोषणा NGX_PROXY_PROTOCOL_V1_MAX_HEADER  107
#घोषणा NGX_PROXY_PROTOCOL_MAX_HEADER     4096


काष्ठा ngx_proxy_protocol_s {
    ngx_str_t           src_addr;
    ngx_str_t           dst_addr;
    in_port_t           src_port;
    in_port_t           dst_port;
    ngx_str_t           tlvs;
};


u_char *ngx_proxy_protocol_read(ngx_connection_t *c, u_char *buf,
    u_char *last);
u_char *ngx_proxy_protocol_write(ngx_connection_t *c, u_char *buf,
    u_char *last);
u_char *ngx_proxy_protocol_v2_write(ngx_connection_t *c, u_char *buf,
    u_char *last, ngx_array_t *tlvs);
ngx_int_t ngx_proxy_protocol_get_tlv(ngx_connection_t *c, ngx_str_t *name,
    ngx_str_t *मूल्य);


#पूर्ण_अगर /* _NGX_PROXY_PROTOCOL_H_INCLUDED_ */
