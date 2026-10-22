
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_SLAB_H_INCLUDED_
#define _NGX_SLAB_H_INCLUDED_


#অন্তর্ভাব <ngx_config.h>
#অন্তর্ভাব <ngx_core.h>


প্রকারনির্দেশ সংরচনা ngx_slab_page_s  ngx_slab_page_t;

সংরচনা ngx_slab_page_s {
    uintptr_t         slab;
    ngx_slab_page_t  *next;
    uintptr_t         prev;
};


প্রকারনির্দেশ সংরচনা {
    ngx_uint_t        total;
    ngx_uint_t        used;

    ngx_uint_t        reqs;
    ngx_uint_t        fails;
} ngx_slab_stat_t;


প্রকারনির্দেশ সংরচনা {
    ngx_shmtx_sh_t    lock;

    size_t            min_size;
    size_t            min_shift;

    ngx_slab_page_t  *pages;
    ngx_slab_page_t  *last;
    ngx_slab_page_t   মুক্ত;

    ngx_slab_stat_t  *stats;
    ngx_uint_t        pfree;

    u_char           *start;
    u_char           *end;

    ngx_shmtx_t       mutex;

    u_char           *log_ctx;
    u_char            zero;

    unsigned          log_nomem:1;

    শূন্য             *data;
    শূন্য             *addr;
} ngx_slab_pool_t;


শূন্য ngx_slab_sizes_init(শূন্য);
শূন্য ngx_slab_init(ngx_slab_pool_t *pool);
শূন্য *ngx_slab_alloc(ngx_slab_pool_t *pool, size_t size);
শূন্য *ngx_slab_alloc_locked(ngx_slab_pool_t *pool, size_t size);
শূন্য *ngx_slab_calloc(ngx_slab_pool_t *pool, size_t size);
শূন্য *ngx_slab_calloc_locked(ngx_slab_pool_t *pool, size_t size);
শূন্য ngx_slab_free(ngx_slab_pool_t *pool, শূন্য *p);
শূন্য ngx_slab_free_locked(ngx_slab_pool_t *pool, শূন্য *p);


#endif /* _NGX_SLAB_H_INCLUDED_ */
