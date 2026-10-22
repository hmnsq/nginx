
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#समावेश <ngx_config.h>
#समावेश <ngx_core.h>


प्रकार काष्ठा {
    ngx_flag_t   pcre_jit;
    ngx_list_t  *studies;
} ngx_regex_conf_t;


अटल ngx_inline व्योम ngx_regex_malloc_init(ngx_pool_t *pool);
अटल ngx_inline व्योम ngx_regex_malloc_done(व्योम);

#अगर (NGX_PCRE2)
अटल व्योम * ngx_libc_cdecl ngx_regex_malloc(माप_प्रकार आकार, व्योम *तथ्य);
अटल व्योम ngx_libc_cdecl ngx_regex_free(व्योम *p, व्योम *तथ्य);
#अन्यथा
अटल व्योम * ngx_libc_cdecl ngx_regex_malloc(माप_प्रकार आकार);
अटल व्योम ngx_libc_cdecl ngx_regex_free(व्योम *p);
#पूर्ण_अगर
अटल व्योम ngx_regex_cleanup(व्योम *तथ्य);

अटल ngx_int_t ngx_regex_module_init(ngx_cycle_t *cycle);

अटल व्योम *ngx_regex_create_conf(ngx_cycle_t *cycle);
अटल अक्षर *ngx_regex_init_conf(ngx_cycle_t *cycle, व्योम *conf);

अटल अक्षर *ngx_regex_pcre_jit(ngx_conf_t *cf, व्योम *post, व्योम *तथ्य);
अटल ngx_conf_post_t  ngx_regex_pcre_jit_post = { ngx_regex_pcre_jit };


अटल ngx_command_t  ngx_regex_commands[] = {

    { ngx_string("pcre_jit"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      0,
      दुरत्व(ngx_regex_conf_t, pcre_jit),
      &ngx_regex_pcre_jit_post },

      ngx_null_command
};


अटल ngx_core_module_t  ngx_regex_module_ctx = {
    ngx_string("regex"),
    ngx_regex_create_conf,
    ngx_regex_init_conf
};


ngx_module_t  ngx_regex_module = {
    NGX_MODULE_V1,
    &ngx_regex_module_ctx,                 /* module context */
    ngx_regex_commands,                    /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    शून्य,                                  /* init master */
    ngx_regex_module_init,                 /* init module */
    शून्य,                                  /* init process */
    शून्य,                                  /* init thread */
    शून्य,                                  /* exit thread */
    शून्य,                                  /* exit process */
    शून्य,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


अटल ngx_pool_t             *ngx_regex_pool;
अटल ngx_list_t             *ngx_regex_studies;
अटल ngx_uint_t              ngx_regex_direct_alloc;

#अगर (NGX_PCRE2)
अटल pcre2_compile_context  *ngx_regex_compile_context;
अटल pcre2_match_तथ्य       *ngx_regex_match_तथ्य;
अटल ngx_uint_t              ngx_regex_match_तथ्य_आकार;
#पूर्ण_अगर


व्योम
ngx_regex_init(व्योम)
{
#अगर !(NGX_PCRE2)
    pcre_malloc = ngx_regex_malloc;
    pcre_free = ngx_regex_free;
#पूर्ण_अगर
}


अटल ngx_inline व्योम
ngx_regex_malloc_init(ngx_pool_t *pool)
{
    ngx_regex_pool = pool;
    ngx_regex_direct_alloc = (pool == शून्य) ? 1 : 0;
}


अटल ngx_inline व्योम
ngx_regex_malloc_done(व्योम)
{
    ngx_regex_pool = शून्य;
    ngx_regex_direct_alloc = 0;
}


#अगर (NGX_PCRE2)

ngx_int_t
ngx_regex_compile(ngx_regex_compile_t *rc)
{
    पूर्णांक                     n, errcode;
    अक्षर                   *p;
    u_char                  errstr[128];
    माप_प्रकार                  erroff;
    uint32_t                options;
    pcre2_code             *re;
    ngx_regex_elt_t        *elt;
    pcre2_general_context  *gctx;
    pcre2_compile_context  *cctx;

    अगर (ngx_regex_compile_context == शून्य) {
        /*
         * Allocate a compile context if not yet allocated.  This uses
         * direct allocations from heap, so the result can be cached
         * even at runtime.
         */

        ngx_regex_malloc_init(शून्य);

        gctx = pcre2_general_context_create(ngx_regex_malloc, ngx_regex_free,
                                            शून्य);
        अगर (gctx == शून्य) {
            ngx_regex_malloc_done();
            जाओ nomem;
        }

        cctx = pcre2_compile_context_create(gctx);
        अगर (cctx == शून्य) {
            pcre2_general_context_free(gctx);
            ngx_regex_malloc_done();
            जाओ nomem;
        }

        ngx_regex_compile_context = cctx;

        pcre2_general_context_free(gctx);
        ngx_regex_malloc_done();
    }

    options = 0;

    अगर (rc->options & NGX_REGEX_CASELESS) {
        options |= PCRE2_CASELESS;
    }

    अगर (rc->options & NGX_REGEX_MULTILINE) {
        options |= PCRE2_MULTILINE;
    }

    अगर (rc->options & ~(NGX_REGEX_CASELESS|NGX_REGEX_MULTILINE)) {
        rc->err.len = ngx_snprintf(rc->err.तथ्य, rc->err.len,
                            "regex \"%V\" compilation failed: invalid options",
                            &rc->pattern)
                      - rc->err.तथ्य;
        वापस NGX_ERROR;
    }

    ngx_regex_malloc_init(rc->pool);

    re = pcre2_compile(rc->pattern.तथ्य, rc->pattern.len, options,
                       &errcode, &erroff, ngx_regex_compile_context);

    /* ensure that there is no current pool */
    ngx_regex_malloc_done();

    अगर (re == शून्य) {
        pcre2_get_error_message(errcode, errstr, 128);

        अगर ((माप_प्रकार) erroff == rc->pattern.len) {
            rc->err.len = ngx_snprintf(rc->err.तथ्य, rc->err.len,
                              "pcre2_compile() failed: %s in \"%V\"",
                               errstr, &rc->pattern)
                          - rc->err.तथ्य;

        } अन्यथा {
            rc->err.len = ngx_snprintf(rc->err.तथ्य, rc->err.len,
                              "pcre2_compile() failed: %s in \"%V\" at \"%s\"",
                               errstr, &rc->pattern, rc->pattern.तथ्य + erroff)
                          - rc->err.तथ्य;
        }

        वापस NGX_ERROR;
    }

    rc->regex = re;

    /* do not study at runtime */

    अगर (ngx_regex_studies != शून्य) {
        elt = ngx_list_धकेलो(ngx_regex_studies);
        अगर (elt == शून्य) {
            जाओ nomem;
        }

        elt->regex = rc->regex;
        elt->name = rc->pattern.तथ्य;
    }

    n = pcre2_pattern_info(re, PCRE2_INFO_CAPTURECOUNT, &rc->captures);
    अगर (n < 0) {
        p = "pcre2_pattern_info(\"%V\", PCRE2_INFO_CAPTURECOUNT) failed: %d";
        जाओ failed;
    }

    अगर (rc->captures == 0) {
        वापस NGX_OK;
    }

    n = pcre2_pattern_info(re, PCRE2_INFO_NAMECOUNT, &rc->named_captures);
    अगर (n < 0) {
        p = "pcre2_pattern_info(\"%V\", PCRE2_INFO_NAMECOUNT) failed: %d";
        जाओ failed;
    }

    अगर (rc->named_captures == 0) {
        वापस NGX_OK;
    }

    n = pcre2_pattern_info(re, PCRE2_INFO_NAMEENTRYSIZE, &rc->name_आकार);
    अगर (n < 0) {
        p = "pcre2_pattern_info(\"%V\", PCRE2_INFO_NAMEENTRYSIZE) failed: %d";
        जाओ failed;
    }

    n = pcre2_pattern_info(re, PCRE2_INFO_NAMETABLE, &rc->names);
    अगर (n < 0) {
        p = "pcre2_pattern_info(\"%V\", PCRE2_INFO_NAMETABLE) failed: %d";
        जाओ failed;
    }

    वापस NGX_OK;

failed:

    rc->err.len = ngx_snprintf(rc->err.तथ्य, rc->err.len, p, &rc->pattern, n)
                  - rc->err.तथ्य;
    वापस NGX_ERROR;

nomem:

    rc->err.len = ngx_snprintf(rc->err.तथ्य, rc->err.len,
                               "regex \"%V\" compilation failed: no memory",
                               &rc->pattern)
                  - rc->err.तथ्य;
    वापस NGX_ERROR;
}

#अन्यथा

ngx_int_t
ngx_regex_compile(ngx_regex_compile_t *rc)
{
    पूर्णांक               n, erroff;
    अक्षर             *p;
    pcre             *re;
    स्थिर अक्षर       *errstr;
    ngx_uint_t        options;
    ngx_regex_elt_t  *elt;

    options = 0;

    अगर (rc->options & NGX_REGEX_CASELESS) {
        options |= PCRE_CASELESS;
    }

    अगर (rc->options & NGX_REGEX_MULTILINE) {
        options |= PCRE_MULTILINE;
    }

    अगर (rc->options & ~(NGX_REGEX_CASELESS|NGX_REGEX_MULTILINE)) {
        rc->err.len = ngx_snprintf(rc->err.तथ्य, rc->err.len,
                            "regex \"%V\" compilation failed: invalid options",
                            &rc->pattern)
                      - rc->err.तथ्य;
        वापस NGX_ERROR;
    }

    ngx_regex_malloc_init(rc->pool);

    re = pcre_compile((स्थिर अक्षर *) rc->pattern.तथ्य, (पूर्णांक) options,
                      &errstr, &erroff, शून्य);

    /* ensure that there is no current pool */
    ngx_regex_malloc_done();

    अगर (re == शून्य) {
        अगर ((माप_प्रकार) erroff == rc->pattern.len) {
           rc->err.len = ngx_snprintf(rc->err.तथ्य, rc->err.len,
                              "pcre_compile() failed: %s in \"%V\"",
                               errstr, &rc->pattern)
                         - rc->err.तथ्य;

        } अन्यथा {
           rc->err.len = ngx_snprintf(rc->err.तथ्य, rc->err.len,
                              "pcre_compile() failed: %s in \"%V\" at \"%s\"",
                               errstr, &rc->pattern, rc->pattern.तथ्य + erroff)
                         - rc->err.तथ्य;
        }

        वापस NGX_ERROR;
    }

    rc->regex = ngx_pcalloc(rc->pool, माप(ngx_regex_t));
    अगर (rc->regex == शून्य) {
        जाओ nomem;
    }

    rc->regex->code = re;

    /* do not study at runtime */

    अगर (ngx_regex_studies != शून्य) {
        elt = ngx_list_धकेलो(ngx_regex_studies);
        अगर (elt == शून्य) {
            जाओ nomem;
        }

        elt->regex = rc->regex;
        elt->name = rc->pattern.तथ्य;
    }

    n = pcre_fullinfo(re, शून्य, PCRE_INFO_CAPTURECOUNT, &rc->captures);
    अगर (n < 0) {
        p = "pcre_fullinfo(\"%V\", PCRE_INFO_CAPTURECOUNT) failed: %d";
        जाओ failed;
    }

    अगर (rc->captures == 0) {
        वापस NGX_OK;
    }

    n = pcre_fullinfo(re, शून्य, PCRE_INFO_NAMECOUNT, &rc->named_captures);
    अगर (n < 0) {
        p = "pcre_fullinfo(\"%V\", PCRE_INFO_NAMECOUNT) failed: %d";
        जाओ failed;
    }

    अगर (rc->named_captures == 0) {
        वापस NGX_OK;
    }

    n = pcre_fullinfo(re, शून्य, PCRE_INFO_NAMEENTRYSIZE, &rc->name_आकार);
    अगर (n < 0) {
        p = "pcre_fullinfo(\"%V\", PCRE_INFO_NAMEENTRYSIZE) failed: %d";
        जाओ failed;
    }

    n = pcre_fullinfo(re, शून्य, PCRE_INFO_NAMETABLE, &rc->names);
    अगर (n < 0) {
        p = "pcre_fullinfo(\"%V\", PCRE_INFO_NAMETABLE) failed: %d";
        जाओ failed;
    }

    वापस NGX_OK;

failed:

    rc->err.len = ngx_snprintf(rc->err.तथ्य, rc->err.len, p, &rc->pattern, n)
                  - rc->err.तथ्य;
    वापस NGX_ERROR;

nomem:

    rc->err.len = ngx_snprintf(rc->err.तथ्य, rc->err.len,
                               "regex \"%V\" compilation failed: no memory",
                               &rc->pattern)
                  - rc->err.तथ्य;
    वापस NGX_ERROR;
}

#पूर्ण_अगर


#अगर (NGX_PCRE2)

ngx_int_t
ngx_regex_exec(ngx_regex_t *re, ngx_str_t *s, पूर्णांक *captures, ngx_uint_t आकार)
{
    माप_प्रकार      *ov;
    ngx_int_t    rc;
    ngx_uint_t   n, i;

    /*
     * The pcre2_match() function might allocate memory for backtracking
     * frames, typical allocations are from 40k and above.  So the allocator
     * is configured to do direct allocations from heap during matching.
     */

    ngx_regex_malloc_init(शून्य);

    अगर (ngx_regex_match_तथ्य == शून्य
        || आकार > ngx_regex_match_तथ्य_आकार)
    {
        /*
         * Allocate a match data if not yet allocated or smaller than
         * needed.
         */

        अगर (ngx_regex_match_तथ्य) {
            pcre2_match_तथ्य_free(ngx_regex_match_तथ्य);
        }

        ngx_regex_match_तथ्य_आकार = आकार;
        ngx_regex_match_तथ्य = pcre2_match_तथ्य_create(आकार / 3, शून्य);

        अगर (ngx_regex_match_तथ्य == शून्य) {
            rc = PCRE2_ERROR_NOMEMORY;
            जाओ failed;
        }
    }

    rc = pcre2_match(re, s->तथ्य, s->len, 0, 0, ngx_regex_match_तथ्य, शून्य);

    अगर (rc < 0) {
        जाओ failed;
    }

    n = pcre2_get_ovector_गिनती(ngx_regex_match_तथ्य);
    ov = pcre2_get_ovector_pointer(ngx_regex_match_तथ्य);

    अगर (n > आकार / 3) {
        n = आकार / 3;
    }

    क्रम (i = 0; i < n; i++) {
        captures[i * 2] = ov[i * 2];
        captures[i * 2 + 1] = ov[i * 2 + 1];
    }

failed:

    ngx_regex_malloc_done();

    वापस rc;
}

#अन्यथा

ngx_int_t
ngx_regex_exec(ngx_regex_t *re, ngx_str_t *s, पूर्णांक *captures, ngx_uint_t आकार)
{
    वापस pcre_exec(re->code, re->extra, (स्थिर अक्षर *) s->तथ्य, s->len,
                     0, 0, captures, आकार);
}

#पूर्ण_अगर


ngx_int_t
ngx_regex_exec_array(ngx_array_t *a, ngx_str_t *s, ngx_log_t *log)
{
    ngx_int_t         n;
    ngx_uint_t        i;
    ngx_regex_elt_t  *re;

    re = a->elts;

    क्रम (i = 0; i < a->nelts; i++) {

        n = ngx_regex_exec(re[i].regex, s, शून्य, 0);

        अगर (n == NGX_REGEX_NO_MATCHED) {
            जारी;
        }

        अगर (n < 0) {
            ngx_log_error(NGX_LOG_ALERT, log, 0,
                          ngx_regex_exec_n " failed: %i on \"%V\" using \"%s\"",
                          n, s, re[i].name);
            वापस NGX_ERROR;
        }

        /* match */

        वापस NGX_OK;
    }

    वापस NGX_DECLINED;
}


#अगर (NGX_PCRE2)

अटल व्योम * ngx_libc_cdecl
ngx_regex_malloc(माप_प्रकार आकार, व्योम *तथ्य)
{
    अगर (ngx_regex_pool) {
        वापस ngx_palloc(ngx_regex_pool, आकार);
    }

    अगर (ngx_regex_direct_alloc) {
        वापस ngx_alloc(आकार, ngx_cycle->log);
    }

    वापस शून्य;
}


अटल व्योम ngx_libc_cdecl
ngx_regex_free(व्योम *p, व्योम *तथ्य)
{
    अगर (ngx_regex_direct_alloc) {
        ngx_free(p);
    }

    वापस;
}

#अन्यथा

अटल व्योम * ngx_libc_cdecl
ngx_regex_malloc(माप_प्रकार आकार)
{
    अगर (ngx_regex_pool) {
        वापस ngx_palloc(ngx_regex_pool, आकार);
    }

    वापस शून्य;
}


अटल व्योम ngx_libc_cdecl
ngx_regex_free(व्योम *p)
{
    वापस;
}

#पूर्ण_अगर


अटल व्योम
ngx_regex_cleanup(व्योम *तथ्य)
{
#अगर (NGX_PCRE2 || NGX_HAVE_PCRE_JIT)
    ngx_regex_conf_t *rcf = तथ्य;

    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_regex_elt_t  *elts;

    part = &rcf->studies->part;
    elts = part->elts;

    क्रम (i = 0; /* void */ ; i++) {

        अगर (i >= part->nelts) {
            अगर (part->अग्र == शून्य) {
                अवरोध;
            }

            part = part->अग्र;
            elts = part->elts;
            i = 0;
        }

        /*
         * The PCRE JIT compiler uses mmap for its executable codes, so we
         * have to explicitly call the pcre_free_study() function to free
         * this memory.  In PCRE2, we call the pcre2_code_free() function
         * for the same reason.
         */

#अगर (NGX_PCRE2)
        pcre2_code_free(elts[i].regex);
#अन्यथा
        अगर (elts[i].regex->extra != शून्य) {
            pcre_free_study(elts[i].regex->extra);
        }
#पूर्ण_अगर
    }
#पूर्ण_अगर

    /*
     * On configuration parsing errors ngx_regex_module_init() will not
     * be called.  Make sure ngx_regex_studies is properly cleared anyway.
     */

    ngx_regex_studies = शून्य;

#अगर (NGX_PCRE2)

    /*
     * Free compile context and match data.  If needed at runtime by
     * the new cycle, these will be re-allocated.
     */

    ngx_regex_malloc_init(शून्य);

    अगर (ngx_regex_compile_context) {
        pcre2_compile_context_free(ngx_regex_compile_context);
        ngx_regex_compile_context = शून्य;
    }

    अगर (ngx_regex_match_तथ्य) {
        pcre2_match_तथ्य_free(ngx_regex_match_तथ्य);
        ngx_regex_match_तथ्य = शून्य;
        ngx_regex_match_तथ्य_आकार = 0;
    }

    ngx_regex_malloc_done();

#पूर्ण_अगर
}


अटल ngx_int_t
ngx_regex_module_init(ngx_cycle_t *cycle)
{
    पूर्णांक                opt;
#अगर !(NGX_PCRE2)
    स्थिर अक्षर        *errstr;
#पूर्ण_अगर
    ngx_uint_t         i;
    ngx_list_part_t   *part;
    ngx_regex_elt_t   *elts;
    ngx_regex_conf_t  *rcf;

    opt = 0;

    rcf = (ngx_regex_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_regex_module);

#अगर (NGX_PCRE2 || NGX_HAVE_PCRE_JIT)

    अगर (rcf->pcre_jit) {
#अगर (NGX_PCRE2)
        opt = 1;
#अन्यथा
        opt = PCRE_STUDY_JIT_COMPILE;
#पूर्ण_अगर
    }

#पूर्ण_अगर

    ngx_regex_malloc_init(cycle->pool);

    part = &rcf->studies->part;
    elts = part->elts;

    क्रम (i = 0; /* void */ ; i++) {

        अगर (i >= part->nelts) {
            अगर (part->अग्र == शून्य) {
                अवरोध;
            }

            part = part->अग्र;
            elts = part->elts;
            i = 0;
        }

#अगर (NGX_PCRE2)

        अगर (opt) {
            पूर्णांक  n;

            n = pcre2_jit_compile(elts[i].regex, PCRE2_JIT_COMPLETE);

            अगर (n != 0) {
                ngx_log_error(NGX_LOG_INFO, cycle->log, 0,
                              "pcre2_jit_compile() failed: %d in \"%s\", "
                              "ignored",
                              n, elts[i].name);
            }
        }

#अन्यथा

        elts[i].regex->extra = pcre_study(elts[i].regex->code, opt, &errstr);

        अगर (errstr != शून्य) {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, 0,
                          "pcre_study() failed: %s in \"%s\"",
                          errstr, elts[i].name);
        }

#अगर (NGX_HAVE_PCRE_JIT)
        अगर (opt & PCRE_STUDY_JIT_COMPILE) {
            पूर्णांक jit, n;

            jit = 0;
            n = pcre_fullinfo(elts[i].regex->code, elts[i].regex->extra,
                              PCRE_INFO_JIT, &jit);

            अगर (n != 0 || jit != 1) {
                ngx_log_error(NGX_LOG_INFO, cycle->log, 0,
                              "JIT compiler does not support pattern: \"%s\"",
                              elts[i].name);
            }
        }
#पूर्ण_अगर
#पूर्ण_अगर
    }

    ngx_regex_malloc_done();

    ngx_regex_studies = शून्य;

    वापस NGX_OK;
}


अटल व्योम *
ngx_regex_create_conf(ngx_cycle_t *cycle)
{
    ngx_regex_conf_t    *rcf;
    ngx_pool_cleanup_t  *cln;

    rcf = ngx_pcalloc(cycle->pool, माप(ngx_regex_conf_t));
    अगर (rcf == शून्य) {
        वापस शून्य;
    }

    rcf->pcre_jit = NGX_CONF_UNSET;

    cln = ngx_pool_cleanup_add(cycle->pool, 0);
    अगर (cln == शून्य) {
        वापस शून्य;
    }

    rcf->studies = ngx_list_create(cycle->pool, 8, माप(ngx_regex_elt_t));
    अगर (rcf->studies == शून्य) {
        वापस शून्य;
    }

    cln->handler = ngx_regex_cleanup;
    cln->तथ्य = rcf;

    ngx_regex_studies = rcf->studies;

    वापस rcf;
}


अटल अक्षर *
ngx_regex_init_conf(ngx_cycle_t *cycle, व्योम *conf)
{
    ngx_regex_conf_t *rcf = conf;

    ngx_conf_init_मूल्य(rcf->pcre_jit, 0);

    वापस NGX_CONF_OK;
}


अटल अक्षर *
ngx_regex_pcre_jit(ngx_conf_t *cf, व्योम *post, व्योम *तथ्य)
{
    ngx_flag_t  *fp = तथ्य;

    अगर (*fp == 0) {
        वापस NGX_CONF_OK;
    }

#अगर (NGX_PCRE2)
    {
    पूर्णांक       r;
    uint32_t  jit;

    jit = 0;
    r = pcre2_config(PCRE2_CONFIG_JIT, &jit);

    अगर (r != 0 || jit != 1) {
        ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                           "PCRE2 library does not support JIT");
        *fp = 0;
    }
    }
#या_अगर (NGX_HAVE_PCRE_JIT)
    {
    पूर्णांक  jit, r;

    jit = 0;
    r = pcre_config(PCRE_CONFIG_JIT, &jit);

    अगर (r != 0 || jit != 1) {
        ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                           "PCRE library does not support JIT");
        *fp = 0;
    }
    }
#अन्यथा
    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "nginx was built without PCRE JIT support");
    *fp = 0;
#पूर्ण_अगर

    वापस NGX_CONF_OK;
}
