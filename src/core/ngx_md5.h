
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_MD5_H_INCLUDED_
#define _NGX_MD5_H_INCLUDED_


#அந்தர்பாவ <ngx_config.h>
#அந்தர்பாவ <ngx_core.h>


வகைவரை கட்டமைப்பு {
    uint64_t  bytes;
    uint32_t  a, b, c, d;
    u_char    buffer[64];
} ngx_md5_t;


வெறுமை ngx_md5_init(ngx_md5_t *ctx);
வெறுமை ngx_md5_update(ngx_md5_t *ctx, மாறிலி வெறுமை *data, size_t size);
வெறுமை ngx_md5_final(u_char result[16], ngx_md5_t *ctx);


#endif /* _NGX_MD5_H_INCLUDED_ */
