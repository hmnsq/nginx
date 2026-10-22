
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#यदि_अपरिभाषितम् _NGX_STRING_H_INCLUDED_
#परिभाषय _NGX_STRING_H_INCLUDED_


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


प्रकारनिर्देश संरचना {
    आकार_t      len;
    u_char     *दत्त;
} ngx_str_t;


प्रकारनिर्देश संरचना {
    ngx_str_t   कुञ्चिका;
    ngx_str_t   मानम्;
} ngx_keyval_t;


प्रकारनिर्देश संरचना {
    अचिह्नित    len:28;

    अचिह्नित    valid:1;
    अचिह्नित    no_cacheable:1;
    अचिह्नित    not_प्राप्तम्:1;
    अचिह्नित    escape:1;

    u_char     *दत्त;
} ngx_variable_मानम्_t;


#परिभाषय ngx_string(str)     { परिमाण(str) - 1, (u_char *) str }
#परिभाषय ngx_null_string     { 0, रिक्त }
#परिभाषय ngx_str_समुच्चयः(str, text)                                               \
    (str)->len = परिमाण(text) - 1; (str)->दत्त = (u_char *) text
#परिभाषय ngx_str_null(str)   (str)->len = 0; (str)->दत्त = रिक्त


#परिभाषय ngx_tolower(c)      (u_char) ((c >= 'A' && c <= 'Z') ? (c | 0x20) : c)
#परिभाषय ngx_toupper(c)      (u_char) ((c >= 'a' && c <= 'z') ? (c & ~0x20) : c)

शून्य ngx_strlow(u_char *dst, u_char *src, आकार_t n);


#परिभाषय ngx_strncmp(s1, s2, n)  वाक्ये_तुलय((ध्रुव वर्ण *) s1, (ध्रुव वर्ण *) s2, n)


/* msvc and icc7 compile strcmp() to inline loop */
#परिभाषय ngx_strcmp(s1, s2)  वाक्यं_तुलय((ध्रुव वर्ण *) s1, (ध्रुव वर्ण *) s2)


#परिभाषय ngx_strstr(s1, s2)  वाक्ये_अन्विष((ध्रुव वर्ण *) s1, (ध्रुव वर्ण *) s2)
#परिभाषय ngx_strlen(s)       वाक्यदैर्घ्यम्((ध्रुव वर्ण *) s)

आकार_t ngx_strnlen(u_char *p, आकार_t n);

#परिभाषय ngx_strchr(s1, c)   strchr((ध्रुव वर्ण *) s1, (पूर्णाङ्क) c)

स्थिर ngx_inline u_char *
ngx_strlchr(u_char *p, u_char *चरमः, u_char c)
{
    यावत् (p < चरमः) {

        यदि (*p == c) {
            प्रत्यागम p;
        }

        p++;
    }

    प्रत्यागम रिक्त;
}


/*
 * msvc and icc7 compile memset() to the inline "rep stos"
 * while ZeroMemory() and bzero() are the calls.
 * icc7 may also inline several mov's of a zeroed register for small blocks.
 */
#परिभाषय ngx_memzero(buf, n)       (शून्य) स्मृतिं_पूरय(buf, 0, n)
#परिभाषय ngx_memset(buf, c, n)     (शून्य) स्मृतिं_पूरय(buf, c, n)

शून्य ngx_explicit_memzero(शून्य *buf, आकार_t n);


#यदि (NGX_MEMCPY_सीमा)

शून्य *ngx_memcpy(शून्य *dst, ध्रुव शून्य *src, आकार_t n);
#परिभाषय ngx_cpymem(dst, src, n)   (((u_char *) ngx_memcpy(dst, src, n)) + (n))

#अन्यथा

/*
 * gcc3, msvc, and icc7 compile memcpy() to the inline "rep movs".
 * gcc3 compiles memcpy(d, s, 4) to the inline "mov"es.
 * icc8 compile memcpy(d, s, 4) to the inline "mov"es or XMM moves.
 */
#परिभाषय ngx_memcpy(dst, src, n)   (शून्य) स्मृतिं_प्रतिलिख(dst, src, n)
#परिभाषय ngx_cpymem(dst, src, n)   (((u_char *) स्मृतिं_प्रतिलिख(dst, src, n)) + (n))

#यदि_अन्तः


#यदि ( __INTEL_COMPILER >= 800 )

/*
 * the simple inline cycle copies the variable length strings up to 16
 * bytes faster than icc8 autodetecting _intel_fast_memcpy()
 */

स्थिर ngx_inline u_char *
ngx_copy(u_char *dst, u_char *src, आकार_t len)
{
    यदि (len < 17) {

        यावत् (len) {
            *dst++ = *src++;
            len--;
        }

        प्रत्यागम dst;

    } अन्यथा {
        प्रत्यागम ngx_cpymem(dst, src, len);
    }
}

#अन्यथा

#परिभाषय ngx_copy                  ngx_cpymem

#यदि_अन्तः


#परिभाषय ngx_memmove(dst, src, n)  (शून्य) memmove(dst, src, n)
#परिभाषय ngx_movemem(dst, src, n)  (((u_char *) memmove(dst, src, n)) + (n))


/* msvc and icc7 compile memcmp() to the inline loop */
#परिभाषय ngx_memcmp(s1, s2, n)     memcmp(s1, s2, n)


u_char *ngx_cpystrn(u_char *dst, u_char *src, आकार_t n);
u_char *ngx_pstrdup(ngx_pool_t *pool, ngx_str_t *src);
u_char * ngx_cdecl ngx_sprintf(u_char *buf, ध्रुव वर्ण *fmt, ...);
u_char * ngx_cdecl ngx_snprintf(u_char *buf, आकार_t max, ध्रुव वर्ण *fmt, ...);
u_char * ngx_cdecl ngx_slprintf(u_char *buf, u_char *चरमः, ध्रुव वर्ण *fmt,
    ...);
u_char *ngx_vslprintf(u_char *buf, u_char *चरमः, ध्रुव वर्ण *fmt, va_list तर्काः);
#परिभाषय ngx_vsnprintf(buf, max, fmt, तर्काः)                                   \
    ngx_vslprintf(buf, buf + (max), fmt, तर्काः)

ngx_int_t ngx_strcasecmp(u_char *s1, u_char *s2);
ngx_int_t ngx_strncasecmp(u_char *s1, u_char *s2, आकार_t n);

u_char *ngx_strnstr(u_char *s1, वर्ण *s2, आकार_t n);

u_char *ngx_strstrn(u_char *s1, वर्ण *s2, आकार_t n);
u_char *ngx_strcasestrn(u_char *s1, वर्ण *s2, आकार_t n);
u_char *ngx_strlcasestrn(u_char *s1, u_char *चरमः, u_char *s2, आकार_t n);

ngx_int_t ngx_rstrncmp(u_char *s1, u_char *s2, आकार_t n);
ngx_int_t ngx_rstrncasecmp(u_char *s1, u_char *s2, आकार_t n);
ngx_int_t ngx_memn2cmp(u_char *s1, u_char *s2, आकार_t n1, आकार_t n2);
ngx_int_t ngx_dns_strcmp(u_char *s1, u_char *s2);
ngx_int_t ngx_filename_cmp(u_char *s1, u_char *s2, आकार_t n);

ngx_int_t ngx_atoi(u_char *पङ्क्तिः, आकार_t n);
ngx_int_t ngx_atofp(u_char *पङ्क्तिः, आकार_t n, आकार_t point);
ssize_t ngx_atosz(u_char *पङ्क्तिः, आकार_t n);
off_t ngx_atoof(u_char *पङ्क्तिः, आकार_t n);
time_t ngx_atotm(u_char *पङ्क्तिः, आकार_t n);
ngx_int_t ngx_hextoi(u_char *पङ्क्तिः, आकार_t n);

u_char *ngx_hex_dump(u_char *dst, u_char *src, आकार_t len);


#परिभाषय ngx_base64_encoded_दीर्घता(len)  (((len + 2) / 3) * 4)
#परिभाषय ngx_base64_decoded_दीर्घता(len)  (((len + 3) / 4) * 3)

शून्य ngx_encode_base64(ngx_str_t *dst, ngx_str_t *src);
शून्य ngx_encode_base64url(ngx_str_t *dst, ngx_str_t *src);
ngx_int_t ngx_decode_base64(ngx_str_t *dst, ngx_str_t *src);
ngx_int_t ngx_decode_base64url(ngx_str_t *dst, ngx_str_t *src);

uint32_t ngx_utf8_decode(u_char **p, आकार_t n);
आकार_t ngx_utf8_दीर्घता(u_char *p, आकार_t n);
u_char *ngx_utf8_cpystrn(u_char *dst, u_char *src, आकार_t n, आकार_t len);


#परिभाषय NGX_ESCAPE_URI            0
#परिभाषय NGX_ESCAPE_तर्काः           1
#परिभाषय NGX_ESCAPE_URI_COMPONENT  2
#परिभाषय NGX_ESCAPE_HTML           3
#परिभाषय NGX_ESCAPE_REFRESH        4
#परिभाषय NGX_ESCAPE_MEMCACHED      5
#परिभाषय NGX_ESCAPE_MAIL_AUTH      6
#परिभाषय NGX_ESCAPE_MAIL_XTEXT     7

#परिभाषय NGX_UNESCAPE_URI       1
#परिभाषय NGX_UNESCAPE_REDIRECT  2

uintptr_t ngx_escape_uri(u_char *dst, u_char *src, आकार_t आकार,
    ngx_uint_t type);
शून्य ngx_unescape_uri(u_char **dst, u_char **src, आकार_t आकार, ngx_uint_t type);
uintptr_t ngx_escape_html(u_char *dst, u_char *src, आकार_t आकार);
uintptr_t ngx_escape_json(u_char *dst, u_char *src, आकार_t आकार);


प्रकारनिर्देश संरचना {
    ngx_rbtree_ग्रन्थिः_t         ग्रन्थिः;
    ngx_str_t                 str;
} ngx_str_ग्रन्थिः_t;


शून्य ngx_str_rbtree_निवेशय_मानम्(ngx_rbtree_ग्रन्थिः_t *अस्थायी,
    ngx_rbtree_ग्रन्थिः_t *ग्रन्थिः, ngx_rbtree_ग्रन्थिः_t *sentinel);
ngx_str_ग्रन्थिः_t *ngx_str_rbtree_lookup(ngx_rbtree_t *rbtree, ngx_str_t *name,
    uint32_t hash);


शून्य ngx_क्रमय(शून्य *आधारः, आकार_t n, आकार_t आकार,
    ngx_int_t (*cmp)(ध्रुव शून्य *, ध्रुव शून्य *));
#परिभाषय ngx_qsort             शीघ्रं_क्रमय


#परिभाषय ngx_मानम्_helper(n)   #n
#परिभाषय ngx_मानम्(n)          ngx_मानम्_helper(n)


#यदि_अन्तः /* _NGX_STRING_H_INCLUDED_ */
