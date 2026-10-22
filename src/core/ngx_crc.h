
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_CRC_H_INCLUDED_
#define _NGX_CRC_H_INCLUDED_


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


/* 32-bit crc16 */

ಸ್ಥಿರ ngx_inline uint32_t
ngx_crc(u_char *data, size_t len)
{
    uint32_t  sum;

    ಪ್ರತಿ (sum = 0; len; len--) {

        /*
         * gcc 2.95.2 x86 and icc 7.1.006 compile
         * that operator into the single "rol" opcode,
         * msvc 6.0sp2 compiles it into four opcodes.
         */
        sum = sum >> 1 | sum << 31;

        sum += *data++;
    }

    ಪ್ರತ್ಯಾಗಮ sum;
}


#endif /* _NGX_CRC_H_INCLUDED_ */
