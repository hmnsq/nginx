
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


فراغ *
ngx_hash_لبھو(ngx_hash_t *hash, ngx_uint_t کلید, u_char *name, ناپ_t len)
{
    ngx_uint_t       i;
    ngx_hash_elt_t  *elt;

#إذا 0
    ngx_log_error(NGX_LOG_ALERT, ngx_چکر->لوگارتھم, 0, "hf:\"%*s\"", len, name);
#جے_انت

    elt = hash->buckets[کلید % hash->ناپ];

    إذا (elt == عدم) {
        أرجع عدم;
    }

    طالما (elt->قدر) {
        إذا (len != (ناپ_t) elt->len) {
            اذهب اگلا;
        }

        لكل (i = 0; i < len; i++) {
            إذا (name[i] != elt->name[i]) {
                اذهب اگلا;
            }
        }

        أرجع elt->قدر;

    اگلا:

        elt = (ngx_hash_elt_t *) ngx_align_ptr(&elt->name[0] + elt->len,
                                               حجم(فراغ *));
        استمر;
    }

    أرجع عدم;
}


فراغ *
ngx_hash_لبھو_wc_سر(ngx_hash_wildcard_t *hwc, u_char *name, ناپ_t len)
{
    فراغ        *قدر;
    ngx_uint_t   i, n, کلید;

#إذا 0
    ngx_log_error(NGX_LOG_ALERT, ngx_چکر->لوگارتھم, 0, "wch:\"%*s\"", len, name);
#جے_انت

    n = len;

    طالما (n) {
        إذا (name[n - 1] == '.') {
            اكسر;
        }

        n--;
    }

    کلید = 0;

    لكل (i = n; i < len; i++) {
        کلید = ngx_hash(کلید, name[i]);
    }

#إذا 0
    ngx_log_error(NGX_LOG_ALERT, ngx_چکر->لوگارتھم, 0, "key:\"%ui\"", کلید);
#جے_انت

    قدر = ngx_hash_لبھو(&hwc->hash, کلید, &name[n], len - n);

#إذا 0
    ngx_log_error(NGX_LOG_ALERT, ngx_چکر->لوگارتھم, 0, "value:\"%p\"", قدر);
#جے_انت

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

            قدر = ngx_hash_لبھو_wc_سر(hwc, name, n - 1);

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
ngx_hash_لبھو_wc_پوچھ(ngx_hash_wildcard_t *hwc, u_char *name, ناپ_t len)
{
    فراغ        *قدر;
    ngx_uint_t   i, کلید;

#إذا 0
    ngx_log_error(NGX_LOG_ALERT, ngx_چکر->لوگارتھم, 0, "wct:\"%*s\"", len, name);
#جے_انت

    کلید = 0;

    لكل (i = 0; i < len; i++) {
        إذا (name[i] == '.') {
            اكسر;
        }

        کلید = ngx_hash(کلید, name[i]);
    }

    إذا (i == len) {
        أرجع عدم;
    }

#إذا 0
    ngx_log_error(NGX_LOG_ALERT, ngx_چکر->لوگارتھم, 0, "key:\"%ui\"", کلید);
#جے_انت

    قدر = ngx_hash_لبھو(&hwc->hash, کلید, name, i);

#إذا 0
    ngx_log_error(NGX_LOG_ALERT, ngx_چکر->لوگارتھم, 0, "value:\"%p\"", قدر);
#جے_انت

    إذا (قدر) {

        /*
         * the 2 low bits of value have the special meaning:
         *     00 - value is data pointer;
         *     11 - value is pointer to wildcard hash allowing "example.*".
         */

        إذا ((uintptr_t) قدر & 2) {

            i++;

            hwc = (ngx_hash_wildcard_t *) ((uintptr_t) قدر & (uintptr_t) ~3);

            قدر = ngx_hash_لبھو_wc_پوچھ(hwc, &name[i], len - i);

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
ngx_hash_لبھو_combined(ngx_hash_combined_t *hash, ngx_uint_t کلید, u_char *name,
    ناپ_t len)
{
    فراغ  *قدر;

    إذا (hash->hash.buckets) {
        قدر = ngx_hash_لبھو(&hash->hash, کلید, name, len);

        إذا (قدر) {
            أرجع قدر;
        }
    }

    إذا (len == 0) {
        أرجع عدم;
    }

    إذا (hash->wc_سر && hash->wc_سر->hash.buckets) {
        قدر = ngx_hash_لبھو_wc_سر(hash->wc_سر, name, len);

        إذا (قدر) {
            أرجع قدر;
        }
    }

    إذا (hash->wc_پوچھ && hash->wc_پوچھ->hash.buckets) {
        قدر = ngx_hash_لبھو_wc_پوچھ(hash->wc_پوچھ, name, len);

        إذا (قدر) {
            أرجع قدر;
        }
    }

    أرجع عدم;
}


#تعریف_کرو NGX_HASH_ELT_ناپ(name)                                               \
    (حجم(فراغ *) + ngx_align((name)->کلید.len + 2, حجم(فراغ *)))

ngx_int_t
ngx_hash_init(ngx_hash_init_t *hinit, ngx_hash_کلید_t *ناں, ngx_uint_t nelts)
{
    u_char          *elts;
    ناپ_t           len;
    u_short         *test;
    ngx_uint_t       i, n, کلید, ناپ, شروع, بالٹی_ناپ;
    ngx_hash_elt_t  *elt, **buckets;

    إذا (hinit->max_ناپ == 0) {
        ngx_log_error(NGX_LOG_EMERG, hinit->pool->لوگارتھم, 0,
                      "could not build %s, you should "
                      "increase %s_max_size: %i",
                      hinit->name, hinit->name, hinit->max_ناپ);
        أرجع NGX_ERROR;
    }

    إذا (hinit->بالٹی_ناپ > 65536 - ngx_cacheline_ناپ) {
        ngx_log_error(NGX_LOG_EMERG, hinit->pool->لوگارتھم, 0,
                      "could not build %s, too large "
                      "%s_bucket_size: %i",
                      hinit->name, hinit->name, hinit->بالٹی_ناپ);
        أرجع NGX_ERROR;
    }

    لكل (n = 0; n < nelts; n++) {
        إذا (ناں[n].کلید.انکڑے == عدم) {
            استمر;
        }

        إذا (hinit->بالٹی_ناپ < NGX_HASH_ELT_ناپ(&ناں[n]) + حجم(فراغ *))
        {
            ngx_log_error(NGX_LOG_EMERG, hinit->pool->لوگارتھم, 0,
                          "could not build %s, you should "
                          "increase %s_bucket_size: %i",
                          hinit->name, hinit->name, hinit->بالٹی_ناپ);
            أرجع NGX_ERROR;
        }
    }

    test = ngx_alloc(hinit->max_ناپ * حجم(u_short), hinit->pool->لوگارتھم);
    إذا (test == عدم) {
        أرجع NGX_ERROR;
    }

    بالٹی_ناپ = hinit->بالٹی_ناپ - حجم(فراغ *);

    شروع = nelts / (بالٹی_ناپ / (2 * حجم(فراغ *)));
    شروع = شروع ? شروع : 1;

    إذا (hinit->max_ناپ > 10000 && nelts && hinit->max_ناپ / nelts < 100) {
        شروع = hinit->max_ناپ - 1000;
    }

    لكل (ناپ = شروع; ناپ <= hinit->max_ناپ; ناپ++) {

        ngx_memzero(test, ناپ * حجم(u_short));

        لكل (n = 0; n < nelts; n++) {
            إذا (ناں[n].کلید.انکڑے == عدم) {
                استمر;
            }

            کلید = ناں[n].کلید_hash % ناپ;
            len = test[کلید] + NGX_HASH_ELT_ناپ(&ناں[n]);

#إذا 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->لوگارتھم, 0,
                          "%ui: %ui %uz \"%V\"",
                          ناپ, کلید, len, &ناں[n].کلید);
#جے_انت

            إذا (len > بالٹی_ناپ) {
                اذهب اگلا;
            }

            test[کلید] = (u_short) len;
        }

        اذهب لبھیا;

    اگلا:

        استمر;
    }

    ناپ = hinit->max_ناپ;

    ngx_log_error(NGX_LOG_WARN, hinit->pool->لوگارتھم, 0,
                  "could not build optimal %s, you should increase "
                  "either %s_max_size: %i or %s_bucket_size: %i; "
                  "ignoring %s_bucket_size",
                  hinit->name, hinit->name, hinit->max_ناپ,
                  hinit->name, hinit->بالٹی_ناپ, hinit->name);

لبھیا:

    لكل (i = 0; i < ناپ; i++) {
        test[i] = حجم(فراغ *);
    }

    لكل (n = 0; n < nelts; n++) {
        إذا (ناں[n].کلید.انکڑے == عدم) {
            استمر;
        }

        کلید = ناں[n].کلید_hash % ناپ;
        len = test[کلید] + NGX_HASH_ELT_ناپ(&ناں[n]);

        إذا (len > 65536 - ngx_cacheline_ناپ) {
            ngx_log_error(NGX_LOG_EMERG, hinit->pool->لوگارتھم, 0,
                          "could not build %s, you should "
                          "increase %s_max_size: %i",
                          hinit->name, hinit->name, hinit->max_ناپ);
            ngx_free(test);
            أرجع NGX_ERROR;
        }

        test[کلید] = (u_short) len;
    }

    len = 0;

    لكل (i = 0; i < ناپ; i++) {
        إذا (test[i] == حجم(فراغ *)) {
            استمر;
        }

        test[i] = (u_short) (ngx_align(test[i], ngx_cacheline_ناپ));

        len += test[i];
    }

    إذا (hinit->hash == عدم) {
        hinit->hash = ngx_pcalloc(hinit->pool, حجم(ngx_hash_wildcard_t)
                                             + ناپ * حجم(ngx_hash_elt_t *));
        إذا (hinit->hash == عدم) {
            ngx_free(test);
            أرجع NGX_ERROR;
        }

        buckets = (ngx_hash_elt_t **)
                      ((u_char *) hinit->hash + حجم(ngx_hash_wildcard_t));

    } وإلا {
        buckets = ngx_pcalloc(hinit->pool, ناپ * حجم(ngx_hash_elt_t *));
        إذا (buckets == عدم) {
            ngx_free(test);
            أرجع NGX_ERROR;
        }
    }

    elts = ngx_palloc(hinit->pool, len + ngx_cacheline_ناپ);
    إذا (elts == عدم) {
        ngx_free(test);
        أرجع NGX_ERROR;
    }

    elts = ngx_align_ptr(elts, ngx_cacheline_ناپ);

    لكل (i = 0; i < ناپ; i++) {
        إذا (test[i] == حجم(فراغ *)) {
            استمر;
        }

        buckets[i] = (ngx_hash_elt_t *) elts;
        elts += test[i];
    }

    لكل (i = 0; i < ناپ; i++) {
        test[i] = 0;
    }

    لكل (n = 0; n < nelts; n++) {
        إذا (ناں[n].کلید.انکڑے == عدم) {
            استمر;
        }

        کلید = ناں[n].کلید_hash % ناپ;
        elt = (ngx_hash_elt_t *) ((u_char *) buckets[کلید] + test[کلید]);

        elt->قدر = ناں[n].قدر;
        elt->len = (u_short) ناں[n].کلید.len;

        ngx_strlow(elt->name, ناں[n].کلید.انکڑے, ناں[n].کلید.len);

        test[کلید] = (u_short) (test[کلید] + NGX_HASH_ELT_ناپ(&ناں[n]));
    }

    لكل (i = 0; i < ناپ; i++) {
        إذا (buckets[i] == عدم) {
            استمر;
        }

        elt = (ngx_hash_elt_t *) ((u_char *) buckets[i] + test[i]);

        elt->قدر = عدم;
    }

    ngx_free(test);

    hinit->hash->buckets = buckets;
    hinit->hash->ناپ = ناپ;

#إذا 0

    لكل (i = 0; i < ناپ; i++) {
        ngx_str_t   val;
        ngx_uint_t  کلید;

        elt = buckets[i];

        إذا (elt == عدم) {
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->لوگارتھم, 0,
                          "%ui: NULL", i);
            استمر;
        }

        طالما (elt->قدر) {
            val.len = elt->len;
            val.انکڑے = &elt->name[0];

            کلید = hinit->کلید(val.انکڑے, val.len);

            ngx_log_error(NGX_LOG_ALERT, hinit->pool->لوگارتھم, 0,
                          "%ui: %p \"%V\" %ui", i, elt, &val, کلید);

            elt = (ngx_hash_elt_t *) ngx_align_ptr(&elt->name[0] + elt->len,
                                                   حجم(فراغ *));
        }
    }

#جے_انت

    أرجع NGX_OK;
}


ngx_int_t
ngx_hash_wildcard_init(ngx_hash_init_t *hinit, ngx_hash_کلید_t *ناں,
    ngx_uint_t nelts)
{
    ناپ_t                len, dot_len;
    ngx_uint_t            i, n, dot;
    ngx_array_t           curr_ناں, اگلا_ناں;
    ngx_hash_کلید_t       *name, *اگلا_name;
    ngx_hash_init_t       h;
    ngx_hash_wildcard_t  *wdc;

    إذا (ngx_array_init(&curr_ناں, hinit->عارضی_pool, nelts,
                       حجم(ngx_hash_کلید_t))
        != NGX_OK)
    {
        أرجع NGX_ERROR;
    }

    إذا (ngx_array_init(&اگلا_ناں, hinit->عارضی_pool, nelts,
                       حجم(ngx_hash_کلید_t))
        != NGX_OK)
    {
        أرجع NGX_ERROR;
    }

    لكل (n = 0; n < nelts; n = i) {

#إذا 0
        ngx_log_error(NGX_LOG_ALERT, hinit->pool->لوگارتھم, 0,
                      "wc0: \"%V\"", &ناں[n].کلید);
#جے_انت

        dot = 0;

        لكل (len = 0; len < ناں[n].کلید.len; len++) {
            إذا (ناں[n].کلید.انکڑے[len] == '.') {
                dot = 1;
                اكسر;
            }
        }

        name = ngx_array_ڈالو(&curr_ناں);
        إذا (name == عدم) {
            أرجع NGX_ERROR;
        }

        name->کلید.len = len;
        name->کلید.انکڑے = ناں[n].کلید.انکڑے;
        name->کلید_hash = hinit->کلید(name->کلید.انکڑے, name->کلید.len);
        name->قدر = ناں[n].قدر;

#إذا 0
        ngx_log_error(NGX_LOG_ALERT, hinit->pool->لوگارتھم, 0,
                      "wc1: \"%V\" %ui", &name->کلید, dot);
#جے_انت

        dot_len = len + 1;

        إذا (dot) {
            len++;
        }

        اگلا_ناں.nelts = 0;

        إذا (ناں[n].کلید.len != len) {
            اگلا_name = ngx_array_ڈالو(&اگلا_ناں);
            إذا (اگلا_name == عدم) {
                أرجع NGX_ERROR;
            }

            اگلا_name->کلید.len = ناں[n].کلید.len - len;
            اگلا_name->کلید.انکڑے = ناں[n].کلید.انکڑے + len;
            اگلا_name->کلید_hash = 0;
            اگلا_name->قدر = ناں[n].قدر;

#إذا 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->لوگارتھم, 0,
                          "wc2: \"%V\"", &اگلا_name->کلید);
#جے_انت
        }

        لكل (i = n + 1; i < nelts; i++) {
            إذا (ngx_strncmp(ناں[n].کلید.انکڑے, ناں[i].کلید.انکڑے, len) != 0) {
                اكسر;
            }

            إذا (!dot
                && ناں[i].کلید.len > len
                && ناں[i].کلید.انکڑے[len] != '.')
            {
                اكسر;
            }

            اگلا_name = ngx_array_ڈالو(&اگلا_ناں);
            إذا (اگلا_name == عدم) {
                أرجع NGX_ERROR;
            }

            اگلا_name->کلید.len = ناں[i].کلید.len - dot_len;
            اگلا_name->کلید.انکڑے = ناں[i].کلید.انکڑے + dot_len;
            اگلا_name->کلید_hash = 0;
            اگلا_name->قدر = ناں[i].قدر;

#إذا 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->لوگارتھم, 0,
                          "wc3: \"%V\"", &اگلا_name->کلید);
#جے_انت
        }

        إذا (اگلا_ناں.nelts) {

            h = *hinit;
            h.hash = عدم;

            إذا (ngx_hash_wildcard_init(&h, (ngx_hash_کلید_t *) اگلا_ناں.elts,
                                       اگلا_ناں.nelts)
                != NGX_OK)
            {
                أرجع NGX_ERROR;
            }

            wdc = (ngx_hash_wildcard_t *) h.hash;

            إذا (ناں[n].کلید.len == len) {
                wdc->قدر = ناں[n].قدر;
            }

            name->قدر = (فراغ *) ((uintptr_t) wdc | (dot ? 3 : 2));

        } وإلا إذا (dot) {
            name->قدر = (فراغ *) ((uintptr_t) name->قدر | 1);
        }
    }

    إذا (ngx_hash_init(hinit, (ngx_hash_کلید_t *) curr_ناں.elts,
                      curr_ناں.nelts)
        != NGX_OK)
    {
        أرجع NGX_ERROR;
    }

    أرجع NGX_OK;
}


ngx_uint_t
ngx_hash_کلید(u_char *انکڑے, ناپ_t len)
{
    ngx_uint_t  i, کلید;

    کلید = 0;

    لكل (i = 0; i < len; i++) {
        کلید = ngx_hash(کلید, انکڑے[i]);
    }

    أرجع کلید;
}


ngx_uint_t
ngx_hash_کلید_lc(u_char *انکڑے, ناپ_t len)
{
    ngx_uint_t  i, کلید;

    کلید = 0;

    لكل (i = 0; i < len; i++) {
        کلید = ngx_hash(کلید, ngx_tolower(انکڑے[i]));
    }

    أرجع کلید;
}


ngx_uint_t
ngx_hash_strlow(u_char *dst, u_char *src, ناپ_t n)
{
    ngx_uint_t  کلید;

    کلید = 0;

    طالما (n--) {
        *dst = ngx_tolower(*src);
        کلید = ngx_hash(کلید, *dst);
        dst++;
        src++;
    }

    أرجع کلید;
}


ngx_int_t
ngx_hash_کلیدیں_array_init(ngx_hash_کلیدیں_arrays_t *ha, ngx_uint_t type)
{
    ngx_uint_t  asize;

    إذا (type == NGX_HASH_SMALL) {
        asize = 4;
        ha->hsize = 107;

    } وإلا {
        asize = NGX_HASH_LARGE_ASIZE;
        ha->hsize = NGX_HASH_LARGE_HSIZE;
    }

    إذا (ngx_array_init(&ha->کلیدیں, ha->عارضی_pool, asize, حجم(ngx_hash_کلید_t))
        != NGX_OK)
    {
        أرجع NGX_ERROR;
    }

    إذا (ngx_array_init(&ha->dns_wc_سر, ha->عارضی_pool, asize,
                       حجم(ngx_hash_کلید_t))
        != NGX_OK)
    {
        أرجع NGX_ERROR;
    }

    إذا (ngx_array_init(&ha->dns_wc_پوچھ, ha->عارضی_pool, asize,
                       حجم(ngx_hash_کلید_t))
        != NGX_OK)
    {
        أرجع NGX_ERROR;
    }

    ha->کلیدیں_hash = ngx_pcalloc(ha->عارضی_pool, حجم(ngx_array_t) * ha->hsize);
    إذا (ha->کلیدیں_hash == عدم) {
        أرجع NGX_ERROR;
    }

    ha->dns_wc_سر_hash = ngx_pcalloc(ha->عارضی_pool,
                                       حجم(ngx_array_t) * ha->hsize);
    إذا (ha->dns_wc_سر_hash == عدم) {
        أرجع NGX_ERROR;
    }

    ha->dns_wc_پوچھ_hash = ngx_pcalloc(ha->عارضی_pool,
                                       حجم(ngx_array_t) * ha->hsize);
    إذا (ha->dns_wc_پوچھ_hash == عدم) {
        أرجع NGX_ERROR;
    }

    أرجع NGX_OK;
}


ngx_int_t
ngx_hash_جوڑو_کلید(ngx_hash_کلیدیں_arrays_t *ha, ngx_str_t *کلید, فراغ *قدر,
    ngx_uint_t flags)
{
    ناپ_t           len;
    u_char          *p;
    ngx_str_t       *name;
    ngx_uint_t       i, k, n, skip, آخری;
    ngx_array_t     *کلیدیں, *hwc;
    ngx_hash_کلید_t  *hk;

    آخری = کلید->len;

    إذا (flags & NGX_HASH_WILDCARD_کلید) {

        /*
         * supported wildcards:
         *     "*.example.com", ".example.com", and "www.example.*"
         */

        n = 0;

        لكل (i = 0; i < کلید->len; i++) {

            إذا (کلید->انکڑے[i] == '*') {
                إذا (++n > 1) {
                    أرجع NGX_DECLINED;
                }
            }

            إذا (کلید->انکڑے[i] == '.' && کلید->انکڑے[i + 1] == '.') {
                أرجع NGX_DECLINED;
            }

            إذا (کلید->انکڑے[i] == '\0') {
                أرجع NGX_DECLINED;
            }
        }

        إذا (کلید->len > 1 && کلید->انکڑے[0] == '.') {
            skip = 1;
            اذهب wildcard;
        }

        إذا (کلید->len > 2) {

            إذا (کلید->انکڑے[0] == '*' && کلید->انکڑے[1] == '.') {
                skip = 2;
                اذهب wildcard;
            }

            إذا (کلید->انکڑے[i - 2] == '.' && کلید->انکڑے[i - 1] == '*') {
                skip = 0;
                آخری -= 2;
                اذهب wildcard;
            }
        }

        إذا (n) {
            أرجع NGX_DECLINED;
        }
    }

    /* exact hash */

    k = 0;

    لكل (i = 0; i < آخری; i++) {
        إذا (!(flags & NGX_HASH_READONLY_کلید)) {
            کلید->انکڑے[i] = ngx_tolower(کلید->انکڑے[i]);
        }
        k = ngx_hash(k, کلید->انکڑے[i]);
    }

    k %= ha->hsize;

    /* check conflicts in exact hash */

    name = ha->کلیدیں_hash[k].elts;

    إذا (name) {
        لكل (i = 0; i < ha->کلیدیں_hash[k].nelts; i++) {
            إذا (آخری != name[i].len) {
                استمر;
            }

            إذا (ngx_strncmp(کلید->انکڑے, name[i].انکڑے, آخری) == 0) {
                أرجع NGX_BUSY;
            }
        }

    } وإلا {
        إذا (ngx_array_init(&ha->کلیدیں_hash[k], ha->عارضی_pool, 4,
                           حجم(ngx_str_t))
            != NGX_OK)
        {
            أرجع NGX_ERROR;
        }
    }

    name = ngx_array_ڈالو(&ha->کلیدیں_hash[k]);
    إذا (name == عدم) {
        أرجع NGX_ERROR;
    }

    *name = *کلید;

    hk = ngx_array_ڈالو(&ha->کلیدیں);
    إذا (hk == عدم) {
        أرجع NGX_ERROR;
    }

    hk->کلید = *کلید;
    hk->کلید_hash = ngx_hash_کلید(کلید->انکڑے, آخری);
    hk->قدر = قدر;

    أرجع NGX_OK;


wildcard:

    /* wildcard hash */

    k = ngx_hash_strlow(&کلید->انکڑے[skip], &کلید->انکڑے[skip], آخری - skip);

    k %= ha->hsize;

    إذا (skip == 1) {

        /* check conflicts in exact hash for ".example.com" */

        name = ha->کلیدیں_hash[k].elts;

        إذا (name) {
            len = آخری - skip;

            لكل (i = 0; i < ha->کلیدیں_hash[k].nelts; i++) {
                إذا (len != name[i].len) {
                    استمر;
                }

                إذا (ngx_strncmp(&کلید->انکڑے[1], name[i].انکڑے, len) == 0) {
                    أرجع NGX_BUSY;
                }
            }

        } وإلا {
            إذا (ngx_array_init(&ha->کلیدیں_hash[k], ha->عارضی_pool, 4,
                               حجم(ngx_str_t))
                != NGX_OK)
            {
                أرجع NGX_ERROR;
            }
        }

        name = ngx_array_ڈالو(&ha->کلیدیں_hash[k]);
        إذا (name == عدم) {
            أرجع NGX_ERROR;
        }

        name->len = آخری - 1;
        name->انکڑے = ngx_pnalloc(ha->عارضی_pool, name->len);
        إذا (name->انکڑے == عدم) {
            أرجع NGX_ERROR;
        }

        ngx_memcpy(name->انکڑے, &کلید->انکڑے[1], name->len);
    }


    إذا (skip) {

        /*
         * convert "*.example.com" to "com.example.\0"
         *      and ".example.com" to "com.example\0"
         */

        p = ngx_pnalloc(ha->عارضی_pool, آخری);
        إذا (p == عدم) {
            أرجع NGX_ERROR;
        }

        len = 0;
        n = 0;

        لكل (i = آخری - 1; i; i--) {
            إذا (کلید->انکڑے[i] == '.') {
                ngx_memcpy(&p[n], &کلید->انکڑے[i + 1], len);
                n += len;
                p[n++] = '.';
                len = 0;
                استمر;
            }

            len++;
        }

        إذا (len) {
            ngx_memcpy(&p[n], &کلید->انکڑے[1], len);
            n += len;
        }

        p[n] = '\0';

        hwc = &ha->dns_wc_سر;
        کلیدیں = &ha->dns_wc_سر_hash[k];

    } وإلا {

        /* convert "www.example.*" to "www.example\0" */

        آخری++;

        p = ngx_pnalloc(ha->عارضی_pool, آخری);
        إذا (p == عدم) {
            أرجع NGX_ERROR;
        }

        ngx_cpystrn(p, کلید->انکڑے, آخری);

        hwc = &ha->dns_wc_پوچھ;
        کلیدیں = &ha->dns_wc_پوچھ_hash[k];
    }


    /* check conflicts in wildcard hash */

    name = کلیدیں->elts;

    إذا (name) {
        len = آخری - skip;

        لكل (i = 0; i < کلیدیں->nelts; i++) {
            إذا (len != name[i].len) {
                استمر;
            }

            إذا (ngx_strncmp(کلید->انکڑے + skip, name[i].انکڑے, len) == 0) {
                أرجع NGX_BUSY;
            }
        }

    } وإلا {
        إذا (ngx_array_init(کلیدیں, ha->عارضی_pool, 4, حجم(ngx_str_t)) != NGX_OK)
        {
            أرجع NGX_ERROR;
        }
    }

    name = ngx_array_ڈالو(کلیدیں);
    إذا (name == عدم) {
        أرجع NGX_ERROR;
    }

    name->len = آخری - skip;
    name->انکڑے = ngx_pnalloc(ha->عارضی_pool, name->len);
    إذا (name->انکڑے == عدم) {
        أرجع NGX_ERROR;
    }

    ngx_memcpy(name->انکڑے, کلید->انکڑے + skip, name->len);


    /* add to wildcard hash */

    hk = ngx_array_ڈالو(hwc);
    إذا (hk == عدم) {
        أرجع NGX_ERROR;
    }

    hk->کلید.len = آخری - 1;
    hk->کلید.انکڑے = p;
    hk->کلید_hash = 0;
    hk->قدر = قدر;

    أرجع NGX_OK;
}
