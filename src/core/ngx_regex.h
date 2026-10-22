
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#اگر_غیر_تعریف_شدہ _NGX_REGEX_H_INCLUDED_
#تعریف_کرو _NGX_REGEX_H_INCLUDED_


#شامل <ngx_config.h>
#شامل <ngx_core.h>


#اگر (NGX_PCRE2)

#تعریف_کرو PCRE2_رمز_UNIT_چوڑائی  8
#شامل <pcre2.h>

#تعریف_کرو NGX_REGEX_NO_MATCHED   PCRE2_ERROR_NOMATCH   /* -1 */

قسمتعریف pcre2_رمز  ngx_regex_t;

#ورنہ

#شامل <pcre.h>

#تعریف_کرو NGX_REGEX_NO_MATCHED   PCRE_ERROR_NOMATCH    /* -1 */

قسمتعریف ساخت {
    pcre        *رمز;
    pcre_extra  *extra;
} ngx_regex_t;

#اگر_ختم


#تعریف_کرو NGX_REGEX_CASELESS     0x00000001
#تعریف_کرو NGX_REGEX_MULTILINE    0x00000002


قسمتعریف ساخت {
    ngx_str_t     نمونہ;
    ngx_pool_t   *pool;
    ngx_uint_t    options;

    ngx_regex_t  *regex;
    صحیح           captures;
    صحیح           named_captures;
    صحیح           name_سائز;
    u_char       *نام;
    ngx_str_t     err;
} ngx_regex_compile_t;


قسمتعریف ساخت {
    ngx_regex_t  *regex;
    u_char       *name;
} ngx_regex_elt_t;


خالی ngx_regex_init(خالی);
ngx_int_t ngx_regex_compile(ngx_regex_compile_t *rc);

ngx_int_t ngx_regex_exec(ngx_regex_t *re, ngx_str_t *s, صحیح *captures,
    ngx_uint_t سائز);

#اگر (NGX_PCRE2)
#تعریف_کرو ngx_regex_exec_n       "pcre2_match()"
#ورنہ
#تعریف_کرو ngx_regex_exec_n       "pcre_exec()"
#اگر_ختم

ngx_int_t ngx_regex_exec_array(ngx_array_t *a, ngx_str_t *s, ngx_log_t *لوگارتھم);


#اگر_ختم /* _NGX_REGEX_H_INCLUDED_ */
