
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#నిర్వచితం_కాకపోతే _NGX_STRING_H_INCLUDED_
#నిర్వచించు _NGX_STRING_H_INCLUDED_


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>


రకనిర్వచనం నిర్మాణం {
    కొలత_t      len;
    u_char     *దత్తాంశం;
} ngx_str_t;


రకనిర్వచనం నిర్మాణం {
    ngx_str_t   కీలకం;
    ngx_str_t   విలువ;
} ngx_keyval_t;


రకనిర్వచనం నిర్మాణం {
    గుర్తులేని    len:28;

    గుర్తులేని    valid:1;
    గుర్తులేని    no_cacheable:1;
    గుర్తులేని    not_దొరికింది:1;
    గుర్తులేని    escape:1;

    u_char     *దత్తాంశం;
} ngx_variable_విలువ_t;


#నిర్వచించు ngx_string(str)     { పరిమాణం(str) - 1, (u_char *) str }
#నిర్వచించు ngx_null_string     { 0, ఖాళీ }
#నిర్వచించు ngx_str_సమితి(str, పాఠ్యం)                                               \
    (str)->len = పరిమాణం(పాఠ్యం) - 1; (str)->దత్తాంశం = (u_char *) పాఠ్యం
#నిర్వచించు ngx_str_null(str)   (str)->len = 0; (str)->దత్తాంశం = ఖాళీ


#నిర్వచించు ngx_tolower(c)      (u_char) ((c >= 'A' && c <= 'Z') ? (c | 0x20) : c)
#నిర్వచించు ngx_toupper(c)      (u_char) ((c >= 'a' && c <= 'z') ? (c & ~0x20) : c)

శూన్య ngx_strlow(u_char *dst, u_char *src, కొలత_t n);


#నిర్వచించు ngx_strncmp(s1, s2, n)  అక్షరశ్రేణి_పోలిక((స్థిరరాశి అక్షరం *) s1, (స్థిరరాశి అక్షరం *) s2, n)


/* msvc and icc7 compile strcmp() to inline loop */
#నిర్వచించు ngx_strcmp(s1, s2)  strcmp((స్థిరరాశి అక్షరం *) s1, (స్థిరరాశి అక్షరం *) s2)


#నిర్వచించు ngx_strstr(s1, s2)  అక్షరశ్రేణి_శోధన((స్థిరరాశి అక్షరం *) s1, (స్థిరరాశి అక్షరం *) s2)
#నిర్వచించు ngx_strlen(s)       అక్షరశ్రేణి_పొడవు((స్థిరరాశి అక్షరం *) s)

కొలత_t ngx_strnlen(u_char *p, కొలత_t n);

#నిర్వచించు ngx_strchr(s1, c)   strchr((స్థిరరాశి అక్షరం *) s1, (పూర్ణాంకం) c)

స్థిర ngx_inline u_char *
ngx_strlchr(u_char *p, u_char *చివరి, u_char c)
{
    అయితే (p < చివరి) {

        ఒకవేళ (*p == c) {
            తిరిగి p;
        }

        p++;
    }

    తిరిగి ఖాళీ;
}


/*
 * msvc and icc7 compile memset() to the inline "rep stos"
 * while ZeroMemory() and bzero() are the calls.
 * icc7 may also inline several mov's of a zeroed register for small blocks.
 */
#నిర్వచించు ngx_memzero(buf, n)       (శూన్య) memset(buf, 0, n)
#నిర్వచించు ngx_memset(buf, c, n)     (శూన్య) memset(buf, c, n)

శూన్య ngx_explicit_memzero(శూన్య *buf, కొలత_t n);


#ఒకవేళ (NGX_MEMCPY_హద్దు)

శూన్య *ngx_memcpy(శూన్య *dst, స్థిరరాశి శూన్య *src, కొలత_t n);
#నిర్వచించు ngx_cpymem(dst, src, n)   (((u_char *) ngx_memcpy(dst, src, n)) + (n))

#లేకపోతే

/*
 * gcc3, msvc, and icc7 compile memcpy() to the inline "rep movs".
 * gcc3 compiles memcpy(d, s, 4) to the inline "mov"es.
 * icc8 compile memcpy(d, s, 4) to the inline "mov"es or XMM moves.
 */
#నిర్వచించు ngx_memcpy(dst, src, n)   (శూన్య) జ్ఞాపకం_నకలు(dst, src, n)
#నిర్వచించు ngx_cpymem(dst, src, n)   (((u_char *) జ్ఞాపకం_నకలు(dst, src, n)) + (n))

#అయితే_ముగింపు


#ఒకవేళ ( __INTEL_COMPILER >= 800 )

/*
 * the simple inline cycle copies the variable length strings up to 16
 * bytes faster than icc8 autodetecting _intel_fast_memcpy()
 */

స్థిర ngx_inline u_char *
ngx_copy(u_char *dst, u_char *src, కొలత_t len)
{
    ఒకవేళ (len < 17) {

        అయితే (len) {
            *dst++ = *src++;
            len--;
        }

        తిరిగి dst;

    } లేకపోతే {
        తిరిగి ngx_cpymem(dst, src, len);
    }
}

#లేకపోతే

#నిర్వచించు ngx_copy                  ngx_cpymem

#అయితే_ముగింపు


#నిర్వచించు ngx_memmove(dst, src, n)  (శూన్య) memmove(dst, src, n)
#నిర్వచించు ngx_movemem(dst, src, n)  (((u_char *) memmove(dst, src, n)) + (n))


/* msvc and icc7 compile memcmp() to the inline loop */
#నిర్వచించు ngx_memcmp(s1, s2, n)     memcmp(s1, s2, n)


u_char *ngx_cpystrn(u_char *dst, u_char *src, కొలత_t n);
u_char *ngx_pstrdup(ngx_pool_t *pool, ngx_str_t *src);
u_char * ngx_cdecl ngx_sprintf(u_char *buf, స్థిరరాశి అక్షరం *fmt, ...);
u_char * ngx_cdecl ngx_snprintf(u_char *buf, కొలత_t max, స్థిరరాశి అక్షరం *fmt, ...);
u_char * ngx_cdecl ngx_slprintf(u_char *buf, u_char *చివరి, స్థిరరాశి అక్షరం *fmt,
    ...);
u_char *ngx_vslprintf(u_char *buf, u_char *చివరి, స్థిరరాశి అక్షరం *fmt, va_list వాదనలు);
#నిర్వచించు ngx_vsnprintf(buf, max, fmt, వాదనలు)                                   \
    ngx_vslprintf(buf, buf + (max), fmt, వాదనలు)

ngx_int_t ngx_strcasecmp(u_char *s1, u_char *s2);
ngx_int_t ngx_strncasecmp(u_char *s1, u_char *s2, కొలత_t n);

u_char *ngx_strnstr(u_char *s1, అక్షరం *s2, కొలత_t n);

u_char *ngx_strstrn(u_char *s1, అక్షరం *s2, కొలత_t n);
u_char *ngx_strcasestrn(u_char *s1, అక్షరం *s2, కొలత_t n);
u_char *ngx_strlcasestrn(u_char *s1, u_char *చివరి, u_char *s2, కొలత_t n);

ngx_int_t ngx_rstrncmp(u_char *s1, u_char *s2, కొలత_t n);
ngx_int_t ngx_rstrncasecmp(u_char *s1, u_char *s2, కొలత_t n);
ngx_int_t ngx_memn2cmp(u_char *s1, u_char *s2, కొలత_t n1, కొలత_t n2);
ngx_int_t ngx_dns_strcmp(u_char *s1, u_char *s2);
ngx_int_t ngx_filename_cmp(u_char *s1, u_char *s2, కొలత_t n);

ngx_int_t ngx_atoi(u_char *పంక్తి, కొలత_t n);
ngx_int_t ngx_atofp(u_char *పంక్తి, కొలత_t n, కొలత_t బిందువు);
ssize_t ngx_atosz(u_char *పంక్తి, కొలత_t n);
off_t ngx_atoof(u_char *పంక్తి, కొలత_t n);
సమయం_t ngx_atotm(u_char *పంక్తి, కొలత_t n);
ngx_int_t ngx_hextoi(u_char *పంక్తి, కొలత_t n);

u_char *ngx_hex_dump(u_char *dst, u_char *src, కొలత_t len);


#నిర్వచించు ngx_base64_encoded_నిడివి(len)  (((len + 2) / 3) * 4)
#నిర్వచించు ngx_base64_decoded_నిడివి(len)  (((len + 3) / 4) * 3)

శూన్య ngx_encode_base64(ngx_str_t *dst, ngx_str_t *src);
శూన్య ngx_encode_base64url(ngx_str_t *dst, ngx_str_t *src);
ngx_int_t ngx_decode_base64(ngx_str_t *dst, ngx_str_t *src);
ngx_int_t ngx_decode_base64url(ngx_str_t *dst, ngx_str_t *src);

uint32_t ngx_utf8_decode(u_char **p, కొలత_t n);
కొలత_t ngx_utf8_నిడివి(u_char *p, కొలత_t n);
u_char *ngx_utf8_cpystrn(u_char *dst, u_char *src, కొలత_t n, కొలత_t len);


#నిర్వచించు NGX_ESCAPE_URI            0
#నిర్వచించు NGX_ESCAPE_వాదనలు           1
#నిర్వచించు NGX_ESCAPE_URI_COMPONENT  2
#నిర్వచించు NGX_ESCAPE_HTML           3
#నిర్వచించు NGX_ESCAPE_REFRESH        4
#నిర్వచించు NGX_ESCAPE_MEMCACHED      5
#నిర్వచించు NGX_ESCAPE_MAIL_AUTH      6
#నిర్వచించు NGX_ESCAPE_MAIL_XTEXT     7

#నిర్వచించు NGX_UNESCAPE_URI       1
#నిర్వచించు NGX_UNESCAPE_REDIRECT  2

uintptr_t ngx_escape_uri(u_char *dst, u_char *src, కొలత_t కొలత,
    ngx_uint_t type);
శూన్య ngx_unescape_uri(u_char **dst, u_char **src, కొలత_t కొలత, ngx_uint_t type);
uintptr_t ngx_escape_html(u_char *dst, u_char *src, కొలత_t కొలత);
uintptr_t ngx_escape_json(u_char *dst, u_char *src, కొలత_t కొలత);


రకనిర్వచనం నిర్మాణం {
    ngx_rbtree_కణుపు_t         కణుపు;
    ngx_str_t                 str;
} ngx_str_కణుపు_t;


శూన్య ngx_str_rbtree_చొప్పించు_విలువ(ngx_rbtree_కణుపు_t *తాత్కాలిక,
    ngx_rbtree_కణుపు_t *కణుపు, ngx_rbtree_కణుపు_t *sentinel);
ngx_str_కణుపు_t *ngx_str_rbtree_lookup(ngx_rbtree_t *rbtree, ngx_str_t *name,
    uint32_t hash);


శూన్య ngx_క్రమీకరించు(శూన్య *పునాది, కొలత_t n, కొలత_t కొలత,
    ngx_int_t (*cmp)(స్థిరరాశి శూన్య *, స్థిరరాశి శూన్య *));
#నిర్వచించు ngx_qsort             qsort


#నిర్వచించు ngx_విలువ_helper(n)   #n
#నిర్వచించు ngx_విలువ(n)          ngx_విలువ_helper(n)


#అయితే_ముగింపు /* _NGX_STRING_H_INCLUDED_ */
