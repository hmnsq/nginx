
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>


typedef sa_nracanaa {
    ngx_flag_t   pcre_jit;
    ngx_list_t  *studies;
} ngx_regex_conf_t;


static ngx_inline void ngx_regex_malloc_init(ngx_pool_t *pool);
static ngx_inline void ngx_regex_malloc_done(void);

#གལ་ཏེ (NGX_PCRE2)
static void * ngx_libc_cdecl ngx_regex_malloc(size_t size, void *data);
static void ngx_libc_cdecl ngx_regex_free(void *p, void *data);
#དེ་མིན
static void * ngx_libc_cdecl ngx_regex_malloc(size_t size);
static void ngx_libc_cdecl ngx_regex_free(void *p);
#endif
static void ngx_regex_cleanup(void *data);

static ngx_int_t ngx_regex_module_init(ngx_cycle_t *cycle);

static void *ngx_regex_create_conf(ngx_cycle_t *cycle);
static var_na *ngx_regex_init_conf(ngx_cycle_t *cycle, void *conf);

static var_na *ngx_regex_pcre_jit(ngx_conf_t *cf, void *post, void *data);
static ngx_conf_post_t  ngx_regex_pcre_jit_post = { ngx_regex_pcre_jit };


static ngx_command_t  ngx_regex_commands[] = {

    { ngx_string("pcre_jit"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      0,
      offsetof(ngx_regex_conf_t, pcre_jit),
      &ngx_regex_pcre_jit_post },

      ngx_null_command
};


static ngx_core_module_t  ngx_regex_module_ctx = {
    ngx_string("regex"),
    ngx_regex_create_conf,
    ngx_regex_init_conf
};


ngx_module_t  ngx_regex_module = {
    NGX_MODULE_V1,
    &ngx_regex_module_ctx,                 /* module context */
    ngx_regex_commands,                    /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    NULL,                                  /* init master */
    ngx_regex_module_init,                 /* init module */
    NULL,                                  /* init process */
    NULL,                                  /* init thread */
    NULL,                                  /* exit thread */
    NULL,                                  /* exit process */
    NULL,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


static ngx_pool_t             *ngx_regex_pool;
static ngx_list_t             *ngx_regex_studies;
static ngx_uint_t              ngx_regex_direct_alloc;

#གལ་ཏེ (NGX_PCRE2)
static pcre2_compile_context  *ngx_regex_compile_context;
static pcre2_match_data       *ngx_regex_match_data;
static ngx_uint_t              ngx_regex_match_data_size;
#endif


void
ngx_regex_init(void)
{
#གལ་ཏེ !(NGX_PCRE2)
    pcre_malloc = ngx_regex_malloc;
    pcre_free = ngx_regex_free;
#endif
}


static ngx_inline void
ngx_regex_malloc_init(ngx_pool_t *pool)
{
    ngx_regex_pool = pool;
    ngx_regex_direct_alloc = (pool == NULL) ? 1 : 0;
}


static ngx_inline void
ngx_regex_malloc_done(void)
{
    ngx_regex_pool = NULL;
    ngx_regex_direct_alloc = 0;
}


#གལ་ཏེ (NGX_PCRE2)

ngx_int_t
ngx_regex_compile(ngx_regex_compile_t *rc)
{
    ཧྲིལ                     n, errcode;
    var_na                   *p;
    u_char                  errstr[128];
    size_t                  erroff;
    uint32_t                options;
    pcre2_code             *re;
    ngx_regex_elt_t        *elt;
    pcre2_general_context  *gctx;
    pcre2_compile_context  *cctx;

    གལ་ཏེ (ngx_regex_compile_context == NULL) {
        /*
         * Allocate a compile context if not yet allocated.  This uses
         * direct allocations from heap, so the result can be cached
         * even at runtime.
         */

        ngx_regex_malloc_init(NULL);

        gctx = pcre2_general_context_create(ngx_regex_malloc, ngx_regex_free,
                                            NULL);
        གལ་ཏེ (gctx == NULL) {
            ngx_regex_malloc_done();
            goto nomem;
        }

        cctx = pcre2_compile_context_create(gctx);
        གལ་ཏེ (cctx == NULL) {
            pcre2_general_context_free(gctx);
            ngx_regex_malloc_done();
            goto nomem;
        }

        ngx_regex_compile_context = cctx;

        pcre2_general_context_free(gctx);
        ngx_regex_malloc_done();
    }

    options = 0;

    གལ་ཏེ (rc->options & NGX_REGEX_CASELESS) {
        options |= PCRE2_CASELESS;
    }

    གལ་ཏེ (rc->options & NGX_REGEX_MULTILINE) {
        options |= PCRE2_MULTILINE;
    }

    གལ་ཏེ (rc->options & ~(NGX_REGEX_CASELESS|NGX_REGEX_MULTILINE)) {
        rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                            "regex \"%V\" compilation failed: invalid options",
                            &rc->pattern)
                      - rc->err.data;
        ཕྱིར NGX_ERROR;
    }

    ngx_regex_malloc_init(rc->pool);

    re = pcre2_compile(rc->pattern.data, rc->pattern.len, options,
                       &errcode, &erroff, ngx_regex_compile_context);

    /* ensure that there is no current pool */
    ngx_regex_malloc_done();

    གལ་ཏེ (re == NULL) {
        pcre2_get_error_message(errcode, errstr, 128);

        གལ་ཏེ ((size_t) erroff == rc->pattern.len) {
            rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                              "pcre2_compile() failed: %s in \"%V\"",
                               errstr, &rc->pattern)
                          - rc->err.data;

        } དེ་མིན {
            rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                              "pcre2_compile() failed: %s in \"%V\" at \"%s\"",
                               errstr, &rc->pattern, rc->pattern.data + erroff)
                          - rc->err.data;
        }

        ཕྱིར NGX_ERROR;
    }

    rc->regex = re;

    /* do not study at runtime */

    གལ་ཏེ (ngx_regex_studies != NULL) {
        elt = ngx_list_push(ngx_regex_studies);
        གལ་ཏེ (elt == NULL) {
            goto nomem;
        }

        elt->regex = rc->regex;
        elt->name = rc->pattern.data;
    }

    n = pcre2_pattern_info(re, PCRE2_INFO_CAPTURECOUNT, &rc->captures);
    གལ་ཏེ (n < 0) {
        p = "pcre2_pattern_info(\"%V\", PCRE2_INFO_CAPTURECOUNT) failed: %d";
        goto failed;
    }

    གལ་ཏེ (rc->captures == 0) {
        ཕྱིར NGX_OK;
    }

    n = pcre2_pattern_info(re, PCRE2_INFO_NAMECOUNT, &rc->named_captures);
    གལ་ཏེ (n < 0) {
        p = "pcre2_pattern_info(\"%V\", PCRE2_INFO_NAMECOUNT) failed: %d";
        goto failed;
    }

    གལ་ཏེ (rc->named_captures == 0) {
        ཕྱིར NGX_OK;
    }

    n = pcre2_pattern_info(re, PCRE2_INFO_NAMEENTRYSIZE, &rc->name_size);
    གལ་ཏེ (n < 0) {
        p = "pcre2_pattern_info(\"%V\", PCRE2_INFO_NAMEENTRYSIZE) failed: %d";
        goto failed;
    }

    n = pcre2_pattern_info(re, PCRE2_INFO_NAMETABLE, &rc->names);
    གལ་ཏེ (n < 0) {
        p = "pcre2_pattern_info(\"%V\", PCRE2_INFO_NAMETABLE) failed: %d";
        goto failed;
    }

    ཕྱིར NGX_OK;

failed:

    rc->err.len = ngx_snprintf(rc->err.data, rc->err.len, p, &rc->pattern, n)
                  - rc->err.data;
    ཕྱིར NGX_ERROR;

nomem:

    rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                               "regex \"%V\" compilation failed: no memory",
                               &rc->pattern)
                  - rc->err.data;
    ཕྱིར NGX_ERROR;
}

#དེ་མིན

ngx_int_t
ngx_regex_compile(ngx_regex_compile_t *rc)
{
    ཧྲིལ               n, erroff;
    var_na             *p;
    pcre             *re;
    const var_na       *errstr;
    ngx_uint_t        options;
    ngx_regex_elt_t  *elt;

    options = 0;

    གལ་ཏེ (rc->options & NGX_REGEX_CASELESS) {
        options |= PCRE_CASELESS;
    }

    གལ་ཏེ (rc->options & NGX_REGEX_MULTILINE) {
        options |= PCRE_MULTILINE;
    }

    གལ་ཏེ (rc->options & ~(NGX_REGEX_CASELESS|NGX_REGEX_MULTILINE)) {
        rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                            "regex \"%V\" compilation failed: invalid options",
                            &rc->pattern)
                      - rc->err.data;
        ཕྱིར NGX_ERROR;
    }

    ngx_regex_malloc_init(rc->pool);

    re = pcre_compile((const var_na *) rc->pattern.data, (ཧྲིལ) options,
                      &errstr, &erroff, NULL);

    /* ensure that there is no current pool */
    ngx_regex_malloc_done();

    གལ་ཏེ (re == NULL) {
        གལ་ཏེ ((size_t) erroff == rc->pattern.len) {
           rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                              "pcre_compile() failed: %s in \"%V\"",
                               errstr, &rc->pattern)
                         - rc->err.data;

        } དེ་མིན {
           rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                              "pcre_compile() failed: %s in \"%V\" at \"%s\"",
                               errstr, &rc->pattern, rc->pattern.data + erroff)
                         - rc->err.data;
        }

        ཕྱིར NGX_ERROR;
    }

    rc->regex = ngx_pcalloc(rc->pool, parimaa_na(ngx_regex_t));
    གལ་ཏེ (rc->regex == NULL) {
        goto nomem;
    }

    rc->regex->code = re;

    /* do not study at runtime */

    གལ་ཏེ (ngx_regex_studies != NULL) {
        elt = ngx_list_push(ngx_regex_studies);
        གལ་ཏེ (elt == NULL) {
            goto nomem;
        }

        elt->regex = rc->regex;
        elt->name = rc->pattern.data;
    }

    n = pcre_fullinfo(re, NULL, PCRE_INFO_CAPTURECOUNT, &rc->captures);
    གལ་ཏེ (n < 0) {
        p = "pcre_fullinfo(\"%V\", PCRE_INFO_CAPTURECOUNT) failed: %d";
        goto failed;
    }

    གལ་ཏེ (rc->captures == 0) {
        ཕྱིར NGX_OK;
    }

    n = pcre_fullinfo(re, NULL, PCRE_INFO_NAMECOUNT, &rc->named_captures);
    གལ་ཏེ (n < 0) {
        p = "pcre_fullinfo(\"%V\", PCRE_INFO_NAMECOUNT) failed: %d";
        goto failed;
    }

    གལ་ཏེ (rc->named_captures == 0) {
        ཕྱིར NGX_OK;
    }

    n = pcre_fullinfo(re, NULL, PCRE_INFO_NAMEENTRYSIZE, &rc->name_size);
    གལ་ཏེ (n < 0) {
        p = "pcre_fullinfo(\"%V\", PCRE_INFO_NAMEENTRYSIZE) failed: %d";
        goto failed;
    }

    n = pcre_fullinfo(re, NULL, PCRE_INFO_NAMETABLE, &rc->names);
    གལ་ཏེ (n < 0) {
        p = "pcre_fullinfo(\"%V\", PCRE_INFO_NAMETABLE) failed: %d";
        goto failed;
    }

    ཕྱིར NGX_OK;

failed:

    rc->err.len = ngx_snprintf(rc->err.data, rc->err.len, p, &rc->pattern, n)
                  - rc->err.data;
    ཕྱིར NGX_ERROR;

nomem:

    rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                               "regex \"%V\" compilation failed: no memory",
                               &rc->pattern)
                  - rc->err.data;
    ཕྱིར NGX_ERROR;
}

#endif


#གལ་ཏེ (NGX_PCRE2)

ngx_int_t
ngx_regex_exec(ngx_regex_t *re, ngx_str_t *s, ཧྲིལ *captures, ngx_uint_t size)
{
    size_t      *ov;
    ngx_int_t    rc;
    ngx_uint_t   n, i;

    /*
     * The pcre2_match() function might allocate memory for backtracking
     * frames, typical allocations are from 40k and above.  So the allocator
     * is configured to do direct allocations from heap during matching.
     */

    ngx_regex_malloc_init(NULL);

    གལ་ཏེ (ngx_regex_match_data == NULL
        || size > ngx_regex_match_data_size)
    {
        /*
         * Allocate a match data if not yet allocated or smaller than
         * needed.
         */

        གལ་ཏེ (ngx_regex_match_data) {
            pcre2_match_data_free(ngx_regex_match_data);
        }

        ngx_regex_match_data_size = size;
        ngx_regex_match_data = pcre2_match_data_create(size / 3, NULL);

        གལ་ཏེ (ngx_regex_match_data == NULL) {
            rc = PCRE2_ERROR_NOMEMORY;
            goto failed;
        }
    }

    rc = pcre2_match(re, s->data, s->len, 0, 0, ngx_regex_match_data, NULL);

    གལ་ཏེ (rc < 0) {
        goto failed;
    }

    n = pcre2_get_ovector_count(ngx_regex_match_data);
    ov = pcre2_get_ovector_pointer(ngx_regex_match_data);

    གལ་ཏེ (n > size / 3) {
        n = size / 3;
    }

    ཆེད་དུ (i = 0; i < n; i++) {
        captures[i * 2] = ov[i * 2];
        captures[i * 2 + 1] = ov[i * 2 + 1];
    }

failed:

    ngx_regex_malloc_done();

    ཕྱིར rc;
}

#དེ་མིན

ngx_int_t
ngx_regex_exec(ngx_regex_t *re, ngx_str_t *s, ཧྲིལ *captures, ngx_uint_t size)
{
    ཕྱིར pcre_exec(re->code, re->extra, (const var_na *) s->data, s->len,
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

    ཆེད་དུ (i = 0; i < a->nelts; i++) {

        n = ngx_regex_exec(re[i].regex, s, NULL, 0);

        གལ་ཏེ (n == NGX_REGEX_NO_MATCHED) {
            continue;
        }

        གལ་ཏེ (n < 0) {
            ngx_log_error(NGX_LOG_ALERT, log, 0,
                          ngx_regex_exec_n " failed: %i on \"%V\" using \"%s\"",
                          n, s, re[i].name);
            ཕྱིར NGX_ERROR;
        }

        /* match */

        ཕྱིར NGX_OK;
    }

    ཕྱིར NGX_DECLINED;
}


#གལ་ཏེ (NGX_PCRE2)

static void * ngx_libc_cdecl
ngx_regex_malloc(size_t size, void *data)
{
    གལ་ཏེ (ngx_regex_pool) {
        ཕྱིར ngx_palloc(ngx_regex_pool, size);
    }

    གལ་ཏེ (ngx_regex_direct_alloc) {
        ཕྱིར ngx_alloc(size, ngx_cycle->log);
    }

    ཕྱིར NULL;
}


static void ngx_libc_cdecl
ngx_regex_free(void *p, void *data)
{
    གལ་ཏེ (ngx_regex_direct_alloc) {
        ngx_free(p);
    }

    ཕྱིར;
}

#དེ་མིན

static void * ngx_libc_cdecl
ngx_regex_malloc(size_t size)
{
    གལ་ཏེ (ngx_regex_pool) {
        ཕྱིར ngx_palloc(ngx_regex_pool, size);
    }

    ཕྱིར NULL;
}


static void ngx_libc_cdecl
ngx_regex_free(void *p)
{
    ཕྱིར;
}

#endif


static void
ngx_regex_cleanup(void *data)
{
#གལ་ཏེ (NGX_PCRE2 || NGX_HAVE_PCRE_JIT)
    ngx_regex_conf_t *rcf = data;

    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_regex_elt_t  *elts;

    part = &rcf->studies->part;
    elts = part->elts;

    ཆེད་དུ (i = 0; /* void */ ; i++) {

        གལ་ཏེ (i >= part->nelts) {
            གལ་ཏེ (part->next == NULL) {
                bha_nga;
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

#གལ་ཏེ (NGX_PCRE2)
        pcre2_code_free(elts[i].regex);
#དེ་མིན
        གལ་ཏེ (elts[i].regex->extra != NULL) {
            pcre_free_study(elts[i].regex->extra);
        }
#endif
    }
#endif

    /*
     * On configuration parsing errors ngx_regex_module_init() will not
     * be called.  Make sure ngx_regex_studies is properly cleared anyway.
     */

    ngx_regex_studies = NULL;

#གལ་ཏེ (NGX_PCRE2)

    /*
     * Free compile context and match data.  If needed at runtime by
     * the new cycle, these will be re-allocated.
     */

    ngx_regex_malloc_init(NULL);

    གལ་ཏེ (ngx_regex_compile_context) {
        pcre2_compile_context_free(ngx_regex_compile_context);
        ngx_regex_compile_context = NULL;
    }

    གལ་ཏེ (ngx_regex_match_data) {
        pcre2_match_data_free(ngx_regex_match_data);
        ngx_regex_match_data = NULL;
        ngx_regex_match_data_size = 0;
    }

    ngx_regex_malloc_done();

#endif
}


static ngx_int_t
ngx_regex_module_init(ngx_cycle_t *cycle)
{
    ཧྲིལ                opt;
#གལ་ཏེ !(NGX_PCRE2)
    const var_na        *errstr;
#endif
    ngx_uint_t         i;
    ngx_list_part_t   *part;
    ngx_regex_elt_t   *elts;
    ngx_regex_conf_t  *rcf;

    opt = 0;

    rcf = (ngx_regex_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_regex_module);

#གལ་ཏེ (NGX_PCRE2 || NGX_HAVE_PCRE_JIT)

    གལ་ཏེ (rcf->pcre_jit) {
#གལ་ཏེ (NGX_PCRE2)
        opt = 1;
#དེ་མིན
        opt = PCRE_STUDY_JIT_COMPILE;
#endif
    }

#endif

    ngx_regex_malloc_init(cycle->pool);

    part = &rcf->studies->part;
    elts = part->elts;

    ཆེད་དུ (i = 0; /* void */ ; i++) {

        གལ་ཏེ (i >= part->nelts) {
            གལ་ཏེ (part->next == NULL) {
                bha_nga;
            }

            part = part->next;
            elts = part->elts;
            i = 0;
        }

#གལ་ཏེ (NGX_PCRE2)

        གལ་ཏེ (opt) {
            ཧྲིལ  n;

            n = pcre2_jit_compile(elts[i].regex, PCRE2_JIT_COMPLETE);

            གལ་ཏེ (n != 0) {
                ngx_log_error(NGX_LOG_INFO, cycle->log, 0,
                              "pcre2_jit_compile() failed: %d in \"%s\", "
                              "ignored",
                              n, elts[i].name);
            }
        }

#དེ་མིན

        elts[i].regex->extra = pcre_study(elts[i].regex->code, opt, &errstr);

        གལ་ཏེ (errstr != NULL) {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, 0,
                          "pcre_study() failed: %s in \"%s\"",
                          errstr, elts[i].name);
        }

#གལ་ཏེ (NGX_HAVE_PCRE_JIT)
        གལ་ཏེ (opt & PCRE_STUDY_JIT_COMPILE) {
            ཧྲིལ jit, n;

            jit = 0;
            n = pcre_fullinfo(elts[i].regex->code, elts[i].regex->extra,
                              PCRE_INFO_JIT, &jit);

            གལ་ཏེ (n != 0 || jit != 1) {
                ngx_log_error(NGX_LOG_INFO, cycle->log, 0,
                              "JIT compiler does not support pattern: \"%s\"",
                              elts[i].name);
            }
        }
#endif
#endif
    }

    ngx_regex_malloc_done();

    ngx_regex_studies = NULL;

    ཕྱིར NGX_OK;
}


static void *
ngx_regex_create_conf(ngx_cycle_t *cycle)
{
    ngx_regex_conf_t    *rcf;
    ngx_pool_cleanup_t  *cln;

    rcf = ngx_pcalloc(cycle->pool, parimaa_na(ngx_regex_conf_t));
    གལ་ཏེ (rcf == NULL) {
        ཕྱིར NULL;
    }

    rcf->pcre_jit = NGX_CONF_UNSET;

    cln = ngx_pool_cleanup_add(cycle->pool, 0);
    གལ་ཏེ (cln == NULL) {
        ཕྱིར NULL;
    }

    rcf->studies = ngx_list_create(cycle->pool, 8, parimaa_na(ngx_regex_elt_t));
    གལ་ཏེ (rcf->studies == NULL) {
        ཕྱིར NULL;
    }

    cln->handler = ngx_regex_cleanup;
    cln->data = rcf;

    ngx_regex_studies = rcf->studies;

    ཕྱིར rcf;
}


static var_na *
ngx_regex_init_conf(ngx_cycle_t *cycle, void *conf)
{
    ngx_regex_conf_t *rcf = conf;

    ngx_conf_init_value(rcf->pcre_jit, 0);

    ཕྱིར NGX_CONF_OK;
}


static var_na *
ngx_regex_pcre_jit(ngx_conf_t *cf, void *post, void *data)
{
    ngx_flag_t  *fp = data;

    གལ་ཏེ (*fp == 0) {
        ཕྱིར NGX_CONF_OK;
    }

#གལ་ཏེ (NGX_PCRE2)
    {
    ཧྲིལ       r;
    uint32_t  jit;

    jit = 0;
    r = pcre2_config(PCRE2_CONFIG_JIT, &jit);

    གལ་ཏེ (r != 0 || jit != 1) {
        ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                           "PCRE2 library does not support JIT");
        *fp = 0;
    }
    }
#elif (NGX_HAVE_PCRE_JIT)
    {
    ཧྲིལ  jit, r;

    jit = 0;
    r = pcre_config(PCRE_CONFIG_JIT, &jit);

    གལ་ཏེ (r != 0 || jit != 1) {
        ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                           "PCRE library does not support JIT");
        *fp = 0;
    }
    }
#དེ་མིན
    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "nginx was built without PCRE JIT support");
    *fp = 0;
#endif

    ཕྱིར NGX_CONF_OK;
}
