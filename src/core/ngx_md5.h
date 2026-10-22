
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_MD5_H_INCLUDED_
#define _NGX_MD5_H_INCLUDED_


#അന്തര്ഭാവ <ngx_config.h>
#അന്തര്ഭാവ <ngx_core.h>


തരനിർവചനം ഘടന {
    uint64_t  bytes;
    uint32_t  a, b, c, d;
    u_char    buffer[64];
} ngx_md5_t;


ശൂന്യം ngx_md5_init(ngx_md5_t *ctx);
ശൂന്യം ngx_md5_update(ngx_md5_t *ctx, സ്ഥിരം ശൂന്യം *data, size_t size);
ശൂന്യം ngx_md5_final(u_char result[16], ngx_md5_t *ctx);


#endif /* _NGX_MD5_H_INCLUDED_ */
