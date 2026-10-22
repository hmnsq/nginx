
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


ساكن ngx_inline فراغ *ngx_palloc_small(ngx_pool_t *pool, اندازه_t اندازه,
    ngx_uint_t align);
ساكن فراغ *ngx_palloc_بلوک(ngx_pool_t *pool, اندازه_t اندازه);
ساكن فراغ *ngx_palloc_large(ngx_pool_t *pool, اندازه_t اندازه);


ngx_pool_t *
ngx_create_pool(اندازه_t اندازه, ngx_log_t *لگاریتم)
{
    ngx_pool_t  *p;

    p = ngx_memalign(NGX_POOL_ALIGNMENT, اندازه, لگاریتم);
    إذا (p == عدم) {
        أرجع عدم;
    }

    p->d.آخرین = (u_char *) p + حجم(ngx_pool_t);
    p->d.انتها = (u_char *) p + اندازه;
    p->d.بعدی = عدم;
    p->d.failed = 0;

    اندازه = اندازه - حجم(ngx_pool_t);
    p->max = (اندازه < NGX_MAX_ALLOC_FROM_POOL) ? اندازه : NGX_MAX_ALLOC_FROM_POOL;

    p->current = p;
    p->chain = عدم;
    p->large = عدم;
    p->cleanup = عدم;
    p->لگاریتم = لگاریتم;

    أرجع p;
}


فراغ
ngx_destroy_pool(ngx_pool_t *pool)
{
    ngx_pool_t          *p, *n;
    ngx_pool_large_t    *l;
    ngx_pool_cleanup_t  *c;

    لكل (c = pool->cleanup; c; c = c->بعدی) {
        إذا (c->handler) {
            ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->لگاریتم, 0,
                           "run cleanup: %p", c);
            c->handler(c->داده);
        }
    }

#إذا (NGX_DEBUG)

    /*
     * we could allocate the pool->log from this pool
     * so we cannot use this log while free()ing the pool
     */

    لكل (l = pool->large; l; l = l->بعدی) {
        ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->لگاریتم, 0, "free: %p", l->alloc);
    }

    لكل (p = pool, n = pool->d.بعدی; /* void */; p = n, n = n->d.بعدی) {
        ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, pool->لگاریتم, 0,
                       "free: %p, unused: %uz", p, p->d.انتها - p->d.آخرین);

        إذا (n == عدم) {
            اكسر;
        }
    }

#پایان_اگر

    لكل (l = pool->large; l; l = l->بعدی) {
        إذا (l->alloc) {
            ngx_free(l->alloc);
        }
    }

    لكل (p = pool, n = pool->d.بعدی; /* void */; p = n, n = n->d.بعدی) {
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

    لكل (l = pool->large; l; l = l->بعدی) {
        إذا (l->alloc) {
            ngx_free(l->alloc);
        }
    }

    لكل (p = pool; p; p = p->d.بعدی) {
        p->d.آخرین = (u_char *) p + حجم(ngx_pool_t);
        p->d.failed = 0;
    }

    pool->current = pool;
    pool->chain = عدم;
    pool->large = عدم;
}


فراغ *
ngx_palloc(ngx_pool_t *pool, اندازه_t اندازه)
{
#إذا !(NGX_DEBUG_PALLOC)
    إذا (اندازه <= pool->max) {
        أرجع ngx_palloc_small(pool, اندازه, 1);
    }
#پایان_اگر

    أرجع ngx_palloc_large(pool, اندازه);
}


فراغ *
ngx_pnalloc(ngx_pool_t *pool, اندازه_t اندازه)
{
#إذا !(NGX_DEBUG_PALLOC)
    إذا (اندازه <= pool->max) {
        أرجع ngx_palloc_small(pool, اندازه, 0);
    }
#پایان_اگر

    أرجع ngx_palloc_large(pool, اندازه);
}


ساكن ngx_inline فراغ *
ngx_palloc_small(ngx_pool_t *pool, اندازه_t اندازه, ngx_uint_t align)
{
    u_char      *m;
    ngx_pool_t  *p;

    p = pool->current;

    افعل {
        m = p->d.آخرین;

        إذا (align) {
            m = ngx_align_ptr(m, NGX_ALIGNMENT);
        }

        إذا ((اندازه_t) (p->d.انتها - m) >= اندازه) {
            p->d.آخرین = m + اندازه;

            أرجع m;
        }

        p = p->d.بعدی;

    } طالما (p);

    أرجع ngx_palloc_بلوک(pool, اندازه);
}


ساكن فراغ *
ngx_palloc_بلوک(ngx_pool_t *pool, اندازه_t اندازه)
{
    u_char      *m;
    اندازه_t       psize;
    ngx_pool_t  *p, *new;

    psize = (اندازه_t) (pool->d.انتها - (u_char *) pool);

    m = ngx_memalign(NGX_POOL_ALIGNMENT, psize, pool->لگاریتم);
    إذا (m == عدم) {
        أرجع عدم;
    }

    new = (ngx_pool_t *) m;

    new->d.انتها = m + psize;
    new->d.بعدی = عدم;
    new->d.failed = 0;

    m += حجم(ngx_pool_داده_t);
    m = ngx_align_ptr(m, NGX_ALIGNMENT);
    new->d.آخرین = m + اندازه;

    لكل (p = pool->current; p->d.بعدی; p = p->d.بعدی) {
        إذا (p->d.failed++ > 4) {
            pool->current = p->d.بعدی;
        }
    }

    p->d.بعدی = new;

    أرجع m;
}


ساكن فراغ *
ngx_palloc_large(ngx_pool_t *pool, اندازه_t اندازه)
{
    فراغ              *p;
    ngx_uint_t         n;
    ngx_pool_large_t  *large;

    p = ngx_alloc(اندازه, pool->لگاریتم);
    إذا (p == عدم) {
        أرجع عدم;
    }

    n = 0;

    لكل (large = pool->large; large; large = large->بعدی) {
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
    large->بعدی = pool->large;
    pool->large = large;

    أرجع p;
}


فراغ *
ngx_pmemalign(ngx_pool_t *pool, اندازه_t اندازه, اندازه_t alignment)
{
    فراغ              *p;
    ngx_pool_large_t  *large;

    p = ngx_memalign(alignment, اندازه, pool->لگاریتم);
    إذا (p == عدم) {
        أرجع عدم;
    }

    large = ngx_palloc_small(pool, حجم(ngx_pool_large_t), 1);
    إذا (large == عدم) {
        ngx_free(p);
        أرجع عدم;
    }

    large->alloc = p;
    large->بعدی = pool->large;
    pool->large = large;

    أرجع p;
}


ngx_int_t
ngx_pfree(ngx_pool_t *pool, فراغ *p)
{
    ngx_pool_large_t  *l;

    لكل (l = pool->large; l; l = l->بعدی) {
        إذا (p == l->alloc) {
            ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->لگاریتم, 0,
                           "free: %p", l->alloc);
            ngx_free(l->alloc);
            l->alloc = عدم;

            أرجع NGX_OK;
        }
    }

    أرجع NGX_DECLINED;
}


فراغ *
ngx_pcalloc(ngx_pool_t *pool, اندازه_t اندازه)
{
    فراغ *p;

    p = ngx_palloc(pool, اندازه);
    إذا (p) {
        ngx_memzero(p, اندازه);
    }

    أرجع p;
}


ngx_pool_cleanup_t *
ngx_pool_cleanup_جمع_کن(ngx_pool_t *p, اندازه_t اندازه)
{
    ngx_pool_cleanup_t  *c;

    c = ngx_palloc(p, حجم(ngx_pool_cleanup_t));
    إذا (c == عدم) {
        أرجع عدم;
    }

    إذا (اندازه) {
        c->داده = ngx_palloc(p, اندازه);
        إذا (c->داده == عدم) {
            أرجع عدم;
        }

    } وإلا {
        c->داده = عدم;
    }

    c->handler = عدم;
    c->بعدی = p->cleanup;

    p->cleanup = c;

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, p->لگاریتم, 0, "add cleanup: %p", c);

    أرجع c;
}


فراغ
ngx_pool_پیاپی_cleanup_file(ngx_pool_t *p, ngx_fd_t fd)
{
    ngx_pool_cleanup_t       *c;
    ngx_pool_cleanup_file_t  *cf;

    لكل (c = p->cleanup; c; c = c->بعدی) {
        إذا (c->handler == ngx_pool_cleanup_file) {

            cf = c->داده;

            إذا (cf->fd == fd) {
                c->handler(cf);
                c->handler = عدم;
                أرجع;
            }
        }
    }
}


فراغ
ngx_pool_cleanup_file(فراغ *داده)
{
    ngx_pool_cleanup_file_t  *c = داده;

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, c->لگاریتم, 0, "file cleanup: fd:%d",
                   c->fd);

    إذا (ngx_close_file(c->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, c->لگاریتم, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", c->name);
    }
}


فراغ
ngx_pool_delete_file(فراغ *داده)
{
    ngx_pool_cleanup_file_t  *c = داده;

    ngx_err_t  err;

    ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, c->لگاریتم, 0, "file cleanup: fd:%d %s",
                   c->fd, c->name);

    إذا (ngx_delete_file(c->name) == NGX_FILE_ERROR) {
        err = ngx_errno;

        إذا (err != NGX_ENOENT) {
            ngx_log_error(NGX_LOG_CRIT, c->لگاریتم, err,
                          ngx_delete_file_n " \"%s\" failed", c->name);
        }
    }

    إذا (ngx_close_file(c->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, c->لگاریتم, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", c->name);
    }
}


#إذا 0

ساكن فراغ *
ngx_get_cached_بلوک(اندازه_t اندازه)
{
    فراغ                     *p;
    ngx_cached_بلوک_جایگاه_t  *جایگاه;

    إذا (ngx_چرخه->cache == عدم) {
        أرجع عدم;
    }

    جایگاه = &ngx_چرخه->cache[(اندازه + ngx_pagesize - 1) / ngx_pagesize];

    جایگاه->tries++;

    إذا (جایگاه->عدد) {
        p = جایگاه->بلوک;
        جایگاه->بلوک = جایگاه->بلوک->بعدی;
        جایگاه->عدد--;
        أرجع p;
    }

    أرجع عدم;
}

#پایان_اگر
