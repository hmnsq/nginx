
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अगर_अघोषित _NGX_CRC_H_INCLUDED_
#घोषणा _NGX_CRC_H_INCLUDED_


#समावेश <ngx_config.h>
#समावेश <ngx_core.h>


/* 32-bit crc16 */

अटल ngx_inline uint32_t
ngx_crc(u_char *तथ्य, माप_प्रकार len)
{
    uint32_t  योग_2;

    क्रम (योग_2 = 0; len; len--) {

        /*
         * gcc 2.95.2 x86 and icc 7.1.006 compile
         * that operator into the single "rol" opcode,
         * msvc 6.0sp2 compiles it into four opcodes.
         */
        योग_2 = योग_2 >> 1 | योग_2 << 31;

        योग_2 += *तथ्य++;
    }

    वापस योग_2;
}


#पूर्ण_अगर /* _NGX_CRC_H_INCLUDED_ */
