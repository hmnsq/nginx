
/*
 * Copyright (C) Maxim Dounin
 * Copyright (C) Nginx, Inc.
 */


/*
 * JSON parser. RFC 8259 for the JSON grammar.

 * Site http://json.org/ may be useful too.
 */


#অন্তর্ভাৱ <ngx_config.h>
#অন্তর্ভাৱ <ngx_core.h>
#অন্তর্ভাৱ <ngx_json_parse.h>


#define NGX_JSON_NOT_SKIPPING  (ngx_uint_t) -1
#define ngx_json_skipping(d)   ((d) != NGX_JSON_NOT_SKIPPING)
#define ngx_json_ws(c)  ((c) == ' ' || (c) == '\t' || (c) == CR || (c) == LF)


স্থির ngx_inline ngx_int_t ngx_json_end_সংখ্যা(ngx_json_ctx_t *ctx,
    u_char *start, u_char *end, ngx_json_state_e *state);
স্থির ngx_inline ngx_int_t ngx_json_emit(ngx_json_ctx_t *ctx,
    ngx_json_event_e event, u_char *data, size_t len);
স্থির ngx_inline ngx_json_state_e ngx_json_after_মান(ngx_json_ctx_t *ctx);
স্থির ngx_inline ngx_int_t ngx_json_close(ngx_json_ctx_t *ctx,
    ngx_json_event_e event, ngx_json_state_e *state, u_char *p);
স্থির ngx_int_t ngx_json_ঠেলো_state(ngx_json_ctx_t *ctx,
    ngx_json_container_e container);


শূন্য
ngx_json_ctx_init(ngx_json_ctx_t *ctx, ngx_pool_t *pool)
{
    ctx->state = ngx_json_start;
    ctx->pool = pool;
    ctx->handler = রিক্ত;
    ctx->data = রিক্ত;
    ctx->codepoint = 0;
    ctx->hex_বাম = 0;

    ctx->depth = 0;
    ctx->skip_until_depth = NGX_JSON_NOT_SKIPPING;
    ctx->max_depth = NGX_JSON_DEFAULT_MAX_DEPTH;

    /* stack is allocated lazily on first push, sized to max_depth */

    ctx->স্তূপ = রিক্ত;

    ctx->in_চাবি = 0;
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

    যদি (len == 0) {
        ঘূৰাই NGX_DECLINED;
    }

    যদি (data == রিক্ত) {
        ঘূৰাই NGX_ERROR;
    }

    যদি (ctx->state == ngx_json_done) {
        ঘূৰাই NGX_DECLINED;
    }

    last = data + len;
    state = ctx->state;
    skip_depth = ctx->skip_until_depth;
    start = রিক্ত;

    বাবে (p = data; p < last; p++) {

        /* skipping suppresses emission only; the machine still validates */

        যদি (ngx_json_skipping(skip_depth) && ctx->depth <= skip_depth
            && (state == ngx_json_object_মান_separator
                || state == ngx_json_array_মান_separator
                || state == ngx_json_done))
        {
            skip_depth = NGX_JSON_NOT_SKIPPING;
            ctx->skip_until_depth = NGX_JSON_NOT_SKIPPING;
            p--;

            অনুৱর্ত;
        }

        ৱিকল্প (state) {

        পক্ষ ngx_json_start:
            যদি (ngx_json_ws(*p)) {
                ভঙ্গ;
            }

            p--;
            state = ngx_json_মান;
            ভঙ্গ;

        পক্ষ ngx_json_object:
            যদি (ngx_json_ws(*p)) {
                ভঙ্গ;
            }

            যদি (*p == '}') {
                rc = ngx_json_close(ctx, NGX_JSON_OBJECT_CLOSE, &state, p);
                যদি (rc != NGX_OK) {
                    ঘূৰাই rc;
                }

                ভঙ্গ;
            }

            /* fall through */

        পক্ষ ngx_json_object_next:
            যদি (ngx_json_ws(*p)) {
                ভঙ্গ;
            }

            যদি (*p == '"') {
                ctx->in_চাবি = 1;
                ctx->escaped = 0;
                start = p + 1;
                state = ngx_json_string;
                ভঙ্গ;
            }

            ঘূৰাই NGX_DECLINED;

        পক্ষ ngx_json_object_name_separator:
            যদি (ngx_json_ws(*p)) {
                ভঙ্গ;
            }

            যদি (*p == ':') {
                state = ngx_json_মান;
                ভঙ্গ;
            }

            ঘূৰাই NGX_DECLINED;

        পক্ষ ngx_json_object_মান_separator:
            যদি (ngx_json_ws(*p)) {
                ভঙ্গ;
            }

            যদি (*p == ',') {
                state = ngx_json_object_next;
                ভঙ্গ;
            }

            যদি (*p == '}') {
                rc = ngx_json_close(ctx, NGX_JSON_OBJECT_CLOSE, &state, p);
                যদি (rc != NGX_OK) {
                    ঘূৰাই rc;
                }

                ভঙ্গ;
            }

            ঘূৰাই NGX_DECLINED;

        পক্ষ ngx_json_array_মান_separator:
            যদি (ngx_json_ws(*p)) {
                ভঙ্গ;
            }

            যদি (*p == ',') {
                state = ngx_json_মান;
                ভঙ্গ;
            }

            যদি (*p == ']') {
                rc = ngx_json_close(ctx, NGX_JSON_ARRAY_CLOSE, &state, p);
                যদি (rc != NGX_OK) {
                    ঘূৰাই rc;
                }

                ভঙ্গ;
            }

            ঘূৰাই NGX_DECLINED;

        পক্ষ ngx_json_array:
            যদি (ngx_json_ws(*p)) {
                ভঙ্গ;
            }

            যদি (*p == ']') {
                rc = ngx_json_close(ctx, NGX_JSON_ARRAY_CLOSE, &state, p);
                যদি (rc != NGX_OK) {
                    ঘূৰাই rc;
                }

                ভঙ্গ;
            }

            /* fall through */

        পক্ষ ngx_json_মান:
            যদি (ngx_json_ws(*p)) {
                ভঙ্গ;
            }

            start = p;

            যদি (*p == '"') {
                ctx->in_চাবি = 0;
                ctx->escaped = 0;
                start = p + 1;
                state = ngx_json_string;
                ভঙ্গ;
            }

            যদি (*p == '{') {
                rc = ngx_json_emit(ctx, NGX_JSON_OBJECT_OPEN, p, 1);
                যদি (rc != NGX_OK) {
                    ঘূৰাই rc;
                }

                rc = ngx_json_ঠেলো_state(ctx, ngx_json_ctx_object);
                যদি (rc != NGX_OK) {
                    ঘূৰাই rc;
                }

                state = ngx_json_object;
                ভঙ্গ;
            }

            যদি (*p == '[') {
                rc = ngx_json_emit(ctx, NGX_JSON_ARRAY_OPEN, p, 1);
                যদি (rc != NGX_OK) {
                    ঘূৰাই rc;
                }

                rc = ngx_json_ঠেলো_state(ctx, ngx_json_ctx_array);
                যদি (rc != NGX_OK) {
                    ঘূৰাই rc;
                }

                state = ngx_json_array;
                ভঙ্গ;
            }

            যদি (*p == '-') {
                state = ngx_json_সংখ্যা_minus;
                ভঙ্গ;
            }

            যদি (*p == '0') {
                state = ngx_json_সংখ্যা_int_zero;
                ভঙ্গ;
            }

            যদি (*p >= '1' && *p <= '9') {
                state = ngx_json_সংখ্যা_int;
                ভঙ্গ;
            }

            যদি (*p == 't') {
                state = ngx_json_true_t;
                ভঙ্গ;
            }

            যদি (*p == 'f') {
                state = ngx_json_false_f;
                ভঙ্গ;
            }

            যদি (*p == 'n') {
                state = ngx_json_null_n;
                ভঙ্গ;
            }

            ঘূৰাই NGX_DECLINED;

        পক্ষ ngx_json_string:
            যদি (*p == '"') {

                যদি (ctx->in_চাবি) {
                    event = ctx->escaped ? NGX_JSON_চাবি_ESCAPED
                                         : NGX_JSON_চাবি;

                    rc = ngx_json_emit(ctx, event, start, p - start);
                    যদি (rc != NGX_OK) {
                        ঘূৰাই rc;
                    }

                    ctx->in_চাবি = 0;
                    state = ngx_json_object_name_separator;

                } অন্যথা {
                    event = ctx->escaped ? NGX_JSON_মান_STRING_ESCAPED
                                         : NGX_JSON_মান_STRING;

                    rc = ngx_json_emit(ctx, event, start, p - start);
                    যদি (rc != NGX_OK) {
                        ঘূৰাই rc;
                    }

                    state = ngx_json_after_মান(ctx);
                }

                ভঙ্গ;
            }

            যদি (*p == '\\') {
                ctx->escaped = 1;
                state = ngx_json_escaped;
                ভঙ্গ;
            }

            যদি (*p < ' ') {
                ঘূৰাই NGX_DECLINED;
            }

            ভঙ্গ;

        পক্ষ ngx_json_escaped:
            যদি (*p == '"' || *p == '\\' || *p == '/' || *p == 'b'
                || *p == 'f' || *p == 'n' || *p == 'r' || *p == 't')
            {
                state = ngx_json_string;
                ভঙ্গ;
            }

            যদি (*p == 'u') {
                ctx->codepoint = 0;
                ctx->hex_বাম = 4;
                state = ngx_json_escaped_hex;
                ভঙ্গ;
            }

            ঘূৰাই NGX_DECLINED;

        পক্ষ ngx_json_escaped_hex:

            /* accumulate the four hex digits of the first \uXXXX escape */

            hd = ngx_json_hex_digit(*p);
            যদি (hd == NGX_ERROR) {
                ঘূৰাই NGX_DECLINED;
            }

            ctx->codepoint = (ctx->codepoint << 4) | (uint32_t) hd;

            যদি (--ctx->hex_বাম > 0) {
                ভঙ্গ;
            }

            যদি (ctx->codepoint >= 0xD800 && ctx->codepoint <= 0xDBFF) {
                /* high surrogate - must be followed by \uDC00..\uDFFF */
                state = ngx_json_surrogate_start;
                ভঙ্গ;
            }

            যদি (ctx->codepoint >= 0xDC00 && ctx->codepoint <= 0xDFFF) {
                /* lone low surrogate */
                ঘূৰাই NGX_DECLINED;
            }

            state = ngx_json_string;
            ভঙ্গ;

        পক্ষ ngx_json_surrogate_start:
            যদি (*p == '\\') {
                state = ngx_json_surrogate_u;
                ভঙ্গ;
            }

            /* high surrogate not followed by \uXXXX */
            ঘূৰাই NGX_DECLINED;

        পক্ষ ngx_json_surrogate_u:
            যদি (*p == 'u') {
                ctx->codepoint = 0;
                ctx->hex_বাম = 4;
                state = ngx_json_surrogate_hex;
                ভঙ্গ;
            }

            /* high surrogate followed by non-\u escape */
            ঘূৰাই NGX_DECLINED;

        পক্ষ ngx_json_surrogate_hex:

            /* accumulate the four hex digits of the low \uXXXX escape */

            hd = ngx_json_hex_digit(*p);
            যদি (hd == NGX_ERROR) {
                ঘূৰাই NGX_DECLINED;
            }

            ctx->codepoint = (ctx->codepoint << 4) | (uint32_t) hd;

            যদি (--ctx->hex_বাম > 0) {
                ভঙ্গ;
            }

            যদি (ctx->codepoint >= 0xDC00 && ctx->codepoint <= 0xDFFF) {
                /* valid surrogate pair */
                state = ngx_json_string;
                ভঙ্গ;
            }

            /* high surrogate not followed by low surrogate */
            ঘূৰাই NGX_DECLINED;

        পক্ষ ngx_json_সংখ্যা_minus:
            যদি (*p >= '1' && *p <= '9') {
                state = ngx_json_সংখ্যা_int;
                ভঙ্গ;
            }

            যদি (*p == '0') {
                state = ngx_json_সংখ্যা_int_zero;
                ভঙ্গ;
            }

            ঘূৰাই NGX_DECLINED;

        পক্ষ ngx_json_সংখ্যা_int_zero:
            যদি (*p == '.') {
                state = ngx_json_সংখ্যা_frac;
                ভঙ্গ;
            }

            যদি (*p == 'e' || *p == 'E') {
                /* No sense, but permitted by RFC */
                state = ngx_json_সংখ্যা_exp;
                ভঙ্গ;
            }

            p--;

            rc = ngx_json_end_সংখ্যা(ctx, start, p + 1, &state);
            যদি (rc != NGX_OK) {
                ঘূৰাই rc;
            }

            ভঙ্গ;

        পক্ষ ngx_json_সংখ্যা_int:
            যদি (*p >= '0' && *p <= '9') {
                ভঙ্গ;
            }

            যদি (*p == '.') {
                state = ngx_json_সংখ্যা_frac;
                ভঙ্গ;
            }

            যদি (*p == 'e' || *p == 'E') {
                /* No sense, but permitted by RFC */
                state = ngx_json_সংখ্যা_exp;
                ভঙ্গ;
            }

            p--;

            rc = ngx_json_end_সংখ্যা(ctx, start, p + 1, &state);
            যদি (rc!= NGX_OK) {
                ঘূৰাই rc;
            }

            ভঙ্গ;

        পক্ষ ngx_json_সংখ্যা_frac:
            যদি (*p >= '0' && *p <= '9') {
                state = ngx_json_সংখ্যা_frac_digit;
                ভঙ্গ;
            }

            ঘূৰাই NGX_DECLINED;

        পক্ষ ngx_json_সংখ্যা_frac_digit:
            যদি (*p >= '0' && *p <= '9') {
                ভঙ্গ;
            }

            যদি (*p == 'e' || *p == 'E') {
                state = ngx_json_সংখ্যা_exp;
                ভঙ্গ;
            }

            p--;

            rc = ngx_json_end_সংখ্যা(ctx, start, p + 1, &state);
            যদি (rc != NGX_OK) {
                ঘূৰাই rc;
            }

            ভঙ্গ;

        পক্ষ ngx_json_সংখ্যা_exp:
            যদি (*p == '-' || *p == '+') {
                state = ngx_json_সংখ্যা_exp_plusminus;
                ভঙ্গ;
            }

            যদি (*p >= '0' && *p <= '9') {
                state = ngx_json_সংখ্যা_exp_digit;
                ভঙ্গ;
            }

            ঘূৰাই NGX_DECLINED;

        পক্ষ ngx_json_সংখ্যা_exp_plusminus:
            যদি (*p >= '0' && *p <= '9') {
                state = ngx_json_সংখ্যা_exp_digit;
                ভঙ্গ;
            }

            ঘূৰাই NGX_DECLINED;

        পক্ষ ngx_json_সংখ্যা_exp_digit:
            যদি (*p >= '0' && *p <= '9') {
                ভঙ্গ;
            }

            p--;

            rc = ngx_json_end_সংখ্যা(ctx, start, p + 1, &state);
            যদি (rc != NGX_OK) {
                ঘূৰাই rc;
            }

            ভঙ্গ;

        পক্ষ ngx_json_true_t:
            যদি (*p == 'r') {
                state = ngx_json_true_tr;
                ভঙ্গ;
            }

            ঘূৰাই NGX_DECLINED;

        পক্ষ ngx_json_true_tr:
            যদি (*p == 'u') {
                state = ngx_json_true_tru;
                ভঙ্গ;
            }

            ঘূৰাই NGX_DECLINED;

        পক্ষ ngx_json_true_tru:
            যদি (*p == 'e') {
                rc = ngx_json_emit(ctx, NGX_JSON_মান_BOOL, start,
                                  p + 1 - start);
                যদি (rc != NGX_OK) {
                    ঘূৰাই rc;
                }

                state = ngx_json_after_মান(ctx);
                ভঙ্গ;
            }

            ঘূৰাই NGX_DECLINED;

        পক্ষ ngx_json_false_f:
            যদি (*p == 'a') {
                state = ngx_json_false_fa;
                ভঙ্গ;
            }

            ঘূৰাই NGX_DECLINED;

        পক্ষ ngx_json_false_fa:
            যদি (*p == 'l') {
                state = ngx_json_false_fal;
                ভঙ্গ;
            }

            ঘূৰাই NGX_DECLINED;

        পক্ষ ngx_json_false_fal:
            যদি (*p == 's') {
                state = ngx_json_false_fals;
                ভঙ্গ;
            }

            ঘূৰাই NGX_DECLINED;

        পক্ষ ngx_json_false_fals:
            যদি (*p == 'e') {
                rc = ngx_json_emit(ctx, NGX_JSON_মান_BOOL, start,
                                  p + 1 - start);
                যদি (rc != NGX_OK) {
                    ঘূৰাই rc;
                }

                state = ngx_json_after_মান(ctx);
                ভঙ্গ;
            }

            ঘূৰাই NGX_DECLINED;

        পক্ষ ngx_json_null_n:
            যদি (*p == 'u') {
                state = ngx_json_null_nu;
                ভঙ্গ;
            }

            ঘূৰাই NGX_DECLINED;

        পক্ষ ngx_json_null_nu:
            যদি (*p == 'l') {
                state = ngx_json_null_nul;
                ভঙ্গ;
            }

            ঘূৰাই NGX_DECLINED;

        পক্ষ ngx_json_null_nul:
            যদি (*p == 'l') {
                rc = ngx_json_emit(ctx, NGX_JSON_মান_NULL, start,
                                  p + 1 - start);
                যদি (rc != NGX_OK) {
                    ঘূৰাই rc;
                }

                state = ngx_json_after_মান(ctx);
                ভঙ্গ;
            }

            ঘূৰাই NGX_DECLINED;

        পক্ষ ngx_json_done:
            যদি (ngx_json_ws(*p)) {
                ভঙ্গ;
            }

            ঘূৰাই NGX_DECLINED;
        }

        skip_depth = ctx->skip_until_depth;
    }

    /* a bare top-level number is not self-delimited; emit it at end of input */

    যদি (ctx->depth == 0
        && (state == ngx_json_সংখ্যা_int_zero
            || state == ngx_json_সংখ্যা_int
            || state == ngx_json_সংখ্যা_frac_digit
            || state == ngx_json_সংখ্যা_exp_digit))
    {
        rc = ngx_json_emit(ctx, NGX_JSON_মান_সংখ্যা, start, last - start);
        যদি (rc != NGX_OK) {
            ঘূৰাই rc;
        }

        state = ngx_json_done;
    }

    ctx->state = state;

    যদি (state == ngx_json_done) {
        ঘূৰাই NGX_OK;
    }

    ঘূৰাই NGX_DECLINED;
}


ngx_int_t
ngx_json_parse(ngx_pool_t *pool, ngx_str_t *json, ngx_json_handler_pt handler,
    শূন্য *data)
{
    ngx_json_ctx_t  jctx;

    ngx_json_ctx_init(&jctx, pool);

    jctx.data = data;
    jctx.handler = handler;

    ঘূৰাই ngx_json_parse_ctx(&jctx, json->data, json->len);
}


স্থির ngx_inline ngx_int_t
ngx_json_end_সংখ্যা(ngx_json_ctx_t *ctx, u_char *start, u_char *end,
    ngx_json_state_e *state)
{
    ngx_int_t  rc;

    rc = ngx_json_emit(ctx, NGX_JSON_মান_সংখ্যা, start, end - start);
    যদি (rc != NGX_OK) {
        ঘূৰাই rc;
    }

    *state = ngx_json_after_মান(ctx);

    ঘূৰাই NGX_OK;
}


স্থির ngx_inline ngx_int_t
ngx_json_emit(ngx_json_ctx_t *ctx, ngx_json_event_e event, u_char *data,
    size_t len)
{
    ngx_int_t  rc;
    ngx_str_t  token;

    যদি (ngx_json_skipping(ctx->skip_until_depth)) {
        ঘূৰাই NGX_OK;
    }

    যদি (ctx->handler == রিক্ত) {
        ঘূৰাই NGX_OK;
    }

    token.data = data;
    token.len = len;
    rc = ctx->handler(ctx, event, &token);

    যদি (rc == NGX_JSON_SKIP) {

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

        ৱিকল্প (event) {

        পক্ষ NGX_JSON_চাবি:
        পক্ষ NGX_JSON_চাবি_ESCAPED:
        পক্ষ NGX_JSON_OBJECT_OPEN:
        পক্ষ NGX_JSON_ARRAY_OPEN:
            ctx->skip_until_depth = ctx->depth;
            ভঙ্গ;

        পক্ষ NGX_JSON_মান_STRING:
        পক্ষ NGX_JSON_মান_STRING_ESCAPED:
        পক্ষ NGX_JSON_মান_সংখ্যা:
        পক্ষ NGX_JSON_মান_BOOL:
        পক্ষ NGX_JSON_মান_NULL:
            যদি (ctx->depth > 0
                && ctx->স্তূপ[ctx->depth - 1] == ngx_json_ctx_array
            ) {
                ctx->skip_until_depth = ctx->depth - 1;
            }
            ভঙ্গ;

        স্ৱভাৱ:
            ভঙ্গ;
        }

        ঘূৰাই NGX_OK;
    }

    ঘূৰাই rc;
}


স্থির ngx_inline ngx_json_state_e
ngx_json_after_মান(ngx_json_ctx_t *ctx)
{
    যদি (ctx->depth == 0) {
        ঘূৰাই ngx_json_done;
    }

    যদি (ctx->স্তূপ[ctx->depth - 1] == ngx_json_ctx_object) {
        ঘূৰাই ngx_json_object_মান_separator;
    }

    ঘূৰাই ngx_json_array_মান_separator;
}


স্থির ngx_inline ngx_int_t
ngx_json_close(ngx_json_ctx_t *ctx, ngx_json_event_e event,
    ngx_json_state_e *state, u_char *p)
{
    ngx_int_t  rc;

    ctx->depth--;

    rc = ngx_json_emit(ctx, event, p, 1);
    যদি (rc != NGX_OK) {
        ঘূৰাই rc;
    }

    *state = ngx_json_after_মান(ctx);

    ঘূৰাই NGX_OK;
}


স্থির ngx_int_t
ngx_json_ঠেলো_state(ngx_json_ctx_t *ctx, ngx_json_container_e container)
{
    ngx_json_container_e  *স্তূপ;

    যদি (ctx->depth >= ctx->max_depth) {
        ঘূৰাই NGX_DECLINED;
    }

    যদি (ctx->স্তূপ == রিক্ত) {
        স্তূপ = ngx_palloc(ctx->pool,
                           ctx->max_depth * পরিমাণ(ngx_json_container_e));
        যদি (স্তূপ == রিক্ত) {
            ঘূৰাই NGX_ERROR;
        }

        ctx->স্তূপ = স্তূপ;
    }

    ctx->স্তূপ[ctx->depth] = container;
    ctx->depth++;

    ঘূৰাই NGX_OK;
}
