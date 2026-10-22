
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#جے_بنا_تعریف _NGX_SHA1_H_INCLUDED_
#تعریف_کرو _NGX_SHA1_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


تعريفنوع بنية {
    uint64_t  بائٹاں;
    uint32_t  a, b, c, d, e, f;
    u_char    بفر[64];
} ngx_sha1_t;


فراغ ngx_sha1_init(ngx_sha1_t *ctx);
فراغ ngx_sha1_update(ngx_sha1_t *ctx, ثابت فراغ *انکڑے, ناپ_t ناپ);
فراغ ngx_sha1_final(u_char نتیجہ[20], ngx_sha1_t *ctx);


#جے_انت /* _NGX_SHA1_H_INCLUDED_ */
