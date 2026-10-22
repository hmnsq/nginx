
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_CRC32_H_INCLUDED_
#define _NGX_CRC32_H_INCLUDED_


#অন্তর্ভাব <ngx_config.h>
#অন্তর্ভাব <ngx_core.h>


বাহ্যিক uint32_t  *ngx_crc32_table_short;
বাহ্যিক uint32_t   ngx_crc32_table256[];
বাহ্যিক uint32_t   ngx_crc32c_table256[];


স্থির ngx_inline uint32_t
ngx_crc32_short(u_char *p, আকার_t len)
{
    u_char    c;
    uint32_t  crc;

    crc = 0xffffffff;

    যখন (len--) {
        c = *p++;
        crc = ngx_crc32_table_short[(crc ^ (c & 0xf)) & 0xf] ^ (crc >> 4);
        crc = ngx_crc32_table_short[(crc ^ (c >> 4)) & 0xf] ^ (crc >> 4);
    }

    ফেরত crc ^ 0xffffffff;
}


স্থির ngx_inline uint32_t
ngx_crc32_long(u_char *p, আকার_t len)
{
    uint32_t  crc;

    crc = 0xffffffff;

    যখন (len--) {
        crc = ngx_crc32_table256[(crc ^ *p++) & 0xff] ^ (crc >> 8);
    }

    ফেরত crc ^ 0xffffffff;
}


#define ngx_crc32_init(crc)                                                   \
    crc = 0xffffffff


স্থির ngx_inline শূন্য
ngx_crc32_update(uint32_t *crc, u_char *p, আকার_t len)
{
    uint32_t  c;

    c = *crc;

    যখন (len--) {
        c = ngx_crc32_table256[(c ^ *p++) & 0xff] ^ (c >> 8);
    }

    *crc = c;
}


#define ngx_crc32_final(crc)                                                  \
    crc ^= 0xffffffff


স্থির ngx_inline uint32_t
ngx_crc32c_long(u_char *p, আকার_t len)
{
    uint32_t  crc;

    crc = 0xffffffff;

    যখন (len--) {
        crc = ngx_crc32c_table256[(crc ^ *p++) & 0xff] ^ (crc >> 8);
    }

    ফেরত crc ^ 0xffffffff;
}


#define ngx_crc32c_init(crc)                                                  \
    crc = 0xffffffff


স্থির ngx_inline শূন্য
ngx_crc32c_update(uint32_t *crc, u_char *p, আকার_t len)
{
    uint32_t  c;

    c = *crc;

    যখন (len--) {
        c = ngx_crc32c_table256[(c ^ *p++) & 0xff] ^ (c >> 8);
    }

    *crc = c;
}


#define ngx_crc32c_final(crc)                                                 \
    crc ^= 0xffffffff


ngx_int_t ngx_crc32_table_init(শূন্য);


#endif /* _NGX_CRC32_H_INCLUDED_ */
