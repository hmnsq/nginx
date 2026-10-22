
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#অন্তর্ভাৱ <ngx_config.h>
#অন্তর্ভাৱ <ngx_core.h>


শূন্য *
ngx_hash_find(ngx_hash_t *hash, ngx_uint_t চাবি, u_char *name, size_t len)
{
    ngx_uint_t       i;
    ngx_hash_elt_t  *elt;

#যদি 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "hf:\"%*s\"", len, name);
#endif

    elt = hash->buckets[চাবি % hash->size];

    যদি (elt == রিক্ত) {
        ঘূৰাই রিক্ত;
    }

    যেতিয়ালৈ (elt->মান) {
        যদি (len != (size_t) elt->len) {
            গচ্ছ next;
        }

        বাবে (i = 0; i < len; i++) {
            যদি (name[i] != elt->name[i]) {
                গচ্ছ next;
            }
        }

        ঘূৰাই elt->মান;

    next:

        elt = (ngx_hash_elt_t *) ngx_align_ptr(&elt->name[0] + elt->len,
                                               পরিমাণ(শূন্য *));
        অনুৱর্ত;
    }

    ঘূৰাই রিক্ত;
}


শূন্য *
ngx_hash_find_wc_head(ngx_hash_wildcard_t *hwc, u_char *name, size_t len)
{
    শূন্য        *মান;
    ngx_uint_t   i, n, চাবি;

#যদি 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "wch:\"%*s\"", len, name);
#endif

    n = len;

    যেতিয়ালৈ (n) {
        যদি (name[n - 1] == '.') {
            ভঙ্গ;
        }

        n--;
    }

    চাবি = 0;

    বাবে (i = n; i < len; i++) {
        চাবি = ngx_hash(চাবি, name[i]);
    }

#যদি 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "key:\"%ui\"", চাবি);
#endif

    মান = ngx_hash_find(&hwc->hash, চাবি, &name[n], len - n);

#যদি 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "value:\"%p\"", মান);
#endif

    যদি (মান) {

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

        যদি ((uintptr_t) মান & 2) {

            যদি (n == 0) {

                /* "example.com" */

                যদি ((uintptr_t) মান & 1) {
                    ঘূৰাই রিক্ত;
                }

                hwc = (ngx_hash_wildcard_t *)
                                          ((uintptr_t) মান & (uintptr_t) ~3);
                ঘূৰাই hwc->মান;
            }

            hwc = (ngx_hash_wildcard_t *) ((uintptr_t) মান & (uintptr_t) ~3);

            মান = ngx_hash_find_wc_head(hwc, name, n - 1);

            যদি (মান) {
                ঘূৰাই মান;
            }

            ঘূৰাই hwc->মান;
        }

        যদি ((uintptr_t) মান & 1) {

            যদি (n == 0) {

                /* "example.com" */

                ঘূৰাই রিক্ত;
            }

            ঘূৰাই (শূন্য *) ((uintptr_t) মান & (uintptr_t) ~3);
        }

        ঘূৰাই মান;
    }

    ঘূৰাই hwc->মান;
}


শূন্য *
ngx_hash_find_wc_tail(ngx_hash_wildcard_t *hwc, u_char *name, size_t len)
{
    শূন্য        *মান;
    ngx_uint_t   i, চাবি;

#যদি 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "wct:\"%*s\"", len, name);
#endif

    চাবি = 0;

    বাবে (i = 0; i < len; i++) {
        যদি (name[i] == '.') {
            ভঙ্গ;
        }

        চাবি = ngx_hash(চাবি, name[i]);
    }

    যদি (i == len) {
        ঘূৰাই রিক্ত;
    }

#যদি 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "key:\"%ui\"", চাবি);
#endif

    মান = ngx_hash_find(&hwc->hash, চাবি, name, i);

#যদি 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "value:\"%p\"", মান);
#endif

    যদি (মান) {

        /*
         * the 2 low bits of value have the special meaning:
         *     00 - value is data pointer;
         *     11 - value is pointer to wildcard hash allowing "example.*".
         */

        যদি ((uintptr_t) মান & 2) {

            i++;

            hwc = (ngx_hash_wildcard_t *) ((uintptr_t) মান & (uintptr_t) ~3);

            মান = ngx_hash_find_wc_tail(hwc, &name[i], len - i);

            যদি (মান) {
                ঘূৰাই মান;
            }

            ঘূৰাই hwc->মান;
        }

        ঘূৰাই মান;
    }

    ঘূৰাই hwc->মান;
}


শূন্য *
ngx_hash_find_combined(ngx_hash_combined_t *hash, ngx_uint_t চাবি, u_char *name,
    size_t len)
{
    শূন্য  *মান;

    যদি (hash->hash.buckets) {
        মান = ngx_hash_find(&hash->hash, চাবি, name, len);

        যদি (মান) {
            ঘূৰাই মান;
        }
    }

    যদি (len == 0) {
        ঘূৰাই রিক্ত;
    }

    যদি (hash->wc_head && hash->wc_head->hash.buckets) {
        মান = ngx_hash_find_wc_head(hash->wc_head, name, len);

        যদি (মান) {
            ঘূৰাই মান;
        }
    }

    যদি (hash->wc_tail && hash->wc_tail->hash.buckets) {
        মান = ngx_hash_find_wc_tail(hash->wc_tail, name, len);

        যদি (মান) {
            ঘূৰাই মান;
        }
    }

    ঘূৰাই রিক্ত;
}


#define NGX_HASH_ELT_SIZE(name)                                               \
    (পরিমাণ(শূন্য *) + ngx_align((name)->চাবি.len + 2, পরিমাণ(শূন্য *)))

ngx_int_t
ngx_hash_init(ngx_hash_init_t *hinit, ngx_hash_চাবি_t *names, ngx_uint_t nelts)
{
    u_char          *elts;
    size_t           len;
    u_short         *test;
    ngx_uint_t       i, n, চাবি, size, start, bucket_size;
    ngx_hash_elt_t  *elt, **buckets;

    যদি (hinit->max_size == 0) {
        ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                      "could not build %s, you should "
                      "increase %s_max_size: %i",
                      hinit->name, hinit->name, hinit->max_size);
        ঘূৰাই NGX_ERROR;
    }

    যদি (hinit->bucket_size > 65536 - ngx_cacheline_size) {
        ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                      "could not build %s, too large "
                      "%s_bucket_size: %i",
                      hinit->name, hinit->name, hinit->bucket_size);
        ঘূৰাই NGX_ERROR;
    }

    বাবে (n = 0; n < nelts; n++) {
        যদি (names[n].চাবি.data == রিক্ত) {
            অনুৱর্ত;
        }

        যদি (hinit->bucket_size < NGX_HASH_ELT_SIZE(&names[n]) + পরিমাণ(শূন্য *))
        {
            ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                          "could not build %s, you should "
                          "increase %s_bucket_size: %i",
                          hinit->name, hinit->name, hinit->bucket_size);
            ঘূৰাই NGX_ERROR;
        }
    }

    test = ngx_alloc(hinit->max_size * পরিমাণ(u_short), hinit->pool->log);
    যদি (test == রিক্ত) {
        ঘূৰাই NGX_ERROR;
    }

    bucket_size = hinit->bucket_size - পরিমাণ(শূন্য *);

    start = nelts / (bucket_size / (2 * পরিমাণ(শূন্য *)));
    start = start ? start : 1;

    যদি (hinit->max_size > 10000 && nelts && hinit->max_size / nelts < 100) {
        start = hinit->max_size - 1000;
    }

    বাবে (size = start; size <= hinit->max_size; size++) {

        ngx_memzero(test, size * পরিমাণ(u_short));

        বাবে (n = 0; n < nelts; n++) {
            যদি (names[n].চাবি.data == রিক্ত) {
                অনুৱর্ত;
            }

            চাবি = names[n].চাবি_hash % size;
            len = test[চাবি] + NGX_HASH_ELT_SIZE(&names[n]);

#যদি 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "%ui: %ui %uz \"%V\"",
                          size, চাবি, len, &names[n].চাবি);
#endif

            যদি (len > bucket_size) {
                গচ্ছ next;
            }

            test[চাবি] = (u_short) len;
        }

        গচ্ছ found;

    next:

        অনুৱর্ত;
    }

    size = hinit->max_size;

    ngx_log_error(NGX_LOG_WARN, hinit->pool->log, 0,
                  "could not build optimal %s, you should increase "
                  "either %s_max_size: %i or %s_bucket_size: %i; "
                  "ignoring %s_bucket_size",
                  hinit->name, hinit->name, hinit->max_size,
                  hinit->name, hinit->bucket_size, hinit->name);

found:

    বাবে (i = 0; i < size; i++) {
        test[i] = পরিমাণ(শূন্য *);
    }

    বাবে (n = 0; n < nelts; n++) {
        যদি (names[n].চাবি.data == রিক্ত) {
            অনুৱর্ত;
        }

        চাবি = names[n].চাবি_hash % size;
        len = test[চাবি] + NGX_HASH_ELT_SIZE(&names[n]);

        যদি (len > 65536 - ngx_cacheline_size) {
            ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                          "could not build %s, you should "
                          "increase %s_max_size: %i",
                          hinit->name, hinit->name, hinit->max_size);
            ngx_free(test);
            ঘূৰাই NGX_ERROR;
        }

        test[চাবি] = (u_short) len;
    }

    len = 0;

    বাবে (i = 0; i < size; i++) {
        যদি (test[i] == পরিমাণ(শূন্য *)) {
            অনুৱর্ত;
        }

        test[i] = (u_short) (ngx_align(test[i], ngx_cacheline_size));

        len += test[i];
    }

    যদি (hinit->hash == রিক্ত) {
        hinit->hash = ngx_pcalloc(hinit->pool, পরিমাণ(ngx_hash_wildcard_t)
                                             + size * পরিমাণ(ngx_hash_elt_t *));
        যদি (hinit->hash == রিক্ত) {
            ngx_free(test);
            ঘূৰাই NGX_ERROR;
        }

        buckets = (ngx_hash_elt_t **)
                      ((u_char *) hinit->hash + পরিমাণ(ngx_hash_wildcard_t));

    } অন্যথা {
        buckets = ngx_pcalloc(hinit->pool, size * পরিমাণ(ngx_hash_elt_t *));
        যদি (buckets == রিক্ত) {
            ngx_free(test);
            ঘূৰাই NGX_ERROR;
        }
    }

    elts = ngx_palloc(hinit->pool, len + ngx_cacheline_size);
    যদি (elts == রিক্ত) {
        ngx_free(test);
        ঘূৰাই NGX_ERROR;
    }

    elts = ngx_align_ptr(elts, ngx_cacheline_size);

    বাবে (i = 0; i < size; i++) {
        যদি (test[i] == পরিমাণ(শূন্য *)) {
            অনুৱর্ত;
        }

        buckets[i] = (ngx_hash_elt_t *) elts;
        elts += test[i];
    }

    বাবে (i = 0; i < size; i++) {
        test[i] = 0;
    }

    বাবে (n = 0; n < nelts; n++) {
        যদি (names[n].চাবি.data == রিক্ত) {
            অনুৱর্ত;
        }

        চাবি = names[n].চাবি_hash % size;
        elt = (ngx_hash_elt_t *) ((u_char *) buckets[চাবি] + test[চাবি]);

        elt->মান = names[n].মান;
        elt->len = (u_short) names[n].চাবি.len;

        ngx_strlow(elt->name, names[n].চাবি.data, names[n].চাবি.len);

        test[চাবি] = (u_short) (test[চাবি] + NGX_HASH_ELT_SIZE(&names[n]));
    }

    বাবে (i = 0; i < size; i++) {
        যদি (buckets[i] == রিক্ত) {
            অনুৱর্ত;
        }

        elt = (ngx_hash_elt_t *) ((u_char *) buckets[i] + test[i]);

        elt->মান = রিক্ত;
    }

    ngx_free(test);

    hinit->hash->buckets = buckets;
    hinit->hash->size = size;

#যদি 0

    বাবে (i = 0; i < size; i++) {
        ngx_str_t   val;
        ngx_uint_t  চাবি;

        elt = buckets[i];

        যদি (elt == রিক্ত) {
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "%ui: NULL", i);
            অনুৱর্ত;
        }

        যেতিয়ালৈ (elt->মান) {
            val.len = elt->len;
            val.data = &elt->name[0];

            চাবি = hinit->চাবি(val.data, val.len);

            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "%ui: %p \"%V\" %ui", i, elt, &val, চাবি);

            elt = (ngx_hash_elt_t *) ngx_align_ptr(&elt->name[0] + elt->len,
                                                   পরিমাণ(শূন্য *));
        }
    }

#endif

    ঘূৰাই NGX_OK;
}


ngx_int_t
ngx_hash_wildcard_init(ngx_hash_init_t *hinit, ngx_hash_চাবি_t *names,
    ngx_uint_t nelts)
{
    size_t                len, dot_len;
    ngx_uint_t            i, n, dot;
    ngx_array_t           curr_names, next_names;
    ngx_hash_চাবি_t       *name, *next_name;
    ngx_hash_init_t       h;
    ngx_hash_wildcard_t  *wdc;

    যদি (ngx_array_init(&curr_names, hinit->temp_pool, nelts,
                       পরিমাণ(ngx_hash_চাবি_t))
        != NGX_OK)
    {
        ঘূৰাই NGX_ERROR;
    }

    যদি (ngx_array_init(&next_names, hinit->temp_pool, nelts,
                       পরিমাণ(ngx_hash_চাবি_t))
        != NGX_OK)
    {
        ঘূৰাই NGX_ERROR;
    }

    বাবে (n = 0; n < nelts; n = i) {

#যদি 0
        ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                      "wc0: \"%V\"", &names[n].চাবি);
#endif

        dot = 0;

        বাবে (len = 0; len < names[n].চাবি.len; len++) {
            যদি (names[n].চাবি.data[len] == '.') {
                dot = 1;
                ভঙ্গ;
            }
        }

        name = ngx_array_ঠেলো(&curr_names);
        যদি (name == রিক্ত) {
            ঘূৰাই NGX_ERROR;
        }

        name->চাবি.len = len;
        name->চাবি.data = names[n].চাবি.data;
        name->চাবি_hash = hinit->চাবি(name->চাবি.data, name->চাবি.len);
        name->মান = names[n].মান;

#যদি 0
        ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                      "wc1: \"%V\" %ui", &name->চাবি, dot);
#endif

        dot_len = len + 1;

        যদি (dot) {
            len++;
        }

        next_names.nelts = 0;

        যদি (names[n].চাবি.len != len) {
            next_name = ngx_array_ঠেলো(&next_names);
            যদি (next_name == রিক্ত) {
                ঘূৰাই NGX_ERROR;
            }

            next_name->চাবি.len = names[n].চাবি.len - len;
            next_name->চাবি.data = names[n].চাবি.data + len;
            next_name->চাবি_hash = 0;
            next_name->মান = names[n].মান;

#যদি 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "wc2: \"%V\"", &next_name->চাবি);
#endif
        }

        বাবে (i = n + 1; i < nelts; i++) {
            যদি (ngx_strncmp(names[n].চাবি.data, names[i].চাবি.data, len) != 0) {
                ভঙ্গ;
            }

            যদি (!dot
                && names[i].চাবি.len > len
                && names[i].চাবি.data[len] != '.')
            {
                ভঙ্গ;
            }

            next_name = ngx_array_ঠেলো(&next_names);
            যদি (next_name == রিক্ত) {
                ঘূৰাই NGX_ERROR;
            }

            next_name->চাবি.len = names[i].চাবি.len - dot_len;
            next_name->চাবি.data = names[i].চাবি.data + dot_len;
            next_name->চাবি_hash = 0;
            next_name->মান = names[i].মান;

#যদি 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "wc3: \"%V\"", &next_name->চাবি);
#endif
        }

        যদি (next_names.nelts) {

            h = *hinit;
            h.hash = রিক্ত;

            যদি (ngx_hash_wildcard_init(&h, (ngx_hash_চাবি_t *) next_names.elts,
                                       next_names.nelts)
                != NGX_OK)
            {
                ঘূৰাই NGX_ERROR;
            }

            wdc = (ngx_hash_wildcard_t *) h.hash;

            যদি (names[n].চাবি.len == len) {
                wdc->মান = names[n].মান;
            }

            name->মান = (শূন্য *) ((uintptr_t) wdc | (dot ? 3 : 2));

        } অন্যথা যদি (dot) {
            name->মান = (শূন্য *) ((uintptr_t) name->মান | 1);
        }
    }

    যদি (ngx_hash_init(hinit, (ngx_hash_চাবি_t *) curr_names.elts,
                      curr_names.nelts)
        != NGX_OK)
    {
        ঘূৰাই NGX_ERROR;
    }

    ঘূৰাই NGX_OK;
}


ngx_uint_t
ngx_hash_চাবি(u_char *data, size_t len)
{
    ngx_uint_t  i, চাবি;

    চাবি = 0;

    বাবে (i = 0; i < len; i++) {
        চাবি = ngx_hash(চাবি, data[i]);
    }

    ঘূৰাই চাবি;
}


ngx_uint_t
ngx_hash_চাবি_lc(u_char *data, size_t len)
{
    ngx_uint_t  i, চাবি;

    চাবি = 0;

    বাবে (i = 0; i < len; i++) {
        চাবি = ngx_hash(চাবি, ngx_tolower(data[i]));
    }

    ঘূৰাই চাবি;
}


ngx_uint_t
ngx_hash_strlow(u_char *dst, u_char *src, size_t n)
{
    ngx_uint_t  চাবি;

    চাবি = 0;

    যেতিয়ালৈ (n--) {
        *dst = ngx_tolower(*src);
        চাবি = ngx_hash(চাবি, *dst);
        dst++;
        src++;
    }

    ঘূৰাই চাবি;
}


ngx_int_t
ngx_hash_চাবিগুলি_array_init(ngx_hash_চাবিগুলি_arrays_t *ha, ngx_uint_t type)
{
    ngx_uint_t  asize;

    যদি (type == NGX_HASH_SMALL) {
        asize = 4;
        ha->hsize = 107;

    } অন্যথা {
        asize = NGX_HASH_LARGE_ASIZE;
        ha->hsize = NGX_HASH_LARGE_HSIZE;
    }

    যদি (ngx_array_init(&ha->চাবিগুলি, ha->temp_pool, asize, পরিমাণ(ngx_hash_চাবি_t))
        != NGX_OK)
    {
        ঘূৰাই NGX_ERROR;
    }

    যদি (ngx_array_init(&ha->dns_wc_head, ha->temp_pool, asize,
                       পরিমাণ(ngx_hash_চাবি_t))
        != NGX_OK)
    {
        ঘূৰাই NGX_ERROR;
    }

    যদি (ngx_array_init(&ha->dns_wc_tail, ha->temp_pool, asize,
                       পরিমাণ(ngx_hash_চাবি_t))
        != NGX_OK)
    {
        ঘূৰাই NGX_ERROR;
    }

    ha->চাবিগুলি_hash = ngx_pcalloc(ha->temp_pool, পরিমাণ(ngx_array_t) * ha->hsize);
    যদি (ha->চাবিগুলি_hash == রিক্ত) {
        ঘূৰাই NGX_ERROR;
    }

    ha->dns_wc_head_hash = ngx_pcalloc(ha->temp_pool,
                                       পরিমাণ(ngx_array_t) * ha->hsize);
    যদি (ha->dns_wc_head_hash == রিক্ত) {
        ঘূৰাই NGX_ERROR;
    }

    ha->dns_wc_tail_hash = ngx_pcalloc(ha->temp_pool,
                                       পরিমাণ(ngx_array_t) * ha->hsize);
    যদি (ha->dns_wc_tail_hash == রিক্ত) {
        ঘূৰাই NGX_ERROR;
    }

    ঘূৰাই NGX_OK;
}


ngx_int_t
ngx_hash_add_চাবি(ngx_hash_চাবিগুলি_arrays_t *ha, ngx_str_t *চাবি, শূন্য *মান,
    ngx_uint_t flags)
{
    size_t           len;
    u_char          *p;
    ngx_str_t       *name;
    ngx_uint_t       i, k, n, skip, last;
    ngx_array_t     *চাবিগুলি, *hwc;
    ngx_hash_চাবি_t  *hk;

    last = চাবি->len;

    যদি (flags & NGX_HASH_WILDCARD_চাবি) {

        /*
         * supported wildcards:
         *     "*.example.com", ".example.com", and "www.example.*"
         */

        n = 0;

        বাবে (i = 0; i < চাবি->len; i++) {

            যদি (চাবি->data[i] == '*') {
                যদি (++n > 1) {
                    ঘূৰাই NGX_DECLINED;
                }
            }

            যদি (চাবি->data[i] == '.' && চাবি->data[i + 1] == '.') {
                ঘূৰাই NGX_DECLINED;
            }

            যদি (চাবি->data[i] == '\0') {
                ঘূৰাই NGX_DECLINED;
            }
        }

        যদি (চাবি->len > 1 && চাবি->data[0] == '.') {
            skip = 1;
            গচ্ছ wildcard;
        }

        যদি (চাবি->len > 2) {

            যদি (চাবি->data[0] == '*' && চাবি->data[1] == '.') {
                skip = 2;
                গচ্ছ wildcard;
            }

            যদি (চাবি->data[i - 2] == '.' && চাবি->data[i - 1] == '*') {
                skip = 0;
                last -= 2;
                গচ্ছ wildcard;
            }
        }

        যদি (n) {
            ঘূৰাই NGX_DECLINED;
        }
    }

    /* exact hash */

    k = 0;

    বাবে (i = 0; i < last; i++) {
        যদি (!(flags & NGX_HASH_READONLY_চাবি)) {
            চাবি->data[i] = ngx_tolower(চাবি->data[i]);
        }
        k = ngx_hash(k, চাবি->data[i]);
    }

    k %= ha->hsize;

    /* check conflicts in exact hash */

    name = ha->চাবিগুলি_hash[k].elts;

    যদি (name) {
        বাবে (i = 0; i < ha->চাবিগুলি_hash[k].nelts; i++) {
            যদি (last != name[i].len) {
                অনুৱর্ত;
            }

            যদি (ngx_strncmp(চাবি->data, name[i].data, last) == 0) {
                ঘূৰাই NGX_BUSY;
            }
        }

    } অন্যথা {
        যদি (ngx_array_init(&ha->চাবিগুলি_hash[k], ha->temp_pool, 4,
                           পরিমাণ(ngx_str_t))
            != NGX_OK)
        {
            ঘূৰাই NGX_ERROR;
        }
    }

    name = ngx_array_ঠেলো(&ha->চাবিগুলি_hash[k]);
    যদি (name == রিক্ত) {
        ঘূৰাই NGX_ERROR;
    }

    *name = *চাবি;

    hk = ngx_array_ঠেলো(&ha->চাবিগুলি);
    যদি (hk == রিক্ত) {
        ঘূৰাই NGX_ERROR;
    }

    hk->চাবি = *চাবি;
    hk->চাবি_hash = ngx_hash_চাবি(চাবি->data, last);
    hk->মান = মান;

    ঘূৰাই NGX_OK;


wildcard:

    /* wildcard hash */

    k = ngx_hash_strlow(&চাবি->data[skip], &চাবি->data[skip], last - skip);

    k %= ha->hsize;

    যদি (skip == 1) {

        /* check conflicts in exact hash for ".example.com" */

        name = ha->চাবিগুলি_hash[k].elts;

        যদি (name) {
            len = last - skip;

            বাবে (i = 0; i < ha->চাবিগুলি_hash[k].nelts; i++) {
                যদি (len != name[i].len) {
                    অনুৱর্ত;
                }

                যদি (ngx_strncmp(&চাবি->data[1], name[i].data, len) == 0) {
                    ঘূৰাই NGX_BUSY;
                }
            }

        } অন্যথা {
            যদি (ngx_array_init(&ha->চাবিগুলি_hash[k], ha->temp_pool, 4,
                               পরিমাণ(ngx_str_t))
                != NGX_OK)
            {
                ঘূৰাই NGX_ERROR;
            }
        }

        name = ngx_array_ঠেলো(&ha->চাবিগুলি_hash[k]);
        যদি (name == রিক্ত) {
            ঘূৰাই NGX_ERROR;
        }

        name->len = last - 1;
        name->data = ngx_pnalloc(ha->temp_pool, name->len);
        যদি (name->data == রিক্ত) {
            ঘূৰাই NGX_ERROR;
        }

        ngx_memcpy(name->data, &চাবি->data[1], name->len);
    }


    যদি (skip) {

        /*
         * convert "*.example.com" to "com.example.\0"
         *      and ".example.com" to "com.example\0"
         */

        p = ngx_pnalloc(ha->temp_pool, last);
        যদি (p == রিক্ত) {
            ঘূৰাই NGX_ERROR;
        }

        len = 0;
        n = 0;

        বাবে (i = last - 1; i; i--) {
            যদি (চাবি->data[i] == '.') {
                ngx_memcpy(&p[n], &চাবি->data[i + 1], len);
                n += len;
                p[n++] = '.';
                len = 0;
                অনুৱর্ত;
            }

            len++;
        }

        যদি (len) {
            ngx_memcpy(&p[n], &চাবি->data[1], len);
            n += len;
        }

        p[n] = '\0';

        hwc = &ha->dns_wc_head;
        চাবিগুলি = &ha->dns_wc_head_hash[k];

    } অন্যথা {

        /* convert "www.example.*" to "www.example\0" */

        last++;

        p = ngx_pnalloc(ha->temp_pool, last);
        যদি (p == রিক্ত) {
            ঘূৰাই NGX_ERROR;
        }

        ngx_cpystrn(p, চাবি->data, last);

        hwc = &ha->dns_wc_tail;
        চাবিগুলি = &ha->dns_wc_tail_hash[k];
    }


    /* check conflicts in wildcard hash */

    name = চাবিগুলি->elts;

    যদি (name) {
        len = last - skip;

        বাবে (i = 0; i < চাবিগুলি->nelts; i++) {
            যদি (len != name[i].len) {
                অনুৱর্ত;
            }

            যদি (ngx_strncmp(চাবি->data + skip, name[i].data, len) == 0) {
                ঘূৰাই NGX_BUSY;
            }
        }

    } অন্যথা {
        যদি (ngx_array_init(চাবিগুলি, ha->temp_pool, 4, পরিমাণ(ngx_str_t)) != NGX_OK)
        {
            ঘূৰাই NGX_ERROR;
        }
    }

    name = ngx_array_ঠেলো(চাবিগুলি);
    যদি (name == রিক্ত) {
        ঘূৰাই NGX_ERROR;
    }

    name->len = last - skip;
    name->data = ngx_pnalloc(ha->temp_pool, name->len);
    যদি (name->data == রিক্ত) {
        ঘূৰাই NGX_ERROR;
    }

    ngx_memcpy(name->data, চাবি->data + skip, name->len);


    /* add to wildcard hash */

    hk = ngx_array_ঠেলো(hwc);
    যদি (hk == রিক্ত) {
        ঘূৰাই NGX_ERROR;
    }

    hk->চাবি.len = last - 1;
    hk->চাবি.data = p;
    hk->চাবি_hash = 0;
    hk->মান = মান;

    ঘূৰাই NGX_OK;
}
