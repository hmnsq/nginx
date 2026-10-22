
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#که_نه_تعریف_شوی NGX_دوراني_ازموینه_32_H_INCLUDED
#تعریف_کړه NGX_دوراني_ازموینه_32_H_INCLUDED


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


خارجي uint32_t  *ngx_دوراني_ازموینه_32_جدول_short;
خارجي uint32_t   ngx_دوراني_ازموینه_32_table256[];
خارجي uint32_t   ngx_crc32c_table256[];


ساكن ngx_inline uint32_t
ngx_دوراني_ازموینه_32_short(u_char *p, اندازه_t len)
{
    u_char    c;
    uint32_t  دوراني_ازموینه;

    دوراني_ازموینه = 0xffffffff;

    طالما (len--) {
        c = *p++;
        دوراني_ازموینه = ngx_دوراني_ازموینه_32_جدول_short[(دوراني_ازموینه ^ (c & 0xf)) & 0xf] ^ (دوراني_ازموینه >> 4);
        دوراني_ازموینه = ngx_دوراني_ازموینه_32_جدول_short[(دوراني_ازموینه ^ (c >> 4)) & 0xf] ^ (دوراني_ازموینه >> 4);
    }

    أرجع دوراني_ازموینه ^ 0xffffffff;
}


ساكن ngx_inline uint32_t
ngx_دوراني_ازموینه_32_long(u_char *p, اندازه_t len)
{
    uint32_t  دوراني_ازموینه;

    دوراني_ازموینه = 0xffffffff;

    طالما (len--) {
        دوراني_ازموینه = ngx_دوراني_ازموینه_32_table256[(دوراني_ازموینه ^ *p++) & 0xff] ^ (دوراني_ازموینه >> 8);
    }

    أرجع دوراني_ازموینه ^ 0xffffffff;
}


#تعریف_کړه ngx_دوراني_ازموینه_32_init(دوراني_ازموینه)                                                   \
    دوراني_ازموینه = 0xffffffff


ساكن ngx_inline فراغ
ngx_دوراني_ازموینه_32_update(uint32_t *دوراني_ازموینه, u_char *p, اندازه_t len)
{
    uint32_t  c;

    c = *دوراني_ازموینه;

    طالما (len--) {
        c = ngx_دوراني_ازموینه_32_table256[(c ^ *p++) & 0xff] ^ (c >> 8);
    }

    *دوراني_ازموینه = c;
}


#تعریف_کړه ngx_دوراني_ازموینه_32_final(دوراني_ازموینه)                                                  \
    دوراني_ازموینه ^= 0xffffffff


ساكن ngx_inline uint32_t
ngx_crc32c_long(u_char *p, اندازه_t len)
{
    uint32_t  دوراني_ازموینه;

    دوراني_ازموینه = 0xffffffff;

    طالما (len--) {
        دوراني_ازموینه = ngx_crc32c_table256[(دوراني_ازموینه ^ *p++) & 0xff] ^ (دوراني_ازموینه >> 8);
    }

    أرجع دوراني_ازموینه ^ 0xffffffff;
}


#تعریف_کړه ngx_crc32c_init(دوراني_ازموینه)                                                  \
    دوراني_ازموینه = 0xffffffff


ساكن ngx_inline فراغ
ngx_crc32c_update(uint32_t *دوراني_ازموینه, u_char *p, اندازه_t len)
{
    uint32_t  c;

    c = *دوراني_ازموینه;

    طالما (len--) {
        c = ngx_crc32c_table256[(c ^ *p++) & 0xff] ^ (c >> 8);
    }

    *دوراني_ازموینه = c;
}


#تعریف_کړه ngx_crc32c_final(دوراني_ازموینه)                                                 \
    دوراني_ازموینه ^= 0xffffffff


ngx_int_t ngx_دوراني_ازموینه_32_جدول_init(فراغ);


#د_که_پای /* _NGX_CRC32_H_INCLUDED_ */
