
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


ساكن ngx_inline فراغ *ngx_palloc_small(ngx_pool_t *pool, ناپ_t ناپ,
    ngx_uint_t align);
ساكن فراغ *ngx_palloc_بلاک(ngx_pool_t *pool, ناپ_t ناپ);
ساكن فراغ *ngx_palloc_large(ngx_pool_t *pool, ناپ_t ناپ);


ngx_pool_t *
ngx_create_pool(ناپ_t ناپ, ngx_log_t *لوگارتھم)
{
    ngx_pool_t  *p;

    p = ngx_memalign(NGX_POOL_ALIGNMENT, ناپ, لوگارتھم);
    إذا (p == عدم) {
        أرجع عدم;
    }

    p->d.آخری = (u_char *) p + حجم(ngx_pool_t);
    p->d.سرا = (u_char *) p + ناپ;
    p->d.اگلا = عدم;
    p->d.failed = 0;

    ناپ = ناپ - حجم(ngx_pool_t);
    p->max = (ناپ < NGX_MAX_ALLOC_FROM_POOL) ? ناپ : NGX_MAX_ALLOC_FROM_POOL;

    p->current = p;
    p->chain = عدم;
    p->large = عدم;
    p->cleanup = عدم;
    p->لوگارتھم = لوگارتھم;

    أرجع p;
}


فراغ
ngx_destroy_pool(ngx_pool_t *pool)
{
    ngx_pool_t          *p, *n;
    ngx_pool_large_t    *l;
    ngx_pool_cleanup_t  *c;

    لكل (c = pool->cleanup; c; c = c->اگلا) {
        إذا (c->handler) {
            ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->لوگارتھم, 0,
                           "run cleanup: %p", c);
            c->handler(c->انکڑے);
        }
    }

#إذا (NGX_DEBUG)

    /*
     * we could allocate the pool->log from this pool
     * so we cannot use this log while free()ing the pool
     */

    لكل (l = pool->large; l; l = l->اگلا) {
        ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->لوگارتھم, 0, "free: %p", l->alloc);
    }

    لكل (p = pool, n = pool->d.اگلا; /* void */; p = n, n = n->d.اگلا) {
        ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, pool->لوگارتھم, 0,
                       "free: %p, unused: %uz", p, p->d.سرا - p->d.آخری);

        إذا (n == عدم) {
            اكسر;
        }
    }

#جے_انت

    لكل (l = pool->large; l; l = l->اگلا) {
        إذا (l->alloc) {
            ngx_free(l->alloc);
        }
    }

    لكل (p = pool, n = pool->d.اگلا; /* void */; p = n, n = n->d.اگلا) {
        ngx_free(p);

        إذا (n == عدم) {
            اكسر;
        }
    }
}


فراغ
ngx_reset_pool(ngx_pool_t *pool)
{
    ngx_pool_t        *p;
    ngx_pool_large_t  *l;

    لكل (l = pool->large; l; l = l->اگلا) {
        إذا (l->alloc) {
            ngx_free(l->alloc);
        }
    }

    لكل (p = pool; p; p = p->d.اگلا) {
        p->d.آخری = (u_char *) p + حجم(ngx_pool_t);
        p->d.failed = 0;
    }

    pool->current = pool;
    pool->chain = عدم;
    pool->large = عدم;
}


فراغ *
ngx_palloc(ngx_pool_t *pool, ناپ_t ناپ)
{
#إذا !(NGX_DEBUG_PALLOC)
    إذا (ناپ <= pool->max) {
        أرجع ngx_palloc_small(pool, ناپ, 1);
    }
#جے_انت

    أرجع ngx_palloc_large(pool, ناپ);
}


فراغ *
ngx_pnalloc(ngx_pool_t *pool, ناپ_t ناپ)
{
#إذا !(NGX_DEBUG_PALLOC)
    إذا (ناپ <= pool->max) {
        أرجع ngx_palloc_small(pool, ناپ, 0);
    }
#جے_انت

    أرجع ngx_palloc_large(pool, ناپ);
}


ساكن ngx_inline فراغ *
ngx_palloc_small(ngx_pool_t *pool, ناپ_t ناپ, ngx_uint_t align)
{
    u_char      *m;
    ngx_pool_t  *p;

    p = pool->current;

    افعل {
        m = p->d.آخری;

        إذا (align) {
            m = ngx_align_ptr(m, NGX_ALIGNMENT);
        }

        إذا ((ناپ_t) (p->d.سرا - m) >= ناپ) {
            p->d.آخری = m + ناپ;

            أرجع m;
        }

        p = p->d.اگلا;

    } طالما (p);

    أرجع ngx_palloc_بلاک(pool, ناپ);
}


ساكن فراغ *
ngx_palloc_بلاک(ngx_pool_t *pool, ناپ_t ناپ)
{
    u_char      *m;
    ناپ_t       psize;
    ngx_pool_t  *p, *new;

    psize = (ناپ_t) (pool->d.سرا - (u_char *) pool);

    m = ngx_memalign(NGX_POOL_ALIGNMENT, psize, pool->لوگارتھم);
    إذا (m == عدم) {
        أرجع عدم;
    }

    new = (ngx_pool_t *) m;

    new->d.سرا = m + psize;
    new->d.اگلا = عدم;
    new->d.failed = 0;

    m += حجم(ngx_pool_انکڑے_t);
    m = ngx_align_ptr(m, NGX_ALIGNMENT);
    new->d.آخری = m + ناپ;

    لكل (p = pool->current; p->d.اگلا; p = p->d.اگلا) {
        إذا (p->d.failed++ > 4) {
            pool->current = p->d.اگلا;
        }
    }

    p->d.اگلا = new;

    أرجع m;
}


ساكن فراغ *
ngx_palloc_large(ngx_pool_t *pool, ناپ_t ناپ)
{
    فراغ              *p;
    ngx_uint_t         n;
    ngx_pool_large_t  *large;

    p = ngx_alloc(ناپ, pool->لوگارتھم);
    إذا (p == عدم) {
        أرجع عدم;
    }

    n = 0;

    لكل (large = pool->large; large; large = large->اگلا) {
        إذا (large->alloc == عدم) {
            large->alloc = p;
            أرجع p;
        }

        إذا (n++ > 3) {
            اكسر;
        }
    }

    large = ngx_palloc_small(pool, حجم(ngx_pool_large_t), 1);
    إذا (large == عدم) {
        ngx_free(p);
        أرجع عدم;
    }

    large->alloc = p;
    large->اگلا = pool->large;
    pool->large = large;

    أرجع p;
}


فراغ *
ngx_pmemalign(ngx_pool_t *pool, ناپ_t ناپ, ناپ_t alignment)
{
    فراغ              *p;
    ngx_pool_large_t  *large;

    p = ngx_memalign(alignment, ناپ, pool->لوگارتھم);
    إذا (p == عدم) {
        أرجع عدم;
    }

    large = ngx_palloc_small(pool, حجم(ngx_pool_large_t), 1);
    إذا (large == عدم) {
        ngx_free(p);
        أرجع عدم;
    }

    large->alloc = p;
    large->اگلا = pool->large;
    pool->large = large;

    أرجع p;
}


ngx_int_t
ngx_pfree(ngx_pool_t *pool, فراغ *p)
{
    ngx_pool_large_t  *l;

    لكل (l = pool->large; l; l = l->اگلا) {
        إذا (p == l->alloc) {
            ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->لوگارتھم, 0,
                           "free: %p", l->alloc);
            ngx_free(l->alloc);
            l->alloc = عدم;

            أرجع NGX_OK;
        }
    }

    أرجع NGX_DECLINED;
}


فراغ *
ngx_pcalloc(ngx_pool_t *pool, ناپ_t ناپ)
{
    فراغ *p;

    p = ngx_palloc(pool, ناپ);
    إذا (p) {
        ngx_memzero(p, ناپ);
    }

    أرجع p;
}


ngx_pool_cleanup_t *
ngx_pool_cleanup_جوڑو(ngx_pool_t *p, ناپ_t ناپ)
{
    ngx_pool_cleanup_t  *c;

    c = ngx_palloc(p, حجم(ngx_pool_cleanup_t));
    إذا (c == عدم) {
        أرجع عدم;
    }

    إذا (ناپ) {
        c->انکڑے = ngx_palloc(p, ناپ);
        إذا (c->انکڑے == عدم) {
            أرجع عدم;
        }

    } وإلا {
        c->انکڑے = عدم;
    }

    c->handler = عدم;
    c->اگلا = p->cleanup;

    p->cleanup = c;

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, p->لوگارتھم, 0, "add cleanup: %p", c);

    أرجع c;
}


فراغ
ngx_pool_لگاتار_cleanup_file(ngx_pool_t *p, ngx_fd_t fd)
{
    ngx_pool_cleanup_t       *c;
    ngx_pool_cleanup_file_t  *cf;

    لكل (c = p->cleanup; c; c = c->اگلا) {
        إذا (c->handler == ngx_pool_cleanup_file) {

            cf = c->انکڑے;

            إذا (cf->fd == fd) {
                c->handler(cf);
                c->handler = عدم;
                أرجع;
            }
        }
    }
}


فراغ
ngx_pool_cleanup_file(فراغ *انکڑے)
{
    ngx_pool_cleanup_file_t  *c = انکڑے;

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, c->لوگارتھم, 0, "file cleanup: fd:%d",
                   c->fd);

    إذا (ngx_close_file(c->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, c->لوگارتھم, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", c->name);
    }
}


فراغ
ngx_pool_delete_file(فراغ *انکڑے)
{
    ngx_pool_cleanup_file_t  *c = انکڑے;

    ngx_err_t  err;

    ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, c->لوگارتھم, 0, "file cleanup: fd:%d %s",
                   c->fd, c->name);

    إذا (ngx_delete_file(c->name) == NGX_FILE_ERROR) {
        err = ngx_errno;

        إذا (err != NGX_ENOENT) {
            ngx_log_error(NGX_LOG_CRIT, c->لوگارتھم, err,
                          ngx_delete_file_n " \"%s\" failed", c->name);
        }
    }

    إذا (ngx_close_file(c->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, c->لوگارتھم, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", c->name);
    }
}


#إذا 0

ساكن فراغ *
ngx_get_cached_بلاک(ناپ_t ناپ)
{
    فراغ                     *p;
    ngx_cached_بلاک_تھاں_t  *تھاں;

    إذا (ngx_چکر->cache == عدم) {
        أرجع عدم;
    }

    تھاں = &ngx_چکر->cache[(ناپ + ngx_pagesize - 1) / ngx_pagesize];

    تھاں->tries++;

    إذا (تھاں->نمبر) {
        p = تھاں->بلاک;
        تھاں->بلاک = تھاں->بلاک->اگلا;
        تھاں->نمبر--;
        أرجع p;
    }

    أرجع عدم;
}

#جے_انت
