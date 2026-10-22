
/*
 * Copyright (C) Nginx, Inc.
 */


#اگر_تعریف_نشده _NGX_JSON_UNESCAPE_H_INCLUDED_
#تعریف_کن _NGX_JSON_UNESCAPE_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


/*
 * Decode JSON escape sequences (RFC 8259, Section 7).  The input
 * is an unquoted JSON string body (no surrounding double quotes).
 *
 * On entry, dst->len is the capacity of dst->data and must be at least
 * src->len.
 * On success, dst->len is the decoded length.
 *
 * Returns NGX_OK, or NGX_ERROR on a malformed escape or invalid input.
 */
ngx_int_t ngx_json_unescape_string(ngx_str_t *dst, ngx_str_t *src);

ngx_int_t ngx_json_hex_digit(u_char ch);


#پایان_اگر /* _NGX_JSON_UNESCAPE_H_INCLUDED_ */
