
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef NGX_RADIX_বৃক্ষ_H_INCLUDED
#define NGX_RADIX_বৃক্ষ_H_INCLUDED


#অন্তর্ভাব <ngx_config.h>
#অন্তর্ভাব <ngx_core.h>


#define NGX_RADIX_NO_মান   (uintptr_t) -1

প্রকারলেখ কাঠামো ngx_radix_গ্রন্থি_s  ngx_radix_গ্রন্থি_t;

কাঠামো ngx_radix_গ্রন্থি_s {
    ngx_radix_গ্রন্থি_t  *ডান;
    ngx_radix_গ্রন্থি_t  *বাম;
    ngx_radix_গ্রন্থি_t  *parent;
    uintptr_t          মান;
};


প্রকারলেখ কাঠামো {
    ngx_radix_গ্রন্থি_t  *মূল;
    ngx_pool_t        *pool;
    ngx_radix_গ্রন্থি_t  *মুক্ত;
    অক্ষর              *start;
    আকার_t             আকার_2;
} ngx_radix_বৃক্ষ_t;


ngx_radix_বৃক্ষ_t *ngx_radix_বৃক্ষ_create(ngx_pool_t *pool,
    ngx_int_t preallocate);

ngx_int_t ngx_radix32tree_ঢোকাও(ngx_radix_বৃক্ষ_t *বৃক্ষ,
    uint32_t চাবি, uint32_t mask, uintptr_t মান);
ngx_int_t ngx_radix32tree_delete(ngx_radix_বৃক্ষ_t *বৃক্ষ,
    uint32_t চাবি, uint32_t mask);
uintptr_t ngx_radix32tree_find(ngx_radix_বৃক্ষ_t *বৃক্ষ, uint32_t চাবি);

#যদি (NGX_HAVE_INET6)
ngx_int_t ngx_radix128tree_ঢোকাও(ngx_radix_বৃক্ষ_t *বৃক্ষ,
    u_char *চাবি, u_char *mask, uintptr_t মান);
ngx_int_t ngx_radix128tree_delete(ngx_radix_বৃক্ষ_t *বৃক্ষ,
    u_char *চাবি, u_char *mask);
uintptr_t ngx_radix128tree_find(ngx_radix_বৃক্ষ_t *বৃক্ষ, u_char *চাবি);
#endif


#endif /* _NGX_RADIX_TREE_H_INCLUDED_ */
