
/*
 * Copyright (C) Austin Appleby
 */


#অন্তর্ভাব <ngx_config.h>
#অন্তর্ভাব <ngx_core.h>


uint32_t
ngx_murmur_hash2(u_char *তথ্য, আকার_t len)
{
    uint32_t  h, k;

    h = 0 ^ len;

    যখন (len >= 4) {
        k  = তথ্য[0];
        k |= তথ্য[1] << 8;
        k |= তথ্য[2] << 16;
        k |= তথ্য[3] << 24;

        k *= 0x5bd1e995;
        k ^= k >> 24;
        k *= 0x5bd1e995;

        h *= 0x5bd1e995;
        h ^= k;

        তথ্য += 4;
        len -= 4;
    }

    নির্বাচন (len) {
    ক্ষেত্র 3:
        h ^= তথ্য[2] << 16;
        /* fall through */
    ক্ষেত্র 2:
        h ^= তথ্য[1] << 8;
        /* fall through */
    ক্ষেত্র 1:
        h ^= তথ্য[0];
        h *= 0x5bd1e995;
    }

    h ^= h >> 13;
    h *= 0x5bd1e995;
    h ^= h >> 15;

    ফেরত h;
}
