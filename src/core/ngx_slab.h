
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#که_نه_تعریف_شوی _NGX_SLAB_H_INCLUDED_
#تعریف_کړه _NGX_SLAB_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


تعريفنوع بنية ngx_slab_پاڼه_s  ngx_slab_پاڼه_t;

بنية ngx_slab_پاڼه_s {
    uintptr_t         slab;
    ngx_slab_پاڼه_t  *بل;
    uintptr_t         prev;
};


تعريفنوع بنية {
    ngx_uint_t        ټول;
    ngx_uint_t        کارول_شوی;

    ngx_uint_t        reqs;
    ngx_uint_t        fails;
} ngx_slab_stat_t;


تعريفنوع بنية {
    ngx_shmtx_sh_t    قلف;

    اندازه_t            min_اندازه;
    اندازه_t            min_ښويدنه;

    ngx_slab_پاڼه_t  *پاڼې;
    ngx_slab_پاڼه_t  *وروستی;
    ngx_slab_پاڼه_t   حرر;

    ngx_slab_stat_t  *stats;
    ngx_uint_t        pfree;

    u_char           *پیل;
    u_char           *پای_ټکی;

    ngx_shmtx_t       mutex;

    u_char           *log_ctx;
    u_char            zero;

    بینښه          log_nomem:1;

    فراغ             *معلومات;
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


#د_که_پای /* _NGX_SLAB_H_INCLUDED_ */
