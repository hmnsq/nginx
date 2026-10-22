
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ NGX_ਚੱਕਰੀ_ਜਾਂਚ_H_INCLUDED
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_ਚੱਕਰੀ_ਜਾਂਚ_H_INCLUDED


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>


/* 32-bit crc16 */

ਸਥਿਰ ngx_inline uint32_t
ngx_ਚੱਕਰੀ_ਜਾਂਚ(u_char *ਅੰਕੜੇ, ਮਾਪ_t len)
{
    uint32_t  ਜੋੜ;

    ਲਈ (ਜੋੜ = 0; len; len--) {

        /*
         * gcc 2.95.2 x86 and icc 7.1.006 compile
         * that operator into the single "rol" opcode,
         * msvc 6.0sp2 compiles it into four opcodes.
         */
        ਜੋੜ = ਜੋੜ >> 1 | ਜੋੜ << 31;

        ਜੋੜ += *ਅੰਕੜੇ++;
    }

    ਵਾਪਸ ਜੋੜ;
}


#ਜੇ_ਅੰਤ /* _NGX_CRC_H_INCLUDED_ */
