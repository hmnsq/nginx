
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#جيڪڏهن_وصف_نه_ٿيل NGX_چڪري_جاچ_H_INCLUDED
#وصف_ڪريو NGX_چڪري_جاچ_H_INCLUDED


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


/* 32-bit crc16 */

ساكن ngx_inline uint32_t
ngx_چڪري_جاچ(u_char *معلومات, سائيز_t len)
{
    uint32_t  جوڙ;

    لكل (جوڙ = 0; len; len--) {

        /*
         * gcc 2.95.2 x86 and icc 7.1.006 compile
         * that operator into the single "rol" opcode,
         * msvc 6.0sp2 compiles it into four opcodes.
         */
        جوڙ = جوڙ >> 1 | جوڙ << 31;

        جوڙ += *معلومات++;
    }

    أرجع جوڙ;
}


#جيڪڏهن_پورو /* _NGX_CRC_H_INCLUDED_ */
