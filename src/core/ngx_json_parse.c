
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

    만약 (len == 0) {
        반환 NGX_DECLINED;
    }

    만약 (data == NULL) {
        반환 NGX_ERROR;
    }

    만약 (ctx->state == ngx_json_done) {
        반환 NGX_DECLINED;
    }

    last = data + len;
    state = ctx->state;
    skip_depth = ctx->skip_until_depth;
    start = NULL;

    위해 (p = data; p < last; p++) {

        /* skipping suppresses emission only; the machine still validates */

        만약 (ngx_json_skipping(skip_depth) && ctx->depth <= skip_depth
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
            만약 (ngx_json_ws(*p)) {
                break;
            }

            p--;
            state = ngx_json_value;
            break;

        case ngx_json_object:
            만약 (ngx_json_ws(*p)) {
                break;
            }

            만약 (*p == '}') {
                rc = ngx_json_close(ctx, NGX_JSON_OBJECT_CLOSE, &state, p);
                만약 (rc != NGX_OK) {
                    반환 rc;
                }

                break;
            }

            /* fall through */

        case ngx_json_object_next:
            만약 (ngx_json_ws(*p)) {
                break;
            }

            만약 (*p == '"') {
                ctx->in_key = 1;
                ctx->escaped = 0;
                start = p + 1;
                state = ngx_json_string;
                break;
            }

            반환 NGX_DECLINED;

        case ngx_json_object_name_separator:
            만약 (ngx_json_ws(*p)) {
                break;
            }

            만약 (*p == ':') {
                state = ngx_json_value;
                break;
            }

            반환 NGX_DECLINED;

        case ngx_json_object_value_separator:
            만약 (ngx_json_ws(*p)) {
                break;
            }

            만약 (*p == ',') {
                state = ngx_json_object_next;
                break;
            }

            만약 (*p == '}') {
                rc = ngx_json_close(ctx, NGX_JSON_OBJECT_CLOSE, &state, p);
                만약 (rc != NGX_OK) {
                    반환 rc;
                }

                break;
            }

            반환 NGX_DECLINED;

        case ngx_json_array_value_separator:
            만약 (ngx_json_ws(*p)) {
                break;
            }

            만약 (*p == ',') {
                state = ngx_json_value;
                break;
            }

            만약 (*p == ']') {
                rc = ngx_json_close(ctx, NGX_JSON_ARRAY_CLOSE, &state, p);
                만약 (rc != NGX_OK) {
                    반환 rc;
                }

                break;
            }

            반환 NGX_DECLINED;

        case ngx_json_array:
            만약 (ngx_json_ws(*p)) {
                break;
            }

            만약 (*p == ']') {
                rc = ngx_json_close(ctx, NGX_JSON_ARRAY_CLOSE, &state, p);
                만약 (rc != NGX_OK) {
                    반환 rc;
                }

                break;
            }

            /* fall through */

        case ngx_json_value:
            만약 (ngx_json_ws(*p)) {
                break;
            }

            start = p;

            만약 (*p == '"') {
                ctx->in_key = 0;
                ctx->escaped = 0;
                start = p + 1;
                state = ngx_json_string;
                break;
            }

            만약 (*p == '{') {
                rc = ngx_json_emit(ctx, NGX_JSON_OBJECT_OPEN, p, 1);
                만약 (rc != NGX_OK) {
                    반환 rc;
                }

                rc = ngx_json_push_state(ctx, ngx_json_ctx_object);
                만약 (rc != NGX_OK) {
                    반환 rc;
                }

                state = ngx_json_object;
                break;
            }

            만약 (*p == '[') {
                rc = ngx_json_emit(ctx, NGX_JSON_ARRAY_OPEN, p, 1);
                만약 (rc != NGX_OK) {
                    반환 rc;
                }

                rc = ngx_json_push_state(ctx, ngx_json_ctx_array);
                만약 (rc != NGX_OK) {
                    반환 rc;
                }

                state = ngx_json_array;
                break;
            }

            만약 (*p == '-') {
                state = ngx_json_number_minus;
                break;
            }

            만약 (*p == '0') {
                state = ngx_json_number_int_zero;
                break;
            }

            만약 (*p >= '1' && *p <= '9') {
                state = ngx_json_number_int;
                break;
            }

            만약 (*p == 't') {
                state = ngx_json_true_t;
                break;
            }

            만약 (*p == 'f') {
                state = ngx_json_false_f;
                break;
            }

            만약 (*p == 'n') {
                state = ngx_json_null_n;
                break;
            }

            반환 NGX_DECLINED;

        case ngx_json_string:
            만약 (*p == '"') {

                만약 (ctx->in_key) {
                    event = ctx->escaped ? NGX_JSON_KEY_ESCAPED
                                         : NGX_JSON_KEY;

                    rc = ngx_json_emit(ctx, event, start, p - start);
                    만약 (rc != NGX_OK) {
                        반환 rc;
                    }

                    ctx->in_key = 0;
                    state = ngx_json_object_name_separator;

                } 아니면 {
                    event = ctx->escaped ? NGX_JSON_VALUE_STRING_ESCAPED
                                         : NGX_JSON_VALUE_STRING;

                    rc = ngx_json_emit(ctx, event, start, p - start);
                    만약 (rc != NGX_OK) {
                        반환 rc;
                    }

                    state = ngx_json_after_value(ctx);
                }

                break;
            }

            만약 (*p == '\\') {
                ctx->escaped = 1;
                state = ngx_json_escaped;
                break;
            }

            만약 (*p < ' ') {
                반환 NGX_DECLINED;
            }

            break;

        case ngx_json_escaped:
            만약 (*p == '"' || *p == '\\' || *p == '/' || *p == 'b'
                || *p == 'f' || *p == 'n' || *p == 'r' || *p == 't')
            {
                state = ngx_json_string;
                break;
            }

            만약 (*p == 'u') {
                ctx->codepoint = 0;
                ctx->hex_left = 4;
                state = ngx_json_escaped_hex;
                break;
            }

            반환 NGX_DECLINED;

        case ngx_json_escaped_hex:

            /* accumulate the four hex digits of the first \uXXXX escape */

            hd = ngx_json_hex_digit(*p);
            만약 (hd == NGX_ERROR) {
                반환 NGX_DECLINED;
            }

            ctx->codepoint = (ctx->codepoint << 4) | (uint32_t) hd;

            만약 (--ctx->hex_left > 0) {
                break;
            }

            만약 (ctx->codepoint >= 0xD800 && ctx->codepoint <= 0xDBFF) {
                /* high surrogate - must be followed by \uDC00..\uDFFF */
                state = ngx_json_surrogate_start;
                break;
            }

            만약 (ctx->codepoint >= 0xDC00 && ctx->codepoint <= 0xDFFF) {
                /* lone low surrogate */
                반환 NGX_DECLINED;
            }

            state = ngx_json_string;
            break;

        case ngx_json_surrogate_start:
            만약 (*p == '\\') {
                state = ngx_json_surrogate_u;
                break;
            }

            /* high surrogate not followed by \uXXXX */
            반환 NGX_DECLINED;

        case ngx_json_surrogate_u:
            만약 (*p == 'u') {
                ctx->codepoint = 0;
                ctx->hex_left = 4;
                state = ngx_json_surrogate_hex;
                break;
            }

            /* high surrogate followed by non-\u escape */
            반환 NGX_DECLINED;

        case ngx_json_surrogate_hex:

            /* accumulate the four hex digits of the low \uXXXX escape */

            hd = ngx_json_hex_digit(*p);
            만약 (hd == NGX_ERROR) {
                반환 NGX_DECLINED;
            }

            ctx->codepoint = (ctx->codepoint << 4) | (uint32_t) hd;

            만약 (--ctx->hex_left > 0) {
                break;
            }

            만약 (ctx->codepoint >= 0xDC00 && ctx->codepoint <= 0xDFFF) {
                /* valid surrogate pair */
                state = ngx_json_string;
                break;
            }

            /* high surrogate not followed by low surrogate */
            반환 NGX_DECLINED;

        case ngx_json_number_minus:
            만약 (*p >= '1' && *p <= '9') {
                state = ngx_json_number_int;
                break;
            }

            만약 (*p == '0') {
                state = ngx_json_number_int_zero;
                break;
            }

            반환 NGX_DECLINED;

        case ngx_json_number_int_zero:
            만약 (*p == '.') {
                state = ngx_json_number_frac;
                break;
            }

            만약 (*p == 'e' || *p == 'E') {
                /* No sense, but permitted by RFC */
                state = ngx_json_number_exp;
                break;
            }

            p--;

            rc = ngx_json_end_number(ctx, start, p + 1, &state);
            만약 (rc != NGX_OK) {
                반환 rc;
            }

            break;

        case ngx_json_number_int:
            만약 (*p >= '0' && *p <= '9') {
                break;
            }

            만약 (*p == '.') {
                state = ngx_json_number_frac;
                break;
            }

            만약 (*p == 'e' || *p == 'E') {
                /* No sense, but permitted by RFC */
                state = ngx_json_number_exp;
                break;
            }

            p--;

            rc = ngx_json_end_number(ctx, start, p + 1, &state);
            만약 (rc!= NGX_OK) {
                반환 rc;
            }

            break;

        case ngx_json_number_frac:
            만약 (*p >= '0' && *p <= '9') {
                state = ngx_json_number_frac_digit;
                break;
            }

            반환 NGX_DECLINED;

        case ngx_json_number_frac_digit:
            만약 (*p >= '0' && *p <= '9') {
                break;
            }

            만약 (*p == 'e' || *p == 'E') {
                state = ngx_json_number_exp;
                break;
            }

            p--;

            rc = ngx_json_end_number(ctx, start, p + 1, &state);
            만약 (rc != NGX_OK) {
                반환 rc;
            }

            break;

        case ngx_json_number_exp:
            만약 (*p == '-' || *p == '+') {
                state = ngx_json_number_exp_plusminus;
                break;
            }

            만약 (*p >= '0' && *p <= '9') {
                state = ngx_json_number_exp_digit;
                break;
            }

            반환 NGX_DECLINED;

        case ngx_json_number_exp_plusminus:
            만약 (*p >= '0' && *p <= '9') {
                state = ngx_json_number_exp_digit;
                break;
            }

            반환 NGX_DECLINED;

        case ngx_json_number_exp_digit:
            만약 (*p >= '0' && *p <= '9') {
                break;
            }

            p--;

            rc = ngx_json_end_number(ctx, start, p + 1, &state);
            만약 (rc != NGX_OK) {
                반환 rc;
            }

            break;

        case ngx_json_true_t:
            만약 (*p == 'r') {
                state = ngx_json_true_tr;
                break;
            }

            반환 NGX_DECLINED;

        case ngx_json_true_tr:
            만약 (*p == 'u') {
                state = ngx_json_true_tru;
                break;
            }

            반환 NGX_DECLINED;

        case ngx_json_true_tru:
            만약 (*p == 'e') {
                rc = ngx_json_emit(ctx, NGX_JSON_VALUE_BOOL, start,
                                  p + 1 - start);
                만약 (rc != NGX_OK) {
                    반환 rc;
                }

                state = ngx_json_after_value(ctx);
                break;
            }

            반환 NGX_DECLINED;

        case ngx_json_false_f:
            만약 (*p == 'a') {
                state = ngx_json_false_fa;
                break;
            }

            반환 NGX_DECLINED;

        case ngx_json_false_fa:
            만약 (*p == 'l') {
                state = ngx_json_false_fal;
                break;
            }

            반환 NGX_DECLINED;

        case ngx_json_false_fal:
            만약 (*p == 's') {
                state = ngx_json_false_fals;
                break;
            }

            반환 NGX_DECLINED;

        case ngx_json_false_fals:
            만약 (*p == 'e') {
                rc = ngx_json_emit(ctx, NGX_JSON_VALUE_BOOL, start,
                                  p + 1 - start);
                만약 (rc != NGX_OK) {
                    반환 rc;
                }

                state = ngx_json_after_value(ctx);
                break;
            }

            반환 NGX_DECLINED;

        case ngx_json_null_n:
            만약 (*p == 'u') {
                state = ngx_json_null_nu;
                break;
            }

            반환 NGX_DECLINED;

        case ngx_json_null_nu:
            만약 (*p == 'l') {
                state = ngx_json_null_nul;
                break;
            }

            반환 NGX_DECLINED;

        case ngx_json_null_nul:
            만약 (*p == 'l') {
                rc = ngx_json_emit(ctx, NGX_JSON_VALUE_NULL, start,
                                  p + 1 - start);
                만약 (rc != NGX_OK) {
                    반환 rc;
                }

                state = ngx_json_after_value(ctx);
                break;
            }

            반환 NGX_DECLINED;

        case ngx_json_done:
            만약 (ngx_json_ws(*p)) {
                break;
            }

            반환 NGX_DECLINED;
        }

        skip_depth = ctx->skip_until_depth;
    }

    /* a bare top-level number is not self-delimited; emit it at end of input */

    만약 (ctx->depth == 0
        && (state == ngx_json_number_int_zero
            || state == ngx_json_number_int
            || state == ngx_json_number_frac_digit
            || state == ngx_json_number_exp_digit))
    {
        rc = ngx_json_emit(ctx, NGX_JSON_VALUE_NUMBER, start, last - start);
        만약 (rc != NGX_OK) {
            반환 rc;
        }

        state = ngx_json_done;
    }

    ctx->state = state;

    만약 (state == ngx_json_done) {
        반환 NGX_OK;
    }

    반환 NGX_DECLINED;
}


ngx_int_t
ngx_json_parse(ngx_pool_t *pool, ngx_str_t *json, ngx_json_handler_pt handler,
    void *data)
{
    ngx_json_ctx_t  jctx;

    ngx_json_ctx_init(&jctx, pool);

    jctx.data = data;
    jctx.handler = handler;

    반환 ngx_json_parse_ctx(&jctx, json->data, json->len);
}


static ngx_inline ngx_int_t
ngx_json_end_number(ngx_json_ctx_t *ctx, u_char *start, u_char *end,
    ngx_json_state_e *state)
{
    ngx_int_t  rc;

    rc = ngx_json_emit(ctx, NGX_JSON_VALUE_NUMBER, start, end - start);
    만약 (rc != NGX_OK) {
        반환 rc;
    }

    *state = ngx_json_after_value(ctx);

    반환 NGX_OK;
}


static ngx_inline ngx_int_t
ngx_json_emit(ngx_json_ctx_t *ctx, ngx_json_event_e event, u_char *data,
    size_t len)
{
    ngx_int_t  rc;
    ngx_str_t  token;

    만약 (ngx_json_skipping(ctx->skip_until_depth)) {
        반환 NGX_OK;
    }

    만약 (ctx->handler == NULL) {
        반환 NGX_OK;
    }

    token.data = data;
    token.len = len;
    rc = ctx->handler(ctx, event, &token);

    만약 (rc == NGX_JSON_SKIP) {

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
            만약 (ctx->depth > 0
                && ctx->stack[ctx->depth - 1] == ngx_json_ctx_array
            ) {
                ctx->skip_until_depth = ctx->depth - 1;
            }
            break;

        default:
            break;
        }

        반환 NGX_OK;
    }

    반환 rc;
}


static ngx_inline ngx_json_state_e
ngx_json_after_value(ngx_json_ctx_t *ctx)
{
    만약 (ctx->depth == 0) {
        반환 ngx_json_done;
    }

    만약 (ctx->stack[ctx->depth - 1] == ngx_json_ctx_object) {
        반환 ngx_json_object_value_separator;
    }

    반환 ngx_json_array_value_separator;
}


static ngx_inline ngx_int_t
ngx_json_close(ngx_json_ctx_t *ctx, ngx_json_event_e event,
    ngx_json_state_e *state, u_char *p)
{
    ngx_int_t  rc;

    ctx->depth--;

    rc = ngx_json_emit(ctx, event, p, 1);
    만약 (rc != NGX_OK) {
        반환 rc;
    }

    *state = ngx_json_after_value(ctx);

    반환 NGX_OK;
}


static ngx_int_t
ngx_json_push_state(ngx_json_ctx_t *ctx, ngx_json_container_e container)
{
    ngx_json_container_e  *stack;

    만약 (ctx->depth >= ctx->max_depth) {
        반환 NGX_DECLINED;
    }

    만약 (ctx->stack == NULL) {
        stack = ngx_palloc(ctx->pool,
                           ctx->max_depth * sizeof(ngx_json_container_e));
        만약 (stack == NULL) {
            반환 NGX_ERROR;
        }

        ctx->stack = stack;
    }

    ctx->stack[ctx->depth] = container;
    ctx->depth++;

    반환 NGX_OK;
}
