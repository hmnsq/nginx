
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_MD5_H_INCLUDED_
#define _NGX_MD5_H_INCLUDED_


#অন্তর্ভাব <ngx_config.h>
#অন্তর্ভাব <ngx_core.h>


প্রকারলেখ কাঠামো {
    uint64_t  bytes;
    uint32_t  a, b, c, d;
    u_char    বাফার[64];
} ngx_md5_t;


শূন্য ngx_md5_init(ngx_md5_t *ctx);
শূন্য ngx_md5_update(ngx_md5_t *ctx, ধ্রুবক শূন্য *তথ্য, আকার_t আকার_2);
শূন্য ngx_md5_final(u_char ফলাফল[16], ngx_md5_t *ctx);


#endif /* _NGX_MD5_H_INCLUDED_ */
