
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef NGX_RADIX_درەخت_H_INCLUDED
#define NGX_RADIX_درەخت_H_INCLUDED


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


#define NGX_RADIX_NO_بەها   (uintptr_t) -1

تعريفنوع بنية ngx_radix_گرێ_s  ngx_radix_گرێ_t;

بنية ngx_radix_گرێ_s {
    ngx_radix_گرێ_t  *ڕاست;
    ngx_radix_گرێ_t  *چەپ;
    ngx_radix_گرێ_t  *parent;
    uintptr_t          بەها;
};


تعريفنوع بنية {
    ngx_radix_گرێ_t  *ڕەگ;
    ngx_pool_t        *pool;
    ngx_radix_گرێ_t  *حرر;
    حرف              *start;
    size_t             size;
} ngx_radix_درەخت_t;


ngx_radix_درەخت_t *ngx_radix_درەخت_create(ngx_pool_t *pool,
    ngx_int_t preallocate);

ngx_int_t ngx_radix32tree_زیادکردن(ngx_radix_درەخت_t *درەخت,
    uint32_t کلیل, uint32_t mask, uintptr_t بەها);
ngx_int_t ngx_radix32tree_delete(ngx_radix_درەخت_t *درەخت,
    uint32_t کلیل, uint32_t mask);
uintptr_t ngx_radix32tree_find(ngx_radix_درەخت_t *درەخت, uint32_t کلیل);

#إذا (NGX_HAVE_INET6)
ngx_int_t ngx_radix128tree_زیادکردن(ngx_radix_درەخت_t *درەخت,
    u_char *کلیل, u_char *mask, uintptr_t بەها);
ngx_int_t ngx_radix128tree_delete(ngx_radix_درەخت_t *درەخت,
    u_char *کلیل, u_char *mask);
uintptr_t ngx_radix128tree_find(ngx_radix_درەخت_t *درەخت, u_char *کلیل);
#endif


#endif /* _NGX_RADIX_TREE_H_INCLUDED_ */
