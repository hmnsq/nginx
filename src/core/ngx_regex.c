
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ngx_flag_t   pcre_jit;
    ngx_list_t  *studies;
} ngx_regex_conf_t;


ਸਥਿਰ ngx_inline ਖਾਲੀ ngx_regex_malloc_init(ngx_pool_t *pool);
ਸਥਿਰ ngx_inline ਖਾਲੀ ngx_regex_malloc_ਪੂਰਾ(ਖਾਲੀ);

#ਜੇ (NGX_PCRE2)
ਸਥਿਰ ਖਾਲੀ * ngx_libc_cdecl ngx_regex_malloc(ਮਾਪ_t ਮਾਪ, ਖਾਲੀ *ਅੰਕੜੇ);
ਸਥਿਰ ਖਾਲੀ ngx_libc_cdecl ngx_regex_free(ਖਾਲੀ *p, ਖਾਲੀ *ਅੰਕੜੇ);
#ਨਹੀਂਤਾਂ
ਸਥਿਰ ਖਾਲੀ * ngx_libc_cdecl ngx_regex_malloc(ਮਾਪ_t ਮਾਪ);
ਸਥਿਰ ਖਾਲੀ ngx_libc_cdecl ngx_regex_free(ਖਾਲੀ *p);
#ਜੇ_ਅੰਤ
ਸਥਿਰ ਖਾਲੀ ngx_regex_cleanup(ਖਾਲੀ *ਅੰਕੜੇ);

ਸਥਿਰ ngx_int_t ngx_regex_module_init(ngx_ਚੱਕਰ_t *ਚੱਕਰ);

ਸਥਿਰ ਖਾਲੀ *ngx_regex_create_conf(ngx_ਚੱਕਰ_t *ਚੱਕਰ);
ਸਥਿਰ ਅੱਖਰ *ngx_regex_init_conf(ngx_ਚੱਕਰ_t *ਚੱਕਰ, ਖਾਲੀ *conf);

ਸਥਿਰ ਅੱਖਰ *ngx_regex_pcre_jit(ngx_conf_t *cf, ਖਾਲੀ *post, ਖਾਲੀ *ਅੰਕੜੇ);
ਸਥਿਰ ngx_conf_post_t  ngx_regex_pcre_jit_post = { ngx_regex_pcre_jit };


ਸਥਿਰ ngx_command_t  ngx_regex_commands[] = {

    { ngx_string("pcre_jit"),
      NGX_MAIN_CONF|NGX_ਸਿੱਧਾ_CONF|NGX_CONF_FLAG,
      ngx_conf_ਸੈੱਟ_flag_ਥਾਂ,
      0,
      offsetof(ngx_regex_conf_t, pcre_jit),
      &ngx_regex_pcre_jit_post },

      ngx_null_command
};


ਸਥਿਰ ngx_core_module_t  ngx_regex_module_ctx = {
    ngx_string("regex"),
    ngx_regex_create_conf,
    ngx_regex_init_conf
};


ngx_module_t  ngx_regex_module = {
    NGX_MODULE_V1,
    &ngx_regex_module_ctx,                 /* module context */
    ngx_regex_commands,                    /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    ਨਿਰਾ,                                  /* init master */
    ngx_regex_module_init,                 /* init module */
    ਨਿਰਾ,                                  /* init process */
    ਨਿਰਾ,                                  /* init thread */
    ਨਿਰਾ,                                  /* exit thread */
    ਨਿਰਾ,                                  /* exit process */
    ਨਿਰਾ,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


ਸਥਿਰ ngx_pool_t             *ngx_regex_pool;
ਸਥਿਰ ngx_list_t             *ngx_regex_studies;
ਸਥਿਰ ngx_uint_t              ngx_regex_ਸਿੱਧਾ_alloc;

#ਜੇ (NGX_PCRE2)
ਸਥਿਰ pcre2_compile_context  *ngx_regex_compile_context;
ਸਥਿਰ pcre2_ਮੇਲ_ਅੰਕੜੇ       *ngx_regex_ਮੇਲ_ਅੰਕੜੇ;
ਸਥਿਰ ngx_uint_t              ngx_regex_ਮੇਲ_ਅੰਕੜੇ_ਮਾਪ;
#ਜੇ_ਅੰਤ


ਖਾਲੀ
ngx_regex_init(ਖਾਲੀ)
{
#ਜੇ !(NGX_PCRE2)
    pcre_malloc = ngx_regex_malloc;
    pcre_free = ngx_regex_free;
#ਜੇ_ਅੰਤ
}


ਸਥਿਰ ngx_inline ਖਾਲੀ
ngx_regex_malloc_init(ngx_pool_t *pool)
{
    ngx_regex_pool = pool;
    ngx_regex_ਸਿੱਧਾ_alloc = (pool == ਨਿਰਾ) ? 1 : 0;
}


ਸਥਿਰ ngx_inline ਖਾਲੀ
ngx_regex_malloc_ਪੂਰਾ(ਖਾਲੀ)
{
    ngx_regex_pool = ਨਿਰਾ;
    ngx_regex_ਸਿੱਧਾ_alloc = 0;
}


#ਜੇ (NGX_PCRE2)

ngx_int_t
ngx_regex_compile(ngx_regex_compile_t *rc)
{
    ਪੂਰਨ                     n, errcode;
    ਅੱਖਰ                   *p;
    u_char                  errstr[128];
    ਮਾਪ_t                  erroff;
    uint32_t                options;
    pcre2_ਕੋਡ             *re;
    ngx_regex_elt_t        *elt;
    pcre2_general_context  *gctx;
    pcre2_compile_context  *cctx;

    ਜੇ (ngx_regex_compile_context == ਨਿਰਾ) {
        /*
         * Allocate a compile context if not yet allocated.  This uses
         * direct allocations from heap, so the result can be cached
         * even at runtime.
         */

        ngx_regex_malloc_init(ਨਿਰਾ);

        gctx = pcre2_general_context_create(ngx_regex_malloc, ngx_regex_free,
                                            ਨਿਰਾ);
        ਜੇ (gctx == ਨਿਰਾ) {
            ngx_regex_malloc_ਪੂਰਾ();
            ਜਾਓ nomem;
        }

        cctx = pcre2_compile_context_create(gctx);
        ਜੇ (cctx == ਨਿਰਾ) {
            pcre2_general_context_free(gctx);
            ngx_regex_malloc_ਪੂਰਾ();
            ਜਾਓ nomem;
        }

        ngx_regex_compile_context = cctx;

        pcre2_general_context_free(gctx);
        ngx_regex_malloc_ਪੂਰਾ();
    }

    options = 0;

    ਜੇ (rc->options & NGX_REGEX_CASELESS) {
        options |= PCRE2_CASELESS;
    }

    ਜੇ (rc->options & NGX_REGEX_MULTILINE) {
        options |= PCRE2_MULTILINE;
    }

    ਜੇ (rc->options & ~(NGX_REGEX_CASELESS|NGX_REGEX_MULTILINE)) {
        rc->err.len = ngx_snprintf(rc->err.ਅੰਕੜੇ, rc->err.len,
                            "regex \"%V\" compilation failed: invalid options",
                            &rc->ਨਮੂਨਾ)
                      - rc->err.ਅੰਕੜੇ;
        ਵਾਪਸ NGX_ERROR;
    }

    ngx_regex_malloc_init(rc->pool);

    re = pcre2_compile(rc->ਨਮੂਨਾ.ਅੰਕੜੇ, rc->ਨਮੂਨਾ.len, options,
                       &errcode, &erroff, ngx_regex_compile_context);

    /* ensure that there is no current pool */
    ngx_regex_malloc_ਪੂਰਾ();

    ਜੇ (re == ਨਿਰਾ) {
        pcre2_get_error_ਸੁਨੇਹਾ(errcode, errstr, 128);

        ਜੇ ((ਮਾਪ_t) erroff == rc->ਨਮੂਨਾ.len) {
            rc->err.len = ngx_snprintf(rc->err.ਅੰਕੜੇ, rc->err.len,
                              "pcre2_compile() failed: %s in \"%V\"",
                               errstr, &rc->ਨਮੂਨਾ)
                          - rc->err.ਅੰਕੜੇ;

        } ਨਹੀਂਤਾਂ {
            rc->err.len = ngx_snprintf(rc->err.ਅੰਕੜੇ, rc->err.len,
                              "pcre2_compile() failed: %s in \"%V\" at \"%s\"",
                               errstr, &rc->ਨਮੂਨਾ, rc->ਨਮੂਨਾ.ਅੰਕੜੇ + erroff)
                          - rc->err.ਅੰਕੜੇ;
        }

        ਵਾਪਸ NGX_ERROR;
    }

    rc->regex = re;

    /* do not study at runtime */

    ਜੇ (ngx_regex_studies != ਨਿਰਾ) {
        elt = ngx_list_ਧੱਕੋ(ngx_regex_studies);
        ਜੇ (elt == ਨਿਰਾ) {
            ਜਾਓ nomem;
        }

        elt->regex = rc->regex;
        elt->name = rc->ਨਮੂਨਾ.ਅੰਕੜੇ;
    }

    n = pcre2_ਨਮੂਨਾ_info(re, PCRE2_INFO_CAPTURECOUNT, &rc->captures);
    ਜੇ (n < 0) {
        p = "pcre2_pattern_info(\"%V\", PCRE2_INFO_CAPTURECOUNT) failed: %d";
        ਜਾਓ failed;
    }

    ਜੇ (rc->captures == 0) {
        ਵਾਪਸ NGX_OK;
    }

    n = pcre2_ਨਮੂਨਾ_info(re, PCRE2_INFO_NAMECOUNT, &rc->named_captures);
    ਜੇ (n < 0) {
        p = "pcre2_pattern_info(\"%V\", PCRE2_INFO_NAMECOUNT) failed: %d";
        ਜਾਓ failed;
    }

    ਜੇ (rc->named_captures == 0) {
        ਵਾਪਸ NGX_OK;
    }

    n = pcre2_ਨਮੂਨਾ_info(re, PCRE2_INFO_NAMEENTRYSIZE, &rc->name_ਮਾਪ);
    ਜੇ (n < 0) {
        p = "pcre2_pattern_info(\"%V\", PCRE2_INFO_NAMEENTRYSIZE) failed: %d";
        ਜਾਓ failed;
    }

    n = pcre2_ਨਮੂਨਾ_info(re, PCRE2_INFO_NAMETABLE, &rc->ਨਾਂ);
    ਜੇ (n < 0) {
        p = "pcre2_pattern_info(\"%V\", PCRE2_INFO_NAMETABLE) failed: %d";
        ਜਾਓ failed;
    }

    ਵਾਪਸ NGX_OK;

failed:

    rc->err.len = ngx_snprintf(rc->err.ਅੰਕੜੇ, rc->err.len, p, &rc->ਨਮੂਨਾ, n)
                  - rc->err.ਅੰਕੜੇ;
    ਵਾਪਸ NGX_ERROR;

nomem:

    rc->err.len = ngx_snprintf(rc->err.ਅੰਕੜੇ, rc->err.len,
                               "regex \"%V\" compilation failed: no memory",
                               &rc->ਨਮੂਨਾ)
                  - rc->err.ਅੰਕੜੇ;
    ਵਾਪਸ NGX_ERROR;
}

#ਨਹੀਂਤਾਂ

ngx_int_t
ngx_regex_compile(ngx_regex_compile_t *rc)
{
    ਪੂਰਨ               n, erroff;
    ਅੱਖਰ             *p;
    pcre             *re;
    ਅਚਲ ਅੱਖਰ       *errstr;
    ngx_uint_t        options;
    ngx_regex_elt_t  *elt;

    options = 0;

    ਜੇ (rc->options & NGX_REGEX_CASELESS) {
        options |= PCRE_CASELESS;
    }

    ਜੇ (rc->options & NGX_REGEX_MULTILINE) {
        options |= PCRE_MULTILINE;
    }

    ਜੇ (rc->options & ~(NGX_REGEX_CASELESS|NGX_REGEX_MULTILINE)) {
        rc->err.len = ngx_snprintf(rc->err.ਅੰਕੜੇ, rc->err.len,
                            "regex \"%V\" compilation failed: invalid options",
                            &rc->ਨਮੂਨਾ)
                      - rc->err.ਅੰਕੜੇ;
        ਵਾਪਸ NGX_ERROR;
    }

    ngx_regex_malloc_init(rc->pool);

    re = pcre_compile((ਅਚਲ ਅੱਖਰ *) rc->ਨਮੂਨਾ.ਅੰਕੜੇ, (ਪੂਰਨ) options,
                      &errstr, &erroff, ਨਿਰਾ);

    /* ensure that there is no current pool */
    ngx_regex_malloc_ਪੂਰਾ();

    ਜੇ (re == ਨਿਰਾ) {
        ਜੇ ((ਮਾਪ_t) erroff == rc->ਨਮੂਨਾ.len) {
           rc->err.len = ngx_snprintf(rc->err.ਅੰਕੜੇ, rc->err.len,
                              "pcre_compile() failed: %s in \"%V\"",
                               errstr, &rc->ਨਮੂਨਾ)
                         - rc->err.ਅੰਕੜੇ;

        } ਨਹੀਂਤਾਂ {
           rc->err.len = ngx_snprintf(rc->err.ਅੰਕੜੇ, rc->err.len,
                              "pcre_compile() failed: %s in \"%V\" at \"%s\"",
                               errstr, &rc->ਨਮੂਨਾ, rc->ਨਮੂਨਾ.ਅੰਕੜੇ + erroff)
                         - rc->err.ਅੰਕੜੇ;
        }

        ਵਾਪਸ NGX_ERROR;
    }

    rc->regex = ngx_pcalloc(rc->pool, ਆਕਾਰ(ngx_regex_t));
    ਜੇ (rc->regex == ਨਿਰਾ) {
        ਜਾਓ nomem;
    }

    rc->regex->ਕੋਡ = re;

    /* do not study at runtime */

    ਜੇ (ngx_regex_studies != ਨਿਰਾ) {
        elt = ngx_list_ਧੱਕੋ(ngx_regex_studies);
        ਜੇ (elt == ਨਿਰਾ) {
            ਜਾਓ nomem;
        }

        elt->regex = rc->regex;
        elt->name = rc->ਨਮੂਨਾ.ਅੰਕੜੇ;
    }

    n = pcre_fullinfo(re, ਨਿਰਾ, PCRE_INFO_CAPTURECOUNT, &rc->captures);
    ਜੇ (n < 0) {
        p = "pcre_fullinfo(\"%V\", PCRE_INFO_CAPTURECOUNT) failed: %d";
        ਜਾਓ failed;
    }

    ਜੇ (rc->captures == 0) {
        ਵਾਪਸ NGX_OK;
    }

    n = pcre_fullinfo(re, ਨਿਰਾ, PCRE_INFO_NAMECOUNT, &rc->named_captures);
    ਜੇ (n < 0) {
        p = "pcre_fullinfo(\"%V\", PCRE_INFO_NAMECOUNT) failed: %d";
        ਜਾਓ failed;
    }

    ਜੇ (rc->named_captures == 0) {
        ਵਾਪਸ NGX_OK;
    }

    n = pcre_fullinfo(re, ਨਿਰਾ, PCRE_INFO_NAMEENTRYSIZE, &rc->name_ਮਾਪ);
    ਜੇ (n < 0) {
        p = "pcre_fullinfo(\"%V\", PCRE_INFO_NAMEENTRYSIZE) failed: %d";
        ਜਾਓ failed;
    }

    n = pcre_fullinfo(re, ਨਿਰਾ, PCRE_INFO_NAMETABLE, &rc->ਨਾਂ);
    ਜੇ (n < 0) {
        p = "pcre_fullinfo(\"%V\", PCRE_INFO_NAMETABLE) failed: %d";
        ਜਾਓ failed;
    }

    ਵਾਪਸ NGX_OK;

failed:

    rc->err.len = ngx_snprintf(rc->err.ਅੰਕੜੇ, rc->err.len, p, &rc->ਨਮੂਨਾ, n)
                  - rc->err.ਅੰਕੜੇ;
    ਵਾਪਸ NGX_ERROR;

nomem:

    rc->err.len = ngx_snprintf(rc->err.ਅੰਕੜੇ, rc->err.len,
                               "regex \"%V\" compilation failed: no memory",
                               &rc->ਨਮੂਨਾ)
                  - rc->err.ਅੰਕੜੇ;
    ਵਾਪਸ NGX_ERROR;
}

#ਜੇ_ਅੰਤ


#ਜੇ (NGX_PCRE2)

ngx_int_t
ngx_regex_exec(ngx_regex_t *re, ngx_str_t *s, ਪੂਰਨ *captures, ngx_uint_t ਮਾਪ)
{
    ਮਾਪ_t      *ov;
    ngx_int_t    rc;
    ngx_uint_t   n, i;

    /*
     * The pcre2_match() function might allocate memory for backtracking
     * frames, typical allocations are from 40k and above.  So the allocator
     * is configured to do direct allocations from heap during matching.
     */

    ngx_regex_malloc_init(ਨਿਰਾ);

    ਜੇ (ngx_regex_ਮੇਲ_ਅੰਕੜੇ == ਨਿਰਾ
        || ਮਾਪ > ngx_regex_ਮੇਲ_ਅੰਕੜੇ_ਮਾਪ)
    {
        /*
         * Allocate a match data if not yet allocated or smaller than
         * needed.
         */

        ਜੇ (ngx_regex_ਮੇਲ_ਅੰਕੜੇ) {
            pcre2_ਮੇਲ_ਅੰਕੜੇ_free(ngx_regex_ਮੇਲ_ਅੰਕੜੇ);
        }

        ngx_regex_ਮੇਲ_ਅੰਕੜੇ_ਮਾਪ = ਮਾਪ;
        ngx_regex_ਮੇਲ_ਅੰਕੜੇ = pcre2_ਮੇਲ_ਅੰਕੜੇ_create(ਮਾਪ / 3, ਨਿਰਾ);

        ਜੇ (ngx_regex_ਮੇਲ_ਅੰਕੜੇ == ਨਿਰਾ) {
            rc = PCRE2_ERROR_NOMEMORY;
            ਜਾਓ failed;
        }
    }

    rc = pcre2_ਮੇਲ(re, s->ਅੰਕੜੇ, s->len, 0, 0, ngx_regex_ਮੇਲ_ਅੰਕੜੇ, ਨਿਰਾ);

    ਜੇ (rc < 0) {
        ਜਾਓ failed;
    }

    n = pcre2_get_ovector_ਗਿਣਤੀ(ngx_regex_ਮੇਲ_ਅੰਕੜੇ);
    ov = pcre2_get_ovector_ਸੂਚਕ(ngx_regex_ਮੇਲ_ਅੰਕੜੇ);

    ਜੇ (n > ਮਾਪ / 3) {
        n = ਮਾਪ / 3;
    }

    ਲਈ (i = 0; i < n; i++) {
        captures[i * 2] = ov[i * 2];
        captures[i * 2 + 1] = ov[i * 2 + 1];
    }

failed:

    ngx_regex_malloc_ਪੂਰਾ();

    ਵਾਪਸ rc;
}

#ਨਹੀਂਤਾਂ

ngx_int_t
ngx_regex_exec(ngx_regex_t *re, ngx_str_t *s, ਪੂਰਨ *captures, ngx_uint_t ਮਾਪ)
{
    ਵਾਪਸ pcre_exec(re->ਕੋਡ, re->extra, (ਅਚਲ ਅੱਖਰ *) s->ਅੰਕੜੇ, s->len,
                     0, 0, captures, ਮਾਪ);
}

#ਜੇ_ਅੰਤ


ngx_int_t
ngx_regex_exec_array(ngx_array_t *a, ngx_str_t *s, ngx_log_t *ਲਘੂਗਣਕ)
{
    ngx_int_t         n;
    ngx_uint_t        i;
    ngx_regex_elt_t  *re;

    re = a->elts;

    ਲਈ (i = 0; i < a->nelts; i++) {

        n = ngx_regex_exec(re[i].regex, s, ਨਿਰਾ, 0);

        ਜੇ (n == NGX_REGEX_NO_MATCHED) {
            ਜਾਰੀ;
        }

        ਜੇ (n < 0) {
            ngx_log_error(NGX_LOG_ALERT, ਲਘੂਗਣਕ, 0,
                          ngx_regex_exec_n " failed: %i on \"%V\" using \"%s\"",
                          n, s, re[i].name);
            ਵਾਪਸ NGX_ERROR;
        }

        /*ਮੇਲ*/

        ਵਾਪਸ NGX_OK;
    }

    ਵਾਪਸ NGX_DECLINED;
}


#ਜੇ (NGX_PCRE2)

ਸਥਿਰ ਖਾਲੀ * ngx_libc_cdecl
ngx_regex_malloc(ਮਾਪ_t ਮਾਪ, ਖਾਲੀ *ਅੰਕੜੇ)
{
    ਜੇ (ngx_regex_pool) {
        ਵਾਪਸ ngx_palloc(ngx_regex_pool, ਮਾਪ);
    }

    ਜੇ (ngx_regex_ਸਿੱਧਾ_alloc) {
        ਵਾਪਸ ngx_alloc(ਮਾਪ, ngx_ਚੱਕਰ->ਲਘੂਗਣਕ);
    }

    ਵਾਪਸ ਨਿਰਾ;
}


ਸਥਿਰ ਖਾਲੀ ngx_libc_cdecl
ngx_regex_free(ਖਾਲੀ *p, ਖਾਲੀ *ਅੰਕੜੇ)
{
    ਜੇ (ngx_regex_ਸਿੱਧਾ_alloc) {
        ngx_free(p);
    }

    ਵਾਪਸ;
}

#ਨਹੀਂਤਾਂ

ਸਥਿਰ ਖਾਲੀ * ngx_libc_cdecl
ngx_regex_malloc(ਮਾਪ_t ਮਾਪ)
{
    ਜੇ (ngx_regex_pool) {
        ਵਾਪਸ ngx_palloc(ngx_regex_pool, ਮਾਪ);
    }

    ਵਾਪਸ ਨਿਰਾ;
}


ਸਥਿਰ ਖਾਲੀ ngx_libc_cdecl
ngx_regex_free(ਖਾਲੀ *p)
{
    ਵਾਪਸ;
}

#ਜੇ_ਅੰਤ


ਸਥਿਰ ਖਾਲੀ
ngx_regex_cleanup(ਖਾਲੀ *ਅੰਕੜੇ)
{
#ਜੇ (NGX_PCRE2 || NGX_HAVE_PCRE_JIT)
    ngx_regex_conf_t *rcf = ਅੰਕੜੇ;

    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_regex_elt_t  *elts;

    part = &rcf->studies->part;
    elts = part->elts;

    ਲਈ (i = 0; /* void */ ; i++) {

        ਜੇ (i >= part->nelts) {
            ਜੇ (part->ਅਗਲਾ == ਨਿਰਾ) {
                ਰੋਕੋ;
            }

            part = part->ਅਗਲਾ;
            elts = part->elts;
            i = 0;
        }

        /*
         * The PCRE JIT compiler uses mmap for its executable codes, so we
         * have to explicitly call the pcre_free_study() function to free
         * this memory.  In PCRE2, we call the pcre2_code_free() function
         * for the same reason.
         */

#ਜੇ (NGX_PCRE2)
        pcre2_ਕੋਡ_free(elts[i].regex);
#ਨਹੀਂਤਾਂ
        ਜੇ (elts[i].regex->extra != ਨਿਰਾ) {
            pcre_free_study(elts[i].regex->extra);
        }
#ਜੇ_ਅੰਤ
    }
#ਜੇ_ਅੰਤ

    /*
     * On configuration parsing errors ngx_regex_module_init() will not
     * be called.  Make sure ngx_regex_studies is properly cleared anyway.
     */

    ngx_regex_studies = ਨਿਰਾ;

#ਜੇ (NGX_PCRE2)

    /*
     * Free compile context and match data.  If needed at runtime by
     * the new cycle, these will be re-allocated.
     */

    ngx_regex_malloc_init(ਨਿਰਾ);

    ਜੇ (ngx_regex_compile_context) {
        pcre2_compile_context_free(ngx_regex_compile_context);
        ngx_regex_compile_context = ਨਿਰਾ;
    }

    ਜੇ (ngx_regex_ਮੇਲ_ਅੰਕੜੇ) {
        pcre2_ਮੇਲ_ਅੰਕੜੇ_free(ngx_regex_ਮੇਲ_ਅੰਕੜੇ);
        ngx_regex_ਮੇਲ_ਅੰਕੜੇ = ਨਿਰਾ;
        ngx_regex_ਮੇਲ_ਅੰਕੜੇ_ਮਾਪ = 0;
    }

    ngx_regex_malloc_ਪੂਰਾ();

#ਜੇ_ਅੰਤ
}


ਸਥਿਰ ngx_int_t
ngx_regex_module_init(ngx_ਚੱਕਰ_t *ਚੱਕਰ)
{
    ਪੂਰਨ                opt;
#ਜੇ !(NGX_PCRE2)
    ਅਚਲ ਅੱਖਰ        *errstr;
#ਜੇ_ਅੰਤ
    ngx_uint_t         i;
    ngx_list_part_t   *part;
    ngx_regex_elt_t   *elts;
    ngx_regex_conf_t  *rcf;

    opt = 0;

    rcf = (ngx_regex_conf_t *) ngx_get_conf(ਚੱਕਰ->conf_ctx, ngx_regex_module);

#ਜੇ (NGX_PCRE2 || NGX_HAVE_PCRE_JIT)

    ਜੇ (rcf->pcre_jit) {
#ਜੇ (NGX_PCRE2)
        opt = 1;
#ਨਹੀਂਤਾਂ
        opt = PCRE_STUDY_JIT_COMPILE;
#ਜੇ_ਅੰਤ
    }

#ਜੇ_ਅੰਤ

    ngx_regex_malloc_init(ਚੱਕਰ->pool);

    part = &rcf->studies->part;
    elts = part->elts;

    ਲਈ (i = 0; /* void */ ; i++) {

        ਜੇ (i >= part->nelts) {
            ਜੇ (part->ਅਗਲਾ == ਨਿਰਾ) {
                ਰੋਕੋ;
            }

            part = part->ਅਗਲਾ;
            elts = part->elts;
            i = 0;
        }

#ਜੇ (NGX_PCRE2)

        ਜੇ (opt) {
            ਪੂਰਨ  n;

            n = pcre2_jit_compile(elts[i].regex, PCRE2_JIT_COMPLETE);

            ਜੇ (n != 0) {
                ngx_log_error(NGX_LOG_INFO, ਚੱਕਰ->ਲਘੂਗਣਕ, 0,
                              "pcre2_jit_compile() failed: %d in \"%s\", "
                              "ignored",
                              n, elts[i].name);
            }
        }

#ਨਹੀਂਤਾਂ

        elts[i].regex->extra = pcre_study(elts[i].regex->ਕੋਡ, opt, &errstr);

        ਜੇ (errstr != ਨਿਰਾ) {
            ngx_log_error(NGX_LOG_ALERT, ਚੱਕਰ->ਲਘੂਗਣਕ, 0,
                          "pcre_study() failed: %s in \"%s\"",
                          errstr, elts[i].name);
        }

#ਜੇ (NGX_HAVE_PCRE_JIT)
        ਜੇ (opt & PCRE_STUDY_JIT_COMPILE) {
            ਪੂਰਨ jit, n;

            jit = 0;
            n = pcre_fullinfo(elts[i].regex->ਕੋਡ, elts[i].regex->extra,
                              PCRE_INFO_JIT, &jit);

            ਜੇ (n != 0 || jit != 1) {
                ngx_log_error(NGX_LOG_INFO, ਚੱਕਰ->ਲਘੂਗਣਕ, 0,
                              "JIT compiler does not support pattern: \"%s\"",
                              elts[i].name);
            }
        }
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ
    }

    ngx_regex_malloc_ਪੂਰਾ();

    ngx_regex_studies = ਨਿਰਾ;

    ਵਾਪਸ NGX_OK;
}


ਸਥਿਰ ਖਾਲੀ *
ngx_regex_create_conf(ngx_ਚੱਕਰ_t *ਚੱਕਰ)
{
    ngx_regex_conf_t    *rcf;
    ngx_pool_cleanup_t  *cln;

    rcf = ngx_pcalloc(ਚੱਕਰ->pool, ਆਕਾਰ(ngx_regex_conf_t));
    ਜੇ (rcf == ਨਿਰਾ) {
        ਵਾਪਸ ਨਿਰਾ;
    }

    rcf->pcre_jit = NGX_CONF_UNSET;

    cln = ngx_pool_cleanup_ਜੋੜੋ(ਚੱਕਰ->pool, 0);
    ਜੇ (cln == ਨਿਰਾ) {
        ਵਾਪਸ ਨਿਰਾ;
    }

    rcf->studies = ngx_list_create(ਚੱਕਰ->pool, 8, ਆਕਾਰ(ngx_regex_elt_t));
    ਜੇ (rcf->studies == ਨਿਰਾ) {
        ਵਾਪਸ ਨਿਰਾ;
    }

    cln->handler = ngx_regex_cleanup;
    cln->ਅੰਕੜੇ = rcf;

    ngx_regex_studies = rcf->studies;

    ਵਾਪਸ rcf;
}


ਸਥਿਰ ਅੱਖਰ *
ngx_regex_init_conf(ngx_ਚੱਕਰ_t *ਚੱਕਰ, ਖਾਲੀ *conf)
{
    ngx_regex_conf_t *rcf = conf;

    ngx_conf_init_ਮੁੱਲ(rcf->pcre_jit, 0);

    ਵਾਪਸ NGX_CONF_OK;
}


ਸਥਿਰ ਅੱਖਰ *
ngx_regex_pcre_jit(ngx_conf_t *cf, ਖਾਲੀ *post, ਖਾਲੀ *ਅੰਕੜੇ)
{
    ngx_flag_t  *fp = ਅੰਕੜੇ;

    ਜੇ (*fp == 0) {
        ਵਾਪਸ NGX_CONF_OK;
    }

#ਜੇ (NGX_PCRE2)
    {
    ਪੂਰਨ       r;
    uint32_t  jit;

    jit = 0;
    r = pcre2_config(PCRE2_CONFIG_JIT, &jit);

    ਜੇ (r != 0 || jit != 1) {
        ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                           "PCRE2 library does not support JIT");
        *fp = 0;
    }
    }
#ਜਾਂਜੇ (NGX_HAVE_PCRE_JIT)
    {
    ਪੂਰਨ  jit, r;

    jit = 0;
    r = pcre_config(PCRE_CONFIG_JIT, &jit);

    ਜੇ (r != 0 || jit != 1) {
        ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                           "PCRE library does not support JIT");
        *fp = 0;
    }
    }
#ਨਹੀਂਤਾਂ
    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "nginx was built without PCRE JIT support");
    *fp = 0;
#ਜੇ_ਅੰਤ

    ਵਾਪਸ NGX_CONF_OK;
}
