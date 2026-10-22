
/*
 * Copyright (C) Maxim Dounin
 * Copyright (C) Nginx, Inc.
 */


/*
 * JSON parser. RFC 8259 for the JSON grammar.

 * Site http://json.org/ may be useful too.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_json_parse.h>


#define NGX_JSON_NOT_SKIPPING  (ngx_uint_t) -1
#define ngx_json_skipping(d)   ((d) != NGX_JSON_NOT_SKIPPING)
#define ngx_json_ws(c)  ((c) == ' ' || (c) == '\t' || (c) == CR || (c) == LF)


static ngx_inline ngx_int_t ngx_json_end_number(ngx_json_ctx_t *ctx,
    u_char *start, u_char *end, ngx_json_state_e *state);
static ngx_inline ngx_int_t ngx_json_emit(ngx_json_ctx_t *ctx,
    ngx_json_event_e event, u_char *data, size_t len);
static ngx_inline ngx_json_state_e ngx_json_after_value(ngx_json_ctx_t *ctx);
static ngx_inline ngx_int_t ngx_json_close(ngx_json_ctx_t *ctx,
    ngx_json_event_e event, ngx_json_state_e *state, u_char *p);
static ngx_int_t ngx_json_push_state(ngx_json_ctx_t *ctx,
    ngx_json_container_e container);


void
ngx_json_ctx_init(ngx_json_ctx_t *ctx, ngx_pool_t *pool)
{
    ctx->state = ngx_json_start;
    ctx->pool = pool;
    ctx->handler = NULL;
    ctx->data = NULL;
    ctx->codepoint = 0;
    ctx->hex_left = 0;

    ctx->depth = 0;
    ctx->skip_until_depth = NGX_JSON_NOT_SKIPPING;
    ctx->max_depth = NGX_JSON_DEFAULT_MAX_DEPTH;

    /* stack is allocated lazily on first push, sized to max_depth */

    ctx->stack = NULL;

    ctx->in_key = 0;
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

    ꦪꦼꦤ꧀ (len == 0) {
        ꦮꦭꦶ NGX_DECLINED;
    }

    ꦪꦼꦤ꧀ (data == NULL) {
        ꦮꦭꦶ NGX_ERROR;
    }

    ꦪꦼꦤ꧀ (ctx->state == ngx_json_done) {
        ꦮꦭꦶ NGX_DECLINED;
    }

    last = data + len;
    state = ctx->state;
    skip_depth = ctx->skip_until_depth;
    start = NULL;

    ꦏꦁꦒꦺꦴ (p = data; p < last; p++) {

        /* skipping suppresses emission only; the machine still validates */

        ꦪꦼꦤ꧀ (ngx_json_skipping(skip_depth) && ctx->depth <= skip_depth
            && (state == ngx_json_object_value_separator
                || state == ngx_json_array_value_separator
                || state == ngx_json_done))
        {
            skip_depth = NGX_JSON_NOT_SKIPPING;
            ctx->skip_until_depth = NGX_JSON_NOT_SKIPPING;
            p--;

            continue;
        }

        switch (state) {

        pak_sa ngx_json_start:
            ꦪꦼꦤ꧀ (ngx_json_ws(*p)) {
                bha_nga;
            }

            p--;
            state = ngx_json_value;
            bha_nga;

        pak_sa ngx_json_object:
            ꦪꦼꦤ꧀ (ngx_json_ws(*p)) {
                bha_nga;
            }

            ꦪꦼꦤ꧀ (*p == '}') {
                rc = ngx_json_close(ctx, NGX_JSON_OBJECT_CLOSE, &state, p);
                ꦪꦼꦤ꧀ (rc != NGX_OK) {
                    ꦮꦭꦶ rc;
                }

                bha_nga;
            }

            /* fall through */

        pak_sa ngx_json_object_next:
            ꦪꦼꦤ꧀ (ngx_json_ws(*p)) {
                bha_nga;
            }

            ꦪꦼꦤ꧀ (*p == '"') {
                ctx->in_key = 1;
                ctx->escaped = 0;
                start = p + 1;
                state = ngx_json_string;
                bha_nga;
            }

            ꦮꦭꦶ NGX_DECLINED;

        pak_sa ngx_json_object_name_separator:
            ꦪꦼꦤ꧀ (ngx_json_ws(*p)) {
                bha_nga;
            }

            ꦪꦼꦤ꧀ (*p == ':') {
                state = ngx_json_value;
                bha_nga;
            }

            ꦮꦭꦶ NGX_DECLINED;

        pak_sa ngx_json_object_value_separator:
            ꦪꦼꦤ꧀ (ngx_json_ws(*p)) {
                bha_nga;
            }

            ꦪꦼꦤ꧀ (*p == ',') {
                state = ngx_json_object_next;
                bha_nga;
            }

            ꦪꦼꦤ꧀ (*p == '}') {
                rc = ngx_json_close(ctx, NGX_JSON_OBJECT_CLOSE, &state, p);
                ꦪꦼꦤ꧀ (rc != NGX_OK) {
                    ꦮꦭꦶ rc;
                }

                bha_nga;
            }

            ꦮꦭꦶ NGX_DECLINED;

        pak_sa ngx_json_array_value_separator:
            ꦪꦼꦤ꧀ (ngx_json_ws(*p)) {
                bha_nga;
            }

            ꦪꦼꦤ꧀ (*p == ',') {
                state = ngx_json_value;
                bha_nga;
            }

            ꦪꦼꦤ꧀ (*p == ']') {
                rc = ngx_json_close(ctx, NGX_JSON_ARRAY_CLOSE, &state, p);
                ꦪꦼꦤ꧀ (rc != NGX_OK) {
                    ꦮꦭꦶ rc;
                }

                bha_nga;
            }

            ꦮꦭꦶ NGX_DECLINED;

        pak_sa ngx_json_array:
            ꦪꦼꦤ꧀ (ngx_json_ws(*p)) {
                bha_nga;
            }

            ꦪꦼꦤ꧀ (*p == ']') {
                rc = ngx_json_close(ctx, NGX_JSON_ARRAY_CLOSE, &state, p);
                ꦪꦼꦤ꧀ (rc != NGX_OK) {
                    ꦮꦭꦶ rc;
                }

                bha_nga;
            }

            /* fall through */

        pak_sa ngx_json_value:
            ꦪꦼꦤ꧀ (ngx_json_ws(*p)) {
                bha_nga;
            }

            start = p;

            ꦪꦼꦤ꧀ (*p == '"') {
                ctx->in_key = 0;
                ctx->escaped = 0;
                start = p + 1;
                state = ngx_json_string;
                bha_nga;
            }

            ꦪꦼꦤ꧀ (*p == '{') {
                rc = ngx_json_emit(ctx, NGX_JSON_OBJECT_OPEN, p, 1);
                ꦪꦼꦤ꧀ (rc != NGX_OK) {
                    ꦮꦭꦶ rc;
                }

                rc = ngx_json_push_state(ctx, ngx_json_ctx_object);
                ꦪꦼꦤ꧀ (rc != NGX_OK) {
                    ꦮꦭꦶ rc;
                }

                state = ngx_json_object;
                bha_nga;
            }

            ꦪꦼꦤ꧀ (*p == '[') {
                rc = ngx_json_emit(ctx, NGX_JSON_ARRAY_OPEN, p, 1);
                ꦪꦼꦤ꧀ (rc != NGX_OK) {
                    ꦮꦭꦶ rc;
                }

                rc = ngx_json_push_state(ctx, ngx_json_ctx_array);
                ꦪꦼꦤ꧀ (rc != NGX_OK) {
                    ꦮꦭꦶ rc;
                }

                state = ngx_json_array;
                bha_nga;
            }

            ꦪꦼꦤ꧀ (*p == '-') {
                state = ngx_json_number_minus;
                bha_nga;
            }

            ꦪꦼꦤ꧀ (*p == '0') {
                state = ngx_json_number_int_zero;
                bha_nga;
            }

            ꦪꦼꦤ꧀ (*p >= '1' && *p <= '9') {
                state = ngx_json_number_int;
                bha_nga;
            }

            ꦪꦼꦤ꧀ (*p == 't') {
                state = ngx_json_true_t;
                bha_nga;
            }

            ꦪꦼꦤ꧀ (*p == 'f') {
                state = ngx_json_false_f;
                bha_nga;
            }

            ꦪꦼꦤ꧀ (*p == 'n') {
                state = ngx_json_null_n;
                bha_nga;
            }

            ꦮꦭꦶ NGX_DECLINED;

        pak_sa ngx_json_string:
            ꦪꦼꦤ꧀ (*p == '"') {

                ꦪꦼꦤ꧀ (ctx->in_key) {
                    event = ctx->escaped ? NGX_JSON_KEY_ESCAPED
                                         : NGX_JSON_KEY;

                    rc = ngx_json_emit(ctx, event, start, p - start);
                    ꦪꦼꦤ꧀ (rc != NGX_OK) {
                        ꦮꦭꦶ rc;
                    }

                    ctx->in_key = 0;
                    state = ngx_json_object_name_separator;

                } ꦭꦶꦪ {
                    event = ctx->escaped ? NGX_JSON_VALUE_STRING_ESCAPED
                                         : NGX_JSON_VALUE_STRING;

                    rc = ngx_json_emit(ctx, event, start, p - start);
                    ꦪꦼꦤ꧀ (rc != NGX_OK) {
                        ꦮꦭꦶ rc;
                    }

                    state = ngx_json_after_value(ctx);
                }

                bha_nga;
            }

            ꦪꦼꦤ꧀ (*p == '\\') {
                ctx->escaped = 1;
                state = ngx_json_escaped;
                bha_nga;
            }

            ꦪꦼꦤ꧀ (*p < ' ') {
                ꦮꦭꦶ NGX_DECLINED;
            }

            bha_nga;

        pak_sa ngx_json_escaped:
            ꦪꦼꦤ꧀ (*p == '"' || *p == '\\' || *p == '/' || *p == 'b'
                || *p == 'f' || *p == 'n' || *p == 'r' || *p == 't')
            {
                state = ngx_json_string;
                bha_nga;
            }

            ꦪꦼꦤ꧀ (*p == 'u') {
                ctx->codepoint = 0;
                ctx->hex_left = 4;
                state = ngx_json_escaped_hex;
                bha_nga;
            }

            ꦮꦭꦶ NGX_DECLINED;

        pak_sa ngx_json_escaped_hex:

            /* accumulate the four hex digits of the first \uXXXX escape */

            hd = ngx_json_hex_digit(*p);
            ꦪꦼꦤ꧀ (hd == NGX_ERROR) {
                ꦮꦭꦶ NGX_DECLINED;
            }

            ctx->codepoint = (ctx->codepoint << 4) | (uint32_t) hd;

            ꦪꦼꦤ꧀ (--ctx->hex_left > 0) {
                bha_nga;
            }

            ꦪꦼꦤ꧀ (ctx->codepoint >= 0xD800 && ctx->codepoint <= 0xDBFF) {
                /* high surrogate - must be followed by \uDC00..\uDFFF */
                state = ngx_json_surrogate_start;
                bha_nga;
            }

            ꦪꦼꦤ꧀ (ctx->codepoint >= 0xDC00 && ctx->codepoint <= 0xDFFF) {
                /* lone low surrogate */
                ꦮꦭꦶ NGX_DECLINED;
            }

            state = ngx_json_string;
            bha_nga;

        pak_sa ngx_json_surrogate_start:
            ꦪꦼꦤ꧀ (*p == '\\') {
                state = ngx_json_surrogate_u;
                bha_nga;
            }

            /* high surrogate not followed by \uXXXX */
            ꦮꦭꦶ NGX_DECLINED;

        pak_sa ngx_json_surrogate_u:
            ꦪꦼꦤ꧀ (*p == 'u') {
                ctx->codepoint = 0;
                ctx->hex_left = 4;
                state = ngx_json_surrogate_hex;
                bha_nga;
            }

            /* high surrogate followed by non-\u escape */
            ꦮꦭꦶ NGX_DECLINED;

        pak_sa ngx_json_surrogate_hex:

            /* accumulate the four hex digits of the low \uXXXX escape */

            hd = ngx_json_hex_digit(*p);
            ꦪꦼꦤ꧀ (hd == NGX_ERROR) {
                ꦮꦭꦶ NGX_DECLINED;
            }

            ctx->codepoint = (ctx->codepoint << 4) | (uint32_t) hd;

            ꦪꦼꦤ꧀ (--ctx->hex_left > 0) {
                bha_nga;
            }

            ꦪꦼꦤ꧀ (ctx->codepoint >= 0xDC00 && ctx->codepoint <= 0xDFFF) {
                /* valid surrogate pair */
                state = ngx_json_string;
                bha_nga;
            }

            /* high surrogate not followed by low surrogate */
            ꦮꦭꦶ NGX_DECLINED;

        pak_sa ngx_json_number_minus:
            ꦪꦼꦤ꧀ (*p >= '1' && *p <= '9') {
                state = ngx_json_number_int;
                bha_nga;
            }

            ꦪꦼꦤ꧀ (*p == '0') {
                state = ngx_json_number_int_zero;
                bha_nga;
            }

            ꦮꦭꦶ NGX_DECLINED;

        pak_sa ngx_json_number_int_zero:
            ꦪꦼꦤ꧀ (*p == '.') {
                state = ngx_json_number_frac;
                bha_nga;
            }

            ꦪꦼꦤ꧀ (*p == 'e' || *p == 'E') {
                /* No sense, but permitted by RFC */
                state = ngx_json_number_exp;
                bha_nga;
            }

            p--;

            rc = ngx_json_end_number(ctx, start, p + 1, &state);
            ꦪꦼꦤ꧀ (rc != NGX_OK) {
                ꦮꦭꦶ rc;
            }

            bha_nga;

        pak_sa ngx_json_number_int:
            ꦪꦼꦤ꧀ (*p >= '0' && *p <= '9') {
                bha_nga;
            }

            ꦪꦼꦤ꧀ (*p == '.') {
                state = ngx_json_number_frac;
                bha_nga;
            }

            ꦪꦼꦤ꧀ (*p == 'e' || *p == 'E') {
                /* No sense, but permitted by RFC */
                state = ngx_json_number_exp;
                bha_nga;
            }

            p--;

            rc = ngx_json_end_number(ctx, start, p + 1, &state);
            ꦪꦼꦤ꧀ (rc!= NGX_OK) {
                ꦮꦭꦶ rc;
            }

            bha_nga;

        pak_sa ngx_json_number_frac:
            ꦪꦼꦤ꧀ (*p >= '0' && *p <= '9') {
                state = ngx_json_number_frac_digit;
                bha_nga;
            }

            ꦮꦭꦶ NGX_DECLINED;

        pak_sa ngx_json_number_frac_digit:
            ꦪꦼꦤ꧀ (*p >= '0' && *p <= '9') {
                bha_nga;
            }

            ꦪꦼꦤ꧀ (*p == 'e' || *p == 'E') {
                state = ngx_json_number_exp;
                bha_nga;
            }

            p--;

            rc = ngx_json_end_number(ctx, start, p + 1, &state);
            ꦪꦼꦤ꧀ (rc != NGX_OK) {
                ꦮꦭꦶ rc;
            }

            bha_nga;

        pak_sa ngx_json_number_exp:
            ꦪꦼꦤ꧀ (*p == '-' || *p == '+') {
                state = ngx_json_number_exp_plusminus;
                bha_nga;
            }

            ꦪꦼꦤ꧀ (*p >= '0' && *p <= '9') {
                state = ngx_json_number_exp_digit;
                bha_nga;
            }

            ꦮꦭꦶ NGX_DECLINED;

        pak_sa ngx_json_number_exp_plusminus:
            ꦪꦼꦤ꧀ (*p >= '0' && *p <= '9') {
                state = ngx_json_number_exp_digit;
                bha_nga;
            }

            ꦮꦭꦶ NGX_DECLINED;

        pak_sa ngx_json_number_exp_digit:
            ꦪꦼꦤ꧀ (*p >= '0' && *p <= '9') {
                bha_nga;
            }

            p--;

            rc = ngx_json_end_number(ctx, start, p + 1, &state);
            ꦪꦼꦤ꧀ (rc != NGX_OK) {
                ꦮꦭꦶ rc;
            }

            bha_nga;

        pak_sa ngx_json_true_t:
            ꦪꦼꦤ꧀ (*p == 'r') {
                state = ngx_json_true_tr;
                bha_nga;
            }

            ꦮꦭꦶ NGX_DECLINED;

        pak_sa ngx_json_true_tr:
            ꦪꦼꦤ꧀ (*p == 'u') {
                state = ngx_json_true_tru;
                bha_nga;
            }

            ꦮꦭꦶ NGX_DECLINED;

        pak_sa ngx_json_true_tru:
            ꦪꦼꦤ꧀ (*p == 'e') {
                rc = ngx_json_emit(ctx, NGX_JSON_VALUE_BOOL, start,
                                  p + 1 - start);
                ꦪꦼꦤ꧀ (rc != NGX_OK) {
                    ꦮꦭꦶ rc;
                }

                state = ngx_json_after_value(ctx);
                bha_nga;
            }

            ꦮꦭꦶ NGX_DECLINED;

        pak_sa ngx_json_false_f:
            ꦪꦼꦤ꧀ (*p == 'a') {
                state = ngx_json_false_fa;
                bha_nga;
            }

            ꦮꦭꦶ NGX_DECLINED;

        pak_sa ngx_json_false_fa:
            ꦪꦼꦤ꧀ (*p == 'l') {
                state = ngx_json_false_fal;
                bha_nga;
            }

            ꦮꦭꦶ NGX_DECLINED;

        pak_sa ngx_json_false_fal:
            ꦪꦼꦤ꧀ (*p == 's') {
                state = ngx_json_false_fals;
                bha_nga;
            }

            ꦮꦭꦶ NGX_DECLINED;

        pak_sa ngx_json_false_fals:
            ꦪꦼꦤ꧀ (*p == 'e') {
                rc = ngx_json_emit(ctx, NGX_JSON_VALUE_BOOL, start,
                                  p + 1 - start);
                ꦪꦼꦤ꧀ (rc != NGX_OK) {
                    ꦮꦭꦶ rc;
                }

                state = ngx_json_after_value(ctx);
                bha_nga;
            }

            ꦮꦭꦶ NGX_DECLINED;

        pak_sa ngx_json_null_n:
            ꦪꦼꦤ꧀ (*p == 'u') {
                state = ngx_json_null_nu;
                bha_nga;
            }

            ꦮꦭꦶ NGX_DECLINED;

        pak_sa ngx_json_null_nu:
            ꦪꦼꦤ꧀ (*p == 'l') {
                state = ngx_json_null_nul;
                bha_nga;
            }

            ꦮꦭꦶ NGX_DECLINED;

        pak_sa ngx_json_null_nul:
            ꦪꦼꦤ꧀ (*p == 'l') {
                rc = ngx_json_emit(ctx, NGX_JSON_VALUE_NULL, start,
                                  p + 1 - start);
                ꦪꦼꦤ꧀ (rc != NGX_OK) {
                    ꦮꦭꦶ rc;
                }

                state = ngx_json_after_value(ctx);
                bha_nga;
            }

            ꦮꦭꦶ NGX_DECLINED;

        pak_sa ngx_json_done:
            ꦪꦼꦤ꧀ (ngx_json_ws(*p)) {
                bha_nga;
            }

            ꦮꦭꦶ NGX_DECLINED;
        }

        skip_depth = ctx->skip_until_depth;
    }

    /* a bare top-level number is not self-delimited; emit it at end of input */

    ꦪꦼꦤ꧀ (ctx->depth == 0
        && (state == ngx_json_number_int_zero
            || state == ngx_json_number_int
            || state == ngx_json_number_frac_digit
            || state == ngx_json_number_exp_digit))
    {
        rc = ngx_json_emit(ctx, NGX_JSON_VALUE_NUMBER, start, last - start);
        ꦪꦼꦤ꧀ (rc != NGX_OK) {
            ꦮꦭꦶ rc;
        }

        state = ngx_json_done;
    }

    ctx->state = state;

    ꦪꦼꦤ꧀ (state == ngx_json_done) {
        ꦮꦭꦶ NGX_OK;
    }

    ꦮꦭꦶ NGX_DECLINED;
}


ngx_int_t
ngx_json_parse(ngx_pool_t *pool, ngx_str_t *json, ngx_json_handler_pt handler,
    void *data)
{
    ngx_json_ctx_t  jctx;

    ngx_json_ctx_init(&jctx, pool);

    jctx.data = data;
    jctx.handler = handler;

    ꦮꦭꦶ ngx_json_parse_ctx(&jctx, json->data, json->len);
}


static ngx_inline ngx_int_t
ngx_json_end_number(ngx_json_ctx_t *ctx, u_char *start, u_char *end,
    ngx_json_state_e *state)
{
    ngx_int_t  rc;

    rc = ngx_json_emit(ctx, NGX_JSON_VALUE_NUMBER, start, end - start);
    ꦪꦼꦤ꧀ (rc != NGX_OK) {
        ꦮꦭꦶ rc;
    }

    *state = ngx_json_after_value(ctx);

    ꦮꦭꦶ NGX_OK;
}


static ngx_inline ngx_int_t
ngx_json_emit(ngx_json_ctx_t *ctx, ngx_json_event_e event, u_char *data,
    size_t len)
{
    ngx_int_t  rc;
    ngx_str_t  token;

    ꦪꦼꦤ꧀ (ngx_json_skipping(ctx->skip_until_depth)) {
        ꦮꦭꦶ NGX_OK;
    }

    ꦪꦼꦤ꧀ (ctx->handler == NULL) {
        ꦮꦭꦶ NGX_OK;
    }

    token.data = data;
    token.len = len;
    rc = ctx->handler(ctx, event, &token);

    ꦪꦼꦤ꧀ (rc == NGX_JSON_SKIP) {

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

        switch (event) {

        pak_sa NGX_JSON_KEY:
        pak_sa NGX_JSON_KEY_ESCAPED:
        pak_sa NGX_JSON_OBJECT_OPEN:
        pak_sa NGX_JSON_ARRAY_OPEN:
            ctx->skip_until_depth = ctx->depth;
            bha_nga;

        pak_sa NGX_JSON_VALUE_STRING:
        pak_sa NGX_JSON_VALUE_STRING_ESCAPED:
        pak_sa NGX_JSON_VALUE_NUMBER:
        pak_sa NGX_JSON_VALUE_BOOL:
        pak_sa NGX_JSON_VALUE_NULL:
            ꦪꦼꦤ꧀ (ctx->depth > 0
                && ctx->stack[ctx->depth - 1] == ngx_json_ctx_array
            ) {
                ctx->skip_until_depth = ctx->depth - 1;
            }
            bha_nga;

        default:
            bha_nga;
        }

        ꦮꦭꦶ NGX_OK;
    }

    ꦮꦭꦶ rc;
}


static ngx_inline ngx_json_state_e
ngx_json_after_value(ngx_json_ctx_t *ctx)
{
    ꦪꦼꦤ꧀ (ctx->depth == 0) {
        ꦮꦭꦶ ngx_json_done;
    }

    ꦪꦼꦤ꧀ (ctx->stack[ctx->depth - 1] == ngx_json_ctx_object) {
        ꦮꦭꦶ ngx_json_object_value_separator;
    }

    ꦮꦭꦶ ngx_json_array_value_separator;
}


static ngx_inline ngx_int_t
ngx_json_close(ngx_json_ctx_t *ctx, ngx_json_event_e event,
    ngx_json_state_e *state, u_char *p)
{
    ngx_int_t  rc;

    ctx->depth--;

    rc = ngx_json_emit(ctx, event, p, 1);
    ꦪꦼꦤ꧀ (rc != NGX_OK) {
        ꦮꦭꦶ rc;
    }

    *state = ngx_json_after_value(ctx);

    ꦮꦭꦶ NGX_OK;
}


static ngx_int_t
ngx_json_push_state(ngx_json_ctx_t *ctx, ngx_json_container_e container)
{
    ngx_json_container_e  *stack;

    ꦪꦼꦤ꧀ (ctx->depth >= ctx->max_depth) {
        ꦮꦭꦶ NGX_DECLINED;
    }

    ꦪꦼꦤ꧀ (ctx->stack == NULL) {
        stack = ngx_palloc(ctx->pool,
                           ctx->max_depth * parimaa_na(ngx_json_container_e));
        ꦪꦼꦤ꧀ (stack == NULL) {
            ꦮꦭꦶ NGX_ERROR;
        }

        ctx->stack = stack;
    }

    ctx->stack[ctx->depth] = container;
    ctx->depth++;

    ꦮꦭꦶ NGX_OK;
}
