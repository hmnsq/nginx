
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>


ਸਥਿਰ ngx_inline ਖਾਲੀ *ngx_palloc_small(ngx_pool_t *pool, ਮਾਪ_t ਮਾਪ,
    ngx_uint_t align);
ਸਥਿਰ ਖਾਲੀ *ngx_palloc_ਖੰਡ(ngx_pool_t *pool, ਮਾਪ_t ਮਾਪ);
ਸਥਿਰ ਖਾਲੀ *ngx_palloc_large(ngx_pool_t *pool, ਮਾਪ_t ਮਾਪ);


ngx_pool_t *
ngx_create_pool(ਮਾਪ_t ਮਾਪ, ngx_log_t *ਲਘੂਗਣਕ)
{
    ngx_pool_t  *p;

    p = ngx_memalign(NGX_POOL_ALIGNMENT, ਮਾਪ, ਲਘੂਗਣਕ);
    ਜੇ (p == ਨਿਰਾ) {
        ਵਾਪਸ ਨਿਰਾ;
    }

    p->d.ਆਖਰੀ = (u_char *) p + ਆਕਾਰ(ngx_pool_t);
    p->d.ਸਿਰਾ = (u_char *) p + ਮਾਪ;
    p->d.ਅਗਲਾ = ਨਿਰਾ;
    p->d.failed = 0;

    ਮਾਪ = ਮਾਪ - ਆਕਾਰ(ngx_pool_t);
    p->max = (ਮਾਪ < NGX_MAX_ALLOC_FROM_POOL) ? ਮਾਪ : NGX_MAX_ALLOC_FROM_POOL;

    p->current = p;
    p->chain = ਨਿਰਾ;
    p->large = ਨਿਰਾ;
    p->cleanup = ਨਿਰਾ;
    p->ਲਘੂਗਣਕ = ਲਘੂਗਣਕ;

    ਵਾਪਸ p;
}


ਖਾਲੀ
ngx_destroy_pool(ngx_pool_t *pool)
{
    ngx_pool_t          *p, *n;
    ngx_pool_large_t    *l;
    ngx_pool_cleanup_t  *c;

    ਲਈ (c = pool->cleanup; c; c = c->ਅਗਲਾ) {
        ਜੇ (c->handler) {
            ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->ਲਘੂਗਣਕ, 0,
                           "run cleanup: %p", c);
            c->handler(c->ਅੰਕੜੇ);
        }
    }

#ਜੇ (NGX_DEBUG)

    /*
     * we could allocate the pool->log from this pool
     * so we cannot use this log while free()ing the pool
     */

    ਲਈ (l = pool->large; l; l = l->ਅਗਲਾ) {
        ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->ਲਘੂਗਣਕ, 0, "free: %p", l->alloc);
    }

    ਲਈ (p = pool, n = pool->d.ਅਗਲਾ; /* void */; p = n, n = n->d.ਅਗਲਾ) {
        ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, pool->ਲਘੂਗਣਕ, 0,
                       "free: %p, unused: %uz", p, p->d.ਸਿਰਾ - p->d.ਆਖਰੀ);

        ਜੇ (n == ਨਿਰਾ) {
            ਰੋਕੋ;
        }
    }

#ਜੇ_ਅੰਤ

    ਲਈ (l = pool->large; l; l = l->ਅਗਲਾ) {
        ਜੇ (l->alloc) {
            ngx_free(l->alloc);
        }
    }

    ਲਈ (p = pool, n = pool->d.ਅਗਲਾ; /* void */; p = n, n = n->d.ਅਗਲਾ) {
        ngx_free(p);

        ਜੇ (n == ਨਿਰਾ) {
            ਰੋਕੋ;
        }
    }
}


ਖਾਲੀ
ngx_reset_pool(ngx_pool_t *pool)
{
    ngx_pool_t        *p;
    ngx_pool_large_t  *l;

    ਲਈ (l = pool->large; l; l = l->ਅਗਲਾ) {
        ਜੇ (l->alloc) {
            ngx_free(l->alloc);
        }
    }

    ਲਈ (p = pool; p; p = p->d.ਅਗਲਾ) {
        p->d.ਆਖਰੀ = (u_char *) p + ਆਕਾਰ(ngx_pool_t);
        p->d.failed = 0;
    }

    pool->current = pool;
    pool->chain = ਨਿਰਾ;
    pool->large = ਨਿਰਾ;
}


ਖਾਲੀ *
ngx_palloc(ngx_pool_t *pool, ਮਾਪ_t ਮਾਪ)
{
#ਜੇ !(NGX_DEBUG_PALLOC)
    ਜੇ (ਮਾਪ <= pool->max) {
        ਵਾਪਸ ngx_palloc_small(pool, ਮਾਪ, 1);
    }
#ਜੇ_ਅੰਤ

    ਵਾਪਸ ngx_palloc_large(pool, ਮਾਪ);
}


ਖਾਲੀ *
ngx_pnalloc(ngx_pool_t *pool, ਮਾਪ_t ਮਾਪ)
{
#ਜੇ !(NGX_DEBUG_PALLOC)
    ਜੇ (ਮਾਪ <= pool->max) {
        ਵਾਪਸ ngx_palloc_small(pool, ਮਾਪ, 0);
    }
#ਜੇ_ਅੰਤ

    ਵਾਪਸ ngx_palloc_large(pool, ਮਾਪ);
}


ਸਥਿਰ ngx_inline ਖਾਲੀ *
ngx_palloc_small(ngx_pool_t *pool, ਮਾਪ_t ਮਾਪ, ngx_uint_t align)
{
    u_char      *m;
    ngx_pool_t  *p;

    p = pool->current;

    ਕਰੋ {
        m = p->d.ਆਖਰੀ;

        ਜੇ (align) {
            m = ngx_align_ptr(m, NGX_ALIGNMENT);
        }

        ਜੇ ((ਮਾਪ_t) (p->d.ਸਿਰਾ - m) >= ਮਾਪ) {
            p->d.ਆਖਰੀ = m + ਮਾਪ;

            ਵਾਪਸ m;
        }

        p = p->d.ਅਗਲਾ;

    } ਜਦੋਂ (p);

    ਵਾਪਸ ngx_palloc_ਖੰਡ(pool, ਮਾਪ);
}


ਸਥਿਰ ਖਾਲੀ *
ngx_palloc_ਖੰਡ(ngx_pool_t *pool, ਮਾਪ_t ਮਾਪ)
{
    u_char      *m;
    ਮਾਪ_t       psize;
    ngx_pool_t  *p, *new;

    psize = (ਮਾਪ_t) (pool->d.ਸਿਰਾ - (u_char *) pool);

    m = ngx_memalign(NGX_POOL_ALIGNMENT, psize, pool->ਲਘੂਗਣਕ);
    ਜੇ (m == ਨਿਰਾ) {
        ਵਾਪਸ ਨਿਰਾ;
    }

    new = (ngx_pool_t *) m;

    new->d.ਸਿਰਾ = m + psize;
    new->d.ਅਗਲਾ = ਨਿਰਾ;
    new->d.failed = 0;

    m += ਆਕਾਰ(ngx_pool_ਅੰਕੜੇ_t);
    m = ngx_align_ptr(m, NGX_ALIGNMENT);
    new->d.ਆਖਰੀ = m + ਮਾਪ;

    ਲਈ (p = pool->current; p->d.ਅਗਲਾ; p = p->d.ਅਗਲਾ) {
        ਜੇ (p->d.failed++ > 4) {
            pool->current = p->d.ਅਗਲਾ;
        }
    }

    p->d.ਅਗਲਾ = new;

    ਵਾਪਸ m;
}


ਸਥਿਰ ਖਾਲੀ *
ngx_palloc_large(ngx_pool_t *pool, ਮਾਪ_t ਮਾਪ)
{
    ਖਾਲੀ              *p;
    ngx_uint_t         n;
    ngx_pool_large_t  *large;

    p = ngx_alloc(ਮਾਪ, pool->ਲਘੂਗਣਕ);
    ਜੇ (p == ਨਿਰਾ) {
        ਵਾਪਸ ਨਿਰਾ;
    }

    n = 0;

    ਲਈ (large = pool->large; large; large = large->ਅਗਲਾ) {
        ਜੇ (large->alloc == ਨਿਰਾ) {
            large->alloc = p;
            ਵਾਪਸ p;
        }

        ਜੇ (n++ > 3) {
            ਰੋਕੋ;
        }
    }

    large = ngx_palloc_small(pool, ਆਕਾਰ(ngx_pool_large_t), 1);
    ਜੇ (large == ਨਿਰਾ) {
        ngx_free(p);
        ਵਾਪਸ ਨਿਰਾ;
    }

    large->alloc = p;
    large->ਅਗਲਾ = pool->large;
    pool->large = large;

    ਵਾਪਸ p;
}


ਖਾਲੀ *
ngx_pmemalign(ngx_pool_t *pool, ਮਾਪ_t ਮਾਪ, ਮਾਪ_t alignment)
{
    ਖਾਲੀ              *p;
    ngx_pool_large_t  *large;

    p = ngx_memalign(alignment, ਮਾਪ, pool->ਲਘੂਗਣਕ);
    ਜੇ (p == ਨਿਰਾ) {
        ਵਾਪਸ ਨਿਰਾ;
    }

    large = ngx_palloc_small(pool, ਆਕਾਰ(ngx_pool_large_t), 1);
    ਜੇ (large == ਨਿਰਾ) {
        ngx_free(p);
        ਵਾਪਸ ਨਿਰਾ;
    }

    large->alloc = p;
    large->ਅਗਲਾ = pool->large;
    pool->large = large;

    ਵਾਪਸ p;
}


ngx_int_t
ngx_pfree(ngx_pool_t *pool, ਖਾਲੀ *p)
{
    ngx_pool_large_t  *l;

    ਲਈ (l = pool->large; l; l = l->ਅਗਲਾ) {
        ਜੇ (p == l->alloc) {
            ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->ਲਘੂਗਣਕ, 0,
                           "free: %p", l->alloc);
            ngx_free(l->alloc);
            l->alloc = ਨਿਰਾ;

            ਵਾਪਸ NGX_OK;
        }
    }

    ਵਾਪਸ NGX_DECLINED;
}


ਖਾਲੀ *
ngx_pcalloc(ngx_pool_t *pool, ਮਾਪ_t ਮਾਪ)
{
    ਖਾਲੀ *p;

    p = ngx_palloc(pool, ਮਾਪ);
    ਜੇ (p) {
        ngx_memzero(p, ਮਾਪ);
    }

    ਵਾਪਸ p;
}


ngx_pool_cleanup_t *
ngx_pool_cleanup_ਜੋੜੋ(ngx_pool_t *p, ਮਾਪ_t ਮਾਪ)
{
    ngx_pool_cleanup_t  *c;

    c = ngx_palloc(p, ਆਕਾਰ(ngx_pool_cleanup_t));
    ਜੇ (c == ਨਿਰਾ) {
        ਵਾਪਸ ਨਿਰਾ;
    }

    ਜੇ (ਮਾਪ) {
        c->ਅੰਕੜੇ = ngx_palloc(p, ਮਾਪ);
        ਜੇ (c->ਅੰਕੜੇ == ਨਿਰਾ) {
            ਵਾਪਸ ਨਿਰਾ;
        }

    } ਨਹੀਂਤਾਂ {
        c->ਅੰਕੜੇ = ਨਿਰਾ;
    }

    c->handler = ਨਿਰਾ;
    c->ਅਗਲਾ = p->cleanup;

    p->cleanup = c;

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, p->ਲਘੂਗਣਕ, 0, "add cleanup: %p", c);

    ਵਾਪਸ c;
}


ਖਾਲੀ
ngx_pool_ਲੜੀਵਾਰ_cleanup_file(ngx_pool_t *p, ngx_fd_t fd)
{
    ngx_pool_cleanup_t       *c;
    ngx_pool_cleanup_file_t  *cf;

    ਲਈ (c = p->cleanup; c; c = c->ਅਗਲਾ) {
        ਜੇ (c->handler == ngx_pool_cleanup_file) {

            cf = c->ਅੰਕੜੇ;

            ਜੇ (cf->fd == fd) {
                c->handler(cf);
                c->handler = ਨਿਰਾ;
                ਵਾਪਸ;
            }
        }
    }
}


ਖਾਲੀ
ngx_pool_cleanup_file(ਖਾਲੀ *ਅੰਕੜੇ)
{
    ngx_pool_cleanup_file_t  *c = ਅੰਕੜੇ;

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, c->ਲਘੂਗਣਕ, 0, "file cleanup: fd:%d",
                   c->fd);

    ਜੇ (ngx_close_file(c->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, c->ਲਘੂਗਣਕ, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", c->name);
    }
}


ਖਾਲੀ
ngx_pool_delete_file(ਖਾਲੀ *ਅੰਕੜੇ)
{
    ngx_pool_cleanup_file_t  *c = ਅੰਕੜੇ;

    ngx_err_t  err;

    ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, c->ਲਘੂਗਣਕ, 0, "file cleanup: fd:%d %s",
                   c->fd, c->name);

    ਜੇ (ngx_delete_file(c->name) == NGX_FILE_ERROR) {
        err = ngx_errno;

        ਜੇ (err != NGX_ENOENT) {
            ngx_log_error(NGX_LOG_CRIT, c->ਲਘੂਗਣਕ, err,
                          ngx_delete_file_n " \"%s\" failed", c->name);
        }
    }

    ਜੇ (ngx_close_file(c->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, c->ਲਘੂਗਣਕ, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", c->name);
    }
}


#ਜੇ 0

ਸਥਿਰ ਖਾਲੀ *
ngx_get_cached_ਖੰਡ(ਮਾਪ_t ਮਾਪ)
{
    ਖਾਲੀ                     *p;
    ngx_cached_ਖੰਡ_ਥਾਂ_t  *ਥਾਂ;

    ਜੇ (ngx_ਚੱਕਰ->cache == ਨਿਰਾ) {
        ਵਾਪਸ ਨਿਰਾ;
    }

    ਥਾਂ = &ngx_ਚੱਕਰ->cache[(ਮਾਪ + ngx_pagesize - 1) / ngx_pagesize];

    ਥਾਂ->tries++;

    ਜੇ (ਥਾਂ->ਸੰਖਿਆ) {
        p = ਥਾਂ->ਖੰਡ;
        ਥਾਂ->ਖੰਡ = ਥਾਂ->ਖੰਡ->ਅਗਲਾ;
        ਥਾਂ->ਸੰਖਿਆ--;
        ਵਾਪਸ p;
    }

    ਵਾਪਸ ਨਿਰਾ;
}

#ਜੇ_ਅੰਤ
