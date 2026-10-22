
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#اگر_تعریف_نشده _NGX_SHA1_H_INCLUDED_
#تعریف_کن _NGX_SHA1_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


تعريفنوع بنية {
    uint64_t  بایتها;
    uint32_t  a, b, c, d, e, f;
    u_char    میانگیر[64];
} ngx_sha1_t;


فراغ ngx_sha1_init(ngx_sha1_t *ctx);
فراغ ngx_sha1_update(ngx_sha1_t *ctx, ثابت فراغ *داده, اندازه_t اندازه);
فراغ ngx_sha1_final(u_char نتیجه[20], ngx_sha1_t *ctx);


#پایان_اگر /* _NGX_SHA1_H_INCLUDED_ */
