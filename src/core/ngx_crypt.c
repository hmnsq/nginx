
/*
 * Copyright (C) Maxim Dounin
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>
#ضمن <ngx_crypt.h>
#ضمن <ngx_md5.h>
#ضمن <ngx_sha1.h>


#אם (NGX_CRYPT)

ساكن ngx_int_t ngx_crypt_apr1(ngx_pool_t *pool, u_char *key, u_char *salt,
    u_char **encrypted);
ساكن ngx_int_t ngx_crypt_plain(ngx_pool_t *pool, u_char *key, u_char *salt,
    u_char **encrypted);
ساكن ngx_int_t ngx_crypt_ssha(ngx_pool_t *pool, u_char *key, u_char *salt,
    u_char **encrypted);
ساكن ngx_int_t ngx_crypt_sha(ngx_pool_t *pool, u_char *key, u_char *salt,
    u_char **encrypted);


ساكن u_char *ngx_crypt_to64(u_char *p, uint32_t v, size_t n);


ngx_int_t
ngx_crypt(ngx_pool_t *pool, u_char *key, u_char *salt, u_char **encrypted)
{
    אם (ngx_strncmp(salt, "$apr1$", حجم("$apr1$") - 1) == 0) {
        החזר ngx_crypt_apr1(pool, key, salt, encrypted);

    } אחרת אם (ngx_strncmp(salt, "{PLAIN}", حجم("{PLAIN}") - 1) == 0) {
        החזר ngx_crypt_plain(pool, key, salt, encrypted);

    } אחרת אם (ngx_strncmp(salt, "{SSHA}", حجم("{SSHA}") - 1) == 0) {
        החזר ngx_crypt_ssha(pool, key, salt, encrypted);

    } אחרת אם (ngx_strncmp(salt, "{SHA}", حجم("{SHA}") - 1) == 0) {
        החזר ngx_crypt_sha(pool, key, salt, encrypted);
    }

    /* fallback to libc crypt() */

    החזר ngx_libc_crypt(pool, key, salt, encrypted);
}


ساكن ngx_int_t
ngx_crypt_apr1(ngx_pool_t *pool, u_char *key, u_char *salt, u_char **encrypted)
{
    ngx_int_t          n;
    ngx_uint_t         i;
    u_char            *p, *last, final[16];
    size_t             saltlen, keylen;
    ngx_md5_t          md5, ctx1;

    /* Apache's apr1 crypt is Poul-Henning Kamp's md5 crypt with $apr1$ magic */

    keylen = ngx_strlen(key);

    /* true salt: no magic, max 8 chars, stop at first $ */

    salt += حجم("$apr1$") - 1;
    last = salt + 8;
    עבור (p = salt; *p && *p != '$' && p < last; p++) { /* void */ }
    saltlen = p - salt;

    /* hash key and salt */

    ngx_md5_init(&md5);
    ngx_md5_update(&md5, key, keylen);
    ngx_md5_update(&md5, (u_char *) "$apr1$", حجم("$apr1$") - 1);
    ngx_md5_update(&md5, salt, saltlen);

    ngx_md5_init(&ctx1);
    ngx_md5_update(&ctx1, key, keylen);
    ngx_md5_update(&ctx1, salt, saltlen);
    ngx_md5_update(&ctx1, key, keylen);
    ngx_md5_final(final, &ctx1);

    עבור (n = keylen; n > 0; n -= 16) {
        ngx_md5_update(&md5, final, n > 16 ? 16 : n);
    }

    ngx_memzero(final, حجم(final));

    עבור (i = keylen; i; i >>= 1) {
        אם (i & 1) {
            ngx_md5_update(&md5, final, 1);

        } אחרת {
            ngx_md5_update(&md5, key, 1);
        }
    }

    ngx_md5_final(final, &md5);

    עבור (i = 0; i < 1000; i++) {
        ngx_md5_init(&ctx1);

        אם (i & 1) {
            ngx_md5_update(&ctx1, key, keylen);

        } אחרת {
            ngx_md5_update(&ctx1, final, 16);
        }

        אם (i % 3) {
            ngx_md5_update(&ctx1, salt, saltlen);
        }

        אם (i % 7) {
            ngx_md5_update(&ctx1, key, keylen);
        }

        אם (i & 1) {
            ngx_md5_update(&ctx1, final, 16);

        } אחרת {
            ngx_md5_update(&ctx1, key, keylen);
        }

        ngx_md5_final(final, &ctx1);
    }

    /* output */

    *encrypted = ngx_pnalloc(pool, حجم("$apr1$") - 1 + saltlen + 1 + 22 + 1);
    אם (*encrypted == عدم) {
        החזר NGX_ERROR;
    }

    p = ngx_cpymem(*encrypted, "$apr1$", حجم("$apr1$") - 1);
    p = ngx_copy(p, salt, saltlen);
    *p++ = '$';

    p = ngx_crypt_to64(p, (final[ 0]<<16) | (final[ 6]<<8) | final[12], 4);
    p = ngx_crypt_to64(p, (final[ 1]<<16) | (final[ 7]<<8) | final[13], 4);
    p = ngx_crypt_to64(p, (final[ 2]<<16) | (final[ 8]<<8) | final[14], 4);
    p = ngx_crypt_to64(p, (final[ 3]<<16) | (final[ 9]<<8) | final[15], 4);
    p = ngx_crypt_to64(p, (final[ 4]<<16) | (final[10]<<8) | final[ 5], 4);
    p = ngx_crypt_to64(p, final[11], 2);
    *p = '\0';

    החזר NGX_OK;
}


ساكن u_char *
ngx_crypt_to64(u_char *p, uint32_t v, size_t n)
{
    ساكن u_char   itoa64[] =
        "./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    כלעוד (n--) {
        *p++ = itoa64[v & 0x3f];
        v >>= 6;
    }

    החזר p;
}


ساكن ngx_int_t
ngx_crypt_plain(ngx_pool_t *pool, u_char *key, u_char *salt, u_char **encrypted)
{
    size_t   len;
    u_char  *p;

    len = ngx_strlen(key);

    *encrypted = ngx_pnalloc(pool, حجم("{PLAIN}") - 1 + len + 1);
    אם (*encrypted == عدم) {
        החזר NGX_ERROR;
    }

    p = ngx_cpymem(*encrypted, "{PLAIN}", حجم("{PLAIN}") - 1);
    ngx_memcpy(p, key, len + 1);

    החזר NGX_OK;
}


ساكن ngx_int_t
ngx_crypt_ssha(ngx_pool_t *pool, u_char *key, u_char *salt, u_char **encrypted)
{
    size_t       len;
    ngx_int_t    rc;
    ngx_str_t    encoded, decoded;
    ngx_sha1_t   sha1;

    /* "{SSHA}" base64(SHA1(key salt) salt) */

    /* decode base64 salt to find out true salt */

    encoded.data = salt + حجم("{SSHA}") - 1;
    encoded.len = ngx_strlen(encoded.data);

    len = ngx_max(ngx_base64_decoded_length(encoded.len), 20);

    decoded.data = ngx_pnalloc(pool, len);
    אם (decoded.data == عدم) {
        החזר NGX_ERROR;
    }

    rc = ngx_decode_base64(&decoded, &encoded);

    אם (rc != NGX_OK || decoded.len < 20) {
        decoded.len = 20;
    }

    /* update SHA1 from key and salt */

    ngx_sha1_init(&sha1);
    ngx_sha1_update(&sha1, key, ngx_strlen(key));
    ngx_sha1_update(&sha1, decoded.data + 20, decoded.len - 20);
    ngx_sha1_final(decoded.data, &sha1);

    /* encode it back to base64 */

    len = حجم("{SSHA}") - 1 + ngx_base64_encoded_length(decoded.len) + 1;

    *encrypted = ngx_pnalloc(pool, len);
    אם (*encrypted == عدم) {
        החזר NGX_ERROR;
    }

    encoded.data = ngx_cpymem(*encrypted, "{SSHA}", حجم("{SSHA}") - 1);
    ngx_encode_base64(&encoded, &decoded);
    encoded.data[encoded.len] = '\0';

    החזר NGX_OK;
}


ساكن ngx_int_t
ngx_crypt_sha(ngx_pool_t *pool, u_char *key, u_char *salt, u_char **encrypted)
{
    size_t      len;
    ngx_str_t   encoded, decoded;
    ngx_sha1_t  sha1;
    u_char      digest[20];

    /* "{SHA}" base64(SHA1(key)) */

    decoded.len = حجم(digest);
    decoded.data = digest;

    ngx_sha1_init(&sha1);
    ngx_sha1_update(&sha1, key, ngx_strlen(key));
    ngx_sha1_final(digest, &sha1);

    len = حجم("{SHA}") - 1 + ngx_base64_encoded_length(decoded.len) + 1;

    *encrypted = ngx_pnalloc(pool, len);
    אם (*encrypted == عدم) {
        החזר NGX_ERROR;
    }

    encoded.data = ngx_cpymem(*encrypted, "{SHA}", حجم("{SHA}") - 1);
    ngx_encode_base64(&encoded, &decoded);
    encoded.data[encoded.len] = '\0';

    החזר NGX_OK;
}

#endif /* NGX_CRYPT */
