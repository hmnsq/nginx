
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_STRING_H_INCLUDED_
#define _NGX_STRING_H_INCLUDED_


#അന്തര്ഭാവ <ngx_config.h>
#അന്തര്ഭാവ <ngx_core.h>


തരനിർവചനം ഘടന {
    size_t      len;
    u_char     *data;
} ngx_str_t;


തരനിർവചനം ഘടന {
    ngx_str_t   താക്കോൽ;
    ngx_str_t   മൂല്യം;
} ngx_keyval_t;


തരനിർവചനം ഘടന {
    unsigned    len:28;

    unsigned    valid:1;
    unsigned    no_cacheable:1;
    unsigned    not_found:1;
    unsigned    escape:1;

    u_char     *data;
} ngx_variable_മൂല്യം_t;


#define ngx_string(str)     { വലുപ്പം(str) - 1, (u_char *) str }
#define ngx_null_string     { 0, ശൂന്യ }
#define ngx_str_set(str, text)                                               \
    (str)->len = വലുപ്പം(text) - 1; (str)->data = (u_char *) text
#define ngx_str_null(str)   (str)->len = 0; (str)->data = ശൂന്യ


#define ngx_tolower(c)      (u_char) ((c >= 'A' && c <= 'Z') ? (c | 0x20) : c)
#define ngx_toupper(c)      (u_char) ((c >= 'a' && c <= 'z') ? (c & ~0x20) : c)

ശൂന്യം ngx_strlow(u_char *dst, u_char *src, size_t n);


#define ngx_strncmp(s1, s2, n)  strncmp((സ്ഥിരം അക്ഷരം *) s1, (സ്ഥിരം അക്ഷരം *) s2, n)


/* msvc and icc7 compile strcmp() to inline loop */
#define ngx_strcmp(s1, s2)  strcmp((സ്ഥിരം അക്ഷരം *) s1, (സ്ഥിരം അക്ഷരം *) s2)


#define ngx_strstr(s1, s2)  strstr((സ്ഥിരം അക്ഷരം *) s1, (സ്ഥിരം അക്ഷരം *) s2)
#define ngx_strlen(s)       strlen((സ്ഥിരം അക്ഷരം *) s)

size_t ngx_strnlen(u_char *p, size_t n);

#define ngx_strchr(s1, c)   strchr((സ്ഥിരം അക്ഷരം *) s1, (പൂർണ്ണസംഖ്യ) c)

സ്ഥിതം ngx_inline u_char *
ngx_strlchr(u_char *p, u_char *last, u_char c)
{
    ആകുമ്പോൾ (p < last) {

        എങ്കിൽ (*p == c) {
            തിരികെ p;
        }

        p++;
    }

    തിരികെ ശൂന്യ;
}


/*
 * msvc and icc7 compile memset() to the inline "rep stos"
 * while ZeroMemory() and bzero() are the calls.
 * icc7 may also inline several mov's of a zeroed register for small blocks.
 */
#define ngx_memzero(buf, n)       (ശൂന്യം) memset(buf, 0, n)
#define ngx_memset(buf, c, n)     (ശൂന്യം) memset(buf, c, n)

ശൂന്യം ngx_explicit_memzero(ശൂന്യം *buf, size_t n);


#എങ്കിൽ (NGX_MEMCPY_LIMIT)

ശൂന്യം *ngx_memcpy(ശൂന്യം *dst, സ്ഥിരം ശൂന്യം *src, size_t n);
#define ngx_cpymem(dst, src, n)   (((u_char *) ngx_memcpy(dst, src, n)) + (n))

#അല്ലെങ്കിൽ

/*
 * gcc3, msvc, and icc7 compile memcpy() to the inline "rep movs".
 * gcc3 compiles memcpy(d, s, 4) to the inline "mov"es.
 * icc8 compile memcpy(d, s, 4) to the inline "mov"es or XMM moves.
 */
#define ngx_memcpy(dst, src, n)   (ശൂന്യം) memcpy(dst, src, n)
#define ngx_cpymem(dst, src, n)   (((u_char *) memcpy(dst, src, n)) + (n))

#endif


#എങ്കിൽ ( __INTEL_COMPILER >= 800 )

/*
 * the simple inline cycle copies the variable length strings up to 16
 * bytes faster than icc8 autodetecting _intel_fast_memcpy()
 */

സ്ഥിതം ngx_inline u_char *
ngx_copy(u_char *dst, u_char *src, size_t len)
{
    എങ്കിൽ (len < 17) {

        ആകുമ്പോൾ (len) {
            *dst++ = *src++;
            len--;
        }

        തിരികെ dst;

    } അല്ലെങ്കിൽ {
        തിരികെ ngx_cpymem(dst, src, len);
    }
}

#അല്ലെങ്കിൽ

#define ngx_copy                  ngx_cpymem

#endif


#define ngx_memmove(dst, src, n)  (ശൂന്യം) memmove(dst, src, n)
#define ngx_movemem(dst, src, n)  (((u_char *) memmove(dst, src, n)) + (n))


/* msvc and icc7 compile memcmp() to the inline loop */
#define ngx_memcmp(s1, s2, n)     memcmp(s1, s2, n)


u_char *ngx_cpystrn(u_char *dst, u_char *src, size_t n);
u_char *ngx_pstrdup(ngx_pool_t *pool, ngx_str_t *src);
u_char * ngx_cdecl ngx_sprintf(u_char *buf, സ്ഥിരം അക്ഷരം *fmt, ...);
u_char * ngx_cdecl ngx_snprintf(u_char *buf, size_t max, സ്ഥിരം അക്ഷരം *fmt, ...);
u_char * ngx_cdecl ngx_slprintf(u_char *buf, u_char *last, സ്ഥിരം അക്ഷരം *fmt,
    ...);
u_char *ngx_vslprintf(u_char *buf, u_char *last, സ്ഥിരം അക്ഷരം *fmt, va_list args);
#define ngx_vsnprintf(buf, max, fmt, args)                                   \
    ngx_vslprintf(buf, buf + (max), fmt, args)

ngx_int_t ngx_strcasecmp(u_char *s1, u_char *s2);
ngx_int_t ngx_strncasecmp(u_char *s1, u_char *s2, size_t n);

u_char *ngx_strnstr(u_char *s1, അക്ഷരം *s2, size_t n);

u_char *ngx_strstrn(u_char *s1, അക്ഷരം *s2, size_t n);
u_char *ngx_strcasestrn(u_char *s1, അക്ഷരം *s2, size_t n);
u_char *ngx_strlcasestrn(u_char *s1, u_char *last, u_char *s2, size_t n);

ngx_int_t ngx_rstrncmp(u_char *s1, u_char *s2, size_t n);
ngx_int_t ngx_rstrncasecmp(u_char *s1, u_char *s2, size_t n);
ngx_int_t ngx_memn2cmp(u_char *s1, u_char *s2, size_t n1, size_t n2);
ngx_int_t ngx_dns_strcmp(u_char *s1, u_char *s2);
ngx_int_t ngx_filename_cmp(u_char *s1, u_char *s2, size_t n);

ngx_int_t ngx_atoi(u_char *വരി, size_t n);
ngx_int_t ngx_atofp(u_char *വരി, size_t n, size_t point);
ssize_t ngx_atosz(u_char *വരി, size_t n);
off_t ngx_atoof(u_char *വരി, size_t n);
time_t ngx_atotm(u_char *വരി, size_t n);
ngx_int_t ngx_hextoi(u_char *വരി, size_t n);

u_char *ngx_hex_dump(u_char *dst, u_char *src, size_t len);


#define ngx_base64_encoded_length(len)  (((len + 2) / 3) * 4)
#define ngx_base64_decoded_length(len)  (((len + 3) / 4) * 3)

ശൂന്യം ngx_encode_base64(ngx_str_t *dst, ngx_str_t *src);
ശൂന്യം ngx_encode_base64url(ngx_str_t *dst, ngx_str_t *src);
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
ശൂന്യം ngx_unescape_uri(u_char **dst, u_char **src, size_t size, ngx_uint_t type);
uintptr_t ngx_escape_html(u_char *dst, u_char *src, size_t size);
uintptr_t ngx_escape_json(u_char *dst, u_char *src, size_t size);


തരനിർവചനം ഘടന {
    ngx_rbtree_കണ്ണി_t         കണ്ണി;
    ngx_str_t                 str;
} ngx_str_കണ്ണി_t;


ശൂന്യം ngx_str_rbtree_തിരുകുക_മൂല്യം(ngx_rbtree_കണ്ണി_t *temp,
    ngx_rbtree_കണ്ണി_t *കണ്ണി, ngx_rbtree_കണ്ണി_t *sentinel);
ngx_str_കണ്ണി_t *ngx_str_rbtree_lookup(ngx_rbtree_t *rbtree, ngx_str_t *name,
    uint32_t hash);


ശൂന്യം ngx_sort(ശൂന്യം *base, size_t n, size_t size,
    ngx_int_t (*cmp)(സ്ഥിരം ശൂന്യം *, സ്ഥിരം ശൂന്യം *));
#define ngx_qsort             qsort


#define ngx_മൂല്യം_helper(n)   #n
#define ngx_മൂല്യം(n)          ngx_മൂല്യം_helper(n)


#endif /* _NGX_STRING_H_INCLUDED_ */
