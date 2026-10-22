
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#அந்தர்பாவ <ngx_config.h>
#அந்தர்பாவ <ngx_core.h>


நிலையான ngx_inline வெறுமை *ngx_palloc_small(ngx_pool_t *pool, size_t size,
    ngx_uint_t align);
நிலையான வெறுமை *ngx_palloc_block(ngx_pool_t *pool, size_t size);
நிலையான வெறுமை *ngx_palloc_large(ngx_pool_t *pool, size_t size);


ngx_pool_t *
ngx_create_pool(size_t size, ngx_log_t *log)
{
    ngx_pool_t  *p;

    p = ngx_memalign(NGX_POOL_ALIGNMENT, size, log);
    எனில் (p == வெற்று) {
        திரும்பு வெற்று;
    }

    p->d.last = (u_char *) p + அளவு(ngx_pool_t);
    p->d.end = (u_char *) p + size;
    p->d.next = வெற்று;
    p->d.failed = 0;

    size = size - அளவு(ngx_pool_t);
    p->max = (size < NGX_MAX_ALLOC_FROM_POOL) ? size : NGX_MAX_ALLOC_FROM_POOL;

    p->current = p;
    p->chain = வெற்று;
    p->large = வெற்று;
    p->cleanup = வெற்று;
    p->log = log;

    திரும்பு p;
}


வெறுமை
ngx_destroy_pool(ngx_pool_t *pool)
{
    ngx_pool_t          *p, *n;
    ngx_pool_large_t    *l;
    ngx_pool_cleanup_t  *c;

    க்கு (c = pool->cleanup; c; c = c->next) {
        எனில் (c->handler) {
            ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0,
                           "run cleanup: %p", c);
            c->handler(c->data);
        }
    }

#எனில் (NGX_DEBUG)

    /*
     * we could allocate the pool->log from this pool
     * so we cannot use this log while free()ing the pool
     */

    க்கு (l = pool->large; l; l = l->next) {
        ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0, "free: %p", l->alloc);
    }

    க்கு (p = pool, n = pool->d.next; /* void */; p = n, n = n->d.next) {
        ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, pool->log, 0,
                       "free: %p, unused: %uz", p, p->d.end - p->d.last);

        எனில் (n == வெற்று) {
            நிறுத்து;
        }
    }

#endif

    க்கு (l = pool->large; l; l = l->next) {
        எனில் (l->alloc) {
            ngx_free(l->alloc);
        }
    }

    க்கு (p = pool, n = pool->d.next; /* void */; p = n, n = n->d.next) {
        ngx_free(p);

        எனில் (n == வெற்று) {
            நிறுத்து;
        }
    }
}


வெறுமை
ngx_reset_pool(ngx_pool_t *pool)
{
    ngx_pool_t        *p;
    ngx_pool_large_t  *l;

    க்கு (l = pool->large; l; l = l->next) {
        எனில் (l->alloc) {
            ngx_free(l->alloc);
        }
    }

    க்கு (p = pool; p; p = p->d.next) {
        p->d.last = (u_char *) p + அளவு(ngx_pool_t);
        p->d.failed = 0;
    }

    pool->current = pool;
    pool->chain = வெற்று;
    pool->large = வெற்று;
}


வெறுமை *
ngx_palloc(ngx_pool_t *pool, size_t size)
{
#எனில் !(NGX_DEBUG_PALLOC)
    எனில் (size <= pool->max) {
        திரும்பு ngx_palloc_small(pool, size, 1);
    }
#endif

    திரும்பு ngx_palloc_large(pool, size);
}


வெறுமை *
ngx_pnalloc(ngx_pool_t *pool, size_t size)
{
#எனில் !(NGX_DEBUG_PALLOC)
    எனில் (size <= pool->max) {
        திரும்பு ngx_palloc_small(pool, size, 0);
    }
#endif

    திரும்பு ngx_palloc_large(pool, size);
}


நிலையான ngx_inline வெறுமை *
ngx_palloc_small(ngx_pool_t *pool, size_t size, ngx_uint_t align)
{
    u_char      *m;
    ngx_pool_t  *p;

    p = pool->current;

    செய் {
        m = p->d.last;

        எனில் (align) {
            m = ngx_align_ptr(m, NGX_ALIGNMENT);
        }

        எனில் ((size_t) (p->d.end - m) >= size) {
            p->d.last = m + size;

            திரும்பு m;
        }

        p = p->d.next;

    } போது (p);

    திரும்பு ngx_palloc_block(pool, size);
}


நிலையான வெறுமை *
ngx_palloc_block(ngx_pool_t *pool, size_t size)
{
    u_char      *m;
    size_t       psize;
    ngx_pool_t  *p, *new;

    psize = (size_t) (pool->d.end - (u_char *) pool);

    m = ngx_memalign(NGX_POOL_ALIGNMENT, psize, pool->log);
    எனில் (m == வெற்று) {
        திரும்பு வெற்று;
    }

    new = (ngx_pool_t *) m;

    new->d.end = m + psize;
    new->d.next = வெற்று;
    new->d.failed = 0;

    m += அளவு(ngx_pool_data_t);
    m = ngx_align_ptr(m, NGX_ALIGNMENT);
    new->d.last = m + size;

    க்கு (p = pool->current; p->d.next; p = p->d.next) {
        எனில் (p->d.failed++ > 4) {
            pool->current = p->d.next;
        }
    }

    p->d.next = new;

    திரும்பு m;
}


நிலையான வெறுமை *
ngx_palloc_large(ngx_pool_t *pool, size_t size)
{
    வெறுமை              *p;
    ngx_uint_t         n;
    ngx_pool_large_t  *large;

    p = ngx_alloc(size, pool->log);
    எனில் (p == வெற்று) {
        திரும்பு வெற்று;
    }

    n = 0;

    க்கு (large = pool->large; large; large = large->next) {
        எனில் (large->alloc == வெற்று) {
            large->alloc = p;
            திரும்பு p;
        }

        எனில் (n++ > 3) {
            நிறுத்து;
        }
    }

    large = ngx_palloc_small(pool, அளவு(ngx_pool_large_t), 1);
    எனில் (large == வெற்று) {
        ngx_free(p);
        திரும்பு வெற்று;
    }

    large->alloc = p;
    large->next = pool->large;
    pool->large = large;

    திரும்பு p;
}


வெறுமை *
ngx_pmemalign(ngx_pool_t *pool, size_t size, size_t alignment)
{
    வெறுமை              *p;
    ngx_pool_large_t  *large;

    p = ngx_memalign(alignment, size, pool->log);
    எனில் (p == வெற்று) {
        திரும்பு வெற்று;
    }

    large = ngx_palloc_small(pool, அளவு(ngx_pool_large_t), 1);
    எனில் (large == வெற்று) {
        ngx_free(p);
        திரும்பு வெற்று;
    }

    large->alloc = p;
    large->next = pool->large;
    pool->large = large;

    திரும்பு p;
}


ngx_int_t
ngx_pfree(ngx_pool_t *pool, வெறுமை *p)
{
    ngx_pool_large_t  *l;

    க்கு (l = pool->large; l; l = l->next) {
        எனில் (p == l->alloc) {
            ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0,
                           "free: %p", l->alloc);
            ngx_free(l->alloc);
            l->alloc = வெற்று;

            திரும்பு NGX_OK;
        }
    }

    திரும்பு NGX_DECLINED;
}


வெறுமை *
ngx_pcalloc(ngx_pool_t *pool, size_t size)
{
    வெறுமை *p;

    p = ngx_palloc(pool, size);
    எனில் (p) {
        ngx_memzero(p, size);
    }

    திரும்பு p;
}


ngx_pool_cleanup_t *
ngx_pool_cleanup_add(ngx_pool_t *p, size_t size)
{
    ngx_pool_cleanup_t  *c;

    c = ngx_palloc(p, அளவு(ngx_pool_cleanup_t));
    எனில் (c == வெற்று) {
        திரும்பு வெற்று;
    }

    எனில் (size) {
        c->data = ngx_palloc(p, size);
        எனில் (c->data == வெற்று) {
            திரும்பு வெற்று;
        }

    } இல்லையெனில் {
        c->data = வெற்று;
    }

    c->handler = வெற்று;
    c->next = p->cleanup;

    p->cleanup = c;

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, p->log, 0, "add cleanup: %p", c);

    திரும்பு c;
}


வெறுமை
ngx_pool_run_cleanup_file(ngx_pool_t *p, ngx_fd_t fd)
{
    ngx_pool_cleanup_t       *c;
    ngx_pool_cleanup_file_t  *cf;

    க்கு (c = p->cleanup; c; c = c->next) {
        எனில் (c->handler == ngx_pool_cleanup_file) {

            cf = c->data;

            எனில் (cf->fd == fd) {
                c->handler(cf);
                c->handler = வெற்று;
                திரும்பு;
            }
        }
    }
}


வெறுமை
ngx_pool_cleanup_file(வெறுமை *data)
{
    ngx_pool_cleanup_file_t  *c = data;

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, c->log, 0, "file cleanup: fd:%d",
                   c->fd);

    எனில் (ngx_close_file(c->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, c->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", c->name);
    }
}


வெறுமை
ngx_pool_delete_file(வெறுமை *data)
{
    ngx_pool_cleanup_file_t  *c = data;

    ngx_err_t  err;

    ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, c->log, 0, "file cleanup: fd:%d %s",
                   c->fd, c->name);

    எனில் (ngx_delete_file(c->name) == NGX_FILE_ERROR) {
        err = ngx_errno;

        எனில் (err != NGX_ENOENT) {
            ngx_log_error(NGX_LOG_CRIT, c->log, err,
                          ngx_delete_file_n " \"%s\" failed", c->name);
        }
    }

    எனில் (ngx_close_file(c->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, c->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", c->name);
    }
}


#எனில் 0

நிலையான வெறுமை *
ngx_get_cached_block(size_t size)
{
    வெறுமை                     *p;
    ngx_cached_block_slot_t  *slot;

    எனில் (ngx_cycle->cache == வெற்று) {
        திரும்பு வெற்று;
    }

    slot = &ngx_cycle->cache[(size + ngx_pagesize - 1) / ngx_pagesize];

    slot->tries++;

    எனில் (slot->எண்) {
        p = slot->block;
        slot->block = slot->block->next;
        slot->எண்--;
        திரும்பு p;
    }

    திரும்பு வெற்று;
}

#endif
