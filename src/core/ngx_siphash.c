
/*
 * Copyright (C) Nginx, Inc.
 */


#شامل <ngx_config.h>
#شامل <ngx_core.h>


/*
 * SipHash-2-4 implementation based on the SipHash specification by
 * Jean-Philippe Aumasson and Daniel J. Bernstein.
 * https://eprint.iacr.org/2012/351.pdf
 */


#تعریف_کرو ngx_siphash_rotl(x, b)                                                \
    (uint64_t) (((x) << (b)) | ((x) >> (64 - (b))))

#تعریف_کرو ngx_sipround                                                          \
    کرو {                                                                      \
        v0 += v1; v1 = ngx_siphash_rotl(v1, 13); v1 ^= v0;                    \
        v0 = ngx_siphash_rotl(v0, 32);                                        \
        v2 += v3; v3 = ngx_siphash_rotl(v3, 16); v3 ^= v2;                    \
        v0 += v3; v3 = ngx_siphash_rotl(v3, 21); v3 ^= v0;                    \
        v2 += v1; v1 = ngx_siphash_rotl(v1, 17); v1 ^= v2;                    \
        v2 = ngx_siphash_rotl(v2, 32);                                        \
    } جب (0)


uint64_t
ngx_siphash(uint64_t k0, uint64_t k1, u_char *ڈیٹا, سائز_t len)
{
    u_char    *انتہا;
    سائز_t     remainder;
    uint64_t   v0, v1, v2, v3, m;

    v0 = k0 ^ 0x736f6d6570736575ULL;
    v1 = k1 ^ 0x646f72616e646f6dULL;
    v2 = k0 ^ 0x6c7967656e657261ULL;
    v3 = k1 ^ 0x7465646279746573ULL;

    remainder = len & 7;
    انتہا = ڈیٹا + len - remainder;

    برائے ( /* void */ ; ڈیٹا != انتہا; ڈیٹا += 8) {
        ngx_memcpy(&m, ڈیٹا, 8);
        v3 ^= m;
        ngx_sipround;
        ngx_sipround;
        v0 ^= m;
    }

    m = (uint64_t) len << 56;

    انتخاب (remainder) {
    حالت 7:
        m |= (uint64_t) ڈیٹا[6] << 48;
        /* fall through */
    حالت 6:
        m |= (uint64_t) ڈیٹا[5] << 40;
        /* fall through */
    حالت 5:
        m |= (uint64_t) ڈیٹا[4] << 32;
        /* fall through */
    حالت 4:
        m |= (uint64_t) ڈیٹا[3] << 24;
        /* fall through */
    حالت 3:
        m |= (uint64_t) ڈیٹا[2] << 16;
        /* fall through */
    حالت 2:
        m |= (uint64_t) ڈیٹا[1] << 8;
        /* fall through */
    حالت 1:
        m |= (uint64_t) ڈیٹا[0];
        توڑو;
    حالت 0:
        توڑو;
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

    واپس v0 ^ v1 ^ v2 ^ v3;
}
