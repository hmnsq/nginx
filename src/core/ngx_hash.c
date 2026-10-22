
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#شامل <ngx_config.h>
#شامل <ngx_core.h>


تهی *
ngx_hash_find(ngx_hash_t *hash, ngx_uint_t کلید, u_char *name, size_t len)
{
    ngx_uint_t       i;
    ngx_hash_elt_t  *elt;

#اگر 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "hf:\"%*s\"", len, name);
#endif

    elt = hash->buckets[کلید % hash->size];

    اگر (elt == پوچ) {
        بازگشت پوچ;
    }

    وقتی (elt->مقدار) {
        اگر (len != (size_t) elt->len) {
            برو next;
        }

        برای (i = 0; i < len; i++) {
            اگر (name[i] != elt->name[i]) {
                برو next;
            }
        }

        بازگشت elt->مقدار;

    next:

        elt = (ngx_hash_elt_t *) ngx_align_ptr(&elt->name[0] + elt->len,
                                               اندازه(تهی *));
        ادامه;
    }

    بازگشت پوچ;
}


تهی *
ngx_hash_find_wc_head(ngx_hash_wildcard_t *hwc, u_char *name, size_t len)
{
    تهی        *مقدار;
    ngx_uint_t   i, n, کلید;

#اگر 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "wch:\"%*s\"", len, name);
#endif

    n = len;

    وقتی (n) {
        اگر (name[n - 1] == '.') {
            بشکن;
        }

        n--;
    }

    کلید = 0;

    برای (i = n; i < len; i++) {
        کلید = ngx_hash(کلید, name[i]);
    }

#اگر 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "key:\"%ui\"", کلید);
#endif

    مقدار = ngx_hash_find(&hwc->hash, کلید, &name[n], len - n);

#اگر 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "value:\"%p\"", مقدار);
#endif

    اگر (مقدار) {

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

        اگر ((uintptr_t) مقدار & 2) {

            اگر (n == 0) {

                /* "example.com" */

                اگر ((uintptr_t) مقدار & 1) {
                    بازگشت پوچ;
                }

                hwc = (ngx_hash_wildcard_t *)
                                          ((uintptr_t) مقدار & (uintptr_t) ~3);
                بازگشت hwc->مقدار;
            }

            hwc = (ngx_hash_wildcard_t *) ((uintptr_t) مقدار & (uintptr_t) ~3);

            مقدار = ngx_hash_find_wc_head(hwc, name, n - 1);

            اگر (مقدار) {
                بازگشت مقدار;
            }

            بازگشت hwc->مقدار;
        }

        اگر ((uintptr_t) مقدار & 1) {

            اگر (n == 0) {

                /* "example.com" */

                بازگشت پوچ;
            }

            بازگشت (تهی *) ((uintptr_t) مقدار & (uintptr_t) ~3);
        }

        بازگشت مقدار;
    }

    بازگشت hwc->مقدار;
}


تهی *
ngx_hash_find_wc_tail(ngx_hash_wildcard_t *hwc, u_char *name, size_t len)
{
    تهی        *مقدار;
    ngx_uint_t   i, کلید;

#اگر 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "wct:\"%*s\"", len, name);
#endif

    کلید = 0;

    برای (i = 0; i < len; i++) {
        اگر (name[i] == '.') {
            بشکن;
        }

        کلید = ngx_hash(کلید, name[i]);
    }

    اگر (i == len) {
        بازگشت پوچ;
    }

#اگر 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "key:\"%ui\"", کلید);
#endif

    مقدار = ngx_hash_find(&hwc->hash, کلید, name, i);

#اگر 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "value:\"%p\"", مقدار);
#endif

    اگر (مقدار) {

        /*
         * the 2 low bits of value have the special meaning:
         *     00 - value is data pointer;
         *     11 - value is pointer to wildcard hash allowing "example.*".
         */

        اگر ((uintptr_t) مقدار & 2) {

            i++;

            hwc = (ngx_hash_wildcard_t *) ((uintptr_t) مقدار & (uintptr_t) ~3);

            مقدار = ngx_hash_find_wc_tail(hwc, &name[i], len - i);

            اگر (مقدار) {
                بازگشت مقدار;
            }

            بازگشت hwc->مقدار;
        }

        بازگشت مقدار;
    }

    بازگشت hwc->مقدار;
}


تهی *
ngx_hash_find_combined(ngx_hash_combined_t *hash, ngx_uint_t کلید, u_char *name,
    size_t len)
{
    تهی  *مقدار;

    اگر (hash->hash.buckets) {
        مقدار = ngx_hash_find(&hash->hash, کلید, name, len);

        اگر (مقدار) {
            بازگشت مقدار;
        }
    }

    اگر (len == 0) {
        بازگشت پوچ;
    }

    اگر (hash->wc_head && hash->wc_head->hash.buckets) {
        مقدار = ngx_hash_find_wc_head(hash->wc_head, name, len);

        اگر (مقدار) {
            بازگشت مقدار;
        }
    }

    اگر (hash->wc_tail && hash->wc_tail->hash.buckets) {
        مقدار = ngx_hash_find_wc_tail(hash->wc_tail, name, len);

        اگر (مقدار) {
            بازگشت مقدار;
        }
    }

    بازگشت پوچ;
}


#define NGX_HASH_ELT_SIZE(name)                                               \
    (اندازه(تهی *) + ngx_align((name)->کلید.len + 2, اندازه(تهی *)))

ngx_int_t
ngx_hash_init(ngx_hash_init_t *hinit, ngx_hash_کلید_t *names, ngx_uint_t nelts)
{
    u_char          *elts;
    size_t           len;
    u_short         *test;
    ngx_uint_t       i, n, کلید, size, start, bucket_size;
    ngx_hash_elt_t  *elt, **buckets;

    اگر (hinit->max_size == 0) {
        ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                      "could not build %s, you should "
                      "increase %s_max_size: %i",
                      hinit->name, hinit->name, hinit->max_size);
        بازگشت NGX_ERROR;
    }

    اگر (hinit->bucket_size > 65536 - ngx_cacheline_size) {
        ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                      "could not build %s, too large "
                      "%s_bucket_size: %i",
                      hinit->name, hinit->name, hinit->bucket_size);
        بازگشت NGX_ERROR;
    }

    برای (n = 0; n < nelts; n++) {
        اگر (names[n].کلید.data == پوچ) {
            ادامه;
        }

        اگر (hinit->bucket_size < NGX_HASH_ELT_SIZE(&names[n]) + اندازه(تهی *))
        {
            ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                          "could not build %s, you should "
                          "increase %s_bucket_size: %i",
                          hinit->name, hinit->name, hinit->bucket_size);
            بازگشت NGX_ERROR;
        }
    }

    test = ngx_alloc(hinit->max_size * اندازه(u_short), hinit->pool->log);
    اگر (test == پوچ) {
        بازگشت NGX_ERROR;
    }

    bucket_size = hinit->bucket_size - اندازه(تهی *);

    start = nelts / (bucket_size / (2 * اندازه(تهی *)));
    start = start ? start : 1;

    اگر (hinit->max_size > 10000 && nelts && hinit->max_size / nelts < 100) {
        start = hinit->max_size - 1000;
    }

    برای (size = start; size <= hinit->max_size; size++) {

        ngx_memzero(test, size * اندازه(u_short));

        برای (n = 0; n < nelts; n++) {
            اگر (names[n].کلید.data == پوچ) {
                ادامه;
            }

            کلید = names[n].کلید_hash % size;
            len = test[کلید] + NGX_HASH_ELT_SIZE(&names[n]);

#اگر 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "%ui: %ui %uz \"%V\"",
                          size, کلید, len, &names[n].کلید);
#endif

            اگر (len > bucket_size) {
                برو next;
            }

            test[کلید] = (u_short) len;
        }

        برو found;

    next:

        ادامه;
    }

    size = hinit->max_size;

    ngx_log_error(NGX_LOG_WARN, hinit->pool->log, 0,
                  "could not build optimal %s, you should increase "
                  "either %s_max_size: %i or %s_bucket_size: %i; "
                  "ignoring %s_bucket_size",
                  hinit->name, hinit->name, hinit->max_size,
                  hinit->name, hinit->bucket_size, hinit->name);

found:

    برای (i = 0; i < size; i++) {
        test[i] = اندازه(تهی *);
    }

    برای (n = 0; n < nelts; n++) {
        اگر (names[n].کلید.data == پوچ) {
            ادامه;
        }

        کلید = names[n].کلید_hash % size;
        len = test[کلید] + NGX_HASH_ELT_SIZE(&names[n]);

        اگر (len > 65536 - ngx_cacheline_size) {
            ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                          "could not build %s, you should "
                          "increase %s_max_size: %i",
                          hinit->name, hinit->name, hinit->max_size);
            ngx_free(test);
            بازگشت NGX_ERROR;
        }

        test[کلید] = (u_short) len;
    }

    len = 0;

    برای (i = 0; i < size; i++) {
        اگر (test[i] == اندازه(تهی *)) {
            ادامه;
        }

        test[i] = (u_short) (ngx_align(test[i], ngx_cacheline_size));

        len += test[i];
    }

    اگر (hinit->hash == پوچ) {
        hinit->hash = ngx_pcalloc(hinit->pool, اندازه(ngx_hash_wildcard_t)
                                             + size * اندازه(ngx_hash_elt_t *));
        اگر (hinit->hash == پوچ) {
            ngx_free(test);
            بازگشت NGX_ERROR;
        }

        buckets = (ngx_hash_elt_t **)
                      ((u_char *) hinit->hash + اندازه(ngx_hash_wildcard_t));

    } وگرنه {
        buckets = ngx_pcalloc(hinit->pool, size * اندازه(ngx_hash_elt_t *));
        اگر (buckets == پوچ) {
            ngx_free(test);
            بازگشت NGX_ERROR;
        }
    }

    elts = ngx_palloc(hinit->pool, len + ngx_cacheline_size);
    اگر (elts == پوچ) {
        ngx_free(test);
        بازگشت NGX_ERROR;
    }

    elts = ngx_align_ptr(elts, ngx_cacheline_size);

    برای (i = 0; i < size; i++) {
        اگر (test[i] == اندازه(تهی *)) {
            ادامه;
        }

        buckets[i] = (ngx_hash_elt_t *) elts;
        elts += test[i];
    }

    برای (i = 0; i < size; i++) {
        test[i] = 0;
    }

    برای (n = 0; n < nelts; n++) {
        اگر (names[n].کلید.data == پوچ) {
            ادامه;
        }

        کلید = names[n].کلید_hash % size;
        elt = (ngx_hash_elt_t *) ((u_char *) buckets[کلید] + test[کلید]);

        elt->مقدار = names[n].مقدار;
        elt->len = (u_short) names[n].کلید.len;

        ngx_strlow(elt->name, names[n].کلید.data, names[n].کلید.len);

        test[کلید] = (u_short) (test[کلید] + NGX_HASH_ELT_SIZE(&names[n]));
    }

    برای (i = 0; i < size; i++) {
        اگر (buckets[i] == پوچ) {
            ادامه;
        }

        elt = (ngx_hash_elt_t *) ((u_char *) buckets[i] + test[i]);

        elt->مقدار = پوچ;
    }

    ngx_free(test);

    hinit->hash->buckets = buckets;
    hinit->hash->size = size;

#اگر 0

    برای (i = 0; i < size; i++) {
        ngx_str_t   val;
        ngx_uint_t  کلید;

        elt = buckets[i];

        اگر (elt == پوچ) {
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "%ui: NULL", i);
            ادامه;
        }

        وقتی (elt->مقدار) {
            val.len = elt->len;
            val.data = &elt->name[0];

            کلید = hinit->کلید(val.data, val.len);

            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "%ui: %p \"%V\" %ui", i, elt, &val, کلید);

            elt = (ngx_hash_elt_t *) ngx_align_ptr(&elt->name[0] + elt->len,
                                                   اندازه(تهی *));
        }
    }

#endif

    بازگشت NGX_OK;
}


ngx_int_t
ngx_hash_wildcard_init(ngx_hash_init_t *hinit, ngx_hash_کلید_t *names,
    ngx_uint_t nelts)
{
    size_t                len, dot_len;
    ngx_uint_t            i, n, dot;
    ngx_array_t           curr_names, next_names;
    ngx_hash_کلید_t       *name, *next_name;
    ngx_hash_init_t       h;
    ngx_hash_wildcard_t  *wdc;

    اگر (ngx_array_init(&curr_names, hinit->temp_pool, nelts,
                       اندازه(ngx_hash_کلید_t))
        != NGX_OK)
    {
        بازگشت NGX_ERROR;
    }

    اگر (ngx_array_init(&next_names, hinit->temp_pool, nelts,
                       اندازه(ngx_hash_کلید_t))
        != NGX_OK)
    {
        بازگشت NGX_ERROR;
    }

    برای (n = 0; n < nelts; n = i) {

#اگر 0
        ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                      "wc0: \"%V\"", &names[n].کلید);
#endif

        dot = 0;

        برای (len = 0; len < names[n].کلید.len; len++) {
            اگر (names[n].کلید.data[len] == '.') {
                dot = 1;
                بشکن;
            }
        }

        name = ngx_array_افزودن(&curr_names);
        اگر (name == پوچ) {
            بازگشت NGX_ERROR;
        }

        name->کلید.len = len;
        name->کلید.data = names[n].کلید.data;
        name->کلید_hash = hinit->کلید(name->کلید.data, name->کلید.len);
        name->مقدار = names[n].مقدار;

#اگر 0
        ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                      "wc1: \"%V\" %ui", &name->کلید, dot);
#endif

        dot_len = len + 1;

        اگر (dot) {
            len++;
        }

        next_names.nelts = 0;

        اگر (names[n].کلید.len != len) {
            next_name = ngx_array_افزودن(&next_names);
            اگر (next_name == پوچ) {
                بازگشت NGX_ERROR;
            }

            next_name->کلید.len = names[n].کلید.len - len;
            next_name->کلید.data = names[n].کلید.data + len;
            next_name->کلید_hash = 0;
            next_name->مقدار = names[n].مقدار;

#اگر 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "wc2: \"%V\"", &next_name->کلید);
#endif
        }

        برای (i = n + 1; i < nelts; i++) {
            اگر (ngx_strncmp(names[n].کلید.data, names[i].کلید.data, len) != 0) {
                بشکن;
            }

            اگر (!dot
                && names[i].کلید.len > len
                && names[i].کلید.data[len] != '.')
            {
                بشکن;
            }

            next_name = ngx_array_افزودن(&next_names);
            اگر (next_name == پوچ) {
                بازگشت NGX_ERROR;
            }

            next_name->کلید.len = names[i].کلید.len - dot_len;
            next_name->کلید.data = names[i].کلید.data + dot_len;
            next_name->کلید_hash = 0;
            next_name->مقدار = names[i].مقدار;

#اگر 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "wc3: \"%V\"", &next_name->کلید);
#endif
        }

        اگر (next_names.nelts) {

            h = *hinit;
            h.hash = پوچ;

            اگر (ngx_hash_wildcard_init(&h, (ngx_hash_کلید_t *) next_names.elts,
                                       next_names.nelts)
                != NGX_OK)
            {
                بازگشت NGX_ERROR;
            }

            wdc = (ngx_hash_wildcard_t *) h.hash;

            اگر (names[n].کلید.len == len) {
                wdc->مقدار = names[n].مقدار;
            }

            name->مقدار = (تهی *) ((uintptr_t) wdc | (dot ? 3 : 2));

        } وگرنه اگر (dot) {
            name->مقدار = (تهی *) ((uintptr_t) name->مقدار | 1);
        }
    }

    اگر (ngx_hash_init(hinit, (ngx_hash_کلید_t *) curr_names.elts,
                      curr_names.nelts)
        != NGX_OK)
    {
        بازگشت NGX_ERROR;
    }

    بازگشت NGX_OK;
}


ngx_uint_t
ngx_hash_کلید(u_char *data, size_t len)
{
    ngx_uint_t  i, کلید;

    کلید = 0;

    برای (i = 0; i < len; i++) {
        کلید = ngx_hash(کلید, data[i]);
    }

    بازگشت کلید;
}


ngx_uint_t
ngx_hash_کلید_lc(u_char *data, size_t len)
{
    ngx_uint_t  i, کلید;

    کلید = 0;

    برای (i = 0; i < len; i++) {
        کلید = ngx_hash(کلید, ngx_tolower(data[i]));
    }

    بازگشت کلید;
}


ngx_uint_t
ngx_hash_strlow(u_char *dst, u_char *src, size_t n)
{
    ngx_uint_t  کلید;

    کلید = 0;

    وقتی (n--) {
        *dst = ngx_tolower(*src);
        کلید = ngx_hash(کلید, *dst);
        dst++;
        src++;
    }

    بازگشت کلید;
}


ngx_int_t
ngx_hash_کلیدها_array_init(ngx_hash_کلیدها_arrays_t *ha, ngx_uint_t type)
{
    ngx_uint_t  asize;

    اگر (type == NGX_HASH_SMALL) {
        asize = 4;
        ha->hsize = 107;

    } وگرنه {
        asize = NGX_HASH_LARGE_ASIZE;
        ha->hsize = NGX_HASH_LARGE_HSIZE;
    }

    اگر (ngx_array_init(&ha->کلیدها, ha->temp_pool, asize, اندازه(ngx_hash_کلید_t))
        != NGX_OK)
    {
        بازگشت NGX_ERROR;
    }

    اگر (ngx_array_init(&ha->dns_wc_head, ha->temp_pool, asize,
                       اندازه(ngx_hash_کلید_t))
        != NGX_OK)
    {
        بازگشت NGX_ERROR;
    }

    اگر (ngx_array_init(&ha->dns_wc_tail, ha->temp_pool, asize,
                       اندازه(ngx_hash_کلید_t))
        != NGX_OK)
    {
        بازگشت NGX_ERROR;
    }

    ha->کلیدها_hash = ngx_pcalloc(ha->temp_pool, اندازه(ngx_array_t) * ha->hsize);
    اگر (ha->کلیدها_hash == پوچ) {
        بازگشت NGX_ERROR;
    }

    ha->dns_wc_head_hash = ngx_pcalloc(ha->temp_pool,
                                       اندازه(ngx_array_t) * ha->hsize);
    اگر (ha->dns_wc_head_hash == پوچ) {
        بازگشت NGX_ERROR;
    }

    ha->dns_wc_tail_hash = ngx_pcalloc(ha->temp_pool,
                                       اندازه(ngx_array_t) * ha->hsize);
    اگر (ha->dns_wc_tail_hash == پوچ) {
        بازگشت NGX_ERROR;
    }

    بازگشت NGX_OK;
}


ngx_int_t
ngx_hash_add_کلید(ngx_hash_کلیدها_arrays_t *ha, ngx_str_t *کلید, تهی *مقدار,
    ngx_uint_t flags)
{
    size_t           len;
    u_char          *p;
    ngx_str_t       *name;
    ngx_uint_t       i, k, n, skip, last;
    ngx_array_t     *کلیدها, *hwc;
    ngx_hash_کلید_t  *hk;

    last = کلید->len;

    اگر (flags & NGX_HASH_WILDCARD_کلید) {

        /*
         * supported wildcards:
         *     "*.example.com", ".example.com", and "www.example.*"
         */

        n = 0;

        برای (i = 0; i < کلید->len; i++) {

            اگر (کلید->data[i] == '*') {
                اگر (++n > 1) {
                    بازگشت NGX_DECLINED;
                }
            }

            اگر (کلید->data[i] == '.' && کلید->data[i + 1] == '.') {
                بازگشت NGX_DECLINED;
            }

            اگر (کلید->data[i] == '\0') {
                بازگشت NGX_DECLINED;
            }
        }

        اگر (کلید->len > 1 && کلید->data[0] == '.') {
            skip = 1;
            برو wildcard;
        }

        اگر (کلید->len > 2) {

            اگر (کلید->data[0] == '*' && کلید->data[1] == '.') {
                skip = 2;
                برو wildcard;
            }

            اگر (کلید->data[i - 2] == '.' && کلید->data[i - 1] == '*') {
                skip = 0;
                last -= 2;
                برو wildcard;
            }
        }

        اگر (n) {
            بازگشت NGX_DECLINED;
        }
    }

    /* exact hash */

    k = 0;

    برای (i = 0; i < last; i++) {
        اگر (!(flags & NGX_HASH_READONLY_کلید)) {
            کلید->data[i] = ngx_tolower(کلید->data[i]);
        }
        k = ngx_hash(k, کلید->data[i]);
    }

    k %= ha->hsize;

    /* check conflicts in exact hash */

    name = ha->کلیدها_hash[k].elts;

    اگر (name) {
        برای (i = 0; i < ha->کلیدها_hash[k].nelts; i++) {
            اگر (last != name[i].len) {
                ادامه;
            }

            اگر (ngx_strncmp(کلید->data, name[i].data, last) == 0) {
                بازگشت NGX_BUSY;
            }
        }

    } وگرنه {
        اگر (ngx_array_init(&ha->کلیدها_hash[k], ha->temp_pool, 4,
                           اندازه(ngx_str_t))
            != NGX_OK)
        {
            بازگشت NGX_ERROR;
        }
    }

    name = ngx_array_افزودن(&ha->کلیدها_hash[k]);
    اگر (name == پوچ) {
        بازگشت NGX_ERROR;
    }

    *name = *کلید;

    hk = ngx_array_افزودن(&ha->کلیدها);
    اگر (hk == پوچ) {
        بازگشت NGX_ERROR;
    }

    hk->کلید = *کلید;
    hk->کلید_hash = ngx_hash_کلید(کلید->data, last);
    hk->مقدار = مقدار;

    بازگشت NGX_OK;


wildcard:

    /* wildcard hash */

    k = ngx_hash_strlow(&کلید->data[skip], &کلید->data[skip], last - skip);

    k %= ha->hsize;

    اگر (skip == 1) {

        /* check conflicts in exact hash for ".example.com" */

        name = ha->کلیدها_hash[k].elts;

        اگر (name) {
            len = last - skip;

            برای (i = 0; i < ha->کلیدها_hash[k].nelts; i++) {
                اگر (len != name[i].len) {
                    ادامه;
                }

                اگر (ngx_strncmp(&کلید->data[1], name[i].data, len) == 0) {
                    بازگشت NGX_BUSY;
                }
            }

        } وگرنه {
            اگر (ngx_array_init(&ha->کلیدها_hash[k], ha->temp_pool, 4,
                               اندازه(ngx_str_t))
                != NGX_OK)
            {
                بازگشت NGX_ERROR;
            }
        }

        name = ngx_array_افزودن(&ha->کلیدها_hash[k]);
        اگر (name == پوچ) {
            بازگشت NGX_ERROR;
        }

        name->len = last - 1;
        name->data = ngx_pnalloc(ha->temp_pool, name->len);
        اگر (name->data == پوچ) {
            بازگشت NGX_ERROR;
        }

        ngx_memcpy(name->data, &کلید->data[1], name->len);
    }


    اگر (skip) {

        /*
         * convert "*.example.com" to "com.example.\0"
         *      and ".example.com" to "com.example\0"
         */

        p = ngx_pnalloc(ha->temp_pool, last);
        اگر (p == پوچ) {
            بازگشت NGX_ERROR;
        }

        len = 0;
        n = 0;

        برای (i = last - 1; i; i--) {
            اگر (کلید->data[i] == '.') {
                ngx_memcpy(&p[n], &کلید->data[i + 1], len);
                n += len;
                p[n++] = '.';
                len = 0;
                ادامه;
            }

            len++;
        }

        اگر (len) {
            ngx_memcpy(&p[n], &کلید->data[1], len);
            n += len;
        }

        p[n] = '\0';

        hwc = &ha->dns_wc_head;
        کلیدها = &ha->dns_wc_head_hash[k];

    } وگرنه {

        /* convert "www.example.*" to "www.example\0" */

        last++;

        p = ngx_pnalloc(ha->temp_pool, last);
        اگر (p == پوچ) {
            بازگشت NGX_ERROR;
        }

        ngx_cpystrn(p, کلید->data, last);

        hwc = &ha->dns_wc_tail;
        کلیدها = &ha->dns_wc_tail_hash[k];
    }


    /* check conflicts in wildcard hash */

    name = کلیدها->elts;

    اگر (name) {
        len = last - skip;

        برای (i = 0; i < کلیدها->nelts; i++) {
            اگر (len != name[i].len) {
                ادامه;
            }

            اگر (ngx_strncmp(کلید->data + skip, name[i].data, len) == 0) {
                بازگشت NGX_BUSY;
            }
        }

    } وگرنه {
        اگر (ngx_array_init(کلیدها, ha->temp_pool, 4, اندازه(ngx_str_t)) != NGX_OK)
        {
            بازگشت NGX_ERROR;
        }
    }

    name = ngx_array_افزودن(کلیدها);
    اگر (name == پوچ) {
        بازگشت NGX_ERROR;
    }

    name->len = last - skip;
    name->data = ngx_pnalloc(ha->temp_pool, name->len);
    اگر (name->data == پوچ) {
        بازگشت NGX_ERROR;
    }

    ngx_memcpy(name->data, کلید->data + skip, name->len);


    /* add to wildcard hash */

    hk = ngx_array_افزودن(hwc);
    اگر (hk == پوچ) {
        بازگشت NGX_ERROR;
    }

    hk->کلید.len = last - 1;
    hk->کلید.data = p;
    hk->کلید_hash = 0;
    hk->مقدار = مقدار;

    بازگشت NGX_OK;
}
