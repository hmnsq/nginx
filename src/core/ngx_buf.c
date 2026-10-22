
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
    もし (b == NULL) {
        戻る NULL;
    }

    b->start = ngx_palloc(pool, size);
    もし (b->start == NULL) {
        戻る NULL;
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

    戻る b;
}


ngx_chain_t *
ngx_alloc_chain_link(ngx_pool_t *pool)
{
    ngx_chain_t  *cl;

    cl = pool->chain;

    もし (cl) {
        pool->chain = cl->next;
        戻る cl;
    }

    cl = ngx_palloc(pool, sizeof(ngx_chain_t));
    もし (cl == NULL) {
        戻る NULL;
    }

    戻る cl;
}


ngx_chain_t *
ngx_create_chain_of_bufs(ngx_pool_t *pool, ngx_bufs_t *bufs)
{
    u_char       *p;
    ngx_int_t     i;
    ngx_buf_t    *b;
    ngx_chain_t  *chain, *cl, **ll;

    p = ngx_palloc(pool, bufs->num * bufs->size);
    もし (p == NULL) {
        戻る NULL;
    }

    ll = &chain;

    ために (i = 0; i < bufs->num; i++) {

        b = ngx_calloc_buf(pool);
        もし (b == NULL) {
            戻る NULL;
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
        もし (cl == NULL) {
            戻る NULL;
        }

        cl->buf = b;
        *ll = cl;
        ll = &cl->next;
    }

    *ll = NULL;

    戻る chain;
}


ngx_int_t
ngx_chain_add_copy(ngx_pool_t *pool, ngx_chain_t **chain, ngx_chain_t *in)
{
    ngx_chain_t  *cl, **ll;

    ll = chain;

    ために (cl = *chain; cl; cl = cl->next) {
        ll = &cl->next;
    }

    ながら (in) {
        cl = ngx_alloc_chain_link(pool);
        もし (cl == NULL) {
            *ll = NULL;
            戻る NGX_ERROR;
        }

        cl->buf = in->buf;
        *ll = cl;
        ll = &cl->next;
        in = in->next;
    }

    *ll = NULL;

    戻る NGX_OK;
}


ngx_chain_t *
ngx_chain_get_free_buf(ngx_pool_t *p, ngx_chain_t **free)
{
    ngx_chain_t  *cl;

    もし (*free) {
        cl = *free;
        *free = cl->next;
        cl->next = NULL;
        戻る cl;
    }

    cl = ngx_alloc_chain_link(p);
    もし (cl == NULL) {
        戻る NULL;
    }

    cl->buf = ngx_calloc_buf(p);
    もし (cl->buf == NULL) {
        戻る NULL;
    }

    cl->next = NULL;

    戻る cl;
}


void
ngx_chain_update_chains(ngx_pool_t *p, ngx_chain_t **free, ngx_chain_t **busy,
    ngx_chain_t **out, ngx_buf_tag_t tag)
{
    ngx_chain_t  *cl;

    もし (*out) {
        もし (*busy == NULL) {
            *busy = *out;

        } 否则 {
            ために (cl = *busy; cl->next; cl = cl->next) { /* void */ }

            cl->next = *out;
        }

        *out = NULL;
    }

    ながら (*busy) {
        cl = *busy;

        もし (cl->buf->tag != tag) {
            *busy = cl->next;
            ngx_free_chain(p, cl);
            continue;
        }

        もし (ngx_buf_size(cl->buf) != 0) {
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

        もし (size > limit - total) {
            size = limit - total;

            aligned = (cl->buf->file_pos + size + ngx_pagesize - 1)
                       & ~((off_t) ngx_pagesize - 1);

            もし (aligned <= cl->buf->file_last) {
                size = aligned - cl->buf->file_pos;
            }

            total += size;
            break;
        }

        total += size;
        fprev = cl->buf->file_pos + size;
        cl = cl->next;

    } ながら (cl
             && cl->buf->in_file
             && total < limit
             && fd == cl->buf->file->fd
             && fprev == cl->buf->file_pos);

    *in = cl;

    戻る total;
}


ngx_chain_t *
ngx_chain_update_sent(ngx_chain_t *in, off_t sent)
{
    off_t  size;

    ために ( /* void */ ; in; in = in->next) {

        もし (ngx_buf_special(in->buf)) {
            continue;
        }

        もし (sent == 0) {
            break;
        }

        size = ngx_buf_size(in->buf);

        もし (sent >= size) {
            sent -= size;

            もし (ngx_buf_in_memory(in->buf)) {
                in->buf->pos = in->buf->last;
            }

            もし (in->buf->in_file) {
                in->buf->file_pos = in->buf->file_last;
            }

            continue;
        }

        もし (ngx_buf_in_memory(in->buf)) {
            in->buf->pos += (size_t) sent;
        }

        もし (in->buf->in_file) {
            in->buf->file_pos += sent;
        }

        break;
    }

    戻る in;
}
