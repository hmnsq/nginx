
/*
 * Copyright (C) Austin Appleby
 */


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>


uint32_t
ngx_murmur_hash2(u_char *ਅੰਕੜੇ, ਮਾਪ_t len)
{
    uint32_t  h, k;

    h = 0 ^ len;

    ਜਦੋਂ (len >= 4) {
        k  = ਅੰਕੜੇ[0];
        k |= ਅੰਕੜੇ[1] << 8;
        k |= ਅੰਕੜੇ[2] << 16;
        k |= ਅੰਕੜੇ[3] << 24;

        k *= 0x5bd1e995;
        k ^= k >> 24;
        k *= 0x5bd1e995;

        h *= 0x5bd1e995;
        h ^= k;

        ਅੰਕੜੇ += 4;
        len -= 4;
    }

    ਚੋਣ (len) {
    ਹਾਲਤ 3:
        h ^= ਅੰਕੜੇ[2] << 16;
        /* fall through */
    ਹਾਲਤ 2:
        h ^= ਅੰਕੜੇ[1] << 8;
        /* fall through */
    ਹਾਲਤ 1:
        h ^= ਅੰਕੜੇ[0];
        h *= 0x5bd1e995;
    }

    h ^= h >> 13;
    h *= 0x5bd1e995;
    h ^= h >> 15;

    ਵਾਪਸ h;
}
