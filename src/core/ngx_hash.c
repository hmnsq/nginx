
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


शून्य *
ngx_hash_find(ngx_hash_t *hash, ngx_uint_t कुञ्चिका, u_char *name, आकार_t len)
{
    ngx_uint_t       i;
    ngx_hash_elt_t  *elt;

#यदि 0
    ngx_log_error(NGX_LOG_ALERT, ngx_आवृत्तिः->लघुगणकः, 0, "hf:\"%*s\"", len, name);
#यदि_अन्तः

    elt = hash->buckets[कुञ्चिका % hash->आकार];

    यदि (elt == रिक्त) {
        प्रत्यागम रिक्त;
    }

    यावत् (elt->मानम्) {
        यदि (len != (आकार_t) elt->len) {
            गच्छ अग्र;
        }

        प्रति (i = 0; i < len; i++) {
            यदि (name[i] != elt->name[i]) {
                गच्छ अग्र;
            }
        }

        प्रत्यागम elt->मानम्;

    अग्र:

        elt = (ngx_hash_elt_t *) ngx_align_ptr(&elt->name[0] + elt->len,
                                               परिमाण(शून्य *));
        अनुवर्त;
    }

    प्रत्यागम रिक्त;
}


शून्य *
ngx_hash_find_wc_शीर्ष(ngx_hash_wildcard_t *hwc, u_char *name, आकार_t len)
{
    शून्य        *मानम्;
    ngx_uint_t   i, n, कुञ्चिका;

#यदि 0
    ngx_log_error(NGX_LOG_ALERT, ngx_आवृत्तिः->लघुगणकः, 0, "wch:\"%*s\"", len, name);
#यदि_अन्तः

    n = len;

    यावत् (n) {
        यदि (name[n - 1] == '.') {
            भङ्ग;
        }

        n--;
    }

    कुञ्चिका = 0;

    प्रति (i = n; i < len; i++) {
        कुञ्चिका = ngx_hash(कुञ्चिका, name[i]);
    }

#यदि 0
    ngx_log_error(NGX_LOG_ALERT, ngx_आवृत्तिः->लघुगणकः, 0, "key:\"%ui\"", कुञ्चिका);
#यदि_अन्तः

    मानम् = ngx_hash_find(&hwc->hash, कुञ्चिका, &name[n], len - n);

#यदि 0
    ngx_log_error(NGX_LOG_ALERT, ngx_आवृत्तिः->लघुगणकः, 0, "value:\"%p\"", मानम्);
#यदि_अन्तः

    यदि (मानम्) {

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

        यदि ((uintptr_t) मानम् & 2) {

            यदि (n == 0) {

                /* "example.com" */

                यदि ((uintptr_t) मानम् & 1) {
                    प्रत्यागम रिक्त;
                }

                hwc = (ngx_hash_wildcard_t *)
                                          ((uintptr_t) मानम् & (uintptr_t) ~3);
                प्रत्यागम hwc->मानम्;
            }

            hwc = (ngx_hash_wildcard_t *) ((uintptr_t) मानम् & (uintptr_t) ~3);

            मानम् = ngx_hash_find_wc_शीर्ष(hwc, name, n - 1);

            यदि (मानम्) {
                प्रत्यागम मानम्;
            }

            प्रत्यागम hwc->मानम्;
        }

        यदि ((uintptr_t) मानम् & 1) {

            यदि (n == 0) {

                /* "example.com" */

                प्रत्यागम रिक्त;
            }

            प्रत्यागम (शून्य *) ((uintptr_t) मानम् & (uintptr_t) ~3);
        }

        प्रत्यागम मानम्;
    }

    प्रत्यागम hwc->मानम्;
}


शून्य *
ngx_hash_find_wc_पुच्छ(ngx_hash_wildcard_t *hwc, u_char *name, आकार_t len)
{
    शून्य        *मानम्;
    ngx_uint_t   i, कुञ्चिका;

#यदि 0
    ngx_log_error(NGX_LOG_ALERT, ngx_आवृत्तिः->लघुगणकः, 0, "wct:\"%*s\"", len, name);
#यदि_अन्तः

    कुञ्चिका = 0;

    प्रति (i = 0; i < len; i++) {
        यदि (name[i] == '.') {
            भङ्ग;
        }

        कुञ्चिका = ngx_hash(कुञ्चिका, name[i]);
    }

    यदि (i == len) {
        प्रत्यागम रिक्त;
    }

#यदि 0
    ngx_log_error(NGX_LOG_ALERT, ngx_आवृत्तिः->लघुगणकः, 0, "key:\"%ui\"", कुञ्चिका);
#यदि_अन्तः

    मानम् = ngx_hash_find(&hwc->hash, कुञ्चिका, name, i);

#यदि 0
    ngx_log_error(NGX_LOG_ALERT, ngx_आवृत्तिः->लघुगणकः, 0, "value:\"%p\"", मानम्);
#यदि_अन्तः

    यदि (मानम्) {

        /*
         * the 2 low bits of value have the special meaning:
         *     00 - value is data pointer;
         *     11 - value is pointer to wildcard hash allowing "example.*".
         */

        यदि ((uintptr_t) मानम् & 2) {

            i++;

            hwc = (ngx_hash_wildcard_t *) ((uintptr_t) मानम् & (uintptr_t) ~3);

            मानम् = ngx_hash_find_wc_पुच्छ(hwc, &name[i], len - i);

            यदि (मानम्) {
                प्रत्यागम मानम्;
            }

            प्रत्यागम hwc->मानम्;
        }

        प्रत्यागम मानम्;
    }

    प्रत्यागम hwc->मानम्;
}


शून्य *
ngx_hash_find_combined(ngx_hash_combined_t *hash, ngx_uint_t कुञ्चिका, u_char *name,
    आकार_t len)
{
    शून्य  *मानम्;

    यदि (hash->hash.buckets) {
        मानम् = ngx_hash_find(&hash->hash, कुञ्चिका, name, len);

        यदि (मानम्) {
            प्रत्यागम मानम्;
        }
    }

    यदि (len == 0) {
        प्रत्यागम रिक्त;
    }

    यदि (hash->wc_शीर्ष && hash->wc_शीर्ष->hash.buckets) {
        मानम् = ngx_hash_find_wc_शीर्ष(hash->wc_शीर्ष, name, len);

        यदि (मानम्) {
            प्रत्यागम मानम्;
        }
    }

    यदि (hash->wc_पुच्छ && hash->wc_पुच्छ->hash.buckets) {
        मानम् = ngx_hash_find_wc_पुच्छ(hash->wc_पुच्छ, name, len);

        यदि (मानम्) {
            प्रत्यागम मानम्;
        }
    }

    प्रत्यागम रिक्त;
}


#परिभाषय NGX_HASH_ELT_आकार(name)                                               \
    (परिमाण(शून्य *) + ngx_align((name)->कुञ्चिका.len + 2, परिमाण(शून्य *)))

ngx_int_t
ngx_hash_init(ngx_hash_init_t *hinit, ngx_hash_कुञ्चिका_t *नामानि, ngx_uint_t nelts)
{
    u_char          *elts;
    आकार_t           len;
    u_short         *test;
    ngx_uint_t       i, n, कुञ्चिका, आकार, start, bucket_आकार;
    ngx_hash_elt_t  *elt, **buckets;

    यदि (hinit->max_आकार == 0) {
        ngx_log_error(NGX_LOG_EMERG, hinit->pool->लघुगणकः, 0,
                      "could not build %s, you should "
                      "increase %s_max_size: %i",
                      hinit->name, hinit->name, hinit->max_आकार);
        प्रत्यागम NGX_ERROR;
    }

    यदि (hinit->bucket_आकार > 65536 - ngx_cacheline_आकार) {
        ngx_log_error(NGX_LOG_EMERG, hinit->pool->लघुगणकः, 0,
                      "could not build %s, too large "
                      "%s_bucket_size: %i",
                      hinit->name, hinit->name, hinit->bucket_आकार);
        प्रत्यागम NGX_ERROR;
    }

    प्रति (n = 0; n < nelts; n++) {
        यदि (नामानि[n].कुञ्चिका.दत्त == रिक्त) {
            अनुवर्त;
        }

        यदि (hinit->bucket_आकार < NGX_HASH_ELT_आकार(&नामानि[n]) + परिमाण(शून्य *))
        {
            ngx_log_error(NGX_LOG_EMERG, hinit->pool->लघुगणकः, 0,
                          "could not build %s, you should "
                          "increase %s_bucket_size: %i",
                          hinit->name, hinit->name, hinit->bucket_आकार);
            प्रत्यागम NGX_ERROR;
        }
    }

    test = ngx_alloc(hinit->max_आकार * परिमाण(u_short), hinit->pool->लघुगणकः);
    यदि (test == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

    bucket_आकार = hinit->bucket_आकार - परिमाण(शून्य *);

    start = nelts / (bucket_आकार / (2 * परिमाण(शून्य *)));
    start = start ? start : 1;

    यदि (hinit->max_आकार > 10000 && nelts && hinit->max_आकार / nelts < 100) {
        start = hinit->max_आकार - 1000;
    }

    प्रति (आकार = start; आकार <= hinit->max_आकार; आकार++) {

        ngx_memzero(test, आकार * परिमाण(u_short));

        प्रति (n = 0; n < nelts; n++) {
            यदि (नामानि[n].कुञ्चिका.दत्त == रिक्त) {
                अनुवर्त;
            }

            कुञ्चिका = नामानि[n].कुञ्चिका_hash % आकार;
            len = test[कुञ्चिका] + NGX_HASH_ELT_आकार(&नामानि[n]);

#यदि 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->लघुगणकः, 0,
                          "%ui: %ui %uz \"%V\"",
                          आकार, कुञ्चिका, len, &नामानि[n].कुञ्चिका);
#यदि_अन्तः

            यदि (len > bucket_आकार) {
                गच्छ अग्र;
            }

            test[कुञ्चिका] = (u_short) len;
        }

        गच्छ प्राप्तम्;

    अग्र:

        अनुवर्त;
    }

    आकार = hinit->max_आकार;

    ngx_log_error(NGX_LOG_WARN, hinit->pool->लघुगणकः, 0,
                  "could not build optimal %s, you should increase "
                  "either %s_max_size: %i or %s_bucket_size: %i; "
                  "ignoring %s_bucket_size",
                  hinit->name, hinit->name, hinit->max_आकार,
                  hinit->name, hinit->bucket_आकार, hinit->name);

प्राप्तम्:

    प्रति (i = 0; i < आकार; i++) {
        test[i] = परिमाण(शून्य *);
    }

    प्रति (n = 0; n < nelts; n++) {
        यदि (नामानि[n].कुञ्चिका.दत्त == रिक्त) {
            अनुवर्त;
        }

        कुञ्चिका = नामानि[n].कुञ्चिका_hash % आकार;
        len = test[कुञ्चिका] + NGX_HASH_ELT_आकार(&नामानि[n]);

        यदि (len > 65536 - ngx_cacheline_आकार) {
            ngx_log_error(NGX_LOG_EMERG, hinit->pool->लघुगणकः, 0,
                          "could not build %s, you should "
                          "increase %s_max_size: %i",
                          hinit->name, hinit->name, hinit->max_आकार);
            ngx_free(test);
            प्रत्यागम NGX_ERROR;
        }

        test[कुञ्चिका] = (u_short) len;
    }

    len = 0;

    प्रति (i = 0; i < आकार; i++) {
        यदि (test[i] == परिमाण(शून्य *)) {
            अनुवर्त;
        }

        test[i] = (u_short) (ngx_align(test[i], ngx_cacheline_आकार));

        len += test[i];
    }

    यदि (hinit->hash == रिक्त) {
        hinit->hash = ngx_pcalloc(hinit->pool, परिमाण(ngx_hash_wildcard_t)
                                             + आकार * परिमाण(ngx_hash_elt_t *));
        यदि (hinit->hash == रिक्त) {
            ngx_free(test);
            प्रत्यागम NGX_ERROR;
        }

        buckets = (ngx_hash_elt_t **)
                      ((u_char *) hinit->hash + परिमाण(ngx_hash_wildcard_t));

    } अन्यथा {
        buckets = ngx_pcalloc(hinit->pool, आकार * परिमाण(ngx_hash_elt_t *));
        यदि (buckets == रिक्त) {
            ngx_free(test);
            प्रत्यागम NGX_ERROR;
        }
    }

    elts = ngx_palloc(hinit->pool, len + ngx_cacheline_आकार);
    यदि (elts == रिक्त) {
        ngx_free(test);
        प्रत्यागम NGX_ERROR;
    }

    elts = ngx_align_ptr(elts, ngx_cacheline_आकार);

    प्रति (i = 0; i < आकार; i++) {
        यदि (test[i] == परिमाण(शून्य *)) {
            अनुवर्त;
        }

        buckets[i] = (ngx_hash_elt_t *) elts;
        elts += test[i];
    }

    प्रति (i = 0; i < आकार; i++) {
        test[i] = 0;
    }

    प्रति (n = 0; n < nelts; n++) {
        यदि (नामानि[n].कुञ्चिका.दत्त == रिक्त) {
            अनुवर्त;
        }

        कुञ्चिका = नामानि[n].कुञ्चिका_hash % आकार;
        elt = (ngx_hash_elt_t *) ((u_char *) buckets[कुञ्चिका] + test[कुञ्चिका]);

        elt->मानम् = नामानि[n].मानम्;
        elt->len = (u_short) नामानि[n].कुञ्चिका.len;

        ngx_strlow(elt->name, नामानि[n].कुञ्चिका.दत्त, नामानि[n].कुञ्चिका.len);

        test[कुञ्चिका] = (u_short) (test[कुञ्चिका] + NGX_HASH_ELT_आकार(&नामानि[n]));
    }

    प्रति (i = 0; i < आकार; i++) {
        यदि (buckets[i] == रिक्त) {
            अनुवर्त;
        }

        elt = (ngx_hash_elt_t *) ((u_char *) buckets[i] + test[i]);

        elt->मानम् = रिक्त;
    }

    ngx_free(test);

    hinit->hash->buckets = buckets;
    hinit->hash->आकार = आकार;

#यदि 0

    प्रति (i = 0; i < आकार; i++) {
        ngx_str_t   val;
        ngx_uint_t  कुञ्चिका;

        elt = buckets[i];

        यदि (elt == रिक्त) {
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->लघुगणकः, 0,
                          "%ui: NULL", i);
            अनुवर्त;
        }

        यावत् (elt->मानम्) {
            val.len = elt->len;
            val.दत्त = &elt->name[0];

            कुञ्चिका = hinit->कुञ्चिका(val.दत्त, val.len);

            ngx_log_error(NGX_LOG_ALERT, hinit->pool->लघुगणकः, 0,
                          "%ui: %p \"%V\" %ui", i, elt, &val, कुञ्चिका);

            elt = (ngx_hash_elt_t *) ngx_align_ptr(&elt->name[0] + elt->len,
                                                   परिमाण(शून्य *));
        }
    }

#यदि_अन्तः

    प्रत्यागम NGX_OK;
}


ngx_int_t
ngx_hash_wildcard_init(ngx_hash_init_t *hinit, ngx_hash_कुञ्चिका_t *नामानि,
    ngx_uint_t nelts)
{
    आकार_t                len, dot_len;
    ngx_uint_t            i, n, dot;
    ngx_array_t           curr_नामानि, अग्र_नामानि;
    ngx_hash_कुञ्चिका_t       *name, *अग्र_name;
    ngx_hash_init_t       h;
    ngx_hash_wildcard_t  *wdc;

    यदि (ngx_array_init(&curr_नामानि, hinit->अस्थायी_pool, nelts,
                       परिमाण(ngx_hash_कुञ्चिका_t))
        != NGX_OK)
    {
        प्रत्यागम NGX_ERROR;
    }

    यदि (ngx_array_init(&अग्र_नामानि, hinit->अस्थायी_pool, nelts,
                       परिमाण(ngx_hash_कुञ्चिका_t))
        != NGX_OK)
    {
        प्रत्यागम NGX_ERROR;
    }

    प्रति (n = 0; n < nelts; n = i) {

#यदि 0
        ngx_log_error(NGX_LOG_ALERT, hinit->pool->लघुगणकः, 0,
                      "wc0: \"%V\"", &नामानि[n].कुञ्चिका);
#यदि_अन्तः

        dot = 0;

        प्रति (len = 0; len < नामानि[n].कुञ्चिका.len; len++) {
            यदि (नामानि[n].कुञ्चिका.दत्त[len] == '.') {
                dot = 1;
                भङ्ग;
            }
        }

        name = ngx_array_प्रक्षिप(&curr_नामानि);
        यदि (name == रिक्त) {
            प्रत्यागम NGX_ERROR;
        }

        name->कुञ्चिका.len = len;
        name->कुञ्चिका.दत्त = नामानि[n].कुञ्चिका.दत्त;
        name->कुञ्चिका_hash = hinit->कुञ्चिका(name->कुञ्चिका.दत्त, name->कुञ्चिका.len);
        name->मानम् = नामानि[n].मानम्;

#यदि 0
        ngx_log_error(NGX_LOG_ALERT, hinit->pool->लघुगणकः, 0,
                      "wc1: \"%V\" %ui", &name->कुञ्चिका, dot);
#यदि_अन्तः

        dot_len = len + 1;

        यदि (dot) {
            len++;
        }

        अग्र_नामानि.nelts = 0;

        यदि (नामानि[n].कुञ्चिका.len != len) {
            अग्र_name = ngx_array_प्रक्षिप(&अग्र_नामानि);
            यदि (अग्र_name == रिक्त) {
                प्रत्यागम NGX_ERROR;
            }

            अग्र_name->कुञ्चिका.len = नामानि[n].कुञ्चिका.len - len;
            अग्र_name->कुञ्चिका.दत्त = नामानि[n].कुञ्चिका.दत्त + len;
            अग्र_name->कुञ्चिका_hash = 0;
            अग्र_name->मानम् = नामानि[n].मानम्;

#यदि 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->लघुगणकः, 0,
                          "wc2: \"%V\"", &अग्र_name->कुञ्चिका);
#यदि_अन्तः
        }

        प्रति (i = n + 1; i < nelts; i++) {
            यदि (ngx_strncmp(नामानि[n].कुञ्चिका.दत्त, नामानि[i].कुञ्चिका.दत्त, len) != 0) {
                भङ्ग;
            }

            यदि (!dot
                && नामानि[i].कुञ्चिका.len > len
                && नामानि[i].कुञ्चिका.दत्त[len] != '.')
            {
                भङ्ग;
            }

            अग्र_name = ngx_array_प्रक्षिप(&अग्र_नामानि);
            यदि (अग्र_name == रिक्त) {
                प्रत्यागम NGX_ERROR;
            }

            अग्र_name->कुञ्चिका.len = नामानि[i].कुञ्चिका.len - dot_len;
            अग्र_name->कुञ्चिका.दत्त = नामानि[i].कुञ्चिका.दत्त + dot_len;
            अग्र_name->कुञ्चिका_hash = 0;
            अग्र_name->मानम् = नामानि[i].मानम्;

#यदि 0
            ngx_log_error(NGX_LOG_ALERT, hinit->pool->लघुगणकः, 0,
                          "wc3: \"%V\"", &अग्र_name->कुञ्चिका);
#यदि_अन्तः
        }

        यदि (अग्र_नामानि.nelts) {

            h = *hinit;
            h.hash = रिक्त;

            यदि (ngx_hash_wildcard_init(&h, (ngx_hash_कुञ्चिका_t *) अग्र_नामानि.elts,
                                       अग्र_नामानि.nelts)
                != NGX_OK)
            {
                प्रत्यागम NGX_ERROR;
            }

            wdc = (ngx_hash_wildcard_t *) h.hash;

            यदि (नामानि[n].कुञ्चिका.len == len) {
                wdc->मानम् = नामानि[n].मानम्;
            }

            name->मानम् = (शून्य *) ((uintptr_t) wdc | (dot ? 3 : 2));

        } अन्यथा यदि (dot) {
            name->मानम् = (शून्य *) ((uintptr_t) name->मानम् | 1);
        }
    }

    यदि (ngx_hash_init(hinit, (ngx_hash_कुञ्चिका_t *) curr_नामानि.elts,
                      curr_नामानि.nelts)
        != NGX_OK)
    {
        प्रत्यागम NGX_ERROR;
    }

    प्रत्यागम NGX_OK;
}


ngx_uint_t
ngx_hash_कुञ्चिका(u_char *दत्त, आकार_t len)
{
    ngx_uint_t  i, कुञ्चिका;

    कुञ्चिका = 0;

    प्रति (i = 0; i < len; i++) {
        कुञ्चिका = ngx_hash(कुञ्चिका, दत्त[i]);
    }

    प्रत्यागम कुञ्चिका;
}


ngx_uint_t
ngx_hash_कुञ्चिका_lc(u_char *दत्त, आकार_t len)
{
    ngx_uint_t  i, कुञ्चिका;

    कुञ्चिका = 0;

    प्रति (i = 0; i < len; i++) {
        कुञ्चिका = ngx_hash(कुञ्चिका, ngx_tolower(दत्त[i]));
    }

    प्रत्यागम कुञ्चिका;
}


ngx_uint_t
ngx_hash_strlow(u_char *dst, u_char *src, आकार_t n)
{
    ngx_uint_t  कुञ्चिका;

    कुञ्चिका = 0;

    यावत् (n--) {
        *dst = ngx_tolower(*src);
        कुञ्चिका = ngx_hash(कुञ्चिका, *dst);
        dst++;
        src++;
    }

    प्रत्यागम कुञ्चिका;
}


ngx_int_t
ngx_hash_कुञ्चिकाः_array_init(ngx_hash_कुञ्चिकाः_arrays_t *ha, ngx_uint_t type)
{
    ngx_uint_t  asize;

    यदि (type == NGX_HASH_SMALL) {
        asize = 4;
        ha->hsize = 107;

    } अन्यथा {
        asize = NGX_HASH_LARGE_ASIZE;
        ha->hsize = NGX_HASH_LARGE_HSIZE;
    }

    यदि (ngx_array_init(&ha->कुञ्चिकाः, ha->अस्थायी_pool, asize, परिमाण(ngx_hash_कुञ्चिका_t))
        != NGX_OK)
    {
        प्रत्यागम NGX_ERROR;
    }

    यदि (ngx_array_init(&ha->dns_wc_शीर्ष, ha->अस्थायी_pool, asize,
                       परिमाण(ngx_hash_कुञ्चिका_t))
        != NGX_OK)
    {
        प्रत्यागम NGX_ERROR;
    }

    यदि (ngx_array_init(&ha->dns_wc_पुच्छ, ha->अस्थायी_pool, asize,
                       परिमाण(ngx_hash_कुञ्चिका_t))
        != NGX_OK)
    {
        प्रत्यागम NGX_ERROR;
    }

    ha->कुञ्चिकाः_hash = ngx_pcalloc(ha->अस्थायी_pool, परिमाण(ngx_array_t) * ha->hsize);
    यदि (ha->कुञ्चिकाः_hash == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

    ha->dns_wc_शीर्ष_hash = ngx_pcalloc(ha->अस्थायी_pool,
                                       परिमाण(ngx_array_t) * ha->hsize);
    यदि (ha->dns_wc_शीर्ष_hash == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

    ha->dns_wc_पुच्छ_hash = ngx_pcalloc(ha->अस्थायी_pool,
                                       परिमाण(ngx_array_t) * ha->hsize);
    यदि (ha->dns_wc_पुच्छ_hash == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

    प्रत्यागम NGX_OK;
}


ngx_int_t
ngx_hash_सङ्कलय_कुञ्चिका(ngx_hash_कुञ्चिकाः_arrays_t *ha, ngx_str_t *कुञ्चिका, शून्य *मानम्,
    ngx_uint_t flags)
{
    आकार_t           len;
    u_char          *p;
    ngx_str_t       *name;
    ngx_uint_t       i, k, n, skip, चरमः;
    ngx_array_t     *कुञ्चिकाः, *hwc;
    ngx_hash_कुञ्चिका_t  *hk;

    चरमः = कुञ्चिका->len;

    यदि (flags & NGX_HASH_WILDCARD_कुञ्चिका) {

        /*
         * supported wildcards:
         *     "*.example.com", ".example.com", and "www.example.*"
         */

        n = 0;

        प्रति (i = 0; i < कुञ्चिका->len; i++) {

            यदि (कुञ्चिका->दत्त[i] == '*') {
                यदि (++n > 1) {
                    प्रत्यागम NGX_DECLINED;
                }
            }

            यदि (कुञ्चिका->दत्त[i] == '.' && कुञ्चिका->दत्त[i + 1] == '.') {
                प्रत्यागम NGX_DECLINED;
            }

            यदि (कुञ्चिका->दत्त[i] == '\0') {
                प्रत्यागम NGX_DECLINED;
            }
        }

        यदि (कुञ्चिका->len > 1 && कुञ्चिका->दत्त[0] == '.') {
            skip = 1;
            गच्छ wildcard;
        }

        यदि (कुञ्चिका->len > 2) {

            यदि (कुञ्चिका->दत्त[0] == '*' && कुञ्चिका->दत्त[1] == '.') {
                skip = 2;
                गच्छ wildcard;
            }

            यदि (कुञ्चिका->दत्त[i - 2] == '.' && कुञ्चिका->दत्त[i - 1] == '*') {
                skip = 0;
                चरमः -= 2;
                गच्छ wildcard;
            }
        }

        यदि (n) {
            प्रत्यागम NGX_DECLINED;
        }
    }

    /* exact hash */

    k = 0;

    प्रति (i = 0; i < चरमः; i++) {
        यदि (!(flags & NGX_HASH_READONLY_कुञ्चिका)) {
            कुञ्चिका->दत्त[i] = ngx_tolower(कुञ्चिका->दत्त[i]);
        }
        k = ngx_hash(k, कुञ्चिका->दत्त[i]);
    }

    k %= ha->hsize;

    /* check conflicts in exact hash */

    name = ha->कुञ्चिकाः_hash[k].elts;

    यदि (name) {
        प्रति (i = 0; i < ha->कुञ्चिकाः_hash[k].nelts; i++) {
            यदि (चरमः != name[i].len) {
                अनुवर्त;
            }

            यदि (ngx_strncmp(कुञ्चिका->दत्त, name[i].दत्त, चरमः) == 0) {
                प्रत्यागम NGX_BUSY;
            }
        }

    } अन्यथा {
        यदि (ngx_array_init(&ha->कुञ्चिकाः_hash[k], ha->अस्थायी_pool, 4,
                           परिमाण(ngx_str_t))
            != NGX_OK)
        {
            प्रत्यागम NGX_ERROR;
        }
    }

    name = ngx_array_प्रक्षिप(&ha->कुञ्चिकाः_hash[k]);
    यदि (name == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

    *name = *कुञ्चिका;

    hk = ngx_array_प्रक्षिप(&ha->कुञ्चिकाः);
    यदि (hk == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

    hk->कुञ्चिका = *कुञ्चिका;
    hk->कुञ्चिका_hash = ngx_hash_कुञ्चिका(कुञ्चिका->दत्त, चरमः);
    hk->मानम् = मानम्;

    प्रत्यागम NGX_OK;


wildcard:

    /* wildcard hash */

    k = ngx_hash_strlow(&कुञ्चिका->दत्त[skip], &कुञ्चिका->दत्त[skip], चरमः - skip);

    k %= ha->hsize;

    यदि (skip == 1) {

        /* check conflicts in exact hash for ".example.com" */

        name = ha->कुञ्चिकाः_hash[k].elts;

        यदि (name) {
            len = चरमः - skip;

            प्रति (i = 0; i < ha->कुञ्चिकाः_hash[k].nelts; i++) {
                यदि (len != name[i].len) {
                    अनुवर्त;
                }

                यदि (ngx_strncmp(&कुञ्चिका->दत्त[1], name[i].दत्त, len) == 0) {
                    प्रत्यागम NGX_BUSY;
                }
            }

        } अन्यथा {
            यदि (ngx_array_init(&ha->कुञ्चिकाः_hash[k], ha->अस्थायी_pool, 4,
                               परिमाण(ngx_str_t))
                != NGX_OK)
            {
                प्रत्यागम NGX_ERROR;
            }
        }

        name = ngx_array_प्रक्षिप(&ha->कुञ्चिकाः_hash[k]);
        यदि (name == रिक्त) {
            प्रत्यागम NGX_ERROR;
        }

        name->len = चरमः - 1;
        name->दत्त = ngx_pnalloc(ha->अस्थायी_pool, name->len);
        यदि (name->दत्त == रिक्त) {
            प्रत्यागम NGX_ERROR;
        }

        ngx_memcpy(name->दत्त, &कुञ्चिका->दत्त[1], name->len);
    }


    यदि (skip) {

        /*
         * convert "*.example.com" to "com.example.\0"
         *      and ".example.com" to "com.example\0"
         */

        p = ngx_pnalloc(ha->अस्थायी_pool, चरमः);
        यदि (p == रिक्त) {
            प्रत्यागम NGX_ERROR;
        }

        len = 0;
        n = 0;

        प्रति (i = चरमः - 1; i; i--) {
            यदि (कुञ्चिका->दत्त[i] == '.') {
                ngx_memcpy(&p[n], &कुञ्चिका->दत्त[i + 1], len);
                n += len;
                p[n++] = '.';
                len = 0;
                अनुवर्त;
            }

            len++;
        }

        यदि (len) {
            ngx_memcpy(&p[n], &कुञ्चिका->दत्त[1], len);
            n += len;
        }

        p[n] = '\0';

        hwc = &ha->dns_wc_शीर्ष;
        कुञ्चिकाः = &ha->dns_wc_शीर्ष_hash[k];

    } अन्यथा {

        /* convert "www.example.*" to "www.example\0" */

        चरमः++;

        p = ngx_pnalloc(ha->अस्थायी_pool, चरमः);
        यदि (p == रिक्त) {
            प्रत्यागम NGX_ERROR;
        }

        ngx_cpystrn(p, कुञ्चिका->दत्त, चरमः);

        hwc = &ha->dns_wc_पुच्छ;
        कुञ्चिकाः = &ha->dns_wc_पुच्छ_hash[k];
    }


    /* check conflicts in wildcard hash */

    name = कुञ्चिकाः->elts;

    यदि (name) {
        len = चरमः - skip;

        प्रति (i = 0; i < कुञ्चिकाः->nelts; i++) {
            यदि (len != name[i].len) {
                अनुवर्त;
            }

            यदि (ngx_strncmp(कुञ्चिका->दत्त + skip, name[i].दत्त, len) == 0) {
                प्रत्यागम NGX_BUSY;
            }
        }

    } अन्यथा {
        यदि (ngx_array_init(कुञ्चिकाः, ha->अस्थायी_pool, 4, परिमाण(ngx_str_t)) != NGX_OK)
        {
            प्रत्यागम NGX_ERROR;
        }
    }

    name = ngx_array_प्रक्षिप(कुञ्चिकाः);
    यदि (name == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

    name->len = चरमः - skip;
    name->दत्त = ngx_pnalloc(ha->अस्थायी_pool, name->len);
    यदि (name->दत्त == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

    ngx_memcpy(name->दत्त, कुञ्चिका->दत्त + skip, name->len);


    /* add to wildcard hash */

    hk = ngx_array_प्रक्षिप(hwc);
    यदि (hk == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

    hk->कुञ्चिका.len = चरमः - 1;
    hk->कुञ्चिका.दत्त = p;
    hk->कुञ्चिका_hash = 0;
    hk->मानम् = मानम्;

    प्रत्यागम NGX_OK;
}
