
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#جے_بنا_تعریف _NGX_PALLOC_H_INCLUDED_
#تعریف_کرو _NGX_PALLOC_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


/*
 * NGX_MAX_ALLOC_FROM_POOL should be (ngx_pagesize - 1), i.e. 4095 on x86.
 * On Windows NT it decreases a number of locked pages in a kernel.
 */
#تعریف_کرو NGX_MAX_ALLOC_FROM_POOL  (ngx_pagesize - 1)

#تعریف_کرو NGX_DEFAULT_POOL_ناپ    (16 * 1024)

#تعریف_کرو NGX_POOL_ALIGNMENT       16
#تعریف_کرو NGX_MIN_POOL_ناپ                                                     \
    ngx_align((حجم(ngx_pool_t) + 2 * حجم(ngx_pool_large_t)),            \
              NGX_POOL_ALIGNMENT)


تعريفنوع فراغ (*ngx_pool_cleanup_pt)(فراغ *انکڑے);

تعريفنوع بنية ngx_pool_cleanup_s  ngx_pool_cleanup_t;

بنية ngx_pool_cleanup_s {
    ngx_pool_cleanup_pt   handler;
    فراغ                 *انکڑے;
    ngx_pool_cleanup_t   *اگلا;
};


تعريفنوع بنية ngx_pool_large_s  ngx_pool_large_t;

بنية ngx_pool_large_s {
    ngx_pool_large_t     *اگلا;
    فراغ                 *alloc;
};


تعريفنوع بنية {
    u_char               *آخری;
    u_char               *سرا;
    ngx_pool_t           *اگلا;
    ngx_uint_t            failed;
} ngx_pool_انکڑے_t;


بنية ngx_pool_s {
    ngx_pool_انکڑے_t       d;
    ناپ_t                max;
    ngx_pool_t           *current;
    ngx_chain_t          *chain;
    ngx_pool_large_t     *large;
    ngx_pool_cleanup_t   *cleanup;
    ngx_log_t            *لوگارتھم;
};


تعريفنوع بنية {
    ngx_fd_t              fd;
    u_char               *name;
    ngx_log_t            *لوگارتھم;
} ngx_pool_cleanup_file_t;


ngx_pool_t *ngx_create_pool(ناپ_t ناپ, ngx_log_t *لوگارتھم);
فراغ ngx_destroy_pool(ngx_pool_t *pool);
فراغ ngx_reset_pool(ngx_pool_t *pool);

فراغ *ngx_palloc(ngx_pool_t *pool, ناپ_t ناپ);
فراغ *ngx_pnalloc(ngx_pool_t *pool, ناپ_t ناپ);
فراغ *ngx_pcalloc(ngx_pool_t *pool, ناپ_t ناپ);
فراغ *ngx_pmemalign(ngx_pool_t *pool, ناپ_t ناپ, ناپ_t alignment);
ngx_int_t ngx_pfree(ngx_pool_t *pool, فراغ *p);


ngx_pool_cleanup_t *ngx_pool_cleanup_جوڑو(ngx_pool_t *p, ناپ_t ناپ);
فراغ ngx_pool_لگاتار_cleanup_file(ngx_pool_t *p, ngx_fd_t fd);
فراغ ngx_pool_cleanup_file(فراغ *انکڑے);
فراغ ngx_pool_delete_file(فراغ *انکڑے);


#جے_انت /* _NGX_PALLOC_H_INCLUDED_ */
