
/*
 * Copyright (C) Maxim Dounin
 * Copyright (C) Nginx, Inc.
 */


#جيڪڏهن_وصف_نه_ٿيل _NGX_JSON_PARSE_H_INCLUDED_
#وصف_ڪريو _NGX_JSON_PARSE_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>
#ضمن <ngx_json_unescape.h>


#وصف_ڪريو NGX_JSON_SKIP                    -7
#وصف_ڪريو NGX_JSON_DEFAULT_MAX_اونهائي       64


تعريفنوع تعداد {
    NGX_JSON_OBJECT_OPEN = 0,
    NGX_JSON_OBJECT_CLOSE,
    NGX_JSON_ARRAY_OPEN,
    NGX_JSON_ARRAY_CLOSE,
    NGX_JSON_ڪنجي,
    NGX_JSON_ڪنجي_ESCAPED,
    NGX_JSON_قدر_STRING,
    NGX_JSON_قدر_STRING_ESCAPED,
    NGX_JSON_قدر_عدد,
    NGX_JSON_قدر_BOOL,
    NGX_JSON_قدر_NULL
} ngx_json_event_e;


تعريفنوع تعداد {
    ngx_json_شروعات = 0,
    ngx_json_پورو,
    ngx_json_object,
    ngx_json_object_ايندڙ,
    ngx_json_object_name_separator,
    ngx_json_قدر,
    ngx_json_object_قدر_separator,
    ngx_json_array_قدر_separator,
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
    ngx_json_surrogate_شروعات,   /* expect '\' beginning the low surrogate */
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

    ngx_json_container_e  *ڍير;
    ngx_uint_t             اونهائي;
    ngx_uint_t             max_اونهائي;

    uint32_t               codepoint;
    ngx_uint_t             hex_کاٻو;         /* hex digits still expected */

    ngx_uint_t             skip_until_اونهائي;

    ngx_pool_t            *pool;
    ngx_json_handler_pt    handler;
    فراغ                  *معلومات;

    ngx_uint_t             اندريون_ڪنجي;           /* unsigned in_key:1; */
    ngx_uint_t             escaped;          /* unsigned escaped:1; */
};


فراغ ngx_json_ctx_init(ngx_json_ctx_t *ctx, ngx_pool_t *pool);
ngx_int_t ngx_json_parse_ctx(ngx_json_ctx_t *ctx, u_char *معلومات, سائيز_t len);
ngx_int_t ngx_json_parse(ngx_pool_t *pool, ngx_str_t *json,
    ngx_json_handler_pt handler, فراغ *معلومات);


#جيڪڏهن_پورو /* _NGX_JSON_PARSE_H_INCLUDED_ */
