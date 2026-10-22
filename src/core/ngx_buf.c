
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


ngx_buf_t *
ngx_create_عارضی_buf(ngx_pool_t *pool, ناپ_t ناپ)
{
    ngx_buf_t *b;

    b = ngx_calloc_buf(pool);
    إذا (b == عدم) {
        أرجع عدم;
    }

    b->شروع = ngx_palloc(pool, ناپ);
    إذا (b->شروع == عدم) {
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

    b->pos = b->شروع;
    b->آخری = b->شروع;
    b->سرا = b->آخری + ناپ;
    b->temporary = 1;

    أرجع b;
}


ngx_chain_t *
ngx_alloc_chain_link(ngx_pool_t *pool)
{
    ngx_chain_t  *cl;

    cl = pool->chain;

    إذا (cl) {
        pool->chain = cl->اگلا;
        أرجع cl;
    }

    cl = ngx_palloc(pool, حجم(ngx_chain_t));
    إذا (cl == عدم) {
        أرجع عدم;
    }

    أرجع cl;
}


ngx_chain_t *
ngx_create_chain_دا_bufs(ngx_pool_t *pool, ngx_bufs_t *bufs)
{
    u_char       *p;
    ngx_int_t     i;
    ngx_buf_t    *b;
    ngx_chain_t  *chain, *cl, **ll;

    p = ngx_palloc(pool, bufs->عدد * bufs->ناپ);
    إذا (p == عدم) {
        أرجع عدم;
    }

    ll = &chain;

    لكل (i = 0; i < bufs->عدد; i++) {

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
        b->آخری = p;
        b->temporary = 1;

        b->شروع = p;
        p += bufs->ناپ;
        b->سرا = p;

        cl = ngx_alloc_chain_link(pool);
        إذا (cl == عدم) {
            أرجع عدم;
        }

        cl->buf = b;
        *ll = cl;
        ll = &cl->اگلا;
    }

    *ll = عدم;

    أرجع chain;
}


ngx_int_t
ngx_chain_جوڑو_copy(ngx_pool_t *pool, ngx_chain_t **chain, ngx_chain_t *وچ)
{
    ngx_chain_t  *cl, **ll;

    ll = chain;

    لكل (cl = *chain; cl; cl = cl->اگلا) {
        ll = &cl->اگلا;
    }

    طالما (وچ) {
        cl = ngx_alloc_chain_link(pool);
        إذا (cl == عدم) {
            *ll = عدم;
            أرجع NGX_ERROR;
        }

        cl->buf = وچ->buf;
        *ll = cl;
        ll = &cl->اگلا;
        وچ = وچ->اگلا;
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
        *حرر = cl->اگلا;
        cl->اگلا = عدم;
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

    cl->اگلا = عدم;

    أرجع cl;
}


فراغ
ngx_chain_update_chains(ngx_pool_t *p, ngx_chain_t **حرر, ngx_chain_t **busy,
    ngx_chain_t **باہرلا, ngx_buf_ٹیگ_t ٹیگ)
{
    ngx_chain_t  *cl;

    إذا (*باہرلا) {
        إذا (*busy == عدم) {
            *busy = *باہرلا;

        } وإلا {
            لكل (cl = *busy; cl->اگلا; cl = cl->اگلا) { /* void */ }

            cl->اگلا = *باہرلا;
        }

        *باہرلا = عدم;
    }

    طالما (*busy) {
        cl = *busy;

        إذا (cl->buf->ٹیگ != ٹیگ) {
            *busy = cl->اگلا;
            ngx_free_chain(p, cl);
            استمر;
        }

        إذا (ngx_buf_ناپ(cl->buf) != 0) {
            اكسر;
        }

        cl->buf->pos = cl->buf->شروع;
        cl->buf->آخری = cl->buf->شروع;

        *busy = cl->اگلا;
        cl->اگلا = *حرر;
        *حرر = cl;
    }
}


off_t
ngx_chain_coalesce_file(ngx_chain_t **وچ, off_t حد_بندی)
{
    off_t         کل, ناپ, aligned, fprev;
    ngx_fd_t      fd;
    ngx_chain_t  *cl;

    کل = 0;

    cl = *وچ;
    fd = cl->buf->file->fd;

    افعل {
        ناپ = cl->buf->file_آخری - cl->buf->file_pos;

        إذا (ناپ > حد_بندی - کل) {
            ناپ = حد_بندی - کل;

            aligned = (cl->buf->file_pos + ناپ + ngx_pagesize - 1)
                       & ~((off_t) ngx_pagesize - 1);

            إذا (aligned <= cl->buf->file_آخری) {
                ناپ = aligned - cl->buf->file_pos;
            }

            کل += ناپ;
            اكسر;
        }

        کل += ناپ;
        fprev = cl->buf->file_pos + ناپ;
        cl = cl->اگلا;

    } طالما (cl
             && cl->buf->وچ_file
             && کل < حد_بندی
             && fd == cl->buf->file->fd
             && fprev == cl->buf->file_pos);

    *وچ = cl;

    أرجع کل;
}


ngx_chain_t *
ngx_chain_update_گھلیا(ngx_chain_t *وچ, off_t گھلیا)
{
    off_t  ناپ;

    لكل ( /* void */ ; وچ; وچ = وچ->اگلا) {

        إذا (ngx_buf_special(وچ->buf)) {
            استمر;
        }

        إذا (گھلیا == 0) {
            اكسر;
        }

        ناپ = ngx_buf_ناپ(وچ->buf);

        إذا (گھلیا >= ناپ) {
            گھلیا -= ناپ;

            إذا (ngx_buf_وچ_حافظہ(وچ->buf)) {
                وچ->buf->pos = وچ->buf->آخری;
            }

            إذا (وچ->buf->وچ_file) {
                وچ->buf->file_pos = وچ->buf->file_آخری;
            }

            استمر;
        }

        إذا (ngx_buf_وچ_حافظہ(وچ->buf)) {
            وچ->buf->pos += (ناپ_t) گھلیا;
        }

        إذا (وچ->buf->وچ_file) {
            وچ->buf->file_pos += گھلیا;
        }

        اكسر;
    }

    أرجع وچ;
}
