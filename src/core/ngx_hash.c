
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


فراغ *
ngx_hash_find(ngx_hash_t *hash, ngx_uint_t key, u_char *name, size_t len)
{
    ngx_uint_t       i;
    ngx_hash_elt_t  *elt;

#𐡄𐡍 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "hf:\"%*s\"", len, name);
#endif

    elt = hash->buckets[key % hash->size];

    𐡄𐡍 (elt == عدم) {
        أرجع عدم;
    }

    طالما (elt->value) {
        𐡄𐡍 (len != (size_t) elt->len) {
            اذهب next;
        }

        لكل (i = 0; i < len; i++) {
            𐡄𐡍 (name[i] != elt->name[i]) {
                اذهب next;
            }
        }

        أرجع elt->value;

    next:

        elt = (ngx_hash_elt_t *) ngx_align_ptr(&elt->name[0] + elt->len,
                                               حجم(فراغ *));
        استمر;
    }

    أرجع عدم;
}


فراغ *
ngx_hash_find_wc_head(ngx_hash_wildcard_t *hwc, u_char *name, size_t len)
{
    فراغ        *value;
    ngx_uint_t   i, n, key;

#𐡄𐡍 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "wch:\"%*s\"", len, name);
#endif

    n = len;

    طالما (n) {
        𐡄𐡍 (name[n - 1] == '.') {
            اكسر;
        }

        n--;
    }

    key = 0;

    لكل (i = n; i < len; i++) {
        key = ngx_hash(key, name[i]);
    }

#𐡄𐡍 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "key:\"%ui\"", key);
#endif

    value = ngx_hash_find(&hwc->hash, key, &name[n], len - n);

#𐡄𐡍 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "value:\"%p\"", value);
#endif

    𐡄𐡍 (value) {

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

        𐡄𐡍 ((uintptr_t) value & 2) {

            𐡄𐡍 (n == 0) {

                /* "example.com" */

                𐡄𐡍 ((uintptr_t) value & 1) {
                    أرجع عدم;
                }

                hwc = (ngx_hash_wildcard_t *)
                                          ((uintptr_t) value & (uintptr_t) ~3);
                أرجع hwc->value;
            }

            hwc = (ngx_hash_wildcard_t *) ((uintptr_t) value & (uintptr_t) ~3);

            value = ngx_hash_find_wc_head(hwc, name, n - 1);

            𐡄𐡍 (value) {
                أرجع value;
            }

            أرجع hwc->value;
        }

        𐡄𐡍 ((uintptr_t) value & 1) {

            𐡄𐡍 (n == 0) {

                /* "example.com" */

                أرجع عدم;
            }

            أرجع (فراغ *) ((uintptr_t) value & (uintptr_t) ~3);
        }

        أرجع value;
    }

    أرجع hwc->value;
}


فراغ *
ngx_hash_find_wc_tail(ngx_hash_wildcard_t *hwc, u_char *name, size_t len)
{
    فراغ        *value;
    ngx_uint_t   i, key;

#𐡄𐡍 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "wct:\"%*s\"", len, name);
#endif

    key = 0;

    لكل (i = 0; i < len; i++) {
        𐡄𐡍 (name[i] == '.') {
            اكسر;
        }

        key = ngx_hash(key, name[i]);
    }

    𐡄𐡍 (i == len) {
        أرجع عدم;
    }

#𐡄𐡍 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "key:\"%ui\"", key);
#endif

    value = ngx_hash_find(&hwc->hash, key, name, i);

#𐡄𐡍 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "value:\"%p\"", value);
#endif

    𐡄𐡍 (value) {

        /*
         * the 2 low bits of value have the special meaning:
         *     00 - value is data pointer;
         *     11 - value is pointer to wildcard hash allowing "example.*".
         */

        𐡄𐡍 ((uintptr_t) value & 2) {

            i++;

            hwc = (ngx_hash_wildcard_t *) ((uintptr_t) value & (uintptr_t) ~3);

            value = ngx_hash_find_wc_tail(hwc, &name[i], len - i);

            𐡄𐡍 (value) {
                أرجع value;
            }

            أرجع hwc->value;
        }

        أرجع value;
    }

    أرجع hwc->value;
}


فراغ *
ngx_hash_find_combined(ngx_hash_combined_t *hash, ngx_uint_t key, u_char *name,
    size_t len)
{
    فراغ  *value;

    𐡄𐡍 (hash->hash.buckets) {
        value = ngx_hash_find(&hash->hash, key, name, len);

        𐡄𐡍 (value) {
            أرجع value;
        }
    }

    𐡄𐡍 (len == 0) {
        أرجع عدم;
    }

    𐡄𐡍 (hash->wc_head && hash->wc_head->hash.buckets) {
        value = ngx_hash_find_wc_head(hash->wc_head, name, len);

        𐡄𐡍 (value) {
            أرجع value;
        }
    }

    𐡄𐡍 (hash->wc_tail && hash->wc_tail->hash.buckets) {
        value = ngx_hash_find_wc_tail(hash->wc_tail, name, len);

        𐡄𐡍 (value) {
            أرجع value;
        }
    }

    أرجع عدم;
}


#define NGX_HASH_ELT_SIZE(name)                                               \
    (حجم(فراغ *) + ngx_align((name)->key.len + 2, حجم(فراغ *)))

ngx_int_t
ngx_hash_init(ngx_hash_init_t *hinit, ngx_hash_key_t *names, ngx_uint_t nelts)
{
    u_char          *elts;
    size_t           len;
    u_short         *test;
    ngx_uint_t       i, n, key, size, start, bucket_size;
    ngx_hash_elt_t  *elt, **buckets;

    𐡄𐡍 (hinit->max_size == 0) {
        ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                      "could not build %s, you should "
                      "increase %s_max_size: %i",
                      hinit->name, hinit->name, hinit->max_size);
        أرجع NGX_ERROR;
    }

    𐡄𐡍 (hinit->bucket_size > 65536 - ngx_cacheline_size) {
        ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                      "could not build %s, too large "
                      "%s_bucket_size: %i",
                      hinit->name, hinit->name, hinit->bucket_size);
        أرجع NGX_ERROR;
    }

    لكل (n = 0; n < nelts; n++) {
        𐡄𐡍 (names[n].key.data == عدم) {
            استمر;
        }

        𐡄𐡍 (hinit->bucket_size < NGX_HASH_ELT_SIZE(&names[n]) + حجم(فراغ *))
        {
            ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                          "could not build %s, you should "
                          "increase %s_bucket_size: %i",
                          hinit->name, hinit->name, hinit->bucket_size);
            أرجع NGX_ERROR;
        }
    }

    test = ngx_alloc(hinit->max_size * حجم(u_short), hinit->pool->log);
    𐡄𐡍 (test == عدم) {
        أرجع NGX_ERROR;
    }

    bucket_size = hinit->bucket_size - حجم(فراغ *);

    start = nelts / (bucket_size / (2 * حجم(فراغ *)));
    start = start ? start : 1;

    𐡄𐡍 (hinit->max_size > 10000 && nelts && hinit->max_size / nelts < 100) {
        start = hinit->max_size - 1000;
    }

    لكل (size = start; size <= hinit->max_size; size++) {

        ngx_memzero(test, size * حجم(u_short));

        لكل (n = 0; n < nelts; n++) {
            𐡄𐡍 (names[n].key.data == عدم) {
                استمر;
            }

            key = names[n].key_hash % size;
            len = test[key] + NGX_HASH_ELT_SIZE(&names[n]);

#𐡄𐡍 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "%ui: %ui %uz \"%V\"",
                          size, key, len, &names[n].key);
#endif

            𐡄𐡍 (len > bucket_size) {
                اذهب next;
            }

            test[key] = (u_short) len;
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
        𐡄𐡍 (names[n].key.data == عدم) {
            استمر;
        }

        key = names[n].key_hash % size;
        len = test[key] + NGX_HASH_ELT_SIZE(&names[n]);

        𐡄𐡍 (len > 65536 - ngx_cacheline_size) {
            ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                          "could not build %s, you should "
                          "increase %s_max_size: %i",
                          hinit->name, hinit->name, hinit->max_size);
            ngx_free(test);
            أرجع NGX_ERROR;
        }

        test[key] = (u_short) len;
    }

    len = 0;

    لكل (i = 0; i < size; i++) {
        𐡄𐡍 (test[i] == حجم(فراغ *)) {
            استمر;
        }

        test[i] = (u_short) (ngx_align(test[i], ngx_cacheline_size));

        len += test[i];
    }

    𐡄𐡍 (hinit->hash == عدم) {
        hinit->hash = ngx_pcalloc(hinit->pool, حجم(ngx_hash_wildcard_t)
                                             + size * حجم(ngx_hash_elt_t *));
        𐡄𐡍 (hinit->hash == عدم) {
            ngx_free(test);
            أرجع NGX_ERROR;
        }

        buckets = (ngx_hash_elt_t **)
                      ((u_char *) hinit->hash + حجم(ngx_hash_wildcard_t));

    } وإلا {
        buckets = ngx_pcalloc(hinit->pool, size * حجم(ngx_hash_elt_t *));
        𐡄𐡍 (buckets == عدم) {
            ngx_free(test);
            أرجع NGX_ERROR;
        }
    }

    elts = ngx_palloc(hinit->pool, len + ngx_cacheline_size);
    𐡄𐡍 (elts == عدم) {
        ngx_free(test);
        أرجع NGX_ERROR;
    }

    elts = ngx_align_ptr(elts, ngx_cacheline_size);

    لكل (i = 0; i < size; i++) {
        𐡄𐡍 (test[i] == حجم(فراغ *)) {
            استمر;
        }

        buckets[i] = (ngx_hash_elt_t *) elts;
        elts += test[i];
    }

    لكل (i = 0; i < size; i++) {
        test[i] = 0;
    }

    لكل (n = 0; n < nelts; n++) {
        𐡄𐡍 (names[n].key.data == عدم) {
            استمر;
        }

        key = names[n].key_hash % size;
        elt = (ngx_hash_elt_t *) ((u_char *) buckets[key] + test[key]);

        elt->value = names[n].value;
        elt->len = (u_short) names[n].key.len;

        ngx_strlow(elt->name, names[n].key.data, names[n].key.len);

        test[key] = (u_short) (test[key] + NGX_HASH_ELT_SIZE(&names[n]));
    }

    لكل (i = 0; i < size; i++) {
        𐡄𐡍 (buckets[i] == عدم) {
            استمر;
        }

        elt = (ngx_hash_elt_t *) ((u_char *) buckets[i] + test[i]);

        elt->value = عدم;
    }

    ngx_free(test);

    hinit->hash->buckets = buckets;
    hinit->hash->size = size;

#𐡄𐡍 0

    لكل (i = 0; i < size; i++) {
        ngx_str_t   val;
        ngx_uint_t  key;

        elt = buckets[i];

        𐡄𐡍 (elt == عدم) {
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "%ui: NULL", i);
            استمر;
        }

        طالما (elt->value) {
            val.len = elt->len;
            val.data = &elt->name[0];

            key = hinit->key(val.data, val.len);

            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "%ui: %p \"%V\" %ui", i, elt, &val, key);

            elt = (ngx_hash_elt_t *) ngx_align_ptr(&elt->name[0] + elt->len,
                                                   حجم(فراغ *));
        }
    }

#endif

    أرجع NGX_OK;
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

    𐡄𐡍 (ngx_array_init(&curr_names, hinit->temp_pool, nelts,
                       حجم(ngx_hash_key_t))
        != NGX_OK)
    {
        أرجع NGX_ERROR;
    }

    𐡄𐡍 (ngx_array_init(&next_names, hinit->temp_pool, nelts,
                       حجم(ngx_hash_key_t))
        != NGX_OK)
    {
        أرجع NGX_ERROR;
    }

    لكل (n = 0; n < nelts; n = i) {

#𐡄𐡍 0
        ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                      "wc0: \"%V\"", &names[n].key);
#endif

        dot = 0;

        لكل (len = 0; len < names[n].key.len; len++) {
            𐡄𐡍 (names[n].key.data[len] == '.') {
                dot = 1;
                اكسر;
            }
        }

        name = ngx_array_push(&curr_names);
        𐡄𐡍 (name == عدم) {
            أرجع NGX_ERROR;
        }

        name->key.len = len;
        name->key.data = names[n].key.data;
        name->key_hash = hinit->key(name->key.data, name->key.len);
        name->value = names[n].value;

#𐡄𐡍 0
        ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                      "wc1: \"%V\" %ui", &name->key, dot);
#endif

        dot_len = len + 1;

        𐡄𐡍 (dot) {
            len++;
        }

        next_names.nelts = 0;

        𐡄𐡍 (names[n].key.len != len) {
            next_name = ngx_array_push(&next_names);
            𐡄𐡍 (next_name == عدم) {
                أرجع NGX_ERROR;
            }

            next_name->key.len = names[n].key.len - len;
            next_name->key.data = names[n].key.data + len;
            next_name->key_hash = 0;
            next_name->value = names[n].value;

#𐡄𐡍 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "wc2: \"%V\"", &next_name->key);
#endif
        }

        لكل (i = n + 1; i < nelts; i++) {
            𐡄𐡍 (ngx_strncmp(names[n].key.data, names[i].key.data, len) != 0) {
                اكسر;
            }

            𐡄𐡍 (!dot
                && names[i].key.len > len
                && names[i].key.data[len] != '.')
            {
                اكسر;
            }

            next_name = ngx_array_push(&next_names);
            𐡄𐡍 (next_name == عدم) {
                أرجع NGX_ERROR;
            }

            next_name->key.len = names[i].key.len - dot_len;
            next_name->key.data = names[i].key.data + dot_len;
            next_name->key_hash = 0;
            next_name->value = names[i].value;

#𐡄𐡍 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "wc3: \"%V\"", &next_name->key);
#endif
        }

        𐡄𐡍 (next_names.nelts) {

            h = *hinit;
            h.hash = عدم;

            𐡄𐡍 (ngx_hash_wildcard_init(&h, (ngx_hash_key_t *) next_names.elts,
                                       next_names.nelts)
                != NGX_OK)
            {
                أرجع NGX_ERROR;
            }

            wdc = (ngx_hash_wildcard_t *) h.hash;

            𐡄𐡍 (names[n].key.len == len) {
                wdc->value = names[n].value;
            }

            name->value = (فراغ *) ((uintptr_t) wdc | (dot ? 3 : 2));

        } وإلا 𐡄𐡍 (dot) {
            name->value = (فراغ *) ((uintptr_t) name->value | 1);
        }
    }

    𐡄𐡍 (ngx_hash_init(hinit, (ngx_hash_key_t *) curr_names.elts,
                      curr_names.nelts)
        != NGX_OK)
    {
        أرجع NGX_ERROR;
    }

    أرجع NGX_OK;
}


ngx_uint_t
ngx_hash_key(u_char *data, size_t len)
{
    ngx_uint_t  i, key;

    key = 0;

    لكل (i = 0; i < len; i++) {
        key = ngx_hash(key, data[i]);
    }

    أرجع key;
}


ngx_uint_t
ngx_hash_key_lc(u_char *data, size_t len)
{
    ngx_uint_t  i, key;

    key = 0;

    لكل (i = 0; i < len; i++) {
        key = ngx_hash(key, ngx_tolower(data[i]));
    }

    أرجع key;
}


ngx_uint_t
ngx_hash_strlow(u_char *dst, u_char *src, size_t n)
{
    ngx_uint_t  key;

    key = 0;

    طالما (n--) {
        *dst = ngx_tolower(*src);
        key = ngx_hash(key, *dst);
        dst++;
        src++;
    }

    أرجع key;
}


ngx_int_t
ngx_hash_keys_array_init(ngx_hash_keys_arrays_t *ha, ngx_uint_t type)
{
    ngx_uint_t  asize;

    𐡄𐡍 (type == NGX_HASH_SMALL) {
        asize = 4;
        ha->hsize = 107;

    } وإلا {
        asize = NGX_HASH_LARGE_ASIZE;
        ha->hsize = NGX_HASH_LARGE_HSIZE;
    }

    𐡄𐡍 (ngx_array_init(&ha->keys, ha->temp_pool, asize, حجم(ngx_hash_key_t))
        != NGX_OK)
    {
        أرجع NGX_ERROR;
    }

    𐡄𐡍 (ngx_array_init(&ha->dns_wc_head, ha->temp_pool, asize,
                       حجم(ngx_hash_key_t))
        != NGX_OK)
    {
        أرجع NGX_ERROR;
    }

    𐡄𐡍 (ngx_array_init(&ha->dns_wc_tail, ha->temp_pool, asize,
                       حجم(ngx_hash_key_t))
        != NGX_OK)
    {
        أرجع NGX_ERROR;
    }

    ha->keys_hash = ngx_pcalloc(ha->temp_pool, حجم(ngx_array_t) * ha->hsize);
    𐡄𐡍 (ha->keys_hash == عدم) {
        أرجع NGX_ERROR;
    }

    ha->dns_wc_head_hash = ngx_pcalloc(ha->temp_pool,
                                       حجم(ngx_array_t) * ha->hsize);
    𐡄𐡍 (ha->dns_wc_head_hash == عدم) {
        أرجع NGX_ERROR;
    }

    ha->dns_wc_tail_hash = ngx_pcalloc(ha->temp_pool,
                                       حجم(ngx_array_t) * ha->hsize);
    𐡄𐡍 (ha->dns_wc_tail_hash == عدم) {
        أرجع NGX_ERROR;
    }

    أرجع NGX_OK;
}


ngx_int_t
ngx_hash_add_key(ngx_hash_keys_arrays_t *ha, ngx_str_t *key, فراغ *value,
    ngx_uint_t flags)
{
    size_t           len;
    u_char          *p;
    ngx_str_t       *name;
    ngx_uint_t       i, k, n, skip, last;
    ngx_array_t     *keys, *hwc;
    ngx_hash_key_t  *hk;

    last = key->len;

    𐡄𐡍 (flags & NGX_HASH_WILDCARD_KEY) {

        /*
         * supported wildcards:
         *     "*.example.com", ".example.com", and "www.example.*"
         */

        n = 0;

        لكل (i = 0; i < key->len; i++) {

            𐡄𐡍 (key->data[i] == '*') {
                𐡄𐡍 (++n > 1) {
                    أرجع NGX_DECLINED;
                }
            }

            𐡄𐡍 (key->data[i] == '.' && key->data[i + 1] == '.') {
                أرجع NGX_DECLINED;
            }

            𐡄𐡍 (key->data[i] == '\0') {
                أرجع NGX_DECLINED;
            }
        }

        𐡄𐡍 (key->len > 1 && key->data[0] == '.') {
            skip = 1;
            اذهب wildcard;
        }

        𐡄𐡍 (key->len > 2) {

            𐡄𐡍 (key->data[0] == '*' && key->data[1] == '.') {
                skip = 2;
                اذهب wildcard;
            }

            𐡄𐡍 (key->data[i - 2] == '.' && key->data[i - 1] == '*') {
                skip = 0;
                last -= 2;
                اذهب wildcard;
            }
        }

        𐡄𐡍 (n) {
            أرجع NGX_DECLINED;
        }
    }

    /* exact hash */

    k = 0;

    لكل (i = 0; i < last; i++) {
        𐡄𐡍 (!(flags & NGX_HASH_READONLY_KEY)) {
            key->data[i] = ngx_tolower(key->data[i]);
        }
        k = ngx_hash(k, key->data[i]);
    }

    k %= ha->hsize;

    /* check conflicts in exact hash */

    name = ha->keys_hash[k].elts;

    𐡄𐡍 (name) {
        لكل (i = 0; i < ha->keys_hash[k].nelts; i++) {
            𐡄𐡍 (last != name[i].len) {
                استمر;
            }

            𐡄𐡍 (ngx_strncmp(key->data, name[i].data, last) == 0) {
                أرجع NGX_BUSY;
            }
        }

    } وإلا {
        𐡄𐡍 (ngx_array_init(&ha->keys_hash[k], ha->temp_pool, 4,
                           حجم(ngx_str_t))
            != NGX_OK)
        {
            أرجع NGX_ERROR;
        }
    }

    name = ngx_array_push(&ha->keys_hash[k]);
    𐡄𐡍 (name == عدم) {
        أرجع NGX_ERROR;
    }

    *name = *key;

    hk = ngx_array_push(&ha->keys);
    𐡄𐡍 (hk == عدم) {
        أرجع NGX_ERROR;
    }

    hk->key = *key;
    hk->key_hash = ngx_hash_key(key->data, last);
    hk->value = value;

    أرجع NGX_OK;


wildcard:

    /* wildcard hash */

    k = ngx_hash_strlow(&key->data[skip], &key->data[skip], last - skip);

    k %= ha->hsize;

    𐡄𐡍 (skip == 1) {

        /* check conflicts in exact hash for ".example.com" */

        name = ha->keys_hash[k].elts;

        𐡄𐡍 (name) {
            len = last - skip;

            لكل (i = 0; i < ha->keys_hash[k].nelts; i++) {
                𐡄𐡍 (len != name[i].len) {
                    استمر;
                }

                𐡄𐡍 (ngx_strncmp(&key->data[1], name[i].data, len) == 0) {
                    أرجع NGX_BUSY;
                }
            }

        } وإلا {
            𐡄𐡍 (ngx_array_init(&ha->keys_hash[k], ha->temp_pool, 4,
                               حجم(ngx_str_t))
                != NGX_OK)
            {
                أرجع NGX_ERROR;
            }
        }

        name = ngx_array_push(&ha->keys_hash[k]);
        𐡄𐡍 (name == عدم) {
            أرجع NGX_ERROR;
        }

        name->len = last - 1;
        name->data = ngx_pnalloc(ha->temp_pool, name->len);
        𐡄𐡍 (name->data == عدم) {
            أرجع NGX_ERROR;
        }

        ngx_memcpy(name->data, &key->data[1], name->len);
    }


    𐡄𐡍 (skip) {

        /*
         * convert "*.example.com" to "com.example.\0"
         *      and ".example.com" to "com.example\0"
         */

        p = ngx_pnalloc(ha->temp_pool, last);
        𐡄𐡍 (p == عدم) {
            أرجع NGX_ERROR;
        }

        len = 0;
        n = 0;

        لكل (i = last - 1; i; i--) {
            𐡄𐡍 (key->data[i] == '.') {
                ngx_memcpy(&p[n], &key->data[i + 1], len);
                n += len;
                p[n++] = '.';
                len = 0;
                استمر;
            }

            len++;
        }

        𐡄𐡍 (len) {
            ngx_memcpy(&p[n], &key->data[1], len);
            n += len;
        }

        p[n] = '\0';

        hwc = &ha->dns_wc_head;
        keys = &ha->dns_wc_head_hash[k];

    } وإلا {

        /* convert "www.example.*" to "www.example\0" */

        last++;

        p = ngx_pnalloc(ha->temp_pool, last);
        𐡄𐡍 (p == عدم) {
            أرجع NGX_ERROR;
        }

        ngx_cpystrn(p, key->data, last);

        hwc = &ha->dns_wc_tail;
        keys = &ha->dns_wc_tail_hash[k];
    }


    /* check conflicts in wildcard hash */

    name = keys->elts;

    𐡄𐡍 (name) {
        len = last - skip;

        لكل (i = 0; i < keys->nelts; i++) {
            𐡄𐡍 (len != name[i].len) {
                استمر;
            }

            𐡄𐡍 (ngx_strncmp(key->data + skip, name[i].data, len) == 0) {
                أرجع NGX_BUSY;
            }
        }

    } وإلا {
        𐡄𐡍 (ngx_array_init(keys, ha->temp_pool, 4, حجم(ngx_str_t)) != NGX_OK)
        {
            أرجع NGX_ERROR;
        }
    }

    name = ngx_array_push(keys);
    𐡄𐡍 (name == عدم) {
        أرجع NGX_ERROR;
    }

    name->len = last - skip;
    name->data = ngx_pnalloc(ha->temp_pool, name->len);
    𐡄𐡍 (name->data == عدم) {
        أرجع NGX_ERROR;
    }

    ngx_memcpy(name->data, key->data + skip, name->len);


    /* add to wildcard hash */

    hk = ngx_array_push(hwc);
    𐡄𐡍 (hk == عدم) {
        أرجع NGX_ERROR;
    }

    hk->key.len = last - 1;
    hk->key.data = p;
    hk->key_hash = 0;
    hk->value = value;

    أرجع NGX_OK;
}
