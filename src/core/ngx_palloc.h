
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_PALLOC_H_INCLUDED_
#define _NGX_PALLOC_H_INCLUDED_


#অন্তর্ভাৱ <ngx_config.h>
#অন্তর্ভাৱ <ngx_core.h>


/*
 * NGX_MAX_ALLOC_FROM_POOL should be (ngx_pagesize - 1), i.e. 4095 on x86.
 * On Windows NT it decreases a number of locked pages in a kernel.
 */
#define NGX_MAX_ALLOC_FROM_POOL  (ngx_pagesize - 1)

#define NGX_DEFAULT_POOL_SIZE    (16 * 1024)

#define NGX_POOL_ALIGNMENT       16
#define NGX_MIN_POOL_SIZE                                                     \
    ngx_align((পরিমাণ(ngx_pool_t) + 2 * পরিমাণ(ngx_pool_large_t)),            \
              NGX_POOL_ALIGNMENT)


প্রকারনির্দেশ শূন্য (*ngx_pool_cleanup_pt)(শূন্য *data);

প্রকারনির্দেশ সংরচনা ngx_pool_cleanup_s  ngx_pool_cleanup_t;

সংরচনা ngx_pool_cleanup_s {
    ngx_pool_cleanup_pt   handler;
    শূন্য                 *data;
    ngx_pool_cleanup_t   *next;
};


প্রকারনির্দেশ সংরচনা ngx_pool_large_s  ngx_pool_large_t;

সংরচনা ngx_pool_large_s {
    ngx_pool_large_t     *next;
    শূন্য                 *alloc;
};


প্রকারনির্দেশ সংরচনা {
    u_char               *last;
    u_char               *end;
    ngx_pool_t           *next;
    ngx_uint_t            failed;
} ngx_pool_data_t;


সংরচনা ngx_pool_s {
    ngx_pool_data_t       d;
    size_t                max;
    ngx_pool_t           *current;
    ngx_chain_t          *chain;
    ngx_pool_large_t     *large;
    ngx_pool_cleanup_t   *cleanup;
    ngx_log_t            *log;
};


প্রকারনির্দেশ সংরচনা {
    ngx_fd_t              fd;
    u_char               *name;
    ngx_log_t            *log;
} ngx_pool_cleanup_file_t;


ngx_pool_t *ngx_create_pool(size_t size, ngx_log_t *log);
শূন্য ngx_destroy_pool(ngx_pool_t *pool);
শূন্য ngx_reset_pool(ngx_pool_t *pool);

শূন্য *ngx_palloc(ngx_pool_t *pool, size_t size);
শূন্য *ngx_pnalloc(ngx_pool_t *pool, size_t size);
শূন্য *ngx_pcalloc(ngx_pool_t *pool, size_t size);
শূন্য *ngx_pmemalign(ngx_pool_t *pool, size_t size, size_t alignment);
ngx_int_t ngx_pfree(ngx_pool_t *pool, শূন্য *p);


ngx_pool_cleanup_t *ngx_pool_cleanup_add(ngx_pool_t *p, size_t size);
শূন্য ngx_pool_run_cleanup_file(ngx_pool_t *p, ngx_fd_t fd);
শূন্য ngx_pool_cleanup_file(শূন্য *data);
শূন্য ngx_pool_delete_file(শূন্য *data);


#endif /* _NGX_PALLOC_H_INCLUDED_ */
