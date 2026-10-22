
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#నిర్వచితం_కాకపోతే _NGX_SLAB_H_INCLUDED_
#నిర్వచించు _NGX_SLAB_H_INCLUDED_


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>


రకనిర్వచనం నిర్మాణం ngx_slab_పుట_s  ngx_slab_పుట_t;

నిర్మాణం ngx_slab_పుట_s {
    uintptr_t         slab;
    ngx_slab_పుట_t  *తదుపరి;
    uintptr_t         prev;
};


రకనిర్వచనం నిర్మాణం {
    ngx_uint_t        మొత్తం;
    ngx_uint_t        వాడిన;

    ngx_uint_t        reqs;
    ngx_uint_t        fails;
} ngx_slab_stat_t;


రకనిర్వచనం నిర్మాణం {
    ngx_shmtx_sh_t    తాళం;

    కొలత_t            min_కొలత;
    కొలత_t            min_జరుపు;

    ngx_slab_పుట_t  *పుటలు;
    ngx_slab_పుట_t  *చివరి;
    ngx_slab_పుట_t   విడుదల;

    ngx_slab_stat_t  *stats;
    ngx_uint_t        pfree;

    u_char           *ప్రారంభం;
    u_char           *చివర;

    ngx_shmtx_t       mutex;

    u_char           *log_ctx;
    u_char            zero;

    గుర్తులేని          log_nomem:1;

    శూన్య             *దత్తాంశం;
    శూన్య             *addr;
} ngx_slab_pool_t;


శూన్య ngx_slab_sizes_init(శూన్య);
శూన్య ngx_slab_init(ngx_slab_pool_t *pool);
శూన్య *ngx_slab_alloc(ngx_slab_pool_t *pool, కొలత_t కొలత);
శూన్య *ngx_slab_alloc_locked(ngx_slab_pool_t *pool, కొలత_t కొలత);
శూన్య *ngx_slab_calloc(ngx_slab_pool_t *pool, కొలత_t కొలత);
శూన్య *ngx_slab_calloc_locked(ngx_slab_pool_t *pool, కొలత_t కొలత);
శూన్య ngx_slab_free(ngx_slab_pool_t *pool, శూన్య *p);
శూన్య ngx_slab_free_locked(ngx_slab_pool_t *pool, శూన్య *p);


#అయితే_ముగింపు /* _NGX_SLAB_H_INCLUDED_ */
