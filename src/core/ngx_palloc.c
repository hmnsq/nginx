
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


ಸ್ಥಿರ ngx_inline ಶೂನ್ಯ *ngx_palloc_small(ngx_pool_t *pool, ಗಾತ್ರ_t ಗಾತ್ರ_2,
    ngx_uint_t align);
ಸ್ಥಿರ ಶೂನ್ಯ *ngx_palloc_ಖಂಡ(ngx_pool_t *pool, ಗಾತ್ರ_t ಗಾತ್ರ_2);
ಸ್ಥಿರ ಶೂನ್ಯ *ngx_palloc_large(ngx_pool_t *pool, ಗಾತ್ರ_t ಗಾತ್ರ_2);


ngx_pool_t *
ngx_create_pool(ಗಾತ್ರ_t ಗಾತ್ರ_2, ngx_log_t *ಲಘುಗಣಕ)
{
    ngx_pool_t  *p;

    p = ngx_memalign(NGX_POOL_ALIGNMENT, ಗಾತ್ರ_2, ಲಘುಗಣಕ);
    ಆದರೆ (p == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    p->d.ಕೊನೆಯ = (u_char *) p + ಗಾತ್ರ(ngx_pool_t);
    p->d.ಕೊನೆ = (u_char *) p + ಗಾತ್ರ_2;
    p->d.ಮುಂದಿನ = ಖಾಲಿ;
    p->d.failed = 0;

    ಗಾತ್ರ_2 = ಗಾತ್ರ_2 - ಗಾತ್ರ(ngx_pool_t);
    p->max = (ಗಾತ್ರ_2 < NGX_MAX_ALLOC_FROM_POOL) ? ಗಾತ್ರ_2 : NGX_MAX_ALLOC_FROM_POOL;

    p->current = p;
    p->chain = ಖಾಲಿ;
    p->large = ಖಾಲಿ;
    p->cleanup = ಖಾಲಿ;
    p->ಲಘುಗಣಕ = ಲಘುಗಣಕ;

    ಹಿಂತಿರುಗಿ p;
}


ಶೂನ್ಯ
ngx_destroy_pool(ngx_pool_t *pool)
{
    ngx_pool_t          *p, *n;
    ngx_pool_large_t    *l;
    ngx_pool_cleanup_t  *c;

    ಗಾಗಿ (c = pool->cleanup; c; c = c->ಮುಂದಿನ) {
        ಆದರೆ (c->handler) {
            ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->ಲಘುಗಣಕ, 0,
                           "run cleanup: %p", c);
            c->handler(c->ದತ್ತಾಂಶ);
        }
    }

#ಆದರೆ (NGX_DEBUG)

    /*
     * we could allocate the pool->log from this pool
     * so we cannot use this log while free()ing the pool
     */

    ಗಾಗಿ (l = pool->large; l; l = l->ಮುಂದಿನ) {
        ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->ಲಘುಗಣಕ, 0, "free: %p", l->alloc);
    }

    ಗಾಗಿ (p = pool, n = pool->d.ಮುಂದಿನ; /* void */; p = n, n = n->d.ಮುಂದಿನ) {
        ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, pool->ಲಘುಗಣಕ, 0,
                       "free: %p, unused: %uz", p, p->d.ಕೊನೆ - p->d.ಕೊನೆಯ);

        ಆದರೆ (n == ಖಾಲಿ) {
            ನಿಲ್ಲಿಸು;
        }
    }

#ಆದರೆ_ಅಂತ್ಯ

    ಗಾಗಿ (l = pool->large; l; l = l->ಮುಂದಿನ) {
        ಆದರೆ (l->alloc) {
            ngx_free(l->alloc);
        }
    }

    ಗಾಗಿ (p = pool, n = pool->d.ಮುಂದಿನ; /* void */; p = n, n = n->d.ಮುಂದಿನ) {
        ngx_free(p);

        ಆದರೆ (n == ಖಾಲಿ) {
            ನಿಲ್ಲಿಸು;
        }
    }
}


ಶೂನ್ಯ
ngx_reset_pool(ngx_pool_t *pool)
{
    ngx_pool_t        *p;
    ngx_pool_large_t  *l;

    ಗಾಗಿ (l = pool->large; l; l = l->ಮುಂದಿನ) {
        ಆದರೆ (l->alloc) {
            ngx_free(l->alloc);
        }
    }

    ಗಾಗಿ (p = pool; p; p = p->d.ಮುಂದಿನ) {
        p->d.ಕೊನೆಯ = (u_char *) p + ಗಾತ್ರ(ngx_pool_t);
        p->d.failed = 0;
    }

    pool->current = pool;
    pool->chain = ಖಾಲಿ;
    pool->large = ಖಾಲಿ;
}


ಶೂನ್ಯ *
ngx_palloc(ngx_pool_t *pool, ಗಾತ್ರ_t ಗಾತ್ರ_2)
{
#ಆದರೆ !(NGX_DEBUG_PALLOC)
    ಆದರೆ (ಗಾತ್ರ_2 <= pool->max) {
        ಹಿಂತಿರುಗಿ ngx_palloc_small(pool, ಗಾತ್ರ_2, 1);
    }
#ಆದರೆ_ಅಂತ್ಯ

    ಹಿಂತಿರುಗಿ ngx_palloc_large(pool, ಗಾತ್ರ_2);
}


ಶೂನ್ಯ *
ngx_pnalloc(ngx_pool_t *pool, ಗಾತ್ರ_t ಗಾತ್ರ_2)
{
#ಆದರೆ !(NGX_DEBUG_PALLOC)
    ಆದರೆ (ಗಾತ್ರ_2 <= pool->max) {
        ಹಿಂತಿರುಗಿ ngx_palloc_small(pool, ಗಾತ್ರ_2, 0);
    }
#ಆದರೆ_ಅಂತ್ಯ

    ಹಿಂತಿರುಗಿ ngx_palloc_large(pool, ಗಾತ್ರ_2);
}


ಸ್ಥಿರ ngx_inline ಶೂನ್ಯ *
ngx_palloc_small(ngx_pool_t *pool, ಗಾತ್ರ_t ಗಾತ್ರ_2, ngx_uint_t align)
{
    u_char      *m;
    ngx_pool_t  *p;

    p = pool->current;

    ಮಾಡು {
        m = p->d.ಕೊನೆಯ;

        ಆದರೆ (align) {
            m = ngx_align_ptr(m, NGX_ALIGNMENT);
        }

        ಆದರೆ ((ಗಾತ್ರ_t) (p->d.ಕೊನೆ - m) >= ಗಾತ್ರ_2) {
            p->d.ಕೊನೆಯ = m + ಗಾತ್ರ_2;

            ಹಿಂತಿರುಗಿ m;
        }

        p = p->d.ಮುಂದಿನ;

    } ಆಗ (p);

    ಹಿಂತಿರುಗಿ ngx_palloc_ಖಂಡ(pool, ಗಾತ್ರ_2);
}


ಸ್ಥಿರ ಶೂನ್ಯ *
ngx_palloc_ಖಂಡ(ngx_pool_t *pool, ಗಾತ್ರ_t ಗಾತ್ರ_2)
{
    u_char      *m;
    ಗಾತ್ರ_t       psize;
    ngx_pool_t  *p, *new;

    psize = (ಗಾತ್ರ_t) (pool->d.ಕೊನೆ - (u_char *) pool);

    m = ngx_memalign(NGX_POOL_ALIGNMENT, psize, pool->ಲಘುಗಣಕ);
    ಆದರೆ (m == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    new = (ngx_pool_t *) m;

    new->d.ಕೊನೆ = m + psize;
    new->d.ಮುಂದಿನ = ಖಾಲಿ;
    new->d.failed = 0;

    m += ಗಾತ್ರ(ngx_pool_ದತ್ತಾಂಶ_t);
    m = ngx_align_ptr(m, NGX_ALIGNMENT);
    new->d.ಕೊನೆಯ = m + ಗಾತ್ರ_2;

    ಗಾಗಿ (p = pool->current; p->d.ಮುಂದಿನ; p = p->d.ಮುಂದಿನ) {
        ಆದರೆ (p->d.failed++ > 4) {
            pool->current = p->d.ಮುಂದಿನ;
        }
    }

    p->d.ಮುಂದಿನ = new;

    ಹಿಂತಿರುಗಿ m;
}


ಸ್ಥಿರ ಶೂನ್ಯ *
ngx_palloc_large(ngx_pool_t *pool, ಗಾತ್ರ_t ಗಾತ್ರ_2)
{
    ಶೂನ್ಯ              *p;
    ngx_uint_t         n;
    ngx_pool_large_t  *large;

    p = ngx_alloc(ಗಾತ್ರ_2, pool->ಲಘುಗಣಕ);
    ಆದರೆ (p == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    n = 0;

    ಗಾಗಿ (large = pool->large; large; large = large->ಮುಂದಿನ) {
        ಆದರೆ (large->alloc == ಖಾಲಿ) {
            large->alloc = p;
            ಹಿಂತಿರುಗಿ p;
        }

        ಆದರೆ (n++ > 3) {
            ನಿಲ್ಲಿಸು;
        }
    }

    large = ngx_palloc_small(pool, ಗಾತ್ರ(ngx_pool_large_t), 1);
    ಆದರೆ (large == ಖಾಲಿ) {
        ngx_free(p);
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    large->alloc = p;
    large->ಮುಂದಿನ = pool->large;
    pool->large = large;

    ಹಿಂತಿರುಗಿ p;
}


ಶೂನ್ಯ *
ngx_pmemalign(ngx_pool_t *pool, ಗಾತ್ರ_t ಗಾತ್ರ_2, ಗಾತ್ರ_t alignment)
{
    ಶೂನ್ಯ              *p;
    ngx_pool_large_t  *large;

    p = ngx_memalign(alignment, ಗಾತ್ರ_2, pool->ಲಘುಗಣಕ);
    ಆದರೆ (p == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    large = ngx_palloc_small(pool, ಗಾತ್ರ(ngx_pool_large_t), 1);
    ಆದರೆ (large == ಖಾಲಿ) {
        ngx_free(p);
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    large->alloc = p;
    large->ಮುಂದಿನ = pool->large;
    pool->large = large;

    ಹಿಂತಿರುಗಿ p;
}


ngx_int_t
ngx_pfree(ngx_pool_t *pool, ಶೂನ್ಯ *p)
{
    ngx_pool_large_t  *l;

    ಗಾಗಿ (l = pool->large; l; l = l->ಮುಂದಿನ) {
        ಆದರೆ (p == l->alloc) {
            ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->ಲಘುಗಣಕ, 0,
                           "free: %p", l->alloc);
            ngx_free(l->alloc);
            l->alloc = ಖಾಲಿ;

            ಹಿಂತಿರುಗಿ NGX_OK;
        }
    }

    ಹಿಂತಿರುಗಿ NGX_DECLINED;
}


ಶೂನ್ಯ *
ngx_pcalloc(ngx_pool_t *pool, ಗಾತ್ರ_t ಗಾತ್ರ_2)
{
    ಶೂನ್ಯ *p;

    p = ngx_palloc(pool, ಗಾತ್ರ_2);
    ಆದರೆ (p) {
        ngx_memzero(p, ಗಾತ್ರ_2);
    }

    ಹಿಂತಿರುಗಿ p;
}


ngx_pool_cleanup_t *
ngx_pool_cleanup_ಕೂಡಿಸು(ngx_pool_t *p, ಗಾತ್ರ_t ಗಾತ್ರ_2)
{
    ngx_pool_cleanup_t  *c;

    c = ngx_palloc(p, ಗಾತ್ರ(ngx_pool_cleanup_t));
    ಆದರೆ (c == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ಆದರೆ (ಗಾತ್ರ_2) {
        c->ದತ್ತಾಂಶ = ngx_palloc(p, ಗಾತ್ರ_2);
        ಆದರೆ (c->ದತ್ತಾಂಶ == ಖಾಲಿ) {
            ಹಿಂತಿರುಗಿ ಖಾಲಿ;
        }

    } ಇಲ್ಲದಿದ್ದರೆ {
        c->ದತ್ತಾಂಶ = ಖಾಲಿ;
    }

    c->handler = ಖಾಲಿ;
    c->ಮುಂದಿನ = p->cleanup;

    p->cleanup = c;

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, p->ಲಘುಗಣಕ, 0, "add cleanup: %p", c);

    ಹಿಂತಿರುಗಿ c;
}


ಶೂನ್ಯ
ngx_pool_ಸತತ_cleanup_file(ngx_pool_t *p, ngx_fd_t fd)
{
    ngx_pool_cleanup_t       *c;
    ngx_pool_cleanup_file_t  *cf;

    ಗಾಗಿ (c = p->cleanup; c; c = c->ಮುಂದಿನ) {
        ಆದರೆ (c->handler == ngx_pool_cleanup_file) {

            cf = c->ದತ್ತಾಂಶ;

            ಆದರೆ (cf->fd == fd) {
                c->handler(cf);
                c->handler = ಖಾಲಿ;
                ಹಿಂತಿರುಗಿ;
            }
        }
    }
}


ಶೂನ್ಯ
ngx_pool_cleanup_file(ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
    ngx_pool_cleanup_file_t  *c = ದತ್ತಾಂಶ;

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, c->ಲಘುಗಣಕ, 0, "file cleanup: fd:%d",
                   c->fd);

    ಆದರೆ (ngx_close_file(c->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, c->ಲಘುಗಣಕ, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", c->name);
    }
}


ಶೂನ್ಯ
ngx_pool_delete_file(ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
    ngx_pool_cleanup_file_t  *c = ದತ್ತಾಂಶ;

    ngx_err_t  err;

    ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, c->ಲಘುಗಣಕ, 0, "file cleanup: fd:%d %s",
                   c->fd, c->name);

    ಆದರೆ (ngx_delete_file(c->name) == NGX_FILE_ERROR) {
        err = ngx_errno;

        ಆದರೆ (err != NGX_ENOENT) {
            ngx_log_error(NGX_LOG_CRIT, c->ಲಘುಗಣಕ, err,
                          ngx_delete_file_n " \"%s\" failed", c->name);
        }
    }

    ಆದರೆ (ngx_close_file(c->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, c->ಲಘುಗಣಕ, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", c->name);
    }
}


#ಆದರೆ 0

ಸ್ಥಿರ ಶೂನ್ಯ *
ngx_get_cached_ಖಂಡ(ಗಾತ್ರ_t ಗಾತ್ರ_2)
{
    ಶೂನ್ಯ                     *p;
    ngx_cached_ಖಂಡ_ಸ್ಥಾನ_t  *ಸ್ಥಾನ;

    ಆದರೆ (ngx_ಚಕ್ರ->cache == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ಸ್ಥಾನ = &ngx_ಚಕ್ರ->cache[(ಗಾತ್ರ_2 + ngx_pagesize - 1) / ngx_pagesize];

    ಸ್ಥಾನ->tries++;

    ಆದರೆ (ಸ್ಥಾನ->ಸಂಖ್ಯೆ) {
        p = ಸ್ಥಾನ->ಖಂಡ;
        ಸ್ಥಾನ->ಖಂಡ = ಸ್ಥಾನ->ಖಂಡ->ಮುಂದಿನ;
        ಸ್ಥಾನ->ಸಂಖ್ಯೆ--;
        ಹಿಂತಿರುಗಿ p;
    }

    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

#ಆದರೆ_ಅಂತ್ಯ
