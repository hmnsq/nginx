
/*
 * Copyright (C) Nginx, Inc.
 */


/*
 * JSON string unescape.  Decodes escape sequences defined by
 * RFC 8259, Section 7, including \uXXXX and UTF-16 surrogate pairs.
 */


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_json_unescape.h>


ਸਥਿਰ ngx_inline u_char *ngx_json_utf8_encode(u_char *dst,
    uint32_t codepoint);


ngx_int_t
ngx_json_unescape_string(ngx_str_t *dst, ngx_str_t *src)
{
    ਮਾਪ_t       ਮਾਪ;
    u_char      *d, *s, ch;
    uint32_t     codepoint, ਉੱਚਾ_surrogate;
    ngx_int_t    n;
    ngx_uint_t   hex_ਖੱਬਾ;

    ਗਿਣਤੀ {
        sw_usual = 0,
        sw_quoted,
        sw_hex,
        sw_surrogate_ਸ਼ੁਰੂ,   /* expect '\' of \uDCxx after high surrogate */
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

    ਜੇ (src->len == 0) {
        dst->len = 0;
        ਵਾਪਸ NGX_OK;
    }

    ਜੇ (dst->len < src->len) {
        ਵਾਪਸ NGX_ERROR;
    }

    s = src->ਅੰਕੜੇ;
    d = dst->ਅੰਕੜੇ;
    ਮਾਪ = src->len;


    state = sw_usual;
    codepoint = 0;
    ਉੱਚਾ_surrogate = 0;
    hex_ਖੱਬਾ = 0;

    ਜਦੋਂ (ਮਾਪ--) {

        ch = *s++;

        ਚੋਣ (state) {
        ਹਾਲਤ sw_usual:

            ਜੇ (ch == '"') {
                ਵਾਪਸ NGX_ERROR;
            }

            ਜੇ (ch == '\\') {
                state = sw_quoted;
                ਰੋਕੋ;
            }

            ਜੇ (ch < 0x20) {
                /* RFC 8259: control characters must be escaped */
                ਵਾਪਸ NGX_ERROR;
            }

            *d++ = ch;
            ਰੋਕੋ;

        ਹਾਲਤ sw_quoted:

            ਚੋਣ (ch) {

            ਹਾਲਤ 'u':
                codepoint = 0;
                hex_ਖੱਬਾ = 4;
                state = sw_hex;
                ਰੋਕੋ;

            ਹਾਲਤ '"':
            ਹਾਲਤ '/':
            ਹਾਲਤ '\\':
                *d++ = ch;
                state = sw_usual;
                ਰੋਕੋ;

            ਹਾਲਤ 'b':
                *d++ = '\b';
                state = sw_usual;
                ਰੋਕੋ;

            ਹਾਲਤ 'f':
                *d++ = '\f';
                state = sw_usual;
                ਰੋਕੋ;

            ਹਾਲਤ 'n':
                *d++ = '\n';
                state = sw_usual;
                ਰੋਕੋ;

            ਹਾਲਤ 'r':
                *d++ = '\r';
                state = sw_usual;
                ਰੋਕੋ;

            ਹਾਲਤ 't':
                *d++ = '\t';
                state = sw_usual;
                ਰੋਕੋ;

            ਮੂਲ:
                ਵਾਪਸ NGX_ERROR;
            }

            ਰੋਕੋ;

        ਹਾਲਤ sw_hex:

            n = ngx_json_hex_digit(ch);
            ਜੇ (n == NGX_ERROR) {
                ਵਾਪਸ NGX_ERROR;
            }

            codepoint = (codepoint << 4) | (uint32_t) n;

            ਜੇ (--hex_ਖੱਬਾ > 0) {
                ਰੋਕੋ;
            }

            ਜੇ (ਉੱਚਾ_surrogate) {
                ਜੇ (codepoint < 0xDC00 || codepoint > 0xDFFF) {
                    ਵਾਪਸ NGX_ERROR;
                }

                codepoint = 0x10000
                            + ((ਉੱਚਾ_surrogate - 0xD800) << 10)
                            + (codepoint - 0xDC00);
                d = ngx_json_utf8_encode(d, codepoint);
                ਉੱਚਾ_surrogate = 0;
                state = sw_usual;
                ਰੋਕੋ;
            }

            ਜੇ (codepoint >= 0xD800 && codepoint <= 0xDBFF) {
                /* high surrogate - wait for the low surrogate */
                ਉੱਚਾ_surrogate = codepoint;
                state = sw_surrogate_ਸ਼ੁਰੂ;
                ਰੋਕੋ;
            }

            ਜੇ (codepoint >= 0xDC00 && codepoint <= 0xDFFF) {
                /* lone low surrogate */
                ਵਾਪਸ NGX_ERROR;
            }

            d = ngx_json_utf8_encode(d, codepoint);
            state = sw_usual;
            ਰੋਕੋ;

        ਹਾਲਤ sw_surrogate_ਸ਼ੁਰੂ:
            /*
             * Expect '\' to begin the low-surrogate escape.
             * Lone surrogates are not valid Unicode scalar values
             * (RFC 8259, Section 8.2).
             */
            ਜੇ (ch == '\\') {
                state = sw_surrogate_u;
                ਰੋਕੋ;
            }

            /* lone high surrogate */
            ਵਾਪਸ NGX_ERROR;

        ਹਾਲਤ sw_surrogate_u:
            ਜੇ (ch == 'u') {
                codepoint = 0;
                hex_ਖੱਬਾ = 4;
                state = sw_hex;
                ਰੋਕੋ;
            }

            /* lone high surrogate */
            ਵਾਪਸ NGX_ERROR;
        }
    }

    ਜੇ (state != sw_usual) {
        /* truncated escape sequence or unpaired high surrogate */
        ਵਾਪਸ NGX_ERROR;
    }

    dst->len = d - dst->ਅੰਕੜੇ;

    ਵਾਪਸ NGX_OK;
}


ਸਥਿਰ ngx_inline u_char *
ngx_json_utf8_encode(u_char *dst, uint32_t codepoint)
{
    ਜੇ (codepoint <= 0x7F) {
        *dst++ = (u_char) codepoint;

    } ਨਹੀਂਤਾਂ ਜੇ (codepoint <= 0x7FF) {
        *dst++ = (u_char) (0xC0 | (codepoint >> 6));
        *dst++ = (u_char) (0x80 | (codepoint & 0x3F));

    } ਨਹੀਂਤਾਂ ਜੇ (codepoint <= 0xFFFF) {
        *dst++ = (u_char) (0xE0 | (codepoint >> 12));
        *dst++ = (u_char) (0x80 | ((codepoint >> 6) & 0x3F));
        *dst++ = (u_char) (0x80 | (codepoint & 0x3F));

    } ਨਹੀਂਤਾਂ {
        *dst++ = (u_char) (0xF0 | (codepoint >> 18));
        *dst++ = (u_char) (0x80 | ((codepoint >> 12) & 0x3F));
        *dst++ = (u_char) (0x80 | ((codepoint >> 6) & 0x3F));
        *dst++ = (u_char) (0x80 | (codepoint & 0x3F));
    }

    ਵਾਪਸ dst;
}


ngx_int_t
ngx_json_hex_digit(u_char ch)
{
    ਜੇ (ch >= '0' && ch <= '9') {
        ਵਾਪਸ ch - '0';
    }

    ch = (u_char) (ch | 0x20);

    ਜੇ (ch >= 'a' && ch <= 'f') {
        ਵਾਪਸ ch - 'a' + 10;
    }

    ਵਾਪਸ NGX_ERROR;
}
