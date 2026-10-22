
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#اگر_تعریف_نشده NGX_بررسی_چرخهای_32_H_INCLUDED
#تعریف_کن NGX_بررسی_چرخهای_32_H_INCLUDED


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


خارجي uint32_t  *ngx_بررسی_چرخهای_32_جدول_short;
خارجي uint32_t   ngx_بررسی_چرخهای_32_table256[];
خارجي uint32_t   ngx_crc32c_table256[];


ساكن ngx_inline uint32_t
ngx_بررسی_چرخهای_32_short(u_char *p, اندازه_t len)
{
    u_char    c;
    uint32_t  بررسی_چرخهای;

    بررسی_چرخهای = 0xffffffff;

    طالما (len--) {
        c = *p++;
        بررسی_چرخهای = ngx_بررسی_چرخهای_32_جدول_short[(بررسی_چرخهای ^ (c & 0xf)) & 0xf] ^ (بررسی_چرخهای >> 4);
        بررسی_چرخهای = ngx_بررسی_چرخهای_32_جدول_short[(بررسی_چرخهای ^ (c >> 4)) & 0xf] ^ (بررسی_چرخهای >> 4);
    }

    أرجع بررسی_چرخهای ^ 0xffffffff;
}


ساكن ngx_inline uint32_t
ngx_بررسی_چرخهای_32_long(u_char *p, اندازه_t len)
{
    uint32_t  بررسی_چرخهای;

    بررسی_چرخهای = 0xffffffff;

    طالما (len--) {
        بررسی_چرخهای = ngx_بررسی_چرخهای_32_table256[(بررسی_چرخهای ^ *p++) & 0xff] ^ (بررسی_چرخهای >> 8);
    }

    أرجع بررسی_چرخهای ^ 0xffffffff;
}


#تعریف_کن ngx_بررسی_چرخهای_32_init(بررسی_چرخهای)                                                   \
    بررسی_چرخهای = 0xffffffff


ساكن ngx_inline فراغ
ngx_بررسی_چرخهای_32_update(uint32_t *بررسی_چرخهای, u_char *p, اندازه_t len)
{
    uint32_t  c;

    c = *بررسی_چرخهای;

    طالما (len--) {
        c = ngx_بررسی_چرخهای_32_table256[(c ^ *p++) & 0xff] ^ (c >> 8);
    }

    *بررسی_چرخهای = c;
}


#تعریف_کن ngx_بررسی_چرخهای_32_final(بررسی_چرخهای)                                                  \
    بررسی_چرخهای ^= 0xffffffff


ساكن ngx_inline uint32_t
ngx_crc32c_long(u_char *p, اندازه_t len)
{
    uint32_t  بررسی_چرخهای;

    بررسی_چرخهای = 0xffffffff;

    طالما (len--) {
        بررسی_چرخهای = ngx_crc32c_table256[(بررسی_چرخهای ^ *p++) & 0xff] ^ (بررسی_چرخهای >> 8);
    }

    أرجع بررسی_چرخهای ^ 0xffffffff;
}


#تعریف_کن ngx_crc32c_init(بررسی_چرخهای)                                                  \
    بررسی_چرخهای = 0xffffffff


ساكن ngx_inline فراغ
ngx_crc32c_update(uint32_t *بررسی_چرخهای, u_char *p, اندازه_t len)
{
    uint32_t  c;

    c = *بررسی_چرخهای;

    طالما (len--) {
        c = ngx_crc32c_table256[(c ^ *p++) & 0xff] ^ (c >> 8);
    }

    *بررسی_چرخهای = c;
}


#تعریف_کن ngx_crc32c_final(بررسی_چرخهای)                                                 \
    بررسی_چرخهای ^= 0xffffffff


ngx_int_t ngx_بررسی_چرخهای_32_جدول_init(فراغ);


#پایان_اگر /* _NGX_CRC32_H_INCLUDED_ */
