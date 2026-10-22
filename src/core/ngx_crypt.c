
/*
 * Copyright (C) Maxim Dounin
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>
#ಅನ್ತರ್ಭಾವ <ngx_crypt.h>
#ಅನ್ತರ್ಭಾವ <ngx_md5.h>
#ಅನ್ತರ್ಭಾವ <ngx_sha1.h>


#ಆದರೆ (NGX_CRYPT)

ಸ್ಥಿರ ngx_int_t ngx_crypt_apr1(ngx_pool_t *pool, u_char *ಕೀಲಿ, u_char *salt,
    u_char **encrypted);
ಸ್ಥಿರ ngx_int_t ngx_crypt_plain(ngx_pool_t *pool, u_char *ಕೀಲಿ, u_char *salt,
    u_char **encrypted);
ಸ್ಥಿರ ngx_int_t ngx_crypt_ssha(ngx_pool_t *pool, u_char *ಕೀಲಿ, u_char *salt,
    u_char **encrypted);
ಸ್ಥಿರ ngx_int_t ngx_crypt_sha(ngx_pool_t *pool, u_char *ಕೀಲಿ, u_char *salt,
    u_char **encrypted);


ಸ್ಥಿರ u_char *ngx_crypt_to64(u_char *p, uint32_t v, ಗಾತ್ರ_t n);


ngx_int_t
ngx_crypt(ngx_pool_t *pool, u_char *ಕೀಲಿ, u_char *salt, u_char **encrypted)
{
    ಆದರೆ (ngx_strncmp(salt, "$apr1$", ಗಾತ್ರ("$apr1$") - 1) == 0) {
        ಹಿಂತಿರುಗಿ ngx_crypt_apr1(pool, ಕೀಲಿ, salt, encrypted);

    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ngx_strncmp(salt, "{PLAIN}", ಗಾತ್ರ("{PLAIN}") - 1) == 0) {
        ಹಿಂತಿರುಗಿ ngx_crypt_plain(pool, ಕೀಲಿ, salt, encrypted);

    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ngx_strncmp(salt, "{SSHA}", ಗಾತ್ರ("{SSHA}") - 1) == 0) {
        ಹಿಂತಿರುಗಿ ngx_crypt_ssha(pool, ಕೀಲಿ, salt, encrypted);

    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ngx_strncmp(salt, "{SHA}", ಗಾತ್ರ("{SHA}") - 1) == 0) {
        ಹಿಂತಿರುಗಿ ngx_crypt_sha(pool, ಕೀಲಿ, salt, encrypted);
    }

    /* fallback to libc crypt() */

    ಹಿಂತಿರುಗಿ ngx_libc_crypt(pool, ಕೀಲಿ, salt, encrypted);
}


ಸ್ಥಿರ ngx_int_t
ngx_crypt_apr1(ngx_pool_t *pool, u_char *ಕೀಲಿ, u_char *salt, u_char **encrypted)
{
    ngx_int_t          n;
    ngx_uint_t         i;
    u_char            *p, *ಕೊನೆಯ, final[16];
    ಗಾತ್ರ_t             saltlen, keylen;
    ngx_md5_t          md5, ctx1;

    /* Apache's apr1 crypt is Poul-Henning Kamp's md5 crypt with $apr1$ magic */

    keylen = ngx_strlen(ಕೀಲಿ);

    /* true salt: no magic, max 8 chars, stop at first $ */

    salt += ಗಾತ್ರ("$apr1$") - 1;
    ಕೊನೆಯ = salt + 8;
    ಗಾಗಿ (p = salt; *p && *p != '$' && p < ಕೊನೆಯ; p++) { /* void */ }
    saltlen = p - salt;

    /* hash key and salt */

    ngx_md5_init(&md5);
    ngx_md5_update(&md5, ಕೀಲಿ, keylen);
    ngx_md5_update(&md5, (u_char *) "$apr1$", ಗಾತ್ರ("$apr1$") - 1);
    ngx_md5_update(&md5, salt, saltlen);

    ngx_md5_init(&ctx1);
    ngx_md5_update(&ctx1, ಕೀಲಿ, keylen);
    ngx_md5_update(&ctx1, salt, saltlen);
    ngx_md5_update(&ctx1, ಕೀಲಿ, keylen);
    ngx_md5_final(final, &ctx1);

    ಗಾಗಿ (n = keylen; n > 0; n -= 16) {
        ngx_md5_update(&md5, final, n > 16 ? 16 : n);
    }

    ngx_memzero(final, ಗಾತ್ರ(final));

    ಗಾಗಿ (i = keylen; i; i >>= 1) {
        ಆದರೆ (i & 1) {
            ngx_md5_update(&md5, final, 1);

        } ಇಲ್ಲದಿದ್ದರೆ {
            ngx_md5_update(&md5, ಕೀಲಿ, 1);
        }
    }

    ngx_md5_final(final, &md5);

    ಗಾಗಿ (i = 0; i < 1000; i++) {
        ngx_md5_init(&ctx1);

        ಆದರೆ (i & 1) {
            ngx_md5_update(&ctx1, ಕೀಲಿ, keylen);

        } ಇಲ್ಲದಿದ್ದರೆ {
            ngx_md5_update(&ctx1, final, 16);
        }

        ಆದರೆ (i % 3) {
            ngx_md5_update(&ctx1, salt, saltlen);
        }

        ಆದರೆ (i % 7) {
            ngx_md5_update(&ctx1, ಕೀಲಿ, keylen);
        }

        ಆದರೆ (i & 1) {
            ngx_md5_update(&ctx1, final, 16);

        } ಇಲ್ಲದಿದ್ದರೆ {
            ngx_md5_update(&ctx1, ಕೀಲಿ, keylen);
        }

        ngx_md5_final(final, &ctx1);
    }

    /* output */

    *encrypted = ngx_pnalloc(pool, ಗಾತ್ರ("$apr1$") - 1 + saltlen + 1 + 22 + 1);
    ಆದರೆ (*encrypted == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    p = ngx_cpymem(*encrypted, "$apr1$", ಗಾತ್ರ("$apr1$") - 1);
    p = ngx_copy(p, salt, saltlen);
    *p++ = '$';

    p = ngx_crypt_to64(p, (final[ 0]<<16) | (final[ 6]<<8) | final[12], 4);
    p = ngx_crypt_to64(p, (final[ 1]<<16) | (final[ 7]<<8) | final[13], 4);
    p = ngx_crypt_to64(p, (final[ 2]<<16) | (final[ 8]<<8) | final[14], 4);
    p = ngx_crypt_to64(p, (final[ 3]<<16) | (final[ 9]<<8) | final[15], 4);
    p = ngx_crypt_to64(p, (final[ 4]<<16) | (final[10]<<8) | final[ 5], 4);
    p = ngx_crypt_to64(p, final[11], 2);
    *p = '\0';

    ಹಿಂತಿರುಗಿ NGX_OK;
}


ಸ್ಥಿರ u_char *
ngx_crypt_to64(u_char *p, uint32_t v, ಗಾತ್ರ_t n)
{
    ಸ್ಥಿರ u_char   itoa64[] =
        "./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    ಆಗ (n--) {
        *p++ = itoa64[v & 0x3f];
        v >>= 6;
    }

    ಹಿಂತಿರುಗಿ p;
}


ಸ್ಥಿರ ngx_int_t
ngx_crypt_plain(ngx_pool_t *pool, u_char *ಕೀಲಿ, u_char *salt, u_char **encrypted)
{
    ಗಾತ್ರ_t   len;
    u_char  *p;

    len = ngx_strlen(ಕೀಲಿ);

    *encrypted = ngx_pnalloc(pool, ಗಾತ್ರ("{PLAIN}") - 1 + len + 1);
    ಆದರೆ (*encrypted == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    p = ngx_cpymem(*encrypted, "{PLAIN}", ಗಾತ್ರ("{PLAIN}") - 1);
    ngx_memcpy(p, ಕೀಲಿ, len + 1);

    ಹಿಂತಿರುಗಿ NGX_OK;
}


ಸ್ಥಿರ ngx_int_t
ngx_crypt_ssha(ngx_pool_t *pool, u_char *ಕೀಲಿ, u_char *salt, u_char **encrypted)
{
    ಗಾತ್ರ_t       len;
    ngx_int_t    rc;
    ngx_str_t    encoded, decoded;
    ngx_sha1_t   sha1;

    /* "{SSHA}" base64(SHA1(key salt) salt) */

    /* decode base64 salt to find out true salt */

    encoded.ದತ್ತಾಂಶ = salt + ಗಾತ್ರ("{SSHA}") - 1;
    encoded.len = ngx_strlen(encoded.ದತ್ತಾಂಶ);

    len = ngx_max(ngx_base64_decoded_ಉದ್ದಳತೆ(encoded.len), 20);

    decoded.ದತ್ತಾಂಶ = ngx_pnalloc(pool, len);
    ಆದರೆ (decoded.ದತ್ತಾಂಶ == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    rc = ngx_decode_base64(&decoded, &encoded);

    ಆದರೆ (rc != NGX_OK || decoded.len < 20) {
        decoded.len = 20;
    }

    /* update SHA1 from key and salt */

    ngx_sha1_init(&sha1);
    ngx_sha1_update(&sha1, ಕೀಲಿ, ngx_strlen(ಕೀಲಿ));
    ngx_sha1_update(&sha1, decoded.ದತ್ತಾಂಶ + 20, decoded.len - 20);
    ngx_sha1_final(decoded.ದತ್ತಾಂಶ, &sha1);

    /* encode it back to base64 */

    len = ಗಾತ್ರ("{SSHA}") - 1 + ngx_base64_encoded_ಉದ್ದಳತೆ(decoded.len) + 1;

    *encrypted = ngx_pnalloc(pool, len);
    ಆದರೆ (*encrypted == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    encoded.ದತ್ತಾಂಶ = ngx_cpymem(*encrypted, "{SSHA}", ಗಾತ್ರ("{SSHA}") - 1);
    ngx_encode_base64(&encoded, &decoded);
    encoded.ದತ್ತಾಂಶ[encoded.len] = '\0';

    ಹಿಂತಿರುಗಿ NGX_OK;
}


ಸ್ಥಿರ ngx_int_t
ngx_crypt_sha(ngx_pool_t *pool, u_char *ಕೀಲಿ, u_char *salt, u_char **encrypted)
{
    ಗಾತ್ರ_t      len;
    ngx_str_t   encoded, decoded;
    ngx_sha1_t  sha1;
    u_char      digest[20];

    /* "{SHA}" base64(SHA1(key)) */

    decoded.len = ಗಾತ್ರ(digest);
    decoded.ದತ್ತಾಂಶ = digest;

    ngx_sha1_init(&sha1);
    ngx_sha1_update(&sha1, ಕೀಲಿ, ngx_strlen(ಕೀಲಿ));
    ngx_sha1_final(digest, &sha1);

    len = ಗಾತ್ರ("{SHA}") - 1 + ngx_base64_encoded_ಉದ್ದಳತೆ(decoded.len) + 1;

    *encrypted = ngx_pnalloc(pool, len);
    ಆದರೆ (*encrypted == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    encoded.ದತ್ತಾಂಶ = ngx_cpymem(*encrypted, "{SHA}", ಗಾತ್ರ("{SHA}") - 1);
    ngx_encode_base64(&encoded, &decoded);
    encoded.ದತ್ತಾಂಶ[encoded.len] = '\0';

    ಹಿಂತಿರುಗಿ NGX_OK;
}

#ಆದರೆ_ಅಂತ್ಯ /* NGX_CRYPT */
