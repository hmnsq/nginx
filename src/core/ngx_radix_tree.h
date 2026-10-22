
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef NGX_RADIX_شجرة_H_INCLUDED
#define NGX_RADIX_شجرة_H_INCLUDED


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


#define NGX_RADIX_NO_قيمة   (uintptr_t) -1

تعريفنوع بنية ngx_radix_عقدة_s  ngx_radix_عقدة_t;

بنية ngx_radix_عقدة_s {
    ngx_radix_عقدة_t  *يمين;
    ngx_radix_عقدة_t  *يسار;
    ngx_radix_عقدة_t  *parent;
    uintptr_t          قيمة;
};


تعريفنوع بنية {
    ngx_radix_عقدة_t  *جذر;
    ngx_pool_t        *pool;
    ngx_radix_عقدة_t  *حرر;
    حرف              *start;
    size_t             size;
} ngx_radix_شجرة_t;


ngx_radix_شجرة_t *ngx_radix_شجرة_create(ngx_pool_t *pool,
    ngx_int_t preallocate);

ngx_int_t ngx_radix32tree_أدرج(ngx_radix_شجرة_t *شجرة,
    uint32_t مفتاح, uint32_t mask, uintptr_t قيمة);
ngx_int_t ngx_radix32tree_delete(ngx_radix_شجرة_t *شجرة,
    uint32_t مفتاح, uint32_t mask);
uintptr_t ngx_radix32tree_find(ngx_radix_شجرة_t *شجرة, uint32_t مفتاح);

#إذا (NGX_HAVE_INET6)
ngx_int_t ngx_radix128tree_أدرج(ngx_radix_شجرة_t *شجرة,
    u_char *مفتاح, u_char *mask, uintptr_t قيمة);
ngx_int_t ngx_radix128tree_delete(ngx_radix_شجرة_t *شجرة,
    u_char *مفتاح, u_char *mask);
uintptr_t ngx_radix128tree_find(ngx_radix_شجرة_t *شجرة, u_char *مفتاح);
#endif


#endif /* _NGX_RADIX_TREE_H_INCLUDED_ */
