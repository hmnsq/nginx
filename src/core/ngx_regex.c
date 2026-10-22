
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#અન્તર્ભાવ <ngx_config.h>
#અન્તર્ભાવ <ngx_core.h>


પ્રકારવ્યાખ્યા સંરચના {
    ngx_flag_t   pcre_jit;
    ngx_list_t  *studies;
} ngx_regex_conf_t;


સ્થિર ngx_inline શૂન્ય ngx_regex_malloc_init(ngx_pool_t *pool);
સ્થિર ngx_inline શૂન્ય ngx_regex_malloc_done(શૂન્ય);

#જો (NGX_PCRE2)
સ્થિર શૂન્ય * ngx_libc_cdecl ngx_regex_malloc(size_t size, શૂન્ય *data);
સ્થિર શૂન્ય ngx_libc_cdecl ngx_regex_free(શૂન્ય *p, શૂન્ય *data);
#નહીંતર
સ્થિર શૂન્ય * ngx_libc_cdecl ngx_regex_malloc(size_t size);
સ્થિર શૂન્ય ngx_libc_cdecl ngx_regex_free(શૂન્ય *p);
#endif
સ્થિર શૂન્ય ngx_regex_cleanup(શૂન્ય *data);

સ્થિર ngx_int_t ngx_regex_module_init(ngx_cycle_t *cycle);

સ્થિર શૂન્ય *ngx_regex_create_conf(ngx_cycle_t *cycle);
સ્થિર અક્ષર *ngx_regex_init_conf(ngx_cycle_t *cycle, શૂન્ય *conf);

સ્થિર અક્ષર *ngx_regex_pcre_jit(ngx_conf_t *cf, શૂન્ય *post, શૂન્ય *data);
સ્થિર ngx_conf_post_t  ngx_regex_pcre_jit_post = { ngx_regex_pcre_jit };


સ્થિર ngx_command_t  ngx_regex_commands[] = {

    { ngx_string("pcre_jit"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      0,
      offsetof(ngx_regex_conf_t, pcre_jit),
      &ngx_regex_pcre_jit_post },

      ngx_null_command
};


સ્થિર ngx_core_module_t  ngx_regex_module_ctx = {
    ngx_string("regex"),
    ngx_regex_create_conf,
    ngx_regex_init_conf
};


ngx_module_t  ngx_regex_module = {
    NGX_MODULE_V1,
    &ngx_regex_module_ctx,                 /* module context */
    ngx_regex_commands,                    /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    ખાલી,                                  /* init master */
    ngx_regex_module_init,                 /* init module */
    ખાલી,                                  /* init process */
    ખાલી,                                  /* init thread */
    ખાલી,                                  /* exit thread */
    ખાલી,                                  /* exit process */
    ખાલી,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


સ્થિર ngx_pool_t             *ngx_regex_pool;
સ્થિર ngx_list_t             *ngx_regex_studies;
સ્થિર ngx_uint_t              ngx_regex_direct_alloc;

#જો (NGX_PCRE2)
સ્થિર pcre2_compile_context  *ngx_regex_compile_context;
સ્થિર pcre2_match_data       *ngx_regex_match_data;
સ્થિર ngx_uint_t              ngx_regex_match_data_size;
#endif


શૂન્ય
ngx_regex_init(શૂન્ય)
{
#જો !(NGX_PCRE2)
    pcre_malloc = ngx_regex_malloc;
    pcre_free = ngx_regex_free;
#endif
}


સ્થિર ngx_inline શૂન્ય
ngx_regex_malloc_init(ngx_pool_t *pool)
{
    ngx_regex_pool = pool;
    ngx_regex_direct_alloc = (pool == ખાલી) ? 1 : 0;
}


સ્થિર ngx_inline શૂન્ય
ngx_regex_malloc_done(શૂન્ય)
{
    ngx_regex_pool = ખાલી;
    ngx_regex_direct_alloc = 0;
}


#જો (NGX_PCRE2)

ngx_int_t
ngx_regex_compile(ngx_regex_compile_t *rc)
{
    પૂર્ણાંક                     n, errcode;
    અક્ષર                   *p;
    u_char                  errstr[128];
    size_t                  erroff;
    uint32_t                options;
    pcre2_code             *re;
    ngx_regex_elt_t        *elt;
    pcre2_general_context  *gctx;
    pcre2_compile_context  *cctx;

    જો (ngx_regex_compile_context == ખાલી) {
        /*
         * Allocate a compile context if not yet allocated.  This uses
         * direct allocations from heap, so the result can be cached
         * even at runtime.
         */

        ngx_regex_malloc_init(ખાલી);

        gctx = pcre2_general_context_create(ngx_regex_malloc, ngx_regex_free,
                                            ખાલી);
        જો (gctx == ખાલી) {
            ngx_regex_malloc_done();
            જાઓ nomem;
        }

        cctx = pcre2_compile_context_create(gctx);
        જો (cctx == ખાલી) {
            pcre2_general_context_free(gctx);
            ngx_regex_malloc_done();
            જાઓ nomem;
        }

        ngx_regex_compile_context = cctx;

        pcre2_general_context_free(gctx);
        ngx_regex_malloc_done();
    }

    options = 0;

    જો (rc->options & NGX_REGEX_CASELESS) {
        options |= PCRE2_CASELESS;
    }

    જો (rc->options & NGX_REGEX_MULTILINE) {
        options |= PCRE2_MULTILINE;
    }

    જો (rc->options & ~(NGX_REGEX_CASELESS|NGX_REGEX_MULTILINE)) {
        rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                            "regex \"%V\" compilation failed: invalid options",
                            &rc->pattern)
                      - rc->err.data;
        પાછા NGX_ERROR;
    }

    ngx_regex_malloc_init(rc->pool);

    re = pcre2_compile(rc->pattern.data, rc->pattern.len, options,
                       &errcode, &erroff, ngx_regex_compile_context);

    /* ensure that there is no current pool */
    ngx_regex_malloc_done();

    જો (re == ખાલી) {
        pcre2_get_error_message(errcode, errstr, 128);

        જો ((size_t) erroff == rc->pattern.len) {
            rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                              "pcre2_compile() failed: %s in \"%V\"",
                               errstr, &rc->pattern)
                          - rc->err.data;

        } નહીંતર {
            rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                              "pcre2_compile() failed: %s in \"%V\" at \"%s\"",
                               errstr, &rc->pattern, rc->pattern.data + erroff)
                          - rc->err.data;
        }

        પાછા NGX_ERROR;
    }

    rc->regex = re;

    /* do not study at runtime */

    જો (ngx_regex_studies != ખાલી) {
        elt = ngx_list_ધકેલો(ngx_regex_studies);
        જો (elt == ખાલી) {
            જાઓ nomem;
        }

        elt->regex = rc->regex;
        elt->name = rc->pattern.data;
    }

    n = pcre2_pattern_info(re, PCRE2_INFO_CAPTURECOUNT, &rc->captures);
    જો (n < 0) {
        p = "pcre2_pattern_info(\"%V\", PCRE2_INFO_CAPTURECOUNT) failed: %d";
        જાઓ failed;
    }

    જો (rc->captures == 0) {
        પાછા NGX_OK;
    }

    n = pcre2_pattern_info(re, PCRE2_INFO_NAMECOUNT, &rc->named_captures);
    જો (n < 0) {
        p = "pcre2_pattern_info(\"%V\", PCRE2_INFO_NAMECOUNT) failed: %d";
        જાઓ failed;
    }

    જો (rc->named_captures == 0) {
        પાછા NGX_OK;
    }

    n = pcre2_pattern_info(re, PCRE2_INFO_NAMEENTRYSIZE, &rc->name_size);
    જો (n < 0) {
        p = "pcre2_pattern_info(\"%V\", PCRE2_INFO_NAMEENTRYSIZE) failed: %d";
        જાઓ failed;
    }

    n = pcre2_pattern_info(re, PCRE2_INFO_NAMETABLE, &rc->names);
    જો (n < 0) {
        p = "pcre2_pattern_info(\"%V\", PCRE2_INFO_NAMETABLE) failed: %d";
        જાઓ failed;
    }

    પાછા NGX_OK;

failed:

    rc->err.len = ngx_snprintf(rc->err.data, rc->err.len, p, &rc->pattern, n)
                  - rc->err.data;
    પાછા NGX_ERROR;

nomem:

    rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                               "regex \"%V\" compilation failed: no memory",
                               &rc->pattern)
                  - rc->err.data;
    પાછા NGX_ERROR;
}

#નહીંતર

ngx_int_t
ngx_regex_compile(ngx_regex_compile_t *rc)
{
    પૂર્ણાંક               n, erroff;
    અક્ષર             *p;
    pcre             *re;
    અચલ અક્ષર       *errstr;
    ngx_uint_t        options;
    ngx_regex_elt_t  *elt;

    options = 0;

    જો (rc->options & NGX_REGEX_CASELESS) {
        options |= PCRE_CASELESS;
    }

    જો (rc->options & NGX_REGEX_MULTILINE) {
        options |= PCRE_MULTILINE;
    }

    જો (rc->options & ~(NGX_REGEX_CASELESS|NGX_REGEX_MULTILINE)) {
        rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                            "regex \"%V\" compilation failed: invalid options",
                            &rc->pattern)
                      - rc->err.data;
        પાછા NGX_ERROR;
    }

    ngx_regex_malloc_init(rc->pool);

    re = pcre_compile((અચલ અક્ષર *) rc->pattern.data, (પૂર્ણાંક) options,
                      &errstr, &erroff, ખાલી);

    /* ensure that there is no current pool */
    ngx_regex_malloc_done();

    જો (re == ખાલી) {
        જો ((size_t) erroff == rc->pattern.len) {
           rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                              "pcre_compile() failed: %s in \"%V\"",
                               errstr, &rc->pattern)
                         - rc->err.data;

        } નહીંતર {
           rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                              "pcre_compile() failed: %s in \"%V\" at \"%s\"",
                               errstr, &rc->pattern, rc->pattern.data + erroff)
                         - rc->err.data;
        }

        પાછા NGX_ERROR;
    }

    rc->regex = ngx_pcalloc(rc->pool, કદ(ngx_regex_t));
    જો (rc->regex == ખાલી) {
        જાઓ nomem;
    }

    rc->regex->code = re;

    /* do not study at runtime */

    જો (ngx_regex_studies != ખાલી) {
        elt = ngx_list_ધકેલો(ngx_regex_studies);
        જો (elt == ખાલી) {
            જાઓ nomem;
        }

        elt->regex = rc->regex;
        elt->name = rc->pattern.data;
    }

    n = pcre_fullinfo(re, ખાલી, PCRE_INFO_CAPTURECOUNT, &rc->captures);
    જો (n < 0) {
        p = "pcre_fullinfo(\"%V\", PCRE_INFO_CAPTURECOUNT) failed: %d";
        જાઓ failed;
    }

    જો (rc->captures == 0) {
        પાછા NGX_OK;
    }

    n = pcre_fullinfo(re, ખાલી, PCRE_INFO_NAMECOUNT, &rc->named_captures);
    જો (n < 0) {
        p = "pcre_fullinfo(\"%V\", PCRE_INFO_NAMECOUNT) failed: %d";
        જાઓ failed;
    }

    જો (rc->named_captures == 0) {
        પાછા NGX_OK;
    }

    n = pcre_fullinfo(re, ખાલી, PCRE_INFO_NAMEENTRYSIZE, &rc->name_size);
    જો (n < 0) {
        p = "pcre_fullinfo(\"%V\", PCRE_INFO_NAMEENTRYSIZE) failed: %d";
        જાઓ failed;
    }

    n = pcre_fullinfo(re, ખાલી, PCRE_INFO_NAMETABLE, &rc->names);
    જો (n < 0) {
        p = "pcre_fullinfo(\"%V\", PCRE_INFO_NAMETABLE) failed: %d";
        જાઓ failed;
    }

    પાછા NGX_OK;

failed:

    rc->err.len = ngx_snprintf(rc->err.data, rc->err.len, p, &rc->pattern, n)
                  - rc->err.data;
    પાછા NGX_ERROR;

nomem:

    rc->err.len = ngx_snprintf(rc->err.data, rc->err.len,
                               "regex \"%V\" compilation failed: no memory",
                               &rc->pattern)
                  - rc->err.data;
    પાછા NGX_ERROR;
}

#endif


#જો (NGX_PCRE2)

ngx_int_t
ngx_regex_exec(ngx_regex_t *re, ngx_str_t *s, પૂર્ણાંક *captures, ngx_uint_t size)
{
    size_t      *ov;
    ngx_int_t    rc;
    ngx_uint_t   n, i;

    /*
     * The pcre2_match() function might allocate memory for backtracking
     * frames, typical allocations are from 40k and above.  So the allocator
     * is configured to do direct allocations from heap during matching.
     */

    ngx_regex_malloc_init(ખાલી);

    જો (ngx_regex_match_data == ખાલી
        || size > ngx_regex_match_data_size)
    {
        /*
         * Allocate a match data if not yet allocated or smaller than
         * needed.
         */

        જો (ngx_regex_match_data) {
            pcre2_match_data_free(ngx_regex_match_data);
        }

        ngx_regex_match_data_size = size;
        ngx_regex_match_data = pcre2_match_data_create(size / 3, ખાલી);

        જો (ngx_regex_match_data == ખાલી) {
            rc = PCRE2_ERROR_NOMEMORY;
            જાઓ failed;
        }
    }

    rc = pcre2_match(re, s->data, s->len, 0, 0, ngx_regex_match_data, ખાલી);

    જો (rc < 0) {
        જાઓ failed;
    }

    n = pcre2_get_ovector_ગણતરી(ngx_regex_match_data);
    ov = pcre2_get_ovector_pointer(ngx_regex_match_data);

    જો (n > size / 3) {
        n = size / 3;
    }

    માટે (i = 0; i < n; i++) {
        captures[i * 2] = ov[i * 2];
        captures[i * 2 + 1] = ov[i * 2 + 1];
    }

failed:

    ngx_regex_malloc_done();

    પાછા rc;
}

#નહીંતર

ngx_int_t
ngx_regex_exec(ngx_regex_t *re, ngx_str_t *s, પૂર્ણાંક *captures, ngx_uint_t size)
{
    પાછા pcre_exec(re->code, re->extra, (અચલ અક્ષર *) s->data, s->len,
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

    માટે (i = 0; i < a->nelts; i++) {

        n = ngx_regex_exec(re[i].regex, s, ખાલી, 0);

        જો (n == NGX_REGEX_NO_MATCHED) {
            ચાલુ;
        }

        જો (n < 0) {
            ngx_log_error(NGX_LOG_ALERT, log, 0,
                          ngx_regex_exec_n " failed: %i on \"%V\" using \"%s\"",
                          n, s, re[i].name);
            પાછા NGX_ERROR;
        }

        /* match */

        પાછા NGX_OK;
    }

    પાછા NGX_DECLINED;
}


#જો (NGX_PCRE2)

સ્થિર શૂન્ય * ngx_libc_cdecl
ngx_regex_malloc(size_t size, શૂન્ય *data)
{
    જો (ngx_regex_pool) {
        પાછા ngx_palloc(ngx_regex_pool, size);
    }

    જો (ngx_regex_direct_alloc) {
        પાછા ngx_alloc(size, ngx_cycle->log);
    }

    પાછા ખાલી;
}


સ્થિર શૂન્ય ngx_libc_cdecl
ngx_regex_free(શૂન્ય *p, શૂન્ય *data)
{
    જો (ngx_regex_direct_alloc) {
        ngx_free(p);
    }

    પાછા;
}

#નહીંતર

સ્થિર શૂન્ય * ngx_libc_cdecl
ngx_regex_malloc(size_t size)
{
    જો (ngx_regex_pool) {
        પાછા ngx_palloc(ngx_regex_pool, size);
    }

    પાછા ખાલી;
}


સ્થિર શૂન્ય ngx_libc_cdecl
ngx_regex_free(શૂન્ય *p)
{
    પાછા;
}

#endif


સ્થિર શૂન્ય
ngx_regex_cleanup(શૂન્ય *data)
{
#જો (NGX_PCRE2 || NGX_HAVE_PCRE_JIT)
    ngx_regex_conf_t *rcf = data;

    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_regex_elt_t  *elts;

    part = &rcf->studies->part;
    elts = part->elts;

    માટે (i = 0; /* void */ ; i++) {

        જો (i >= part->nelts) {
            જો (part->next == ખાલી) {
                તોડો;
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

#જો (NGX_PCRE2)
        pcre2_code_free(elts[i].regex);
#નહીંતર
        જો (elts[i].regex->extra != ખાલી) {
            pcre_free_study(elts[i].regex->extra);
        }
#endif
    }
#endif

    /*
     * On configuration parsing errors ngx_regex_module_init() will not
     * be called.  Make sure ngx_regex_studies is properly cleared anyway.
     */

    ngx_regex_studies = ખાલી;

#જો (NGX_PCRE2)

    /*
     * Free compile context and match data.  If needed at runtime by
     * the new cycle, these will be re-allocated.
     */

    ngx_regex_malloc_init(ખાલી);

    જો (ngx_regex_compile_context) {
        pcre2_compile_context_free(ngx_regex_compile_context);
        ngx_regex_compile_context = ખાલી;
    }

    જો (ngx_regex_match_data) {
        pcre2_match_data_free(ngx_regex_match_data);
        ngx_regex_match_data = ખાલી;
        ngx_regex_match_data_size = 0;
    }

    ngx_regex_malloc_done();

#endif
}


સ્થિર ngx_int_t
ngx_regex_module_init(ngx_cycle_t *cycle)
{
    પૂર્ણાંક                opt;
#જો !(NGX_PCRE2)
    અચલ અક્ષર        *errstr;
#endif
    ngx_uint_t         i;
    ngx_list_part_t   *part;
    ngx_regex_elt_t   *elts;
    ngx_regex_conf_t  *rcf;

    opt = 0;

    rcf = (ngx_regex_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_regex_module);

#જો (NGX_PCRE2 || NGX_HAVE_PCRE_JIT)

    જો (rcf->pcre_jit) {
#જો (NGX_PCRE2)
        opt = 1;
#નહીંતર
        opt = PCRE_STUDY_JIT_COMPILE;
#endif
    }

#endif

    ngx_regex_malloc_init(cycle->pool);

    part = &rcf->studies->part;
    elts = part->elts;

    માટે (i = 0; /* void */ ; i++) {

        જો (i >= part->nelts) {
            જો (part->next == ખાલી) {
                તોડો;
            }

            part = part->next;
            elts = part->elts;
            i = 0;
        }

#જો (NGX_PCRE2)

        જો (opt) {
            પૂર્ણાંક  n;

            n = pcre2_jit_compile(elts[i].regex, PCRE2_JIT_COMPLETE);

            જો (n != 0) {
                ngx_log_error(NGX_LOG_INFO, cycle->log, 0,
                              "pcre2_jit_compile() failed: %d in \"%s\", "
                              "ignored",
                              n, elts[i].name);
            }
        }

#નહીંતર

        elts[i].regex->extra = pcre_study(elts[i].regex->code, opt, &errstr);

        જો (errstr != ખાલી) {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, 0,
                          "pcre_study() failed: %s in \"%s\"",
                          errstr, elts[i].name);
        }

#જો (NGX_HAVE_PCRE_JIT)
        જો (opt & PCRE_STUDY_JIT_COMPILE) {
            પૂર્ણાંક jit, n;

            jit = 0;
            n = pcre_fullinfo(elts[i].regex->code, elts[i].regex->extra,
                              PCRE_INFO_JIT, &jit);

            જો (n != 0 || jit != 1) {
                ngx_log_error(NGX_LOG_INFO, cycle->log, 0,
                              "JIT compiler does not support pattern: \"%s\"",
                              elts[i].name);
            }
        }
#endif
#endif
    }

    ngx_regex_malloc_done();

    ngx_regex_studies = ખાલી;

    પાછા NGX_OK;
}


સ્થિર શૂન્ય *
ngx_regex_create_conf(ngx_cycle_t *cycle)
{
    ngx_regex_conf_t    *rcf;
    ngx_pool_cleanup_t  *cln;

    rcf = ngx_pcalloc(cycle->pool, કદ(ngx_regex_conf_t));
    જો (rcf == ખાલી) {
        પાછા ખાલી;
    }

    rcf->pcre_jit = NGX_CONF_UNSET;

    cln = ngx_pool_cleanup_add(cycle->pool, 0);
    જો (cln == ખાલી) {
        પાછા ખાલી;
    }

    rcf->studies = ngx_list_create(cycle->pool, 8, કદ(ngx_regex_elt_t));
    જો (rcf->studies == ખાલી) {
        પાછા ખાલી;
    }

    cln->handler = ngx_regex_cleanup;
    cln->data = rcf;

    ngx_regex_studies = rcf->studies;

    પાછા rcf;
}


સ્થિર અક્ષર *
ngx_regex_init_conf(ngx_cycle_t *cycle, શૂન્ય *conf)
{
    ngx_regex_conf_t *rcf = conf;

    ngx_conf_init_કિંમત(rcf->pcre_jit, 0);

    પાછા NGX_CONF_OK;
}


સ્થિર અક્ષર *
ngx_regex_pcre_jit(ngx_conf_t *cf, શૂન્ય *post, શૂન્ય *data)
{
    ngx_flag_t  *fp = data;

    જો (*fp == 0) {
        પાછા NGX_CONF_OK;
    }

#જો (NGX_PCRE2)
    {
    પૂર્ણાંક       r;
    uint32_t  jit;

    jit = 0;
    r = pcre2_config(PCRE2_CONFIG_JIT, &jit);

    જો (r != 0 || jit != 1) {
        ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                           "PCRE2 library does not support JIT");
        *fp = 0;
    }
    }
#અથવાજો (NGX_HAVE_PCRE_JIT)
    {
    પૂર્ણાંક  jit, r;

    jit = 0;
    r = pcre_config(PCRE_CONFIG_JIT, &jit);

    જો (r != 0 || jit != 1) {
        ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                           "PCRE library does not support JIT");
        *fp = 0;
    }
    }
#નહીંતર
    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "nginx was built without PCRE JIT support");
    *fp = 0;
#endif

    પાછા NGX_CONF_OK;
}
