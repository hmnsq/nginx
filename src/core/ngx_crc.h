
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_CRC_H_INCLUDED_
#define _NGX_CRC_H_INCLUDED_


#অন্তর্ভাব <ngx_config.h>
#অন্তর্ভাব <ngx_core.h>


/* 32-bit crc16 */

স্থির ngx_inline uint32_t
ngx_crc(u_char *তথ্য, আকার_t len)
{
    uint32_t  যোগ_2;

    জন্য (যোগ_2 = 0; len; len--) {

        /*
         * gcc 2.95.2 x86 and icc 7.1.006 compile
         * that operator into the single "rol" opcode,
         * msvc 6.0sp2 compiles it into four opcodes.
         */
        যোগ_2 = যোগ_2 >> 1 | যোগ_2 << 31;

        যোগ_2 += *তথ্য++;
    }

    ফেরত যোগ_2;
}


#endif /* _NGX_CRC_H_INCLUDED_ */
