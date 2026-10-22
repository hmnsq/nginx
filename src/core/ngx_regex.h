
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#جے_بنا_تعریف _NGX_REGEX_H_INCLUDED_
#تعریف_کرو _NGX_REGEX_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


#إذا (NGX_PCRE2)

#تعریف_کرو PCRE2_کوڈ_UNIT_چوڑائی  8
#ضمن <pcre2.h>

#تعریف_کرو NGX_REGEX_NO_MATCHED   PCRE2_ERROR_NOMATCH   /* -1 */

تعريفنوع pcre2_کوڈ  ngx_regex_t;

#وإلا

#ضمن <pcre.h>

#تعریف_کرو NGX_REGEX_NO_MATCHED   PCRE_ERROR_NOMATCH    /* -1 */

تعريفنوع بنية {
    pcre        *کوڈ;
    pcre_extra  *extra;
} ngx_regex_t;

#جے_انت


#تعریف_کرو NGX_REGEX_CASELESS     0x00000001
#تعریف_کرو NGX_REGEX_MULTILINE    0x00000002


تعريفنوع بنية {
    ngx_str_t     نمونہ;
    ngx_pool_t   *pool;
    ngx_uint_t    options;

    ngx_regex_t  *regex;
    صحيح           captures;
    صحيح           named_captures;
    صحيح           name_ناپ;
    u_char       *ناں;
    ngx_str_t     err;
} ngx_regex_compile_t;


تعريفنوع بنية {
    ngx_regex_t  *regex;
    u_char       *name;
} ngx_regex_elt_t;


فراغ ngx_regex_init(فراغ);
ngx_int_t ngx_regex_compile(ngx_regex_compile_t *rc);

ngx_int_t ngx_regex_exec(ngx_regex_t *re, ngx_str_t *s, صحيح *captures,
    ngx_uint_t ناپ);

#إذا (NGX_PCRE2)
#تعریف_کرو ngx_regex_exec_n       "pcre2_match()"
#وإلا
#تعریف_کرو ngx_regex_exec_n       "pcre_exec()"
#جے_انت

ngx_int_t ngx_regex_exec_array(ngx_array_t *a, ngx_str_t *s, ngx_log_t *لوگارتھم);


#جے_انت /* _NGX_REGEX_H_INCLUDED_ */
