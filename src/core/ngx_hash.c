
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


ಶೂನ್ಯ *
ngx_hash_ಕಂಡುಹಿಡಿ(ngx_hash_t *hash, ngx_uint_t ಕೀಲಿ, u_char *name, ಗಾತ್ರ_t len)
{
    ngx_uint_t       i;
    ngx_hash_elt_t  *elt;

#ಆದರೆ 0
    ngx_log_error(NGX_LOG_ALERT, ngx_ಚಕ್ರ->ಲಘುಗಣಕ, 0, "hf:\"%*s\"", len, name);
#ಆದರೆ_ಅಂತ್ಯ

    elt = hash->buckets[ಕೀಲಿ % hash->ಗಾತ್ರ_2];

    ಆದರೆ (elt == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ಆಗ (elt->ಮೌಲ್ಯ) {
        ಆದರೆ (len != (ಗಾತ್ರ_t) elt->len) {
            ಹೋಗು ಮುಂದಿನ;
        }

        ಗಾಗಿ (i = 0; i < len; i++) {
            ಆದರೆ (name[i] != elt->name[i]) {
                ಹೋಗು ಮುಂದಿನ;
            }
        }

        ಹಿಂತಿರುಗಿ elt->ಮೌಲ್ಯ;

    ಮುಂದಿನ:

        elt = (ngx_hash_elt_t *) ngx_align_ptr(&elt->name[0] + elt->len,
                                               ಗಾತ್ರ(ಶೂನ್ಯ *));
        ಮುಂದುವರಿ;
    }

    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}


ಶೂನ್ಯ *
ngx_hash_ಕಂಡುಹಿಡಿ_wc_ತಲೆ(ngx_hash_wildcard_t *hwc, u_char *name, ಗಾತ್ರ_t len)
{
    ಶೂನ್ಯ        *ಮೌಲ್ಯ;
    ngx_uint_t   i, n, ಕೀಲಿ;

#ಆದರೆ 0
    ngx_log_error(NGX_LOG_ALERT, ngx_ಚಕ್ರ->ಲಘುಗಣಕ, 0, "wch:\"%*s\"", len, name);
#ಆದರೆ_ಅಂತ್ಯ

    n = len;

    ಆಗ (n) {
        ಆದರೆ (name[n - 1] == '.') {
            ನಿಲ್ಲಿಸು;
        }

        n--;
    }

    ಕೀಲಿ = 0;

    ಗಾಗಿ (i = n; i < len; i++) {
        ಕೀಲಿ = ngx_hash(ಕೀಲಿ, name[i]);
    }

#ಆದರೆ 0
    ngx_log_error(NGX_LOG_ALERT, ngx_ಚಕ್ರ->ಲಘುಗಣಕ, 0, "key:\"%ui\"", ಕೀಲಿ);
#ಆದರೆ_ಅಂತ್ಯ

    ಮೌಲ್ಯ = ngx_hash_ಕಂಡುಹಿಡಿ(&hwc->hash, ಕೀಲಿ, &name[n], len - n);

#ಆದರೆ 0
    ngx_log_error(NGX_LOG_ALERT, ngx_ಚಕ್ರ->ಲಘುಗಣಕ, 0, "value:\"%p\"", ಮೌಲ್ಯ);
#ಆದರೆ_ಅಂತ್ಯ

    ಆದರೆ (ಮೌಲ್ಯ) {

        /*
         * the 2 low bits of value have the special meaning:
         *     00 - value is data pointer for both "example.com"
         *          and "*.example.com";
         *     01 - value is data pointer for "*.example.com" only;
         *     10 - value is pointer to wildcard hash allowing
         *          both "example.com" and "*.example.com";
         *     11 - value is pointer to wildcard hash allowing
         *          "*.example.com" only.
         */

        ಆದರೆ ((uintptr_t) ಮೌಲ್ಯ & 2) {

            ಆದರೆ (n == 0) {

                /* "example.com" */

                ಆದರೆ ((uintptr_t) ಮೌಲ್ಯ & 1) {
                    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
                }

                hwc = (ngx_hash_wildcard_t *)
                                          ((uintptr_t) ಮೌಲ್ಯ & (uintptr_t) ~3);
                ಹಿಂತಿರುಗಿ hwc->ಮೌಲ್ಯ;
            }

            hwc = (ngx_hash_wildcard_t *) ((uintptr_t) ಮೌಲ್ಯ & (uintptr_t) ~3);

            ಮೌಲ್ಯ = ngx_hash_ಕಂಡುಹಿಡಿ_wc_ತಲೆ(hwc, name, n - 1);

            ಆದರೆ (ಮೌಲ್ಯ) {
                ಹಿಂತಿರುಗಿ ಮೌಲ್ಯ;
            }

            ಹಿಂತಿರುಗಿ hwc->ಮೌಲ್ಯ;
        }

        ಆದರೆ ((uintptr_t) ಮೌಲ್ಯ & 1) {

            ಆದರೆ (n == 0) {

                /* "example.com" */

                ಹಿಂತಿರುಗಿ ಖಾಲಿ;
            }

            ಹಿಂತಿರುಗಿ (ಶೂನ್ಯ *) ((uintptr_t) ಮೌಲ್ಯ & (uintptr_t) ~3);
        }

        ಹಿಂತಿರುಗಿ ಮೌಲ್ಯ;
    }

    ಹಿಂತಿರುಗಿ hwc->ಮೌಲ್ಯ;
}


ಶೂನ್ಯ *
ngx_hash_ಕಂಡುಹಿಡಿ_wc_ಬಾಲ(ngx_hash_wildcard_t *hwc, u_char *name, ಗಾತ್ರ_t len)
{
    ಶೂನ್ಯ        *ಮೌಲ್ಯ;
    ngx_uint_t   i, ಕೀಲಿ;

#ಆದರೆ 0
    ngx_log_error(NGX_LOG_ALERT, ngx_ಚಕ್ರ->ಲಘುಗಣಕ, 0, "wct:\"%*s\"", len, name);
#ಆದರೆ_ಅಂತ್ಯ

    ಕೀಲಿ = 0;

    ಗಾಗಿ (i = 0; i < len; i++) {
        ಆದರೆ (name[i] == '.') {
            ನಿಲ್ಲಿಸು;
        }

        ಕೀಲಿ = ngx_hash(ಕೀಲಿ, name[i]);
    }

    ಆದರೆ (i == len) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

#ಆದರೆ 0
    ngx_log_error(NGX_LOG_ALERT, ngx_ಚಕ್ರ->ಲಘುಗಣಕ, 0, "key:\"%ui\"", ಕೀಲಿ);
#ಆದರೆ_ಅಂತ್ಯ

    ಮೌಲ್ಯ = ngx_hash_ಕಂಡುಹಿಡಿ(&hwc->hash, ಕೀಲಿ, name, i);

#ಆದರೆ 0
    ngx_log_error(NGX_LOG_ALERT, ngx_ಚಕ್ರ->ಲಘುಗಣಕ, 0, "value:\"%p\"", ಮೌಲ್ಯ);
#ಆದರೆ_ಅಂತ್ಯ

    ಆದರೆ (ಮೌಲ್ಯ) {

        /*
         * the 2 low bits of value have the special meaning:
         *     00 - value is data pointer;
         *     11 - value is pointer to wildcard hash allowing "example.*".
         */

        ಆದರೆ ((uintptr_t) ಮೌಲ್ಯ & 2) {

            i++;

            hwc = (ngx_hash_wildcard_t *) ((uintptr_t) ಮೌಲ್ಯ & (uintptr_t) ~3);

            ಮೌಲ್ಯ = ngx_hash_ಕಂಡುಹಿಡಿ_wc_ಬಾಲ(hwc, &name[i], len - i);

            ಆದರೆ (ಮೌಲ್ಯ) {
                ಹಿಂತಿರುಗಿ ಮೌಲ್ಯ;
            }

            ಹಿಂತಿರುಗಿ hwc->ಮೌಲ್ಯ;
        }

        ಹಿಂತಿರುಗಿ ಮೌಲ್ಯ;
    }

    ಹಿಂತಿರುಗಿ hwc->ಮೌಲ್ಯ;
}


ಶೂನ್ಯ *
ngx_hash_ಕಂಡುಹಿಡಿ_combined(ngx_hash_combined_t *hash, ngx_uint_t ಕೀಲಿ, u_char *name,
    ಗಾತ್ರ_t len)
{
    ಶೂನ್ಯ  *ಮೌಲ್ಯ;

    ಆದರೆ (hash->hash.buckets) {
        ಮೌಲ್ಯ = ngx_hash_ಕಂಡುಹಿಡಿ(&hash->hash, ಕೀಲಿ, name, len);

        ಆದರೆ (ಮೌಲ್ಯ) {
            ಹಿಂತಿರುಗಿ ಮೌಲ್ಯ;
        }
    }

    ಆದರೆ (len == 0) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ಆದರೆ (hash->wc_ತಲೆ && hash->wc_ತಲೆ->hash.buckets) {
        ಮೌಲ್ಯ = ngx_hash_ಕಂಡುಹಿಡಿ_wc_ತಲೆ(hash->wc_ತಲೆ, name, len);

        ಆದರೆ (ಮೌಲ್ಯ) {
            ಹಿಂತಿರುಗಿ ಮೌಲ್ಯ;
        }
    }

    ಆದರೆ (hash->wc_ಬಾಲ && hash->wc_ಬಾಲ->hash.buckets) {
        ಮೌಲ್ಯ = ngx_hash_ಕಂಡುಹಿಡಿ_wc_ಬಾಲ(hash->wc_ಬಾಲ, name, len);

        ಆದರೆ (ಮೌಲ್ಯ) {
            ಹಿಂತಿರುಗಿ ಮೌಲ್ಯ;
        }
    }

    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}


#ವ್ಯಾಖ್ಯಾನಿಸು NGX_HASH_ELT_ಗಾತ್ರ(name)                                               \
    (ಗಾತ್ರ(ಶೂನ್ಯ *) + ngx_align((name)->ಕೀಲಿ.len + 2, ಗಾತ್ರ(ಶೂನ್ಯ *)))

ngx_int_t
ngx_hash_init(ngx_hash_init_t *hinit, ngx_hash_ಕೀಲಿ_t *ಹೆಸರುಗಳು, ngx_uint_t nelts)
{
    u_char          *elts;
    ಗಾತ್ರ_t           len;
    u_short         *test;
    ngx_uint_t       i, n, ಕೀಲಿ, ಗಾತ್ರ_2, ಆರಂಭ, ಪಾತ್ರೆ_ಗಾತ್ರ;
    ngx_hash_elt_t  *elt, **buckets;

    ಆದರೆ (hinit->max_ಗಾತ್ರ == 0) {
        ngx_log_error(NGX_LOG_EMERG, hinit->pool->ಲಘುಗಣಕ, 0,
                      "could not build %s, you should "
                      "increase %s_max_size: %i",
                      hinit->name, hinit->name, hinit->max_ಗಾತ್ರ);
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    ಆದರೆ (hinit->ಪಾತ್ರೆ_ಗಾತ್ರ > 65536 - ngx_cacheline_ಗಾತ್ರ) {
        ngx_log_error(NGX_LOG_EMERG, hinit->pool->ಲಘುಗಣಕ, 0,
                      "could not build %s, too large "
                      "%s_bucket_size: %i",
                      hinit->name, hinit->name, hinit->ಪಾತ್ರೆ_ಗಾತ್ರ);
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    ಗಾಗಿ (n = 0; n < nelts; n++) {
        ಆದರೆ (ಹೆಸರುಗಳು[n].ಕೀಲಿ.ದತ್ತಾಂಶ == ಖಾಲಿ) {
            ಮುಂದುವರಿ;
        }

        ಆದರೆ (hinit->ಪಾತ್ರೆ_ಗಾತ್ರ < NGX_HASH_ELT_ಗಾತ್ರ(&ಹೆಸರುಗಳು[n]) + ಗಾತ್ರ(ಶೂನ್ಯ *))
        {
            ngx_log_error(NGX_LOG_EMERG, hinit->pool->ಲಘುಗಣಕ, 0,
                          "could not build %s, you should "
                          "increase %s_bucket_size: %i",
                          hinit->name, hinit->name, hinit->ಪಾತ್ರೆ_ಗಾತ್ರ);
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }
    }

    test = ngx_alloc(hinit->max_ಗಾತ್ರ * ಗಾತ್ರ(u_short), hinit->pool->ಲಘುಗಣಕ);
    ಆದರೆ (test == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    ಪಾತ್ರೆ_ಗಾತ್ರ = hinit->ಪಾತ್ರೆ_ಗಾತ್ರ - ಗಾತ್ರ(ಶೂನ್ಯ *);

    ಆರಂಭ = nelts / (ಪಾತ್ರೆ_ಗಾತ್ರ / (2 * ಗಾತ್ರ(ಶೂನ್ಯ *)));
    ಆರಂಭ = ಆರಂಭ ? ಆರಂಭ : 1;

    ಆದರೆ (hinit->max_ಗಾತ್ರ > 10000 && nelts && hinit->max_ಗಾತ್ರ / nelts < 100) {
        ಆರಂಭ = hinit->max_ಗಾತ್ರ - 1000;
    }

    ಗಾಗಿ (ಗಾತ್ರ_2 = ಆರಂಭ; ಗಾತ್ರ_2 <= hinit->max_ಗಾತ್ರ; ಗಾತ್ರ_2++) {

        ngx_memzero(test, ಗಾತ್ರ_2 * ಗಾತ್ರ(u_short));

        ಗಾಗಿ (n = 0; n < nelts; n++) {
            ಆದರೆ (ಹೆಸರುಗಳು[n].ಕೀಲಿ.ದತ್ತಾಂಶ == ಖಾಲಿ) {
                ಮುಂದುವರಿ;
            }

            ಕೀಲಿ = ಹೆಸರುಗಳು[n].ಕೀಲಿ_hash % ಗಾತ್ರ_2;
            len = test[ಕೀಲಿ] + NGX_HASH_ELT_ಗಾತ್ರ(&ಹೆಸರುಗಳು[n]);

#ಆದರೆ 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->ಲಘುಗಣಕ, 0,
                          "%ui: %ui %uz \"%V\"",
                          ಗಾತ್ರ_2, ಕೀಲಿ, len, &ಹೆಸರುಗಳು[n].ಕೀಲಿ);
#ಆದರೆ_ಅಂತ್ಯ

            ಆದರೆ (len > ಪಾತ್ರೆ_ಗಾತ್ರ) {
                ಹೋಗು ಮುಂದಿನ;
            }

            test[ಕೀಲಿ] = (u_short) len;
        }

        ಹೋಗು ಸಿಕ್ಕಿತು;

    ಮುಂದಿನ:

        ಮುಂದುವರಿ;
    }

    ಗಾತ್ರ_2 = hinit->max_ಗಾತ್ರ;

    ngx_log_error(NGX_LOG_WARN, hinit->pool->ಲಘುಗಣಕ, 0,
                  "could not build optimal %s, you should increase "
                  "either %s_max_size: %i or %s_bucket_size: %i; "
                  "ignoring %s_bucket_size",
                  hinit->name, hinit->name, hinit->max_ಗಾತ್ರ,
                  hinit->name, hinit->ಪಾತ್ರೆ_ಗಾತ್ರ, hinit->name);

ಸಿಕ್ಕಿತು:

    ಗಾಗಿ (i = 0; i < ಗಾತ್ರ_2; i++) {
        test[i] = ಗಾತ್ರ(ಶೂನ್ಯ *);
    }

    ಗಾಗಿ (n = 0; n < nelts; n++) {
        ಆದರೆ (ಹೆಸರುಗಳು[n].ಕೀಲಿ.ದತ್ತಾಂಶ == ಖಾಲಿ) {
            ಮುಂದುವರಿ;
        }

        ಕೀಲಿ = ಹೆಸರುಗಳು[n].ಕೀಲಿ_hash % ಗಾತ್ರ_2;
        len = test[ಕೀಲಿ] + NGX_HASH_ELT_ಗಾತ್ರ(&ಹೆಸರುಗಳು[n]);

        ಆದರೆ (len > 65536 - ngx_cacheline_ಗಾತ್ರ) {
            ngx_log_error(NGX_LOG_EMERG, hinit->pool->ಲಘುಗಣಕ, 0,
                          "could not build %s, you should "
                          "increase %s_max_size: %i",
                          hinit->name, hinit->name, hinit->max_ಗಾತ್ರ);
            ngx_free(test);
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        test[ಕೀಲಿ] = (u_short) len;
    }

    len = 0;

    ಗಾಗಿ (i = 0; i < ಗಾತ್ರ_2; i++) {
        ಆದರೆ (test[i] == ಗಾತ್ರ(ಶೂನ್ಯ *)) {
            ಮುಂದುವರಿ;
        }

        test[i] = (u_short) (ngx_align(test[i], ngx_cacheline_ಗಾತ್ರ));

        len += test[i];
    }

    ಆದರೆ (hinit->hash == ಖಾಲಿ) {
        hinit->hash = ngx_pcalloc(hinit->pool, ಗಾತ್ರ(ngx_hash_wildcard_t)
                                             + ಗಾತ್ರ_2 * ಗಾತ್ರ(ngx_hash_elt_t *));
        ಆದರೆ (hinit->hash == ಖಾಲಿ) {
            ngx_free(test);
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        buckets = (ngx_hash_elt_t **)
                      ((u_char *) hinit->hash + ಗಾತ್ರ(ngx_hash_wildcard_t));

    } ಇಲ್ಲದಿದ್ದರೆ {
        buckets = ngx_pcalloc(hinit->pool, ಗಾತ್ರ_2 * ಗಾತ್ರ(ngx_hash_elt_t *));
        ಆದರೆ (buckets == ಖಾಲಿ) {
            ngx_free(test);
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }
    }

    elts = ngx_palloc(hinit->pool, len + ngx_cacheline_ಗಾತ್ರ);
    ಆದರೆ (elts == ಖಾಲಿ) {
        ngx_free(test);
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    elts = ngx_align_ptr(elts, ngx_cacheline_ಗಾತ್ರ);

    ಗಾಗಿ (i = 0; i < ಗಾತ್ರ_2; i++) {
        ಆದರೆ (test[i] == ಗಾತ್ರ(ಶೂನ್ಯ *)) {
            ಮುಂದುವರಿ;
        }

        buckets[i] = (ngx_hash_elt_t *) elts;
        elts += test[i];
    }

    ಗಾಗಿ (i = 0; i < ಗಾತ್ರ_2; i++) {
        test[i] = 0;
    }

    ಗಾಗಿ (n = 0; n < nelts; n++) {
        ಆದರೆ (ಹೆಸರುಗಳು[n].ಕೀಲಿ.ದತ್ತಾಂಶ == ಖಾಲಿ) {
            ಮುಂದುವರಿ;
        }

        ಕೀಲಿ = ಹೆಸರುಗಳು[n].ಕೀಲಿ_hash % ಗಾತ್ರ_2;
        elt = (ngx_hash_elt_t *) ((u_char *) buckets[ಕೀಲಿ] + test[ಕೀಲಿ]);

        elt->ಮೌಲ್ಯ = ಹೆಸರುಗಳು[n].ಮೌಲ್ಯ;
        elt->len = (u_short) ಹೆಸರುಗಳು[n].ಕೀಲಿ.len;

        ngx_strlow(elt->name, ಹೆಸರುಗಳು[n].ಕೀಲಿ.ದತ್ತಾಂಶ, ಹೆಸರುಗಳು[n].ಕೀಲಿ.len);

        test[ಕೀಲಿ] = (u_short) (test[ಕೀಲಿ] + NGX_HASH_ELT_ಗಾತ್ರ(&ಹೆಸರುಗಳು[n]));
    }

    ಗಾಗಿ (i = 0; i < ಗಾತ್ರ_2; i++) {
        ಆದರೆ (buckets[i] == ಖಾಲಿ) {
            ಮುಂದುವರಿ;
        }

        elt = (ngx_hash_elt_t *) ((u_char *) buckets[i] + test[i]);

        elt->ಮೌಲ್ಯ = ಖಾಲಿ;
    }

    ngx_free(test);

    hinit->hash->buckets = buckets;
    hinit->hash->ಗಾತ್ರ_2 = ಗಾತ್ರ_2;

#ಆದರೆ 0

    ಗಾಗಿ (i = 0; i < ಗಾತ್ರ_2; i++) {
        ngx_str_t   val;
        ngx_uint_t  ಕೀಲಿ;

        elt = buckets[i];

        ಆದರೆ (elt == ಖಾಲಿ) {
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->ಲಘುಗಣಕ, 0,
                          "%ui: NULL", i);
            ಮುಂದುವರಿ;
        }

        ಆಗ (elt->ಮೌಲ್ಯ) {
            val.len = elt->len;
            val.ದತ್ತಾಂಶ = &elt->name[0];

            ಕೀಲಿ = hinit->ಕೀಲಿ(val.ದತ್ತಾಂಶ, val.len);

            ngx_log_error(NGX_LOG_ALERT, hinit->pool->ಲಘುಗಣಕ, 0,
                          "%ui: %p \"%V\" %ui", i, elt, &val, ಕೀಲಿ);

            elt = (ngx_hash_elt_t *) ngx_align_ptr(&elt->name[0] + elt->len,
                                                   ಗಾತ್ರ(ಶೂನ್ಯ *));
        }
    }

#ಆದರೆ_ಅಂತ್ಯ

    ಹಿಂತಿರುಗಿ NGX_OK;
}


ngx_int_t
ngx_hash_wildcard_init(ngx_hash_init_t *hinit, ngx_hash_ಕೀಲಿ_t *ಹೆಸರುಗಳು,
    ngx_uint_t nelts)
{
    ಗಾತ್ರ_t                len, dot_len;
    ngx_uint_t            i, n, dot;
    ngx_array_t           curr_ಹೆಸರುಗಳು, ಮುಂದಿನ_ಹೆಸರುಗಳು;
    ngx_hash_ಕೀಲಿ_t       *name, *ಮುಂದಿನ_name;
    ngx_hash_init_t       h;
    ngx_hash_wildcard_t  *wdc;

    ಆದರೆ (ngx_array_init(&curr_ಹೆಸರುಗಳು, hinit->ತಾತ್ಕಾಲಿಕ_pool, nelts,
                       ಗಾತ್ರ(ngx_hash_ಕೀಲಿ_t))
        != NGX_OK)
    {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    ಆದರೆ (ngx_array_init(&ಮುಂದಿನ_ಹೆಸರುಗಳು, hinit->ತಾತ್ಕಾಲಿಕ_pool, nelts,
                       ಗಾತ್ರ(ngx_hash_ಕೀಲಿ_t))
        != NGX_OK)
    {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    ಗಾಗಿ (n = 0; n < nelts; n = i) {

#ಆದರೆ 0
        ngx_log_error(NGX_LOG_ALERT, hinit->pool->ಲಘುಗಣಕ, 0,
                      "wc0: \"%V\"", &ಹೆಸರುಗಳು[n].ಕೀಲಿ);
#ಆದರೆ_ಅಂತ್ಯ

        dot = 0;

        ಗಾಗಿ (len = 0; len < ಹೆಸರುಗಳು[n].ಕೀಲಿ.len; len++) {
            ಆದರೆ (ಹೆಸರುಗಳು[n].ಕೀಲಿ.ದತ್ತಾಂಶ[len] == '.') {
                dot = 1;
                ನಿಲ್ಲಿಸು;
            }
        }

        name = ngx_array_ತಳ್ಳು(&curr_ಹೆಸರುಗಳು);
        ಆದರೆ (name == ಖಾಲಿ) {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        name->ಕೀಲಿ.len = len;
        name->ಕೀಲಿ.ದತ್ತಾಂಶ = ಹೆಸರುಗಳು[n].ಕೀಲಿ.ದತ್ತಾಂಶ;
        name->ಕೀಲಿ_hash = hinit->ಕೀಲಿ(name->ಕೀಲಿ.ದತ್ತಾಂಶ, name->ಕೀಲಿ.len);
        name->ಮೌಲ್ಯ = ಹೆಸರುಗಳು[n].ಮೌಲ್ಯ;

#ಆದರೆ 0
        ngx_log_error(NGX_LOG_ALERT, hinit->pool->ಲಘುಗಣಕ, 0,
                      "wc1: \"%V\" %ui", &name->ಕೀಲಿ, dot);
#ಆದರೆ_ಅಂತ್ಯ

        dot_len = len + 1;

        ಆದರೆ (dot) {
            len++;
        }

        ಮುಂದಿನ_ಹೆಸರುಗಳು.nelts = 0;

        ಆದರೆ (ಹೆಸರುಗಳು[n].ಕೀಲಿ.len != len) {
            ಮುಂದಿನ_name = ngx_array_ತಳ್ಳು(&ಮುಂದಿನ_ಹೆಸರುಗಳು);
            ಆದರೆ (ಮುಂದಿನ_name == ಖಾಲಿ) {
                ಹಿಂತಿರುಗಿ NGX_ERROR;
            }

            ಮುಂದಿನ_name->ಕೀಲಿ.len = ಹೆಸರುಗಳು[n].ಕೀಲಿ.len - len;
            ಮುಂದಿನ_name->ಕೀಲಿ.ದತ್ತಾಂಶ = ಹೆಸರುಗಳು[n].ಕೀಲಿ.ದತ್ತಾಂಶ + len;
            ಮುಂದಿನ_name->ಕೀಲಿ_hash = 0;
            ಮುಂದಿನ_name->ಮೌಲ್ಯ = ಹೆಸರುಗಳು[n].ಮೌಲ್ಯ;

#ಆದರೆ 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->ಲಘುಗಣಕ, 0,
                          "wc2: \"%V\"", &ಮುಂದಿನ_name->ಕೀಲಿ);
#ಆದರೆ_ಅಂತ್ಯ
        }

        ಗಾಗಿ (i = n + 1; i < nelts; i++) {
            ಆದರೆ (ngx_strncmp(ಹೆಸರುಗಳು[n].ಕೀಲಿ.ದತ್ತಾಂಶ, ಹೆಸರುಗಳು[i].ಕೀಲಿ.ದತ್ತಾಂಶ, len) != 0) {
                ನಿಲ್ಲಿಸು;
            }

            ಆದರೆ (!dot
                && ಹೆಸರುಗಳು[i].ಕೀಲಿ.len > len
                && ಹೆಸರುಗಳು[i].ಕೀಲಿ.ದತ್ತಾಂಶ[len] != '.')
            {
                ನಿಲ್ಲಿಸು;
            }

            ಮುಂದಿನ_name = ngx_array_ತಳ್ಳು(&ಮುಂದಿನ_ಹೆಸರುಗಳು);
            ಆದರೆ (ಮುಂದಿನ_name == ಖಾಲಿ) {
                ಹಿಂತಿರುಗಿ NGX_ERROR;
            }

            ಮುಂದಿನ_name->ಕೀಲಿ.len = ಹೆಸರುಗಳು[i].ಕೀಲಿ.len - dot_len;
            ಮುಂದಿನ_name->ಕೀಲಿ.ದತ್ತಾಂಶ = ಹೆಸರುಗಳು[i].ಕೀಲಿ.ದತ್ತಾಂಶ + dot_len;
            ಮುಂದಿನ_name->ಕೀಲಿ_hash = 0;
            ಮುಂದಿನ_name->ಮೌಲ್ಯ = ಹೆಸರುಗಳು[i].ಮೌಲ್ಯ;

#ಆದರೆ 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->ಲಘುಗಣಕ, 0,
                          "wc3: \"%V\"", &ಮುಂದಿನ_name->ಕೀಲಿ);
#ಆದರೆ_ಅಂತ್ಯ
        }

        ಆದರೆ (ಮುಂದಿನ_ಹೆಸರುಗಳು.nelts) {

            h = *hinit;
            h.hash = ಖಾಲಿ;

            ಆದರೆ (ngx_hash_wildcard_init(&h, (ngx_hash_ಕೀಲಿ_t *) ಮುಂದಿನ_ಹೆಸರುಗಳು.elts,
                                       ಮುಂದಿನ_ಹೆಸರುಗಳು.nelts)
                != NGX_OK)
            {
                ಹಿಂತಿರುಗಿ NGX_ERROR;
            }

            wdc = (ngx_hash_wildcard_t *) h.hash;

            ಆದರೆ (ಹೆಸರುಗಳು[n].ಕೀಲಿ.len == len) {
                wdc->ಮೌಲ್ಯ = ಹೆಸರುಗಳು[n].ಮೌಲ್ಯ;
            }

            name->ಮೌಲ್ಯ = (ಶೂನ್ಯ *) ((uintptr_t) wdc | (dot ? 3 : 2));

        } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (dot) {
            name->ಮೌಲ್ಯ = (ಶೂನ್ಯ *) ((uintptr_t) name->ಮೌಲ್ಯ | 1);
        }
    }

    ಆದರೆ (ngx_hash_init(hinit, (ngx_hash_ಕೀಲಿ_t *) curr_ಹೆಸರುಗಳು.elts,
                      curr_ಹೆಸರುಗಳು.nelts)
        != NGX_OK)
    {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    ಹಿಂತಿರುಗಿ NGX_OK;
}


ngx_uint_t
ngx_hash_ಕೀಲಿ(u_char *ದತ್ತಾಂಶ, ಗಾತ್ರ_t len)
{
    ngx_uint_t  i, ಕೀಲಿ;

    ಕೀಲಿ = 0;

    ಗಾಗಿ (i = 0; i < len; i++) {
        ಕೀಲಿ = ngx_hash(ಕೀಲಿ, ದತ್ತಾಂಶ[i]);
    }

    ಹಿಂತಿರುಗಿ ಕೀಲಿ;
}


ngx_uint_t
ngx_hash_ಕೀಲಿ_lc(u_char *ದತ್ತಾಂಶ, ಗಾತ್ರ_t len)
{
    ngx_uint_t  i, ಕೀಲಿ;

    ಕೀಲಿ = 0;

    ಗಾಗಿ (i = 0; i < len; i++) {
        ಕೀಲಿ = ngx_hash(ಕೀಲಿ, ngx_tolower(ದತ್ತಾಂಶ[i]));
    }

    ಹಿಂತಿರುಗಿ ಕೀಲಿ;
}


ngx_uint_t
ngx_hash_strlow(u_char *dst, u_char *src, ಗಾತ್ರ_t n)
{
    ngx_uint_t  ಕೀಲಿ;

    ಕೀಲಿ = 0;

    ಆಗ (n--) {
        *dst = ngx_tolower(*src);
        ಕೀಲಿ = ngx_hash(ಕೀಲಿ, *dst);
        dst++;
        src++;
    }

    ಹಿಂತಿರುಗಿ ಕೀಲಿ;
}


ngx_int_t
ngx_hash_ಕೀಲಿಗಳು_array_init(ngx_hash_ಕೀಲಿಗಳು_arrays_t *ha, ngx_uint_t type)
{
    ngx_uint_t  asize;

    ಆದರೆ (type == NGX_HASH_SMALL) {
        asize = 4;
        ha->hsize = 107;

    } ಇಲ್ಲದಿದ್ದರೆ {
        asize = NGX_HASH_LARGE_ASIZE;
        ha->hsize = NGX_HASH_LARGE_HSIZE;
    }

    ಆದರೆ (ngx_array_init(&ha->ಕೀಲಿಗಳು, ha->ತಾತ್ಕಾಲಿಕ_pool, asize, ಗಾತ್ರ(ngx_hash_ಕೀಲಿ_t))
        != NGX_OK)
    {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    ಆದರೆ (ngx_array_init(&ha->dns_wc_ತಲೆ, ha->ತಾತ್ಕಾಲಿಕ_pool, asize,
                       ಗಾತ್ರ(ngx_hash_ಕೀಲಿ_t))
        != NGX_OK)
    {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    ಆದರೆ (ngx_array_init(&ha->dns_wc_ಬಾಲ, ha->ತಾತ್ಕಾಲಿಕ_pool, asize,
                       ಗಾತ್ರ(ngx_hash_ಕೀಲಿ_t))
        != NGX_OK)
    {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    ha->ಕೀಲಿಗಳು_hash = ngx_pcalloc(ha->ತಾತ್ಕಾಲಿಕ_pool, ಗಾತ್ರ(ngx_array_t) * ha->hsize);
    ಆದರೆ (ha->ಕೀಲಿಗಳು_hash == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    ha->dns_wc_ತಲೆ_hash = ngx_pcalloc(ha->ತಾತ್ಕಾಲಿಕ_pool,
                                       ಗಾತ್ರ(ngx_array_t) * ha->hsize);
    ಆದರೆ (ha->dns_wc_ತಲೆ_hash == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    ha->dns_wc_ಬಾಲ_hash = ngx_pcalloc(ha->ತಾತ್ಕಾಲಿಕ_pool,
                                       ಗಾತ್ರ(ngx_array_t) * ha->hsize);
    ಆದರೆ (ha->dns_wc_ಬಾಲ_hash == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    ಹಿಂತಿರುಗಿ NGX_OK;
}


ngx_int_t
ngx_hash_ಕೂಡಿಸು_ಕೀಲಿ(ngx_hash_ಕೀಲಿಗಳು_arrays_t *ha, ngx_str_t *ಕೀಲಿ, ಶೂನ್ಯ *ಮೌಲ್ಯ,
    ngx_uint_t flags)
{
    ಗಾತ್ರ_t           len;
    u_char          *p;
    ngx_str_t       *name;
    ngx_uint_t       i, k, n, skip, ಕೊನೆಯ;
    ngx_array_t     *ಕೀಲಿಗಳು, *hwc;
    ngx_hash_ಕೀಲಿ_t  *hk;

    ಕೊನೆಯ = ಕೀಲಿ->len;

    ಆದರೆ (flags & NGX_HASH_WILDCARD_ಕೀಲಿ) {

        /*
         * supported wildcards:
         *     "*.example.com", ".example.com", and "www.example.*"
         */

        n = 0;

        ಗಾಗಿ (i = 0; i < ಕೀಲಿ->len; i++) {

            ಆದರೆ (ಕೀಲಿ->ದತ್ತಾಂಶ[i] == '*') {
                ಆದರೆ (++n > 1) {
                    ಹಿಂತಿರುಗಿ NGX_DECLINED;
                }
            }

            ಆದರೆ (ಕೀಲಿ->ದತ್ತಾಂಶ[i] == '.' && ಕೀಲಿ->ದತ್ತಾಂಶ[i + 1] == '.') {
                ಹಿಂತಿರುಗಿ NGX_DECLINED;
            }

            ಆದರೆ (ಕೀಲಿ->ದತ್ತಾಂಶ[i] == '\0') {
                ಹಿಂತಿರುಗಿ NGX_DECLINED;
            }
        }

        ಆದರೆ (ಕೀಲಿ->len > 1 && ಕೀಲಿ->ದತ್ತಾಂಶ[0] == '.') {
            skip = 1;
            ಹೋಗು wildcard;
        }

        ಆದರೆ (ಕೀಲಿ->len > 2) {

            ಆದರೆ (ಕೀಲಿ->ದತ್ತಾಂಶ[0] == '*' && ಕೀಲಿ->ದತ್ತಾಂಶ[1] == '.') {
                skip = 2;
                ಹೋಗು wildcard;
            }

            ಆದರೆ (ಕೀಲಿ->ದತ್ತಾಂಶ[i - 2] == '.' && ಕೀಲಿ->ದತ್ತಾಂಶ[i - 1] == '*') {
                skip = 0;
                ಕೊನೆಯ -= 2;
                ಹೋಗು wildcard;
            }
        }

        ಆದರೆ (n) {
            ಹಿಂತಿರುಗಿ NGX_DECLINED;
        }
    }

    /* exact hash */

    k = 0;

    ಗಾಗಿ (i = 0; i < ಕೊನೆಯ; i++) {
        ಆದರೆ (!(flags & NGX_HASH_READONLY_ಕೀಲಿ)) {
            ಕೀಲಿ->ದತ್ತಾಂಶ[i] = ngx_tolower(ಕೀಲಿ->ದತ್ತಾಂಶ[i]);
        }
        k = ngx_hash(k, ಕೀಲಿ->ದತ್ತಾಂಶ[i]);
    }

    k %= ha->hsize;

    /* check conflicts in exact hash */

    name = ha->ಕೀಲಿಗಳು_hash[k].elts;

    ಆದರೆ (name) {
        ಗಾಗಿ (i = 0; i < ha->ಕೀಲಿಗಳು_hash[k].nelts; i++) {
            ಆದರೆ (ಕೊನೆಯ != name[i].len) {
                ಮುಂದುವರಿ;
            }

            ಆದರೆ (ngx_strncmp(ಕೀಲಿ->ದತ್ತಾಂಶ, name[i].ದತ್ತಾಂಶ, ಕೊನೆಯ) == 0) {
                ಹಿಂತಿರುಗಿ NGX_BUSY;
            }
        }

    } ಇಲ್ಲದಿದ್ದರೆ {
        ಆದರೆ (ngx_array_init(&ha->ಕೀಲಿಗಳು_hash[k], ha->ತಾತ್ಕಾಲಿಕ_pool, 4,
                           ಗಾತ್ರ(ngx_str_t))
            != NGX_OK)
        {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }
    }

    name = ngx_array_ತಳ್ಳು(&ha->ಕೀಲಿಗಳು_hash[k]);
    ಆದರೆ (name == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    *name = *ಕೀಲಿ;

    hk = ngx_array_ತಳ್ಳು(&ha->ಕೀಲಿಗಳು);
    ಆದರೆ (hk == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    hk->ಕೀಲಿ = *ಕೀಲಿ;
    hk->ಕೀಲಿ_hash = ngx_hash_ಕೀಲಿ(ಕೀಲಿ->ದತ್ತಾಂಶ, ಕೊನೆಯ);
    hk->ಮೌಲ್ಯ = ಮೌಲ್ಯ;

    ಹಿಂತಿರುಗಿ NGX_OK;


wildcard:

    /* wildcard hash */

    k = ngx_hash_strlow(&ಕೀಲಿ->ದತ್ತಾಂಶ[skip], &ಕೀಲಿ->ದತ್ತಾಂಶ[skip], ಕೊನೆಯ - skip);

    k %= ha->hsize;

    ಆದರೆ (skip == 1) {

        /* check conflicts in exact hash for ".example.com" */

        name = ha->ಕೀಲಿಗಳು_hash[k].elts;

        ಆದರೆ (name) {
            len = ಕೊನೆಯ - skip;

            ಗಾಗಿ (i = 0; i < ha->ಕೀಲಿಗಳು_hash[k].nelts; i++) {
                ಆದರೆ (len != name[i].len) {
                    ಮುಂದುವರಿ;
                }

                ಆದರೆ (ngx_strncmp(&ಕೀಲಿ->ದತ್ತಾಂಶ[1], name[i].ದತ್ತಾಂಶ, len) == 0) {
                    ಹಿಂತಿರುಗಿ NGX_BUSY;
                }
            }

        } ಇಲ್ಲದಿದ್ದರೆ {
            ಆದರೆ (ngx_array_init(&ha->ಕೀಲಿಗಳು_hash[k], ha->ತಾತ್ಕಾಲಿಕ_pool, 4,
                               ಗಾತ್ರ(ngx_str_t))
                != NGX_OK)
            {
                ಹಿಂತಿರುಗಿ NGX_ERROR;
            }
        }

        name = ngx_array_ತಳ್ಳು(&ha->ಕೀಲಿಗಳು_hash[k]);
        ಆದರೆ (name == ಖಾಲಿ) {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        name->len = ಕೊನೆಯ - 1;
        name->ದತ್ತಾಂಶ = ngx_pnalloc(ha->ತಾತ್ಕಾಲಿಕ_pool, name->len);
        ಆದರೆ (name->ದತ್ತಾಂಶ == ಖಾಲಿ) {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        ngx_memcpy(name->ದತ್ತಾಂಶ, &ಕೀಲಿ->ದತ್ತಾಂಶ[1], name->len);
    }


    ಆದರೆ (skip) {

        /*
         * convert "*.example.com" to "com.example.\0"
         *      and ".example.com" to "com.example\0"
         */

        p = ngx_pnalloc(ha->ತಾತ್ಕಾಲಿಕ_pool, ಕೊನೆಯ);
        ಆದರೆ (p == ಖಾಲಿ) {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        len = 0;
        n = 0;

        ಗಾಗಿ (i = ಕೊನೆಯ - 1; i; i--) {
            ಆದರೆ (ಕೀಲಿ->ದತ್ತಾಂಶ[i] == '.') {
                ngx_memcpy(&p[n], &ಕೀಲಿ->ದತ್ತಾಂಶ[i + 1], len);
                n += len;
                p[n++] = '.';
                len = 0;
                ಮುಂದುವರಿ;
            }

            len++;
        }

        ಆದರೆ (len) {
            ngx_memcpy(&p[n], &ಕೀಲಿ->ದತ್ತಾಂಶ[1], len);
            n += len;
        }

        p[n] = '\0';

        hwc = &ha->dns_wc_ತಲೆ;
        ಕೀಲಿಗಳು = &ha->dns_wc_ತಲೆ_hash[k];

    } ಇಲ್ಲದಿದ್ದರೆ {

        /* convert "www.example.*" to "www.example\0" */

        ಕೊನೆಯ++;

        p = ngx_pnalloc(ha->ತಾತ್ಕಾಲಿಕ_pool, ಕೊನೆಯ);
        ಆದರೆ (p == ಖಾಲಿ) {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        ngx_cpystrn(p, ಕೀಲಿ->ದತ್ತಾಂಶ, ಕೊನೆಯ);

        hwc = &ha->dns_wc_ಬಾಲ;
        ಕೀಲಿಗಳು = &ha->dns_wc_ಬಾಲ_hash[k];
    }


    /* check conflicts in wildcard hash */

    name = ಕೀಲಿಗಳು->elts;

    ಆದರೆ (name) {
        len = ಕೊನೆಯ - skip;

        ಗಾಗಿ (i = 0; i < ಕೀಲಿಗಳು->nelts; i++) {
            ಆದರೆ (len != name[i].len) {
                ಮುಂದುವರಿ;
            }

            ಆದರೆ (ngx_strncmp(ಕೀಲಿ->ದತ್ತಾಂಶ + skip, name[i].ದತ್ತಾಂಶ, len) == 0) {
                ಹಿಂತಿರುಗಿ NGX_BUSY;
            }
        }

    } ಇಲ್ಲದಿದ್ದರೆ {
        ಆದರೆ (ngx_array_init(ಕೀಲಿಗಳು, ha->ತಾತ್ಕಾಲಿಕ_pool, 4, ಗಾತ್ರ(ngx_str_t)) != NGX_OK)
        {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }
    }

    name = ngx_array_ತಳ್ಳು(ಕೀಲಿಗಳು);
    ಆದರೆ (name == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    name->len = ಕೊನೆಯ - skip;
    name->ದತ್ತಾಂಶ = ngx_pnalloc(ha->ತಾತ್ಕಾಲಿಕ_pool, name->len);
    ಆದರೆ (name->ದತ್ತಾಂಶ == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    ngx_memcpy(name->ದತ್ತಾಂಶ, ಕೀಲಿ->ದತ್ತಾಂಶ + skip, name->len);


    /* add to wildcard hash */

    hk = ngx_array_ತಳ್ಳು(hwc);
    ಆದರೆ (hk == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    hk->ಕೀಲಿ.len = ಕೊನೆಯ - 1;
    hk->ಕೀಲಿ.ದತ್ತಾಂಶ = p;
    hk->ಕೀಲಿ_hash = 0;
    hk->ಮೌಲ್ಯ = ಮೌಲ್ಯ;

    ಹಿಂತಿರುಗಿ NGX_OK;
}
