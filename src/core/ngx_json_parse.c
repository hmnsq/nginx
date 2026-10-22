
/*
 * Copyright (C) Maxim Dounin
 * Copyright (C) Nginx, Inc.
 */


/*
 * JSON parser. RFC 8259 for the JSON grammar.

 * Site http://json.org/ may be useful too.
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>
#ಅನ್ತರ್ಭಾವ <ngx_json_parse.h>


#ವ್ಯಾಖ್ಯಾನಿಸು NGX_JSON_NOT_SKIPPING  (ngx_uint_t) -1
#ವ್ಯಾಖ್ಯಾನಿಸು ngx_json_skipping(d)   ((d) != NGX_JSON_NOT_SKIPPING)
#ವ್ಯಾಖ್ಯಾನಿಸು ngx_json_ws(c)  ((c) == ' ' || (c) == '\t' || (c) == CR || (c) == LF)


ಸ್ಥಿರ ngx_inline ngx_int_t ngx_json_ಕೊನೆ_ಸಂಖ್ಯೆ(ngx_json_ctx_t *ctx,
    u_char *ಆರಂಭ, u_char *ಕೊನೆ, ngx_json_state_e *state);
ಸ್ಥಿರ ngx_inline ngx_int_t ngx_json_emit(ngx_json_ctx_t *ctx,
    ngx_json_event_e event, u_char *ದತ್ತಾಂಶ, ಗಾತ್ರ_t len);
ಸ್ಥಿರ ngx_inline ngx_json_state_e ngx_json_after_ಮೌಲ್ಯ(ngx_json_ctx_t *ctx);
ಸ್ಥಿರ ngx_inline ngx_int_t ngx_json_close(ngx_json_ctx_t *ctx,
    ngx_json_event_e event, ngx_json_state_e *state, u_char *p);
ಸ್ಥಿರ ngx_int_t ngx_json_ತಳ್ಳು_state(ngx_json_ctx_t *ctx,
    ngx_json_container_e container);


ಶೂನ್ಯ
ngx_json_ctx_init(ngx_json_ctx_t *ctx, ngx_pool_t *pool)
{
    ctx->state = ngx_json_ಆರಂಭ;
    ctx->pool = pool;
    ctx->handler = ಖಾಲಿ;
    ctx->ದತ್ತಾಂಶ = ಖಾಲಿ;
    ctx->codepoint = 0;
    ctx->hex_ಎಡ = 0;

    ctx->ಆಳ = 0;
    ctx->skip_until_ಆಳ = NGX_JSON_NOT_SKIPPING;
    ctx->max_ಆಳ = NGX_JSON_DEFAULT_MAX_ಆಳ;

    /* stack is allocated lazily on first push, sized to max_depth */

    ctx->ರಾಶಿ = ಖಾಲಿ;

    ctx->ಒಳ_ಕೀಲಿ = 0;
    ctx->escaped = 0;
}


ngx_int_t
ngx_json_parse_ctx(ngx_json_ctx_t *ctx, u_char *ದತ್ತಾಂಶ, ಗಾತ್ರ_t len)
{
    u_char            *p, *ಕೊನೆಯ, *ಆರಂಭ;
    ngx_int_t          rc, hd;
    ngx_uint_t         skip_ಆಳ;
    ngx_json_event_e   event;
    ngx_json_state_e   state;

    ಆದರೆ (len == 0) {
        ಹಿಂತಿರುಗಿ NGX_DECLINED;
    }

    ಆದರೆ (ದತ್ತಾಂಶ == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    ಆದರೆ (ctx->state == ngx_json_ಮುಗಿದ) {
        ಹಿಂತಿರುಗಿ NGX_DECLINED;
    }

    ಕೊನೆಯ = ದತ್ತಾಂಶ + len;
    state = ctx->state;
    skip_ಆಳ = ctx->skip_until_ಆಳ;
    ಆರಂಭ = ಖಾಲಿ;

    ಗಾಗಿ (p = ದತ್ತಾಂಶ; p < ಕೊನೆಯ; p++) {

        /* skipping suppresses emission only; the machine still validates */

        ಆದರೆ (ngx_json_skipping(skip_ಆಳ) && ctx->ಆಳ <= skip_ಆಳ
            && (state == ngx_json_object_ಮೌಲ್ಯ_separator
                || state == ngx_json_array_ಮೌಲ್ಯ_separator
                || state == ngx_json_ಮುಗಿದ))
        {
            skip_ಆಳ = NGX_JSON_NOT_SKIPPING;
            ctx->skip_until_ಆಳ = NGX_JSON_NOT_SKIPPING;
            p--;

            ಮುಂದುವರಿ;
        }

        ಆಯ್ಕೆ (state) {

        ಸ್ಥಿತಿ ngx_json_ಆರಂಭ:
            ಆದರೆ (ngx_json_ws(*p)) {
                ನಿಲ್ಲಿಸು;
            }

            p--;
            state = ngx_json_ಮೌಲ್ಯ;
            ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ ngx_json_object:
            ಆದರೆ (ngx_json_ws(*p)) {
                ನಿಲ್ಲಿಸು;
            }

            ಆದರೆ (*p == '}') {
                rc = ngx_json_close(ctx, NGX_JSON_OBJECT_CLOSE, &state, p);
                ಆದರೆ (rc != NGX_OK) {
                    ಹಿಂತಿರುಗಿ rc;
                }

                ನಿಲ್ಲಿಸು;
            }

            /* fall through */

        ಸ್ಥಿತಿ ngx_json_object_ಮುಂದಿನ:
            ಆದರೆ (ngx_json_ws(*p)) {
                ನಿಲ್ಲಿಸು;
            }

            ಆದರೆ (*p == '"') {
                ctx->ಒಳ_ಕೀಲಿ = 1;
                ctx->escaped = 0;
                ಆರಂಭ = p + 1;
                state = ngx_json_string;
                ನಿಲ್ಲಿಸು;
            }

            ಹಿಂತಿರುಗಿ NGX_DECLINED;

        ಸ್ಥಿತಿ ngx_json_object_name_separator:
            ಆದರೆ (ngx_json_ws(*p)) {
                ನಿಲ್ಲಿಸು;
            }

            ಆದರೆ (*p == ':') {
                state = ngx_json_ಮೌಲ್ಯ;
                ನಿಲ್ಲಿಸು;
            }

            ಹಿಂತಿರುಗಿ NGX_DECLINED;

        ಸ್ಥಿತಿ ngx_json_object_ಮೌಲ್ಯ_separator:
            ಆದರೆ (ngx_json_ws(*p)) {
                ನಿಲ್ಲಿಸು;
            }

            ಆದರೆ (*p == ',') {
                state = ngx_json_object_ಮುಂದಿನ;
                ನಿಲ್ಲಿಸು;
            }

            ಆದರೆ (*p == '}') {
                rc = ngx_json_close(ctx, NGX_JSON_OBJECT_CLOSE, &state, p);
                ಆದರೆ (rc != NGX_OK) {
                    ಹಿಂತಿರುಗಿ rc;
                }

                ನಿಲ್ಲಿಸು;
            }

            ಹಿಂತಿರುಗಿ NGX_DECLINED;

        ಸ್ಥಿತಿ ngx_json_array_ಮೌಲ್ಯ_separator:
            ಆದರೆ (ngx_json_ws(*p)) {
                ನಿಲ್ಲಿಸು;
            }

            ಆದರೆ (*p == ',') {
                state = ngx_json_ಮೌಲ್ಯ;
                ನಿಲ್ಲಿಸು;
            }

            ಆದರೆ (*p == ']') {
                rc = ngx_json_close(ctx, NGX_JSON_ARRAY_CLOSE, &state, p);
                ಆದರೆ (rc != NGX_OK) {
                    ಹಿಂತಿರುಗಿ rc;
                }

                ನಿಲ್ಲಿಸು;
            }

            ಹಿಂತಿರುಗಿ NGX_DECLINED;

        ಸ್ಥಿತಿ ngx_json_array:
            ಆದರೆ (ngx_json_ws(*p)) {
                ನಿಲ್ಲಿಸು;
            }

            ಆದರೆ (*p == ']') {
                rc = ngx_json_close(ctx, NGX_JSON_ARRAY_CLOSE, &state, p);
                ಆದರೆ (rc != NGX_OK) {
                    ಹಿಂತಿರುಗಿ rc;
                }

                ನಿಲ್ಲಿಸು;
            }

            /* fall through */

        ಸ್ಥಿತಿ ngx_json_ಮೌಲ್ಯ:
            ಆದರೆ (ngx_json_ws(*p)) {
                ನಿಲ್ಲಿಸು;
            }

            ಆರಂಭ = p;

            ಆದರೆ (*p == '"') {
                ctx->ಒಳ_ಕೀಲಿ = 0;
                ctx->escaped = 0;
                ಆರಂಭ = p + 1;
                state = ngx_json_string;
                ನಿಲ್ಲಿಸು;
            }

            ಆದರೆ (*p == '{') {
                rc = ngx_json_emit(ctx, NGX_JSON_OBJECT_OPEN, p, 1);
                ಆದರೆ (rc != NGX_OK) {
                    ಹಿಂತಿರುಗಿ rc;
                }

                rc = ngx_json_ತಳ್ಳು_state(ctx, ngx_json_ctx_object);
                ಆದರೆ (rc != NGX_OK) {
                    ಹಿಂತಿರುಗಿ rc;
                }

                state = ngx_json_object;
                ನಿಲ್ಲಿಸು;
            }

            ಆದರೆ (*p == '[') {
                rc = ngx_json_emit(ctx, NGX_JSON_ARRAY_OPEN, p, 1);
                ಆದರೆ (rc != NGX_OK) {
                    ಹಿಂತಿರುಗಿ rc;
                }

                rc = ngx_json_ತಳ್ಳು_state(ctx, ngx_json_ctx_array);
                ಆದರೆ (rc != NGX_OK) {
                    ಹಿಂತಿರುಗಿ rc;
                }

                state = ngx_json_array;
                ನಿಲ್ಲಿಸು;
            }

            ಆದರೆ (*p == '-') {
                state = ngx_json_ಸಂಖ್ಯೆ_minus;
                ನಿಲ್ಲಿಸು;
            }

            ಆದರೆ (*p == '0') {
                state = ngx_json_ಸಂಖ್ಯೆ_int_zero;
                ನಿಲ್ಲಿಸು;
            }

            ಆದರೆ (*p >= '1' && *p <= '9') {
                state = ngx_json_ಸಂಖ್ಯೆ_int;
                ನಿಲ್ಲಿಸು;
            }

            ಆದರೆ (*p == 't') {
                state = ngx_json_true_t;
                ನಿಲ್ಲಿಸು;
            }

            ಆದರೆ (*p == 'f') {
                state = ngx_json_false_f;
                ನಿಲ್ಲಿಸು;
            }

            ಆದರೆ (*p == 'n') {
                state = ngx_json_null_n;
                ನಿಲ್ಲಿಸು;
            }

            ಹಿಂತಿರುಗಿ NGX_DECLINED;

        ಸ್ಥಿತಿ ngx_json_string:
            ಆದರೆ (*p == '"') {

                ಆದರೆ (ctx->ಒಳ_ಕೀಲಿ) {
                    event = ctx->escaped ? NGX_JSON_ಕೀಲಿ_ESCAPED
                                         : NGX_JSON_ಕೀಲಿ;

                    rc = ngx_json_emit(ctx, event, ಆರಂಭ, p - ಆರಂಭ);
                    ಆದರೆ (rc != NGX_OK) {
                        ಹಿಂತಿರುಗಿ rc;
                    }

                    ctx->ಒಳ_ಕೀಲಿ = 0;
                    state = ngx_json_object_name_separator;

                } ಇಲ್ಲದಿದ್ದರೆ {
                    event = ctx->escaped ? NGX_JSON_ಮೌಲ್ಯ_STRING_ESCAPED
                                         : NGX_JSON_ಮೌಲ್ಯ_STRING;

                    rc = ngx_json_emit(ctx, event, ಆರಂಭ, p - ಆರಂಭ);
                    ಆದರೆ (rc != NGX_OK) {
                        ಹಿಂತಿರುಗಿ rc;
                    }

                    state = ngx_json_after_ಮೌಲ್ಯ(ctx);
                }

                ನಿಲ್ಲಿಸು;
            }

            ಆದರೆ (*p == '\\') {
                ctx->escaped = 1;
                state = ngx_json_escaped;
                ನಿಲ್ಲಿಸು;
            }

            ಆದರೆ (*p < ' ') {
                ಹಿಂತಿರುಗಿ NGX_DECLINED;
            }

            ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ ngx_json_escaped:
            ಆದರೆ (*p == '"' || *p == '\\' || *p == '/' || *p == 'b'
                || *p == 'f' || *p == 'n' || *p == 'r' || *p == 't')
            {
                state = ngx_json_string;
                ನಿಲ್ಲಿಸು;
            }

            ಆದರೆ (*p == 'u') {
                ctx->codepoint = 0;
                ctx->hex_ಎಡ = 4;
                state = ngx_json_escaped_hex;
                ನಿಲ್ಲಿಸು;
            }

            ಹಿಂತಿರುಗಿ NGX_DECLINED;

        ಸ್ಥಿತಿ ngx_json_escaped_hex:

            /* accumulate the four hex digits of the first \uXXXX escape */

            hd = ngx_json_hex_digit(*p);
            ಆದರೆ (hd == NGX_ERROR) {
                ಹಿಂತಿರುಗಿ NGX_DECLINED;
            }

            ctx->codepoint = (ctx->codepoint << 4) | (uint32_t) hd;

            ಆದರೆ (--ctx->hex_ಎಡ > 0) {
                ನಿಲ್ಲಿಸು;
            }

            ಆದರೆ (ctx->codepoint >= 0xD800 && ctx->codepoint <= 0xDBFF) {
                /* high surrogate - must be followed by \uDC00..\uDFFF */
                state = ngx_json_surrogate_ಆರಂಭ;
                ನಿಲ್ಲಿಸು;
            }

            ಆದರೆ (ctx->codepoint >= 0xDC00 && ctx->codepoint <= 0xDFFF) {
                /* lone low surrogate */
                ಹಿಂತಿರುಗಿ NGX_DECLINED;
            }

            state = ngx_json_string;
            ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ ngx_json_surrogate_ಆರಂಭ:
            ಆದರೆ (*p == '\\') {
                state = ngx_json_surrogate_u;
                ನಿಲ್ಲಿಸು;
            }

            /* high surrogate not followed by \uXXXX */
            ಹಿಂತಿರುಗಿ NGX_DECLINED;

        ಸ್ಥಿತಿ ngx_json_surrogate_u:
            ಆದರೆ (*p == 'u') {
                ctx->codepoint = 0;
                ctx->hex_ಎಡ = 4;
                state = ngx_json_surrogate_hex;
                ನಿಲ್ಲಿಸು;
            }

            /* high surrogate followed by non-\u escape */
            ಹಿಂತಿರುಗಿ NGX_DECLINED;

        ಸ್ಥಿತಿ ngx_json_surrogate_hex:

            /* accumulate the four hex digits of the low \uXXXX escape */

            hd = ngx_json_hex_digit(*p);
            ಆದರೆ (hd == NGX_ERROR) {
                ಹಿಂತಿರುಗಿ NGX_DECLINED;
            }

            ctx->codepoint = (ctx->codepoint << 4) | (uint32_t) hd;

            ಆದರೆ (--ctx->hex_ಎಡ > 0) {
                ನಿಲ್ಲಿಸು;
            }

            ಆದರೆ (ctx->codepoint >= 0xDC00 && ctx->codepoint <= 0xDFFF) {
                /* valid surrogate pair */
                state = ngx_json_string;
                ನಿಲ್ಲಿಸು;
            }

            /* high surrogate not followed by low surrogate */
            ಹಿಂತಿರುಗಿ NGX_DECLINED;

        ಸ್ಥಿತಿ ngx_json_ಸಂಖ್ಯೆ_minus:
            ಆದರೆ (*p >= '1' && *p <= '9') {
                state = ngx_json_ಸಂಖ್ಯೆ_int;
                ನಿಲ್ಲಿಸು;
            }

            ಆದರೆ (*p == '0') {
                state = ngx_json_ಸಂಖ್ಯೆ_int_zero;
                ನಿಲ್ಲಿಸು;
            }

            ಹಿಂತಿರುಗಿ NGX_DECLINED;

        ಸ್ಥಿತಿ ngx_json_ಸಂಖ್ಯೆ_int_zero:
            ಆದರೆ (*p == '.') {
                state = ngx_json_ಸಂಖ್ಯೆ_frac;
                ನಿಲ್ಲಿಸು;
            }

            ಆದರೆ (*p == 'e' || *p == 'E') {
                /* No sense, but permitted by RFC */
                state = ngx_json_ಸಂಖ್ಯೆ_exp;
                ನಿಲ್ಲಿಸು;
            }

            p--;

            rc = ngx_json_ಕೊನೆ_ಸಂಖ್ಯೆ(ctx, ಆರಂಭ, p + 1, &state);
            ಆದರೆ (rc != NGX_OK) {
                ಹಿಂತಿರುಗಿ rc;
            }

            ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ ngx_json_ಸಂಖ್ಯೆ_int:
            ಆದರೆ (*p >= '0' && *p <= '9') {
                ನಿಲ್ಲಿಸು;
            }

            ಆದರೆ (*p == '.') {
                state = ngx_json_ಸಂಖ್ಯೆ_frac;
                ನಿಲ್ಲಿಸು;
            }

            ಆದರೆ (*p == 'e' || *p == 'E') {
                /* No sense, but permitted by RFC */
                state = ngx_json_ಸಂಖ್ಯೆ_exp;
                ನಿಲ್ಲಿಸು;
            }

            p--;

            rc = ngx_json_ಕೊನೆ_ಸಂಖ್ಯೆ(ctx, ಆರಂಭ, p + 1, &state);
            ಆದರೆ (rc!= NGX_OK) {
                ಹಿಂತಿರುಗಿ rc;
            }

            ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ ngx_json_ಸಂಖ್ಯೆ_frac:
            ಆದರೆ (*p >= '0' && *p <= '9') {
                state = ngx_json_ಸಂಖ್ಯೆ_frac_digit;
                ನಿಲ್ಲಿಸು;
            }

            ಹಿಂತಿರುಗಿ NGX_DECLINED;

        ಸ್ಥಿತಿ ngx_json_ಸಂಖ್ಯೆ_frac_digit:
            ಆದರೆ (*p >= '0' && *p <= '9') {
                ನಿಲ್ಲಿಸು;
            }

            ಆದರೆ (*p == 'e' || *p == 'E') {
                state = ngx_json_ಸಂಖ್ಯೆ_exp;
                ನಿಲ್ಲಿಸು;
            }

            p--;

            rc = ngx_json_ಕೊನೆ_ಸಂಖ್ಯೆ(ctx, ಆರಂಭ, p + 1, &state);
            ಆದರೆ (rc != NGX_OK) {
                ಹಿಂತಿರುಗಿ rc;
            }

            ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ ngx_json_ಸಂಖ್ಯೆ_exp:
            ಆದರೆ (*p == '-' || *p == '+') {
                state = ngx_json_ಸಂಖ್ಯೆ_exp_plusminus;
                ನಿಲ್ಲಿಸು;
            }

            ಆದರೆ (*p >= '0' && *p <= '9') {
                state = ngx_json_ಸಂಖ್ಯೆ_exp_digit;
                ನಿಲ್ಲಿಸು;
            }

            ಹಿಂತಿರುಗಿ NGX_DECLINED;

        ಸ್ಥಿತಿ ngx_json_ಸಂಖ್ಯೆ_exp_plusminus:
            ಆದರೆ (*p >= '0' && *p <= '9') {
                state = ngx_json_ಸಂಖ್ಯೆ_exp_digit;
                ನಿಲ್ಲಿಸು;
            }

            ಹಿಂತಿರುಗಿ NGX_DECLINED;

        ಸ್ಥಿತಿ ngx_json_ಸಂಖ್ಯೆ_exp_digit:
            ಆದರೆ (*p >= '0' && *p <= '9') {
                ನಿಲ್ಲಿಸು;
            }

            p--;

            rc = ngx_json_ಕೊನೆ_ಸಂಖ್ಯೆ(ctx, ಆರಂಭ, p + 1, &state);
            ಆದರೆ (rc != NGX_OK) {
                ಹಿಂತಿರುಗಿ rc;
            }

            ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ ngx_json_true_t:
            ಆದರೆ (*p == 'r') {
                state = ngx_json_true_tr;
                ನಿಲ್ಲಿಸು;
            }

            ಹಿಂತಿರುಗಿ NGX_DECLINED;

        ಸ್ಥಿತಿ ngx_json_true_tr:
            ಆದರೆ (*p == 'u') {
                state = ngx_json_true_tru;
                ನಿಲ್ಲಿಸು;
            }

            ಹಿಂತಿರುಗಿ NGX_DECLINED;

        ಸ್ಥಿತಿ ngx_json_true_tru:
            ಆದರೆ (*p == 'e') {
                rc = ngx_json_emit(ctx, NGX_JSON_ಮೌಲ್ಯ_BOOL, ಆರಂಭ,
                                  p + 1 - ಆರಂಭ);
                ಆದರೆ (rc != NGX_OK) {
                    ಹಿಂತಿರುಗಿ rc;
                }

                state = ngx_json_after_ಮೌಲ್ಯ(ctx);
                ನಿಲ್ಲಿಸು;
            }

            ಹಿಂತಿರುಗಿ NGX_DECLINED;

        ಸ್ಥಿತಿ ngx_json_false_f:
            ಆದರೆ (*p == 'a') {
                state = ngx_json_false_fa;
                ನಿಲ್ಲಿಸು;
            }

            ಹಿಂತಿರುಗಿ NGX_DECLINED;

        ಸ್ಥಿತಿ ngx_json_false_fa:
            ಆದರೆ (*p == 'l') {
                state = ngx_json_false_fal;
                ನಿಲ್ಲಿಸು;
            }

            ಹಿಂತಿರುಗಿ NGX_DECLINED;

        ಸ್ಥಿತಿ ngx_json_false_fal:
            ಆದರೆ (*p == 's') {
                state = ngx_json_false_fals;
                ನಿಲ್ಲಿಸು;
            }

            ಹಿಂತಿರುಗಿ NGX_DECLINED;

        ಸ್ಥಿತಿ ngx_json_false_fals:
            ಆದರೆ (*p == 'e') {
                rc = ngx_json_emit(ctx, NGX_JSON_ಮೌಲ್ಯ_BOOL, ಆರಂಭ,
                                  p + 1 - ಆರಂಭ);
                ಆದರೆ (rc != NGX_OK) {
                    ಹಿಂತಿರುಗಿ rc;
                }

                state = ngx_json_after_ಮೌಲ್ಯ(ctx);
                ನಿಲ್ಲಿಸು;
            }

            ಹಿಂತಿರುಗಿ NGX_DECLINED;

        ಸ್ಥಿತಿ ngx_json_null_n:
            ಆದರೆ (*p == 'u') {
                state = ngx_json_null_nu;
                ನಿಲ್ಲಿಸು;
            }

            ಹಿಂತಿರುಗಿ NGX_DECLINED;

        ಸ್ಥಿತಿ ngx_json_null_nu:
            ಆದರೆ (*p == 'l') {
                state = ngx_json_null_nul;
                ನಿಲ್ಲಿಸು;
            }

            ಹಿಂತಿರುಗಿ NGX_DECLINED;

        ಸ್ಥಿತಿ ngx_json_null_nul:
            ಆದರೆ (*p == 'l') {
                rc = ngx_json_emit(ctx, NGX_JSON_ಮೌಲ್ಯ_NULL, ಆರಂಭ,
                                  p + 1 - ಆರಂಭ);
                ಆದರೆ (rc != NGX_OK) {
                    ಹಿಂತಿರುಗಿ rc;
                }

                state = ngx_json_after_ಮೌಲ್ಯ(ctx);
                ನಿಲ್ಲಿಸು;
            }

            ಹಿಂತಿರುಗಿ NGX_DECLINED;

        ಸ್ಥಿತಿ ngx_json_ಮುಗಿದ:
            ಆದರೆ (ngx_json_ws(*p)) {
                ನಿಲ್ಲಿಸು;
            }

            ಹಿಂತಿರುಗಿ NGX_DECLINED;
        }

        skip_ಆಳ = ctx->skip_until_ಆಳ;
    }

    /* a bare top-level number is not self-delimited; emit it at end of input */

    ಆದರೆ (ctx->ಆಳ == 0
        && (state == ngx_json_ಸಂಖ್ಯೆ_int_zero
            || state == ngx_json_ಸಂಖ್ಯೆ_int
            || state == ngx_json_ಸಂಖ್ಯೆ_frac_digit
            || state == ngx_json_ಸಂಖ್ಯೆ_exp_digit))
    {
        rc = ngx_json_emit(ctx, NGX_JSON_ಮೌಲ್ಯ_ಸಂಖ್ಯೆ, ಆರಂಭ, ಕೊನೆಯ - ಆರಂಭ);
        ಆದರೆ (rc != NGX_OK) {
            ಹಿಂತಿರುಗಿ rc;
        }

        state = ngx_json_ಮುಗಿದ;
    }

    ctx->state = state;

    ಆದರೆ (state == ngx_json_ಮುಗಿದ) {
        ಹಿಂತಿರುಗಿ NGX_OK;
    }

    ಹಿಂತಿರುಗಿ NGX_DECLINED;
}


ngx_int_t
ngx_json_parse(ngx_pool_t *pool, ngx_str_t *json, ngx_json_handler_pt handler,
    ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
    ngx_json_ctx_t  jctx;

    ngx_json_ctx_init(&jctx, pool);

    jctx.ದತ್ತಾಂಶ = ದತ್ತಾಂಶ;
    jctx.handler = handler;

    ಹಿಂತಿರುಗಿ ngx_json_parse_ctx(&jctx, json->ದತ್ತಾಂಶ, json->len);
}


ಸ್ಥಿರ ngx_inline ngx_int_t
ngx_json_ಕೊನೆ_ಸಂಖ್ಯೆ(ngx_json_ctx_t *ctx, u_char *ಆರಂಭ, u_char *ಕೊನೆ,
    ngx_json_state_e *state)
{
    ngx_int_t  rc;

    rc = ngx_json_emit(ctx, NGX_JSON_ಮೌಲ್ಯ_ಸಂಖ್ಯೆ, ಆರಂಭ, ಕೊನೆ - ಆರಂಭ);
    ಆದರೆ (rc != NGX_OK) {
        ಹಿಂತಿರುಗಿ rc;
    }

    *state = ngx_json_after_ಮೌಲ್ಯ(ctx);

    ಹಿಂತಿರುಗಿ NGX_OK;
}


ಸ್ಥಿರ ngx_inline ngx_int_t
ngx_json_emit(ngx_json_ctx_t *ctx, ngx_json_event_e event, u_char *ದತ್ತಾಂಶ,
    ಗಾತ್ರ_t len)
{
    ngx_int_t  rc;
    ngx_str_t  token;

    ಆದರೆ (ngx_json_skipping(ctx->skip_until_ಆಳ)) {
        ಹಿಂತಿರುಗಿ NGX_OK;
    }

    ಆದರೆ (ctx->handler == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ NGX_OK;
    }

    token.ದತ್ತಾಂಶ = ದತ್ತಾಂಶ;
    token.len = len;
    rc = ctx->handler(ctx, event, &token);

    ಆದರೆ (rc == NGX_JSON_SKIP) {

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

        ಆಯ್ಕೆ (event) {

        ಸ್ಥಿತಿ NGX_JSON_ಕೀಲಿ:
        ಸ್ಥಿತಿ NGX_JSON_ಕೀಲಿ_ESCAPED:
        ಸ್ಥಿತಿ NGX_JSON_OBJECT_OPEN:
        ಸ್ಥಿತಿ NGX_JSON_ARRAY_OPEN:
            ctx->skip_until_ಆಳ = ctx->ಆಳ;
            ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ NGX_JSON_ಮೌಲ್ಯ_STRING:
        ಸ್ಥಿತಿ NGX_JSON_ಮೌಲ್ಯ_STRING_ESCAPED:
        ಸ್ಥಿತಿ NGX_JSON_ಮೌಲ್ಯ_ಸಂಖ್ಯೆ:
        ಸ್ಥಿತಿ NGX_JSON_ಮೌಲ್ಯ_BOOL:
        ಸ್ಥಿತಿ NGX_JSON_ಮೌಲ್ಯ_NULL:
            ಆದರೆ (ctx->ಆಳ > 0
                && ctx->ರಾಶಿ[ctx->ಆಳ - 1] == ngx_json_ctx_array
            ) {
                ctx->skip_until_ಆಳ = ctx->ಆಳ - 1;
            }
            ನಿಲ್ಲಿಸು;

        ಪೂರ್ವನಿಯೋಜಿತ:
            ನಿಲ್ಲಿಸು;
        }

        ಹಿಂತಿರುಗಿ NGX_OK;
    }

    ಹಿಂತಿರುಗಿ rc;
}


ಸ್ಥಿರ ngx_inline ngx_json_state_e
ngx_json_after_ಮೌಲ್ಯ(ngx_json_ctx_t *ctx)
{
    ಆದರೆ (ctx->ಆಳ == 0) {
        ಹಿಂತಿರುಗಿ ngx_json_ಮುಗಿದ;
    }

    ಆದರೆ (ctx->ರಾಶಿ[ctx->ಆಳ - 1] == ngx_json_ctx_object) {
        ಹಿಂತಿರುಗಿ ngx_json_object_ಮೌಲ್ಯ_separator;
    }

    ಹಿಂತಿರುಗಿ ngx_json_array_ಮೌಲ್ಯ_separator;
}


ಸ್ಥಿರ ngx_inline ngx_int_t
ngx_json_close(ngx_json_ctx_t *ctx, ngx_json_event_e event,
    ngx_json_state_e *state, u_char *p)
{
    ngx_int_t  rc;

    ctx->ಆಳ--;

    rc = ngx_json_emit(ctx, event, p, 1);
    ಆದರೆ (rc != NGX_OK) {
        ಹಿಂತಿರುಗಿ rc;
    }

    *state = ngx_json_after_ಮೌಲ್ಯ(ctx);

    ಹಿಂತಿರುಗಿ NGX_OK;
}


ಸ್ಥಿರ ngx_int_t
ngx_json_ತಳ್ಳು_state(ngx_json_ctx_t *ctx, ngx_json_container_e container)
{
    ngx_json_container_e  *ರಾಶಿ;

    ಆದರೆ (ctx->ಆಳ >= ctx->max_ಆಳ) {
        ಹಿಂತಿರುಗಿ NGX_DECLINED;
    }

    ಆದರೆ (ctx->ರಾಶಿ == ಖಾಲಿ) {
        ರಾಶಿ = ngx_palloc(ctx->pool,
                           ctx->max_ಆಳ * ಗಾತ್ರ(ngx_json_container_e));
        ಆದರೆ (ರಾಶಿ == ಖಾಲಿ) {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        ctx->ರಾಶಿ = ರಾಶಿ;
    }

    ctx->ರಾಶಿ[ctx->ಆಳ] = container;
    ctx->ಆಳ++;

    ಹಿಂತಿರುಗಿ NGX_OK;
}
