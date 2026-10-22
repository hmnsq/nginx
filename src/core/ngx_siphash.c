
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


#define ngx_siphash_rotl(x, b)                                                \
    (uint64_t) (((x) << (b)) | ((x) >> (64 - (b))))

#define ngx_sipround                                                          \
    ಕುರು {                                                                      \
        v0 += v1; v1 = ngx_siphash_rotl(v1, 13); v1 ^= v0;                    \
        v0 = ngx_siphash_rotl(v0, 32);                                        \
        v2 += v3; v3 = ngx_siphash_rotl(v3, 16); v3 ^= v2;                    \
        v0 += v3; v3 = ngx_siphash_rotl(v3, 21); v3 ^= v0;                    \
        v2 += v1; v1 = ngx_siphash_rotl(v1, 17); v1 ^= v2;                    \
        v2 = ngx_siphash_rotl(v2, 32);                                        \
    } ಯಾವತ್ (0)


uint64_t
ngx_siphash(uint64_t k0, uint64_t k1, u_char *data, size_t len)
{
    u_char    *end;
    size_t     remainder;
    uint64_t   v0, v1, v2, v3, m;

    v0 = k0 ^ 0x736f6d6570736575ULL;
    v1 = k1 ^ 0x646f72616e646f6dULL;
    v2 = k0 ^ 0x6c7967656e657261ULL;
    v3 = k1 ^ 0x7465646279746573ULL;

    remainder = len & 7;
    end = data + len - remainder;

    ಪ್ರತಿ ( /* void */ ; data != end; data += 8) {
        ngx_memcpy(&m, data, 8);
        v3 ^= m;
        ngx_sipround;
        ngx_sipround;
        v0 ^= m;
    }

    m = (uint64_t) len << 56;

    ವಿಕಲ್ಪ (remainder) {
    ಪಕ್ಷ 7:
        m |= (uint64_t) data[6] << 48;
        /* fall through */
    ಪಕ್ಷ 6:
        m |= (uint64_t) data[5] << 40;
        /* fall through */
    ಪಕ್ಷ 5:
        m |= (uint64_t) data[4] << 32;
        /* fall through */
    ಪಕ್ಷ 4:
        m |= (uint64_t) data[3] << 24;
        /* fall through */
    ಪಕ್ಷ 3:
        m |= (uint64_t) data[2] << 16;
        /* fall through */
    ಪಕ್ಷ 2:
        m |= (uint64_t) data[1] << 8;
        /* fall through */
    ಪಕ್ಷ 1:
        m |= (uint64_t) data[0];
        ಭಙ್ಗ;
    ಪಕ್ಷ 0:
        ಭಙ್ಗ;
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

    ಪ್ರತ್ಯಾಗಮ v0 ^ v1 ^ v2 ^ v3;
}
