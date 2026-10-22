
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_SHA1_H_INCLUDED_
#define _NGX_SHA1_H_INCLUDED_


#অন্তর্ভাব <ngx_config.h>
#অন্তর্ভাব <ngx_core.h>


প্রকারলেখ কাঠামো {
    uint64_t  bytes;
    uint32_t  a, b, c, d, e, f;
    u_char    বাফার[64];
} ngx_sha1_t;


শূন্য ngx_sha1_init(ngx_sha1_t *ctx);
শূন্য ngx_sha1_update(ngx_sha1_t *ctx, ধ্রুবক শূন্য *তথ্য, আকার_t আকার_2);
শূন্য ngx_sha1_final(u_char ফলাফল[20], ngx_sha1_t *ctx);


#endif /* _NGX_SHA1_H_INCLUDED_ */
