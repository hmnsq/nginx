
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


ngx_buf_t *
ngx_create_temp_buf(ngx_pool_t *pool, माप_प्रकार size)
{
    ngx_buf_t *b;

    b = ngx_calloc_buf(pool);
    यदि (b == रिक्त) {
        फर्काऊ रिक्त;
    }

    b->start = ngx_palloc(pool, size);
    यदि (b->start == रिक्त) {
        फर्काऊ रिक्त;
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

    फर्काऊ b;
}


ngx_chain_t *
ngx_alloc_chain_link(ngx_pool_t *pool)
{
    ngx_chain_t  *cl;

    cl = pool->chain;

    यदि (cl) {
        pool->chain = cl->next;
        फर्काऊ cl;
    }

    cl = ngx_palloc(pool, परिमाण(ngx_chain_t));
    यदि (cl == रिक्त) {
        फर्काऊ रिक्त;
    }

    फर्काऊ cl;
}


ngx_chain_t *
ngx_create_chain_of_bufs(ngx_pool_t *pool, ngx_bufs_t *bufs)
{
    u_char       *p;
    ngx_int_t     i;
    ngx_buf_t    *b;
    ngx_chain_t  *chain, *cl, **ll;

    p = ngx_palloc(pool, bufs->अङ्क * bufs->size);
    यदि (p == रिक्त) {
        फर्काऊ रिक्त;
    }

    ll = &chain;

    लागि (i = 0; i < bufs->अङ्क; i++) {

        b = ngx_calloc_buf(pool);
        यदि (b == रिक्त) {
            फर्काऊ रिक्त;
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
        यदि (cl == रिक्त) {
            फर्काऊ रिक्त;
        }

        cl->buf = b;
        *ll = cl;
        ll = &cl->next;
    }

    *ll = रिक्त;

    फर्काऊ chain;
}


ngx_int_t
ngx_chain_add_copy(ngx_pool_t *pool, ngx_chain_t **chain, ngx_chain_t *in)
{
    ngx_chain_t  *cl, **ll;

    ll = chain;

    लागि (cl = *chain; cl; cl = cl->next) {
        ll = &cl->next;
    }

    जबसम्म (in) {
        cl = ngx_alloc_chain_link(pool);
        यदि (cl == रिक्त) {
            *ll = रिक्त;
            फर्काऊ NGX_ERROR;
        }

        cl->buf = in->buf;
        *ll = cl;
        ll = &cl->next;
        in = in->next;
    }

    *ll = रिक्त;

    फर्काऊ NGX_OK;
}


ngx_chain_t *
ngx_chain_get_free_buf(ngx_pool_t *p, ngx_chain_t **मुक्त)
{
    ngx_chain_t  *cl;

    यदि (*मुक्त) {
        cl = *मुक्त;
        *मुक्त = cl->next;
        cl->next = रिक्त;
        फर्काऊ cl;
    }

    cl = ngx_alloc_chain_link(p);
    यदि (cl == रिक्त) {
        फर्काऊ रिक्त;
    }

    cl->buf = ngx_calloc_buf(p);
    यदि (cl->buf == रिक्त) {
        फर्काऊ रिक्त;
    }

    cl->next = रिक्त;

    फर्काऊ cl;
}


शून्य
ngx_chain_update_chains(ngx_pool_t *p, ngx_chain_t **मुक्त, ngx_chain_t **busy,
    ngx_chain_t **out, ngx_buf_tag_t tag)
{
    ngx_chain_t  *cl;

    यदि (*out) {
        यदि (*busy == रिक्त) {
            *busy = *out;

        } अन्यथा {
            लागि (cl = *busy; cl->next; cl = cl->next) { /* void */ }

            cl->next = *out;
        }

        *out = रिक्त;
    }

    जबसम्म (*busy) {
        cl = *busy;

        यदि (cl->buf->tag != tag) {
            *busy = cl->next;
            ngx_free_chain(p, cl);
            जारी;
        }

        यदि (ngx_buf_size(cl->buf) != 0) {
            रोक;
        }

        cl->buf->pos = cl->buf->start;
        cl->buf->last = cl->buf->start;

        *busy = cl->next;
        cl->next = *मुक्त;
        *मुक्त = cl;
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

    गर {
        size = cl->buf->file_last - cl->buf->file_pos;

        यदि (size > limit - total) {
            size = limit - total;

            aligned = (cl->buf->file_pos + size + ngx_pagesize - 1)
                       & ~((off_t) ngx_pagesize - 1);

            यदि (aligned <= cl->buf->file_last) {
                size = aligned - cl->buf->file_pos;
            }

            total += size;
            रोक;
        }

        total += size;
        fprev = cl->buf->file_pos + size;
        cl = cl->next;

    } जबसम्म (cl
             && cl->buf->in_file
             && total < limit
             && fd == cl->buf->file->fd
             && fprev == cl->buf->file_pos);

    *in = cl;

    फर्काऊ total;
}


ngx_chain_t *
ngx_chain_update_sent(ngx_chain_t *in, off_t sent)
{
    off_t  size;

    लागि ( /* void */ ; in; in = in->next) {

        यदि (ngx_buf_special(in->buf)) {
            जारी;
        }

        यदि (sent == 0) {
            रोक;
        }

        size = ngx_buf_size(in->buf);

        यदि (sent >= size) {
            sent -= size;

            यदि (ngx_buf_in_memory(in->buf)) {
                in->buf->pos = in->buf->last;
            }

            यदि (in->buf->in_file) {
                in->buf->file_pos = in->buf->file_last;
            }

            जारी;
        }

        यदि (ngx_buf_in_memory(in->buf)) {
            in->buf->pos += (माप_प्रकार) sent;
        }

        यदि (in->buf->in_file) {
            in->buf->file_pos += sent;
        }

        रोक;
    }

    फर्काऊ in;
}
