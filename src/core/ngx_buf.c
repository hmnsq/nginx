
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
    ប្រសិនបើ (b == NULL) {
        ត្រឡប់ NULL;
    }

    b->start = ngx_palloc(pool, size);
    ប្រសិនបើ (b->start == NULL) {
        ត្រឡប់ NULL;
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

    ត្រឡប់ b;
}


ngx_chain_t *
ngx_alloc_chain_link(ngx_pool_t *pool)
{
    ngx_chain_t  *cl;

    cl = pool->chain;

    ប្រសិនបើ (cl) {
        pool->chain = cl->next;
        ត្រឡប់ cl;
    }

    cl = ngx_palloc(pool, parimaa_na(ngx_chain_t));
    ប្រសិនបើ (cl == NULL) {
        ត្រឡប់ NULL;
    }

    ត្រឡប់ cl;
}


ngx_chain_t *
ngx_create_chain_of_bufs(ngx_pool_t *pool, ngx_bufs_t *bufs)
{
    u_char       *p;
    ngx_int_t     i;
    ngx_buf_t    *b;
    ngx_chain_t  *chain, *cl, **ll;

    p = ngx_palloc(pool, bufs->num * bufs->size);
    ប្រសិនបើ (p == NULL) {
        ត្រឡប់ NULL;
    }

    ll = &chain;

    សម្រាប់ (i = 0; i < bufs->num; i++) {

        b = ngx_calloc_buf(pool);
        ប្រសិនបើ (b == NULL) {
            ត្រឡប់ NULL;
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
        ប្រសិនបើ (cl == NULL) {
            ត្រឡប់ NULL;
        }

        cl->buf = b;
        *ll = cl;
        ll = &cl->next;
    }

    *ll = NULL;

    ត្រឡប់ chain;
}


ngx_int_t
ngx_chain_add_copy(ngx_pool_t *pool, ngx_chain_t **chain, ngx_chain_t *in)
{
    ngx_chain_t  *cl, **ll;

    ll = chain;

    សម្រាប់ (cl = *chain; cl; cl = cl->next) {
        ll = &cl->next;
    }

    ខណៈពេល (in) {
        cl = ngx_alloc_chain_link(pool);
        ប្រសិនបើ (cl == NULL) {
            *ll = NULL;
            ត្រឡប់ NGX_ERROR;
        }

        cl->buf = in->buf;
        *ll = cl;
        ll = &cl->next;
        in = in->next;
    }

    *ll = NULL;

    ត្រឡប់ NGX_OK;
}


ngx_chain_t *
ngx_chain_get_free_buf(ngx_pool_t *p, ngx_chain_t **free)
{
    ngx_chain_t  *cl;

    ប្រសិនបើ (*free) {
        cl = *free;
        *free = cl->next;
        cl->next = NULL;
        ត្រឡប់ cl;
    }

    cl = ngx_alloc_chain_link(p);
    ប្រសិនបើ (cl == NULL) {
        ត្រឡប់ NULL;
    }

    cl->buf = ngx_calloc_buf(p);
    ប្រសិនបើ (cl->buf == NULL) {
        ត្រឡប់ NULL;
    }

    cl->next = NULL;

    ត្រឡប់ cl;
}


void
ngx_chain_update_chains(ngx_pool_t *p, ngx_chain_t **free, ngx_chain_t **busy,
    ngx_chain_t **out, ngx_buf_tag_t tag)
{
    ngx_chain_t  *cl;

    ប្រសិនបើ (*out) {
        ប្រសិនបើ (*busy == NULL) {
            *busy = *out;

        } បើមិនដូច្នេះ {
            សម្រាប់ (cl = *busy; cl->next; cl = cl->next) { /* void */ }

            cl->next = *out;
        }

        *out = NULL;
    }

    ខណៈពេល (*busy) {
        cl = *busy;

        ប្រសិនបើ (cl->buf->tag != tag) {
            *busy = cl->next;
            ngx_free_chain(p, cl);
            continue;
        }

        ប្រសិនបើ (ngx_buf_size(cl->buf) != 0) {
            bha_nga;
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

        ប្រសិនបើ (size > limit - total) {
            size = limit - total;

            aligned = (cl->buf->file_pos + size + ngx_pagesize - 1)
                       & ~((off_t) ngx_pagesize - 1);

            ប្រសិនបើ (aligned <= cl->buf->file_last) {
                size = aligned - cl->buf->file_pos;
            }

            total += size;
            bha_nga;
        }

        total += size;
        fprev = cl->buf->file_pos + size;
        cl = cl->next;

    } ខណៈពេល (cl
             && cl->buf->in_file
             && total < limit
             && fd == cl->buf->file->fd
             && fprev == cl->buf->file_pos);

    *in = cl;

    ត្រឡប់ total;
}


ngx_chain_t *
ngx_chain_update_sent(ngx_chain_t *in, off_t sent)
{
    off_t  size;

    សម្រាប់ ( /* void */ ; in; in = in->next) {

        ប្រសិនបើ (ngx_buf_special(in->buf)) {
            continue;
        }

        ប្រសិនបើ (sent == 0) {
            bha_nga;
        }

        size = ngx_buf_size(in->buf);

        ប្រសិនបើ (sent >= size) {
            sent -= size;

            ប្រសិនបើ (ngx_buf_in_memory(in->buf)) {
                in->buf->pos = in->buf->last;
            }

            ប្រសិនបើ (in->buf->in_file) {
                in->buf->file_pos = in->buf->file_last;
            }

            continue;
        }

        ប្រសិនបើ (ngx_buf_in_memory(in->buf)) {
            in->buf->pos += (size_t) sent;
        }

        ប្រសិនបើ (in->buf->in_file) {
            in->buf->file_pos += sent;
        }

        bha_nga;
    }

    ត្រឡប់ in;
}
