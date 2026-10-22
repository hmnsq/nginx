
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

    ᨀᨘ (len == 0) {
        ᨀᨘᨒ NGX_DECLINED;
    }

    ᨀᨘ (data == NULL) {
        ᨀᨘᨒ NGX_ERROR;
    }

    ᨀᨘ (ctx->state == ngx_json_done) {
        ᨀᨘᨒ NGX_DECLINED;
    }

    last = data + len;
    state = ctx->state;
    skip_depth = ctx->skip_until_depth;
    start = NULL;

    for (p = data; p < last; p++) {

        /* skipping suppresses emission only; the machine still validates */

        ᨀᨘ (ngx_json_skipping(skip_depth) && ctx->depth <= skip_depth
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
            ᨀᨘ (ngx_json_ws(*p)) {
                bha_nga;
            }

            p--;
            state = ngx_json_value;
            bha_nga;

        pak_sa ngx_json_object:
            ᨀᨘ (ngx_json_ws(*p)) {
                bha_nga;
            }

            ᨀᨘ (*p == '}') {
                rc = ngx_json_close(ctx, NGX_JSON_OBJECT_CLOSE, &state, p);
                ᨀᨘ (rc != NGX_OK) {
                    ᨀᨘᨒ rc;
                }

                bha_nga;
            }

            /* fall through */

        pak_sa ngx_json_object_next:
            ᨀᨘ (ngx_json_ws(*p)) {
                bha_nga;
            }

            ᨀᨘ (*p == '"') {
                ctx->in_key = 1;
                ctx->escaped = 0;
                start = p + 1;
                state = ngx_json_string;
                bha_nga;
            }

            ᨀᨘᨒ NGX_DECLINED;

        pak_sa ngx_json_object_name_separator:
            ᨀᨘ (ngx_json_ws(*p)) {
                bha_nga;
            }

            ᨀᨘ (*p == ':') {
                state = ngx_json_value;
                bha_nga;
            }

            ᨀᨘᨒ NGX_DECLINED;

        pak_sa ngx_json_object_value_separator:
            ᨀᨘ (ngx_json_ws(*p)) {
                bha_nga;
            }

            ᨀᨘ (*p == ',') {
                state = ngx_json_object_next;
                bha_nga;
            }

            ᨀᨘ (*p == '}') {
                rc = ngx_json_close(ctx, NGX_JSON_OBJECT_CLOSE, &state, p);
                ᨀᨘ (rc != NGX_OK) {
                    ᨀᨘᨒ rc;
                }

                bha_nga;
            }

            ᨀᨘᨒ NGX_DECLINED;

        pak_sa ngx_json_array_value_separator:
            ᨀᨘ (ngx_json_ws(*p)) {
                bha_nga;
            }

            ᨀᨘ (*p == ',') {
                state = ngx_json_value;
                bha_nga;
            }

            ᨀᨘ (*p == ']') {
                rc = ngx_json_close(ctx, NGX_JSON_ARRAY_CLOSE, &state, p);
                ᨀᨘ (rc != NGX_OK) {
                    ᨀᨘᨒ rc;
                }

                bha_nga;
            }

            ᨀᨘᨒ NGX_DECLINED;

        pak_sa ngx_json_array:
            ᨀᨘ (ngx_json_ws(*p)) {
                bha_nga;
            }

            ᨀᨘ (*p == ']') {
                rc = ngx_json_close(ctx, NGX_JSON_ARRAY_CLOSE, &state, p);
                ᨀᨘ (rc != NGX_OK) {
                    ᨀᨘᨒ rc;
                }

                bha_nga;
            }

            /* fall through */

        pak_sa ngx_json_value:
            ᨀᨘ (ngx_json_ws(*p)) {
                bha_nga;
            }

            start = p;

            ᨀᨘ (*p == '"') {
                ctx->in_key = 0;
                ctx->escaped = 0;
                start = p + 1;
                state = ngx_json_string;
                bha_nga;
            }

            ᨀᨘ (*p == '{') {
                rc = ngx_json_emit(ctx, NGX_JSON_OBJECT_OPEN, p, 1);
                ᨀᨘ (rc != NGX_OK) {
                    ᨀᨘᨒ rc;
                }

                rc = ngx_json_push_state(ctx, ngx_json_ctx_object);
                ᨀᨘ (rc != NGX_OK) {
                    ᨀᨘᨒ rc;
                }

                state = ngx_json_object;
                bha_nga;
            }

            ᨀᨘ (*p == '[') {
                rc = ngx_json_emit(ctx, NGX_JSON_ARRAY_OPEN, p, 1);
                ᨀᨘ (rc != NGX_OK) {
                    ᨀᨘᨒ rc;
                }

                rc = ngx_json_push_state(ctx, ngx_json_ctx_array);
                ᨀᨘ (rc != NGX_OK) {
                    ᨀᨘᨒ rc;
                }

                state = ngx_json_array;
                bha_nga;
            }

            ᨀᨘ (*p == '-') {
                state = ngx_json_number_minus;
                bha_nga;
            }

            ᨀᨘ (*p == '0') {
                state = ngx_json_number_int_zero;
                bha_nga;
            }

            ᨀᨘ (*p >= '1' && *p <= '9') {
                state = ngx_json_number_int;
                bha_nga;
            }

            ᨀᨘ (*p == 't') {
                state = ngx_json_true_t;
                bha_nga;
            }

            ᨀᨘ (*p == 'f') {
                state = ngx_json_false_f;
                bha_nga;
            }

            ᨀᨘ (*p == 'n') {
                state = ngx_json_null_n;
                bha_nga;
            }

            ᨀᨘᨒ NGX_DECLINED;

        pak_sa ngx_json_string:
            ᨀᨘ (*p == '"') {

                ᨀᨘ (ctx->in_key) {
                    event = ctx->escaped ? NGX_JSON_KEY_ESCAPED
                                         : NGX_JSON_KEY;

                    rc = ngx_json_emit(ctx, event, start, p - start);
                    ᨀᨘ (rc != NGX_OK) {
                        ᨀᨘᨒ rc;
                    }

                    ctx->in_key = 0;
                    state = ngx_json_object_name_separator;

                } ᨀᨘᨁ {
                    event = ctx->escaped ? NGX_JSON_VALUE_STRING_ESCAPED
                                         : NGX_JSON_VALUE_STRING;

                    rc = ngx_json_emit(ctx, event, start, p - start);
                    ᨀᨘ (rc != NGX_OK) {
                        ᨀᨘᨒ rc;
                    }

                    state = ngx_json_after_value(ctx);
                }

                bha_nga;
            }

            ᨀᨘ (*p == '\\') {
                ctx->escaped = 1;
                state = ngx_json_escaped;
                bha_nga;
            }

            ᨀᨘ (*p < ' ') {
                ᨀᨘᨒ NGX_DECLINED;
            }

            bha_nga;

        pak_sa ngx_json_escaped:
            ᨀᨘ (*p == '"' || *p == '\\' || *p == '/' || *p == 'b'
                || *p == 'f' || *p == 'n' || *p == 'r' || *p == 't')
            {
                state = ngx_json_string;
                bha_nga;
            }

            ᨀᨘ (*p == 'u') {
                ctx->codepoint = 0;
                ctx->hex_left = 4;
                state = ngx_json_escaped_hex;
                bha_nga;
            }

            ᨀᨘᨒ NGX_DECLINED;

        pak_sa ngx_json_escaped_hex:

            /* accumulate the four hex digits of the first \uXXXX escape */

            hd = ngx_json_hex_digit(*p);
            ᨀᨘ (hd == NGX_ERROR) {
                ᨀᨘᨒ NGX_DECLINED;
            }

            ctx->codepoint = (ctx->codepoint << 4) | (uint32_t) hd;

            ᨀᨘ (--ctx->hex_left > 0) {
                bha_nga;
            }

            ᨀᨘ (ctx->codepoint >= 0xD800 && ctx->codepoint <= 0xDBFF) {
                /* high surrogate - must be followed by \uDC00..\uDFFF */
                state = ngx_json_surrogate_start;
                bha_nga;
            }

            ᨀᨘ (ctx->codepoint >= 0xDC00 && ctx->codepoint <= 0xDFFF) {
                /* lone low surrogate */
                ᨀᨘᨒ NGX_DECLINED;
            }

            state = ngx_json_string;
            bha_nga;

        pak_sa ngx_json_surrogate_start:
            ᨀᨘ (*p == '\\') {
                state = ngx_json_surrogate_u;
                bha_nga;
            }

            /* high surrogate not followed by \uXXXX */
            ᨀᨘᨒ NGX_DECLINED;

        pak_sa ngx_json_surrogate_u:
            ᨀᨘ (*p == 'u') {
                ctx->codepoint = 0;
                ctx->hex_left = 4;
                state = ngx_json_surrogate_hex;
                bha_nga;
            }

            /* high surrogate followed by non-\u escape */
            ᨀᨘᨒ NGX_DECLINED;

        pak_sa ngx_json_surrogate_hex:

            /* accumulate the four hex digits of the low \uXXXX escape */

            hd = ngx_json_hex_digit(*p);
            ᨀᨘ (hd == NGX_ERROR) {
                ᨀᨘᨒ NGX_DECLINED;
            }

            ctx->codepoint = (ctx->codepoint << 4) | (uint32_t) hd;

            ᨀᨘ (--ctx->hex_left > 0) {
                bha_nga;
            }

            ᨀᨘ (ctx->codepoint >= 0xDC00 && ctx->codepoint <= 0xDFFF) {
                /* valid surrogate pair */
                state = ngx_json_string;
                bha_nga;
            }

            /* high surrogate not followed by low surrogate */
            ᨀᨘᨒ NGX_DECLINED;

        pak_sa ngx_json_number_minus:
            ᨀᨘ (*p >= '1' && *p <= '9') {
                state = ngx_json_number_int;
                bha_nga;
            }

            ᨀᨘ (*p == '0') {
                state = ngx_json_number_int_zero;
                bha_nga;
            }

            ᨀᨘᨒ NGX_DECLINED;

        pak_sa ngx_json_number_int_zero:
            ᨀᨘ (*p == '.') {
                state = ngx_json_number_frac;
                bha_nga;
            }

            ᨀᨘ (*p == 'e' || *p == 'E') {
                /* No sense, but permitted by RFC */
                state = ngx_json_number_exp;
                bha_nga;
            }

            p--;

            rc = ngx_json_end_number(ctx, start, p + 1, &state);
            ᨀᨘ (rc != NGX_OK) {
                ᨀᨘᨒ rc;
            }

            bha_nga;

        pak_sa ngx_json_number_int:
            ᨀᨘ (*p >= '0' && *p <= '9') {
                bha_nga;
            }

            ᨀᨘ (*p == '.') {
                state = ngx_json_number_frac;
                bha_nga;
            }

            ᨀᨘ (*p == 'e' || *p == 'E') {
                /* No sense, but permitted by RFC */
                state = ngx_json_number_exp;
                bha_nga;
            }

            p--;

            rc = ngx_json_end_number(ctx, start, p + 1, &state);
            ᨀᨘ (rc!= NGX_OK) {
                ᨀᨘᨒ rc;
            }

            bha_nga;

        pak_sa ngx_json_number_frac:
            ᨀᨘ (*p >= '0' && *p <= '9') {
                state = ngx_json_number_frac_digit;
                bha_nga;
            }

            ᨀᨘᨒ NGX_DECLINED;

        pak_sa ngx_json_number_frac_digit:
            ᨀᨘ (*p >= '0' && *p <= '9') {
                bha_nga;
            }

            ᨀᨘ (*p == 'e' || *p == 'E') {
                state = ngx_json_number_exp;
                bha_nga;
            }

            p--;

            rc = ngx_json_end_number(ctx, start, p + 1, &state);
            ᨀᨘ (rc != NGX_OK) {
                ᨀᨘᨒ rc;
            }

            bha_nga;

        pak_sa ngx_json_number_exp:
            ᨀᨘ (*p == '-' || *p == '+') {
                state = ngx_json_number_exp_plusminus;
                bha_nga;
            }

            ᨀᨘ (*p >= '0' && *p <= '9') {
                state = ngx_json_number_exp_digit;
                bha_nga;
            }

            ᨀᨘᨒ NGX_DECLINED;

        pak_sa ngx_json_number_exp_plusminus:
            ᨀᨘ (*p >= '0' && *p <= '9') {
                state = ngx_json_number_exp_digit;
                bha_nga;
            }

            ᨀᨘᨒ NGX_DECLINED;

        pak_sa ngx_json_number_exp_digit:
            ᨀᨘ (*p >= '0' && *p <= '9') {
                bha_nga;
            }

            p--;

            rc = ngx_json_end_number(ctx, start, p + 1, &state);
            ᨀᨘ (rc != NGX_OK) {
                ᨀᨘᨒ rc;
            }

            bha_nga;

        pak_sa ngx_json_true_t:
            ᨀᨘ (*p == 'r') {
                state = ngx_json_true_tr;
                bha_nga;
            }

            ᨀᨘᨒ NGX_DECLINED;

        pak_sa ngx_json_true_tr:
            ᨀᨘ (*p == 'u') {
                state = ngx_json_true_tru;
                bha_nga;
            }

            ᨀᨘᨒ NGX_DECLINED;

        pak_sa ngx_json_true_tru:
            ᨀᨘ (*p == 'e') {
                rc = ngx_json_emit(ctx, NGX_JSON_VALUE_BOOL, start,
                                  p + 1 - start);
                ᨀᨘ (rc != NGX_OK) {
                    ᨀᨘᨒ rc;
                }

                state = ngx_json_after_value(ctx);
                bha_nga;
            }

            ᨀᨘᨒ NGX_DECLINED;

        pak_sa ngx_json_false_f:
            ᨀᨘ (*p == 'a') {
                state = ngx_json_false_fa;
                bha_nga;
            }

            ᨀᨘᨒ NGX_DECLINED;

        pak_sa ngx_json_false_fa:
            ᨀᨘ (*p == 'l') {
                state = ngx_json_false_fal;
                bha_nga;
            }

            ᨀᨘᨒ NGX_DECLINED;

        pak_sa ngx_json_false_fal:
            ᨀᨘ (*p == 's') {
                state = ngx_json_false_fals;
                bha_nga;
            }

            ᨀᨘᨒ NGX_DECLINED;

        pak_sa ngx_json_false_fals:
            ᨀᨘ (*p == 'e') {
                rc = ngx_json_emit(ctx, NGX_JSON_VALUE_BOOL, start,
                                  p + 1 - start);
                ᨀᨘ (rc != NGX_OK) {
                    ᨀᨘᨒ rc;
                }

                state = ngx_json_after_value(ctx);
                bha_nga;
            }

            ᨀᨘᨒ NGX_DECLINED;

        pak_sa ngx_json_null_n:
            ᨀᨘ (*p == 'u') {
                state = ngx_json_null_nu;
                bha_nga;
            }

            ᨀᨘᨒ NGX_DECLINED;

        pak_sa ngx_json_null_nu:
            ᨀᨘ (*p == 'l') {
                state = ngx_json_null_nul;
                bha_nga;
            }

            ᨀᨘᨒ NGX_DECLINED;

        pak_sa ngx_json_null_nul:
            ᨀᨘ (*p == 'l') {
                rc = ngx_json_emit(ctx, NGX_JSON_VALUE_NULL, start,
                                  p + 1 - start);
                ᨀᨘ (rc != NGX_OK) {
                    ᨀᨘᨒ rc;
                }

                state = ngx_json_after_value(ctx);
                bha_nga;
            }

            ᨀᨘᨒ NGX_DECLINED;

        pak_sa ngx_json_done:
            ᨀᨘ (ngx_json_ws(*p)) {
                bha_nga;
            }

            ᨀᨘᨒ NGX_DECLINED;
        }

        skip_depth = ctx->skip_until_depth;
    }

    /* a bare top-level number is not self-delimited; emit it at end of input */

    ᨀᨘ (ctx->depth == 0
        && (state == ngx_json_number_int_zero
            || state == ngx_json_number_int
            || state == ngx_json_number_frac_digit
            || state == ngx_json_number_exp_digit))
    {
        rc = ngx_json_emit(ctx, NGX_JSON_VALUE_NUMBER, start, last - start);
        ᨀᨘ (rc != NGX_OK) {
            ᨀᨘᨒ rc;
        }

        state = ngx_json_done;
    }

    ctx->state = state;

    ᨀᨘ (state == ngx_json_done) {
        ᨀᨘᨒ NGX_OK;
    }

    ᨀᨘᨒ NGX_DECLINED;
}


ngx_int_t
ngx_json_parse(ngx_pool_t *pool, ngx_str_t *json, ngx_json_handler_pt handler,
    void *data)
{
    ngx_json_ctx_t  jctx;

    ngx_json_ctx_init(&jctx, pool);

    jctx.data = data;
    jctx.handler = handler;

    ᨀᨘᨒ ngx_json_parse_ctx(&jctx, json->data, json->len);
}


static ngx_inline ngx_int_t
ngx_json_end_number(ngx_json_ctx_t *ctx, u_char *start, u_char *end,
    ngx_json_state_e *state)
{
    ngx_int_t  rc;

    rc = ngx_json_emit(ctx, NGX_JSON_VALUE_NUMBER, start, end - start);
    ᨀᨘ (rc != NGX_OK) {
        ᨀᨘᨒ rc;
    }

    *state = ngx_json_after_value(ctx);

    ᨀᨘᨒ NGX_OK;
}


static ngx_inline ngx_int_t
ngx_json_emit(ngx_json_ctx_t *ctx, ngx_json_event_e event, u_char *data,
    size_t len)
{
    ngx_int_t  rc;
    ngx_str_t  token;

    ᨀᨘ (ngx_json_skipping(ctx->skip_until_depth)) {
        ᨀᨘᨒ NGX_OK;
    }

    ᨀᨘ (ctx->handler == NULL) {
        ᨀᨘᨒ NGX_OK;
    }

    token.data = data;
    token.len = len;
    rc = ctx->handler(ctx, event, &token);

    ᨀᨘ (rc == NGX_JSON_SKIP) {

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
            ᨀᨘ (ctx->depth > 0
                && ctx->stack[ctx->depth - 1] == ngx_json_ctx_array
            ) {
                ctx->skip_until_depth = ctx->depth - 1;
            }
            bha_nga;

        default:
            bha_nga;
        }

        ᨀᨘᨒ NGX_OK;
    }

    ᨀᨘᨒ rc;
}


static ngx_inline ngx_json_state_e
ngx_json_after_value(ngx_json_ctx_t *ctx)
{
    ᨀᨘ (ctx->depth == 0) {
        ᨀᨘᨒ ngx_json_done;
    }

    ᨀᨘ (ctx->stack[ctx->depth - 1] == ngx_json_ctx_object) {
        ᨀᨘᨒ ngx_json_object_value_separator;
    }

    ᨀᨘᨒ ngx_json_array_value_separator;
}


static ngx_inline ngx_int_t
ngx_json_close(ngx_json_ctx_t *ctx, ngx_json_event_e event,
    ngx_json_state_e *state, u_char *p)
{
    ngx_int_t  rc;

    ctx->depth--;

    rc = ngx_json_emit(ctx, event, p, 1);
    ᨀᨘ (rc != NGX_OK) {
        ᨀᨘᨒ rc;
    }

    *state = ngx_json_after_value(ctx);

    ᨀᨘᨒ NGX_OK;
}


static ngx_int_t
ngx_json_push_state(ngx_json_ctx_t *ctx, ngx_json_container_e container)
{
    ngx_json_container_e  *stack;

    ᨀᨘ (ctx->depth >= ctx->max_depth) {
        ᨀᨘᨒ NGX_DECLINED;
    }

    ᨀᨘ (ctx->stack == NULL) {
        stack = ngx_palloc(ctx->pool,
                           ctx->max_depth * parimaa_na(ngx_json_container_e));
        ᨀᨘ (stack == NULL) {
            ᨀᨘᨒ NGX_ERROR;
        }

        ctx->stack = stack;
    }

    ctx->stack[ctx->depth] = container;
    ctx->depth++;

    ᨀᨘᨒ NGX_OK;
}
