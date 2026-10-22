
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#جيڪڏهن_وصف_نه_ٿيل NGX_RADIX_وڻ_H_INCLUDED
#وصف_ڪريو NGX_RADIX_وڻ_H_INCLUDED


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


#وصف_ڪريو NGX_RADIX_NO_قدر   (uintptr_t) -1

تعريفنوع بنية ngx_radix_ڳنڍ_s  ngx_radix_ڳنڍ_t;

بنية ngx_radix_ڳنڍ_s {
    ngx_radix_ڳنڍ_t  *ساڄو;
    ngx_radix_ڳنڍ_t  *کاٻو;
    ngx_radix_ڳنڍ_t  *والد;
    uintptr_t          قدر;
};


تعريفنوع بنية {
    ngx_radix_ڳنڍ_t  *پاڙ;
    ngx_pool_t        *pool;
    ngx_radix_ڳنڍ_t  *حرر;
    حرف              *شروعات;
    سائيز_t             سائيز;
} ngx_radix_وڻ_t;


ngx_radix_وڻ_t *ngx_radix_وڻ_create(ngx_pool_t *pool,
    ngx_int_t preallocate);

ngx_int_t ngx_radix32tree_وجهو(ngx_radix_وڻ_t *وڻ,
    uint32_t ڪنجي, uint32_t نقاب, uintptr_t قدر);
ngx_int_t ngx_radix32tree_delete(ngx_radix_وڻ_t *وڻ,
    uint32_t ڪنجي, uint32_t نقاب);
uintptr_t ngx_radix32tree_ڳوليو(ngx_radix_وڻ_t *وڻ, uint32_t ڪنجي);

#إذا (NGX_HAVE_INET6)
ngx_int_t ngx_radix128tree_وجهو(ngx_radix_وڻ_t *وڻ,
    u_char *ڪنجي, u_char *نقاب, uintptr_t قدر);
ngx_int_t ngx_radix128tree_delete(ngx_radix_وڻ_t *وڻ,
    u_char *ڪنجي, u_char *نقاب);
uintptr_t ngx_radix128tree_ڳوليو(ngx_radix_وڻ_t *وڻ, u_char *ڪنجي);
#جيڪڏهن_پورو


#جيڪڏهن_پورو /* _NGX_RADIX_TREE_H_INCLUDED_ */
