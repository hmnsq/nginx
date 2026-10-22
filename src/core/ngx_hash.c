
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


शून्य *
ngx_hash_find(ngx_hash_t *hash, ngx_uint_t कुञ्चिका, u_char *name, size_t len)
{
    ngx_uint_t       i;
    ngx_hash_elt_t  *elt;

#जइ 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "hf:\"%*s\"", len, name);
#endif

    elt = hash->buckets[कुञ्चिका % hash->size];

    जइ (elt == रिक्त) {
        प्रत्यागम रिक्त;
    }

    जाव (elt->मानम्) {
        जइ (len != (size_t) elt->len) {
            गच्छ next;
        }

        प्रति (i = 0; i < len; i++) {
            जइ (name[i] != elt->name[i]) {
                गच्छ next;
            }
        }

        प्रत्यागम elt->मानम्;

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
    शून्य        *मानम्;
    ngx_uint_t   i, n, कुञ्चिका;

#जइ 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "wch:\"%*s\"", len, name);
#endif

    n = len;

    जाव (n) {
        जइ (name[n - 1] == '.') {
            भङ्ग;
        }

        n--;
    }

    कुञ्चिका = 0;

    प्रति (i = n; i < len; i++) {
        कुञ्चिका = ngx_hash(कुञ्चिका, name[i]);
    }

#जइ 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "key:\"%ui\"", कुञ्चिका);
#endif

    मानम् = ngx_hash_find(&hwc->hash, कुञ्चिका, &name[n], len - n);

#जइ 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "value:\"%p\"", मानम्);
#endif

    जइ (मानम्) {

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

        जइ ((uintptr_t) मानम् & 2) {

            जइ (n == 0) {

                /* "example.com" */

                जइ ((uintptr_t) मानम् & 1) {
                    प्रत्यागम रिक्त;
                }

                hwc = (ngx_hash_wildcard_t *)
                                          ((uintptr_t) मानम् & (uintptr_t) ~3);
                प्रत्यागम hwc->मानम्;
            }

            hwc = (ngx_hash_wildcard_t *) ((uintptr_t) मानम् & (uintptr_t) ~3);

            मानम् = ngx_hash_find_wc_head(hwc, name, n - 1);

            जइ (मानम्) {
                प्रत्यागम मानम्;
            }

            प्रत्यागम hwc->मानम्;
        }

        जइ ((uintptr_t) मानम् & 1) {

            जइ (n == 0) {

                /* "example.com" */

                प्रत्यागम रिक्त;
            }

            प्रत्यागम (शून्य *) ((uintptr_t) मानम् & (uintptr_t) ~3);
        }

        प्रत्यागम मानम्;
    }

    प्रत्यागम hwc->मानम्;
}


शून्य *
ngx_hash_find_wc_tail(ngx_hash_wildcard_t *hwc, u_char *name, size_t len)
{
    शून्य        *मानम्;
    ngx_uint_t   i, कुञ्चिका;

#जइ 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "wct:\"%*s\"", len, name);
#endif

    कुञ्चिका = 0;

    प्रति (i = 0; i < len; i++) {
        जइ (name[i] == '.') {
            भङ्ग;
        }

        कुञ्चिका = ngx_hash(कुञ्चिका, name[i]);
    }

    जइ (i == len) {
        प्रत्यागम रिक्त;
    }

#जइ 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "key:\"%ui\"", कुञ्चिका);
#endif

    मानम् = ngx_hash_find(&hwc->hash, कुञ्चिका, name, i);

#जइ 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "value:\"%p\"", मानम्);
#endif

    जइ (मानम्) {

        /*
         * the 2 low bits of value have the special meaning:
         *     00 - value is data pointer;
         *     11 - value is pointer to wildcard hash allowing "example.*".
         */

        जइ ((uintptr_t) मानम् & 2) {

            i++;

            hwc = (ngx_hash_wildcard_t *) ((uintptr_t) मानम् & (uintptr_t) ~3);

            मानम् = ngx_hash_find_wc_tail(hwc, &name[i], len - i);

            जइ (मानम्) {
                प्रत्यागम मानम्;
            }

            प्रत्यागम hwc->मानम्;
        }

        प्रत्यागम मानम्;
    }

    प्रत्यागम hwc->मानम्;
}


शून्य *
ngx_hash_find_combined(ngx_hash_combined_t *hash, ngx_uint_t कुञ्चिका, u_char *name,
    size_t len)
{
    शून्य  *मानम्;

    जइ (hash->hash.buckets) {
        मानम् = ngx_hash_find(&hash->hash, कुञ्चिका, name, len);

        जइ (मानम्) {
            प्रत्यागम मानम्;
        }
    }

    जइ (len == 0) {
        प्रत्यागम रिक्त;
    }

    जइ (hash->wc_head && hash->wc_head->hash.buckets) {
        मानम् = ngx_hash_find_wc_head(hash->wc_head, name, len);

        जइ (मानम्) {
            प्रत्यागम मानम्;
        }
    }

    जइ (hash->wc_tail && hash->wc_tail->hash.buckets) {
        मानम् = ngx_hash_find_wc_tail(hash->wc_tail, name, len);

        जइ (मानम्) {
            प्रत्यागम मानम्;
        }
    }

    प्रत्यागम रिक्त;
}


#define NGX_HASH_ELT_SIZE(name)                                               \
    (परिमाण(शून्य *) + ngx_align((name)->कुञ्चिका.len + 2, परिमाण(शून्य *)))

ngx_int_t
ngx_hash_init(ngx_hash_init_t *hinit, ngx_hash_कुञ्चिका_t *names, ngx_uint_t nelts)
{
    u_char          *elts;
    size_t           len;
    u_short         *test;
    ngx_uint_t       i, n, कुञ्चिका, size, start, bucket_size;
    ngx_hash_elt_t  *elt, **buckets;

    जइ (hinit->max_size == 0) {
        ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                      "could not build %s, you should "
                      "increase %s_max_size: %i",
                      hinit->name, hinit->name, hinit->max_size);
        प्रत्यागम NGX_ERROR;
    }

    जइ (hinit->bucket_size > 65536 - ngx_cacheline_size) {
        ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                      "could not build %s, too large "
                      "%s_bucket_size: %i",
                      hinit->name, hinit->name, hinit->bucket_size);
        प्रत्यागम NGX_ERROR;
    }

    प्रति (n = 0; n < nelts; n++) {
        जइ (names[n].कुञ्चिका.data == रिक्त) {
            अनुवर्त;
        }

        जइ (hinit->bucket_size < NGX_HASH_ELT_SIZE(&names[n]) + परिमाण(शून्य *))
        {
            ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                          "could not build %s, you should "
                          "increase %s_bucket_size: %i",
                          hinit->name, hinit->name, hinit->bucket_size);
            प्रत्यागम NGX_ERROR;
        }
    }

    test = ngx_alloc(hinit->max_size * परिमाण(u_short), hinit->pool->log);
    जइ (test == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

    bucket_size = hinit->bucket_size - परिमाण(शून्य *);

    start = nelts / (bucket_size / (2 * परिमाण(शून्य *)));
    start = start ? start : 1;

    जइ (hinit->max_size > 10000 && nelts && hinit->max_size / nelts < 100) {
        start = hinit->max_size - 1000;
    }

    प्रति (size = start; size <= hinit->max_size; size++) {

        ngx_memzero(test, size * परिमाण(u_short));

        प्रति (n = 0; n < nelts; n++) {
            जइ (names[n].कुञ्चिका.data == रिक्त) {
                अनुवर्त;
            }

            कुञ्चिका = names[n].कुञ्चिका_hash % size;
            len = test[कुञ्चिका] + NGX_HASH_ELT_SIZE(&names[n]);

#जइ 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "%ui: %ui %uz \"%V\"",
                          size, कुञ्चिका, len, &names[n].कुञ्चिका);
#endif

            जइ (len > bucket_size) {
                गच्छ next;
            }

            test[कुञ्चिका] = (u_short) len;
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
        जइ (names[n].कुञ्चिका.data == रिक्त) {
            अनुवर्त;
        }

        कुञ्चिका = names[n].कुञ्चिका_hash % size;
        len = test[कुञ्चिका] + NGX_HASH_ELT_SIZE(&names[n]);

        जइ (len > 65536 - ngx_cacheline_size) {
            ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                          "could not build %s, you should "
                          "increase %s_max_size: %i",
                          hinit->name, hinit->name, hinit->max_size);
            ngx_free(test);
            प्रत्यागम NGX_ERROR;
        }

        test[कुञ्चिका] = (u_short) len;
    }

    len = 0;

    प्रति (i = 0; i < size; i++) {
        जइ (test[i] == परिमाण(शून्य *)) {
            अनुवर्त;
        }

        test[i] = (u_short) (ngx_align(test[i], ngx_cacheline_size));

        len += test[i];
    }

    जइ (hinit->hash == रिक्त) {
        hinit->hash = ngx_pcalloc(hinit->pool, परिमाण(ngx_hash_wildcard_t)
                                             + size * परिमाण(ngx_hash_elt_t *));
        जइ (hinit->hash == रिक्त) {
            ngx_free(test);
            प्रत्यागम NGX_ERROR;
        }

        buckets = (ngx_hash_elt_t **)
                      ((u_char *) hinit->hash + परिमाण(ngx_hash_wildcard_t));

    } अण्णहा {
        buckets = ngx_pcalloc(hinit->pool, size * परिमाण(ngx_hash_elt_t *));
        जइ (buckets == रिक्त) {
            ngx_free(test);
            प्रत्यागम NGX_ERROR;
        }
    }

    elts = ngx_palloc(hinit->pool, len + ngx_cacheline_size);
    जइ (elts == रिक्त) {
        ngx_free(test);
        प्रत्यागम NGX_ERROR;
    }

    elts = ngx_align_ptr(elts, ngx_cacheline_size);

    प्रति (i = 0; i < size; i++) {
        जइ (test[i] == परिमाण(शून्य *)) {
            अनुवर्त;
        }

        buckets[i] = (ngx_hash_elt_t *) elts;
        elts += test[i];
    }

    प्रति (i = 0; i < size; i++) {
        test[i] = 0;
    }

    प्रति (n = 0; n < nelts; n++) {
        जइ (names[n].कुञ्चिका.data == रिक्त) {
            अनुवर्त;
        }

        कुञ्चिका = names[n].कुञ्चिका_hash % size;
        elt = (ngx_hash_elt_t *) ((u_char *) buckets[कुञ्चिका] + test[कुञ्चिका]);

        elt->मानम् = names[n].मानम्;
        elt->len = (u_short) names[n].कुञ्चिका.len;

        ngx_strlow(elt->name, names[n].कुञ्चिका.data, names[n].कुञ्चिका.len);

        test[कुञ्चिका] = (u_short) (test[कुञ्चिका] + NGX_HASH_ELT_SIZE(&names[n]));
    }

    प्रति (i = 0; i < size; i++) {
        जइ (buckets[i] == रिक्त) {
            अनुवर्त;
        }

        elt = (ngx_hash_elt_t *) ((u_char *) buckets[i] + test[i]);

        elt->मानम् = रिक्त;
    }

    ngx_free(test);

    hinit->hash->buckets = buckets;
    hinit->hash->size = size;

#जइ 0

    प्रति (i = 0; i < size; i++) {
        ngx_str_t   val;
        ngx_uint_t  कुञ्चिका;

        elt = buckets[i];

        जइ (elt == रिक्त) {
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "%ui: NULL", i);
            अनुवर्त;
        }

        जाव (elt->मानम्) {
            val.len = elt->len;
            val.data = &elt->name[0];

            कुञ्चिका = hinit->कुञ्चिका(val.data, val.len);

            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "%ui: %p \"%V\" %ui", i, elt, &val, कुञ्चिका);

            elt = (ngx_hash_elt_t *) ngx_align_ptr(&elt->name[0] + elt->len,
                                                   परिमाण(शून्य *));
        }
    }

#endif

    प्रत्यागम NGX_OK;
}


ngx_int_t
ngx_hash_wildcard_init(ngx_hash_init_t *hinit, ngx_hash_कुञ्चिका_t *names,
    ngx_uint_t nelts)
{
    size_t                len, dot_len;
    ngx_uint_t            i, n, dot;
    ngx_array_t           curr_names, next_names;
    ngx_hash_कुञ्चिका_t       *name, *next_name;
    ngx_hash_init_t       h;
    ngx_hash_wildcard_t  *wdc;

    जइ (ngx_array_init(&curr_names, hinit->temp_pool, nelts,
                       परिमाण(ngx_hash_कुञ्चिका_t))
        != NGX_OK)
    {
        प्रत्यागम NGX_ERROR;
    }

    जइ (ngx_array_init(&next_names, hinit->temp_pool, nelts,
                       परिमाण(ngx_hash_कुञ्चिका_t))
        != NGX_OK)
    {
        प्रत्यागम NGX_ERROR;
    }

    प्रति (n = 0; n < nelts; n = i) {

#जइ 0
        ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                      "wc0: \"%V\"", &names[n].कुञ्चिका);
#endif

        dot = 0;

        प्रति (len = 0; len < names[n].कुञ्चिका.len; len++) {
            जइ (names[n].कुञ्चिका.data[len] == '.') {
                dot = 1;
                भङ्ग;
            }
        }

        name = ngx_array_प्रक्षिप(&curr_names);
        जइ (name == रिक्त) {
            प्रत्यागम NGX_ERROR;
        }

        name->कुञ्चिका.len = len;
        name->कुञ्चिका.data = names[n].कुञ्चिका.data;
        name->कुञ्चिका_hash = hinit->कुञ्चिका(name->कुञ्चिका.data, name->कुञ्चिका.len);
        name->मानम् = names[n].मानम्;

#जइ 0
        ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                      "wc1: \"%V\" %ui", &name->कुञ्चिका, dot);
#endif

        dot_len = len + 1;

        जइ (dot) {
            len++;
        }

        next_names.nelts = 0;

        जइ (names[n].कुञ्चिका.len != len) {
            next_name = ngx_array_प्रक्षिप(&next_names);
            जइ (next_name == रिक्त) {
                प्रत्यागम NGX_ERROR;
            }

            next_name->कुञ्चिका.len = names[n].कुञ्चिका.len - len;
            next_name->कुञ्चिका.data = names[n].कुञ्चिका.data + len;
            next_name->कुञ्चिका_hash = 0;
            next_name->मानम् = names[n].मानम्;

#जइ 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "wc2: \"%V\"", &next_name->कुञ्चिका);
#endif
        }

        प्रति (i = n + 1; i < nelts; i++) {
            जइ (ngx_strncmp(names[n].कुञ्चिका.data, names[i].कुञ्चिका.data, len) != 0) {
                भङ्ग;
            }

            जइ (!dot
                && names[i].कुञ्चिका.len > len
                && names[i].कुञ्चिका.data[len] != '.')
            {
                भङ्ग;
            }

            next_name = ngx_array_प्रक्षिप(&next_names);
            जइ (next_name == रिक्त) {
                प्रत्यागम NGX_ERROR;
            }

            next_name->कुञ्चिका.len = names[i].कुञ्चिका.len - dot_len;
            next_name->कुञ्चिका.data = names[i].कुञ्चिका.data + dot_len;
            next_name->कुञ्चिका_hash = 0;
            next_name->मानम् = names[i].मानम्;

#जइ 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "wc3: \"%V\"", &next_name->कुञ्चिका);
#endif
        }

        जइ (next_names.nelts) {

            h = *hinit;
            h.hash = रिक्त;

            जइ (ngx_hash_wildcard_init(&h, (ngx_hash_कुञ्चिका_t *) next_names.elts,
                                       next_names.nelts)
                != NGX_OK)
            {
                प्रत्यागम NGX_ERROR;
            }

            wdc = (ngx_hash_wildcard_t *) h.hash;

            जइ (names[n].कुञ्चिका.len == len) {
                wdc->मानम् = names[n].मानम्;
            }

            name->मानम् = (शून्य *) ((uintptr_t) wdc | (dot ? 3 : 2));

        } अण्णहा जइ (dot) {
            name->मानम् = (शून्य *) ((uintptr_t) name->मानम् | 1);
        }
    }

    जइ (ngx_hash_init(hinit, (ngx_hash_कुञ्चिका_t *) curr_names.elts,
                      curr_names.nelts)
        != NGX_OK)
    {
        प्रत्यागम NGX_ERROR;
    }

    प्रत्यागम NGX_OK;
}


ngx_uint_t
ngx_hash_कुञ्चिका(u_char *data, size_t len)
{
    ngx_uint_t  i, कुञ्चिका;

    कुञ्चिका = 0;

    प्रति (i = 0; i < len; i++) {
        कुञ्चिका = ngx_hash(कुञ्चिका, data[i]);
    }

    प्रत्यागम कुञ्चिका;
}


ngx_uint_t
ngx_hash_कुञ्चिका_lc(u_char *data, size_t len)
{
    ngx_uint_t  i, कुञ्चिका;

    कुञ्चिका = 0;

    प्रति (i = 0; i < len; i++) {
        कुञ्चिका = ngx_hash(कुञ्चिका, ngx_tolower(data[i]));
    }

    प्रत्यागम कुञ्चिका;
}


ngx_uint_t
ngx_hash_strlow(u_char *dst, u_char *src, size_t n)
{
    ngx_uint_t  कुञ्चिका;

    कुञ्चिका = 0;

    जाव (n--) {
        *dst = ngx_tolower(*src);
        कुञ्चिका = ngx_hash(कुञ्चिका, *dst);
        dst++;
        src++;
    }

    प्रत्यागम कुञ्चिका;
}


ngx_int_t
ngx_hash_कुञ्चिकाः_array_init(ngx_hash_कुञ्चिकाः_arrays_t *ha, ngx_uint_t type)
{
    ngx_uint_t  asize;

    जइ (type == NGX_HASH_SMALL) {
        asize = 4;
        ha->hsize = 107;

    } अण्णहा {
        asize = NGX_HASH_LARGE_ASIZE;
        ha->hsize = NGX_HASH_LARGE_HSIZE;
    }

    जइ (ngx_array_init(&ha->कुञ्चिकाः, ha->temp_pool, asize, परिमाण(ngx_hash_कुञ्चिका_t))
        != NGX_OK)
    {
        प्रत्यागम NGX_ERROR;
    }

    जइ (ngx_array_init(&ha->dns_wc_head, ha->temp_pool, asize,
                       परिमाण(ngx_hash_कुञ्चिका_t))
        != NGX_OK)
    {
        प्रत्यागम NGX_ERROR;
    }

    जइ (ngx_array_init(&ha->dns_wc_tail, ha->temp_pool, asize,
                       परिमाण(ngx_hash_कुञ्चिका_t))
        != NGX_OK)
    {
        प्रत्यागम NGX_ERROR;
    }

    ha->कुञ्चिकाः_hash = ngx_pcalloc(ha->temp_pool, परिमाण(ngx_array_t) * ha->hsize);
    जइ (ha->कुञ्चिकाः_hash == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

    ha->dns_wc_head_hash = ngx_pcalloc(ha->temp_pool,
                                       परिमाण(ngx_array_t) * ha->hsize);
    जइ (ha->dns_wc_head_hash == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

    ha->dns_wc_tail_hash = ngx_pcalloc(ha->temp_pool,
                                       परिमाण(ngx_array_t) * ha->hsize);
    जइ (ha->dns_wc_tail_hash == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

    प्रत्यागम NGX_OK;
}


ngx_int_t
ngx_hash_add_कुञ्चिका(ngx_hash_कुञ्चिकाः_arrays_t *ha, ngx_str_t *कुञ्चिका, शून्य *मानम्,
    ngx_uint_t flags)
{
    size_t           len;
    u_char          *p;
    ngx_str_t       *name;
    ngx_uint_t       i, k, n, skip, last;
    ngx_array_t     *कुञ्चिकाः, *hwc;
    ngx_hash_कुञ्चिका_t  *hk;

    last = कुञ्चिका->len;

    जइ (flags & NGX_HASH_WILDCARD_कुञ्चिका) {

        /*
         * supported wildcards:
         *     "*.example.com", ".example.com", and "www.example.*"
         */

        n = 0;

        प्रति (i = 0; i < कुञ्चिका->len; i++) {

            जइ (कुञ्चिका->data[i] == '*') {
                जइ (++n > 1) {
                    प्रत्यागम NGX_DECLINED;
                }
            }

            जइ (कुञ्चिका->data[i] == '.' && कुञ्चिका->data[i + 1] == '.') {
                प्रत्यागम NGX_DECLINED;
            }

            जइ (कुञ्चिका->data[i] == '\0') {
                प्रत्यागम NGX_DECLINED;
            }
        }

        जइ (कुञ्चिका->len > 1 && कुञ्चिका->data[0] == '.') {
            skip = 1;
            गच्छ wildcard;
        }

        जइ (कुञ्चिका->len > 2) {

            जइ (कुञ्चिका->data[0] == '*' && कुञ्चिका->data[1] == '.') {
                skip = 2;
                गच्छ wildcard;
            }

            जइ (कुञ्चिका->data[i - 2] == '.' && कुञ्चिका->data[i - 1] == '*') {
                skip = 0;
                last -= 2;
                गच्छ wildcard;
            }
        }

        जइ (n) {
            प्रत्यागम NGX_DECLINED;
        }
    }

    /* exact hash */

    k = 0;

    प्रति (i = 0; i < last; i++) {
        जइ (!(flags & NGX_HASH_READONLY_कुञ्चिका)) {
            कुञ्चिका->data[i] = ngx_tolower(कुञ्चिका->data[i]);
        }
        k = ngx_hash(k, कुञ्चिका->data[i]);
    }

    k %= ha->hsize;

    /* check conflicts in exact hash */

    name = ha->कुञ्चिकाः_hash[k].elts;

    जइ (name) {
        प्रति (i = 0; i < ha->कुञ्चिकाः_hash[k].nelts; i++) {
            जइ (last != name[i].len) {
                अनुवर्त;
            }

            जइ (ngx_strncmp(कुञ्चिका->data, name[i].data, last) == 0) {
                प्रत्यागम NGX_BUSY;
            }
        }

    } अण्णहा {
        जइ (ngx_array_init(&ha->कुञ्चिकाः_hash[k], ha->temp_pool, 4,
                           परिमाण(ngx_str_t))
            != NGX_OK)
        {
            प्रत्यागम NGX_ERROR;
        }
    }

    name = ngx_array_प्रक्षिप(&ha->कुञ्चिकाः_hash[k]);
    जइ (name == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

    *name = *कुञ्चिका;

    hk = ngx_array_प्रक्षिप(&ha->कुञ्चिकाः);
    जइ (hk == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

    hk->कुञ्चिका = *कुञ्चिका;
    hk->कुञ्चिका_hash = ngx_hash_कुञ्चिका(कुञ्चिका->data, last);
    hk->मानम् = मानम्;

    प्रत्यागम NGX_OK;


wildcard:

    /* wildcard hash */

    k = ngx_hash_strlow(&कुञ्चिका->data[skip], &कुञ्चिका->data[skip], last - skip);

    k %= ha->hsize;

    जइ (skip == 1) {

        /* check conflicts in exact hash for ".example.com" */

        name = ha->कुञ्चिकाः_hash[k].elts;

        जइ (name) {
            len = last - skip;

            प्रति (i = 0; i < ha->कुञ्चिकाः_hash[k].nelts; i++) {
                जइ (len != name[i].len) {
                    अनुवर्त;
                }

                जइ (ngx_strncmp(&कुञ्चिका->data[1], name[i].data, len) == 0) {
                    प्रत्यागम NGX_BUSY;
                }
            }

        } अण्णहा {
            जइ (ngx_array_init(&ha->कुञ्चिकाः_hash[k], ha->temp_pool, 4,
                               परिमाण(ngx_str_t))
                != NGX_OK)
            {
                प्रत्यागम NGX_ERROR;
            }
        }

        name = ngx_array_प्रक्षिप(&ha->कुञ्चिकाः_hash[k]);
        जइ (name == रिक्त) {
            प्रत्यागम NGX_ERROR;
        }

        name->len = last - 1;
        name->data = ngx_pnalloc(ha->temp_pool, name->len);
        जइ (name->data == रिक्त) {
            प्रत्यागम NGX_ERROR;
        }

        ngx_memcpy(name->data, &कुञ्चिका->data[1], name->len);
    }


    जइ (skip) {

        /*
         * convert "*.example.com" to "com.example.\0"
         *      and ".example.com" to "com.example\0"
         */

        p = ngx_pnalloc(ha->temp_pool, last);
        जइ (p == रिक्त) {
            प्रत्यागम NGX_ERROR;
        }

        len = 0;
        n = 0;

        प्रति (i = last - 1; i; i--) {
            जइ (कुञ्चिका->data[i] == '.') {
                ngx_memcpy(&p[n], &कुञ्चिका->data[i + 1], len);
                n += len;
                p[n++] = '.';
                len = 0;
                अनुवर्त;
            }

            len++;
        }

        जइ (len) {
            ngx_memcpy(&p[n], &कुञ्चिका->data[1], len);
            n += len;
        }

        p[n] = '\0';

        hwc = &ha->dns_wc_head;
        कुञ्चिकाः = &ha->dns_wc_head_hash[k];

    } अण्णहा {

        /* convert "www.example.*" to "www.example\0" */

        last++;

        p = ngx_pnalloc(ha->temp_pool, last);
        जइ (p == रिक्त) {
            प्रत्यागम NGX_ERROR;
        }

        ngx_cpystrn(p, कुञ्चिका->data, last);

        hwc = &ha->dns_wc_tail;
        कुञ्चिकाः = &ha->dns_wc_tail_hash[k];
    }


    /* check conflicts in wildcard hash */

    name = कुञ्चिकाः->elts;

    जइ (name) {
        len = last - skip;

        प्रति (i = 0; i < कुञ्चिकाः->nelts; i++) {
            जइ (len != name[i].len) {
                अनुवर्त;
            }

            जइ (ngx_strncmp(कुञ्चिका->data + skip, name[i].data, len) == 0) {
                प्रत्यागम NGX_BUSY;
            }
        }

    } अण्णहा {
        जइ (ngx_array_init(कुञ्चिकाः, ha->temp_pool, 4, परिमाण(ngx_str_t)) != NGX_OK)
        {
            प्रत्यागम NGX_ERROR;
        }
    }

    name = ngx_array_प्रक्षिप(कुञ्चिकाः);
    जइ (name == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

    name->len = last - skip;
    name->data = ngx_pnalloc(ha->temp_pool, name->len);
    जइ (name->data == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

    ngx_memcpy(name->data, कुञ्चिका->data + skip, name->len);


    /* add to wildcard hash */

    hk = ngx_array_प्रक्षिप(hwc);
    जइ (hk == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

    hk->कुञ्चिका.len = last - 1;
    hk->कुञ्चिका.data = p;
    hk->कुञ्चिका_hash = 0;
    hk->मानम् = मानम्;

    प्रत्यागम NGX_OK;
}
