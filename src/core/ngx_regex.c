
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>


రకనిర్వచనం నిర్మాణం {
    ngx_flag_t   pcre_jit;
    ngx_list_t  *studies;
} ngx_regex_conf_t;


స్థిర ngx_inline శూన్య ngx_regex_malloc_init(ngx_pool_t *pool);
స్థిర ngx_inline శూన్య ngx_regex_malloc_పూర్తి(శూన్య);

#ఒకవేళ (NGX_PCRE2)
స్థిర శూన్య * ngx_libc_cdecl ngx_regex_malloc(కొలత_t కొలత, శూన్య *దత్తాంశం);
స్థిర శూన్య ngx_libc_cdecl ngx_regex_free(శూన్య *p, శూన్య *దత్తాంశం);
#లేకపోతే
స్థిర శూన్య * ngx_libc_cdecl ngx_regex_malloc(కొలత_t కొలత);
స్థిర శూన్య ngx_libc_cdecl ngx_regex_free(శూన్య *p);
#అయితే_ముగింపు
స్థిర శూన్య ngx_regex_cleanup(శూన్య *దత్తాంశం);

స్థిర ngx_int_t ngx_regex_module_init(ngx_చక్రం_t *చక్రం);

స్థిర శూన్య *ngx_regex_create_conf(ngx_చక్రం_t *చక్రం);
స్థిర అక్షరం *ngx_regex_init_conf(ngx_చక్రం_t *చక్రం, శూన్య *conf);

స్థిర అక్షరం *ngx_regex_pcre_jit(ngx_conf_t *cf, శూన్య *post, శూన్య *దత్తాంశం);
స్థిర ngx_conf_post_t  ngx_regex_pcre_jit_post = { ngx_regex_pcre_jit };


స్థిర ngx_command_t  ngx_regex_commands[] = {

    { ngx_string("pcre_jit"),
      NGX_MAIN_CONF|NGX_ప్రత్యక్ష_CONF|NGX_CONF_FLAG,
      ngx_conf_సమితి_flag_స్థానం,
      0,
      offsetof(ngx_regex_conf_t, pcre_jit),
      &ngx_regex_pcre_jit_post },

      ngx_null_command
};


స్థిర ngx_core_module_t  ngx_regex_module_ctx = {
    ngx_string("regex"),
    ngx_regex_create_conf,
    ngx_regex_init_conf
};


ngx_module_t  ngx_regex_module = {
    NGX_MODULE_V1,
    &ngx_regex_module_ctx,                 /* module context */
    ngx_regex_commands,                    /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    ఖాళీ,                                  /* init master */
    ngx_regex_module_init,                 /* init module */
    ఖాళీ,                                  /* init process */
    ఖాళీ,                                  /* init thread */
    ఖాళీ,                                  /* exit thread */
    ఖాళీ,                                  /* exit process */
    ఖాళీ,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


స్థిర ngx_pool_t             *ngx_regex_pool;
స్థిర ngx_list_t             *ngx_regex_studies;
స్థిర ngx_uint_t              ngx_regex_ప్రత్యక్ష_alloc;

#ఒకవేళ (NGX_PCRE2)
స్థిర pcre2_compile_context  *ngx_regex_compile_context;
స్థిర pcre2_సరిపోలిక_దత్తాంశం       *ngx_regex_సరిపోలిక_దత్తాంశం;
స్థిర ngx_uint_t              ngx_regex_సరిపోలిక_దత్తాంశం_కొలత;
#అయితే_ముగింపు


శూన్య
ngx_regex_init(శూన్య)
{
#ఒకవేళ !(NGX_PCRE2)
    pcre_malloc = ngx_regex_malloc;
    pcre_free = ngx_regex_free;
#అయితే_ముగింపు
}


స్థిర ngx_inline శూన్య
ngx_regex_malloc_init(ngx_pool_t *pool)
{
    ngx_regex_pool = pool;
    ngx_regex_ప్రత్యక్ష_alloc = (pool == ఖాళీ) ? 1 : 0;
}


స్థిర ngx_inline శూన్య
ngx_regex_malloc_పూర్తి(శూన్య)
{
    ngx_regex_pool = ఖాళీ;
    ngx_regex_ప్రత్యక్ష_alloc = 0;
}


#ఒకవేళ (NGX_PCRE2)

ngx_int_t
ngx_regex_compile(ngx_regex_compile_t *rc)
{
    పూర్ణాంకం                     n, errcode;
    అక్షరం                   *p;
    u_char                  errstr[128];
    కొలత_t                  erroff;
    uint32_t                options;
    pcre2_సంకేతం             *re;
    ngx_regex_elt_t        *elt;
    pcre2_general_context  *gctx;
    pcre2_compile_context  *cctx;

    ఒకవేళ (ngx_regex_compile_context == ఖాళీ) {
        /*
         * Allocate a compile context if not yet allocated.  This uses
         * direct allocations from heap, so the result can be cached
         * even at runtime.
         */

        ngx_regex_malloc_init(ఖాళీ);

        gctx = pcre2_general_context_create(ngx_regex_malloc, ngx_regex_free,
                                            ఖాళీ);
        ఒకవేళ (gctx == ఖాళీ) {
            ngx_regex_malloc_పూర్తి();
            వెళ్ళు nomem;
        }

        cctx = pcre2_compile_context_create(gctx);
        ఒకవేళ (cctx == ఖాళీ) {
            pcre2_general_context_free(gctx);
            ngx_regex_malloc_పూర్తి();
            వెళ్ళు nomem;
        }

        ngx_regex_compile_context = cctx;

        pcre2_general_context_free(gctx);
        ngx_regex_malloc_పూర్తి();
    }

    options = 0;

    ఒకవేళ (rc->options & NGX_REGEX_CASELESS) {
        options |= PCRE2_CASELESS;
    }

    ఒకవేళ (rc->options & NGX_REGEX_MULTILINE) {
        options |= PCRE2_MULTILINE;
    }

    ఒకవేళ (rc->options & ~(NGX_REGEX_CASELESS|NGX_REGEX_MULTILINE)) {
        rc->err.len = ngx_snprintf(rc->err.దత్తాంశం, rc->err.len,
                            "regex \"%V\" compilation failed: invalid options",
                            &rc->నమూనా)
                      - rc->err.దత్తాంశం;
        తిరిగి NGX_ERROR;
    }

    ngx_regex_malloc_init(rc->pool);

    re = pcre2_compile(rc->నమూనా.దత్తాంశం, rc->నమూనా.len, options,
                       &errcode, &erroff, ngx_regex_compile_context);

    /* ensure that there is no current pool */
    ngx_regex_malloc_పూర్తి();

    ఒకవేళ (re == ఖాళీ) {
        pcre2_get_error_సందేశం(errcode, errstr, 128);

        ఒకవేళ ((కొలత_t) erroff == rc->నమూనా.len) {
            rc->err.len = ngx_snprintf(rc->err.దత్తాంశం, rc->err.len,
                              "pcre2_compile() failed: %s in \"%V\"",
                               errstr, &rc->నమూనా)
                          - rc->err.దత్తాంశం;

        } లేకపోతే {
            rc->err.len = ngx_snprintf(rc->err.దత్తాంశం, rc->err.len,
                              "pcre2_compile() failed: %s in \"%V\" at \"%s\"",
                               errstr, &rc->నమూనా, rc->నమూనా.దత్తాంశం + erroff)
                          - rc->err.దత్తాంశం;
        }

        తిరిగి NGX_ERROR;
    }

    rc->regex = re;

    /* do not study at runtime */

    ఒకవేళ (ngx_regex_studies != ఖాళీ) {
        elt = ngx_list_నెట్టు(ngx_regex_studies);
        ఒకవేళ (elt == ఖాళీ) {
            వెళ్ళు nomem;
        }

        elt->regex = rc->regex;
        elt->name = rc->నమూనా.దత్తాంశం;
    }

    n = pcre2_నమూనా_info(re, PCRE2_INFO_CAPTURECOUNT, &rc->captures);
    ఒకవేళ (n < 0) {
        p = "pcre2_pattern_info(\"%V\", PCRE2_INFO_CAPTURECOUNT) failed: %d";
        వెళ్ళు failed;
    }

    ఒకవేళ (rc->captures == 0) {
        తిరిగి NGX_OK;
    }

    n = pcre2_నమూనా_info(re, PCRE2_INFO_NAMECOUNT, &rc->named_captures);
    ఒకవేళ (n < 0) {
        p = "pcre2_pattern_info(\"%V\", PCRE2_INFO_NAMECOUNT) failed: %d";
        వెళ్ళు failed;
    }

    ఒకవేళ (rc->named_captures == 0) {
        తిరిగి NGX_OK;
    }

    n = pcre2_నమూనా_info(re, PCRE2_INFO_NAMEENTRYSIZE, &rc->name_కొలత);
    ఒకవేళ (n < 0) {
        p = "pcre2_pattern_info(\"%V\", PCRE2_INFO_NAMEENTRYSIZE) failed: %d";
        వెళ్ళు failed;
    }

    n = pcre2_నమూనా_info(re, PCRE2_INFO_NAMETABLE, &rc->పేర్లు);
    ఒకవేళ (n < 0) {
        p = "pcre2_pattern_info(\"%V\", PCRE2_INFO_NAMETABLE) failed: %d";
        వెళ్ళు failed;
    }

    తిరిగి NGX_OK;

failed:

    rc->err.len = ngx_snprintf(rc->err.దత్తాంశం, rc->err.len, p, &rc->నమూనా, n)
                  - rc->err.దత్తాంశం;
    తిరిగి NGX_ERROR;

nomem:

    rc->err.len = ngx_snprintf(rc->err.దత్తాంశం, rc->err.len,
                               "regex \"%V\" compilation failed: no memory",
                               &rc->నమూనా)
                  - rc->err.దత్తాంశం;
    తిరిగి NGX_ERROR;
}

#లేకపోతే

ngx_int_t
ngx_regex_compile(ngx_regex_compile_t *rc)
{
    పూర్ణాంకం               n, erroff;
    అక్షరం             *p;
    pcre             *re;
    స్థిరరాశి అక్షరం       *errstr;
    ngx_uint_t        options;
    ngx_regex_elt_t  *elt;

    options = 0;

    ఒకవేళ (rc->options & NGX_REGEX_CASELESS) {
        options |= PCRE_CASELESS;
    }

    ఒకవేళ (rc->options & NGX_REGEX_MULTILINE) {
        options |= PCRE_MULTILINE;
    }

    ఒకవేళ (rc->options & ~(NGX_REGEX_CASELESS|NGX_REGEX_MULTILINE)) {
        rc->err.len = ngx_snprintf(rc->err.దత్తాంశం, rc->err.len,
                            "regex \"%V\" compilation failed: invalid options",
                            &rc->నమూనా)
                      - rc->err.దత్తాంశం;
        తిరిగి NGX_ERROR;
    }

    ngx_regex_malloc_init(rc->pool);

    re = pcre_compile((స్థిరరాశి అక్షరం *) rc->నమూనా.దత్తాంశం, (పూర్ణాంకం) options,
                      &errstr, &erroff, ఖాళీ);

    /* ensure that there is no current pool */
    ngx_regex_malloc_పూర్తి();

    ఒకవేళ (re == ఖాళీ) {
        ఒకవేళ ((కొలత_t) erroff == rc->నమూనా.len) {
           rc->err.len = ngx_snprintf(rc->err.దత్తాంశం, rc->err.len,
                              "pcre_compile() failed: %s in \"%V\"",
                               errstr, &rc->నమూనా)
                         - rc->err.దత్తాంశం;

        } లేకపోతే {
           rc->err.len = ngx_snprintf(rc->err.దత్తాంశం, rc->err.len,
                              "pcre_compile() failed: %s in \"%V\" at \"%s\"",
                               errstr, &rc->నమూనా, rc->నమూనా.దత్తాంశం + erroff)
                         - rc->err.దత్తాంశం;
        }

        తిరిగి NGX_ERROR;
    }

    rc->regex = ngx_pcalloc(rc->pool, పరిమాణం(ngx_regex_t));
    ఒకవేళ (rc->regex == ఖాళీ) {
        వెళ్ళు nomem;
    }

    rc->regex->సంకేతం_2 = re;

    /* do not study at runtime */

    ఒకవేళ (ngx_regex_studies != ఖాళీ) {
        elt = ngx_list_నెట్టు(ngx_regex_studies);
        ఒకవేళ (elt == ఖాళీ) {
            వెళ్ళు nomem;
        }

        elt->regex = rc->regex;
        elt->name = rc->నమూనా.దత్తాంశం;
    }

    n = pcre_fullinfo(re, ఖాళీ, PCRE_INFO_CAPTURECOUNT, &rc->captures);
    ఒకవేళ (n < 0) {
        p = "pcre_fullinfo(\"%V\", PCRE_INFO_CAPTURECOUNT) failed: %d";
        వెళ్ళు failed;
    }

    ఒకవేళ (rc->captures == 0) {
        తిరిగి NGX_OK;
    }

    n = pcre_fullinfo(re, ఖాళీ, PCRE_INFO_NAMECOUNT, &rc->named_captures);
    ఒకవేళ (n < 0) {
        p = "pcre_fullinfo(\"%V\", PCRE_INFO_NAMECOUNT) failed: %d";
        వెళ్ళు failed;
    }

    ఒకవేళ (rc->named_captures == 0) {
        తిరిగి NGX_OK;
    }

    n = pcre_fullinfo(re, ఖాళీ, PCRE_INFO_NAMEENTRYSIZE, &rc->name_కొలత);
    ఒకవేళ (n < 0) {
        p = "pcre_fullinfo(\"%V\", PCRE_INFO_NAMEENTRYSIZE) failed: %d";
        వెళ్ళు failed;
    }

    n = pcre_fullinfo(re, ఖాళీ, PCRE_INFO_NAMETABLE, &rc->పేర్లు);
    ఒకవేళ (n < 0) {
        p = "pcre_fullinfo(\"%V\", PCRE_INFO_NAMETABLE) failed: %d";
        వెళ్ళు failed;
    }

    తిరిగి NGX_OK;

failed:

    rc->err.len = ngx_snprintf(rc->err.దత్తాంశం, rc->err.len, p, &rc->నమూనా, n)
                  - rc->err.దత్తాంశం;
    తిరిగి NGX_ERROR;

nomem:

    rc->err.len = ngx_snprintf(rc->err.దత్తాంశం, rc->err.len,
                               "regex \"%V\" compilation failed: no memory",
                               &rc->నమూనా)
                  - rc->err.దత్తాంశం;
    తిరిగి NGX_ERROR;
}

#అయితే_ముగింపు


#ఒకవేళ (NGX_PCRE2)

ngx_int_t
ngx_regex_exec(ngx_regex_t *re, ngx_str_t *s, పూర్ణాంకం *captures, ngx_uint_t కొలత)
{
    కొలత_t      *ov;
    ngx_int_t    rc;
    ngx_uint_t   n, i;

    /*
     * The pcre2_match() function might allocate memory for backtracking
     * frames, typical allocations are from 40k and above.  So the allocator
     * is configured to do direct allocations from heap during matching.
     */

    ngx_regex_malloc_init(ఖాళీ);

    ఒకవేళ (ngx_regex_సరిపోలిక_దత్తాంశం == ఖాళీ
        || కొలత > ngx_regex_సరిపోలిక_దత్తాంశం_కొలత)
    {
        /*
         * Allocate a match data if not yet allocated or smaller than
         * needed.
         */

        ఒకవేళ (ngx_regex_సరిపోలిక_దత్తాంశం) {
            pcre2_సరిపోలిక_దత్తాంశం_free(ngx_regex_సరిపోలిక_దత్తాంశం);
        }

        ngx_regex_సరిపోలిక_దత్తాంశం_కొలత = కొలత;
        ngx_regex_సరిపోలిక_దత్తాంశం = pcre2_సరిపోలిక_దత్తాంశం_create(కొలత / 3, ఖాళీ);

        ఒకవేళ (ngx_regex_సరిపోలిక_దత్తాంశం == ఖాళీ) {
            rc = PCRE2_ERROR_NOMEMORY;
            వెళ్ళు failed;
        }
    }

    rc = pcre2_సరిపోలిక(re, s->దత్తాంశం, s->len, 0, 0, ngx_regex_సరిపోలిక_దత్తాంశం, ఖాళీ);

    ఒకవేళ (rc < 0) {
        వెళ్ళు failed;
    }

    n = pcre2_get_ovector_లెక్క(ngx_regex_సరిపోలిక_దత్తాంశం);
    ov = pcre2_get_ovector_సూచిక(ngx_regex_సరిపోలిక_దత్తాంశం);

    ఒకవేళ (n > కొలత / 3) {
        n = కొలత / 3;
    }

    కొరకు (i = 0; i < n; i++) {
        captures[i * 2] = ov[i * 2];
        captures[i * 2 + 1] = ov[i * 2 + 1];
    }

failed:

    ngx_regex_malloc_పూర్తి();

    తిరిగి rc;
}

#లేకపోతే

ngx_int_t
ngx_regex_exec(ngx_regex_t *re, ngx_str_t *s, పూర్ణాంకం *captures, ngx_uint_t కొలత)
{
    తిరిగి pcre_exec(re->సంకేతం_2, re->extra, (స్థిరరాశి అక్షరం *) s->దత్తాంశం, s->len,
                     0, 0, captures, కొలత);
}

#అయితే_ముగింపు


ngx_int_t
ngx_regex_exec_array(ngx_array_t *a, ngx_str_t *s, ngx_log_t *సంవర్గమానం)
{
    ngx_int_t         n;
    ngx_uint_t        i;
    ngx_regex_elt_t  *re;

    re = a->elts;

    కొరకు (i = 0; i < a->nelts; i++) {

        n = ngx_regex_exec(re[i].regex, s, ఖాళీ, 0);

        ఒకవేళ (n == NGX_REGEX_NO_MATCHED) {
            కొనసాగు;
        }

        ఒకవేళ (n < 0) {
            ngx_log_error(NGX_LOG_ALERT, సంవర్గమానం, 0,
                          ngx_regex_exec_n " failed: %i on \"%V\" using \"%s\"",
                          n, s, re[i].name);
            తిరిగి NGX_ERROR;
        }

        /*సరిపోలిక*/

        తిరిగి NGX_OK;
    }

    తిరిగి NGX_DECLINED;
}


#ఒకవేళ (NGX_PCRE2)

స్థిర శూన్య * ngx_libc_cdecl
ngx_regex_malloc(కొలత_t కొలత, శూన్య *దత్తాంశం)
{
    ఒకవేళ (ngx_regex_pool) {
        తిరిగి ngx_palloc(ngx_regex_pool, కొలత);
    }

    ఒకవేళ (ngx_regex_ప్రత్యక్ష_alloc) {
        తిరిగి ngx_alloc(కొలత, ngx_చక్రం->సంవర్గమానం);
    }

    తిరిగి ఖాళీ;
}


స్థిర శూన్య ngx_libc_cdecl
ngx_regex_free(శూన్య *p, శూన్య *దత్తాంశం)
{
    ఒకవేళ (ngx_regex_ప్రత్యక్ష_alloc) {
        ngx_free(p);
    }

    తిరిగి;
}

#లేకపోతే

స్థిర శూన్య * ngx_libc_cdecl
ngx_regex_malloc(కొలత_t కొలత)
{
    ఒకవేళ (ngx_regex_pool) {
        తిరిగి ngx_palloc(ngx_regex_pool, కొలత);
    }

    తిరిగి ఖాళీ;
}


స్థిర శూన్య ngx_libc_cdecl
ngx_regex_free(శూన్య *p)
{
    తిరిగి;
}

#అయితే_ముగింపు


స్థిర శూన్య
ngx_regex_cleanup(శూన్య *దత్తాంశం)
{
#ఒకవేళ (NGX_PCRE2 || NGX_HAVE_PCRE_JIT)
    ngx_regex_conf_t *rcf = దత్తాంశం;

    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_regex_elt_t  *elts;

    part = &rcf->studies->part;
    elts = part->elts;

    కొరకు (i = 0; /* void */ ; i++) {

        ఒకవేళ (i >= part->nelts) {
            ఒకవేళ (part->తదుపరి == ఖాళీ) {
                ఆపు;
            }

            part = part->తదుపరి;
            elts = part->elts;
            i = 0;
        }

        /*
         * The PCRE JIT compiler uses mmap for its executable codes, so we
         * have to explicitly call the pcre_free_study() function to free
         * this memory.  In PCRE2, we call the pcre2_code_free() function
         * for the same reason.
         */

#ఒకవేళ (NGX_PCRE2)
        pcre2_సంకేతం_free(elts[i].regex);
#లేకపోతే
        ఒకవేళ (elts[i].regex->extra != ఖాళీ) {
            pcre_free_study(elts[i].regex->extra);
        }
#అయితే_ముగింపు
    }
#అయితే_ముగింపు

    /*
     * On configuration parsing errors ngx_regex_module_init() will not
     * be called.  Make sure ngx_regex_studies is properly cleared anyway.
     */

    ngx_regex_studies = ఖాళీ;

#ఒకవేళ (NGX_PCRE2)

    /*
     * Free compile context and match data.  If needed at runtime by
     * the new cycle, these will be re-allocated.
     */

    ngx_regex_malloc_init(ఖాళీ);

    ఒకవేళ (ngx_regex_compile_context) {
        pcre2_compile_context_free(ngx_regex_compile_context);
        ngx_regex_compile_context = ఖాళీ;
    }

    ఒకవేళ (ngx_regex_సరిపోలిక_దత్తాంశం) {
        pcre2_సరిపోలిక_దత్తాంశం_free(ngx_regex_సరిపోలిక_దత్తాంశం);
        ngx_regex_సరిపోలిక_దత్తాంశం = ఖాళీ;
        ngx_regex_సరిపోలిక_దత్తాంశం_కొలత = 0;
    }

    ngx_regex_malloc_పూర్తి();

#అయితే_ముగింపు
}


స్థిర ngx_int_t
ngx_regex_module_init(ngx_చక్రం_t *చక్రం)
{
    పూర్ణాంకం                opt;
#ఒకవేళ !(NGX_PCRE2)
    స్థిరరాశి అక్షరం        *errstr;
#అయితే_ముగింపు
    ngx_uint_t         i;
    ngx_list_part_t   *part;
    ngx_regex_elt_t   *elts;
    ngx_regex_conf_t  *rcf;

    opt = 0;

    rcf = (ngx_regex_conf_t *) ngx_get_conf(చక్రం->conf_ctx, ngx_regex_module);

#ఒకవేళ (NGX_PCRE2 || NGX_HAVE_PCRE_JIT)

    ఒకవేళ (rcf->pcre_jit) {
#ఒకవేళ (NGX_PCRE2)
        opt = 1;
#లేకపోతే
        opt = PCRE_STUDY_JIT_COMPILE;
#అయితే_ముగింపు
    }

#అయితే_ముగింపు

    ngx_regex_malloc_init(చక్రం->pool);

    part = &rcf->studies->part;
    elts = part->elts;

    కొరకు (i = 0; /* void */ ; i++) {

        ఒకవేళ (i >= part->nelts) {
            ఒకవేళ (part->తదుపరి == ఖాళీ) {
                ఆపు;
            }

            part = part->తదుపరి;
            elts = part->elts;
            i = 0;
        }

#ఒకవేళ (NGX_PCRE2)

        ఒకవేళ (opt) {
            పూర్ణాంకం  n;

            n = pcre2_jit_compile(elts[i].regex, PCRE2_JIT_COMPLETE);

            ఒకవేళ (n != 0) {
                ngx_log_error(NGX_LOG_INFO, చక్రం->సంవర్గమానం, 0,
                              "pcre2_jit_compile() failed: %d in \"%s\", "
                              "ignored",
                              n, elts[i].name);
            }
        }

#లేకపోతే

        elts[i].regex->extra = pcre_study(elts[i].regex->సంకేతం_2, opt, &errstr);

        ఒకవేళ (errstr != ఖాళీ) {
            ngx_log_error(NGX_LOG_ALERT, చక్రం->సంవర్గమానం, 0,
                          "pcre_study() failed: %s in \"%s\"",
                          errstr, elts[i].name);
        }

#ఒకవేళ (NGX_HAVE_PCRE_JIT)
        ఒకవేళ (opt & PCRE_STUDY_JIT_COMPILE) {
            పూర్ణాంకం jit, n;

            jit = 0;
            n = pcre_fullinfo(elts[i].regex->సంకేతం_2, elts[i].regex->extra,
                              PCRE_INFO_JIT, &jit);

            ఒకవేళ (n != 0 || jit != 1) {
                ngx_log_error(NGX_LOG_INFO, చక్రం->సంవర్గమానం, 0,
                              "JIT compiler does not support pattern: \"%s\"",
                              elts[i].name);
            }
        }
#అయితే_ముగింపు
#అయితే_ముగింపు
    }

    ngx_regex_malloc_పూర్తి();

    ngx_regex_studies = ఖాళీ;

    తిరిగి NGX_OK;
}


స్థిర శూన్య *
ngx_regex_create_conf(ngx_చక్రం_t *చక్రం)
{
    ngx_regex_conf_t    *rcf;
    ngx_pool_cleanup_t  *cln;

    rcf = ngx_pcalloc(చక్రం->pool, పరిమాణం(ngx_regex_conf_t));
    ఒకవేళ (rcf == ఖాళీ) {
        తిరిగి ఖాళీ;
    }

    rcf->pcre_jit = NGX_CONF_UNSET;

    cln = ngx_pool_cleanup_కూడు(చక్రం->pool, 0);
    ఒకవేళ (cln == ఖాళీ) {
        తిరిగి ఖాళీ;
    }

    rcf->studies = ngx_list_create(చక్రం->pool, 8, పరిమాణం(ngx_regex_elt_t));
    ఒకవేళ (rcf->studies == ఖాళీ) {
        తిరిగి ఖాళీ;
    }

    cln->handler = ngx_regex_cleanup;
    cln->దత్తాంశం = rcf;

    ngx_regex_studies = rcf->studies;

    తిరిగి rcf;
}


స్థిర అక్షరం *
ngx_regex_init_conf(ngx_చక్రం_t *చక్రం, శూన్య *conf)
{
    ngx_regex_conf_t *rcf = conf;

    ngx_conf_init_విలువ(rcf->pcre_jit, 0);

    తిరిగి NGX_CONF_OK;
}


స్థిర అక్షరం *
ngx_regex_pcre_jit(ngx_conf_t *cf, శూన్య *post, శూన్య *దత్తాంశం)
{
    ngx_flag_t  *fp = దత్తాంశం;

    ఒకవేళ (*fp == 0) {
        తిరిగి NGX_CONF_OK;
    }

#ఒకవేళ (NGX_PCRE2)
    {
    పూర్ణాంకం       r;
    uint32_t  jit;

    jit = 0;
    r = pcre2_config(PCRE2_CONFIG_JIT, &jit);

    ఒకవేళ (r != 0 || jit != 1) {
        ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                           "PCRE2 library does not support JIT");
        *fp = 0;
    }
    }
#లేదావేళ (NGX_HAVE_PCRE_JIT)
    {
    పూర్ణాంకం  jit, r;

    jit = 0;
    r = pcre_config(PCRE_CONFIG_JIT, &jit);

    ఒకవేళ (r != 0 || jit != 1) {
        ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                           "PCRE library does not support JIT");
        *fp = 0;
    }
    }
#లేకపోతే
    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "nginx was built without PCRE JIT support");
    *fp = 0;
#అయితే_ముగింపు

    తిరిగి NGX_CONF_OK;
}
