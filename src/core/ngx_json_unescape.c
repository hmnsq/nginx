
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


ایستا ngx_inline u_char *ngx_json_utf8_encode(u_char *dst,
    uint32_t codepoint);


ngx_int_t
ngx_json_unescape_string(ngx_str_t *dst, ngx_str_t *src)
{
    size_t       size;
    u_char      *d, *s, ch;
    uint32_t     codepoint, بالا_surrogate;
    ngx_int_t    n;
    ngx_uint_t   hex_چپ;

    شمارش {
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

    اگر (src->len == 0) {
        dst->len = 0;
        بازگشت NGX_OK;
    }

    اگر (dst->len < src->len) {
        بازگشت NGX_ERROR;
    }

    s = src->data;
    d = dst->data;
    size = src->len;


    state = sw_usual;
    codepoint = 0;
    بالا_surrogate = 0;
    hex_چپ = 0;

    وقتی (size--) {

        ch = *s++;

        انتخاب (state) {
        حالت sw_usual:

            اگر (ch == '"') {
                بازگشت NGX_ERROR;
            }

            اگر (ch == '\\') {
                state = sw_quoted;
                بشکن;
            }

            اگر (ch < 0x20) {
                /* RFC 8259: control characters must be escaped */
                بازگشت NGX_ERROR;
            }

            *d++ = ch;
            بشکن;

        حالت sw_quoted:

            انتخاب (ch) {

            حالت 'u':
                codepoint = 0;
                hex_چپ = 4;
                state = sw_hex;
                بشکن;

            حالت '"':
            حالت '/':
            حالت '\\':
                *d++ = ch;
                state = sw_usual;
                بشکن;

            حالت 'b':
                *d++ = '\b';
                state = sw_usual;
                بشکن;

            حالت 'f':
                *d++ = '\f';
                state = sw_usual;
                بشکن;

            حالت 'n':
                *d++ = '\n';
                state = sw_usual;
                بشکن;

            حالت 'r':
                *d++ = '\r';
                state = sw_usual;
                بشکن;

            حالت 't':
                *d++ = '\t';
                state = sw_usual;
                بشکن;

            پیشفرض:
                بازگشت NGX_ERROR;
            }

            بشکن;

        حالت sw_hex:

            n = ngx_json_hex_digit(ch);
            اگر (n == NGX_ERROR) {
                بازگشت NGX_ERROR;
            }

            codepoint = (codepoint << 4) | (uint32_t) n;

            اگر (--hex_چپ > 0) {
                بشکن;
            }

            اگر (بالا_surrogate) {
                اگر (codepoint < 0xDC00 || codepoint > 0xDFFF) {
                    بازگشت NGX_ERROR;
                }

                codepoint = 0x10000
                            + ((بالا_surrogate - 0xD800) << 10)
                            + (codepoint - 0xDC00);
                d = ngx_json_utf8_encode(d, codepoint);
                بالا_surrogate = 0;
                state = sw_usual;
                بشکن;
            }

            اگر (codepoint >= 0xD800 && codepoint <= 0xDBFF) {
                /* high surrogate - wait for the low surrogate */
                بالا_surrogate = codepoint;
                state = sw_surrogate_start;
                بشکن;
            }

            اگر (codepoint >= 0xDC00 && codepoint <= 0xDFFF) {
                /* lone low surrogate */
                بازگشت NGX_ERROR;
            }

            d = ngx_json_utf8_encode(d, codepoint);
            state = sw_usual;
            بشکن;

        حالت sw_surrogate_start:
            /*
             * Expect '\' to begin the low-surrogate escape.
             * Lone surrogates are not valid Unicode scalar values
             * (RFC 8259, Section 8.2).
             */
            اگر (ch == '\\') {
                state = sw_surrogate_u;
                بشکن;
            }

            /* lone high surrogate */
            بازگشت NGX_ERROR;

        حالت sw_surrogate_u:
            اگر (ch == 'u') {
                codepoint = 0;
                hex_چپ = 4;
                state = sw_hex;
                بشکن;
            }

            /* lone high surrogate */
            بازگشت NGX_ERROR;
        }
    }

    اگر (state != sw_usual) {
        /* truncated escape sequence or unpaired high surrogate */
        بازگشت NGX_ERROR;
    }

    dst->len = d - dst->data;

    بازگشت NGX_OK;
}


ایستا ngx_inline u_char *
ngx_json_utf8_encode(u_char *dst, uint32_t codepoint)
{
    اگر (codepoint <= 0x7F) {
        *dst++ = (u_char) codepoint;

    } وگرنه اگر (codepoint <= 0x7FF) {
        *dst++ = (u_char) (0xC0 | (codepoint >> 6));
        *dst++ = (u_char) (0x80 | (codepoint & 0x3F));

    } وگرنه اگر (codepoint <= 0xFFFF) {
        *dst++ = (u_char) (0xE0 | (codepoint >> 12));
        *dst++ = (u_char) (0x80 | ((codepoint >> 6) & 0x3F));
        *dst++ = (u_char) (0x80 | (codepoint & 0x3F));

    } وگرنه {
        *dst++ = (u_char) (0xF0 | (codepoint >> 18));
        *dst++ = (u_char) (0x80 | ((codepoint >> 12) & 0x3F));
        *dst++ = (u_char) (0x80 | ((codepoint >> 6) & 0x3F));
        *dst++ = (u_char) (0x80 | (codepoint & 0x3F));
    }

    بازگشت dst;
}


ngx_int_t
ngx_json_hex_digit(u_char ch)
{
    اگر (ch >= '0' && ch <= '9') {
        بازگشت ch - '0';
    }

    ch = (u_char) (ch | 0x20);

    اگر (ch >= 'a' && ch <= 'f') {
        بازگشت ch - 'a' + 10;
    }

    بازگشت NGX_ERROR;
}
