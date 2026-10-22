
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अगर_अघोषित _NGX_CRC32_H_INCLUDED_
#घोषणा _NGX_CRC32_H_INCLUDED_


#समावेश <ngx_config.h>
#समावेश <ngx_core.h>


बाह्य uint32_t  *ngx_crc32_table_short;
बाह्य uint32_t   ngx_crc32_table256[];
बाह्य uint32_t   ngx_crc32c_table256[];


अटल ngx_inline uint32_t
ngx_crc32_short(u_char *p, माप_प्रकार len)
{
    u_char    c;
    uint32_t  crc;

    crc = 0xffffffff;

    जबतक (len--) {
        c = *p++;
        crc = ngx_crc32_table_short[(crc ^ (c & 0xf)) & 0xf] ^ (crc >> 4);
        crc = ngx_crc32_table_short[(crc ^ (c >> 4)) & 0xf] ^ (crc >> 4);
    }

    वापस crc ^ 0xffffffff;
}


अटल ngx_inline uint32_t
ngx_crc32_long(u_char *p, माप_प्रकार len)
{
    uint32_t  crc;

    crc = 0xffffffff;

    जबतक (len--) {
        crc = ngx_crc32_table256[(crc ^ *p++) & 0xff] ^ (crc >> 8);
    }

    वापस crc ^ 0xffffffff;
}


#घोषणा ngx_crc32_init(crc)                                                   \
    crc = 0xffffffff


अटल ngx_inline व्योम
ngx_crc32_update(uint32_t *crc, u_char *p, माप_प्रकार len)
{
    uint32_t  c;

    c = *crc;

    जबतक (len--) {
        c = ngx_crc32_table256[(c ^ *p++) & 0xff] ^ (c >> 8);
    }

    *crc = c;
}


#घोषणा ngx_crc32_final(crc)                                                  \
    crc ^= 0xffffffff


अटल ngx_inline uint32_t
ngx_crc32c_long(u_char *p, माप_प्रकार len)
{
    uint32_t  crc;

    crc = 0xffffffff;

    जबतक (len--) {
        crc = ngx_crc32c_table256[(crc ^ *p++) & 0xff] ^ (crc >> 8);
    }

    वापस crc ^ 0xffffffff;
}


#घोषणा ngx_crc32c_init(crc)                                                  \
    crc = 0xffffffff


अटल ngx_inline व्योम
ngx_crc32c_update(uint32_t *crc, u_char *p, माप_प्रकार len)
{
    uint32_t  c;

    c = *crc;

    जबतक (len--) {
        c = ngx_crc32c_table256[(c ^ *p++) & 0xff] ^ (c >> 8);
    }

    *crc = c;
}


#घोषणा ngx_crc32c_final(crc)                                                 \
    crc ^= 0xffffffff


ngx_int_t ngx_crc32_table_init(व्योम);


#पूर्ण_अगर /* _NGX_CRC32_H_INCLUDED_ */
