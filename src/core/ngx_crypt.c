
/*
 * Copyright (C) Maxim Dounin
 */


#അന്തര്ഭാവ <ngx_config.h>
#അന്തര്ഭാവ <ngx_core.h>
#അന്തര്ഭാവ <ngx_crypt.h>
#അന്തര്ഭാവ <ngx_md5.h>
#അന്തര്ഭാവ <ngx_sha1.h>


#എങ്കിൽ (NGX_CRYPT)

സ്ഥിതം ngx_int_t ngx_crypt_apr1(ngx_pool_t *pool, u_char *താക്കോൽ, u_char *salt,
    u_char **encrypted);
സ്ഥിതം ngx_int_t ngx_crypt_plain(ngx_pool_t *pool, u_char *താക്കോൽ, u_char *salt,
    u_char **encrypted);
സ്ഥിതം ngx_int_t ngx_crypt_ssha(ngx_pool_t *pool, u_char *താക്കോൽ, u_char *salt,
    u_char **encrypted);
സ്ഥിതം ngx_int_t ngx_crypt_sha(ngx_pool_t *pool, u_char *താക്കോൽ, u_char *salt,
    u_char **encrypted);


സ്ഥിതം u_char *ngx_crypt_to64(u_char *p, uint32_t v, size_t n);


ngx_int_t
ngx_crypt(ngx_pool_t *pool, u_char *താക്കോൽ, u_char *salt, u_char **encrypted)
{
    എങ്കിൽ (ngx_strncmp(salt, "$apr1$", വലുപ്പം("$apr1$") - 1) == 0) {
        തിരികെ ngx_crypt_apr1(pool, താക്കോൽ, salt, encrypted);

    } അല്ലെങ്കിൽ എങ്കിൽ (ngx_strncmp(salt, "{PLAIN}", വലുപ്പം("{PLAIN}") - 1) == 0) {
        തിരികെ ngx_crypt_plain(pool, താക്കോൽ, salt, encrypted);

    } അല്ലെങ്കിൽ എങ്കിൽ (ngx_strncmp(salt, "{SSHA}", വലുപ്പം("{SSHA}") - 1) == 0) {
        തിരികെ ngx_crypt_ssha(pool, താക്കോൽ, salt, encrypted);

    } അല്ലെങ്കിൽ എങ്കിൽ (ngx_strncmp(salt, "{SHA}", വലുപ്പം("{SHA}") - 1) == 0) {
        തിരികെ ngx_crypt_sha(pool, താക്കോൽ, salt, encrypted);
    }

    /* fallback to libc crypt() */

    തിരികെ ngx_libc_crypt(pool, താക്കോൽ, salt, encrypted);
}


സ്ഥിതം ngx_int_t
ngx_crypt_apr1(ngx_pool_t *pool, u_char *താക്കോൽ, u_char *salt, u_char **encrypted)
{
    ngx_int_t          n;
    ngx_uint_t         i;
    u_char            *p, *last, final[16];
    size_t             saltlen, keylen;
    ngx_md5_t          md5, ctx1;

    /* Apache's apr1 crypt is Poul-Henning Kamp's md5 crypt with $apr1$ magic */

    keylen = ngx_strlen(താക്കോൽ);

    /* true salt: no magic, max 8 chars, stop at first $ */

    salt += വലുപ്പം("$apr1$") - 1;
    last = salt + 8;
    വേണ്ടി (p = salt; *p && *p != '$' && p < last; p++) { /* void */ }
    saltlen = p - salt;

    /* hash key and salt */

    ngx_md5_init(&md5);
    ngx_md5_update(&md5, താക്കോൽ, keylen);
    ngx_md5_update(&md5, (u_char *) "$apr1$", വലുപ്പം("$apr1$") - 1);
    ngx_md5_update(&md5, salt, saltlen);

    ngx_md5_init(&ctx1);
    ngx_md5_update(&ctx1, താക്കോൽ, keylen);
    ngx_md5_update(&ctx1, salt, saltlen);
    ngx_md5_update(&ctx1, താക്കോൽ, keylen);
    ngx_md5_final(final, &ctx1);

    വേണ്ടി (n = keylen; n > 0; n -= 16) {
        ngx_md5_update(&md5, final, n > 16 ? 16 : n);
    }

    ngx_memzero(final, വലുപ്പം(final));

    വേണ്ടി (i = keylen; i; i >>= 1) {
        എങ്കിൽ (i & 1) {
            ngx_md5_update(&md5, final, 1);

        } അല്ലെങ്കിൽ {
            ngx_md5_update(&md5, താക്കോൽ, 1);
        }
    }

    ngx_md5_final(final, &md5);

    വേണ്ടി (i = 0; i < 1000; i++) {
        ngx_md5_init(&ctx1);

        എങ്കിൽ (i & 1) {
            ngx_md5_update(&ctx1, താക്കോൽ, keylen);

        } അല്ലെങ്കിൽ {
            ngx_md5_update(&ctx1, final, 16);
        }

        എങ്കിൽ (i % 3) {
            ngx_md5_update(&ctx1, salt, saltlen);
        }

        എങ്കിൽ (i % 7) {
            ngx_md5_update(&ctx1, താക്കോൽ, keylen);
        }

        എങ്കിൽ (i & 1) {
            ngx_md5_update(&ctx1, final, 16);

        } അല്ലെങ്കിൽ {
            ngx_md5_update(&ctx1, താക്കോൽ, keylen);
        }

        ngx_md5_final(final, &ctx1);
    }

    /* output */

    *encrypted = ngx_pnalloc(pool, വലുപ്പം("$apr1$") - 1 + saltlen + 1 + 22 + 1);
    എങ്കിൽ (*encrypted == ശൂന്യ) {
        തിരികെ NGX_ERROR;
    }

    p = ngx_cpymem(*encrypted, "$apr1$", വലുപ്പം("$apr1$") - 1);
    p = ngx_copy(p, salt, saltlen);
    *p++ = '$';

    p = ngx_crypt_to64(p, (final[ 0]<<16) | (final[ 6]<<8) | final[12], 4);
    p = ngx_crypt_to64(p, (final[ 1]<<16) | (final[ 7]<<8) | final[13], 4);
    p = ngx_crypt_to64(p, (final[ 2]<<16) | (final[ 8]<<8) | final[14], 4);
    p = ngx_crypt_to64(p, (final[ 3]<<16) | (final[ 9]<<8) | final[15], 4);
    p = ngx_crypt_to64(p, (final[ 4]<<16) | (final[10]<<8) | final[ 5], 4);
    p = ngx_crypt_to64(p, final[11], 2);
    *p = '\0';

    തിരികെ NGX_OK;
}


സ്ഥിതം u_char *
ngx_crypt_to64(u_char *p, uint32_t v, size_t n)
{
    സ്ഥിതം u_char   itoa64[] =
        "./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    ആകുമ്പോൾ (n--) {
        *p++ = itoa64[v & 0x3f];
        v >>= 6;
    }

    തിരികെ p;
}


സ്ഥിതം ngx_int_t
ngx_crypt_plain(ngx_pool_t *pool, u_char *താക്കോൽ, u_char *salt, u_char **encrypted)
{
    size_t   len;
    u_char  *p;

    len = ngx_strlen(താക്കോൽ);

    *encrypted = ngx_pnalloc(pool, വലുപ്പം("{PLAIN}") - 1 + len + 1);
    എങ്കിൽ (*encrypted == ശൂന്യ) {
        തിരികെ NGX_ERROR;
    }

    p = ngx_cpymem(*encrypted, "{PLAIN}", വലുപ്പം("{PLAIN}") - 1);
    ngx_memcpy(p, താക്കോൽ, len + 1);

    തിരികെ NGX_OK;
}


സ്ഥിതം ngx_int_t
ngx_crypt_ssha(ngx_pool_t *pool, u_char *താക്കോൽ, u_char *salt, u_char **encrypted)
{
    size_t       len;
    ngx_int_t    rc;
    ngx_str_t    encoded, decoded;
    ngx_sha1_t   sha1;

    /* "{SSHA}" base64(SHA1(key salt) salt) */

    /* decode base64 salt to find out true salt */

    encoded.data = salt + വലുപ്പം("{SSHA}") - 1;
    encoded.len = ngx_strlen(encoded.data);

    len = ngx_max(ngx_base64_decoded_length(encoded.len), 20);

    decoded.data = ngx_pnalloc(pool, len);
    എങ്കിൽ (decoded.data == ശൂന്യ) {
        തിരികെ NGX_ERROR;
    }

    rc = ngx_decode_base64(&decoded, &encoded);

    എങ്കിൽ (rc != NGX_OK || decoded.len < 20) {
        decoded.len = 20;
    }

    /* update SHA1 from key and salt */

    ngx_sha1_init(&sha1);
    ngx_sha1_update(&sha1, താക്കോൽ, ngx_strlen(താക്കോൽ));
    ngx_sha1_update(&sha1, decoded.data + 20, decoded.len - 20);
    ngx_sha1_final(decoded.data, &sha1);

    /* encode it back to base64 */

    len = വലുപ്പം("{SSHA}") - 1 + ngx_base64_encoded_length(decoded.len) + 1;

    *encrypted = ngx_pnalloc(pool, len);
    എങ്കിൽ (*encrypted == ശൂന്യ) {
        തിരികെ NGX_ERROR;
    }

    encoded.data = ngx_cpymem(*encrypted, "{SSHA}", വലുപ്പം("{SSHA}") - 1);
    ngx_encode_base64(&encoded, &decoded);
    encoded.data[encoded.len] = '\0';

    തിരികെ NGX_OK;
}


സ്ഥിതം ngx_int_t
ngx_crypt_sha(ngx_pool_t *pool, u_char *താക്കോൽ, u_char *salt, u_char **encrypted)
{
    size_t      len;
    ngx_str_t   encoded, decoded;
    ngx_sha1_t  sha1;
    u_char      digest[20];

    /* "{SHA}" base64(SHA1(key)) */

    decoded.len = വലുപ്പം(digest);
    decoded.data = digest;

    ngx_sha1_init(&sha1);
    ngx_sha1_update(&sha1, താക്കോൽ, ngx_strlen(താക്കോൽ));
    ngx_sha1_final(digest, &sha1);

    len = വലുപ്പം("{SHA}") - 1 + ngx_base64_encoded_length(decoded.len) + 1;

    *encrypted = ngx_pnalloc(pool, len);
    എങ്കിൽ (*encrypted == ശൂന്യ) {
        തിരികെ NGX_ERROR;
    }

    encoded.data = ngx_cpymem(*encrypted, "{SHA}", വലുപ്പം("{SHA}") - 1);
    ngx_encode_base64(&encoded, &decoded);
    encoded.data[encoded.len] = '\0';

    തിരികെ NGX_OK;
}

#endif /* NGX_CRYPT */
