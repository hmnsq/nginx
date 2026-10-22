
/*
 * Copyright (C) Maxim Dounin
 * Copyright (C) Nginx, Inc.
 */


#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ _NGX_JSON_PARSE_H_INCLUDED_
#ವ್ಯಾಖ್ಯಾನಿಸು _NGX_JSON_PARSE_H_INCLUDED_


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>
#ಅನ್ತರ್ಭಾವ <ngx_json_unescape.h>


#ವ್ಯಾಖ್ಯಾನಿಸು NGX_JSON_SKIP                    -7
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_JSON_DEFAULT_MAX_ಆಳ       64


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಎಣಿಕೆ {
    NGX_JSON_OBJECT_OPEN = 0,
    NGX_JSON_OBJECT_CLOSE,
    NGX_JSON_ARRAY_OPEN,
    NGX_JSON_ARRAY_CLOSE,
    NGX_JSON_ಕೀಲಿ,
    NGX_JSON_ಕೀಲಿ_ESCAPED,
    NGX_JSON_ಮೌಲ್ಯ_STRING,
    NGX_JSON_ಮೌಲ್ಯ_STRING_ESCAPED,
    NGX_JSON_ಮೌಲ್ಯ_ಸಂಖ್ಯೆ,
    NGX_JSON_ಮೌಲ್ಯ_BOOL,
    NGX_JSON_ಮೌಲ್ಯ_NULL
} ngx_json_event_e;


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಎಣಿಕೆ {
    ngx_json_ಆರಂಭ = 0,
    ngx_json_ಮುಗಿದ,
    ngx_json_object,
    ngx_json_object_ಮುಂದಿನ,
    ngx_json_object_name_separator,
    ngx_json_ಮೌಲ್ಯ,
    ngx_json_object_ಮೌಲ್ಯ_separator,
    ngx_json_array_ಮೌಲ್ಯ_separator,
    ngx_json_array,
    ngx_json_string,
    ngx_json_ಸಂಖ್ಯೆ_minus,
    ngx_json_ಸಂಖ್ಯೆ_int,
    ngx_json_ಸಂಖ್ಯೆ_int_zero,
    ngx_json_ಸಂಖ್ಯೆ_frac,
    ngx_json_ಸಂಖ್ಯೆ_frac_digit,
    ngx_json_ಸಂಖ್ಯೆ_exp,
    ngx_json_ಸಂಖ್ಯೆ_exp_plusminus,
    ngx_json_ಸಂಖ್ಯೆ_exp_digit,
    ngx_json_escaped,
    ngx_json_escaped_hex,       /* first \uXXXX: reading the four hex digits */
    ngx_json_surrogate_ಆರಂಭ,   /* expect '\' beginning the low surrogate */
    ngx_json_surrogate_u,       /* expect 'u' of the low surrogate */
    ngx_json_surrogate_hex,     /* low \uXXXX: reading the four hex digits */
    ngx_json_true_t,
    ngx_json_true_tr,
    ngx_json_true_tru,
    ngx_json_false_f,
    ngx_json_false_fa,
    ngx_json_false_fal,
    ngx_json_false_fals,
    ngx_json_null_n,
    ngx_json_null_nu,
    ngx_json_null_nul
} ngx_json_state_e;


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಎಣಿಕೆ {
    ngx_json_ctx_none = 0,
    ngx_json_ctx_object,
    ngx_json_ctx_array
} ngx_json_container_e;


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ ngx_json_ctx_s  ngx_json_ctx_t;


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ngx_int_t (*ngx_json_handler_pt)(ngx_json_ctx_t *ctx,
    ngx_json_event_e event, ngx_str_t *token);


ರಚನೆ ngx_json_ctx_s {
    ngx_json_state_e       state;

    ngx_json_container_e  *ರಾಶಿ;
    ngx_uint_t             ಆಳ;
    ngx_uint_t             max_ಆಳ;

    uint32_t               codepoint;
    ngx_uint_t             hex_ಎಡ;         /* hex digits still expected */

    ngx_uint_t             skip_until_ಆಳ;

    ngx_pool_t            *pool;
    ngx_json_handler_pt    handler;
    ಶೂನ್ಯ                  *ದತ್ತಾಂಶ;

    ngx_uint_t             ಒಳ_ಕೀಲಿ;           /* unsigned in_key:1; */
    ngx_uint_t             escaped;          /* unsigned escaped:1; */
};


ಶೂನ್ಯ ngx_json_ctx_init(ngx_json_ctx_t *ctx, ngx_pool_t *pool);
ngx_int_t ngx_json_parse_ctx(ngx_json_ctx_t *ctx, u_char *ದತ್ತಾಂಶ, ಗಾತ್ರ_t len);
ngx_int_t ngx_json_parse(ngx_pool_t *pool, ngx_str_t *json,
    ngx_json_handler_pt handler, ಶೂನ್ಯ *ದತ್ತಾಂಶ);


#ಆದರೆ_ಅಂತ್ಯ /* _NGX_JSON_PARSE_H_INCLUDED_ */
