
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#నిర్వచితం_కాకపోతే _NGX_REGEX_H_INCLUDED_
#నిర్వచించు _NGX_REGEX_H_INCLUDED_


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>


#ఒకవేళ (NGX_PCRE2)

#నిర్వచించు PCRE2_సంకేతం_UNIT_వెడల్పు  8
#అన్తర్భావ <pcre2.h>

#నిర్వచించు NGX_REGEX_NO_MATCHED   PCRE2_ERROR_NOMATCH   /* -1 */

రకనిర్వచనం pcre2_సంకేతం  ngx_regex_t;

#లేకపోతే

#అన్తర్భావ <pcre.h>

#నిర్వచించు NGX_REGEX_NO_MATCHED   PCRE_ERROR_NOMATCH    /* -1 */

రకనిర్వచనం నిర్మాణం {
    pcre        *సంకేతం_2;
    pcre_extra  *extra;
} ngx_regex_t;

#అయితే_ముగింపు


#నిర్వచించు NGX_REGEX_CASELESS     0x00000001
#నిర్వచించు NGX_REGEX_MULTILINE    0x00000002


రకనిర్వచనం నిర్మాణం {
    ngx_str_t     నమూనా;
    ngx_pool_t   *pool;
    ngx_uint_t    options;

    ngx_regex_t  *regex;
    పూర్ణాంకం           captures;
    పూర్ణాంకం           named_captures;
    పూర్ణాంకం           name_కొలత;
    u_char       *పేర్లు;
    ngx_str_t     err;
} ngx_regex_compile_t;


రకనిర్వచనం నిర్మాణం {
    ngx_regex_t  *regex;
    u_char       *name;
} ngx_regex_elt_t;


శూన్య ngx_regex_init(శూన్య);
ngx_int_t ngx_regex_compile(ngx_regex_compile_t *rc);

ngx_int_t ngx_regex_exec(ngx_regex_t *re, ngx_str_t *s, పూర్ణాంకం *captures,
    ngx_uint_t కొలత);

#ఒకవేళ (NGX_PCRE2)
#నిర్వచించు ngx_regex_exec_n       "pcre2_match()"
#లేకపోతే
#నిర్వచించు ngx_regex_exec_n       "pcre_exec()"
#అయితే_ముగింపు

ngx_int_t ngx_regex_exec_array(ngx_array_t *a, ngx_str_t *s, ngx_log_t *సంవర్గమానం);


#అయితే_ముగింపు /* _NGX_REGEX_H_INCLUDED_ */
