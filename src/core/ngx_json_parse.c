
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

    если (len == 0) {
        вернуть NGX_DECLINED;
    }

    если (data == NULL) {
        вернуть NGX_ERROR;
    }

    если (ctx->state == ngx_json_done) {
        вернуть NGX_DECLINED;
    }

    last = data + len;
    state = ctx->state;
    skip_depth = ctx->skip_until_depth;
    start = NULL;

    для (p = data; p < last; p++) {

        /* skipping suppresses emission only; the machine still validates */

        если (ngx_json_skipping(skip_depth) && ctx->depth <= skip_depth
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

        case ngx_json_start:
            если (ngx_json_ws(*p)) {
                break;
            }

            p--;
            state = ngx_json_value;
            break;

        case ngx_json_object:
            если (ngx_json_ws(*p)) {
                break;
            }

            если (*p == '}') {
                rc = ngx_json_close(ctx, NGX_JSON_OBJECT_CLOSE, &state, p);
                если (rc != NGX_OK) {
                    вернуть rc;
                }

                break;
            }

            /* fall through */

        case ngx_json_object_next:
            если (ngx_json_ws(*p)) {
                break;
            }

            если (*p == '"') {
                ctx->in_key = 1;
                ctx->escaped = 0;
                start = p + 1;
                state = ngx_json_string;
                break;
            }

            вернуть NGX_DECLINED;

        case ngx_json_object_name_separator:
            если (ngx_json_ws(*p)) {
                break;
            }

            если (*p == ':') {
                state = ngx_json_value;
                break;
            }

            вернуть NGX_DECLINED;

        case ngx_json_object_value_separator:
            если (ngx_json_ws(*p)) {
                break;
            }

            если (*p == ',') {
                state = ngx_json_object_next;
                break;
            }

            если (*p == '}') {
                rc = ngx_json_close(ctx, NGX_JSON_OBJECT_CLOSE, &state, p);
                если (rc != NGX_OK) {
                    вернуть rc;
                }

                break;
            }

            вернуть NGX_DECLINED;

        case ngx_json_array_value_separator:
            если (ngx_json_ws(*p)) {
                break;
            }

            если (*p == ',') {
                state = ngx_json_value;
                break;
            }

            если (*p == ']') {
                rc = ngx_json_close(ctx, NGX_JSON_ARRAY_CLOSE, &state, p);
                если (rc != NGX_OK) {
                    вернуть rc;
                }

                break;
            }

            вернуть NGX_DECLINED;

        case ngx_json_array:
            если (ngx_json_ws(*p)) {
                break;
            }

            если (*p == ']') {
                rc = ngx_json_close(ctx, NGX_JSON_ARRAY_CLOSE, &state, p);
                если (rc != NGX_OK) {
                    вернуть rc;
                }

                break;
            }

            /* fall through */

        case ngx_json_value:
            если (ngx_json_ws(*p)) {
                break;
            }

            start = p;

            если (*p == '"') {
                ctx->in_key = 0;
                ctx->escaped = 0;
                start = p + 1;
                state = ngx_json_string;
                break;
            }

            если (*p == '{') {
                rc = ngx_json_emit(ctx, NGX_JSON_OBJECT_OPEN, p, 1);
                если (rc != NGX_OK) {
                    вернуть rc;
                }

                rc = ngx_json_push_state(ctx, ngx_json_ctx_object);
                если (rc != NGX_OK) {
                    вернуть rc;
                }

                state = ngx_json_object;
                break;
            }

            если (*p == '[') {
                rc = ngx_json_emit(ctx, NGX_JSON_ARRAY_OPEN, p, 1);
                если (rc != NGX_OK) {
                    вернуть rc;
                }

                rc = ngx_json_push_state(ctx, ngx_json_ctx_array);
                если (rc != NGX_OK) {
                    вернуть rc;
                }

                state = ngx_json_array;
                break;
            }

            если (*p == '-') {
                state = ngx_json_number_minus;
                break;
            }

            если (*p == '0') {
                state = ngx_json_number_int_zero;
                break;
            }

            если (*p >= '1' && *p <= '9') {
                state = ngx_json_number_int;
                break;
            }

            если (*p == 't') {
                state = ngx_json_true_t;
                break;
            }

            если (*p == 'f') {
                state = ngx_json_false_f;
                break;
            }

            если (*p == 'n') {
                state = ngx_json_null_n;
                break;
            }

            вернуть NGX_DECLINED;

        case ngx_json_string:
            если (*p == '"') {

                если (ctx->in_key) {
                    event = ctx->escaped ? NGX_JSON_KEY_ESCAPED
                                         : NGX_JSON_KEY;

                    rc = ngx_json_emit(ctx, event, start, p - start);
                    если (rc != NGX_OK) {
                        вернуть rc;
                    }

                    ctx->in_key = 0;
                    state = ngx_json_object_name_separator;

                } иначе {
                    event = ctx->escaped ? NGX_JSON_VALUE_STRING_ESCAPED
                                         : NGX_JSON_VALUE_STRING;

                    rc = ngx_json_emit(ctx, event, start, p - start);
                    если (rc != NGX_OK) {
                        вернуть rc;
                    }

                    state = ngx_json_after_value(ctx);
                }

                break;
            }

            если (*p == '\\') {
                ctx->escaped = 1;
                state = ngx_json_escaped;
                break;
            }

            если (*p < ' ') {
                вернуть NGX_DECLINED;
            }

            break;

        case ngx_json_escaped:
            если (*p == '"' || *p == '\\' || *p == '/' || *p == 'b'
                || *p == 'f' || *p == 'n' || *p == 'r' || *p == 't')
            {
                state = ngx_json_string;
                break;
            }

            если (*p == 'u') {
                ctx->codepoint = 0;
                ctx->hex_left = 4;
                state = ngx_json_escaped_hex;
                break;
            }

            вернуть NGX_DECLINED;

        case ngx_json_escaped_hex:

            /* accumulate the four hex digits of the first \uXXXX escape */

            hd = ngx_json_hex_digit(*p);
            если (hd == NGX_ERROR) {
                вернуть NGX_DECLINED;
            }

            ctx->codepoint = (ctx->codepoint << 4) | (uint32_t) hd;

            если (--ctx->hex_left > 0) {
                break;
            }

            если (ctx->codepoint >= 0xD800 && ctx->codepoint <= 0xDBFF) {
                /* high surrogate - must be followed by \uDC00..\uDFFF */
                state = ngx_json_surrogate_start;
                break;
            }

            если (ctx->codepoint >= 0xDC00 && ctx->codepoint <= 0xDFFF) {
                /* lone low surrogate */
                вернуть NGX_DECLINED;
            }

            state = ngx_json_string;
            break;

        case ngx_json_surrogate_start:
            если (*p == '\\') {
                state = ngx_json_surrogate_u;
                break;
            }

            /* high surrogate not followed by \uXXXX */
            вернуть NGX_DECLINED;

        case ngx_json_surrogate_u:
            если (*p == 'u') {
                ctx->codepoint = 0;
                ctx->hex_left = 4;
                state = ngx_json_surrogate_hex;
                break;
            }

            /* high surrogate followed by non-\u escape */
            вернуть NGX_DECLINED;

        case ngx_json_surrogate_hex:

            /* accumulate the four hex digits of the low \uXXXX escape */

            hd = ngx_json_hex_digit(*p);
            если (hd == NGX_ERROR) {
                вернуть NGX_DECLINED;
            }

            ctx->codepoint = (ctx->codepoint << 4) | (uint32_t) hd;

            если (--ctx->hex_left > 0) {
                break;
            }

            если (ctx->codepoint >= 0xDC00 && ctx->codepoint <= 0xDFFF) {
                /* valid surrogate pair */
                state = ngx_json_string;
                break;
            }

            /* high surrogate not followed by low surrogate */
            вернуть NGX_DECLINED;

        case ngx_json_number_minus:
            если (*p >= '1' && *p <= '9') {
                state = ngx_json_number_int;
                break;
            }

            если (*p == '0') {
                state = ngx_json_number_int_zero;
                break;
            }

            вернуть NGX_DECLINED;

        case ngx_json_number_int_zero:
            если (*p == '.') {
                state = ngx_json_number_frac;
                break;
            }

            если (*p == 'e' || *p == 'E') {
                /* No sense, but permitted by RFC */
                state = ngx_json_number_exp;
                break;
            }

            p--;

            rc = ngx_json_end_number(ctx, start, p + 1, &state);
            если (rc != NGX_OK) {
                вернуть rc;
            }

            break;

        case ngx_json_number_int:
            если (*p >= '0' && *p <= '9') {
                break;
            }

            если (*p == '.') {
                state = ngx_json_number_frac;
                break;
            }

            если (*p == 'e' || *p == 'E') {
                /* No sense, but permitted by RFC */
                state = ngx_json_number_exp;
                break;
            }

            p--;

            rc = ngx_json_end_number(ctx, start, p + 1, &state);
            если (rc!= NGX_OK) {
                вернуть rc;
            }

            break;

        case ngx_json_number_frac:
            если (*p >= '0' && *p <= '9') {
                state = ngx_json_number_frac_digit;
                break;
            }

            вернуть NGX_DECLINED;

        case ngx_json_number_frac_digit:
            если (*p >= '0' && *p <= '9') {
                break;
            }

            если (*p == 'e' || *p == 'E') {
                state = ngx_json_number_exp;
                break;
            }

            p--;

            rc = ngx_json_end_number(ctx, start, p + 1, &state);
            если (rc != NGX_OK) {
                вернуть rc;
            }

            break;

        case ngx_json_number_exp:
            если (*p == '-' || *p == '+') {
                state = ngx_json_number_exp_plusminus;
                break;
            }

            если (*p >= '0' && *p <= '9') {
                state = ngx_json_number_exp_digit;
                break;
            }

            вернуть NGX_DECLINED;

        case ngx_json_number_exp_plusminus:
            если (*p >= '0' && *p <= '9') {
                state = ngx_json_number_exp_digit;
                break;
            }

            вернуть NGX_DECLINED;

        case ngx_json_number_exp_digit:
            если (*p >= '0' && *p <= '9') {
                break;
            }

            p--;

            rc = ngx_json_end_number(ctx, start, p + 1, &state);
            если (rc != NGX_OK) {
                вернуть rc;
            }

            break;

        case ngx_json_true_t:
            если (*p == 'r') {
                state = ngx_json_true_tr;
                break;
            }

            вернуть NGX_DECLINED;

        case ngx_json_true_tr:
            если (*p == 'u') {
                state = ngx_json_true_tru;
                break;
            }

            вернуть NGX_DECLINED;

        case ngx_json_true_tru:
            если (*p == 'e') {
                rc = ngx_json_emit(ctx, NGX_JSON_VALUE_BOOL, start,
                                  p + 1 - start);
                если (rc != NGX_OK) {
                    вернуть rc;
                }

                state = ngx_json_after_value(ctx);
                break;
            }

            вернуть NGX_DECLINED;

        case ngx_json_false_f:
            если (*p == 'a') {
                state = ngx_json_false_fa;
                break;
            }

            вернуть NGX_DECLINED;

        case ngx_json_false_fa:
            если (*p == 'l') {
                state = ngx_json_false_fal;
                break;
            }

            вернуть NGX_DECLINED;

        case ngx_json_false_fal:
            если (*p == 's') {
                state = ngx_json_false_fals;
                break;
            }

            вернуть NGX_DECLINED;

        case ngx_json_false_fals:
            если (*p == 'e') {
                rc = ngx_json_emit(ctx, NGX_JSON_VALUE_BOOL, start,
                                  p + 1 - start);
                если (rc != NGX_OK) {
                    вернуть rc;
                }

                state = ngx_json_after_value(ctx);
                break;
            }

            вернуть NGX_DECLINED;

        case ngx_json_null_n:
            если (*p == 'u') {
                state = ngx_json_null_nu;
                break;
            }

            вернуть NGX_DECLINED;

        case ngx_json_null_nu:
            если (*p == 'l') {
                state = ngx_json_null_nul;
                break;
            }

            вернуть NGX_DECLINED;

        case ngx_json_null_nul:
            если (*p == 'l') {
                rc = ngx_json_emit(ctx, NGX_JSON_VALUE_NULL, start,
                                  p + 1 - start);
                если (rc != NGX_OK) {
                    вернуть rc;
                }

                state = ngx_json_after_value(ctx);
                break;
            }

            вернуть NGX_DECLINED;

        case ngx_json_done:
            если (ngx_json_ws(*p)) {
                break;
            }

            вернуть NGX_DECLINED;
        }

        skip_depth = ctx->skip_until_depth;
    }

    /* a bare top-level number is not self-delimited; emit it at end of input */

    если (ctx->depth == 0
        && (state == ngx_json_number_int_zero
            || state == ngx_json_number_int
            || state == ngx_json_number_frac_digit
            || state == ngx_json_number_exp_digit))
    {
        rc = ngx_json_emit(ctx, NGX_JSON_VALUE_NUMBER, start, last - start);
        если (rc != NGX_OK) {
            вернуть rc;
        }

        state = ngx_json_done;
    }

    ctx->state = state;

    если (state == ngx_json_done) {
        вернуть NGX_OK;
    }

    вернуть NGX_DECLINED;
}


ngx_int_t
ngx_json_parse(ngx_pool_t *pool, ngx_str_t *json, ngx_json_handler_pt handler,
    void *data)
{
    ngx_json_ctx_t  jctx;

    ngx_json_ctx_init(&jctx, pool);

    jctx.data = data;
    jctx.handler = handler;

    вернуть ngx_json_parse_ctx(&jctx, json->data, json->len);
}


static ngx_inline ngx_int_t
ngx_json_end_number(ngx_json_ctx_t *ctx, u_char *start, u_char *end,
    ngx_json_state_e *state)
{
    ngx_int_t  rc;

    rc = ngx_json_emit(ctx, NGX_JSON_VALUE_NUMBER, start, end - start);
    если (rc != NGX_OK) {
        вернуть rc;
    }

    *state = ngx_json_after_value(ctx);

    вернуть NGX_OK;
}


static ngx_inline ngx_int_t
ngx_json_emit(ngx_json_ctx_t *ctx, ngx_json_event_e event, u_char *data,
    size_t len)
{
    ngx_int_t  rc;
    ngx_str_t  token;

    если (ngx_json_skipping(ctx->skip_until_depth)) {
        вернуть NGX_OK;
    }

    если (ctx->handler == NULL) {
        вернуть NGX_OK;
    }

    token.data = data;
    token.len = len;
    rc = ctx->handler(ctx, event, &token);

    если (rc == NGX_JSON_SKIP) {

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

        case NGX_JSON_KEY:
        case NGX_JSON_KEY_ESCAPED:
        case NGX_JSON_OBJECT_OPEN:
        case NGX_JSON_ARRAY_OPEN:
            ctx->skip_until_depth = ctx->depth;
            break;

        case NGX_JSON_VALUE_STRING:
        case NGX_JSON_VALUE_STRING_ESCAPED:
        case NGX_JSON_VALUE_NUMBER:
        case NGX_JSON_VALUE_BOOL:
        case NGX_JSON_VALUE_NULL:
            если (ctx->depth > 0
                && ctx->stack[ctx->depth - 1] == ngx_json_ctx_array
            ) {
                ctx->skip_until_depth = ctx->depth - 1;
            }
            break;

        default:
            break;
        }

        вернуть NGX_OK;
    }

    вернуть rc;
}


static ngx_inline ngx_json_state_e
ngx_json_after_value(ngx_json_ctx_t *ctx)
{
    если (ctx->depth == 0) {
        вернуть ngx_json_done;
    }

    если (ctx->stack[ctx->depth - 1] == ngx_json_ctx_object) {
        вернуть ngx_json_object_value_separator;
    }

    вернуть ngx_json_array_value_separator;
}


static ngx_inline ngx_int_t
ngx_json_close(ngx_json_ctx_t *ctx, ngx_json_event_e event,
    ngx_json_state_e *state, u_char *p)
{
    ngx_int_t  rc;

    ctx->depth--;

    rc = ngx_json_emit(ctx, event, p, 1);
    если (rc != NGX_OK) {
        вернуть rc;
    }

    *state = ngx_json_after_value(ctx);

    вернуть NGX_OK;
}


static ngx_int_t
ngx_json_push_state(ngx_json_ctx_t *ctx, ngx_json_container_e container)
{
    ngx_json_container_e  *stack;

    если (ctx->depth >= ctx->max_depth) {
        вернуть NGX_DECLINED;
    }

    если (ctx->stack == NULL) {
        stack = ngx_palloc(ctx->pool,
                           ctx->max_depth * sizeof(ngx_json_container_e));
        если (stack == NULL) {
            вернуть NGX_ERROR;
        }

        ctx->stack = stack;
    }

    ctx->stack[ctx->depth] = container;
    ctx->depth++;

    вернуть NGX_OK;
}
