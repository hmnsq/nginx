
/*
 * Copyright (C) Maxim Dounin
 * Copyright (C) Nginx, Inc.
 */


/*
 * JSON parser. RFC 8259 for the JSON grammar.

 * Site http://json.org/ may be useful too.
 */


#അന്തര്ഭാവ <ngx_config.h>
#അന്തര്ഭാവ <ngx_core.h>
#അന്തര്ഭാവ <ngx_json_parse.h>


#define NGX_JSON_NOT_SKIPPING  (ngx_uint_t) -1
#define ngx_json_skipping(d)   ((d) != NGX_JSON_NOT_SKIPPING)
#define ngx_json_ws(c)  ((c) == ' ' || (c) == '\t' || (c) == CR || (c) == LF)


സ്ഥിതം ngx_inline ngx_int_t ngx_json_end_സംഖ്യ(ngx_json_ctx_t *ctx,
    u_char *start, u_char *end, ngx_json_state_e *state);
സ്ഥിതം ngx_inline ngx_int_t ngx_json_emit(ngx_json_ctx_t *ctx,
    ngx_json_event_e event, u_char *data, size_t len);
സ്ഥിതം ngx_inline ngx_json_state_e ngx_json_after_മൂല്യം(ngx_json_ctx_t *ctx);
സ്ഥിതം ngx_inline ngx_int_t ngx_json_close(ngx_json_ctx_t *ctx,
    ngx_json_event_e event, ngx_json_state_e *state, u_char *p);
സ്ഥിതം ngx_int_t ngx_json_തള്ളുക_state(ngx_json_ctx_t *ctx,
    ngx_json_container_e container);


ശൂന്യം
ngx_json_ctx_init(ngx_json_ctx_t *ctx, ngx_pool_t *pool)
{
    ctx->state = ngx_json_start;
    ctx->pool = pool;
    ctx->handler = ശൂന്യ;
    ctx->data = ശൂന്യ;
    ctx->codepoint = 0;
    ctx->hex_ഇടത് = 0;

    ctx->depth = 0;
    ctx->skip_until_depth = NGX_JSON_NOT_SKIPPING;
    ctx->max_depth = NGX_JSON_DEFAULT_MAX_DEPTH;

    /* stack is allocated lazily on first push, sized to max_depth */

    ctx->അട്ടി = ശൂന്യ;

    ctx->in_താക്കോൽ = 0;
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

    എങ്കിൽ (len == 0) {
        തിരികെ NGX_DECLINED;
    }

    എങ്കിൽ (data == ശൂന്യ) {
        തിരികെ NGX_ERROR;
    }

    എങ്കിൽ (ctx->state == ngx_json_done) {
        തിരികെ NGX_DECLINED;
    }

    last = data + len;
    state = ctx->state;
    skip_depth = ctx->skip_until_depth;
    start = ശൂന്യ;

    വേണ്ടി (p = data; p < last; p++) {

        /* skipping suppresses emission only; the machine still validates */

        എങ്കിൽ (ngx_json_skipping(skip_depth) && ctx->depth <= skip_depth
            && (state == ngx_json_object_മൂല്യം_separator
                || state == ngx_json_array_മൂല്യം_separator
                || state == ngx_json_done))
        {
            skip_depth = NGX_JSON_NOT_SKIPPING;
            ctx->skip_until_depth = NGX_JSON_NOT_SKIPPING;
            p--;

            തുടരുക;
        }

        തിരഞ്ഞെടുപ്പ് (state) {

        അവസ്ഥ ngx_json_start:
            എങ്കിൽ (ngx_json_ws(*p)) {
                നിർത്തുക;
            }

            p--;
            state = ngx_json_മൂല്യം;
            നിർത്തുക;

        അവസ്ഥ ngx_json_object:
            എങ്കിൽ (ngx_json_ws(*p)) {
                നിർത്തുക;
            }

            എങ്കിൽ (*p == '}') {
                rc = ngx_json_close(ctx, NGX_JSON_OBJECT_CLOSE, &state, p);
                എങ്കിൽ (rc != NGX_OK) {
                    തിരികെ rc;
                }

                നിർത്തുക;
            }

            /* fall through */

        അവസ്ഥ ngx_json_object_next:
            എങ്കിൽ (ngx_json_ws(*p)) {
                നിർത്തുക;
            }

            എങ്കിൽ (*p == '"') {
                ctx->in_താക്കോൽ = 1;
                ctx->escaped = 0;
                start = p + 1;
                state = ngx_json_string;
                നിർത്തുക;
            }

            തിരികെ NGX_DECLINED;

        അവസ്ഥ ngx_json_object_name_separator:
            എങ്കിൽ (ngx_json_ws(*p)) {
                നിർത്തുക;
            }

            എങ്കിൽ (*p == ':') {
                state = ngx_json_മൂല്യം;
                നിർത്തുക;
            }

            തിരികെ NGX_DECLINED;

        അവസ്ഥ ngx_json_object_മൂല്യം_separator:
            എങ്കിൽ (ngx_json_ws(*p)) {
                നിർത്തുക;
            }

            എങ്കിൽ (*p == ',') {
                state = ngx_json_object_next;
                നിർത്തുക;
            }

            എങ്കിൽ (*p == '}') {
                rc = ngx_json_close(ctx, NGX_JSON_OBJECT_CLOSE, &state, p);
                എങ്കിൽ (rc != NGX_OK) {
                    തിരികെ rc;
                }

                നിർത്തുക;
            }

            തിരികെ NGX_DECLINED;

        അവസ്ഥ ngx_json_array_മൂല്യം_separator:
            എങ്കിൽ (ngx_json_ws(*p)) {
                നിർത്തുക;
            }

            എങ്കിൽ (*p == ',') {
                state = ngx_json_മൂല്യം;
                നിർത്തുക;
            }

            എങ്കിൽ (*p == ']') {
                rc = ngx_json_close(ctx, NGX_JSON_ARRAY_CLOSE, &state, p);
                എങ്കിൽ (rc != NGX_OK) {
                    തിരികെ rc;
                }

                നിർത്തുക;
            }

            തിരികെ NGX_DECLINED;

        അവസ്ഥ ngx_json_array:
            എങ്കിൽ (ngx_json_ws(*p)) {
                നിർത്തുക;
            }

            എങ്കിൽ (*p == ']') {
                rc = ngx_json_close(ctx, NGX_JSON_ARRAY_CLOSE, &state, p);
                എങ്കിൽ (rc != NGX_OK) {
                    തിരികെ rc;
                }

                നിർത്തുക;
            }

            /* fall through */

        അവസ്ഥ ngx_json_മൂല്യം:
            എങ്കിൽ (ngx_json_ws(*p)) {
                നിർത്തുക;
            }

            start = p;

            എങ്കിൽ (*p == '"') {
                ctx->in_താക്കോൽ = 0;
                ctx->escaped = 0;
                start = p + 1;
                state = ngx_json_string;
                നിർത്തുക;
            }

            എങ്കിൽ (*p == '{') {
                rc = ngx_json_emit(ctx, NGX_JSON_OBJECT_OPEN, p, 1);
                എങ്കിൽ (rc != NGX_OK) {
                    തിരികെ rc;
                }

                rc = ngx_json_തള്ളുക_state(ctx, ngx_json_ctx_object);
                എങ്കിൽ (rc != NGX_OK) {
                    തിരികെ rc;
                }

                state = ngx_json_object;
                നിർത്തുക;
            }

            എങ്കിൽ (*p == '[') {
                rc = ngx_json_emit(ctx, NGX_JSON_ARRAY_OPEN, p, 1);
                എങ്കിൽ (rc != NGX_OK) {
                    തിരികെ rc;
                }

                rc = ngx_json_തള്ളുക_state(ctx, ngx_json_ctx_array);
                എങ്കിൽ (rc != NGX_OK) {
                    തിരികെ rc;
                }

                state = ngx_json_array;
                നിർത്തുക;
            }

            എങ്കിൽ (*p == '-') {
                state = ngx_json_സംഖ്യ_minus;
                നിർത്തുക;
            }

            എങ്കിൽ (*p == '0') {
                state = ngx_json_സംഖ്യ_int_zero;
                നിർത്തുക;
            }

            എങ്കിൽ (*p >= '1' && *p <= '9') {
                state = ngx_json_സംഖ്യ_int;
                നിർത്തുക;
            }

            എങ്കിൽ (*p == 't') {
                state = ngx_json_true_t;
                നിർത്തുക;
            }

            എങ്കിൽ (*p == 'f') {
                state = ngx_json_false_f;
                നിർത്തുക;
            }

            എങ്കിൽ (*p == 'n') {
                state = ngx_json_null_n;
                നിർത്തുക;
            }

            തിരികെ NGX_DECLINED;

        അവസ്ഥ ngx_json_string:
            എങ്കിൽ (*p == '"') {

                എങ്കിൽ (ctx->in_താക്കോൽ) {
                    event = ctx->escaped ? NGX_JSON_താക്കോൽ_ESCAPED
                                         : NGX_JSON_താക്കോൽ;

                    rc = ngx_json_emit(ctx, event, start, p - start);
                    എങ്കിൽ (rc != NGX_OK) {
                        തിരികെ rc;
                    }

                    ctx->in_താക്കോൽ = 0;
                    state = ngx_json_object_name_separator;

                } അല്ലെങ്കിൽ {
                    event = ctx->escaped ? NGX_JSON_മൂല്യം_STRING_ESCAPED
                                         : NGX_JSON_മൂല്യം_STRING;

                    rc = ngx_json_emit(ctx, event, start, p - start);
                    എങ്കിൽ (rc != NGX_OK) {
                        തിരികെ rc;
                    }

                    state = ngx_json_after_മൂല്യം(ctx);
                }

                നിർത്തുക;
            }

            എങ്കിൽ (*p == '\\') {
                ctx->escaped = 1;
                state = ngx_json_escaped;
                നിർത്തുക;
            }

            എങ്കിൽ (*p < ' ') {
                തിരികെ NGX_DECLINED;
            }

            നിർത്തുക;

        അവസ്ഥ ngx_json_escaped:
            എങ്കിൽ (*p == '"' || *p == '\\' || *p == '/' || *p == 'b'
                || *p == 'f' || *p == 'n' || *p == 'r' || *p == 't')
            {
                state = ngx_json_string;
                നിർത്തുക;
            }

            എങ്കിൽ (*p == 'u') {
                ctx->codepoint = 0;
                ctx->hex_ഇടത് = 4;
                state = ngx_json_escaped_hex;
                നിർത്തുക;
            }

            തിരികെ NGX_DECLINED;

        അവസ്ഥ ngx_json_escaped_hex:

            /* accumulate the four hex digits of the first \uXXXX escape */

            hd = ngx_json_hex_digit(*p);
            എങ്കിൽ (hd == NGX_ERROR) {
                തിരികെ NGX_DECLINED;
            }

            ctx->codepoint = (ctx->codepoint << 4) | (uint32_t) hd;

            എങ്കിൽ (--ctx->hex_ഇടത് > 0) {
                നിർത്തുക;
            }

            എങ്കിൽ (ctx->codepoint >= 0xD800 && ctx->codepoint <= 0xDBFF) {
                /* high surrogate - must be followed by \uDC00..\uDFFF */
                state = ngx_json_surrogate_start;
                നിർത്തുക;
            }

            എങ്കിൽ (ctx->codepoint >= 0xDC00 && ctx->codepoint <= 0xDFFF) {
                /* lone low surrogate */
                തിരികെ NGX_DECLINED;
            }

            state = ngx_json_string;
            നിർത്തുക;

        അവസ്ഥ ngx_json_surrogate_start:
            എങ്കിൽ (*p == '\\') {
                state = ngx_json_surrogate_u;
                നിർത്തുക;
            }

            /* high surrogate not followed by \uXXXX */
            തിരികെ NGX_DECLINED;

        അവസ്ഥ ngx_json_surrogate_u:
            എങ്കിൽ (*p == 'u') {
                ctx->codepoint = 0;
                ctx->hex_ഇടത് = 4;
                state = ngx_json_surrogate_hex;
                നിർത്തുക;
            }

            /* high surrogate followed by non-\u escape */
            തിരികെ NGX_DECLINED;

        അവസ്ഥ ngx_json_surrogate_hex:

            /* accumulate the four hex digits of the low \uXXXX escape */

            hd = ngx_json_hex_digit(*p);
            എങ്കിൽ (hd == NGX_ERROR) {
                തിരികെ NGX_DECLINED;
            }

            ctx->codepoint = (ctx->codepoint << 4) | (uint32_t) hd;

            എങ്കിൽ (--ctx->hex_ഇടത് > 0) {
                നിർത്തുക;
            }

            എങ്കിൽ (ctx->codepoint >= 0xDC00 && ctx->codepoint <= 0xDFFF) {
                /* valid surrogate pair */
                state = ngx_json_string;
                നിർത്തുക;
            }

            /* high surrogate not followed by low surrogate */
            തിരികെ NGX_DECLINED;

        അവസ്ഥ ngx_json_സംഖ്യ_minus:
            എങ്കിൽ (*p >= '1' && *p <= '9') {
                state = ngx_json_സംഖ്യ_int;
                നിർത്തുക;
            }

            എങ്കിൽ (*p == '0') {
                state = ngx_json_സംഖ്യ_int_zero;
                നിർത്തുക;
            }

            തിരികെ NGX_DECLINED;

        അവസ്ഥ ngx_json_സംഖ്യ_int_zero:
            എങ്കിൽ (*p == '.') {
                state = ngx_json_സംഖ്യ_frac;
                നിർത്തുക;
            }

            എങ്കിൽ (*p == 'e' || *p == 'E') {
                /* No sense, but permitted by RFC */
                state = ngx_json_സംഖ്യ_exp;
                നിർത്തുക;
            }

            p--;

            rc = ngx_json_end_സംഖ്യ(ctx, start, p + 1, &state);
            എങ്കിൽ (rc != NGX_OK) {
                തിരികെ rc;
            }

            നിർത്തുക;

        അവസ്ഥ ngx_json_സംഖ്യ_int:
            എങ്കിൽ (*p >= '0' && *p <= '9') {
                നിർത്തുക;
            }

            എങ്കിൽ (*p == '.') {
                state = ngx_json_സംഖ്യ_frac;
                നിർത്തുക;
            }

            എങ്കിൽ (*p == 'e' || *p == 'E') {
                /* No sense, but permitted by RFC */
                state = ngx_json_സംഖ്യ_exp;
                നിർത്തുക;
            }

            p--;

            rc = ngx_json_end_സംഖ്യ(ctx, start, p + 1, &state);
            എങ്കിൽ (rc!= NGX_OK) {
                തിരികെ rc;
            }

            നിർത്തുക;

        അവസ്ഥ ngx_json_സംഖ്യ_frac:
            എങ്കിൽ (*p >= '0' && *p <= '9') {
                state = ngx_json_സംഖ്യ_frac_digit;
                നിർത്തുക;
            }

            തിരികെ NGX_DECLINED;

        അവസ്ഥ ngx_json_സംഖ്യ_frac_digit:
            എങ്കിൽ (*p >= '0' && *p <= '9') {
                നിർത്തുക;
            }

            എങ്കിൽ (*p == 'e' || *p == 'E') {
                state = ngx_json_സംഖ്യ_exp;
                നിർത്തുക;
            }

            p--;

            rc = ngx_json_end_സംഖ്യ(ctx, start, p + 1, &state);
            എങ്കിൽ (rc != NGX_OK) {
                തിരികെ rc;
            }

            നിർത്തുക;

        അവസ്ഥ ngx_json_സംഖ്യ_exp:
            എങ്കിൽ (*p == '-' || *p == '+') {
                state = ngx_json_സംഖ്യ_exp_plusminus;
                നിർത്തുക;
            }

            എങ്കിൽ (*p >= '0' && *p <= '9') {
                state = ngx_json_സംഖ്യ_exp_digit;
                നിർത്തുക;
            }

            തിരികെ NGX_DECLINED;

        അവസ്ഥ ngx_json_സംഖ്യ_exp_plusminus:
            എങ്കിൽ (*p >= '0' && *p <= '9') {
                state = ngx_json_സംഖ്യ_exp_digit;
                നിർത്തുക;
            }

            തിരികെ NGX_DECLINED;

        അവസ്ഥ ngx_json_സംഖ്യ_exp_digit:
            എങ്കിൽ (*p >= '0' && *p <= '9') {
                നിർത്തുക;
            }

            p--;

            rc = ngx_json_end_സംഖ്യ(ctx, start, p + 1, &state);
            എങ്കിൽ (rc != NGX_OK) {
                തിരികെ rc;
            }

            നിർത്തുക;

        അവസ്ഥ ngx_json_true_t:
            എങ്കിൽ (*p == 'r') {
                state = ngx_json_true_tr;
                നിർത്തുക;
            }

            തിരികെ NGX_DECLINED;

        അവസ്ഥ ngx_json_true_tr:
            എങ്കിൽ (*p == 'u') {
                state = ngx_json_true_tru;
                നിർത്തുക;
            }

            തിരികെ NGX_DECLINED;

        അവസ്ഥ ngx_json_true_tru:
            എങ്കിൽ (*p == 'e') {
                rc = ngx_json_emit(ctx, NGX_JSON_മൂല്യം_BOOL, start,
                                  p + 1 - start);
                എങ്കിൽ (rc != NGX_OK) {
                    തിരികെ rc;
                }

                state = ngx_json_after_മൂല്യം(ctx);
                നിർത്തുക;
            }

            തിരികെ NGX_DECLINED;

        അവസ്ഥ ngx_json_false_f:
            എങ്കിൽ (*p == 'a') {
                state = ngx_json_false_fa;
                നിർത്തുക;
            }

            തിരികെ NGX_DECLINED;

        അവസ്ഥ ngx_json_false_fa:
            എങ്കിൽ (*p == 'l') {
                state = ngx_json_false_fal;
                നിർത്തുക;
            }

            തിരികെ NGX_DECLINED;

        അവസ്ഥ ngx_json_false_fal:
            എങ്കിൽ (*p == 's') {
                state = ngx_json_false_fals;
                നിർത്തുക;
            }

            തിരികെ NGX_DECLINED;

        അവസ്ഥ ngx_json_false_fals:
            എങ്കിൽ (*p == 'e') {
                rc = ngx_json_emit(ctx, NGX_JSON_മൂല്യം_BOOL, start,
                                  p + 1 - start);
                എങ്കിൽ (rc != NGX_OK) {
                    തിരികെ rc;
                }

                state = ngx_json_after_മൂല്യം(ctx);
                നിർത്തുക;
            }

            തിരികെ NGX_DECLINED;

        അവസ്ഥ ngx_json_null_n:
            എങ്കിൽ (*p == 'u') {
                state = ngx_json_null_nu;
                നിർത്തുക;
            }

            തിരികെ NGX_DECLINED;

        അവസ്ഥ ngx_json_null_nu:
            എങ്കിൽ (*p == 'l') {
                state = ngx_json_null_nul;
                നിർത്തുക;
            }

            തിരികെ NGX_DECLINED;

        അവസ്ഥ ngx_json_null_nul:
            എങ്കിൽ (*p == 'l') {
                rc = ngx_json_emit(ctx, NGX_JSON_മൂല്യം_NULL, start,
                                  p + 1 - start);
                എങ്കിൽ (rc != NGX_OK) {
                    തിരികെ rc;
                }

                state = ngx_json_after_മൂല്യം(ctx);
                നിർത്തുക;
            }

            തിരികെ NGX_DECLINED;

        അവസ്ഥ ngx_json_done:
            എങ്കിൽ (ngx_json_ws(*p)) {
                നിർത്തുക;
            }

            തിരികെ NGX_DECLINED;
        }

        skip_depth = ctx->skip_until_depth;
    }

    /* a bare top-level number is not self-delimited; emit it at end of input */

    എങ്കിൽ (ctx->depth == 0
        && (state == ngx_json_സംഖ്യ_int_zero
            || state == ngx_json_സംഖ്യ_int
            || state == ngx_json_സംഖ്യ_frac_digit
            || state == ngx_json_സംഖ്യ_exp_digit))
    {
        rc = ngx_json_emit(ctx, NGX_JSON_മൂല്യം_സംഖ്യ, start, last - start);
        എങ്കിൽ (rc != NGX_OK) {
            തിരികെ rc;
        }

        state = ngx_json_done;
    }

    ctx->state = state;

    എങ്കിൽ (state == ngx_json_done) {
        തിരികെ NGX_OK;
    }

    തിരികെ NGX_DECLINED;
}


ngx_int_t
ngx_json_parse(ngx_pool_t *pool, ngx_str_t *json, ngx_json_handler_pt handler,
    ശൂന്യം *data)
{
    ngx_json_ctx_t  jctx;

    ngx_json_ctx_init(&jctx, pool);

    jctx.data = data;
    jctx.handler = handler;

    തിരികെ ngx_json_parse_ctx(&jctx, json->data, json->len);
}


സ്ഥിതം ngx_inline ngx_int_t
ngx_json_end_സംഖ്യ(ngx_json_ctx_t *ctx, u_char *start, u_char *end,
    ngx_json_state_e *state)
{
    ngx_int_t  rc;

    rc = ngx_json_emit(ctx, NGX_JSON_മൂല്യം_സംഖ്യ, start, end - start);
    എങ്കിൽ (rc != NGX_OK) {
        തിരികെ rc;
    }

    *state = ngx_json_after_മൂല്യം(ctx);

    തിരികെ NGX_OK;
}


സ്ഥിതം ngx_inline ngx_int_t
ngx_json_emit(ngx_json_ctx_t *ctx, ngx_json_event_e event, u_char *data,
    size_t len)
{
    ngx_int_t  rc;
    ngx_str_t  token;

    എങ്കിൽ (ngx_json_skipping(ctx->skip_until_depth)) {
        തിരികെ NGX_OK;
    }

    എങ്കിൽ (ctx->handler == ശൂന്യ) {
        തിരികെ NGX_OK;
    }

    token.data = data;
    token.len = len;
    rc = ctx->handler(ctx, event, &token);

    എങ്കിൽ (rc == NGX_JSON_SKIP) {

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

        തിരഞ്ഞെടുപ്പ് (event) {

        അവസ്ഥ NGX_JSON_താക്കോൽ:
        അവസ്ഥ NGX_JSON_താക്കോൽ_ESCAPED:
        അവസ്ഥ NGX_JSON_OBJECT_OPEN:
        അവസ്ഥ NGX_JSON_ARRAY_OPEN:
            ctx->skip_until_depth = ctx->depth;
            നിർത്തുക;

        അവസ്ഥ NGX_JSON_മൂല്യം_STRING:
        അവസ്ഥ NGX_JSON_മൂല്യം_STRING_ESCAPED:
        അവസ്ഥ NGX_JSON_മൂല്യം_സംഖ്യ:
        അവസ്ഥ NGX_JSON_മൂല്യം_BOOL:
        അവസ്ഥ NGX_JSON_മൂല്യം_NULL:
            എങ്കിൽ (ctx->depth > 0
                && ctx->അട്ടി[ctx->depth - 1] == ngx_json_ctx_array
            ) {
                ctx->skip_until_depth = ctx->depth - 1;
            }
            നിർത്തുക;

        സ്ഥിരസ്ഥിതി:
            നിർത്തുക;
        }

        തിരികെ NGX_OK;
    }

    തിരികെ rc;
}


സ്ഥിതം ngx_inline ngx_json_state_e
ngx_json_after_മൂല്യം(ngx_json_ctx_t *ctx)
{
    എങ്കിൽ (ctx->depth == 0) {
        തിരികെ ngx_json_done;
    }

    എങ്കിൽ (ctx->അട്ടി[ctx->depth - 1] == ngx_json_ctx_object) {
        തിരികെ ngx_json_object_മൂല്യം_separator;
    }

    തിരികെ ngx_json_array_മൂല്യം_separator;
}


സ്ഥിതം ngx_inline ngx_int_t
ngx_json_close(ngx_json_ctx_t *ctx, ngx_json_event_e event,
    ngx_json_state_e *state, u_char *p)
{
    ngx_int_t  rc;

    ctx->depth--;

    rc = ngx_json_emit(ctx, event, p, 1);
    എങ്കിൽ (rc != NGX_OK) {
        തിരികെ rc;
    }

    *state = ngx_json_after_മൂല്യം(ctx);

    തിരികെ NGX_OK;
}


സ്ഥിതം ngx_int_t
ngx_json_തള്ളുക_state(ngx_json_ctx_t *ctx, ngx_json_container_e container)
{
    ngx_json_container_e  *അട്ടി;

    എങ്കിൽ (ctx->depth >= ctx->max_depth) {
        തിരികെ NGX_DECLINED;
    }

    എങ്കിൽ (ctx->അട്ടി == ശൂന്യ) {
        അട്ടി = ngx_palloc(ctx->pool,
                           ctx->max_depth * വലുപ്പം(ngx_json_container_e));
        എങ്കിൽ (അട്ടി == ശൂന്യ) {
            തിരികെ NGX_ERROR;
        }

        ctx->അട്ടി = അട്ടി;
    }

    ctx->അട്ടി[ctx->depth] = container;
    ctx->depth++;

    തിരികെ NGX_OK;
}
