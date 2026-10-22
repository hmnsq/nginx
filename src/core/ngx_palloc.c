
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


स्थिर ngx_inline शून्य *ngx_palloc_small(ngx_pool_t *pool, size_t size,
    ngx_uint_t align);
स्थिर शून्य *ngx_palloc_block(ngx_pool_t *pool, size_t size);
स्थिर शून्य *ngx_palloc_large(ngx_pool_t *pool, size_t size);


ngx_pool_t *
ngx_create_pool(size_t size, ngx_log_t *log)
{
    ngx_pool_t  *p;

    p = ngx_memalign(NGX_POOL_ALIGNMENT, size, log);
    सचे (p == रिक्त) {
        पटिनिवत्त रिक्त;
    }

    p->d.last = (u_char *) p + परिमाण(ngx_pool_t);
    p->d.end = (u_char *) p + size;
    p->d.next = रिक्त;
    p->d.failed = 0;

    size = size - परिमाण(ngx_pool_t);
    p->max = (size < NGX_MAX_ALLOC_FROM_POOL) ? size : NGX_MAX_ALLOC_FROM_POOL;

    p->current = p;
    p->chain = रिक्त;
    p->large = रिक्त;
    p->cleanup = रिक्त;
    p->log = log;

    पटिनिवत्त p;
}


शून्य
ngx_destroy_pool(ngx_pool_t *pool)
{
    ngx_pool_t          *p, *n;
    ngx_pool_large_t    *l;
    ngx_pool_cleanup_t  *c;

    अत्थाय (c = pool->cleanup; c; c = c->next) {
        सचे (c->handler) {
            ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0,
                           "run cleanup: %p", c);
            c->handler(c->data);
        }
    }

#सचे (NGX_DEBUG)

    /*
     * we could allocate the pool->log from this pool
     * so we cannot use this log while free()ing the pool
     */

    अत्थाय (l = pool->large; l; l = l->next) {
        ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0, "free: %p", l->alloc);
    }

    अत्थाय (p = pool, n = pool->d.next; /* void */; p = n, n = n->d.next) {
        ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, pool->log, 0,
                       "free: %p, unused: %uz", p, p->d.end - p->d.last);

        सचे (n == रिक्त) {
            भङ्ग;
        }
    }

#endif

    अत्थाय (l = pool->large; l; l = l->next) {
        सचे (l->alloc) {
            ngx_free(l->alloc);
        }
    }

    अत्थाय (p = pool, n = pool->d.next; /* void */; p = n, n = n->d.next) {
        ngx_free(p);

        सचे (n == रिक्त) {
            भङ्ग;
        }
    }
}


शून्य
ngx_reset_pool(ngx_pool_t *pool)
{
    ngx_pool_t        *p;
    ngx_pool_large_t  *l;

    अत्थाय (l = pool->large; l; l = l->next) {
        सचे (l->alloc) {
            ngx_free(l->alloc);
        }
    }

    अत्थाय (p = pool; p; p = p->d.next) {
        p->d.last = (u_char *) p + परिमाण(ngx_pool_t);
        p->d.failed = 0;
    }

    pool->current = pool;
    pool->chain = रिक्त;
    pool->large = रिक्त;
}


शून्य *
ngx_palloc(ngx_pool_t *pool, size_t size)
{
#सचे !(NGX_DEBUG_PALLOC)
    सचे (size <= pool->max) {
        पटिनिवत्त ngx_palloc_small(pool, size, 1);
    }
#endif

    पटिनिवत्त ngx_palloc_large(pool, size);
}


शून्य *
ngx_pnalloc(ngx_pool_t *pool, size_t size)
{
#सचे !(NGX_DEBUG_PALLOC)
    सचे (size <= pool->max) {
        पटिनिवत्त ngx_palloc_small(pool, size, 0);
    }
#endif

    पटिनिवत्त ngx_palloc_large(pool, size);
}


स्थिर ngx_inline शून्य *
ngx_palloc_small(ngx_pool_t *pool, size_t size, ngx_uint_t align)
{
    u_char      *m;
    ngx_pool_t  *p;

    p = pool->current;

    करोहि {
        m = p->d.last;

        सचे (align) {
            m = ngx_align_ptr(m, NGX_ALIGNMENT);
        }

        सचे ((size_t) (p->d.end - m) >= size) {
            p->d.last = m + size;

            पटिनिवत्त m;
        }

        p = p->d.next;

    } याव (p);

    पटिनिवत्त ngx_palloc_block(pool, size);
}


स्थिर शून्य *
ngx_palloc_block(ngx_pool_t *pool, size_t size)
{
    u_char      *m;
    size_t       psize;
    ngx_pool_t  *p, *new;

    psize = (size_t) (pool->d.end - (u_char *) pool);

    m = ngx_memalign(NGX_POOL_ALIGNMENT, psize, pool->log);
    सचे (m == रिक्त) {
        पटिनिवत्त रिक्त;
    }

    new = (ngx_pool_t *) m;

    new->d.end = m + psize;
    new->d.next = रिक्त;
    new->d.failed = 0;

    m += परिमाण(ngx_pool_data_t);
    m = ngx_align_ptr(m, NGX_ALIGNMENT);
    new->d.last = m + size;

    अत्थाय (p = pool->current; p->d.next; p = p->d.next) {
        सचे (p->d.failed++ > 4) {
            pool->current = p->d.next;
        }
    }

    p->d.next = new;

    पटिनिवत्त m;
}


स्थिर शून्य *
ngx_palloc_large(ngx_pool_t *pool, size_t size)
{
    शून्य              *p;
    ngx_uint_t         n;
    ngx_pool_large_t  *large;

    p = ngx_alloc(size, pool->log);
    सचे (p == रिक्त) {
        पटिनिवत्त रिक्त;
    }

    n = 0;

    अत्थाय (large = pool->large; large; large = large->next) {
        सचे (large->alloc == रिक्त) {
            large->alloc = p;
            पटिनिवत्त p;
        }

        सचे (n++ > 3) {
            भङ्ग;
        }
    }

    large = ngx_palloc_small(pool, परिमाण(ngx_pool_large_t), 1);
    सचे (large == रिक्त) {
        ngx_free(p);
        पटिनिवत्त रिक्त;
    }

    large->alloc = p;
    large->next = pool->large;
    pool->large = large;

    पटिनिवत्त p;
}


शून्य *
ngx_pmemalign(ngx_pool_t *pool, size_t size, size_t alignment)
{
    शून्य              *p;
    ngx_pool_large_t  *large;

    p = ngx_memalign(alignment, size, pool->log);
    सचे (p == रिक्त) {
        पटिनिवत्त रिक्त;
    }

    large = ngx_palloc_small(pool, परिमाण(ngx_pool_large_t), 1);
    सचे (large == रिक्त) {
        ngx_free(p);
        पटिनिवत्त रिक्त;
    }

    large->alloc = p;
    large->next = pool->large;
    pool->large = large;

    पटिनिवत्त p;
}


ngx_int_t
ngx_pfree(ngx_pool_t *pool, शून्य *p)
{
    ngx_pool_large_t  *l;

    अत्थाय (l = pool->large; l; l = l->next) {
        सचे (p == l->alloc) {
            ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0,
                           "free: %p", l->alloc);
            ngx_free(l->alloc);
            l->alloc = रिक्त;

            पटिनिवत्त NGX_OK;
        }
    }

    पटिनिवत्त NGX_DECLINED;
}


शून्य *
ngx_pcalloc(ngx_pool_t *pool, size_t size)
{
    शून्य *p;

    p = ngx_palloc(pool, size);
    सचे (p) {
        ngx_memzero(p, size);
    }

    पटिनिवत्त p;
}


ngx_pool_cleanup_t *
ngx_pool_cleanup_add(ngx_pool_t *p, size_t size)
{
    ngx_pool_cleanup_t  *c;

    c = ngx_palloc(p, परिमाण(ngx_pool_cleanup_t));
    सचे (c == रिक्त) {
        पटिनिवत्त रिक्त;
    }

    सचे (size) {
        c->data = ngx_palloc(p, size);
        सचे (c->data == रिक्त) {
            पटिनिवत्त रिक्त;
        }

    } अञ्ञथा {
        c->data = रिक्त;
    }

    c->handler = रिक्त;
    c->next = p->cleanup;

    p->cleanup = c;

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, p->log, 0, "add cleanup: %p", c);

    पटिनिवत्त c;
}


शून्य
ngx_pool_run_cleanup_file(ngx_pool_t *p, ngx_fd_t fd)
{
    ngx_pool_cleanup_t       *c;
    ngx_pool_cleanup_file_t  *cf;

    अत्थाय (c = p->cleanup; c; c = c->next) {
        सचे (c->handler == ngx_pool_cleanup_file) {

            cf = c->data;

            सचे (cf->fd == fd) {
                c->handler(cf);
                c->handler = रिक्त;
                पटिनिवत्त;
            }
        }
    }
}


शून्य
ngx_pool_cleanup_file(शून्य *data)
{
    ngx_pool_cleanup_file_t  *c = data;

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, c->log, 0, "file cleanup: fd:%d",
                   c->fd);

    सचे (ngx_close_file(c->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, c->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", c->name);
    }
}


शून्य
ngx_pool_delete_file(शून्य *data)
{
    ngx_pool_cleanup_file_t  *c = data;

    ngx_err_t  err;

    ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, c->log, 0, "file cleanup: fd:%d %s",
                   c->fd, c->name);

    सचे (ngx_delete_file(c->name) == NGX_FILE_ERROR) {
        err = ngx_errno;

        सचे (err != NGX_ENOENT) {
            ngx_log_error(NGX_LOG_CRIT, c->log, err,
                          ngx_delete_file_n " \"%s\" failed", c->name);
        }
    }

    सचे (ngx_close_file(c->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, c->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", c->name);
    }
}


#सचे 0

स्थिर शून्य *
ngx_get_cached_block(size_t size)
{
    शून्य                     *p;
    ngx_cached_block_slot_t  *slot;

    सचे (ngx_cycle->cache == रिक्त) {
        पटिनिवत्त रिक्त;
    }

    slot = &ngx_cycle->cache[(size + ngx_pagesize - 1) / ngx_pagesize];

    slot->tries++;

    सचे (slot->सङ्ख्या) {
        p = slot->block;
        slot->block = slot->block->next;
        slot->सङ्ख्या--;
        पटिनिवत्त p;
    }

    पटिनिवत्त रिक्त;
}

#endif
