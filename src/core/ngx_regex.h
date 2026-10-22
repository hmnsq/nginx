
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_REGEX_H_INCLUDED_
#define _NGX_REGEX_H_INCLUDED_


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


#ಯದಿ (NGX_PCRE2)

#define PCRE2_CODE_UNIT_WIDTH  8
#ಅನ್ತರ್ಭಾವ <pcre2.h>

#define NGX_REGEX_NO_MATCHED   PCRE2_ERROR_NOMATCH   /* -1 */

ಪ್ರಕಾರನಿರ್ದೇಶ pcre2_code  ngx_regex_t;

#ಅನ್ಯಥಾ

#ಅನ್ತರ್ಭಾವ <pcre.h>

#define NGX_REGEX_NO_MATCHED   PCRE_ERROR_NOMATCH    /* -1 */

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    pcre        *code;
    pcre_extra  *extra;
} ngx_regex_t;

#endif


#define NGX_REGEX_CASELESS     0x00000001
#define NGX_REGEX_MULTILINE    0x00000002


ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    ngx_str_t     pattern;
    ngx_pool_t   *pool;
    ngx_uint_t    options;

    ngx_regex_t  *regex;
    ಪೂರ್ಣಾಙ್ಕ           captures;
    ಪೂರ್ಣಾಙ್ಕ           named_captures;
    ಪೂರ್ಣಾಙ್ಕ           name_size;
    u_char       *names;
    ngx_str_t     err;
} ngx_regex_compile_t;


ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    ngx_regex_t  *regex;
    u_char       *name;
} ngx_regex_elt_t;


ಶೂನ್ಯ ngx_regex_init(ಶೂನ್ಯ);
ngx_int_t ngx_regex_compile(ngx_regex_compile_t *rc);

ngx_int_t ngx_regex_exec(ngx_regex_t *re, ngx_str_t *s, ಪೂರ್ಣಾಙ್ಕ *captures,
    ngx_uint_t size);

#ಯದಿ (NGX_PCRE2)
#define ngx_regex_exec_n       "pcre2_match()"
#ಅನ್ಯಥಾ
#define ngx_regex_exec_n       "pcre_exec()"
#endif

ngx_int_t ngx_regex_exec_array(ngx_array_t *a, ngx_str_t *s, ngx_log_t *log);


#endif /* _NGX_REGEX_H_INCLUDED_ */
