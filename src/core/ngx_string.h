
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#اگر_غیر_تعریف_شدہ _NGX_STRING_H_INCLUDED_
#تعریف_کرو _NGX_STRING_H_INCLUDED_


#شامل <ngx_config.h>
#شامل <ngx_core.h>


قسمتعریف ساخت {
    سائز_t      len;
    u_char     *ڈیٹا;
} ngx_str_t;


قسمتعریف ساخت {
    ngx_str_t   کلید;
    ngx_str_t   قدر;
} ngx_keyval_t;


قسمتعریف ساخت {
    غیرعلامتی    len:28;

    غیرعلامتی    valid:1;
    غیرعلامتی    no_cacheable:1;
    غیرعلامتی    not_ملا:1;
    غیرعلامتی    escape:1;

    u_char     *ڈیٹا;
} ngx_variable_قدر_t;


#تعریف_کرو ngx_string(str)     { حجم(str) - 1, (u_char *) str }
#تعریف_کرو ngx_null_string     { 0, تہی }
#تعریف_کرو ngx_str_سیٹ(str, متن)                                               \
    (str)->len = حجم(متن) - 1; (str)->ڈیٹا = (u_char *) متن
#تعریف_کرو ngx_str_null(str)   (str)->len = 0; (str)->ڈیٹا = تہی


#تعریف_کرو ngx_tolower(c)      (u_char) ((c >= 'A' && c <= 'Z') ? (c | 0x20) : c)
#تعریف_کرو ngx_toupper(c)      (u_char) ((c >= 'a' && c <= 'z') ? (c & ~0x20) : c)

خالی ngx_strlow(u_char *dst, u_char *src, سائز_t n);


#تعریف_کرو ngx_strncmp(s1, s2, n)  لڑی_موازنہ((ثابت حرف *) s1, (ثابت حرف *) s2, n)


/* msvc and icc7 compile strcmp() to inline loop */
#تعریف_کرو ngx_strcmp(s1, s2)  strcmp((ثابت حرف *) s1, (ثابت حرف *) s2)


#تعریف_کرو ngx_strstr(s1, s2)  لڑی_تلاش((ثابت حرف *) s1, (ثابت حرف *) s2)
#تعریف_کرو ngx_strlen(s)       لڑی_لمبائی((ثابت حرف *) s)

سائز_t ngx_strnlen(u_char *p, سائز_t n);

#تعریف_کرو ngx_strchr(s1, c)   strchr((ثابت حرف *) s1, (صحیح) c)

ساکن ngx_inline u_char *
ngx_strlchr(u_char *p, u_char *آخری, u_char c)
{
    جب (p < آخری) {

        اگر (*p == c) {
            واپس p;
        }

        p++;
    }

    واپس تہی;
}


/*
 * msvc and icc7 compile memset() to the inline "rep stos"
 * while ZeroMemory() and bzero() are the calls.
 * icc7 may also inline several mov's of a zeroed register for small blocks.
 */
#تعریف_کرو ngx_memzero(buf, n)       (خالی) memset(buf, 0, n)
#تعریف_کرو ngx_memset(buf, c, n)     (خالی) memset(buf, c, n)

خالی ngx_explicit_memzero(خالی *buf, سائز_t n);


#اگر (NGX_MEMCPY_حد_بندی)

خالی *ngx_memcpy(خالی *dst, ثابت خالی *src, سائز_t n);
#تعریف_کرو ngx_cpymem(dst, src, n)   (((u_char *) ngx_memcpy(dst, src, n)) + (n))

#ورنہ

/*
 * gcc3, msvc, and icc7 compile memcpy() to the inline "rep movs".
 * gcc3 compiles memcpy(d, s, 4) to the inline "mov"es.
 * icc8 compile memcpy(d, s, 4) to the inline "mov"es or XMM moves.
 */
#تعریف_کرو ngx_memcpy(dst, src, n)   (خالی) حافظہ_نقل(dst, src, n)
#تعریف_کرو ngx_cpymem(dst, src, n)   (((u_char *) حافظہ_نقل(dst, src, n)) + (n))

#اگر_ختم


#اگر ( __INTEL_COMPILER >= 800 )

/*
 * the simple inline cycle copies the variable length strings up to 16
 * bytes faster than icc8 autodetecting _intel_fast_memcpy()
 */

ساکن ngx_inline u_char *
ngx_copy(u_char *dst, u_char *src, سائز_t len)
{
    اگر (len < 17) {

        جب (len) {
            *dst++ = *src++;
            len--;
        }

        واپس dst;

    } ورنہ {
        واپس ngx_cpymem(dst, src, len);
    }
}

#ورنہ

#تعریف_کرو ngx_copy                  ngx_cpymem

#اگر_ختم


#تعریف_کرو ngx_memmove(dst, src, n)  (خالی) memmove(dst, src, n)
#تعریف_کرو ngx_movemem(dst, src, n)  (((u_char *) memmove(dst, src, n)) + (n))


/* msvc and icc7 compile memcmp() to the inline loop */
#تعریف_کرو ngx_memcmp(s1, s2, n)     memcmp(s1, s2, n)


u_char *ngx_cpystrn(u_char *dst, u_char *src, سائز_t n);
u_char *ngx_pstrdup(ngx_pool_t *pool, ngx_str_t *src);
u_char * ngx_cdecl ngx_sprintf(u_char *buf, ثابت حرف *fmt, ...);
u_char * ngx_cdecl ngx_snprintf(u_char *buf, سائز_t max, ثابت حرف *fmt, ...);
u_char * ngx_cdecl ngx_slprintf(u_char *buf, u_char *آخری, ثابت حرف *fmt,
    ...);
u_char *ngx_vslprintf(u_char *buf, u_char *آخری, ثابت حرف *fmt, va_list دلائل);
#تعریف_کرو ngx_vsnprintf(buf, max, fmt, دلائل)                                   \
    ngx_vslprintf(buf, buf + (max), fmt, دلائل)

ngx_int_t ngx_strcasecmp(u_char *s1, u_char *s2);
ngx_int_t ngx_strncasecmp(u_char *s1, u_char *s2, سائز_t n);

u_char *ngx_strnstr(u_char *s1, حرف *s2, سائز_t n);

u_char *ngx_strstrn(u_char *s1, حرف *s2, سائز_t n);
u_char *ngx_strcasestrn(u_char *s1, حرف *s2, سائز_t n);
u_char *ngx_strlcasestrn(u_char *s1, u_char *آخری, u_char *s2, سائز_t n);

ngx_int_t ngx_rstrncmp(u_char *s1, u_char *s2, سائز_t n);
ngx_int_t ngx_rstrncasecmp(u_char *s1, u_char *s2, سائز_t n);
ngx_int_t ngx_memn2cmp(u_char *s1, u_char *s2, سائز_t n1, سائز_t n2);
ngx_int_t ngx_dns_strcmp(u_char *s1, u_char *s2);
ngx_int_t ngx_filename_cmp(u_char *s1, u_char *s2, سائز_t n);

ngx_int_t ngx_atoi(u_char *سطر, سائز_t n);
ngx_int_t ngx_atofp(u_char *سطر, سائز_t n, سائز_t نقطہ);
ssize_t ngx_atosz(u_char *سطر, سائز_t n);
off_t ngx_atoof(u_char *سطر, سائز_t n);
وقت_t ngx_atotm(u_char *سطر, سائز_t n);
ngx_int_t ngx_hextoi(u_char *سطر, سائز_t n);

u_char *ngx_hex_dump(u_char *dst, u_char *src, سائز_t len);


#تعریف_کرو ngx_base64_encoded_لمبائی(len)  (((len + 2) / 3) * 4)
#تعریف_کرو ngx_base64_decoded_لمبائی(len)  (((len + 3) / 4) * 3)

خالی ngx_encode_base64(ngx_str_t *dst, ngx_str_t *src);
خالی ngx_encode_base64url(ngx_str_t *dst, ngx_str_t *src);
ngx_int_t ngx_decode_base64(ngx_str_t *dst, ngx_str_t *src);
ngx_int_t ngx_decode_base64url(ngx_str_t *dst, ngx_str_t *src);

uint32_t ngx_utf8_decode(u_char **p, سائز_t n);
سائز_t ngx_utf8_لمبائی(u_char *p, سائز_t n);
u_char *ngx_utf8_cpystrn(u_char *dst, u_char *src, سائز_t n, سائز_t len);


#تعریف_کرو NGX_ESCAPE_URI            0
#تعریف_کرو NGX_ESCAPE_دلائل           1
#تعریف_کرو NGX_ESCAPE_URI_COMPONENT  2
#تعریف_کرو NGX_ESCAPE_HTML           3
#تعریف_کرو NGX_ESCAPE_REFRESH        4
#تعریف_کرو NGX_ESCAPE_MEMCACHED      5
#تعریف_کرو NGX_ESCAPE_MAIL_AUTH      6
#تعریف_کرو NGX_ESCAPE_MAIL_XTEXT     7

#تعریف_کرو NGX_UNESCAPE_URI       1
#تعریف_کرو NGX_UNESCAPE_REDIRECT  2

uintptr_t ngx_escape_uri(u_char *dst, u_char *src, سائز_t سائز,
    ngx_uint_t type);
خالی ngx_unescape_uri(u_char **dst, u_char **src, سائز_t سائز, ngx_uint_t type);
uintptr_t ngx_escape_html(u_char *dst, u_char *src, سائز_t سائز);
uintptr_t ngx_escape_json(u_char *dst, u_char *src, سائز_t سائز);


قسمتعریف ساخت {
    ngx_rbtree_گرہ_t         گرہ;
    ngx_str_t                 str;
} ngx_str_گرہ_t;


خالی ngx_str_rbtree_داخل_کرو_قدر(ngx_rbtree_گرہ_t *عارضی,
    ngx_rbtree_گرہ_t *گرہ, ngx_rbtree_گرہ_t *sentinel);
ngx_str_گرہ_t *ngx_str_rbtree_lookup(ngx_rbtree_t *rbtree, ngx_str_t *name,
    uint32_t hash);


خالی ngx_ترتیب_دو(خالی *بنیاد, سائز_t n, سائز_t سائز,
    ngx_int_t (*cmp)(ثابت خالی *, ثابت خالی *));
#تعریف_کرو ngx_qsort             qsort


#تعریف_کرو ngx_قدر_helper(n)   #n
#تعریف_کرو ngx_قدر(n)          ngx_قدر_helper(n)


#اگر_ختم /* _NGX_STRING_H_INCLUDED_ */
