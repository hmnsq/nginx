
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ NGX_RADIX_ਰੁੱਖ_H_INCLUDED
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_RADIX_ਰੁੱਖ_H_INCLUDED


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_RADIX_NO_ਮੁੱਲ   (uintptr_t) -1

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ngx_radix_ਗੰਢ_s  ngx_radix_ਗੰਢ_t;

ਬਣਤਰ ngx_radix_ਗੰਢ_s {
    ngx_radix_ਗੰਢ_t  *ਸੱਜਾ;
    ngx_radix_ਗੰਢ_t  *ਖੱਬਾ;
    ngx_radix_ਗੰਢ_t  *ਮਾਪਾ;
    uintptr_t          ਮੁੱਲ;
};


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ngx_radix_ਗੰਢ_t  *ਜੜ੍ਹ;
    ngx_pool_t        *pool;
    ngx_radix_ਗੰਢ_t  *ਮੁਕਤ;
    ਅੱਖਰ              *ਸ਼ੁਰੂ;
    ਮਾਪ_t             ਮਾਪ;
} ngx_radix_ਰੁੱਖ_t;


ngx_radix_ਰੁੱਖ_t *ngx_radix_ਰੁੱਖ_create(ngx_pool_t *pool,
    ngx_int_t preallocate);

ngx_int_t ngx_radix32tree_ਪਾਓ(ngx_radix_ਰੁੱਖ_t *ਰੁੱਖ,
    uint32_t ਕੁੰਜੀ, uint32_t ਨਕਾਬ, uintptr_t ਮੁੱਲ);
ngx_int_t ngx_radix32tree_delete(ngx_radix_ਰੁੱਖ_t *ਰੁੱਖ,
    uint32_t ਕੁੰਜੀ, uint32_t ਨਕਾਬ);
uintptr_t ngx_radix32tree_ਲੱਭੋ(ngx_radix_ਰੁੱਖ_t *ਰੁੱਖ, uint32_t ਕੁੰਜੀ);

#ਜੇ (NGX_HAVE_INET6)
ngx_int_t ngx_radix128tree_ਪਾਓ(ngx_radix_ਰੁੱਖ_t *ਰੁੱਖ,
    u_char *ਕੁੰਜੀ, u_char *ਨਕਾਬ, uintptr_t ਮੁੱਲ);
ngx_int_t ngx_radix128tree_delete(ngx_radix_ਰੁੱਖ_t *ਰੁੱਖ,
    u_char *ਕੁੰਜੀ, u_char *ਨਕਾਬ);
uintptr_t ngx_radix128tree_ਲੱਭੋ(ngx_radix_ਰੁੱਖ_t *ਰੁੱਖ, u_char *ਕੁੰਜੀ);
#ਜੇ_ਅੰਤ


#ਜੇ_ਅੰਤ /* _NGX_RADIX_TREE_H_INCLUDED_ */
