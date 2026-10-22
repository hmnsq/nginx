
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


#تعریف_کرو NGX_JSON_NOT_SKIPPING  (ngx_uint_t) -1
#تعریف_کرو ngx_json_skipping(d)   ((d) != NGX_JSON_NOT_SKIPPING)
#تعریف_کرو ngx_json_ws(c)  ((c) == ' ' || (c) == '\t' || (c) == CR || (c) == LF)


ساکن ngx_inline ngx_int_t ngx_json_انتہا_نمبر(ngx_json_ctx_t *ctx,
    u_char *آغاز, u_char *انتہا, ngx_json_state_e *state);
ساکن ngx_inline ngx_int_t ngx_json_emit(ngx_json_ctx_t *ctx,
    ngx_json_event_e event, u_char *ڈیٹا, سائز_t len);
ساکن ngx_inline ngx_json_state_e ngx_json_after_قدر(ngx_json_ctx_t *ctx);
ساکن ngx_inline ngx_int_t ngx_json_close(ngx_json_ctx_t *ctx,
    ngx_json_event_e event, ngx_json_state_e *state, u_char *p);
ساکن ngx_int_t ngx_json_ڈالو_state(ngx_json_ctx_t *ctx,
    ngx_json_container_e container);


خالی
ngx_json_ctx_init(ngx_json_ctx_t *ctx, ngx_pool_t *pool)
{
    ctx->state = ngx_json_آغاز;
    ctx->pool = pool;
    ctx->handler = تہی;
    ctx->ڈیٹا = تہی;
    ctx->codepoint = 0;
    ctx->hex_بایاں = 0;

    ctx->گہرائی = 0;
    ctx->skip_until_گہرائی = NGX_JSON_NOT_SKIPPING;
    ctx->max_گہرائی = NGX_JSON_DEFAULT_MAX_گہرائی;

    /* stack is allocated lazily on first push, sized to max_depth */

    ctx->ڈھیر = تہی;

    ctx->اندر_کلید = 0;
    ctx->escaped = 0;
}


ngx_int_t
ngx_json_parse_ctx(ngx_json_ctx_t *ctx, u_char *ڈیٹا, سائز_t len)
{
    u_char            *p, *آخری, *آغاز;
    ngx_int_t          rc, hd;
    ngx_uint_t         skip_گہرائی;
    ngx_json_event_e   event;
    ngx_json_state_e   state;

    اگر (len == 0) {
        واپس NGX_DECLINED;
    }

    اگر (ڈیٹا == تہی) {
        واپس NGX_ERROR;
    }

    اگر (ctx->state == ngx_json_مکمل) {
        واپس NGX_DECLINED;
    }

    آخری = ڈیٹا + len;
    state = ctx->state;
    skip_گہرائی = ctx->skip_until_گہرائی;
    آغاز = تہی;

    برائے (p = ڈیٹا; p < آخری; p++) {

        /* skipping suppresses emission only; the machine still validates */

        اگر (ngx_json_skipping(skip_گہرائی) && ctx->گہرائی <= skip_گہرائی
            && (state == ngx_json_object_قدر_separator
                || state == ngx_json_array_قدر_separator
                || state == ngx_json_مکمل))
        {
            skip_گہرائی = NGX_JSON_NOT_SKIPPING;
            ctx->skip_until_گہرائی = NGX_JSON_NOT_SKIPPING;
            p--;

            جاری;
        }

        انتخاب (state) {

        حالت ngx_json_آغاز:
            اگر (ngx_json_ws(*p)) {
                توڑو;
            }

            p--;
            state = ngx_json_قدر;
            توڑو;

        حالت ngx_json_object:
            اگر (ngx_json_ws(*p)) {
                توڑو;
            }

            اگر (*p == '}') {
                rc = ngx_json_close(ctx, NGX_JSON_OBJECT_CLOSE, &state, p);
                اگر (rc != NGX_OK) {
                    واپس rc;
                }

                توڑو;
            }

            /* fall through */

        حالت ngx_json_object_اگلا:
            اگر (ngx_json_ws(*p)) {
                توڑو;
            }

            اگر (*p == '"') {
                ctx->اندر_کلید = 1;
                ctx->escaped = 0;
                آغاز = p + 1;
                state = ngx_json_string;
                توڑو;
            }

            واپس NGX_DECLINED;

        حالت ngx_json_object_name_separator:
            اگر (ngx_json_ws(*p)) {
                توڑو;
            }

            اگر (*p == ':') {
                state = ngx_json_قدر;
                توڑو;
            }

            واپس NGX_DECLINED;

        حالت ngx_json_object_قدر_separator:
            اگر (ngx_json_ws(*p)) {
                توڑو;
            }

            اگر (*p == ',') {
                state = ngx_json_object_اگلا;
                توڑو;
            }

            اگر (*p == '}') {
                rc = ngx_json_close(ctx, NGX_JSON_OBJECT_CLOSE, &state, p);
                اگر (rc != NGX_OK) {
                    واپس rc;
                }

                توڑو;
            }

            واپس NGX_DECLINED;

        حالت ngx_json_array_قدر_separator:
            اگر (ngx_json_ws(*p)) {
                توڑو;
            }

            اگر (*p == ',') {
                state = ngx_json_قدر;
                توڑو;
            }

            اگر (*p == ']') {
                rc = ngx_json_close(ctx, NGX_JSON_ARRAY_CLOSE, &state, p);
                اگر (rc != NGX_OK) {
                    واپس rc;
                }

                توڑو;
            }

            واپس NGX_DECLINED;

        حالت ngx_json_array:
            اگر (ngx_json_ws(*p)) {
                توڑو;
            }

            اگر (*p == ']') {
                rc = ngx_json_close(ctx, NGX_JSON_ARRAY_CLOSE, &state, p);
                اگر (rc != NGX_OK) {
                    واپس rc;
                }

                توڑو;
            }

            /* fall through */

        حالت ngx_json_قدر:
            اگر (ngx_json_ws(*p)) {
                توڑو;
            }

            آغاز = p;

            اگر (*p == '"') {
                ctx->اندر_کلید = 0;
                ctx->escaped = 0;
                آغاز = p + 1;
                state = ngx_json_string;
                توڑو;
            }

            اگر (*p == '{') {
                rc = ngx_json_emit(ctx, NGX_JSON_OBJECT_OPEN, p, 1);
                اگر (rc != NGX_OK) {
                    واپس rc;
                }

                rc = ngx_json_ڈالو_state(ctx, ngx_json_ctx_object);
                اگر (rc != NGX_OK) {
                    واپس rc;
                }

                state = ngx_json_object;
                توڑو;
            }

            اگر (*p == '[') {
                rc = ngx_json_emit(ctx, NGX_JSON_ARRAY_OPEN, p, 1);
                اگر (rc != NGX_OK) {
                    واپس rc;
                }

                rc = ngx_json_ڈالو_state(ctx, ngx_json_ctx_array);
                اگر (rc != NGX_OK) {
                    واپس rc;
                }

                state = ngx_json_array;
                توڑو;
            }

            اگر (*p == '-') {
                state = ngx_json_نمبر_minus;
                توڑو;
            }

            اگر (*p == '0') {
                state = ngx_json_نمبر_int_zero;
                توڑو;
            }

            اگر (*p >= '1' && *p <= '9') {
                state = ngx_json_نمبر_int;
                توڑو;
            }

            اگر (*p == 't') {
                state = ngx_json_true_t;
                توڑو;
            }

            اگر (*p == 'f') {
                state = ngx_json_false_f;
                توڑو;
            }

            اگر (*p == 'n') {
                state = ngx_json_null_n;
                توڑو;
            }

            واپس NGX_DECLINED;

        حالت ngx_json_string:
            اگر (*p == '"') {

                اگر (ctx->اندر_کلید) {
                    event = ctx->escaped ? NGX_JSON_کلید_ESCAPED
                                         : NGX_JSON_کلید;

                    rc = ngx_json_emit(ctx, event, آغاز, p - آغاز);
                    اگر (rc != NGX_OK) {
                        واپس rc;
                    }

                    ctx->اندر_کلید = 0;
                    state = ngx_json_object_name_separator;

                } ورنہ {
                    event = ctx->escaped ? NGX_JSON_قدر_STRING_ESCAPED
                                         : NGX_JSON_قدر_STRING;

                    rc = ngx_json_emit(ctx, event, آغاز, p - آغاز);
                    اگر (rc != NGX_OK) {
                        واپس rc;
                    }

                    state = ngx_json_after_قدر(ctx);
                }

                توڑو;
            }

            اگر (*p == '\\') {
                ctx->escaped = 1;
                state = ngx_json_escaped;
                توڑو;
            }

            اگر (*p < ' ') {
                واپس NGX_DECLINED;
            }

            توڑو;

        حالت ngx_json_escaped:
            اگر (*p == '"' || *p == '\\' || *p == '/' || *p == 'b'
                || *p == 'f' || *p == 'n' || *p == 'r' || *p == 't')
            {
                state = ngx_json_string;
                توڑو;
            }

            اگر (*p == 'u') {
                ctx->codepoint = 0;
                ctx->hex_بایاں = 4;
                state = ngx_json_escaped_hex;
                توڑو;
            }

            واپس NGX_DECLINED;

        حالت ngx_json_escaped_hex:

            /* accumulate the four hex digits of the first \uXXXX escape */

            hd = ngx_json_hex_digit(*p);
            اگر (hd == NGX_ERROR) {
                واپس NGX_DECLINED;
            }

            ctx->codepoint = (ctx->codepoint << 4) | (uint32_t) hd;

            اگر (--ctx->hex_بایاں > 0) {
                توڑو;
            }

            اگر (ctx->codepoint >= 0xD800 && ctx->codepoint <= 0xDBFF) {
                /* high surrogate - must be followed by \uDC00..\uDFFF */
                state = ngx_json_surrogate_آغاز;
                توڑو;
            }

            اگر (ctx->codepoint >= 0xDC00 && ctx->codepoint <= 0xDFFF) {
                /* lone low surrogate */
                واپس NGX_DECLINED;
            }

            state = ngx_json_string;
            توڑو;

        حالت ngx_json_surrogate_آغاز:
            اگر (*p == '\\') {
                state = ngx_json_surrogate_u;
                توڑو;
            }

            /* high surrogate not followed by \uXXXX */
            واپس NGX_DECLINED;

        حالت ngx_json_surrogate_u:
            اگر (*p == 'u') {
                ctx->codepoint = 0;
                ctx->hex_بایاں = 4;
                state = ngx_json_surrogate_hex;
                توڑو;
            }

            /* high surrogate followed by non-\u escape */
            واپس NGX_DECLINED;

        حالت ngx_json_surrogate_hex:

            /* accumulate the four hex digits of the low \uXXXX escape */

            hd = ngx_json_hex_digit(*p);
            اگر (hd == NGX_ERROR) {
                واپس NGX_DECLINED;
            }

            ctx->codepoint = (ctx->codepoint << 4) | (uint32_t) hd;

            اگر (--ctx->hex_بایاں > 0) {
                توڑو;
            }

            اگر (ctx->codepoint >= 0xDC00 && ctx->codepoint <= 0xDFFF) {
                /* valid surrogate pair */
                state = ngx_json_string;
                توڑو;
            }

            /* high surrogate not followed by low surrogate */
            واپس NGX_DECLINED;

        حالت ngx_json_نمبر_minus:
            اگر (*p >= '1' && *p <= '9') {
                state = ngx_json_نمبر_int;
                توڑو;
            }

            اگر (*p == '0') {
                state = ngx_json_نمبر_int_zero;
                توڑو;
            }

            واپس NGX_DECLINED;

        حالت ngx_json_نمبر_int_zero:
            اگر (*p == '.') {
                state = ngx_json_نمبر_frac;
                توڑو;
            }

            اگر (*p == 'e' || *p == 'E') {
                /* No sense, but permitted by RFC */
                state = ngx_json_نمبر_exp;
                توڑو;
            }

            p--;

            rc = ngx_json_انتہا_نمبر(ctx, آغاز, p + 1, &state);
            اگر (rc != NGX_OK) {
                واپس rc;
            }

            توڑو;

        حالت ngx_json_نمبر_int:
            اگر (*p >= '0' && *p <= '9') {
                توڑو;
            }

            اگر (*p == '.') {
                state = ngx_json_نمبر_frac;
                توڑو;
            }

            اگر (*p == 'e' || *p == 'E') {
                /* No sense, but permitted by RFC */
                state = ngx_json_نمبر_exp;
                توڑو;
            }

            p--;

            rc = ngx_json_انتہا_نمبر(ctx, آغاز, p + 1, &state);
            اگر (rc!= NGX_OK) {
                واپس rc;
            }

            توڑو;

        حالت ngx_json_نمبر_frac:
            اگر (*p >= '0' && *p <= '9') {
                state = ngx_json_نمبر_frac_digit;
                توڑو;
            }

            واپس NGX_DECLINED;

        حالت ngx_json_نمبر_frac_digit:
            اگر (*p >= '0' && *p <= '9') {
                توڑو;
            }

            اگر (*p == 'e' || *p == 'E') {
                state = ngx_json_نمبر_exp;
                توڑو;
            }

            p--;

            rc = ngx_json_انتہا_نمبر(ctx, آغاز, p + 1, &state);
            اگر (rc != NGX_OK) {
                واپس rc;
            }

            توڑو;

        حالت ngx_json_نمبر_exp:
            اگر (*p == '-' || *p == '+') {
                state = ngx_json_نمبر_exp_plusminus;
                توڑو;
            }

            اگر (*p >= '0' && *p <= '9') {
                state = ngx_json_نمبر_exp_digit;
                توڑو;
            }

            واپس NGX_DECLINED;

        حالت ngx_json_نمبر_exp_plusminus:
            اگر (*p >= '0' && *p <= '9') {
                state = ngx_json_نمبر_exp_digit;
                توڑو;
            }

            واپس NGX_DECLINED;

        حالت ngx_json_نمبر_exp_digit:
            اگر (*p >= '0' && *p <= '9') {
                توڑو;
            }

            p--;

            rc = ngx_json_انتہا_نمبر(ctx, آغاز, p + 1, &state);
            اگر (rc != NGX_OK) {
                واپس rc;
            }

            توڑو;

        حالت ngx_json_true_t:
            اگر (*p == 'r') {
                state = ngx_json_true_tr;
                توڑو;
            }

            واپس NGX_DECLINED;

        حالت ngx_json_true_tr:
            اگر (*p == 'u') {
                state = ngx_json_true_tru;
                توڑو;
            }

            واپس NGX_DECLINED;

        حالت ngx_json_true_tru:
            اگر (*p == 'e') {
                rc = ngx_json_emit(ctx, NGX_JSON_قدر_BOOL, آغاز,
                                  p + 1 - آغاز);
                اگر (rc != NGX_OK) {
                    واپس rc;
                }

                state = ngx_json_after_قدر(ctx);
                توڑو;
            }

            واپس NGX_DECLINED;

        حالت ngx_json_false_f:
            اگر (*p == 'a') {
                state = ngx_json_false_fa;
                توڑو;
            }

            واپس NGX_DECLINED;

        حالت ngx_json_false_fa:
            اگر (*p == 'l') {
                state = ngx_json_false_fal;
                توڑو;
            }

            واپس NGX_DECLINED;

        حالت ngx_json_false_fal:
            اگر (*p == 's') {
                state = ngx_json_false_fals;
                توڑو;
            }

            واپس NGX_DECLINED;

        حالت ngx_json_false_fals:
            اگر (*p == 'e') {
                rc = ngx_json_emit(ctx, NGX_JSON_قدر_BOOL, آغاز,
                                  p + 1 - آغاز);
                اگر (rc != NGX_OK) {
                    واپس rc;
                }

                state = ngx_json_after_قدر(ctx);
                توڑو;
            }

            واپس NGX_DECLINED;

        حالت ngx_json_null_n:
            اگر (*p == 'u') {
                state = ngx_json_null_nu;
                توڑو;
            }

            واپس NGX_DECLINED;

        حالت ngx_json_null_nu:
            اگر (*p == 'l') {
                state = ngx_json_null_nul;
                توڑو;
            }

            واپس NGX_DECLINED;

        حالت ngx_json_null_nul:
            اگر (*p == 'l') {
                rc = ngx_json_emit(ctx, NGX_JSON_قدر_NULL, آغاز,
                                  p + 1 - آغاز);
                اگر (rc != NGX_OK) {
                    واپس rc;
                }

                state = ngx_json_after_قدر(ctx);
                توڑو;
            }

            واپس NGX_DECLINED;

        حالت ngx_json_مکمل:
            اگر (ngx_json_ws(*p)) {
                توڑو;
            }

            واپس NGX_DECLINED;
        }

        skip_گہرائی = ctx->skip_until_گہرائی;
    }

    /* a bare top-level number is not self-delimited; emit it at end of input */

    اگر (ctx->گہرائی == 0
        && (state == ngx_json_نمبر_int_zero
            || state == ngx_json_نمبر_int
            || state == ngx_json_نمبر_frac_digit
            || state == ngx_json_نمبر_exp_digit))
    {
        rc = ngx_json_emit(ctx, NGX_JSON_قدر_نمبر, آغاز, آخری - آغاز);
        اگر (rc != NGX_OK) {
            واپس rc;
        }

        state = ngx_json_مکمل;
    }

    ctx->state = state;

    اگر (state == ngx_json_مکمل) {
        واپس NGX_OK;
    }

    واپس NGX_DECLINED;
}


ngx_int_t
ngx_json_parse(ngx_pool_t *pool, ngx_str_t *json, ngx_json_handler_pt handler,
    خالی *ڈیٹا)
{
    ngx_json_ctx_t  jctx;

    ngx_json_ctx_init(&jctx, pool);

    jctx.ڈیٹا = ڈیٹا;
    jctx.handler = handler;

    واپس ngx_json_parse_ctx(&jctx, json->ڈیٹا, json->len);
}


ساکن ngx_inline ngx_int_t
ngx_json_انتہا_نمبر(ngx_json_ctx_t *ctx, u_char *آغاز, u_char *انتہا,
    ngx_json_state_e *state)
{
    ngx_int_t  rc;

    rc = ngx_json_emit(ctx, NGX_JSON_قدر_نمبر, آغاز, انتہا - آغاز);
    اگر (rc != NGX_OK) {
        واپس rc;
    }

    *state = ngx_json_after_قدر(ctx);

    واپس NGX_OK;
}


ساکن ngx_inline ngx_int_t
ngx_json_emit(ngx_json_ctx_t *ctx, ngx_json_event_e event, u_char *ڈیٹا,
    سائز_t len)
{
    ngx_int_t  rc;
    ngx_str_t  token;

    اگر (ngx_json_skipping(ctx->skip_until_گہرائی)) {
        واپس NGX_OK;
    }

    اگر (ctx->handler == تہی) {
        واپس NGX_OK;
    }

    token.ڈیٹا = ڈیٹا;
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
            ctx->skip_until_گہرائی = ctx->گہرائی;
            توڑو;

        حالت NGX_JSON_قدر_STRING:
        حالت NGX_JSON_قدر_STRING_ESCAPED:
        حالت NGX_JSON_قدر_نمبر:
        حالت NGX_JSON_قدر_BOOL:
        حالت NGX_JSON_قدر_NULL:
            اگر (ctx->گہرائی > 0
                && ctx->ڈھیر[ctx->گہرائی - 1] == ngx_json_ctx_array
            ) {
                ctx->skip_until_گہرائی = ctx->گہرائی - 1;
            }
            توڑو;

        طےشدہ:
            توڑو;
        }

        واپس NGX_OK;
    }

    واپس rc;
}


ساکن ngx_inline ngx_json_state_e
ngx_json_after_قدر(ngx_json_ctx_t *ctx)
{
    اگر (ctx->گہرائی == 0) {
        واپس ngx_json_مکمل;
    }

    اگر (ctx->ڈھیر[ctx->گہرائی - 1] == ngx_json_ctx_object) {
        واپس ngx_json_object_قدر_separator;
    }

    واپس ngx_json_array_قدر_separator;
}


ساکن ngx_inline ngx_int_t
ngx_json_close(ngx_json_ctx_t *ctx, ngx_json_event_e event,
    ngx_json_state_e *state, u_char *p)
{
    ngx_int_t  rc;

    ctx->گہرائی--;

    rc = ngx_json_emit(ctx, event, p, 1);
    اگر (rc != NGX_OK) {
        واپس rc;
    }

    *state = ngx_json_after_قدر(ctx);

    واپس NGX_OK;
}


ساکن ngx_int_t
ngx_json_ڈالو_state(ngx_json_ctx_t *ctx, ngx_json_container_e container)
{
    ngx_json_container_e  *ڈھیر;

    اگر (ctx->گہرائی >= ctx->max_گہرائی) {
        واپس NGX_DECLINED;
    }

    اگر (ctx->ڈھیر == تہی) {
        ڈھیر = ngx_palloc(ctx->pool,
                           ctx->max_گہرائی * حجم(ngx_json_container_e));
        اگر (ڈھیر == تہی) {
            واپس NGX_ERROR;
        }

        ctx->ڈھیر = ڈھیر;
    }

    ctx->ڈھیر[ctx->گہرائی] = container;
    ctx->گہرائی++;

    واپس NGX_OK;
}
