
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
    если (b == NULL) {
        вернуть NULL;
    }

    b->start = ngx_palloc(pool, size);
    если (b->start == NULL) {
        вернуть NULL;
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

    вернуть b;
}


ngx_chain_t *
ngx_alloc_chain_link(ngx_pool_t *pool)
{
    ngx_chain_t  *cl;

    cl = pool->chain;

    если (cl) {
        pool->chain = cl->next;
        вернуть cl;
    }

    cl = ngx_palloc(pool, sizeof(ngx_chain_t));
    если (cl == NULL) {
        вернуть NULL;
    }

    вернуть cl;
}


ngx_chain_t *
ngx_create_chain_of_bufs(ngx_pool_t *pool, ngx_bufs_t *bufs)
{
    u_char       *p;
    ngx_int_t     i;
    ngx_buf_t    *b;
    ngx_chain_t  *chain, *cl, **ll;

    p = ngx_palloc(pool, bufs->num * bufs->size);
    если (p == NULL) {
        вернуть NULL;
    }

    ll = &chain;

    для (i = 0; i < bufs->num; i++) {

        b = ngx_calloc_buf(pool);
        если (b == NULL) {
            вернуть NULL;
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
        если (cl == NULL) {
            вернуть NULL;
        }

        cl->buf = b;
        *ll = cl;
        ll = &cl->next;
    }

    *ll = NULL;

    вернуть chain;
}


ngx_int_t
ngx_chain_add_copy(ngx_pool_t *pool, ngx_chain_t **chain, ngx_chain_t *in)
{
    ngx_chain_t  *cl, **ll;

    ll = chain;

    для (cl = *chain; cl; cl = cl->next) {
        ll = &cl->next;
    }

    пока (in) {
        cl = ngx_alloc_chain_link(pool);
        если (cl == NULL) {
            *ll = NULL;
            вернуть NGX_ERROR;
        }

        cl->buf = in->buf;
        *ll = cl;
        ll = &cl->next;
        in = in->next;
    }

    *ll = NULL;

    вернуть NGX_OK;
}


ngx_chain_t *
ngx_chain_get_free_buf(ngx_pool_t *p, ngx_chain_t **free)
{
    ngx_chain_t  *cl;

    если (*free) {
        cl = *free;
        *free = cl->next;
        cl->next = NULL;
        вернуть cl;
    }

    cl = ngx_alloc_chain_link(p);
    если (cl == NULL) {
        вернуть NULL;
    }

    cl->buf = ngx_calloc_buf(p);
    если (cl->buf == NULL) {
        вернуть NULL;
    }

    cl->next = NULL;

    вернуть cl;
}


void
ngx_chain_update_chains(ngx_pool_t *p, ngx_chain_t **free, ngx_chain_t **busy,
    ngx_chain_t **out, ngx_buf_tag_t tag)
{
    ngx_chain_t  *cl;

    если (*out) {
        если (*busy == NULL) {
            *busy = *out;

        } иначе {
            для (cl = *busy; cl->next; cl = cl->next) { /* void */ }

            cl->next = *out;
        }

        *out = NULL;
    }

    пока (*busy) {
        cl = *busy;

        если (cl->buf->tag != tag) {
            *busy = cl->next;
            ngx_free_chain(p, cl);
            continue;
        }

        если (ngx_buf_size(cl->buf) != 0) {
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

        если (size > limit - total) {
            size = limit - total;

            aligned = (cl->buf->file_pos + size + ngx_pagesize - 1)
                       & ~((off_t) ngx_pagesize - 1);

            если (aligned <= cl->buf->file_last) {
                size = aligned - cl->buf->file_pos;
            }

            total += size;
            break;
        }

        total += size;
        fprev = cl->buf->file_pos + size;
        cl = cl->next;

    } пока (cl
             && cl->buf->in_file
             && total < limit
             && fd == cl->buf->file->fd
             && fprev == cl->buf->file_pos);

    *in = cl;

    вернуть total;
}


ngx_chain_t *
ngx_chain_update_sent(ngx_chain_t *in, off_t sent)
{
    off_t  size;

    для ( /* void */ ; in; in = in->next) {

        если (ngx_buf_special(in->buf)) {
            continue;
        }

        если (sent == 0) {
            break;
        }

        size = ngx_buf_size(in->buf);

        если (sent >= size) {
            sent -= size;

            если (ngx_buf_in_memory(in->buf)) {
                in->buf->pos = in->buf->last;
            }

            если (in->buf->in_file) {
                in->buf->file_pos = in->buf->file_last;
            }

            continue;
        }

        если (ngx_buf_in_memory(in->buf)) {
            in->buf->pos += (size_t) sent;
        }

        если (in->buf->in_file) {
            in->buf->file_pos += sent;
        }

        break;
    }

    вернуть in;
}
