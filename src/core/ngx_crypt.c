
/*
 * Copyright (C) Maxim Dounin
 */


#समावेश <ngx_config.h>
#समावेश <ngx_core.h>
#समावेश <ngx_crypt.h>
#समावेश <ngx_md5.h>
#समावेश <ngx_sha1.h>


#अगर (NGX_CRYPT)

अटल ngx_int_t ngx_crypt_apr1(ngx_pool_t *pool, u_char *कुंजी, u_char *salt,
    u_char **encrypted);
अटल ngx_int_t ngx_crypt_plain(ngx_pool_t *pool, u_char *कुंजी, u_char *salt,
    u_char **encrypted);
अटल ngx_int_t ngx_crypt_ssha(ngx_pool_t *pool, u_char *कुंजी, u_char *salt,
    u_char **encrypted);
अटल ngx_int_t ngx_crypt_sha(ngx_pool_t *pool, u_char *कुंजी, u_char *salt,
    u_char **encrypted);


अटल u_char *ngx_crypt_to64(u_char *p, uint32_t v, माप_प्रकार n);


ngx_int_t
ngx_crypt(ngx_pool_t *pool, u_char *कुंजी, u_char *salt, u_char **encrypted)
{
    अगर (ngx_strncmp(salt, "$apr1$", माप("$apr1$") - 1) == 0) {
        वापस ngx_crypt_apr1(pool, कुंजी, salt, encrypted);

    } अन्यथा अगर (ngx_strncmp(salt, "{PLAIN}", माप("{PLAIN}") - 1) == 0) {
        वापस ngx_crypt_plain(pool, कुंजी, salt, encrypted);

    } अन्यथा अगर (ngx_strncmp(salt, "{SSHA}", माप("{SSHA}") - 1) == 0) {
        वापस ngx_crypt_ssha(pool, कुंजी, salt, encrypted);

    } अन्यथा अगर (ngx_strncmp(salt, "{SHA}", माप("{SHA}") - 1) == 0) {
        वापस ngx_crypt_sha(pool, कुंजी, salt, encrypted);
    }

    /* fallback to libc crypt() */

    वापस ngx_libc_crypt(pool, कुंजी, salt, encrypted);
}


अटल ngx_int_t
ngx_crypt_apr1(ngx_pool_t *pool, u_char *कुंजी, u_char *salt, u_char **encrypted)
{
    ngx_int_t          n;
    ngx_uint_t         i;
    u_char            *p, *last, final[16];
    माप_प्रकार             saltlen, keylen;
    ngx_md5_t          md5, ctx1;

    /* Apache's apr1 crypt is Poul-Henning Kamp's md5 crypt with $apr1$ magic */

    keylen = ngx_strlen(कुंजी);

    /* true salt: no magic, max 8 chars, stop at first $ */

    salt += माप("$apr1$") - 1;
    last = salt + 8;
    क्रम (p = salt; *p && *p != '$' && p < last; p++) { /* void */ }
    saltlen = p - salt;

    /* hash key and salt */

    ngx_md5_init(&md5);
    ngx_md5_update(&md5, कुंजी, keylen);
    ngx_md5_update(&md5, (u_char *) "$apr1$", माप("$apr1$") - 1);
    ngx_md5_update(&md5, salt, saltlen);

    ngx_md5_init(&ctx1);
    ngx_md5_update(&ctx1, कुंजी, keylen);
    ngx_md5_update(&ctx1, salt, saltlen);
    ngx_md5_update(&ctx1, कुंजी, keylen);
    ngx_md5_final(final, &ctx1);

    क्रम (n = keylen; n > 0; n -= 16) {
        ngx_md5_update(&md5, final, n > 16 ? 16 : n);
    }

    ngx_memzero(final, माप(final));

    क्रम (i = keylen; i; i >>= 1) {
        अगर (i & 1) {
            ngx_md5_update(&md5, final, 1);

        } अन्यथा {
            ngx_md5_update(&md5, कुंजी, 1);
        }
    }

    ngx_md5_final(final, &md5);

    क्रम (i = 0; i < 1000; i++) {
        ngx_md5_init(&ctx1);

        अगर (i & 1) {
            ngx_md5_update(&ctx1, कुंजी, keylen);

        } अन्यथा {
            ngx_md5_update(&ctx1, final, 16);
        }

        अगर (i % 3) {
            ngx_md5_update(&ctx1, salt, saltlen);
        }

        अगर (i % 7) {
            ngx_md5_update(&ctx1, कुंजी, keylen);
        }

        अगर (i & 1) {
            ngx_md5_update(&ctx1, final, 16);

        } अन्यथा {
            ngx_md5_update(&ctx1, कुंजी, keylen);
        }

        ngx_md5_final(final, &ctx1);
    }

    /* output */

    *encrypted = ngx_pnalloc(pool, माप("$apr1$") - 1 + saltlen + 1 + 22 + 1);
    अगर (*encrypted == शून्य) {
        वापस NGX_ERROR;
    }

    p = ngx_cpymem(*encrypted, "$apr1$", माप("$apr1$") - 1);
    p = ngx_copy(p, salt, saltlen);
    *p++ = '$';

    p = ngx_crypt_to64(p, (final[ 0]<<16) | (final[ 6]<<8) | final[12], 4);
    p = ngx_crypt_to64(p, (final[ 1]<<16) | (final[ 7]<<8) | final[13], 4);
    p = ngx_crypt_to64(p, (final[ 2]<<16) | (final[ 8]<<8) | final[14], 4);
    p = ngx_crypt_to64(p, (final[ 3]<<16) | (final[ 9]<<8) | final[15], 4);
    p = ngx_crypt_to64(p, (final[ 4]<<16) | (final[10]<<8) | final[ 5], 4);
    p = ngx_crypt_to64(p, final[11], 2);
    *p = '\0';

    वापस NGX_OK;
}


अटल u_char *
ngx_crypt_to64(u_char *p, uint32_t v, माप_प्रकार n)
{
    अटल u_char   itoa64[] =
        "./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    जबतक (n--) {
        *p++ = itoa64[v & 0x3f];
        v >>= 6;
    }

    वापस p;
}


अटल ngx_int_t
ngx_crypt_plain(ngx_pool_t *pool, u_char *कुंजी, u_char *salt, u_char **encrypted)
{
    माप_प्रकार   len;
    u_char  *p;

    len = ngx_strlen(कुंजी);

    *encrypted = ngx_pnalloc(pool, माप("{PLAIN}") - 1 + len + 1);
    अगर (*encrypted == शून्य) {
        वापस NGX_ERROR;
    }

    p = ngx_cpymem(*encrypted, "{PLAIN}", माप("{PLAIN}") - 1);
    ngx_memcpy(p, कुंजी, len + 1);

    वापस NGX_OK;
}


अटल ngx_int_t
ngx_crypt_ssha(ngx_pool_t *pool, u_char *कुंजी, u_char *salt, u_char **encrypted)
{
    माप_प्रकार       len;
    ngx_int_t    rc;
    ngx_str_t    encoded, decoded;
    ngx_sha1_t   sha1;

    /* "{SSHA}" base64(SHA1(key salt) salt) */

    /* decode base64 salt to find out true salt */

    encoded.तथ्य = salt + माप("{SSHA}") - 1;
    encoded.len = ngx_strlen(encoded.तथ्य);

    len = ngx_max(ngx_base64_decoded_लंबाई(encoded.len), 20);

    decoded.तथ्य = ngx_pnalloc(pool, len);
    अगर (decoded.तथ्य == शून्य) {
        वापस NGX_ERROR;
    }

    rc = ngx_decode_base64(&decoded, &encoded);

    अगर (rc != NGX_OK || decoded.len < 20) {
        decoded.len = 20;
    }

    /* update SHA1 from key and salt */

    ngx_sha1_init(&sha1);
    ngx_sha1_update(&sha1, कुंजी, ngx_strlen(कुंजी));
    ngx_sha1_update(&sha1, decoded.तथ्य + 20, decoded.len - 20);
    ngx_sha1_final(decoded.तथ्य, &sha1);

    /* encode it back to base64 */

    len = माप("{SSHA}") - 1 + ngx_base64_encoded_लंबाई(decoded.len) + 1;

    *encrypted = ngx_pnalloc(pool, len);
    अगर (*encrypted == शून्य) {
        वापस NGX_ERROR;
    }

    encoded.तथ्य = ngx_cpymem(*encrypted, "{SSHA}", माप("{SSHA}") - 1);
    ngx_encode_base64(&encoded, &decoded);
    encoded.तथ्य[encoded.len] = '\0';

    वापस NGX_OK;
}


अटल ngx_int_t
ngx_crypt_sha(ngx_pool_t *pool, u_char *कुंजी, u_char *salt, u_char **encrypted)
{
    माप_प्रकार      len;
    ngx_str_t   encoded, decoded;
    ngx_sha1_t  sha1;
    u_char      digest[20];

    /* "{SHA}" base64(SHA1(key)) */

    decoded.len = माप(digest);
    decoded.तथ्य = digest;

    ngx_sha1_init(&sha1);
    ngx_sha1_update(&sha1, कुंजी, ngx_strlen(कुंजी));
    ngx_sha1_final(digest, &sha1);

    len = माप("{SHA}") - 1 + ngx_base64_encoded_लंबाई(decoded.len) + 1;

    *encrypted = ngx_pnalloc(pool, len);
    अगर (*encrypted == शून्य) {
        वापस NGX_ERROR;
    }

    encoded.तथ्य = ngx_cpymem(*encrypted, "{SHA}", माप("{SHA}") - 1);
    ngx_encode_base64(&encoded, &decoded);
    encoded.तथ्य[encoded.len] = '\0';

    वापस NGX_OK;
}

#पूर्ण_अगर /* NGX_CRYPT */
