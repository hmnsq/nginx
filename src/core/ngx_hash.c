
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


शून्य *
ngx_hash_find(ngx_hash_t *hash, ngx_uint_t किल्ली, u_char *name, माप_प्रकार len)
{
    ngx_uint_t       i;
    ngx_hash_elt_t  *elt;

#जर 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "hf:\"%*s\"", len, name);
#endif

    elt = hash->buckets[किल्ली % hash->size];

    जर (elt == रिक्त) {
        परत रिक्त;
    }

    पर्यंत (elt->मूल्य) {
        जर (len != (माप_प्रकार) elt->len) {
            जा next;
        }

        साठी (i = 0; i < len; i++) {
            जर (name[i] != elt->name[i]) {
                जा next;
            }
        }

        परत elt->मूल्य;

    next:

        elt = (ngx_hash_elt_t *) ngx_align_ptr(&elt->name[0] + elt->len,
                                               आकार(शून्य *));
        सुरूठेवा;
    }

    परत रिक्त;
}


शून्य *
ngx_hash_find_wc_head(ngx_hash_wildcard_t *hwc, u_char *name, माप_प्रकार len)
{
    शून्य        *मूल्य;
    ngx_uint_t   i, n, किल्ली;

#जर 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "wch:\"%*s\"", len, name);
#endif

    n = len;

    पर्यंत (n) {
        जर (name[n - 1] == '.') {
            थांबा;
        }

        n--;
    }

    किल्ली = 0;

    साठी (i = n; i < len; i++) {
        किल्ली = ngx_hash(किल्ली, name[i]);
    }

#जर 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "key:\"%ui\"", किल्ली);
#endif

    मूल्य = ngx_hash_find(&hwc->hash, किल्ली, &name[n], len - n);

#जर 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "value:\"%p\"", मूल्य);
#endif

    जर (मूल्य) {

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

        जर ((uintptr_t) मूल्य & 2) {

            जर (n == 0) {

                /* "example.com" */

                जर ((uintptr_t) मूल्य & 1) {
                    परत रिक्त;
                }

                hwc = (ngx_hash_wildcard_t *)
                                          ((uintptr_t) मूल्य & (uintptr_t) ~3);
                परत hwc->मूल्य;
            }

            hwc = (ngx_hash_wildcard_t *) ((uintptr_t) मूल्य & (uintptr_t) ~3);

            मूल्य = ngx_hash_find_wc_head(hwc, name, n - 1);

            जर (मूल्य) {
                परत मूल्य;
            }

            परत hwc->मूल्य;
        }

        जर ((uintptr_t) मूल्य & 1) {

            जर (n == 0) {

                /* "example.com" */

                परत रिक्त;
            }

            परत (शून्य *) ((uintptr_t) मूल्य & (uintptr_t) ~3);
        }

        परत मूल्य;
    }

    परत hwc->मूल्य;
}


शून्य *
ngx_hash_find_wc_tail(ngx_hash_wildcard_t *hwc, u_char *name, माप_प्रकार len)
{
    शून्य        *मूल्य;
    ngx_uint_t   i, किल्ली;

#जर 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "wct:\"%*s\"", len, name);
#endif

    किल्ली = 0;

    साठी (i = 0; i < len; i++) {
        जर (name[i] == '.') {
            थांबा;
        }

        किल्ली = ngx_hash(किल्ली, name[i]);
    }

    जर (i == len) {
        परत रिक्त;
    }

#जर 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "key:\"%ui\"", किल्ली);
#endif

    मूल्य = ngx_hash_find(&hwc->hash, किल्ली, name, i);

#जर 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "value:\"%p\"", मूल्य);
#endif

    जर (मूल्य) {

        /*
         * the 2 low bits of value have the special meaning:
         *     00 - value is data pointer;
         *     11 - value is pointer to wildcard hash allowing "example.*".
         */

        जर ((uintptr_t) मूल्य & 2) {

            i++;

            hwc = (ngx_hash_wildcard_t *) ((uintptr_t) मूल्य & (uintptr_t) ~3);

            मूल्य = ngx_hash_find_wc_tail(hwc, &name[i], len - i);

            जर (मूल्य) {
                परत मूल्य;
            }

            परत hwc->मूल्य;
        }

        परत मूल्य;
    }

    परत hwc->मूल्य;
}


शून्य *
ngx_hash_find_combined(ngx_hash_combined_t *hash, ngx_uint_t किल्ली, u_char *name,
    माप_प्रकार len)
{
    शून्य  *मूल्य;

    जर (hash->hash.buckets) {
        मूल्य = ngx_hash_find(&hash->hash, किल्ली, name, len);

        जर (मूल्य) {
            परत मूल्य;
        }
    }

    जर (len == 0) {
        परत रिक्त;
    }

    जर (hash->wc_head && hash->wc_head->hash.buckets) {
        मूल्य = ngx_hash_find_wc_head(hash->wc_head, name, len);

        जर (मूल्य) {
            परत मूल्य;
        }
    }

    जर (hash->wc_tail && hash->wc_tail->hash.buckets) {
        मूल्य = ngx_hash_find_wc_tail(hash->wc_tail, name, len);

        जर (मूल्य) {
            परत मूल्य;
        }
    }

    परत रिक्त;
}


#define NGX_HASH_ELT_SIZE(name)                                               \
    (आकार(शून्य *) + ngx_align((name)->किल्ली.len + 2, आकार(शून्य *)))

ngx_int_t
ngx_hash_init(ngx_hash_init_t *hinit, ngx_hash_किल्ली_t *names, ngx_uint_t nelts)
{
    u_char          *elts;
    माप_प्रकार           len;
    u_short         *test;
    ngx_uint_t       i, n, किल्ली, size, start, bucket_size;
    ngx_hash_elt_t  *elt, **buckets;

    जर (hinit->max_size == 0) {
        ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                      "could not build %s, you should "
                      "increase %s_max_size: %i",
                      hinit->name, hinit->name, hinit->max_size);
        परत NGX_ERROR;
    }

    जर (hinit->bucket_size > 65536 - ngx_cacheline_size) {
        ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                      "could not build %s, too large "
                      "%s_bucket_size: %i",
                      hinit->name, hinit->name, hinit->bucket_size);
        परत NGX_ERROR;
    }

    साठी (n = 0; n < nelts; n++) {
        जर (names[n].किल्ली.data == रिक्त) {
            सुरूठेवा;
        }

        जर (hinit->bucket_size < NGX_HASH_ELT_SIZE(&names[n]) + आकार(शून्य *))
        {
            ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                          "could not build %s, you should "
                          "increase %s_bucket_size: %i",
                          hinit->name, hinit->name, hinit->bucket_size);
            परत NGX_ERROR;
        }
    }

    test = ngx_alloc(hinit->max_size * आकार(u_short), hinit->pool->log);
    जर (test == रिक्त) {
        परत NGX_ERROR;
    }

    bucket_size = hinit->bucket_size - आकार(शून्य *);

    start = nelts / (bucket_size / (2 * आकार(शून्य *)));
    start = start ? start : 1;

    जर (hinit->max_size > 10000 && nelts && hinit->max_size / nelts < 100) {
        start = hinit->max_size - 1000;
    }

    साठी (size = start; size <= hinit->max_size; size++) {

        ngx_memzero(test, size * आकार(u_short));

        साठी (n = 0; n < nelts; n++) {
            जर (names[n].किल्ली.data == रिक्त) {
                सुरूठेवा;
            }

            किल्ली = names[n].किल्ली_hash % size;
            len = test[किल्ली] + NGX_HASH_ELT_SIZE(&names[n]);

#जर 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "%ui: %ui %uz \"%V\"",
                          size, किल्ली, len, &names[n].किल्ली);
#endif

            जर (len > bucket_size) {
                जा next;
            }

            test[किल्ली] = (u_short) len;
        }

        जा found;

    next:

        सुरूठेवा;
    }

    size = hinit->max_size;

    ngx_log_error(NGX_LOG_WARN, hinit->pool->log, 0,
                  "could not build optimal %s, you should increase "
                  "either %s_max_size: %i or %s_bucket_size: %i; "
                  "ignoring %s_bucket_size",
                  hinit->name, hinit->name, hinit->max_size,
                  hinit->name, hinit->bucket_size, hinit->name);

found:

    साठी (i = 0; i < size; i++) {
        test[i] = आकार(शून्य *);
    }

    साठी (n = 0; n < nelts; n++) {
        जर (names[n].किल्ली.data == रिक्त) {
            सुरूठेवा;
        }

        किल्ली = names[n].किल्ली_hash % size;
        len = test[किल्ली] + NGX_HASH_ELT_SIZE(&names[n]);

        जर (len > 65536 - ngx_cacheline_size) {
            ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                          "could not build %s, you should "
                          "increase %s_max_size: %i",
                          hinit->name, hinit->name, hinit->max_size);
            ngx_free(test);
            परत NGX_ERROR;
        }

        test[किल्ली] = (u_short) len;
    }

    len = 0;

    साठी (i = 0; i < size; i++) {
        जर (test[i] == आकार(शून्य *)) {
            सुरूठेवा;
        }

        test[i] = (u_short) (ngx_align(test[i], ngx_cacheline_size));

        len += test[i];
    }

    जर (hinit->hash == रिक्त) {
        hinit->hash = ngx_pcalloc(hinit->pool, आकार(ngx_hash_wildcard_t)
                                             + size * आकार(ngx_hash_elt_t *));
        जर (hinit->hash == रिक्त) {
            ngx_free(test);
            परत NGX_ERROR;
        }

        buckets = (ngx_hash_elt_t **)
                      ((u_char *) hinit->hash + आकार(ngx_hash_wildcard_t));

    } अन्यथा {
        buckets = ngx_pcalloc(hinit->pool, size * आकार(ngx_hash_elt_t *));
        जर (buckets == रिक्त) {
            ngx_free(test);
            परत NGX_ERROR;
        }
    }

    elts = ngx_palloc(hinit->pool, len + ngx_cacheline_size);
    जर (elts == रिक्त) {
        ngx_free(test);
        परत NGX_ERROR;
    }

    elts = ngx_align_ptr(elts, ngx_cacheline_size);

    साठी (i = 0; i < size; i++) {
        जर (test[i] == आकार(शून्य *)) {
            सुरूठेवा;
        }

        buckets[i] = (ngx_hash_elt_t *) elts;
        elts += test[i];
    }

    साठी (i = 0; i < size; i++) {
        test[i] = 0;
    }

    साठी (n = 0; n < nelts; n++) {
        जर (names[n].किल्ली.data == रिक्त) {
            सुरूठेवा;
        }

        किल्ली = names[n].किल्ली_hash % size;
        elt = (ngx_hash_elt_t *) ((u_char *) buckets[किल्ली] + test[किल्ली]);

        elt->मूल्य = names[n].मूल्य;
        elt->len = (u_short) names[n].किल्ली.len;

        ngx_strlow(elt->name, names[n].किल्ली.data, names[n].किल्ली.len);

        test[किल्ली] = (u_short) (test[किल्ली] + NGX_HASH_ELT_SIZE(&names[n]));
    }

    साठी (i = 0; i < size; i++) {
        जर (buckets[i] == रिक्त) {
            सुरूठेवा;
        }

        elt = (ngx_hash_elt_t *) ((u_char *) buckets[i] + test[i]);

        elt->मूल्य = रिक्त;
    }

    ngx_free(test);

    hinit->hash->buckets = buckets;
    hinit->hash->size = size;

#जर 0

    साठी (i = 0; i < size; i++) {
        ngx_str_t   val;
        ngx_uint_t  किल्ली;

        elt = buckets[i];

        जर (elt == रिक्त) {
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "%ui: NULL", i);
            सुरूठेवा;
        }

        पर्यंत (elt->मूल्य) {
            val.len = elt->len;
            val.data = &elt->name[0];

            किल्ली = hinit->किल्ली(val.data, val.len);

            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "%ui: %p \"%V\" %ui", i, elt, &val, किल्ली);

            elt = (ngx_hash_elt_t *) ngx_align_ptr(&elt->name[0] + elt->len,
                                                   आकार(शून्य *));
        }
    }

#endif

    परत NGX_OK;
}


ngx_int_t
ngx_hash_wildcard_init(ngx_hash_init_t *hinit, ngx_hash_किल्ली_t *names,
    ngx_uint_t nelts)
{
    माप_प्रकार                len, dot_len;
    ngx_uint_t            i, n, dot;
    ngx_array_t           curr_names, next_names;
    ngx_hash_किल्ली_t       *name, *next_name;
    ngx_hash_init_t       h;
    ngx_hash_wildcard_t  *wdc;

    जर (ngx_array_init(&curr_names, hinit->temp_pool, nelts,
                       आकार(ngx_hash_किल्ली_t))
        != NGX_OK)
    {
        परत NGX_ERROR;
    }

    जर (ngx_array_init(&next_names, hinit->temp_pool, nelts,
                       आकार(ngx_hash_किल्ली_t))
        != NGX_OK)
    {
        परत NGX_ERROR;
    }

    साठी (n = 0; n < nelts; n = i) {

#जर 0
        ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                      "wc0: \"%V\"", &names[n].किल्ली);
#endif

        dot = 0;

        साठी (len = 0; len < names[n].किल्ली.len; len++) {
            जर (names[n].किल्ली.data[len] == '.') {
                dot = 1;
                थांबा;
            }
        }

        name = ngx_array_ढकला(&curr_names);
        जर (name == रिक्त) {
            परत NGX_ERROR;
        }

        name->किल्ली.len = len;
        name->किल्ली.data = names[n].किल्ली.data;
        name->किल्ली_hash = hinit->किल्ली(name->किल्ली.data, name->किल्ली.len);
        name->मूल्य = names[n].मूल्य;

#जर 0
        ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                      "wc1: \"%V\" %ui", &name->किल्ली, dot);
#endif

        dot_len = len + 1;

        जर (dot) {
            len++;
        }

        next_names.nelts = 0;

        जर (names[n].किल्ली.len != len) {
            next_name = ngx_array_ढकला(&next_names);
            जर (next_name == रिक्त) {
                परत NGX_ERROR;
            }

            next_name->किल्ली.len = names[n].किल्ली.len - len;
            next_name->किल्ली.data = names[n].किल्ली.data + len;
            next_name->किल्ली_hash = 0;
            next_name->मूल्य = names[n].मूल्य;

#जर 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "wc2: \"%V\"", &next_name->किल्ली);
#endif
        }

        साठी (i = n + 1; i < nelts; i++) {
            जर (ngx_strncmp(names[n].किल्ली.data, names[i].किल्ली.data, len) != 0) {
                थांबा;
            }

            जर (!dot
                && names[i].किल्ली.len > len
                && names[i].किल्ली.data[len] != '.')
            {
                थांबा;
            }

            next_name = ngx_array_ढकला(&next_names);
            जर (next_name == रिक्त) {
                परत NGX_ERROR;
            }

            next_name->किल्ली.len = names[i].किल्ली.len - dot_len;
            next_name->किल्ली.data = names[i].किल्ली.data + dot_len;
            next_name->किल्ली_hash = 0;
            next_name->मूल्य = names[i].मूल्य;

#जर 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "wc3: \"%V\"", &next_name->किल्ली);
#endif
        }

        जर (next_names.nelts) {

            h = *hinit;
            h.hash = रिक्त;

            जर (ngx_hash_wildcard_init(&h, (ngx_hash_किल्ली_t *) next_names.elts,
                                       next_names.nelts)
                != NGX_OK)
            {
                परत NGX_ERROR;
            }

            wdc = (ngx_hash_wildcard_t *) h.hash;

            जर (names[n].किल्ली.len == len) {
                wdc->मूल्य = names[n].मूल्य;
            }

            name->मूल्य = (शून्य *) ((uintptr_t) wdc | (dot ? 3 : 2));

        } अन्यथा जर (dot) {
            name->मूल्य = (शून्य *) ((uintptr_t) name->मूल्य | 1);
        }
    }

    जर (ngx_hash_init(hinit, (ngx_hash_किल्ली_t *) curr_names.elts,
                      curr_names.nelts)
        != NGX_OK)
    {
        परत NGX_ERROR;
    }

    परत NGX_OK;
}


ngx_uint_t
ngx_hash_किल्ली(u_char *data, माप_प्रकार len)
{
    ngx_uint_t  i, किल्ली;

    किल्ली = 0;

    साठी (i = 0; i < len; i++) {
        किल्ली = ngx_hash(किल्ली, data[i]);
    }

    परत किल्ली;
}


ngx_uint_t
ngx_hash_किल्ली_lc(u_char *data, माप_प्रकार len)
{
    ngx_uint_t  i, किल्ली;

    किल्ली = 0;

    साठी (i = 0; i < len; i++) {
        किल्ली = ngx_hash(किल्ली, ngx_tolower(data[i]));
    }

    परत किल्ली;
}


ngx_uint_t
ngx_hash_strlow(u_char *dst, u_char *src, माप_प्रकार n)
{
    ngx_uint_t  किल्ली;

    किल्ली = 0;

    पर्यंत (n--) {
        *dst = ngx_tolower(*src);
        किल्ली = ngx_hash(किल्ली, *dst);
        dst++;
        src++;
    }

    परत किल्ली;
}


ngx_int_t
ngx_hash_किल्ल्या_array_init(ngx_hash_किल्ल्या_arrays_t *ha, ngx_uint_t type)
{
    ngx_uint_t  asize;

    जर (type == NGX_HASH_SMALL) {
        asize = 4;
        ha->hsize = 107;

    } अन्यथा {
        asize = NGX_HASH_LARGE_ASIZE;
        ha->hsize = NGX_HASH_LARGE_HSIZE;
    }

    जर (ngx_array_init(&ha->किल्ल्या, ha->temp_pool, asize, आकार(ngx_hash_किल्ली_t))
        != NGX_OK)
    {
        परत NGX_ERROR;
    }

    जर (ngx_array_init(&ha->dns_wc_head, ha->temp_pool, asize,
                       आकार(ngx_hash_किल्ली_t))
        != NGX_OK)
    {
        परत NGX_ERROR;
    }

    जर (ngx_array_init(&ha->dns_wc_tail, ha->temp_pool, asize,
                       आकार(ngx_hash_किल्ली_t))
        != NGX_OK)
    {
        परत NGX_ERROR;
    }

    ha->किल्ल्या_hash = ngx_pcalloc(ha->temp_pool, आकार(ngx_array_t) * ha->hsize);
    जर (ha->किल्ल्या_hash == रिक्त) {
        परत NGX_ERROR;
    }

    ha->dns_wc_head_hash = ngx_pcalloc(ha->temp_pool,
                                       आकार(ngx_array_t) * ha->hsize);
    जर (ha->dns_wc_head_hash == रिक्त) {
        परत NGX_ERROR;
    }

    ha->dns_wc_tail_hash = ngx_pcalloc(ha->temp_pool,
                                       आकार(ngx_array_t) * ha->hsize);
    जर (ha->dns_wc_tail_hash == रिक्त) {
        परत NGX_ERROR;
    }

    परत NGX_OK;
}


ngx_int_t
ngx_hash_add_किल्ली(ngx_hash_किल्ल्या_arrays_t *ha, ngx_str_t *किल्ली, शून्य *मूल्य,
    ngx_uint_t flags)
{
    माप_प्रकार           len;
    u_char          *p;
    ngx_str_t       *name;
    ngx_uint_t       i, k, n, skip, last;
    ngx_array_t     *किल्ल्या, *hwc;
    ngx_hash_किल्ली_t  *hk;

    last = किल्ली->len;

    जर (flags & NGX_HASH_WILDCARD_किल्ली) {

        /*
         * supported wildcards:
         *     "*.example.com", ".example.com", and "www.example.*"
         */

        n = 0;

        साठी (i = 0; i < किल्ली->len; i++) {

            जर (किल्ली->data[i] == '*') {
                जर (++n > 1) {
                    परत NGX_DECLINED;
                }
            }

            जर (किल्ली->data[i] == '.' && किल्ली->data[i + 1] == '.') {
                परत NGX_DECLINED;
            }

            जर (किल्ली->data[i] == '\0') {
                परत NGX_DECLINED;
            }
        }

        जर (किल्ली->len > 1 && किल्ली->data[0] == '.') {
            skip = 1;
            जा wildcard;
        }

        जर (किल्ली->len > 2) {

            जर (किल्ली->data[0] == '*' && किल्ली->data[1] == '.') {
                skip = 2;
                जा wildcard;
            }

            जर (किल्ली->data[i - 2] == '.' && किल्ली->data[i - 1] == '*') {
                skip = 0;
                last -= 2;
                जा wildcard;
            }
        }

        जर (n) {
            परत NGX_DECLINED;
        }
    }

    /* exact hash */

    k = 0;

    साठी (i = 0; i < last; i++) {
        जर (!(flags & NGX_HASH_READONLY_किल्ली)) {
            किल्ली->data[i] = ngx_tolower(किल्ली->data[i]);
        }
        k = ngx_hash(k, किल्ली->data[i]);
    }

    k %= ha->hsize;

    /* check conflicts in exact hash */

    name = ha->किल्ल्या_hash[k].elts;

    जर (name) {
        साठी (i = 0; i < ha->किल्ल्या_hash[k].nelts; i++) {
            जर (last != name[i].len) {
                सुरूठेवा;
            }

            जर (ngx_strncmp(किल्ली->data, name[i].data, last) == 0) {
                परत NGX_BUSY;
            }
        }

    } अन्यथा {
        जर (ngx_array_init(&ha->किल्ल्या_hash[k], ha->temp_pool, 4,
                           आकार(ngx_str_t))
            != NGX_OK)
        {
            परत NGX_ERROR;
        }
    }

    name = ngx_array_ढकला(&ha->किल्ल्या_hash[k]);
    जर (name == रिक्त) {
        परत NGX_ERROR;
    }

    *name = *किल्ली;

    hk = ngx_array_ढकला(&ha->किल्ल्या);
    जर (hk == रिक्त) {
        परत NGX_ERROR;
    }

    hk->किल्ली = *किल्ली;
    hk->किल्ली_hash = ngx_hash_किल्ली(किल्ली->data, last);
    hk->मूल्य = मूल्य;

    परत NGX_OK;


wildcard:

    /* wildcard hash */

    k = ngx_hash_strlow(&किल्ली->data[skip], &किल्ली->data[skip], last - skip);

    k %= ha->hsize;

    जर (skip == 1) {

        /* check conflicts in exact hash for ".example.com" */

        name = ha->किल्ल्या_hash[k].elts;

        जर (name) {
            len = last - skip;

            साठी (i = 0; i < ha->किल्ल्या_hash[k].nelts; i++) {
                जर (len != name[i].len) {
                    सुरूठेवा;
                }

                जर (ngx_strncmp(&किल्ली->data[1], name[i].data, len) == 0) {
                    परत NGX_BUSY;
                }
            }

        } अन्यथा {
            जर (ngx_array_init(&ha->किल्ल्या_hash[k], ha->temp_pool, 4,
                               आकार(ngx_str_t))
                != NGX_OK)
            {
                परत NGX_ERROR;
            }
        }

        name = ngx_array_ढकला(&ha->किल्ल्या_hash[k]);
        जर (name == रिक्त) {
            परत NGX_ERROR;
        }

        name->len = last - 1;
        name->data = ngx_pnalloc(ha->temp_pool, name->len);
        जर (name->data == रिक्त) {
            परत NGX_ERROR;
        }

        ngx_memcpy(name->data, &किल्ली->data[1], name->len);
    }


    जर (skip) {

        /*
         * convert "*.example.com" to "com.example.\0"
         *      and ".example.com" to "com.example\0"
         */

        p = ngx_pnalloc(ha->temp_pool, last);
        जर (p == रिक्त) {
            परत NGX_ERROR;
        }

        len = 0;
        n = 0;

        साठी (i = last - 1; i; i--) {
            जर (किल्ली->data[i] == '.') {
                ngx_memcpy(&p[n], &किल्ली->data[i + 1], len);
                n += len;
                p[n++] = '.';
                len = 0;
                सुरूठेवा;
            }

            len++;
        }

        जर (len) {
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
        जर (p == रिक्त) {
            परत NGX_ERROR;
        }

        ngx_cpystrn(p, किल्ली->data, last);

        hwc = &ha->dns_wc_tail;
        किल्ल्या = &ha->dns_wc_tail_hash[k];
    }


    /* check conflicts in wildcard hash */

    name = किल्ल्या->elts;

    जर (name) {
        len = last - skip;

        साठी (i = 0; i < किल्ल्या->nelts; i++) {
            जर (len != name[i].len) {
                सुरूठेवा;
            }

            जर (ngx_strncmp(किल्ली->data + skip, name[i].data, len) == 0) {
                परत NGX_BUSY;
            }
        }

    } अन्यथा {
        जर (ngx_array_init(किल्ल्या, ha->temp_pool, 4, आकार(ngx_str_t)) != NGX_OK)
        {
            परत NGX_ERROR;
        }
    }

    name = ngx_array_ढकला(किल्ल्या);
    जर (name == रिक्त) {
        परत NGX_ERROR;
    }

    name->len = last - skip;
    name->data = ngx_pnalloc(ha->temp_pool, name->len);
    जर (name->data == रिक्त) {
        परत NGX_ERROR;
    }

    ngx_memcpy(name->data, किल्ली->data + skip, name->len);


    /* add to wildcard hash */

    hk = ngx_array_ढकला(hwc);
    जर (hk == रिक्त) {
        परत NGX_ERROR;
    }

    hk->किल्ली.len = last - 1;
    hk->किल्ली.data = p;
    hk->किल्ली_hash = 0;
    hk->मूल्य = मूल्य;

    परत NGX_OK;
}
