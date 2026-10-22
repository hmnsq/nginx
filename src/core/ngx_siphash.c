
/*
 * Copyright (C) Nginx, Inc.
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


/*
 * SipHash-2-4 implementation based on the SipHash specification by
 * Jean-Philippe Aumasson and Daniel J. Bernstein.
 * https://eprint.iacr.org/2012/351.pdf
 */


#ವ್ಯಾಖ್ಯಾನಿಸು ngx_siphash_rotl(x, b)                                                \
    (uint64_t) (((x) << (b)) | ((x) >> (64 - (b))))

#ವ್ಯಾಖ್ಯಾನಿಸು ngx_sipround                                                          \
    ಮಾಡು {                                                                      \
        v0 += v1; v1 = ngx_siphash_rotl(v1, 13); v1 ^= v0;                    \
        v0 = ngx_siphash_rotl(v0, 32);                                        \
        v2 += v3; v3 = ngx_siphash_rotl(v3, 16); v3 ^= v2;                    \
        v0 += v3; v3 = ngx_siphash_rotl(v3, 21); v3 ^= v0;                    \
        v2 += v1; v1 = ngx_siphash_rotl(v1, 17); v1 ^= v2;                    \
        v2 = ngx_siphash_rotl(v2, 32);                                        \
    } ಆಗ (0)


uint64_t
ngx_siphash(uint64_t k0, uint64_t k1, u_char *ದತ್ತಾಂಶ, ಗಾತ್ರ_t len)
{
    u_char    *ಕೊನೆ;
    ಗಾತ್ರ_t     remainder;
    uint64_t   v0, v1, v2, v3, m;

    v0 = k0 ^ 0x736f6d6570736575ULL;
    v1 = k1 ^ 0x646f72616e646f6dULL;
    v2 = k0 ^ 0x6c7967656e657261ULL;
    v3 = k1 ^ 0x7465646279746573ULL;

    remainder = len & 7;
    ಕೊನೆ = ದತ್ತಾಂಶ + len - remainder;

    ಗಾಗಿ ( /* void */ ; ದತ್ತಾಂಶ != ಕೊನೆ; ದತ್ತಾಂಶ += 8) {
        ngx_memcpy(&m, ದತ್ತಾಂಶ, 8);
        v3 ^= m;
        ngx_sipround;
        ngx_sipround;
        v0 ^= m;
    }

    m = (uint64_t) len << 56;

    ಆಯ್ಕೆ (remainder) {
    ಸ್ಥಿತಿ 7:
        m |= (uint64_t) ದತ್ತಾಂಶ[6] << 48;
        /* fall through */
    ಸ್ಥಿತಿ 6:
        m |= (uint64_t) ದತ್ತಾಂಶ[5] << 40;
        /* fall through */
    ಸ್ಥಿತಿ 5:
        m |= (uint64_t) ದತ್ತಾಂಶ[4] << 32;
        /* fall through */
    ಸ್ಥಿತಿ 4:
        m |= (uint64_t) ದತ್ತಾಂಶ[3] << 24;
        /* fall through */
    ಸ್ಥಿತಿ 3:
        m |= (uint64_t) ದತ್ತಾಂಶ[2] << 16;
        /* fall through */
    ಸ್ಥಿತಿ 2:
        m |= (uint64_t) ದತ್ತಾಂಶ[1] << 8;
        /* fall through */
    ಸ್ಥಿತಿ 1:
        m |= (uint64_t) ದತ್ತಾಂಶ[0];
        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 0:
        ನಿಲ್ಲಿಸು;
    }

    v3 ^= m;
    ngx_sipround;
    ngx_sipround;
    v0 ^= m;

    v2 ^= 0xff;
    ngx_sipround;
    ngx_sipround;
    ngx_sipround;
    ngx_sipround;

    ಹಿಂತಿರುಗಿ v0 ^ v1 ^ v2 ^ v3;
}
