
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
    uint32_t     codepoint, वरचा_surrogate;
    ngx_int_t    n;
    ngx_uint_t   hex_डावा;

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

    जर (src->len == 0) {
        dst->len = 0;
        परत NGX_OK;
    }

    जर (dst->len < src->len) {
        परत NGX_ERROR;
    }

    s = src->data;
    d = dst->data;
    size = src->len;


    state = sw_usual;
    codepoint = 0;
    वरचा_surrogate = 0;
    hex_डावा = 0;

    पर्यंत (size--) {

        ch = *s++;

        निवड (state) {
        स्थिती sw_usual:

            जर (ch == '"') {
                परत NGX_ERROR;
            }

            जर (ch == '\\') {
                state = sw_quoted;
                थांबा;
            }

            जर (ch < 0x20) {
                /* RFC 8259: control characters must be escaped */
                परत NGX_ERROR;
            }

            *d++ = ch;
            थांबा;

        स्थिती sw_quoted:

            निवड (ch) {

            स्थिती 'u':
                codepoint = 0;
                hex_डावा = 4;
                state = sw_hex;
                थांबा;

            स्थिती '"':
            स्थिती '/':
            स्थिती '\\':
                *d++ = ch;
                state = sw_usual;
                थांबा;

            स्थिती 'b':
                *d++ = '\b';
                state = sw_usual;
                थांबा;

            स्थिती 'f':
                *d++ = '\f';
                state = sw_usual;
                थांबा;

            स्थिती 'n':
                *d++ = '\n';
                state = sw_usual;
                थांबा;

            स्थिती 'r':
                *d++ = '\r';
                state = sw_usual;
                थांबा;

            स्थिती 't':
                *d++ = '\t';
                state = sw_usual;
                थांबा;

            पूर्वनिर्धारित:
                परत NGX_ERROR;
            }

            थांबा;

        स्थिती sw_hex:

            n = ngx_json_hex_digit(ch);
            जर (n == NGX_ERROR) {
                परत NGX_ERROR;
            }

            codepoint = (codepoint << 4) | (uint32_t) n;

            जर (--hex_डावा > 0) {
                थांबा;
            }

            जर (वरचा_surrogate) {
                जर (codepoint < 0xDC00 || codepoint > 0xDFFF) {
                    परत NGX_ERROR;
                }

                codepoint = 0x10000
                            + ((वरचा_surrogate - 0xD800) << 10)
                            + (codepoint - 0xDC00);
                d = ngx_json_utf8_encode(d, codepoint);
                वरचा_surrogate = 0;
                state = sw_usual;
                थांबा;
            }

            जर (codepoint >= 0xD800 && codepoint <= 0xDBFF) {
                /* high surrogate - wait for the low surrogate */
                वरचा_surrogate = codepoint;
                state = sw_surrogate_start;
                थांबा;
            }

            जर (codepoint >= 0xDC00 && codepoint <= 0xDFFF) {
                /* lone low surrogate */
                परत NGX_ERROR;
            }

            d = ngx_json_utf8_encode(d, codepoint);
            state = sw_usual;
            थांबा;

        स्थिती sw_surrogate_start:
            /*
             * Expect '\' to begin the low-surrogate escape.
             * Lone surrogates are not valid Unicode scalar values
             * (RFC 8259, Section 8.2).
             */
            जर (ch == '\\') {
                state = sw_surrogate_u;
                थांबा;
            }

            /* lone high surrogate */
            परत NGX_ERROR;

        स्थिती sw_surrogate_u:
            जर (ch == 'u') {
                codepoint = 0;
                hex_डावा = 4;
                state = sw_hex;
                थांबा;
            }

            /* lone high surrogate */
            परत NGX_ERROR;
        }
    }

    जर (state != sw_usual) {
        /* truncated escape sequence or unpaired high surrogate */
        परत NGX_ERROR;
    }

    dst->len = d - dst->data;

    परत NGX_OK;
}


स्थिर ngx_inline u_char *
ngx_json_utf8_encode(u_char *dst, uint32_t codepoint)
{
    जर (codepoint <= 0x7F) {
        *dst++ = (u_char) codepoint;

    } अन्यथा जर (codepoint <= 0x7FF) {
        *dst++ = (u_char) (0xC0 | (codepoint >> 6));
        *dst++ = (u_char) (0x80 | (codepoint & 0x3F));

    } अन्यथा जर (codepoint <= 0xFFFF) {
        *dst++ = (u_char) (0xE0 | (codepoint >> 12));
        *dst++ = (u_char) (0x80 | ((codepoint >> 6) & 0x3F));
        *dst++ = (u_char) (0x80 | (codepoint & 0x3F));

    } अन्यथा {
        *dst++ = (u_char) (0xF0 | (codepoint >> 18));
        *dst++ = (u_char) (0x80 | ((codepoint >> 12) & 0x3F));
        *dst++ = (u_char) (0x80 | ((codepoint >> 6) & 0x3F));
        *dst++ = (u_char) (0x80 | (codepoint & 0x3F));
    }

    परत dst;
}


ngx_int_t
ngx_json_hex_digit(u_char ch)
{
    जर (ch >= '0' && ch <= '9') {
        परत ch - '0';
    }

    ch = (u_char) (ch | 0x20);

    जर (ch >= 'a' && ch <= 'f') {
        परत ch - 'a' + 10;
    }

    परत NGX_ERROR;
}
