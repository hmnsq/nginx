
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#समावेश <ngx_config.h>
#समावेश <ngx_core.h>


अटल ngx_inline व्योम *ngx_palloc_small(ngx_pool_t *pool, माप_प्रकार आकार,
    ngx_uint_t align);
अटल व्योम *ngx_palloc_block(ngx_pool_t *pool, माप_प्रकार आकार);
अटल व्योम *ngx_palloc_large(ngx_pool_t *pool, माप_प्रकार आकार);


ngx_pool_t *
ngx_create_pool(माप_प्रकार आकार, ngx_log_t *log)
{
    ngx_pool_t  *p;

    p = ngx_memalign(NGX_POOL_ALIGNMENT, आकार, log);
    अगर (p == शून्य) {
        वापस शून्य;
    }

    p->d.last = (u_char *) p + माप(ngx_pool_t);
    p->d.end = (u_char *) p + आकार;
    p->d.अग्र = शून्य;
    p->d.failed = 0;

    आकार = आकार - माप(ngx_pool_t);
    p->max = (आकार < NGX_MAX_ALLOC_FROM_POOL) ? आकार : NGX_MAX_ALLOC_FROM_POOL;

    p->current = p;
    p->chain = शून्य;
    p->large = शून्य;
    p->cleanup = शून्य;
    p->log = log;

    वापस p;
}


व्योम
ngx_destroy_pool(ngx_pool_t *pool)
{
    ngx_pool_t          *p, *n;
    ngx_pool_large_t    *l;
    ngx_pool_cleanup_t  *c;

    क्रम (c = pool->cleanup; c; c = c->अग्र) {
        अगर (c->handler) {
            ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0,
                           "run cleanup: %p", c);
            c->handler(c->तथ्य);
        }
    }

#अगर (NGX_DEBUG)

    /*
     * we could allocate the pool->log from this pool
     * so we cannot use this log while free()ing the pool
     */

    क्रम (l = pool->large; l; l = l->अग्र) {
        ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0, "free: %p", l->alloc);
    }

    क्रम (p = pool, n = pool->d.अग्र; /* void */; p = n, n = n->d.अग्र) {
        ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, pool->log, 0,
                       "free: %p, unused: %uz", p, p->d.end - p->d.last);

        अगर (n == शून्य) {
            अवरोध;
        }
    }

#पूर्ण_अगर

    क्रम (l = pool->large; l; l = l->अग्र) {
        अगर (l->alloc) {
            ngx_free(l->alloc);
        }
    }

    क्रम (p = pool, n = pool->d.अग्र; /* void */; p = n, n = n->d.अग्र) {
        ngx_free(p);

        अगर (n == शून्य) {
            अवरोध;
        }
    }
}


व्योम
ngx_reset_pool(ngx_pool_t *pool)
{
    ngx_pool_t        *p;
    ngx_pool_large_t  *l;

    क्रम (l = pool->large; l; l = l->अग्र) {
        अगर (l->alloc) {
            ngx_free(l->alloc);
        }
    }

    क्रम (p = pool; p; p = p->d.अग्र) {
        p->d.last = (u_char *) p + माप(ngx_pool_t);
        p->d.failed = 0;
    }

    pool->current = pool;
    pool->chain = शून्य;
    pool->large = शून्य;
}


व्योम *
ngx_palloc(ngx_pool_t *pool, माप_प्रकार आकार)
{
#अगर !(NGX_DEBUG_PALLOC)
    अगर (आकार <= pool->max) {
        वापस ngx_palloc_small(pool, आकार, 1);
    }
#पूर्ण_अगर

    वापस ngx_palloc_large(pool, आकार);
}


व्योम *
ngx_pnalloc(ngx_pool_t *pool, माप_प्रकार आकार)
{
#अगर !(NGX_DEBUG_PALLOC)
    अगर (आकार <= pool->max) {
        वापस ngx_palloc_small(pool, आकार, 0);
    }
#पूर्ण_अगर

    वापस ngx_palloc_large(pool, आकार);
}


अटल ngx_inline व्योम *
ngx_palloc_small(ngx_pool_t *pool, माप_प्रकार आकार, ngx_uint_t align)
{
    u_char      *m;
    ngx_pool_t  *p;

    p = pool->current;

    करो {
        m = p->d.last;

        अगर (align) {
            m = ngx_align_ptr(m, NGX_ALIGNMENT);
        }

        अगर ((माप_प्रकार) (p->d.end - m) >= आकार) {
            p->d.last = m + आकार;

            वापस m;
        }

        p = p->d.अग्र;

    } जबतक (p);

    वापस ngx_palloc_block(pool, आकार);
}


अटल व्योम *
ngx_palloc_block(ngx_pool_t *pool, माप_प्रकार आकार)
{
    u_char      *m;
    माप_प्रकार       psize;
    ngx_pool_t  *p, *new;

    psize = (माप_प्रकार) (pool->d.end - (u_char *) pool);

    m = ngx_memalign(NGX_POOL_ALIGNMENT, psize, pool->log);
    अगर (m == शून्य) {
        वापस शून्य;
    }

    new = (ngx_pool_t *) m;

    new->d.end = m + psize;
    new->d.अग्र = शून्य;
    new->d.failed = 0;

    m += माप(ngx_pool_तथ्य_t);
    m = ngx_align_ptr(m, NGX_ALIGNMENT);
    new->d.last = m + आकार;

    क्रम (p = pool->current; p->d.अग्र; p = p->d.अग्र) {
        अगर (p->d.failed++ > 4) {
            pool->current = p->d.अग्र;
        }
    }

    p->d.अग्र = new;

    वापस m;
}


अटल व्योम *
ngx_palloc_large(ngx_pool_t *pool, माप_प्रकार आकार)
{
    व्योम              *p;
    ngx_uint_t         n;
    ngx_pool_large_t  *large;

    p = ngx_alloc(आकार, pool->log);
    अगर (p == शून्य) {
        वापस शून्य;
    }

    n = 0;

    क्रम (large = pool->large; large; large = large->अग्र) {
        अगर (large->alloc == शून्य) {
            large->alloc = p;
            वापस p;
        }

        अगर (n++ > 3) {
            अवरोध;
        }
    }

    large = ngx_palloc_small(pool, माप(ngx_pool_large_t), 1);
    अगर (large == शून्य) {
        ngx_free(p);
        वापस शून्य;
    }

    large->alloc = p;
    large->अग्र = pool->large;
    pool->large = large;

    वापस p;
}


व्योम *
ngx_pmemalign(ngx_pool_t *pool, माप_प्रकार आकार, माप_प्रकार alignment)
{
    व्योम              *p;
    ngx_pool_large_t  *large;

    p = ngx_memalign(alignment, आकार, pool->log);
    अगर (p == शून्य) {
        वापस शून्य;
    }

    large = ngx_palloc_small(pool, माप(ngx_pool_large_t), 1);
    अगर (large == शून्य) {
        ngx_free(p);
        वापस शून्य;
    }

    large->alloc = p;
    large->अग्र = pool->large;
    pool->large = large;

    वापस p;
}


ngx_int_t
ngx_pfree(ngx_pool_t *pool, व्योम *p)
{
    ngx_pool_large_t  *l;

    क्रम (l = pool->large; l; l = l->अग्र) {
        अगर (p == l->alloc) {
            ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0,
                           "free: %p", l->alloc);
            ngx_free(l->alloc);
            l->alloc = शून्य;

            वापस NGX_OK;
        }
    }

    वापस NGX_DECLINED;
}


व्योम *
ngx_pcalloc(ngx_pool_t *pool, माप_प्रकार आकार)
{
    व्योम *p;

    p = ngx_palloc(pool, आकार);
    अगर (p) {
        ngx_memzero(p, आकार);
    }

    वापस p;
}


ngx_pool_cleanup_t *
ngx_pool_cleanup_add(ngx_pool_t *p, माप_प्रकार आकार)
{
    ngx_pool_cleanup_t  *c;

    c = ngx_palloc(p, माप(ngx_pool_cleanup_t));
    अगर (c == शून्य) {
        वापस शून्य;
    }

    अगर (आकार) {
        c->तथ्य = ngx_palloc(p, आकार);
        अगर (c->तथ्य == शून्य) {
            वापस शून्य;
        }

    } अन्यथा {
        c->तथ्य = शून्य;
    }

    c->handler = शून्य;
    c->अग्र = p->cleanup;

    p->cleanup = c;

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, p->log, 0, "add cleanup: %p", c);

    वापस c;
}


व्योम
ngx_pool_run_cleanup_file(ngx_pool_t *p, ngx_fd_t fd)
{
    ngx_pool_cleanup_t       *c;
    ngx_pool_cleanup_file_t  *cf;

    क्रम (c = p->cleanup; c; c = c->अग्र) {
        अगर (c->handler == ngx_pool_cleanup_file) {

            cf = c->तथ्य;

            अगर (cf->fd == fd) {
                c->handler(cf);
                c->handler = शून्य;
                वापस;
            }
        }
    }
}


व्योम
ngx_pool_cleanup_file(व्योम *तथ्य)
{
    ngx_pool_cleanup_file_t  *c = तथ्य;

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, c->log, 0, "file cleanup: fd:%d",
                   c->fd);

    अगर (ngx_close_file(c->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, c->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", c->name);
    }
}


व्योम
ngx_pool_delete_file(व्योम *तथ्य)
{
    ngx_pool_cleanup_file_t  *c = तथ्य;

    ngx_err_t  err;

    ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, c->log, 0, "file cleanup: fd:%d %s",
                   c->fd, c->name);

    अगर (ngx_delete_file(c->name) == NGX_FILE_ERROR) {
        err = ngx_errno;

        अगर (err != NGX_ENOENT) {
            ngx_log_error(NGX_LOG_CRIT, c->log, err,
                          ngx_delete_file_n " \"%s\" failed", c->name);
        }
    }

    अगर (ngx_close_file(c->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, c->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", c->name);
    }
}


#अगर 0

अटल व्योम *
ngx_get_cached_block(माप_प्रकार आकार)
{
    व्योम                     *p;
    ngx_cached_block_slot_t  *slot;

    अगर (ngx_cycle->cache == शून्य) {
        वापस शून्य;
    }

    slot = &ngx_cycle->cache[(आकार + ngx_pagesize - 1) / ngx_pagesize];

    slot->tries++;

    अगर (slot->संख्या) {
        p = slot->block;
        slot->block = slot->block->अग्र;
        slot->संख्या--;
        वापस p;
    }

    वापस शून्य;
}

#पूर्ण_अगर
