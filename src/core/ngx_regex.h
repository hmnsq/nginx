
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ _NGX_REGEX_H_INCLUDED_
#ವ್ಯಾಖ್ಯಾನಿಸು _NGX_REGEX_H_INCLUDED_


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


#ಆದರೆ (NGX_PCRE2)

#ವ್ಯಾಖ್ಯಾನಿಸು PCRE2_ಸಂಕೇತ_UNIT_ಅಗಲ  8
#ಅನ್ತರ್ಭಾವ <pcre2.h>

#ವ್ಯಾಖ್ಯಾನಿಸು NGX_REGEX_NO_MATCHED   PCRE2_ERROR_NOMATCH   /* -1 */

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ pcre2_ಸಂಕೇತ  ngx_regex_t;

#ಇಲ್ಲದಿದ್ದರೆ

#ಅನ್ತರ್ಭಾವ <pcre.h>

#ವ್ಯಾಖ್ಯಾನಿಸು NGX_REGEX_NO_MATCHED   PCRE_ERROR_NOMATCH    /* -1 */

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    pcre        *ಸಂಕೇತ_2;
    pcre_extra  *extra;
} ngx_regex_t;

#ಆದರೆ_ಅಂತ್ಯ


#ವ್ಯಾಖ್ಯಾನಿಸು NGX_REGEX_CASELESS     0x00000001
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_REGEX_MULTILINE    0x00000002


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ngx_str_t     ನಮೂನೆ;
    ngx_pool_t   *pool;
    ngx_uint_t    options;

    ngx_regex_t  *regex;
    ಪೂರ್ಣಾಂಕ           captures;
    ಪೂರ್ಣಾಂಕ           named_captures;
    ಪೂರ್ಣಾಂಕ           name_ಗಾತ್ರ;
    u_char       *ಹೆಸರುಗಳು;
    ngx_str_t     err;
} ngx_regex_compile_t;


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ngx_regex_t  *regex;
    u_char       *name;
} ngx_regex_elt_t;


ಶೂನ್ಯ ngx_regex_init(ಶೂನ್ಯ);
ngx_int_t ngx_regex_compile(ngx_regex_compile_t *rc);

ngx_int_t ngx_regex_exec(ngx_regex_t *re, ngx_str_t *s, ಪೂರ್ಣಾಂಕ *captures,
    ngx_uint_t ಗಾತ್ರ_2);

#ಆದರೆ (NGX_PCRE2)
#ವ್ಯಾಖ್ಯಾನಿಸು ngx_regex_exec_n       "pcre2_match()"
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು ngx_regex_exec_n       "pcre_exec()"
#ಆದರೆ_ಅಂತ್ಯ

ngx_int_t ngx_regex_exec_array(ngx_array_t *a, ngx_str_t *s, ngx_log_t *ಲಘುಗಣಕ);


#ಆದರೆ_ಅಂತ್ಯ /* _NGX_REGEX_H_INCLUDED_ */
