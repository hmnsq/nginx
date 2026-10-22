
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#جيڪڏهن_وصف_نه_ٿيل _NGX_SHA1_H_INCLUDED_
#وصف_ڪريو _NGX_SHA1_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


تعريفنوع بنية {
    uint64_t  بائيٽون;
    uint32_t  a, b, c, d, e, f;
    u_char    بفر[64];
} ngx_sha1_t;


فراغ ngx_sha1_init(ngx_sha1_t *ctx);
فراغ ngx_sha1_update(ngx_sha1_t *ctx, ثابت فراغ *معلومات, سائيز_t سائيز);
فراغ ngx_sha1_final(u_char نتيجو[20], ngx_sha1_t *ctx);


#جيڪڏهن_پورو /* _NGX_SHA1_H_INCLUDED_ */
