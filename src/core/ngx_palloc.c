
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#അന്തര്ഭാവ <ngx_config.h>
#അന്തര്ഭാവ <ngx_core.h>


സ്ഥിതം ngx_inline ശൂന്യം *ngx_palloc_small(ngx_pool_t *pool, size_t size,
    ngx_uint_t align);
സ്ഥിതം ശൂന്യം *ngx_palloc_block(ngx_pool_t *pool, size_t size);
സ്ഥിതം ശൂന്യം *ngx_palloc_large(ngx_pool_t *pool, size_t size);


ngx_pool_t *
ngx_create_pool(size_t size, ngx_log_t *log)
{
    ngx_pool_t  *p;

    p = ngx_memalign(NGX_POOL_ALIGNMENT, size, log);
    എങ്കിൽ (p == ശൂന്യ) {
        തിരികെ ശൂന്യ;
    }

    p->d.last = (u_char *) p + വലുപ്പം(ngx_pool_t);
    p->d.end = (u_char *) p + size;
    p->d.next = ശൂന്യ;
    p->d.failed = 0;

    size = size - വലുപ്പം(ngx_pool_t);
    p->max = (size < NGX_MAX_ALLOC_FROM_POOL) ? size : NGX_MAX_ALLOC_FROM_POOL;

    p->current = p;
    p->chain = ശൂന്യ;
    p->large = ശൂന്യ;
    p->cleanup = ശൂന്യ;
    p->log = log;

    തിരികെ p;
}


ശൂന്യം
ngx_destroy_pool(ngx_pool_t *pool)
{
    ngx_pool_t          *p, *n;
    ngx_pool_large_t    *l;
    ngx_pool_cleanup_t  *c;

    വേണ്ടി (c = pool->cleanup; c; c = c->next) {
        എങ്കിൽ (c->handler) {
            ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0,
                           "run cleanup: %p", c);
            c->handler(c->data);
        }
    }

#എങ്കിൽ (NGX_DEBUG)

    /*
     * we could allocate the pool->log from this pool
     * so we cannot use this log while free()ing the pool
     */

    വേണ്ടി (l = pool->large; l; l = l->next) {
        ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0, "free: %p", l->alloc);
    }

    വേണ്ടി (p = pool, n = pool->d.next; /* void */; p = n, n = n->d.next) {
        ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, pool->log, 0,
                       "free: %p, unused: %uz", p, p->d.end - p->d.last);

        എങ്കിൽ (n == ശൂന്യ) {
            നിർത്തുക;
        }
    }

#endif

    വേണ്ടി (l = pool->large; l; l = l->next) {
        എങ്കിൽ (l->alloc) {
            ngx_free(l->alloc);
        }
    }

    വേണ്ടി (p = pool, n = pool->d.next; /* void */; p = n, n = n->d.next) {
        ngx_free(p);

        എങ്കിൽ (n == ശൂന്യ) {
            നിർത്തുക;
        }
    }
}


ശൂന്യം
ngx_reset_pool(ngx_pool_t *pool)
{
    ngx_pool_t        *p;
    ngx_pool_large_t  *l;

    വേണ്ടി (l = pool->large; l; l = l->next) {
        എങ്കിൽ (l->alloc) {
            ngx_free(l->alloc);
        }
    }

    വേണ്ടി (p = pool; p; p = p->d.next) {
        p->d.last = (u_char *) p + വലുപ്പം(ngx_pool_t);
        p->d.failed = 0;
    }

    pool->current = pool;
    pool->chain = ശൂന്യ;
    pool->large = ശൂന്യ;
}


ശൂന്യം *
ngx_palloc(ngx_pool_t *pool, size_t size)
{
#എങ്കിൽ !(NGX_DEBUG_PALLOC)
    എങ്കിൽ (size <= pool->max) {
        തിരികെ ngx_palloc_small(pool, size, 1);
    }
#endif

    തിരികെ ngx_palloc_large(pool, size);
}


ശൂന്യം *
ngx_pnalloc(ngx_pool_t *pool, size_t size)
{
#എങ്കിൽ !(NGX_DEBUG_PALLOC)
    എങ്കിൽ (size <= pool->max) {
        തിരികെ ngx_palloc_small(pool, size, 0);
    }
#endif

    തിരികെ ngx_palloc_large(pool, size);
}


സ്ഥിതം ngx_inline ശൂന്യം *
ngx_palloc_small(ngx_pool_t *pool, size_t size, ngx_uint_t align)
{
    u_char      *m;
    ngx_pool_t  *p;

    p = pool->current;

    ചെയ്യുക {
        m = p->d.last;

        എങ്കിൽ (align) {
            m = ngx_align_ptr(m, NGX_ALIGNMENT);
        }

        എങ്കിൽ ((size_t) (p->d.end - m) >= size) {
            p->d.last = m + size;

            തിരികെ m;
        }

        p = p->d.next;

    } ആകുമ്പോൾ (p);

    തിരികെ ngx_palloc_block(pool, size);
}


സ്ഥിതം ശൂന്യം *
ngx_palloc_block(ngx_pool_t *pool, size_t size)
{
    u_char      *m;
    size_t       psize;
    ngx_pool_t  *p, *new;

    psize = (size_t) (pool->d.end - (u_char *) pool);

    m = ngx_memalign(NGX_POOL_ALIGNMENT, psize, pool->log);
    എങ്കിൽ (m == ശൂന്യ) {
        തിരികെ ശൂന്യ;
    }

    new = (ngx_pool_t *) m;

    new->d.end = m + psize;
    new->d.next = ശൂന്യ;
    new->d.failed = 0;

    m += വലുപ്പം(ngx_pool_data_t);
    m = ngx_align_ptr(m, NGX_ALIGNMENT);
    new->d.last = m + size;

    വേണ്ടി (p = pool->current; p->d.next; p = p->d.next) {
        എങ്കിൽ (p->d.failed++ > 4) {
            pool->current = p->d.next;
        }
    }

    p->d.next = new;

    തിരികെ m;
}


സ്ഥിതം ശൂന്യം *
ngx_palloc_large(ngx_pool_t *pool, size_t size)
{
    ശൂന്യം              *p;
    ngx_uint_t         n;
    ngx_pool_large_t  *large;

    p = ngx_alloc(size, pool->log);
    എങ്കിൽ (p == ശൂന്യ) {
        തിരികെ ശൂന്യ;
    }

    n = 0;

    വേണ്ടി (large = pool->large; large; large = large->next) {
        എങ്കിൽ (large->alloc == ശൂന്യ) {
            large->alloc = p;
            തിരികെ p;
        }

        എങ്കിൽ (n++ > 3) {
            നിർത്തുക;
        }
    }

    large = ngx_palloc_small(pool, വലുപ്പം(ngx_pool_large_t), 1);
    എങ്കിൽ (large == ശൂന്യ) {
        ngx_free(p);
        തിരികെ ശൂന്യ;
    }

    large->alloc = p;
    large->next = pool->large;
    pool->large = large;

    തിരികെ p;
}


ശൂന്യം *
ngx_pmemalign(ngx_pool_t *pool, size_t size, size_t alignment)
{
    ശൂന്യം              *p;
    ngx_pool_large_t  *large;

    p = ngx_memalign(alignment, size, pool->log);
    എങ്കിൽ (p == ശൂന്യ) {
        തിരികെ ശൂന്യ;
    }

    large = ngx_palloc_small(pool, വലുപ്പം(ngx_pool_large_t), 1);
    എങ്കിൽ (large == ശൂന്യ) {
        ngx_free(p);
        തിരികെ ശൂന്യ;
    }

    large->alloc = p;
    large->next = pool->large;
    pool->large = large;

    തിരികെ p;
}


ngx_int_t
ngx_pfree(ngx_pool_t *pool, ശൂന്യം *p)
{
    ngx_pool_large_t  *l;

    വേണ്ടി (l = pool->large; l; l = l->next) {
        എങ്കിൽ (p == l->alloc) {
            ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0,
                           "free: %p", l->alloc);
            ngx_free(l->alloc);
            l->alloc = ശൂന്യ;

            തിരികെ NGX_OK;
        }
    }

    തിരികെ NGX_DECLINED;
}


ശൂന്യം *
ngx_pcalloc(ngx_pool_t *pool, size_t size)
{
    ശൂന്യം *p;

    p = ngx_palloc(pool, size);
    എങ്കിൽ (p) {
        ngx_memzero(p, size);
    }

    തിരികെ p;
}


ngx_pool_cleanup_t *
ngx_pool_cleanup_add(ngx_pool_t *p, size_t size)
{
    ngx_pool_cleanup_t  *c;

    c = ngx_palloc(p, വലുപ്പം(ngx_pool_cleanup_t));
    എങ്കിൽ (c == ശൂന്യ) {
        തിരികെ ശൂന്യ;
    }

    എങ്കിൽ (size) {
        c->data = ngx_palloc(p, size);
        എങ്കിൽ (c->data == ശൂന്യ) {
            തിരികെ ശൂന്യ;
        }

    } അല്ലെങ്കിൽ {
        c->data = ശൂന്യ;
    }

    c->handler = ശൂന്യ;
    c->next = p->cleanup;

    p->cleanup = c;

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, p->log, 0, "add cleanup: %p", c);

    തിരികെ c;
}


ശൂന്യം
ngx_pool_run_cleanup_file(ngx_pool_t *p, ngx_fd_t fd)
{
    ngx_pool_cleanup_t       *c;
    ngx_pool_cleanup_file_t  *cf;

    വേണ്ടി (c = p->cleanup; c; c = c->next) {
        എങ്കിൽ (c->handler == ngx_pool_cleanup_file) {

            cf = c->data;

            എങ്കിൽ (cf->fd == fd) {
                c->handler(cf);
                c->handler = ശൂന്യ;
                തിരികെ;
            }
        }
    }
}


ശൂന്യം
ngx_pool_cleanup_file(ശൂന്യം *data)
{
    ngx_pool_cleanup_file_t  *c = data;

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, c->log, 0, "file cleanup: fd:%d",
                   c->fd);

    എങ്കിൽ (ngx_close_file(c->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, c->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", c->name);
    }
}


ശൂന്യം
ngx_pool_delete_file(ശൂന്യം *data)
{
    ngx_pool_cleanup_file_t  *c = data;

    ngx_err_t  err;

    ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, c->log, 0, "file cleanup: fd:%d %s",
                   c->fd, c->name);

    എങ്കിൽ (ngx_delete_file(c->name) == NGX_FILE_ERROR) {
        err = ngx_errno;

        എങ്കിൽ (err != NGX_ENOENT) {
            ngx_log_error(NGX_LOG_CRIT, c->log, err,
                          ngx_delete_file_n " \"%s\" failed", c->name);
        }
    }

    എങ്കിൽ (ngx_close_file(c->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, c->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", c->name);
    }
}


#എങ്കിൽ 0

സ്ഥിതം ശൂന്യം *
ngx_get_cached_block(size_t size)
{
    ശൂന്യം                     *p;
    ngx_cached_block_slot_t  *slot;

    എങ്കിൽ (ngx_cycle->cache == ശൂന്യ) {
        തിരികെ ശൂന്യ;
    }

    slot = &ngx_cycle->cache[(size + ngx_pagesize - 1) / ngx_pagesize];

    slot->tries++;

    എങ്കിൽ (slot->സംഖ്യ) {
        p = slot->block;
        slot->block = slot->block->next;
        slot->സംഖ്യ--;
        തിരികെ p;
    }

    തിരികെ ശൂന്യ;
}

#endif
