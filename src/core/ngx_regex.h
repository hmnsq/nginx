
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_REGEX_H_INCLUDED_
#define _NGX_REGEX_H_INCLUDED_


#ଅନ୍ତର୍ଭାଵ <ngx_config.h>
#ଅନ୍ତର୍ଭାଵ <ngx_core.h>


#ଯଦି (NGX_PCRE2)

#define PCRE2_CODE_UNIT_WIDTH  8
#ଅନ୍ତର୍ଭାଵ <pcre2.h>

#define NGX_REGEX_NO_MATCHED   PCRE2_ERROR_NOMATCH   /* -1 */

ପ୍ରକାରବ୍ୟାଖ୍ୟା pcre2_code  ngx_regex_t;

#ଅନ୍ୟଥା

#ଅନ୍ତର୍ଭାଵ <pcre.h>

#define NGX_REGEX_NO_MATCHED   PCRE_ERROR_NOMATCH    /* -1 */

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    pcre        *code;
    pcre_extra  *extra;
} ngx_regex_t;

#endif


#define NGX_REGEX_CASELESS     0x00000001
#define NGX_REGEX_MULTILINE    0x00000002


ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ngx_str_t     pattern;
    ngx_pool_t   *pool;
    ngx_uint_t    options;

    ngx_regex_t  *regex;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା           captures;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା           named_captures;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା           name_size;
    u_char       *names;
    ngx_str_t     err;
} ngx_regex_compile_t;


ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ngx_regex_t  *regex;
    u_char       *name;
} ngx_regex_elt_t;


ଶୂନ୍ୟ ngx_regex_init(ଶୂନ୍ୟ);
ngx_int_t ngx_regex_compile(ngx_regex_compile_t *rc);

ngx_int_t ngx_regex_exec(ngx_regex_t *re, ngx_str_t *s, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *captures,
    ngx_uint_t size);

#ଯଦି (NGX_PCRE2)
#define ngx_regex_exec_n       "pcre2_match()"
#ଅନ୍ୟଥା
#define ngx_regex_exec_n       "pcre_exec()"
#endif

ngx_int_t ngx_regex_exec_array(ngx_array_t *a, ngx_str_t *s, ngx_log_t *log);


#endif /* _NGX_REGEX_H_INCLUDED_ */
