
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


تعريفنوع بنية {
    ngx_flag_t   pcre_jit;
    ngx_list_t  *studies;
} ngx_regex_conf_t;


ساكن ngx_inline فراغ ngx_regex_malloc_init(ngx_pool_t *pool);
ساكن ngx_inline فراغ ngx_regex_malloc_انجامشده(فراغ);

#إذا (NGX_PCRE2)
ساكن فراغ * ngx_libc_cdecl ngx_regex_malloc(اندازه_t اندازه, فراغ *داده);
ساكن فراغ ngx_libc_cdecl ngx_regex_free(فراغ *p, فراغ *داده);
#وإلا
ساكن فراغ * ngx_libc_cdecl ngx_regex_malloc(اندازه_t اندازه);
ساكن فراغ ngx_libc_cdecl ngx_regex_free(فراغ *p);
#پایان_اگر
ساكن فراغ ngx_regex_cleanup(فراغ *داده);

ساكن ngx_int_t ngx_regex_module_init(ngx_چرخه_t *چرخه);

ساكن فراغ *ngx_regex_create_conf(ngx_چرخه_t *چرخه);
ساكن حرف *ngx_regex_init_conf(ngx_چرخه_t *چرخه, فراغ *conf);

ساكن حرف *ngx_regex_pcre_jit(ngx_conf_t *cf, فراغ *post, فراغ *داده);
ساكن ngx_conf_post_t  ngx_regex_pcre_jit_post = { ngx_regex_pcre_jit };


ساكن ngx_command_t  ngx_regex_commands[] = {

    { ngx_string("pcre_jit"),
      NGX_MAIN_CONF|NGX_مستقیم_CONF|NGX_CONF_FLAG,
      ngx_conf_مجموعه_flag_جایگاه,
      0,
      offsetof(ngx_regex_conf_t, pcre_jit),
      &ngx_regex_pcre_jit_post },

      ngx_null_command
};


ساكن ngx_core_module_t  ngx_regex_module_ctx = {
    ngx_string("regex"),
    ngx_regex_create_conf,
    ngx_regex_init_conf
};


ngx_module_t  ngx_regex_module = {
    NGX_MODULE_V1,
    &ngx_regex_module_ctx,                 /* module context */
    ngx_regex_commands,                    /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    عدم,                                  /* init master */
    ngx_regex_module_init,                 /* init module */
    عدم,                                  /* init process */
    عدم,                                  /* init thread */
    عدم,                                  /* exit thread */
    عدم,                                  /* exit process */
    عدم,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


ساكن ngx_pool_t             *ngx_regex_pool;
ساكن ngx_list_t             *ngx_regex_studies;
ساكن ngx_uint_t              ngx_regex_مستقیم_alloc;

#إذا (NGX_PCRE2)
ساكن pcre2_compile_context  *ngx_regex_compile_context;
ساكن pcre2_تطابق_داده       *ngx_regex_تطابق_داده;
ساكن ngx_uint_t              ngx_regex_تطابق_داده_اندازه;
#پایان_اگر


فراغ
ngx_regex_init(فراغ)
{
#إذا !(NGX_PCRE2)
    pcre_malloc = ngx_regex_malloc;
    pcre_free = ngx_regex_free;
#پایان_اگر
}


ساكن ngx_inline فراغ
ngx_regex_malloc_init(ngx_pool_t *pool)
{
    ngx_regex_pool = pool;
    ngx_regex_مستقیم_alloc = (pool == عدم) ? 1 : 0;
}


ساكن ngx_inline فراغ
ngx_regex_malloc_انجامشده(فراغ)
{
    ngx_regex_pool = عدم;
    ngx_regex_مستقیم_alloc = 0;
}


#إذا (NGX_PCRE2)

ngx_int_t
ngx_regex_compile(ngx_regex_compile_t *rc)
{
    صحيح                     n, errcode;
    حرف                   *p;
    u_char                  errstr[128];
    اندازه_t                  erroff;
    uint32_t                options;
    pcre2_کد             *re;
    ngx_regex_elt_t        *elt;
    pcre2_general_context  *gctx;
    pcre2_compile_context  *cctx;

    إذا (ngx_regex_compile_context == عدم) {
        /*
         * Allocate a compile context if not yet allocated.  This uses
         * direct allocations from heap, so the result can be cached
         * even at runtime.
         */

        ngx_regex_malloc_init(عدم);

        gctx = pcre2_general_context_create(ngx_regex_malloc, ngx_regex_free,
                                            عدم);
        إذا (gctx == عدم) {
            ngx_regex_malloc_انجامشده();
            اذهب nomem;
        }

        cctx = pcre2_compile_context_create(gctx);
        إذا (cctx == عدم) {
            pcre2_general_context_free(gctx);
            ngx_regex_malloc_انجامشده();
            اذهب nomem;
        }

        ngx_regex_compile_context = cctx;

        pcre2_general_context_free(gctx);
        ngx_regex_malloc_انجامشده();
    }

    options = 0;

    إذا (rc->options & NGX_REGEX_CASELESS) {
        options |= PCRE2_CASELESS;
    }

    إذا (rc->options & NGX_REGEX_MULTILINE) {
        options |= PCRE2_MULTILINE;
    }

    إذا (rc->options & ~(NGX_REGEX_CASELESS|NGX_REGEX_MULTILINE)) {
        rc->err.len = ngx_snprintf(rc->err.داده, rc->err.len,
                            "regex \"%V\" compilation failed: invalid options",
                            &rc->الگو)
                      - rc->err.داده;
        أرجع NGX_ERROR;
    }

    ngx_regex_malloc_init(rc->pool);

    re = pcre2_compile(rc->الگو.داده, rc->الگو.len, options,
                       &errcode, &erroff, ngx_regex_compile_context);

    /* ensure that there is no current pool */
    ngx_regex_malloc_انجامشده();

    إذا (re == عدم) {
        pcre2_get_error_پیام(errcode, errstr, 128);

        إذا ((اندازه_t) erroff == rc->الگو.len) {
            rc->err.len = ngx_snprintf(rc->err.داده, rc->err.len,
                              "pcre2_compile() failed: %s in \"%V\"",
                               errstr, &rc->الگو)
                          - rc->err.داده;

        } وإلا {
            rc->err.len = ngx_snprintf(rc->err.داده, rc->err.len,
                              "pcre2_compile() failed: %s in \"%V\" at \"%s\"",
                               errstr, &rc->الگو, rc->الگو.داده + erroff)
                          - rc->err.داده;
        }

        أرجع NGX_ERROR;
    }

    rc->regex = re;

    /* do not study at runtime */

    إذا (ngx_regex_studies != عدم) {
        elt = ngx_list_افزودن(ngx_regex_studies);
        إذا (elt == عدم) {
            اذهب nomem;
        }

        elt->regex = rc->regex;
        elt->name = rc->الگو.داده;
    }

    n = pcre2_الگو_info(re, PCRE2_INFO_CAPTURECOUNT, &rc->captures);
    إذا (n < 0) {
        p = "pcre2_pattern_info(\"%V\", PCRE2_INFO_CAPTURECOUNT) failed: %d";
        اذهب failed;
    }

    إذا (rc->captures == 0) {
        أرجع NGX_OK;
    }

    n = pcre2_الگو_info(re, PCRE2_INFO_NAMECOUNT, &rc->named_captures);
    إذا (n < 0) {
        p = "pcre2_pattern_info(\"%V\", PCRE2_INFO_NAMECOUNT) failed: %d";
        اذهب failed;
    }

    إذا (rc->named_captures == 0) {
        أرجع NGX_OK;
    }

    n = pcre2_الگو_info(re, PCRE2_INFO_NAMEENTRYSIZE, &rc->name_اندازه);
    إذا (n < 0) {
        p = "pcre2_pattern_info(\"%V\", PCRE2_INFO_NAMEENTRYSIZE) failed: %d";
        اذهب failed;
    }

    n = pcre2_الگو_info(re, PCRE2_INFO_NAMETABLE, &rc->نامها);
    إذا (n < 0) {
        p = "pcre2_pattern_info(\"%V\", PCRE2_INFO_NAMETABLE) failed: %d";
        اذهب failed;
    }

    أرجع NGX_OK;

failed:

    rc->err.len = ngx_snprintf(rc->err.داده, rc->err.len, p, &rc->الگو, n)
                  - rc->err.داده;
    أرجع NGX_ERROR;

nomem:

    rc->err.len = ngx_snprintf(rc->err.داده, rc->err.len,
                               "regex \"%V\" compilation failed: no memory",
                               &rc->الگو)
                  - rc->err.داده;
    أرجع NGX_ERROR;
}

#وإلا

ngx_int_t
ngx_regex_compile(ngx_regex_compile_t *rc)
{
    صحيح               n, erroff;
    حرف             *p;
    pcre             *re;
    ثابت حرف       *errstr;
    ngx_uint_t        options;
    ngx_regex_elt_t  *elt;

    options = 0;

    إذا (rc->options & NGX_REGEX_CASELESS) {
        options |= PCRE_CASELESS;
    }

    إذا (rc->options & NGX_REGEX_MULTILINE) {
        options |= PCRE_MULTILINE;
    }

    إذا (rc->options & ~(NGX_REGEX_CASELESS|NGX_REGEX_MULTILINE)) {
        rc->err.len = ngx_snprintf(rc->err.داده, rc->err.len,
                            "regex \"%V\" compilation failed: invalid options",
                            &rc->الگو)
                      - rc->err.داده;
        أرجع NGX_ERROR;
    }

    ngx_regex_malloc_init(rc->pool);

    re = pcre_compile((ثابت حرف *) rc->الگو.داده, (صحيح) options,
                      &errstr, &erroff, عدم);

    /* ensure that there is no current pool */
    ngx_regex_malloc_انجامشده();

    إذا (re == عدم) {
        إذا ((اندازه_t) erroff == rc->الگو.len) {
           rc->err.len = ngx_snprintf(rc->err.داده, rc->err.len,
                              "pcre_compile() failed: %s in \"%V\"",
                               errstr, &rc->الگو)
                         - rc->err.داده;

        } وإلا {
           rc->err.len = ngx_snprintf(rc->err.داده, rc->err.len,
                              "pcre_compile() failed: %s in \"%V\" at \"%s\"",
                               errstr, &rc->الگو, rc->الگو.داده + erroff)
                         - rc->err.داده;
        }

        أرجع NGX_ERROR;
    }

    rc->regex = ngx_pcalloc(rc->pool, حجم(ngx_regex_t));
    إذا (rc->regex == عدم) {
        اذهب nomem;
    }

    rc->regex->کد = re;

    /* do not study at runtime */

    إذا (ngx_regex_studies != عدم) {
        elt = ngx_list_افزودن(ngx_regex_studies);
        إذا (elt == عدم) {
            اذهب nomem;
        }

        elt->regex = rc->regex;
        elt->name = rc->الگو.داده;
    }

    n = pcre_fullinfo(re, عدم, PCRE_INFO_CAPTURECOUNT, &rc->captures);
    إذا (n < 0) {
        p = "pcre_fullinfo(\"%V\", PCRE_INFO_CAPTURECOUNT) failed: %d";
        اذهب failed;
    }

    إذا (rc->captures == 0) {
        أرجع NGX_OK;
    }

    n = pcre_fullinfo(re, عدم, PCRE_INFO_NAMECOUNT, &rc->named_captures);
    إذا (n < 0) {
        p = "pcre_fullinfo(\"%V\", PCRE_INFO_NAMECOUNT) failed: %d";
        اذهب failed;
    }

    إذا (rc->named_captures == 0) {
        أرجع NGX_OK;
    }

    n = pcre_fullinfo(re, عدم, PCRE_INFO_NAMEENTRYSIZE, &rc->name_اندازه);
    إذا (n < 0) {
        p = "pcre_fullinfo(\"%V\", PCRE_INFO_NAMEENTRYSIZE) failed: %d";
        اذهب failed;
    }

    n = pcre_fullinfo(re, عدم, PCRE_INFO_NAMETABLE, &rc->نامها);
    إذا (n < 0) {
        p = "pcre_fullinfo(\"%V\", PCRE_INFO_NAMETABLE) failed: %d";
        اذهب failed;
    }

    أرجع NGX_OK;

failed:

    rc->err.len = ngx_snprintf(rc->err.داده, rc->err.len, p, &rc->الگو, n)
                  - rc->err.داده;
    أرجع NGX_ERROR;

nomem:

    rc->err.len = ngx_snprintf(rc->err.داده, rc->err.len,
                               "regex \"%V\" compilation failed: no memory",
                               &rc->الگو)
                  - rc->err.داده;
    أرجع NGX_ERROR;
}

#پایان_اگر


#إذا (NGX_PCRE2)

ngx_int_t
ngx_regex_exec(ngx_regex_t *re, ngx_str_t *s, صحيح *captures, ngx_uint_t اندازه)
{
    اندازه_t      *ov;
    ngx_int_t    rc;
    ngx_uint_t   n, i;

    /*
     * The pcre2_match() function might allocate memory for backtracking
     * frames, typical allocations are from 40k and above.  So the allocator
     * is configured to do direct allocations from heap during matching.
     */

    ngx_regex_malloc_init(عدم);

    إذا (ngx_regex_تطابق_داده == عدم
        || اندازه > ngx_regex_تطابق_داده_اندازه)
    {
        /*
         * Allocate a match data if not yet allocated or smaller than
         * needed.
         */

        إذا (ngx_regex_تطابق_داده) {
            pcre2_تطابق_داده_free(ngx_regex_تطابق_داده);
        }

        ngx_regex_تطابق_داده_اندازه = اندازه;
        ngx_regex_تطابق_داده = pcre2_تطابق_داده_create(اندازه / 3, عدم);

        إذا (ngx_regex_تطابق_داده == عدم) {
            rc = PCRE2_ERROR_NOMEMORY;
            اذهب failed;
        }
    }

    rc = pcre2_تطابق(re, s->داده, s->len, 0, 0, ngx_regex_تطابق_داده, عدم);

    إذا (rc < 0) {
        اذهب failed;
    }

    n = pcre2_get_ovector_شمار(ngx_regex_تطابق_داده);
    ov = pcre2_get_ovector_اشاره_گر(ngx_regex_تطابق_داده);

    إذا (n > اندازه / 3) {
        n = اندازه / 3;
    }

    لكل (i = 0; i < n; i++) {
        captures[i * 2] = ov[i * 2];
        captures[i * 2 + 1] = ov[i * 2 + 1];
    }

failed:

    ngx_regex_malloc_انجامشده();

    أرجع rc;
}

#وإلا

ngx_int_t
ngx_regex_exec(ngx_regex_t *re, ngx_str_t *s, صحيح *captures, ngx_uint_t اندازه)
{
    أرجع pcre_exec(re->کد, re->extra, (ثابت حرف *) s->داده, s->len,
                     0, 0, captures, اندازه);
}

#پایان_اگر


ngx_int_t
ngx_regex_exec_array(ngx_array_t *a, ngx_str_t *s, ngx_log_t *لگاریتم)
{
    ngx_int_t         n;
    ngx_uint_t        i;
    ngx_regex_elt_t  *re;

    re = a->elts;

    لكل (i = 0; i < a->nelts; i++) {

        n = ngx_regex_exec(re[i].regex, s, عدم, 0);

        إذا (n == NGX_REGEX_NO_MATCHED) {
            استمر;
        }

        إذا (n < 0) {
            ngx_log_error(NGX_LOG_ALERT, لگاریتم, 0,
                          ngx_regex_exec_n " failed: %i on \"%V\" using \"%s\"",
                          n, s, re[i].name);
            أرجع NGX_ERROR;
        }

        /*تطابق*/

        أرجع NGX_OK;
    }

    أرجع NGX_DECLINED;
}


#إذا (NGX_PCRE2)

ساكن فراغ * ngx_libc_cdecl
ngx_regex_malloc(اندازه_t اندازه, فراغ *داده)
{
    إذا (ngx_regex_pool) {
        أرجع ngx_palloc(ngx_regex_pool, اندازه);
    }

    إذا (ngx_regex_مستقیم_alloc) {
        أرجع ngx_alloc(اندازه, ngx_چرخه->لگاریتم);
    }

    أرجع عدم;
}


ساكن فراغ ngx_libc_cdecl
ngx_regex_free(فراغ *p, فراغ *داده)
{
    إذا (ngx_regex_مستقیم_alloc) {
        ngx_free(p);
    }

    أرجع;
}

#وإلا

ساكن فراغ * ngx_libc_cdecl
ngx_regex_malloc(اندازه_t اندازه)
{
    إذا (ngx_regex_pool) {
        أرجع ngx_palloc(ngx_regex_pool, اندازه);
    }

    أرجع عدم;
}


ساكن فراغ ngx_libc_cdecl
ngx_regex_free(فراغ *p)
{
    أرجع;
}

#پایان_اگر


ساكن فراغ
ngx_regex_cleanup(فراغ *داده)
{
#إذا (NGX_PCRE2 || NGX_HAVE_PCRE_JIT)
    ngx_regex_conf_t *rcf = داده;

    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_regex_elt_t  *elts;

    part = &rcf->studies->part;
    elts = part->elts;

    لكل (i = 0; /* void */ ; i++) {

        إذا (i >= part->nelts) {
            إذا (part->بعدی == عدم) {
                اكسر;
            }

            part = part->بعدی;
            elts = part->elts;
            i = 0;
        }

        /*
         * The PCRE JIT compiler uses mmap for its executable codes, so we
         * have to explicitly call the pcre_free_study() function to free
         * this memory.  In PCRE2, we call the pcre2_code_free() function
         * for the same reason.
         */

#إذا (NGX_PCRE2)
        pcre2_کد_free(elts[i].regex);
#وإلا
        إذا (elts[i].regex->extra != عدم) {
            pcre_free_study(elts[i].regex->extra);
        }
#پایان_اگر
    }
#پایان_اگر

    /*
     * On configuration parsing errors ngx_regex_module_init() will not
     * be called.  Make sure ngx_regex_studies is properly cleared anyway.
     */

    ngx_regex_studies = عدم;

#إذا (NGX_PCRE2)

    /*
     * Free compile context and match data.  If needed at runtime by
     * the new cycle, these will be re-allocated.
     */

    ngx_regex_malloc_init(عدم);

    إذا (ngx_regex_compile_context) {
        pcre2_compile_context_free(ngx_regex_compile_context);
        ngx_regex_compile_context = عدم;
    }

    إذا (ngx_regex_تطابق_داده) {
        pcre2_تطابق_داده_free(ngx_regex_تطابق_داده);
        ngx_regex_تطابق_داده = عدم;
        ngx_regex_تطابق_داده_اندازه = 0;
    }

    ngx_regex_malloc_انجامشده();

#پایان_اگر
}


ساكن ngx_int_t
ngx_regex_module_init(ngx_چرخه_t *چرخه)
{
    صحيح                opt;
#إذا !(NGX_PCRE2)
    ثابت حرف        *errstr;
#پایان_اگر
    ngx_uint_t         i;
    ngx_list_part_t   *part;
    ngx_regex_elt_t   *elts;
    ngx_regex_conf_t  *rcf;

    opt = 0;

    rcf = (ngx_regex_conf_t *) ngx_get_conf(چرخه->conf_ctx, ngx_regex_module);

#إذا (NGX_PCRE2 || NGX_HAVE_PCRE_JIT)

    إذا (rcf->pcre_jit) {
#إذا (NGX_PCRE2)
        opt = 1;
#وإلا
        opt = PCRE_STUDY_JIT_COMPILE;
#پایان_اگر
    }

#پایان_اگر

    ngx_regex_malloc_init(چرخه->pool);

    part = &rcf->studies->part;
    elts = part->elts;

    لكل (i = 0; /* void */ ; i++) {

        إذا (i >= part->nelts) {
            إذا (part->بعدی == عدم) {
                اكسر;
            }

            part = part->بعدی;
            elts = part->elts;
            i = 0;
        }

#إذا (NGX_PCRE2)

        إذا (opt) {
            صحيح  n;

            n = pcre2_jit_compile(elts[i].regex, PCRE2_JIT_COMPLETE);

            إذا (n != 0) {
                ngx_log_error(NGX_LOG_INFO, چرخه->لگاریتم, 0,
                              "pcre2_jit_compile() failed: %d in \"%s\", "
                              "ignored",
                              n, elts[i].name);
            }
        }

#وإلا

        elts[i].regex->extra = pcre_study(elts[i].regex->کد, opt, &errstr);

        إذا (errstr != عدم) {
            ngx_log_error(NGX_LOG_ALERT, چرخه->لگاریتم, 0,
                          "pcre_study() failed: %s in \"%s\"",
                          errstr, elts[i].name);
        }

#إذا (NGX_HAVE_PCRE_JIT)
        إذا (opt & PCRE_STUDY_JIT_COMPILE) {
            صحيح jit, n;

            jit = 0;
            n = pcre_fullinfo(elts[i].regex->کد, elts[i].regex->extra,
                              PCRE_INFO_JIT, &jit);

            إذا (n != 0 || jit != 1) {
                ngx_log_error(NGX_LOG_INFO, چرخه->لگاریتم, 0,
                              "JIT compiler does not support pattern: \"%s\"",
                              elts[i].name);
            }
        }
#پایان_اگر
#پایان_اگر
    }

    ngx_regex_malloc_انجامشده();

    ngx_regex_studies = عدم;

    أرجع NGX_OK;
}


ساكن فراغ *
ngx_regex_create_conf(ngx_چرخه_t *چرخه)
{
    ngx_regex_conf_t    *rcf;
    ngx_pool_cleanup_t  *cln;

    rcf = ngx_pcalloc(چرخه->pool, حجم(ngx_regex_conf_t));
    إذا (rcf == عدم) {
        أرجع عدم;
    }

    rcf->pcre_jit = NGX_CONF_UNSET;

    cln = ngx_pool_cleanup_جمع_کن(چرخه->pool, 0);
    إذا (cln == عدم) {
        أرجع عدم;
    }

    rcf->studies = ngx_list_create(چرخه->pool, 8, حجم(ngx_regex_elt_t));
    إذا (rcf->studies == عدم) {
        أرجع عدم;
    }

    cln->handler = ngx_regex_cleanup;
    cln->داده = rcf;

    ngx_regex_studies = rcf->studies;

    أرجع rcf;
}


ساكن حرف *
ngx_regex_init_conf(ngx_چرخه_t *چرخه, فراغ *conf)
{
    ngx_regex_conf_t *rcf = conf;

    ngx_conf_init_مقدار(rcf->pcre_jit, 0);

    أرجع NGX_CONF_OK;
}


ساكن حرف *
ngx_regex_pcre_jit(ngx_conf_t *cf, فراغ *post, فراغ *داده)
{
    ngx_flag_t  *fp = داده;

    إذا (*fp == 0) {
        أرجع NGX_CONF_OK;
    }

#إذا (NGX_PCRE2)
    {
    صحيح       r;
    uint32_t  jit;

    jit = 0;
    r = pcre2_config(PCRE2_CONFIG_JIT, &jit);

    إذا (r != 0 || jit != 1) {
        ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                           "PCRE2 library does not support JIT");
        *fp = 0;
    }
    }
#وإلاإذا (NGX_HAVE_PCRE_JIT)
    {
    صحيح  jit, r;

    jit = 0;
    r = pcre_config(PCRE_CONFIG_JIT, &jit);

    إذا (r != 0 || jit != 1) {
        ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                           "PCRE library does not support JIT");
        *fp = 0;
    }
    }
#وإلا
    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "nginx was built without PCRE JIT support");
    *fp = 0;
#پایان_اگر

    أرجع NGX_CONF_OK;
}
