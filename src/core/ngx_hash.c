
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


فراغ *
ngx_hash_ڳوليو(ngx_hash_t *hash, ngx_uint_t ڪنجي, u_char *name, سائيز_t len)
{
    ngx_uint_t       i;
    ngx_hash_elt_t  *elt;

#إذا 0
    ngx_log_error(NGX_LOG_ALERT, ngx_چڪر->لاگرٿم, 0, "hf:\"%*s\"", len, name);
#جيڪڏهن_پورو

    elt = hash->buckets[ڪنجي % hash->سائيز];

    إذا (elt == عدم) {
        أرجع عدم;
    }

    طالما (elt->قدر) {
        إذا (len != (سائيز_t) elt->len) {
            اذهب ايندڙ;
        }

        لكل (i = 0; i < len; i++) {
            إذا (name[i] != elt->name[i]) {
                اذهب ايندڙ;
            }
        }

        أرجع elt->قدر;

    ايندڙ:

        elt = (ngx_hash_elt_t *) ngx_align_ptr(&elt->name[0] + elt->len,
                                               حجم(فراغ *));
        استمر;
    }

    أرجع عدم;
}


فراغ *
ngx_hash_ڳوليو_wc_سر(ngx_hash_wildcard_t *hwc, u_char *name, سائيز_t len)
{
    فراغ        *قدر;
    ngx_uint_t   i, n, ڪنجي;

#إذا 0
    ngx_log_error(NGX_LOG_ALERT, ngx_چڪر->لاگرٿم, 0, "wch:\"%*s\"", len, name);
#جيڪڏهن_پورو

    n = len;

    طالما (n) {
        إذا (name[n - 1] == '.') {
            اكسر;
        }

        n--;
    }

    ڪنجي = 0;

    لكل (i = n; i < len; i++) {
        ڪنجي = ngx_hash(ڪنجي, name[i]);
    }

#إذا 0
    ngx_log_error(NGX_LOG_ALERT, ngx_چڪر->لاگرٿم, 0, "key:\"%ui\"", ڪنجي);
#جيڪڏهن_پورو

    قدر = ngx_hash_ڳوليو(&hwc->hash, ڪنجي, &name[n], len - n);

#إذا 0
    ngx_log_error(NGX_LOG_ALERT, ngx_چڪر->لاگرٿم, 0, "value:\"%p\"", قدر);
#جيڪڏهن_پورو

    إذا (قدر) {

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

        إذا ((uintptr_t) قدر & 2) {

            إذا (n == 0) {

                /* "example.com" */

                إذا ((uintptr_t) قدر & 1) {
                    أرجع عدم;
                }

                hwc = (ngx_hash_wildcard_t *)
                                          ((uintptr_t) قدر & (uintptr_t) ~3);
                أرجع hwc->قدر;
            }

            hwc = (ngx_hash_wildcard_t *) ((uintptr_t) قدر & (uintptr_t) ~3);

            قدر = ngx_hash_ڳوليو_wc_سر(hwc, name, n - 1);

            إذا (قدر) {
                أرجع قدر;
            }

            أرجع hwc->قدر;
        }

        إذا ((uintptr_t) قدر & 1) {

            إذا (n == 0) {

                /* "example.com" */

                أرجع عدم;
            }

            أرجع (فراغ *) ((uintptr_t) قدر & (uintptr_t) ~3);
        }

        أرجع قدر;
    }

    أرجع hwc->قدر;
}


فراغ *
ngx_hash_ڳوليو_wc_پڇ(ngx_hash_wildcard_t *hwc, u_char *name, سائيز_t len)
{
    فراغ        *قدر;
    ngx_uint_t   i, ڪنجي;

#إذا 0
    ngx_log_error(NGX_LOG_ALERT, ngx_چڪر->لاگرٿم, 0, "wct:\"%*s\"", len, name);
#جيڪڏهن_پورو

    ڪنجي = 0;

    لكل (i = 0; i < len; i++) {
        إذا (name[i] == '.') {
            اكسر;
        }

        ڪنجي = ngx_hash(ڪنجي, name[i]);
    }

    إذا (i == len) {
        أرجع عدم;
    }

#إذا 0
    ngx_log_error(NGX_LOG_ALERT, ngx_چڪر->لاگرٿم, 0, "key:\"%ui\"", ڪنجي);
#جيڪڏهن_پورو

    قدر = ngx_hash_ڳوليو(&hwc->hash, ڪنجي, name, i);

#إذا 0
    ngx_log_error(NGX_LOG_ALERT, ngx_چڪر->لاگرٿم, 0, "value:\"%p\"", قدر);
#جيڪڏهن_پورو

    إذا (قدر) {

        /*
         * the 2 low bits of value have the special meaning:
         *     00 - value is data pointer;
         *     11 - value is pointer to wildcard hash allowing "example.*".
         */

        إذا ((uintptr_t) قدر & 2) {

            i++;

            hwc = (ngx_hash_wildcard_t *) ((uintptr_t) قدر & (uintptr_t) ~3);

            قدر = ngx_hash_ڳوليو_wc_پڇ(hwc, &name[i], len - i);

            إذا (قدر) {
                أرجع قدر;
            }

            أرجع hwc->قدر;
        }

        أرجع قدر;
    }

    أرجع hwc->قدر;
}


فراغ *
ngx_hash_ڳوليو_combined(ngx_hash_combined_t *hash, ngx_uint_t ڪنجي, u_char *name,
    سائيز_t len)
{
    فراغ  *قدر;

    إذا (hash->hash.buckets) {
        قدر = ngx_hash_ڳوليو(&hash->hash, ڪنجي, name, len);

        إذا (قدر) {
            أرجع قدر;
        }
    }

    إذا (len == 0) {
        أرجع عدم;
    }

    إذا (hash->wc_سر && hash->wc_سر->hash.buckets) {
        قدر = ngx_hash_ڳوليو_wc_سر(hash->wc_سر, name, len);

        إذا (قدر) {
            أرجع قدر;
        }
    }

    إذا (hash->wc_پڇ && hash->wc_پڇ->hash.buckets) {
        قدر = ngx_hash_ڳوليو_wc_پڇ(hash->wc_پڇ, name, len);

        إذا (قدر) {
            أرجع قدر;
        }
    }

    أرجع عدم;
}


#وصف_ڪريو NGX_HASH_ELT_سائيز(name)                                               \
    (حجم(فراغ *) + ngx_align((name)->ڪنجي.len + 2, حجم(فراغ *)))

ngx_int_t
ngx_hash_init(ngx_hash_init_t *hinit, ngx_hash_ڪنجي_t *نالا, ngx_uint_t nelts)
{
    u_char          *elts;
    سائيز_t           len;
    u_short         *test;
    ngx_uint_t       i, n, ڪنجي, سائيز, شروعات, ٻالٽي_سائيز;
    ngx_hash_elt_t  *elt, **buckets;

    إذا (hinit->max_سائيز == 0) {
        ngx_log_error(NGX_LOG_EMERG, hinit->pool->لاگرٿم, 0,
                      "could not build %s, you should "
                      "increase %s_max_size: %i",
                      hinit->name, hinit->name, hinit->max_سائيز);
        أرجع NGX_ERROR;
    }

    إذا (hinit->ٻالٽي_سائيز > 65536 - ngx_cacheline_سائيز) {
        ngx_log_error(NGX_LOG_EMERG, hinit->pool->لاگرٿم, 0,
                      "could not build %s, too large "
                      "%s_bucket_size: %i",
                      hinit->name, hinit->name, hinit->ٻالٽي_سائيز);
        أرجع NGX_ERROR;
    }

    لكل (n = 0; n < nelts; n++) {
        إذا (نالا[n].ڪنجي.معلومات == عدم) {
            استمر;
        }

        إذا (hinit->ٻالٽي_سائيز < NGX_HASH_ELT_سائيز(&نالا[n]) + حجم(فراغ *))
        {
            ngx_log_error(NGX_LOG_EMERG, hinit->pool->لاگرٿم, 0,
                          "could not build %s, you should "
                          "increase %s_bucket_size: %i",
                          hinit->name, hinit->name, hinit->ٻالٽي_سائيز);
            أرجع NGX_ERROR;
        }
    }

    test = ngx_alloc(hinit->max_سائيز * حجم(u_short), hinit->pool->لاگرٿم);
    إذا (test == عدم) {
        أرجع NGX_ERROR;
    }

    ٻالٽي_سائيز = hinit->ٻالٽي_سائيز - حجم(فراغ *);

    شروعات = nelts / (ٻالٽي_سائيز / (2 * حجم(فراغ *)));
    شروعات = شروعات ? شروعات : 1;

    إذا (hinit->max_سائيز > 10000 && nelts && hinit->max_سائيز / nelts < 100) {
        شروعات = hinit->max_سائيز - 1000;
    }

    لكل (سائيز = شروعات; سائيز <= hinit->max_سائيز; سائيز++) {

        ngx_memzero(test, سائيز * حجم(u_short));

        لكل (n = 0; n < nelts; n++) {
            إذا (نالا[n].ڪنجي.معلومات == عدم) {
                استمر;
            }

            ڪنجي = نالا[n].ڪنجي_hash % سائيز;
            len = test[ڪنجي] + NGX_HASH_ELT_سائيز(&نالا[n]);

#إذا 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->لاگرٿم, 0,
                          "%ui: %ui %uz \"%V\"",
                          سائيز, ڪنجي, len, &نالا[n].ڪنجي);
#جيڪڏهن_پورو

            إذا (len > ٻالٽي_سائيز) {
                اذهب ايندڙ;
            }

            test[ڪنجي] = (u_short) len;
        }

        اذهب مليو;

    ايندڙ:

        استمر;
    }

    سائيز = hinit->max_سائيز;

    ngx_log_error(NGX_LOG_WARN, hinit->pool->لاگرٿم, 0,
                  "could not build optimal %s, you should increase "
                  "either %s_max_size: %i or %s_bucket_size: %i; "
                  "ignoring %s_bucket_size",
                  hinit->name, hinit->name, hinit->max_سائيز,
                  hinit->name, hinit->ٻالٽي_سائيز, hinit->name);

مليو:

    لكل (i = 0; i < سائيز; i++) {
        test[i] = حجم(فراغ *);
    }

    لكل (n = 0; n < nelts; n++) {
        إذا (نالا[n].ڪنجي.معلومات == عدم) {
            استمر;
        }

        ڪنجي = نالا[n].ڪنجي_hash % سائيز;
        len = test[ڪنجي] + NGX_HASH_ELT_سائيز(&نالا[n]);

        إذا (len > 65536 - ngx_cacheline_سائيز) {
            ngx_log_error(NGX_LOG_EMERG, hinit->pool->لاگرٿم, 0,
                          "could not build %s, you should "
                          "increase %s_max_size: %i",
                          hinit->name, hinit->name, hinit->max_سائيز);
            ngx_free(test);
            أرجع NGX_ERROR;
        }

        test[ڪنجي] = (u_short) len;
    }

    len = 0;

    لكل (i = 0; i < سائيز; i++) {
        إذا (test[i] == حجم(فراغ *)) {
            استمر;
        }

        test[i] = (u_short) (ngx_align(test[i], ngx_cacheline_سائيز));

        len += test[i];
    }

    إذا (hinit->hash == عدم) {
        hinit->hash = ngx_pcalloc(hinit->pool, حجم(ngx_hash_wildcard_t)
                                             + سائيز * حجم(ngx_hash_elt_t *));
        إذا (hinit->hash == عدم) {
            ngx_free(test);
            أرجع NGX_ERROR;
        }

        buckets = (ngx_hash_elt_t **)
                      ((u_char *) hinit->hash + حجم(ngx_hash_wildcard_t));

    } وإلا {
        buckets = ngx_pcalloc(hinit->pool, سائيز * حجم(ngx_hash_elt_t *));
        إذا (buckets == عدم) {
            ngx_free(test);
            أرجع NGX_ERROR;
        }
    }

    elts = ngx_palloc(hinit->pool, len + ngx_cacheline_سائيز);
    إذا (elts == عدم) {
        ngx_free(test);
        أرجع NGX_ERROR;
    }

    elts = ngx_align_ptr(elts, ngx_cacheline_سائيز);

    لكل (i = 0; i < سائيز; i++) {
        إذا (test[i] == حجم(فراغ *)) {
            استمر;
        }

        buckets[i] = (ngx_hash_elt_t *) elts;
        elts += test[i];
    }

    لكل (i = 0; i < سائيز; i++) {
        test[i] = 0;
    }

    لكل (n = 0; n < nelts; n++) {
        إذا (نالا[n].ڪنجي.معلومات == عدم) {
            استمر;
        }

        ڪنجي = نالا[n].ڪنجي_hash % سائيز;
        elt = (ngx_hash_elt_t *) ((u_char *) buckets[ڪنجي] + test[ڪنجي]);

        elt->قدر = نالا[n].قدر;
        elt->len = (u_short) نالا[n].ڪنجي.len;

        ngx_strlow(elt->name, نالا[n].ڪنجي.معلومات, نالا[n].ڪنجي.len);

        test[ڪنجي] = (u_short) (test[ڪنجي] + NGX_HASH_ELT_سائيز(&نالا[n]));
    }

    لكل (i = 0; i < سائيز; i++) {
        إذا (buckets[i] == عدم) {
            استمر;
        }

        elt = (ngx_hash_elt_t *) ((u_char *) buckets[i] + test[i]);

        elt->قدر = عدم;
    }

    ngx_free(test);

    hinit->hash->buckets = buckets;
    hinit->hash->سائيز = سائيز;

#إذا 0

    لكل (i = 0; i < سائيز; i++) {
        ngx_str_t   val;
        ngx_uint_t  ڪنجي;

        elt = buckets[i];

        إذا (elt == عدم) {
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->لاگرٿم, 0,
                          "%ui: NULL", i);
            استمر;
        }

        طالما (elt->قدر) {
            val.len = elt->len;
            val.معلومات = &elt->name[0];

            ڪنجي = hinit->ڪنجي(val.معلومات, val.len);

            ngx_log_error(NGX_LOG_ALERT, hinit->pool->لاگرٿم, 0,
                          "%ui: %p \"%V\" %ui", i, elt, &val, ڪنجي);

            elt = (ngx_hash_elt_t *) ngx_align_ptr(&elt->name[0] + elt->len,
                                                   حجم(فراغ *));
        }
    }

#جيڪڏهن_پورو

    أرجع NGX_OK;
}


ngx_int_t
ngx_hash_wildcard_init(ngx_hash_init_t *hinit, ngx_hash_ڪنجي_t *نالا,
    ngx_uint_t nelts)
{
    سائيز_t                len, dot_len;
    ngx_uint_t            i, n, dot;
    ngx_array_t           curr_نالا, ايندڙ_نالا;
    ngx_hash_ڪنجي_t       *name, *ايندڙ_name;
    ngx_hash_init_t       h;
    ngx_hash_wildcard_t  *wdc;

    إذا (ngx_array_init(&curr_نالا, hinit->عارضي_pool, nelts,
                       حجم(ngx_hash_ڪنجي_t))
        != NGX_OK)
    {
        أرجع NGX_ERROR;
    }

    إذا (ngx_array_init(&ايندڙ_نالا, hinit->عارضي_pool, nelts,
                       حجم(ngx_hash_ڪنجي_t))
        != NGX_OK)
    {
        أرجع NGX_ERROR;
    }

    لكل (n = 0; n < nelts; n = i) {

#إذا 0
        ngx_log_error(NGX_LOG_ALERT, hinit->pool->لاگرٿم, 0,
                      "wc0: \"%V\"", &نالا[n].ڪنجي);
#جيڪڏهن_پورو

        dot = 0;

        لكل (len = 0; len < نالا[n].ڪنجي.len; len++) {
            إذا (نالا[n].ڪنجي.معلومات[len] == '.') {
                dot = 1;
                اكسر;
            }
        }

        name = ngx_array_ڌڪو(&curr_نالا);
        إذا (name == عدم) {
            أرجع NGX_ERROR;
        }

        name->ڪنجي.len = len;
        name->ڪنجي.معلومات = نالا[n].ڪنجي.معلومات;
        name->ڪنجي_hash = hinit->ڪنجي(name->ڪنجي.معلومات, name->ڪنجي.len);
        name->قدر = نالا[n].قدر;

#إذا 0
        ngx_log_error(NGX_LOG_ALERT, hinit->pool->لاگرٿم, 0,
                      "wc1: \"%V\" %ui", &name->ڪنجي, dot);
#جيڪڏهن_پورو

        dot_len = len + 1;

        إذا (dot) {
            len++;
        }

        ايندڙ_نالا.nelts = 0;

        إذا (نالا[n].ڪنجي.len != len) {
            ايندڙ_name = ngx_array_ڌڪو(&ايندڙ_نالا);
            إذا (ايندڙ_name == عدم) {
                أرجع NGX_ERROR;
            }

            ايندڙ_name->ڪنجي.len = نالا[n].ڪنجي.len - len;
            ايندڙ_name->ڪنجي.معلومات = نالا[n].ڪنجي.معلومات + len;
            ايندڙ_name->ڪنجي_hash = 0;
            ايندڙ_name->قدر = نالا[n].قدر;

#إذا 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->لاگرٿم, 0,
                          "wc2: \"%V\"", &ايندڙ_name->ڪنجي);
#جيڪڏهن_پورو
        }

        لكل (i = n + 1; i < nelts; i++) {
            إذا (ngx_strncmp(نالا[n].ڪنجي.معلومات, نالا[i].ڪنجي.معلومات, len) != 0) {
                اكسر;
            }

            إذا (!dot
                && نالا[i].ڪنجي.len > len
                && نالا[i].ڪنجي.معلومات[len] != '.')
            {
                اكسر;
            }

            ايندڙ_name = ngx_array_ڌڪو(&ايندڙ_نالا);
            إذا (ايندڙ_name == عدم) {
                أرجع NGX_ERROR;
            }

            ايندڙ_name->ڪنجي.len = نالا[i].ڪنجي.len - dot_len;
            ايندڙ_name->ڪنجي.معلومات = نالا[i].ڪنجي.معلومات + dot_len;
            ايندڙ_name->ڪنجي_hash = 0;
            ايندڙ_name->قدر = نالا[i].قدر;

#إذا 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->لاگرٿم, 0,
                          "wc3: \"%V\"", &ايندڙ_name->ڪنجي);
#جيڪڏهن_پورو
        }

        إذا (ايندڙ_نالا.nelts) {

            h = *hinit;
            h.hash = عدم;

            إذا (ngx_hash_wildcard_init(&h, (ngx_hash_ڪنجي_t *) ايندڙ_نالا.elts,
                                       ايندڙ_نالا.nelts)
                != NGX_OK)
            {
                أرجع NGX_ERROR;
            }

            wdc = (ngx_hash_wildcard_t *) h.hash;

            إذا (نالا[n].ڪنجي.len == len) {
                wdc->قدر = نالا[n].قدر;
            }

            name->قدر = (فراغ *) ((uintptr_t) wdc | (dot ? 3 : 2));

        } وإلا إذا (dot) {
            name->قدر = (فراغ *) ((uintptr_t) name->قدر | 1);
        }
    }

    إذا (ngx_hash_init(hinit, (ngx_hash_ڪنجي_t *) curr_نالا.elts,
                      curr_نالا.nelts)
        != NGX_OK)
    {
        أرجع NGX_ERROR;
    }

    أرجع NGX_OK;
}


ngx_uint_t
ngx_hash_ڪنجي(u_char *معلومات, سائيز_t len)
{
    ngx_uint_t  i, ڪنجي;

    ڪنجي = 0;

    لكل (i = 0; i < len; i++) {
        ڪنجي = ngx_hash(ڪنجي, معلومات[i]);
    }

    أرجع ڪنجي;
}


ngx_uint_t
ngx_hash_ڪنجي_lc(u_char *معلومات, سائيز_t len)
{
    ngx_uint_t  i, ڪنجي;

    ڪنجي = 0;

    لكل (i = 0; i < len; i++) {
        ڪنجي = ngx_hash(ڪنجي, ngx_tolower(معلومات[i]));
    }

    أرجع ڪنجي;
}


ngx_uint_t
ngx_hash_strlow(u_char *dst, u_char *src, سائيز_t n)
{
    ngx_uint_t  ڪنجي;

    ڪنجي = 0;

    طالما (n--) {
        *dst = ngx_tolower(*src);
        ڪنجي = ngx_hash(ڪنجي, *dst);
        dst++;
        src++;
    }

    أرجع ڪنجي;
}


ngx_int_t
ngx_hash_ڪنجيون_array_init(ngx_hash_ڪنجيون_arrays_t *ha, ngx_uint_t type)
{
    ngx_uint_t  asize;

    إذا (type == NGX_HASH_SMALL) {
        asize = 4;
        ha->hsize = 107;

    } وإلا {
        asize = NGX_HASH_LARGE_ASIZE;
        ha->hsize = NGX_HASH_LARGE_HSIZE;
    }

    إذا (ngx_array_init(&ha->ڪنجيون, ha->عارضي_pool, asize, حجم(ngx_hash_ڪنجي_t))
        != NGX_OK)
    {
        أرجع NGX_ERROR;
    }

    إذا (ngx_array_init(&ha->dns_wc_سر, ha->عارضي_pool, asize,
                       حجم(ngx_hash_ڪنجي_t))
        != NGX_OK)
    {
        أرجع NGX_ERROR;
    }

    إذا (ngx_array_init(&ha->dns_wc_پڇ, ha->عارضي_pool, asize,
                       حجم(ngx_hash_ڪنجي_t))
        != NGX_OK)
    {
        أرجع NGX_ERROR;
    }

    ha->ڪنجيون_hash = ngx_pcalloc(ha->عارضي_pool, حجم(ngx_array_t) * ha->hsize);
    إذا (ha->ڪنجيون_hash == عدم) {
        أرجع NGX_ERROR;
    }

    ha->dns_wc_سر_hash = ngx_pcalloc(ha->عارضي_pool,
                                       حجم(ngx_array_t) * ha->hsize);
    إذا (ha->dns_wc_سر_hash == عدم) {
        أرجع NGX_ERROR;
    }

    ha->dns_wc_پڇ_hash = ngx_pcalloc(ha->عارضي_pool,
                                       حجم(ngx_array_t) * ha->hsize);
    إذا (ha->dns_wc_پڇ_hash == عدم) {
        أرجع NGX_ERROR;
    }

    أرجع NGX_OK;
}


ngx_int_t
ngx_hash_جوڙيو_ڪنجي(ngx_hash_ڪنجيون_arrays_t *ha, ngx_str_t *ڪنجي, فراغ *قدر,
    ngx_uint_t flags)
{
    سائيز_t           len;
    u_char          *p;
    ngx_str_t       *name;
    ngx_uint_t       i, k, n, skip, آخري;
    ngx_array_t     *ڪنجيون, *hwc;
    ngx_hash_ڪنجي_t  *hk;

    آخري = ڪنجي->len;

    إذا (flags & NGX_HASH_WILDCARD_ڪنجي) {

        /*
         * supported wildcards:
         *     "*.example.com", ".example.com", and "www.example.*"
         */

        n = 0;

        لكل (i = 0; i < ڪنجي->len; i++) {

            إذا (ڪنجي->معلومات[i] == '*') {
                إذا (++n > 1) {
                    أرجع NGX_DECLINED;
                }
            }

            إذا (ڪنجي->معلومات[i] == '.' && ڪنجي->معلومات[i + 1] == '.') {
                أرجع NGX_DECLINED;
            }

            إذا (ڪنجي->معلومات[i] == '\0') {
                أرجع NGX_DECLINED;
            }
        }

        إذا (ڪنجي->len > 1 && ڪنجي->معلومات[0] == '.') {
            skip = 1;
            اذهب wildcard;
        }

        إذا (ڪنجي->len > 2) {

            إذا (ڪنجي->معلومات[0] == '*' && ڪنجي->معلومات[1] == '.') {
                skip = 2;
                اذهب wildcard;
            }

            إذا (ڪنجي->معلومات[i - 2] == '.' && ڪنجي->معلومات[i - 1] == '*') {
                skip = 0;
                آخري -= 2;
                اذهب wildcard;
            }
        }

        إذا (n) {
            أرجع NGX_DECLINED;
        }
    }

    /* exact hash */

    k = 0;

    لكل (i = 0; i < آخري; i++) {
        إذا (!(flags & NGX_HASH_READONLY_ڪنجي)) {
            ڪنجي->معلومات[i] = ngx_tolower(ڪنجي->معلومات[i]);
        }
        k = ngx_hash(k, ڪنجي->معلومات[i]);
    }

    k %= ha->hsize;

    /* check conflicts in exact hash */

    name = ha->ڪنجيون_hash[k].elts;

    إذا (name) {
        لكل (i = 0; i < ha->ڪنجيون_hash[k].nelts; i++) {
            إذا (آخري != name[i].len) {
                استمر;
            }

            إذا (ngx_strncmp(ڪنجي->معلومات, name[i].معلومات, آخري) == 0) {
                أرجع NGX_BUSY;
            }
        }

    } وإلا {
        إذا (ngx_array_init(&ha->ڪنجيون_hash[k], ha->عارضي_pool, 4,
                           حجم(ngx_str_t))
            != NGX_OK)
        {
            أرجع NGX_ERROR;
        }
    }

    name = ngx_array_ڌڪو(&ha->ڪنجيون_hash[k]);
    إذا (name == عدم) {
        أرجع NGX_ERROR;
    }

    *name = *ڪنجي;

    hk = ngx_array_ڌڪو(&ha->ڪنجيون);
    إذا (hk == عدم) {
        أرجع NGX_ERROR;
    }

    hk->ڪنجي = *ڪنجي;
    hk->ڪنجي_hash = ngx_hash_ڪنجي(ڪنجي->معلومات, آخري);
    hk->قدر = قدر;

    أرجع NGX_OK;


wildcard:

    /* wildcard hash */

    k = ngx_hash_strlow(&ڪنجي->معلومات[skip], &ڪنجي->معلومات[skip], آخري - skip);

    k %= ha->hsize;

    إذا (skip == 1) {

        /* check conflicts in exact hash for ".example.com" */

        name = ha->ڪنجيون_hash[k].elts;

        إذا (name) {
            len = آخري - skip;

            لكل (i = 0; i < ha->ڪنجيون_hash[k].nelts; i++) {
                إذا (len != name[i].len) {
                    استمر;
                }

                إذا (ngx_strncmp(&ڪنجي->معلومات[1], name[i].معلومات, len) == 0) {
                    أرجع NGX_BUSY;
                }
            }

        } وإلا {
            إذا (ngx_array_init(&ha->ڪنجيون_hash[k], ha->عارضي_pool, 4,
                               حجم(ngx_str_t))
                != NGX_OK)
            {
                أرجع NGX_ERROR;
            }
        }

        name = ngx_array_ڌڪو(&ha->ڪنجيون_hash[k]);
        إذا (name == عدم) {
            أرجع NGX_ERROR;
        }

        name->len = آخري - 1;
        name->معلومات = ngx_pnalloc(ha->عارضي_pool, name->len);
        إذا (name->معلومات == عدم) {
            أرجع NGX_ERROR;
        }

        ngx_memcpy(name->معلومات, &ڪنجي->معلومات[1], name->len);
    }


    إذا (skip) {

        /*
         * convert "*.example.com" to "com.example.\0"
         *      and ".example.com" to "com.example\0"
         */

        p = ngx_pnalloc(ha->عارضي_pool, آخري);
        إذا (p == عدم) {
            أرجع NGX_ERROR;
        }

        len = 0;
        n = 0;

        لكل (i = آخري - 1; i; i--) {
            إذا (ڪنجي->معلومات[i] == '.') {
                ngx_memcpy(&p[n], &ڪنجي->معلومات[i + 1], len);
                n += len;
                p[n++] = '.';
                len = 0;
                استمر;
            }

            len++;
        }

        إذا (len) {
            ngx_memcpy(&p[n], &ڪنجي->معلومات[1], len);
            n += len;
        }

        p[n] = '\0';

        hwc = &ha->dns_wc_سر;
        ڪنجيون = &ha->dns_wc_سر_hash[k];

    } وإلا {

        /* convert "www.example.*" to "www.example\0" */

        آخري++;

        p = ngx_pnalloc(ha->عارضي_pool, آخري);
        إذا (p == عدم) {
            أرجع NGX_ERROR;
        }

        ngx_cpystrn(p, ڪنجي->معلومات, آخري);

        hwc = &ha->dns_wc_پڇ;
        ڪنجيون = &ha->dns_wc_پڇ_hash[k];
    }


    /* check conflicts in wildcard hash */

    name = ڪنجيون->elts;

    إذا (name) {
        len = آخري - skip;

        لكل (i = 0; i < ڪنجيون->nelts; i++) {
            إذا (len != name[i].len) {
                استمر;
            }

            إذا (ngx_strncmp(ڪنجي->معلومات + skip, name[i].معلومات, len) == 0) {
                أرجع NGX_BUSY;
            }
        }

    } وإلا {
        إذا (ngx_array_init(ڪنجيون, ha->عارضي_pool, 4, حجم(ngx_str_t)) != NGX_OK)
        {
            أرجع NGX_ERROR;
        }
    }

    name = ngx_array_ڌڪو(ڪنجيون);
    إذا (name == عدم) {
        أرجع NGX_ERROR;
    }

    name->len = آخري - skip;
    name->معلومات = ngx_pnalloc(ha->عارضي_pool, name->len);
    إذا (name->معلومات == عدم) {
        أرجع NGX_ERROR;
    }

    ngx_memcpy(name->معلومات, ڪنجي->معلومات + skip, name->len);


    /* add to wildcard hash */

    hk = ngx_array_ڌڪو(hwc);
    إذا (hk == عدم) {
        أرجع NGX_ERROR;
    }

    hk->ڪنجي.len = آخري - 1;
    hk->ڪنجي.معلومات = p;
    hk->ڪنجي_hash = 0;
    hk->قدر = قدر;

    أرجع NGX_OK;
}
