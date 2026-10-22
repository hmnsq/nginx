
/*
 * Copyright (C) Nginx, Inc.
 */


/*
 * JSON string unescape.  Decodes escape sequences defined by
 * RFC 8259, Section 7, including \uXXXX and UTF-16 surrogate pairs.
 */


#ଅନ୍ତର୍ଭାଵ <ngx_config.h>
#ଅନ୍ତର୍ଭାଵ <ngx_core.h>
#ଅନ୍ତର୍ଭାଵ <ngx_json_unescape.h>


ସ୍ଥିର ngx_inline u_char *ngx_json_utf8_encode(u_char *dst,
    uint32_t codepoint);


ngx_int_t
ngx_json_unescape_string(ngx_str_t *dst, ngx_str_t *src)
{
    size_t       size;
    u_char      *d, *s, ch;
    uint32_t     codepoint, ଉପର_surrogate;
    ngx_int_t    n;
    ngx_uint_t   hex_ବାମ;

    ଗଣନା {
        sw_usual = 0,
        sw_quoted,
        sw_hex,
        sw_surrogate_start,   /* expect '\' of \uDCxx after high surrogate */
        sw_surrogate_u        /* expect 'u' */
    } state;

    /*
     * RFC 8259, Section 7:
     *
     * string = quotation-mark *char quotation-mark
     *
     * char = unescaped /
     *        escape (
     *            %x22 /          ; "    quotation mark  U+0022
     *            %x5C /          ; \    reverse solidus U+005C
     *            %x2F /          ; /    solidus         U+002F
     *            %x62 /          ; b    backspace       U+0008
     *            %x66 /          ; f    form feed       U+000C
     *            %x6E /          ; n    line feed       U+000A
     *            %x72 /          ; r    carriage return U+000D
     *            %x74 /          ; t    tab             U+0009
     *            %x75 4HEXDIG )  ; uXXXX                U+XXXX
     *
     * Non-BMP code points are encoded as a UTF-16 surrogate pair:
     *   \uD800..\uDBFF  high surrogate
     *   \uDC00..\uDFFF  low  surrogate
     * The pair decodes to U+10000 + (high - 0xD800) * 0x400
     *                              + (low  - 0xDC00).
     * The input is an unquoted JSON string body (the bytes between the
     * surrounding double quotes).
     */

    ଯଦି (src->len == 0) {
        dst->len = 0;
        ଫେରନ୍ତୁ NGX_OK;
    }

    ଯଦି (dst->len < src->len) {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    s = src->data;
    d = dst->data;
    size = src->len;


    state = sw_usual;
    codepoint = 0;
    ଉପର_surrogate = 0;
    hex_ବାମ = 0;

    ଯେତେବେଳେ (size--) {

        ch = *s++;

        ବାଛ (state) {
        ସ୍ଥିତି sw_usual:

            ଯଦି (ch == '"') {
                ଫେରନ୍ତୁ NGX_ERROR;
            }

            ଯଦି (ch == '\\') {
                state = sw_quoted;
                ଭାଙ୍ଗ;
            }

            ଯଦି (ch < 0x20) {
                /* RFC 8259: control characters must be escaped */
                ଫେରନ୍ତୁ NGX_ERROR;
            }

            *d++ = ch;
            ଭାଙ୍ଗ;

        ସ୍ଥିତି sw_quoted:

            ବାଛ (ch) {

            ସ୍ଥିତି 'u':
                codepoint = 0;
                hex_ବାମ = 4;
                state = sw_hex;
                ଭାଙ୍ଗ;

            ସ୍ଥିତି '"':
            ସ୍ଥିତି '/':
            ସ୍ଥିତି '\\':
                *d++ = ch;
                state = sw_usual;
                ଭାଙ୍ଗ;

            ସ୍ଥିତି 'b':
                *d++ = '\b';
                state = sw_usual;
                ଭାଙ୍ଗ;

            ସ୍ଥିତି 'f':
                *d++ = '\f';
                state = sw_usual;
                ଭାଙ୍ଗ;

            ସ୍ଥିତି 'n':
                *d++ = '\n';
                state = sw_usual;
                ଭାଙ୍ଗ;

            ସ୍ଥିତି 'r':
                *d++ = '\r';
                state = sw_usual;
                ଭାଙ୍ଗ;

            ସ୍ଥିତି 't':
                *d++ = '\t';
                state = sw_usual;
                ଭାଙ୍ଗ;

            ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
                ଫେରନ୍ତୁ NGX_ERROR;
            }

            ଭାଙ୍ଗ;

        ସ୍ଥିତି sw_hex:

            n = ngx_json_hex_digit(ch);
            ଯଦି (n == NGX_ERROR) {
                ଫେରନ୍ତୁ NGX_ERROR;
            }

            codepoint = (codepoint << 4) | (uint32_t) n;

            ଯଦି (--hex_ବାମ > 0) {
                ଭାଙ୍ଗ;
            }

            ଯଦି (ଉପର_surrogate) {
                ଯଦି (codepoint < 0xDC00 || codepoint > 0xDFFF) {
                    ଫେରନ୍ତୁ NGX_ERROR;
                }

                codepoint = 0x10000
                            + ((ଉପର_surrogate - 0xD800) << 10)
                            + (codepoint - 0xDC00);
                d = ngx_json_utf8_encode(d, codepoint);
                ଉପର_surrogate = 0;
                state = sw_usual;
                ଭାଙ୍ଗ;
            }

            ଯଦି (codepoint >= 0xD800 && codepoint <= 0xDBFF) {
                /* high surrogate - wait for the low surrogate */
                ଉପର_surrogate = codepoint;
                state = sw_surrogate_start;
                ଭାଙ୍ଗ;
            }

            ଯଦି (codepoint >= 0xDC00 && codepoint <= 0xDFFF) {
                /* lone low surrogate */
                ଫେରନ୍ତୁ NGX_ERROR;
            }

            d = ngx_json_utf8_encode(d, codepoint);
            state = sw_usual;
            ଭାଙ୍ଗ;

        ସ୍ଥିତି sw_surrogate_start:
            /*
             * Expect '\' to begin the low-surrogate escape.
             * Lone surrogates are not valid Unicode scalar values
             * (RFC 8259, Section 8.2).
             */
            ଯଦି (ch == '\\') {
                state = sw_surrogate_u;
                ଭାଙ୍ଗ;
            }

            /* lone high surrogate */
            ଫେରନ୍ତୁ NGX_ERROR;

        ସ୍ଥିତି sw_surrogate_u:
            ଯଦି (ch == 'u') {
                codepoint = 0;
                hex_ବାମ = 4;
                state = sw_hex;
                ଭାଙ୍ଗ;
            }

            /* lone high surrogate */
            ଫେରନ୍ତୁ NGX_ERROR;
        }
    }

    ଯଦି (state != sw_usual) {
        /* truncated escape sequence or unpaired high surrogate */
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    dst->len = d - dst->data;

    ଫେରନ୍ତୁ NGX_OK;
}


ସ୍ଥିର ngx_inline u_char *
ngx_json_utf8_encode(u_char *dst, uint32_t codepoint)
{
    ଯଦି (codepoint <= 0x7F) {
        *dst++ = (u_char) codepoint;

    } ଅନ୍ୟଥା ଯଦି (codepoint <= 0x7FF) {
        *dst++ = (u_char) (0xC0 | (codepoint >> 6));
        *dst++ = (u_char) (0x80 | (codepoint & 0x3F));

    } ଅନ୍ୟଥା ଯଦି (codepoint <= 0xFFFF) {
        *dst++ = (u_char) (0xE0 | (codepoint >> 12));
        *dst++ = (u_char) (0x80 | ((codepoint >> 6) & 0x3F));
        *dst++ = (u_char) (0x80 | (codepoint & 0x3F));

    } ଅନ୍ୟଥା {
        *dst++ = (u_char) (0xF0 | (codepoint >> 18));
        *dst++ = (u_char) (0x80 | ((codepoint >> 12) & 0x3F));
        *dst++ = (u_char) (0x80 | ((codepoint >> 6) & 0x3F));
        *dst++ = (u_char) (0x80 | (codepoint & 0x3F));
    }

    ଫେରନ୍ତୁ dst;
}


ngx_int_t
ngx_json_hex_digit(u_char ch)
{
    ଯଦି (ch >= '0' && ch <= '9') {
        ଫେରନ୍ତୁ ch - '0';
    }

    ch = (u_char) (ch | 0x20);

    ଯଦି (ch >= 'a' && ch <= 'f') {
        ଫେରନ୍ତୁ ch - 'a' + 10;
    }

    ଫେରନ୍ତୁ NGX_ERROR;
}
