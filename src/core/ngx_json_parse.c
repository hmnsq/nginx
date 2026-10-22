
/*
 * Copyright (C) Maxim Dounin
 * Copyright (C) Nginx, Inc.
 */


/*
 * JSON parser. RFC 8259 for the JSON grammar.

 * Site http://json.org/ may be useful too.
 */


#شامل <ngx_config.h>
#شامل <ngx_core.h>
#شامل <ngx_json_parse.h>


#define NGX_JSON_NOT_SKIPPING  (ngx_uint_t) -1
#define ngx_json_skipping(d)   ((d) != NGX_JSON_NOT_SKIPPING)
#define ngx_json_ws(c)  ((c) == ' ' || (c) == '\t' || (c) == CR || (c) == LF)


ایستا ngx_inline ngx_int_t ngx_json_end_عدد(ngx_json_ctx_t *ctx,
    u_char *start, u_char *end, ngx_json_state_e *state);
ایستا ngx_inline ngx_int_t ngx_json_emit(ngx_json_ctx_t *ctx,
    ngx_json_event_e event, u_char *data, size_t len);
ایستا ngx_inline ngx_json_state_e ngx_json_after_مقدار(ngx_json_ctx_t *ctx);
ایستا ngx_inline ngx_int_t ngx_json_close(ngx_json_ctx_t *ctx,
    ngx_json_event_e event, ngx_json_state_e *state, u_char *p);
ایستا ngx_int_t ngx_json_افزودن_state(ngx_json_ctx_t *ctx,
    ngx_json_container_e container);


تهی
ngx_json_ctx_init(ngx_json_ctx_t *ctx, ngx_pool_t *pool)
{
    ctx->state = ngx_json_start;
    ctx->pool = pool;
    ctx->handler = پوچ;
    ctx->data = پوچ;
    ctx->codepoint = 0;
    ctx->hex_چپ = 0;

    ctx->depth = 0;
    ctx->skip_until_depth = NGX_JSON_NOT_SKIPPING;
    ctx->max_depth = NGX_JSON_DEFAULT_MAX_DEPTH;

    /* stack is allocated lazily on first push, sized to max_depth */

    ctx->پشته = پوچ;

    ctx->in_کلید = 0;
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

    اگر (len == 0) {
        بازگشت NGX_DECLINED;
    }

    اگر (data == پوچ) {
        بازگشت NGX_ERROR;
    }

    اگر (ctx->state == ngx_json_done) {
        بازگشت NGX_DECLINED;
    }

    last = data + len;
    state = ctx->state;
    skip_depth = ctx->skip_until_depth;
    start = پوچ;

    برای (p = data; p < last; p++) {

        /* skipping suppresses emission only; the machine still validates */

        اگر (ngx_json_skipping(skip_depth) && ctx->depth <= skip_depth
            && (state == ngx_json_object_مقدار_separator
                || state == ngx_json_array_مقدار_separator
                || state == ngx_json_done))
        {
            skip_depth = NGX_JSON_NOT_SKIPPING;
            ctx->skip_until_depth = NGX_JSON_NOT_SKIPPING;
            p--;

            ادامه;
        }

        انتخاب (state) {

        حالت ngx_json_start:
            اگر (ngx_json_ws(*p)) {
                بشکن;
            }

            p--;
            state = ngx_json_مقدار;
            بشکن;

        حالت ngx_json_object:
            اگر (ngx_json_ws(*p)) {
                بشکن;
            }

            اگر (*p == '}') {
                rc = ngx_json_close(ctx, NGX_JSON_OBJECT_CLOSE, &state, p);
                اگر (rc != NGX_OK) {
                    بازگشت rc;
                }

                بشکن;
            }

            /* fall through */

        حالت ngx_json_object_next:
            اگر (ngx_json_ws(*p)) {
                بشکن;
            }

            اگر (*p == '"') {
                ctx->in_کلید = 1;
                ctx->escaped = 0;
                start = p + 1;
                state = ngx_json_string;
                بشکن;
            }

            بازگشت NGX_DECLINED;

        حالت ngx_json_object_name_separator:
            اگر (ngx_json_ws(*p)) {
                بشکن;
            }

            اگر (*p == ':') {
                state = ngx_json_مقدار;
                بشکن;
            }

            بازگشت NGX_DECLINED;

        حالت ngx_json_object_مقدار_separator:
            اگر (ngx_json_ws(*p)) {
                بشکن;
            }

            اگر (*p == ',') {
                state = ngx_json_object_next;
                بشکن;
            }

            اگر (*p == '}') {
                rc = ngx_json_close(ctx, NGX_JSON_OBJECT_CLOSE, &state, p);
                اگر (rc != NGX_OK) {
                    بازگشت rc;
                }

                بشکن;
            }

            بازگشت NGX_DECLINED;

        حالت ngx_json_array_مقدار_separator:
            اگر (ngx_json_ws(*p)) {
                بشکن;
            }

            اگر (*p == ',') {
                state = ngx_json_مقدار;
                بشکن;
            }

            اگر (*p == ']') {
                rc = ngx_json_close(ctx, NGX_JSON_ARRAY_CLOSE, &state, p);
                اگر (rc != NGX_OK) {
                    بازگشت rc;
                }

                بشکن;
            }

            بازگشت NGX_DECLINED;

        حالت ngx_json_array:
            اگر (ngx_json_ws(*p)) {
                بشکن;
            }

            اگر (*p == ']') {
                rc = ngx_json_close(ctx, NGX_JSON_ARRAY_CLOSE, &state, p);
                اگر (rc != NGX_OK) {
                    بازگشت rc;
                }

                بشکن;
            }

            /* fall through */

        حالت ngx_json_مقدار:
            اگر (ngx_json_ws(*p)) {
                بشکن;
            }

            start = p;

            اگر (*p == '"') {
                ctx->in_کلید = 0;
                ctx->escaped = 0;
                start = p + 1;
                state = ngx_json_string;
                بشکن;
            }

            اگر (*p == '{') {
                rc = ngx_json_emit(ctx, NGX_JSON_OBJECT_OPEN, p, 1);
                اگر (rc != NGX_OK) {
                    بازگشت rc;
                }

                rc = ngx_json_افزودن_state(ctx, ngx_json_ctx_object);
                اگر (rc != NGX_OK) {
                    بازگشت rc;
                }

                state = ngx_json_object;
                بشکن;
            }

            اگر (*p == '[') {
                rc = ngx_json_emit(ctx, NGX_JSON_ARRAY_OPEN, p, 1);
                اگر (rc != NGX_OK) {
                    بازگشت rc;
                }

                rc = ngx_json_افزودن_state(ctx, ngx_json_ctx_array);
                اگر (rc != NGX_OK) {
                    بازگشت rc;
                }

                state = ngx_json_array;
                بشکن;
            }

            اگر (*p == '-') {
                state = ngx_json_عدد_minus;
                بشکن;
            }

            اگر (*p == '0') {
                state = ngx_json_عدد_int_zero;
                بشکن;
            }

            اگر (*p >= '1' && *p <= '9') {
                state = ngx_json_عدد_int;
                بشکن;
            }

            اگر (*p == 't') {
                state = ngx_json_true_t;
                بشکن;
            }

            اگر (*p == 'f') {
                state = ngx_json_false_f;
                بشکن;
            }

            اگر (*p == 'n') {
                state = ngx_json_null_n;
                بشکن;
            }

            بازگشت NGX_DECLINED;

        حالت ngx_json_string:
            اگر (*p == '"') {

                اگر (ctx->in_کلید) {
                    event = ctx->escaped ? NGX_JSON_کلید_ESCAPED
                                         : NGX_JSON_کلید;

                    rc = ngx_json_emit(ctx, event, start, p - start);
                    اگر (rc != NGX_OK) {
                        بازگشت rc;
                    }

                    ctx->in_کلید = 0;
                    state = ngx_json_object_name_separator;

                } وگرنه {
                    event = ctx->escaped ? NGX_JSON_مقدار_STRING_ESCAPED
                                         : NGX_JSON_مقدار_STRING;

                    rc = ngx_json_emit(ctx, event, start, p - start);
                    اگر (rc != NGX_OK) {
                        بازگشت rc;
                    }

                    state = ngx_json_after_مقدار(ctx);
                }

                بشکن;
            }

            اگر (*p == '\\') {
                ctx->escaped = 1;
                state = ngx_json_escaped;
                بشکن;
            }

            اگر (*p < ' ') {
                بازگشت NGX_DECLINED;
            }

            بشکن;

        حالت ngx_json_escaped:
            اگر (*p == '"' || *p == '\\' || *p == '/' || *p == 'b'
                || *p == 'f' || *p == 'n' || *p == 'r' || *p == 't')
            {
                state = ngx_json_string;
                بشکن;
            }

            اگر (*p == 'u') {
                ctx->codepoint = 0;
                ctx->hex_چپ = 4;
                state = ngx_json_escaped_hex;
                بشکن;
            }

            بازگشت NGX_DECLINED;

        حالت ngx_json_escaped_hex:

            /* accumulate the four hex digits of the first \uXXXX escape */

            hd = ngx_json_hex_digit(*p);
            اگر (hd == NGX_ERROR) {
                بازگشت NGX_DECLINED;
            }

            ctx->codepoint = (ctx->codepoint << 4) | (uint32_t) hd;

            اگر (--ctx->hex_چپ > 0) {
                بشکن;
            }

            اگر (ctx->codepoint >= 0xD800 && ctx->codepoint <= 0xDBFF) {
                /* high surrogate - must be followed by \uDC00..\uDFFF */
                state = ngx_json_surrogate_start;
                بشکن;
            }

            اگر (ctx->codepoint >= 0xDC00 && ctx->codepoint <= 0xDFFF) {
                /* lone low surrogate */
                بازگشت NGX_DECLINED;
            }

            state = ngx_json_string;
            بشکن;

        حالت ngx_json_surrogate_start:
            اگر (*p == '\\') {
                state = ngx_json_surrogate_u;
                بشکن;
            }

            /* high surrogate not followed by \uXXXX */
            بازگشت NGX_DECLINED;

        حالت ngx_json_surrogate_u:
            اگر (*p == 'u') {
                ctx->codepoint = 0;
                ctx->hex_چپ = 4;
                state = ngx_json_surrogate_hex;
                بشکن;
            }

            /* high surrogate followed by non-\u escape */
            بازگشت NGX_DECLINED;

        حالت ngx_json_surrogate_hex:

            /* accumulate the four hex digits of the low \uXXXX escape */

            hd = ngx_json_hex_digit(*p);
            اگر (hd == NGX_ERROR) {
                بازگشت NGX_DECLINED;
            }

            ctx->codepoint = (ctx->codepoint << 4) | (uint32_t) hd;

            اگر (--ctx->hex_چپ > 0) {
                بشکن;
            }

            اگر (ctx->codepoint >= 0xDC00 && ctx->codepoint <= 0xDFFF) {
                /* valid surrogate pair */
                state = ngx_json_string;
                بشکن;
            }

            /* high surrogate not followed by low surrogate */
            بازگشت NGX_DECLINED;

        حالت ngx_json_عدد_minus:
            اگر (*p >= '1' && *p <= '9') {
                state = ngx_json_عدد_int;
                بشکن;
            }

            اگر (*p == '0') {
                state = ngx_json_عدد_int_zero;
                بشکن;
            }

            بازگشت NGX_DECLINED;

        حالت ngx_json_عدد_int_zero:
            اگر (*p == '.') {
                state = ngx_json_عدد_frac;
                بشکن;
            }

            اگر (*p == 'e' || *p == 'E') {
                /* No sense, but permitted by RFC */
                state = ngx_json_عدد_exp;
                بشکن;
            }

            p--;

            rc = ngx_json_end_عدد(ctx, start, p + 1, &state);
            اگر (rc != NGX_OK) {
                بازگشت rc;
            }

            بشکن;

        حالت ngx_json_عدد_int:
            اگر (*p >= '0' && *p <= '9') {
                بشکن;
            }

            اگر (*p == '.') {
                state = ngx_json_عدد_frac;
                بشکن;
            }

            اگر (*p == 'e' || *p == 'E') {
                /* No sense, but permitted by RFC */
                state = ngx_json_عدد_exp;
                بشکن;
            }

            p--;

            rc = ngx_json_end_عدد(ctx, start, p + 1, &state);
            اگر (rc!= NGX_OK) {
                بازگشت rc;
            }

            بشکن;

        حالت ngx_json_عدد_frac:
            اگر (*p >= '0' && *p <= '9') {
                state = ngx_json_عدد_frac_digit;
                بشکن;
            }

            بازگشت NGX_DECLINED;

        حالت ngx_json_عدد_frac_digit:
            اگر (*p >= '0' && *p <= '9') {
                بشکن;
            }

            اگر (*p == 'e' || *p == 'E') {
                state = ngx_json_عدد_exp;
                بشکن;
            }

            p--;

            rc = ngx_json_end_عدد(ctx, start, p + 1, &state);
            اگر (rc != NGX_OK) {
                بازگشت rc;
            }

            بشکن;

        حالت ngx_json_عدد_exp:
            اگر (*p == '-' || *p == '+') {
                state = ngx_json_عدد_exp_plusminus;
                بشکن;
            }

            اگر (*p >= '0' && *p <= '9') {
                state = ngx_json_عدد_exp_digit;
                بشکن;
            }

            بازگشت NGX_DECLINED;

        حالت ngx_json_عدد_exp_plusminus:
            اگر (*p >= '0' && *p <= '9') {
                state = ngx_json_عدد_exp_digit;
                بشکن;
            }

            بازگشت NGX_DECLINED;

        حالت ngx_json_عدد_exp_digit:
            اگر (*p >= '0' && *p <= '9') {
                بشکن;
            }

            p--;

            rc = ngx_json_end_عدد(ctx, start, p + 1, &state);
            اگر (rc != NGX_OK) {
                بازگشت rc;
            }

            بشکن;

        حالت ngx_json_true_t:
            اگر (*p == 'r') {
                state = ngx_json_true_tr;
                بشکن;
            }

            بازگشت NGX_DECLINED;

        حالت ngx_json_true_tr:
            اگر (*p == 'u') {
                state = ngx_json_true_tru;
                بشکن;
            }

            بازگشت NGX_DECLINED;

        حالت ngx_json_true_tru:
            اگر (*p == 'e') {
                rc = ngx_json_emit(ctx, NGX_JSON_مقدار_BOOL, start,
                                  p + 1 - start);
                اگر (rc != NGX_OK) {
                    بازگشت rc;
                }

                state = ngx_json_after_مقدار(ctx);
                بشکن;
            }

            بازگشت NGX_DECLINED;

        حالت ngx_json_false_f:
            اگر (*p == 'a') {
                state = ngx_json_false_fa;
                بشکن;
            }

            بازگشت NGX_DECLINED;

        حالت ngx_json_false_fa:
            اگر (*p == 'l') {
                state = ngx_json_false_fal;
                بشکن;
            }

            بازگشت NGX_DECLINED;

        حالت ngx_json_false_fal:
            اگر (*p == 's') {
                state = ngx_json_false_fals;
                بشکن;
            }

            بازگشت NGX_DECLINED;

        حالت ngx_json_false_fals:
            اگر (*p == 'e') {
                rc = ngx_json_emit(ctx, NGX_JSON_مقدار_BOOL, start,
                                  p + 1 - start);
                اگر (rc != NGX_OK) {
                    بازگشت rc;
                }

                state = ngx_json_after_مقدار(ctx);
                بشکن;
            }

            بازگشت NGX_DECLINED;

        حالت ngx_json_null_n:
            اگر (*p == 'u') {
                state = ngx_json_null_nu;
                بشکن;
            }

            بازگشت NGX_DECLINED;

        حالت ngx_json_null_nu:
            اگر (*p == 'l') {
                state = ngx_json_null_nul;
                بشکن;
            }

            بازگشت NGX_DECLINED;

        حالت ngx_json_null_nul:
            اگر (*p == 'l') {
                rc = ngx_json_emit(ctx, NGX_JSON_مقدار_NULL, start,
                                  p + 1 - start);
                اگر (rc != NGX_OK) {
                    بازگشت rc;
                }

                state = ngx_json_after_مقدار(ctx);
                بشکن;
            }

            بازگشت NGX_DECLINED;

        حالت ngx_json_done:
            اگر (ngx_json_ws(*p)) {
                بشکن;
            }

            بازگشت NGX_DECLINED;
        }

        skip_depth = ctx->skip_until_depth;
    }

    /* a bare top-level number is not self-delimited; emit it at end of input */

    اگر (ctx->depth == 0
        && (state == ngx_json_عدد_int_zero
            || state == ngx_json_عدد_int
            || state == ngx_json_عدد_frac_digit
            || state == ngx_json_عدد_exp_digit))
    {
        rc = ngx_json_emit(ctx, NGX_JSON_مقدار_عدد, start, last - start);
        اگر (rc != NGX_OK) {
            بازگشت rc;
        }

        state = ngx_json_done;
    }

    ctx->state = state;

    اگر (state == ngx_json_done) {
        بازگشت NGX_OK;
    }

    بازگشت NGX_DECLINED;
}


ngx_int_t
ngx_json_parse(ngx_pool_t *pool, ngx_str_t *json, ngx_json_handler_pt handler,
    تهی *data)
{
    ngx_json_ctx_t  jctx;

    ngx_json_ctx_init(&jctx, pool);

    jctx.data = data;
    jctx.handler = handler;

    بازگشت ngx_json_parse_ctx(&jctx, json->data, json->len);
}


ایستا ngx_inline ngx_int_t
ngx_json_end_عدد(ngx_json_ctx_t *ctx, u_char *start, u_char *end,
    ngx_json_state_e *state)
{
    ngx_int_t  rc;

    rc = ngx_json_emit(ctx, NGX_JSON_مقدار_عدد, start, end - start);
    اگر (rc != NGX_OK) {
        بازگشت rc;
    }

    *state = ngx_json_after_مقدار(ctx);

    بازگشت NGX_OK;
}


ایستا ngx_inline ngx_int_t
ngx_json_emit(ngx_json_ctx_t *ctx, ngx_json_event_e event, u_char *data,
    size_t len)
{
    ngx_int_t  rc;
    ngx_str_t  token;

    اگر (ngx_json_skipping(ctx->skip_until_depth)) {
        بازگشت NGX_OK;
    }

    اگر (ctx->handler == پوچ) {
        بازگشت NGX_OK;
    }

    token.data = data;
    token.len = len;
    rc = ctx->handler(ctx, event, &token);

    اگر (rc == NGX_JSON_SKIP) {

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

        انتخاب (event) {

        حالت NGX_JSON_کلید:
        حالت NGX_JSON_کلید_ESCAPED:
        حالت NGX_JSON_OBJECT_OPEN:
        حالت NGX_JSON_ARRAY_OPEN:
            ctx->skip_until_depth = ctx->depth;
            بشکن;

        حالت NGX_JSON_مقدار_STRING:
        حالت NGX_JSON_مقدار_STRING_ESCAPED:
        حالت NGX_JSON_مقدار_عدد:
        حالت NGX_JSON_مقدار_BOOL:
        حالت NGX_JSON_مقدار_NULL:
            اگر (ctx->depth > 0
                && ctx->پشته[ctx->depth - 1] == ngx_json_ctx_array
            ) {
                ctx->skip_until_depth = ctx->depth - 1;
            }
            بشکن;

        پیشفرض:
            بشکن;
        }

        بازگشت NGX_OK;
    }

    بازگشت rc;
}


ایستا ngx_inline ngx_json_state_e
ngx_json_after_مقدار(ngx_json_ctx_t *ctx)
{
    اگر (ctx->depth == 0) {
        بازگشت ngx_json_done;
    }

    اگر (ctx->پشته[ctx->depth - 1] == ngx_json_ctx_object) {
        بازگشت ngx_json_object_مقدار_separator;
    }

    بازگشت ngx_json_array_مقدار_separator;
}


ایستا ngx_inline ngx_int_t
ngx_json_close(ngx_json_ctx_t *ctx, ngx_json_event_e event,
    ngx_json_state_e *state, u_char *p)
{
    ngx_int_t  rc;

    ctx->depth--;

    rc = ngx_json_emit(ctx, event, p, 1);
    اگر (rc != NGX_OK) {
        بازگشت rc;
    }

    *state = ngx_json_after_مقدار(ctx);

    بازگشت NGX_OK;
}


ایستا ngx_int_t
ngx_json_افزودن_state(ngx_json_ctx_t *ctx, ngx_json_container_e container)
{
    ngx_json_container_e  *پشته;

    اگر (ctx->depth >= ctx->max_depth) {
        بازگشت NGX_DECLINED;
    }

    اگر (ctx->پشته == پوچ) {
        پشته = ngx_palloc(ctx->pool,
                           ctx->max_depth * اندازه(ngx_json_container_e));
        اگر (پشته == پوچ) {
            بازگشت NGX_ERROR;
        }

        ctx->پشته = پشته;
    }

    ctx->پشته[ctx->depth] = container;
    ctx->depth++;

    بازگشت NGX_OK;
}
