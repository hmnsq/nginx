
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef NGX_RADIX_रूख_H_INCLUDED
#define NGX_RADIX_रूख_H_INCLUDED


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


#define NGX_RADIX_NO_मान   (uintptr_t) -1

प्रकारनिर्देश संरचना ngx_radix_गाँठो_s  ngx_radix_गाँठो_t;

संरचना ngx_radix_गाँठो_s {
    ngx_radix_गाँठो_t  *दायाँ;
    ngx_radix_गाँठो_t  *बायाँ;
    ngx_radix_गाँठो_t  *parent;
    uintptr_t          मान;
};


प्रकारनिर्देश संरचना {
    ngx_radix_गाँठो_t  *जरा;
    ngx_pool_t        *pool;
    ngx_radix_गाँठो_t  *मुक्त;
    वर्ण              *start;
    माप_प्रकार             size;
} ngx_radix_रूख_t;


ngx_radix_रूख_t *ngx_radix_रूख_create(ngx_pool_t *pool,
    ngx_int_t preallocate);

ngx_int_t ngx_radix32tree_घुसाउ(ngx_radix_रूख_t *रूख,
    uint32_t साँचो, uint32_t mask, uintptr_t मान);
ngx_int_t ngx_radix32tree_delete(ngx_radix_रूख_t *रूख,
    uint32_t साँचो, uint32_t mask);
uintptr_t ngx_radix32tree_find(ngx_radix_रूख_t *रूख, uint32_t साँचो);

#यदि (NGX_HAVE_INET6)
ngx_int_t ngx_radix128tree_घुसाउ(ngx_radix_रूख_t *रूख,
    u_char *साँचो, u_char *mask, uintptr_t मान);
ngx_int_t ngx_radix128tree_delete(ngx_radix_रूख_t *रूख,
    u_char *साँचो, u_char *mask);
uintptr_t ngx_radix128tree_find(ngx_radix_रूख_t *रूख, u_char *साँचो);
#endif


#endif /* _NGX_RADIX_TREE_H_INCLUDED_ */
