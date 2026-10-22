
/*
 * Copyright (C) Austin Appleby
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


uint32_t
ngx_murmur_hash2(u_char *معلومات, سائيز_t len)
{
    uint32_t  h, k;

    h = 0 ^ len;

    طالما (len >= 4) {
        k  = معلومات[0];
        k |= معلومات[1] << 8;
        k |= معلومات[2] << 16;
        k |= معلومات[3] << 24;

        k *= 0x5bd1e995;
        k ^= k >> 24;
        k *= 0x5bd1e995;

        h *= 0x5bd1e995;
        h ^= k;

        معلومات += 4;
        len -= 4;
    }

    بدل (len) {
    حالة 3:
        h ^= معلومات[2] << 16;
        /* fall through */
    حالة 2:
        h ^= معلومات[1] << 8;
        /* fall through */
    حالة 1:
        h ^= معلومات[0];
        h *= 0x5bd1e995;
    }

    h ^= h >> 13;
    h *= 0x5bd1e995;
    h ^= h >> 15;

    أرجع h;
}
