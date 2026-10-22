
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अगर_अघोषित _NGX_SHA1_H_INCLUDED_
#घोषणा _NGX_SHA1_H_INCLUDED_


#समावेश <ngx_config.h>
#समावेश <ngx_core.h>


प्रकार काष्ठा {
    uint64_t  bytes;
    uint32_t  a, b, c, d, e, f;
    u_char    बफर[64];
} ngx_sha1_t;


व्योम ngx_sha1_init(ngx_sha1_t *ctx);
व्योम ngx_sha1_update(ngx_sha1_t *ctx, स्थिर व्योम *तथ्य, माप_प्रकार आकार);
व्योम ngx_sha1_final(u_char परिणाम[20], ngx_sha1_t *ctx);


#पूर्ण_अगर /* _NGX_SHA1_H_INCLUDED_ */
