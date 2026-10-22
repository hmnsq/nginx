
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


ಸ್ಥಿರ ngx_inline ಶೂನ್ಯ *ngx_palloc_small(ngx_pool_t *pool, size_t size,
    ngx_uint_t align);
ಸ್ಥಿರ ಶೂನ್ಯ *ngx_palloc_block(ngx_pool_t *pool, size_t size);
ಸ್ಥಿರ ಶೂನ್ಯ *ngx_palloc_large(ngx_pool_t *pool, size_t size);


ngx_pool_t *
ngx_create_pool(size_t size, ngx_log_t *log)
{
    ngx_pool_t  *p;

    p = ngx_memalign(NGX_POOL_ALIGNMENT, size, log);
    ಯದಿ (p == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    p->d.last = (u_char *) p + ಪರಿಮಾಣ(ngx_pool_t);
    p->d.end = (u_char *) p + size;
    p->d.next = ರಿಕ್ತ;
    p->d.failed = 0;

    size = size - ಪರಿಮಾಣ(ngx_pool_t);
    p->max = (size < NGX_MAX_ALLOC_FROM_POOL) ? size : NGX_MAX_ALLOC_FROM_POOL;

    p->current = p;
    p->chain = ರಿಕ್ತ;
    p->large = ರಿಕ್ತ;
    p->cleanup = ರಿಕ್ತ;
    p->log = log;

    ಪ್ರತ್ಯಾಗಮ p;
}


ಶೂನ್ಯ
ngx_destroy_pool(ngx_pool_t *pool)
{
    ngx_pool_t          *p, *n;
    ngx_pool_large_t    *l;
    ngx_pool_cleanup_t  *c;

    ಪ್ರತಿ (c = pool->cleanup; c; c = c->next) {
        ಯದಿ (c->handler) {
            ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0,
                           "run cleanup: %p", c);
            c->handler(c->data);
        }
    }

#ಯದಿ (NGX_DEBUG)

    /*
     * we could allocate the pool->log from this pool
     * so we cannot use this log while free()ing the pool
     */

    ಪ್ರತಿ (l = pool->large; l; l = l->next) {
        ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0, "free: %p", l->alloc);
    }

    ಪ್ರತಿ (p = pool, n = pool->d.next; /* void */; p = n, n = n->d.next) {
        ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, pool->log, 0,
                       "free: %p, unused: %uz", p, p->d.end - p->d.last);

        ಯದಿ (n == ರಿಕ್ತ) {
            ಭಙ್ಗ;
        }
    }

#endif

    ಪ್ರತಿ (l = pool->large; l; l = l->next) {
        ಯದಿ (l->alloc) {
            ngx_free(l->alloc);
        }
    }

    ಪ್ರತಿ (p = pool, n = pool->d.next; /* void */; p = n, n = n->d.next) {
        ngx_free(p);

        ಯದಿ (n == ರಿಕ್ತ) {
            ಭಙ್ಗ;
        }
    }
}


ಶೂನ್ಯ
ngx_reset_pool(ngx_pool_t *pool)
{
    ngx_pool_t        *p;
    ngx_pool_large_t  *l;

    ಪ್ರತಿ (l = pool->large; l; l = l->next) {
        ಯದಿ (l->alloc) {
            ngx_free(l->alloc);
        }
    }

    ಪ್ರತಿ (p = pool; p; p = p->d.next) {
        p->d.last = (u_char *) p + ಪರಿಮಾಣ(ngx_pool_t);
        p->d.failed = 0;
    }

    pool->current = pool;
    pool->chain = ರಿಕ್ತ;
    pool->large = ರಿಕ್ತ;
}


ಶೂನ್ಯ *
ngx_palloc(ngx_pool_t *pool, size_t size)
{
#ಯದಿ !(NGX_DEBUG_PALLOC)
    ಯದಿ (size <= pool->max) {
        ಪ್ರತ್ಯಾಗಮ ngx_palloc_small(pool, size, 1);
    }
#endif

    ಪ್ರತ್ಯಾಗಮ ngx_palloc_large(pool, size);
}


ಶೂನ್ಯ *
ngx_pnalloc(ngx_pool_t *pool, size_t size)
{
#ಯದಿ !(NGX_DEBUG_PALLOC)
    ಯದಿ (size <= pool->max) {
        ಪ್ರತ್ಯಾಗಮ ngx_palloc_small(pool, size, 0);
    }
#endif

    ಪ್ರತ್ಯಾಗಮ ngx_palloc_large(pool, size);
}


ಸ್ಥಿರ ngx_inline ಶೂನ್ಯ *
ngx_palloc_small(ngx_pool_t *pool, size_t size, ngx_uint_t align)
{
    u_char      *m;
    ngx_pool_t  *p;

    p = pool->current;

    ಕುರು {
        m = p->d.last;

        ಯದಿ (align) {
            m = ngx_align_ptr(m, NGX_ALIGNMENT);
        }

        ಯದಿ ((size_t) (p->d.end - m) >= size) {
            p->d.last = m + size;

            ಪ್ರತ್ಯಾಗಮ m;
        }

        p = p->d.next;

    } ಯಾವತ್ (p);

    ಪ್ರತ್ಯಾಗಮ ngx_palloc_block(pool, size);
}


ಸ್ಥಿರ ಶೂನ್ಯ *
ngx_palloc_block(ngx_pool_t *pool, size_t size)
{
    u_char      *m;
    size_t       psize;
    ngx_pool_t  *p, *new;

    psize = (size_t) (pool->d.end - (u_char *) pool);

    m = ngx_memalign(NGX_POOL_ALIGNMENT, psize, pool->log);
    ಯದಿ (m == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    new = (ngx_pool_t *) m;

    new->d.end = m + psize;
    new->d.next = ರಿಕ್ತ;
    new->d.failed = 0;

    m += ಪರಿಮಾಣ(ngx_pool_data_t);
    m = ngx_align_ptr(m, NGX_ALIGNMENT);
    new->d.last = m + size;

    ಪ್ರತಿ (p = pool->current; p->d.next; p = p->d.next) {
        ಯದಿ (p->d.failed++ > 4) {
            pool->current = p->d.next;
        }
    }

    p->d.next = new;

    ಪ್ರತ್ಯಾಗಮ m;
}


ಸ್ಥಿರ ಶೂನ್ಯ *
ngx_palloc_large(ngx_pool_t *pool, size_t size)
{
    ಶೂನ್ಯ              *p;
    ngx_uint_t         n;
    ngx_pool_large_t  *large;

    p = ngx_alloc(size, pool->log);
    ಯದಿ (p == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    n = 0;

    ಪ್ರತಿ (large = pool->large; large; large = large->next) {
        ಯದಿ (large->alloc == ರಿಕ್ತ) {
            large->alloc = p;
            ಪ್ರತ್ಯಾಗಮ p;
        }

        ಯದಿ (n++ > 3) {
            ಭಙ್ಗ;
        }
    }

    large = ngx_palloc_small(pool, ಪರಿಮಾಣ(ngx_pool_large_t), 1);
    ಯದಿ (large == ರಿಕ್ತ) {
        ngx_free(p);
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    large->alloc = p;
    large->next = pool->large;
    pool->large = large;

    ಪ್ರತ್ಯಾಗಮ p;
}


ಶೂನ್ಯ *
ngx_pmemalign(ngx_pool_t *pool, size_t size, size_t alignment)
{
    ಶೂನ್ಯ              *p;
    ngx_pool_large_t  *large;

    p = ngx_memalign(alignment, size, pool->log);
    ಯದಿ (p == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    large = ngx_palloc_small(pool, ಪರಿಮಾಣ(ngx_pool_large_t), 1);
    ಯದಿ (large == ರಿಕ್ತ) {
        ngx_free(p);
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    large->alloc = p;
    large->next = pool->large;
    pool->large = large;

    ಪ್ರತ್ಯಾಗಮ p;
}


ngx_int_t
ngx_pfree(ngx_pool_t *pool, ಶೂನ್ಯ *p)
{
    ngx_pool_large_t  *l;

    ಪ್ರತಿ (l = pool->large; l; l = l->next) {
        ಯದಿ (p == l->alloc) {
            ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0,
                           "free: %p", l->alloc);
            ngx_free(l->alloc);
            l->alloc = ರಿಕ್ತ;

            ಪ್ರತ್ಯಾಗಮ NGX_OK;
        }
    }

    ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;
}


ಶೂನ್ಯ *
ngx_pcalloc(ngx_pool_t *pool, size_t size)
{
    ಶೂನ್ಯ *p;

    p = ngx_palloc(pool, size);
    ಯದಿ (p) {
        ngx_memzero(p, size);
    }

    ಪ್ರತ್ಯಾಗಮ p;
}


ngx_pool_cleanup_t *
ngx_pool_cleanup_add(ngx_pool_t *p, size_t size)
{
    ngx_pool_cleanup_t  *c;

    c = ngx_palloc(p, ಪರಿಮಾಣ(ngx_pool_cleanup_t));
    ಯದಿ (c == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ಯದಿ (size) {
        c->data = ngx_palloc(p, size);
        ಯದಿ (c->data == ರಿಕ್ತ) {
            ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
        }

    } ಅನ್ಯಥಾ {
        c->data = ರಿಕ್ತ;
    }

    c->handler = ರಿಕ್ತ;
    c->next = p->cleanup;

    p->cleanup = c;

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, p->log, 0, "add cleanup: %p", c);

    ಪ್ರತ್ಯಾಗಮ c;
}


ಶೂನ್ಯ
ngx_pool_run_cleanup_file(ngx_pool_t *p, ngx_fd_t fd)
{
    ngx_pool_cleanup_t       *c;
    ngx_pool_cleanup_file_t  *cf;

    ಪ್ರತಿ (c = p->cleanup; c; c = c->next) {
        ಯದಿ (c->handler == ngx_pool_cleanup_file) {

            cf = c->data;

            ಯದಿ (cf->fd == fd) {
                c->handler(cf);
                c->handler = ರಿಕ್ತ;
                ಪ್ರತ್ಯಾಗಮ;
            }
        }
    }
}


ಶೂನ್ಯ
ngx_pool_cleanup_file(ಶೂನ್ಯ *data)
{
    ngx_pool_cleanup_file_t  *c = data;

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, c->log, 0, "file cleanup: fd:%d",
                   c->fd);

    ಯದಿ (ngx_close_file(c->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, c->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", c->name);
    }
}


ಶೂನ್ಯ
ngx_pool_delete_file(ಶೂನ್ಯ *data)
{
    ngx_pool_cleanup_file_t  *c = data;

    ngx_err_t  err;

    ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, c->log, 0, "file cleanup: fd:%d %s",
                   c->fd, c->name);

    ಯದಿ (ngx_delete_file(c->name) == NGX_FILE_ERROR) {
        err = ngx_errno;

        ಯದಿ (err != NGX_ENOENT) {
            ngx_log_error(NGX_LOG_CRIT, c->log, err,
                          ngx_delete_file_n " \"%s\" failed", c->name);
        }
    }

    ಯದಿ (ngx_close_file(c->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, c->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", c->name);
    }
}


#ಯದಿ 0

ಸ್ಥಿರ ಶೂನ್ಯ *
ngx_get_cached_block(size_t size)
{
    ಶೂನ್ಯ                     *p;
    ngx_cached_block_slot_t  *slot;

    ಯದಿ (ngx_cycle->cache == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    slot = &ngx_cycle->cache[(size + ngx_pagesize - 1) / ngx_pagesize];

    slot->tries++;

    ಯದಿ (slot->ಸಂಖ್ಯೆ) {
        p = slot->block;
        slot->block = slot->block->next;
        slot->ಸಂಖ್ಯೆ--;
        ಪ್ರತ್ಯಾಗಮ p;
    }

    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

#endif
