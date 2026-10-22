
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#اگر_غیر_تعریف_شدہ _NGX_SLAB_H_INCLUDED_
#تعریف_کرو _NGX_SLAB_H_INCLUDED_


#شامل <ngx_config.h>
#شامل <ngx_core.h>


قسمتعریف ساخت ngx_slab_صفحہ_s  ngx_slab_صفحہ_t;

ساخت ngx_slab_صفحہ_s {
    uintptr_t         slab;
    ngx_slab_صفحہ_t  *اگلا;
    uintptr_t         پچھلا;
};


قسمتعریف ساخت {
    ngx_uint_t        کل;
    ngx_uint_t        مستعمل;

    ngx_uint_t        reqs;
    ngx_uint_t        fails;
} ngx_slab_stat_t;


قسمتعریف ساخت {
    ngx_shmtx_sh_t    تالا;

    سائز_t            min_سائز;
    سائز_t            min_سرکاؤ;

    ngx_slab_صفحہ_t  *صفحات;
    ngx_slab_صفحہ_t  *آخری;
    ngx_slab_صفحہ_t   آزاد;

    ngx_slab_stat_t  *stats;
    ngx_uint_t        pfree;

    u_char           *آغاز;
    u_char           *انتہا;

    ngx_shmtx_t       mutex;

    u_char           *log_ctx;
    u_char            zero;

    غیرعلامتی          log_nomem:1;

    خالی             *ڈیٹا;
    خالی             *addr;
} ngx_slab_pool_t;


خالی ngx_slab_sizes_init(خالی);
خالی ngx_slab_init(ngx_slab_pool_t *pool);
خالی *ngx_slab_alloc(ngx_slab_pool_t *pool, سائز_t سائز);
خالی *ngx_slab_alloc_locked(ngx_slab_pool_t *pool, سائز_t سائز);
خالی *ngx_slab_calloc(ngx_slab_pool_t *pool, سائز_t سائز);
خالی *ngx_slab_calloc_locked(ngx_slab_pool_t *pool, سائز_t سائز);
خالی ngx_slab_free(ngx_slab_pool_t *pool, خالی *p);
خالی ngx_slab_free_locked(ngx_slab_pool_t *pool, خالی *p);


#اگر_ختم /* _NGX_SLAB_H_INCLUDED_ */
