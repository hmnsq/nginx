
/*
 * Copyright (C) Maxim Dounin
 * Copyright (C) Nginx, Inc.
 */


#నిర్వచితం_కాకపోతే _NGX_JSON_PARSE_H_INCLUDED_
#నిర్వచించు _NGX_JSON_PARSE_H_INCLUDED_


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>
#అన్తర్భావ <ngx_json_unescape.h>


#నిర్వచించు NGX_JSON_SKIP                    -7
#నిర్వచించు NGX_JSON_DEFAULT_MAX_లోతు       64


రకనిర్వచనం గణన {
    NGX_JSON_OBJECT_OPEN = 0,
    NGX_JSON_OBJECT_CLOSE,
    NGX_JSON_ARRAY_OPEN,
    NGX_JSON_ARRAY_CLOSE,
    NGX_JSON_కీలకం,
    NGX_JSON_కీలకం_ESCAPED,
    NGX_JSON_విలువ_STRING,
    NGX_JSON_విలువ_STRING_ESCAPED,
    NGX_JSON_విలువ_సంఖ్య,
    NGX_JSON_విలువ_BOOL,
    NGX_JSON_విలువ_NULL
} ngx_json_event_e;


రకనిర్వచనం గణన {
    ngx_json_ప్రారంభం = 0,
    ngx_json_పూర్తి,
    ngx_json_object,
    ngx_json_object_తదుపరి,
    ngx_json_object_name_separator,
    ngx_json_విలువ,
    ngx_json_object_విలువ_separator,
    ngx_json_array_విలువ_separator,
    ngx_json_array,
    ngx_json_string,
    ngx_json_సంఖ్య_minus,
    ngx_json_సంఖ్య_int,
    ngx_json_సంఖ్య_int_zero,
    ngx_json_సంఖ్య_frac,
    ngx_json_సంఖ్య_frac_digit,
    ngx_json_సంఖ్య_exp,
    ngx_json_సంఖ్య_exp_plusminus,
    ngx_json_సంఖ్య_exp_digit,
    ngx_json_escaped,
    ngx_json_escaped_hex,       /* first \uXXXX: reading the four hex digits */
    ngx_json_surrogate_ప్రారంభం,   /* expect '\' beginning the low surrogate */
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


రకనిర్వచనం గణన {
    ngx_json_ctx_none = 0,
    ngx_json_ctx_object,
    ngx_json_ctx_array
} ngx_json_container_e;


రకనిర్వచనం నిర్మాణం ngx_json_ctx_s  ngx_json_ctx_t;


రకనిర్వచనం ngx_int_t (*ngx_json_handler_pt)(ngx_json_ctx_t *ctx,
    ngx_json_event_e event, ngx_str_t *token);


నిర్మాణం ngx_json_ctx_s {
    ngx_json_state_e       state;

    ngx_json_container_e  *దొంతర;
    ngx_uint_t             లోతు;
    ngx_uint_t             max_లోతు;

    uint32_t               codepoint;
    ngx_uint_t             hex_ఎడమ;         /* hex digits still expected */

    ngx_uint_t             skip_until_లోతు;

    ngx_pool_t            *pool;
    ngx_json_handler_pt    handler;
    శూన్య                  *దత్తాంశం;

    ngx_uint_t             లోపల_కీలకం;           /* unsigned in_key:1; */
    ngx_uint_t             escaped;          /* unsigned escaped:1; */
};


శూన్య ngx_json_ctx_init(ngx_json_ctx_t *ctx, ngx_pool_t *pool);
ngx_int_t ngx_json_parse_ctx(ngx_json_ctx_t *ctx, u_char *దత్తాంశం, కొలత_t len);
ngx_int_t ngx_json_parse(ngx_pool_t *pool, ngx_str_t *json,
    ngx_json_handler_pt handler, శూన్య *దత్తాంశం);


#అయితే_ముగింపు /* _NGX_JSON_PARSE_H_INCLUDED_ */
