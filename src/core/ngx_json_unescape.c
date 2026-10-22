
/*
 * Copyright (C) Nginx, Inc.
 */


/*
 * JSON string unescape.  Decodes escape sequences defined by
 * RFC 8259, Section 7, including \uXXXX and UTF-16 surrogate pairs.
 */


#شامل <ngx_config.h>
#شامل <ngx_core.h>
#شامل <ngx_json_unescape.h>


ساکن ngx_inline u_char *ngx_json_utf8_encode(u_char *dst,
    uint32_t codepoint);


ngx_int_t
ngx_json_unescape_string(ngx_str_t *dst, ngx_str_t *src)
{
    سائز_t       سائز;
    u_char      *d, *s, ch;
    uint32_t     codepoint, بلند_surrogate;
    ngx_int_t    n;
    ngx_uint_t   hex_بایاں;

    شمار {
        sw_usual = 0,
        sw_quoted,
        sw_hex,
        sw_surrogate_آغاز,   /* expect '\' of \uDCxx after high surrogate */
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

    اگر (src->len == 0) {
        dst->len = 0;
        واپس NGX_OK;
    }

    اگر (dst->len < src->len) {
        واپس NGX_ERROR;
    }

    s = src->ڈیٹا;
    d = dst->ڈیٹا;
    سائز = src->len;


    state = sw_usual;
    codepoint = 0;
    بلند_surrogate = 0;
    hex_بایاں = 0;

    جب (سائز--) {

        ch = *s++;

        انتخاب (state) {
        حالت sw_usual:

            اگر (ch == '"') {
                واپس NGX_ERROR;
            }

            اگر (ch == '\\') {
                state = sw_quoted;
                توڑو;
            }

            اگر (ch < 0x20) {
                /* RFC 8259: control characters must be escaped */
                واپس NGX_ERROR;
            }

            *d++ = ch;
            توڑو;

        حالت sw_quoted:

            انتخاب (ch) {

            حالت 'u':
                codepoint = 0;
                hex_بایاں = 4;
                state = sw_hex;
                توڑو;

            حالت '"':
            حالت '/':
            حالت '\\':
                *d++ = ch;
                state = sw_usual;
                توڑو;

            حالت 'b':
                *d++ = '\b';
                state = sw_usual;
                توڑو;

            حالت 'f':
                *d++ = '\f';
                state = sw_usual;
                توڑو;

            حالت 'n':
                *d++ = '\n';
                state = sw_usual;
                توڑو;

            حالت 'r':
                *d++ = '\r';
                state = sw_usual;
                توڑو;

            حالت 't':
                *d++ = '\t';
                state = sw_usual;
                توڑو;

            طےشدہ:
                واپس NGX_ERROR;
            }

            توڑو;

        حالت sw_hex:

            n = ngx_json_hex_digit(ch);
            اگر (n == NGX_ERROR) {
                واپس NGX_ERROR;
            }

            codepoint = (codepoint << 4) | (uint32_t) n;

            اگر (--hex_بایاں > 0) {
                توڑو;
            }

            اگر (بلند_surrogate) {
                اگر (codepoint < 0xDC00 || codepoint > 0xDFFF) {
                    واپس NGX_ERROR;
                }

                codepoint = 0x10000
                            + ((بلند_surrogate - 0xD800) << 10)
                            + (codepoint - 0xDC00);
                d = ngx_json_utf8_encode(d, codepoint);
                بلند_surrogate = 0;
                state = sw_usual;
                توڑو;
            }

            اگر (codepoint >= 0xD800 && codepoint <= 0xDBFF) {
                /* high surrogate - wait for the low surrogate */
                بلند_surrogate = codepoint;
                state = sw_surrogate_آغاز;
                توڑو;
            }

            اگر (codepoint >= 0xDC00 && codepoint <= 0xDFFF) {
                /* lone low surrogate */
                واپس NGX_ERROR;
            }

            d = ngx_json_utf8_encode(d, codepoint);
            state = sw_usual;
            توڑو;

        حالت sw_surrogate_آغاز:
            /*
             * Expect '\' to begin the low-surrogate escape.
             * Lone surrogates are not valid Unicode scalar values
             * (RFC 8259, Section 8.2).
             */
            اگر (ch == '\\') {
                state = sw_surrogate_u;
                توڑو;
            }

            /* lone high surrogate */
            واپس NGX_ERROR;

        حالت sw_surrogate_u:
            اگر (ch == 'u') {
                codepoint = 0;
                hex_بایاں = 4;
                state = sw_hex;
                توڑو;
            }

            /* lone high surrogate */
            واپس NGX_ERROR;
        }
    }

    اگر (state != sw_usual) {
        /* truncated escape sequence or unpaired high surrogate */
        واپس NGX_ERROR;
    }

    dst->len = d - dst->ڈیٹا;

    واپس NGX_OK;
}


ساکن ngx_inline u_char *
ngx_json_utf8_encode(u_char *dst, uint32_t codepoint)
{
    اگر (codepoint <= 0x7F) {
        *dst++ = (u_char) codepoint;

    } ورنہ اگر (codepoint <= 0x7FF) {
        *dst++ = (u_char) (0xC0 | (codepoint >> 6));
        *dst++ = (u_char) (0x80 | (codepoint & 0x3F));

    } ورنہ اگر (codepoint <= 0xFFFF) {
        *dst++ = (u_char) (0xE0 | (codepoint >> 12));
        *dst++ = (u_char) (0x80 | ((codepoint >> 6) & 0x3F));
        *dst++ = (u_char) (0x80 | (codepoint & 0x3F));

    } ورنہ {
        *dst++ = (u_char) (0xF0 | (codepoint >> 18));
        *dst++ = (u_char) (0x80 | ((codepoint >> 12) & 0x3F));
        *dst++ = (u_char) (0x80 | ((codepoint >> 6) & 0x3F));
        *dst++ = (u_char) (0x80 | (codepoint & 0x3F));
    }

    واپس dst;
}


ngx_int_t
ngx_json_hex_digit(u_char ch)
{
    اگر (ch >= '0' && ch <= '9') {
        واپس ch - '0';
    }

    ch = (u_char) (ch | 0x20);

    اگر (ch >= 'a' && ch <= 'f') {
        واپس ch - 'a' + 10;
    }

    واپس NGX_ERROR;
}
