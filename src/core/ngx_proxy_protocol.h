
/*
 * Copyright (C) Roman Arutyunyan
 * Copyright (C) Nginx, Inc.
 */


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ _NGX_PROXY_PROTOCOL_H_INCLUDED_
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _NGX_PROXY_PROTOCOL_H_INCLUDED_


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_PROXY_PROTOCOL_V1_MAX_ਸਿਰਲੇਖ  107
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_PROXY_PROTOCOL_MAX_ਸਿਰਲੇਖ     4096


ਬਣਤਰ ngx_proxy_protocol_s {
    ngx_str_t           src_addr;
    ngx_str_t           dst_addr;
    ਵਿੱਚ_ਪੋਰਟ_t           src_ਪੋਰਟ;
    ਵਿੱਚ_ਪੋਰਟ_t           dst_ਪੋਰਟ;
    ngx_str_t           tlvs;
};


u_char *ngx_proxy_protocol_read(ngx_connection_t *c, u_char *buf,
    u_char *ਆਖਰੀ);
u_char *ngx_proxy_protocol_write(ngx_connection_t *c, u_char *buf,
    u_char *ਆਖਰੀ);
u_char *ngx_proxy_protocol_v2_write(ngx_connection_t *c, u_char *buf,
    u_char *ਆਖਰੀ, ngx_array_t *tlvs);
ngx_int_t ngx_proxy_protocol_get_tlv(ngx_connection_t *c, ngx_str_t *name,
    ngx_str_t *ਮੁੱਲ);


#ਜੇ_ਅੰਤ /* _NGX_PROXY_PROTOCOL_H_INCLUDED_ */
