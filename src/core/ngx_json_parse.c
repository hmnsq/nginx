
/*
 * Copyright (C) Maxim Dounin
 * Copyright (C) Nginx, Inc.
 */


/*
 * JSON parser. RFC 8259 for the JSON grammar.

 * Site http://json.org/ may be useful too.
 */


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>
#అన్తర్భావ <ngx_json_parse.h>


#నిర్వచించు NGX_JSON_NOT_SKIPPING  (ngx_uint_t) -1
#నిర్వచించు ngx_json_skipping(d)   ((d) != NGX_JSON_NOT_SKIPPING)
#నిర్వచించు ngx_json_ws(c)  ((c) == ' ' || (c) == '\t' || (c) == CR || (c) == LF)


స్థిర ngx_inline ngx_int_t ngx_json_చివర_సంఖ్య(ngx_json_ctx_t *ctx,
    u_char *ప్రారంభం, u_char *చివర, ngx_json_state_e *state);
స్థిర ngx_inline ngx_int_t ngx_json_emit(ngx_json_ctx_t *ctx,
    ngx_json_event_e event, u_char *దత్తాంశం, కొలత_t len);
స్థిర ngx_inline ngx_json_state_e ngx_json_after_విలువ(ngx_json_ctx_t *ctx);
స్థిర ngx_inline ngx_int_t ngx_json_close(ngx_json_ctx_t *ctx,
    ngx_json_event_e event, ngx_json_state_e *state, u_char *p);
స్థిర ngx_int_t ngx_json_నెట్టు_state(ngx_json_ctx_t *ctx,
    ngx_json_container_e container);


శూన్య
ngx_json_ctx_init(ngx_json_ctx_t *ctx, ngx_pool_t *pool)
{
    ctx->state = ngx_json_ప్రారంభం;
    ctx->pool = pool;
    ctx->handler = ఖాళీ;
    ctx->దత్తాంశం = ఖాళీ;
    ctx->codepoint = 0;
    ctx->hex_ఎడమ = 0;

    ctx->లోతు = 0;
    ctx->skip_until_లోతు = NGX_JSON_NOT_SKIPPING;
    ctx->max_లోతు = NGX_JSON_DEFAULT_MAX_లోతు;

    /* stack is allocated lazily on first push, sized to max_depth */

    ctx->దొంతర = ఖాళీ;

    ctx->లోపల_కీలకం = 0;
    ctx->escaped = 0;
}


ngx_int_t
ngx_json_parse_ctx(ngx_json_ctx_t *ctx, u_char *దత్తాంశం, కొలత_t len)
{
    u_char            *p, *చివరి, *ప్రారంభం;
    ngx_int_t          rc, hd;
    ngx_uint_t         skip_లోతు;
    ngx_json_event_e   event;
    ngx_json_state_e   state;

    ఒకవేళ (len == 0) {
        తిరిగి NGX_DECLINED;
    }

    ఒకవేళ (దత్తాంశం == ఖాళీ) {
        తిరిగి NGX_ERROR;
    }

    ఒకవేళ (ctx->state == ngx_json_పూర్తి) {
        తిరిగి NGX_DECLINED;
    }

    చివరి = దత్తాంశం + len;
    state = ctx->state;
    skip_లోతు = ctx->skip_until_లోతు;
    ప్రారంభం = ఖాళీ;

    కొరకు (p = దత్తాంశం; p < చివరి; p++) {

        /* skipping suppresses emission only; the machine still validates */

        ఒకవేళ (ngx_json_skipping(skip_లోతు) && ctx->లోతు <= skip_లోతు
            && (state == ngx_json_object_విలువ_separator
                || state == ngx_json_array_విలువ_separator
                || state == ngx_json_పూర్తి))
        {
            skip_లోతు = NGX_JSON_NOT_SKIPPING;
            ctx->skip_until_లోతు = NGX_JSON_NOT_SKIPPING;
            p--;

            కొనసాగు;
        }

        ఎంపిక (state) {

        స్థితి ngx_json_ప్రారంభం:
            ఒకవేళ (ngx_json_ws(*p)) {
                ఆపు;
            }

            p--;
            state = ngx_json_విలువ;
            ఆపు;

        స్థితి ngx_json_object:
            ఒకవేళ (ngx_json_ws(*p)) {
                ఆపు;
            }

            ఒకవేళ (*p == '}') {
                rc = ngx_json_close(ctx, NGX_JSON_OBJECT_CLOSE, &state, p);
                ఒకవేళ (rc != NGX_OK) {
                    తిరిగి rc;
                }

                ఆపు;
            }

            /* fall through */

        స్థితి ngx_json_object_తదుపరి:
            ఒకవేళ (ngx_json_ws(*p)) {
                ఆపు;
            }

            ఒకవేళ (*p == '"') {
                ctx->లోపల_కీలకం = 1;
                ctx->escaped = 0;
                ప్రారంభం = p + 1;
                state = ngx_json_string;
                ఆపు;
            }

            తిరిగి NGX_DECLINED;

        స్థితి ngx_json_object_name_separator:
            ఒకవేళ (ngx_json_ws(*p)) {
                ఆపు;
            }

            ఒకవేళ (*p == ':') {
                state = ngx_json_విలువ;
                ఆపు;
            }

            తిరిగి NGX_DECLINED;

        స్థితి ngx_json_object_విలువ_separator:
            ఒకవేళ (ngx_json_ws(*p)) {
                ఆపు;
            }

            ఒకవేళ (*p == ',') {
                state = ngx_json_object_తదుపరి;
                ఆపు;
            }

            ఒకవేళ (*p == '}') {
                rc = ngx_json_close(ctx, NGX_JSON_OBJECT_CLOSE, &state, p);
                ఒకవేళ (rc != NGX_OK) {
                    తిరిగి rc;
                }

                ఆపు;
            }

            తిరిగి NGX_DECLINED;

        స్థితి ngx_json_array_విలువ_separator:
            ఒకవేళ (ngx_json_ws(*p)) {
                ఆపు;
            }

            ఒకవేళ (*p == ',') {
                state = ngx_json_విలువ;
                ఆపు;
            }

            ఒకవేళ (*p == ']') {
                rc = ngx_json_close(ctx, NGX_JSON_ARRAY_CLOSE, &state, p);
                ఒకవేళ (rc != NGX_OK) {
                    తిరిగి rc;
                }

                ఆపు;
            }

            తిరిగి NGX_DECLINED;

        స్థితి ngx_json_array:
            ఒకవేళ (ngx_json_ws(*p)) {
                ఆపు;
            }

            ఒకవేళ (*p == ']') {
                rc = ngx_json_close(ctx, NGX_JSON_ARRAY_CLOSE, &state, p);
                ఒకవేళ (rc != NGX_OK) {
                    తిరిగి rc;
                }

                ఆపు;
            }

            /* fall through */

        స్థితి ngx_json_విలువ:
            ఒకవేళ (ngx_json_ws(*p)) {
                ఆపు;
            }

            ప్రారంభం = p;

            ఒకవేళ (*p == '"') {
                ctx->లోపల_కీలకం = 0;
                ctx->escaped = 0;
                ప్రారంభం = p + 1;
                state = ngx_json_string;
                ఆపు;
            }

            ఒకవేళ (*p == '{') {
                rc = ngx_json_emit(ctx, NGX_JSON_OBJECT_OPEN, p, 1);
                ఒకవేళ (rc != NGX_OK) {
                    తిరిగి rc;
                }

                rc = ngx_json_నెట్టు_state(ctx, ngx_json_ctx_object);
                ఒకవేళ (rc != NGX_OK) {
                    తిరిగి rc;
                }

                state = ngx_json_object;
                ఆపు;
            }

            ఒకవేళ (*p == '[') {
                rc = ngx_json_emit(ctx, NGX_JSON_ARRAY_OPEN, p, 1);
                ఒకవేళ (rc != NGX_OK) {
                    తిరిగి rc;
                }

                rc = ngx_json_నెట్టు_state(ctx, ngx_json_ctx_array);
                ఒకవేళ (rc != NGX_OK) {
                    తిరిగి rc;
                }

                state = ngx_json_array;
                ఆపు;
            }

            ఒకవేళ (*p == '-') {
                state = ngx_json_సంఖ్య_minus;
                ఆపు;
            }

            ఒకవేళ (*p == '0') {
                state = ngx_json_సంఖ్య_int_zero;
                ఆపు;
            }

            ఒకవేళ (*p >= '1' && *p <= '9') {
                state = ngx_json_సంఖ్య_int;
                ఆపు;
            }

            ఒకవేళ (*p == 't') {
                state = ngx_json_true_t;
                ఆపు;
            }

            ఒకవేళ (*p == 'f') {
                state = ngx_json_false_f;
                ఆపు;
            }

            ఒకవేళ (*p == 'n') {
                state = ngx_json_null_n;
                ఆపు;
            }

            తిరిగి NGX_DECLINED;

        స్థితి ngx_json_string:
            ఒకవేళ (*p == '"') {

                ఒకవేళ (ctx->లోపల_కీలకం) {
                    event = ctx->escaped ? NGX_JSON_కీలకం_ESCAPED
                                         : NGX_JSON_కీలకం;

                    rc = ngx_json_emit(ctx, event, ప్రారంభం, p - ప్రారంభం);
                    ఒకవేళ (rc != NGX_OK) {
                        తిరిగి rc;
                    }

                    ctx->లోపల_కీలకం = 0;
                    state = ngx_json_object_name_separator;

                } లేకపోతే {
                    event = ctx->escaped ? NGX_JSON_విలువ_STRING_ESCAPED
                                         : NGX_JSON_విలువ_STRING;

                    rc = ngx_json_emit(ctx, event, ప్రారంభం, p - ప్రారంభం);
                    ఒకవేళ (rc != NGX_OK) {
                        తిరిగి rc;
                    }

                    state = ngx_json_after_విలువ(ctx);
                }

                ఆపు;
            }

            ఒకవేళ (*p == '\\') {
                ctx->escaped = 1;
                state = ngx_json_escaped;
                ఆపు;
            }

            ఒకవేళ (*p < ' ') {
                తిరిగి NGX_DECLINED;
            }

            ఆపు;

        స్థితి ngx_json_escaped:
            ఒకవేళ (*p == '"' || *p == '\\' || *p == '/' || *p == 'b'
                || *p == 'f' || *p == 'n' || *p == 'r' || *p == 't')
            {
                state = ngx_json_string;
                ఆపు;
            }

            ఒకవేళ (*p == 'u') {
                ctx->codepoint = 0;
                ctx->hex_ఎడమ = 4;
                state = ngx_json_escaped_hex;
                ఆపు;
            }

            తిరిగి NGX_DECLINED;

        స్థితి ngx_json_escaped_hex:

            /* accumulate the four hex digits of the first \uXXXX escape */

            hd = ngx_json_hex_digit(*p);
            ఒకవేళ (hd == NGX_ERROR) {
                తిరిగి NGX_DECLINED;
            }

            ctx->codepoint = (ctx->codepoint << 4) | (uint32_t) hd;

            ఒకవేళ (--ctx->hex_ఎడమ > 0) {
                ఆపు;
            }

            ఒకవేళ (ctx->codepoint >= 0xD800 && ctx->codepoint <= 0xDBFF) {
                /* high surrogate - must be followed by \uDC00..\uDFFF */
                state = ngx_json_surrogate_ప్రారంభం;
                ఆపు;
            }

            ఒకవేళ (ctx->codepoint >= 0xDC00 && ctx->codepoint <= 0xDFFF) {
                /* lone low surrogate */
                తిరిగి NGX_DECLINED;
            }

            state = ngx_json_string;
            ఆపు;

        స్థితి ngx_json_surrogate_ప్రారంభం:
            ఒకవేళ (*p == '\\') {
                state = ngx_json_surrogate_u;
                ఆపు;
            }

            /* high surrogate not followed by \uXXXX */
            తిరిగి NGX_DECLINED;

        స్థితి ngx_json_surrogate_u:
            ఒకవేళ (*p == 'u') {
                ctx->codepoint = 0;
                ctx->hex_ఎడమ = 4;
                state = ngx_json_surrogate_hex;
                ఆపు;
            }

            /* high surrogate followed by non-\u escape */
            తిరిగి NGX_DECLINED;

        స్థితి ngx_json_surrogate_hex:

            /* accumulate the four hex digits of the low \uXXXX escape */

            hd = ngx_json_hex_digit(*p);
            ఒకవేళ (hd == NGX_ERROR) {
                తిరిగి NGX_DECLINED;
            }

            ctx->codepoint = (ctx->codepoint << 4) | (uint32_t) hd;

            ఒకవేళ (--ctx->hex_ఎడమ > 0) {
                ఆపు;
            }

            ఒకవేళ (ctx->codepoint >= 0xDC00 && ctx->codepoint <= 0xDFFF) {
                /* valid surrogate pair */
                state = ngx_json_string;
                ఆపు;
            }

            /* high surrogate not followed by low surrogate */
            తిరిగి NGX_DECLINED;

        స్థితి ngx_json_సంఖ్య_minus:
            ఒకవేళ (*p >= '1' && *p <= '9') {
                state = ngx_json_సంఖ్య_int;
                ఆపు;
            }

            ఒకవేళ (*p == '0') {
                state = ngx_json_సంఖ్య_int_zero;
                ఆపు;
            }

            తిరిగి NGX_DECLINED;

        స్థితి ngx_json_సంఖ్య_int_zero:
            ఒకవేళ (*p == '.') {
                state = ngx_json_సంఖ్య_frac;
                ఆపు;
            }

            ఒకవేళ (*p == 'e' || *p == 'E') {
                /* No sense, but permitted by RFC */
                state = ngx_json_సంఖ్య_exp;
                ఆపు;
            }

            p--;

            rc = ngx_json_చివర_సంఖ్య(ctx, ప్రారంభం, p + 1, &state);
            ఒకవేళ (rc != NGX_OK) {
                తిరిగి rc;
            }

            ఆపు;

        స్థితి ngx_json_సంఖ్య_int:
            ఒకవేళ (*p >= '0' && *p <= '9') {
                ఆపు;
            }

            ఒకవేళ (*p == '.') {
                state = ngx_json_సంఖ్య_frac;
                ఆపు;
            }

            ఒకవేళ (*p == 'e' || *p == 'E') {
                /* No sense, but permitted by RFC */
                state = ngx_json_సంఖ్య_exp;
                ఆపు;
            }

            p--;

            rc = ngx_json_చివర_సంఖ్య(ctx, ప్రారంభం, p + 1, &state);
            ఒకవేళ (rc!= NGX_OK) {
                తిరిగి rc;
            }

            ఆపు;

        స్థితి ngx_json_సంఖ్య_frac:
            ఒకవేళ (*p >= '0' && *p <= '9') {
                state = ngx_json_సంఖ్య_frac_digit;
                ఆపు;
            }

            తిరిగి NGX_DECLINED;

        స్థితి ngx_json_సంఖ్య_frac_digit:
            ఒకవేళ (*p >= '0' && *p <= '9') {
                ఆపు;
            }

            ఒకవేళ (*p == 'e' || *p == 'E') {
                state = ngx_json_సంఖ్య_exp;
                ఆపు;
            }

            p--;

            rc = ngx_json_చివర_సంఖ్య(ctx, ప్రారంభం, p + 1, &state);
            ఒకవేళ (rc != NGX_OK) {
                తిరిగి rc;
            }

            ఆపు;

        స్థితి ngx_json_సంఖ్య_exp:
            ఒకవేళ (*p == '-' || *p == '+') {
                state = ngx_json_సంఖ్య_exp_plusminus;
                ఆపు;
            }

            ఒకవేళ (*p >= '0' && *p <= '9') {
                state = ngx_json_సంఖ్య_exp_digit;
                ఆపు;
            }

            తిరిగి NGX_DECLINED;

        స్థితి ngx_json_సంఖ్య_exp_plusminus:
            ఒకవేళ (*p >= '0' && *p <= '9') {
                state = ngx_json_సంఖ్య_exp_digit;
                ఆపు;
            }

            తిరిగి NGX_DECLINED;

        స్థితి ngx_json_సంఖ్య_exp_digit:
            ఒకవేళ (*p >= '0' && *p <= '9') {
                ఆపు;
            }

            p--;

            rc = ngx_json_చివర_సంఖ్య(ctx, ప్రారంభం, p + 1, &state);
            ఒకవేళ (rc != NGX_OK) {
                తిరిగి rc;
            }

            ఆపు;

        స్థితి ngx_json_true_t:
            ఒకవేళ (*p == 'r') {
                state = ngx_json_true_tr;
                ఆపు;
            }

            తిరిగి NGX_DECLINED;

        స్థితి ngx_json_true_tr:
            ఒకవేళ (*p == 'u') {
                state = ngx_json_true_tru;
                ఆపు;
            }

            తిరిగి NGX_DECLINED;

        స్థితి ngx_json_true_tru:
            ఒకవేళ (*p == 'e') {
                rc = ngx_json_emit(ctx, NGX_JSON_విలువ_BOOL, ప్రారంభం,
                                  p + 1 - ప్రారంభం);
                ఒకవేళ (rc != NGX_OK) {
                    తిరిగి rc;
                }

                state = ngx_json_after_విలువ(ctx);
                ఆపు;
            }

            తిరిగి NGX_DECLINED;

        స్థితి ngx_json_false_f:
            ఒకవేళ (*p == 'a') {
                state = ngx_json_false_fa;
                ఆపు;
            }

            తిరిగి NGX_DECLINED;

        స్థితి ngx_json_false_fa:
            ఒకవేళ (*p == 'l') {
                state = ngx_json_false_fal;
                ఆపు;
            }

            తిరిగి NGX_DECLINED;

        స్థితి ngx_json_false_fal:
            ఒకవేళ (*p == 's') {
                state = ngx_json_false_fals;
                ఆపు;
            }

            తిరిగి NGX_DECLINED;

        స్థితి ngx_json_false_fals:
            ఒకవేళ (*p == 'e') {
                rc = ngx_json_emit(ctx, NGX_JSON_విలువ_BOOL, ప్రారంభం,
                                  p + 1 - ప్రారంభం);
                ఒకవేళ (rc != NGX_OK) {
                    తిరిగి rc;
                }

                state = ngx_json_after_విలువ(ctx);
                ఆపు;
            }

            తిరిగి NGX_DECLINED;

        స్థితి ngx_json_null_n:
            ఒకవేళ (*p == 'u') {
                state = ngx_json_null_nu;
                ఆపు;
            }

            తిరిగి NGX_DECLINED;

        స్థితి ngx_json_null_nu:
            ఒకవేళ (*p == 'l') {
                state = ngx_json_null_nul;
                ఆపు;
            }

            తిరిగి NGX_DECLINED;

        స్థితి ngx_json_null_nul:
            ఒకవేళ (*p == 'l') {
                rc = ngx_json_emit(ctx, NGX_JSON_విలువ_NULL, ప్రారంభం,
                                  p + 1 - ప్రారంభం);
                ఒకవేళ (rc != NGX_OK) {
                    తిరిగి rc;
                }

                state = ngx_json_after_విలువ(ctx);
                ఆపు;
            }

            తిరిగి NGX_DECLINED;

        స్థితి ngx_json_పూర్తి:
            ఒకవేళ (ngx_json_ws(*p)) {
                ఆపు;
            }

            తిరిగి NGX_DECLINED;
        }

        skip_లోతు = ctx->skip_until_లోతు;
    }

    /* a bare top-level number is not self-delimited; emit it at end of input */

    ఒకవేళ (ctx->లోతు == 0
        && (state == ngx_json_సంఖ్య_int_zero
            || state == ngx_json_సంఖ్య_int
            || state == ngx_json_సంఖ్య_frac_digit
            || state == ngx_json_సంఖ్య_exp_digit))
    {
        rc = ngx_json_emit(ctx, NGX_JSON_విలువ_సంఖ్య, ప్రారంభం, చివరి - ప్రారంభం);
        ఒకవేళ (rc != NGX_OK) {
            తిరిగి rc;
        }

        state = ngx_json_పూర్తి;
    }

    ctx->state = state;

    ఒకవేళ (state == ngx_json_పూర్తి) {
        తిరిగి NGX_OK;
    }

    తిరిగి NGX_DECLINED;
}


ngx_int_t
ngx_json_parse(ngx_pool_t *pool, ngx_str_t *json, ngx_json_handler_pt handler,
    శూన్య *దత్తాంశం)
{
    ngx_json_ctx_t  jctx;

    ngx_json_ctx_init(&jctx, pool);

    jctx.దత్తాంశం = దత్తాంశం;
    jctx.handler = handler;

    తిరిగి ngx_json_parse_ctx(&jctx, json->దత్తాంశం, json->len);
}


స్థిర ngx_inline ngx_int_t
ngx_json_చివర_సంఖ్య(ngx_json_ctx_t *ctx, u_char *ప్రారంభం, u_char *చివర,
    ngx_json_state_e *state)
{
    ngx_int_t  rc;

    rc = ngx_json_emit(ctx, NGX_JSON_విలువ_సంఖ్య, ప్రారంభం, చివర - ప్రారంభం);
    ఒకవేళ (rc != NGX_OK) {
        తిరిగి rc;
    }

    *state = ngx_json_after_విలువ(ctx);

    తిరిగి NGX_OK;
}


స్థిర ngx_inline ngx_int_t
ngx_json_emit(ngx_json_ctx_t *ctx, ngx_json_event_e event, u_char *దత్తాంశం,
    కొలత_t len)
{
    ngx_int_t  rc;
    ngx_str_t  token;

    ఒకవేళ (ngx_json_skipping(ctx->skip_until_లోతు)) {
        తిరిగి NGX_OK;
    }

    ఒకవేళ (ctx->handler == ఖాళీ) {
        తిరిగి NGX_OK;
    }

    token.దత్తాంశం = దత్తాంశం;
    token.len = len;
    rc = ctx->handler(ctx, event, &token);

    ఒకవేళ (rc == NGX_JSON_SKIP) {

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

        ఎంపిక (event) {

        స్థితి NGX_JSON_కీలకం:
        స్థితి NGX_JSON_కీలకం_ESCAPED:
        స్థితి NGX_JSON_OBJECT_OPEN:
        స్థితి NGX_JSON_ARRAY_OPEN:
            ctx->skip_until_లోతు = ctx->లోతు;
            ఆపు;

        స్థితి NGX_JSON_విలువ_STRING:
        స్థితి NGX_JSON_విలువ_STRING_ESCAPED:
        స్థితి NGX_JSON_విలువ_సంఖ్య:
        స్థితి NGX_JSON_విలువ_BOOL:
        స్థితి NGX_JSON_విలువ_NULL:
            ఒకవేళ (ctx->లోతు > 0
                && ctx->దొంతర[ctx->లోతు - 1] == ngx_json_ctx_array
            ) {
                ctx->skip_until_లోతు = ctx->లోతు - 1;
            }
            ఆపు;

        అప్రమేయ:
            ఆపు;
        }

        తిరిగి NGX_OK;
    }

    తిరిగి rc;
}


స్థిర ngx_inline ngx_json_state_e
ngx_json_after_విలువ(ngx_json_ctx_t *ctx)
{
    ఒకవేళ (ctx->లోతు == 0) {
        తిరిగి ngx_json_పూర్తి;
    }

    ఒకవేళ (ctx->దొంతర[ctx->లోతు - 1] == ngx_json_ctx_object) {
        తిరిగి ngx_json_object_విలువ_separator;
    }

    తిరిగి ngx_json_array_విలువ_separator;
}


స్థిర ngx_inline ngx_int_t
ngx_json_close(ngx_json_ctx_t *ctx, ngx_json_event_e event,
    ngx_json_state_e *state, u_char *p)
{
    ngx_int_t  rc;

    ctx->లోతు--;

    rc = ngx_json_emit(ctx, event, p, 1);
    ఒకవేళ (rc != NGX_OK) {
        తిరిగి rc;
    }

    *state = ngx_json_after_విలువ(ctx);

    తిరిగి NGX_OK;
}


స్థిర ngx_int_t
ngx_json_నెట్టు_state(ngx_json_ctx_t *ctx, ngx_json_container_e container)
{
    ngx_json_container_e  *దొంతర;

    ఒకవేళ (ctx->లోతు >= ctx->max_లోతు) {
        తిరిగి NGX_DECLINED;
    }

    ఒకవేళ (ctx->దొంతర == ఖాళీ) {
        దొంతర = ngx_palloc(ctx->pool,
                           ctx->max_లోతు * పరిమాణం(ngx_json_container_e));
        ఒకవేళ (దొంతర == ఖాళీ) {
            తిరిగి NGX_ERROR;
        }

        ctx->దొంతర = దొంతర;
    }

    ctx->దొంతర[ctx->లోతు] = container;
    ctx->లోతు++;

    తిరిగి NGX_OK;
}
