
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#جيڪڏهن_وصف_نه_ٿيل NGX_چڪري_جاچ_32_H_INCLUDED
#وصف_ڪريو NGX_چڪري_جاچ_32_H_INCLUDED


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


خارجي uint32_t  *ngx_چڪري_جاچ_32_جدول_short;
خارجي uint32_t   ngx_چڪري_جاچ_32_table256[];
خارجي uint32_t   ngx_crc32c_table256[];


ساكن ngx_inline uint32_t
ngx_چڪري_جاچ_32_short(u_char *p, سائيز_t len)
{
    u_char    c;
    uint32_t  چڪري_جاچ;

    چڪري_جاچ = 0xffffffff;

    طالما (len--) {
        c = *p++;
        چڪري_جاچ = ngx_چڪري_جاچ_32_جدول_short[(چڪري_جاچ ^ (c & 0xf)) & 0xf] ^ (چڪري_جاچ >> 4);
        چڪري_جاچ = ngx_چڪري_جاچ_32_جدول_short[(چڪري_جاچ ^ (c >> 4)) & 0xf] ^ (چڪري_جاچ >> 4);
    }

    أرجع چڪري_جاچ ^ 0xffffffff;
}


ساكن ngx_inline uint32_t
ngx_چڪري_جاچ_32_long(u_char *p, سائيز_t len)
{
    uint32_t  چڪري_جاچ;

    چڪري_جاچ = 0xffffffff;

    طالما (len--) {
        چڪري_جاچ = ngx_چڪري_جاچ_32_table256[(چڪري_جاچ ^ *p++) & 0xff] ^ (چڪري_جاچ >> 8);
    }

    أرجع چڪري_جاچ ^ 0xffffffff;
}


#وصف_ڪريو ngx_چڪري_جاچ_32_init(چڪري_جاچ)                                                   \
    چڪري_جاچ = 0xffffffff


ساكن ngx_inline فراغ
ngx_چڪري_جاچ_32_update(uint32_t *چڪري_جاچ, u_char *p, سائيز_t len)
{
    uint32_t  c;

    c = *چڪري_جاچ;

    طالما (len--) {
        c = ngx_چڪري_جاچ_32_table256[(c ^ *p++) & 0xff] ^ (c >> 8);
    }

    *چڪري_جاچ = c;
}


#وصف_ڪريو ngx_چڪري_جاچ_32_final(چڪري_جاچ)                                                  \
    چڪري_جاچ ^= 0xffffffff


ساكن ngx_inline uint32_t
ngx_crc32c_long(u_char *p, سائيز_t len)
{
    uint32_t  چڪري_جاچ;

    چڪري_جاچ = 0xffffffff;

    طالما (len--) {
        چڪري_جاچ = ngx_crc32c_table256[(چڪري_جاچ ^ *p++) & 0xff] ^ (چڪري_جاچ >> 8);
    }

    أرجع چڪري_جاچ ^ 0xffffffff;
}


#وصف_ڪريو ngx_crc32c_init(چڪري_جاچ)                                                  \
    چڪري_جاچ = 0xffffffff


ساكن ngx_inline فراغ
ngx_crc32c_update(uint32_t *چڪري_جاچ, u_char *p, سائيز_t len)
{
    uint32_t  c;

    c = *چڪري_جاچ;

    طالما (len--) {
        c = ngx_crc32c_table256[(c ^ *p++) & 0xff] ^ (c >> 8);
    }

    *چڪري_جاچ = c;
}


#وصف_ڪريو ngx_crc32c_final(چڪري_جاچ)                                                 \
    چڪري_جاچ ^= 0xffffffff


ngx_int_t ngx_چڪري_جاچ_32_جدول_init(فراغ);


#جيڪڏهن_پورو /* _NGX_CRC32_H_INCLUDED_ */
