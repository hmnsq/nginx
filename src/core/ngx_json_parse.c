
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


#define NGX_JSON_NOT_SKIPPING  (ngx_uint_t) -1
#define ngx_json_skipping(d)   ((d) != NGX_JSON_NOT_SKIPPING)
#define ngx_json_ws(c)  ((c) == ' ' || (c) == '\t' || (c) == CR || (c) == LF)


ಸ್ಥಿರ ngx_inline ngx_int_t ngx_json_end_ಸಂಖ್ಯೆ(ngx_json_ctx_t *ctx,
    u_char *start, u_char *end, ngx_json_state_e *state);
ಸ್ಥಿರ ngx_inline ngx_int_t ngx_json_emit(ngx_json_ctx_t *ctx,
    ngx_json_event_e event, u_char *data, size_t len);
ಸ್ಥಿರ ngx_inline ngx_json_state_e ngx_json_after_ಮೌಲ್ಯ(ngx_json_ctx_t *ctx);
ಸ್ಥಿರ ngx_inline ngx_int_t ngx_json_close(ngx_json_ctx_t *ctx,
    ngx_json_event_e event, ngx_json_state_e *state, u_char *p);
ಸ್ಥಿರ ngx_int_t ngx_json_ತಳ್ಳು_state(ngx_json_ctx_t *ctx,
    ngx_json_container_e container);


ಶೂನ್ಯ
ngx_json_ctx_init(ngx_json_ctx_t *ctx, ngx_pool_t *pool)
{
    ctx->state = ngx_json_start;
    ctx->pool = pool;
    ctx->handler = ರಿಕ್ತ;
    ctx->data = ರಿಕ್ತ;
    ctx->codepoint = 0;
    ctx->hex_ಎಡ = 0;

    ctx->depth = 0;
    ctx->skip_until_depth = NGX_JSON_NOT_SKIPPING;
    ctx->max_depth = NGX_JSON_DEFAULT_MAX_DEPTH;

    /* stack is allocated lazily on first push, sized to max_depth */

    ctx->ರಾಶಿ = ರಿಕ್ತ;

    ctx->in_ಕೀಲಿ = 0;
    ctx->escaped = 0;
}


ngx_int_t
ngx_json_parse_ctx(ngx_json_ctx_t *ctx, u_char *data, size_t len)
{
    u_char            *p, *last, *start;
    ngx_int_t          rc, hd;
    ngx_uint_t         skip_depth;
    ngx_json_event_e   event;
    ngx_json_state_e   state;

    ಯದಿ (len == 0) {
        ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;
    }

    ಯದಿ (data == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    ಯದಿ (ctx->state == ngx_json_done) {
        ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;
    }

    last = data + len;
    state = ctx->state;
    skip_depth = ctx->skip_until_depth;
    start = ರಿಕ್ತ;

    ಪ್ರತಿ (p = data; p < last; p++) {

        /* skipping suppresses emission only; the machine still validates */

        ಯದಿ (ngx_json_skipping(skip_depth) && ctx->depth <= skip_depth
            && (state == ngx_json_object_ಮೌಲ್ಯ_separator
                || state == ngx_json_array_ಮೌಲ್ಯ_separator
                || state == ngx_json_done))
        {
            skip_depth = NGX_JSON_NOT_SKIPPING;
            ctx->skip_until_depth = NGX_JSON_NOT_SKIPPING;
            p--;

            ಅನುವರ್ತ;
        }

        ವಿಕಲ್ಪ (state) {

        ಪಕ್ಷ ngx_json_start:
            ಯದಿ (ngx_json_ws(*p)) {
                ಭಙ್ಗ;
            }

            p--;
            state = ngx_json_ಮೌಲ್ಯ;
            ಭಙ್ಗ;

        ಪಕ್ಷ ngx_json_object:
            ಯದಿ (ngx_json_ws(*p)) {
                ಭಙ್ಗ;
            }

            ಯದಿ (*p == '}') {
                rc = ngx_json_close(ctx, NGX_JSON_OBJECT_CLOSE, &state, p);
                ಯದಿ (rc != NGX_OK) {
                    ಪ್ರತ್ಯಾಗಮ rc;
                }

                ಭಙ್ಗ;
            }

            /* fall through */

        ಪಕ್ಷ ngx_json_object_next:
            ಯದಿ (ngx_json_ws(*p)) {
                ಭಙ್ಗ;
            }

            ಯದಿ (*p == '"') {
                ctx->in_ಕೀಲಿ = 1;
                ctx->escaped = 0;
                start = p + 1;
                state = ngx_json_string;
                ಭಙ್ಗ;
            }

            ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;

        ಪಕ್ಷ ngx_json_object_name_separator:
            ಯದಿ (ngx_json_ws(*p)) {
                ಭಙ್ಗ;
            }

            ಯದಿ (*p == ':') {
                state = ngx_json_ಮೌಲ್ಯ;
                ಭಙ್ಗ;
            }

            ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;

        ಪಕ್ಷ ngx_json_object_ಮೌಲ್ಯ_separator:
            ಯದಿ (ngx_json_ws(*p)) {
                ಭಙ್ಗ;
            }

            ಯದಿ (*p == ',') {
                state = ngx_json_object_next;
                ಭಙ್ಗ;
            }

            ಯದಿ (*p == '}') {
                rc = ngx_json_close(ctx, NGX_JSON_OBJECT_CLOSE, &state, p);
                ಯದಿ (rc != NGX_OK) {
                    ಪ್ರತ್ಯಾಗಮ rc;
                }

                ಭಙ್ಗ;
            }

            ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;

        ಪಕ್ಷ ngx_json_array_ಮೌಲ್ಯ_separator:
            ಯದಿ (ngx_json_ws(*p)) {
                ಭಙ್ಗ;
            }

            ಯದಿ (*p == ',') {
                state = ngx_json_ಮೌಲ್ಯ;
                ಭಙ್ಗ;
            }

            ಯದಿ (*p == ']') {
                rc = ngx_json_close(ctx, NGX_JSON_ARRAY_CLOSE, &state, p);
                ಯದಿ (rc != NGX_OK) {
                    ಪ್ರತ್ಯಾಗಮ rc;
                }

                ಭಙ್ಗ;
            }

            ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;

        ಪಕ್ಷ ngx_json_array:
            ಯದಿ (ngx_json_ws(*p)) {
                ಭಙ್ಗ;
            }

            ಯದಿ (*p == ']') {
                rc = ngx_json_close(ctx, NGX_JSON_ARRAY_CLOSE, &state, p);
                ಯದಿ (rc != NGX_OK) {
                    ಪ್ರತ್ಯಾಗಮ rc;
                }

                ಭಙ್ಗ;
            }

            /* fall through */

        ಪಕ್ಷ ngx_json_ಮೌಲ್ಯ:
            ಯದಿ (ngx_json_ws(*p)) {
                ಭಙ್ಗ;
            }

            start = p;

            ಯದಿ (*p == '"') {
                ctx->in_ಕೀಲಿ = 0;
                ctx->escaped = 0;
                start = p + 1;
                state = ngx_json_string;
                ಭಙ್ಗ;
            }

            ಯದಿ (*p == '{') {
                rc = ngx_json_emit(ctx, NGX_JSON_OBJECT_OPEN, p, 1);
                ಯದಿ (rc != NGX_OK) {
                    ಪ್ರತ್ಯಾಗಮ rc;
                }

                rc = ngx_json_ತಳ್ಳು_state(ctx, ngx_json_ctx_object);
                ಯದಿ (rc != NGX_OK) {
                    ಪ್ರತ್ಯಾಗಮ rc;
                }

                state = ngx_json_object;
                ಭಙ್ಗ;
            }

            ಯದಿ (*p == '[') {
                rc = ngx_json_emit(ctx, NGX_JSON_ARRAY_OPEN, p, 1);
                ಯದಿ (rc != NGX_OK) {
                    ಪ್ರತ್ಯಾಗಮ rc;
                }

                rc = ngx_json_ತಳ್ಳು_state(ctx, ngx_json_ctx_array);
                ಯದಿ (rc != NGX_OK) {
                    ಪ್ರತ್ಯಾಗಮ rc;
                }

                state = ngx_json_array;
                ಭಙ್ಗ;
            }

            ಯದಿ (*p == '-') {
                state = ngx_json_ಸಂಖ್ಯೆ_minus;
                ಭಙ್ಗ;
            }

            ಯದಿ (*p == '0') {
                state = ngx_json_ಸಂಖ್ಯೆ_int_zero;
                ಭಙ್ಗ;
            }

            ಯದಿ (*p >= '1' && *p <= '9') {
                state = ngx_json_ಸಂಖ್ಯೆ_int;
                ಭಙ್ಗ;
            }

            ಯದಿ (*p == 't') {
                state = ngx_json_true_t;
                ಭಙ್ಗ;
            }

            ಯದಿ (*p == 'f') {
                state = ngx_json_false_f;
                ಭಙ್ಗ;
            }

            ಯದಿ (*p == 'n') {
                state = ngx_json_null_n;
                ಭಙ್ಗ;
            }

            ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;

        ಪಕ್ಷ ngx_json_string:
            ಯದಿ (*p == '"') {

                ಯದಿ (ctx->in_ಕೀಲಿ) {
                    event = ctx->escaped ? NGX_JSON_ಕೀಲಿ_ESCAPED
                                         : NGX_JSON_ಕೀಲಿ;

                    rc = ngx_json_emit(ctx, event, start, p - start);
                    ಯದಿ (rc != NGX_OK) {
                        ಪ್ರತ್ಯಾಗಮ rc;
                    }

                    ctx->in_ಕೀಲಿ = 0;
                    state = ngx_json_object_name_separator;

                } ಅನ್ಯಥಾ {
                    event = ctx->escaped ? NGX_JSON_ಮೌಲ್ಯ_STRING_ESCAPED
                                         : NGX_JSON_ಮೌಲ್ಯ_STRING;

                    rc = ngx_json_emit(ctx, event, start, p - start);
                    ಯದಿ (rc != NGX_OK) {
                        ಪ್ರತ್ಯಾಗಮ rc;
                    }

                    state = ngx_json_after_ಮೌಲ್ಯ(ctx);
                }

                ಭಙ್ಗ;
            }

            ಯದಿ (*p == '\\') {
                ctx->escaped = 1;
                state = ngx_json_escaped;
                ಭಙ್ಗ;
            }

            ಯದಿ (*p < ' ') {
                ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;
            }

            ಭಙ್ಗ;

        ಪಕ್ಷ ngx_json_escaped:
            ಯದಿ (*p == '"' || *p == '\\' || *p == '/' || *p == 'b'
                || *p == 'f' || *p == 'n' || *p == 'r' || *p == 't')
            {
                state = ngx_json_string;
                ಭಙ್ಗ;
            }

            ಯದಿ (*p == 'u') {
                ctx->codepoint = 0;
                ctx->hex_ಎಡ = 4;
                state = ngx_json_escaped_hex;
                ಭಙ್ಗ;
            }

            ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;

        ಪಕ್ಷ ngx_json_escaped_hex:

            /* accumulate the four hex digits of the first \uXXXX escape */

            hd = ngx_json_hex_digit(*p);
            ಯದಿ (hd == NGX_ERROR) {
                ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;
            }

            ctx->codepoint = (ctx->codepoint << 4) | (uint32_t) hd;

            ಯದಿ (--ctx->hex_ಎಡ > 0) {
                ಭಙ್ಗ;
            }

            ಯದಿ (ctx->codepoint >= 0xD800 && ctx->codepoint <= 0xDBFF) {
                /* high surrogate - must be followed by \uDC00..\uDFFF */
                state = ngx_json_surrogate_start;
                ಭಙ್ಗ;
            }

            ಯದಿ (ctx->codepoint >= 0xDC00 && ctx->codepoint <= 0xDFFF) {
                /* lone low surrogate */
                ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;
            }

            state = ngx_json_string;
            ಭಙ್ಗ;

        ಪಕ್ಷ ngx_json_surrogate_start:
            ಯದಿ (*p == '\\') {
                state = ngx_json_surrogate_u;
                ಭಙ್ಗ;
            }

            /* high surrogate not followed by \uXXXX */
            ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;

        ಪಕ್ಷ ngx_json_surrogate_u:
            ಯದಿ (*p == 'u') {
                ctx->codepoint = 0;
                ctx->hex_ಎಡ = 4;
                state = ngx_json_surrogate_hex;
                ಭಙ್ಗ;
            }

            /* high surrogate followed by non-\u escape */
            ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;

        ಪಕ್ಷ ngx_json_surrogate_hex:

            /* accumulate the four hex digits of the low \uXXXX escape */

            hd = ngx_json_hex_digit(*p);
            ಯದಿ (hd == NGX_ERROR) {
                ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;
            }

            ctx->codepoint = (ctx->codepoint << 4) | (uint32_t) hd;

            ಯದಿ (--ctx->hex_ಎಡ > 0) {
                ಭಙ್ಗ;
            }

            ಯದಿ (ctx->codepoint >= 0xDC00 && ctx->codepoint <= 0xDFFF) {
                /* valid surrogate pair */
                state = ngx_json_string;
                ಭಙ್ಗ;
            }

            /* high surrogate not followed by low surrogate */
            ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;

        ಪಕ್ಷ ngx_json_ಸಂಖ್ಯೆ_minus:
            ಯದಿ (*p >= '1' && *p <= '9') {
                state = ngx_json_ಸಂಖ್ಯೆ_int;
                ಭಙ್ಗ;
            }

            ಯದಿ (*p == '0') {
                state = ngx_json_ಸಂಖ್ಯೆ_int_zero;
                ಭಙ್ಗ;
            }

            ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;

        ಪಕ್ಷ ngx_json_ಸಂಖ್ಯೆ_int_zero:
            ಯದಿ (*p == '.') {
                state = ngx_json_ಸಂಖ್ಯೆ_frac;
                ಭಙ್ಗ;
            }

            ಯದಿ (*p == 'e' || *p == 'E') {
                /* No sense, but permitted by RFC */
                state = ngx_json_ಸಂಖ್ಯೆ_exp;
                ಭಙ್ಗ;
            }

            p--;

            rc = ngx_json_end_ಸಂಖ್ಯೆ(ctx, start, p + 1, &state);
            ಯದಿ (rc != NGX_OK) {
                ಪ್ರತ್ಯಾಗಮ rc;
            }

            ಭಙ್ಗ;

        ಪಕ್ಷ ngx_json_ಸಂಖ್ಯೆ_int:
            ಯದಿ (*p >= '0' && *p <= '9') {
                ಭಙ್ಗ;
            }

            ಯದಿ (*p == '.') {
                state = ngx_json_ಸಂಖ್ಯೆ_frac;
                ಭಙ್ಗ;
            }

            ಯದಿ (*p == 'e' || *p == 'E') {
                /* No sense, but permitted by RFC */
                state = ngx_json_ಸಂಖ್ಯೆ_exp;
                ಭಙ್ಗ;
            }

            p--;

            rc = ngx_json_end_ಸಂಖ್ಯೆ(ctx, start, p + 1, &state);
            ಯದಿ (rc!= NGX_OK) {
                ಪ್ರತ್ಯಾಗಮ rc;
            }

            ಭಙ್ಗ;

        ಪಕ್ಷ ngx_json_ಸಂಖ್ಯೆ_frac:
            ಯದಿ (*p >= '0' && *p <= '9') {
                state = ngx_json_ಸಂಖ್ಯೆ_frac_digit;
                ಭಙ್ಗ;
            }

            ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;

        ಪಕ್ಷ ngx_json_ಸಂಖ್ಯೆ_frac_digit:
            ಯದಿ (*p >= '0' && *p <= '9') {
                ಭಙ್ಗ;
            }

            ಯದಿ (*p == 'e' || *p == 'E') {
                state = ngx_json_ಸಂಖ್ಯೆ_exp;
                ಭಙ್ಗ;
            }

            p--;

            rc = ngx_json_end_ಸಂಖ್ಯೆ(ctx, start, p + 1, &state);
            ಯದಿ (rc != NGX_OK) {
                ಪ್ರತ್ಯಾಗಮ rc;
            }

            ಭಙ್ಗ;

        ಪಕ್ಷ ngx_json_ಸಂಖ್ಯೆ_exp:
            ಯದಿ (*p == '-' || *p == '+') {
                state = ngx_json_ಸಂಖ್ಯೆ_exp_plusminus;
                ಭಙ್ಗ;
            }

            ಯದಿ (*p >= '0' && *p <= '9') {
                state = ngx_json_ಸಂಖ್ಯೆ_exp_digit;
                ಭಙ್ಗ;
            }

            ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;

        ಪಕ್ಷ ngx_json_ಸಂಖ್ಯೆ_exp_plusminus:
            ಯದಿ (*p >= '0' && *p <= '9') {
                state = ngx_json_ಸಂಖ್ಯೆ_exp_digit;
                ಭಙ್ಗ;
            }

            ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;

        ಪಕ್ಷ ngx_json_ಸಂಖ್ಯೆ_exp_digit:
            ಯದಿ (*p >= '0' && *p <= '9') {
                ಭಙ್ಗ;
            }

            p--;

            rc = ngx_json_end_ಸಂಖ್ಯೆ(ctx, start, p + 1, &state);
            ಯದಿ (rc != NGX_OK) {
                ಪ್ರತ್ಯಾಗಮ rc;
            }

            ಭಙ್ಗ;

        ಪಕ್ಷ ngx_json_true_t:
            ಯದಿ (*p == 'r') {
                state = ngx_json_true_tr;
                ಭಙ್ಗ;
            }

            ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;

        ಪಕ್ಷ ngx_json_true_tr:
            ಯದಿ (*p == 'u') {
                state = ngx_json_true_tru;
                ಭಙ್ಗ;
            }

            ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;

        ಪಕ್ಷ ngx_json_true_tru:
            ಯದಿ (*p == 'e') {
                rc = ngx_json_emit(ctx, NGX_JSON_ಮೌಲ್ಯ_BOOL, start,
                                  p + 1 - start);
                ಯದಿ (rc != NGX_OK) {
                    ಪ್ರತ್ಯಾಗಮ rc;
                }

                state = ngx_json_after_ಮೌಲ್ಯ(ctx);
                ಭಙ್ಗ;
            }

            ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;

        ಪಕ್ಷ ngx_json_false_f:
            ಯದಿ (*p == 'a') {
                state = ngx_json_false_fa;
                ಭಙ್ಗ;
            }

            ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;

        ಪಕ್ಷ ngx_json_false_fa:
            ಯದಿ (*p == 'l') {
                state = ngx_json_false_fal;
                ಭಙ್ಗ;
            }

            ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;

        ಪಕ್ಷ ngx_json_false_fal:
            ಯದಿ (*p == 's') {
                state = ngx_json_false_fals;
                ಭಙ್ಗ;
            }

            ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;

        ಪಕ್ಷ ngx_json_false_fals:
            ಯದಿ (*p == 'e') {
                rc = ngx_json_emit(ctx, NGX_JSON_ಮೌಲ್ಯ_BOOL, start,
                                  p + 1 - start);
                ಯದಿ (rc != NGX_OK) {
                    ಪ್ರತ್ಯಾಗಮ rc;
                }

                state = ngx_json_after_ಮೌಲ್ಯ(ctx);
                ಭಙ್ಗ;
            }

            ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;

        ಪಕ್ಷ ngx_json_null_n:
            ಯದಿ (*p == 'u') {
                state = ngx_json_null_nu;
                ಭಙ್ಗ;
            }

            ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;

        ಪಕ್ಷ ngx_json_null_nu:
            ಯದಿ (*p == 'l') {
                state = ngx_json_null_nul;
                ಭಙ್ಗ;
            }

            ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;

        ಪಕ್ಷ ngx_json_null_nul:
            ಯದಿ (*p == 'l') {
                rc = ngx_json_emit(ctx, NGX_JSON_ಮೌಲ್ಯ_NULL, start,
                                  p + 1 - start);
                ಯದಿ (rc != NGX_OK) {
                    ಪ್ರತ್ಯಾಗಮ rc;
                }

                state = ngx_json_after_ಮೌಲ್ಯ(ctx);
                ಭಙ್ಗ;
            }

            ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;

        ಪಕ್ಷ ngx_json_done:
            ಯದಿ (ngx_json_ws(*p)) {
                ಭಙ್ಗ;
            }

            ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;
        }

        skip_depth = ctx->skip_until_depth;
    }

    /* a bare top-level number is not self-delimited; emit it at end of input */

    ಯದಿ (ctx->depth == 0
        && (state == ngx_json_ಸಂಖ್ಯೆ_int_zero
            || state == ngx_json_ಸಂಖ್ಯೆ_int
            || state == ngx_json_ಸಂಖ್ಯೆ_frac_digit
            || state == ngx_json_ಸಂಖ್ಯೆ_exp_digit))
    {
        rc = ngx_json_emit(ctx, NGX_JSON_ಮೌಲ್ಯ_ಸಂಖ್ಯೆ, start, last - start);
        ಯದಿ (rc != NGX_OK) {
            ಪ್ರತ್ಯಾಗಮ rc;
        }

        state = ngx_json_done;
    }

    ctx->state = state;

    ಯದಿ (state == ngx_json_done) {
        ಪ್ರತ್ಯಾಗಮ NGX_OK;
    }

    ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;
}


ngx_int_t
ngx_json_parse(ngx_pool_t *pool, ngx_str_t *json, ngx_json_handler_pt handler,
    ಶೂನ್ಯ *data)
{
    ngx_json_ctx_t  jctx;

    ngx_json_ctx_init(&jctx, pool);

    jctx.data = data;
    jctx.handler = handler;

    ಪ್ರತ್ಯಾಗಮ ngx_json_parse_ctx(&jctx, json->data, json->len);
}


ಸ್ಥಿರ ngx_inline ngx_int_t
ngx_json_end_ಸಂಖ್ಯೆ(ngx_json_ctx_t *ctx, u_char *start, u_char *end,
    ngx_json_state_e *state)
{
    ngx_int_t  rc;

    rc = ngx_json_emit(ctx, NGX_JSON_ಮೌಲ್ಯ_ಸಂಖ್ಯೆ, start, end - start);
    ಯದಿ (rc != NGX_OK) {
        ಪ್ರತ್ಯಾಗಮ rc;
    }

    *state = ngx_json_after_ಮೌಲ್ಯ(ctx);

    ಪ್ರತ್ಯಾಗಮ NGX_OK;
}


ಸ್ಥಿರ ngx_inline ngx_int_t
ngx_json_emit(ngx_json_ctx_t *ctx, ngx_json_event_e event, u_char *data,
    size_t len)
{
    ngx_int_t  rc;
    ngx_str_t  token;

    ಯದಿ (ngx_json_skipping(ctx->skip_until_depth)) {
        ಪ್ರತ್ಯಾಗಮ NGX_OK;
    }

    ಯದಿ (ctx->handler == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ NGX_OK;
    }

    token.data = data;
    token.len = len;
    rc = ctx->handler(ctx, event, &token);

    ಯದಿ (rc == NGX_JSON_SKIP) {

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

        ವಿಕಲ್ಪ (event) {

        ಪಕ್ಷ NGX_JSON_ಕೀಲಿ:
        ಪಕ್ಷ NGX_JSON_ಕೀಲಿ_ESCAPED:
        ಪಕ್ಷ NGX_JSON_OBJECT_OPEN:
        ಪಕ್ಷ NGX_JSON_ARRAY_OPEN:
            ctx->skip_until_depth = ctx->depth;
            ಭಙ್ಗ;

        ಪಕ್ಷ NGX_JSON_ಮೌಲ್ಯ_STRING:
        ಪಕ್ಷ NGX_JSON_ಮೌಲ್ಯ_STRING_ESCAPED:
        ಪಕ್ಷ NGX_JSON_ಮೌಲ್ಯ_ಸಂಖ್ಯೆ:
        ಪಕ್ಷ NGX_JSON_ಮೌಲ್ಯ_BOOL:
        ಪಕ್ಷ NGX_JSON_ಮೌಲ್ಯ_NULL:
            ಯದಿ (ctx->depth > 0
                && ctx->ರಾಶಿ[ctx->depth - 1] == ngx_json_ctx_array
            ) {
                ctx->skip_until_depth = ctx->depth - 1;
            }
            ಭಙ್ಗ;

        ಸ್ವಭಾವ:
            ಭಙ್ಗ;
        }

        ಪ್ರತ್ಯಾಗಮ NGX_OK;
    }

    ಪ್ರತ್ಯಾಗಮ rc;
}


ಸ್ಥಿರ ngx_inline ngx_json_state_e
ngx_json_after_ಮೌಲ್ಯ(ngx_json_ctx_t *ctx)
{
    ಯದಿ (ctx->depth == 0) {
        ಪ್ರತ್ಯಾಗಮ ngx_json_done;
    }

    ಯದಿ (ctx->ರಾಶಿ[ctx->depth - 1] == ngx_json_ctx_object) {
        ಪ್ರತ್ಯಾಗಮ ngx_json_object_ಮೌಲ್ಯ_separator;
    }

    ಪ್ರತ್ಯಾಗಮ ngx_json_array_ಮೌಲ್ಯ_separator;
}


ಸ್ಥಿರ ngx_inline ngx_int_t
ngx_json_close(ngx_json_ctx_t *ctx, ngx_json_event_e event,
    ngx_json_state_e *state, u_char *p)
{
    ngx_int_t  rc;

    ctx->depth--;

    rc = ngx_json_emit(ctx, event, p, 1);
    ಯದಿ (rc != NGX_OK) {
        ಪ್ರತ್ಯಾಗಮ rc;
    }

    *state = ngx_json_after_ಮೌಲ್ಯ(ctx);

    ಪ್ರತ್ಯಾಗಮ NGX_OK;
}


ಸ್ಥಿರ ngx_int_t
ngx_json_ತಳ್ಳು_state(ngx_json_ctx_t *ctx, ngx_json_container_e container)
{
    ngx_json_container_e  *ರಾಶಿ;

    ಯದಿ (ctx->depth >= ctx->max_depth) {
        ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;
    }

    ಯದಿ (ctx->ರಾಶಿ == ರಿಕ್ತ) {
        ರಾಶಿ = ngx_palloc(ctx->pool,
                           ctx->max_depth * ಪರಿಮಾಣ(ngx_json_container_e));
        ಯದಿ (ರಾಶಿ == ರಿಕ್ತ) {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        ctx->ರಾಶಿ = ರಾಶಿ;
    }

    ctx->ರಾಶಿ[ctx->depth] = container;
    ctx->depth++;

    ಪ್ರತ್ಯಾಗಮ NGX_OK;
}
