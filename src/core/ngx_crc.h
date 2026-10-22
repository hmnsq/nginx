
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#నిర్వచితం_కాకపోతే NGX_చక్రీయ_తనిఖీ_H_INCLUDED
#నిర్వచించు NGX_చక్రీయ_తనిఖీ_H_INCLUDED


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>


/* 32-bit crc16 */

స్థిర ngx_inline uint32_t
ngx_చక్రీయ_తనిఖీ(u_char *దత్తాంశం, కొలత_t len)
{
    uint32_t  కూడిక;

    కొరకు (కూడిక = 0; len; len--) {

        /*
         * gcc 2.95.2 x86 and icc 7.1.006 compile
         * that operator into the single "rol" opcode,
         * msvc 6.0sp2 compiles it into four opcodes.
         */
        కూడిక = కూడిక >> 1 | కూడిక << 31;

        కూడిక += *దత్తాంశం++;
    }

    తిరిగి కూడిక;
}


#అయితే_ముగింపు /* _NGX_CRC_H_INCLUDED_ */
