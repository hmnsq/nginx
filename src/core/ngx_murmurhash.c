
/*
 * Copyright (C) Austin Appleby
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


uint32_t
ngx_murmur_hash2(u_char *दत्त, आकार_t len)
{
    uint32_t  h, k;

    h = 0 ^ len;

    यावत् (len >= 4) {
        k  = दत्त[0];
        k |= दत्त[1] << 8;
        k |= दत्त[2] << 16;
        k |= दत्त[3] << 24;

        k *= 0x5bd1e995;
        k ^= k >> 24;
        k *= 0x5bd1e995;

        h *= 0x5bd1e995;
        h ^= k;

        दत्त += 4;
        len -= 4;
    }

    विकल्प (len) {
    पक्ष 3:
        h ^= दत्त[2] << 16;
        /* fall through */
    पक्ष 2:
        h ^= दत्त[1] << 8;
        /* fall through */
    पक्ष 1:
        h ^= दत्त[0];
        h *= 0x5bd1e995;
    }

    h ^= h >> 13;
    h *= 0x5bd1e995;
    h ^= h >> 15;

    प्रत्यागम h;
}
