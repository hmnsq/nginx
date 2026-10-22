
/*
 * Copyright (C) Maxim Dounin
 * Copyright (C) Nginx, Inc.
 */


/*
 * JSON parser. RFC 8259 for the JSON grammar.

 * Site http://json.org/ may be useful too.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>
#ضمن <ngx_json_parse.h>


#define NGX_JSON_NOT_SKIPPING  (ngx_uint_t) -1
#define ngx_json_skipping(d)   ((d) != NGX_JSON_NOT_SKIPPING)
#define ngx_json_ws(c)  ((c) == ' ' || (c) == '\t' || (c) == CR || (c) == LF)


ساكن ngx_inline ngx_int_t ngx_json_end_number(ngx_json_ctx_t *ctx,
    u_char *start, u_char *end, ngx_json_state_e *state);
ساكن ngx_inline ngx_int_t ngx_json_emit(ngx_json_ctx_t *ctx,
    ngx_json_event_e event, u_char *data, size_t len);
ساكن ngx_inline ngx_json_state_e ngx_json_after_value(ngx_json_ctx_t *ctx);
ساكن ngx_inline ngx_int_t ngx_json_close(ngx_json_ctx_t *ctx,
    ngx_json_event_e event, ngx_json_state_e *state, u_char *p);
ساكن ngx_int_t ngx_json_push_state(ngx_json_ctx_t *ctx,
    ngx_json_container_e container);


فراغ
ngx_json_ctx_init(ngx_json_ctx_t *ctx, ngx_pool_t *pool)
{
    ctx->state = ngx_json_start;
    ctx->pool = pool;
    ctx->handler = عدم;
    ctx->data = عدم;
    ctx->codepoint = 0;
    ctx->hex_left = 0;

    ctx->depth = 0;
    ctx->skip_until_depth = NGX_JSON_NOT_SKIPPING;
    ctx->max_depth = NGX_JSON_DEFAULT_MAX_DEPTH;

    /* stack is allocated lazily on first push, sized to max_depth */

    ctx->stack = عدم;

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

    אם (len == 0) {
        החזר NGX_DECLINED;
    }

    אם (data == عدم) {
        החזר NGX_ERROR;
    }

    אם (ctx->state == ngx_json_done) {
        החזר NGX_DECLINED;
    }

    last = data + len;
    state = ctx->state;
    skip_depth = ctx->skip_until_depth;
    start = عدم;

    עבור (p = data; p < last; p++) {

        /* skipping suppresses emission only; the machine still validates */

        אם (ngx_json_skipping(skip_depth) && ctx->depth <= skip_depth
            && (state == ngx_json_object_value_separator
                || state == ngx_json_array_value_separator
                || state == ngx_json_done))
        {
            skip_depth = NGX_JSON_NOT_SKIPPING;
            ctx->skip_until_depth = NGX_JSON_NOT_SKIPPING;
            p--;

            استمر;
        }

        بدل (state) {

        حالة ngx_json_start:
            אם (ngx_json_ws(*p)) {
                اكسر;
            }

            p--;
            state = ngx_json_value;
            اكسر;

        حالة ngx_json_object:
            אם (ngx_json_ws(*p)) {
                اكسر;
            }

            אם (*p == '}') {
                rc = ngx_json_close(ctx, NGX_JSON_OBJECT_CLOSE, &state, p);
                אם (rc != NGX_OK) {
                    החזר rc;
                }

                اكسر;
            }

            /* fall through */

        حالة ngx_json_object_next:
            אם (ngx_json_ws(*p)) {
                اكسر;
            }

            אם (*p == '"') {
                ctx->in_key = 1;
                ctx->escaped = 0;
                start = p + 1;
                state = ngx_json_string;
                اكسر;
            }

            החזר NGX_DECLINED;

        حالة ngx_json_object_name_separator:
            אם (ngx_json_ws(*p)) {
                اكسر;
            }

            אם (*p == ':') {
                state = ngx_json_value;
                اكسر;
            }

            החזר NGX_DECLINED;

        حالة ngx_json_object_value_separator:
            אם (ngx_json_ws(*p)) {
                اكسر;
            }

            אם (*p == ',') {
                state = ngx_json_object_next;
                اكسر;
            }

            אם (*p == '}') {
                rc = ngx_json_close(ctx, NGX_JSON_OBJECT_CLOSE, &state, p);
                אם (rc != NGX_OK) {
                    החזר rc;
                }

                اكسر;
            }

            החזר NGX_DECLINED;

        حالة ngx_json_array_value_separator:
            אם (ngx_json_ws(*p)) {
                اكسر;
            }

            אם (*p == ',') {
                state = ngx_json_value;
                اكسر;
            }

            אם (*p == ']') {
                rc = ngx_json_close(ctx, NGX_JSON_ARRAY_CLOSE, &state, p);
                אם (rc != NGX_OK) {
                    החזר rc;
                }

                اكسر;
            }

            החזר NGX_DECLINED;

        حالة ngx_json_array:
            אם (ngx_json_ws(*p)) {
                اكسر;
            }

            אם (*p == ']') {
                rc = ngx_json_close(ctx, NGX_JSON_ARRAY_CLOSE, &state, p);
                אם (rc != NGX_OK) {
                    החזר rc;
                }

                اكسر;
            }

            /* fall through */

        حالة ngx_json_value:
            אם (ngx_json_ws(*p)) {
                اكسر;
            }

            start = p;

            אם (*p == '"') {
                ctx->in_key = 0;
                ctx->escaped = 0;
                start = p + 1;
                state = ngx_json_string;
                اكسر;
            }

            אם (*p == '{') {
                rc = ngx_json_emit(ctx, NGX_JSON_OBJECT_OPEN, p, 1);
                אם (rc != NGX_OK) {
                    החזר rc;
                }

                rc = ngx_json_push_state(ctx, ngx_json_ctx_object);
                אם (rc != NGX_OK) {
                    החזר rc;
                }

                state = ngx_json_object;
                اكسر;
            }

            אם (*p == '[') {
                rc = ngx_json_emit(ctx, NGX_JSON_ARRAY_OPEN, p, 1);
                אם (rc != NGX_OK) {
                    החזר rc;
                }

                rc = ngx_json_push_state(ctx, ngx_json_ctx_array);
                אם (rc != NGX_OK) {
                    החזר rc;
                }

                state = ngx_json_array;
                اكسر;
            }

            אם (*p == '-') {
                state = ngx_json_number_minus;
                اكسر;
            }

            אם (*p == '0') {
                state = ngx_json_number_int_zero;
                اكسر;
            }

            אם (*p >= '1' && *p <= '9') {
                state = ngx_json_number_int;
                اكسر;
            }

            אם (*p == 't') {
                state = ngx_json_true_t;
                اكسر;
            }

            אם (*p == 'f') {
                state = ngx_json_false_f;
                اكسر;
            }

            אם (*p == 'n') {
                state = ngx_json_null_n;
                اكسر;
            }

            החזר NGX_DECLINED;

        حالة ngx_json_string:
            אם (*p == '"') {

                אם (ctx->in_key) {
                    event = ctx->escaped ? NGX_JSON_KEY_ESCAPED
                                         : NGX_JSON_KEY;

                    rc = ngx_json_emit(ctx, event, start, p - start);
                    אם (rc != NGX_OK) {
                        החזר rc;
                    }

                    ctx->in_key = 0;
                    state = ngx_json_object_name_separator;

                } אחרת {
                    event = ctx->escaped ? NGX_JSON_VALUE_STRING_ESCAPED
                                         : NGX_JSON_VALUE_STRING;

                    rc = ngx_json_emit(ctx, event, start, p - start);
                    אם (rc != NGX_OK) {
                        החזר rc;
                    }

                    state = ngx_json_after_value(ctx);
                }

                اكسر;
            }

            אם (*p == '\\') {
                ctx->escaped = 1;
                state = ngx_json_escaped;
                اكسر;
            }

            אם (*p < ' ') {
                החזר NGX_DECLINED;
            }

            اكسر;

        حالة ngx_json_escaped:
            אם (*p == '"' || *p == '\\' || *p == '/' || *p == 'b'
                || *p == 'f' || *p == 'n' || *p == 'r' || *p == 't')
            {
                state = ngx_json_string;
                اكسر;
            }

            אם (*p == 'u') {
                ctx->codepoint = 0;
                ctx->hex_left = 4;
                state = ngx_json_escaped_hex;
                اكسر;
            }

            החזר NGX_DECLINED;

        حالة ngx_json_escaped_hex:

            /* accumulate the four hex digits of the first \uXXXX escape */

            hd = ngx_json_hex_digit(*p);
            אם (hd == NGX_ERROR) {
                החזר NGX_DECLINED;
            }

            ctx->codepoint = (ctx->codepoint << 4) | (uint32_t) hd;

            אם (--ctx->hex_left > 0) {
                اكسر;
            }

            אם (ctx->codepoint >= 0xD800 && ctx->codepoint <= 0xDBFF) {
                /* high surrogate - must be followed by \uDC00..\uDFFF */
                state = ngx_json_surrogate_start;
                اكسر;
            }

            אם (ctx->codepoint >= 0xDC00 && ctx->codepoint <= 0xDFFF) {
                /* lone low surrogate */
                החזר NGX_DECLINED;
            }

            state = ngx_json_string;
            اكسر;

        حالة ngx_json_surrogate_start:
            אם (*p == '\\') {
                state = ngx_json_surrogate_u;
                اكسر;
            }

            /* high surrogate not followed by \uXXXX */
            החזר NGX_DECLINED;

        حالة ngx_json_surrogate_u:
            אם (*p == 'u') {
                ctx->codepoint = 0;
                ctx->hex_left = 4;
                state = ngx_json_surrogate_hex;
                اكسر;
            }

            /* high surrogate followed by non-\u escape */
            החזר NGX_DECLINED;

        حالة ngx_json_surrogate_hex:

            /* accumulate the four hex digits of the low \uXXXX escape */

            hd = ngx_json_hex_digit(*p);
            אם (hd == NGX_ERROR) {
                החזר NGX_DECLINED;
            }

            ctx->codepoint = (ctx->codepoint << 4) | (uint32_t) hd;

            אם (--ctx->hex_left > 0) {
                اكسر;
            }

            אם (ctx->codepoint >= 0xDC00 && ctx->codepoint <= 0xDFFF) {
                /* valid surrogate pair */
                state = ngx_json_string;
                اكسر;
            }

            /* high surrogate not followed by low surrogate */
            החזר NGX_DECLINED;

        حالة ngx_json_number_minus:
            אם (*p >= '1' && *p <= '9') {
                state = ngx_json_number_int;
                اكسر;
            }

            אם (*p == '0') {
                state = ngx_json_number_int_zero;
                اكسر;
            }

            החזר NGX_DECLINED;

        حالة ngx_json_number_int_zero:
            אם (*p == '.') {
                state = ngx_json_number_frac;
                اكسر;
            }

            אם (*p == 'e' || *p == 'E') {
                /* No sense, but permitted by RFC */
                state = ngx_json_number_exp;
                اكسر;
            }

            p--;

            rc = ngx_json_end_number(ctx, start, p + 1, &state);
            אם (rc != NGX_OK) {
                החזר rc;
            }

            اكسر;

        حالة ngx_json_number_int:
            אם (*p >= '0' && *p <= '9') {
                اكسر;
            }

            אם (*p == '.') {
                state = ngx_json_number_frac;
                اكسر;
            }

            אם (*p == 'e' || *p == 'E') {
                /* No sense, but permitted by RFC */
                state = ngx_json_number_exp;
                اكسر;
            }

            p--;

            rc = ngx_json_end_number(ctx, start, p + 1, &state);
            אם (rc!= NGX_OK) {
                החזר rc;
            }

            اكسر;

        حالة ngx_json_number_frac:
            אם (*p >= '0' && *p <= '9') {
                state = ngx_json_number_frac_digit;
                اكسر;
            }

            החזר NGX_DECLINED;

        حالة ngx_json_number_frac_digit:
            אם (*p >= '0' && *p <= '9') {
                اكسر;
            }

            אם (*p == 'e' || *p == 'E') {
                state = ngx_json_number_exp;
                اكسر;
            }

            p--;

            rc = ngx_json_end_number(ctx, start, p + 1, &state);
            אם (rc != NGX_OK) {
                החזר rc;
            }

            اكسر;

        حالة ngx_json_number_exp:
            אם (*p == '-' || *p == '+') {
                state = ngx_json_number_exp_plusminus;
                اكسر;
            }

            אם (*p >= '0' && *p <= '9') {
                state = ngx_json_number_exp_digit;
                اكسر;
            }

            החזר NGX_DECLINED;

        حالة ngx_json_number_exp_plusminus:
            אם (*p >= '0' && *p <= '9') {
                state = ngx_json_number_exp_digit;
                اكسر;
            }

            החזר NGX_DECLINED;

        حالة ngx_json_number_exp_digit:
            אם (*p >= '0' && *p <= '9') {
                اكسر;
            }

            p--;

            rc = ngx_json_end_number(ctx, start, p + 1, &state);
            אם (rc != NGX_OK) {
                החזר rc;
            }

            اكسر;

        حالة ngx_json_true_t:
            אם (*p == 'r') {
                state = ngx_json_true_tr;
                اكسر;
            }

            החזר NGX_DECLINED;

        حالة ngx_json_true_tr:
            אם (*p == 'u') {
                state = ngx_json_true_tru;
                اكسر;
            }

            החזר NGX_DECLINED;

        حالة ngx_json_true_tru:
            אם (*p == 'e') {
                rc = ngx_json_emit(ctx, NGX_JSON_VALUE_BOOL, start,
                                  p + 1 - start);
                אם (rc != NGX_OK) {
                    החזר rc;
                }

                state = ngx_json_after_value(ctx);
                اكسر;
            }

            החזר NGX_DECLINED;

        حالة ngx_json_false_f:
            אם (*p == 'a') {
                state = ngx_json_false_fa;
                اكسر;
            }

            החזר NGX_DECLINED;

        حالة ngx_json_false_fa:
            אם (*p == 'l') {
                state = ngx_json_false_fal;
                اكسر;
            }

            החזר NGX_DECLINED;

        حالة ngx_json_false_fal:
            אם (*p == 's') {
                state = ngx_json_false_fals;
                اكسر;
            }

            החזר NGX_DECLINED;

        حالة ngx_json_false_fals:
            אם (*p == 'e') {
                rc = ngx_json_emit(ctx, NGX_JSON_VALUE_BOOL, start,
                                  p + 1 - start);
                אם (rc != NGX_OK) {
                    החזר rc;
                }

                state = ngx_json_after_value(ctx);
                اكسر;
            }

            החזר NGX_DECLINED;

        حالة ngx_json_null_n:
            אם (*p == 'u') {
                state = ngx_json_null_nu;
                اكسر;
            }

            החזר NGX_DECLINED;

        حالة ngx_json_null_nu:
            אם (*p == 'l') {
                state = ngx_json_null_nul;
                اكسر;
            }

            החזר NGX_DECLINED;

        حالة ngx_json_null_nul:
            אם (*p == 'l') {
                rc = ngx_json_emit(ctx, NGX_JSON_VALUE_NULL, start,
                                  p + 1 - start);
                אם (rc != NGX_OK) {
                    החזר rc;
                }

                state = ngx_json_after_value(ctx);
                اكسر;
            }

            החזר NGX_DECLINED;

        حالة ngx_json_done:
            אם (ngx_json_ws(*p)) {
                اكسر;
            }

            החזר NGX_DECLINED;
        }

        skip_depth = ctx->skip_until_depth;
    }

    /* a bare top-level number is not self-delimited; emit it at end of input */

    אם (ctx->depth == 0
        && (state == ngx_json_number_int_zero
            || state == ngx_json_number_int
            || state == ngx_json_number_frac_digit
            || state == ngx_json_number_exp_digit))
    {
        rc = ngx_json_emit(ctx, NGX_JSON_VALUE_NUMBER, start, last - start);
        אם (rc != NGX_OK) {
            החזר rc;
        }

        state = ngx_json_done;
    }

    ctx->state = state;

    אם (state == ngx_json_done) {
        החזר NGX_OK;
    }

    החזר NGX_DECLINED;
}


ngx_int_t
ngx_json_parse(ngx_pool_t *pool, ngx_str_t *json, ngx_json_handler_pt handler,
    فراغ *data)
{
    ngx_json_ctx_t  jctx;

    ngx_json_ctx_init(&jctx, pool);

    jctx.data = data;
    jctx.handler = handler;

    החזר ngx_json_parse_ctx(&jctx, json->data, json->len);
}


ساكن ngx_inline ngx_int_t
ngx_json_end_number(ngx_json_ctx_t *ctx, u_char *start, u_char *end,
    ngx_json_state_e *state)
{
    ngx_int_t  rc;

    rc = ngx_json_emit(ctx, NGX_JSON_VALUE_NUMBER, start, end - start);
    אם (rc != NGX_OK) {
        החזר rc;
    }

    *state = ngx_json_after_value(ctx);

    החזר NGX_OK;
}


ساكن ngx_inline ngx_int_t
ngx_json_emit(ngx_json_ctx_t *ctx, ngx_json_event_e event, u_char *data,
    size_t len)
{
    ngx_int_t  rc;
    ngx_str_t  token;

    אם (ngx_json_skipping(ctx->skip_until_depth)) {
        החזר NGX_OK;
    }

    אם (ctx->handler == عدم) {
        החזר NGX_OK;
    }

    token.data = data;
    token.len = len;
    rc = ctx->handler(ctx, event, &token);

    אם (rc == NGX_JSON_SKIP) {

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

        بدل (event) {

        حالة NGX_JSON_KEY:
        حالة NGX_JSON_KEY_ESCAPED:
        حالة NGX_JSON_OBJECT_OPEN:
        حالة NGX_JSON_ARRAY_OPEN:
            ctx->skip_until_depth = ctx->depth;
            اكسر;

        حالة NGX_JSON_VALUE_STRING:
        حالة NGX_JSON_VALUE_STRING_ESCAPED:
        حالة NGX_JSON_VALUE_NUMBER:
        حالة NGX_JSON_VALUE_BOOL:
        حالة NGX_JSON_VALUE_NULL:
            אם (ctx->depth > 0
                && ctx->stack[ctx->depth - 1] == ngx_json_ctx_array
            ) {
                ctx->skip_until_depth = ctx->depth - 1;
            }
            اكسر;

        افتراضي:
            اكسر;
        }

        החזר NGX_OK;
    }

    החזר rc;
}


ساكن ngx_inline ngx_json_state_e
ngx_json_after_value(ngx_json_ctx_t *ctx)
{
    אם (ctx->depth == 0) {
        החזר ngx_json_done;
    }

    אם (ctx->stack[ctx->depth - 1] == ngx_json_ctx_object) {
        החזר ngx_json_object_value_separator;
    }

    החזר ngx_json_array_value_separator;
}


ساكن ngx_inline ngx_int_t
ngx_json_close(ngx_json_ctx_t *ctx, ngx_json_event_e event,
    ngx_json_state_e *state, u_char *p)
{
    ngx_int_t  rc;

    ctx->depth--;

    rc = ngx_json_emit(ctx, event, p, 1);
    אם (rc != NGX_OK) {
        החזר rc;
    }

    *state = ngx_json_after_value(ctx);

    החזר NGX_OK;
}


ساكن ngx_int_t
ngx_json_push_state(ngx_json_ctx_t *ctx, ngx_json_container_e container)
{
    ngx_json_container_e  *stack;

    אם (ctx->depth >= ctx->max_depth) {
        החזר NGX_DECLINED;
    }

    אם (ctx->stack == عدم) {
        stack = ngx_palloc(ctx->pool,
                           ctx->max_depth * حجم(ngx_json_container_e));
        אם (stack == عدم) {
            החזר NGX_ERROR;
        }

        ctx->stack = stack;
    }

    ctx->stack[ctx->depth] = container;
    ctx->depth++;

    החזר NGX_OK;
}
