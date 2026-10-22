
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#അന്തര്ഭാവ <ngx_config.h>
#അന്തര്ഭാവ <ngx_core.h>


ശൂന്യം *
ngx_hash_find(ngx_hash_t *hash, ngx_uint_t താക്കോൽ, u_char *name, size_t len)
{
    ngx_uint_t       i;
    ngx_hash_elt_t  *elt;

#എങ്കിൽ 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "hf:\"%*s\"", len, name);
#endif

    elt = hash->buckets[താക്കോൽ % hash->size];

    എങ്കിൽ (elt == ശൂന്യ) {
        തിരികെ ശൂന്യ;
    }

    ആകുമ്പോൾ (elt->മൂല്യം) {
        എങ്കിൽ (len != (size_t) elt->len) {
            പോകുക next;
        }

        വേണ്ടി (i = 0; i < len; i++) {
            എങ്കിൽ (name[i] != elt->name[i]) {
                പോകുക next;
            }
        }

        തിരികെ elt->മൂല്യം;

    next:

        elt = (ngx_hash_elt_t *) ngx_align_ptr(&elt->name[0] + elt->len,
                                               വലുപ്പം(ശൂന്യം *));
        തുടരുക;
    }

    തിരികെ ശൂന്യ;
}


ശൂന്യം *
ngx_hash_find_wc_head(ngx_hash_wildcard_t *hwc, u_char *name, size_t len)
{
    ശൂന്യം        *മൂല്യം;
    ngx_uint_t   i, n, താക്കോൽ;

#എങ്കിൽ 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "wch:\"%*s\"", len, name);
#endif

    n = len;

    ആകുമ്പോൾ (n) {
        എങ്കിൽ (name[n - 1] == '.') {
            നിർത്തുക;
        }

        n--;
    }

    താക്കോൽ = 0;

    വേണ്ടി (i = n; i < len; i++) {
        താക്കോൽ = ngx_hash(താക്കോൽ, name[i]);
    }

#എങ്കിൽ 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "key:\"%ui\"", താക്കോൽ);
#endif

    മൂല്യം = ngx_hash_find(&hwc->hash, താക്കോൽ, &name[n], len - n);

#എങ്കിൽ 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "value:\"%p\"", മൂല്യം);
#endif

    എങ്കിൽ (മൂല്യം) {

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

        എങ്കിൽ ((uintptr_t) മൂല്യം & 2) {

            എങ്കിൽ (n == 0) {

                /* "example.com" */

                എങ്കിൽ ((uintptr_t) മൂല്യം & 1) {
                    തിരികെ ശൂന്യ;
                }

                hwc = (ngx_hash_wildcard_t *)
                                          ((uintptr_t) മൂല്യം & (uintptr_t) ~3);
                തിരികെ hwc->മൂല്യം;
            }

            hwc = (ngx_hash_wildcard_t *) ((uintptr_t) മൂല്യം & (uintptr_t) ~3);

            മൂല്യം = ngx_hash_find_wc_head(hwc, name, n - 1);

            എങ്കിൽ (മൂല്യം) {
                തിരികെ മൂല്യം;
            }

            തിരികെ hwc->മൂല്യം;
        }

        എങ്കിൽ ((uintptr_t) മൂല്യം & 1) {

            എങ്കിൽ (n == 0) {

                /* "example.com" */

                തിരികെ ശൂന്യ;
            }

            തിരികെ (ശൂന്യം *) ((uintptr_t) മൂല്യം & (uintptr_t) ~3);
        }

        തിരികെ മൂല്യം;
    }

    തിരികെ hwc->മൂല്യം;
}


ശൂന്യം *
ngx_hash_find_wc_tail(ngx_hash_wildcard_t *hwc, u_char *name, size_t len)
{
    ശൂന്യം        *മൂല്യം;
    ngx_uint_t   i, താക്കോൽ;

#എങ്കിൽ 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "wct:\"%*s\"", len, name);
#endif

    താക്കോൽ = 0;

    വേണ്ടി (i = 0; i < len; i++) {
        എങ്കിൽ (name[i] == '.') {
            നിർത്തുക;
        }

        താക്കോൽ = ngx_hash(താക്കോൽ, name[i]);
    }

    എങ്കിൽ (i == len) {
        തിരികെ ശൂന്യ;
    }

#എങ്കിൽ 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "key:\"%ui\"", താക്കോൽ);
#endif

    മൂല്യം = ngx_hash_find(&hwc->hash, താക്കോൽ, name, i);

#എങ്കിൽ 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "value:\"%p\"", മൂല്യം);
#endif

    എങ്കിൽ (മൂല്യം) {

        /*
         * the 2 low bits of value have the special meaning:
         *     00 - value is data pointer;
         *     11 - value is pointer to wildcard hash allowing "example.*".
         */

        എങ്കിൽ ((uintptr_t) മൂല്യം & 2) {

            i++;

            hwc = (ngx_hash_wildcard_t *) ((uintptr_t) മൂല്യം & (uintptr_t) ~3);

            മൂല്യം = ngx_hash_find_wc_tail(hwc, &name[i], len - i);

            എങ്കിൽ (മൂല്യം) {
                തിരികെ മൂല്യം;
            }

            തിരികെ hwc->മൂല്യം;
        }

        തിരികെ മൂല്യം;
    }

    തിരികെ hwc->മൂല്യം;
}


ശൂന്യം *
ngx_hash_find_combined(ngx_hash_combined_t *hash, ngx_uint_t താക്കോൽ, u_char *name,
    size_t len)
{
    ശൂന്യം  *മൂല്യം;

    എങ്കിൽ (hash->hash.buckets) {
        മൂല്യം = ngx_hash_find(&hash->hash, താക്കോൽ, name, len);

        എങ്കിൽ (മൂല്യം) {
            തിരികെ മൂല്യം;
        }
    }

    എങ്കിൽ (len == 0) {
        തിരികെ ശൂന്യ;
    }

    എങ്കിൽ (hash->wc_head && hash->wc_head->hash.buckets) {
        മൂല്യം = ngx_hash_find_wc_head(hash->wc_head, name, len);

        എങ്കിൽ (മൂല്യം) {
            തിരികെ മൂല്യം;
        }
    }

    എങ്കിൽ (hash->wc_tail && hash->wc_tail->hash.buckets) {
        മൂല്യം = ngx_hash_find_wc_tail(hash->wc_tail, name, len);

        എങ്കിൽ (മൂല്യം) {
            തിരികെ മൂല്യം;
        }
    }

    തിരികെ ശൂന്യ;
}


#define NGX_HASH_ELT_SIZE(name)                                               \
    (വലുപ്പം(ശൂന്യം *) + ngx_align((name)->താക്കോൽ.len + 2, വലുപ്പം(ശൂന്യം *)))

ngx_int_t
ngx_hash_init(ngx_hash_init_t *hinit, ngx_hash_താക്കോൽ_t *names, ngx_uint_t nelts)
{
    u_char          *elts;
    size_t           len;
    u_short         *test;
    ngx_uint_t       i, n, താക്കോൽ, size, start, bucket_size;
    ngx_hash_elt_t  *elt, **buckets;

    എങ്കിൽ (hinit->max_size == 0) {
        ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                      "could not build %s, you should "
                      "increase %s_max_size: %i",
                      hinit->name, hinit->name, hinit->max_size);
        തിരികെ NGX_ERROR;
    }

    എങ്കിൽ (hinit->bucket_size > 65536 - ngx_cacheline_size) {
        ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                      "could not build %s, too large "
                      "%s_bucket_size: %i",
                      hinit->name, hinit->name, hinit->bucket_size);
        തിരികെ NGX_ERROR;
    }

    വേണ്ടി (n = 0; n < nelts; n++) {
        എങ്കിൽ (names[n].താക്കോൽ.data == ശൂന്യ) {
            തുടരുക;
        }

        എങ്കിൽ (hinit->bucket_size < NGX_HASH_ELT_SIZE(&names[n]) + വലുപ്പം(ശൂന്യം *))
        {
            ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                          "could not build %s, you should "
                          "increase %s_bucket_size: %i",
                          hinit->name, hinit->name, hinit->bucket_size);
            തിരികെ NGX_ERROR;
        }
    }

    test = ngx_alloc(hinit->max_size * വലുപ്പം(u_short), hinit->pool->log);
    എങ്കിൽ (test == ശൂന്യ) {
        തിരികെ NGX_ERROR;
    }

    bucket_size = hinit->bucket_size - വലുപ്പം(ശൂന്യം *);

    start = nelts / (bucket_size / (2 * വലുപ്പം(ശൂന്യം *)));
    start = start ? start : 1;

    എങ്കിൽ (hinit->max_size > 10000 && nelts && hinit->max_size / nelts < 100) {
        start = hinit->max_size - 1000;
    }

    വേണ്ടി (size = start; size <= hinit->max_size; size++) {

        ngx_memzero(test, size * വലുപ്പം(u_short));

        വേണ്ടി (n = 0; n < nelts; n++) {
            എങ്കിൽ (names[n].താക്കോൽ.data == ശൂന്യ) {
                തുടരുക;
            }

            താക്കോൽ = names[n].താക്കോൽ_hash % size;
            len = test[താക്കോൽ] + NGX_HASH_ELT_SIZE(&names[n]);

#എങ്കിൽ 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "%ui: %ui %uz \"%V\"",
                          size, താക്കോൽ, len, &names[n].താക്കോൽ);
#endif

            എങ്കിൽ (len > bucket_size) {
                പോകുക next;
            }

            test[താക്കോൽ] = (u_short) len;
        }

        പോകുക found;

    next:

        തുടരുക;
    }

    size = hinit->max_size;

    ngx_log_error(NGX_LOG_WARN, hinit->pool->log, 0,
                  "could not build optimal %s, you should increase "
                  "either %s_max_size: %i or %s_bucket_size: %i; "
                  "ignoring %s_bucket_size",
                  hinit->name, hinit->name, hinit->max_size,
                  hinit->name, hinit->bucket_size, hinit->name);

found:

    വേണ്ടി (i = 0; i < size; i++) {
        test[i] = വലുപ്പം(ശൂന്യം *);
    }

    വേണ്ടി (n = 0; n < nelts; n++) {
        എങ്കിൽ (names[n].താക്കോൽ.data == ശൂന്യ) {
            തുടരുക;
        }

        താക്കോൽ = names[n].താക്കോൽ_hash % size;
        len = test[താക്കോൽ] + NGX_HASH_ELT_SIZE(&names[n]);

        എങ്കിൽ (len > 65536 - ngx_cacheline_size) {
            ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                          "could not build %s, you should "
                          "increase %s_max_size: %i",
                          hinit->name, hinit->name, hinit->max_size);
            ngx_free(test);
            തിരികെ NGX_ERROR;
        }

        test[താക്കോൽ] = (u_short) len;
    }

    len = 0;

    വേണ്ടി (i = 0; i < size; i++) {
        എങ്കിൽ (test[i] == വലുപ്പം(ശൂന്യം *)) {
            തുടരുക;
        }

        test[i] = (u_short) (ngx_align(test[i], ngx_cacheline_size));

        len += test[i];
    }

    എങ്കിൽ (hinit->hash == ശൂന്യ) {
        hinit->hash = ngx_pcalloc(hinit->pool, വലുപ്പം(ngx_hash_wildcard_t)
                                             + size * വലുപ്പം(ngx_hash_elt_t *));
        എങ്കിൽ (hinit->hash == ശൂന്യ) {
            ngx_free(test);
            തിരികെ NGX_ERROR;
        }

        buckets = (ngx_hash_elt_t **)
                      ((u_char *) hinit->hash + വലുപ്പം(ngx_hash_wildcard_t));

    } അല്ലെങ്കിൽ {
        buckets = ngx_pcalloc(hinit->pool, size * വലുപ്പം(ngx_hash_elt_t *));
        എങ്കിൽ (buckets == ശൂന്യ) {
            ngx_free(test);
            തിരികെ NGX_ERROR;
        }
    }

    elts = ngx_palloc(hinit->pool, len + ngx_cacheline_size);
    എങ്കിൽ (elts == ശൂന്യ) {
        ngx_free(test);
        തിരികെ NGX_ERROR;
    }

    elts = ngx_align_ptr(elts, ngx_cacheline_size);

    വേണ്ടി (i = 0; i < size; i++) {
        എങ്കിൽ (test[i] == വലുപ്പം(ശൂന്യം *)) {
            തുടരുക;
        }

        buckets[i] = (ngx_hash_elt_t *) elts;
        elts += test[i];
    }

    വേണ്ടി (i = 0; i < size; i++) {
        test[i] = 0;
    }

    വേണ്ടി (n = 0; n < nelts; n++) {
        എങ്കിൽ (names[n].താക്കോൽ.data == ശൂന്യ) {
            തുടരുക;
        }

        താക്കോൽ = names[n].താക്കോൽ_hash % size;
        elt = (ngx_hash_elt_t *) ((u_char *) buckets[താക്കോൽ] + test[താക്കോൽ]);

        elt->മൂല്യം = names[n].മൂല്യം;
        elt->len = (u_short) names[n].താക്കോൽ.len;

        ngx_strlow(elt->name, names[n].താക്കോൽ.data, names[n].താക്കോൽ.len);

        test[താക്കോൽ] = (u_short) (test[താക്കോൽ] + NGX_HASH_ELT_SIZE(&names[n]));
    }

    വേണ്ടി (i = 0; i < size; i++) {
        എങ്കിൽ (buckets[i] == ശൂന്യ) {
            തുടരുക;
        }

        elt = (ngx_hash_elt_t *) ((u_char *) buckets[i] + test[i]);

        elt->മൂല്യം = ശൂന്യ;
    }

    ngx_free(test);

    hinit->hash->buckets = buckets;
    hinit->hash->size = size;

#എങ്കിൽ 0

    വേണ്ടി (i = 0; i < size; i++) {
        ngx_str_t   val;
        ngx_uint_t  താക്കോൽ;

        elt = buckets[i];

        എങ്കിൽ (elt == ശൂന്യ) {
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "%ui: NULL", i);
            തുടരുക;
        }

        ആകുമ്പോൾ (elt->മൂല്യം) {
            val.len = elt->len;
            val.data = &elt->name[0];

            താക്കോൽ = hinit->താക്കോൽ(val.data, val.len);

            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "%ui: %p \"%V\" %ui", i, elt, &val, താക്കോൽ);

            elt = (ngx_hash_elt_t *) ngx_align_ptr(&elt->name[0] + elt->len,
                                                   വലുപ്പം(ശൂന്യം *));
        }
    }

#endif

    തിരികെ NGX_OK;
}


ngx_int_t
ngx_hash_wildcard_init(ngx_hash_init_t *hinit, ngx_hash_താക്കോൽ_t *names,
    ngx_uint_t nelts)
{
    size_t                len, dot_len;
    ngx_uint_t            i, n, dot;
    ngx_array_t           curr_names, next_names;
    ngx_hash_താക്കോൽ_t       *name, *next_name;
    ngx_hash_init_t       h;
    ngx_hash_wildcard_t  *wdc;

    എങ്കിൽ (ngx_array_init(&curr_names, hinit->temp_pool, nelts,
                       വലുപ്പം(ngx_hash_താക്കോൽ_t))
        != NGX_OK)
    {
        തിരികെ NGX_ERROR;
    }

    എങ്കിൽ (ngx_array_init(&next_names, hinit->temp_pool, nelts,
                       വലുപ്പം(ngx_hash_താക്കോൽ_t))
        != NGX_OK)
    {
        തിരികെ NGX_ERROR;
    }

    വേണ്ടി (n = 0; n < nelts; n = i) {

#എങ്കിൽ 0
        ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                      "wc0: \"%V\"", &names[n].താക്കോൽ);
#endif

        dot = 0;

        വേണ്ടി (len = 0; len < names[n].താക്കോൽ.len; len++) {
            എങ്കിൽ (names[n].താക്കോൽ.data[len] == '.') {
                dot = 1;
                നിർത്തുക;
            }
        }

        name = ngx_array_തള്ളുക(&curr_names);
        എങ്കിൽ (name == ശൂന്യ) {
            തിരികെ NGX_ERROR;
        }

        name->താക്കോൽ.len = len;
        name->താക്കോൽ.data = names[n].താക്കോൽ.data;
        name->താക്കോൽ_hash = hinit->താക്കോൽ(name->താക്കോൽ.data, name->താക്കോൽ.len);
        name->മൂല്യം = names[n].മൂല്യം;

#എങ്കിൽ 0
        ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                      "wc1: \"%V\" %ui", &name->താക്കോൽ, dot);
#endif

        dot_len = len + 1;

        എങ്കിൽ (dot) {
            len++;
        }

        next_names.nelts = 0;

        എങ്കിൽ (names[n].താക്കോൽ.len != len) {
            next_name = ngx_array_തള്ളുക(&next_names);
            എങ്കിൽ (next_name == ശൂന്യ) {
                തിരികെ NGX_ERROR;
            }

            next_name->താക്കോൽ.len = names[n].താക്കോൽ.len - len;
            next_name->താക്കോൽ.data = names[n].താക്കോൽ.data + len;
            next_name->താക്കോൽ_hash = 0;
            next_name->മൂല്യം = names[n].മൂല്യം;

#എങ്കിൽ 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "wc2: \"%V\"", &next_name->താക്കോൽ);
#endif
        }

        വേണ്ടി (i = n + 1; i < nelts; i++) {
            എങ്കിൽ (ngx_strncmp(names[n].താക്കോൽ.data, names[i].താക്കോൽ.data, len) != 0) {
                നിർത്തുക;
            }

            എങ്കിൽ (!dot
                && names[i].താക്കോൽ.len > len
                && names[i].താക്കോൽ.data[len] != '.')
            {
                നിർത്തുക;
            }

            next_name = ngx_array_തള്ളുക(&next_names);
            എങ്കിൽ (next_name == ശൂന്യ) {
                തിരികെ NGX_ERROR;
            }

            next_name->താക്കോൽ.len = names[i].താക്കോൽ.len - dot_len;
            next_name->താക്കോൽ.data = names[i].താക്കോൽ.data + dot_len;
            next_name->താക്കോൽ_hash = 0;
            next_name->മൂല്യം = names[i].മൂല്യം;

#എങ്കിൽ 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "wc3: \"%V\"", &next_name->താക്കോൽ);
#endif
        }

        എങ്കിൽ (next_names.nelts) {

            h = *hinit;
            h.hash = ശൂന്യ;

            എങ്കിൽ (ngx_hash_wildcard_init(&h, (ngx_hash_താക്കോൽ_t *) next_names.elts,
                                       next_names.nelts)
                != NGX_OK)
            {
                തിരികെ NGX_ERROR;
            }

            wdc = (ngx_hash_wildcard_t *) h.hash;

            എങ്കിൽ (names[n].താക്കോൽ.len == len) {
                wdc->മൂല്യം = names[n].മൂല്യം;
            }

            name->മൂല്യം = (ശൂന്യം *) ((uintptr_t) wdc | (dot ? 3 : 2));

        } അല്ലെങ്കിൽ എങ്കിൽ (dot) {
            name->മൂല്യം = (ശൂന്യം *) ((uintptr_t) name->മൂല്യം | 1);
        }
    }

    എങ്കിൽ (ngx_hash_init(hinit, (ngx_hash_താക്കോൽ_t *) curr_names.elts,
                      curr_names.nelts)
        != NGX_OK)
    {
        തിരികെ NGX_ERROR;
    }

    തിരികെ NGX_OK;
}


ngx_uint_t
ngx_hash_താക്കോൽ(u_char *data, size_t len)
{
    ngx_uint_t  i, താക്കോൽ;

    താക്കോൽ = 0;

    വേണ്ടി (i = 0; i < len; i++) {
        താക്കോൽ = ngx_hash(താക്കോൽ, data[i]);
    }

    തിരികെ താക്കോൽ;
}


ngx_uint_t
ngx_hash_താക്കോൽ_lc(u_char *data, size_t len)
{
    ngx_uint_t  i, താക്കോൽ;

    താക്കോൽ = 0;

    വേണ്ടി (i = 0; i < len; i++) {
        താക്കോൽ = ngx_hash(താക്കോൽ, ngx_tolower(data[i]));
    }

    തിരികെ താക്കോൽ;
}


ngx_uint_t
ngx_hash_strlow(u_char *dst, u_char *src, size_t n)
{
    ngx_uint_t  താക്കോൽ;

    താക്കോൽ = 0;

    ആകുമ്പോൾ (n--) {
        *dst = ngx_tolower(*src);
        താക്കോൽ = ngx_hash(താക്കോൽ, *dst);
        dst++;
        src++;
    }

    തിരികെ താക്കോൽ;
}


ngx_int_t
ngx_hash_താക്കോലുകൾ_array_init(ngx_hash_താക്കോലുകൾ_arrays_t *ha, ngx_uint_t type)
{
    ngx_uint_t  asize;

    എങ്കിൽ (type == NGX_HASH_SMALL) {
        asize = 4;
        ha->hsize = 107;

    } അല്ലെങ്കിൽ {
        asize = NGX_HASH_LARGE_ASIZE;
        ha->hsize = NGX_HASH_LARGE_HSIZE;
    }

    എങ്കിൽ (ngx_array_init(&ha->താക്കോലുകൾ, ha->temp_pool, asize, വലുപ്പം(ngx_hash_താക്കോൽ_t))
        != NGX_OK)
    {
        തിരികെ NGX_ERROR;
    }

    എങ്കിൽ (ngx_array_init(&ha->dns_wc_head, ha->temp_pool, asize,
                       വലുപ്പം(ngx_hash_താക്കോൽ_t))
        != NGX_OK)
    {
        തിരികെ NGX_ERROR;
    }

    എങ്കിൽ (ngx_array_init(&ha->dns_wc_tail, ha->temp_pool, asize,
                       വലുപ്പം(ngx_hash_താക്കോൽ_t))
        != NGX_OK)
    {
        തിരികെ NGX_ERROR;
    }

    ha->താക്കോലുകൾ_hash = ngx_pcalloc(ha->temp_pool, വലുപ്പം(ngx_array_t) * ha->hsize);
    എങ്കിൽ (ha->താക്കോലുകൾ_hash == ശൂന്യ) {
        തിരികെ NGX_ERROR;
    }

    ha->dns_wc_head_hash = ngx_pcalloc(ha->temp_pool,
                                       വലുപ്പം(ngx_array_t) * ha->hsize);
    എങ്കിൽ (ha->dns_wc_head_hash == ശൂന്യ) {
        തിരികെ NGX_ERROR;
    }

    ha->dns_wc_tail_hash = ngx_pcalloc(ha->temp_pool,
                                       വലുപ്പം(ngx_array_t) * ha->hsize);
    എങ്കിൽ (ha->dns_wc_tail_hash == ശൂന്യ) {
        തിരികെ NGX_ERROR;
    }

    തിരികെ NGX_OK;
}


ngx_int_t
ngx_hash_add_താക്കോൽ(ngx_hash_താക്കോലുകൾ_arrays_t *ha, ngx_str_t *താക്കോൽ, ശൂന്യം *മൂല്യം,
    ngx_uint_t flags)
{
    size_t           len;
    u_char          *p;
    ngx_str_t       *name;
    ngx_uint_t       i, k, n, skip, last;
    ngx_array_t     *താക്കോലുകൾ, *hwc;
    ngx_hash_താക്കോൽ_t  *hk;

    last = താക്കോൽ->len;

    എങ്കിൽ (flags & NGX_HASH_WILDCARD_താക്കോൽ) {

        /*
         * supported wildcards:
         *     "*.example.com", ".example.com", and "www.example.*"
         */

        n = 0;

        വേണ്ടി (i = 0; i < താക്കോൽ->len; i++) {

            എങ്കിൽ (താക്കോൽ->data[i] == '*') {
                എങ്കിൽ (++n > 1) {
                    തിരികെ NGX_DECLINED;
                }
            }

            എങ്കിൽ (താക്കോൽ->data[i] == '.' && താക്കോൽ->data[i + 1] == '.') {
                തിരികെ NGX_DECLINED;
            }

            എങ്കിൽ (താക്കോൽ->data[i] == '\0') {
                തിരികെ NGX_DECLINED;
            }
        }

        എങ്കിൽ (താക്കോൽ->len > 1 && താക്കോൽ->data[0] == '.') {
            skip = 1;
            പോകുക wildcard;
        }

        എങ്കിൽ (താക്കോൽ->len > 2) {

            എങ്കിൽ (താക്കോൽ->data[0] == '*' && താക്കോൽ->data[1] == '.') {
                skip = 2;
                പോകുക wildcard;
            }

            എങ്കിൽ (താക്കോൽ->data[i - 2] == '.' && താക്കോൽ->data[i - 1] == '*') {
                skip = 0;
                last -= 2;
                പോകുക wildcard;
            }
        }

        എങ്കിൽ (n) {
            തിരികെ NGX_DECLINED;
        }
    }

    /* exact hash */

    k = 0;

    വേണ്ടി (i = 0; i < last; i++) {
        എങ്കിൽ (!(flags & NGX_HASH_READONLY_താക്കോൽ)) {
            താക്കോൽ->data[i] = ngx_tolower(താക്കോൽ->data[i]);
        }
        k = ngx_hash(k, താക്കോൽ->data[i]);
    }

    k %= ha->hsize;

    /* check conflicts in exact hash */

    name = ha->താക്കോലുകൾ_hash[k].elts;

    എങ്കിൽ (name) {
        വേണ്ടി (i = 0; i < ha->താക്കോലുകൾ_hash[k].nelts; i++) {
            എങ്കിൽ (last != name[i].len) {
                തുടരുക;
            }

            എങ്കിൽ (ngx_strncmp(താക്കോൽ->data, name[i].data, last) == 0) {
                തിരികെ NGX_BUSY;
            }
        }

    } അല്ലെങ്കിൽ {
        എങ്കിൽ (ngx_array_init(&ha->താക്കോലുകൾ_hash[k], ha->temp_pool, 4,
                           വലുപ്പം(ngx_str_t))
            != NGX_OK)
        {
            തിരികെ NGX_ERROR;
        }
    }

    name = ngx_array_തള്ളുക(&ha->താക്കോലുകൾ_hash[k]);
    എങ്കിൽ (name == ശൂന്യ) {
        തിരികെ NGX_ERROR;
    }

    *name = *താക്കോൽ;

    hk = ngx_array_തള്ളുക(&ha->താക്കോലുകൾ);
    എങ്കിൽ (hk == ശൂന്യ) {
        തിരികെ NGX_ERROR;
    }

    hk->താക്കോൽ = *താക്കോൽ;
    hk->താക്കോൽ_hash = ngx_hash_താക്കോൽ(താക്കോൽ->data, last);
    hk->മൂല്യം = മൂല്യം;

    തിരികെ NGX_OK;


wildcard:

    /* wildcard hash */

    k = ngx_hash_strlow(&താക്കോൽ->data[skip], &താക്കോൽ->data[skip], last - skip);

    k %= ha->hsize;

    എങ്കിൽ (skip == 1) {

        /* check conflicts in exact hash for ".example.com" */

        name = ha->താക്കോലുകൾ_hash[k].elts;

        എങ്കിൽ (name) {
            len = last - skip;

            വേണ്ടി (i = 0; i < ha->താക്കോലുകൾ_hash[k].nelts; i++) {
                എങ്കിൽ (len != name[i].len) {
                    തുടരുക;
                }

                എങ്കിൽ (ngx_strncmp(&താക്കോൽ->data[1], name[i].data, len) == 0) {
                    തിരികെ NGX_BUSY;
                }
            }

        } അല്ലെങ്കിൽ {
            എങ്കിൽ (ngx_array_init(&ha->താക്കോലുകൾ_hash[k], ha->temp_pool, 4,
                               വലുപ്പം(ngx_str_t))
                != NGX_OK)
            {
                തിരികെ NGX_ERROR;
            }
        }

        name = ngx_array_തള്ളുക(&ha->താക്കോലുകൾ_hash[k]);
        എങ്കിൽ (name == ശൂന്യ) {
            തിരികെ NGX_ERROR;
        }

        name->len = last - 1;
        name->data = ngx_pnalloc(ha->temp_pool, name->len);
        എങ്കിൽ (name->data == ശൂന്യ) {
            തിരികെ NGX_ERROR;
        }

        ngx_memcpy(name->data, &താക്കോൽ->data[1], name->len);
    }


    എങ്കിൽ (skip) {

        /*
         * convert "*.example.com" to "com.example.\0"
         *      and ".example.com" to "com.example\0"
         */

        p = ngx_pnalloc(ha->temp_pool, last);
        എങ്കിൽ (p == ശൂന്യ) {
            തിരികെ NGX_ERROR;
        }

        len = 0;
        n = 0;

        വേണ്ടി (i = last - 1; i; i--) {
            എങ്കിൽ (താക്കോൽ->data[i] == '.') {
                ngx_memcpy(&p[n], &താക്കോൽ->data[i + 1], len);
                n += len;
                p[n++] = '.';
                len = 0;
                തുടരുക;
            }

            len++;
        }

        എങ്കിൽ (len) {
            ngx_memcpy(&p[n], &താക്കോൽ->data[1], len);
            n += len;
        }

        p[n] = '\0';

        hwc = &ha->dns_wc_head;
        താക്കോലുകൾ = &ha->dns_wc_head_hash[k];

    } അല്ലെങ്കിൽ {

        /* convert "www.example.*" to "www.example\0" */

        last++;

        p = ngx_pnalloc(ha->temp_pool, last);
        എങ്കിൽ (p == ശൂന്യ) {
            തിരികെ NGX_ERROR;
        }

        ngx_cpystrn(p, താക്കോൽ->data, last);

        hwc = &ha->dns_wc_tail;
        താക്കോലുകൾ = &ha->dns_wc_tail_hash[k];
    }


    /* check conflicts in wildcard hash */

    name = താക്കോലുകൾ->elts;

    എങ്കിൽ (name) {
        len = last - skip;

        വേണ്ടി (i = 0; i < താക്കോലുകൾ->nelts; i++) {
            എങ്കിൽ (len != name[i].len) {
                തുടരുക;
            }

            എങ്കിൽ (ngx_strncmp(താക്കോൽ->data + skip, name[i].data, len) == 0) {
                തിരികെ NGX_BUSY;
            }
        }

    } അല്ലെങ്കിൽ {
        എങ്കിൽ (ngx_array_init(താക്കോലുകൾ, ha->temp_pool, 4, വലുപ്പം(ngx_str_t)) != NGX_OK)
        {
            തിരികെ NGX_ERROR;
        }
    }

    name = ngx_array_തള്ളുക(താക്കോലുകൾ);
    എങ്കിൽ (name == ശൂന്യ) {
        തിരികെ NGX_ERROR;
    }

    name->len = last - skip;
    name->data = ngx_pnalloc(ha->temp_pool, name->len);
    എങ്കിൽ (name->data == ശൂന്യ) {
        തിരികെ NGX_ERROR;
    }

    ngx_memcpy(name->data, താക്കോൽ->data + skip, name->len);


    /* add to wildcard hash */

    hk = ngx_array_തള്ളുക(hwc);
    എങ്കിൽ (hk == ശൂന്യ) {
        തിരികെ NGX_ERROR;
    }

    hk->താക്കോൽ.len = last - 1;
    hk->താക്കോൽ.data = p;
    hk->താക്കോൽ_hash = 0;
    hk->മൂല്യം = മൂല്യം;

    തിരികെ NGX_OK;
}
