
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
    ꦪꦼꦤ꧀ (p == NULL) {
        ꦮꦭꦶ NULL;
    }

    p->d.last = (u_char *) p + parimaa_na(ngx_pool_t);
    p->d.end = (u_char *) p + size;
    p->d.next = NULL;
    p->d.failed = 0;

    size = size - parimaa_na(ngx_pool_t);
    p->max = (size < NGX_MAX_ALLOC_FROM_POOL) ? size : NGX_MAX_ALLOC_FROM_POOL;

    p->current = p;
    p->chain = NULL;
    p->large = NULL;
    p->cleanup = NULL;
    p->log = log;

    ꦮꦭꦶ p;
}


void
ngx_destroy_pool(ngx_pool_t *pool)
{
    ngx_pool_t          *p, *n;
    ngx_pool_large_t    *l;
    ngx_pool_cleanup_t  *c;

    ꦏꦁꦒꦺꦴ (c = pool->cleanup; c; c = c->next) {
        ꦪꦼꦤ꧀ (c->handler) {
            ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0,
                           "run cleanup: %p", c);
            c->handler(c->data);
        }
    }

#ꦪꦼꦤ꧀ (NGX_DEBUG)

    /*
     * we could allocate the pool->log from this pool
     * so we cannot use this log while free()ing the pool
     */

    ꦏꦁꦒꦺꦴ (l = pool->large; l; l = l->next) {
        ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0, "free: %p", l->alloc);
    }

    ꦏꦁꦒꦺꦴ (p = pool, n = pool->d.next; /* void */; p = n, n = n->d.next) {
        ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, pool->log, 0,
                       "free: %p, unused: %uz", p, p->d.end - p->d.last);

        ꦪꦼꦤ꧀ (n == NULL) {
            bha_nga;
        }
    }

#endif

    ꦏꦁꦒꦺꦴ (l = pool->large; l; l = l->next) {
        ꦪꦼꦤ꧀ (l->alloc) {
            ngx_free(l->alloc);
        }
    }

    ꦏꦁꦒꦺꦴ (p = pool, n = pool->d.next; /* void */; p = n, n = n->d.next) {
        ngx_free(p);

        ꦪꦼꦤ꧀ (n == NULL) {
            bha_nga;
        }
    }
}


void
ngx_reset_pool(ngx_pool_t *pool)
{
    ngx_pool_t        *p;
    ngx_pool_large_t  *l;

    ꦏꦁꦒꦺꦴ (l = pool->large; l; l = l->next) {
        ꦪꦼꦤ꧀ (l->alloc) {
            ngx_free(l->alloc);
        }
    }

    ꦏꦁꦒꦺꦴ (p = pool; p; p = p->d.next) {
        p->d.last = (u_char *) p + parimaa_na(ngx_pool_t);
        p->d.failed = 0;
    }

    pool->current = pool;
    pool->chain = NULL;
    pool->large = NULL;
}


void *
ngx_palloc(ngx_pool_t *pool, size_t size)
{
#ꦪꦼꦤ꧀ !(NGX_DEBUG_PALLOC)
    ꦪꦼꦤ꧀ (size <= pool->max) {
        ꦮꦭꦶ ngx_palloc_small(pool, size, 1);
    }
#endif

    ꦮꦭꦶ ngx_palloc_large(pool, size);
}


void *
ngx_pnalloc(ngx_pool_t *pool, size_t size)
{
#ꦪꦼꦤ꧀ !(NGX_DEBUG_PALLOC)
    ꦪꦼꦤ꧀ (size <= pool->max) {
        ꦮꦭꦶ ngx_palloc_small(pool, size, 0);
    }
#endif

    ꦮꦭꦶ ngx_palloc_large(pool, size);
}


static ngx_inline void *
ngx_palloc_small(ngx_pool_t *pool, size_t size, ngx_uint_t align)
{
    u_char      *m;
    ngx_pool_t  *p;

    p = pool->current;

    do {
        m = p->d.last;

        ꦪꦼꦤ꧀ (align) {
            m = ngx_align_ptr(m, NGX_ALIGNMENT);
        }

        ꦪꦼꦤ꧀ ((size_t) (p->d.end - m) >= size) {
            p->d.last = m + size;

            ꦮꦭꦶ m;
        }

        p = p->d.next;

    } ꦱꦼꦢꦼꦭ (p);

    ꦮꦭꦶ ngx_palloc_block(pool, size);
}


static void *
ngx_palloc_block(ngx_pool_t *pool, size_t size)
{
    u_char      *m;
    size_t       psize;
    ngx_pool_t  *p, *new;

    psize = (size_t) (pool->d.end - (u_char *) pool);

    m = ngx_memalign(NGX_POOL_ALIGNMENT, psize, pool->log);
    ꦪꦼꦤ꧀ (m == NULL) {
        ꦮꦭꦶ NULL;
    }

    new = (ngx_pool_t *) m;

    new->d.end = m + psize;
    new->d.next = NULL;
    new->d.failed = 0;

    m += parimaa_na(ngx_pool_data_t);
    m = ngx_align_ptr(m, NGX_ALIGNMENT);
    new->d.last = m + size;

    ꦏꦁꦒꦺꦴ (p = pool->current; p->d.next; p = p->d.next) {
        ꦪꦼꦤ꧀ (p->d.failed++ > 4) {
            pool->current = p->d.next;
        }
    }

    p->d.next = new;

    ꦮꦭꦶ m;
}


static void *
ngx_palloc_large(ngx_pool_t *pool, size_t size)
{
    void              *p;
    ngx_uint_t         n;
    ngx_pool_large_t  *large;

    p = ngx_alloc(size, pool->log);
    ꦪꦼꦤ꧀ (p == NULL) {
        ꦮꦭꦶ NULL;
    }

    n = 0;

    ꦏꦁꦒꦺꦴ (large = pool->large; large; large = large->next) {
        ꦪꦼꦤ꧀ (large->alloc == NULL) {
            large->alloc = p;
            ꦮꦭꦶ p;
        }

        ꦪꦼꦤ꧀ (n++ > 3) {
            bha_nga;
        }
    }

    large = ngx_palloc_small(pool, parimaa_na(ngx_pool_large_t), 1);
    ꦪꦼꦤ꧀ (large == NULL) {
        ngx_free(p);
        ꦮꦭꦶ NULL;
    }

    large->alloc = p;
    large->next = pool->large;
    pool->large = large;

    ꦮꦭꦶ p;
}


void *
ngx_pmemalign(ngx_pool_t *pool, size_t size, size_t alignment)
{
    void              *p;
    ngx_pool_large_t  *large;

    p = ngx_memalign(alignment, size, pool->log);
    ꦪꦼꦤ꧀ (p == NULL) {
        ꦮꦭꦶ NULL;
    }

    large = ngx_palloc_small(pool, parimaa_na(ngx_pool_large_t), 1);
    ꦪꦼꦤ꧀ (large == NULL) {
        ngx_free(p);
        ꦮꦭꦶ NULL;
    }

    large->alloc = p;
    large->next = pool->large;
    pool->large = large;

    ꦮꦭꦶ p;
}


ngx_int_t
ngx_pfree(ngx_pool_t *pool, void *p)
{
    ngx_pool_large_t  *l;

    ꦏꦁꦒꦺꦴ (l = pool->large; l; l = l->next) {
        ꦪꦼꦤ꧀ (p == l->alloc) {
            ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0,
                           "free: %p", l->alloc);
            ngx_free(l->alloc);
            l->alloc = NULL;

            ꦮꦭꦶ NGX_OK;
        }
    }

    ꦮꦭꦶ NGX_DECLINED;
}


void *
ngx_pcalloc(ngx_pool_t *pool, size_t size)
{
    void *p;

    p = ngx_palloc(pool, size);
    ꦪꦼꦤ꧀ (p) {
        ngx_memzero(p, size);
    }

    ꦮꦭꦶ p;
}


ngx_pool_cleanup_t *
ngx_pool_cleanup_add(ngx_pool_t *p, size_t size)
{
    ngx_pool_cleanup_t  *c;

    c = ngx_palloc(p, parimaa_na(ngx_pool_cleanup_t));
    ꦪꦼꦤ꧀ (c == NULL) {
        ꦮꦭꦶ NULL;
    }

    ꦪꦼꦤ꧀ (size) {
        c->data = ngx_palloc(p, size);
        ꦪꦼꦤ꧀ (c->data == NULL) {
            ꦮꦭꦶ NULL;
        }

    } ꦭꦶꦪ {
        c->data = NULL;
    }

    c->handler = NULL;
    c->next = p->cleanup;

    p->cleanup = c;

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, p->log, 0, "add cleanup: %p", c);

    ꦮꦭꦶ c;
}


void
ngx_pool_run_cleanup_file(ngx_pool_t *p, ngx_fd_t fd)
{
    ngx_pool_cleanup_t       *c;
    ngx_pool_cleanup_file_t  *cf;

    ꦏꦁꦒꦺꦴ (c = p->cleanup; c; c = c->next) {
        ꦪꦼꦤ꧀ (c->handler == ngx_pool_cleanup_file) {

            cf = c->data;

            ꦪꦼꦤ꧀ (cf->fd == fd) {
                c->handler(cf);
                c->handler = NULL;
                ꦮꦭꦶ;
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

    ꦪꦼꦤ꧀ (ngx_close_file(c->fd) == NGX_FILE_ERROR) {
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

    ꦪꦼꦤ꧀ (ngx_delete_file(c->name) == NGX_FILE_ERROR) {
        err = ngx_errno;

        ꦪꦼꦤ꧀ (err != NGX_ENOENT) {
            ngx_log_error(NGX_LOG_CRIT, c->log, err,
                          ngx_delete_file_n " \"%s\" failed", c->name);
        }
    }

    ꦪꦼꦤ꧀ (ngx_close_file(c->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, c->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", c->name);
    }
}


#ꦪꦼꦤ꧀ 0

static void *
ngx_get_cached_block(size_t size)
{
    void                     *p;
    ngx_cached_block_slot_t  *slot;

    ꦪꦼꦤ꧀ (ngx_cycle->cache == NULL) {
        ꦮꦭꦶ NULL;
    }

    slot = &ngx_cycle->cache[(size + ngx_pagesize - 1) / ngx_pagesize];

    slot->tries++;

    ꦪꦼꦤ꧀ (slot->number) {
        p = slot->block;
        slot->block = slot->block->next;
        slot->number--;
        ꦮꦭꦶ p;
    }

    ꦮꦭꦶ NULL;
}

#endif
