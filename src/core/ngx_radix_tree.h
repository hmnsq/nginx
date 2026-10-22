
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef NGX_RADIX_ବୃକ୍ଷ_H_INCLUDED
#define NGX_RADIX_ବୃକ୍ଷ_H_INCLUDED


#ଅନ୍ତର୍ଭାଵ <ngx_config.h>
#ଅନ୍ତର୍ଭାଵ <ngx_core.h>


#define NGX_RADIX_NO_ମୂଲ୍ୟ   (uintptr_t) -1

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା ngx_radix_ଗଣ୍ଠି_s  ngx_radix_ଗଣ୍ଠି_t;

ସଂରଚନା ngx_radix_ଗଣ୍ଠି_s {
    ngx_radix_ଗଣ୍ଠି_t  *ଡାହାଣ;
    ngx_radix_ଗଣ୍ଠି_t  *ବାମ;
    ngx_radix_ଗଣ୍ଠି_t  *parent;
    uintptr_t          ମୂଲ୍ୟ;
};


ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ngx_radix_ଗଣ୍ଠି_t  *ମୂଳ;
    ngx_pool_t        *pool;
    ngx_radix_ଗଣ୍ଠି_t  *ମୁକ୍ତ;
    ଅକ୍ଷର              *start;
    size_t             size;
} ngx_radix_ବୃକ୍ଷ_t;


ngx_radix_ବୃକ୍ଷ_t *ngx_radix_ବୃକ୍ଷ_create(ngx_pool_t *pool,
    ngx_int_t preallocate);

ngx_int_t ngx_radix32tree_ଭର୍ତ୍ତି(ngx_radix_ବୃକ୍ଷ_t *ବୃକ୍ଷ,
    uint32_t ଚାବି, uint32_t mask, uintptr_t ମୂଲ୍ୟ);
ngx_int_t ngx_radix32tree_delete(ngx_radix_ବୃକ୍ଷ_t *ବୃକ୍ଷ,
    uint32_t ଚାବି, uint32_t mask);
uintptr_t ngx_radix32tree_find(ngx_radix_ବୃକ୍ଷ_t *ବୃକ୍ଷ, uint32_t ଚାବି);

#ଯଦି (NGX_HAVE_INET6)
ngx_int_t ngx_radix128tree_ଭର୍ତ୍ତି(ngx_radix_ବୃକ୍ଷ_t *ବୃକ୍ଷ,
    u_char *ଚାବି, u_char *mask, uintptr_t ମୂଲ୍ୟ);
ngx_int_t ngx_radix128tree_delete(ngx_radix_ବୃକ୍ଷ_t *ବୃକ୍ଷ,
    u_char *ଚାବି, u_char *mask);
uintptr_t ngx_radix128tree_find(ngx_radix_ବୃକ୍ଷ_t *ବୃକ୍ଷ, u_char *ଚାବି);
#endif


#endif /* _NGX_RADIX_TREE_H_INCLUDED_ */
