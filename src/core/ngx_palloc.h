
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#جيڪڏهن_وصف_نه_ٿيل _NGX_PALLOC_H_INCLUDED_
#وصف_ڪريو _NGX_PALLOC_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


/*
 * NGX_MAX_ALLOC_FROM_POOL should be (ngx_pagesize - 1), i.e. 4095 on x86.
 * On Windows NT it decreases a number of locked pages in a kernel.
 */
#وصف_ڪريو NGX_MAX_ALLOC_FROM_POOL  (ngx_pagesize - 1)

#وصف_ڪريو NGX_DEFAULT_POOL_سائيز    (16 * 1024)

#وصف_ڪريو NGX_POOL_ALIGNMENT       16
#وصف_ڪريو NGX_MIN_POOL_سائيز                                                     \
    ngx_align((حجم(ngx_pool_t) + 2 * حجم(ngx_pool_large_t)),            \
              NGX_POOL_ALIGNMENT)


تعريفنوع فراغ (*ngx_pool_cleanup_pt)(فراغ *معلومات);

تعريفنوع بنية ngx_pool_cleanup_s  ngx_pool_cleanup_t;

بنية ngx_pool_cleanup_s {
    ngx_pool_cleanup_pt   handler;
    فراغ                 *معلومات;
    ngx_pool_cleanup_t   *ايندڙ;
};


تعريفنوع بنية ngx_pool_large_s  ngx_pool_large_t;

بنية ngx_pool_large_s {
    ngx_pool_large_t     *ايندڙ;
    فراغ                 *alloc;
};


تعريفنوع بنية {
    u_char               *آخري;
    u_char               *پڇاڙي;
    ngx_pool_t           *ايندڙ;
    ngx_uint_t            failed;
} ngx_pool_معلومات_t;


بنية ngx_pool_s {
    ngx_pool_معلومات_t       d;
    سائيز_t                max;
    ngx_pool_t           *current;
    ngx_chain_t          *chain;
    ngx_pool_large_t     *large;
    ngx_pool_cleanup_t   *cleanup;
    ngx_log_t            *لاگرٿم;
};


تعريفنوع بنية {
    ngx_fd_t              fd;
    u_char               *name;
    ngx_log_t            *لاگرٿم;
} ngx_pool_cleanup_file_t;


ngx_pool_t *ngx_create_pool(سائيز_t سائيز, ngx_log_t *لاگرٿم);
فراغ ngx_destroy_pool(ngx_pool_t *pool);
فراغ ngx_reset_pool(ngx_pool_t *pool);

فراغ *ngx_palloc(ngx_pool_t *pool, سائيز_t سائيز);
فراغ *ngx_pnalloc(ngx_pool_t *pool, سائيز_t سائيز);
فراغ *ngx_pcalloc(ngx_pool_t *pool, سائيز_t سائيز);
فراغ *ngx_pmemalign(ngx_pool_t *pool, سائيز_t سائيز, سائيز_t alignment);
ngx_int_t ngx_pfree(ngx_pool_t *pool, فراغ *p);


ngx_pool_cleanup_t *ngx_pool_cleanup_جوڙيو(ngx_pool_t *p, سائيز_t سائيز);
فراغ ngx_pool_لاڳيتو_cleanup_file(ngx_pool_t *p, ngx_fd_t fd);
فراغ ngx_pool_cleanup_file(فراغ *معلومات);
فراغ ngx_pool_delete_file(فراغ *معلومات);


#جيڪڏهن_پورو /* _NGX_PALLOC_H_INCLUDED_ */
