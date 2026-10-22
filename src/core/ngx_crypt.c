
/*
 * Copyright (C) Maxim Dounin
 */


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_crypt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_md5.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_sha1.h>


#ਜੇ (NGX_CRYPT)

ਸਥਿਰ ngx_int_t ngx_crypt_apr1(ngx_pool_t *pool, u_char *ਕੁੰਜੀ, u_char *salt,
    u_char **encrypted);
ਸਥਿਰ ngx_int_t ngx_crypt_plain(ngx_pool_t *pool, u_char *ਕੁੰਜੀ, u_char *salt,
    u_char **encrypted);
ਸਥਿਰ ngx_int_t ngx_crypt_ssha(ngx_pool_t *pool, u_char *ਕੁੰਜੀ, u_char *salt,
    u_char **encrypted);
ਸਥਿਰ ngx_int_t ngx_crypt_sha(ngx_pool_t *pool, u_char *ਕੁੰਜੀ, u_char *salt,
    u_char **encrypted);


ਸਥਿਰ u_char *ngx_crypt_to64(u_char *p, uint32_t v, ਮਾਪ_t n);


ngx_int_t
ngx_crypt(ngx_pool_t *pool, u_char *ਕੁੰਜੀ, u_char *salt, u_char **encrypted)
{
    ਜੇ (ngx_strncmp(salt, "$apr1$", ਆਕਾਰ("$apr1$") - 1) == 0) {
        ਵਾਪਸ ngx_crypt_apr1(pool, ਕੁੰਜੀ, salt, encrypted);

    } ਨਹੀਂਤਾਂ ਜੇ (ngx_strncmp(salt, "{PLAIN}", ਆਕਾਰ("{PLAIN}") - 1) == 0) {
        ਵਾਪਸ ngx_crypt_plain(pool, ਕੁੰਜੀ, salt, encrypted);

    } ਨਹੀਂਤਾਂ ਜੇ (ngx_strncmp(salt, "{SSHA}", ਆਕਾਰ("{SSHA}") - 1) == 0) {
        ਵਾਪਸ ngx_crypt_ssha(pool, ਕੁੰਜੀ, salt, encrypted);

    } ਨਹੀਂਤਾਂ ਜੇ (ngx_strncmp(salt, "{SHA}", ਆਕਾਰ("{SHA}") - 1) == 0) {
        ਵਾਪਸ ngx_crypt_sha(pool, ਕੁੰਜੀ, salt, encrypted);
    }

    /* fallback to libc crypt() */

    ਵਾਪਸ ngx_libc_crypt(pool, ਕੁੰਜੀ, salt, encrypted);
}


ਸਥਿਰ ngx_int_t
ngx_crypt_apr1(ngx_pool_t *pool, u_char *ਕੁੰਜੀ, u_char *salt, u_char **encrypted)
{
    ngx_int_t          n;
    ngx_uint_t         i;
    u_char            *p, *ਆਖਰੀ, final[16];
    ਮਾਪ_t             saltlen, keylen;
    ngx_md5_t          md5, ctx1;

    /* Apache's apr1 crypt is Poul-Henning Kamp's md5 crypt with $apr1$ magic */

    keylen = ngx_strlen(ਕੁੰਜੀ);

    /* true salt: no magic, max 8 chars, stop at first $ */

    salt += ਆਕਾਰ("$apr1$") - 1;
    ਆਖਰੀ = salt + 8;
    ਲਈ (p = salt; *p && *p != '$' && p < ਆਖਰੀ; p++) { /* void */ }
    saltlen = p - salt;

    /* hash key and salt */

    ngx_md5_init(&md5);
    ngx_md5_update(&md5, ਕੁੰਜੀ, keylen);
    ngx_md5_update(&md5, (u_char *) "$apr1$", ਆਕਾਰ("$apr1$") - 1);
    ngx_md5_update(&md5, salt, saltlen);

    ngx_md5_init(&ctx1);
    ngx_md5_update(&ctx1, ਕੁੰਜੀ, keylen);
    ngx_md5_update(&ctx1, salt, saltlen);
    ngx_md5_update(&ctx1, ਕੁੰਜੀ, keylen);
    ngx_md5_final(final, &ctx1);

    ਲਈ (n = keylen; n > 0; n -= 16) {
        ngx_md5_update(&md5, final, n > 16 ? 16 : n);
    }

    ngx_memzero(final, ਆਕਾਰ(final));

    ਲਈ (i = keylen; i; i >>= 1) {
        ਜੇ (i & 1) {
            ngx_md5_update(&md5, final, 1);

        } ਨਹੀਂਤਾਂ {
            ngx_md5_update(&md5, ਕੁੰਜੀ, 1);
        }
    }

    ngx_md5_final(final, &md5);

    ਲਈ (i = 0; i < 1000; i++) {
        ngx_md5_init(&ctx1);

        ਜੇ (i & 1) {
            ngx_md5_update(&ctx1, ਕੁੰਜੀ, keylen);

        } ਨਹੀਂਤਾਂ {
            ngx_md5_update(&ctx1, final, 16);
        }

        ਜੇ (i % 3) {
            ngx_md5_update(&ctx1, salt, saltlen);
        }

        ਜੇ (i % 7) {
            ngx_md5_update(&ctx1, ਕੁੰਜੀ, keylen);
        }

        ਜੇ (i & 1) {
            ngx_md5_update(&ctx1, final, 16);

        } ਨਹੀਂਤਾਂ {
            ngx_md5_update(&ctx1, ਕੁੰਜੀ, keylen);
        }

        ngx_md5_final(final, &ctx1);
    }

    /* output */

    *encrypted = ngx_pnalloc(pool, ਆਕਾਰ("$apr1$") - 1 + saltlen + 1 + 22 + 1);
    ਜੇ (*encrypted == ਨਿਰਾ) {
        ਵਾਪਸ NGX_ERROR;
    }

    p = ngx_cpymem(*encrypted, "$apr1$", ਆਕਾਰ("$apr1$") - 1);
    p = ngx_copy(p, salt, saltlen);
    *p++ = '$';

    p = ngx_crypt_to64(p, (final[ 0]<<16) | (final[ 6]<<8) | final[12], 4);
    p = ngx_crypt_to64(p, (final[ 1]<<16) | (final[ 7]<<8) | final[13], 4);
    p = ngx_crypt_to64(p, (final[ 2]<<16) | (final[ 8]<<8) | final[14], 4);
    p = ngx_crypt_to64(p, (final[ 3]<<16) | (final[ 9]<<8) | final[15], 4);
    p = ngx_crypt_to64(p, (final[ 4]<<16) | (final[10]<<8) | final[ 5], 4);
    p = ngx_crypt_to64(p, final[11], 2);
    *p = '\0';

    ਵਾਪਸ NGX_OK;
}


ਸਥਿਰ u_char *
ngx_crypt_to64(u_char *p, uint32_t v, ਮਾਪ_t n)
{
    ਸਥਿਰ u_char   itoa64[] =
        "./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    ਜਦੋਂ (n--) {
        *p++ = itoa64[v & 0x3f];
        v >>= 6;
    }

    ਵਾਪਸ p;
}


ਸਥਿਰ ngx_int_t
ngx_crypt_plain(ngx_pool_t *pool, u_char *ਕੁੰਜੀ, u_char *salt, u_char **encrypted)
{
    ਮਾਪ_t   len;
    u_char  *p;

    len = ngx_strlen(ਕੁੰਜੀ);

    *encrypted = ngx_pnalloc(pool, ਆਕਾਰ("{PLAIN}") - 1 + len + 1);
    ਜੇ (*encrypted == ਨਿਰਾ) {
        ਵਾਪਸ NGX_ERROR;
    }

    p = ngx_cpymem(*encrypted, "{PLAIN}", ਆਕਾਰ("{PLAIN}") - 1);
    ngx_memcpy(p, ਕੁੰਜੀ, len + 1);

    ਵਾਪਸ NGX_OK;
}


ਸਥਿਰ ngx_int_t
ngx_crypt_ssha(ngx_pool_t *pool, u_char *ਕੁੰਜੀ, u_char *salt, u_char **encrypted)
{
    ਮਾਪ_t       len;
    ngx_int_t    rc;
    ngx_str_t    encoded, decoded;
    ngx_sha1_t   sha1;

    /* "{SSHA}" base64(SHA1(key salt) salt) */

    /* decode base64 salt to find out true salt */

    encoded.ਅੰਕੜੇ = salt + ਆਕਾਰ("{SSHA}") - 1;
    encoded.len = ngx_strlen(encoded.ਅੰਕੜੇ);

    len = ngx_max(ngx_base64_decoded_ਲੰਮਾਈ(encoded.len), 20);

    decoded.ਅੰਕੜੇ = ngx_pnalloc(pool, len);
    ਜੇ (decoded.ਅੰਕੜੇ == ਨਿਰਾ) {
        ਵਾਪਸ NGX_ERROR;
    }

    rc = ngx_decode_base64(&decoded, &encoded);

    ਜੇ (rc != NGX_OK || decoded.len < 20) {
        decoded.len = 20;
    }

    /* update SHA1 from key and salt */

    ngx_sha1_init(&sha1);
    ngx_sha1_update(&sha1, ਕੁੰਜੀ, ngx_strlen(ਕੁੰਜੀ));
    ngx_sha1_update(&sha1, decoded.ਅੰਕੜੇ + 20, decoded.len - 20);
    ngx_sha1_final(decoded.ਅੰਕੜੇ, &sha1);

    /* encode it back to base64 */

    len = ਆਕਾਰ("{SSHA}") - 1 + ngx_base64_encoded_ਲੰਮਾਈ(decoded.len) + 1;

    *encrypted = ngx_pnalloc(pool, len);
    ਜੇ (*encrypted == ਨਿਰਾ) {
        ਵਾਪਸ NGX_ERROR;
    }

    encoded.ਅੰਕੜੇ = ngx_cpymem(*encrypted, "{SSHA}", ਆਕਾਰ("{SSHA}") - 1);
    ngx_encode_base64(&encoded, &decoded);
    encoded.ਅੰਕੜੇ[encoded.len] = '\0';

    ਵਾਪਸ NGX_OK;
}


ਸਥਿਰ ngx_int_t
ngx_crypt_sha(ngx_pool_t *pool, u_char *ਕੁੰਜੀ, u_char *salt, u_char **encrypted)
{
    ਮਾਪ_t      len;
    ngx_str_t   encoded, decoded;
    ngx_sha1_t  sha1;
    u_char      digest[20];

    /* "{SHA}" base64(SHA1(key)) */

    decoded.len = ਆਕਾਰ(digest);
    decoded.ਅੰਕੜੇ = digest;

    ngx_sha1_init(&sha1);
    ngx_sha1_update(&sha1, ਕੁੰਜੀ, ngx_strlen(ਕੁੰਜੀ));
    ngx_sha1_final(digest, &sha1);

    len = ਆਕਾਰ("{SHA}") - 1 + ngx_base64_encoded_ਲੰਮਾਈ(decoded.len) + 1;

    *encrypted = ngx_pnalloc(pool, len);
    ਜੇ (*encrypted == ਨਿਰਾ) {
        ਵਾਪਸ NGX_ERROR;
    }

    encoded.ਅੰਕੜੇ = ngx_cpymem(*encrypted, "{SHA}", ਆਕਾਰ("{SHA}") - 1);
    ngx_encode_base64(&encoded, &decoded);
    encoded.ਅੰਕੜੇ[encoded.len] = '\0';

    ਵਾਪਸ NGX_OK;
}

#ਜੇ_ਅੰਤ /* NGX_CRYPT */
