
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#समावेश <ngx_config.h>
#समावेश <ngx_core.h>


ngx_buf_t *
ngx_create_अस्थायी_buf(ngx_pool_t *pool, माप_प्रकार आकार)
{
    ngx_buf_t *b;

    b = ngx_calloc_buf(pool);
    अगर (b == शून्य) {
        वापस शून्य;
    }

    b->start = ngx_palloc(pool, आकार);
    अगर (b->start == शून्य) {
        वापस शून्य;
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
    b->end = b->last + आकार;
    b->temporary = 1;

    वापस b;
}


ngx_chain_t *
ngx_alloc_chain_link(ngx_pool_t *pool)
{
    ngx_chain_t  *cl;

    cl = pool->chain;

    अगर (cl) {
        pool->chain = cl->अग्र;
        वापस cl;
    }

    cl = ngx_palloc(pool, माप(ngx_chain_t));
    अगर (cl == शून्य) {
        वापस शून्य;
    }

    वापस cl;
}


ngx_chain_t *
ngx_create_chain_of_bufs(ngx_pool_t *pool, ngx_bufs_t *bufs)
{
    u_char       *p;
    ngx_int_t     i;
    ngx_buf_t    *b;
    ngx_chain_t  *chain, *cl, **ll;

    p = ngx_palloc(pool, bufs->अंक * bufs->आकार);
    अगर (p == शून्य) {
        वापस शून्य;
    }

    ll = &chain;

    क्रम (i = 0; i < bufs->अंक; i++) {

        b = ngx_calloc_buf(pool);
        अगर (b == शून्य) {
            वापस शून्य;
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
        p += bufs->आकार;
        b->end = p;

        cl = ngx_alloc_chain_link(pool);
        अगर (cl == शून्य) {
            वापस शून्य;
        }

        cl->buf = b;
        *ll = cl;
        ll = &cl->अग्र;
    }

    *ll = शून्य;

    वापस chain;
}


ngx_int_t
ngx_chain_add_copy(ngx_pool_t *pool, ngx_chain_t **chain, ngx_chain_t *in)
{
    ngx_chain_t  *cl, **ll;

    ll = chain;

    क्रम (cl = *chain; cl; cl = cl->अग्र) {
        ll = &cl->अग्र;
    }

    जबतक (in) {
        cl = ngx_alloc_chain_link(pool);
        अगर (cl == शून्य) {
            *ll = शून्य;
            वापस NGX_ERROR;
        }

        cl->buf = in->buf;
        *ll = cl;
        ll = &cl->अग्र;
        in = in->अग्र;
    }

    *ll = शून्य;

    वापस NGX_OK;
}


ngx_chain_t *
ngx_chain_get_free_buf(ngx_pool_t *p, ngx_chain_t **मुक्त)
{
    ngx_chain_t  *cl;

    अगर (*मुक्त) {
        cl = *मुक्त;
        *मुक्त = cl->अग्र;
        cl->अग्र = शून्य;
        वापस cl;
    }

    cl = ngx_alloc_chain_link(p);
    अगर (cl == शून्य) {
        वापस शून्य;
    }

    cl->buf = ngx_calloc_buf(p);
    अगर (cl->buf == शून्य) {
        वापस शून्य;
    }

    cl->अग्र = शून्य;

    वापस cl;
}


व्योम
ngx_chain_update_chains(ngx_pool_t *p, ngx_chain_t **मुक्त, ngx_chain_t **busy,
    ngx_chain_t **out, ngx_buf_tag_t tag)
{
    ngx_chain_t  *cl;

    अगर (*out) {
        अगर (*busy == शून्य) {
            *busy = *out;

        } अन्यथा {
            क्रम (cl = *busy; cl->अग्र; cl = cl->अग्र) { /* void */ }

            cl->अग्र = *out;
        }

        *out = शून्य;
    }

    जबतक (*busy) {
        cl = *busy;

        अगर (cl->buf->tag != tag) {
            *busy = cl->अग्र;
            ngx_free_chain(p, cl);
            जारी;
        }

        अगर (ngx_buf_आकार(cl->buf) != 0) {
            अवरोध;
        }

        cl->buf->pos = cl->buf->start;
        cl->buf->last = cl->buf->start;

        *busy = cl->अग्र;
        cl->अग्र = *मुक्त;
        *मुक्त = cl;
    }
}


off_t
ngx_chain_coalesce_file(ngx_chain_t **in, off_t limit)
{
    off_t         योग, आकार, aligned, fprev;
    ngx_fd_t      fd;
    ngx_chain_t  *cl;

    योग = 0;

    cl = *in;
    fd = cl->buf->file->fd;

    करो {
        आकार = cl->buf->file_last - cl->buf->file_pos;

        अगर (आकार > limit - योग) {
            आकार = limit - योग;

            aligned = (cl->buf->file_pos + आकार + ngx_pagesize - 1)
                       & ~((off_t) ngx_pagesize - 1);

            अगर (aligned <= cl->buf->file_last) {
                आकार = aligned - cl->buf->file_pos;
            }

            योग += आकार;
            अवरोध;
        }

        योग += आकार;
        fprev = cl->buf->file_pos + आकार;
        cl = cl->अग्र;

    } जबतक (cl
             && cl->buf->in_file
             && योग < limit
             && fd == cl->buf->file->fd
             && fprev == cl->buf->file_pos);

    *in = cl;

    वापस योग;
}


ngx_chain_t *
ngx_chain_update_sent(ngx_chain_t *in, off_t sent)
{
    off_t  आकार;

    क्रम ( /* void */ ; in; in = in->अग्र) {

        अगर (ngx_buf_special(in->buf)) {
            जारी;
        }

        अगर (sent == 0) {
            अवरोध;
        }

        आकार = ngx_buf_आकार(in->buf);

        अगर (sent >= आकार) {
            sent -= आकार;

            अगर (ngx_buf_in_memory(in->buf)) {
                in->buf->pos = in->buf->last;
            }

            अगर (in->buf->in_file) {
                in->buf->file_pos = in->buf->file_last;
            }

            जारी;
        }

        अगर (ngx_buf_in_memory(in->buf)) {
            in->buf->pos += (माप_प्रकार) sent;
        }

        अगर (in->buf->in_file) {
            in->buf->file_pos += sent;
        }

        अवरोध;
    }

    वापस in;
}
