
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


स्थिर ngx_inline शून्य *ngx_palloc_small(ngx_pool_t *pool, माप_प्रकार size,
    ngx_uint_t align);
स्थिर शून्य *ngx_palloc_block(ngx_pool_t *pool, माप_प्रकार size);
स्थिर शून्य *ngx_palloc_large(ngx_pool_t *pool, माप_प्रकार size);


ngx_pool_t *
ngx_create_pool(माप_प्रकार size, ngx_log_t *log)
{
    ngx_pool_t  *p;

    p = ngx_memalign(NGX_POOL_ALIGNMENT, size, log);
    यदि (p == रिक्त) {
        फर्काऊ रिक्त;
    }

    p->d.last = (u_char *) p + परिमाण(ngx_pool_t);
    p->d.end = (u_char *) p + size;
    p->d.next = रिक्त;
    p->d.failed = 0;

    size = size - परिमाण(ngx_pool_t);
    p->max = (size < NGX_MAX_ALLOC_FROM_POOL) ? size : NGX_MAX_ALLOC_FROM_POOL;

    p->current = p;
    p->chain = रिक्त;
    p->large = रिक्त;
    p->cleanup = रिक्त;
    p->log = log;

    फर्काऊ p;
}


शून्य
ngx_destroy_pool(ngx_pool_t *pool)
{
    ngx_pool_t          *p, *n;
    ngx_pool_large_t    *l;
    ngx_pool_cleanup_t  *c;

    लागि (c = pool->cleanup; c; c = c->next) {
        यदि (c->handler) {
            ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0,
                           "run cleanup: %p", c);
            c->handler(c->data);
        }
    }

#यदि (NGX_DEBUG)

    /*
     * we could allocate the pool->log from this pool
     * so we cannot use this log while free()ing the pool
     */

    लागि (l = pool->large; l; l = l->next) {
        ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0, "free: %p", l->alloc);
    }

    लागि (p = pool, n = pool->d.next; /* void */; p = n, n = n->d.next) {
        ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, pool->log, 0,
                       "free: %p, unused: %uz", p, p->d.end - p->d.last);

        यदि (n == रिक्त) {
            रोक;
        }
    }

#endif

    लागि (l = pool->large; l; l = l->next) {
        यदि (l->alloc) {
            ngx_free(l->alloc);
        }
    }

    लागि (p = pool, n = pool->d.next; /* void */; p = n, n = n->d.next) {
        ngx_free(p);

        यदि (n == रिक्त) {
            रोक;
        }
    }
}


शून्य
ngx_reset_pool(ngx_pool_t *pool)
{
    ngx_pool_t        *p;
    ngx_pool_large_t  *l;

    लागि (l = pool->large; l; l = l->next) {
        यदि (l->alloc) {
            ngx_free(l->alloc);
        }
    }

    लागि (p = pool; p; p = p->d.next) {
        p->d.last = (u_char *) p + परिमाण(ngx_pool_t);
        p->d.failed = 0;
    }

    pool->current = pool;
    pool->chain = रिक्त;
    pool->large = रिक्त;
}


शून्य *
ngx_palloc(ngx_pool_t *pool, माप_प्रकार size)
{
#यदि !(NGX_DEBUG_PALLOC)
    यदि (size <= pool->max) {
        फर्काऊ ngx_palloc_small(pool, size, 1);
    }
#endif

    फर्काऊ ngx_palloc_large(pool, size);
}


शून्य *
ngx_pnalloc(ngx_pool_t *pool, माप_प्रकार size)
{
#यदि !(NGX_DEBUG_PALLOC)
    यदि (size <= pool->max) {
        फर्काऊ ngx_palloc_small(pool, size, 0);
    }
#endif

    फर्काऊ ngx_palloc_large(pool, size);
}


स्थिर ngx_inline शून्य *
ngx_palloc_small(ngx_pool_t *pool, माप_प्रकार size, ngx_uint_t align)
{
    u_char      *m;
    ngx_pool_t  *p;

    p = pool->current;

    गर {
        m = p->d.last;

        यदि (align) {
            m = ngx_align_ptr(m, NGX_ALIGNMENT);
        }

        यदि ((माप_प्रकार) (p->d.end - m) >= size) {
            p->d.last = m + size;

            फर्काऊ m;
        }

        p = p->d.next;

    } जबसम्म (p);

    फर्काऊ ngx_palloc_block(pool, size);
}


स्थिर शून्य *
ngx_palloc_block(ngx_pool_t *pool, माप_प्रकार size)
{
    u_char      *m;
    माप_प्रकार       psize;
    ngx_pool_t  *p, *new;

    psize = (माप_प्रकार) (pool->d.end - (u_char *) pool);

    m = ngx_memalign(NGX_POOL_ALIGNMENT, psize, pool->log);
    यदि (m == रिक्त) {
        फर्काऊ रिक्त;
    }

    new = (ngx_pool_t *) m;

    new->d.end = m + psize;
    new->d.next = रिक्त;
    new->d.failed = 0;

    m += परिमाण(ngx_pool_data_t);
    m = ngx_align_ptr(m, NGX_ALIGNMENT);
    new->d.last = m + size;

    लागि (p = pool->current; p->d.next; p = p->d.next) {
        यदि (p->d.failed++ > 4) {
            pool->current = p->d.next;
        }
    }

    p->d.next = new;

    फर्काऊ m;
}


स्थिर शून्य *
ngx_palloc_large(ngx_pool_t *pool, माप_प्रकार size)
{
    शून्य              *p;
    ngx_uint_t         n;
    ngx_pool_large_t  *large;

    p = ngx_alloc(size, pool->log);
    यदि (p == रिक्त) {
        फर्काऊ रिक्त;
    }

    n = 0;

    लागि (large = pool->large; large; large = large->next) {
        यदि (large->alloc == रिक्त) {
            large->alloc = p;
            फर्काऊ p;
        }

        यदि (n++ > 3) {
            रोक;
        }
    }

    large = ngx_palloc_small(pool, परिमाण(ngx_pool_large_t), 1);
    यदि (large == रिक्त) {
        ngx_free(p);
        फर्काऊ रिक्त;
    }

    large->alloc = p;
    large->next = pool->large;
    pool->large = large;

    फर्काऊ p;
}


शून्य *
ngx_pmemalign(ngx_pool_t *pool, माप_प्रकार size, माप_प्रकार alignment)
{
    शून्य              *p;
    ngx_pool_large_t  *large;

    p = ngx_memalign(alignment, size, pool->log);
    यदि (p == रिक्त) {
        फर्काऊ रिक्त;
    }

    large = ngx_palloc_small(pool, परिमाण(ngx_pool_large_t), 1);
    यदि (large == रिक्त) {
        ngx_free(p);
        फर्काऊ रिक्त;
    }

    large->alloc = p;
    large->next = pool->large;
    pool->large = large;

    फर्काऊ p;
}


ngx_int_t
ngx_pfree(ngx_pool_t *pool, शून्य *p)
{
    ngx_pool_large_t  *l;

    लागि (l = pool->large; l; l = l->next) {
        यदि (p == l->alloc) {
            ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0,
                           "free: %p", l->alloc);
            ngx_free(l->alloc);
            l->alloc = रिक्त;

            फर्काऊ NGX_OK;
        }
    }

    फर्काऊ NGX_DECLINED;
}


शून्य *
ngx_pcalloc(ngx_pool_t *pool, माप_प्रकार size)
{
    शून्य *p;

    p = ngx_palloc(pool, size);
    यदि (p) {
        ngx_memzero(p, size);
    }

    फर्काऊ p;
}


ngx_pool_cleanup_t *
ngx_pool_cleanup_add(ngx_pool_t *p, माप_प्रकार size)
{
    ngx_pool_cleanup_t  *c;

    c = ngx_palloc(p, परिमाण(ngx_pool_cleanup_t));
    यदि (c == रिक्त) {
        फर्काऊ रिक्त;
    }

    यदि (size) {
        c->data = ngx_palloc(p, size);
        यदि (c->data == रिक्त) {
            फर्काऊ रिक्त;
        }

    } अन्यथा {
        c->data = रिक्त;
    }

    c->handler = रिक्त;
    c->next = p->cleanup;

    p->cleanup = c;

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, p->log, 0, "add cleanup: %p", c);

    फर्काऊ c;
}


शून्य
ngx_pool_run_cleanup_file(ngx_pool_t *p, ngx_fd_t fd)
{
    ngx_pool_cleanup_t       *c;
    ngx_pool_cleanup_file_t  *cf;

    लागि (c = p->cleanup; c; c = c->next) {
        यदि (c->handler == ngx_pool_cleanup_file) {

            cf = c->data;

            यदि (cf->fd == fd) {
                c->handler(cf);
                c->handler = रिक्त;
                फर्काऊ;
            }
        }
    }
}


शून्य
ngx_pool_cleanup_file(शून्य *data)
{
    ngx_pool_cleanup_file_t  *c = data;

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, c->log, 0, "file cleanup: fd:%d",
                   c->fd);

    यदि (ngx_close_file(c->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, c->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", c->name);
    }
}


शून्य
ngx_pool_delete_file(शून्य *data)
{
    ngx_pool_cleanup_file_t  *c = data;

    ngx_err_t  err;

    ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, c->log, 0, "file cleanup: fd:%d %s",
                   c->fd, c->name);

    यदि (ngx_delete_file(c->name) == NGX_FILE_ERROR) {
        err = ngx_errno;

        यदि (err != NGX_ENOENT) {
            ngx_log_error(NGX_LOG_CRIT, c->log, err,
                          ngx_delete_file_n " \"%s\" failed", c->name);
        }
    }

    यदि (ngx_close_file(c->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, c->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", c->name);
    }
}


#यदि 0

स्थिर शून्य *
ngx_get_cached_block(माप_प्रकार size)
{
    शून्य                     *p;
    ngx_cached_block_slot_t  *slot;

    यदि (ngx_cycle->cache == रिक्त) {
        फर्काऊ रिक्त;
    }

    slot = &ngx_cycle->cache[(size + ngx_pagesize - 1) / ngx_pagesize];

    slot->tries++;

    यदि (slot->सङ्ख्या) {
        p = slot->block;
        slot->block = slot->block->next;
        slot->सङ्ख्या--;
        फर्काऊ p;
    }

    फर्काऊ रिक्त;
}

#endif
