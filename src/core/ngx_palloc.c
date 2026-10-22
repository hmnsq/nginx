
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#شامل <ngx_config.h>
#شامل <ngx_core.h>


ایستا ngx_inline تهی *ngx_palloc_small(ngx_pool_t *pool, size_t size,
    ngx_uint_t align);
ایستا تهی *ngx_palloc_block(ngx_pool_t *pool, size_t size);
ایستا تهی *ngx_palloc_large(ngx_pool_t *pool, size_t size);


ngx_pool_t *
ngx_create_pool(size_t size, ngx_log_t *log)
{
    ngx_pool_t  *p;

    p = ngx_memalign(NGX_POOL_ALIGNMENT, size, log);
    اگر (p == پوچ) {
        بازگشت پوچ;
    }

    p->d.last = (u_char *) p + اندازه(ngx_pool_t);
    p->d.end = (u_char *) p + size;
    p->d.next = پوچ;
    p->d.failed = 0;

    size = size - اندازه(ngx_pool_t);
    p->max = (size < NGX_MAX_ALLOC_FROM_POOL) ? size : NGX_MAX_ALLOC_FROM_POOL;

    p->current = p;
    p->chain = پوچ;
    p->large = پوچ;
    p->cleanup = پوچ;
    p->log = log;

    بازگشت p;
}


تهی
ngx_destroy_pool(ngx_pool_t *pool)
{
    ngx_pool_t          *p, *n;
    ngx_pool_large_t    *l;
    ngx_pool_cleanup_t  *c;

    برای (c = pool->cleanup; c; c = c->next) {
        اگر (c->handler) {
            ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0,
                           "run cleanup: %p", c);
            c->handler(c->data);
        }
    }

#اگر (NGX_DEBUG)

    /*
     * we could allocate the pool->log from this pool
     * so we cannot use this log while free()ing the pool
     */

    برای (l = pool->large; l; l = l->next) {
        ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0, "free: %p", l->alloc);
    }

    برای (p = pool, n = pool->d.next; /* void */; p = n, n = n->d.next) {
        ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, pool->log, 0,
                       "free: %p, unused: %uz", p, p->d.end - p->d.last);

        اگر (n == پوچ) {
            بشکن;
        }
    }

#endif

    برای (l = pool->large; l; l = l->next) {
        اگر (l->alloc) {
            ngx_free(l->alloc);
        }
    }

    برای (p = pool, n = pool->d.next; /* void */; p = n, n = n->d.next) {
        ngx_free(p);

        اگر (n == پوچ) {
            بشکن;
        }
    }
}


تهی
ngx_reset_pool(ngx_pool_t *pool)
{
    ngx_pool_t        *p;
    ngx_pool_large_t  *l;

    برای (l = pool->large; l; l = l->next) {
        اگر (l->alloc) {
            ngx_free(l->alloc);
        }
    }

    برای (p = pool; p; p = p->d.next) {
        p->d.last = (u_char *) p + اندازه(ngx_pool_t);
        p->d.failed = 0;
    }

    pool->current = pool;
    pool->chain = پوچ;
    pool->large = پوچ;
}


تهی *
ngx_palloc(ngx_pool_t *pool, size_t size)
{
#اگر !(NGX_DEBUG_PALLOC)
    اگر (size <= pool->max) {
        بازگشت ngx_palloc_small(pool, size, 1);
    }
#endif

    بازگشت ngx_palloc_large(pool, size);
}


تهی *
ngx_pnalloc(ngx_pool_t *pool, size_t size)
{
#اگر !(NGX_DEBUG_PALLOC)
    اگر (size <= pool->max) {
        بازگشت ngx_palloc_small(pool, size, 0);
    }
#endif

    بازگشت ngx_palloc_large(pool, size);
}


ایستا ngx_inline تهی *
ngx_palloc_small(ngx_pool_t *pool, size_t size, ngx_uint_t align)
{
    u_char      *m;
    ngx_pool_t  *p;

    p = pool->current;

    انجامبده {
        m = p->d.last;

        اگر (align) {
            m = ngx_align_ptr(m, NGX_ALIGNMENT);
        }

        اگر ((size_t) (p->d.end - m) >= size) {
            p->d.last = m + size;

            بازگشت m;
        }

        p = p->d.next;

    } وقتی (p);

    بازگشت ngx_palloc_block(pool, size);
}


ایستا تهی *
ngx_palloc_block(ngx_pool_t *pool, size_t size)
{
    u_char      *m;
    size_t       psize;
    ngx_pool_t  *p, *new;

    psize = (size_t) (pool->d.end - (u_char *) pool);

    m = ngx_memalign(NGX_POOL_ALIGNMENT, psize, pool->log);
    اگر (m == پوچ) {
        بازگشت پوچ;
    }

    new = (ngx_pool_t *) m;

    new->d.end = m + psize;
    new->d.next = پوچ;
    new->d.failed = 0;

    m += اندازه(ngx_pool_data_t);
    m = ngx_align_ptr(m, NGX_ALIGNMENT);
    new->d.last = m + size;

    برای (p = pool->current; p->d.next; p = p->d.next) {
        اگر (p->d.failed++ > 4) {
            pool->current = p->d.next;
        }
    }

    p->d.next = new;

    بازگشت m;
}


ایستا تهی *
ngx_palloc_large(ngx_pool_t *pool, size_t size)
{
    تهی              *p;
    ngx_uint_t         n;
    ngx_pool_large_t  *large;

    p = ngx_alloc(size, pool->log);
    اگر (p == پوچ) {
        بازگشت پوچ;
    }

    n = 0;

    برای (large = pool->large; large; large = large->next) {
        اگر (large->alloc == پوچ) {
            large->alloc = p;
            بازگشت p;
        }

        اگر (n++ > 3) {
            بشکن;
        }
    }

    large = ngx_palloc_small(pool, اندازه(ngx_pool_large_t), 1);
    اگر (large == پوچ) {
        ngx_free(p);
        بازگشت پوچ;
    }

    large->alloc = p;
    large->next = pool->large;
    pool->large = large;

    بازگشت p;
}


تهی *
ngx_pmemalign(ngx_pool_t *pool, size_t size, size_t alignment)
{
    تهی              *p;
    ngx_pool_large_t  *large;

    p = ngx_memalign(alignment, size, pool->log);
    اگر (p == پوچ) {
        بازگشت پوچ;
    }

    large = ngx_palloc_small(pool, اندازه(ngx_pool_large_t), 1);
    اگر (large == پوچ) {
        ngx_free(p);
        بازگشت پوچ;
    }

    large->alloc = p;
    large->next = pool->large;
    pool->large = large;

    بازگشت p;
}


ngx_int_t
ngx_pfree(ngx_pool_t *pool, تهی *p)
{
    ngx_pool_large_t  *l;

    برای (l = pool->large; l; l = l->next) {
        اگر (p == l->alloc) {
            ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0,
                           "free: %p", l->alloc);
            ngx_free(l->alloc);
            l->alloc = پوچ;

            بازگشت NGX_OK;
        }
    }

    بازگشت NGX_DECLINED;
}


تهی *
ngx_pcalloc(ngx_pool_t *pool, size_t size)
{
    تهی *p;

    p = ngx_palloc(pool, size);
    اگر (p) {
        ngx_memzero(p, size);
    }

    بازگشت p;
}


ngx_pool_cleanup_t *
ngx_pool_cleanup_add(ngx_pool_t *p, size_t size)
{
    ngx_pool_cleanup_t  *c;

    c = ngx_palloc(p, اندازه(ngx_pool_cleanup_t));
    اگر (c == پوچ) {
        بازگشت پوچ;
    }

    اگر (size) {
        c->data = ngx_palloc(p, size);
        اگر (c->data == پوچ) {
            بازگشت پوچ;
        }

    } وگرنه {
        c->data = پوچ;
    }

    c->handler = پوچ;
    c->next = p->cleanup;

    p->cleanup = c;

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, p->log, 0, "add cleanup: %p", c);

    بازگشت c;
}


تهی
ngx_pool_run_cleanup_file(ngx_pool_t *p, ngx_fd_t fd)
{
    ngx_pool_cleanup_t       *c;
    ngx_pool_cleanup_file_t  *cf;

    برای (c = p->cleanup; c; c = c->next) {
        اگر (c->handler == ngx_pool_cleanup_file) {

            cf = c->data;

            اگر (cf->fd == fd) {
                c->handler(cf);
                c->handler = پوچ;
                بازگشت;
            }
        }
    }
}


تهی
ngx_pool_cleanup_file(تهی *data)
{
    ngx_pool_cleanup_file_t  *c = data;

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, c->log, 0, "file cleanup: fd:%d",
                   c->fd);

    اگر (ngx_close_file(c->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, c->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", c->name);
    }
}


تهی
ngx_pool_delete_file(تهی *data)
{
    ngx_pool_cleanup_file_t  *c = data;

    ngx_err_t  err;

    ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, c->log, 0, "file cleanup: fd:%d %s",
                   c->fd, c->name);

    اگر (ngx_delete_file(c->name) == NGX_FILE_ERROR) {
        err = ngx_errno;

        اگر (err != NGX_ENOENT) {
            ngx_log_error(NGX_LOG_CRIT, c->log, err,
                          ngx_delete_file_n " \"%s\" failed", c->name);
        }
    }

    اگر (ngx_close_file(c->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, c->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", c->name);
    }
}


#اگر 0

ایستا تهی *
ngx_get_cached_block(size_t size)
{
    تهی                     *p;
    ngx_cached_block_slot_t  *slot;

    اگر (ngx_cycle->cache == پوچ) {
        بازگشت پوچ;
    }

    slot = &ngx_cycle->cache[(size + ngx_pagesize - 1) / ngx_pagesize];

    slot->tries++;

    اگر (slot->عدد) {
        p = slot->block;
        slot->block = slot->block->next;
        slot->عدد--;
        بازگشت p;
    }

    بازگشت پوچ;
}

#endif
