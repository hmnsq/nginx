
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_REGEX_H_INCLUDED_
#define _NGX_REGEX_H_INCLUDED_


#অন্তর্ভাব <ngx_config.h>
#অন্তর্ভাব <ngx_core.h>


#যদি (NGX_PCRE2)

#define PCRE2_CODE_UNIT_WIDTH  8
#অন্তর্ভাব <pcre2.h>

#define NGX_REGEX_NO_MATCHED   PCRE2_ERROR_NOMATCH   /* -1 */

প্রকারলেখ pcre2_code  ngx_regex_t;

#অন্যথা

#অন্তর্ভাব <pcre.h>

#define NGX_REGEX_NO_MATCHED   PCRE_ERROR_NOMATCH    /* -1 */

প্রকারলেখ কাঠামো {
    pcre        *code;
    pcre_extra  *extra;
} ngx_regex_t;

#endif


#define NGX_REGEX_CASELESS     0x00000001
#define NGX_REGEX_MULTILINE    0x00000002


প্রকারলেখ কাঠামো {
    ngx_str_t     pattern;
    ngx_pool_t   *pool;
    ngx_uint_t    options;

    ngx_regex_t  *regex;
    পূর্ণসংখ্যা           captures;
    পূর্ণসংখ্যা           named_captures;
    পূর্ণসংখ্যা           name_আকার;
    u_char       *names;
    ngx_str_t     err;
} ngx_regex_compile_t;


প্রকারলেখ কাঠামো {
    ngx_regex_t  *regex;
    u_char       *name;
} ngx_regex_elt_t;


শূন্য ngx_regex_init(শূন্য);
ngx_int_t ngx_regex_compile(ngx_regex_compile_t *rc);

ngx_int_t ngx_regex_exec(ngx_regex_t *re, ngx_str_t *s, পূর্ণসংখ্যা *captures,
    ngx_uint_t আকার_2);

#যদি (NGX_PCRE2)
#define ngx_regex_exec_n       "pcre2_match()"
#অন্যথা
#define ngx_regex_exec_n       "pcre_exec()"
#endif

ngx_int_t ngx_regex_exec_array(ngx_array_t *a, ngx_str_t *s, ngx_log_t *log);


#endif /* _NGX_REGEX_H_INCLUDED_ */
