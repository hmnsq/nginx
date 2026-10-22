
/*
 * Copyright (C) Maxim Dounin
 * Copyright (C) Nginx, Inc.
 */


/*
 * JSON parser. RFC 8259 for the JSON grammar.

 * Site http://json.org/ may be useful too.
 */


#समावेश <ngx_config.h>
#समावेश <ngx_core.h>
#समावेश <ngx_json_parse.h>


#घोषणा NGX_JSON_NOT_SKIPPING  (ngx_uint_t) -1
#घोषणा ngx_json_skipping(d)   ((d) != NGX_JSON_NOT_SKIPPING)
#घोषणा ngx_json_ws(c)  ((c) == ' ' || (c) == '\t' || (c) == CR || (c) == LF)


अटल ngx_inline ngx_int_t ngx_json_end_संख्या(ngx_json_ctx_t *ctx,
    u_char *start, u_char *end, ngx_json_state_e *state);
अटल ngx_inline ngx_int_t ngx_json_emit(ngx_json_ctx_t *ctx,
    ngx_json_event_e event, u_char *तथ्य, माप_प्रकार len);
अटल ngx_inline ngx_json_state_e ngx_json_after_मूल्य(ngx_json_ctx_t *ctx);
अटल ngx_inline ngx_int_t ngx_json_close(ngx_json_ctx_t *ctx,
    ngx_json_event_e event, ngx_json_state_e *state, u_char *p);
अटल ngx_int_t ngx_json_धकेलो_state(ngx_json_ctx_t *ctx,
    ngx_json_container_e container);


व्योम
ngx_json_ctx_init(ngx_json_ctx_t *ctx, ngx_pool_t *pool)
{
    ctx->state = ngx_json_start;
    ctx->pool = pool;
    ctx->handler = शून्य;
    ctx->तथ्य = शून्य;
    ctx->codepoint = 0;
    ctx->hex_बायाँ = 0;

    ctx->depth = 0;
    ctx->skip_until_depth = NGX_JSON_NOT_SKIPPING;
    ctx->max_depth = NGX_JSON_DEFAULT_MAX_DEPTH;

    /* stack is allocated lazily on first push, sized to max_depth */

    ctx->ढेर = शून्य;

    ctx->in_कुंजी = 0;
    ctx->escaped = 0;
}


ngx_int_t
ngx_json_parse_ctx(ngx_json_ctx_t *ctx, u_char *तथ्य, माप_प्रकार len)
{
    u_char            *p, *last, *start;
    ngx_int_t          rc, hd;
    ngx_uint_t         skip_depth;
    ngx_json_event_e   event;
    ngx_json_state_e   state;

    अगर (len == 0) {
        वापस NGX_DECLINED;
    }

    अगर (तथ्य == शून्य) {
        वापस NGX_ERROR;
    }

    अगर (ctx->state == ngx_json_done) {
        वापस NGX_DECLINED;
    }

    last = तथ्य + len;
    state = ctx->state;
    skip_depth = ctx->skip_until_depth;
    start = शून्य;

    क्रम (p = तथ्य; p < last; p++) {

        /* skipping suppresses emission only; the machine still validates */

        अगर (ngx_json_skipping(skip_depth) && ctx->depth <= skip_depth
            && (state == ngx_json_object_मूल्य_separator
                || state == ngx_json_array_मूल्य_separator
                || state == ngx_json_done))
        {
            skip_depth = NGX_JSON_NOT_SKIPPING;
            ctx->skip_until_depth = NGX_JSON_NOT_SKIPPING;
            p--;

            जारी;
        }

        चयन (state) {

        हाल ngx_json_start:
            अगर (ngx_json_ws(*p)) {
                अवरोध;
            }

            p--;
            state = ngx_json_मूल्य;
            अवरोध;

        हाल ngx_json_object:
            अगर (ngx_json_ws(*p)) {
                अवरोध;
            }

            अगर (*p == '}') {
                rc = ngx_json_close(ctx, NGX_JSON_OBJECT_CLOSE, &state, p);
                अगर (rc != NGX_OK) {
                    वापस rc;
                }

                अवरोध;
            }

            /* fall through */

        हाल ngx_json_object_अग्र:
            अगर (ngx_json_ws(*p)) {
                अवरोध;
            }

            अगर (*p == '"') {
                ctx->in_कुंजी = 1;
                ctx->escaped = 0;
                start = p + 1;
                state = ngx_json_string;
                अवरोध;
            }

            वापस NGX_DECLINED;

        हाल ngx_json_object_name_separator:
            अगर (ngx_json_ws(*p)) {
                अवरोध;
            }

            अगर (*p == ':') {
                state = ngx_json_मूल्य;
                अवरोध;
            }

            वापस NGX_DECLINED;

        हाल ngx_json_object_मूल्य_separator:
            अगर (ngx_json_ws(*p)) {
                अवरोध;
            }

            अगर (*p == ',') {
                state = ngx_json_object_अग्र;
                अवरोध;
            }

            अगर (*p == '}') {
                rc = ngx_json_close(ctx, NGX_JSON_OBJECT_CLOSE, &state, p);
                अगर (rc != NGX_OK) {
                    वापस rc;
                }

                अवरोध;
            }

            वापस NGX_DECLINED;

        हाल ngx_json_array_मूल्य_separator:
            अगर (ngx_json_ws(*p)) {
                अवरोध;
            }

            अगर (*p == ',') {
                state = ngx_json_मूल्य;
                अवरोध;
            }

            अगर (*p == ']') {
                rc = ngx_json_close(ctx, NGX_JSON_ARRAY_CLOSE, &state, p);
                अगर (rc != NGX_OK) {
                    वापस rc;
                }

                अवरोध;
            }

            वापस NGX_DECLINED;

        हाल ngx_json_array:
            अगर (ngx_json_ws(*p)) {
                अवरोध;
            }

            अगर (*p == ']') {
                rc = ngx_json_close(ctx, NGX_JSON_ARRAY_CLOSE, &state, p);
                अगर (rc != NGX_OK) {
                    वापस rc;
                }

                अवरोध;
            }

            /* fall through */

        हाल ngx_json_मूल्य:
            अगर (ngx_json_ws(*p)) {
                अवरोध;
            }

            start = p;

            अगर (*p == '"') {
                ctx->in_कुंजी = 0;
                ctx->escaped = 0;
                start = p + 1;
                state = ngx_json_string;
                अवरोध;
            }

            अगर (*p == '{') {
                rc = ngx_json_emit(ctx, NGX_JSON_OBJECT_OPEN, p, 1);
                अगर (rc != NGX_OK) {
                    वापस rc;
                }

                rc = ngx_json_धकेलो_state(ctx, ngx_json_ctx_object);
                अगर (rc != NGX_OK) {
                    वापस rc;
                }

                state = ngx_json_object;
                अवरोध;
            }

            अगर (*p == '[') {
                rc = ngx_json_emit(ctx, NGX_JSON_ARRAY_OPEN, p, 1);
                अगर (rc != NGX_OK) {
                    वापस rc;
                }

                rc = ngx_json_धकेलो_state(ctx, ngx_json_ctx_array);
                अगर (rc != NGX_OK) {
                    वापस rc;
                }

                state = ngx_json_array;
                अवरोध;
            }

            अगर (*p == '-') {
                state = ngx_json_संख्या_minus;
                अवरोध;
            }

            अगर (*p == '0') {
                state = ngx_json_संख्या_int_zero;
                अवरोध;
            }

            अगर (*p >= '1' && *p <= '9') {
                state = ngx_json_संख्या_int;
                अवरोध;
            }

            अगर (*p == 't') {
                state = ngx_json_true_t;
                अवरोध;
            }

            अगर (*p == 'f') {
                state = ngx_json_false_f;
                अवरोध;
            }

            अगर (*p == 'n') {
                state = ngx_json_null_n;
                अवरोध;
            }

            वापस NGX_DECLINED;

        हाल ngx_json_string:
            अगर (*p == '"') {

                अगर (ctx->in_कुंजी) {
                    event = ctx->escaped ? NGX_JSON_कुंजी_ESCAPED
                                         : NGX_JSON_कुंजी;

                    rc = ngx_json_emit(ctx, event, start, p - start);
                    अगर (rc != NGX_OK) {
                        वापस rc;
                    }

                    ctx->in_कुंजी = 0;
                    state = ngx_json_object_name_separator;

                } अन्यथा {
                    event = ctx->escaped ? NGX_JSON_मूल्य_STRING_ESCAPED
                                         : NGX_JSON_मूल्य_STRING;

                    rc = ngx_json_emit(ctx, event, start, p - start);
                    अगर (rc != NGX_OK) {
                        वापस rc;
                    }

                    state = ngx_json_after_मूल्य(ctx);
                }

                अवरोध;
            }

            अगर (*p == '\\') {
                ctx->escaped = 1;
                state = ngx_json_escaped;
                अवरोध;
            }

            अगर (*p < ' ') {
                वापस NGX_DECLINED;
            }

            अवरोध;

        हाल ngx_json_escaped:
            अगर (*p == '"' || *p == '\\' || *p == '/' || *p == 'b'
                || *p == 'f' || *p == 'n' || *p == 'r' || *p == 't')
            {
                state = ngx_json_string;
                अवरोध;
            }

            अगर (*p == 'u') {
                ctx->codepoint = 0;
                ctx->hex_बायाँ = 4;
                state = ngx_json_escaped_hex;
                अवरोध;
            }

            वापस NGX_DECLINED;

        हाल ngx_json_escaped_hex:

            /* accumulate the four hex digits of the first \uXXXX escape */

            hd = ngx_json_hex_digit(*p);
            अगर (hd == NGX_ERROR) {
                वापस NGX_DECLINED;
            }

            ctx->codepoint = (ctx->codepoint << 4) | (uint32_t) hd;

            अगर (--ctx->hex_बायाँ > 0) {
                अवरोध;
            }

            अगर (ctx->codepoint >= 0xD800 && ctx->codepoint <= 0xDBFF) {
                /* high surrogate - must be followed by \uDC00..\uDFFF */
                state = ngx_json_surrogate_start;
                अवरोध;
            }

            अगर (ctx->codepoint >= 0xDC00 && ctx->codepoint <= 0xDFFF) {
                /* lone low surrogate */
                वापस NGX_DECLINED;
            }

            state = ngx_json_string;
            अवरोध;

        हाल ngx_json_surrogate_start:
            अगर (*p == '\\') {
                state = ngx_json_surrogate_u;
                अवरोध;
            }

            /* high surrogate not followed by \uXXXX */
            वापस NGX_DECLINED;

        हाल ngx_json_surrogate_u:
            अगर (*p == 'u') {
                ctx->codepoint = 0;
                ctx->hex_बायाँ = 4;
                state = ngx_json_surrogate_hex;
                अवरोध;
            }

            /* high surrogate followed by non-\u escape */
            वापस NGX_DECLINED;

        हाल ngx_json_surrogate_hex:

            /* accumulate the four hex digits of the low \uXXXX escape */

            hd = ngx_json_hex_digit(*p);
            अगर (hd == NGX_ERROR) {
                वापस NGX_DECLINED;
            }

            ctx->codepoint = (ctx->codepoint << 4) | (uint32_t) hd;

            अगर (--ctx->hex_बायाँ > 0) {
                अवरोध;
            }

            अगर (ctx->codepoint >= 0xDC00 && ctx->codepoint <= 0xDFFF) {
                /* valid surrogate pair */
                state = ngx_json_string;
                अवरोध;
            }

            /* high surrogate not followed by low surrogate */
            वापस NGX_DECLINED;

        हाल ngx_json_संख्या_minus:
            अगर (*p >= '1' && *p <= '9') {
                state = ngx_json_संख्या_int;
                अवरोध;
            }

            अगर (*p == '0') {
                state = ngx_json_संख्या_int_zero;
                अवरोध;
            }

            वापस NGX_DECLINED;

        हाल ngx_json_संख्या_int_zero:
            अगर (*p == '.') {
                state = ngx_json_संख्या_frac;
                अवरोध;
            }

            अगर (*p == 'e' || *p == 'E') {
                /* No sense, but permitted by RFC */
                state = ngx_json_संख्या_exp;
                अवरोध;
            }

            p--;

            rc = ngx_json_end_संख्या(ctx, start, p + 1, &state);
            अगर (rc != NGX_OK) {
                वापस rc;
            }

            अवरोध;

        हाल ngx_json_संख्या_int:
            अगर (*p >= '0' && *p <= '9') {
                अवरोध;
            }

            अगर (*p == '.') {
                state = ngx_json_संख्या_frac;
                अवरोध;
            }

            अगर (*p == 'e' || *p == 'E') {
                /* No sense, but permitted by RFC */
                state = ngx_json_संख्या_exp;
                अवरोध;
            }

            p--;

            rc = ngx_json_end_संख्या(ctx, start, p + 1, &state);
            अगर (rc!= NGX_OK) {
                वापस rc;
            }

            अवरोध;

        हाल ngx_json_संख्या_frac:
            अगर (*p >= '0' && *p <= '9') {
                state = ngx_json_संख्या_frac_digit;
                अवरोध;
            }

            वापस NGX_DECLINED;

        हाल ngx_json_संख्या_frac_digit:
            अगर (*p >= '0' && *p <= '9') {
                अवरोध;
            }

            अगर (*p == 'e' || *p == 'E') {
                state = ngx_json_संख्या_exp;
                अवरोध;
            }

            p--;

            rc = ngx_json_end_संख्या(ctx, start, p + 1, &state);
            अगर (rc != NGX_OK) {
                वापस rc;
            }

            अवरोध;

        हाल ngx_json_संख्या_exp:
            अगर (*p == '-' || *p == '+') {
                state = ngx_json_संख्या_exp_plusminus;
                अवरोध;
            }

            अगर (*p >= '0' && *p <= '9') {
                state = ngx_json_संख्या_exp_digit;
                अवरोध;
            }

            वापस NGX_DECLINED;

        हाल ngx_json_संख्या_exp_plusminus:
            अगर (*p >= '0' && *p <= '9') {
                state = ngx_json_संख्या_exp_digit;
                अवरोध;
            }

            वापस NGX_DECLINED;

        हाल ngx_json_संख्या_exp_digit:
            अगर (*p >= '0' && *p <= '9') {
                अवरोध;
            }

            p--;

            rc = ngx_json_end_संख्या(ctx, start, p + 1, &state);
            अगर (rc != NGX_OK) {
                वापस rc;
            }

            अवरोध;

        हाल ngx_json_true_t:
            अगर (*p == 'r') {
                state = ngx_json_true_tr;
                अवरोध;
            }

            वापस NGX_DECLINED;

        हाल ngx_json_true_tr:
            अगर (*p == 'u') {
                state = ngx_json_true_tru;
                अवरोध;
            }

            वापस NGX_DECLINED;

        हाल ngx_json_true_tru:
            अगर (*p == 'e') {
                rc = ngx_json_emit(ctx, NGX_JSON_मूल्य_BOOL, start,
                                  p + 1 - start);
                अगर (rc != NGX_OK) {
                    वापस rc;
                }

                state = ngx_json_after_मूल्य(ctx);
                अवरोध;
            }

            वापस NGX_DECLINED;

        हाल ngx_json_false_f:
            अगर (*p == 'a') {
                state = ngx_json_false_fa;
                अवरोध;
            }

            वापस NGX_DECLINED;

        हाल ngx_json_false_fa:
            अगर (*p == 'l') {
                state = ngx_json_false_fal;
                अवरोध;
            }

            वापस NGX_DECLINED;

        हाल ngx_json_false_fal:
            अगर (*p == 's') {
                state = ngx_json_false_fals;
                अवरोध;
            }

            वापस NGX_DECLINED;

        हाल ngx_json_false_fals:
            अगर (*p == 'e') {
                rc = ngx_json_emit(ctx, NGX_JSON_मूल्य_BOOL, start,
                                  p + 1 - start);
                अगर (rc != NGX_OK) {
                    वापस rc;
                }

                state = ngx_json_after_मूल्य(ctx);
                अवरोध;
            }

            वापस NGX_DECLINED;

        हाल ngx_json_null_n:
            अगर (*p == 'u') {
                state = ngx_json_null_nu;
                अवरोध;
            }

            वापस NGX_DECLINED;

        हाल ngx_json_null_nu:
            अगर (*p == 'l') {
                state = ngx_json_null_nul;
                अवरोध;
            }

            वापस NGX_DECLINED;

        हाल ngx_json_null_nul:
            अगर (*p == 'l') {
                rc = ngx_json_emit(ctx, NGX_JSON_मूल्य_NULL, start,
                                  p + 1 - start);
                अगर (rc != NGX_OK) {
                    वापस rc;
                }

                state = ngx_json_after_मूल्य(ctx);
                अवरोध;
            }

            वापस NGX_DECLINED;

        हाल ngx_json_done:
            अगर (ngx_json_ws(*p)) {
                अवरोध;
            }

            वापस NGX_DECLINED;
        }

        skip_depth = ctx->skip_until_depth;
    }

    /* a bare top-level number is not self-delimited; emit it at end of input */

    अगर (ctx->depth == 0
        && (state == ngx_json_संख्या_int_zero
            || state == ngx_json_संख्या_int
            || state == ngx_json_संख्या_frac_digit
            || state == ngx_json_संख्या_exp_digit))
    {
        rc = ngx_json_emit(ctx, NGX_JSON_मूल्य_संख्या, start, last - start);
        अगर (rc != NGX_OK) {
            वापस rc;
        }

        state = ngx_json_done;
    }

    ctx->state = state;

    अगर (state == ngx_json_done) {
        वापस NGX_OK;
    }

    वापस NGX_DECLINED;
}


ngx_int_t
ngx_json_parse(ngx_pool_t *pool, ngx_str_t *json, ngx_json_handler_pt handler,
    व्योम *तथ्य)
{
    ngx_json_ctx_t  jctx;

    ngx_json_ctx_init(&jctx, pool);

    jctx.तथ्य = तथ्य;
    jctx.handler = handler;

    वापस ngx_json_parse_ctx(&jctx, json->तथ्य, json->len);
}


अटल ngx_inline ngx_int_t
ngx_json_end_संख्या(ngx_json_ctx_t *ctx, u_char *start, u_char *end,
    ngx_json_state_e *state)
{
    ngx_int_t  rc;

    rc = ngx_json_emit(ctx, NGX_JSON_मूल्य_संख्या, start, end - start);
    अगर (rc != NGX_OK) {
        वापस rc;
    }

    *state = ngx_json_after_मूल्य(ctx);

    वापस NGX_OK;
}


अटल ngx_inline ngx_int_t
ngx_json_emit(ngx_json_ctx_t *ctx, ngx_json_event_e event, u_char *तथ्य,
    माप_प्रकार len)
{
    ngx_int_t  rc;
    ngx_str_t  token;

    अगर (ngx_json_skipping(ctx->skip_until_depth)) {
        वापस NGX_OK;
    }

    अगर (ctx->handler == शून्य) {
        वापस NGX_OK;
    }

    token.तथ्य = तथ्य;
    token.len = len;
    rc = ctx->handler(ctx, event, &token);

    अगर (rc == NGX_JSON_SKIP) {

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

        चयन (event) {

        हाल NGX_JSON_कुंजी:
        हाल NGX_JSON_कुंजी_ESCAPED:
        हाल NGX_JSON_OBJECT_OPEN:
        हाल NGX_JSON_ARRAY_OPEN:
            ctx->skip_until_depth = ctx->depth;
            अवरोध;

        हाल NGX_JSON_मूल्य_STRING:
        हाल NGX_JSON_मूल्य_STRING_ESCAPED:
        हाल NGX_JSON_मूल्य_संख्या:
        हाल NGX_JSON_मूल्य_BOOL:
        हाल NGX_JSON_मूल्य_NULL:
            अगर (ctx->depth > 0
                && ctx->ढेर[ctx->depth - 1] == ngx_json_ctx_array
            ) {
                ctx->skip_until_depth = ctx->depth - 1;
            }
            अवरोध;

        शेष:
            अवरोध;
        }

        वापस NGX_OK;
    }

    वापस rc;
}


अटल ngx_inline ngx_json_state_e
ngx_json_after_मूल्य(ngx_json_ctx_t *ctx)
{
    अगर (ctx->depth == 0) {
        वापस ngx_json_done;
    }

    अगर (ctx->ढेर[ctx->depth - 1] == ngx_json_ctx_object) {
        वापस ngx_json_object_मूल्य_separator;
    }

    वापस ngx_json_array_मूल्य_separator;
}


अटल ngx_inline ngx_int_t
ngx_json_close(ngx_json_ctx_t *ctx, ngx_json_event_e event,
    ngx_json_state_e *state, u_char *p)
{
    ngx_int_t  rc;

    ctx->depth--;

    rc = ngx_json_emit(ctx, event, p, 1);
    अगर (rc != NGX_OK) {
        वापस rc;
    }

    *state = ngx_json_after_मूल्य(ctx);

    वापस NGX_OK;
}


अटल ngx_int_t
ngx_json_धकेलो_state(ngx_json_ctx_t *ctx, ngx_json_container_e container)
{
    ngx_json_container_e  *ढेर;

    अगर (ctx->depth >= ctx->max_depth) {
        वापस NGX_DECLINED;
    }

    अगर (ctx->ढेर == शून्य) {
        ढेर = ngx_palloc(ctx->pool,
                           ctx->max_depth * माप(ngx_json_container_e));
        अगर (ढेर == शून्य) {
            वापस NGX_ERROR;
        }

        ctx->ढेर = ढेर;
    }

    ctx->ढेर[ctx->depth] = container;
    ctx->depth++;

    वापस NGX_OK;
}
