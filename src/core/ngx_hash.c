
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


शून्य *
ngx_hash_find(ngx_hash_t *hash, ngx_uint_t किल्ली, u_char *name, size_t len)
{
    ngx_uint_t       i;
    ngx_hash_elt_t  *elt;

#यदि 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "hf:\"%*s\"", len, name);
#endif

    elt = hash->buckets[किल्ली % hash->size];

    यदि (elt == रिक्त) {
        प्रत्यागम रिक्त;
    }

    यावत् (elt->मूल्य) {
        यदि (len != (size_t) elt->len) {
            गच्छ next;
        }

        प्रति (i = 0; i < len; i++) {
            यदि (name[i] != elt->name[i]) {
                गच्छ next;
            }
        }

        प्रत्यागम elt->मूल्य;

    next:

        elt = (ngx_hash_elt_t *) ngx_align_ptr(&elt->name[0] + elt->len,
                                               परिमाण(शून्य *));
        अनुवर्त;
    }

    प्रत्यागम रिक्त;
}


शून्य *
ngx_hash_find_wc_head(ngx_hash_wildcard_t *hwc, u_char *name, size_t len)
{
    शून्य        *मूल्य;
    ngx_uint_t   i, n, किल्ली;

#यदि 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "wch:\"%*s\"", len, name);
#endif

    n = len;

    यावत् (n) {
        यदि (name[n - 1] == '.') {
            भङ्ग;
        }

        n--;
    }

    किल्ली = 0;

    प्रति (i = n; i < len; i++) {
        किल्ली = ngx_hash(किल्ली, name[i]);
    }

#यदि 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "key:\"%ui\"", किल्ली);
#endif

    मूल्य = ngx_hash_find(&hwc->hash, किल्ली, &name[n], len - n);

#यदि 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "value:\"%p\"", मूल्य);
#endif

    यदि (मूल्य) {

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

        यदि ((uintptr_t) मूल्य & 2) {

            यदि (n == 0) {

                /* "example.com" */

                यदि ((uintptr_t) मूल्य & 1) {
                    प्रत्यागम रिक्त;
                }

                hwc = (ngx_hash_wildcard_t *)
                                          ((uintptr_t) मूल्य & (uintptr_t) ~3);
                प्रत्यागम hwc->मूल्य;
            }

            hwc = (ngx_hash_wildcard_t *) ((uintptr_t) मूल्य & (uintptr_t) ~3);

            मूल्य = ngx_hash_find_wc_head(hwc, name, n - 1);

            यदि (मूल्य) {
                प्रत्यागम मूल्य;
            }

            प्रत्यागम hwc->मूल्य;
        }

        यदि ((uintptr_t) मूल्य & 1) {

            यदि (n == 0) {

                /* "example.com" */

                प्रत्यागम रिक्त;
            }

            प्रत्यागम (शून्य *) ((uintptr_t) मूल्य & (uintptr_t) ~3);
        }

        प्रत्यागम मूल्य;
    }

    प्रत्यागम hwc->मूल्य;
}


शून्य *
ngx_hash_find_wc_tail(ngx_hash_wildcard_t *hwc, u_char *name, size_t len)
{
    शून्य        *मूल्य;
    ngx_uint_t   i, किल्ली;

#यदि 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "wct:\"%*s\"", len, name);
#endif

    किल्ली = 0;

    प्रति (i = 0; i < len; i++) {
        यदि (name[i] == '.') {
            भङ्ग;
        }

        किल्ली = ngx_hash(किल्ली, name[i]);
    }

    यदि (i == len) {
        प्रत्यागम रिक्त;
    }

#यदि 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "key:\"%ui\"", किल्ली);
#endif

    मूल्य = ngx_hash_find(&hwc->hash, किल्ली, name, i);

#यदि 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "value:\"%p\"", मूल्य);
#endif

    यदि (मूल्य) {

        /*
         * the 2 low bits of value have the special meaning:
         *     00 - value is data pointer;
         *     11 - value is pointer to wildcard hash allowing "example.*".
         */

        यदि ((uintptr_t) मूल्य & 2) {

            i++;

            hwc = (ngx_hash_wildcard_t *) ((uintptr_t) मूल्य & (uintptr_t) ~3);

            मूल्य = ngx_hash_find_wc_tail(hwc, &name[i], len - i);

            यदि (मूल्य) {
                प्रत्यागम मूल्य;
            }

            प्रत्यागम hwc->मूल्य;
        }

        प्रत्यागम मूल्य;
    }

    प्रत्यागम hwc->मूल्य;
}


शून्य *
ngx_hash_find_combined(ngx_hash_combined_t *hash, ngx_uint_t किल्ली, u_char *name,
    size_t len)
{
    शून्य  *मूल्य;

    यदि (hash->hash.buckets) {
        मूल्य = ngx_hash_find(&hash->hash, किल्ली, name, len);

        यदि (मूल्य) {
            प्रत्यागम मूल्य;
        }
    }

    यदि (len == 0) {
        प्रत्यागम रिक्त;
    }

    यदि (hash->wc_head && hash->wc_head->hash.buckets) {
        मूल्य = ngx_hash_find_wc_head(hash->wc_head, name, len);

        यदि (मूल्य) {
            प्रत्यागम मूल्य;
        }
    }

    यदि (hash->wc_tail && hash->wc_tail->hash.buckets) {
        मूल्य = ngx_hash_find_wc_tail(hash->wc_tail, name, len);

        यदि (मूल्य) {
            प्रत्यागम मूल्य;
        }
    }

    प्रत्यागम रिक्त;
}


#define NGX_HASH_ELT_SIZE(name)                                               \
    (परिमाण(शून्य *) + ngx_align((name)->किल्ली.len + 2, परिमाण(शून्य *)))

ngx_int_t
ngx_hash_init(ngx_hash_init_t *hinit, ngx_hash_किल्ली_t *names, ngx_uint_t nelts)
{
    u_char          *elts;
    size_t           len;
    u_short         *test;
    ngx_uint_t       i, n, किल्ली, size, start, bucket_size;
    ngx_hash_elt_t  *elt, **buckets;

    यदि (hinit->max_size == 0) {
        ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                      "could not build %s, you should "
                      "increase %s_max_size: %i",
                      hinit->name, hinit->name, hinit->max_size);
        प्रत्यागम NGX_ERROR;
    }

    यदि (hinit->bucket_size > 65536 - ngx_cacheline_size) {
        ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                      "could not build %s, too large "
                      "%s_bucket_size: %i",
                      hinit->name, hinit->name, hinit->bucket_size);
        प्रत्यागम NGX_ERROR;
    }

    प्रति (n = 0; n < nelts; n++) {
        यदि (names[n].किल्ली.data == रिक्त) {
            अनुवर्त;
        }

        यदि (hinit->bucket_size < NGX_HASH_ELT_SIZE(&names[n]) + परिमाण(शून्य *))
        {
            ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                          "could not build %s, you should "
                          "increase %s_bucket_size: %i",
                          hinit->name, hinit->name, hinit->bucket_size);
            प्रत्यागम NGX_ERROR;
        }
    }

    test = ngx_alloc(hinit->max_size * परिमाण(u_short), hinit->pool->log);
    यदि (test == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

    bucket_size = hinit->bucket_size - परिमाण(शून्य *);

    start = nelts / (bucket_size / (2 * परिमाण(शून्य *)));
    start = start ? start : 1;

    यदि (hinit->max_size > 10000 && nelts && hinit->max_size / nelts < 100) {
        start = hinit->max_size - 1000;
    }

    प्रति (size = start; size <= hinit->max_size; size++) {

        ngx_memzero(test, size * परिमाण(u_short));

        प्रति (n = 0; n < nelts; n++) {
            यदि (names[n].किल्ली.data == रिक्त) {
                अनुवर्त;
            }

            किल्ली = names[n].किल्ली_hash % size;
            len = test[किल्ली] + NGX_HASH_ELT_SIZE(&names[n]);

#यदि 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "%ui: %ui %uz \"%V\"",
                          size, किल्ली, len, &names[n].किल्ली);
#endif

            यदि (len > bucket_size) {
                गच्छ next;
            }

            test[किल्ली] = (u_short) len;
        }

        गच्छ found;

    next:

        अनुवर्त;
    }

    size = hinit->max_size;

    ngx_log_error(NGX_LOG_WARN, hinit->pool->log, 0,
                  "could not build optimal %s, you should increase "
                  "either %s_max_size: %i or %s_bucket_size: %i; "
                  "ignoring %s_bucket_size",
                  hinit->name, hinit->name, hinit->max_size,
                  hinit->name, hinit->bucket_size, hinit->name);

found:

    प्रति (i = 0; i < size; i++) {
        test[i] = परिमाण(शून्य *);
    }

    प्रति (n = 0; n < nelts; n++) {
        यदि (names[n].किल्ली.data == रिक्त) {
            अनुवर्त;
        }

        किल्ली = names[n].किल्ली_hash % size;
        len = test[किल्ली] + NGX_HASH_ELT_SIZE(&names[n]);

        यदि (len > 65536 - ngx_cacheline_size) {
            ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                          "could not build %s, you should "
                          "increase %s_max_size: %i",
                          hinit->name, hinit->name, hinit->max_size);
            ngx_free(test);
            प्रत्यागम NGX_ERROR;
        }

        test[किल्ली] = (u_short) len;
    }

    len = 0;

    प्रति (i = 0; i < size; i++) {
        यदि (test[i] == परिमाण(शून्य *)) {
            अनुवर्त;
        }

        test[i] = (u_short) (ngx_align(test[i], ngx_cacheline_size));

        len += test[i];
    }

    यदि (hinit->hash == रिक्त) {
        hinit->hash = ngx_pcalloc(hinit->pool, परिमाण(ngx_hash_wildcard_t)
                                             + size * परिमाण(ngx_hash_elt_t *));
        यदि (hinit->hash == रिक्त) {
            ngx_free(test);
            प्रत्यागम NGX_ERROR;
        }

        buckets = (ngx_hash_elt_t **)
                      ((u_char *) hinit->hash + परिमाण(ngx_hash_wildcard_t));

    } अन्यथा {
        buckets = ngx_pcalloc(hinit->pool, size * परिमाण(ngx_hash_elt_t *));
        यदि (buckets == रिक्त) {
            ngx_free(test);
            प्रत्यागम NGX_ERROR;
        }
    }

    elts = ngx_palloc(hinit->pool, len + ngx_cacheline_size);
    यदि (elts == रिक्त) {
        ngx_free(test);
        प्रत्यागम NGX_ERROR;
    }

    elts = ngx_align_ptr(elts, ngx_cacheline_size);

    प्रति (i = 0; i < size; i++) {
        यदि (test[i] == परिमाण(शून्य *)) {
            अनुवर्त;
        }

        buckets[i] = (ngx_hash_elt_t *) elts;
        elts += test[i];
    }

    प्रति (i = 0; i < size; i++) {
        test[i] = 0;
    }

    प्रति (n = 0; n < nelts; n++) {
        यदि (names[n].किल्ली.data == रिक्त) {
            अनुवर्त;
        }

        किल्ली = names[n].किल्ली_hash % size;
        elt = (ngx_hash_elt_t *) ((u_char *) buckets[किल्ली] + test[किल्ली]);

        elt->मूल्य = names[n].मूल्य;
        elt->len = (u_short) names[n].किल्ली.len;

        ngx_strlow(elt->name, names[n].किल्ली.data, names[n].किल्ली.len);

        test[किल्ली] = (u_short) (test[किल्ली] + NGX_HASH_ELT_SIZE(&names[n]));
    }

    प्रति (i = 0; i < size; i++) {
        यदि (buckets[i] == रिक्त) {
            अनुवर्त;
        }

        elt = (ngx_hash_elt_t *) ((u_char *) buckets[i] + test[i]);

        elt->मूल्य = रिक्त;
    }

    ngx_free(test);

    hinit->hash->buckets = buckets;
    hinit->hash->size = size;

#यदि 0

    प्रति (i = 0; i < size; i++) {
        ngx_str_t   val;
        ngx_uint_t  किल्ली;

        elt = buckets[i];

        यदि (elt == रिक्त) {
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "%ui: NULL", i);
            अनुवर्त;
        }

        यावत् (elt->मूल्य) {
            val.len = elt->len;
            val.data = &elt->name[0];

            किल्ली = hinit->किल्ली(val.data, val.len);

            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "%ui: %p \"%V\" %ui", i, elt, &val, किल्ली);

            elt = (ngx_hash_elt_t *) ngx_align_ptr(&elt->name[0] + elt->len,
                                                   परिमाण(शून्य *));
        }
    }

#endif

    प्रत्यागम NGX_OK;
}


ngx_int_t
ngx_hash_wildcard_init(ngx_hash_init_t *hinit, ngx_hash_किल्ली_t *names,
    ngx_uint_t nelts)
{
    size_t                len, dot_len;
    ngx_uint_t            i, n, dot;
    ngx_array_t           curr_names, next_names;
    ngx_hash_किल्ली_t       *name, *next_name;
    ngx_hash_init_t       h;
    ngx_hash_wildcard_t  *wdc;

    यदि (ngx_array_init(&curr_names, hinit->temp_pool, nelts,
                       परिमाण(ngx_hash_किल्ली_t))
        != NGX_OK)
    {
        प्रत्यागम NGX_ERROR;
    }

    यदि (ngx_array_init(&next_names, hinit->temp_pool, nelts,
                       परिमाण(ngx_hash_किल्ली_t))
        != NGX_OK)
    {
        प्रत्यागम NGX_ERROR;
    }

    प्रति (n = 0; n < nelts; n = i) {

#यदि 0
        ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                      "wc0: \"%V\"", &names[n].किल्ली);
#endif

        dot = 0;

        प्रति (len = 0; len < names[n].किल्ली.len; len++) {
            यदि (names[n].किल्ली.data[len] == '.') {
                dot = 1;
                भङ्ग;
            }
        }

        name = ngx_array_ढकला(&curr_names);
        यदि (name == रिक्त) {
            प्रत्यागम NGX_ERROR;
        }

        name->किल्ली.len = len;
        name->किल्ली.data = names[n].किल्ली.data;
        name->किल्ली_hash = hinit->किल्ली(name->किल्ली.data, name->किल्ली.len);
        name->मूल्य = names[n].मूल्य;

#यदि 0
        ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                      "wc1: \"%V\" %ui", &name->किल्ली, dot);
#endif

        dot_len = len + 1;

        यदि (dot) {
            len++;
        }

        next_names.nelts = 0;

        यदि (names[n].किल्ली.len != len) {
            next_name = ngx_array_ढकला(&next_names);
            यदि (next_name == रिक्त) {
                प्रत्यागम NGX_ERROR;
            }

            next_name->किल्ली.len = names[n].किल्ली.len - len;
            next_name->किल्ली.data = names[n].किल्ली.data + len;
            next_name->किल्ली_hash = 0;
            next_name->मूल्य = names[n].मूल्य;

#यदि 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "wc2: \"%V\"", &next_name->किल्ली);
#endif
        }

        प्रति (i = n + 1; i < nelts; i++) {
            यदि (ngx_strncmp(names[n].किल्ली.data, names[i].किल्ली.data, len) != 0) {
                भङ्ग;
            }

            यदि (!dot
                && names[i].किल्ली.len > len
                && names[i].किल्ली.data[len] != '.')
            {
                भङ्ग;
            }

            next_name = ngx_array_ढकला(&next_names);
            यदि (next_name == रिक्त) {
                प्रत्यागम NGX_ERROR;
            }

            next_name->किल्ली.len = names[i].किल्ली.len - dot_len;
            next_name->किल्ली.data = names[i].किल्ली.data + dot_len;
            next_name->किल्ली_hash = 0;
            next_name->मूल्य = names[i].मूल्य;

#यदि 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "wc3: \"%V\"", &next_name->किल्ली);
#endif
        }

        यदि (next_names.nelts) {

            h = *hinit;
            h.hash = रिक्त;

            यदि (ngx_hash_wildcard_init(&h, (ngx_hash_किल्ली_t *) next_names.elts,
                                       next_names.nelts)
                != NGX_OK)
            {
                प्रत्यागम NGX_ERROR;
            }

            wdc = (ngx_hash_wildcard_t *) h.hash;

            यदि (names[n].किल्ली.len == len) {
                wdc->मूल्य = names[n].मूल्य;
            }

            name->मूल्य = (शून्य *) ((uintptr_t) wdc | (dot ? 3 : 2));

        } अन्यथा यदि (dot) {
            name->मूल्य = (शून्य *) ((uintptr_t) name->मूल्य | 1);
        }
    }

    यदि (ngx_hash_init(hinit, (ngx_hash_किल्ली_t *) curr_names.elts,
                      curr_names.nelts)
        != NGX_OK)
    {
        प्रत्यागम NGX_ERROR;
    }

    प्रत्यागम NGX_OK;
}


ngx_uint_t
ngx_hash_किल्ली(u_char *data, size_t len)
{
    ngx_uint_t  i, किल्ली;

    किल्ली = 0;

    प्रति (i = 0; i < len; i++) {
        किल्ली = ngx_hash(किल्ली, data[i]);
    }

    प्रत्यागम किल्ली;
}


ngx_uint_t
ngx_hash_किल्ली_lc(u_char *data, size_t len)
{
    ngx_uint_t  i, किल्ली;

    किल्ली = 0;

    प्रति (i = 0; i < len; i++) {
        किल्ली = ngx_hash(किल्ली, ngx_tolower(data[i]));
    }

    प्रत्यागम किल्ली;
}


ngx_uint_t
ngx_hash_strlow(u_char *dst, u_char *src, size_t n)
{
    ngx_uint_t  किल्ली;

    किल्ली = 0;

    यावत् (n--) {
        *dst = ngx_tolower(*src);
        किल्ली = ngx_hash(किल्ली, *dst);
        dst++;
        src++;
    }

    प्रत्यागम किल्ली;
}


ngx_int_t
ngx_hash_किल्ल्या_array_init(ngx_hash_किल्ल्या_arrays_t *ha, ngx_uint_t type)
{
    ngx_uint_t  asize;

    यदि (type == NGX_HASH_SMALL) {
        asize = 4;
        ha->hsize = 107;

    } अन्यथा {
        asize = NGX_HASH_LARGE_ASIZE;
        ha->hsize = NGX_HASH_LARGE_HSIZE;
    }

    यदि (ngx_array_init(&ha->किल्ल्या, ha->temp_pool, asize, परिमाण(ngx_hash_किल्ली_t))
        != NGX_OK)
    {
        प्रत्यागम NGX_ERROR;
    }

    यदि (ngx_array_init(&ha->dns_wc_head, ha->temp_pool, asize,
                       परिमाण(ngx_hash_किल्ली_t))
        != NGX_OK)
    {
        प्रत्यागम NGX_ERROR;
    }

    यदि (ngx_array_init(&ha->dns_wc_tail, ha->temp_pool, asize,
                       परिमाण(ngx_hash_किल्ली_t))
        != NGX_OK)
    {
        प्रत्यागम NGX_ERROR;
    }

    ha->किल्ल्या_hash = ngx_pcalloc(ha->temp_pool, परिमाण(ngx_array_t) * ha->hsize);
    यदि (ha->किल्ल्या_hash == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

    ha->dns_wc_head_hash = ngx_pcalloc(ha->temp_pool,
                                       परिमाण(ngx_array_t) * ha->hsize);
    यदि (ha->dns_wc_head_hash == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

    ha->dns_wc_tail_hash = ngx_pcalloc(ha->temp_pool,
                                       परिमाण(ngx_array_t) * ha->hsize);
    यदि (ha->dns_wc_tail_hash == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

    प्रत्यागम NGX_OK;
}


ngx_int_t
ngx_hash_add_किल्ली(ngx_hash_किल्ल्या_arrays_t *ha, ngx_str_t *किल्ली, शून्य *मूल्य,
    ngx_uint_t flags)
{
    size_t           len;
    u_char          *p;
    ngx_str_t       *name;
    ngx_uint_t       i, k, n, skip, last;
    ngx_array_t     *किल्ल्या, *hwc;
    ngx_hash_किल्ली_t  *hk;

    last = किल्ली->len;

    यदि (flags & NGX_HASH_WILDCARD_किल्ली) {

        /*
         * supported wildcards:
         *     "*.example.com", ".example.com", and "www.example.*"
         */

        n = 0;

        प्रति (i = 0; i < किल्ली->len; i++) {

            यदि (किल्ली->data[i] == '*') {
                यदि (++n > 1) {
                    प्रत्यागम NGX_DECLINED;
                }
            }

            यदि (किल्ली->data[i] == '.' && किल्ली->data[i + 1] == '.') {
                प्रत्यागम NGX_DECLINED;
            }

            यदि (किल्ली->data[i] == '\0') {
                प्रत्यागम NGX_DECLINED;
            }
        }

        यदि (किल्ली->len > 1 && किल्ली->data[0] == '.') {
            skip = 1;
            गच्छ wildcard;
        }

        यदि (किल्ली->len > 2) {

            यदि (किल्ली->data[0] == '*' && किल्ली->data[1] == '.') {
                skip = 2;
                गच्छ wildcard;
            }

            यदि (किल्ली->data[i - 2] == '.' && किल्ली->data[i - 1] == '*') {
                skip = 0;
                last -= 2;
                गच्छ wildcard;
            }
        }

        यदि (n) {
            प्रत्यागम NGX_DECLINED;
        }
    }

    /* exact hash */

    k = 0;

    प्रति (i = 0; i < last; i++) {
        यदि (!(flags & NGX_HASH_READONLY_किल्ली)) {
            किल्ली->data[i] = ngx_tolower(किल्ली->data[i]);
        }
        k = ngx_hash(k, किल्ली->data[i]);
    }

    k %= ha->hsize;

    /* check conflicts in exact hash */

    name = ha->किल्ल्या_hash[k].elts;

    यदि (name) {
        प्रति (i = 0; i < ha->किल्ल्या_hash[k].nelts; i++) {
            यदि (last != name[i].len) {
                अनुवर्त;
            }

            यदि (ngx_strncmp(किल्ली->data, name[i].data, last) == 0) {
                प्रत्यागम NGX_BUSY;
            }
        }

    } अन्यथा {
        यदि (ngx_array_init(&ha->किल्ल्या_hash[k], ha->temp_pool, 4,
                           परिमाण(ngx_str_t))
            != NGX_OK)
        {
            प्रत्यागम NGX_ERROR;
        }
    }

    name = ngx_array_ढकला(&ha->किल्ल्या_hash[k]);
    यदि (name == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

    *name = *किल्ली;

    hk = ngx_array_ढकला(&ha->किल्ल्या);
    यदि (hk == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

    hk->किल्ली = *किल्ली;
    hk->किल्ली_hash = ngx_hash_किल्ली(किल्ली->data, last);
    hk->मूल्य = मूल्य;

    प्रत्यागम NGX_OK;


wildcard:

    /* wildcard hash */

    k = ngx_hash_strlow(&किल्ली->data[skip], &किल्ली->data[skip], last - skip);

    k %= ha->hsize;

    यदि (skip == 1) {

        /* check conflicts in exact hash for ".example.com" */

        name = ha->किल्ल्या_hash[k].elts;

        यदि (name) {
            len = last - skip;

            प्रति (i = 0; i < ha->किल्ल्या_hash[k].nelts; i++) {
                यदि (len != name[i].len) {
                    अनुवर्त;
                }

                यदि (ngx_strncmp(&किल्ली->data[1], name[i].data, len) == 0) {
                    प्रत्यागम NGX_BUSY;
                }
            }

        } अन्यथा {
            यदि (ngx_array_init(&ha->किल्ल्या_hash[k], ha->temp_pool, 4,
                               परिमाण(ngx_str_t))
                != NGX_OK)
            {
                प्रत्यागम NGX_ERROR;
            }
        }

        name = ngx_array_ढकला(&ha->किल्ल्या_hash[k]);
        यदि (name == रिक्त) {
            प्रत्यागम NGX_ERROR;
        }

        name->len = last - 1;
        name->data = ngx_pnalloc(ha->temp_pool, name->len);
        यदि (name->data == रिक्त) {
            प्रत्यागम NGX_ERROR;
        }

        ngx_memcpy(name->data, &किल्ली->data[1], name->len);
    }


    यदि (skip) {

        /*
         * convert "*.example.com" to "com.example.\0"
         *      and ".example.com" to "com.example\0"
         */

        p = ngx_pnalloc(ha->temp_pool, last);
        यदि (p == रिक्त) {
            प्रत्यागम NGX_ERROR;
        }

        len = 0;
        n = 0;

        प्रति (i = last - 1; i; i--) {
            यदि (किल्ली->data[i] == '.') {
                ngx_memcpy(&p[n], &किल्ली->data[i + 1], len);
                n += len;
                p[n++] = '.';
                len = 0;
                अनुवर्त;
            }

            len++;
        }

        यदि (len) {
            ngx_memcpy(&p[n], &किल्ली->data[1], len);
            n += len;
        }

        p[n] = '\0';

        hwc = &ha->dns_wc_head;
        किल्ल्या = &ha->dns_wc_head_hash[k];

    } अन्यथा {

        /* convert "www.example.*" to "www.example\0" */

        last++;

        p = ngx_pnalloc(ha->temp_pool, last);
        यदि (p == रिक्त) {
            प्रत्यागम NGX_ERROR;
        }

        ngx_cpystrn(p, किल्ली->data, last);

        hwc = &ha->dns_wc_tail;
        किल्ल्या = &ha->dns_wc_tail_hash[k];
    }


    /* check conflicts in wildcard hash */

    name = किल्ल्या->elts;

    यदि (name) {
        len = last - skip;

        प्रति (i = 0; i < किल्ल्या->nelts; i++) {
            यदि (len != name[i].len) {
                अनुवर्त;
            }

            यदि (ngx_strncmp(किल्ली->data + skip, name[i].data, len) == 0) {
                प्रत्यागम NGX_BUSY;
            }
        }

    } अन्यथा {
        यदि (ngx_array_init(किल्ल्या, ha->temp_pool, 4, परिमाण(ngx_str_t)) != NGX_OK)
        {
            प्रत्यागम NGX_ERROR;
        }
    }

    name = ngx_array_ढकला(किल्ल्या);
    यदि (name == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

    name->len = last - skip;
    name->data = ngx_pnalloc(ha->temp_pool, name->len);
    यदि (name->data == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

    ngx_memcpy(name->data, किल्ली->data + skip, name->len);


    /* add to wildcard hash */

    hk = ngx_array_ढकला(hwc);
    यदि (hk == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

    hk->किल्ली.len = last - 1;
    hk->किल्ली.data = p;
    hk->किल्ली_hash = 0;
    hk->मूल्य = मूल्य;

    प्रत्यागम NGX_OK;
}
