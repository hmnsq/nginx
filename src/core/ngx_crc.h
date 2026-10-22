
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#اگر_تعریف_نشده NGX_بررسی_چرخهای_H_INCLUDED
#تعریف_کن NGX_بررسی_چرخهای_H_INCLUDED


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


/* 32-bit crc16 */

ساكن ngx_inline uint32_t
ngx_بررسی_چرخهای(u_char *داده, اندازه_t len)
{
    uint32_t  جمع;

    لكل (جمع = 0; len; len--) {

        /*
         * gcc 2.95.2 x86 and icc 7.1.006 compile
         * that operator into the single "rol" opcode,
         * msvc 6.0sp2 compiles it into four opcodes.
         */
        جمع = جمع >> 1 | جمع << 31;

        جمع += *داده++;
    }

    أرجع جمع;
}


#پایان_اگر /* _NGX_CRC_H_INCLUDED_ */
