
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


स्थिर ngx_inline ngx_int_t ngx_json_end_संख्या(ngx_json_ctx_t *ctx,
    u_char *start, u_char *end, ngx_json_state_e *state);
स्थिर ngx_inline ngx_int_t ngx_json_emit(ngx_json_ctx_t *ctx,
    ngx_json_event_e event, u_char *data, माप_प्रकार len);
स्थिर ngx_inline ngx_json_state_e ngx_json_after_मूल्य(ngx_json_ctx_t *ctx);
स्थिर ngx_inline ngx_int_t ngx_json_close(ngx_json_ctx_t *ctx,
    ngx_json_event_e event, ngx_json_state_e *state, u_char *p);
स्थिर ngx_int_t ngx_json_ढकला_state(ngx_json_ctx_t *ctx,
    ngx_json_container_e container);


शून्य
ngx_json_ctx_init(ngx_json_ctx_t *ctx, ngx_pool_t *pool)
{
    ctx->state = ngx_json_start;
    ctx->pool = pool;
    ctx->handler = रिक्त;
    ctx->data = रिक्त;
    ctx->codepoint = 0;
    ctx->hex_डावा = 0;

    ctx->depth = 0;
    ctx->skip_until_depth = NGX_JSON_NOT_SKIPPING;
    ctx->max_depth = NGX_JSON_DEFAULT_MAX_DEPTH;

    /* stack is allocated lazily on first push, sized to max_depth */

    ctx->चळत = रिक्त;

    ctx->in_किल्ली = 0;
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

    जर (len == 0) {
        परत NGX_DECLINED;
    }

    जर (data == रिक्त) {
        परत NGX_ERROR;
    }

    जर (ctx->state == ngx_json_done) {
        परत NGX_DECLINED;
    }

    last = data + len;
    state = ctx->state;
    skip_depth = ctx->skip_until_depth;
    start = रिक्त;

    साठी (p = data; p < last; p++) {

        /* skipping suppresses emission only; the machine still validates */

        जर (ngx_json_skipping(skip_depth) && ctx->depth <= skip_depth
            && (state == ngx_json_object_मूल्य_separator
                || state == ngx_json_array_मूल्य_separator
                || state == ngx_json_done))
        {
            skip_depth = NGX_JSON_NOT_SKIPPING;
            ctx->skip_until_depth = NGX_JSON_NOT_SKIPPING;
            p--;

            सुरूठेवा;
        }

        निवड (state) {

        स्थिती ngx_json_start:
            जर (ngx_json_ws(*p)) {
                थांबा;
            }

            p--;
            state = ngx_json_मूल्य;
            थांबा;

        स्थिती ngx_json_object:
            जर (ngx_json_ws(*p)) {
                थांबा;
            }

            जर (*p == '}') {
                rc = ngx_json_close(ctx, NGX_JSON_OBJECT_CLOSE, &state, p);
                जर (rc != NGX_OK) {
                    परत rc;
                }

                थांबा;
            }

            /* fall through */

        स्थिती ngx_json_object_next:
            जर (ngx_json_ws(*p)) {
                थांबा;
            }

            जर (*p == '"') {
                ctx->in_किल्ली = 1;
                ctx->escaped = 0;
                start = p + 1;
                state = ngx_json_string;
                थांबा;
            }

            परत NGX_DECLINED;

        स्थिती ngx_json_object_name_separator:
            जर (ngx_json_ws(*p)) {
                थांबा;
            }

            जर (*p == ':') {
                state = ngx_json_मूल्य;
                थांबा;
            }

            परत NGX_DECLINED;

        स्थिती ngx_json_object_मूल्य_separator:
            जर (ngx_json_ws(*p)) {
                थांबा;
            }

            जर (*p == ',') {
                state = ngx_json_object_next;
                थांबा;
            }

            जर (*p == '}') {
                rc = ngx_json_close(ctx, NGX_JSON_OBJECT_CLOSE, &state, p);
                जर (rc != NGX_OK) {
                    परत rc;
                }

                थांबा;
            }

            परत NGX_DECLINED;

        स्थिती ngx_json_array_मूल्य_separator:
            जर (ngx_json_ws(*p)) {
                थांबा;
            }

            जर (*p == ',') {
                state = ngx_json_मूल्य;
                थांबा;
            }

            जर (*p == ']') {
                rc = ngx_json_close(ctx, NGX_JSON_ARRAY_CLOSE, &state, p);
                जर (rc != NGX_OK) {
                    परत rc;
                }

                थांबा;
            }

            परत NGX_DECLINED;

        स्थिती ngx_json_array:
            जर (ngx_json_ws(*p)) {
                थांबा;
            }

            जर (*p == ']') {
                rc = ngx_json_close(ctx, NGX_JSON_ARRAY_CLOSE, &state, p);
                जर (rc != NGX_OK) {
                    परत rc;
                }

                थांबा;
            }

            /* fall through */

        स्थिती ngx_json_मूल्य:
            जर (ngx_json_ws(*p)) {
                थांबा;
            }

            start = p;

            जर (*p == '"') {
                ctx->in_किल्ली = 0;
                ctx->escaped = 0;
                start = p + 1;
                state = ngx_json_string;
                थांबा;
            }

            जर (*p == '{') {
                rc = ngx_json_emit(ctx, NGX_JSON_OBJECT_OPEN, p, 1);
                जर (rc != NGX_OK) {
                    परत rc;
                }

                rc = ngx_json_ढकला_state(ctx, ngx_json_ctx_object);
                जर (rc != NGX_OK) {
                    परत rc;
                }

                state = ngx_json_object;
                थांबा;
            }

            जर (*p == '[') {
                rc = ngx_json_emit(ctx, NGX_JSON_ARRAY_OPEN, p, 1);
                जर (rc != NGX_OK) {
                    परत rc;
                }

                rc = ngx_json_ढकला_state(ctx, ngx_json_ctx_array);
                जर (rc != NGX_OK) {
                    परत rc;
                }

                state = ngx_json_array;
                थांबा;
            }

            जर (*p == '-') {
                state = ngx_json_संख्या_minus;
                थांबा;
            }

            जर (*p == '0') {
                state = ngx_json_संख्या_int_zero;
                थांबा;
            }

            जर (*p >= '1' && *p <= '9') {
                state = ngx_json_संख्या_int;
                थांबा;
            }

            जर (*p == 't') {
                state = ngx_json_true_t;
                थांबा;
            }

            जर (*p == 'f') {
                state = ngx_json_false_f;
                थांबा;
            }

            जर (*p == 'n') {
                state = ngx_json_null_n;
                थांबा;
            }

            परत NGX_DECLINED;

        स्थिती ngx_json_string:
            जर (*p == '"') {

                जर (ctx->in_किल्ली) {
                    event = ctx->escaped ? NGX_JSON_किल्ली_ESCAPED
                                         : NGX_JSON_किल्ली;

                    rc = ngx_json_emit(ctx, event, start, p - start);
                    जर (rc != NGX_OK) {
                        परत rc;
                    }

                    ctx->in_किल्ली = 0;
                    state = ngx_json_object_name_separator;

                } अन्यथा {
                    event = ctx->escaped ? NGX_JSON_मूल्य_STRING_ESCAPED
                                         : NGX_JSON_मूल्य_STRING;

                    rc = ngx_json_emit(ctx, event, start, p - start);
                    जर (rc != NGX_OK) {
                        परत rc;
                    }

                    state = ngx_json_after_मूल्य(ctx);
                }

                थांबा;
            }

            जर (*p == '\\') {
                ctx->escaped = 1;
                state = ngx_json_escaped;
                थांबा;
            }

            जर (*p < ' ') {
                परत NGX_DECLINED;
            }

            थांबा;

        स्थिती ngx_json_escaped:
            जर (*p == '"' || *p == '\\' || *p == '/' || *p == 'b'
                || *p == 'f' || *p == 'n' || *p == 'r' || *p == 't')
            {
                state = ngx_json_string;
                थांबा;
            }

            जर (*p == 'u') {
                ctx->codepoint = 0;
                ctx->hex_डावा = 4;
                state = ngx_json_escaped_hex;
                थांबा;
            }

            परत NGX_DECLINED;

        स्थिती ngx_json_escaped_hex:

            /* accumulate the four hex digits of the first \uXXXX escape */

            hd = ngx_json_hex_digit(*p);
            जर (hd == NGX_ERROR) {
                परत NGX_DECLINED;
            }

            ctx->codepoint = (ctx->codepoint << 4) | (uint32_t) hd;

            जर (--ctx->hex_डावा > 0) {
                थांबा;
            }

            जर (ctx->codepoint >= 0xD800 && ctx->codepoint <= 0xDBFF) {
                /* high surrogate - must be followed by \uDC00..\uDFFF */
                state = ngx_json_surrogate_start;
                थांबा;
            }

            जर (ctx->codepoint >= 0xDC00 && ctx->codepoint <= 0xDFFF) {
                /* lone low surrogate */
                परत NGX_DECLINED;
            }

            state = ngx_json_string;
            थांबा;

        स्थिती ngx_json_surrogate_start:
            जर (*p == '\\') {
                state = ngx_json_surrogate_u;
                थांबा;
            }

            /* high surrogate not followed by \uXXXX */
            परत NGX_DECLINED;

        स्थिती ngx_json_surrogate_u:
            जर (*p == 'u') {
                ctx->codepoint = 0;
                ctx->hex_डावा = 4;
                state = ngx_json_surrogate_hex;
                थांबा;
            }

            /* high surrogate followed by non-\u escape */
            परत NGX_DECLINED;

        स्थिती ngx_json_surrogate_hex:

            /* accumulate the four hex digits of the low \uXXXX escape */

            hd = ngx_json_hex_digit(*p);
            जर (hd == NGX_ERROR) {
                परत NGX_DECLINED;
            }

            ctx->codepoint = (ctx->codepoint << 4) | (uint32_t) hd;

            जर (--ctx->hex_डावा > 0) {
                थांबा;
            }

            जर (ctx->codepoint >= 0xDC00 && ctx->codepoint <= 0xDFFF) {
                /* valid surrogate pair */
                state = ngx_json_string;
                थांबा;
            }

            /* high surrogate not followed by low surrogate */
            परत NGX_DECLINED;

        स्थिती ngx_json_संख्या_minus:
            जर (*p >= '1' && *p <= '9') {
                state = ngx_json_संख्या_int;
                थांबा;
            }

            जर (*p == '0') {
                state = ngx_json_संख्या_int_zero;
                थांबा;
            }

            परत NGX_DECLINED;

        स्थिती ngx_json_संख्या_int_zero:
            जर (*p == '.') {
                state = ngx_json_संख्या_frac;
                थांबा;
            }

            जर (*p == 'e' || *p == 'E') {
                /* No sense, but permitted by RFC */
                state = ngx_json_संख्या_exp;
                थांबा;
            }

            p--;

            rc = ngx_json_end_संख्या(ctx, start, p + 1, &state);
            जर (rc != NGX_OK) {
                परत rc;
            }

            थांबा;

        स्थिती ngx_json_संख्या_int:
            जर (*p >= '0' && *p <= '9') {
                थांबा;
            }

            जर (*p == '.') {
                state = ngx_json_संख्या_frac;
                थांबा;
            }

            जर (*p == 'e' || *p == 'E') {
                /* No sense, but permitted by RFC */
                state = ngx_json_संख्या_exp;
                थांबा;
            }

            p--;

            rc = ngx_json_end_संख्या(ctx, start, p + 1, &state);
            जर (rc!= NGX_OK) {
                परत rc;
            }

            थांबा;

        स्थिती ngx_json_संख्या_frac:
            जर (*p >= '0' && *p <= '9') {
                state = ngx_json_संख्या_frac_digit;
                थांबा;
            }

            परत NGX_DECLINED;

        स्थिती ngx_json_संख्या_frac_digit:
            जर (*p >= '0' && *p <= '9') {
                थांबा;
            }

            जर (*p == 'e' || *p == 'E') {
                state = ngx_json_संख्या_exp;
                थांबा;
            }

            p--;

            rc = ngx_json_end_संख्या(ctx, start, p + 1, &state);
            जर (rc != NGX_OK) {
                परत rc;
            }

            थांबा;

        स्थिती ngx_json_संख्या_exp:
            जर (*p == '-' || *p == '+') {
                state = ngx_json_संख्या_exp_plusminus;
                थांबा;
            }

            जर (*p >= '0' && *p <= '9') {
                state = ngx_json_संख्या_exp_digit;
                थांबा;
            }

            परत NGX_DECLINED;

        स्थिती ngx_json_संख्या_exp_plusminus:
            जर (*p >= '0' && *p <= '9') {
                state = ngx_json_संख्या_exp_digit;
                थांबा;
            }

            परत NGX_DECLINED;

        स्थिती ngx_json_संख्या_exp_digit:
            जर (*p >= '0' && *p <= '9') {
                थांबा;
            }

            p--;

            rc = ngx_json_end_संख्या(ctx, start, p + 1, &state);
            जर (rc != NGX_OK) {
                परत rc;
            }

            थांबा;

        स्थिती ngx_json_true_t:
            जर (*p == 'r') {
                state = ngx_json_true_tr;
                थांबा;
            }

            परत NGX_DECLINED;

        स्थिती ngx_json_true_tr:
            जर (*p == 'u') {
                state = ngx_json_true_tru;
                थांबा;
            }

            परत NGX_DECLINED;

        स्थिती ngx_json_true_tru:
            जर (*p == 'e') {
                rc = ngx_json_emit(ctx, NGX_JSON_मूल्य_BOOL, start,
                                  p + 1 - start);
                जर (rc != NGX_OK) {
                    परत rc;
                }

                state = ngx_json_after_मूल्य(ctx);
                थांबा;
            }

            परत NGX_DECLINED;

        स्थिती ngx_json_false_f:
            जर (*p == 'a') {
                state = ngx_json_false_fa;
                थांबा;
            }

            परत NGX_DECLINED;

        स्थिती ngx_json_false_fa:
            जर (*p == 'l') {
                state = ngx_json_false_fal;
                थांबा;
            }

            परत NGX_DECLINED;

        स्थिती ngx_json_false_fal:
            जर (*p == 's') {
                state = ngx_json_false_fals;
                थांबा;
            }

            परत NGX_DECLINED;

        स्थिती ngx_json_false_fals:
            जर (*p == 'e') {
                rc = ngx_json_emit(ctx, NGX_JSON_मूल्य_BOOL, start,
                                  p + 1 - start);
                जर (rc != NGX_OK) {
                    परत rc;
                }

                state = ngx_json_after_मूल्य(ctx);
                थांबा;
            }

            परत NGX_DECLINED;

        स्थिती ngx_json_null_n:
            जर (*p == 'u') {
                state = ngx_json_null_nu;
                थांबा;
            }

            परत NGX_DECLINED;

        स्थिती ngx_json_null_nu:
            जर (*p == 'l') {
                state = ngx_json_null_nul;
                थांबा;
            }

            परत NGX_DECLINED;

        स्थिती ngx_json_null_nul:
            जर (*p == 'l') {
                rc = ngx_json_emit(ctx, NGX_JSON_मूल्य_NULL, start,
                                  p + 1 - start);
                जर (rc != NGX_OK) {
                    परत rc;
                }

                state = ngx_json_after_मूल्य(ctx);
                थांबा;
            }

            परत NGX_DECLINED;

        स्थिती ngx_json_done:
            जर (ngx_json_ws(*p)) {
                थांबा;
            }

            परत NGX_DECLINED;
        }

        skip_depth = ctx->skip_until_depth;
    }

    /* a bare top-level number is not self-delimited; emit it at end of input */

    जर (ctx->depth == 0
        && (state == ngx_json_संख्या_int_zero
            || state == ngx_json_संख्या_int
            || state == ngx_json_संख्या_frac_digit
            || state == ngx_json_संख्या_exp_digit))
    {
        rc = ngx_json_emit(ctx, NGX_JSON_मूल्य_संख्या, start, last - start);
        जर (rc != NGX_OK) {
            परत rc;
        }

        state = ngx_json_done;
    }

    ctx->state = state;

    जर (state == ngx_json_done) {
        परत NGX_OK;
    }

    परत NGX_DECLINED;
}


ngx_int_t
ngx_json_parse(ngx_pool_t *pool, ngx_str_t *json, ngx_json_handler_pt handler,
    शून्य *data)
{
    ngx_json_ctx_t  jctx;

    ngx_json_ctx_init(&jctx, pool);

    jctx.data = data;
    jctx.handler = handler;

    परत ngx_json_parse_ctx(&jctx, json->data, json->len);
}


स्थिर ngx_inline ngx_int_t
ngx_json_end_संख्या(ngx_json_ctx_t *ctx, u_char *start, u_char *end,
    ngx_json_state_e *state)
{
    ngx_int_t  rc;

    rc = ngx_json_emit(ctx, NGX_JSON_मूल्य_संख्या, start, end - start);
    जर (rc != NGX_OK) {
        परत rc;
    }

    *state = ngx_json_after_मूल्य(ctx);

    परत NGX_OK;
}


स्थिर ngx_inline ngx_int_t
ngx_json_emit(ngx_json_ctx_t *ctx, ngx_json_event_e event, u_char *data,
    माप_प्रकार len)
{
    ngx_int_t  rc;
    ngx_str_t  token;

    जर (ngx_json_skipping(ctx->skip_until_depth)) {
        परत NGX_OK;
    }

    जर (ctx->handler == रिक्त) {
        परत NGX_OK;
    }

    token.data = data;
    token.len = len;
    rc = ctx->handler(ctx, event, &token);

    जर (rc == NGX_JSON_SKIP) {

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

        निवड (event) {

        स्थिती NGX_JSON_किल्ली:
        स्थिती NGX_JSON_किल्ली_ESCAPED:
        स्थिती NGX_JSON_OBJECT_OPEN:
        स्थिती NGX_JSON_ARRAY_OPEN:
            ctx->skip_until_depth = ctx->depth;
            थांबा;

        स्थिती NGX_JSON_मूल्य_STRING:
        स्थिती NGX_JSON_मूल्य_STRING_ESCAPED:
        स्थिती NGX_JSON_मूल्य_संख्या:
        स्थिती NGX_JSON_मूल्य_BOOL:
        स्थिती NGX_JSON_मूल्य_NULL:
            जर (ctx->depth > 0
                && ctx->चळत[ctx->depth - 1] == ngx_json_ctx_array
            ) {
                ctx->skip_until_depth = ctx->depth - 1;
            }
            थांबा;

        पूर्वनिर्धारित:
            थांबा;
        }

        परत NGX_OK;
    }

    परत rc;
}


स्थिर ngx_inline ngx_json_state_e
ngx_json_after_मूल्य(ngx_json_ctx_t *ctx)
{
    जर (ctx->depth == 0) {
        परत ngx_json_done;
    }

    जर (ctx->चळत[ctx->depth - 1] == ngx_json_ctx_object) {
        परत ngx_json_object_मूल्य_separator;
    }

    परत ngx_json_array_मूल्य_separator;
}


स्थिर ngx_inline ngx_int_t
ngx_json_close(ngx_json_ctx_t *ctx, ngx_json_event_e event,
    ngx_json_state_e *state, u_char *p)
{
    ngx_int_t  rc;

    ctx->depth--;

    rc = ngx_json_emit(ctx, event, p, 1);
    जर (rc != NGX_OK) {
        परत rc;
    }

    *state = ngx_json_after_मूल्य(ctx);

    परत NGX_OK;
}


स्थिर ngx_int_t
ngx_json_ढकला_state(ngx_json_ctx_t *ctx, ngx_json_container_e container)
{
    ngx_json_container_e  *चळत;

    जर (ctx->depth >= ctx->max_depth) {
        परत NGX_DECLINED;
    }

    जर (ctx->चळत == रिक्त) {
        चळत = ngx_palloc(ctx->pool,
                           ctx->max_depth * आकार(ngx_json_container_e));
        जर (चळत == रिक्त) {
            परत NGX_ERROR;
        }

        ctx->चळत = चळत;
    }

    ctx->चळत[ctx->depth] = container;
    ctx->depth++;

    परत NGX_OK;
}
