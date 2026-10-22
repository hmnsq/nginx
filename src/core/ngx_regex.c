
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    ngx_flag_t   pcre_jit;
    ngx_list_t  *studies;
} ngx_regex_conf_t;


ಸ್ಥಿರ ngx_inline ಶೂನ್ಯ ngx_regex_malloc_init(ngx_pool_t *pool);
ಸ್ಥಿರ ngx_inline ಶೂನ್ಯ ngx_regex_malloc_done(ಶೂನ್ಯ);

#ಯದಿ (NGX_PCRE2)
ಸ್ಥಿರ ಶೂನ್ಯ * ngx_libc_cdecl ngx_regex_malloc(size_t size, ಶೂನ್ಯ *data);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_libc_cdecl ngx_regex_free(ಶೂನ್ಯ *p, ಶೂನ್ಯ *data);
#ಅನ್ಯಥಾ
ಸ್ಥಿರ ಶೂನ್ಯ * ngx_libc_cdecl ngx_regex_malloc(size_t size);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_libc_cdecl ngx_regex_free(ಶೂನ್ಯ *p);
#endif
ಸ್ಥಿರ ಶೂನ್ಯ ngx_regex_cleanup(ಶೂನ್ಯ *data);

ಸ್ಥಿರ ngx_int_t ngx_regex_module_init(ngx_cycle_t *cycle);

ಸ್ಥಿರ ಶೂನ್ಯ *ngx_regex_create_conf(ngx_cycle_t *cycle);
ಸ್ಥಿರ ವರ್ಣ *ngx_regex_init_conf(ngx_cycle_t *cycle, ಶೂನ್ಯ *conf);

ಸ್ಥಿರ ವರ್ಣ *ngx_regex_pcre_jit(ngx_conf_t *cf, ಶೂನ್ಯ *post, ಶೂನ್ಯ *data);
ಸ್ಥಿರ ngx_conf_post_t  ngx_regex_pcre_jit_post = { ngx_regex_pcre_jit };


ಸ್ಥಿರ ngx_command_t  ngx_regex_commands[] = {

    { ngx_string("pcre_jit"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
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
    ರಿಕ್ತ,                                  /* init master */
    ngx_regex_module_init,                 /* init module */
    ರಿಕ್ತ,                                  /* init process */
    ರಿಕ್ತ,                                  /* init thread */
    ರಿಕ್ತ,                                  /* exit thread */
    ರಿಕ್ತ,                                  /* exit process */
    ರಿಕ್ತ,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


ಸ್ಥಿರ ngx_pool_t             *ngx_regex_pool;
ಸ್ಥಿರ ngx_list_t             *ngx_regex_studies;
ಸ್ಥಿರ ngx_uint_t              ngx_regex_direct_alloc;

#ಯದಿ (NGX_PCRE2)
ಸ್ಥಿರ pcre2_compile_context  *ngx_regex_compile_context;
ಸ್ಥಿರ pcre2_match_data       *ngx_regex_match_data;
ಸ್ಥಿರ ngx_uint_t              ngx_regex_match_data_size;
#endif


ಶೂನ್ಯ
ngx_regex_init(ಶೂನ್ಯ)
{
#ಯದಿ !(NGX_PCRE2)
    pcre_malloc = ngx_regex_malloc;
    pcre_free = ngx_regex_free;
#endif
}


ಸ್ಥಿರ ngx_inline ಶೂನ್ಯ
ngx_regex_malloc_init(ngx_pool_t *pool)
{
    ngx_regex_pool = pool;
    ngx_regex_direct_alloc = (pool == ರಿಕ್ತ) ? 1 : 0;
}


ಸ್ಥಿರ ngx_inline ಶೂನ್ಯ
ngx_regex_malloc_done(ಶೂನ್ಯ)
{
    ngx_regex_pool = ರಿಕ್ತ;
    ngx_regex_direct_alloc = 0;
}


#ಯದಿ (NGX_PCRE2)

ngx_int_t
ngx_regex_compile(ngx_regex_compile_t *rc)
{
    ಪೂರ್ಣಾಙ್ಕ                     n, errcode;
    ವರ್ಣ                   *p;
    u_char                  errstr[128];
    size_t                  erroff;
    uint32_t                options;
    pcre2_code             *re;
    ngx_regex_elt_t        *elt;
    pcre2_general_context  *gctx;
    pcre2_compile_context  *cctx;

    ಯದಿ (ngx_regex_compile_context == ರಿಕ್ತ) {
        /*
         * Allocate a compile context if not yet allocated.  This uses
         * direct allocations from heap, so the result can be cached
         * even at runtime.
         */

        ngx_regex_malloc_init(ರಿಕ್ತ);

        gctx = pcre2_general_context_create(ngx_regex_malloc, ngx_regex_free,
                                            ರಿಕ್ತ);
        ಯದಿ (gctx == ರಿಕ್ತ) {
            ngx_regex_malloc_done();
            ಗಚ್ಛ nomem;
        }

        cctx = pcre2_compile_context_create(gctx);
        ಯದಿ (cctx == ರಿಕ್ತ) {
            pcre2_general_context_free(gctx);
            ngx_regex_malloc_done();
            ಗಚ್ಛ nomem;
        }

        ngx_regex_compile_context = cctx;

        pcre2_general_context_free(gctx);
        ngx_regex_malloc_done();
    }

    options = 0;

    ಯದಿ (rc->options & NGX_REGEX_CASELESS) {
        options |= PCRE2_CASELESS;
    }

    ಯದಿ (rc->options & NGX_REGEX_MULTILINE) {
        options |= PCRE2_MULTILINE;
    }

    ಯದಿ (rc->options & ~(NGX_REGEX_CASELESS|NGX_REGEX_MULTILINE)) {
        rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                            "regex \"%V\" compilation failed: invalid options",
                            &rc->pattern)
                      - rc->err.data;
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    ngx_regex_malloc_init(rc->pool);

    re = pcre2_compile(rc->pattern.data, rc->pattern.len, options,
                       &errcode, &erroff, ngx_regex_compile_context);

    /* ensure that there is no current pool */
    ngx_regex_malloc_done();

    ಯದಿ (re == ರಿಕ್ತ) {
        pcre2_get_error_message(errcode, errstr, 128);

        ಯದಿ ((size_t) erroff == rc->pattern.len) {
            rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                              "pcre2_compile() failed: %s in \"%V\"",
                               errstr, &rc->pattern)
                          - rc->err.data;

        } ಅನ್ಯಥಾ {
            rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                              "pcre2_compile() failed: %s in \"%V\" at \"%s\"",
                               errstr, &rc->pattern, rc->pattern.data + erroff)
                          - rc->err.data;
        }

        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    rc->regex = re;

    /* do not study at runtime */

    ಯದಿ (ngx_regex_studies != ರಿಕ್ತ) {
        elt = ngx_list_ತಳ್ಳು(ngx_regex_studies);
        ಯದಿ (elt == ರಿಕ್ತ) {
            ಗಚ್ಛ nomem;
        }

        elt->regex = rc->regex;
        elt->name = rc->pattern.data;
    }

    n = pcre2_pattern_info(re, PCRE2_INFO_CAPTURECOUNT, &rc->captures);
    ಯದಿ (n < 0) {
        p = "pcre2_pattern_info(\"%V\", PCRE2_INFO_CAPTURECOUNT) failed: %d";
        ಗಚ್ಛ failed;
    }

    ಯದಿ (rc->captures == 0) {
        ಪ್ರತ್ಯಾಗಮ NGX_OK;
    }

    n = pcre2_pattern_info(re, PCRE2_INFO_NAMECOUNT, &rc->named_captures);
    ಯದಿ (n < 0) {
        p = "pcre2_pattern_info(\"%V\", PCRE2_INFO_NAMECOUNT) failed: %d";
        ಗಚ್ಛ failed;
    }

    ಯದಿ (rc->named_captures == 0) {
        ಪ್ರತ್ಯಾಗಮ NGX_OK;
    }

    n = pcre2_pattern_info(re, PCRE2_INFO_NAMEENTRYSIZE, &rc->name_size);
    ಯದಿ (n < 0) {
        p = "pcre2_pattern_info(\"%V\", PCRE2_INFO_NAMEENTRYSIZE) failed: %d";
        ಗಚ್ಛ failed;
    }

    n = pcre2_pattern_info(re, PCRE2_INFO_NAMETABLE, &rc->names);
    ಯದಿ (n < 0) {
        p = "pcre2_pattern_info(\"%V\", PCRE2_INFO_NAMETABLE) failed: %d";
        ಗಚ್ಛ failed;
    }

    ಪ್ರತ್ಯಾಗಮ NGX_OK;

failed:

    rc->err.len = ngx_snprintf(rc->err.data, rc->err.len, p, &rc->pattern, n)
                  - rc->err.data;
    ಪ್ರತ್ಯಾಗಮ NGX_ERROR;

nomem:

    rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                               "regex \"%V\" compilation failed: no memory",
                               &rc->pattern)
                  - rc->err.data;
    ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
}

#ಅನ್ಯಥಾ

ngx_int_t
ngx_regex_compile(ngx_regex_compile_t *rc)
{
    ಪೂರ್ಣಾಙ್ಕ               n, erroff;
    ವರ್ಣ             *p;
    pcre             *re;
    ಧ್ರುವ ವರ್ಣ       *errstr;
    ngx_uint_t        options;
    ngx_regex_elt_t  *elt;

    options = 0;

    ಯದಿ (rc->options & NGX_REGEX_CASELESS) {
        options |= PCRE_CASELESS;
    }

    ಯದಿ (rc->options & NGX_REGEX_MULTILINE) {
        options |= PCRE_MULTILINE;
    }

    ಯದಿ (rc->options & ~(NGX_REGEX_CASELESS|NGX_REGEX_MULTILINE)) {
        rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                            "regex \"%V\" compilation failed: invalid options",
                            &rc->pattern)
                      - rc->err.data;
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    ngx_regex_malloc_init(rc->pool);

    re = pcre_compile((ಧ್ರುವ ವರ್ಣ *) rc->pattern.data, (ಪೂರ್ಣಾಙ್ಕ) options,
                      &errstr, &erroff, ರಿಕ್ತ);

    /* ensure that there is no current pool */
    ngx_regex_malloc_done();

    ಯದಿ (re == ರಿಕ್ತ) {
        ಯದಿ ((size_t) erroff == rc->pattern.len) {
           rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                              "pcre_compile() failed: %s in \"%V\"",
                               errstr, &rc->pattern)
                         - rc->err.data;

        } ಅನ್ಯಥಾ {
           rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                              "pcre_compile() failed: %s in \"%V\" at \"%s\"",
                               errstr, &rc->pattern, rc->pattern.data + erroff)
                         - rc->err.data;
        }

        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    rc->regex = ngx_pcalloc(rc->pool, ಪರಿಮಾಣ(ngx_regex_t));
    ಯದಿ (rc->regex == ರಿಕ್ತ) {
        ಗಚ್ಛ nomem;
    }

    rc->regex->code = re;

    /* do not study at runtime */

    ಯದಿ (ngx_regex_studies != ರಿಕ್ತ) {
        elt = ngx_list_ತಳ್ಳು(ngx_regex_studies);
        ಯದಿ (elt == ರಿಕ್ತ) {
            ಗಚ್ಛ nomem;
        }

        elt->regex = rc->regex;
        elt->name = rc->pattern.data;
    }

    n = pcre_fullinfo(re, ರಿಕ್ತ, PCRE_INFO_CAPTURECOUNT, &rc->captures);
    ಯದಿ (n < 0) {
        p = "pcre_fullinfo(\"%V\", PCRE_INFO_CAPTURECOUNT) failed: %d";
        ಗಚ್ಛ failed;
    }

    ಯದಿ (rc->captures == 0) {
        ಪ್ರತ್ಯಾಗಮ NGX_OK;
    }

    n = pcre_fullinfo(re, ರಿಕ್ತ, PCRE_INFO_NAMECOUNT, &rc->named_captures);
    ಯದಿ (n < 0) {
        p = "pcre_fullinfo(\"%V\", PCRE_INFO_NAMECOUNT) failed: %d";
        ಗಚ್ಛ failed;
    }

    ಯದಿ (rc->named_captures == 0) {
        ಪ್ರತ್ಯಾಗಮ NGX_OK;
    }

    n = pcre_fullinfo(re, ರಿಕ್ತ, PCRE_INFO_NAMEENTRYSIZE, &rc->name_size);
    ಯದಿ (n < 0) {
        p = "pcre_fullinfo(\"%V\", PCRE_INFO_NAMEENTRYSIZE) failed: %d";
        ಗಚ್ಛ failed;
    }

    n = pcre_fullinfo(re, ರಿಕ್ತ, PCRE_INFO_NAMETABLE, &rc->names);
    ಯದಿ (n < 0) {
        p = "pcre_fullinfo(\"%V\", PCRE_INFO_NAMETABLE) failed: %d";
        ಗಚ್ಛ failed;
    }

    ಪ್ರತ್ಯಾಗಮ NGX_OK;

failed:

    rc->err.len = ngx_snprintf(rc->err.data, rc->err.len, p, &rc->pattern, n)
                  - rc->err.data;
    ಪ್ರತ್ಯಾಗಮ NGX_ERROR;

nomem:

    rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                               "regex \"%V\" compilation failed: no memory",
                               &rc->pattern)
                  - rc->err.data;
    ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
}

#endif


#ಯದಿ (NGX_PCRE2)

ngx_int_t
ngx_regex_exec(ngx_regex_t *re, ngx_str_t *s, ಪೂರ್ಣಾಙ್ಕ *captures, ngx_uint_t size)
{
    size_t      *ov;
    ngx_int_t    rc;
    ngx_uint_t   n, i;

    /*
     * The pcre2_match() function might allocate memory for backtracking
     * frames, typical allocations are from 40k and above.  So the allocator
     * is configured to do direct allocations from heap during matching.
     */

    ngx_regex_malloc_init(ರಿಕ್ತ);

    ಯದಿ (ngx_regex_match_data == ರಿಕ್ತ
        || size > ngx_regex_match_data_size)
    {
        /*
         * Allocate a match data if not yet allocated or smaller than
         * needed.
         */

        ಯದಿ (ngx_regex_match_data) {
            pcre2_match_data_free(ngx_regex_match_data);
        }

        ngx_regex_match_data_size = size;
        ngx_regex_match_data = pcre2_match_data_create(size / 3, ರಿಕ್ತ);

        ಯದಿ (ngx_regex_match_data == ರಿಕ್ತ) {
            rc = PCRE2_ERROR_NOMEMORY;
            ಗಚ್ಛ failed;
        }
    }

    rc = pcre2_match(re, s->data, s->len, 0, 0, ngx_regex_match_data, ರಿಕ್ತ);

    ಯದಿ (rc < 0) {
        ಗಚ್ಛ failed;
    }

    n = pcre2_get_ovector_ಎಣಿಕೆ(ngx_regex_match_data);
    ov = pcre2_get_ovector_pointer(ngx_regex_match_data);

    ಯದಿ (n > size / 3) {
        n = size / 3;
    }

    ಪ್ರತಿ (i = 0; i < n; i++) {
        captures[i * 2] = ov[i * 2];
        captures[i * 2 + 1] = ov[i * 2 + 1];
    }

failed:

    ngx_regex_malloc_done();

    ಪ್ರತ್ಯಾಗಮ rc;
}

#ಅನ್ಯಥಾ

ngx_int_t
ngx_regex_exec(ngx_regex_t *re, ngx_str_t *s, ಪೂರ್ಣಾಙ್ಕ *captures, ngx_uint_t size)
{
    ಪ್ರತ್ಯಾಗಮ pcre_exec(re->code, re->extra, (ಧ್ರುವ ವರ್ಣ *) s->data, s->len,
                     0, 0, captures, size);
}

#endif


ngx_int_t
ngx_regex_exec_array(ngx_array_t *a, ngx_str_t *s, ngx_log_t *log)
{
    ngx_int_t         n;
    ngx_uint_t        i;
    ngx_regex_elt_t  *re;

    re = a->elts;

    ಪ್ರತಿ (i = 0; i < a->nelts; i++) {

        n = ngx_regex_exec(re[i].regex, s, ರಿಕ್ತ, 0);

        ಯದಿ (n == NGX_REGEX_NO_MATCHED) {
            ಅನುವರ್ತ;
        }

        ಯದಿ (n < 0) {
            ngx_log_error(NGX_LOG_ALERT, log, 0,
                          ngx_regex_exec_n " failed: %i on \"%V\" using \"%s\"",
                          n, s, re[i].name);
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        /* match */

        ಪ್ರತ್ಯಾಗಮ NGX_OK;
    }

    ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;
}


#ಯದಿ (NGX_PCRE2)

ಸ್ಥಿರ ಶೂನ್ಯ * ngx_libc_cdecl
ngx_regex_malloc(size_t size, ಶೂನ್ಯ *data)
{
    ಯದಿ (ngx_regex_pool) {
        ಪ್ರತ್ಯಾಗಮ ngx_palloc(ngx_regex_pool, size);
    }

    ಯದಿ (ngx_regex_direct_alloc) {
        ಪ್ರತ್ಯಾಗಮ ngx_alloc(size, ngx_cycle->log);
    }

    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}


ಸ್ಥಿರ ಶೂನ್ಯ ngx_libc_cdecl
ngx_regex_free(ಶೂನ್ಯ *p, ಶೂನ್ಯ *data)
{
    ಯದಿ (ngx_regex_direct_alloc) {
        ngx_free(p);
    }

    ಪ್ರತ್ಯಾಗಮ;
}

#ಅನ್ಯಥಾ

ಸ್ಥಿರ ಶೂನ್ಯ * ngx_libc_cdecl
ngx_regex_malloc(size_t size)
{
    ಯದಿ (ngx_regex_pool) {
        ಪ್ರತ್ಯಾಗಮ ngx_palloc(ngx_regex_pool, size);
    }

    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}


ಸ್ಥಿರ ಶೂನ್ಯ ngx_libc_cdecl
ngx_regex_free(ಶೂನ್ಯ *p)
{
    ಪ್ರತ್ಯಾಗಮ;
}

#endif


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_regex_cleanup(ಶೂನ್ಯ *data)
{
#ಯದಿ (NGX_PCRE2 || NGX_HAVE_PCRE_JIT)
    ngx_regex_conf_t *rcf = data;

    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_regex_elt_t  *elts;

    part = &rcf->studies->part;
    elts = part->elts;

    ಪ್ರತಿ (i = 0; /* void */ ; i++) {

        ಯದಿ (i >= part->nelts) {
            ಯದಿ (part->next == ರಿಕ್ತ) {
                ಭಙ್ಗ;
            }

            part = part->next;
            elts = part->elts;
            i = 0;
        }

        /*
         * The PCRE JIT compiler uses mmap for its executable codes, so we
         * have to explicitly call the pcre_free_study() function to free
         * this memory.  In PCRE2, we call the pcre2_code_free() function
         * for the same reason.
         */

#ಯದಿ (NGX_PCRE2)
        pcre2_code_free(elts[i].regex);
#ಅನ್ಯಥಾ
        ಯದಿ (elts[i].regex->extra != ರಿಕ್ತ) {
            pcre_free_study(elts[i].regex->extra);
        }
#endif
    }
#endif

    /*
     * On configuration parsing errors ngx_regex_module_init() will not
     * be called.  Make sure ngx_regex_studies is properly cleared anyway.
     */

    ngx_regex_studies = ರಿಕ್ತ;

#ಯದಿ (NGX_PCRE2)

    /*
     * Free compile context and match data.  If needed at runtime by
     * the new cycle, these will be re-allocated.
     */

    ngx_regex_malloc_init(ರಿಕ್ತ);

    ಯದಿ (ngx_regex_compile_context) {
        pcre2_compile_context_free(ngx_regex_compile_context);
        ngx_regex_compile_context = ರಿಕ್ತ;
    }

    ಯದಿ (ngx_regex_match_data) {
        pcre2_match_data_free(ngx_regex_match_data);
        ngx_regex_match_data = ರಿಕ್ತ;
        ngx_regex_match_data_size = 0;
    }

    ngx_regex_malloc_done();

#endif
}


ಸ್ಥಿರ ngx_int_t
ngx_regex_module_init(ngx_cycle_t *cycle)
{
    ಪೂರ್ಣಾಙ್ಕ                opt;
#ಯದಿ !(NGX_PCRE2)
    ಧ್ರುವ ವರ್ಣ        *errstr;
#endif
    ngx_uint_t         i;
    ngx_list_part_t   *part;
    ngx_regex_elt_t   *elts;
    ngx_regex_conf_t  *rcf;

    opt = 0;

    rcf = (ngx_regex_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_regex_module);

#ಯದಿ (NGX_PCRE2 || NGX_HAVE_PCRE_JIT)

    ಯದಿ (rcf->pcre_jit) {
#ಯದಿ (NGX_PCRE2)
        opt = 1;
#ಅನ್ಯಥಾ
        opt = PCRE_STUDY_JIT_COMPILE;
#endif
    }

#endif

    ngx_regex_malloc_init(cycle->pool);

    part = &rcf->studies->part;
    elts = part->elts;

    ಪ್ರತಿ (i = 0; /* void */ ; i++) {

        ಯದಿ (i >= part->nelts) {
            ಯದಿ (part->next == ರಿಕ್ತ) {
                ಭಙ್ಗ;
            }

            part = part->next;
            elts = part->elts;
            i = 0;
        }

#ಯದಿ (NGX_PCRE2)

        ಯದಿ (opt) {
            ಪೂರ್ಣಾಙ್ಕ  n;

            n = pcre2_jit_compile(elts[i].regex, PCRE2_JIT_COMPLETE);

            ಯದಿ (n != 0) {
                ngx_log_error(NGX_LOG_INFO, cycle->log, 0,
                              "pcre2_jit_compile() failed: %d in \"%s\", "
                              "ignored",
                              n, elts[i].name);
            }
        }

#ಅನ್ಯಥಾ

        elts[i].regex->extra = pcre_study(elts[i].regex->code, opt, &errstr);

        ಯದಿ (errstr != ರಿಕ್ತ) {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, 0,
                          "pcre_study() failed: %s in \"%s\"",
                          errstr, elts[i].name);
        }

#ಯದಿ (NGX_HAVE_PCRE_JIT)
        ಯದಿ (opt & PCRE_STUDY_JIT_COMPILE) {
            ಪೂರ್ಣಾಙ್ಕ jit, n;

            jit = 0;
            n = pcre_fullinfo(elts[i].regex->code, elts[i].regex->extra,
                              PCRE_INFO_JIT, &jit);

            ಯದಿ (n != 0 || jit != 1) {
                ngx_log_error(NGX_LOG_INFO, cycle->log, 0,
                              "JIT compiler does not support pattern: \"%s\"",
                              elts[i].name);
            }
        }
#endif
#endif
    }

    ngx_regex_malloc_done();

    ngx_regex_studies = ರಿಕ್ತ;

    ಪ್ರತ್ಯಾಗಮ NGX_OK;
}


ಸ್ಥಿರ ಶೂನ್ಯ *
ngx_regex_create_conf(ngx_cycle_t *cycle)
{
    ngx_regex_conf_t    *rcf;
    ngx_pool_cleanup_t  *cln;

    rcf = ngx_pcalloc(cycle->pool, ಪರಿಮಾಣ(ngx_regex_conf_t));
    ಯದಿ (rcf == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    rcf->pcre_jit = NGX_CONF_UNSET;

    cln = ngx_pool_cleanup_add(cycle->pool, 0);
    ಯದಿ (cln == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    rcf->studies = ngx_list_create(cycle->pool, 8, ಪರಿಮಾಣ(ngx_regex_elt_t));
    ಯದಿ (rcf->studies == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    cln->handler = ngx_regex_cleanup;
    cln->data = rcf;

    ngx_regex_studies = rcf->studies;

    ಪ್ರತ್ಯಾಗಮ rcf;
}


ಸ್ಥಿರ ವರ್ಣ *
ngx_regex_init_conf(ngx_cycle_t *cycle, ಶೂನ್ಯ *conf)
{
    ngx_regex_conf_t *rcf = conf;

    ngx_conf_init_ಮೌಲ್ಯ(rcf->pcre_jit, 0);

    ಪ್ರತ್ಯಾಗಮ NGX_CONF_OK;
}


ಸ್ಥಿರ ವರ್ಣ *
ngx_regex_pcre_jit(ngx_conf_t *cf, ಶೂನ್ಯ *post, ಶೂನ್ಯ *data)
{
    ngx_flag_t  *fp = data;

    ಯದಿ (*fp == 0) {
        ಪ್ರತ್ಯಾಗಮ NGX_CONF_OK;
    }

#ಯದಿ (NGX_PCRE2)
    {
    ಪೂರ್ಣಾಙ್ಕ       r;
    uint32_t  jit;

    jit = 0;
    r = pcre2_config(PCRE2_CONFIG_JIT, &jit);

    ಯದಿ (r != 0 || jit != 1) {
        ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                           "PCRE2 library does not support JIT");
        *fp = 0;
    }
    }
#ನೋಚೇದ್ಯದಿ (NGX_HAVE_PCRE_JIT)
    {
    ಪೂರ್ಣಾಙ್ಕ  jit, r;

    jit = 0;
    r = pcre_config(PCRE_CONFIG_JIT, &jit);

    ಯದಿ (r != 0 || jit != 1) {
        ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                           "PCRE library does not support JIT");
        *fp = 0;
    }
    }
#ಅನ್ಯಥಾ
    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "nginx was built without PCRE JIT support");
    *fp = 0;
#endif

    ಪ್ರತ್ಯಾಗಮ NGX_CONF_OK;
}
