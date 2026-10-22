
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


ساكن ngx_inline فراغ *ngx_palloc_small(ngx_pool_t *pool, size_t size,
    ngx_uint_t align);
ساكن فراغ *ngx_palloc_block(ngx_pool_t *pool, size_t size);
ساكن فراغ *ngx_palloc_large(ngx_pool_t *pool, size_t size);


ngx_pool_t *
ngx_create_pool(size_t size, ngx_log_t *log)
{
    ngx_pool_t  *p;

    p = ngx_memalign(NGX_POOL_ALIGNMENT, size, log);
    אם (p == عدم) {
        החזר عدم;
    }

    p->d.last = (u_char *) p + حجم(ngx_pool_t);
    p->d.end = (u_char *) p + size;
    p->d.next = عدم;
    p->d.failed = 0;

    size = size - حجم(ngx_pool_t);
    p->max = (size < NGX_MAX_ALLOC_FROM_POOL) ? size : NGX_MAX_ALLOC_FROM_POOL;

    p->current = p;
    p->chain = عدم;
    p->large = عدم;
    p->cleanup = عدم;
    p->log = log;

    החזר p;
}


فراغ
ngx_destroy_pool(ngx_pool_t *pool)
{
    ngx_pool_t          *p, *n;
    ngx_pool_large_t    *l;
    ngx_pool_cleanup_t  *c;

    עבור (c = pool->cleanup; c; c = c->next) {
        אם (c->handler) {
            ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0,
                           "run cleanup: %p", c);
            c->handler(c->data);
        }
    }

#אם (NGX_DEBUG)

    /*
     * we could allocate the pool->log from this pool
     * so we cannot use this log while free()ing the pool
     */

    עבור (l = pool->large; l; l = l->next) {
        ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0, "free: %p", l->alloc);
    }

    עבור (p = pool, n = pool->d.next; /* void */; p = n, n = n->d.next) {
        ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, pool->log, 0,
                       "free: %p, unused: %uz", p, p->d.end - p->d.last);

        אם (n == عدم) {
            اكسر;
        }
    }

#endif

    עבור (l = pool->large; l; l = l->next) {
        אם (l->alloc) {
            ngx_free(l->alloc);
        }
    }

    עבור (p = pool, n = pool->d.next; /* void */; p = n, n = n->d.next) {
        ngx_free(p);

        אם (n == عدم) {
            اكسر;
        }
    }
}


فراغ
ngx_reset_pool(ngx_pool_t *pool)
{
    ngx_pool_t        *p;
    ngx_pool_large_t  *l;

    עבור (l = pool->large; l; l = l->next) {
        אם (l->alloc) {
            ngx_free(l->alloc);
        }
    }

    עבור (p = pool; p; p = p->d.next) {
        p->d.last = (u_char *) p + حجم(ngx_pool_t);
        p->d.failed = 0;
    }

    pool->current = pool;
    pool->chain = عدم;
    pool->large = عدم;
}


فراغ *
ngx_palloc(ngx_pool_t *pool, size_t size)
{
#אם !(NGX_DEBUG_PALLOC)
    אם (size <= pool->max) {
        החזר ngx_palloc_small(pool, size, 1);
    }
#endif

    החזר ngx_palloc_large(pool, size);
}


فراغ *
ngx_pnalloc(ngx_pool_t *pool, size_t size)
{
#אם !(NGX_DEBUG_PALLOC)
    אם (size <= pool->max) {
        החזר ngx_palloc_small(pool, size, 0);
    }
#endif

    החזר ngx_palloc_large(pool, size);
}


ساكن ngx_inline فراغ *
ngx_palloc_small(ngx_pool_t *pool, size_t size, ngx_uint_t align)
{
    u_char      *m;
    ngx_pool_t  *p;

    p = pool->current;

    עשה {
        m = p->d.last;

        אם (align) {
            m = ngx_align_ptr(m, NGX_ALIGNMENT);
        }

        אם ((size_t) (p->d.end - m) >= size) {
            p->d.last = m + size;

            החזר m;
        }

        p = p->d.next;

    } כלעוד (p);

    החזר ngx_palloc_block(pool, size);
}


ساكن فراغ *
ngx_palloc_block(ngx_pool_t *pool, size_t size)
{
    u_char      *m;
    size_t       psize;
    ngx_pool_t  *p, *new;

    psize = (size_t) (pool->d.end - (u_char *) pool);

    m = ngx_memalign(NGX_POOL_ALIGNMENT, psize, pool->log);
    אם (m == عدم) {
        החזר عدم;
    }

    new = (ngx_pool_t *) m;

    new->d.end = m + psize;
    new->d.next = عدم;
    new->d.failed = 0;

    m += حجم(ngx_pool_data_t);
    m = ngx_align_ptr(m, NGX_ALIGNMENT);
    new->d.last = m + size;

    עבור (p = pool->current; p->d.next; p = p->d.next) {
        אם (p->d.failed++ > 4) {
            pool->current = p->d.next;
        }
    }

    p->d.next = new;

    החזר m;
}


ساكن فراغ *
ngx_palloc_large(ngx_pool_t *pool, size_t size)
{
    فراغ              *p;
    ngx_uint_t         n;
    ngx_pool_large_t  *large;

    p = ngx_alloc(size, pool->log);
    אם (p == عدم) {
        החזר عدم;
    }

    n = 0;

    עבור (large = pool->large; large; large = large->next) {
        אם (large->alloc == عدم) {
            large->alloc = p;
            החזר p;
        }

        אם (n++ > 3) {
            اكسر;
        }
    }

    large = ngx_palloc_small(pool, حجم(ngx_pool_large_t), 1);
    אם (large == عدم) {
        ngx_free(p);
        החזר عدم;
    }

    large->alloc = p;
    large->next = pool->large;
    pool->large = large;

    החזר p;
}


فراغ *
ngx_pmemalign(ngx_pool_t *pool, size_t size, size_t alignment)
{
    فراغ              *p;
    ngx_pool_large_t  *large;

    p = ngx_memalign(alignment, size, pool->log);
    אם (p == عدم) {
        החזר عدم;
    }

    large = ngx_palloc_small(pool, حجم(ngx_pool_large_t), 1);
    אם (large == عدم) {
        ngx_free(p);
        החזר عدم;
    }

    large->alloc = p;
    large->next = pool->large;
    pool->large = large;

    החזר p;
}


ngx_int_t
ngx_pfree(ngx_pool_t *pool, فراغ *p)
{
    ngx_pool_large_t  *l;

    עבור (l = pool->large; l; l = l->next) {
        אם (p == l->alloc) {
            ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0,
                           "free: %p", l->alloc);
            ngx_free(l->alloc);
            l->alloc = عدم;

            החזר NGX_OK;
        }
    }

    החזר NGX_DECLINED;
}


فراغ *
ngx_pcalloc(ngx_pool_t *pool, size_t size)
{
    فراغ *p;

    p = ngx_palloc(pool, size);
    אם (p) {
        ngx_memzero(p, size);
    }

    החזר p;
}


ngx_pool_cleanup_t *
ngx_pool_cleanup_add(ngx_pool_t *p, size_t size)
{
    ngx_pool_cleanup_t  *c;

    c = ngx_palloc(p, حجم(ngx_pool_cleanup_t));
    אם (c == عدم) {
        החזר عدم;
    }

    אם (size) {
        c->data = ngx_palloc(p, size);
        אם (c->data == عدم) {
            החזר عدم;
        }

    } אחרת {
        c->data = عدم;
    }

    c->handler = عدم;
    c->next = p->cleanup;

    p->cleanup = c;

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, p->log, 0, "add cleanup: %p", c);

    החזר c;
}


فراغ
ngx_pool_run_cleanup_file(ngx_pool_t *p, ngx_fd_t fd)
{
    ngx_pool_cleanup_t       *c;
    ngx_pool_cleanup_file_t  *cf;

    עבור (c = p->cleanup; c; c = c->next) {
        אם (c->handler == ngx_pool_cleanup_file) {

            cf = c->data;

            אם (cf->fd == fd) {
                c->handler(cf);
                c->handler = عدم;
                החזר;
            }
        }
    }
}


فراغ
ngx_pool_cleanup_file(فراغ *data)
{
    ngx_pool_cleanup_file_t  *c = data;

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, c->log, 0, "file cleanup: fd:%d",
                   c->fd);

    אם (ngx_close_file(c->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, c->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", c->name);
    }
}


فراغ
ngx_pool_delete_file(فراغ *data)
{
    ngx_pool_cleanup_file_t  *c = data;

    ngx_err_t  err;

    ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, c->log, 0, "file cleanup: fd:%d %s",
                   c->fd, c->name);

    אם (ngx_delete_file(c->name) == NGX_FILE_ERROR) {
        err = ngx_errno;

        אם (err != NGX_ENOENT) {
            ngx_log_error(NGX_LOG_CRIT, c->log, err,
                          ngx_delete_file_n " \"%s\" failed", c->name);
        }
    }

    אם (ngx_close_file(c->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, c->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", c->name);
    }
}


#אם 0

ساكن فراغ *
ngx_get_cached_block(size_t size)
{
    فراغ                     *p;
    ngx_cached_block_slot_t  *slot;

    אם (ngx_cycle->cache == عدم) {
        החזר عدم;
    }

    slot = &ngx_cycle->cache[(size + ngx_pagesize - 1) / ngx_pagesize];

    slot->tries++;

    אם (slot->number) {
        p = slot->block;
        slot->block = slot->block->next;
        slot->number--;
        החזר p;
    }

    החזר عدم;
}

#endif
