
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#اگر_تعریف_نشده _NGX_MD5_H_INCLUDED_
#تعریف_کن _NGX_MD5_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


تعريفنوع بنية {
    uint64_t  بایتها;
    uint32_t  a, b, c, d;
    u_char    میانگیر[64];
} ngx_md5_t;


فراغ ngx_md5_init(ngx_md5_t *ctx);
فراغ ngx_md5_update(ngx_md5_t *ctx, ثابت فراغ *داده, اندازه_t اندازه);
فراغ ngx_md5_final(u_char نتیجه[16], ngx_md5_t *ctx);


#پایان_اگر /* _NGX_MD5_H_INCLUDED_ */
