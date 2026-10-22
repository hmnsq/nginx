
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_SHA1_H_INCLUDED_
#define _NGX_SHA1_H_INCLUDED_


#അന്തര്ഭാവ <ngx_config.h>
#അന്തര്ഭാവ <ngx_core.h>


തരനിർവചനം ഘടന {
    uint64_t  bytes;
    uint32_t  a, b, c, d, e, f;
    u_char    buffer[64];
} ngx_sha1_t;


ശൂന്യം ngx_sha1_init(ngx_sha1_t *ctx);
ശൂന്യം ngx_sha1_update(ngx_sha1_t *ctx, സ്ഥിരം ശൂന്യം *data, size_t size);
ശൂന്യം ngx_sha1_final(u_char result[20], ngx_sha1_t *ctx);


#endif /* _NGX_SHA1_H_INCLUDED_ */
