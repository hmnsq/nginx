
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


ngx_buf_t *
ngx_create_موقت_buf(ngx_pool_t *pool, اندازه_t اندازه)
{
    ngx_buf_t *b;

    b = ngx_calloc_buf(pool);
    إذا (b == عدم) {
        أرجع عدم;
    }

    b->شروع = ngx_palloc(pool, اندازه);
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
    b->آخرین = b->شروع;
    b->انتها = b->آخرین + اندازه;
    b->temporary = 1;

    أرجع b;
}


ngx_chain_t *
ngx_alloc_chain_link(ngx_pool_t *pool)
{
    ngx_chain_t  *cl;

    cl = pool->chain;

    إذا (cl) {
        pool->chain = cl->بعدی;
        أرجع cl;
    }

    cl = ngx_palloc(pool, حجم(ngx_chain_t));
    إذا (cl == عدم) {
        أرجع عدم;
    }

    أرجع cl;
}


ngx_chain_t *
ngx_create_chain_از_bufs(ngx_pool_t *pool, ngx_bufs_t *bufs)
{
    u_char       *p;
    ngx_int_t     i;
    ngx_buf_t    *b;
    ngx_chain_t  *chain, *cl, **ll;

    p = ngx_palloc(pool, bufs->رقم * bufs->اندازه);
    إذا (p == عدم) {
        أرجع عدم;
    }

    ll = &chain;

    لكل (i = 0; i < bufs->رقم; i++) {

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
        b->آخرین = p;
        b->temporary = 1;

        b->شروع = p;
        p += bufs->اندازه;
        b->انتها = p;

        cl = ngx_alloc_chain_link(pool);
        إذا (cl == عدم) {
            أرجع عدم;
        }

        cl->buf = b;
        *ll = cl;
        ll = &cl->بعدی;
    }

    *ll = عدم;

    أرجع chain;
}


ngx_int_t
ngx_chain_جمع_کن_copy(ngx_pool_t *pool, ngx_chain_t **chain, ngx_chain_t *درون)
{
    ngx_chain_t  *cl, **ll;

    ll = chain;

    لكل (cl = *chain; cl; cl = cl->بعدی) {
        ll = &cl->بعدی;
    }

    طالما (درون) {
        cl = ngx_alloc_chain_link(pool);
        إذا (cl == عدم) {
            *ll = عدم;
            أرجع NGX_ERROR;
        }

        cl->buf = درون->buf;
        *ll = cl;
        ll = &cl->بعدی;
        درون = درون->بعدی;
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
        *حرر = cl->بعدی;
        cl->بعدی = عدم;
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

    cl->بعدی = عدم;

    أرجع cl;
}


فراغ
ngx_chain_update_chains(ngx_pool_t *p, ngx_chain_t **حرر, ngx_chain_t **busy,
    ngx_chain_t **بیرون, ngx_buf_برچسب_t برچسب)
{
    ngx_chain_t  *cl;

    إذا (*بیرون) {
        إذا (*busy == عدم) {
            *busy = *بیرون;

        } وإلا {
            لكل (cl = *busy; cl->بعدی; cl = cl->بعدی) { /* void */ }

            cl->بعدی = *بیرون;
        }

        *بیرون = عدم;
    }

    طالما (*busy) {
        cl = *busy;

        إذا (cl->buf->برچسب != برچسب) {
            *busy = cl->بعدی;
            ngx_free_chain(p, cl);
            استمر;
        }

        إذا (ngx_buf_اندازه(cl->buf) != 0) {
            اكسر;
        }

        cl->buf->pos = cl->buf->شروع;
        cl->buf->آخرین = cl->buf->شروع;

        *busy = cl->بعدی;
        cl->بعدی = *حرر;
        *حرر = cl;
    }
}


off_t
ngx_chain_coalesce_file(ngx_chain_t **درون, off_t حد)
{
    off_t         مجموع, اندازه, aligned, fprev;
    ngx_fd_t      fd;
    ngx_chain_t  *cl;

    مجموع = 0;

    cl = *درون;
    fd = cl->buf->file->fd;

    افعل {
        اندازه = cl->buf->file_آخرین - cl->buf->file_pos;

        إذا (اندازه > حد - مجموع) {
            اندازه = حد - مجموع;

            aligned = (cl->buf->file_pos + اندازه + ngx_pagesize - 1)
                       & ~((off_t) ngx_pagesize - 1);

            إذا (aligned <= cl->buf->file_آخرین) {
                اندازه = aligned - cl->buf->file_pos;
            }

            مجموع += اندازه;
            اكسر;
        }

        مجموع += اندازه;
        fprev = cl->buf->file_pos + اندازه;
        cl = cl->بعدی;

    } طالما (cl
             && cl->buf->درون_file
             && مجموع < حد
             && fd == cl->buf->file->fd
             && fprev == cl->buf->file_pos);

    *درون = cl;

    أرجع مجموع;
}


ngx_chain_t *
ngx_chain_update_فرستاده(ngx_chain_t *درون, off_t فرستاده)
{
    off_t  اندازه;

    لكل ( /* void */ ; درون; درون = درون->بعدی) {

        إذا (ngx_buf_special(درون->buf)) {
            استمر;
        }

        إذا (فرستاده == 0) {
            اكسر;
        }

        اندازه = ngx_buf_اندازه(درون->buf);

        إذا (فرستاده >= اندازه) {
            فرستاده -= اندازه;

            إذا (ngx_buf_درون_حافظه(درون->buf)) {
                درون->buf->pos = درون->buf->آخرین;
            }

            إذا (درون->buf->درون_file) {
                درون->buf->file_pos = درون->buf->file_آخرین;
            }

            استمر;
        }

        إذا (ngx_buf_درون_حافظه(درون->buf)) {
            درون->buf->pos += (اندازه_t) فرستاده;
        }

        إذا (درون->buf->درون_file) {
            درون->buf->file_pos += فرستاده;
        }

        اكسر;
    }

    أرجع درون;
}
