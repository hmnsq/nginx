
/*
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


/*
 * SipHash-2-4 implementation based on the SipHash specification by
 * Jean-Philippe Aumasson and Daniel J. Bernstein.
 * https://eprint.iacr.org/2012/351.pdf
 */


#define ngx_siphash_rotl(x, b)                                                \
    (uint64_t) (((x) << (b)) | ((x) >> (64 - (b))))

#define ngx_sipround                                                          \
    करा {                                                                      \
        v0 += v1; v1 = ngx_siphash_rotl(v1, 13); v1 ^= v0;                    \
        v0 = ngx_siphash_rotl(v0, 32);                                        \
        v2 += v3; v3 = ngx_siphash_rotl(v3, 16); v3 ^= v2;                    \
        v0 += v3; v3 = ngx_siphash_rotl(v3, 21); v3 ^= v0;                    \
        v2 += v1; v1 = ngx_siphash_rotl(v1, 17); v1 ^= v2;                    \
        v2 = ngx_siphash_rotl(v2, 32);                                        \
    } पर्यंत (0)


uint64_t
ngx_siphash(uint64_t k0, uint64_t k1, u_char *data, माप_प्रकार len)
{
    u_char    *end;
    माप_प्रकार     remainder;
    uint64_t   v0, v1, v2, v3, m;

    v0 = k0 ^ 0x736f6d6570736575ULL;
    v1 = k1 ^ 0x646f72616e646f6dULL;
    v2 = k0 ^ 0x6c7967656e657261ULL;
    v3 = k1 ^ 0x7465646279746573ULL;

    remainder = len & 7;
    end = data + len - remainder;

    साठी ( /* void */ ; data != end; data += 8) {
        ngx_memcpy(&m, data, 8);
        v3 ^= m;
        ngx_sipround;
        ngx_sipround;
        v0 ^= m;
    }

    m = (uint64_t) len << 56;

    निवड (remainder) {
    स्थिती 7:
        m |= (uint64_t) data[6] << 48;
        /* fall through */
    स्थिती 6:
        m |= (uint64_t) data[5] << 40;
        /* fall through */
    स्थिती 5:
        m |= (uint64_t) data[4] << 32;
        /* fall through */
    स्थिती 4:
        m |= (uint64_t) data[3] << 24;
        /* fall through */
    स्थिती 3:
        m |= (uint64_t) data[2] << 16;
        /* fall through */
    स्थिती 2:
        m |= (uint64_t) data[1] << 8;
        /* fall through */
    स्थिती 1:
        m |= (uint64_t) data[0];
        थांबा;
    स्थिती 0:
        थांबा;
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

    परत v0 ^ v1 ^ v2 ^ v3;
}
