
/*
 * Copyright (C) Maxim Dounin
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_JSON_PARSE_H_INCLUDED_
#define _NGX_JSON_PARSE_H_INCLUDED_


#அந்தர்பாவ <ngx_config.h>
#அந்தர்பாவ <ngx_core.h>
#அந்தர்பாவ <ngx_json_unescape.h>


#define NGX_JSON_SKIP                    -7
#define NGX_JSON_DEFAULT_MAX_DEPTH       64


வகைவரை எண்ணிக்கை {
    NGX_JSON_OBJECT_OPEN = 0,
    NGX_JSON_OBJECT_CLOSE,
    NGX_JSON_ARRAY_OPEN,
    NGX_JSON_ARRAY_CLOSE,
    NGX_JSON_சாவி,
    NGX_JSON_சாவி_ESCAPED,
    NGX_JSON_மதிப்பு_STRING,
    NGX_JSON_மதிப்பு_STRING_ESCAPED,
    NGX_JSON_மதிப்பு_எண்,
    NGX_JSON_மதிப்பு_BOOL,
    NGX_JSON_மதிப்பு_NULL
} ngx_json_event_e;


வகைவரை எண்ணிக்கை {
    ngx_json_start = 0,
    ngx_json_done,
    ngx_json_object,
    ngx_json_object_next,
    ngx_json_object_name_separator,
    ngx_json_மதிப்பு,
    ngx_json_object_மதிப்பு_separator,
    ngx_json_array_மதிப்பு_separator,
    ngx_json_array,
    ngx_json_string,
    ngx_json_எண்_minus,
    ngx_json_எண்_int,
    ngx_json_எண்_int_zero,
    ngx_json_எண்_frac,
    ngx_json_எண்_frac_digit,
    ngx_json_எண்_exp,
    ngx_json_எண்_exp_plusminus,
    ngx_json_எண்_exp_digit,
    ngx_json_escaped,
    ngx_json_escaped_hex,       /* first \uXXXX: reading the four hex digits */
    ngx_json_surrogate_start,   /* expect '\' beginning the low surrogate */
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


வகைவரை எண்ணிக்கை {
    ngx_json_ctx_none = 0,
    ngx_json_ctx_object,
    ngx_json_ctx_array
} ngx_json_container_e;


வகைவரை கட்டமைப்பு ngx_json_ctx_s  ngx_json_ctx_t;


வகைவரை ngx_int_t (*ngx_json_handler_pt)(ngx_json_ctx_t *ctx,
    ngx_json_event_e event, ngx_str_t *token);


கட்டமைப்பு ngx_json_ctx_s {
    ngx_json_state_e       state;

    ngx_json_container_e  *அடுக்கு;
    ngx_uint_t             depth;
    ngx_uint_t             max_depth;

    uint32_t               codepoint;
    ngx_uint_t             hex_இடது;         /* hex digits still expected */

    ngx_uint_t             skip_until_depth;

    ngx_pool_t            *pool;
    ngx_json_handler_pt    handler;
    வெறுமை                  *data;

    ngx_uint_t             in_சாவி;           /* unsigned in_key:1; */
    ngx_uint_t             escaped;          /* unsigned escaped:1; */
};


வெறுமை ngx_json_ctx_init(ngx_json_ctx_t *ctx, ngx_pool_t *pool);
ngx_int_t ngx_json_parse_ctx(ngx_json_ctx_t *ctx, u_char *data, size_t len);
ngx_int_t ngx_json_parse(ngx_pool_t *pool, ngx_str_t *json,
    ngx_json_handler_pt handler, வெறுமை *data);


#endif /* _NGX_JSON_PARSE_H_INCLUDED_ */
