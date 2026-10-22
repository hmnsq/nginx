
/*
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


/*
 * SipHash-2-4 implementation based on the SipHash specification by
 * Jean-Philippe Aumasson and Daniel J. Bernstein.
 * https://eprint.iacr.org/2012/351.pdf
 */


#تعریف_کن ngx_siphash_rotl(x, b)                                                \
    (uint64_t) (((x) << (b)) | ((x) >> (64 - (b))))

#تعریف_کن ngx_sipround                                                          \
    افعل {                                                                      \
        v0 += v1; v1 = ngx_siphash_rotl(v1, 13); v1 ^= v0;                    \
        v0 = ngx_siphash_rotl(v0, 32);                                        \
        v2 += v3; v3 = ngx_siphash_rotl(v3, 16); v3 ^= v2;                    \
        v0 += v3; v3 = ngx_siphash_rotl(v3, 21); v3 ^= v0;                    \
        v2 += v1; v1 = ngx_siphash_rotl(v1, 17); v1 ^= v2;                    \
        v2 = ngx_siphash_rotl(v2, 32);                                        \
    } طالما (0)


uint64_t
ngx_siphash(uint64_t k0, uint64_t k1, u_char *داده, اندازه_t len)
{
    u_char    *انتها;
    اندازه_t     remainder;
    uint64_t   v0, v1, v2, v3, m;

    v0 = k0 ^ 0x736f6d6570736575ULL;
    v1 = k1 ^ 0x646f72616e646f6dULL;
    v2 = k0 ^ 0x6c7967656e657261ULL;
    v3 = k1 ^ 0x7465646279746573ULL;

    remainder = len & 7;
    انتها = داده + len - remainder;

    لكل ( /* void */ ; داده != انتها; داده += 8) {
        ngx_memcpy(&m, داده, 8);
        v3 ^= m;
        ngx_sipround;
        ngx_sipround;
        v0 ^= m;
    }

    m = (uint64_t) len << 56;

    بدل (remainder) {
    حالة 7:
        m |= (uint64_t) داده[6] << 48;
        /* fall through */
    حالة 6:
        m |= (uint64_t) داده[5] << 40;
        /* fall through */
    حالة 5:
        m |= (uint64_t) داده[4] << 32;
        /* fall through */
    حالة 4:
        m |= (uint64_t) داده[3] << 24;
        /* fall through */
    حالة 3:
        m |= (uint64_t) داده[2] << 16;
        /* fall through */
    حالة 2:
        m |= (uint64_t) داده[1] << 8;
        /* fall through */
    حالة 1:
        m |= (uint64_t) داده[0];
        اكسر;
    حالة 0:
        اكسر;
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

    أرجع v0 ^ v1 ^ v2 ^ v3;
}
