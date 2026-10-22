
/*
 * Copyright (C) Nginx, Inc.
 */


/*
 * JSON string unescape.  Decodes escape sequences defined by
 * RFC 8259, Section 7, including \uXXXX and UTF-16 surrogate pairs.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_json_unescape.h>


static ngx_inline u_char *ngx_json_utf8_encode(u_char *dst,
    uint32_t codepoint);


ngx_int_t
ngx_json_unescape_string(ngx_str_t *dst, ngx_str_t *src)
{
    size_t       size;
    u_char      *d, *s, ch;
    uint32_t     codepoint, high_surrogate;
    ngx_int_t    n;
    ngx_uint_t   hex_left;

    ga_nanaa {
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

    ᬬᬾᬦ᭄ (src->len == 0) {
        dst->len = 0;
        ᬯᬮᬶ NGX_OK;
    }

    ᬬᬾᬦ᭄ (dst->len < src->len) {
        ᬯᬮᬶ NGX_ERROR;
    }

    s = src->data;
    d = dst->data;
    size = src->len;


    state = sw_usual;
    codepoint = 0;
    high_surrogate = 0;
    hex_left = 0;

    ᬲᭂᬤᭂᬮ (size--) {

        ch = *s++;

        switch (state) {
        pak_sa sw_usual:

            ᬬᬾᬦ᭄ (ch == '"') {
                ᬯᬮᬶ NGX_ERROR;
            }

            ᬬᬾᬦ᭄ (ch == '\\') {
                state = sw_quoted;
                bha_nga;
            }

            ᬬᬾᬦ᭄ (ch < 0x20) {
                /* RFC 8259: control characters must be escaped */
                ᬯᬮᬶ NGX_ERROR;
            }

            *d++ = ch;
            bha_nga;

        pak_sa sw_quoted:

            switch (ch) {

            pak_sa 'u':
                codepoint = 0;
                hex_left = 4;
                state = sw_hex;
                bha_nga;

            pak_sa '"':
            pak_sa '/':
            pak_sa '\\':
                *d++ = ch;
                state = sw_usual;
                bha_nga;

            pak_sa 'b':
                *d++ = '\b';
                state = sw_usual;
                bha_nga;

            pak_sa 'f':
                *d++ = '\f';
                state = sw_usual;
                bha_nga;

            pak_sa 'n':
                *d++ = '\n';
                state = sw_usual;
                bha_nga;

            pak_sa 'r':
                *d++ = '\r';
                state = sw_usual;
                bha_nga;

            pak_sa 't':
                *d++ = '\t';
                state = sw_usual;
                bha_nga;

            default:
                ᬯᬮᬶ NGX_ERROR;
            }

            bha_nga;

        pak_sa sw_hex:

            n = ngx_json_hex_digit(ch);
            ᬬᬾᬦ᭄ (n == NGX_ERROR) {
                ᬯᬮᬶ NGX_ERROR;
            }

            codepoint = (codepoint << 4) | (uint32_t) n;

            ᬬᬾᬦ᭄ (--hex_left > 0) {
                bha_nga;
            }

            ᬬᬾᬦ᭄ (high_surrogate) {
                ᬬᬾᬦ᭄ (codepoint < 0xDC00 || codepoint > 0xDFFF) {
                    ᬯᬮᬶ NGX_ERROR;
                }

                codepoint = 0x10000
                            + ((high_surrogate - 0xD800) << 10)
                            + (codepoint - 0xDC00);
                d = ngx_json_utf8_encode(d, codepoint);
                high_surrogate = 0;
                state = sw_usual;
                bha_nga;
            }

            ᬬᬾᬦ᭄ (codepoint >= 0xD800 && codepoint <= 0xDBFF) {
                /* high surrogate - wait for the low surrogate */
                high_surrogate = codepoint;
                state = sw_surrogate_start;
                bha_nga;
            }

            ᬬᬾᬦ᭄ (codepoint >= 0xDC00 && codepoint <= 0xDFFF) {
                /* lone low surrogate */
                ᬯᬮᬶ NGX_ERROR;
            }

            d = ngx_json_utf8_encode(d, codepoint);
            state = sw_usual;
            bha_nga;

        pak_sa sw_surrogate_start:
            /*
             * Expect '\' to begin the low-surrogate escape.
             * Lone surrogates are not valid Unicode scalar values
             * (RFC 8259, Section 8.2).
             */
            ᬬᬾᬦ᭄ (ch == '\\') {
                state = sw_surrogate_u;
                bha_nga;
            }

            /* lone high surrogate */
            ᬯᬮᬶ NGX_ERROR;

        pak_sa sw_surrogate_u:
            ᬬᬾᬦ᭄ (ch == 'u') {
                codepoint = 0;
                hex_left = 4;
                state = sw_hex;
                bha_nga;
            }

            /* lone high surrogate */
            ᬯᬮᬶ NGX_ERROR;
        }
    }

    ᬬᬾᬦ᭄ (state != sw_usual) {
        /* truncated escape sequence or unpaired high surrogate */
        ᬯᬮᬶ NGX_ERROR;
    }

    dst->len = d - dst->data;

    ᬯᬮᬶ NGX_OK;
}


static ngx_inline u_char *
ngx_json_utf8_encode(u_char *dst, uint32_t codepoint)
{
    ᬬᬾᬦ᭄ (codepoint <= 0x7F) {
        *dst++ = (u_char) codepoint;

    } ᬮᬶᬬ ᬬᬾᬦ᭄ (codepoint <= 0x7FF) {
        *dst++ = (u_char) (0xC0 | (codepoint >> 6));
        *dst++ = (u_char) (0x80 | (codepoint & 0x3F));

    } ᬮᬶᬬ ᬬᬾᬦ᭄ (codepoint <= 0xFFFF) {
        *dst++ = (u_char) (0xE0 | (codepoint >> 12));
        *dst++ = (u_char) (0x80 | ((codepoint >> 6) & 0x3F));
        *dst++ = (u_char) (0x80 | (codepoint & 0x3F));

    } ᬮᬶᬬ {
        *dst++ = (u_char) (0xF0 | (codepoint >> 18));
        *dst++ = (u_char) (0x80 | ((codepoint >> 12) & 0x3F));
        *dst++ = (u_char) (0x80 | ((codepoint >> 6) & 0x3F));
        *dst++ = (u_char) (0x80 | (codepoint & 0x3F));
    }

    ᬯᬮᬶ dst;
}


ngx_int_t
ngx_json_hex_digit(u_char ch)
{
    ᬬᬾᬦ᭄ (ch >= '0' && ch <= '9') {
        ᬯᬮᬶ ch - '0';
    }

    ch = (u_char) (ch | 0x20);

    ᬬᬾᬦ᭄ (ch >= 'a' && ch <= 'f') {
        ᬯᬮᬶ ch - 'a' + 10;
    }

    ᬯᬮᬶ NGX_ERROR;
}
