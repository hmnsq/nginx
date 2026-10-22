
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_CRC32_H_INCLUDED_
#define _NGX_CRC32_H_INCLUDED_


#شامل <ngx_config.h>
#شامل <ngx_core.h>


بیرونی uint32_t  *ngx_crc32_table_short;
بیرونی uint32_t   ngx_crc32_table256[];
بیرونی uint32_t   ngx_crc32c_table256[];


ایستا ngx_inline uint32_t
ngx_crc32_short(u_char *p, size_t len)
{
    u_char    c;
    uint32_t  crc;

    crc = 0xffffffff;

    وقتی (len--) {
        c = *p++;
        crc = ngx_crc32_table_short[(crc ^ (c & 0xf)) & 0xf] ^ (crc >> 4);
        crc = ngx_crc32_table_short[(crc ^ (c >> 4)) & 0xf] ^ (crc >> 4);
    }

    بازگشت crc ^ 0xffffffff;
}


ایستا ngx_inline uint32_t
ngx_crc32_long(u_char *p, size_t len)
{
    uint32_t  crc;

    crc = 0xffffffff;

    وقتی (len--) {
        crc = ngx_crc32_table256[(crc ^ *p++) & 0xff] ^ (crc >> 8);
    }

    بازگشت crc ^ 0xffffffff;
}


#define ngx_crc32_init(crc)                                                   \
    crc = 0xffffffff


ایستا ngx_inline تهی
ngx_crc32_update(uint32_t *crc, u_char *p, size_t len)
{
    uint32_t  c;

    c = *crc;

    وقتی (len--) {
        c = ngx_crc32_table256[(c ^ *p++) & 0xff] ^ (c >> 8);
    }

    *crc = c;
}


#define ngx_crc32_final(crc)                                                  \
    crc ^= 0xffffffff


ایستا ngx_inline uint32_t
ngx_crc32c_long(u_char *p, size_t len)
{
    uint32_t  crc;

    crc = 0xffffffff;

    وقتی (len--) {
        crc = ngx_crc32c_table256[(crc ^ *p++) & 0xff] ^ (crc >> 8);
    }

    بازگشت crc ^ 0xffffffff;
}


#define ngx_crc32c_init(crc)                                                  \
    crc = 0xffffffff


ایستا ngx_inline تهی
ngx_crc32c_update(uint32_t *crc, u_char *p, size_t len)
{
    uint32_t  c;

    c = *crc;

    وقتی (len--) {
        c = ngx_crc32c_table256[(c ^ *p++) & 0xff] ^ (c >> 8);
    }

    *crc = c;
}


#define ngx_crc32c_final(crc)                                                 \
    crc ^= 0xffffffff


ngx_int_t ngx_crc32_table_init(تهی);


#endif /* _NGX_CRC32_H_INCLUDED_ */
