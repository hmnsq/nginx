
/*
 * Copyright (C) Nginx, Inc.
 */


#समावेश <ngx_config.h>
#समावेश <ngx_core.h>


/*
 * SipHash-2-4 implementation based on the SipHash specification by
 * Jean-Philippe Aumasson and Daniel J. Bernstein.
 * https://eprint.iacr.org/2012/351.pdf
 */


#घोषणा ngx_siphash_rotl(x, b)                                                \
    (uint64_t) (((x) << (b)) | ((x) >> (64 - (b))))

#घोषणा ngx_sipround                                                          \
    करो {                                                                      \
        v0 += v1; v1 = ngx_siphash_rotl(v1, 13); v1 ^= v0;                    \
        v0 = ngx_siphash_rotl(v0, 32);                                        \
        v2 += v3; v3 = ngx_siphash_rotl(v3, 16); v3 ^= v2;                    \
        v0 += v3; v3 = ngx_siphash_rotl(v3, 21); v3 ^= v0;                    \
        v2 += v1; v1 = ngx_siphash_rotl(v1, 17); v1 ^= v2;                    \
        v2 = ngx_siphash_rotl(v2, 32);                                        \
    } जबतक (0)


uint64_t
ngx_siphash(uint64_t k0, uint64_t k1, u_char *तथ्य, माप_प्रकार len)
{
    u_char    *end;
    माप_प्रकार     remainder;
    uint64_t   v0, v1, v2, v3, m;

    v0 = k0 ^ 0x736f6d6570736575ULL;
    v1 = k1 ^ 0x646f72616e646f6dULL;
    v2 = k0 ^ 0x6c7967656e657261ULL;
    v3 = k1 ^ 0x7465646279746573ULL;

    remainder = len & 7;
    end = तथ्य + len - remainder;

    क्रम ( /* void */ ; तथ्य != end; तथ्य += 8) {
        ngx_memcpy(&m, तथ्य, 8);
        v3 ^= m;
        ngx_sipround;
        ngx_sipround;
        v0 ^= m;
    }

    m = (uint64_t) len << 56;

    चयन (remainder) {
    हाल 7:
        m |= (uint64_t) तथ्य[6] << 48;
        /* fall through */
    हाल 6:
        m |= (uint64_t) तथ्य[5] << 40;
        /* fall through */
    हाल 5:
        m |= (uint64_t) तथ्य[4] << 32;
        /* fall through */
    हाल 4:
        m |= (uint64_t) तथ्य[3] << 24;
        /* fall through */
    हाल 3:
        m |= (uint64_t) तथ्य[2] << 16;
        /* fall through */
    हाल 2:
        m |= (uint64_t) तथ्य[1] << 8;
        /* fall through */
    हाल 1:
        m |= (uint64_t) तथ्य[0];
        अवरोध;
    हाल 0:
        अवरोध;
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

    वापस v0 ^ v1 ^ v2 ^ v3;
}
