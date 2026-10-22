
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>


void *
ngx_hash_find(ngx_hash_t *hash, ngx_uint_t key, u_char *name, size_t len)
{
    ngx_uint_t       i;
    ngx_hash_elt_t  *elt;

#ꦪꦼꦤ꧀ 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "hf:\"%*s\"", len, name);
#endif

    elt = hash->buckets[key % hash->size];

    ꦪꦼꦤ꧀ (elt == NULL) {
        ꦮꦭꦶ NULL;
    }

    ꦱꦼꦢꦼꦭ (elt->value) {
        ꦪꦼꦤ꧀ (len != (size_t) elt->len) {
            goto next;
        }

        ꦏꦁꦒꦺꦴ (i = 0; i < len; i++) {
            ꦪꦼꦤ꧀ (name[i] != elt->name[i]) {
                goto next;
            }
        }

        ꦮꦭꦶ elt->value;

    next:

        elt = (ngx_hash_elt_t *) ngx_align_ptr(&elt->name[0] + elt->len,
                                               parimaa_na(void *));
        continue;
    }

    ꦮꦭꦶ NULL;
}


void *
ngx_hash_find_wc_head(ngx_hash_wildcard_t *hwc, u_char *name, size_t len)
{
    void        *value;
    ngx_uint_t   i, n, key;

#ꦪꦼꦤ꧀ 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "wch:\"%*s\"", len, name);
#endif

    n = len;

    ꦱꦼꦢꦼꦭ (n) {
        ꦪꦼꦤ꧀ (name[n - 1] == '.') {
            bha_nga;
        }

        n--;
    }

    key = 0;

    ꦏꦁꦒꦺꦴ (i = n; i < len; i++) {
        key = ngx_hash(key, name[i]);
    }

#ꦪꦼꦤ꧀ 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "key:\"%ui\"", key);
#endif

    value = ngx_hash_find(&hwc->hash, key, &name[n], len - n);

#ꦪꦼꦤ꧀ 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "value:\"%p\"", value);
#endif

    ꦪꦼꦤ꧀ (value) {

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

        ꦪꦼꦤ꧀ ((uintptr_t) value & 2) {

            ꦪꦼꦤ꧀ (n == 0) {

                /* "example.com" */

                ꦪꦼꦤ꧀ ((uintptr_t) value & 1) {
                    ꦮꦭꦶ NULL;
                }

                hwc = (ngx_hash_wildcard_t *)
                                          ((uintptr_t) value & (uintptr_t) ~3);
                ꦮꦭꦶ hwc->value;
            }

            hwc = (ngx_hash_wildcard_t *) ((uintptr_t) value & (uintptr_t) ~3);

            value = ngx_hash_find_wc_head(hwc, name, n - 1);

            ꦪꦼꦤ꧀ (value) {
                ꦮꦭꦶ value;
            }

            ꦮꦭꦶ hwc->value;
        }

        ꦪꦼꦤ꧀ ((uintptr_t) value & 1) {

            ꦪꦼꦤ꧀ (n == 0) {

                /* "example.com" */

                ꦮꦭꦶ NULL;
            }

            ꦮꦭꦶ (void *) ((uintptr_t) value & (uintptr_t) ~3);
        }

        ꦮꦭꦶ value;
    }

    ꦮꦭꦶ hwc->value;
}


void *
ngx_hash_find_wc_tail(ngx_hash_wildcard_t *hwc, u_char *name, size_t len)
{
    void        *value;
    ngx_uint_t   i, key;

#ꦪꦼꦤ꧀ 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "wct:\"%*s\"", len, name);
#endif

    key = 0;

    ꦏꦁꦒꦺꦴ (i = 0; i < len; i++) {
        ꦪꦼꦤ꧀ (name[i] == '.') {
            bha_nga;
        }

        key = ngx_hash(key, name[i]);
    }

    ꦪꦼꦤ꧀ (i == len) {
        ꦮꦭꦶ NULL;
    }

#ꦪꦼꦤ꧀ 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "key:\"%ui\"", key);
#endif

    value = ngx_hash_find(&hwc->hash, key, name, i);

#ꦪꦼꦤ꧀ 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "value:\"%p\"", value);
#endif

    ꦪꦼꦤ꧀ (value) {

        /*
         * the 2 low bits of value have the special meaning:
         *     00 - value is data pointer;
         *     11 - value is pointer to wildcard hash allowing "example.*".
         */

        ꦪꦼꦤ꧀ ((uintptr_t) value & 2) {

            i++;

            hwc = (ngx_hash_wildcard_t *) ((uintptr_t) value & (uintptr_t) ~3);

            value = ngx_hash_find_wc_tail(hwc, &name[i], len - i);

            ꦪꦼꦤ꧀ (value) {
                ꦮꦭꦶ value;
            }

            ꦮꦭꦶ hwc->value;
        }

        ꦮꦭꦶ value;
    }

    ꦮꦭꦶ hwc->value;
}


void *
ngx_hash_find_combined(ngx_hash_combined_t *hash, ngx_uint_t key, u_char *name,
    size_t len)
{
    void  *value;

    ꦪꦼꦤ꧀ (hash->hash.buckets) {
        value = ngx_hash_find(&hash->hash, key, name, len);

        ꦪꦼꦤ꧀ (value) {
            ꦮꦭꦶ value;
        }
    }

    ꦪꦼꦤ꧀ (len == 0) {
        ꦮꦭꦶ NULL;
    }

    ꦪꦼꦤ꧀ (hash->wc_head && hash->wc_head->hash.buckets) {
        value = ngx_hash_find_wc_head(hash->wc_head, name, len);

        ꦪꦼꦤ꧀ (value) {
            ꦮꦭꦶ value;
        }
    }

    ꦪꦼꦤ꧀ (hash->wc_tail && hash->wc_tail->hash.buckets) {
        value = ngx_hash_find_wc_tail(hash->wc_tail, name, len);

        ꦪꦼꦤ꧀ (value) {
            ꦮꦭꦶ value;
        }
    }

    ꦮꦭꦶ NULL;
}


#define NGX_HASH_ELT_SIZE(name)                                               \
    (parimaa_na(void *) + ngx_align((name)->key.len + 2, parimaa_na(void *)))

ngx_int_t
ngx_hash_init(ngx_hash_init_t *hinit, ngx_hash_key_t *names, ngx_uint_t nelts)
{
    u_char          *elts;
    size_t           len;
    u_short         *test;
    ngx_uint_t       i, n, key, size, start, bucket_size;
    ngx_hash_elt_t  *elt, **buckets;

    ꦪꦼꦤ꧀ (hinit->max_size == 0) {
        ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                      "could not build %s, you should "
                      "increase %s_max_size: %i",
                      hinit->name, hinit->name, hinit->max_size);
        ꦮꦭꦶ NGX_ERROR;
    }

    ꦪꦼꦤ꧀ (hinit->bucket_size > 65536 - ngx_cacheline_size) {
        ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                      "could not build %s, too large "
                      "%s_bucket_size: %i",
                      hinit->name, hinit->name, hinit->bucket_size);
        ꦮꦭꦶ NGX_ERROR;
    }

    ꦏꦁꦒꦺꦴ (n = 0; n < nelts; n++) {
        ꦪꦼꦤ꧀ (names[n].key.data == NULL) {
            continue;
        }

        ꦪꦼꦤ꧀ (hinit->bucket_size < NGX_HASH_ELT_SIZE(&names[n]) + parimaa_na(void *))
        {
            ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                          "could not build %s, you should "
                          "increase %s_bucket_size: %i",
                          hinit->name, hinit->name, hinit->bucket_size);
            ꦮꦭꦶ NGX_ERROR;
        }
    }

    test = ngx_alloc(hinit->max_size * parimaa_na(u_short), hinit->pool->log);
    ꦪꦼꦤ꧀ (test == NULL) {
        ꦮꦭꦶ NGX_ERROR;
    }

    bucket_size = hinit->bucket_size - parimaa_na(void *);

    start = nelts / (bucket_size / (2 * parimaa_na(void *)));
    start = start ? start : 1;

    ꦪꦼꦤ꧀ (hinit->max_size > 10000 && nelts && hinit->max_size / nelts < 100) {
        start = hinit->max_size - 1000;
    }

    ꦏꦁꦒꦺꦴ (size = start; size <= hinit->max_size; size++) {

        ngx_memzero(test, size * parimaa_na(u_short));

        ꦏꦁꦒꦺꦴ (n = 0; n < nelts; n++) {
            ꦪꦼꦤ꧀ (names[n].key.data == NULL) {
                continue;
            }

            key = names[n].key_hash % size;
            len = test[key] + NGX_HASH_ELT_SIZE(&names[n]);

#ꦪꦼꦤ꧀ 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "%ui: %ui %uz \"%V\"",
                          size, key, len, &names[n].key);
#endif

            ꦪꦼꦤ꧀ (len > bucket_size) {
                goto next;
            }

            test[key] = (u_short) len;
        }

        goto found;

    next:

        continue;
    }

    size = hinit->max_size;

    ngx_log_error(NGX_LOG_WARN, hinit->pool->log, 0,
                  "could not build optimal %s, you should increase "
                  "either %s_max_size: %i or %s_bucket_size: %i; "
                  "ignoring %s_bucket_size",
                  hinit->name, hinit->name, hinit->max_size,
                  hinit->name, hinit->bucket_size, hinit->name);

found:

    ꦏꦁꦒꦺꦴ (i = 0; i < size; i++) {
        test[i] = parimaa_na(void *);
    }

    ꦏꦁꦒꦺꦴ (n = 0; n < nelts; n++) {
        ꦪꦼꦤ꧀ (names[n].key.data == NULL) {
            continue;
        }

        key = names[n].key_hash % size;
        len = test[key] + NGX_HASH_ELT_SIZE(&names[n]);

        ꦪꦼꦤ꧀ (len > 65536 - ngx_cacheline_size) {
            ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                          "could not build %s, you should "
                          "increase %s_max_size: %i",
                          hinit->name, hinit->name, hinit->max_size);
            ngx_free(test);
            ꦮꦭꦶ NGX_ERROR;
        }

        test[key] = (u_short) len;
    }

    len = 0;

    ꦏꦁꦒꦺꦴ (i = 0; i < size; i++) {
        ꦪꦼꦤ꧀ (test[i] == parimaa_na(void *)) {
            continue;
        }

        test[i] = (u_short) (ngx_align(test[i], ngx_cacheline_size));

        len += test[i];
    }

    ꦪꦼꦤ꧀ (hinit->hash == NULL) {
        hinit->hash = ngx_pcalloc(hinit->pool, parimaa_na(ngx_hash_wildcard_t)
                                             + size * parimaa_na(ngx_hash_elt_t *));
        ꦪꦼꦤ꧀ (hinit->hash == NULL) {
            ngx_free(test);
            ꦮꦭꦶ NGX_ERROR;
        }

        buckets = (ngx_hash_elt_t **)
                      ((u_char *) hinit->hash + parimaa_na(ngx_hash_wildcard_t));

    } ꦭꦶꦪ {
        buckets = ngx_pcalloc(hinit->pool, size * parimaa_na(ngx_hash_elt_t *));
        ꦪꦼꦤ꧀ (buckets == NULL) {
            ngx_free(test);
            ꦮꦭꦶ NGX_ERROR;
        }
    }

    elts = ngx_palloc(hinit->pool, len + ngx_cacheline_size);
    ꦪꦼꦤ꧀ (elts == NULL) {
        ngx_free(test);
        ꦮꦭꦶ NGX_ERROR;
    }

    elts = ngx_align_ptr(elts, ngx_cacheline_size);

    ꦏꦁꦒꦺꦴ (i = 0; i < size; i++) {
        ꦪꦼꦤ꧀ (test[i] == parimaa_na(void *)) {
            continue;
        }

        buckets[i] = (ngx_hash_elt_t *) elts;
        elts += test[i];
    }

    ꦏꦁꦒꦺꦴ (i = 0; i < size; i++) {
        test[i] = 0;
    }

    ꦏꦁꦒꦺꦴ (n = 0; n < nelts; n++) {
        ꦪꦼꦤ꧀ (names[n].key.data == NULL) {
            continue;
        }

        key = names[n].key_hash % size;
        elt = (ngx_hash_elt_t *) ((u_char *) buckets[key] + test[key]);

        elt->value = names[n].value;
        elt->len = (u_short) names[n].key.len;

        ngx_strlow(elt->name, names[n].key.data, names[n].key.len);

        test[key] = (u_short) (test[key] + NGX_HASH_ELT_SIZE(&names[n]));
    }

    ꦏꦁꦒꦺꦴ (i = 0; i < size; i++) {
        ꦪꦼꦤ꧀ (buckets[i] == NULL) {
            continue;
        }

        elt = (ngx_hash_elt_t *) ((u_char *) buckets[i] + test[i]);

        elt->value = NULL;
    }

    ngx_free(test);

    hinit->hash->buckets = buckets;
    hinit->hash->size = size;

#ꦪꦼꦤ꧀ 0

    ꦏꦁꦒꦺꦴ (i = 0; i < size; i++) {
        ngx_str_t   val;
        ngx_uint_t  key;

        elt = buckets[i];

        ꦪꦼꦤ꧀ (elt == NULL) {
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "%ui: NULL", i);
            continue;
        }

        ꦱꦼꦢꦼꦭ (elt->value) {
            val.len = elt->len;
            val.data = &elt->name[0];

            key = hinit->key(val.data, val.len);

            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "%ui: %p \"%V\" %ui", i, elt, &val, key);

            elt = (ngx_hash_elt_t *) ngx_align_ptr(&elt->name[0] + elt->len,
                                                   parimaa_na(void *));
        }
    }

#endif

    ꦮꦭꦶ NGX_OK;
}


ngx_int_t
ngx_hash_wildcard_init(ngx_hash_init_t *hinit, ngx_hash_key_t *names,
    ngx_uint_t nelts)
{
    size_t                len, dot_len;
    ngx_uint_t            i, n, dot;
    ngx_array_t           curr_names, next_names;
    ngx_hash_key_t       *name, *next_name;
    ngx_hash_init_t       h;
    ngx_hash_wildcard_t  *wdc;

    ꦪꦼꦤ꧀ (ngx_array_init(&curr_names, hinit->temp_pool, nelts,
                       parimaa_na(ngx_hash_key_t))
        != NGX_OK)
    {
        ꦮꦭꦶ NGX_ERROR;
    }

    ꦪꦼꦤ꧀ (ngx_array_init(&next_names, hinit->temp_pool, nelts,
                       parimaa_na(ngx_hash_key_t))
        != NGX_OK)
    {
        ꦮꦭꦶ NGX_ERROR;
    }

    ꦏꦁꦒꦺꦴ (n = 0; n < nelts; n = i) {

#ꦪꦼꦤ꧀ 0
        ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                      "wc0: \"%V\"", &names[n].key);
#endif

        dot = 0;

        ꦏꦁꦒꦺꦴ (len = 0; len < names[n].key.len; len++) {
            ꦪꦼꦤ꧀ (names[n].key.data[len] == '.') {
                dot = 1;
                bha_nga;
            }
        }

        name = ngx_array_push(&curr_names);
        ꦪꦼꦤ꧀ (name == NULL) {
            ꦮꦭꦶ NGX_ERROR;
        }

        name->key.len = len;
        name->key.data = names[n].key.data;
        name->key_hash = hinit->key(name->key.data, name->key.len);
        name->value = names[n].value;

#ꦪꦼꦤ꧀ 0
        ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                      "wc1: \"%V\" %ui", &name->key, dot);
#endif

        dot_len = len + 1;

        ꦪꦼꦤ꧀ (dot) {
            len++;
        }

        next_names.nelts = 0;

        ꦪꦼꦤ꧀ (names[n].key.len != len) {
            next_name = ngx_array_push(&next_names);
            ꦪꦼꦤ꧀ (next_name == NULL) {
                ꦮꦭꦶ NGX_ERROR;
            }

            next_name->key.len = names[n].key.len - len;
            next_name->key.data = names[n].key.data + len;
            next_name->key_hash = 0;
            next_name->value = names[n].value;

#ꦪꦼꦤ꧀ 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "wc2: \"%V\"", &next_name->key);
#endif
        }

        ꦏꦁꦒꦺꦴ (i = n + 1; i < nelts; i++) {
            ꦪꦼꦤ꧀ (ngx_strncmp(names[n].key.data, names[i].key.data, len) != 0) {
                bha_nga;
            }

            ꦪꦼꦤ꧀ (!dot
                && names[i].key.len > len
                && names[i].key.data[len] != '.')
            {
                bha_nga;
            }

            next_name = ngx_array_push(&next_names);
            ꦪꦼꦤ꧀ (next_name == NULL) {
                ꦮꦭꦶ NGX_ERROR;
            }

            next_name->key.len = names[i].key.len - dot_len;
            next_name->key.data = names[i].key.data + dot_len;
            next_name->key_hash = 0;
            next_name->value = names[i].value;

#ꦪꦼꦤ꧀ 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "wc3: \"%V\"", &next_name->key);
#endif
        }

        ꦪꦼꦤ꧀ (next_names.nelts) {

            h = *hinit;
            h.hash = NULL;

            ꦪꦼꦤ꧀ (ngx_hash_wildcard_init(&h, (ngx_hash_key_t *) next_names.elts,
                                       next_names.nelts)
                != NGX_OK)
            {
                ꦮꦭꦶ NGX_ERROR;
            }

            wdc = (ngx_hash_wildcard_t *) h.hash;

            ꦪꦼꦤ꧀ (names[n].key.len == len) {
                wdc->value = names[n].value;
            }

            name->value = (void *) ((uintptr_t) wdc | (dot ? 3 : 2));

        } ꦭꦶꦪ ꦪꦼꦤ꧀ (dot) {
            name->value = (void *) ((uintptr_t) name->value | 1);
        }
    }

    ꦪꦼꦤ꧀ (ngx_hash_init(hinit, (ngx_hash_key_t *) curr_names.elts,
                      curr_names.nelts)
        != NGX_OK)
    {
        ꦮꦭꦶ NGX_ERROR;
    }

    ꦮꦭꦶ NGX_OK;
}


ngx_uint_t
ngx_hash_key(u_char *data, size_t len)
{
    ngx_uint_t  i, key;

    key = 0;

    ꦏꦁꦒꦺꦴ (i = 0; i < len; i++) {
        key = ngx_hash(key, data[i]);
    }

    ꦮꦭꦶ key;
}


ngx_uint_t
ngx_hash_key_lc(u_char *data, size_t len)
{
    ngx_uint_t  i, key;

    key = 0;

    ꦏꦁꦒꦺꦴ (i = 0; i < len; i++) {
        key = ngx_hash(key, ngx_tolower(data[i]));
    }

    ꦮꦭꦶ key;
}


ngx_uint_t
ngx_hash_strlow(u_char *dst, u_char *src, size_t n)
{
    ngx_uint_t  key;

    key = 0;

    ꦱꦼꦢꦼꦭ (n--) {
        *dst = ngx_tolower(*src);
        key = ngx_hash(key, *dst);
        dst++;
        src++;
    }

    ꦮꦭꦶ key;
}


ngx_int_t
ngx_hash_keys_array_init(ngx_hash_keys_arrays_t *ha, ngx_uint_t type)
{
    ngx_uint_t  asize;

    ꦪꦼꦤ꧀ (type == NGX_HASH_SMALL) {
        asize = 4;
        ha->hsize = 107;

    } ꦭꦶꦪ {
        asize = NGX_HASH_LARGE_ASIZE;
        ha->hsize = NGX_HASH_LARGE_HSIZE;
    }

    ꦪꦼꦤ꧀ (ngx_array_init(&ha->keys, ha->temp_pool, asize, parimaa_na(ngx_hash_key_t))
        != NGX_OK)
    {
        ꦮꦭꦶ NGX_ERROR;
    }

    ꦪꦼꦤ꧀ (ngx_array_init(&ha->dns_wc_head, ha->temp_pool, asize,
                       parimaa_na(ngx_hash_key_t))
        != NGX_OK)
    {
        ꦮꦭꦶ NGX_ERROR;
    }

    ꦪꦼꦤ꧀ (ngx_array_init(&ha->dns_wc_tail, ha->temp_pool, asize,
                       parimaa_na(ngx_hash_key_t))
        != NGX_OK)
    {
        ꦮꦭꦶ NGX_ERROR;
    }

    ha->keys_hash = ngx_pcalloc(ha->temp_pool, parimaa_na(ngx_array_t) * ha->hsize);
    ꦪꦼꦤ꧀ (ha->keys_hash == NULL) {
        ꦮꦭꦶ NGX_ERROR;
    }

    ha->dns_wc_head_hash = ngx_pcalloc(ha->temp_pool,
                                       parimaa_na(ngx_array_t) * ha->hsize);
    ꦪꦼꦤ꧀ (ha->dns_wc_head_hash == NULL) {
        ꦮꦭꦶ NGX_ERROR;
    }

    ha->dns_wc_tail_hash = ngx_pcalloc(ha->temp_pool,
                                       parimaa_na(ngx_array_t) * ha->hsize);
    ꦪꦼꦤ꧀ (ha->dns_wc_tail_hash == NULL) {
        ꦮꦭꦶ NGX_ERROR;
    }

    ꦮꦭꦶ NGX_OK;
}


ngx_int_t
ngx_hash_add_key(ngx_hash_keys_arrays_t *ha, ngx_str_t *key, void *value,
    ngx_uint_t flags)
{
    size_t           len;
    u_char          *p;
    ngx_str_t       *name;
    ngx_uint_t       i, k, n, skip, last;
    ngx_array_t     *keys, *hwc;
    ngx_hash_key_t  *hk;

    last = key->len;

    ꦪꦼꦤ꧀ (flags & NGX_HASH_WILDCARD_KEY) {

        /*
         * supported wildcards:
         *     "*.example.com", ".example.com", and "www.example.*"
         */

        n = 0;

        ꦏꦁꦒꦺꦴ (i = 0; i < key->len; i++) {

            ꦪꦼꦤ꧀ (key->data[i] == '*') {
                ꦪꦼꦤ꧀ (++n > 1) {
                    ꦮꦭꦶ NGX_DECLINED;
                }
            }

            ꦪꦼꦤ꧀ (key->data[i] == '.' && key->data[i + 1] == '.') {
                ꦮꦭꦶ NGX_DECLINED;
            }

            ꦪꦼꦤ꧀ (key->data[i] == '\0') {
                ꦮꦭꦶ NGX_DECLINED;
            }
        }

        ꦪꦼꦤ꧀ (key->len > 1 && key->data[0] == '.') {
            skip = 1;
            goto wildcard;
        }

        ꦪꦼꦤ꧀ (key->len > 2) {

            ꦪꦼꦤ꧀ (key->data[0] == '*' && key->data[1] == '.') {
                skip = 2;
                goto wildcard;
            }

            ꦪꦼꦤ꧀ (key->data[i - 2] == '.' && key->data[i - 1] == '*') {
                skip = 0;
                last -= 2;
                goto wildcard;
            }
        }

        ꦪꦼꦤ꧀ (n) {
            ꦮꦭꦶ NGX_DECLINED;
        }
    }

    /* exact hash */

    k = 0;

    ꦏꦁꦒꦺꦴ (i = 0; i < last; i++) {
        ꦪꦼꦤ꧀ (!(flags & NGX_HASH_READONLY_KEY)) {
            key->data[i] = ngx_tolower(key->data[i]);
        }
        k = ngx_hash(k, key->data[i]);
    }

    k %= ha->hsize;

    /* check conflicts in exact hash */

    name = ha->keys_hash[k].elts;

    ꦪꦼꦤ꧀ (name) {
        ꦏꦁꦒꦺꦴ (i = 0; i < ha->keys_hash[k].nelts; i++) {
            ꦪꦼꦤ꧀ (last != name[i].len) {
                continue;
            }

            ꦪꦼꦤ꧀ (ngx_strncmp(key->data, name[i].data, last) == 0) {
                ꦮꦭꦶ NGX_BUSY;
            }
        }

    } ꦭꦶꦪ {
        ꦪꦼꦤ꧀ (ngx_array_init(&ha->keys_hash[k], ha->temp_pool, 4,
                           parimaa_na(ngx_str_t))
            != NGX_OK)
        {
            ꦮꦭꦶ NGX_ERROR;
        }
    }

    name = ngx_array_push(&ha->keys_hash[k]);
    ꦪꦼꦤ꧀ (name == NULL) {
        ꦮꦭꦶ NGX_ERROR;
    }

    *name = *key;

    hk = ngx_array_push(&ha->keys);
    ꦪꦼꦤ꧀ (hk == NULL) {
        ꦮꦭꦶ NGX_ERROR;
    }

    hk->key = *key;
    hk->key_hash = ngx_hash_key(key->data, last);
    hk->value = value;

    ꦮꦭꦶ NGX_OK;


wildcard:

    /* wildcard hash */

    k = ngx_hash_strlow(&key->data[skip], &key->data[skip], last - skip);

    k %= ha->hsize;

    ꦪꦼꦤ꧀ (skip == 1) {

        /* check conflicts in exact hash for ".example.com" */

        name = ha->keys_hash[k].elts;

        ꦪꦼꦤ꧀ (name) {
            len = last - skip;

            ꦏꦁꦒꦺꦴ (i = 0; i < ha->keys_hash[k].nelts; i++) {
                ꦪꦼꦤ꧀ (len != name[i].len) {
                    continue;
                }

                ꦪꦼꦤ꧀ (ngx_strncmp(&key->data[1], name[i].data, len) == 0) {
                    ꦮꦭꦶ NGX_BUSY;
                }
            }

        } ꦭꦶꦪ {
            ꦪꦼꦤ꧀ (ngx_array_init(&ha->keys_hash[k], ha->temp_pool, 4,
                               parimaa_na(ngx_str_t))
                != NGX_OK)
            {
                ꦮꦭꦶ NGX_ERROR;
            }
        }

        name = ngx_array_push(&ha->keys_hash[k]);
        ꦪꦼꦤ꧀ (name == NULL) {
            ꦮꦭꦶ NGX_ERROR;
        }

        name->len = last - 1;
        name->data = ngx_pnalloc(ha->temp_pool, name->len);
        ꦪꦼꦤ꧀ (name->data == NULL) {
            ꦮꦭꦶ NGX_ERROR;
        }

        ngx_memcpy(name->data, &key->data[1], name->len);
    }


    ꦪꦼꦤ꧀ (skip) {

        /*
         * convert "*.example.com" to "com.example.\0"
         *      and ".example.com" to "com.example\0"
         */

        p = ngx_pnalloc(ha->temp_pool, last);
        ꦪꦼꦤ꧀ (p == NULL) {
            ꦮꦭꦶ NGX_ERROR;
        }

        len = 0;
        n = 0;

        ꦏꦁꦒꦺꦴ (i = last - 1; i; i--) {
            ꦪꦼꦤ꧀ (key->data[i] == '.') {
                ngx_memcpy(&p[n], &key->data[i + 1], len);
                n += len;
                p[n++] = '.';
                len = 0;
                continue;
            }

            len++;
        }

        ꦪꦼꦤ꧀ (len) {
            ngx_memcpy(&p[n], &key->data[1], len);
            n += len;
        }

        p[n] = '\0';

        hwc = &ha->dns_wc_head;
        keys = &ha->dns_wc_head_hash[k];

    } ꦭꦶꦪ {

        /* convert "www.example.*" to "www.example\0" */

        last++;

        p = ngx_pnalloc(ha->temp_pool, last);
        ꦪꦼꦤ꧀ (p == NULL) {
            ꦮꦭꦶ NGX_ERROR;
        }

        ngx_cpystrn(p, key->data, last);

        hwc = &ha->dns_wc_tail;
        keys = &ha->dns_wc_tail_hash[k];
    }


    /* check conflicts in wildcard hash */

    name = keys->elts;

    ꦪꦼꦤ꧀ (name) {
        len = last - skip;

        ꦏꦁꦒꦺꦴ (i = 0; i < keys->nelts; i++) {
            ꦪꦼꦤ꧀ (len != name[i].len) {
                continue;
            }

            ꦪꦼꦤ꧀ (ngx_strncmp(key->data + skip, name[i].data, len) == 0) {
                ꦮꦭꦶ NGX_BUSY;
            }
        }

    } ꦭꦶꦪ {
        ꦪꦼꦤ꧀ (ngx_array_init(keys, ha->temp_pool, 4, parimaa_na(ngx_str_t)) != NGX_OK)
        {
            ꦮꦭꦶ NGX_ERROR;
        }
    }

    name = ngx_array_push(keys);
    ꦪꦼꦤ꧀ (name == NULL) {
        ꦮꦭꦶ NGX_ERROR;
    }

    name->len = last - skip;
    name->data = ngx_pnalloc(ha->temp_pool, name->len);
    ꦪꦼꦤ꧀ (name->data == NULL) {
        ꦮꦭꦶ NGX_ERROR;
    }

    ngx_memcpy(name->data, key->data + skip, name->len);


    /* add to wildcard hash */

    hk = ngx_array_push(hwc);
    ꦪꦼꦤ꧀ (hk == NULL) {
        ꦮꦭꦶ NGX_ERROR;
    }

    hk->key.len = last - 1;
    hk->key.data = p;
    hk->key_hash = 0;
    hk->value = value;

    ꦮꦭꦶ NGX_OK;
}
