
/*
 * Copyright (C) Austin Appleby
 */


#समावेश <ngx_config.h>
#समावेश <ngx_core.h>


uint32_t
ngx_murmur_hash2(u_char *तथ्य, माप_प्रकार len)
{
    uint32_t  h, k;

    h = 0 ^ len;

    जबतक (len >= 4) {
        k  = तथ्य[0];
        k |= तथ्य[1] << 8;
        k |= तथ्य[2] << 16;
        k |= तथ्य[3] << 24;

        k *= 0x5bd1e995;
        k ^= k >> 24;
        k *= 0x5bd1e995;

        h *= 0x5bd1e995;
        h ^= k;

        तथ्य += 4;
        len -= 4;
    }

    चयन (len) {
    हाल 3:
        h ^= तथ्य[2] << 16;
        /* fall through */
    हाल 2:
        h ^= तथ्य[1] << 8;
        /* fall through */
    हाल 1:
        h ^= तथ्य[0];
        h *= 0x5bd1e995;
    }

    h ^= h >> 13;
    h *= 0x5bd1e995;
    h ^= h >> 15;

    वापस h;
}
