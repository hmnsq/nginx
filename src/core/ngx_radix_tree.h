
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef NGX_RADIX_വൃക്ഷം_H_INCLUDED
#define NGX_RADIX_വൃക്ഷം_H_INCLUDED


#അന്തര്ഭാവ <ngx_config.h>
#അന്തര്ഭാവ <ngx_core.h>


#define NGX_RADIX_NO_മൂല്യം   (uintptr_t) -1

തരനിർവചനം ഘടന ngx_radix_കണ്ണി_s  ngx_radix_കണ്ണി_t;

ഘടന ngx_radix_കണ്ണി_s {
    ngx_radix_കണ്ണി_t  *വലത്;
    ngx_radix_കണ്ണി_t  *ഇടത്;
    ngx_radix_കണ്ണി_t  *parent;
    uintptr_t          മൂല്യം;
};


തരനിർവചനം ഘടന {
    ngx_radix_കണ്ണി_t  *വേര്;
    ngx_pool_t        *pool;
    ngx_radix_കണ്ണി_t  *സ്വതന്ത്രം;
    അക്ഷരം              *start;
    size_t             size;
} ngx_radix_വൃക്ഷം_t;


ngx_radix_വൃക്ഷം_t *ngx_radix_വൃക്ഷം_create(ngx_pool_t *pool,
    ngx_int_t preallocate);

ngx_int_t ngx_radix32tree_തിരുകുക(ngx_radix_വൃക്ഷം_t *വൃക്ഷം,
    uint32_t താക്കോൽ, uint32_t mask, uintptr_t മൂല്യം);
ngx_int_t ngx_radix32tree_delete(ngx_radix_വൃക്ഷം_t *വൃക്ഷം,
    uint32_t താക്കോൽ, uint32_t mask);
uintptr_t ngx_radix32tree_find(ngx_radix_വൃക്ഷം_t *വൃക്ഷം, uint32_t താക്കോൽ);

#എങ്കിൽ (NGX_HAVE_INET6)
ngx_int_t ngx_radix128tree_തിരുകുക(ngx_radix_വൃക്ഷം_t *വൃക്ഷം,
    u_char *താക്കോൽ, u_char *mask, uintptr_t മൂല്യം);
ngx_int_t ngx_radix128tree_delete(ngx_radix_വൃക്ഷം_t *വൃക്ഷം,
    u_char *താക്കോൽ, u_char *mask);
uintptr_t ngx_radix128tree_find(ngx_radix_വൃക്ഷം_t *വൃക്ഷം, u_char *താക്കോൽ);
#endif


#endif /* _NGX_RADIX_TREE_H_INCLUDED_ */
