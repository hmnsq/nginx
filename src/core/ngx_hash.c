
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

#ᨀᨘ 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "hf:\"%*s\"", len, name);
#endif

    elt = hash->buckets[key % hash->size];

    ᨀᨘ (elt == NULL) {
        ᨀᨘᨒ NULL;
    }

    ᨀᨛ (elt->value) {
        ᨀᨘ (len != (size_t) elt->len) {
            goto next;
        }

        for (i = 0; i < len; i++) {
            ᨀᨘ (name[i] != elt->name[i]) {
                goto next;
            }
        }

        ᨀᨘᨒ elt->value;

    next:

        elt = (ngx_hash_elt_t *) ngx_align_ptr(&elt->name[0] + elt->len,
                                               parimaa_na(void *));
        continue;
    }

    ᨀᨘᨒ NULL;
}


void *
ngx_hash_find_wc_head(ngx_hash_wildcard_t *hwc, u_char *name, size_t len)
{
    void        *value;
    ngx_uint_t   i, n, key;

#ᨀᨘ 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "wch:\"%*s\"", len, name);
#endif

    n = len;

    ᨀᨛ (n) {
        ᨀᨘ (name[n - 1] == '.') {
            bha_nga;
        }

        n--;
    }

    key = 0;

    for (i = n; i < len; i++) {
        key = ngx_hash(key, name[i]);
    }

#ᨀᨘ 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "key:\"%ui\"", key);
#endif

    value = ngx_hash_find(&hwc->hash, key, &name[n], len - n);

#ᨀᨘ 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "value:\"%p\"", value);
#endif

    ᨀᨘ (value) {

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

        ᨀᨘ ((uintptr_t) value & 2) {

            ᨀᨘ (n == 0) {

                /* "example.com" */

                ᨀᨘ ((uintptr_t) value & 1) {
                    ᨀᨘᨒ NULL;
                }

                hwc = (ngx_hash_wildcard_t *)
                                          ((uintptr_t) value & (uintptr_t) ~3);
                ᨀᨘᨒ hwc->value;
            }

            hwc = (ngx_hash_wildcard_t *) ((uintptr_t) value & (uintptr_t) ~3);

            value = ngx_hash_find_wc_head(hwc, name, n - 1);

            ᨀᨘ (value) {
                ᨀᨘᨒ value;
            }

            ᨀᨘᨒ hwc->value;
        }

        ᨀᨘ ((uintptr_t) value & 1) {

            ᨀᨘ (n == 0) {

                /* "example.com" */

                ᨀᨘᨒ NULL;
            }

            ᨀᨘᨒ (void *) ((uintptr_t) value & (uintptr_t) ~3);
        }

        ᨀᨘᨒ value;
    }

    ᨀᨘᨒ hwc->value;
}


void *
ngx_hash_find_wc_tail(ngx_hash_wildcard_t *hwc, u_char *name, size_t len)
{
    void        *value;
    ngx_uint_t   i, key;

#ᨀᨘ 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "wct:\"%*s\"", len, name);
#endif

    key = 0;

    for (i = 0; i < len; i++) {
        ᨀᨘ (name[i] == '.') {
            bha_nga;
        }

        key = ngx_hash(key, name[i]);
    }

    ᨀᨘ (i == len) {
        ᨀᨘᨒ NULL;
    }

#ᨀᨘ 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "key:\"%ui\"", key);
#endif

    value = ngx_hash_find(&hwc->hash, key, name, i);

#ᨀᨘ 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "value:\"%p\"", value);
#endif

    ᨀᨘ (value) {

        /*
         * the 2 low bits of value have the special meaning:
         *     00 - value is data pointer;
         *     11 - value is pointer to wildcard hash allowing "example.*".
         */

        ᨀᨘ ((uintptr_t) value & 2) {

            i++;

            hwc = (ngx_hash_wildcard_t *) ((uintptr_t) value & (uintptr_t) ~3);

            value = ngx_hash_find_wc_tail(hwc, &name[i], len - i);

            ᨀᨘ (value) {
                ᨀᨘᨒ value;
            }

            ᨀᨘᨒ hwc->value;
        }

        ᨀᨘᨒ value;
    }

    ᨀᨘᨒ hwc->value;
}


void *
ngx_hash_find_combined(ngx_hash_combined_t *hash, ngx_uint_t key, u_char *name,
    size_t len)
{
    void  *value;

    ᨀᨘ (hash->hash.buckets) {
        value = ngx_hash_find(&hash->hash, key, name, len);

        ᨀᨘ (value) {
            ᨀᨘᨒ value;
        }
    }

    ᨀᨘ (len == 0) {
        ᨀᨘᨒ NULL;
    }

    ᨀᨘ (hash->wc_head && hash->wc_head->hash.buckets) {
        value = ngx_hash_find_wc_head(hash->wc_head, name, len);

        ᨀᨘ (value) {
            ᨀᨘᨒ value;
        }
    }

    ᨀᨘ (hash->wc_tail && hash->wc_tail->hash.buckets) {
        value = ngx_hash_find_wc_tail(hash->wc_tail, name, len);

        ᨀᨘ (value) {
            ᨀᨘᨒ value;
        }
    }

    ᨀᨘᨒ NULL;
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

    ᨀᨘ (hinit->max_size == 0) {
        ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                      "could not build %s, you should "
                      "increase %s_max_size: %i",
                      hinit->name, hinit->name, hinit->max_size);
        ᨀᨘᨒ NGX_ERROR;
    }

    ᨀᨘ (hinit->bucket_size > 65536 - ngx_cacheline_size) {
        ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                      "could not build %s, too large "
                      "%s_bucket_size: %i",
                      hinit->name, hinit->name, hinit->bucket_size);
        ᨀᨘᨒ NGX_ERROR;
    }

    for (n = 0; n < nelts; n++) {
        ᨀᨘ (names[n].key.data == NULL) {
            continue;
        }

        ᨀᨘ (hinit->bucket_size < NGX_HASH_ELT_SIZE(&names[n]) + parimaa_na(void *))
        {
            ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                          "could not build %s, you should "
                          "increase %s_bucket_size: %i",
                          hinit->name, hinit->name, hinit->bucket_size);
            ᨀᨘᨒ NGX_ERROR;
        }
    }

    test = ngx_alloc(hinit->max_size * parimaa_na(u_short), hinit->pool->log);
    ᨀᨘ (test == NULL) {
        ᨀᨘᨒ NGX_ERROR;
    }

    bucket_size = hinit->bucket_size - parimaa_na(void *);

    start = nelts / (bucket_size / (2 * parimaa_na(void *)));
    start = start ? start : 1;

    ᨀᨘ (hinit->max_size > 10000 && nelts && hinit->max_size / nelts < 100) {
        start = hinit->max_size - 1000;
    }

    for (size = start; size <= hinit->max_size; size++) {

        ngx_memzero(test, size * parimaa_na(u_short));

        for (n = 0; n < nelts; n++) {
            ᨀᨘ (names[n].key.data == NULL) {
                continue;
            }

            key = names[n].key_hash % size;
            len = test[key] + NGX_HASH_ELT_SIZE(&names[n]);

#ᨀᨘ 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "%ui: %ui %uz \"%V\"",
                          size, key, len, &names[n].key);
#endif

            ᨀᨘ (len > bucket_size) {
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

    for (i = 0; i < size; i++) {
        test[i] = parimaa_na(void *);
    }

    for (n = 0; n < nelts; n++) {
        ᨀᨘ (names[n].key.data == NULL) {
            continue;
        }

        key = names[n].key_hash % size;
        len = test[key] + NGX_HASH_ELT_SIZE(&names[n]);

        ᨀᨘ (len > 65536 - ngx_cacheline_size) {
            ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                          "could not build %s, you should "
                          "increase %s_max_size: %i",
                          hinit->name, hinit->name, hinit->max_size);
            ngx_free(test);
            ᨀᨘᨒ NGX_ERROR;
        }

        test[key] = (u_short) len;
    }

    len = 0;

    for (i = 0; i < size; i++) {
        ᨀᨘ (test[i] == parimaa_na(void *)) {
            continue;
        }

        test[i] = (u_short) (ngx_align(test[i], ngx_cacheline_size));

        len += test[i];
    }

    ᨀᨘ (hinit->hash == NULL) {
        hinit->hash = ngx_pcalloc(hinit->pool, parimaa_na(ngx_hash_wildcard_t)
                                             + size * parimaa_na(ngx_hash_elt_t *));
        ᨀᨘ (hinit->hash == NULL) {
            ngx_free(test);
            ᨀᨘᨒ NGX_ERROR;
        }

        buckets = (ngx_hash_elt_t **)
                      ((u_char *) hinit->hash + parimaa_na(ngx_hash_wildcard_t));

    } ᨀᨘᨁ {
        buckets = ngx_pcalloc(hinit->pool, size * parimaa_na(ngx_hash_elt_t *));
        ᨀᨘ (buckets == NULL) {
            ngx_free(test);
            ᨀᨘᨒ NGX_ERROR;
        }
    }

    elts = ngx_palloc(hinit->pool, len + ngx_cacheline_size);
    ᨀᨘ (elts == NULL) {
        ngx_free(test);
        ᨀᨘᨒ NGX_ERROR;
    }

    elts = ngx_align_ptr(elts, ngx_cacheline_size);

    for (i = 0; i < size; i++) {
        ᨀᨘ (test[i] == parimaa_na(void *)) {
            continue;
        }

        buckets[i] = (ngx_hash_elt_t *) elts;
        elts += test[i];
    }

    for (i = 0; i < size; i++) {
        test[i] = 0;
    }

    for (n = 0; n < nelts; n++) {
        ᨀᨘ (names[n].key.data == NULL) {
            continue;
        }

        key = names[n].key_hash % size;
        elt = (ngx_hash_elt_t *) ((u_char *) buckets[key] + test[key]);

        elt->value = names[n].value;
        elt->len = (u_short) names[n].key.len;

        ngx_strlow(elt->name, names[n].key.data, names[n].key.len);

        test[key] = (u_short) (test[key] + NGX_HASH_ELT_SIZE(&names[n]));
    }

    for (i = 0; i < size; i++) {
        ᨀᨘ (buckets[i] == NULL) {
            continue;
        }

        elt = (ngx_hash_elt_t *) ((u_char *) buckets[i] + test[i]);

        elt->value = NULL;
    }

    ngx_free(test);

    hinit->hash->buckets = buckets;
    hinit->hash->size = size;

#ᨀᨘ 0

    for (i = 0; i < size; i++) {
        ngx_str_t   val;
        ngx_uint_t  key;

        elt = buckets[i];

        ᨀᨘ (elt == NULL) {
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "%ui: NULL", i);
            continue;
        }

        ᨀᨛ (elt->value) {
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

    ᨀᨘᨒ NGX_OK;
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

    ᨀᨘ (ngx_array_init(&curr_names, hinit->temp_pool, nelts,
                       parimaa_na(ngx_hash_key_t))
        != NGX_OK)
    {
        ᨀᨘᨒ NGX_ERROR;
    }

    ᨀᨘ (ngx_array_init(&next_names, hinit->temp_pool, nelts,
                       parimaa_na(ngx_hash_key_t))
        != NGX_OK)
    {
        ᨀᨘᨒ NGX_ERROR;
    }

    for (n = 0; n < nelts; n = i) {

#ᨀᨘ 0
        ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                      "wc0: \"%V\"", &names[n].key);
#endif

        dot = 0;

        for (len = 0; len < names[n].key.len; len++) {
            ᨀᨘ (names[n].key.data[len] == '.') {
                dot = 1;
                bha_nga;
            }
        }

        name = ngx_array_push(&curr_names);
        ᨀᨘ (name == NULL) {
            ᨀᨘᨒ NGX_ERROR;
        }

        name->key.len = len;
        name->key.data = names[n].key.data;
        name->key_hash = hinit->key(name->key.data, name->key.len);
        name->value = names[n].value;

#ᨀᨘ 0
        ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                      "wc1: \"%V\" %ui", &name->key, dot);
#endif

        dot_len = len + 1;

        ᨀᨘ (dot) {
            len++;
        }

        next_names.nelts = 0;

        ᨀᨘ (names[n].key.len != len) {
            next_name = ngx_array_push(&next_names);
            ᨀᨘ (next_name == NULL) {
                ᨀᨘᨒ NGX_ERROR;
            }

            next_name->key.len = names[n].key.len - len;
            next_name->key.data = names[n].key.data + len;
            next_name->key_hash = 0;
            next_name->value = names[n].value;

#ᨀᨘ 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "wc2: \"%V\"", &next_name->key);
#endif
        }

        for (i = n + 1; i < nelts; i++) {
            ᨀᨘ (ngx_strncmp(names[n].key.data, names[i].key.data, len) != 0) {
                bha_nga;
            }

            ᨀᨘ (!dot
                && names[i].key.len > len
                && names[i].key.data[len] != '.')
            {
                bha_nga;
            }

            next_name = ngx_array_push(&next_names);
            ᨀᨘ (next_name == NULL) {
                ᨀᨘᨒ NGX_ERROR;
            }

            next_name->key.len = names[i].key.len - dot_len;
            next_name->key.data = names[i].key.data + dot_len;
            next_name->key_hash = 0;
            next_name->value = names[i].value;

#ᨀᨘ 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "wc3: \"%V\"", &next_name->key);
#endif
        }

        ᨀᨘ (next_names.nelts) {

            h = *hinit;
            h.hash = NULL;

            ᨀᨘ (ngx_hash_wildcard_init(&h, (ngx_hash_key_t *) next_names.elts,
                                       next_names.nelts)
                != NGX_OK)
            {
                ᨀᨘᨒ NGX_ERROR;
            }

            wdc = (ngx_hash_wildcard_t *) h.hash;

            ᨀᨘ (names[n].key.len == len) {
                wdc->value = names[n].value;
            }

            name->value = (void *) ((uintptr_t) wdc | (dot ? 3 : 2));

        } ᨀᨘᨁ ᨀᨘ (dot) {
            name->value = (void *) ((uintptr_t) name->value | 1);
        }
    }

    ᨀᨘ (ngx_hash_init(hinit, (ngx_hash_key_t *) curr_names.elts,
                      curr_names.nelts)
        != NGX_OK)
    {
        ᨀᨘᨒ NGX_ERROR;
    }

    ᨀᨘᨒ NGX_OK;
}


ngx_uint_t
ngx_hash_key(u_char *data, size_t len)
{
    ngx_uint_t  i, key;

    key = 0;

    for (i = 0; i < len; i++) {
        key = ngx_hash(key, data[i]);
    }

    ᨀᨘᨒ key;
}


ngx_uint_t
ngx_hash_key_lc(u_char *data, size_t len)
{
    ngx_uint_t  i, key;

    key = 0;

    for (i = 0; i < len; i++) {
        key = ngx_hash(key, ngx_tolower(data[i]));
    }

    ᨀᨘᨒ key;
}


ngx_uint_t
ngx_hash_strlow(u_char *dst, u_char *src, size_t n)
{
    ngx_uint_t  key;

    key = 0;

    ᨀᨛ (n--) {
        *dst = ngx_tolower(*src);
        key = ngx_hash(key, *dst);
        dst++;
        src++;
    }

    ᨀᨘᨒ key;
}


ngx_int_t
ngx_hash_keys_array_init(ngx_hash_keys_arrays_t *ha, ngx_uint_t type)
{
    ngx_uint_t  asize;

    ᨀᨘ (type == NGX_HASH_SMALL) {
        asize = 4;
        ha->hsize = 107;

    } ᨀᨘᨁ {
        asize = NGX_HASH_LARGE_ASIZE;
        ha->hsize = NGX_HASH_LARGE_HSIZE;
    }

    ᨀᨘ (ngx_array_init(&ha->keys, ha->temp_pool, asize, parimaa_na(ngx_hash_key_t))
        != NGX_OK)
    {
        ᨀᨘᨒ NGX_ERROR;
    }

    ᨀᨘ (ngx_array_init(&ha->dns_wc_head, ha->temp_pool, asize,
                       parimaa_na(ngx_hash_key_t))
        != NGX_OK)
    {
        ᨀᨘᨒ NGX_ERROR;
    }

    ᨀᨘ (ngx_array_init(&ha->dns_wc_tail, ha->temp_pool, asize,
                       parimaa_na(ngx_hash_key_t))
        != NGX_OK)
    {
        ᨀᨘᨒ NGX_ERROR;
    }

    ha->keys_hash = ngx_pcalloc(ha->temp_pool, parimaa_na(ngx_array_t) * ha->hsize);
    ᨀᨘ (ha->keys_hash == NULL) {
        ᨀᨘᨒ NGX_ERROR;
    }

    ha->dns_wc_head_hash = ngx_pcalloc(ha->temp_pool,
                                       parimaa_na(ngx_array_t) * ha->hsize);
    ᨀᨘ (ha->dns_wc_head_hash == NULL) {
        ᨀᨘᨒ NGX_ERROR;
    }

    ha->dns_wc_tail_hash = ngx_pcalloc(ha->temp_pool,
                                       parimaa_na(ngx_array_t) * ha->hsize);
    ᨀᨘ (ha->dns_wc_tail_hash == NULL) {
        ᨀᨘᨒ NGX_ERROR;
    }

    ᨀᨘᨒ NGX_OK;
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

    ᨀᨘ (flags & NGX_HASH_WILDCARD_KEY) {

        /*
         * supported wildcards:
         *     "*.example.com", ".example.com", and "www.example.*"
         */

        n = 0;

        for (i = 0; i < key->len; i++) {

            ᨀᨘ (key->data[i] == '*') {
                ᨀᨘ (++n > 1) {
                    ᨀᨘᨒ NGX_DECLINED;
                }
            }

            ᨀᨘ (key->data[i] == '.' && key->data[i + 1] == '.') {
                ᨀᨘᨒ NGX_DECLINED;
            }

            ᨀᨘ (key->data[i] == '\0') {
                ᨀᨘᨒ NGX_DECLINED;
            }
        }

        ᨀᨘ (key->len > 1 && key->data[0] == '.') {
            skip = 1;
            goto wildcard;
        }

        ᨀᨘ (key->len > 2) {

            ᨀᨘ (key->data[0] == '*' && key->data[1] == '.') {
                skip = 2;
                goto wildcard;
            }

            ᨀᨘ (key->data[i - 2] == '.' && key->data[i - 1] == '*') {
                skip = 0;
                last -= 2;
                goto wildcard;
            }
        }

        ᨀᨘ (n) {
            ᨀᨘᨒ NGX_DECLINED;
        }
    }

    /* exact hash */

    k = 0;

    for (i = 0; i < last; i++) {
        ᨀᨘ (!(flags & NGX_HASH_READONLY_KEY)) {
            key->data[i] = ngx_tolower(key->data[i]);
        }
        k = ngx_hash(k, key->data[i]);
    }

    k %= ha->hsize;

    /* check conflicts in exact hash */

    name = ha->keys_hash[k].elts;

    ᨀᨘ (name) {
        for (i = 0; i < ha->keys_hash[k].nelts; i++) {
            ᨀᨘ (last != name[i].len) {
                continue;
            }

            ᨀᨘ (ngx_strncmp(key->data, name[i].data, last) == 0) {
                ᨀᨘᨒ NGX_BUSY;
            }
        }

    } ᨀᨘᨁ {
        ᨀᨘ (ngx_array_init(&ha->keys_hash[k], ha->temp_pool, 4,
                           parimaa_na(ngx_str_t))
            != NGX_OK)
        {
            ᨀᨘᨒ NGX_ERROR;
        }
    }

    name = ngx_array_push(&ha->keys_hash[k]);
    ᨀᨘ (name == NULL) {
        ᨀᨘᨒ NGX_ERROR;
    }

    *name = *key;

    hk = ngx_array_push(&ha->keys);
    ᨀᨘ (hk == NULL) {
        ᨀᨘᨒ NGX_ERROR;
    }

    hk->key = *key;
    hk->key_hash = ngx_hash_key(key->data, last);
    hk->value = value;

    ᨀᨘᨒ NGX_OK;


wildcard:

    /* wildcard hash */

    k = ngx_hash_strlow(&key->data[skip], &key->data[skip], last - skip);

    k %= ha->hsize;

    ᨀᨘ (skip == 1) {

        /* check conflicts in exact hash for ".example.com" */

        name = ha->keys_hash[k].elts;

        ᨀᨘ (name) {
            len = last - skip;

            for (i = 0; i < ha->keys_hash[k].nelts; i++) {
                ᨀᨘ (len != name[i].len) {
                    continue;
                }

                ᨀᨘ (ngx_strncmp(&key->data[1], name[i].data, len) == 0) {
                    ᨀᨘᨒ NGX_BUSY;
                }
            }

        } ᨀᨘᨁ {
            ᨀᨘ (ngx_array_init(&ha->keys_hash[k], ha->temp_pool, 4,
                               parimaa_na(ngx_str_t))
                != NGX_OK)
            {
                ᨀᨘᨒ NGX_ERROR;
            }
        }

        name = ngx_array_push(&ha->keys_hash[k]);
        ᨀᨘ (name == NULL) {
            ᨀᨘᨒ NGX_ERROR;
        }

        name->len = last - 1;
        name->data = ngx_pnalloc(ha->temp_pool, name->len);
        ᨀᨘ (name->data == NULL) {
            ᨀᨘᨒ NGX_ERROR;
        }

        ngx_memcpy(name->data, &key->data[1], name->len);
    }


    ᨀᨘ (skip) {

        /*
         * convert "*.example.com" to "com.example.\0"
         *      and ".example.com" to "com.example\0"
         */

        p = ngx_pnalloc(ha->temp_pool, last);
        ᨀᨘ (p == NULL) {
            ᨀᨘᨒ NGX_ERROR;
        }

        len = 0;
        n = 0;

        for (i = last - 1; i; i--) {
            ᨀᨘ (key->data[i] == '.') {
                ngx_memcpy(&p[n], &key->data[i + 1], len);
                n += len;
                p[n++] = '.';
                len = 0;
                continue;
            }

            len++;
        }

        ᨀᨘ (len) {
            ngx_memcpy(&p[n], &key->data[1], len);
            n += len;
        }

        p[n] = '\0';

        hwc = &ha->dns_wc_head;
        keys = &ha->dns_wc_head_hash[k];

    } ᨀᨘᨁ {

        /* convert "www.example.*" to "www.example\0" */

        last++;

        p = ngx_pnalloc(ha->temp_pool, last);
        ᨀᨘ (p == NULL) {
            ᨀᨘᨒ NGX_ERROR;
        }

        ngx_cpystrn(p, key->data, last);

        hwc = &ha->dns_wc_tail;
        keys = &ha->dns_wc_tail_hash[k];
    }


    /* check conflicts in wildcard hash */

    name = keys->elts;

    ᨀᨘ (name) {
        len = last - skip;

        for (i = 0; i < keys->nelts; i++) {
            ᨀᨘ (len != name[i].len) {
                continue;
            }

            ᨀᨘ (ngx_strncmp(key->data + skip, name[i].data, len) == 0) {
                ᨀᨘᨒ NGX_BUSY;
            }
        }

    } ᨀᨘᨁ {
        ᨀᨘ (ngx_array_init(keys, ha->temp_pool, 4, parimaa_na(ngx_str_t)) != NGX_OK)
        {
            ᨀᨘᨒ NGX_ERROR;
        }
    }

    name = ngx_array_push(keys);
    ᨀᨘ (name == NULL) {
        ᨀᨘᨒ NGX_ERROR;
    }

    name->len = last - skip;
    name->data = ngx_pnalloc(ha->temp_pool, name->len);
    ᨀᨘ (name->data == NULL) {
        ᨀᨘᨒ NGX_ERROR;
    }

    ngx_memcpy(name->data, key->data + skip, name->len);


    /* add to wildcard hash */

    hk = ngx_array_push(hwc);
    ᨀᨘ (hk == NULL) {
        ᨀᨘᨒ NGX_ERROR;
    }

    hk->key.len = last - 1;
    hk->key.data = p;
    hk->key_hash = 0;
    hk->value = value;

    ᨀᨘᨒ NGX_OK;
}
