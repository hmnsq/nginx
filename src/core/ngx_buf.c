
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#અન્તર્ભાવ <ngx_config.h>
#અન્તર્ભાવ <ngx_core.h>


ngx_buf_t *
ngx_create_temp_buf(ngx_pool_t *pool, size_t size)
{
    ngx_buf_t *b;

    b = ngx_calloc_buf(pool);
    જો (b == ખાલી) {
        પાછા ખાલી;
    }

    b->start = ngx_palloc(pool, size);
    જો (b->start == ખાલી) {
        પાછા ખાલી;
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

    પાછા b;
}


ngx_chain_t *
ngx_alloc_chain_link(ngx_pool_t *pool)
{
    ngx_chain_t  *cl;

    cl = pool->chain;

    જો (cl) {
        pool->chain = cl->next;
        પાછા cl;
    }

    cl = ngx_palloc(pool, કદ(ngx_chain_t));
    જો (cl == ખાલી) {
        પાછા ખાલી;
    }

    પાછા cl;
}


ngx_chain_t *
ngx_create_chain_of_bufs(ngx_pool_t *pool, ngx_bufs_t *bufs)
{
    u_char       *p;
    ngx_int_t     i;
    ngx_buf_t    *b;
    ngx_chain_t  *chain, *cl, **ll;

    p = ngx_palloc(pool, bufs->અંક * bufs->size);
    જો (p == ખાલી) {
        પાછા ખાલી;
    }

    ll = &chain;

    માટે (i = 0; i < bufs->અંક; i++) {

        b = ngx_calloc_buf(pool);
        જો (b == ખાલી) {
            પાછા ખાલી;
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
        જો (cl == ખાલી) {
            પાછા ખાલી;
        }

        cl->buf = b;
        *ll = cl;
        ll = &cl->next;
    }

    *ll = ખાલી;

    પાછા chain;
}


ngx_int_t
ngx_chain_add_copy(ngx_pool_t *pool, ngx_chain_t **chain, ngx_chain_t *in)
{
    ngx_chain_t  *cl, **ll;

    ll = chain;

    માટે (cl = *chain; cl; cl = cl->next) {
        ll = &cl->next;
    }

    જ્યારે (in) {
        cl = ngx_alloc_chain_link(pool);
        જો (cl == ખાલી) {
            *ll = ખાલી;
            પાછા NGX_ERROR;
        }

        cl->buf = in->buf;
        *ll = cl;
        ll = &cl->next;
        in = in->next;
    }

    *ll = ખાલી;

    પાછા NGX_OK;
}


ngx_chain_t *
ngx_chain_get_free_buf(ngx_pool_t *p, ngx_chain_t **મુક્ત)
{
    ngx_chain_t  *cl;

    જો (*મુક્ત) {
        cl = *મુક્ત;
        *મુક્ત = cl->next;
        cl->next = ખાલી;
        પાછા cl;
    }

    cl = ngx_alloc_chain_link(p);
    જો (cl == ખાલી) {
        પાછા ખાલી;
    }

    cl->buf = ngx_calloc_buf(p);
    જો (cl->buf == ખાલી) {
        પાછા ખાલી;
    }

    cl->next = ખાલી;

    પાછા cl;
}


શૂન્ય
ngx_chain_update_chains(ngx_pool_t *p, ngx_chain_t **મુક્ત, ngx_chain_t **busy,
    ngx_chain_t **out, ngx_buf_tag_t tag)
{
    ngx_chain_t  *cl;

    જો (*out) {
        જો (*busy == ખાલી) {
            *busy = *out;

        } નહીંતર {
            માટે (cl = *busy; cl->next; cl = cl->next) { /* void */ }

            cl->next = *out;
        }

        *out = ખાલી;
    }

    જ્યારે (*busy) {
        cl = *busy;

        જો (cl->buf->tag != tag) {
            *busy = cl->next;
            ngx_free_chain(p, cl);
            ચાલુ;
        }

        જો (ngx_buf_size(cl->buf) != 0) {
            તોડો;
        }

        cl->buf->pos = cl->buf->start;
        cl->buf->last = cl->buf->start;

        *busy = cl->next;
        cl->next = *મુક્ત;
        *મુક્ત = cl;
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

    કરો {
        size = cl->buf->file_last - cl->buf->file_pos;

        જો (size > limit - total) {
            size = limit - total;

            aligned = (cl->buf->file_pos + size + ngx_pagesize - 1)
                       & ~((off_t) ngx_pagesize - 1);

            જો (aligned <= cl->buf->file_last) {
                size = aligned - cl->buf->file_pos;
            }

            total += size;
            તોડો;
        }

        total += size;
        fprev = cl->buf->file_pos + size;
        cl = cl->next;

    } જ્યારે (cl
             && cl->buf->in_file
             && total < limit
             && fd == cl->buf->file->fd
             && fprev == cl->buf->file_pos);

    *in = cl;

    પાછા total;
}


ngx_chain_t *
ngx_chain_update_sent(ngx_chain_t *in, off_t sent)
{
    off_t  size;

    માટે ( /* void */ ; in; in = in->next) {

        જો (ngx_buf_special(in->buf)) {
            ચાલુ;
        }

        જો (sent == 0) {
            તોડો;
        }

        size = ngx_buf_size(in->buf);

        જો (sent >= size) {
            sent -= size;

            જો (ngx_buf_in_memory(in->buf)) {
                in->buf->pos = in->buf->last;
            }

            જો (in->buf->in_file) {
                in->buf->file_pos = in->buf->file_last;
            }

            ચાલુ;
        }

        જો (ngx_buf_in_memory(in->buf)) {
            in->buf->pos += (size_t) sent;
        }

        જો (in->buf->in_file) {
            in->buf->file_pos += sent;
        }

        તોડો;
    }

    પાછા in;
}
