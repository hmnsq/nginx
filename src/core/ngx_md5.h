
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#که_نه_تعریف_شوی _NGX_MD5_H_INCLUDED_
#تعریف_کړه _NGX_MD5_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


تعريفنوع بنية {
    uint64_t  بایټونه;
    uint32_t  a, b, c, d;
    u_char    بفر[64];
} ngx_md5_t;


فراغ ngx_md5_init(ngx_md5_t *ctx);
فراغ ngx_md5_update(ngx_md5_t *ctx, ثابت فراغ *معلومات, اندازه_t اندازه);
فراغ ngx_md5_final(u_char پایله[16], ngx_md5_t *ctx);


#د_که_پای /* _NGX_MD5_H_INCLUDED_ */
