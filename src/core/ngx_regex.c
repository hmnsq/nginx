
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ngx_flag_t   pcre_jit;
    ngx_list_t  *studies;
} ngx_regex_conf_t;


ಸ್ಥಿರ ngx_inline ಶೂನ್ಯ ngx_regex_malloc_init(ngx_pool_t *pool);
ಸ್ಥಿರ ngx_inline ಶೂನ್ಯ ngx_regex_malloc_ಮುಗಿದ(ಶೂನ್ಯ);

#ಆದರೆ (NGX_PCRE2)
ಸ್ಥಿರ ಶೂನ್ಯ * ngx_libc_cdecl ngx_regex_malloc(ಗಾತ್ರ_t ಗಾತ್ರ_2, ಶೂನ್ಯ *ದತ್ತಾಂಶ);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_libc_cdecl ngx_regex_free(ಶೂನ್ಯ *p, ಶೂನ್ಯ *ದತ್ತಾಂಶ);
#ಇಲ್ಲದಿದ್ದರೆ
ಸ್ಥಿರ ಶೂನ್ಯ * ngx_libc_cdecl ngx_regex_malloc(ಗಾತ್ರ_t ಗಾತ್ರ_2);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_libc_cdecl ngx_regex_free(ಶೂನ್ಯ *p);
#ಆದರೆ_ಅಂತ್ಯ
ಸ್ಥಿರ ಶೂನ್ಯ ngx_regex_cleanup(ಶೂನ್ಯ *ದತ್ತಾಂಶ);

ಸ್ಥಿರ ngx_int_t ngx_regex_module_init(ngx_ಚಕ್ರ_t *ಚಕ್ರ);

ಸ್ಥಿರ ಶೂನ್ಯ *ngx_regex_create_conf(ngx_ಚಕ್ರ_t *ಚಕ್ರ);
ಸ್ಥಿರ ಅಕ್ಷರ *ngx_regex_init_conf(ngx_ಚಕ್ರ_t *ಚಕ್ರ, ಶೂನ್ಯ *conf);

ಸ್ಥಿರ ಅಕ್ಷರ *ngx_regex_pcre_jit(ngx_conf_t *cf, ಶೂನ್ಯ *post, ಶೂನ್ಯ *ದತ್ತಾಂಶ);
ಸ್ಥಿರ ngx_conf_post_t  ngx_regex_pcre_jit_post = { ngx_regex_pcre_jit };


ಸ್ಥಿರ ngx_command_t  ngx_regex_commands[] = {

    { ngx_string("pcre_jit"),
      NGX_MAIN_CONF|NGX_ನೇರ_CONF|NGX_CONF_FLAG,
      ngx_conf_ಗಣ_flag_ಸ್ಥಾನ,
      0,
      offsetof(ngx_regex_conf_t, pcre_jit),
      &ngx_regex_pcre_jit_post },

      ngx_null_command
};


ಸ್ಥಿರ ngx_core_module_t  ngx_regex_module_ctx = {
    ngx_string("regex"),
    ngx_regex_create_conf,
    ngx_regex_init_conf
};


ngx_module_t  ngx_regex_module = {
    NGX_MODULE_V1,
    &ngx_regex_module_ctx,                 /* module context */
    ngx_regex_commands,                    /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    ಖಾಲಿ,                                  /* init master */
    ngx_regex_module_init,                 /* init module */
    ಖಾಲಿ,                                  /* init process */
    ಖಾಲಿ,                                  /* init thread */
    ಖಾಲಿ,                                  /* exit thread */
    ಖಾಲಿ,                                  /* exit process */
    ಖಾಲಿ,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


ಸ್ಥಿರ ngx_pool_t             *ngx_regex_pool;
ಸ್ಥಿರ ngx_list_t             *ngx_regex_studies;
ಸ್ಥಿರ ngx_uint_t              ngx_regex_ನೇರ_alloc;

#ಆದರೆ (NGX_PCRE2)
ಸ್ಥಿರ pcre2_compile_context  *ngx_regex_compile_context;
ಸ್ಥಿರ pcre2_ಹೊಂದಿಕೆ_ದತ್ತಾಂಶ       *ngx_regex_ಹೊಂದಿಕೆ_ದತ್ತಾಂಶ;
ಸ್ಥಿರ ngx_uint_t              ngx_regex_ಹೊಂದಿಕೆ_ದತ್ತಾಂಶ_ಗಾತ್ರ;
#ಆದರೆ_ಅಂತ್ಯ


ಶೂನ್ಯ
ngx_regex_init(ಶೂನ್ಯ)
{
#ಆದರೆ !(NGX_PCRE2)
    pcre_malloc = ngx_regex_malloc;
    pcre_free = ngx_regex_free;
#ಆದರೆ_ಅಂತ್ಯ
}


ಸ್ಥಿರ ngx_inline ಶೂನ್ಯ
ngx_regex_malloc_init(ngx_pool_t *pool)
{
    ngx_regex_pool = pool;
    ngx_regex_ನೇರ_alloc = (pool == ಖಾಲಿ) ? 1 : 0;
}


ಸ್ಥಿರ ngx_inline ಶೂನ್ಯ
ngx_regex_malloc_ಮುಗಿದ(ಶೂನ್ಯ)
{
    ngx_regex_pool = ಖಾಲಿ;
    ngx_regex_ನೇರ_alloc = 0;
}


#ಆದರೆ (NGX_PCRE2)

ngx_int_t
ngx_regex_compile(ngx_regex_compile_t *rc)
{
    ಪೂರ್ಣಾಂಕ                     n, errcode;
    ಅಕ್ಷರ                   *p;
    u_char                  errstr[128];
    ಗಾತ್ರ_t                  erroff;
    uint32_t                options;
    pcre2_ಸಂಕೇತ             *re;
    ngx_regex_elt_t        *elt;
    pcre2_general_context  *gctx;
    pcre2_compile_context  *cctx;

    ಆದರೆ (ngx_regex_compile_context == ಖಾಲಿ) {
        /*
         * Allocate a compile context if not yet allocated.  This uses
         * direct allocations from heap, so the result can be cached
         * even at runtime.
         */

        ngx_regex_malloc_init(ಖಾಲಿ);

        gctx = pcre2_general_context_create(ngx_regex_malloc, ngx_regex_free,
                                            ಖಾಲಿ);
        ಆದರೆ (gctx == ಖಾಲಿ) {
            ngx_regex_malloc_ಮುಗಿದ();
            ಹೋಗು nomem;
        }

        cctx = pcre2_compile_context_create(gctx);
        ಆದರೆ (cctx == ಖಾಲಿ) {
            pcre2_general_context_free(gctx);
            ngx_regex_malloc_ಮುಗಿದ();
            ಹೋಗು nomem;
        }

        ngx_regex_compile_context = cctx;

        pcre2_general_context_free(gctx);
        ngx_regex_malloc_ಮುಗಿದ();
    }

    options = 0;

    ಆದರೆ (rc->options & NGX_REGEX_CASELESS) {
        options |= PCRE2_CASELESS;
    }

    ಆದರೆ (rc->options & NGX_REGEX_MULTILINE) {
        options |= PCRE2_MULTILINE;
    }

    ಆದರೆ (rc->options & ~(NGX_REGEX_CASELESS|NGX_REGEX_MULTILINE)) {
        rc->err.len = ngx_snprintf(rc->err.ದತ್ತಾಂಶ, rc->err.len,
                            "regex \"%V\" compilation failed: invalid options",
                            &rc->ನಮೂನೆ)
                      - rc->err.ದತ್ತಾಂಶ;
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    ngx_regex_malloc_init(rc->pool);

    re = pcre2_compile(rc->ನಮೂನೆ.ದತ್ತಾಂಶ, rc->ನಮೂನೆ.len, options,
                       &errcode, &erroff, ngx_regex_compile_context);

    /* ensure that there is no current pool */
    ngx_regex_malloc_ಮುಗಿದ();

    ಆದರೆ (re == ಖಾಲಿ) {
        pcre2_get_error_ಸಂದೇಶ(errcode, errstr, 128);

        ಆದರೆ ((ಗಾತ್ರ_t) erroff == rc->ನಮೂನೆ.len) {
            rc->err.len = ngx_snprintf(rc->err.ದತ್ತಾಂಶ, rc->err.len,
                              "pcre2_compile() failed: %s in \"%V\"",
                               errstr, &rc->ನಮೂನೆ)
                          - rc->err.ದತ್ತಾಂಶ;

        } ಇಲ್ಲದಿದ್ದರೆ {
            rc->err.len = ngx_snprintf(rc->err.ದತ್ತಾಂಶ, rc->err.len,
                              "pcre2_compile() failed: %s in \"%V\" at \"%s\"",
                               errstr, &rc->ನಮೂನೆ, rc->ನಮೂನೆ.ದತ್ತಾಂಶ + erroff)
                          - rc->err.ದತ್ತಾಂಶ;
        }

        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    rc->regex = re;

    /* do not study at runtime */

    ಆದರೆ (ngx_regex_studies != ಖಾಲಿ) {
        elt = ngx_list_ತಳ್ಳು(ngx_regex_studies);
        ಆದರೆ (elt == ಖಾಲಿ) {
            ಹೋಗು nomem;
        }

        elt->regex = rc->regex;
        elt->name = rc->ನಮೂನೆ.ದತ್ತಾಂಶ;
    }

    n = pcre2_ನಮೂನೆ_info(re, PCRE2_INFO_CAPTURECOUNT, &rc->captures);
    ಆದರೆ (n < 0) {
        p = "pcre2_pattern_info(\"%V\", PCRE2_INFO_CAPTURECOUNT) failed: %d";
        ಹೋಗು failed;
    }

    ಆದರೆ (rc->captures == 0) {
        ಹಿಂತಿರುಗಿ NGX_OK;
    }

    n = pcre2_ನಮೂನೆ_info(re, PCRE2_INFO_NAMECOUNT, &rc->named_captures);
    ಆದರೆ (n < 0) {
        p = "pcre2_pattern_info(\"%V\", PCRE2_INFO_NAMECOUNT) failed: %d";
        ಹೋಗು failed;
    }

    ಆದರೆ (rc->named_captures == 0) {
        ಹಿಂತಿರುಗಿ NGX_OK;
    }

    n = pcre2_ನಮೂನೆ_info(re, PCRE2_INFO_NAMEENTRYSIZE, &rc->name_ಗಾತ್ರ);
    ಆದರೆ (n < 0) {
        p = "pcre2_pattern_info(\"%V\", PCRE2_INFO_NAMEENTRYSIZE) failed: %d";
        ಹೋಗು failed;
    }

    n = pcre2_ನಮೂನೆ_info(re, PCRE2_INFO_NAMETABLE, &rc->ಹೆಸರುಗಳು);
    ಆದರೆ (n < 0) {
        p = "pcre2_pattern_info(\"%V\", PCRE2_INFO_NAMETABLE) failed: %d";
        ಹೋಗು failed;
    }

    ಹಿಂತಿರುಗಿ NGX_OK;

failed:

    rc->err.len = ngx_snprintf(rc->err.ದತ್ತಾಂಶ, rc->err.len, p, &rc->ನಮೂನೆ, n)
                  - rc->err.ದತ್ತಾಂಶ;
    ಹಿಂತಿರುಗಿ NGX_ERROR;

nomem:

    rc->err.len = ngx_snprintf(rc->err.ದತ್ತಾಂಶ, rc->err.len,
                               "regex \"%V\" compilation failed: no memory",
                               &rc->ನಮೂನೆ)
                  - rc->err.ದತ್ತಾಂಶ;
    ಹಿಂತಿರುಗಿ NGX_ERROR;
}

#ಇಲ್ಲದಿದ್ದರೆ

ngx_int_t
ngx_regex_compile(ngx_regex_compile_t *rc)
{
    ಪೂರ್ಣಾಂಕ               n, erroff;
    ಅಕ್ಷರ             *p;
    pcre             *re;
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ       *errstr;
    ngx_uint_t        options;
    ngx_regex_elt_t  *elt;

    options = 0;

    ಆದರೆ (rc->options & NGX_REGEX_CASELESS) {
        options |= PCRE_CASELESS;
    }

    ಆದರೆ (rc->options & NGX_REGEX_MULTILINE) {
        options |= PCRE_MULTILINE;
    }

    ಆದರೆ (rc->options & ~(NGX_REGEX_CASELESS|NGX_REGEX_MULTILINE)) {
        rc->err.len = ngx_snprintf(rc->err.ದತ್ತಾಂಶ, rc->err.len,
                            "regex \"%V\" compilation failed: invalid options",
                            &rc->ನಮೂನೆ)
                      - rc->err.ದತ್ತಾಂಶ;
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    ngx_regex_malloc_init(rc->pool);

    re = pcre_compile((ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *) rc->ನಮೂನೆ.ದತ್ತಾಂಶ, (ಪೂರ್ಣಾಂಕ) options,
                      &errstr, &erroff, ಖಾಲಿ);

    /* ensure that there is no current pool */
    ngx_regex_malloc_ಮುಗಿದ();

    ಆದರೆ (re == ಖಾಲಿ) {
        ಆದರೆ ((ಗಾತ್ರ_t) erroff == rc->ನಮೂನೆ.len) {
           rc->err.len = ngx_snprintf(rc->err.ದತ್ತಾಂಶ, rc->err.len,
                              "pcre_compile() failed: %s in \"%V\"",
                               errstr, &rc->ನಮೂನೆ)
                         - rc->err.ದತ್ತಾಂಶ;

        } ಇಲ್ಲದಿದ್ದರೆ {
           rc->err.len = ngx_snprintf(rc->err.ದತ್ತಾಂಶ, rc->err.len,
                              "pcre_compile() failed: %s in \"%V\" at \"%s\"",
                               errstr, &rc->ನಮೂನೆ, rc->ನಮೂನೆ.ದತ್ತಾಂಶ + erroff)
                         - rc->err.ದತ್ತಾಂಶ;
        }

        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    rc->regex = ngx_pcalloc(rc->pool, ಗಾತ್ರ(ngx_regex_t));
    ಆದರೆ (rc->regex == ಖಾಲಿ) {
        ಹೋಗು nomem;
    }

    rc->regex->ಸಂಕೇತ_2 = re;

    /* do not study at runtime */

    ಆದರೆ (ngx_regex_studies != ಖಾಲಿ) {
        elt = ngx_list_ತಳ್ಳು(ngx_regex_studies);
        ಆದರೆ (elt == ಖಾಲಿ) {
            ಹೋಗು nomem;
        }

        elt->regex = rc->regex;
        elt->name = rc->ನಮೂನೆ.ದತ್ತಾಂಶ;
    }

    n = pcre_fullinfo(re, ಖಾಲಿ, PCRE_INFO_CAPTURECOUNT, &rc->captures);
    ಆದರೆ (n < 0) {
        p = "pcre_fullinfo(\"%V\", PCRE_INFO_CAPTURECOUNT) failed: %d";
        ಹೋಗು failed;
    }

    ಆದರೆ (rc->captures == 0) {
        ಹಿಂತಿರುಗಿ NGX_OK;
    }

    n = pcre_fullinfo(re, ಖಾಲಿ, PCRE_INFO_NAMECOUNT, &rc->named_captures);
    ಆದರೆ (n < 0) {
        p = "pcre_fullinfo(\"%V\", PCRE_INFO_NAMECOUNT) failed: %d";
        ಹೋಗು failed;
    }

    ಆದರೆ (rc->named_captures == 0) {
        ಹಿಂತಿರುಗಿ NGX_OK;
    }

    n = pcre_fullinfo(re, ಖಾಲಿ, PCRE_INFO_NAMEENTRYSIZE, &rc->name_ಗಾತ್ರ);
    ಆದರೆ (n < 0) {
        p = "pcre_fullinfo(\"%V\", PCRE_INFO_NAMEENTRYSIZE) failed: %d";
        ಹೋಗು failed;
    }

    n = pcre_fullinfo(re, ಖಾಲಿ, PCRE_INFO_NAMETABLE, &rc->ಹೆಸರುಗಳು);
    ಆದರೆ (n < 0) {
        p = "pcre_fullinfo(\"%V\", PCRE_INFO_NAMETABLE) failed: %d";
        ಹೋಗು failed;
    }

    ಹಿಂತಿರುಗಿ NGX_OK;

failed:

    rc->err.len = ngx_snprintf(rc->err.ದತ್ತಾಂಶ, rc->err.len, p, &rc->ನಮೂನೆ, n)
                  - rc->err.ದತ್ತಾಂಶ;
    ಹಿಂತಿರುಗಿ NGX_ERROR;

nomem:

    rc->err.len = ngx_snprintf(rc->err.ದತ್ತಾಂಶ, rc->err.len,
                               "regex \"%V\" compilation failed: no memory",
                               &rc->ನಮೂನೆ)
                  - rc->err.ದತ್ತಾಂಶ;
    ಹಿಂತಿರುಗಿ NGX_ERROR;
}

#ಆದರೆ_ಅಂತ್ಯ


#ಆದರೆ (NGX_PCRE2)

ngx_int_t
ngx_regex_exec(ngx_regex_t *re, ngx_str_t *s, ಪೂರ್ಣಾಂಕ *captures, ngx_uint_t ಗಾತ್ರ_2)
{
    ಗಾತ್ರ_t      *ov;
    ngx_int_t    rc;
    ngx_uint_t   n, i;

    /*
     * The pcre2_match() function might allocate memory for backtracking
     * frames, typical allocations are from 40k and above.  So the allocator
     * is configured to do direct allocations from heap during matching.
     */

    ngx_regex_malloc_init(ಖಾಲಿ);

    ಆದರೆ (ngx_regex_ಹೊಂದಿಕೆ_ದತ್ತಾಂಶ == ಖಾಲಿ
        || ಗಾತ್ರ_2 > ngx_regex_ಹೊಂದಿಕೆ_ದತ್ತಾಂಶ_ಗಾತ್ರ)
    {
        /*
         * Allocate a match data if not yet allocated or smaller than
         * needed.
         */

        ಆದರೆ (ngx_regex_ಹೊಂದಿಕೆ_ದತ್ತಾಂಶ) {
            pcre2_ಹೊಂದಿಕೆ_ದತ್ತಾಂಶ_free(ngx_regex_ಹೊಂದಿಕೆ_ದತ್ತಾಂಶ);
        }

        ngx_regex_ಹೊಂದಿಕೆ_ದತ್ತಾಂಶ_ಗಾತ್ರ = ಗಾತ್ರ_2;
        ngx_regex_ಹೊಂದಿಕೆ_ದತ್ತಾಂಶ = pcre2_ಹೊಂದಿಕೆ_ದತ್ತಾಂಶ_create(ಗಾತ್ರ_2 / 3, ಖಾಲಿ);

        ಆದರೆ (ngx_regex_ಹೊಂದಿಕೆ_ದತ್ತಾಂಶ == ಖಾಲಿ) {
            rc = PCRE2_ERROR_NOMEMORY;
            ಹೋಗು failed;
        }
    }

    rc = pcre2_ಹೊಂದಿಕೆ(re, s->ದತ್ತಾಂಶ, s->len, 0, 0, ngx_regex_ಹೊಂದಿಕೆ_ದತ್ತಾಂಶ, ಖಾಲಿ);

    ಆದರೆ (rc < 0) {
        ಹೋಗು failed;
    }

    n = pcre2_get_ovector_ಎಣಿಕೆ(ngx_regex_ಹೊಂದಿಕೆ_ದತ್ತಾಂಶ);
    ov = pcre2_get_ovector_ಸೂಚಕ(ngx_regex_ಹೊಂದಿಕೆ_ದತ್ತಾಂಶ);

    ಆದರೆ (n > ಗಾತ್ರ_2 / 3) {
        n = ಗಾತ್ರ_2 / 3;
    }

    ಗಾಗಿ (i = 0; i < n; i++) {
        captures[i * 2] = ov[i * 2];
        captures[i * 2 + 1] = ov[i * 2 + 1];
    }

failed:

    ngx_regex_malloc_ಮುಗಿದ();

    ಹಿಂತಿರುಗಿ rc;
}

#ಇಲ್ಲದಿದ್ದರೆ

ngx_int_t
ngx_regex_exec(ngx_regex_t *re, ngx_str_t *s, ಪೂರ್ಣಾಂಕ *captures, ngx_uint_t ಗಾತ್ರ_2)
{
    ಹಿಂತಿರುಗಿ pcre_exec(re->ಸಂಕೇತ_2, re->extra, (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *) s->ದತ್ತಾಂಶ, s->len,
                     0, 0, captures, ಗಾತ್ರ_2);
}

#ಆದರೆ_ಅಂತ್ಯ


ngx_int_t
ngx_regex_exec_array(ngx_array_t *a, ngx_str_t *s, ngx_log_t *ಲಘುಗಣಕ)
{
    ngx_int_t         n;
    ngx_uint_t        i;
    ngx_regex_elt_t  *re;

    re = a->elts;

    ಗಾಗಿ (i = 0; i < a->nelts; i++) {

        n = ngx_regex_exec(re[i].regex, s, ಖಾಲಿ, 0);

        ಆದರೆ (n == NGX_REGEX_NO_MATCHED) {
            ಮುಂದುವರಿ;
        }

        ಆದರೆ (n < 0) {
            ngx_log_error(NGX_LOG_ALERT, ಲಘುಗಣಕ, 0,
                          ngx_regex_exec_n " failed: %i on \"%V\" using \"%s\"",
                          n, s, re[i].name);
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        /*ಹೊಂದಿಕೆ*/

        ಹಿಂತಿರುಗಿ NGX_OK;
    }

    ಹಿಂತಿರುಗಿ NGX_DECLINED;
}


#ಆದರೆ (NGX_PCRE2)

ಸ್ಥಿರ ಶೂನ್ಯ * ngx_libc_cdecl
ngx_regex_malloc(ಗಾತ್ರ_t ಗಾತ್ರ_2, ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
    ಆದರೆ (ngx_regex_pool) {
        ಹಿಂತಿರುಗಿ ngx_palloc(ngx_regex_pool, ಗಾತ್ರ_2);
    }

    ಆದರೆ (ngx_regex_ನೇರ_alloc) {
        ಹಿಂತಿರುಗಿ ngx_alloc(ಗಾತ್ರ_2, ngx_ಚಕ್ರ->ಲಘುಗಣಕ);
    }

    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}


ಸ್ಥಿರ ಶೂನ್ಯ ngx_libc_cdecl
ngx_regex_free(ಶೂನ್ಯ *p, ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
    ಆದರೆ (ngx_regex_ನೇರ_alloc) {
        ngx_free(p);
    }

    ಹಿಂತಿರುಗಿ;
}

#ಇಲ್ಲದಿದ್ದರೆ

ಸ್ಥಿರ ಶೂನ್ಯ * ngx_libc_cdecl
ngx_regex_malloc(ಗಾತ್ರ_t ಗಾತ್ರ_2)
{
    ಆದರೆ (ngx_regex_pool) {
        ಹಿಂತಿರುಗಿ ngx_palloc(ngx_regex_pool, ಗಾತ್ರ_2);
    }

    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}


ಸ್ಥಿರ ಶೂನ್ಯ ngx_libc_cdecl
ngx_regex_free(ಶೂನ್ಯ *p)
{
    ಹಿಂತಿರುಗಿ;
}

#ಆದರೆ_ಅಂತ್ಯ


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_regex_cleanup(ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
#ಆದರೆ (NGX_PCRE2 || NGX_HAVE_PCRE_JIT)
    ngx_regex_conf_t *rcf = ದತ್ತಾಂಶ;

    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_regex_elt_t  *elts;

    part = &rcf->studies->part;
    elts = part->elts;

    ಗಾಗಿ (i = 0; /* void */ ; i++) {

        ಆದರೆ (i >= part->nelts) {
            ಆದರೆ (part->ಮುಂದಿನ == ಖಾಲಿ) {
                ನಿಲ್ಲಿಸು;
            }

            part = part->ಮುಂದಿನ;
            elts = part->elts;
            i = 0;
        }

        /*
         * The PCRE JIT compiler uses mmap for its executable codes, so we
         * have to explicitly call the pcre_free_study() function to free
         * this memory.  In PCRE2, we call the pcre2_code_free() function
         * for the same reason.
         */

#ಆದರೆ (NGX_PCRE2)
        pcre2_ಸಂಕೇತ_free(elts[i].regex);
#ಇಲ್ಲದಿದ್ದರೆ
        ಆದರೆ (elts[i].regex->extra != ಖಾಲಿ) {
            pcre_free_study(elts[i].regex->extra);
        }
#ಆದರೆ_ಅಂತ್ಯ
    }
#ಆದರೆ_ಅಂತ್ಯ

    /*
     * On configuration parsing errors ngx_regex_module_init() will not
     * be called.  Make sure ngx_regex_studies is properly cleared anyway.
     */

    ngx_regex_studies = ಖಾಲಿ;

#ಆದರೆ (NGX_PCRE2)

    /*
     * Free compile context and match data.  If needed at runtime by
     * the new cycle, these will be re-allocated.
     */

    ngx_regex_malloc_init(ಖಾಲಿ);

    ಆದರೆ (ngx_regex_compile_context) {
        pcre2_compile_context_free(ngx_regex_compile_context);
        ngx_regex_compile_context = ಖಾಲಿ;
    }

    ಆದರೆ (ngx_regex_ಹೊಂದಿಕೆ_ದತ್ತಾಂಶ) {
        pcre2_ಹೊಂದಿಕೆ_ದತ್ತಾಂಶ_free(ngx_regex_ಹೊಂದಿಕೆ_ದತ್ತಾಂಶ);
        ngx_regex_ಹೊಂದಿಕೆ_ದತ್ತಾಂಶ = ಖಾಲಿ;
        ngx_regex_ಹೊಂದಿಕೆ_ದತ್ತಾಂಶ_ಗಾತ್ರ = 0;
    }

    ngx_regex_malloc_ಮುಗಿದ();

#ಆದರೆ_ಅಂತ್ಯ
}


ಸ್ಥಿರ ngx_int_t
ngx_regex_module_init(ngx_ಚಕ್ರ_t *ಚಕ್ರ)
{
    ಪೂರ್ಣಾಂಕ                opt;
#ಆದರೆ !(NGX_PCRE2)
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ        *errstr;
#ಆದರೆ_ಅಂತ್ಯ
    ngx_uint_t         i;
    ngx_list_part_t   *part;
    ngx_regex_elt_t   *elts;
    ngx_regex_conf_t  *rcf;

    opt = 0;

    rcf = (ngx_regex_conf_t *) ngx_get_conf(ಚಕ್ರ->conf_ctx, ngx_regex_module);

#ಆದರೆ (NGX_PCRE2 || NGX_HAVE_PCRE_JIT)

    ಆದರೆ (rcf->pcre_jit) {
#ಆದರೆ (NGX_PCRE2)
        opt = 1;
#ಇಲ್ಲದಿದ್ದರೆ
        opt = PCRE_STUDY_JIT_COMPILE;
#ಆದರೆ_ಅಂತ್ಯ
    }

#ಆದರೆ_ಅಂತ್ಯ

    ngx_regex_malloc_init(ಚಕ್ರ->pool);

    part = &rcf->studies->part;
    elts = part->elts;

    ಗಾಗಿ (i = 0; /* void */ ; i++) {

        ಆದರೆ (i >= part->nelts) {
            ಆದರೆ (part->ಮುಂದಿನ == ಖಾಲಿ) {
                ನಿಲ್ಲಿಸು;
            }

            part = part->ಮುಂದಿನ;
            elts = part->elts;
            i = 0;
        }

#ಆದರೆ (NGX_PCRE2)

        ಆದರೆ (opt) {
            ಪೂರ್ಣಾಂಕ  n;

            n = pcre2_jit_compile(elts[i].regex, PCRE2_JIT_COMPLETE);

            ಆದರೆ (n != 0) {
                ngx_log_error(NGX_LOG_INFO, ಚಕ್ರ->ಲಘುಗಣಕ, 0,
                              "pcre2_jit_compile() failed: %d in \"%s\", "
                              "ignored",
                              n, elts[i].name);
            }
        }

#ಇಲ್ಲದಿದ್ದರೆ

        elts[i].regex->extra = pcre_study(elts[i].regex->ಸಂಕೇತ_2, opt, &errstr);

        ಆದರೆ (errstr != ಖಾಲಿ) {
            ngx_log_error(NGX_LOG_ALERT, ಚಕ್ರ->ಲಘುಗಣಕ, 0,
                          "pcre_study() failed: %s in \"%s\"",
                          errstr, elts[i].name);
        }

#ಆದರೆ (NGX_HAVE_PCRE_JIT)
        ಆದರೆ (opt & PCRE_STUDY_JIT_COMPILE) {
            ಪೂರ್ಣಾಂಕ jit, n;

            jit = 0;
            n = pcre_fullinfo(elts[i].regex->ಸಂಕೇತ_2, elts[i].regex->extra,
                              PCRE_INFO_JIT, &jit);

            ಆದರೆ (n != 0 || jit != 1) {
                ngx_log_error(NGX_LOG_INFO, ಚಕ್ರ->ಲಘುಗಣಕ, 0,
                              "JIT compiler does not support pattern: \"%s\"",
                              elts[i].name);
            }
        }
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ
    }

    ngx_regex_malloc_ಮುಗಿದ();

    ngx_regex_studies = ಖಾಲಿ;

    ಹಿಂತಿರುಗಿ NGX_OK;
}


ಸ್ಥಿರ ಶೂನ್ಯ *
ngx_regex_create_conf(ngx_ಚಕ್ರ_t *ಚಕ್ರ)
{
    ngx_regex_conf_t    *rcf;
    ngx_pool_cleanup_t  *cln;

    rcf = ngx_pcalloc(ಚಕ್ರ->pool, ಗಾತ್ರ(ngx_regex_conf_t));
    ಆದರೆ (rcf == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    rcf->pcre_jit = NGX_CONF_UNSET;

    cln = ngx_pool_cleanup_ಕೂಡಿಸು(ಚಕ್ರ->pool, 0);
    ಆದರೆ (cln == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    rcf->studies = ngx_list_create(ಚಕ್ರ->pool, 8, ಗಾತ್ರ(ngx_regex_elt_t));
    ಆದರೆ (rcf->studies == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    cln->handler = ngx_regex_cleanup;
    cln->ದತ್ತಾಂಶ = rcf;

    ngx_regex_studies = rcf->studies;

    ಹಿಂತಿರುಗಿ rcf;
}


ಸ್ಥಿರ ಅಕ್ಷರ *
ngx_regex_init_conf(ngx_ಚಕ್ರ_t *ಚಕ್ರ, ಶೂನ್ಯ *conf)
{
    ngx_regex_conf_t *rcf = conf;

    ngx_conf_init_ಮೌಲ್ಯ(rcf->pcre_jit, 0);

    ಹಿಂತಿರುಗಿ NGX_CONF_OK;
}


ಸ್ಥಿರ ಅಕ್ಷರ *
ngx_regex_pcre_jit(ngx_conf_t *cf, ಶೂನ್ಯ *post, ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
    ngx_flag_t  *fp = ದತ್ತಾಂಶ;

    ಆದರೆ (*fp == 0) {
        ಹಿಂತಿರುಗಿ NGX_CONF_OK;
    }

#ಆದರೆ (NGX_PCRE2)
    {
    ಪೂರ್ಣಾಂಕ       r;
    uint32_t  jit;

    jit = 0;
    r = pcre2_config(PCRE2_CONFIG_JIT, &jit);

    ಆದರೆ (r != 0 || jit != 1) {
        ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                           "PCRE2 library does not support JIT");
        *fp = 0;
    }
    }
#ಅಥವಾಆದರೆ (NGX_HAVE_PCRE_JIT)
    {
    ಪೂರ್ಣಾಂಕ  jit, r;

    jit = 0;
    r = pcre_config(PCRE_CONFIG_JIT, &jit);

    ಆದರೆ (r != 0 || jit != 1) {
        ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                           "PCRE library does not support JIT");
        *fp = 0;
    }
    }
#ಇಲ್ಲದಿದ್ದರೆ
    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "nginx was built without PCRE JIT support");
    *fp = 0;
#ಆದರೆ_ಅಂತ್ಯ

    ಹಿಂತಿರುಗಿ NGX_CONF_OK;
}
