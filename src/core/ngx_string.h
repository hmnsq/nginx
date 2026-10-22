
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#که_نه_تعریف_شوی _NGX_STRING_H_INCLUDED_
#تعریف_کړه _NGX_STRING_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


تعريفنوع بنية {
    اندازه_t      len;
    u_char     *معلومات;
} ngx_str_t;


تعريفنوع بنية {
    ngx_str_t   کلی;
    ngx_str_t   ارزښت;
} ngx_keyval_t;


تعريفنوع بنية {
    بینښه    len:28;

    بینښه    valid:1;
    بینښه    no_cacheable:1;
    بینښه    not_وموندل:1;
    بینښه    escape:1;

    u_char     *معلومات;
} ngx_variable_ارزښت_t;


#تعریف_کړه ngx_string(str)     { حجم(str) - 1, (u_char *) str }
#تعریف_کړه ngx_null_string     { 0, عدم }
#تعریف_کړه ngx_str_سېټ(str, متن)                                               \
    (str)->len = حجم(متن) - 1; (str)->معلومات = (u_char *) متن
#تعریف_کړه ngx_str_null(str)   (str)->len = 0; (str)->معلومات = عدم


#تعریف_کړه ngx_tolower(c)      (u_char) ((c >= 'A' && c <= 'Z') ? (c | 0x20) : c)
#تعریف_کړه ngx_toupper(c)      (u_char) ((c >= 'a' && c <= 'z') ? (c & ~0x20) : c)

فراغ ngx_strlow(u_char *dst, u_char *src, اندازه_t n);


#تعریف_کړه ngx_strncmp(s1, s2, n)  مزي_پرتله((ثابت حرف *) s1, (ثابت حرف *) s2, n)


/* msvc and icc7 compile strcmp() to inline loop */
#تعریف_کړه ngx_strcmp(s1, s2)  strcmp((ثابت حرف *) s1, (ثابت حرف *) s2)


#تعریف_کړه ngx_strstr(s1, s2)  مزي_پلټنه((ثابت حرف *) s1, (ثابت حرف *) s2)
#تعریف_کړه ngx_strlen(s)       مزي_اوږدوالی((ثابت حرف *) s)

اندازه_t ngx_strnlen(u_char *p, اندازه_t n);

#تعریف_کړه ngx_strchr(s1, c)   strchr((ثابت حرف *) s1, (صحيح) c)

ساكن ngx_inline u_char *
ngx_strlchr(u_char *p, u_char *وروستی, u_char c)
{
    طالما (p < وروستی) {

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
#تعریف_کړه ngx_memzero(buf, n)       (فراغ) memset(buf, 0, n)
#تعریف_کړه ngx_memset(buf, c, n)     (فراغ) memset(buf, c, n)

فراغ ngx_explicit_memzero(فراغ *buf, اندازه_t n);


#إذا (NGX_MEMCPY_حد)

فراغ *ngx_memcpy(فراغ *dst, ثابت فراغ *src, اندازه_t n);
#تعریف_کړه ngx_cpymem(dst, src, n)   (((u_char *) ngx_memcpy(dst, src, n)) + (n))

#وإلا

/*
 * gcc3, msvc, and icc7 compile memcpy() to the inline "rep movs".
 * gcc3 compiles memcpy(d, s, 4) to the inline "mov"es.
 * icc8 compile memcpy(d, s, 4) to the inline "mov"es or XMM moves.
 */
#تعریف_کړه ngx_memcpy(dst, src, n)   (فراغ) حافظه_نقل(dst, src, n)
#تعریف_کړه ngx_cpymem(dst, src, n)   (((u_char *) حافظه_نقل(dst, src, n)) + (n))

#د_که_پای


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

#تعریف_کړه ngx_copy                  ngx_cpymem

#د_که_پای


#تعریف_کړه ngx_memmove(dst, src, n)  (فراغ) memmove(dst, src, n)
#تعریف_کړه ngx_movemem(dst, src, n)  (((u_char *) memmove(dst, src, n)) + (n))


/* msvc and icc7 compile memcmp() to the inline loop */
#تعریف_کړه ngx_memcmp(s1, s2, n)     memcmp(s1, s2, n)


u_char *ngx_cpystrn(u_char *dst, u_char *src, اندازه_t n);
u_char *ngx_pstrdup(ngx_pool_t *pool, ngx_str_t *src);
u_char * ngx_cdecl ngx_sprintf(u_char *buf, ثابت حرف *fmt, ...);
u_char * ngx_cdecl ngx_snprintf(u_char *buf, اندازه_t max, ثابت حرف *fmt, ...);
u_char * ngx_cdecl ngx_slprintf(u_char *buf, u_char *وروستی, ثابت حرف *fmt,
    ...);
u_char *ngx_vslprintf(u_char *buf, u_char *وروستی, ثابت حرف *fmt, va_list دلیلونه);
#تعریف_کړه ngx_vsnprintf(buf, max, fmt, دلیلونه)                                   \
    ngx_vslprintf(buf, buf + (max), fmt, دلیلونه)

ngx_int_t ngx_strcasecmp(u_char *s1, u_char *s2);
ngx_int_t ngx_strncasecmp(u_char *s1, u_char *s2, اندازه_t n);

u_char *ngx_strnstr(u_char *s1, حرف *s2, اندازه_t n);

u_char *ngx_strstrn(u_char *s1, حرف *s2, اندازه_t n);
u_char *ngx_strcasestrn(u_char *s1, حرف *s2, اندازه_t n);
u_char *ngx_strlcasestrn(u_char *s1, u_char *وروستی, u_char *s2, اندازه_t n);

ngx_int_t ngx_rstrncmp(u_char *s1, u_char *s2, اندازه_t n);
ngx_int_t ngx_rstrncasecmp(u_char *s1, u_char *s2, اندازه_t n);
ngx_int_t ngx_memn2cmp(u_char *s1, u_char *s2, اندازه_t n1, اندازه_t n2);
ngx_int_t ngx_dns_strcmp(u_char *s1, u_char *s2);
ngx_int_t ngx_filename_cmp(u_char *s1, u_char *s2, اندازه_t n);

ngx_int_t ngx_atoi(u_char *کرښه, اندازه_t n);
ngx_int_t ngx_atofp(u_char *کرښه, اندازه_t n, اندازه_t ټکی);
ssize_t ngx_atosz(u_char *کرښه, اندازه_t n);
off_t ngx_atoof(u_char *کرښه, اندازه_t n);
وخت_t ngx_atotm(u_char *کرښه, اندازه_t n);
ngx_int_t ngx_hextoi(u_char *کرښه, اندازه_t n);

u_char *ngx_hex_dump(u_char *dst, u_char *src, اندازه_t len);


#تعریف_کړه ngx_base64_encoded_اوږدتیا(len)  (((len + 2) / 3) * 4)
#تعریف_کړه ngx_base64_decoded_اوږدتیا(len)  (((len + 3) / 4) * 3)

فراغ ngx_encode_base64(ngx_str_t *dst, ngx_str_t *src);
فراغ ngx_encode_base64url(ngx_str_t *dst, ngx_str_t *src);
ngx_int_t ngx_decode_base64(ngx_str_t *dst, ngx_str_t *src);
ngx_int_t ngx_decode_base64url(ngx_str_t *dst, ngx_str_t *src);

uint32_t ngx_utf8_decode(u_char **p, اندازه_t n);
اندازه_t ngx_utf8_اوږدتیا(u_char *p, اندازه_t n);
u_char *ngx_utf8_cpystrn(u_char *dst, u_char *src, اندازه_t n, اندازه_t len);


#تعریف_کړه NGX_ESCAPE_URI            0
#تعریف_کړه NGX_ESCAPE_دلیلونه           1
#تعریف_کړه NGX_ESCAPE_URI_COMPONENT  2
#تعریف_کړه NGX_ESCAPE_HTML           3
#تعریف_کړه NGX_ESCAPE_REFRESH        4
#تعریف_کړه NGX_ESCAPE_MEMCACHED      5
#تعریف_کړه NGX_ESCAPE_MAIL_AUTH      6
#تعریف_کړه NGX_ESCAPE_MAIL_XTEXT     7

#تعریف_کړه NGX_UNESCAPE_URI       1
#تعریف_کړه NGX_UNESCAPE_REDIRECT  2

uintptr_t ngx_escape_uri(u_char *dst, u_char *src, اندازه_t اندازه,
    ngx_uint_t type);
فراغ ngx_unescape_uri(u_char **dst, u_char **src, اندازه_t اندازه, ngx_uint_t type);
uintptr_t ngx_escape_html(u_char *dst, u_char *src, اندازه_t اندازه);
uintptr_t ngx_escape_json(u_char *dst, u_char *src, اندازه_t اندازه);


تعريفنوع بنية {
    ngx_rbtree_غوټه_t         غوټه;
    ngx_str_t                 str;
} ngx_str_غوټه_t;


فراغ ngx_str_rbtree_ورننباسه_ارزښت(ngx_rbtree_غوټه_t *لنډمهاله,
    ngx_rbtree_غوټه_t *غوټه, ngx_rbtree_غوټه_t *sentinel);
ngx_str_غوټه_t *ngx_str_rbtree_lookup(ngx_rbtree_t *rbtree, ngx_str_t *name,
    uint32_t hash);


فراغ ngx_ترتیبول(فراغ *بنسټ, اندازه_t n, اندازه_t اندازه,
    ngx_int_t (*cmp)(ثابت فراغ *, ثابت فراغ *));
#تعریف_کړه ngx_qsort             qsort


#تعریف_کړه ngx_ارزښت_helper(n)   #n
#تعریف_کړه ngx_ارزښت(n)          ngx_ارزښت_helper(n)


#د_که_پای /* _NGX_STRING_H_INCLUDED_ */
