
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_CRC32_H_INCLUDED_
#define _NGX_CRC32_H_INCLUDED_


#અન્તર્ભાવ <ngx_config.h>
#અન્તર્ભાવ <ngx_core.h>


બાહ્ય uint32_t  *ngx_crc32_table_short;
બાહ્ય uint32_t   ngx_crc32_table256[];
બાહ્ય uint32_t   ngx_crc32c_table256[];


સ્થિર ngx_inline uint32_t
ngx_crc32_short(u_char *p, size_t len)
{
    u_char    c;
    uint32_t  crc;

    crc = 0xffffffff;

    જ્યારે (len--) {
        c = *p++;
        crc = ngx_crc32_table_short[(crc ^ (c & 0xf)) & 0xf] ^ (crc >> 4);
        crc = ngx_crc32_table_short[(crc ^ (c >> 4)) & 0xf] ^ (crc >> 4);
    }

    પાછા crc ^ 0xffffffff;
}


સ્થિર ngx_inline uint32_t
ngx_crc32_long(u_char *p, size_t len)
{
    uint32_t  crc;

    crc = 0xffffffff;

    જ્યારે (len--) {
        crc = ngx_crc32_table256[(crc ^ *p++) & 0xff] ^ (crc >> 8);
    }

    પાછા crc ^ 0xffffffff;
}


#define ngx_crc32_init(crc)                                                   \
    crc = 0xffffffff


સ્થિર ngx_inline શૂન્ય
ngx_crc32_update(uint32_t *crc, u_char *p, size_t len)
{
    uint32_t  c;

    c = *crc;

    જ્યારે (len--) {
        c = ngx_crc32_table256[(c ^ *p++) & 0xff] ^ (c >> 8);
    }

    *crc = c;
}


#define ngx_crc32_final(crc)                                                  \
    crc ^= 0xffffffff


સ્થિર ngx_inline uint32_t
ngx_crc32c_long(u_char *p, size_t len)
{
    uint32_t  crc;

    crc = 0xffffffff;

    જ્યારે (len--) {
        crc = ngx_crc32c_table256[(crc ^ *p++) & 0xff] ^ (crc >> 8);
    }

    પાછા crc ^ 0xffffffff;
}


#define ngx_crc32c_init(crc)                                                  \
    crc = 0xffffffff


સ્થિર ngx_inline શૂન્ય
ngx_crc32c_update(uint32_t *crc, u_char *p, size_t len)
{
    uint32_t  c;

    c = *crc;

    જ્યારે (len--) {
        c = ngx_crc32c_table256[(c ^ *p++) & 0xff] ^ (c >> 8);
    }

    *crc = c;
}


#define ngx_crc32c_final(crc)                                                 \
    crc ^= 0xffffffff


ngx_int_t ngx_crc32_table_init(શૂન્ય);


#endif /* _NGX_CRC32_H_INCLUDED_ */
