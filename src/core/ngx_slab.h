
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ _NGX_SLAB_H_INCLUDED_
#ವ್ಯಾಖ್ಯಾನಿಸು _NGX_SLAB_H_INCLUDED_


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ ngx_slab_ಪುಟ_s  ngx_slab_ಪುಟ_t;

ರಚನೆ ngx_slab_ಪುಟ_s {
    uintptr_t         slab;
    ngx_slab_ಪುಟ_t  *ಮುಂದಿನ;
    uintptr_t         prev;
};


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ngx_uint_t        ಒಟ್ಟು;
    ngx_uint_t        ಬಳಸಿದ;

    ngx_uint_t        reqs;
    ngx_uint_t        fails;
} ngx_slab_stat_t;


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ngx_shmtx_sh_t    ಬೀಗ;

    ಗಾತ್ರ_t            min_ಗಾತ್ರ;
    ಗಾತ್ರ_t            min_ಸರಿಸು;

    ngx_slab_ಪುಟ_t  *ಪುಟಗಳು;
    ngx_slab_ಪುಟ_t  *ಕೊನೆಯ;
    ngx_slab_ಪುಟ_t   ಬಿಡುಗಡೆ;

    ngx_slab_stat_t  *stats;
    ngx_uint_t        pfree;

    u_char           *ಆರಂಭ;
    u_char           *ಕೊನೆ;

    ngx_shmtx_t       mutex;

    u_char           *log_ctx;
    u_char            zero;

    ಚಿಹ್ನೆರಹಿತ          log_nomem:1;

    ಶೂನ್ಯ             *ದತ್ತಾಂಶ;
    ಶೂನ್ಯ             *addr;
} ngx_slab_pool_t;


ಶೂನ್ಯ ngx_slab_sizes_init(ಶೂನ್ಯ);
ಶೂನ್ಯ ngx_slab_init(ngx_slab_pool_t *pool);
ಶೂನ್ಯ *ngx_slab_alloc(ngx_slab_pool_t *pool, ಗಾತ್ರ_t ಗಾತ್ರ_2);
ಶೂನ್ಯ *ngx_slab_alloc_locked(ngx_slab_pool_t *pool, ಗಾತ್ರ_t ಗಾತ್ರ_2);
ಶೂನ್ಯ *ngx_slab_calloc(ngx_slab_pool_t *pool, ಗಾತ್ರ_t ಗಾತ್ರ_2);
ಶೂನ್ಯ *ngx_slab_calloc_locked(ngx_slab_pool_t *pool, ಗಾತ್ರ_t ಗಾತ್ರ_2);
ಶೂನ್ಯ ngx_slab_free(ngx_slab_pool_t *pool, ಶೂನ್ಯ *p);
ಶೂನ್ಯ ngx_slab_free_locked(ngx_slab_pool_t *pool, ಶೂನ್ಯ *p);


#ಆದರೆ_ಅಂತ್ಯ /* _NGX_SLAB_H_INCLUDED_ */
