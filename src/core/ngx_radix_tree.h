
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef NGX_RADIX_पुनो_H_INCLUDED
#define NGX_RADIX_पुनो_H_INCLUDED


#include <ngx_config.h>
#include <ngx_core.h>


#define NGX_RADIX_NO_हालागा   (uintptr_t) -1

typedef struct ngx_radix_बुहोल_s  ngx_radix_बुहोल_t;

struct ngx_radix_बुहोल_s {
    ngx_radix_बुहोल_t  *कानान;
    ngx_radix_बुहोल_t  *कालिवा;
    ngx_radix_बुहोल_t  *parent;
    uintptr_t          हालागा;
};


typedef struct {
    ngx_radix_बुहोल_t  *उगात;
    ngx_pool_t        *pool;
    ngx_radix_बुहोल_t  *free;
    char              *start;
    size_t             size;
} ngx_radix_पुनो_t;


ngx_radix_पुनो_t *ngx_radix_पुनो_create(ngx_pool_t *pool,
    ngx_int_t preallocate);

ngx_int_t ngx_radix32tree_इसिंगित(ngx_radix_पुनो_t *पुनो,
    uint32_t सुसि, uint32_t mask, uintptr_t हालागा);
ngx_int_t ngx_radix32tree_delete(ngx_radix_पुनो_t *पुनो,
    uint32_t सुसि, uint32_t mask);
uintptr_t ngx_radix32tree_find(ngx_radix_पुनो_t *पुनो, uint32_t सुसि);

#if (NGX_HAVE_INET6)
ngx_int_t ngx_radix128tree_इसिंगित(ngx_radix_पुनो_t *पुनो,
    u_char *सुसि, u_char *mask, uintptr_t हालागा);
ngx_int_t ngx_radix128tree_delete(ngx_radix_पुनो_t *पुनो,
    u_char *सुसि, u_char *mask);
uintptr_t ngx_radix128tree_find(ngx_radix_पुनो_t *पुनो, u_char *सुसि);
#endif


#endif /* _NGX_RADIX_TREE_H_INCLUDED_ */
