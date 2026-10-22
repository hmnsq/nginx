
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


ngx_buf_t *
ngx_create_لنډمهاله_buf(ngx_pool_t *pool, اندازه_t اندازه)
{
    ngx_buf_t *b;

    b = ngx_calloc_buf(pool);
    إذا (b == عدم) {
        أرجع عدم;
    }

    b->پیل = ngx_palloc(pool, اندازه);
    إذا (b->پیل == عدم) {
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

    b->pos = b->پیل;
    b->وروستی = b->پیل;
    b->پای_ټکی = b->وروستی + اندازه;
    b->temporary = 1;

    أرجع b;
}


ngx_chain_t *
ngx_alloc_chain_link(ngx_pool_t *pool)
{
    ngx_chain_t  *cl;

    cl = pool->chain;

    إذا (cl) {
        pool->chain = cl->بل;
        أرجع cl;
    }

    cl = ngx_palloc(pool, حجم(ngx_chain_t));
    إذا (cl == عدم) {
        أرجع عدم;
    }

    أرجع cl;
}


ngx_chain_t *
ngx_create_chain_د_bufs(ngx_pool_t *pool, ngx_bufs_t *bufs)
{
    u_char       *p;
    ngx_int_t     i;
    ngx_buf_t    *b;
    ngx_chain_t  *chain, *cl, **ll;

    p = ngx_palloc(pool, bufs->عدد * bufs->اندازه);
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
        b->وروستی = p;
        b->temporary = 1;

        b->پیل = p;
        p += bufs->اندازه;
        b->پای_ټکی = p;

        cl = ngx_alloc_chain_link(pool);
        إذا (cl == عدم) {
            أرجع عدم;
        }

        cl->buf = b;
        *ll = cl;
        ll = &cl->بل;
    }

    *ll = عدم;

    أرجع chain;
}


ngx_int_t
ngx_chain_جمع_کړه_copy(ngx_pool_t *pool, ngx_chain_t **chain, ngx_chain_t *دننه)
{
    ngx_chain_t  *cl, **ll;

    ll = chain;

    لكل (cl = *chain; cl; cl = cl->بل) {
        ll = &cl->بل;
    }

    طالما (دننه) {
        cl = ngx_alloc_chain_link(pool);
        إذا (cl == عدم) {
            *ll = عدم;
            أرجع NGX_ERROR;
        }

        cl->buf = دننه->buf;
        *ll = cl;
        ll = &cl->بل;
        دننه = دننه->بل;
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
        *حرر = cl->بل;
        cl->بل = عدم;
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

    cl->بل = عدم;

    أرجع cl;
}


فراغ
ngx_chain_update_chains(ngx_pool_t *p, ngx_chain_t **حرر, ngx_chain_t **busy,
    ngx_chain_t **بهر, ngx_buf_نښان_t نښان)
{
    ngx_chain_t  *cl;

    إذا (*بهر) {
        إذا (*busy == عدم) {
            *busy = *بهر;

        } وإلا {
            لكل (cl = *busy; cl->بل; cl = cl->بل) { /* void */ }

            cl->بل = *بهر;
        }

        *بهر = عدم;
    }

    طالما (*busy) {
        cl = *busy;

        إذا (cl->buf->نښان != نښان) {
            *busy = cl->بل;
            ngx_free_chain(p, cl);
            استمر;
        }

        إذا (ngx_buf_اندازه(cl->buf) != 0) {
            اكسر;
        }

        cl->buf->pos = cl->buf->پیل;
        cl->buf->وروستی = cl->buf->پیل;

        *busy = cl->بل;
        cl->بل = *حرر;
        *حرر = cl;
    }
}


off_t
ngx_chain_coalesce_file(ngx_chain_t **دننه, off_t حد)
{
    off_t         ټول, اندازه, aligned, fprev;
    ngx_fd_t      fd;
    ngx_chain_t  *cl;

    ټول = 0;

    cl = *دننه;
    fd = cl->buf->file->fd;

    افعل {
        اندازه = cl->buf->file_وروستی - cl->buf->file_pos;

        إذا (اندازه > حد - ټول) {
            اندازه = حد - ټول;

            aligned = (cl->buf->file_pos + اندازه + ngx_pagesize - 1)
                       & ~((off_t) ngx_pagesize - 1);

            إذا (aligned <= cl->buf->file_وروستی) {
                اندازه = aligned - cl->buf->file_pos;
            }

            ټول += اندازه;
            اكسر;
        }

        ټول += اندازه;
        fprev = cl->buf->file_pos + اندازه;
        cl = cl->بل;

    } طالما (cl
             && cl->buf->دننه_file
             && ټول < حد
             && fd == cl->buf->file->fd
             && fprev == cl->buf->file_pos);

    *دننه = cl;

    أرجع ټول;
}


ngx_chain_t *
ngx_chain_update_لېږل_شوی(ngx_chain_t *دننه, off_t لېږل_شوی)
{
    off_t  اندازه;

    لكل ( /* void */ ; دننه; دننه = دننه->بل) {

        إذا (ngx_buf_special(دننه->buf)) {
            استمر;
        }

        إذا (لېږل_شوی == 0) {
            اكسر;
        }

        اندازه = ngx_buf_اندازه(دننه->buf);

        إذا (لېږل_شوی >= اندازه) {
            لېږل_شوی -= اندازه;

            إذا (ngx_buf_دننه_حافظه(دننه->buf)) {
                دننه->buf->pos = دننه->buf->وروستی;
            }

            إذا (دننه->buf->دننه_file) {
                دننه->buf->file_pos = دننه->buf->file_وروستی;
            }

            استمر;
        }

        إذا (ngx_buf_دننه_حافظه(دننه->buf)) {
            دننه->buf->pos += (اندازه_t) لېږل_شوی;
        }

        إذا (دننه->buf->دننه_file) {
            دننه->buf->file_pos += لېږل_شوی;
        }

        اكسر;
    }

    أرجع دننه;
}
