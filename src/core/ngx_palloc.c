
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


ساكن ngx_inline فراغ *ngx_palloc_small(ngx_pool_t *pool, سائيز_t سائيز,
    ngx_uint_t align);
ساكن فراغ *ngx_palloc_بلاڪ(ngx_pool_t *pool, سائيز_t سائيز);
ساكن فراغ *ngx_palloc_large(ngx_pool_t *pool, سائيز_t سائيز);


ngx_pool_t *
ngx_create_pool(سائيز_t سائيز, ngx_log_t *لاگرٿم)
{
    ngx_pool_t  *p;

    p = ngx_memalign(NGX_POOL_ALIGNMENT, سائيز, لاگرٿم);
    إذا (p == عدم) {
        أرجع عدم;
    }

    p->d.آخري = (u_char *) p + حجم(ngx_pool_t);
    p->d.پڇاڙي = (u_char *) p + سائيز;
    p->d.ايندڙ = عدم;
    p->d.failed = 0;

    سائيز = سائيز - حجم(ngx_pool_t);
    p->max = (سائيز < NGX_MAX_ALLOC_FROM_POOL) ? سائيز : NGX_MAX_ALLOC_FROM_POOL;

    p->current = p;
    p->chain = عدم;
    p->large = عدم;
    p->cleanup = عدم;
    p->لاگرٿم = لاگرٿم;

    أرجع p;
}


فراغ
ngx_destroy_pool(ngx_pool_t *pool)
{
    ngx_pool_t          *p, *n;
    ngx_pool_large_t    *l;
    ngx_pool_cleanup_t  *c;

    لكل (c = pool->cleanup; c; c = c->ايندڙ) {
        إذا (c->handler) {
            ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->لاگرٿم, 0,
                           "run cleanup: %p", c);
            c->handler(c->معلومات);
        }
    }

#إذا (NGX_DEBUG)

    /*
     * we could allocate the pool->log from this pool
     * so we cannot use this log while free()ing the pool
     */

    لكل (l = pool->large; l; l = l->ايندڙ) {
        ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->لاگرٿم, 0, "free: %p", l->alloc);
    }

    لكل (p = pool, n = pool->d.ايندڙ; /* void */; p = n, n = n->d.ايندڙ) {
        ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, pool->لاگرٿم, 0,
                       "free: %p, unused: %uz", p, p->d.پڇاڙي - p->d.آخري);

        إذا (n == عدم) {
            اكسر;
        }
    }

#جيڪڏهن_پورو

    لكل (l = pool->large; l; l = l->ايندڙ) {
        إذا (l->alloc) {
            ngx_free(l->alloc);
        }
    }

    لكل (p = pool, n = pool->d.ايندڙ; /* void */; p = n, n = n->d.ايندڙ) {
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

    لكل (l = pool->large; l; l = l->ايندڙ) {
        إذا (l->alloc) {
            ngx_free(l->alloc);
        }
    }

    لكل (p = pool; p; p = p->d.ايندڙ) {
        p->d.آخري = (u_char *) p + حجم(ngx_pool_t);
        p->d.failed = 0;
    }

    pool->current = pool;
    pool->chain = عدم;
    pool->large = عدم;
}


فراغ *
ngx_palloc(ngx_pool_t *pool, سائيز_t سائيز)
{
#إذا !(NGX_DEBUG_PALLOC)
    إذا (سائيز <= pool->max) {
        أرجع ngx_palloc_small(pool, سائيز, 1);
    }
#جيڪڏهن_پورو

    أرجع ngx_palloc_large(pool, سائيز);
}


فراغ *
ngx_pnalloc(ngx_pool_t *pool, سائيز_t سائيز)
{
#إذا !(NGX_DEBUG_PALLOC)
    إذا (سائيز <= pool->max) {
        أرجع ngx_palloc_small(pool, سائيز, 0);
    }
#جيڪڏهن_پورو

    أرجع ngx_palloc_large(pool, سائيز);
}


ساكن ngx_inline فراغ *
ngx_palloc_small(ngx_pool_t *pool, سائيز_t سائيز, ngx_uint_t align)
{
    u_char      *m;
    ngx_pool_t  *p;

    p = pool->current;

    افعل {
        m = p->d.آخري;

        إذا (align) {
            m = ngx_align_ptr(m, NGX_ALIGNMENT);
        }

        إذا ((سائيز_t) (p->d.پڇاڙي - m) >= سائيز) {
            p->d.آخري = m + سائيز;

            أرجع m;
        }

        p = p->d.ايندڙ;

    } طالما (p);

    أرجع ngx_palloc_بلاڪ(pool, سائيز);
}


ساكن فراغ *
ngx_palloc_بلاڪ(ngx_pool_t *pool, سائيز_t سائيز)
{
    u_char      *m;
    سائيز_t       psize;
    ngx_pool_t  *p, *new;

    psize = (سائيز_t) (pool->d.پڇاڙي - (u_char *) pool);

    m = ngx_memalign(NGX_POOL_ALIGNMENT, psize, pool->لاگرٿم);
    إذا (m == عدم) {
        أرجع عدم;
    }

    new = (ngx_pool_t *) m;

    new->d.پڇاڙي = m + psize;
    new->d.ايندڙ = عدم;
    new->d.failed = 0;

    m += حجم(ngx_pool_معلومات_t);
    m = ngx_align_ptr(m, NGX_ALIGNMENT);
    new->d.آخري = m + سائيز;

    لكل (p = pool->current; p->d.ايندڙ; p = p->d.ايندڙ) {
        إذا (p->d.failed++ > 4) {
            pool->current = p->d.ايندڙ;
        }
    }

    p->d.ايندڙ = new;

    أرجع m;
}


ساكن فراغ *
ngx_palloc_large(ngx_pool_t *pool, سائيز_t سائيز)
{
    فراغ              *p;
    ngx_uint_t         n;
    ngx_pool_large_t  *large;

    p = ngx_alloc(سائيز, pool->لاگرٿم);
    إذا (p == عدم) {
        أرجع عدم;
    }

    n = 0;

    لكل (large = pool->large; large; large = large->ايندڙ) {
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
    large->ايندڙ = pool->large;
    pool->large = large;

    أرجع p;
}


فراغ *
ngx_pmemalign(ngx_pool_t *pool, سائيز_t سائيز, سائيز_t alignment)
{
    فراغ              *p;
    ngx_pool_large_t  *large;

    p = ngx_memalign(alignment, سائيز, pool->لاگرٿم);
    إذا (p == عدم) {
        أرجع عدم;
    }

    large = ngx_palloc_small(pool, حجم(ngx_pool_large_t), 1);
    إذا (large == عدم) {
        ngx_free(p);
        أرجع عدم;
    }

    large->alloc = p;
    large->ايندڙ = pool->large;
    pool->large = large;

    أرجع p;
}


ngx_int_t
ngx_pfree(ngx_pool_t *pool, فراغ *p)
{
    ngx_pool_large_t  *l;

    لكل (l = pool->large; l; l = l->ايندڙ) {
        إذا (p == l->alloc) {
            ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->لاگرٿم, 0,
                           "free: %p", l->alloc);
            ngx_free(l->alloc);
            l->alloc = عدم;

            أرجع NGX_OK;
        }
    }

    أرجع NGX_DECLINED;
}


فراغ *
ngx_pcalloc(ngx_pool_t *pool, سائيز_t سائيز)
{
    فراغ *p;

    p = ngx_palloc(pool, سائيز);
    إذا (p) {
        ngx_memzero(p, سائيز);
    }

    أرجع p;
}


ngx_pool_cleanup_t *
ngx_pool_cleanup_جوڙيو(ngx_pool_t *p, سائيز_t سائيز)
{
    ngx_pool_cleanup_t  *c;

    c = ngx_palloc(p, حجم(ngx_pool_cleanup_t));
    إذا (c == عدم) {
        أرجع عدم;
    }

    إذا (سائيز) {
        c->معلومات = ngx_palloc(p, سائيز);
        إذا (c->معلومات == عدم) {
            أرجع عدم;
        }

    } وإلا {
        c->معلومات = عدم;
    }

    c->handler = عدم;
    c->ايندڙ = p->cleanup;

    p->cleanup = c;

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, p->لاگرٿم, 0, "add cleanup: %p", c);

    أرجع c;
}


فراغ
ngx_pool_لاڳيتو_cleanup_file(ngx_pool_t *p, ngx_fd_t fd)
{
    ngx_pool_cleanup_t       *c;
    ngx_pool_cleanup_file_t  *cf;

    لكل (c = p->cleanup; c; c = c->ايندڙ) {
        إذا (c->handler == ngx_pool_cleanup_file) {

            cf = c->معلومات;

            إذا (cf->fd == fd) {
                c->handler(cf);
                c->handler = عدم;
                أرجع;
            }
        }
    }
}


فراغ
ngx_pool_cleanup_file(فراغ *معلومات)
{
    ngx_pool_cleanup_file_t  *c = معلومات;

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, c->لاگرٿم, 0, "file cleanup: fd:%d",
                   c->fd);

    إذا (ngx_close_file(c->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, c->لاگرٿم, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", c->name);
    }
}


فراغ
ngx_pool_delete_file(فراغ *معلومات)
{
    ngx_pool_cleanup_file_t  *c = معلومات;

    ngx_err_t  err;

    ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, c->لاگرٿم, 0, "file cleanup: fd:%d %s",
                   c->fd, c->name);

    إذا (ngx_delete_file(c->name) == NGX_FILE_ERROR) {
        err = ngx_errno;

        إذا (err != NGX_ENOENT) {
            ngx_log_error(NGX_LOG_CRIT, c->لاگرٿم, err,
                          ngx_delete_file_n " \"%s\" failed", c->name);
        }
    }

    إذا (ngx_close_file(c->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, c->لاگرٿم, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", c->name);
    }
}


#إذا 0

ساكن فراغ *
ngx_get_cached_بلاڪ(سائيز_t سائيز)
{
    فراغ                     *p;
    ngx_cached_بلاڪ_جڳهه_t  *جڳهه;

    إذا (ngx_چڪر->cache == عدم) {
        أرجع عدم;
    }

    جڳهه = &ngx_چڪر->cache[(سائيز + ngx_pagesize - 1) / ngx_pagesize];

    جڳهه->tries++;

    إذا (جڳهه->عدد) {
        p = جڳهه->بلاڪ;
        جڳهه->بلاڪ = جڳهه->بلاڪ->ايندڙ;
        جڳهه->عدد--;
        أرجع p;
    }

    أرجع عدم;
}

#جيڪڏهن_پورو
