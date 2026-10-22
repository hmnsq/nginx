
/*
 * Copyright (C) Nginx, Inc.
 */


#അന്തര്ഭാവ <ngx_config.h>
#അന്തര്ഭാവ <ngx_core.h>


/*
 * SipHash-2-4 implementation based on the SipHash specification by
 * Jean-Philippe Aumasson and Daniel J. Bernstein.
 * https://eprint.iacr.org/2012/351.pdf
 */


#define ngx_siphash_rotl(x, b)                                                \
    (uint64_t) (((x) << (b)) | ((x) >> (64 - (b))))

#define ngx_sipround                                                          \
    ചെയ്യുക {                                                                      \
        v0 += v1; v1 = ngx_siphash_rotl(v1, 13); v1 ^= v0;                    \
        v0 = ngx_siphash_rotl(v0, 32);                                        \
        v2 += v3; v3 = ngx_siphash_rotl(v3, 16); v3 ^= v2;                    \
        v0 += v3; v3 = ngx_siphash_rotl(v3, 21); v3 ^= v0;                    \
        v2 += v1; v1 = ngx_siphash_rotl(v1, 17); v1 ^= v2;                    \
        v2 = ngx_siphash_rotl(v2, 32);                                        \
    } ആകുമ്പോൾ (0)


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

    വേണ്ടി ( /* void */ ; data != end; data += 8) {
        ngx_memcpy(&m, data, 8);
        v3 ^= m;
        ngx_sipround;
        ngx_sipround;
        v0 ^= m;
    }

    m = (uint64_t) len << 56;

    തിരഞ്ഞെടുപ്പ് (remainder) {
    അവസ്ഥ 7:
        m |= (uint64_t) data[6] << 48;
        /* fall through */
    അവസ്ഥ 6:
        m |= (uint64_t) data[5] << 40;
        /* fall through */
    അവസ്ഥ 5:
        m |= (uint64_t) data[4] << 32;
        /* fall through */
    അവസ്ഥ 4:
        m |= (uint64_t) data[3] << 24;
        /* fall through */
    അവസ്ഥ 3:
        m |= (uint64_t) data[2] << 16;
        /* fall through */
    അവസ്ഥ 2:
        m |= (uint64_t) data[1] << 8;
        /* fall through */
    അവസ്ഥ 1:
        m |= (uint64_t) data[0];
        നിർത്തുക;
    അവസ്ഥ 0:
        നിർത്തുക;
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

    തിരികെ v0 ^ v1 ^ v2 ^ v3;
}
