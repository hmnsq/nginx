
/*
 * Copyright (C) Nginx, Inc.
 */


/*
 * JSON string unescape.  Decodes escape sequences defined by
 * RFC 8259, Section 7, including \uXXXX and UTF-16 surrogate pairs.
 */


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>
#అన్తర్భావ <ngx_json_unescape.h>


స్థిర ngx_inline u_char *ngx_json_utf8_encode(u_char *dst,
    uint32_t codepoint);


ngx_int_t
ngx_json_unescape_string(ngx_str_t *dst, ngx_str_t *src)
{
    కొలత_t       కొలత;
    u_char      *d, *s, ch;
    uint32_t     codepoint, ఎగువ_surrogate;
    ngx_int_t    n;
    ngx_uint_t   hex_ఎడమ;

    గణన {
        sw_usual = 0,
        sw_quoted,
        sw_hex,
        sw_surrogate_ప్రారంభం,   /* expect '\' of \uDCxx after high surrogate */
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

    ఒకవేళ (src->len == 0) {
        dst->len = 0;
        తిరిగి NGX_OK;
    }

    ఒకవేళ (dst->len < src->len) {
        తిరిగి NGX_ERROR;
    }

    s = src->దత్తాంశం;
    d = dst->దత్తాంశం;
    కొలత = src->len;


    state = sw_usual;
    codepoint = 0;
    ఎగువ_surrogate = 0;
    hex_ఎడమ = 0;

    అయితే (కొలత--) {

        ch = *s++;

        ఎంపిక (state) {
        స్థితి sw_usual:

            ఒకవేళ (ch == '"') {
                తిరిగి NGX_ERROR;
            }

            ఒకవేళ (ch == '\\') {
                state = sw_quoted;
                ఆపు;
            }

            ఒకవేళ (ch < 0x20) {
                /* RFC 8259: control characters must be escaped */
                తిరిగి NGX_ERROR;
            }

            *d++ = ch;
            ఆపు;

        స్థితి sw_quoted:

            ఎంపిక (ch) {

            స్థితి 'u':
                codepoint = 0;
                hex_ఎడమ = 4;
                state = sw_hex;
                ఆపు;

            స్థితి '"':
            స్థితి '/':
            స్థితి '\\':
                *d++ = ch;
                state = sw_usual;
                ఆపు;

            స్థితి 'b':
                *d++ = '\b';
                state = sw_usual;
                ఆపు;

            స్థితి 'f':
                *d++ = '\f';
                state = sw_usual;
                ఆపు;

            స్థితి 'n':
                *d++ = '\n';
                state = sw_usual;
                ఆపు;

            స్థితి 'r':
                *d++ = '\r';
                state = sw_usual;
                ఆపు;

            స్థితి 't':
                *d++ = '\t';
                state = sw_usual;
                ఆపు;

            అప్రమేయ:
                తిరిగి NGX_ERROR;
            }

            ఆపు;

        స్థితి sw_hex:

            n = ngx_json_hex_digit(ch);
            ఒకవేళ (n == NGX_ERROR) {
                తిరిగి NGX_ERROR;
            }

            codepoint = (codepoint << 4) | (uint32_t) n;

            ఒకవేళ (--hex_ఎడమ > 0) {
                ఆపు;
            }

            ఒకవేళ (ఎగువ_surrogate) {
                ఒకవేళ (codepoint < 0xDC00 || codepoint > 0xDFFF) {
                    తిరిగి NGX_ERROR;
                }

                codepoint = 0x10000
                            + ((ఎగువ_surrogate - 0xD800) << 10)
                            + (codepoint - 0xDC00);
                d = ngx_json_utf8_encode(d, codepoint);
                ఎగువ_surrogate = 0;
                state = sw_usual;
                ఆపు;
            }

            ఒకవేళ (codepoint >= 0xD800 && codepoint <= 0xDBFF) {
                /* high surrogate - wait for the low surrogate */
                ఎగువ_surrogate = codepoint;
                state = sw_surrogate_ప్రారంభం;
                ఆపు;
            }

            ఒకవేళ (codepoint >= 0xDC00 && codepoint <= 0xDFFF) {
                /* lone low surrogate */
                తిరిగి NGX_ERROR;
            }

            d = ngx_json_utf8_encode(d, codepoint);
            state = sw_usual;
            ఆపు;

        స్థితి sw_surrogate_ప్రారంభం:
            /*
             * Expect '\' to begin the low-surrogate escape.
             * Lone surrogates are not valid Unicode scalar values
             * (RFC 8259, Section 8.2).
             */
            ఒకవేళ (ch == '\\') {
                state = sw_surrogate_u;
                ఆపు;
            }

            /* lone high surrogate */
            తిరిగి NGX_ERROR;

        స్థితి sw_surrogate_u:
            ఒకవేళ (ch == 'u') {
                codepoint = 0;
                hex_ఎడమ = 4;
                state = sw_hex;
                ఆపు;
            }

            /* lone high surrogate */
            తిరిగి NGX_ERROR;
        }
    }

    ఒకవేళ (state != sw_usual) {
        /* truncated escape sequence or unpaired high surrogate */
        తిరిగి NGX_ERROR;
    }

    dst->len = d - dst->దత్తాంశం;

    తిరిగి NGX_OK;
}


స్థిర ngx_inline u_char *
ngx_json_utf8_encode(u_char *dst, uint32_t codepoint)
{
    ఒకవేళ (codepoint <= 0x7F) {
        *dst++ = (u_char) codepoint;

    } లేకపోతే ఒకవేళ (codepoint <= 0x7FF) {
        *dst++ = (u_char) (0xC0 | (codepoint >> 6));
        *dst++ = (u_char) (0x80 | (codepoint & 0x3F));

    } లేకపోతే ఒకవేళ (codepoint <= 0xFFFF) {
        *dst++ = (u_char) (0xE0 | (codepoint >> 12));
        *dst++ = (u_char) (0x80 | ((codepoint >> 6) & 0x3F));
        *dst++ = (u_char) (0x80 | (codepoint & 0x3F));

    } లేకపోతే {
        *dst++ = (u_char) (0xF0 | (codepoint >> 18));
        *dst++ = (u_char) (0x80 | ((codepoint >> 12) & 0x3F));
        *dst++ = (u_char) (0x80 | ((codepoint >> 6) & 0x3F));
        *dst++ = (u_char) (0x80 | (codepoint & 0x3F));
    }

    తిరిగి dst;
}


ngx_int_t
ngx_json_hex_digit(u_char ch)
{
    ఒకవేళ (ch >= '0' && ch <= '9') {
        తిరిగి ch - '0';
    }

    ch = (u_char) (ch | 0x20);

    ఒకవేళ (ch >= 'a' && ch <= 'f') {
        తిరిగి ch - 'a' + 10;
    }

    తిరిగి NGX_ERROR;
}
