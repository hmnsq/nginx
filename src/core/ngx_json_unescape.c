
/*
 * Copyright (C) Nginx, Inc.
 */


/*
 * JSON string unescape.  Decodes escape sequences defined by
 * RFC 8259, Section 7, including \uXXXX and UTF-16 surrogate pairs.
 */


#समावेश <ngx_config.h>
#समावेश <ngx_core.h>
#समावेश <ngx_json_unescape.h>


अटल ngx_inline u_char *ngx_json_utf8_encode(u_char *dst,
    uint32_t codepoint);


ngx_int_t
ngx_json_unescape_string(ngx_str_t *dst, ngx_str_t *src)
{
    माप_प्रकार       आकार;
    u_char      *d, *s, ch;
    uint32_t     codepoint, ऊपरी_surrogate;
    ngx_int_t    n;
    ngx_uint_t   hex_बायाँ;

    क्रमागत {
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

    अगर (src->len == 0) {
        dst->len = 0;
        वापस NGX_OK;
    }

    अगर (dst->len < src->len) {
        वापस NGX_ERROR;
    }

    s = src->तथ्य;
    d = dst->तथ्य;
    आकार = src->len;


    state = sw_usual;
    codepoint = 0;
    ऊपरी_surrogate = 0;
    hex_बायाँ = 0;

    जबतक (आकार--) {

        ch = *s++;

        चयन (state) {
        हाल sw_usual:

            अगर (ch == '"') {
                वापस NGX_ERROR;
            }

            अगर (ch == '\\') {
                state = sw_quoted;
                अवरोध;
            }

            अगर (ch < 0x20) {
                /* RFC 8259: control characters must be escaped */
                वापस NGX_ERROR;
            }

            *d++ = ch;
            अवरोध;

        हाल sw_quoted:

            चयन (ch) {

            हाल 'u':
                codepoint = 0;
                hex_बायाँ = 4;
                state = sw_hex;
                अवरोध;

            हाल '"':
            हाल '/':
            हाल '\\':
                *d++ = ch;
                state = sw_usual;
                अवरोध;

            हाल 'b':
                *d++ = '\b';
                state = sw_usual;
                अवरोध;

            हाल 'f':
                *d++ = '\f';
                state = sw_usual;
                अवरोध;

            हाल 'n':
                *d++ = '\n';
                state = sw_usual;
                अवरोध;

            हाल 'r':
                *d++ = '\r';
                state = sw_usual;
                अवरोध;

            हाल 't':
                *d++ = '\t';
                state = sw_usual;
                अवरोध;

            शेष:
                वापस NGX_ERROR;
            }

            अवरोध;

        हाल sw_hex:

            n = ngx_json_hex_digit(ch);
            अगर (n == NGX_ERROR) {
                वापस NGX_ERROR;
            }

            codepoint = (codepoint << 4) | (uint32_t) n;

            अगर (--hex_बायाँ > 0) {
                अवरोध;
            }

            अगर (ऊपरी_surrogate) {
                अगर (codepoint < 0xDC00 || codepoint > 0xDFFF) {
                    वापस NGX_ERROR;
                }

                codepoint = 0x10000
                            + ((ऊपरी_surrogate - 0xD800) << 10)
                            + (codepoint - 0xDC00);
                d = ngx_json_utf8_encode(d, codepoint);
                ऊपरी_surrogate = 0;
                state = sw_usual;
                अवरोध;
            }

            अगर (codepoint >= 0xD800 && codepoint <= 0xDBFF) {
                /* high surrogate - wait for the low surrogate */
                ऊपरी_surrogate = codepoint;
                state = sw_surrogate_start;
                अवरोध;
            }

            अगर (codepoint >= 0xDC00 && codepoint <= 0xDFFF) {
                /* lone low surrogate */
                वापस NGX_ERROR;
            }

            d = ngx_json_utf8_encode(d, codepoint);
            state = sw_usual;
            अवरोध;

        हाल sw_surrogate_start:
            /*
             * Expect '\' to begin the low-surrogate escape.
             * Lone surrogates are not valid Unicode scalar values
             * (RFC 8259, Section 8.2).
             */
            अगर (ch == '\\') {
                state = sw_surrogate_u;
                अवरोध;
            }

            /* lone high surrogate */
            वापस NGX_ERROR;

        हाल sw_surrogate_u:
            अगर (ch == 'u') {
                codepoint = 0;
                hex_बायाँ = 4;
                state = sw_hex;
                अवरोध;
            }

            /* lone high surrogate */
            वापस NGX_ERROR;
        }
    }

    अगर (state != sw_usual) {
        /* truncated escape sequence or unpaired high surrogate */
        वापस NGX_ERROR;
    }

    dst->len = d - dst->तथ्य;

    वापस NGX_OK;
}


अटल ngx_inline u_char *
ngx_json_utf8_encode(u_char *dst, uint32_t codepoint)
{
    अगर (codepoint <= 0x7F) {
        *dst++ = (u_char) codepoint;

    } अन्यथा अगर (codepoint <= 0x7FF) {
        *dst++ = (u_char) (0xC0 | (codepoint >> 6));
        *dst++ = (u_char) (0x80 | (codepoint & 0x3F));

    } अन्यथा अगर (codepoint <= 0xFFFF) {
        *dst++ = (u_char) (0xE0 | (codepoint >> 12));
        *dst++ = (u_char) (0x80 | ((codepoint >> 6) & 0x3F));
        *dst++ = (u_char) (0x80 | (codepoint & 0x3F));

    } अन्यथा {
        *dst++ = (u_char) (0xF0 | (codepoint >> 18));
        *dst++ = (u_char) (0x80 | ((codepoint >> 12) & 0x3F));
        *dst++ = (u_char) (0x80 | ((codepoint >> 6) & 0x3F));
        *dst++ = (u_char) (0x80 | (codepoint & 0x3F));
    }

    वापस dst;
}


ngx_int_t
ngx_json_hex_digit(u_char ch)
{
    अगर (ch >= '0' && ch <= '9') {
        वापस ch - '0';
    }

    ch = (u_char) (ch | 0x20);

    अगर (ch >= 'a' && ch <= 'f') {
        वापस ch - 'a' + 10;
    }

    वापस NGX_ERROR;
}
