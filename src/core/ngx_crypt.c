
/*
 * Copyright (C) Maxim Dounin
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>
#अन्तर्भाव <ngx_crypt.h>
#अन्तर्भाव <ngx_md5.h>
#अन्तर्भाव <ngx_sha1.h>


#सचे (NGX_CRYPT)

स्थिर ngx_int_t ngx_crypt_apr1(ngx_pool_t *pool, u_char *कुञ्चिका, u_char *salt,
    u_char **encrypted);
स्थिर ngx_int_t ngx_crypt_plain(ngx_pool_t *pool, u_char *कुञ्चिका, u_char *salt,
    u_char **encrypted);
स्थिर ngx_int_t ngx_crypt_ssha(ngx_pool_t *pool, u_char *कुञ्चिका, u_char *salt,
    u_char **encrypted);
स्थिर ngx_int_t ngx_crypt_sha(ngx_pool_t *pool, u_char *कुञ्चिका, u_char *salt,
    u_char **encrypted);


स्थिर u_char *ngx_crypt_to64(u_char *p, uint32_t v, size_t n);


ngx_int_t
ngx_crypt(ngx_pool_t *pool, u_char *कुञ्चिका, u_char *salt, u_char **encrypted)
{
    सचे (ngx_strncmp(salt, "$apr1$", परिमाण("$apr1$") - 1) == 0) {
        पटिनिवत्त ngx_crypt_apr1(pool, कुञ्चिका, salt, encrypted);

    } अञ्ञथा सचे (ngx_strncmp(salt, "{PLAIN}", परिमाण("{PLAIN}") - 1) == 0) {
        पटिनिवत्त ngx_crypt_plain(pool, कुञ्चिका, salt, encrypted);

    } अञ्ञथा सचे (ngx_strncmp(salt, "{SSHA}", परिमाण("{SSHA}") - 1) == 0) {
        पटिनिवत्त ngx_crypt_ssha(pool, कुञ्चिका, salt, encrypted);

    } अञ्ञथा सचे (ngx_strncmp(salt, "{SHA}", परिमाण("{SHA}") - 1) == 0) {
        पटिनिवत्त ngx_crypt_sha(pool, कुञ्चिका, salt, encrypted);
    }

    /* fallback to libc crypt() */

    पटिनिवत्त ngx_libc_crypt(pool, कुञ्चिका, salt, encrypted);
}


स्थिर ngx_int_t
ngx_crypt_apr1(ngx_pool_t *pool, u_char *कुञ्चिका, u_char *salt, u_char **encrypted)
{
    ngx_int_t          n;
    ngx_uint_t         i;
    u_char            *p, *last, final[16];
    size_t             saltlen, keylen;
    ngx_md5_t          md5, ctx1;

    /* Apache's apr1 crypt is Poul-Henning Kamp's md5 crypt with $apr1$ magic */

    keylen = ngx_strlen(कुञ्चिका);

    /* true salt: no magic, max 8 chars, stop at first $ */

    salt += परिमाण("$apr1$") - 1;
    last = salt + 8;
    अत्थाय (p = salt; *p && *p != '$' && p < last; p++) { /* void */ }
    saltlen = p - salt;

    /* hash key and salt */

    ngx_md5_init(&md5);
    ngx_md5_update(&md5, कुञ्चिका, keylen);
    ngx_md5_update(&md5, (u_char *) "$apr1$", परिमाण("$apr1$") - 1);
    ngx_md5_update(&md5, salt, saltlen);

    ngx_md5_init(&ctx1);
    ngx_md5_update(&ctx1, कुञ्चिका, keylen);
    ngx_md5_update(&ctx1, salt, saltlen);
    ngx_md5_update(&ctx1, कुञ्चिका, keylen);
    ngx_md5_final(final, &ctx1);

    अत्थाय (n = keylen; n > 0; n -= 16) {
        ngx_md5_update(&md5, final, n > 16 ? 16 : n);
    }

    ngx_memzero(final, परिमाण(final));

    अत्थाय (i = keylen; i; i >>= 1) {
        सचे (i & 1) {
            ngx_md5_update(&md5, final, 1);

        } अञ्ञथा {
            ngx_md5_update(&md5, कुञ्चिका, 1);
        }
    }

    ngx_md5_final(final, &md5);

    अत्थाय (i = 0; i < 1000; i++) {
        ngx_md5_init(&ctx1);

        सचे (i & 1) {
            ngx_md5_update(&ctx1, कुञ्चिका, keylen);

        } अञ्ञथा {
            ngx_md5_update(&ctx1, final, 16);
        }

        सचे (i % 3) {
            ngx_md5_update(&ctx1, salt, saltlen);
        }

        सचे (i % 7) {
            ngx_md5_update(&ctx1, कुञ्चिका, keylen);
        }

        सचे (i & 1) {
            ngx_md5_update(&ctx1, final, 16);

        } अञ्ञथा {
            ngx_md5_update(&ctx1, कुञ्चिका, keylen);
        }

        ngx_md5_final(final, &ctx1);
    }

    /* output */

    *encrypted = ngx_pnalloc(pool, परिमाण("$apr1$") - 1 + saltlen + 1 + 22 + 1);
    सचे (*encrypted == रिक्त) {
        पटिनिवत्त NGX_ERROR;
    }

    p = ngx_cpymem(*encrypted, "$apr1$", परिमाण("$apr1$") - 1);
    p = ngx_copy(p, salt, saltlen);
    *p++ = '$';

    p = ngx_crypt_to64(p, (final[ 0]<<16) | (final[ 6]<<8) | final[12], 4);
    p = ngx_crypt_to64(p, (final[ 1]<<16) | (final[ 7]<<8) | final[13], 4);
    p = ngx_crypt_to64(p, (final[ 2]<<16) | (final[ 8]<<8) | final[14], 4);
    p = ngx_crypt_to64(p, (final[ 3]<<16) | (final[ 9]<<8) | final[15], 4);
    p = ngx_crypt_to64(p, (final[ 4]<<16) | (final[10]<<8) | final[ 5], 4);
    p = ngx_crypt_to64(p, final[11], 2);
    *p = '\0';

    पटिनिवत्त NGX_OK;
}


स्थिर u_char *
ngx_crypt_to64(u_char *p, uint32_t v, size_t n)
{
    स्थिर u_char   itoa64[] =
        "./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    याव (n--) {
        *p++ = itoa64[v & 0x3f];
        v >>= 6;
    }

    पटिनिवत्त p;
}


स्थिर ngx_int_t
ngx_crypt_plain(ngx_pool_t *pool, u_char *कुञ्चिका, u_char *salt, u_char **encrypted)
{
    size_t   len;
    u_char  *p;

    len = ngx_strlen(कुञ्चिका);

    *encrypted = ngx_pnalloc(pool, परिमाण("{PLAIN}") - 1 + len + 1);
    सचे (*encrypted == रिक्त) {
        पटिनिवत्त NGX_ERROR;
    }

    p = ngx_cpymem(*encrypted, "{PLAIN}", परिमाण("{PLAIN}") - 1);
    ngx_memcpy(p, कुञ्चिका, len + 1);

    पटिनिवत्त NGX_OK;
}


स्थिर ngx_int_t
ngx_crypt_ssha(ngx_pool_t *pool, u_char *कुञ्चिका, u_char *salt, u_char **encrypted)
{
    size_t       len;
    ngx_int_t    rc;
    ngx_str_t    encoded, decoded;
    ngx_sha1_t   sha1;

    /* "{SSHA}" base64(SHA1(key salt) salt) */

    /* decode base64 salt to find out true salt */

    encoded.data = salt + परिमाण("{SSHA}") - 1;
    encoded.len = ngx_strlen(encoded.data);

    len = ngx_max(ngx_base64_decoded_length(encoded.len), 20);

    decoded.data = ngx_pnalloc(pool, len);
    सचे (decoded.data == रिक्त) {
        पटिनिवत्त NGX_ERROR;
    }

    rc = ngx_decode_base64(&decoded, &encoded);

    सचे (rc != NGX_OK || decoded.len < 20) {
        decoded.len = 20;
    }

    /* update SHA1 from key and salt */

    ngx_sha1_init(&sha1);
    ngx_sha1_update(&sha1, कुञ्चिका, ngx_strlen(कुञ्चिका));
    ngx_sha1_update(&sha1, decoded.data + 20, decoded.len - 20);
    ngx_sha1_final(decoded.data, &sha1);

    /* encode it back to base64 */

    len = परिमाण("{SSHA}") - 1 + ngx_base64_encoded_length(decoded.len) + 1;

    *encrypted = ngx_pnalloc(pool, len);
    सचे (*encrypted == रिक्त) {
        पटिनिवत्त NGX_ERROR;
    }

    encoded.data = ngx_cpymem(*encrypted, "{SSHA}", परिमाण("{SSHA}") - 1);
    ngx_encode_base64(&encoded, &decoded);
    encoded.data[encoded.len] = '\0';

    पटिनिवत्त NGX_OK;
}


स्थिर ngx_int_t
ngx_crypt_sha(ngx_pool_t *pool, u_char *कुञ्चिका, u_char *salt, u_char **encrypted)
{
    size_t      len;
    ngx_str_t   encoded, decoded;
    ngx_sha1_t  sha1;
    u_char      digest[20];

    /* "{SHA}" base64(SHA1(key)) */

    decoded.len = परिमाण(digest);
    decoded.data = digest;

    ngx_sha1_init(&sha1);
    ngx_sha1_update(&sha1, कुञ्चिका, ngx_strlen(कुञ्चिका));
    ngx_sha1_final(digest, &sha1);

    len = परिमाण("{SHA}") - 1 + ngx_base64_encoded_length(decoded.len) + 1;

    *encrypted = ngx_pnalloc(pool, len);
    सचे (*encrypted == रिक्त) {
        पटिनिवत्त NGX_ERROR;
    }

    encoded.data = ngx_cpymem(*encrypted, "{SHA}", परिमाण("{SHA}") - 1);
    ngx_encode_base64(&encoded, &decoded);
    encoded.data[encoded.len] = '\0';

    पटिनिवत्त NGX_OK;
}

#endif /* NGX_CRYPT */
