
/*
 * Copyright (C) Maxim Dounin
 * Copyright (C) Nginx, Inc.
 */


#اگر_غیر_تعریف_شدہ _NGX_JSON_PARSE_H_INCLUDED_
#تعریف_کرو _NGX_JSON_PARSE_H_INCLUDED_


#شامل <ngx_config.h>
#شامل <ngx_core.h>
#شامل <ngx_json_unescape.h>


#تعریف_کرو NGX_JSON_SKIP                    -7
#تعریف_کرو NGX_JSON_DEFAULT_MAX_گہرائی       64


قسمتعریف شمار {
    NGX_JSON_OBJECT_OPEN = 0,
    NGX_JSON_OBJECT_CLOSE,
    NGX_JSON_ARRAY_OPEN,
    NGX_JSON_ARRAY_CLOSE,
    NGX_JSON_کلید,
    NGX_JSON_کلید_ESCAPED,
    NGX_JSON_قدر_STRING,
    NGX_JSON_قدر_STRING_ESCAPED,
    NGX_JSON_قدر_نمبر,
    NGX_JSON_قدر_BOOL,
    NGX_JSON_قدر_NULL
} ngx_json_event_e;


قسمتعریف شمار {
    ngx_json_آغاز = 0,
    ngx_json_مکمل,
    ngx_json_object,
    ngx_json_object_اگلا,
    ngx_json_object_name_separator,
    ngx_json_قدر,
    ngx_json_object_قدر_separator,
    ngx_json_array_قدر_separator,
    ngx_json_array,
    ngx_json_string,
    ngx_json_نمبر_minus,
    ngx_json_نمبر_int,
    ngx_json_نمبر_int_zero,
    ngx_json_نمبر_frac,
    ngx_json_نمبر_frac_digit,
    ngx_json_نمبر_exp,
    ngx_json_نمبر_exp_plusminus,
    ngx_json_نمبر_exp_digit,
    ngx_json_escaped,
    ngx_json_escaped_hex,       /* first \uXXXX: reading the four hex digits */
    ngx_json_surrogate_آغاز,   /* expect '\' beginning the low surrogate */
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


قسمتعریف شمار {
    ngx_json_ctx_none = 0,
    ngx_json_ctx_object,
    ngx_json_ctx_array
} ngx_json_container_e;


قسمتعریف ساخت ngx_json_ctx_s  ngx_json_ctx_t;


قسمتعریف ngx_int_t (*ngx_json_handler_pt)(ngx_json_ctx_t *ctx,
    ngx_json_event_e event, ngx_str_t *token);


ساخت ngx_json_ctx_s {
    ngx_json_state_e       state;

    ngx_json_container_e  *ڈھیر;
    ngx_uint_t             گہرائی;
    ngx_uint_t             max_گہرائی;

    uint32_t               codepoint;
    ngx_uint_t             hex_بایاں;         /* hex digits still expected */

    ngx_uint_t             skip_until_گہرائی;

    ngx_pool_t            *pool;
    ngx_json_handler_pt    handler;
    خالی                  *ڈیٹا;

    ngx_uint_t             اندر_کلید;           /* unsigned in_key:1; */
    ngx_uint_t             escaped;          /* unsigned escaped:1; */
};


خالی ngx_json_ctx_init(ngx_json_ctx_t *ctx, ngx_pool_t *pool);
ngx_int_t ngx_json_parse_ctx(ngx_json_ctx_t *ctx, u_char *ڈیٹا, سائز_t len);
ngx_int_t ngx_json_parse(ngx_pool_t *pool, ngx_str_t *json,
    ngx_json_handler_pt handler, خالی *ڈیٹا);


#اگر_ختم /* _NGX_JSON_PARSE_H_INCLUDED_ */
