
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_SLAB_H_INCLUDED_
#define _NGX_SLAB_H_INCLUDED_


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ ngx_slab_page_s  ngx_slab_page_t;

ಸಂರಚನಾ ngx_slab_page_s {
    uintptr_t         slab;
    ngx_slab_page_t  *next;
    uintptr_t         prev;
};


ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    ngx_uint_t        total;
    ngx_uint_t        used;

    ngx_uint_t        reqs;
    ngx_uint_t        fails;
} ngx_slab_stat_t;


ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    ngx_shmtx_sh_t    lock;

    size_t            min_size;
    size_t            min_shift;

    ngx_slab_page_t  *pages;
    ngx_slab_page_t  *last;
    ngx_slab_page_t   ಮುಕ್ತ;

    ngx_slab_stat_t  *stats;
    ngx_uint_t        pfree;

    u_char           *start;
    u_char           *end;

    ngx_shmtx_t       mutex;

    u_char           *log_ctx;
    u_char            zero;

    unsigned          log_nomem:1;

    ಶೂನ್ಯ             *data;
    ಶೂನ್ಯ             *addr;
} ngx_slab_pool_t;


ಶೂನ್ಯ ngx_slab_sizes_init(ಶೂನ್ಯ);
ಶೂನ್ಯ ngx_slab_init(ngx_slab_pool_t *pool);
ಶೂನ್ಯ *ngx_slab_alloc(ngx_slab_pool_t *pool, size_t size);
ಶೂನ್ಯ *ngx_slab_alloc_locked(ngx_slab_pool_t *pool, size_t size);
ಶೂನ್ಯ *ngx_slab_calloc(ngx_slab_pool_t *pool, size_t size);
ಶೂನ್ಯ *ngx_slab_calloc_locked(ngx_slab_pool_t *pool, size_t size);
ಶೂನ್ಯ ngx_slab_free(ngx_slab_pool_t *pool, ಶೂನ್ಯ *p);
ಶೂನ್ಯ ngx_slab_free_locked(ngx_slab_pool_t *pool, ಶೂನ್ಯ *p);


#endif /* _NGX_SLAB_H_INCLUDED_ */
