
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>


static ngx_inline void *ngx_palloc_small(ngx_pool_t *pool, size_t size,
    ngx_uint_t align);
static void *ngx_palloc_block(ngx_pool_t *pool, size_t size);
static void *ngx_palloc_large(ngx_pool_t *pool, size_t size);


ngx_pool_t *
ngx_create_pool(size_t size, ngx_log_t *log)
{
    ngx_pool_t  *p;

    p = ngx_memalign(NGX_POOL_ALIGNMENT, size, log);
    만약 (p == NULL) {
        반환 NULL;
    }

    p->d.last = (u_char *) p + sizeof(ngx_pool_t);
    p->d.end = (u_char *) p + size;
    p->d.next = NULL;
    p->d.failed = 0;

    size = size - sizeof(ngx_pool_t);
    p->max = (size < NGX_MAX_ALLOC_FROM_POOL) ? size : NGX_MAX_ALLOC_FROM_POOL;

    p->current = p;
    p->chain = NULL;
    p->large = NULL;
    p->cleanup = NULL;
    p->log = log;

    반환 p;
}


void
ngx_destroy_pool(ngx_pool_t *pool)
{
    ngx_pool_t          *p, *n;
    ngx_pool_large_t    *l;
    ngx_pool_cleanup_t  *c;

    위해 (c = pool->cleanup; c; c = c->next) {
        만약 (c->handler) {
            ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0,
                           "run cleanup: %p", c);
            c->handler(c->data);
        }
    }

#만약 (NGX_DEBUG)

    /*
     * we could allocate the pool->log from this pool
     * so we cannot use this log while free()ing the pool
     */

    위해 (l = pool->large; l; l = l->next) {
        ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0, "free: %p", l->alloc);
    }

    위해 (p = pool, n = pool->d.next; /* void */; p = n, n = n->d.next) {
        ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, pool->log, 0,
                       "free: %p, unused: %uz", p, p->d.end - p->d.last);

        만약 (n == NULL) {
            break;
        }
    }

#endif

    위해 (l = pool->large; l; l = l->next) {
        만약 (l->alloc) {
            ngx_free(l->alloc);
        }
    }

    위해 (p = pool, n = pool->d.next; /* void */; p = n, n = n->d.next) {
        ngx_free(p);

        만약 (n == NULL) {
            break;
        }
    }
}


void
ngx_reset_pool(ngx_pool_t *pool)
{
    ngx_pool_t        *p;
    ngx_pool_large_t  *l;

    위해 (l = pool->large; l; l = l->next) {
        만약 (l->alloc) {
            ngx_free(l->alloc);
        }
    }

    위해 (p = pool; p; p = p->d.next) {
        p->d.last = (u_char *) p + sizeof(ngx_pool_t);
        p->d.failed = 0;
    }

    pool->current = pool;
    pool->chain = NULL;
    pool->large = NULL;
}


void *
ngx_palloc(ngx_pool_t *pool, size_t size)
{
#만약 !(NGX_DEBUG_PALLOC)
    만약 (size <= pool->max) {
        반환 ngx_palloc_small(pool, size, 1);
    }
#endif

    반환 ngx_palloc_large(pool, size);
}


void *
ngx_pnalloc(ngx_pool_t *pool, size_t size)
{
#만약 !(NGX_DEBUG_PALLOC)
    만약 (size <= pool->max) {
        반환 ngx_palloc_small(pool, size, 0);
    }
#endif

    반환 ngx_palloc_large(pool, size);
}


static ngx_inline void *
ngx_palloc_small(ngx_pool_t *pool, size_t size, ngx_uint_t align)
{
    u_char      *m;
    ngx_pool_t  *p;

    p = pool->current;

    do {
        m = p->d.last;

        만약 (align) {
            m = ngx_align_ptr(m, NGX_ALIGNMENT);
        }

        만약 ((size_t) (p->d.end - m) >= size) {
            p->d.last = m + size;

            반환 m;
        }

        p = p->d.next;

    } 동안 (p);

    반환 ngx_palloc_block(pool, size);
}


static void *
ngx_palloc_block(ngx_pool_t *pool, size_t size)
{
    u_char      *m;
    size_t       psize;
    ngx_pool_t  *p, *new;

    psize = (size_t) (pool->d.end - (u_char *) pool);

    m = ngx_memalign(NGX_POOL_ALIGNMENT, psize, pool->log);
    만약 (m == NULL) {
        반환 NULL;
    }

    new = (ngx_pool_t *) m;

    new->d.end = m + psize;
    new->d.next = NULL;
    new->d.failed = 0;

    m += sizeof(ngx_pool_data_t);
    m = ngx_align_ptr(m, NGX_ALIGNMENT);
    new->d.last = m + size;

    위해 (p = pool->current; p->d.next; p = p->d.next) {
        만약 (p->d.failed++ > 4) {
            pool->current = p->d.next;
        }
    }

    p->d.next = new;

    반환 m;
}


static void *
ngx_palloc_large(ngx_pool_t *pool, size_t size)
{
    void              *p;
    ngx_uint_t         n;
    ngx_pool_large_t  *large;

    p = ngx_alloc(size, pool->log);
    만약 (p == NULL) {
        반환 NULL;
    }

    n = 0;

    위해 (large = pool->large; large; large = large->next) {
        만약 (large->alloc == NULL) {
            large->alloc = p;
            반환 p;
        }

        만약 (n++ > 3) {
            break;
        }
    }

    large = ngx_palloc_small(pool, sizeof(ngx_pool_large_t), 1);
    만약 (large == NULL) {
        ngx_free(p);
        반환 NULL;
    }

    large->alloc = p;
    large->next = pool->large;
    pool->large = large;

    반환 p;
}


void *
ngx_pmemalign(ngx_pool_t *pool, size_t size, size_t alignment)
{
    void              *p;
    ngx_pool_large_t  *large;

    p = ngx_memalign(alignment, size, pool->log);
    만약 (p == NULL) {
        반환 NULL;
    }

    large = ngx_palloc_small(pool, sizeof(ngx_pool_large_t), 1);
    만약 (large == NULL) {
        ngx_free(p);
        반환 NULL;
    }

    large->alloc = p;
    large->next = pool->large;
    pool->large = large;

    반환 p;
}


ngx_int_t
ngx_pfree(ngx_pool_t *pool, void *p)
{
    ngx_pool_large_t  *l;

    위해 (l = pool->large; l; l = l->next) {
        만약 (p == l->alloc) {
            ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0,
                           "free: %p", l->alloc);
            ngx_free(l->alloc);
            l->alloc = NULL;

            반환 NGX_OK;
        }
    }

    반환 NGX_DECLINED;
}


void *
ngx_pcalloc(ngx_pool_t *pool, size_t size)
{
    void *p;

    p = ngx_palloc(pool, size);
    만약 (p) {
        ngx_memzero(p, size);
    }

    반환 p;
}


ngx_pool_cleanup_t *
ngx_pool_cleanup_add(ngx_pool_t *p, size_t size)
{
    ngx_pool_cleanup_t  *c;

    c = ngx_palloc(p, sizeof(ngx_pool_cleanup_t));
    만약 (c == NULL) {
        반환 NULL;
    }

    만약 (size) {
        c->data = ngx_palloc(p, size);
        만약 (c->data == NULL) {
            반환 NULL;
        }

    } 아니면 {
        c->data = NULL;
    }

    c->handler = NULL;
    c->next = p->cleanup;

    p->cleanup = c;

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, p->log, 0, "add cleanup: %p", c);

    반환 c;
}


void
ngx_pool_run_cleanup_file(ngx_pool_t *p, ngx_fd_t fd)
{
    ngx_pool_cleanup_t       *c;
    ngx_pool_cleanup_file_t  *cf;

    위해 (c = p->cleanup; c; c = c->next) {
        만약 (c->handler == ngx_pool_cleanup_file) {

            cf = c->data;

            만약 (cf->fd == fd) {
                c->handler(cf);
                c->handler = NULL;
                반환;
            }
        }
    }
}


void
ngx_pool_cleanup_file(void *data)
{
    ngx_pool_cleanup_file_t  *c = data;

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, c->log, 0, "file cleanup: fd:%d",
                   c->fd);

    만약 (ngx_close_file(c->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, c->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", c->name);
    }
}


void
ngx_pool_delete_file(void *data)
{
    ngx_pool_cleanup_file_t  *c = data;

    ngx_err_t  err;

    ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, c->log, 0, "file cleanup: fd:%d %s",
                   c->fd, c->name);

    만약 (ngx_delete_file(c->name) == NGX_FILE_ERROR) {
        err = ngx_errno;

        만약 (err != NGX_ENOENT) {
            ngx_log_error(NGX_LOG_CRIT, c->log, err,
                          ngx_delete_file_n " \"%s\" failed", c->name);
        }
    }

    만약 (ngx_close_file(c->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, c->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", c->name);
    }
}


#만약 0

static void *
ngx_get_cached_block(size_t size)
{
    void                     *p;
    ngx_cached_block_slot_t  *slot;

    만약 (ngx_cycle->cache == NULL) {
        반환 NULL;
    }

    slot = &ngx_cycle->cache[(size + ngx_pagesize - 1) / ngx_pagesize];

    slot->tries++;

    만약 (slot->number) {
        p = slot->block;
        slot->block = slot->block->next;
        slot->number--;
        반환 p;
    }

    반환 NULL;
}

#endif
