
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#اگر_غیر_تعریف_شدہ NGX_چکری_جانچ_32_H_INCLUDED
#تعریف_کرو NGX_چکری_جانچ_32_H_INCLUDED


#شامل <ngx_config.h>
#شامل <ngx_core.h>


خارجی uint32_t  *ngx_چکری_جانچ_32_جدول_short;
خارجی uint32_t   ngx_چکری_جانچ_32_table256[];
خارجی uint32_t   ngx_crc32c_table256[];


ساکن ngx_inline uint32_t
ngx_چکری_جانچ_32_short(u_char *p, سائز_t len)
{
    u_char    c;
    uint32_t  چکری_جانچ;

    چکری_جانچ = 0xffffffff;

    جب (len--) {
        c = *p++;
        چکری_جانچ = ngx_چکری_جانچ_32_جدول_short[(چکری_جانچ ^ (c & 0xf)) & 0xf] ^ (چکری_جانچ >> 4);
        چکری_جانچ = ngx_چکری_جانچ_32_جدول_short[(چکری_جانچ ^ (c >> 4)) & 0xf] ^ (چکری_جانچ >> 4);
    }

    واپس چکری_جانچ ^ 0xffffffff;
}


ساکن ngx_inline uint32_t
ngx_چکری_جانچ_32_long(u_char *p, سائز_t len)
{
    uint32_t  چکری_جانچ;

    چکری_جانچ = 0xffffffff;

    جب (len--) {
        چکری_جانچ = ngx_چکری_جانچ_32_table256[(چکری_جانچ ^ *p++) & 0xff] ^ (چکری_جانچ >> 8);
    }

    واپس چکری_جانچ ^ 0xffffffff;
}


#تعریف_کرو ngx_چکری_جانچ_32_init(چکری_جانچ)                                                   \
    چکری_جانچ = 0xffffffff


ساکن ngx_inline خالی
ngx_چکری_جانچ_32_update(uint32_t *چکری_جانچ, u_char *p, سائز_t len)
{
    uint32_t  c;

    c = *چکری_جانچ;

    جب (len--) {
        c = ngx_چکری_جانچ_32_table256[(c ^ *p++) & 0xff] ^ (c >> 8);
    }

    *چکری_جانچ = c;
}


#تعریف_کرو ngx_چکری_جانچ_32_final(چکری_جانچ)                                                  \
    چکری_جانچ ^= 0xffffffff


ساکن ngx_inline uint32_t
ngx_crc32c_long(u_char *p, سائز_t len)
{
    uint32_t  چکری_جانچ;

    چکری_جانچ = 0xffffffff;

    جب (len--) {
        چکری_جانچ = ngx_crc32c_table256[(چکری_جانچ ^ *p++) & 0xff] ^ (چکری_جانچ >> 8);
    }

    واپس چکری_جانچ ^ 0xffffffff;
}


#تعریف_کرو ngx_crc32c_init(چکری_جانچ)                                                  \
    چکری_جانچ = 0xffffffff


ساکن ngx_inline خالی
ngx_crc32c_update(uint32_t *چکری_جانچ, u_char *p, سائز_t len)
{
    uint32_t  c;

    c = *چکری_جانچ;

    جب (len--) {
        c = ngx_crc32c_table256[(c ^ *p++) & 0xff] ^ (c >> 8);
    }

    *چکری_جانچ = c;
}


#تعریف_کرو ngx_crc32c_final(چکری_جانچ)                                                 \
    چکری_جانچ ^= 0xffffffff


ngx_int_t ngx_چکری_جانچ_32_جدول_init(خالی);


#اگر_ختم /* _NGX_CRC32_H_INCLUDED_ */
