
/*
 * Copyright (C) Austin Appleby
 */


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>


uint32_t
ngx_murmur_hash2(u_char *దత్తాంశం, కొలత_t len)
{
    uint32_t  h, k;

    h = 0 ^ len;

    అయితే (len >= 4) {
        k  = దత్తాంశం[0];
        k |= దత్తాంశం[1] << 8;
        k |= దత్తాంశం[2] << 16;
        k |= దత్తాంశం[3] << 24;

        k *= 0x5bd1e995;
        k ^= k >> 24;
        k *= 0x5bd1e995;

        h *= 0x5bd1e995;
        h ^= k;

        దత్తాంశం += 4;
        len -= 4;
    }

    ఎంపిక (len) {
    స్థితి 3:
        h ^= దత్తాంశం[2] << 16;
        /* fall through */
    స్థితి 2:
        h ^= దత్తాంశం[1] << 8;
        /* fall through */
    స్థితి 1:
        h ^= దత్తాంశం[0];
        h *= 0x5bd1e995;
    }

    h ^= h >> 13;
    h *= 0x5bd1e995;
    h ^= h >> 15;

    తిరిగి h;
}
