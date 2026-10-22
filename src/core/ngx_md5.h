
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_MD5_H_INCLUDED_
#define _NGX_MD5_H_INCLUDED_


#شامل <ngx_config.h>
#شامل <ngx_core.h>


تعریفنوع ساختار {
    uint64_t  bytes;
    uint32_t  a, b, c, d;
    u_char    buffer[64];
} ngx_md5_t;


تهی ngx_md5_init(ngx_md5_t *ctx);
تهی ngx_md5_update(ngx_md5_t *ctx, ثابت تهی *data, size_t size);
تهی ngx_md5_final(u_char result[16], ngx_md5_t *ctx);


#endif /* _NGX_MD5_H_INCLUDED_ */
