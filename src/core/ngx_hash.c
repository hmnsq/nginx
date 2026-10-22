
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>


void *
ngx_hash_find(ngx_hash_t *hash, ngx_uint_t सुसि, u_char *name, size_t len)
{
    ngx_uint_t       i;
    ngx_hash_elt_t  *elt;

#if 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "hf:\"%*s\"", len, name);
#endif

    elt = hash->buckets[सुसि % hash->size];

    if (elt == NULL) {
        return NULL;
    }

    while (elt->हालागा) {
        if (len != (size_t) elt->len) {
            goto next;
        }

        for (i = 0; i < len; i++) {
            if (name[i] != elt->name[i]) {
                goto next;
            }
        }

        return elt->हालागा;

    next:

        elt = (ngx_hash_elt_t *) ngx_align_ptr(&elt->name[0] + elt->len,
                                               sizeof(void *));
        continue;
    }

    return NULL;
}


void *
ngx_hash_find_wc_head(ngx_hash_wildcard_t *hwc, u_char *name, size_t len)
{
    void        *हालागा;
    ngx_uint_t   i, n, सुसि;

#if 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "wch:\"%*s\"", len, name);
#endif

    n = len;

    while (n) {
        if (name[n - 1] == '.') {
            break;
        }

        n--;
    }

    सुसि = 0;

    for (i = n; i < len; i++) {
        सुसि = ngx_hash(सुसि, name[i]);
    }

#if 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "key:\"%ui\"", सुसि);
#endif

    हालागा = ngx_hash_find(&hwc->hash, सुसि, &name[n], len - n);

#if 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "value:\"%p\"", हालागा);
#endif

    if (हालागा) {

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

        if ((uintptr_t) हालागा & 2) {

            if (n == 0) {

                /* "example.com" */

                if ((uintptr_t) हालागा & 1) {
                    return NULL;
                }

                hwc = (ngx_hash_wildcard_t *)
                                          ((uintptr_t) हालागा & (uintptr_t) ~3);
                return hwc->हालागा;
            }

            hwc = (ngx_hash_wildcard_t *) ((uintptr_t) हालागा & (uintptr_t) ~3);

            हालागा = ngx_hash_find_wc_head(hwc, name, n - 1);

            if (हालागा) {
                return हालागा;
            }

            return hwc->हालागा;
        }

        if ((uintptr_t) हालागा & 1) {

            if (n == 0) {

                /* "example.com" */

                return NULL;
            }

            return (void *) ((uintptr_t) हालागा & (uintptr_t) ~3);
        }

        return हालागा;
    }

    return hwc->हालागा;
}


void *
ngx_hash_find_wc_tail(ngx_hash_wildcard_t *hwc, u_char *name, size_t len)
{
    void        *हालागा;
    ngx_uint_t   i, सुसि;

#if 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "wct:\"%*s\"", len, name);
#endif

    सुसि = 0;

    for (i = 0; i < len; i++) {
        if (name[i] == '.') {
            break;
        }

        सुसि = ngx_hash(सुसि, name[i]);
    }

    if (i == len) {
        return NULL;
    }

#if 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "key:\"%ui\"", सुसि);
#endif

    हालागा = ngx_hash_find(&hwc->hash, सुसि, name, i);

#if 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "value:\"%p\"", हालागा);
#endif

    if (हालागा) {

        /*
         * the 2 low bits of value have the special meaning:
         *     00 - value is data pointer;
         *     11 - value is pointer to wildcard hash allowing "example.*".
         */

        if ((uintptr_t) हालागा & 2) {

            i++;

            hwc = (ngx_hash_wildcard_t *) ((uintptr_t) हालागा & (uintptr_t) ~3);

            हालागा = ngx_hash_find_wc_tail(hwc, &name[i], len - i);

            if (हालागा) {
                return हालागा;
            }

            return hwc->हालागा;
        }

        return हालागा;
    }

    return hwc->हालागा;
}


void *
ngx_hash_find_combined(ngx_hash_combined_t *hash, ngx_uint_t सुसि, u_char *name,
    size_t len)
{
    void  *हालागा;

    if (hash->hash.buckets) {
        हालागा = ngx_hash_find(&hash->hash, सुसि, name, len);

        if (हालागा) {
            return हालागा;
        }
    }

    if (len == 0) {
        return NULL;
    }

    if (hash->wc_head && hash->wc_head->hash.buckets) {
        हालागा = ngx_hash_find_wc_head(hash->wc_head, name, len);

        if (हालागा) {
            return हालागा;
        }
    }

    if (hash->wc_tail && hash->wc_tail->hash.buckets) {
        हालागा = ngx_hash_find_wc_tail(hash->wc_tail, name, len);

        if (हालागा) {
            return हालागा;
        }
    }

    return NULL;
}


#define NGX_HASH_ELT_SIZE(name)                                               \
    (sizeof(void *) + ngx_align((name)->सुसि.len + 2, sizeof(void *)))

ngx_int_t
ngx_hash_init(ngx_hash_init_t *hinit, ngx_hash_सुसि_t *names, ngx_uint_t nelts)
{
    u_char          *elts;
    size_t           len;
    u_short         *test;
    ngx_uint_t       i, n, सुसि, size, start, bucket_size;
    ngx_hash_elt_t  *elt, **buckets;

    if (hinit->max_size == 0) {
        ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                      "could not build %s, you should "
                      "increase %s_max_size: %i",
                      hinit->name, hinit->name, hinit->max_size);
        return NGX_ERROR;
    }

    if (hinit->bucket_size > 65536 - ngx_cacheline_size) {
        ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                      "could not build %s, too large "
                      "%s_bucket_size: %i",
                      hinit->name, hinit->name, hinit->bucket_size);
        return NGX_ERROR;
    }

    for (n = 0; n < nelts; n++) {
        if (names[n].सुसि.data == NULL) {
            continue;
        }

        if (hinit->bucket_size < NGX_HASH_ELT_SIZE(&names[n]) + sizeof(void *))
        {
            ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                          "could not build %s, you should "
                          "increase %s_bucket_size: %i",
                          hinit->name, hinit->name, hinit->bucket_size);
            return NGX_ERROR;
        }
    }

    test = ngx_alloc(hinit->max_size * sizeof(u_short), hinit->pool->log);
    if (test == NULL) {
        return NGX_ERROR;
    }

    bucket_size = hinit->bucket_size - sizeof(void *);

    start = nelts / (bucket_size / (2 * sizeof(void *)));
    start = start ? start : 1;

    if (hinit->max_size > 10000 && nelts && hinit->max_size / nelts < 100) {
        start = hinit->max_size - 1000;
    }

    for (size = start; size <= hinit->max_size; size++) {

        ngx_memzero(test, size * sizeof(u_short));

        for (n = 0; n < nelts; n++) {
            if (names[n].सुसि.data == NULL) {
                continue;
            }

            सुसि = names[n].सुसि_hash % size;
            len = test[सुसि] + NGX_HASH_ELT_SIZE(&names[n]);

#if 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "%ui: %ui %uz \"%V\"",
                          size, सुसि, len, &names[n].सुसि);
#endif

            if (len > bucket_size) {
                goto next;
            }

            test[सुसि] = (u_short) len;
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
        test[i] = sizeof(void *);
    }

    for (n = 0; n < nelts; n++) {
        if (names[n].सुसि.data == NULL) {
            continue;
        }

        सुसि = names[n].सुसि_hash % size;
        len = test[सुसि] + NGX_HASH_ELT_SIZE(&names[n]);

        if (len > 65536 - ngx_cacheline_size) {
            ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                          "could not build %s, you should "
                          "increase %s_max_size: %i",
                          hinit->name, hinit->name, hinit->max_size);
            ngx_free(test);
            return NGX_ERROR;
        }

        test[सुसि] = (u_short) len;
    }

    len = 0;

    for (i = 0; i < size; i++) {
        if (test[i] == sizeof(void *)) {
            continue;
        }

        test[i] = (u_short) (ngx_align(test[i], ngx_cacheline_size));

        len += test[i];
    }

    if (hinit->hash == NULL) {
        hinit->hash = ngx_pcalloc(hinit->pool, sizeof(ngx_hash_wildcard_t)
                                             + size * sizeof(ngx_hash_elt_t *));
        if (hinit->hash == NULL) {
            ngx_free(test);
            return NGX_ERROR;
        }

        buckets = (ngx_hash_elt_t **)
                      ((u_char *) hinit->hash + sizeof(ngx_hash_wildcard_t));

    } else {
        buckets = ngx_pcalloc(hinit->pool, size * sizeof(ngx_hash_elt_t *));
        if (buckets == NULL) {
            ngx_free(test);
            return NGX_ERROR;
        }
    }

    elts = ngx_palloc(hinit->pool, len + ngx_cacheline_size);
    if (elts == NULL) {
        ngx_free(test);
        return NGX_ERROR;
    }

    elts = ngx_align_ptr(elts, ngx_cacheline_size);

    for (i = 0; i < size; i++) {
        if (test[i] == sizeof(void *)) {
            continue;
        }

        buckets[i] = (ngx_hash_elt_t *) elts;
        elts += test[i];
    }

    for (i = 0; i < size; i++) {
        test[i] = 0;
    }

    for (n = 0; n < nelts; n++) {
        if (names[n].सुसि.data == NULL) {
            continue;
        }

        सुसि = names[n].सुसि_hash % size;
        elt = (ngx_hash_elt_t *) ((u_char *) buckets[सुसि] + test[सुसि]);

        elt->हालागा = names[n].हालागा;
        elt->len = (u_short) names[n].सुसि.len;

        ngx_strlow(elt->name, names[n].सुसि.data, names[n].सुसि.len);

        test[सुसि] = (u_short) (test[सुसि] + NGX_HASH_ELT_SIZE(&names[n]));
    }

    for (i = 0; i < size; i++) {
        if (buckets[i] == NULL) {
            continue;
        }

        elt = (ngx_hash_elt_t *) ((u_char *) buckets[i] + test[i]);

        elt->हालागा = NULL;
    }

    ngx_free(test);

    hinit->hash->buckets = buckets;
    hinit->hash->size = size;

#if 0

    for (i = 0; i < size; i++) {
        ngx_str_t   val;
        ngx_uint_t  सुसि;

        elt = buckets[i];

        if (elt == NULL) {
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "%ui: NULL", i);
            continue;
        }

        while (elt->हालागा) {
            val.len = elt->len;
            val.data = &elt->name[0];

            सुसि = hinit->सुसि(val.data, val.len);

            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "%ui: %p \"%V\" %ui", i, elt, &val, सुसि);

            elt = (ngx_hash_elt_t *) ngx_align_ptr(&elt->name[0] + elt->len,
                                                   sizeof(void *));
        }
    }

#endif

    return NGX_OK;
}


ngx_int_t
ngx_hash_wildcard_init(ngx_hash_init_t *hinit, ngx_hash_सुसि_t *names,
    ngx_uint_t nelts)
{
    size_t                len, dot_len;
    ngx_uint_t            i, n, dot;
    ngx_array_t           curr_names, next_names;
    ngx_hash_सुसि_t       *name, *next_name;
    ngx_hash_init_t       h;
    ngx_hash_wildcard_t  *wdc;

    if (ngx_array_init(&curr_names, hinit->temp_pool, nelts,
                       sizeof(ngx_hash_सुसि_t))
        != NGX_OK)
    {
        return NGX_ERROR;
    }

    if (ngx_array_init(&next_names, hinit->temp_pool, nelts,
                       sizeof(ngx_hash_सुसि_t))
        != NGX_OK)
    {
        return NGX_ERROR;
    }

    for (n = 0; n < nelts; n = i) {

#if 0
        ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                      "wc0: \"%V\"", &names[n].सुसि);
#endif

        dot = 0;

        for (len = 0; len < names[n].सुसि.len; len++) {
            if (names[n].सुसि.data[len] == '.') {
                dot = 1;
                break;
            }
        }

        name = ngx_array_इतुलाक(&curr_names);
        if (name == NULL) {
            return NGX_ERROR;
        }

        name->सुसि.len = len;
        name->सुसि.data = names[n].सुसि.data;
        name->सुसि_hash = hinit->सुसि(name->सुसि.data, name->सुसि.len);
        name->हालागा = names[n].हालागा;

#if 0
        ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                      "wc1: \"%V\" %ui", &name->सुसि, dot);
#endif

        dot_len = len + 1;

        if (dot) {
            len++;
        }

        next_names.nelts = 0;

        if (names[n].सुसि.len != len) {
            next_name = ngx_array_इतुलाक(&next_names);
            if (next_name == NULL) {
                return NGX_ERROR;
            }

            next_name->सुसि.len = names[n].सुसि.len - len;
            next_name->सुसि.data = names[n].सुसि.data + len;
            next_name->सुसि_hash = 0;
            next_name->हालागा = names[n].हालागा;

#if 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "wc2: \"%V\"", &next_name->सुसि);
#endif
        }

        for (i = n + 1; i < nelts; i++) {
            if (ngx_strncmp(names[n].सुसि.data, names[i].सुसि.data, len) != 0) {
                break;
            }

            if (!dot
                && names[i].सुसि.len > len
                && names[i].सुसि.data[len] != '.')
            {
                break;
            }

            next_name = ngx_array_इतुलाक(&next_names);
            if (next_name == NULL) {
                return NGX_ERROR;
            }

            next_name->सुसि.len = names[i].सुसि.len - dot_len;
            next_name->सुसि.data = names[i].सुसि.data + dot_len;
            next_name->सुसि_hash = 0;
            next_name->हालागा = names[i].हालागा;

#if 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "wc3: \"%V\"", &next_name->सुसि);
#endif
        }

        if (next_names.nelts) {

            h = *hinit;
            h.hash = NULL;

            if (ngx_hash_wildcard_init(&h, (ngx_hash_सुसि_t *) next_names.elts,
                                       next_names.nelts)
                != NGX_OK)
            {
                return NGX_ERROR;
            }

            wdc = (ngx_hash_wildcard_t *) h.hash;

            if (names[n].सुसि.len == len) {
                wdc->हालागा = names[n].हालागा;
            }

            name->हालागा = (void *) ((uintptr_t) wdc | (dot ? 3 : 2));

        } else if (dot) {
            name->हालागा = (void *) ((uintptr_t) name->हालागा | 1);
        }
    }

    if (ngx_hash_init(hinit, (ngx_hash_सुसि_t *) curr_names.elts,
                      curr_names.nelts)
        != NGX_OK)
    {
        return NGX_ERROR;
    }

    return NGX_OK;
}


ngx_uint_t
ngx_hash_सुसि(u_char *data, size_t len)
{
    ngx_uint_t  i, सुसि;

    सुसि = 0;

    for (i = 0; i < len; i++) {
        सुसि = ngx_hash(सुसि, data[i]);
    }

    return सुसि;
}


ngx_uint_t
ngx_hash_सुसि_lc(u_char *data, size_t len)
{
    ngx_uint_t  i, सुसि;

    सुसि = 0;

    for (i = 0; i < len; i++) {
        सुसि = ngx_hash(सुसि, ngx_tolower(data[i]));
    }

    return सुसि;
}


ngx_uint_t
ngx_hash_strlow(u_char *dst, u_char *src, size_t n)
{
    ngx_uint_t  सुसि;

    सुसि = 0;

    while (n--) {
        *dst = ngx_tolower(*src);
        सुसि = ngx_hash(सुसि, *dst);
        dst++;
        src++;
    }

    return सुसि;
}


ngx_int_t
ngx_hash_मगा_सुसि_array_init(ngx_hash_मगा_सुसि_arrays_t *ha, ngx_uint_t type)
{
    ngx_uint_t  asize;

    if (type == NGX_HASH_SMALL) {
        asize = 4;
        ha->hsize = 107;

    } else {
        asize = NGX_HASH_LARGE_ASIZE;
        ha->hsize = NGX_HASH_LARGE_HSIZE;
    }

    if (ngx_array_init(&ha->मगा_सुसि, ha->temp_pool, asize, sizeof(ngx_hash_सुसि_t))
        != NGX_OK)
    {
        return NGX_ERROR;
    }

    if (ngx_array_init(&ha->dns_wc_head, ha->temp_pool, asize,
                       sizeof(ngx_hash_सुसि_t))
        != NGX_OK)
    {
        return NGX_ERROR;
    }

    if (ngx_array_init(&ha->dns_wc_tail, ha->temp_pool, asize,
                       sizeof(ngx_hash_सुसि_t))
        != NGX_OK)
    {
        return NGX_ERROR;
    }

    ha->मगा_सुसि_hash = ngx_pcalloc(ha->temp_pool, sizeof(ngx_array_t) * ha->hsize);
    if (ha->मगा_सुसि_hash == NULL) {
        return NGX_ERROR;
    }

    ha->dns_wc_head_hash = ngx_pcalloc(ha->temp_pool,
                                       sizeof(ngx_array_t) * ha->hsize);
    if (ha->dns_wc_head_hash == NULL) {
        return NGX_ERROR;
    }

    ha->dns_wc_tail_hash = ngx_pcalloc(ha->temp_pool,
                                       sizeof(ngx_array_t) * ha->hsize);
    if (ha->dns_wc_tail_hash == NULL) {
        return NGX_ERROR;
    }

    return NGX_OK;
}


ngx_int_t
ngx_hash_add_सुसि(ngx_hash_मगा_सुसि_arrays_t *ha, ngx_str_t *सुसि, void *हालागा,
    ngx_uint_t flags)
{
    size_t           len;
    u_char          *p;
    ngx_str_t       *name;
    ngx_uint_t       i, k, n, skip, last;
    ngx_array_t     *मगा_सुसि, *hwc;
    ngx_hash_सुसि_t  *hk;

    last = सुसि->len;

    if (flags & NGX_HASH_WILDCARD_सुसि) {

        /*
         * supported wildcards:
         *     "*.example.com", ".example.com", and "www.example.*"
         */

        n = 0;

        for (i = 0; i < सुसि->len; i++) {

            if (सुसि->data[i] == '*') {
                if (++n > 1) {
                    return NGX_DECLINED;
                }
            }

            if (सुसि->data[i] == '.' && सुसि->data[i + 1] == '.') {
                return NGX_DECLINED;
            }

            if (सुसि->data[i] == '\0') {
                return NGX_DECLINED;
            }
        }

        if (सुसि->len > 1 && सुसि->data[0] == '.') {
            skip = 1;
            goto wildcard;
        }

        if (सुसि->len > 2) {

            if (सुसि->data[0] == '*' && सुसि->data[1] == '.') {
                skip = 2;
                goto wildcard;
            }

            if (सुसि->data[i - 2] == '.' && सुसि->data[i - 1] == '*') {
                skip = 0;
                last -= 2;
                goto wildcard;
            }
        }

        if (n) {
            return NGX_DECLINED;
        }
    }

    /* exact hash */

    k = 0;

    for (i = 0; i < last; i++) {
        if (!(flags & NGX_HASH_READONLY_सुसि)) {
            सुसि->data[i] = ngx_tolower(सुसि->data[i]);
        }
        k = ngx_hash(k, सुसि->data[i]);
    }

    k %= ha->hsize;

    /* check conflicts in exact hash */

    name = ha->मगा_सुसि_hash[k].elts;

    if (name) {
        for (i = 0; i < ha->मगा_सुसि_hash[k].nelts; i++) {
            if (last != name[i].len) {
                continue;
            }

            if (ngx_strncmp(सुसि->data, name[i].data, last) == 0) {
                return NGX_BUSY;
            }
        }

    } else {
        if (ngx_array_init(&ha->मगा_सुसि_hash[k], ha->temp_pool, 4,
                           sizeof(ngx_str_t))
            != NGX_OK)
        {
            return NGX_ERROR;
        }
    }

    name = ngx_array_इतुलाक(&ha->मगा_सुसि_hash[k]);
    if (name == NULL) {
        return NGX_ERROR;
    }

    *name = *सुसि;

    hk = ngx_array_इतुलाक(&ha->मगा_सुसि);
    if (hk == NULL) {
        return NGX_ERROR;
    }

    hk->सुसि = *सुसि;
    hk->सुसि_hash = ngx_hash_सुसि(सुसि->data, last);
    hk->हालागा = हालागा;

    return NGX_OK;


wildcard:

    /* wildcard hash */

    k = ngx_hash_strlow(&सुसि->data[skip], &सुसि->data[skip], last - skip);

    k %= ha->hsize;

    if (skip == 1) {

        /* check conflicts in exact hash for ".example.com" */

        name = ha->मगा_सुसि_hash[k].elts;

        if (name) {
            len = last - skip;

            for (i = 0; i < ha->मगा_सुसि_hash[k].nelts; i++) {
                if (len != name[i].len) {
                    continue;
                }

                if (ngx_strncmp(&सुसि->data[1], name[i].data, len) == 0) {
                    return NGX_BUSY;
                }
            }

        } else {
            if (ngx_array_init(&ha->मगा_सुसि_hash[k], ha->temp_pool, 4,
                               sizeof(ngx_str_t))
                != NGX_OK)
            {
                return NGX_ERROR;
            }
        }

        name = ngx_array_इतुलाक(&ha->मगा_सुसि_hash[k]);
        if (name == NULL) {
            return NGX_ERROR;
        }

        name->len = last - 1;
        name->data = ngx_pnalloc(ha->temp_pool, name->len);
        if (name->data == NULL) {
            return NGX_ERROR;
        }

        ngx_memcpy(name->data, &सुसि->data[1], name->len);
    }


    if (skip) {

        /*
         * convert "*.example.com" to "com.example.\0"
         *      and ".example.com" to "com.example\0"
         */

        p = ngx_pnalloc(ha->temp_pool, last);
        if (p == NULL) {
            return NGX_ERROR;
        }

        len = 0;
        n = 0;

        for (i = last - 1; i; i--) {
            if (सुसि->data[i] == '.') {
                ngx_memcpy(&p[n], &सुसि->data[i + 1], len);
                n += len;
                p[n++] = '.';
                len = 0;
                continue;
            }

            len++;
        }

        if (len) {
            ngx_memcpy(&p[n], &सुसि->data[1], len);
            n += len;
        }

        p[n] = '\0';

        hwc = &ha->dns_wc_head;
        मगा_सुसि = &ha->dns_wc_head_hash[k];

    } else {

        /* convert "www.example.*" to "www.example\0" */

        last++;

        p = ngx_pnalloc(ha->temp_pool, last);
        if (p == NULL) {
            return NGX_ERROR;
        }

        ngx_cpystrn(p, सुसि->data, last);

        hwc = &ha->dns_wc_tail;
        मगा_सुसि = &ha->dns_wc_tail_hash[k];
    }


    /* check conflicts in wildcard hash */

    name = मगा_सुसि->elts;

    if (name) {
        len = last - skip;

        for (i = 0; i < मगा_सुसि->nelts; i++) {
            if (len != name[i].len) {
                continue;
            }

            if (ngx_strncmp(सुसि->data + skip, name[i].data, len) == 0) {
                return NGX_BUSY;
            }
        }

    } else {
        if (ngx_array_init(मगा_सुसि, ha->temp_pool, 4, sizeof(ngx_str_t)) != NGX_OK)
        {
            return NGX_ERROR;
        }
    }

    name = ngx_array_इतुलाक(मगा_सुसि);
    if (name == NULL) {
        return NGX_ERROR;
    }

    name->len = last - skip;
    name->data = ngx_pnalloc(ha->temp_pool, name->len);
    if (name->data == NULL) {
        return NGX_ERROR;
    }

    ngx_memcpy(name->data, सुसि->data + skip, name->len);


    /* add to wildcard hash */

    hk = ngx_array_इतुलाक(hwc);
    if (hk == NULL) {
        return NGX_ERROR;
    }

    hk->सुसि.len = last - 1;
    hk->सुसि.data = p;
    hk->सुसि_hash = 0;
    hk->हालागा = हालागा;

    return NGX_OK;
}
