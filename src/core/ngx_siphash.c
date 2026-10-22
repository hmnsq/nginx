
/*
 * Copyright (C) Nginx, Inc.
 */


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>


/*
 * SipHash-2-4 implementation based on the SipHash specification by
 * Jean-Philippe Aumasson and Daniel J. Bernstein.
 * https://eprint.iacr.org/2012/351.pdf
 */


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_siphash_rotl(x, b)                                                \
    (uint64_t) (((x) << (b)) | ((x) >> (64 - (b))))

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_sipround                                                          \
    ਕਰੋ {                                                                      \
        v0 += v1; v1 = ngx_siphash_rotl(v1, 13); v1 ^= v0;                    \
        v0 = ngx_siphash_rotl(v0, 32);                                        \
        v2 += v3; v3 = ngx_siphash_rotl(v3, 16); v3 ^= v2;                    \
        v0 += v3; v3 = ngx_siphash_rotl(v3, 21); v3 ^= v0;                    \
        v2 += v1; v1 = ngx_siphash_rotl(v1, 17); v1 ^= v2;                    \
        v2 = ngx_siphash_rotl(v2, 32);                                        \
    } ਜਦੋਂ (0)


uint64_t
ngx_siphash(uint64_t k0, uint64_t k1, u_char *ਅੰਕੜੇ, ਮਾਪ_t len)
{
    u_char    *ਸਿਰਾ;
    ਮਾਪ_t     remainder;
    uint64_t   v0, v1, v2, v3, m;

    v0 = k0 ^ 0x736f6d6570736575ULL;
    v1 = k1 ^ 0x646f72616e646f6dULL;
    v2 = k0 ^ 0x6c7967656e657261ULL;
    v3 = k1 ^ 0x7465646279746573ULL;

    remainder = len & 7;
    ਸਿਰਾ = ਅੰਕੜੇ + len - remainder;

    ਲਈ ( /* void */ ; ਅੰਕੜੇ != ਸਿਰਾ; ਅੰਕੜੇ += 8) {
        ngx_memcpy(&m, ਅੰਕੜੇ, 8);
        v3 ^= m;
        ngx_sipround;
        ngx_sipround;
        v0 ^= m;
    }

    m = (uint64_t) len << 56;

    ਚੋਣ (remainder) {
    ਹਾਲਤ 7:
        m |= (uint64_t) ਅੰਕੜੇ[6] << 48;
        /* fall through */
    ਹਾਲਤ 6:
        m |= (uint64_t) ਅੰਕੜੇ[5] << 40;
        /* fall through */
    ਹਾਲਤ 5:
        m |= (uint64_t) ਅੰਕੜੇ[4] << 32;
        /* fall through */
    ਹਾਲਤ 4:
        m |= (uint64_t) ਅੰਕੜੇ[3] << 24;
        /* fall through */
    ਹਾਲਤ 3:
        m |= (uint64_t) ਅੰਕੜੇ[2] << 16;
        /* fall through */
    ਹਾਲਤ 2:
        m |= (uint64_t) ਅੰਕੜੇ[1] << 8;
        /* fall through */
    ਹਾਲਤ 1:
        m |= (uint64_t) ਅੰਕੜੇ[0];
        ਰੋਕੋ;
    ਹਾਲਤ 0:
        ਰੋਕੋ;
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

    ਵਾਪਸ v0 ^ v1 ^ v2 ^ v3;
}
