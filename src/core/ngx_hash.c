
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


فراغ *
ngx_hash_find(ngx_hash_t *hash, ngx_uint_t مفتاح, u_char *name, size_t len)
{
    ngx_uint_t       i;
    ngx_hash_elt_t  *elt;

#إذا 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "hf:\"%*s\"", len, name);
#endif

    elt = hash->buckets[مفتاح % hash->size];

    إذا (elt == عدم) {
        إرجاع عدم;
    }

    بينما (elt->قيمة) {
        إذا (len != (size_t) elt->len) {
            اذهب next;
        }

        لكل (i = 0; i < len; i++) {
            إذا (name[i] != elt->name[i]) {
                اذهب next;
            }
        }

        إرجاع elt->قيمة;

    next:

        elt = (ngx_hash_elt_t *) ngx_align_ptr(&elt->name[0] + elt->len,
                                               حجم(فراغ *));
        استمر;
    }

    إرجاع عدم;
}


فراغ *
ngx_hash_find_wc_head(ngx_hash_wildcard_t *hwc, u_char *name, size_t len)
{
    فراغ        *قيمة;
    ngx_uint_t   i, n, مفتاح;

#إذا 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "wch:\"%*s\"", len, name);
#endif

    n = len;

    بينما (n) {
        إذا (name[n - 1] == '.') {
            اكسر;
        }

        n--;
    }

    مفتاح = 0;

    لكل (i = n; i < len; i++) {
        مفتاح = ngx_hash(مفتاح, name[i]);
    }

#إذا 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "key:\"%ui\"", مفتاح);
#endif

    قيمة = ngx_hash_find(&hwc->hash, مفتاح, &name[n], len - n);

#إذا 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "value:\"%p\"", قيمة);
#endif

    إذا (قيمة) {

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

        إذا ((uintptr_t) قيمة & 2) {

            إذا (n == 0) {

                /* "example.com" */

                إذا ((uintptr_t) قيمة & 1) {
                    إرجاع عدم;
                }

                hwc = (ngx_hash_wildcard_t *)
                                          ((uintptr_t) قيمة & (uintptr_t) ~3);
                إرجاع hwc->قيمة;
            }

            hwc = (ngx_hash_wildcard_t *) ((uintptr_t) قيمة & (uintptr_t) ~3);

            قيمة = ngx_hash_find_wc_head(hwc, name, n - 1);

            إذا (قيمة) {
                إرجاع قيمة;
            }

            إرجاع hwc->قيمة;
        }

        إذا ((uintptr_t) قيمة & 1) {

            إذا (n == 0) {

                /* "example.com" */

                إرجاع عدم;
            }

            إرجاع (فراغ *) ((uintptr_t) قيمة & (uintptr_t) ~3);
        }

        إرجاع قيمة;
    }

    إرجاع hwc->قيمة;
}


فراغ *
ngx_hash_find_wc_tail(ngx_hash_wildcard_t *hwc, u_char *name, size_t len)
{
    فراغ        *قيمة;
    ngx_uint_t   i, مفتاح;

#إذا 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "wct:\"%*s\"", len, name);
#endif

    مفتاح = 0;

    لكل (i = 0; i < len; i++) {
        إذا (name[i] == '.') {
            اكسر;
        }

        مفتاح = ngx_hash(مفتاح, name[i]);
    }

    إذا (i == len) {
        إرجاع عدم;
    }

#إذا 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "key:\"%ui\"", مفتاح);
#endif

    قيمة = ngx_hash_find(&hwc->hash, مفتاح, name, i);

#إذا 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "value:\"%p\"", قيمة);
#endif

    إذا (قيمة) {

        /*
         * the 2 low bits of value have the special meaning:
         *     00 - value is data pointer;
         *     11 - value is pointer to wildcard hash allowing "example.*".
         */

        إذا ((uintptr_t) قيمة & 2) {

            i++;

            hwc = (ngx_hash_wildcard_t *) ((uintptr_t) قيمة & (uintptr_t) ~3);

            قيمة = ngx_hash_find_wc_tail(hwc, &name[i], len - i);

            إذا (قيمة) {
                إرجاع قيمة;
            }

            إرجاع hwc->قيمة;
        }

        إرجاع قيمة;
    }

    إرجاع hwc->قيمة;
}


فراغ *
ngx_hash_find_combined(ngx_hash_combined_t *hash, ngx_uint_t مفتاح, u_char *name,
    size_t len)
{
    فراغ  *قيمة;

    إذا (hash->hash.buckets) {
        قيمة = ngx_hash_find(&hash->hash, مفتاح, name, len);

        إذا (قيمة) {
            إرجاع قيمة;
        }
    }

    إذا (len == 0) {
        إرجاع عدم;
    }

    إذا (hash->wc_head && hash->wc_head->hash.buckets) {
        قيمة = ngx_hash_find_wc_head(hash->wc_head, name, len);

        إذا (قيمة) {
            إرجاع قيمة;
        }
    }

    إذا (hash->wc_tail && hash->wc_tail->hash.buckets) {
        قيمة = ngx_hash_find_wc_tail(hash->wc_tail, name, len);

        إذا (قيمة) {
            إرجاع قيمة;
        }
    }

    إرجاع عدم;
}


#define NGX_HASH_ELT_SIZE(name)                                               \
    (حجم(فراغ *) + ngx_align((name)->مفتاح.len + 2, حجم(فراغ *)))

ngx_int_t
ngx_hash_init(ngx_hash_init_t *hinit, ngx_hash_مفتاح_t *names, ngx_uint_t nelts)
{
    u_char          *elts;
    size_t           len;
    u_short         *test;
    ngx_uint_t       i, n, مفتاح, size, start, bucket_size;
    ngx_hash_elt_t  *elt, **buckets;

    إذا (hinit->max_size == 0) {
        ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                      "could not build %s, you should "
                      "increase %s_max_size: %i",
                      hinit->name, hinit->name, hinit->max_size);
        إرجاع NGX_ERROR;
    }

    إذا (hinit->bucket_size > 65536 - ngx_cacheline_size) {
        ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                      "could not build %s, too large "
                      "%s_bucket_size: %i",
                      hinit->name, hinit->name, hinit->bucket_size);
        إرجاع NGX_ERROR;
    }

    لكل (n = 0; n < nelts; n++) {
        إذا (names[n].مفتاح.data == عدم) {
            استمر;
        }

        إذا (hinit->bucket_size < NGX_HASH_ELT_SIZE(&names[n]) + حجم(فراغ *))
        {
            ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                          "could not build %s, you should "
                          "increase %s_bucket_size: %i",
                          hinit->name, hinit->name, hinit->bucket_size);
            إرجاع NGX_ERROR;
        }
    }

    test = ngx_alloc(hinit->max_size * حجم(u_short), hinit->pool->log);
    إذا (test == عدم) {
        إرجاع NGX_ERROR;
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
            إذا (names[n].مفتاح.data == عدم) {
                استمر;
            }

            مفتاح = names[n].مفتاح_hash % size;
            len = test[مفتاح] + NGX_HASH_ELT_SIZE(&names[n]);

#إذا 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "%ui: %ui %uz \"%V\"",
                          size, مفتاح, len, &names[n].مفتاح);
#endif

            إذا (len > bucket_size) {
                اذهب next;
            }

            test[مفتاح] = (u_short) len;
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
        إذا (names[n].مفتاح.data == عدم) {
            استمر;
        }

        مفتاح = names[n].مفتاح_hash % size;
        len = test[مفتاح] + NGX_HASH_ELT_SIZE(&names[n]);

        إذا (len > 65536 - ngx_cacheline_size) {
            ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                          "could not build %s, you should "
                          "increase %s_max_size: %i",
                          hinit->name, hinit->name, hinit->max_size);
            ngx_free(test);
            إرجاع NGX_ERROR;
        }

        test[مفتاح] = (u_short) len;
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
            إرجاع NGX_ERROR;
        }

        buckets = (ngx_hash_elt_t **)
                      ((u_char *) hinit->hash + حجم(ngx_hash_wildcard_t));

    } وإلا {
        buckets = ngx_pcalloc(hinit->pool, size * حجم(ngx_hash_elt_t *));
        إذا (buckets == عدم) {
            ngx_free(test);
            إرجاع NGX_ERROR;
        }
    }

    elts = ngx_palloc(hinit->pool, len + ngx_cacheline_size);
    إذا (elts == عدم) {
        ngx_free(test);
        إرجاع NGX_ERROR;
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
        إذا (names[n].مفتاح.data == عدم) {
            استمر;
        }

        مفتاح = names[n].مفتاح_hash % size;
        elt = (ngx_hash_elt_t *) ((u_char *) buckets[مفتاح] + test[مفتاح]);

        elt->قيمة = names[n].قيمة;
        elt->len = (u_short) names[n].مفتاح.len;

        ngx_strlow(elt->name, names[n].مفتاح.data, names[n].مفتاح.len);

        test[مفتاح] = (u_short) (test[مفتاح] + NGX_HASH_ELT_SIZE(&names[n]));
    }

    لكل (i = 0; i < size; i++) {
        إذا (buckets[i] == عدم) {
            استمر;
        }

        elt = (ngx_hash_elt_t *) ((u_char *) buckets[i] + test[i]);

        elt->قيمة = عدم;
    }

    ngx_free(test);

    hinit->hash->buckets = buckets;
    hinit->hash->size = size;

#إذا 0

    لكل (i = 0; i < size; i++) {
        ngx_str_t   val;
        ngx_uint_t  مفتاح;

        elt = buckets[i];

        إذا (elt == عدم) {
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "%ui: NULL", i);
            استمر;
        }

        بينما (elt->قيمة) {
            val.len = elt->len;
            val.data = &elt->name[0];

            مفتاح = hinit->مفتاح(val.data, val.len);

            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "%ui: %p \"%V\" %ui", i, elt, &val, مفتاح);

            elt = (ngx_hash_elt_t *) ngx_align_ptr(&elt->name[0] + elt->len,
                                                   حجم(فراغ *));
        }
    }

#endif

    إرجاع NGX_OK;
}


ngx_int_t
ngx_hash_wildcard_init(ngx_hash_init_t *hinit, ngx_hash_مفتاح_t *names,
    ngx_uint_t nelts)
{
    size_t                len, dot_len;
    ngx_uint_t            i, n, dot;
    ngx_array_t           curr_names, next_names;
    ngx_hash_مفتاح_t       *name, *next_name;
    ngx_hash_init_t       h;
    ngx_hash_wildcard_t  *wdc;

    إذا (ngx_array_init(&curr_names, hinit->temp_pool, nelts,
                       حجم(ngx_hash_مفتاح_t))
        != NGX_OK)
    {
        إرجاع NGX_ERROR;
    }

    إذا (ngx_array_init(&next_names, hinit->temp_pool, nelts,
                       حجم(ngx_hash_مفتاح_t))
        != NGX_OK)
    {
        إرجاع NGX_ERROR;
    }

    لكل (n = 0; n < nelts; n = i) {

#إذا 0
        ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                      "wc0: \"%V\"", &names[n].مفتاح);
#endif

        dot = 0;

        لكل (len = 0; len < names[n].مفتاح.len; len++) {
            إذا (names[n].مفتاح.data[len] == '.') {
                dot = 1;
                اكسر;
            }
        }

        name = ngx_array_ادفع(&curr_names);
        إذا (name == عدم) {
            إرجاع NGX_ERROR;
        }

        name->مفتاح.len = len;
        name->مفتاح.data = names[n].مفتاح.data;
        name->مفتاح_hash = hinit->مفتاح(name->مفتاح.data, name->مفتاح.len);
        name->قيمة = names[n].قيمة;

#إذا 0
        ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                      "wc1: \"%V\" %ui", &name->مفتاح, dot);
#endif

        dot_len = len + 1;

        إذا (dot) {
            len++;
        }

        next_names.nelts = 0;

        إذا (names[n].مفتاح.len != len) {
            next_name = ngx_array_ادفع(&next_names);
            إذا (next_name == عدم) {
                إرجاع NGX_ERROR;
            }

            next_name->مفتاح.len = names[n].مفتاح.len - len;
            next_name->مفتاح.data = names[n].مفتاح.data + len;
            next_name->مفتاح_hash = 0;
            next_name->قيمة = names[n].قيمة;

#إذا 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "wc2: \"%V\"", &next_name->مفتاح);
#endif
        }

        لكل (i = n + 1; i < nelts; i++) {
            إذا (ngx_strncmp(names[n].مفتاح.data, names[i].مفتاح.data, len) != 0) {
                اكسر;
            }

            إذا (!dot
                && names[i].مفتاح.len > len
                && names[i].مفتاح.data[len] != '.')
            {
                اكسر;
            }

            next_name = ngx_array_ادفع(&next_names);
            إذا (next_name == عدم) {
                إرجاع NGX_ERROR;
            }

            next_name->مفتاح.len = names[i].مفتاح.len - dot_len;
            next_name->مفتاح.data = names[i].مفتاح.data + dot_len;
            next_name->مفتاح_hash = 0;
            next_name->قيمة = names[i].قيمة;

#إذا 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "wc3: \"%V\"", &next_name->مفتاح);
#endif
        }

        إذا (next_names.nelts) {

            h = *hinit;
            h.hash = عدم;

            إذا (ngx_hash_wildcard_init(&h, (ngx_hash_مفتاح_t *) next_names.elts,
                                       next_names.nelts)
                != NGX_OK)
            {
                إرجاع NGX_ERROR;
            }

            wdc = (ngx_hash_wildcard_t *) h.hash;

            إذا (names[n].مفتاح.len == len) {
                wdc->قيمة = names[n].قيمة;
            }

            name->قيمة = (فراغ *) ((uintptr_t) wdc | (dot ? 3 : 2));

        } وإلا إذا (dot) {
            name->قيمة = (فراغ *) ((uintptr_t) name->قيمة | 1);
        }
    }

    إذا (ngx_hash_init(hinit, (ngx_hash_مفتاح_t *) curr_names.elts,
                      curr_names.nelts)
        != NGX_OK)
    {
        إرجاع NGX_ERROR;
    }

    إرجاع NGX_OK;
}


ngx_uint_t
ngx_hash_مفتاح(u_char *data, size_t len)
{
    ngx_uint_t  i, مفتاح;

    مفتاح = 0;

    لكل (i = 0; i < len; i++) {
        مفتاح = ngx_hash(مفتاح, data[i]);
    }

    إرجاع مفتاح;
}


ngx_uint_t
ngx_hash_مفتاح_lc(u_char *data, size_t len)
{
    ngx_uint_t  i, مفتاح;

    مفتاح = 0;

    لكل (i = 0; i < len; i++) {
        مفتاح = ngx_hash(مفتاح, ngx_tolower(data[i]));
    }

    إرجاع مفتاح;
}


ngx_uint_t
ngx_hash_strlow(u_char *dst, u_char *src, size_t n)
{
    ngx_uint_t  مفتاح;

    مفتاح = 0;

    بينما (n--) {
        *dst = ngx_tolower(*src);
        مفتاح = ngx_hash(مفتاح, *dst);
        dst++;
        src++;
    }

    إرجاع مفتاح;
}


ngx_int_t
ngx_hash_مفاتيح_array_init(ngx_hash_مفاتيح_arrays_t *ha, ngx_uint_t type)
{
    ngx_uint_t  asize;

    إذا (type == NGX_HASH_SMALL) {
        asize = 4;
        ha->hsize = 107;

    } وإلا {
        asize = NGX_HASH_LARGE_ASIZE;
        ha->hsize = NGX_HASH_LARGE_HSIZE;
    }

    إذا (ngx_array_init(&ha->مفاتيح, ha->temp_pool, asize, حجم(ngx_hash_مفتاح_t))
        != NGX_OK)
    {
        إرجاع NGX_ERROR;
    }

    إذا (ngx_array_init(&ha->dns_wc_head, ha->temp_pool, asize,
                       حجم(ngx_hash_مفتاح_t))
        != NGX_OK)
    {
        إرجاع NGX_ERROR;
    }

    إذا (ngx_array_init(&ha->dns_wc_tail, ha->temp_pool, asize,
                       حجم(ngx_hash_مفتاح_t))
        != NGX_OK)
    {
        إرجاع NGX_ERROR;
    }

    ha->مفاتيح_hash = ngx_pcalloc(ha->temp_pool, حجم(ngx_array_t) * ha->hsize);
    إذا (ha->مفاتيح_hash == عدم) {
        إرجاع NGX_ERROR;
    }

    ha->dns_wc_head_hash = ngx_pcalloc(ha->temp_pool,
                                       حجم(ngx_array_t) * ha->hsize);
    إذا (ha->dns_wc_head_hash == عدم) {
        إرجاع NGX_ERROR;
    }

    ha->dns_wc_tail_hash = ngx_pcalloc(ha->temp_pool,
                                       حجم(ngx_array_t) * ha->hsize);
    إذا (ha->dns_wc_tail_hash == عدم) {
        إرجاع NGX_ERROR;
    }

    إرجاع NGX_OK;
}


ngx_int_t
ngx_hash_add_مفتاح(ngx_hash_مفاتيح_arrays_t *ha, ngx_str_t *مفتاح, فراغ *قيمة,
    ngx_uint_t flags)
{
    size_t           len;
    u_char          *p;
    ngx_str_t       *name;
    ngx_uint_t       i, k, n, skip, last;
    ngx_array_t     *مفاتيح, *hwc;
    ngx_hash_مفتاح_t  *hk;

    last = مفتاح->len;

    إذا (flags & NGX_HASH_WILDCARD_مفتاح) {

        /*
         * supported wildcards:
         *     "*.example.com", ".example.com", and "www.example.*"
         */

        n = 0;

        لكل (i = 0; i < مفتاح->len; i++) {

            إذا (مفتاح->data[i] == '*') {
                إذا (++n > 1) {
                    إرجاع NGX_DECLINED;
                }
            }

            إذا (مفتاح->data[i] == '.' && مفتاح->data[i + 1] == '.') {
                إرجاع NGX_DECLINED;
            }

            إذا (مفتاح->data[i] == '\0') {
                إرجاع NGX_DECLINED;
            }
        }

        إذا (مفتاح->len > 1 && مفتاح->data[0] == '.') {
            skip = 1;
            اذهب wildcard;
        }

        إذا (مفتاح->len > 2) {

            إذا (مفتاح->data[0] == '*' && مفتاح->data[1] == '.') {
                skip = 2;
                اذهب wildcard;
            }

            إذا (مفتاح->data[i - 2] == '.' && مفتاح->data[i - 1] == '*') {
                skip = 0;
                last -= 2;
                اذهب wildcard;
            }
        }

        إذا (n) {
            إرجاع NGX_DECLINED;
        }
    }

    /* exact hash */

    k = 0;

    لكل (i = 0; i < last; i++) {
        إذا (!(flags & NGX_HASH_READONLY_مفتاح)) {
            مفتاح->data[i] = ngx_tolower(مفتاح->data[i]);
        }
        k = ngx_hash(k, مفتاح->data[i]);
    }

    k %= ha->hsize;

    /* check conflicts in exact hash */

    name = ha->مفاتيح_hash[k].elts;

    إذا (name) {
        لكل (i = 0; i < ha->مفاتيح_hash[k].nelts; i++) {
            إذا (last != name[i].len) {
                استمر;
            }

            إذا (ngx_strncmp(مفتاح->data, name[i].data, last) == 0) {
                إرجاع NGX_BUSY;
            }
        }

    } وإلا {
        إذا (ngx_array_init(&ha->مفاتيح_hash[k], ha->temp_pool, 4,
                           حجم(ngx_str_t))
            != NGX_OK)
        {
            إرجاع NGX_ERROR;
        }
    }

    name = ngx_array_ادفع(&ha->مفاتيح_hash[k]);
    إذا (name == عدم) {
        إرجاع NGX_ERROR;
    }

    *name = *مفتاح;

    hk = ngx_array_ادفع(&ha->مفاتيح);
    إذا (hk == عدم) {
        إرجاع NGX_ERROR;
    }

    hk->مفتاح = *مفتاح;
    hk->مفتاح_hash = ngx_hash_مفتاح(مفتاح->data, last);
    hk->قيمة = قيمة;

    إرجاع NGX_OK;


wildcard:

    /* wildcard hash */

    k = ngx_hash_strlow(&مفتاح->data[skip], &مفتاح->data[skip], last - skip);

    k %= ha->hsize;

    إذا (skip == 1) {

        /* check conflicts in exact hash for ".example.com" */

        name = ha->مفاتيح_hash[k].elts;

        إذا (name) {
            len = last - skip;

            لكل (i = 0; i < ha->مفاتيح_hash[k].nelts; i++) {
                إذا (len != name[i].len) {
                    استمر;
                }

                إذا (ngx_strncmp(&مفتاح->data[1], name[i].data, len) == 0) {
                    إرجاع NGX_BUSY;
                }
            }

        } وإلا {
            إذا (ngx_array_init(&ha->مفاتيح_hash[k], ha->temp_pool, 4,
                               حجم(ngx_str_t))
                != NGX_OK)
            {
                إرجاع NGX_ERROR;
            }
        }

        name = ngx_array_ادفع(&ha->مفاتيح_hash[k]);
        إذا (name == عدم) {
            إرجاع NGX_ERROR;
        }

        name->len = last - 1;
        name->data = ngx_pnalloc(ha->temp_pool, name->len);
        إذا (name->data == عدم) {
            إرجاع NGX_ERROR;
        }

        ngx_memcpy(name->data, &مفتاح->data[1], name->len);
    }


    إذا (skip) {

        /*
         * convert "*.example.com" to "com.example.\0"
         *      and ".example.com" to "com.example\0"
         */

        p = ngx_pnalloc(ha->temp_pool, last);
        إذا (p == عدم) {
            إرجاع NGX_ERROR;
        }

        len = 0;
        n = 0;

        لكل (i = last - 1; i; i--) {
            إذا (مفتاح->data[i] == '.') {
                ngx_memcpy(&p[n], &مفتاح->data[i + 1], len);
                n += len;
                p[n++] = '.';
                len = 0;
                استمر;
            }

            len++;
        }

        إذا (len) {
            ngx_memcpy(&p[n], &مفتاح->data[1], len);
            n += len;
        }

        p[n] = '\0';

        hwc = &ha->dns_wc_head;
        مفاتيح = &ha->dns_wc_head_hash[k];

    } وإلا {

        /* convert "www.example.*" to "www.example\0" */

        last++;

        p = ngx_pnalloc(ha->temp_pool, last);
        إذا (p == عدم) {
            إرجاع NGX_ERROR;
        }

        ngx_cpystrn(p, مفتاح->data, last);

        hwc = &ha->dns_wc_tail;
        مفاتيح = &ha->dns_wc_tail_hash[k];
    }


    /* check conflicts in wildcard hash */

    name = مفاتيح->elts;

    إذا (name) {
        len = last - skip;

        لكل (i = 0; i < مفاتيح->nelts; i++) {
            إذا (len != name[i].len) {
                استمر;
            }

            إذا (ngx_strncmp(مفتاح->data + skip, name[i].data, len) == 0) {
                إرجاع NGX_BUSY;
            }
        }

    } وإلا {
        إذا (ngx_array_init(مفاتيح, ha->temp_pool, 4, حجم(ngx_str_t)) != NGX_OK)
        {
            إرجاع NGX_ERROR;
        }
    }

    name = ngx_array_ادفع(مفاتيح);
    إذا (name == عدم) {
        إرجاع NGX_ERROR;
    }

    name->len = last - skip;
    name->data = ngx_pnalloc(ha->temp_pool, name->len);
    إذا (name->data == عدم) {
        إرجاع NGX_ERROR;
    }

    ngx_memcpy(name->data, مفتاح->data + skip, name->len);


    /* add to wildcard hash */

    hk = ngx_array_ادفع(hwc);
    إذا (hk == عدم) {
        إرجاع NGX_ERROR;
    }

    hk->مفتاح.len = last - 1;
    hk->مفتاح.data = p;
    hk->مفتاح_hash = 0;
    hk->قيمة = قيمة;

    إرجاع NGX_OK;
}
