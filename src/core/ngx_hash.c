
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>


ਖਾਲੀ *
ngx_hash_ਲੱਭੋ(ngx_hash_t *hash, ngx_uint_t ਕੁੰਜੀ, u_char *name, ਮਾਪ_t len)
{
    ngx_uint_t       i;
    ngx_hash_elt_t  *elt;

#ਜੇ 0
    ngx_log_error(NGX_LOG_ALERT, ngx_ਚੱਕਰ->ਲਘੂਗਣਕ, 0, "hf:\"%*s\"", len, name);
#ਜੇ_ਅੰਤ

    elt = hash->buckets[ਕੁੰਜੀ % hash->ਮਾਪ];

    ਜੇ (elt == ਨਿਰਾ) {
        ਵਾਪਸ ਨਿਰਾ;
    }

    ਜਦੋਂ (elt->ਮੁੱਲ) {
        ਜੇ (len != (ਮਾਪ_t) elt->len) {
            ਜਾਓ ਅਗਲਾ;
        }

        ਲਈ (i = 0; i < len; i++) {
            ਜੇ (name[i] != elt->name[i]) {
                ਜਾਓ ਅਗਲਾ;
            }
        }

        ਵਾਪਸ elt->ਮੁੱਲ;

    ਅਗਲਾ:

        elt = (ngx_hash_elt_t *) ngx_align_ptr(&elt->name[0] + elt->len,
                                               ਆਕਾਰ(ਖਾਲੀ *));
        ਜਾਰੀ;
    }

    ਵਾਪਸ ਨਿਰਾ;
}


ਖਾਲੀ *
ngx_hash_ਲੱਭੋ_wc_ਸਿਰ(ngx_hash_wildcard_t *hwc, u_char *name, ਮਾਪ_t len)
{
    ਖਾਲੀ        *ਮੁੱਲ;
    ngx_uint_t   i, n, ਕੁੰਜੀ;

#ਜੇ 0
    ngx_log_error(NGX_LOG_ALERT, ngx_ਚੱਕਰ->ਲਘੂਗਣਕ, 0, "wch:\"%*s\"", len, name);
#ਜੇ_ਅੰਤ

    n = len;

    ਜਦੋਂ (n) {
        ਜੇ (name[n - 1] == '.') {
            ਰੋਕੋ;
        }

        n--;
    }

    ਕੁੰਜੀ = 0;

    ਲਈ (i = n; i < len; i++) {
        ਕੁੰਜੀ = ngx_hash(ਕੁੰਜੀ, name[i]);
    }

#ਜੇ 0
    ngx_log_error(NGX_LOG_ALERT, ngx_ਚੱਕਰ->ਲਘੂਗਣਕ, 0, "key:\"%ui\"", ਕੁੰਜੀ);
#ਜੇ_ਅੰਤ

    ਮੁੱਲ = ngx_hash_ਲੱਭੋ(&hwc->hash, ਕੁੰਜੀ, &name[n], len - n);

#ਜੇ 0
    ngx_log_error(NGX_LOG_ALERT, ngx_ਚੱਕਰ->ਲਘੂਗਣਕ, 0, "value:\"%p\"", ਮੁੱਲ);
#ਜੇ_ਅੰਤ

    ਜੇ (ਮੁੱਲ) {

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

        ਜੇ ((uintptr_t) ਮੁੱਲ & 2) {

            ਜੇ (n == 0) {

                /* "example.com" */

                ਜੇ ((uintptr_t) ਮੁੱਲ & 1) {
                    ਵਾਪਸ ਨਿਰਾ;
                }

                hwc = (ngx_hash_wildcard_t *)
                                          ((uintptr_t) ਮੁੱਲ & (uintptr_t) ~3);
                ਵਾਪਸ hwc->ਮੁੱਲ;
            }

            hwc = (ngx_hash_wildcard_t *) ((uintptr_t) ਮੁੱਲ & (uintptr_t) ~3);

            ਮੁੱਲ = ngx_hash_ਲੱਭੋ_wc_ਸਿਰ(hwc, name, n - 1);

            ਜੇ (ਮੁੱਲ) {
                ਵਾਪਸ ਮੁੱਲ;
            }

            ਵਾਪਸ hwc->ਮੁੱਲ;
        }

        ਜੇ ((uintptr_t) ਮੁੱਲ & 1) {

            ਜੇ (n == 0) {

                /* "example.com" */

                ਵਾਪਸ ਨਿਰਾ;
            }

            ਵਾਪਸ (ਖਾਲੀ *) ((uintptr_t) ਮੁੱਲ & (uintptr_t) ~3);
        }

        ਵਾਪਸ ਮੁੱਲ;
    }

    ਵਾਪਸ hwc->ਮੁੱਲ;
}


ਖਾਲੀ *
ngx_hash_ਲੱਭੋ_wc_ਪੂਛ(ngx_hash_wildcard_t *hwc, u_char *name, ਮਾਪ_t len)
{
    ਖਾਲੀ        *ਮੁੱਲ;
    ngx_uint_t   i, ਕੁੰਜੀ;

#ਜੇ 0
    ngx_log_error(NGX_LOG_ALERT, ngx_ਚੱਕਰ->ਲਘੂਗਣਕ, 0, "wct:\"%*s\"", len, name);
#ਜੇ_ਅੰਤ

    ਕੁੰਜੀ = 0;

    ਲਈ (i = 0; i < len; i++) {
        ਜੇ (name[i] == '.') {
            ਰੋਕੋ;
        }

        ਕੁੰਜੀ = ngx_hash(ਕੁੰਜੀ, name[i]);
    }

    ਜੇ (i == len) {
        ਵਾਪਸ ਨਿਰਾ;
    }

#ਜੇ 0
    ngx_log_error(NGX_LOG_ALERT, ngx_ਚੱਕਰ->ਲਘੂਗਣਕ, 0, "key:\"%ui\"", ਕੁੰਜੀ);
#ਜੇ_ਅੰਤ

    ਮੁੱਲ = ngx_hash_ਲੱਭੋ(&hwc->hash, ਕੁੰਜੀ, name, i);

#ਜੇ 0
    ngx_log_error(NGX_LOG_ALERT, ngx_ਚੱਕਰ->ਲਘੂਗਣਕ, 0, "value:\"%p\"", ਮੁੱਲ);
#ਜੇ_ਅੰਤ

    ਜੇ (ਮੁੱਲ) {

        /*
         * the 2 low bits of value have the special meaning:
         *     00 - value is data pointer;
         *     11 - value is pointer to wildcard hash allowing "example.*".
         */

        ਜੇ ((uintptr_t) ਮੁੱਲ & 2) {

            i++;

            hwc = (ngx_hash_wildcard_t *) ((uintptr_t) ਮੁੱਲ & (uintptr_t) ~3);

            ਮੁੱਲ = ngx_hash_ਲੱਭੋ_wc_ਪੂਛ(hwc, &name[i], len - i);

            ਜੇ (ਮੁੱਲ) {
                ਵਾਪਸ ਮੁੱਲ;
            }

            ਵਾਪਸ hwc->ਮੁੱਲ;
        }

        ਵਾਪਸ ਮੁੱਲ;
    }

    ਵਾਪਸ hwc->ਮੁੱਲ;
}


ਖਾਲੀ *
ngx_hash_ਲੱਭੋ_combined(ngx_hash_combined_t *hash, ngx_uint_t ਕੁੰਜੀ, u_char *name,
    ਮਾਪ_t len)
{
    ਖਾਲੀ  *ਮੁੱਲ;

    ਜੇ (hash->hash.buckets) {
        ਮੁੱਲ = ngx_hash_ਲੱਭੋ(&hash->hash, ਕੁੰਜੀ, name, len);

        ਜੇ (ਮੁੱਲ) {
            ਵਾਪਸ ਮੁੱਲ;
        }
    }

    ਜੇ (len == 0) {
        ਵਾਪਸ ਨਿਰਾ;
    }

    ਜੇ (hash->wc_ਸਿਰ && hash->wc_ਸਿਰ->hash.buckets) {
        ਮੁੱਲ = ngx_hash_ਲੱਭੋ_wc_ਸਿਰ(hash->wc_ਸਿਰ, name, len);

        ਜੇ (ਮੁੱਲ) {
            ਵਾਪਸ ਮੁੱਲ;
        }
    }

    ਜੇ (hash->wc_ਪੂਛ && hash->wc_ਪੂਛ->hash.buckets) {
        ਮੁੱਲ = ngx_hash_ਲੱਭੋ_wc_ਪੂਛ(hash->wc_ਪੂਛ, name, len);

        ਜੇ (ਮੁੱਲ) {
            ਵਾਪਸ ਮੁੱਲ;
        }
    }

    ਵਾਪਸ ਨਿਰਾ;
}


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_HASH_ELT_ਮਾਪ(name)                                               \
    (ਆਕਾਰ(ਖਾਲੀ *) + ngx_align((name)->ਕੁੰਜੀ.len + 2, ਆਕਾਰ(ਖਾਲੀ *)))

ngx_int_t
ngx_hash_init(ngx_hash_init_t *hinit, ngx_hash_ਕੁੰਜੀ_t *ਨਾਂ, ngx_uint_t nelts)
{
    u_char          *elts;
    ਮਾਪ_t           len;
    u_short         *test;
    ngx_uint_t       i, n, ਕੁੰਜੀ, ਮਾਪ, ਸ਼ੁਰੂ, ਬਾਲਟੀ_ਮਾਪ;
    ngx_hash_elt_t  *elt, **buckets;

    ਜੇ (hinit->max_ਮਾਪ == 0) {
        ngx_log_error(NGX_LOG_EMERG, hinit->pool->ਲਘੂਗਣਕ, 0,
                      "could not build %s, you should "
                      "increase %s_max_size: %i",
                      hinit->name, hinit->name, hinit->max_ਮਾਪ);
        ਵਾਪਸ NGX_ERROR;
    }

    ਜੇ (hinit->ਬਾਲਟੀ_ਮਾਪ > 65536 - ngx_cacheline_ਮਾਪ) {
        ngx_log_error(NGX_LOG_EMERG, hinit->pool->ਲਘੂਗਣਕ, 0,
                      "could not build %s, too large "
                      "%s_bucket_size: %i",
                      hinit->name, hinit->name, hinit->ਬਾਲਟੀ_ਮਾਪ);
        ਵਾਪਸ NGX_ERROR;
    }

    ਲਈ (n = 0; n < nelts; n++) {
        ਜੇ (ਨਾਂ[n].ਕੁੰਜੀ.ਅੰਕੜੇ == ਨਿਰਾ) {
            ਜਾਰੀ;
        }

        ਜੇ (hinit->ਬਾਲਟੀ_ਮਾਪ < NGX_HASH_ELT_ਮਾਪ(&ਨਾਂ[n]) + ਆਕਾਰ(ਖਾਲੀ *))
        {
            ngx_log_error(NGX_LOG_EMERG, hinit->pool->ਲਘੂਗਣਕ, 0,
                          "could not build %s, you should "
                          "increase %s_bucket_size: %i",
                          hinit->name, hinit->name, hinit->ਬਾਲਟੀ_ਮਾਪ);
            ਵਾਪਸ NGX_ERROR;
        }
    }

    test = ngx_alloc(hinit->max_ਮਾਪ * ਆਕਾਰ(u_short), hinit->pool->ਲਘੂਗਣਕ);
    ਜੇ (test == ਨਿਰਾ) {
        ਵਾਪਸ NGX_ERROR;
    }

    ਬਾਲਟੀ_ਮਾਪ = hinit->ਬਾਲਟੀ_ਮਾਪ - ਆਕਾਰ(ਖਾਲੀ *);

    ਸ਼ੁਰੂ = nelts / (ਬਾਲਟੀ_ਮਾਪ / (2 * ਆਕਾਰ(ਖਾਲੀ *)));
    ਸ਼ੁਰੂ = ਸ਼ੁਰੂ ? ਸ਼ੁਰੂ : 1;

    ਜੇ (hinit->max_ਮਾਪ > 10000 && nelts && hinit->max_ਮਾਪ / nelts < 100) {
        ਸ਼ੁਰੂ = hinit->max_ਮਾਪ - 1000;
    }

    ਲਈ (ਮਾਪ = ਸ਼ੁਰੂ; ਮਾਪ <= hinit->max_ਮਾਪ; ਮਾਪ++) {

        ngx_memzero(test, ਮਾਪ * ਆਕਾਰ(u_short));

        ਲਈ (n = 0; n < nelts; n++) {
            ਜੇ (ਨਾਂ[n].ਕੁੰਜੀ.ਅੰਕੜੇ == ਨਿਰਾ) {
                ਜਾਰੀ;
            }

            ਕੁੰਜੀ = ਨਾਂ[n].ਕੁੰਜੀ_hash % ਮਾਪ;
            len = test[ਕੁੰਜੀ] + NGX_HASH_ELT_ਮਾਪ(&ਨਾਂ[n]);

#ਜੇ 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->ਲਘੂਗਣਕ, 0,
                          "%ui: %ui %uz \"%V\"",
                          ਮਾਪ, ਕੁੰਜੀ, len, &ਨਾਂ[n].ਕੁੰਜੀ);
#ਜੇ_ਅੰਤ

            ਜੇ (len > ਬਾਲਟੀ_ਮਾਪ) {
                ਜਾਓ ਅਗਲਾ;
            }

            test[ਕੁੰਜੀ] = (u_short) len;
        }

        ਜਾਓ ਮਿਲਿਆ;

    ਅਗਲਾ:

        ਜਾਰੀ;
    }

    ਮਾਪ = hinit->max_ਮਾਪ;

    ngx_log_error(NGX_LOG_WARN, hinit->pool->ਲਘੂਗਣਕ, 0,
                  "could not build optimal %s, you should increase "
                  "either %s_max_size: %i or %s_bucket_size: %i; "
                  "ignoring %s_bucket_size",
                  hinit->name, hinit->name, hinit->max_ਮਾਪ,
                  hinit->name, hinit->ਬਾਲਟੀ_ਮਾਪ, hinit->name);

ਮਿਲਿਆ:

    ਲਈ (i = 0; i < ਮਾਪ; i++) {
        test[i] = ਆਕਾਰ(ਖਾਲੀ *);
    }

    ਲਈ (n = 0; n < nelts; n++) {
        ਜੇ (ਨਾਂ[n].ਕੁੰਜੀ.ਅੰਕੜੇ == ਨਿਰਾ) {
            ਜਾਰੀ;
        }

        ਕੁੰਜੀ = ਨਾਂ[n].ਕੁੰਜੀ_hash % ਮਾਪ;
        len = test[ਕੁੰਜੀ] + NGX_HASH_ELT_ਮਾਪ(&ਨਾਂ[n]);

        ਜੇ (len > 65536 - ngx_cacheline_ਮਾਪ) {
            ngx_log_error(NGX_LOG_EMERG, hinit->pool->ਲਘੂਗਣਕ, 0,
                          "could not build %s, you should "
                          "increase %s_max_size: %i",
                          hinit->name, hinit->name, hinit->max_ਮਾਪ);
            ngx_free(test);
            ਵਾਪਸ NGX_ERROR;
        }

        test[ਕੁੰਜੀ] = (u_short) len;
    }

    len = 0;

    ਲਈ (i = 0; i < ਮਾਪ; i++) {
        ਜੇ (test[i] == ਆਕਾਰ(ਖਾਲੀ *)) {
            ਜਾਰੀ;
        }

        test[i] = (u_short) (ngx_align(test[i], ngx_cacheline_ਮਾਪ));

        len += test[i];
    }

    ਜੇ (hinit->hash == ਨਿਰਾ) {
        hinit->hash = ngx_pcalloc(hinit->pool, ਆਕਾਰ(ngx_hash_wildcard_t)
                                             + ਮਾਪ * ਆਕਾਰ(ngx_hash_elt_t *));
        ਜੇ (hinit->hash == ਨਿਰਾ) {
            ngx_free(test);
            ਵਾਪਸ NGX_ERROR;
        }

        buckets = (ngx_hash_elt_t **)
                      ((u_char *) hinit->hash + ਆਕਾਰ(ngx_hash_wildcard_t));

    } ਨਹੀਂਤਾਂ {
        buckets = ngx_pcalloc(hinit->pool, ਮਾਪ * ਆਕਾਰ(ngx_hash_elt_t *));
        ਜੇ (buckets == ਨਿਰਾ) {
            ngx_free(test);
            ਵਾਪਸ NGX_ERROR;
        }
    }

    elts = ngx_palloc(hinit->pool, len + ngx_cacheline_ਮਾਪ);
    ਜੇ (elts == ਨਿਰਾ) {
        ngx_free(test);
        ਵਾਪਸ NGX_ERROR;
    }

    elts = ngx_align_ptr(elts, ngx_cacheline_ਮਾਪ);

    ਲਈ (i = 0; i < ਮਾਪ; i++) {
        ਜੇ (test[i] == ਆਕਾਰ(ਖਾਲੀ *)) {
            ਜਾਰੀ;
        }

        buckets[i] = (ngx_hash_elt_t *) elts;
        elts += test[i];
    }

    ਲਈ (i = 0; i < ਮਾਪ; i++) {
        test[i] = 0;
    }

    ਲਈ (n = 0; n < nelts; n++) {
        ਜੇ (ਨਾਂ[n].ਕੁੰਜੀ.ਅੰਕੜੇ == ਨਿਰਾ) {
            ਜਾਰੀ;
        }

        ਕੁੰਜੀ = ਨਾਂ[n].ਕੁੰਜੀ_hash % ਮਾਪ;
        elt = (ngx_hash_elt_t *) ((u_char *) buckets[ਕੁੰਜੀ] + test[ਕੁੰਜੀ]);

        elt->ਮੁੱਲ = ਨਾਂ[n].ਮੁੱਲ;
        elt->len = (u_short) ਨਾਂ[n].ਕੁੰਜੀ.len;

        ngx_strlow(elt->name, ਨਾਂ[n].ਕੁੰਜੀ.ਅੰਕੜੇ, ਨਾਂ[n].ਕੁੰਜੀ.len);

        test[ਕੁੰਜੀ] = (u_short) (test[ਕੁੰਜੀ] + NGX_HASH_ELT_ਮਾਪ(&ਨਾਂ[n]));
    }

    ਲਈ (i = 0; i < ਮਾਪ; i++) {
        ਜੇ (buckets[i] == ਨਿਰਾ) {
            ਜਾਰੀ;
        }

        elt = (ngx_hash_elt_t *) ((u_char *) buckets[i] + test[i]);

        elt->ਮੁੱਲ = ਨਿਰਾ;
    }

    ngx_free(test);

    hinit->hash->buckets = buckets;
    hinit->hash->ਮਾਪ = ਮਾਪ;

#ਜੇ 0

    ਲਈ (i = 0; i < ਮਾਪ; i++) {
        ngx_str_t   val;
        ngx_uint_t  ਕੁੰਜੀ;

        elt = buckets[i];

        ਜੇ (elt == ਨਿਰਾ) {
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->ਲਘੂਗਣਕ, 0,
                          "%ui: NULL", i);
            ਜਾਰੀ;
        }

        ਜਦੋਂ (elt->ਮੁੱਲ) {
            val.len = elt->len;
            val.ਅੰਕੜੇ = &elt->name[0];

            ਕੁੰਜੀ = hinit->ਕੁੰਜੀ(val.ਅੰਕੜੇ, val.len);

            ngx_log_error(NGX_LOG_ALERT, hinit->pool->ਲਘੂਗਣਕ, 0,
                          "%ui: %p \"%V\" %ui", i, elt, &val, ਕੁੰਜੀ);

            elt = (ngx_hash_elt_t *) ngx_align_ptr(&elt->name[0] + elt->len,
                                                   ਆਕਾਰ(ਖਾਲੀ *));
        }
    }

#ਜੇ_ਅੰਤ

    ਵਾਪਸ NGX_OK;
}


ngx_int_t
ngx_hash_wildcard_init(ngx_hash_init_t *hinit, ngx_hash_ਕੁੰਜੀ_t *ਨਾਂ,
    ngx_uint_t nelts)
{
    ਮਾਪ_t                len, dot_len;
    ngx_uint_t            i, n, dot;
    ngx_array_t           curr_ਨਾਂ, ਅਗਲਾ_ਨਾਂ;
    ngx_hash_ਕੁੰਜੀ_t       *name, *ਅਗਲਾ_name;
    ngx_hash_init_t       h;
    ngx_hash_wildcard_t  *wdc;

    ਜੇ (ngx_array_init(&curr_ਨਾਂ, hinit->ਆਰਜ਼ੀ_pool, nelts,
                       ਆਕਾਰ(ngx_hash_ਕੁੰਜੀ_t))
        != NGX_OK)
    {
        ਵਾਪਸ NGX_ERROR;
    }

    ਜੇ (ngx_array_init(&ਅਗਲਾ_ਨਾਂ, hinit->ਆਰਜ਼ੀ_pool, nelts,
                       ਆਕਾਰ(ngx_hash_ਕੁੰਜੀ_t))
        != NGX_OK)
    {
        ਵਾਪਸ NGX_ERROR;
    }

    ਲਈ (n = 0; n < nelts; n = i) {

#ਜੇ 0
        ngx_log_error(NGX_LOG_ALERT, hinit->pool->ਲਘੂਗਣਕ, 0,
                      "wc0: \"%V\"", &ਨਾਂ[n].ਕੁੰਜੀ);
#ਜੇ_ਅੰਤ

        dot = 0;

        ਲਈ (len = 0; len < ਨਾਂ[n].ਕੁੰਜੀ.len; len++) {
            ਜੇ (ਨਾਂ[n].ਕੁੰਜੀ.ਅੰਕੜੇ[len] == '.') {
                dot = 1;
                ਰੋਕੋ;
            }
        }

        name = ngx_array_ਧੱਕੋ(&curr_ਨਾਂ);
        ਜੇ (name == ਨਿਰਾ) {
            ਵਾਪਸ NGX_ERROR;
        }

        name->ਕੁੰਜੀ.len = len;
        name->ਕੁੰਜੀ.ਅੰਕੜੇ = ਨਾਂ[n].ਕੁੰਜੀ.ਅੰਕੜੇ;
        name->ਕੁੰਜੀ_hash = hinit->ਕੁੰਜੀ(name->ਕੁੰਜੀ.ਅੰਕੜੇ, name->ਕੁੰਜੀ.len);
        name->ਮੁੱਲ = ਨਾਂ[n].ਮੁੱਲ;

#ਜੇ 0
        ngx_log_error(NGX_LOG_ALERT, hinit->pool->ਲਘੂਗਣਕ, 0,
                      "wc1: \"%V\" %ui", &name->ਕੁੰਜੀ, dot);
#ਜੇ_ਅੰਤ

        dot_len = len + 1;

        ਜੇ (dot) {
            len++;
        }

        ਅਗਲਾ_ਨਾਂ.nelts = 0;

        ਜੇ (ਨਾਂ[n].ਕੁੰਜੀ.len != len) {
            ਅਗਲਾ_name = ngx_array_ਧੱਕੋ(&ਅਗਲਾ_ਨਾਂ);
            ਜੇ (ਅਗਲਾ_name == ਨਿਰਾ) {
                ਵਾਪਸ NGX_ERROR;
            }

            ਅਗਲਾ_name->ਕੁੰਜੀ.len = ਨਾਂ[n].ਕੁੰਜੀ.len - len;
            ਅਗਲਾ_name->ਕੁੰਜੀ.ਅੰਕੜੇ = ਨਾਂ[n].ਕੁੰਜੀ.ਅੰਕੜੇ + len;
            ਅਗਲਾ_name->ਕੁੰਜੀ_hash = 0;
            ਅਗਲਾ_name->ਮੁੱਲ = ਨਾਂ[n].ਮੁੱਲ;

#ਜੇ 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->ਲਘੂਗਣਕ, 0,
                          "wc2: \"%V\"", &ਅਗਲਾ_name->ਕੁੰਜੀ);
#ਜੇ_ਅੰਤ
        }

        ਲਈ (i = n + 1; i < nelts; i++) {
            ਜੇ (ngx_strncmp(ਨਾਂ[n].ਕੁੰਜੀ.ਅੰਕੜੇ, ਨਾਂ[i].ਕੁੰਜੀ.ਅੰਕੜੇ, len) != 0) {
                ਰੋਕੋ;
            }

            ਜੇ (!dot
                && ਨਾਂ[i].ਕੁੰਜੀ.len > len
                && ਨਾਂ[i].ਕੁੰਜੀ.ਅੰਕੜੇ[len] != '.')
            {
                ਰੋਕੋ;
            }

            ਅਗਲਾ_name = ngx_array_ਧੱਕੋ(&ਅਗਲਾ_ਨਾਂ);
            ਜੇ (ਅਗਲਾ_name == ਨਿਰਾ) {
                ਵਾਪਸ NGX_ERROR;
            }

            ਅਗਲਾ_name->ਕੁੰਜੀ.len = ਨਾਂ[i].ਕੁੰਜੀ.len - dot_len;
            ਅਗਲਾ_name->ਕੁੰਜੀ.ਅੰਕੜੇ = ਨਾਂ[i].ਕੁੰਜੀ.ਅੰਕੜੇ + dot_len;
            ਅਗਲਾ_name->ਕੁੰਜੀ_hash = 0;
            ਅਗਲਾ_name->ਮੁੱਲ = ਨਾਂ[i].ਮੁੱਲ;

#ਜੇ 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->ਲਘੂਗਣਕ, 0,
                          "wc3: \"%V\"", &ਅਗਲਾ_name->ਕੁੰਜੀ);
#ਜੇ_ਅੰਤ
        }

        ਜੇ (ਅਗਲਾ_ਨਾਂ.nelts) {

            h = *hinit;
            h.hash = ਨਿਰਾ;

            ਜੇ (ngx_hash_wildcard_init(&h, (ngx_hash_ਕੁੰਜੀ_t *) ਅਗਲਾ_ਨਾਂ.elts,
                                       ਅਗਲਾ_ਨਾਂ.nelts)
                != NGX_OK)
            {
                ਵਾਪਸ NGX_ERROR;
            }

            wdc = (ngx_hash_wildcard_t *) h.hash;

            ਜੇ (ਨਾਂ[n].ਕੁੰਜੀ.len == len) {
                wdc->ਮੁੱਲ = ਨਾਂ[n].ਮੁੱਲ;
            }

            name->ਮੁੱਲ = (ਖਾਲੀ *) ((uintptr_t) wdc | (dot ? 3 : 2));

        } ਨਹੀਂਤਾਂ ਜੇ (dot) {
            name->ਮੁੱਲ = (ਖਾਲੀ *) ((uintptr_t) name->ਮੁੱਲ | 1);
        }
    }

    ਜੇ (ngx_hash_init(hinit, (ngx_hash_ਕੁੰਜੀ_t *) curr_ਨਾਂ.elts,
                      curr_ਨਾਂ.nelts)
        != NGX_OK)
    {
        ਵਾਪਸ NGX_ERROR;
    }

    ਵਾਪਸ NGX_OK;
}


ngx_uint_t
ngx_hash_ਕੁੰਜੀ(u_char *ਅੰਕੜੇ, ਮਾਪ_t len)
{
    ngx_uint_t  i, ਕੁੰਜੀ;

    ਕੁੰਜੀ = 0;

    ਲਈ (i = 0; i < len; i++) {
        ਕੁੰਜੀ = ngx_hash(ਕੁੰਜੀ, ਅੰਕੜੇ[i]);
    }

    ਵਾਪਸ ਕੁੰਜੀ;
}


ngx_uint_t
ngx_hash_ਕੁੰਜੀ_lc(u_char *ਅੰਕੜੇ, ਮਾਪ_t len)
{
    ngx_uint_t  i, ਕੁੰਜੀ;

    ਕੁੰਜੀ = 0;

    ਲਈ (i = 0; i < len; i++) {
        ਕੁੰਜੀ = ngx_hash(ਕੁੰਜੀ, ngx_tolower(ਅੰਕੜੇ[i]));
    }

    ਵਾਪਸ ਕੁੰਜੀ;
}


ngx_uint_t
ngx_hash_strlow(u_char *dst, u_char *src, ਮਾਪ_t n)
{
    ngx_uint_t  ਕੁੰਜੀ;

    ਕੁੰਜੀ = 0;

    ਜਦੋਂ (n--) {
        *dst = ngx_tolower(*src);
        ਕੁੰਜੀ = ngx_hash(ਕੁੰਜੀ, *dst);
        dst++;
        src++;
    }

    ਵਾਪਸ ਕੁੰਜੀ;
}


ngx_int_t
ngx_hash_ਕੁੰਜੀਆਂ_array_init(ngx_hash_ਕੁੰਜੀਆਂ_arrays_t *ha, ngx_uint_t type)
{
    ngx_uint_t  asize;

    ਜੇ (type == NGX_HASH_SMALL) {
        asize = 4;
        ha->hsize = 107;

    } ਨਹੀਂਤਾਂ {
        asize = NGX_HASH_LARGE_ASIZE;
        ha->hsize = NGX_HASH_LARGE_HSIZE;
    }

    ਜੇ (ngx_array_init(&ha->ਕੁੰਜੀਆਂ, ha->ਆਰਜ਼ੀ_pool, asize, ਆਕਾਰ(ngx_hash_ਕੁੰਜੀ_t))
        != NGX_OK)
    {
        ਵਾਪਸ NGX_ERROR;
    }

    ਜੇ (ngx_array_init(&ha->dns_wc_ਸਿਰ, ha->ਆਰਜ਼ੀ_pool, asize,
                       ਆਕਾਰ(ngx_hash_ਕੁੰਜੀ_t))
        != NGX_OK)
    {
        ਵਾਪਸ NGX_ERROR;
    }

    ਜੇ (ngx_array_init(&ha->dns_wc_ਪੂਛ, ha->ਆਰਜ਼ੀ_pool, asize,
                       ਆਕਾਰ(ngx_hash_ਕੁੰਜੀ_t))
        != NGX_OK)
    {
        ਵਾਪਸ NGX_ERROR;
    }

    ha->ਕੁੰਜੀਆਂ_hash = ngx_pcalloc(ha->ਆਰਜ਼ੀ_pool, ਆਕਾਰ(ngx_array_t) * ha->hsize);
    ਜੇ (ha->ਕੁੰਜੀਆਂ_hash == ਨਿਰਾ) {
        ਵਾਪਸ NGX_ERROR;
    }

    ha->dns_wc_ਸਿਰ_hash = ngx_pcalloc(ha->ਆਰਜ਼ੀ_pool,
                                       ਆਕਾਰ(ngx_array_t) * ha->hsize);
    ਜੇ (ha->dns_wc_ਸਿਰ_hash == ਨਿਰਾ) {
        ਵਾਪਸ NGX_ERROR;
    }

    ha->dns_wc_ਪੂਛ_hash = ngx_pcalloc(ha->ਆਰਜ਼ੀ_pool,
                                       ਆਕਾਰ(ngx_array_t) * ha->hsize);
    ਜੇ (ha->dns_wc_ਪੂਛ_hash == ਨਿਰਾ) {
        ਵਾਪਸ NGX_ERROR;
    }

    ਵਾਪਸ NGX_OK;
}


ngx_int_t
ngx_hash_ਜੋੜੋ_ਕੁੰਜੀ(ngx_hash_ਕੁੰਜੀਆਂ_arrays_t *ha, ngx_str_t *ਕੁੰਜੀ, ਖਾਲੀ *ਮੁੱਲ,
    ngx_uint_t flags)
{
    ਮਾਪ_t           len;
    u_char          *p;
    ngx_str_t       *name;
    ngx_uint_t       i, k, n, skip, ਆਖਰੀ;
    ngx_array_t     *ਕੁੰਜੀਆਂ, *hwc;
    ngx_hash_ਕੁੰਜੀ_t  *hk;

    ਆਖਰੀ = ਕੁੰਜੀ->len;

    ਜੇ (flags & NGX_HASH_WILDCARD_ਕੁੰਜੀ) {

        /*
         * supported wildcards:
         *     "*.example.com", ".example.com", and "www.example.*"
         */

        n = 0;

        ਲਈ (i = 0; i < ਕੁੰਜੀ->len; i++) {

            ਜੇ (ਕੁੰਜੀ->ਅੰਕੜੇ[i] == '*') {
                ਜੇ (++n > 1) {
                    ਵਾਪਸ NGX_DECLINED;
                }
            }

            ਜੇ (ਕੁੰਜੀ->ਅੰਕੜੇ[i] == '.' && ਕੁੰਜੀ->ਅੰਕੜੇ[i + 1] == '.') {
                ਵਾਪਸ NGX_DECLINED;
            }

            ਜੇ (ਕੁੰਜੀ->ਅੰਕੜੇ[i] == '\0') {
                ਵਾਪਸ NGX_DECLINED;
            }
        }

        ਜੇ (ਕੁੰਜੀ->len > 1 && ਕੁੰਜੀ->ਅੰਕੜੇ[0] == '.') {
            skip = 1;
            ਜਾਓ wildcard;
        }

        ਜੇ (ਕੁੰਜੀ->len > 2) {

            ਜੇ (ਕੁੰਜੀ->ਅੰਕੜੇ[0] == '*' && ਕੁੰਜੀ->ਅੰਕੜੇ[1] == '.') {
                skip = 2;
                ਜਾਓ wildcard;
            }

            ਜੇ (ਕੁੰਜੀ->ਅੰਕੜੇ[i - 2] == '.' && ਕੁੰਜੀ->ਅੰਕੜੇ[i - 1] == '*') {
                skip = 0;
                ਆਖਰੀ -= 2;
                ਜਾਓ wildcard;
            }
        }

        ਜੇ (n) {
            ਵਾਪਸ NGX_DECLINED;
        }
    }

    /* exact hash */

    k = 0;

    ਲਈ (i = 0; i < ਆਖਰੀ; i++) {
        ਜੇ (!(flags & NGX_HASH_READONLY_ਕੁੰਜੀ)) {
            ਕੁੰਜੀ->ਅੰਕੜੇ[i] = ngx_tolower(ਕੁੰਜੀ->ਅੰਕੜੇ[i]);
        }
        k = ngx_hash(k, ਕੁੰਜੀ->ਅੰਕੜੇ[i]);
    }

    k %= ha->hsize;

    /* check conflicts in exact hash */

    name = ha->ਕੁੰਜੀਆਂ_hash[k].elts;

    ਜੇ (name) {
        ਲਈ (i = 0; i < ha->ਕੁੰਜੀਆਂ_hash[k].nelts; i++) {
            ਜੇ (ਆਖਰੀ != name[i].len) {
                ਜਾਰੀ;
            }

            ਜੇ (ngx_strncmp(ਕੁੰਜੀ->ਅੰਕੜੇ, name[i].ਅੰਕੜੇ, ਆਖਰੀ) == 0) {
                ਵਾਪਸ NGX_BUSY;
            }
        }

    } ਨਹੀਂਤਾਂ {
        ਜੇ (ngx_array_init(&ha->ਕੁੰਜੀਆਂ_hash[k], ha->ਆਰਜ਼ੀ_pool, 4,
                           ਆਕਾਰ(ngx_str_t))
            != NGX_OK)
        {
            ਵਾਪਸ NGX_ERROR;
        }
    }

    name = ngx_array_ਧੱਕੋ(&ha->ਕੁੰਜੀਆਂ_hash[k]);
    ਜੇ (name == ਨਿਰਾ) {
        ਵਾਪਸ NGX_ERROR;
    }

    *name = *ਕੁੰਜੀ;

    hk = ngx_array_ਧੱਕੋ(&ha->ਕੁੰਜੀਆਂ);
    ਜੇ (hk == ਨਿਰਾ) {
        ਵਾਪਸ NGX_ERROR;
    }

    hk->ਕੁੰਜੀ = *ਕੁੰਜੀ;
    hk->ਕੁੰਜੀ_hash = ngx_hash_ਕੁੰਜੀ(ਕੁੰਜੀ->ਅੰਕੜੇ, ਆਖਰੀ);
    hk->ਮੁੱਲ = ਮੁੱਲ;

    ਵਾਪਸ NGX_OK;


wildcard:

    /* wildcard hash */

    k = ngx_hash_strlow(&ਕੁੰਜੀ->ਅੰਕੜੇ[skip], &ਕੁੰਜੀ->ਅੰਕੜੇ[skip], ਆਖਰੀ - skip);

    k %= ha->hsize;

    ਜੇ (skip == 1) {

        /* check conflicts in exact hash for ".example.com" */

        name = ha->ਕੁੰਜੀਆਂ_hash[k].elts;

        ਜੇ (name) {
            len = ਆਖਰੀ - skip;

            ਲਈ (i = 0; i < ha->ਕੁੰਜੀਆਂ_hash[k].nelts; i++) {
                ਜੇ (len != name[i].len) {
                    ਜਾਰੀ;
                }

                ਜੇ (ngx_strncmp(&ਕੁੰਜੀ->ਅੰਕੜੇ[1], name[i].ਅੰਕੜੇ, len) == 0) {
                    ਵਾਪਸ NGX_BUSY;
                }
            }

        } ਨਹੀਂਤਾਂ {
            ਜੇ (ngx_array_init(&ha->ਕੁੰਜੀਆਂ_hash[k], ha->ਆਰਜ਼ੀ_pool, 4,
                               ਆਕਾਰ(ngx_str_t))
                != NGX_OK)
            {
                ਵਾਪਸ NGX_ERROR;
            }
        }

        name = ngx_array_ਧੱਕੋ(&ha->ਕੁੰਜੀਆਂ_hash[k]);
        ਜੇ (name == ਨਿਰਾ) {
            ਵਾਪਸ NGX_ERROR;
        }

        name->len = ਆਖਰੀ - 1;
        name->ਅੰਕੜੇ = ngx_pnalloc(ha->ਆਰਜ਼ੀ_pool, name->len);
        ਜੇ (name->ਅੰਕੜੇ == ਨਿਰਾ) {
            ਵਾਪਸ NGX_ERROR;
        }

        ngx_memcpy(name->ਅੰਕੜੇ, &ਕੁੰਜੀ->ਅੰਕੜੇ[1], name->len);
    }


    ਜੇ (skip) {

        /*
         * convert "*.example.com" to "com.example.\0"
         *      and ".example.com" to "com.example\0"
         */

        p = ngx_pnalloc(ha->ਆਰਜ਼ੀ_pool, ਆਖਰੀ);
        ਜੇ (p == ਨਿਰਾ) {
            ਵਾਪਸ NGX_ERROR;
        }

        len = 0;
        n = 0;

        ਲਈ (i = ਆਖਰੀ - 1; i; i--) {
            ਜੇ (ਕੁੰਜੀ->ਅੰਕੜੇ[i] == '.') {
                ngx_memcpy(&p[n], &ਕੁੰਜੀ->ਅੰਕੜੇ[i + 1], len);
                n += len;
                p[n++] = '.';
                len = 0;
                ਜਾਰੀ;
            }

            len++;
        }

        ਜੇ (len) {
            ngx_memcpy(&p[n], &ਕੁੰਜੀ->ਅੰਕੜੇ[1], len);
            n += len;
        }

        p[n] = '\0';

        hwc = &ha->dns_wc_ਸਿਰ;
        ਕੁੰਜੀਆਂ = &ha->dns_wc_ਸਿਰ_hash[k];

    } ਨਹੀਂਤਾਂ {

        /* convert "www.example.*" to "www.example\0" */

        ਆਖਰੀ++;

        p = ngx_pnalloc(ha->ਆਰਜ਼ੀ_pool, ਆਖਰੀ);
        ਜੇ (p == ਨਿਰਾ) {
            ਵਾਪਸ NGX_ERROR;
        }

        ngx_cpystrn(p, ਕੁੰਜੀ->ਅੰਕੜੇ, ਆਖਰੀ);

        hwc = &ha->dns_wc_ਪੂਛ;
        ਕੁੰਜੀਆਂ = &ha->dns_wc_ਪੂਛ_hash[k];
    }


    /* check conflicts in wildcard hash */

    name = ਕੁੰਜੀਆਂ->elts;

    ਜੇ (name) {
        len = ਆਖਰੀ - skip;

        ਲਈ (i = 0; i < ਕੁੰਜੀਆਂ->nelts; i++) {
            ਜੇ (len != name[i].len) {
                ਜਾਰੀ;
            }

            ਜੇ (ngx_strncmp(ਕੁੰਜੀ->ਅੰਕੜੇ + skip, name[i].ਅੰਕੜੇ, len) == 0) {
                ਵਾਪਸ NGX_BUSY;
            }
        }

    } ਨਹੀਂਤਾਂ {
        ਜੇ (ngx_array_init(ਕੁੰਜੀਆਂ, ha->ਆਰਜ਼ੀ_pool, 4, ਆਕਾਰ(ngx_str_t)) != NGX_OK)
        {
            ਵਾਪਸ NGX_ERROR;
        }
    }

    name = ngx_array_ਧੱਕੋ(ਕੁੰਜੀਆਂ);
    ਜੇ (name == ਨਿਰਾ) {
        ਵਾਪਸ NGX_ERROR;
    }

    name->len = ਆਖਰੀ - skip;
    name->ਅੰਕੜੇ = ngx_pnalloc(ha->ਆਰਜ਼ੀ_pool, name->len);
    ਜੇ (name->ਅੰਕੜੇ == ਨਿਰਾ) {
        ਵਾਪਸ NGX_ERROR;
    }

    ngx_memcpy(name->ਅੰਕੜੇ, ਕੁੰਜੀ->ਅੰਕੜੇ + skip, name->len);


    /* add to wildcard hash */

    hk = ngx_array_ਧੱਕੋ(hwc);
    ਜੇ (hk == ਨਿਰਾ) {
        ਵਾਪਸ NGX_ERROR;
    }

    hk->ਕੁੰਜੀ.len = ਆਖਰੀ - 1;
    hk->ਕੁੰਜੀ.ਅੰਕੜੇ = p;
    hk->ਕੁੰਜੀ_hash = 0;
    hk->ਮੁੱਲ = ਮੁੱਲ;

    ਵਾਪਸ NGX_OK;
}
