
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef NGX_RADIX_वृक्ष_H_INCLUDED
#define NGX_RADIX_वृक्ष_H_INCLUDED


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


#define NGX_RADIX_NO_मूल्य   (uintptr_t) -1

प्रकारनिर्देश संरचना ngx_radix_गाँठ_s  ngx_radix_गाँठ_t;

संरचना ngx_radix_गाँठ_s {
    ngx_radix_गाँठ_t  *दायाँ;
    ngx_radix_गाँठ_t  *बायाँ;
    ngx_radix_गाँठ_t  *parent;
    uintptr_t          मूल्य;
};


प्रकारनिर्देश संरचना {
    ngx_radix_गाँठ_t  *मूल;
    ngx_pool_t        *pool;
    ngx_radix_गाँठ_t  *मुक्त;
    वर्ण              *start;
    size_t             size;
} ngx_radix_वृक्ष_t;


ngx_radix_वृक्ष_t *ngx_radix_वृक्ष_create(ngx_pool_t *pool,
    ngx_int_t preallocate);

ngx_int_t ngx_radix32tree_जोड़ो(ngx_radix_वृक्ष_t *वृक्ष,
    uint32_t कुंजी, uint32_t mask, uintptr_t मूल्य);
ngx_int_t ngx_radix32tree_delete(ngx_radix_वृक्ष_t *वृक्ष,
    uint32_t कुंजी, uint32_t mask);
uintptr_t ngx_radix32tree_find(ngx_radix_वृक्ष_t *वृक्ष, uint32_t कुंजी);

#यदि (NGX_HAVE_INET6)
ngx_int_t ngx_radix128tree_जोड़ो(ngx_radix_वृक्ष_t *वृक्ष,
    u_char *कुंजी, u_char *mask, uintptr_t मूल्य);
ngx_int_t ngx_radix128tree_delete(ngx_radix_वृक्ष_t *वृक्ष,
    u_char *कुंजी, u_char *mask);
uintptr_t ngx_radix128tree_find(ngx_radix_वृक्ष_t *वृक्ष, u_char *कुंजी);
#endif


#endif /* _NGX_RADIX_TREE_H_INCLUDED_ */
