
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#شامل <ngx_config.h>
#شامل <ngx_core.h>


ngx_buf_t *
ngx_create_temp_buf(ngx_pool_t *pool, size_t size)
{
    ngx_buf_t *b;

    b = ngx_calloc_buf(pool);
    اگر (b == پوچ) {
        بازگشت پوچ;
    }

    b->start = ngx_palloc(pool, size);
    اگر (b->start == پوچ) {
        بازگشت پوچ;
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

    بازگشت b;
}


ngx_chain_t *
ngx_alloc_chain_link(ngx_pool_t *pool)
{
    ngx_chain_t  *cl;

    cl = pool->chain;

    اگر (cl) {
        pool->chain = cl->next;
        بازگشت cl;
    }

    cl = ngx_palloc(pool, اندازه(ngx_chain_t));
    اگر (cl == پوچ) {
        بازگشت پوچ;
    }

    بازگشت cl;
}


ngx_chain_t *
ngx_create_chain_of_bufs(ngx_pool_t *pool, ngx_bufs_t *bufs)
{
    u_char       *p;
    ngx_int_t     i;
    ngx_buf_t    *b;
    ngx_chain_t  *chain, *cl, **ll;

    p = ngx_palloc(pool, bufs->رقم * bufs->size);
    اگر (p == پوچ) {
        بازگشت پوچ;
    }

    ll = &chain;

    برای (i = 0; i < bufs->رقم; i++) {

        b = ngx_calloc_buf(pool);
        اگر (b == پوچ) {
            بازگشت پوچ;
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
        اگر (cl == پوچ) {
            بازگشت پوچ;
        }

        cl->buf = b;
        *ll = cl;
        ll = &cl->next;
    }

    *ll = پوچ;

    بازگشت chain;
}


ngx_int_t
ngx_chain_add_copy(ngx_pool_t *pool, ngx_chain_t **chain, ngx_chain_t *in)
{
    ngx_chain_t  *cl, **ll;

    ll = chain;

    برای (cl = *chain; cl; cl = cl->next) {
        ll = &cl->next;
    }

    وقتی (in) {
        cl = ngx_alloc_chain_link(pool);
        اگر (cl == پوچ) {
            *ll = پوچ;
            بازگشت NGX_ERROR;
        }

        cl->buf = in->buf;
        *ll = cl;
        ll = &cl->next;
        in = in->next;
    }

    *ll = پوچ;

    بازگشت NGX_OK;
}


ngx_chain_t *
ngx_chain_get_free_buf(ngx_pool_t *p, ngx_chain_t **آزاد)
{
    ngx_chain_t  *cl;

    اگر (*آزاد) {
        cl = *آزاد;
        *آزاد = cl->next;
        cl->next = پوچ;
        بازگشت cl;
    }

    cl = ngx_alloc_chain_link(p);
    اگر (cl == پوچ) {
        بازگشت پوچ;
    }

    cl->buf = ngx_calloc_buf(p);
    اگر (cl->buf == پوچ) {
        بازگشت پوچ;
    }

    cl->next = پوچ;

    بازگشت cl;
}


تهی
ngx_chain_update_chains(ngx_pool_t *p, ngx_chain_t **آزاد, ngx_chain_t **busy,
    ngx_chain_t **out, ngx_buf_tag_t tag)
{
    ngx_chain_t  *cl;

    اگر (*out) {
        اگر (*busy == پوچ) {
            *busy = *out;

        } وگرنه {
            برای (cl = *busy; cl->next; cl = cl->next) { /* void */ }

            cl->next = *out;
        }

        *out = پوچ;
    }

    وقتی (*busy) {
        cl = *busy;

        اگر (cl->buf->tag != tag) {
            *busy = cl->next;
            ngx_free_chain(p, cl);
            ادامه;
        }

        اگر (ngx_buf_size(cl->buf) != 0) {
            بشکن;
        }

        cl->buf->pos = cl->buf->start;
        cl->buf->last = cl->buf->start;

        *busy = cl->next;
        cl->next = *آزاد;
        *آزاد = cl;
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

    انجامبده {
        size = cl->buf->file_last - cl->buf->file_pos;

        اگر (size > limit - total) {
            size = limit - total;

            aligned = (cl->buf->file_pos + size + ngx_pagesize - 1)
                       & ~((off_t) ngx_pagesize - 1);

            اگر (aligned <= cl->buf->file_last) {
                size = aligned - cl->buf->file_pos;
            }

            total += size;
            بشکن;
        }

        total += size;
        fprev = cl->buf->file_pos + size;
        cl = cl->next;

    } وقتی (cl
             && cl->buf->in_file
             && total < limit
             && fd == cl->buf->file->fd
             && fprev == cl->buf->file_pos);

    *in = cl;

    بازگشت total;
}


ngx_chain_t *
ngx_chain_update_sent(ngx_chain_t *in, off_t sent)
{
    off_t  size;

    برای ( /* void */ ; in; in = in->next) {

        اگر (ngx_buf_special(in->buf)) {
            ادامه;
        }

        اگر (sent == 0) {
            بشکن;
        }

        size = ngx_buf_size(in->buf);

        اگر (sent >= size) {
            sent -= size;

            اگر (ngx_buf_in_memory(in->buf)) {
                in->buf->pos = in->buf->last;
            }

            اگر (in->buf->in_file) {
                in->buf->file_pos = in->buf->file_last;
            }

            ادامه;
        }

        اگر (ngx_buf_in_memory(in->buf)) {
            in->buf->pos += (size_t) sent;
        }

        اگر (in->buf->in_file) {
            in->buf->file_pos += sent;
        }

        بشکن;
    }

    بازگشت in;
}
