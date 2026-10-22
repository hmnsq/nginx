
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


ngx_buf_t *
ngx_create_عارضي_buf(ngx_pool_t *pool, سائيز_t سائيز)
{
    ngx_buf_t *b;

    b = ngx_calloc_buf(pool);
    إذا (b == عدم) {
        أرجع عدم;
    }

    b->شروعات = ngx_palloc(pool, سائيز);
    إذا (b->شروعات == عدم) {
        أرجع عدم;
    }

    /*
     * set by ngx_calloc_buf():
     *
     *     b->file_pos = 0;
     *     b->file_last = 0;
     *     b->file = NULL;
     *     b->shadow = NULL;
     *     b->tag = 0;
     *     and flags
     */

    b->pos = b->شروعات;
    b->آخري = b->شروعات;
    b->پڇاڙي = b->آخري + سائيز;
    b->temporary = 1;

    أرجع b;
}


ngx_chain_t *
ngx_alloc_chain_link(ngx_pool_t *pool)
{
    ngx_chain_t  *cl;

    cl = pool->chain;

    إذا (cl) {
        pool->chain = cl->ايندڙ;
        أرجع cl;
    }

    cl = ngx_palloc(pool, حجم(ngx_chain_t));
    إذا (cl == عدم) {
        أرجع عدم;
    }

    أرجع cl;
}


ngx_chain_t *
ngx_create_chain_جو_bufs(ngx_pool_t *pool, ngx_bufs_t *bufs)
{
    u_char       *p;
    ngx_int_t     i;
    ngx_buf_t    *b;
    ngx_chain_t  *chain, *cl, **ll;

    p = ngx_palloc(pool, bufs->انگ * bufs->سائيز);
    إذا (p == عدم) {
        أرجع عدم;
    }

    ll = &chain;

    لكل (i = 0; i < bufs->انگ; i++) {

        b = ngx_calloc_buf(pool);
        إذا (b == عدم) {
            أرجع عدم;
        }

        /*
         * set by ngx_calloc_buf():
         *
         *     b->file_pos = 0;
         *     b->file_last = 0;
         *     b->file = NULL;
         *     b->shadow = NULL;
         *     b->tag = 0;
         *     and flags
         *
         */

        b->pos = p;
        b->آخري = p;
        b->temporary = 1;

        b->شروعات = p;
        p += bufs->سائيز;
        b->پڇاڙي = p;

        cl = ngx_alloc_chain_link(pool);
        إذا (cl == عدم) {
            أرجع عدم;
        }

        cl->buf = b;
        *ll = cl;
        ll = &cl->ايندڙ;
    }

    *ll = عدم;

    أرجع chain;
}


ngx_int_t
ngx_chain_جوڙيو_copy(ngx_pool_t *pool, ngx_chain_t **chain, ngx_chain_t *اندريون)
{
    ngx_chain_t  *cl, **ll;

    ll = chain;

    لكل (cl = *chain; cl; cl = cl->ايندڙ) {
        ll = &cl->ايندڙ;
    }

    طالما (اندريون) {
        cl = ngx_alloc_chain_link(pool);
        إذا (cl == عدم) {
            *ll = عدم;
            أرجع NGX_ERROR;
        }

        cl->buf = اندريون->buf;
        *ll = cl;
        ll = &cl->ايندڙ;
        اندريون = اندريون->ايندڙ;
    }

    *ll = عدم;

    أرجع NGX_OK;
}


ngx_chain_t *
ngx_chain_get_free_buf(ngx_pool_t *p, ngx_chain_t **حرر)
{
    ngx_chain_t  *cl;

    إذا (*حرر) {
        cl = *حرر;
        *حرر = cl->ايندڙ;
        cl->ايندڙ = عدم;
        أرجع cl;
    }

    cl = ngx_alloc_chain_link(p);
    إذا (cl == عدم) {
        أرجع عدم;
    }

    cl->buf = ngx_calloc_buf(p);
    إذا (cl->buf == عدم) {
        أرجع عدم;
    }

    cl->ايندڙ = عدم;

    أرجع cl;
}


فراغ
ngx_chain_update_chains(ngx_pool_t *p, ngx_chain_t **حرر, ngx_chain_t **busy,
    ngx_chain_t **ٻاهريون, ngx_buf_ٽيگ_t ٽيگ)
{
    ngx_chain_t  *cl;

    إذا (*ٻاهريون) {
        إذا (*busy == عدم) {
            *busy = *ٻاهريون;

        } وإلا {
            لكل (cl = *busy; cl->ايندڙ; cl = cl->ايندڙ) { /* void */ }

            cl->ايندڙ = *ٻاهريون;
        }

        *ٻاهريون = عدم;
    }

    طالما (*busy) {
        cl = *busy;

        إذا (cl->buf->ٽيگ != ٽيگ) {
            *busy = cl->ايندڙ;
            ngx_free_chain(p, cl);
            استمر;
        }

        إذا (ngx_buf_سائيز(cl->buf) != 0) {
            اكسر;
        }

        cl->buf->pos = cl->buf->شروعات;
        cl->buf->آخري = cl->buf->شروعات;

        *busy = cl->ايندڙ;
        cl->ايندڙ = *حرر;
        *حرر = cl;
    }
}


off_t
ngx_chain_coalesce_file(ngx_chain_t **اندريون, off_t حد_بندي)
{
    off_t         ڪل, سائيز, aligned, fprev;
    ngx_fd_t      fd;
    ngx_chain_t  *cl;

    ڪل = 0;

    cl = *اندريون;
    fd = cl->buf->file->fd;

    افعل {
        سائيز = cl->buf->file_آخري - cl->buf->file_pos;

        إذا (سائيز > حد_بندي - ڪل) {
            سائيز = حد_بندي - ڪل;

            aligned = (cl->buf->file_pos + سائيز + ngx_pagesize - 1)
                       & ~((off_t) ngx_pagesize - 1);

            إذا (aligned <= cl->buf->file_آخري) {
                سائيز = aligned - cl->buf->file_pos;
            }

            ڪل += سائيز;
            اكسر;
        }

        ڪل += سائيز;
        fprev = cl->buf->file_pos + سائيز;
        cl = cl->ايندڙ;

    } طالما (cl
             && cl->buf->اندريون_file
             && ڪل < حد_بندي
             && fd == cl->buf->file->fd
             && fprev == cl->buf->file_pos);

    *اندريون = cl;

    أرجع ڪل;
}


ngx_chain_t *
ngx_chain_update_موڪليل(ngx_chain_t *اندريون, off_t موڪليل)
{
    off_t  سائيز;

    لكل ( /* void */ ; اندريون; اندريون = اندريون->ايندڙ) {

        إذا (ngx_buf_special(اندريون->buf)) {
            استمر;
        }

        إذا (موڪليل == 0) {
            اكسر;
        }

        سائيز = ngx_buf_سائيز(اندريون->buf);

        إذا (موڪليل >= سائيز) {
            موڪليل -= سائيز;

            إذا (ngx_buf_اندريون_يادگيري(اندريون->buf)) {
                اندريون->buf->pos = اندريون->buf->آخري;
            }

            إذا (اندريون->buf->اندريون_file) {
                اندريون->buf->file_pos = اندريون->buf->file_آخري;
            }

            استمر;
        }

        إذا (ngx_buf_اندريون_يادگيري(اندريون->buf)) {
            اندريون->buf->pos += (سائيز_t) موڪليل;
        }

        إذا (اندريون->buf->اندريون_file) {
            اندريون->buf->file_pos += موڪليل;
        }

        اكسر;
    }

    أرجع اندريون;
}
