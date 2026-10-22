
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अगर_अघोषित _NGX_REGEX_H_INCLUDED_
#घोषणा _NGX_REGEX_H_INCLUDED_


#समावेश <ngx_config.h>
#समावेश <ngx_core.h>


#अगर (NGX_PCRE2)

#घोषणा PCRE2_CODE_UNIT_WIDTH  8
#समावेश <pcre2.h>

#घोषणा NGX_REGEX_NO_MATCHED   PCRE2_ERROR_NOMATCH   /* -1 */

प्रकार pcre2_code  ngx_regex_t;

#अन्यथा

#समावेश <pcre.h>

#घोषणा NGX_REGEX_NO_MATCHED   PCRE_ERROR_NOMATCH    /* -1 */

प्रकार काष्ठा {
    pcre        *code;
    pcre_extra  *extra;
} ngx_regex_t;

#पूर्ण_अगर


#घोषणा NGX_REGEX_CASELESS     0x00000001
#घोषणा NGX_REGEX_MULTILINE    0x00000002


प्रकार काष्ठा {
    ngx_str_t     pattern;
    ngx_pool_t   *pool;
    ngx_uint_t    options;

    ngx_regex_t  *regex;
    पूर्णांक           captures;
    पूर्णांक           named_captures;
    पूर्णांक           name_आकार;
    u_char       *names;
    ngx_str_t     err;
} ngx_regex_compile_t;


प्रकार काष्ठा {
    ngx_regex_t  *regex;
    u_char       *name;
} ngx_regex_elt_t;


व्योम ngx_regex_init(व्योम);
ngx_int_t ngx_regex_compile(ngx_regex_compile_t *rc);

ngx_int_t ngx_regex_exec(ngx_regex_t *re, ngx_str_t *s, पूर्णांक *captures,
    ngx_uint_t आकार);

#अगर (NGX_PCRE2)
#घोषणा ngx_regex_exec_n       "pcre2_match()"
#अन्यथा
#घोषणा ngx_regex_exec_n       "pcre_exec()"
#पूर्ण_अगर

ngx_int_t ngx_regex_exec_array(ngx_array_t *a, ngx_str_t *s, ngx_log_t *log);


#पूर्ण_अगर /* _NGX_REGEX_H_INCLUDED_ */
