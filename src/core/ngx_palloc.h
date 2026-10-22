
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#که_نه_تعریف_شوی _NGX_PALLOC_H_INCLUDED_
#تعریف_کړه _NGX_PALLOC_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


/*
 * NGX_MAX_ALLOC_FROM_POOL should be (ngx_pagesize - 1), i.e. 4095 on x86.
 * On Windows NT it decreases a number of locked pages in a kernel.
 */
#تعریف_کړه NGX_MAX_ALLOC_FROM_POOL  (ngx_pagesize - 1)

#تعریف_کړه NGX_DEFAULT_POOL_اندازه    (16 * 1024)

#تعریف_کړه NGX_POOL_ALIGNMENT       16
#تعریف_کړه NGX_MIN_POOL_اندازه                                                     \
    ngx_align((حجم(ngx_pool_t) + 2 * حجم(ngx_pool_large_t)),            \
              NGX_POOL_ALIGNMENT)


تعريفنوع فراغ (*ngx_pool_cleanup_pt)(فراغ *معلومات);

تعريفنوع بنية ngx_pool_cleanup_s  ngx_pool_cleanup_t;

بنية ngx_pool_cleanup_s {
    ngx_pool_cleanup_pt   handler;
    فراغ                 *معلومات;
    ngx_pool_cleanup_t   *بل;
};


تعريفنوع بنية ngx_pool_large_s  ngx_pool_large_t;

بنية ngx_pool_large_s {
    ngx_pool_large_t     *بل;
    فراغ                 *alloc;
};


تعريفنوع بنية {
    u_char               *وروستی;
    u_char               *پای_ټکی;
    ngx_pool_t           *بل;
    ngx_uint_t            failed;
} ngx_pool_معلومات_t;


بنية ngx_pool_s {
    ngx_pool_معلومات_t       d;
    اندازه_t                max;
    ngx_pool_t           *current;
    ngx_chain_t          *chain;
    ngx_pool_large_t     *large;
    ngx_pool_cleanup_t   *cleanup;
    ngx_log_t            *لوګاریتم;
};


تعريفنوع بنية {
    ngx_fd_t              fd;
    u_char               *name;
    ngx_log_t            *لوګاریتم;
} ngx_pool_cleanup_file_t;


ngx_pool_t *ngx_create_pool(اندازه_t اندازه, ngx_log_t *لوګاریتم);
فراغ ngx_destroy_pool(ngx_pool_t *pool);
فراغ ngx_reset_pool(ngx_pool_t *pool);

فراغ *ngx_palloc(ngx_pool_t *pool, اندازه_t اندازه);
فراغ *ngx_pnalloc(ngx_pool_t *pool, اندازه_t اندازه);
فراغ *ngx_pcalloc(ngx_pool_t *pool, اندازه_t اندازه);
فراغ *ngx_pmemalign(ngx_pool_t *pool, اندازه_t اندازه, اندازه_t alignment);
ngx_int_t ngx_pfree(ngx_pool_t *pool, فراغ *p);


ngx_pool_cleanup_t *ngx_pool_cleanup_جمع_کړه(ngx_pool_t *p, اندازه_t اندازه);
فراغ ngx_pool_پرلپسې_cleanup_file(ngx_pool_t *p, ngx_fd_t fd);
فراغ ngx_pool_cleanup_file(فراغ *معلومات);
فراغ ngx_pool_delete_file(فراغ *معلومات);


#د_که_پای /* _NGX_PALLOC_H_INCLUDED_ */
