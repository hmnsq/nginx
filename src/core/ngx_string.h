
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_STRING_H_INCLUDED_
#define _NGX_STRING_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


تعريفنوع بنية {
    size_t      len;
    u_char     *data;
} ngx_str_t;


تعريفنوع بنية {
    ngx_str_t   کلیل;
    ngx_str_t   بەها;
} ngx_keyval_t;


تعريفنوع بنية {
    unsigned    len:28;

    unsigned    valid:1;
    unsigned    no_cacheable:1;
    unsigned    not_found:1;
    unsigned    escape:1;

    u_char     *data;
} ngx_variable_بەها_t;


#define ngx_string(str)     { حجم(str) - 1, (u_char *) str }
#define ngx_null_string     { 0, عدم }
#define ngx_str_set(str, text)                                               \
    (str)->len = حجم(text) - 1; (str)->data = (u_char *) text
#define ngx_str_null(str)   (str)->len = 0; (str)->data = عدم


#define ngx_tolower(c)      (u_char) ((c >= 'A' && c <= 'Z') ? (c | 0x20) : c)
#define ngx_toupper(c)      (u_char) ((c >= 'a' && c <= 'z') ? (c & ~0x20) : c)

فراغ ngx_strlow(u_char *dst, u_char *src, size_t n);


#define ngx_strncmp(s1, s2, n)  strncmp((ثابت حرف *) s1, (ثابت حرف *) s2, n)


/* msvc and icc7 compile strcmp() to inline loop */
#define ngx_strcmp(s1, s2)  strcmp((ثابت حرف *) s1, (ثابت حرف *) s2)


#define ngx_strstr(s1, s2)  strstr((ثابت حرف *) s1, (ثابت حرف *) s2)
#define ngx_strlen(s)       strlen((ثابت حرف *) s)

size_t ngx_strnlen(u_char *p, size_t n);

#define ngx_strchr(s1, c)   strchr((ثابت حرف *) s1, (صحيح) c)

ساكن ngx_inline u_char *
ngx_strlchr(u_char *p, u_char *last, u_char c)
{
    طالما (p < last) {

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
#define ngx_memzero(buf, n)       (فراغ) memset(buf, 0, n)
#define ngx_memset(buf, c, n)     (فراغ) memset(buf, c, n)

فراغ ngx_explicit_memzero(فراغ *buf, size_t n);


#إذا (NGX_MEMCPY_LIMIT)

فراغ *ngx_memcpy(فراغ *dst, ثابت فراغ *src, size_t n);
#define ngx_cpymem(dst, src, n)   (((u_char *) ngx_memcpy(dst, src, n)) + (n))

#وإلا

/*
 * gcc3, msvc, and icc7 compile memcpy() to the inline "rep movs".
 * gcc3 compiles memcpy(d, s, 4) to the inline "mov"es.
 * icc8 compile memcpy(d, s, 4) to the inline "mov"es or XMM moves.
 */
#define ngx_memcpy(dst, src, n)   (فراغ) memcpy(dst, src, n)
#define ngx_cpymem(dst, src, n)   (((u_char *) memcpy(dst, src, n)) + (n))

#endif


#إذا ( __INTEL_COMPILER >= 800 )

/*
 * the simple inline cycle copies the variable length strings up to 16
 * bytes faster than icc8 autodetecting _intel_fast_memcpy()
 */

ساكن ngx_inline u_char *
ngx_copy(u_char *dst, u_char *src, size_t len)
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

#define ngx_copy                  ngx_cpymem

#endif


#define ngx_memmove(dst, src, n)  (فراغ) memmove(dst, src, n)
#define ngx_movemem(dst, src, n)  (((u_char *) memmove(dst, src, n)) + (n))


/* msvc and icc7 compile memcmp() to the inline loop */
#define ngx_memcmp(s1, s2, n)     memcmp(s1, s2, n)


u_char *ngx_cpystrn(u_char *dst, u_char *src, size_t n);
u_char *ngx_pstrdup(ngx_pool_t *pool, ngx_str_t *src);
u_char * ngx_cdecl ngx_sprintf(u_char *buf, ثابت حرف *fmt, ...);
u_char * ngx_cdecl ngx_snprintf(u_char *buf, size_t max, ثابت حرف *fmt, ...);
u_char * ngx_cdecl ngx_slprintf(u_char *buf, u_char *last, ثابت حرف *fmt,
    ...);
u_char *ngx_vslprintf(u_char *buf, u_char *last, ثابت حرف *fmt, va_list args);
#define ngx_vsnprintf(buf, max, fmt, args)                                   \
    ngx_vslprintf(buf, buf + (max), fmt, args)

ngx_int_t ngx_strcasecmp(u_char *s1, u_char *s2);
ngx_int_t ngx_strncasecmp(u_char *s1, u_char *s2, size_t n);

u_char *ngx_strnstr(u_char *s1, حرف *s2, size_t n);

u_char *ngx_strstrn(u_char *s1, حرف *s2, size_t n);
u_char *ngx_strcasestrn(u_char *s1, حرف *s2, size_t n);
u_char *ngx_strlcasestrn(u_char *s1, u_char *last, u_char *s2, size_t n);

ngx_int_t ngx_rstrncmp(u_char *s1, u_char *s2, size_t n);
ngx_int_t ngx_rstrncasecmp(u_char *s1, u_char *s2, size_t n);
ngx_int_t ngx_memn2cmp(u_char *s1, u_char *s2, size_t n1, size_t n2);
ngx_int_t ngx_dns_strcmp(u_char *s1, u_char *s2);
ngx_int_t ngx_filename_cmp(u_char *s1, u_char *s2, size_t n);

ngx_int_t ngx_atoi(u_char *هێڵ, size_t n);
ngx_int_t ngx_atofp(u_char *هێڵ, size_t n, size_t point);
ssize_t ngx_atosz(u_char *هێڵ, size_t n);
off_t ngx_atoof(u_char *هێڵ, size_t n);
time_t ngx_atotm(u_char *هێڵ, size_t n);
ngx_int_t ngx_hextoi(u_char *هێڵ, size_t n);

u_char *ngx_hex_dump(u_char *dst, u_char *src, size_t len);


#define ngx_base64_encoded_length(len)  (((len + 2) / 3) * 4)
#define ngx_base64_decoded_length(len)  (((len + 3) / 4) * 3)

فراغ ngx_encode_base64(ngx_str_t *dst, ngx_str_t *src);
فراغ ngx_encode_base64url(ngx_str_t *dst, ngx_str_t *src);
ngx_int_t ngx_decode_base64(ngx_str_t *dst, ngx_str_t *src);
ngx_int_t ngx_decode_base64url(ngx_str_t *dst, ngx_str_t *src);

uint32_t ngx_utf8_decode(u_char **p, size_t n);
size_t ngx_utf8_length(u_char *p, size_t n);
u_char *ngx_utf8_cpystrn(u_char *dst, u_char *src, size_t n, size_t len);


#define NGX_ESCAPE_URI            0
#define NGX_ESCAPE_ARGS           1
#define NGX_ESCAPE_URI_COMPONENT  2
#define NGX_ESCAPE_HTML           3
#define NGX_ESCAPE_REFRESH        4
#define NGX_ESCAPE_MEMCACHED      5
#define NGX_ESCAPE_MAIL_AUTH      6
#define NGX_ESCAPE_MAIL_XTEXT     7

#define NGX_UNESCAPE_URI       1
#define NGX_UNESCAPE_REDIRECT  2

uintptr_t ngx_escape_uri(u_char *dst, u_char *src, size_t size,
    ngx_uint_t type);
فراغ ngx_unescape_uri(u_char **dst, u_char **src, size_t size, ngx_uint_t type);
uintptr_t ngx_escape_html(u_char *dst, u_char *src, size_t size);
uintptr_t ngx_escape_json(u_char *dst, u_char *src, size_t size);


تعريفنوع بنية {
    ngx_rbtree_گرێ_t         گرێ;
    ngx_str_t                 str;
} ngx_str_گرێ_t;


فراغ ngx_str_rbtree_زیادکردن_بەها(ngx_rbtree_گرێ_t *temp,
    ngx_rbtree_گرێ_t *گرێ, ngx_rbtree_گرێ_t *sentinel);
ngx_str_گرێ_t *ngx_str_rbtree_lookup(ngx_rbtree_t *rbtree, ngx_str_t *name,
    uint32_t hash);


فراغ ngx_sort(فراغ *base, size_t n, size_t size,
    ngx_int_t (*cmp)(ثابت فراغ *, ثابت فراغ *));
#define ngx_qsort             qsort


#define ngx_بەها_helper(n)   #n
#define ngx_بەها(n)          ngx_بەها_helper(n)


#endif /* _NGX_STRING_H_INCLUDED_ */
