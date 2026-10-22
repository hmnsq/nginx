
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_REGEX_H_INCLUDED_
#define _NGX_REGEX_H_INCLUDED_


#അന്തര്ഭാവ <ngx_config.h>
#അന്തര്ഭാവ <ngx_core.h>


#എങ്കിൽ (NGX_PCRE2)

#define PCRE2_CODE_UNIT_WIDTH  8
#അന്തര്ഭാവ <pcre2.h>

#define NGX_REGEX_NO_MATCHED   PCRE2_ERROR_NOMATCH   /* -1 */

തരനിർവചനം pcre2_code  ngx_regex_t;

#അല്ലെങ്കിൽ

#അന്തര്ഭാവ <pcre.h>

#define NGX_REGEX_NO_MATCHED   PCRE_ERROR_NOMATCH    /* -1 */

തരനിർവചനം ഘടന {
    pcre        *code;
    pcre_extra  *extra;
} ngx_regex_t;

#endif


#define NGX_REGEX_CASELESS     0x00000001
#define NGX_REGEX_MULTILINE    0x00000002


തരനിർവചനം ഘടന {
    ngx_str_t     pattern;
    ngx_pool_t   *pool;
    ngx_uint_t    options;

    ngx_regex_t  *regex;
    പൂർണ്ണസംഖ്യ           captures;
    പൂർണ്ണസംഖ്യ           named_captures;
    പൂർണ്ണസംഖ്യ           name_size;
    u_char       *names;
    ngx_str_t     err;
} ngx_regex_compile_t;


തരനിർവചനം ഘടന {
    ngx_regex_t  *regex;
    u_char       *name;
} ngx_regex_elt_t;


ശൂന്യം ngx_regex_init(ശൂന്യം);
ngx_int_t ngx_regex_compile(ngx_regex_compile_t *rc);

ngx_int_t ngx_regex_exec(ngx_regex_t *re, ngx_str_t *s, പൂർണ്ണസംഖ്യ *captures,
    ngx_uint_t size);

#എങ്കിൽ (NGX_PCRE2)
#define ngx_regex_exec_n       "pcre2_match()"
#അല്ലെങ്കിൽ
#define ngx_regex_exec_n       "pcre_exec()"
#endif

ngx_int_t ngx_regex_exec_array(ngx_array_t *a, ngx_str_t *s, ngx_log_t *log);


#endif /* _NGX_REGEX_H_INCLUDED_ */
