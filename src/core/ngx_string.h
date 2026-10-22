
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#جے_بنا_تعریف _NGX_STRING_H_INCLUDED_
#تعریف_کرو _NGX_STRING_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


تعريفنوع بنية {
    ناپ_t      len;
    u_char     *انکڑے;
} ngx_str_t;


تعريفنوع بنية {
    ngx_str_t   کلید;
    ngx_str_t   قدر;
} ngx_keyval_t;


تعريفنوع بنية {
    بنانشانی    len:28;

    بنانشانی    valid:1;
    بنانشانی    no_cacheable:1;
    بنانشانی    not_لبھیا:1;
    بنانشانی    escape:1;

    u_char     *انکڑے;
} ngx_variable_قدر_t;


#تعریف_کرو ngx_string(str)     { حجم(str) - 1, (u_char *) str }
#تعریف_کرو ngx_null_string     { 0, عدم }
#تعریف_کرو ngx_str_سیٹ(str, متن)                                               \
    (str)->len = حجم(متن) - 1; (str)->انکڑے = (u_char *) متن
#تعریف_کرو ngx_str_null(str)   (str)->len = 0; (str)->انکڑے = عدم


#تعریف_کرو ngx_tolower(c)      (u_char) ((c >= 'A' && c <= 'Z') ? (c | 0x20) : c)
#تعریف_کرو ngx_toupper(c)      (u_char) ((c >= 'a' && c <= 'z') ? (c & ~0x20) : c)

فراغ ngx_strlow(u_char *dst, u_char *src, ناپ_t n);


#تعریف_کرو ngx_strncmp(s1, s2, n)  لڑی_مقابلہ((ثابت حرف *) s1, (ثابت حرف *) s2, n)


/* msvc and icc7 compile strcmp() to inline loop */
#تعریف_کرو ngx_strcmp(s1, s2)  strcmp((ثابت حرف *) s1, (ثابت حرف *) s2)


#تعریف_کرو ngx_strstr(s1, s2)  لڑی_کھوج((ثابت حرف *) s1, (ثابت حرف *) s2)
#تعریف_کرو ngx_strlen(s)       لڑی_لمبائی((ثابت حرف *) s)

ناپ_t ngx_strnlen(u_char *p, ناپ_t n);

#تعریف_کرو ngx_strchr(s1, c)   strchr((ثابت حرف *) s1, (صحيح) c)

ساكن ngx_inline u_char *
ngx_strlchr(u_char *p, u_char *آخری, u_char c)
{
    طالما (p < آخری) {

        إذا (*p == c) {
            أرجع p;
        }

        p++;
    }

    أرجع عدم;
}


/*
 * msvc and icc7 compile memset() to the inline "rep stos"
 * while ZeroMemory() and bzero() are the calls.
 * icc7 may also inline several mov's of a zeroed register for small blocks.
 */
#تعریف_کرو ngx_memzero(buf, n)       (فراغ) memset(buf, 0, n)
#تعریف_کرو ngx_memset(buf, c, n)     (فراغ) memset(buf, c, n)

فراغ ngx_explicit_memzero(فراغ *buf, ناپ_t n);


#إذا (NGX_MEMCPY_حد_بندی)

فراغ *ngx_memcpy(فراغ *dst, ثابت فراغ *src, ناپ_t n);
#تعریف_کرو ngx_cpymem(dst, src, n)   (((u_char *) ngx_memcpy(dst, src, n)) + (n))

#وإلا

/*
 * gcc3, msvc, and icc7 compile memcpy() to the inline "rep movs".
 * gcc3 compiles memcpy(d, s, 4) to the inline "mov"es.
 * icc8 compile memcpy(d, s, 4) to the inline "mov"es or XMM moves.
 */
#تعریف_کرو ngx_memcpy(dst, src, n)   (فراغ) حافظہ_نقل(dst, src, n)
#تعریف_کرو ngx_cpymem(dst, src, n)   (((u_char *) حافظہ_نقل(dst, src, n)) + (n))

#جے_انت


#إذا ( __INTEL_COMPILER >= 800 )

/*
 * the simple inline cycle copies the variable length strings up to 16
 * bytes faster than icc8 autodetecting _intel_fast_memcpy()
 */

ساكن ngx_inline u_char *
ngx_copy(u_char *dst, u_char *src, ناپ_t len)
{
    إذا (len < 17) {

        طالما (len) {
            *dst++ = *src++;
            len--;
        }

        أرجع dst;

    } وإلا {
        أرجع ngx_cpymem(dst, src, len);
    }
}

#وإلا

#تعریف_کرو ngx_copy                  ngx_cpymem

#جے_انت


#تعریف_کرو ngx_memmove(dst, src, n)  (فراغ) memmove(dst, src, n)
#تعریف_کرو ngx_movemem(dst, src, n)  (((u_char *) memmove(dst, src, n)) + (n))


/* msvc and icc7 compile memcmp() to the inline loop */
#تعریف_کرو ngx_memcmp(s1, s2, n)     memcmp(s1, s2, n)


u_char *ngx_cpystrn(u_char *dst, u_char *src, ناپ_t n);
u_char *ngx_pstrdup(ngx_pool_t *pool, ngx_str_t *src);
u_char * ngx_cdecl ngx_sprintf(u_char *buf, ثابت حرف *fmt, ...);
u_char * ngx_cdecl ngx_snprintf(u_char *buf, ناپ_t max, ثابت حرف *fmt, ...);
u_char * ngx_cdecl ngx_slprintf(u_char *buf, u_char *آخری, ثابت حرف *fmt,
    ...);
u_char *ngx_vslprintf(u_char *buf, u_char *آخری, ثابت حرف *fmt, va_list دلیلاں);
#تعریف_کرو ngx_vsnprintf(buf, max, fmt, دلیلاں)                                   \
    ngx_vslprintf(buf, buf + (max), fmt, دلیلاں)

ngx_int_t ngx_strcasecmp(u_char *s1, u_char *s2);
ngx_int_t ngx_strncasecmp(u_char *s1, u_char *s2, ناپ_t n);

u_char *ngx_strnstr(u_char *s1, حرف *s2, ناپ_t n);

u_char *ngx_strstrn(u_char *s1, حرف *s2, ناپ_t n);
u_char *ngx_strcasestrn(u_char *s1, حرف *s2, ناپ_t n);
u_char *ngx_strlcasestrn(u_char *s1, u_char *آخری, u_char *s2, ناپ_t n);

ngx_int_t ngx_rstrncmp(u_char *s1, u_char *s2, ناپ_t n);
ngx_int_t ngx_rstrncasecmp(u_char *s1, u_char *s2, ناپ_t n);
ngx_int_t ngx_memn2cmp(u_char *s1, u_char *s2, ناپ_t n1, ناپ_t n2);
ngx_int_t ngx_dns_strcmp(u_char *s1, u_char *s2);
ngx_int_t ngx_filename_cmp(u_char *s1, u_char *s2, ناپ_t n);

ngx_int_t ngx_atoi(u_char *سطر, ناپ_t n);
ngx_int_t ngx_atofp(u_char *سطر, ناپ_t n, ناپ_t نقطہ);
ssize_t ngx_atosz(u_char *سطر, ناپ_t n);
off_t ngx_atoof(u_char *سطر, ناپ_t n);
ویلا_t ngx_atotm(u_char *سطر, ناپ_t n);
ngx_int_t ngx_hextoi(u_char *سطر, ناپ_t n);

u_char *ngx_hex_dump(u_char *dst, u_char *src, ناپ_t len);


#تعریف_کرو ngx_base64_encoded_لمائی(len)  (((len + 2) / 3) * 4)
#تعریف_کرو ngx_base64_decoded_لمائی(len)  (((len + 3) / 4) * 3)

فراغ ngx_encode_base64(ngx_str_t *dst, ngx_str_t *src);
فراغ ngx_encode_base64url(ngx_str_t *dst, ngx_str_t *src);
ngx_int_t ngx_decode_base64(ngx_str_t *dst, ngx_str_t *src);
ngx_int_t ngx_decode_base64url(ngx_str_t *dst, ngx_str_t *src);

uint32_t ngx_utf8_decode(u_char **p, ناپ_t n);
ناپ_t ngx_utf8_لمائی(u_char *p, ناپ_t n);
u_char *ngx_utf8_cpystrn(u_char *dst, u_char *src, ناپ_t n, ناپ_t len);


#تعریف_کرو NGX_ESCAPE_URI            0
#تعریف_کرو NGX_ESCAPE_دلیلاں           1
#تعریف_کرو NGX_ESCAPE_URI_COMPONENT  2
#تعریف_کرو NGX_ESCAPE_HTML           3
#تعریف_کرو NGX_ESCAPE_REFRESH        4
#تعریف_کرو NGX_ESCAPE_MEMCACHED      5
#تعریف_کرو NGX_ESCAPE_MAIL_AUTH      6
#تعریف_کرو NGX_ESCAPE_MAIL_XTEXT     7

#تعریف_کرو NGX_UNESCAPE_URI       1
#تعریف_کرو NGX_UNESCAPE_REDIRECT  2

uintptr_t ngx_escape_uri(u_char *dst, u_char *src, ناپ_t ناپ,
    ngx_uint_t type);
فراغ ngx_unescape_uri(u_char **dst, u_char **src, ناپ_t ناپ, ngx_uint_t type);
uintptr_t ngx_escape_html(u_char *dst, u_char *src, ناپ_t ناپ);
uintptr_t ngx_escape_json(u_char *dst, u_char *src, ناپ_t ناپ);


تعريفنوع بنية {
    ngx_rbtree_گرہ_t         گرہ;
    ngx_str_t                 str;
} ngx_str_گرہ_t;


فراغ ngx_str_rbtree_داخل_کرو_قدر(ngx_rbtree_گرہ_t *عارضی,
    ngx_rbtree_گرہ_t *گرہ, ngx_rbtree_گرہ_t *sentinel);
ngx_str_گرہ_t *ngx_str_rbtree_lookup(ngx_rbtree_t *rbtree, ngx_str_t *name,
    uint32_t hash);


فراغ ngx_ترتیب_دیو(فراغ *ادھار, ناپ_t n, ناپ_t ناپ,
    ngx_int_t (*cmp)(ثابت فراغ *, ثابت فراغ *));
#تعریف_کرو ngx_qsort             qsort


#تعریف_کرو ngx_قدر_helper(n)   #n
#تعریف_کرو ngx_قدر(n)          ngx_قدر_helper(n)


#جے_انت /* _NGX_STRING_H_INCLUDED_ */
