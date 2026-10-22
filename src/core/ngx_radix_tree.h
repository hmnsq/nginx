
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef NGX_RADIX_دەرەخ_H_INCLUDED
#define NGX_RADIX_دەرەخ_H_INCLUDED


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


#define NGX_RADIX_NO_قىممەت   (uintptr_t) -1

تعريفنوع بنية ngx_radix_تۈگۈن_s  ngx_radix_تۈگۈن_t;

بنية ngx_radix_تۈگۈن_s {
    ngx_radix_تۈگۈن_t  *ئوڭ;
    ngx_radix_تۈگۈن_t  *سول;
    ngx_radix_تۈگۈن_t  *parent;
    uintptr_t          قىممەت;
};


تعريفنوع بنية {
    ngx_radix_تۈگۈن_t  *يىلتىز;
    ngx_pool_t        *pool;
    ngx_radix_تۈگۈن_t  *حرر;
    حرف              *start;
    size_t             size;
} ngx_radix_دەرەخ_t;


ngx_radix_دەرەخ_t *ngx_radix_دەرەخ_create(ngx_pool_t *pool,
    ngx_int_t preallocate);

ngx_int_t ngx_radix32tree_قىستۇرۇش(ngx_radix_دەرەخ_t *دەرەخ,
    uint32_t ئاچقۇچ, uint32_t mask, uintptr_t قىممەت);
ngx_int_t ngx_radix32tree_delete(ngx_radix_دەرەخ_t *دەرەخ,
    uint32_t ئاچقۇچ, uint32_t mask);
uintptr_t ngx_radix32tree_find(ngx_radix_دەرەخ_t *دەرەخ, uint32_t ئاچقۇچ);

#إذا (NGX_HAVE_INET6)
ngx_int_t ngx_radix128tree_قىستۇرۇش(ngx_radix_دەرەخ_t *دەرەخ,
    u_char *ئاچقۇچ, u_char *mask, uintptr_t قىممەت);
ngx_int_t ngx_radix128tree_delete(ngx_radix_دەرەخ_t *دەرەخ,
    u_char *ئاچقۇچ, u_char *mask);
uintptr_t ngx_radix128tree_find(ngx_radix_دەرەخ_t *دەرەخ, u_char *ئاچقۇچ);
#endif


#endif /* _NGX_RADIX_TREE_H_INCLUDED_ */
