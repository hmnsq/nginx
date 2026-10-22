
/*
 * Copyright (C) Austin Appleby
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


uint32_t
ngx_murmur_hash2(u_char *data, माप_प्रकार len)
{
    uint32_t  h, k;

    h = 0 ^ len;

    जबसम्म (len >= 4) {
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

    विकल्प (len) {
    पक्ष 3:
        h ^= data[2] << 16;
        /* fall through */
    पक्ष 2:
        h ^= data[1] << 8;
        /* fall through */
    पक्ष 1:
        h ^= data[0];
        h *= 0x5bd1e995;
    }

    h ^= h >> 13;
    h *= 0x5bd1e995;
    h ^= h >> 15;

    फर्काऊ h;
}
