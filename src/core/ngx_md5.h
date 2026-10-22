
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#جے_بنا_تعریف _NGX_MD5_H_INCLUDED_
#تعریف_کرو _NGX_MD5_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


تعريفنوع بنية {
    uint64_t  بائٹاں;
    uint32_t  a, b, c, d;
    u_char    بفر[64];
} ngx_md5_t;


فراغ ngx_md5_init(ngx_md5_t *ctx);
فراغ ngx_md5_update(ngx_md5_t *ctx, ثابت فراغ *انکڑے, ناپ_t ناپ);
فراغ ngx_md5_final(u_char نتیجہ[16], ngx_md5_t *ctx);


#جے_انت /* _NGX_MD5_H_INCLUDED_ */
