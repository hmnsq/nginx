
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef NGX_RADIX_மரம்_H_INCLUDED
#define NGX_RADIX_மரம்_H_INCLUDED


#அந்தர்பாவ <ngx_config.h>
#அந்தர்பாவ <ngx_core.h>


#define NGX_RADIX_NO_மதிப்பு   (uintptr_t) -1

வகைவரை கட்டமைப்பு ngx_radix_முனை_s  ngx_radix_முனை_t;

கட்டமைப்பு ngx_radix_முனை_s {
    ngx_radix_முனை_t  *வலது;
    ngx_radix_முனை_t  *இடது;
    ngx_radix_முனை_t  *parent;
    uintptr_t          மதிப்பு;
};


வகைவரை கட்டமைப்பு {
    ngx_radix_முனை_t  *வேர்;
    ngx_pool_t        *pool;
    ngx_radix_முனை_t  *விடுவி;
    எழுத்து              *start;
    size_t             size;
} ngx_radix_மரம்_t;


ngx_radix_மரம்_t *ngx_radix_மரம்_create(ngx_pool_t *pool,
    ngx_int_t preallocate);

ngx_int_t ngx_radix32tree_செருகு(ngx_radix_மரம்_t *மரம்,
    uint32_t சாவி, uint32_t mask, uintptr_t மதிப்பு);
ngx_int_t ngx_radix32tree_delete(ngx_radix_மரம்_t *மரம்,
    uint32_t சாவி, uint32_t mask);
uintptr_t ngx_radix32tree_find(ngx_radix_மரம்_t *மரம், uint32_t சாவி);

#எனில் (NGX_HAVE_INET6)
ngx_int_t ngx_radix128tree_செருகு(ngx_radix_மரம்_t *மரம்,
    u_char *சாவி, u_char *mask, uintptr_t மதிப்பு);
ngx_int_t ngx_radix128tree_delete(ngx_radix_மரம்_t *மரம்,
    u_char *சாவி, u_char *mask);
uintptr_t ngx_radix128tree_find(ngx_radix_மரம்_t *மரம், u_char *சாவி);
#endif


#endif /* _NGX_RADIX_TREE_H_INCLUDED_ */
