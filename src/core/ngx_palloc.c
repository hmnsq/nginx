
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ଅନ୍ତର୍ଭାଵ <ngx_config.h>
#ଅନ୍ତର୍ଭାଵ <ngx_core.h>


ସ୍ଥିର ngx_inline ଶୂନ୍ୟ *ngx_palloc_small(ngx_pool_t *pool, size_t size,
    ngx_uint_t align);
ସ୍ଥିର ଶୂନ୍ୟ *ngx_palloc_block(ngx_pool_t *pool, size_t size);
ସ୍ଥିର ଶୂନ୍ୟ *ngx_palloc_large(ngx_pool_t *pool, size_t size);


ngx_pool_t *
ngx_create_pool(size_t size, ngx_log_t *log)
{
    ngx_pool_t  *p;

    p = ngx_memalign(NGX_POOL_ALIGNMENT, size, log);
    ଯଦି (p == ଖାଲି) {
        ଫେରନ୍ତୁ ଖାଲି;
    }

    p->d.last = (u_char *) p + ଆକାର(ngx_pool_t);
    p->d.end = (u_char *) p + size;
    p->d.next = ଖାଲି;
    p->d.failed = 0;

    size = size - ଆକାର(ngx_pool_t);
    p->max = (size < NGX_MAX_ALLOC_FROM_POOL) ? size : NGX_MAX_ALLOC_FROM_POOL;

    p->current = p;
    p->chain = ଖାଲି;
    p->large = ଖାଲି;
    p->cleanup = ଖାଲି;
    p->log = log;

    ଫେରନ୍ତୁ p;
}


ଶୂନ୍ୟ
ngx_destroy_pool(ngx_pool_t *pool)
{
    ngx_pool_t          *p, *n;
    ngx_pool_large_t    *l;
    ngx_pool_cleanup_t  *c;

    ପାଇଁ (c = pool->cleanup; c; c = c->next) {
        ଯଦି (c->handler) {
            ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0,
                           "run cleanup: %p", c);
            c->handler(c->data);
        }
    }

#ଯଦି (NGX_DEBUG)

    /*
     * we could allocate the pool->log from this pool
     * so we cannot use this log while free()ing the pool
     */

    ପାଇଁ (l = pool->large; l; l = l->next) {
        ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0, "free: %p", l->alloc);
    }

    ପାଇଁ (p = pool, n = pool->d.next; /* void */; p = n, n = n->d.next) {
        ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, pool->log, 0,
                       "free: %p, unused: %uz", p, p->d.end - p->d.last);

        ଯଦି (n == ଖାଲି) {
            ଭାଙ୍ଗ;
        }
    }

#endif

    ପାଇଁ (l = pool->large; l; l = l->next) {
        ଯଦି (l->alloc) {
            ngx_free(l->alloc);
        }
    }

    ପାଇଁ (p = pool, n = pool->d.next; /* void */; p = n, n = n->d.next) {
        ngx_free(p);

        ଯଦି (n == ଖାଲି) {
            ଭାଙ୍ଗ;
        }
    }
}


ଶୂନ୍ୟ
ngx_reset_pool(ngx_pool_t *pool)
{
    ngx_pool_t        *p;
    ngx_pool_large_t  *l;

    ପାଇଁ (l = pool->large; l; l = l->next) {
        ଯଦି (l->alloc) {
            ngx_free(l->alloc);
        }
    }

    ପାଇଁ (p = pool; p; p = p->d.next) {
        p->d.last = (u_char *) p + ଆକାର(ngx_pool_t);
        p->d.failed = 0;
    }

    pool->current = pool;
    pool->chain = ଖାଲି;
    pool->large = ଖାଲି;
}


ଶୂନ୍ୟ *
ngx_palloc(ngx_pool_t *pool, size_t size)
{
#ଯଦି !(NGX_DEBUG_PALLOC)
    ଯଦି (size <= pool->max) {
        ଫେରନ୍ତୁ ngx_palloc_small(pool, size, 1);
    }
#endif

    ଫେରନ୍ତୁ ngx_palloc_large(pool, size);
}


ଶୂନ୍ୟ *
ngx_pnalloc(ngx_pool_t *pool, size_t size)
{
#ଯଦି !(NGX_DEBUG_PALLOC)
    ଯଦି (size <= pool->max) {
        ଫେରନ୍ତୁ ngx_palloc_small(pool, size, 0);
    }
#endif

    ଫେରନ୍ତୁ ngx_palloc_large(pool, size);
}


ସ୍ଥିର ngx_inline ଶୂନ୍ୟ *
ngx_palloc_small(ngx_pool_t *pool, size_t size, ngx_uint_t align)
{
    u_char      *m;
    ngx_pool_t  *p;

    p = pool->current;

    କର {
        m = p->d.last;

        ଯଦି (align) {
            m = ngx_align_ptr(m, NGX_ALIGNMENT);
        }

        ଯଦି ((size_t) (p->d.end - m) >= size) {
            p->d.last = m + size;

            ଫେରନ୍ତୁ m;
        }

        p = p->d.next;

    } ଯେତେବେଳେ (p);

    ଫେରନ୍ତୁ ngx_palloc_block(pool, size);
}


ସ୍ଥିର ଶୂନ୍ୟ *
ngx_palloc_block(ngx_pool_t *pool, size_t size)
{
    u_char      *m;
    size_t       psize;
    ngx_pool_t  *p, *new;

    psize = (size_t) (pool->d.end - (u_char *) pool);

    m = ngx_memalign(NGX_POOL_ALIGNMENT, psize, pool->log);
    ଯଦି (m == ଖାଲି) {
        ଫେରନ୍ତୁ ଖାଲି;
    }

    new = (ngx_pool_t *) m;

    new->d.end = m + psize;
    new->d.next = ଖାଲି;
    new->d.failed = 0;

    m += ଆକାର(ngx_pool_data_t);
    m = ngx_align_ptr(m, NGX_ALIGNMENT);
    new->d.last = m + size;

    ପାଇଁ (p = pool->current; p->d.next; p = p->d.next) {
        ଯଦି (p->d.failed++ > 4) {
            pool->current = p->d.next;
        }
    }

    p->d.next = new;

    ଫେରନ୍ତୁ m;
}


ସ୍ଥିର ଶୂନ୍ୟ *
ngx_palloc_large(ngx_pool_t *pool, size_t size)
{
    ଶୂନ୍ୟ              *p;
    ngx_uint_t         n;
    ngx_pool_large_t  *large;

    p = ngx_alloc(size, pool->log);
    ଯଦି (p == ଖାଲି) {
        ଫେରନ୍ତୁ ଖାଲି;
    }

    n = 0;

    ପାଇଁ (large = pool->large; large; large = large->next) {
        ଯଦି (large->alloc == ଖାଲି) {
            large->alloc = p;
            ଫେରନ୍ତୁ p;
        }

        ଯଦି (n++ > 3) {
            ଭାଙ୍ଗ;
        }
    }

    large = ngx_palloc_small(pool, ଆକାର(ngx_pool_large_t), 1);
    ଯଦି (large == ଖାଲି) {
        ngx_free(p);
        ଫେରନ୍ତୁ ଖାଲି;
    }

    large->alloc = p;
    large->next = pool->large;
    pool->large = large;

    ଫେରନ୍ତୁ p;
}


ଶୂନ୍ୟ *
ngx_pmemalign(ngx_pool_t *pool, size_t size, size_t alignment)
{
    ଶୂନ୍ୟ              *p;
    ngx_pool_large_t  *large;

    p = ngx_memalign(alignment, size, pool->log);
    ଯଦି (p == ଖାଲି) {
        ଫେରନ୍ତୁ ଖାଲି;
    }

    large = ngx_palloc_small(pool, ଆକାର(ngx_pool_large_t), 1);
    ଯଦି (large == ଖାଲି) {
        ngx_free(p);
        ଫେରନ୍ତୁ ଖାଲି;
    }

    large->alloc = p;
    large->next = pool->large;
    pool->large = large;

    ଫେରନ୍ତୁ p;
}


ngx_int_t
ngx_pfree(ngx_pool_t *pool, ଶୂନ୍ୟ *p)
{
    ngx_pool_large_t  *l;

    ପାଇଁ (l = pool->large; l; l = l->next) {
        ଯଦି (p == l->alloc) {
            ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0,
                           "free: %p", l->alloc);
            ngx_free(l->alloc);
            l->alloc = ଖାଲି;

            ଫେରନ୍ତୁ NGX_OK;
        }
    }

    ଫେରନ୍ତୁ NGX_DECLINED;
}


ଶୂନ୍ୟ *
ngx_pcalloc(ngx_pool_t *pool, size_t size)
{
    ଶୂନ୍ୟ *p;

    p = ngx_palloc(pool, size);
    ଯଦି (p) {
        ngx_memzero(p, size);
    }

    ଫେରନ୍ତୁ p;
}


ngx_pool_cleanup_t *
ngx_pool_cleanup_add(ngx_pool_t *p, size_t size)
{
    ngx_pool_cleanup_t  *c;

    c = ngx_palloc(p, ଆକାର(ngx_pool_cleanup_t));
    ଯଦି (c == ଖାଲି) {
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ଯଦି (size) {
        c->data = ngx_palloc(p, size);
        ଯଦି (c->data == ଖାଲି) {
            ଫେରନ୍ତୁ ଖାଲି;
        }

    } ଅନ୍ୟଥା {
        c->data = ଖାଲି;
    }

    c->handler = ଖାଲି;
    c->next = p->cleanup;

    p->cleanup = c;

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, p->log, 0, "add cleanup: %p", c);

    ଫେରନ୍ତୁ c;
}


ଶୂନ୍ୟ
ngx_pool_run_cleanup_file(ngx_pool_t *p, ngx_fd_t fd)
{
    ngx_pool_cleanup_t       *c;
    ngx_pool_cleanup_file_t  *cf;

    ପାଇଁ (c = p->cleanup; c; c = c->next) {
        ଯଦି (c->handler == ngx_pool_cleanup_file) {

            cf = c->data;

            ଯଦି (cf->fd == fd) {
                c->handler(cf);
                c->handler = ଖାଲି;
                ଫେରନ୍ତୁ;
            }
        }
    }
}


ଶୂନ୍ୟ
ngx_pool_cleanup_file(ଶୂନ୍ୟ *data)
{
    ngx_pool_cleanup_file_t  *c = data;

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, c->log, 0, "file cleanup: fd:%d",
                   c->fd);

    ଯଦି (ngx_close_file(c->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, c->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", c->name);
    }
}


ଶୂନ୍ୟ
ngx_pool_delete_file(ଶୂନ୍ୟ *data)
{
    ngx_pool_cleanup_file_t  *c = data;

    ngx_err_t  err;

    ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, c->log, 0, "file cleanup: fd:%d %s",
                   c->fd, c->name);

    ଯଦି (ngx_delete_file(c->name) == NGX_FILE_ERROR) {
        err = ngx_errno;

        ଯଦି (err != NGX_ENOENT) {
            ngx_log_error(NGX_LOG_CRIT, c->log, err,
                          ngx_delete_file_n " \"%s\" failed", c->name);
        }
    }

    ଯଦି (ngx_close_file(c->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, c->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", c->name);
    }
}


#ଯଦି 0

ସ୍ଥିର ଶୂନ୍ୟ *
ngx_get_cached_block(size_t size)
{
    ଶୂନ୍ୟ                     *p;
    ngx_cached_block_slot_t  *slot;

    ଯଦି (ngx_cycle->cache == ଖାଲି) {
        ଫେରନ୍ତୁ ଖାଲି;
    }

    slot = &ngx_cycle->cache[(size + ngx_pagesize - 1) / ngx_pagesize];

    slot->tries++;

    ଯଦି (slot->ସଂଖ୍ୟା) {
        p = slot->block;
        slot->block = slot->block->next;
        slot->ସଂଖ୍ୟା--;
        ଫେରନ୍ତୁ p;
    }

    ଫେରନ୍ତୁ ଖାଲି;
}

#endif
