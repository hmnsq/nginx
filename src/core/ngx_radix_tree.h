
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef NGX_RADIX_بشيا_H_INCLUDED
#define NGX_RADIX_بشيا_H_INCLUDED


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


#define NGX_RADIX_NO_درج   (uintptr_t) -1

تعريفنوع بنية ngx_radix_كلي_s  ngx_radix_كلي_t;

بنية ngx_radix_كلي_s {
    ngx_radix_كلي_t  *دام;
    ngx_radix_كلي_t  *هغو;
    ngx_radix_كلي_t  *parent;
    uintptr_t          درج;
};


تعريفنوع بنية {
    ngx_radix_كلي_t  *سيو;
    ngx_pool_t        *pool;
    ngx_radix_كلي_t  *حرر;
    حرف              *start;
    size_t             size;
} ngx_radix_بشيا_t;


ngx_radix_بشيا_t *ngx_radix_بشيا_create(ngx_pool_t *pool,
    ngx_int_t preallocate);

ngx_int_t ngx_radix32tree_سك(ngx_radix_بشيا_t *بشيا,
    uint32_t مبودي, uint32_t mask, uintptr_t درج);
ngx_int_t ngx_radix32tree_delete(ngx_radix_بشيا_t *بشيا,
    uint32_t مبودي, uint32_t mask);
uintptr_t ngx_radix32tree_find(ngx_radix_بشيا_t *بشيا, uint32_t مبودي);

#إذا (NGX_HAVE_INET6)
ngx_int_t ngx_radix128tree_سك(ngx_radix_بشيا_t *بشيا,
    u_char *مبودي, u_char *mask, uintptr_t درج);
ngx_int_t ngx_radix128tree_delete(ngx_radix_بشيا_t *بشيا,
    u_char *مبودي, u_char *mask);
uintptr_t ngx_radix128tree_find(ngx_radix_بشيا_t *بشيا, u_char *مبودي);
#endif


#endif /* _NGX_RADIX_TREE_H_INCLUDED_ */
