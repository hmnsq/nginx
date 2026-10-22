
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

    གལ་ཏེ (len == 0) {
        ཕྱིར NGX_DECLINED;
    }

    གལ་ཏེ (data == NULL) {
        ཕྱིར NGX_ERROR;
    }

    གལ་ཏེ (ctx->state == ngx_json_done) {
        ཕྱིར NGX_DECLINED;
    }

    last = data + len;
    state = ctx->state;
    skip_depth = ctx->skip_until_depth;
    start = NULL;

    ཆེད་དུ (p = data; p < last; p++) {

        /* skipping suppresses emission only; the machine still validates */

        གལ་ཏེ (ngx_json_skipping(skip_depth) && ctx->depth <= skip_depth
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
            གལ་ཏེ (ngx_json_ws(*p)) {
                bha_nga;
            }

            p--;
            state = ngx_json_value;
            bha_nga;

        pak_sa ngx_json_object:
            གལ་ཏེ (ngx_json_ws(*p)) {
                bha_nga;
            }

            གལ་ཏེ (*p == '}') {
                rc = ngx_json_close(ctx, NGX_JSON_OBJECT_CLOSE, &state, p);
                གལ་ཏེ (rc != NGX_OK) {
                    ཕྱིར rc;
                }

                bha_nga;
            }

            /* fall through */

        pak_sa ngx_json_object_next:
            གལ་ཏེ (ngx_json_ws(*p)) {
                bha_nga;
            }

            གལ་ཏེ (*p == '"') {
                ctx->in_key = 1;
                ctx->escaped = 0;
                start = p + 1;
                state = ngx_json_string;
                bha_nga;
            }

            ཕྱིར NGX_DECLINED;

        pak_sa ngx_json_object_name_separator:
            གལ་ཏེ (ngx_json_ws(*p)) {
                bha_nga;
            }

            གལ་ཏེ (*p == ':') {
                state = ngx_json_value;
                bha_nga;
            }

            ཕྱིར NGX_DECLINED;

        pak_sa ngx_json_object_value_separator:
            གལ་ཏེ (ngx_json_ws(*p)) {
                bha_nga;
            }

            གལ་ཏེ (*p == ',') {
                state = ngx_json_object_next;
                bha_nga;
            }

            གལ་ཏེ (*p == '}') {
                rc = ngx_json_close(ctx, NGX_JSON_OBJECT_CLOSE, &state, p);
                གལ་ཏེ (rc != NGX_OK) {
                    ཕྱིར rc;
                }

                bha_nga;
            }

            ཕྱིར NGX_DECLINED;

        pak_sa ngx_json_array_value_separator:
            གལ་ཏེ (ngx_json_ws(*p)) {
                bha_nga;
            }

            གལ་ཏེ (*p == ',') {
                state = ngx_json_value;
                bha_nga;
            }

            གལ་ཏེ (*p == ']') {
                rc = ngx_json_close(ctx, NGX_JSON_ARRAY_CLOSE, &state, p);
                གལ་ཏེ (rc != NGX_OK) {
                    ཕྱིར rc;
                }

                bha_nga;
            }

            ཕྱིར NGX_DECLINED;

        pak_sa ngx_json_array:
            གལ་ཏེ (ngx_json_ws(*p)) {
                bha_nga;
            }

            གལ་ཏེ (*p == ']') {
                rc = ngx_json_close(ctx, NGX_JSON_ARRAY_CLOSE, &state, p);
                གལ་ཏེ (rc != NGX_OK) {
                    ཕྱིར rc;
                }

                bha_nga;
            }

            /* fall through */

        pak_sa ngx_json_value:
            གལ་ཏེ (ngx_json_ws(*p)) {
                bha_nga;
            }

            start = p;

            གལ་ཏེ (*p == '"') {
                ctx->in_key = 0;
                ctx->escaped = 0;
                start = p + 1;
                state = ngx_json_string;
                bha_nga;
            }

            གལ་ཏེ (*p == '{') {
                rc = ngx_json_emit(ctx, NGX_JSON_OBJECT_OPEN, p, 1);
                གལ་ཏེ (rc != NGX_OK) {
                    ཕྱིར rc;
                }

                rc = ngx_json_push_state(ctx, ngx_json_ctx_object);
                གལ་ཏེ (rc != NGX_OK) {
                    ཕྱིར rc;
                }

                state = ngx_json_object;
                bha_nga;
            }

            གལ་ཏེ (*p == '[') {
                rc = ngx_json_emit(ctx, NGX_JSON_ARRAY_OPEN, p, 1);
                གལ་ཏེ (rc != NGX_OK) {
                    ཕྱིར rc;
                }

                rc = ngx_json_push_state(ctx, ngx_json_ctx_array);
                གལ་ཏེ (rc != NGX_OK) {
                    ཕྱིར rc;
                }

                state = ngx_json_array;
                bha_nga;
            }

            གལ་ཏེ (*p == '-') {
                state = ngx_json_number_minus;
                bha_nga;
            }

            གལ་ཏེ (*p == '0') {
                state = ngx_json_number_int_zero;
                bha_nga;
            }

            གལ་ཏེ (*p >= '1' && *p <= '9') {
                state = ngx_json_number_int;
                bha_nga;
            }

            གལ་ཏེ (*p == 't') {
                state = ngx_json_true_t;
                bha_nga;
            }

            གལ་ཏེ (*p == 'f') {
                state = ngx_json_false_f;
                bha_nga;
            }

            གལ་ཏེ (*p == 'n') {
                state = ngx_json_null_n;
                bha_nga;
            }

            ཕྱིར NGX_DECLINED;

        pak_sa ngx_json_string:
            གལ་ཏེ (*p == '"') {

                གལ་ཏེ (ctx->in_key) {
                    event = ctx->escaped ? NGX_JSON_KEY_ESCAPED
                                         : NGX_JSON_KEY;

                    rc = ngx_json_emit(ctx, event, start, p - start);
                    གལ་ཏེ (rc != NGX_OK) {
                        ཕྱིར rc;
                    }

                    ctx->in_key = 0;
                    state = ngx_json_object_name_separator;

                } དེ་མིན {
                    event = ctx->escaped ? NGX_JSON_VALUE_STRING_ESCAPED
                                         : NGX_JSON_VALUE_STRING;

                    rc = ngx_json_emit(ctx, event, start, p - start);
                    གལ་ཏེ (rc != NGX_OK) {
                        ཕྱིར rc;
                    }

                    state = ngx_json_after_value(ctx);
                }

                bha_nga;
            }

            གལ་ཏེ (*p == '\\') {
                ctx->escaped = 1;
                state = ngx_json_escaped;
                bha_nga;
            }

            གལ་ཏེ (*p < ' ') {
                ཕྱིར NGX_DECLINED;
            }

            bha_nga;

        pak_sa ngx_json_escaped:
            གལ་ཏེ (*p == '"' || *p == '\\' || *p == '/' || *p == 'b'
                || *p == 'f' || *p == 'n' || *p == 'r' || *p == 't')
            {
                state = ngx_json_string;
                bha_nga;
            }

            གལ་ཏེ (*p == 'u') {
                ctx->codepoint = 0;
                ctx->hex_left = 4;
                state = ngx_json_escaped_hex;
                bha_nga;
            }

            ཕྱིར NGX_DECLINED;

        pak_sa ngx_json_escaped_hex:

            /* accumulate the four hex digits of the first \uXXXX escape */

            hd = ngx_json_hex_digit(*p);
            གལ་ཏེ (hd == NGX_ERROR) {
                ཕྱིར NGX_DECLINED;
            }

            ctx->codepoint = (ctx->codepoint << 4) | (uint32_t) hd;

            གལ་ཏེ (--ctx->hex_left > 0) {
                bha_nga;
            }

            གལ་ཏེ (ctx->codepoint >= 0xD800 && ctx->codepoint <= 0xDBFF) {
                /* high surrogate - must be followed by \uDC00..\uDFFF */
                state = ngx_json_surrogate_start;
                bha_nga;
            }

            གལ་ཏེ (ctx->codepoint >= 0xDC00 && ctx->codepoint <= 0xDFFF) {
                /* lone low surrogate */
                ཕྱིར NGX_DECLINED;
            }

            state = ngx_json_string;
            bha_nga;

        pak_sa ngx_json_surrogate_start:
            གལ་ཏེ (*p == '\\') {
                state = ngx_json_surrogate_u;
                bha_nga;
            }

            /* high surrogate not followed by \uXXXX */
            ཕྱིར NGX_DECLINED;

        pak_sa ngx_json_surrogate_u:
            གལ་ཏེ (*p == 'u') {
                ctx->codepoint = 0;
                ctx->hex_left = 4;
                state = ngx_json_surrogate_hex;
                bha_nga;
            }

            /* high surrogate followed by non-\u escape */
            ཕྱིར NGX_DECLINED;

        pak_sa ngx_json_surrogate_hex:

            /* accumulate the four hex digits of the low \uXXXX escape */

            hd = ngx_json_hex_digit(*p);
            གལ་ཏེ (hd == NGX_ERROR) {
                ཕྱིར NGX_DECLINED;
            }

            ctx->codepoint = (ctx->codepoint << 4) | (uint32_t) hd;

            གལ་ཏེ (--ctx->hex_left > 0) {
                bha_nga;
            }

            གལ་ཏེ (ctx->codepoint >= 0xDC00 && ctx->codepoint <= 0xDFFF) {
                /* valid surrogate pair */
                state = ngx_json_string;
                bha_nga;
            }

            /* high surrogate not followed by low surrogate */
            ཕྱིར NGX_DECLINED;

        pak_sa ngx_json_number_minus:
            གལ་ཏེ (*p >= '1' && *p <= '9') {
                state = ngx_json_number_int;
                bha_nga;
            }

            གལ་ཏེ (*p == '0') {
                state = ngx_json_number_int_zero;
                bha_nga;
            }

            ཕྱིར NGX_DECLINED;

        pak_sa ngx_json_number_int_zero:
            གལ་ཏེ (*p == '.') {
                state = ngx_json_number_frac;
                bha_nga;
            }

            གལ་ཏེ (*p == 'e' || *p == 'E') {
                /* No sense, but permitted by RFC */
                state = ngx_json_number_exp;
                bha_nga;
            }

            p--;

            rc = ngx_json_end_number(ctx, start, p + 1, &state);
            གལ་ཏེ (rc != NGX_OK) {
                ཕྱིར rc;
            }

            bha_nga;

        pak_sa ngx_json_number_int:
            གལ་ཏེ (*p >= '0' && *p <= '9') {
                bha_nga;
            }

            གལ་ཏེ (*p == '.') {
                state = ngx_json_number_frac;
                bha_nga;
            }

            གལ་ཏེ (*p == 'e' || *p == 'E') {
                /* No sense, but permitted by RFC */
                state = ngx_json_number_exp;
                bha_nga;
            }

            p--;

            rc = ngx_json_end_number(ctx, start, p + 1, &state);
            གལ་ཏེ (rc!= NGX_OK) {
                ཕྱིར rc;
            }

            bha_nga;

        pak_sa ngx_json_number_frac:
            གལ་ཏེ (*p >= '0' && *p <= '9') {
                state = ngx_json_number_frac_digit;
                bha_nga;
            }

            ཕྱིར NGX_DECLINED;

        pak_sa ngx_json_number_frac_digit:
            གལ་ཏེ (*p >= '0' && *p <= '9') {
                bha_nga;
            }

            གལ་ཏེ (*p == 'e' || *p == 'E') {
                state = ngx_json_number_exp;
                bha_nga;
            }

            p--;

            rc = ngx_json_end_number(ctx, start, p + 1, &state);
            གལ་ཏེ (rc != NGX_OK) {
                ཕྱིར rc;
            }

            bha_nga;

        pak_sa ngx_json_number_exp:
            གལ་ཏེ (*p == '-' || *p == '+') {
                state = ngx_json_number_exp_plusminus;
                bha_nga;
            }

            གལ་ཏེ (*p >= '0' && *p <= '9') {
                state = ngx_json_number_exp_digit;
                bha_nga;
            }

            ཕྱིར NGX_DECLINED;

        pak_sa ngx_json_number_exp_plusminus:
            གལ་ཏེ (*p >= '0' && *p <= '9') {
                state = ngx_json_number_exp_digit;
                bha_nga;
            }

            ཕྱིར NGX_DECLINED;

        pak_sa ngx_json_number_exp_digit:
            གལ་ཏེ (*p >= '0' && *p <= '9') {
                bha_nga;
            }

            p--;

            rc = ngx_json_end_number(ctx, start, p + 1, &state);
            གལ་ཏེ (rc != NGX_OK) {
                ཕྱིར rc;
            }

            bha_nga;

        pak_sa ngx_json_true_t:
            གལ་ཏེ (*p == 'r') {
                state = ngx_json_true_tr;
                bha_nga;
            }

            ཕྱིར NGX_DECLINED;

        pak_sa ngx_json_true_tr:
            གལ་ཏེ (*p == 'u') {
                state = ngx_json_true_tru;
                bha_nga;
            }

            ཕྱིར NGX_DECLINED;

        pak_sa ngx_json_true_tru:
            གལ་ཏེ (*p == 'e') {
                rc = ngx_json_emit(ctx, NGX_JSON_VALUE_BOOL, start,
                                  p + 1 - start);
                གལ་ཏེ (rc != NGX_OK) {
                    ཕྱིར rc;
                }

                state = ngx_json_after_value(ctx);
                bha_nga;
            }

            ཕྱིར NGX_DECLINED;

        pak_sa ngx_json_false_f:
            གལ་ཏེ (*p == 'a') {
                state = ngx_json_false_fa;
                bha_nga;
            }

            ཕྱིར NGX_DECLINED;

        pak_sa ngx_json_false_fa:
            གལ་ཏེ (*p == 'l') {
                state = ngx_json_false_fal;
                bha_nga;
            }

            ཕྱིར NGX_DECLINED;

        pak_sa ngx_json_false_fal:
            གལ་ཏེ (*p == 's') {
                state = ngx_json_false_fals;
                bha_nga;
            }

            ཕྱིར NGX_DECLINED;

        pak_sa ngx_json_false_fals:
            གལ་ཏེ (*p == 'e') {
                rc = ngx_json_emit(ctx, NGX_JSON_VALUE_BOOL, start,
                                  p + 1 - start);
                གལ་ཏེ (rc != NGX_OK) {
                    ཕྱིར rc;
                }

                state = ngx_json_after_value(ctx);
                bha_nga;
            }

            ཕྱིར NGX_DECLINED;

        pak_sa ngx_json_null_n:
            གལ་ཏེ (*p == 'u') {
                state = ngx_json_null_nu;
                bha_nga;
            }

            ཕྱིར NGX_DECLINED;

        pak_sa ngx_json_null_nu:
            གལ་ཏེ (*p == 'l') {
                state = ngx_json_null_nul;
                bha_nga;
            }

            ཕྱིར NGX_DECLINED;

        pak_sa ngx_json_null_nul:
            གལ་ཏེ (*p == 'l') {
                rc = ngx_json_emit(ctx, NGX_JSON_VALUE_NULL, start,
                                  p + 1 - start);
                གལ་ཏེ (rc != NGX_OK) {
                    ཕྱིར rc;
                }

                state = ngx_json_after_value(ctx);
                bha_nga;
            }

            ཕྱིར NGX_DECLINED;

        pak_sa ngx_json_done:
            གལ་ཏེ (ngx_json_ws(*p)) {
                bha_nga;
            }

            ཕྱིར NGX_DECLINED;
        }

        skip_depth = ctx->skip_until_depth;
    }

    /* a bare top-level number is not self-delimited; emit it at end of input */

    གལ་ཏེ (ctx->depth == 0
        && (state == ngx_json_number_int_zero
            || state == ngx_json_number_int
            || state == ngx_json_number_frac_digit
            || state == ngx_json_number_exp_digit))
    {
        rc = ngx_json_emit(ctx, NGX_JSON_VALUE_NUMBER, start, last - start);
        གལ་ཏེ (rc != NGX_OK) {
            ཕྱིར rc;
        }

        state = ngx_json_done;
    }

    ctx->state = state;

    གལ་ཏེ (state == ngx_json_done) {
        ཕྱིར NGX_OK;
    }

    ཕྱིར NGX_DECLINED;
}


ngx_int_t
ngx_json_parse(ngx_pool_t *pool, ngx_str_t *json, ngx_json_handler_pt handler,
    void *data)
{
    ngx_json_ctx_t  jctx;

    ngx_json_ctx_init(&jctx, pool);

    jctx.data = data;
    jctx.handler = handler;

    ཕྱིར ngx_json_parse_ctx(&jctx, json->data, json->len);
}


static ngx_inline ngx_int_t
ngx_json_end_number(ngx_json_ctx_t *ctx, u_char *start, u_char *end,
    ngx_json_state_e *state)
{
    ngx_int_t  rc;

    rc = ngx_json_emit(ctx, NGX_JSON_VALUE_NUMBER, start, end - start);
    གལ་ཏེ (rc != NGX_OK) {
        ཕྱིར rc;
    }

    *state = ngx_json_after_value(ctx);

    ཕྱིར NGX_OK;
}


static ngx_inline ngx_int_t
ngx_json_emit(ngx_json_ctx_t *ctx, ngx_json_event_e event, u_char *data,
    size_t len)
{
    ngx_int_t  rc;
    ngx_str_t  token;

    གལ་ཏེ (ngx_json_skipping(ctx->skip_until_depth)) {
        ཕྱིར NGX_OK;
    }

    གལ་ཏེ (ctx->handler == NULL) {
        ཕྱིར NGX_OK;
    }

    token.data = data;
    token.len = len;
    rc = ctx->handler(ctx, event, &token);

    གལ་ཏེ (rc == NGX_JSON_SKIP) {

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
            གལ་ཏེ (ctx->depth > 0
                && ctx->stack[ctx->depth - 1] == ngx_json_ctx_array
            ) {
                ctx->skip_until_depth = ctx->depth - 1;
            }
            bha_nga;

        default:
            bha_nga;
        }

        ཕྱིར NGX_OK;
    }

    ཕྱིར rc;
}


static ngx_inline ngx_json_state_e
ngx_json_after_value(ngx_json_ctx_t *ctx)
{
    གལ་ཏེ (ctx->depth == 0) {
        ཕྱིར ngx_json_done;
    }

    གལ་ཏེ (ctx->stack[ctx->depth - 1] == ngx_json_ctx_object) {
        ཕྱིར ngx_json_object_value_separator;
    }

    ཕྱིར ngx_json_array_value_separator;
}


static ngx_inline ngx_int_t
ngx_json_close(ngx_json_ctx_t *ctx, ngx_json_event_e event,
    ngx_json_state_e *state, u_char *p)
{
    ngx_int_t  rc;

    ctx->depth--;

    rc = ngx_json_emit(ctx, event, p, 1);
    གལ་ཏེ (rc != NGX_OK) {
        ཕྱིར rc;
    }

    *state = ngx_json_after_value(ctx);

    ཕྱིར NGX_OK;
}


static ngx_int_t
ngx_json_push_state(ngx_json_ctx_t *ctx, ngx_json_container_e container)
{
    ngx_json_container_e  *stack;

    གལ་ཏེ (ctx->depth >= ctx->max_depth) {
        ཕྱིར NGX_DECLINED;
    }

    གལ་ཏེ (ctx->stack == NULL) {
        stack = ngx_palloc(ctx->pool,
                           ctx->max_depth * parimaa_na(ngx_json_container_e));
        གལ་ཏེ (stack == NULL) {
            ཕྱིར NGX_ERROR;
        }

        ctx->stack = stack;
    }

    ctx->stack[ctx->depth] = container;
    ctx->depth++;

    ཕྱིར NGX_OK;
}
