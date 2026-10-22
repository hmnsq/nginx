
/*
 * Copyright (C) Nginx, Inc.
 */


/*
 * JSON string unescape.  Decodes escape sequences defined by
 * RFC 8259, Section 7, including \uXXXX and UTF-16 surrogate pairs.
 */


#அந்தர்பாவ <ngx_config.h>
#அந்தர்பாவ <ngx_core.h>
#அந்தர்பாவ <ngx_json_unescape.h>


நிலையான ngx_inline u_char *ngx_json_utf8_encode(u_char *dst,
    uint32_t codepoint);


ngx_int_t
ngx_json_unescape_string(ngx_str_t *dst, ngx_str_t *src)
{
    size_t       size;
    u_char      *d, *s, ch;
    uint32_t     codepoint, உயர்வு_surrogate;
    ngx_int_t    n;
    ngx_uint_t   hex_இடது;

    எண்ணிக்கை {
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

    எனில் (src->len == 0) {
        dst->len = 0;
        திரும்பு NGX_OK;
    }

    எனில் (dst->len < src->len) {
        திரும்பு NGX_ERROR;
    }

    s = src->data;
    d = dst->data;
    size = src->len;


    state = sw_usual;
    codepoint = 0;
    உயர்வு_surrogate = 0;
    hex_இடது = 0;

    போது (size--) {

        ch = *s++;

        தேர்வு (state) {
        நிலை sw_usual:

            எனில் (ch == '"') {
                திரும்பு NGX_ERROR;
            }

            எனில் (ch == '\\') {
                state = sw_quoted;
                நிறுத்து;
            }

            எனில் (ch < 0x20) {
                /* RFC 8259: control characters must be escaped */
                திரும்பு NGX_ERROR;
            }

            *d++ = ch;
            நிறுத்து;

        நிலை sw_quoted:

            தேர்வு (ch) {

            நிலை 'u':
                codepoint = 0;
                hex_இடது = 4;
                state = sw_hex;
                நிறுத்து;

            நிலை '"':
            நிலை '/':
            நிலை '\\':
                *d++ = ch;
                state = sw_usual;
                நிறுத்து;

            நிலை 'b':
                *d++ = '\b';
                state = sw_usual;
                நிறுத்து;

            நிலை 'f':
                *d++ = '\f';
                state = sw_usual;
                நிறுத்து;

            நிலை 'n':
                *d++ = '\n';
                state = sw_usual;
                நிறுத்து;

            நிலை 'r':
                *d++ = '\r';
                state = sw_usual;
                நிறுத்து;

            நிலை 't':
                *d++ = '\t';
                state = sw_usual;
                நிறுத்து;

            இயல்பு:
                திரும்பு NGX_ERROR;
            }

            நிறுத்து;

        நிலை sw_hex:

            n = ngx_json_hex_digit(ch);
            எனில் (n == NGX_ERROR) {
                திரும்பு NGX_ERROR;
            }

            codepoint = (codepoint << 4) | (uint32_t) n;

            எனில் (--hex_இடது > 0) {
                நிறுத்து;
            }

            எனில் (உயர்வு_surrogate) {
                எனில் (codepoint < 0xDC00 || codepoint > 0xDFFF) {
                    திரும்பு NGX_ERROR;
                }

                codepoint = 0x10000
                            + ((உயர்வு_surrogate - 0xD800) << 10)
                            + (codepoint - 0xDC00);
                d = ngx_json_utf8_encode(d, codepoint);
                உயர்வு_surrogate = 0;
                state = sw_usual;
                நிறுத்து;
            }

            எனில் (codepoint >= 0xD800 && codepoint <= 0xDBFF) {
                /* high surrogate - wait for the low surrogate */
                உயர்வு_surrogate = codepoint;
                state = sw_surrogate_start;
                நிறுத்து;
            }

            எனில் (codepoint >= 0xDC00 && codepoint <= 0xDFFF) {
                /* lone low surrogate */
                திரும்பு NGX_ERROR;
            }

            d = ngx_json_utf8_encode(d, codepoint);
            state = sw_usual;
            நிறுத்து;

        நிலை sw_surrogate_start:
            /*
             * Expect '\' to begin the low-surrogate escape.
             * Lone surrogates are not valid Unicode scalar values
             * (RFC 8259, Section 8.2).
             */
            எனில் (ch == '\\') {
                state = sw_surrogate_u;
                நிறுத்து;
            }

            /* lone high surrogate */
            திரும்பு NGX_ERROR;

        நிலை sw_surrogate_u:
            எனில் (ch == 'u') {
                codepoint = 0;
                hex_இடது = 4;
                state = sw_hex;
                நிறுத்து;
            }

            /* lone high surrogate */
            திரும்பு NGX_ERROR;
        }
    }

    எனில் (state != sw_usual) {
        /* truncated escape sequence or unpaired high surrogate */
        திரும்பு NGX_ERROR;
    }

    dst->len = d - dst->data;

    திரும்பு NGX_OK;
}


நிலையான ngx_inline u_char *
ngx_json_utf8_encode(u_char *dst, uint32_t codepoint)
{
    எனில் (codepoint <= 0x7F) {
        *dst++ = (u_char) codepoint;

    } இல்லையெனில் எனில் (codepoint <= 0x7FF) {
        *dst++ = (u_char) (0xC0 | (codepoint >> 6));
        *dst++ = (u_char) (0x80 | (codepoint & 0x3F));

    } இல்லையெனில் எனில் (codepoint <= 0xFFFF) {
        *dst++ = (u_char) (0xE0 | (codepoint >> 12));
        *dst++ = (u_char) (0x80 | ((codepoint >> 6) & 0x3F));
        *dst++ = (u_char) (0x80 | (codepoint & 0x3F));

    } இல்லையெனில் {
        *dst++ = (u_char) (0xF0 | (codepoint >> 18));
        *dst++ = (u_char) (0x80 | ((codepoint >> 12) & 0x3F));
        *dst++ = (u_char) (0x80 | ((codepoint >> 6) & 0x3F));
        *dst++ = (u_char) (0x80 | (codepoint & 0x3F));
    }

    திரும்பு dst;
}


ngx_int_t
ngx_json_hex_digit(u_char ch)
{
    எனில் (ch >= '0' && ch <= '9') {
        திரும்பு ch - '0';
    }

    ch = (u_char) (ch | 0x20);

    எனில் (ch >= 'a' && ch <= 'f') {
        திரும்பு ch - 'a' + 10;
    }

    திரும்பு NGX_ERROR;
}
