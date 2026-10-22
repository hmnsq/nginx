
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_PALLOC_H_INCLUDED_
#define _NGX_PALLOC_H_INCLUDED_


#அந்தர்பாவ <ngx_config.h>
#அந்தர்பாவ <ngx_core.h>


/*
 * NGX_MAX_ALLOC_FROM_POOL should be (ngx_pagesize - 1), i.e. 4095 on x86.
 * On Windows NT it decreases a number of locked pages in a kernel.
 */
#define NGX_MAX_ALLOC_FROM_POOL  (ngx_pagesize - 1)

#define NGX_DEFAULT_POOL_SIZE    (16 * 1024)

#define NGX_POOL_ALIGNMENT       16
#define NGX_MIN_POOL_SIZE                                                     \
    ngx_align((அளவு(ngx_pool_t) + 2 * அளவு(ngx_pool_large_t)),            \
              NGX_POOL_ALIGNMENT)


வகைவரை வெறுமை (*ngx_pool_cleanup_pt)(வெறுமை *data);

வகைவரை கட்டமைப்பு ngx_pool_cleanup_s  ngx_pool_cleanup_t;

கட்டமைப்பு ngx_pool_cleanup_s {
    ngx_pool_cleanup_pt   handler;
    வெறுமை                 *data;
    ngx_pool_cleanup_t   *next;
};


வகைவரை கட்டமைப்பு ngx_pool_large_s  ngx_pool_large_t;

கட்டமைப்பு ngx_pool_large_s {
    ngx_pool_large_t     *next;
    வெறுமை                 *alloc;
};


வகைவரை கட்டமைப்பு {
    u_char               *last;
    u_char               *end;
    ngx_pool_t           *next;
    ngx_uint_t            failed;
} ngx_pool_data_t;


கட்டமைப்பு ngx_pool_s {
    ngx_pool_data_t       d;
    size_t                max;
    ngx_pool_t           *current;
    ngx_chain_t          *chain;
    ngx_pool_large_t     *large;
    ngx_pool_cleanup_t   *cleanup;
    ngx_log_t            *log;
};


வகைவரை கட்டமைப்பு {
    ngx_fd_t              fd;
    u_char               *name;
    ngx_log_t            *log;
} ngx_pool_cleanup_file_t;


ngx_pool_t *ngx_create_pool(size_t size, ngx_log_t *log);
வெறுமை ngx_destroy_pool(ngx_pool_t *pool);
வெறுமை ngx_reset_pool(ngx_pool_t *pool);

வெறுமை *ngx_palloc(ngx_pool_t *pool, size_t size);
வெறுமை *ngx_pnalloc(ngx_pool_t *pool, size_t size);
வெறுமை *ngx_pcalloc(ngx_pool_t *pool, size_t size);
வெறுமை *ngx_pmemalign(ngx_pool_t *pool, size_t size, size_t alignment);
ngx_int_t ngx_pfree(ngx_pool_t *pool, வெறுமை *p);


ngx_pool_cleanup_t *ngx_pool_cleanup_add(ngx_pool_t *p, size_t size);
வெறுமை ngx_pool_run_cleanup_file(ngx_pool_t *p, ngx_fd_t fd);
வெறுமை ngx_pool_cleanup_file(வெறுமை *data);
வெறுமை ngx_pool_delete_file(வெறுமை *data);


#endif /* _NGX_PALLOC_H_INCLUDED_ */
