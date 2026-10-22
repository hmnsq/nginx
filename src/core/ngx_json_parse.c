
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


#تعریف_کن NGX_JSON_NOT_SKIPPING  (ngx_uint_t) -1
#تعریف_کن ngx_json_skipping(d)   ((d) != NGX_JSON_NOT_SKIPPING)
#تعریف_کن ngx_json_ws(c)  ((c) == ' ' || (c) == '\t' || (c) == CR || (c) == LF)


ساكن ngx_inline ngx_int_t ngx_json_انتها_عدد(ngx_json_ctx_t *ctx,
    u_char *شروع, u_char *انتها, ngx_json_state_e *state);
ساكن ngx_inline ngx_int_t ngx_json_emit(ngx_json_ctx_t *ctx,
    ngx_json_event_e event, u_char *داده, اندازه_t len);
ساكن ngx_inline ngx_json_state_e ngx_json_after_مقدار(ngx_json_ctx_t *ctx);
ساكن ngx_inline ngx_int_t ngx_json_close(ngx_json_ctx_t *ctx,
    ngx_json_event_e event, ngx_json_state_e *state, u_char *p);
ساكن ngx_int_t ngx_json_افزودن_state(ngx_json_ctx_t *ctx,
    ngx_json_container_e container);


فراغ
ngx_json_ctx_init(ngx_json_ctx_t *ctx, ngx_pool_t *pool)
{
    ctx->state = ngx_json_شروع;
    ctx->pool = pool;
    ctx->handler = عدم;
    ctx->داده = عدم;
    ctx->codepoint = 0;
    ctx->hex_چپ = 0;

    ctx->عمق = 0;
    ctx->skip_until_عمق = NGX_JSON_NOT_SKIPPING;
    ctx->max_عمق = NGX_JSON_DEFAULT_MAX_عمق;

    /* stack is allocated lazily on first push, sized to max_depth */

    ctx->پشته = عدم;

    ctx->درون_کلید = 0;
    ctx->escaped = 0;
}


ngx_int_t
ngx_json_parse_ctx(ngx_json_ctx_t *ctx, u_char *داده, اندازه_t len)
{
    u_char            *p, *آخرین, *شروع;
    ngx_int_t          rc, hd;
    ngx_uint_t         skip_عمق;
    ngx_json_event_e   event;
    ngx_json_state_e   state;

    إذا (len == 0) {
        أرجع NGX_DECLINED;
    }

    إذا (داده == عدم) {
        أرجع NGX_ERROR;
    }

    إذا (ctx->state == ngx_json_انجامشده) {
        أرجع NGX_DECLINED;
    }

    آخرین = داده + len;
    state = ctx->state;
    skip_عمق = ctx->skip_until_عمق;
    شروع = عدم;

    لكل (p = داده; p < آخرین; p++) {

        /* skipping suppresses emission only; the machine still validates */

        إذا (ngx_json_skipping(skip_عمق) && ctx->عمق <= skip_عمق
            && (state == ngx_json_object_مقدار_separator
                || state == ngx_json_array_مقدار_separator
                || state == ngx_json_انجامشده))
        {
            skip_عمق = NGX_JSON_NOT_SKIPPING;
            ctx->skip_until_عمق = NGX_JSON_NOT_SKIPPING;
            p--;

            استمر;
        }

        بدل (state) {

        حالة ngx_json_شروع:
            إذا (ngx_json_ws(*p)) {
                اكسر;
            }

            p--;
            state = ngx_json_مقدار;
            اكسر;

        حالة ngx_json_object:
            إذا (ngx_json_ws(*p)) {
                اكسر;
            }

            إذا (*p == '}') {
                rc = ngx_json_close(ctx, NGX_JSON_OBJECT_CLOSE, &state, p);
                إذا (rc != NGX_OK) {
                    أرجع rc;
                }

                اكسر;
            }

            /* fall through */

        حالة ngx_json_object_بعدی:
            إذا (ngx_json_ws(*p)) {
                اكسر;
            }

            إذا (*p == '"') {
                ctx->درون_کلید = 1;
                ctx->escaped = 0;
                شروع = p + 1;
                state = ngx_json_string;
                اكسر;
            }

            أرجع NGX_DECLINED;

        حالة ngx_json_object_name_separator:
            إذا (ngx_json_ws(*p)) {
                اكسر;
            }

            إذا (*p == ':') {
                state = ngx_json_مقدار;
                اكسر;
            }

            أرجع NGX_DECLINED;

        حالة ngx_json_object_مقدار_separator:
            إذا (ngx_json_ws(*p)) {
                اكسر;
            }

            إذا (*p == ',') {
                state = ngx_json_object_بعدی;
                اكسر;
            }

            إذا (*p == '}') {
                rc = ngx_json_close(ctx, NGX_JSON_OBJECT_CLOSE, &state, p);
                إذا (rc != NGX_OK) {
                    أرجع rc;
                }

                اكسر;
            }

            أرجع NGX_DECLINED;

        حالة ngx_json_array_مقدار_separator:
            إذا (ngx_json_ws(*p)) {
                اكسر;
            }

            إذا (*p == ',') {
                state = ngx_json_مقدار;
                اكسر;
            }

            إذا (*p == ']') {
                rc = ngx_json_close(ctx, NGX_JSON_ARRAY_CLOSE, &state, p);
                إذا (rc != NGX_OK) {
                    أرجع rc;
                }

                اكسر;
            }

            أرجع NGX_DECLINED;

        حالة ngx_json_array:
            إذا (ngx_json_ws(*p)) {
                اكسر;
            }

            إذا (*p == ']') {
                rc = ngx_json_close(ctx, NGX_JSON_ARRAY_CLOSE, &state, p);
                إذا (rc != NGX_OK) {
                    أرجع rc;
                }

                اكسر;
            }

            /* fall through */

        حالة ngx_json_مقدار:
            إذا (ngx_json_ws(*p)) {
                اكسر;
            }

            شروع = p;

            إذا (*p == '"') {
                ctx->درون_کلید = 0;
                ctx->escaped = 0;
                شروع = p + 1;
                state = ngx_json_string;
                اكسر;
            }

            إذا (*p == '{') {
                rc = ngx_json_emit(ctx, NGX_JSON_OBJECT_OPEN, p, 1);
                إذا (rc != NGX_OK) {
                    أرجع rc;
                }

                rc = ngx_json_افزودن_state(ctx, ngx_json_ctx_object);
                إذا (rc != NGX_OK) {
                    أرجع rc;
                }

                state = ngx_json_object;
                اكسر;
            }

            إذا (*p == '[') {
                rc = ngx_json_emit(ctx, NGX_JSON_ARRAY_OPEN, p, 1);
                إذا (rc != NGX_OK) {
                    أرجع rc;
                }

                rc = ngx_json_افزودن_state(ctx, ngx_json_ctx_array);
                إذا (rc != NGX_OK) {
                    أرجع rc;
                }

                state = ngx_json_array;
                اكسر;
            }

            إذا (*p == '-') {
                state = ngx_json_عدد_minus;
                اكسر;
            }

            إذا (*p == '0') {
                state = ngx_json_عدد_int_zero;
                اكسر;
            }

            إذا (*p >= '1' && *p <= '9') {
                state = ngx_json_عدد_int;
                اكسر;
            }

            إذا (*p == 't') {
                state = ngx_json_true_t;
                اكسر;
            }

            إذا (*p == 'f') {
                state = ngx_json_false_f;
                اكسر;
            }

            إذا (*p == 'n') {
                state = ngx_json_null_n;
                اكسر;
            }

            أرجع NGX_DECLINED;

        حالة ngx_json_string:
            إذا (*p == '"') {

                إذا (ctx->درون_کلید) {
                    event = ctx->escaped ? NGX_JSON_کلید_ESCAPED
                                         : NGX_JSON_کلید;

                    rc = ngx_json_emit(ctx, event, شروع, p - شروع);
                    إذا (rc != NGX_OK) {
                        أرجع rc;
                    }

                    ctx->درون_کلید = 0;
                    state = ngx_json_object_name_separator;

                } وإلا {
                    event = ctx->escaped ? NGX_JSON_مقدار_STRING_ESCAPED
                                         : NGX_JSON_مقدار_STRING;

                    rc = ngx_json_emit(ctx, event, شروع, p - شروع);
                    إذا (rc != NGX_OK) {
                        أرجع rc;
                    }

                    state = ngx_json_after_مقدار(ctx);
                }

                اكسر;
            }

            إذا (*p == '\\') {
                ctx->escaped = 1;
                state = ngx_json_escaped;
                اكسر;
            }

            إذا (*p < ' ') {
                أرجع NGX_DECLINED;
            }

            اكسر;

        حالة ngx_json_escaped:
            إذا (*p == '"' || *p == '\\' || *p == '/' || *p == 'b'
                || *p == 'f' || *p == 'n' || *p == 'r' || *p == 't')
            {
                state = ngx_json_string;
                اكسر;
            }

            إذا (*p == 'u') {
                ctx->codepoint = 0;
                ctx->hex_چپ = 4;
                state = ngx_json_escaped_hex;
                اكسر;
            }

            أرجع NGX_DECLINED;

        حالة ngx_json_escaped_hex:

            /* accumulate the four hex digits of the first \uXXXX escape */

            hd = ngx_json_hex_digit(*p);
            إذا (hd == NGX_ERROR) {
                أرجع NGX_DECLINED;
            }

            ctx->codepoint = (ctx->codepoint << 4) | (uint32_t) hd;

            إذا (--ctx->hex_چپ > 0) {
                اكسر;
            }

            إذا (ctx->codepoint >= 0xD800 && ctx->codepoint <= 0xDBFF) {
                /* high surrogate - must be followed by \uDC00..\uDFFF */
                state = ngx_json_surrogate_شروع;
                اكسر;
            }

            إذا (ctx->codepoint >= 0xDC00 && ctx->codepoint <= 0xDFFF) {
                /* lone low surrogate */
                أرجع NGX_DECLINED;
            }

            state = ngx_json_string;
            اكسر;

        حالة ngx_json_surrogate_شروع:
            إذا (*p == '\\') {
                state = ngx_json_surrogate_u;
                اكسر;
            }

            /* high surrogate not followed by \uXXXX */
            أرجع NGX_DECLINED;

        حالة ngx_json_surrogate_u:
            إذا (*p == 'u') {
                ctx->codepoint = 0;
                ctx->hex_چپ = 4;
                state = ngx_json_surrogate_hex;
                اكسر;
            }

            /* high surrogate followed by non-\u escape */
            أرجع NGX_DECLINED;

        حالة ngx_json_surrogate_hex:

            /* accumulate the four hex digits of the low \uXXXX escape */

            hd = ngx_json_hex_digit(*p);
            إذا (hd == NGX_ERROR) {
                أرجع NGX_DECLINED;
            }

            ctx->codepoint = (ctx->codepoint << 4) | (uint32_t) hd;

            إذا (--ctx->hex_چپ > 0) {
                اكسر;
            }

            إذا (ctx->codepoint >= 0xDC00 && ctx->codepoint <= 0xDFFF) {
                /* valid surrogate pair */
                state = ngx_json_string;
                اكسر;
            }

            /* high surrogate not followed by low surrogate */
            أرجع NGX_DECLINED;

        حالة ngx_json_عدد_minus:
            إذا (*p >= '1' && *p <= '9') {
                state = ngx_json_عدد_int;
                اكسر;
            }

            إذا (*p == '0') {
                state = ngx_json_عدد_int_zero;
                اكسر;
            }

            أرجع NGX_DECLINED;

        حالة ngx_json_عدد_int_zero:
            إذا (*p == '.') {
                state = ngx_json_عدد_frac;
                اكسر;
            }

            إذا (*p == 'e' || *p == 'E') {
                /* No sense, but permitted by RFC */
                state = ngx_json_عدد_exp;
                اكسر;
            }

            p--;

            rc = ngx_json_انتها_عدد(ctx, شروع, p + 1, &state);
            إذا (rc != NGX_OK) {
                أرجع rc;
            }

            اكسر;

        حالة ngx_json_عدد_int:
            إذا (*p >= '0' && *p <= '9') {
                اكسر;
            }

            إذا (*p == '.') {
                state = ngx_json_عدد_frac;
                اكسر;
            }

            إذا (*p == 'e' || *p == 'E') {
                /* No sense, but permitted by RFC */
                state = ngx_json_عدد_exp;
                اكسر;
            }

            p--;

            rc = ngx_json_انتها_عدد(ctx, شروع, p + 1, &state);
            إذا (rc!= NGX_OK) {
                أرجع rc;
            }

            اكسر;

        حالة ngx_json_عدد_frac:
            إذا (*p >= '0' && *p <= '9') {
                state = ngx_json_عدد_frac_digit;
                اكسر;
            }

            أرجع NGX_DECLINED;

        حالة ngx_json_عدد_frac_digit:
            إذا (*p >= '0' && *p <= '9') {
                اكسر;
            }

            إذا (*p == 'e' || *p == 'E') {
                state = ngx_json_عدد_exp;
                اكسر;
            }

            p--;

            rc = ngx_json_انتها_عدد(ctx, شروع, p + 1, &state);
            إذا (rc != NGX_OK) {
                أرجع rc;
            }

            اكسر;

        حالة ngx_json_عدد_exp:
            إذا (*p == '-' || *p == '+') {
                state = ngx_json_عدد_exp_plusminus;
                اكسر;
            }

            إذا (*p >= '0' && *p <= '9') {
                state = ngx_json_عدد_exp_digit;
                اكسر;
            }

            أرجع NGX_DECLINED;

        حالة ngx_json_عدد_exp_plusminus:
            إذا (*p >= '0' && *p <= '9') {
                state = ngx_json_عدد_exp_digit;
                اكسر;
            }

            أرجع NGX_DECLINED;

        حالة ngx_json_عدد_exp_digit:
            إذا (*p >= '0' && *p <= '9') {
                اكسر;
            }

            p--;

            rc = ngx_json_انتها_عدد(ctx, شروع, p + 1, &state);
            إذا (rc != NGX_OK) {
                أرجع rc;
            }

            اكسر;

        حالة ngx_json_true_t:
            إذا (*p == 'r') {
                state = ngx_json_true_tr;
                اكسر;
            }

            أرجع NGX_DECLINED;

        حالة ngx_json_true_tr:
            إذا (*p == 'u') {
                state = ngx_json_true_tru;
                اكسر;
            }

            أرجع NGX_DECLINED;

        حالة ngx_json_true_tru:
            إذا (*p == 'e') {
                rc = ngx_json_emit(ctx, NGX_JSON_مقدار_BOOL, شروع,
                                  p + 1 - شروع);
                إذا (rc != NGX_OK) {
                    أرجع rc;
                }

                state = ngx_json_after_مقدار(ctx);
                اكسر;
            }

            أرجع NGX_DECLINED;

        حالة ngx_json_false_f:
            إذا (*p == 'a') {
                state = ngx_json_false_fa;
                اكسر;
            }

            أرجع NGX_DECLINED;

        حالة ngx_json_false_fa:
            إذا (*p == 'l') {
                state = ngx_json_false_fal;
                اكسر;
            }

            أرجع NGX_DECLINED;

        حالة ngx_json_false_fal:
            إذا (*p == 's') {
                state = ngx_json_false_fals;
                اكسر;
            }

            أرجع NGX_DECLINED;

        حالة ngx_json_false_fals:
            إذا (*p == 'e') {
                rc = ngx_json_emit(ctx, NGX_JSON_مقدار_BOOL, شروع,
                                  p + 1 - شروع);
                إذا (rc != NGX_OK) {
                    أرجع rc;
                }

                state = ngx_json_after_مقدار(ctx);
                اكسر;
            }

            أرجع NGX_DECLINED;

        حالة ngx_json_null_n:
            إذا (*p == 'u') {
                state = ngx_json_null_nu;
                اكسر;
            }

            أرجع NGX_DECLINED;

        حالة ngx_json_null_nu:
            إذا (*p == 'l') {
                state = ngx_json_null_nul;
                اكسر;
            }

            أرجع NGX_DECLINED;

        حالة ngx_json_null_nul:
            إذا (*p == 'l') {
                rc = ngx_json_emit(ctx, NGX_JSON_مقدار_NULL, شروع,
                                  p + 1 - شروع);
                إذا (rc != NGX_OK) {
                    أرجع rc;
                }

                state = ngx_json_after_مقدار(ctx);
                اكسر;
            }

            أرجع NGX_DECLINED;

        حالة ngx_json_انجامشده:
            إذا (ngx_json_ws(*p)) {
                اكسر;
            }

            أرجع NGX_DECLINED;
        }

        skip_عمق = ctx->skip_until_عمق;
    }

    /* a bare top-level number is not self-delimited; emit it at end of input */

    إذا (ctx->عمق == 0
        && (state == ngx_json_عدد_int_zero
            || state == ngx_json_عدد_int
            || state == ngx_json_عدد_frac_digit
            || state == ngx_json_عدد_exp_digit))
    {
        rc = ngx_json_emit(ctx, NGX_JSON_مقدار_عدد, شروع, آخرین - شروع);
        إذا (rc != NGX_OK) {
            أرجع rc;
        }

        state = ngx_json_انجامشده;
    }

    ctx->state = state;

    إذا (state == ngx_json_انجامشده) {
        أرجع NGX_OK;
    }

    أرجع NGX_DECLINED;
}


ngx_int_t
ngx_json_parse(ngx_pool_t *pool, ngx_str_t *json, ngx_json_handler_pt handler,
    فراغ *داده)
{
    ngx_json_ctx_t  jctx;

    ngx_json_ctx_init(&jctx, pool);

    jctx.داده = داده;
    jctx.handler = handler;

    أرجع ngx_json_parse_ctx(&jctx, json->داده, json->len);
}


ساكن ngx_inline ngx_int_t
ngx_json_انتها_عدد(ngx_json_ctx_t *ctx, u_char *شروع, u_char *انتها,
    ngx_json_state_e *state)
{
    ngx_int_t  rc;

    rc = ngx_json_emit(ctx, NGX_JSON_مقدار_عدد, شروع, انتها - شروع);
    إذا (rc != NGX_OK) {
        أرجع rc;
    }

    *state = ngx_json_after_مقدار(ctx);

    أرجع NGX_OK;
}


ساكن ngx_inline ngx_int_t
ngx_json_emit(ngx_json_ctx_t *ctx, ngx_json_event_e event, u_char *داده,
    اندازه_t len)
{
    ngx_int_t  rc;
    ngx_str_t  token;

    إذا (ngx_json_skipping(ctx->skip_until_عمق)) {
        أرجع NGX_OK;
    }

    إذا (ctx->handler == عدم) {
        أرجع NGX_OK;
    }

    token.داده = داده;
    token.len = len;
    rc = ctx->handler(ctx, event, &token);

    إذا (rc == NGX_JSON_SKIP) {

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

        حالة NGX_JSON_کلید:
        حالة NGX_JSON_کلید_ESCAPED:
        حالة NGX_JSON_OBJECT_OPEN:
        حالة NGX_JSON_ARRAY_OPEN:
            ctx->skip_until_عمق = ctx->عمق;
            اكسر;

        حالة NGX_JSON_مقدار_STRING:
        حالة NGX_JSON_مقدار_STRING_ESCAPED:
        حالة NGX_JSON_مقدار_عدد:
        حالة NGX_JSON_مقدار_BOOL:
        حالة NGX_JSON_مقدار_NULL:
            إذا (ctx->عمق > 0
                && ctx->پشته[ctx->عمق - 1] == ngx_json_ctx_array
            ) {
                ctx->skip_until_عمق = ctx->عمق - 1;
            }
            اكسر;

        افتراضي:
            اكسر;
        }

        أرجع NGX_OK;
    }

    أرجع rc;
}


ساكن ngx_inline ngx_json_state_e
ngx_json_after_مقدار(ngx_json_ctx_t *ctx)
{
    إذا (ctx->عمق == 0) {
        أرجع ngx_json_انجامشده;
    }

    إذا (ctx->پشته[ctx->عمق - 1] == ngx_json_ctx_object) {
        أرجع ngx_json_object_مقدار_separator;
    }

    أرجع ngx_json_array_مقدار_separator;
}


ساكن ngx_inline ngx_int_t
ngx_json_close(ngx_json_ctx_t *ctx, ngx_json_event_e event,
    ngx_json_state_e *state, u_char *p)
{
    ngx_int_t  rc;

    ctx->عمق--;

    rc = ngx_json_emit(ctx, event, p, 1);
    إذا (rc != NGX_OK) {
        أرجع rc;
    }

    *state = ngx_json_after_مقدار(ctx);

    أرجع NGX_OK;
}


ساكن ngx_int_t
ngx_json_افزودن_state(ngx_json_ctx_t *ctx, ngx_json_container_e container)
{
    ngx_json_container_e  *پشته;

    إذا (ctx->عمق >= ctx->max_عمق) {
        أرجع NGX_DECLINED;
    }

    إذا (ctx->پشته == عدم) {
        پشته = ngx_palloc(ctx->pool,
                           ctx->max_عمق * حجم(ngx_json_container_e));
        إذا (پشته == عدم) {
            أرجع NGX_ERROR;
        }

        ctx->پشته = پشته;
    }

    ctx->پشته[ctx->عمق] = container;
    ctx->عمق++;

    أرجع NGX_OK;
}
