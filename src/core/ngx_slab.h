
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#اگر_تعریف_نشده _NGX_SLAB_H_INCLUDED_
#تعریف_کن _NGX_SLAB_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


تعريفنوع بنية ngx_slab_صفحه_s  ngx_slab_صفحه_t;

بنية ngx_slab_صفحه_s {
    uintptr_t         slab;
    ngx_slab_صفحه_t  *بعدی;
    uintptr_t         prev;
};


تعريفنوع بنية {
    ngx_uint_t        مجموع;
    ngx_uint_t        استفاده_شده;

    ngx_uint_t        reqs;
    ngx_uint_t        fails;
} ngx_slab_stat_t;


تعريفنوع بنية {
    ngx_shmtx_sh_t    قفل;

    اندازه_t            min_اندازه;
    اندازه_t            min_لغزش;

    ngx_slab_صفحه_t  *صفحات;
    ngx_slab_صفحه_t  *آخرین;
    ngx_slab_صفحه_t   حرر;

    ngx_slab_stat_t  *stats;
    ngx_uint_t        pfree;

    u_char           *شروع;
    u_char           *انتها;

    ngx_shmtx_t       mutex;

    u_char           *log_ctx;
    u_char            zero;

    بیعلامت          log_nomem:1;

    فراغ             *داده;
    فراغ             *addr;
} ngx_slab_pool_t;


فراغ ngx_slab_sizes_init(فراغ);
فراغ ngx_slab_init(ngx_slab_pool_t *pool);
فراغ *ngx_slab_alloc(ngx_slab_pool_t *pool, اندازه_t اندازه);
فراغ *ngx_slab_alloc_locked(ngx_slab_pool_t *pool, اندازه_t اندازه);
فراغ *ngx_slab_calloc(ngx_slab_pool_t *pool, اندازه_t اندازه);
فراغ *ngx_slab_calloc_locked(ngx_slab_pool_t *pool, اندازه_t اندازه);
فراغ ngx_slab_free(ngx_slab_pool_t *pool, فراغ *p);
فراغ ngx_slab_free_locked(ngx_slab_pool_t *pool, فراغ *p);


#پایان_اگر /* _NGX_SLAB_H_INCLUDED_ */
