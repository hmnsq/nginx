
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_MD5_H_INCLUDED_
#define _NGX_MD5_H_INCLUDED_


#ଅନ୍ତର୍ଭାଵ <ngx_config.h>
#ଅନ୍ତର୍ଭାଵ <ngx_core.h>


ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    uint64_t  bytes;
    uint32_t  a, b, c, d;
    u_char    buffer[64];
} ngx_md5_t;


ଶୂନ୍ୟ ngx_md5_init(ngx_md5_t *ctx);
ଶୂନ୍ୟ ngx_md5_update(ngx_md5_t *ctx, ଅଚଳ ଶୂନ୍ୟ *data, size_t size);
ଶୂନ୍ୟ ngx_md5_final(u_char result[16], ngx_md5_t *ctx);


#endif /* _NGX_MD5_H_INCLUDED_ */
