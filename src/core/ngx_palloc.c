
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#অন্তর্ভাব <ngx_config.h>
#অন্তর্ভাব <ngx_core.h>


স্থির ngx_inline শূন্য *ngx_palloc_small(ngx_pool_t *pool, আকার_t আকার_2,
    ngx_uint_t align);
স্থির শূন্য *ngx_palloc_block(ngx_pool_t *pool, আকার_t আকার_2);
স্থির শূন্য *ngx_palloc_large(ngx_pool_t *pool, আকার_t আকার_2);


ngx_pool_t *
ngx_create_pool(আকার_t আকার_2, ngx_log_t *log)
{
    ngx_pool_t  *p;

    p = ngx_memalign(NGX_POOL_ALIGNMENT, আকার_2, log);
    যদি (p == ফাঁকা) {
        ফেরত ফাঁকা;
    }

    p->d.last = (u_char *) p + আকার(ngx_pool_t);
    p->d.end = (u_char *) p + আকার_2;
    p->d.পরবর্তী = ফাঁকা;
    p->d.failed = 0;

    আকার_2 = আকার_2 - আকার(ngx_pool_t);
    p->max = (আকার_2 < NGX_MAX_ALLOC_FROM_POOL) ? আকার_2 : NGX_MAX_ALLOC_FROM_POOL;

    p->current = p;
    p->chain = ফাঁকা;
    p->large = ফাঁকা;
    p->cleanup = ফাঁকা;
    p->log = log;

    ফেরত p;
}


শূন্য
ngx_destroy_pool(ngx_pool_t *pool)
{
    ngx_pool_t          *p, *n;
    ngx_pool_large_t    *l;
    ngx_pool_cleanup_t  *c;

    জন্য (c = pool->cleanup; c; c = c->পরবর্তী) {
        যদি (c->handler) {
            ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0,
                           "run cleanup: %p", c);
            c->handler(c->তথ্য);
        }
    }

#যদি (NGX_DEBUG)

    /*
     * we could allocate the pool->log from this pool
     * so we cannot use this log while free()ing the pool
     */

    জন্য (l = pool->large; l; l = l->পরবর্তী) {
        ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0, "free: %p", l->alloc);
    }

    জন্য (p = pool, n = pool->d.পরবর্তী; /* void */; p = n, n = n->d.পরবর্তী) {
        ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, pool->log, 0,
                       "free: %p, unused: %uz", p, p->d.end - p->d.last);

        যদি (n == ফাঁকা) {
            বিরতি;
        }
    }

#endif

    জন্য (l = pool->large; l; l = l->পরবর্তী) {
        যদি (l->alloc) {
            ngx_free(l->alloc);
        }
    }

    জন্য (p = pool, n = pool->d.পরবর্তী; /* void */; p = n, n = n->d.পরবর্তী) {
        ngx_free(p);

        যদি (n == ফাঁকা) {
            বিরতি;
        }
    }
}


শূন্য
ngx_reset_pool(ngx_pool_t *pool)
{
    ngx_pool_t        *p;
    ngx_pool_large_t  *l;

    জন্য (l = pool->large; l; l = l->পরবর্তী) {
        যদি (l->alloc) {
            ngx_free(l->alloc);
        }
    }

    জন্য (p = pool; p; p = p->d.পরবর্তী) {
        p->d.last = (u_char *) p + আকার(ngx_pool_t);
        p->d.failed = 0;
    }

    pool->current = pool;
    pool->chain = ফাঁকা;
    pool->large = ফাঁকা;
}


শূন্য *
ngx_palloc(ngx_pool_t *pool, আকার_t আকার_2)
{
#যদি !(NGX_DEBUG_PALLOC)
    যদি (আকার_2 <= pool->max) {
        ফেরত ngx_palloc_small(pool, আকার_2, 1);
    }
#endif

    ফেরত ngx_palloc_large(pool, আকার_2);
}


শূন্য *
ngx_pnalloc(ngx_pool_t *pool, আকার_t আকার_2)
{
#যদি !(NGX_DEBUG_PALLOC)
    যদি (আকার_2 <= pool->max) {
        ফেরত ngx_palloc_small(pool, আকার_2, 0);
    }
#endif

    ফেরত ngx_palloc_large(pool, আকার_2);
}


স্থির ngx_inline শূন্য *
ngx_palloc_small(ngx_pool_t *pool, আকার_t আকার_2, ngx_uint_t align)
{
    u_char      *m;
    ngx_pool_t  *p;

    p = pool->current;

    করো {
        m = p->d.last;

        যদি (align) {
            m = ngx_align_ptr(m, NGX_ALIGNMENT);
        }

        যদি ((আকার_t) (p->d.end - m) >= আকার_2) {
            p->d.last = m + আকার_2;

            ফেরত m;
        }

        p = p->d.পরবর্তী;

    } যখন (p);

    ফেরত ngx_palloc_block(pool, আকার_2);
}


স্থির শূন্য *
ngx_palloc_block(ngx_pool_t *pool, আকার_t আকার_2)
{
    u_char      *m;
    আকার_t       psize;
    ngx_pool_t  *p, *new;

    psize = (আকার_t) (pool->d.end - (u_char *) pool);

    m = ngx_memalign(NGX_POOL_ALIGNMENT, psize, pool->log);
    যদি (m == ফাঁকা) {
        ফেরত ফাঁকা;
    }

    new = (ngx_pool_t *) m;

    new->d.end = m + psize;
    new->d.পরবর্তী = ফাঁকা;
    new->d.failed = 0;

    m += আকার(ngx_pool_তথ্য_t);
    m = ngx_align_ptr(m, NGX_ALIGNMENT);
    new->d.last = m + আকার_2;

    জন্য (p = pool->current; p->d.পরবর্তী; p = p->d.পরবর্তী) {
        যদি (p->d.failed++ > 4) {
            pool->current = p->d.পরবর্তী;
        }
    }

    p->d.পরবর্তী = new;

    ফেরত m;
}


স্থির শূন্য *
ngx_palloc_large(ngx_pool_t *pool, আকার_t আকার_2)
{
    শূন্য              *p;
    ngx_uint_t         n;
    ngx_pool_large_t  *large;

    p = ngx_alloc(আকার_2, pool->log);
    যদি (p == ফাঁকা) {
        ফেরত ফাঁকা;
    }

    n = 0;

    জন্য (large = pool->large; large; large = large->পরবর্তী) {
        যদি (large->alloc == ফাঁকা) {
            large->alloc = p;
            ফেরত p;
        }

        যদি (n++ > 3) {
            বিরতি;
        }
    }

    large = ngx_palloc_small(pool, আকার(ngx_pool_large_t), 1);
    যদি (large == ফাঁকা) {
        ngx_free(p);
        ফেরত ফাঁকা;
    }

    large->alloc = p;
    large->পরবর্তী = pool->large;
    pool->large = large;

    ফেরত p;
}


শূন্য *
ngx_pmemalign(ngx_pool_t *pool, আকার_t আকার_2, আকার_t alignment)
{
    শূন্য              *p;
    ngx_pool_large_t  *large;

    p = ngx_memalign(alignment, আকার_2, pool->log);
    যদি (p == ফাঁকা) {
        ফেরত ফাঁকা;
    }

    large = ngx_palloc_small(pool, আকার(ngx_pool_large_t), 1);
    যদি (large == ফাঁকা) {
        ngx_free(p);
        ফেরত ফাঁকা;
    }

    large->alloc = p;
    large->পরবর্তী = pool->large;
    pool->large = large;

    ফেরত p;
}


ngx_int_t
ngx_pfree(ngx_pool_t *pool, শূন্য *p)
{
    ngx_pool_large_t  *l;

    জন্য (l = pool->large; l; l = l->পরবর্তী) {
        যদি (p == l->alloc) {
            ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0,
                           "free: %p", l->alloc);
            ngx_free(l->alloc);
            l->alloc = ফাঁকা;

            ফেরত NGX_OK;
        }
    }

    ফেরত NGX_DECLINED;
}


শূন্য *
ngx_pcalloc(ngx_pool_t *pool, আকার_t আকার_2)
{
    শূন্য *p;

    p = ngx_palloc(pool, আকার_2);
    যদি (p) {
        ngx_memzero(p, আকার_2);
    }

    ফেরত p;
}


ngx_pool_cleanup_t *
ngx_pool_cleanup_add(ngx_pool_t *p, আকার_t আকার_2)
{
    ngx_pool_cleanup_t  *c;

    c = ngx_palloc(p, আকার(ngx_pool_cleanup_t));
    যদি (c == ফাঁকা) {
        ফেরত ফাঁকা;
    }

    যদি (আকার_2) {
        c->তথ্য = ngx_palloc(p, আকার_2);
        যদি (c->তথ্য == ফাঁকা) {
            ফেরত ফাঁকা;
        }

    } অন্যথা {
        c->তথ্য = ফাঁকা;
    }

    c->handler = ফাঁকা;
    c->পরবর্তী = p->cleanup;

    p->cleanup = c;

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, p->log, 0, "add cleanup: %p", c);

    ফেরত c;
}


শূন্য
ngx_pool_run_cleanup_file(ngx_pool_t *p, ngx_fd_t fd)
{
    ngx_pool_cleanup_t       *c;
    ngx_pool_cleanup_file_t  *cf;

    জন্য (c = p->cleanup; c; c = c->পরবর্তী) {
        যদি (c->handler == ngx_pool_cleanup_file) {

            cf = c->তথ্য;

            যদি (cf->fd == fd) {
                c->handler(cf);
                c->handler = ফাঁকা;
                ফেরত;
            }
        }
    }
}


শূন্য
ngx_pool_cleanup_file(শূন্য *তথ্য)
{
    ngx_pool_cleanup_file_t  *c = তথ্য;

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, c->log, 0, "file cleanup: fd:%d",
                   c->fd);

    যদি (ngx_close_file(c->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, c->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", c->name);
    }
}


শূন্য
ngx_pool_delete_file(শূন্য *তথ্য)
{
    ngx_pool_cleanup_file_t  *c = তথ্য;

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
ngx_get_cached_block(আকার_t আকার_2)
{
    শূন্য                     *p;
    ngx_cached_block_slot_t  *slot;

    যদি (ngx_cycle->cache == ফাঁকা) {
        ফেরত ফাঁকা;
    }

    slot = &ngx_cycle->cache[(আকার_2 + ngx_pagesize - 1) / ngx_pagesize];

    slot->tries++;

    যদি (slot->সংখ্যা) {
        p = slot->block;
        slot->block = slot->block->পরবর্তী;
        slot->সংখ্যা--;
        ফেরত p;
    }

    ফেরত ফাঁকা;
}

#endif
