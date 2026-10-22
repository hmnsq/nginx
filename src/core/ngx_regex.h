
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ _NGX_REGEX_H_INCLUDED_
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _NGX_REGEX_H_INCLUDED_


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>


#ਜੇ (NGX_PCRE2)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PCRE2_ਕੋਡ_UNIT_ਚੌੜਾਈ  8
#ਅਨ੍ਤਰ੍ਭਾਵ <pcre2.h>

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_REGEX_NO_MATCHED   PCRE2_ERROR_NOMATCH   /* -1 */

ਕਿਸਮਪਰਿਭਾਸ਼ਾ pcre2_ਕੋਡ  ngx_regex_t;

#ਨਹੀਂਤਾਂ

#ਅਨ੍ਤਰ੍ਭਾਵ <pcre.h>

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_REGEX_NO_MATCHED   PCRE_ERROR_NOMATCH    /* -1 */

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    pcre        *ਕੋਡ;
    pcre_extra  *extra;
} ngx_regex_t;

#ਜੇ_ਅੰਤ


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_REGEX_CASELESS     0x00000001
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_REGEX_MULTILINE    0x00000002


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ngx_str_t     ਨਮੂਨਾ;
    ngx_pool_t   *pool;
    ngx_uint_t    options;

    ngx_regex_t  *regex;
    ਪੂਰਨ           captures;
    ਪੂਰਨ           named_captures;
    ਪੂਰਨ           name_ਮਾਪ;
    u_char       *ਨਾਂ;
    ngx_str_t     err;
} ngx_regex_compile_t;


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ngx_regex_t  *regex;
    u_char       *name;
} ngx_regex_elt_t;


ਖਾਲੀ ngx_regex_init(ਖਾਲੀ);
ngx_int_t ngx_regex_compile(ngx_regex_compile_t *rc);

ngx_int_t ngx_regex_exec(ngx_regex_t *re, ngx_str_t *s, ਪੂਰਨ *captures,
    ngx_uint_t ਮਾਪ);

#ਜੇ (NGX_PCRE2)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_regex_exec_n       "pcre2_match()"
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_regex_exec_n       "pcre_exec()"
#ਜੇ_ਅੰਤ

ngx_int_t ngx_regex_exec_array(ngx_array_t *a, ngx_str_t *s, ngx_log_t *ਲਘੂਗਣਕ);


#ਜੇ_ਅੰਤ /* _NGX_REGEX_H_INCLUDED_ */
