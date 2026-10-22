
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
    size_t       size;
    u_char      *d, *s, ch;
    uint32_t     codepoint, उच्चः_surrogate;
    ngx_int_t    n;
    ngx_uint_t   hex_वामः;

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

    सचे (src->len == 0) {
        dst->len = 0;
        पटिनिवत्त NGX_OK;
    }

    सचे (dst->len < src->len) {
        पटिनिवत्त NGX_ERROR;
    }

    s = src->data;
    d = dst->data;
    size = src->len;


    state = sw_usual;
    codepoint = 0;
    उच्चः_surrogate = 0;
    hex_वामः = 0;

    याव (size--) {

        ch = *s++;

        विकल्प (state) {
        पक्ष sw_usual:

            सचे (ch == '"') {
                पटिनिवत्त NGX_ERROR;
            }

            सचे (ch == '\\') {
                state = sw_quoted;
                भङ्ग;
            }

            सचे (ch < 0x20) {
                /* RFC 8259: control characters must be escaped */
                पटिनिवत्त NGX_ERROR;
            }

            *d++ = ch;
            भङ्ग;

        पक्ष sw_quoted:

            विकल्प (ch) {

            पक्ष 'u':
                codepoint = 0;
                hex_वामः = 4;
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
                पटिनिवत्त NGX_ERROR;
            }

            भङ्ग;

        पक्ष sw_hex:

            n = ngx_json_hex_digit(ch);
            सचे (n == NGX_ERROR) {
                पटिनिवत्त NGX_ERROR;
            }

            codepoint = (codepoint << 4) | (uint32_t) n;

            सचे (--hex_वामः > 0) {
                भङ्ग;
            }

            सचे (उच्चः_surrogate) {
                सचे (codepoint < 0xDC00 || codepoint > 0xDFFF) {
                    पटिनिवत्त NGX_ERROR;
                }

                codepoint = 0x10000
                            + ((उच्चः_surrogate - 0xD800) << 10)
                            + (codepoint - 0xDC00);
                d = ngx_json_utf8_encode(d, codepoint);
                उच्चः_surrogate = 0;
                state = sw_usual;
                भङ्ग;
            }

            सचे (codepoint >= 0xD800 && codepoint <= 0xDBFF) {
                /* high surrogate - wait for the low surrogate */
                उच्चः_surrogate = codepoint;
                state = sw_surrogate_start;
                भङ्ग;
            }

            सचे (codepoint >= 0xDC00 && codepoint <= 0xDFFF) {
                /* lone low surrogate */
                पटिनिवत्त NGX_ERROR;
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
            सचे (ch == '\\') {
                state = sw_surrogate_u;
                भङ्ग;
            }

            /* lone high surrogate */
            पटिनिवत्त NGX_ERROR;

        पक्ष sw_surrogate_u:
            सचे (ch == 'u') {
                codepoint = 0;
                hex_वामः = 4;
                state = sw_hex;
                भङ्ग;
            }

            /* lone high surrogate */
            पटिनिवत्त NGX_ERROR;
        }
    }

    सचे (state != sw_usual) {
        /* truncated escape sequence or unpaired high surrogate */
        पटिनिवत्त NGX_ERROR;
    }

    dst->len = d - dst->data;

    पटिनिवत्त NGX_OK;
}


स्थिर ngx_inline u_char *
ngx_json_utf8_encode(u_char *dst, uint32_t codepoint)
{
    सचे (codepoint <= 0x7F) {
        *dst++ = (u_char) codepoint;

    } अञ्ञथा सचे (codepoint <= 0x7FF) {
        *dst++ = (u_char) (0xC0 | (codepoint >> 6));
        *dst++ = (u_char) (0x80 | (codepoint & 0x3F));

    } अञ्ञथा सचे (codepoint <= 0xFFFF) {
        *dst++ = (u_char) (0xE0 | (codepoint >> 12));
        *dst++ = (u_char) (0x80 | ((codepoint >> 6) & 0x3F));
        *dst++ = (u_char) (0x80 | (codepoint & 0x3F));

    } अञ्ञथा {
        *dst++ = (u_char) (0xF0 | (codepoint >> 18));
        *dst++ = (u_char) (0x80 | ((codepoint >> 12) & 0x3F));
        *dst++ = (u_char) (0x80 | ((codepoint >> 6) & 0x3F));
        *dst++ = (u_char) (0x80 | (codepoint & 0x3F));
    }

    पटिनिवत्त dst;
}


ngx_int_t
ngx_json_hex_digit(u_char ch)
{
    सचे (ch >= '0' && ch <= '9') {
        पटिनिवत्त ch - '0';
    }

    ch = (u_char) (ch | 0x20);

    सचे (ch >= 'a' && ch <= 'f') {
        पटिनिवत्त ch - 'a' + 10;
    }

    पटिनिवत्त NGX_ERROR;
}
