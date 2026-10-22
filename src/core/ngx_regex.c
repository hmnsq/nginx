
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#அந்தர்பாவ <ngx_config.h>
#அந்தர்பாவ <ngx_core.h>


வகைவரை கட்டமைப்பு {
    ngx_flag_t   pcre_jit;
    ngx_list_t  *studies;
} ngx_regex_conf_t;


நிலையான ngx_inline வெறுமை ngx_regex_malloc_init(ngx_pool_t *pool);
நிலையான ngx_inline வெறுமை ngx_regex_malloc_done(வெறுமை);

#எனில் (NGX_PCRE2)
நிலையான வெறுமை * ngx_libc_cdecl ngx_regex_malloc(size_t size, வெறுமை *data);
நிலையான வெறுமை ngx_libc_cdecl ngx_regex_free(வெறுமை *p, வெறுமை *data);
#இல்லையெனில்
நிலையான வெறுமை * ngx_libc_cdecl ngx_regex_malloc(size_t size);
நிலையான வெறுமை ngx_libc_cdecl ngx_regex_free(வெறுமை *p);
#endif
நிலையான வெறுமை ngx_regex_cleanup(வெறுமை *data);

நிலையான ngx_int_t ngx_regex_module_init(ngx_cycle_t *cycle);

நிலையான வெறுமை *ngx_regex_create_conf(ngx_cycle_t *cycle);
நிலையான எழுத்து *ngx_regex_init_conf(ngx_cycle_t *cycle, வெறுமை *conf);

நிலையான எழுத்து *ngx_regex_pcre_jit(ngx_conf_t *cf, வெறுமை *post, வெறுமை *data);
நிலையான ngx_conf_post_t  ngx_regex_pcre_jit_post = { ngx_regex_pcre_jit };


நிலையான ngx_command_t  ngx_regex_commands[] = {

    { ngx_string("pcre_jit"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      0,
      offsetof(ngx_regex_conf_t, pcre_jit),
      &ngx_regex_pcre_jit_post },

      ngx_null_command
};


நிலையான ngx_core_module_t  ngx_regex_module_ctx = {
    ngx_string("regex"),
    ngx_regex_create_conf,
    ngx_regex_init_conf
};


ngx_module_t  ngx_regex_module = {
    NGX_MODULE_V1,
    &ngx_regex_module_ctx,                 /* module context */
    ngx_regex_commands,                    /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    வெற்று,                                  /* init master */
    ngx_regex_module_init,                 /* init module */
    வெற்று,                                  /* init process */
    வெற்று,                                  /* init thread */
    வெற்று,                                  /* exit thread */
    வெற்று,                                  /* exit process */
    வெற்று,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


நிலையான ngx_pool_t             *ngx_regex_pool;
நிலையான ngx_list_t             *ngx_regex_studies;
நிலையான ngx_uint_t              ngx_regex_direct_alloc;

#எனில் (NGX_PCRE2)
நிலையான pcre2_compile_context  *ngx_regex_compile_context;
நிலையான pcre2_match_data       *ngx_regex_match_data;
நிலையான ngx_uint_t              ngx_regex_match_data_size;
#endif


வெறுமை
ngx_regex_init(வெறுமை)
{
#எனில் !(NGX_PCRE2)
    pcre_malloc = ngx_regex_malloc;
    pcre_free = ngx_regex_free;
#endif
}


நிலையான ngx_inline வெறுமை
ngx_regex_malloc_init(ngx_pool_t *pool)
{
    ngx_regex_pool = pool;
    ngx_regex_direct_alloc = (pool == வெற்று) ? 1 : 0;
}


நிலையான ngx_inline வெறுமை
ngx_regex_malloc_done(வெறுமை)
{
    ngx_regex_pool = வெற்று;
    ngx_regex_direct_alloc = 0;
}


#எனில் (NGX_PCRE2)

ngx_int_t
ngx_regex_compile(ngx_regex_compile_t *rc)
{
    முழு                     n, errcode;
    எழுத்து                   *p;
    u_char                  errstr[128];
    size_t                  erroff;
    uint32_t                options;
    pcre2_code             *re;
    ngx_regex_elt_t        *elt;
    pcre2_general_context  *gctx;
    pcre2_compile_context  *cctx;

    எனில் (ngx_regex_compile_context == வெற்று) {
        /*
         * Allocate a compile context if not yet allocated.  This uses
         * direct allocations from heap, so the result can be cached
         * even at runtime.
         */

        ngx_regex_malloc_init(வெற்று);

        gctx = pcre2_general_context_create(ngx_regex_malloc, ngx_regex_free,
                                            வெற்று);
        எனில் (gctx == வெற்று) {
            ngx_regex_malloc_done();
            செல் nomem;
        }

        cctx = pcre2_compile_context_create(gctx);
        எனில் (cctx == வெற்று) {
            pcre2_general_context_free(gctx);
            ngx_regex_malloc_done();
            செல் nomem;
        }

        ngx_regex_compile_context = cctx;

        pcre2_general_context_free(gctx);
        ngx_regex_malloc_done();
    }

    options = 0;

    எனில் (rc->options & NGX_REGEX_CASELESS) {
        options |= PCRE2_CASELESS;
    }

    எனில் (rc->options & NGX_REGEX_MULTILINE) {
        options |= PCRE2_MULTILINE;
    }

    எனில் (rc->options & ~(NGX_REGEX_CASELESS|NGX_REGEX_MULTILINE)) {
        rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                            "regex \"%V\" compilation failed: invalid options",
                            &rc->pattern)
                      - rc->err.data;
        திரும்பு NGX_ERROR;
    }

    ngx_regex_malloc_init(rc->pool);

    re = pcre2_compile(rc->pattern.data, rc->pattern.len, options,
                       &errcode, &erroff, ngx_regex_compile_context);

    /* ensure that there is no current pool */
    ngx_regex_malloc_done();

    எனில் (re == வெற்று) {
        pcre2_get_error_message(errcode, errstr, 128);

        எனில் ((size_t) erroff == rc->pattern.len) {
            rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                              "pcre2_compile() failed: %s in \"%V\"",
                               errstr, &rc->pattern)
                          - rc->err.data;

        } இல்லையெனில் {
            rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                              "pcre2_compile() failed: %s in \"%V\" at \"%s\"",
                               errstr, &rc->pattern, rc->pattern.data + erroff)
                          - rc->err.data;
        }

        திரும்பு NGX_ERROR;
    }

    rc->regex = re;

    /* do not study at runtime */

    எனில் (ngx_regex_studies != வெற்று) {
        elt = ngx_list_தள்ளு(ngx_regex_studies);
        எனில் (elt == வெற்று) {
            செல் nomem;
        }

        elt->regex = rc->regex;
        elt->name = rc->pattern.data;
    }

    n = pcre2_pattern_info(re, PCRE2_INFO_CAPTURECOUNT, &rc->captures);
    எனில் (n < 0) {
        p = "pcre2_pattern_info(\"%V\", PCRE2_INFO_CAPTURECOUNT) failed: %d";
        செல் failed;
    }

    எனில் (rc->captures == 0) {
        திரும்பு NGX_OK;
    }

    n = pcre2_pattern_info(re, PCRE2_INFO_NAMECOUNT, &rc->named_captures);
    எனில் (n < 0) {
        p = "pcre2_pattern_info(\"%V\", PCRE2_INFO_NAMECOUNT) failed: %d";
        செல் failed;
    }

    எனில் (rc->named_captures == 0) {
        திரும்பு NGX_OK;
    }

    n = pcre2_pattern_info(re, PCRE2_INFO_NAMEENTRYSIZE, &rc->name_size);
    எனில் (n < 0) {
        p = "pcre2_pattern_info(\"%V\", PCRE2_INFO_NAMEENTRYSIZE) failed: %d";
        செல் failed;
    }

    n = pcre2_pattern_info(re, PCRE2_INFO_NAMETABLE, &rc->names);
    எனில் (n < 0) {
        p = "pcre2_pattern_info(\"%V\", PCRE2_INFO_NAMETABLE) failed: %d";
        செல் failed;
    }

    திரும்பு NGX_OK;

failed:

    rc->err.len = ngx_snprintf(rc->err.data, rc->err.len, p, &rc->pattern, n)
                  - rc->err.data;
    திரும்பு NGX_ERROR;

nomem:

    rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                               "regex \"%V\" compilation failed: no memory",
                               &rc->pattern)
                  - rc->err.data;
    திரும்பு NGX_ERROR;
}

#இல்லையெனில்

ngx_int_t
ngx_regex_compile(ngx_regex_compile_t *rc)
{
    முழு               n, erroff;
    எழுத்து             *p;
    pcre             *re;
    மாறிலி எழுத்து       *errstr;
    ngx_uint_t        options;
    ngx_regex_elt_t  *elt;

    options = 0;

    எனில் (rc->options & NGX_REGEX_CASELESS) {
        options |= PCRE_CASELESS;
    }

    எனில் (rc->options & NGX_REGEX_MULTILINE) {
        options |= PCRE_MULTILINE;
    }

    எனில் (rc->options & ~(NGX_REGEX_CASELESS|NGX_REGEX_MULTILINE)) {
        rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                            "regex \"%V\" compilation failed: invalid options",
                            &rc->pattern)
                      - rc->err.data;
        திரும்பு NGX_ERROR;
    }

    ngx_regex_malloc_init(rc->pool);

    re = pcre_compile((மாறிலி எழுத்து *) rc->pattern.data, (முழு) options,
                      &errstr, &erroff, வெற்று);

    /* ensure that there is no current pool */
    ngx_regex_malloc_done();

    எனில் (re == வெற்று) {
        எனில் ((size_t) erroff == rc->pattern.len) {
           rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                              "pcre_compile() failed: %s in \"%V\"",
                               errstr, &rc->pattern)
                         - rc->err.data;

        } இல்லையெனில் {
           rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                              "pcre_compile() failed: %s in \"%V\" at \"%s\"",
                               errstr, &rc->pattern, rc->pattern.data + erroff)
                         - rc->err.data;
        }

        திரும்பு NGX_ERROR;
    }

    rc->regex = ngx_pcalloc(rc->pool, அளவு(ngx_regex_t));
    எனில் (rc->regex == வெற்று) {
        செல் nomem;
    }

    rc->regex->code = re;

    /* do not study at runtime */

    எனில் (ngx_regex_studies != வெற்று) {
        elt = ngx_list_தள்ளு(ngx_regex_studies);
        எனில் (elt == வெற்று) {
            செல் nomem;
        }

        elt->regex = rc->regex;
        elt->name = rc->pattern.data;
    }

    n = pcre_fullinfo(re, வெற்று, PCRE_INFO_CAPTURECOUNT, &rc->captures);
    எனில் (n < 0) {
        p = "pcre_fullinfo(\"%V\", PCRE_INFO_CAPTURECOUNT) failed: %d";
        செல் failed;
    }

    எனில் (rc->captures == 0) {
        திரும்பு NGX_OK;
    }

    n = pcre_fullinfo(re, வெற்று, PCRE_INFO_NAMECOUNT, &rc->named_captures);
    எனில் (n < 0) {
        p = "pcre_fullinfo(\"%V\", PCRE_INFO_NAMECOUNT) failed: %d";
        செல் failed;
    }

    எனில் (rc->named_captures == 0) {
        திரும்பு NGX_OK;
    }

    n = pcre_fullinfo(re, வெற்று, PCRE_INFO_NAMEENTRYSIZE, &rc->name_size);
    எனில் (n < 0) {
        p = "pcre_fullinfo(\"%V\", PCRE_INFO_NAMEENTRYSIZE) failed: %d";
        செல் failed;
    }

    n = pcre_fullinfo(re, வெற்று, PCRE_INFO_NAMETABLE, &rc->names);
    எனில் (n < 0) {
        p = "pcre_fullinfo(\"%V\", PCRE_INFO_NAMETABLE) failed: %d";
        செல் failed;
    }

    திரும்பு NGX_OK;

failed:

    rc->err.len = ngx_snprintf(rc->err.data, rc->err.len, p, &rc->pattern, n)
                  - rc->err.data;
    திரும்பு NGX_ERROR;

nomem:

    rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                               "regex \"%V\" compilation failed: no memory",
                               &rc->pattern)
                  - rc->err.data;
    திரும்பு NGX_ERROR;
}

#endif


#எனில் (NGX_PCRE2)

ngx_int_t
ngx_regex_exec(ngx_regex_t *re, ngx_str_t *s, முழு *captures, ngx_uint_t size)
{
    size_t      *ov;
    ngx_int_t    rc;
    ngx_uint_t   n, i;

    /*
     * The pcre2_match() function might allocate memory for backtracking
     * frames, typical allocations are from 40k and above.  So the allocator
     * is configured to do direct allocations from heap during matching.
     */

    ngx_regex_malloc_init(வெற்று);

    எனில் (ngx_regex_match_data == வெற்று
        || size > ngx_regex_match_data_size)
    {
        /*
         * Allocate a match data if not yet allocated or smaller than
         * needed.
         */

        எனில் (ngx_regex_match_data) {
            pcre2_match_data_free(ngx_regex_match_data);
        }

        ngx_regex_match_data_size = size;
        ngx_regex_match_data = pcre2_match_data_create(size / 3, வெற்று);

        எனில் (ngx_regex_match_data == வெற்று) {
            rc = PCRE2_ERROR_NOMEMORY;
            செல் failed;
        }
    }

    rc = pcre2_match(re, s->data, s->len, 0, 0, ngx_regex_match_data, வெற்று);

    எனில் (rc < 0) {
        செல் failed;
    }

    n = pcre2_get_ovector_மொத்தம்(ngx_regex_match_data);
    ov = pcre2_get_ovector_pointer(ngx_regex_match_data);

    எனில் (n > size / 3) {
        n = size / 3;
    }

    க்கு (i = 0; i < n; i++) {
        captures[i * 2] = ov[i * 2];
        captures[i * 2 + 1] = ov[i * 2 + 1];
    }

failed:

    ngx_regex_malloc_done();

    திரும்பு rc;
}

#இல்லையெனில்

ngx_int_t
ngx_regex_exec(ngx_regex_t *re, ngx_str_t *s, முழு *captures, ngx_uint_t size)
{
    திரும்பு pcre_exec(re->code, re->extra, (மாறிலி எழுத்து *) s->data, s->len,
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

    க்கு (i = 0; i < a->nelts; i++) {

        n = ngx_regex_exec(re[i].regex, s, வெற்று, 0);

        எனில் (n == NGX_REGEX_NO_MATCHED) {
            தொடர்;
        }

        எனில் (n < 0) {
            ngx_log_error(NGX_LOG_ALERT, log, 0,
                          ngx_regex_exec_n " failed: %i on \"%V\" using \"%s\"",
                          n, s, re[i].name);
            திரும்பு NGX_ERROR;
        }

        /* match */

        திரும்பு NGX_OK;
    }

    திரும்பு NGX_DECLINED;
}


#எனில் (NGX_PCRE2)

நிலையான வெறுமை * ngx_libc_cdecl
ngx_regex_malloc(size_t size, வெறுமை *data)
{
    எனில் (ngx_regex_pool) {
        திரும்பு ngx_palloc(ngx_regex_pool, size);
    }

    எனில் (ngx_regex_direct_alloc) {
        திரும்பு ngx_alloc(size, ngx_cycle->log);
    }

    திரும்பு வெற்று;
}


நிலையான வெறுமை ngx_libc_cdecl
ngx_regex_free(வெறுமை *p, வெறுமை *data)
{
    எனில் (ngx_regex_direct_alloc) {
        ngx_free(p);
    }

    திரும்பு;
}

#இல்லையெனில்

நிலையான வெறுமை * ngx_libc_cdecl
ngx_regex_malloc(size_t size)
{
    எனில் (ngx_regex_pool) {
        திரும்பு ngx_palloc(ngx_regex_pool, size);
    }

    திரும்பு வெற்று;
}


நிலையான வெறுமை ngx_libc_cdecl
ngx_regex_free(வெறுமை *p)
{
    திரும்பு;
}

#endif


நிலையான வெறுமை
ngx_regex_cleanup(வெறுமை *data)
{
#எனில் (NGX_PCRE2 || NGX_HAVE_PCRE_JIT)
    ngx_regex_conf_t *rcf = data;

    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_regex_elt_t  *elts;

    part = &rcf->studies->part;
    elts = part->elts;

    க்கு (i = 0; /* void */ ; i++) {

        எனில் (i >= part->nelts) {
            எனில் (part->next == வெற்று) {
                நிறுத்து;
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

#எனில் (NGX_PCRE2)
        pcre2_code_free(elts[i].regex);
#இல்லையெனில்
        எனில் (elts[i].regex->extra != வெற்று) {
            pcre_free_study(elts[i].regex->extra);
        }
#endif
    }
#endif

    /*
     * On configuration parsing errors ngx_regex_module_init() will not
     * be called.  Make sure ngx_regex_studies is properly cleared anyway.
     */

    ngx_regex_studies = வெற்று;

#எனில் (NGX_PCRE2)

    /*
     * Free compile context and match data.  If needed at runtime by
     * the new cycle, these will be re-allocated.
     */

    ngx_regex_malloc_init(வெற்று);

    எனில் (ngx_regex_compile_context) {
        pcre2_compile_context_free(ngx_regex_compile_context);
        ngx_regex_compile_context = வெற்று;
    }

    எனில் (ngx_regex_match_data) {
        pcre2_match_data_free(ngx_regex_match_data);
        ngx_regex_match_data = வெற்று;
        ngx_regex_match_data_size = 0;
    }

    ngx_regex_malloc_done();

#endif
}


நிலையான ngx_int_t
ngx_regex_module_init(ngx_cycle_t *cycle)
{
    முழு                opt;
#எனில் !(NGX_PCRE2)
    மாறிலி எழுத்து        *errstr;
#endif
    ngx_uint_t         i;
    ngx_list_part_t   *part;
    ngx_regex_elt_t   *elts;
    ngx_regex_conf_t  *rcf;

    opt = 0;

    rcf = (ngx_regex_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_regex_module);

#எனில் (NGX_PCRE2 || NGX_HAVE_PCRE_JIT)

    எனில் (rcf->pcre_jit) {
#எனில் (NGX_PCRE2)
        opt = 1;
#இல்லையெனில்
        opt = PCRE_STUDY_JIT_COMPILE;
#endif
    }

#endif

    ngx_regex_malloc_init(cycle->pool);

    part = &rcf->studies->part;
    elts = part->elts;

    க்கு (i = 0; /* void */ ; i++) {

        எனில் (i >= part->nelts) {
            எனில் (part->next == வெற்று) {
                நிறுத்து;
            }

            part = part->next;
            elts = part->elts;
            i = 0;
        }

#எனில் (NGX_PCRE2)

        எனில் (opt) {
            முழு  n;

            n = pcre2_jit_compile(elts[i].regex, PCRE2_JIT_COMPLETE);

            எனில் (n != 0) {
                ngx_log_error(NGX_LOG_INFO, cycle->log, 0,
                              "pcre2_jit_compile() failed: %d in \"%s\", "
                              "ignored",
                              n, elts[i].name);
            }
        }

#இல்லையெனில்

        elts[i].regex->extra = pcre_study(elts[i].regex->code, opt, &errstr);

        எனில் (errstr != வெற்று) {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, 0,
                          "pcre_study() failed: %s in \"%s\"",
                          errstr, elts[i].name);
        }

#எனில் (NGX_HAVE_PCRE_JIT)
        எனில் (opt & PCRE_STUDY_JIT_COMPILE) {
            முழு jit, n;

            jit = 0;
            n = pcre_fullinfo(elts[i].regex->code, elts[i].regex->extra,
                              PCRE_INFO_JIT, &jit);

            எனில் (n != 0 || jit != 1) {
                ngx_log_error(NGX_LOG_INFO, cycle->log, 0,
                              "JIT compiler does not support pattern: \"%s\"",
                              elts[i].name);
            }
        }
#endif
#endif
    }

    ngx_regex_malloc_done();

    ngx_regex_studies = வெற்று;

    திரும்பு NGX_OK;
}


நிலையான வெறுமை *
ngx_regex_create_conf(ngx_cycle_t *cycle)
{
    ngx_regex_conf_t    *rcf;
    ngx_pool_cleanup_t  *cln;

    rcf = ngx_pcalloc(cycle->pool, அளவு(ngx_regex_conf_t));
    எனில் (rcf == வெற்று) {
        திரும்பு வெற்று;
    }

    rcf->pcre_jit = NGX_CONF_UNSET;

    cln = ngx_pool_cleanup_add(cycle->pool, 0);
    எனில் (cln == வெற்று) {
        திரும்பு வெற்று;
    }

    rcf->studies = ngx_list_create(cycle->pool, 8, அளவு(ngx_regex_elt_t));
    எனில் (rcf->studies == வெற்று) {
        திரும்பு வெற்று;
    }

    cln->handler = ngx_regex_cleanup;
    cln->data = rcf;

    ngx_regex_studies = rcf->studies;

    திரும்பு rcf;
}


நிலையான எழுத்து *
ngx_regex_init_conf(ngx_cycle_t *cycle, வெறுமை *conf)
{
    ngx_regex_conf_t *rcf = conf;

    ngx_conf_init_மதிப்பு(rcf->pcre_jit, 0);

    திரும்பு NGX_CONF_OK;
}


நிலையான எழுத்து *
ngx_regex_pcre_jit(ngx_conf_t *cf, வெறுமை *post, வெறுமை *data)
{
    ngx_flag_t  *fp = data;

    எனில் (*fp == 0) {
        திரும்பு NGX_CONF_OK;
    }

#எனில் (NGX_PCRE2)
    {
    முழு       r;
    uint32_t  jit;

    jit = 0;
    r = pcre2_config(PCRE2_CONFIG_JIT, &jit);

    எனில் (r != 0 || jit != 1) {
        ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                           "PCRE2 library does not support JIT");
        *fp = 0;
    }
    }
#அல்லதெனில் (NGX_HAVE_PCRE_JIT)
    {
    முழு  jit, r;

    jit = 0;
    r = pcre_config(PCRE_CONFIG_JIT, &jit);

    எனில் (r != 0 || jit != 1) {
        ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                           "PCRE library does not support JIT");
        *fp = 0;
    }
    }
#இல்லையெனில்
    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "nginx was built without PCRE JIT support");
    *fp = 0;
#endif

    திரும்பு NGX_CONF_OK;
}
