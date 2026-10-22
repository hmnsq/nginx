
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


शून्य *
ngx_hash_find(ngx_hash_t *hash, ngx_uint_t साँचो, u_char *name, माप_प्रकार len)
{
    ngx_uint_t       i;
    ngx_hash_elt_t  *elt;

#यदि 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "hf:\"%*s\"", len, name);
#endif

    elt = hash->buckets[साँचो % hash->size];

    यदि (elt == रिक्त) {
        फर्काऊ रिक्त;
    }

    जबसम्म (elt->मान) {
        यदि (len != (माप_प्रकार) elt->len) {
            गच्छ next;
        }

        लागि (i = 0; i < len; i++) {
            यदि (name[i] != elt->name[i]) {
                गच्छ next;
            }
        }

        फर्काऊ elt->मान;

    next:

        elt = (ngx_hash_elt_t *) ngx_align_ptr(&elt->name[0] + elt->len,
                                               परिमाण(शून्य *));
        जारी;
    }

    फर्काऊ रिक्त;
}


शून्य *
ngx_hash_find_wc_head(ngx_hash_wildcard_t *hwc, u_char *name, माप_प्रकार len)
{
    शून्य        *मान;
    ngx_uint_t   i, n, साँचो;

#यदि 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "wch:\"%*s\"", len, name);
#endif

    n = len;

    जबसम्म (n) {
        यदि (name[n - 1] == '.') {
            रोक;
        }

        n--;
    }

    साँचो = 0;

    लागि (i = n; i < len; i++) {
        साँचो = ngx_hash(साँचो, name[i]);
    }

#यदि 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "key:\"%ui\"", साँचो);
#endif

    मान = ngx_hash_find(&hwc->hash, साँचो, &name[n], len - n);

#यदि 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "value:\"%p\"", मान);
#endif

    यदि (मान) {

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

        यदि ((uintptr_t) मान & 2) {

            यदि (n == 0) {

                /* "example.com" */

                यदि ((uintptr_t) मान & 1) {
                    फर्काऊ रिक्त;
                }

                hwc = (ngx_hash_wildcard_t *)
                                          ((uintptr_t) मान & (uintptr_t) ~3);
                फर्काऊ hwc->मान;
            }

            hwc = (ngx_hash_wildcard_t *) ((uintptr_t) मान & (uintptr_t) ~3);

            मान = ngx_hash_find_wc_head(hwc, name, n - 1);

            यदि (मान) {
                फर्काऊ मान;
            }

            फर्काऊ hwc->मान;
        }

        यदि ((uintptr_t) मान & 1) {

            यदि (n == 0) {

                /* "example.com" */

                फर्काऊ रिक्त;
            }

            फर्काऊ (शून्य *) ((uintptr_t) मान & (uintptr_t) ~3);
        }

        फर्काऊ मान;
    }

    फर्काऊ hwc->मान;
}


शून्य *
ngx_hash_find_wc_tail(ngx_hash_wildcard_t *hwc, u_char *name, माप_प्रकार len)
{
    शून्य        *मान;
    ngx_uint_t   i, साँचो;

#यदि 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "wct:\"%*s\"", len, name);
#endif

    साँचो = 0;

    लागि (i = 0; i < len; i++) {
        यदि (name[i] == '.') {
            रोक;
        }

        साँचो = ngx_hash(साँचो, name[i]);
    }

    यदि (i == len) {
        फर्काऊ रिक्त;
    }

#यदि 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "key:\"%ui\"", साँचो);
#endif

    मान = ngx_hash_find(&hwc->hash, साँचो, name, i);

#यदि 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "value:\"%p\"", मान);
#endif

    यदि (मान) {

        /*
         * the 2 low bits of value have the special meaning:
         *     00 - value is data pointer;
         *     11 - value is pointer to wildcard hash allowing "example.*".
         */

        यदि ((uintptr_t) मान & 2) {

            i++;

            hwc = (ngx_hash_wildcard_t *) ((uintptr_t) मान & (uintptr_t) ~3);

            मान = ngx_hash_find_wc_tail(hwc, &name[i], len - i);

            यदि (मान) {
                फर्काऊ मान;
            }

            फर्काऊ hwc->मान;
        }

        फर्काऊ मान;
    }

    फर्काऊ hwc->मान;
}


शून्य *
ngx_hash_find_combined(ngx_hash_combined_t *hash, ngx_uint_t साँचो, u_char *name,
    माप_प्रकार len)
{
    शून्य  *मान;

    यदि (hash->hash.buckets) {
        मान = ngx_hash_find(&hash->hash, साँचो, name, len);

        यदि (मान) {
            फर्काऊ मान;
        }
    }

    यदि (len == 0) {
        फर्काऊ रिक्त;
    }

    यदि (hash->wc_head && hash->wc_head->hash.buckets) {
        मान = ngx_hash_find_wc_head(hash->wc_head, name, len);

        यदि (मान) {
            फर्काऊ मान;
        }
    }

    यदि (hash->wc_tail && hash->wc_tail->hash.buckets) {
        मान = ngx_hash_find_wc_tail(hash->wc_tail, name, len);

        यदि (मान) {
            फर्काऊ मान;
        }
    }

    फर्काऊ रिक्त;
}


#define NGX_HASH_ELT_SIZE(name)                                               \
    (परिमाण(शून्य *) + ngx_align((name)->साँचो.len + 2, परिमाण(शून्य *)))

ngx_int_t
ngx_hash_init(ngx_hash_init_t *hinit, ngx_hash_साँचो_t *names, ngx_uint_t nelts)
{
    u_char          *elts;
    माप_प्रकार           len;
    u_short         *test;
    ngx_uint_t       i, n, साँचो, size, start, bucket_size;
    ngx_hash_elt_t  *elt, **buckets;

    यदि (hinit->max_size == 0) {
        ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                      "could not build %s, you should "
                      "increase %s_max_size: %i",
                      hinit->name, hinit->name, hinit->max_size);
        फर्काऊ NGX_ERROR;
    }

    यदि (hinit->bucket_size > 65536 - ngx_cacheline_size) {
        ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                      "could not build %s, too large "
                      "%s_bucket_size: %i",
                      hinit->name, hinit->name, hinit->bucket_size);
        फर्काऊ NGX_ERROR;
    }

    लागि (n = 0; n < nelts; n++) {
        यदि (names[n].साँचो.data == रिक्त) {
            जारी;
        }

        यदि (hinit->bucket_size < NGX_HASH_ELT_SIZE(&names[n]) + परिमाण(शून्य *))
        {
            ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                          "could not build %s, you should "
                          "increase %s_bucket_size: %i",
                          hinit->name, hinit->name, hinit->bucket_size);
            फर्काऊ NGX_ERROR;
        }
    }

    test = ngx_alloc(hinit->max_size * परिमाण(u_short), hinit->pool->log);
    यदि (test == रिक्त) {
        फर्काऊ NGX_ERROR;
    }

    bucket_size = hinit->bucket_size - परिमाण(शून्य *);

    start = nelts / (bucket_size / (2 * परिमाण(शून्य *)));
    start = start ? start : 1;

    यदि (hinit->max_size > 10000 && nelts && hinit->max_size / nelts < 100) {
        start = hinit->max_size - 1000;
    }

    लागि (size = start; size <= hinit->max_size; size++) {

        ngx_memzero(test, size * परिमाण(u_short));

        लागि (n = 0; n < nelts; n++) {
            यदि (names[n].साँचो.data == रिक्त) {
                जारी;
            }

            साँचो = names[n].साँचो_hash % size;
            len = test[साँचो] + NGX_HASH_ELT_SIZE(&names[n]);

#यदि 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "%ui: %ui %uz \"%V\"",
                          size, साँचो, len, &names[n].साँचो);
#endif

            यदि (len > bucket_size) {
                गच्छ next;
            }

            test[साँचो] = (u_short) len;
        }

        गच्छ found;

    next:

        जारी;
    }

    size = hinit->max_size;

    ngx_log_error(NGX_LOG_WARN, hinit->pool->log, 0,
                  "could not build optimal %s, you should increase "
                  "either %s_max_size: %i or %s_bucket_size: %i; "
                  "ignoring %s_bucket_size",
                  hinit->name, hinit->name, hinit->max_size,
                  hinit->name, hinit->bucket_size, hinit->name);

found:

    लागि (i = 0; i < size; i++) {
        test[i] = परिमाण(शून्य *);
    }

    लागि (n = 0; n < nelts; n++) {
        यदि (names[n].साँचो.data == रिक्त) {
            जारी;
        }

        साँचो = names[n].साँचो_hash % size;
        len = test[साँचो] + NGX_HASH_ELT_SIZE(&names[n]);

        यदि (len > 65536 - ngx_cacheline_size) {
            ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                          "could not build %s, you should "
                          "increase %s_max_size: %i",
                          hinit->name, hinit->name, hinit->max_size);
            ngx_free(test);
            फर्काऊ NGX_ERROR;
        }

        test[साँचो] = (u_short) len;
    }

    len = 0;

    लागि (i = 0; i < size; i++) {
        यदि (test[i] == परिमाण(शून्य *)) {
            जारी;
        }

        test[i] = (u_short) (ngx_align(test[i], ngx_cacheline_size));

        len += test[i];
    }

    यदि (hinit->hash == रिक्त) {
        hinit->hash = ngx_pcalloc(hinit->pool, परिमाण(ngx_hash_wildcard_t)
                                             + size * परिमाण(ngx_hash_elt_t *));
        यदि (hinit->hash == रिक्त) {
            ngx_free(test);
            फर्काऊ NGX_ERROR;
        }

        buckets = (ngx_hash_elt_t **)
                      ((u_char *) hinit->hash + परिमाण(ngx_hash_wildcard_t));

    } अन्यथा {
        buckets = ngx_pcalloc(hinit->pool, size * परिमाण(ngx_hash_elt_t *));
        यदि (buckets == रिक्त) {
            ngx_free(test);
            फर्काऊ NGX_ERROR;
        }
    }

    elts = ngx_palloc(hinit->pool, len + ngx_cacheline_size);
    यदि (elts == रिक्त) {
        ngx_free(test);
        फर्काऊ NGX_ERROR;
    }

    elts = ngx_align_ptr(elts, ngx_cacheline_size);

    लागि (i = 0; i < size; i++) {
        यदि (test[i] == परिमाण(शून्य *)) {
            जारी;
        }

        buckets[i] = (ngx_hash_elt_t *) elts;
        elts += test[i];
    }

    लागि (i = 0; i < size; i++) {
        test[i] = 0;
    }

    लागि (n = 0; n < nelts; n++) {
        यदि (names[n].साँचो.data == रिक्त) {
            जारी;
        }

        साँचो = names[n].साँचो_hash % size;
        elt = (ngx_hash_elt_t *) ((u_char *) buckets[साँचो] + test[साँचो]);

        elt->मान = names[n].मान;
        elt->len = (u_short) names[n].साँचो.len;

        ngx_strlow(elt->name, names[n].साँचो.data, names[n].साँचो.len);

        test[साँचो] = (u_short) (test[साँचो] + NGX_HASH_ELT_SIZE(&names[n]));
    }

    लागि (i = 0; i < size; i++) {
        यदि (buckets[i] == रिक्त) {
            जारी;
        }

        elt = (ngx_hash_elt_t *) ((u_char *) buckets[i] + test[i]);

        elt->मान = रिक्त;
    }

    ngx_free(test);

    hinit->hash->buckets = buckets;
    hinit->hash->size = size;

#यदि 0

    लागि (i = 0; i < size; i++) {
        ngx_str_t   val;
        ngx_uint_t  साँचो;

        elt = buckets[i];

        यदि (elt == रिक्त) {
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "%ui: NULL", i);
            जारी;
        }

        जबसम्म (elt->मान) {
            val.len = elt->len;
            val.data = &elt->name[0];

            साँचो = hinit->साँचो(val.data, val.len);

            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "%ui: %p \"%V\" %ui", i, elt, &val, साँचो);

            elt = (ngx_hash_elt_t *) ngx_align_ptr(&elt->name[0] + elt->len,
                                                   परिमाण(शून्य *));
        }
    }

#endif

    फर्काऊ NGX_OK;
}


ngx_int_t
ngx_hash_wildcard_init(ngx_hash_init_t *hinit, ngx_hash_साँचो_t *names,
    ngx_uint_t nelts)
{
    माप_प्रकार                len, dot_len;
    ngx_uint_t            i, n, dot;
    ngx_array_t           curr_names, next_names;
    ngx_hash_साँचो_t       *name, *next_name;
    ngx_hash_init_t       h;
    ngx_hash_wildcard_t  *wdc;

    यदि (ngx_array_init(&curr_names, hinit->temp_pool, nelts,
                       परिमाण(ngx_hash_साँचो_t))
        != NGX_OK)
    {
        फर्काऊ NGX_ERROR;
    }

    यदि (ngx_array_init(&next_names, hinit->temp_pool, nelts,
                       परिमाण(ngx_hash_साँचो_t))
        != NGX_OK)
    {
        फर्काऊ NGX_ERROR;
    }

    लागि (n = 0; n < nelts; n = i) {

#यदि 0
        ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                      "wc0: \"%V\"", &names[n].साँचो);
#endif

        dot = 0;

        लागि (len = 0; len < names[n].साँचो.len; len++) {
            यदि (names[n].साँचो.data[len] == '.') {
                dot = 1;
                रोक;
            }
        }

        name = ngx_array_थप(&curr_names);
        यदि (name == रिक्त) {
            फर्काऊ NGX_ERROR;
        }

        name->साँचो.len = len;
        name->साँचो.data = names[n].साँचो.data;
        name->साँचो_hash = hinit->साँचो(name->साँचो.data, name->साँचो.len);
        name->मान = names[n].मान;

#यदि 0
        ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                      "wc1: \"%V\" %ui", &name->साँचो, dot);
#endif

        dot_len = len + 1;

        यदि (dot) {
            len++;
        }

        next_names.nelts = 0;

        यदि (names[n].साँचो.len != len) {
            next_name = ngx_array_थप(&next_names);
            यदि (next_name == रिक्त) {
                फर्काऊ NGX_ERROR;
            }

            next_name->साँचो.len = names[n].साँचो.len - len;
            next_name->साँचो.data = names[n].साँचो.data + len;
            next_name->साँचो_hash = 0;
            next_name->मान = names[n].मान;

#यदि 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "wc2: \"%V\"", &next_name->साँचो);
#endif
        }

        लागि (i = n + 1; i < nelts; i++) {
            यदि (ngx_strncmp(names[n].साँचो.data, names[i].साँचो.data, len) != 0) {
                रोक;
            }

            यदि (!dot
                && names[i].साँचो.len > len
                && names[i].साँचो.data[len] != '.')
            {
                रोक;
            }

            next_name = ngx_array_थप(&next_names);
            यदि (next_name == रिक्त) {
                फर्काऊ NGX_ERROR;
            }

            next_name->साँचो.len = names[i].साँचो.len - dot_len;
            next_name->साँचो.data = names[i].साँचो.data + dot_len;
            next_name->साँचो_hash = 0;
            next_name->मान = names[i].मान;

#यदि 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "wc3: \"%V\"", &next_name->साँचो);
#endif
        }

        यदि (next_names.nelts) {

            h = *hinit;
            h.hash = रिक्त;

            यदि (ngx_hash_wildcard_init(&h, (ngx_hash_साँचो_t *) next_names.elts,
                                       next_names.nelts)
                != NGX_OK)
            {
                फर्काऊ NGX_ERROR;
            }

            wdc = (ngx_hash_wildcard_t *) h.hash;

            यदि (names[n].साँचो.len == len) {
                wdc->मान = names[n].मान;
            }

            name->मान = (शून्य *) ((uintptr_t) wdc | (dot ? 3 : 2));

        } अन्यथा यदि (dot) {
            name->मान = (शून्य *) ((uintptr_t) name->मान | 1);
        }
    }

    यदि (ngx_hash_init(hinit, (ngx_hash_साँचो_t *) curr_names.elts,
                      curr_names.nelts)
        != NGX_OK)
    {
        फर्काऊ NGX_ERROR;
    }

    फर्काऊ NGX_OK;
}


ngx_uint_t
ngx_hash_साँचो(u_char *data, माप_प्रकार len)
{
    ngx_uint_t  i, साँचो;

    साँचो = 0;

    लागि (i = 0; i < len; i++) {
        साँचो = ngx_hash(साँचो, data[i]);
    }

    फर्काऊ साँचो;
}


ngx_uint_t
ngx_hash_साँचो_lc(u_char *data, माप_प्रकार len)
{
    ngx_uint_t  i, साँचो;

    साँचो = 0;

    लागि (i = 0; i < len; i++) {
        साँचो = ngx_hash(साँचो, ngx_tolower(data[i]));
    }

    फर्काऊ साँचो;
}


ngx_uint_t
ngx_hash_strlow(u_char *dst, u_char *src, माप_प्रकार n)
{
    ngx_uint_t  साँचो;

    साँचो = 0;

    जबसम्म (n--) {
        *dst = ngx_tolower(*src);
        साँचो = ngx_hash(साँचो, *dst);
        dst++;
        src++;
    }

    फर्काऊ साँचो;
}


ngx_int_t
ngx_hash_साँचोहरू_array_init(ngx_hash_साँचोहरू_arrays_t *ha, ngx_uint_t type)
{
    ngx_uint_t  asize;

    यदि (type == NGX_HASH_SMALL) {
        asize = 4;
        ha->hsize = 107;

    } अन्यथा {
        asize = NGX_HASH_LARGE_ASIZE;
        ha->hsize = NGX_HASH_LARGE_HSIZE;
    }

    यदि (ngx_array_init(&ha->साँचोहरू, ha->temp_pool, asize, परिमाण(ngx_hash_साँचो_t))
        != NGX_OK)
    {
        फर्काऊ NGX_ERROR;
    }

    यदि (ngx_array_init(&ha->dns_wc_head, ha->temp_pool, asize,
                       परिमाण(ngx_hash_साँचो_t))
        != NGX_OK)
    {
        फर्काऊ NGX_ERROR;
    }

    यदि (ngx_array_init(&ha->dns_wc_tail, ha->temp_pool, asize,
                       परिमाण(ngx_hash_साँचो_t))
        != NGX_OK)
    {
        फर्काऊ NGX_ERROR;
    }

    ha->साँचोहरू_hash = ngx_pcalloc(ha->temp_pool, परिमाण(ngx_array_t) * ha->hsize);
    यदि (ha->साँचोहरू_hash == रिक्त) {
        फर्काऊ NGX_ERROR;
    }

    ha->dns_wc_head_hash = ngx_pcalloc(ha->temp_pool,
                                       परिमाण(ngx_array_t) * ha->hsize);
    यदि (ha->dns_wc_head_hash == रिक्त) {
        फर्काऊ NGX_ERROR;
    }

    ha->dns_wc_tail_hash = ngx_pcalloc(ha->temp_pool,
                                       परिमाण(ngx_array_t) * ha->hsize);
    यदि (ha->dns_wc_tail_hash == रिक्त) {
        फर्काऊ NGX_ERROR;
    }

    फर्काऊ NGX_OK;
}


ngx_int_t
ngx_hash_add_साँचो(ngx_hash_साँचोहरू_arrays_t *ha, ngx_str_t *साँचो, शून्य *मान,
    ngx_uint_t flags)
{
    माप_प्रकार           len;
    u_char          *p;
    ngx_str_t       *name;
    ngx_uint_t       i, k, n, skip, last;
    ngx_array_t     *साँचोहरू, *hwc;
    ngx_hash_साँचो_t  *hk;

    last = साँचो->len;

    यदि (flags & NGX_HASH_WILDCARD_साँचो) {

        /*
         * supported wildcards:
         *     "*.example.com", ".example.com", and "www.example.*"
         */

        n = 0;

        लागि (i = 0; i < साँचो->len; i++) {

            यदि (साँचो->data[i] == '*') {
                यदि (++n > 1) {
                    फर्काऊ NGX_DECLINED;
                }
            }

            यदि (साँचो->data[i] == '.' && साँचो->data[i + 1] == '.') {
                फर्काऊ NGX_DECLINED;
            }

            यदि (साँचो->data[i] == '\0') {
                फर्काऊ NGX_DECLINED;
            }
        }

        यदि (साँचो->len > 1 && साँचो->data[0] == '.') {
            skip = 1;
            गच्छ wildcard;
        }

        यदि (साँचो->len > 2) {

            यदि (साँचो->data[0] == '*' && साँचो->data[1] == '.') {
                skip = 2;
                गच्छ wildcard;
            }

            यदि (साँचो->data[i - 2] == '.' && साँचो->data[i - 1] == '*') {
                skip = 0;
                last -= 2;
                गच्छ wildcard;
            }
        }

        यदि (n) {
            फर्काऊ NGX_DECLINED;
        }
    }

    /* exact hash */

    k = 0;

    लागि (i = 0; i < last; i++) {
        यदि (!(flags & NGX_HASH_READONLY_साँचो)) {
            साँचो->data[i] = ngx_tolower(साँचो->data[i]);
        }
        k = ngx_hash(k, साँचो->data[i]);
    }

    k %= ha->hsize;

    /* check conflicts in exact hash */

    name = ha->साँचोहरू_hash[k].elts;

    यदि (name) {
        लागि (i = 0; i < ha->साँचोहरू_hash[k].nelts; i++) {
            यदि (last != name[i].len) {
                जारी;
            }

            यदि (ngx_strncmp(साँचो->data, name[i].data, last) == 0) {
                फर्काऊ NGX_BUSY;
            }
        }

    } अन्यथा {
        यदि (ngx_array_init(&ha->साँचोहरू_hash[k], ha->temp_pool, 4,
                           परिमाण(ngx_str_t))
            != NGX_OK)
        {
            फर्काऊ NGX_ERROR;
        }
    }

    name = ngx_array_थप(&ha->साँचोहरू_hash[k]);
    यदि (name == रिक्त) {
        फर्काऊ NGX_ERROR;
    }

    *name = *साँचो;

    hk = ngx_array_थप(&ha->साँचोहरू);
    यदि (hk == रिक्त) {
        फर्काऊ NGX_ERROR;
    }

    hk->साँचो = *साँचो;
    hk->साँचो_hash = ngx_hash_साँचो(साँचो->data, last);
    hk->मान = मान;

    फर्काऊ NGX_OK;


wildcard:

    /* wildcard hash */

    k = ngx_hash_strlow(&साँचो->data[skip], &साँचो->data[skip], last - skip);

    k %= ha->hsize;

    यदि (skip == 1) {

        /* check conflicts in exact hash for ".example.com" */

        name = ha->साँचोहरू_hash[k].elts;

        यदि (name) {
            len = last - skip;

            लागि (i = 0; i < ha->साँचोहरू_hash[k].nelts; i++) {
                यदि (len != name[i].len) {
                    जारी;
                }

                यदि (ngx_strncmp(&साँचो->data[1], name[i].data, len) == 0) {
                    फर्काऊ NGX_BUSY;
                }
            }

        } अन्यथा {
            यदि (ngx_array_init(&ha->साँचोहरू_hash[k], ha->temp_pool, 4,
                               परिमाण(ngx_str_t))
                != NGX_OK)
            {
                फर्काऊ NGX_ERROR;
            }
        }

        name = ngx_array_थप(&ha->साँचोहरू_hash[k]);
        यदि (name == रिक्त) {
            फर्काऊ NGX_ERROR;
        }

        name->len = last - 1;
        name->data = ngx_pnalloc(ha->temp_pool, name->len);
        यदि (name->data == रिक्त) {
            फर्काऊ NGX_ERROR;
        }

        ngx_memcpy(name->data, &साँचो->data[1], name->len);
    }


    यदि (skip) {

        /*
         * convert "*.example.com" to "com.example.\0"
         *      and ".example.com" to "com.example\0"
         */

        p = ngx_pnalloc(ha->temp_pool, last);
        यदि (p == रिक्त) {
            फर्काऊ NGX_ERROR;
        }

        len = 0;
        n = 0;

        लागि (i = last - 1; i; i--) {
            यदि (साँचो->data[i] == '.') {
                ngx_memcpy(&p[n], &साँचो->data[i + 1], len);
                n += len;
                p[n++] = '.';
                len = 0;
                जारी;
            }

            len++;
        }

        यदि (len) {
            ngx_memcpy(&p[n], &साँचो->data[1], len);
            n += len;
        }

        p[n] = '\0';

        hwc = &ha->dns_wc_head;
        साँचोहरू = &ha->dns_wc_head_hash[k];

    } अन्यथा {

        /* convert "www.example.*" to "www.example\0" */

        last++;

        p = ngx_pnalloc(ha->temp_pool, last);
        यदि (p == रिक्त) {
            फर्काऊ NGX_ERROR;
        }

        ngx_cpystrn(p, साँचो->data, last);

        hwc = &ha->dns_wc_tail;
        साँचोहरू = &ha->dns_wc_tail_hash[k];
    }


    /* check conflicts in wildcard hash */

    name = साँचोहरू->elts;

    यदि (name) {
        len = last - skip;

        लागि (i = 0; i < साँचोहरू->nelts; i++) {
            यदि (len != name[i].len) {
                जारी;
            }

            यदि (ngx_strncmp(साँचो->data + skip, name[i].data, len) == 0) {
                फर्काऊ NGX_BUSY;
            }
        }

    } अन्यथा {
        यदि (ngx_array_init(साँचोहरू, ha->temp_pool, 4, परिमाण(ngx_str_t)) != NGX_OK)
        {
            फर्काऊ NGX_ERROR;
        }
    }

    name = ngx_array_थप(साँचोहरू);
    यदि (name == रिक्त) {
        फर्काऊ NGX_ERROR;
    }

    name->len = last - skip;
    name->data = ngx_pnalloc(ha->temp_pool, name->len);
    यदि (name->data == रिक्त) {
        फर्काऊ NGX_ERROR;
    }

    ngx_memcpy(name->data, साँचो->data + skip, name->len);


    /* add to wildcard hash */

    hk = ngx_array_थप(hwc);
    यदि (hk == रिक्त) {
        फर्काऊ NGX_ERROR;
    }

    hk->साँचो.len = last - 1;
    hk->साँचो.data = p;
    hk->साँचो_hash = 0;
    hk->मान = मान;

    फर्काऊ NGX_OK;
}
