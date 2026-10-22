
/*
 * Copyright (C) Maxim Dounin
 * Copyright (C) Nginx, Inc.
 */


/*
 * JSON parser. RFC 8259 for the JSON grammar.

 * Site http://json.org/ may be useful too.
 */


#ଅନ୍ତର୍ଭାଵ <ngx_config.h>
#ଅନ୍ତର୍ଭାଵ <ngx_core.h>
#ଅନ୍ତର୍ଭାଵ <ngx_json_parse.h>


#define NGX_JSON_NOT_SKIPPING  (ngx_uint_t) -1
#define ngx_json_skipping(d)   ((d) != NGX_JSON_NOT_SKIPPING)
#define ngx_json_ws(c)  ((c) == ' ' || (c) == '\t' || (c) == CR || (c) == LF)


ସ୍ଥିର ngx_inline ngx_int_t ngx_json_end_ସଂଖ୍ୟା(ngx_json_ctx_t *ctx,
    u_char *start, u_char *end, ngx_json_state_e *state);
ସ୍ଥିର ngx_inline ngx_int_t ngx_json_emit(ngx_json_ctx_t *ctx,
    ngx_json_event_e event, u_char *data, size_t len);
ସ୍ଥିର ngx_inline ngx_json_state_e ngx_json_after_ମୂଲ୍ୟ(ngx_json_ctx_t *ctx);
ସ୍ଥିର ngx_inline ngx_int_t ngx_json_close(ngx_json_ctx_t *ctx,
    ngx_json_event_e event, ngx_json_state_e *state, u_char *p);
ସ୍ଥିର ngx_int_t ngx_json_ଠେଲ_state(ngx_json_ctx_t *ctx,
    ngx_json_container_e container);


ଶୂନ୍ୟ
ngx_json_ctx_init(ngx_json_ctx_t *ctx, ngx_pool_t *pool)
{
    ctx->state = ngx_json_start;
    ctx->pool = pool;
    ctx->handler = ଖାଲି;
    ctx->data = ଖାଲି;
    ctx->codepoint = 0;
    ctx->hex_ବାମ = 0;

    ctx->depth = 0;
    ctx->skip_until_depth = NGX_JSON_NOT_SKIPPING;
    ctx->max_depth = NGX_JSON_DEFAULT_MAX_DEPTH;

    /* stack is allocated lazily on first push, sized to max_depth */

    ctx->ଗଦା = ଖାଲି;

    ctx->in_ଚାବି = 0;
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

    ଯଦି (len == 0) {
        ଫେରନ୍ତୁ NGX_DECLINED;
    }

    ଯଦି (data == ଖାଲି) {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    ଯଦି (ctx->state == ngx_json_done) {
        ଫେରନ୍ତୁ NGX_DECLINED;
    }

    last = data + len;
    state = ctx->state;
    skip_depth = ctx->skip_until_depth;
    start = ଖାଲି;

    ପାଇଁ (p = data; p < last; p++) {

        /* skipping suppresses emission only; the machine still validates */

        ଯଦି (ngx_json_skipping(skip_depth) && ctx->depth <= skip_depth
            && (state == ngx_json_object_ମୂଲ୍ୟ_separator
                || state == ngx_json_array_ମୂଲ୍ୟ_separator
                || state == ngx_json_done))
        {
            skip_depth = NGX_JSON_NOT_SKIPPING;
            ctx->skip_until_depth = NGX_JSON_NOT_SKIPPING;
            p--;

            ଜାରି;
        }

        ବାଛ (state) {

        ସ୍ଥିତି ngx_json_start:
            ଯଦି (ngx_json_ws(*p)) {
                ଭାଙ୍ଗ;
            }

            p--;
            state = ngx_json_ମୂଲ୍ୟ;
            ଭାଙ୍ଗ;

        ସ୍ଥିତି ngx_json_object:
            ଯଦି (ngx_json_ws(*p)) {
                ଭାଙ୍ଗ;
            }

            ଯଦି (*p == '}') {
                rc = ngx_json_close(ctx, NGX_JSON_OBJECT_CLOSE, &state, p);
                ଯଦି (rc != NGX_OK) {
                    ଫେରନ୍ତୁ rc;
                }

                ଭାଙ୍ଗ;
            }

            /* fall through */

        ସ୍ଥିତି ngx_json_object_next:
            ଯଦି (ngx_json_ws(*p)) {
                ଭାଙ୍ଗ;
            }

            ଯଦି (*p == '"') {
                ctx->in_ଚାବି = 1;
                ctx->escaped = 0;
                start = p + 1;
                state = ngx_json_string;
                ଭାଙ୍ଗ;
            }

            ଫେରନ୍ତୁ NGX_DECLINED;

        ସ୍ଥିତି ngx_json_object_name_separator:
            ଯଦି (ngx_json_ws(*p)) {
                ଭାଙ୍ଗ;
            }

            ଯଦି (*p == ':') {
                state = ngx_json_ମୂଲ୍ୟ;
                ଭାଙ୍ଗ;
            }

            ଫେରନ୍ତୁ NGX_DECLINED;

        ସ୍ଥିତି ngx_json_object_ମୂଲ୍ୟ_separator:
            ଯଦି (ngx_json_ws(*p)) {
                ଭାଙ୍ଗ;
            }

            ଯଦି (*p == ',') {
                state = ngx_json_object_next;
                ଭାଙ୍ଗ;
            }

            ଯଦି (*p == '}') {
                rc = ngx_json_close(ctx, NGX_JSON_OBJECT_CLOSE, &state, p);
                ଯଦି (rc != NGX_OK) {
                    ଫେରନ୍ତୁ rc;
                }

                ଭାଙ୍ଗ;
            }

            ଫେରନ୍ତୁ NGX_DECLINED;

        ସ୍ଥିତି ngx_json_array_ମୂଲ୍ୟ_separator:
            ଯଦି (ngx_json_ws(*p)) {
                ଭାଙ୍ଗ;
            }

            ଯଦି (*p == ',') {
                state = ngx_json_ମୂଲ୍ୟ;
                ଭାଙ୍ଗ;
            }

            ଯଦି (*p == ']') {
                rc = ngx_json_close(ctx, NGX_JSON_ARRAY_CLOSE, &state, p);
                ଯଦି (rc != NGX_OK) {
                    ଫେରନ୍ତୁ rc;
                }

                ଭାଙ୍ଗ;
            }

            ଫେରନ୍ତୁ NGX_DECLINED;

        ସ୍ଥିତି ngx_json_array:
            ଯଦି (ngx_json_ws(*p)) {
                ଭାଙ୍ଗ;
            }

            ଯଦି (*p == ']') {
                rc = ngx_json_close(ctx, NGX_JSON_ARRAY_CLOSE, &state, p);
                ଯଦି (rc != NGX_OK) {
                    ଫେରନ୍ତୁ rc;
                }

                ଭାଙ୍ଗ;
            }

            /* fall through */

        ସ୍ଥିତି ngx_json_ମୂଲ୍ୟ:
            ଯଦି (ngx_json_ws(*p)) {
                ଭାଙ୍ଗ;
            }

            start = p;

            ଯଦି (*p == '"') {
                ctx->in_ଚାବି = 0;
                ctx->escaped = 0;
                start = p + 1;
                state = ngx_json_string;
                ଭାଙ୍ଗ;
            }

            ଯଦି (*p == '{') {
                rc = ngx_json_emit(ctx, NGX_JSON_OBJECT_OPEN, p, 1);
                ଯଦି (rc != NGX_OK) {
                    ଫେରନ୍ତୁ rc;
                }

                rc = ngx_json_ଠେଲ_state(ctx, ngx_json_ctx_object);
                ଯଦି (rc != NGX_OK) {
                    ଫେରନ୍ତୁ rc;
                }

                state = ngx_json_object;
                ଭାଙ୍ଗ;
            }

            ଯଦି (*p == '[') {
                rc = ngx_json_emit(ctx, NGX_JSON_ARRAY_OPEN, p, 1);
                ଯଦି (rc != NGX_OK) {
                    ଫେରନ୍ତୁ rc;
                }

                rc = ngx_json_ଠେଲ_state(ctx, ngx_json_ctx_array);
                ଯଦି (rc != NGX_OK) {
                    ଫେରନ୍ତୁ rc;
                }

                state = ngx_json_array;
                ଭାଙ୍ଗ;
            }

            ଯଦି (*p == '-') {
                state = ngx_json_ସଂଖ୍ୟା_minus;
                ଭାଙ୍ଗ;
            }

            ଯଦି (*p == '0') {
                state = ngx_json_ସଂଖ୍ୟା_int_zero;
                ଭାଙ୍ଗ;
            }

            ଯଦି (*p >= '1' && *p <= '9') {
                state = ngx_json_ସଂଖ୍ୟା_int;
                ଭାଙ୍ଗ;
            }

            ଯଦି (*p == 't') {
                state = ngx_json_true_t;
                ଭାଙ୍ଗ;
            }

            ଯଦି (*p == 'f') {
                state = ngx_json_false_f;
                ଭାଙ୍ଗ;
            }

            ଯଦି (*p == 'n') {
                state = ngx_json_null_n;
                ଭାଙ୍ଗ;
            }

            ଫେରନ୍ତୁ NGX_DECLINED;

        ସ୍ଥିତି ngx_json_string:
            ଯଦି (*p == '"') {

                ଯଦି (ctx->in_ଚାବି) {
                    event = ctx->escaped ? NGX_JSON_ଚାବି_ESCAPED
                                         : NGX_JSON_ଚାବି;

                    rc = ngx_json_emit(ctx, event, start, p - start);
                    ଯଦି (rc != NGX_OK) {
                        ଫେରନ୍ତୁ rc;
                    }

                    ctx->in_ଚାବି = 0;
                    state = ngx_json_object_name_separator;

                } ଅନ୍ୟଥା {
                    event = ctx->escaped ? NGX_JSON_ମୂଲ୍ୟ_STRING_ESCAPED
                                         : NGX_JSON_ମୂଲ୍ୟ_STRING;

                    rc = ngx_json_emit(ctx, event, start, p - start);
                    ଯଦି (rc != NGX_OK) {
                        ଫେରନ୍ତୁ rc;
                    }

                    state = ngx_json_after_ମୂଲ୍ୟ(ctx);
                }

                ଭାଙ୍ଗ;
            }

            ଯଦି (*p == '\\') {
                ctx->escaped = 1;
                state = ngx_json_escaped;
                ଭାଙ୍ଗ;
            }

            ଯଦି (*p < ' ') {
                ଫେରନ୍ତୁ NGX_DECLINED;
            }

            ଭାଙ୍ଗ;

        ସ୍ଥିତି ngx_json_escaped:
            ଯଦି (*p == '"' || *p == '\\' || *p == '/' || *p == 'b'
                || *p == 'f' || *p == 'n' || *p == 'r' || *p == 't')
            {
                state = ngx_json_string;
                ଭାଙ୍ଗ;
            }

            ଯଦି (*p == 'u') {
                ctx->codepoint = 0;
                ctx->hex_ବାମ = 4;
                state = ngx_json_escaped_hex;
                ଭାଙ୍ଗ;
            }

            ଫେରନ୍ତୁ NGX_DECLINED;

        ସ୍ଥିତି ngx_json_escaped_hex:

            /* accumulate the four hex digits of the first \uXXXX escape */

            hd = ngx_json_hex_digit(*p);
            ଯଦି (hd == NGX_ERROR) {
                ଫେରନ୍ତୁ NGX_DECLINED;
            }

            ctx->codepoint = (ctx->codepoint << 4) | (uint32_t) hd;

            ଯଦି (--ctx->hex_ବାମ > 0) {
                ଭାଙ୍ଗ;
            }

            ଯଦି (ctx->codepoint >= 0xD800 && ctx->codepoint <= 0xDBFF) {
                /* high surrogate - must be followed by \uDC00..\uDFFF */
                state = ngx_json_surrogate_start;
                ଭାଙ୍ଗ;
            }

            ଯଦି (ctx->codepoint >= 0xDC00 && ctx->codepoint <= 0xDFFF) {
                /* lone low surrogate */
                ଫେରନ୍ତୁ NGX_DECLINED;
            }

            state = ngx_json_string;
            ଭାଙ୍ଗ;

        ସ୍ଥିତି ngx_json_surrogate_start:
            ଯଦି (*p == '\\') {
                state = ngx_json_surrogate_u;
                ଭାଙ୍ଗ;
            }

            /* high surrogate not followed by \uXXXX */
            ଫେରନ୍ତୁ NGX_DECLINED;

        ସ୍ଥିତି ngx_json_surrogate_u:
            ଯଦି (*p == 'u') {
                ctx->codepoint = 0;
                ctx->hex_ବାମ = 4;
                state = ngx_json_surrogate_hex;
                ଭାଙ୍ଗ;
            }

            /* high surrogate followed by non-\u escape */
            ଫେରନ୍ତୁ NGX_DECLINED;

        ସ୍ଥିତି ngx_json_surrogate_hex:

            /* accumulate the four hex digits of the low \uXXXX escape */

            hd = ngx_json_hex_digit(*p);
            ଯଦି (hd == NGX_ERROR) {
                ଫେରନ୍ତୁ NGX_DECLINED;
            }

            ctx->codepoint = (ctx->codepoint << 4) | (uint32_t) hd;

            ଯଦି (--ctx->hex_ବାମ > 0) {
                ଭାଙ୍ଗ;
            }

            ଯଦି (ctx->codepoint >= 0xDC00 && ctx->codepoint <= 0xDFFF) {
                /* valid surrogate pair */
                state = ngx_json_string;
                ଭାଙ୍ଗ;
            }

            /* high surrogate not followed by low surrogate */
            ଫେରନ୍ତୁ NGX_DECLINED;

        ସ୍ଥିତି ngx_json_ସଂଖ୍ୟା_minus:
            ଯଦି (*p >= '1' && *p <= '9') {
                state = ngx_json_ସଂଖ୍ୟା_int;
                ଭାଙ୍ଗ;
            }

            ଯଦି (*p == '0') {
                state = ngx_json_ସଂଖ୍ୟା_int_zero;
                ଭାଙ୍ଗ;
            }

            ଫେରନ୍ତୁ NGX_DECLINED;

        ସ୍ଥିତି ngx_json_ସଂଖ୍ୟା_int_zero:
            ଯଦି (*p == '.') {
                state = ngx_json_ସଂଖ୍ୟା_frac;
                ଭାଙ୍ଗ;
            }

            ଯଦି (*p == 'e' || *p == 'E') {
                /* No sense, but permitted by RFC */
                state = ngx_json_ସଂଖ୍ୟା_exp;
                ଭାଙ୍ଗ;
            }

            p--;

            rc = ngx_json_end_ସଂଖ୍ୟା(ctx, start, p + 1, &state);
            ଯଦି (rc != NGX_OK) {
                ଫେରନ୍ତୁ rc;
            }

            ଭାଙ୍ଗ;

        ସ୍ଥିତି ngx_json_ସଂଖ୍ୟା_int:
            ଯଦି (*p >= '0' && *p <= '9') {
                ଭାଙ୍ଗ;
            }

            ଯଦି (*p == '.') {
                state = ngx_json_ସଂଖ୍ୟା_frac;
                ଭାଙ୍ଗ;
            }

            ଯଦି (*p == 'e' || *p == 'E') {
                /* No sense, but permitted by RFC */
                state = ngx_json_ସଂଖ୍ୟା_exp;
                ଭାଙ୍ଗ;
            }

            p--;

            rc = ngx_json_end_ସଂଖ୍ୟା(ctx, start, p + 1, &state);
            ଯଦି (rc!= NGX_OK) {
                ଫେରନ୍ତୁ rc;
            }

            ଭାଙ୍ଗ;

        ସ୍ଥିତି ngx_json_ସଂଖ୍ୟା_frac:
            ଯଦି (*p >= '0' && *p <= '9') {
                state = ngx_json_ସଂଖ୍ୟା_frac_digit;
                ଭାଙ୍ଗ;
            }

            ଫେରନ୍ତୁ NGX_DECLINED;

        ସ୍ଥିତି ngx_json_ସଂଖ୍ୟା_frac_digit:
            ଯଦି (*p >= '0' && *p <= '9') {
                ଭାଙ୍ଗ;
            }

            ଯଦି (*p == 'e' || *p == 'E') {
                state = ngx_json_ସଂଖ୍ୟା_exp;
                ଭାଙ୍ଗ;
            }

            p--;

            rc = ngx_json_end_ସଂଖ୍ୟା(ctx, start, p + 1, &state);
            ଯଦି (rc != NGX_OK) {
                ଫେରନ୍ତୁ rc;
            }

            ଭାଙ୍ଗ;

        ସ୍ଥିତି ngx_json_ସଂଖ୍ୟା_exp:
            ଯଦି (*p == '-' || *p == '+') {
                state = ngx_json_ସଂଖ୍ୟା_exp_plusminus;
                ଭାଙ୍ଗ;
            }

            ଯଦି (*p >= '0' && *p <= '9') {
                state = ngx_json_ସଂଖ୍ୟା_exp_digit;
                ଭାଙ୍ଗ;
            }

            ଫେରନ୍ତୁ NGX_DECLINED;

        ସ୍ଥିତି ngx_json_ସଂଖ୍ୟା_exp_plusminus:
            ଯଦି (*p >= '0' && *p <= '9') {
                state = ngx_json_ସଂଖ୍ୟା_exp_digit;
                ଭାଙ୍ଗ;
            }

            ଫେରନ୍ତୁ NGX_DECLINED;

        ସ୍ଥିତି ngx_json_ସଂଖ୍ୟା_exp_digit:
            ଯଦି (*p >= '0' && *p <= '9') {
                ଭାଙ୍ଗ;
            }

            p--;

            rc = ngx_json_end_ସଂଖ୍ୟା(ctx, start, p + 1, &state);
            ଯଦି (rc != NGX_OK) {
                ଫେରନ୍ତୁ rc;
            }

            ଭାଙ୍ଗ;

        ସ୍ଥିତି ngx_json_true_t:
            ଯଦି (*p == 'r') {
                state = ngx_json_true_tr;
                ଭାଙ୍ଗ;
            }

            ଫେରନ୍ତୁ NGX_DECLINED;

        ସ୍ଥିତି ngx_json_true_tr:
            ଯଦି (*p == 'u') {
                state = ngx_json_true_tru;
                ଭାଙ୍ଗ;
            }

            ଫେରନ୍ତୁ NGX_DECLINED;

        ସ୍ଥିତି ngx_json_true_tru:
            ଯଦି (*p == 'e') {
                rc = ngx_json_emit(ctx, NGX_JSON_ମୂଲ୍ୟ_BOOL, start,
                                  p + 1 - start);
                ଯଦି (rc != NGX_OK) {
                    ଫେରନ୍ତୁ rc;
                }

                state = ngx_json_after_ମୂଲ୍ୟ(ctx);
                ଭାଙ୍ଗ;
            }

            ଫେରନ୍ତୁ NGX_DECLINED;

        ସ୍ଥିତି ngx_json_false_f:
            ଯଦି (*p == 'a') {
                state = ngx_json_false_fa;
                ଭାଙ୍ଗ;
            }

            ଫେରନ୍ତୁ NGX_DECLINED;

        ସ୍ଥିତି ngx_json_false_fa:
            ଯଦି (*p == 'l') {
                state = ngx_json_false_fal;
                ଭାଙ୍ଗ;
            }

            ଫେରନ୍ତୁ NGX_DECLINED;

        ସ୍ଥିତି ngx_json_false_fal:
            ଯଦି (*p == 's') {
                state = ngx_json_false_fals;
                ଭାଙ୍ଗ;
            }

            ଫେରନ୍ତୁ NGX_DECLINED;

        ସ୍ଥିତି ngx_json_false_fals:
            ଯଦି (*p == 'e') {
                rc = ngx_json_emit(ctx, NGX_JSON_ମୂଲ୍ୟ_BOOL, start,
                                  p + 1 - start);
                ଯଦି (rc != NGX_OK) {
                    ଫେରନ୍ତୁ rc;
                }

                state = ngx_json_after_ମୂଲ୍ୟ(ctx);
                ଭାଙ୍ଗ;
            }

            ଫେରନ୍ତୁ NGX_DECLINED;

        ସ୍ଥିତି ngx_json_null_n:
            ଯଦି (*p == 'u') {
                state = ngx_json_null_nu;
                ଭାଙ୍ଗ;
            }

            ଫେରନ୍ତୁ NGX_DECLINED;

        ସ୍ଥିତି ngx_json_null_nu:
            ଯଦି (*p == 'l') {
                state = ngx_json_null_nul;
                ଭାଙ୍ଗ;
            }

            ଫେରନ୍ତୁ NGX_DECLINED;

        ସ୍ଥିତି ngx_json_null_nul:
            ଯଦି (*p == 'l') {
                rc = ngx_json_emit(ctx, NGX_JSON_ମୂଲ୍ୟ_NULL, start,
                                  p + 1 - start);
                ଯଦି (rc != NGX_OK) {
                    ଫେରନ୍ତୁ rc;
                }

                state = ngx_json_after_ମୂଲ୍ୟ(ctx);
                ଭାଙ୍ଗ;
            }

            ଫେରନ୍ତୁ NGX_DECLINED;

        ସ୍ଥିତି ngx_json_done:
            ଯଦି (ngx_json_ws(*p)) {
                ଭାଙ୍ଗ;
            }

            ଫେରନ୍ତୁ NGX_DECLINED;
        }

        skip_depth = ctx->skip_until_depth;
    }

    /* a bare top-level number is not self-delimited; emit it at end of input */

    ଯଦି (ctx->depth == 0
        && (state == ngx_json_ସଂଖ୍ୟା_int_zero
            || state == ngx_json_ସଂଖ୍ୟା_int
            || state == ngx_json_ସଂଖ୍ୟା_frac_digit
            || state == ngx_json_ସଂଖ୍ୟା_exp_digit))
    {
        rc = ngx_json_emit(ctx, NGX_JSON_ମୂଲ୍ୟ_ସଂଖ୍ୟା, start, last - start);
        ଯଦି (rc != NGX_OK) {
            ଫେରନ୍ତୁ rc;
        }

        state = ngx_json_done;
    }

    ctx->state = state;

    ଯଦି (state == ngx_json_done) {
        ଫେରନ୍ତୁ NGX_OK;
    }

    ଫେରନ୍ତୁ NGX_DECLINED;
}


ngx_int_t
ngx_json_parse(ngx_pool_t *pool, ngx_str_t *json, ngx_json_handler_pt handler,
    ଶୂନ୍ୟ *data)
{
    ngx_json_ctx_t  jctx;

    ngx_json_ctx_init(&jctx, pool);

    jctx.data = data;
    jctx.handler = handler;

    ଫେରନ୍ତୁ ngx_json_parse_ctx(&jctx, json->data, json->len);
}


ସ୍ଥିର ngx_inline ngx_int_t
ngx_json_end_ସଂଖ୍ୟା(ngx_json_ctx_t *ctx, u_char *start, u_char *end,
    ngx_json_state_e *state)
{
    ngx_int_t  rc;

    rc = ngx_json_emit(ctx, NGX_JSON_ମୂଲ୍ୟ_ସଂଖ୍ୟା, start, end - start);
    ଯଦି (rc != NGX_OK) {
        ଫେରନ୍ତୁ rc;
    }

    *state = ngx_json_after_ମୂଲ୍ୟ(ctx);

    ଫେରନ୍ତୁ NGX_OK;
}


ସ୍ଥିର ngx_inline ngx_int_t
ngx_json_emit(ngx_json_ctx_t *ctx, ngx_json_event_e event, u_char *data,
    size_t len)
{
    ngx_int_t  rc;
    ngx_str_t  token;

    ଯଦି (ngx_json_skipping(ctx->skip_until_depth)) {
        ଫେରନ୍ତୁ NGX_OK;
    }

    ଯଦି (ctx->handler == ଖାଲି) {
        ଫେରନ୍ତୁ NGX_OK;
    }

    token.data = data;
    token.len = len;
    rc = ctx->handler(ctx, event, &token);

    ଯଦି (rc == NGX_JSON_SKIP) {

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

        ବାଛ (event) {

        ସ୍ଥିତି NGX_JSON_ଚାବି:
        ସ୍ଥିତି NGX_JSON_ଚାବି_ESCAPED:
        ସ୍ଥିତି NGX_JSON_OBJECT_OPEN:
        ସ୍ଥିତି NGX_JSON_ARRAY_OPEN:
            ctx->skip_until_depth = ctx->depth;
            ଭାଙ୍ଗ;

        ସ୍ଥିତି NGX_JSON_ମୂଲ୍ୟ_STRING:
        ସ୍ଥିତି NGX_JSON_ମୂଲ୍ୟ_STRING_ESCAPED:
        ସ୍ଥିତି NGX_JSON_ମୂଲ୍ୟ_ସଂଖ୍ୟା:
        ସ୍ଥିତି NGX_JSON_ମୂଲ୍ୟ_BOOL:
        ସ୍ଥିତି NGX_JSON_ମୂଲ୍ୟ_NULL:
            ଯଦି (ctx->depth > 0
                && ctx->ଗଦା[ctx->depth - 1] == ngx_json_ctx_array
            ) {
                ctx->skip_until_depth = ctx->depth - 1;
            }
            ଭାଙ୍ଗ;

        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
            ଭାଙ୍ଗ;
        }

        ଫେରନ୍ତୁ NGX_OK;
    }

    ଫେରନ୍ତୁ rc;
}


ସ୍ଥିର ngx_inline ngx_json_state_e
ngx_json_after_ମୂଲ୍ୟ(ngx_json_ctx_t *ctx)
{
    ଯଦି (ctx->depth == 0) {
        ଫେରନ୍ତୁ ngx_json_done;
    }

    ଯଦି (ctx->ଗଦା[ctx->depth - 1] == ngx_json_ctx_object) {
        ଫେରନ୍ତୁ ngx_json_object_ମୂଲ୍ୟ_separator;
    }

    ଫେରନ୍ତୁ ngx_json_array_ମୂଲ୍ୟ_separator;
}


ସ୍ଥିର ngx_inline ngx_int_t
ngx_json_close(ngx_json_ctx_t *ctx, ngx_json_event_e event,
    ngx_json_state_e *state, u_char *p)
{
    ngx_int_t  rc;

    ctx->depth--;

    rc = ngx_json_emit(ctx, event, p, 1);
    ଯଦି (rc != NGX_OK) {
        ଫେରନ୍ତୁ rc;
    }

    *state = ngx_json_after_ମୂଲ୍ୟ(ctx);

    ଫେରନ୍ତୁ NGX_OK;
}


ସ୍ଥିର ngx_int_t
ngx_json_ଠେଲ_state(ngx_json_ctx_t *ctx, ngx_json_container_e container)
{
    ngx_json_container_e  *ଗଦା;

    ଯଦି (ctx->depth >= ctx->max_depth) {
        ଫେରନ୍ତୁ NGX_DECLINED;
    }

    ଯଦି (ctx->ଗଦା == ଖାଲି) {
        ଗଦା = ngx_palloc(ctx->pool,
                           ctx->max_depth * ଆକାର(ngx_json_container_e));
        ଯଦି (ଗଦା == ଖାଲି) {
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        ctx->ଗଦା = ଗଦା;
    }

    ctx->ଗଦା[ctx->depth] = container;
    ctx->depth++;

    ଫେରନ୍ତୁ NGX_OK;
}
