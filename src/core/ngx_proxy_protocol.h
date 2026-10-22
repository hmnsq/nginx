
/*
 * Copyright (C) Roman Arutyunyan
 * Copyright (C) Nginx, Inc.
 */


#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ _NGX_PROXY_PROTOCOL_H_INCLUDED_
#ವ್ಯಾಖ್ಯಾನಿಸು _NGX_PROXY_PROTOCOL_H_INCLUDED_


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


#ವ್ಯಾಖ್ಯಾನಿಸು NGX_PROXY_PROTOCOL_V1_MAX_ಶೀರ್ಷಿಕೆ  107
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_PROXY_PROTOCOL_MAX_ಶೀರ್ಷಿಕೆ     4096


ರಚನೆ ngx_proxy_protocol_s {
    ngx_str_t           src_addr;
    ngx_str_t           dst_addr;
    ಒಳ_ಪೋರ್ಟ್_t           src_ಪೋರ್ಟ್;
    ಒಳ_ಪೋರ್ಟ್_t           dst_ಪೋರ್ಟ್;
    ngx_str_t           tlvs;
};


u_char *ngx_proxy_protocol_read(ngx_connection_t *c, u_char *buf,
    u_char *ಕೊನೆಯ);
u_char *ngx_proxy_protocol_write(ngx_connection_t *c, u_char *buf,
    u_char *ಕೊನೆಯ);
u_char *ngx_proxy_protocol_v2_write(ngx_connection_t *c, u_char *buf,
    u_char *ಕೊನೆಯ, ngx_array_t *tlvs);
ngx_int_t ngx_proxy_protocol_get_tlv(ngx_connection_t *c, ngx_str_t *name,
    ngx_str_t *ಮೌಲ್ಯ);


#ಆದರೆ_ಅಂತ್ಯ /* _NGX_PROXY_PROTOCOL_H_INCLUDED_ */
