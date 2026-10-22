
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


فراغ *
ngx_hash_پیدا_کن(ngx_hash_t *hash, ngx_uint_t کلید, u_char *name, اندازه_t len)
{
    ngx_uint_t       i;
    ngx_hash_elt_t  *elt;

#إذا 0
    ngx_log_error(NGX_LOG_ALERT, ngx_چرخه->لگاریتم, 0, "hf:\"%*s\"", len, name);
#پایان_اگر

    elt = hash->buckets[کلید % hash->اندازه];

    إذا (elt == عدم) {
        أرجع عدم;
    }

    طالما (elt->مقدار) {
        إذا (len != (اندازه_t) elt->len) {
            اذهب بعدی;
        }

        لكل (i = 0; i < len; i++) {
            إذا (name[i] != elt->name[i]) {
                اذهب بعدی;
            }
        }

        أرجع elt->مقدار;

    بعدی:

        elt = (ngx_hash_elt_t *) ngx_align_ptr(&elt->name[0] + elt->len,
                                               حجم(فراغ *));
        استمر;
    }

    أرجع عدم;
}


فراغ *
ngx_hash_پیدا_کن_wc_سر(ngx_hash_wildcard_t *hwc, u_char *name, اندازه_t len)
{
    فراغ        *مقدار;
    ngx_uint_t   i, n, کلید;

#إذا 0
    ngx_log_error(NGX_LOG_ALERT, ngx_چرخه->لگاریتم, 0, "wch:\"%*s\"", len, name);
#پایان_اگر

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
    ngx_log_error(NGX_LOG_ALERT, ngx_چرخه->لگاریتم, 0, "key:\"%ui\"", کلید);
#پایان_اگر

    مقدار = ngx_hash_پیدا_کن(&hwc->hash, کلید, &name[n], len - n);

#إذا 0
    ngx_log_error(NGX_LOG_ALERT, ngx_چرخه->لگاریتم, 0, "value:\"%p\"", مقدار);
#پایان_اگر

    إذا (مقدار) {

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

        إذا ((uintptr_t) مقدار & 2) {

            إذا (n == 0) {

                /* "example.com" */

                إذا ((uintptr_t) مقدار & 1) {
                    أرجع عدم;
                }

                hwc = (ngx_hash_wildcard_t *)
                                          ((uintptr_t) مقدار & (uintptr_t) ~3);
                أرجع hwc->مقدار;
            }

            hwc = (ngx_hash_wildcard_t *) ((uintptr_t) مقدار & (uintptr_t) ~3);

            مقدار = ngx_hash_پیدا_کن_wc_سر(hwc, name, n - 1);

            إذا (مقدار) {
                أرجع مقدار;
            }

            أرجع hwc->مقدار;
        }

        إذا ((uintptr_t) مقدار & 1) {

            إذا (n == 0) {

                /* "example.com" */

                أرجع عدم;
            }

            أرجع (فراغ *) ((uintptr_t) مقدار & (uintptr_t) ~3);
        }

        أرجع مقدار;
    }

    أرجع hwc->مقدار;
}


فراغ *
ngx_hash_پیدا_کن_wc_دم(ngx_hash_wildcard_t *hwc, u_char *name, اندازه_t len)
{
    فراغ        *مقدار;
    ngx_uint_t   i, کلید;

#إذا 0
    ngx_log_error(NGX_LOG_ALERT, ngx_چرخه->لگاریتم, 0, "wct:\"%*s\"", len, name);
#پایان_اگر

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
    ngx_log_error(NGX_LOG_ALERT, ngx_چرخه->لگاریتم, 0, "key:\"%ui\"", کلید);
#پایان_اگر

    مقدار = ngx_hash_پیدا_کن(&hwc->hash, کلید, name, i);

#إذا 0
    ngx_log_error(NGX_LOG_ALERT, ngx_چرخه->لگاریتم, 0, "value:\"%p\"", مقدار);
#پایان_اگر

    إذا (مقدار) {

        /*
         * the 2 low bits of value have the special meaning:
         *     00 - value is data pointer;
         *     11 - value is pointer to wildcard hash allowing "example.*".
         */

        إذا ((uintptr_t) مقدار & 2) {

            i++;

            hwc = (ngx_hash_wildcard_t *) ((uintptr_t) مقدار & (uintptr_t) ~3);

            مقدار = ngx_hash_پیدا_کن_wc_دم(hwc, &name[i], len - i);

            إذا (مقدار) {
                أرجع مقدار;
            }

            أرجع hwc->مقدار;
        }

        أرجع مقدار;
    }

    أرجع hwc->مقدار;
}


فراغ *
ngx_hash_پیدا_کن_combined(ngx_hash_combined_t *hash, ngx_uint_t کلید, u_char *name,
    اندازه_t len)
{
    فراغ  *مقدار;

    إذا (hash->hash.buckets) {
        مقدار = ngx_hash_پیدا_کن(&hash->hash, کلید, name, len);

        إذا (مقدار) {
            أرجع مقدار;
        }
    }

    إذا (len == 0) {
        أرجع عدم;
    }

    إذا (hash->wc_سر && hash->wc_سر->hash.buckets) {
        مقدار = ngx_hash_پیدا_کن_wc_سر(hash->wc_سر, name, len);

        إذا (مقدار) {
            أرجع مقدار;
        }
    }

    إذا (hash->wc_دم && hash->wc_دم->hash.buckets) {
        مقدار = ngx_hash_پیدا_کن_wc_دم(hash->wc_دم, name, len);

        إذا (مقدار) {
            أرجع مقدار;
        }
    }

    أرجع عدم;
}


#تعریف_کن NGX_HASH_ELT_اندازه(name)                                               \
    (حجم(فراغ *) + ngx_align((name)->کلید.len + 2, حجم(فراغ *)))

ngx_int_t
ngx_hash_init(ngx_hash_init_t *hinit, ngx_hash_کلید_t *نامها, ngx_uint_t nelts)
{
    u_char          *elts;
    اندازه_t           len;
    u_short         *test;
    ngx_uint_t       i, n, کلید, اندازه, شروع, سطل_اندازه;
    ngx_hash_elt_t  *elt, **buckets;

    إذا (hinit->max_اندازه == 0) {
        ngx_log_error(NGX_LOG_EMERG, hinit->pool->لگاریتم, 0,
                      "could not build %s, you should "
                      "increase %s_max_size: %i",
                      hinit->name, hinit->name, hinit->max_اندازه);
        أرجع NGX_ERROR;
    }

    إذا (hinit->سطل_اندازه > 65536 - ngx_cacheline_اندازه) {
        ngx_log_error(NGX_LOG_EMERG, hinit->pool->لگاریتم, 0,
                      "could not build %s, too large "
                      "%s_bucket_size: %i",
                      hinit->name, hinit->name, hinit->سطل_اندازه);
        أرجع NGX_ERROR;
    }

    لكل (n = 0; n < nelts; n++) {
        إذا (نامها[n].کلید.داده == عدم) {
            استمر;
        }

        إذا (hinit->سطل_اندازه < NGX_HASH_ELT_اندازه(&نامها[n]) + حجم(فراغ *))
        {
            ngx_log_error(NGX_LOG_EMERG, hinit->pool->لگاریتم, 0,
                          "could not build %s, you should "
                          "increase %s_bucket_size: %i",
                          hinit->name, hinit->name, hinit->سطل_اندازه);
            أرجع NGX_ERROR;
        }
    }

    test = ngx_alloc(hinit->max_اندازه * حجم(u_short), hinit->pool->لگاریتم);
    إذا (test == عدم) {
        أرجع NGX_ERROR;
    }

    سطل_اندازه = hinit->سطل_اندازه - حجم(فراغ *);

    شروع = nelts / (سطل_اندازه / (2 * حجم(فراغ *)));
    شروع = شروع ? شروع : 1;

    إذا (hinit->max_اندازه > 10000 && nelts && hinit->max_اندازه / nelts < 100) {
        شروع = hinit->max_اندازه - 1000;
    }

    لكل (اندازه = شروع; اندازه <= hinit->max_اندازه; اندازه++) {

        ngx_memzero(test, اندازه * حجم(u_short));

        لكل (n = 0; n < nelts; n++) {
            إذا (نامها[n].کلید.داده == عدم) {
                استمر;
            }

            کلید = نامها[n].کلید_hash % اندازه;
            len = test[کلید] + NGX_HASH_ELT_اندازه(&نامها[n]);

#إذا 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->لگاریتم, 0,
                          "%ui: %ui %uz \"%V\"",
                          اندازه, کلید, len, &نامها[n].کلید);
#پایان_اگر

            إذا (len > سطل_اندازه) {
                اذهب بعدی;
            }

            test[کلید] = (u_short) len;
        }

        اذهب یافت;

    بعدی:

        استمر;
    }

    اندازه = hinit->max_اندازه;

    ngx_log_error(NGX_LOG_WARN, hinit->pool->لگاریتم, 0,
                  "could not build optimal %s, you should increase "
                  "either %s_max_size: %i or %s_bucket_size: %i; "
                  "ignoring %s_bucket_size",
                  hinit->name, hinit->name, hinit->max_اندازه,
                  hinit->name, hinit->سطل_اندازه, hinit->name);

یافت:

    لكل (i = 0; i < اندازه; i++) {
        test[i] = حجم(فراغ *);
    }

    لكل (n = 0; n < nelts; n++) {
        إذا (نامها[n].کلید.داده == عدم) {
            استمر;
        }

        کلید = نامها[n].کلید_hash % اندازه;
        len = test[کلید] + NGX_HASH_ELT_اندازه(&نامها[n]);

        إذا (len > 65536 - ngx_cacheline_اندازه) {
            ngx_log_error(NGX_LOG_EMERG, hinit->pool->لگاریتم, 0,
                          "could not build %s, you should "
                          "increase %s_max_size: %i",
                          hinit->name, hinit->name, hinit->max_اندازه);
            ngx_free(test);
            أرجع NGX_ERROR;
        }

        test[کلید] = (u_short) len;
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
        إذا (نامها[n].کلید.داده == عدم) {
            استمر;
        }

        کلید = نامها[n].کلید_hash % اندازه;
        elt = (ngx_hash_elt_t *) ((u_char *) buckets[کلید] + test[کلید]);

        elt->مقدار = نامها[n].مقدار;
        elt->len = (u_short) نامها[n].کلید.len;

        ngx_strlow(elt->name, نامها[n].کلید.داده, نامها[n].کلید.len);

        test[کلید] = (u_short) (test[کلید] + NGX_HASH_ELT_اندازه(&نامها[n]));
    }

    لكل (i = 0; i < اندازه; i++) {
        إذا (buckets[i] == عدم) {
            استمر;
        }

        elt = (ngx_hash_elt_t *) ((u_char *) buckets[i] + test[i]);

        elt->مقدار = عدم;
    }

    ngx_free(test);

    hinit->hash->buckets = buckets;
    hinit->hash->اندازه = اندازه;

#إذا 0

    لكل (i = 0; i < اندازه; i++) {
        ngx_str_t   val;
        ngx_uint_t  کلید;

        elt = buckets[i];

        إذا (elt == عدم) {
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->لگاریتم, 0,
                          "%ui: NULL", i);
            استمر;
        }

        طالما (elt->مقدار) {
            val.len = elt->len;
            val.داده = &elt->name[0];

            کلید = hinit->کلید(val.داده, val.len);

            ngx_log_error(NGX_LOG_ALERT, hinit->pool->لگاریتم, 0,
                          "%ui: %p \"%V\" %ui", i, elt, &val, کلید);

            elt = (ngx_hash_elt_t *) ngx_align_ptr(&elt->name[0] + elt->len,
                                                   حجم(فراغ *));
        }
    }

#پایان_اگر

    أرجع NGX_OK;
}


ngx_int_t
ngx_hash_wildcard_init(ngx_hash_init_t *hinit, ngx_hash_کلید_t *نامها,
    ngx_uint_t nelts)
{
    اندازه_t                len, dot_len;
    ngx_uint_t            i, n, dot;
    ngx_array_t           curr_نامها, بعدی_نامها;
    ngx_hash_کلید_t       *name, *بعدی_name;
    ngx_hash_init_t       h;
    ngx_hash_wildcard_t  *wdc;

    إذا (ngx_array_init(&curr_نامها, hinit->موقت_pool, nelts,
                       حجم(ngx_hash_کلید_t))
        != NGX_OK)
    {
        أرجع NGX_ERROR;
    }

    إذا (ngx_array_init(&بعدی_نامها, hinit->موقت_pool, nelts,
                       حجم(ngx_hash_کلید_t))
        != NGX_OK)
    {
        أرجع NGX_ERROR;
    }

    لكل (n = 0; n < nelts; n = i) {

#إذا 0
        ngx_log_error(NGX_LOG_ALERT, hinit->pool->لگاریتم, 0,
                      "wc0: \"%V\"", &نامها[n].کلید);
#پایان_اگر

        dot = 0;

        لكل (len = 0; len < نامها[n].کلید.len; len++) {
            إذا (نامها[n].کلید.داده[len] == '.') {
                dot = 1;
                اكسر;
            }
        }

        name = ngx_array_افزودن(&curr_نامها);
        إذا (name == عدم) {
            أرجع NGX_ERROR;
        }

        name->کلید.len = len;
        name->کلید.داده = نامها[n].کلید.داده;
        name->کلید_hash = hinit->کلید(name->کلید.داده, name->کلید.len);
        name->مقدار = نامها[n].مقدار;

#إذا 0
        ngx_log_error(NGX_LOG_ALERT, hinit->pool->لگاریتم, 0,
                      "wc1: \"%V\" %ui", &name->کلید, dot);
#پایان_اگر

        dot_len = len + 1;

        إذا (dot) {
            len++;
        }

        بعدی_نامها.nelts = 0;

        إذا (نامها[n].کلید.len != len) {
            بعدی_name = ngx_array_افزودن(&بعدی_نامها);
            إذا (بعدی_name == عدم) {
                أرجع NGX_ERROR;
            }

            بعدی_name->کلید.len = نامها[n].کلید.len - len;
            بعدی_name->کلید.داده = نامها[n].کلید.داده + len;
            بعدی_name->کلید_hash = 0;
            بعدی_name->مقدار = نامها[n].مقدار;

#إذا 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->لگاریتم, 0,
                          "wc2: \"%V\"", &بعدی_name->کلید);
#پایان_اگر
        }

        لكل (i = n + 1; i < nelts; i++) {
            إذا (ngx_strncmp(نامها[n].کلید.داده, نامها[i].کلید.داده, len) != 0) {
                اكسر;
            }

            إذا (!dot
                && نامها[i].کلید.len > len
                && نامها[i].کلید.داده[len] != '.')
            {
                اكسر;
            }

            بعدی_name = ngx_array_افزودن(&بعدی_نامها);
            إذا (بعدی_name == عدم) {
                أرجع NGX_ERROR;
            }

            بعدی_name->کلید.len = نامها[i].کلید.len - dot_len;
            بعدی_name->کلید.داده = نامها[i].کلید.داده + dot_len;
            بعدی_name->کلید_hash = 0;
            بعدی_name->مقدار = نامها[i].مقدار;

#إذا 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->لگاریتم, 0,
                          "wc3: \"%V\"", &بعدی_name->کلید);
#پایان_اگر
        }

        إذا (بعدی_نامها.nelts) {

            h = *hinit;
            h.hash = عدم;

            إذا (ngx_hash_wildcard_init(&h, (ngx_hash_کلید_t *) بعدی_نامها.elts,
                                       بعدی_نامها.nelts)
                != NGX_OK)
            {
                أرجع NGX_ERROR;
            }

            wdc = (ngx_hash_wildcard_t *) h.hash;

            إذا (نامها[n].کلید.len == len) {
                wdc->مقدار = نامها[n].مقدار;
            }

            name->مقدار = (فراغ *) ((uintptr_t) wdc | (dot ? 3 : 2));

        } وإلا إذا (dot) {
            name->مقدار = (فراغ *) ((uintptr_t) name->مقدار | 1);
        }
    }

    إذا (ngx_hash_init(hinit, (ngx_hash_کلید_t *) curr_نامها.elts,
                      curr_نامها.nelts)
        != NGX_OK)
    {
        أرجع NGX_ERROR;
    }

    أرجع NGX_OK;
}


ngx_uint_t
ngx_hash_کلید(u_char *داده, اندازه_t len)
{
    ngx_uint_t  i, کلید;

    کلید = 0;

    لكل (i = 0; i < len; i++) {
        کلید = ngx_hash(کلید, داده[i]);
    }

    أرجع کلید;
}


ngx_uint_t
ngx_hash_کلید_lc(u_char *داده, اندازه_t len)
{
    ngx_uint_t  i, کلید;

    کلید = 0;

    لكل (i = 0; i < len; i++) {
        کلید = ngx_hash(کلید, ngx_tolower(داده[i]));
    }

    أرجع کلید;
}


ngx_uint_t
ngx_hash_strlow(u_char *dst, u_char *src, اندازه_t n)
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
ngx_hash_کلیدها_array_init(ngx_hash_کلیدها_arrays_t *ha, ngx_uint_t type)
{
    ngx_uint_t  asize;

    إذا (type == NGX_HASH_SMALL) {
        asize = 4;
        ha->hsize = 107;

    } وإلا {
        asize = NGX_HASH_LARGE_ASIZE;
        ha->hsize = NGX_HASH_LARGE_HSIZE;
    }

    إذا (ngx_array_init(&ha->کلیدها, ha->موقت_pool, asize, حجم(ngx_hash_کلید_t))
        != NGX_OK)
    {
        أرجع NGX_ERROR;
    }

    إذا (ngx_array_init(&ha->dns_wc_سر, ha->موقت_pool, asize,
                       حجم(ngx_hash_کلید_t))
        != NGX_OK)
    {
        أرجع NGX_ERROR;
    }

    إذا (ngx_array_init(&ha->dns_wc_دم, ha->موقت_pool, asize,
                       حجم(ngx_hash_کلید_t))
        != NGX_OK)
    {
        أرجع NGX_ERROR;
    }

    ha->کلیدها_hash = ngx_pcalloc(ha->موقت_pool, حجم(ngx_array_t) * ha->hsize);
    إذا (ha->کلیدها_hash == عدم) {
        أرجع NGX_ERROR;
    }

    ha->dns_wc_سر_hash = ngx_pcalloc(ha->موقت_pool,
                                       حجم(ngx_array_t) * ha->hsize);
    إذا (ha->dns_wc_سر_hash == عدم) {
        أرجع NGX_ERROR;
    }

    ha->dns_wc_دم_hash = ngx_pcalloc(ha->موقت_pool,
                                       حجم(ngx_array_t) * ha->hsize);
    إذا (ha->dns_wc_دم_hash == عدم) {
        أرجع NGX_ERROR;
    }

    أرجع NGX_OK;
}


ngx_int_t
ngx_hash_جمع_کن_کلید(ngx_hash_کلیدها_arrays_t *ha, ngx_str_t *کلید, فراغ *مقدار,
    ngx_uint_t flags)
{
    اندازه_t           len;
    u_char          *p;
    ngx_str_t       *name;
    ngx_uint_t       i, k, n, skip, آخرین;
    ngx_array_t     *کلیدها, *hwc;
    ngx_hash_کلید_t  *hk;

    آخرین = کلید->len;

    إذا (flags & NGX_HASH_WILDCARD_کلید) {

        /*
         * supported wildcards:
         *     "*.example.com", ".example.com", and "www.example.*"
         */

        n = 0;

        لكل (i = 0; i < کلید->len; i++) {

            إذا (کلید->داده[i] == '*') {
                إذا (++n > 1) {
                    أرجع NGX_DECLINED;
                }
            }

            إذا (کلید->داده[i] == '.' && کلید->داده[i + 1] == '.') {
                أرجع NGX_DECLINED;
            }

            إذا (کلید->داده[i] == '\0') {
                أرجع NGX_DECLINED;
            }
        }

        إذا (کلید->len > 1 && کلید->داده[0] == '.') {
            skip = 1;
            اذهب wildcard;
        }

        إذا (کلید->len > 2) {

            إذا (کلید->داده[0] == '*' && کلید->داده[1] == '.') {
                skip = 2;
                اذهب wildcard;
            }

            إذا (کلید->داده[i - 2] == '.' && کلید->داده[i - 1] == '*') {
                skip = 0;
                آخرین -= 2;
                اذهب wildcard;
            }
        }

        إذا (n) {
            أرجع NGX_DECLINED;
        }
    }

    /* exact hash */

    k = 0;

    لكل (i = 0; i < آخرین; i++) {
        إذا (!(flags & NGX_HASH_READONLY_کلید)) {
            کلید->داده[i] = ngx_tolower(کلید->داده[i]);
        }
        k = ngx_hash(k, کلید->داده[i]);
    }

    k %= ha->hsize;

    /* check conflicts in exact hash */

    name = ha->کلیدها_hash[k].elts;

    إذا (name) {
        لكل (i = 0; i < ha->کلیدها_hash[k].nelts; i++) {
            إذا (آخرین != name[i].len) {
                استمر;
            }

            إذا (ngx_strncmp(کلید->داده, name[i].داده, آخرین) == 0) {
                أرجع NGX_BUSY;
            }
        }

    } وإلا {
        إذا (ngx_array_init(&ha->کلیدها_hash[k], ha->موقت_pool, 4,
                           حجم(ngx_str_t))
            != NGX_OK)
        {
            أرجع NGX_ERROR;
        }
    }

    name = ngx_array_افزودن(&ha->کلیدها_hash[k]);
    إذا (name == عدم) {
        أرجع NGX_ERROR;
    }

    *name = *کلید;

    hk = ngx_array_افزودن(&ha->کلیدها);
    إذا (hk == عدم) {
        أرجع NGX_ERROR;
    }

    hk->کلید = *کلید;
    hk->کلید_hash = ngx_hash_کلید(کلید->داده, آخرین);
    hk->مقدار = مقدار;

    أرجع NGX_OK;


wildcard:

    /* wildcard hash */

    k = ngx_hash_strlow(&کلید->داده[skip], &کلید->داده[skip], آخرین - skip);

    k %= ha->hsize;

    إذا (skip == 1) {

        /* check conflicts in exact hash for ".example.com" */

        name = ha->کلیدها_hash[k].elts;

        إذا (name) {
            len = آخرین - skip;

            لكل (i = 0; i < ha->کلیدها_hash[k].nelts; i++) {
                إذا (len != name[i].len) {
                    استمر;
                }

                إذا (ngx_strncmp(&کلید->داده[1], name[i].داده, len) == 0) {
                    أرجع NGX_BUSY;
                }
            }

        } وإلا {
            إذا (ngx_array_init(&ha->کلیدها_hash[k], ha->موقت_pool, 4,
                               حجم(ngx_str_t))
                != NGX_OK)
            {
                أرجع NGX_ERROR;
            }
        }

        name = ngx_array_افزودن(&ha->کلیدها_hash[k]);
        إذا (name == عدم) {
            أرجع NGX_ERROR;
        }

        name->len = آخرین - 1;
        name->داده = ngx_pnalloc(ha->موقت_pool, name->len);
        إذا (name->داده == عدم) {
            أرجع NGX_ERROR;
        }

        ngx_memcpy(name->داده, &کلید->داده[1], name->len);
    }


    إذا (skip) {

        /*
         * convert "*.example.com" to "com.example.\0"
         *      and ".example.com" to "com.example\0"
         */

        p = ngx_pnalloc(ha->موقت_pool, آخرین);
        إذا (p == عدم) {
            أرجع NGX_ERROR;
        }

        len = 0;
        n = 0;

        لكل (i = آخرین - 1; i; i--) {
            إذا (کلید->داده[i] == '.') {
                ngx_memcpy(&p[n], &کلید->داده[i + 1], len);
                n += len;
                p[n++] = '.';
                len = 0;
                استمر;
            }

            len++;
        }

        إذا (len) {
            ngx_memcpy(&p[n], &کلید->داده[1], len);
            n += len;
        }

        p[n] = '\0';

        hwc = &ha->dns_wc_سر;
        کلیدها = &ha->dns_wc_سر_hash[k];

    } وإلا {

        /* convert "www.example.*" to "www.example\0" */

        آخرین++;

        p = ngx_pnalloc(ha->موقت_pool, آخرین);
        إذا (p == عدم) {
            أرجع NGX_ERROR;
        }

        ngx_cpystrn(p, کلید->داده, آخرین);

        hwc = &ha->dns_wc_دم;
        کلیدها = &ha->dns_wc_دم_hash[k];
    }


    /* check conflicts in wildcard hash */

    name = کلیدها->elts;

    إذا (name) {
        len = آخرین - skip;

        لكل (i = 0; i < کلیدها->nelts; i++) {
            إذا (len != name[i].len) {
                استمر;
            }

            إذا (ngx_strncmp(کلید->داده + skip, name[i].داده, len) == 0) {
                أرجع NGX_BUSY;
            }
        }

    } وإلا {
        إذا (ngx_array_init(کلیدها, ha->موقت_pool, 4, حجم(ngx_str_t)) != NGX_OK)
        {
            أرجع NGX_ERROR;
        }
    }

    name = ngx_array_افزودن(کلیدها);
    إذا (name == عدم) {
        أرجع NGX_ERROR;
    }

    name->len = آخرین - skip;
    name->داده = ngx_pnalloc(ha->موقت_pool, name->len);
    إذا (name->داده == عدم) {
        أرجع NGX_ERROR;
    }

    ngx_memcpy(name->داده, کلید->داده + skip, name->len);


    /* add to wildcard hash */

    hk = ngx_array_افزودن(hwc);
    إذا (hk == عدم) {
        أرجع NGX_ERROR;
    }

    hk->کلید.len = آخرین - 1;
    hk->کلید.داده = p;
    hk->کلید_hash = 0;
    hk->مقدار = مقدار;

    أرجع NGX_OK;
}
