
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


ngx_buf_t *
ngx_create_temp_buf(ngx_pool_t *pool, size_t size)
{
    ngx_buf_t *b;

    b = ngx_calloc_buf(pool);
    ಯದಿ (b == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    b->start = ngx_palloc(pool, size);
    ಯದಿ (b->start == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
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

    ಪ್ರತ್ಯಾಗಮ b;
}


ngx_chain_t *
ngx_alloc_chain_link(ngx_pool_t *pool)
{
    ngx_chain_t  *cl;

    cl = pool->chain;

    ಯದಿ (cl) {
        pool->chain = cl->next;
        ಪ್ರತ್ಯಾಗಮ cl;
    }

    cl = ngx_palloc(pool, ಪರಿಮಾಣ(ngx_chain_t));
    ಯದಿ (cl == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ಪ್ರತ್ಯಾಗಮ cl;
}


ngx_chain_t *
ngx_create_chain_of_bufs(ngx_pool_t *pool, ngx_bufs_t *bufs)
{
    u_char       *p;
    ngx_int_t     i;
    ngx_buf_t    *b;
    ngx_chain_t  *chain, *cl, **ll;

    p = ngx_palloc(pool, bufs->ಅಂಕೆ * bufs->size);
    ಯದಿ (p == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ll = &chain;

    ಪ್ರತಿ (i = 0; i < bufs->ಅಂಕೆ; i++) {

        b = ngx_calloc_buf(pool);
        ಯದಿ (b == ರಿಕ್ತ) {
            ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
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
        ಯದಿ (cl == ರಿಕ್ತ) {
            ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
        }

        cl->buf = b;
        *ll = cl;
        ll = &cl->next;
    }

    *ll = ರಿಕ್ತ;

    ಪ್ರತ್ಯಾಗಮ chain;
}


ngx_int_t
ngx_chain_add_copy(ngx_pool_t *pool, ngx_chain_t **chain, ngx_chain_t *in)
{
    ngx_chain_t  *cl, **ll;

    ll = chain;

    ಪ್ರತಿ (cl = *chain; cl; cl = cl->next) {
        ll = &cl->next;
    }

    ಯಾವತ್ (in) {
        cl = ngx_alloc_chain_link(pool);
        ಯದಿ (cl == ರಿಕ್ತ) {
            *ll = ರಿಕ್ತ;
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        cl->buf = in->buf;
        *ll = cl;
        ll = &cl->next;
        in = in->next;
    }

    *ll = ರಿಕ್ತ;

    ಪ್ರತ್ಯಾಗಮ NGX_OK;
}


ngx_chain_t *
ngx_chain_get_free_buf(ngx_pool_t *p, ngx_chain_t **ಮುಕ್ತ)
{
    ngx_chain_t  *cl;

    ಯದಿ (*ಮುಕ್ತ) {
        cl = *ಮುಕ್ತ;
        *ಮುಕ್ತ = cl->next;
        cl->next = ರಿಕ್ತ;
        ಪ್ರತ್ಯಾಗಮ cl;
    }

    cl = ngx_alloc_chain_link(p);
    ಯದಿ (cl == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    cl->buf = ngx_calloc_buf(p);
    ಯದಿ (cl->buf == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    cl->next = ರಿಕ್ತ;

    ಪ್ರತ್ಯಾಗಮ cl;
}


ಶೂನ್ಯ
ngx_chain_update_chains(ngx_pool_t *p, ngx_chain_t **ಮುಕ್ತ, ngx_chain_t **busy,
    ngx_chain_t **out, ngx_buf_tag_t tag)
{
    ngx_chain_t  *cl;

    ಯದಿ (*out) {
        ಯದಿ (*busy == ರಿಕ್ತ) {
            *busy = *out;

        } ಅನ್ಯಥಾ {
            ಪ್ರತಿ (cl = *busy; cl->next; cl = cl->next) { /* void */ }

            cl->next = *out;
        }

        *out = ರಿಕ್ತ;
    }

    ಯಾವತ್ (*busy) {
        cl = *busy;

        ಯದಿ (cl->buf->tag != tag) {
            *busy = cl->next;
            ngx_free_chain(p, cl);
            ಅನುವರ್ತ;
        }

        ಯದಿ (ngx_buf_size(cl->buf) != 0) {
            ಭಙ್ಗ;
        }

        cl->buf->pos = cl->buf->start;
        cl->buf->last = cl->buf->start;

        *busy = cl->next;
        cl->next = *ಮುಕ್ತ;
        *ಮುಕ್ತ = cl;
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

    ಕುರು {
        size = cl->buf->file_last - cl->buf->file_pos;

        ಯದಿ (size > limit - total) {
            size = limit - total;

            aligned = (cl->buf->file_pos + size + ngx_pagesize - 1)
                       & ~((off_t) ngx_pagesize - 1);

            ಯದಿ (aligned <= cl->buf->file_last) {
                size = aligned - cl->buf->file_pos;
            }

            total += size;
            ಭಙ್ಗ;
        }

        total += size;
        fprev = cl->buf->file_pos + size;
        cl = cl->next;

    } ಯಾವತ್ (cl
             && cl->buf->in_file
             && total < limit
             && fd == cl->buf->file->fd
             && fprev == cl->buf->file_pos);

    *in = cl;

    ಪ್ರತ್ಯಾಗಮ total;
}


ngx_chain_t *
ngx_chain_update_sent(ngx_chain_t *in, off_t sent)
{
    off_t  size;

    ಪ್ರತಿ ( /* void */ ; in; in = in->next) {

        ಯದಿ (ngx_buf_special(in->buf)) {
            ಅನುವರ್ತ;
        }

        ಯದಿ (sent == 0) {
            ಭಙ್ಗ;
        }

        size = ngx_buf_size(in->buf);

        ಯದಿ (sent >= size) {
            sent -= size;

            ಯದಿ (ngx_buf_in_memory(in->buf)) {
                in->buf->pos = in->buf->last;
            }

            ಯದಿ (in->buf->in_file) {
                in->buf->file_pos = in->buf->file_last;
            }

            ಅನುವರ್ತ;
        }

        ಯದಿ (ngx_buf_in_memory(in->buf)) {
            in->buf->pos += (size_t) sent;
        }

        ಯದಿ (in->buf->in_file) {
            in->buf->file_pos += sent;
        }

        ಭಙ್ಗ;
    }

    ಪ್ರತ್ಯಾಗಮ in;
}
