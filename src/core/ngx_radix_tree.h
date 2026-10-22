
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#اگر_تعریف_نشده NGX_RADIX_درخت_H_INCLUDED
#تعریف_کن NGX_RADIX_درخت_H_INCLUDED


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


#تعریف_کن NGX_RADIX_NO_مقدار   (uintptr_t) -1

تعريفنوع بنية ngx_radix_گره_s  ngx_radix_گره_t;

بنية ngx_radix_گره_s {
    ngx_radix_گره_t  *راست;
    ngx_radix_گره_t  *چپ;
    ngx_radix_گره_t  *والد;
    uintptr_t          مقدار;
};


تعريفنوع بنية {
    ngx_radix_گره_t  *ریشه;
    ngx_pool_t        *pool;
    ngx_radix_گره_t  *حرر;
    حرف              *شروع;
    اندازه_t             اندازه;
} ngx_radix_درخت_t;


ngx_radix_درخت_t *ngx_radix_درخت_create(ngx_pool_t *pool,
    ngx_int_t preallocate);

ngx_int_t ngx_radix32tree_درج(ngx_radix_درخت_t *درخت,
    uint32_t کلید, uint32_t ماسک, uintptr_t مقدار);
ngx_int_t ngx_radix32tree_delete(ngx_radix_درخت_t *درخت,
    uint32_t کلید, uint32_t ماسک);
uintptr_t ngx_radix32tree_پیدا_کن(ngx_radix_درخت_t *درخت, uint32_t کلید);

#إذا (NGX_HAVE_INET6)
ngx_int_t ngx_radix128tree_درج(ngx_radix_درخت_t *درخت,
    u_char *کلید, u_char *ماسک, uintptr_t مقدار);
ngx_int_t ngx_radix128tree_delete(ngx_radix_درخت_t *درخت,
    u_char *کلید, u_char *ماسک);
uintptr_t ngx_radix128tree_پیدا_کن(ngx_radix_درخت_t *درخت, u_char *کلید);
#پایان_اگر


#پایان_اگر /* _NGX_RADIX_TREE_H_INCLUDED_ */
