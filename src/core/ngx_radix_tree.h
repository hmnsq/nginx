
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#که_نه_تعریف_شوی NGX_RADIX_ونه_H_INCLUDED
#تعریف_کړه NGX_RADIX_ونه_H_INCLUDED


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


#تعریف_کړه NGX_RADIX_NO_ارزښت   (uintptr_t) -1

تعريفنوع بنية ngx_radix_غوټه_s  ngx_radix_غوټه_t;

بنية ngx_radix_غوټه_s {
    ngx_radix_غوټه_t  *ښی;
    ngx_radix_غوټه_t  *کیڼ;
    ngx_radix_غوټه_t  *پلار;
    uintptr_t          ارزښت;
};


تعريفنوع بنية {
    ngx_radix_غوټه_t  *ریښه;
    ngx_pool_t        *pool;
    ngx_radix_غوټه_t  *حرر;
    حرف              *پیل;
    اندازه_t             اندازه;
} ngx_radix_ونه_t;


ngx_radix_ونه_t *ngx_radix_ونه_create(ngx_pool_t *pool,
    ngx_int_t preallocate);

ngx_int_t ngx_radix32tree_ورننباسه(ngx_radix_ونه_t *ونه,
    uint32_t کلی, uint32_t ماسک, uintptr_t ارزښت);
ngx_int_t ngx_radix32tree_delete(ngx_radix_ونه_t *ونه,
    uint32_t کلی, uint32_t ماسک);
uintptr_t ngx_radix32tree_پیدا_کړه(ngx_radix_ونه_t *ونه, uint32_t کلی);

#إذا (NGX_HAVE_INET6)
ngx_int_t ngx_radix128tree_ورننباسه(ngx_radix_ونه_t *ونه,
    u_char *کلی, u_char *ماسک, uintptr_t ارزښت);
ngx_int_t ngx_radix128tree_delete(ngx_radix_ونه_t *ونه,
    u_char *کلی, u_char *ماسک);
uintptr_t ngx_radix128tree_پیدا_کړه(ngx_radix_ونه_t *ونه, u_char *کلی);
#د_که_پای


#د_که_پای /* _NGX_RADIX_TREE_H_INCLUDED_ */
