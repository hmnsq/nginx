
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ _NGX_STRING_H_INCLUDED_
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _NGX_STRING_H_INCLUDED_


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ਮਾਪ_t      len;
    u_char     *ਅੰਕੜੇ;
} ngx_str_t;


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ngx_str_t   ਕੁੰਜੀ;
    ngx_str_t   ਮੁੱਲ;
} ngx_keyval_t;


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ਬਿਨਾਨਿਸ਼ਾਨੀ    len:28;

    ਬਿਨਾਨਿਸ਼ਾਨੀ    valid:1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ    no_cacheable:1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ    not_ਮਿਲਿਆ:1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ    escape:1;

    u_char     *ਅੰਕੜੇ;
} ngx_variable_ਮੁੱਲ_t;


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_string(str)     { ਆਕਾਰ(str) - 1, (u_char *) str }
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_null_string     { 0, ਨਿਰਾ }
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_str_ਸੈੱਟ(str, ਪਾਠ)                                               \
    (str)->len = ਆਕਾਰ(ਪਾਠ) - 1; (str)->ਅੰਕੜੇ = (u_char *) ਪਾਠ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_str_null(str)   (str)->len = 0; (str)->ਅੰਕੜੇ = ਨਿਰਾ


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_tolower(c)      (u_char) ((c >= 'A' && c <= 'Z') ? (c | 0x20) : c)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_toupper(c)      (u_char) ((c >= 'a' && c <= 'z') ? (c & ~0x20) : c)

ਖਾਲੀ ngx_strlow(u_char *dst, u_char *src, ਮਾਪ_t n);


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_strncmp(s1, s2, n)  ਲੜੀ_ਤੁਲਨਾ((ਅਚਲ ਅੱਖਰ *) s1, (ਅਚਲ ਅੱਖਰ *) s2, n)


/* msvc and icc7 compile strcmp() to inline loop */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_strcmp(s1, s2)  strcmp((ਅਚਲ ਅੱਖਰ *) s1, (ਅਚਲ ਅੱਖਰ *) s2)


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_strstr(s1, s2)  ਲੜੀ_ਖੋਜ((ਅਚਲ ਅੱਖਰ *) s1, (ਅਚਲ ਅੱਖਰ *) s2)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_strlen(s)       ਲੜੀ_ਲੰਬਾਈ((ਅਚਲ ਅੱਖਰ *) s)

ਮਾਪ_t ngx_strnlen(u_char *p, ਮਾਪ_t n);

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_strchr(s1, c)   strchr((ਅਚਲ ਅੱਖਰ *) s1, (ਪੂਰਨ) c)

ਸਥਿਰ ngx_inline u_char *
ngx_strlchr(u_char *p, u_char *ਆਖਰੀ, u_char c)
{
    ਜਦੋਂ (p < ਆਖਰੀ) {

        ਜੇ (*p == c) {
            ਵਾਪਸ p;
        }

        p++;
    }

    ਵਾਪਸ ਨਿਰਾ;
}


/*
 * msvc and icc7 compile memset() to the inline "rep stos"
 * while ZeroMemory() and bzero() are the calls.
 * icc7 may also inline several mov's of a zeroed register for small blocks.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_memzero(buf, n)       (ਖਾਲੀ) memset(buf, 0, n)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_memset(buf, c, n)     (ਖਾਲੀ) memset(buf, c, n)

ਖਾਲੀ ngx_explicit_memzero(ਖਾਲੀ *buf, ਮਾਪ_t n);


#ਜੇ (NGX_MEMCPY_ਹੱਦਬੰਦੀ)

ਖਾਲੀ *ngx_memcpy(ਖਾਲੀ *dst, ਅਚਲ ਖਾਲੀ *src, ਮਾਪ_t n);
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_cpymem(dst, src, n)   (((u_char *) ngx_memcpy(dst, src, n)) + (n))

#ਨਹੀਂਤਾਂ

/*
 * gcc3, msvc, and icc7 compile memcpy() to the inline "rep movs".
 * gcc3 compiles memcpy(d, s, 4) to the inline "mov"es.
 * icc8 compile memcpy(d, s, 4) to the inline "mov"es or XMM moves.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_memcpy(dst, src, n)   (ਖਾਲੀ) ਸਮ੍ਰਿਤੀ_ਨਕਲ(dst, src, n)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_cpymem(dst, src, n)   (((u_char *) ਸਮ੍ਰਿਤੀ_ਨਕਲ(dst, src, n)) + (n))

#ਜੇ_ਅੰਤ


#ਜੇ ( __INTEL_COMPILER >= 800 )

/*
 * the simple inline cycle copies the variable length strings up to 16
 * bytes faster than icc8 autodetecting _intel_fast_memcpy()
 */

ਸਥਿਰ ngx_inline u_char *
ngx_copy(u_char *dst, u_char *src, ਮਾਪ_t len)
{
    ਜੇ (len < 17) {

        ਜਦੋਂ (len) {
            *dst++ = *src++;
            len--;
        }

        ਵਾਪਸ dst;

    } ਨਹੀਂਤਾਂ {
        ਵਾਪਸ ngx_cpymem(dst, src, len);
    }
}

#ਨਹੀਂਤਾਂ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_copy                  ngx_cpymem

#ਜੇ_ਅੰਤ


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_memmove(dst, src, n)  (ਖਾਲੀ) memmove(dst, src, n)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_movemem(dst, src, n)  (((u_char *) memmove(dst, src, n)) + (n))


/* msvc and icc7 compile memcmp() to the inline loop */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_memcmp(s1, s2, n)     memcmp(s1, s2, n)


u_char *ngx_cpystrn(u_char *dst, u_char *src, ਮਾਪ_t n);
u_char *ngx_pstrdup(ngx_pool_t *pool, ngx_str_t *src);
u_char * ngx_cdecl ngx_sprintf(u_char *buf, ਅਚਲ ਅੱਖਰ *fmt, ...);
u_char * ngx_cdecl ngx_snprintf(u_char *buf, ਮਾਪ_t max, ਅਚਲ ਅੱਖਰ *fmt, ...);
u_char * ngx_cdecl ngx_slprintf(u_char *buf, u_char *ਆਖਰੀ, ਅਚਲ ਅੱਖਰ *fmt,
    ...);
u_char *ngx_vslprintf(u_char *buf, u_char *ਆਖਰੀ, ਅਚਲ ਅੱਖਰ *fmt, va_list ਦਲੀਲਾਂ);
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_vsnprintf(buf, max, fmt, ਦਲੀਲਾਂ)                                   \
    ngx_vslprintf(buf, buf + (max), fmt, ਦਲੀਲਾਂ)

ngx_int_t ngx_strcasecmp(u_char *s1, u_char *s2);
ngx_int_t ngx_strncasecmp(u_char *s1, u_char *s2, ਮਾਪ_t n);

u_char *ngx_strnstr(u_char *s1, ਅੱਖਰ *s2, ਮਾਪ_t n);

u_char *ngx_strstrn(u_char *s1, ਅੱਖਰ *s2, ਮਾਪ_t n);
u_char *ngx_strcasestrn(u_char *s1, ਅੱਖਰ *s2, ਮਾਪ_t n);
u_char *ngx_strlcasestrn(u_char *s1, u_char *ਆਖਰੀ, u_char *s2, ਮਾਪ_t n);

ngx_int_t ngx_rstrncmp(u_char *s1, u_char *s2, ਮਾਪ_t n);
ngx_int_t ngx_rstrncasecmp(u_char *s1, u_char *s2, ਮਾਪ_t n);
ngx_int_t ngx_memn2cmp(u_char *s1, u_char *s2, ਮਾਪ_t n1, ਮਾਪ_t n2);
ngx_int_t ngx_dns_strcmp(u_char *s1, u_char *s2);
ngx_int_t ngx_filename_cmp(u_char *s1, u_char *s2, ਮਾਪ_t n);

ngx_int_t ngx_atoi(u_char *ਸਤਰ, ਮਾਪ_t n);
ngx_int_t ngx_atofp(u_char *ਸਤਰ, ਮਾਪ_t n, ਮਾਪ_t ਬਿੰਦੂ);
ssize_t ngx_atosz(u_char *ਸਤਰ, ਮਾਪ_t n);
off_t ngx_atoof(u_char *ਸਤਰ, ਮਾਪ_t n);
ਸਮਾਂ_t ngx_atotm(u_char *ਸਤਰ, ਮਾਪ_t n);
ngx_int_t ngx_hextoi(u_char *ਸਤਰ, ਮਾਪ_t n);

u_char *ngx_hex_dump(u_char *dst, u_char *src, ਮਾਪ_t len);


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_base64_encoded_ਲੰਮਾਈ(len)  (((len + 2) / 3) * 4)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_base64_decoded_ਲੰਮਾਈ(len)  (((len + 3) / 4) * 3)

ਖਾਲੀ ngx_encode_base64(ngx_str_t *dst, ngx_str_t *src);
ਖਾਲੀ ngx_encode_base64url(ngx_str_t *dst, ngx_str_t *src);
ngx_int_t ngx_decode_base64(ngx_str_t *dst, ngx_str_t *src);
ngx_int_t ngx_decode_base64url(ngx_str_t *dst, ngx_str_t *src);

uint32_t ngx_utf8_decode(u_char **p, ਮਾਪ_t n);
ਮਾਪ_t ngx_utf8_ਲੰਮਾਈ(u_char *p, ਮਾਪ_t n);
u_char *ngx_utf8_cpystrn(u_char *dst, u_char *src, ਮਾਪ_t n, ਮਾਪ_t len);


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_ESCAPE_URI            0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_ESCAPE_ਦਲੀਲਾਂ           1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_ESCAPE_URI_COMPONENT  2
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_ESCAPE_HTML           3
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_ESCAPE_REFRESH        4
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_ESCAPE_MEMCACHED      5
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_ESCAPE_MAIL_AUTH      6
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_ESCAPE_MAIL_XTEXT     7

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_UNESCAPE_URI       1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_UNESCAPE_REDIRECT  2

uintptr_t ngx_escape_uri(u_char *dst, u_char *src, ਮਾਪ_t ਮਾਪ,
    ngx_uint_t type);
ਖਾਲੀ ngx_unescape_uri(u_char **dst, u_char **src, ਮਾਪ_t ਮਾਪ, ngx_uint_t type);
uintptr_t ngx_escape_html(u_char *dst, u_char *src, ਮਾਪ_t ਮਾਪ);
uintptr_t ngx_escape_json(u_char *dst, u_char *src, ਮਾਪ_t ਮਾਪ);


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ngx_rbtree_ਗੰਢ_t         ਗੰਢ;
    ngx_str_t                 str;
} ngx_str_ਗੰਢ_t;


ਖਾਲੀ ngx_str_rbtree_ਪਾਓ_ਮੁੱਲ(ngx_rbtree_ਗੰਢ_t *ਆਰਜ਼ੀ,
    ngx_rbtree_ਗੰਢ_t *ਗੰਢ, ngx_rbtree_ਗੰਢ_t *sentinel);
ngx_str_ਗੰਢ_t *ngx_str_rbtree_lookup(ngx_rbtree_t *rbtree, ngx_str_t *name,
    uint32_t hash);


ਖਾਲੀ ngx_ਛਾਂਟੋ(ਖਾਲੀ *ਅਧਾਰ, ਮਾਪ_t n, ਮਾਪ_t ਮਾਪ,
    ngx_int_t (*cmp)(ਅਚਲ ਖਾਲੀ *, ਅਚਲ ਖਾਲੀ *));
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_qsort             qsort


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_ਮੁੱਲ_helper(n)   #n
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_ਮੁੱਲ(n)          ngx_ਮੁੱਲ_helper(n)


#ਜੇ_ਅੰਤ /* _NGX_STRING_H_INCLUDED_ */
