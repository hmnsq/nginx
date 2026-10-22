
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#यदि_अपरिभाषितम् _NGX_MD5_H_INCLUDED_
#परिभाषय _NGX_MD5_H_INCLUDED_


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


प्रकारनिर्देश संरचना {
    uint64_t  अष्टकानि;
    uint32_t  a, b, c, d;
    u_char    बफर[64];
} ngx_md5_t;


शून्य ngx_md5_init(ngx_md5_t *ctx);
शून्य ngx_md5_update(ngx_md5_t *ctx, ध्रुव शून्य *दत्त, आकार_t आकार);
शून्य ngx_md5_final(u_char परिणाम[16], ngx_md5_t *ctx);


#यदि_अन्तः /* _NGX_MD5_H_INCLUDED_ */
