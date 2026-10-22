
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef NGX_RADIX_درخت_H_INCLUDED
#define NGX_RADIX_درخت_H_INCLUDED


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


#define NGX_RADIX_NO_قدر   (uintptr_t) -1

تعريفنوع بنية ngx_radix_گرہ_s  ngx_radix_گرہ_t;

بنية ngx_radix_گرہ_s {
    ngx_radix_گرہ_t  *دایاں;
    ngx_radix_گرہ_t  *بایاں;
    ngx_radix_گرہ_t  *parent;
    uintptr_t          قدر;
};


تعريفنوع بنية {
    ngx_radix_گرہ_t  *جڑ;
    ngx_pool_t        *pool;
    ngx_radix_گرہ_t  *حرر;
    حرف              *start;
    size_t             size;
} ngx_radix_درخت_t;


ngx_radix_درخت_t *ngx_radix_درخت_create(ngx_pool_t *pool,
    ngx_int_t preallocate);

ngx_int_t ngx_radix32tree_داخل_کرو(ngx_radix_درخت_t *درخت,
    uint32_t کلید, uint32_t mask, uintptr_t قدر);
ngx_int_t ngx_radix32tree_delete(ngx_radix_درخت_t *درخت,
    uint32_t کلید, uint32_t mask);
uintptr_t ngx_radix32tree_find(ngx_radix_درخت_t *درخت, uint32_t کلید);

#إذا (NGX_HAVE_INET6)
ngx_int_t ngx_radix128tree_داخل_کرو(ngx_radix_درخت_t *درخت,
    u_char *کلید, u_char *mask, uintptr_t قدر);
ngx_int_t ngx_radix128tree_delete(ngx_radix_درخت_t *درخت,
    u_char *کلید, u_char *mask);
uintptr_t ngx_radix128tree_find(ngx_radix_درخت_t *درخت, u_char *کلید);
#endif


#endif /* _NGX_RADIX_TREE_H_INCLUDED_ */
