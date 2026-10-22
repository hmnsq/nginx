
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अगर_अघोषित _NGX_SLAB_H_INCLUDED_
#घोषणा _NGX_SLAB_H_INCLUDED_


#समावेश <ngx_config.h>
#समावेश <ngx_core.h>


प्रकार काष्ठा ngx_slab_page_s  ngx_slab_page_t;

काष्ठा ngx_slab_page_s {
    uintptr_t         slab;
    ngx_slab_page_t  *अग्र;
    uintptr_t         पूर्व;
};


प्रकार काष्ठा {
    ngx_uint_t        योग;
    ngx_uint_t        used;

    ngx_uint_t        reqs;
    ngx_uint_t        fails;
} ngx_slab_stat_t;


प्रकार काष्ठा {
    ngx_shmtx_sh_t    lock;

    माप_प्रकार            min_आकार;
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

    व्योम             *तथ्य;
    व्योम             *addr;
} ngx_slab_pool_t;


व्योम ngx_slab_sizes_init(व्योम);
व्योम ngx_slab_init(ngx_slab_pool_t *pool);
व्योम *ngx_slab_alloc(ngx_slab_pool_t *pool, माप_प्रकार आकार);
व्योम *ngx_slab_alloc_locked(ngx_slab_pool_t *pool, माप_प्रकार आकार);
व्योम *ngx_slab_calloc(ngx_slab_pool_t *pool, माप_प्रकार आकार);
व्योम *ngx_slab_calloc_locked(ngx_slab_pool_t *pool, माप_प्रकार आकार);
व्योम ngx_slab_free(ngx_slab_pool_t *pool, व्योम *p);
व्योम ngx_slab_free_locked(ngx_slab_pool_t *pool, व्योम *p);


#पूर्ण_अगर /* _NGX_SLAB_H_INCLUDED_ */
