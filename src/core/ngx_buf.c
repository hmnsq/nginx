
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
    如果 (b == NULL) {
        返回 NULL;
    }

    b->start = ngx_palloc(pool, size);
    如果 (b->start == NULL) {
        返回 NULL;
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

    返回 b;
}


ngx_chain_t *
ngx_alloc_chain_link(ngx_pool_t *pool)
{
    ngx_chain_t  *cl;

    cl = pool->chain;

    如果 (cl) {
        pool->chain = cl->next;
        返回 cl;
    }

    cl = ngx_palloc(pool, sizeof(ngx_chain_t));
    如果 (cl == NULL) {
        返回 NULL;
    }

    返回 cl;
}


ngx_chain_t *
ngx_create_chain_of_bufs(ngx_pool_t *pool, ngx_bufs_t *bufs)
{
    u_char       *p;
    ngx_int_t     i;
    ngx_buf_t    *b;
    ngx_chain_t  *chain, *cl, **ll;

    p = ngx_palloc(pool, bufs->num * bufs->size);
    如果 (p == NULL) {
        返回 NULL;
    }

    ll = &chain;

    为 (i = 0; i < bufs->num; i++) {

        b = ngx_calloc_buf(pool);
        如果 (b == NULL) {
            返回 NULL;
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
        如果 (cl == NULL) {
            返回 NULL;
        }

        cl->buf = b;
        *ll = cl;
        ll = &cl->next;
    }

    *ll = NULL;

    返回 chain;
}


ngx_int_t
ngx_chain_add_copy(ngx_pool_t *pool, ngx_chain_t **chain, ngx_chain_t *in)
{
    ngx_chain_t  *cl, **ll;

    ll = chain;

    为 (cl = *chain; cl; cl = cl->next) {
        ll = &cl->next;
    }

    当 (in) {
        cl = ngx_alloc_chain_link(pool);
        如果 (cl == NULL) {
            *ll = NULL;
            返回 NGX_ERROR;
        }

        cl->buf = in->buf;
        *ll = cl;
        ll = &cl->next;
        in = in->next;
    }

    *ll = NULL;

    返回 NGX_OK;
}


ngx_chain_t *
ngx_chain_get_free_buf(ngx_pool_t *p, ngx_chain_t **free)
{
    ngx_chain_t  *cl;

    如果 (*free) {
        cl = *free;
        *free = cl->next;
        cl->next = NULL;
        返回 cl;
    }

    cl = ngx_alloc_chain_link(p);
    如果 (cl == NULL) {
        返回 NULL;
    }

    cl->buf = ngx_calloc_buf(p);
    如果 (cl->buf == NULL) {
        返回 NULL;
    }

    cl->next = NULL;

    返回 cl;
}


void
ngx_chain_update_chains(ngx_pool_t *p, ngx_chain_t **free, ngx_chain_t **busy,
    ngx_chain_t **out, ngx_buf_tag_t tag)
{
    ngx_chain_t  *cl;

    如果 (*out) {
        如果 (*busy == NULL) {
            *busy = *out;

        } 否则 {
            为 (cl = *busy; cl->next; cl = cl->next) { /* void */ }

            cl->next = *out;
        }

        *out = NULL;
    }

    当 (*busy) {
        cl = *busy;

        如果 (cl->buf->tag != tag) {
            *busy = cl->next;
            ngx_free_chain(p, cl);
            continue;
        }

        如果 (ngx_buf_size(cl->buf) != 0) {
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

        如果 (size > limit - total) {
            size = limit - total;

            aligned = (cl->buf->file_pos + size + ngx_pagesize - 1)
                       & ~((off_t) ngx_pagesize - 1);

            如果 (aligned <= cl->buf->file_last) {
                size = aligned - cl->buf->file_pos;
            }

            total += size;
            break;
        }

        total += size;
        fprev = cl->buf->file_pos + size;
        cl = cl->next;

    } 当 (cl
             && cl->buf->in_file
             && total < limit
             && fd == cl->buf->file->fd
             && fprev == cl->buf->file_pos);

    *in = cl;

    返回 total;
}


ngx_chain_t *
ngx_chain_update_sent(ngx_chain_t *in, off_t sent)
{
    off_t  size;

    为 ( /* void */ ; in; in = in->next) {

        如果 (ngx_buf_special(in->buf)) {
            continue;
        }

        如果 (sent == 0) {
            break;
        }

        size = ngx_buf_size(in->buf);

        如果 (sent >= size) {
            sent -= size;

            如果 (ngx_buf_in_memory(in->buf)) {
                in->buf->pos = in->buf->last;
            }

            如果 (in->buf->in_file) {
                in->buf->file_pos = in->buf->file_last;
            }

            continue;
        }

        如果 (ngx_buf_in_memory(in->buf)) {
            in->buf->pos += (size_t) sent;
        }

        如果 (in->buf->in_file) {
            in->buf->file_pos += sent;
        }

        break;
    }

    返回 in;
}
