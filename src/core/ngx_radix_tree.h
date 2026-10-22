
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef NGX_RADIX_درخت_H_INCLUDED
#define NGX_RADIX_درخت_H_INCLUDED


#شامل <ngx_config.h>
#شامل <ngx_core.h>


#define NGX_RADIX_NO_مقدار   (uintptr_t) -1

تعریفنوع ساختار ngx_radix_گره_s  ngx_radix_گره_t;

ساختار ngx_radix_گره_s {
    ngx_radix_گره_t  *راست;
    ngx_radix_گره_t  *چپ;
    ngx_radix_گره_t  *parent;
    uintptr_t          مقدار;
};


تعریفنوع ساختار {
    ngx_radix_گره_t  *ریشه;
    ngx_pool_t        *pool;
    ngx_radix_گره_t  *آزاد;
    نویسه              *start;
    size_t             size;
} ngx_radix_درخت_t;


ngx_radix_درخت_t *ngx_radix_درخت_create(ngx_pool_t *pool,
    ngx_int_t preallocate);

ngx_int_t ngx_radix32tree_درج(ngx_radix_درخت_t *درخت,
    uint32_t کلید, uint32_t mask, uintptr_t مقدار);
ngx_int_t ngx_radix32tree_delete(ngx_radix_درخت_t *درخت,
    uint32_t کلید, uint32_t mask);
uintptr_t ngx_radix32tree_find(ngx_radix_درخت_t *درخت, uint32_t کلید);

#اگر (NGX_HAVE_INET6)
ngx_int_t ngx_radix128tree_درج(ngx_radix_درخت_t *درخت,
    u_char *کلید, u_char *mask, uintptr_t مقدار);
ngx_int_t ngx_radix128tree_delete(ngx_radix_درخت_t *درخت,
    u_char *کلید, u_char *mask);
uintptr_t ngx_radix128tree_find(ngx_radix_درخت_t *درخت, u_char *کلید);
#endif


#endif /* _NGX_RADIX_TREE_H_INCLUDED_ */
