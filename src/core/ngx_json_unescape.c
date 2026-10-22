
/*
 * Copyright (C) Nginx, Inc.
 */


/*
 * JSON string unescape.  Decodes escape sequences defined by
 * RFC 8259, Section 7, including \uXXXX and UTF-16 surrogate pairs.
 */


#അന്തര്ഭാവ <ngx_config.h>
#അന്തര്ഭാവ <ngx_core.h>
#അന്തര്ഭാവ <ngx_json_unescape.h>


സ്ഥിതം ngx_inline u_char *ngx_json_utf8_encode(u_char *dst,
    uint32_t codepoint);


ngx_int_t
ngx_json_unescape_string(ngx_str_t *dst, ngx_str_t *src)
{
    size_t       size;
    u_char      *d, *s, ch;
    uint32_t     codepoint, മുകളിലെ_surrogate;
    ngx_int_t    n;
    ngx_uint_t   hex_ഇടത്;

    എണ്ണൽ {
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

    എങ്കിൽ (src->len == 0) {
        dst->len = 0;
        തിരികെ NGX_OK;
    }

    എങ്കിൽ (dst->len < src->len) {
        തിരികെ NGX_ERROR;
    }

    s = src->data;
    d = dst->data;
    size = src->len;


    state = sw_usual;
    codepoint = 0;
    മുകളിലെ_surrogate = 0;
    hex_ഇടത് = 0;

    ആകുമ്പോൾ (size--) {

        ch = *s++;

        തിരഞ്ഞെടുപ്പ് (state) {
        അവസ്ഥ sw_usual:

            എങ്കിൽ (ch == '"') {
                തിരികെ NGX_ERROR;
            }

            എങ്കിൽ (ch == '\\') {
                state = sw_quoted;
                നിർത്തുക;
            }

            എങ്കിൽ (ch < 0x20) {
                /* RFC 8259: control characters must be escaped */
                തിരികെ NGX_ERROR;
            }

            *d++ = ch;
            നിർത്തുക;

        അവസ്ഥ sw_quoted:

            തിരഞ്ഞെടുപ്പ് (ch) {

            അവസ്ഥ 'u':
                codepoint = 0;
                hex_ഇടത് = 4;
                state = sw_hex;
                നിർത്തുക;

            അവസ്ഥ '"':
            അവസ്ഥ '/':
            അവസ്ഥ '\\':
                *d++ = ch;
                state = sw_usual;
                നിർത്തുക;

            അവസ്ഥ 'b':
                *d++ = '\b';
                state = sw_usual;
                നിർത്തുക;

            അവസ്ഥ 'f':
                *d++ = '\f';
                state = sw_usual;
                നിർത്തുക;

            അവസ്ഥ 'n':
                *d++ = '\n';
                state = sw_usual;
                നിർത്തുക;

            അവസ്ഥ 'r':
                *d++ = '\r';
                state = sw_usual;
                നിർത്തുക;

            അവസ്ഥ 't':
                *d++ = '\t';
                state = sw_usual;
                നിർത്തുക;

            സ്ഥിരസ്ഥിതി:
                തിരികെ NGX_ERROR;
            }

            നിർത്തുക;

        അവസ്ഥ sw_hex:

            n = ngx_json_hex_digit(ch);
            എങ്കിൽ (n == NGX_ERROR) {
                തിരികെ NGX_ERROR;
            }

            codepoint = (codepoint << 4) | (uint32_t) n;

            എങ്കിൽ (--hex_ഇടത് > 0) {
                നിർത്തുക;
            }

            എങ്കിൽ (മുകളിലെ_surrogate) {
                എങ്കിൽ (codepoint < 0xDC00 || codepoint > 0xDFFF) {
                    തിരികെ NGX_ERROR;
                }

                codepoint = 0x10000
                            + ((മുകളിലെ_surrogate - 0xD800) << 10)
                            + (codepoint - 0xDC00);
                d = ngx_json_utf8_encode(d, codepoint);
                മുകളിലെ_surrogate = 0;
                state = sw_usual;
                നിർത്തുക;
            }

            എങ്കിൽ (codepoint >= 0xD800 && codepoint <= 0xDBFF) {
                /* high surrogate - wait for the low surrogate */
                മുകളിലെ_surrogate = codepoint;
                state = sw_surrogate_start;
                നിർത്തുക;
            }

            എങ്കിൽ (codepoint >= 0xDC00 && codepoint <= 0xDFFF) {
                /* lone low surrogate */
                തിരികെ NGX_ERROR;
            }

            d = ngx_json_utf8_encode(d, codepoint);
            state = sw_usual;
            നിർത്തുക;

        അവസ്ഥ sw_surrogate_start:
            /*
             * Expect '\' to begin the low-surrogate escape.
             * Lone surrogates are not valid Unicode scalar values
             * (RFC 8259, Section 8.2).
             */
            എങ്കിൽ (ch == '\\') {
                state = sw_surrogate_u;
                നിർത്തുക;
            }

            /* lone high surrogate */
            തിരികെ NGX_ERROR;

        അവസ്ഥ sw_surrogate_u:
            എങ്കിൽ (ch == 'u') {
                codepoint = 0;
                hex_ഇടത് = 4;
                state = sw_hex;
                നിർത്തുക;
            }

            /* lone high surrogate */
            തിരികെ NGX_ERROR;
        }
    }

    എങ്കിൽ (state != sw_usual) {
        /* truncated escape sequence or unpaired high surrogate */
        തിരികെ NGX_ERROR;
    }

    dst->len = d - dst->data;

    തിരികെ NGX_OK;
}


സ്ഥിതം ngx_inline u_char *
ngx_json_utf8_encode(u_char *dst, uint32_t codepoint)
{
    എങ്കിൽ (codepoint <= 0x7F) {
        *dst++ = (u_char) codepoint;

    } അല്ലെങ്കിൽ എങ്കിൽ (codepoint <= 0x7FF) {
        *dst++ = (u_char) (0xC0 | (codepoint >> 6));
        *dst++ = (u_char) (0x80 | (codepoint & 0x3F));

    } അല്ലെങ്കിൽ എങ്കിൽ (codepoint <= 0xFFFF) {
        *dst++ = (u_char) (0xE0 | (codepoint >> 12));
        *dst++ = (u_char) (0x80 | ((codepoint >> 6) & 0x3F));
        *dst++ = (u_char) (0x80 | (codepoint & 0x3F));

    } അല്ലെങ്കിൽ {
        *dst++ = (u_char) (0xF0 | (codepoint >> 18));
        *dst++ = (u_char) (0x80 | ((codepoint >> 12) & 0x3F));
        *dst++ = (u_char) (0x80 | ((codepoint >> 6) & 0x3F));
        *dst++ = (u_char) (0x80 | (codepoint & 0x3F));
    }

    തിരികെ dst;
}


ngx_int_t
ngx_json_hex_digit(u_char ch)
{
    എങ്കിൽ (ch >= '0' && ch <= '9') {
        തിരികെ ch - '0';
    }

    ch = (u_char) (ch | 0x20);

    എങ്കിൽ (ch >= 'a' && ch <= 'f') {
        തിരികെ ch - 'a' + 10;
    }

    തിരികെ NGX_ERROR;
}
