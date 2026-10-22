
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#यदि_अपरिभाषितम् NGX_चक्रीयपरीक्षा_32_H_INCLUDED
#परिभाषय NGX_चक्रीयपरीक्षा_32_H_INCLUDED


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


बाह्य uint32_t  *ngx_चक्रीयपरीक्षा_32_सारणी_short;
बाह्य uint32_t   ngx_चक्रीयपरीक्षा_32_table256[];
बाह्य uint32_t   ngx_crc32c_table256[];


स्थिर ngx_inline uint32_t
ngx_चक्रीयपरीक्षा_32_short(u_char *p, आकार_t len)
{
    u_char    c;
    uint32_t  crc;

    crc = 0xffffffff;

    यावत् (len--) {
        c = *p++;
        crc = ngx_चक्रीयपरीक्षा_32_सारणी_short[(crc ^ (c & 0xf)) & 0xf] ^ (crc >> 4);
        crc = ngx_चक्रीयपरीक्षा_32_सारणी_short[(crc ^ (c >> 4)) & 0xf] ^ (crc >> 4);
    }

    प्रत्यागम crc ^ 0xffffffff;
}


स्थिर ngx_inline uint32_t
ngx_चक्रीयपरीक्षा_32_long(u_char *p, आकार_t len)
{
    uint32_t  crc;

    crc = 0xffffffff;

    यावत् (len--) {
        crc = ngx_चक्रीयपरीक्षा_32_table256[(crc ^ *p++) & 0xff] ^ (crc >> 8);
    }

    प्रत्यागम crc ^ 0xffffffff;
}


#परिभाषय ngx_चक्रीयपरीक्षा_32_init(crc)                                                   \
    crc = 0xffffffff


स्थिर ngx_inline शून्य
ngx_चक्रीयपरीक्षा_32_update(uint32_t *crc, u_char *p, आकार_t len)
{
    uint32_t  c;

    c = *crc;

    यावत् (len--) {
        c = ngx_चक्रीयपरीक्षा_32_table256[(c ^ *p++) & 0xff] ^ (c >> 8);
    }

    *crc = c;
}


#परिभाषय ngx_चक्रीयपरीक्षा_32_final(crc)                                                  \
    crc ^= 0xffffffff


स्थिर ngx_inline uint32_t
ngx_crc32c_long(u_char *p, आकार_t len)
{
    uint32_t  crc;

    crc = 0xffffffff;

    यावत् (len--) {
        crc = ngx_crc32c_table256[(crc ^ *p++) & 0xff] ^ (crc >> 8);
    }

    प्रत्यागम crc ^ 0xffffffff;
}


#परिभाषय ngx_crc32c_init(crc)                                                  \
    crc = 0xffffffff


स्थिर ngx_inline शून्य
ngx_crc32c_update(uint32_t *crc, u_char *p, आकार_t len)
{
    uint32_t  c;

    c = *crc;

    यावत् (len--) {
        c = ngx_crc32c_table256[(c ^ *p++) & 0xff] ^ (c >> 8);
    }

    *crc = c;
}


#परिभाषय ngx_crc32c_final(crc)                                                 \
    crc ^= 0xffffffff


ngx_int_t ngx_चक्रीयपरीक्षा_32_सारणी_init(शून्य);


#यदि_अन्तः /* _NGX_CRC32_H_INCLUDED_ */
