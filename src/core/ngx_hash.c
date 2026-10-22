
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


فراغ *
ngx_hash_پیدا_کړه(ngx_hash_t *hash, ngx_uint_t کلی, u_char *name, اندازه_t len)
{
    ngx_uint_t       i;
    ngx_hash_elt_t  *elt;

#إذا 0
    ngx_log_error(NGX_LOG_ALERT, ngx_دوره->لوګاریتم, 0, "hf:\"%*s\"", len, name);
#د_که_پای

    elt = hash->buckets[کلی % hash->اندازه];

    إذا (elt == عدم) {
        أرجع عدم;
    }

    طالما (elt->ارزښت) {
        إذا (len != (اندازه_t) elt->len) {
            اذهب بل;
        }

        لكل (i = 0; i < len; i++) {
            إذا (name[i] != elt->name[i]) {
                اذهب بل;
            }
        }

        أرجع elt->ارزښت;

    بل:

        elt = (ngx_hash_elt_t *) ngx_align_ptr(&elt->name[0] + elt->len,
                                               حجم(فراغ *));
        استمر;
    }

    أرجع عدم;
}


فراغ *
ngx_hash_پیدا_کړه_wc_سر(ngx_hash_wildcard_t *hwc, u_char *name, اندازه_t len)
{
    فراغ        *ارزښت;
    ngx_uint_t   i, n, کلی;

#إذا 0
    ngx_log_error(NGX_LOG_ALERT, ngx_دوره->لوګاریتم, 0, "wch:\"%*s\"", len, name);
#د_که_پای

    n = len;

    طالما (n) {
        إذا (name[n - 1] == '.') {
            اكسر;
        }

        n--;
    }

    کلی = 0;

    لكل (i = n; i < len; i++) {
        کلی = ngx_hash(کلی, name[i]);
    }

#إذا 0
    ngx_log_error(NGX_LOG_ALERT, ngx_دوره->لوګاریتم, 0, "key:\"%ui\"", کلی);
#د_که_پای

    ارزښت = ngx_hash_پیدا_کړه(&hwc->hash, کلی, &name[n], len - n);

#إذا 0
    ngx_log_error(NGX_LOG_ALERT, ngx_دوره->لوګاریتم, 0, "value:\"%p\"", ارزښت);
#د_که_پای

    إذا (ارزښت) {

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

        إذا ((uintptr_t) ارزښت & 2) {

            إذا (n == 0) {

                /* "example.com" */

                إذا ((uintptr_t) ارزښت & 1) {
                    أرجع عدم;
                }

                hwc = (ngx_hash_wildcard_t *)
                                          ((uintptr_t) ارزښت & (uintptr_t) ~3);
                أرجع hwc->ارزښت;
            }

            hwc = (ngx_hash_wildcard_t *) ((uintptr_t) ارزښت & (uintptr_t) ~3);

            ارزښت = ngx_hash_پیدا_کړه_wc_سر(hwc, name, n - 1);

            إذا (ارزښت) {
                أرجع ارزښت;
            }

            أرجع hwc->ارزښت;
        }

        إذا ((uintptr_t) ارزښت & 1) {

            إذا (n == 0) {

                /* "example.com" */

                أرجع عدم;
            }

            أرجع (فراغ *) ((uintptr_t) ارزښت & (uintptr_t) ~3);
        }

        أرجع ارزښت;
    }

    أرجع hwc->ارزښت;
}


فراغ *
ngx_hash_پیدا_کړه_wc_لکۍ(ngx_hash_wildcard_t *hwc, u_char *name, اندازه_t len)
{
    فراغ        *ارزښت;
    ngx_uint_t   i, کلی;

#إذا 0
    ngx_log_error(NGX_LOG_ALERT, ngx_دوره->لوګاریتم, 0, "wct:\"%*s\"", len, name);
#د_که_پای

    کلی = 0;

    لكل (i = 0; i < len; i++) {
        إذا (name[i] == '.') {
            اكسر;
        }

        کلی = ngx_hash(کلی, name[i]);
    }

    إذا (i == len) {
        أرجع عدم;
    }

#إذا 0
    ngx_log_error(NGX_LOG_ALERT, ngx_دوره->لوګاریتم, 0, "key:\"%ui\"", کلی);
#د_که_پای

    ارزښت = ngx_hash_پیدا_کړه(&hwc->hash, کلی, name, i);

#إذا 0
    ngx_log_error(NGX_LOG_ALERT, ngx_دوره->لوګاریتم, 0, "value:\"%p\"", ارزښت);
#د_که_پای

    إذا (ارزښت) {

        /*
         * the 2 low bits of value have the special meaning:
         *     00 - value is data pointer;
         *     11 - value is pointer to wildcard hash allowing "example.*".
         */

        إذا ((uintptr_t) ارزښت & 2) {

            i++;

            hwc = (ngx_hash_wildcard_t *) ((uintptr_t) ارزښت & (uintptr_t) ~3);

            ارزښت = ngx_hash_پیدا_کړه_wc_لکۍ(hwc, &name[i], len - i);

            إذا (ارزښت) {
                أرجع ارزښت;
            }

            أرجع hwc->ارزښت;
        }

        أرجع ارزښت;
    }

    أرجع hwc->ارزښت;
}


فراغ *
ngx_hash_پیدا_کړه_combined(ngx_hash_combined_t *hash, ngx_uint_t کلی, u_char *name,
    اندازه_t len)
{
    فراغ  *ارزښت;

    إذا (hash->hash.buckets) {
        ارزښت = ngx_hash_پیدا_کړه(&hash->hash, کلی, name, len);

        إذا (ارزښت) {
            أرجع ارزښت;
        }
    }

    إذا (len == 0) {
        أرجع عدم;
    }

    إذا (hash->wc_سر && hash->wc_سر->hash.buckets) {
        ارزښت = ngx_hash_پیدا_کړه_wc_سر(hash->wc_سر, name, len);

        إذا (ارزښت) {
            أرجع ارزښت;
        }
    }

    إذا (hash->wc_لکۍ && hash->wc_لکۍ->hash.buckets) {
        ارزښت = ngx_hash_پیدا_کړه_wc_لکۍ(hash->wc_لکۍ, name, len);

        إذا (ارزښت) {
            أرجع ارزښت;
        }
    }

    أرجع عدم;
}


#تعریف_کړه NGX_HASH_ELT_اندازه(name)                                               \
    (حجم(فراغ *) + ngx_align((name)->کلی.len + 2, حجم(فراغ *)))

ngx_int_t
ngx_hash_init(ngx_hash_init_t *hinit, ngx_hash_کلی_t *نومونه, ngx_uint_t nelts)
{
    u_char          *elts;
    اندازه_t           len;
    u_short         *test;
    ngx_uint_t       i, n, کلی, اندازه, پیل, لوښی_اندازه;
    ngx_hash_elt_t  *elt, **buckets;

    إذا (hinit->max_اندازه == 0) {
        ngx_log_error(NGX_LOG_EMERG, hinit->pool->لوګاریتم, 0,
                      "could not build %s, you should "
                      "increase %s_max_size: %i",
                      hinit->name, hinit->name, hinit->max_اندازه);
        أرجع NGX_ERROR;
    }

    إذا (hinit->لوښی_اندازه > 65536 - ngx_cacheline_اندازه) {
        ngx_log_error(NGX_LOG_EMERG, hinit->pool->لوګاریتم, 0,
                      "could not build %s, too large "
                      "%s_bucket_size: %i",
                      hinit->name, hinit->name, hinit->لوښی_اندازه);
        أرجع NGX_ERROR;
    }

    لكل (n = 0; n < nelts; n++) {
        إذا (نومونه[n].کلی.معلومات == عدم) {
            استمر;
        }

        إذا (hinit->لوښی_اندازه < NGX_HASH_ELT_اندازه(&نومونه[n]) + حجم(فراغ *))
        {
            ngx_log_error(NGX_LOG_EMERG, hinit->pool->لوګاریتم, 0,
                          "could not build %s, you should "
                          "increase %s_bucket_size: %i",
                          hinit->name, hinit->name, hinit->لوښی_اندازه);
            أرجع NGX_ERROR;
        }
    }

    test = ngx_alloc(hinit->max_اندازه * حجم(u_short), hinit->pool->لوګاریتم);
    إذا (test == عدم) {
        أرجع NGX_ERROR;
    }

    لوښی_اندازه = hinit->لوښی_اندازه - حجم(فراغ *);

    پیل = nelts / (لوښی_اندازه / (2 * حجم(فراغ *)));
    پیل = پیل ? پیل : 1;

    إذا (hinit->max_اندازه > 10000 && nelts && hinit->max_اندازه / nelts < 100) {
        پیل = hinit->max_اندازه - 1000;
    }

    لكل (اندازه = پیل; اندازه <= hinit->max_اندازه; اندازه++) {

        ngx_memzero(test, اندازه * حجم(u_short));

        لكل (n = 0; n < nelts; n++) {
            إذا (نومونه[n].کلی.معلومات == عدم) {
                استمر;
            }

            کلی = نومونه[n].کلی_hash % اندازه;
            len = test[کلی] + NGX_HASH_ELT_اندازه(&نومونه[n]);

#إذا 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->لوګاریتم, 0,
                          "%ui: %ui %uz \"%V\"",
                          اندازه, کلی, len, &نومونه[n].کلی);
#د_که_پای

            إذا (len > لوښی_اندازه) {
                اذهب بل;
            }

            test[کلی] = (u_short) len;
        }

        اذهب وموندل;

    بل:

        استمر;
    }

    اندازه = hinit->max_اندازه;

    ngx_log_error(NGX_LOG_WARN, hinit->pool->لوګاریتم, 0,
                  "could not build optimal %s, you should increase "
                  "either %s_max_size: %i or %s_bucket_size: %i; "
                  "ignoring %s_bucket_size",
                  hinit->name, hinit->name, hinit->max_اندازه,
                  hinit->name, hinit->لوښی_اندازه, hinit->name);

وموندل:

    لكل (i = 0; i < اندازه; i++) {
        test[i] = حجم(فراغ *);
    }

    لكل (n = 0; n < nelts; n++) {
        إذا (نومونه[n].کلی.معلومات == عدم) {
            استمر;
        }

        کلی = نومونه[n].کلی_hash % اندازه;
        len = test[کلی] + NGX_HASH_ELT_اندازه(&نومونه[n]);

        إذا (len > 65536 - ngx_cacheline_اندازه) {
            ngx_log_error(NGX_LOG_EMERG, hinit->pool->لوګاریتم, 0,
                          "could not build %s, you should "
                          "increase %s_max_size: %i",
                          hinit->name, hinit->name, hinit->max_اندازه);
            ngx_free(test);
            أرجع NGX_ERROR;
        }

        test[کلی] = (u_short) len;
    }

    len = 0;

    لكل (i = 0; i < اندازه; i++) {
        إذا (test[i] == حجم(فراغ *)) {
            استمر;
        }

        test[i] = (u_short) (ngx_align(test[i], ngx_cacheline_اندازه));

        len += test[i];
    }

    إذا (hinit->hash == عدم) {
        hinit->hash = ngx_pcalloc(hinit->pool, حجم(ngx_hash_wildcard_t)
                                             + اندازه * حجم(ngx_hash_elt_t *));
        إذا (hinit->hash == عدم) {
            ngx_free(test);
            أرجع NGX_ERROR;
        }

        buckets = (ngx_hash_elt_t **)
                      ((u_char *) hinit->hash + حجم(ngx_hash_wildcard_t));

    } وإلا {
        buckets = ngx_pcalloc(hinit->pool, اندازه * حجم(ngx_hash_elt_t *));
        إذا (buckets == عدم) {
            ngx_free(test);
            أرجع NGX_ERROR;
        }
    }

    elts = ngx_palloc(hinit->pool, len + ngx_cacheline_اندازه);
    إذا (elts == عدم) {
        ngx_free(test);
        أرجع NGX_ERROR;
    }

    elts = ngx_align_ptr(elts, ngx_cacheline_اندازه);

    لكل (i = 0; i < اندازه; i++) {
        إذا (test[i] == حجم(فراغ *)) {
            استمر;
        }

        buckets[i] = (ngx_hash_elt_t *) elts;
        elts += test[i];
    }

    لكل (i = 0; i < اندازه; i++) {
        test[i] = 0;
    }

    لكل (n = 0; n < nelts; n++) {
        إذا (نومونه[n].کلی.معلومات == عدم) {
            استمر;
        }

        کلی = نومونه[n].کلی_hash % اندازه;
        elt = (ngx_hash_elt_t *) ((u_char *) buckets[کلی] + test[کلی]);

        elt->ارزښت = نومونه[n].ارزښت;
        elt->len = (u_short) نومونه[n].کلی.len;

        ngx_strlow(elt->name, نومونه[n].کلی.معلومات, نومونه[n].کلی.len);

        test[کلی] = (u_short) (test[کلی] + NGX_HASH_ELT_اندازه(&نومونه[n]));
    }

    لكل (i = 0; i < اندازه; i++) {
        إذا (buckets[i] == عدم) {
            استمر;
        }

        elt = (ngx_hash_elt_t *) ((u_char *) buckets[i] + test[i]);

        elt->ارزښت = عدم;
    }

    ngx_free(test);

    hinit->hash->buckets = buckets;
    hinit->hash->اندازه = اندازه;

#إذا 0

    لكل (i = 0; i < اندازه; i++) {
        ngx_str_t   val;
        ngx_uint_t  کلی;

        elt = buckets[i];

        إذا (elt == عدم) {
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->لوګاریتم, 0,
                          "%ui: NULL", i);
            استمر;
        }

        طالما (elt->ارزښت) {
            val.len = elt->len;
            val.معلومات = &elt->name[0];

            کلی = hinit->کلی(val.معلومات, val.len);

            ngx_log_error(NGX_LOG_ALERT, hinit->pool->لوګاریتم, 0,
                          "%ui: %p \"%V\" %ui", i, elt, &val, کلی);

            elt = (ngx_hash_elt_t *) ngx_align_ptr(&elt->name[0] + elt->len,
                                                   حجم(فراغ *));
        }
    }

#د_که_پای

    أرجع NGX_OK;
}


ngx_int_t
ngx_hash_wildcard_init(ngx_hash_init_t *hinit, ngx_hash_کلی_t *نومونه,
    ngx_uint_t nelts)
{
    اندازه_t                len, dot_len;
    ngx_uint_t            i, n, dot;
    ngx_array_t           curr_نومونه, بل_نومونه;
    ngx_hash_کلی_t       *name, *بل_name;
    ngx_hash_init_t       h;
    ngx_hash_wildcard_t  *wdc;

    إذا (ngx_array_init(&curr_نومونه, hinit->لنډمهاله_pool, nelts,
                       حجم(ngx_hash_کلی_t))
        != NGX_OK)
    {
        أرجع NGX_ERROR;
    }

    إذا (ngx_array_init(&بل_نومونه, hinit->لنډمهاله_pool, nelts,
                       حجم(ngx_hash_کلی_t))
        != NGX_OK)
    {
        أرجع NGX_ERROR;
    }

    لكل (n = 0; n < nelts; n = i) {

#إذا 0
        ngx_log_error(NGX_LOG_ALERT, hinit->pool->لوګاریتم, 0,
                      "wc0: \"%V\"", &نومونه[n].کلی);
#د_که_پای

        dot = 0;

        لكل (len = 0; len < نومونه[n].کلی.len; len++) {
            إذا (نومونه[n].کلی.معلومات[len] == '.') {
                dot = 1;
                اكسر;
            }
        }

        name = ngx_array_ټېل(&curr_نومونه);
        إذا (name == عدم) {
            أرجع NGX_ERROR;
        }

        name->کلی.len = len;
        name->کلی.معلومات = نومونه[n].کلی.معلومات;
        name->کلی_hash = hinit->کلی(name->کلی.معلومات, name->کلی.len);
        name->ارزښت = نومونه[n].ارزښت;

#إذا 0
        ngx_log_error(NGX_LOG_ALERT, hinit->pool->لوګاریتم, 0,
                      "wc1: \"%V\" %ui", &name->کلی, dot);
#د_که_پای

        dot_len = len + 1;

        إذا (dot) {
            len++;
        }

        بل_نومونه.nelts = 0;

        إذا (نومونه[n].کلی.len != len) {
            بل_name = ngx_array_ټېل(&بل_نومونه);
            إذا (بل_name == عدم) {
                أرجع NGX_ERROR;
            }

            بل_name->کلی.len = نومونه[n].کلی.len - len;
            بل_name->کلی.معلومات = نومونه[n].کلی.معلومات + len;
            بل_name->کلی_hash = 0;
            بل_name->ارزښت = نومونه[n].ارزښت;

#إذا 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->لوګاریتم, 0,
                          "wc2: \"%V\"", &بل_name->کلی);
#د_که_پای
        }

        لكل (i = n + 1; i < nelts; i++) {
            إذا (ngx_strncmp(نومونه[n].کلی.معلومات, نومونه[i].کلی.معلومات, len) != 0) {
                اكسر;
            }

            إذا (!dot
                && نومونه[i].کلی.len > len
                && نومونه[i].کلی.معلومات[len] != '.')
            {
                اكسر;
            }

            بل_name = ngx_array_ټېل(&بل_نومونه);
            إذا (بل_name == عدم) {
                أرجع NGX_ERROR;
            }

            بل_name->کلی.len = نومونه[i].کلی.len - dot_len;
            بل_name->کلی.معلومات = نومونه[i].کلی.معلومات + dot_len;
            بل_name->کلی_hash = 0;
            بل_name->ارزښت = نومونه[i].ارزښت;

#إذا 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->لوګاریتم, 0,
                          "wc3: \"%V\"", &بل_name->کلی);
#د_که_پای
        }

        إذا (بل_نومونه.nelts) {

            h = *hinit;
            h.hash = عدم;

            إذا (ngx_hash_wildcard_init(&h, (ngx_hash_کلی_t *) بل_نومونه.elts,
                                       بل_نومونه.nelts)
                != NGX_OK)
            {
                أرجع NGX_ERROR;
            }

            wdc = (ngx_hash_wildcard_t *) h.hash;

            إذا (نومونه[n].کلی.len == len) {
                wdc->ارزښت = نومونه[n].ارزښت;
            }

            name->ارزښت = (فراغ *) ((uintptr_t) wdc | (dot ? 3 : 2));

        } وإلا إذا (dot) {
            name->ارزښت = (فراغ *) ((uintptr_t) name->ارزښت | 1);
        }
    }

    إذا (ngx_hash_init(hinit, (ngx_hash_کلی_t *) curr_نومونه.elts,
                      curr_نومونه.nelts)
        != NGX_OK)
    {
        أرجع NGX_ERROR;
    }

    أرجع NGX_OK;
}


ngx_uint_t
ngx_hash_کلی(u_char *معلومات, اندازه_t len)
{
    ngx_uint_t  i, کلی;

    کلی = 0;

    لكل (i = 0; i < len; i++) {
        کلی = ngx_hash(کلی, معلومات[i]);
    }

    أرجع کلی;
}


ngx_uint_t
ngx_hash_کلی_lc(u_char *معلومات, اندازه_t len)
{
    ngx_uint_t  i, کلی;

    کلی = 0;

    لكل (i = 0; i < len; i++) {
        کلی = ngx_hash(کلی, ngx_tolower(معلومات[i]));
    }

    أرجع کلی;
}


ngx_uint_t
ngx_hash_strlow(u_char *dst, u_char *src, اندازه_t n)
{
    ngx_uint_t  کلی;

    کلی = 0;

    طالما (n--) {
        *dst = ngx_tolower(*src);
        کلی = ngx_hash(کلی, *dst);
        dst++;
        src++;
    }

    أرجع کلی;
}


ngx_int_t
ngx_hash_کلیګانې_array_init(ngx_hash_کلیګانې_arrays_t *ha, ngx_uint_t type)
{
    ngx_uint_t  asize;

    إذا (type == NGX_HASH_SMALL) {
        asize = 4;
        ha->hsize = 107;

    } وإلا {
        asize = NGX_HASH_LARGE_ASIZE;
        ha->hsize = NGX_HASH_LARGE_HSIZE;
    }

    إذا (ngx_array_init(&ha->کلیګانې, ha->لنډمهاله_pool, asize, حجم(ngx_hash_کلی_t))
        != NGX_OK)
    {
        أرجع NGX_ERROR;
    }

    إذا (ngx_array_init(&ha->dns_wc_سر, ha->لنډمهاله_pool, asize,
                       حجم(ngx_hash_کلی_t))
        != NGX_OK)
    {
        أرجع NGX_ERROR;
    }

    إذا (ngx_array_init(&ha->dns_wc_لکۍ, ha->لنډمهاله_pool, asize,
                       حجم(ngx_hash_کلی_t))
        != NGX_OK)
    {
        أرجع NGX_ERROR;
    }

    ha->کلیګانې_hash = ngx_pcalloc(ha->لنډمهاله_pool, حجم(ngx_array_t) * ha->hsize);
    إذا (ha->کلیګانې_hash == عدم) {
        أرجع NGX_ERROR;
    }

    ha->dns_wc_سر_hash = ngx_pcalloc(ha->لنډمهاله_pool,
                                       حجم(ngx_array_t) * ha->hsize);
    إذا (ha->dns_wc_سر_hash == عدم) {
        أرجع NGX_ERROR;
    }

    ha->dns_wc_لکۍ_hash = ngx_pcalloc(ha->لنډمهاله_pool,
                                       حجم(ngx_array_t) * ha->hsize);
    إذا (ha->dns_wc_لکۍ_hash == عدم) {
        أرجع NGX_ERROR;
    }

    أرجع NGX_OK;
}


ngx_int_t
ngx_hash_جمع_کړه_کلی(ngx_hash_کلیګانې_arrays_t *ha, ngx_str_t *کلی, فراغ *ارزښت,
    ngx_uint_t flags)
{
    اندازه_t           len;
    u_char          *p;
    ngx_str_t       *name;
    ngx_uint_t       i, k, n, skip, وروستی;
    ngx_array_t     *کلیګانې, *hwc;
    ngx_hash_کلی_t  *hk;

    وروستی = کلی->len;

    إذا (flags & NGX_HASH_WILDCARD_کلی) {

        /*
         * supported wildcards:
         *     "*.example.com", ".example.com", and "www.example.*"
         */

        n = 0;

        لكل (i = 0; i < کلی->len; i++) {

            إذا (کلی->معلومات[i] == '*') {
                إذا (++n > 1) {
                    أرجع NGX_DECLINED;
                }
            }

            إذا (کلی->معلومات[i] == '.' && کلی->معلومات[i + 1] == '.') {
                أرجع NGX_DECLINED;
            }

            إذا (کلی->معلومات[i] == '\0') {
                أرجع NGX_DECLINED;
            }
        }

        إذا (کلی->len > 1 && کلی->معلومات[0] == '.') {
            skip = 1;
            اذهب wildcard;
        }

        إذا (کلی->len > 2) {

            إذا (کلی->معلومات[0] == '*' && کلی->معلومات[1] == '.') {
                skip = 2;
                اذهب wildcard;
            }

            إذا (کلی->معلومات[i - 2] == '.' && کلی->معلومات[i - 1] == '*') {
                skip = 0;
                وروستی -= 2;
                اذهب wildcard;
            }
        }

        إذا (n) {
            أرجع NGX_DECLINED;
        }
    }

    /* exact hash */

    k = 0;

    لكل (i = 0; i < وروستی; i++) {
        إذا (!(flags & NGX_HASH_READONLY_کلی)) {
            کلی->معلومات[i] = ngx_tolower(کلی->معلومات[i]);
        }
        k = ngx_hash(k, کلی->معلومات[i]);
    }

    k %= ha->hsize;

    /* check conflicts in exact hash */

    name = ha->کلیګانې_hash[k].elts;

    إذا (name) {
        لكل (i = 0; i < ha->کلیګانې_hash[k].nelts; i++) {
            إذا (وروستی != name[i].len) {
                استمر;
            }

            إذا (ngx_strncmp(کلی->معلومات, name[i].معلومات, وروستی) == 0) {
                أرجع NGX_BUSY;
            }
        }

    } وإلا {
        إذا (ngx_array_init(&ha->کلیګانې_hash[k], ha->لنډمهاله_pool, 4,
                           حجم(ngx_str_t))
            != NGX_OK)
        {
            أرجع NGX_ERROR;
        }
    }

    name = ngx_array_ټېل(&ha->کلیګانې_hash[k]);
    إذا (name == عدم) {
        أرجع NGX_ERROR;
    }

    *name = *کلی;

    hk = ngx_array_ټېل(&ha->کلیګانې);
    إذا (hk == عدم) {
        أرجع NGX_ERROR;
    }

    hk->کلی = *کلی;
    hk->کلی_hash = ngx_hash_کلی(کلی->معلومات, وروستی);
    hk->ارزښت = ارزښت;

    أرجع NGX_OK;


wildcard:

    /* wildcard hash */

    k = ngx_hash_strlow(&کلی->معلومات[skip], &کلی->معلومات[skip], وروستی - skip);

    k %= ha->hsize;

    إذا (skip == 1) {

        /* check conflicts in exact hash for ".example.com" */

        name = ha->کلیګانې_hash[k].elts;

        إذا (name) {
            len = وروستی - skip;

            لكل (i = 0; i < ha->کلیګانې_hash[k].nelts; i++) {
                إذا (len != name[i].len) {
                    استمر;
                }

                إذا (ngx_strncmp(&کلی->معلومات[1], name[i].معلومات, len) == 0) {
                    أرجع NGX_BUSY;
                }
            }

        } وإلا {
            إذا (ngx_array_init(&ha->کلیګانې_hash[k], ha->لنډمهاله_pool, 4,
                               حجم(ngx_str_t))
                != NGX_OK)
            {
                أرجع NGX_ERROR;
            }
        }

        name = ngx_array_ټېل(&ha->کلیګانې_hash[k]);
        إذا (name == عدم) {
            أرجع NGX_ERROR;
        }

        name->len = وروستی - 1;
        name->معلومات = ngx_pnalloc(ha->لنډمهاله_pool, name->len);
        إذا (name->معلومات == عدم) {
            أرجع NGX_ERROR;
        }

        ngx_memcpy(name->معلومات, &کلی->معلومات[1], name->len);
    }


    إذا (skip) {

        /*
         * convert "*.example.com" to "com.example.\0"
         *      and ".example.com" to "com.example\0"
         */

        p = ngx_pnalloc(ha->لنډمهاله_pool, وروستی);
        إذا (p == عدم) {
            أرجع NGX_ERROR;
        }

        len = 0;
        n = 0;

        لكل (i = وروستی - 1; i; i--) {
            إذا (کلی->معلومات[i] == '.') {
                ngx_memcpy(&p[n], &کلی->معلومات[i + 1], len);
                n += len;
                p[n++] = '.';
                len = 0;
                استمر;
            }

            len++;
        }

        إذا (len) {
            ngx_memcpy(&p[n], &کلی->معلومات[1], len);
            n += len;
        }

        p[n] = '\0';

        hwc = &ha->dns_wc_سر;
        کلیګانې = &ha->dns_wc_سر_hash[k];

    } وإلا {

        /* convert "www.example.*" to "www.example\0" */

        وروستی++;

        p = ngx_pnalloc(ha->لنډمهاله_pool, وروستی);
        إذا (p == عدم) {
            أرجع NGX_ERROR;
        }

        ngx_cpystrn(p, کلی->معلومات, وروستی);

        hwc = &ha->dns_wc_لکۍ;
        کلیګانې = &ha->dns_wc_لکۍ_hash[k];
    }


    /* check conflicts in wildcard hash */

    name = کلیګانې->elts;

    إذا (name) {
        len = وروستی - skip;

        لكل (i = 0; i < کلیګانې->nelts; i++) {
            إذا (len != name[i].len) {
                استمر;
            }

            إذا (ngx_strncmp(کلی->معلومات + skip, name[i].معلومات, len) == 0) {
                أرجع NGX_BUSY;
            }
        }

    } وإلا {
        إذا (ngx_array_init(کلیګانې, ha->لنډمهاله_pool, 4, حجم(ngx_str_t)) != NGX_OK)
        {
            أرجع NGX_ERROR;
        }
    }

    name = ngx_array_ټېل(کلیګانې);
    إذا (name == عدم) {
        أرجع NGX_ERROR;
    }

    name->len = وروستی - skip;
    name->معلومات = ngx_pnalloc(ha->لنډمهاله_pool, name->len);
    إذا (name->معلومات == عدم) {
        أرجع NGX_ERROR;
    }

    ngx_memcpy(name->معلومات, کلی->معلومات + skip, name->len);


    /* add to wildcard hash */

    hk = ngx_array_ټېل(hwc);
    إذا (hk == عدم) {
        أرجع NGX_ERROR;
    }

    hk->کلی.len = وروستی - 1;
    hk->کلی.معلومات = p;
    hk->کلی_hash = 0;
    hk->ارزښت = ارزښت;

    أرجع NGX_OK;
}
