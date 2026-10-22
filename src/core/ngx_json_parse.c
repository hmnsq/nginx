
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


ساكن ngx_inline ngx_int_t ngx_json_end_العدد(ngx_json_ctx_t *ctx,
    u_char *start, u_char *end, ngx_json_state_e *state);
ساكن ngx_inline ngx_int_t ngx_json_emit(ngx_json_ctx_t *ctx,
    ngx_json_event_e event, u_char *data, size_t len);
ساكن ngx_inline ngx_json_state_e ngx_json_after_قيمة(ngx_json_ctx_t *ctx);
ساكن ngx_inline ngx_int_t ngx_json_close(ngx_json_ctx_t *ctx,
    ngx_json_event_e event, ngx_json_state_e *state, u_char *p);
ساكن ngx_int_t ngx_json_ادفع_state(ngx_json_ctx_t *ctx,
    ngx_json_container_e container);


فراغ
ngx_json_ctx_init(ngx_json_ctx_t *ctx, ngx_pool_t *pool)
{
    ctx->state = ngx_json_start;
    ctx->pool = pool;
    ctx->handler = عدم;
    ctx->data = عدم;
    ctx->codepoint = 0;
    ctx->hex_يسار = 0;

    ctx->depth = 0;
    ctx->skip_until_depth = NGX_JSON_NOT_SKIPPING;
    ctx->max_depth = NGX_JSON_DEFAULT_MAX_DEPTH;

    /* stack is allocated lazily on first push, sized to max_depth */

    ctx->مكدس = عدم;

    ctx->in_مفتاح = 0;
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

    إذا (len == 0) {
        إرجاع NGX_DECLINED;
    }

    إذا (data == عدم) {
        إرجاع NGX_ERROR;
    }

    إذا (ctx->state == ngx_json_done) {
        إرجاع NGX_DECLINED;
    }

    last = data + len;
    state = ctx->state;
    skip_depth = ctx->skip_until_depth;
    start = عدم;

    لكل (p = data; p < last; p++) {

        /* skipping suppresses emission only; the machine still validates */

        إذا (ngx_json_skipping(skip_depth) && ctx->depth <= skip_depth
            && (state == ngx_json_object_قيمة_separator
                || state == ngx_json_array_قيمة_separator
                || state == ngx_json_done))
        {
            skip_depth = NGX_JSON_NOT_SKIPPING;
            ctx->skip_until_depth = NGX_JSON_NOT_SKIPPING;
            p--;

            استمر;
        }

        بدل (state) {

        حالة ngx_json_start:
            إذا (ngx_json_ws(*p)) {
                اكسر;
            }

            p--;
            state = ngx_json_قيمة;
            اكسر;

        حالة ngx_json_object:
            إذا (ngx_json_ws(*p)) {
                اكسر;
            }

            إذا (*p == '}') {
                rc = ngx_json_close(ctx, NGX_JSON_OBJECT_CLOSE, &state, p);
                إذا (rc != NGX_OK) {
                    إرجاع rc;
                }

                اكسر;
            }

            /* fall through */

        حالة ngx_json_object_next:
            إذا (ngx_json_ws(*p)) {
                اكسر;
            }

            إذا (*p == '"') {
                ctx->in_مفتاح = 1;
                ctx->escaped = 0;
                start = p + 1;
                state = ngx_json_string;
                اكسر;
            }

            إرجاع NGX_DECLINED;

        حالة ngx_json_object_name_separator:
            إذا (ngx_json_ws(*p)) {
                اكسر;
            }

            إذا (*p == ':') {
                state = ngx_json_قيمة;
                اكسر;
            }

            إرجاع NGX_DECLINED;

        حالة ngx_json_object_قيمة_separator:
            إذا (ngx_json_ws(*p)) {
                اكسر;
            }

            إذا (*p == ',') {
                state = ngx_json_object_next;
                اكسر;
            }

            إذا (*p == '}') {
                rc = ngx_json_close(ctx, NGX_JSON_OBJECT_CLOSE, &state, p);
                إذا (rc != NGX_OK) {
                    إرجاع rc;
                }

                اكسر;
            }

            إرجاع NGX_DECLINED;

        حالة ngx_json_array_قيمة_separator:
            إذا (ngx_json_ws(*p)) {
                اكسر;
            }

            إذا (*p == ',') {
                state = ngx_json_قيمة;
                اكسر;
            }

            إذا (*p == ']') {
                rc = ngx_json_close(ctx, NGX_JSON_ARRAY_CLOSE, &state, p);
                إذا (rc != NGX_OK) {
                    إرجاع rc;
                }

                اكسر;
            }

            إرجاع NGX_DECLINED;

        حالة ngx_json_array:
            إذا (ngx_json_ws(*p)) {
                اكسر;
            }

            إذا (*p == ']') {
                rc = ngx_json_close(ctx, NGX_JSON_ARRAY_CLOSE, &state, p);
                إذا (rc != NGX_OK) {
                    إرجاع rc;
                }

                اكسر;
            }

            /* fall through */

        حالة ngx_json_قيمة:
            إذا (ngx_json_ws(*p)) {
                اكسر;
            }

            start = p;

            إذا (*p == '"') {
                ctx->in_مفتاح = 0;
                ctx->escaped = 0;
                start = p + 1;
                state = ngx_json_string;
                اكسر;
            }

            إذا (*p == '{') {
                rc = ngx_json_emit(ctx, NGX_JSON_OBJECT_OPEN, p, 1);
                إذا (rc != NGX_OK) {
                    إرجاع rc;
                }

                rc = ngx_json_ادفع_state(ctx, ngx_json_ctx_object);
                إذا (rc != NGX_OK) {
                    إرجاع rc;
                }

                state = ngx_json_object;
                اكسر;
            }

            إذا (*p == '[') {
                rc = ngx_json_emit(ctx, NGX_JSON_ARRAY_OPEN, p, 1);
                إذا (rc != NGX_OK) {
                    إرجاع rc;
                }

                rc = ngx_json_ادفع_state(ctx, ngx_json_ctx_array);
                إذا (rc != NGX_OK) {
                    إرجاع rc;
                }

                state = ngx_json_array;
                اكسر;
            }

            إذا (*p == '-') {
                state = ngx_json_العدد_minus;
                اكسر;
            }

            إذا (*p == '0') {
                state = ngx_json_العدد_int_zero;
                اكسر;
            }

            إذا (*p >= '1' && *p <= '9') {
                state = ngx_json_العدد_int;
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

            إرجاع NGX_DECLINED;

        حالة ngx_json_string:
            إذا (*p == '"') {

                إذا (ctx->in_مفتاح) {
                    event = ctx->escaped ? NGX_JSON_مفتاح_ESCAPED
                                         : NGX_JSON_مفتاح;

                    rc = ngx_json_emit(ctx, event, start, p - start);
                    إذا (rc != NGX_OK) {
                        إرجاع rc;
                    }

                    ctx->in_مفتاح = 0;
                    state = ngx_json_object_name_separator;

                } وإلا {
                    event = ctx->escaped ? NGX_JSON_قيمة_STRING_ESCAPED
                                         : NGX_JSON_قيمة_STRING;

                    rc = ngx_json_emit(ctx, event, start, p - start);
                    إذا (rc != NGX_OK) {
                        إرجاع rc;
                    }

                    state = ngx_json_after_قيمة(ctx);
                }

                اكسر;
            }

            إذا (*p == '\\') {
                ctx->escaped = 1;
                state = ngx_json_escaped;
                اكسر;
            }

            إذا (*p < ' ') {
                إرجاع NGX_DECLINED;
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
                ctx->hex_يسار = 4;
                state = ngx_json_escaped_hex;
                اكسر;
            }

            إرجاع NGX_DECLINED;

        حالة ngx_json_escaped_hex:

            /* accumulate the four hex digits of the first \uXXXX escape */

            hd = ngx_json_hex_digit(*p);
            إذا (hd == NGX_ERROR) {
                إرجاع NGX_DECLINED;
            }

            ctx->codepoint = (ctx->codepoint << 4) | (uint32_t) hd;

            إذا (--ctx->hex_يسار > 0) {
                اكسر;
            }

            إذا (ctx->codepoint >= 0xD800 && ctx->codepoint <= 0xDBFF) {
                /* high surrogate - must be followed by \uDC00..\uDFFF */
                state = ngx_json_surrogate_start;
                اكسر;
            }

            إذا (ctx->codepoint >= 0xDC00 && ctx->codepoint <= 0xDFFF) {
                /* lone low surrogate */
                إرجاع NGX_DECLINED;
            }

            state = ngx_json_string;
            اكسر;

        حالة ngx_json_surrogate_start:
            إذا (*p == '\\') {
                state = ngx_json_surrogate_u;
                اكسر;
            }

            /* high surrogate not followed by \uXXXX */
            إرجاع NGX_DECLINED;

        حالة ngx_json_surrogate_u:
            إذا (*p == 'u') {
                ctx->codepoint = 0;
                ctx->hex_يسار = 4;
                state = ngx_json_surrogate_hex;
                اكسر;
            }

            /* high surrogate followed by non-\u escape */
            إرجاع NGX_DECLINED;

        حالة ngx_json_surrogate_hex:

            /* accumulate the four hex digits of the low \uXXXX escape */

            hd = ngx_json_hex_digit(*p);
            إذا (hd == NGX_ERROR) {
                إرجاع NGX_DECLINED;
            }

            ctx->codepoint = (ctx->codepoint << 4) | (uint32_t) hd;

            إذا (--ctx->hex_يسار > 0) {
                اكسر;
            }

            إذا (ctx->codepoint >= 0xDC00 && ctx->codepoint <= 0xDFFF) {
                /* valid surrogate pair */
                state = ngx_json_string;
                اكسر;
            }

            /* high surrogate not followed by low surrogate */
            إرجاع NGX_DECLINED;

        حالة ngx_json_العدد_minus:
            إذا (*p >= '1' && *p <= '9') {
                state = ngx_json_العدد_int;
                اكسر;
            }

            إذا (*p == '0') {
                state = ngx_json_العدد_int_zero;
                اكسر;
            }

            إرجاع NGX_DECLINED;

        حالة ngx_json_العدد_int_zero:
            إذا (*p == '.') {
                state = ngx_json_العدد_frac;
                اكسر;
            }

            إذا (*p == 'e' || *p == 'E') {
                /* No sense, but permitted by RFC */
                state = ngx_json_العدد_exp;
                اكسر;
            }

            p--;

            rc = ngx_json_end_العدد(ctx, start, p + 1, &state);
            إذا (rc != NGX_OK) {
                إرجاع rc;
            }

            اكسر;

        حالة ngx_json_العدد_int:
            إذا (*p >= '0' && *p <= '9') {
                اكسر;
            }

            إذا (*p == '.') {
                state = ngx_json_العدد_frac;
                اكسر;
            }

            إذا (*p == 'e' || *p == 'E') {
                /* No sense, but permitted by RFC */
                state = ngx_json_العدد_exp;
                اكسر;
            }

            p--;

            rc = ngx_json_end_العدد(ctx, start, p + 1, &state);
            إذا (rc!= NGX_OK) {
                إرجاع rc;
            }

            اكسر;

        حالة ngx_json_العدد_frac:
            إذا (*p >= '0' && *p <= '9') {
                state = ngx_json_العدد_frac_digit;
                اكسر;
            }

            إرجاع NGX_DECLINED;

        حالة ngx_json_العدد_frac_digit:
            إذا (*p >= '0' && *p <= '9') {
                اكسر;
            }

            إذا (*p == 'e' || *p == 'E') {
                state = ngx_json_العدد_exp;
                اكسر;
            }

            p--;

            rc = ngx_json_end_العدد(ctx, start, p + 1, &state);
            إذا (rc != NGX_OK) {
                إرجاع rc;
            }

            اكسر;

        حالة ngx_json_العدد_exp:
            إذا (*p == '-' || *p == '+') {
                state = ngx_json_العدد_exp_plusminus;
                اكسر;
            }

            إذا (*p >= '0' && *p <= '9') {
                state = ngx_json_العدد_exp_digit;
                اكسر;
            }

            إرجاع NGX_DECLINED;

        حالة ngx_json_العدد_exp_plusminus:
            إذا (*p >= '0' && *p <= '9') {
                state = ngx_json_العدد_exp_digit;
                اكسر;
            }

            إرجاع NGX_DECLINED;

        حالة ngx_json_العدد_exp_digit:
            إذا (*p >= '0' && *p <= '9') {
                اكسر;
            }

            p--;

            rc = ngx_json_end_العدد(ctx, start, p + 1, &state);
            إذا (rc != NGX_OK) {
                إرجاع rc;
            }

            اكسر;

        حالة ngx_json_true_t:
            إذا (*p == 'r') {
                state = ngx_json_true_tr;
                اكسر;
            }

            إرجاع NGX_DECLINED;

        حالة ngx_json_true_tr:
            إذا (*p == 'u') {
                state = ngx_json_true_tru;
                اكسر;
            }

            إرجاع NGX_DECLINED;

        حالة ngx_json_true_tru:
            إذا (*p == 'e') {
                rc = ngx_json_emit(ctx, NGX_JSON_قيمة_BOOL, start,
                                  p + 1 - start);
                إذا (rc != NGX_OK) {
                    إرجاع rc;
                }

                state = ngx_json_after_قيمة(ctx);
                اكسر;
            }

            إرجاع NGX_DECLINED;

        حالة ngx_json_false_f:
            إذا (*p == 'a') {
                state = ngx_json_false_fa;
                اكسر;
            }

            إرجاع NGX_DECLINED;

        حالة ngx_json_false_fa:
            إذا (*p == 'l') {
                state = ngx_json_false_fal;
                اكسر;
            }

            إرجاع NGX_DECLINED;

        حالة ngx_json_false_fal:
            إذا (*p == 's') {
                state = ngx_json_false_fals;
                اكسر;
            }

            إرجاع NGX_DECLINED;

        حالة ngx_json_false_fals:
            إذا (*p == 'e') {
                rc = ngx_json_emit(ctx, NGX_JSON_قيمة_BOOL, start,
                                  p + 1 - start);
                إذا (rc != NGX_OK) {
                    إرجاع rc;
                }

                state = ngx_json_after_قيمة(ctx);
                اكسر;
            }

            إرجاع NGX_DECLINED;

        حالة ngx_json_null_n:
            إذا (*p == 'u') {
                state = ngx_json_null_nu;
                اكسر;
            }

            إرجاع NGX_DECLINED;

        حالة ngx_json_null_nu:
            إذا (*p == 'l') {
                state = ngx_json_null_nul;
                اكسر;
            }

            إرجاع NGX_DECLINED;

        حالة ngx_json_null_nul:
            إذا (*p == 'l') {
                rc = ngx_json_emit(ctx, NGX_JSON_قيمة_NULL, start,
                                  p + 1 - start);
                إذا (rc != NGX_OK) {
                    إرجاع rc;
                }

                state = ngx_json_after_قيمة(ctx);
                اكسر;
            }

            إرجاع NGX_DECLINED;

        حالة ngx_json_done:
            إذا (ngx_json_ws(*p)) {
                اكسر;
            }

            إرجاع NGX_DECLINED;
        }

        skip_depth = ctx->skip_until_depth;
    }

    /* a bare top-level number is not self-delimited; emit it at end of input */

    إذا (ctx->depth == 0
        && (state == ngx_json_العدد_int_zero
            || state == ngx_json_العدد_int
            || state == ngx_json_العدد_frac_digit
            || state == ngx_json_العدد_exp_digit))
    {
        rc = ngx_json_emit(ctx, NGX_JSON_قيمة_العدد, start, last - start);
        إذا (rc != NGX_OK) {
            إرجاع rc;
        }

        state = ngx_json_done;
    }

    ctx->state = state;

    إذا (state == ngx_json_done) {
        إرجاع NGX_OK;
    }

    إرجاع NGX_DECLINED;
}


ngx_int_t
ngx_json_parse(ngx_pool_t *pool, ngx_str_t *json, ngx_json_handler_pt handler,
    فراغ *data)
{
    ngx_json_ctx_t  jctx;

    ngx_json_ctx_init(&jctx, pool);

    jctx.data = data;
    jctx.handler = handler;

    إرجاع ngx_json_parse_ctx(&jctx, json->data, json->len);
}


ساكن ngx_inline ngx_int_t
ngx_json_end_العدد(ngx_json_ctx_t *ctx, u_char *start, u_char *end,
    ngx_json_state_e *state)
{
    ngx_int_t  rc;

    rc = ngx_json_emit(ctx, NGX_JSON_قيمة_العدد, start, end - start);
    إذا (rc != NGX_OK) {
        إرجاع rc;
    }

    *state = ngx_json_after_قيمة(ctx);

    إرجاع NGX_OK;
}


ساكن ngx_inline ngx_int_t
ngx_json_emit(ngx_json_ctx_t *ctx, ngx_json_event_e event, u_char *data,
    size_t len)
{
    ngx_int_t  rc;
    ngx_str_t  token;

    إذا (ngx_json_skipping(ctx->skip_until_depth)) {
        إرجاع NGX_OK;
    }

    إذا (ctx->handler == عدم) {
        إرجاع NGX_OK;
    }

    token.data = data;
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

        حالة NGX_JSON_مفتاح:
        حالة NGX_JSON_مفتاح_ESCAPED:
        حالة NGX_JSON_OBJECT_OPEN:
        حالة NGX_JSON_ARRAY_OPEN:
            ctx->skip_until_depth = ctx->depth;
            اكسر;

        حالة NGX_JSON_قيمة_STRING:
        حالة NGX_JSON_قيمة_STRING_ESCAPED:
        حالة NGX_JSON_قيمة_العدد:
        حالة NGX_JSON_قيمة_BOOL:
        حالة NGX_JSON_قيمة_NULL:
            إذا (ctx->depth > 0
                && ctx->مكدس[ctx->depth - 1] == ngx_json_ctx_array
            ) {
                ctx->skip_until_depth = ctx->depth - 1;
            }
            اكسر;

        افتراضي:
            اكسر;
        }

        إرجاع NGX_OK;
    }

    إرجاع rc;
}


ساكن ngx_inline ngx_json_state_e
ngx_json_after_قيمة(ngx_json_ctx_t *ctx)
{
    إذا (ctx->depth == 0) {
        إرجاع ngx_json_done;
    }

    إذا (ctx->مكدس[ctx->depth - 1] == ngx_json_ctx_object) {
        إرجاع ngx_json_object_قيمة_separator;
    }

    إرجاع ngx_json_array_قيمة_separator;
}


ساكن ngx_inline ngx_int_t
ngx_json_close(ngx_json_ctx_t *ctx, ngx_json_event_e event,
    ngx_json_state_e *state, u_char *p)
{
    ngx_int_t  rc;

    ctx->depth--;

    rc = ngx_json_emit(ctx, event, p, 1);
    إذا (rc != NGX_OK) {
        إرجاع rc;
    }

    *state = ngx_json_after_قيمة(ctx);

    إرجاع NGX_OK;
}


ساكن ngx_int_t
ngx_json_ادفع_state(ngx_json_ctx_t *ctx, ngx_json_container_e container)
{
    ngx_json_container_e  *مكدس;

    إذا (ctx->depth >= ctx->max_depth) {
        إرجاع NGX_DECLINED;
    }

    إذا (ctx->مكدس == عدم) {
        مكدس = ngx_palloc(ctx->pool,
                           ctx->max_depth * حجم(ngx_json_container_e));
        إذا (مكدس == عدم) {
            إرجاع NGX_ERROR;
        }

        ctx->مكدس = مكدس;
    }

    ctx->مكدس[ctx->depth] = container;
    ctx->depth++;

    إرجاع NGX_OK;
}
