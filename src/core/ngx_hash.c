
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#شامل <ngx_config.h>
#شامل <ngx_core.h>


خالی *
ngx_hash_ڈھونڈو(ngx_hash_t *hash, ngx_uint_t کلید, u_char *name, سائز_t len)
{
    ngx_uint_t       i;
    ngx_hash_elt_t  *elt;

#اگر 0
    ngx_log_error(NGX_LOG_ALERT, ngx_چکر->لوگارتھم, 0, "hf:\"%*s\"", len, name);
#اگر_ختم

    elt = hash->buckets[کلید % hash->سائز];

    اگر (elt == تہی) {
        واپس تہی;
    }

    جب (elt->قدر) {
        اگر (len != (سائز_t) elt->len) {
            جاؤ اگلا;
        }

        برائے (i = 0; i < len; i++) {
            اگر (name[i] != elt->name[i]) {
                جاؤ اگلا;
            }
        }

        واپس elt->قدر;

    اگلا:

        elt = (ngx_hash_elt_t *) ngx_align_ptr(&elt->name[0] + elt->len,
                                               حجم(خالی *));
        جاری;
    }

    واپس تہی;
}


خالی *
ngx_hash_ڈھونڈو_wc_سر(ngx_hash_wildcard_t *hwc, u_char *name, سائز_t len)
{
    خالی        *قدر;
    ngx_uint_t   i, n, کلید;

#اگر 0
    ngx_log_error(NGX_LOG_ALERT, ngx_چکر->لوگارتھم, 0, "wch:\"%*s\"", len, name);
#اگر_ختم

    n = len;

    جب (n) {
        اگر (name[n - 1] == '.') {
            توڑو;
        }

        n--;
    }

    کلید = 0;

    برائے (i = n; i < len; i++) {
        کلید = ngx_hash(کلید, name[i]);
    }

#اگر 0
    ngx_log_error(NGX_LOG_ALERT, ngx_چکر->لوگارتھم, 0, "key:\"%ui\"", کلید);
#اگر_ختم

    قدر = ngx_hash_ڈھونڈو(&hwc->hash, کلید, &name[n], len - n);

#اگر 0
    ngx_log_error(NGX_LOG_ALERT, ngx_چکر->لوگارتھم, 0, "value:\"%p\"", قدر);
#اگر_ختم

    اگر (قدر) {

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

        اگر ((uintptr_t) قدر & 2) {

            اگر (n == 0) {

                /* "example.com" */

                اگر ((uintptr_t) قدر & 1) {
                    واپس تہی;
                }

                hwc = (ngx_hash_wildcard_t *)
                                          ((uintptr_t) قدر & (uintptr_t) ~3);
                واپس hwc->قدر;
            }

            hwc = (ngx_hash_wildcard_t *) ((uintptr_t) قدر & (uintptr_t) ~3);

            قدر = ngx_hash_ڈھونڈو_wc_سر(hwc, name, n - 1);

            اگر (قدر) {
                واپس قدر;
            }

            واپس hwc->قدر;
        }

        اگر ((uintptr_t) قدر & 1) {

            اگر (n == 0) {

                /* "example.com" */

                واپس تہی;
            }

            واپس (خالی *) ((uintptr_t) قدر & (uintptr_t) ~3);
        }

        واپس قدر;
    }

    واپس hwc->قدر;
}


خالی *
ngx_hash_ڈھونڈو_wc_دم(ngx_hash_wildcard_t *hwc, u_char *name, سائز_t len)
{
    خالی        *قدر;
    ngx_uint_t   i, کلید;

#اگر 0
    ngx_log_error(NGX_LOG_ALERT, ngx_چکر->لوگارتھم, 0, "wct:\"%*s\"", len, name);
#اگر_ختم

    کلید = 0;

    برائے (i = 0; i < len; i++) {
        اگر (name[i] == '.') {
            توڑو;
        }

        کلید = ngx_hash(کلید, name[i]);
    }

    اگر (i == len) {
        واپس تہی;
    }

#اگر 0
    ngx_log_error(NGX_LOG_ALERT, ngx_چکر->لوگارتھم, 0, "key:\"%ui\"", کلید);
#اگر_ختم

    قدر = ngx_hash_ڈھونڈو(&hwc->hash, کلید, name, i);

#اگر 0
    ngx_log_error(NGX_LOG_ALERT, ngx_چکر->لوگارتھم, 0, "value:\"%p\"", قدر);
#اگر_ختم

    اگر (قدر) {

        /*
         * the 2 low bits of value have the special meaning:
         *     00 - value is data pointer;
         *     11 - value is pointer to wildcard hash allowing "example.*".
         */

        اگر ((uintptr_t) قدر & 2) {

            i++;

            hwc = (ngx_hash_wildcard_t *) ((uintptr_t) قدر & (uintptr_t) ~3);

            قدر = ngx_hash_ڈھونڈو_wc_دم(hwc, &name[i], len - i);

            اگر (قدر) {
                واپس قدر;
            }

            واپس hwc->قدر;
        }

        واپس قدر;
    }

    واپس hwc->قدر;
}


خالی *
ngx_hash_ڈھونڈو_combined(ngx_hash_combined_t *hash, ngx_uint_t کلید, u_char *name,
    سائز_t len)
{
    خالی  *قدر;

    اگر (hash->hash.buckets) {
        قدر = ngx_hash_ڈھونڈو(&hash->hash, کلید, name, len);

        اگر (قدر) {
            واپس قدر;
        }
    }

    اگر (len == 0) {
        واپس تہی;
    }

    اگر (hash->wc_سر && hash->wc_سر->hash.buckets) {
        قدر = ngx_hash_ڈھونڈو_wc_سر(hash->wc_سر, name, len);

        اگر (قدر) {
            واپس قدر;
        }
    }

    اگر (hash->wc_دم && hash->wc_دم->hash.buckets) {
        قدر = ngx_hash_ڈھونڈو_wc_دم(hash->wc_دم, name, len);

        اگر (قدر) {
            واپس قدر;
        }
    }

    واپس تہی;
}


#تعریف_کرو NGX_HASH_ELT_سائز(name)                                               \
    (حجم(خالی *) + ngx_align((name)->کلید.len + 2, حجم(خالی *)))

ngx_int_t
ngx_hash_init(ngx_hash_init_t *hinit, ngx_hash_کلید_t *نام, ngx_uint_t nelts)
{
    u_char          *elts;
    سائز_t           len;
    u_short         *test;
    ngx_uint_t       i, n, کلید, سائز, آغاز, بالٹی_سائز;
    ngx_hash_elt_t  *elt, **buckets;

    اگر (hinit->max_سائز == 0) {
        ngx_log_error(NGX_LOG_EMERG, hinit->pool->لوگارتھم, 0,
                      "could not build %s, you should "
                      "increase %s_max_size: %i",
                      hinit->name, hinit->name, hinit->max_سائز);
        واپس NGX_ERROR;
    }

    اگر (hinit->بالٹی_سائز > 65536 - ngx_cacheline_سائز) {
        ngx_log_error(NGX_LOG_EMERG, hinit->pool->لوگارتھم, 0,
                      "could not build %s, too large "
                      "%s_bucket_size: %i",
                      hinit->name, hinit->name, hinit->بالٹی_سائز);
        واپس NGX_ERROR;
    }

    برائے (n = 0; n < nelts; n++) {
        اگر (نام[n].کلید.ڈیٹا == تہی) {
            جاری;
        }

        اگر (hinit->بالٹی_سائز < NGX_HASH_ELT_سائز(&نام[n]) + حجم(خالی *))
        {
            ngx_log_error(NGX_LOG_EMERG, hinit->pool->لوگارتھم, 0,
                          "could not build %s, you should "
                          "increase %s_bucket_size: %i",
                          hinit->name, hinit->name, hinit->بالٹی_سائز);
            واپس NGX_ERROR;
        }
    }

    test = ngx_alloc(hinit->max_سائز * حجم(u_short), hinit->pool->لوگارتھم);
    اگر (test == تہی) {
        واپس NGX_ERROR;
    }

    بالٹی_سائز = hinit->بالٹی_سائز - حجم(خالی *);

    آغاز = nelts / (بالٹی_سائز / (2 * حجم(خالی *)));
    آغاز = آغاز ? آغاز : 1;

    اگر (hinit->max_سائز > 10000 && nelts && hinit->max_سائز / nelts < 100) {
        آغاز = hinit->max_سائز - 1000;
    }

    برائے (سائز = آغاز; سائز <= hinit->max_سائز; سائز++) {

        ngx_memzero(test, سائز * حجم(u_short));

        برائے (n = 0; n < nelts; n++) {
            اگر (نام[n].کلید.ڈیٹا == تہی) {
                جاری;
            }

            کلید = نام[n].کلید_hash % سائز;
            len = test[کلید] + NGX_HASH_ELT_سائز(&نام[n]);

#اگر 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->لوگارتھم, 0,
                          "%ui: %ui %uz \"%V\"",
                          سائز, کلید, len, &نام[n].کلید);
#اگر_ختم

            اگر (len > بالٹی_سائز) {
                جاؤ اگلا;
            }

            test[کلید] = (u_short) len;
        }

        جاؤ ملا;

    اگلا:

        جاری;
    }

    سائز = hinit->max_سائز;

    ngx_log_error(NGX_LOG_WARN, hinit->pool->لوگارتھم, 0,
                  "could not build optimal %s, you should increase "
                  "either %s_max_size: %i or %s_bucket_size: %i; "
                  "ignoring %s_bucket_size",
                  hinit->name, hinit->name, hinit->max_سائز,
                  hinit->name, hinit->بالٹی_سائز, hinit->name);

ملا:

    برائے (i = 0; i < سائز; i++) {
        test[i] = حجم(خالی *);
    }

    برائے (n = 0; n < nelts; n++) {
        اگر (نام[n].کلید.ڈیٹا == تہی) {
            جاری;
        }

        کلید = نام[n].کلید_hash % سائز;
        len = test[کلید] + NGX_HASH_ELT_سائز(&نام[n]);

        اگر (len > 65536 - ngx_cacheline_سائز) {
            ngx_log_error(NGX_LOG_EMERG, hinit->pool->لوگارتھم, 0,
                          "could not build %s, you should "
                          "increase %s_max_size: %i",
                          hinit->name, hinit->name, hinit->max_سائز);
            ngx_free(test);
            واپس NGX_ERROR;
        }

        test[کلید] = (u_short) len;
    }

    len = 0;

    برائے (i = 0; i < سائز; i++) {
        اگر (test[i] == حجم(خالی *)) {
            جاری;
        }

        test[i] = (u_short) (ngx_align(test[i], ngx_cacheline_سائز));

        len += test[i];
    }

    اگر (hinit->hash == تہی) {
        hinit->hash = ngx_pcalloc(hinit->pool, حجم(ngx_hash_wildcard_t)
                                             + سائز * حجم(ngx_hash_elt_t *));
        اگر (hinit->hash == تہی) {
            ngx_free(test);
            واپس NGX_ERROR;
        }

        buckets = (ngx_hash_elt_t **)
                      ((u_char *) hinit->hash + حجم(ngx_hash_wildcard_t));

    } ورنہ {
        buckets = ngx_pcalloc(hinit->pool, سائز * حجم(ngx_hash_elt_t *));
        اگر (buckets == تہی) {
            ngx_free(test);
            واپس NGX_ERROR;
        }
    }

    elts = ngx_palloc(hinit->pool, len + ngx_cacheline_سائز);
    اگر (elts == تہی) {
        ngx_free(test);
        واپس NGX_ERROR;
    }

    elts = ngx_align_ptr(elts, ngx_cacheline_سائز);

    برائے (i = 0; i < سائز; i++) {
        اگر (test[i] == حجم(خالی *)) {
            جاری;
        }

        buckets[i] = (ngx_hash_elt_t *) elts;
        elts += test[i];
    }

    برائے (i = 0; i < سائز; i++) {
        test[i] = 0;
    }

    برائے (n = 0; n < nelts; n++) {
        اگر (نام[n].کلید.ڈیٹا == تہی) {
            جاری;
        }

        کلید = نام[n].کلید_hash % سائز;
        elt = (ngx_hash_elt_t *) ((u_char *) buckets[کلید] + test[کلید]);

        elt->قدر = نام[n].قدر;
        elt->len = (u_short) نام[n].کلید.len;

        ngx_strlow(elt->name, نام[n].کلید.ڈیٹا, نام[n].کلید.len);

        test[کلید] = (u_short) (test[کلید] + NGX_HASH_ELT_سائز(&نام[n]));
    }

    برائے (i = 0; i < سائز; i++) {
        اگر (buckets[i] == تہی) {
            جاری;
        }

        elt = (ngx_hash_elt_t *) ((u_char *) buckets[i] + test[i]);

        elt->قدر = تہی;
    }

    ngx_free(test);

    hinit->hash->buckets = buckets;
    hinit->hash->سائز = سائز;

#اگر 0

    برائے (i = 0; i < سائز; i++) {
        ngx_str_t   val;
        ngx_uint_t  کلید;

        elt = buckets[i];

        اگر (elt == تہی) {
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->لوگارتھم, 0,
                          "%ui: NULL", i);
            جاری;
        }

        جب (elt->قدر) {
            val.len = elt->len;
            val.ڈیٹا = &elt->name[0];

            کلید = hinit->کلید(val.ڈیٹا, val.len);

            ngx_log_error(NGX_LOG_ALERT, hinit->pool->لوگارتھم, 0,
                          "%ui: %p \"%V\" %ui", i, elt, &val, کلید);

            elt = (ngx_hash_elt_t *) ngx_align_ptr(&elt->name[0] + elt->len,
                                                   حجم(خالی *));
        }
    }

#اگر_ختم

    واپس NGX_OK;
}


ngx_int_t
ngx_hash_wildcard_init(ngx_hash_init_t *hinit, ngx_hash_کلید_t *نام,
    ngx_uint_t nelts)
{
    سائز_t                len, dot_len;
    ngx_uint_t            i, n, dot;
    ngx_array_t           curr_نام, اگلا_نام;
    ngx_hash_کلید_t       *name, *اگلا_name;
    ngx_hash_init_t       h;
    ngx_hash_wildcard_t  *wdc;

    اگر (ngx_array_init(&curr_نام, hinit->عارضی_pool, nelts,
                       حجم(ngx_hash_کلید_t))
        != NGX_OK)
    {
        واپس NGX_ERROR;
    }

    اگر (ngx_array_init(&اگلا_نام, hinit->عارضی_pool, nelts,
                       حجم(ngx_hash_کلید_t))
        != NGX_OK)
    {
        واپس NGX_ERROR;
    }

    برائے (n = 0; n < nelts; n = i) {

#اگر 0
        ngx_log_error(NGX_LOG_ALERT, hinit->pool->لوگارتھم, 0,
                      "wc0: \"%V\"", &نام[n].کلید);
#اگر_ختم

        dot = 0;

        برائے (len = 0; len < نام[n].کلید.len; len++) {
            اگر (نام[n].کلید.ڈیٹا[len] == '.') {
                dot = 1;
                توڑو;
            }
        }

        name = ngx_array_ڈالو(&curr_نام);
        اگر (name == تہی) {
            واپس NGX_ERROR;
        }

        name->کلید.len = len;
        name->کلید.ڈیٹا = نام[n].کلید.ڈیٹا;
        name->کلید_hash = hinit->کلید(name->کلید.ڈیٹا, name->کلید.len);
        name->قدر = نام[n].قدر;

#اگر 0
        ngx_log_error(NGX_LOG_ALERT, hinit->pool->لوگارتھم, 0,
                      "wc1: \"%V\" %ui", &name->کلید, dot);
#اگر_ختم

        dot_len = len + 1;

        اگر (dot) {
            len++;
        }

        اگلا_نام.nelts = 0;

        اگر (نام[n].کلید.len != len) {
            اگلا_name = ngx_array_ڈالو(&اگلا_نام);
            اگر (اگلا_name == تہی) {
                واپس NGX_ERROR;
            }

            اگلا_name->کلید.len = نام[n].کلید.len - len;
            اگلا_name->کلید.ڈیٹا = نام[n].کلید.ڈیٹا + len;
            اگلا_name->کلید_hash = 0;
            اگلا_name->قدر = نام[n].قدر;

#اگر 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->لوگارتھم, 0,
                          "wc2: \"%V\"", &اگلا_name->کلید);
#اگر_ختم
        }

        برائے (i = n + 1; i < nelts; i++) {
            اگر (ngx_strncmp(نام[n].کلید.ڈیٹا, نام[i].کلید.ڈیٹا, len) != 0) {
                توڑو;
            }

            اگر (!dot
                && نام[i].کلید.len > len
                && نام[i].کلید.ڈیٹا[len] != '.')
            {
                توڑو;
            }

            اگلا_name = ngx_array_ڈالو(&اگلا_نام);
            اگر (اگلا_name == تہی) {
                واپس NGX_ERROR;
            }

            اگلا_name->کلید.len = نام[i].کلید.len - dot_len;
            اگلا_name->کلید.ڈیٹا = نام[i].کلید.ڈیٹا + dot_len;
            اگلا_name->کلید_hash = 0;
            اگلا_name->قدر = نام[i].قدر;

#اگر 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->لوگارتھم, 0,
                          "wc3: \"%V\"", &اگلا_name->کلید);
#اگر_ختم
        }

        اگر (اگلا_نام.nelts) {

            h = *hinit;
            h.hash = تہی;

            اگر (ngx_hash_wildcard_init(&h, (ngx_hash_کلید_t *) اگلا_نام.elts,
                                       اگلا_نام.nelts)
                != NGX_OK)
            {
                واپس NGX_ERROR;
            }

            wdc = (ngx_hash_wildcard_t *) h.hash;

            اگر (نام[n].کلید.len == len) {
                wdc->قدر = نام[n].قدر;
            }

            name->قدر = (خالی *) ((uintptr_t) wdc | (dot ? 3 : 2));

        } ورنہ اگر (dot) {
            name->قدر = (خالی *) ((uintptr_t) name->قدر | 1);
        }
    }

    اگر (ngx_hash_init(hinit, (ngx_hash_کلید_t *) curr_نام.elts,
                      curr_نام.nelts)
        != NGX_OK)
    {
        واپس NGX_ERROR;
    }

    واپس NGX_OK;
}


ngx_uint_t
ngx_hash_کلید(u_char *ڈیٹا, سائز_t len)
{
    ngx_uint_t  i, کلید;

    کلید = 0;

    برائے (i = 0; i < len; i++) {
        کلید = ngx_hash(کلید, ڈیٹا[i]);
    }

    واپس کلید;
}


ngx_uint_t
ngx_hash_کلید_lc(u_char *ڈیٹا, سائز_t len)
{
    ngx_uint_t  i, کلید;

    کلید = 0;

    برائے (i = 0; i < len; i++) {
        کلید = ngx_hash(کلید, ngx_tolower(ڈیٹا[i]));
    }

    واپس کلید;
}


ngx_uint_t
ngx_hash_strlow(u_char *dst, u_char *src, سائز_t n)
{
    ngx_uint_t  کلید;

    کلید = 0;

    جب (n--) {
        *dst = ngx_tolower(*src);
        کلید = ngx_hash(کلید, *dst);
        dst++;
        src++;
    }

    واپس کلید;
}


ngx_int_t
ngx_hash_کلیدیں_array_init(ngx_hash_کلیدیں_arrays_t *ha, ngx_uint_t type)
{
    ngx_uint_t  asize;

    اگر (type == NGX_HASH_SMALL) {
        asize = 4;
        ha->hsize = 107;

    } ورنہ {
        asize = NGX_HASH_LARGE_ASIZE;
        ha->hsize = NGX_HASH_LARGE_HSIZE;
    }

    اگر (ngx_array_init(&ha->کلیدیں, ha->عارضی_pool, asize, حجم(ngx_hash_کلید_t))
        != NGX_OK)
    {
        واپس NGX_ERROR;
    }

    اگر (ngx_array_init(&ha->dns_wc_سر, ha->عارضی_pool, asize,
                       حجم(ngx_hash_کلید_t))
        != NGX_OK)
    {
        واپس NGX_ERROR;
    }

    اگر (ngx_array_init(&ha->dns_wc_دم, ha->عارضی_pool, asize,
                       حجم(ngx_hash_کلید_t))
        != NGX_OK)
    {
        واپس NGX_ERROR;
    }

    ha->کلیدیں_hash = ngx_pcalloc(ha->عارضی_pool, حجم(ngx_array_t) * ha->hsize);
    اگر (ha->کلیدیں_hash == تہی) {
        واپس NGX_ERROR;
    }

    ha->dns_wc_سر_hash = ngx_pcalloc(ha->عارضی_pool,
                                       حجم(ngx_array_t) * ha->hsize);
    اگر (ha->dns_wc_سر_hash == تہی) {
        واپس NGX_ERROR;
    }

    ha->dns_wc_دم_hash = ngx_pcalloc(ha->عارضی_pool,
                                       حجم(ngx_array_t) * ha->hsize);
    اگر (ha->dns_wc_دم_hash == تہی) {
        واپس NGX_ERROR;
    }

    واپس NGX_OK;
}


ngx_int_t
ngx_hash_جوڑو_کلید(ngx_hash_کلیدیں_arrays_t *ha, ngx_str_t *کلید, خالی *قدر,
    ngx_uint_t flags)
{
    سائز_t           len;
    u_char          *p;
    ngx_str_t       *name;
    ngx_uint_t       i, k, n, skip, آخری;
    ngx_array_t     *کلیدیں, *hwc;
    ngx_hash_کلید_t  *hk;

    آخری = کلید->len;

    اگر (flags & NGX_HASH_WILDCARD_کلید) {

        /*
         * supported wildcards:
         *     "*.example.com", ".example.com", and "www.example.*"
         */

        n = 0;

        برائے (i = 0; i < کلید->len; i++) {

            اگر (کلید->ڈیٹا[i] == '*') {
                اگر (++n > 1) {
                    واپس NGX_DECLINED;
                }
            }

            اگر (کلید->ڈیٹا[i] == '.' && کلید->ڈیٹا[i + 1] == '.') {
                واپس NGX_DECLINED;
            }

            اگر (کلید->ڈیٹا[i] == '\0') {
                واپس NGX_DECLINED;
            }
        }

        اگر (کلید->len > 1 && کلید->ڈیٹا[0] == '.') {
            skip = 1;
            جاؤ wildcard;
        }

        اگر (کلید->len > 2) {

            اگر (کلید->ڈیٹا[0] == '*' && کلید->ڈیٹا[1] == '.') {
                skip = 2;
                جاؤ wildcard;
            }

            اگر (کلید->ڈیٹا[i - 2] == '.' && کلید->ڈیٹا[i - 1] == '*') {
                skip = 0;
                آخری -= 2;
                جاؤ wildcard;
            }
        }

        اگر (n) {
            واپس NGX_DECLINED;
        }
    }

    /* exact hash */

    k = 0;

    برائے (i = 0; i < آخری; i++) {
        اگر (!(flags & NGX_HASH_READONLY_کلید)) {
            کلید->ڈیٹا[i] = ngx_tolower(کلید->ڈیٹا[i]);
        }
        k = ngx_hash(k, کلید->ڈیٹا[i]);
    }

    k %= ha->hsize;

    /* check conflicts in exact hash */

    name = ha->کلیدیں_hash[k].elts;

    اگر (name) {
        برائے (i = 0; i < ha->کلیدیں_hash[k].nelts; i++) {
            اگر (آخری != name[i].len) {
                جاری;
            }

            اگر (ngx_strncmp(کلید->ڈیٹا, name[i].ڈیٹا, آخری) == 0) {
                واپس NGX_BUSY;
            }
        }

    } ورنہ {
        اگر (ngx_array_init(&ha->کلیدیں_hash[k], ha->عارضی_pool, 4,
                           حجم(ngx_str_t))
            != NGX_OK)
        {
            واپس NGX_ERROR;
        }
    }

    name = ngx_array_ڈالو(&ha->کلیدیں_hash[k]);
    اگر (name == تہی) {
        واپس NGX_ERROR;
    }

    *name = *کلید;

    hk = ngx_array_ڈالو(&ha->کلیدیں);
    اگر (hk == تہی) {
        واپس NGX_ERROR;
    }

    hk->کلید = *کلید;
    hk->کلید_hash = ngx_hash_کلید(کلید->ڈیٹا, آخری);
    hk->قدر = قدر;

    واپس NGX_OK;


wildcard:

    /* wildcard hash */

    k = ngx_hash_strlow(&کلید->ڈیٹا[skip], &کلید->ڈیٹا[skip], آخری - skip);

    k %= ha->hsize;

    اگر (skip == 1) {

        /* check conflicts in exact hash for ".example.com" */

        name = ha->کلیدیں_hash[k].elts;

        اگر (name) {
            len = آخری - skip;

            برائے (i = 0; i < ha->کلیدیں_hash[k].nelts; i++) {
                اگر (len != name[i].len) {
                    جاری;
                }

                اگر (ngx_strncmp(&کلید->ڈیٹا[1], name[i].ڈیٹا, len) == 0) {
                    واپس NGX_BUSY;
                }
            }

        } ورنہ {
            اگر (ngx_array_init(&ha->کلیدیں_hash[k], ha->عارضی_pool, 4,
                               حجم(ngx_str_t))
                != NGX_OK)
            {
                واپس NGX_ERROR;
            }
        }

        name = ngx_array_ڈالو(&ha->کلیدیں_hash[k]);
        اگر (name == تہی) {
            واپس NGX_ERROR;
        }

        name->len = آخری - 1;
        name->ڈیٹا = ngx_pnalloc(ha->عارضی_pool, name->len);
        اگر (name->ڈیٹا == تہی) {
            واپس NGX_ERROR;
        }

        ngx_memcpy(name->ڈیٹا, &کلید->ڈیٹا[1], name->len);
    }


    اگر (skip) {

        /*
         * convert "*.example.com" to "com.example.\0"
         *      and ".example.com" to "com.example\0"
         */

        p = ngx_pnalloc(ha->عارضی_pool, آخری);
        اگر (p == تہی) {
            واپس NGX_ERROR;
        }

        len = 0;
        n = 0;

        برائے (i = آخری - 1; i; i--) {
            اگر (کلید->ڈیٹا[i] == '.') {
                ngx_memcpy(&p[n], &کلید->ڈیٹا[i + 1], len);
                n += len;
                p[n++] = '.';
                len = 0;
                جاری;
            }

            len++;
        }

        اگر (len) {
            ngx_memcpy(&p[n], &کلید->ڈیٹا[1], len);
            n += len;
        }

        p[n] = '\0';

        hwc = &ha->dns_wc_سر;
        کلیدیں = &ha->dns_wc_سر_hash[k];

    } ورنہ {

        /* convert "www.example.*" to "www.example\0" */

        آخری++;

        p = ngx_pnalloc(ha->عارضی_pool, آخری);
        اگر (p == تہی) {
            واپس NGX_ERROR;
        }

        ngx_cpystrn(p, کلید->ڈیٹا, آخری);

        hwc = &ha->dns_wc_دم;
        کلیدیں = &ha->dns_wc_دم_hash[k];
    }


    /* check conflicts in wildcard hash */

    name = کلیدیں->elts;

    اگر (name) {
        len = آخری - skip;

        برائے (i = 0; i < کلیدیں->nelts; i++) {
            اگر (len != name[i].len) {
                جاری;
            }

            اگر (ngx_strncmp(کلید->ڈیٹا + skip, name[i].ڈیٹا, len) == 0) {
                واپس NGX_BUSY;
            }
        }

    } ورنہ {
        اگر (ngx_array_init(کلیدیں, ha->عارضی_pool, 4, حجم(ngx_str_t)) != NGX_OK)
        {
            واپس NGX_ERROR;
        }
    }

    name = ngx_array_ڈالو(کلیدیں);
    اگر (name == تہی) {
        واپس NGX_ERROR;
    }

    name->len = آخری - skip;
    name->ڈیٹا = ngx_pnalloc(ha->عارضی_pool, name->len);
    اگر (name->ڈیٹا == تہی) {
        واپس NGX_ERROR;
    }

    ngx_memcpy(name->ڈیٹا, کلید->ڈیٹا + skip, name->len);


    /* add to wildcard hash */

    hk = ngx_array_ڈالو(hwc);
    اگر (hk == تہی) {
        واپس NGX_ERROR;
    }

    hk->کلید.len = آخری - 1;
    hk->کلید.ڈیٹا = p;
    hk->کلید_hash = 0;
    hk->قدر = قدر;

    واپس NGX_OK;
}
