
/*
 * Copyright (C) Nginx, Inc.
 */


/*
 * JSON string unescape.  Decodes escape sequences defined by
 * RFC 8259, Section 7, including \uXXXX and UTF-16 surrogate pairs.
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>
#ಅನ್ತರ್ಭಾವ <ngx_json_unescape.h>


ಸ್ಥಿರ ngx_inline u_char *ngx_json_utf8_encode(u_char *dst,
    uint32_t codepoint);


ngx_int_t
ngx_json_unescape_string(ngx_str_t *dst, ngx_str_t *src)
{
    ಗಾತ್ರ_t       ಗಾತ್ರ_2;
    u_char      *d, *s, ch;
    uint32_t     codepoint, ಮೇಲಿನ_surrogate;
    ngx_int_t    n;
    ngx_uint_t   hex_ಎಡ;

    ಎಣಿಕೆ {
        sw_usual = 0,
        sw_quoted,
        sw_hex,
        sw_surrogate_ಆರಂಭ,   /* expect '\' of \uDCxx after high surrogate */
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

    ಆದರೆ (src->len == 0) {
        dst->len = 0;
        ಹಿಂತಿರುಗಿ NGX_OK;
    }

    ಆದರೆ (dst->len < src->len) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    s = src->ದತ್ತಾಂಶ;
    d = dst->ದತ್ತಾಂಶ;
    ಗಾತ್ರ_2 = src->len;


    state = sw_usual;
    codepoint = 0;
    ಮೇಲಿನ_surrogate = 0;
    hex_ಎಡ = 0;

    ಆಗ (ಗಾತ್ರ_2--) {

        ch = *s++;

        ಆಯ್ಕೆ (state) {
        ಸ್ಥಿತಿ sw_usual:

            ಆದರೆ (ch == '"') {
                ಹಿಂತಿರುಗಿ NGX_ERROR;
            }

            ಆದರೆ (ch == '\\') {
                state = sw_quoted;
                ನಿಲ್ಲಿಸು;
            }

            ಆದರೆ (ch < 0x20) {
                /* RFC 8259: control characters must be escaped */
                ಹಿಂತಿರುಗಿ NGX_ERROR;
            }

            *d++ = ch;
            ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ sw_quoted:

            ಆಯ್ಕೆ (ch) {

            ಸ್ಥಿತಿ 'u':
                codepoint = 0;
                hex_ಎಡ = 4;
                state = sw_hex;
                ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ '"':
            ಸ್ಥಿತಿ '/':
            ಸ್ಥಿತಿ '\\':
                *d++ = ch;
                state = sw_usual;
                ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ 'b':
                *d++ = '\b';
                state = sw_usual;
                ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ 'f':
                *d++ = '\f';
                state = sw_usual;
                ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ 'n':
                *d++ = '\n';
                state = sw_usual;
                ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ 'r':
                *d++ = '\r';
                state = sw_usual;
                ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ 't':
                *d++ = '\t';
                state = sw_usual;
                ನಿಲ್ಲಿಸು;

            ಪೂರ್ವನಿಯೋಜಿತ:
                ಹಿಂತಿರುಗಿ NGX_ERROR;
            }

            ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ sw_hex:

            n = ngx_json_hex_digit(ch);
            ಆದರೆ (n == NGX_ERROR) {
                ಹಿಂತಿರುಗಿ NGX_ERROR;
            }

            codepoint = (codepoint << 4) | (uint32_t) n;

            ಆದರೆ (--hex_ಎಡ > 0) {
                ನಿಲ್ಲಿಸು;
            }

            ಆದರೆ (ಮೇಲಿನ_surrogate) {
                ಆದರೆ (codepoint < 0xDC00 || codepoint > 0xDFFF) {
                    ಹಿಂತಿರುಗಿ NGX_ERROR;
                }

                codepoint = 0x10000
                            + ((ಮೇಲಿನ_surrogate - 0xD800) << 10)
                            + (codepoint - 0xDC00);
                d = ngx_json_utf8_encode(d, codepoint);
                ಮೇಲಿನ_surrogate = 0;
                state = sw_usual;
                ನಿಲ್ಲಿಸು;
            }

            ಆದರೆ (codepoint >= 0xD800 && codepoint <= 0xDBFF) {
                /* high surrogate - wait for the low surrogate */
                ಮೇಲಿನ_surrogate = codepoint;
                state = sw_surrogate_ಆರಂಭ;
                ನಿಲ್ಲಿಸು;
            }

            ಆದರೆ (codepoint >= 0xDC00 && codepoint <= 0xDFFF) {
                /* lone low surrogate */
                ಹಿಂತಿರುಗಿ NGX_ERROR;
            }

            d = ngx_json_utf8_encode(d, codepoint);
            state = sw_usual;
            ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ sw_surrogate_ಆರಂಭ:
            /*
             * Expect '\' to begin the low-surrogate escape.
             * Lone surrogates are not valid Unicode scalar values
             * (RFC 8259, Section 8.2).
             */
            ಆದರೆ (ch == '\\') {
                state = sw_surrogate_u;
                ನಿಲ್ಲಿಸು;
            }

            /* lone high surrogate */
            ಹಿಂತಿರುಗಿ NGX_ERROR;

        ಸ್ಥಿತಿ sw_surrogate_u:
            ಆದರೆ (ch == 'u') {
                codepoint = 0;
                hex_ಎಡ = 4;
                state = sw_hex;
                ನಿಲ್ಲಿಸು;
            }

            /* lone high surrogate */
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }
    }

    ಆದರೆ (state != sw_usual) {
        /* truncated escape sequence or unpaired high surrogate */
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    dst->len = d - dst->ದತ್ತಾಂಶ;

    ಹಿಂತಿರುಗಿ NGX_OK;
}


ಸ್ಥಿರ ngx_inline u_char *
ngx_json_utf8_encode(u_char *dst, uint32_t codepoint)
{
    ಆದರೆ (codepoint <= 0x7F) {
        *dst++ = (u_char) codepoint;

    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (codepoint <= 0x7FF) {
        *dst++ = (u_char) (0xC0 | (codepoint >> 6));
        *dst++ = (u_char) (0x80 | (codepoint & 0x3F));

    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (codepoint <= 0xFFFF) {
        *dst++ = (u_char) (0xE0 | (codepoint >> 12));
        *dst++ = (u_char) (0x80 | ((codepoint >> 6) & 0x3F));
        *dst++ = (u_char) (0x80 | (codepoint & 0x3F));

    } ಇಲ್ಲದಿದ್ದರೆ {
        *dst++ = (u_char) (0xF0 | (codepoint >> 18));
        *dst++ = (u_char) (0x80 | ((codepoint >> 12) & 0x3F));
        *dst++ = (u_char) (0x80 | ((codepoint >> 6) & 0x3F));
        *dst++ = (u_char) (0x80 | (codepoint & 0x3F));
    }

    ಹಿಂತಿರುಗಿ dst;
}


ngx_int_t
ngx_json_hex_digit(u_char ch)
{
    ಆದರೆ (ch >= '0' && ch <= '9') {
        ಹಿಂತಿರುಗಿ ch - '0';
    }

    ch = (u_char) (ch | 0x20);

    ಆದರೆ (ch >= 'a' && ch <= 'f') {
        ಹಿಂತಿರುಗಿ ch - 'a' + 10;
    }

    ಹಿಂತಿರುಗಿ NGX_ERROR;
}
