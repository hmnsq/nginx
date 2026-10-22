
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#અન્તર્ભાવ <ngx_config.h>
#અન્તર્ભાવ <ngx_core.h>


સ્થિર ngx_inline શૂન્ય *ngx_palloc_small(ngx_pool_t *pool, size_t size,
    ngx_uint_t align);
સ્થિર શૂન્ય *ngx_palloc_block(ngx_pool_t *pool, size_t size);
સ્થિર શૂન્ય *ngx_palloc_large(ngx_pool_t *pool, size_t size);


ngx_pool_t *
ngx_create_pool(size_t size, ngx_log_t *log)
{
    ngx_pool_t  *p;

    p = ngx_memalign(NGX_POOL_ALIGNMENT, size, log);
    જો (p == ખાલી) {
        પાછા ખાલી;
    }

    p->d.last = (u_char *) p + કદ(ngx_pool_t);
    p->d.end = (u_char *) p + size;
    p->d.next = ખાલી;
    p->d.failed = 0;

    size = size - કદ(ngx_pool_t);
    p->max = (size < NGX_MAX_ALLOC_FROM_POOL) ? size : NGX_MAX_ALLOC_FROM_POOL;

    p->current = p;
    p->chain = ખાલી;
    p->large = ખાલી;
    p->cleanup = ખાલી;
    p->log = log;

    પાછા p;
}


શૂન્ય
ngx_destroy_pool(ngx_pool_t *pool)
{
    ngx_pool_t          *p, *n;
    ngx_pool_large_t    *l;
    ngx_pool_cleanup_t  *c;

    માટે (c = pool->cleanup; c; c = c->next) {
        જો (c->handler) {
            ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0,
                           "run cleanup: %p", c);
            c->handler(c->data);
        }
    }

#જો (NGX_DEBUG)

    /*
     * we could allocate the pool->log from this pool
     * so we cannot use this log while free()ing the pool
     */

    માટે (l = pool->large; l; l = l->next) {
        ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0, "free: %p", l->alloc);
    }

    માટે (p = pool, n = pool->d.next; /* void */; p = n, n = n->d.next) {
        ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, pool->log, 0,
                       "free: %p, unused: %uz", p, p->d.end - p->d.last);

        જો (n == ખાલી) {
            તોડો;
        }
    }

#endif

    માટે (l = pool->large; l; l = l->next) {
        જો (l->alloc) {
            ngx_free(l->alloc);
        }
    }

    માટે (p = pool, n = pool->d.next; /* void */; p = n, n = n->d.next) {
        ngx_free(p);

        જો (n == ખાલી) {
            તોડો;
        }
    }
}


શૂન્ય
ngx_reset_pool(ngx_pool_t *pool)
{
    ngx_pool_t        *p;
    ngx_pool_large_t  *l;

    માટે (l = pool->large; l; l = l->next) {
        જો (l->alloc) {
            ngx_free(l->alloc);
        }
    }

    માટે (p = pool; p; p = p->d.next) {
        p->d.last = (u_char *) p + કદ(ngx_pool_t);
        p->d.failed = 0;
    }

    pool->current = pool;
    pool->chain = ખાલી;
    pool->large = ખાલી;
}


શૂન્ય *
ngx_palloc(ngx_pool_t *pool, size_t size)
{
#જો !(NGX_DEBUG_PALLOC)
    જો (size <= pool->max) {
        પાછા ngx_palloc_small(pool, size, 1);
    }
#endif

    પાછા ngx_palloc_large(pool, size);
}


શૂન્ય *
ngx_pnalloc(ngx_pool_t *pool, size_t size)
{
#જો !(NGX_DEBUG_PALLOC)
    જો (size <= pool->max) {
        પાછા ngx_palloc_small(pool, size, 0);
    }
#endif

    પાછા ngx_palloc_large(pool, size);
}


સ્થિર ngx_inline શૂન્ય *
ngx_palloc_small(ngx_pool_t *pool, size_t size, ngx_uint_t align)
{
    u_char      *m;
    ngx_pool_t  *p;

    p = pool->current;

    કરો {
        m = p->d.last;

        જો (align) {
            m = ngx_align_ptr(m, NGX_ALIGNMENT);
        }

        જો ((size_t) (p->d.end - m) >= size) {
            p->d.last = m + size;

            પાછા m;
        }

        p = p->d.next;

    } જ્યારે (p);

    પાછા ngx_palloc_block(pool, size);
}


સ્થિર શૂન્ય *
ngx_palloc_block(ngx_pool_t *pool, size_t size)
{
    u_char      *m;
    size_t       psize;
    ngx_pool_t  *p, *new;

    psize = (size_t) (pool->d.end - (u_char *) pool);

    m = ngx_memalign(NGX_POOL_ALIGNMENT, psize, pool->log);
    જો (m == ખાલી) {
        પાછા ખાલી;
    }

    new = (ngx_pool_t *) m;

    new->d.end = m + psize;
    new->d.next = ખાલી;
    new->d.failed = 0;

    m += કદ(ngx_pool_data_t);
    m = ngx_align_ptr(m, NGX_ALIGNMENT);
    new->d.last = m + size;

    માટે (p = pool->current; p->d.next; p = p->d.next) {
        જો (p->d.failed++ > 4) {
            pool->current = p->d.next;
        }
    }

    p->d.next = new;

    પાછા m;
}


સ્થિર શૂન્ય *
ngx_palloc_large(ngx_pool_t *pool, size_t size)
{
    શૂન્ય              *p;
    ngx_uint_t         n;
    ngx_pool_large_t  *large;

    p = ngx_alloc(size, pool->log);
    જો (p == ખાલી) {
        પાછા ખાલી;
    }

    n = 0;

    માટે (large = pool->large; large; large = large->next) {
        જો (large->alloc == ખાલી) {
            large->alloc = p;
            પાછા p;
        }

        જો (n++ > 3) {
            તોડો;
        }
    }

    large = ngx_palloc_small(pool, કદ(ngx_pool_large_t), 1);
    જો (large == ખાલી) {
        ngx_free(p);
        પાછા ખાલી;
    }

    large->alloc = p;
    large->next = pool->large;
    pool->large = large;

    પાછા p;
}


શૂન્ય *
ngx_pmemalign(ngx_pool_t *pool, size_t size, size_t alignment)
{
    શૂન્ય              *p;
    ngx_pool_large_t  *large;

    p = ngx_memalign(alignment, size, pool->log);
    જો (p == ખાલી) {
        પાછા ખાલી;
    }

    large = ngx_palloc_small(pool, કદ(ngx_pool_large_t), 1);
    જો (large == ખાલી) {
        ngx_free(p);
        પાછા ખાલી;
    }

    large->alloc = p;
    large->next = pool->large;
    pool->large = large;

    પાછા p;
}


ngx_int_t
ngx_pfree(ngx_pool_t *pool, શૂન્ય *p)
{
    ngx_pool_large_t  *l;

    માટે (l = pool->large; l; l = l->next) {
        જો (p == l->alloc) {
            ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0,
                           "free: %p", l->alloc);
            ngx_free(l->alloc);
            l->alloc = ખાલી;

            પાછા NGX_OK;
        }
    }

    પાછા NGX_DECLINED;
}


શૂન્ય *
ngx_pcalloc(ngx_pool_t *pool, size_t size)
{
    શૂન્ય *p;

    p = ngx_palloc(pool, size);
    જો (p) {
        ngx_memzero(p, size);
    }

    પાછા p;
}


ngx_pool_cleanup_t *
ngx_pool_cleanup_add(ngx_pool_t *p, size_t size)
{
    ngx_pool_cleanup_t  *c;

    c = ngx_palloc(p, કદ(ngx_pool_cleanup_t));
    જો (c == ખાલી) {
        પાછા ખાલી;
    }

    જો (size) {
        c->data = ngx_palloc(p, size);
        જો (c->data == ખાલી) {
            પાછા ખાલી;
        }

    } નહીંતર {
        c->data = ખાલી;
    }

    c->handler = ખાલી;
    c->next = p->cleanup;

    p->cleanup = c;

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, p->log, 0, "add cleanup: %p", c);

    પાછા c;
}


શૂન્ય
ngx_pool_run_cleanup_file(ngx_pool_t *p, ngx_fd_t fd)
{
    ngx_pool_cleanup_t       *c;
    ngx_pool_cleanup_file_t  *cf;

    માટે (c = p->cleanup; c; c = c->next) {
        જો (c->handler == ngx_pool_cleanup_file) {

            cf = c->data;

            જો (cf->fd == fd) {
                c->handler(cf);
                c->handler = ખાલી;
                પાછા;
            }
        }
    }
}


શૂન્ય
ngx_pool_cleanup_file(શૂન્ય *data)
{
    ngx_pool_cleanup_file_t  *c = data;

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, c->log, 0, "file cleanup: fd:%d",
                   c->fd);

    જો (ngx_close_file(c->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, c->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", c->name);
    }
}


શૂન્ય
ngx_pool_delete_file(શૂન્ય *data)
{
    ngx_pool_cleanup_file_t  *c = data;

    ngx_err_t  err;

    ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, c->log, 0, "file cleanup: fd:%d %s",
                   c->fd, c->name);

    જો (ngx_delete_file(c->name) == NGX_FILE_ERROR) {
        err = ngx_errno;

        જો (err != NGX_ENOENT) {
            ngx_log_error(NGX_LOG_CRIT, c->log, err,
                          ngx_delete_file_n " \"%s\" failed", c->name);
        }
    }

    જો (ngx_close_file(c->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, c->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", c->name);
    }
}


#જો 0

સ્થિર શૂન્ય *
ngx_get_cached_block(size_t size)
{
    શૂન્ય                     *p;
    ngx_cached_block_slot_t  *slot;

    જો (ngx_cycle->cache == ખાલી) {
        પાછા ખાલી;
    }

    slot = &ngx_cycle->cache[(size + ngx_pagesize - 1) / ngx_pagesize];

    slot->tries++;

    જો (slot->સંખ્યા) {
        p = slot->block;
        slot->block = slot->block->next;
        slot->સંખ્યા--;
        પાછા p;
    }

    પાછા ખાલી;
}

#endif
