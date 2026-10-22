
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_CRC32_H_INCLUDED_
#define _NGX_CRC32_H_INCLUDED_


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


बाह्य uint32_t  *ngx_crc32_table_short;
बाह्य uint32_t   ngx_crc32_table256[];
बाह्य uint32_t   ngx_crc32c_table256[];


स्थिर ngx_inline uint32_t
ngx_crc32_short(u_char *p, माप_प्रकार len)
{
    u_char    c;
    uint32_t  crc;

    crc = 0xffffffff;

    जबसम्म (len--) {
        c = *p++;
        crc = ngx_crc32_table_short[(crc ^ (c & 0xf)) & 0xf] ^ (crc >> 4);
        crc = ngx_crc32_table_short[(crc ^ (c >> 4)) & 0xf] ^ (crc >> 4);
    }

    फर्काऊ crc ^ 0xffffffff;
}


स्थिर ngx_inline uint32_t
ngx_crc32_long(u_char *p, माप_प्रकार len)
{
    uint32_t  crc;

    crc = 0xffffffff;

    जबसम्म (len--) {
        crc = ngx_crc32_table256[(crc ^ *p++) & 0xff] ^ (crc >> 8);
    }

    फर्काऊ crc ^ 0xffffffff;
}


#define ngx_crc32_init(crc)                                                   \
    crc = 0xffffffff


स्थिर ngx_inline शून्य
ngx_crc32_update(uint32_t *crc, u_char *p, माप_प्रकार len)
{
    uint32_t  c;

    c = *crc;

    जबसम्म (len--) {
        c = ngx_crc32_table256[(c ^ *p++) & 0xff] ^ (c >> 8);
    }

    *crc = c;
}


#define ngx_crc32_final(crc)                                                  \
    crc ^= 0xffffffff


स्थिर ngx_inline uint32_t
ngx_crc32c_long(u_char *p, माप_प्रकार len)
{
    uint32_t  crc;

    crc = 0xffffffff;

    जबसम्म (len--) {
        crc = ngx_crc32c_table256[(crc ^ *p++) & 0xff] ^ (crc >> 8);
    }

    फर्काऊ crc ^ 0xffffffff;
}


#define ngx_crc32c_init(crc)                                                  \
    crc = 0xffffffff


स्थिर ngx_inline शून्य
ngx_crc32c_update(uint32_t *crc, u_char *p, माप_प्रकार len)
{
    uint32_t  c;

    c = *crc;

    जबसम्म (len--) {
        c = ngx_crc32c_table256[(c ^ *p++) & 0xff] ^ (c >> 8);
    }

    *crc = c;
}


#define ngx_crc32c_final(crc)                                                 \
    crc ^= 0xffffffff


ngx_int_t ngx_crc32_table_init(शून्य);


#endif /* _NGX_CRC32_H_INCLUDED_ */
