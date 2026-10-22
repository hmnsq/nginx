
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


فراغ *
ngx_hash_find(ngx_hash_t *hash, ngx_uint_t کلیل, u_char *name, size_t len)
{
    ngx_uint_t       i;
    ngx_hash_elt_t  *elt;

#إذا 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "hf:\"%*s\"", len, name);
#endif

    elt = hash->buckets[کلیل % hash->size];

    إذا (elt == عدم) {
        أرجع عدم;
    }

    طالما (elt->بەها) {
        إذا (len != (size_t) elt->len) {
            اذهب next;
        }

        لكل (i = 0; i < len; i++) {
            إذا (name[i] != elt->name[i]) {
                اذهب next;
            }
        }

        أرجع elt->بەها;

    next:

        elt = (ngx_hash_elt_t *) ngx_align_ptr(&elt->name[0] + elt->len,
                                               حجم(فراغ *));
        استمر;
    }

    أرجع عدم;
}


فراغ *
ngx_hash_find_wc_head(ngx_hash_wildcard_t *hwc, u_char *name, size_t len)
{
    فراغ        *بەها;
    ngx_uint_t   i, n, کلیل;

#إذا 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "wch:\"%*s\"", len, name);
#endif

    n = len;

    طالما (n) {
        إذا (name[n - 1] == '.') {
            اكسر;
        }

        n--;
    }

    کلیل = 0;

    لكل (i = n; i < len; i++) {
        کلیل = ngx_hash(کلیل, name[i]);
    }

#إذا 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "key:\"%ui\"", کلیل);
#endif

    بەها = ngx_hash_find(&hwc->hash, کلیل, &name[n], len - n);

#إذا 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "value:\"%p\"", بەها);
#endif

    إذا (بەها) {

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

        إذا ((uintptr_t) بەها & 2) {

            إذا (n == 0) {

                /* "example.com" */

                إذا ((uintptr_t) بەها & 1) {
                    أرجع عدم;
                }

                hwc = (ngx_hash_wildcard_t *)
                                          ((uintptr_t) بەها & (uintptr_t) ~3);
                أرجع hwc->بەها;
            }

            hwc = (ngx_hash_wildcard_t *) ((uintptr_t) بەها & (uintptr_t) ~3);

            بەها = ngx_hash_find_wc_head(hwc, name, n - 1);

            إذا (بەها) {
                أرجع بەها;
            }

            أرجع hwc->بەها;
        }

        إذا ((uintptr_t) بەها & 1) {

            إذا (n == 0) {

                /* "example.com" */

                أرجع عدم;
            }

            أرجع (فراغ *) ((uintptr_t) بەها & (uintptr_t) ~3);
        }

        أرجع بەها;
    }

    أرجع hwc->بەها;
}


فراغ *
ngx_hash_find_wc_tail(ngx_hash_wildcard_t *hwc, u_char *name, size_t len)
{
    فراغ        *بەها;
    ngx_uint_t   i, کلیل;

#إذا 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "wct:\"%*s\"", len, name);
#endif

    کلیل = 0;

    لكل (i = 0; i < len; i++) {
        إذا (name[i] == '.') {
            اكسر;
        }

        کلیل = ngx_hash(کلیل, name[i]);
    }

    إذا (i == len) {
        أرجع عدم;
    }

#إذا 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "key:\"%ui\"", کلیل);
#endif

    بەها = ngx_hash_find(&hwc->hash, کلیل, name, i);

#إذا 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "value:\"%p\"", بەها);
#endif

    إذا (بەها) {

        /*
         * the 2 low bits of value have the special meaning:
         *     00 - value is data pointer;
         *     11 - value is pointer to wildcard hash allowing "example.*".
         */

        إذا ((uintptr_t) بەها & 2) {

            i++;

            hwc = (ngx_hash_wildcard_t *) ((uintptr_t) بەها & (uintptr_t) ~3);

            بەها = ngx_hash_find_wc_tail(hwc, &name[i], len - i);

            إذا (بەها) {
                أرجع بەها;
            }

            أرجع hwc->بەها;
        }

        أرجع بەها;
    }

    أرجع hwc->بەها;
}


فراغ *
ngx_hash_find_combined(ngx_hash_combined_t *hash, ngx_uint_t کلیل, u_char *name,
    size_t len)
{
    فراغ  *بەها;

    إذا (hash->hash.buckets) {
        بەها = ngx_hash_find(&hash->hash, کلیل, name, len);

        إذا (بەها) {
            أرجع بەها;
        }
    }

    إذا (len == 0) {
        أرجع عدم;
    }

    إذا (hash->wc_head && hash->wc_head->hash.buckets) {
        بەها = ngx_hash_find_wc_head(hash->wc_head, name, len);

        إذا (بەها) {
            أرجع بەها;
        }
    }

    إذا (hash->wc_tail && hash->wc_tail->hash.buckets) {
        بەها = ngx_hash_find_wc_tail(hash->wc_tail, name, len);

        إذا (بەها) {
            أرجع بەها;
        }
    }

    أرجع عدم;
}


#define NGX_HASH_ELT_SIZE(name)                                               \
    (حجم(فراغ *) + ngx_align((name)->کلیل.len + 2, حجم(فراغ *)))

ngx_int_t
ngx_hash_init(ngx_hash_init_t *hinit, ngx_hash_کلیل_t *names, ngx_uint_t nelts)
{
    u_char          *elts;
    size_t           len;
    u_short         *test;
    ngx_uint_t       i, n, کلیل, size, start, bucket_size;
    ngx_hash_elt_t  *elt, **buckets;

    إذا (hinit->max_size == 0) {
        ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                      "could not build %s, you should "
                      "increase %s_max_size: %i",
                      hinit->name, hinit->name, hinit->max_size);
        أرجع NGX_ERROR;
    }

    إذا (hinit->bucket_size > 65536 - ngx_cacheline_size) {
        ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                      "could not build %s, too large "
                      "%s_bucket_size: %i",
                      hinit->name, hinit->name, hinit->bucket_size);
        أرجع NGX_ERROR;
    }

    لكل (n = 0; n < nelts; n++) {
        إذا (names[n].کلیل.data == عدم) {
            استمر;
        }

        إذا (hinit->bucket_size < NGX_HASH_ELT_SIZE(&names[n]) + حجم(فراغ *))
        {
            ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                          "could not build %s, you should "
                          "increase %s_bucket_size: %i",
                          hinit->name, hinit->name, hinit->bucket_size);
            أرجع NGX_ERROR;
        }
    }

    test = ngx_alloc(hinit->max_size * حجم(u_short), hinit->pool->log);
    إذا (test == عدم) {
        أرجع NGX_ERROR;
    }

    bucket_size = hinit->bucket_size - حجم(فراغ *);

    start = nelts / (bucket_size / (2 * حجم(فراغ *)));
    start = start ? start : 1;

    إذا (hinit->max_size > 10000 && nelts && hinit->max_size / nelts < 100) {
        start = hinit->max_size - 1000;
    }

    لكل (size = start; size <= hinit->max_size; size++) {

        ngx_memzero(test, size * حجم(u_short));

        لكل (n = 0; n < nelts; n++) {
            إذا (names[n].کلیل.data == عدم) {
                استمر;
            }

            کلیل = names[n].کلیل_hash % size;
            len = test[کلیل] + NGX_HASH_ELT_SIZE(&names[n]);

#إذا 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "%ui: %ui %uz \"%V\"",
                          size, کلیل, len, &names[n].کلیل);
#endif

            إذا (len > bucket_size) {
                اذهب next;
            }

            test[کلیل] = (u_short) len;
        }

        اذهب found;

    next:

        استمر;
    }

    size = hinit->max_size;

    ngx_log_error(NGX_LOG_WARN, hinit->pool->log, 0,
                  "could not build optimal %s, you should increase "
                  "either %s_max_size: %i or %s_bucket_size: %i; "
                  "ignoring %s_bucket_size",
                  hinit->name, hinit->name, hinit->max_size,
                  hinit->name, hinit->bucket_size, hinit->name);

found:

    لكل (i = 0; i < size; i++) {
        test[i] = حجم(فراغ *);
    }

    لكل (n = 0; n < nelts; n++) {
        إذا (names[n].کلیل.data == عدم) {
            استمر;
        }

        کلیل = names[n].کلیل_hash % size;
        len = test[کلیل] + NGX_HASH_ELT_SIZE(&names[n]);

        إذا (len > 65536 - ngx_cacheline_size) {
            ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                          "could not build %s, you should "
                          "increase %s_max_size: %i",
                          hinit->name, hinit->name, hinit->max_size);
            ngx_free(test);
            أرجع NGX_ERROR;
        }

        test[کلیل] = (u_short) len;
    }

    len = 0;

    لكل (i = 0; i < size; i++) {
        إذا (test[i] == حجم(فراغ *)) {
            استمر;
        }

        test[i] = (u_short) (ngx_align(test[i], ngx_cacheline_size));

        len += test[i];
    }

    إذا (hinit->hash == عدم) {
        hinit->hash = ngx_pcalloc(hinit->pool, حجم(ngx_hash_wildcard_t)
                                             + size * حجم(ngx_hash_elt_t *));
        إذا (hinit->hash == عدم) {
            ngx_free(test);
            أرجع NGX_ERROR;
        }

        buckets = (ngx_hash_elt_t **)
                      ((u_char *) hinit->hash + حجم(ngx_hash_wildcard_t));

    } وإلا {
        buckets = ngx_pcalloc(hinit->pool, size * حجم(ngx_hash_elt_t *));
        إذا (buckets == عدم) {
            ngx_free(test);
            أرجع NGX_ERROR;
        }
    }

    elts = ngx_palloc(hinit->pool, len + ngx_cacheline_size);
    إذا (elts == عدم) {
        ngx_free(test);
        أرجع NGX_ERROR;
    }

    elts = ngx_align_ptr(elts, ngx_cacheline_size);

    لكل (i = 0; i < size; i++) {
        إذا (test[i] == حجم(فراغ *)) {
            استمر;
        }

        buckets[i] = (ngx_hash_elt_t *) elts;
        elts += test[i];
    }

    لكل (i = 0; i < size; i++) {
        test[i] = 0;
    }

    لكل (n = 0; n < nelts; n++) {
        إذا (names[n].کلیل.data == عدم) {
            استمر;
        }

        کلیل = names[n].کلیل_hash % size;
        elt = (ngx_hash_elt_t *) ((u_char *) buckets[کلیل] + test[کلیل]);

        elt->بەها = names[n].بەها;
        elt->len = (u_short) names[n].کلیل.len;

        ngx_strlow(elt->name, names[n].کلیل.data, names[n].کلیل.len);

        test[کلیل] = (u_short) (test[کلیل] + NGX_HASH_ELT_SIZE(&names[n]));
    }

    لكل (i = 0; i < size; i++) {
        إذا (buckets[i] == عدم) {
            استمر;
        }

        elt = (ngx_hash_elt_t *) ((u_char *) buckets[i] + test[i]);

        elt->بەها = عدم;
    }

    ngx_free(test);

    hinit->hash->buckets = buckets;
    hinit->hash->size = size;

#إذا 0

    لكل (i = 0; i < size; i++) {
        ngx_str_t   val;
        ngx_uint_t  کلیل;

        elt = buckets[i];

        إذا (elt == عدم) {
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "%ui: NULL", i);
            استمر;
        }

        طالما (elt->بەها) {
            val.len = elt->len;
            val.data = &elt->name[0];

            کلیل = hinit->کلیل(val.data, val.len);

            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "%ui: %p \"%V\" %ui", i, elt, &val, کلیل);

            elt = (ngx_hash_elt_t *) ngx_align_ptr(&elt->name[0] + elt->len,
                                                   حجم(فراغ *));
        }
    }

#endif

    أرجع NGX_OK;
}


ngx_int_t
ngx_hash_wildcard_init(ngx_hash_init_t *hinit, ngx_hash_کلیل_t *names,
    ngx_uint_t nelts)
{
    size_t                len, dot_len;
    ngx_uint_t            i, n, dot;
    ngx_array_t           curr_names, next_names;
    ngx_hash_کلیل_t       *name, *next_name;
    ngx_hash_init_t       h;
    ngx_hash_wildcard_t  *wdc;

    إذا (ngx_array_init(&curr_names, hinit->temp_pool, nelts,
                       حجم(ngx_hash_کلیل_t))
        != NGX_OK)
    {
        أرجع NGX_ERROR;
    }

    إذا (ngx_array_init(&next_names, hinit->temp_pool, nelts,
                       حجم(ngx_hash_کلیل_t))
        != NGX_OK)
    {
        أرجع NGX_ERROR;
    }

    لكل (n = 0; n < nelts; n = i) {

#إذا 0
        ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                      "wc0: \"%V\"", &names[n].کلیل);
#endif

        dot = 0;

        لكل (len = 0; len < names[n].کلیل.len; len++) {
            إذا (names[n].کلیل.data[len] == '.') {
                dot = 1;
                اكسر;
            }
        }

        name = ngx_array_پاڵنان(&curr_names);
        إذا (name == عدم) {
            أرجع NGX_ERROR;
        }

        name->کلیل.len = len;
        name->کلیل.data = names[n].کلیل.data;
        name->کلیل_hash = hinit->کلیل(name->کلیل.data, name->کلیل.len);
        name->بەها = names[n].بەها;

#إذا 0
        ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                      "wc1: \"%V\" %ui", &name->کلیل, dot);
#endif

        dot_len = len + 1;

        إذا (dot) {
            len++;
        }

        next_names.nelts = 0;

        إذا (names[n].کلیل.len != len) {
            next_name = ngx_array_پاڵنان(&next_names);
            إذا (next_name == عدم) {
                أرجع NGX_ERROR;
            }

            next_name->کلیل.len = names[n].کلیل.len - len;
            next_name->کلیل.data = names[n].کلیل.data + len;
            next_name->کلیل_hash = 0;
            next_name->بەها = names[n].بەها;

#إذا 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "wc2: \"%V\"", &next_name->کلیل);
#endif
        }

        لكل (i = n + 1; i < nelts; i++) {
            إذا (ngx_strncmp(names[n].کلیل.data, names[i].کلیل.data, len) != 0) {
                اكسر;
            }

            إذا (!dot
                && names[i].کلیل.len > len
                && names[i].کلیل.data[len] != '.')
            {
                اكسر;
            }

            next_name = ngx_array_پاڵنان(&next_names);
            إذا (next_name == عدم) {
                أرجع NGX_ERROR;
            }

            next_name->کلیل.len = names[i].کلیل.len - dot_len;
            next_name->کلیل.data = names[i].کلیل.data + dot_len;
            next_name->کلیل_hash = 0;
            next_name->بەها = names[i].بەها;

#إذا 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "wc3: \"%V\"", &next_name->کلیل);
#endif
        }

        إذا (next_names.nelts) {

            h = *hinit;
            h.hash = عدم;

            إذا (ngx_hash_wildcard_init(&h, (ngx_hash_کلیل_t *) next_names.elts,
                                       next_names.nelts)
                != NGX_OK)
            {
                أرجع NGX_ERROR;
            }

            wdc = (ngx_hash_wildcard_t *) h.hash;

            إذا (names[n].کلیل.len == len) {
                wdc->بەها = names[n].بەها;
            }

            name->بەها = (فراغ *) ((uintptr_t) wdc | (dot ? 3 : 2));

        } وإلا إذا (dot) {
            name->بەها = (فراغ *) ((uintptr_t) name->بەها | 1);
        }
    }

    إذا (ngx_hash_init(hinit, (ngx_hash_کلیل_t *) curr_names.elts,
                      curr_names.nelts)
        != NGX_OK)
    {
        أرجع NGX_ERROR;
    }

    أرجع NGX_OK;
}


ngx_uint_t
ngx_hash_کلیل(u_char *data, size_t len)
{
    ngx_uint_t  i, کلیل;

    کلیل = 0;

    لكل (i = 0; i < len; i++) {
        کلیل = ngx_hash(کلیل, data[i]);
    }

    أرجع کلیل;
}


ngx_uint_t
ngx_hash_کلیل_lc(u_char *data, size_t len)
{
    ngx_uint_t  i, کلیل;

    کلیل = 0;

    لكل (i = 0; i < len; i++) {
        کلیل = ngx_hash(کلیل, ngx_tolower(data[i]));
    }

    أرجع کلیل;
}


ngx_uint_t
ngx_hash_strlow(u_char *dst, u_char *src, size_t n)
{
    ngx_uint_t  کلیل;

    کلیل = 0;

    طالما (n--) {
        *dst = ngx_tolower(*src);
        کلیل = ngx_hash(کلیل, *dst);
        dst++;
        src++;
    }

    أرجع کلیل;
}


ngx_int_t
ngx_hash_کلیلەکان_array_init(ngx_hash_کلیلەکان_arrays_t *ha, ngx_uint_t type)
{
    ngx_uint_t  asize;

    إذا (type == NGX_HASH_SMALL) {
        asize = 4;
        ha->hsize = 107;

    } وإلا {
        asize = NGX_HASH_LARGE_ASIZE;
        ha->hsize = NGX_HASH_LARGE_HSIZE;
    }

    إذا (ngx_array_init(&ha->کلیلەکان, ha->temp_pool, asize, حجم(ngx_hash_کلیل_t))
        != NGX_OK)
    {
        أرجع NGX_ERROR;
    }

    إذا (ngx_array_init(&ha->dns_wc_head, ha->temp_pool, asize,
                       حجم(ngx_hash_کلیل_t))
        != NGX_OK)
    {
        أرجع NGX_ERROR;
    }

    إذا (ngx_array_init(&ha->dns_wc_tail, ha->temp_pool, asize,
                       حجم(ngx_hash_کلیل_t))
        != NGX_OK)
    {
        أرجع NGX_ERROR;
    }

    ha->کلیلەکان_hash = ngx_pcalloc(ha->temp_pool, حجم(ngx_array_t) * ha->hsize);
    إذا (ha->کلیلەکان_hash == عدم) {
        أرجع NGX_ERROR;
    }

    ha->dns_wc_head_hash = ngx_pcalloc(ha->temp_pool,
                                       حجم(ngx_array_t) * ha->hsize);
    إذا (ha->dns_wc_head_hash == عدم) {
        أرجع NGX_ERROR;
    }

    ha->dns_wc_tail_hash = ngx_pcalloc(ha->temp_pool,
                                       حجم(ngx_array_t) * ha->hsize);
    إذا (ha->dns_wc_tail_hash == عدم) {
        أرجع NGX_ERROR;
    }

    أرجع NGX_OK;
}


ngx_int_t
ngx_hash_add_کلیل(ngx_hash_کلیلەکان_arrays_t *ha, ngx_str_t *کلیل, فراغ *بەها,
    ngx_uint_t flags)
{
    size_t           len;
    u_char          *p;
    ngx_str_t       *name;
    ngx_uint_t       i, k, n, skip, last;
    ngx_array_t     *کلیلەکان, *hwc;
    ngx_hash_کلیل_t  *hk;

    last = کلیل->len;

    إذا (flags & NGX_HASH_WILDCARD_کلیل) {

        /*
         * supported wildcards:
         *     "*.example.com", ".example.com", and "www.example.*"
         */

        n = 0;

        لكل (i = 0; i < کلیل->len; i++) {

            إذا (کلیل->data[i] == '*') {
                إذا (++n > 1) {
                    أرجع NGX_DECLINED;
                }
            }

            إذا (کلیل->data[i] == '.' && کلیل->data[i + 1] == '.') {
                أرجع NGX_DECLINED;
            }

            إذا (کلیل->data[i] == '\0') {
                أرجع NGX_DECLINED;
            }
        }

        إذا (کلیل->len > 1 && کلیل->data[0] == '.') {
            skip = 1;
            اذهب wildcard;
        }

        إذا (کلیل->len > 2) {

            إذا (کلیل->data[0] == '*' && کلیل->data[1] == '.') {
                skip = 2;
                اذهب wildcard;
            }

            إذا (کلیل->data[i - 2] == '.' && کلیل->data[i - 1] == '*') {
                skip = 0;
                last -= 2;
                اذهب wildcard;
            }
        }

        إذا (n) {
            أرجع NGX_DECLINED;
        }
    }

    /* exact hash */

    k = 0;

    لكل (i = 0; i < last; i++) {
        إذا (!(flags & NGX_HASH_READONLY_کلیل)) {
            کلیل->data[i] = ngx_tolower(کلیل->data[i]);
        }
        k = ngx_hash(k, کلیل->data[i]);
    }

    k %= ha->hsize;

    /* check conflicts in exact hash */

    name = ha->کلیلەکان_hash[k].elts;

    إذا (name) {
        لكل (i = 0; i < ha->کلیلەکان_hash[k].nelts; i++) {
            إذا (last != name[i].len) {
                استمر;
            }

            إذا (ngx_strncmp(کلیل->data, name[i].data, last) == 0) {
                أرجع NGX_BUSY;
            }
        }

    } وإلا {
        إذا (ngx_array_init(&ha->کلیلەکان_hash[k], ha->temp_pool, 4,
                           حجم(ngx_str_t))
            != NGX_OK)
        {
            أرجع NGX_ERROR;
        }
    }

    name = ngx_array_پاڵنان(&ha->کلیلەکان_hash[k]);
    إذا (name == عدم) {
        أرجع NGX_ERROR;
    }

    *name = *کلیل;

    hk = ngx_array_پاڵنان(&ha->کلیلەکان);
    إذا (hk == عدم) {
        أرجع NGX_ERROR;
    }

    hk->کلیل = *کلیل;
    hk->کلیل_hash = ngx_hash_کلیل(کلیل->data, last);
    hk->بەها = بەها;

    أرجع NGX_OK;


wildcard:

    /* wildcard hash */

    k = ngx_hash_strlow(&کلیل->data[skip], &کلیل->data[skip], last - skip);

    k %= ha->hsize;

    إذا (skip == 1) {

        /* check conflicts in exact hash for ".example.com" */

        name = ha->کلیلەکان_hash[k].elts;

        إذا (name) {
            len = last - skip;

            لكل (i = 0; i < ha->کلیلەکان_hash[k].nelts; i++) {
                إذا (len != name[i].len) {
                    استمر;
                }

                إذا (ngx_strncmp(&کلیل->data[1], name[i].data, len) == 0) {
                    أرجع NGX_BUSY;
                }
            }

        } وإلا {
            إذا (ngx_array_init(&ha->کلیلەکان_hash[k], ha->temp_pool, 4,
                               حجم(ngx_str_t))
                != NGX_OK)
            {
                أرجع NGX_ERROR;
            }
        }

        name = ngx_array_پاڵنان(&ha->کلیلەکان_hash[k]);
        إذا (name == عدم) {
            أرجع NGX_ERROR;
        }

        name->len = last - 1;
        name->data = ngx_pnalloc(ha->temp_pool, name->len);
        إذا (name->data == عدم) {
            أرجع NGX_ERROR;
        }

        ngx_memcpy(name->data, &کلیل->data[1], name->len);
    }


    إذا (skip) {

        /*
         * convert "*.example.com" to "com.example.\0"
         *      and ".example.com" to "com.example\0"
         */

        p = ngx_pnalloc(ha->temp_pool, last);
        إذا (p == عدم) {
            أرجع NGX_ERROR;
        }

        len = 0;
        n = 0;

        لكل (i = last - 1; i; i--) {
            إذا (کلیل->data[i] == '.') {
                ngx_memcpy(&p[n], &کلیل->data[i + 1], len);
                n += len;
                p[n++] = '.';
                len = 0;
                استمر;
            }

            len++;
        }

        إذا (len) {
            ngx_memcpy(&p[n], &کلیل->data[1], len);
            n += len;
        }

        p[n] = '\0';

        hwc = &ha->dns_wc_head;
        کلیلەکان = &ha->dns_wc_head_hash[k];

    } وإلا {

        /* convert "www.example.*" to "www.example\0" */

        last++;

        p = ngx_pnalloc(ha->temp_pool, last);
        إذا (p == عدم) {
            أرجع NGX_ERROR;
        }

        ngx_cpystrn(p, کلیل->data, last);

        hwc = &ha->dns_wc_tail;
        کلیلەکان = &ha->dns_wc_tail_hash[k];
    }


    /* check conflicts in wildcard hash */

    name = کلیلەکان->elts;

    إذا (name) {
        len = last - skip;

        لكل (i = 0; i < کلیلەکان->nelts; i++) {
            إذا (len != name[i].len) {
                استمر;
            }

            إذا (ngx_strncmp(کلیل->data + skip, name[i].data, len) == 0) {
                أرجع NGX_BUSY;
            }
        }

    } وإلا {
        إذا (ngx_array_init(کلیلەکان, ha->temp_pool, 4, حجم(ngx_str_t)) != NGX_OK)
        {
            أرجع NGX_ERROR;
        }
    }

    name = ngx_array_پاڵنان(کلیلەکان);
    إذا (name == عدم) {
        أرجع NGX_ERROR;
    }

    name->len = last - skip;
    name->data = ngx_pnalloc(ha->temp_pool, name->len);
    إذا (name->data == عدم) {
        أرجع NGX_ERROR;
    }

    ngx_memcpy(name->data, کلیل->data + skip, name->len);


    /* add to wildcard hash */

    hk = ngx_array_پاڵنان(hwc);
    إذا (hk == عدم) {
        أرجع NGX_ERROR;
    }

    hk->کلیل.len = last - 1;
    hk->کلیل.data = p;
    hk->کلیل_hash = 0;
    hk->بەها = بەها;

    أرجع NGX_OK;
}
