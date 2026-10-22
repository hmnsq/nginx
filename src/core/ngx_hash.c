
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#અન્તર્ભાવ <ngx_config.h>
#અન્તર્ભાવ <ngx_core.h>


શૂન્ય *
ngx_hash_find(ngx_hash_t *hash, ngx_uint_t ચાવી, u_char *name, size_t len)
{
    ngx_uint_t       i;
    ngx_hash_elt_t  *elt;

#જો 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "hf:\"%*s\"", len, name);
#endif

    elt = hash->buckets[ચાવી % hash->size];

    જો (elt == ખાલી) {
        પાછા ખાલી;
    }

    જ્યારે (elt->કિંમત) {
        જો (len != (size_t) elt->len) {
            જાઓ next;
        }

        માટે (i = 0; i < len; i++) {
            જો (name[i] != elt->name[i]) {
                જાઓ next;
            }
        }

        પાછા elt->કિંમત;

    next:

        elt = (ngx_hash_elt_t *) ngx_align_ptr(&elt->name[0] + elt->len,
                                               કદ(શૂન્ય *));
        ચાલુ;
    }

    પાછા ખાલી;
}


શૂન્ય *
ngx_hash_find_wc_head(ngx_hash_wildcard_t *hwc, u_char *name, size_t len)
{
    શૂન્ય        *કિંમત;
    ngx_uint_t   i, n, ચાવી;

#જો 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "wch:\"%*s\"", len, name);
#endif

    n = len;

    જ્યારે (n) {
        જો (name[n - 1] == '.') {
            તોડો;
        }

        n--;
    }

    ચાવી = 0;

    માટે (i = n; i < len; i++) {
        ચાવી = ngx_hash(ચાવી, name[i]);
    }

#જો 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "key:\"%ui\"", ચાવી);
#endif

    કિંમત = ngx_hash_find(&hwc->hash, ચાવી, &name[n], len - n);

#જો 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "value:\"%p\"", કિંમત);
#endif

    જો (કિંમત) {

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

        જો ((uintptr_t) કિંમત & 2) {

            જો (n == 0) {

                /* "example.com" */

                જો ((uintptr_t) કિંમત & 1) {
                    પાછા ખાલી;
                }

                hwc = (ngx_hash_wildcard_t *)
                                          ((uintptr_t) કિંમત & (uintptr_t) ~3);
                પાછા hwc->કિંમત;
            }

            hwc = (ngx_hash_wildcard_t *) ((uintptr_t) કિંમત & (uintptr_t) ~3);

            કિંમત = ngx_hash_find_wc_head(hwc, name, n - 1);

            જો (કિંમત) {
                પાછા કિંમત;
            }

            પાછા hwc->કિંમત;
        }

        જો ((uintptr_t) કિંમત & 1) {

            જો (n == 0) {

                /* "example.com" */

                પાછા ખાલી;
            }

            પાછા (શૂન્ય *) ((uintptr_t) કિંમત & (uintptr_t) ~3);
        }

        પાછા કિંમત;
    }

    પાછા hwc->કિંમત;
}


શૂન્ય *
ngx_hash_find_wc_tail(ngx_hash_wildcard_t *hwc, u_char *name, size_t len)
{
    શૂન્ય        *કિંમત;
    ngx_uint_t   i, ચાવી;

#જો 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "wct:\"%*s\"", len, name);
#endif

    ચાવી = 0;

    માટે (i = 0; i < len; i++) {
        જો (name[i] == '.') {
            તોડો;
        }

        ચાવી = ngx_hash(ચાવી, name[i]);
    }

    જો (i == len) {
        પાછા ખાલી;
    }

#જો 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "key:\"%ui\"", ચાવી);
#endif

    કિંમત = ngx_hash_find(&hwc->hash, ચાવી, name, i);

#જો 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "value:\"%p\"", કિંમત);
#endif

    જો (કિંમત) {

        /*
         * the 2 low bits of value have the special meaning:
         *     00 - value is data pointer;
         *     11 - value is pointer to wildcard hash allowing "example.*".
         */

        જો ((uintptr_t) કિંમત & 2) {

            i++;

            hwc = (ngx_hash_wildcard_t *) ((uintptr_t) કિંમત & (uintptr_t) ~3);

            કિંમત = ngx_hash_find_wc_tail(hwc, &name[i], len - i);

            જો (કિંમત) {
                પાછા કિંમત;
            }

            પાછા hwc->કિંમત;
        }

        પાછા કિંમત;
    }

    પાછા hwc->કિંમત;
}


શૂન્ય *
ngx_hash_find_combined(ngx_hash_combined_t *hash, ngx_uint_t ચાવી, u_char *name,
    size_t len)
{
    શૂન્ય  *કિંમત;

    જો (hash->hash.buckets) {
        કિંમત = ngx_hash_find(&hash->hash, ચાવી, name, len);

        જો (કિંમત) {
            પાછા કિંમત;
        }
    }

    જો (len == 0) {
        પાછા ખાલી;
    }

    જો (hash->wc_head && hash->wc_head->hash.buckets) {
        કિંમત = ngx_hash_find_wc_head(hash->wc_head, name, len);

        જો (કિંમત) {
            પાછા કિંમત;
        }
    }

    જો (hash->wc_tail && hash->wc_tail->hash.buckets) {
        કિંમત = ngx_hash_find_wc_tail(hash->wc_tail, name, len);

        જો (કિંમત) {
            પાછા કિંમત;
        }
    }

    પાછા ખાલી;
}


#define NGX_HASH_ELT_SIZE(name)                                               \
    (કદ(શૂન્ય *) + ngx_align((name)->ચાવી.len + 2, કદ(શૂન્ય *)))

ngx_int_t
ngx_hash_init(ngx_hash_init_t *hinit, ngx_hash_ચાવી_t *names, ngx_uint_t nelts)
{
    u_char          *elts;
    size_t           len;
    u_short         *test;
    ngx_uint_t       i, n, ચાવી, size, start, bucket_size;
    ngx_hash_elt_t  *elt, **buckets;

    જો (hinit->max_size == 0) {
        ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                      "could not build %s, you should "
                      "increase %s_max_size: %i",
                      hinit->name, hinit->name, hinit->max_size);
        પાછા NGX_ERROR;
    }

    જો (hinit->bucket_size > 65536 - ngx_cacheline_size) {
        ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                      "could not build %s, too large "
                      "%s_bucket_size: %i",
                      hinit->name, hinit->name, hinit->bucket_size);
        પાછા NGX_ERROR;
    }

    માટે (n = 0; n < nelts; n++) {
        જો (names[n].ચાવી.data == ખાલી) {
            ચાલુ;
        }

        જો (hinit->bucket_size < NGX_HASH_ELT_SIZE(&names[n]) + કદ(શૂન્ય *))
        {
            ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                          "could not build %s, you should "
                          "increase %s_bucket_size: %i",
                          hinit->name, hinit->name, hinit->bucket_size);
            પાછા NGX_ERROR;
        }
    }

    test = ngx_alloc(hinit->max_size * કદ(u_short), hinit->pool->log);
    જો (test == ખાલી) {
        પાછા NGX_ERROR;
    }

    bucket_size = hinit->bucket_size - કદ(શૂન્ય *);

    start = nelts / (bucket_size / (2 * કદ(શૂન્ય *)));
    start = start ? start : 1;

    જો (hinit->max_size > 10000 && nelts && hinit->max_size / nelts < 100) {
        start = hinit->max_size - 1000;
    }

    માટે (size = start; size <= hinit->max_size; size++) {

        ngx_memzero(test, size * કદ(u_short));

        માટે (n = 0; n < nelts; n++) {
            જો (names[n].ચાવી.data == ખાલી) {
                ચાલુ;
            }

            ચાવી = names[n].ચાવી_hash % size;
            len = test[ચાવી] + NGX_HASH_ELT_SIZE(&names[n]);

#જો 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "%ui: %ui %uz \"%V\"",
                          size, ચાવી, len, &names[n].ચાવી);
#endif

            જો (len > bucket_size) {
                જાઓ next;
            }

            test[ચાવી] = (u_short) len;
        }

        જાઓ found;

    next:

        ચાલુ;
    }

    size = hinit->max_size;

    ngx_log_error(NGX_LOG_WARN, hinit->pool->log, 0,
                  "could not build optimal %s, you should increase "
                  "either %s_max_size: %i or %s_bucket_size: %i; "
                  "ignoring %s_bucket_size",
                  hinit->name, hinit->name, hinit->max_size,
                  hinit->name, hinit->bucket_size, hinit->name);

found:

    માટે (i = 0; i < size; i++) {
        test[i] = કદ(શૂન્ય *);
    }

    માટે (n = 0; n < nelts; n++) {
        જો (names[n].ચાવી.data == ખાલી) {
            ચાલુ;
        }

        ચાવી = names[n].ચાવી_hash % size;
        len = test[ચાવી] + NGX_HASH_ELT_SIZE(&names[n]);

        જો (len > 65536 - ngx_cacheline_size) {
            ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                          "could not build %s, you should "
                          "increase %s_max_size: %i",
                          hinit->name, hinit->name, hinit->max_size);
            ngx_free(test);
            પાછા NGX_ERROR;
        }

        test[ચાવી] = (u_short) len;
    }

    len = 0;

    માટે (i = 0; i < size; i++) {
        જો (test[i] == કદ(શૂન્ય *)) {
            ચાલુ;
        }

        test[i] = (u_short) (ngx_align(test[i], ngx_cacheline_size));

        len += test[i];
    }

    જો (hinit->hash == ખાલી) {
        hinit->hash = ngx_pcalloc(hinit->pool, કદ(ngx_hash_wildcard_t)
                                             + size * કદ(ngx_hash_elt_t *));
        જો (hinit->hash == ખાલી) {
            ngx_free(test);
            પાછા NGX_ERROR;
        }

        buckets = (ngx_hash_elt_t **)
                      ((u_char *) hinit->hash + કદ(ngx_hash_wildcard_t));

    } નહીંતર {
        buckets = ngx_pcalloc(hinit->pool, size * કદ(ngx_hash_elt_t *));
        જો (buckets == ખાલી) {
            ngx_free(test);
            પાછા NGX_ERROR;
        }
    }

    elts = ngx_palloc(hinit->pool, len + ngx_cacheline_size);
    જો (elts == ખાલી) {
        ngx_free(test);
        પાછા NGX_ERROR;
    }

    elts = ngx_align_ptr(elts, ngx_cacheline_size);

    માટે (i = 0; i < size; i++) {
        જો (test[i] == કદ(શૂન્ય *)) {
            ચાલુ;
        }

        buckets[i] = (ngx_hash_elt_t *) elts;
        elts += test[i];
    }

    માટે (i = 0; i < size; i++) {
        test[i] = 0;
    }

    માટે (n = 0; n < nelts; n++) {
        જો (names[n].ચાવી.data == ખાલી) {
            ચાલુ;
        }

        ચાવી = names[n].ચાવી_hash % size;
        elt = (ngx_hash_elt_t *) ((u_char *) buckets[ચાવી] + test[ચાવી]);

        elt->કિંમત = names[n].કિંમત;
        elt->len = (u_short) names[n].ચાવી.len;

        ngx_strlow(elt->name, names[n].ચાવી.data, names[n].ચાવી.len);

        test[ચાવી] = (u_short) (test[ચાવી] + NGX_HASH_ELT_SIZE(&names[n]));
    }

    માટે (i = 0; i < size; i++) {
        જો (buckets[i] == ખાલી) {
            ચાલુ;
        }

        elt = (ngx_hash_elt_t *) ((u_char *) buckets[i] + test[i]);

        elt->કિંમત = ખાલી;
    }

    ngx_free(test);

    hinit->hash->buckets = buckets;
    hinit->hash->size = size;

#જો 0

    માટે (i = 0; i < size; i++) {
        ngx_str_t   val;
        ngx_uint_t  ચાવી;

        elt = buckets[i];

        જો (elt == ખાલી) {
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "%ui: NULL", i);
            ચાલુ;
        }

        જ્યારે (elt->કિંમત) {
            val.len = elt->len;
            val.data = &elt->name[0];

            ચાવી = hinit->ચાવી(val.data, val.len);

            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "%ui: %p \"%V\" %ui", i, elt, &val, ચાવી);

            elt = (ngx_hash_elt_t *) ngx_align_ptr(&elt->name[0] + elt->len,
                                                   કદ(શૂન્ય *));
        }
    }

#endif

    પાછા NGX_OK;
}


ngx_int_t
ngx_hash_wildcard_init(ngx_hash_init_t *hinit, ngx_hash_ચાવી_t *names,
    ngx_uint_t nelts)
{
    size_t                len, dot_len;
    ngx_uint_t            i, n, dot;
    ngx_array_t           curr_names, next_names;
    ngx_hash_ચાવી_t       *name, *next_name;
    ngx_hash_init_t       h;
    ngx_hash_wildcard_t  *wdc;

    જો (ngx_array_init(&curr_names, hinit->temp_pool, nelts,
                       કદ(ngx_hash_ચાવી_t))
        != NGX_OK)
    {
        પાછા NGX_ERROR;
    }

    જો (ngx_array_init(&next_names, hinit->temp_pool, nelts,
                       કદ(ngx_hash_ચાવી_t))
        != NGX_OK)
    {
        પાછા NGX_ERROR;
    }

    માટે (n = 0; n < nelts; n = i) {

#જો 0
        ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                      "wc0: \"%V\"", &names[n].ચાવી);
#endif

        dot = 0;

        માટે (len = 0; len < names[n].ચાવી.len; len++) {
            જો (names[n].ચાવી.data[len] == '.') {
                dot = 1;
                તોડો;
            }
        }

        name = ngx_array_ધકેલો(&curr_names);
        જો (name == ખાલી) {
            પાછા NGX_ERROR;
        }

        name->ચાવી.len = len;
        name->ચાવી.data = names[n].ચાવી.data;
        name->ચાવી_hash = hinit->ચાવી(name->ચાવી.data, name->ચાવી.len);
        name->કિંમત = names[n].કિંમત;

#જો 0
        ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                      "wc1: \"%V\" %ui", &name->ચાવી, dot);
#endif

        dot_len = len + 1;

        જો (dot) {
            len++;
        }

        next_names.nelts = 0;

        જો (names[n].ચાવી.len != len) {
            next_name = ngx_array_ધકેલો(&next_names);
            જો (next_name == ખાલી) {
                પાછા NGX_ERROR;
            }

            next_name->ચાવી.len = names[n].ચાવી.len - len;
            next_name->ચાવી.data = names[n].ચાવી.data + len;
            next_name->ચાવી_hash = 0;
            next_name->કિંમત = names[n].કિંમત;

#જો 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "wc2: \"%V\"", &next_name->ચાવી);
#endif
        }

        માટે (i = n + 1; i < nelts; i++) {
            જો (ngx_strncmp(names[n].ચાવી.data, names[i].ચાવી.data, len) != 0) {
                તોડો;
            }

            જો (!dot
                && names[i].ચાવી.len > len
                && names[i].ચાવી.data[len] != '.')
            {
                તોડો;
            }

            next_name = ngx_array_ધકેલો(&next_names);
            જો (next_name == ખાલી) {
                પાછા NGX_ERROR;
            }

            next_name->ચાવી.len = names[i].ચાવી.len - dot_len;
            next_name->ચાવી.data = names[i].ચાવી.data + dot_len;
            next_name->ચાવી_hash = 0;
            next_name->કિંમત = names[i].કિંમત;

#જો 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "wc3: \"%V\"", &next_name->ચાવી);
#endif
        }

        જો (next_names.nelts) {

            h = *hinit;
            h.hash = ખાલી;

            જો (ngx_hash_wildcard_init(&h, (ngx_hash_ચાવી_t *) next_names.elts,
                                       next_names.nelts)
                != NGX_OK)
            {
                પાછા NGX_ERROR;
            }

            wdc = (ngx_hash_wildcard_t *) h.hash;

            જો (names[n].ચાવી.len == len) {
                wdc->કિંમત = names[n].કિંમત;
            }

            name->કિંમત = (શૂન્ય *) ((uintptr_t) wdc | (dot ? 3 : 2));

        } નહીંતર જો (dot) {
            name->કિંમત = (શૂન્ય *) ((uintptr_t) name->કિંમત | 1);
        }
    }

    જો (ngx_hash_init(hinit, (ngx_hash_ચાવી_t *) curr_names.elts,
                      curr_names.nelts)
        != NGX_OK)
    {
        પાછા NGX_ERROR;
    }

    પાછા NGX_OK;
}


ngx_uint_t
ngx_hash_ચાવી(u_char *data, size_t len)
{
    ngx_uint_t  i, ચાવી;

    ચાવી = 0;

    માટે (i = 0; i < len; i++) {
        ચાવી = ngx_hash(ચાવી, data[i]);
    }

    પાછા ચાવી;
}


ngx_uint_t
ngx_hash_ચાવી_lc(u_char *data, size_t len)
{
    ngx_uint_t  i, ચાવી;

    ચાવી = 0;

    માટે (i = 0; i < len; i++) {
        ચાવી = ngx_hash(ચાવી, ngx_tolower(data[i]));
    }

    પાછા ચાવી;
}


ngx_uint_t
ngx_hash_strlow(u_char *dst, u_char *src, size_t n)
{
    ngx_uint_t  ચાવી;

    ચાવી = 0;

    જ્યારે (n--) {
        *dst = ngx_tolower(*src);
        ચાવી = ngx_hash(ચાવી, *dst);
        dst++;
        src++;
    }

    પાછા ચાવી;
}


ngx_int_t
ngx_hash_ચાવીઓ_array_init(ngx_hash_ચાવીઓ_arrays_t *ha, ngx_uint_t type)
{
    ngx_uint_t  asize;

    જો (type == NGX_HASH_SMALL) {
        asize = 4;
        ha->hsize = 107;

    } નહીંતર {
        asize = NGX_HASH_LARGE_ASIZE;
        ha->hsize = NGX_HASH_LARGE_HSIZE;
    }

    જો (ngx_array_init(&ha->ચાવીઓ, ha->temp_pool, asize, કદ(ngx_hash_ચાવી_t))
        != NGX_OK)
    {
        પાછા NGX_ERROR;
    }

    જો (ngx_array_init(&ha->dns_wc_head, ha->temp_pool, asize,
                       કદ(ngx_hash_ચાવી_t))
        != NGX_OK)
    {
        પાછા NGX_ERROR;
    }

    જો (ngx_array_init(&ha->dns_wc_tail, ha->temp_pool, asize,
                       કદ(ngx_hash_ચાવી_t))
        != NGX_OK)
    {
        પાછા NGX_ERROR;
    }

    ha->ચાવીઓ_hash = ngx_pcalloc(ha->temp_pool, કદ(ngx_array_t) * ha->hsize);
    જો (ha->ચાવીઓ_hash == ખાલી) {
        પાછા NGX_ERROR;
    }

    ha->dns_wc_head_hash = ngx_pcalloc(ha->temp_pool,
                                       કદ(ngx_array_t) * ha->hsize);
    જો (ha->dns_wc_head_hash == ખાલી) {
        પાછા NGX_ERROR;
    }

    ha->dns_wc_tail_hash = ngx_pcalloc(ha->temp_pool,
                                       કદ(ngx_array_t) * ha->hsize);
    જો (ha->dns_wc_tail_hash == ખાલી) {
        પાછા NGX_ERROR;
    }

    પાછા NGX_OK;
}


ngx_int_t
ngx_hash_add_ચાવી(ngx_hash_ચાવીઓ_arrays_t *ha, ngx_str_t *ચાવી, શૂન્ય *કિંમત,
    ngx_uint_t flags)
{
    size_t           len;
    u_char          *p;
    ngx_str_t       *name;
    ngx_uint_t       i, k, n, skip, last;
    ngx_array_t     *ચાવીઓ, *hwc;
    ngx_hash_ચાવી_t  *hk;

    last = ચાવી->len;

    જો (flags & NGX_HASH_WILDCARD_ચાવી) {

        /*
         * supported wildcards:
         *     "*.example.com", ".example.com", and "www.example.*"
         */

        n = 0;

        માટે (i = 0; i < ચાવી->len; i++) {

            જો (ચાવી->data[i] == '*') {
                જો (++n > 1) {
                    પાછા NGX_DECLINED;
                }
            }

            જો (ચાવી->data[i] == '.' && ચાવી->data[i + 1] == '.') {
                પાછા NGX_DECLINED;
            }

            જો (ચાવી->data[i] == '\0') {
                પાછા NGX_DECLINED;
            }
        }

        જો (ચાવી->len > 1 && ચાવી->data[0] == '.') {
            skip = 1;
            જાઓ wildcard;
        }

        જો (ચાવી->len > 2) {

            જો (ચાવી->data[0] == '*' && ચાવી->data[1] == '.') {
                skip = 2;
                જાઓ wildcard;
            }

            જો (ચાવી->data[i - 2] == '.' && ચાવી->data[i - 1] == '*') {
                skip = 0;
                last -= 2;
                જાઓ wildcard;
            }
        }

        જો (n) {
            પાછા NGX_DECLINED;
        }
    }

    /* exact hash */

    k = 0;

    માટે (i = 0; i < last; i++) {
        જો (!(flags & NGX_HASH_READONLY_ચાવી)) {
            ચાવી->data[i] = ngx_tolower(ચાવી->data[i]);
        }
        k = ngx_hash(k, ચાવી->data[i]);
    }

    k %= ha->hsize;

    /* check conflicts in exact hash */

    name = ha->ચાવીઓ_hash[k].elts;

    જો (name) {
        માટે (i = 0; i < ha->ચાવીઓ_hash[k].nelts; i++) {
            જો (last != name[i].len) {
                ચાલુ;
            }

            જો (ngx_strncmp(ચાવી->data, name[i].data, last) == 0) {
                પાછા NGX_BUSY;
            }
        }

    } નહીંતર {
        જો (ngx_array_init(&ha->ચાવીઓ_hash[k], ha->temp_pool, 4,
                           કદ(ngx_str_t))
            != NGX_OK)
        {
            પાછા NGX_ERROR;
        }
    }

    name = ngx_array_ધકેલો(&ha->ચાવીઓ_hash[k]);
    જો (name == ખાલી) {
        પાછા NGX_ERROR;
    }

    *name = *ચાવી;

    hk = ngx_array_ધકેલો(&ha->ચાવીઓ);
    જો (hk == ખાલી) {
        પાછા NGX_ERROR;
    }

    hk->ચાવી = *ચાવી;
    hk->ચાવી_hash = ngx_hash_ચાવી(ચાવી->data, last);
    hk->કિંમત = કિંમત;

    પાછા NGX_OK;


wildcard:

    /* wildcard hash */

    k = ngx_hash_strlow(&ચાવી->data[skip], &ચાવી->data[skip], last - skip);

    k %= ha->hsize;

    જો (skip == 1) {

        /* check conflicts in exact hash for ".example.com" */

        name = ha->ચાવીઓ_hash[k].elts;

        જો (name) {
            len = last - skip;

            માટે (i = 0; i < ha->ચાવીઓ_hash[k].nelts; i++) {
                જો (len != name[i].len) {
                    ચાલુ;
                }

                જો (ngx_strncmp(&ચાવી->data[1], name[i].data, len) == 0) {
                    પાછા NGX_BUSY;
                }
            }

        } નહીંતર {
            જો (ngx_array_init(&ha->ચાવીઓ_hash[k], ha->temp_pool, 4,
                               કદ(ngx_str_t))
                != NGX_OK)
            {
                પાછા NGX_ERROR;
            }
        }

        name = ngx_array_ધકેલો(&ha->ચાવીઓ_hash[k]);
        જો (name == ખાલી) {
            પાછા NGX_ERROR;
        }

        name->len = last - 1;
        name->data = ngx_pnalloc(ha->temp_pool, name->len);
        જો (name->data == ખાલી) {
            પાછા NGX_ERROR;
        }

        ngx_memcpy(name->data, &ચાવી->data[1], name->len);
    }


    જો (skip) {

        /*
         * convert "*.example.com" to "com.example.\0"
         *      and ".example.com" to "com.example\0"
         */

        p = ngx_pnalloc(ha->temp_pool, last);
        જો (p == ખાલી) {
            પાછા NGX_ERROR;
        }

        len = 0;
        n = 0;

        માટે (i = last - 1; i; i--) {
            જો (ચાવી->data[i] == '.') {
                ngx_memcpy(&p[n], &ચાવી->data[i + 1], len);
                n += len;
                p[n++] = '.';
                len = 0;
                ચાલુ;
            }

            len++;
        }

        જો (len) {
            ngx_memcpy(&p[n], &ચાવી->data[1], len);
            n += len;
        }

        p[n] = '\0';

        hwc = &ha->dns_wc_head;
        ચાવીઓ = &ha->dns_wc_head_hash[k];

    } નહીંતર {

        /* convert "www.example.*" to "www.example\0" */

        last++;

        p = ngx_pnalloc(ha->temp_pool, last);
        જો (p == ખાલી) {
            પાછા NGX_ERROR;
        }

        ngx_cpystrn(p, ચાવી->data, last);

        hwc = &ha->dns_wc_tail;
        ચાવીઓ = &ha->dns_wc_tail_hash[k];
    }


    /* check conflicts in wildcard hash */

    name = ચાવીઓ->elts;

    જો (name) {
        len = last - skip;

        માટે (i = 0; i < ચાવીઓ->nelts; i++) {
            જો (len != name[i].len) {
                ચાલુ;
            }

            જો (ngx_strncmp(ચાવી->data + skip, name[i].data, len) == 0) {
                પાછા NGX_BUSY;
            }
        }

    } નહીંતર {
        જો (ngx_array_init(ચાવીઓ, ha->temp_pool, 4, કદ(ngx_str_t)) != NGX_OK)
        {
            પાછા NGX_ERROR;
        }
    }

    name = ngx_array_ધકેલો(ચાવીઓ);
    જો (name == ખાલી) {
        પાછા NGX_ERROR;
    }

    name->len = last - skip;
    name->data = ngx_pnalloc(ha->temp_pool, name->len);
    જો (name->data == ખાલી) {
        પાછા NGX_ERROR;
    }

    ngx_memcpy(name->data, ચાવી->data + skip, name->len);


    /* add to wildcard hash */

    hk = ngx_array_ધકેલો(hwc);
    જો (hk == ખાલી) {
        પાછા NGX_ERROR;
    }

    hk->ચાવી.len = last - 1;
    hk->ચાવી.data = p;
    hk->ચાવી_hash = 0;
    hk->કિંમત = કિંમત;

    પાછા NGX_OK;
}
