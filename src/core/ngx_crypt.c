
/*
 * Copyright (C) Maxim Dounin
 */


#شامل <ngx_config.h>
#شامل <ngx_core.h>
#شامل <ngx_crypt.h>
#شامل <ngx_md5.h>
#شامل <ngx_sha1.h>


#اگر (NGX_CRYPT)

ساکن ngx_int_t ngx_crypt_apr1(ngx_pool_t *pool, u_char *کلید, u_char *salt,
    u_char **encrypted);
ساکن ngx_int_t ngx_crypt_plain(ngx_pool_t *pool, u_char *کلید, u_char *salt,
    u_char **encrypted);
ساکن ngx_int_t ngx_crypt_ssha(ngx_pool_t *pool, u_char *کلید, u_char *salt,
    u_char **encrypted);
ساکن ngx_int_t ngx_crypt_sha(ngx_pool_t *pool, u_char *کلید, u_char *salt,
    u_char **encrypted);


ساکن u_char *ngx_crypt_to64(u_char *p, uint32_t v, سائز_t n);


ngx_int_t
ngx_crypt(ngx_pool_t *pool, u_char *کلید, u_char *salt, u_char **encrypted)
{
    اگر (ngx_strncmp(salt, "$apr1$", حجم("$apr1$") - 1) == 0) {
        واپس ngx_crypt_apr1(pool, کلید, salt, encrypted);

    } ورنہ اگر (ngx_strncmp(salt, "{PLAIN}", حجم("{PLAIN}") - 1) == 0) {
        واپس ngx_crypt_plain(pool, کلید, salt, encrypted);

    } ورنہ اگر (ngx_strncmp(salt, "{SSHA}", حجم("{SSHA}") - 1) == 0) {
        واپس ngx_crypt_ssha(pool, کلید, salt, encrypted);

    } ورنہ اگر (ngx_strncmp(salt, "{SHA}", حجم("{SHA}") - 1) == 0) {
        واپس ngx_crypt_sha(pool, کلید, salt, encrypted);
    }

    /* fallback to libc crypt() */

    واپس ngx_libc_crypt(pool, کلید, salt, encrypted);
}


ساکن ngx_int_t
ngx_crypt_apr1(ngx_pool_t *pool, u_char *کلید, u_char *salt, u_char **encrypted)
{
    ngx_int_t          n;
    ngx_uint_t         i;
    u_char            *p, *آخری, final[16];
    سائز_t             saltlen, keylen;
    ngx_md5_t          md5, ctx1;

    /* Apache's apr1 crypt is Poul-Henning Kamp's md5 crypt with $apr1$ magic */

    keylen = ngx_strlen(کلید);

    /* true salt: no magic, max 8 chars, stop at first $ */

    salt += حجم("$apr1$") - 1;
    آخری = salt + 8;
    برائے (p = salt; *p && *p != '$' && p < آخری; p++) { /* void */ }
    saltlen = p - salt;

    /* hash key and salt */

    ngx_md5_init(&md5);
    ngx_md5_update(&md5, کلید, keylen);
    ngx_md5_update(&md5, (u_char *) "$apr1$", حجم("$apr1$") - 1);
    ngx_md5_update(&md5, salt, saltlen);

    ngx_md5_init(&ctx1);
    ngx_md5_update(&ctx1, کلید, keylen);
    ngx_md5_update(&ctx1, salt, saltlen);
    ngx_md5_update(&ctx1, کلید, keylen);
    ngx_md5_final(final, &ctx1);

    برائے (n = keylen; n > 0; n -= 16) {
        ngx_md5_update(&md5, final, n > 16 ? 16 : n);
    }

    ngx_memzero(final, حجم(final));

    برائے (i = keylen; i; i >>= 1) {
        اگر (i & 1) {
            ngx_md5_update(&md5, final, 1);

        } ورنہ {
            ngx_md5_update(&md5, کلید, 1);
        }
    }

    ngx_md5_final(final, &md5);

    برائے (i = 0; i < 1000; i++) {
        ngx_md5_init(&ctx1);

        اگر (i & 1) {
            ngx_md5_update(&ctx1, کلید, keylen);

        } ورنہ {
            ngx_md5_update(&ctx1, final, 16);
        }

        اگر (i % 3) {
            ngx_md5_update(&ctx1, salt, saltlen);
        }

        اگر (i % 7) {
            ngx_md5_update(&ctx1, کلید, keylen);
        }

        اگر (i & 1) {
            ngx_md5_update(&ctx1, final, 16);

        } ورنہ {
            ngx_md5_update(&ctx1, کلید, keylen);
        }

        ngx_md5_final(final, &ctx1);
    }

    /* output */

    *encrypted = ngx_pnalloc(pool, حجم("$apr1$") - 1 + saltlen + 1 + 22 + 1);
    اگر (*encrypted == تہی) {
        واپس NGX_ERROR;
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

    واپس NGX_OK;
}


ساکن u_char *
ngx_crypt_to64(u_char *p, uint32_t v, سائز_t n)
{
    ساکن u_char   itoa64[] =
        "./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    جب (n--) {
        *p++ = itoa64[v & 0x3f];
        v >>= 6;
    }

    واپس p;
}


ساکن ngx_int_t
ngx_crypt_plain(ngx_pool_t *pool, u_char *کلید, u_char *salt, u_char **encrypted)
{
    سائز_t   len;
    u_char  *p;

    len = ngx_strlen(کلید);

    *encrypted = ngx_pnalloc(pool, حجم("{PLAIN}") - 1 + len + 1);
    اگر (*encrypted == تہی) {
        واپس NGX_ERROR;
    }

    p = ngx_cpymem(*encrypted, "{PLAIN}", حجم("{PLAIN}") - 1);
    ngx_memcpy(p, کلید, len + 1);

    واپس NGX_OK;
}


ساکن ngx_int_t
ngx_crypt_ssha(ngx_pool_t *pool, u_char *کلید, u_char *salt, u_char **encrypted)
{
    سائز_t       len;
    ngx_int_t    rc;
    ngx_str_t    encoded, decoded;
    ngx_sha1_t   sha1;

    /* "{SSHA}" base64(SHA1(key salt) salt) */

    /* decode base64 salt to find out true salt */

    encoded.ڈیٹا = salt + حجم("{SSHA}") - 1;
    encoded.len = ngx_strlen(encoded.ڈیٹا);

    len = ngx_max(ngx_base64_decoded_لمبائی(encoded.len), 20);

    decoded.ڈیٹا = ngx_pnalloc(pool, len);
    اگر (decoded.ڈیٹا == تہی) {
        واپس NGX_ERROR;
    }

    rc = ngx_decode_base64(&decoded, &encoded);

    اگر (rc != NGX_OK || decoded.len < 20) {
        decoded.len = 20;
    }

    /* update SHA1 from key and salt */

    ngx_sha1_init(&sha1);
    ngx_sha1_update(&sha1, کلید, ngx_strlen(کلید));
    ngx_sha1_update(&sha1, decoded.ڈیٹا + 20, decoded.len - 20);
    ngx_sha1_final(decoded.ڈیٹا, &sha1);

    /* encode it back to base64 */

    len = حجم("{SSHA}") - 1 + ngx_base64_encoded_لمبائی(decoded.len) + 1;

    *encrypted = ngx_pnalloc(pool, len);
    اگر (*encrypted == تہی) {
        واپس NGX_ERROR;
    }

    encoded.ڈیٹا = ngx_cpymem(*encrypted, "{SSHA}", حجم("{SSHA}") - 1);
    ngx_encode_base64(&encoded, &decoded);
    encoded.ڈیٹا[encoded.len] = '\0';

    واپس NGX_OK;
}


ساکن ngx_int_t
ngx_crypt_sha(ngx_pool_t *pool, u_char *کلید, u_char *salt, u_char **encrypted)
{
    سائز_t      len;
    ngx_str_t   encoded, decoded;
    ngx_sha1_t  sha1;
    u_char      digest[20];

    /* "{SHA}" base64(SHA1(key)) */

    decoded.len = حجم(digest);
    decoded.ڈیٹا = digest;

    ngx_sha1_init(&sha1);
    ngx_sha1_update(&sha1, کلید, ngx_strlen(کلید));
    ngx_sha1_final(digest, &sha1);

    len = حجم("{SHA}") - 1 + ngx_base64_encoded_لمبائی(decoded.len) + 1;

    *encrypted = ngx_pnalloc(pool, len);
    اگر (*encrypted == تہی) {
        واپس NGX_ERROR;
    }

    encoded.ڈیٹا = ngx_cpymem(*encrypted, "{SHA}", حجم("{SHA}") - 1);
    ngx_encode_base64(&encoded, &decoded);
    encoded.ڈیٹا[encoded.len] = '\0';

    واپس NGX_OK;
}

#اگر_ختم /* NGX_CRYPT */
