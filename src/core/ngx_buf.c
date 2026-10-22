
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
    אם (b == عدم) {
        החזר عدم;
    }

    b->start = ngx_palloc(pool, size);
    אם (b->start == عدم) {
        החזר عدم;
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

    החזר b;
}


ngx_chain_t *
ngx_alloc_chain_link(ngx_pool_t *pool)
{
    ngx_chain_t  *cl;

    cl = pool->chain;

    אם (cl) {
        pool->chain = cl->next;
        החזר cl;
    }

    cl = ngx_palloc(pool, حجم(ngx_chain_t));
    אם (cl == عدم) {
        החזר عدم;
    }

    החזר cl;
}


ngx_chain_t *
ngx_create_chain_of_bufs(ngx_pool_t *pool, ngx_bufs_t *bufs)
{
    u_char       *p;
    ngx_int_t     i;
    ngx_buf_t    *b;
    ngx_chain_t  *chain, *cl, **ll;

    p = ngx_palloc(pool, bufs->num * bufs->size);
    אם (p == عدم) {
        החזר عدم;
    }

    ll = &chain;

    עבור (i = 0; i < bufs->num; i++) {

        b = ngx_calloc_buf(pool);
        אם (b == عدم) {
            החזר عدم;
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
        אם (cl == عدم) {
            החזר عدم;
        }

        cl->buf = b;
        *ll = cl;
        ll = &cl->next;
    }

    *ll = عدم;

    החזר chain;
}


ngx_int_t
ngx_chain_add_copy(ngx_pool_t *pool, ngx_chain_t **chain, ngx_chain_t *in)
{
    ngx_chain_t  *cl, **ll;

    ll = chain;

    עבור (cl = *chain; cl; cl = cl->next) {
        ll = &cl->next;
    }

    כלעוד (in) {
        cl = ngx_alloc_chain_link(pool);
        אם (cl == عدم) {
            *ll = عدم;
            החזר NGX_ERROR;
        }

        cl->buf = in->buf;
        *ll = cl;
        ll = &cl->next;
        in = in->next;
    }

    *ll = عدم;

    החזר NGX_OK;
}


ngx_chain_t *
ngx_chain_get_free_buf(ngx_pool_t *p, ngx_chain_t **حرر)
{
    ngx_chain_t  *cl;

    אם (*حرر) {
        cl = *حرر;
        *حرر = cl->next;
        cl->next = عدم;
        החזר cl;
    }

    cl = ngx_alloc_chain_link(p);
    אם (cl == عدم) {
        החזר عدم;
    }

    cl->buf = ngx_calloc_buf(p);
    אם (cl->buf == عدم) {
        החזר عدم;
    }

    cl->next = عدم;

    החזר cl;
}


فراغ
ngx_chain_update_chains(ngx_pool_t *p, ngx_chain_t **حرر, ngx_chain_t **busy,
    ngx_chain_t **out, ngx_buf_tag_t tag)
{
    ngx_chain_t  *cl;

    אם (*out) {
        אם (*busy == عدم) {
            *busy = *out;

        } אחרת {
            עבור (cl = *busy; cl->next; cl = cl->next) { /* void */ }

            cl->next = *out;
        }

        *out = عدم;
    }

    כלעוד (*busy) {
        cl = *busy;

        אם (cl->buf->tag != tag) {
            *busy = cl->next;
            ngx_free_chain(p, cl);
            استمر;
        }

        אם (ngx_buf_size(cl->buf) != 0) {
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

    עשה {
        size = cl->buf->file_last - cl->buf->file_pos;

        אם (size > limit - total) {
            size = limit - total;

            aligned = (cl->buf->file_pos + size + ngx_pagesize - 1)
                       & ~((off_t) ngx_pagesize - 1);

            אם (aligned <= cl->buf->file_last) {
                size = aligned - cl->buf->file_pos;
            }

            total += size;
            اكسر;
        }

        total += size;
        fprev = cl->buf->file_pos + size;
        cl = cl->next;

    } כלעוד (cl
             && cl->buf->in_file
             && total < limit
             && fd == cl->buf->file->fd
             && fprev == cl->buf->file_pos);

    *in = cl;

    החזר total;
}


ngx_chain_t *
ngx_chain_update_sent(ngx_chain_t *in, off_t sent)
{
    off_t  size;

    עבור ( /* void */ ; in; in = in->next) {

        אם (ngx_buf_special(in->buf)) {
            استمر;
        }

        אם (sent == 0) {
            اكسر;
        }

        size = ngx_buf_size(in->buf);

        אם (sent >= size) {
            sent -= size;

            אם (ngx_buf_in_memory(in->buf)) {
                in->buf->pos = in->buf->last;
            }

            אם (in->buf->in_file) {
                in->buf->file_pos = in->buf->file_last;
            }

            استمر;
        }

        אם (ngx_buf_in_memory(in->buf)) {
            in->buf->pos += (size_t) sent;
        }

        אם (in->buf->in_file) {
            in->buf->file_pos += sent;
        }

        اكسر;
    }

    החזר in;
}
