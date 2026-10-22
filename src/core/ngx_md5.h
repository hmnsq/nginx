
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#اگر_غیر_تعریف_شدہ _NGX_MD5_H_INCLUDED_
#تعریف_کرو _NGX_MD5_H_INCLUDED_


#شامل <ngx_config.h>
#شامل <ngx_core.h>


قسمتعریف ساخت {
    uint64_t  بائٹیں;
    uint32_t  a, b, c, d;
    u_char    بفر[64];
} ngx_md5_t;


خالی ngx_md5_init(ngx_md5_t *ctx);
خالی ngx_md5_update(ngx_md5_t *ctx, ثابت خالی *ڈیٹا, سائز_t سائز);
خالی ngx_md5_final(u_char نتیجہ[16], ngx_md5_t *ctx);


#اگر_ختم /* _NGX_MD5_H_INCLUDED_ */
