
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#شامل <ngx_config.h>
#شامل <ngx_core.h>


قسمتعریف ساخت {
    ngx_flag_t   pcre_jit;
    ngx_list_t  *studies;
} ngx_regex_conf_t;


ساکن ngx_inline خالی ngx_regex_malloc_init(ngx_pool_t *pool);
ساکن ngx_inline خالی ngx_regex_malloc_مکمل(خالی);

#اگر (NGX_PCRE2)
ساکن خالی * ngx_libc_cdecl ngx_regex_malloc(سائز_t سائز, خالی *ڈیٹا);
ساکن خالی ngx_libc_cdecl ngx_regex_free(خالی *p, خالی *ڈیٹا);
#ورنہ
ساکن خالی * ngx_libc_cdecl ngx_regex_malloc(سائز_t سائز);
ساکن خالی ngx_libc_cdecl ngx_regex_free(خالی *p);
#اگر_ختم
ساکن خالی ngx_regex_cleanup(خالی *ڈیٹا);

ساکن ngx_int_t ngx_regex_module_init(ngx_چکر_t *چکر);

ساکن خالی *ngx_regex_create_conf(ngx_چکر_t *چکر);
ساکن حرف *ngx_regex_init_conf(ngx_چکر_t *چکر, خالی *conf);

ساکن حرف *ngx_regex_pcre_jit(ngx_conf_t *cf, خالی *post, خالی *ڈیٹا);
ساکن ngx_conf_post_t  ngx_regex_pcre_jit_post = { ngx_regex_pcre_jit };


ساکن ngx_command_t  ngx_regex_commands[] = {

    { ngx_string("pcre_jit"),
      NGX_MAIN_CONF|NGX_براہ_راست_CONF|NGX_CONF_FLAG,
      ngx_conf_سیٹ_flag_خانہ,
      0,
      offsetof(ngx_regex_conf_t, pcre_jit),
      &ngx_regex_pcre_jit_post },

      ngx_null_command
};


ساکن ngx_core_module_t  ngx_regex_module_ctx = {
    ngx_string("regex"),
    ngx_regex_create_conf,
    ngx_regex_init_conf
};


ngx_module_t  ngx_regex_module = {
    NGX_MODULE_V1,
    &ngx_regex_module_ctx,                 /* module context */
    ngx_regex_commands,                    /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    تہی,                                  /* init master */
    ngx_regex_module_init,                 /* init module */
    تہی,                                  /* init process */
    تہی,                                  /* init thread */
    تہی,                                  /* exit thread */
    تہی,                                  /* exit process */
    تہی,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


ساکن ngx_pool_t             *ngx_regex_pool;
ساکن ngx_list_t             *ngx_regex_studies;
ساکن ngx_uint_t              ngx_regex_براہ_راست_alloc;

#اگر (NGX_PCRE2)
ساکن pcre2_compile_context  *ngx_regex_compile_context;
ساکن pcre2_مطابقت_ڈیٹا       *ngx_regex_مطابقت_ڈیٹا;
ساکن ngx_uint_t              ngx_regex_مطابقت_ڈیٹا_سائز;
#اگر_ختم


خالی
ngx_regex_init(خالی)
{
#اگر !(NGX_PCRE2)
    pcre_malloc = ngx_regex_malloc;
    pcre_free = ngx_regex_free;
#اگر_ختم
}


ساکن ngx_inline خالی
ngx_regex_malloc_init(ngx_pool_t *pool)
{
    ngx_regex_pool = pool;
    ngx_regex_براہ_راست_alloc = (pool == تہی) ? 1 : 0;
}


ساکن ngx_inline خالی
ngx_regex_malloc_مکمل(خالی)
{
    ngx_regex_pool = تہی;
    ngx_regex_براہ_راست_alloc = 0;
}


#اگر (NGX_PCRE2)

ngx_int_t
ngx_regex_compile(ngx_regex_compile_t *rc)
{
    صحیح                     n, errcode;
    حرف                   *p;
    u_char                  errstr[128];
    سائز_t                  erroff;
    uint32_t                options;
    pcre2_رمز             *re;
    ngx_regex_elt_t        *elt;
    pcre2_general_context  *gctx;
    pcre2_compile_context  *cctx;

    اگر (ngx_regex_compile_context == تہی) {
        /*
         * Allocate a compile context if not yet allocated.  This uses
         * direct allocations from heap, so the result can be cached
         * even at runtime.
         */

        ngx_regex_malloc_init(تہی);

        gctx = pcre2_general_context_create(ngx_regex_malloc, ngx_regex_free,
                                            تہی);
        اگر (gctx == تہی) {
            ngx_regex_malloc_مکمل();
            جاؤ nomem;
        }

        cctx = pcre2_compile_context_create(gctx);
        اگر (cctx == تہی) {
            pcre2_general_context_free(gctx);
            ngx_regex_malloc_مکمل();
            جاؤ nomem;
        }

        ngx_regex_compile_context = cctx;

        pcre2_general_context_free(gctx);
        ngx_regex_malloc_مکمل();
    }

    options = 0;

    اگر (rc->options & NGX_REGEX_CASELESS) {
        options |= PCRE2_CASELESS;
    }

    اگر (rc->options & NGX_REGEX_MULTILINE) {
        options |= PCRE2_MULTILINE;
    }

    اگر (rc->options & ~(NGX_REGEX_CASELESS|NGX_REGEX_MULTILINE)) {
        rc->err.len = ngx_snprintf(rc->err.ڈیٹا, rc->err.len,
                            "regex \"%V\" compilation failed: invalid options",
                            &rc->نمونہ)
                      - rc->err.ڈیٹا;
        واپس NGX_ERROR;
    }

    ngx_regex_malloc_init(rc->pool);

    re = pcre2_compile(rc->نمونہ.ڈیٹا, rc->نمونہ.len, options,
                       &errcode, &erroff, ngx_regex_compile_context);

    /* ensure that there is no current pool */
    ngx_regex_malloc_مکمل();

    اگر (re == تہی) {
        pcre2_get_error_پیغام(errcode, errstr, 128);

        اگر ((سائز_t) erroff == rc->نمونہ.len) {
            rc->err.len = ngx_snprintf(rc->err.ڈیٹا, rc->err.len,
                              "pcre2_compile() failed: %s in \"%V\"",
                               errstr, &rc->نمونہ)
                          - rc->err.ڈیٹا;

        } ورنہ {
            rc->err.len = ngx_snprintf(rc->err.ڈیٹا, rc->err.len,
                              "pcre2_compile() failed: %s in \"%V\" at \"%s\"",
                               errstr, &rc->نمونہ, rc->نمونہ.ڈیٹا + erroff)
                          - rc->err.ڈیٹا;
        }

        واپس NGX_ERROR;
    }

    rc->regex = re;

    /* do not study at runtime */

    اگر (ngx_regex_studies != تہی) {
        elt = ngx_list_ڈالو(ngx_regex_studies);
        اگر (elt == تہی) {
            جاؤ nomem;
        }

        elt->regex = rc->regex;
        elt->name = rc->نمونہ.ڈیٹا;
    }

    n = pcre2_نمونہ_info(re, PCRE2_INFO_CAPTURECOUNT, &rc->captures);
    اگر (n < 0) {
        p = "pcre2_pattern_info(\"%V\", PCRE2_INFO_CAPTURECOUNT) failed: %d";
        جاؤ failed;
    }

    اگر (rc->captures == 0) {
        واپس NGX_OK;
    }

    n = pcre2_نمونہ_info(re, PCRE2_INFO_NAMECOUNT, &rc->named_captures);
    اگر (n < 0) {
        p = "pcre2_pattern_info(\"%V\", PCRE2_INFO_NAMECOUNT) failed: %d";
        جاؤ failed;
    }

    اگر (rc->named_captures == 0) {
        واپس NGX_OK;
    }

    n = pcre2_نمونہ_info(re, PCRE2_INFO_NAMEENTRYSIZE, &rc->name_سائز);
    اگر (n < 0) {
        p = "pcre2_pattern_info(\"%V\", PCRE2_INFO_NAMEENTRYSIZE) failed: %d";
        جاؤ failed;
    }

    n = pcre2_نمونہ_info(re, PCRE2_INFO_NAMETABLE, &rc->نام);
    اگر (n < 0) {
        p = "pcre2_pattern_info(\"%V\", PCRE2_INFO_NAMETABLE) failed: %d";
        جاؤ failed;
    }

    واپس NGX_OK;

failed:

    rc->err.len = ngx_snprintf(rc->err.ڈیٹا, rc->err.len, p, &rc->نمونہ, n)
                  - rc->err.ڈیٹا;
    واپس NGX_ERROR;

nomem:

    rc->err.len = ngx_snprintf(rc->err.ڈیٹا, rc->err.len,
                               "regex \"%V\" compilation failed: no memory",
                               &rc->نمونہ)
                  - rc->err.ڈیٹا;
    واپس NGX_ERROR;
}

#ورنہ

ngx_int_t
ngx_regex_compile(ngx_regex_compile_t *rc)
{
    صحیح               n, erroff;
    حرف             *p;
    pcre             *re;
    ثابت حرف       *errstr;
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
        rc->err.len = ngx_snprintf(rc->err.ڈیٹا, rc->err.len,
                            "regex \"%V\" compilation failed: invalid options",
                            &rc->نمونہ)
                      - rc->err.ڈیٹا;
        واپس NGX_ERROR;
    }

    ngx_regex_malloc_init(rc->pool);

    re = pcre_compile((ثابت حرف *) rc->نمونہ.ڈیٹا, (صحیح) options,
                      &errstr, &erroff, تہی);

    /* ensure that there is no current pool */
    ngx_regex_malloc_مکمل();

    اگر (re == تہی) {
        اگر ((سائز_t) erroff == rc->نمونہ.len) {
           rc->err.len = ngx_snprintf(rc->err.ڈیٹا, rc->err.len,
                              "pcre_compile() failed: %s in \"%V\"",
                               errstr, &rc->نمونہ)
                         - rc->err.ڈیٹا;

        } ورنہ {
           rc->err.len = ngx_snprintf(rc->err.ڈیٹا, rc->err.len,
                              "pcre_compile() failed: %s in \"%V\" at \"%s\"",
                               errstr, &rc->نمونہ, rc->نمونہ.ڈیٹا + erroff)
                         - rc->err.ڈیٹا;
        }

        واپس NGX_ERROR;
    }

    rc->regex = ngx_pcalloc(rc->pool, حجم(ngx_regex_t));
    اگر (rc->regex == تہی) {
        جاؤ nomem;
    }

    rc->regex->رمز = re;

    /* do not study at runtime */

    اگر (ngx_regex_studies != تہی) {
        elt = ngx_list_ڈالو(ngx_regex_studies);
        اگر (elt == تہی) {
            جاؤ nomem;
        }

        elt->regex = rc->regex;
        elt->name = rc->نمونہ.ڈیٹا;
    }

    n = pcre_fullinfo(re, تہی, PCRE_INFO_CAPTURECOUNT, &rc->captures);
    اگر (n < 0) {
        p = "pcre_fullinfo(\"%V\", PCRE_INFO_CAPTURECOUNT) failed: %d";
        جاؤ failed;
    }

    اگر (rc->captures == 0) {
        واپس NGX_OK;
    }

    n = pcre_fullinfo(re, تہی, PCRE_INFO_NAMECOUNT, &rc->named_captures);
    اگر (n < 0) {
        p = "pcre_fullinfo(\"%V\", PCRE_INFO_NAMECOUNT) failed: %d";
        جاؤ failed;
    }

    اگر (rc->named_captures == 0) {
        واپس NGX_OK;
    }

    n = pcre_fullinfo(re, تہی, PCRE_INFO_NAMEENTRYSIZE, &rc->name_سائز);
    اگر (n < 0) {
        p = "pcre_fullinfo(\"%V\", PCRE_INFO_NAMEENTRYSIZE) failed: %d";
        جاؤ failed;
    }

    n = pcre_fullinfo(re, تہی, PCRE_INFO_NAMETABLE, &rc->نام);
    اگر (n < 0) {
        p = "pcre_fullinfo(\"%V\", PCRE_INFO_NAMETABLE) failed: %d";
        جاؤ failed;
    }

    واپس NGX_OK;

failed:

    rc->err.len = ngx_snprintf(rc->err.ڈیٹا, rc->err.len, p, &rc->نمونہ, n)
                  - rc->err.ڈیٹا;
    واپس NGX_ERROR;

nomem:

    rc->err.len = ngx_snprintf(rc->err.ڈیٹا, rc->err.len,
                               "regex \"%V\" compilation failed: no memory",
                               &rc->نمونہ)
                  - rc->err.ڈیٹا;
    واپس NGX_ERROR;
}

#اگر_ختم


#اگر (NGX_PCRE2)

ngx_int_t
ngx_regex_exec(ngx_regex_t *re, ngx_str_t *s, صحیح *captures, ngx_uint_t سائز)
{
    سائز_t      *ov;
    ngx_int_t    rc;
    ngx_uint_t   n, i;

    /*
     * The pcre2_match() function might allocate memory for backtracking
     * frames, typical allocations are from 40k and above.  So the allocator
     * is configured to do direct allocations from heap during matching.
     */

    ngx_regex_malloc_init(تہی);

    اگر (ngx_regex_مطابقت_ڈیٹا == تہی
        || سائز > ngx_regex_مطابقت_ڈیٹا_سائز)
    {
        /*
         * Allocate a match data if not yet allocated or smaller than
         * needed.
         */

        اگر (ngx_regex_مطابقت_ڈیٹا) {
            pcre2_مطابقت_ڈیٹا_free(ngx_regex_مطابقت_ڈیٹا);
        }

        ngx_regex_مطابقت_ڈیٹا_سائز = سائز;
        ngx_regex_مطابقت_ڈیٹا = pcre2_مطابقت_ڈیٹا_create(سائز / 3, تہی);

        اگر (ngx_regex_مطابقت_ڈیٹا == تہی) {
            rc = PCRE2_ERROR_NOMEMORY;
            جاؤ failed;
        }
    }

    rc = pcre2_مطابقت(re, s->ڈیٹا, s->len, 0, 0, ngx_regex_مطابقت_ڈیٹا, تہی);

    اگر (rc < 0) {
        جاؤ failed;
    }

    n = pcre2_get_ovector_گنتی(ngx_regex_مطابقت_ڈیٹا);
    ov = pcre2_get_ovector_اشارہ_گر(ngx_regex_مطابقت_ڈیٹا);

    اگر (n > سائز / 3) {
        n = سائز / 3;
    }

    برائے (i = 0; i < n; i++) {
        captures[i * 2] = ov[i * 2];
        captures[i * 2 + 1] = ov[i * 2 + 1];
    }

failed:

    ngx_regex_malloc_مکمل();

    واپس rc;
}

#ورنہ

ngx_int_t
ngx_regex_exec(ngx_regex_t *re, ngx_str_t *s, صحیح *captures, ngx_uint_t سائز)
{
    واپس pcre_exec(re->رمز, re->extra, (ثابت حرف *) s->ڈیٹا, s->len,
                     0, 0, captures, سائز);
}

#اگر_ختم


ngx_int_t
ngx_regex_exec_array(ngx_array_t *a, ngx_str_t *s, ngx_log_t *لوگارتھم)
{
    ngx_int_t         n;
    ngx_uint_t        i;
    ngx_regex_elt_t  *re;

    re = a->elts;

    برائے (i = 0; i < a->nelts; i++) {

        n = ngx_regex_exec(re[i].regex, s, تہی, 0);

        اگر (n == NGX_REGEX_NO_MATCHED) {
            جاری;
        }

        اگر (n < 0) {
            ngx_log_error(NGX_LOG_ALERT, لوگارتھم, 0,
                          ngx_regex_exec_n " failed: %i on \"%V\" using \"%s\"",
                          n, s, re[i].name);
            واپس NGX_ERROR;
        }

        /*مطابقت*/

        واپس NGX_OK;
    }

    واپس NGX_DECLINED;
}


#اگر (NGX_PCRE2)

ساکن خالی * ngx_libc_cdecl
ngx_regex_malloc(سائز_t سائز, خالی *ڈیٹا)
{
    اگر (ngx_regex_pool) {
        واپس ngx_palloc(ngx_regex_pool, سائز);
    }

    اگر (ngx_regex_براہ_راست_alloc) {
        واپس ngx_alloc(سائز, ngx_چکر->لوگارتھم);
    }

    واپس تہی;
}


ساکن خالی ngx_libc_cdecl
ngx_regex_free(خالی *p, خالی *ڈیٹا)
{
    اگر (ngx_regex_براہ_راست_alloc) {
        ngx_free(p);
    }

    واپس;
}

#ورنہ

ساکن خالی * ngx_libc_cdecl
ngx_regex_malloc(سائز_t سائز)
{
    اگر (ngx_regex_pool) {
        واپس ngx_palloc(ngx_regex_pool, سائز);
    }

    واپس تہی;
}


ساکن خالی ngx_libc_cdecl
ngx_regex_free(خالی *p)
{
    واپس;
}

#اگر_ختم


ساکن خالی
ngx_regex_cleanup(خالی *ڈیٹا)
{
#اگر (NGX_PCRE2 || NGX_HAVE_PCRE_JIT)
    ngx_regex_conf_t *rcf = ڈیٹا;

    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_regex_elt_t  *elts;

    part = &rcf->studies->part;
    elts = part->elts;

    برائے (i = 0; /* void */ ; i++) {

        اگر (i >= part->nelts) {
            اگر (part->اگلا == تہی) {
                توڑو;
            }

            part = part->اگلا;
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
        pcre2_رمز_free(elts[i].regex);
#ورنہ
        اگر (elts[i].regex->extra != تہی) {
            pcre_free_study(elts[i].regex->extra);
        }
#اگر_ختم
    }
#اگر_ختم

    /*
     * On configuration parsing errors ngx_regex_module_init() will not
     * be called.  Make sure ngx_regex_studies is properly cleared anyway.
     */

    ngx_regex_studies = تہی;

#اگر (NGX_PCRE2)

    /*
     * Free compile context and match data.  If needed at runtime by
     * the new cycle, these will be re-allocated.
     */

    ngx_regex_malloc_init(تہی);

    اگر (ngx_regex_compile_context) {
        pcre2_compile_context_free(ngx_regex_compile_context);
        ngx_regex_compile_context = تہی;
    }

    اگر (ngx_regex_مطابقت_ڈیٹا) {
        pcre2_مطابقت_ڈیٹا_free(ngx_regex_مطابقت_ڈیٹا);
        ngx_regex_مطابقت_ڈیٹا = تہی;
        ngx_regex_مطابقت_ڈیٹا_سائز = 0;
    }

    ngx_regex_malloc_مکمل();

#اگر_ختم
}


ساکن ngx_int_t
ngx_regex_module_init(ngx_چکر_t *چکر)
{
    صحیح                opt;
#اگر !(NGX_PCRE2)
    ثابت حرف        *errstr;
#اگر_ختم
    ngx_uint_t         i;
    ngx_list_part_t   *part;
    ngx_regex_elt_t   *elts;
    ngx_regex_conf_t  *rcf;

    opt = 0;

    rcf = (ngx_regex_conf_t *) ngx_get_conf(چکر->conf_ctx, ngx_regex_module);

#اگر (NGX_PCRE2 || NGX_HAVE_PCRE_JIT)

    اگر (rcf->pcre_jit) {
#اگر (NGX_PCRE2)
        opt = 1;
#ورنہ
        opt = PCRE_STUDY_JIT_COMPILE;
#اگر_ختم
    }

#اگر_ختم

    ngx_regex_malloc_init(چکر->pool);

    part = &rcf->studies->part;
    elts = part->elts;

    برائے (i = 0; /* void */ ; i++) {

        اگر (i >= part->nelts) {
            اگر (part->اگلا == تہی) {
                توڑو;
            }

            part = part->اگلا;
            elts = part->elts;
            i = 0;
        }

#اگر (NGX_PCRE2)

        اگر (opt) {
            صحیح  n;

            n = pcre2_jit_compile(elts[i].regex, PCRE2_JIT_COMPLETE);

            اگر (n != 0) {
                ngx_log_error(NGX_LOG_INFO, چکر->لوگارتھم, 0,
                              "pcre2_jit_compile() failed: %d in \"%s\", "
                              "ignored",
                              n, elts[i].name);
            }
        }

#ورنہ

        elts[i].regex->extra = pcre_study(elts[i].regex->رمز, opt, &errstr);

        اگر (errstr != تہی) {
            ngx_log_error(NGX_LOG_ALERT, چکر->لوگارتھم, 0,
                          "pcre_study() failed: %s in \"%s\"",
                          errstr, elts[i].name);
        }

#اگر (NGX_HAVE_PCRE_JIT)
        اگر (opt & PCRE_STUDY_JIT_COMPILE) {
            صحیح jit, n;

            jit = 0;
            n = pcre_fullinfo(elts[i].regex->رمز, elts[i].regex->extra,
                              PCRE_INFO_JIT, &jit);

            اگر (n != 0 || jit != 1) {
                ngx_log_error(NGX_LOG_INFO, چکر->لوگارتھم, 0,
                              "JIT compiler does not support pattern: \"%s\"",
                              elts[i].name);
            }
        }
#اگر_ختم
#اگر_ختم
    }

    ngx_regex_malloc_مکمل();

    ngx_regex_studies = تہی;

    واپس NGX_OK;
}


ساکن خالی *
ngx_regex_create_conf(ngx_چکر_t *چکر)
{
    ngx_regex_conf_t    *rcf;
    ngx_pool_cleanup_t  *cln;

    rcf = ngx_pcalloc(چکر->pool, حجم(ngx_regex_conf_t));
    اگر (rcf == تہی) {
        واپس تہی;
    }

    rcf->pcre_jit = NGX_CONF_UNSET;

    cln = ngx_pool_cleanup_جوڑو(چکر->pool, 0);
    اگر (cln == تہی) {
        واپس تہی;
    }

    rcf->studies = ngx_list_create(چکر->pool, 8, حجم(ngx_regex_elt_t));
    اگر (rcf->studies == تہی) {
        واپس تہی;
    }

    cln->handler = ngx_regex_cleanup;
    cln->ڈیٹا = rcf;

    ngx_regex_studies = rcf->studies;

    واپس rcf;
}


ساکن حرف *
ngx_regex_init_conf(ngx_چکر_t *چکر, خالی *conf)
{
    ngx_regex_conf_t *rcf = conf;

    ngx_conf_init_قدر(rcf->pcre_jit, 0);

    واپس NGX_CONF_OK;
}


ساکن حرف *
ngx_regex_pcre_jit(ngx_conf_t *cf, خالی *post, خالی *ڈیٹا)
{
    ngx_flag_t  *fp = ڈیٹا;

    اگر (*fp == 0) {
        واپس NGX_CONF_OK;
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
#وگرنہ_اگر (NGX_HAVE_PCRE_JIT)
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
#ورنہ
    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "nginx was built without PCRE JIT support");
    *fp = 0;
#اگر_ختم

    واپس NGX_CONF_OK;
}
