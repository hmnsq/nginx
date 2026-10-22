
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#அந்தர்பாவ <ngx_config.h>
#அந்தர்பாவ <ngx_core.h>


வெறுமை *
ngx_hash_find(ngx_hash_t *hash, ngx_uint_t சாவி, u_char *name, size_t len)
{
    ngx_uint_t       i;
    ngx_hash_elt_t  *elt;

#எனில் 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "hf:\"%*s\"", len, name);
#endif

    elt = hash->buckets[சாவி % hash->size];

    எனில் (elt == வெற்று) {
        திரும்பு வெற்று;
    }

    போது (elt->மதிப்பு) {
        எனில் (len != (size_t) elt->len) {
            செல் next;
        }

        க்கு (i = 0; i < len; i++) {
            எனில் (name[i] != elt->name[i]) {
                செல் next;
            }
        }

        திரும்பு elt->மதிப்பு;

    next:

        elt = (ngx_hash_elt_t *) ngx_align_ptr(&elt->name[0] + elt->len,
                                               அளவு(வெறுமை *));
        தொடர்;
    }

    திரும்பு வெற்று;
}


வெறுமை *
ngx_hash_find_wc_head(ngx_hash_wildcard_t *hwc, u_char *name, size_t len)
{
    வெறுமை        *மதிப்பு;
    ngx_uint_t   i, n, சாவி;

#எனில் 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "wch:\"%*s\"", len, name);
#endif

    n = len;

    போது (n) {
        எனில் (name[n - 1] == '.') {
            நிறுத்து;
        }

        n--;
    }

    சாவி = 0;

    க்கு (i = n; i < len; i++) {
        சாவி = ngx_hash(சாவி, name[i]);
    }

#எனில் 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "key:\"%ui\"", சாவி);
#endif

    மதிப்பு = ngx_hash_find(&hwc->hash, சாவி, &name[n], len - n);

#எனில் 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "value:\"%p\"", மதிப்பு);
#endif

    எனில் (மதிப்பு) {

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

        எனில் ((uintptr_t) மதிப்பு & 2) {

            எனில் (n == 0) {

                /* "example.com" */

                எனில் ((uintptr_t) மதிப்பு & 1) {
                    திரும்பு வெற்று;
                }

                hwc = (ngx_hash_wildcard_t *)
                                          ((uintptr_t) மதிப்பு & (uintptr_t) ~3);
                திரும்பு hwc->மதிப்பு;
            }

            hwc = (ngx_hash_wildcard_t *) ((uintptr_t) மதிப்பு & (uintptr_t) ~3);

            மதிப்பு = ngx_hash_find_wc_head(hwc, name, n - 1);

            எனில் (மதிப்பு) {
                திரும்பு மதிப்பு;
            }

            திரும்பு hwc->மதிப்பு;
        }

        எனில் ((uintptr_t) மதிப்பு & 1) {

            எனில் (n == 0) {

                /* "example.com" */

                திரும்பு வெற்று;
            }

            திரும்பு (வெறுமை *) ((uintptr_t) மதிப்பு & (uintptr_t) ~3);
        }

        திரும்பு மதிப்பு;
    }

    திரும்பு hwc->மதிப்பு;
}


வெறுமை *
ngx_hash_find_wc_tail(ngx_hash_wildcard_t *hwc, u_char *name, size_t len)
{
    வெறுமை        *மதிப்பு;
    ngx_uint_t   i, சாவி;

#எனில் 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "wct:\"%*s\"", len, name);
#endif

    சாவி = 0;

    க்கு (i = 0; i < len; i++) {
        எனில் (name[i] == '.') {
            நிறுத்து;
        }

        சாவி = ngx_hash(சாவி, name[i]);
    }

    எனில் (i == len) {
        திரும்பு வெற்று;
    }

#எனில் 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "key:\"%ui\"", சாவி);
#endif

    மதிப்பு = ngx_hash_find(&hwc->hash, சாவி, name, i);

#எனில் 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "value:\"%p\"", மதிப்பு);
#endif

    எனில் (மதிப்பு) {

        /*
         * the 2 low bits of value have the special meaning:
         *     00 - value is data pointer;
         *     11 - value is pointer to wildcard hash allowing "example.*".
         */

        எனில் ((uintptr_t) மதிப்பு & 2) {

            i++;

            hwc = (ngx_hash_wildcard_t *) ((uintptr_t) மதிப்பு & (uintptr_t) ~3);

            மதிப்பு = ngx_hash_find_wc_tail(hwc, &name[i], len - i);

            எனில் (மதிப்பு) {
                திரும்பு மதிப்பு;
            }

            திரும்பு hwc->மதிப்பு;
        }

        திரும்பு மதிப்பு;
    }

    திரும்பு hwc->மதிப்பு;
}


வெறுமை *
ngx_hash_find_combined(ngx_hash_combined_t *hash, ngx_uint_t சாவி, u_char *name,
    size_t len)
{
    வெறுமை  *மதிப்பு;

    எனில் (hash->hash.buckets) {
        மதிப்பு = ngx_hash_find(&hash->hash, சாவி, name, len);

        எனில் (மதிப்பு) {
            திரும்பு மதிப்பு;
        }
    }

    எனில் (len == 0) {
        திரும்பு வெற்று;
    }

    எனில் (hash->wc_head && hash->wc_head->hash.buckets) {
        மதிப்பு = ngx_hash_find_wc_head(hash->wc_head, name, len);

        எனில் (மதிப்பு) {
            திரும்பு மதிப்பு;
        }
    }

    எனில் (hash->wc_tail && hash->wc_tail->hash.buckets) {
        மதிப்பு = ngx_hash_find_wc_tail(hash->wc_tail, name, len);

        எனில் (மதிப்பு) {
            திரும்பு மதிப்பு;
        }
    }

    திரும்பு வெற்று;
}


#define NGX_HASH_ELT_SIZE(name)                                               \
    (அளவு(வெறுமை *) + ngx_align((name)->சாவி.len + 2, அளவு(வெறுமை *)))

ngx_int_t
ngx_hash_init(ngx_hash_init_t *hinit, ngx_hash_சாவி_t *names, ngx_uint_t nelts)
{
    u_char          *elts;
    size_t           len;
    u_short         *test;
    ngx_uint_t       i, n, சாவி, size, start, bucket_size;
    ngx_hash_elt_t  *elt, **buckets;

    எனில் (hinit->max_size == 0) {
        ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                      "could not build %s, you should "
                      "increase %s_max_size: %i",
                      hinit->name, hinit->name, hinit->max_size);
        திரும்பு NGX_ERROR;
    }

    எனில் (hinit->bucket_size > 65536 - ngx_cacheline_size) {
        ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                      "could not build %s, too large "
                      "%s_bucket_size: %i",
                      hinit->name, hinit->name, hinit->bucket_size);
        திரும்பு NGX_ERROR;
    }

    க்கு (n = 0; n < nelts; n++) {
        எனில் (names[n].சாவி.data == வெற்று) {
            தொடர்;
        }

        எனில் (hinit->bucket_size < NGX_HASH_ELT_SIZE(&names[n]) + அளவு(வெறுமை *))
        {
            ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                          "could not build %s, you should "
                          "increase %s_bucket_size: %i",
                          hinit->name, hinit->name, hinit->bucket_size);
            திரும்பு NGX_ERROR;
        }
    }

    test = ngx_alloc(hinit->max_size * அளவு(u_short), hinit->pool->log);
    எனில் (test == வெற்று) {
        திரும்பு NGX_ERROR;
    }

    bucket_size = hinit->bucket_size - அளவு(வெறுமை *);

    start = nelts / (bucket_size / (2 * அளவு(வெறுமை *)));
    start = start ? start : 1;

    எனில் (hinit->max_size > 10000 && nelts && hinit->max_size / nelts < 100) {
        start = hinit->max_size - 1000;
    }

    க்கு (size = start; size <= hinit->max_size; size++) {

        ngx_memzero(test, size * அளவு(u_short));

        க்கு (n = 0; n < nelts; n++) {
            எனில் (names[n].சாவி.data == வெற்று) {
                தொடர்;
            }

            சாவி = names[n].சாவி_hash % size;
            len = test[சாவி] + NGX_HASH_ELT_SIZE(&names[n]);

#எனில் 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "%ui: %ui %uz \"%V\"",
                          size, சாவி, len, &names[n].சாவி);
#endif

            எனில் (len > bucket_size) {
                செல் next;
            }

            test[சாவி] = (u_short) len;
        }

        செல் found;

    next:

        தொடர்;
    }

    size = hinit->max_size;

    ngx_log_error(NGX_LOG_WARN, hinit->pool->log, 0,
                  "could not build optimal %s, you should increase "
                  "either %s_max_size: %i or %s_bucket_size: %i; "
                  "ignoring %s_bucket_size",
                  hinit->name, hinit->name, hinit->max_size,
                  hinit->name, hinit->bucket_size, hinit->name);

found:

    க்கு (i = 0; i < size; i++) {
        test[i] = அளவு(வெறுமை *);
    }

    க்கு (n = 0; n < nelts; n++) {
        எனில் (names[n].சாவி.data == வெற்று) {
            தொடர்;
        }

        சாவி = names[n].சாவி_hash % size;
        len = test[சாவி] + NGX_HASH_ELT_SIZE(&names[n]);

        எனில் (len > 65536 - ngx_cacheline_size) {
            ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                          "could not build %s, you should "
                          "increase %s_max_size: %i",
                          hinit->name, hinit->name, hinit->max_size);
            ngx_free(test);
            திரும்பு NGX_ERROR;
        }

        test[சாவி] = (u_short) len;
    }

    len = 0;

    க்கு (i = 0; i < size; i++) {
        எனில் (test[i] == அளவு(வெறுமை *)) {
            தொடர்;
        }

        test[i] = (u_short) (ngx_align(test[i], ngx_cacheline_size));

        len += test[i];
    }

    எனில் (hinit->hash == வெற்று) {
        hinit->hash = ngx_pcalloc(hinit->pool, அளவு(ngx_hash_wildcard_t)
                                             + size * அளவு(ngx_hash_elt_t *));
        எனில் (hinit->hash == வெற்று) {
            ngx_free(test);
            திரும்பு NGX_ERROR;
        }

        buckets = (ngx_hash_elt_t **)
                      ((u_char *) hinit->hash + அளவு(ngx_hash_wildcard_t));

    } இல்லையெனில் {
        buckets = ngx_pcalloc(hinit->pool, size * அளவு(ngx_hash_elt_t *));
        எனில் (buckets == வெற்று) {
            ngx_free(test);
            திரும்பு NGX_ERROR;
        }
    }

    elts = ngx_palloc(hinit->pool, len + ngx_cacheline_size);
    எனில் (elts == வெற்று) {
        ngx_free(test);
        திரும்பு NGX_ERROR;
    }

    elts = ngx_align_ptr(elts, ngx_cacheline_size);

    க்கு (i = 0; i < size; i++) {
        எனில் (test[i] == அளவு(வெறுமை *)) {
            தொடர்;
        }

        buckets[i] = (ngx_hash_elt_t *) elts;
        elts += test[i];
    }

    க்கு (i = 0; i < size; i++) {
        test[i] = 0;
    }

    க்கு (n = 0; n < nelts; n++) {
        எனில் (names[n].சாவி.data == வெற்று) {
            தொடர்;
        }

        சாவி = names[n].சாவி_hash % size;
        elt = (ngx_hash_elt_t *) ((u_char *) buckets[சாவி] + test[சாவி]);

        elt->மதிப்பு = names[n].மதிப்பு;
        elt->len = (u_short) names[n].சாவி.len;

        ngx_strlow(elt->name, names[n].சாவி.data, names[n].சாவி.len);

        test[சாவி] = (u_short) (test[சாவி] + NGX_HASH_ELT_SIZE(&names[n]));
    }

    க்கு (i = 0; i < size; i++) {
        எனில் (buckets[i] == வெற்று) {
            தொடர்;
        }

        elt = (ngx_hash_elt_t *) ((u_char *) buckets[i] + test[i]);

        elt->மதிப்பு = வெற்று;
    }

    ngx_free(test);

    hinit->hash->buckets = buckets;
    hinit->hash->size = size;

#எனில் 0

    க்கு (i = 0; i < size; i++) {
        ngx_str_t   val;
        ngx_uint_t  சாவி;

        elt = buckets[i];

        எனில் (elt == வெற்று) {
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "%ui: NULL", i);
            தொடர்;
        }

        போது (elt->மதிப்பு) {
            val.len = elt->len;
            val.data = &elt->name[0];

            சாவி = hinit->சாவி(val.data, val.len);

            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "%ui: %p \"%V\" %ui", i, elt, &val, சாவி);

            elt = (ngx_hash_elt_t *) ngx_align_ptr(&elt->name[0] + elt->len,
                                                   அளவு(வெறுமை *));
        }
    }

#endif

    திரும்பு NGX_OK;
}


ngx_int_t
ngx_hash_wildcard_init(ngx_hash_init_t *hinit, ngx_hash_சாவி_t *names,
    ngx_uint_t nelts)
{
    size_t                len, dot_len;
    ngx_uint_t            i, n, dot;
    ngx_array_t           curr_names, next_names;
    ngx_hash_சாவி_t       *name, *next_name;
    ngx_hash_init_t       h;
    ngx_hash_wildcard_t  *wdc;

    எனில் (ngx_array_init(&curr_names, hinit->temp_pool, nelts,
                       அளவு(ngx_hash_சாவி_t))
        != NGX_OK)
    {
        திரும்பு NGX_ERROR;
    }

    எனில் (ngx_array_init(&next_names, hinit->temp_pool, nelts,
                       அளவு(ngx_hash_சாவி_t))
        != NGX_OK)
    {
        திரும்பு NGX_ERROR;
    }

    க்கு (n = 0; n < nelts; n = i) {

#எனில் 0
        ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                      "wc0: \"%V\"", &names[n].சாவி);
#endif

        dot = 0;

        க்கு (len = 0; len < names[n].சாவி.len; len++) {
            எனில் (names[n].சாவி.data[len] == '.') {
                dot = 1;
                நிறுத்து;
            }
        }

        name = ngx_array_தள்ளு(&curr_names);
        எனில் (name == வெற்று) {
            திரும்பு NGX_ERROR;
        }

        name->சாவி.len = len;
        name->சாவி.data = names[n].சாவி.data;
        name->சாவி_hash = hinit->சாவி(name->சாவி.data, name->சாவி.len);
        name->மதிப்பு = names[n].மதிப்பு;

#எனில் 0
        ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                      "wc1: \"%V\" %ui", &name->சாவி, dot);
#endif

        dot_len = len + 1;

        எனில் (dot) {
            len++;
        }

        next_names.nelts = 0;

        எனில் (names[n].சாவி.len != len) {
            next_name = ngx_array_தள்ளு(&next_names);
            எனில் (next_name == வெற்று) {
                திரும்பு NGX_ERROR;
            }

            next_name->சாவி.len = names[n].சாவி.len - len;
            next_name->சாவி.data = names[n].சாவி.data + len;
            next_name->சாவி_hash = 0;
            next_name->மதிப்பு = names[n].மதிப்பு;

#எனில் 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "wc2: \"%V\"", &next_name->சாவி);
#endif
        }

        க்கு (i = n + 1; i < nelts; i++) {
            எனில் (ngx_strncmp(names[n].சாவி.data, names[i].சாவி.data, len) != 0) {
                நிறுத்து;
            }

            எனில் (!dot
                && names[i].சாவி.len > len
                && names[i].சாவி.data[len] != '.')
            {
                நிறுத்து;
            }

            next_name = ngx_array_தள்ளு(&next_names);
            எனில் (next_name == வெற்று) {
                திரும்பு NGX_ERROR;
            }

            next_name->சாவி.len = names[i].சாவி.len - dot_len;
            next_name->சாவி.data = names[i].சாவி.data + dot_len;
            next_name->சாவி_hash = 0;
            next_name->மதிப்பு = names[i].மதிப்பு;

#எனில் 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "wc3: \"%V\"", &next_name->சாவி);
#endif
        }

        எனில் (next_names.nelts) {

            h = *hinit;
            h.hash = வெற்று;

            எனில் (ngx_hash_wildcard_init(&h, (ngx_hash_சாவி_t *) next_names.elts,
                                       next_names.nelts)
                != NGX_OK)
            {
                திரும்பு NGX_ERROR;
            }

            wdc = (ngx_hash_wildcard_t *) h.hash;

            எனில் (names[n].சாவி.len == len) {
                wdc->மதிப்பு = names[n].மதிப்பு;
            }

            name->மதிப்பு = (வெறுமை *) ((uintptr_t) wdc | (dot ? 3 : 2));

        } இல்லையெனில் எனில் (dot) {
            name->மதிப்பு = (வெறுமை *) ((uintptr_t) name->மதிப்பு | 1);
        }
    }

    எனில் (ngx_hash_init(hinit, (ngx_hash_சாவி_t *) curr_names.elts,
                      curr_names.nelts)
        != NGX_OK)
    {
        திரும்பு NGX_ERROR;
    }

    திரும்பு NGX_OK;
}


ngx_uint_t
ngx_hash_சாவி(u_char *data, size_t len)
{
    ngx_uint_t  i, சாவி;

    சாவி = 0;

    க்கு (i = 0; i < len; i++) {
        சாவி = ngx_hash(சாவி, data[i]);
    }

    திரும்பு சாவி;
}


ngx_uint_t
ngx_hash_சாவி_lc(u_char *data, size_t len)
{
    ngx_uint_t  i, சாவி;

    சாவி = 0;

    க்கு (i = 0; i < len; i++) {
        சாவி = ngx_hash(சாவி, ngx_tolower(data[i]));
    }

    திரும்பு சாவி;
}


ngx_uint_t
ngx_hash_strlow(u_char *dst, u_char *src, size_t n)
{
    ngx_uint_t  சாவி;

    சாவி = 0;

    போது (n--) {
        *dst = ngx_tolower(*src);
        சாவி = ngx_hash(சாவி, *dst);
        dst++;
        src++;
    }

    திரும்பு சாவி;
}


ngx_int_t
ngx_hash_சாவிகள்_array_init(ngx_hash_சாவிகள்_arrays_t *ha, ngx_uint_t type)
{
    ngx_uint_t  asize;

    எனில் (type == NGX_HASH_SMALL) {
        asize = 4;
        ha->hsize = 107;

    } இல்லையெனில் {
        asize = NGX_HASH_LARGE_ASIZE;
        ha->hsize = NGX_HASH_LARGE_HSIZE;
    }

    எனில் (ngx_array_init(&ha->சாவிகள், ha->temp_pool, asize, அளவு(ngx_hash_சாவி_t))
        != NGX_OK)
    {
        திரும்பு NGX_ERROR;
    }

    எனில் (ngx_array_init(&ha->dns_wc_head, ha->temp_pool, asize,
                       அளவு(ngx_hash_சாவி_t))
        != NGX_OK)
    {
        திரும்பு NGX_ERROR;
    }

    எனில் (ngx_array_init(&ha->dns_wc_tail, ha->temp_pool, asize,
                       அளவு(ngx_hash_சாவி_t))
        != NGX_OK)
    {
        திரும்பு NGX_ERROR;
    }

    ha->சாவிகள்_hash = ngx_pcalloc(ha->temp_pool, அளவு(ngx_array_t) * ha->hsize);
    எனில் (ha->சாவிகள்_hash == வெற்று) {
        திரும்பு NGX_ERROR;
    }

    ha->dns_wc_head_hash = ngx_pcalloc(ha->temp_pool,
                                       அளவு(ngx_array_t) * ha->hsize);
    எனில் (ha->dns_wc_head_hash == வெற்று) {
        திரும்பு NGX_ERROR;
    }

    ha->dns_wc_tail_hash = ngx_pcalloc(ha->temp_pool,
                                       அளவு(ngx_array_t) * ha->hsize);
    எனில் (ha->dns_wc_tail_hash == வெற்று) {
        திரும்பு NGX_ERROR;
    }

    திரும்பு NGX_OK;
}


ngx_int_t
ngx_hash_add_சாவி(ngx_hash_சாவிகள்_arrays_t *ha, ngx_str_t *சாவி, வெறுமை *மதிப்பு,
    ngx_uint_t flags)
{
    size_t           len;
    u_char          *p;
    ngx_str_t       *name;
    ngx_uint_t       i, k, n, skip, last;
    ngx_array_t     *சாவிகள், *hwc;
    ngx_hash_சாவி_t  *hk;

    last = சாவி->len;

    எனில் (flags & NGX_HASH_WILDCARD_சாவி) {

        /*
         * supported wildcards:
         *     "*.example.com", ".example.com", and "www.example.*"
         */

        n = 0;

        க்கு (i = 0; i < சாவி->len; i++) {

            எனில் (சாவி->data[i] == '*') {
                எனில் (++n > 1) {
                    திரும்பு NGX_DECLINED;
                }
            }

            எனில் (சாவி->data[i] == '.' && சாவி->data[i + 1] == '.') {
                திரும்பு NGX_DECLINED;
            }

            எனில் (சாவி->data[i] == '\0') {
                திரும்பு NGX_DECLINED;
            }
        }

        எனில் (சாவி->len > 1 && சாவி->data[0] == '.') {
            skip = 1;
            செல் wildcard;
        }

        எனில் (சாவி->len > 2) {

            எனில் (சாவி->data[0] == '*' && சாவி->data[1] == '.') {
                skip = 2;
                செல் wildcard;
            }

            எனில் (சாவி->data[i - 2] == '.' && சாவி->data[i - 1] == '*') {
                skip = 0;
                last -= 2;
                செல் wildcard;
            }
        }

        எனில் (n) {
            திரும்பு NGX_DECLINED;
        }
    }

    /* exact hash */

    k = 0;

    க்கு (i = 0; i < last; i++) {
        எனில் (!(flags & NGX_HASH_READONLY_சாவி)) {
            சாவி->data[i] = ngx_tolower(சாவி->data[i]);
        }
        k = ngx_hash(k, சாவி->data[i]);
    }

    k %= ha->hsize;

    /* check conflicts in exact hash */

    name = ha->சாவிகள்_hash[k].elts;

    எனில் (name) {
        க்கு (i = 0; i < ha->சாவிகள்_hash[k].nelts; i++) {
            எனில் (last != name[i].len) {
                தொடர்;
            }

            எனில் (ngx_strncmp(சாவி->data, name[i].data, last) == 0) {
                திரும்பு NGX_BUSY;
            }
        }

    } இல்லையெனில் {
        எனில் (ngx_array_init(&ha->சாவிகள்_hash[k], ha->temp_pool, 4,
                           அளவு(ngx_str_t))
            != NGX_OK)
        {
            திரும்பு NGX_ERROR;
        }
    }

    name = ngx_array_தள்ளு(&ha->சாவிகள்_hash[k]);
    எனில் (name == வெற்று) {
        திரும்பு NGX_ERROR;
    }

    *name = *சாவி;

    hk = ngx_array_தள்ளு(&ha->சாவிகள்);
    எனில் (hk == வெற்று) {
        திரும்பு NGX_ERROR;
    }

    hk->சாவி = *சாவி;
    hk->சாவி_hash = ngx_hash_சாவி(சாவி->data, last);
    hk->மதிப்பு = மதிப்பு;

    திரும்பு NGX_OK;


wildcard:

    /* wildcard hash */

    k = ngx_hash_strlow(&சாவி->data[skip], &சாவி->data[skip], last - skip);

    k %= ha->hsize;

    எனில் (skip == 1) {

        /* check conflicts in exact hash for ".example.com" */

        name = ha->சாவிகள்_hash[k].elts;

        எனில் (name) {
            len = last - skip;

            க்கு (i = 0; i < ha->சாவிகள்_hash[k].nelts; i++) {
                எனில் (len != name[i].len) {
                    தொடர்;
                }

                எனில் (ngx_strncmp(&சாவி->data[1], name[i].data, len) == 0) {
                    திரும்பு NGX_BUSY;
                }
            }

        } இல்லையெனில் {
            எனில் (ngx_array_init(&ha->சாவிகள்_hash[k], ha->temp_pool, 4,
                               அளவு(ngx_str_t))
                != NGX_OK)
            {
                திரும்பு NGX_ERROR;
            }
        }

        name = ngx_array_தள்ளு(&ha->சாவிகள்_hash[k]);
        எனில் (name == வெற்று) {
            திரும்பு NGX_ERROR;
        }

        name->len = last - 1;
        name->data = ngx_pnalloc(ha->temp_pool, name->len);
        எனில் (name->data == வெற்று) {
            திரும்பு NGX_ERROR;
        }

        ngx_memcpy(name->data, &சாவி->data[1], name->len);
    }


    எனில் (skip) {

        /*
         * convert "*.example.com" to "com.example.\0"
         *      and ".example.com" to "com.example\0"
         */

        p = ngx_pnalloc(ha->temp_pool, last);
        எனில் (p == வெற்று) {
            திரும்பு NGX_ERROR;
        }

        len = 0;
        n = 0;

        க்கு (i = last - 1; i; i--) {
            எனில் (சாவி->data[i] == '.') {
                ngx_memcpy(&p[n], &சாவி->data[i + 1], len);
                n += len;
                p[n++] = '.';
                len = 0;
                தொடர்;
            }

            len++;
        }

        எனில் (len) {
            ngx_memcpy(&p[n], &சாவி->data[1], len);
            n += len;
        }

        p[n] = '\0';

        hwc = &ha->dns_wc_head;
        சாவிகள் = &ha->dns_wc_head_hash[k];

    } இல்லையெனில் {

        /* convert "www.example.*" to "www.example\0" */

        last++;

        p = ngx_pnalloc(ha->temp_pool, last);
        எனில் (p == வெற்று) {
            திரும்பு NGX_ERROR;
        }

        ngx_cpystrn(p, சாவி->data, last);

        hwc = &ha->dns_wc_tail;
        சாவிகள் = &ha->dns_wc_tail_hash[k];
    }


    /* check conflicts in wildcard hash */

    name = சாவிகள்->elts;

    எனில் (name) {
        len = last - skip;

        க்கு (i = 0; i < சாவிகள்->nelts; i++) {
            எனில் (len != name[i].len) {
                தொடர்;
            }

            எனில் (ngx_strncmp(சாவி->data + skip, name[i].data, len) == 0) {
                திரும்பு NGX_BUSY;
            }
        }

    } இல்லையெனில் {
        எனில் (ngx_array_init(சாவிகள், ha->temp_pool, 4, அளவு(ngx_str_t)) != NGX_OK)
        {
            திரும்பு NGX_ERROR;
        }
    }

    name = ngx_array_தள்ளு(சாவிகள்);
    எனில் (name == வெற்று) {
        திரும்பு NGX_ERROR;
    }

    name->len = last - skip;
    name->data = ngx_pnalloc(ha->temp_pool, name->len);
    எனில் (name->data == வெற்று) {
        திரும்பு NGX_ERROR;
    }

    ngx_memcpy(name->data, சாவி->data + skip, name->len);


    /* add to wildcard hash */

    hk = ngx_array_தள்ளு(hwc);
    எனில் (hk == வெற்று) {
        திரும்பு NGX_ERROR;
    }

    hk->சாவி.len = last - 1;
    hk->சாவி.data = p;
    hk->சாவி_hash = 0;
    hk->மதிப்பு = மதிப்பு;

    திரும்பு NGX_OK;
}
