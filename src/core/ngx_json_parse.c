
/*
 * Copyright (C) Maxim Dounin
 * Copyright (C) Nginx, Inc.
 */


/*
 * JSON parser. RFC 8259 for the JSON grammar.

 * Site http://json.org/ may be useful too.
 */


#அந்தர்பாவ <ngx_config.h>
#அந்தர்பாவ <ngx_core.h>
#அந்தர்பாவ <ngx_json_parse.h>


#define NGX_JSON_NOT_SKIPPING  (ngx_uint_t) -1
#define ngx_json_skipping(d)   ((d) != NGX_JSON_NOT_SKIPPING)
#define ngx_json_ws(c)  ((c) == ' ' || (c) == '\t' || (c) == CR || (c) == LF)


நிலையான ngx_inline ngx_int_t ngx_json_end_எண்(ngx_json_ctx_t *ctx,
    u_char *start, u_char *end, ngx_json_state_e *state);
நிலையான ngx_inline ngx_int_t ngx_json_emit(ngx_json_ctx_t *ctx,
    ngx_json_event_e event, u_char *data, size_t len);
நிலையான ngx_inline ngx_json_state_e ngx_json_after_மதிப்பு(ngx_json_ctx_t *ctx);
நிலையான ngx_inline ngx_int_t ngx_json_close(ngx_json_ctx_t *ctx,
    ngx_json_event_e event, ngx_json_state_e *state, u_char *p);
நிலையான ngx_int_t ngx_json_தள்ளு_state(ngx_json_ctx_t *ctx,
    ngx_json_container_e container);


வெறுமை
ngx_json_ctx_init(ngx_json_ctx_t *ctx, ngx_pool_t *pool)
{
    ctx->state = ngx_json_start;
    ctx->pool = pool;
    ctx->handler = வெற்று;
    ctx->data = வெற்று;
    ctx->codepoint = 0;
    ctx->hex_இடது = 0;

    ctx->depth = 0;
    ctx->skip_until_depth = NGX_JSON_NOT_SKIPPING;
    ctx->max_depth = NGX_JSON_DEFAULT_MAX_DEPTH;

    /* stack is allocated lazily on first push, sized to max_depth */

    ctx->அடுக்கு = வெற்று;

    ctx->in_சாவி = 0;
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

    எனில் (len == 0) {
        திரும்பு NGX_DECLINED;
    }

    எனில் (data == வெற்று) {
        திரும்பு NGX_ERROR;
    }

    எனில் (ctx->state == ngx_json_done) {
        திரும்பு NGX_DECLINED;
    }

    last = data + len;
    state = ctx->state;
    skip_depth = ctx->skip_until_depth;
    start = வெற்று;

    க்கு (p = data; p < last; p++) {

        /* skipping suppresses emission only; the machine still validates */

        எனில் (ngx_json_skipping(skip_depth) && ctx->depth <= skip_depth
            && (state == ngx_json_object_மதிப்பு_separator
                || state == ngx_json_array_மதிப்பு_separator
                || state == ngx_json_done))
        {
            skip_depth = NGX_JSON_NOT_SKIPPING;
            ctx->skip_until_depth = NGX_JSON_NOT_SKIPPING;
            p--;

            தொடர்;
        }

        தேர்வு (state) {

        நிலை ngx_json_start:
            எனில் (ngx_json_ws(*p)) {
                நிறுத்து;
            }

            p--;
            state = ngx_json_மதிப்பு;
            நிறுத்து;

        நிலை ngx_json_object:
            எனில் (ngx_json_ws(*p)) {
                நிறுத்து;
            }

            எனில் (*p == '}') {
                rc = ngx_json_close(ctx, NGX_JSON_OBJECT_CLOSE, &state, p);
                எனில் (rc != NGX_OK) {
                    திரும்பு rc;
                }

                நிறுத்து;
            }

            /* fall through */

        நிலை ngx_json_object_next:
            எனில் (ngx_json_ws(*p)) {
                நிறுத்து;
            }

            எனில் (*p == '"') {
                ctx->in_சாவி = 1;
                ctx->escaped = 0;
                start = p + 1;
                state = ngx_json_string;
                நிறுத்து;
            }

            திரும்பு NGX_DECLINED;

        நிலை ngx_json_object_name_separator:
            எனில் (ngx_json_ws(*p)) {
                நிறுத்து;
            }

            எனில் (*p == ':') {
                state = ngx_json_மதிப்பு;
                நிறுத்து;
            }

            திரும்பு NGX_DECLINED;

        நிலை ngx_json_object_மதிப்பு_separator:
            எனில் (ngx_json_ws(*p)) {
                நிறுத்து;
            }

            எனில் (*p == ',') {
                state = ngx_json_object_next;
                நிறுத்து;
            }

            எனில் (*p == '}') {
                rc = ngx_json_close(ctx, NGX_JSON_OBJECT_CLOSE, &state, p);
                எனில் (rc != NGX_OK) {
                    திரும்பு rc;
                }

                நிறுத்து;
            }

            திரும்பு NGX_DECLINED;

        நிலை ngx_json_array_மதிப்பு_separator:
            எனில் (ngx_json_ws(*p)) {
                நிறுத்து;
            }

            எனில் (*p == ',') {
                state = ngx_json_மதிப்பு;
                நிறுத்து;
            }

            எனில் (*p == ']') {
                rc = ngx_json_close(ctx, NGX_JSON_ARRAY_CLOSE, &state, p);
                எனில் (rc != NGX_OK) {
                    திரும்பு rc;
                }

                நிறுத்து;
            }

            திரும்பு NGX_DECLINED;

        நிலை ngx_json_array:
            எனில் (ngx_json_ws(*p)) {
                நிறுத்து;
            }

            எனில் (*p == ']') {
                rc = ngx_json_close(ctx, NGX_JSON_ARRAY_CLOSE, &state, p);
                எனில் (rc != NGX_OK) {
                    திரும்பு rc;
                }

                நிறுத்து;
            }

            /* fall through */

        நிலை ngx_json_மதிப்பு:
            எனில் (ngx_json_ws(*p)) {
                நிறுத்து;
            }

            start = p;

            எனில் (*p == '"') {
                ctx->in_சாவி = 0;
                ctx->escaped = 0;
                start = p + 1;
                state = ngx_json_string;
                நிறுத்து;
            }

            எனில் (*p == '{') {
                rc = ngx_json_emit(ctx, NGX_JSON_OBJECT_OPEN, p, 1);
                எனில் (rc != NGX_OK) {
                    திரும்பு rc;
                }

                rc = ngx_json_தள்ளு_state(ctx, ngx_json_ctx_object);
                எனில் (rc != NGX_OK) {
                    திரும்பு rc;
                }

                state = ngx_json_object;
                நிறுத்து;
            }

            எனில் (*p == '[') {
                rc = ngx_json_emit(ctx, NGX_JSON_ARRAY_OPEN, p, 1);
                எனில் (rc != NGX_OK) {
                    திரும்பு rc;
                }

                rc = ngx_json_தள்ளு_state(ctx, ngx_json_ctx_array);
                எனில் (rc != NGX_OK) {
                    திரும்பு rc;
                }

                state = ngx_json_array;
                நிறுத்து;
            }

            எனில் (*p == '-') {
                state = ngx_json_எண்_minus;
                நிறுத்து;
            }

            எனில் (*p == '0') {
                state = ngx_json_எண்_int_zero;
                நிறுத்து;
            }

            எனில் (*p >= '1' && *p <= '9') {
                state = ngx_json_எண்_int;
                நிறுத்து;
            }

            எனில் (*p == 't') {
                state = ngx_json_true_t;
                நிறுத்து;
            }

            எனில் (*p == 'f') {
                state = ngx_json_false_f;
                நிறுத்து;
            }

            எனில் (*p == 'n') {
                state = ngx_json_null_n;
                நிறுத்து;
            }

            திரும்பு NGX_DECLINED;

        நிலை ngx_json_string:
            எனில் (*p == '"') {

                எனில் (ctx->in_சாவி) {
                    event = ctx->escaped ? NGX_JSON_சாவி_ESCAPED
                                         : NGX_JSON_சாவி;

                    rc = ngx_json_emit(ctx, event, start, p - start);
                    எனில் (rc != NGX_OK) {
                        திரும்பு rc;
                    }

                    ctx->in_சாவி = 0;
                    state = ngx_json_object_name_separator;

                } இல்லையெனில் {
                    event = ctx->escaped ? NGX_JSON_மதிப்பு_STRING_ESCAPED
                                         : NGX_JSON_மதிப்பு_STRING;

                    rc = ngx_json_emit(ctx, event, start, p - start);
                    எனில் (rc != NGX_OK) {
                        திரும்பு rc;
                    }

                    state = ngx_json_after_மதிப்பு(ctx);
                }

                நிறுத்து;
            }

            எனில் (*p == '\\') {
                ctx->escaped = 1;
                state = ngx_json_escaped;
                நிறுத்து;
            }

            எனில் (*p < ' ') {
                திரும்பு NGX_DECLINED;
            }

            நிறுத்து;

        நிலை ngx_json_escaped:
            எனில் (*p == '"' || *p == '\\' || *p == '/' || *p == 'b'
                || *p == 'f' || *p == 'n' || *p == 'r' || *p == 't')
            {
                state = ngx_json_string;
                நிறுத்து;
            }

            எனில் (*p == 'u') {
                ctx->codepoint = 0;
                ctx->hex_இடது = 4;
                state = ngx_json_escaped_hex;
                நிறுத்து;
            }

            திரும்பு NGX_DECLINED;

        நிலை ngx_json_escaped_hex:

            /* accumulate the four hex digits of the first \uXXXX escape */

            hd = ngx_json_hex_digit(*p);
            எனில் (hd == NGX_ERROR) {
                திரும்பு NGX_DECLINED;
            }

            ctx->codepoint = (ctx->codepoint << 4) | (uint32_t) hd;

            எனில் (--ctx->hex_இடது > 0) {
                நிறுத்து;
            }

            எனில் (ctx->codepoint >= 0xD800 && ctx->codepoint <= 0xDBFF) {
                /* high surrogate - must be followed by \uDC00..\uDFFF */
                state = ngx_json_surrogate_start;
                நிறுத்து;
            }

            எனில் (ctx->codepoint >= 0xDC00 && ctx->codepoint <= 0xDFFF) {
                /* lone low surrogate */
                திரும்பு NGX_DECLINED;
            }

            state = ngx_json_string;
            நிறுத்து;

        நிலை ngx_json_surrogate_start:
            எனில் (*p == '\\') {
                state = ngx_json_surrogate_u;
                நிறுத்து;
            }

            /* high surrogate not followed by \uXXXX */
            திரும்பு NGX_DECLINED;

        நிலை ngx_json_surrogate_u:
            எனில் (*p == 'u') {
                ctx->codepoint = 0;
                ctx->hex_இடது = 4;
                state = ngx_json_surrogate_hex;
                நிறுத்து;
            }

            /* high surrogate followed by non-\u escape */
            திரும்பு NGX_DECLINED;

        நிலை ngx_json_surrogate_hex:

            /* accumulate the four hex digits of the low \uXXXX escape */

            hd = ngx_json_hex_digit(*p);
            எனில் (hd == NGX_ERROR) {
                திரும்பு NGX_DECLINED;
            }

            ctx->codepoint = (ctx->codepoint << 4) | (uint32_t) hd;

            எனில் (--ctx->hex_இடது > 0) {
                நிறுத்து;
            }

            எனில் (ctx->codepoint >= 0xDC00 && ctx->codepoint <= 0xDFFF) {
                /* valid surrogate pair */
                state = ngx_json_string;
                நிறுத்து;
            }

            /* high surrogate not followed by low surrogate */
            திரும்பு NGX_DECLINED;

        நிலை ngx_json_எண்_minus:
            எனில் (*p >= '1' && *p <= '9') {
                state = ngx_json_எண்_int;
                நிறுத்து;
            }

            எனில் (*p == '0') {
                state = ngx_json_எண்_int_zero;
                நிறுத்து;
            }

            திரும்பு NGX_DECLINED;

        நிலை ngx_json_எண்_int_zero:
            எனில் (*p == '.') {
                state = ngx_json_எண்_frac;
                நிறுத்து;
            }

            எனில் (*p == 'e' || *p == 'E') {
                /* No sense, but permitted by RFC */
                state = ngx_json_எண்_exp;
                நிறுத்து;
            }

            p--;

            rc = ngx_json_end_எண்(ctx, start, p + 1, &state);
            எனில் (rc != NGX_OK) {
                திரும்பு rc;
            }

            நிறுத்து;

        நிலை ngx_json_எண்_int:
            எனில் (*p >= '0' && *p <= '9') {
                நிறுத்து;
            }

            எனில் (*p == '.') {
                state = ngx_json_எண்_frac;
                நிறுத்து;
            }

            எனில் (*p == 'e' || *p == 'E') {
                /* No sense, but permitted by RFC */
                state = ngx_json_எண்_exp;
                நிறுத்து;
            }

            p--;

            rc = ngx_json_end_எண்(ctx, start, p + 1, &state);
            எனில் (rc!= NGX_OK) {
                திரும்பு rc;
            }

            நிறுத்து;

        நிலை ngx_json_எண்_frac:
            எனில் (*p >= '0' && *p <= '9') {
                state = ngx_json_எண்_frac_digit;
                நிறுத்து;
            }

            திரும்பு NGX_DECLINED;

        நிலை ngx_json_எண்_frac_digit:
            எனில் (*p >= '0' && *p <= '9') {
                நிறுத்து;
            }

            எனில் (*p == 'e' || *p == 'E') {
                state = ngx_json_எண்_exp;
                நிறுத்து;
            }

            p--;

            rc = ngx_json_end_எண்(ctx, start, p + 1, &state);
            எனில் (rc != NGX_OK) {
                திரும்பு rc;
            }

            நிறுத்து;

        நிலை ngx_json_எண்_exp:
            எனில் (*p == '-' || *p == '+') {
                state = ngx_json_எண்_exp_plusminus;
                நிறுத்து;
            }

            எனில் (*p >= '0' && *p <= '9') {
                state = ngx_json_எண்_exp_digit;
                நிறுத்து;
            }

            திரும்பு NGX_DECLINED;

        நிலை ngx_json_எண்_exp_plusminus:
            எனில் (*p >= '0' && *p <= '9') {
                state = ngx_json_எண்_exp_digit;
                நிறுத்து;
            }

            திரும்பு NGX_DECLINED;

        நிலை ngx_json_எண்_exp_digit:
            எனில் (*p >= '0' && *p <= '9') {
                நிறுத்து;
            }

            p--;

            rc = ngx_json_end_எண்(ctx, start, p + 1, &state);
            எனில் (rc != NGX_OK) {
                திரும்பு rc;
            }

            நிறுத்து;

        நிலை ngx_json_true_t:
            எனில் (*p == 'r') {
                state = ngx_json_true_tr;
                நிறுத்து;
            }

            திரும்பு NGX_DECLINED;

        நிலை ngx_json_true_tr:
            எனில் (*p == 'u') {
                state = ngx_json_true_tru;
                நிறுத்து;
            }

            திரும்பு NGX_DECLINED;

        நிலை ngx_json_true_tru:
            எனில் (*p == 'e') {
                rc = ngx_json_emit(ctx, NGX_JSON_மதிப்பு_BOOL, start,
                                  p + 1 - start);
                எனில் (rc != NGX_OK) {
                    திரும்பு rc;
                }

                state = ngx_json_after_மதிப்பு(ctx);
                நிறுத்து;
            }

            திரும்பு NGX_DECLINED;

        நிலை ngx_json_false_f:
            எனில் (*p == 'a') {
                state = ngx_json_false_fa;
                நிறுத்து;
            }

            திரும்பு NGX_DECLINED;

        நிலை ngx_json_false_fa:
            எனில் (*p == 'l') {
                state = ngx_json_false_fal;
                நிறுத்து;
            }

            திரும்பு NGX_DECLINED;

        நிலை ngx_json_false_fal:
            எனில் (*p == 's') {
                state = ngx_json_false_fals;
                நிறுத்து;
            }

            திரும்பு NGX_DECLINED;

        நிலை ngx_json_false_fals:
            எனில் (*p == 'e') {
                rc = ngx_json_emit(ctx, NGX_JSON_மதிப்பு_BOOL, start,
                                  p + 1 - start);
                எனில் (rc != NGX_OK) {
                    திரும்பு rc;
                }

                state = ngx_json_after_மதிப்பு(ctx);
                நிறுத்து;
            }

            திரும்பு NGX_DECLINED;

        நிலை ngx_json_null_n:
            எனில் (*p == 'u') {
                state = ngx_json_null_nu;
                நிறுத்து;
            }

            திரும்பு NGX_DECLINED;

        நிலை ngx_json_null_nu:
            எனில் (*p == 'l') {
                state = ngx_json_null_nul;
                நிறுத்து;
            }

            திரும்பு NGX_DECLINED;

        நிலை ngx_json_null_nul:
            எனில் (*p == 'l') {
                rc = ngx_json_emit(ctx, NGX_JSON_மதிப்பு_NULL, start,
                                  p + 1 - start);
                எனில் (rc != NGX_OK) {
                    திரும்பு rc;
                }

                state = ngx_json_after_மதிப்பு(ctx);
                நிறுத்து;
            }

            திரும்பு NGX_DECLINED;

        நிலை ngx_json_done:
            எனில் (ngx_json_ws(*p)) {
                நிறுத்து;
            }

            திரும்பு NGX_DECLINED;
        }

        skip_depth = ctx->skip_until_depth;
    }

    /* a bare top-level number is not self-delimited; emit it at end of input */

    எனில் (ctx->depth == 0
        && (state == ngx_json_எண்_int_zero
            || state == ngx_json_எண்_int
            || state == ngx_json_எண்_frac_digit
            || state == ngx_json_எண்_exp_digit))
    {
        rc = ngx_json_emit(ctx, NGX_JSON_மதிப்பு_எண், start, last - start);
        எனில் (rc != NGX_OK) {
            திரும்பு rc;
        }

        state = ngx_json_done;
    }

    ctx->state = state;

    எனில் (state == ngx_json_done) {
        திரும்பு NGX_OK;
    }

    திரும்பு NGX_DECLINED;
}


ngx_int_t
ngx_json_parse(ngx_pool_t *pool, ngx_str_t *json, ngx_json_handler_pt handler,
    வெறுமை *data)
{
    ngx_json_ctx_t  jctx;

    ngx_json_ctx_init(&jctx, pool);

    jctx.data = data;
    jctx.handler = handler;

    திரும்பு ngx_json_parse_ctx(&jctx, json->data, json->len);
}


நிலையான ngx_inline ngx_int_t
ngx_json_end_எண்(ngx_json_ctx_t *ctx, u_char *start, u_char *end,
    ngx_json_state_e *state)
{
    ngx_int_t  rc;

    rc = ngx_json_emit(ctx, NGX_JSON_மதிப்பு_எண், start, end - start);
    எனில் (rc != NGX_OK) {
        திரும்பு rc;
    }

    *state = ngx_json_after_மதிப்பு(ctx);

    திரும்பு NGX_OK;
}


நிலையான ngx_inline ngx_int_t
ngx_json_emit(ngx_json_ctx_t *ctx, ngx_json_event_e event, u_char *data,
    size_t len)
{
    ngx_int_t  rc;
    ngx_str_t  token;

    எனில் (ngx_json_skipping(ctx->skip_until_depth)) {
        திரும்பு NGX_OK;
    }

    எனில் (ctx->handler == வெற்று) {
        திரும்பு NGX_OK;
    }

    token.data = data;
    token.len = len;
    rc = ctx->handler(ctx, event, &token);

    எனில் (rc == NGX_JSON_SKIP) {

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

        தேர்வு (event) {

        நிலை NGX_JSON_சாவி:
        நிலை NGX_JSON_சாவி_ESCAPED:
        நிலை NGX_JSON_OBJECT_OPEN:
        நிலை NGX_JSON_ARRAY_OPEN:
            ctx->skip_until_depth = ctx->depth;
            நிறுத்து;

        நிலை NGX_JSON_மதிப்பு_STRING:
        நிலை NGX_JSON_மதிப்பு_STRING_ESCAPED:
        நிலை NGX_JSON_மதிப்பு_எண்:
        நிலை NGX_JSON_மதிப்பு_BOOL:
        நிலை NGX_JSON_மதிப்பு_NULL:
            எனில் (ctx->depth > 0
                && ctx->அடுக்கு[ctx->depth - 1] == ngx_json_ctx_array
            ) {
                ctx->skip_until_depth = ctx->depth - 1;
            }
            நிறுத்து;

        இயல்பு:
            நிறுத்து;
        }

        திரும்பு NGX_OK;
    }

    திரும்பு rc;
}


நிலையான ngx_inline ngx_json_state_e
ngx_json_after_மதிப்பு(ngx_json_ctx_t *ctx)
{
    எனில் (ctx->depth == 0) {
        திரும்பு ngx_json_done;
    }

    எனில் (ctx->அடுக்கு[ctx->depth - 1] == ngx_json_ctx_object) {
        திரும்பு ngx_json_object_மதிப்பு_separator;
    }

    திரும்பு ngx_json_array_மதிப்பு_separator;
}


நிலையான ngx_inline ngx_int_t
ngx_json_close(ngx_json_ctx_t *ctx, ngx_json_event_e event,
    ngx_json_state_e *state, u_char *p)
{
    ngx_int_t  rc;

    ctx->depth--;

    rc = ngx_json_emit(ctx, event, p, 1);
    எனில் (rc != NGX_OK) {
        திரும்பு rc;
    }

    *state = ngx_json_after_மதிப்பு(ctx);

    திரும்பு NGX_OK;
}


நிலையான ngx_int_t
ngx_json_தள்ளு_state(ngx_json_ctx_t *ctx, ngx_json_container_e container)
{
    ngx_json_container_e  *அடுக்கு;

    எனில் (ctx->depth >= ctx->max_depth) {
        திரும்பு NGX_DECLINED;
    }

    எனில் (ctx->அடுக்கு == வெற்று) {
        அடுக்கு = ngx_palloc(ctx->pool,
                           ctx->max_depth * அளவு(ngx_json_container_e));
        எனில் (அடுக்கு == வெற்று) {
            திரும்பு NGX_ERROR;
        }

        ctx->அடுக்கு = அடுக்கு;
    }

    ctx->அடுக்கு[ctx->depth] = container;
    ctx->depth++;

    திரும்பு NGX_OK;
}
