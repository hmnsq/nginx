
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#که_نه_تعریف_شوی NGX_دوراني_ازموینه_H_INCLUDED
#تعریف_کړه NGX_دوراني_ازموینه_H_INCLUDED


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


/* 32-bit crc16 */

ساكن ngx_inline uint32_t
ngx_دوراني_ازموینه(u_char *معلومات, اندازه_t len)
{
    uint32_t  مجموعه;

    لكل (مجموعه = 0; len; len--) {

        /*
         * gcc 2.95.2 x86 and icc 7.1.006 compile
         * that operator into the single "rol" opcode,
         * msvc 6.0sp2 compiles it into four opcodes.
         */
        مجموعه = مجموعه >> 1 | مجموعه << 31;

        مجموعه += *معلومات++;
    }

    أرجع مجموعه;
}


#د_که_پای /* _NGX_CRC_H_INCLUDED_ */
