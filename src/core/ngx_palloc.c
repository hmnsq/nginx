
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#شامل <ngx_config.h>
#شامل <ngx_core.h>


ساکن ngx_inline خالی *ngx_palloc_small(ngx_pool_t *pool, سائز_t سائز,
    ngx_uint_t align);
ساکن خالی *ngx_palloc_بلاک(ngx_pool_t *pool, سائز_t سائز);
ساکن خالی *ngx_palloc_large(ngx_pool_t *pool, سائز_t سائز);


ngx_pool_t *
ngx_create_pool(سائز_t سائز, ngx_log_t *لوگارتھم)
{
    ngx_pool_t  *p;

    p = ngx_memalign(NGX_POOL_ALIGNMENT, سائز, لوگارتھم);
    اگر (p == تہی) {
        واپس تہی;
    }

    p->d.آخری = (u_char *) p + حجم(ngx_pool_t);
    p->d.انتہا = (u_char *) p + سائز;
    p->d.اگلا = تہی;
    p->d.failed = 0;

    سائز = سائز - حجم(ngx_pool_t);
    p->max = (سائز < NGX_MAX_ALLOC_FROM_POOL) ? سائز : NGX_MAX_ALLOC_FROM_POOL;

    p->current = p;
    p->chain = تہی;
    p->large = تہی;
    p->cleanup = تہی;
    p->لوگارتھم = لوگارتھم;

    واپس p;
}


خالی
ngx_destroy_pool(ngx_pool_t *pool)
{
    ngx_pool_t          *p, *n;
    ngx_pool_large_t    *l;
    ngx_pool_cleanup_t  *c;

    برائے (c = pool->cleanup; c; c = c->اگلا) {
        اگر (c->handler) {
            ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->لوگارتھم, 0,
                           "run cleanup: %p", c);
            c->handler(c->ڈیٹا);
        }
    }

#اگر (NGX_DEBUG)

    /*
     * we could allocate the pool->log from this pool
     * so we cannot use this log while free()ing the pool
     */

    برائے (l = pool->large; l; l = l->اگلا) {
        ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->لوگارتھم, 0, "free: %p", l->alloc);
    }

    برائے (p = pool, n = pool->d.اگلا; /* void */; p = n, n = n->d.اگلا) {
        ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, pool->لوگارتھم, 0,
                       "free: %p, unused: %uz", p, p->d.انتہا - p->d.آخری);

        اگر (n == تہی) {
            توڑو;
        }
    }

#اگر_ختم

    برائے (l = pool->large; l; l = l->اگلا) {
        اگر (l->alloc) {
            ngx_free(l->alloc);
        }
    }

    برائے (p = pool, n = pool->d.اگلا; /* void */; p = n, n = n->d.اگلا) {
        ngx_free(p);

        اگر (n == تہی) {
            توڑو;
        }
    }
}


خالی
ngx_reset_pool(ngx_pool_t *pool)
{
    ngx_pool_t        *p;
    ngx_pool_large_t  *l;

    برائے (l = pool->large; l; l = l->اگلا) {
        اگر (l->alloc) {
            ngx_free(l->alloc);
        }
    }

    برائے (p = pool; p; p = p->d.اگلا) {
        p->d.آخری = (u_char *) p + حجم(ngx_pool_t);
        p->d.failed = 0;
    }

    pool->current = pool;
    pool->chain = تہی;
    pool->large = تہی;
}


خالی *
ngx_palloc(ngx_pool_t *pool, سائز_t سائز)
{
#اگر !(NGX_DEBUG_PALLOC)
    اگر (سائز <= pool->max) {
        واپس ngx_palloc_small(pool, سائز, 1);
    }
#اگر_ختم

    واپس ngx_palloc_large(pool, سائز);
}


خالی *
ngx_pnalloc(ngx_pool_t *pool, سائز_t سائز)
{
#اگر !(NGX_DEBUG_PALLOC)
    اگر (سائز <= pool->max) {
        واپس ngx_palloc_small(pool, سائز, 0);
    }
#اگر_ختم

    واپس ngx_palloc_large(pool, سائز);
}


ساکن ngx_inline خالی *
ngx_palloc_small(ngx_pool_t *pool, سائز_t سائز, ngx_uint_t align)
{
    u_char      *m;
    ngx_pool_t  *p;

    p = pool->current;

    کرو {
        m = p->d.آخری;

        اگر (align) {
            m = ngx_align_ptr(m, NGX_ALIGNMENT);
        }

        اگر ((سائز_t) (p->d.انتہا - m) >= سائز) {
            p->d.آخری = m + سائز;

            واپس m;
        }

        p = p->d.اگلا;

    } جب (p);

    واپس ngx_palloc_بلاک(pool, سائز);
}


ساکن خالی *
ngx_palloc_بلاک(ngx_pool_t *pool, سائز_t سائز)
{
    u_char      *m;
    سائز_t       psize;
    ngx_pool_t  *p, *new;

    psize = (سائز_t) (pool->d.انتہا - (u_char *) pool);

    m = ngx_memalign(NGX_POOL_ALIGNMENT, psize, pool->لوگارتھم);
    اگر (m == تہی) {
        واپس تہی;
    }

    new = (ngx_pool_t *) m;

    new->d.انتہا = m + psize;
    new->d.اگلا = تہی;
    new->d.failed = 0;

    m += حجم(ngx_pool_ڈیٹا_t);
    m = ngx_align_ptr(m, NGX_ALIGNMENT);
    new->d.آخری = m + سائز;

    برائے (p = pool->current; p->d.اگلا; p = p->d.اگلا) {
        اگر (p->d.failed++ > 4) {
            pool->current = p->d.اگلا;
        }
    }

    p->d.اگلا = new;

    واپس m;
}


ساکن خالی *
ngx_palloc_large(ngx_pool_t *pool, سائز_t سائز)
{
    خالی              *p;
    ngx_uint_t         n;
    ngx_pool_large_t  *large;

    p = ngx_alloc(سائز, pool->لوگارتھم);
    اگر (p == تہی) {
        واپس تہی;
    }

    n = 0;

    برائے (large = pool->large; large; large = large->اگلا) {
        اگر (large->alloc == تہی) {
            large->alloc = p;
            واپس p;
        }

        اگر (n++ > 3) {
            توڑو;
        }
    }

    large = ngx_palloc_small(pool, حجم(ngx_pool_large_t), 1);
    اگر (large == تہی) {
        ngx_free(p);
        واپس تہی;
    }

    large->alloc = p;
    large->اگلا = pool->large;
    pool->large = large;

    واپس p;
}


خالی *
ngx_pmemalign(ngx_pool_t *pool, سائز_t سائز, سائز_t alignment)
{
    خالی              *p;
    ngx_pool_large_t  *large;

    p = ngx_memalign(alignment, سائز, pool->لوگارتھم);
    اگر (p == تہی) {
        واپس تہی;
    }

    large = ngx_palloc_small(pool, حجم(ngx_pool_large_t), 1);
    اگر (large == تہی) {
        ngx_free(p);
        واپس تہی;
    }

    large->alloc = p;
    large->اگلا = pool->large;
    pool->large = large;

    واپس p;
}


ngx_int_t
ngx_pfree(ngx_pool_t *pool, خالی *p)
{
    ngx_pool_large_t  *l;

    برائے (l = pool->large; l; l = l->اگلا) {
        اگر (p == l->alloc) {
            ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->لوگارتھم, 0,
                           "free: %p", l->alloc);
            ngx_free(l->alloc);
            l->alloc = تہی;

            واپس NGX_OK;
        }
    }

    واپس NGX_DECLINED;
}


خالی *
ngx_pcalloc(ngx_pool_t *pool, سائز_t سائز)
{
    خالی *p;

    p = ngx_palloc(pool, سائز);
    اگر (p) {
        ngx_memzero(p, سائز);
    }

    واپس p;
}


ngx_pool_cleanup_t *
ngx_pool_cleanup_جوڑو(ngx_pool_t *p, سائز_t سائز)
{
    ngx_pool_cleanup_t  *c;

    c = ngx_palloc(p, حجم(ngx_pool_cleanup_t));
    اگر (c == تہی) {
        واپس تہی;
    }

    اگر (سائز) {
        c->ڈیٹا = ngx_palloc(p, سائز);
        اگر (c->ڈیٹا == تہی) {
            واپس تہی;
        }

    } ورنہ {
        c->ڈیٹا = تہی;
    }

    c->handler = تہی;
    c->اگلا = p->cleanup;

    p->cleanup = c;

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, p->لوگارتھم, 0, "add cleanup: %p", c);

    واپس c;
}


خالی
ngx_pool_تسلسل_cleanup_file(ngx_pool_t *p, ngx_fd_t fd)
{
    ngx_pool_cleanup_t       *c;
    ngx_pool_cleanup_file_t  *cf;

    برائے (c = p->cleanup; c; c = c->اگلا) {
        اگر (c->handler == ngx_pool_cleanup_file) {

            cf = c->ڈیٹا;

            اگر (cf->fd == fd) {
                c->handler(cf);
                c->handler = تہی;
                واپس;
            }
        }
    }
}


خالی
ngx_pool_cleanup_file(خالی *ڈیٹا)
{
    ngx_pool_cleanup_file_t  *c = ڈیٹا;

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, c->لوگارتھم, 0, "file cleanup: fd:%d",
                   c->fd);

    اگر (ngx_close_file(c->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, c->لوگارتھم, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", c->name);
    }
}


خالی
ngx_pool_delete_file(خالی *ڈیٹا)
{
    ngx_pool_cleanup_file_t  *c = ڈیٹا;

    ngx_err_t  err;

    ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, c->لوگارتھم, 0, "file cleanup: fd:%d %s",
                   c->fd, c->name);

    اگر (ngx_delete_file(c->name) == NGX_FILE_ERROR) {
        err = ngx_errno;

        اگر (err != NGX_ENOENT) {
            ngx_log_error(NGX_LOG_CRIT, c->لوگارتھم, err,
                          ngx_delete_file_n " \"%s\" failed", c->name);
        }
    }

    اگر (ngx_close_file(c->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, c->لوگارتھم, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", c->name);
    }
}


#اگر 0

ساکن خالی *
ngx_get_cached_بلاک(سائز_t سائز)
{
    خالی                     *p;
    ngx_cached_بلاک_خانہ_t  *خانہ;

    اگر (ngx_چکر->cache == تہی) {
        واپس تہی;
    }

    خانہ = &ngx_چکر->cache[(سائز + ngx_pagesize - 1) / ngx_pagesize];

    خانہ->tries++;

    اگر (خانہ->نمبر) {
        p = خانہ->بلاک;
        خانہ->بلاک = خانہ->بلاک->اگلا;
        خانہ->نمبر--;
        واپس p;
    }

    واپس تہی;
}

#اگر_ختم
