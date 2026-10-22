
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_STRING_H_INCLUDED_
#define _NGX_STRING_H_INCLUDED_


#અન્તર્ભાવ <ngx_config.h>
#અન્તર્ભાવ <ngx_core.h>


પ્રકારવ્યાખ્યા સંરચના {
    size_t      len;
    u_char     *data;
} ngx_str_t;


પ્રકારવ્યાખ્યા સંરચના {
    ngx_str_t   ચાવી;
    ngx_str_t   કિંમત;
} ngx_keyval_t;


પ્રકારવ્યાખ્યા સંરચના {
    unsigned    len:28;

    unsigned    valid:1;
    unsigned    no_cacheable:1;
    unsigned    not_found:1;
    unsigned    escape:1;

    u_char     *data;
} ngx_variable_કિંમત_t;


#define ngx_string(str)     { કદ(str) - 1, (u_char *) str }
#define ngx_null_string     { 0, ખાલી }
#define ngx_str_set(str, text)                                               \
    (str)->len = કદ(text) - 1; (str)->data = (u_char *) text
#define ngx_str_null(str)   (str)->len = 0; (str)->data = ખાલી


#define ngx_tolower(c)      (u_char) ((c >= 'A' && c <= 'Z') ? (c | 0x20) : c)
#define ngx_toupper(c)      (u_char) ((c >= 'a' && c <= 'z') ? (c & ~0x20) : c)

શૂન્ય ngx_strlow(u_char *dst, u_char *src, size_t n);


#define ngx_strncmp(s1, s2, n)  strncmp((અચલ અક્ષર *) s1, (અચલ અક્ષર *) s2, n)


/* msvc and icc7 compile strcmp() to inline loop */
#define ngx_strcmp(s1, s2)  strcmp((અચલ અક્ષર *) s1, (અચલ અક્ષર *) s2)


#define ngx_strstr(s1, s2)  strstr((અચલ અક્ષર *) s1, (અચલ અક્ષર *) s2)
#define ngx_strlen(s)       strlen((અચલ અક્ષર *) s)

size_t ngx_strnlen(u_char *p, size_t n);

#define ngx_strchr(s1, c)   strchr((અચલ અક્ષર *) s1, (પૂર્ણાંક) c)

સ્થિર ngx_inline u_char *
ngx_strlchr(u_char *p, u_char *last, u_char c)
{
    જ્યારે (p < last) {

        જો (*p == c) {
            પાછા p;
        }

        p++;
    }

    પાછા ખાલી;
}


/*
 * msvc and icc7 compile memset() to the inline "rep stos"
 * while ZeroMemory() and bzero() are the calls.
 * icc7 may also inline several mov's of a zeroed register for small blocks.
 */
#define ngx_memzero(buf, n)       (શૂન્ય) memset(buf, 0, n)
#define ngx_memset(buf, c, n)     (શૂન્ય) memset(buf, c, n)

શૂન્ય ngx_explicit_memzero(શૂન્ય *buf, size_t n);


#જો (NGX_MEMCPY_LIMIT)

શૂન્ય *ngx_memcpy(શૂન્ય *dst, અચલ શૂન્ય *src, size_t n);
#define ngx_cpymem(dst, src, n)   (((u_char *) ngx_memcpy(dst, src, n)) + (n))

#નહીંતર

/*
 * gcc3, msvc, and icc7 compile memcpy() to the inline "rep movs".
 * gcc3 compiles memcpy(d, s, 4) to the inline "mov"es.
 * icc8 compile memcpy(d, s, 4) to the inline "mov"es or XMM moves.
 */
#define ngx_memcpy(dst, src, n)   (શૂન્ય) memcpy(dst, src, n)
#define ngx_cpymem(dst, src, n)   (((u_char *) memcpy(dst, src, n)) + (n))

#endif


#જો ( __INTEL_COMPILER >= 800 )

/*
 * the simple inline cycle copies the variable length strings up to 16
 * bytes faster than icc8 autodetecting _intel_fast_memcpy()
 */

સ્થિર ngx_inline u_char *
ngx_copy(u_char *dst, u_char *src, size_t len)
{
    જો (len < 17) {

        જ્યારે (len) {
            *dst++ = *src++;
            len--;
        }

        પાછા dst;

    } નહીંતર {
        પાછા ngx_cpymem(dst, src, len);
    }
}

#નહીંતર

#define ngx_copy                  ngx_cpymem

#endif


#define ngx_memmove(dst, src, n)  (શૂન્ય) memmove(dst, src, n)
#define ngx_movemem(dst, src, n)  (((u_char *) memmove(dst, src, n)) + (n))


/* msvc and icc7 compile memcmp() to the inline loop */
#define ngx_memcmp(s1, s2, n)     memcmp(s1, s2, n)


u_char *ngx_cpystrn(u_char *dst, u_char *src, size_t n);
u_char *ngx_pstrdup(ngx_pool_t *pool, ngx_str_t *src);
u_char * ngx_cdecl ngx_sprintf(u_char *buf, અચલ અક્ષર *fmt, ...);
u_char * ngx_cdecl ngx_snprintf(u_char *buf, size_t max, અચલ અક્ષર *fmt, ...);
u_char * ngx_cdecl ngx_slprintf(u_char *buf, u_char *last, અચલ અક્ષર *fmt,
    ...);
u_char *ngx_vslprintf(u_char *buf, u_char *last, અચલ અક્ષર *fmt, va_list args);
#define ngx_vsnprintf(buf, max, fmt, args)                                   \
    ngx_vslprintf(buf, buf + (max), fmt, args)

ngx_int_t ngx_strcasecmp(u_char *s1, u_char *s2);
ngx_int_t ngx_strncasecmp(u_char *s1, u_char *s2, size_t n);

u_char *ngx_strnstr(u_char *s1, અક્ષર *s2, size_t n);

u_char *ngx_strstrn(u_char *s1, અક્ષર *s2, size_t n);
u_char *ngx_strcasestrn(u_char *s1, અક્ષર *s2, size_t n);
u_char *ngx_strlcasestrn(u_char *s1, u_char *last, u_char *s2, size_t n);

ngx_int_t ngx_rstrncmp(u_char *s1, u_char *s2, size_t n);
ngx_int_t ngx_rstrncasecmp(u_char *s1, u_char *s2, size_t n);
ngx_int_t ngx_memn2cmp(u_char *s1, u_char *s2, size_t n1, size_t n2);
ngx_int_t ngx_dns_strcmp(u_char *s1, u_char *s2);
ngx_int_t ngx_filename_cmp(u_char *s1, u_char *s2, size_t n);

ngx_int_t ngx_atoi(u_char *લીટી, size_t n);
ngx_int_t ngx_atofp(u_char *લીટી, size_t n, size_t point);
ssize_t ngx_atosz(u_char *લીટી, size_t n);
off_t ngx_atoof(u_char *લીટી, size_t n);
time_t ngx_atotm(u_char *લીટી, size_t n);
ngx_int_t ngx_hextoi(u_char *લીટી, size_t n);

u_char *ngx_hex_dump(u_char *dst, u_char *src, size_t len);


#define ngx_base64_encoded_length(len)  (((len + 2) / 3) * 4)
#define ngx_base64_decoded_length(len)  (((len + 3) / 4) * 3)

શૂન્ય ngx_encode_base64(ngx_str_t *dst, ngx_str_t *src);
શૂન્ય ngx_encode_base64url(ngx_str_t *dst, ngx_str_t *src);
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
શૂન્ય ngx_unescape_uri(u_char **dst, u_char **src, size_t size, ngx_uint_t type);
uintptr_t ngx_escape_html(u_char *dst, u_char *src, size_t size);
uintptr_t ngx_escape_json(u_char *dst, u_char *src, size_t size);


પ્રકારવ્યાખ્યા સંરચના {
    ngx_rbtree_ગાંઠ_t         ગાંઠ;
    ngx_str_t                 str;
} ngx_str_ગાંઠ_t;


શૂન્ય ngx_str_rbtree_ઉમેરો_કિંમત(ngx_rbtree_ગાંઠ_t *temp,
    ngx_rbtree_ગાંઠ_t *ગાંઠ, ngx_rbtree_ગાંઠ_t *sentinel);
ngx_str_ગાંઠ_t *ngx_str_rbtree_lookup(ngx_rbtree_t *rbtree, ngx_str_t *name,
    uint32_t hash);


શૂન્ય ngx_sort(શૂન્ય *base, size_t n, size_t size,
    ngx_int_t (*cmp)(અચલ શૂન્ય *, અચલ શૂન્ય *));
#define ngx_qsort             qsort


#define ngx_કિંમત_helper(n)   #n
#define ngx_કિંમત(n)          ngx_કિંમત_helper(n)


#endif /* _NGX_STRING_H_INCLUDED_ */
