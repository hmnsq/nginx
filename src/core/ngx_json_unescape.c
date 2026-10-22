
/*
 * Copyright (C) Nginx, Inc.
 */


/*
 * JSON string unescape.  Decodes escape sequences defined by
 * RFC 8259, Section 7, including \uXXXX and UTF-16 surrogate pairs.
 */


#অন্তর্ভাৱ <ngx_config.h>
#অন্তর্ভাৱ <ngx_core.h>
#অন্তর্ভাৱ <ngx_json_unescape.h>


স্থির ngx_inline u_char *ngx_json_utf8_encode(u_char *dst,
    uint32_t codepoint);


ngx_int_t
ngx_json_unescape_string(ngx_str_t *dst, ngx_str_t *src)
{
    size_t       size;
    u_char      *d, *s, ch;
    uint32_t     codepoint, উপৰেৰ_surrogate;
    ngx_int_t    n;
    ngx_uint_t   hex_বাম;

    গণনা {
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

    যদি (src->len == 0) {
        dst->len = 0;
        ঘূৰাই NGX_OK;
    }

    যদি (dst->len < src->len) {
        ঘূৰাই NGX_ERROR;
    }

    s = src->data;
    d = dst->data;
    size = src->len;


    state = sw_usual;
    codepoint = 0;
    উপৰেৰ_surrogate = 0;
    hex_বাম = 0;

    যেতিয়ালৈ (size--) {

        ch = *s++;

        ৱিকল্প (state) {
        পক্ষ sw_usual:

            যদি (ch == '"') {
                ঘূৰাই NGX_ERROR;
            }

            যদি (ch == '\\') {
                state = sw_quoted;
                ভঙ্গ;
            }

            যদি (ch < 0x20) {
                /* RFC 8259: control characters must be escaped */
                ঘূৰাই NGX_ERROR;
            }

            *d++ = ch;
            ভঙ্গ;

        পক্ষ sw_quoted:

            ৱিকল্প (ch) {

            পক্ষ 'u':
                codepoint = 0;
                hex_বাম = 4;
                state = sw_hex;
                ভঙ্গ;

            পক্ষ '"':
            পক্ষ '/':
            পক্ষ '\\':
                *d++ = ch;
                state = sw_usual;
                ভঙ্গ;

            পক্ষ 'b':
                *d++ = '\b';
                state = sw_usual;
                ভঙ্গ;

            পক্ষ 'f':
                *d++ = '\f';
                state = sw_usual;
                ভঙ্গ;

            পক্ষ 'n':
                *d++ = '\n';
                state = sw_usual;
                ভঙ্গ;

            পক্ষ 'r':
                *d++ = '\r';
                state = sw_usual;
                ভঙ্গ;

            পক্ষ 't':
                *d++ = '\t';
                state = sw_usual;
                ভঙ্গ;

            স্ৱভাৱ:
                ঘূৰাই NGX_ERROR;
            }

            ভঙ্গ;

        পক্ষ sw_hex:

            n = ngx_json_hex_digit(ch);
            যদি (n == NGX_ERROR) {
                ঘূৰাই NGX_ERROR;
            }

            codepoint = (codepoint << 4) | (uint32_t) n;

            যদি (--hex_বাম > 0) {
                ভঙ্গ;
            }

            যদি (উপৰেৰ_surrogate) {
                যদি (codepoint < 0xDC00 || codepoint > 0xDFFF) {
                    ঘূৰাই NGX_ERROR;
                }

                codepoint = 0x10000
                            + ((উপৰেৰ_surrogate - 0xD800) << 10)
                            + (codepoint - 0xDC00);
                d = ngx_json_utf8_encode(d, codepoint);
                উপৰেৰ_surrogate = 0;
                state = sw_usual;
                ভঙ্গ;
            }

            যদি (codepoint >= 0xD800 && codepoint <= 0xDBFF) {
                /* high surrogate - wait for the low surrogate */
                উপৰেৰ_surrogate = codepoint;
                state = sw_surrogate_start;
                ভঙ্গ;
            }

            যদি (codepoint >= 0xDC00 && codepoint <= 0xDFFF) {
                /* lone low surrogate */
                ঘূৰাই NGX_ERROR;
            }

            d = ngx_json_utf8_encode(d, codepoint);
            state = sw_usual;
            ভঙ্গ;

        পক্ষ sw_surrogate_start:
            /*
             * Expect '\' to begin the low-surrogate escape.
             * Lone surrogates are not valid Unicode scalar values
             * (RFC 8259, Section 8.2).
             */
            যদি (ch == '\\') {
                state = sw_surrogate_u;
                ভঙ্গ;
            }

            /* lone high surrogate */
            ঘূৰাই NGX_ERROR;

        পক্ষ sw_surrogate_u:
            যদি (ch == 'u') {
                codepoint = 0;
                hex_বাম = 4;
                state = sw_hex;
                ভঙ্গ;
            }

            /* lone high surrogate */
            ঘূৰাই NGX_ERROR;
        }
    }

    যদি (state != sw_usual) {
        /* truncated escape sequence or unpaired high surrogate */
        ঘূৰাই NGX_ERROR;
    }

    dst->len = d - dst->data;

    ঘূৰাই NGX_OK;
}


স্থির ngx_inline u_char *
ngx_json_utf8_encode(u_char *dst, uint32_t codepoint)
{
    যদি (codepoint <= 0x7F) {
        *dst++ = (u_char) codepoint;

    } অন্যথা যদি (codepoint <= 0x7FF) {
        *dst++ = (u_char) (0xC0 | (codepoint >> 6));
        *dst++ = (u_char) (0x80 | (codepoint & 0x3F));

    } অন্যথা যদি (codepoint <= 0xFFFF) {
        *dst++ = (u_char) (0xE0 | (codepoint >> 12));
        *dst++ = (u_char) (0x80 | ((codepoint >> 6) & 0x3F));
        *dst++ = (u_char) (0x80 | (codepoint & 0x3F));

    } অন্যথা {
        *dst++ = (u_char) (0xF0 | (codepoint >> 18));
        *dst++ = (u_char) (0x80 | ((codepoint >> 12) & 0x3F));
        *dst++ = (u_char) (0x80 | ((codepoint >> 6) & 0x3F));
        *dst++ = (u_char) (0x80 | (codepoint & 0x3F));
    }

    ঘূৰাই dst;
}


ngx_int_t
ngx_json_hex_digit(u_char ch)
{
    যদি (ch >= '0' && ch <= '9') {
        ঘূৰাই ch - '0';
    }

    ch = (u_char) (ch | 0x20);

    যদি (ch >= 'a' && ch <= 'f') {
        ঘূৰাই ch - 'a' + 10;
    }

    ঘূৰাই NGX_ERROR;
}
