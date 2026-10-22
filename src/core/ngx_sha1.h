
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#که_نه_تعریف_شوی _NGX_SHA1_H_INCLUDED_
#تعریف_کړه _NGX_SHA1_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


تعريفنوع بنية {
    uint64_t  بایټونه;
    uint32_t  a, b, c, d, e, f;
    u_char    بفر[64];
} ngx_sha1_t;


فراغ ngx_sha1_init(ngx_sha1_t *ctx);
فراغ ngx_sha1_update(ngx_sha1_t *ctx, ثابت فراغ *معلومات, اندازه_t اندازه);
فراغ ngx_sha1_final(u_char پایله[20], ngx_sha1_t *ctx);


#د_که_پای /* _NGX_SHA1_H_INCLUDED_ */
