
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#اگر_غیر_تعریف_شدہ NGX_چکری_جانچ_H_INCLUDED
#تعریف_کرو NGX_چکری_جانچ_H_INCLUDED


#شامل <ngx_config.h>
#شامل <ngx_core.h>


/* 32-bit crc16 */

ساکن ngx_inline uint32_t
ngx_چکری_جانچ(u_char *ڈیٹا, سائز_t len)
{
    uint32_t  مجموع;

    برائے (مجموع = 0; len; len--) {

        /*
         * gcc 2.95.2 x86 and icc 7.1.006 compile
         * that operator into the single "rol" opcode,
         * msvc 6.0sp2 compiles it into four opcodes.
         */
        مجموع = مجموع >> 1 | مجموع << 31;

        مجموع += *ڈیٹا++;
    }

    واپس مجموع;
}


#اگر_ختم /* _NGX_CRC_H_INCLUDED_ */
