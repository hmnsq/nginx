
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#اگر_غیر_تعریف_شدہ _NGX_SHA1_H_INCLUDED_
#تعریف_کرو _NGX_SHA1_H_INCLUDED_


#شامل <ngx_config.h>
#شامل <ngx_core.h>


قسمتعریف ساخت {
    uint64_t  بائٹیں;
    uint32_t  a, b, c, d, e, f;
    u_char    بفر[64];
} ngx_sha1_t;


خالی ngx_sha1_init(ngx_sha1_t *ctx);
خالی ngx_sha1_update(ngx_sha1_t *ctx, ثابت خالی *ڈیٹا, سائز_t سائز);
خالی ngx_sha1_final(u_char نتیجہ[20], ngx_sha1_t *ctx);


#اگر_ختم /* _NGX_SHA1_H_INCLUDED_ */
