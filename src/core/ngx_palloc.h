
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#నిర్వచితం_కాకపోతే _NGX_PALLOC_H_INCLUDED_
#నిర్వచించు _NGX_PALLOC_H_INCLUDED_


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>


/*
 * NGX_MAX_ALLOC_FROM_POOL should be (ngx_pagesize - 1), i.e. 4095 on x86.
 * On Windows NT it decreases a number of locked pages in a kernel.
 */
#నిర్వచించు NGX_MAX_ALLOC_FROM_POOL  (ngx_pagesize - 1)

#నిర్వచించు NGX_DEFAULT_POOL_కొలత    (16 * 1024)

#నిర్వచించు NGX_POOL_ALIGNMENT       16
#నిర్వచించు NGX_MIN_POOL_కొలత                                                     \
    ngx_align((పరిమాణం(ngx_pool_t) + 2 * పరిమాణం(ngx_pool_large_t)),            \
              NGX_POOL_ALIGNMENT)


రకనిర్వచనం శూన్య (*ngx_pool_cleanup_pt)(శూన్య *దత్తాంశం);

రకనిర్వచనం నిర్మాణం ngx_pool_cleanup_s  ngx_pool_cleanup_t;

నిర్మాణం ngx_pool_cleanup_s {
    ngx_pool_cleanup_pt   handler;
    శూన్య                 *దత్తాంశం;
    ngx_pool_cleanup_t   *తదుపరి;
};


రకనిర్వచనం నిర్మాణం ngx_pool_large_s  ngx_pool_large_t;

నిర్మాణం ngx_pool_large_s {
    ngx_pool_large_t     *తదుపరి;
    శూన్య                 *alloc;
};


రకనిర్వచనం నిర్మాణం {
    u_char               *చివరి;
    u_char               *చివర;
    ngx_pool_t           *తదుపరి;
    ngx_uint_t            failed;
} ngx_pool_దత్తాంశం_t;


నిర్మాణం ngx_pool_s {
    ngx_pool_దత్తాంశం_t       d;
    కొలత_t                max;
    ngx_pool_t           *current;
    ngx_chain_t          *chain;
    ngx_pool_large_t     *large;
    ngx_pool_cleanup_t   *cleanup;
    ngx_log_t            *సంవర్గమానం;
};


రకనిర్వచనం నిర్మాణం {
    ngx_fd_t              fd;
    u_char               *name;
    ngx_log_t            *సంవర్గమానం;
} ngx_pool_cleanup_file_t;


ngx_pool_t *ngx_create_pool(కొలత_t కొలత, ngx_log_t *సంవర్గమానం);
శూన్య ngx_destroy_pool(ngx_pool_t *pool);
శూన్య ngx_reset_pool(ngx_pool_t *pool);

శూన్య *ngx_palloc(ngx_pool_t *pool, కొలత_t కొలత);
శూన్య *ngx_pnalloc(ngx_pool_t *pool, కొలత_t కొలత);
శూన్య *ngx_pcalloc(ngx_pool_t *pool, కొలత_t కొలత);
శూన్య *ngx_pmemalign(ngx_pool_t *pool, కొలత_t కొలత, కొలత_t alignment);
ngx_int_t ngx_pfree(ngx_pool_t *pool, శూన్య *p);


ngx_pool_cleanup_t *ngx_pool_cleanup_కూడు(ngx_pool_t *p, కొలత_t కొలత);
శూన్య ngx_pool_నిరంతరం_cleanup_file(ngx_pool_t *p, ngx_fd_t fd);
శూన్య ngx_pool_cleanup_file(శూన్య *దత్తాంశం);
శూన్య ngx_pool_delete_file(శూన్య *దత్తాంశం);


#అయితే_ముగింపు /* _NGX_PALLOC_H_INCLUDED_ */
