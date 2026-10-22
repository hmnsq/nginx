
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#অন্তর্ভাব <ngx_config.h>
#অন্তর্ভাব <ngx_core.h>


প্রকারনির্দেশ সংরচনা {
    ngx_flag_t   pcre_jit;
    ngx_list_t  *studies;
} ngx_regex_conf_t;


স্থির ngx_inline শূন্য ngx_regex_malloc_init(ngx_pool_t *pool);
স্থির ngx_inline শূন্য ngx_regex_malloc_done(শূন্য);

#যদি (NGX_PCRE2)
স্থির শূন্য * ngx_libc_cdecl ngx_regex_malloc(size_t size, শূন্য *data);
স্থির শূন্য ngx_libc_cdecl ngx_regex_free(শূন্য *p, শূন্য *data);
#অন্যথা
স্থির শূন্য * ngx_libc_cdecl ngx_regex_malloc(size_t size);
স্থির শূন্য ngx_libc_cdecl ngx_regex_free(শূন্য *p);
#endif
স্থির শূন্য ngx_regex_cleanup(শূন্য *data);

স্থির ngx_int_t ngx_regex_module_init(ngx_cycle_t *cycle);

স্থির শূন্য *ngx_regex_create_conf(ngx_cycle_t *cycle);
স্থির বর্ণ *ngx_regex_init_conf(ngx_cycle_t *cycle, শূন্য *conf);

স্থির বর্ণ *ngx_regex_pcre_jit(ngx_conf_t *cf, শূন্য *post, শূন্য *data);
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
    রিক্ত,                                  /* init master */
    ngx_regex_module_init,                 /* init module */
    রিক্ত,                                  /* init process */
    রিক্ত,                                  /* init thread */
    রিক্ত,                                  /* exit thread */
    রিক্ত,                                  /* exit process */
    রিক্ত,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


স্থির ngx_pool_t             *ngx_regex_pool;
স্থির ngx_list_t             *ngx_regex_studies;
স্থির ngx_uint_t              ngx_regex_direct_alloc;

#যদি (NGX_PCRE2)
স্থির pcre2_compile_context  *ngx_regex_compile_context;
স্থির pcre2_match_data       *ngx_regex_match_data;
স্থির ngx_uint_t              ngx_regex_match_data_size;
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
    ngx_regex_direct_alloc = (pool == রিক্ত) ? 1 : 0;
}


স্থির ngx_inline শূন্য
ngx_regex_malloc_done(শূন্য)
{
    ngx_regex_pool = রিক্ত;
    ngx_regex_direct_alloc = 0;
}


#যদি (NGX_PCRE2)

ngx_int_t
ngx_regex_compile(ngx_regex_compile_t *rc)
{
    পূর্ণাঙ্ক                     n, errcode;
    বর্ণ                   *p;
    u_char                  errstr[128];
    size_t                  erroff;
    uint32_t                options;
    pcre2_code             *re;
    ngx_regex_elt_t        *elt;
    pcre2_general_context  *gctx;
    pcre2_compile_context  *cctx;

    যদি (ngx_regex_compile_context == রিক্ত) {
        /*
         * Allocate a compile context if not yet allocated.  This uses
         * direct allocations from heap, so the result can be cached
         * even at runtime.
         */

        ngx_regex_malloc_init(রিক্ত);

        gctx = pcre2_general_context_create(ngx_regex_malloc, ngx_regex_free,
                                            রিক্ত);
        যদি (gctx == রিক্ত) {
            ngx_regex_malloc_done();
            গচ্ছ nomem;
        }

        cctx = pcre2_compile_context_create(gctx);
        যদি (cctx == রিক্ত) {
            pcre2_general_context_free(gctx);
            ngx_regex_malloc_done();
            গচ্ছ nomem;
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
        rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                            "regex \"%V\" compilation failed: invalid options",
                            &rc->pattern)
                      - rc->err.data;
        প্রত্যাগম NGX_ERROR;
    }

    ngx_regex_malloc_init(rc->pool);

    re = pcre2_compile(rc->pattern.data, rc->pattern.len, options,
                       &errcode, &erroff, ngx_regex_compile_context);

    /* ensure that there is no current pool */
    ngx_regex_malloc_done();

    যদি (re == রিক্ত) {
        pcre2_get_error_message(errcode, errstr, 128);

        যদি ((size_t) erroff == rc->pattern.len) {
            rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                              "pcre2_compile() failed: %s in \"%V\"",
                               errstr, &rc->pattern)
                          - rc->err.data;

        } অন্যথা {
            rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                              "pcre2_compile() failed: %s in \"%V\" at \"%s\"",
                               errstr, &rc->pattern, rc->pattern.data + erroff)
                          - rc->err.data;
        }

        প্রত্যাগম NGX_ERROR;
    }

    rc->regex = re;

    /* do not study at runtime */

    যদি (ngx_regex_studies != রিক্ত) {
        elt = ngx_list_ঠেলো(ngx_regex_studies);
        যদি (elt == রিক্ত) {
            গচ্ছ nomem;
        }

        elt->regex = rc->regex;
        elt->name = rc->pattern.data;
    }

    n = pcre2_pattern_info(re, PCRE2_INFO_CAPTURECOUNT, &rc->captures);
    যদি (n < 0) {
        p = "pcre2_pattern_info(\"%V\", PCRE2_INFO_CAPTURECOUNT) failed: %d";
        গচ্ছ failed;
    }

    যদি (rc->captures == 0) {
        প্রত্যাগম NGX_OK;
    }

    n = pcre2_pattern_info(re, PCRE2_INFO_NAMECOUNT, &rc->named_captures);
    যদি (n < 0) {
        p = "pcre2_pattern_info(\"%V\", PCRE2_INFO_NAMECOUNT) failed: %d";
        গচ্ছ failed;
    }

    যদি (rc->named_captures == 0) {
        প্রত্যাগম NGX_OK;
    }

    n = pcre2_pattern_info(re, PCRE2_INFO_NAMEENTRYSIZE, &rc->name_size);
    যদি (n < 0) {
        p = "pcre2_pattern_info(\"%V\", PCRE2_INFO_NAMEENTRYSIZE) failed: %d";
        গচ্ছ failed;
    }

    n = pcre2_pattern_info(re, PCRE2_INFO_NAMETABLE, &rc->names);
    যদি (n < 0) {
        p = "pcre2_pattern_info(\"%V\", PCRE2_INFO_NAMETABLE) failed: %d";
        গচ্ছ failed;
    }

    প্রত্যাগম NGX_OK;

failed:

    rc->err.len = ngx_snprintf(rc->err.data, rc->err.len, p, &rc->pattern, n)
                  - rc->err.data;
    প্রত্যাগম NGX_ERROR;

nomem:

    rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                               "regex \"%V\" compilation failed: no memory",
                               &rc->pattern)
                  - rc->err.data;
    প্রত্যাগম NGX_ERROR;
}

#অন্যথা

ngx_int_t
ngx_regex_compile(ngx_regex_compile_t *rc)
{
    পূর্ণাঙ্ক               n, erroff;
    বর্ণ             *p;
    pcre             *re;
    ধ্রুব বর্ণ       *errstr;
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
        rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                            "regex \"%V\" compilation failed: invalid options",
                            &rc->pattern)
                      - rc->err.data;
        প্রত্যাগম NGX_ERROR;
    }

    ngx_regex_malloc_init(rc->pool);

    re = pcre_compile((ধ্রুব বর্ণ *) rc->pattern.data, (পূর্ণাঙ্ক) options,
                      &errstr, &erroff, রিক্ত);

    /* ensure that there is no current pool */
    ngx_regex_malloc_done();

    যদি (re == রিক্ত) {
        যদি ((size_t) erroff == rc->pattern.len) {
           rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                              "pcre_compile() failed: %s in \"%V\"",
                               errstr, &rc->pattern)
                         - rc->err.data;

        } অন্যথা {
           rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                              "pcre_compile() failed: %s in \"%V\" at \"%s\"",
                               errstr, &rc->pattern, rc->pattern.data + erroff)
                         - rc->err.data;
        }

        প্রত্যাগম NGX_ERROR;
    }

    rc->regex = ngx_pcalloc(rc->pool, পরিমাণ(ngx_regex_t));
    যদি (rc->regex == রিক্ত) {
        গচ্ছ nomem;
    }

    rc->regex->code = re;

    /* do not study at runtime */

    যদি (ngx_regex_studies != রিক্ত) {
        elt = ngx_list_ঠেলো(ngx_regex_studies);
        যদি (elt == রিক্ত) {
            গচ্ছ nomem;
        }

        elt->regex = rc->regex;
        elt->name = rc->pattern.data;
    }

    n = pcre_fullinfo(re, রিক্ত, PCRE_INFO_CAPTURECOUNT, &rc->captures);
    যদি (n < 0) {
        p = "pcre_fullinfo(\"%V\", PCRE_INFO_CAPTURECOUNT) failed: %d";
        গচ্ছ failed;
    }

    যদি (rc->captures == 0) {
        প্রত্যাগম NGX_OK;
    }

    n = pcre_fullinfo(re, রিক্ত, PCRE_INFO_NAMECOUNT, &rc->named_captures);
    যদি (n < 0) {
        p = "pcre_fullinfo(\"%V\", PCRE_INFO_NAMECOUNT) failed: %d";
        গচ্ছ failed;
    }

    যদি (rc->named_captures == 0) {
        প্রত্যাগম NGX_OK;
    }

    n = pcre_fullinfo(re, রিক্ত, PCRE_INFO_NAMEENTRYSIZE, &rc->name_size);
    যদি (n < 0) {
        p = "pcre_fullinfo(\"%V\", PCRE_INFO_NAMEENTRYSIZE) failed: %d";
        গচ্ছ failed;
    }

    n = pcre_fullinfo(re, রিক্ত, PCRE_INFO_NAMETABLE, &rc->names);
    যদি (n < 0) {
        p = "pcre_fullinfo(\"%V\", PCRE_INFO_NAMETABLE) failed: %d";
        গচ্ছ failed;
    }

    প্রত্যাগম NGX_OK;

failed:

    rc->err.len = ngx_snprintf(rc->err.data, rc->err.len, p, &rc->pattern, n)
                  - rc->err.data;
    প্রত্যাগম NGX_ERROR;

nomem:

    rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                               "regex \"%V\" compilation failed: no memory",
                               &rc->pattern)
                  - rc->err.data;
    প্রত্যাগম NGX_ERROR;
}

#endif


#যদি (NGX_PCRE2)

ngx_int_t
ngx_regex_exec(ngx_regex_t *re, ngx_str_t *s, পূর্ণাঙ্ক *captures, ngx_uint_t size)
{
    size_t      *ov;
    ngx_int_t    rc;
    ngx_uint_t   n, i;

    /*
     * The pcre2_match() function might allocate memory for backtracking
     * frames, typical allocations are from 40k and above.  So the allocator
     * is configured to do direct allocations from heap during matching.
     */

    ngx_regex_malloc_init(রিক্ত);

    যদি (ngx_regex_match_data == রিক্ত
        || size > ngx_regex_match_data_size)
    {
        /*
         * Allocate a match data if not yet allocated or smaller than
         * needed.
         */

        যদি (ngx_regex_match_data) {
            pcre2_match_data_free(ngx_regex_match_data);
        }

        ngx_regex_match_data_size = size;
        ngx_regex_match_data = pcre2_match_data_create(size / 3, রিক্ত);

        যদি (ngx_regex_match_data == রিক্ত) {
            rc = PCRE2_ERROR_NOMEMORY;
            গচ্ছ failed;
        }
    }

    rc = pcre2_match(re, s->data, s->len, 0, 0, ngx_regex_match_data, রিক্ত);

    যদি (rc < 0) {
        গচ্ছ failed;
    }

    n = pcre2_get_ovector_গুনতি(ngx_regex_match_data);
    ov = pcre2_get_ovector_pointer(ngx_regex_match_data);

    যদি (n > size / 3) {
        n = size / 3;
    }

    প্রতি (i = 0; i < n; i++) {
        captures[i * 2] = ov[i * 2];
        captures[i * 2 + 1] = ov[i * 2 + 1];
    }

failed:

    ngx_regex_malloc_done();

    প্রত্যাগম rc;
}

#অন্যথা

ngx_int_t
ngx_regex_exec(ngx_regex_t *re, ngx_str_t *s, পূর্ণাঙ্ক *captures, ngx_uint_t size)
{
    প্রত্যাগম pcre_exec(re->code, re->extra, (ধ্রুব বর্ণ *) s->data, s->len,
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

    প্রতি (i = 0; i < a->nelts; i++) {

        n = ngx_regex_exec(re[i].regex, s, রিক্ত, 0);

        যদি (n == NGX_REGEX_NO_MATCHED) {
            অনুবর্ত;
        }

        যদি (n < 0) {
            ngx_log_error(NGX_LOG_ALERT, log, 0,
                          ngx_regex_exec_n " failed: %i on \"%V\" using \"%s\"",
                          n, s, re[i].name);
            প্রত্যাগম NGX_ERROR;
        }

        /* match */

        প্রত্যাগম NGX_OK;
    }

    প্রত্যাগম NGX_DECLINED;
}


#যদি (NGX_PCRE2)

স্থির শূন্য * ngx_libc_cdecl
ngx_regex_malloc(size_t size, শূন্য *data)
{
    যদি (ngx_regex_pool) {
        প্রত্যাগম ngx_palloc(ngx_regex_pool, size);
    }

    যদি (ngx_regex_direct_alloc) {
        প্রত্যাগম ngx_alloc(size, ngx_cycle->log);
    }

    প্রত্যাগম রিক্ত;
}


স্থির শূন্য ngx_libc_cdecl
ngx_regex_free(শূন্য *p, শূন্য *data)
{
    যদি (ngx_regex_direct_alloc) {
        ngx_free(p);
    }

    প্রত্যাগম;
}

#অন্যথা

স্থির শূন্য * ngx_libc_cdecl
ngx_regex_malloc(size_t size)
{
    যদি (ngx_regex_pool) {
        প্রত্যাগম ngx_palloc(ngx_regex_pool, size);
    }

    প্রত্যাগম রিক্ত;
}


স্থির শূন্য ngx_libc_cdecl
ngx_regex_free(শূন্য *p)
{
    প্রত্যাগম;
}

#endif


স্থির শূন্য
ngx_regex_cleanup(শূন্য *data)
{
#যদি (NGX_PCRE2 || NGX_HAVE_PCRE_JIT)
    ngx_regex_conf_t *rcf = data;

    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_regex_elt_t  *elts;

    part = &rcf->studies->part;
    elts = part->elts;

    প্রতি (i = 0; /* void */ ; i++) {

        যদি (i >= part->nelts) {
            যদি (part->next == রিক্ত) {
                ভঙ্গ;
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

#যদি (NGX_PCRE2)
        pcre2_code_free(elts[i].regex);
#অন্যথা
        যদি (elts[i].regex->extra != রিক্ত) {
            pcre_free_study(elts[i].regex->extra);
        }
#endif
    }
#endif

    /*
     * On configuration parsing errors ngx_regex_module_init() will not
     * be called.  Make sure ngx_regex_studies is properly cleared anyway.
     */

    ngx_regex_studies = রিক্ত;

#যদি (NGX_PCRE2)

    /*
     * Free compile context and match data.  If needed at runtime by
     * the new cycle, these will be re-allocated.
     */

    ngx_regex_malloc_init(রিক্ত);

    যদি (ngx_regex_compile_context) {
        pcre2_compile_context_free(ngx_regex_compile_context);
        ngx_regex_compile_context = রিক্ত;
    }

    যদি (ngx_regex_match_data) {
        pcre2_match_data_free(ngx_regex_match_data);
        ngx_regex_match_data = রিক্ত;
        ngx_regex_match_data_size = 0;
    }

    ngx_regex_malloc_done();

#endif
}


স্থির ngx_int_t
ngx_regex_module_init(ngx_cycle_t *cycle)
{
    পূর্ণাঙ্ক                opt;
#যদি !(NGX_PCRE2)
    ধ্রুব বর্ণ        *errstr;
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

    প্রতি (i = 0; /* void */ ; i++) {

        যদি (i >= part->nelts) {
            যদি (part->next == রিক্ত) {
                ভঙ্গ;
            }

            part = part->next;
            elts = part->elts;
            i = 0;
        }

#যদি (NGX_PCRE2)

        যদি (opt) {
            পূর্ণাঙ্ক  n;

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

        যদি (errstr != রিক্ত) {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, 0,
                          "pcre_study() failed: %s in \"%s\"",
                          errstr, elts[i].name);
        }

#যদি (NGX_HAVE_PCRE_JIT)
        যদি (opt & PCRE_STUDY_JIT_COMPILE) {
            পূর্ণাঙ্ক jit, n;

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

    ngx_regex_studies = রিক্ত;

    প্রত্যাগম NGX_OK;
}


স্থির শূন্য *
ngx_regex_create_conf(ngx_cycle_t *cycle)
{
    ngx_regex_conf_t    *rcf;
    ngx_pool_cleanup_t  *cln;

    rcf = ngx_pcalloc(cycle->pool, পরিমাণ(ngx_regex_conf_t));
    যদি (rcf == রিক্ত) {
        প্রত্যাগম রিক্ত;
    }

    rcf->pcre_jit = NGX_CONF_UNSET;

    cln = ngx_pool_cleanup_add(cycle->pool, 0);
    যদি (cln == রিক্ত) {
        প্রত্যাগম রিক্ত;
    }

    rcf->studies = ngx_list_create(cycle->pool, 8, পরিমাণ(ngx_regex_elt_t));
    যদি (rcf->studies == রিক্ত) {
        প্রত্যাগম রিক্ত;
    }

    cln->handler = ngx_regex_cleanup;
    cln->data = rcf;

    ngx_regex_studies = rcf->studies;

    প্রত্যাগম rcf;
}


স্থির বর্ণ *
ngx_regex_init_conf(ngx_cycle_t *cycle, শূন্য *conf)
{
    ngx_regex_conf_t *rcf = conf;

    ngx_conf_init_মান(rcf->pcre_jit, 0);

    প্রত্যাগম NGX_CONF_OK;
}


স্থির বর্ণ *
ngx_regex_pcre_jit(ngx_conf_t *cf, শূন্য *post, শূন্য *data)
{
    ngx_flag_t  *fp = data;

    যদি (*fp == 0) {
        প্রত্যাগম NGX_CONF_OK;
    }

#যদি (NGX_PCRE2)
    {
    পূর্ণাঙ্ক       r;
    uint32_t  jit;

    jit = 0;
    r = pcre2_config(PCRE2_CONFIG_JIT, &jit);

    যদি (r != 0 || jit != 1) {
        ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                           "PCRE2 library does not support JIT");
        *fp = 0;
    }
    }
#নোচেদ্যদি (NGX_HAVE_PCRE_JIT)
    {
    পূর্ণাঙ্ক  jit, r;

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

    প্রত্যাগম NGX_CONF_OK;
}
