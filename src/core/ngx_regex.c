
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


प्रकारनिर्देश संरचना {
    ngx_flag_t   pcre_jit;
    ngx_list_t  *studies;
} ngx_regex_conf_t;


स्थिर ngx_inline शून्य ngx_regex_malloc_init(ngx_pool_t *pool);
स्थिर ngx_inline शून्य ngx_regex_malloc_done(शून्य);

#सचे (NGX_PCRE2)
स्थिर शून्य * ngx_libc_cdecl ngx_regex_malloc(size_t size, शून्य *data);
स्थिर शून्य ngx_libc_cdecl ngx_regex_free(शून्य *p, शून्य *data);
#अञ्ञथा
स्थिर शून्य * ngx_libc_cdecl ngx_regex_malloc(size_t size);
स्थिर शून्य ngx_libc_cdecl ngx_regex_free(शून्य *p);
#endif
स्थिर शून्य ngx_regex_cleanup(शून्य *data);

स्थिर ngx_int_t ngx_regex_module_init(ngx_cycle_t *cycle);

स्थिर शून्य *ngx_regex_create_conf(ngx_cycle_t *cycle);
स्थिर वर्ण *ngx_regex_init_conf(ngx_cycle_t *cycle, शून्य *conf);

स्थिर वर्ण *ngx_regex_pcre_jit(ngx_conf_t *cf, शून्य *post, शून्य *data);
स्थिर ngx_conf_post_t  ngx_regex_pcre_jit_post = { ngx_regex_pcre_jit };


स्थिर ngx_command_t  ngx_regex_commands[] = {

    { ngx_string("pcre_jit"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      0,
      offsetof(ngx_regex_conf_t, pcre_jit),
      &ngx_regex_pcre_jit_post },

      ngx_null_command
};


स्थिर ngx_core_module_t  ngx_regex_module_ctx = {
    ngx_string("regex"),
    ngx_regex_create_conf,
    ngx_regex_init_conf
};


ngx_module_t  ngx_regex_module = {
    NGX_MODULE_V1,
    &ngx_regex_module_ctx,                 /* module context */
    ngx_regex_commands,                    /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    रिक्त,                                  /* init master */
    ngx_regex_module_init,                 /* init module */
    रिक्त,                                  /* init process */
    रिक्त,                                  /* init thread */
    रिक्त,                                  /* exit thread */
    रिक्त,                                  /* exit process */
    रिक्त,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


स्थिर ngx_pool_t             *ngx_regex_pool;
स्थिर ngx_list_t             *ngx_regex_studies;
स्थिर ngx_uint_t              ngx_regex_direct_alloc;

#सचे (NGX_PCRE2)
स्थिर pcre2_compile_context  *ngx_regex_compile_context;
स्थिर pcre2_match_data       *ngx_regex_match_data;
स्थिर ngx_uint_t              ngx_regex_match_data_size;
#endif


शून्य
ngx_regex_init(शून्य)
{
#सचे !(NGX_PCRE2)
    pcre_malloc = ngx_regex_malloc;
    pcre_free = ngx_regex_free;
#endif
}


स्थिर ngx_inline शून्य
ngx_regex_malloc_init(ngx_pool_t *pool)
{
    ngx_regex_pool = pool;
    ngx_regex_direct_alloc = (pool == रिक्त) ? 1 : 0;
}


स्थिर ngx_inline शून्य
ngx_regex_malloc_done(शून्य)
{
    ngx_regex_pool = रिक्त;
    ngx_regex_direct_alloc = 0;
}


#सचे (NGX_PCRE2)

ngx_int_t
ngx_regex_compile(ngx_regex_compile_t *rc)
{
    पूर्णाङ्क                     n, errcode;
    वर्ण                   *p;
    u_char                  errstr[128];
    size_t                  erroff;
    uint32_t                options;
    pcre2_code             *re;
    ngx_regex_elt_t        *elt;
    pcre2_general_context  *gctx;
    pcre2_compile_context  *cctx;

    सचे (ngx_regex_compile_context == रिक्त) {
        /*
         * Allocate a compile context if not yet allocated.  This uses
         * direct allocations from heap, so the result can be cached
         * even at runtime.
         */

        ngx_regex_malloc_init(रिक्त);

        gctx = pcre2_general_context_create(ngx_regex_malloc, ngx_regex_free,
                                            रिक्त);
        सचे (gctx == रिक्त) {
            ngx_regex_malloc_done();
            गच्छ nomem;
        }

        cctx = pcre2_compile_context_create(gctx);
        सचे (cctx == रिक्त) {
            pcre2_general_context_free(gctx);
            ngx_regex_malloc_done();
            गच्छ nomem;
        }

        ngx_regex_compile_context = cctx;

        pcre2_general_context_free(gctx);
        ngx_regex_malloc_done();
    }

    options = 0;

    सचे (rc->options & NGX_REGEX_CASELESS) {
        options |= PCRE2_CASELESS;
    }

    सचे (rc->options & NGX_REGEX_MULTILINE) {
        options |= PCRE2_MULTILINE;
    }

    सचे (rc->options & ~(NGX_REGEX_CASELESS|NGX_REGEX_MULTILINE)) {
        rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                            "regex \"%V\" compilation failed: invalid options",
                            &rc->pattern)
                      - rc->err.data;
        पटिनिवत्त NGX_ERROR;
    }

    ngx_regex_malloc_init(rc->pool);

    re = pcre2_compile(rc->pattern.data, rc->pattern.len, options,
                       &errcode, &erroff, ngx_regex_compile_context);

    /* ensure that there is no current pool */
    ngx_regex_malloc_done();

    सचे (re == रिक्त) {
        pcre2_get_error_message(errcode, errstr, 128);

        सचे ((size_t) erroff == rc->pattern.len) {
            rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                              "pcre2_compile() failed: %s in \"%V\"",
                               errstr, &rc->pattern)
                          - rc->err.data;

        } अञ्ञथा {
            rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                              "pcre2_compile() failed: %s in \"%V\" at \"%s\"",
                               errstr, &rc->pattern, rc->pattern.data + erroff)
                          - rc->err.data;
        }

        पटिनिवत्त NGX_ERROR;
    }

    rc->regex = re;

    /* do not study at runtime */

    सचे (ngx_regex_studies != रिक्त) {
        elt = ngx_list_प्रक्षिप(ngx_regex_studies);
        सचे (elt == रिक्त) {
            गच्छ nomem;
        }

        elt->regex = rc->regex;
        elt->name = rc->pattern.data;
    }

    n = pcre2_pattern_info(re, PCRE2_INFO_CAPTURECOUNT, &rc->captures);
    सचे (n < 0) {
        p = "pcre2_pattern_info(\"%V\", PCRE2_INFO_CAPTURECOUNT) failed: %d";
        गच्छ failed;
    }

    सचे (rc->captures == 0) {
        पटिनिवत्त NGX_OK;
    }

    n = pcre2_pattern_info(re, PCRE2_INFO_NAMECOUNT, &rc->named_captures);
    सचे (n < 0) {
        p = "pcre2_pattern_info(\"%V\", PCRE2_INFO_NAMECOUNT) failed: %d";
        गच्छ failed;
    }

    सचे (rc->named_captures == 0) {
        पटिनिवत्त NGX_OK;
    }

    n = pcre2_pattern_info(re, PCRE2_INFO_NAMEENTRYSIZE, &rc->name_size);
    सचे (n < 0) {
        p = "pcre2_pattern_info(\"%V\", PCRE2_INFO_NAMEENTRYSIZE) failed: %d";
        गच्छ failed;
    }

    n = pcre2_pattern_info(re, PCRE2_INFO_NAMETABLE, &rc->names);
    सचे (n < 0) {
        p = "pcre2_pattern_info(\"%V\", PCRE2_INFO_NAMETABLE) failed: %d";
        गच्छ failed;
    }

    पटिनिवत्त NGX_OK;

failed:

    rc->err.len = ngx_snprintf(rc->err.data, rc->err.len, p, &rc->pattern, n)
                  - rc->err.data;
    पटिनिवत्त NGX_ERROR;

nomem:

    rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                               "regex \"%V\" compilation failed: no memory",
                               &rc->pattern)
                  - rc->err.data;
    पटिनिवत्त NGX_ERROR;
}

#अञ्ञथा

ngx_int_t
ngx_regex_compile(ngx_regex_compile_t *rc)
{
    पूर्णाङ्क               n, erroff;
    वर्ण             *p;
    pcre             *re;
    ध्रुव वर्ण       *errstr;
    ngx_uint_t        options;
    ngx_regex_elt_t  *elt;

    options = 0;

    सचे (rc->options & NGX_REGEX_CASELESS) {
        options |= PCRE_CASELESS;
    }

    सचे (rc->options & NGX_REGEX_MULTILINE) {
        options |= PCRE_MULTILINE;
    }

    सचे (rc->options & ~(NGX_REGEX_CASELESS|NGX_REGEX_MULTILINE)) {
        rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                            "regex \"%V\" compilation failed: invalid options",
                            &rc->pattern)
                      - rc->err.data;
        पटिनिवत्त NGX_ERROR;
    }

    ngx_regex_malloc_init(rc->pool);

    re = pcre_compile((ध्रुव वर्ण *) rc->pattern.data, (पूर्णाङ्क) options,
                      &errstr, &erroff, रिक्त);

    /* ensure that there is no current pool */
    ngx_regex_malloc_done();

    सचे (re == रिक्त) {
        सचे ((size_t) erroff == rc->pattern.len) {
           rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                              "pcre_compile() failed: %s in \"%V\"",
                               errstr, &rc->pattern)
                         - rc->err.data;

        } अञ्ञथा {
           rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                              "pcre_compile() failed: %s in \"%V\" at \"%s\"",
                               errstr, &rc->pattern, rc->pattern.data + erroff)
                         - rc->err.data;
        }

        पटिनिवत्त NGX_ERROR;
    }

    rc->regex = ngx_pcalloc(rc->pool, परिमाण(ngx_regex_t));
    सचे (rc->regex == रिक्त) {
        गच्छ nomem;
    }

    rc->regex->code = re;

    /* do not study at runtime */

    सचे (ngx_regex_studies != रिक्त) {
        elt = ngx_list_प्रक्षिप(ngx_regex_studies);
        सचे (elt == रिक्त) {
            गच्छ nomem;
        }

        elt->regex = rc->regex;
        elt->name = rc->pattern.data;
    }

    n = pcre_fullinfo(re, रिक्त, PCRE_INFO_CAPTURECOUNT, &rc->captures);
    सचे (n < 0) {
        p = "pcre_fullinfo(\"%V\", PCRE_INFO_CAPTURECOUNT) failed: %d";
        गच्छ failed;
    }

    सचे (rc->captures == 0) {
        पटिनिवत्त NGX_OK;
    }

    n = pcre_fullinfo(re, रिक्त, PCRE_INFO_NAMECOUNT, &rc->named_captures);
    सचे (n < 0) {
        p = "pcre_fullinfo(\"%V\", PCRE_INFO_NAMECOUNT) failed: %d";
        गच्छ failed;
    }

    सचे (rc->named_captures == 0) {
        पटिनिवत्त NGX_OK;
    }

    n = pcre_fullinfo(re, रिक्त, PCRE_INFO_NAMEENTRYSIZE, &rc->name_size);
    सचे (n < 0) {
        p = "pcre_fullinfo(\"%V\", PCRE_INFO_NAMEENTRYSIZE) failed: %d";
        गच्छ failed;
    }

    n = pcre_fullinfo(re, रिक्त, PCRE_INFO_NAMETABLE, &rc->names);
    सचे (n < 0) {
        p = "pcre_fullinfo(\"%V\", PCRE_INFO_NAMETABLE) failed: %d";
        गच्छ failed;
    }

    पटिनिवत्त NGX_OK;

failed:

    rc->err.len = ngx_snprintf(rc->err.data, rc->err.len, p, &rc->pattern, n)
                  - rc->err.data;
    पटिनिवत्त NGX_ERROR;

nomem:

    rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                               "regex \"%V\" compilation failed: no memory",
                               &rc->pattern)
                  - rc->err.data;
    पटिनिवत्त NGX_ERROR;
}

#endif


#सचे (NGX_PCRE2)

ngx_int_t
ngx_regex_exec(ngx_regex_t *re, ngx_str_t *s, पूर्णाङ्क *captures, ngx_uint_t size)
{
    size_t      *ov;
    ngx_int_t    rc;
    ngx_uint_t   n, i;

    /*
     * The pcre2_match() function might allocate memory for backtracking
     * frames, typical allocations are from 40k and above.  So the allocator
     * is configured to do direct allocations from heap during matching.
     */

    ngx_regex_malloc_init(रिक्त);

    सचे (ngx_regex_match_data == रिक्त
        || size > ngx_regex_match_data_size)
    {
        /*
         * Allocate a match data if not yet allocated or smaller than
         * needed.
         */

        सचे (ngx_regex_match_data) {
            pcre2_match_data_free(ngx_regex_match_data);
        }

        ngx_regex_match_data_size = size;
        ngx_regex_match_data = pcre2_match_data_create(size / 3, रिक्त);

        सचे (ngx_regex_match_data == रिक्त) {
            rc = PCRE2_ERROR_NOMEMORY;
            गच्छ failed;
        }
    }

    rc = pcre2_match(re, s->data, s->len, 0, 0, ngx_regex_match_data, रिक्त);

    सचे (rc < 0) {
        गच्छ failed;
    }

    n = pcre2_get_ovector_गणनम्(ngx_regex_match_data);
    ov = pcre2_get_ovector_pointer(ngx_regex_match_data);

    सचे (n > size / 3) {
        n = size / 3;
    }

    अत्थाय (i = 0; i < n; i++) {
        captures[i * 2] = ov[i * 2];
        captures[i * 2 + 1] = ov[i * 2 + 1];
    }

failed:

    ngx_regex_malloc_done();

    पटिनिवत्त rc;
}

#अञ्ञथा

ngx_int_t
ngx_regex_exec(ngx_regex_t *re, ngx_str_t *s, पूर्णाङ्क *captures, ngx_uint_t size)
{
    पटिनिवत्त pcre_exec(re->code, re->extra, (ध्रुव वर्ण *) s->data, s->len,
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

    अत्थाय (i = 0; i < a->nelts; i++) {

        n = ngx_regex_exec(re[i].regex, s, रिक्त, 0);

        सचे (n == NGX_REGEX_NO_MATCHED) {
            अनुवर्त;
        }

        सचे (n < 0) {
            ngx_log_error(NGX_LOG_ALERT, log, 0,
                          ngx_regex_exec_n " failed: %i on \"%V\" using \"%s\"",
                          n, s, re[i].name);
            पटिनिवत्त NGX_ERROR;
        }

        /* match */

        पटिनिवत्त NGX_OK;
    }

    पटिनिवत्त NGX_DECLINED;
}


#सचे (NGX_PCRE2)

स्थिर शून्य * ngx_libc_cdecl
ngx_regex_malloc(size_t size, शून्य *data)
{
    सचे (ngx_regex_pool) {
        पटिनिवत्त ngx_palloc(ngx_regex_pool, size);
    }

    सचे (ngx_regex_direct_alloc) {
        पटिनिवत्त ngx_alloc(size, ngx_cycle->log);
    }

    पटिनिवत्त रिक्त;
}


स्थिर शून्य ngx_libc_cdecl
ngx_regex_free(शून्य *p, शून्य *data)
{
    सचे (ngx_regex_direct_alloc) {
        ngx_free(p);
    }

    पटिनिवत्त;
}

#अञ्ञथा

स्थिर शून्य * ngx_libc_cdecl
ngx_regex_malloc(size_t size)
{
    सचे (ngx_regex_pool) {
        पटिनिवत्त ngx_palloc(ngx_regex_pool, size);
    }

    पटिनिवत्त रिक्त;
}


स्थिर शून्य ngx_libc_cdecl
ngx_regex_free(शून्य *p)
{
    पटिनिवत्त;
}

#endif


स्थिर शून्य
ngx_regex_cleanup(शून्य *data)
{
#सचे (NGX_PCRE2 || NGX_HAVE_PCRE_JIT)
    ngx_regex_conf_t *rcf = data;

    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_regex_elt_t  *elts;

    part = &rcf->studies->part;
    elts = part->elts;

    अत्थाय (i = 0; /* void */ ; i++) {

        सचे (i >= part->nelts) {
            सचे (part->next == रिक्त) {
                भङ्ग;
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

#सचे (NGX_PCRE2)
        pcre2_code_free(elts[i].regex);
#अञ्ञथा
        सचे (elts[i].regex->extra != रिक्त) {
            pcre_free_study(elts[i].regex->extra);
        }
#endif
    }
#endif

    /*
     * On configuration parsing errors ngx_regex_module_init() will not
     * be called.  Make sure ngx_regex_studies is properly cleared anyway.
     */

    ngx_regex_studies = रिक्त;

#सचे (NGX_PCRE2)

    /*
     * Free compile context and match data.  If needed at runtime by
     * the new cycle, these will be re-allocated.
     */

    ngx_regex_malloc_init(रिक्त);

    सचे (ngx_regex_compile_context) {
        pcre2_compile_context_free(ngx_regex_compile_context);
        ngx_regex_compile_context = रिक्त;
    }

    सचे (ngx_regex_match_data) {
        pcre2_match_data_free(ngx_regex_match_data);
        ngx_regex_match_data = रिक्त;
        ngx_regex_match_data_size = 0;
    }

    ngx_regex_malloc_done();

#endif
}


स्थिर ngx_int_t
ngx_regex_module_init(ngx_cycle_t *cycle)
{
    पूर्णाङ्क                opt;
#सचे !(NGX_PCRE2)
    ध्रुव वर्ण        *errstr;
#endif
    ngx_uint_t         i;
    ngx_list_part_t   *part;
    ngx_regex_elt_t   *elts;
    ngx_regex_conf_t  *rcf;

    opt = 0;

    rcf = (ngx_regex_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_regex_module);

#सचे (NGX_PCRE2 || NGX_HAVE_PCRE_JIT)

    सचे (rcf->pcre_jit) {
#सचे (NGX_PCRE2)
        opt = 1;
#अञ्ञथा
        opt = PCRE_STUDY_JIT_COMPILE;
#endif
    }

#endif

    ngx_regex_malloc_init(cycle->pool);

    part = &rcf->studies->part;
    elts = part->elts;

    अत्थाय (i = 0; /* void */ ; i++) {

        सचे (i >= part->nelts) {
            सचे (part->next == रिक्त) {
                भङ्ग;
            }

            part = part->next;
            elts = part->elts;
            i = 0;
        }

#सचे (NGX_PCRE2)

        सचे (opt) {
            पूर्णाङ्क  n;

            n = pcre2_jit_compile(elts[i].regex, PCRE2_JIT_COMPLETE);

            सचे (n != 0) {
                ngx_log_error(NGX_LOG_INFO, cycle->log, 0,
                              "pcre2_jit_compile() failed: %d in \"%s\", "
                              "ignored",
                              n, elts[i].name);
            }
        }

#अञ्ञथा

        elts[i].regex->extra = pcre_study(elts[i].regex->code, opt, &errstr);

        सचे (errstr != रिक्त) {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, 0,
                          "pcre_study() failed: %s in \"%s\"",
                          errstr, elts[i].name);
        }

#सचे (NGX_HAVE_PCRE_JIT)
        सचे (opt & PCRE_STUDY_JIT_COMPILE) {
            पूर्णाङ्क jit, n;

            jit = 0;
            n = pcre_fullinfo(elts[i].regex->code, elts[i].regex->extra,
                              PCRE_INFO_JIT, &jit);

            सचे (n != 0 || jit != 1) {
                ngx_log_error(NGX_LOG_INFO, cycle->log, 0,
                              "JIT compiler does not support pattern: \"%s\"",
                              elts[i].name);
            }
        }
#endif
#endif
    }

    ngx_regex_malloc_done();

    ngx_regex_studies = रिक्त;

    पटिनिवत्त NGX_OK;
}


स्थिर शून्य *
ngx_regex_create_conf(ngx_cycle_t *cycle)
{
    ngx_regex_conf_t    *rcf;
    ngx_pool_cleanup_t  *cln;

    rcf = ngx_pcalloc(cycle->pool, परिमाण(ngx_regex_conf_t));
    सचे (rcf == रिक्त) {
        पटिनिवत्त रिक्त;
    }

    rcf->pcre_jit = NGX_CONF_UNSET;

    cln = ngx_pool_cleanup_add(cycle->pool, 0);
    सचे (cln == रिक्त) {
        पटिनिवत्त रिक्त;
    }

    rcf->studies = ngx_list_create(cycle->pool, 8, परिमाण(ngx_regex_elt_t));
    सचे (rcf->studies == रिक्त) {
        पटिनिवत्त रिक्त;
    }

    cln->handler = ngx_regex_cleanup;
    cln->data = rcf;

    ngx_regex_studies = rcf->studies;

    पटिनिवत्त rcf;
}


स्थिर वर्ण *
ngx_regex_init_conf(ngx_cycle_t *cycle, शून्य *conf)
{
    ngx_regex_conf_t *rcf = conf;

    ngx_conf_init_मानम्(rcf->pcre_jit, 0);

    पटिनिवत्त NGX_CONF_OK;
}


स्थिर वर्ण *
ngx_regex_pcre_jit(ngx_conf_t *cf, शून्य *post, शून्य *data)
{
    ngx_flag_t  *fp = data;

    सचे (*fp == 0) {
        पटिनिवत्त NGX_CONF_OK;
    }

#सचे (NGX_PCRE2)
    {
    पूर्णाङ्क       r;
    uint32_t  jit;

    jit = 0;
    r = pcre2_config(PCRE2_CONFIG_JIT, &jit);

    सचे (r != 0 || jit != 1) {
        ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                           "PCRE2 library does not support JIT");
        *fp = 0;
    }
    }
#नोचेद्यदि (NGX_HAVE_PCRE_JIT)
    {
    पूर्णाङ्क  jit, r;

    jit = 0;
    r = pcre_config(PCRE_CONFIG_JIT, &jit);

    सचे (r != 0 || jit != 1) {
        ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                           "PCRE library does not support JIT");
        *fp = 0;
    }
    }
#अञ्ञथा
    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "nginx was built without PCRE JIT support");
    *fp = 0;
#endif

    पटिनिवत्त NGX_CONF_OK;
}
