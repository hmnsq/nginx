
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#جے_بنا_تعریف _NGX_SLAB_H_INCLUDED_
#تعریف_کرو _NGX_SLAB_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


تعريفنوع بنية ngx_slab_ورقہ_s  ngx_slab_ورقہ_t;

بنية ngx_slab_ورقہ_s {
    uintptr_t         slab;
    ngx_slab_ورقہ_t  *اگلا;
    uintptr_t         prev;
};


تعريفنوع بنية {
    ngx_uint_t        کل;
    ngx_uint_t        ورتیا;

    ngx_uint_t        reqs;
    ngx_uint_t        fails;
} ngx_slab_stat_t;


تعريفنوع بنية {
    ngx_shmtx_sh_t    جندرا;

    ناپ_t            min_ناپ;
    ناپ_t            min_کھسکاؤ;

    ngx_slab_ورقہ_t  *ورقے;
    ngx_slab_ورقہ_t  *آخری;
    ngx_slab_ورقہ_t   حرر;

    ngx_slab_stat_t  *stats;
    ngx_uint_t        pfree;

    u_char           *شروع;
    u_char           *سرا;

    ngx_shmtx_t       mutex;

    u_char           *log_ctx;
    u_char            zero;

    بنانشانی          log_nomem:1;

    فراغ             *انکڑے;
    فراغ             *addr;
} ngx_slab_pool_t;


فراغ ngx_slab_sizes_init(فراغ);
فراغ ngx_slab_init(ngx_slab_pool_t *pool);
فراغ *ngx_slab_alloc(ngx_slab_pool_t *pool, ناپ_t ناپ);
فراغ *ngx_slab_alloc_locked(ngx_slab_pool_t *pool, ناپ_t ناپ);
فراغ *ngx_slab_calloc(ngx_slab_pool_t *pool, ناپ_t ناپ);
فراغ *ngx_slab_calloc_locked(ngx_slab_pool_t *pool, ناپ_t ناپ);
فراغ ngx_slab_free(ngx_slab_pool_t *pool, فراغ *p);
فراغ ngx_slab_free_locked(ngx_slab_pool_t *pool, فراغ *p);


#جے_انت /* _NGX_SLAB_H_INCLUDED_ */
