
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#اگر_تعریف_نشده _NGX_PALLOC_H_INCLUDED_
#تعریف_کن _NGX_PALLOC_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


/*
 * NGX_MAX_ALLOC_FROM_POOL should be (ngx_pagesize - 1), i.e. 4095 on x86.
 * On Windows NT it decreases a number of locked pages in a kernel.
 */
#تعریف_کن NGX_MAX_ALLOC_FROM_POOL  (ngx_pagesize - 1)

#تعریف_کن NGX_DEFAULT_POOL_اندازه    (16 * 1024)

#تعریف_کن NGX_POOL_ALIGNMENT       16
#تعریف_کن NGX_MIN_POOL_اندازه                                                     \
    ngx_align((حجم(ngx_pool_t) + 2 * حجم(ngx_pool_large_t)),            \
              NGX_POOL_ALIGNMENT)


تعريفنوع فراغ (*ngx_pool_cleanup_pt)(فراغ *داده);

تعريفنوع بنية ngx_pool_cleanup_s  ngx_pool_cleanup_t;

بنية ngx_pool_cleanup_s {
    ngx_pool_cleanup_pt   handler;
    فراغ                 *داده;
    ngx_pool_cleanup_t   *بعدی;
};


تعريفنوع بنية ngx_pool_large_s  ngx_pool_large_t;

بنية ngx_pool_large_s {
    ngx_pool_large_t     *بعدی;
    فراغ                 *alloc;
};


تعريفنوع بنية {
    u_char               *آخرین;
    u_char               *انتها;
    ngx_pool_t           *بعدی;
    ngx_uint_t            failed;
} ngx_pool_داده_t;


بنية ngx_pool_s {
    ngx_pool_داده_t       d;
    اندازه_t                max;
    ngx_pool_t           *current;
    ngx_chain_t          *chain;
    ngx_pool_large_t     *large;
    ngx_pool_cleanup_t   *cleanup;
    ngx_log_t            *لگاریتم;
};


تعريفنوع بنية {
    ngx_fd_t              fd;
    u_char               *name;
    ngx_log_t            *لگاریتم;
} ngx_pool_cleanup_file_t;


ngx_pool_t *ngx_create_pool(اندازه_t اندازه, ngx_log_t *لگاریتم);
فراغ ngx_destroy_pool(ngx_pool_t *pool);
فراغ ngx_reset_pool(ngx_pool_t *pool);

فراغ *ngx_palloc(ngx_pool_t *pool, اندازه_t اندازه);
فراغ *ngx_pnalloc(ngx_pool_t *pool, اندازه_t اندازه);
فراغ *ngx_pcalloc(ngx_pool_t *pool, اندازه_t اندازه);
فراغ *ngx_pmemalign(ngx_pool_t *pool, اندازه_t اندازه, اندازه_t alignment);
ngx_int_t ngx_pfree(ngx_pool_t *pool, فراغ *p);


ngx_pool_cleanup_t *ngx_pool_cleanup_جمع_کن(ngx_pool_t *p, اندازه_t اندازه);
فراغ ngx_pool_پیاپی_cleanup_file(ngx_pool_t *p, ngx_fd_t fd);
فراغ ngx_pool_cleanup_file(فراغ *داده);
فراغ ngx_pool_delete_file(فراغ *داده);


#پایان_اگر /* _NGX_PALLOC_H_INCLUDED_ */
