
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#यदि_अपरिभाषितम् NGX_RADIX_वृक्षः_H_INCLUDED
#परिभाषय NGX_RADIX_वृक्षः_H_INCLUDED


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


#परिभाषय NGX_RADIX_NO_मानम्   (uintptr_t) -1

प्रकारनिर्देश संरचना ngx_radix_ग्रन्थिः_s  ngx_radix_ग्रन्थिः_t;

संरचना ngx_radix_ग्रन्थिः_s {
    ngx_radix_ग्रन्थिः_t  *दक्षिणः;
    ngx_radix_ग्रन्थिः_t  *वामः;
    ngx_radix_ग्रन्थिः_t  *parent;
    uintptr_t          मानम्;
};


प्रकारनिर्देश संरचना {
    ngx_radix_ग्रन्थिः_t  *मूलम्;
    ngx_pool_t        *pool;
    ngx_radix_ग्रन्थिः_t  *मुक्त;
    वर्ण              *start;
    आकार_t             आकार;
} ngx_radix_वृक्षः_t;


ngx_radix_वृक्षः_t *ngx_radix_वृक्षः_create(ngx_pool_t *pool,
    ngx_int_t preallocate);

ngx_int_t ngx_radix32tree_निवेशय(ngx_radix_वृक्षः_t *वृक्षः,
    uint32_t कुञ्चिका, uint32_t आवरणम्, uintptr_t मानम्);
ngx_int_t ngx_radix32tree_delete(ngx_radix_वृक्षः_t *वृक्षः,
    uint32_t कुञ्चिका, uint32_t आवरणम्);
uintptr_t ngx_radix32tree_find(ngx_radix_वृक्षः_t *वृक्षः, uint32_t कुञ्चिका);

#यदि (NGX_HAVE_INET6)
ngx_int_t ngx_radix128tree_निवेशय(ngx_radix_वृक्षः_t *वृक्षः,
    u_char *कुञ्चिका, u_char *आवरणम्, uintptr_t मानम्);
ngx_int_t ngx_radix128tree_delete(ngx_radix_वृक्षः_t *वृक्षः,
    u_char *कुञ्चिका, u_char *आवरणम्);
uintptr_t ngx_radix128tree_find(ngx_radix_वृक्षः_t *वृक्षः, u_char *कुञ्चिका);
#यदि_अन्तः


#यदि_अन्तः /* _NGX_RADIX_TREE_H_INCLUDED_ */
