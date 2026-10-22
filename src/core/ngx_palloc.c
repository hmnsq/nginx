
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#অন্তর্ভাব <ngx_config.h>
#অন্তর্ভাব <ngx_core.h>


স্থির ngx_inline শূন্য *ngx_palloc_small(ngx_pool_t *pool, size_t size,
    ngx_uint_t align);
স্থির শূন্য *ngx_palloc_block(ngx_pool_t *pool, size_t size);
স্থির শূন্য *ngx_palloc_large(ngx_pool_t *pool, size_t size);


ngx_pool_t *
ngx_create_pool(size_t size, ngx_log_t *log)
{
    ngx_pool_t  *p;

    p = ngx_memalign(NGX_POOL_ALIGNMENT, size, log);
    যদি (p == রিক্ত) {
        প্রত্যাগম রিক্ত;
    }

    p->d.last = (u_char *) p + পরিমাণ(ngx_pool_t);
    p->d.end = (u_char *) p + size;
    p->d.next = রিক্ত;
    p->d.failed = 0;

    size = size - পরিমাণ(ngx_pool_t);
    p->max = (size < NGX_MAX_ALLOC_FROM_POOL) ? size : NGX_MAX_ALLOC_FROM_POOL;

    p->current = p;
    p->chain = রিক্ত;
    p->large = রিক্ত;
    p->cleanup = রিক্ত;
    p->log = log;

    প্রত্যাগম p;
}


শূন্য
ngx_destroy_pool(ngx_pool_t *pool)
{
    ngx_pool_t          *p, *n;
    ngx_pool_large_t    *l;
    ngx_pool_cleanup_t  *c;

    প্রতি (c = pool->cleanup; c; c = c->next) {
        যদি (c->handler) {
            ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0,
                           "run cleanup: %p", c);
            c->handler(c->data);
        }
    }

#যদি (NGX_DEBUG)

    /*
     * we could allocate the pool->log from this pool
     * so we cannot use this log while free()ing the pool
     */

    প্রতি (l = pool->large; l; l = l->next) {
        ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0, "free: %p", l->alloc);
    }

    প্রতি (p = pool, n = pool->d.next; /* void */; p = n, n = n->d.next) {
        ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, pool->log, 0,
                       "free: %p, unused: %uz", p, p->d.end - p->d.last);

        যদি (n == রিক্ত) {
            ভঙ্গ;
        }
    }

#endif

    প্রতি (l = pool->large; l; l = l->next) {
        যদি (l->alloc) {
            ngx_free(l->alloc);
        }
    }

    প্রতি (p = pool, n = pool->d.next; /* void */; p = n, n = n->d.next) {
        ngx_free(p);

        যদি (n == রিক্ত) {
            ভঙ্গ;
        }
    }
}


শূন্য
ngx_reset_pool(ngx_pool_t *pool)
{
    ngx_pool_t        *p;
    ngx_pool_large_t  *l;

    প্রতি (l = pool->large; l; l = l->next) {
        যদি (l->alloc) {
            ngx_free(l->alloc);
        }
    }

    প্রতি (p = pool; p; p = p->d.next) {
        p->d.last = (u_char *) p + পরিমাণ(ngx_pool_t);
        p->d.failed = 0;
    }

    pool->current = pool;
    pool->chain = রিক্ত;
    pool->large = রিক্ত;
}


শূন্য *
ngx_palloc(ngx_pool_t *pool, size_t size)
{
#যদি !(NGX_DEBUG_PALLOC)
    যদি (size <= pool->max) {
        প্রত্যাগম ngx_palloc_small(pool, size, 1);
    }
#endif

    প্রত্যাগম ngx_palloc_large(pool, size);
}


শূন্য *
ngx_pnalloc(ngx_pool_t *pool, size_t size)
{
#যদি !(NGX_DEBUG_PALLOC)
    যদি (size <= pool->max) {
        প্রত্যাগম ngx_palloc_small(pool, size, 0);
    }
#endif

    প্রত্যাগম ngx_palloc_large(pool, size);
}


স্থির ngx_inline শূন্য *
ngx_palloc_small(ngx_pool_t *pool, size_t size, ngx_uint_t align)
{
    u_char      *m;
    ngx_pool_t  *p;

    p = pool->current;

    কুরু {
        m = p->d.last;

        যদি (align) {
            m = ngx_align_ptr(m, NGX_ALIGNMENT);
        }

        যদি ((size_t) (p->d.end - m) >= size) {
            p->d.last = m + size;

            প্রত্যাগম m;
        }

        p = p->d.next;

    } যাবত্ (p);

    প্রত্যাগম ngx_palloc_block(pool, size);
}


স্থির শূন্য *
ngx_palloc_block(ngx_pool_t *pool, size_t size)
{
    u_char      *m;
    size_t       psize;
    ngx_pool_t  *p, *new;

    psize = (size_t) (pool->d.end - (u_char *) pool);

    m = ngx_memalign(NGX_POOL_ALIGNMENT, psize, pool->log);
    যদি (m == রিক্ত) {
        প্রত্যাগম রিক্ত;
    }

    new = (ngx_pool_t *) m;

    new->d.end = m + psize;
    new->d.next = রিক্ত;
    new->d.failed = 0;

    m += পরিমাণ(ngx_pool_data_t);
    m = ngx_align_ptr(m, NGX_ALIGNMENT);
    new->d.last = m + size;

    প্রতি (p = pool->current; p->d.next; p = p->d.next) {
        যদি (p->d.failed++ > 4) {
            pool->current = p->d.next;
        }
    }

    p->d.next = new;

    প্রত্যাগম m;
}


স্থির শূন্য *
ngx_palloc_large(ngx_pool_t *pool, size_t size)
{
    শূন্য              *p;
    ngx_uint_t         n;
    ngx_pool_large_t  *large;

    p = ngx_alloc(size, pool->log);
    যদি (p == রিক্ত) {
        প্রত্যাগম রিক্ত;
    }

    n = 0;

    প্রতি (large = pool->large; large; large = large->next) {
        যদি (large->alloc == রিক্ত) {
            large->alloc = p;
            প্রত্যাগম p;
        }

        যদি (n++ > 3) {
            ভঙ্গ;
        }
    }

    large = ngx_palloc_small(pool, পরিমাণ(ngx_pool_large_t), 1);
    যদি (large == রিক্ত) {
        ngx_free(p);
        প্রত্যাগম রিক্ত;
    }

    large->alloc = p;
    large->next = pool->large;
    pool->large = large;

    প্রত্যাগম p;
}


শূন্য *
ngx_pmemalign(ngx_pool_t *pool, size_t size, size_t alignment)
{
    শূন্য              *p;
    ngx_pool_large_t  *large;

    p = ngx_memalign(alignment, size, pool->log);
    যদি (p == রিক্ত) {
        প্রত্যাগম রিক্ত;
    }

    large = ngx_palloc_small(pool, পরিমাণ(ngx_pool_large_t), 1);
    যদি (large == রিক্ত) {
        ngx_free(p);
        প্রত্যাগম রিক্ত;
    }

    large->alloc = p;
    large->next = pool->large;
    pool->large = large;

    প্রত্যাগম p;
}


ngx_int_t
ngx_pfree(ngx_pool_t *pool, শূন্য *p)
{
    ngx_pool_large_t  *l;

    প্রতি (l = pool->large; l; l = l->next) {
        যদি (p == l->alloc) {
            ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0,
                           "free: %p", l->alloc);
            ngx_free(l->alloc);
            l->alloc = রিক্ত;

            প্রত্যাগম NGX_OK;
        }
    }

    প্রত্যাগম NGX_DECLINED;
}


শূন্য *
ngx_pcalloc(ngx_pool_t *pool, size_t size)
{
    শূন্য *p;

    p = ngx_palloc(pool, size);
    যদি (p) {
        ngx_memzero(p, size);
    }

    প্রত্যাগম p;
}


ngx_pool_cleanup_t *
ngx_pool_cleanup_add(ngx_pool_t *p, size_t size)
{
    ngx_pool_cleanup_t  *c;

    c = ngx_palloc(p, পরিমাণ(ngx_pool_cleanup_t));
    যদি (c == রিক্ত) {
        প্রত্যাগম রিক্ত;
    }

    যদি (size) {
        c->data = ngx_palloc(p, size);
        যদি (c->data == রিক্ত) {
            প্রত্যাগম রিক্ত;
        }

    } অন্যথা {
        c->data = রিক্ত;
    }

    c->handler = রিক্ত;
    c->next = p->cleanup;

    p->cleanup = c;

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, p->log, 0, "add cleanup: %p", c);

    প্রত্যাগম c;
}


শূন্য
ngx_pool_run_cleanup_file(ngx_pool_t *p, ngx_fd_t fd)
{
    ngx_pool_cleanup_t       *c;
    ngx_pool_cleanup_file_t  *cf;

    প্রতি (c = p->cleanup; c; c = c->next) {
        যদি (c->handler == ngx_pool_cleanup_file) {

            cf = c->data;

            যদি (cf->fd == fd) {
                c->handler(cf);
                c->handler = রিক্ত;
                প্রত্যাগম;
            }
        }
    }
}


শূন্য
ngx_pool_cleanup_file(শূন্য *data)
{
    ngx_pool_cleanup_file_t  *c = data;

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, c->log, 0, "file cleanup: fd:%d",
                   c->fd);

    যদি (ngx_close_file(c->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, c->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", c->name);
    }
}


শূন্য
ngx_pool_delete_file(শূন্য *data)
{
    ngx_pool_cleanup_file_t  *c = data;

    ngx_err_t  err;

    ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, c->log, 0, "file cleanup: fd:%d %s",
                   c->fd, c->name);

    যদি (ngx_delete_file(c->name) == NGX_FILE_ERROR) {
        err = ngx_errno;

        যদি (err != NGX_ENOENT) {
            ngx_log_error(NGX_LOG_CRIT, c->log, err,
                          ngx_delete_file_n " \"%s\" failed", c->name);
        }
    }

    যদি (ngx_close_file(c->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, c->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", c->name);
    }
}


#যদি 0

স্থির শূন্য *
ngx_get_cached_block(size_t size)
{
    শূন্য                     *p;
    ngx_cached_block_slot_t  *slot;

    যদি (ngx_cycle->cache == রিক্ত) {
        প্রত্যাগম রিক্ত;
    }

    slot = &ngx_cycle->cache[(size + ngx_pagesize - 1) / ngx_pagesize];

    slot->tries++;

    যদি (slot->সংখ্যা) {
        p = slot->block;
        slot->block = slot->block->next;
        slot->সংখ্যা--;
        প্রত্যাগম p;
    }

    প্রত্যাগম রিক্ত;
}

#endif
