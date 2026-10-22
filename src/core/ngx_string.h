
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#جيڪڏهن_وصف_نه_ٿيل _NGX_STRING_H_INCLUDED_
#وصف_ڪريو _NGX_STRING_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


تعريفنوع بنية {
    سائيز_t      len;
    u_char     *معلومات;
} ngx_str_t;


تعريفنوع بنية {
    ngx_str_t   ڪنجي;
    ngx_str_t   قدر;
} ngx_keyval_t;


تعريفنوع بنية {
    بنانشان    len:28;

    بنانشان    valid:1;
    بنانشان    no_cacheable:1;
    بنانشان    not_مليو:1;
    بنانشان    escape:1;

    u_char     *معلومات;
} ngx_variable_قدر_t;


#وصف_ڪريو ngx_string(str)     { حجم(str) - 1, (u_char *) str }
#وصف_ڪريو ngx_null_string     { 0, عدم }
#وصف_ڪريو ngx_str_سيٽ(str, متن)                                               \
    (str)->len = حجم(متن) - 1; (str)->معلومات = (u_char *) متن
#وصف_ڪريو ngx_str_null(str)   (str)->len = 0; (str)->معلومات = عدم


#وصف_ڪريو ngx_tolower(c)      (u_char) ((c >= 'A' && c <= 'Z') ? (c | 0x20) : c)
#وصف_ڪريو ngx_toupper(c)      (u_char) ((c >= 'a' && c <= 'z') ? (c & ~0x20) : c)

فراغ ngx_strlow(u_char *dst, u_char *src, سائيز_t n);


#وصف_ڪريو ngx_strncmp(s1, s2, n)  لڙي_ڀيٽ((ثابت حرف *) s1, (ثابت حرف *) s2, n)


/* msvc and icc7 compile strcmp() to inline loop */
#وصف_ڪريو ngx_strcmp(s1, s2)  strcmp((ثابت حرف *) s1, (ثابت حرف *) s2)


#وصف_ڪريو ngx_strstr(s1, s2)  لڙي_ڳولا((ثابت حرف *) s1, (ثابت حرف *) s2)
#وصف_ڪريو ngx_strlen(s)       لڙي_ڊيگهه((ثابت حرف *) s)

سائيز_t ngx_strnlen(u_char *p, سائيز_t n);

#وصف_ڪريو ngx_strchr(s1, c)   strchr((ثابت حرف *) s1, (صحيح) c)

ساكن ngx_inline u_char *
ngx_strlchr(u_char *p, u_char *آخري, u_char c)
{
    طالما (p < آخري) {

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
#وصف_ڪريو ngx_memzero(buf, n)       (فراغ) memset(buf, 0, n)
#وصف_ڪريو ngx_memset(buf, c, n)     (فراغ) memset(buf, c, n)

فراغ ngx_explicit_memzero(فراغ *buf, سائيز_t n);


#إذا (NGX_MEMCPY_حد_بندي)

فراغ *ngx_memcpy(فراغ *dst, ثابت فراغ *src, سائيز_t n);
#وصف_ڪريو ngx_cpymem(dst, src, n)   (((u_char *) ngx_memcpy(dst, src, n)) + (n))

#وإلا

/*
 * gcc3, msvc, and icc7 compile memcpy() to the inline "rep movs".
 * gcc3 compiles memcpy(d, s, 4) to the inline "mov"es.
 * icc8 compile memcpy(d, s, 4) to the inline "mov"es or XMM moves.
 */
#وصف_ڪريو ngx_memcpy(dst, src, n)   (فراغ) يادگيري_نقل(dst, src, n)
#وصف_ڪريو ngx_cpymem(dst, src, n)   (((u_char *) يادگيري_نقل(dst, src, n)) + (n))

#جيڪڏهن_پورو


#إذا ( __INTEL_COMPILER >= 800 )

/*
 * the simple inline cycle copies the variable length strings up to 16
 * bytes faster than icc8 autodetecting _intel_fast_memcpy()
 */

ساكن ngx_inline u_char *
ngx_copy(u_char *dst, u_char *src, سائيز_t len)
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

#وصف_ڪريو ngx_copy                  ngx_cpymem

#جيڪڏهن_پورو


#وصف_ڪريو ngx_memmove(dst, src, n)  (فراغ) memmove(dst, src, n)
#وصف_ڪريو ngx_movemem(dst, src, n)  (((u_char *) memmove(dst, src, n)) + (n))


/* msvc and icc7 compile memcmp() to the inline loop */
#وصف_ڪريو ngx_memcmp(s1, s2, n)     memcmp(s1, s2, n)


u_char *ngx_cpystrn(u_char *dst, u_char *src, سائيز_t n);
u_char *ngx_pstrdup(ngx_pool_t *pool, ngx_str_t *src);
u_char * ngx_cdecl ngx_sprintf(u_char *buf, ثابت حرف *fmt, ...);
u_char * ngx_cdecl ngx_snprintf(u_char *buf, سائيز_t max, ثابت حرف *fmt, ...);
u_char * ngx_cdecl ngx_slprintf(u_char *buf, u_char *آخري, ثابت حرف *fmt,
    ...);
u_char *ngx_vslprintf(u_char *buf, u_char *آخري, ثابت حرف *fmt, va_list دليلون);
#وصف_ڪريو ngx_vsnprintf(buf, max, fmt, دليلون)                                   \
    ngx_vslprintf(buf, buf + (max), fmt, دليلون)

ngx_int_t ngx_strcasecmp(u_char *s1, u_char *s2);
ngx_int_t ngx_strncasecmp(u_char *s1, u_char *s2, سائيز_t n);

u_char *ngx_strnstr(u_char *s1, حرف *s2, سائيز_t n);

u_char *ngx_strstrn(u_char *s1, حرف *s2, سائيز_t n);
u_char *ngx_strcasestrn(u_char *s1, حرف *s2, سائيز_t n);
u_char *ngx_strlcasestrn(u_char *s1, u_char *آخري, u_char *s2, سائيز_t n);

ngx_int_t ngx_rstrncmp(u_char *s1, u_char *s2, سائيز_t n);
ngx_int_t ngx_rstrncasecmp(u_char *s1, u_char *s2, سائيز_t n);
ngx_int_t ngx_memn2cmp(u_char *s1, u_char *s2, سائيز_t n1, سائيز_t n2);
ngx_int_t ngx_dns_strcmp(u_char *s1, u_char *s2);
ngx_int_t ngx_filename_cmp(u_char *s1, u_char *s2, سائيز_t n);

ngx_int_t ngx_atoi(u_char *سٽ, سائيز_t n);
ngx_int_t ngx_atofp(u_char *سٽ, سائيز_t n, سائيز_t نقطو);
ssize_t ngx_atosz(u_char *سٽ, سائيز_t n);
off_t ngx_atoof(u_char *سٽ, سائيز_t n);
وقت_t ngx_atotm(u_char *سٽ, سائيز_t n);
ngx_int_t ngx_hextoi(u_char *سٽ, سائيز_t n);

u_char *ngx_hex_dump(u_char *dst, u_char *src, سائيز_t len);


#وصف_ڪريو ngx_base64_encoded_ڊيگهائي(len)  (((len + 2) / 3) * 4)
#وصف_ڪريو ngx_base64_decoded_ڊيگهائي(len)  (((len + 3) / 4) * 3)

فراغ ngx_encode_base64(ngx_str_t *dst, ngx_str_t *src);
فراغ ngx_encode_base64url(ngx_str_t *dst, ngx_str_t *src);
ngx_int_t ngx_decode_base64(ngx_str_t *dst, ngx_str_t *src);
ngx_int_t ngx_decode_base64url(ngx_str_t *dst, ngx_str_t *src);

uint32_t ngx_utf8_decode(u_char **p, سائيز_t n);
سائيز_t ngx_utf8_ڊيگهائي(u_char *p, سائيز_t n);
u_char *ngx_utf8_cpystrn(u_char *dst, u_char *src, سائيز_t n, سائيز_t len);


#وصف_ڪريو NGX_ESCAPE_URI            0
#وصف_ڪريو NGX_ESCAPE_دليلون           1
#وصف_ڪريو NGX_ESCAPE_URI_COMPONENT  2
#وصف_ڪريو NGX_ESCAPE_HTML           3
#وصف_ڪريو NGX_ESCAPE_REFRESH        4
#وصف_ڪريو NGX_ESCAPE_MEMCACHED      5
#وصف_ڪريو NGX_ESCAPE_MAIL_AUTH      6
#وصف_ڪريو NGX_ESCAPE_MAIL_XTEXT     7

#وصف_ڪريو NGX_UNESCAPE_URI       1
#وصف_ڪريو NGX_UNESCAPE_REDIRECT  2

uintptr_t ngx_escape_uri(u_char *dst, u_char *src, سائيز_t سائيز,
    ngx_uint_t type);
فراغ ngx_unescape_uri(u_char **dst, u_char **src, سائيز_t سائيز, ngx_uint_t type);
uintptr_t ngx_escape_html(u_char *dst, u_char *src, سائيز_t سائيز);
uintptr_t ngx_escape_json(u_char *dst, u_char *src, سائيز_t سائيز);


تعريفنوع بنية {
    ngx_rbtree_ڳنڍ_t         ڳنڍ;
    ngx_str_t                 str;
} ngx_str_ڳنڍ_t;


فراغ ngx_str_rbtree_وجهو_قدر(ngx_rbtree_ڳنڍ_t *عارضي,
    ngx_rbtree_ڳنڍ_t *ڳنڍ, ngx_rbtree_ڳنڍ_t *sentinel);
ngx_str_ڳنڍ_t *ngx_str_rbtree_lookup(ngx_rbtree_t *rbtree, ngx_str_t *name,
    uint32_t hash);


فراغ ngx_ترتيب_ڏيو(فراغ *بنياد, سائيز_t n, سائيز_t سائيز,
    ngx_int_t (*cmp)(ثابت فراغ *, ثابت فراغ *));
#وصف_ڪريو ngx_qsort             qsort


#وصف_ڪريو ngx_قدر_helper(n)   #n
#وصف_ڪريو ngx_قدر(n)          ngx_قدر_helper(n)


#جيڪڏهن_پورو /* _NGX_STRING_H_INCLUDED_ */
