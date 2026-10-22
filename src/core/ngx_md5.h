
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#جيڪڏهن_وصف_نه_ٿيل _NGX_MD5_H_INCLUDED_
#وصف_ڪريو _NGX_MD5_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


تعريفنوع بنية {
    uint64_t  بائيٽون;
    uint32_t  a, b, c, d;
    u_char    بفر[64];
} ngx_md5_t;


فراغ ngx_md5_init(ngx_md5_t *ctx);
فراغ ngx_md5_update(ngx_md5_t *ctx, ثابت فراغ *معلومات, سائيز_t سائيز);
فراغ ngx_md5_final(u_char نتيجو[16], ngx_md5_t *ctx);


#جيڪڏهن_پورو /* _NGX_MD5_H_INCLUDED_ */
