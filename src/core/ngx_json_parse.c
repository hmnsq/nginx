
/*
 * Copyright (C) Maxim Dounin
 * Copyright (C) Nginx, Inc.
 */


/*
 * JSON parser. RFC 8259 for the JSON grammar.

 * Site http://json.org/ may be useful too.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>
#अन्तर्भाव <ngx_json_parse.h>


#define NGX_JSON_NOT_SKIPPING  (ngx_uint_t) -1
#define ngx_json_skipping(d)   ((d) != NGX_JSON_NOT_SKIPPING)
#define ngx_json_ws(c)  ((c) == ' ' || (c) == '\t' || (c) == CR || (c) == LF)


स्थिर ngx_inline ngx_int_t ngx_json_end_सङ्ख्या(ngx_json_ctx_t *ctx,
    u_char *start, u_char *end, ngx_json_state_e *state);
स्थिर ngx_inline ngx_int_t ngx_json_emit(ngx_json_ctx_t *ctx,
    ngx_json_event_e event, u_char *data, size_t len);
स्थिर ngx_inline ngx_json_state_e ngx_json_after_मानम्(ngx_json_ctx_t *ctx);
स्थिर ngx_inline ngx_int_t ngx_json_close(ngx_json_ctx_t *ctx,
    ngx_json_event_e event, ngx_json_state_e *state, u_char *p);
स्थिर ngx_int_t ngx_json_प्रक्षिप_state(ngx_json_ctx_t *ctx,
    ngx_json_container_e container);


शून्य
ngx_json_ctx_init(ngx_json_ctx_t *ctx, ngx_pool_t *pool)
{
    ctx->state = ngx_json_start;
    ctx->pool = pool;
    ctx->handler = रिक्त;
    ctx->data = रिक्त;
    ctx->codepoint = 0;
    ctx->hex_वामः = 0;

    ctx->depth = 0;
    ctx->skip_until_depth = NGX_JSON_NOT_SKIPPING;
    ctx->max_depth = NGX_JSON_DEFAULT_MAX_DEPTH;

    /* stack is allocated lazily on first push, sized to max_depth */

    ctx->राशिः = रिक्त;

    ctx->in_कुञ्चिका = 0;
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

    जइ (len == 0) {
        प्रत्यागम NGX_DECLINED;
    }

    जइ (data == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

    जइ (ctx->state == ngx_json_done) {
        प्रत्यागम NGX_DECLINED;
    }

    last = data + len;
    state = ctx->state;
    skip_depth = ctx->skip_until_depth;
    start = रिक्त;

    प्रति (p = data; p < last; p++) {

        /* skipping suppresses emission only; the machine still validates */

        जइ (ngx_json_skipping(skip_depth) && ctx->depth <= skip_depth
            && (state == ngx_json_object_मानम्_separator
                || state == ngx_json_array_मानम्_separator
                || state == ngx_json_done))
        {
            skip_depth = NGX_JSON_NOT_SKIPPING;
            ctx->skip_until_depth = NGX_JSON_NOT_SKIPPING;
            p--;

            अनुवर्त;
        }

        विकल्प (state) {

        पक्ष ngx_json_start:
            जइ (ngx_json_ws(*p)) {
                भङ्ग;
            }

            p--;
            state = ngx_json_मानम्;
            भङ्ग;

        पक्ष ngx_json_object:
            जइ (ngx_json_ws(*p)) {
                भङ्ग;
            }

            जइ (*p == '}') {
                rc = ngx_json_close(ctx, NGX_JSON_OBJECT_CLOSE, &state, p);
                जइ (rc != NGX_OK) {
                    प्रत्यागम rc;
                }

                भङ्ग;
            }

            /* fall through */

        पक्ष ngx_json_object_next:
            जइ (ngx_json_ws(*p)) {
                भङ्ग;
            }

            जइ (*p == '"') {
                ctx->in_कुञ्चिका = 1;
                ctx->escaped = 0;
                start = p + 1;
                state = ngx_json_string;
                भङ्ग;
            }

            प्रत्यागम NGX_DECLINED;

        पक्ष ngx_json_object_name_separator:
            जइ (ngx_json_ws(*p)) {
                भङ्ग;
            }

            जइ (*p == ':') {
                state = ngx_json_मानम्;
                भङ्ग;
            }

            प्रत्यागम NGX_DECLINED;

        पक्ष ngx_json_object_मानम्_separator:
            जइ (ngx_json_ws(*p)) {
                भङ्ग;
            }

            जइ (*p == ',') {
                state = ngx_json_object_next;
                भङ्ग;
            }

            जइ (*p == '}') {
                rc = ngx_json_close(ctx, NGX_JSON_OBJECT_CLOSE, &state, p);
                जइ (rc != NGX_OK) {
                    प्रत्यागम rc;
                }

                भङ्ग;
            }

            प्रत्यागम NGX_DECLINED;

        पक्ष ngx_json_array_मानम्_separator:
            जइ (ngx_json_ws(*p)) {
                भङ्ग;
            }

            जइ (*p == ',') {
                state = ngx_json_मानम्;
                भङ्ग;
            }

            जइ (*p == ']') {
                rc = ngx_json_close(ctx, NGX_JSON_ARRAY_CLOSE, &state, p);
                जइ (rc != NGX_OK) {
                    प्रत्यागम rc;
                }

                भङ्ग;
            }

            प्रत्यागम NGX_DECLINED;

        पक्ष ngx_json_array:
            जइ (ngx_json_ws(*p)) {
                भङ्ग;
            }

            जइ (*p == ']') {
                rc = ngx_json_close(ctx, NGX_JSON_ARRAY_CLOSE, &state, p);
                जइ (rc != NGX_OK) {
                    प्रत्यागम rc;
                }

                भङ्ग;
            }

            /* fall through */

        पक्ष ngx_json_मानम्:
            जइ (ngx_json_ws(*p)) {
                भङ्ग;
            }

            start = p;

            जइ (*p == '"') {
                ctx->in_कुञ्चिका = 0;
                ctx->escaped = 0;
                start = p + 1;
                state = ngx_json_string;
                भङ्ग;
            }

            जइ (*p == '{') {
                rc = ngx_json_emit(ctx, NGX_JSON_OBJECT_OPEN, p, 1);
                जइ (rc != NGX_OK) {
                    प्रत्यागम rc;
                }

                rc = ngx_json_प्रक्षिप_state(ctx, ngx_json_ctx_object);
                जइ (rc != NGX_OK) {
                    प्रत्यागम rc;
                }

                state = ngx_json_object;
                भङ्ग;
            }

            जइ (*p == '[') {
                rc = ngx_json_emit(ctx, NGX_JSON_ARRAY_OPEN, p, 1);
                जइ (rc != NGX_OK) {
                    प्रत्यागम rc;
                }

                rc = ngx_json_प्रक्षिप_state(ctx, ngx_json_ctx_array);
                जइ (rc != NGX_OK) {
                    प्रत्यागम rc;
                }

                state = ngx_json_array;
                भङ्ग;
            }

            जइ (*p == '-') {
                state = ngx_json_सङ्ख्या_minus;
                भङ्ग;
            }

            जइ (*p == '0') {
                state = ngx_json_सङ्ख्या_int_zero;
                भङ्ग;
            }

            जइ (*p >= '1' && *p <= '9') {
                state = ngx_json_सङ्ख्या_int;
                भङ्ग;
            }

            जइ (*p == 't') {
                state = ngx_json_true_t;
                भङ्ग;
            }

            जइ (*p == 'f') {
                state = ngx_json_false_f;
                भङ्ग;
            }

            जइ (*p == 'n') {
                state = ngx_json_null_n;
                भङ्ग;
            }

            प्रत्यागम NGX_DECLINED;

        पक्ष ngx_json_string:
            जइ (*p == '"') {

                जइ (ctx->in_कुञ्चिका) {
                    event = ctx->escaped ? NGX_JSON_कुञ्चिका_ESCAPED
                                         : NGX_JSON_कुञ्चिका;

                    rc = ngx_json_emit(ctx, event, start, p - start);
                    जइ (rc != NGX_OK) {
                        प्रत्यागम rc;
                    }

                    ctx->in_कुञ्चिका = 0;
                    state = ngx_json_object_name_separator;

                } अण्णहा {
                    event = ctx->escaped ? NGX_JSON_मानम्_STRING_ESCAPED
                                         : NGX_JSON_मानम्_STRING;

                    rc = ngx_json_emit(ctx, event, start, p - start);
                    जइ (rc != NGX_OK) {
                        प्रत्यागम rc;
                    }

                    state = ngx_json_after_मानम्(ctx);
                }

                भङ्ग;
            }

            जइ (*p == '\\') {
                ctx->escaped = 1;
                state = ngx_json_escaped;
                भङ्ग;
            }

            जइ (*p < ' ') {
                प्रत्यागम NGX_DECLINED;
            }

            भङ्ग;

        पक्ष ngx_json_escaped:
            जइ (*p == '"' || *p == '\\' || *p == '/' || *p == 'b'
                || *p == 'f' || *p == 'n' || *p == 'r' || *p == 't')
            {
                state = ngx_json_string;
                भङ्ग;
            }

            जइ (*p == 'u') {
                ctx->codepoint = 0;
                ctx->hex_वामः = 4;
                state = ngx_json_escaped_hex;
                भङ्ग;
            }

            प्रत्यागम NGX_DECLINED;

        पक्ष ngx_json_escaped_hex:

            /* accumulate the four hex digits of the first \uXXXX escape */

            hd = ngx_json_hex_digit(*p);
            जइ (hd == NGX_ERROR) {
                प्रत्यागम NGX_DECLINED;
            }

            ctx->codepoint = (ctx->codepoint << 4) | (uint32_t) hd;

            जइ (--ctx->hex_वामः > 0) {
                भङ्ग;
            }

            जइ (ctx->codepoint >= 0xD800 && ctx->codepoint <= 0xDBFF) {
                /* high surrogate - must be followed by \uDC00..\uDFFF */
                state = ngx_json_surrogate_start;
                भङ्ग;
            }

            जइ (ctx->codepoint >= 0xDC00 && ctx->codepoint <= 0xDFFF) {
                /* lone low surrogate */
                प्रत्यागम NGX_DECLINED;
            }

            state = ngx_json_string;
            भङ्ग;

        पक्ष ngx_json_surrogate_start:
            जइ (*p == '\\') {
                state = ngx_json_surrogate_u;
                भङ्ग;
            }

            /* high surrogate not followed by \uXXXX */
            प्रत्यागम NGX_DECLINED;

        पक्ष ngx_json_surrogate_u:
            जइ (*p == 'u') {
                ctx->codepoint = 0;
                ctx->hex_वामः = 4;
                state = ngx_json_surrogate_hex;
                भङ्ग;
            }

            /* high surrogate followed by non-\u escape */
            प्रत्यागम NGX_DECLINED;

        पक्ष ngx_json_surrogate_hex:

            /* accumulate the four hex digits of the low \uXXXX escape */

            hd = ngx_json_hex_digit(*p);
            जइ (hd == NGX_ERROR) {
                प्रत्यागम NGX_DECLINED;
            }

            ctx->codepoint = (ctx->codepoint << 4) | (uint32_t) hd;

            जइ (--ctx->hex_वामः > 0) {
                भङ्ग;
            }

            जइ (ctx->codepoint >= 0xDC00 && ctx->codepoint <= 0xDFFF) {
                /* valid surrogate pair */
                state = ngx_json_string;
                भङ्ग;
            }

            /* high surrogate not followed by low surrogate */
            प्रत्यागम NGX_DECLINED;

        पक्ष ngx_json_सङ्ख्या_minus:
            जइ (*p >= '1' && *p <= '9') {
                state = ngx_json_सङ्ख्या_int;
                भङ्ग;
            }

            जइ (*p == '0') {
                state = ngx_json_सङ्ख्या_int_zero;
                भङ्ग;
            }

            प्रत्यागम NGX_DECLINED;

        पक्ष ngx_json_सङ्ख्या_int_zero:
            जइ (*p == '.') {
                state = ngx_json_सङ्ख्या_frac;
                भङ्ग;
            }

            जइ (*p == 'e' || *p == 'E') {
                /* No sense, but permitted by RFC */
                state = ngx_json_सङ्ख्या_exp;
                भङ्ग;
            }

            p--;

            rc = ngx_json_end_सङ्ख्या(ctx, start, p + 1, &state);
            जइ (rc != NGX_OK) {
                प्रत्यागम rc;
            }

            भङ्ग;

        पक्ष ngx_json_सङ्ख्या_int:
            जइ (*p >= '0' && *p <= '9') {
                भङ्ग;
            }

            जइ (*p == '.') {
                state = ngx_json_सङ्ख्या_frac;
                भङ्ग;
            }

            जइ (*p == 'e' || *p == 'E') {
                /* No sense, but permitted by RFC */
                state = ngx_json_सङ्ख्या_exp;
                भङ्ग;
            }

            p--;

            rc = ngx_json_end_सङ्ख्या(ctx, start, p + 1, &state);
            जइ (rc!= NGX_OK) {
                प्रत्यागम rc;
            }

            भङ्ग;

        पक्ष ngx_json_सङ्ख्या_frac:
            जइ (*p >= '0' && *p <= '9') {
                state = ngx_json_सङ्ख्या_frac_digit;
                भङ्ग;
            }

            प्रत्यागम NGX_DECLINED;

        पक्ष ngx_json_सङ्ख्या_frac_digit:
            जइ (*p >= '0' && *p <= '9') {
                भङ्ग;
            }

            जइ (*p == 'e' || *p == 'E') {
                state = ngx_json_सङ्ख्या_exp;
                भङ्ग;
            }

            p--;

            rc = ngx_json_end_सङ्ख्या(ctx, start, p + 1, &state);
            जइ (rc != NGX_OK) {
                प्रत्यागम rc;
            }

            भङ्ग;

        पक्ष ngx_json_सङ्ख्या_exp:
            जइ (*p == '-' || *p == '+') {
                state = ngx_json_सङ्ख्या_exp_plusminus;
                भङ्ग;
            }

            जइ (*p >= '0' && *p <= '9') {
                state = ngx_json_सङ्ख्या_exp_digit;
                भङ्ग;
            }

            प्रत्यागम NGX_DECLINED;

        पक्ष ngx_json_सङ्ख्या_exp_plusminus:
            जइ (*p >= '0' && *p <= '9') {
                state = ngx_json_सङ्ख्या_exp_digit;
                भङ्ग;
            }

            प्रत्यागम NGX_DECLINED;

        पक्ष ngx_json_सङ्ख्या_exp_digit:
            जइ (*p >= '0' && *p <= '9') {
                भङ्ग;
            }

            p--;

            rc = ngx_json_end_सङ्ख्या(ctx, start, p + 1, &state);
            जइ (rc != NGX_OK) {
                प्रत्यागम rc;
            }

            भङ्ग;

        पक्ष ngx_json_true_t:
            जइ (*p == 'r') {
                state = ngx_json_true_tr;
                भङ्ग;
            }

            प्रत्यागम NGX_DECLINED;

        पक्ष ngx_json_true_tr:
            जइ (*p == 'u') {
                state = ngx_json_true_tru;
                भङ्ग;
            }

            प्रत्यागम NGX_DECLINED;

        पक्ष ngx_json_true_tru:
            जइ (*p == 'e') {
                rc = ngx_json_emit(ctx, NGX_JSON_मानम्_BOOL, start,
                                  p + 1 - start);
                जइ (rc != NGX_OK) {
                    प्रत्यागम rc;
                }

                state = ngx_json_after_मानम्(ctx);
                भङ्ग;
            }

            प्रत्यागम NGX_DECLINED;

        पक्ष ngx_json_false_f:
            जइ (*p == 'a') {
                state = ngx_json_false_fa;
                भङ्ग;
            }

            प्रत्यागम NGX_DECLINED;

        पक्ष ngx_json_false_fa:
            जइ (*p == 'l') {
                state = ngx_json_false_fal;
                भङ्ग;
            }

            प्रत्यागम NGX_DECLINED;

        पक्ष ngx_json_false_fal:
            जइ (*p == 's') {
                state = ngx_json_false_fals;
                भङ्ग;
            }

            प्रत्यागम NGX_DECLINED;

        पक्ष ngx_json_false_fals:
            जइ (*p == 'e') {
                rc = ngx_json_emit(ctx, NGX_JSON_मानम्_BOOL, start,
                                  p + 1 - start);
                जइ (rc != NGX_OK) {
                    प्रत्यागम rc;
                }

                state = ngx_json_after_मानम्(ctx);
                भङ्ग;
            }

            प्रत्यागम NGX_DECLINED;

        पक्ष ngx_json_null_n:
            जइ (*p == 'u') {
                state = ngx_json_null_nu;
                भङ्ग;
            }

            प्रत्यागम NGX_DECLINED;

        पक्ष ngx_json_null_nu:
            जइ (*p == 'l') {
                state = ngx_json_null_nul;
                भङ्ग;
            }

            प्रत्यागम NGX_DECLINED;

        पक्ष ngx_json_null_nul:
            जइ (*p == 'l') {
                rc = ngx_json_emit(ctx, NGX_JSON_मानम्_NULL, start,
                                  p + 1 - start);
                जइ (rc != NGX_OK) {
                    प्रत्यागम rc;
                }

                state = ngx_json_after_मानम्(ctx);
                भङ्ग;
            }

            प्रत्यागम NGX_DECLINED;

        पक्ष ngx_json_done:
            जइ (ngx_json_ws(*p)) {
                भङ्ग;
            }

            प्रत्यागम NGX_DECLINED;
        }

        skip_depth = ctx->skip_until_depth;
    }

    /* a bare top-level number is not self-delimited; emit it at end of input */

    जइ (ctx->depth == 0
        && (state == ngx_json_सङ्ख्या_int_zero
            || state == ngx_json_सङ्ख्या_int
            || state == ngx_json_सङ्ख्या_frac_digit
            || state == ngx_json_सङ्ख्या_exp_digit))
    {
        rc = ngx_json_emit(ctx, NGX_JSON_मानम्_सङ्ख्या, start, last - start);
        जइ (rc != NGX_OK) {
            प्रत्यागम rc;
        }

        state = ngx_json_done;
    }

    ctx->state = state;

    जइ (state == ngx_json_done) {
        प्रत्यागम NGX_OK;
    }

    प्रत्यागम NGX_DECLINED;
}


ngx_int_t
ngx_json_parse(ngx_pool_t *pool, ngx_str_t *json, ngx_json_handler_pt handler,
    शून्य *data)
{
    ngx_json_ctx_t  jctx;

    ngx_json_ctx_init(&jctx, pool);

    jctx.data = data;
    jctx.handler = handler;

    प्रत्यागम ngx_json_parse_ctx(&jctx, json->data, json->len);
}


स्थिर ngx_inline ngx_int_t
ngx_json_end_सङ्ख्या(ngx_json_ctx_t *ctx, u_char *start, u_char *end,
    ngx_json_state_e *state)
{
    ngx_int_t  rc;

    rc = ngx_json_emit(ctx, NGX_JSON_मानम्_सङ्ख्या, start, end - start);
    जइ (rc != NGX_OK) {
        प्रत्यागम rc;
    }

    *state = ngx_json_after_मानम्(ctx);

    प्रत्यागम NGX_OK;
}


स्थिर ngx_inline ngx_int_t
ngx_json_emit(ngx_json_ctx_t *ctx, ngx_json_event_e event, u_char *data,
    size_t len)
{
    ngx_int_t  rc;
    ngx_str_t  token;

    जइ (ngx_json_skipping(ctx->skip_until_depth)) {
        प्रत्यागम NGX_OK;
    }

    जइ (ctx->handler == रिक्त) {
        प्रत्यागम NGX_OK;
    }

    token.data = data;
    token.len = len;
    rc = ctx->handler(ctx, event, &token);

    जइ (rc == NGX_JSON_SKIP) {

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

        विकल्प (event) {

        पक्ष NGX_JSON_कुञ्चिका:
        पक्ष NGX_JSON_कुञ्चिका_ESCAPED:
        पक्ष NGX_JSON_OBJECT_OPEN:
        पक्ष NGX_JSON_ARRAY_OPEN:
            ctx->skip_until_depth = ctx->depth;
            भङ्ग;

        पक्ष NGX_JSON_मानम्_STRING:
        पक्ष NGX_JSON_मानम्_STRING_ESCAPED:
        पक्ष NGX_JSON_मानम्_सङ्ख्या:
        पक्ष NGX_JSON_मानम्_BOOL:
        पक्ष NGX_JSON_मानम्_NULL:
            जइ (ctx->depth > 0
                && ctx->राशिः[ctx->depth - 1] == ngx_json_ctx_array
            ) {
                ctx->skip_until_depth = ctx->depth - 1;
            }
            भङ्ग;

        स्वभाव:
            भङ्ग;
        }

        प्रत्यागम NGX_OK;
    }

    प्रत्यागम rc;
}


स्थिर ngx_inline ngx_json_state_e
ngx_json_after_मानम्(ngx_json_ctx_t *ctx)
{
    जइ (ctx->depth == 0) {
        प्रत्यागम ngx_json_done;
    }

    जइ (ctx->राशिः[ctx->depth - 1] == ngx_json_ctx_object) {
        प्रत्यागम ngx_json_object_मानम्_separator;
    }

    प्रत्यागम ngx_json_array_मानम्_separator;
}


स्थिर ngx_inline ngx_int_t
ngx_json_close(ngx_json_ctx_t *ctx, ngx_json_event_e event,
    ngx_json_state_e *state, u_char *p)
{
    ngx_int_t  rc;

    ctx->depth--;

    rc = ngx_json_emit(ctx, event, p, 1);
    जइ (rc != NGX_OK) {
        प्रत्यागम rc;
    }

    *state = ngx_json_after_मानम्(ctx);

    प्रत्यागम NGX_OK;
}


स्थिर ngx_int_t
ngx_json_प्रक्षिप_state(ngx_json_ctx_t *ctx, ngx_json_container_e container)
{
    ngx_json_container_e  *राशिः;

    जइ (ctx->depth >= ctx->max_depth) {
        प्रत्यागम NGX_DECLINED;
    }

    जइ (ctx->राशिः == रिक्त) {
        राशिः = ngx_palloc(ctx->pool,
                           ctx->max_depth * परिमाण(ngx_json_container_e));
        जइ (राशिः == रिक्त) {
            प्रत्यागम NGX_ERROR;
        }

        ctx->राशिः = राशिः;
    }

    ctx->राशिः[ctx->depth] = container;
    ctx->depth++;

    प्रत्यागम NGX_OK;
}
