
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#यदि_अपरिभाषितम् _NGX_REGEX_H_INCLUDED_
#परिभाषय _NGX_REGEX_H_INCLUDED_


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


#यदि (NGX_PCRE2)

#परिभाषय PCRE2_संहिता_UNIT_विस्तारः  8
#अन्तर्भाव <pcre2.h>

#परिभाषय NGX_REGEX_NO_MATCHED   PCRE2_ERROR_NOMATCH   /* -1 */

प्रकारनिर्देश pcre2_संहिता  ngx_regex_t;

#अन्यथा

#अन्तर्भाव <pcre.h>

#परिभाषय NGX_REGEX_NO_MATCHED   PCRE_ERROR_NOMATCH    /* -1 */

प्रकारनिर्देश संरचना {
    pcre        *संहिता;
    pcre_extra  *extra;
} ngx_regex_t;

#यदि_अन्तः


#परिभाषय NGX_REGEX_CASELESS     0x00000001
#परिभाषय NGX_REGEX_MULTILINE    0x00000002


प्रकारनिर्देश संरचना {
    ngx_str_t     आदर्शः;
    ngx_pool_t   *pool;
    ngx_uint_t    options;

    ngx_regex_t  *regex;
    पूर्णाङ्क           captures;
    पूर्णाङ्क           named_captures;
    पूर्णाङ्क           name_आकार;
    u_char       *नामानि;
    ngx_str_t     err;
} ngx_regex_compile_t;


प्रकारनिर्देश संरचना {
    ngx_regex_t  *regex;
    u_char       *name;
} ngx_regex_elt_t;


शून्य ngx_regex_init(शून्य);
ngx_int_t ngx_regex_compile(ngx_regex_compile_t *rc);

ngx_int_t ngx_regex_exec(ngx_regex_t *re, ngx_str_t *s, पूर्णाङ्क *captures,
    ngx_uint_t आकार);

#यदि (NGX_PCRE2)
#परिभाषय ngx_regex_exec_n       "pcre2_match()"
#अन्यथा
#परिभाषय ngx_regex_exec_n       "pcre_exec()"
#यदि_अन्तः

ngx_int_t ngx_regex_exec_array(ngx_array_t *a, ngx_str_t *s, ngx_log_t *लघुगणकः);


#यदि_अन्तः /* _NGX_REGEX_H_INCLUDED_ */
