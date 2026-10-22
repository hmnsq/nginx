
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ଅନ୍ତର୍ଭାଵ <ngx_config.h>
#ଅନ୍ତର୍ଭାଵ <ngx_core.h>


ngx_buf_t *
ngx_create_temp_buf(ngx_pool_t *pool, size_t size)
{
    ngx_buf_t *b;

    b = ngx_calloc_buf(pool);
    ଯଦି (b == ଖାଲି) {
        ଫେରନ୍ତୁ ଖାଲି;
    }

    b->start = ngx_palloc(pool, size);
    ଯଦି (b->start == ଖାଲି) {
        ଫେରନ୍ତୁ ଖାଲି;
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

    ଫେରନ୍ତୁ b;
}


ngx_chain_t *
ngx_alloc_chain_link(ngx_pool_t *pool)
{
    ngx_chain_t  *cl;

    cl = pool->chain;

    ଯଦି (cl) {
        pool->chain = cl->next;
        ଫେରନ୍ତୁ cl;
    }

    cl = ngx_palloc(pool, ଆକାର(ngx_chain_t));
    ଯଦି (cl == ଖାଲି) {
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ଫେରନ୍ତୁ cl;
}


ngx_chain_t *
ngx_create_chain_of_bufs(ngx_pool_t *pool, ngx_bufs_t *bufs)
{
    u_char       *p;
    ngx_int_t     i;
    ngx_buf_t    *b;
    ngx_chain_t  *chain, *cl, **ll;

    p = ngx_palloc(pool, bufs->ଅଙ୍କ * bufs->size);
    ଯଦି (p == ଖାଲି) {
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ll = &chain;

    ପାଇଁ (i = 0; i < bufs->ଅଙ୍କ; i++) {

        b = ngx_calloc_buf(pool);
        ଯଦି (b == ଖାଲି) {
            ଫେରନ୍ତୁ ଖାଲି;
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
        ଯଦି (cl == ଖାଲି) {
            ଫେରନ୍ତୁ ଖାଲି;
        }

        cl->buf = b;
        *ll = cl;
        ll = &cl->next;
    }

    *ll = ଖାଲି;

    ଫେରନ୍ତୁ chain;
}


ngx_int_t
ngx_chain_add_copy(ngx_pool_t *pool, ngx_chain_t **chain, ngx_chain_t *in)
{
    ngx_chain_t  *cl, **ll;

    ll = chain;

    ପାଇଁ (cl = *chain; cl; cl = cl->next) {
        ll = &cl->next;
    }

    ଯେତେବେଳେ (in) {
        cl = ngx_alloc_chain_link(pool);
        ଯଦି (cl == ଖାଲି) {
            *ll = ଖାଲି;
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        cl->buf = in->buf;
        *ll = cl;
        ll = &cl->next;
        in = in->next;
    }

    *ll = ଖାଲି;

    ଫେରନ୍ତୁ NGX_OK;
}


ngx_chain_t *
ngx_chain_get_free_buf(ngx_pool_t *p, ngx_chain_t **ମୁକ୍ତ)
{
    ngx_chain_t  *cl;

    ଯଦି (*ମୁକ୍ତ) {
        cl = *ମୁକ୍ତ;
        *ମୁକ୍ତ = cl->next;
        cl->next = ଖାଲି;
        ଫେରନ୍ତୁ cl;
    }

    cl = ngx_alloc_chain_link(p);
    ଯଦି (cl == ଖାଲି) {
        ଫେରନ୍ତୁ ଖାଲି;
    }

    cl->buf = ngx_calloc_buf(p);
    ଯଦି (cl->buf == ଖାଲି) {
        ଫେରନ୍ତୁ ଖାଲି;
    }

    cl->next = ଖାଲି;

    ଫେରନ୍ତୁ cl;
}


ଶୂନ୍ୟ
ngx_chain_update_chains(ngx_pool_t *p, ngx_chain_t **ମୁକ୍ତ, ngx_chain_t **busy,
    ngx_chain_t **out, ngx_buf_tag_t tag)
{
    ngx_chain_t  *cl;

    ଯଦି (*out) {
        ଯଦି (*busy == ଖାଲି) {
            *busy = *out;

        } ଅନ୍ୟଥା {
            ପାଇଁ (cl = *busy; cl->next; cl = cl->next) { /* void */ }

            cl->next = *out;
        }

        *out = ଖାଲି;
    }

    ଯେତେବେଳେ (*busy) {
        cl = *busy;

        ଯଦି (cl->buf->tag != tag) {
            *busy = cl->next;
            ngx_free_chain(p, cl);
            ଜାରି;
        }

        ଯଦି (ngx_buf_size(cl->buf) != 0) {
            ଭାଙ୍ଗ;
        }

        cl->buf->pos = cl->buf->start;
        cl->buf->last = cl->buf->start;

        *busy = cl->next;
        cl->next = *ମୁକ୍ତ;
        *ମୁକ୍ତ = cl;
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

    କର {
        size = cl->buf->file_last - cl->buf->file_pos;

        ଯଦି (size > limit - total) {
            size = limit - total;

            aligned = (cl->buf->file_pos + size + ngx_pagesize - 1)
                       & ~((off_t) ngx_pagesize - 1);

            ଯଦି (aligned <= cl->buf->file_last) {
                size = aligned - cl->buf->file_pos;
            }

            total += size;
            ଭାଙ୍ଗ;
        }

        total += size;
        fprev = cl->buf->file_pos + size;
        cl = cl->next;

    } ଯେତେବେଳେ (cl
             && cl->buf->in_file
             && total < limit
             && fd == cl->buf->file->fd
             && fprev == cl->buf->file_pos);

    *in = cl;

    ଫେରନ୍ତୁ total;
}


ngx_chain_t *
ngx_chain_update_sent(ngx_chain_t *in, off_t sent)
{
    off_t  size;

    ପାଇଁ ( /* void */ ; in; in = in->next) {

        ଯଦି (ngx_buf_special(in->buf)) {
            ଜାରି;
        }

        ଯଦି (sent == 0) {
            ଭାଙ୍ଗ;
        }

        size = ngx_buf_size(in->buf);

        ଯଦି (sent >= size) {
            sent -= size;

            ଯଦି (ngx_buf_in_memory(in->buf)) {
                in->buf->pos = in->buf->last;
            }

            ଯଦି (in->buf->in_file) {
                in->buf->file_pos = in->buf->file_last;
            }

            ଜାରି;
        }

        ଯଦି (ngx_buf_in_memory(in->buf)) {
            in->buf->pos += (size_t) sent;
        }

        ଯଦି (in->buf->in_file) {
            in->buf->file_pos += sent;
        }

        ଭାଙ୍ଗ;
    }

    ଫେରନ୍ତୁ in;
}
