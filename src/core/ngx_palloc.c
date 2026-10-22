
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


स्थिर ngx_inline शून्य *ngx_palloc_small(ngx_pool_t *pool, आकार_t आकार,
    ngx_uint_t align);
स्थिर शून्य *ngx_palloc_खण्डम्(ngx_pool_t *pool, आकार_t आकार);
स्थिर शून्य *ngx_palloc_large(ngx_pool_t *pool, आकार_t आकार);


ngx_pool_t *
ngx_create_pool(आकार_t आकार, ngx_log_t *लघुगणकः)
{
    ngx_pool_t  *p;

    p = ngx_memalign(NGX_POOL_ALIGNMENT, आकार, लघुगणकः);
    यदि (p == रिक्त) {
        प्रत्यागम रिक्त;
    }

    p->d.चरमः = (u_char *) p + परिमाण(ngx_pool_t);
    p->d.अन्तः = (u_char *) p + आकार;
    p->d.अग्र = रिक्त;
    p->d.failed = 0;

    आकार = आकार - परिमाण(ngx_pool_t);
    p->max = (आकार < NGX_MAX_ALLOC_FROM_POOL) ? आकार : NGX_MAX_ALLOC_FROM_POOL;

    p->current = p;
    p->chain = रिक्त;
    p->large = रिक्त;
    p->cleanup = रिक्त;
    p->लघुगणकः = लघुगणकः;

    प्रत्यागम p;
}


शून्य
ngx_destroy_pool(ngx_pool_t *pool)
{
    ngx_pool_t          *p, *n;
    ngx_pool_large_t    *l;
    ngx_pool_cleanup_t  *c;

    प्रति (c = pool->cleanup; c; c = c->अग्र) {
        यदि (c->handler) {
            ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->लघुगणकः, 0,
                           "run cleanup: %p", c);
            c->handler(c->दत्त);
        }
    }

#यदि (NGX_DEBUG)

    /*
     * we could allocate the pool->log from this pool
     * so we cannot use this log while free()ing the pool
     */

    प्रति (l = pool->large; l; l = l->अग्र) {
        ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->लघुगणकः, 0, "free: %p", l->alloc);
    }

    प्रति (p = pool, n = pool->d.अग्र; /* void */; p = n, n = n->d.अग्र) {
        ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, pool->लघुगणकः, 0,
                       "free: %p, unused: %uz", p, p->d.अन्तः - p->d.चरमः);

        यदि (n == रिक्त) {
            भङ्ग;
        }
    }

#यदि_अन्तः

    प्रति (l = pool->large; l; l = l->अग्र) {
        यदि (l->alloc) {
            ngx_free(l->alloc);
        }
    }

    प्रति (p = pool, n = pool->d.अग्र; /* void */; p = n, n = n->d.अग्र) {
        ngx_free(p);

        यदि (n == रिक्त) {
            भङ्ग;
        }
    }
}


शून्य
ngx_reset_pool(ngx_pool_t *pool)
{
    ngx_pool_t        *p;
    ngx_pool_large_t  *l;

    प्रति (l = pool->large; l; l = l->अग्र) {
        यदि (l->alloc) {
            ngx_free(l->alloc);
        }
    }

    प्रति (p = pool; p; p = p->d.अग्र) {
        p->d.चरमः = (u_char *) p + परिमाण(ngx_pool_t);
        p->d.failed = 0;
    }

    pool->current = pool;
    pool->chain = रिक्त;
    pool->large = रिक्त;
}


शून्य *
ngx_palloc(ngx_pool_t *pool, आकार_t आकार)
{
#यदि !(NGX_DEBUG_PALLOC)
    यदि (आकार <= pool->max) {
        प्रत्यागम ngx_palloc_small(pool, आकार, 1);
    }
#यदि_अन्तः

    प्रत्यागम ngx_palloc_large(pool, आकार);
}


शून्य *
ngx_pnalloc(ngx_pool_t *pool, आकार_t आकार)
{
#यदि !(NGX_DEBUG_PALLOC)
    यदि (आकार <= pool->max) {
        प्रत्यागम ngx_palloc_small(pool, आकार, 0);
    }
#यदि_अन्तः

    प्रत्यागम ngx_palloc_large(pool, आकार);
}


स्थिर ngx_inline शून्य *
ngx_palloc_small(ngx_pool_t *pool, आकार_t आकार, ngx_uint_t align)
{
    u_char      *m;
    ngx_pool_t  *p;

    p = pool->current;

    कुरु {
        m = p->d.चरमः;

        यदि (align) {
            m = ngx_align_ptr(m, NGX_ALIGNMENT);
        }

        यदि ((आकार_t) (p->d.अन्तः - m) >= आकार) {
            p->d.चरमः = m + आकार;

            प्रत्यागम m;
        }

        p = p->d.अग्र;

    } यावत् (p);

    प्रत्यागम ngx_palloc_खण्डम्(pool, आकार);
}


स्थिर शून्य *
ngx_palloc_खण्डम्(ngx_pool_t *pool, आकार_t आकार)
{
    u_char      *m;
    आकार_t       psize;
    ngx_pool_t  *p, *new;

    psize = (आकार_t) (pool->d.अन्तः - (u_char *) pool);

    m = ngx_memalign(NGX_POOL_ALIGNMENT, psize, pool->लघुगणकः);
    यदि (m == रिक्त) {
        प्रत्यागम रिक्त;
    }

    new = (ngx_pool_t *) m;

    new->d.अन्तः = m + psize;
    new->d.अग्र = रिक्त;
    new->d.failed = 0;

    m += परिमाण(ngx_pool_दत्त_t);
    m = ngx_align_ptr(m, NGX_ALIGNMENT);
    new->d.चरमः = m + आकार;

    प्रति (p = pool->current; p->d.अग्र; p = p->d.अग्र) {
        यदि (p->d.failed++ > 4) {
            pool->current = p->d.अग्र;
        }
    }

    p->d.अग्र = new;

    प्रत्यागम m;
}


स्थिर शून्य *
ngx_palloc_large(ngx_pool_t *pool, आकार_t आकार)
{
    शून्य              *p;
    ngx_uint_t         n;
    ngx_pool_large_t  *large;

    p = ngx_alloc(आकार, pool->लघुगणकः);
    यदि (p == रिक्त) {
        प्रत्यागम रिक्त;
    }

    n = 0;

    प्रति (large = pool->large; large; large = large->अग्र) {
        यदि (large->alloc == रिक्त) {
            large->alloc = p;
            प्रत्यागम p;
        }

        यदि (n++ > 3) {
            भङ्ग;
        }
    }

    large = ngx_palloc_small(pool, परिमाण(ngx_pool_large_t), 1);
    यदि (large == रिक्त) {
        ngx_free(p);
        प्रत्यागम रिक्त;
    }

    large->alloc = p;
    large->अग्र = pool->large;
    pool->large = large;

    प्रत्यागम p;
}


शून्य *
ngx_pmemalign(ngx_pool_t *pool, आकार_t आकार, आकार_t alignment)
{
    शून्य              *p;
    ngx_pool_large_t  *large;

    p = ngx_memalign(alignment, आकार, pool->लघुगणकः);
    यदि (p == रिक्त) {
        प्रत्यागम रिक्त;
    }

    large = ngx_palloc_small(pool, परिमाण(ngx_pool_large_t), 1);
    यदि (large == रिक्त) {
        ngx_free(p);
        प्रत्यागम रिक्त;
    }

    large->alloc = p;
    large->अग्र = pool->large;
    pool->large = large;

    प्रत्यागम p;
}


ngx_int_t
ngx_pfree(ngx_pool_t *pool, शून्य *p)
{
    ngx_pool_large_t  *l;

    प्रति (l = pool->large; l; l = l->अग्र) {
        यदि (p == l->alloc) {
            ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->लघुगणकः, 0,
                           "free: %p", l->alloc);
            ngx_free(l->alloc);
            l->alloc = रिक्त;

            प्रत्यागम NGX_OK;
        }
    }

    प्रत्यागम NGX_DECLINED;
}


शून्य *
ngx_pcalloc(ngx_pool_t *pool, आकार_t आकार)
{
    शून्य *p;

    p = ngx_palloc(pool, आकार);
    यदि (p) {
        ngx_memzero(p, आकार);
    }

    प्रत्यागम p;
}


ngx_pool_cleanup_t *
ngx_pool_cleanup_सङ्कलय(ngx_pool_t *p, आकार_t आकार)
{
    ngx_pool_cleanup_t  *c;

    c = ngx_palloc(p, परिमाण(ngx_pool_cleanup_t));
    यदि (c == रिक्त) {
        प्रत्यागम रिक्त;
    }

    यदि (आकार) {
        c->दत्त = ngx_palloc(p, आकार);
        यदि (c->दत्त == रिक्त) {
            प्रत्यागम रिक्त;
        }

    } अन्यथा {
        c->दत्त = रिक्त;
    }

    c->handler = रिक्त;
    c->अग्र = p->cleanup;

    p->cleanup = c;

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, p->लघुगणकः, 0, "add cleanup: %p", c);

    प्रत्यागम c;
}


शून्य
ngx_pool_सततम्_cleanup_file(ngx_pool_t *p, ngx_fd_t fd)
{
    ngx_pool_cleanup_t       *c;
    ngx_pool_cleanup_file_t  *cf;

    प्रति (c = p->cleanup; c; c = c->अग्र) {
        यदि (c->handler == ngx_pool_cleanup_file) {

            cf = c->दत्त;

            यदि (cf->fd == fd) {
                c->handler(cf);
                c->handler = रिक्त;
                प्रत्यागम;
            }
        }
    }
}


शून्य
ngx_pool_cleanup_file(शून्य *दत्त)
{
    ngx_pool_cleanup_file_t  *c = दत्त;

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, c->लघुगणकः, 0, "file cleanup: fd:%d",
                   c->fd);

    यदि (ngx_close_file(c->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, c->लघुगणकः, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", c->name);
    }
}


शून्य
ngx_pool_delete_file(शून्य *दत्त)
{
    ngx_pool_cleanup_file_t  *c = दत्त;

    ngx_err_t  err;

    ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, c->लघुगणकः, 0, "file cleanup: fd:%d %s",
                   c->fd, c->name);

    यदि (ngx_delete_file(c->name) == NGX_FILE_ERROR) {
        err = ngx_errno;

        यदि (err != NGX_ENOENT) {
            ngx_log_error(NGX_LOG_CRIT, c->लघुगणकः, err,
                          ngx_delete_file_n " \"%s\" failed", c->name);
        }
    }

    यदि (ngx_close_file(c->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, c->लघुगणकः, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", c->name);
    }
}


#यदि 0

स्थिर शून्य *
ngx_get_cached_खण्डम्(आकार_t आकार)
{
    शून्य                     *p;
    ngx_cached_खण्डम्_slot_t  *slot;

    यदि (ngx_आवृत्तिः->cache == रिक्त) {
        प्रत्यागम रिक्त;
    }

    slot = &ngx_आवृत्तिः->cache[(आकार + ngx_pagesize - 1) / ngx_pagesize];

    slot->tries++;

    यदि (slot->सङ्ख्या) {
        p = slot->खण्डम्;
        slot->खण्डम् = slot->खण्डम्->अग्र;
        slot->सङ्ख्या--;
        प्रत्यागम p;
    }

    प्रत्यागम रिक्त;
}

#यदि_अन्तः
