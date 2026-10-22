
/*
 * Copyright (C) Maxim Dounin
 * Copyright (C) Nginx, Inc.
 */


/*
 * JSON parser. RFC 8259 for the JSON grammar.

 * Site http://json.org/ may be useful too.
 */


#અન્તર્ભાવ <ngx_config.h>
#અન્તર્ભાવ <ngx_core.h>
#અન્તર્ભાવ <ngx_json_parse.h>


#define NGX_JSON_NOT_SKIPPING  (ngx_uint_t) -1
#define ngx_json_skipping(d)   ((d) != NGX_JSON_NOT_SKIPPING)
#define ngx_json_ws(c)  ((c) == ' ' || (c) == '\t' || (c) == CR || (c) == LF)


સ્થિર ngx_inline ngx_int_t ngx_json_end_સંખ્યા(ngx_json_ctx_t *ctx,
    u_char *start, u_char *end, ngx_json_state_e *state);
સ્થિર ngx_inline ngx_int_t ngx_json_emit(ngx_json_ctx_t *ctx,
    ngx_json_event_e event, u_char *data, size_t len);
સ્થિર ngx_inline ngx_json_state_e ngx_json_after_કિંમત(ngx_json_ctx_t *ctx);
સ્થિર ngx_inline ngx_int_t ngx_json_close(ngx_json_ctx_t *ctx,
    ngx_json_event_e event, ngx_json_state_e *state, u_char *p);
સ્થિર ngx_int_t ngx_json_ધકેલો_state(ngx_json_ctx_t *ctx,
    ngx_json_container_e container);


શૂન્ય
ngx_json_ctx_init(ngx_json_ctx_t *ctx, ngx_pool_t *pool)
{
    ctx->state = ngx_json_start;
    ctx->pool = pool;
    ctx->handler = ખાલી;
    ctx->data = ખાલી;
    ctx->codepoint = 0;
    ctx->hex_ડાબું = 0;

    ctx->depth = 0;
    ctx->skip_until_depth = NGX_JSON_NOT_SKIPPING;
    ctx->max_depth = NGX_JSON_DEFAULT_MAX_DEPTH;

    /* stack is allocated lazily on first push, sized to max_depth */

    ctx->ઢગલો = ખાલી;

    ctx->in_ચાવી = 0;
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

    જો (len == 0) {
        પાછા NGX_DECLINED;
    }

    જો (data == ખાલી) {
        પાછા NGX_ERROR;
    }

    જો (ctx->state == ngx_json_done) {
        પાછા NGX_DECLINED;
    }

    last = data + len;
    state = ctx->state;
    skip_depth = ctx->skip_until_depth;
    start = ખાલી;

    માટે (p = data; p < last; p++) {

        /* skipping suppresses emission only; the machine still validates */

        જો (ngx_json_skipping(skip_depth) && ctx->depth <= skip_depth
            && (state == ngx_json_object_કિંમત_separator
                || state == ngx_json_array_કિંમત_separator
                || state == ngx_json_done))
        {
            skip_depth = NGX_JSON_NOT_SKIPPING;
            ctx->skip_until_depth = NGX_JSON_NOT_SKIPPING;
            p--;

            ચાલુ;
        }

        પસંદગી (state) {

        સ્થિતિ ngx_json_start:
            જો (ngx_json_ws(*p)) {
                તોડો;
            }

            p--;
            state = ngx_json_કિંમત;
            તોડો;

        સ્થિતિ ngx_json_object:
            જો (ngx_json_ws(*p)) {
                તોડો;
            }

            જો (*p == '}') {
                rc = ngx_json_close(ctx, NGX_JSON_OBJECT_CLOSE, &state, p);
                જો (rc != NGX_OK) {
                    પાછા rc;
                }

                તોડો;
            }

            /* fall through */

        સ્થિતિ ngx_json_object_next:
            જો (ngx_json_ws(*p)) {
                તોડો;
            }

            જો (*p == '"') {
                ctx->in_ચાવી = 1;
                ctx->escaped = 0;
                start = p + 1;
                state = ngx_json_string;
                તોડો;
            }

            પાછા NGX_DECLINED;

        સ્થિતિ ngx_json_object_name_separator:
            જો (ngx_json_ws(*p)) {
                તોડો;
            }

            જો (*p == ':') {
                state = ngx_json_કિંમત;
                તોડો;
            }

            પાછા NGX_DECLINED;

        સ્થિતિ ngx_json_object_કિંમત_separator:
            જો (ngx_json_ws(*p)) {
                તોડો;
            }

            જો (*p == ',') {
                state = ngx_json_object_next;
                તોડો;
            }

            જો (*p == '}') {
                rc = ngx_json_close(ctx, NGX_JSON_OBJECT_CLOSE, &state, p);
                જો (rc != NGX_OK) {
                    પાછા rc;
                }

                તોડો;
            }

            પાછા NGX_DECLINED;

        સ્થિતિ ngx_json_array_કિંમત_separator:
            જો (ngx_json_ws(*p)) {
                તોડો;
            }

            જો (*p == ',') {
                state = ngx_json_કિંમત;
                તોડો;
            }

            જો (*p == ']') {
                rc = ngx_json_close(ctx, NGX_JSON_ARRAY_CLOSE, &state, p);
                જો (rc != NGX_OK) {
                    પાછા rc;
                }

                તોડો;
            }

            પાછા NGX_DECLINED;

        સ્થિતિ ngx_json_array:
            જો (ngx_json_ws(*p)) {
                તોડો;
            }

            જો (*p == ']') {
                rc = ngx_json_close(ctx, NGX_JSON_ARRAY_CLOSE, &state, p);
                જો (rc != NGX_OK) {
                    પાછા rc;
                }

                તોડો;
            }

            /* fall through */

        સ્થિતિ ngx_json_કિંમત:
            જો (ngx_json_ws(*p)) {
                તોડો;
            }

            start = p;

            જો (*p == '"') {
                ctx->in_ચાવી = 0;
                ctx->escaped = 0;
                start = p + 1;
                state = ngx_json_string;
                તોડો;
            }

            જો (*p == '{') {
                rc = ngx_json_emit(ctx, NGX_JSON_OBJECT_OPEN, p, 1);
                જો (rc != NGX_OK) {
                    પાછા rc;
                }

                rc = ngx_json_ધકેલો_state(ctx, ngx_json_ctx_object);
                જો (rc != NGX_OK) {
                    પાછા rc;
                }

                state = ngx_json_object;
                તોડો;
            }

            જો (*p == '[') {
                rc = ngx_json_emit(ctx, NGX_JSON_ARRAY_OPEN, p, 1);
                જો (rc != NGX_OK) {
                    પાછા rc;
                }

                rc = ngx_json_ધકેલો_state(ctx, ngx_json_ctx_array);
                જો (rc != NGX_OK) {
                    પાછા rc;
                }

                state = ngx_json_array;
                તોડો;
            }

            જો (*p == '-') {
                state = ngx_json_સંખ્યા_minus;
                તોડો;
            }

            જો (*p == '0') {
                state = ngx_json_સંખ્યા_int_zero;
                તોડો;
            }

            જો (*p >= '1' && *p <= '9') {
                state = ngx_json_સંખ્યા_int;
                તોડો;
            }

            જો (*p == 't') {
                state = ngx_json_true_t;
                તોડો;
            }

            જો (*p == 'f') {
                state = ngx_json_false_f;
                તોડો;
            }

            જો (*p == 'n') {
                state = ngx_json_null_n;
                તોડો;
            }

            પાછા NGX_DECLINED;

        સ્થિતિ ngx_json_string:
            જો (*p == '"') {

                જો (ctx->in_ચાવી) {
                    event = ctx->escaped ? NGX_JSON_ચાવી_ESCAPED
                                         : NGX_JSON_ચાવી;

                    rc = ngx_json_emit(ctx, event, start, p - start);
                    જો (rc != NGX_OK) {
                        પાછા rc;
                    }

                    ctx->in_ચાવી = 0;
                    state = ngx_json_object_name_separator;

                } નહીંતર {
                    event = ctx->escaped ? NGX_JSON_કિંમત_STRING_ESCAPED
                                         : NGX_JSON_કિંમત_STRING;

                    rc = ngx_json_emit(ctx, event, start, p - start);
                    જો (rc != NGX_OK) {
                        પાછા rc;
                    }

                    state = ngx_json_after_કિંમત(ctx);
                }

                તોડો;
            }

            જો (*p == '\\') {
                ctx->escaped = 1;
                state = ngx_json_escaped;
                તોડો;
            }

            જો (*p < ' ') {
                પાછા NGX_DECLINED;
            }

            તોડો;

        સ્થિતિ ngx_json_escaped:
            જો (*p == '"' || *p == '\\' || *p == '/' || *p == 'b'
                || *p == 'f' || *p == 'n' || *p == 'r' || *p == 't')
            {
                state = ngx_json_string;
                તોડો;
            }

            જો (*p == 'u') {
                ctx->codepoint = 0;
                ctx->hex_ડાબું = 4;
                state = ngx_json_escaped_hex;
                તોડો;
            }

            પાછા NGX_DECLINED;

        સ્થિતિ ngx_json_escaped_hex:

            /* accumulate the four hex digits of the first \uXXXX escape */

            hd = ngx_json_hex_digit(*p);
            જો (hd == NGX_ERROR) {
                પાછા NGX_DECLINED;
            }

            ctx->codepoint = (ctx->codepoint << 4) | (uint32_t) hd;

            જો (--ctx->hex_ડાબું > 0) {
                તોડો;
            }

            જો (ctx->codepoint >= 0xD800 && ctx->codepoint <= 0xDBFF) {
                /* high surrogate - must be followed by \uDC00..\uDFFF */
                state = ngx_json_surrogate_start;
                તોડો;
            }

            જો (ctx->codepoint >= 0xDC00 && ctx->codepoint <= 0xDFFF) {
                /* lone low surrogate */
                પાછા NGX_DECLINED;
            }

            state = ngx_json_string;
            તોડો;

        સ્થિતિ ngx_json_surrogate_start:
            જો (*p == '\\') {
                state = ngx_json_surrogate_u;
                તોડો;
            }

            /* high surrogate not followed by \uXXXX */
            પાછા NGX_DECLINED;

        સ્થિતિ ngx_json_surrogate_u:
            જો (*p == 'u') {
                ctx->codepoint = 0;
                ctx->hex_ડાબું = 4;
                state = ngx_json_surrogate_hex;
                તોડો;
            }

            /* high surrogate followed by non-\u escape */
            પાછા NGX_DECLINED;

        સ્થિતિ ngx_json_surrogate_hex:

            /* accumulate the four hex digits of the low \uXXXX escape */

            hd = ngx_json_hex_digit(*p);
            જો (hd == NGX_ERROR) {
                પાછા NGX_DECLINED;
            }

            ctx->codepoint = (ctx->codepoint << 4) | (uint32_t) hd;

            જો (--ctx->hex_ડાબું > 0) {
                તોડો;
            }

            જો (ctx->codepoint >= 0xDC00 && ctx->codepoint <= 0xDFFF) {
                /* valid surrogate pair */
                state = ngx_json_string;
                તોડો;
            }

            /* high surrogate not followed by low surrogate */
            પાછા NGX_DECLINED;

        સ્થિતિ ngx_json_સંખ્યા_minus:
            જો (*p >= '1' && *p <= '9') {
                state = ngx_json_સંખ્યા_int;
                તોડો;
            }

            જો (*p == '0') {
                state = ngx_json_સંખ્યા_int_zero;
                તોડો;
            }

            પાછા NGX_DECLINED;

        સ્થિતિ ngx_json_સંખ્યા_int_zero:
            જો (*p == '.') {
                state = ngx_json_સંખ્યા_frac;
                તોડો;
            }

            જો (*p == 'e' || *p == 'E') {
                /* No sense, but permitted by RFC */
                state = ngx_json_સંખ્યા_exp;
                તોડો;
            }

            p--;

            rc = ngx_json_end_સંખ્યા(ctx, start, p + 1, &state);
            જો (rc != NGX_OK) {
                પાછા rc;
            }

            તોડો;

        સ્થિતિ ngx_json_સંખ્યા_int:
            જો (*p >= '0' && *p <= '9') {
                તોડો;
            }

            જો (*p == '.') {
                state = ngx_json_સંખ્યા_frac;
                તોડો;
            }

            જો (*p == 'e' || *p == 'E') {
                /* No sense, but permitted by RFC */
                state = ngx_json_સંખ્યા_exp;
                તોડો;
            }

            p--;

            rc = ngx_json_end_સંખ્યા(ctx, start, p + 1, &state);
            જો (rc!= NGX_OK) {
                પાછા rc;
            }

            તોડો;

        સ્થિતિ ngx_json_સંખ્યા_frac:
            જો (*p >= '0' && *p <= '9') {
                state = ngx_json_સંખ્યા_frac_digit;
                તોડો;
            }

            પાછા NGX_DECLINED;

        સ્થિતિ ngx_json_સંખ્યા_frac_digit:
            જો (*p >= '0' && *p <= '9') {
                તોડો;
            }

            જો (*p == 'e' || *p == 'E') {
                state = ngx_json_સંખ્યા_exp;
                તોડો;
            }

            p--;

            rc = ngx_json_end_સંખ્યા(ctx, start, p + 1, &state);
            જો (rc != NGX_OK) {
                પાછા rc;
            }

            તોડો;

        સ્થિતિ ngx_json_સંખ્યા_exp:
            જો (*p == '-' || *p == '+') {
                state = ngx_json_સંખ્યા_exp_plusminus;
                તોડો;
            }

            જો (*p >= '0' && *p <= '9') {
                state = ngx_json_સંખ્યા_exp_digit;
                તોડો;
            }

            પાછા NGX_DECLINED;

        સ્થિતિ ngx_json_સંખ્યા_exp_plusminus:
            જો (*p >= '0' && *p <= '9') {
                state = ngx_json_સંખ્યા_exp_digit;
                તોડો;
            }

            પાછા NGX_DECLINED;

        સ્થિતિ ngx_json_સંખ્યા_exp_digit:
            જો (*p >= '0' && *p <= '9') {
                તોડો;
            }

            p--;

            rc = ngx_json_end_સંખ્યા(ctx, start, p + 1, &state);
            જો (rc != NGX_OK) {
                પાછા rc;
            }

            તોડો;

        સ્થિતિ ngx_json_true_t:
            જો (*p == 'r') {
                state = ngx_json_true_tr;
                તોડો;
            }

            પાછા NGX_DECLINED;

        સ્થિતિ ngx_json_true_tr:
            જો (*p == 'u') {
                state = ngx_json_true_tru;
                તોડો;
            }

            પાછા NGX_DECLINED;

        સ્થિતિ ngx_json_true_tru:
            જો (*p == 'e') {
                rc = ngx_json_emit(ctx, NGX_JSON_કિંમત_BOOL, start,
                                  p + 1 - start);
                જો (rc != NGX_OK) {
                    પાછા rc;
                }

                state = ngx_json_after_કિંમત(ctx);
                તોડો;
            }

            પાછા NGX_DECLINED;

        સ્થિતિ ngx_json_false_f:
            જો (*p == 'a') {
                state = ngx_json_false_fa;
                તોડો;
            }

            પાછા NGX_DECLINED;

        સ્થિતિ ngx_json_false_fa:
            જો (*p == 'l') {
                state = ngx_json_false_fal;
                તોડો;
            }

            પાછા NGX_DECLINED;

        સ્થિતિ ngx_json_false_fal:
            જો (*p == 's') {
                state = ngx_json_false_fals;
                તોડો;
            }

            પાછા NGX_DECLINED;

        સ્થિતિ ngx_json_false_fals:
            જો (*p == 'e') {
                rc = ngx_json_emit(ctx, NGX_JSON_કિંમત_BOOL, start,
                                  p + 1 - start);
                જો (rc != NGX_OK) {
                    પાછા rc;
                }

                state = ngx_json_after_કિંમત(ctx);
                તોડો;
            }

            પાછા NGX_DECLINED;

        સ્થિતિ ngx_json_null_n:
            જો (*p == 'u') {
                state = ngx_json_null_nu;
                તોડો;
            }

            પાછા NGX_DECLINED;

        સ્થિતિ ngx_json_null_nu:
            જો (*p == 'l') {
                state = ngx_json_null_nul;
                તોડો;
            }

            પાછા NGX_DECLINED;

        સ્થિતિ ngx_json_null_nul:
            જો (*p == 'l') {
                rc = ngx_json_emit(ctx, NGX_JSON_કિંમત_NULL, start,
                                  p + 1 - start);
                જો (rc != NGX_OK) {
                    પાછા rc;
                }

                state = ngx_json_after_કિંમત(ctx);
                તોડો;
            }

            પાછા NGX_DECLINED;

        સ્થિતિ ngx_json_done:
            જો (ngx_json_ws(*p)) {
                તોડો;
            }

            પાછા NGX_DECLINED;
        }

        skip_depth = ctx->skip_until_depth;
    }

    /* a bare top-level number is not self-delimited; emit it at end of input */

    જો (ctx->depth == 0
        && (state == ngx_json_સંખ્યા_int_zero
            || state == ngx_json_સંખ્યા_int
            || state == ngx_json_સંખ્યા_frac_digit
            || state == ngx_json_સંખ્યા_exp_digit))
    {
        rc = ngx_json_emit(ctx, NGX_JSON_કિંમત_સંખ્યા, start, last - start);
        જો (rc != NGX_OK) {
            પાછા rc;
        }

        state = ngx_json_done;
    }

    ctx->state = state;

    જો (state == ngx_json_done) {
        પાછા NGX_OK;
    }

    પાછા NGX_DECLINED;
}


ngx_int_t
ngx_json_parse(ngx_pool_t *pool, ngx_str_t *json, ngx_json_handler_pt handler,
    શૂન્ય *data)
{
    ngx_json_ctx_t  jctx;

    ngx_json_ctx_init(&jctx, pool);

    jctx.data = data;
    jctx.handler = handler;

    પાછા ngx_json_parse_ctx(&jctx, json->data, json->len);
}


સ્થિર ngx_inline ngx_int_t
ngx_json_end_સંખ્યા(ngx_json_ctx_t *ctx, u_char *start, u_char *end,
    ngx_json_state_e *state)
{
    ngx_int_t  rc;

    rc = ngx_json_emit(ctx, NGX_JSON_કિંમત_સંખ્યા, start, end - start);
    જો (rc != NGX_OK) {
        પાછા rc;
    }

    *state = ngx_json_after_કિંમત(ctx);

    પાછા NGX_OK;
}


સ્થિર ngx_inline ngx_int_t
ngx_json_emit(ngx_json_ctx_t *ctx, ngx_json_event_e event, u_char *data,
    size_t len)
{
    ngx_int_t  rc;
    ngx_str_t  token;

    જો (ngx_json_skipping(ctx->skip_until_depth)) {
        પાછા NGX_OK;
    }

    જો (ctx->handler == ખાલી) {
        પાછા NGX_OK;
    }

    token.data = data;
    token.len = len;
    rc = ctx->handler(ctx, event, &token);

    જો (rc == NGX_JSON_SKIP) {

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

        પસંદગી (event) {

        સ્થિતિ NGX_JSON_ચાવી:
        સ્થિતિ NGX_JSON_ચાવી_ESCAPED:
        સ્થિતિ NGX_JSON_OBJECT_OPEN:
        સ્થિતિ NGX_JSON_ARRAY_OPEN:
            ctx->skip_until_depth = ctx->depth;
            તોડો;

        સ્થિતિ NGX_JSON_કિંમત_STRING:
        સ્થિતિ NGX_JSON_કિંમત_STRING_ESCAPED:
        સ્થિતિ NGX_JSON_કિંમત_સંખ્યા:
        સ્થિતિ NGX_JSON_કિંમત_BOOL:
        સ્થિતિ NGX_JSON_કિંમત_NULL:
            જો (ctx->depth > 0
                && ctx->ઢગલો[ctx->depth - 1] == ngx_json_ctx_array
            ) {
                ctx->skip_until_depth = ctx->depth - 1;
            }
            તોડો;

        મૂળભૂત:
            તોડો;
        }

        પાછા NGX_OK;
    }

    પાછા rc;
}


સ્થિર ngx_inline ngx_json_state_e
ngx_json_after_કિંમત(ngx_json_ctx_t *ctx)
{
    જો (ctx->depth == 0) {
        પાછા ngx_json_done;
    }

    જો (ctx->ઢગલો[ctx->depth - 1] == ngx_json_ctx_object) {
        પાછા ngx_json_object_કિંમત_separator;
    }

    પાછા ngx_json_array_કિંમત_separator;
}


સ્થિર ngx_inline ngx_int_t
ngx_json_close(ngx_json_ctx_t *ctx, ngx_json_event_e event,
    ngx_json_state_e *state, u_char *p)
{
    ngx_int_t  rc;

    ctx->depth--;

    rc = ngx_json_emit(ctx, event, p, 1);
    જો (rc != NGX_OK) {
        પાછા rc;
    }

    *state = ngx_json_after_કિંમત(ctx);

    પાછા NGX_OK;
}


સ્થિર ngx_int_t
ngx_json_ધકેલો_state(ngx_json_ctx_t *ctx, ngx_json_container_e container)
{
    ngx_json_container_e  *ઢગલો;

    જો (ctx->depth >= ctx->max_depth) {
        પાછા NGX_DECLINED;
    }

    જો (ctx->ઢગલો == ખાલી) {
        ઢગલો = ngx_palloc(ctx->pool,
                           ctx->max_depth * કદ(ngx_json_container_e));
        જો (ઢગલો == ખાલી) {
            પાછા NGX_ERROR;
        }

        ctx->ઢગલો = ઢગલો;
    }

    ctx->ઢગલો[ctx->depth] = container;
    ctx->depth++;

    પાછા NGX_OK;
}
