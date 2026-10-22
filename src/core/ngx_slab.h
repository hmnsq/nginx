
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_SLAB_H_INCLUDED_
#define _NGX_SLAB_H_INCLUDED_


#অন্তর্ভাব <ngx_config.h>
#অন্তর্ভাব <ngx_core.h>


প্রকারলেখ কাঠামো ngx_slab_page_s  ngx_slab_page_t;

কাঠামো ngx_slab_page_s {
    uintptr_t         slab;
    ngx_slab_page_t  *পরবর্তী;
    uintptr_t         পূর্ব;
};


প্রকারলেখ কাঠামো {
    ngx_uint_t        যোগ;
    ngx_uint_t        used;

    ngx_uint_t        reqs;
    ngx_uint_t        fails;
} ngx_slab_stat_t;


প্রকারলেখ কাঠামো {
    ngx_shmtx_sh_t    lock;

    আকার_t            min_আকার;
    আকার_t            min_shift;

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

    শূন্য             *তথ্য;
    শূন্য             *addr;
} ngx_slab_pool_t;


শূন্য ngx_slab_sizes_init(শূন্য);
শূন্য ngx_slab_init(ngx_slab_pool_t *pool);
শূন্য *ngx_slab_alloc(ngx_slab_pool_t *pool, আকার_t আকার_2);
শূন্য *ngx_slab_alloc_locked(ngx_slab_pool_t *pool, আকার_t আকার_2);
শূন্য *ngx_slab_calloc(ngx_slab_pool_t *pool, আকার_t আকার_2);
শূন্য *ngx_slab_calloc_locked(ngx_slab_pool_t *pool, আকার_t আকার_2);
শূন্য ngx_slab_free(ngx_slab_pool_t *pool, শূন্য *p);
শূন্য ngx_slab_free_locked(ngx_slab_pool_t *pool, শূন্য *p);


#endif /* _NGX_SLAB_H_INCLUDED_ */
