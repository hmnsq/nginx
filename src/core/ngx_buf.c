
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


ngx_buf_t *
ngx_create_temp_buf(ngx_pool_t *pool, size_t size)
{
    ngx_buf_t *b;

    b = ngx_calloc_buf(pool);
    إذا (b == عدم) {
        أرجع عدم;
    }

    b->start = ngx_palloc(pool, size);
    إذا (b->start == عدم) {
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

    b->pos = b->start;
    b->last = b->start;
    b->end = b->last + size;
    b->temporary = 1;

    أرجع b;
}


ngx_chain_t *
ngx_alloc_chain_link(ngx_pool_t *pool)
{
    ngx_chain_t  *cl;

    cl = pool->chain;

    إذا (cl) {
        pool->chain = cl->next;
        أرجع cl;
    }

    cl = ngx_palloc(pool, حجم(ngx_chain_t));
    إذا (cl == عدم) {
        أرجع عدم;
    }

    أرجع cl;
}


ngx_chain_t *
ngx_create_chain_of_bufs(ngx_pool_t *pool, ngx_bufs_t *bufs)
{
    u_char       *p;
    ngx_int_t     i;
    ngx_buf_t    *b;
    ngx_chain_t  *chain, *cl, **ll;

    p = ngx_palloc(pool, bufs->ژمارە * bufs->size);
    إذا (p == عدم) {
        أرجع عدم;
    }

    ll = &chain;

    لكل (i = 0; i < bufs->ژمارە; i++) {

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
        b->last = p;
        b->temporary = 1;

        b->start = p;
        p += bufs->size;
        b->end = p;

        cl = ngx_alloc_chain_link(pool);
        إذا (cl == عدم) {
            أرجع عدم;
        }

        cl->buf = b;
        *ll = cl;
        ll = &cl->next;
    }

    *ll = عدم;

    أرجع chain;
}


ngx_int_t
ngx_chain_add_copy(ngx_pool_t *pool, ngx_chain_t **chain, ngx_chain_t *in)
{
    ngx_chain_t  *cl, **ll;

    ll = chain;

    لكل (cl = *chain; cl; cl = cl->next) {
        ll = &cl->next;
    }

    طالما (in) {
        cl = ngx_alloc_chain_link(pool);
        إذا (cl == عدم) {
            *ll = عدم;
            أرجع NGX_ERROR;
        }

        cl->buf = in->buf;
        *ll = cl;
        ll = &cl->next;
        in = in->next;
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
        *حرر = cl->next;
        cl->next = عدم;
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

    cl->next = عدم;

    أرجع cl;
}


فراغ
ngx_chain_update_chains(ngx_pool_t *p, ngx_chain_t **حرر, ngx_chain_t **busy,
    ngx_chain_t **out, ngx_buf_tag_t tag)
{
    ngx_chain_t  *cl;

    إذا (*out) {
        إذا (*busy == عدم) {
            *busy = *out;

        } وإلا {
            لكل (cl = *busy; cl->next; cl = cl->next) { /* void */ }

            cl->next = *out;
        }

        *out = عدم;
    }

    طالما (*busy) {
        cl = *busy;

        إذا (cl->buf->tag != tag) {
            *busy = cl->next;
            ngx_free_chain(p, cl);
            استمر;
        }

        إذا (ngx_buf_size(cl->buf) != 0) {
            اكسر;
        }

        cl->buf->pos = cl->buf->start;
        cl->buf->last = cl->buf->start;

        *busy = cl->next;
        cl->next = *حرر;
        *حرر = cl;
    }
}


off_t
ngx_chain_coalesce_file(ngx_chain_t **in, off_t limit)
{
    off_t         total, size, aligned, fprev;
    ngx_fd_t      fd;
    ngx_chain_t  *cl;

    total = 0;

    cl = *in;
    fd = cl->buf->file->fd;

    افعل {
        size = cl->buf->file_last - cl->buf->file_pos;

        إذا (size > limit - total) {
            size = limit - total;

            aligned = (cl->buf->file_pos + size + ngx_pagesize - 1)
                       & ~((off_t) ngx_pagesize - 1);

            إذا (aligned <= cl->buf->file_last) {
                size = aligned - cl->buf->file_pos;
            }

            total += size;
            اكسر;
        }

        total += size;
        fprev = cl->buf->file_pos + size;
        cl = cl->next;

    } طالما (cl
             && cl->buf->in_file
             && total < limit
             && fd == cl->buf->file->fd
             && fprev == cl->buf->file_pos);

    *in = cl;

    أرجع total;
}


ngx_chain_t *
ngx_chain_update_sent(ngx_chain_t *in, off_t sent)
{
    off_t  size;

    لكل ( /* void */ ; in; in = in->next) {

        إذا (ngx_buf_special(in->buf)) {
            استمر;
        }

        إذا (sent == 0) {
            اكسر;
        }

        size = ngx_buf_size(in->buf);

        إذا (sent >= size) {
            sent -= size;

            إذا (ngx_buf_in_memory(in->buf)) {
                in->buf->pos = in->buf->last;
            }

            إذا (in->buf->in_file) {
                in->buf->file_pos = in->buf->file_last;
            }

            استمر;
        }

        إذا (ngx_buf_in_memory(in->buf)) {
            in->buf->pos += (size_t) sent;
        }

        إذا (in->buf->in_file) {
            in->buf->file_pos += sent;
        }

        اكسر;
    }

    أرجع in;
}
