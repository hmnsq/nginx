
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_STRING_H_INCLUDED_
#define _NGX_STRING_H_INCLUDED_


#অন্তর্ভাব <ngx_config.h>
#অন্তর্ভাব <ngx_core.h>


প্রকারলেখ কাঠামো {
    আকার_t      len;
    u_char     *তথ্য;
} ngx_str_t;


প্রকারলেখ কাঠামো {
    ngx_str_t   চাবি;
    ngx_str_t   মান;
} ngx_keyval_t;


প্রকারলেখ কাঠামো {
    unsigned    len:28;

    unsigned    valid:1;
    unsigned    no_cacheable:1;
    unsigned    not_found:1;
    unsigned    escape:1;

    u_char     *তথ্য;
} ngx_variable_মান_t;


#define ngx_string(str)     { আকার(str) - 1, (u_char *) str }
#define ngx_null_string     { 0, ফাঁকা }
#define ngx_str_set(str, text)                                               \
    (str)->len = আকার(text) - 1; (str)->তথ্য = (u_char *) text
#define ngx_str_null(str)   (str)->len = 0; (str)->তথ্য = ফাঁকা


#define ngx_tolower(c)      (u_char) ((c >= 'A' && c <= 'Z') ? (c | 0x20) : c)
#define ngx_toupper(c)      (u_char) ((c >= 'a' && c <= 'z') ? (c & ~0x20) : c)

শূন্য ngx_strlow(u_char *dst, u_char *src, আকার_t n);


#define ngx_strncmp(s1, s2, n)  strncmp((ধ্রুবক অক্ষর *) s1, (ধ্রুবক অক্ষর *) s2, n)


/* msvc and icc7 compile strcmp() to inline loop */
#define ngx_strcmp(s1, s2)  strcmp((ধ্রুবক অক্ষর *) s1, (ধ্রুবক অক্ষর *) s2)


#define ngx_strstr(s1, s2)  strstr((ধ্রুবক অক্ষর *) s1, (ধ্রুবক অক্ষর *) s2)
#define ngx_strlen(s)       strlen((ধ্রুবক অক্ষর *) s)

আকার_t ngx_strnlen(u_char *p, আকার_t n);

#define ngx_strchr(s1, c)   strchr((ধ্রুবক অক্ষর *) s1, (পূর্ণসংখ্যা) c)

স্থির ngx_inline u_char *
ngx_strlchr(u_char *p, u_char *last, u_char c)
{
    যখন (p < last) {

        যদি (*p == c) {
            ফেরত p;
        }

        p++;
    }

    ফেরত ফাঁকা;
}


/*
 * msvc and icc7 compile memset() to the inline "rep stos"
 * while ZeroMemory() and bzero() are the calls.
 * icc7 may also inline several mov's of a zeroed register for small blocks.
 */
#define ngx_memzero(buf, n)       (শূন্য) memset(buf, 0, n)
#define ngx_memset(buf, c, n)     (শূন্য) memset(buf, c, n)

শূন্য ngx_explicit_memzero(শূন্য *buf, আকার_t n);


#যদি (NGX_MEMCPY_LIMIT)

শূন্য *ngx_memcpy(শূন্য *dst, ধ্রুবক শূন্য *src, আকার_t n);
#define ngx_cpymem(dst, src, n)   (((u_char *) ngx_memcpy(dst, src, n)) + (n))

#অন্যথা

/*
 * gcc3, msvc, and icc7 compile memcpy() to the inline "rep movs".
 * gcc3 compiles memcpy(d, s, 4) to the inline "mov"es.
 * icc8 compile memcpy(d, s, 4) to the inline "mov"es or XMM moves.
 */
#define ngx_memcpy(dst, src, n)   (শূন্য) memcpy(dst, src, n)
#define ngx_cpymem(dst, src, n)   (((u_char *) memcpy(dst, src, n)) + (n))

#endif


#যদি ( __INTEL_COMPILER >= 800 )

/*
 * the simple inline cycle copies the variable length strings up to 16
 * bytes faster than icc8 autodetecting _intel_fast_memcpy()
 */

স্থির ngx_inline u_char *
ngx_copy(u_char *dst, u_char *src, আকার_t len)
{
    যদি (len < 17) {

        যখন (len) {
            *dst++ = *src++;
            len--;
        }

        ফেরত dst;

    } অন্যথা {
        ফেরত ngx_cpymem(dst, src, len);
    }
}

#অন্যথা

#define ngx_copy                  ngx_cpymem

#endif


#define ngx_memmove(dst, src, n)  (শূন্য) memmove(dst, src, n)
#define ngx_movemem(dst, src, n)  (((u_char *) memmove(dst, src, n)) + (n))


/* msvc and icc7 compile memcmp() to the inline loop */
#define ngx_memcmp(s1, s2, n)     memcmp(s1, s2, n)


u_char *ngx_cpystrn(u_char *dst, u_char *src, আকার_t n);
u_char *ngx_pstrdup(ngx_pool_t *pool, ngx_str_t *src);
u_char * ngx_cdecl ngx_sprintf(u_char *buf, ধ্রুবক অক্ষর *fmt, ...);
u_char * ngx_cdecl ngx_snprintf(u_char *buf, আকার_t max, ধ্রুবক অক্ষর *fmt, ...);
u_char * ngx_cdecl ngx_slprintf(u_char *buf, u_char *last, ধ্রুবক অক্ষর *fmt,
    ...);
u_char *ngx_vslprintf(u_char *buf, u_char *last, ধ্রুবক অক্ষর *fmt, va_list args);
#define ngx_vsnprintf(buf, max, fmt, args)                                   \
    ngx_vslprintf(buf, buf + (max), fmt, args)

ngx_int_t ngx_strcasecmp(u_char *s1, u_char *s2);
ngx_int_t ngx_strncasecmp(u_char *s1, u_char *s2, আকার_t n);

u_char *ngx_strnstr(u_char *s1, অক্ষর *s2, আকার_t n);

u_char *ngx_strstrn(u_char *s1, অক্ষর *s2, আকার_t n);
u_char *ngx_strcasestrn(u_char *s1, অক্ষর *s2, আকার_t n);
u_char *ngx_strlcasestrn(u_char *s1, u_char *last, u_char *s2, আকার_t n);

ngx_int_t ngx_rstrncmp(u_char *s1, u_char *s2, আকার_t n);
ngx_int_t ngx_rstrncasecmp(u_char *s1, u_char *s2, আকার_t n);
ngx_int_t ngx_memn2cmp(u_char *s1, u_char *s2, আকার_t n1, আকার_t n2);
ngx_int_t ngx_dns_strcmp(u_char *s1, u_char *s2);
ngx_int_t ngx_filename_cmp(u_char *s1, u_char *s2, আকার_t n);

ngx_int_t ngx_atoi(u_char *সারি, আকার_t n);
ngx_int_t ngx_atofp(u_char *সারি, আকার_t n, আকার_t point);
ssize_t ngx_atosz(u_char *সারি, আকার_t n);
off_t ngx_atoof(u_char *সারি, আকার_t n);
time_t ngx_atotm(u_char *সারি, আকার_t n);
ngx_int_t ngx_hextoi(u_char *সারি, আকার_t n);

u_char *ngx_hex_dump(u_char *dst, u_char *src, আকার_t len);


#define ngx_base64_encoded_দৈর্ঘ্য(len)  (((len + 2) / 3) * 4)
#define ngx_base64_decoded_দৈর্ঘ্য(len)  (((len + 3) / 4) * 3)

শূন্য ngx_encode_base64(ngx_str_t *dst, ngx_str_t *src);
শূন্য ngx_encode_base64url(ngx_str_t *dst, ngx_str_t *src);
ngx_int_t ngx_decode_base64(ngx_str_t *dst, ngx_str_t *src);
ngx_int_t ngx_decode_base64url(ngx_str_t *dst, ngx_str_t *src);

uint32_t ngx_utf8_decode(u_char **p, আকার_t n);
আকার_t ngx_utf8_দৈর্ঘ্য(u_char *p, আকার_t n);
u_char *ngx_utf8_cpystrn(u_char *dst, u_char *src, আকার_t n, আকার_t len);


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

uintptr_t ngx_escape_uri(u_char *dst, u_char *src, আকার_t আকার_2,
    ngx_uint_t type);
শূন্য ngx_unescape_uri(u_char **dst, u_char **src, আকার_t আকার_2, ngx_uint_t type);
uintptr_t ngx_escape_html(u_char *dst, u_char *src, আকার_t আকার_2);
uintptr_t ngx_escape_json(u_char *dst, u_char *src, আকার_t আকার_2);


প্রকারলেখ কাঠামো {
    ngx_rbtree_গ্রন্থি_t         গ্রন্থি;
    ngx_str_t                 str;
} ngx_str_গ্রন্থি_t;


শূন্য ngx_str_rbtree_ঢোকাও_মান(ngx_rbtree_গ্রন্থি_t *অস্থায়ী,
    ngx_rbtree_গ্রন্থি_t *গ্রন্থি, ngx_rbtree_গ্রন্থি_t *sentinel);
ngx_str_গ্রন্থি_t *ngx_str_rbtree_lookup(ngx_rbtree_t *rbtree, ngx_str_t *name,
    uint32_t hash);


শূন্য ngx_sort(শূন্য *base, আকার_t n, আকার_t আকার_2,
    ngx_int_t (*cmp)(ধ্রুবক শূন্য *, ধ্রুবক শূন্য *));
#define ngx_qsort             qsort


#define ngx_মান_helper(n)   #n
#define ngx_মান(n)          ngx_মান_helper(n)


#endif /* _NGX_STRING_H_INCLUDED_ */
