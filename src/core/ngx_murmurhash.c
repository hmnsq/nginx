
/*
 * Copyright (C) Austin Appleby
 */


#شامل <ngx_config.h>
#شامل <ngx_core.h>


uint32_t
ngx_murmur_hash2(u_char *ڈیٹا, سائز_t len)
{
    uint32_t  h, k;

    h = 0 ^ len;

    جب (len >= 4) {
        k  = ڈیٹا[0];
        k |= ڈیٹا[1] << 8;
        k |= ڈیٹا[2] << 16;
        k |= ڈیٹا[3] << 24;

        k *= 0x5bd1e995;
        k ^= k >> 24;
        k *= 0x5bd1e995;

        h *= 0x5bd1e995;
        h ^= k;

        ڈیٹا += 4;
        len -= 4;
    }

    انتخاب (len) {
    حالت 3:
        h ^= ڈیٹا[2] << 16;
        /* fall through */
    حالت 2:
        h ^= ڈیٹا[1] << 8;
        /* fall through */
    حالت 1:
        h ^= ڈیٹا[0];
        h *= 0x5bd1e995;
    }

    h ^= h >> 13;
    h *= 0x5bd1e995;
    h ^= h >> 15;

    واپس h;
}
