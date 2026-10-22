
/*
 * Copyright (C) Maxim Dounin
 * Copyright (C) Nginx, Inc.
 */


#اگر_تعریف_نشده _NGX_JSON_PARSE_H_INCLUDED_
#تعریف_کن _NGX_JSON_PARSE_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>
#ضمن <ngx_json_unescape.h>


#تعریف_کن NGX_JSON_SKIP                    -7
#تعریف_کن NGX_JSON_DEFAULT_MAX_عمق       64


تعريفنوع تعداد {
    NGX_JSON_OBJECT_OPEN = 0,
    NGX_JSON_OBJECT_CLOSE,
    NGX_JSON_ARRAY_OPEN,
    NGX_JSON_ARRAY_CLOSE,
    NGX_JSON_کلید,
    NGX_JSON_کلید_ESCAPED,
    NGX_JSON_مقدار_STRING,
    NGX_JSON_مقدار_STRING_ESCAPED,
    NGX_JSON_مقدار_عدد,
    NGX_JSON_مقدار_BOOL,
    NGX_JSON_مقدار_NULL
} ngx_json_event_e;


تعريفنوع تعداد {
    ngx_json_شروع = 0,
    ngx_json_انجامشده,
    ngx_json_object,
    ngx_json_object_بعدی,
    ngx_json_object_name_separator,
    ngx_json_مقدار,
    ngx_json_object_مقدار_separator,
    ngx_json_array_مقدار_separator,
    ngx_json_array,
    ngx_json_string,
    ngx_json_عدد_minus,
    ngx_json_عدد_int,
    ngx_json_عدد_int_zero,
    ngx_json_عدد_frac,
    ngx_json_عدد_frac_digit,
    ngx_json_عدد_exp,
    ngx_json_عدد_exp_plusminus,
    ngx_json_عدد_exp_digit,
    ngx_json_escaped,
    ngx_json_escaped_hex,       /* first \uXXXX: reading the four hex digits */
    ngx_json_surrogate_شروع,   /* expect '\' beginning the low surrogate */
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


تعريفنوع تعداد {
    ngx_json_ctx_none = 0,
    ngx_json_ctx_object,
    ngx_json_ctx_array
} ngx_json_container_e;


تعريفنوع بنية ngx_json_ctx_s  ngx_json_ctx_t;


تعريفنوع ngx_int_t (*ngx_json_handler_pt)(ngx_json_ctx_t *ctx,
    ngx_json_event_e event, ngx_str_t *token);


بنية ngx_json_ctx_s {
    ngx_json_state_e       state;

    ngx_json_container_e  *پشته;
    ngx_uint_t             عمق;
    ngx_uint_t             max_عمق;

    uint32_t               codepoint;
    ngx_uint_t             hex_چپ;         /* hex digits still expected */

    ngx_uint_t             skip_until_عمق;

    ngx_pool_t            *pool;
    ngx_json_handler_pt    handler;
    فراغ                  *داده;

    ngx_uint_t             درون_کلید;           /* unsigned in_key:1; */
    ngx_uint_t             escaped;          /* unsigned escaped:1; */
};


فراغ ngx_json_ctx_init(ngx_json_ctx_t *ctx, ngx_pool_t *pool);
ngx_int_t ngx_json_parse_ctx(ngx_json_ctx_t *ctx, u_char *داده, اندازه_t len);
ngx_int_t ngx_json_parse(ngx_pool_t *pool, ngx_str_t *json,
    ngx_json_handler_pt handler, فراغ *داده);


#پایان_اگر /* _NGX_JSON_PARSE_H_INCLUDED_ */
