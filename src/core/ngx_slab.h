
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#جيڪڏهن_وصف_نه_ٿيل _NGX_SLAB_H_INCLUDED_
#وصف_ڪريو _NGX_SLAB_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


تعريفنوع بنية ngx_slab_صفحو_s  ngx_slab_صفحو_t;

بنية ngx_slab_صفحو_s {
    uintptr_t         slab;
    ngx_slab_صفحو_t  *ايندڙ;
    uintptr_t         prev;
};


تعريفنوع بنية {
    ngx_uint_t        ڪل;
    ngx_uint_t        استعمال_ٿيل;

    ngx_uint_t        reqs;
    ngx_uint_t        fails;
} ngx_slab_stat_t;


تعريفنوع بنية {
    ngx_shmtx_sh_t    تالو;

    سائيز_t            min_سائيز;
    سائيز_t            min_سرڪايو;

    ngx_slab_صفحو_t  *صفحا;
    ngx_slab_صفحو_t  *آخري;
    ngx_slab_صفحو_t   حرر;

    ngx_slab_stat_t  *stats;
    ngx_uint_t        pfree;

    u_char           *شروعات;
    u_char           *پڇاڙي;

    ngx_shmtx_t       mutex;

    u_char           *log_ctx;
    u_char            zero;

    بنانشان          log_nomem:1;

    فراغ             *معلومات;
    فراغ             *addr;
} ngx_slab_pool_t;


فراغ ngx_slab_sizes_init(فراغ);
فراغ ngx_slab_init(ngx_slab_pool_t *pool);
فراغ *ngx_slab_alloc(ngx_slab_pool_t *pool, سائيز_t سائيز);
فراغ *ngx_slab_alloc_locked(ngx_slab_pool_t *pool, سائيز_t سائيز);
فراغ *ngx_slab_calloc(ngx_slab_pool_t *pool, سائيز_t سائيز);
فراغ *ngx_slab_calloc_locked(ngx_slab_pool_t *pool, سائيز_t سائيز);
فراغ ngx_slab_free(ngx_slab_pool_t *pool, فراغ *p);
فراغ ngx_slab_free_locked(ngx_slab_pool_t *pool, فراغ *p);


#جيڪڏهن_پورو /* _NGX_SLAB_H_INCLUDED_ */
