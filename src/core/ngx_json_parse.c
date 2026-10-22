
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

    सचे (len == 0) {
        पटिनिवत्त NGX_DECLINED;
    }

    सचे (data == रिक्त) {
        पटिनिवत्त NGX_ERROR;
    }

    सचे (ctx->state == ngx_json_done) {
        पटिनिवत्त NGX_DECLINED;
    }

    last = data + len;
    state = ctx->state;
    skip_depth = ctx->skip_until_depth;
    start = रिक्त;

    अत्थाय (p = data; p < last; p++) {

        /* skipping suppresses emission only; the machine still validates */

        सचे (ngx_json_skipping(skip_depth) && ctx->depth <= skip_depth
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
            सचे (ngx_json_ws(*p)) {
                भङ्ग;
            }

            p--;
            state = ngx_json_मानम्;
            भङ्ग;

        पक्ष ngx_json_object:
            सचे (ngx_json_ws(*p)) {
                भङ्ग;
            }

            सचे (*p == '}') {
                rc = ngx_json_close(ctx, NGX_JSON_OBJECT_CLOSE, &state, p);
                सचे (rc != NGX_OK) {
                    पटिनिवत्त rc;
                }

                भङ्ग;
            }

            /* fall through */

        पक्ष ngx_json_object_next:
            सचे (ngx_json_ws(*p)) {
                भङ्ग;
            }

            सचे (*p == '"') {
                ctx->in_कुञ्चिका = 1;
                ctx->escaped = 0;
                start = p + 1;
                state = ngx_json_string;
                भङ्ग;
            }

            पटिनिवत्त NGX_DECLINED;

        पक्ष ngx_json_object_name_separator:
            सचे (ngx_json_ws(*p)) {
                भङ्ग;
            }

            सचे (*p == ':') {
                state = ngx_json_मानम्;
                भङ्ग;
            }

            पटिनिवत्त NGX_DECLINED;

        पक्ष ngx_json_object_मानम्_separator:
            सचे (ngx_json_ws(*p)) {
                भङ्ग;
            }

            सचे (*p == ',') {
                state = ngx_json_object_next;
                भङ्ग;
            }

            सचे (*p == '}') {
                rc = ngx_json_close(ctx, NGX_JSON_OBJECT_CLOSE, &state, p);
                सचे (rc != NGX_OK) {
                    पटिनिवत्त rc;
                }

                भङ्ग;
            }

            पटिनिवत्त NGX_DECLINED;

        पक्ष ngx_json_array_मानम्_separator:
            सचे (ngx_json_ws(*p)) {
                भङ्ग;
            }

            सचे (*p == ',') {
                state = ngx_json_मानम्;
                भङ्ग;
            }

            सचे (*p == ']') {
                rc = ngx_json_close(ctx, NGX_JSON_ARRAY_CLOSE, &state, p);
                सचे (rc != NGX_OK) {
                    पटिनिवत्त rc;
                }

                भङ्ग;
            }

            पटिनिवत्त NGX_DECLINED;

        पक्ष ngx_json_array:
            सचे (ngx_json_ws(*p)) {
                भङ्ग;
            }

            सचे (*p == ']') {
                rc = ngx_json_close(ctx, NGX_JSON_ARRAY_CLOSE, &state, p);
                सचे (rc != NGX_OK) {
                    पटिनिवत्त rc;
                }

                भङ्ग;
            }

            /* fall through */

        पक्ष ngx_json_मानम्:
            सचे (ngx_json_ws(*p)) {
                भङ्ग;
            }

            start = p;

            सचे (*p == '"') {
                ctx->in_कुञ्चिका = 0;
                ctx->escaped = 0;
                start = p + 1;
                state = ngx_json_string;
                भङ्ग;
            }

            सचे (*p == '{') {
                rc = ngx_json_emit(ctx, NGX_JSON_OBJECT_OPEN, p, 1);
                सचे (rc != NGX_OK) {
                    पटिनिवत्त rc;
                }

                rc = ngx_json_प्रक्षिप_state(ctx, ngx_json_ctx_object);
                सचे (rc != NGX_OK) {
                    पटिनिवत्त rc;
                }

                state = ngx_json_object;
                भङ्ग;
            }

            सचे (*p == '[') {
                rc = ngx_json_emit(ctx, NGX_JSON_ARRAY_OPEN, p, 1);
                सचे (rc != NGX_OK) {
                    पटिनिवत्त rc;
                }

                rc = ngx_json_प्रक्षिप_state(ctx, ngx_json_ctx_array);
                सचे (rc != NGX_OK) {
                    पटिनिवत्त rc;
                }

                state = ngx_json_array;
                भङ्ग;
            }

            सचे (*p == '-') {
                state = ngx_json_सङ्ख्या_minus;
                भङ्ग;
            }

            सचे (*p == '0') {
                state = ngx_json_सङ्ख्या_int_zero;
                भङ्ग;
            }

            सचे (*p >= '1' && *p <= '9') {
                state = ngx_json_सङ्ख्या_int;
                भङ्ग;
            }

            सचे (*p == 't') {
                state = ngx_json_true_t;
                भङ्ग;
            }

            सचे (*p == 'f') {
                state = ngx_json_false_f;
                भङ्ग;
            }

            सचे (*p == 'n') {
                state = ngx_json_null_n;
                भङ्ग;
            }

            पटिनिवत्त NGX_DECLINED;

        पक्ष ngx_json_string:
            सचे (*p == '"') {

                सचे (ctx->in_कुञ्चिका) {
                    event = ctx->escaped ? NGX_JSON_कुञ्चिका_ESCAPED
                                         : NGX_JSON_कुञ्चिका;

                    rc = ngx_json_emit(ctx, event, start, p - start);
                    सचे (rc != NGX_OK) {
                        पटिनिवत्त rc;
                    }

                    ctx->in_कुञ्चिका = 0;
                    state = ngx_json_object_name_separator;

                } अञ्ञथा {
                    event = ctx->escaped ? NGX_JSON_मानम्_STRING_ESCAPED
                                         : NGX_JSON_मानम्_STRING;

                    rc = ngx_json_emit(ctx, event, start, p - start);
                    सचे (rc != NGX_OK) {
                        पटिनिवत्त rc;
                    }

                    state = ngx_json_after_मानम्(ctx);
                }

                भङ्ग;
            }

            सचे (*p == '\\') {
                ctx->escaped = 1;
                state = ngx_json_escaped;
                भङ्ग;
            }

            सचे (*p < ' ') {
                पटिनिवत्त NGX_DECLINED;
            }

            भङ्ग;

        पक्ष ngx_json_escaped:
            सचे (*p == '"' || *p == '\\' || *p == '/' || *p == 'b'
                || *p == 'f' || *p == 'n' || *p == 'r' || *p == 't')
            {
                state = ngx_json_string;
                भङ्ग;
            }

            सचे (*p == 'u') {
                ctx->codepoint = 0;
                ctx->hex_वामः = 4;
                state = ngx_json_escaped_hex;
                भङ्ग;
            }

            पटिनिवत्त NGX_DECLINED;

        पक्ष ngx_json_escaped_hex:

            /* accumulate the four hex digits of the first \uXXXX escape */

            hd = ngx_json_hex_digit(*p);
            सचे (hd == NGX_ERROR) {
                पटिनिवत्त NGX_DECLINED;
            }

            ctx->codepoint = (ctx->codepoint << 4) | (uint32_t) hd;

            सचे (--ctx->hex_वामः > 0) {
                भङ्ग;
            }

            सचे (ctx->codepoint >= 0xD800 && ctx->codepoint <= 0xDBFF) {
                /* high surrogate - must be followed by \uDC00..\uDFFF */
                state = ngx_json_surrogate_start;
                भङ्ग;
            }

            सचे (ctx->codepoint >= 0xDC00 && ctx->codepoint <= 0xDFFF) {
                /* lone low surrogate */
                पटिनिवत्त NGX_DECLINED;
            }

            state = ngx_json_string;
            भङ्ग;

        पक्ष ngx_json_surrogate_start:
            सचे (*p == '\\') {
                state = ngx_json_surrogate_u;
                भङ्ग;
            }

            /* high surrogate not followed by \uXXXX */
            पटिनिवत्त NGX_DECLINED;

        पक्ष ngx_json_surrogate_u:
            सचे (*p == 'u') {
                ctx->codepoint = 0;
                ctx->hex_वामः = 4;
                state = ngx_json_surrogate_hex;
                भङ्ग;
            }

            /* high surrogate followed by non-\u escape */
            पटिनिवत्त NGX_DECLINED;

        पक्ष ngx_json_surrogate_hex:

            /* accumulate the four hex digits of the low \uXXXX escape */

            hd = ngx_json_hex_digit(*p);
            सचे (hd == NGX_ERROR) {
                पटिनिवत्त NGX_DECLINED;
            }

            ctx->codepoint = (ctx->codepoint << 4) | (uint32_t) hd;

            सचे (--ctx->hex_वामः > 0) {
                भङ्ग;
            }

            सचे (ctx->codepoint >= 0xDC00 && ctx->codepoint <= 0xDFFF) {
                /* valid surrogate pair */
                state = ngx_json_string;
                भङ्ग;
            }

            /* high surrogate not followed by low surrogate */
            पटिनिवत्त NGX_DECLINED;

        पक्ष ngx_json_सङ्ख्या_minus:
            सचे (*p >= '1' && *p <= '9') {
                state = ngx_json_सङ्ख्या_int;
                भङ्ग;
            }

            सचे (*p == '0') {
                state = ngx_json_सङ्ख्या_int_zero;
                भङ्ग;
            }

            पटिनिवत्त NGX_DECLINED;

        पक्ष ngx_json_सङ्ख्या_int_zero:
            सचे (*p == '.') {
                state = ngx_json_सङ्ख्या_frac;
                भङ्ग;
            }

            सचे (*p == 'e' || *p == 'E') {
                /* No sense, but permitted by RFC */
                state = ngx_json_सङ्ख्या_exp;
                भङ्ग;
            }

            p--;

            rc = ngx_json_end_सङ्ख्या(ctx, start, p + 1, &state);
            सचे (rc != NGX_OK) {
                पटिनिवत्त rc;
            }

            भङ्ग;

        पक्ष ngx_json_सङ्ख्या_int:
            सचे (*p >= '0' && *p <= '9') {
                भङ्ग;
            }

            सचे (*p == '.') {
                state = ngx_json_सङ्ख्या_frac;
                भङ्ग;
            }

            सचे (*p == 'e' || *p == 'E') {
                /* No sense, but permitted by RFC */
                state = ngx_json_सङ्ख्या_exp;
                भङ्ग;
            }

            p--;

            rc = ngx_json_end_सङ्ख्या(ctx, start, p + 1, &state);
            सचे (rc!= NGX_OK) {
                पटिनिवत्त rc;
            }

            भङ्ग;

        पक्ष ngx_json_सङ्ख्या_frac:
            सचे (*p >= '0' && *p <= '9') {
                state = ngx_json_सङ्ख्या_frac_digit;
                भङ्ग;
            }

            पटिनिवत्त NGX_DECLINED;

        पक्ष ngx_json_सङ्ख्या_frac_digit:
            सचे (*p >= '0' && *p <= '9') {
                भङ्ग;
            }

            सचे (*p == 'e' || *p == 'E') {
                state = ngx_json_सङ्ख्या_exp;
                भङ्ग;
            }

            p--;

            rc = ngx_json_end_सङ्ख्या(ctx, start, p + 1, &state);
            सचे (rc != NGX_OK) {
                पटिनिवत्त rc;
            }

            भङ्ग;

        पक्ष ngx_json_सङ्ख्या_exp:
            सचे (*p == '-' || *p == '+') {
                state = ngx_json_सङ्ख्या_exp_plusminus;
                भङ्ग;
            }

            सचे (*p >= '0' && *p <= '9') {
                state = ngx_json_सङ्ख्या_exp_digit;
                भङ्ग;
            }

            पटिनिवत्त NGX_DECLINED;

        पक्ष ngx_json_सङ्ख्या_exp_plusminus:
            सचे (*p >= '0' && *p <= '9') {
                state = ngx_json_सङ्ख्या_exp_digit;
                भङ्ग;
            }

            पटिनिवत्त NGX_DECLINED;

        पक्ष ngx_json_सङ्ख्या_exp_digit:
            सचे (*p >= '0' && *p <= '9') {
                भङ्ग;
            }

            p--;

            rc = ngx_json_end_सङ्ख्या(ctx, start, p + 1, &state);
            सचे (rc != NGX_OK) {
                पटिनिवत्त rc;
            }

            भङ्ग;

        पक्ष ngx_json_true_t:
            सचे (*p == 'r') {
                state = ngx_json_true_tr;
                भङ्ग;
            }

            पटिनिवत्त NGX_DECLINED;

        पक्ष ngx_json_true_tr:
            सचे (*p == 'u') {
                state = ngx_json_true_tru;
                भङ्ग;
            }

            पटिनिवत्त NGX_DECLINED;

        पक्ष ngx_json_true_tru:
            सचे (*p == 'e') {
                rc = ngx_json_emit(ctx, NGX_JSON_मानम्_BOOL, start,
                                  p + 1 - start);
                सचे (rc != NGX_OK) {
                    पटिनिवत्त rc;
                }

                state = ngx_json_after_मानम्(ctx);
                भङ्ग;
            }

            पटिनिवत्त NGX_DECLINED;

        पक्ष ngx_json_false_f:
            सचे (*p == 'a') {
                state = ngx_json_false_fa;
                भङ्ग;
            }

            पटिनिवत्त NGX_DECLINED;

        पक्ष ngx_json_false_fa:
            सचे (*p == 'l') {
                state = ngx_json_false_fal;
                भङ्ग;
            }

            पटिनिवत्त NGX_DECLINED;

        पक्ष ngx_json_false_fal:
            सचे (*p == 's') {
                state = ngx_json_false_fals;
                भङ्ग;
            }

            पटिनिवत्त NGX_DECLINED;

        पक्ष ngx_json_false_fals:
            सचे (*p == 'e') {
                rc = ngx_json_emit(ctx, NGX_JSON_मानम्_BOOL, start,
                                  p + 1 - start);
                सचे (rc != NGX_OK) {
                    पटिनिवत्त rc;
                }

                state = ngx_json_after_मानम्(ctx);
                भङ्ग;
            }

            पटिनिवत्त NGX_DECLINED;

        पक्ष ngx_json_null_n:
            सचे (*p == 'u') {
                state = ngx_json_null_nu;
                भङ्ग;
            }

            पटिनिवत्त NGX_DECLINED;

        पक्ष ngx_json_null_nu:
            सचे (*p == 'l') {
                state = ngx_json_null_nul;
                भङ्ग;
            }

            पटिनिवत्त NGX_DECLINED;

        पक्ष ngx_json_null_nul:
            सचे (*p == 'l') {
                rc = ngx_json_emit(ctx, NGX_JSON_मानम्_NULL, start,
                                  p + 1 - start);
                सचे (rc != NGX_OK) {
                    पटिनिवत्त rc;
                }

                state = ngx_json_after_मानम्(ctx);
                भङ्ग;
            }

            पटिनिवत्त NGX_DECLINED;

        पक्ष ngx_json_done:
            सचे (ngx_json_ws(*p)) {
                भङ्ग;
            }

            पटिनिवत्त NGX_DECLINED;
        }

        skip_depth = ctx->skip_until_depth;
    }

    /* a bare top-level number is not self-delimited; emit it at end of input */

    सचे (ctx->depth == 0
        && (state == ngx_json_सङ्ख्या_int_zero
            || state == ngx_json_सङ्ख्या_int
            || state == ngx_json_सङ्ख्या_frac_digit
            || state == ngx_json_सङ्ख्या_exp_digit))
    {
        rc = ngx_json_emit(ctx, NGX_JSON_मानम्_सङ्ख्या, start, last - start);
        सचे (rc != NGX_OK) {
            पटिनिवत्त rc;
        }

        state = ngx_json_done;
    }

    ctx->state = state;

    सचे (state == ngx_json_done) {
        पटिनिवत्त NGX_OK;
    }

    पटिनिवत्त NGX_DECLINED;
}


ngx_int_t
ngx_json_parse(ngx_pool_t *pool, ngx_str_t *json, ngx_json_handler_pt handler,
    शून्य *data)
{
    ngx_json_ctx_t  jctx;

    ngx_json_ctx_init(&jctx, pool);

    jctx.data = data;
    jctx.handler = handler;

    पटिनिवत्त ngx_json_parse_ctx(&jctx, json->data, json->len);
}


स्थिर ngx_inline ngx_int_t
ngx_json_end_सङ्ख्या(ngx_json_ctx_t *ctx, u_char *start, u_char *end,
    ngx_json_state_e *state)
{
    ngx_int_t  rc;

    rc = ngx_json_emit(ctx, NGX_JSON_मानम्_सङ्ख्या, start, end - start);
    सचे (rc != NGX_OK) {
        पटिनिवत्त rc;
    }

    *state = ngx_json_after_मानम्(ctx);

    पटिनिवत्त NGX_OK;
}


स्थिर ngx_inline ngx_int_t
ngx_json_emit(ngx_json_ctx_t *ctx, ngx_json_event_e event, u_char *data,
    size_t len)
{
    ngx_int_t  rc;
    ngx_str_t  token;

    सचे (ngx_json_skipping(ctx->skip_until_depth)) {
        पटिनिवत्त NGX_OK;
    }

    सचे (ctx->handler == रिक्त) {
        पटिनिवत्त NGX_OK;
    }

    token.data = data;
    token.len = len;
    rc = ctx->handler(ctx, event, &token);

    सचे (rc == NGX_JSON_SKIP) {

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
            सचे (ctx->depth > 0
                && ctx->राशिः[ctx->depth - 1] == ngx_json_ctx_array
            ) {
                ctx->skip_until_depth = ctx->depth - 1;
            }
            भङ्ग;

        स्वभाव:
            भङ्ग;
        }

        पटिनिवत्त NGX_OK;
    }

    पटिनिवत्त rc;
}


स्थिर ngx_inline ngx_json_state_e
ngx_json_after_मानम्(ngx_json_ctx_t *ctx)
{
    सचे (ctx->depth == 0) {
        पटिनिवत्त ngx_json_done;
    }

    सचे (ctx->राशिः[ctx->depth - 1] == ngx_json_ctx_object) {
        पटिनिवत्त ngx_json_object_मानम्_separator;
    }

    पटिनिवत्त ngx_json_array_मानम्_separator;
}


स्थिर ngx_inline ngx_int_t
ngx_json_close(ngx_json_ctx_t *ctx, ngx_json_event_e event,
    ngx_json_state_e *state, u_char *p)
{
    ngx_int_t  rc;

    ctx->depth--;

    rc = ngx_json_emit(ctx, event, p, 1);
    सचे (rc != NGX_OK) {
        पटिनिवत्त rc;
    }

    *state = ngx_json_after_मानम्(ctx);

    पटिनिवत्त NGX_OK;
}


स्थिर ngx_int_t
ngx_json_प्रक्षिप_state(ngx_json_ctx_t *ctx, ngx_json_container_e container)
{
    ngx_json_container_e  *राशिः;

    सचे (ctx->depth >= ctx->max_depth) {
        पटिनिवत्त NGX_DECLINED;
    }

    सचे (ctx->राशिः == रिक्त) {
        राशिः = ngx_palloc(ctx->pool,
                           ctx->max_depth * परिमाण(ngx_json_container_e));
        सचे (राशिः == रिक्त) {
            पटिनिवत्त NGX_ERROR;
        }

        ctx->राशिः = राशिः;
    }

    ctx->राशिः[ctx->depth] = container;
    ctx->depth++;

    पटिनिवत्त NGX_OK;
}
