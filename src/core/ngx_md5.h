
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_MD5_H_INCLUDED_
#define _NGX_MD5_H_INCLUDED_


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    uint64_t  bytes;
    uint32_t  a, b, c, d;
    u_char    buffer[64];
} ngx_md5_t;


ಶೂನ್ಯ ngx_md5_init(ngx_md5_t *ctx);
ಶೂನ್ಯ ngx_md5_update(ngx_md5_t *ctx, ಧ್ರುವ ಶೂನ್ಯ *data, size_t size);
ಶೂನ್ಯ ngx_md5_final(u_char result[16], ngx_md5_t *ctx);


#endif /* _NGX_MD5_H_INCLUDED_ */
