
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#அந்தர்பாவ <ngx_config.h>
#அந்தர்பாவ <ngx_core.h>


ngx_buf_t *
ngx_create_temp_buf(ngx_pool_t *pool, size_t size)
{
    ngx_buf_t *b;

    b = ngx_calloc_buf(pool);
    எனில் (b == வெற்று) {
        திரும்பு வெற்று;
    }

    b->start = ngx_palloc(pool, size);
    எனில் (b->start == வெற்று) {
        திரும்பு வெற்று;
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

    திரும்பு b;
}


ngx_chain_t *
ngx_alloc_chain_link(ngx_pool_t *pool)
{
    ngx_chain_t  *cl;

    cl = pool->chain;

    எனில் (cl) {
        pool->chain = cl->next;
        திரும்பு cl;
    }

    cl = ngx_palloc(pool, அளவு(ngx_chain_t));
    எனில் (cl == வெற்று) {
        திரும்பு வெற்று;
    }

    திரும்பு cl;
}


ngx_chain_t *
ngx_create_chain_of_bufs(ngx_pool_t *pool, ngx_bufs_t *bufs)
{
    u_char       *p;
    ngx_int_t     i;
    ngx_buf_t    *b;
    ngx_chain_t  *chain, *cl, **ll;

    p = ngx_palloc(pool, bufs->இலக்கம் * bufs->size);
    எனில் (p == வெற்று) {
        திரும்பு வெற்று;
    }

    ll = &chain;

    க்கு (i = 0; i < bufs->இலக்கம்; i++) {

        b = ngx_calloc_buf(pool);
        எனில் (b == வெற்று) {
            திரும்பு வெற்று;
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
        எனில் (cl == வெற்று) {
            திரும்பு வெற்று;
        }

        cl->buf = b;
        *ll = cl;
        ll = &cl->next;
    }

    *ll = வெற்று;

    திரும்பு chain;
}


ngx_int_t
ngx_chain_add_copy(ngx_pool_t *pool, ngx_chain_t **chain, ngx_chain_t *in)
{
    ngx_chain_t  *cl, **ll;

    ll = chain;

    க்கு (cl = *chain; cl; cl = cl->next) {
        ll = &cl->next;
    }

    போது (in) {
        cl = ngx_alloc_chain_link(pool);
        எனில் (cl == வெற்று) {
            *ll = வெற்று;
            திரும்பு NGX_ERROR;
        }

        cl->buf = in->buf;
        *ll = cl;
        ll = &cl->next;
        in = in->next;
    }

    *ll = வெற்று;

    திரும்பு NGX_OK;
}


ngx_chain_t *
ngx_chain_get_free_buf(ngx_pool_t *p, ngx_chain_t **விடுவி)
{
    ngx_chain_t  *cl;

    எனில் (*விடுவி) {
        cl = *விடுவி;
        *விடுவி = cl->next;
        cl->next = வெற்று;
        திரும்பு cl;
    }

    cl = ngx_alloc_chain_link(p);
    எனில் (cl == வெற்று) {
        திரும்பு வெற்று;
    }

    cl->buf = ngx_calloc_buf(p);
    எனில் (cl->buf == வெற்று) {
        திரும்பு வெற்று;
    }

    cl->next = வெற்று;

    திரும்பு cl;
}


வெறுமை
ngx_chain_update_chains(ngx_pool_t *p, ngx_chain_t **விடுவி, ngx_chain_t **busy,
    ngx_chain_t **out, ngx_buf_tag_t tag)
{
    ngx_chain_t  *cl;

    எனில் (*out) {
        எனில் (*busy == வெற்று) {
            *busy = *out;

        } இல்லையெனில் {
            க்கு (cl = *busy; cl->next; cl = cl->next) { /* void */ }

            cl->next = *out;
        }

        *out = வெற்று;
    }

    போது (*busy) {
        cl = *busy;

        எனில் (cl->buf->tag != tag) {
            *busy = cl->next;
            ngx_free_chain(p, cl);
            தொடர்;
        }

        எனில் (ngx_buf_size(cl->buf) != 0) {
            நிறுத்து;
        }

        cl->buf->pos = cl->buf->start;
        cl->buf->last = cl->buf->start;

        *busy = cl->next;
        cl->next = *விடுவி;
        *விடுவி = cl;
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

    செய் {
        size = cl->buf->file_last - cl->buf->file_pos;

        எனில் (size > limit - total) {
            size = limit - total;

            aligned = (cl->buf->file_pos + size + ngx_pagesize - 1)
                       & ~((off_t) ngx_pagesize - 1);

            எனில் (aligned <= cl->buf->file_last) {
                size = aligned - cl->buf->file_pos;
            }

            total += size;
            நிறுத்து;
        }

        total += size;
        fprev = cl->buf->file_pos + size;
        cl = cl->next;

    } போது (cl
             && cl->buf->in_file
             && total < limit
             && fd == cl->buf->file->fd
             && fprev == cl->buf->file_pos);

    *in = cl;

    திரும்பு total;
}


ngx_chain_t *
ngx_chain_update_sent(ngx_chain_t *in, off_t sent)
{
    off_t  size;

    க்கு ( /* void */ ; in; in = in->next) {

        எனில் (ngx_buf_special(in->buf)) {
            தொடர்;
        }

        எனில் (sent == 0) {
            நிறுத்து;
        }

        size = ngx_buf_size(in->buf);

        எனில் (sent >= size) {
            sent -= size;

            எனில் (ngx_buf_in_memory(in->buf)) {
                in->buf->pos = in->buf->last;
            }

            எனில் (in->buf->in_file) {
                in->buf->file_pos = in->buf->file_last;
            }

            தொடர்;
        }

        எனில் (ngx_buf_in_memory(in->buf)) {
            in->buf->pos += (size_t) sent;
        }

        எனில் (in->buf->in_file) {
            in->buf->file_pos += sent;
        }

        நிறுத்து;
    }

    திரும்பு in;
}
