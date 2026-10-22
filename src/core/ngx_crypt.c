
/*
 * Copyright (C) Maxim Dounin
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>
#ಅನ್ತರ್ಭಾವ <ngx_crypt.h>
#ಅನ್ತರ್ಭಾವ <ngx_md5.h>
#ಅನ್ತರ್ಭಾವ <ngx_sha1.h>


#ಯದಿ (NGX_CRYPT)

ಸ್ಥಿರ ngx_int_t ngx_crypt_apr1(ngx_pool_t *pool, u_char *ಕೀಲಿ, u_char *salt,
    u_char **encrypted);
ಸ್ಥಿರ ngx_int_t ngx_crypt_plain(ngx_pool_t *pool, u_char *ಕೀಲಿ, u_char *salt,
    u_char **encrypted);
ಸ್ಥಿರ ngx_int_t ngx_crypt_ssha(ngx_pool_t *pool, u_char *ಕೀಲಿ, u_char *salt,
    u_char **encrypted);
ಸ್ಥಿರ ngx_int_t ngx_crypt_sha(ngx_pool_t *pool, u_char *ಕೀಲಿ, u_char *salt,
    u_char **encrypted);


ಸ್ಥಿರ u_char *ngx_crypt_to64(u_char *p, uint32_t v, size_t n);


ngx_int_t
ngx_crypt(ngx_pool_t *pool, u_char *ಕೀಲಿ, u_char *salt, u_char **encrypted)
{
    ಯದಿ (ngx_strncmp(salt, "$apr1$", ಪರಿಮಾಣ("$apr1$") - 1) == 0) {
        ಪ್ರತ್ಯಾಗಮ ngx_crypt_apr1(pool, ಕೀಲಿ, salt, encrypted);

    } ಅನ್ಯಥಾ ಯದಿ (ngx_strncmp(salt, "{PLAIN}", ಪರಿಮಾಣ("{PLAIN}") - 1) == 0) {
        ಪ್ರತ್ಯಾಗಮ ngx_crypt_plain(pool, ಕೀಲಿ, salt, encrypted);

    } ಅನ್ಯಥಾ ಯದಿ (ngx_strncmp(salt, "{SSHA}", ಪರಿಮಾಣ("{SSHA}") - 1) == 0) {
        ಪ್ರತ್ಯಾಗಮ ngx_crypt_ssha(pool, ಕೀಲಿ, salt, encrypted);

    } ಅನ್ಯಥಾ ಯದಿ (ngx_strncmp(salt, "{SHA}", ಪರಿಮಾಣ("{SHA}") - 1) == 0) {
        ಪ್ರತ್ಯಾಗಮ ngx_crypt_sha(pool, ಕೀಲಿ, salt, encrypted);
    }

    /* fallback to libc crypt() */

    ಪ್ರತ್ಯಾಗಮ ngx_libc_crypt(pool, ಕೀಲಿ, salt, encrypted);
}


ಸ್ಥಿರ ngx_int_t
ngx_crypt_apr1(ngx_pool_t *pool, u_char *ಕೀಲಿ, u_char *salt, u_char **encrypted)
{
    ngx_int_t          n;
    ngx_uint_t         i;
    u_char            *p, *last, final[16];
    size_t             saltlen, keylen;
    ngx_md5_t          md5, ctx1;

    /* Apache's apr1 crypt is Poul-Henning Kamp's md5 crypt with $apr1$ magic */

    keylen = ngx_strlen(ಕೀಲಿ);

    /* true salt: no magic, max 8 chars, stop at first $ */

    salt += ಪರಿಮಾಣ("$apr1$") - 1;
    last = salt + 8;
    ಪ್ರತಿ (p = salt; *p && *p != '$' && p < last; p++) { /* void */ }
    saltlen = p - salt;

    /* hash key and salt */

    ngx_md5_init(&md5);
    ngx_md5_update(&md5, ಕೀಲಿ, keylen);
    ngx_md5_update(&md5, (u_char *) "$apr1$", ಪರಿಮಾಣ("$apr1$") - 1);
    ngx_md5_update(&md5, salt, saltlen);

    ngx_md5_init(&ctx1);
    ngx_md5_update(&ctx1, ಕೀಲಿ, keylen);
    ngx_md5_update(&ctx1, salt, saltlen);
    ngx_md5_update(&ctx1, ಕೀಲಿ, keylen);
    ngx_md5_final(final, &ctx1);

    ಪ್ರತಿ (n = keylen; n > 0; n -= 16) {
        ngx_md5_update(&md5, final, n > 16 ? 16 : n);
    }

    ngx_memzero(final, ಪರಿಮಾಣ(final));

    ಪ್ರತಿ (i = keylen; i; i >>= 1) {
        ಯದಿ (i & 1) {
            ngx_md5_update(&md5, final, 1);

        } ಅನ್ಯಥಾ {
            ngx_md5_update(&md5, ಕೀಲಿ, 1);
        }
    }

    ngx_md5_final(final, &md5);

    ಪ್ರತಿ (i = 0; i < 1000; i++) {
        ngx_md5_init(&ctx1);

        ಯದಿ (i & 1) {
            ngx_md5_update(&ctx1, ಕೀಲಿ, keylen);

        } ಅನ್ಯಥಾ {
            ngx_md5_update(&ctx1, final, 16);
        }

        ಯದಿ (i % 3) {
            ngx_md5_update(&ctx1, salt, saltlen);
        }

        ಯದಿ (i % 7) {
            ngx_md5_update(&ctx1, ಕೀಲಿ, keylen);
        }

        ಯದಿ (i & 1) {
            ngx_md5_update(&ctx1, final, 16);

        } ಅನ್ಯಥಾ {
            ngx_md5_update(&ctx1, ಕೀಲಿ, keylen);
        }

        ngx_md5_final(final, &ctx1);
    }

    /* output */

    *encrypted = ngx_pnalloc(pool, ಪರಿಮಾಣ("$apr1$") - 1 + saltlen + 1 + 22 + 1);
    ಯದಿ (*encrypted == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    p = ngx_cpymem(*encrypted, "$apr1$", ಪರಿಮಾಣ("$apr1$") - 1);
    p = ngx_copy(p, salt, saltlen);
    *p++ = '$';

    p = ngx_crypt_to64(p, (final[ 0]<<16) | (final[ 6]<<8) | final[12], 4);
    p = ngx_crypt_to64(p, (final[ 1]<<16) | (final[ 7]<<8) | final[13], 4);
    p = ngx_crypt_to64(p, (final[ 2]<<16) | (final[ 8]<<8) | final[14], 4);
    p = ngx_crypt_to64(p, (final[ 3]<<16) | (final[ 9]<<8) | final[15], 4);
    p = ngx_crypt_to64(p, (final[ 4]<<16) | (final[10]<<8) | final[ 5], 4);
    p = ngx_crypt_to64(p, final[11], 2);
    *p = '\0';

    ಪ್ರತ್ಯಾಗಮ NGX_OK;
}


ಸ್ಥಿರ u_char *
ngx_crypt_to64(u_char *p, uint32_t v, size_t n)
{
    ಸ್ಥಿರ u_char   itoa64[] =
        "./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    ಯಾವತ್ (n--) {
        *p++ = itoa64[v & 0x3f];
        v >>= 6;
    }

    ಪ್ರತ್ಯಾಗಮ p;
}


ಸ್ಥಿರ ngx_int_t
ngx_crypt_plain(ngx_pool_t *pool, u_char *ಕೀಲಿ, u_char *salt, u_char **encrypted)
{
    size_t   len;
    u_char  *p;

    len = ngx_strlen(ಕೀಲಿ);

    *encrypted = ngx_pnalloc(pool, ಪರಿಮಾಣ("{PLAIN}") - 1 + len + 1);
    ಯದಿ (*encrypted == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    p = ngx_cpymem(*encrypted, "{PLAIN}", ಪರಿಮಾಣ("{PLAIN}") - 1);
    ngx_memcpy(p, ಕೀಲಿ, len + 1);

    ಪ್ರತ್ಯಾಗಮ NGX_OK;
}


ಸ್ಥಿರ ngx_int_t
ngx_crypt_ssha(ngx_pool_t *pool, u_char *ಕೀಲಿ, u_char *salt, u_char **encrypted)
{
    size_t       len;
    ngx_int_t    rc;
    ngx_str_t    encoded, decoded;
    ngx_sha1_t   sha1;

    /* "{SSHA}" base64(SHA1(key salt) salt) */

    /* decode base64 salt to find out true salt */

    encoded.data = salt + ಪರಿಮಾಣ("{SSHA}") - 1;
    encoded.len = ngx_strlen(encoded.data);

    len = ngx_max(ngx_base64_decoded_length(encoded.len), 20);

    decoded.data = ngx_pnalloc(pool, len);
    ಯದಿ (decoded.data == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    rc = ngx_decode_base64(&decoded, &encoded);

    ಯದಿ (rc != NGX_OK || decoded.len < 20) {
        decoded.len = 20;
    }

    /* update SHA1 from key and salt */

    ngx_sha1_init(&sha1);
    ngx_sha1_update(&sha1, ಕೀಲಿ, ngx_strlen(ಕೀಲಿ));
    ngx_sha1_update(&sha1, decoded.data + 20, decoded.len - 20);
    ngx_sha1_final(decoded.data, &sha1);

    /* encode it back to base64 */

    len = ಪರಿಮಾಣ("{SSHA}") - 1 + ngx_base64_encoded_length(decoded.len) + 1;

    *encrypted = ngx_pnalloc(pool, len);
    ಯದಿ (*encrypted == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    encoded.data = ngx_cpymem(*encrypted, "{SSHA}", ಪರಿಮಾಣ("{SSHA}") - 1);
    ngx_encode_base64(&encoded, &decoded);
    encoded.data[encoded.len] = '\0';

    ಪ್ರತ್ಯಾಗಮ NGX_OK;
}


ಸ್ಥಿರ ngx_int_t
ngx_crypt_sha(ngx_pool_t *pool, u_char *ಕೀಲಿ, u_char *salt, u_char **encrypted)
{
    size_t      len;
    ngx_str_t   encoded, decoded;
    ngx_sha1_t  sha1;
    u_char      digest[20];

    /* "{SHA}" base64(SHA1(key)) */

    decoded.len = ಪರಿಮಾಣ(digest);
    decoded.data = digest;

    ngx_sha1_init(&sha1);
    ngx_sha1_update(&sha1, ಕೀಲಿ, ngx_strlen(ಕೀಲಿ));
    ngx_sha1_final(digest, &sha1);

    len = ಪರಿಮಾಣ("{SHA}") - 1 + ngx_base64_encoded_length(decoded.len) + 1;

    *encrypted = ngx_pnalloc(pool, len);
    ಯದಿ (*encrypted == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    encoded.data = ngx_cpymem(*encrypted, "{SHA}", ಪರಿಮಾಣ("{SHA}") - 1);
    ngx_encode_base64(&encoded, &decoded);
    encoded.data[encoded.len] = '\0';

    ಪ್ರತ್ಯಾಗಮ NGX_OK;
}

#endif /* NGX_CRYPT */
