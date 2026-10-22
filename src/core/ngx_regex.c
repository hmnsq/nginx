
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#അന്തര്ഭാവ <ngx_config.h>
#അന്തര്ഭാവ <ngx_core.h>


തരനിർവചനം ഘടന {
    ngx_flag_t   pcre_jit;
    ngx_list_t  *studies;
} ngx_regex_conf_t;


സ്ഥിതം ngx_inline ശൂന്യം ngx_regex_malloc_init(ngx_pool_t *pool);
സ്ഥിതം ngx_inline ശൂന്യം ngx_regex_malloc_done(ശൂന്യം);

#എങ്കിൽ (NGX_PCRE2)
സ്ഥിതം ശൂന്യം * ngx_libc_cdecl ngx_regex_malloc(size_t size, ശൂന്യം *data);
സ്ഥിതം ശൂന്യം ngx_libc_cdecl ngx_regex_free(ശൂന്യം *p, ശൂന്യം *data);
#അല്ലെങ്കിൽ
സ്ഥിതം ശൂന്യം * ngx_libc_cdecl ngx_regex_malloc(size_t size);
സ്ഥിതം ശൂന്യം ngx_libc_cdecl ngx_regex_free(ശൂന്യം *p);
#endif
സ്ഥിതം ശൂന്യം ngx_regex_cleanup(ശൂന്യം *data);

സ്ഥിതം ngx_int_t ngx_regex_module_init(ngx_cycle_t *cycle);

സ്ഥിതം ശൂന്യം *ngx_regex_create_conf(ngx_cycle_t *cycle);
സ്ഥിതം അക്ഷരം *ngx_regex_init_conf(ngx_cycle_t *cycle, ശൂന്യം *conf);

സ്ഥിതം അക്ഷരം *ngx_regex_pcre_jit(ngx_conf_t *cf, ശൂന്യം *post, ശൂന്യം *data);
സ്ഥിതം ngx_conf_post_t  ngx_regex_pcre_jit_post = { ngx_regex_pcre_jit };


സ്ഥിതം ngx_command_t  ngx_regex_commands[] = {

    { ngx_string("pcre_jit"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      0,
      offsetof(ngx_regex_conf_t, pcre_jit),
      &ngx_regex_pcre_jit_post },

      ngx_null_command
};


സ്ഥിതം ngx_core_module_t  ngx_regex_module_ctx = {
    ngx_string("regex"),
    ngx_regex_create_conf,
    ngx_regex_init_conf
};


ngx_module_t  ngx_regex_module = {
    NGX_MODULE_V1,
    &ngx_regex_module_ctx,                 /* module context */
    ngx_regex_commands,                    /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    ശൂന്യ,                                  /* init master */
    ngx_regex_module_init,                 /* init module */
    ശൂന്യ,                                  /* init process */
    ശൂന്യ,                                  /* init thread */
    ശൂന്യ,                                  /* exit thread */
    ശൂന്യ,                                  /* exit process */
    ശൂന്യ,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


സ്ഥിതം ngx_pool_t             *ngx_regex_pool;
സ്ഥിതം ngx_list_t             *ngx_regex_studies;
സ്ഥിതം ngx_uint_t              ngx_regex_direct_alloc;

#എങ്കിൽ (NGX_PCRE2)
സ്ഥിതം pcre2_compile_context  *ngx_regex_compile_context;
സ്ഥിതം pcre2_match_data       *ngx_regex_match_data;
സ്ഥിതം ngx_uint_t              ngx_regex_match_data_size;
#endif


ശൂന്യം
ngx_regex_init(ശൂന്യം)
{
#എങ്കിൽ !(NGX_PCRE2)
    pcre_malloc = ngx_regex_malloc;
    pcre_free = ngx_regex_free;
#endif
}


സ്ഥിതം ngx_inline ശൂന്യം
ngx_regex_malloc_init(ngx_pool_t *pool)
{
    ngx_regex_pool = pool;
    ngx_regex_direct_alloc = (pool == ശൂന്യ) ? 1 : 0;
}


സ്ഥിതം ngx_inline ശൂന്യം
ngx_regex_malloc_done(ശൂന്യം)
{
    ngx_regex_pool = ശൂന്യ;
    ngx_regex_direct_alloc = 0;
}


#എങ്കിൽ (NGX_PCRE2)

ngx_int_t
ngx_regex_compile(ngx_regex_compile_t *rc)
{
    പൂർണ്ണസംഖ്യ                     n, errcode;
    അക്ഷരം                   *p;
    u_char                  errstr[128];
    size_t                  erroff;
    uint32_t                options;
    pcre2_code             *re;
    ngx_regex_elt_t        *elt;
    pcre2_general_context  *gctx;
    pcre2_compile_context  *cctx;

    എങ്കിൽ (ngx_regex_compile_context == ശൂന്യ) {
        /*
         * Allocate a compile context if not yet allocated.  This uses
         * direct allocations from heap, so the result can be cached
         * even at runtime.
         */

        ngx_regex_malloc_init(ശൂന്യ);

        gctx = pcre2_general_context_create(ngx_regex_malloc, ngx_regex_free,
                                            ശൂന്യ);
        എങ്കിൽ (gctx == ശൂന്യ) {
            ngx_regex_malloc_done();
            പോകുക nomem;
        }

        cctx = pcre2_compile_context_create(gctx);
        എങ്കിൽ (cctx == ശൂന്യ) {
            pcre2_general_context_free(gctx);
            ngx_regex_malloc_done();
            പോകുക nomem;
        }

        ngx_regex_compile_context = cctx;

        pcre2_general_context_free(gctx);
        ngx_regex_malloc_done();
    }

    options = 0;

    എങ്കിൽ (rc->options & NGX_REGEX_CASELESS) {
        options |= PCRE2_CASELESS;
    }

    എങ്കിൽ (rc->options & NGX_REGEX_MULTILINE) {
        options |= PCRE2_MULTILINE;
    }

    എങ്കിൽ (rc->options & ~(NGX_REGEX_CASELESS|NGX_REGEX_MULTILINE)) {
        rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                            "regex \"%V\" compilation failed: invalid options",
                            &rc->pattern)
                      - rc->err.data;
        തിരികെ NGX_ERROR;
    }

    ngx_regex_malloc_init(rc->pool);

    re = pcre2_compile(rc->pattern.data, rc->pattern.len, options,
                       &errcode, &erroff, ngx_regex_compile_context);

    /* ensure that there is no current pool */
    ngx_regex_malloc_done();

    എങ്കിൽ (re == ശൂന്യ) {
        pcre2_get_error_message(errcode, errstr, 128);

        എങ്കിൽ ((size_t) erroff == rc->pattern.len) {
            rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                              "pcre2_compile() failed: %s in \"%V\"",
                               errstr, &rc->pattern)
                          - rc->err.data;

        } അല്ലെങ്കിൽ {
            rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                              "pcre2_compile() failed: %s in \"%V\" at \"%s\"",
                               errstr, &rc->pattern, rc->pattern.data + erroff)
                          - rc->err.data;
        }

        തിരികെ NGX_ERROR;
    }

    rc->regex = re;

    /* do not study at runtime */

    എങ്കിൽ (ngx_regex_studies != ശൂന്യ) {
        elt = ngx_list_തള്ളുക(ngx_regex_studies);
        എങ്കിൽ (elt == ശൂന്യ) {
            പോകുക nomem;
        }

        elt->regex = rc->regex;
        elt->name = rc->pattern.data;
    }

    n = pcre2_pattern_info(re, PCRE2_INFO_CAPTURECOUNT, &rc->captures);
    എങ്കിൽ (n < 0) {
        p = "pcre2_pattern_info(\"%V\", PCRE2_INFO_CAPTURECOUNT) failed: %d";
        പോകുക failed;
    }

    എങ്കിൽ (rc->captures == 0) {
        തിരികെ NGX_OK;
    }

    n = pcre2_pattern_info(re, PCRE2_INFO_NAMECOUNT, &rc->named_captures);
    എങ്കിൽ (n < 0) {
        p = "pcre2_pattern_info(\"%V\", PCRE2_INFO_NAMECOUNT) failed: %d";
        പോകുക failed;
    }

    എങ്കിൽ (rc->named_captures == 0) {
        തിരികെ NGX_OK;
    }

    n = pcre2_pattern_info(re, PCRE2_INFO_NAMEENTRYSIZE, &rc->name_size);
    എങ്കിൽ (n < 0) {
        p = "pcre2_pattern_info(\"%V\", PCRE2_INFO_NAMEENTRYSIZE) failed: %d";
        പോകുക failed;
    }

    n = pcre2_pattern_info(re, PCRE2_INFO_NAMETABLE, &rc->names);
    എങ്കിൽ (n < 0) {
        p = "pcre2_pattern_info(\"%V\", PCRE2_INFO_NAMETABLE) failed: %d";
        പോകുക failed;
    }

    തിരികെ NGX_OK;

failed:

    rc->err.len = ngx_snprintf(rc->err.data, rc->err.len, p, &rc->pattern, n)
                  - rc->err.data;
    തിരികെ NGX_ERROR;

nomem:

    rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                               "regex \"%V\" compilation failed: no memory",
                               &rc->pattern)
                  - rc->err.data;
    തിരികെ NGX_ERROR;
}

#അല്ലെങ്കിൽ

ngx_int_t
ngx_regex_compile(ngx_regex_compile_t *rc)
{
    പൂർണ്ണസംഖ്യ               n, erroff;
    അക്ഷരം             *p;
    pcre             *re;
    സ്ഥിരം അക്ഷരം       *errstr;
    ngx_uint_t        options;
    ngx_regex_elt_t  *elt;

    options = 0;

    എങ്കിൽ (rc->options & NGX_REGEX_CASELESS) {
        options |= PCRE_CASELESS;
    }

    എങ്കിൽ (rc->options & NGX_REGEX_MULTILINE) {
        options |= PCRE_MULTILINE;
    }

    എങ്കിൽ (rc->options & ~(NGX_REGEX_CASELESS|NGX_REGEX_MULTILINE)) {
        rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                            "regex \"%V\" compilation failed: invalid options",
                            &rc->pattern)
                      - rc->err.data;
        തിരികെ NGX_ERROR;
    }

    ngx_regex_malloc_init(rc->pool);

    re = pcre_compile((സ്ഥിരം അക്ഷരം *) rc->pattern.data, (പൂർണ്ണസംഖ്യ) options,
                      &errstr, &erroff, ശൂന്യ);

    /* ensure that there is no current pool */
    ngx_regex_malloc_done();

    എങ്കിൽ (re == ശൂന്യ) {
        എങ്കിൽ ((size_t) erroff == rc->pattern.len) {
           rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                              "pcre_compile() failed: %s in \"%V\"",
                               errstr, &rc->pattern)
                         - rc->err.data;

        } അല്ലെങ്കിൽ {
           rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                              "pcre_compile() failed: %s in \"%V\" at \"%s\"",
                               errstr, &rc->pattern, rc->pattern.data + erroff)
                         - rc->err.data;
        }

        തിരികെ NGX_ERROR;
    }

    rc->regex = ngx_pcalloc(rc->pool, വലുപ്പം(ngx_regex_t));
    എങ്കിൽ (rc->regex == ശൂന്യ) {
        പോകുക nomem;
    }

    rc->regex->code = re;

    /* do not study at runtime */

    എങ്കിൽ (ngx_regex_studies != ശൂന്യ) {
        elt = ngx_list_തള്ളുക(ngx_regex_studies);
        എങ്കിൽ (elt == ശൂന്യ) {
            പോകുക nomem;
        }

        elt->regex = rc->regex;
        elt->name = rc->pattern.data;
    }

    n = pcre_fullinfo(re, ശൂന്യ, PCRE_INFO_CAPTURECOUNT, &rc->captures);
    എങ്കിൽ (n < 0) {
        p = "pcre_fullinfo(\"%V\", PCRE_INFO_CAPTURECOUNT) failed: %d";
        പോകുക failed;
    }

    എങ്കിൽ (rc->captures == 0) {
        തിരികെ NGX_OK;
    }

    n = pcre_fullinfo(re, ശൂന്യ, PCRE_INFO_NAMECOUNT, &rc->named_captures);
    എങ്കിൽ (n < 0) {
        p = "pcre_fullinfo(\"%V\", PCRE_INFO_NAMECOUNT) failed: %d";
        പോകുക failed;
    }

    എങ്കിൽ (rc->named_captures == 0) {
        തിരികെ NGX_OK;
    }

    n = pcre_fullinfo(re, ശൂന്യ, PCRE_INFO_NAMEENTRYSIZE, &rc->name_size);
    എങ്കിൽ (n < 0) {
        p = "pcre_fullinfo(\"%V\", PCRE_INFO_NAMEENTRYSIZE) failed: %d";
        പോകുക failed;
    }

    n = pcre_fullinfo(re, ശൂന്യ, PCRE_INFO_NAMETABLE, &rc->names);
    എങ്കിൽ (n < 0) {
        p = "pcre_fullinfo(\"%V\", PCRE_INFO_NAMETABLE) failed: %d";
        പോകുക failed;
    }

    തിരികെ NGX_OK;

failed:

    rc->err.len = ngx_snprintf(rc->err.data, rc->err.len, p, &rc->pattern, n)
                  - rc->err.data;
    തിരികെ NGX_ERROR;

nomem:

    rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                               "regex \"%V\" compilation failed: no memory",
                               &rc->pattern)
                  - rc->err.data;
    തിരികെ NGX_ERROR;
}

#endif


#എങ്കിൽ (NGX_PCRE2)

ngx_int_t
ngx_regex_exec(ngx_regex_t *re, ngx_str_t *s, പൂർണ്ണസംഖ്യ *captures, ngx_uint_t size)
{
    size_t      *ov;
    ngx_int_t    rc;
    ngx_uint_t   n, i;

    /*
     * The pcre2_match() function might allocate memory for backtracking
     * frames, typical allocations are from 40k and above.  So the allocator
     * is configured to do direct allocations from heap during matching.
     */

    ngx_regex_malloc_init(ശൂന്യ);

    എങ്കിൽ (ngx_regex_match_data == ശൂന്യ
        || size > ngx_regex_match_data_size)
    {
        /*
         * Allocate a match data if not yet allocated or smaller than
         * needed.
         */

        എങ്കിൽ (ngx_regex_match_data) {
            pcre2_match_data_free(ngx_regex_match_data);
        }

        ngx_regex_match_data_size = size;
        ngx_regex_match_data = pcre2_match_data_create(size / 3, ശൂന്യ);

        എങ്കിൽ (ngx_regex_match_data == ശൂന്യ) {
            rc = PCRE2_ERROR_NOMEMORY;
            പോകുക failed;
        }
    }

    rc = pcre2_match(re, s->data, s->len, 0, 0, ngx_regex_match_data, ശൂന്യ);

    എങ്കിൽ (rc < 0) {
        പോകുക failed;
    }

    n = pcre2_get_ovector_എണ്ണം(ngx_regex_match_data);
    ov = pcre2_get_ovector_pointer(ngx_regex_match_data);

    എങ്കിൽ (n > size / 3) {
        n = size / 3;
    }

    വേണ്ടി (i = 0; i < n; i++) {
        captures[i * 2] = ov[i * 2];
        captures[i * 2 + 1] = ov[i * 2 + 1];
    }

failed:

    ngx_regex_malloc_done();

    തിരികെ rc;
}

#അല്ലെങ്കിൽ

ngx_int_t
ngx_regex_exec(ngx_regex_t *re, ngx_str_t *s, പൂർണ്ണസംഖ്യ *captures, ngx_uint_t size)
{
    തിരികെ pcre_exec(re->code, re->extra, (സ്ഥിരം അക്ഷരം *) s->data, s->len,
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

    വേണ്ടി (i = 0; i < a->nelts; i++) {

        n = ngx_regex_exec(re[i].regex, s, ശൂന്യ, 0);

        എങ്കിൽ (n == NGX_REGEX_NO_MATCHED) {
            തുടരുക;
        }

        എങ്കിൽ (n < 0) {
            ngx_log_error(NGX_LOG_ALERT, log, 0,
                          ngx_regex_exec_n " failed: %i on \"%V\" using \"%s\"",
                          n, s, re[i].name);
            തിരികെ NGX_ERROR;
        }

        /* match */

        തിരികെ NGX_OK;
    }

    തിരികെ NGX_DECLINED;
}


#എങ്കിൽ (NGX_PCRE2)

സ്ഥിതം ശൂന്യം * ngx_libc_cdecl
ngx_regex_malloc(size_t size, ശൂന്യം *data)
{
    എങ്കിൽ (ngx_regex_pool) {
        തിരികെ ngx_palloc(ngx_regex_pool, size);
    }

    എങ്കിൽ (ngx_regex_direct_alloc) {
        തിരികെ ngx_alloc(size, ngx_cycle->log);
    }

    തിരികെ ശൂന്യ;
}


സ്ഥിതം ശൂന്യം ngx_libc_cdecl
ngx_regex_free(ശൂന്യം *p, ശൂന്യം *data)
{
    എങ്കിൽ (ngx_regex_direct_alloc) {
        ngx_free(p);
    }

    തിരികെ;
}

#അല്ലെങ്കിൽ

സ്ഥിതം ശൂന്യം * ngx_libc_cdecl
ngx_regex_malloc(size_t size)
{
    എങ്കിൽ (ngx_regex_pool) {
        തിരികെ ngx_palloc(ngx_regex_pool, size);
    }

    തിരികെ ശൂന്യ;
}


സ്ഥിതം ശൂന്യം ngx_libc_cdecl
ngx_regex_free(ശൂന്യം *p)
{
    തിരികെ;
}

#endif


സ്ഥിതം ശൂന്യം
ngx_regex_cleanup(ശൂന്യം *data)
{
#എങ്കിൽ (NGX_PCRE2 || NGX_HAVE_PCRE_JIT)
    ngx_regex_conf_t *rcf = data;

    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_regex_elt_t  *elts;

    part = &rcf->studies->part;
    elts = part->elts;

    വേണ്ടി (i = 0; /* void */ ; i++) {

        എങ്കിൽ (i >= part->nelts) {
            എങ്കിൽ (part->next == ശൂന്യ) {
                നിർത്തുക;
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

#എങ്കിൽ (NGX_PCRE2)
        pcre2_code_free(elts[i].regex);
#അല്ലെങ്കിൽ
        എങ്കിൽ (elts[i].regex->extra != ശൂന്യ) {
            pcre_free_study(elts[i].regex->extra);
        }
#endif
    }
#endif

    /*
     * On configuration parsing errors ngx_regex_module_init() will not
     * be called.  Make sure ngx_regex_studies is properly cleared anyway.
     */

    ngx_regex_studies = ശൂന്യ;

#എങ്കിൽ (NGX_PCRE2)

    /*
     * Free compile context and match data.  If needed at runtime by
     * the new cycle, these will be re-allocated.
     */

    ngx_regex_malloc_init(ശൂന്യ);

    എങ്കിൽ (ngx_regex_compile_context) {
        pcre2_compile_context_free(ngx_regex_compile_context);
        ngx_regex_compile_context = ശൂന്യ;
    }

    എങ്കിൽ (ngx_regex_match_data) {
        pcre2_match_data_free(ngx_regex_match_data);
        ngx_regex_match_data = ശൂന്യ;
        ngx_regex_match_data_size = 0;
    }

    ngx_regex_malloc_done();

#endif
}


സ്ഥിതം ngx_int_t
ngx_regex_module_init(ngx_cycle_t *cycle)
{
    പൂർണ്ണസംഖ്യ                opt;
#എങ്കിൽ !(NGX_PCRE2)
    സ്ഥിരം അക്ഷരം        *errstr;
#endif
    ngx_uint_t         i;
    ngx_list_part_t   *part;
    ngx_regex_elt_t   *elts;
    ngx_regex_conf_t  *rcf;

    opt = 0;

    rcf = (ngx_regex_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_regex_module);

#എങ്കിൽ (NGX_PCRE2 || NGX_HAVE_PCRE_JIT)

    എങ്കിൽ (rcf->pcre_jit) {
#എങ്കിൽ (NGX_PCRE2)
        opt = 1;
#അല്ലെങ്കിൽ
        opt = PCRE_STUDY_JIT_COMPILE;
#endif
    }

#endif

    ngx_regex_malloc_init(cycle->pool);

    part = &rcf->studies->part;
    elts = part->elts;

    വേണ്ടി (i = 0; /* void */ ; i++) {

        എങ്കിൽ (i >= part->nelts) {
            എങ്കിൽ (part->next == ശൂന്യ) {
                നിർത്തുക;
            }

            part = part->next;
            elts = part->elts;
            i = 0;
        }

#എങ്കിൽ (NGX_PCRE2)

        എങ്കിൽ (opt) {
            പൂർണ്ണസംഖ്യ  n;

            n = pcre2_jit_compile(elts[i].regex, PCRE2_JIT_COMPLETE);

            എങ്കിൽ (n != 0) {
                ngx_log_error(NGX_LOG_INFO, cycle->log, 0,
                              "pcre2_jit_compile() failed: %d in \"%s\", "
                              "ignored",
                              n, elts[i].name);
            }
        }

#അല്ലെങ്കിൽ

        elts[i].regex->extra = pcre_study(elts[i].regex->code, opt, &errstr);

        എങ്കിൽ (errstr != ശൂന്യ) {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, 0,
                          "pcre_study() failed: %s in \"%s\"",
                          errstr, elts[i].name);
        }

#എങ്കിൽ (NGX_HAVE_PCRE_JIT)
        എങ്കിൽ (opt & PCRE_STUDY_JIT_COMPILE) {
            പൂർണ്ണസംഖ്യ jit, n;

            jit = 0;
            n = pcre_fullinfo(elts[i].regex->code, elts[i].regex->extra,
                              PCRE_INFO_JIT, &jit);

            എങ്കിൽ (n != 0 || jit != 1) {
                ngx_log_error(NGX_LOG_INFO, cycle->log, 0,
                              "JIT compiler does not support pattern: \"%s\"",
                              elts[i].name);
            }
        }
#endif
#endif
    }

    ngx_regex_malloc_done();

    ngx_regex_studies = ശൂന്യ;

    തിരികെ NGX_OK;
}


സ്ഥിതം ശൂന്യം *
ngx_regex_create_conf(ngx_cycle_t *cycle)
{
    ngx_regex_conf_t    *rcf;
    ngx_pool_cleanup_t  *cln;

    rcf = ngx_pcalloc(cycle->pool, വലുപ്പം(ngx_regex_conf_t));
    എങ്കിൽ (rcf == ശൂന്യ) {
        തിരികെ ശൂന്യ;
    }

    rcf->pcre_jit = NGX_CONF_UNSET;

    cln = ngx_pool_cleanup_add(cycle->pool, 0);
    എങ്കിൽ (cln == ശൂന്യ) {
        തിരികെ ശൂന്യ;
    }

    rcf->studies = ngx_list_create(cycle->pool, 8, വലുപ്പം(ngx_regex_elt_t));
    എങ്കിൽ (rcf->studies == ശൂന്യ) {
        തിരികെ ശൂന്യ;
    }

    cln->handler = ngx_regex_cleanup;
    cln->data = rcf;

    ngx_regex_studies = rcf->studies;

    തിരികെ rcf;
}


സ്ഥിതം അക്ഷരം *
ngx_regex_init_conf(ngx_cycle_t *cycle, ശൂന്യം *conf)
{
    ngx_regex_conf_t *rcf = conf;

    ngx_conf_init_മൂല്യം(rcf->pcre_jit, 0);

    തിരികെ NGX_CONF_OK;
}


സ്ഥിതം അക്ഷരം *
ngx_regex_pcre_jit(ngx_conf_t *cf, ശൂന്യം *post, ശൂന്യം *data)
{
    ngx_flag_t  *fp = data;

    എങ്കിൽ (*fp == 0) {
        തിരികെ NGX_CONF_OK;
    }

#എങ്കിൽ (NGX_PCRE2)
    {
    പൂർണ്ണസംഖ്യ       r;
    uint32_t  jit;

    jit = 0;
    r = pcre2_config(PCRE2_CONFIG_JIT, &jit);

    എങ്കിൽ (r != 0 || jit != 1) {
        ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                           "PCRE2 library does not support JIT");
        *fp = 0;
    }
    }
#അല്ലെങ്കിലെങ്കിൽ (NGX_HAVE_PCRE_JIT)
    {
    പൂർണ്ണസംഖ്യ  jit, r;

    jit = 0;
    r = pcre_config(PCRE_CONFIG_JIT, &jit);

    എങ്കിൽ (r != 0 || jit != 1) {
        ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                           "PCRE library does not support JIT");
        *fp = 0;
    }
    }
#അല്ലെങ്കിൽ
    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "nginx was built without PCRE JIT support");
    *fp = 0;
#endif

    തിരികെ NGX_CONF_OK;
}
