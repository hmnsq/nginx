
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef NGX_RADIX_ڤوکوق_H_INCLUDED
#define NGX_RADIX_ڤوکوق_H_INCLUDED


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


#define NGX_RADIX_NO_نيلاي   (uintptr_t) -1

تعريفنوع بنية ngx_radix_نود_s  ngx_radix_نود_t;

بنية ngx_radix_نود_s {
    ngx_radix_نود_t  *کانن;
    ngx_radix_نود_t  *کيري;
    ngx_radix_نود_t  *parent;
    uintptr_t          نيلاي;
};


تعريفنوع بنية {
    ngx_radix_نود_t  *اکر;
    ngx_pool_t        *pool;
    ngx_radix_نود_t  *حرر;
    حرف              *start;
    size_t             size;
} ngx_radix_ڤوکوق_t;


ngx_radix_ڤوکوق_t *ngx_radix_ڤوکوق_create(ngx_pool_t *pool,
    ngx_int_t preallocate);

ngx_int_t ngx_radix32tree_سيسيڤ(ngx_radix_ڤوکوق_t *ڤوکوق,
    uint32_t کونچي, uint32_t mask, uintptr_t نيلاي);
ngx_int_t ngx_radix32tree_delete(ngx_radix_ڤوکوق_t *ڤوکوق,
    uint32_t کونچي, uint32_t mask);
uintptr_t ngx_radix32tree_find(ngx_radix_ڤوکوق_t *ڤوکوق, uint32_t کونچي);

#إذا (NGX_HAVE_INET6)
ngx_int_t ngx_radix128tree_سيسيڤ(ngx_radix_ڤوکوق_t *ڤوکوق,
    u_char *کونچي, u_char *mask, uintptr_t نيلاي);
ngx_int_t ngx_radix128tree_delete(ngx_radix_ڤوکوق_t *ڤوکوق,
    u_char *کونچي, u_char *mask);
uintptr_t ngx_radix128tree_find(ngx_radix_ڤوکوق_t *ڤوکوق, u_char *کونچي);
#endif


#endif /* _NGX_RADIX_TREE_H_INCLUDED_ */
