
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अगर_अघोषित NGX_RADIX_वृक्ष_H_INCLUDED
#घोषणा NGX_RADIX_वृक्ष_H_INCLUDED


#समावेश <ngx_config.h>
#समावेश <ngx_core.h>


#घोषणा NGX_RADIX_NO_मूल्य   (uintptr_t) -1

प्रकार काष्ठा ngx_radix_गाँठ_s  ngx_radix_गाँठ_t;

काष्ठा ngx_radix_गाँठ_s {
    ngx_radix_गाँठ_t  *दायाँ;
    ngx_radix_गाँठ_t  *बायाँ;
    ngx_radix_गाँठ_t  *parent;
    uintptr_t          मूल्य;
};


प्रकार काष्ठा {
    ngx_radix_गाँठ_t  *मूल;
    ngx_pool_t        *pool;
    ngx_radix_गाँठ_t  *मुक्त;
    अक्षर              *start;
    माप_प्रकार             आकार;
} ngx_radix_वृक्ष_t;


ngx_radix_वृक्ष_t *ngx_radix_वृक्ष_create(ngx_pool_t *pool,
    ngx_int_t preallocate);

ngx_int_t ngx_radix32tree_जोड़ो(ngx_radix_वृक्ष_t *वृक्ष,
    uint32_t कुंजी, uint32_t mask, uintptr_t मूल्य);
ngx_int_t ngx_radix32tree_delete(ngx_radix_वृक्ष_t *वृक्ष,
    uint32_t कुंजी, uint32_t mask);
uintptr_t ngx_radix32tree_find(ngx_radix_वृक्ष_t *वृक्ष, uint32_t कुंजी);

#अगर (NGX_HAVE_INET6)
ngx_int_t ngx_radix128tree_जोड़ो(ngx_radix_वृक्ष_t *वृक्ष,
    u_char *कुंजी, u_char *mask, uintptr_t मूल्य);
ngx_int_t ngx_radix128tree_delete(ngx_radix_वृक्ष_t *वृक्ष,
    u_char *कुंजी, u_char *mask);
uintptr_t ngx_radix128tree_find(ngx_radix_वृक्ष_t *वृक्ष, u_char *कुंजी);
#पूर्ण_अगर


#पूर्ण_अगर /* _NGX_RADIX_TREE_H_INCLUDED_ */
