
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#اگر_تعریف_نشده _NGX_STRING_H_INCLUDED_
#تعریف_کن _NGX_STRING_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


تعريفنوع بنية {
    اندازه_t      len;
    u_char     *داده;
} ngx_str_t;


تعريفنوع بنية {
    ngx_str_t   کلید;
    ngx_str_t   مقدار;
} ngx_keyval_t;


تعريفنوع بنية {
    بیعلامت    len:28;

    بیعلامت    valid:1;
    بیعلامت    no_cacheable:1;
    بیعلامت    not_یافت:1;
    بیعلامت    escape:1;

    u_char     *داده;
} ngx_variable_مقدار_t;


#تعریف_کن ngx_string(str)     { حجم(str) - 1, (u_char *) str }
#تعریف_کن ngx_null_string     { 0, عدم }
#تعریف_کن ngx_str_مجموعه(str, متن)                                               \
    (str)->len = حجم(متن) - 1; (str)->داده = (u_char *) متن
#تعریف_کن ngx_str_null(str)   (str)->len = 0; (str)->داده = عدم


#تعریف_کن ngx_tolower(c)      (u_char) ((c >= 'A' && c <= 'Z') ? (c | 0x20) : c)
#تعریف_کن ngx_toupper(c)      (u_char) ((c >= 'a' && c <= 'z') ? (c & ~0x20) : c)

فراغ ngx_strlow(u_char *dst, u_char *src, اندازه_t n);


#تعریف_کن ngx_strncmp(s1, s2, n)  مقایسه_رشته((ثابت حرف *) s1, (ثابت حرف *) s2, n)


/* msvc and icc7 compile strcmp() to inline loop */
#تعریف_کن ngx_strcmp(s1, s2)  strcmp((ثابت حرف *) s1, (ثابت حرف *) s2)


#تعریف_کن ngx_strstr(s1, s2)  جستجوی_رشته((ثابت حرف *) s1, (ثابت حرف *) s2)
#تعریف_کن ngx_strlen(s)       طول_رشته((ثابت حرف *) s)

اندازه_t ngx_strnlen(u_char *p, اندازه_t n);

#تعریف_کن ngx_strchr(s1, c)   strchr((ثابت حرف *) s1, (صحيح) c)

ساكن ngx_inline u_char *
ngx_strlchr(u_char *p, u_char *آخرین, u_char c)
{
    طالما (p < آخرین) {

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
#تعریف_کن ngx_memzero(buf, n)       (فراغ) memset(buf, 0, n)
#تعریف_کن ngx_memset(buf, c, n)     (فراغ) memset(buf, c, n)

فراغ ngx_explicit_memzero(فراغ *buf, اندازه_t n);


#إذا (NGX_MEMCPY_حد)

فراغ *ngx_memcpy(فراغ *dst, ثابت فراغ *src, اندازه_t n);
#تعریف_کن ngx_cpymem(dst, src, n)   (((u_char *) ngx_memcpy(dst, src, n)) + (n))

#وإلا

/*
 * gcc3, msvc, and icc7 compile memcpy() to the inline "rep movs".
 * gcc3 compiles memcpy(d, s, 4) to the inline "mov"es.
 * icc8 compile memcpy(d, s, 4) to the inline "mov"es or XMM moves.
 */
#تعریف_کن ngx_memcpy(dst, src, n)   (فراغ) کپی_حافظه(dst, src, n)
#تعریف_کن ngx_cpymem(dst, src, n)   (((u_char *) کپی_حافظه(dst, src, n)) + (n))

#پایان_اگر


#إذا ( __INTEL_COMPILER >= 800 )

/*
 * the simple inline cycle copies the variable length strings up to 16
 * bytes faster than icc8 autodetecting _intel_fast_memcpy()
 */

ساكن ngx_inline u_char *
ngx_copy(u_char *dst, u_char *src, اندازه_t len)
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

#تعریف_کن ngx_copy                  ngx_cpymem

#پایان_اگر


#تعریف_کن ngx_memmove(dst, src, n)  (فراغ) memmove(dst, src, n)
#تعریف_کن ngx_movemem(dst, src, n)  (((u_char *) memmove(dst, src, n)) + (n))


/* msvc and icc7 compile memcmp() to the inline loop */
#تعریف_کن ngx_memcmp(s1, s2, n)     memcmp(s1, s2, n)


u_char *ngx_cpystrn(u_char *dst, u_char *src, اندازه_t n);
u_char *ngx_pstrdup(ngx_pool_t *pool, ngx_str_t *src);
u_char * ngx_cdecl ngx_sprintf(u_char *buf, ثابت حرف *fmt, ...);
u_char * ngx_cdecl ngx_snprintf(u_char *buf, اندازه_t max, ثابت حرف *fmt, ...);
u_char * ngx_cdecl ngx_slprintf(u_char *buf, u_char *آخرین, ثابت حرف *fmt,
    ...);
u_char *ngx_vslprintf(u_char *buf, u_char *آخرین, ثابت حرف *fmt, va_list آرگومانها);
#تعریف_کن ngx_vsnprintf(buf, max, fmt, آرگومانها)                                   \
    ngx_vslprintf(buf, buf + (max), fmt, آرگومانها)

ngx_int_t ngx_strcasecmp(u_char *s1, u_char *s2);
ngx_int_t ngx_strncasecmp(u_char *s1, u_char *s2, اندازه_t n);

u_char *ngx_strnstr(u_char *s1, حرف *s2, اندازه_t n);

u_char *ngx_strstrn(u_char *s1, حرف *s2, اندازه_t n);
u_char *ngx_strcasestrn(u_char *s1, حرف *s2, اندازه_t n);
u_char *ngx_strlcasestrn(u_char *s1, u_char *آخرین, u_char *s2, اندازه_t n);

ngx_int_t ngx_rstrncmp(u_char *s1, u_char *s2, اندازه_t n);
ngx_int_t ngx_rstrncasecmp(u_char *s1, u_char *s2, اندازه_t n);
ngx_int_t ngx_memn2cmp(u_char *s1, u_char *s2, اندازه_t n1, اندازه_t n2);
ngx_int_t ngx_dns_strcmp(u_char *s1, u_char *s2);
ngx_int_t ngx_filename_cmp(u_char *s1, u_char *s2, اندازه_t n);

ngx_int_t ngx_atoi(u_char *سطر, اندازه_t n);
ngx_int_t ngx_atofp(u_char *سطر, اندازه_t n, اندازه_t نقطه);
ssize_t ngx_atosz(u_char *سطر, اندازه_t n);
off_t ngx_atoof(u_char *سطر, اندازه_t n);
زمان_t ngx_atotm(u_char *سطر, اندازه_t n);
ngx_int_t ngx_hextoi(u_char *سطر, اندازه_t n);

u_char *ngx_hex_dump(u_char *dst, u_char *src, اندازه_t len);


#تعریف_کن ngx_base64_encoded_درازی(len)  (((len + 2) / 3) * 4)
#تعریف_کن ngx_base64_decoded_درازی(len)  (((len + 3) / 4) * 3)

فراغ ngx_encode_base64(ngx_str_t *dst, ngx_str_t *src);
فراغ ngx_encode_base64url(ngx_str_t *dst, ngx_str_t *src);
ngx_int_t ngx_decode_base64(ngx_str_t *dst, ngx_str_t *src);
ngx_int_t ngx_decode_base64url(ngx_str_t *dst, ngx_str_t *src);

uint32_t ngx_utf8_decode(u_char **p, اندازه_t n);
اندازه_t ngx_utf8_درازی(u_char *p, اندازه_t n);
u_char *ngx_utf8_cpystrn(u_char *dst, u_char *src, اندازه_t n, اندازه_t len);


#تعریف_کن NGX_ESCAPE_URI            0
#تعریف_کن NGX_ESCAPE_آرگومانها           1
#تعریف_کن NGX_ESCAPE_URI_COMPONENT  2
#تعریف_کن NGX_ESCAPE_HTML           3
#تعریف_کن NGX_ESCAPE_REFRESH        4
#تعریف_کن NGX_ESCAPE_MEMCACHED      5
#تعریف_کن NGX_ESCAPE_MAIL_AUTH      6
#تعریف_کن NGX_ESCAPE_MAIL_XTEXT     7

#تعریف_کن NGX_UNESCAPE_URI       1
#تعریف_کن NGX_UNESCAPE_REDIRECT  2

uintptr_t ngx_escape_uri(u_char *dst, u_char *src, اندازه_t اندازه,
    ngx_uint_t type);
فراغ ngx_unescape_uri(u_char **dst, u_char **src, اندازه_t اندازه, ngx_uint_t type);
uintptr_t ngx_escape_html(u_char *dst, u_char *src, اندازه_t اندازه);
uintptr_t ngx_escape_json(u_char *dst, u_char *src, اندازه_t اندازه);


تعريفنوع بنية {
    ngx_rbtree_گره_t         گره;
    ngx_str_t                 str;
} ngx_str_گره_t;


فراغ ngx_str_rbtree_درج_مقدار(ngx_rbtree_گره_t *موقت,
    ngx_rbtree_گره_t *گره, ngx_rbtree_گره_t *sentinel);
ngx_str_گره_t *ngx_str_rbtree_lookup(ngx_rbtree_t *rbtree, ngx_str_t *name,
    uint32_t hash);


فراغ ngx_مرتب_سازی(فراغ *پایه, اندازه_t n, اندازه_t اندازه,
    ngx_int_t (*cmp)(ثابت فراغ *, ثابت فراغ *));
#تعریف_کن ngx_qsort             qsort


#تعریف_کن ngx_مقدار_helper(n)   #n
#تعریف_کن ngx_مقدار(n)          ngx_مقدار_helper(n)


#پایان_اگر /* _NGX_STRING_H_INCLUDED_ */
