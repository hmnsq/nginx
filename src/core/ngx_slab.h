
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#यदि_अपरिभाषितम् _NGX_SLAB_H_INCLUDED_
#परिभाषय _NGX_SLAB_H_INCLUDED_


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


प्रकारनिर्देश संरचना ngx_slab_page_s  ngx_slab_page_t;

संरचना ngx_slab_page_s {
    uintptr_t         slab;
    ngx_slab_page_t  *अग्र;
    uintptr_t         पूर्व;
};


प्रकारनिर्देश संरचना {
    ngx_uint_t        योग;
    ngx_uint_t        प्रयुक्तम्;

    ngx_uint_t        reqs;
    ngx_uint_t        fails;
} ngx_slab_stat_t;


प्रकारनिर्देश संरचना {
    ngx_shmtx_sh_t    तालकम्;

    आकार_t            min_आकार;
    आकार_t            min_सरणम्;

    ngx_slab_page_t  *पृष्ठानि;
    ngx_slab_page_t  *चरमः;
    ngx_slab_page_t   मुक्त;

    ngx_slab_stat_t  *stats;
    ngx_uint_t        pfree;

    u_char           *start;
    u_char           *अन्तः;

    ngx_shmtx_t       mutex;

    u_char           *log_ctx;
    u_char            zero;

    अचिह्नित          log_nomem:1;

    शून्य             *दत्त;
    शून्य             *addr;
} ngx_slab_pool_t;


शून्य ngx_slab_sizes_init(शून्य);
शून्य ngx_slab_init(ngx_slab_pool_t *pool);
शून्य *ngx_slab_alloc(ngx_slab_pool_t *pool, आकार_t आकार);
शून्य *ngx_slab_alloc_locked(ngx_slab_pool_t *pool, आकार_t आकार);
शून्य *ngx_slab_calloc(ngx_slab_pool_t *pool, आकार_t आकार);
शून्य *ngx_slab_calloc_locked(ngx_slab_pool_t *pool, आकार_t आकार);
शून्य ngx_slab_free(ngx_slab_pool_t *pool, शून्य *p);
शून्य ngx_slab_free_locked(ngx_slab_pool_t *pool, शून्य *p);


#यदि_अन्तः /* _NGX_SLAB_H_INCLUDED_ */
