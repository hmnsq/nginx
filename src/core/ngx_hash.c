
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ଅନ୍ତର୍ଭାଵ <ngx_config.h>
#ଅନ୍ତର୍ଭାଵ <ngx_core.h>


ଶୂନ୍ୟ *
ngx_hash_find(ngx_hash_t *hash, ngx_uint_t ଚାବି, u_char *name, size_t len)
{
    ngx_uint_t       i;
    ngx_hash_elt_t  *elt;

#ଯଦି 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "hf:\"%*s\"", len, name);
#endif

    elt = hash->buckets[ଚାବି % hash->size];

    ଯଦି (elt == ଖାଲି) {
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ଯେତେବେଳେ (elt->ମୂଲ୍ୟ) {
        ଯଦି (len != (size_t) elt->len) {
            ଯାଅ next;
        }

        ପାଇଁ (i = 0; i < len; i++) {
            ଯଦି (name[i] != elt->name[i]) {
                ଯାଅ next;
            }
        }

        ଫେରନ୍ତୁ elt->ମୂଲ୍ୟ;

    next:

        elt = (ngx_hash_elt_t *) ngx_align_ptr(&elt->name[0] + elt->len,
                                               ଆକାର(ଶୂନ୍ୟ *));
        ଜାରି;
    }

    ଫେରନ୍ତୁ ଖାଲି;
}


ଶୂନ୍ୟ *
ngx_hash_find_wc_head(ngx_hash_wildcard_t *hwc, u_char *name, size_t len)
{
    ଶୂନ୍ୟ        *ମୂଲ୍ୟ;
    ngx_uint_t   i, n, ଚାବି;

#ଯଦି 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "wch:\"%*s\"", len, name);
#endif

    n = len;

    ଯେତେବେଳେ (n) {
        ଯଦି (name[n - 1] == '.') {
            ଭାଙ୍ଗ;
        }

        n--;
    }

    ଚାବି = 0;

    ପାଇଁ (i = n; i < len; i++) {
        ଚାବି = ngx_hash(ଚାବି, name[i]);
    }

#ଯଦି 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "key:\"%ui\"", ଚାବି);
#endif

    ମୂଲ୍ୟ = ngx_hash_find(&hwc->hash, ଚାବି, &name[n], len - n);

#ଯଦି 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "value:\"%p\"", ମୂଲ୍ୟ);
#endif

    ଯଦି (ମୂଲ୍ୟ) {

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

        ଯଦି ((uintptr_t) ମୂଲ୍ୟ & 2) {

            ଯଦି (n == 0) {

                /* "example.com" */

                ଯଦି ((uintptr_t) ମୂଲ୍ୟ & 1) {
                    ଫେରନ୍ତୁ ଖାଲି;
                }

                hwc = (ngx_hash_wildcard_t *)
                                          ((uintptr_t) ମୂଲ୍ୟ & (uintptr_t) ~3);
                ଫେରନ୍ତୁ hwc->ମୂଲ୍ୟ;
            }

            hwc = (ngx_hash_wildcard_t *) ((uintptr_t) ମୂଲ୍ୟ & (uintptr_t) ~3);

            ମୂଲ୍ୟ = ngx_hash_find_wc_head(hwc, name, n - 1);

            ଯଦି (ମୂଲ୍ୟ) {
                ଫେରନ୍ତୁ ମୂଲ୍ୟ;
            }

            ଫେରନ୍ତୁ hwc->ମୂଲ୍ୟ;
        }

        ଯଦି ((uintptr_t) ମୂଲ୍ୟ & 1) {

            ଯଦି (n == 0) {

                /* "example.com" */

                ଫେରନ୍ତୁ ଖାଲି;
            }

            ଫେରନ୍ତୁ (ଶୂନ୍ୟ *) ((uintptr_t) ମୂଲ୍ୟ & (uintptr_t) ~3);
        }

        ଫେରନ୍ତୁ ମୂଲ୍ୟ;
    }

    ଫେରନ୍ତୁ hwc->ମୂଲ୍ୟ;
}


ଶୂନ୍ୟ *
ngx_hash_find_wc_tail(ngx_hash_wildcard_t *hwc, u_char *name, size_t len)
{
    ଶୂନ୍ୟ        *ମୂଲ୍ୟ;
    ngx_uint_t   i, ଚାବି;

#ଯଦି 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "wct:\"%*s\"", len, name);
#endif

    ଚାବି = 0;

    ପାଇଁ (i = 0; i < len; i++) {
        ଯଦି (name[i] == '.') {
            ଭାଙ୍ଗ;
        }

        ଚାବି = ngx_hash(ଚାବି, name[i]);
    }

    ଯଦି (i == len) {
        ଫେରନ୍ତୁ ଖାଲି;
    }

#ଯଦି 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "key:\"%ui\"", ଚାବି);
#endif

    ମୂଲ୍ୟ = ngx_hash_find(&hwc->hash, ଚାବି, name, i);

#ଯଦି 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "value:\"%p\"", ମୂଲ୍ୟ);
#endif

    ଯଦି (ମୂଲ୍ୟ) {

        /*
         * the 2 low bits of value have the special meaning:
         *     00 - value is data pointer;
         *     11 - value is pointer to wildcard hash allowing "example.*".
         */

        ଯଦି ((uintptr_t) ମୂଲ୍ୟ & 2) {

            i++;

            hwc = (ngx_hash_wildcard_t *) ((uintptr_t) ମୂଲ୍ୟ & (uintptr_t) ~3);

            ମୂଲ୍ୟ = ngx_hash_find_wc_tail(hwc, &name[i], len - i);

            ଯଦି (ମୂଲ୍ୟ) {
                ଫେରନ୍ତୁ ମୂଲ୍ୟ;
            }

            ଫେରନ୍ତୁ hwc->ମୂଲ୍ୟ;
        }

        ଫେରନ୍ତୁ ମୂଲ୍ୟ;
    }

    ଫେରନ୍ତୁ hwc->ମୂଲ୍ୟ;
}


ଶୂନ୍ୟ *
ngx_hash_find_combined(ngx_hash_combined_t *hash, ngx_uint_t ଚାବି, u_char *name,
    size_t len)
{
    ଶୂନ୍ୟ  *ମୂଲ୍ୟ;

    ଯଦି (hash->hash.buckets) {
        ମୂଲ୍ୟ = ngx_hash_find(&hash->hash, ଚାବି, name, len);

        ଯଦି (ମୂଲ୍ୟ) {
            ଫେରନ୍ତୁ ମୂଲ୍ୟ;
        }
    }

    ଯଦି (len == 0) {
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ଯଦି (hash->wc_head && hash->wc_head->hash.buckets) {
        ମୂଲ୍ୟ = ngx_hash_find_wc_head(hash->wc_head, name, len);

        ଯଦି (ମୂଲ୍ୟ) {
            ଫେରନ୍ତୁ ମୂଲ୍ୟ;
        }
    }

    ଯଦି (hash->wc_tail && hash->wc_tail->hash.buckets) {
        ମୂଲ୍ୟ = ngx_hash_find_wc_tail(hash->wc_tail, name, len);

        ଯଦି (ମୂଲ୍ୟ) {
            ଫେରନ୍ତୁ ମୂଲ୍ୟ;
        }
    }

    ଫେରନ୍ତୁ ଖାଲି;
}


#define NGX_HASH_ELT_SIZE(name)                                               \
    (ଆକାର(ଶୂନ୍ୟ *) + ngx_align((name)->ଚାବି.len + 2, ଆକାର(ଶୂନ୍ୟ *)))

ngx_int_t
ngx_hash_init(ngx_hash_init_t *hinit, ngx_hash_ଚାବି_t *names, ngx_uint_t nelts)
{
    u_char          *elts;
    size_t           len;
    u_short         *test;
    ngx_uint_t       i, n, ଚାବି, size, start, bucket_size;
    ngx_hash_elt_t  *elt, **buckets;

    ଯଦି (hinit->max_size == 0) {
        ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                      "could not build %s, you should "
                      "increase %s_max_size: %i",
                      hinit->name, hinit->name, hinit->max_size);
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    ଯଦି (hinit->bucket_size > 65536 - ngx_cacheline_size) {
        ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                      "could not build %s, too large "
                      "%s_bucket_size: %i",
                      hinit->name, hinit->name, hinit->bucket_size);
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    ପାଇଁ (n = 0; n < nelts; n++) {
        ଯଦି (names[n].ଚାବି.data == ଖାଲି) {
            ଜାରି;
        }

        ଯଦି (hinit->bucket_size < NGX_HASH_ELT_SIZE(&names[n]) + ଆକାର(ଶୂନ୍ୟ *))
        {
            ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                          "could not build %s, you should "
                          "increase %s_bucket_size: %i",
                          hinit->name, hinit->name, hinit->bucket_size);
            ଫେରନ୍ତୁ NGX_ERROR;
        }
    }

    test = ngx_alloc(hinit->max_size * ଆକାର(u_short), hinit->pool->log);
    ଯଦି (test == ଖାଲି) {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    bucket_size = hinit->bucket_size - ଆକାର(ଶୂନ୍ୟ *);

    start = nelts / (bucket_size / (2 * ଆକାର(ଶୂନ୍ୟ *)));
    start = start ? start : 1;

    ଯଦି (hinit->max_size > 10000 && nelts && hinit->max_size / nelts < 100) {
        start = hinit->max_size - 1000;
    }

    ପାଇଁ (size = start; size <= hinit->max_size; size++) {

        ngx_memzero(test, size * ଆକାର(u_short));

        ପାଇଁ (n = 0; n < nelts; n++) {
            ଯଦି (names[n].ଚାବି.data == ଖାଲି) {
                ଜାରି;
            }

            ଚାବି = names[n].ଚାବି_hash % size;
            len = test[ଚାବି] + NGX_HASH_ELT_SIZE(&names[n]);

#ଯଦି 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "%ui: %ui %uz \"%V\"",
                          size, ଚାବି, len, &names[n].ଚାବି);
#endif

            ଯଦି (len > bucket_size) {
                ଯାଅ next;
            }

            test[ଚାବି] = (u_short) len;
        }

        ଯାଅ found;

    next:

        ଜାରି;
    }

    size = hinit->max_size;

    ngx_log_error(NGX_LOG_WARN, hinit->pool->log, 0,
                  "could not build optimal %s, you should increase "
                  "either %s_max_size: %i or %s_bucket_size: %i; "
                  "ignoring %s_bucket_size",
                  hinit->name, hinit->name, hinit->max_size,
                  hinit->name, hinit->bucket_size, hinit->name);

found:

    ପାଇଁ (i = 0; i < size; i++) {
        test[i] = ଆକାର(ଶୂନ୍ୟ *);
    }

    ପାଇଁ (n = 0; n < nelts; n++) {
        ଯଦି (names[n].ଚାବି.data == ଖାଲି) {
            ଜାରି;
        }

        ଚାବି = names[n].ଚାବି_hash % size;
        len = test[ଚାବି] + NGX_HASH_ELT_SIZE(&names[n]);

        ଯଦି (len > 65536 - ngx_cacheline_size) {
            ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                          "could not build %s, you should "
                          "increase %s_max_size: %i",
                          hinit->name, hinit->name, hinit->max_size);
            ngx_free(test);
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        test[ଚାବି] = (u_short) len;
    }

    len = 0;

    ପାଇଁ (i = 0; i < size; i++) {
        ଯଦି (test[i] == ଆକାର(ଶୂନ୍ୟ *)) {
            ଜାରି;
        }

        test[i] = (u_short) (ngx_align(test[i], ngx_cacheline_size));

        len += test[i];
    }

    ଯଦି (hinit->hash == ଖାଲି) {
        hinit->hash = ngx_pcalloc(hinit->pool, ଆକାର(ngx_hash_wildcard_t)
                                             + size * ଆକାର(ngx_hash_elt_t *));
        ଯଦି (hinit->hash == ଖାଲି) {
            ngx_free(test);
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        buckets = (ngx_hash_elt_t **)
                      ((u_char *) hinit->hash + ଆକାର(ngx_hash_wildcard_t));

    } ଅନ୍ୟଥା {
        buckets = ngx_pcalloc(hinit->pool, size * ଆକାର(ngx_hash_elt_t *));
        ଯଦି (buckets == ଖାଲି) {
            ngx_free(test);
            ଫେରନ୍ତୁ NGX_ERROR;
        }
    }

    elts = ngx_palloc(hinit->pool, len + ngx_cacheline_size);
    ଯଦି (elts == ଖାଲି) {
        ngx_free(test);
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    elts = ngx_align_ptr(elts, ngx_cacheline_size);

    ପାଇଁ (i = 0; i < size; i++) {
        ଯଦି (test[i] == ଆକାର(ଶୂନ୍ୟ *)) {
            ଜାରି;
        }

        buckets[i] = (ngx_hash_elt_t *) elts;
        elts += test[i];
    }

    ପାଇଁ (i = 0; i < size; i++) {
        test[i] = 0;
    }

    ପାଇଁ (n = 0; n < nelts; n++) {
        ଯଦି (names[n].ଚାବି.data == ଖାଲି) {
            ଜାରି;
        }

        ଚାବି = names[n].ଚାବି_hash % size;
        elt = (ngx_hash_elt_t *) ((u_char *) buckets[ଚାବି] + test[ଚାବି]);

        elt->ମୂଲ୍ୟ = names[n].ମୂଲ୍ୟ;
        elt->len = (u_short) names[n].ଚାବି.len;

        ngx_strlow(elt->name, names[n].ଚାବି.data, names[n].ଚାବି.len);

        test[ଚାବି] = (u_short) (test[ଚାବି] + NGX_HASH_ELT_SIZE(&names[n]));
    }

    ପାଇଁ (i = 0; i < size; i++) {
        ଯଦି (buckets[i] == ଖାଲି) {
            ଜାରି;
        }

        elt = (ngx_hash_elt_t *) ((u_char *) buckets[i] + test[i]);

        elt->ମୂଲ୍ୟ = ଖାଲି;
    }

    ngx_free(test);

    hinit->hash->buckets = buckets;
    hinit->hash->size = size;

#ଯଦି 0

    ପାଇଁ (i = 0; i < size; i++) {
        ngx_str_t   val;
        ngx_uint_t  ଚାବି;

        elt = buckets[i];

        ଯଦି (elt == ଖାଲି) {
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "%ui: NULL", i);
            ଜାରି;
        }

        ଯେତେବେଳେ (elt->ମୂଲ୍ୟ) {
            val.len = elt->len;
            val.data = &elt->name[0];

            ଚାବି = hinit->ଚାବି(val.data, val.len);

            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "%ui: %p \"%V\" %ui", i, elt, &val, ଚାବି);

            elt = (ngx_hash_elt_t *) ngx_align_ptr(&elt->name[0] + elt->len,
                                                   ଆକାର(ଶୂନ୍ୟ *));
        }
    }

#endif

    ଫେରନ୍ତୁ NGX_OK;
}


ngx_int_t
ngx_hash_wildcard_init(ngx_hash_init_t *hinit, ngx_hash_ଚାବି_t *names,
    ngx_uint_t nelts)
{
    size_t                len, dot_len;
    ngx_uint_t            i, n, dot;
    ngx_array_t           curr_names, next_names;
    ngx_hash_ଚାବି_t       *name, *next_name;
    ngx_hash_init_t       h;
    ngx_hash_wildcard_t  *wdc;

    ଯଦି (ngx_array_init(&curr_names, hinit->temp_pool, nelts,
                       ଆକାର(ngx_hash_ଚାବି_t))
        != NGX_OK)
    {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    ଯଦି (ngx_array_init(&next_names, hinit->temp_pool, nelts,
                       ଆକାର(ngx_hash_ଚାବି_t))
        != NGX_OK)
    {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    ପାଇଁ (n = 0; n < nelts; n = i) {

#ଯଦି 0
        ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                      "wc0: \"%V\"", &names[n].ଚାବି);
#endif

        dot = 0;

        ପାଇଁ (len = 0; len < names[n].ଚାବି.len; len++) {
            ଯଦି (names[n].ଚାବି.data[len] == '.') {
                dot = 1;
                ଭାଙ୍ଗ;
            }
        }

        name = ngx_array_ଠେଲ(&curr_names);
        ଯଦି (name == ଖାଲି) {
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        name->ଚାବି.len = len;
        name->ଚାବି.data = names[n].ଚାବି.data;
        name->ଚାବି_hash = hinit->ଚାବି(name->ଚାବି.data, name->ଚାବି.len);
        name->ମୂଲ୍ୟ = names[n].ମୂଲ୍ୟ;

#ଯଦି 0
        ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                      "wc1: \"%V\" %ui", &name->ଚାବି, dot);
#endif

        dot_len = len + 1;

        ଯଦି (dot) {
            len++;
        }

        next_names.nelts = 0;

        ଯଦି (names[n].ଚାବି.len != len) {
            next_name = ngx_array_ଠେଲ(&next_names);
            ଯଦି (next_name == ଖାଲି) {
                ଫେରନ୍ତୁ NGX_ERROR;
            }

            next_name->ଚାବି.len = names[n].ଚାବି.len - len;
            next_name->ଚାବି.data = names[n].ଚାବି.data + len;
            next_name->ଚାବି_hash = 0;
            next_name->ମୂଲ୍ୟ = names[n].ମୂଲ୍ୟ;

#ଯଦି 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "wc2: \"%V\"", &next_name->ଚାବି);
#endif
        }

        ପାଇଁ (i = n + 1; i < nelts; i++) {
            ଯଦି (ngx_strncmp(names[n].ଚାବି.data, names[i].ଚାବି.data, len) != 0) {
                ଭାଙ୍ଗ;
            }

            ଯଦି (!dot
                && names[i].ଚାବି.len > len
                && names[i].ଚାବି.data[len] != '.')
            {
                ଭାଙ୍ଗ;
            }

            next_name = ngx_array_ଠେଲ(&next_names);
            ଯଦି (next_name == ଖାଲି) {
                ଫେରନ୍ତୁ NGX_ERROR;
            }

            next_name->ଚାବି.len = names[i].ଚାବି.len - dot_len;
            next_name->ଚାବି.data = names[i].ଚାବି.data + dot_len;
            next_name->ଚାବି_hash = 0;
            next_name->ମୂଲ୍ୟ = names[i].ମୂଲ୍ୟ;

#ଯଦି 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "wc3: \"%V\"", &next_name->ଚାବି);
#endif
        }

        ଯଦି (next_names.nelts) {

            h = *hinit;
            h.hash = ଖାଲି;

            ଯଦି (ngx_hash_wildcard_init(&h, (ngx_hash_ଚାବି_t *) next_names.elts,
                                       next_names.nelts)
                != NGX_OK)
            {
                ଫେରନ୍ତୁ NGX_ERROR;
            }

            wdc = (ngx_hash_wildcard_t *) h.hash;

            ଯଦି (names[n].ଚାବି.len == len) {
                wdc->ମୂଲ୍ୟ = names[n].ମୂଲ୍ୟ;
            }

            name->ମୂଲ୍ୟ = (ଶୂନ୍ୟ *) ((uintptr_t) wdc | (dot ? 3 : 2));

        } ଅନ୍ୟଥା ଯଦି (dot) {
            name->ମୂଲ୍ୟ = (ଶୂନ୍ୟ *) ((uintptr_t) name->ମୂଲ୍ୟ | 1);
        }
    }

    ଯଦି (ngx_hash_init(hinit, (ngx_hash_ଚାବି_t *) curr_names.elts,
                      curr_names.nelts)
        != NGX_OK)
    {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    ଫେରନ୍ତୁ NGX_OK;
}


ngx_uint_t
ngx_hash_ଚାବି(u_char *data, size_t len)
{
    ngx_uint_t  i, ଚାବି;

    ଚାବି = 0;

    ପାଇଁ (i = 0; i < len; i++) {
        ଚାବି = ngx_hash(ଚାବି, data[i]);
    }

    ଫେରନ୍ତୁ ଚାବି;
}


ngx_uint_t
ngx_hash_ଚାବି_lc(u_char *data, size_t len)
{
    ngx_uint_t  i, ଚାବି;

    ଚାବି = 0;

    ପାଇଁ (i = 0; i < len; i++) {
        ଚାବି = ngx_hash(ଚାବି, ngx_tolower(data[i]));
    }

    ଫେରନ୍ତୁ ଚାବି;
}


ngx_uint_t
ngx_hash_strlow(u_char *dst, u_char *src, size_t n)
{
    ngx_uint_t  ଚାବି;

    ଚାବି = 0;

    ଯେତେବେଳେ (n--) {
        *dst = ngx_tolower(*src);
        ଚାବି = ngx_hash(ଚାବି, *dst);
        dst++;
        src++;
    }

    ଫେରନ୍ତୁ ଚାବି;
}


ngx_int_t
ngx_hash_ଚାବିଗୁଡ଼ିକ_array_init(ngx_hash_ଚାବିଗୁଡ଼ିକ_arrays_t *ha, ngx_uint_t type)
{
    ngx_uint_t  asize;

    ଯଦି (type == NGX_HASH_SMALL) {
        asize = 4;
        ha->hsize = 107;

    } ଅନ୍ୟଥା {
        asize = NGX_HASH_LARGE_ASIZE;
        ha->hsize = NGX_HASH_LARGE_HSIZE;
    }

    ଯଦି (ngx_array_init(&ha->ଚାବିଗୁଡ଼ିକ, ha->temp_pool, asize, ଆକାର(ngx_hash_ଚାବି_t))
        != NGX_OK)
    {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    ଯଦି (ngx_array_init(&ha->dns_wc_head, ha->temp_pool, asize,
                       ଆକାର(ngx_hash_ଚାବି_t))
        != NGX_OK)
    {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    ଯଦି (ngx_array_init(&ha->dns_wc_tail, ha->temp_pool, asize,
                       ଆକାର(ngx_hash_ଚାବି_t))
        != NGX_OK)
    {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    ha->ଚାବିଗୁଡ଼ିକ_hash = ngx_pcalloc(ha->temp_pool, ଆକାର(ngx_array_t) * ha->hsize);
    ଯଦି (ha->ଚାବିଗୁଡ଼ିକ_hash == ଖାଲି) {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    ha->dns_wc_head_hash = ngx_pcalloc(ha->temp_pool,
                                       ଆକାର(ngx_array_t) * ha->hsize);
    ଯଦି (ha->dns_wc_head_hash == ଖାଲି) {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    ha->dns_wc_tail_hash = ngx_pcalloc(ha->temp_pool,
                                       ଆକାର(ngx_array_t) * ha->hsize);
    ଯଦି (ha->dns_wc_tail_hash == ଖାଲି) {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    ଫେରନ୍ତୁ NGX_OK;
}


ngx_int_t
ngx_hash_add_ଚାବି(ngx_hash_ଚାବିଗୁଡ଼ିକ_arrays_t *ha, ngx_str_t *ଚାବି, ଶୂନ୍ୟ *ମୂଲ୍ୟ,
    ngx_uint_t flags)
{
    size_t           len;
    u_char          *p;
    ngx_str_t       *name;
    ngx_uint_t       i, k, n, skip, last;
    ngx_array_t     *ଚାବିଗୁଡ଼ିକ, *hwc;
    ngx_hash_ଚାବି_t  *hk;

    last = ଚାବି->len;

    ଯଦି (flags & NGX_HASH_WILDCARD_ଚାବି) {

        /*
         * supported wildcards:
         *     "*.example.com", ".example.com", and "www.example.*"
         */

        n = 0;

        ପାଇଁ (i = 0; i < ଚାବି->len; i++) {

            ଯଦି (ଚାବି->data[i] == '*') {
                ଯଦି (++n > 1) {
                    ଫେରନ୍ତୁ NGX_DECLINED;
                }
            }

            ଯଦି (ଚାବି->data[i] == '.' && ଚାବି->data[i + 1] == '.') {
                ଫେରନ୍ତୁ NGX_DECLINED;
            }

            ଯଦି (ଚାବି->data[i] == '\0') {
                ଫେରନ୍ତୁ NGX_DECLINED;
            }
        }

        ଯଦି (ଚାବି->len > 1 && ଚାବି->data[0] == '.') {
            skip = 1;
            ଯାଅ wildcard;
        }

        ଯଦି (ଚାବି->len > 2) {

            ଯଦି (ଚାବି->data[0] == '*' && ଚାବି->data[1] == '.') {
                skip = 2;
                ଯାଅ wildcard;
            }

            ଯଦି (ଚାବି->data[i - 2] == '.' && ଚାବି->data[i - 1] == '*') {
                skip = 0;
                last -= 2;
                ଯାଅ wildcard;
            }
        }

        ଯଦି (n) {
            ଫେରନ୍ତୁ NGX_DECLINED;
        }
    }

    /* exact hash */

    k = 0;

    ପାଇଁ (i = 0; i < last; i++) {
        ଯଦି (!(flags & NGX_HASH_READONLY_ଚାବି)) {
            ଚାବି->data[i] = ngx_tolower(ଚାବି->data[i]);
        }
        k = ngx_hash(k, ଚାବି->data[i]);
    }

    k %= ha->hsize;

    /* check conflicts in exact hash */

    name = ha->ଚାବିଗୁଡ଼ିକ_hash[k].elts;

    ଯଦି (name) {
        ପାଇଁ (i = 0; i < ha->ଚାବିଗୁଡ଼ିକ_hash[k].nelts; i++) {
            ଯଦି (last != name[i].len) {
                ଜାରି;
            }

            ଯଦି (ngx_strncmp(ଚାବି->data, name[i].data, last) == 0) {
                ଫେରନ୍ତୁ NGX_BUSY;
            }
        }

    } ଅନ୍ୟଥା {
        ଯଦି (ngx_array_init(&ha->ଚାବିଗୁଡ଼ିକ_hash[k], ha->temp_pool, 4,
                           ଆକାର(ngx_str_t))
            != NGX_OK)
        {
            ଫେରନ୍ତୁ NGX_ERROR;
        }
    }

    name = ngx_array_ଠେଲ(&ha->ଚାବିଗୁଡ଼ିକ_hash[k]);
    ଯଦି (name == ଖାଲି) {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    *name = *ଚାବି;

    hk = ngx_array_ଠେଲ(&ha->ଚାବିଗୁଡ଼ିକ);
    ଯଦି (hk == ଖାଲି) {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    hk->ଚାବି = *ଚାବି;
    hk->ଚାବି_hash = ngx_hash_ଚାବି(ଚାବି->data, last);
    hk->ମୂଲ୍ୟ = ମୂଲ୍ୟ;

    ଫେରନ୍ତୁ NGX_OK;


wildcard:

    /* wildcard hash */

    k = ngx_hash_strlow(&ଚାବି->data[skip], &ଚାବି->data[skip], last - skip);

    k %= ha->hsize;

    ଯଦି (skip == 1) {

        /* check conflicts in exact hash for ".example.com" */

        name = ha->ଚାବିଗୁଡ଼ିକ_hash[k].elts;

        ଯଦି (name) {
            len = last - skip;

            ପାଇଁ (i = 0; i < ha->ଚାବିଗୁଡ଼ିକ_hash[k].nelts; i++) {
                ଯଦି (len != name[i].len) {
                    ଜାରି;
                }

                ଯଦି (ngx_strncmp(&ଚାବି->data[1], name[i].data, len) == 0) {
                    ଫେରନ୍ତୁ NGX_BUSY;
                }
            }

        } ଅନ୍ୟଥା {
            ଯଦି (ngx_array_init(&ha->ଚାବିଗୁଡ଼ିକ_hash[k], ha->temp_pool, 4,
                               ଆକାର(ngx_str_t))
                != NGX_OK)
            {
                ଫେରନ୍ତୁ NGX_ERROR;
            }
        }

        name = ngx_array_ଠେଲ(&ha->ଚାବିଗୁଡ଼ିକ_hash[k]);
        ଯଦି (name == ଖାଲି) {
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        name->len = last - 1;
        name->data = ngx_pnalloc(ha->temp_pool, name->len);
        ଯଦି (name->data == ଖାଲି) {
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        ngx_memcpy(name->data, &ଚାବି->data[1], name->len);
    }


    ଯଦି (skip) {

        /*
         * convert "*.example.com" to "com.example.\0"
         *      and ".example.com" to "com.example\0"
         */

        p = ngx_pnalloc(ha->temp_pool, last);
        ଯଦି (p == ଖାଲି) {
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        len = 0;
        n = 0;

        ପାଇଁ (i = last - 1; i; i--) {
            ଯଦି (ଚାବି->data[i] == '.') {
                ngx_memcpy(&p[n], &ଚାବି->data[i + 1], len);
                n += len;
                p[n++] = '.';
                len = 0;
                ଜାରି;
            }

            len++;
        }

        ଯଦି (len) {
            ngx_memcpy(&p[n], &ଚାବି->data[1], len);
            n += len;
        }

        p[n] = '\0';

        hwc = &ha->dns_wc_head;
        ଚାବିଗୁଡ଼ିକ = &ha->dns_wc_head_hash[k];

    } ଅନ୍ୟଥା {

        /* convert "www.example.*" to "www.example\0" */

        last++;

        p = ngx_pnalloc(ha->temp_pool, last);
        ଯଦି (p == ଖାଲି) {
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        ngx_cpystrn(p, ଚାବି->data, last);

        hwc = &ha->dns_wc_tail;
        ଚାବିଗୁଡ଼ିକ = &ha->dns_wc_tail_hash[k];
    }


    /* check conflicts in wildcard hash */

    name = ଚାବିଗୁଡ଼ିକ->elts;

    ଯଦି (name) {
        len = last - skip;

        ପାଇଁ (i = 0; i < ଚାବିଗୁଡ଼ିକ->nelts; i++) {
            ଯଦି (len != name[i].len) {
                ଜାରି;
            }

            ଯଦି (ngx_strncmp(ଚାବି->data + skip, name[i].data, len) == 0) {
                ଫେରନ୍ତୁ NGX_BUSY;
            }
        }

    } ଅନ୍ୟଥା {
        ଯଦି (ngx_array_init(ଚାବିଗୁଡ଼ିକ, ha->temp_pool, 4, ଆକାର(ngx_str_t)) != NGX_OK)
        {
            ଫେରନ୍ତୁ NGX_ERROR;
        }
    }

    name = ngx_array_ଠେଲ(ଚାବିଗୁଡ଼ିକ);
    ଯଦି (name == ଖାଲି) {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    name->len = last - skip;
    name->data = ngx_pnalloc(ha->temp_pool, name->len);
    ଯଦି (name->data == ଖାଲି) {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    ngx_memcpy(name->data, ଚାବି->data + skip, name->len);


    /* add to wildcard hash */

    hk = ngx_array_ଠେଲ(hwc);
    ଯଦି (hk == ଖାଲି) {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    hk->ଚାବି.len = last - 1;
    hk->ଚାବି.data = p;
    hk->ଚାବି_hash = 0;
    hk->ମୂଲ୍ୟ = ମୂଲ୍ୟ;

    ଫେରନ୍ତୁ NGX_OK;
}
