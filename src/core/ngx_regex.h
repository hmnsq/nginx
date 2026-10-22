
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_REGEX_H_INCLUDED_
#define _NGX_REGEX_H_INCLUDED_


#અન્તર્ભાવ <ngx_config.h>
#અન્તર્ભાવ <ngx_core.h>


#જો (NGX_PCRE2)

#define PCRE2_CODE_UNIT_WIDTH  8
#અન્તર્ભાવ <pcre2.h>

#define NGX_REGEX_NO_MATCHED   PCRE2_ERROR_NOMATCH   /* -1 */

પ્રકારવ્યાખ્યા pcre2_code  ngx_regex_t;

#નહીંતર

#અન્તર્ભાવ <pcre.h>

#define NGX_REGEX_NO_MATCHED   PCRE_ERROR_NOMATCH    /* -1 */

પ્રકારવ્યાખ્યા સંરચના {
    pcre        *code;
    pcre_extra  *extra;
} ngx_regex_t;

#endif


#define NGX_REGEX_CASELESS     0x00000001
#define NGX_REGEX_MULTILINE    0x00000002


પ્રકારવ્યાખ્યા સંરચના {
    ngx_str_t     pattern;
    ngx_pool_t   *pool;
    ngx_uint_t    options;

    ngx_regex_t  *regex;
    પૂર્ણાંક           captures;
    પૂર્ણાંક           named_captures;
    પૂર્ણાંક           name_size;
    u_char       *names;
    ngx_str_t     err;
} ngx_regex_compile_t;


પ્રકારવ્યાખ્યા સંરચના {
    ngx_regex_t  *regex;
    u_char       *name;
} ngx_regex_elt_t;


શૂન્ય ngx_regex_init(શૂન્ય);
ngx_int_t ngx_regex_compile(ngx_regex_compile_t *rc);

ngx_int_t ngx_regex_exec(ngx_regex_t *re, ngx_str_t *s, પૂર્ણાંક *captures,
    ngx_uint_t size);

#જો (NGX_PCRE2)
#define ngx_regex_exec_n       "pcre2_match()"
#નહીંતર
#define ngx_regex_exec_n       "pcre_exec()"
#endif

ngx_int_t ngx_regex_exec_array(ngx_array_t *a, ngx_str_t *s, ngx_log_t *log);


#endif /* _NGX_REGEX_H_INCLUDED_ */
