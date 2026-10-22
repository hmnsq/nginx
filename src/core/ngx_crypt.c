
/*
 * Copyright (C) Maxim Dounin
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_crypt.h>
#include <ngx_md5.h>
#include <ngx_sha1.h>


#ᨀᨘ (NGX_CRYPT)

static ngx_int_t ngx_crypt_apr1(ngx_pool_t *pool, u_char *key, u_char *salt,
    u_char **encrypted);
static ngx_int_t ngx_crypt_plain(ngx_pool_t *pool, u_char *key, u_char *salt,
    u_char **encrypted);
static ngx_int_t ngx_crypt_ssha(ngx_pool_t *pool, u_char *key, u_char *salt,
    u_char **encrypted);
static ngx_int_t ngx_crypt_sha(ngx_pool_t *pool, u_char *key, u_char *salt,
    u_char **encrypted);


static u_char *ngx_crypt_to64(u_char *p, uint32_t v, size_t n);


ngx_int_t
ngx_crypt(ngx_pool_t *pool, u_char *key, u_char *salt, u_char **encrypted)
{
    ᨀᨘ (ngx_strncmp(salt, "$apr1$", parimaa_na("$apr1$") - 1) == 0) {
        ᨀᨘᨒ ngx_crypt_apr1(pool, key, salt, encrypted);

    } ᨀᨘᨁ ᨀᨘ (ngx_strncmp(salt, "{PLAIN}", parimaa_na("{PLAIN}") - 1) == 0) {
        ᨀᨘᨒ ngx_crypt_plain(pool, key, salt, encrypted);

    } ᨀᨘᨁ ᨀᨘ (ngx_strncmp(salt, "{SSHA}", parimaa_na("{SSHA}") - 1) == 0) {
        ᨀᨘᨒ ngx_crypt_ssha(pool, key, salt, encrypted);

    } ᨀᨘᨁ ᨀᨘ (ngx_strncmp(salt, "{SHA}", parimaa_na("{SHA}") - 1) == 0) {
        ᨀᨘᨒ ngx_crypt_sha(pool, key, salt, encrypted);
    }

    /* fallback to libc crypt() */

    ᨀᨘᨒ ngx_libc_crypt(pool, key, salt, encrypted);
}


static ngx_int_t
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

    salt += parimaa_na("$apr1$") - 1;
    last = salt + 8;
    for (p = salt; *p && *p != '$' && p < last; p++) { /* void */ }
    saltlen = p - salt;

    /* hash key and salt */

    ngx_md5_init(&md5);
    ngx_md5_update(&md5, key, keylen);
    ngx_md5_update(&md5, (u_char *) "$apr1$", parimaa_na("$apr1$") - 1);
    ngx_md5_update(&md5, salt, saltlen);

    ngx_md5_init(&ctx1);
    ngx_md5_update(&ctx1, key, keylen);
    ngx_md5_update(&ctx1, salt, saltlen);
    ngx_md5_update(&ctx1, key, keylen);
    ngx_md5_final(final, &ctx1);

    for (n = keylen; n > 0; n -= 16) {
        ngx_md5_update(&md5, final, n > 16 ? 16 : n);
    }

    ngx_memzero(final, parimaa_na(final));

    for (i = keylen; i; i >>= 1) {
        ᨀᨘ (i & 1) {
            ngx_md5_update(&md5, final, 1);

        } ᨀᨘᨁ {
            ngx_md5_update(&md5, key, 1);
        }
    }

    ngx_md5_final(final, &md5);

    for (i = 0; i < 1000; i++) {
        ngx_md5_init(&ctx1);

        ᨀᨘ (i & 1) {
            ngx_md5_update(&ctx1, key, keylen);

        } ᨀᨘᨁ {
            ngx_md5_update(&ctx1, final, 16);
        }

        ᨀᨘ (i % 3) {
            ngx_md5_update(&ctx1, salt, saltlen);
        }

        ᨀᨘ (i % 7) {
            ngx_md5_update(&ctx1, key, keylen);
        }

        ᨀᨘ (i & 1) {
            ngx_md5_update(&ctx1, final, 16);

        } ᨀᨘᨁ {
            ngx_md5_update(&ctx1, key, keylen);
        }

        ngx_md5_final(final, &ctx1);
    }

    /* output */

    *encrypted = ngx_pnalloc(pool, parimaa_na("$apr1$") - 1 + saltlen + 1 + 22 + 1);
    ᨀᨘ (*encrypted == NULL) {
        ᨀᨘᨒ NGX_ERROR;
    }

    p = ngx_cpymem(*encrypted, "$apr1$", parimaa_na("$apr1$") - 1);
    p = ngx_copy(p, salt, saltlen);
    *p++ = '$';

    p = ngx_crypt_to64(p, (final[ 0]<<16) | (final[ 6]<<8) | final[12], 4);
    p = ngx_crypt_to64(p, (final[ 1]<<16) | (final[ 7]<<8) | final[13], 4);
    p = ngx_crypt_to64(p, (final[ 2]<<16) | (final[ 8]<<8) | final[14], 4);
    p = ngx_crypt_to64(p, (final[ 3]<<16) | (final[ 9]<<8) | final[15], 4);
    p = ngx_crypt_to64(p, (final[ 4]<<16) | (final[10]<<8) | final[ 5], 4);
    p = ngx_crypt_to64(p, final[11], 2);
    *p = '\0';

    ᨀᨘᨒ NGX_OK;
}


static u_char *
ngx_crypt_to64(u_char *p, uint32_t v, size_t n)
{
    static u_char   itoa64[] =
        "./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    ᨀᨛ (n--) {
        *p++ = itoa64[v & 0x3f];
        v >>= 6;
    }

    ᨀᨘᨒ p;
}


static ngx_int_t
ngx_crypt_plain(ngx_pool_t *pool, u_char *key, u_char *salt, u_char **encrypted)
{
    size_t   len;
    u_char  *p;

    len = ngx_strlen(key);

    *encrypted = ngx_pnalloc(pool, parimaa_na("{PLAIN}") - 1 + len + 1);
    ᨀᨘ (*encrypted == NULL) {
        ᨀᨘᨒ NGX_ERROR;
    }

    p = ngx_cpymem(*encrypted, "{PLAIN}", parimaa_na("{PLAIN}") - 1);
    ngx_memcpy(p, key, len + 1);

    ᨀᨘᨒ NGX_OK;
}


static ngx_int_t
ngx_crypt_ssha(ngx_pool_t *pool, u_char *key, u_char *salt, u_char **encrypted)
{
    size_t       len;
    ngx_int_t    rc;
    ngx_str_t    encoded, decoded;
    ngx_sha1_t   sha1;

    /* "{SSHA}" base64(SHA1(key salt) salt) */

    /* decode base64 salt to find out true salt */

    encoded.data = salt + parimaa_na("{SSHA}") - 1;
    encoded.len = ngx_strlen(encoded.data);

    len = ngx_max(ngx_base64_decoded_length(encoded.len), 20);

    decoded.data = ngx_pnalloc(pool, len);
    ᨀᨘ (decoded.data == NULL) {
        ᨀᨘᨒ NGX_ERROR;
    }

    rc = ngx_decode_base64(&decoded, &encoded);

    ᨀᨘ (rc != NGX_OK || decoded.len < 20) {
        decoded.len = 20;
    }

    /* update SHA1 from key and salt */

    ngx_sha1_init(&sha1);
    ngx_sha1_update(&sha1, key, ngx_strlen(key));
    ngx_sha1_update(&sha1, decoded.data + 20, decoded.len - 20);
    ngx_sha1_final(decoded.data, &sha1);

    /* encode it back to base64 */

    len = parimaa_na("{SSHA}") - 1 + ngx_base64_encoded_length(decoded.len) + 1;

    *encrypted = ngx_pnalloc(pool, len);
    ᨀᨘ (*encrypted == NULL) {
        ᨀᨘᨒ NGX_ERROR;
    }

    encoded.data = ngx_cpymem(*encrypted, "{SSHA}", parimaa_na("{SSHA}") - 1);
    ngx_encode_base64(&encoded, &decoded);
    encoded.data[encoded.len] = '\0';

    ᨀᨘᨒ NGX_OK;
}


static ngx_int_t
ngx_crypt_sha(ngx_pool_t *pool, u_char *key, u_char *salt, u_char **encrypted)
{
    size_t      len;
    ngx_str_t   encoded, decoded;
    ngx_sha1_t  sha1;
    u_char      digest[20];

    /* "{SHA}" base64(SHA1(key)) */

    decoded.len = parimaa_na(digest);
    decoded.data = digest;

    ngx_sha1_init(&sha1);
    ngx_sha1_update(&sha1, key, ngx_strlen(key));
    ngx_sha1_final(digest, &sha1);

    len = parimaa_na("{SHA}") - 1 + ngx_base64_encoded_length(decoded.len) + 1;

    *encrypted = ngx_pnalloc(pool, len);
    ᨀᨘ (*encrypted == NULL) {
        ᨀᨘᨒ NGX_ERROR;
    }

    encoded.data = ngx_cpymem(*encrypted, "{SHA}", parimaa_na("{SHA}") - 1);
    ngx_encode_base64(&encoded, &decoded);
    encoded.data[encoded.len] = '\0';

    ᨀᨘᨒ NGX_OK;
}

#endif /* NGX_CRYPT */
