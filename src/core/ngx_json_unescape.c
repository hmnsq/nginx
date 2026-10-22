
/*
 * Copyright (C) Nginx, Inc.
 */


/*
 * JSON string unescape.  Decodes escape sequences defined by
 * RFC 8259, Section 7, including \uXXXX and UTF-16 surrogate pairs.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>
#अन्तर्भाव <ngx_json_unescape.h>


स्थिर ngx_inline u_char *ngx_json_utf8_encode(u_char *dst,
    uint32_t codepoint);


ngx_int_t
ngx_json_unescape_string(ngx_str_t *dst, ngx_str_t *src)
{
    माप_प्रकार       size;
    u_char      *d, *s, ch;
    uint32_t     codepoint, ऊपरी_surrogate;
    ngx_int_t    n;
    ngx_uint_t   hex_बायाँ;

    गणना {
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

    यदि (src->len == 0) {
        dst->len = 0;
        प्रत्यागम NGX_OK;
    }

    यदि (dst->len < src->len) {
        प्रत्यागम NGX_ERROR;
    }

    s = src->data;
    d = dst->data;
    size = src->len;


    state = sw_usual;
    codepoint = 0;
    ऊपरी_surrogate = 0;
    hex_बायाँ = 0;

    यावत् (size--) {

        ch = *s++;

        विकल्प (state) {
        पक्ष sw_usual:

            यदि (ch == '"') {
                प्रत्यागम NGX_ERROR;
            }

            यदि (ch == '\\') {
                state = sw_quoted;
                भङ्ग;
            }

            यदि (ch < 0x20) {
                /* RFC 8259: control characters must be escaped */
                प्रत्यागम NGX_ERROR;
            }

            *d++ = ch;
            भङ्ग;

        पक्ष sw_quoted:

            विकल्प (ch) {

            पक्ष 'u':
                codepoint = 0;
                hex_बायाँ = 4;
                state = sw_hex;
                भङ्ग;

            पक्ष '"':
            पक्ष '/':
            पक्ष '\\':
                *d++ = ch;
                state = sw_usual;
                भङ्ग;

            पक्ष 'b':
                *d++ = '\b';
                state = sw_usual;
                भङ्ग;

            पक्ष 'f':
                *d++ = '\f';
                state = sw_usual;
                भङ्ग;

            पक्ष 'n':
                *d++ = '\n';
                state = sw_usual;
                भङ्ग;

            पक्ष 'r':
                *d++ = '\r';
                state = sw_usual;
                भङ्ग;

            पक्ष 't':
                *d++ = '\t';
                state = sw_usual;
                भङ्ग;

            स्वभाव:
                प्रत्यागम NGX_ERROR;
            }

            भङ्ग;

        पक्ष sw_hex:

            n = ngx_json_hex_digit(ch);
            यदि (n == NGX_ERROR) {
                प्रत्यागम NGX_ERROR;
            }

            codepoint = (codepoint << 4) | (uint32_t) n;

            यदि (--hex_बायाँ > 0) {
                भङ्ग;
            }

            यदि (ऊपरी_surrogate) {
                यदि (codepoint < 0xDC00 || codepoint > 0xDFFF) {
                    प्रत्यागम NGX_ERROR;
                }

                codepoint = 0x10000
                            + ((ऊपरी_surrogate - 0xD800) << 10)
                            + (codepoint - 0xDC00);
                d = ngx_json_utf8_encode(d, codepoint);
                ऊपरी_surrogate = 0;
                state = sw_usual;
                भङ्ग;
            }

            यदि (codepoint >= 0xD800 && codepoint <= 0xDBFF) {
                /* high surrogate - wait for the low surrogate */
                ऊपरी_surrogate = codepoint;
                state = sw_surrogate_start;
                भङ्ग;
            }

            यदि (codepoint >= 0xDC00 && codepoint <= 0xDFFF) {
                /* lone low surrogate */
                प्रत्यागम NGX_ERROR;
            }

            d = ngx_json_utf8_encode(d, codepoint);
            state = sw_usual;
            भङ्ग;

        पक्ष sw_surrogate_start:
            /*
             * Expect '\' to begin the low-surrogate escape.
             * Lone surrogates are not valid Unicode scalar values
             * (RFC 8259, Section 8.2).
             */
            यदि (ch == '\\') {
                state = sw_surrogate_u;
                भङ्ग;
            }

            /* lone high surrogate */
            प्रत्यागम NGX_ERROR;

        पक्ष sw_surrogate_u:
            यदि (ch == 'u') {
                codepoint = 0;
                hex_बायाँ = 4;
                state = sw_hex;
                भङ्ग;
            }

            /* lone high surrogate */
            प्रत्यागम NGX_ERROR;
        }
    }

    यदि (state != sw_usual) {
        /* truncated escape sequence or unpaired high surrogate */
        प्रत्यागम NGX_ERROR;
    }

    dst->len = d - dst->data;

    प्रत्यागम NGX_OK;
}


स्थिर ngx_inline u_char *
ngx_json_utf8_encode(u_char *dst, uint32_t codepoint)
{
    यदि (codepoint <= 0x7F) {
        *dst++ = (u_char) codepoint;

    } अन्यथा यदि (codepoint <= 0x7FF) {
        *dst++ = (u_char) (0xC0 | (codepoint >> 6));
        *dst++ = (u_char) (0x80 | (codepoint & 0x3F));

    } अन्यथा यदि (codepoint <= 0xFFFF) {
        *dst++ = (u_char) (0xE0 | (codepoint >> 12));
        *dst++ = (u_char) (0x80 | ((codepoint >> 6) & 0x3F));
        *dst++ = (u_char) (0x80 | (codepoint & 0x3F));

    } अन्यथा {
        *dst++ = (u_char) (0xF0 | (codepoint >> 18));
        *dst++ = (u_char) (0x80 | ((codepoint >> 12) & 0x3F));
        *dst++ = (u_char) (0x80 | ((codepoint >> 6) & 0x3F));
        *dst++ = (u_char) (0x80 | (codepoint & 0x3F));
    }

    प्रत्यागम dst;
}


ngx_int_t
ngx_json_hex_digit(u_char ch)
{
    यदि (ch >= '0' && ch <= '9') {
        प्रत्यागम ch - '0';
    }

    ch = (u_char) (ch | 0x20);

    यदि (ch >= 'a' && ch <= 'f') {
        प्रत्यागम ch - 'a' + 10;
    }

    प्रत्यागम NGX_ERROR;
}
