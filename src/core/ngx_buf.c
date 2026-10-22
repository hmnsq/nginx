
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>


ngx_buf_t *
ngx_create_temp_buf(ngx_pool_t *pool, size_t size)
{
    ngx_buf_t *b;

    b = ngx_calloc_buf(pool);
    ако (b == NULL) {
        върни NULL;
    }

    b->start = ngx_palloc(pool, size);
    ако (b->start == NULL) {
        върни NULL;
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

    върни b;
}


ngx_chain_t *
ngx_alloc_chain_link(ngx_pool_t *pool)
{
    ngx_chain_t  *cl;

    cl = pool->chain;

    ако (cl) {
        pool->chain = cl->next;
        върни cl;
    }

    cl = ngx_palloc(pool, sizeof(ngx_chain_t));
    ако (cl == NULL) {
        върни NULL;
    }

    върни cl;
}


ngx_chain_t *
ngx_create_chain_of_bufs(ngx_pool_t *pool, ngx_bufs_t *bufs)
{
    u_char       *p;
    ngx_int_t     i;
    ngx_buf_t    *b;
    ngx_chain_t  *chain, *cl, **ll;

    p = ngx_palloc(pool, bufs->num * bufs->size);
    ако (p == NULL) {
        върни NULL;
    }

    ll = &chain;

    за (i = 0; i < bufs->num; i++) {

        b = ngx_calloc_buf(pool);
        ако (b == NULL) {
            върни NULL;
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
        ако (cl == NULL) {
            върни NULL;
        }

        cl->buf = b;
        *ll = cl;
        ll = &cl->next;
    }

    *ll = NULL;

    върни chain;
}


ngx_int_t
ngx_chain_add_copy(ngx_pool_t *pool, ngx_chain_t **chain, ngx_chain_t *in)
{
    ngx_chain_t  *cl, **ll;

    ll = chain;

    за (cl = *chain; cl; cl = cl->next) {
        ll = &cl->next;
    }

    докато (in) {
        cl = ngx_alloc_chain_link(pool);
        ако (cl == NULL) {
            *ll = NULL;
            върни NGX_ERROR;
        }

        cl->buf = in->buf;
        *ll = cl;
        ll = &cl->next;
        in = in->next;
    }

    *ll = NULL;

    върни NGX_OK;
}


ngx_chain_t *
ngx_chain_get_free_buf(ngx_pool_t *p, ngx_chain_t **free)
{
    ngx_chain_t  *cl;

    ако (*free) {
        cl = *free;
        *free = cl->next;
        cl->next = NULL;
        върни cl;
    }

    cl = ngx_alloc_chain_link(p);
    ако (cl == NULL) {
        върни NULL;
    }

    cl->buf = ngx_calloc_buf(p);
    ако (cl->buf == NULL) {
        върни NULL;
    }

    cl->next = NULL;

    върни cl;
}


void
ngx_chain_update_chains(ngx_pool_t *p, ngx_chain_t **free, ngx_chain_t **busy,
    ngx_chain_t **out, ngx_buf_tag_t tag)
{
    ngx_chain_t  *cl;

    ако (*out) {
        ако (*busy == NULL) {
            *busy = *out;

        } иначе {
            за (cl = *busy; cl->next; cl = cl->next) { /* void */ }

            cl->next = *out;
        }

        *out = NULL;
    }

    докато (*busy) {
        cl = *busy;

        ако (cl->buf->tag != tag) {
            *busy = cl->next;
            ngx_free_chain(p, cl);
            continue;
        }

        ако (ngx_buf_size(cl->buf) != 0) {
            break;
        }

        cl->buf->pos = cl->buf->start;
        cl->buf->last = cl->buf->start;

        *busy = cl->next;
        cl->next = *free;
        *free = cl;
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

    do {
        size = cl->buf->file_last - cl->buf->file_pos;

        ако (size > limit - total) {
            size = limit - total;

            aligned = (cl->buf->file_pos + size + ngx_pagesize - 1)
                       & ~((off_t) ngx_pagesize - 1);

            ако (aligned <= cl->buf->file_last) {
                size = aligned - cl->buf->file_pos;
            }

            total += size;
            break;
        }

        total += size;
        fprev = cl->buf->file_pos + size;
        cl = cl->next;

    } докато (cl
             && cl->buf->in_file
             && total < limit
             && fd == cl->buf->file->fd
             && fprev == cl->buf->file_pos);

    *in = cl;

    върни total;
}


ngx_chain_t *
ngx_chain_update_sent(ngx_chain_t *in, off_t sent)
{
    off_t  size;

    за ( /* void */ ; in; in = in->next) {

        ако (ngx_buf_special(in->buf)) {
            continue;
        }

        ако (sent == 0) {
            break;
        }

        size = ngx_buf_size(in->buf);

        ако (sent >= size) {
            sent -= size;

            ако (ngx_buf_in_memory(in->buf)) {
                in->buf->pos = in->buf->last;
            }

            ако (in->buf->in_file) {
                in->buf->file_pos = in->buf->file_last;
            }

            continue;
        }

        ако (ngx_buf_in_memory(in->buf)) {
            in->buf->pos += (size_t) sent;
        }

        ако (in->buf->in_file) {
            in->buf->file_pos += sent;
        }

        break;
    }

    върни in;
}
