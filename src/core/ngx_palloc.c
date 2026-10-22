
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


ساكن ngx_inline فراغ *ngx_palloc_small(ngx_pool_t *pool, size_t size,
    ngx_uint_t align);
ساكن فراغ *ngx_palloc_block(ngx_pool_t *pool, size_t size);
ساكن فراغ *ngx_palloc_large(ngx_pool_t *pool, size_t size);


ngx_pool_t *
ngx_create_pool(size_t size, ngx_log_t *log)
{
    ngx_pool_t  *p;

    p = ngx_memalign(NGX_POOL_ALIGNMENT, size, log);
    إذا (p == عدم) {
        أرجع عدم;
    }

    p->d.last = (u_char *) p + حجم(ngx_pool_t);
    p->d.end = (u_char *) p + size;
    p->d.next = عدم;
    p->d.failed = 0;

    size = size - حجم(ngx_pool_t);
    p->max = (size < NGX_MAX_ALLOC_FROM_POOL) ? size : NGX_MAX_ALLOC_FROM_POOL;

    p->current = p;
    p->chain = عدم;
    p->large = عدم;
    p->cleanup = عدم;
    p->log = log;

    أرجع p;
}


فراغ
ngx_destroy_pool(ngx_pool_t *pool)
{
    ngx_pool_t          *p, *n;
    ngx_pool_large_t    *l;
    ngx_pool_cleanup_t  *c;

    لكل (c = pool->cleanup; c; c = c->next) {
        إذا (c->handler) {
            ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0,
                           "run cleanup: %p", c);
            c->handler(c->data);
        }
    }

#إذا (NGX_DEBUG)

    /*
     * we could allocate the pool->log from this pool
     * so we cannot use this log while free()ing the pool
     */

    لكل (l = pool->large; l; l = l->next) {
        ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0, "free: %p", l->alloc);
    }

    لكل (p = pool, n = pool->d.next; /* void */; p = n, n = n->d.next) {
        ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, pool->log, 0,
                       "free: %p, unused: %uz", p, p->d.end - p->d.last);

        إذا (n == عدم) {
            اكسر;
        }
    }

#endif

    لكل (l = pool->large; l; l = l->next) {
        إذا (l->alloc) {
            ngx_free(l->alloc);
        }
    }

    لكل (p = pool, n = pool->d.next; /* void */; p = n, n = n->d.next) {
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

    لكل (l = pool->large; l; l = l->next) {
        إذا (l->alloc) {
            ngx_free(l->alloc);
        }
    }

    لكل (p = pool; p; p = p->d.next) {
        p->d.last = (u_char *) p + حجم(ngx_pool_t);
        p->d.failed = 0;
    }

    pool->current = pool;
    pool->chain = عدم;
    pool->large = عدم;
}


فراغ *
ngx_palloc(ngx_pool_t *pool, size_t size)
{
#إذا !(NGX_DEBUG_PALLOC)
    إذا (size <= pool->max) {
        أرجع ngx_palloc_small(pool, size, 1);
    }
#endif

    أرجع ngx_palloc_large(pool, size);
}


فراغ *
ngx_pnalloc(ngx_pool_t *pool, size_t size)
{
#إذا !(NGX_DEBUG_PALLOC)
    إذا (size <= pool->max) {
        أرجع ngx_palloc_small(pool, size, 0);
    }
#endif

    أرجع ngx_palloc_large(pool, size);
}


ساكن ngx_inline فراغ *
ngx_palloc_small(ngx_pool_t *pool, size_t size, ngx_uint_t align)
{
    u_char      *m;
    ngx_pool_t  *p;

    p = pool->current;

    افعل {
        m = p->d.last;

        إذا (align) {
            m = ngx_align_ptr(m, NGX_ALIGNMENT);
        }

        إذا ((size_t) (p->d.end - m) >= size) {
            p->d.last = m + size;

            أرجع m;
        }

        p = p->d.next;

    } طالما (p);

    أرجع ngx_palloc_block(pool, size);
}


ساكن فراغ *
ngx_palloc_block(ngx_pool_t *pool, size_t size)
{
    u_char      *m;
    size_t       psize;
    ngx_pool_t  *p, *new;

    psize = (size_t) (pool->d.end - (u_char *) pool);

    m = ngx_memalign(NGX_POOL_ALIGNMENT, psize, pool->log);
    إذا (m == عدم) {
        أرجع عدم;
    }

    new = (ngx_pool_t *) m;

    new->d.end = m + psize;
    new->d.next = عدم;
    new->d.failed = 0;

    m += حجم(ngx_pool_data_t);
    m = ngx_align_ptr(m, NGX_ALIGNMENT);
    new->d.last = m + size;

    لكل (p = pool->current; p->d.next; p = p->d.next) {
        إذا (p->d.failed++ > 4) {
            pool->current = p->d.next;
        }
    }

    p->d.next = new;

    أرجع m;
}


ساكن فراغ *
ngx_palloc_large(ngx_pool_t *pool, size_t size)
{
    فراغ              *p;
    ngx_uint_t         n;
    ngx_pool_large_t  *large;

    p = ngx_alloc(size, pool->log);
    إذا (p == عدم) {
        أرجع عدم;
    }

    n = 0;

    لكل (large = pool->large; large; large = large->next) {
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
    large->next = pool->large;
    pool->large = large;

    أرجع p;
}


فراغ *
ngx_pmemalign(ngx_pool_t *pool, size_t size, size_t alignment)
{
    فراغ              *p;
    ngx_pool_large_t  *large;

    p = ngx_memalign(alignment, size, pool->log);
    إذا (p == عدم) {
        أرجع عدم;
    }

    large = ngx_palloc_small(pool, حجم(ngx_pool_large_t), 1);
    إذا (large == عدم) {
        ngx_free(p);
        أرجع عدم;
    }

    large->alloc = p;
    large->next = pool->large;
    pool->large = large;

    أرجع p;
}


ngx_int_t
ngx_pfree(ngx_pool_t *pool, فراغ *p)
{
    ngx_pool_large_t  *l;

    لكل (l = pool->large; l; l = l->next) {
        إذا (p == l->alloc) {
            ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0,
                           "free: %p", l->alloc);
            ngx_free(l->alloc);
            l->alloc = عدم;

            أرجع NGX_OK;
        }
    }

    أرجع NGX_DECLINED;
}


فراغ *
ngx_pcalloc(ngx_pool_t *pool, size_t size)
{
    فراغ *p;

    p = ngx_palloc(pool, size);
    إذا (p) {
        ngx_memzero(p, size);
    }

    أرجع p;
}


ngx_pool_cleanup_t *
ngx_pool_cleanup_add(ngx_pool_t *p, size_t size)
{
    ngx_pool_cleanup_t  *c;

    c = ngx_palloc(p, حجم(ngx_pool_cleanup_t));
    إذا (c == عدم) {
        أرجع عدم;
    }

    إذا (size) {
        c->data = ngx_palloc(p, size);
        إذا (c->data == عدم) {
            أرجع عدم;
        }

    } وإلا {
        c->data = عدم;
    }

    c->handler = عدم;
    c->next = p->cleanup;

    p->cleanup = c;

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, p->log, 0, "add cleanup: %p", c);

    أرجع c;
}


فراغ
ngx_pool_run_cleanup_file(ngx_pool_t *p, ngx_fd_t fd)
{
    ngx_pool_cleanup_t       *c;
    ngx_pool_cleanup_file_t  *cf;

    لكل (c = p->cleanup; c; c = c->next) {
        إذا (c->handler == ngx_pool_cleanup_file) {

            cf = c->data;

            إذا (cf->fd == fd) {
                c->handler(cf);
                c->handler = عدم;
                أرجع;
            }
        }
    }
}


فراغ
ngx_pool_cleanup_file(فراغ *data)
{
    ngx_pool_cleanup_file_t  *c = data;

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, c->log, 0, "file cleanup: fd:%d",
                   c->fd);

    إذا (ngx_close_file(c->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, c->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", c->name);
    }
}


فراغ
ngx_pool_delete_file(فراغ *data)
{
    ngx_pool_cleanup_file_t  *c = data;

    ngx_err_t  err;

    ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, c->log, 0, "file cleanup: fd:%d %s",
                   c->fd, c->name);

    إذا (ngx_delete_file(c->name) == NGX_FILE_ERROR) {
        err = ngx_errno;

        إذا (err != NGX_ENOENT) {
            ngx_log_error(NGX_LOG_CRIT, c->log, err,
                          ngx_delete_file_n " \"%s\" failed", c->name);
        }
    }

    إذا (ngx_close_file(c->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, c->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", c->name);
    }
}


#إذا 0

ساكن فراغ *
ngx_get_cached_block(size_t size)
{
    فراغ                     *p;
    ngx_cached_block_slot_t  *slot;

    إذا (ngx_cycle->cache == عدم) {
        أرجع عدم;
    }

    slot = &ngx_cycle->cache[(size + ngx_pagesize - 1) / ngx_pagesize];

    slot->tries++;

    إذا (slot->ژمار) {
        p = slot->block;
        slot->block = slot->block->next;
        slot->ژمار--;
        أرجع p;
    }

    أرجع عدم;
}

#endif
