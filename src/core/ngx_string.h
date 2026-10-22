
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_STRING_H_INCLUDED_
#define _NGX_STRING_H_INCLUDED_


#ଅନ୍ତର୍ଭାଵ <ngx_config.h>
#ଅନ୍ତର୍ଭାଵ <ngx_core.h>


ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    size_t      len;
    u_char     *data;
} ngx_str_t;


ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ngx_str_t   ଚାବି;
    ngx_str_t   ମୂଲ୍ୟ;
} ngx_keyval_t;


ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    unsigned    len:28;

    unsigned    valid:1;
    unsigned    no_cacheable:1;
    unsigned    not_found:1;
    unsigned    escape:1;

    u_char     *data;
} ngx_variable_ମୂଲ୍ୟ_t;


#define ngx_string(str)     { ଆକାର(str) - 1, (u_char *) str }
#define ngx_null_string     { 0, ଖାଲି }
#define ngx_str_set(str, text)                                               \
    (str)->len = ଆକାର(text) - 1; (str)->data = (u_char *) text
#define ngx_str_null(str)   (str)->len = 0; (str)->data = ଖାଲି


#define ngx_tolower(c)      (u_char) ((c >= 'A' && c <= 'Z') ? (c | 0x20) : c)
#define ngx_toupper(c)      (u_char) ((c >= 'a' && c <= 'z') ? (c & ~0x20) : c)

ଶୂନ୍ୟ ngx_strlow(u_char *dst, u_char *src, size_t n);


#define ngx_strncmp(s1, s2, n)  strncmp((ଅଚଳ ଅକ୍ଷର *) s1, (ଅଚଳ ଅକ୍ଷର *) s2, n)


/* msvc and icc7 compile strcmp() to inline loop */
#define ngx_strcmp(s1, s2)  strcmp((ଅଚଳ ଅକ୍ଷର *) s1, (ଅଚଳ ଅକ୍ଷର *) s2)


#define ngx_strstr(s1, s2)  strstr((ଅଚଳ ଅକ୍ଷର *) s1, (ଅଚଳ ଅକ୍ଷର *) s2)
#define ngx_strlen(s)       strlen((ଅଚଳ ଅକ୍ଷର *) s)

size_t ngx_strnlen(u_char *p, size_t n);

#define ngx_strchr(s1, c)   strchr((ଅଚଳ ଅକ୍ଷର *) s1, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) c)

ସ୍ଥିର ngx_inline u_char *
ngx_strlchr(u_char *p, u_char *last, u_char c)
{
    ଯେତେବେଳେ (p < last) {

        ଯଦି (*p == c) {
            ଫେରନ୍ତୁ p;
        }

        p++;
    }

    ଫେରନ୍ତୁ ଖାଲି;
}


/*
 * msvc and icc7 compile memset() to the inline "rep stos"
 * while ZeroMemory() and bzero() are the calls.
 * icc7 may also inline several mov's of a zeroed register for small blocks.
 */
#define ngx_memzero(buf, n)       (ଶୂନ୍ୟ) memset(buf, 0, n)
#define ngx_memset(buf, c, n)     (ଶୂନ୍ୟ) memset(buf, c, n)

ଶୂନ୍ୟ ngx_explicit_memzero(ଶୂନ୍ୟ *buf, size_t n);


#ଯଦି (NGX_MEMCPY_LIMIT)

ଶୂନ୍ୟ *ngx_memcpy(ଶୂନ୍ୟ *dst, ଅଚଳ ଶୂନ୍ୟ *src, size_t n);
#define ngx_cpymem(dst, src, n)   (((u_char *) ngx_memcpy(dst, src, n)) + (n))

#ଅନ୍ୟଥା

/*
 * gcc3, msvc, and icc7 compile memcpy() to the inline "rep movs".
 * gcc3 compiles memcpy(d, s, 4) to the inline "mov"es.
 * icc8 compile memcpy(d, s, 4) to the inline "mov"es or XMM moves.
 */
#define ngx_memcpy(dst, src, n)   (ଶୂନ୍ୟ) memcpy(dst, src, n)
#define ngx_cpymem(dst, src, n)   (((u_char *) memcpy(dst, src, n)) + (n))

#endif


#ଯଦି ( __INTEL_COMPILER >= 800 )

/*
 * the simple inline cycle copies the variable length strings up to 16
 * bytes faster than icc8 autodetecting _intel_fast_memcpy()
 */

ସ୍ଥିର ngx_inline u_char *
ngx_copy(u_char *dst, u_char *src, size_t len)
{
    ଯଦି (len < 17) {

        ଯେତେବେଳେ (len) {
            *dst++ = *src++;
            len--;
        }

        ଫେରନ୍ତୁ dst;

    } ଅନ୍ୟଥା {
        ଫେରନ୍ତୁ ngx_cpymem(dst, src, len);
    }
}

#ଅନ୍ୟଥା

#define ngx_copy                  ngx_cpymem

#endif


#define ngx_memmove(dst, src, n)  (ଶୂନ୍ୟ) memmove(dst, src, n)
#define ngx_movemem(dst, src, n)  (((u_char *) memmove(dst, src, n)) + (n))


/* msvc and icc7 compile memcmp() to the inline loop */
#define ngx_memcmp(s1, s2, n)     memcmp(s1, s2, n)


u_char *ngx_cpystrn(u_char *dst, u_char *src, size_t n);
u_char *ngx_pstrdup(ngx_pool_t *pool, ngx_str_t *src);
u_char * ngx_cdecl ngx_sprintf(u_char *buf, ଅଚଳ ଅକ୍ଷର *fmt, ...);
u_char * ngx_cdecl ngx_snprintf(u_char *buf, size_t max, ଅଚଳ ଅକ୍ଷର *fmt, ...);
u_char * ngx_cdecl ngx_slprintf(u_char *buf, u_char *last, ଅଚଳ ଅକ୍ଷର *fmt,
    ...);
u_char *ngx_vslprintf(u_char *buf, u_char *last, ଅଚଳ ଅକ୍ଷର *fmt, va_list args);
#define ngx_vsnprintf(buf, max, fmt, args)                                   \
    ngx_vslprintf(buf, buf + (max), fmt, args)

ngx_int_t ngx_strcasecmp(u_char *s1, u_char *s2);
ngx_int_t ngx_strncasecmp(u_char *s1, u_char *s2, size_t n);

u_char *ngx_strnstr(u_char *s1, ଅକ୍ଷର *s2, size_t n);

u_char *ngx_strstrn(u_char *s1, ଅକ୍ଷର *s2, size_t n);
u_char *ngx_strcasestrn(u_char *s1, ଅକ୍ଷର *s2, size_t n);
u_char *ngx_strlcasestrn(u_char *s1, u_char *last, u_char *s2, size_t n);

ngx_int_t ngx_rstrncmp(u_char *s1, u_char *s2, size_t n);
ngx_int_t ngx_rstrncasecmp(u_char *s1, u_char *s2, size_t n);
ngx_int_t ngx_memn2cmp(u_char *s1, u_char *s2, size_t n1, size_t n2);
ngx_int_t ngx_dns_strcmp(u_char *s1, u_char *s2);
ngx_int_t ngx_filename_cmp(u_char *s1, u_char *s2, size_t n);

ngx_int_t ngx_atoi(u_char *ଧାଡ଼ି, size_t n);
ngx_int_t ngx_atofp(u_char *ଧାଡ଼ି, size_t n, size_t point);
ssize_t ngx_atosz(u_char *ଧାଡ଼ି, size_t n);
off_t ngx_atoof(u_char *ଧାଡ଼ି, size_t n);
time_t ngx_atotm(u_char *ଧାଡ଼ି, size_t n);
ngx_int_t ngx_hextoi(u_char *ଧାଡ଼ି, size_t n);

u_char *ngx_hex_dump(u_char *dst, u_char *src, size_t len);


#define ngx_base64_encoded_length(len)  (((len + 2) / 3) * 4)
#define ngx_base64_decoded_length(len)  (((len + 3) / 4) * 3)

ଶୂନ୍ୟ ngx_encode_base64(ngx_str_t *dst, ngx_str_t *src);
ଶୂନ୍ୟ ngx_encode_base64url(ngx_str_t *dst, ngx_str_t *src);
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
ଶୂନ୍ୟ ngx_unescape_uri(u_char **dst, u_char **src, size_t size, ngx_uint_t type);
uintptr_t ngx_escape_html(u_char *dst, u_char *src, size_t size);
uintptr_t ngx_escape_json(u_char *dst, u_char *src, size_t size);


ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ngx_rbtree_ଗଣ୍ଠି_t         ଗଣ୍ଠି;
    ngx_str_t                 str;
} ngx_str_ଗଣ୍ଠି_t;


ଶୂନ୍ୟ ngx_str_rbtree_ଭର୍ତ୍ତି_ମୂଲ୍ୟ(ngx_rbtree_ଗଣ୍ଠି_t *temp,
    ngx_rbtree_ଗଣ୍ଠି_t *ଗଣ୍ଠି, ngx_rbtree_ଗଣ୍ଠି_t *sentinel);
ngx_str_ଗଣ୍ଠି_t *ngx_str_rbtree_lookup(ngx_rbtree_t *rbtree, ngx_str_t *name,
    uint32_t hash);


ଶୂନ୍ୟ ngx_sort(ଶୂନ୍ୟ *base, size_t n, size_t size,
    ngx_int_t (*cmp)(ଅଚଳ ଶୂନ୍ୟ *, ଅଚଳ ଶୂନ୍ୟ *));
#define ngx_qsort             qsort


#define ngx_ମୂଲ୍ୟ_helper(n)   #n
#define ngx_ମୂଲ୍ୟ(n)          ngx_ମୂଲ୍ୟ_helper(n)


#endif /* _NGX_STRING_H_INCLUDED_ */
