
/*
 * Copyright (C) Maxim Dounin
 * Copyright (C) Nginx, Inc.
 */


#यदि_अपरिभाषितम् _NGX_JSON_PARSE_H_INCLUDED_
#परिभाषय _NGX_JSON_PARSE_H_INCLUDED_


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>
#अन्तर्भाव <ngx_json_unescape.h>


#परिभाषय NGX_JSON_SKIP                    -7
#परिभाषय NGX_JSON_DEFAULT_MAX_गभीरता       64


प्रकारनिर्देश गणना {
    NGX_JSON_OBJECT_OPEN = 0,
    NGX_JSON_OBJECT_CLOSE,
    NGX_JSON_ARRAY_OPEN,
    NGX_JSON_ARRAY_CLOSE,
    NGX_JSON_कुञ्चिका,
    NGX_JSON_कुञ्चिका_ESCAPED,
    NGX_JSON_मानम्_STRING,
    NGX_JSON_मानम्_STRING_ESCAPED,
    NGX_JSON_मानम्_सङ्ख्या,
    NGX_JSON_मानम्_BOOL,
    NGX_JSON_मानम्_NULL
} ngx_json_event_e;


प्रकारनिर्देश गणना {
    ngx_json_start = 0,
    ngx_json_समाप्तम्,
    ngx_json_object,
    ngx_json_object_अग्र,
    ngx_json_object_name_separator,
    ngx_json_मानम्,
    ngx_json_object_मानम्_separator,
    ngx_json_array_मानम्_separator,
    ngx_json_array,
    ngx_json_string,
    ngx_json_सङ्ख्या_minus,
    ngx_json_सङ्ख्या_int,
    ngx_json_सङ्ख्या_int_zero,
    ngx_json_सङ्ख्या_frac,
    ngx_json_सङ्ख्या_frac_digit,
    ngx_json_सङ्ख्या_exp,
    ngx_json_सङ्ख्या_exp_plusminus,
    ngx_json_सङ्ख्या_exp_digit,
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


प्रकारनिर्देश गणना {
    ngx_json_ctx_none = 0,
    ngx_json_ctx_object,
    ngx_json_ctx_array
} ngx_json_container_e;


प्रकारनिर्देश संरचना ngx_json_ctx_s  ngx_json_ctx_t;


प्रकारनिर्देश ngx_int_t (*ngx_json_handler_pt)(ngx_json_ctx_t *ctx,
    ngx_json_event_e event, ngx_str_t *token);


संरचना ngx_json_ctx_s {
    ngx_json_state_e       state;

    ngx_json_container_e  *राशिः;
    ngx_uint_t             गभीरता;
    ngx_uint_t             max_गभीरता;

    uint32_t               codepoint;
    ngx_uint_t             hex_वामः;         /* hex digits still expected */

    ngx_uint_t             skip_until_गभीरता;

    ngx_pool_t            *pool;
    ngx_json_handler_pt    handler;
    शून्य                  *दत्त;

    ngx_uint_t             अन्तर्_कुञ्चिका;           /* unsigned in_key:1; */
    ngx_uint_t             escaped;          /* unsigned escaped:1; */
};


शून्य ngx_json_ctx_init(ngx_json_ctx_t *ctx, ngx_pool_t *pool);
ngx_int_t ngx_json_parse_ctx(ngx_json_ctx_t *ctx, u_char *दत्त, आकार_t len);
ngx_int_t ngx_json_parse(ngx_pool_t *pool, ngx_str_t *json,
    ngx_json_handler_pt handler, शून्य *दत्त);


#यदि_अन्तः /* _NGX_JSON_PARSE_H_INCLUDED_ */
