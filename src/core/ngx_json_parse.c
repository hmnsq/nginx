
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

    如果 (len == 0) {
        返回 NGX_DECLINED;
    }

    如果 (data == NULL) {
        返回 NGX_ERROR;
    }

    如果 (ctx->state == ngx_json_done) {
        返回 NGX_DECLINED;
    }

    last = data + len;
    state = ctx->state;
    skip_depth = ctx->skip_until_depth;
    start = NULL;

    为 (p = data; p < last; p++) {

        /* skipping suppresses emission only; the machine still validates */

        如果 (ngx_json_skipping(skip_depth) && ctx->depth <= skip_depth
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
            如果 (ngx_json_ws(*p)) {
                break;
            }

            p--;
            state = ngx_json_value;
            break;

        case ngx_json_object:
            如果 (ngx_json_ws(*p)) {
                break;
            }

            如果 (*p == '}') {
                rc = ngx_json_close(ctx, NGX_JSON_OBJECT_CLOSE, &state, p);
                如果 (rc != NGX_OK) {
                    返回 rc;
                }

                break;
            }

            /* fall through */

        case ngx_json_object_next:
            如果 (ngx_json_ws(*p)) {
                break;
            }

            如果 (*p == '"') {
                ctx->in_key = 1;
                ctx->escaped = 0;
                start = p + 1;
                state = ngx_json_string;
                break;
            }

            返回 NGX_DECLINED;

        case ngx_json_object_name_separator:
            如果 (ngx_json_ws(*p)) {
                break;
            }

            如果 (*p == ':') {
                state = ngx_json_value;
                break;
            }

            返回 NGX_DECLINED;

        case ngx_json_object_value_separator:
            如果 (ngx_json_ws(*p)) {
                break;
            }

            如果 (*p == ',') {
                state = ngx_json_object_next;
                break;
            }

            如果 (*p == '}') {
                rc = ngx_json_close(ctx, NGX_JSON_OBJECT_CLOSE, &state, p);
                如果 (rc != NGX_OK) {
                    返回 rc;
                }

                break;
            }

            返回 NGX_DECLINED;

        case ngx_json_array_value_separator:
            如果 (ngx_json_ws(*p)) {
                break;
            }

            如果 (*p == ',') {
                state = ngx_json_value;
                break;
            }

            如果 (*p == ']') {
                rc = ngx_json_close(ctx, NGX_JSON_ARRAY_CLOSE, &state, p);
                如果 (rc != NGX_OK) {
                    返回 rc;
                }

                break;
            }

            返回 NGX_DECLINED;

        case ngx_json_array:
            如果 (ngx_json_ws(*p)) {
                break;
            }

            如果 (*p == ']') {
                rc = ngx_json_close(ctx, NGX_JSON_ARRAY_CLOSE, &state, p);
                如果 (rc != NGX_OK) {
                    返回 rc;
                }

                break;
            }

            /* fall through */

        case ngx_json_value:
            如果 (ngx_json_ws(*p)) {
                break;
            }

            start = p;

            如果 (*p == '"') {
                ctx->in_key = 0;
                ctx->escaped = 0;
                start = p + 1;
                state = ngx_json_string;
                break;
            }

            如果 (*p == '{') {
                rc = ngx_json_emit(ctx, NGX_JSON_OBJECT_OPEN, p, 1);
                如果 (rc != NGX_OK) {
                    返回 rc;
                }

                rc = ngx_json_push_state(ctx, ngx_json_ctx_object);
                如果 (rc != NGX_OK) {
                    返回 rc;
                }

                state = ngx_json_object;
                break;
            }

            如果 (*p == '[') {
                rc = ngx_json_emit(ctx, NGX_JSON_ARRAY_OPEN, p, 1);
                如果 (rc != NGX_OK) {
                    返回 rc;
                }

                rc = ngx_json_push_state(ctx, ngx_json_ctx_array);
                如果 (rc != NGX_OK) {
                    返回 rc;
                }

                state = ngx_json_array;
                break;
            }

            如果 (*p == '-') {
                state = ngx_json_number_minus;
                break;
            }

            如果 (*p == '0') {
                state = ngx_json_number_int_zero;
                break;
            }

            如果 (*p >= '1' && *p <= '9') {
                state = ngx_json_number_int;
                break;
            }

            如果 (*p == 't') {
                state = ngx_json_true_t;
                break;
            }

            如果 (*p == 'f') {
                state = ngx_json_false_f;
                break;
            }

            如果 (*p == 'n') {
                state = ngx_json_null_n;
                break;
            }

            返回 NGX_DECLINED;

        case ngx_json_string:
            如果 (*p == '"') {

                如果 (ctx->in_key) {
                    event = ctx->escaped ? NGX_JSON_KEY_ESCAPED
                                         : NGX_JSON_KEY;

                    rc = ngx_json_emit(ctx, event, start, p - start);
                    如果 (rc != NGX_OK) {
                        返回 rc;
                    }

                    ctx->in_key = 0;
                    state = ngx_json_object_name_separator;

                } 否则 {
                    event = ctx->escaped ? NGX_JSON_VALUE_STRING_ESCAPED
                                         : NGX_JSON_VALUE_STRING;

                    rc = ngx_json_emit(ctx, event, start, p - start);
                    如果 (rc != NGX_OK) {
                        返回 rc;
                    }

                    state = ngx_json_after_value(ctx);
                }

                break;
            }

            如果 (*p == '\\') {
                ctx->escaped = 1;
                state = ngx_json_escaped;
                break;
            }

            如果 (*p < ' ') {
                返回 NGX_DECLINED;
            }

            break;

        case ngx_json_escaped:
            如果 (*p == '"' || *p == '\\' || *p == '/' || *p == 'b'
                || *p == 'f' || *p == 'n' || *p == 'r' || *p == 't')
            {
                state = ngx_json_string;
                break;
            }

            如果 (*p == 'u') {
                ctx->codepoint = 0;
                ctx->hex_left = 4;
                state = ngx_json_escaped_hex;
                break;
            }

            返回 NGX_DECLINED;

        case ngx_json_escaped_hex:

            /* accumulate the four hex digits of the first \uXXXX escape */

            hd = ngx_json_hex_digit(*p);
            如果 (hd == NGX_ERROR) {
                返回 NGX_DECLINED;
            }

            ctx->codepoint = (ctx->codepoint << 4) | (uint32_t) hd;

            如果 (--ctx->hex_left > 0) {
                break;
            }

            如果 (ctx->codepoint >= 0xD800 && ctx->codepoint <= 0xDBFF) {
                /* high surrogate - must be followed by \uDC00..\uDFFF */
                state = ngx_json_surrogate_start;
                break;
            }

            如果 (ctx->codepoint >= 0xDC00 && ctx->codepoint <= 0xDFFF) {
                /* lone low surrogate */
                返回 NGX_DECLINED;
            }

            state = ngx_json_string;
            break;

        case ngx_json_surrogate_start:
            如果 (*p == '\\') {
                state = ngx_json_surrogate_u;
                break;
            }

            /* high surrogate not followed by \uXXXX */
            返回 NGX_DECLINED;

        case ngx_json_surrogate_u:
            如果 (*p == 'u') {
                ctx->codepoint = 0;
                ctx->hex_left = 4;
                state = ngx_json_surrogate_hex;
                break;
            }

            /* high surrogate followed by non-\u escape */
            返回 NGX_DECLINED;

        case ngx_json_surrogate_hex:

            /* accumulate the four hex digits of the low \uXXXX escape */

            hd = ngx_json_hex_digit(*p);
            如果 (hd == NGX_ERROR) {
                返回 NGX_DECLINED;
            }

            ctx->codepoint = (ctx->codepoint << 4) | (uint32_t) hd;

            如果 (--ctx->hex_left > 0) {
                break;
            }

            如果 (ctx->codepoint >= 0xDC00 && ctx->codepoint <= 0xDFFF) {
                /* valid surrogate pair */
                state = ngx_json_string;
                break;
            }

            /* high surrogate not followed by low surrogate */
            返回 NGX_DECLINED;

        case ngx_json_number_minus:
            如果 (*p >= '1' && *p <= '9') {
                state = ngx_json_number_int;
                break;
            }

            如果 (*p == '0') {
                state = ngx_json_number_int_zero;
                break;
            }

            返回 NGX_DECLINED;

        case ngx_json_number_int_zero:
            如果 (*p == '.') {
                state = ngx_json_number_frac;
                break;
            }

            如果 (*p == 'e' || *p == 'E') {
                /* No sense, but permitted by RFC */
                state = ngx_json_number_exp;
                break;
            }

            p--;

            rc = ngx_json_end_number(ctx, start, p + 1, &state);
            如果 (rc != NGX_OK) {
                返回 rc;
            }

            break;

        case ngx_json_number_int:
            如果 (*p >= '0' && *p <= '9') {
                break;
            }

            如果 (*p == '.') {
                state = ngx_json_number_frac;
                break;
            }

            如果 (*p == 'e' || *p == 'E') {
                /* No sense, but permitted by RFC */
                state = ngx_json_number_exp;
                break;
            }

            p--;

            rc = ngx_json_end_number(ctx, start, p + 1, &state);
            如果 (rc!= NGX_OK) {
                返回 rc;
            }

            break;

        case ngx_json_number_frac:
            如果 (*p >= '0' && *p <= '9') {
                state = ngx_json_number_frac_digit;
                break;
            }

            返回 NGX_DECLINED;

        case ngx_json_number_frac_digit:
            如果 (*p >= '0' && *p <= '9') {
                break;
            }

            如果 (*p == 'e' || *p == 'E') {
                state = ngx_json_number_exp;
                break;
            }

            p--;

            rc = ngx_json_end_number(ctx, start, p + 1, &state);
            如果 (rc != NGX_OK) {
                返回 rc;
            }

            break;

        case ngx_json_number_exp:
            如果 (*p == '-' || *p == '+') {
                state = ngx_json_number_exp_plusminus;
                break;
            }

            如果 (*p >= '0' && *p <= '9') {
                state = ngx_json_number_exp_digit;
                break;
            }

            返回 NGX_DECLINED;

        case ngx_json_number_exp_plusminus:
            如果 (*p >= '0' && *p <= '9') {
                state = ngx_json_number_exp_digit;
                break;
            }

            返回 NGX_DECLINED;

        case ngx_json_number_exp_digit:
            如果 (*p >= '0' && *p <= '9') {
                break;
            }

            p--;

            rc = ngx_json_end_number(ctx, start, p + 1, &state);
            如果 (rc != NGX_OK) {
                返回 rc;
            }

            break;

        case ngx_json_true_t:
            如果 (*p == 'r') {
                state = ngx_json_true_tr;
                break;
            }

            返回 NGX_DECLINED;

        case ngx_json_true_tr:
            如果 (*p == 'u') {
                state = ngx_json_true_tru;
                break;
            }

            返回 NGX_DECLINED;

        case ngx_json_true_tru:
            如果 (*p == 'e') {
                rc = ngx_json_emit(ctx, NGX_JSON_VALUE_BOOL, start,
                                  p + 1 - start);
                如果 (rc != NGX_OK) {
                    返回 rc;
                }

                state = ngx_json_after_value(ctx);
                break;
            }

            返回 NGX_DECLINED;

        case ngx_json_false_f:
            如果 (*p == 'a') {
                state = ngx_json_false_fa;
                break;
            }

            返回 NGX_DECLINED;

        case ngx_json_false_fa:
            如果 (*p == 'l') {
                state = ngx_json_false_fal;
                break;
            }

            返回 NGX_DECLINED;

        case ngx_json_false_fal:
            如果 (*p == 's') {
                state = ngx_json_false_fals;
                break;
            }

            返回 NGX_DECLINED;

        case ngx_json_false_fals:
            如果 (*p == 'e') {
                rc = ngx_json_emit(ctx, NGX_JSON_VALUE_BOOL, start,
                                  p + 1 - start);
                如果 (rc != NGX_OK) {
                    返回 rc;
                }

                state = ngx_json_after_value(ctx);
                break;
            }

            返回 NGX_DECLINED;

        case ngx_json_null_n:
            如果 (*p == 'u') {
                state = ngx_json_null_nu;
                break;
            }

            返回 NGX_DECLINED;

        case ngx_json_null_nu:
            如果 (*p == 'l') {
                state = ngx_json_null_nul;
                break;
            }

            返回 NGX_DECLINED;

        case ngx_json_null_nul:
            如果 (*p == 'l') {
                rc = ngx_json_emit(ctx, NGX_JSON_VALUE_NULL, start,
                                  p + 1 - start);
                如果 (rc != NGX_OK) {
                    返回 rc;
                }

                state = ngx_json_after_value(ctx);
                break;
            }

            返回 NGX_DECLINED;

        case ngx_json_done:
            如果 (ngx_json_ws(*p)) {
                break;
            }

            返回 NGX_DECLINED;
        }

        skip_depth = ctx->skip_until_depth;
    }

    /* a bare top-level number is not self-delimited; emit it at end of input */

    如果 (ctx->depth == 0
        && (state == ngx_json_number_int_zero
            || state == ngx_json_number_int
            || state == ngx_json_number_frac_digit
            || state == ngx_json_number_exp_digit))
    {
        rc = ngx_json_emit(ctx, NGX_JSON_VALUE_NUMBER, start, last - start);
        如果 (rc != NGX_OK) {
            返回 rc;
        }

        state = ngx_json_done;
    }

    ctx->state = state;

    如果 (state == ngx_json_done) {
        返回 NGX_OK;
    }

    返回 NGX_DECLINED;
}


ngx_int_t
ngx_json_parse(ngx_pool_t *pool, ngx_str_t *json, ngx_json_handler_pt handler,
    void *data)
{
    ngx_json_ctx_t  jctx;

    ngx_json_ctx_init(&jctx, pool);

    jctx.data = data;
    jctx.handler = handler;

    返回 ngx_json_parse_ctx(&jctx, json->data, json->len);
}


static ngx_inline ngx_int_t
ngx_json_end_number(ngx_json_ctx_t *ctx, u_char *start, u_char *end,
    ngx_json_state_e *state)
{
    ngx_int_t  rc;

    rc = ngx_json_emit(ctx, NGX_JSON_VALUE_NUMBER, start, end - start);
    如果 (rc != NGX_OK) {
        返回 rc;
    }

    *state = ngx_json_after_value(ctx);

    返回 NGX_OK;
}


static ngx_inline ngx_int_t
ngx_json_emit(ngx_json_ctx_t *ctx, ngx_json_event_e event, u_char *data,
    size_t len)
{
    ngx_int_t  rc;
    ngx_str_t  token;

    如果 (ngx_json_skipping(ctx->skip_until_depth)) {
        返回 NGX_OK;
    }

    如果 (ctx->handler == NULL) {
        返回 NGX_OK;
    }

    token.data = data;
    token.len = len;
    rc = ctx->handler(ctx, event, &token);

    如果 (rc == NGX_JSON_SKIP) {

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
            如果 (ctx->depth > 0
                && ctx->stack[ctx->depth - 1] == ngx_json_ctx_array
            ) {
                ctx->skip_until_depth = ctx->depth - 1;
            }
            break;

        default:
            break;
        }

        返回 NGX_OK;
    }

    返回 rc;
}


static ngx_inline ngx_json_state_e
ngx_json_after_value(ngx_json_ctx_t *ctx)
{
    如果 (ctx->depth == 0) {
        返回 ngx_json_done;
    }

    如果 (ctx->stack[ctx->depth - 1] == ngx_json_ctx_object) {
        返回 ngx_json_object_value_separator;
    }

    返回 ngx_json_array_value_separator;
}


static ngx_inline ngx_int_t
ngx_json_close(ngx_json_ctx_t *ctx, ngx_json_event_e event,
    ngx_json_state_e *state, u_char *p)
{
    ngx_int_t  rc;

    ctx->depth--;

    rc = ngx_json_emit(ctx, event, p, 1);
    如果 (rc != NGX_OK) {
        返回 rc;
    }

    *state = ngx_json_after_value(ctx);

    返回 NGX_OK;
}


static ngx_int_t
ngx_json_push_state(ngx_json_ctx_t *ctx, ngx_json_container_e container)
{
    ngx_json_container_e  *stack;

    如果 (ctx->depth >= ctx->max_depth) {
        返回 NGX_DECLINED;
    }

    如果 (ctx->stack == NULL) {
        stack = ngx_palloc(ctx->pool,
                           ctx->max_depth * sizeof(ngx_json_container_e));
        如果 (stack == NULL) {
            返回 NGX_ERROR;
        }

        ctx->stack = stack;
    }

    ctx->stack[ctx->depth] = container;
    ctx->depth++;

    返回 NGX_OK;
}
