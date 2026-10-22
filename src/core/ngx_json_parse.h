
/*
 * Copyright (C) Maxim Dounin
 * Copyright (C) Nginx, Inc.
 */


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ _NGX_JSON_PARSE_H_INCLUDED_
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _NGX_JSON_PARSE_H_INCLUDED_


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_json_unescape.h>


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_JSON_SKIP                    -7
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_JSON_DEFAULT_MAX_ਡੂੰਘਾਈ       64


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਗਿਣਤੀ {
    NGX_JSON_OBJECT_OPEN = 0,
    NGX_JSON_OBJECT_CLOSE,
    NGX_JSON_ARRAY_OPEN,
    NGX_JSON_ARRAY_CLOSE,
    NGX_JSON_ਕੁੰਜੀ,
    NGX_JSON_ਕੁੰਜੀ_ESCAPED,
    NGX_JSON_ਮੁੱਲ_STRING,
    NGX_JSON_ਮੁੱਲ_STRING_ESCAPED,
    NGX_JSON_ਮੁੱਲ_ਸੰਖਿਆ,
    NGX_JSON_ਮੁੱਲ_BOOL,
    NGX_JSON_ਮੁੱਲ_NULL
} ngx_json_event_e;


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਗਿਣਤੀ {
    ngx_json_ਸ਼ੁਰੂ = 0,
    ngx_json_ਪੂਰਾ,
    ngx_json_object,
    ngx_json_object_ਅਗਲਾ,
    ngx_json_object_name_separator,
    ngx_json_ਮੁੱਲ,
    ngx_json_object_ਮੁੱਲ_separator,
    ngx_json_array_ਮੁੱਲ_separator,
    ngx_json_array,
    ngx_json_string,
    ngx_json_ਸੰਖਿਆ_minus,
    ngx_json_ਸੰਖਿਆ_int,
    ngx_json_ਸੰਖਿਆ_int_zero,
    ngx_json_ਸੰਖਿਆ_frac,
    ngx_json_ਸੰਖਿਆ_frac_digit,
    ngx_json_ਸੰਖਿਆ_exp,
    ngx_json_ਸੰਖਿਆ_exp_plusminus,
    ngx_json_ਸੰਖਿਆ_exp_digit,
    ngx_json_escaped,
    ngx_json_escaped_hex,       /* first \uXXXX: reading the four hex digits */
    ngx_json_surrogate_ਸ਼ੁਰੂ,   /* expect '\' beginning the low surrogate */
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


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਗਿਣਤੀ {
    ngx_json_ctx_none = 0,
    ngx_json_ctx_object,
    ngx_json_ctx_array
} ngx_json_container_e;


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ngx_json_ctx_s  ngx_json_ctx_t;


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ngx_int_t (*ngx_json_handler_pt)(ngx_json_ctx_t *ctx,
    ngx_json_event_e event, ngx_str_t *token);


ਬਣਤਰ ngx_json_ctx_s {
    ngx_json_state_e       state;

    ngx_json_container_e  *ਢੇਰ;
    ngx_uint_t             ਡੂੰਘਾਈ;
    ngx_uint_t             max_ਡੂੰਘਾਈ;

    uint32_t               codepoint;
    ngx_uint_t             hex_ਖੱਬਾ;         /* hex digits still expected */

    ngx_uint_t             skip_until_ਡੂੰਘਾਈ;

    ngx_pool_t            *pool;
    ngx_json_handler_pt    handler;
    ਖਾਲੀ                  *ਅੰਕੜੇ;

    ngx_uint_t             ਵਿੱਚ_ਕੁੰਜੀ;           /* unsigned in_key:1; */
    ngx_uint_t             escaped;          /* unsigned escaped:1; */
};


ਖਾਲੀ ngx_json_ctx_init(ngx_json_ctx_t *ctx, ngx_pool_t *pool);
ngx_int_t ngx_json_parse_ctx(ngx_json_ctx_t *ctx, u_char *ਅੰਕੜੇ, ਮਾਪ_t len);
ngx_int_t ngx_json_parse(ngx_pool_t *pool, ngx_str_t *json,
    ngx_json_handler_pt handler, ਖਾਲੀ *ਅੰਕੜੇ);


#ਜੇ_ਅੰਤ /* _NGX_JSON_PARSE_H_INCLUDED_ */
