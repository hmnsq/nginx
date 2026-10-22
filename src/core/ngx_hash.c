
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

#ຖ້າ 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "hf:\"%*s\"", len, name);
#endif

    elt = hash->buckets[key % hash->size];

    ຖ້າ (elt == NULL) {
        ສົ່ງຄືນ NULL;
    }

    ໃນຂະນະທີ່ (elt->value) {
        ຖ້າ (len != (size_t) elt->len) {
            goto next;
        }

        ສຳລັບ (i = 0; i < len; i++) {
            ຖ້າ (name[i] != elt->name[i]) {
                goto next;
            }
        }

        ສົ່ງຄືນ elt->value;

    next:

        elt = (ngx_hash_elt_t *) ngx_align_ptr(&elt->name[0] + elt->len,
                                               parimaa_na(void *));
        continue;
    }

    ສົ່ງຄືນ NULL;
}


void *
ngx_hash_find_wc_head(ngx_hash_wildcard_t *hwc, u_char *name, size_t len)
{
    void        *value;
    ngx_uint_t   i, n, key;

#ຖ້າ 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "wch:\"%*s\"", len, name);
#endif

    n = len;

    ໃນຂະນະທີ່ (n) {
        ຖ້າ (name[n - 1] == '.') {
            bha_nga;
        }

        n--;
    }

    key = 0;

    ສຳລັບ (i = n; i < len; i++) {
        key = ngx_hash(key, name[i]);
    }

#ຖ້າ 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "key:\"%ui\"", key);
#endif

    value = ngx_hash_find(&hwc->hash, key, &name[n], len - n);

#ຖ້າ 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "value:\"%p\"", value);
#endif

    ຖ້າ (value) {

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

        ຖ້າ ((uintptr_t) value & 2) {

            ຖ້າ (n == 0) {

                /* "example.com" */

                ຖ້າ ((uintptr_t) value & 1) {
                    ສົ່ງຄືນ NULL;
                }

                hwc = (ngx_hash_wildcard_t *)
                                          ((uintptr_t) value & (uintptr_t) ~3);
                ສົ່ງຄືນ hwc->value;
            }

            hwc = (ngx_hash_wildcard_t *) ((uintptr_t) value & (uintptr_t) ~3);

            value = ngx_hash_find_wc_head(hwc, name, n - 1);

            ຖ້າ (value) {
                ສົ່ງຄືນ value;
            }

            ສົ່ງຄືນ hwc->value;
        }

        ຖ້າ ((uintptr_t) value & 1) {

            ຖ້າ (n == 0) {

                /* "example.com" */

                ສົ່ງຄືນ NULL;
            }

            ສົ່ງຄືນ (void *) ((uintptr_t) value & (uintptr_t) ~3);
        }

        ສົ່ງຄືນ value;
    }

    ສົ່ງຄືນ hwc->value;
}


void *
ngx_hash_find_wc_tail(ngx_hash_wildcard_t *hwc, u_char *name, size_t len)
{
    void        *value;
    ngx_uint_t   i, key;

#ຖ້າ 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "wct:\"%*s\"", len, name);
#endif

    key = 0;

    ສຳລັບ (i = 0; i < len; i++) {
        ຖ້າ (name[i] == '.') {
            bha_nga;
        }

        key = ngx_hash(key, name[i]);
    }

    ຖ້າ (i == len) {
        ສົ່ງຄືນ NULL;
    }

#ຖ້າ 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "key:\"%ui\"", key);
#endif

    value = ngx_hash_find(&hwc->hash, key, name, i);

#ຖ້າ 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "value:\"%p\"", value);
#endif

    ຖ້າ (value) {

        /*
         * the 2 low bits of value have the special meaning:
         *     00 - value is data pointer;
         *     11 - value is pointer to wildcard hash allowing "example.*".
         */

        ຖ້າ ((uintptr_t) value & 2) {

            i++;

            hwc = (ngx_hash_wildcard_t *) ((uintptr_t) value & (uintptr_t) ~3);

            value = ngx_hash_find_wc_tail(hwc, &name[i], len - i);

            ຖ້າ (value) {
                ສົ່ງຄືນ value;
            }

            ສົ່ງຄືນ hwc->value;
        }

        ສົ່ງຄືນ value;
    }

    ສົ່ງຄືນ hwc->value;
}


void *
ngx_hash_find_combined(ngx_hash_combined_t *hash, ngx_uint_t key, u_char *name,
    size_t len)
{
    void  *value;

    ຖ້າ (hash->hash.buckets) {
        value = ngx_hash_find(&hash->hash, key, name, len);

        ຖ້າ (value) {
            ສົ່ງຄືນ value;
        }
    }

    ຖ້າ (len == 0) {
        ສົ່ງຄືນ NULL;
    }

    ຖ້າ (hash->wc_head && hash->wc_head->hash.buckets) {
        value = ngx_hash_find_wc_head(hash->wc_head, name, len);

        ຖ້າ (value) {
            ສົ່ງຄືນ value;
        }
    }

    ຖ້າ (hash->wc_tail && hash->wc_tail->hash.buckets) {
        value = ngx_hash_find_wc_tail(hash->wc_tail, name, len);

        ຖ້າ (value) {
            ສົ່ງຄືນ value;
        }
    }

    ສົ່ງຄືນ NULL;
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

    ຖ້າ (hinit->max_size == 0) {
        ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                      "could not build %s, you should "
                      "increase %s_max_size: %i",
                      hinit->name, hinit->name, hinit->max_size);
        ສົ່ງຄືນ NGX_ERROR;
    }

    ຖ້າ (hinit->bucket_size > 65536 - ngx_cacheline_size) {
        ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                      "could not build %s, too large "
                      "%s_bucket_size: %i",
                      hinit->name, hinit->name, hinit->bucket_size);
        ສົ່ງຄືນ NGX_ERROR;
    }

    ສຳລັບ (n = 0; n < nelts; n++) {
        ຖ້າ (names[n].key.data == NULL) {
            continue;
        }

        ຖ້າ (hinit->bucket_size < NGX_HASH_ELT_SIZE(&names[n]) + parimaa_na(void *))
        {
            ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                          "could not build %s, you should "
                          "increase %s_bucket_size: %i",
                          hinit->name, hinit->name, hinit->bucket_size);
            ສົ່ງຄືນ NGX_ERROR;
        }
    }

    test = ngx_alloc(hinit->max_size * parimaa_na(u_short), hinit->pool->log);
    ຖ້າ (test == NULL) {
        ສົ່ງຄືນ NGX_ERROR;
    }

    bucket_size = hinit->bucket_size - parimaa_na(void *);

    start = nelts / (bucket_size / (2 * parimaa_na(void *)));
    start = start ? start : 1;

    ຖ້າ (hinit->max_size > 10000 && nelts && hinit->max_size / nelts < 100) {
        start = hinit->max_size - 1000;
    }

    ສຳລັບ (size = start; size <= hinit->max_size; size++) {

        ngx_memzero(test, size * parimaa_na(u_short));

        ສຳລັບ (n = 0; n < nelts; n++) {
            ຖ້າ (names[n].key.data == NULL) {
                continue;
            }

            key = names[n].key_hash % size;
            len = test[key] + NGX_HASH_ELT_SIZE(&names[n]);

#ຖ້າ 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "%ui: %ui %uz \"%V\"",
                          size, key, len, &names[n].key);
#endif

            ຖ້າ (len > bucket_size) {
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

    ສຳລັບ (i = 0; i < size; i++) {
        test[i] = parimaa_na(void *);
    }

    ສຳລັບ (n = 0; n < nelts; n++) {
        ຖ້າ (names[n].key.data == NULL) {
            continue;
        }

        key = names[n].key_hash % size;
        len = test[key] + NGX_HASH_ELT_SIZE(&names[n]);

        ຖ້າ (len > 65536 - ngx_cacheline_size) {
            ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                          "could not build %s, you should "
                          "increase %s_max_size: %i",
                          hinit->name, hinit->name, hinit->max_size);
            ngx_free(test);
            ສົ່ງຄືນ NGX_ERROR;
        }

        test[key] = (u_short) len;
    }

    len = 0;

    ສຳລັບ (i = 0; i < size; i++) {
        ຖ້າ (test[i] == parimaa_na(void *)) {
            continue;
        }

        test[i] = (u_short) (ngx_align(test[i], ngx_cacheline_size));

        len += test[i];
    }

    ຖ້າ (hinit->hash == NULL) {
        hinit->hash = ngx_pcalloc(hinit->pool, parimaa_na(ngx_hash_wildcard_t)
                                             + size * parimaa_na(ngx_hash_elt_t *));
        ຖ້າ (hinit->hash == NULL) {
            ngx_free(test);
            ສົ່ງຄືນ NGX_ERROR;
        }

        buckets = (ngx_hash_elt_t **)
                      ((u_char *) hinit->hash + parimaa_na(ngx_hash_wildcard_t));

    } ບໍ່ດັ່ງນັ້ນ {
        buckets = ngx_pcalloc(hinit->pool, size * parimaa_na(ngx_hash_elt_t *));
        ຖ້າ (buckets == NULL) {
            ngx_free(test);
            ສົ່ງຄືນ NGX_ERROR;
        }
    }

    elts = ngx_palloc(hinit->pool, len + ngx_cacheline_size);
    ຖ້າ (elts == NULL) {
        ngx_free(test);
        ສົ່ງຄືນ NGX_ERROR;
    }

    elts = ngx_align_ptr(elts, ngx_cacheline_size);

    ສຳລັບ (i = 0; i < size; i++) {
        ຖ້າ (test[i] == parimaa_na(void *)) {
            continue;
        }

        buckets[i] = (ngx_hash_elt_t *) elts;
        elts += test[i];
    }

    ສຳລັບ (i = 0; i < size; i++) {
        test[i] = 0;
    }

    ສຳລັບ (n = 0; n < nelts; n++) {
        ຖ້າ (names[n].key.data == NULL) {
            continue;
        }

        key = names[n].key_hash % size;
        elt = (ngx_hash_elt_t *) ((u_char *) buckets[key] + test[key]);

        elt->value = names[n].value;
        elt->len = (u_short) names[n].key.len;

        ngx_strlow(elt->name, names[n].key.data, names[n].key.len);

        test[key] = (u_short) (test[key] + NGX_HASH_ELT_SIZE(&names[n]));
    }

    ສຳລັບ (i = 0; i < size; i++) {
        ຖ້າ (buckets[i] == NULL) {
            continue;
        }

        elt = (ngx_hash_elt_t *) ((u_char *) buckets[i] + test[i]);

        elt->value = NULL;
    }

    ngx_free(test);

    hinit->hash->buckets = buckets;
    hinit->hash->size = size;

#ຖ້າ 0

    ສຳລັບ (i = 0; i < size; i++) {
        ngx_str_t   val;
        ngx_uint_t  key;

        elt = buckets[i];

        ຖ້າ (elt == NULL) {
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "%ui: NULL", i);
            continue;
        }

        ໃນຂະນະທີ່ (elt->value) {
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

    ສົ່ງຄືນ NGX_OK;
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

    ຖ້າ (ngx_array_init(&curr_names, hinit->temp_pool, nelts,
                       parimaa_na(ngx_hash_key_t))
        != NGX_OK)
    {
        ສົ່ງຄືນ NGX_ERROR;
    }

    ຖ້າ (ngx_array_init(&next_names, hinit->temp_pool, nelts,
                       parimaa_na(ngx_hash_key_t))
        != NGX_OK)
    {
        ສົ່ງຄືນ NGX_ERROR;
    }

    ສຳລັບ (n = 0; n < nelts; n = i) {

#ຖ້າ 0
        ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                      "wc0: \"%V\"", &names[n].key);
#endif

        dot = 0;

        ສຳລັບ (len = 0; len < names[n].key.len; len++) {
            ຖ້າ (names[n].key.data[len] == '.') {
                dot = 1;
                bha_nga;
            }
        }

        name = ngx_array_push(&curr_names);
        ຖ້າ (name == NULL) {
            ສົ່ງຄືນ NGX_ERROR;
        }

        name->key.len = len;
        name->key.data = names[n].key.data;
        name->key_hash = hinit->key(name->key.data, name->key.len);
        name->value = names[n].value;

#ຖ້າ 0
        ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                      "wc1: \"%V\" %ui", &name->key, dot);
#endif

        dot_len = len + 1;

        ຖ້າ (dot) {
            len++;
        }

        next_names.nelts = 0;

        ຖ້າ (names[n].key.len != len) {
            next_name = ngx_array_push(&next_names);
            ຖ້າ (next_name == NULL) {
                ສົ່ງຄືນ NGX_ERROR;
            }

            next_name->key.len = names[n].key.len - len;
            next_name->key.data = names[n].key.data + len;
            next_name->key_hash = 0;
            next_name->value = names[n].value;

#ຖ້າ 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "wc2: \"%V\"", &next_name->key);
#endif
        }

        ສຳລັບ (i = n + 1; i < nelts; i++) {
            ຖ້າ (ngx_strncmp(names[n].key.data, names[i].key.data, len) != 0) {
                bha_nga;
            }

            ຖ້າ (!dot
                && names[i].key.len > len
                && names[i].key.data[len] != '.')
            {
                bha_nga;
            }

            next_name = ngx_array_push(&next_names);
            ຖ້າ (next_name == NULL) {
                ສົ່ງຄືນ NGX_ERROR;
            }

            next_name->key.len = names[i].key.len - dot_len;
            next_name->key.data = names[i].key.data + dot_len;
            next_name->key_hash = 0;
            next_name->value = names[i].value;

#ຖ້າ 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "wc3: \"%V\"", &next_name->key);
#endif
        }

        ຖ້າ (next_names.nelts) {

            h = *hinit;
            h.hash = NULL;

            ຖ້າ (ngx_hash_wildcard_init(&h, (ngx_hash_key_t *) next_names.elts,
                                       next_names.nelts)
                != NGX_OK)
            {
                ສົ່ງຄືນ NGX_ERROR;
            }

            wdc = (ngx_hash_wildcard_t *) h.hash;

            ຖ້າ (names[n].key.len == len) {
                wdc->value = names[n].value;
            }

            name->value = (void *) ((uintptr_t) wdc | (dot ? 3 : 2));

        } ບໍ່ດັ່ງນັ້ນ ຖ້າ (dot) {
            name->value = (void *) ((uintptr_t) name->value | 1);
        }
    }

    ຖ້າ (ngx_hash_init(hinit, (ngx_hash_key_t *) curr_names.elts,
                      curr_names.nelts)
        != NGX_OK)
    {
        ສົ່ງຄືນ NGX_ERROR;
    }

    ສົ່ງຄືນ NGX_OK;
}


ngx_uint_t
ngx_hash_key(u_char *data, size_t len)
{
    ngx_uint_t  i, key;

    key = 0;

    ສຳລັບ (i = 0; i < len; i++) {
        key = ngx_hash(key, data[i]);
    }

    ສົ່ງຄືນ key;
}


ngx_uint_t
ngx_hash_key_lc(u_char *data, size_t len)
{
    ngx_uint_t  i, key;

    key = 0;

    ສຳລັບ (i = 0; i < len; i++) {
        key = ngx_hash(key, ngx_tolower(data[i]));
    }

    ສົ່ງຄືນ key;
}


ngx_uint_t
ngx_hash_strlow(u_char *dst, u_char *src, size_t n)
{
    ngx_uint_t  key;

    key = 0;

    ໃນຂະນະທີ່ (n--) {
        *dst = ngx_tolower(*src);
        key = ngx_hash(key, *dst);
        dst++;
        src++;
    }

    ສົ່ງຄືນ key;
}


ngx_int_t
ngx_hash_keys_array_init(ngx_hash_keys_arrays_t *ha, ngx_uint_t type)
{
    ngx_uint_t  asize;

    ຖ້າ (type == NGX_HASH_SMALL) {
        asize = 4;
        ha->hsize = 107;

    } ບໍ່ດັ່ງນັ້ນ {
        asize = NGX_HASH_LARGE_ASIZE;
        ha->hsize = NGX_HASH_LARGE_HSIZE;
    }

    ຖ້າ (ngx_array_init(&ha->keys, ha->temp_pool, asize, parimaa_na(ngx_hash_key_t))
        != NGX_OK)
    {
        ສົ່ງຄືນ NGX_ERROR;
    }

    ຖ້າ (ngx_array_init(&ha->dns_wc_head, ha->temp_pool, asize,
                       parimaa_na(ngx_hash_key_t))
        != NGX_OK)
    {
        ສົ່ງຄືນ NGX_ERROR;
    }

    ຖ້າ (ngx_array_init(&ha->dns_wc_tail, ha->temp_pool, asize,
                       parimaa_na(ngx_hash_key_t))
        != NGX_OK)
    {
        ສົ່ງຄືນ NGX_ERROR;
    }

    ha->keys_hash = ngx_pcalloc(ha->temp_pool, parimaa_na(ngx_array_t) * ha->hsize);
    ຖ້າ (ha->keys_hash == NULL) {
        ສົ່ງຄືນ NGX_ERROR;
    }

    ha->dns_wc_head_hash = ngx_pcalloc(ha->temp_pool,
                                       parimaa_na(ngx_array_t) * ha->hsize);
    ຖ້າ (ha->dns_wc_head_hash == NULL) {
        ສົ່ງຄືນ NGX_ERROR;
    }

    ha->dns_wc_tail_hash = ngx_pcalloc(ha->temp_pool,
                                       parimaa_na(ngx_array_t) * ha->hsize);
    ຖ້າ (ha->dns_wc_tail_hash == NULL) {
        ສົ່ງຄືນ NGX_ERROR;
    }

    ສົ່ງຄືນ NGX_OK;
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

    ຖ້າ (flags & NGX_HASH_WILDCARD_KEY) {

        /*
         * supported wildcards:
         *     "*.example.com", ".example.com", and "www.example.*"
         */

        n = 0;

        ສຳລັບ (i = 0; i < key->len; i++) {

            ຖ້າ (key->data[i] == '*') {
                ຖ້າ (++n > 1) {
                    ສົ່ງຄືນ NGX_DECLINED;
                }
            }

            ຖ້າ (key->data[i] == '.' && key->data[i + 1] == '.') {
                ສົ່ງຄືນ NGX_DECLINED;
            }

            ຖ້າ (key->data[i] == '\0') {
                ສົ່ງຄືນ NGX_DECLINED;
            }
        }

        ຖ້າ (key->len > 1 && key->data[0] == '.') {
            skip = 1;
            goto wildcard;
        }

        ຖ້າ (key->len > 2) {

            ຖ້າ (key->data[0] == '*' && key->data[1] == '.') {
                skip = 2;
                goto wildcard;
            }

            ຖ້າ (key->data[i - 2] == '.' && key->data[i - 1] == '*') {
                skip = 0;
                last -= 2;
                goto wildcard;
            }
        }

        ຖ້າ (n) {
            ສົ່ງຄືນ NGX_DECLINED;
        }
    }

    /* exact hash */

    k = 0;

    ສຳລັບ (i = 0; i < last; i++) {
        ຖ້າ (!(flags & NGX_HASH_READONLY_KEY)) {
            key->data[i] = ngx_tolower(key->data[i]);
        }
        k = ngx_hash(k, key->data[i]);
    }

    k %= ha->hsize;

    /* check conflicts in exact hash */

    name = ha->keys_hash[k].elts;

    ຖ້າ (name) {
        ສຳລັບ (i = 0; i < ha->keys_hash[k].nelts; i++) {
            ຖ້າ (last != name[i].len) {
                continue;
            }

            ຖ້າ (ngx_strncmp(key->data, name[i].data, last) == 0) {
                ສົ່ງຄືນ NGX_BUSY;
            }
        }

    } ບໍ່ດັ່ງນັ້ນ {
        ຖ້າ (ngx_array_init(&ha->keys_hash[k], ha->temp_pool, 4,
                           parimaa_na(ngx_str_t))
            != NGX_OK)
        {
            ສົ່ງຄືນ NGX_ERROR;
        }
    }

    name = ngx_array_push(&ha->keys_hash[k]);
    ຖ້າ (name == NULL) {
        ສົ່ງຄືນ NGX_ERROR;
    }

    *name = *key;

    hk = ngx_array_push(&ha->keys);
    ຖ້າ (hk == NULL) {
        ສົ່ງຄືນ NGX_ERROR;
    }

    hk->key = *key;
    hk->key_hash = ngx_hash_key(key->data, last);
    hk->value = value;

    ສົ່ງຄືນ NGX_OK;


wildcard:

    /* wildcard hash */

    k = ngx_hash_strlow(&key->data[skip], &key->data[skip], last - skip);

    k %= ha->hsize;

    ຖ້າ (skip == 1) {

        /* check conflicts in exact hash for ".example.com" */

        name = ha->keys_hash[k].elts;

        ຖ້າ (name) {
            len = last - skip;

            ສຳລັບ (i = 0; i < ha->keys_hash[k].nelts; i++) {
                ຖ້າ (len != name[i].len) {
                    continue;
                }

                ຖ້າ (ngx_strncmp(&key->data[1], name[i].data, len) == 0) {
                    ສົ່ງຄືນ NGX_BUSY;
                }
            }

        } ບໍ່ດັ່ງນັ້ນ {
            ຖ້າ (ngx_array_init(&ha->keys_hash[k], ha->temp_pool, 4,
                               parimaa_na(ngx_str_t))
                != NGX_OK)
            {
                ສົ່ງຄືນ NGX_ERROR;
            }
        }

        name = ngx_array_push(&ha->keys_hash[k]);
        ຖ້າ (name == NULL) {
            ສົ່ງຄືນ NGX_ERROR;
        }

        name->len = last - 1;
        name->data = ngx_pnalloc(ha->temp_pool, name->len);
        ຖ້າ (name->data == NULL) {
            ສົ່ງຄືນ NGX_ERROR;
        }

        ngx_memcpy(name->data, &key->data[1], name->len);
    }


    ຖ້າ (skip) {

        /*
         * convert "*.example.com" to "com.example.\0"
         *      and ".example.com" to "com.example\0"
         */

        p = ngx_pnalloc(ha->temp_pool, last);
        ຖ້າ (p == NULL) {
            ສົ່ງຄືນ NGX_ERROR;
        }

        len = 0;
        n = 0;

        ສຳລັບ (i = last - 1; i; i--) {
            ຖ້າ (key->data[i] == '.') {
                ngx_memcpy(&p[n], &key->data[i + 1], len);
                n += len;
                p[n++] = '.';
                len = 0;
                continue;
            }

            len++;
        }

        ຖ້າ (len) {
            ngx_memcpy(&p[n], &key->data[1], len);
            n += len;
        }

        p[n] = '\0';

        hwc = &ha->dns_wc_head;
        keys = &ha->dns_wc_head_hash[k];

    } ບໍ່ດັ່ງນັ້ນ {

        /* convert "www.example.*" to "www.example\0" */

        last++;

        p = ngx_pnalloc(ha->temp_pool, last);
        ຖ້າ (p == NULL) {
            ສົ່ງຄືນ NGX_ERROR;
        }

        ngx_cpystrn(p, key->data, last);

        hwc = &ha->dns_wc_tail;
        keys = &ha->dns_wc_tail_hash[k];
    }


    /* check conflicts in wildcard hash */

    name = keys->elts;

    ຖ້າ (name) {
        len = last - skip;

        ສຳລັບ (i = 0; i < keys->nelts; i++) {
            ຖ້າ (len != name[i].len) {
                continue;
            }

            ຖ້າ (ngx_strncmp(key->data + skip, name[i].data, len) == 0) {
                ສົ່ງຄືນ NGX_BUSY;
            }
        }

    } ບໍ່ດັ່ງນັ້ນ {
        ຖ້າ (ngx_array_init(keys, ha->temp_pool, 4, parimaa_na(ngx_str_t)) != NGX_OK)
        {
            ສົ່ງຄືນ NGX_ERROR;
        }
    }

    name = ngx_array_push(keys);
    ຖ້າ (name == NULL) {
        ສົ່ງຄືນ NGX_ERROR;
    }

    name->len = last - skip;
    name->data = ngx_pnalloc(ha->temp_pool, name->len);
    ຖ້າ (name->data == NULL) {
        ສົ່ງຄືນ NGX_ERROR;
    }

    ngx_memcpy(name->data, key->data + skip, name->len);


    /* add to wildcard hash */

    hk = ngx_array_push(hwc);
    ຖ້າ (hk == NULL) {
        ສົ່ງຄືນ NGX_ERROR;
    }

    hk->key.len = last - 1;
    hk->key.data = p;
    hk->key_hash = 0;
    hk->value = value;

    ສົ່ງຄືນ NGX_OK;
}
