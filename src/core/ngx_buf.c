
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
    ܐܢ (b == عدم) {
        ܦܢܐ عدم;
    }

    b->start = ngx_palloc(pool, size);
    ܐܢ (b->start == عدم) {
        ܦܢܐ عدم;
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

    ܦܢܐ b;
}


ngx_chain_t *
ngx_alloc_chain_link(ngx_pool_t *pool)
{
    ngx_chain_t  *cl;

    cl = pool->chain;

    ܐܢ (cl) {
        pool->chain = cl->next;
        ܦܢܐ cl;
    }

    cl = ngx_palloc(pool, حجم(ngx_chain_t));
    ܐܢ (cl == عدم) {
        ܦܢܐ عدم;
    }

    ܦܢܐ cl;
}


ngx_chain_t *
ngx_create_chain_of_bufs(ngx_pool_t *pool, ngx_bufs_t *bufs)
{
    u_char       *p;
    ngx_int_t     i;
    ngx_buf_t    *b;
    ngx_chain_t  *chain, *cl, **ll;

    p = ngx_palloc(pool, bufs->num * bufs->size);
    ܐܢ (p == عدم) {
        ܦܢܐ عدم;
    }

    ll = &chain;

    لكل (i = 0; i < bufs->num; i++) {

        b = ngx_calloc_buf(pool);
        ܐܢ (b == عدم) {
            ܦܢܐ عدم;
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
        ܐܢ (cl == عدم) {
            ܦܢܐ عدم;
        }

        cl->buf = b;
        *ll = cl;
        ll = &cl->next;
    }

    *ll = عدم;

    ܦܢܐ chain;
}


ngx_int_t
ngx_chain_add_copy(ngx_pool_t *pool, ngx_chain_t **chain, ngx_chain_t *in)
{
    ngx_chain_t  *cl, **ll;

    ll = chain;

    لكل (cl = *chain; cl; cl = cl->next) {
        ll = &cl->next;
    }

    ܟܡܐ (in) {
        cl = ngx_alloc_chain_link(pool);
        ܐܢ (cl == عدم) {
            *ll = عدم;
            ܦܢܐ NGX_ERROR;
        }

        cl->buf = in->buf;
        *ll = cl;
        ll = &cl->next;
        in = in->next;
    }

    *ll = عدم;

    ܦܢܐ NGX_OK;
}


ngx_chain_t *
ngx_chain_get_free_buf(ngx_pool_t *p, ngx_chain_t **حرر)
{
    ngx_chain_t  *cl;

    ܐܢ (*حرر) {
        cl = *حرر;
        *حرر = cl->next;
        cl->next = عدم;
        ܦܢܐ cl;
    }

    cl = ngx_alloc_chain_link(p);
    ܐܢ (cl == عدم) {
        ܦܢܐ عدم;
    }

    cl->buf = ngx_calloc_buf(p);
    ܐܢ (cl->buf == عدم) {
        ܦܢܐ عدم;
    }

    cl->next = عدم;

    ܦܢܐ cl;
}


فراغ
ngx_chain_update_chains(ngx_pool_t *p, ngx_chain_t **حرر, ngx_chain_t **busy,
    ngx_chain_t **out, ngx_buf_tag_t tag)
{
    ngx_chain_t  *cl;

    ܐܢ (*out) {
        ܐܢ (*busy == عدم) {
            *busy = *out;

        } ܐܚܪܢܐ {
            لكل (cl = *busy; cl->next; cl = cl->next) { /* void */ }

            cl->next = *out;
        }

        *out = عدم;
    }

    ܟܡܐ (*busy) {
        cl = *busy;

        ܐܢ (cl->buf->tag != tag) {
            *busy = cl->next;
            ngx_free_chain(p, cl);
            استمر;
        }

        ܐܢ (ngx_buf_size(cl->buf) != 0) {
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

    ܥܒܕ {
        size = cl->buf->file_last - cl->buf->file_pos;

        ܐܢ (size > limit - total) {
            size = limit - total;

            aligned = (cl->buf->file_pos + size + ngx_pagesize - 1)
                       & ~((off_t) ngx_pagesize - 1);

            ܐܢ (aligned <= cl->buf->file_last) {
                size = aligned - cl->buf->file_pos;
            }

            total += size;
            اكسر;
        }

        total += size;
        fprev = cl->buf->file_pos + size;
        cl = cl->next;

    } ܟܡܐ (cl
             && cl->buf->in_file
             && total < limit
             && fd == cl->buf->file->fd
             && fprev == cl->buf->file_pos);

    *in = cl;

    ܦܢܐ total;
}


ngx_chain_t *
ngx_chain_update_sent(ngx_chain_t *in, off_t sent)
{
    off_t  size;

    لكل ( /* void */ ; in; in = in->next) {

        ܐܢ (ngx_buf_special(in->buf)) {
            استمر;
        }

        ܐܢ (sent == 0) {
            اكسر;
        }

        size = ngx_buf_size(in->buf);

        ܐܢ (sent >= size) {
            sent -= size;

            ܐܢ (ngx_buf_in_memory(in->buf)) {
                in->buf->pos = in->buf->last;
            }

            ܐܢ (in->buf->in_file) {
                in->buf->file_pos = in->buf->file_last;
            }

            استمر;
        }

        ܐܢ (ngx_buf_in_memory(in->buf)) {
            in->buf->pos += (size_t) sent;
        }

        ܐܢ (in->buf->in_file) {
            in->buf->file_pos += sent;
        }

        اكسر;
    }

    ܦܢܐ in;
}
