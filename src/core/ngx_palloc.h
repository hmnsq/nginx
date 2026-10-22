
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_PALLOC_H_INCLUDED_
#define _NGX_PALLOC_H_INCLUDED_


#অন্তর্ভাব <ngx_config.h>
#অন্তর্ভাব <ngx_core.h>


/*
 * NGX_MAX_ALLOC_FROM_POOL should be (ngx_pagesize - 1), i.e. 4095 on x86.
 * On Windows NT it decreases a number of locked pages in a kernel.
 */
#define NGX_MAX_ALLOC_FROM_POOL  (ngx_pagesize - 1)

#define NGX_DEFAULT_POOL_আকার    (16 * 1024)

#define NGX_POOL_ALIGNMENT       16
#define NGX_MIN_POOL_আকার                                                     \
    ngx_align((আকার(ngx_pool_t) + 2 * আকার(ngx_pool_large_t)),            \
              NGX_POOL_ALIGNMENT)


প্রকারলেখ শূন্য (*ngx_pool_cleanup_pt)(শূন্য *তথ্য);

প্রকারলেখ কাঠামো ngx_pool_cleanup_s  ngx_pool_cleanup_t;

কাঠামো ngx_pool_cleanup_s {
    ngx_pool_cleanup_pt   handler;
    শূন্য                 *তথ্য;
    ngx_pool_cleanup_t   *পরবর্তী;
};


প্রকারলেখ কাঠামো ngx_pool_large_s  ngx_pool_large_t;

কাঠামো ngx_pool_large_s {
    ngx_pool_large_t     *পরবর্তী;
    শূন্য                 *alloc;
};


প্রকারলেখ কাঠামো {
    u_char               *last;
    u_char               *end;
    ngx_pool_t           *পরবর্তী;
    ngx_uint_t            failed;
} ngx_pool_তথ্য_t;


কাঠামো ngx_pool_s {
    ngx_pool_তথ্য_t       d;
    আকার_t                max;
    ngx_pool_t           *current;
    ngx_chain_t          *chain;
    ngx_pool_large_t     *large;
    ngx_pool_cleanup_t   *cleanup;
    ngx_log_t            *log;
};


প্রকারলেখ কাঠামো {
    ngx_fd_t              fd;
    u_char               *name;
    ngx_log_t            *log;
} ngx_pool_cleanup_file_t;


ngx_pool_t *ngx_create_pool(আকার_t আকার_2, ngx_log_t *log);
শূন্য ngx_destroy_pool(ngx_pool_t *pool);
শূন্য ngx_reset_pool(ngx_pool_t *pool);

শূন্য *ngx_palloc(ngx_pool_t *pool, আকার_t আকার_2);
শূন্য *ngx_pnalloc(ngx_pool_t *pool, আকার_t আকার_2);
শূন্য *ngx_pcalloc(ngx_pool_t *pool, আকার_t আকার_2);
শূন্য *ngx_pmemalign(ngx_pool_t *pool, আকার_t আকার_2, আকার_t alignment);
ngx_int_t ngx_pfree(ngx_pool_t *pool, শূন্য *p);


ngx_pool_cleanup_t *ngx_pool_cleanup_add(ngx_pool_t *p, আকার_t আকার_2);
শূন্য ngx_pool_run_cleanup_file(ngx_pool_t *p, ngx_fd_t fd);
শূন্য ngx_pool_cleanup_file(শূন্য *তথ্য);
শূন্য ngx_pool_delete_file(শূন্য *তথ্য);


#endif /* _NGX_PALLOC_H_INCLUDED_ */
