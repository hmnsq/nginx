
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ଅନ୍ତର୍ଭାଵ <ngx_config.h>
#ଅନ୍ତର୍ଭାଵ <ngx_core.h>


ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ngx_flag_t   pcre_jit;
    ngx_list_t  *studies;
} ngx_regex_conf_t;


ସ୍ଥିର ngx_inline ଶୂନ୍ୟ ngx_regex_malloc_init(ngx_pool_t *pool);
ସ୍ଥିର ngx_inline ଶୂନ୍ୟ ngx_regex_malloc_done(ଶୂନ୍ୟ);

#ଯଦି (NGX_PCRE2)
ସ୍ଥିର ଶୂନ୍ୟ * ngx_libc_cdecl ngx_regex_malloc(size_t size, ଶୂନ୍ୟ *data);
ସ୍ଥିର ଶୂନ୍ୟ ngx_libc_cdecl ngx_regex_free(ଶୂନ୍ୟ *p, ଶୂନ୍ୟ *data);
#ଅନ୍ୟଥା
ସ୍ଥିର ଶୂନ୍ୟ * ngx_libc_cdecl ngx_regex_malloc(size_t size);
ସ୍ଥିର ଶୂନ୍ୟ ngx_libc_cdecl ngx_regex_free(ଶୂନ୍ୟ *p);
#endif
ସ୍ଥିର ଶୂନ୍ୟ ngx_regex_cleanup(ଶୂନ୍ୟ *data);

ସ୍ଥିର ngx_int_t ngx_regex_module_init(ngx_cycle_t *cycle);

ସ୍ଥିର ଶୂନ୍ୟ *ngx_regex_create_conf(ngx_cycle_t *cycle);
ସ୍ଥିର ଅକ୍ଷର *ngx_regex_init_conf(ngx_cycle_t *cycle, ଶୂନ୍ୟ *conf);

ସ୍ଥିର ଅକ୍ଷର *ngx_regex_pcre_jit(ngx_conf_t *cf, ଶୂନ୍ୟ *post, ଶୂନ୍ୟ *data);
ସ୍ଥିର ngx_conf_post_t  ngx_regex_pcre_jit_post = { ngx_regex_pcre_jit };


ସ୍ଥିର ngx_command_t  ngx_regex_commands[] = {

    { ngx_string("pcre_jit"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      0,
      offsetof(ngx_regex_conf_t, pcre_jit),
      &ngx_regex_pcre_jit_post },

      ngx_null_command
};


ସ୍ଥିର ngx_core_module_t  ngx_regex_module_ctx = {
    ngx_string("regex"),
    ngx_regex_create_conf,
    ngx_regex_init_conf
};


ngx_module_t  ngx_regex_module = {
    NGX_MODULE_V1,
    &ngx_regex_module_ctx,                 /* module context */
    ngx_regex_commands,                    /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    ଖାଲି,                                  /* init master */
    ngx_regex_module_init,                 /* init module */
    ଖାଲି,                                  /* init process */
    ଖାଲି,                                  /* init thread */
    ଖାଲି,                                  /* exit thread */
    ଖାଲି,                                  /* exit process */
    ଖାଲି,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


ସ୍ଥିର ngx_pool_t             *ngx_regex_pool;
ସ୍ଥିର ngx_list_t             *ngx_regex_studies;
ସ୍ଥିର ngx_uint_t              ngx_regex_direct_alloc;

#ଯଦି (NGX_PCRE2)
ସ୍ଥିର pcre2_compile_context  *ngx_regex_compile_context;
ସ୍ଥିର pcre2_match_data       *ngx_regex_match_data;
ସ୍ଥିର ngx_uint_t              ngx_regex_match_data_size;
#endif


ଶୂନ୍ୟ
ngx_regex_init(ଶୂନ୍ୟ)
{
#ଯଦି !(NGX_PCRE2)
    pcre_malloc = ngx_regex_malloc;
    pcre_free = ngx_regex_free;
#endif
}


ସ୍ଥିର ngx_inline ଶୂନ୍ୟ
ngx_regex_malloc_init(ngx_pool_t *pool)
{
    ngx_regex_pool = pool;
    ngx_regex_direct_alloc = (pool == ଖାଲି) ? 1 : 0;
}


ସ୍ଥିର ngx_inline ଶୂନ୍ୟ
ngx_regex_malloc_done(ଶୂନ୍ୟ)
{
    ngx_regex_pool = ଖାଲି;
    ngx_regex_direct_alloc = 0;
}


#ଯଦି (NGX_PCRE2)

ngx_int_t
ngx_regex_compile(ngx_regex_compile_t *rc)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା                     n, errcode;
    ଅକ୍ଷର                   *p;
    u_char                  errstr[128];
    size_t                  erroff;
    uint32_t                options;
    pcre2_code             *re;
    ngx_regex_elt_t        *elt;
    pcre2_general_context  *gctx;
    pcre2_compile_context  *cctx;

    ଯଦି (ngx_regex_compile_context == ଖାଲି) {
        /*
         * Allocate a compile context if not yet allocated.  This uses
         * direct allocations from heap, so the result can be cached
         * even at runtime.
         */

        ngx_regex_malloc_init(ଖାଲି);

        gctx = pcre2_general_context_create(ngx_regex_malloc, ngx_regex_free,
                                            ଖାଲି);
        ଯଦି (gctx == ଖାଲି) {
            ngx_regex_malloc_done();
            ଯାଅ nomem;
        }

        cctx = pcre2_compile_context_create(gctx);
        ଯଦି (cctx == ଖାଲି) {
            pcre2_general_context_free(gctx);
            ngx_regex_malloc_done();
            ଯାଅ nomem;
        }

        ngx_regex_compile_context = cctx;

        pcre2_general_context_free(gctx);
        ngx_regex_malloc_done();
    }

    options = 0;

    ଯଦି (rc->options & NGX_REGEX_CASELESS) {
        options |= PCRE2_CASELESS;
    }

    ଯଦି (rc->options & NGX_REGEX_MULTILINE) {
        options |= PCRE2_MULTILINE;
    }

    ଯଦି (rc->options & ~(NGX_REGEX_CASELESS|NGX_REGEX_MULTILINE)) {
        rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                            "regex \"%V\" compilation failed: invalid options",
                            &rc->pattern)
                      - rc->err.data;
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    ngx_regex_malloc_init(rc->pool);

    re = pcre2_compile(rc->pattern.data, rc->pattern.len, options,
                       &errcode, &erroff, ngx_regex_compile_context);

    /* ensure that there is no current pool */
    ngx_regex_malloc_done();

    ଯଦି (re == ଖାଲି) {
        pcre2_get_error_message(errcode, errstr, 128);

        ଯଦି ((size_t) erroff == rc->pattern.len) {
            rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                              "pcre2_compile() failed: %s in \"%V\"",
                               errstr, &rc->pattern)
                          - rc->err.data;

        } ଅନ୍ୟଥା {
            rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                              "pcre2_compile() failed: %s in \"%V\" at \"%s\"",
                               errstr, &rc->pattern, rc->pattern.data + erroff)
                          - rc->err.data;
        }

        ଫେରନ୍ତୁ NGX_ERROR;
    }

    rc->regex = re;

    /* do not study at runtime */

    ଯଦି (ngx_regex_studies != ଖାଲି) {
        elt = ngx_list_ଠେଲ(ngx_regex_studies);
        ଯଦି (elt == ଖାଲି) {
            ଯାଅ nomem;
        }

        elt->regex = rc->regex;
        elt->name = rc->pattern.data;
    }

    n = pcre2_pattern_info(re, PCRE2_INFO_CAPTURECOUNT, &rc->captures);
    ଯଦି (n < 0) {
        p = "pcre2_pattern_info(\"%V\", PCRE2_INFO_CAPTURECOUNT) failed: %d";
        ଯାଅ failed;
    }

    ଯଦି (rc->captures == 0) {
        ଫେରନ୍ତୁ NGX_OK;
    }

    n = pcre2_pattern_info(re, PCRE2_INFO_NAMECOUNT, &rc->named_captures);
    ଯଦି (n < 0) {
        p = "pcre2_pattern_info(\"%V\", PCRE2_INFO_NAMECOUNT) failed: %d";
        ଯାଅ failed;
    }

    ଯଦି (rc->named_captures == 0) {
        ଫେରନ୍ତୁ NGX_OK;
    }

    n = pcre2_pattern_info(re, PCRE2_INFO_NAMEENTRYSIZE, &rc->name_size);
    ଯଦି (n < 0) {
        p = "pcre2_pattern_info(\"%V\", PCRE2_INFO_NAMEENTRYSIZE) failed: %d";
        ଯାଅ failed;
    }

    n = pcre2_pattern_info(re, PCRE2_INFO_NAMETABLE, &rc->names);
    ଯଦି (n < 0) {
        p = "pcre2_pattern_info(\"%V\", PCRE2_INFO_NAMETABLE) failed: %d";
        ଯାଅ failed;
    }

    ଫେରନ୍ତୁ NGX_OK;

failed:

    rc->err.len = ngx_snprintf(rc->err.data, rc->err.len, p, &rc->pattern, n)
                  - rc->err.data;
    ଫେରନ୍ତୁ NGX_ERROR;

nomem:

    rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                               "regex \"%V\" compilation failed: no memory",
                               &rc->pattern)
                  - rc->err.data;
    ଫେରନ୍ତୁ NGX_ERROR;
}

#ଅନ୍ୟଥା

ngx_int_t
ngx_regex_compile(ngx_regex_compile_t *rc)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା               n, erroff;
    ଅକ୍ଷର             *p;
    pcre             *re;
    ଅଚଳ ଅକ୍ଷର       *errstr;
    ngx_uint_t        options;
    ngx_regex_elt_t  *elt;

    options = 0;

    ଯଦି (rc->options & NGX_REGEX_CASELESS) {
        options |= PCRE_CASELESS;
    }

    ଯଦି (rc->options & NGX_REGEX_MULTILINE) {
        options |= PCRE_MULTILINE;
    }

    ଯଦି (rc->options & ~(NGX_REGEX_CASELESS|NGX_REGEX_MULTILINE)) {
        rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                            "regex \"%V\" compilation failed: invalid options",
                            &rc->pattern)
                      - rc->err.data;
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    ngx_regex_malloc_init(rc->pool);

    re = pcre_compile((ଅଚଳ ଅକ୍ଷର *) rc->pattern.data, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) options,
                      &errstr, &erroff, ଖାଲି);

    /* ensure that there is no current pool */
    ngx_regex_malloc_done();

    ଯଦି (re == ଖାଲି) {
        ଯଦି ((size_t) erroff == rc->pattern.len) {
           rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                              "pcre_compile() failed: %s in \"%V\"",
                               errstr, &rc->pattern)
                         - rc->err.data;

        } ଅନ୍ୟଥା {
           rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                              "pcre_compile() failed: %s in \"%V\" at \"%s\"",
                               errstr, &rc->pattern, rc->pattern.data + erroff)
                         - rc->err.data;
        }

        ଫେରନ୍ତୁ NGX_ERROR;
    }

    rc->regex = ngx_pcalloc(rc->pool, ଆକାର(ngx_regex_t));
    ଯଦି (rc->regex == ଖାଲି) {
        ଯାଅ nomem;
    }

    rc->regex->code = re;

    /* do not study at runtime */

    ଯଦି (ngx_regex_studies != ଖାଲି) {
        elt = ngx_list_ଠେଲ(ngx_regex_studies);
        ଯଦି (elt == ଖାଲି) {
            ଯାଅ nomem;
        }

        elt->regex = rc->regex;
        elt->name = rc->pattern.data;
    }

    n = pcre_fullinfo(re, ଖାଲି, PCRE_INFO_CAPTURECOUNT, &rc->captures);
    ଯଦି (n < 0) {
        p = "pcre_fullinfo(\"%V\", PCRE_INFO_CAPTURECOUNT) failed: %d";
        ଯାଅ failed;
    }

    ଯଦି (rc->captures == 0) {
        ଫେରନ୍ତୁ NGX_OK;
    }

    n = pcre_fullinfo(re, ଖାଲି, PCRE_INFO_NAMECOUNT, &rc->named_captures);
    ଯଦି (n < 0) {
        p = "pcre_fullinfo(\"%V\", PCRE_INFO_NAMECOUNT) failed: %d";
        ଯାଅ failed;
    }

    ଯଦି (rc->named_captures == 0) {
        ଫେରନ୍ତୁ NGX_OK;
    }

    n = pcre_fullinfo(re, ଖାଲି, PCRE_INFO_NAMEENTRYSIZE, &rc->name_size);
    ଯଦି (n < 0) {
        p = "pcre_fullinfo(\"%V\", PCRE_INFO_NAMEENTRYSIZE) failed: %d";
        ଯାଅ failed;
    }

    n = pcre_fullinfo(re, ଖାଲି, PCRE_INFO_NAMETABLE, &rc->names);
    ଯଦି (n < 0) {
        p = "pcre_fullinfo(\"%V\", PCRE_INFO_NAMETABLE) failed: %d";
        ଯାଅ failed;
    }

    ଫେରନ୍ତୁ NGX_OK;

failed:

    rc->err.len = ngx_snprintf(rc->err.data, rc->err.len, p, &rc->pattern, n)
                  - rc->err.data;
    ଫେରନ୍ତୁ NGX_ERROR;

nomem:

    rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                               "regex \"%V\" compilation failed: no memory",
                               &rc->pattern)
                  - rc->err.data;
    ଫେରନ୍ତୁ NGX_ERROR;
}

#endif


#ଯଦି (NGX_PCRE2)

ngx_int_t
ngx_regex_exec(ngx_regex_t *re, ngx_str_t *s, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *captures, ngx_uint_t size)
{
    size_t      *ov;
    ngx_int_t    rc;
    ngx_uint_t   n, i;

    /*
     * The pcre2_match() function might allocate memory for backtracking
     * frames, typical allocations are from 40k and above.  So the allocator
     * is configured to do direct allocations from heap during matching.
     */

    ngx_regex_malloc_init(ଖାଲି);

    ଯଦି (ngx_regex_match_data == ଖାଲି
        || size > ngx_regex_match_data_size)
    {
        /*
         * Allocate a match data if not yet allocated or smaller than
         * needed.
         */

        ଯଦି (ngx_regex_match_data) {
            pcre2_match_data_free(ngx_regex_match_data);
        }

        ngx_regex_match_data_size = size;
        ngx_regex_match_data = pcre2_match_data_create(size / 3, ଖାଲି);

        ଯଦି (ngx_regex_match_data == ଖାଲି) {
            rc = PCRE2_ERROR_NOMEMORY;
            ଯାଅ failed;
        }
    }

    rc = pcre2_match(re, s->data, s->len, 0, 0, ngx_regex_match_data, ଖାଲି);

    ଯଦି (rc < 0) {
        ଯାଅ failed;
    }

    n = pcre2_get_ovector_ଗଣନା(ngx_regex_match_data);
    ov = pcre2_get_ovector_pointer(ngx_regex_match_data);

    ଯଦି (n > size / 3) {
        n = size / 3;
    }

    ପାଇଁ (i = 0; i < n; i++) {
        captures[i * 2] = ov[i * 2];
        captures[i * 2 + 1] = ov[i * 2 + 1];
    }

failed:

    ngx_regex_malloc_done();

    ଫେରନ୍ତୁ rc;
}

#ଅନ୍ୟଥା

ngx_int_t
ngx_regex_exec(ngx_regex_t *re, ngx_str_t *s, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *captures, ngx_uint_t size)
{
    ଫେରନ୍ତୁ pcre_exec(re->code, re->extra, (ଅଚଳ ଅକ୍ଷର *) s->data, s->len,
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

    ପାଇଁ (i = 0; i < a->nelts; i++) {

        n = ngx_regex_exec(re[i].regex, s, ଖାଲି, 0);

        ଯଦି (n == NGX_REGEX_NO_MATCHED) {
            ଜାରି;
        }

        ଯଦି (n < 0) {
            ngx_log_error(NGX_LOG_ALERT, log, 0,
                          ngx_regex_exec_n " failed: %i on \"%V\" using \"%s\"",
                          n, s, re[i].name);
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        /* match */

        ଫେରନ୍ତୁ NGX_OK;
    }

    ଫେରନ୍ତୁ NGX_DECLINED;
}


#ଯଦି (NGX_PCRE2)

ସ୍ଥିର ଶୂନ୍ୟ * ngx_libc_cdecl
ngx_regex_malloc(size_t size, ଶୂନ୍ୟ *data)
{
    ଯଦି (ngx_regex_pool) {
        ଫେରନ୍ତୁ ngx_palloc(ngx_regex_pool, size);
    }

    ଯଦି (ngx_regex_direct_alloc) {
        ଫେରନ୍ତୁ ngx_alloc(size, ngx_cycle->log);
    }

    ଫେରନ୍ତୁ ଖାଲି;
}


ସ୍ଥିର ଶୂନ୍ୟ ngx_libc_cdecl
ngx_regex_free(ଶୂନ୍ୟ *p, ଶୂନ୍ୟ *data)
{
    ଯଦି (ngx_regex_direct_alloc) {
        ngx_free(p);
    }

    ଫେରନ୍ତୁ;
}

#ଅନ୍ୟଥା

ସ୍ଥିର ଶୂନ୍ୟ * ngx_libc_cdecl
ngx_regex_malloc(size_t size)
{
    ଯଦି (ngx_regex_pool) {
        ଫେରନ୍ତୁ ngx_palloc(ngx_regex_pool, size);
    }

    ଫେରନ୍ତୁ ଖାଲି;
}


ସ୍ଥିର ଶୂନ୍ୟ ngx_libc_cdecl
ngx_regex_free(ଶୂନ୍ୟ *p)
{
    ଫେରନ୍ତୁ;
}

#endif


ସ୍ଥିର ଶୂନ୍ୟ
ngx_regex_cleanup(ଶୂନ୍ୟ *data)
{
#ଯଦି (NGX_PCRE2 || NGX_HAVE_PCRE_JIT)
    ngx_regex_conf_t *rcf = data;

    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_regex_elt_t  *elts;

    part = &rcf->studies->part;
    elts = part->elts;

    ପାଇଁ (i = 0; /* void */ ; i++) {

        ଯଦି (i >= part->nelts) {
            ଯଦି (part->next == ଖାଲି) {
                ଭାଙ୍ଗ;
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

#ଯଦି (NGX_PCRE2)
        pcre2_code_free(elts[i].regex);
#ଅନ୍ୟଥା
        ଯଦି (elts[i].regex->extra != ଖାଲି) {
            pcre_free_study(elts[i].regex->extra);
        }
#endif
    }
#endif

    /*
     * On configuration parsing errors ngx_regex_module_init() will not
     * be called.  Make sure ngx_regex_studies is properly cleared anyway.
     */

    ngx_regex_studies = ଖାଲି;

#ଯଦି (NGX_PCRE2)

    /*
     * Free compile context and match data.  If needed at runtime by
     * the new cycle, these will be re-allocated.
     */

    ngx_regex_malloc_init(ଖାଲି);

    ଯଦି (ngx_regex_compile_context) {
        pcre2_compile_context_free(ngx_regex_compile_context);
        ngx_regex_compile_context = ଖାଲି;
    }

    ଯଦି (ngx_regex_match_data) {
        pcre2_match_data_free(ngx_regex_match_data);
        ngx_regex_match_data = ଖାଲି;
        ngx_regex_match_data_size = 0;
    }

    ngx_regex_malloc_done();

#endif
}


ସ୍ଥିର ngx_int_t
ngx_regex_module_init(ngx_cycle_t *cycle)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା                opt;
#ଯଦି !(NGX_PCRE2)
    ଅଚଳ ଅକ୍ଷର        *errstr;
#endif
    ngx_uint_t         i;
    ngx_list_part_t   *part;
    ngx_regex_elt_t   *elts;
    ngx_regex_conf_t  *rcf;

    opt = 0;

    rcf = (ngx_regex_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_regex_module);

#ଯଦି (NGX_PCRE2 || NGX_HAVE_PCRE_JIT)

    ଯଦି (rcf->pcre_jit) {
#ଯଦି (NGX_PCRE2)
        opt = 1;
#ଅନ୍ୟଥା
        opt = PCRE_STUDY_JIT_COMPILE;
#endif
    }

#endif

    ngx_regex_malloc_init(cycle->pool);

    part = &rcf->studies->part;
    elts = part->elts;

    ପାଇଁ (i = 0; /* void */ ; i++) {

        ଯଦି (i >= part->nelts) {
            ଯଦି (part->next == ଖାଲି) {
                ଭାଙ୍ଗ;
            }

            part = part->next;
            elts = part->elts;
            i = 0;
        }

#ଯଦି (NGX_PCRE2)

        ଯଦି (opt) {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା  n;

            n = pcre2_jit_compile(elts[i].regex, PCRE2_JIT_COMPLETE);

            ଯଦି (n != 0) {
                ngx_log_error(NGX_LOG_INFO, cycle->log, 0,
                              "pcre2_jit_compile() failed: %d in \"%s\", "
                              "ignored",
                              n, elts[i].name);
            }
        }

#ଅନ୍ୟଥା

        elts[i].regex->extra = pcre_study(elts[i].regex->code, opt, &errstr);

        ଯଦି (errstr != ଖାଲି) {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, 0,
                          "pcre_study() failed: %s in \"%s\"",
                          errstr, elts[i].name);
        }

#ଯଦି (NGX_HAVE_PCRE_JIT)
        ଯଦି (opt & PCRE_STUDY_JIT_COMPILE) {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା jit, n;

            jit = 0;
            n = pcre_fullinfo(elts[i].regex->code, elts[i].regex->extra,
                              PCRE_INFO_JIT, &jit);

            ଯଦି (n != 0 || jit != 1) {
                ngx_log_error(NGX_LOG_INFO, cycle->log, 0,
                              "JIT compiler does not support pattern: \"%s\"",
                              elts[i].name);
            }
        }
#endif
#endif
    }

    ngx_regex_malloc_done();

    ngx_regex_studies = ଖାଲି;

    ଫେରନ୍ତୁ NGX_OK;
}


ସ୍ଥିର ଶୂନ୍ୟ *
ngx_regex_create_conf(ngx_cycle_t *cycle)
{
    ngx_regex_conf_t    *rcf;
    ngx_pool_cleanup_t  *cln;

    rcf = ngx_pcalloc(cycle->pool, ଆକାର(ngx_regex_conf_t));
    ଯଦି (rcf == ଖାଲି) {
        ଫେରନ୍ତୁ ଖାଲି;
    }

    rcf->pcre_jit = NGX_CONF_UNSET;

    cln = ngx_pool_cleanup_add(cycle->pool, 0);
    ଯଦି (cln == ଖାଲି) {
        ଫେରନ୍ତୁ ଖାଲି;
    }

    rcf->studies = ngx_list_create(cycle->pool, 8, ଆକାର(ngx_regex_elt_t));
    ଯଦି (rcf->studies == ଖାଲି) {
        ଫେରନ୍ତୁ ଖାଲି;
    }

    cln->handler = ngx_regex_cleanup;
    cln->data = rcf;

    ngx_regex_studies = rcf->studies;

    ଫେରନ୍ତୁ rcf;
}


ସ୍ଥିର ଅକ୍ଷର *
ngx_regex_init_conf(ngx_cycle_t *cycle, ଶୂନ୍ୟ *conf)
{
    ngx_regex_conf_t *rcf = conf;

    ngx_conf_init_ମୂଲ୍ୟ(rcf->pcre_jit, 0);

    ଫେରନ୍ତୁ NGX_CONF_OK;
}


ସ୍ଥିର ଅକ୍ଷର *
ngx_regex_pcre_jit(ngx_conf_t *cf, ଶୂନ୍ୟ *post, ଶୂନ୍ୟ *data)
{
    ngx_flag_t  *fp = data;

    ଯଦି (*fp == 0) {
        ଫେରନ୍ତୁ NGX_CONF_OK;
    }

#ଯଦି (NGX_PCRE2)
    {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା       r;
    uint32_t  jit;

    jit = 0;
    r = pcre2_config(PCRE2_CONFIG_JIT, &jit);

    ଯଦି (r != 0 || jit != 1) {
        ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                           "PCRE2 library does not support JIT");
        *fp = 0;
    }
    }
#ନଚେତଯଦି (NGX_HAVE_PCRE_JIT)
    {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା  jit, r;

    jit = 0;
    r = pcre_config(PCRE_CONFIG_JIT, &jit);

    ଯଦି (r != 0 || jit != 1) {
        ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                           "PCRE library does not support JIT");
        *fp = 0;
    }
    }
#ଅନ୍ୟଥା
    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "nginx was built without PCRE JIT support");
    *fp = 0;
#endif

    ଫେରନ୍ତୁ NGX_CONF_OK;
}
