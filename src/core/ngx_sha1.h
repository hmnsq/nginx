
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_SHA1_H_INCLUDED_
#define _NGX_SHA1_H_INCLUDED_


#অন্তর্ভাব <ngx_config.h>
#অন্তর্ভাব <ngx_core.h>


প্রকারনির্দেশ সংরচনা {
    uint64_t  bytes;
    uint32_t  a, b, c, d, e, f;
    u_char    buffer[64];
} ngx_sha1_t;


শূন্য ngx_sha1_init(ngx_sha1_t *ctx);
শূন্য ngx_sha1_update(ngx_sha1_t *ctx, ধ্রুব শূন্য *data, size_t size);
শূন্য ngx_sha1_final(u_char result[20], ngx_sha1_t *ctx);


#endif /* _NGX_SHA1_H_INCLUDED_ */
