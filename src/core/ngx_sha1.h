
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_SHA1_H_INCLUDED_
#define _NGX_SHA1_H_INCLUDED_


#ଅନ୍ତର୍ଭାଵ <ngx_config.h>
#ଅନ୍ତର୍ଭାଵ <ngx_core.h>


ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    uint64_t  bytes;
    uint32_t  a, b, c, d, e, f;
    u_char    buffer[64];
} ngx_sha1_t;


ଶୂନ୍ୟ ngx_sha1_init(ngx_sha1_t *ctx);
ଶୂନ୍ୟ ngx_sha1_update(ngx_sha1_t *ctx, ଅଚଳ ଶୂନ୍ୟ *data, size_t size);
ଶୂନ୍ୟ ngx_sha1_final(u_char result[20], ngx_sha1_t *ctx);


#endif /* _NGX_SHA1_H_INCLUDED_ */
