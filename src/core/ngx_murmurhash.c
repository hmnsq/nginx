
/*
 * Copyright (C) Austin Appleby
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


uint32_t
ngx_murmur_hash2(u_char *ದತ್ತಾಂಶ, ಗಾತ್ರ_t len)
{
    uint32_t  h, k;

    h = 0 ^ len;

    ಆಗ (len >= 4) {
        k  = ದತ್ತಾಂಶ[0];
        k |= ದತ್ತಾಂಶ[1] << 8;
        k |= ದತ್ತಾಂಶ[2] << 16;
        k |= ದತ್ತಾಂಶ[3] << 24;

        k *= 0x5bd1e995;
        k ^= k >> 24;
        k *= 0x5bd1e995;

        h *= 0x5bd1e995;
        h ^= k;

        ದತ್ತಾಂಶ += 4;
        len -= 4;
    }

    ಆಯ್ಕೆ (len) {
    ಸ್ಥಿತಿ 3:
        h ^= ದತ್ತಾಂಶ[2] << 16;
        /* fall through */
    ಸ್ಥಿತಿ 2:
        h ^= ದತ್ತಾಂಶ[1] << 8;
        /* fall through */
    ಸ್ಥಿತಿ 1:
        h ^= ದತ್ತಾಂಶ[0];
        h *= 0x5bd1e995;
    }

    h ^= h >> 13;
    h *= 0x5bd1e995;
    h ^= h >> 15;

    ಹಿಂತಿರುಗಿ h;
}
