
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#അന്തര്ഭാവ <ngx_config.h>
#അന്തര്ഭാവ <ngx_core.h>


ngx_buf_t *
ngx_create_temp_buf(ngx_pool_t *pool, size_t size)
{
    ngx_buf_t *b;

    b = ngx_calloc_buf(pool);
    എങ്കിൽ (b == ശൂന്യ) {
        തിരികെ ശൂന്യ;
    }

    b->start = ngx_palloc(pool, size);
    എങ്കിൽ (b->start == ശൂന്യ) {
        തിരികെ ശൂന്യ;
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

    തിരികെ b;
}


ngx_chain_t *
ngx_alloc_chain_link(ngx_pool_t *pool)
{
    ngx_chain_t  *cl;

    cl = pool->chain;

    എങ്കിൽ (cl) {
        pool->chain = cl->next;
        തിരികെ cl;
    }

    cl = ngx_palloc(pool, വലുപ്പം(ngx_chain_t));
    എങ്കിൽ (cl == ശൂന്യ) {
        തിരികെ ശൂന്യ;
    }

    തിരികെ cl;
}


ngx_chain_t *
ngx_create_chain_of_bufs(ngx_pool_t *pool, ngx_bufs_t *bufs)
{
    u_char       *p;
    ngx_int_t     i;
    ngx_buf_t    *b;
    ngx_chain_t  *chain, *cl, **ll;

    p = ngx_palloc(pool, bufs->അക്കം * bufs->size);
    എങ്കിൽ (p == ശൂന്യ) {
        തിരികെ ശൂന്യ;
    }

    ll = &chain;

    വേണ്ടി (i = 0; i < bufs->അക്കം; i++) {

        b = ngx_calloc_buf(pool);
        എങ്കിൽ (b == ശൂന്യ) {
            തിരികെ ശൂന്യ;
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
        എങ്കിൽ (cl == ശൂന്യ) {
            തിരികെ ശൂന്യ;
        }

        cl->buf = b;
        *ll = cl;
        ll = &cl->next;
    }

    *ll = ശൂന്യ;

    തിരികെ chain;
}


ngx_int_t
ngx_chain_add_copy(ngx_pool_t *pool, ngx_chain_t **chain, ngx_chain_t *in)
{
    ngx_chain_t  *cl, **ll;

    ll = chain;

    വേണ്ടി (cl = *chain; cl; cl = cl->next) {
        ll = &cl->next;
    }

    ആകുമ്പോൾ (in) {
        cl = ngx_alloc_chain_link(pool);
        എങ്കിൽ (cl == ശൂന്യ) {
            *ll = ശൂന്യ;
            തിരികെ NGX_ERROR;
        }

        cl->buf = in->buf;
        *ll = cl;
        ll = &cl->next;
        in = in->next;
    }

    *ll = ശൂന്യ;

    തിരികെ NGX_OK;
}


ngx_chain_t *
ngx_chain_get_free_buf(ngx_pool_t *p, ngx_chain_t **സ്വതന്ത്രം)
{
    ngx_chain_t  *cl;

    എങ്കിൽ (*സ്വതന്ത്രം) {
        cl = *സ്വതന്ത്രം;
        *സ്വതന്ത്രം = cl->next;
        cl->next = ശൂന്യ;
        തിരികെ cl;
    }

    cl = ngx_alloc_chain_link(p);
    എങ്കിൽ (cl == ശൂന്യ) {
        തിരികെ ശൂന്യ;
    }

    cl->buf = ngx_calloc_buf(p);
    എങ്കിൽ (cl->buf == ശൂന്യ) {
        തിരികെ ശൂന്യ;
    }

    cl->next = ശൂന്യ;

    തിരികെ cl;
}


ശൂന്യം
ngx_chain_update_chains(ngx_pool_t *p, ngx_chain_t **സ്വതന്ത്രം, ngx_chain_t **busy,
    ngx_chain_t **out, ngx_buf_tag_t tag)
{
    ngx_chain_t  *cl;

    എങ്കിൽ (*out) {
        എങ്കിൽ (*busy == ശൂന്യ) {
            *busy = *out;

        } അല്ലെങ്കിൽ {
            വേണ്ടി (cl = *busy; cl->next; cl = cl->next) { /* void */ }

            cl->next = *out;
        }

        *out = ശൂന്യ;
    }

    ആകുമ്പോൾ (*busy) {
        cl = *busy;

        എങ്കിൽ (cl->buf->tag != tag) {
            *busy = cl->next;
            ngx_free_chain(p, cl);
            തുടരുക;
        }

        എങ്കിൽ (ngx_buf_size(cl->buf) != 0) {
            നിർത്തുക;
        }

        cl->buf->pos = cl->buf->start;
        cl->buf->last = cl->buf->start;

        *busy = cl->next;
        cl->next = *സ്വതന്ത്രം;
        *സ്വതന്ത്രം = cl;
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

    ചെയ്യുക {
        size = cl->buf->file_last - cl->buf->file_pos;

        എങ്കിൽ (size > limit - total) {
            size = limit - total;

            aligned = (cl->buf->file_pos + size + ngx_pagesize - 1)
                       & ~((off_t) ngx_pagesize - 1);

            എങ്കിൽ (aligned <= cl->buf->file_last) {
                size = aligned - cl->buf->file_pos;
            }

            total += size;
            നിർത്തുക;
        }

        total += size;
        fprev = cl->buf->file_pos + size;
        cl = cl->next;

    } ആകുമ്പോൾ (cl
             && cl->buf->in_file
             && total < limit
             && fd == cl->buf->file->fd
             && fprev == cl->buf->file_pos);

    *in = cl;

    തിരികെ total;
}


ngx_chain_t *
ngx_chain_update_sent(ngx_chain_t *in, off_t sent)
{
    off_t  size;

    വേണ്ടി ( /* void */ ; in; in = in->next) {

        എങ്കിൽ (ngx_buf_special(in->buf)) {
            തുടരുക;
        }

        എങ്കിൽ (sent == 0) {
            നിർത്തുക;
        }

        size = ngx_buf_size(in->buf);

        എങ്കിൽ (sent >= size) {
            sent -= size;

            എങ്കിൽ (ngx_buf_in_memory(in->buf)) {
                in->buf->pos = in->buf->last;
            }

            എങ്കിൽ (in->buf->in_file) {
                in->buf->file_pos = in->buf->file_last;
            }

            തുടരുക;
        }

        എങ്കിൽ (ngx_buf_in_memory(in->buf)) {
            in->buf->pos += (size_t) sent;
        }

        എങ്കിൽ (in->buf->in_file) {
            in->buf->file_pos += sent;
        }

        നിർത്തുക;
    }

    തിരികെ in;
}
