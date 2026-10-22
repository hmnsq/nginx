
/*
 * Copyright (C) Maxim Dounin
 */


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>
#అన్తర్భావ <ngx_crypt.h>
#అన్తర్భావ <ngx_md5.h>
#అన్తర్భావ <ngx_sha1.h>


#ఒకవేళ (NGX_CRYPT)

స్థిర ngx_int_t ngx_crypt_apr1(ngx_pool_t *pool, u_char *కీలకం, u_char *salt,
    u_char **encrypted);
స్థిర ngx_int_t ngx_crypt_plain(ngx_pool_t *pool, u_char *కీలకం, u_char *salt,
    u_char **encrypted);
స్థిర ngx_int_t ngx_crypt_ssha(ngx_pool_t *pool, u_char *కీలకం, u_char *salt,
    u_char **encrypted);
స్థిర ngx_int_t ngx_crypt_sha(ngx_pool_t *pool, u_char *కీలకం, u_char *salt,
    u_char **encrypted);


స్థిర u_char *ngx_crypt_to64(u_char *p, uint32_t v, కొలత_t n);


ngx_int_t
ngx_crypt(ngx_pool_t *pool, u_char *కీలకం, u_char *salt, u_char **encrypted)
{
    ఒకవేళ (ngx_strncmp(salt, "$apr1$", పరిమాణం("$apr1$") - 1) == 0) {
        తిరిగి ngx_crypt_apr1(pool, కీలకం, salt, encrypted);

    } లేకపోతే ఒకవేళ (ngx_strncmp(salt, "{PLAIN}", పరిమాణం("{PLAIN}") - 1) == 0) {
        తిరిగి ngx_crypt_plain(pool, కీలకం, salt, encrypted);

    } లేకపోతే ఒకవేళ (ngx_strncmp(salt, "{SSHA}", పరిమాణం("{SSHA}") - 1) == 0) {
        తిరిగి ngx_crypt_ssha(pool, కీలకం, salt, encrypted);

    } లేకపోతే ఒకవేళ (ngx_strncmp(salt, "{SHA}", పరిమాణం("{SHA}") - 1) == 0) {
        తిరిగి ngx_crypt_sha(pool, కీలకం, salt, encrypted);
    }

    /* fallback to libc crypt() */

    తిరిగి ngx_libc_crypt(pool, కీలకం, salt, encrypted);
}


స్థిర ngx_int_t
ngx_crypt_apr1(ngx_pool_t *pool, u_char *కీలకం, u_char *salt, u_char **encrypted)
{
    ngx_int_t          n;
    ngx_uint_t         i;
    u_char            *p, *చివరి, final[16];
    కొలత_t             saltlen, keylen;
    ngx_md5_t          md5, ctx1;

    /* Apache's apr1 crypt is Poul-Henning Kamp's md5 crypt with $apr1$ magic */

    keylen = ngx_strlen(కీలకం);

    /* true salt: no magic, max 8 chars, stop at first $ */

    salt += పరిమాణం("$apr1$") - 1;
    చివరి = salt + 8;
    కొరకు (p = salt; *p && *p != '$' && p < చివరి; p++) { /* void */ }
    saltlen = p - salt;

    /* hash key and salt */

    ngx_md5_init(&md5);
    ngx_md5_update(&md5, కీలకం, keylen);
    ngx_md5_update(&md5, (u_char *) "$apr1$", పరిమాణం("$apr1$") - 1);
    ngx_md5_update(&md5, salt, saltlen);

    ngx_md5_init(&ctx1);
    ngx_md5_update(&ctx1, కీలకం, keylen);
    ngx_md5_update(&ctx1, salt, saltlen);
    ngx_md5_update(&ctx1, కీలకం, keylen);
    ngx_md5_final(final, &ctx1);

    కొరకు (n = keylen; n > 0; n -= 16) {
        ngx_md5_update(&md5, final, n > 16 ? 16 : n);
    }

    ngx_memzero(final, పరిమాణం(final));

    కొరకు (i = keylen; i; i >>= 1) {
        ఒకవేళ (i & 1) {
            ngx_md5_update(&md5, final, 1);

        } లేకపోతే {
            ngx_md5_update(&md5, కీలకం, 1);
        }
    }

    ngx_md5_final(final, &md5);

    కొరకు (i = 0; i < 1000; i++) {
        ngx_md5_init(&ctx1);

        ఒకవేళ (i & 1) {
            ngx_md5_update(&ctx1, కీలకం, keylen);

        } లేకపోతే {
            ngx_md5_update(&ctx1, final, 16);
        }

        ఒకవేళ (i % 3) {
            ngx_md5_update(&ctx1, salt, saltlen);
        }

        ఒకవేళ (i % 7) {
            ngx_md5_update(&ctx1, కీలకం, keylen);
        }

        ఒకవేళ (i & 1) {
            ngx_md5_update(&ctx1, final, 16);

        } లేకపోతే {
            ngx_md5_update(&ctx1, కీలకం, keylen);
        }

        ngx_md5_final(final, &ctx1);
    }

    /* output */

    *encrypted = ngx_pnalloc(pool, పరిమాణం("$apr1$") - 1 + saltlen + 1 + 22 + 1);
    ఒకవేళ (*encrypted == ఖాళీ) {
        తిరిగి NGX_ERROR;
    }

    p = ngx_cpymem(*encrypted, "$apr1$", పరిమాణం("$apr1$") - 1);
    p = ngx_copy(p, salt, saltlen);
    *p++ = '$';

    p = ngx_crypt_to64(p, (final[ 0]<<16) | (final[ 6]<<8) | final[12], 4);
    p = ngx_crypt_to64(p, (final[ 1]<<16) | (final[ 7]<<8) | final[13], 4);
    p = ngx_crypt_to64(p, (final[ 2]<<16) | (final[ 8]<<8) | final[14], 4);
    p = ngx_crypt_to64(p, (final[ 3]<<16) | (final[ 9]<<8) | final[15], 4);
    p = ngx_crypt_to64(p, (final[ 4]<<16) | (final[10]<<8) | final[ 5], 4);
    p = ngx_crypt_to64(p, final[11], 2);
    *p = '\0';

    తిరిగి NGX_OK;
}


స్థిర u_char *
ngx_crypt_to64(u_char *p, uint32_t v, కొలత_t n)
{
    స్థిర u_char   itoa64[] =
        "./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    అయితే (n--) {
        *p++ = itoa64[v & 0x3f];
        v >>= 6;
    }

    తిరిగి p;
}


స్థిర ngx_int_t
ngx_crypt_plain(ngx_pool_t *pool, u_char *కీలకం, u_char *salt, u_char **encrypted)
{
    కొలత_t   len;
    u_char  *p;

    len = ngx_strlen(కీలకం);

    *encrypted = ngx_pnalloc(pool, పరిమాణం("{PLAIN}") - 1 + len + 1);
    ఒకవేళ (*encrypted == ఖాళీ) {
        తిరిగి NGX_ERROR;
    }

    p = ngx_cpymem(*encrypted, "{PLAIN}", పరిమాణం("{PLAIN}") - 1);
    ngx_memcpy(p, కీలకం, len + 1);

    తిరిగి NGX_OK;
}


స్థిర ngx_int_t
ngx_crypt_ssha(ngx_pool_t *pool, u_char *కీలకం, u_char *salt, u_char **encrypted)
{
    కొలత_t       len;
    ngx_int_t    rc;
    ngx_str_t    encoded, decoded;
    ngx_sha1_t   sha1;

    /* "{SSHA}" base64(SHA1(key salt) salt) */

    /* decode base64 salt to find out true salt */

    encoded.దత్తాంశం = salt + పరిమాణం("{SSHA}") - 1;
    encoded.len = ngx_strlen(encoded.దత్తాంశం);

    len = ngx_max(ngx_base64_decoded_నిడివి(encoded.len), 20);

    decoded.దత్తాంశం = ngx_pnalloc(pool, len);
    ఒకవేళ (decoded.దత్తాంశం == ఖాళీ) {
        తిరిగి NGX_ERROR;
    }

    rc = ngx_decode_base64(&decoded, &encoded);

    ఒకవేళ (rc != NGX_OK || decoded.len < 20) {
        decoded.len = 20;
    }

    /* update SHA1 from key and salt */

    ngx_sha1_init(&sha1);
    ngx_sha1_update(&sha1, కీలకం, ngx_strlen(కీలకం));
    ngx_sha1_update(&sha1, decoded.దత్తాంశం + 20, decoded.len - 20);
    ngx_sha1_final(decoded.దత్తాంశం, &sha1);

    /* encode it back to base64 */

    len = పరిమాణం("{SSHA}") - 1 + ngx_base64_encoded_నిడివి(decoded.len) + 1;

    *encrypted = ngx_pnalloc(pool, len);
    ఒకవేళ (*encrypted == ఖాళీ) {
        తిరిగి NGX_ERROR;
    }

    encoded.దత్తాంశం = ngx_cpymem(*encrypted, "{SSHA}", పరిమాణం("{SSHA}") - 1);
    ngx_encode_base64(&encoded, &decoded);
    encoded.దత్తాంశం[encoded.len] = '\0';

    తిరిగి NGX_OK;
}


స్థిర ngx_int_t
ngx_crypt_sha(ngx_pool_t *pool, u_char *కీలకం, u_char *salt, u_char **encrypted)
{
    కొలత_t      len;
    ngx_str_t   encoded, decoded;
    ngx_sha1_t  sha1;
    u_char      digest[20];

    /* "{SHA}" base64(SHA1(key)) */

    decoded.len = పరిమాణం(digest);
    decoded.దత్తాంశం = digest;

    ngx_sha1_init(&sha1);
    ngx_sha1_update(&sha1, కీలకం, ngx_strlen(కీలకం));
    ngx_sha1_final(digest, &sha1);

    len = పరిమాణం("{SHA}") - 1 + ngx_base64_encoded_నిడివి(decoded.len) + 1;

    *encrypted = ngx_pnalloc(pool, len);
    ఒకవేళ (*encrypted == ఖాళీ) {
        తిరిగి NGX_ERROR;
    }

    encoded.దత్తాంశం = ngx_cpymem(*encrypted, "{SHA}", పరిమాణం("{SHA}") - 1);
    ngx_encode_base64(&encoded, &decoded);
    encoded.దత్తాంశం[encoded.len] = '\0';

    తిరిగి NGX_OK;
}

#అయితే_ముగింపు /* NGX_CRYPT */
