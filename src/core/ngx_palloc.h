
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ _NGX_PALLOC_H_INCLUDED_
#ವ್ಯಾಖ್ಯಾನಿಸು _NGX_PALLOC_H_INCLUDED_


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


/*
 * NGX_MAX_ALLOC_FROM_POOL should be (ngx_pagesize - 1), i.e. 4095 on x86.
 * On Windows NT it decreases a number of locked pages in a kernel.
 */
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MAX_ALLOC_FROM_POOL  (ngx_pagesize - 1)

#ವ್ಯಾಖ್ಯಾನಿಸು NGX_DEFAULT_POOL_ಗಾತ್ರ    (16 * 1024)

#ವ್ಯಾಖ್ಯಾನಿಸು NGX_POOL_ALIGNMENT       16
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MIN_POOL_ಗಾತ್ರ                                                     \
    ngx_align((ಗಾತ್ರ(ngx_pool_t) + 2 * ಗಾತ್ರ(ngx_pool_large_t)),            \
              NGX_POOL_ALIGNMENT)


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (*ngx_pool_cleanup_pt)(ಶೂನ್ಯ *ದತ್ತಾಂಶ);

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ ngx_pool_cleanup_s  ngx_pool_cleanup_t;

ರಚನೆ ngx_pool_cleanup_s {
    ngx_pool_cleanup_pt   handler;
    ಶೂನ್ಯ                 *ದತ್ತಾಂಶ;
    ngx_pool_cleanup_t   *ಮುಂದಿನ;
};


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ ngx_pool_large_s  ngx_pool_large_t;

ರಚನೆ ngx_pool_large_s {
    ngx_pool_large_t     *ಮುಂದಿನ;
    ಶೂನ್ಯ                 *alloc;
};


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    u_char               *ಕೊನೆಯ;
    u_char               *ಕೊನೆ;
    ngx_pool_t           *ಮುಂದಿನ;
    ngx_uint_t            failed;
} ngx_pool_ದತ್ತಾಂಶ_t;


ರಚನೆ ngx_pool_s {
    ngx_pool_ದತ್ತಾಂಶ_t       d;
    ಗಾತ್ರ_t                max;
    ngx_pool_t           *current;
    ngx_chain_t          *chain;
    ngx_pool_large_t     *large;
    ngx_pool_cleanup_t   *cleanup;
    ngx_log_t            *ಲಘುಗಣಕ;
};


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ngx_fd_t              fd;
    u_char               *name;
    ngx_log_t            *ಲಘುಗಣಕ;
} ngx_pool_cleanup_file_t;


ngx_pool_t *ngx_create_pool(ಗಾತ್ರ_t ಗಾತ್ರ_2, ngx_log_t *ಲಘುಗಣಕ);
ಶೂನ್ಯ ngx_destroy_pool(ngx_pool_t *pool);
ಶೂನ್ಯ ngx_reset_pool(ngx_pool_t *pool);

ಶೂನ್ಯ *ngx_palloc(ngx_pool_t *pool, ಗಾತ್ರ_t ಗಾತ್ರ_2);
ಶೂನ್ಯ *ngx_pnalloc(ngx_pool_t *pool, ಗಾತ್ರ_t ಗಾತ್ರ_2);
ಶೂನ್ಯ *ngx_pcalloc(ngx_pool_t *pool, ಗಾತ್ರ_t ಗಾತ್ರ_2);
ಶೂನ್ಯ *ngx_pmemalign(ngx_pool_t *pool, ಗಾತ್ರ_t ಗಾತ್ರ_2, ಗಾತ್ರ_t alignment);
ngx_int_t ngx_pfree(ngx_pool_t *pool, ಶೂನ್ಯ *p);


ngx_pool_cleanup_t *ngx_pool_cleanup_ಕೂಡಿಸು(ngx_pool_t *p, ಗಾತ್ರ_t ಗಾತ್ರ_2);
ಶೂನ್ಯ ngx_pool_ಸತತ_cleanup_file(ngx_pool_t *p, ngx_fd_t fd);
ಶೂನ್ಯ ngx_pool_cleanup_file(ಶೂನ್ಯ *ದತ್ತಾಂಶ);
ಶೂನ್ಯ ngx_pool_delete_file(ಶೂನ್ಯ *ದತ್ತಾಂಶ);


#ಆದರೆ_ಅಂತ್ಯ /* _NGX_PALLOC_H_INCLUDED_ */
