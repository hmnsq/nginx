
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#অন্তর্ভাব <ngx_config.h>
#অন্তর্ভাব <ngx_core.h>


শূন্য *
ngx_hash_find(ngx_hash_t *hash, ngx_uint_t চাবি, u_char *name, আকার_t len)
{
    ngx_uint_t       i;
    ngx_hash_elt_t  *elt;

#যদি 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "hf:\"%*s\"", len, name);
#endif

    elt = hash->buckets[চাবি % hash->আকার_2];

    যদি (elt == ফাঁকা) {
        ফেরত ফাঁকা;
    }

    যখন (elt->মান) {
        যদি (len != (আকার_t) elt->len) {
            যাও পরবর্তী;
        }

        জন্য (i = 0; i < len; i++) {
            যদি (name[i] != elt->name[i]) {
                যাও পরবর্তী;
            }
        }

        ফেরত elt->মান;

    পরবর্তী:

        elt = (ngx_hash_elt_t *) ngx_align_ptr(&elt->name[0] + elt->len,
                                               আকার(শূন্য *));
        চালু;
    }

    ফেরত ফাঁকা;
}


শূন্য *
ngx_hash_find_wc_শীর্ষ(ngx_hash_wildcard_t *hwc, u_char *name, আকার_t len)
{
    শূন্য        *মান;
    ngx_uint_t   i, n, চাবি;

#যদি 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "wch:\"%*s\"", len, name);
#endif

    n = len;

    যখন (n) {
        যদি (name[n - 1] == '.') {
            বিরতি;
        }

        n--;
    }

    চাবি = 0;

    জন্য (i = n; i < len; i++) {
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
                    ফেরত ফাঁকা;
                }

                hwc = (ngx_hash_wildcard_t *)
                                          ((uintptr_t) মান & (uintptr_t) ~3);
                ফেরত hwc->মান;
            }

            hwc = (ngx_hash_wildcard_t *) ((uintptr_t) মান & (uintptr_t) ~3);

            মান = ngx_hash_find_wc_শীর্ষ(hwc, name, n - 1);

            যদি (মান) {
                ফেরত মান;
            }

            ফেরত hwc->মান;
        }

        যদি ((uintptr_t) মান & 1) {

            যদি (n == 0) {

                /* "example.com" */

                ফেরত ফাঁকা;
            }

            ফেরত (শূন্য *) ((uintptr_t) মান & (uintptr_t) ~3);
        }

        ফেরত মান;
    }

    ফেরত hwc->মান;
}


শূন্য *
ngx_hash_find_wc_লেজ(ngx_hash_wildcard_t *hwc, u_char *name, আকার_t len)
{
    শূন্য        *মান;
    ngx_uint_t   i, চাবি;

#যদি 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "wct:\"%*s\"", len, name);
#endif

    চাবি = 0;

    জন্য (i = 0; i < len; i++) {
        যদি (name[i] == '.') {
            বিরতি;
        }

        চাবি = ngx_hash(চাবি, name[i]);
    }

    যদি (i == len) {
        ফেরত ফাঁকা;
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

            মান = ngx_hash_find_wc_লেজ(hwc, &name[i], len - i);

            যদি (মান) {
                ফেরত মান;
            }

            ফেরত hwc->মান;
        }

        ফেরত মান;
    }

    ফেরত hwc->মান;
}


শূন্য *
ngx_hash_find_combined(ngx_hash_combined_t *hash, ngx_uint_t চাবি, u_char *name,
    আকার_t len)
{
    শূন্য  *মান;

    যদি (hash->hash.buckets) {
        মান = ngx_hash_find(&hash->hash, চাবি, name, len);

        যদি (মান) {
            ফেরত মান;
        }
    }

    যদি (len == 0) {
        ফেরত ফাঁকা;
    }

    যদি (hash->wc_শীর্ষ && hash->wc_শীর্ষ->hash.buckets) {
        মান = ngx_hash_find_wc_শীর্ষ(hash->wc_শীর্ষ, name, len);

        যদি (মান) {
            ফেরত মান;
        }
    }

    যদি (hash->wc_লেজ && hash->wc_লেজ->hash.buckets) {
        মান = ngx_hash_find_wc_লেজ(hash->wc_লেজ, name, len);

        যদি (মান) {
            ফেরত মান;
        }
    }

    ফেরত ফাঁকা;
}


#define NGX_HASH_ELT_আকার(name)                                               \
    (আকার(শূন্য *) + ngx_align((name)->চাবি.len + 2, আকার(শূন্য *)))

ngx_int_t
ngx_hash_init(ngx_hash_init_t *hinit, ngx_hash_চাবি_t *names, ngx_uint_t nelts)
{
    u_char          *elts;
    আকার_t           len;
    u_short         *test;
    ngx_uint_t       i, n, চাবি, আকার_2, start, bucket_আকার;
    ngx_hash_elt_t  *elt, **buckets;

    যদি (hinit->max_আকার == 0) {
        ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                      "could not build %s, you should "
                      "increase %s_max_size: %i",
                      hinit->name, hinit->name, hinit->max_আকার);
        ফেরত NGX_ERROR;
    }

    যদি (hinit->bucket_আকার > 65536 - ngx_cacheline_আকার) {
        ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                      "could not build %s, too large "
                      "%s_bucket_size: %i",
                      hinit->name, hinit->name, hinit->bucket_আকার);
        ফেরত NGX_ERROR;
    }

    জন্য (n = 0; n < nelts; n++) {
        যদি (names[n].চাবি.তথ্য == ফাঁকা) {
            চালু;
        }

        যদি (hinit->bucket_আকার < NGX_HASH_ELT_আকার(&names[n]) + আকার(শূন্য *))
        {
            ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                          "could not build %s, you should "
                          "increase %s_bucket_size: %i",
                          hinit->name, hinit->name, hinit->bucket_আকার);
            ফেরত NGX_ERROR;
        }
    }

    test = ngx_alloc(hinit->max_আকার * আকার(u_short), hinit->pool->log);
    যদি (test == ফাঁকা) {
        ফেরত NGX_ERROR;
    }

    bucket_আকার = hinit->bucket_আকার - আকার(শূন্য *);

    start = nelts / (bucket_আকার / (2 * আকার(শূন্য *)));
    start = start ? start : 1;

    যদি (hinit->max_আকার > 10000 && nelts && hinit->max_আকার / nelts < 100) {
        start = hinit->max_আকার - 1000;
    }

    জন্য (আকার_2 = start; আকার_2 <= hinit->max_আকার; আকার_2++) {

        ngx_memzero(test, আকার_2 * আকার(u_short));

        জন্য (n = 0; n < nelts; n++) {
            যদি (names[n].চাবি.তথ্য == ফাঁকা) {
                চালু;
            }

            চাবি = names[n].চাবি_hash % আকার_2;
            len = test[চাবি] + NGX_HASH_ELT_আকার(&names[n]);

#যদি 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "%ui: %ui %uz \"%V\"",
                          আকার_2, চাবি, len, &names[n].চাবি);
#endif

            যদি (len > bucket_আকার) {
                যাও পরবর্তী;
            }

            test[চাবি] = (u_short) len;
        }

        যাও found;

    পরবর্তী:

        চালু;
    }

    আকার_2 = hinit->max_আকার;

    ngx_log_error(NGX_LOG_WARN, hinit->pool->log, 0,
                  "could not build optimal %s, you should increase "
                  "either %s_max_size: %i or %s_bucket_size: %i; "
                  "ignoring %s_bucket_size",
                  hinit->name, hinit->name, hinit->max_আকার,
                  hinit->name, hinit->bucket_আকার, hinit->name);

found:

    জন্য (i = 0; i < আকার_2; i++) {
        test[i] = আকার(শূন্য *);
    }

    জন্য (n = 0; n < nelts; n++) {
        যদি (names[n].চাবি.তথ্য == ফাঁকা) {
            চালু;
        }

        চাবি = names[n].চাবি_hash % আকার_2;
        len = test[চাবি] + NGX_HASH_ELT_আকার(&names[n]);

        যদি (len > 65536 - ngx_cacheline_আকার) {
            ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                          "could not build %s, you should "
                          "increase %s_max_size: %i",
                          hinit->name, hinit->name, hinit->max_আকার);
            ngx_free(test);
            ফেরত NGX_ERROR;
        }

        test[চাবি] = (u_short) len;
    }

    len = 0;

    জন্য (i = 0; i < আকার_2; i++) {
        যদি (test[i] == আকার(শূন্য *)) {
            চালু;
        }

        test[i] = (u_short) (ngx_align(test[i], ngx_cacheline_আকার));

        len += test[i];
    }

    যদি (hinit->hash == ফাঁকা) {
        hinit->hash = ngx_pcalloc(hinit->pool, আকার(ngx_hash_wildcard_t)
                                             + আকার_2 * আকার(ngx_hash_elt_t *));
        যদি (hinit->hash == ফাঁকা) {
            ngx_free(test);
            ফেরত NGX_ERROR;
        }

        buckets = (ngx_hash_elt_t **)
                      ((u_char *) hinit->hash + আকার(ngx_hash_wildcard_t));

    } অন্যথা {
        buckets = ngx_pcalloc(hinit->pool, আকার_2 * আকার(ngx_hash_elt_t *));
        যদি (buckets == ফাঁকা) {
            ngx_free(test);
            ফেরত NGX_ERROR;
        }
    }

    elts = ngx_palloc(hinit->pool, len + ngx_cacheline_আকার);
    যদি (elts == ফাঁকা) {
        ngx_free(test);
        ফেরত NGX_ERROR;
    }

    elts = ngx_align_ptr(elts, ngx_cacheline_আকার);

    জন্য (i = 0; i < আকার_2; i++) {
        যদি (test[i] == আকার(শূন্য *)) {
            চালু;
        }

        buckets[i] = (ngx_hash_elt_t *) elts;
        elts += test[i];
    }

    জন্য (i = 0; i < আকার_2; i++) {
        test[i] = 0;
    }

    জন্য (n = 0; n < nelts; n++) {
        যদি (names[n].চাবি.তথ্য == ফাঁকা) {
            চালু;
        }

        চাবি = names[n].চাবি_hash % আকার_2;
        elt = (ngx_hash_elt_t *) ((u_char *) buckets[চাবি] + test[চাবি]);

        elt->মান = names[n].মান;
        elt->len = (u_short) names[n].চাবি.len;

        ngx_strlow(elt->name, names[n].চাবি.তথ্য, names[n].চাবি.len);

        test[চাবি] = (u_short) (test[চাবি] + NGX_HASH_ELT_আকার(&names[n]));
    }

    জন্য (i = 0; i < আকার_2; i++) {
        যদি (buckets[i] == ফাঁকা) {
            চালু;
        }

        elt = (ngx_hash_elt_t *) ((u_char *) buckets[i] + test[i]);

        elt->মান = ফাঁকা;
    }

    ngx_free(test);

    hinit->hash->buckets = buckets;
    hinit->hash->আকার_2 = আকার_2;

#যদি 0

    জন্য (i = 0; i < আকার_2; i++) {
        ngx_str_t   val;
        ngx_uint_t  চাবি;

        elt = buckets[i];

        যদি (elt == ফাঁকা) {
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "%ui: NULL", i);
            চালু;
        }

        যখন (elt->মান) {
            val.len = elt->len;
            val.তথ্য = &elt->name[0];

            চাবি = hinit->চাবি(val.তথ্য, val.len);

            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "%ui: %p \"%V\" %ui", i, elt, &val, চাবি);

            elt = (ngx_hash_elt_t *) ngx_align_ptr(&elt->name[0] + elt->len,
                                                   আকার(শূন্য *));
        }
    }

#endif

    ফেরত NGX_OK;
}


ngx_int_t
ngx_hash_wildcard_init(ngx_hash_init_t *hinit, ngx_hash_চাবি_t *names,
    ngx_uint_t nelts)
{
    আকার_t                len, dot_len;
    ngx_uint_t            i, n, dot;
    ngx_array_t           curr_names, পরবর্তী_names;
    ngx_hash_চাবি_t       *name, *পরবর্তী_name;
    ngx_hash_init_t       h;
    ngx_hash_wildcard_t  *wdc;

    যদি (ngx_array_init(&curr_names, hinit->অস্থায়ী_pool, nelts,
                       আকার(ngx_hash_চাবি_t))
        != NGX_OK)
    {
        ফেরত NGX_ERROR;
    }

    যদি (ngx_array_init(&পরবর্তী_names, hinit->অস্থায়ী_pool, nelts,
                       আকার(ngx_hash_চাবি_t))
        != NGX_OK)
    {
        ফেরত NGX_ERROR;
    }

    জন্য (n = 0; n < nelts; n = i) {

#যদি 0
        ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                      "wc0: \"%V\"", &names[n].চাবি);
#endif

        dot = 0;

        জন্য (len = 0; len < names[n].চাবি.len; len++) {
            যদি (names[n].চাবি.তথ্য[len] == '.') {
                dot = 1;
                বিরতি;
            }
        }

        name = ngx_array_ঠেলো(&curr_names);
        যদি (name == ফাঁকা) {
            ফেরত NGX_ERROR;
        }

        name->চাবি.len = len;
        name->চাবি.তথ্য = names[n].চাবি.তথ্য;
        name->চাবি_hash = hinit->চাবি(name->চাবি.তথ্য, name->চাবি.len);
        name->মান = names[n].মান;

#যদি 0
        ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                      "wc1: \"%V\" %ui", &name->চাবি, dot);
#endif

        dot_len = len + 1;

        যদি (dot) {
            len++;
        }

        পরবর্তী_names.nelts = 0;

        যদি (names[n].চাবি.len != len) {
            পরবর্তী_name = ngx_array_ঠেলো(&পরবর্তী_names);
            যদি (পরবর্তী_name == ফাঁকা) {
                ফেরত NGX_ERROR;
            }

            পরবর্তী_name->চাবি.len = names[n].চাবি.len - len;
            পরবর্তী_name->চাবি.তথ্য = names[n].চাবি.তথ্য + len;
            পরবর্তী_name->চাবি_hash = 0;
            পরবর্তী_name->মান = names[n].মান;

#যদি 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "wc2: \"%V\"", &পরবর্তী_name->চাবি);
#endif
        }

        জন্য (i = n + 1; i < nelts; i++) {
            যদি (ngx_strncmp(names[n].চাবি.তথ্য, names[i].চাবি.তথ্য, len) != 0) {
                বিরতি;
            }

            যদি (!dot
                && names[i].চাবি.len > len
                && names[i].চাবি.তথ্য[len] != '.')
            {
                বিরতি;
            }

            পরবর্তী_name = ngx_array_ঠেলো(&পরবর্তী_names);
            যদি (পরবর্তী_name == ফাঁকা) {
                ফেরত NGX_ERROR;
            }

            পরবর্তী_name->চাবি.len = names[i].চাবি.len - dot_len;
            পরবর্তী_name->চাবি.তথ্য = names[i].চাবি.তথ্য + dot_len;
            পরবর্তী_name->চাবি_hash = 0;
            পরবর্তী_name->মান = names[i].মান;

#যদি 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "wc3: \"%V\"", &পরবর্তী_name->চাবি);
#endif
        }

        যদি (পরবর্তী_names.nelts) {

            h = *hinit;
            h.hash = ফাঁকা;

            যদি (ngx_hash_wildcard_init(&h, (ngx_hash_চাবি_t *) পরবর্তী_names.elts,
                                       পরবর্তী_names.nelts)
                != NGX_OK)
            {
                ফেরত NGX_ERROR;
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
        ফেরত NGX_ERROR;
    }

    ফেরত NGX_OK;
}


ngx_uint_t
ngx_hash_চাবি(u_char *তথ্য, আকার_t len)
{
    ngx_uint_t  i, চাবি;

    চাবি = 0;

    জন্য (i = 0; i < len; i++) {
        চাবি = ngx_hash(চাবি, তথ্য[i]);
    }

    ফেরত চাবি;
}


ngx_uint_t
ngx_hash_চাবি_lc(u_char *তথ্য, আকার_t len)
{
    ngx_uint_t  i, চাবি;

    চাবি = 0;

    জন্য (i = 0; i < len; i++) {
        চাবি = ngx_hash(চাবি, ngx_tolower(তথ্য[i]));
    }

    ফেরত চাবি;
}


ngx_uint_t
ngx_hash_strlow(u_char *dst, u_char *src, আকার_t n)
{
    ngx_uint_t  চাবি;

    চাবি = 0;

    যখন (n--) {
        *dst = ngx_tolower(*src);
        চাবি = ngx_hash(চাবি, *dst);
        dst++;
        src++;
    }

    ফেরত চাবি;
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

    যদি (ngx_array_init(&ha->চাবিগুলি, ha->অস্থায়ী_pool, asize, আকার(ngx_hash_চাবি_t))
        != NGX_OK)
    {
        ফেরত NGX_ERROR;
    }

    যদি (ngx_array_init(&ha->dns_wc_শীর্ষ, ha->অস্থায়ী_pool, asize,
                       আকার(ngx_hash_চাবি_t))
        != NGX_OK)
    {
        ফেরত NGX_ERROR;
    }

    যদি (ngx_array_init(&ha->dns_wc_লেজ, ha->অস্থায়ী_pool, asize,
                       আকার(ngx_hash_চাবি_t))
        != NGX_OK)
    {
        ফেরত NGX_ERROR;
    }

    ha->চাবিগুলি_hash = ngx_pcalloc(ha->অস্থায়ী_pool, আকার(ngx_array_t) * ha->hsize);
    যদি (ha->চাবিগুলি_hash == ফাঁকা) {
        ফেরত NGX_ERROR;
    }

    ha->dns_wc_শীর্ষ_hash = ngx_pcalloc(ha->অস্থায়ী_pool,
                                       আকার(ngx_array_t) * ha->hsize);
    যদি (ha->dns_wc_শীর্ষ_hash == ফাঁকা) {
        ফেরত NGX_ERROR;
    }

    ha->dns_wc_লেজ_hash = ngx_pcalloc(ha->অস্থায়ী_pool,
                                       আকার(ngx_array_t) * ha->hsize);
    যদি (ha->dns_wc_লেজ_hash == ফাঁকা) {
        ফেরত NGX_ERROR;
    }

    ফেরত NGX_OK;
}


ngx_int_t
ngx_hash_add_চাবি(ngx_hash_চাবিগুলি_arrays_t *ha, ngx_str_t *চাবি, শূন্য *মান,
    ngx_uint_t flags)
{
    আকার_t           len;
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

        জন্য (i = 0; i < চাবি->len; i++) {

            যদি (চাবি->তথ্য[i] == '*') {
                যদি (++n > 1) {
                    ফেরত NGX_DECLINED;
                }
            }

            যদি (চাবি->তথ্য[i] == '.' && চাবি->তথ্য[i + 1] == '.') {
                ফেরত NGX_DECLINED;
            }

            যদি (চাবি->তথ্য[i] == '\0') {
                ফেরত NGX_DECLINED;
            }
        }

        যদি (চাবি->len > 1 && চাবি->তথ্য[0] == '.') {
            skip = 1;
            যাও wildcard;
        }

        যদি (চাবি->len > 2) {

            যদি (চাবি->তথ্য[0] == '*' && চাবি->তথ্য[1] == '.') {
                skip = 2;
                যাও wildcard;
            }

            যদি (চাবি->তথ্য[i - 2] == '.' && চাবি->তথ্য[i - 1] == '*') {
                skip = 0;
                last -= 2;
                যাও wildcard;
            }
        }

        যদি (n) {
            ফেরত NGX_DECLINED;
        }
    }

    /* exact hash */

    k = 0;

    জন্য (i = 0; i < last; i++) {
        যদি (!(flags & NGX_HASH_READONLY_চাবি)) {
            চাবি->তথ্য[i] = ngx_tolower(চাবি->তথ্য[i]);
        }
        k = ngx_hash(k, চাবি->তথ্য[i]);
    }

    k %= ha->hsize;

    /* check conflicts in exact hash */

    name = ha->চাবিগুলি_hash[k].elts;

    যদি (name) {
        জন্য (i = 0; i < ha->চাবিগুলি_hash[k].nelts; i++) {
            যদি (last != name[i].len) {
                চালু;
            }

            যদি (ngx_strncmp(চাবি->তথ্য, name[i].তথ্য, last) == 0) {
                ফেরত NGX_BUSY;
            }
        }

    } অন্যথা {
        যদি (ngx_array_init(&ha->চাবিগুলি_hash[k], ha->অস্থায়ী_pool, 4,
                           আকার(ngx_str_t))
            != NGX_OK)
        {
            ফেরত NGX_ERROR;
        }
    }

    name = ngx_array_ঠেলো(&ha->চাবিগুলি_hash[k]);
    যদি (name == ফাঁকা) {
        ফেরত NGX_ERROR;
    }

    *name = *চাবি;

    hk = ngx_array_ঠেলো(&ha->চাবিগুলি);
    যদি (hk == ফাঁকা) {
        ফেরত NGX_ERROR;
    }

    hk->চাবি = *চাবি;
    hk->চাবি_hash = ngx_hash_চাবি(চাবি->তথ্য, last);
    hk->মান = মান;

    ফেরত NGX_OK;


wildcard:

    /* wildcard hash */

    k = ngx_hash_strlow(&চাবি->তথ্য[skip], &চাবি->তথ্য[skip], last - skip);

    k %= ha->hsize;

    যদি (skip == 1) {

        /* check conflicts in exact hash for ".example.com" */

        name = ha->চাবিগুলি_hash[k].elts;

        যদি (name) {
            len = last - skip;

            জন্য (i = 0; i < ha->চাবিগুলি_hash[k].nelts; i++) {
                যদি (len != name[i].len) {
                    চালু;
                }

                যদি (ngx_strncmp(&চাবি->তথ্য[1], name[i].তথ্য, len) == 0) {
                    ফেরত NGX_BUSY;
                }
            }

        } অন্যথা {
            যদি (ngx_array_init(&ha->চাবিগুলি_hash[k], ha->অস্থায়ী_pool, 4,
                               আকার(ngx_str_t))
                != NGX_OK)
            {
                ফেরত NGX_ERROR;
            }
        }

        name = ngx_array_ঠেলো(&ha->চাবিগুলি_hash[k]);
        যদি (name == ফাঁকা) {
            ফেরত NGX_ERROR;
        }

        name->len = last - 1;
        name->তথ্য = ngx_pnalloc(ha->অস্থায়ী_pool, name->len);
        যদি (name->তথ্য == ফাঁকা) {
            ফেরত NGX_ERROR;
        }

        ngx_memcpy(name->তথ্য, &চাবি->তথ্য[1], name->len);
    }


    যদি (skip) {

        /*
         * convert "*.example.com" to "com.example.\0"
         *      and ".example.com" to "com.example\0"
         */

        p = ngx_pnalloc(ha->অস্থায়ী_pool, last);
        যদি (p == ফাঁকা) {
            ফেরত NGX_ERROR;
        }

        len = 0;
        n = 0;

        জন্য (i = last - 1; i; i--) {
            যদি (চাবি->তথ্য[i] == '.') {
                ngx_memcpy(&p[n], &চাবি->তথ্য[i + 1], len);
                n += len;
                p[n++] = '.';
                len = 0;
                চালু;
            }

            len++;
        }

        যদি (len) {
            ngx_memcpy(&p[n], &চাবি->তথ্য[1], len);
            n += len;
        }

        p[n] = '\0';

        hwc = &ha->dns_wc_শীর্ষ;
        চাবিগুলি = &ha->dns_wc_শীর্ষ_hash[k];

    } অন্যথা {

        /* convert "www.example.*" to "www.example\0" */

        last++;

        p = ngx_pnalloc(ha->অস্থায়ী_pool, last);
        যদি (p == ফাঁকা) {
            ফেরত NGX_ERROR;
        }

        ngx_cpystrn(p, চাবি->তথ্য, last);

        hwc = &ha->dns_wc_লেজ;
        চাবিগুলি = &ha->dns_wc_লেজ_hash[k];
    }


    /* check conflicts in wildcard hash */

    name = চাবিগুলি->elts;

    যদি (name) {
        len = last - skip;

        জন্য (i = 0; i < চাবিগুলি->nelts; i++) {
            যদি (len != name[i].len) {
                চালু;
            }

            যদি (ngx_strncmp(চাবি->তথ্য + skip, name[i].তথ্য, len) == 0) {
                ফেরত NGX_BUSY;
            }
        }

    } অন্যথা {
        যদি (ngx_array_init(চাবিগুলি, ha->অস্থায়ী_pool, 4, আকার(ngx_str_t)) != NGX_OK)
        {
            ফেরত NGX_ERROR;
        }
    }

    name = ngx_array_ঠেলো(চাবিগুলি);
    যদি (name == ফাঁকা) {
        ফেরত NGX_ERROR;
    }

    name->len = last - skip;
    name->তথ্য = ngx_pnalloc(ha->অস্থায়ী_pool, name->len);
    যদি (name->তথ্য == ফাঁকা) {
        ফেরত NGX_ERROR;
    }

    ngx_memcpy(name->তথ্য, চাবি->তথ্য + skip, name->len);


    /* add to wildcard hash */

    hk = ngx_array_ঠেলো(hwc);
    যদি (hk == ফাঁকা) {
        ফেরত NGX_ERROR;
    }

    hk->চাবি.len = last - 1;
    hk->চাবি.তথ্য = p;
    hk->চাবি_hash = 0;
    hk->মান = মান;

    ফেরত NGX_OK;
}
