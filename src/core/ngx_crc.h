
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#यदि_अपरिभाषितम् _NGX_CRC_H_INCLUDED_
#परिभाषय _NGX_CRC_H_INCLUDED_


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


/* 32-bit crc16 */

स्थिर ngx_inline uint32_t
ngx_crc(u_char *दत्त, आकार_t len)
{
    uint32_t  योग_2;

    प्रति (योग_2 = 0; len; len--) {

        /*
         * gcc 2.95.2 x86 and icc 7.1.006 compile
         * that operator into the single "rol" opcode,
         * msvc 6.0sp2 compiles it into four opcodes.
         */
        योग_2 = योग_2 >> 1 | योग_2 << 31;

        योग_2 += *दत्त++;
    }

    प्रत्यागम योग_2;
}


#यदि_अन्तः /* _NGX_CRC_H_INCLUDED_ */
