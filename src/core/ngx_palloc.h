
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#اگر_غیر_تعریف_شدہ _NGX_PALLOC_H_INCLUDED_
#تعریف_کرو _NGX_PALLOC_H_INCLUDED_


#شامل <ngx_config.h>
#شامل <ngx_core.h>


/*
 * NGX_MAX_ALLOC_FROM_POOL should be (ngx_pagesize - 1), i.e. 4095 on x86.
 * On Windows NT it decreases a number of locked pages in a kernel.
 */
#تعریف_کرو NGX_MAX_ALLOC_FROM_POOL  (ngx_pagesize - 1)

#تعریف_کرو NGX_DEFAULT_POOL_سائز    (16 * 1024)

#تعریف_کرو NGX_POOL_ALIGNMENT       16
#تعریف_کرو NGX_MIN_POOL_سائز                                                     \
    ngx_align((حجم(ngx_pool_t) + 2 * حجم(ngx_pool_large_t)),            \
              NGX_POOL_ALIGNMENT)


قسمتعریف خالی (*ngx_pool_cleanup_pt)(خالی *ڈیٹا);

قسمتعریف ساخت ngx_pool_cleanup_s  ngx_pool_cleanup_t;

ساخت ngx_pool_cleanup_s {
    ngx_pool_cleanup_pt   handler;
    خالی                 *ڈیٹا;
    ngx_pool_cleanup_t   *اگلا;
};


قسمتعریف ساخت ngx_pool_large_s  ngx_pool_large_t;

ساخت ngx_pool_large_s {
    ngx_pool_large_t     *اگلا;
    خالی                 *alloc;
};


قسمتعریف ساخت {
    u_char               *آخری;
    u_char               *انتہا;
    ngx_pool_t           *اگلا;
    ngx_uint_t            failed;
} ngx_pool_ڈیٹا_t;


ساخت ngx_pool_s {
    ngx_pool_ڈیٹا_t       d;
    سائز_t                max;
    ngx_pool_t           *current;
    ngx_chain_t          *chain;
    ngx_pool_large_t     *large;
    ngx_pool_cleanup_t   *cleanup;
    ngx_log_t            *لوگارتھم;
};


قسمتعریف ساخت {
    ngx_fd_t              fd;
    u_char               *name;
    ngx_log_t            *لوگارتھم;
} ngx_pool_cleanup_file_t;


ngx_pool_t *ngx_create_pool(سائز_t سائز, ngx_log_t *لوگارتھم);
خالی ngx_destroy_pool(ngx_pool_t *pool);
خالی ngx_reset_pool(ngx_pool_t *pool);

خالی *ngx_palloc(ngx_pool_t *pool, سائز_t سائز);
خالی *ngx_pnalloc(ngx_pool_t *pool, سائز_t سائز);
خالی *ngx_pcalloc(ngx_pool_t *pool, سائز_t سائز);
خالی *ngx_pmemalign(ngx_pool_t *pool, سائز_t سائز, سائز_t alignment);
ngx_int_t ngx_pfree(ngx_pool_t *pool, خالی *p);


ngx_pool_cleanup_t *ngx_pool_cleanup_جوڑو(ngx_pool_t *p, سائز_t سائز);
خالی ngx_pool_تسلسل_cleanup_file(ngx_pool_t *p, ngx_fd_t fd);
خالی ngx_pool_cleanup_file(خالی *ڈیٹا);
خالی ngx_pool_delete_file(خالی *ڈیٹا);


#اگر_ختم /* _NGX_PALLOC_H_INCLUDED_ */
