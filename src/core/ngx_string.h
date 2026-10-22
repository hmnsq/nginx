
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अगर_अघोषित _NGX_STRING_H_INCLUDED_
#घोषणा _NGX_STRING_H_INCLUDED_


#समावेश <ngx_config.h>
#समावेश <ngx_core.h>


प्रकार काष्ठा {
    माप_प्रकार      len;
    u_char     *तथ्य;
} ngx_str_t;


प्रकार काष्ठा {
    ngx_str_t   कुंजी;
    ngx_str_t   मूल्य;
} ngx_keyval_t;


प्रकार काष्ठा {
    अचिन्हित    len:28;

    अचिन्हित    valid:1;
    अचिन्हित    no_cacheable:1;
    अचिन्हित    not_found:1;
    अचिन्हित    escape:1;

    u_char     *तथ्य;
} ngx_variable_मूल्य_t;


#घोषणा ngx_string(str)     { माप(str) - 1, (u_char *) str }
#घोषणा ngx_null_string     { 0, शून्य }
#घोषणा ngx_str_set(str, text)                                               \
    (str)->len = माप(text) - 1; (str)->तथ्य = (u_char *) text
#घोषणा ngx_str_null(str)   (str)->len = 0; (str)->तथ्य = शून्य


#घोषणा ngx_tolower(c)      (u_char) ((c >= 'A' && c <= 'Z') ? (c | 0x20) : c)
#घोषणा ngx_toupper(c)      (u_char) ((c >= 'a' && c <= 'z') ? (c & ~0x20) : c)

व्योम ngx_strlow(u_char *dst, u_char *src, माप_प्रकार n);


#घोषणा ngx_strncmp(s1, s2, n)  म_भेदन((स्थिर अक्षर *) s1, (स्थिर अक्षर *) s2, n)


/* msvc and icc7 compile strcmp() to inline loop */
#घोषणा ngx_strcmp(s1, s2)  म_भेद((स्थिर अक्षर *) s1, (स्थिर अक्षर *) s2)


#घोषणा ngx_strstr(s1, s2)  म_माला((स्थिर अक्षर *) s1, (स्थिर अक्षर *) s2)
#घोषणा ngx_strlen(s)       म_माप((स्थिर अक्षर *) s)

माप_प्रकार ngx_strnlen(u_char *p, माप_प्रकार n);

#घोषणा ngx_strchr(s1, c)   म_अक्षर((स्थिर अक्षर *) s1, (पूर्णांक) c)

अटल ngx_inline u_char *
ngx_strlchr(u_char *p, u_char *last, u_char c)
{
    जबतक (p < last) {

        अगर (*p == c) {
            वापस p;
        }

        p++;
    }

    वापस शून्य;
}


/*
 * msvc and icc7 compile memset() to the inline "rep stos"
 * while ZeroMemory() and bzero() are the calls.
 * icc7 may also inline several mov's of a zeroed register for small blocks.
 */
#घोषणा ngx_memzero(buf, n)       (व्योम) स_रखो(buf, 0, n)
#घोषणा ngx_memset(buf, c, n)     (व्योम) स_रखो(buf, c, n)

व्योम ngx_explicit_memzero(व्योम *buf, माप_प्रकार n);


#अगर (NGX_MEMCPY_LIMIT)

व्योम *ngx_memcpy(व्योम *dst, स्थिर व्योम *src, माप_प्रकार n);
#घोषणा ngx_cpymem(dst, src, n)   (((u_char *) ngx_memcpy(dst, src, n)) + (n))

#अन्यथा

/*
 * gcc3, msvc, and icc7 compile memcpy() to the inline "rep movs".
 * gcc3 compiles memcpy(d, s, 4) to the inline "mov"es.
 * icc8 compile memcpy(d, s, 4) to the inline "mov"es or XMM moves.
 */
#घोषणा ngx_memcpy(dst, src, n)   (व्योम) स_नकल(dst, src, n)
#घोषणा ngx_cpymem(dst, src, n)   (((u_char *) स_नकल(dst, src, n)) + (n))

#पूर्ण_अगर


#अगर ( __INTEL_COMPILER >= 800 )

/*
 * the simple inline cycle copies the variable length strings up to 16
 * bytes faster than icc8 autodetecting _intel_fast_memcpy()
 */

अटल ngx_inline u_char *
ngx_copy(u_char *dst, u_char *src, माप_प्रकार len)
{
    अगर (len < 17) {

        जबतक (len) {
            *dst++ = *src++;
            len--;
        }

        वापस dst;

    } अन्यथा {
        वापस ngx_cpymem(dst, src, len);
    }
}

#अन्यथा

#घोषणा ngx_copy                  ngx_cpymem

#पूर्ण_अगर


#घोषणा ngx_memmove(dst, src, n)  (व्योम) स_हटाओ(dst, src, n)
#घोषणा ngx_movemem(dst, src, n)  (((u_char *) स_हटाओ(dst, src, n)) + (n))


/* msvc and icc7 compile memcmp() to the inline loop */
#घोषणा ngx_memcmp(s1, s2, n)     स_भेद(s1, s2, n)


u_char *ngx_cpystrn(u_char *dst, u_char *src, माप_प्रकार n);
u_char *ngx_pstrdup(ngx_pool_t *pool, ngx_str_t *src);
u_char * ngx_cdecl ngx_sprintf(u_char *buf, स्थिर अक्षर *fmt, ...);
u_char * ngx_cdecl ngx_snprintf(u_char *buf, माप_प्रकार max, स्थिर अक्षर *fmt, ...);
u_char * ngx_cdecl ngx_slprintf(u_char *buf, u_char *last, स्थिर अक्षर *fmt,
    ...);
u_char *ngx_vslprintf(u_char *buf, u_char *last, स्थिर अक्षर *fmt, बहु_सूची args);
#घोषणा ngx_vsnprintf(buf, max, fmt, args)                                   \
    ngx_vslprintf(buf, buf + (max), fmt, args)

ngx_int_t ngx_strcasecmp(u_char *s1, u_char *s2);
ngx_int_t ngx_strncasecmp(u_char *s1, u_char *s2, माप_प्रकार n);

u_char *ngx_strnstr(u_char *s1, अक्षर *s2, माप_प्रकार n);

u_char *ngx_strstrn(u_char *s1, अक्षर *s2, माप_प्रकार n);
u_char *ngx_strcasestrn(u_char *s1, अक्षर *s2, माप_प्रकार n);
u_char *ngx_strlcasestrn(u_char *s1, u_char *last, u_char *s2, माप_प्रकार n);

ngx_int_t ngx_rstrncmp(u_char *s1, u_char *s2, माप_प्रकार n);
ngx_int_t ngx_rstrncasecmp(u_char *s1, u_char *s2, माप_प्रकार n);
ngx_int_t ngx_memn2cmp(u_char *s1, u_char *s2, माप_प्रकार n1, माप_प्रकार n2);
ngx_int_t ngx_dns_strcmp(u_char *s1, u_char *s2);
ngx_int_t ngx_filename_cmp(u_char *s1, u_char *s2, माप_प्रकार n);

ngx_int_t ngx_atoi(u_char *पंक्ति, माप_प्रकार n);
ngx_int_t ngx_atofp(u_char *पंक्ति, माप_प्रकार n, माप_प्रकार point);
ssize_t ngx_atosz(u_char *पंक्ति, माप_प्रकार n);
off_t ngx_atoof(u_char *पंक्ति, माप_प्रकार n);
समय_प्रकार ngx_atotm(u_char *पंक्ति, माप_प्रकार n);
ngx_int_t ngx_hextoi(u_char *पंक्ति, माप_प्रकार n);

u_char *ngx_hex_dump(u_char *dst, u_char *src, माप_प्रकार len);


#घोषणा ngx_base64_encoded_लंबाई(len)  (((len + 2) / 3) * 4)
#घोषणा ngx_base64_decoded_लंबाई(len)  (((len + 3) / 4) * 3)

व्योम ngx_encode_base64(ngx_str_t *dst, ngx_str_t *src);
व्योम ngx_encode_base64url(ngx_str_t *dst, ngx_str_t *src);
ngx_int_t ngx_decode_base64(ngx_str_t *dst, ngx_str_t *src);
ngx_int_t ngx_decode_base64url(ngx_str_t *dst, ngx_str_t *src);

uint32_t ngx_utf8_decode(u_char **p, माप_प्रकार n);
माप_प्रकार ngx_utf8_लंबाई(u_char *p, माप_प्रकार n);
u_char *ngx_utf8_cpystrn(u_char *dst, u_char *src, माप_प्रकार n, माप_प्रकार len);


#घोषणा NGX_ESCAPE_URI            0
#घोषणा NGX_ESCAPE_ARGS           1
#घोषणा NGX_ESCAPE_URI_COMPONENT  2
#घोषणा NGX_ESCAPE_HTML           3
#घोषणा NGX_ESCAPE_REFRESH        4
#घोषणा NGX_ESCAPE_MEMCACHED      5
#घोषणा NGX_ESCAPE_MAIL_AUTH      6
#घोषणा NGX_ESCAPE_MAIL_XTEXT     7

#घोषणा NGX_UNESCAPE_URI       1
#घोषणा NGX_UNESCAPE_REDIRECT  2

uintptr_t ngx_escape_uri(u_char *dst, u_char *src, माप_प्रकार आकार,
    ngx_uint_t type);
व्योम ngx_unescape_uri(u_char **dst, u_char **src, माप_प्रकार आकार, ngx_uint_t type);
uintptr_t ngx_escape_html(u_char *dst, u_char *src, माप_प्रकार आकार);
uintptr_t ngx_escape_json(u_char *dst, u_char *src, माप_प्रकार आकार);


प्रकार काष्ठा {
    ngx_rbtree_गाँठ_t         गाँठ;
    ngx_str_t                 str;
} ngx_str_गाँठ_t;


व्योम ngx_str_rbtree_जोड़ो_मूल्य(ngx_rbtree_गाँठ_t *अस्थायी,
    ngx_rbtree_गाँठ_t *गाँठ, ngx_rbtree_गाँठ_t *sentinel);
ngx_str_गाँठ_t *ngx_str_rbtree_lookup(ngx_rbtree_t *rbtree, ngx_str_t *name,
    uint32_t hash);


व्योम ngx_sort(व्योम *base, माप_प्रकार n, माप_प्रकार आकार,
    ngx_int_t (*cmp)(स्थिर व्योम *, स्थिर व्योम *));
#घोषणा ngx_qsort             क्विक


#घोषणा ngx_मूल्य_helper(n)   #n
#घोषणा ngx_मूल्य(n)          ngx_मूल्य_helper(n)


#पूर्ण_अगर /* _NGX_STRING_H_INCLUDED_ */
