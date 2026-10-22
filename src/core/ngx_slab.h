
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_SLAB_H_INCLUDED_
#define _NGX_SLAB_H_INCLUDED_


#شامل <ngx_config.h>
#شامل <ngx_core.h>


تعریفنوع ساختار ngx_slab_page_s  ngx_slab_page_t;

ساختار ngx_slab_page_s {
    uintptr_t         slab;
    ngx_slab_page_t  *next;
    uintptr_t         prev;
};


تعریفنوع ساختار {
    ngx_uint_t        total;
    ngx_uint_t        used;

    ngx_uint_t        reqs;
    ngx_uint_t        fails;
} ngx_slab_stat_t;


تعریفنوع ساختار {
    ngx_shmtx_sh_t    lock;

    size_t            min_size;
    size_t            min_shift;

    ngx_slab_page_t  *pages;
    ngx_slab_page_t  *last;
    ngx_slab_page_t   آزاد;

    ngx_slab_stat_t  *stats;
    ngx_uint_t        pfree;

    u_char           *start;
    u_char           *end;

    ngx_shmtx_t       mutex;

    u_char           *log_ctx;
    u_char            zero;

    unsigned          log_nomem:1;

    تهی             *data;
    تهی             *addr;
} ngx_slab_pool_t;


تهی ngx_slab_sizes_init(تهی);
تهی ngx_slab_init(ngx_slab_pool_t *pool);
تهی *ngx_slab_alloc(ngx_slab_pool_t *pool, size_t size);
تهی *ngx_slab_alloc_locked(ngx_slab_pool_t *pool, size_t size);
تهی *ngx_slab_calloc(ngx_slab_pool_t *pool, size_t size);
تهی *ngx_slab_calloc_locked(ngx_slab_pool_t *pool, size_t size);
تهی ngx_slab_free(ngx_slab_pool_t *pool, تهی *p);
تهی ngx_slab_free_locked(ngx_slab_pool_t *pool, تهی *p);


#endif /* _NGX_SLAB_H_INCLUDED_ */
