
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_SHA1_H_INCLUDED_
#define _NGX_SHA1_H_INCLUDED_


#شامل <ngx_config.h>
#شامل <ngx_core.h>


تعریفنوع ساختار {
    uint64_t  bytes;
    uint32_t  a, b, c, d, e, f;
    u_char    buffer[64];
} ngx_sha1_t;


تهی ngx_sha1_init(ngx_sha1_t *ctx);
تهی ngx_sha1_update(ngx_sha1_t *ctx, ثابت تهی *data, size_t size);
تهی ngx_sha1_final(u_char result[20], ngx_sha1_t *ctx);


#endif /* _NGX_SHA1_H_INCLUDED_ */
