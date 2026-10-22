
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ _NGX_PALLOC_H_INCLUDED_
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _NGX_PALLOC_H_INCLUDED_


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>


/*
 * NGX_MAX_ALLOC_FROM_POOL should be (ngx_pagesize - 1), i.e. 4095 on x86.
 * On Windows NT it decreases a number of locked pages in a kernel.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MAX_ALLOC_FROM_POOL  (ngx_pagesize - 1)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_DEFAULT_POOL_ਮਾਪ    (16 * 1024)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_POOL_ALIGNMENT       16
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MIN_POOL_ਮਾਪ                                                     \
    ngx_align((ਆਕਾਰ(ngx_pool_t) + 2 * ਆਕਾਰ(ngx_pool_large_t)),            \
              NGX_POOL_ALIGNMENT)


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (*ngx_pool_cleanup_pt)(ਖਾਲੀ *ਅੰਕੜੇ);

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ngx_pool_cleanup_s  ngx_pool_cleanup_t;

ਬਣਤਰ ngx_pool_cleanup_s {
    ngx_pool_cleanup_pt   handler;
    ਖਾਲੀ                 *ਅੰਕੜੇ;
    ngx_pool_cleanup_t   *ਅਗਲਾ;
};


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ngx_pool_large_s  ngx_pool_large_t;

ਬਣਤਰ ngx_pool_large_s {
    ngx_pool_large_t     *ਅਗਲਾ;
    ਖਾਲੀ                 *alloc;
};


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    u_char               *ਆਖਰੀ;
    u_char               *ਸਿਰਾ;
    ngx_pool_t           *ਅਗਲਾ;
    ngx_uint_t            failed;
} ngx_pool_ਅੰਕੜੇ_t;


ਬਣਤਰ ngx_pool_s {
    ngx_pool_ਅੰਕੜੇ_t       d;
    ਮਾਪ_t                max;
    ngx_pool_t           *current;
    ngx_chain_t          *chain;
    ngx_pool_large_t     *large;
    ngx_pool_cleanup_t   *cleanup;
    ngx_log_t            *ਲਘੂਗਣਕ;
};


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ngx_fd_t              fd;
    u_char               *name;
    ngx_log_t            *ਲਘੂਗਣਕ;
} ngx_pool_cleanup_file_t;


ngx_pool_t *ngx_create_pool(ਮਾਪ_t ਮਾਪ, ngx_log_t *ਲਘੂਗਣਕ);
ਖਾਲੀ ngx_destroy_pool(ngx_pool_t *pool);
ਖਾਲੀ ngx_reset_pool(ngx_pool_t *pool);

ਖਾਲੀ *ngx_palloc(ngx_pool_t *pool, ਮਾਪ_t ਮਾਪ);
ਖਾਲੀ *ngx_pnalloc(ngx_pool_t *pool, ਮਾਪ_t ਮਾਪ);
ਖਾਲੀ *ngx_pcalloc(ngx_pool_t *pool, ਮਾਪ_t ਮਾਪ);
ਖਾਲੀ *ngx_pmemalign(ngx_pool_t *pool, ਮਾਪ_t ਮਾਪ, ਮਾਪ_t alignment);
ngx_int_t ngx_pfree(ngx_pool_t *pool, ਖਾਲੀ *p);


ngx_pool_cleanup_t *ngx_pool_cleanup_ਜੋੜੋ(ngx_pool_t *p, ਮਾਪ_t ਮਾਪ);
ਖਾਲੀ ngx_pool_ਲੜੀਵਾਰ_cleanup_file(ngx_pool_t *p, ngx_fd_t fd);
ਖਾਲੀ ngx_pool_cleanup_file(ਖਾਲੀ *ਅੰਕੜੇ);
ਖਾਲੀ ngx_pool_delete_file(ਖਾਲੀ *ਅੰਕੜੇ);


#ਜੇ_ਅੰਤ /* _NGX_PALLOC_H_INCLUDED_ */
