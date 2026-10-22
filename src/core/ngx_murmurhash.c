
/*
 * Copyright (C) Austin Appleby
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


uint32_t
ngx_murmur_hash2(u_char *data, size_t len)
{
    uint32_t  h, k;

    h = 0 ^ len;

    ಯಾವತ್ (len >= 4) {
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

    ವಿಕಲ್ಪ (len) {
    ಪಕ್ಷ 3:
        h ^= data[2] << 16;
        /* fall through */
    ಪಕ್ಷ 2:
        h ^= data[1] << 8;
        /* fall through */
    ಪಕ್ಷ 1:
        h ^= data[0];
        h *= 0x5bd1e995;
    }

    h ^= h >> 13;
    h *= 0x5bd1e995;
    h ^= h >> 15;

    ಪ್ರತ್ಯಾಗಮ h;
}
