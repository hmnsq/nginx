
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अगर_अघोषित _NGX_MD5_H_INCLUDED_
#घोषणा _NGX_MD5_H_INCLUDED_


#समावेश <ngx_config.h>
#समावेश <ngx_core.h>


प्रकार काष्ठा {
    uint64_t  bytes;
    uint32_t  a, b, c, d;
    u_char    बफर[64];
} ngx_md5_t;


व्योम ngx_md5_init(ngx_md5_t *ctx);
व्योम ngx_md5_update(ngx_md5_t *ctx, स्थिर व्योम *तथ्य, माप_प्रकार आकार);
व्योम ngx_md5_final(u_char परिणाम[16], ngx_md5_t *ctx);


#पूर्ण_अगर /* _NGX_MD5_H_INCLUDED_ */
