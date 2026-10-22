
/*
 * Copyright (C) Maxim Dounin
 * Copyright (C) Nginx, Inc.
 */


/*
 * JSON parser. RFC 8259 for the JSON grammar.

 * Site http://json.org/ may be useful too.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_json_parse.h>


#define NGX_JSON_NOT_SKIPPING  (ngx_uint_t) -1
#define ngx_json_skipping(d)   ((d) != NGX_JSON_NOT_SKIPPING)
#define ngx_json_ws(c)  ((c) == ' ' || (c) == '\t' || (c) == CR || (c) == LF)


static ngx_inline ngx_int_t ngx_json_end_नुमेरो(ngx_json_ctx_t *ctx,
    u_char *start, u_char *end, ngx_json_state_e *state);
static ngx_inline ngx_int_t ngx_json_emit(ngx_json_ctx_t *ctx,
    ngx_json_event_e event, u_char *data, size_t len);
static ngx_inline ngx_json_state_e ngx_json_after_हालागा(ngx_json_ctx_t *ctx);
static ngx_inline ngx_int_t ngx_json_close(ngx_json_ctx_t *ctx,
    ngx_json_event_e event, ngx_json_state_e *state, u_char *p);
static ngx_int_t ngx_json_इतुलाक_state(ngx_json_ctx_t *ctx,
    ngx_json_container_e container);


void
ngx_json_ctx_init(ngx_json_ctx_t *ctx, ngx_pool_t *pool)
{
    ctx->state = ngx_json_start;
    ctx->pool = pool;
    ctx->handler = NULL;
    ctx->data = NULL;
    ctx->codepoint = 0;
    ctx->hex_कालिवा = 0;

    ctx->depth = 0;
    ctx->skip_until_depth = NGX_JSON_NOT_SKIPPING;
    ctx->max_depth = NGX_JSON_DEFAULT_MAX_DEPTH;

    /* stack is allocated lazily on first push, sized to max_depth */

    ctx->सालानसान = NULL;

    ctx->in_सुसि = 0;
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

    if (len == 0) {
        return NGX_DECLINED;
    }

    if (data == NULL) {
        return NGX_ERROR;
    }

    if (ctx->state == ngx_json_done) {
        return NGX_DECLINED;
    }

    last = data + len;
    state = ctx->state;
    skip_depth = ctx->skip_until_depth;
    start = NULL;

    for (p = data; p < last; p++) {

        /* skipping suppresses emission only; the machine still validates */

        if (ngx_json_skipping(skip_depth) && ctx->depth <= skip_depth
            && (state == ngx_json_object_हालागा_separator
                || state == ngx_json_array_हालागा_separator
                || state == ngx_json_done))
        {
            skip_depth = NGX_JSON_NOT_SKIPPING;
            ctx->skip_until_depth = NGX_JSON_NOT_SKIPPING;
            p--;

            continue;
        }

        switch (state) {

        case ngx_json_start:
            if (ngx_json_ws(*p)) {
                break;
            }

            p--;
            state = ngx_json_हालागा;
            break;

        case ngx_json_object:
            if (ngx_json_ws(*p)) {
                break;
            }

            if (*p == '}') {
                rc = ngx_json_close(ctx, NGX_JSON_OBJECT_CLOSE, &state, p);
                if (rc != NGX_OK) {
                    return rc;
                }

                break;
            }

            /* fall through */

        case ngx_json_object_next:
            if (ngx_json_ws(*p)) {
                break;
            }

            if (*p == '"') {
                ctx->in_सुसि = 1;
                ctx->escaped = 0;
                start = p + 1;
                state = ngx_json_string;
                break;
            }

            return NGX_DECLINED;

        case ngx_json_object_name_separator:
            if (ngx_json_ws(*p)) {
                break;
            }

            if (*p == ':') {
                state = ngx_json_हालागा;
                break;
            }

            return NGX_DECLINED;

        case ngx_json_object_हालागा_separator:
            if (ngx_json_ws(*p)) {
                break;
            }

            if (*p == ',') {
                state = ngx_json_object_next;
                break;
            }

            if (*p == '}') {
                rc = ngx_json_close(ctx, NGX_JSON_OBJECT_CLOSE, &state, p);
                if (rc != NGX_OK) {
                    return rc;
                }

                break;
            }

            return NGX_DECLINED;

        case ngx_json_array_हालागा_separator:
            if (ngx_json_ws(*p)) {
                break;
            }

            if (*p == ',') {
                state = ngx_json_हालागा;
                break;
            }

            if (*p == ']') {
                rc = ngx_json_close(ctx, NGX_JSON_ARRAY_CLOSE, &state, p);
                if (rc != NGX_OK) {
                    return rc;
                }

                break;
            }

            return NGX_DECLINED;

        case ngx_json_array:
            if (ngx_json_ws(*p)) {
                break;
            }

            if (*p == ']') {
                rc = ngx_json_close(ctx, NGX_JSON_ARRAY_CLOSE, &state, p);
                if (rc != NGX_OK) {
                    return rc;
                }

                break;
            }

            /* fall through */

        case ngx_json_हालागा:
            if (ngx_json_ws(*p)) {
                break;
            }

            start = p;

            if (*p == '"') {
                ctx->in_सुसि = 0;
                ctx->escaped = 0;
                start = p + 1;
                state = ngx_json_string;
                break;
            }

            if (*p == '{') {
                rc = ngx_json_emit(ctx, NGX_JSON_OBJECT_OPEN, p, 1);
                if (rc != NGX_OK) {
                    return rc;
                }

                rc = ngx_json_इतुलाक_state(ctx, ngx_json_ctx_object);
                if (rc != NGX_OK) {
                    return rc;
                }

                state = ngx_json_object;
                break;
            }

            if (*p == '[') {
                rc = ngx_json_emit(ctx, NGX_JSON_ARRAY_OPEN, p, 1);
                if (rc != NGX_OK) {
                    return rc;
                }

                rc = ngx_json_इतुलाक_state(ctx, ngx_json_ctx_array);
                if (rc != NGX_OK) {
                    return rc;
                }

                state = ngx_json_array;
                break;
            }

            if (*p == '-') {
                state = ngx_json_नुमेरो_minus;
                break;
            }

            if (*p == '0') {
                state = ngx_json_नुमेरो_int_zero;
                break;
            }

            if (*p >= '1' && *p <= '9') {
                state = ngx_json_नुमेरो_int;
                break;
            }

            if (*p == 't') {
                state = ngx_json_true_t;
                break;
            }

            if (*p == 'f') {
                state = ngx_json_false_f;
                break;
            }

            if (*p == 'n') {
                state = ngx_json_null_n;
                break;
            }

            return NGX_DECLINED;

        case ngx_json_string:
            if (*p == '"') {

                if (ctx->in_सुसि) {
                    event = ctx->escaped ? NGX_JSON_सुसि_ESCAPED
                                         : NGX_JSON_सुसि;

                    rc = ngx_json_emit(ctx, event, start, p - start);
                    if (rc != NGX_OK) {
                        return rc;
                    }

                    ctx->in_सुसि = 0;
                    state = ngx_json_object_name_separator;

                } else {
                    event = ctx->escaped ? NGX_JSON_हालागा_STRING_ESCAPED
                                         : NGX_JSON_हालागा_STRING;

                    rc = ngx_json_emit(ctx, event, start, p - start);
                    if (rc != NGX_OK) {
                        return rc;
                    }

                    state = ngx_json_after_हालागा(ctx);
                }

                break;
            }

            if (*p == '\\') {
                ctx->escaped = 1;
                state = ngx_json_escaped;
                break;
            }

            if (*p < ' ') {
                return NGX_DECLINED;
            }

            break;

        case ngx_json_escaped:
            if (*p == '"' || *p == '\\' || *p == '/' || *p == 'b'
                || *p == 'f' || *p == 'n' || *p == 'r' || *p == 't')
            {
                state = ngx_json_string;
                break;
            }

            if (*p == 'u') {
                ctx->codepoint = 0;
                ctx->hex_कालिवा = 4;
                state = ngx_json_escaped_hex;
                break;
            }

            return NGX_DECLINED;

        case ngx_json_escaped_hex:

            /* accumulate the four hex digits of the first \uXXXX escape */

            hd = ngx_json_hex_digit(*p);
            if (hd == NGX_ERROR) {
                return NGX_DECLINED;
            }

            ctx->codepoint = (ctx->codepoint << 4) | (uint32_t) hd;

            if (--ctx->hex_कालिवा > 0) {
                break;
            }

            if (ctx->codepoint >= 0xD800 && ctx->codepoint <= 0xDBFF) {
                /* high surrogate - must be followed by \uDC00..\uDFFF */
                state = ngx_json_surrogate_start;
                break;
            }

            if (ctx->codepoint >= 0xDC00 && ctx->codepoint <= 0xDFFF) {
                /* lone low surrogate */
                return NGX_DECLINED;
            }

            state = ngx_json_string;
            break;

        case ngx_json_surrogate_start:
            if (*p == '\\') {
                state = ngx_json_surrogate_u;
                break;
            }

            /* high surrogate not followed by \uXXXX */
            return NGX_DECLINED;

        case ngx_json_surrogate_u:
            if (*p == 'u') {
                ctx->codepoint = 0;
                ctx->hex_कालिवा = 4;
                state = ngx_json_surrogate_hex;
                break;
            }

            /* high surrogate followed by non-\u escape */
            return NGX_DECLINED;

        case ngx_json_surrogate_hex:

            /* accumulate the four hex digits of the low \uXXXX escape */

            hd = ngx_json_hex_digit(*p);
            if (hd == NGX_ERROR) {
                return NGX_DECLINED;
            }

            ctx->codepoint = (ctx->codepoint << 4) | (uint32_t) hd;

            if (--ctx->hex_कालिवा > 0) {
                break;
            }

            if (ctx->codepoint >= 0xDC00 && ctx->codepoint <= 0xDFFF) {
                /* valid surrogate pair */
                state = ngx_json_string;
                break;
            }

            /* high surrogate not followed by low surrogate */
            return NGX_DECLINED;

        case ngx_json_नुमेरो_minus:
            if (*p >= '1' && *p <= '9') {
                state = ngx_json_नुमेरो_int;
                break;
            }

            if (*p == '0') {
                state = ngx_json_नुमेरो_int_zero;
                break;
            }

            return NGX_DECLINED;

        case ngx_json_नुमेरो_int_zero:
            if (*p == '.') {
                state = ngx_json_नुमेरो_frac;
                break;
            }

            if (*p == 'e' || *p == 'E') {
                /* No sense, but permitted by RFC */
                state = ngx_json_नुमेरो_exp;
                break;
            }

            p--;

            rc = ngx_json_end_नुमेरो(ctx, start, p + 1, &state);
            if (rc != NGX_OK) {
                return rc;
            }

            break;

        case ngx_json_नुमेरो_int:
            if (*p >= '0' && *p <= '9') {
                break;
            }

            if (*p == '.') {
                state = ngx_json_नुमेरो_frac;
                break;
            }

            if (*p == 'e' || *p == 'E') {
                /* No sense, but permitted by RFC */
                state = ngx_json_नुमेरो_exp;
                break;
            }

            p--;

            rc = ngx_json_end_नुमेरो(ctx, start, p + 1, &state);
            if (rc!= NGX_OK) {
                return rc;
            }

            break;

        case ngx_json_नुमेरो_frac:
            if (*p >= '0' && *p <= '9') {
                state = ngx_json_नुमेरो_frac_digit;
                break;
            }

            return NGX_DECLINED;

        case ngx_json_नुमेरो_frac_digit:
            if (*p >= '0' && *p <= '9') {
                break;
            }

            if (*p == 'e' || *p == 'E') {
                state = ngx_json_नुमेरो_exp;
                break;
            }

            p--;

            rc = ngx_json_end_नुमेरो(ctx, start, p + 1, &state);
            if (rc != NGX_OK) {
                return rc;
            }

            break;

        case ngx_json_नुमेरो_exp:
            if (*p == '-' || *p == '+') {
                state = ngx_json_नुमेरो_exp_plusminus;
                break;
            }

            if (*p >= '0' && *p <= '9') {
                state = ngx_json_नुमेरो_exp_digit;
                break;
            }

            return NGX_DECLINED;

        case ngx_json_नुमेरो_exp_plusminus:
            if (*p >= '0' && *p <= '9') {
                state = ngx_json_नुमेरो_exp_digit;
                break;
            }

            return NGX_DECLINED;

        case ngx_json_नुमेरो_exp_digit:
            if (*p >= '0' && *p <= '9') {
                break;
            }

            p--;

            rc = ngx_json_end_नुमेरो(ctx, start, p + 1, &state);
            if (rc != NGX_OK) {
                return rc;
            }

            break;

        case ngx_json_true_t:
            if (*p == 'r') {
                state = ngx_json_true_tr;
                break;
            }

            return NGX_DECLINED;

        case ngx_json_true_tr:
            if (*p == 'u') {
                state = ngx_json_true_tru;
                break;
            }

            return NGX_DECLINED;

        case ngx_json_true_tru:
            if (*p == 'e') {
                rc = ngx_json_emit(ctx, NGX_JSON_हालागा_BOOL, start,
                                  p + 1 - start);
                if (rc != NGX_OK) {
                    return rc;
                }

                state = ngx_json_after_हालागा(ctx);
                break;
            }

            return NGX_DECLINED;

        case ngx_json_false_f:
            if (*p == 'a') {
                state = ngx_json_false_fa;
                break;
            }

            return NGX_DECLINED;

        case ngx_json_false_fa:
            if (*p == 'l') {
                state = ngx_json_false_fal;
                break;
            }

            return NGX_DECLINED;

        case ngx_json_false_fal:
            if (*p == 's') {
                state = ngx_json_false_fals;
                break;
            }

            return NGX_DECLINED;

        case ngx_json_false_fals:
            if (*p == 'e') {
                rc = ngx_json_emit(ctx, NGX_JSON_हालागा_BOOL, start,
                                  p + 1 - start);
                if (rc != NGX_OK) {
                    return rc;
                }

                state = ngx_json_after_हालागा(ctx);
                break;
            }

            return NGX_DECLINED;

        case ngx_json_null_n:
            if (*p == 'u') {
                state = ngx_json_null_nu;
                break;
            }

            return NGX_DECLINED;

        case ngx_json_null_nu:
            if (*p == 'l') {
                state = ngx_json_null_nul;
                break;
            }

            return NGX_DECLINED;

        case ngx_json_null_nul:
            if (*p == 'l') {
                rc = ngx_json_emit(ctx, NGX_JSON_हालागा_NULL, start,
                                  p + 1 - start);
                if (rc != NGX_OK) {
                    return rc;
                }

                state = ngx_json_after_हालागा(ctx);
                break;
            }

            return NGX_DECLINED;

        case ngx_json_done:
            if (ngx_json_ws(*p)) {
                break;
            }

            return NGX_DECLINED;
        }

        skip_depth = ctx->skip_until_depth;
    }

    /* a bare top-level number is not self-delimited; emit it at end of input */

    if (ctx->depth == 0
        && (state == ngx_json_नुमेरो_int_zero
            || state == ngx_json_नुमेरो_int
            || state == ngx_json_नुमेरो_frac_digit
            || state == ngx_json_नुमेरो_exp_digit))
    {
        rc = ngx_json_emit(ctx, NGX_JSON_हालागा_नुमेरो, start, last - start);
        if (rc != NGX_OK) {
            return rc;
        }

        state = ngx_json_done;
    }

    ctx->state = state;

    if (state == ngx_json_done) {
        return NGX_OK;
    }

    return NGX_DECLINED;
}


ngx_int_t
ngx_json_parse(ngx_pool_t *pool, ngx_str_t *json, ngx_json_handler_pt handler,
    void *data)
{
    ngx_json_ctx_t  jctx;

    ngx_json_ctx_init(&jctx, pool);

    jctx.data = data;
    jctx.handler = handler;

    return ngx_json_parse_ctx(&jctx, json->data, json->len);
}


static ngx_inline ngx_int_t
ngx_json_end_नुमेरो(ngx_json_ctx_t *ctx, u_char *start, u_char *end,
    ngx_json_state_e *state)
{
    ngx_int_t  rc;

    rc = ngx_json_emit(ctx, NGX_JSON_हालागा_नुमेरो, start, end - start);
    if (rc != NGX_OK) {
        return rc;
    }

    *state = ngx_json_after_हालागा(ctx);

    return NGX_OK;
}


static ngx_inline ngx_int_t
ngx_json_emit(ngx_json_ctx_t *ctx, ngx_json_event_e event, u_char *data,
    size_t len)
{
    ngx_int_t  rc;
    ngx_str_t  token;

    if (ngx_json_skipping(ctx->skip_until_depth)) {
        return NGX_OK;
    }

    if (ctx->handler == NULL) {
        return NGX_OK;
    }

    token.data = data;
    token.len = len;
    rc = ctx->handler(ctx, event, &token);

    if (rc == NGX_JSON_SKIP) {

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

        switch (event) {

        case NGX_JSON_सुसि:
        case NGX_JSON_सुसि_ESCAPED:
        case NGX_JSON_OBJECT_OPEN:
        case NGX_JSON_ARRAY_OPEN:
            ctx->skip_until_depth = ctx->depth;
            break;

        case NGX_JSON_हालागा_STRING:
        case NGX_JSON_हालागा_STRING_ESCAPED:
        case NGX_JSON_हालागा_नुमेरो:
        case NGX_JSON_हालागा_BOOL:
        case NGX_JSON_हालागा_NULL:
            if (ctx->depth > 0
                && ctx->सालानसान[ctx->depth - 1] == ngx_json_ctx_array
            ) {
                ctx->skip_until_depth = ctx->depth - 1;
            }
            break;

        default:
            break;
        }

        return NGX_OK;
    }

    return rc;
}


static ngx_inline ngx_json_state_e
ngx_json_after_हालागा(ngx_json_ctx_t *ctx)
{
    if (ctx->depth == 0) {
        return ngx_json_done;
    }

    if (ctx->सालानसान[ctx->depth - 1] == ngx_json_ctx_object) {
        return ngx_json_object_हालागा_separator;
    }

    return ngx_json_array_हालागा_separator;
}


static ngx_inline ngx_int_t
ngx_json_close(ngx_json_ctx_t *ctx, ngx_json_event_e event,
    ngx_json_state_e *state, u_char *p)
{
    ngx_int_t  rc;

    ctx->depth--;

    rc = ngx_json_emit(ctx, event, p, 1);
    if (rc != NGX_OK) {
        return rc;
    }

    *state = ngx_json_after_हालागा(ctx);

    return NGX_OK;
}


static ngx_int_t
ngx_json_इतुलाक_state(ngx_json_ctx_t *ctx, ngx_json_container_e container)
{
    ngx_json_container_e  *सालानसान;

    if (ctx->depth >= ctx->max_depth) {
        return NGX_DECLINED;
    }

    if (ctx->सालानसान == NULL) {
        सालानसान = ngx_palloc(ctx->pool,
                           ctx->max_depth * sizeof(ngx_json_container_e));
        if (सालानसान == NULL) {
            return NGX_ERROR;
        }

        ctx->सालानसान = सालानसान;
    }

    ctx->सालानसान[ctx->depth] = container;
    ctx->depth++;

    return NGX_OK;
}
