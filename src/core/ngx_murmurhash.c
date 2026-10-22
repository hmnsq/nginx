
/*
 * Copyright (C) Austin Appleby
 */


#ଅନ୍ତର୍ଭାଵ <ngx_config.h>
#ଅନ୍ତର୍ଭାଵ <ngx_core.h>


uint32_t
ngx_murmur_hash2(u_char *data, size_t len)
{
    uint32_t  h, k;

    h = 0 ^ len;

    ଯେତେବେଳେ (len >= 4) {
        k  = data[0];
        k |= data[1] << 8;
        k |= data[2] << 16;
        k |= data[3] << 24;

        k *= 0x5bd1e995;
        k ^= k >> 24;
        k *= 0x5bd1e995;

        h *= 0x5bd1e995;
        h ^= k;

        data += 4;
        len -= 4;
    }

    ବାଛ (len) {
    ସ୍ଥିତି 3:
        h ^= data[2] << 16;
        /* fall through */
    ସ୍ଥିତି 2:
        h ^= data[1] << 8;
        /* fall through */
    ସ୍ଥିତି 1:
        h ^= data[0];
        h *= 0x5bd1e995;
    }

    h ^= h >> 13;
    h *= 0x5bd1e995;
    h ^= h >> 15;

    ଫେରନ୍ତୁ h;
}
