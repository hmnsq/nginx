
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>


స్థిర ngx_inline శూన్య *ngx_palloc_small(ngx_pool_t *pool, కొలత_t కొలత,
    ngx_uint_t align);
స్థిర శూన్య *ngx_palloc_ఖండం(ngx_pool_t *pool, కొలత_t కొలత);
స్థిర శూన్య *ngx_palloc_large(ngx_pool_t *pool, కొలత_t కొలత);


ngx_pool_t *
ngx_create_pool(కొలత_t కొలత, ngx_log_t *సంవర్గమానం)
{
    ngx_pool_t  *p;

    p = ngx_memalign(NGX_POOL_ALIGNMENT, కొలత, సంవర్గమానం);
    ఒకవేళ (p == ఖాళీ) {
        తిరిగి ఖాళీ;
    }

    p->d.చివరి = (u_char *) p + పరిమాణం(ngx_pool_t);
    p->d.చివర = (u_char *) p + కొలత;
    p->d.తదుపరి = ఖాళీ;
    p->d.failed = 0;

    కొలత = కొలత - పరిమాణం(ngx_pool_t);
    p->max = (కొలత < NGX_MAX_ALLOC_FROM_POOL) ? కొలత : NGX_MAX_ALLOC_FROM_POOL;

    p->current = p;
    p->chain = ఖాళీ;
    p->large = ఖాళీ;
    p->cleanup = ఖాళీ;
    p->సంవర్గమానం = సంవర్గమానం;

    తిరిగి p;
}


శూన్య
ngx_destroy_pool(ngx_pool_t *pool)
{
    ngx_pool_t          *p, *n;
    ngx_pool_large_t    *l;
    ngx_pool_cleanup_t  *c;

    కొరకు (c = pool->cleanup; c; c = c->తదుపరి) {
        ఒకవేళ (c->handler) {
            ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->సంవర్గమానం, 0,
                           "run cleanup: %p", c);
            c->handler(c->దత్తాంశం);
        }
    }

#ఒకవేళ (NGX_DEBUG)

    /*
     * we could allocate the pool->log from this pool
     * so we cannot use this log while free()ing the pool
     */

    కొరకు (l = pool->large; l; l = l->తదుపరి) {
        ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->సంవర్గమానం, 0, "free: %p", l->alloc);
    }

    కొరకు (p = pool, n = pool->d.తదుపరి; /* void */; p = n, n = n->d.తదుపరి) {
        ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, pool->సంవర్గమానం, 0,
                       "free: %p, unused: %uz", p, p->d.చివర - p->d.చివరి);

        ఒకవేళ (n == ఖాళీ) {
            ఆపు;
        }
    }

#అయితే_ముగింపు

    కొరకు (l = pool->large; l; l = l->తదుపరి) {
        ఒకవేళ (l->alloc) {
            ngx_free(l->alloc);
        }
    }

    కొరకు (p = pool, n = pool->d.తదుపరి; /* void */; p = n, n = n->d.తదుపరి) {
        ngx_free(p);

        ఒకవేళ (n == ఖాళీ) {
            ఆపు;
        }
    }
}


శూన్య
ngx_reset_pool(ngx_pool_t *pool)
{
    ngx_pool_t        *p;
    ngx_pool_large_t  *l;

    కొరకు (l = pool->large; l; l = l->తదుపరి) {
        ఒకవేళ (l->alloc) {
            ngx_free(l->alloc);
        }
    }

    కొరకు (p = pool; p; p = p->d.తదుపరి) {
        p->d.చివరి = (u_char *) p + పరిమాణం(ngx_pool_t);
        p->d.failed = 0;
    }

    pool->current = pool;
    pool->chain = ఖాళీ;
    pool->large = ఖాళీ;
}


శూన్య *
ngx_palloc(ngx_pool_t *pool, కొలత_t కొలత)
{
#ఒకవేళ !(NGX_DEBUG_PALLOC)
    ఒకవేళ (కొలత <= pool->max) {
        తిరిగి ngx_palloc_small(pool, కొలత, 1);
    }
#అయితే_ముగింపు

    తిరిగి ngx_palloc_large(pool, కొలత);
}


శూన్య *
ngx_pnalloc(ngx_pool_t *pool, కొలత_t కొలత)
{
#ఒకవేళ !(NGX_DEBUG_PALLOC)
    ఒకవేళ (కొలత <= pool->max) {
        తిరిగి ngx_palloc_small(pool, కొలత, 0);
    }
#అయితే_ముగింపు

    తిరిగి ngx_palloc_large(pool, కొలత);
}


స్థిర ngx_inline శూన్య *
ngx_palloc_small(ngx_pool_t *pool, కొలత_t కొలత, ngx_uint_t align)
{
    u_char      *m;
    ngx_pool_t  *p;

    p = pool->current;

    చేయి {
        m = p->d.చివరి;

        ఒకవేళ (align) {
            m = ngx_align_ptr(m, NGX_ALIGNMENT);
        }

        ఒకవేళ ((కొలత_t) (p->d.చివర - m) >= కొలత) {
            p->d.చివరి = m + కొలత;

            తిరిగి m;
        }

        p = p->d.తదుపరి;

    } అయితే (p);

    తిరిగి ngx_palloc_ఖండం(pool, కొలత);
}


స్థిర శూన్య *
ngx_palloc_ఖండం(ngx_pool_t *pool, కొలత_t కొలత)
{
    u_char      *m;
    కొలత_t       psize;
    ngx_pool_t  *p, *new;

    psize = (కొలత_t) (pool->d.చివర - (u_char *) pool);

    m = ngx_memalign(NGX_POOL_ALIGNMENT, psize, pool->సంవర్గమానం);
    ఒకవేళ (m == ఖాళీ) {
        తిరిగి ఖాళీ;
    }

    new = (ngx_pool_t *) m;

    new->d.చివర = m + psize;
    new->d.తదుపరి = ఖాళీ;
    new->d.failed = 0;

    m += పరిమాణం(ngx_pool_దత్తాంశం_t);
    m = ngx_align_ptr(m, NGX_ALIGNMENT);
    new->d.చివరి = m + కొలత;

    కొరకు (p = pool->current; p->d.తదుపరి; p = p->d.తదుపరి) {
        ఒకవేళ (p->d.failed++ > 4) {
            pool->current = p->d.తదుపరి;
        }
    }

    p->d.తదుపరి = new;

    తిరిగి m;
}


స్థిర శూన్య *
ngx_palloc_large(ngx_pool_t *pool, కొలత_t కొలత)
{
    శూన్య              *p;
    ngx_uint_t         n;
    ngx_pool_large_t  *large;

    p = ngx_alloc(కొలత, pool->సంవర్గమానం);
    ఒకవేళ (p == ఖాళీ) {
        తిరిగి ఖాళీ;
    }

    n = 0;

    కొరకు (large = pool->large; large; large = large->తదుపరి) {
        ఒకవేళ (large->alloc == ఖాళీ) {
            large->alloc = p;
            తిరిగి p;
        }

        ఒకవేళ (n++ > 3) {
            ఆపు;
        }
    }

    large = ngx_palloc_small(pool, పరిమాణం(ngx_pool_large_t), 1);
    ఒకవేళ (large == ఖాళీ) {
        ngx_free(p);
        తిరిగి ఖాళీ;
    }

    large->alloc = p;
    large->తదుపరి = pool->large;
    pool->large = large;

    తిరిగి p;
}


శూన్య *
ngx_pmemalign(ngx_pool_t *pool, కొలత_t కొలత, కొలత_t alignment)
{
    శూన్య              *p;
    ngx_pool_large_t  *large;

    p = ngx_memalign(alignment, కొలత, pool->సంవర్గమానం);
    ఒకవేళ (p == ఖాళీ) {
        తిరిగి ఖాళీ;
    }

    large = ngx_palloc_small(pool, పరిమాణం(ngx_pool_large_t), 1);
    ఒకవేళ (large == ఖాళీ) {
        ngx_free(p);
        తిరిగి ఖాళీ;
    }

    large->alloc = p;
    large->తదుపరి = pool->large;
    pool->large = large;

    తిరిగి p;
}


ngx_int_t
ngx_pfree(ngx_pool_t *pool, శూన్య *p)
{
    ngx_pool_large_t  *l;

    కొరకు (l = pool->large; l; l = l->తదుపరి) {
        ఒకవేళ (p == l->alloc) {
            ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->సంవర్గమానం, 0,
                           "free: %p", l->alloc);
            ngx_free(l->alloc);
            l->alloc = ఖాళీ;

            తిరిగి NGX_OK;
        }
    }

    తిరిగి NGX_DECLINED;
}


శూన్య *
ngx_pcalloc(ngx_pool_t *pool, కొలత_t కొలత)
{
    శూన్య *p;

    p = ngx_palloc(pool, కొలత);
    ఒకవేళ (p) {
        ngx_memzero(p, కొలత);
    }

    తిరిగి p;
}


ngx_pool_cleanup_t *
ngx_pool_cleanup_కూడు(ngx_pool_t *p, కొలత_t కొలత)
{
    ngx_pool_cleanup_t  *c;

    c = ngx_palloc(p, పరిమాణం(ngx_pool_cleanup_t));
    ఒకవేళ (c == ఖాళీ) {
        తిరిగి ఖాళీ;
    }

    ఒకవేళ (కొలత) {
        c->దత్తాంశం = ngx_palloc(p, కొలత);
        ఒకవేళ (c->దత్తాంశం == ఖాళీ) {
            తిరిగి ఖాళీ;
        }

    } లేకపోతే {
        c->దత్తాంశం = ఖాళీ;
    }

    c->handler = ఖాళీ;
    c->తదుపరి = p->cleanup;

    p->cleanup = c;

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, p->సంవర్గమానం, 0, "add cleanup: %p", c);

    తిరిగి c;
}


శూన్య
ngx_pool_నిరంతరం_cleanup_file(ngx_pool_t *p, ngx_fd_t fd)
{
    ngx_pool_cleanup_t       *c;
    ngx_pool_cleanup_file_t  *cf;

    కొరకు (c = p->cleanup; c; c = c->తదుపరి) {
        ఒకవేళ (c->handler == ngx_pool_cleanup_file) {

            cf = c->దత్తాంశం;

            ఒకవేళ (cf->fd == fd) {
                c->handler(cf);
                c->handler = ఖాళీ;
                తిరిగి;
            }
        }
    }
}


శూన్య
ngx_pool_cleanup_file(శూన్య *దత్తాంశం)
{
    ngx_pool_cleanup_file_t  *c = దత్తాంశం;

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, c->సంవర్గమానం, 0, "file cleanup: fd:%d",
                   c->fd);

    ఒకవేళ (ngx_close_file(c->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, c->సంవర్గమానం, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", c->name);
    }
}


శూన్య
ngx_pool_delete_file(శూన్య *దత్తాంశం)
{
    ngx_pool_cleanup_file_t  *c = దత్తాంశం;

    ngx_err_t  err;

    ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, c->సంవర్గమానం, 0, "file cleanup: fd:%d %s",
                   c->fd, c->name);

    ఒకవేళ (ngx_delete_file(c->name) == NGX_FILE_ERROR) {
        err = ngx_errno;

        ఒకవేళ (err != NGX_ENOENT) {
            ngx_log_error(NGX_LOG_CRIT, c->సంవర్గమానం, err,
                          ngx_delete_file_n " \"%s\" failed", c->name);
        }
    }

    ఒకవేళ (ngx_close_file(c->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, c->సంవర్గమానం, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", c->name);
    }
}


#ఒకవేళ 0

స్థిర శూన్య *
ngx_get_cached_ఖండం(కొలత_t కొలత)
{
    శూన్య                     *p;
    ngx_cached_ఖండం_స్థానం_t  *స్థానం;

    ఒకవేళ (ngx_చక్రం->cache == ఖాళీ) {
        తిరిగి ఖాళీ;
    }

    స్థానం = &ngx_చక్రం->cache[(కొలత + ngx_pagesize - 1) / ngx_pagesize];

    స్థానం->tries++;

    ఒకవేళ (స్థానం->సంఖ్య) {
        p = స్థానం->ఖండం;
        స్థానం->ఖండం = స్థానం->ఖండం->తదుపరి;
        స్థానం->సంఖ్య--;
        తిరిగి p;
    }

    తిరిగి ఖాళీ;
}

#అయితే_ముగింపు
