
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#समावेश <ngx_config.h>
#समावेश <ngx_core.h>


व्योम *
ngx_hash_find(ngx_hash_t *hash, ngx_uint_t कुंजी, u_char *name, माप_प्रकार len)
{
    ngx_uint_t       i;
    ngx_hash_elt_t  *elt;

#अगर 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "hf:\"%*s\"", len, name);
#पूर्ण_अगर

    elt = hash->buckets[कुंजी % hash->आकार];

    अगर (elt == शून्य) {
        वापस शून्य;
    }

    जबतक (elt->मूल्य) {
        अगर (len != (माप_प्रकार) elt->len) {
            जाओ अग्र;
        }

        क्रम (i = 0; i < len; i++) {
            अगर (name[i] != elt->name[i]) {
                जाओ अग्र;
            }
        }

        वापस elt->मूल्य;

    अग्र:

        elt = (ngx_hash_elt_t *) ngx_align_ptr(&elt->name[0] + elt->len,
                                               माप(व्योम *));
        जारी;
    }

    वापस शून्य;
}


व्योम *
ngx_hash_find_wc_शीर्ष(ngx_hash_wildcard_t *hwc, u_char *name, माप_प्रकार len)
{
    व्योम        *मूल्य;
    ngx_uint_t   i, n, कुंजी;

#अगर 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "wch:\"%*s\"", len, name);
#पूर्ण_अगर

    n = len;

    जबतक (n) {
        अगर (name[n - 1] == '.') {
            अवरोध;
        }

        n--;
    }

    कुंजी = 0;

    क्रम (i = n; i < len; i++) {
        कुंजी = ngx_hash(कुंजी, name[i]);
    }

#अगर 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "key:\"%ui\"", कुंजी);
#पूर्ण_अगर

    मूल्य = ngx_hash_find(&hwc->hash, कुंजी, &name[n], len - n);

#अगर 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "value:\"%p\"", मूल्य);
#पूर्ण_अगर

    अगर (मूल्य) {

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

        अगर ((uintptr_t) मूल्य & 2) {

            अगर (n == 0) {

                /* "example.com" */

                अगर ((uintptr_t) मूल्य & 1) {
                    वापस शून्य;
                }

                hwc = (ngx_hash_wildcard_t *)
                                          ((uintptr_t) मूल्य & (uintptr_t) ~3);
                वापस hwc->मूल्य;
            }

            hwc = (ngx_hash_wildcard_t *) ((uintptr_t) मूल्य & (uintptr_t) ~3);

            मूल्य = ngx_hash_find_wc_शीर्ष(hwc, name, n - 1);

            अगर (मूल्य) {
                वापस मूल्य;
            }

            वापस hwc->मूल्य;
        }

        अगर ((uintptr_t) मूल्य & 1) {

            अगर (n == 0) {

                /* "example.com" */

                वापस शून्य;
            }

            वापस (व्योम *) ((uintptr_t) मूल्य & (uintptr_t) ~3);
        }

        वापस मूल्य;
    }

    वापस hwc->मूल्य;
}


व्योम *
ngx_hash_find_wc_पुच्छ(ngx_hash_wildcard_t *hwc, u_char *name, माप_प्रकार len)
{
    व्योम        *मूल्य;
    ngx_uint_t   i, कुंजी;

#अगर 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "wct:\"%*s\"", len, name);
#पूर्ण_अगर

    कुंजी = 0;

    क्रम (i = 0; i < len; i++) {
        अगर (name[i] == '.') {
            अवरोध;
        }

        कुंजी = ngx_hash(कुंजी, name[i]);
    }

    अगर (i == len) {
        वापस शून्य;
    }

#अगर 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "key:\"%ui\"", कुंजी);
#पूर्ण_अगर

    मूल्य = ngx_hash_find(&hwc->hash, कुंजी, name, i);

#अगर 0
    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "value:\"%p\"", मूल्य);
#पूर्ण_अगर

    अगर (मूल्य) {

        /*
         * the 2 low bits of value have the special meaning:
         *     00 - value is data pointer;
         *     11 - value is pointer to wildcard hash allowing "example.*".
         */

        अगर ((uintptr_t) मूल्य & 2) {

            i++;

            hwc = (ngx_hash_wildcard_t *) ((uintptr_t) मूल्य & (uintptr_t) ~3);

            मूल्य = ngx_hash_find_wc_पुच्छ(hwc, &name[i], len - i);

            अगर (मूल्य) {
                वापस मूल्य;
            }

            वापस hwc->मूल्य;
        }

        वापस मूल्य;
    }

    वापस hwc->मूल्य;
}


व्योम *
ngx_hash_find_combined(ngx_hash_combined_t *hash, ngx_uint_t कुंजी, u_char *name,
    माप_प्रकार len)
{
    व्योम  *मूल्य;

    अगर (hash->hash.buckets) {
        मूल्य = ngx_hash_find(&hash->hash, कुंजी, name, len);

        अगर (मूल्य) {
            वापस मूल्य;
        }
    }

    अगर (len == 0) {
        वापस शून्य;
    }

    अगर (hash->wc_शीर्ष && hash->wc_शीर्ष->hash.buckets) {
        मूल्य = ngx_hash_find_wc_शीर्ष(hash->wc_शीर्ष, name, len);

        अगर (मूल्य) {
            वापस मूल्य;
        }
    }

    अगर (hash->wc_पुच्छ && hash->wc_पुच्छ->hash.buckets) {
        मूल्य = ngx_hash_find_wc_पुच्छ(hash->wc_पुच्छ, name, len);

        अगर (मूल्य) {
            वापस मूल्य;
        }
    }

    वापस शून्य;
}


#घोषणा NGX_HASH_ELT_आकार(name)                                               \
    (माप(व्योम *) + ngx_align((name)->कुंजी.len + 2, माप(व्योम *)))

ngx_int_t
ngx_hash_init(ngx_hash_init_t *hinit, ngx_hash_कुंजी_t *names, ngx_uint_t nelts)
{
    u_char          *elts;
    माप_प्रकार           len;
    u_short         *test;
    ngx_uint_t       i, n, कुंजी, आकार, start, bucket_आकार;
    ngx_hash_elt_t  *elt, **buckets;

    अगर (hinit->max_आकार == 0) {
        ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                      "could not build %s, you should "
                      "increase %s_max_size: %i",
                      hinit->name, hinit->name, hinit->max_आकार);
        वापस NGX_ERROR;
    }

    अगर (hinit->bucket_आकार > 65536 - ngx_cacheline_आकार) {
        ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                      "could not build %s, too large "
                      "%s_bucket_size: %i",
                      hinit->name, hinit->name, hinit->bucket_आकार);
        वापस NGX_ERROR;
    }

    क्रम (n = 0; n < nelts; n++) {
        अगर (names[n].कुंजी.तथ्य == शून्य) {
            जारी;
        }

        अगर (hinit->bucket_आकार < NGX_HASH_ELT_आकार(&names[n]) + माप(व्योम *))
        {
            ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                          "could not build %s, you should "
                          "increase %s_bucket_size: %i",
                          hinit->name, hinit->name, hinit->bucket_आकार);
            वापस NGX_ERROR;
        }
    }

    test = ngx_alloc(hinit->max_आकार * माप(u_short), hinit->pool->log);
    अगर (test == शून्य) {
        वापस NGX_ERROR;
    }

    bucket_आकार = hinit->bucket_आकार - माप(व्योम *);

    start = nelts / (bucket_आकार / (2 * माप(व्योम *)));
    start = start ? start : 1;

    अगर (hinit->max_आकार > 10000 && nelts && hinit->max_आकार / nelts < 100) {
        start = hinit->max_आकार - 1000;
    }

    क्रम (आकार = start; आकार <= hinit->max_आकार; आकार++) {

        ngx_memzero(test, आकार * माप(u_short));

        क्रम (n = 0; n < nelts; n++) {
            अगर (names[n].कुंजी.तथ्य == शून्य) {
                जारी;
            }

            कुंजी = names[n].कुंजी_hash % आकार;
            len = test[कुंजी] + NGX_HASH_ELT_आकार(&names[n]);

#अगर 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "%ui: %ui %uz \"%V\"",
                          आकार, कुंजी, len, &names[n].कुंजी);
#पूर्ण_अगर

            अगर (len > bucket_आकार) {
                जाओ अग्र;
            }

            test[कुंजी] = (u_short) len;
        }

        जाओ found;

    अग्र:

        जारी;
    }

    आकार = hinit->max_आकार;

    ngx_log_error(NGX_LOG_WARN, hinit->pool->log, 0,
                  "could not build optimal %s, you should increase "
                  "either %s_max_size: %i or %s_bucket_size: %i; "
                  "ignoring %s_bucket_size",
                  hinit->name, hinit->name, hinit->max_आकार,
                  hinit->name, hinit->bucket_आकार, hinit->name);

found:

    क्रम (i = 0; i < आकार; i++) {
        test[i] = माप(व्योम *);
    }

    क्रम (n = 0; n < nelts; n++) {
        अगर (names[n].कुंजी.तथ्य == शून्य) {
            जारी;
        }

        कुंजी = names[n].कुंजी_hash % आकार;
        len = test[कुंजी] + NGX_HASH_ELT_आकार(&names[n]);

        अगर (len > 65536 - ngx_cacheline_आकार) {
            ngx_log_error(NGX_LOG_EMERG, hinit->pool->log, 0,
                          "could not build %s, you should "
                          "increase %s_max_size: %i",
                          hinit->name, hinit->name, hinit->max_आकार);
            ngx_free(test);
            वापस NGX_ERROR;
        }

        test[कुंजी] = (u_short) len;
    }

    len = 0;

    क्रम (i = 0; i < आकार; i++) {
        अगर (test[i] == माप(व्योम *)) {
            जारी;
        }

        test[i] = (u_short) (ngx_align(test[i], ngx_cacheline_आकार));

        len += test[i];
    }

    अगर (hinit->hash == शून्य) {
        hinit->hash = ngx_pcalloc(hinit->pool, माप(ngx_hash_wildcard_t)
                                             + आकार * माप(ngx_hash_elt_t *));
        अगर (hinit->hash == शून्य) {
            ngx_free(test);
            वापस NGX_ERROR;
        }

        buckets = (ngx_hash_elt_t **)
                      ((u_char *) hinit->hash + माप(ngx_hash_wildcard_t));

    } अन्यथा {
        buckets = ngx_pcalloc(hinit->pool, आकार * माप(ngx_hash_elt_t *));
        अगर (buckets == शून्य) {
            ngx_free(test);
            वापस NGX_ERROR;
        }
    }

    elts = ngx_palloc(hinit->pool, len + ngx_cacheline_आकार);
    अगर (elts == शून्य) {
        ngx_free(test);
        वापस NGX_ERROR;
    }

    elts = ngx_align_ptr(elts, ngx_cacheline_आकार);

    क्रम (i = 0; i < आकार; i++) {
        अगर (test[i] == माप(व्योम *)) {
            जारी;
        }

        buckets[i] = (ngx_hash_elt_t *) elts;
        elts += test[i];
    }

    क्रम (i = 0; i < आकार; i++) {
        test[i] = 0;
    }

    क्रम (n = 0; n < nelts; n++) {
        अगर (names[n].कुंजी.तथ्य == शून्य) {
            जारी;
        }

        कुंजी = names[n].कुंजी_hash % आकार;
        elt = (ngx_hash_elt_t *) ((u_char *) buckets[कुंजी] + test[कुंजी]);

        elt->मूल्य = names[n].मूल्य;
        elt->len = (u_short) names[n].कुंजी.len;

        ngx_strlow(elt->name, names[n].कुंजी.तथ्य, names[n].कुंजी.len);

        test[कुंजी] = (u_short) (test[कुंजी] + NGX_HASH_ELT_आकार(&names[n]));
    }

    क्रम (i = 0; i < आकार; i++) {
        अगर (buckets[i] == शून्य) {
            जारी;
        }

        elt = (ngx_hash_elt_t *) ((u_char *) buckets[i] + test[i]);

        elt->मूल्य = शून्य;
    }

    ngx_free(test);

    hinit->hash->buckets = buckets;
    hinit->hash->आकार = आकार;

#अगर 0

    क्रम (i = 0; i < आकार; i++) {
        ngx_str_t   val;
        ngx_uint_t  कुंजी;

        elt = buckets[i];

        अगर (elt == शून्य) {
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "%ui: NULL", i);
            जारी;
        }

        जबतक (elt->मूल्य) {
            val.len = elt->len;
            val.तथ्य = &elt->name[0];

            कुंजी = hinit->कुंजी(val.तथ्य, val.len);

            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "%ui: %p \"%V\" %ui", i, elt, &val, कुंजी);

            elt = (ngx_hash_elt_t *) ngx_align_ptr(&elt->name[0] + elt->len,
                                                   माप(व्योम *));
        }
    }

#पूर्ण_अगर

    वापस NGX_OK;
}


ngx_int_t
ngx_hash_wildcard_init(ngx_hash_init_t *hinit, ngx_hash_कुंजी_t *names,
    ngx_uint_t nelts)
{
    माप_प्रकार                len, dot_len;
    ngx_uint_t            i, n, dot;
    ngx_array_t           curr_names, अग्र_names;
    ngx_hash_कुंजी_t       *name, *अग्र_name;
    ngx_hash_init_t       h;
    ngx_hash_wildcard_t  *wdc;

    अगर (ngx_array_init(&curr_names, hinit->अस्थायी_pool, nelts,
                       माप(ngx_hash_कुंजी_t))
        != NGX_OK)
    {
        वापस NGX_ERROR;
    }

    अगर (ngx_array_init(&अग्र_names, hinit->अस्थायी_pool, nelts,
                       माप(ngx_hash_कुंजी_t))
        != NGX_OK)
    {
        वापस NGX_ERROR;
    }

    क्रम (n = 0; n < nelts; n = i) {

#अगर 0
        ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                      "wc0: \"%V\"", &names[n].कुंजी);
#पूर्ण_अगर

        dot = 0;

        क्रम (len = 0; len < names[n].कुंजी.len; len++) {
            अगर (names[n].कुंजी.तथ्य[len] == '.') {
                dot = 1;
                अवरोध;
            }
        }

        name = ngx_array_धकेलो(&curr_names);
        अगर (name == शून्य) {
            वापस NGX_ERROR;
        }

        name->कुंजी.len = len;
        name->कुंजी.तथ्य = names[n].कुंजी.तथ्य;
        name->कुंजी_hash = hinit->कुंजी(name->कुंजी.तथ्य, name->कुंजी.len);
        name->मूल्य = names[n].मूल्य;

#अगर 0
        ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                      "wc1: \"%V\" %ui", &name->कुंजी, dot);
#पूर्ण_अगर

        dot_len = len + 1;

        अगर (dot) {
            len++;
        }

        अग्र_names.nelts = 0;

        अगर (names[n].कुंजी.len != len) {
            अग्र_name = ngx_array_धकेलो(&अग्र_names);
            अगर (अग्र_name == शून्य) {
                वापस NGX_ERROR;
            }

            अग्र_name->कुंजी.len = names[n].कुंजी.len - len;
            अग्र_name->कुंजी.तथ्य = names[n].कुंजी.तथ्य + len;
            अग्र_name->कुंजी_hash = 0;
            अग्र_name->मूल्य = names[n].मूल्य;

#अगर 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "wc2: \"%V\"", &अग्र_name->कुंजी);
#पूर्ण_अगर
        }

        क्रम (i = n + 1; i < nelts; i++) {
            अगर (ngx_strncmp(names[n].कुंजी.तथ्य, names[i].कुंजी.तथ्य, len) != 0) {
                अवरोध;
            }

            अगर (!dot
                && names[i].कुंजी.len > len
                && names[i].कुंजी.तथ्य[len] != '.')
            {
                अवरोध;
            }

            अग्र_name = ngx_array_धकेलो(&अग्र_names);
            अगर (अग्र_name == शून्य) {
                वापस NGX_ERROR;
            }

            अग्र_name->कुंजी.len = names[i].कुंजी.len - dot_len;
            अग्र_name->कुंजी.तथ्य = names[i].कुंजी.तथ्य + dot_len;
            अग्र_name->कुंजी_hash = 0;
            अग्र_name->मूल्य = names[i].मूल्य;

#अगर 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->log, 0,
                          "wc3: \"%V\"", &अग्र_name->कुंजी);
#पूर्ण_अगर
        }

        अगर (अग्र_names.nelts) {

            h = *hinit;
            h.hash = शून्य;

            अगर (ngx_hash_wildcard_init(&h, (ngx_hash_कुंजी_t *) अग्र_names.elts,
                                       अग्र_names.nelts)
                != NGX_OK)
            {
                वापस NGX_ERROR;
            }

            wdc = (ngx_hash_wildcard_t *) h.hash;

            अगर (names[n].कुंजी.len == len) {
                wdc->मूल्य = names[n].मूल्य;
            }

            name->मूल्य = (व्योम *) ((uintptr_t) wdc | (dot ? 3 : 2));

        } अन्यथा अगर (dot) {
            name->मूल्य = (व्योम *) ((uintptr_t) name->मूल्य | 1);
        }
    }

    अगर (ngx_hash_init(hinit, (ngx_hash_कुंजी_t *) curr_names.elts,
                      curr_names.nelts)
        != NGX_OK)
    {
        वापस NGX_ERROR;
    }

    वापस NGX_OK;
}


ngx_uint_t
ngx_hash_कुंजी(u_char *तथ्य, माप_प्रकार len)
{
    ngx_uint_t  i, कुंजी;

    कुंजी = 0;

    क्रम (i = 0; i < len; i++) {
        कुंजी = ngx_hash(कुंजी, तथ्य[i]);
    }

    वापस कुंजी;
}


ngx_uint_t
ngx_hash_कुंजी_lc(u_char *तथ्य, माप_प्रकार len)
{
    ngx_uint_t  i, कुंजी;

    कुंजी = 0;

    क्रम (i = 0; i < len; i++) {
        कुंजी = ngx_hash(कुंजी, ngx_tolower(तथ्य[i]));
    }

    वापस कुंजी;
}


ngx_uint_t
ngx_hash_strlow(u_char *dst, u_char *src, माप_प्रकार n)
{
    ngx_uint_t  कुंजी;

    कुंजी = 0;

    जबतक (n--) {
        *dst = ngx_tolower(*src);
        कुंजी = ngx_hash(कुंजी, *dst);
        dst++;
        src++;
    }

    वापस कुंजी;
}


ngx_int_t
ngx_hash_कुंजियाँ_array_init(ngx_hash_कुंजियाँ_arrays_t *ha, ngx_uint_t type)
{
    ngx_uint_t  asize;

    अगर (type == NGX_HASH_SMALL) {
        asize = 4;
        ha->hsize = 107;

    } अन्यथा {
        asize = NGX_HASH_LARGE_ASIZE;
        ha->hsize = NGX_HASH_LARGE_HSIZE;
    }

    अगर (ngx_array_init(&ha->कुंजियाँ, ha->अस्थायी_pool, asize, माप(ngx_hash_कुंजी_t))
        != NGX_OK)
    {
        वापस NGX_ERROR;
    }

    अगर (ngx_array_init(&ha->dns_wc_शीर्ष, ha->अस्थायी_pool, asize,
                       माप(ngx_hash_कुंजी_t))
        != NGX_OK)
    {
        वापस NGX_ERROR;
    }

    अगर (ngx_array_init(&ha->dns_wc_पुच्छ, ha->अस्थायी_pool, asize,
                       माप(ngx_hash_कुंजी_t))
        != NGX_OK)
    {
        वापस NGX_ERROR;
    }

    ha->कुंजियाँ_hash = ngx_pcalloc(ha->अस्थायी_pool, माप(ngx_array_t) * ha->hsize);
    अगर (ha->कुंजियाँ_hash == शून्य) {
        वापस NGX_ERROR;
    }

    ha->dns_wc_शीर्ष_hash = ngx_pcalloc(ha->अस्थायी_pool,
                                       माप(ngx_array_t) * ha->hsize);
    अगर (ha->dns_wc_शीर्ष_hash == शून्य) {
        वापस NGX_ERROR;
    }

    ha->dns_wc_पुच्छ_hash = ngx_pcalloc(ha->अस्थायी_pool,
                                       माप(ngx_array_t) * ha->hsize);
    अगर (ha->dns_wc_पुच्छ_hash == शून्य) {
        वापस NGX_ERROR;
    }

    वापस NGX_OK;
}


ngx_int_t
ngx_hash_add_कुंजी(ngx_hash_कुंजियाँ_arrays_t *ha, ngx_str_t *कुंजी, व्योम *मूल्य,
    ngx_uint_t flags)
{
    माप_प्रकार           len;
    u_char          *p;
    ngx_str_t       *name;
    ngx_uint_t       i, k, n, skip, last;
    ngx_array_t     *कुंजियाँ, *hwc;
    ngx_hash_कुंजी_t  *hk;

    last = कुंजी->len;

    अगर (flags & NGX_HASH_WILDCARD_कुंजी) {

        /*
         * supported wildcards:
         *     "*.example.com", ".example.com", and "www.example.*"
         */

        n = 0;

        क्रम (i = 0; i < कुंजी->len; i++) {

            अगर (कुंजी->तथ्य[i] == '*') {
                अगर (++n > 1) {
                    वापस NGX_DECLINED;
                }
            }

            अगर (कुंजी->तथ्य[i] == '.' && कुंजी->तथ्य[i + 1] == '.') {
                वापस NGX_DECLINED;
            }

            अगर (कुंजी->तथ्य[i] == '\0') {
                वापस NGX_DECLINED;
            }
        }

        अगर (कुंजी->len > 1 && कुंजी->तथ्य[0] == '.') {
            skip = 1;
            जाओ wildcard;
        }

        अगर (कुंजी->len > 2) {

            अगर (कुंजी->तथ्य[0] == '*' && कुंजी->तथ्य[1] == '.') {
                skip = 2;
                जाओ wildcard;
            }

            अगर (कुंजी->तथ्य[i - 2] == '.' && कुंजी->तथ्य[i - 1] == '*') {
                skip = 0;
                last -= 2;
                जाओ wildcard;
            }
        }

        अगर (n) {
            वापस NGX_DECLINED;
        }
    }

    /* exact hash */

    k = 0;

    क्रम (i = 0; i < last; i++) {
        अगर (!(flags & NGX_HASH_READONLY_कुंजी)) {
            कुंजी->तथ्य[i] = ngx_tolower(कुंजी->तथ्य[i]);
        }
        k = ngx_hash(k, कुंजी->तथ्य[i]);
    }

    k %= ha->hsize;

    /* check conflicts in exact hash */

    name = ha->कुंजियाँ_hash[k].elts;

    अगर (name) {
        क्रम (i = 0; i < ha->कुंजियाँ_hash[k].nelts; i++) {
            अगर (last != name[i].len) {
                जारी;
            }

            अगर (ngx_strncmp(कुंजी->तथ्य, name[i].तथ्य, last) == 0) {
                वापस NGX_BUSY;
            }
        }

    } अन्यथा {
        अगर (ngx_array_init(&ha->कुंजियाँ_hash[k], ha->अस्थायी_pool, 4,
                           माप(ngx_str_t))
            != NGX_OK)
        {
            वापस NGX_ERROR;
        }
    }

    name = ngx_array_धकेलो(&ha->कुंजियाँ_hash[k]);
    अगर (name == शून्य) {
        वापस NGX_ERROR;
    }

    *name = *कुंजी;

    hk = ngx_array_धकेलो(&ha->कुंजियाँ);
    अगर (hk == शून्य) {
        वापस NGX_ERROR;
    }

    hk->कुंजी = *कुंजी;
    hk->कुंजी_hash = ngx_hash_कुंजी(कुंजी->तथ्य, last);
    hk->मूल्य = मूल्य;

    वापस NGX_OK;


wildcard:

    /* wildcard hash */

    k = ngx_hash_strlow(&कुंजी->तथ्य[skip], &कुंजी->तथ्य[skip], last - skip);

    k %= ha->hsize;

    अगर (skip == 1) {

        /* check conflicts in exact hash for ".example.com" */

        name = ha->कुंजियाँ_hash[k].elts;

        अगर (name) {
            len = last - skip;

            क्रम (i = 0; i < ha->कुंजियाँ_hash[k].nelts; i++) {
                अगर (len != name[i].len) {
                    जारी;
                }

                अगर (ngx_strncmp(&कुंजी->तथ्य[1], name[i].तथ्य, len) == 0) {
                    वापस NGX_BUSY;
                }
            }

        } अन्यथा {
            अगर (ngx_array_init(&ha->कुंजियाँ_hash[k], ha->अस्थायी_pool, 4,
                               माप(ngx_str_t))
                != NGX_OK)
            {
                वापस NGX_ERROR;
            }
        }

        name = ngx_array_धकेलो(&ha->कुंजियाँ_hash[k]);
        अगर (name == शून्य) {
            वापस NGX_ERROR;
        }

        name->len = last - 1;
        name->तथ्य = ngx_pnalloc(ha->अस्थायी_pool, name->len);
        अगर (name->तथ्य == शून्य) {
            वापस NGX_ERROR;
        }

        ngx_memcpy(name->तथ्य, &कुंजी->तथ्य[1], name->len);
    }


    अगर (skip) {

        /*
         * convert "*.example.com" to "com.example.\0"
         *      and ".example.com" to "com.example\0"
         */

        p = ngx_pnalloc(ha->अस्थायी_pool, last);
        अगर (p == शून्य) {
            वापस NGX_ERROR;
        }

        len = 0;
        n = 0;

        क्रम (i = last - 1; i; i--) {
            अगर (कुंजी->तथ्य[i] == '.') {
                ngx_memcpy(&p[n], &कुंजी->तथ्य[i + 1], len);
                n += len;
                p[n++] = '.';
                len = 0;
                जारी;
            }

            len++;
        }

        अगर (len) {
            ngx_memcpy(&p[n], &कुंजी->तथ्य[1], len);
            n += len;
        }

        p[n] = '\0';

        hwc = &ha->dns_wc_शीर्ष;
        कुंजियाँ = &ha->dns_wc_शीर्ष_hash[k];

    } अन्यथा {

        /* convert "www.example.*" to "www.example\0" */

        last++;

        p = ngx_pnalloc(ha->अस्थायी_pool, last);
        अगर (p == शून्य) {
            वापस NGX_ERROR;
        }

        ngx_cpystrn(p, कुंजी->तथ्य, last);

        hwc = &ha->dns_wc_पुच्छ;
        कुंजियाँ = &ha->dns_wc_पुच्छ_hash[k];
    }


    /* check conflicts in wildcard hash */

    name = कुंजियाँ->elts;

    अगर (name) {
        len = last - skip;

        क्रम (i = 0; i < कुंजियाँ->nelts; i++) {
            अगर (len != name[i].len) {
                जारी;
            }

            अगर (ngx_strncmp(कुंजी->तथ्य + skip, name[i].तथ्य, len) == 0) {
                वापस NGX_BUSY;
            }
        }

    } अन्यथा {
        अगर (ngx_array_init(कुंजियाँ, ha->अस्थायी_pool, 4, माप(ngx_str_t)) != NGX_OK)
        {
            वापस NGX_ERROR;
        }
    }

    name = ngx_array_धकेलो(कुंजियाँ);
    अगर (name == शून्य) {
        वापस NGX_ERROR;
    }

    name->len = last - skip;
    name->तथ्य = ngx_pnalloc(ha->अस्थायी_pool, name->len);
    अगर (name->तथ्य == शून्य) {
        वापस NGX_ERROR;
    }

    ngx_memcpy(name->तथ्य, कुंजी->तथ्य + skip, name->len);


    /* add to wildcard hash */

    hk = ngx_array_धकेलो(hwc);
    अगर (hk == शून्य) {
        वापस NGX_ERROR;
    }

    hk->कुंजी.len = last - 1;
    hk->कुंजी.तथ्य = p;
    hk->कुंजी_hash = 0;
    hk->मूल्य = मूल्य;

    वापस NGX_OK;
}
