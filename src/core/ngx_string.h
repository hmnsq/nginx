
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ _NGX_STRING_H_INCLUDED_
#ವ್ಯಾಖ್ಯಾನಿಸು _NGX_STRING_H_INCLUDED_


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ಗಾತ್ರ_t      len;
    u_char     *ದತ್ತಾಂಶ;
} ngx_str_t;


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ngx_str_t   ಕೀಲಿ;
    ngx_str_t   ಮೌಲ್ಯ;
} ngx_keyval_t;


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ಚಿಹ್ನೆರಹಿತ    len:28;

    ಚಿಹ್ನೆರಹಿತ    valid:1;
    ಚಿಹ್ನೆರಹಿತ    no_cacheable:1;
    ಚಿಹ್ನೆರಹಿತ    not_ಸಿಕ್ಕಿತು:1;
    ಚಿಹ್ನೆರಹಿತ    escape:1;

    u_char     *ದತ್ತಾಂಶ;
} ngx_variable_ಮೌಲ್ಯ_t;


#ವ್ಯಾಖ್ಯಾನಿಸು ngx_string(str)     { ಗಾತ್ರ(str) - 1, (u_char *) str }
#ವ್ಯಾಖ್ಯಾನಿಸು ngx_null_string     { 0, ಖಾಲಿ }
#ವ್ಯಾಖ್ಯಾನಿಸು ngx_str_ಗಣ(str, ಪಠ್ಯ)                                               \
    (str)->len = ಗಾತ್ರ(ಪಠ್ಯ) - 1; (str)->ದತ್ತಾಂಶ = (u_char *) ಪಠ್ಯ
#ವ್ಯಾಖ್ಯಾನಿಸು ngx_str_null(str)   (str)->len = 0; (str)->ದತ್ತಾಂಶ = ಖಾಲಿ


#ವ್ಯಾಖ್ಯಾನಿಸು ngx_tolower(c)      (u_char) ((c >= 'A' && c <= 'Z') ? (c | 0x20) : c)
#ವ್ಯಾಖ್ಯಾನಿಸು ngx_toupper(c)      (u_char) ((c >= 'a' && c <= 'z') ? (c & ~0x20) : c)

ಶೂನ್ಯ ngx_strlow(u_char *dst, u_char *src, ಗಾತ್ರ_t n);


#ವ್ಯಾಖ್ಯಾನಿಸು ngx_strncmp(s1, s2, n)  ಅಕ್ಷರಸರಣಿ_ಹೋಲಿಕೆ((ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *) s1, (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *) s2, n)


/* msvc and icc7 compile strcmp() to inline loop */
#ವ್ಯಾಖ್ಯಾನಿಸು ngx_strcmp(s1, s2)  strcmp((ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *) s1, (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *) s2)


#ವ್ಯಾಖ್ಯಾನಿಸು ngx_strstr(s1, s2)  ಅಕ್ಷರಸರಣಿ_ಹುಡುಕು((ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *) s1, (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *) s2)
#ವ್ಯಾಖ್ಯಾನಿಸು ngx_strlen(s)       ಅಕ್ಷರಸರಣಿ_ಉದ್ದ((ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *) s)

ಗಾತ್ರ_t ngx_strnlen(u_char *p, ಗಾತ್ರ_t n);

#ವ್ಯಾಖ್ಯಾನಿಸು ngx_strchr(s1, c)   strchr((ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *) s1, (ಪೂರ್ಣಾಂಕ) c)

ಸ್ಥಿರ ngx_inline u_char *
ngx_strlchr(u_char *p, u_char *ಕೊನೆಯ, u_char c)
{
    ಆಗ (p < ಕೊನೆಯ) {

        ಆದರೆ (*p == c) {
            ಹಿಂತಿರುಗಿ p;
        }

        p++;
    }

    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}


/*
 * msvc and icc7 compile memset() to the inline "rep stos"
 * while ZeroMemory() and bzero() are the calls.
 * icc7 may also inline several mov's of a zeroed register for small blocks.
 */
#ವ್ಯಾಖ್ಯಾನಿಸು ngx_memzero(buf, n)       (ಶೂನ್ಯ) memset(buf, 0, n)
#ವ್ಯಾಖ್ಯಾನಿಸು ngx_memset(buf, c, n)     (ಶೂನ್ಯ) memset(buf, c, n)

ಶೂನ್ಯ ngx_explicit_memzero(ಶೂನ್ಯ *buf, ಗಾತ್ರ_t n);


#ಆದರೆ (NGX_MEMCPY_ಎಲ್ಲೆ)

ಶೂನ್ಯ *ngx_memcpy(ಶೂನ್ಯ *dst, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *src, ಗಾತ್ರ_t n);
#ವ್ಯಾಖ್ಯಾನಿಸು ngx_cpymem(dst, src, n)   (((u_char *) ngx_memcpy(dst, src, n)) + (n))

#ಇಲ್ಲದಿದ್ದರೆ

/*
 * gcc3, msvc, and icc7 compile memcpy() to the inline "rep movs".
 * gcc3 compiles memcpy(d, s, 4) to the inline "mov"es.
 * icc8 compile memcpy(d, s, 4) to the inline "mov"es or XMM moves.
 */
#ವ್ಯಾಖ್ಯಾನಿಸು ngx_memcpy(dst, src, n)   (ಶೂನ್ಯ) ಸ್ಮರಣೆ_ನಕಲು(dst, src, n)
#ವ್ಯಾಖ್ಯಾನಿಸು ngx_cpymem(dst, src, n)   (((u_char *) ಸ್ಮರಣೆ_ನಕಲು(dst, src, n)) + (n))

#ಆದರೆ_ಅಂತ್ಯ


#ಆದರೆ ( __INTEL_COMPILER >= 800 )

/*
 * the simple inline cycle copies the variable length strings up to 16
 * bytes faster than icc8 autodetecting _intel_fast_memcpy()
 */

ಸ್ಥಿರ ngx_inline u_char *
ngx_copy(u_char *dst, u_char *src, ಗಾತ್ರ_t len)
{
    ಆದರೆ (len < 17) {

        ಆಗ (len) {
            *dst++ = *src++;
            len--;
        }

        ಹಿಂತಿರುಗಿ dst;

    } ಇಲ್ಲದಿದ್ದರೆ {
        ಹಿಂತಿರುಗಿ ngx_cpymem(dst, src, len);
    }
}

#ಇಲ್ಲದಿದ್ದರೆ

#ವ್ಯಾಖ್ಯಾನಿಸು ngx_copy                  ngx_cpymem

#ಆದರೆ_ಅಂತ್ಯ


#ವ್ಯಾಖ್ಯಾನಿಸು ngx_memmove(dst, src, n)  (ಶೂನ್ಯ) memmove(dst, src, n)
#ವ್ಯಾಖ್ಯಾನಿಸು ngx_movemem(dst, src, n)  (((u_char *) memmove(dst, src, n)) + (n))


/* msvc and icc7 compile memcmp() to the inline loop */
#ವ್ಯಾಖ್ಯಾನಿಸು ngx_memcmp(s1, s2, n)     memcmp(s1, s2, n)


u_char *ngx_cpystrn(u_char *dst, u_char *src, ಗಾತ್ರ_t n);
u_char *ngx_pstrdup(ngx_pool_t *pool, ngx_str_t *src);
u_char * ngx_cdecl ngx_sprintf(u_char *buf, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fmt, ...);
u_char * ngx_cdecl ngx_snprintf(u_char *buf, ಗಾತ್ರ_t max, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fmt, ...);
u_char * ngx_cdecl ngx_slprintf(u_char *buf, u_char *ಕೊನೆಯ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fmt,
    ...);
u_char *ngx_vslprintf(u_char *buf, u_char *ಕೊನೆಯ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fmt, va_list ವಾದಗಳು);
#ವ್ಯಾಖ್ಯಾನಿಸು ngx_vsnprintf(buf, max, fmt, ವಾದಗಳು)                                   \
    ngx_vslprintf(buf, buf + (max), fmt, ವಾದಗಳು)

ngx_int_t ngx_strcasecmp(u_char *s1, u_char *s2);
ngx_int_t ngx_strncasecmp(u_char *s1, u_char *s2, ಗಾತ್ರ_t n);

u_char *ngx_strnstr(u_char *s1, ಅಕ್ಷರ *s2, ಗಾತ್ರ_t n);

u_char *ngx_strstrn(u_char *s1, ಅಕ್ಷರ *s2, ಗಾತ್ರ_t n);
u_char *ngx_strcasestrn(u_char *s1, ಅಕ್ಷರ *s2, ಗಾತ್ರ_t n);
u_char *ngx_strlcasestrn(u_char *s1, u_char *ಕೊನೆಯ, u_char *s2, ಗಾತ್ರ_t n);

ngx_int_t ngx_rstrncmp(u_char *s1, u_char *s2, ಗಾತ್ರ_t n);
ngx_int_t ngx_rstrncasecmp(u_char *s1, u_char *s2, ಗಾತ್ರ_t n);
ngx_int_t ngx_memn2cmp(u_char *s1, u_char *s2, ಗಾತ್ರ_t n1, ಗಾತ್ರ_t n2);
ngx_int_t ngx_dns_strcmp(u_char *s1, u_char *s2);
ngx_int_t ngx_filename_cmp(u_char *s1, u_char *s2, ಗಾತ್ರ_t n);

ngx_int_t ngx_atoi(u_char *ಸಾಲು, ಗಾತ್ರ_t n);
ngx_int_t ngx_atofp(u_char *ಸಾಲು, ಗಾತ್ರ_t n, ಗಾತ್ರ_t ಬಿಂದು);
ssize_t ngx_atosz(u_char *ಸಾಲು, ಗಾತ್ರ_t n);
off_t ngx_atoof(u_char *ಸಾಲು, ಗಾತ್ರ_t n);
ಸಮಯ_t ngx_atotm(u_char *ಸಾಲು, ಗಾತ್ರ_t n);
ngx_int_t ngx_hextoi(u_char *ಸಾಲು, ಗಾತ್ರ_t n);

u_char *ngx_hex_dump(u_char *dst, u_char *src, ಗಾತ್ರ_t len);


#ವ್ಯಾಖ್ಯಾನಿಸು ngx_base64_encoded_ಉದ್ದಳತೆ(len)  (((len + 2) / 3) * 4)
#ವ್ಯಾಖ್ಯಾನಿಸು ngx_base64_decoded_ಉದ್ದಳತೆ(len)  (((len + 3) / 4) * 3)

ಶೂನ್ಯ ngx_encode_base64(ngx_str_t *dst, ngx_str_t *src);
ಶೂನ್ಯ ngx_encode_base64url(ngx_str_t *dst, ngx_str_t *src);
ngx_int_t ngx_decode_base64(ngx_str_t *dst, ngx_str_t *src);
ngx_int_t ngx_decode_base64url(ngx_str_t *dst, ngx_str_t *src);

uint32_t ngx_utf8_decode(u_char **p, ಗಾತ್ರ_t n);
ಗಾತ್ರ_t ngx_utf8_ಉದ್ದಳತೆ(u_char *p, ಗಾತ್ರ_t n);
u_char *ngx_utf8_cpystrn(u_char *dst, u_char *src, ಗಾತ್ರ_t n, ಗಾತ್ರ_t len);


#ವ್ಯಾಖ್ಯಾನಿಸು NGX_ESCAPE_URI            0
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_ESCAPE_ವಾದಗಳು           1
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_ESCAPE_URI_COMPONENT  2
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_ESCAPE_HTML           3
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_ESCAPE_REFRESH        4
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_ESCAPE_MEMCACHED      5
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_ESCAPE_MAIL_AUTH      6
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_ESCAPE_MAIL_XTEXT     7

#ವ್ಯಾಖ್ಯಾನಿಸು NGX_UNESCAPE_URI       1
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_UNESCAPE_REDIRECT  2

uintptr_t ngx_escape_uri(u_char *dst, u_char *src, ಗಾತ್ರ_t ಗಾತ್ರ_2,
    ngx_uint_t type);
ಶೂನ್ಯ ngx_unescape_uri(u_char **dst, u_char **src, ಗಾತ್ರ_t ಗಾತ್ರ_2, ngx_uint_t type);
uintptr_t ngx_escape_html(u_char *dst, u_char *src, ಗಾತ್ರ_t ಗಾತ್ರ_2);
uintptr_t ngx_escape_json(u_char *dst, u_char *src, ಗಾತ್ರ_t ಗಾತ್ರ_2);


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ngx_rbtree_ಗಂಟು_t         ಗಂಟು;
    ngx_str_t                 str;
} ngx_str_ಗಂಟು_t;


ಶೂನ್ಯ ngx_str_rbtree_ಸೇರಿಸು_ಮೌಲ್ಯ(ngx_rbtree_ಗಂಟು_t *ತಾತ್ಕಾಲಿಕ,
    ngx_rbtree_ಗಂಟು_t *ಗಂಟು, ngx_rbtree_ಗಂಟು_t *sentinel);
ngx_str_ಗಂಟು_t *ngx_str_rbtree_lookup(ngx_rbtree_t *rbtree, ngx_str_t *name,
    uint32_t hash);


ಶೂನ್ಯ ngx_ವಿಂಗಡಿಸು(ಶೂನ್ಯ *ತಳ, ಗಾತ್ರ_t n, ಗಾತ್ರ_t ಗಾತ್ರ_2,
    ngx_int_t (*cmp)(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *));
#ವ್ಯಾಖ್ಯಾನಿಸು ngx_qsort             qsort


#ವ್ಯಾಖ್ಯಾನಿಸು ngx_ಮೌಲ್ಯ_helper(n)   #n
#ವ್ಯಾಖ್ಯಾನಿಸು ngx_ಮೌಲ್ಯ(n)          ngx_ಮೌಲ್ಯ_helper(n)


#ಆದರೆ_ಅಂತ್ಯ /* _NGX_STRING_H_INCLUDED_ */
