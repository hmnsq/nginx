
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ _NGX_SLAB_H_INCLUDED_
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _NGX_SLAB_H_INCLUDED_


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ngx_slab_ਪੰਨਾ_s  ngx_slab_ਪੰਨਾ_t;

ਬਣਤਰ ngx_slab_ਪੰਨਾ_s {
    uintptr_t         slab;
    ngx_slab_ਪੰਨਾ_t  *ਅਗਲਾ;
    uintptr_t         prev;
};


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ngx_uint_t        ਕੁੱਲ;
    ngx_uint_t        ਵਰਤਿਆ;

    ngx_uint_t        reqs;
    ngx_uint_t        fails;
} ngx_slab_stat_t;


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ngx_shmtx_sh_t    ਤਾਲਾ;

    ਮਾਪ_t            min_ਮਾਪ;
    ਮਾਪ_t            min_ਖਿਸਕਾਓ;

    ngx_slab_ਪੰਨਾ_t  *ਪੰਨੇ;
    ngx_slab_ਪੰਨਾ_t  *ਆਖਰੀ;
    ngx_slab_ਪੰਨਾ_t   ਮੁਕਤ;

    ngx_slab_stat_t  *stats;
    ngx_uint_t        pfree;

    u_char           *ਸ਼ੁਰੂ;
    u_char           *ਸਿਰਾ;

    ngx_shmtx_t       mutex;

    u_char           *log_ctx;
    u_char            zero;

    ਬਿਨਾਨਿਸ਼ਾਨੀ          log_nomem:1;

    ਖਾਲੀ             *ਅੰਕੜੇ;
    ਖਾਲੀ             *addr;
} ngx_slab_pool_t;


ਖਾਲੀ ngx_slab_sizes_init(ਖਾਲੀ);
ਖਾਲੀ ngx_slab_init(ngx_slab_pool_t *pool);
ਖਾਲੀ *ngx_slab_alloc(ngx_slab_pool_t *pool, ਮਾਪ_t ਮਾਪ);
ਖਾਲੀ *ngx_slab_alloc_locked(ngx_slab_pool_t *pool, ਮਾਪ_t ਮਾਪ);
ਖਾਲੀ *ngx_slab_calloc(ngx_slab_pool_t *pool, ਮਾਪ_t ਮਾਪ);
ਖਾਲੀ *ngx_slab_calloc_locked(ngx_slab_pool_t *pool, ਮਾਪ_t ਮਾਪ);
ਖਾਲੀ ngx_slab_free(ngx_slab_pool_t *pool, ਖਾਲੀ *p);
ਖਾਲੀ ngx_slab_free_locked(ngx_slab_pool_t *pool, ਖਾਲੀ *p);


#ਜੇ_ਅੰਤ /* _NGX_SLAB_H_INCLUDED_ */
