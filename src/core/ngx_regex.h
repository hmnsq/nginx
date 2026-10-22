
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#جيڪڏهن_وصف_نه_ٿيل _NGX_REGEX_H_INCLUDED_
#وصف_ڪريو _NGX_REGEX_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


#إذا (NGX_PCRE2)

#وصف_ڪريو PCRE2_ڪوڊ_UNIT_ويڪر  8
#ضمن <pcre2.h>

#وصف_ڪريو NGX_REGEX_NO_MATCHED   PCRE2_ERROR_NOMATCH   /* -1 */

تعريفنوع pcre2_ڪوڊ  ngx_regex_t;

#وإلا

#ضمن <pcre.h>

#وصف_ڪريو NGX_REGEX_NO_MATCHED   PCRE_ERROR_NOMATCH    /* -1 */

تعريفنوع بنية {
    pcre        *ڪوڊ;
    pcre_extra  *extra;
} ngx_regex_t;

#جيڪڏهن_پورو


#وصف_ڪريو NGX_REGEX_CASELESS     0x00000001
#وصف_ڪريو NGX_REGEX_MULTILINE    0x00000002


تعريفنوع بنية {
    ngx_str_t     نمونو;
    ngx_pool_t   *pool;
    ngx_uint_t    options;

    ngx_regex_t  *regex;
    صحيح           captures;
    صحيح           named_captures;
    صحيح           name_سائيز;
    u_char       *نالا;
    ngx_str_t     err;
} ngx_regex_compile_t;


تعريفنوع بنية {
    ngx_regex_t  *regex;
    u_char       *name;
} ngx_regex_elt_t;


فراغ ngx_regex_init(فراغ);
ngx_int_t ngx_regex_compile(ngx_regex_compile_t *rc);

ngx_int_t ngx_regex_exec(ngx_regex_t *re, ngx_str_t *s, صحيح *captures,
    ngx_uint_t سائيز);

#إذا (NGX_PCRE2)
#وصف_ڪريو ngx_regex_exec_n       "pcre2_match()"
#وإلا
#وصف_ڪريو ngx_regex_exec_n       "pcre_exec()"
#جيڪڏهن_پورو

ngx_int_t ngx_regex_exec_array(ngx_array_t *a, ngx_str_t *s, ngx_log_t *لاگرٿم);


#جيڪڏهن_پورو /* _NGX_REGEX_H_INCLUDED_ */
