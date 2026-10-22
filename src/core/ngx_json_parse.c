
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


#تعریف_کړه NGX_JSON_NOT_SKIPPING  (ngx_uint_t) -1
#تعریف_کړه ngx_json_skipping(d)   ((d) != NGX_JSON_NOT_SKIPPING)
#تعریف_کړه ngx_json_ws(c)  ((c) == ' ' || (c) == '\t' || (c) == CR || (c) == LF)


ساكن ngx_inline ngx_int_t ngx_json_پای_ټکی_شمېره(ngx_json_ctx_t *ctx,
    u_char *پیل, u_char *پای_ټکی, ngx_json_state_e *state);
ساكن ngx_inline ngx_int_t ngx_json_emit(ngx_json_ctx_t *ctx,
    ngx_json_event_e event, u_char *معلومات, اندازه_t len);
ساكن ngx_inline ngx_json_state_e ngx_json_after_ارزښت(ngx_json_ctx_t *ctx);
ساكن ngx_inline ngx_int_t ngx_json_close(ngx_json_ctx_t *ctx,
    ngx_json_event_e event, ngx_json_state_e *state, u_char *p);
ساكن ngx_int_t ngx_json_ټېل_state(ngx_json_ctx_t *ctx,
    ngx_json_container_e container);


فراغ
ngx_json_ctx_init(ngx_json_ctx_t *ctx, ngx_pool_t *pool)
{
    ctx->state = ngx_json_پیل;
    ctx->pool = pool;
    ctx->handler = عدم;
    ctx->معلومات = عدم;
    ctx->codepoint = 0;
    ctx->hex_کیڼ = 0;

    ctx->ژوروالی = 0;
    ctx->skip_until_ژوروالی = NGX_JSON_NOT_SKIPPING;
    ctx->max_ژوروالی = NGX_JSON_DEFAULT_MAX_ژوروالی;

    /* stack is allocated lazily on first push, sized to max_depth */

    ctx->ډېری = عدم;

    ctx->دننه_کلی = 0;
    ctx->escaped = 0;
}


ngx_int_t
ngx_json_parse_ctx(ngx_json_ctx_t *ctx, u_char *معلومات, اندازه_t len)
{
    u_char            *p, *وروستی, *پیل;
    ngx_int_t          rc, hd;
    ngx_uint_t         skip_ژوروالی;
    ngx_json_event_e   event;
    ngx_json_state_e   state;

    إذا (len == 0) {
        أرجع NGX_DECLINED;
    }

    إذا (معلومات == عدم) {
        أرجع NGX_ERROR;
    }

    إذا (ctx->state == ngx_json_بشپړ) {
        أرجع NGX_DECLINED;
    }

    وروستی = معلومات + len;
    state = ctx->state;
    skip_ژوروالی = ctx->skip_until_ژوروالی;
    پیل = عدم;

    لكل (p = معلومات; p < وروستی; p++) {

        /* skipping suppresses emission only; the machine still validates */

        إذا (ngx_json_skipping(skip_ژوروالی) && ctx->ژوروالی <= skip_ژوروالی
            && (state == ngx_json_object_ارزښت_separator
                || state == ngx_json_array_ارزښت_separator
                || state == ngx_json_بشپړ))
        {
            skip_ژوروالی = NGX_JSON_NOT_SKIPPING;
            ctx->skip_until_ژوروالی = NGX_JSON_NOT_SKIPPING;
            p--;

            استمر;
        }

        بدل (state) {

        حالة ngx_json_پیل:
            إذا (ngx_json_ws(*p)) {
                اكسر;
            }

            p--;
            state = ngx_json_ارزښت;
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

        حالة ngx_json_object_بل:
            إذا (ngx_json_ws(*p)) {
                اكسر;
            }

            إذا (*p == '"') {
                ctx->دننه_کلی = 1;
                ctx->escaped = 0;
                پیل = p + 1;
                state = ngx_json_string;
                اكسر;
            }

            أرجع NGX_DECLINED;

        حالة ngx_json_object_name_separator:
            إذا (ngx_json_ws(*p)) {
                اكسر;
            }

            إذا (*p == ':') {
                state = ngx_json_ارزښت;
                اكسر;
            }

            أرجع NGX_DECLINED;

        حالة ngx_json_object_ارزښت_separator:
            إذا (ngx_json_ws(*p)) {
                اكسر;
            }

            إذا (*p == ',') {
                state = ngx_json_object_بل;
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

        حالة ngx_json_array_ارزښت_separator:
            إذا (ngx_json_ws(*p)) {
                اكسر;
            }

            إذا (*p == ',') {
                state = ngx_json_ارزښت;
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

        حالة ngx_json_ارزښت:
            إذا (ngx_json_ws(*p)) {
                اكسر;
            }

            پیل = p;

            إذا (*p == '"') {
                ctx->دننه_کلی = 0;
                ctx->escaped = 0;
                پیل = p + 1;
                state = ngx_json_string;
                اكسر;
            }

            إذا (*p == '{') {
                rc = ngx_json_emit(ctx, NGX_JSON_OBJECT_OPEN, p, 1);
                إذا (rc != NGX_OK) {
                    أرجع rc;
                }

                rc = ngx_json_ټېل_state(ctx, ngx_json_ctx_object);
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

                rc = ngx_json_ټېل_state(ctx, ngx_json_ctx_array);
                إذا (rc != NGX_OK) {
                    أرجع rc;
                }

                state = ngx_json_array;
                اكسر;
            }

            إذا (*p == '-') {
                state = ngx_json_شمېره_minus;
                اكسر;
            }

            إذا (*p == '0') {
                state = ngx_json_شمېره_int_zero;
                اكسر;
            }

            إذا (*p >= '1' && *p <= '9') {
                state = ngx_json_شمېره_int;
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

                إذا (ctx->دننه_کلی) {
                    event = ctx->escaped ? NGX_JSON_کلی_ESCAPED
                                         : NGX_JSON_کلی;

                    rc = ngx_json_emit(ctx, event, پیل, p - پیل);
                    إذا (rc != NGX_OK) {
                        أرجع rc;
                    }

                    ctx->دننه_کلی = 0;
                    state = ngx_json_object_name_separator;

                } وإلا {
                    event = ctx->escaped ? NGX_JSON_ارزښت_STRING_ESCAPED
                                         : NGX_JSON_ارزښت_STRING;

                    rc = ngx_json_emit(ctx, event, پیل, p - پیل);
                    إذا (rc != NGX_OK) {
                        أرجع rc;
                    }

                    state = ngx_json_after_ارزښت(ctx);
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
                ctx->hex_کیڼ = 4;
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

            إذا (--ctx->hex_کیڼ > 0) {
                اكسر;
            }

            إذا (ctx->codepoint >= 0xD800 && ctx->codepoint <= 0xDBFF) {
                /* high surrogate - must be followed by \uDC00..\uDFFF */
                state = ngx_json_surrogate_پیل;
                اكسر;
            }

            إذا (ctx->codepoint >= 0xDC00 && ctx->codepoint <= 0xDFFF) {
                /* lone low surrogate */
                أرجع NGX_DECLINED;
            }

            state = ngx_json_string;
            اكسر;

        حالة ngx_json_surrogate_پیل:
            إذا (*p == '\\') {
                state = ngx_json_surrogate_u;
                اكسر;
            }

            /* high surrogate not followed by \uXXXX */
            أرجع NGX_DECLINED;

        حالة ngx_json_surrogate_u:
            إذا (*p == 'u') {
                ctx->codepoint = 0;
                ctx->hex_کیڼ = 4;
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

            إذا (--ctx->hex_کیڼ > 0) {
                اكسر;
            }

            إذا (ctx->codepoint >= 0xDC00 && ctx->codepoint <= 0xDFFF) {
                /* valid surrogate pair */
                state = ngx_json_string;
                اكسر;
            }

            /* high surrogate not followed by low surrogate */
            أرجع NGX_DECLINED;

        حالة ngx_json_شمېره_minus:
            إذا (*p >= '1' && *p <= '9') {
                state = ngx_json_شمېره_int;
                اكسر;
            }

            إذا (*p == '0') {
                state = ngx_json_شمېره_int_zero;
                اكسر;
            }

            أرجع NGX_DECLINED;

        حالة ngx_json_شمېره_int_zero:
            إذا (*p == '.') {
                state = ngx_json_شمېره_frac;
                اكسر;
            }

            إذا (*p == 'e' || *p == 'E') {
                /* No sense, but permitted by RFC */
                state = ngx_json_شمېره_exp;
                اكسر;
            }

            p--;

            rc = ngx_json_پای_ټکی_شمېره(ctx, پیل, p + 1, &state);
            إذا (rc != NGX_OK) {
                أرجع rc;
            }

            اكسر;

        حالة ngx_json_شمېره_int:
            إذا (*p >= '0' && *p <= '9') {
                اكسر;
            }

            إذا (*p == '.') {
                state = ngx_json_شمېره_frac;
                اكسر;
            }

            إذا (*p == 'e' || *p == 'E') {
                /* No sense, but permitted by RFC */
                state = ngx_json_شمېره_exp;
                اكسر;
            }

            p--;

            rc = ngx_json_پای_ټکی_شمېره(ctx, پیل, p + 1, &state);
            إذا (rc!= NGX_OK) {
                أرجع rc;
            }

            اكسر;

        حالة ngx_json_شمېره_frac:
            إذا (*p >= '0' && *p <= '9') {
                state = ngx_json_شمېره_frac_digit;
                اكسر;
            }

            أرجع NGX_DECLINED;

        حالة ngx_json_شمېره_frac_digit:
            إذا (*p >= '0' && *p <= '9') {
                اكسر;
            }

            إذا (*p == 'e' || *p == 'E') {
                state = ngx_json_شمېره_exp;
                اكسر;
            }

            p--;

            rc = ngx_json_پای_ټکی_شمېره(ctx, پیل, p + 1, &state);
            إذا (rc != NGX_OK) {
                أرجع rc;
            }

            اكسر;

        حالة ngx_json_شمېره_exp:
            إذا (*p == '-' || *p == '+') {
                state = ngx_json_شمېره_exp_plusminus;
                اكسر;
            }

            إذا (*p >= '0' && *p <= '9') {
                state = ngx_json_شمېره_exp_digit;
                اكسر;
            }

            أرجع NGX_DECLINED;

        حالة ngx_json_شمېره_exp_plusminus:
            إذا (*p >= '0' && *p <= '9') {
                state = ngx_json_شمېره_exp_digit;
                اكسر;
            }

            أرجع NGX_DECLINED;

        حالة ngx_json_شمېره_exp_digit:
            إذا (*p >= '0' && *p <= '9') {
                اكسر;
            }

            p--;

            rc = ngx_json_پای_ټکی_شمېره(ctx, پیل, p + 1, &state);
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
                rc = ngx_json_emit(ctx, NGX_JSON_ارزښت_BOOL, پیل,
                                  p + 1 - پیل);
                إذا (rc != NGX_OK) {
                    أرجع rc;
                }

                state = ngx_json_after_ارزښت(ctx);
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
                rc = ngx_json_emit(ctx, NGX_JSON_ارزښت_BOOL, پیل,
                                  p + 1 - پیل);
                إذا (rc != NGX_OK) {
                    أرجع rc;
                }

                state = ngx_json_after_ارزښت(ctx);
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
                rc = ngx_json_emit(ctx, NGX_JSON_ارزښت_NULL, پیل,
                                  p + 1 - پیل);
                إذا (rc != NGX_OK) {
                    أرجع rc;
                }

                state = ngx_json_after_ارزښت(ctx);
                اكسر;
            }

            أرجع NGX_DECLINED;

        حالة ngx_json_بشپړ:
            إذا (ngx_json_ws(*p)) {
                اكسر;
            }

            أرجع NGX_DECLINED;
        }

        skip_ژوروالی = ctx->skip_until_ژوروالی;
    }

    /* a bare top-level number is not self-delimited; emit it at end of input */

    إذا (ctx->ژوروالی == 0
        && (state == ngx_json_شمېره_int_zero
            || state == ngx_json_شمېره_int
            || state == ngx_json_شمېره_frac_digit
            || state == ngx_json_شمېره_exp_digit))
    {
        rc = ngx_json_emit(ctx, NGX_JSON_ارزښت_شمېره, پیل, وروستی - پیل);
        إذا (rc != NGX_OK) {
            أرجع rc;
        }

        state = ngx_json_بشپړ;
    }

    ctx->state = state;

    إذا (state == ngx_json_بشپړ) {
        أرجع NGX_OK;
    }

    أرجع NGX_DECLINED;
}


ngx_int_t
ngx_json_parse(ngx_pool_t *pool, ngx_str_t *json, ngx_json_handler_pt handler,
    فراغ *معلومات)
{
    ngx_json_ctx_t  jctx;

    ngx_json_ctx_init(&jctx, pool);

    jctx.معلومات = معلومات;
    jctx.handler = handler;

    أرجع ngx_json_parse_ctx(&jctx, json->معلومات, json->len);
}


ساكن ngx_inline ngx_int_t
ngx_json_پای_ټکی_شمېره(ngx_json_ctx_t *ctx, u_char *پیل, u_char *پای_ټکی,
    ngx_json_state_e *state)
{
    ngx_int_t  rc;

    rc = ngx_json_emit(ctx, NGX_JSON_ارزښت_شمېره, پیل, پای_ټکی - پیل);
    إذا (rc != NGX_OK) {
        أرجع rc;
    }

    *state = ngx_json_after_ارزښت(ctx);

    أرجع NGX_OK;
}


ساكن ngx_inline ngx_int_t
ngx_json_emit(ngx_json_ctx_t *ctx, ngx_json_event_e event, u_char *معلومات,
    اندازه_t len)
{
    ngx_int_t  rc;
    ngx_str_t  token;

    إذا (ngx_json_skipping(ctx->skip_until_ژوروالی)) {
        أرجع NGX_OK;
    }

    إذا (ctx->handler == عدم) {
        أرجع NGX_OK;
    }

    token.معلومات = معلومات;
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

        حالة NGX_JSON_کلی:
        حالة NGX_JSON_کلی_ESCAPED:
        حالة NGX_JSON_OBJECT_OPEN:
        حالة NGX_JSON_ARRAY_OPEN:
            ctx->skip_until_ژوروالی = ctx->ژوروالی;
            اكسر;

        حالة NGX_JSON_ارزښت_STRING:
        حالة NGX_JSON_ارزښت_STRING_ESCAPED:
        حالة NGX_JSON_ارزښت_شمېره:
        حالة NGX_JSON_ارزښت_BOOL:
        حالة NGX_JSON_ارزښت_NULL:
            إذا (ctx->ژوروالی > 0
                && ctx->ډېری[ctx->ژوروالی - 1] == ngx_json_ctx_array
            ) {
                ctx->skip_until_ژوروالی = ctx->ژوروالی - 1;
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
ngx_json_after_ارزښت(ngx_json_ctx_t *ctx)
{
    إذا (ctx->ژوروالی == 0) {
        أرجع ngx_json_بشپړ;
    }

    إذا (ctx->ډېری[ctx->ژوروالی - 1] == ngx_json_ctx_object) {
        أرجع ngx_json_object_ارزښت_separator;
    }

    أرجع ngx_json_array_ارزښت_separator;
}


ساكن ngx_inline ngx_int_t
ngx_json_close(ngx_json_ctx_t *ctx, ngx_json_event_e event,
    ngx_json_state_e *state, u_char *p)
{
    ngx_int_t  rc;

    ctx->ژوروالی--;

    rc = ngx_json_emit(ctx, event, p, 1);
    إذا (rc != NGX_OK) {
        أرجع rc;
    }

    *state = ngx_json_after_ارزښت(ctx);

    أرجع NGX_OK;
}


ساكن ngx_int_t
ngx_json_ټېل_state(ngx_json_ctx_t *ctx, ngx_json_container_e container)
{
    ngx_json_container_e  *ډېری;

    إذا (ctx->ژوروالی >= ctx->max_ژوروالی) {
        أرجع NGX_DECLINED;
    }

    إذا (ctx->ډېری == عدم) {
        ډېری = ngx_palloc(ctx->pool,
                           ctx->max_ژوروالی * حجم(ngx_json_container_e));
        إذا (ډېری == عدم) {
            أرجع NGX_ERROR;
        }

        ctx->ډېری = ډېری;
    }

    ctx->ډېری[ctx->ژوروالی] = container;
    ctx->ژوروالی++;

    أرجع NGX_OK;
}
