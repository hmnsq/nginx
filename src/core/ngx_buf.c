
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#অন্তর্ভাৱ <ngx_config.h>
#অন্তর্ভাৱ <ngx_core.h>


ngx_buf_t *
ngx_create_temp_buf(ngx_pool_t *pool, size_t size)
{
    ngx_buf_t *b;

    b = ngx_calloc_buf(pool);
    যদি (b == রিক্ত) {
        ঘূৰাই রিক্ত;
    }

    b->start = ngx_palloc(pool, size);
    যদি (b->start == রিক্ত) {
        ঘূৰাই রিক্ত;
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

    ঘূৰাই b;
}


ngx_chain_t *
ngx_alloc_chain_link(ngx_pool_t *pool)
{
    ngx_chain_t  *cl;

    cl = pool->chain;

    যদি (cl) {
        pool->chain = cl->next;
        ঘূৰাই cl;
    }

    cl = ngx_palloc(pool, পরিমাণ(ngx_chain_t));
    যদি (cl == রিক্ত) {
        ঘূৰাই রিক্ত;
    }

    ঘূৰাই cl;
}


ngx_chain_t *
ngx_create_chain_of_bufs(ngx_pool_t *pool, ngx_bufs_t *bufs)
{
    u_char       *p;
    ngx_int_t     i;
    ngx_buf_t    *b;
    ngx_chain_t  *chain, *cl, **ll;

    p = ngx_palloc(pool, bufs->অঙ্ক * bufs->size);
    যদি (p == রিক্ত) {
        ঘূৰাই রিক্ত;
    }

    ll = &chain;

    বাবে (i = 0; i < bufs->অঙ্ক; i++) {

        b = ngx_calloc_buf(pool);
        যদি (b == রিক্ত) {
            ঘূৰাই রিক্ত;
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
        যদি (cl == রিক্ত) {
            ঘূৰাই রিক্ত;
        }

        cl->buf = b;
        *ll = cl;
        ll = &cl->next;
    }

    *ll = রিক্ত;

    ঘূৰাই chain;
}


ngx_int_t
ngx_chain_add_copy(ngx_pool_t *pool, ngx_chain_t **chain, ngx_chain_t *in)
{
    ngx_chain_t  *cl, **ll;

    ll = chain;

    বাবে (cl = *chain; cl; cl = cl->next) {
        ll = &cl->next;
    }

    যেতিয়ালৈ (in) {
        cl = ngx_alloc_chain_link(pool);
        যদি (cl == রিক্ত) {
            *ll = রিক্ত;
            ঘূৰাই NGX_ERROR;
        }

        cl->buf = in->buf;
        *ll = cl;
        ll = &cl->next;
        in = in->next;
    }

    *ll = রিক্ত;

    ঘূৰাই NGX_OK;
}


ngx_chain_t *
ngx_chain_get_free_buf(ngx_pool_t *p, ngx_chain_t **মুক্ত)
{
    ngx_chain_t  *cl;

    যদি (*মুক্ত) {
        cl = *মুক্ত;
        *মুক্ত = cl->next;
        cl->next = রিক্ত;
        ঘূৰাই cl;
    }

    cl = ngx_alloc_chain_link(p);
    যদি (cl == রিক্ত) {
        ঘূৰাই রিক্ত;
    }

    cl->buf = ngx_calloc_buf(p);
    যদি (cl->buf == রিক্ত) {
        ঘূৰাই রিক্ত;
    }

    cl->next = রিক্ত;

    ঘূৰাই cl;
}


শূন্য
ngx_chain_update_chains(ngx_pool_t *p, ngx_chain_t **মুক্ত, ngx_chain_t **busy,
    ngx_chain_t **out, ngx_buf_tag_t tag)
{
    ngx_chain_t  *cl;

    যদি (*out) {
        যদি (*busy == রিক্ত) {
            *busy = *out;

        } অন্যথা {
            বাবে (cl = *busy; cl->next; cl = cl->next) { /* void */ }

            cl->next = *out;
        }

        *out = রিক্ত;
    }

    যেতিয়ালৈ (*busy) {
        cl = *busy;

        যদি (cl->buf->tag != tag) {
            *busy = cl->next;
            ngx_free_chain(p, cl);
            অনুৱর্ত;
        }

        যদি (ngx_buf_size(cl->buf) != 0) {
            ভঙ্গ;
        }

        cl->buf->pos = cl->buf->start;
        cl->buf->last = cl->buf->start;

        *busy = cl->next;
        cl->next = *মুক্ত;
        *মুক্ত = cl;
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

    কৰা {
        size = cl->buf->file_last - cl->buf->file_pos;

        যদি (size > limit - total) {
            size = limit - total;

            aligned = (cl->buf->file_pos + size + ngx_pagesize - 1)
                       & ~((off_t) ngx_pagesize - 1);

            যদি (aligned <= cl->buf->file_last) {
                size = aligned - cl->buf->file_pos;
            }

            total += size;
            ভঙ্গ;
        }

        total += size;
        fprev = cl->buf->file_pos + size;
        cl = cl->next;

    } যেতিয়ালৈ (cl
             && cl->buf->in_file
             && total < limit
             && fd == cl->buf->file->fd
             && fprev == cl->buf->file_pos);

    *in = cl;

    ঘূৰাই total;
}


ngx_chain_t *
ngx_chain_update_sent(ngx_chain_t *in, off_t sent)
{
    off_t  size;

    বাবে ( /* void */ ; in; in = in->next) {

        যদি (ngx_buf_special(in->buf)) {
            অনুৱর্ত;
        }

        যদি (sent == 0) {
            ভঙ্গ;
        }

        size = ngx_buf_size(in->buf);

        যদি (sent >= size) {
            sent -= size;

            যদি (ngx_buf_in_memory(in->buf)) {
                in->buf->pos = in->buf->last;
            }

            যদি (in->buf->in_file) {
                in->buf->file_pos = in->buf->file_last;
            }

            অনুৱর্ত;
        }

        যদি (ngx_buf_in_memory(in->buf)) {
            in->buf->pos += (size_t) sent;
        }

        যদি (in->buf->in_file) {
            in->buf->file_pos += sent;
        }

        ভঙ্গ;
    }

    ঘূৰাই in;
}
