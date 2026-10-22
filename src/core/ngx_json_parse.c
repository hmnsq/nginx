
/*
 * Copyright (C) Maxim Dounin
 * Copyright (C) Nginx, Inc.
 */


/*
 * JSON parser. RFC 8259 for the JSON grammar.

 * Site http://json.org/ may be useful too.
 */


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_json_parse.h>


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_JSON_NOT_SKIPPING  (ngx_uint_t) -1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_json_skipping(d)   ((d) != NGX_JSON_NOT_SKIPPING)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_json_ws(c)  ((c) == ' ' || (c) == '\t' || (c) == CR || (c) == LF)


ਸਥਿਰ ngx_inline ngx_int_t ngx_json_ਸਿਰਾ_ਸੰਖਿਆ(ngx_json_ctx_t *ctx,
    u_char *ਸ਼ੁਰੂ, u_char *ਸਿਰਾ, ngx_json_state_e *state);
ਸਥਿਰ ngx_inline ngx_int_t ngx_json_emit(ngx_json_ctx_t *ctx,
    ngx_json_event_e event, u_char *ਅੰਕੜੇ, ਮਾਪ_t len);
ਸਥਿਰ ngx_inline ngx_json_state_e ngx_json_after_ਮੁੱਲ(ngx_json_ctx_t *ctx);
ਸਥਿਰ ngx_inline ngx_int_t ngx_json_close(ngx_json_ctx_t *ctx,
    ngx_json_event_e event, ngx_json_state_e *state, u_char *p);
ਸਥਿਰ ngx_int_t ngx_json_ਧੱਕੋ_state(ngx_json_ctx_t *ctx,
    ngx_json_container_e container);


ਖਾਲੀ
ngx_json_ctx_init(ngx_json_ctx_t *ctx, ngx_pool_t *pool)
{
    ctx->state = ngx_json_ਸ਼ੁਰੂ;
    ctx->pool = pool;
    ctx->handler = ਨਿਰਾ;
    ctx->ਅੰਕੜੇ = ਨਿਰਾ;
    ctx->codepoint = 0;
    ctx->hex_ਖੱਬਾ = 0;

    ctx->ਡੂੰਘਾਈ = 0;
    ctx->skip_until_ਡੂੰਘਾਈ = NGX_JSON_NOT_SKIPPING;
    ctx->max_ਡੂੰਘਾਈ = NGX_JSON_DEFAULT_MAX_ਡੂੰਘਾਈ;

    /* stack is allocated lazily on first push, sized to max_depth */

    ctx->ਢੇਰ = ਨਿਰਾ;

    ctx->ਵਿੱਚ_ਕੁੰਜੀ = 0;
    ctx->escaped = 0;
}


ngx_int_t
ngx_json_parse_ctx(ngx_json_ctx_t *ctx, u_char *ਅੰਕੜੇ, ਮਾਪ_t len)
{
    u_char            *p, *ਆਖਰੀ, *ਸ਼ੁਰੂ;
    ngx_int_t          rc, hd;
    ngx_uint_t         skip_ਡੂੰਘਾਈ;
    ngx_json_event_e   event;
    ngx_json_state_e   state;

    ਜੇ (len == 0) {
        ਵਾਪਸ NGX_DECLINED;
    }

    ਜੇ (ਅੰਕੜੇ == ਨਿਰਾ) {
        ਵਾਪਸ NGX_ERROR;
    }

    ਜੇ (ctx->state == ngx_json_ਪੂਰਾ) {
        ਵਾਪਸ NGX_DECLINED;
    }

    ਆਖਰੀ = ਅੰਕੜੇ + len;
    state = ctx->state;
    skip_ਡੂੰਘਾਈ = ctx->skip_until_ਡੂੰਘਾਈ;
    ਸ਼ੁਰੂ = ਨਿਰਾ;

    ਲਈ (p = ਅੰਕੜੇ; p < ਆਖਰੀ; p++) {

        /* skipping suppresses emission only; the machine still validates */

        ਜੇ (ngx_json_skipping(skip_ਡੂੰਘਾਈ) && ctx->ਡੂੰਘਾਈ <= skip_ਡੂੰਘਾਈ
            && (state == ngx_json_object_ਮੁੱਲ_separator
                || state == ngx_json_array_ਮੁੱਲ_separator
                || state == ngx_json_ਪੂਰਾ))
        {
            skip_ਡੂੰਘਾਈ = NGX_JSON_NOT_SKIPPING;
            ctx->skip_until_ਡੂੰਘਾਈ = NGX_JSON_NOT_SKIPPING;
            p--;

            ਜਾਰੀ;
        }

        ਚੋਣ (state) {

        ਹਾਲਤ ngx_json_ਸ਼ੁਰੂ:
            ਜੇ (ngx_json_ws(*p)) {
                ਰੋਕੋ;
            }

            p--;
            state = ngx_json_ਮੁੱਲ;
            ਰੋਕੋ;

        ਹਾਲਤ ngx_json_object:
            ਜੇ (ngx_json_ws(*p)) {
                ਰੋਕੋ;
            }

            ਜੇ (*p == '}') {
                rc = ngx_json_close(ctx, NGX_JSON_OBJECT_CLOSE, &state, p);
                ਜੇ (rc != NGX_OK) {
                    ਵਾਪਸ rc;
                }

                ਰੋਕੋ;
            }

            /* fall through */

        ਹਾਲਤ ngx_json_object_ਅਗਲਾ:
            ਜੇ (ngx_json_ws(*p)) {
                ਰੋਕੋ;
            }

            ਜੇ (*p == '"') {
                ctx->ਵਿੱਚ_ਕੁੰਜੀ = 1;
                ctx->escaped = 0;
                ਸ਼ੁਰੂ = p + 1;
                state = ngx_json_string;
                ਰੋਕੋ;
            }

            ਵਾਪਸ NGX_DECLINED;

        ਹਾਲਤ ngx_json_object_name_separator:
            ਜੇ (ngx_json_ws(*p)) {
                ਰੋਕੋ;
            }

            ਜੇ (*p == ':') {
                state = ngx_json_ਮੁੱਲ;
                ਰੋਕੋ;
            }

            ਵਾਪਸ NGX_DECLINED;

        ਹਾਲਤ ngx_json_object_ਮੁੱਲ_separator:
            ਜੇ (ngx_json_ws(*p)) {
                ਰੋਕੋ;
            }

            ਜੇ (*p == ',') {
                state = ngx_json_object_ਅਗਲਾ;
                ਰੋਕੋ;
            }

            ਜੇ (*p == '}') {
                rc = ngx_json_close(ctx, NGX_JSON_OBJECT_CLOSE, &state, p);
                ਜੇ (rc != NGX_OK) {
                    ਵਾਪਸ rc;
                }

                ਰੋਕੋ;
            }

            ਵਾਪਸ NGX_DECLINED;

        ਹਾਲਤ ngx_json_array_ਮੁੱਲ_separator:
            ਜੇ (ngx_json_ws(*p)) {
                ਰੋਕੋ;
            }

            ਜੇ (*p == ',') {
                state = ngx_json_ਮੁੱਲ;
                ਰੋਕੋ;
            }

            ਜੇ (*p == ']') {
                rc = ngx_json_close(ctx, NGX_JSON_ARRAY_CLOSE, &state, p);
                ਜੇ (rc != NGX_OK) {
                    ਵਾਪਸ rc;
                }

                ਰੋਕੋ;
            }

            ਵਾਪਸ NGX_DECLINED;

        ਹਾਲਤ ngx_json_array:
            ਜੇ (ngx_json_ws(*p)) {
                ਰੋਕੋ;
            }

            ਜੇ (*p == ']') {
                rc = ngx_json_close(ctx, NGX_JSON_ARRAY_CLOSE, &state, p);
                ਜੇ (rc != NGX_OK) {
                    ਵਾਪਸ rc;
                }

                ਰੋਕੋ;
            }

            /* fall through */

        ਹਾਲਤ ngx_json_ਮੁੱਲ:
            ਜੇ (ngx_json_ws(*p)) {
                ਰੋਕੋ;
            }

            ਸ਼ੁਰੂ = p;

            ਜੇ (*p == '"') {
                ctx->ਵਿੱਚ_ਕੁੰਜੀ = 0;
                ctx->escaped = 0;
                ਸ਼ੁਰੂ = p + 1;
                state = ngx_json_string;
                ਰੋਕੋ;
            }

            ਜੇ (*p == '{') {
                rc = ngx_json_emit(ctx, NGX_JSON_OBJECT_OPEN, p, 1);
                ਜੇ (rc != NGX_OK) {
                    ਵਾਪਸ rc;
                }

                rc = ngx_json_ਧੱਕੋ_state(ctx, ngx_json_ctx_object);
                ਜੇ (rc != NGX_OK) {
                    ਵਾਪਸ rc;
                }

                state = ngx_json_object;
                ਰੋਕੋ;
            }

            ਜੇ (*p == '[') {
                rc = ngx_json_emit(ctx, NGX_JSON_ARRAY_OPEN, p, 1);
                ਜੇ (rc != NGX_OK) {
                    ਵਾਪਸ rc;
                }

                rc = ngx_json_ਧੱਕੋ_state(ctx, ngx_json_ctx_array);
                ਜੇ (rc != NGX_OK) {
                    ਵਾਪਸ rc;
                }

                state = ngx_json_array;
                ਰੋਕੋ;
            }

            ਜੇ (*p == '-') {
                state = ngx_json_ਸੰਖਿਆ_minus;
                ਰੋਕੋ;
            }

            ਜੇ (*p == '0') {
                state = ngx_json_ਸੰਖਿਆ_int_zero;
                ਰੋਕੋ;
            }

            ਜੇ (*p >= '1' && *p <= '9') {
                state = ngx_json_ਸੰਖਿਆ_int;
                ਰੋਕੋ;
            }

            ਜੇ (*p == 't') {
                state = ngx_json_true_t;
                ਰੋਕੋ;
            }

            ਜੇ (*p == 'f') {
                state = ngx_json_false_f;
                ਰੋਕੋ;
            }

            ਜੇ (*p == 'n') {
                state = ngx_json_null_n;
                ਰੋਕੋ;
            }

            ਵਾਪਸ NGX_DECLINED;

        ਹਾਲਤ ngx_json_string:
            ਜੇ (*p == '"') {

                ਜੇ (ctx->ਵਿੱਚ_ਕੁੰਜੀ) {
                    event = ctx->escaped ? NGX_JSON_ਕੁੰਜੀ_ESCAPED
                                         : NGX_JSON_ਕੁੰਜੀ;

                    rc = ngx_json_emit(ctx, event, ਸ਼ੁਰੂ, p - ਸ਼ੁਰੂ);
                    ਜੇ (rc != NGX_OK) {
                        ਵਾਪਸ rc;
                    }

                    ctx->ਵਿੱਚ_ਕੁੰਜੀ = 0;
                    state = ngx_json_object_name_separator;

                } ਨਹੀਂਤਾਂ {
                    event = ctx->escaped ? NGX_JSON_ਮੁੱਲ_STRING_ESCAPED
                                         : NGX_JSON_ਮੁੱਲ_STRING;

                    rc = ngx_json_emit(ctx, event, ਸ਼ੁਰੂ, p - ਸ਼ੁਰੂ);
                    ਜੇ (rc != NGX_OK) {
                        ਵਾਪਸ rc;
                    }

                    state = ngx_json_after_ਮੁੱਲ(ctx);
                }

                ਰੋਕੋ;
            }

            ਜੇ (*p == '\\') {
                ctx->escaped = 1;
                state = ngx_json_escaped;
                ਰੋਕੋ;
            }

            ਜੇ (*p < ' ') {
                ਵਾਪਸ NGX_DECLINED;
            }

            ਰੋਕੋ;

        ਹਾਲਤ ngx_json_escaped:
            ਜੇ (*p == '"' || *p == '\\' || *p == '/' || *p == 'b'
                || *p == 'f' || *p == 'n' || *p == 'r' || *p == 't')
            {
                state = ngx_json_string;
                ਰੋਕੋ;
            }

            ਜੇ (*p == 'u') {
                ctx->codepoint = 0;
                ctx->hex_ਖੱਬਾ = 4;
                state = ngx_json_escaped_hex;
                ਰੋਕੋ;
            }

            ਵਾਪਸ NGX_DECLINED;

        ਹਾਲਤ ngx_json_escaped_hex:

            /* accumulate the four hex digits of the first \uXXXX escape */

            hd = ngx_json_hex_digit(*p);
            ਜੇ (hd == NGX_ERROR) {
                ਵਾਪਸ NGX_DECLINED;
            }

            ctx->codepoint = (ctx->codepoint << 4) | (uint32_t) hd;

            ਜੇ (--ctx->hex_ਖੱਬਾ > 0) {
                ਰੋਕੋ;
            }

            ਜੇ (ctx->codepoint >= 0xD800 && ctx->codepoint <= 0xDBFF) {
                /* high surrogate - must be followed by \uDC00..\uDFFF */
                state = ngx_json_surrogate_ਸ਼ੁਰੂ;
                ਰੋਕੋ;
            }

            ਜੇ (ctx->codepoint >= 0xDC00 && ctx->codepoint <= 0xDFFF) {
                /* lone low surrogate */
                ਵਾਪਸ NGX_DECLINED;
            }

            state = ngx_json_string;
            ਰੋਕੋ;

        ਹਾਲਤ ngx_json_surrogate_ਸ਼ੁਰੂ:
            ਜੇ (*p == '\\') {
                state = ngx_json_surrogate_u;
                ਰੋਕੋ;
            }

            /* high surrogate not followed by \uXXXX */
            ਵਾਪਸ NGX_DECLINED;

        ਹਾਲਤ ngx_json_surrogate_u:
            ਜੇ (*p == 'u') {
                ctx->codepoint = 0;
                ctx->hex_ਖੱਬਾ = 4;
                state = ngx_json_surrogate_hex;
                ਰੋਕੋ;
            }

            /* high surrogate followed by non-\u escape */
            ਵਾਪਸ NGX_DECLINED;

        ਹਾਲਤ ngx_json_surrogate_hex:

            /* accumulate the four hex digits of the low \uXXXX escape */

            hd = ngx_json_hex_digit(*p);
            ਜੇ (hd == NGX_ERROR) {
                ਵਾਪਸ NGX_DECLINED;
            }

            ctx->codepoint = (ctx->codepoint << 4) | (uint32_t) hd;

            ਜੇ (--ctx->hex_ਖੱਬਾ > 0) {
                ਰੋਕੋ;
            }

            ਜੇ (ctx->codepoint >= 0xDC00 && ctx->codepoint <= 0xDFFF) {
                /* valid surrogate pair */
                state = ngx_json_string;
                ਰੋਕੋ;
            }

            /* high surrogate not followed by low surrogate */
            ਵਾਪਸ NGX_DECLINED;

        ਹਾਲਤ ngx_json_ਸੰਖਿਆ_minus:
            ਜੇ (*p >= '1' && *p <= '9') {
                state = ngx_json_ਸੰਖਿਆ_int;
                ਰੋਕੋ;
            }

            ਜੇ (*p == '0') {
                state = ngx_json_ਸੰਖਿਆ_int_zero;
                ਰੋਕੋ;
            }

            ਵਾਪਸ NGX_DECLINED;

        ਹਾਲਤ ngx_json_ਸੰਖਿਆ_int_zero:
            ਜੇ (*p == '.') {
                state = ngx_json_ਸੰਖਿਆ_frac;
                ਰੋਕੋ;
            }

            ਜੇ (*p == 'e' || *p == 'E') {
                /* No sense, but permitted by RFC */
                state = ngx_json_ਸੰਖਿਆ_exp;
                ਰੋਕੋ;
            }

            p--;

            rc = ngx_json_ਸਿਰਾ_ਸੰਖਿਆ(ctx, ਸ਼ੁਰੂ, p + 1, &state);
            ਜੇ (rc != NGX_OK) {
                ਵਾਪਸ rc;
            }

            ਰੋਕੋ;

        ਹਾਲਤ ngx_json_ਸੰਖਿਆ_int:
            ਜੇ (*p >= '0' && *p <= '9') {
                ਰੋਕੋ;
            }

            ਜੇ (*p == '.') {
                state = ngx_json_ਸੰਖਿਆ_frac;
                ਰੋਕੋ;
            }

            ਜੇ (*p == 'e' || *p == 'E') {
                /* No sense, but permitted by RFC */
                state = ngx_json_ਸੰਖਿਆ_exp;
                ਰੋਕੋ;
            }

            p--;

            rc = ngx_json_ਸਿਰਾ_ਸੰਖਿਆ(ctx, ਸ਼ੁਰੂ, p + 1, &state);
            ਜੇ (rc!= NGX_OK) {
                ਵਾਪਸ rc;
            }

            ਰੋਕੋ;

        ਹਾਲਤ ngx_json_ਸੰਖਿਆ_frac:
            ਜੇ (*p >= '0' && *p <= '9') {
                state = ngx_json_ਸੰਖਿਆ_frac_digit;
                ਰੋਕੋ;
            }

            ਵਾਪਸ NGX_DECLINED;

        ਹਾਲਤ ngx_json_ਸੰਖਿਆ_frac_digit:
            ਜੇ (*p >= '0' && *p <= '9') {
                ਰੋਕੋ;
            }

            ਜੇ (*p == 'e' || *p == 'E') {
                state = ngx_json_ਸੰਖਿਆ_exp;
                ਰੋਕੋ;
            }

            p--;

            rc = ngx_json_ਸਿਰਾ_ਸੰਖਿਆ(ctx, ਸ਼ੁਰੂ, p + 1, &state);
            ਜੇ (rc != NGX_OK) {
                ਵਾਪਸ rc;
            }

            ਰੋਕੋ;

        ਹਾਲਤ ngx_json_ਸੰਖਿਆ_exp:
            ਜੇ (*p == '-' || *p == '+') {
                state = ngx_json_ਸੰਖਿਆ_exp_plusminus;
                ਰੋਕੋ;
            }

            ਜੇ (*p >= '0' && *p <= '9') {
                state = ngx_json_ਸੰਖਿਆ_exp_digit;
                ਰੋਕੋ;
            }

            ਵਾਪਸ NGX_DECLINED;

        ਹਾਲਤ ngx_json_ਸੰਖਿਆ_exp_plusminus:
            ਜੇ (*p >= '0' && *p <= '9') {
                state = ngx_json_ਸੰਖਿਆ_exp_digit;
                ਰੋਕੋ;
            }

            ਵਾਪਸ NGX_DECLINED;

        ਹਾਲਤ ngx_json_ਸੰਖਿਆ_exp_digit:
            ਜੇ (*p >= '0' && *p <= '9') {
                ਰੋਕੋ;
            }

            p--;

            rc = ngx_json_ਸਿਰਾ_ਸੰਖਿਆ(ctx, ਸ਼ੁਰੂ, p + 1, &state);
            ਜੇ (rc != NGX_OK) {
                ਵਾਪਸ rc;
            }

            ਰੋਕੋ;

        ਹਾਲਤ ngx_json_true_t:
            ਜੇ (*p == 'r') {
                state = ngx_json_true_tr;
                ਰੋਕੋ;
            }

            ਵਾਪਸ NGX_DECLINED;

        ਹਾਲਤ ngx_json_true_tr:
            ਜੇ (*p == 'u') {
                state = ngx_json_true_tru;
                ਰੋਕੋ;
            }

            ਵਾਪਸ NGX_DECLINED;

        ਹਾਲਤ ngx_json_true_tru:
            ਜੇ (*p == 'e') {
                rc = ngx_json_emit(ctx, NGX_JSON_ਮੁੱਲ_BOOL, ਸ਼ੁਰੂ,
                                  p + 1 - ਸ਼ੁਰੂ);
                ਜੇ (rc != NGX_OK) {
                    ਵਾਪਸ rc;
                }

                state = ngx_json_after_ਮੁੱਲ(ctx);
                ਰੋਕੋ;
            }

            ਵਾਪਸ NGX_DECLINED;

        ਹਾਲਤ ngx_json_false_f:
            ਜੇ (*p == 'a') {
                state = ngx_json_false_fa;
                ਰੋਕੋ;
            }

            ਵਾਪਸ NGX_DECLINED;

        ਹਾਲਤ ngx_json_false_fa:
            ਜੇ (*p == 'l') {
                state = ngx_json_false_fal;
                ਰੋਕੋ;
            }

            ਵਾਪਸ NGX_DECLINED;

        ਹਾਲਤ ngx_json_false_fal:
            ਜੇ (*p == 's') {
                state = ngx_json_false_fals;
                ਰੋਕੋ;
            }

            ਵਾਪਸ NGX_DECLINED;

        ਹਾਲਤ ngx_json_false_fals:
            ਜੇ (*p == 'e') {
                rc = ngx_json_emit(ctx, NGX_JSON_ਮੁੱਲ_BOOL, ਸ਼ੁਰੂ,
                                  p + 1 - ਸ਼ੁਰੂ);
                ਜੇ (rc != NGX_OK) {
                    ਵਾਪਸ rc;
                }

                state = ngx_json_after_ਮੁੱਲ(ctx);
                ਰੋਕੋ;
            }

            ਵਾਪਸ NGX_DECLINED;

        ਹਾਲਤ ngx_json_null_n:
            ਜੇ (*p == 'u') {
                state = ngx_json_null_nu;
                ਰੋਕੋ;
            }

            ਵਾਪਸ NGX_DECLINED;

        ਹਾਲਤ ngx_json_null_nu:
            ਜੇ (*p == 'l') {
                state = ngx_json_null_nul;
                ਰੋਕੋ;
            }

            ਵਾਪਸ NGX_DECLINED;

        ਹਾਲਤ ngx_json_null_nul:
            ਜੇ (*p == 'l') {
                rc = ngx_json_emit(ctx, NGX_JSON_ਮੁੱਲ_NULL, ਸ਼ੁਰੂ,
                                  p + 1 - ਸ਼ੁਰੂ);
                ਜੇ (rc != NGX_OK) {
                    ਵਾਪਸ rc;
                }

                state = ngx_json_after_ਮੁੱਲ(ctx);
                ਰੋਕੋ;
            }

            ਵਾਪਸ NGX_DECLINED;

        ਹਾਲਤ ngx_json_ਪੂਰਾ:
            ਜੇ (ngx_json_ws(*p)) {
                ਰੋਕੋ;
            }

            ਵਾਪਸ NGX_DECLINED;
        }

        skip_ਡੂੰਘਾਈ = ctx->skip_until_ਡੂੰਘਾਈ;
    }

    /* a bare top-level number is not self-delimited; emit it at end of input */

    ਜੇ (ctx->ਡੂੰਘਾਈ == 0
        && (state == ngx_json_ਸੰਖਿਆ_int_zero
            || state == ngx_json_ਸੰਖਿਆ_int
            || state == ngx_json_ਸੰਖਿਆ_frac_digit
            || state == ngx_json_ਸੰਖਿਆ_exp_digit))
    {
        rc = ngx_json_emit(ctx, NGX_JSON_ਮੁੱਲ_ਸੰਖਿਆ, ਸ਼ੁਰੂ, ਆਖਰੀ - ਸ਼ੁਰੂ);
        ਜੇ (rc != NGX_OK) {
            ਵਾਪਸ rc;
        }

        state = ngx_json_ਪੂਰਾ;
    }

    ctx->state = state;

    ਜੇ (state == ngx_json_ਪੂਰਾ) {
        ਵਾਪਸ NGX_OK;
    }

    ਵਾਪਸ NGX_DECLINED;
}


ngx_int_t
ngx_json_parse(ngx_pool_t *pool, ngx_str_t *json, ngx_json_handler_pt handler,
    ਖਾਲੀ *ਅੰਕੜੇ)
{
    ngx_json_ctx_t  jctx;

    ngx_json_ctx_init(&jctx, pool);

    jctx.ਅੰਕੜੇ = ਅੰਕੜੇ;
    jctx.handler = handler;

    ਵਾਪਸ ngx_json_parse_ctx(&jctx, json->ਅੰਕੜੇ, json->len);
}


ਸਥਿਰ ngx_inline ngx_int_t
ngx_json_ਸਿਰਾ_ਸੰਖਿਆ(ngx_json_ctx_t *ctx, u_char *ਸ਼ੁਰੂ, u_char *ਸਿਰਾ,
    ngx_json_state_e *state)
{
    ngx_int_t  rc;

    rc = ngx_json_emit(ctx, NGX_JSON_ਮੁੱਲ_ਸੰਖਿਆ, ਸ਼ੁਰੂ, ਸਿਰਾ - ਸ਼ੁਰੂ);
    ਜੇ (rc != NGX_OK) {
        ਵਾਪਸ rc;
    }

    *state = ngx_json_after_ਮੁੱਲ(ctx);

    ਵਾਪਸ NGX_OK;
}


ਸਥਿਰ ngx_inline ngx_int_t
ngx_json_emit(ngx_json_ctx_t *ctx, ngx_json_event_e event, u_char *ਅੰਕੜੇ,
    ਮਾਪ_t len)
{
    ngx_int_t  rc;
    ngx_str_t  token;

    ਜੇ (ngx_json_skipping(ctx->skip_until_ਡੂੰਘਾਈ)) {
        ਵਾਪਸ NGX_OK;
    }

    ਜੇ (ctx->handler == ਨਿਰਾ) {
        ਵਾਪਸ NGX_OK;
    }

    token.ਅੰਕੜੇ = ਅੰਕੜੇ;
    token.len = len;
    rc = ctx->handler(ctx, event, &token);

    ਜੇ (rc == NGX_JSON_SKIP) {

        /*
         * NGX_JSON_SKIP prunes the rest of the current container.  It is
         * honoured only where it is meaningful:
         *
         *   - OBJECT_OPEN / ARRAY_OPEN: skip the whole container;
         *   - a scalar VALUE_* that is an array element: skip the rest of
         *     that array.
         *
         * A skip requested on a CLOSE, or on an object member value, is
         * ignored: object members are navigated by KEY (use a KEY skip),
         * and a CLOSE fires after the container is already complete.
         */

        ਚੋਣ (event) {

        ਹਾਲਤ NGX_JSON_ਕੁੰਜੀ:
        ਹਾਲਤ NGX_JSON_ਕੁੰਜੀ_ESCAPED:
        ਹਾਲਤ NGX_JSON_OBJECT_OPEN:
        ਹਾਲਤ NGX_JSON_ARRAY_OPEN:
            ctx->skip_until_ਡੂੰਘਾਈ = ctx->ਡੂੰਘਾਈ;
            ਰੋਕੋ;

        ਹਾਲਤ NGX_JSON_ਮੁੱਲ_STRING:
        ਹਾਲਤ NGX_JSON_ਮੁੱਲ_STRING_ESCAPED:
        ਹਾਲਤ NGX_JSON_ਮੁੱਲ_ਸੰਖਿਆ:
        ਹਾਲਤ NGX_JSON_ਮੁੱਲ_BOOL:
        ਹਾਲਤ NGX_JSON_ਮੁੱਲ_NULL:
            ਜੇ (ctx->ਡੂੰਘਾਈ > 0
                && ctx->ਢੇਰ[ctx->ਡੂੰਘਾਈ - 1] == ngx_json_ctx_array
            ) {
                ctx->skip_until_ਡੂੰਘਾਈ = ctx->ਡੂੰਘਾਈ - 1;
            }
            ਰੋਕੋ;

        ਮੂਲ:
            ਰੋਕੋ;
        }

        ਵਾਪਸ NGX_OK;
    }

    ਵਾਪਸ rc;
}


ਸਥਿਰ ngx_inline ngx_json_state_e
ngx_json_after_ਮੁੱਲ(ngx_json_ctx_t *ctx)
{
    ਜੇ (ctx->ਡੂੰਘਾਈ == 0) {
        ਵਾਪਸ ngx_json_ਪੂਰਾ;
    }

    ਜੇ (ctx->ਢੇਰ[ctx->ਡੂੰਘਾਈ - 1] == ngx_json_ctx_object) {
        ਵਾਪਸ ngx_json_object_ਮੁੱਲ_separator;
    }

    ਵਾਪਸ ngx_json_array_ਮੁੱਲ_separator;
}


ਸਥਿਰ ngx_inline ngx_int_t
ngx_json_close(ngx_json_ctx_t *ctx, ngx_json_event_e event,
    ngx_json_state_e *state, u_char *p)
{
    ngx_int_t  rc;

    ctx->ਡੂੰਘਾਈ--;

    rc = ngx_json_emit(ctx, event, p, 1);
    ਜੇ (rc != NGX_OK) {
        ਵਾਪਸ rc;
    }

    *state = ngx_json_after_ਮੁੱਲ(ctx);

    ਵਾਪਸ NGX_OK;
}


ਸਥਿਰ ngx_int_t
ngx_json_ਧੱਕੋ_state(ngx_json_ctx_t *ctx, ngx_json_container_e container)
{
    ngx_json_container_e  *ਢੇਰ;

    ਜੇ (ctx->ਡੂੰਘਾਈ >= ctx->max_ਡੂੰਘਾਈ) {
        ਵਾਪਸ NGX_DECLINED;
    }

    ਜੇ (ctx->ਢੇਰ == ਨਿਰਾ) {
        ਢੇਰ = ngx_palloc(ctx->pool,
                           ctx->max_ਡੂੰਘਾਈ * ਆਕਾਰ(ngx_json_container_e));
        ਜੇ (ਢੇਰ == ਨਿਰਾ) {
            ਵਾਪਸ NGX_ERROR;
        }

        ctx->ਢੇਰ = ਢੇਰ;
    }

    ctx->ਢੇਰ[ctx->ਡੂੰਘਾਈ] = container;
    ctx->ਡੂੰਘਾਈ++;

    ਵਾਪਸ NGX_OK;
}
