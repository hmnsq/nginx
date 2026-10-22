
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#شامل <ngx_config.h>
#شامل <ngx_core.h>


تعریفنوع ساختار {
    ngx_flag_t   pcre_jit;
    ngx_list_t  *studies;
} ngx_regex_conf_t;


ایستا ngx_inline تهی ngx_regex_malloc_init(ngx_pool_t *pool);
ایستا ngx_inline تهی ngx_regex_malloc_done(تهی);

#اگر (NGX_PCRE2)
ایستا تهی * ngx_libc_cdecl ngx_regex_malloc(size_t size, تهی *data);
ایستا تهی ngx_libc_cdecl ngx_regex_free(تهی *p, تهی *data);
#وگرنه
ایستا تهی * ngx_libc_cdecl ngx_regex_malloc(size_t size);
ایستا تهی ngx_libc_cdecl ngx_regex_free(تهی *p);
#endif
ایستا تهی ngx_regex_cleanup(تهی *data);

ایستا ngx_int_t ngx_regex_module_init(ngx_cycle_t *cycle);

ایستا تهی *ngx_regex_create_conf(ngx_cycle_t *cycle);
ایستا نویسه *ngx_regex_init_conf(ngx_cycle_t *cycle, تهی *conf);

ایستا نویسه *ngx_regex_pcre_jit(ngx_conf_t *cf, تهی *post, تهی *data);
ایستا ngx_conf_post_t  ngx_regex_pcre_jit_post = { ngx_regex_pcre_jit };


ایستا ngx_command_t  ngx_regex_commands[] = {

    { ngx_string("pcre_jit"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      0,
      offsetof(ngx_regex_conf_t, pcre_jit),
      &ngx_regex_pcre_jit_post },

      ngx_null_command
};


ایستا ngx_core_module_t  ngx_regex_module_ctx = {
    ngx_string("regex"),
    ngx_regex_create_conf,
    ngx_regex_init_conf
};


ngx_module_t  ngx_regex_module = {
    NGX_MODULE_V1,
    &ngx_regex_module_ctx,                 /* module context */
    ngx_regex_commands,                    /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    پوچ,                                  /* init master */
    ngx_regex_module_init,                 /* init module */
    پوچ,                                  /* init process */
    پوچ,                                  /* init thread */
    پوچ,                                  /* exit thread */
    پوچ,                                  /* exit process */
    پوچ,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


ایستا ngx_pool_t             *ngx_regex_pool;
ایستا ngx_list_t             *ngx_regex_studies;
ایستا ngx_uint_t              ngx_regex_direct_alloc;

#اگر (NGX_PCRE2)
ایستا pcre2_compile_context  *ngx_regex_compile_context;
ایستا pcre2_match_data       *ngx_regex_match_data;
ایستا ngx_uint_t              ngx_regex_match_data_size;
#endif


تهی
ngx_regex_init(تهی)
{
#اگر !(NGX_PCRE2)
    pcre_malloc = ngx_regex_malloc;
    pcre_free = ngx_regex_free;
#endif
}


ایستا ngx_inline تهی
ngx_regex_malloc_init(ngx_pool_t *pool)
{
    ngx_regex_pool = pool;
    ngx_regex_direct_alloc = (pool == پوچ) ? 1 : 0;
}


ایستا ngx_inline تهی
ngx_regex_malloc_done(تهی)
{
    ngx_regex_pool = پوچ;
    ngx_regex_direct_alloc = 0;
}


#اگر (NGX_PCRE2)

ngx_int_t
ngx_regex_compile(ngx_regex_compile_t *rc)
{
    صحیح                     n, errcode;
    نویسه                   *p;
    u_char                  errstr[128];
    size_t                  erroff;
    uint32_t                options;
    pcre2_code             *re;
    ngx_regex_elt_t        *elt;
    pcre2_general_context  *gctx;
    pcre2_compile_context  *cctx;

    اگر (ngx_regex_compile_context == پوچ) {
        /*
         * Allocate a compile context if not yet allocated.  This uses
         * direct allocations from heap, so the result can be cached
         * even at runtime.
         */

        ngx_regex_malloc_init(پوچ);

        gctx = pcre2_general_context_create(ngx_regex_malloc, ngx_regex_free,
                                            پوچ);
        اگر (gctx == پوچ) {
            ngx_regex_malloc_done();
            برو nomem;
        }

        cctx = pcre2_compile_context_create(gctx);
        اگر (cctx == پوچ) {
            pcre2_general_context_free(gctx);
            ngx_regex_malloc_done();
            برو nomem;
        }

        ngx_regex_compile_context = cctx;

        pcre2_general_context_free(gctx);
        ngx_regex_malloc_done();
    }

    options = 0;

    اگر (rc->options & NGX_REGEX_CASELESS) {
        options |= PCRE2_CASELESS;
    }

    اگر (rc->options & NGX_REGEX_MULTILINE) {
        options |= PCRE2_MULTILINE;
    }

    اگر (rc->options & ~(NGX_REGEX_CASELESS|NGX_REGEX_MULTILINE)) {
        rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                            "regex \"%V\" compilation failed: invalid options",
                            &rc->pattern)
                      - rc->err.data;
        بازگشت NGX_ERROR;
    }

    ngx_regex_malloc_init(rc->pool);

    re = pcre2_compile(rc->pattern.data, rc->pattern.len, options,
                       &errcode, &erroff, ngx_regex_compile_context);

    /* ensure that there is no current pool */
    ngx_regex_malloc_done();

    اگر (re == پوچ) {
        pcre2_get_error_message(errcode, errstr, 128);

        اگر ((size_t) erroff == rc->pattern.len) {
            rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                              "pcre2_compile() failed: %s in \"%V\"",
                               errstr, &rc->pattern)
                          - rc->err.data;

        } وگرنه {
            rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                              "pcre2_compile() failed: %s in \"%V\" at \"%s\"",
                               errstr, &rc->pattern, rc->pattern.data + erroff)
                          - rc->err.data;
        }

        بازگشت NGX_ERROR;
    }

    rc->regex = re;

    /* do not study at runtime */

    اگر (ngx_regex_studies != پوچ) {
        elt = ngx_list_افزودن(ngx_regex_studies);
        اگر (elt == پوچ) {
            برو nomem;
        }

        elt->regex = rc->regex;
        elt->name = rc->pattern.data;
    }

    n = pcre2_pattern_info(re, PCRE2_INFO_CAPTURECOUNT, &rc->captures);
    اگر (n < 0) {
        p = "pcre2_pattern_info(\"%V\", PCRE2_INFO_CAPTURECOUNT) failed: %d";
        برو failed;
    }

    اگر (rc->captures == 0) {
        بازگشت NGX_OK;
    }

    n = pcre2_pattern_info(re, PCRE2_INFO_NAMECOUNT, &rc->named_captures);
    اگر (n < 0) {
        p = "pcre2_pattern_info(\"%V\", PCRE2_INFO_NAMECOUNT) failed: %d";
        برو failed;
    }

    اگر (rc->named_captures == 0) {
        بازگشت NGX_OK;
    }

    n = pcre2_pattern_info(re, PCRE2_INFO_NAMEENTRYSIZE, &rc->name_size);
    اگر (n < 0) {
        p = "pcre2_pattern_info(\"%V\", PCRE2_INFO_NAMEENTRYSIZE) failed: %d";
        برو failed;
    }

    n = pcre2_pattern_info(re, PCRE2_INFO_NAMETABLE, &rc->names);
    اگر (n < 0) {
        p = "pcre2_pattern_info(\"%V\", PCRE2_INFO_NAMETABLE) failed: %d";
        برو failed;
    }

    بازگشت NGX_OK;

failed:

    rc->err.len = ngx_snprintf(rc->err.data, rc->err.len, p, &rc->pattern, n)
                  - rc->err.data;
    بازگشت NGX_ERROR;

nomem:

    rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                               "regex \"%V\" compilation failed: no memory",
                               &rc->pattern)
                  - rc->err.data;
    بازگشت NGX_ERROR;
}

#وگرنه

ngx_int_t
ngx_regex_compile(ngx_regex_compile_t *rc)
{
    صحیح               n, erroff;
    نویسه             *p;
    pcre             *re;
    ثابت نویسه       *errstr;
    ngx_uint_t        options;
    ngx_regex_elt_t  *elt;

    options = 0;

    اگر (rc->options & NGX_REGEX_CASELESS) {
        options |= PCRE_CASELESS;
    }

    اگر (rc->options & NGX_REGEX_MULTILINE) {
        options |= PCRE_MULTILINE;
    }

    اگر (rc->options & ~(NGX_REGEX_CASELESS|NGX_REGEX_MULTILINE)) {
        rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                            "regex \"%V\" compilation failed: invalid options",
                            &rc->pattern)
                      - rc->err.data;
        بازگشت NGX_ERROR;
    }

    ngx_regex_malloc_init(rc->pool);

    re = pcre_compile((ثابت نویسه *) rc->pattern.data, (صحیح) options,
                      &errstr, &erroff, پوچ);

    /* ensure that there is no current pool */
    ngx_regex_malloc_done();

    اگر (re == پوچ) {
        اگر ((size_t) erroff == rc->pattern.len) {
           rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                              "pcre_compile() failed: %s in \"%V\"",
                               errstr, &rc->pattern)
                         - rc->err.data;

        } وگرنه {
           rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                              "pcre_compile() failed: %s in \"%V\" at \"%s\"",
                               errstr, &rc->pattern, rc->pattern.data + erroff)
                         - rc->err.data;
        }

        بازگشت NGX_ERROR;
    }

    rc->regex = ngx_pcalloc(rc->pool, اندازه(ngx_regex_t));
    اگر (rc->regex == پوچ) {
        برو nomem;
    }

    rc->regex->code = re;

    /* do not study at runtime */

    اگر (ngx_regex_studies != پوچ) {
        elt = ngx_list_افزودن(ngx_regex_studies);
        اگر (elt == پوچ) {
            برو nomem;
        }

        elt->regex = rc->regex;
        elt->name = rc->pattern.data;
    }

    n = pcre_fullinfo(re, پوچ, PCRE_INFO_CAPTURECOUNT, &rc->captures);
    اگر (n < 0) {
        p = "pcre_fullinfo(\"%V\", PCRE_INFO_CAPTURECOUNT) failed: %d";
        برو failed;
    }

    اگر (rc->captures == 0) {
        بازگشت NGX_OK;
    }

    n = pcre_fullinfo(re, پوچ, PCRE_INFO_NAMECOUNT, &rc->named_captures);
    اگر (n < 0) {
        p = "pcre_fullinfo(\"%V\", PCRE_INFO_NAMECOUNT) failed: %d";
        برو failed;
    }

    اگر (rc->named_captures == 0) {
        بازگشت NGX_OK;
    }

    n = pcre_fullinfo(re, پوچ, PCRE_INFO_NAMEENTRYSIZE, &rc->name_size);
    اگر (n < 0) {
        p = "pcre_fullinfo(\"%V\", PCRE_INFO_NAMEENTRYSIZE) failed: %d";
        برو failed;
    }

    n = pcre_fullinfo(re, پوچ, PCRE_INFO_NAMETABLE, &rc->names);
    اگر (n < 0) {
        p = "pcre_fullinfo(\"%V\", PCRE_INFO_NAMETABLE) failed: %d";
        برو failed;
    }

    بازگشت NGX_OK;

failed:

    rc->err.len = ngx_snprintf(rc->err.data, rc->err.len, p, &rc->pattern, n)
                  - rc->err.data;
    بازگشت NGX_ERROR;

nomem:

    rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                               "regex \"%V\" compilation failed: no memory",
                               &rc->pattern)
                  - rc->err.data;
    بازگشت NGX_ERROR;
}

#endif


#اگر (NGX_PCRE2)

ngx_int_t
ngx_regex_exec(ngx_regex_t *re, ngx_str_t *s, صحیح *captures, ngx_uint_t size)
{
    size_t      *ov;
    ngx_int_t    rc;
    ngx_uint_t   n, i;

    /*
     * The pcre2_match() function might allocate memory for backtracking
     * frames, typical allocations are from 40k and above.  So the allocator
     * is configured to do direct allocations from heap during matching.
     */

    ngx_regex_malloc_init(پوچ);

    اگر (ngx_regex_match_data == پوچ
        || size > ngx_regex_match_data_size)
    {
        /*
         * Allocate a match data if not yet allocated or smaller than
         * needed.
         */

        اگر (ngx_regex_match_data) {
            pcre2_match_data_free(ngx_regex_match_data);
        }

        ngx_regex_match_data_size = size;
        ngx_regex_match_data = pcre2_match_data_create(size / 3, پوچ);

        اگر (ngx_regex_match_data == پوچ) {
            rc = PCRE2_ERROR_NOMEMORY;
            برو failed;
        }
    }

    rc = pcre2_match(re, s->data, s->len, 0, 0, ngx_regex_match_data, پوچ);

    اگر (rc < 0) {
        برو failed;
    }

    n = pcre2_get_ovector_شمار(ngx_regex_match_data);
    ov = pcre2_get_ovector_pointer(ngx_regex_match_data);

    اگر (n > size / 3) {
        n = size / 3;
    }

    برای (i = 0; i < n; i++) {
        captures[i * 2] = ov[i * 2];
        captures[i * 2 + 1] = ov[i * 2 + 1];
    }

failed:

    ngx_regex_malloc_done();

    بازگشت rc;
}

#وگرنه

ngx_int_t
ngx_regex_exec(ngx_regex_t *re, ngx_str_t *s, صحیح *captures, ngx_uint_t size)
{
    بازگشت pcre_exec(re->code, re->extra, (ثابت نویسه *) s->data, s->len,
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

    برای (i = 0; i < a->nelts; i++) {

        n = ngx_regex_exec(re[i].regex, s, پوچ, 0);

        اگر (n == NGX_REGEX_NO_MATCHED) {
            ادامه;
        }

        اگر (n < 0) {
            ngx_log_error(NGX_LOG_ALERT, log, 0,
                          ngx_regex_exec_n " failed: %i on \"%V\" using \"%s\"",
                          n, s, re[i].name);
            بازگشت NGX_ERROR;
        }

        /* match */

        بازگشت NGX_OK;
    }

    بازگشت NGX_DECLINED;
}


#اگر (NGX_PCRE2)

ایستا تهی * ngx_libc_cdecl
ngx_regex_malloc(size_t size, تهی *data)
{
    اگر (ngx_regex_pool) {
        بازگشت ngx_palloc(ngx_regex_pool, size);
    }

    اگر (ngx_regex_direct_alloc) {
        بازگشت ngx_alloc(size, ngx_cycle->log);
    }

    بازگشت پوچ;
}


ایستا تهی ngx_libc_cdecl
ngx_regex_free(تهی *p, تهی *data)
{
    اگر (ngx_regex_direct_alloc) {
        ngx_free(p);
    }

    بازگشت;
}

#وگرنه

ایستا تهی * ngx_libc_cdecl
ngx_regex_malloc(size_t size)
{
    اگر (ngx_regex_pool) {
        بازگشت ngx_palloc(ngx_regex_pool, size);
    }

    بازگشت پوچ;
}


ایستا تهی ngx_libc_cdecl
ngx_regex_free(تهی *p)
{
    بازگشت;
}

#endif


ایستا تهی
ngx_regex_cleanup(تهی *data)
{
#اگر (NGX_PCRE2 || NGX_HAVE_PCRE_JIT)
    ngx_regex_conf_t *rcf = data;

    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_regex_elt_t  *elts;

    part = &rcf->studies->part;
    elts = part->elts;

    برای (i = 0; /* void */ ; i++) {

        اگر (i >= part->nelts) {
            اگر (part->next == پوچ) {
                بشکن;
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

#اگر (NGX_PCRE2)
        pcre2_code_free(elts[i].regex);
#وگرنه
        اگر (elts[i].regex->extra != پوچ) {
            pcre_free_study(elts[i].regex->extra);
        }
#endif
    }
#endif

    /*
     * On configuration parsing errors ngx_regex_module_init() will not
     * be called.  Make sure ngx_regex_studies is properly cleared anyway.
     */

    ngx_regex_studies = پوچ;

#اگر (NGX_PCRE2)

    /*
     * Free compile context and match data.  If needed at runtime by
     * the new cycle, these will be re-allocated.
     */

    ngx_regex_malloc_init(پوچ);

    اگر (ngx_regex_compile_context) {
        pcre2_compile_context_free(ngx_regex_compile_context);
        ngx_regex_compile_context = پوچ;
    }

    اگر (ngx_regex_match_data) {
        pcre2_match_data_free(ngx_regex_match_data);
        ngx_regex_match_data = پوچ;
        ngx_regex_match_data_size = 0;
    }

    ngx_regex_malloc_done();

#endif
}


ایستا ngx_int_t
ngx_regex_module_init(ngx_cycle_t *cycle)
{
    صحیح                opt;
#اگر !(NGX_PCRE2)
    ثابت نویسه        *errstr;
#endif
    ngx_uint_t         i;
    ngx_list_part_t   *part;
    ngx_regex_elt_t   *elts;
    ngx_regex_conf_t  *rcf;

    opt = 0;

    rcf = (ngx_regex_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_regex_module);

#اگر (NGX_PCRE2 || NGX_HAVE_PCRE_JIT)

    اگر (rcf->pcre_jit) {
#اگر (NGX_PCRE2)
        opt = 1;
#وگرنه
        opt = PCRE_STUDY_JIT_COMPILE;
#endif
    }

#endif

    ngx_regex_malloc_init(cycle->pool);

    part = &rcf->studies->part;
    elts = part->elts;

    برای (i = 0; /* void */ ; i++) {

        اگر (i >= part->nelts) {
            اگر (part->next == پوچ) {
                بشکن;
            }

            part = part->next;
            elts = part->elts;
            i = 0;
        }

#اگر (NGX_PCRE2)

        اگر (opt) {
            صحیح  n;

            n = pcre2_jit_compile(elts[i].regex, PCRE2_JIT_COMPLETE);

            اگر (n != 0) {
                ngx_log_error(NGX_LOG_INFO, cycle->log, 0,
                              "pcre2_jit_compile() failed: %d in \"%s\", "
                              "ignored",
                              n, elts[i].name);
            }
        }

#وگرنه

        elts[i].regex->extra = pcre_study(elts[i].regex->code, opt, &errstr);

        اگر (errstr != پوچ) {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, 0,
                          "pcre_study() failed: %s in \"%s\"",
                          errstr, elts[i].name);
        }

#اگر (NGX_HAVE_PCRE_JIT)
        اگر (opt & PCRE_STUDY_JIT_COMPILE) {
            صحیح jit, n;

            jit = 0;
            n = pcre_fullinfo(elts[i].regex->code, elts[i].regex->extra,
                              PCRE_INFO_JIT, &jit);

            اگر (n != 0 || jit != 1) {
                ngx_log_error(NGX_LOG_INFO, cycle->log, 0,
                              "JIT compiler does not support pattern: \"%s\"",
                              elts[i].name);
            }
        }
#endif
#endif
    }

    ngx_regex_malloc_done();

    ngx_regex_studies = پوچ;

    بازگشت NGX_OK;
}


ایستا تهی *
ngx_regex_create_conf(ngx_cycle_t *cycle)
{
    ngx_regex_conf_t    *rcf;
    ngx_pool_cleanup_t  *cln;

    rcf = ngx_pcalloc(cycle->pool, اندازه(ngx_regex_conf_t));
    اگر (rcf == پوچ) {
        بازگشت پوچ;
    }

    rcf->pcre_jit = NGX_CONF_UNSET;

    cln = ngx_pool_cleanup_add(cycle->pool, 0);
    اگر (cln == پوچ) {
        بازگشت پوچ;
    }

    rcf->studies = ngx_list_create(cycle->pool, 8, اندازه(ngx_regex_elt_t));
    اگر (rcf->studies == پوچ) {
        بازگشت پوچ;
    }

    cln->handler = ngx_regex_cleanup;
    cln->data = rcf;

    ngx_regex_studies = rcf->studies;

    بازگشت rcf;
}


ایستا نویسه *
ngx_regex_init_conf(ngx_cycle_t *cycle, تهی *conf)
{
    ngx_regex_conf_t *rcf = conf;

    ngx_conf_init_مقدار(rcf->pcre_jit, 0);

    بازگشت NGX_CONF_OK;
}


ایستا نویسه *
ngx_regex_pcre_jit(ngx_conf_t *cf, تهی *post, تهی *data)
{
    ngx_flag_t  *fp = data;

    اگر (*fp == 0) {
        بازگشت NGX_CONF_OK;
    }

#اگر (NGX_PCRE2)
    {
    صحیح       r;
    uint32_t  jit;

    jit = 0;
    r = pcre2_config(PCRE2_CONFIG_JIT, &jit);

    اگر (r != 0 || jit != 1) {
        ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                           "PCRE2 library does not support JIT");
        *fp = 0;
    }
    }
#وگرنهاگر (NGX_HAVE_PCRE_JIT)
    {
    صحیح  jit, r;

    jit = 0;
    r = pcre_config(PCRE_CONFIG_JIT, &jit);

    اگر (r != 0 || jit != 1) {
        ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                           "PCRE library does not support JIT");
        *fp = 0;
    }
    }
#وگرنه
    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "nginx was built without PCRE JIT support");
    *fp = 0;
#endif

    بازگشت NGX_CONF_OK;
}
