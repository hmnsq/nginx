
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#অন্তর্ভাব <ngx_config.h>
#অন্তর্ভাব <ngx_core.h>


প্রকারলেখ কাঠামো {
    ngx_flag_t   pcre_jit;
    ngx_list_t  *studies;
} ngx_regex_conf_t;


স্থির ngx_inline শূন্য ngx_regex_malloc_init(ngx_pool_t *pool);
স্থির ngx_inline শূন্য ngx_regex_malloc_done(শূন্য);

#যদি (NGX_PCRE2)
স্থির শূন্য * ngx_libc_cdecl ngx_regex_malloc(আকার_t আকার_2, শূন্য *তথ্য);
স্থির শূন্য ngx_libc_cdecl ngx_regex_free(শূন্য *p, শূন্য *তথ্য);
#অন্যথা
স্থির শূন্য * ngx_libc_cdecl ngx_regex_malloc(আকার_t আকার_2);
স্থির শূন্য ngx_libc_cdecl ngx_regex_free(শূন্য *p);
#endif
স্থির শূন্য ngx_regex_cleanup(শূন্য *তথ্য);

স্থির ngx_int_t ngx_regex_module_init(ngx_cycle_t *cycle);

স্থির শূন্য *ngx_regex_create_conf(ngx_cycle_t *cycle);
স্থির অক্ষর *ngx_regex_init_conf(ngx_cycle_t *cycle, শূন্য *conf);

স্থির অক্ষর *ngx_regex_pcre_jit(ngx_conf_t *cf, শূন্য *post, শূন্য *তথ্য);
স্থির ngx_conf_post_t  ngx_regex_pcre_jit_post = { ngx_regex_pcre_jit };


স্থির ngx_command_t  ngx_regex_commands[] = {

    { ngx_string("pcre_jit"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      0,
      offsetof(ngx_regex_conf_t, pcre_jit),
      &ngx_regex_pcre_jit_post },

      ngx_null_command
};


স্থির ngx_core_module_t  ngx_regex_module_ctx = {
    ngx_string("regex"),
    ngx_regex_create_conf,
    ngx_regex_init_conf
};


ngx_module_t  ngx_regex_module = {
    NGX_MODULE_V1,
    &ngx_regex_module_ctx,                 /* module context */
    ngx_regex_commands,                    /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    ফাঁকা,                                  /* init master */
    ngx_regex_module_init,                 /* init module */
    ফাঁকা,                                  /* init process */
    ফাঁকা,                                  /* init thread */
    ফাঁকা,                                  /* exit thread */
    ফাঁকা,                                  /* exit process */
    ফাঁকা,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


স্থির ngx_pool_t             *ngx_regex_pool;
স্থির ngx_list_t             *ngx_regex_studies;
স্থির ngx_uint_t              ngx_regex_direct_alloc;

#যদি (NGX_PCRE2)
স্থির pcre2_compile_context  *ngx_regex_compile_context;
স্থির pcre2_match_তথ্য       *ngx_regex_match_তথ্য;
স্থির ngx_uint_t              ngx_regex_match_তথ্য_আকার;
#endif


শূন্য
ngx_regex_init(শূন্য)
{
#যদি !(NGX_PCRE2)
    pcre_malloc = ngx_regex_malloc;
    pcre_free = ngx_regex_free;
#endif
}


স্থির ngx_inline শূন্য
ngx_regex_malloc_init(ngx_pool_t *pool)
{
    ngx_regex_pool = pool;
    ngx_regex_direct_alloc = (pool == ফাঁকা) ? 1 : 0;
}


স্থির ngx_inline শূন্য
ngx_regex_malloc_done(শূন্য)
{
    ngx_regex_pool = ফাঁকা;
    ngx_regex_direct_alloc = 0;
}


#যদি (NGX_PCRE2)

ngx_int_t
ngx_regex_compile(ngx_regex_compile_t *rc)
{
    পূর্ণসংখ্যা                     n, errcode;
    অক্ষর                   *p;
    u_char                  errstr[128];
    আকার_t                  erroff;
    uint32_t                options;
    pcre2_code             *re;
    ngx_regex_elt_t        *elt;
    pcre2_general_context  *gctx;
    pcre2_compile_context  *cctx;

    যদি (ngx_regex_compile_context == ফাঁকা) {
        /*
         * Allocate a compile context if not yet allocated.  This uses
         * direct allocations from heap, so the result can be cached
         * even at runtime.
         */

        ngx_regex_malloc_init(ফাঁকা);

        gctx = pcre2_general_context_create(ngx_regex_malloc, ngx_regex_free,
                                            ফাঁকা);
        যদি (gctx == ফাঁকা) {
            ngx_regex_malloc_done();
            যাও nomem;
        }

        cctx = pcre2_compile_context_create(gctx);
        যদি (cctx == ফাঁকা) {
            pcre2_general_context_free(gctx);
            ngx_regex_malloc_done();
            যাও nomem;
        }

        ngx_regex_compile_context = cctx;

        pcre2_general_context_free(gctx);
        ngx_regex_malloc_done();
    }

    options = 0;

    যদি (rc->options & NGX_REGEX_CASELESS) {
        options |= PCRE2_CASELESS;
    }

    যদি (rc->options & NGX_REGEX_MULTILINE) {
        options |= PCRE2_MULTILINE;
    }

    যদি (rc->options & ~(NGX_REGEX_CASELESS|NGX_REGEX_MULTILINE)) {
        rc->err.len = ngx_snprintf(rc->err.তথ্য, rc->err.len,
                            "regex \"%V\" compilation failed: invalid options",
                            &rc->pattern)
                      - rc->err.তথ্য;
        ফেরত NGX_ERROR;
    }

    ngx_regex_malloc_init(rc->pool);

    re = pcre2_compile(rc->pattern.তথ্য, rc->pattern.len, options,
                       &errcode, &erroff, ngx_regex_compile_context);

    /* ensure that there is no current pool */
    ngx_regex_malloc_done();

    যদি (re == ফাঁকা) {
        pcre2_get_error_message(errcode, errstr, 128);

        যদি ((আকার_t) erroff == rc->pattern.len) {
            rc->err.len = ngx_snprintf(rc->err.তথ্য, rc->err.len,
                              "pcre2_compile() failed: %s in \"%V\"",
                               errstr, &rc->pattern)
                          - rc->err.তথ্য;

        } অন্যথা {
            rc->err.len = ngx_snprintf(rc->err.তথ্য, rc->err.len,
                              "pcre2_compile() failed: %s in \"%V\" at \"%s\"",
                               errstr, &rc->pattern, rc->pattern.তথ্য + erroff)
                          - rc->err.তথ্য;
        }

        ফেরত NGX_ERROR;
    }

    rc->regex = re;

    /* do not study at runtime */

    যদি (ngx_regex_studies != ফাঁকা) {
        elt = ngx_list_ঠেলো(ngx_regex_studies);
        যদি (elt == ফাঁকা) {
            যাও nomem;
        }

        elt->regex = rc->regex;
        elt->name = rc->pattern.তথ্য;
    }

    n = pcre2_pattern_info(re, PCRE2_INFO_CAPTURECOUNT, &rc->captures);
    যদি (n < 0) {
        p = "pcre2_pattern_info(\"%V\", PCRE2_INFO_CAPTURECOUNT) failed: %d";
        যাও failed;
    }

    যদি (rc->captures == 0) {
        ফেরত NGX_OK;
    }

    n = pcre2_pattern_info(re, PCRE2_INFO_NAMECOUNT, &rc->named_captures);
    যদি (n < 0) {
        p = "pcre2_pattern_info(\"%V\", PCRE2_INFO_NAMECOUNT) failed: %d";
        যাও failed;
    }

    যদি (rc->named_captures == 0) {
        ফেরত NGX_OK;
    }

    n = pcre2_pattern_info(re, PCRE2_INFO_NAMEENTRYSIZE, &rc->name_আকার);
    যদি (n < 0) {
        p = "pcre2_pattern_info(\"%V\", PCRE2_INFO_NAMEENTRYSIZE) failed: %d";
        যাও failed;
    }

    n = pcre2_pattern_info(re, PCRE2_INFO_NAMETABLE, &rc->names);
    যদি (n < 0) {
        p = "pcre2_pattern_info(\"%V\", PCRE2_INFO_NAMETABLE) failed: %d";
        যাও failed;
    }

    ফেরত NGX_OK;

failed:

    rc->err.len = ngx_snprintf(rc->err.তথ্য, rc->err.len, p, &rc->pattern, n)
                  - rc->err.তথ্য;
    ফেরত NGX_ERROR;

nomem:

    rc->err.len = ngx_snprintf(rc->err.তথ্য, rc->err.len,
                               "regex \"%V\" compilation failed: no memory",
                               &rc->pattern)
                  - rc->err.তথ্য;
    ফেরত NGX_ERROR;
}

#অন্যথা

ngx_int_t
ngx_regex_compile(ngx_regex_compile_t *rc)
{
    পূর্ণসংখ্যা               n, erroff;
    অক্ষর             *p;
    pcre             *re;
    ধ্রুবক অক্ষর       *errstr;
    ngx_uint_t        options;
    ngx_regex_elt_t  *elt;

    options = 0;

    যদি (rc->options & NGX_REGEX_CASELESS) {
        options |= PCRE_CASELESS;
    }

    যদি (rc->options & NGX_REGEX_MULTILINE) {
        options |= PCRE_MULTILINE;
    }

    যদি (rc->options & ~(NGX_REGEX_CASELESS|NGX_REGEX_MULTILINE)) {
        rc->err.len = ngx_snprintf(rc->err.তথ্য, rc->err.len,
                            "regex \"%V\" compilation failed: invalid options",
                            &rc->pattern)
                      - rc->err.তথ্য;
        ফেরত NGX_ERROR;
    }

    ngx_regex_malloc_init(rc->pool);

    re = pcre_compile((ধ্রুবক অক্ষর *) rc->pattern.তথ্য, (পূর্ণসংখ্যা) options,
                      &errstr, &erroff, ফাঁকা);

    /* ensure that there is no current pool */
    ngx_regex_malloc_done();

    যদি (re == ফাঁকা) {
        যদি ((আকার_t) erroff == rc->pattern.len) {
           rc->err.len = ngx_snprintf(rc->err.তথ্য, rc->err.len,
                              "pcre_compile() failed: %s in \"%V\"",
                               errstr, &rc->pattern)
                         - rc->err.তথ্য;

        } অন্যথা {
           rc->err.len = ngx_snprintf(rc->err.তথ্য, rc->err.len,
                              "pcre_compile() failed: %s in \"%V\" at \"%s\"",
                               errstr, &rc->pattern, rc->pattern.তথ্য + erroff)
                         - rc->err.তথ্য;
        }

        ফেরত NGX_ERROR;
    }

    rc->regex = ngx_pcalloc(rc->pool, আকার(ngx_regex_t));
    যদি (rc->regex == ফাঁকা) {
        যাও nomem;
    }

    rc->regex->code = re;

    /* do not study at runtime */

    যদি (ngx_regex_studies != ফাঁকা) {
        elt = ngx_list_ঠেলো(ngx_regex_studies);
        যদি (elt == ফাঁকা) {
            যাও nomem;
        }

        elt->regex = rc->regex;
        elt->name = rc->pattern.তথ্য;
    }

    n = pcre_fullinfo(re, ফাঁকা, PCRE_INFO_CAPTURECOUNT, &rc->captures);
    যদি (n < 0) {
        p = "pcre_fullinfo(\"%V\", PCRE_INFO_CAPTURECOUNT) failed: %d";
        যাও failed;
    }

    যদি (rc->captures == 0) {
        ফেরত NGX_OK;
    }

    n = pcre_fullinfo(re, ফাঁকা, PCRE_INFO_NAMECOUNT, &rc->named_captures);
    যদি (n < 0) {
        p = "pcre_fullinfo(\"%V\", PCRE_INFO_NAMECOUNT) failed: %d";
        যাও failed;
    }

    যদি (rc->named_captures == 0) {
        ফেরত NGX_OK;
    }

    n = pcre_fullinfo(re, ফাঁকা, PCRE_INFO_NAMEENTRYSIZE, &rc->name_আকার);
    যদি (n < 0) {
        p = "pcre_fullinfo(\"%V\", PCRE_INFO_NAMEENTRYSIZE) failed: %d";
        যাও failed;
    }

    n = pcre_fullinfo(re, ফাঁকা, PCRE_INFO_NAMETABLE, &rc->names);
    যদি (n < 0) {
        p = "pcre_fullinfo(\"%V\", PCRE_INFO_NAMETABLE) failed: %d";
        যাও failed;
    }

    ফেরত NGX_OK;

failed:

    rc->err.len = ngx_snprintf(rc->err.তথ্য, rc->err.len, p, &rc->pattern, n)
                  - rc->err.তথ্য;
    ফেরত NGX_ERROR;

nomem:

    rc->err.len = ngx_snprintf(rc->err.তথ্য, rc->err.len,
                               "regex \"%V\" compilation failed: no memory",
                               &rc->pattern)
                  - rc->err.তথ্য;
    ফেরত NGX_ERROR;
}

#endif


#যদি (NGX_PCRE2)

ngx_int_t
ngx_regex_exec(ngx_regex_t *re, ngx_str_t *s, পূর্ণসংখ্যা *captures, ngx_uint_t আকার_2)
{
    আকার_t      *ov;
    ngx_int_t    rc;
    ngx_uint_t   n, i;

    /*
     * The pcre2_match() function might allocate memory for backtracking
     * frames, typical allocations are from 40k and above.  So the allocator
     * is configured to do direct allocations from heap during matching.
     */

    ngx_regex_malloc_init(ফাঁকা);

    যদি (ngx_regex_match_তথ্য == ফাঁকা
        || আকার_2 > ngx_regex_match_তথ্য_আকার)
    {
        /*
         * Allocate a match data if not yet allocated or smaller than
         * needed.
         */

        যদি (ngx_regex_match_তথ্য) {
            pcre2_match_তথ্য_free(ngx_regex_match_তথ্য);
        }

        ngx_regex_match_তথ্য_আকার = আকার_2;
        ngx_regex_match_তথ্য = pcre2_match_তথ্য_create(আকার_2 / 3, ফাঁকা);

        যদি (ngx_regex_match_তথ্য == ফাঁকা) {
            rc = PCRE2_ERROR_NOMEMORY;
            যাও failed;
        }
    }

    rc = pcre2_match(re, s->তথ্য, s->len, 0, 0, ngx_regex_match_তথ্য, ফাঁকা);

    যদি (rc < 0) {
        যাও failed;
    }

    n = pcre2_get_ovector_গুনতি(ngx_regex_match_তথ্য);
    ov = pcre2_get_ovector_pointer(ngx_regex_match_তথ্য);

    যদি (n > আকার_2 / 3) {
        n = আকার_2 / 3;
    }

    জন্য (i = 0; i < n; i++) {
        captures[i * 2] = ov[i * 2];
        captures[i * 2 + 1] = ov[i * 2 + 1];
    }

failed:

    ngx_regex_malloc_done();

    ফেরত rc;
}

#অন্যথা

ngx_int_t
ngx_regex_exec(ngx_regex_t *re, ngx_str_t *s, পূর্ণসংখ্যা *captures, ngx_uint_t আকার_2)
{
    ফেরত pcre_exec(re->code, re->extra, (ধ্রুবক অক্ষর *) s->তথ্য, s->len,
                     0, 0, captures, আকার_2);
}

#endif


ngx_int_t
ngx_regex_exec_array(ngx_array_t *a, ngx_str_t *s, ngx_log_t *log)
{
    ngx_int_t         n;
    ngx_uint_t        i;
    ngx_regex_elt_t  *re;

    re = a->elts;

    জন্য (i = 0; i < a->nelts; i++) {

        n = ngx_regex_exec(re[i].regex, s, ফাঁকা, 0);

        যদি (n == NGX_REGEX_NO_MATCHED) {
            চালু;
        }

        যদি (n < 0) {
            ngx_log_error(NGX_LOG_ALERT, log, 0,
                          ngx_regex_exec_n " failed: %i on \"%V\" using \"%s\"",
                          n, s, re[i].name);
            ফেরত NGX_ERROR;
        }

        /* match */

        ফেরত NGX_OK;
    }

    ফেরত NGX_DECLINED;
}


#যদি (NGX_PCRE2)

স্থির শূন্য * ngx_libc_cdecl
ngx_regex_malloc(আকার_t আকার_2, শূন্য *তথ্য)
{
    যদি (ngx_regex_pool) {
        ফেরত ngx_palloc(ngx_regex_pool, আকার_2);
    }

    যদি (ngx_regex_direct_alloc) {
        ফেরত ngx_alloc(আকার_2, ngx_cycle->log);
    }

    ফেরত ফাঁকা;
}


স্থির শূন্য ngx_libc_cdecl
ngx_regex_free(শূন্য *p, শূন্য *তথ্য)
{
    যদি (ngx_regex_direct_alloc) {
        ngx_free(p);
    }

    ফেরত;
}

#অন্যথা

স্থির শূন্য * ngx_libc_cdecl
ngx_regex_malloc(আকার_t আকার_2)
{
    যদি (ngx_regex_pool) {
        ফেরত ngx_palloc(ngx_regex_pool, আকার_2);
    }

    ফেরত ফাঁকা;
}


স্থির শূন্য ngx_libc_cdecl
ngx_regex_free(শূন্য *p)
{
    ফেরত;
}

#endif


স্থির শূন্য
ngx_regex_cleanup(শূন্য *তথ্য)
{
#যদি (NGX_PCRE2 || NGX_HAVE_PCRE_JIT)
    ngx_regex_conf_t *rcf = তথ্য;

    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_regex_elt_t  *elts;

    part = &rcf->studies->part;
    elts = part->elts;

    জন্য (i = 0; /* void */ ; i++) {

        যদি (i >= part->nelts) {
            যদি (part->পরবর্তী == ফাঁকা) {
                বিরতি;
            }

            part = part->পরবর্তী;
            elts = part->elts;
            i = 0;
        }

        /*
         * The PCRE JIT compiler uses mmap for its executable codes, so we
         * have to explicitly call the pcre_free_study() function to free
         * this memory.  In PCRE2, we call the pcre2_code_free() function
         * for the same reason.
         */

#যদি (NGX_PCRE2)
        pcre2_code_free(elts[i].regex);
#অন্যথা
        যদি (elts[i].regex->extra != ফাঁকা) {
            pcre_free_study(elts[i].regex->extra);
        }
#endif
    }
#endif

    /*
     * On configuration parsing errors ngx_regex_module_init() will not
     * be called.  Make sure ngx_regex_studies is properly cleared anyway.
     */

    ngx_regex_studies = ফাঁকা;

#যদি (NGX_PCRE2)

    /*
     * Free compile context and match data.  If needed at runtime by
     * the new cycle, these will be re-allocated.
     */

    ngx_regex_malloc_init(ফাঁকা);

    যদি (ngx_regex_compile_context) {
        pcre2_compile_context_free(ngx_regex_compile_context);
        ngx_regex_compile_context = ফাঁকা;
    }

    যদি (ngx_regex_match_তথ্য) {
        pcre2_match_তথ্য_free(ngx_regex_match_তথ্য);
        ngx_regex_match_তথ্য = ফাঁকা;
        ngx_regex_match_তথ্য_আকার = 0;
    }

    ngx_regex_malloc_done();

#endif
}


স্থির ngx_int_t
ngx_regex_module_init(ngx_cycle_t *cycle)
{
    পূর্ণসংখ্যা                opt;
#যদি !(NGX_PCRE2)
    ধ্রুবক অক্ষর        *errstr;
#endif
    ngx_uint_t         i;
    ngx_list_part_t   *part;
    ngx_regex_elt_t   *elts;
    ngx_regex_conf_t  *rcf;

    opt = 0;

    rcf = (ngx_regex_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_regex_module);

#যদি (NGX_PCRE2 || NGX_HAVE_PCRE_JIT)

    যদি (rcf->pcre_jit) {
#যদি (NGX_PCRE2)
        opt = 1;
#অন্যথা
        opt = PCRE_STUDY_JIT_COMPILE;
#endif
    }

#endif

    ngx_regex_malloc_init(cycle->pool);

    part = &rcf->studies->part;
    elts = part->elts;

    জন্য (i = 0; /* void */ ; i++) {

        যদি (i >= part->nelts) {
            যদি (part->পরবর্তী == ফাঁকা) {
                বিরতি;
            }

            part = part->পরবর্তী;
            elts = part->elts;
            i = 0;
        }

#যদি (NGX_PCRE2)

        যদি (opt) {
            পূর্ণসংখ্যা  n;

            n = pcre2_jit_compile(elts[i].regex, PCRE2_JIT_COMPLETE);

            যদি (n != 0) {
                ngx_log_error(NGX_LOG_INFO, cycle->log, 0,
                              "pcre2_jit_compile() failed: %d in \"%s\", "
                              "ignored",
                              n, elts[i].name);
            }
        }

#অন্যথা

        elts[i].regex->extra = pcre_study(elts[i].regex->code, opt, &errstr);

        যদি (errstr != ফাঁকা) {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, 0,
                          "pcre_study() failed: %s in \"%s\"",
                          errstr, elts[i].name);
        }

#যদি (NGX_HAVE_PCRE_JIT)
        যদি (opt & PCRE_STUDY_JIT_COMPILE) {
            পূর্ণসংখ্যা jit, n;

            jit = 0;
            n = pcre_fullinfo(elts[i].regex->code, elts[i].regex->extra,
                              PCRE_INFO_JIT, &jit);

            যদি (n != 0 || jit != 1) {
                ngx_log_error(NGX_LOG_INFO, cycle->log, 0,
                              "JIT compiler does not support pattern: \"%s\"",
                              elts[i].name);
            }
        }
#endif
#endif
    }

    ngx_regex_malloc_done();

    ngx_regex_studies = ফাঁকা;

    ফেরত NGX_OK;
}


স্থির শূন্য *
ngx_regex_create_conf(ngx_cycle_t *cycle)
{
    ngx_regex_conf_t    *rcf;
    ngx_pool_cleanup_t  *cln;

    rcf = ngx_pcalloc(cycle->pool, আকার(ngx_regex_conf_t));
    যদি (rcf == ফাঁকা) {
        ফেরত ফাঁকা;
    }

    rcf->pcre_jit = NGX_CONF_UNSET;

    cln = ngx_pool_cleanup_add(cycle->pool, 0);
    যদি (cln == ফাঁকা) {
        ফেরত ফাঁকা;
    }

    rcf->studies = ngx_list_create(cycle->pool, 8, আকার(ngx_regex_elt_t));
    যদি (rcf->studies == ফাঁকা) {
        ফেরত ফাঁকা;
    }

    cln->handler = ngx_regex_cleanup;
    cln->তথ্য = rcf;

    ngx_regex_studies = rcf->studies;

    ফেরত rcf;
}


স্থির অক্ষর *
ngx_regex_init_conf(ngx_cycle_t *cycle, শূন্য *conf)
{
    ngx_regex_conf_t *rcf = conf;

    ngx_conf_init_মান(rcf->pcre_jit, 0);

    ফেরত NGX_CONF_OK;
}


স্থির অক্ষর *
ngx_regex_pcre_jit(ngx_conf_t *cf, শূন্য *post, শূন্য *তথ্য)
{
    ngx_flag_t  *fp = তথ্য;

    যদি (*fp == 0) {
        ফেরত NGX_CONF_OK;
    }

#যদি (NGX_PCRE2)
    {
    পূর্ণসংখ্যা       r;
    uint32_t  jit;

    jit = 0;
    r = pcre2_config(PCRE2_CONFIG_JIT, &jit);

    যদি (r != 0 || jit != 1) {
        ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                           "PCRE2 library does not support JIT");
        *fp = 0;
    }
    }
#নতুবা_যদি (NGX_HAVE_PCRE_JIT)
    {
    পূর্ণসংখ্যা  jit, r;

    jit = 0;
    r = pcre_config(PCRE_CONFIG_JIT, &jit);

    যদি (r != 0 || jit != 1) {
        ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                           "PCRE library does not support JIT");
        *fp = 0;
    }
    }
#অন্যথা
    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "nginx was built without PCRE JIT support");
    *fp = 0;
#endif

    ফেরত NGX_CONF_OK;
}
