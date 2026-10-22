
/*
 * Copyright (C) Maxim Dounin
 */


#অন্তর্ভাব <ngx_config.h>
#অন্তর্ভাব <ngx_core.h>
#অন্তর্ভাব <ngx_crypt.h>
#অন্তর্ভাব <ngx_md5.h>
#অন্তর্ভাব <ngx_sha1.h>


#যদি (NGX_CRYPT)

স্থির ngx_int_t ngx_crypt_apr1(ngx_pool_t *pool, u_char *চাবি, u_char *salt,
    u_char **encrypted);
স্থির ngx_int_t ngx_crypt_plain(ngx_pool_t *pool, u_char *চাবি, u_char *salt,
    u_char **encrypted);
স্থির ngx_int_t ngx_crypt_ssha(ngx_pool_t *pool, u_char *চাবি, u_char *salt,
    u_char **encrypted);
স্থির ngx_int_t ngx_crypt_sha(ngx_pool_t *pool, u_char *চাবি, u_char *salt,
    u_char **encrypted);


স্থির u_char *ngx_crypt_to64(u_char *p, uint32_t v, size_t n);


ngx_int_t
ngx_crypt(ngx_pool_t *pool, u_char *চাবি, u_char *salt, u_char **encrypted)
{
    যদি (ngx_strncmp(salt, "$apr1$", পরিমাণ("$apr1$") - 1) == 0) {
        প্রত্যাগম ngx_crypt_apr1(pool, চাবি, salt, encrypted);

    } অন্যথা যদি (ngx_strncmp(salt, "{PLAIN}", পরিমাণ("{PLAIN}") - 1) == 0) {
        প্রত্যাগম ngx_crypt_plain(pool, চাবি, salt, encrypted);

    } অন্যথা যদি (ngx_strncmp(salt, "{SSHA}", পরিমাণ("{SSHA}") - 1) == 0) {
        প্রত্যাগম ngx_crypt_ssha(pool, চাবি, salt, encrypted);

    } অন্যথা যদি (ngx_strncmp(salt, "{SHA}", পরিমাণ("{SHA}") - 1) == 0) {
        প্রত্যাগম ngx_crypt_sha(pool, চাবি, salt, encrypted);
    }

    /* fallback to libc crypt() */

    প্রত্যাগম ngx_libc_crypt(pool, চাবি, salt, encrypted);
}


স্থির ngx_int_t
ngx_crypt_apr1(ngx_pool_t *pool, u_char *চাবি, u_char *salt, u_char **encrypted)
{
    ngx_int_t          n;
    ngx_uint_t         i;
    u_char            *p, *last, final[16];
    size_t             saltlen, keylen;
    ngx_md5_t          md5, ctx1;

    /* Apache's apr1 crypt is Poul-Henning Kamp's md5 crypt with $apr1$ magic */

    keylen = ngx_strlen(চাবি);

    /* true salt: no magic, max 8 chars, stop at first $ */

    salt += পরিমাণ("$apr1$") - 1;
    last = salt + 8;
    প্রতি (p = salt; *p && *p != '$' && p < last; p++) { /* void */ }
    saltlen = p - salt;

    /* hash key and salt */

    ngx_md5_init(&md5);
    ngx_md5_update(&md5, চাবি, keylen);
    ngx_md5_update(&md5, (u_char *) "$apr1$", পরিমাণ("$apr1$") - 1);
    ngx_md5_update(&md5, salt, saltlen);

    ngx_md5_init(&ctx1);
    ngx_md5_update(&ctx1, চাবি, keylen);
    ngx_md5_update(&ctx1, salt, saltlen);
    ngx_md5_update(&ctx1, চাবি, keylen);
    ngx_md5_final(final, &ctx1);

    প্রতি (n = keylen; n > 0; n -= 16) {
        ngx_md5_update(&md5, final, n > 16 ? 16 : n);
    }

    ngx_memzero(final, পরিমাণ(final));

    প্রতি (i = keylen; i; i >>= 1) {
        যদি (i & 1) {
            ngx_md5_update(&md5, final, 1);

        } অন্যথা {
            ngx_md5_update(&md5, চাবি, 1);
        }
    }

    ngx_md5_final(final, &md5);

    প্রতি (i = 0; i < 1000; i++) {
        ngx_md5_init(&ctx1);

        যদি (i & 1) {
            ngx_md5_update(&ctx1, চাবি, keylen);

        } অন্যথা {
            ngx_md5_update(&ctx1, final, 16);
        }

        যদি (i % 3) {
            ngx_md5_update(&ctx1, salt, saltlen);
        }

        যদি (i % 7) {
            ngx_md5_update(&ctx1, চাবি, keylen);
        }

        যদি (i & 1) {
            ngx_md5_update(&ctx1, final, 16);

        } অন্যথা {
            ngx_md5_update(&ctx1, চাবি, keylen);
        }

        ngx_md5_final(final, &ctx1);
    }

    /* output */

    *encrypted = ngx_pnalloc(pool, পরিমাণ("$apr1$") - 1 + saltlen + 1 + 22 + 1);
    যদি (*encrypted == রিক্ত) {
        প্রত্যাগম NGX_ERROR;
    }

    p = ngx_cpymem(*encrypted, "$apr1$", পরিমাণ("$apr1$") - 1);
    p = ngx_copy(p, salt, saltlen);
    *p++ = '$';

    p = ngx_crypt_to64(p, (final[ 0]<<16) | (final[ 6]<<8) | final[12], 4);
    p = ngx_crypt_to64(p, (final[ 1]<<16) | (final[ 7]<<8) | final[13], 4);
    p = ngx_crypt_to64(p, (final[ 2]<<16) | (final[ 8]<<8) | final[14], 4);
    p = ngx_crypt_to64(p, (final[ 3]<<16) | (final[ 9]<<8) | final[15], 4);
    p = ngx_crypt_to64(p, (final[ 4]<<16) | (final[10]<<8) | final[ 5], 4);
    p = ngx_crypt_to64(p, final[11], 2);
    *p = '\0';

    প্রত্যাগম NGX_OK;
}


স্থির u_char *
ngx_crypt_to64(u_char *p, uint32_t v, size_t n)
{
    স্থির u_char   itoa64[] =
        "./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    যাবত্ (n--) {
        *p++ = itoa64[v & 0x3f];
        v >>= 6;
    }

    প্রত্যাগম p;
}


স্থির ngx_int_t
ngx_crypt_plain(ngx_pool_t *pool, u_char *চাবি, u_char *salt, u_char **encrypted)
{
    size_t   len;
    u_char  *p;

    len = ngx_strlen(চাবি);

    *encrypted = ngx_pnalloc(pool, পরিমাণ("{PLAIN}") - 1 + len + 1);
    যদি (*encrypted == রিক্ত) {
        প্রত্যাগম NGX_ERROR;
    }

    p = ngx_cpymem(*encrypted, "{PLAIN}", পরিমাণ("{PLAIN}") - 1);
    ngx_memcpy(p, চাবি, len + 1);

    প্রত্যাগম NGX_OK;
}


স্থির ngx_int_t
ngx_crypt_ssha(ngx_pool_t *pool, u_char *চাবি, u_char *salt, u_char **encrypted)
{
    size_t       len;
    ngx_int_t    rc;
    ngx_str_t    encoded, decoded;
    ngx_sha1_t   sha1;

    /* "{SSHA}" base64(SHA1(key salt) salt) */

    /* decode base64 salt to find out true salt */

    encoded.data = salt + পরিমাণ("{SSHA}") - 1;
    encoded.len = ngx_strlen(encoded.data);

    len = ngx_max(ngx_base64_decoded_length(encoded.len), 20);

    decoded.data = ngx_pnalloc(pool, len);
    যদি (decoded.data == রিক্ত) {
        প্রত্যাগম NGX_ERROR;
    }

    rc = ngx_decode_base64(&decoded, &encoded);

    যদি (rc != NGX_OK || decoded.len < 20) {
        decoded.len = 20;
    }

    /* update SHA1 from key and salt */

    ngx_sha1_init(&sha1);
    ngx_sha1_update(&sha1, চাবি, ngx_strlen(চাবি));
    ngx_sha1_update(&sha1, decoded.data + 20, decoded.len - 20);
    ngx_sha1_final(decoded.data, &sha1);

    /* encode it back to base64 */

    len = পরিমাণ("{SSHA}") - 1 + ngx_base64_encoded_length(decoded.len) + 1;

    *encrypted = ngx_pnalloc(pool, len);
    যদি (*encrypted == রিক্ত) {
        প্রত্যাগম NGX_ERROR;
    }

    encoded.data = ngx_cpymem(*encrypted, "{SSHA}", পরিমাণ("{SSHA}") - 1);
    ngx_encode_base64(&encoded, &decoded);
    encoded.data[encoded.len] = '\0';

    প্রত্যাগম NGX_OK;
}


স্থির ngx_int_t
ngx_crypt_sha(ngx_pool_t *pool, u_char *চাবি, u_char *salt, u_char **encrypted)
{
    size_t      len;
    ngx_str_t   encoded, decoded;
    ngx_sha1_t  sha1;
    u_char      digest[20];

    /* "{SHA}" base64(SHA1(key)) */

    decoded.len = পরিমাণ(digest);
    decoded.data = digest;

    ngx_sha1_init(&sha1);
    ngx_sha1_update(&sha1, চাবি, ngx_strlen(চাবি));
    ngx_sha1_final(digest, &sha1);

    len = পরিমাণ("{SHA}") - 1 + ngx_base64_encoded_length(decoded.len) + 1;

    *encrypted = ngx_pnalloc(pool, len);
    যদি (*encrypted == রিক্ত) {
        প্রত্যাগম NGX_ERROR;
    }

    encoded.data = ngx_cpymem(*encrypted, "{SHA}", পরিমাণ("{SHA}") - 1);
    ngx_encode_base64(&encoded, &decoded);
    encoded.data[encoded.len] = '\0';

    প্রত্যাগম NGX_OK;
}

#endif /* NGX_CRYPT */
