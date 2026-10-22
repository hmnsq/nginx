
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
    जर (p == रिक्त) {
        परत रिक्त;
    }

    p->d.last = (u_char *) p + आकार(ngx_pool_t);
    p->d.end = (u_char *) p + size;
    p->d.next = रिक्त;
    p->d.failed = 0;

    size = size - आकार(ngx_pool_t);
    p->max = (size < NGX_MAX_ALLOC_FROM_POOL) ? size : NGX_MAX_ALLOC_FROM_POOL;

    p->current = p;
    p->chain = रिक्त;
    p->large = रिक्त;
    p->cleanup = रिक्त;
    p->log = log;

    परत p;
}


शून्य
ngx_destroy_pool(ngx_pool_t *pool)
{
    ngx_pool_t          *p, *n;
    ngx_pool_large_t    *l;
    ngx_pool_cleanup_t  *c;

    साठी (c = pool->cleanup; c; c = c->next) {
        जर (c->handler) {
            ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0,
                           "run cleanup: %p", c);
            c->handler(c->data);
        }
    }

#जर (NGX_DEBUG)

    /*
     * we could allocate the pool->log from this pool
     * so we cannot use this log while free()ing the pool
     */

    साठी (l = pool->large; l; l = l->next) {
        ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0, "free: %p", l->alloc);
    }

    साठी (p = pool, n = pool->d.next; /* void */; p = n, n = n->d.next) {
        ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, pool->log, 0,
                       "free: %p, unused: %uz", p, p->d.end - p->d.last);

        जर (n == रिक्त) {
            थांबा;
        }
    }

#endif

    साठी (l = pool->large; l; l = l->next) {
        जर (l->alloc) {
            ngx_free(l->alloc);
        }
    }

    साठी (p = pool, n = pool->d.next; /* void */; p = n, n = n->d.next) {
        ngx_free(p);

        जर (n == रिक्त) {
            थांबा;
        }
    }
}


शून्य
ngx_reset_pool(ngx_pool_t *pool)
{
    ngx_pool_t        *p;
    ngx_pool_large_t  *l;

    साठी (l = pool->large; l; l = l->next) {
        जर (l->alloc) {
            ngx_free(l->alloc);
        }
    }

    साठी (p = pool; p; p = p->d.next) {
        p->d.last = (u_char *) p + आकार(ngx_pool_t);
        p->d.failed = 0;
    }

    pool->current = pool;
    pool->chain = रिक्त;
    pool->large = रिक्त;
}


शून्य *
ngx_palloc(ngx_pool_t *pool, माप_प्रकार size)
{
#जर !(NGX_DEBUG_PALLOC)
    जर (size <= pool->max) {
        परत ngx_palloc_small(pool, size, 1);
    }
#endif

    परत ngx_palloc_large(pool, size);
}


शून्य *
ngx_pnalloc(ngx_pool_t *pool, माप_प्रकार size)
{
#जर !(NGX_DEBUG_PALLOC)
    जर (size <= pool->max) {
        परत ngx_palloc_small(pool, size, 0);
    }
#endif

    परत ngx_palloc_large(pool, size);
}


स्थिर ngx_inline शून्य *
ngx_palloc_small(ngx_pool_t *pool, माप_प्रकार size, ngx_uint_t align)
{
    u_char      *m;
    ngx_pool_t  *p;

    p = pool->current;

    करा {
        m = p->d.last;

        जर (align) {
            m = ngx_align_ptr(m, NGX_ALIGNMENT);
        }

        जर ((माप_प्रकार) (p->d.end - m) >= size) {
            p->d.last = m + size;

            परत m;
        }

        p = p->d.next;

    } पर्यंत (p);

    परत ngx_palloc_block(pool, size);
}


स्थिर शून्य *
ngx_palloc_block(ngx_pool_t *pool, माप_प्रकार size)
{
    u_char      *m;
    माप_प्रकार       psize;
    ngx_pool_t  *p, *new;

    psize = (माप_प्रकार) (pool->d.end - (u_char *) pool);

    m = ngx_memalign(NGX_POOL_ALIGNMENT, psize, pool->log);
    जर (m == रिक्त) {
        परत रिक्त;
    }

    new = (ngx_pool_t *) m;

    new->d.end = m + psize;
    new->d.next = रिक्त;
    new->d.failed = 0;

    m += आकार(ngx_pool_data_t);
    m = ngx_align_ptr(m, NGX_ALIGNMENT);
    new->d.last = m + size;

    साठी (p = pool->current; p->d.next; p = p->d.next) {
        जर (p->d.failed++ > 4) {
            pool->current = p->d.next;
        }
    }

    p->d.next = new;

    परत m;
}


स्थिर शून्य *
ngx_palloc_large(ngx_pool_t *pool, माप_प्रकार size)
{
    शून्य              *p;
    ngx_uint_t         n;
    ngx_pool_large_t  *large;

    p = ngx_alloc(size, pool->log);
    जर (p == रिक्त) {
        परत रिक्त;
    }

    n = 0;

    साठी (large = pool->large; large; large = large->next) {
        जर (large->alloc == रिक्त) {
            large->alloc = p;
            परत p;
        }

        जर (n++ > 3) {
            थांबा;
        }
    }

    large = ngx_palloc_small(pool, आकार(ngx_pool_large_t), 1);
    जर (large == रिक्त) {
        ngx_free(p);
        परत रिक्त;
    }

    large->alloc = p;
    large->next = pool->large;
    pool->large = large;

    परत p;
}


शून्य *
ngx_pmemalign(ngx_pool_t *pool, माप_प्रकार size, माप_प्रकार alignment)
{
    शून्य              *p;
    ngx_pool_large_t  *large;

    p = ngx_memalign(alignment, size, pool->log);
    जर (p == रिक्त) {
        परत रिक्त;
    }

    large = ngx_palloc_small(pool, आकार(ngx_pool_large_t), 1);
    जर (large == रिक्त) {
        ngx_free(p);
        परत रिक्त;
    }

    large->alloc = p;
    large->next = pool->large;
    pool->large = large;

    परत p;
}


ngx_int_t
ngx_pfree(ngx_pool_t *pool, शून्य *p)
{
    ngx_pool_large_t  *l;

    साठी (l = pool->large; l; l = l->next) {
        जर (p == l->alloc) {
            ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0,
                           "free: %p", l->alloc);
            ngx_free(l->alloc);
            l->alloc = रिक्त;

            परत NGX_OK;
        }
    }

    परत NGX_DECLINED;
}


शून्य *
ngx_pcalloc(ngx_pool_t *pool, माप_प्रकार size)
{
    शून्य *p;

    p = ngx_palloc(pool, size);
    जर (p) {
        ngx_memzero(p, size);
    }

    परत p;
}


ngx_pool_cleanup_t *
ngx_pool_cleanup_add(ngx_pool_t *p, माप_प्रकार size)
{
    ngx_pool_cleanup_t  *c;

    c = ngx_palloc(p, आकार(ngx_pool_cleanup_t));
    जर (c == रिक्त) {
        परत रिक्त;
    }

    जर (size) {
        c->data = ngx_palloc(p, size);
        जर (c->data == रिक्त) {
            परत रिक्त;
        }

    } अन्यथा {
        c->data = रिक्त;
    }

    c->handler = रिक्त;
    c->next = p->cleanup;

    p->cleanup = c;

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, p->log, 0, "add cleanup: %p", c);

    परत c;
}


शून्य
ngx_pool_run_cleanup_file(ngx_pool_t *p, ngx_fd_t fd)
{
    ngx_pool_cleanup_t       *c;
    ngx_pool_cleanup_file_t  *cf;

    साठी (c = p->cleanup; c; c = c->next) {
        जर (c->handler == ngx_pool_cleanup_file) {

            cf = c->data;

            जर (cf->fd == fd) {
                c->handler(cf);
                c->handler = रिक्त;
                परत;
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

    जर (ngx_close_file(c->fd) == NGX_FILE_ERROR) {
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

    जर (ngx_delete_file(c->name) == NGX_FILE_ERROR) {
        err = ngx_errno;

        जर (err != NGX_ENOENT) {
            ngx_log_error(NGX_LOG_CRIT, c->log, err,
                          ngx_delete_file_n " \"%s\" failed", c->name);
        }
    }

    जर (ngx_close_file(c->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, c->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", c->name);
    }
}


#जर 0

स्थिर शून्य *
ngx_get_cached_block(माप_प्रकार size)
{
    शून्य                     *p;
    ngx_cached_block_slot_t  *slot;

    जर (ngx_cycle->cache == रिक्त) {
        परत रिक्त;
    }

    slot = &ngx_cycle->cache[(size + ngx_pagesize - 1) / ngx_pagesize];

    slot->tries++;

    जर (slot->संख्या) {
        p = slot->block;
        slot->block = slot->block->next;
        slot->संख्या--;
        परत p;
    }

    परत रिक्त;
}

#endif
