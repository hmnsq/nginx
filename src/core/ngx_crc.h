
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#جے_بنا_تعریف NGX_چکری_جانچ_H_INCLUDED
#تعریف_کرو NGX_چکری_جانچ_H_INCLUDED


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


/* 32-bit crc16 */

ساكن ngx_inline uint32_t
ngx_چکری_جانچ(u_char *انکڑے, ناپ_t len)
{
    uint32_t  جوڑ;

    لكل (جوڑ = 0; len; len--) {

        /*
         * gcc 2.95.2 x86 and icc 7.1.006 compile
         * that operator into the single "rol" opcode,
         * msvc 6.0sp2 compiles it into four opcodes.
         */
        جوڑ = جوڑ >> 1 | جوڑ << 31;

        جوڑ += *انکڑے++;
    }

    أرجع جوڑ;
}


#جے_انت /* _NGX_CRC_H_INCLUDED_ */
