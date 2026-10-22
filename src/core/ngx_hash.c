
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


ಶೂನ್ಯ *
ngx_hash_find(ngx_hash_t *hash, ngx_uint_t ಕೀಲಿ, u_char *name, size_t len)
{
    ngx_uint_t       i;
    ngx_hash_elt_t  *elt;

#ಯದಿ 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "hf:\"%*s\"", len, name);
#endif

    elt = hash->buckets[ಕೀಲಿ % hash->size];

    ಯದಿ (elt == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ಯಾವತ್ (elt->ಮೌಲ್ಯ) {
        ಯದಿ (len != (size_t) elt->len) {
            ಗಚ್ಛ next;
        }

        ಪ್ರತಿ (i = 0; i < len; i++) {
            ಯದಿ (name[i] != elt->name[i]) {
                ಗಚ್ಛ next;
            }
        }

        ಪ್ರತ್ಯಾಗಮ elt->ಮೌಲ್ಯ;

    next:

        elt = (ngx_hash_elt_t *) ngx_align_ptr(&elt->name[0] + elt->len,
                                               ಪರಿಮಾಣ(ಶೂನ್ಯ *));
        ಅನುವರ್ತ;
    }

    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}


ಶೂನ್ಯ *
ngx_hash_find_wc_head(ngx_hash_wildcard_t *hwc, u_char *name, size_t len)
{
    ಶೂನ್ಯ        *ಮೌಲ್ಯ;
    ngx_uint_t   i, n, ಕೀಲಿ;

#ಯದಿ 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "wch:\"%*s\"", len, name);
#endif

    n = len;

    ಯಾವತ್ (n) {
        ಯದಿ (name[n - 1] == '.') {
            ಭಙ್ಗ;
        }

        n--;
    }

    ಕೀಲಿ = 0;

    ಪ್ರತಿ (i = n; i < len; i++) {
        ಕೀಲಿ = ngx_hash(ಕೀಲಿ, name[i]);
    }

#ಯದಿ 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "key:\"%ui\"", ಕೀಲಿ);
#endif

    ಮೌಲ್ಯ = ngx_hash_find(&hwc->hash, ಕೀಲಿ, &name[n], len - n);

#ಯದಿ 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "value:\"%p\"", ಮೌಲ್ಯ);
#endif

    ಯದಿ (ಮೌಲ್ಯ) {

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

        ಯದಿ ((uintptr_t) ಮೌಲ್ಯ & 2) {

            ಯದಿ (n == 0) {

                /* "example.com" */

                ಯದಿ ((uintptr_t) ಮೌಲ್ಯ & 1) {
                    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
                }

                hwc = (ngx_hash_wildcard_t *)
                                          ((uintptr_t) ಮೌಲ್ಯ & (uintptr_t) ~3);
                ಪ್ರತ್ಯಾಗಮ hwc->ಮೌಲ್ಯ;
            }

            hwc = (ngx_hash_wildcard_t *) ((uintptr_t) ಮೌಲ್ಯ & (uintptr_t) ~3);

            ಮೌಲ್ಯ = ngx_hash_find_wc_head(hwc, name, n - 1);

            ಯದಿ (ಮೌಲ್ಯ) {
                ಪ್ರತ್ಯಾಗಮ ಮೌಲ್ಯ;
            }

            ಪ್ರತ್ಯಾಗಮ hwc->ಮೌಲ್ಯ;
        }

        ಯದಿ ((uintptr_t) ಮೌಲ್ಯ & 1) {

            ಯದಿ (n == 0) {

                /* "example.com" */

                ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
            }

            ಪ್ರತ್ಯಾಗಮ (ಶೂನ್ಯ *) ((uintptr_t) ಮೌಲ್ಯ & (uintptr_t) ~3);
        }

        ಪ್ರತ್ಯಾಗಮ ಮೌಲ್ಯ;
    }

    ಪ್ರತ್ಯಾಗಮ hwc->ಮೌಲ್ಯ;
}


ಶೂನ್ಯ *
ngx_hash_find_wc_tail(ngx_hash_wildcard_t *hwc, u_char *name, size_t len)
{
    ಶೂನ್ಯ        *ಮೌಲ್ಯ;
    ngx_uint_t   i, ಕೀಲಿ;

#ಯದಿ 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "wct:\"%*s\"", len, name);
#endif

    ಕೀಲಿ = 0;

    ಪ್ರತಿ (i = 0; i < len; i++) {
        ಯದಿ (name[i] == '.') {
            ಭಙ್ಗ;
        }

        ಕೀಲಿ = ngx_hash(ಕೀಲಿ, name[i]);
    }

    ಯದಿ (i == len) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

#ಯದಿ 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "key:\"%ui\"", ಕೀಲಿ);
#endif

    ಮೌಲ್ಯ = ngx_hash_find(&hwc->hash, ಕೀಲಿ, name, i);

#ಯದಿ 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "value:\"%p\"", ಮೌಲ್ಯ);
#endif

    ಯದಿ (ಮೌಲ್ಯ) {

        /*
         * the 2 low bits of value have the special meaning:
         *     00 - value is data pointer;
         *     11 - value is pointer to wildcard hash allowing "example.*".
         */

        ಯದಿ ((uintptr_t) ಮೌಲ್ಯ & 2) {

            i++;

            hwc = (ngx_hash_wildcard_t *) ((uintptr_t) ಮೌಲ್ಯ & (uintptr_t) ~3);

            ಮೌಲ್ಯ = ngx_hash_find_wc_tail(hwc, &name[i], len - i);

            ಯದಿ (ಮೌಲ್ಯ) {
                ಪ್ರತ್ಯಾಗಮ ಮೌಲ್ಯ;
            }

            ಪ್ರತ್ಯಾಗಮ hwc->ಮೌಲ್ಯ;
        }

        ಪ್ರತ್ಯಾಗಮ ಮೌಲ್ಯ;
    }

    ಪ್ರತ್ಯಾಗಮ hwc->ಮೌಲ್ಯ;
}


ಶೂನ್ಯ *
ngx_hash_find_combined(ngx_hash_combined_t *hash, ngx_uint_t ಕೀಲಿ, u_char *name,
    size_t len)
{
    ಶೂನ್ಯ  *ಮೌಲ್ಯ;

    ಯದಿ (hash->hash.buckets) {
        ಮೌಲ್ಯ = ngx_hash_find(&hash->hash, ಕೀಲಿ, name, len);

        ಯದಿ (ಮೌಲ್ಯ) {
            ಪ್ರತ್ಯಾಗಮ ಮೌಲ್ಯ;
        }
    }

    ಯದಿ (len == 0) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ಯದಿ (hash->wc_head && hash->wc_head->hash.buckets) {
        ಮೌಲ್ಯ = ngx_hash_find_wc_head(hash->wc_head, name, len);

        ಯದಿ (ಮೌಲ್ಯ) {
            ಪ್ರತ್ಯಾಗಮ ಮೌಲ್ಯ;
        }
    }

    ಯದಿ (hash->wc_tail && hash->wc_tail->hash.buckets) {
        ಮೌಲ್ಯ = ngx_hash_find_wc_tail(hash->wc_tail, name, len);

        ಯದಿ (ಮೌಲ್ಯ) {
            ಪ್ರತ್ಯಾಗಮ ಮೌಲ್ಯ;
        }
    }

    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}


#define NGX_HASH_ELT_SIZE(name)                                               \
    (ಪರಿಮಾಣ(ಶೂನ್ಯ *) + ngx_align((name)->ಕೀಲಿ.len + 2, ಪರಿಮಾಣ(ಶೂನ್ಯ *)))

ngx_int_t
ngx_hash_init(ngx_hash_init_t *hinit, ngx_hash_ಕೀಲಿ_t *names, ngx_uint_t nelts)
{
    u_char          *elts;
    size_t           len;
    u_short         *test;
    ngx_uint_t       i, n, ಕೀಲಿ, size, start, bucket_size;
    ngx_hash_elt_t  *elt, **buckets;

    ಯದಿ (hinit->max_size == 0) {
        ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                      "could not build %s, you should "
                      "increase %s_max_size: %i",
                      hinit->name, hinit->name, hinit->max_size);
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    ಯದಿ (hinit->bucket_size > 65536 - ngx_cacheline_size) {
        ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                      "could not build %s, too large "
                      "%s_bucket_size: %i",
                      hinit->name, hinit->name, hinit->bucket_size);
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    ಪ್ರತಿ (n = 0; n < nelts; n++) {
        ಯದಿ (names[n].ಕೀಲಿ.data == ರಿಕ್ತ) {
            ಅನುವರ್ತ;
        }

        ಯದಿ (hinit->bucket_size < NGX_HASH_ELT_SIZE(&names[n]) + ಪರಿಮಾಣ(ಶೂನ್ಯ *))
        {
            ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                          "could not build %s, you should "
                          "increase %s_bucket_size: %i",
                          hinit->name, hinit->name, hinit->bucket_size);
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }
    }

    test = ngx_alloc(hinit->max_size * ಪರಿಮಾಣ(u_short), hinit->pool->log);
    ಯದಿ (test == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    bucket_size = hinit->bucket_size - ಪರಿಮಾಣ(ಶೂನ್ಯ *);

    start = nelts / (bucket_size / (2 * ಪರಿಮಾಣ(ಶೂನ್ಯ *)));
    start = start ? start : 1;

    ಯದಿ (hinit->max_size > 10000 && nelts && hinit->max_size / nelts < 100) {
        start = hinit->max_size - 1000;
    }

    ಪ್ರತಿ (size = start; size <= hinit->max_size; size++) {

        ngx_memzero(test, size * ಪರಿಮಾಣ(u_short));

        ಪ್ರತಿ (n = 0; n < nelts; n++) {
            ಯದಿ (names[n].ಕೀಲಿ.data == ರಿಕ್ತ) {
                ಅನುವರ್ತ;
            }

            ಕೀಲಿ = names[n].ಕೀಲಿ_hash % size;
            len = test[ಕೀಲಿ] + NGX_HASH_ELT_SIZE(&names[n]);

#ಯದಿ 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "%ui: %ui %uz \"%V\"",
                          size, ಕೀಲಿ, len, &names[n].ಕೀಲಿ);
#endif

            ಯದಿ (len > bucket_size) {
                ಗಚ್ಛ next;
            }

            test[ಕೀಲಿ] = (u_short) len;
        }

        ಗಚ್ಛ found;

    next:

        ಅನುವರ್ತ;
    }

    size = hinit->max_size;

    ngx_log_error(NGX_LOG_WARN, hinit->pool->log, 0,
                  "could not build optimal %s, you should increase "
                  "either %s_max_size: %i or %s_bucket_size: %i; "
                  "ignoring %s_bucket_size",
                  hinit->name, hinit->name, hinit->max_size,
                  hinit->name, hinit->bucket_size, hinit->name);

found:

    ಪ್ರತಿ (i = 0; i < size; i++) {
        test[i] = ಪರಿಮಾಣ(ಶೂನ್ಯ *);
    }

    ಪ್ರತಿ (n = 0; n < nelts; n++) {
        ಯದಿ (names[n].ಕೀಲಿ.data == ರಿಕ್ತ) {
            ಅನುವರ್ತ;
        }

        ಕೀಲಿ = names[n].ಕೀಲಿ_hash % size;
        len = test[ಕೀಲಿ] + NGX_HASH_ELT_SIZE(&names[n]);

        ಯದಿ (len > 65536 - ngx_cacheline_size) {
            ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                          "could not build %s, you should "
                          "increase %s_max_size: %i",
                          hinit->name, hinit->name, hinit->max_size);
            ngx_free(test);
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        test[ಕೀಲಿ] = (u_short) len;
    }

    len = 0;

    ಪ್ರತಿ (i = 0; i < size; i++) {
        ಯದಿ (test[i] == ಪರಿಮಾಣ(ಶೂನ್ಯ *)) {
            ಅನುವರ್ತ;
        }

        test[i] = (u_short) (ngx_align(test[i], ngx_cacheline_size));

        len += test[i];
    }

    ಯದಿ (hinit->hash == ರಿಕ್ತ) {
        hinit->hash = ngx_pcalloc(hinit->pool, ಪರಿಮಾಣ(ngx_hash_wildcard_t)
                                             + size * ಪರಿಮಾಣ(ngx_hash_elt_t *));
        ಯದಿ (hinit->hash == ರಿಕ್ತ) {
            ngx_free(test);
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        buckets = (ngx_hash_elt_t **)
                      ((u_char *) hinit->hash + ಪರಿಮಾಣ(ngx_hash_wildcard_t));

    } ಅನ್ಯಥಾ {
        buckets = ngx_pcalloc(hinit->pool, size * ಪರಿಮಾಣ(ngx_hash_elt_t *));
        ಯದಿ (buckets == ರಿಕ್ತ) {
            ngx_free(test);
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }
    }

    elts = ngx_palloc(hinit->pool, len + ngx_cacheline_size);
    ಯದಿ (elts == ರಿಕ್ತ) {
        ngx_free(test);
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    elts = ngx_align_ptr(elts, ngx_cacheline_size);

    ಪ್ರತಿ (i = 0; i < size; i++) {
        ಯದಿ (test[i] == ಪರಿಮಾಣ(ಶೂನ್ಯ *)) {
            ಅನುವರ್ತ;
        }

        buckets[i] = (ngx_hash_elt_t *) elts;
        elts += test[i];
    }

    ಪ್ರತಿ (i = 0; i < size; i++) {
        test[i] = 0;
    }

    ಪ್ರತಿ (n = 0; n < nelts; n++) {
        ಯದಿ (names[n].ಕೀಲಿ.data == ರಿಕ್ತ) {
            ಅನುವರ್ತ;
        }

        ಕೀಲಿ = names[n].ಕೀಲಿ_hash % size;
        elt = (ngx_hash_elt_t *) ((u_char *) buckets[ಕೀಲಿ] + test[ಕೀಲಿ]);

        elt->ಮೌಲ್ಯ = names[n].ಮೌಲ್ಯ;
        elt->len = (u_short) names[n].ಕೀಲಿ.len;

        ngx_strlow(elt->name, names[n].ಕೀಲಿ.data, names[n].ಕೀಲಿ.len);

        test[ಕೀಲಿ] = (u_short) (test[ಕೀಲಿ] + NGX_HASH_ELT_SIZE(&names[n]));
    }

    ಪ್ರತಿ (i = 0; i < size; i++) {
        ಯದಿ (buckets[i] == ರಿಕ್ತ) {
            ಅನುವರ್ತ;
        }

        elt = (ngx_hash_elt_t *) ((u_char *) buckets[i] + test[i]);

        elt->ಮೌಲ್ಯ = ರಿಕ್ತ;
    }

    ngx_free(test);

    hinit->hash->buckets = buckets;
    hinit->hash->size = size;

#ಯದಿ 0

    ಪ್ರತಿ (i = 0; i < size; i++) {
        ngx_str_t   val;
        ngx_uint_t  ಕೀಲಿ;

        elt = buckets[i];

        ಯದಿ (elt == ರಿಕ್ತ) {
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "%ui: NULL", i);
            ಅನುವರ್ತ;
        }

        ಯಾವತ್ (elt->ಮೌಲ್ಯ) {
            val.len = elt->len;
            val.data = &elt->name[0];

            ಕೀಲಿ = hinit->ಕೀಲಿ(val.data, val.len);

            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "%ui: %p \"%V\" %ui", i, elt, &val, ಕೀಲಿ);

            elt = (ngx_hash_elt_t *) ngx_align_ptr(&elt->name[0] + elt->len,
                                                   ಪರಿಮಾಣ(ಶೂನ್ಯ *));
        }
    }

#endif

    ಪ್ರತ್ಯಾಗಮ NGX_OK;
}


ngx_int_t
ngx_hash_wildcard_init(ngx_hash_init_t *hinit, ngx_hash_ಕೀಲಿ_t *names,
    ngx_uint_t nelts)
{
    size_t                len, dot_len;
    ngx_uint_t            i, n, dot;
    ngx_array_t           curr_names, next_names;
    ngx_hash_ಕೀಲಿ_t       *name, *next_name;
    ngx_hash_init_t       h;
    ngx_hash_wildcard_t  *wdc;

    ಯದಿ (ngx_array_init(&curr_names, hinit->temp_pool, nelts,
                       ಪರಿಮಾಣ(ngx_hash_ಕೀಲಿ_t))
        != NGX_OK)
    {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    ಯದಿ (ngx_array_init(&next_names, hinit->temp_pool, nelts,
                       ಪರಿಮಾಣ(ngx_hash_ಕೀಲಿ_t))
        != NGX_OK)
    {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    ಪ್ರತಿ (n = 0; n < nelts; n = i) {

#ಯದಿ 0
        ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                      "wc0: \"%V\"", &names[n].ಕೀಲಿ);
#endif

        dot = 0;

        ಪ್ರತಿ (len = 0; len < names[n].ಕೀಲಿ.len; len++) {
            ಯದಿ (names[n].ಕೀಲಿ.data[len] == '.') {
                dot = 1;
                ಭಙ್ಗ;
            }
        }

        name = ngx_array_ತಳ್ಳು(&curr_names);
        ಯದಿ (name == ರಿಕ್ತ) {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        name->ಕೀಲಿ.len = len;
        name->ಕೀಲಿ.data = names[n].ಕೀಲಿ.data;
        name->ಕೀಲಿ_hash = hinit->ಕೀಲಿ(name->ಕೀಲಿ.data, name->ಕೀಲಿ.len);
        name->ಮೌಲ್ಯ = names[n].ಮೌಲ್ಯ;

#ಯದಿ 0
        ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                      "wc1: \"%V\" %ui", &name->ಕೀಲಿ, dot);
#endif

        dot_len = len + 1;

        ಯದಿ (dot) {
            len++;
        }

        next_names.nelts = 0;

        ಯದಿ (names[n].ಕೀಲಿ.len != len) {
            next_name = ngx_array_ತಳ್ಳು(&next_names);
            ಯದಿ (next_name == ರಿಕ್ತ) {
                ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
            }

            next_name->ಕೀಲಿ.len = names[n].ಕೀಲಿ.len - len;
            next_name->ಕೀಲಿ.data = names[n].ಕೀಲಿ.data + len;
            next_name->ಕೀಲಿ_hash = 0;
            next_name->ಮೌಲ್ಯ = names[n].ಮೌಲ್ಯ;

#ಯದಿ 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "wc2: \"%V\"", &next_name->ಕೀಲಿ);
#endif
        }

        ಪ್ರತಿ (i = n + 1; i < nelts; i++) {
            ಯದಿ (ngx_strncmp(names[n].ಕೀಲಿ.data, names[i].ಕೀಲಿ.data, len) != 0) {
                ಭಙ್ಗ;
            }

            ಯದಿ (!dot
                && names[i].ಕೀಲಿ.len > len
                && names[i].ಕೀಲಿ.data[len] != '.')
            {
                ಭಙ್ಗ;
            }

            next_name = ngx_array_ತಳ್ಳು(&next_names);
            ಯದಿ (next_name == ರಿಕ್ತ) {
                ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
            }

            next_name->ಕೀಲಿ.len = names[i].ಕೀಲಿ.len - dot_len;
            next_name->ಕೀಲಿ.data = names[i].ಕೀಲಿ.data + dot_len;
            next_name->ಕೀಲಿ_hash = 0;
            next_name->ಮೌಲ್ಯ = names[i].ಮೌಲ್ಯ;

#ಯದಿ 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "wc3: \"%V\"", &next_name->ಕೀಲಿ);
#endif
        }

        ಯದಿ (next_names.nelts) {

            h = *hinit;
            h.hash = ರಿಕ್ತ;

            ಯದಿ (ngx_hash_wildcard_init(&h, (ngx_hash_ಕೀಲಿ_t *) next_names.elts,
                                       next_names.nelts)
                != NGX_OK)
            {
                ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
            }

            wdc = (ngx_hash_wildcard_t *) h.hash;

            ಯದಿ (names[n].ಕೀಲಿ.len == len) {
                wdc->ಮೌಲ್ಯ = names[n].ಮೌಲ್ಯ;
            }

            name->ಮೌಲ್ಯ = (ಶೂನ್ಯ *) ((uintptr_t) wdc | (dot ? 3 : 2));

        } ಅನ್ಯಥಾ ಯದಿ (dot) {
            name->ಮೌಲ್ಯ = (ಶೂನ್ಯ *) ((uintptr_t) name->ಮೌಲ್ಯ | 1);
        }
    }

    ಯದಿ (ngx_hash_init(hinit, (ngx_hash_ಕೀಲಿ_t *) curr_names.elts,
                      curr_names.nelts)
        != NGX_OK)
    {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    ಪ್ರತ್ಯಾಗಮ NGX_OK;
}


ngx_uint_t
ngx_hash_ಕೀಲಿ(u_char *data, size_t len)
{
    ngx_uint_t  i, ಕೀಲಿ;

    ಕೀಲಿ = 0;

    ಪ್ರತಿ (i = 0; i < len; i++) {
        ಕೀಲಿ = ngx_hash(ಕೀಲಿ, data[i]);
    }

    ಪ್ರತ್ಯಾಗಮ ಕೀಲಿ;
}


ngx_uint_t
ngx_hash_ಕೀಲಿ_lc(u_char *data, size_t len)
{
    ngx_uint_t  i, ಕೀಲಿ;

    ಕೀಲಿ = 0;

    ಪ್ರತಿ (i = 0; i < len; i++) {
        ಕೀಲಿ = ngx_hash(ಕೀಲಿ, ngx_tolower(data[i]));
    }

    ಪ್ರತ್ಯಾಗಮ ಕೀಲಿ;
}


ngx_uint_t
ngx_hash_strlow(u_char *dst, u_char *src, size_t n)
{
    ngx_uint_t  ಕೀಲಿ;

    ಕೀಲಿ = 0;

    ಯಾವತ್ (n--) {
        *dst = ngx_tolower(*src);
        ಕೀಲಿ = ngx_hash(ಕೀಲಿ, *dst);
        dst++;
        src++;
    }

    ಪ್ರತ್ಯಾಗಮ ಕೀಲಿ;
}


ngx_int_t
ngx_hash_ಕೀಲಿಗಳು_array_init(ngx_hash_ಕೀಲಿಗಳು_arrays_t *ha, ngx_uint_t type)
{
    ngx_uint_t  asize;

    ಯದಿ (type == NGX_HASH_SMALL) {
        asize = 4;
        ha->hsize = 107;

    } ಅನ್ಯಥಾ {
        asize = NGX_HASH_LARGE_ASIZE;
        ha->hsize = NGX_HASH_LARGE_HSIZE;
    }

    ಯದಿ (ngx_array_init(&ha->ಕೀಲಿಗಳು, ha->temp_pool, asize, ಪರಿಮಾಣ(ngx_hash_ಕೀಲಿ_t))
        != NGX_OK)
    {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    ಯದಿ (ngx_array_init(&ha->dns_wc_head, ha->temp_pool, asize,
                       ಪರಿಮಾಣ(ngx_hash_ಕೀಲಿ_t))
        != NGX_OK)
    {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    ಯದಿ (ngx_array_init(&ha->dns_wc_tail, ha->temp_pool, asize,
                       ಪರಿಮಾಣ(ngx_hash_ಕೀಲಿ_t))
        != NGX_OK)
    {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    ha->ಕೀಲಿಗಳು_hash = ngx_pcalloc(ha->temp_pool, ಪರಿಮಾಣ(ngx_array_t) * ha->hsize);
    ಯದಿ (ha->ಕೀಲಿಗಳು_hash == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    ha->dns_wc_head_hash = ngx_pcalloc(ha->temp_pool,
                                       ಪರಿಮಾಣ(ngx_array_t) * ha->hsize);
    ಯದಿ (ha->dns_wc_head_hash == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    ha->dns_wc_tail_hash = ngx_pcalloc(ha->temp_pool,
                                       ಪರಿಮಾಣ(ngx_array_t) * ha->hsize);
    ಯದಿ (ha->dns_wc_tail_hash == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    ಪ್ರತ್ಯಾಗಮ NGX_OK;
}


ngx_int_t
ngx_hash_add_ಕೀಲಿ(ngx_hash_ಕೀಲಿಗಳು_arrays_t *ha, ngx_str_t *ಕೀಲಿ, ಶೂನ್ಯ *ಮೌಲ್ಯ,
    ngx_uint_t flags)
{
    size_t           len;
    u_char          *p;
    ngx_str_t       *name;
    ngx_uint_t       i, k, n, skip, last;
    ngx_array_t     *ಕೀಲಿಗಳು, *hwc;
    ngx_hash_ಕೀಲಿ_t  *hk;

    last = ಕೀಲಿ->len;

    ಯದಿ (flags & NGX_HASH_WILDCARD_ಕೀಲಿ) {

        /*
         * supported wildcards:
         *     "*.example.com", ".example.com", and "www.example.*"
         */

        n = 0;

        ಪ್ರತಿ (i = 0; i < ಕೀಲಿ->len; i++) {

            ಯದಿ (ಕೀಲಿ->data[i] == '*') {
                ಯದಿ (++n > 1) {
                    ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;
                }
            }

            ಯದಿ (ಕೀಲಿ->data[i] == '.' && ಕೀಲಿ->data[i + 1] == '.') {
                ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;
            }

            ಯದಿ (ಕೀಲಿ->data[i] == '\0') {
                ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;
            }
        }

        ಯದಿ (ಕೀಲಿ->len > 1 && ಕೀಲಿ->data[0] == '.') {
            skip = 1;
            ಗಚ್ಛ wildcard;
        }

        ಯದಿ (ಕೀಲಿ->len > 2) {

            ಯದಿ (ಕೀಲಿ->data[0] == '*' && ಕೀಲಿ->data[1] == '.') {
                skip = 2;
                ಗಚ್ಛ wildcard;
            }

            ಯದಿ (ಕೀಲಿ->data[i - 2] == '.' && ಕೀಲಿ->data[i - 1] == '*') {
                skip = 0;
                last -= 2;
                ಗಚ್ಛ wildcard;
            }
        }

        ಯದಿ (n) {
            ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;
        }
    }

    /* exact hash */

    k = 0;

    ಪ್ರತಿ (i = 0; i < last; i++) {
        ಯದಿ (!(flags & NGX_HASH_READONLY_ಕೀಲಿ)) {
            ಕೀಲಿ->data[i] = ngx_tolower(ಕೀಲಿ->data[i]);
        }
        k = ngx_hash(k, ಕೀಲಿ->data[i]);
    }

    k %= ha->hsize;

    /* check conflicts in exact hash */

    name = ha->ಕೀಲಿಗಳು_hash[k].elts;

    ಯದಿ (name) {
        ಪ್ರತಿ (i = 0; i < ha->ಕೀಲಿಗಳು_hash[k].nelts; i++) {
            ಯದಿ (last != name[i].len) {
                ಅನುವರ್ತ;
            }

            ಯದಿ (ngx_strncmp(ಕೀಲಿ->data, name[i].data, last) == 0) {
                ಪ್ರತ್ಯಾಗಮ NGX_BUSY;
            }
        }

    } ಅನ್ಯಥಾ {
        ಯದಿ (ngx_array_init(&ha->ಕೀಲಿಗಳು_hash[k], ha->temp_pool, 4,
                           ಪರಿಮಾಣ(ngx_str_t))
            != NGX_OK)
        {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }
    }

    name = ngx_array_ತಳ್ಳು(&ha->ಕೀಲಿಗಳು_hash[k]);
    ಯದಿ (name == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    *name = *ಕೀಲಿ;

    hk = ngx_array_ತಳ್ಳು(&ha->ಕೀಲಿಗಳು);
    ಯದಿ (hk == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    hk->ಕೀಲಿ = *ಕೀಲಿ;
    hk->ಕೀಲಿ_hash = ngx_hash_ಕೀಲಿ(ಕೀಲಿ->data, last);
    hk->ಮೌಲ್ಯ = ಮೌಲ್ಯ;

    ಪ್ರತ್ಯಾಗಮ NGX_OK;


wildcard:

    /* wildcard hash */

    k = ngx_hash_strlow(&ಕೀಲಿ->data[skip], &ಕೀಲಿ->data[skip], last - skip);

    k %= ha->hsize;

    ಯದಿ (skip == 1) {

        /* check conflicts in exact hash for ".example.com" */

        name = ha->ಕೀಲಿಗಳು_hash[k].elts;

        ಯದಿ (name) {
            len = last - skip;

            ಪ್ರತಿ (i = 0; i < ha->ಕೀಲಿಗಳು_hash[k].nelts; i++) {
                ಯದಿ (len != name[i].len) {
                    ಅನುವರ್ತ;
                }

                ಯದಿ (ngx_strncmp(&ಕೀಲಿ->data[1], name[i].data, len) == 0) {
                    ಪ್ರತ್ಯಾಗಮ NGX_BUSY;
                }
            }

        } ಅನ್ಯಥಾ {
            ಯದಿ (ngx_array_init(&ha->ಕೀಲಿಗಳು_hash[k], ha->temp_pool, 4,
                               ಪರಿಮಾಣ(ngx_str_t))
                != NGX_OK)
            {
                ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
            }
        }

        name = ngx_array_ತಳ್ಳು(&ha->ಕೀಲಿಗಳು_hash[k]);
        ಯದಿ (name == ರಿಕ್ತ) {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        name->len = last - 1;
        name->data = ngx_pnalloc(ha->temp_pool, name->len);
        ಯದಿ (name->data == ರಿಕ್ತ) {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        ngx_memcpy(name->data, &ಕೀಲಿ->data[1], name->len);
    }


    ಯದಿ (skip) {

        /*
         * convert "*.example.com" to "com.example.\0"
         *      and ".example.com" to "com.example\0"
         */

        p = ngx_pnalloc(ha->temp_pool, last);
        ಯದಿ (p == ರಿಕ್ತ) {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        len = 0;
        n = 0;

        ಪ್ರತಿ (i = last - 1; i; i--) {
            ಯದಿ (ಕೀಲಿ->data[i] == '.') {
                ngx_memcpy(&p[n], &ಕೀಲಿ->data[i + 1], len);
                n += len;
                p[n++] = '.';
                len = 0;
                ಅನುವರ್ತ;
            }

            len++;
        }

        ಯದಿ (len) {
            ngx_memcpy(&p[n], &ಕೀಲಿ->data[1], len);
            n += len;
        }

        p[n] = '\0';

        hwc = &ha->dns_wc_head;
        ಕೀಲಿಗಳು = &ha->dns_wc_head_hash[k];

    } ಅನ್ಯಥಾ {

        /* convert "www.example.*" to "www.example\0" */

        last++;

        p = ngx_pnalloc(ha->temp_pool, last);
        ಯದಿ (p == ರಿಕ್ತ) {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        ngx_cpystrn(p, ಕೀಲಿ->data, last);

        hwc = &ha->dns_wc_tail;
        ಕೀಲಿಗಳು = &ha->dns_wc_tail_hash[k];
    }


    /* check conflicts in wildcard hash */

    name = ಕೀಲಿಗಳು->elts;

    ಯದಿ (name) {
        len = last - skip;

        ಪ್ರತಿ (i = 0; i < ಕೀಲಿಗಳು->nelts; i++) {
            ಯದಿ (len != name[i].len) {
                ಅನುವರ್ತ;
            }

            ಯದಿ (ngx_strncmp(ಕೀಲಿ->data + skip, name[i].data, len) == 0) {
                ಪ್ರತ್ಯಾಗಮ NGX_BUSY;
            }
        }

    } ಅನ್ಯಥಾ {
        ಯದಿ (ngx_array_init(ಕೀಲಿಗಳು, ha->temp_pool, 4, ಪರಿಮಾಣ(ngx_str_t)) != NGX_OK)
        {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }
    }

    name = ngx_array_ತಳ್ಳು(ಕೀಲಿಗಳು);
    ಯದಿ (name == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    name->len = last - skip;
    name->data = ngx_pnalloc(ha->temp_pool, name->len);
    ಯದಿ (name->data == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    ngx_memcpy(name->data, ಕೀಲಿ->data + skip, name->len);


    /* add to wildcard hash */

    hk = ngx_array_ತಳ್ಳು(hwc);
    ಯದಿ (hk == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    hk->ಕೀಲಿ.len = last - 1;
    hk->ಕೀಲಿ.data = p;
    hk->ಕೀಲಿ_hash = 0;
    hk->ಮೌಲ್ಯ = ಮೌಲ್ಯ;

    ಪ್ರತ್ಯಾಗಮ NGX_OK;
}
