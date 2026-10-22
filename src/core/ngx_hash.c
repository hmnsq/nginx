
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>


శూన్య *
ngx_hash_కనుగొను(ngx_hash_t *hash, ngx_uint_t కీలకం, u_char *name, కొలత_t len)
{
    ngx_uint_t       i;
    ngx_hash_elt_t  *elt;

#ఒకవేళ 0
    ngx_log_error(NGX_LOG_ALERT, ngx_చక్రం->సంవర్గమానం, 0, "hf:\"%*s\"", len, name);
#అయితే_ముగింపు

    elt = hash->buckets[కీలకం % hash->కొలత];

    ఒకవేళ (elt == ఖాళీ) {
        తిరిగి ఖాళీ;
    }

    అయితే (elt->విలువ) {
        ఒకవేళ (len != (కొలత_t) elt->len) {
            వెళ్ళు తదుపరి;
        }

        కొరకు (i = 0; i < len; i++) {
            ఒకవేళ (name[i] != elt->name[i]) {
                వెళ్ళు తదుపరి;
            }
        }

        తిరిగి elt->విలువ;

    తదుపరి:

        elt = (ngx_hash_elt_t *) ngx_align_ptr(&elt->name[0] + elt->len,
                                               పరిమాణం(శూన్య *));
        కొనసాగు;
    }

    తిరిగి ఖాళీ;
}


శూన్య *
ngx_hash_కనుగొను_wc_తల(ngx_hash_wildcard_t *hwc, u_char *name, కొలత_t len)
{
    శూన్య        *విలువ;
    ngx_uint_t   i, n, కీలకం;

#ఒకవేళ 0
    ngx_log_error(NGX_LOG_ALERT, ngx_చక్రం->సంవర్గమానం, 0, "wch:\"%*s\"", len, name);
#అయితే_ముగింపు

    n = len;

    అయితే (n) {
        ఒకవేళ (name[n - 1] == '.') {
            ఆపు;
        }

        n--;
    }

    కీలకం = 0;

    కొరకు (i = n; i < len; i++) {
        కీలకం = ngx_hash(కీలకం, name[i]);
    }

#ఒకవేళ 0
    ngx_log_error(NGX_LOG_ALERT, ngx_చక్రం->సంవర్గమానం, 0, "key:\"%ui\"", కీలకం);
#అయితే_ముగింపు

    విలువ = ngx_hash_కనుగొను(&hwc->hash, కీలకం, &name[n], len - n);

#ఒకవేళ 0
    ngx_log_error(NGX_LOG_ALERT, ngx_చక్రం->సంవర్గమానం, 0, "value:\"%p\"", విలువ);
#అయితే_ముగింపు

    ఒకవేళ (విలువ) {

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

        ఒకవేళ ((uintptr_t) విలువ & 2) {

            ఒకవేళ (n == 0) {

                /* "example.com" */

                ఒకవేళ ((uintptr_t) విలువ & 1) {
                    తిరిగి ఖాళీ;
                }

                hwc = (ngx_hash_wildcard_t *)
                                          ((uintptr_t) విలువ & (uintptr_t) ~3);
                తిరిగి hwc->విలువ;
            }

            hwc = (ngx_hash_wildcard_t *) ((uintptr_t) విలువ & (uintptr_t) ~3);

            విలువ = ngx_hash_కనుగొను_wc_తల(hwc, name, n - 1);

            ఒకవేళ (విలువ) {
                తిరిగి విలువ;
            }

            తిరిగి hwc->విలువ;
        }

        ఒకవేళ ((uintptr_t) విలువ & 1) {

            ఒకవేళ (n == 0) {

                /* "example.com" */

                తిరిగి ఖాళీ;
            }

            తిరిగి (శూన్య *) ((uintptr_t) విలువ & (uintptr_t) ~3);
        }

        తిరిగి విలువ;
    }

    తిరిగి hwc->విలువ;
}


శూన్య *
ngx_hash_కనుగొను_wc_తోక(ngx_hash_wildcard_t *hwc, u_char *name, కొలత_t len)
{
    శూన్య        *విలువ;
    ngx_uint_t   i, కీలకం;

#ఒకవేళ 0
    ngx_log_error(NGX_LOG_ALERT, ngx_చక్రం->సంవర్గమానం, 0, "wct:\"%*s\"", len, name);
#అయితే_ముగింపు

    కీలకం = 0;

    కొరకు (i = 0; i < len; i++) {
        ఒకవేళ (name[i] == '.') {
            ఆపు;
        }

        కీలకం = ngx_hash(కీలకం, name[i]);
    }

    ఒకవేళ (i == len) {
        తిరిగి ఖాళీ;
    }

#ఒకవేళ 0
    ngx_log_error(NGX_LOG_ALERT, ngx_చక్రం->సంవర్గమానం, 0, "key:\"%ui\"", కీలకం);
#అయితే_ముగింపు

    విలువ = ngx_hash_కనుగొను(&hwc->hash, కీలకం, name, i);

#ఒకవేళ 0
    ngx_log_error(NGX_LOG_ALERT, ngx_చక్రం->సంవర్గమానం, 0, "value:\"%p\"", విలువ);
#అయితే_ముగింపు

    ఒకవేళ (విలువ) {

        /*
         * the 2 low bits of value have the special meaning:
         *     00 - value is data pointer;
         *     11 - value is pointer to wildcard hash allowing "example.*".
         */

        ఒకవేళ ((uintptr_t) విలువ & 2) {

            i++;

            hwc = (ngx_hash_wildcard_t *) ((uintptr_t) విలువ & (uintptr_t) ~3);

            విలువ = ngx_hash_కనుగొను_wc_తోక(hwc, &name[i], len - i);

            ఒకవేళ (విలువ) {
                తిరిగి విలువ;
            }

            తిరిగి hwc->విలువ;
        }

        తిరిగి విలువ;
    }

    తిరిగి hwc->విలువ;
}


శూన్య *
ngx_hash_కనుగొను_combined(ngx_hash_combined_t *hash, ngx_uint_t కీలకం, u_char *name,
    కొలత_t len)
{
    శూన్య  *విలువ;

    ఒకవేళ (hash->hash.buckets) {
        విలువ = ngx_hash_కనుగొను(&hash->hash, కీలకం, name, len);

        ఒకవేళ (విలువ) {
            తిరిగి విలువ;
        }
    }

    ఒకవేళ (len == 0) {
        తిరిగి ఖాళీ;
    }

    ఒకవేళ (hash->wc_తల && hash->wc_తల->hash.buckets) {
        విలువ = ngx_hash_కనుగొను_wc_తల(hash->wc_తల, name, len);

        ఒకవేళ (విలువ) {
            తిరిగి విలువ;
        }
    }

    ఒకవేళ (hash->wc_తోక && hash->wc_తోక->hash.buckets) {
        విలువ = ngx_hash_కనుగొను_wc_తోక(hash->wc_తోక, name, len);

        ఒకవేళ (విలువ) {
            తిరిగి విలువ;
        }
    }

    తిరిగి ఖాళీ;
}


#నిర్వచించు NGX_HASH_ELT_కొలత(name)                                               \
    (పరిమాణం(శూన్య *) + ngx_align((name)->కీలకం.len + 2, పరిమాణం(శూన్య *)))

ngx_int_t
ngx_hash_init(ngx_hash_init_t *hinit, ngx_hash_కీలకం_t *పేర్లు, ngx_uint_t nelts)
{
    u_char          *elts;
    కొలత_t           len;
    u_short         *test;
    ngx_uint_t       i, n, కీలకం, కొలత, ప్రారంభం, పాత్ర_కొలత;
    ngx_hash_elt_t  *elt, **buckets;

    ఒకవేళ (hinit->max_కొలత == 0) {
        ngx_log_error(NGX_LOG_EMERG, hinit->pool->సంవర్గమానం, 0,
                      "could not build %s, you should "
                      "increase %s_max_size: %i",
                      hinit->name, hinit->name, hinit->max_కొలత);
        తిరిగి NGX_ERROR;
    }

    ఒకవేళ (hinit->పాత్ర_కొలత > 65536 - ngx_cacheline_కొలత) {
        ngx_log_error(NGX_LOG_EMERG, hinit->pool->సంవర్గమానం, 0,
                      "could not build %s, too large "
                      "%s_bucket_size: %i",
                      hinit->name, hinit->name, hinit->పాత్ర_కొలత);
        తిరిగి NGX_ERROR;
    }

    కొరకు (n = 0; n < nelts; n++) {
        ఒకవేళ (పేర్లు[n].కీలకం.దత్తాంశం == ఖాళీ) {
            కొనసాగు;
        }

        ఒకవేళ (hinit->పాత్ర_కొలత < NGX_HASH_ELT_కొలత(&పేర్లు[n]) + పరిమాణం(శూన్య *))
        {
            ngx_log_error(NGX_LOG_EMERG, hinit->pool->సంవర్గమానం, 0,
                          "could not build %s, you should "
                          "increase %s_bucket_size: %i",
                          hinit->name, hinit->name, hinit->పాత్ర_కొలత);
            తిరిగి NGX_ERROR;
        }
    }

    test = ngx_alloc(hinit->max_కొలత * పరిమాణం(u_short), hinit->pool->సంవర్గమానం);
    ఒకవేళ (test == ఖాళీ) {
        తిరిగి NGX_ERROR;
    }

    పాత్ర_కొలత = hinit->పాత్ర_కొలత - పరిమాణం(శూన్య *);

    ప్రారంభం = nelts / (పాత్ర_కొలత / (2 * పరిమాణం(శూన్య *)));
    ప్రారంభం = ప్రారంభం ? ప్రారంభం : 1;

    ఒకవేళ (hinit->max_కొలత > 10000 && nelts && hinit->max_కొలత / nelts < 100) {
        ప్రారంభం = hinit->max_కొలత - 1000;
    }

    కొరకు (కొలత = ప్రారంభం; కొలత <= hinit->max_కొలత; కొలత++) {

        ngx_memzero(test, కొలత * పరిమాణం(u_short));

        కొరకు (n = 0; n < nelts; n++) {
            ఒకవేళ (పేర్లు[n].కీలకం.దత్తాంశం == ఖాళీ) {
                కొనసాగు;
            }

            కీలకం = పేర్లు[n].కీలకం_hash % కొలత;
            len = test[కీలకం] + NGX_HASH_ELT_కొలత(&పేర్లు[n]);

#ఒకవేళ 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->సంవర్గమానం, 0,
                          "%ui: %ui %uz \"%V\"",
                          కొలత, కీలకం, len, &పేర్లు[n].కీలకం);
#అయితే_ముగింపు

            ఒకవేళ (len > పాత్ర_కొలత) {
                వెళ్ళు తదుపరి;
            }

            test[కీలకం] = (u_short) len;
        }

        వెళ్ళు దొరికింది;

    తదుపరి:

        కొనసాగు;
    }

    కొలత = hinit->max_కొలత;

    ngx_log_error(NGX_LOG_WARN, hinit->pool->సంవర్గమానం, 0,
                  "could not build optimal %s, you should increase "
                  "either %s_max_size: %i or %s_bucket_size: %i; "
                  "ignoring %s_bucket_size",
                  hinit->name, hinit->name, hinit->max_కొలత,
                  hinit->name, hinit->పాత్ర_కొలత, hinit->name);

దొరికింది:

    కొరకు (i = 0; i < కొలత; i++) {
        test[i] = పరిమాణం(శూన్య *);
    }

    కొరకు (n = 0; n < nelts; n++) {
        ఒకవేళ (పేర్లు[n].కీలకం.దత్తాంశం == ఖాళీ) {
            కొనసాగు;
        }

        కీలకం = పేర్లు[n].కీలకం_hash % కొలత;
        len = test[కీలకం] + NGX_HASH_ELT_కొలత(&పేర్లు[n]);

        ఒకవేళ (len > 65536 - ngx_cacheline_కొలత) {
            ngx_log_error(NGX_LOG_EMERG, hinit->pool->సంవర్గమానం, 0,
                          "could not build %s, you should "
                          "increase %s_max_size: %i",
                          hinit->name, hinit->name, hinit->max_కొలత);
            ngx_free(test);
            తిరిగి NGX_ERROR;
        }

        test[కీలకం] = (u_short) len;
    }

    len = 0;

    కొరకు (i = 0; i < కొలత; i++) {
        ఒకవేళ (test[i] == పరిమాణం(శూన్య *)) {
            కొనసాగు;
        }

        test[i] = (u_short) (ngx_align(test[i], ngx_cacheline_కొలత));

        len += test[i];
    }

    ఒకవేళ (hinit->hash == ఖాళీ) {
        hinit->hash = ngx_pcalloc(hinit->pool, పరిమాణం(ngx_hash_wildcard_t)
                                             + కొలత * పరిమాణం(ngx_hash_elt_t *));
        ఒకవేళ (hinit->hash == ఖాళీ) {
            ngx_free(test);
            తిరిగి NGX_ERROR;
        }

        buckets = (ngx_hash_elt_t **)
                      ((u_char *) hinit->hash + పరిమాణం(ngx_hash_wildcard_t));

    } లేకపోతే {
        buckets = ngx_pcalloc(hinit->pool, కొలత * పరిమాణం(ngx_hash_elt_t *));
        ఒకవేళ (buckets == ఖాళీ) {
            ngx_free(test);
            తిరిగి NGX_ERROR;
        }
    }

    elts = ngx_palloc(hinit->pool, len + ngx_cacheline_కొలత);
    ఒకవేళ (elts == ఖాళీ) {
        ngx_free(test);
        తిరిగి NGX_ERROR;
    }

    elts = ngx_align_ptr(elts, ngx_cacheline_కొలత);

    కొరకు (i = 0; i < కొలత; i++) {
        ఒకవేళ (test[i] == పరిమాణం(శూన్య *)) {
            కొనసాగు;
        }

        buckets[i] = (ngx_hash_elt_t *) elts;
        elts += test[i];
    }

    కొరకు (i = 0; i < కొలత; i++) {
        test[i] = 0;
    }

    కొరకు (n = 0; n < nelts; n++) {
        ఒకవేళ (పేర్లు[n].కీలకం.దత్తాంశం == ఖాళీ) {
            కొనసాగు;
        }

        కీలకం = పేర్లు[n].కీలకం_hash % కొలత;
        elt = (ngx_hash_elt_t *) ((u_char *) buckets[కీలకం] + test[కీలకం]);

        elt->విలువ = పేర్లు[n].విలువ;
        elt->len = (u_short) పేర్లు[n].కీలకం.len;

        ngx_strlow(elt->name, పేర్లు[n].కీలకం.దత్తాంశం, పేర్లు[n].కీలకం.len);

        test[కీలకం] = (u_short) (test[కీలకం] + NGX_HASH_ELT_కొలత(&పేర్లు[n]));
    }

    కొరకు (i = 0; i < కొలత; i++) {
        ఒకవేళ (buckets[i] == ఖాళీ) {
            కొనసాగు;
        }

        elt = (ngx_hash_elt_t *) ((u_char *) buckets[i] + test[i]);

        elt->విలువ = ఖాళీ;
    }

    ngx_free(test);

    hinit->hash->buckets = buckets;
    hinit->hash->కొలత = కొలత;

#ఒకవేళ 0

    కొరకు (i = 0; i < కొలత; i++) {
        ngx_str_t   val;
        ngx_uint_t  కీలకం;

        elt = buckets[i];

        ఒకవేళ (elt == ఖాళీ) {
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->సంవర్గమానం, 0,
                          "%ui: NULL", i);
            కొనసాగు;
        }

        అయితే (elt->విలువ) {
            val.len = elt->len;
            val.దత్తాంశం = &elt->name[0];

            కీలకం = hinit->కీలకం(val.దత్తాంశం, val.len);

            ngx_log_error(NGX_LOG_ALERT, hinit->pool->సంవర్గమానం, 0,
                          "%ui: %p \"%V\" %ui", i, elt, &val, కీలకం);

            elt = (ngx_hash_elt_t *) ngx_align_ptr(&elt->name[0] + elt->len,
                                                   పరిమాణం(శూన్య *));
        }
    }

#అయితే_ముగింపు

    తిరిగి NGX_OK;
}


ngx_int_t
ngx_hash_wildcard_init(ngx_hash_init_t *hinit, ngx_hash_కీలకం_t *పేర్లు,
    ngx_uint_t nelts)
{
    కొలత_t                len, dot_len;
    ngx_uint_t            i, n, dot;
    ngx_array_t           curr_పేర్లు, తదుపరి_పేర్లు;
    ngx_hash_కీలకం_t       *name, *తదుపరి_name;
    ngx_hash_init_t       h;
    ngx_hash_wildcard_t  *wdc;

    ఒకవేళ (ngx_array_init(&curr_పేర్లు, hinit->తాత్కాలిక_pool, nelts,
                       పరిమాణం(ngx_hash_కీలకం_t))
        != NGX_OK)
    {
        తిరిగి NGX_ERROR;
    }

    ఒకవేళ (ngx_array_init(&తదుపరి_పేర్లు, hinit->తాత్కాలిక_pool, nelts,
                       పరిమాణం(ngx_hash_కీలకం_t))
        != NGX_OK)
    {
        తిరిగి NGX_ERROR;
    }

    కొరకు (n = 0; n < nelts; n = i) {

#ఒకవేళ 0
        ngx_log_error(NGX_LOG_ALERT, hinit->pool->సంవర్గమానం, 0,
                      "wc0: \"%V\"", &పేర్లు[n].కీలకం);
#అయితే_ముగింపు

        dot = 0;

        కొరకు (len = 0; len < పేర్లు[n].కీలకం.len; len++) {
            ఒకవేళ (పేర్లు[n].కీలకం.దత్తాంశం[len] == '.') {
                dot = 1;
                ఆపు;
            }
        }

        name = ngx_array_నెట్టు(&curr_పేర్లు);
        ఒకవేళ (name == ఖాళీ) {
            తిరిగి NGX_ERROR;
        }

        name->కీలకం.len = len;
        name->కీలకం.దత్తాంశం = పేర్లు[n].కీలకం.దత్తాంశం;
        name->కీలకం_hash = hinit->కీలకం(name->కీలకం.దత్తాంశం, name->కీలకం.len);
        name->విలువ = పేర్లు[n].విలువ;

#ఒకవేళ 0
        ngx_log_error(NGX_LOG_ALERT, hinit->pool->సంవర్గమానం, 0,
                      "wc1: \"%V\" %ui", &name->కీలకం, dot);
#అయితే_ముగింపు

        dot_len = len + 1;

        ఒకవేళ (dot) {
            len++;
        }

        తదుపరి_పేర్లు.nelts = 0;

        ఒకవేళ (పేర్లు[n].కీలకం.len != len) {
            తదుపరి_name = ngx_array_నెట్టు(&తదుపరి_పేర్లు);
            ఒకవేళ (తదుపరి_name == ఖాళీ) {
                తిరిగి NGX_ERROR;
            }

            తదుపరి_name->కీలకం.len = పేర్లు[n].కీలకం.len - len;
            తదుపరి_name->కీలకం.దత్తాంశం = పేర్లు[n].కీలకం.దత్తాంశం + len;
            తదుపరి_name->కీలకం_hash = 0;
            తదుపరి_name->విలువ = పేర్లు[n].విలువ;

#ఒకవేళ 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->సంవర్గమానం, 0,
                          "wc2: \"%V\"", &తదుపరి_name->కీలకం);
#అయితే_ముగింపు
        }

        కొరకు (i = n + 1; i < nelts; i++) {
            ఒకవేళ (ngx_strncmp(పేర్లు[n].కీలకం.దత్తాంశం, పేర్లు[i].కీలకం.దత్తాంశం, len) != 0) {
                ఆపు;
            }

            ఒకవేళ (!dot
                && పేర్లు[i].కీలకం.len > len
                && పేర్లు[i].కీలకం.దత్తాంశం[len] != '.')
            {
                ఆపు;
            }

            తదుపరి_name = ngx_array_నెట్టు(&తదుపరి_పేర్లు);
            ఒకవేళ (తదుపరి_name == ఖాళీ) {
                తిరిగి NGX_ERROR;
            }

            తదుపరి_name->కీలకం.len = పేర్లు[i].కీలకం.len - dot_len;
            తదుపరి_name->కీలకం.దత్తాంశం = పేర్లు[i].కీలకం.దత్తాంశం + dot_len;
            తదుపరి_name->కీలకం_hash = 0;
            తదుపరి_name->విలువ = పేర్లు[i].విలువ;

#ఒకవేళ 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->సంవర్గమానం, 0,
                          "wc3: \"%V\"", &తదుపరి_name->కీలకం);
#అయితే_ముగింపు
        }

        ఒకవేళ (తదుపరి_పేర్లు.nelts) {

            h = *hinit;
            h.hash = ఖాళీ;

            ఒకవేళ (ngx_hash_wildcard_init(&h, (ngx_hash_కీలకం_t *) తదుపరి_పేర్లు.elts,
                                       తదుపరి_పేర్లు.nelts)
                != NGX_OK)
            {
                తిరిగి NGX_ERROR;
            }

            wdc = (ngx_hash_wildcard_t *) h.hash;

            ఒకవేళ (పేర్లు[n].కీలకం.len == len) {
                wdc->విలువ = పేర్లు[n].విలువ;
            }

            name->విలువ = (శూన్య *) ((uintptr_t) wdc | (dot ? 3 : 2));

        } లేకపోతే ఒకవేళ (dot) {
            name->విలువ = (శూన్య *) ((uintptr_t) name->విలువ | 1);
        }
    }

    ఒకవేళ (ngx_hash_init(hinit, (ngx_hash_కీలకం_t *) curr_పేర్లు.elts,
                      curr_పేర్లు.nelts)
        != NGX_OK)
    {
        తిరిగి NGX_ERROR;
    }

    తిరిగి NGX_OK;
}


ngx_uint_t
ngx_hash_కీలకం(u_char *దత్తాంశం, కొలత_t len)
{
    ngx_uint_t  i, కీలకం;

    కీలకం = 0;

    కొరకు (i = 0; i < len; i++) {
        కీలకం = ngx_hash(కీలకం, దత్తాంశం[i]);
    }

    తిరిగి కీలకం;
}


ngx_uint_t
ngx_hash_కీలకం_lc(u_char *దత్తాంశం, కొలత_t len)
{
    ngx_uint_t  i, కీలకం;

    కీలకం = 0;

    కొరకు (i = 0; i < len; i++) {
        కీలకం = ngx_hash(కీలకం, ngx_tolower(దత్తాంశం[i]));
    }

    తిరిగి కీలకం;
}


ngx_uint_t
ngx_hash_strlow(u_char *dst, u_char *src, కొలత_t n)
{
    ngx_uint_t  కీలకం;

    కీలకం = 0;

    అయితే (n--) {
        *dst = ngx_tolower(*src);
        కీలకం = ngx_hash(కీలకం, *dst);
        dst++;
        src++;
    }

    తిరిగి కీలకం;
}


ngx_int_t
ngx_hash_కీలకాలు_array_init(ngx_hash_కీలకాలు_arrays_t *ha, ngx_uint_t type)
{
    ngx_uint_t  asize;

    ఒకవేళ (type == NGX_HASH_SMALL) {
        asize = 4;
        ha->hsize = 107;

    } లేకపోతే {
        asize = NGX_HASH_LARGE_ASIZE;
        ha->hsize = NGX_HASH_LARGE_HSIZE;
    }

    ఒకవేళ (ngx_array_init(&ha->కీలకాలు, ha->తాత్కాలిక_pool, asize, పరిమాణం(ngx_hash_కీలకం_t))
        != NGX_OK)
    {
        తిరిగి NGX_ERROR;
    }

    ఒకవేళ (ngx_array_init(&ha->dns_wc_తల, ha->తాత్కాలిక_pool, asize,
                       పరిమాణం(ngx_hash_కీలకం_t))
        != NGX_OK)
    {
        తిరిగి NGX_ERROR;
    }

    ఒకవేళ (ngx_array_init(&ha->dns_wc_తోక, ha->తాత్కాలిక_pool, asize,
                       పరిమాణం(ngx_hash_కీలకం_t))
        != NGX_OK)
    {
        తిరిగి NGX_ERROR;
    }

    ha->కీలకాలు_hash = ngx_pcalloc(ha->తాత్కాలిక_pool, పరిమాణం(ngx_array_t) * ha->hsize);
    ఒకవేళ (ha->కీలకాలు_hash == ఖాళీ) {
        తిరిగి NGX_ERROR;
    }

    ha->dns_wc_తల_hash = ngx_pcalloc(ha->తాత్కాలిక_pool,
                                       పరిమాణం(ngx_array_t) * ha->hsize);
    ఒకవేళ (ha->dns_wc_తల_hash == ఖాళీ) {
        తిరిగి NGX_ERROR;
    }

    ha->dns_wc_తోక_hash = ngx_pcalloc(ha->తాత్కాలిక_pool,
                                       పరిమాణం(ngx_array_t) * ha->hsize);
    ఒకవేళ (ha->dns_wc_తోక_hash == ఖాళీ) {
        తిరిగి NGX_ERROR;
    }

    తిరిగి NGX_OK;
}


ngx_int_t
ngx_hash_కూడు_కీలకం(ngx_hash_కీలకాలు_arrays_t *ha, ngx_str_t *కీలకం, శూన్య *విలువ,
    ngx_uint_t flags)
{
    కొలత_t           len;
    u_char          *p;
    ngx_str_t       *name;
    ngx_uint_t       i, k, n, skip, చివరి;
    ngx_array_t     *కీలకాలు, *hwc;
    ngx_hash_కీలకం_t  *hk;

    చివరి = కీలకం->len;

    ఒకవేళ (flags & NGX_HASH_WILDCARD_కీలకం) {

        /*
         * supported wildcards:
         *     "*.example.com", ".example.com", and "www.example.*"
         */

        n = 0;

        కొరకు (i = 0; i < కీలకం->len; i++) {

            ఒకవేళ (కీలకం->దత్తాంశం[i] == '*') {
                ఒకవేళ (++n > 1) {
                    తిరిగి NGX_DECLINED;
                }
            }

            ఒకవేళ (కీలకం->దత్తాంశం[i] == '.' && కీలకం->దత్తాంశం[i + 1] == '.') {
                తిరిగి NGX_DECLINED;
            }

            ఒకవేళ (కీలకం->దత్తాంశం[i] == '\0') {
                తిరిగి NGX_DECLINED;
            }
        }

        ఒకవేళ (కీలకం->len > 1 && కీలకం->దత్తాంశం[0] == '.') {
            skip = 1;
            వెళ్ళు wildcard;
        }

        ఒకవేళ (కీలకం->len > 2) {

            ఒకవేళ (కీలకం->దత్తాంశం[0] == '*' && కీలకం->దత్తాంశం[1] == '.') {
                skip = 2;
                వెళ్ళు wildcard;
            }

            ఒకవేళ (కీలకం->దత్తాంశం[i - 2] == '.' && కీలకం->దత్తాంశం[i - 1] == '*') {
                skip = 0;
                చివరి -= 2;
                వెళ్ళు wildcard;
            }
        }

        ఒకవేళ (n) {
            తిరిగి NGX_DECLINED;
        }
    }

    /* exact hash */

    k = 0;

    కొరకు (i = 0; i < చివరి; i++) {
        ఒకవేళ (!(flags & NGX_HASH_READONLY_కీలకం)) {
            కీలకం->దత్తాంశం[i] = ngx_tolower(కీలకం->దత్తాంశం[i]);
        }
        k = ngx_hash(k, కీలకం->దత్తాంశం[i]);
    }

    k %= ha->hsize;

    /* check conflicts in exact hash */

    name = ha->కీలకాలు_hash[k].elts;

    ఒకవేళ (name) {
        కొరకు (i = 0; i < ha->కీలకాలు_hash[k].nelts; i++) {
            ఒకవేళ (చివరి != name[i].len) {
                కొనసాగు;
            }

            ఒకవేళ (ngx_strncmp(కీలకం->దత్తాంశం, name[i].దత్తాంశం, చివరి) == 0) {
                తిరిగి NGX_BUSY;
            }
        }

    } లేకపోతే {
        ఒకవేళ (ngx_array_init(&ha->కీలకాలు_hash[k], ha->తాత్కాలిక_pool, 4,
                           పరిమాణం(ngx_str_t))
            != NGX_OK)
        {
            తిరిగి NGX_ERROR;
        }
    }

    name = ngx_array_నెట్టు(&ha->కీలకాలు_hash[k]);
    ఒకవేళ (name == ఖాళీ) {
        తిరిగి NGX_ERROR;
    }

    *name = *కీలకం;

    hk = ngx_array_నెట్టు(&ha->కీలకాలు);
    ఒకవేళ (hk == ఖాళీ) {
        తిరిగి NGX_ERROR;
    }

    hk->కీలకం = *కీలకం;
    hk->కీలకం_hash = ngx_hash_కీలకం(కీలకం->దత్తాంశం, చివరి);
    hk->విలువ = విలువ;

    తిరిగి NGX_OK;


wildcard:

    /* wildcard hash */

    k = ngx_hash_strlow(&కీలకం->దత్తాంశం[skip], &కీలకం->దత్తాంశం[skip], చివరి - skip);

    k %= ha->hsize;

    ఒకవేళ (skip == 1) {

        /* check conflicts in exact hash for ".example.com" */

        name = ha->కీలకాలు_hash[k].elts;

        ఒకవేళ (name) {
            len = చివరి - skip;

            కొరకు (i = 0; i < ha->కీలకాలు_hash[k].nelts; i++) {
                ఒకవేళ (len != name[i].len) {
                    కొనసాగు;
                }

                ఒకవేళ (ngx_strncmp(&కీలకం->దత్తాంశం[1], name[i].దత్తాంశం, len) == 0) {
                    తిరిగి NGX_BUSY;
                }
            }

        } లేకపోతే {
            ఒకవేళ (ngx_array_init(&ha->కీలకాలు_hash[k], ha->తాత్కాలిక_pool, 4,
                               పరిమాణం(ngx_str_t))
                != NGX_OK)
            {
                తిరిగి NGX_ERROR;
            }
        }

        name = ngx_array_నెట్టు(&ha->కీలకాలు_hash[k]);
        ఒకవేళ (name == ఖాళీ) {
            తిరిగి NGX_ERROR;
        }

        name->len = చివరి - 1;
        name->దత్తాంశం = ngx_pnalloc(ha->తాత్కాలిక_pool, name->len);
        ఒకవేళ (name->దత్తాంశం == ఖాళీ) {
            తిరిగి NGX_ERROR;
        }

        ngx_memcpy(name->దత్తాంశం, &కీలకం->దత్తాంశం[1], name->len);
    }


    ఒకవేళ (skip) {

        /*
         * convert "*.example.com" to "com.example.\0"
         *      and ".example.com" to "com.example\0"
         */

        p = ngx_pnalloc(ha->తాత్కాలిక_pool, చివరి);
        ఒకవేళ (p == ఖాళీ) {
            తిరిగి NGX_ERROR;
        }

        len = 0;
        n = 0;

        కొరకు (i = చివరి - 1; i; i--) {
            ఒకవేళ (కీలకం->దత్తాంశం[i] == '.') {
                ngx_memcpy(&p[n], &కీలకం->దత్తాంశం[i + 1], len);
                n += len;
                p[n++] = '.';
                len = 0;
                కొనసాగు;
            }

            len++;
        }

        ఒకవేళ (len) {
            ngx_memcpy(&p[n], &కీలకం->దత్తాంశం[1], len);
            n += len;
        }

        p[n] = '\0';

        hwc = &ha->dns_wc_తల;
        కీలకాలు = &ha->dns_wc_తల_hash[k];

    } లేకపోతే {

        /* convert "www.example.*" to "www.example\0" */

        చివరి++;

        p = ngx_pnalloc(ha->తాత్కాలిక_pool, చివరి);
        ఒకవేళ (p == ఖాళీ) {
            తిరిగి NGX_ERROR;
        }

        ngx_cpystrn(p, కీలకం->దత్తాంశం, చివరి);

        hwc = &ha->dns_wc_తోక;
        కీలకాలు = &ha->dns_wc_తోక_hash[k];
    }


    /* check conflicts in wildcard hash */

    name = కీలకాలు->elts;

    ఒకవేళ (name) {
        len = చివరి - skip;

        కొరకు (i = 0; i < కీలకాలు->nelts; i++) {
            ఒకవేళ (len != name[i].len) {
                కొనసాగు;
            }

            ఒకవేళ (ngx_strncmp(కీలకం->దత్తాంశం + skip, name[i].దత్తాంశం, len) == 0) {
                తిరిగి NGX_BUSY;
            }
        }

    } లేకపోతే {
        ఒకవేళ (ngx_array_init(కీలకాలు, ha->తాత్కాలిక_pool, 4, పరిమాణం(ngx_str_t)) != NGX_OK)
        {
            తిరిగి NGX_ERROR;
        }
    }

    name = ngx_array_నెట్టు(కీలకాలు);
    ఒకవేళ (name == ఖాళీ) {
        తిరిగి NGX_ERROR;
    }

    name->len = చివరి - skip;
    name->దత్తాంశం = ngx_pnalloc(ha->తాత్కాలిక_pool, name->len);
    ఒకవేళ (name->దత్తాంశం == ఖాళీ) {
        తిరిగి NGX_ERROR;
    }

    ngx_memcpy(name->దత్తాంశం, కీలకం->దత్తాంశం + skip, name->len);


    /* add to wildcard hash */

    hk = ngx_array_నెట్టు(hwc);
    ఒకవేళ (hk == ఖాళీ) {
        తిరిగి NGX_ERROR;
    }

    hk->కీలకం.len = చివరి - 1;
    hk->కీలకం.దత్తాంశం = p;
    hk->కీలకం_hash = 0;
    hk->విలువ = విలువ;

    తిరిగి NGX_OK;
}
