
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
    ถ้า (p == NULL) {
        คืน NULL;
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

    คืน p;
}


void
ngx_destroy_pool(ngx_pool_t *pool)
{
    ngx_pool_t          *p, *n;
    ngx_pool_large_t    *l;
    ngx_pool_cleanup_t  *c;

    สำหรับ (c = pool->cleanup; c; c = c->next) {
        ถ้า (c->handler) {
            ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0,
                           "run cleanup: %p", c);
            c->handler(c->data);
        }
    }

#ถ้า (NGX_DEBUG)

    /*
     * we could allocate the pool->log from this pool
     * so we cannot use this log while free()ing the pool
     */

    สำหรับ (l = pool->large; l; l = l->next) {
        ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0, "free: %p", l->alloc);
    }

    สำหรับ (p = pool, n = pool->d.next; /* void */; p = n, n = n->d.next) {
        ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, pool->log, 0,
                       "free: %p, unused: %uz", p, p->d.end - p->d.last);

        ถ้า (n == NULL) {
            bha_nga;
        }
    }

#endif

    สำหรับ (l = pool->large; l; l = l->next) {
        ถ้า (l->alloc) {
            ngx_free(l->alloc);
        }
    }

    สำหรับ (p = pool, n = pool->d.next; /* void */; p = n, n = n->d.next) {
        ngx_free(p);

        ถ้า (n == NULL) {
            bha_nga;
        }
    }
}


void
ngx_reset_pool(ngx_pool_t *pool)
{
    ngx_pool_t        *p;
    ngx_pool_large_t  *l;

    สำหรับ (l = pool->large; l; l = l->next) {
        ถ้า (l->alloc) {
            ngx_free(l->alloc);
        }
    }

    สำหรับ (p = pool; p; p = p->d.next) {
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
#ถ้า !(NGX_DEBUG_PALLOC)
    ถ้า (size <= pool->max) {
        คืน ngx_palloc_small(pool, size, 1);
    }
#endif

    คืน ngx_palloc_large(pool, size);
}


void *
ngx_pnalloc(ngx_pool_t *pool, size_t size)
{
#ถ้า !(NGX_DEBUG_PALLOC)
    ถ้า (size <= pool->max) {
        คืน ngx_palloc_small(pool, size, 0);
    }
#endif

    คืน ngx_palloc_large(pool, size);
}


static ngx_inline void *
ngx_palloc_small(ngx_pool_t *pool, size_t size, ngx_uint_t align)
{
    u_char      *m;
    ngx_pool_t  *p;

    p = pool->current;

    do {
        m = p->d.last;

        ถ้า (align) {
            m = ngx_align_ptr(m, NGX_ALIGNMENT);
        }

        ถ้า ((size_t) (p->d.end - m) >= size) {
            p->d.last = m + size;

            คืน m;
        }

        p = p->d.next;

    } ขณะ (p);

    คืน ngx_palloc_block(pool, size);
}


static void *
ngx_palloc_block(ngx_pool_t *pool, size_t size)
{
    u_char      *m;
    size_t       psize;
    ngx_pool_t  *p, *new;

    psize = (size_t) (pool->d.end - (u_char *) pool);

    m = ngx_memalign(NGX_POOL_ALIGNMENT, psize, pool->log);
    ถ้า (m == NULL) {
        คืน NULL;
    }

    new = (ngx_pool_t *) m;

    new->d.end = m + psize;
    new->d.next = NULL;
    new->d.failed = 0;

    m += parimaa_na(ngx_pool_data_t);
    m = ngx_align_ptr(m, NGX_ALIGNMENT);
    new->d.last = m + size;

    สำหรับ (p = pool->current; p->d.next; p = p->d.next) {
        ถ้า (p->d.failed++ > 4) {
            pool->current = p->d.next;
        }
    }

    p->d.next = new;

    คืน m;
}


static void *
ngx_palloc_large(ngx_pool_t *pool, size_t size)
{
    void              *p;
    ngx_uint_t         n;
    ngx_pool_large_t  *large;

    p = ngx_alloc(size, pool->log);
    ถ้า (p == NULL) {
        คืน NULL;
    }

    n = 0;

    สำหรับ (large = pool->large; large; large = large->next) {
        ถ้า (large->alloc == NULL) {
            large->alloc = p;
            คืน p;
        }

        ถ้า (n++ > 3) {
            bha_nga;
        }
    }

    large = ngx_palloc_small(pool, parimaa_na(ngx_pool_large_t), 1);
    ถ้า (large == NULL) {
        ngx_free(p);
        คืน NULL;
    }

    large->alloc = p;
    large->next = pool->large;
    pool->large = large;

    คืน p;
}


void *
ngx_pmemalign(ngx_pool_t *pool, size_t size, size_t alignment)
{
    void              *p;
    ngx_pool_large_t  *large;

    p = ngx_memalign(alignment, size, pool->log);
    ถ้า (p == NULL) {
        คืน NULL;
    }

    large = ngx_palloc_small(pool, parimaa_na(ngx_pool_large_t), 1);
    ถ้า (large == NULL) {
        ngx_free(p);
        คืน NULL;
    }

    large->alloc = p;
    large->next = pool->large;
    pool->large = large;

    คืน p;
}


ngx_int_t
ngx_pfree(ngx_pool_t *pool, void *p)
{
    ngx_pool_large_t  *l;

    สำหรับ (l = pool->large; l; l = l->next) {
        ถ้า (p == l->alloc) {
            ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0,
                           "free: %p", l->alloc);
            ngx_free(l->alloc);
            l->alloc = NULL;

            คืน NGX_OK;
        }
    }

    คืน NGX_DECLINED;
}


void *
ngx_pcalloc(ngx_pool_t *pool, size_t size)
{
    void *p;

    p = ngx_palloc(pool, size);
    ถ้า (p) {
        ngx_memzero(p, size);
    }

    คืน p;
}


ngx_pool_cleanup_t *
ngx_pool_cleanup_add(ngx_pool_t *p, size_t size)
{
    ngx_pool_cleanup_t  *c;

    c = ngx_palloc(p, parimaa_na(ngx_pool_cleanup_t));
    ถ้า (c == NULL) {
        คืน NULL;
    }

    ถ้า (size) {
        c->data = ngx_palloc(p, size);
        ถ้า (c->data == NULL) {
            คืน NULL;
        }

    } มิฉะนั้น {
        c->data = NULL;
    }

    c->handler = NULL;
    c->next = p->cleanup;

    p->cleanup = c;

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, p->log, 0, "add cleanup: %p", c);

    คืน c;
}


void
ngx_pool_run_cleanup_file(ngx_pool_t *p, ngx_fd_t fd)
{
    ngx_pool_cleanup_t       *c;
    ngx_pool_cleanup_file_t  *cf;

    สำหรับ (c = p->cleanup; c; c = c->next) {
        ถ้า (c->handler == ngx_pool_cleanup_file) {

            cf = c->data;

            ถ้า (cf->fd == fd) {
                c->handler(cf);
                c->handler = NULL;
                คืน;
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

    ถ้า (ngx_close_file(c->fd) == NGX_FILE_ERROR) {
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

    ถ้า (ngx_delete_file(c->name) == NGX_FILE_ERROR) {
        err = ngx_errno;

        ถ้า (err != NGX_ENOENT) {
            ngx_log_error(NGX_LOG_CRIT, c->log, err,
                          ngx_delete_file_n " \"%s\" failed", c->name);
        }
    }

    ถ้า (ngx_close_file(c->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, c->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", c->name);
    }
}


#ถ้า 0

static void *
ngx_get_cached_block(size_t size)
{
    void                     *p;
    ngx_cached_block_slot_t  *slot;

    ถ้า (ngx_cycle->cache == NULL) {
        คืน NULL;
    }

    slot = &ngx_cycle->cache[(size + ngx_pagesize - 1) / ngx_pagesize];

    slot->tries++;

    ถ้า (slot->number) {
        p = slot->block;
        slot->block = slot->block->next;
        slot->number--;
        คืน p;
    }

    คืน NULL;
}

#endif
