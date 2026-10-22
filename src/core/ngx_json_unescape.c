
/*
 * Copyright (C) Nginx, Inc.
 */


/*
 * JSON string unescape.  Decodes escape sequences defined by
 * RFC 8259, Section 7, including \uXXXX and UTF-16 surrogate pairs.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>
#ضمن <ngx_json_unescape.h>


ساكن ngx_inline u_char *ngx_json_utf8_encode(u_char *dst,
    uint32_t codepoint);


ngx_int_t
ngx_json_unescape_string(ngx_str_t *dst, ngx_str_t *src)
{
    size_t       size;
    u_char      *d, *s, ch;
    uint32_t     codepoint, high_surrogate;
    ngx_int_t    n;
    ngx_uint_t   hex_left;

    تعداد {
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

    ܐܢ (src->len == 0) {
        dst->len = 0;
        ܦܢܐ NGX_OK;
    }

    ܐܢ (dst->len < src->len) {
        ܦܢܐ NGX_ERROR;
    }

    s = src->data;
    d = dst->data;
    size = src->len;


    state = sw_usual;
    codepoint = 0;
    high_surrogate = 0;
    hex_left = 0;

    ܟܡܐ (size--) {

        ch = *s++;

        بدل (state) {
        حالة sw_usual:

            ܐܢ (ch == '"') {
                ܦܢܐ NGX_ERROR;
            }

            ܐܢ (ch == '\\') {
                state = sw_quoted;
                اكسر;
            }

            ܐܢ (ch < 0x20) {
                /* RFC 8259: control characters must be escaped */
                ܦܢܐ NGX_ERROR;
            }

            *d++ = ch;
            اكسر;

        حالة sw_quoted:

            بدل (ch) {

            حالة 'u':
                codepoint = 0;
                hex_left = 4;
                state = sw_hex;
                اكسر;

            حالة '"':
            حالة '/':
            حالة '\\':
                *d++ = ch;
                state = sw_usual;
                اكسر;

            حالة 'b':
                *d++ = '\b';
                state = sw_usual;
                اكسر;

            حالة 'f':
                *d++ = '\f';
                state = sw_usual;
                اكسر;

            حالة 'n':
                *d++ = '\n';
                state = sw_usual;
                اكسر;

            حالة 'r':
                *d++ = '\r';
                state = sw_usual;
                اكسر;

            حالة 't':
                *d++ = '\t';
                state = sw_usual;
                اكسر;

            افتراضي:
                ܦܢܐ NGX_ERROR;
            }

            اكسر;

        حالة sw_hex:

            n = ngx_json_hex_digit(ch);
            ܐܢ (n == NGX_ERROR) {
                ܦܢܐ NGX_ERROR;
            }

            codepoint = (codepoint << 4) | (uint32_t) n;

            ܐܢ (--hex_left > 0) {
                اكسر;
            }

            ܐܢ (high_surrogate) {
                ܐܢ (codepoint < 0xDC00 || codepoint > 0xDFFF) {
                    ܦܢܐ NGX_ERROR;
                }

                codepoint = 0x10000
                            + ((high_surrogate - 0xD800) << 10)
                            + (codepoint - 0xDC00);
                d = ngx_json_utf8_encode(d, codepoint);
                high_surrogate = 0;
                state = sw_usual;
                اكسر;
            }

            ܐܢ (codepoint >= 0xD800 && codepoint <= 0xDBFF) {
                /* high surrogate - wait for the low surrogate */
                high_surrogate = codepoint;
                state = sw_surrogate_start;
                اكسر;
            }

            ܐܢ (codepoint >= 0xDC00 && codepoint <= 0xDFFF) {
                /* lone low surrogate */
                ܦܢܐ NGX_ERROR;
            }

            d = ngx_json_utf8_encode(d, codepoint);
            state = sw_usual;
            اكسر;

        حالة sw_surrogate_start:
            /*
             * Expect '\' to begin the low-surrogate escape.
             * Lone surrogates are not valid Unicode scalar values
             * (RFC 8259, Section 8.2).
             */
            ܐܢ (ch == '\\') {
                state = sw_surrogate_u;
                اكسر;
            }

            /* lone high surrogate */
            ܦܢܐ NGX_ERROR;

        حالة sw_surrogate_u:
            ܐܢ (ch == 'u') {
                codepoint = 0;
                hex_left = 4;
                state = sw_hex;
                اكسر;
            }

            /* lone high surrogate */
            ܦܢܐ NGX_ERROR;
        }
    }

    ܐܢ (state != sw_usual) {
        /* truncated escape sequence or unpaired high surrogate */
        ܦܢܐ NGX_ERROR;
    }

    dst->len = d - dst->data;

    ܦܢܐ NGX_OK;
}


ساكن ngx_inline u_char *
ngx_json_utf8_encode(u_char *dst, uint32_t codepoint)
{
    ܐܢ (codepoint <= 0x7F) {
        *dst++ = (u_char) codepoint;

    } ܐܚܪܢܐ ܐܢ (codepoint <= 0x7FF) {
        *dst++ = (u_char) (0xC0 | (codepoint >> 6));
        *dst++ = (u_char) (0x80 | (codepoint & 0x3F));

    } ܐܚܪܢܐ ܐܢ (codepoint <= 0xFFFF) {
        *dst++ = (u_char) (0xE0 | (codepoint >> 12));
        *dst++ = (u_char) (0x80 | ((codepoint >> 6) & 0x3F));
        *dst++ = (u_char) (0x80 | (codepoint & 0x3F));

    } ܐܚܪܢܐ {
        *dst++ = (u_char) (0xF0 | (codepoint >> 18));
        *dst++ = (u_char) (0x80 | ((codepoint >> 12) & 0x3F));
        *dst++ = (u_char) (0x80 | ((codepoint >> 6) & 0x3F));
        *dst++ = (u_char) (0x80 | (codepoint & 0x3F));
    }

    ܦܢܐ dst;
}


ngx_int_t
ngx_json_hex_digit(u_char ch)
{
    ܐܢ (ch >= '0' && ch <= '9') {
        ܦܢܐ ch - '0';
    }

    ch = (u_char) (ch | 0x20);

    ܐܢ (ch >= 'a' && ch <= 'f') {
        ܦܢܐ ch - 'a' + 10;
    }

    ܦܢܐ NGX_ERROR;
}
