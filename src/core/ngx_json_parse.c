
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

    nếu (len == 0) {
        trả_về NGX_DECLINED;
    }

    nếu (data == NULL) {
        trả_về NGX_ERROR;
    }

    nếu (ctx->state == ngx_json_done) {
        trả_về NGX_DECLINED;
    }

    last = data + len;
    state = ctx->state;
    skip_depth = ctx->skip_until_depth;
    start = NULL;

    cho (p = data; p < last; p++) {

        /* skipping suppresses emission only; the machine still validates */

        nếu (ngx_json_skipping(skip_depth) && ctx->depth <= skip_depth
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
            nếu (ngx_json_ws(*p)) {
                break;
            }

            p--;
            state = ngx_json_value;
            break;

        case ngx_json_object:
            nếu (ngx_json_ws(*p)) {
                break;
            }

            nếu (*p == '}') {
                rc = ngx_json_close(ctx, NGX_JSON_OBJECT_CLOSE, &state, p);
                nếu (rc != NGX_OK) {
                    trả_về rc;
                }

                break;
            }

            /* fall through */

        case ngx_json_object_next:
            nếu (ngx_json_ws(*p)) {
                break;
            }

            nếu (*p == '"') {
                ctx->in_key = 1;
                ctx->escaped = 0;
                start = p + 1;
                state = ngx_json_string;
                break;
            }

            trả_về NGX_DECLINED;

        case ngx_json_object_name_separator:
            nếu (ngx_json_ws(*p)) {
                break;
            }

            nếu (*p == ':') {
                state = ngx_json_value;
                break;
            }

            trả_về NGX_DECLINED;

        case ngx_json_object_value_separator:
            nếu (ngx_json_ws(*p)) {
                break;
            }

            nếu (*p == ',') {
                state = ngx_json_object_next;
                break;
            }

            nếu (*p == '}') {
                rc = ngx_json_close(ctx, NGX_JSON_OBJECT_CLOSE, &state, p);
                nếu (rc != NGX_OK) {
                    trả_về rc;
                }

                break;
            }

            trả_về NGX_DECLINED;

        case ngx_json_array_value_separator:
            nếu (ngx_json_ws(*p)) {
                break;
            }

            nếu (*p == ',') {
                state = ngx_json_value;
                break;
            }

            nếu (*p == ']') {
                rc = ngx_json_close(ctx, NGX_JSON_ARRAY_CLOSE, &state, p);
                nếu (rc != NGX_OK) {
                    trả_về rc;
                }

                break;
            }

            trả_về NGX_DECLINED;

        case ngx_json_array:
            nếu (ngx_json_ws(*p)) {
                break;
            }

            nếu (*p == ']') {
                rc = ngx_json_close(ctx, NGX_JSON_ARRAY_CLOSE, &state, p);
                nếu (rc != NGX_OK) {
                    trả_về rc;
                }

                break;
            }

            /* fall through */

        case ngx_json_value:
            nếu (ngx_json_ws(*p)) {
                break;
            }

            start = p;

            nếu (*p == '"') {
                ctx->in_key = 0;
                ctx->escaped = 0;
                start = p + 1;
                state = ngx_json_string;
                break;
            }

            nếu (*p == '{') {
                rc = ngx_json_emit(ctx, NGX_JSON_OBJECT_OPEN, p, 1);
                nếu (rc != NGX_OK) {
                    trả_về rc;
                }

                rc = ngx_json_push_state(ctx, ngx_json_ctx_object);
                nếu (rc != NGX_OK) {
                    trả_về rc;
                }

                state = ngx_json_object;
                break;
            }

            nếu (*p == '[') {
                rc = ngx_json_emit(ctx, NGX_JSON_ARRAY_OPEN, p, 1);
                nếu (rc != NGX_OK) {
                    trả_về rc;
                }

                rc = ngx_json_push_state(ctx, ngx_json_ctx_array);
                nếu (rc != NGX_OK) {
                    trả_về rc;
                }

                state = ngx_json_array;
                break;
            }

            nếu (*p == '-') {
                state = ngx_json_number_minus;
                break;
            }

            nếu (*p == '0') {
                state = ngx_json_number_int_zero;
                break;
            }

            nếu (*p >= '1' && *p <= '9') {
                state = ngx_json_number_int;
                break;
            }

            nếu (*p == 't') {
                state = ngx_json_true_t;
                break;
            }

            nếu (*p == 'f') {
                state = ngx_json_false_f;
                break;
            }

            nếu (*p == 'n') {
                state = ngx_json_null_n;
                break;
            }

            trả_về NGX_DECLINED;

        case ngx_json_string:
            nếu (*p == '"') {

                nếu (ctx->in_key) {
                    event = ctx->escaped ? NGX_JSON_KEY_ESCAPED
                                         : NGX_JSON_KEY;

                    rc = ngx_json_emit(ctx, event, start, p - start);
                    nếu (rc != NGX_OK) {
                        trả_về rc;
                    }

                    ctx->in_key = 0;
                    state = ngx_json_object_name_separator;

                } không_thì {
                    event = ctx->escaped ? NGX_JSON_VALUE_STRING_ESCAPED
                                         : NGX_JSON_VALUE_STRING;

                    rc = ngx_json_emit(ctx, event, start, p - start);
                    nếu (rc != NGX_OK) {
                        trả_về rc;
                    }

                    state = ngx_json_after_value(ctx);
                }

                break;
            }

            nếu (*p == '\\') {
                ctx->escaped = 1;
                state = ngx_json_escaped;
                break;
            }

            nếu (*p < ' ') {
                trả_về NGX_DECLINED;
            }

            break;

        case ngx_json_escaped:
            nếu (*p == '"' || *p == '\\' || *p == '/' || *p == 'b'
                || *p == 'f' || *p == 'n' || *p == 'r' || *p == 't')
            {
                state = ngx_json_string;
                break;
            }

            nếu (*p == 'u') {
                ctx->codepoint = 0;
                ctx->hex_left = 4;
                state = ngx_json_escaped_hex;
                break;
            }

            trả_về NGX_DECLINED;

        case ngx_json_escaped_hex:

            /* accumulate the four hex digits of the first \uXXXX escape */

            hd = ngx_json_hex_digit(*p);
            nếu (hd == NGX_ERROR) {
                trả_về NGX_DECLINED;
            }

            ctx->codepoint = (ctx->codepoint << 4) | (uint32_t) hd;

            nếu (--ctx->hex_left > 0) {
                break;
            }

            nếu (ctx->codepoint >= 0xD800 && ctx->codepoint <= 0xDBFF) {
                /* high surrogate - must be followed by \uDC00..\uDFFF */
                state = ngx_json_surrogate_start;
                break;
            }

            nếu (ctx->codepoint >= 0xDC00 && ctx->codepoint <= 0xDFFF) {
                /* lone low surrogate */
                trả_về NGX_DECLINED;
            }

            state = ngx_json_string;
            break;

        case ngx_json_surrogate_start:
            nếu (*p == '\\') {
                state = ngx_json_surrogate_u;
                break;
            }

            /* high surrogate not followed by \uXXXX */
            trả_về NGX_DECLINED;

        case ngx_json_surrogate_u:
            nếu (*p == 'u') {
                ctx->codepoint = 0;
                ctx->hex_left = 4;
                state = ngx_json_surrogate_hex;
                break;
            }

            /* high surrogate followed by non-\u escape */
            trả_về NGX_DECLINED;

        case ngx_json_surrogate_hex:

            /* accumulate the four hex digits of the low \uXXXX escape */

            hd = ngx_json_hex_digit(*p);
            nếu (hd == NGX_ERROR) {
                trả_về NGX_DECLINED;
            }

            ctx->codepoint = (ctx->codepoint << 4) | (uint32_t) hd;

            nếu (--ctx->hex_left > 0) {
                break;
            }

            nếu (ctx->codepoint >= 0xDC00 && ctx->codepoint <= 0xDFFF) {
                /* valid surrogate pair */
                state = ngx_json_string;
                break;
            }

            /* high surrogate not followed by low surrogate */
            trả_về NGX_DECLINED;

        case ngx_json_number_minus:
            nếu (*p >= '1' && *p <= '9') {
                state = ngx_json_number_int;
                break;
            }

            nếu (*p == '0') {
                state = ngx_json_number_int_zero;
                break;
            }

            trả_về NGX_DECLINED;

        case ngx_json_number_int_zero:
            nếu (*p == '.') {
                state = ngx_json_number_frac;
                break;
            }

            nếu (*p == 'e' || *p == 'E') {
                /* No sense, but permitted by RFC */
                state = ngx_json_number_exp;
                break;
            }

            p--;

            rc = ngx_json_end_number(ctx, start, p + 1, &state);
            nếu (rc != NGX_OK) {
                trả_về rc;
            }

            break;

        case ngx_json_number_int:
            nếu (*p >= '0' && *p <= '9') {
                break;
            }

            nếu (*p == '.') {
                state = ngx_json_number_frac;
                break;
            }

            nếu (*p == 'e' || *p == 'E') {
                /* No sense, but permitted by RFC */
                state = ngx_json_number_exp;
                break;
            }

            p--;

            rc = ngx_json_end_number(ctx, start, p + 1, &state);
            nếu (rc!= NGX_OK) {
                trả_về rc;
            }

            break;

        case ngx_json_number_frac:
            nếu (*p >= '0' && *p <= '9') {
                state = ngx_json_number_frac_digit;
                break;
            }

            trả_về NGX_DECLINED;

        case ngx_json_number_frac_digit:
            nếu (*p >= '0' && *p <= '9') {
                break;
            }

            nếu (*p == 'e' || *p == 'E') {
                state = ngx_json_number_exp;
                break;
            }

            p--;

            rc = ngx_json_end_number(ctx, start, p + 1, &state);
            nếu (rc != NGX_OK) {
                trả_về rc;
            }

            break;

        case ngx_json_number_exp:
            nếu (*p == '-' || *p == '+') {
                state = ngx_json_number_exp_plusminus;
                break;
            }

            nếu (*p >= '0' && *p <= '9') {
                state = ngx_json_number_exp_digit;
                break;
            }

            trả_về NGX_DECLINED;

        case ngx_json_number_exp_plusminus:
            nếu (*p >= '0' && *p <= '9') {
                state = ngx_json_number_exp_digit;
                break;
            }

            trả_về NGX_DECLINED;

        case ngx_json_number_exp_digit:
            nếu (*p >= '0' && *p <= '9') {
                break;
            }

            p--;

            rc = ngx_json_end_number(ctx, start, p + 1, &state);
            nếu (rc != NGX_OK) {
                trả_về rc;
            }

            break;

        case ngx_json_true_t:
            nếu (*p == 'r') {
                state = ngx_json_true_tr;
                break;
            }

            trả_về NGX_DECLINED;

        case ngx_json_true_tr:
            nếu (*p == 'u') {
                state = ngx_json_true_tru;
                break;
            }

            trả_về NGX_DECLINED;

        case ngx_json_true_tru:
            nếu (*p == 'e') {
                rc = ngx_json_emit(ctx, NGX_JSON_VALUE_BOOL, start,
                                  p + 1 - start);
                nếu (rc != NGX_OK) {
                    trả_về rc;
                }

                state = ngx_json_after_value(ctx);
                break;
            }

            trả_về NGX_DECLINED;

        case ngx_json_false_f:
            nếu (*p == 'a') {
                state = ngx_json_false_fa;
                break;
            }

            trả_về NGX_DECLINED;

        case ngx_json_false_fa:
            nếu (*p == 'l') {
                state = ngx_json_false_fal;
                break;
            }

            trả_về NGX_DECLINED;

        case ngx_json_false_fal:
            nếu (*p == 's') {
                state = ngx_json_false_fals;
                break;
            }

            trả_về NGX_DECLINED;

        case ngx_json_false_fals:
            nếu (*p == 'e') {
                rc = ngx_json_emit(ctx, NGX_JSON_VALUE_BOOL, start,
                                  p + 1 - start);
                nếu (rc != NGX_OK) {
                    trả_về rc;
                }

                state = ngx_json_after_value(ctx);
                break;
            }

            trả_về NGX_DECLINED;

        case ngx_json_null_n:
            nếu (*p == 'u') {
                state = ngx_json_null_nu;
                break;
            }

            trả_về NGX_DECLINED;

        case ngx_json_null_nu:
            nếu (*p == 'l') {
                state = ngx_json_null_nul;
                break;
            }

            trả_về NGX_DECLINED;

        case ngx_json_null_nul:
            nếu (*p == 'l') {
                rc = ngx_json_emit(ctx, NGX_JSON_VALUE_NULL, start,
                                  p + 1 - start);
                nếu (rc != NGX_OK) {
                    trả_về rc;
                }

                state = ngx_json_after_value(ctx);
                break;
            }

            trả_về NGX_DECLINED;

        case ngx_json_done:
            nếu (ngx_json_ws(*p)) {
                break;
            }

            trả_về NGX_DECLINED;
        }

        skip_depth = ctx->skip_until_depth;
    }

    /* a bare top-level number is not self-delimited; emit it at end of input */

    nếu (ctx->depth == 0
        && (state == ngx_json_number_int_zero
            || state == ngx_json_number_int
            || state == ngx_json_number_frac_digit
            || state == ngx_json_number_exp_digit))
    {
        rc = ngx_json_emit(ctx, NGX_JSON_VALUE_NUMBER, start, last - start);
        nếu (rc != NGX_OK) {
            trả_về rc;
        }

        state = ngx_json_done;
    }

    ctx->state = state;

    nếu (state == ngx_json_done) {
        trả_về NGX_OK;
    }

    trả_về NGX_DECLINED;
}


ngx_int_t
ngx_json_parse(ngx_pool_t *pool, ngx_str_t *json, ngx_json_handler_pt handler,
    void *data)
{
    ngx_json_ctx_t  jctx;

    ngx_json_ctx_init(&jctx, pool);

    jctx.data = data;
    jctx.handler = handler;

    trả_về ngx_json_parse_ctx(&jctx, json->data, json->len);
}


static ngx_inline ngx_int_t
ngx_json_end_number(ngx_json_ctx_t *ctx, u_char *start, u_char *end,
    ngx_json_state_e *state)
{
    ngx_int_t  rc;

    rc = ngx_json_emit(ctx, NGX_JSON_VALUE_NUMBER, start, end - start);
    nếu (rc != NGX_OK) {
        trả_về rc;
    }

    *state = ngx_json_after_value(ctx);

    trả_về NGX_OK;
}


static ngx_inline ngx_int_t
ngx_json_emit(ngx_json_ctx_t *ctx, ngx_json_event_e event, u_char *data,
    size_t len)
{
    ngx_int_t  rc;
    ngx_str_t  token;

    nếu (ngx_json_skipping(ctx->skip_until_depth)) {
        trả_về NGX_OK;
    }

    nếu (ctx->handler == NULL) {
        trả_về NGX_OK;
    }

    token.data = data;
    token.len = len;
    rc = ctx->handler(ctx, event, &token);

    nếu (rc == NGX_JSON_SKIP) {

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
            nếu (ctx->depth > 0
                && ctx->stack[ctx->depth - 1] == ngx_json_ctx_array
            ) {
                ctx->skip_until_depth = ctx->depth - 1;
            }
            break;

        default:
            break;
        }

        trả_về NGX_OK;
    }

    trả_về rc;
}


static ngx_inline ngx_json_state_e
ngx_json_after_value(ngx_json_ctx_t *ctx)
{
    nếu (ctx->depth == 0) {
        trả_về ngx_json_done;
    }

    nếu (ctx->stack[ctx->depth - 1] == ngx_json_ctx_object) {
        trả_về ngx_json_object_value_separator;
    }

    trả_về ngx_json_array_value_separator;
}


static ngx_inline ngx_int_t
ngx_json_close(ngx_json_ctx_t *ctx, ngx_json_event_e event,
    ngx_json_state_e *state, u_char *p)
{
    ngx_int_t  rc;

    ctx->depth--;

    rc = ngx_json_emit(ctx, event, p, 1);
    nếu (rc != NGX_OK) {
        trả_về rc;
    }

    *state = ngx_json_after_value(ctx);

    trả_về NGX_OK;
}


static ngx_int_t
ngx_json_push_state(ngx_json_ctx_t *ctx, ngx_json_container_e container)
{
    ngx_json_container_e  *stack;

    nếu (ctx->depth >= ctx->max_depth) {
        trả_về NGX_DECLINED;
    }

    nếu (ctx->stack == NULL) {
        stack = ngx_palloc(ctx->pool,
                           ctx->max_depth * sizeof(ngx_json_container_e));
        nếu (stack == NULL) {
            trả_về NGX_ERROR;
        }

        ctx->stack = stack;
    }

    ctx->stack[ctx->depth] = container;
    ctx->depth++;

    trả_về NGX_OK;
}
