
/*
 * Copyright (C) Maxim Dounin
 */


#ଅନ୍ତର୍ଭାଵ <ngx_config.h>
#ଅନ୍ତର୍ଭାଵ <ngx_core.h>
#ଅନ୍ତର୍ଭାଵ <ngx_crypt.h>
#ଅନ୍ତର୍ଭାଵ <ngx_md5.h>
#ଅନ୍ତର୍ଭାଵ <ngx_sha1.h>


#ଯଦି (NGX_CRYPT)

ସ୍ଥିର ngx_int_t ngx_crypt_apr1(ngx_pool_t *pool, u_char *ଚାବି, u_char *salt,
    u_char **encrypted);
ସ୍ଥିର ngx_int_t ngx_crypt_plain(ngx_pool_t *pool, u_char *ଚାବି, u_char *salt,
    u_char **encrypted);
ସ୍ଥିର ngx_int_t ngx_crypt_ssha(ngx_pool_t *pool, u_char *ଚାବି, u_char *salt,
    u_char **encrypted);
ସ୍ଥିର ngx_int_t ngx_crypt_sha(ngx_pool_t *pool, u_char *ଚାବି, u_char *salt,
    u_char **encrypted);


ସ୍ଥିର u_char *ngx_crypt_to64(u_char *p, uint32_t v, size_t n);


ngx_int_t
ngx_crypt(ngx_pool_t *pool, u_char *ଚାବି, u_char *salt, u_char **encrypted)
{
    ଯଦି (ngx_strncmp(salt, "$apr1$", ଆକାର("$apr1$") - 1) == 0) {
        ଫେରନ୍ତୁ ngx_crypt_apr1(pool, ଚାବି, salt, encrypted);

    } ଅନ୍ୟଥା ଯଦି (ngx_strncmp(salt, "{PLAIN}", ଆକାର("{PLAIN}") - 1) == 0) {
        ଫେରନ୍ତୁ ngx_crypt_plain(pool, ଚାବି, salt, encrypted);

    } ଅନ୍ୟଥା ଯଦି (ngx_strncmp(salt, "{SSHA}", ଆକାର("{SSHA}") - 1) == 0) {
        ଫେରନ୍ତୁ ngx_crypt_ssha(pool, ଚାବି, salt, encrypted);

    } ଅନ୍ୟଥା ଯଦି (ngx_strncmp(salt, "{SHA}", ଆକାର("{SHA}") - 1) == 0) {
        ଫେରନ୍ତୁ ngx_crypt_sha(pool, ଚାବି, salt, encrypted);
    }

    /* fallback to libc crypt() */

    ଫେରନ୍ତୁ ngx_libc_crypt(pool, ଚାବି, salt, encrypted);
}


ସ୍ଥିର ngx_int_t
ngx_crypt_apr1(ngx_pool_t *pool, u_char *ଚାବି, u_char *salt, u_char **encrypted)
{
    ngx_int_t          n;
    ngx_uint_t         i;
    u_char            *p, *last, final[16];
    size_t             saltlen, keylen;
    ngx_md5_t          md5, ctx1;

    /* Apache's apr1 crypt is Poul-Henning Kamp's md5 crypt with $apr1$ magic */

    keylen = ngx_strlen(ଚାବି);

    /* true salt: no magic, max 8 chars, stop at first $ */

    salt += ଆକାର("$apr1$") - 1;
    last = salt + 8;
    ପାଇଁ (p = salt; *p && *p != '$' && p < last; p++) { /* void */ }
    saltlen = p - salt;

    /* hash key and salt */

    ngx_md5_init(&md5);
    ngx_md5_update(&md5, ଚାବି, keylen);
    ngx_md5_update(&md5, (u_char *) "$apr1$", ଆକାର("$apr1$") - 1);
    ngx_md5_update(&md5, salt, saltlen);

    ngx_md5_init(&ctx1);
    ngx_md5_update(&ctx1, ଚାବି, keylen);
    ngx_md5_update(&ctx1, salt, saltlen);
    ngx_md5_update(&ctx1, ଚାବି, keylen);
    ngx_md5_final(final, &ctx1);

    ପାଇଁ (n = keylen; n > 0; n -= 16) {
        ngx_md5_update(&md5, final, n > 16 ? 16 : n);
    }

    ngx_memzero(final, ଆକାର(final));

    ପାଇଁ (i = keylen; i; i >>= 1) {
        ଯଦି (i & 1) {
            ngx_md5_update(&md5, final, 1);

        } ଅନ୍ୟଥା {
            ngx_md5_update(&md5, ଚାବି, 1);
        }
    }

    ngx_md5_final(final, &md5);

    ପାଇଁ (i = 0; i < 1000; i++) {
        ngx_md5_init(&ctx1);

        ଯଦି (i & 1) {
            ngx_md5_update(&ctx1, ଚାବି, keylen);

        } ଅନ୍ୟଥା {
            ngx_md5_update(&ctx1, final, 16);
        }

        ଯଦି (i % 3) {
            ngx_md5_update(&ctx1, salt, saltlen);
        }

        ଯଦି (i % 7) {
            ngx_md5_update(&ctx1, ଚାବି, keylen);
        }

        ଯଦି (i & 1) {
            ngx_md5_update(&ctx1, final, 16);

        } ଅନ୍ୟଥା {
            ngx_md5_update(&ctx1, ଚାବି, keylen);
        }

        ngx_md5_final(final, &ctx1);
    }

    /* output */

    *encrypted = ngx_pnalloc(pool, ଆକାର("$apr1$") - 1 + saltlen + 1 + 22 + 1);
    ଯଦି (*encrypted == ଖାଲି) {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    p = ngx_cpymem(*encrypted, "$apr1$", ଆକାର("$apr1$") - 1);
    p = ngx_copy(p, salt, saltlen);
    *p++ = '$';

    p = ngx_crypt_to64(p, (final[ 0]<<16) | (final[ 6]<<8) | final[12], 4);
    p = ngx_crypt_to64(p, (final[ 1]<<16) | (final[ 7]<<8) | final[13], 4);
    p = ngx_crypt_to64(p, (final[ 2]<<16) | (final[ 8]<<8) | final[14], 4);
    p = ngx_crypt_to64(p, (final[ 3]<<16) | (final[ 9]<<8) | final[15], 4);
    p = ngx_crypt_to64(p, (final[ 4]<<16) | (final[10]<<8) | final[ 5], 4);
    p = ngx_crypt_to64(p, final[11], 2);
    *p = '\0';

    ଫେରନ୍ତୁ NGX_OK;
}


ସ୍ଥିର u_char *
ngx_crypt_to64(u_char *p, uint32_t v, size_t n)
{
    ସ୍ଥିର u_char   itoa64[] =
        "./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    ଯେତେବେଳେ (n--) {
        *p++ = itoa64[v & 0x3f];
        v >>= 6;
    }

    ଫେରନ୍ତୁ p;
}


ସ୍ଥିର ngx_int_t
ngx_crypt_plain(ngx_pool_t *pool, u_char *ଚାବି, u_char *salt, u_char **encrypted)
{
    size_t   len;
    u_char  *p;

    len = ngx_strlen(ଚାବି);

    *encrypted = ngx_pnalloc(pool, ଆକାର("{PLAIN}") - 1 + len + 1);
    ଯଦି (*encrypted == ଖାଲି) {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    p = ngx_cpymem(*encrypted, "{PLAIN}", ଆକାର("{PLAIN}") - 1);
    ngx_memcpy(p, ଚାବି, len + 1);

    ଫେରନ୍ତୁ NGX_OK;
}


ସ୍ଥିର ngx_int_t
ngx_crypt_ssha(ngx_pool_t *pool, u_char *ଚାବି, u_char *salt, u_char **encrypted)
{
    size_t       len;
    ngx_int_t    rc;
    ngx_str_t    encoded, decoded;
    ngx_sha1_t   sha1;

    /* "{SSHA}" base64(SHA1(key salt) salt) */

    /* decode base64 salt to find out true salt */

    encoded.data = salt + ଆକାର("{SSHA}") - 1;
    encoded.len = ngx_strlen(encoded.data);

    len = ngx_max(ngx_base64_decoded_length(encoded.len), 20);

    decoded.data = ngx_pnalloc(pool, len);
    ଯଦି (decoded.data == ଖାଲି) {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    rc = ngx_decode_base64(&decoded, &encoded);

    ଯଦି (rc != NGX_OK || decoded.len < 20) {
        decoded.len = 20;
    }

    /* update SHA1 from key and salt */

    ngx_sha1_init(&sha1);
    ngx_sha1_update(&sha1, ଚାବି, ngx_strlen(ଚାବି));
    ngx_sha1_update(&sha1, decoded.data + 20, decoded.len - 20);
    ngx_sha1_final(decoded.data, &sha1);

    /* encode it back to base64 */

    len = ଆକାର("{SSHA}") - 1 + ngx_base64_encoded_length(decoded.len) + 1;

    *encrypted = ngx_pnalloc(pool, len);
    ଯଦି (*encrypted == ଖାଲି) {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    encoded.data = ngx_cpymem(*encrypted, "{SSHA}", ଆକାର("{SSHA}") - 1);
    ngx_encode_base64(&encoded, &decoded);
    encoded.data[encoded.len] = '\0';

    ଫେରନ୍ତୁ NGX_OK;
}


ସ୍ଥିର ngx_int_t
ngx_crypt_sha(ngx_pool_t *pool, u_char *ଚାବି, u_char *salt, u_char **encrypted)
{
    size_t      len;
    ngx_str_t   encoded, decoded;
    ngx_sha1_t  sha1;
    u_char      digest[20];

    /* "{SHA}" base64(SHA1(key)) */

    decoded.len = ଆକାର(digest);
    decoded.data = digest;

    ngx_sha1_init(&sha1);
    ngx_sha1_update(&sha1, ଚାବି, ngx_strlen(ଚାବି));
    ngx_sha1_final(digest, &sha1);

    len = ଆକାର("{SHA}") - 1 + ngx_base64_encoded_length(decoded.len) + 1;

    *encrypted = ngx_pnalloc(pool, len);
    ଯଦି (*encrypted == ଖାଲି) {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    encoded.data = ngx_cpymem(*encrypted, "{SHA}", ଆକାର("{SHA}") - 1);
    ngx_encode_base64(&encoded, &decoded);
    encoded.data[encoded.len] = '\0';

    ଫେରନ୍ତୁ NGX_OK;
}

#endif /* NGX_CRYPT */
