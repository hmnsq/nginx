
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_SLAB_H_INCLUDED_
#define _NGX_SLAB_H_INCLUDED_


#ଅନ୍ତର୍ଭାଵ <ngx_config.h>
#ଅନ୍ତର୍ଭାଵ <ngx_core.h>


ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା ngx_slab_page_s  ngx_slab_page_t;

ସଂରଚନା ngx_slab_page_s {
    uintptr_t         slab;
    ngx_slab_page_t  *next;
    uintptr_t         prev;
};


ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ngx_uint_t        total;
    ngx_uint_t        used;

    ngx_uint_t        reqs;
    ngx_uint_t        fails;
} ngx_slab_stat_t;


ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ngx_shmtx_sh_t    lock;

    size_t            min_size;
    size_t            min_shift;

    ngx_slab_page_t  *pages;
    ngx_slab_page_t  *last;
    ngx_slab_page_t   ମୁକ୍ତ;

    ngx_slab_stat_t  *stats;
    ngx_uint_t        pfree;

    u_char           *start;
    u_char           *end;

    ngx_shmtx_t       mutex;

    u_char           *log_ctx;
    u_char            zero;

    unsigned          log_nomem:1;

    ଶୂନ୍ୟ             *data;
    ଶୂନ୍ୟ             *addr;
} ngx_slab_pool_t;


ଶୂନ୍ୟ ngx_slab_sizes_init(ଶୂନ୍ୟ);
ଶୂନ୍ୟ ngx_slab_init(ngx_slab_pool_t *pool);
ଶୂନ୍ୟ *ngx_slab_alloc(ngx_slab_pool_t *pool, size_t size);
ଶୂନ୍ୟ *ngx_slab_alloc_locked(ngx_slab_pool_t *pool, size_t size);
ଶୂନ୍ୟ *ngx_slab_calloc(ngx_slab_pool_t *pool, size_t size);
ଶୂନ୍ୟ *ngx_slab_calloc_locked(ngx_slab_pool_t *pool, size_t size);
ଶୂନ୍ୟ ngx_slab_free(ngx_slab_pool_t *pool, ଶୂନ୍ୟ *p);
ଶୂନ୍ୟ ngx_slab_free_locked(ngx_slab_pool_t *pool, ଶୂନ୍ୟ *p);


#endif /* _NGX_SLAB_H_INCLUDED_ */
