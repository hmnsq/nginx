
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
    ngx_json_event_e event, u_char *data, माप_प्रकार len);
स्थिर ngx_inline ngx_json_state_e ngx_json_after_मान(ngx_json_ctx_t *ctx);
स्थिर ngx_inline ngx_int_t ngx_json_close(ngx_json_ctx_t *ctx,
    ngx_json_event_e event, ngx_json_state_e *state, u_char *p);
स्थिर ngx_int_t ngx_json_थप_state(ngx_json_ctx_t *ctx,
    ngx_json_container_e container);


शून्य
ngx_json_ctx_init(ngx_json_ctx_t *ctx, ngx_pool_t *pool)
{
    ctx->state = ngx_json_start;
    ctx->pool = pool;
    ctx->handler = रिक्त;
    ctx->data = रिक्त;
    ctx->codepoint = 0;
    ctx->hex_बायाँ = 0;

    ctx->depth = 0;
    ctx->skip_until_depth = NGX_JSON_NOT_SKIPPING;
    ctx->max_depth = NGX_JSON_DEFAULT_MAX_DEPTH;

    /* stack is allocated lazily on first push, sized to max_depth */

    ctx->थाक = रिक्त;

    ctx->in_साँचो = 0;
    ctx->escaped = 0;
}


ngx_int_t
ngx_json_parse_ctx(ngx_json_ctx_t *ctx, u_char *data, माप_प्रकार len)
{
    u_char            *p, *last, *start;
    ngx_int_t          rc, hd;
    ngx_uint_t         skip_depth;
    ngx_json_event_e   event;
    ngx_json_state_e   state;

    यदि (len == 0) {
        फर्काऊ NGX_DECLINED;
    }

    यदि (data == रिक्त) {
        फर्काऊ NGX_ERROR;
    }

    यदि (ctx->state == ngx_json_done) {
        फर्काऊ NGX_DECLINED;
    }

    last = data + len;
    state = ctx->state;
    skip_depth = ctx->skip_until_depth;
    start = रिक्त;

    लागि (p = data; p < last; p++) {

        /* skipping suppresses emission only; the machine still validates */

        यदि (ngx_json_skipping(skip_depth) && ctx->depth <= skip_depth
            && (state == ngx_json_object_मान_separator
                || state == ngx_json_array_मान_separator
                || state == ngx_json_done))
        {
            skip_depth = NGX_JSON_NOT_SKIPPING;
            ctx->skip_until_depth = NGX_JSON_NOT_SKIPPING;
            p--;

            जारी;
        }

        विकल्प (state) {

        पक्ष ngx_json_start:
            यदि (ngx_json_ws(*p)) {
                रोक;
            }

            p--;
            state = ngx_json_मान;
            रोक;

        पक्ष ngx_json_object:
            यदि (ngx_json_ws(*p)) {
                रोक;
            }

            यदि (*p == '}') {
                rc = ngx_json_close(ctx, NGX_JSON_OBJECT_CLOSE, &state, p);
                यदि (rc != NGX_OK) {
                    फर्काऊ rc;
                }

                रोक;
            }

            /* fall through */

        पक्ष ngx_json_object_next:
            यदि (ngx_json_ws(*p)) {
                रोक;
            }

            यदि (*p == '"') {
                ctx->in_साँचो = 1;
                ctx->escaped = 0;
                start = p + 1;
                state = ngx_json_string;
                रोक;
            }

            फर्काऊ NGX_DECLINED;

        पक्ष ngx_json_object_name_separator:
            यदि (ngx_json_ws(*p)) {
                रोक;
            }

            यदि (*p == ':') {
                state = ngx_json_मान;
                रोक;
            }

            फर्काऊ NGX_DECLINED;

        पक्ष ngx_json_object_मान_separator:
            यदि (ngx_json_ws(*p)) {
                रोक;
            }

            यदि (*p == ',') {
                state = ngx_json_object_next;
                रोक;
            }

            यदि (*p == '}') {
                rc = ngx_json_close(ctx, NGX_JSON_OBJECT_CLOSE, &state, p);
                यदि (rc != NGX_OK) {
                    फर्काऊ rc;
                }

                रोक;
            }

            फर्काऊ NGX_DECLINED;

        पक्ष ngx_json_array_मान_separator:
            यदि (ngx_json_ws(*p)) {
                रोक;
            }

            यदि (*p == ',') {
                state = ngx_json_मान;
                रोक;
            }

            यदि (*p == ']') {
                rc = ngx_json_close(ctx, NGX_JSON_ARRAY_CLOSE, &state, p);
                यदि (rc != NGX_OK) {
                    फर्काऊ rc;
                }

                रोक;
            }

            फर्काऊ NGX_DECLINED;

        पक्ष ngx_json_array:
            यदि (ngx_json_ws(*p)) {
                रोक;
            }

            यदि (*p == ']') {
                rc = ngx_json_close(ctx, NGX_JSON_ARRAY_CLOSE, &state, p);
                यदि (rc != NGX_OK) {
                    फर्काऊ rc;
                }

                रोक;
            }

            /* fall through */

        पक्ष ngx_json_मान:
            यदि (ngx_json_ws(*p)) {
                रोक;
            }

            start = p;

            यदि (*p == '"') {
                ctx->in_साँचो = 0;
                ctx->escaped = 0;
                start = p + 1;
                state = ngx_json_string;
                रोक;
            }

            यदि (*p == '{') {
                rc = ngx_json_emit(ctx, NGX_JSON_OBJECT_OPEN, p, 1);
                यदि (rc != NGX_OK) {
                    फर्काऊ rc;
                }

                rc = ngx_json_थप_state(ctx, ngx_json_ctx_object);
                यदि (rc != NGX_OK) {
                    फर्काऊ rc;
                }

                state = ngx_json_object;
                रोक;
            }

            यदि (*p == '[') {
                rc = ngx_json_emit(ctx, NGX_JSON_ARRAY_OPEN, p, 1);
                यदि (rc != NGX_OK) {
                    फर्काऊ rc;
                }

                rc = ngx_json_थप_state(ctx, ngx_json_ctx_array);
                यदि (rc != NGX_OK) {
                    फर्काऊ rc;
                }

                state = ngx_json_array;
                रोक;
            }

            यदि (*p == '-') {
                state = ngx_json_सङ्ख्या_minus;
                रोक;
            }

            यदि (*p == '0') {
                state = ngx_json_सङ्ख्या_int_zero;
                रोक;
            }

            यदि (*p >= '1' && *p <= '9') {
                state = ngx_json_सङ्ख्या_int;
                रोक;
            }

            यदि (*p == 't') {
                state = ngx_json_true_t;
                रोक;
            }

            यदि (*p == 'f') {
                state = ngx_json_false_f;
                रोक;
            }

            यदि (*p == 'n') {
                state = ngx_json_null_n;
                रोक;
            }

            फर्काऊ NGX_DECLINED;

        पक्ष ngx_json_string:
            यदि (*p == '"') {

                यदि (ctx->in_साँचो) {
                    event = ctx->escaped ? NGX_JSON_साँचो_ESCAPED
                                         : NGX_JSON_साँचो;

                    rc = ngx_json_emit(ctx, event, start, p - start);
                    यदि (rc != NGX_OK) {
                        फर्काऊ rc;
                    }

                    ctx->in_साँचो = 0;
                    state = ngx_json_object_name_separator;

                } अन्यथा {
                    event = ctx->escaped ? NGX_JSON_मान_STRING_ESCAPED
                                         : NGX_JSON_मान_STRING;

                    rc = ngx_json_emit(ctx, event, start, p - start);
                    यदि (rc != NGX_OK) {
                        फर्काऊ rc;
                    }

                    state = ngx_json_after_मान(ctx);
                }

                रोक;
            }

            यदि (*p == '\\') {
                ctx->escaped = 1;
                state = ngx_json_escaped;
                रोक;
            }

            यदि (*p < ' ') {
                फर्काऊ NGX_DECLINED;
            }

            रोक;

        पक्ष ngx_json_escaped:
            यदि (*p == '"' || *p == '\\' || *p == '/' || *p == 'b'
                || *p == 'f' || *p == 'n' || *p == 'r' || *p == 't')
            {
                state = ngx_json_string;
                रोक;
            }

            यदि (*p == 'u') {
                ctx->codepoint = 0;
                ctx->hex_बायाँ = 4;
                state = ngx_json_escaped_hex;
                रोक;
            }

            फर्काऊ NGX_DECLINED;

        पक्ष ngx_json_escaped_hex:

            /* accumulate the four hex digits of the first \uXXXX escape */

            hd = ngx_json_hex_digit(*p);
            यदि (hd == NGX_ERROR) {
                फर्काऊ NGX_DECLINED;
            }

            ctx->codepoint = (ctx->codepoint << 4) | (uint32_t) hd;

            यदि (--ctx->hex_बायाँ > 0) {
                रोक;
            }

            यदि (ctx->codepoint >= 0xD800 && ctx->codepoint <= 0xDBFF) {
                /* high surrogate - must be followed by \uDC00..\uDFFF */
                state = ngx_json_surrogate_start;
                रोक;
            }

            यदि (ctx->codepoint >= 0xDC00 && ctx->codepoint <= 0xDFFF) {
                /* lone low surrogate */
                फर्काऊ NGX_DECLINED;
            }

            state = ngx_json_string;
            रोक;

        पक्ष ngx_json_surrogate_start:
            यदि (*p == '\\') {
                state = ngx_json_surrogate_u;
                रोक;
            }

            /* high surrogate not followed by \uXXXX */
            फर्काऊ NGX_DECLINED;

        पक्ष ngx_json_surrogate_u:
            यदि (*p == 'u') {
                ctx->codepoint = 0;
                ctx->hex_बायाँ = 4;
                state = ngx_json_surrogate_hex;
                रोक;
            }

            /* high surrogate followed by non-\u escape */
            फर्काऊ NGX_DECLINED;

        पक्ष ngx_json_surrogate_hex:

            /* accumulate the four hex digits of the low \uXXXX escape */

            hd = ngx_json_hex_digit(*p);
            यदि (hd == NGX_ERROR) {
                फर्काऊ NGX_DECLINED;
            }

            ctx->codepoint = (ctx->codepoint << 4) | (uint32_t) hd;

            यदि (--ctx->hex_बायाँ > 0) {
                रोक;
            }

            यदि (ctx->codepoint >= 0xDC00 && ctx->codepoint <= 0xDFFF) {
                /* valid surrogate pair */
                state = ngx_json_string;
                रोक;
            }

            /* high surrogate not followed by low surrogate */
            फर्काऊ NGX_DECLINED;

        पक्ष ngx_json_सङ्ख्या_minus:
            यदि (*p >= '1' && *p <= '9') {
                state = ngx_json_सङ्ख्या_int;
                रोक;
            }

            यदि (*p == '0') {
                state = ngx_json_सङ्ख्या_int_zero;
                रोक;
            }

            फर्काऊ NGX_DECLINED;

        पक्ष ngx_json_सङ्ख्या_int_zero:
            यदि (*p == '.') {
                state = ngx_json_सङ्ख्या_frac;
                रोक;
            }

            यदि (*p == 'e' || *p == 'E') {
                /* No sense, but permitted by RFC */
                state = ngx_json_सङ्ख्या_exp;
                रोक;
            }

            p--;

            rc = ngx_json_end_सङ्ख्या(ctx, start, p + 1, &state);
            यदि (rc != NGX_OK) {
                फर्काऊ rc;
            }

            रोक;

        पक्ष ngx_json_सङ्ख्या_int:
            यदि (*p >= '0' && *p <= '9') {
                रोक;
            }

            यदि (*p == '.') {
                state = ngx_json_सङ्ख्या_frac;
                रोक;
            }

            यदि (*p == 'e' || *p == 'E') {
                /* No sense, but permitted by RFC */
                state = ngx_json_सङ्ख्या_exp;
                रोक;
            }

            p--;

            rc = ngx_json_end_सङ्ख्या(ctx, start, p + 1, &state);
            यदि (rc!= NGX_OK) {
                फर्काऊ rc;
            }

            रोक;

        पक्ष ngx_json_सङ्ख्या_frac:
            यदि (*p >= '0' && *p <= '9') {
                state = ngx_json_सङ्ख्या_frac_digit;
                रोक;
            }

            फर्काऊ NGX_DECLINED;

        पक्ष ngx_json_सङ्ख्या_frac_digit:
            यदि (*p >= '0' && *p <= '9') {
                रोक;
            }

            यदि (*p == 'e' || *p == 'E') {
                state = ngx_json_सङ्ख्या_exp;
                रोक;
            }

            p--;

            rc = ngx_json_end_सङ्ख्या(ctx, start, p + 1, &state);
            यदि (rc != NGX_OK) {
                फर्काऊ rc;
            }

            रोक;

        पक्ष ngx_json_सङ्ख्या_exp:
            यदि (*p == '-' || *p == '+') {
                state = ngx_json_सङ्ख्या_exp_plusminus;
                रोक;
            }

            यदि (*p >= '0' && *p <= '9') {
                state = ngx_json_सङ्ख्या_exp_digit;
                रोक;
            }

            फर्काऊ NGX_DECLINED;

        पक्ष ngx_json_सङ्ख्या_exp_plusminus:
            यदि (*p >= '0' && *p <= '9') {
                state = ngx_json_सङ्ख्या_exp_digit;
                रोक;
            }

            फर्काऊ NGX_DECLINED;

        पक्ष ngx_json_सङ्ख्या_exp_digit:
            यदि (*p >= '0' && *p <= '9') {
                रोक;
            }

            p--;

            rc = ngx_json_end_सङ्ख्या(ctx, start, p + 1, &state);
            यदि (rc != NGX_OK) {
                फर्काऊ rc;
            }

            रोक;

        पक्ष ngx_json_true_t:
            यदि (*p == 'r') {
                state = ngx_json_true_tr;
                रोक;
            }

            फर्काऊ NGX_DECLINED;

        पक्ष ngx_json_true_tr:
            यदि (*p == 'u') {
                state = ngx_json_true_tru;
                रोक;
            }

            फर्काऊ NGX_DECLINED;

        पक्ष ngx_json_true_tru:
            यदि (*p == 'e') {
                rc = ngx_json_emit(ctx, NGX_JSON_मान_BOOL, start,
                                  p + 1 - start);
                यदि (rc != NGX_OK) {
                    फर्काऊ rc;
                }

                state = ngx_json_after_मान(ctx);
                रोक;
            }

            फर्काऊ NGX_DECLINED;

        पक्ष ngx_json_false_f:
            यदि (*p == 'a') {
                state = ngx_json_false_fa;
                रोक;
            }

            फर्काऊ NGX_DECLINED;

        पक्ष ngx_json_false_fa:
            यदि (*p == 'l') {
                state = ngx_json_false_fal;
                रोक;
            }

            फर्काऊ NGX_DECLINED;

        पक्ष ngx_json_false_fal:
            यदि (*p == 's') {
                state = ngx_json_false_fals;
                रोक;
            }

            फर्काऊ NGX_DECLINED;

        पक्ष ngx_json_false_fals:
            यदि (*p == 'e') {
                rc = ngx_json_emit(ctx, NGX_JSON_मान_BOOL, start,
                                  p + 1 - start);
                यदि (rc != NGX_OK) {
                    फर्काऊ rc;
                }

                state = ngx_json_after_मान(ctx);
                रोक;
            }

            फर्काऊ NGX_DECLINED;

        पक्ष ngx_json_null_n:
            यदि (*p == 'u') {
                state = ngx_json_null_nu;
                रोक;
            }

            फर्काऊ NGX_DECLINED;

        पक्ष ngx_json_null_nu:
            यदि (*p == 'l') {
                state = ngx_json_null_nul;
                रोक;
            }

            फर्काऊ NGX_DECLINED;

        पक्ष ngx_json_null_nul:
            यदि (*p == 'l') {
                rc = ngx_json_emit(ctx, NGX_JSON_मान_NULL, start,
                                  p + 1 - start);
                यदि (rc != NGX_OK) {
                    फर्काऊ rc;
                }

                state = ngx_json_after_मान(ctx);
                रोक;
            }

            फर्काऊ NGX_DECLINED;

        पक्ष ngx_json_done:
            यदि (ngx_json_ws(*p)) {
                रोक;
            }

            फर्काऊ NGX_DECLINED;
        }

        skip_depth = ctx->skip_until_depth;
    }

    /* a bare top-level number is not self-delimited; emit it at end of input */

    यदि (ctx->depth == 0
        && (state == ngx_json_सङ्ख्या_int_zero
            || state == ngx_json_सङ्ख्या_int
            || state == ngx_json_सङ्ख्या_frac_digit
            || state == ngx_json_सङ्ख्या_exp_digit))
    {
        rc = ngx_json_emit(ctx, NGX_JSON_मान_सङ्ख्या, start, last - start);
        यदि (rc != NGX_OK) {
            फर्काऊ rc;
        }

        state = ngx_json_done;
    }

    ctx->state = state;

    यदि (state == ngx_json_done) {
        फर्काऊ NGX_OK;
    }

    फर्काऊ NGX_DECLINED;
}


ngx_int_t
ngx_json_parse(ngx_pool_t *pool, ngx_str_t *json, ngx_json_handler_pt handler,
    शून्य *data)
{
    ngx_json_ctx_t  jctx;

    ngx_json_ctx_init(&jctx, pool);

    jctx.data = data;
    jctx.handler = handler;

    फर्काऊ ngx_json_parse_ctx(&jctx, json->data, json->len);
}


स्थिर ngx_inline ngx_int_t
ngx_json_end_सङ्ख्या(ngx_json_ctx_t *ctx, u_char *start, u_char *end,
    ngx_json_state_e *state)
{
    ngx_int_t  rc;

    rc = ngx_json_emit(ctx, NGX_JSON_मान_सङ्ख्या, start, end - start);
    यदि (rc != NGX_OK) {
        फर्काऊ rc;
    }

    *state = ngx_json_after_मान(ctx);

    फर्काऊ NGX_OK;
}


स्थिर ngx_inline ngx_int_t
ngx_json_emit(ngx_json_ctx_t *ctx, ngx_json_event_e event, u_char *data,
    माप_प्रकार len)
{
    ngx_int_t  rc;
    ngx_str_t  token;

    यदि (ngx_json_skipping(ctx->skip_until_depth)) {
        फर्काऊ NGX_OK;
    }

    यदि (ctx->handler == रिक्त) {
        फर्काऊ NGX_OK;
    }

    token.data = data;
    token.len = len;
    rc = ctx->handler(ctx, event, &token);

    यदि (rc == NGX_JSON_SKIP) {

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

        पक्ष NGX_JSON_साँचो:
        पक्ष NGX_JSON_साँचो_ESCAPED:
        पक्ष NGX_JSON_OBJECT_OPEN:
        पक्ष NGX_JSON_ARRAY_OPEN:
            ctx->skip_until_depth = ctx->depth;
            रोक;

        पक्ष NGX_JSON_मान_STRING:
        पक्ष NGX_JSON_मान_STRING_ESCAPED:
        पक्ष NGX_JSON_मान_सङ्ख्या:
        पक्ष NGX_JSON_मान_BOOL:
        पक्ष NGX_JSON_मान_NULL:
            यदि (ctx->depth > 0
                && ctx->थाक[ctx->depth - 1] == ngx_json_ctx_array
            ) {
                ctx->skip_until_depth = ctx->depth - 1;
            }
            रोक;

        स्वभाव:
            रोक;
        }

        फर्काऊ NGX_OK;
    }

    फर्काऊ rc;
}


स्थिर ngx_inline ngx_json_state_e
ngx_json_after_मान(ngx_json_ctx_t *ctx)
{
    यदि (ctx->depth == 0) {
        फर्काऊ ngx_json_done;
    }

    यदि (ctx->थाक[ctx->depth - 1] == ngx_json_ctx_object) {
        फर्काऊ ngx_json_object_मान_separator;
    }

    फर्काऊ ngx_json_array_मान_separator;
}


स्थिर ngx_inline ngx_int_t
ngx_json_close(ngx_json_ctx_t *ctx, ngx_json_event_e event,
    ngx_json_state_e *state, u_char *p)
{
    ngx_int_t  rc;

    ctx->depth--;

    rc = ngx_json_emit(ctx, event, p, 1);
    यदि (rc != NGX_OK) {
        फर्काऊ rc;
    }

    *state = ngx_json_after_मान(ctx);

    फर्काऊ NGX_OK;
}


स्थिर ngx_int_t
ngx_json_थप_state(ngx_json_ctx_t *ctx, ngx_json_container_e container)
{
    ngx_json_container_e  *थाक;

    यदि (ctx->depth >= ctx->max_depth) {
        फर्काऊ NGX_DECLINED;
    }

    यदि (ctx->थाक == रिक्त) {
        थाक = ngx_palloc(ctx->pool,
                           ctx->max_depth * परिमाण(ngx_json_container_e));
        यदि (थाक == रिक्त) {
            फर्काऊ NGX_ERROR;
        }

        ctx->थाक = थाक;
    }

    ctx->थाक[ctx->depth] = container;
    ctx->depth++;

    फर्काऊ NGX_OK;
}
