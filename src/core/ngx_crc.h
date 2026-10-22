
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ NGX_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_H_INCLUDED
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_H_INCLUDED


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


/* 32-bit crc16 */

ಸ್ಥಿರ ngx_inline uint32_t
ngx_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ(u_char *ದತ್ತಾಂಶ, ಗಾತ್ರ_t len)
{
    uint32_t  ಮೊತ್ತ;

    ಗಾಗಿ (ಮೊತ್ತ = 0; len; len--) {

        /*
         * gcc 2.95.2 x86 and icc 7.1.006 compile
         * that operator into the single "rol" opcode,
         * msvc 6.0sp2 compiles it into four opcodes.
         */
        ಮೊತ್ತ = ಮೊತ್ತ >> 1 | ಮೊತ್ತ << 31;

        ಮೊತ್ತ += *ದತ್ತಾಂಶ++;
    }

    ಹಿಂತಿರುಗಿ ಮೊತ್ತ;
}


#ಆದರೆ_ಅಂತ್ಯ /* _NGX_CRC_H_INCLUDED_ */
