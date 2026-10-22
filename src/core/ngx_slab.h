
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_SLAB_H_INCLUDED_
#define _NGX_SLAB_H_INCLUDED_


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


प्रकारनिर्देश संरचना ngx_slab_page_s  ngx_slab_page_t;

संरचना ngx_slab_page_s {
    uintptr_t         slab;
    ngx_slab_page_t  *next;
    uintptr_t         prev;
};


प्रकारनिर्देश संरचना {
    ngx_uint_t        total;
    ngx_uint_t        used;

    ngx_uint_t        reqs;
    ngx_uint_t        fails;
} ngx_slab_stat_t;


प्रकारनिर्देश संरचना {
    ngx_shmtx_sh_t    lock;

    माप_प्रकार            min_size;
    माप_प्रकार            min_shift;

    ngx_slab_page_t  *pages;
    ngx_slab_page_t  *last;
    ngx_slab_page_t   मुक्त;

    ngx_slab_stat_t  *stats;
    ngx_uint_t        pfree;

    u_char           *start;
    u_char           *end;

    ngx_shmtx_t       mutex;

    u_char           *log_ctx;
    u_char            zero;

    अचिन्हित          log_nomem:1;

    शून्य             *data;
    शून्य             *addr;
} ngx_slab_pool_t;


शून्य ngx_slab_sizes_init(शून्य);
शून्य ngx_slab_init(ngx_slab_pool_t *pool);
शून्य *ngx_slab_alloc(ngx_slab_pool_t *pool, माप_प्रकार size);
शून्य *ngx_slab_alloc_locked(ngx_slab_pool_t *pool, माप_प्रकार size);
शून्य *ngx_slab_calloc(ngx_slab_pool_t *pool, माप_प्रकार size);
शून्य *ngx_slab_calloc_locked(ngx_slab_pool_t *pool, माप_प्रकार size);
शून्य ngx_slab_free(ngx_slab_pool_t *pool, शून्य *p);
शून्य ngx_slab_free_locked(ngx_slab_pool_t *pool, शून्य *p);


#endif /* _NGX_SLAB_H_INCLUDED_ */
