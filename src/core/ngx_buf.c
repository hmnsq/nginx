
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
    གལ་ཏེ (b == NULL) {
        ཕྱིར NULL;
    }

    b->start = ngx_palloc(pool, size);
    གལ་ཏེ (b->start == NULL) {
        ཕྱིར NULL;
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

    ཕྱིར b;
}


ngx_chain_t *
ngx_alloc_chain_link(ngx_pool_t *pool)
{
    ngx_chain_t  *cl;

    cl = pool->chain;

    གལ་ཏེ (cl) {
        pool->chain = cl->next;
        ཕྱིར cl;
    }

    cl = ngx_palloc(pool, parimaa_na(ngx_chain_t));
    གལ་ཏེ (cl == NULL) {
        ཕྱིར NULL;
    }

    ཕྱིར cl;
}


ngx_chain_t *
ngx_create_chain_of_bufs(ngx_pool_t *pool, ngx_bufs_t *bufs)
{
    u_char       *p;
    ngx_int_t     i;
    ngx_buf_t    *b;
    ngx_chain_t  *chain, *cl, **ll;

    p = ngx_palloc(pool, bufs->num * bufs->size);
    གལ་ཏེ (p == NULL) {
        ཕྱིར NULL;
    }

    ll = &chain;

    ཆེད་དུ (i = 0; i < bufs->num; i++) {

        b = ngx_calloc_buf(pool);
        གལ་ཏེ (b == NULL) {
            ཕྱིར NULL;
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
        གལ་ཏེ (cl == NULL) {
            ཕྱིར NULL;
        }

        cl->buf = b;
        *ll = cl;
        ll = &cl->next;
    }

    *ll = NULL;

    ཕྱིར chain;
}


ngx_int_t
ngx_chain_add_copy(ngx_pool_t *pool, ngx_chain_t **chain, ngx_chain_t *in)
{
    ngx_chain_t  *cl, **ll;

    ll = chain;

    ཆེད་དུ (cl = *chain; cl; cl = cl->next) {
        ll = &cl->next;
    }

    སྐབས (in) {
        cl = ngx_alloc_chain_link(pool);
        གལ་ཏེ (cl == NULL) {
            *ll = NULL;
            ཕྱིར NGX_ERROR;
        }

        cl->buf = in->buf;
        *ll = cl;
        ll = &cl->next;
        in = in->next;
    }

    *ll = NULL;

    ཕྱིར NGX_OK;
}


ngx_chain_t *
ngx_chain_get_free_buf(ngx_pool_t *p, ngx_chain_t **free)
{
    ngx_chain_t  *cl;

    གལ་ཏེ (*free) {
        cl = *free;
        *free = cl->next;
        cl->next = NULL;
        ཕྱིར cl;
    }

    cl = ngx_alloc_chain_link(p);
    གལ་ཏེ (cl == NULL) {
        ཕྱིར NULL;
    }

    cl->buf = ngx_calloc_buf(p);
    གལ་ཏེ (cl->buf == NULL) {
        ཕྱིར NULL;
    }

    cl->next = NULL;

    ཕྱིར cl;
}


void
ngx_chain_update_chains(ngx_pool_t *p, ngx_chain_t **free, ngx_chain_t **busy,
    ngx_chain_t **out, ngx_buf_tag_t tag)
{
    ngx_chain_t  *cl;

    གལ་ཏེ (*out) {
        གལ་ཏེ (*busy == NULL) {
            *busy = *out;

        } དེ་མིན {
            ཆེད་དུ (cl = *busy; cl->next; cl = cl->next) { /* void */ }

            cl->next = *out;
        }

        *out = NULL;
    }

    སྐབས (*busy) {
        cl = *busy;

        གལ་ཏེ (cl->buf->tag != tag) {
            *busy = cl->next;
            ngx_free_chain(p, cl);
            continue;
        }

        གལ་ཏེ (ngx_buf_size(cl->buf) != 0) {
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

        གལ་ཏེ (size > limit - total) {
            size = limit - total;

            aligned = (cl->buf->file_pos + size + ngx_pagesize - 1)
                       & ~((off_t) ngx_pagesize - 1);

            གལ་ཏེ (aligned <= cl->buf->file_last) {
                size = aligned - cl->buf->file_pos;
            }

            total += size;
            bha_nga;
        }

        total += size;
        fprev = cl->buf->file_pos + size;
        cl = cl->next;

    } སྐབས (cl
             && cl->buf->in_file
             && total < limit
             && fd == cl->buf->file->fd
             && fprev == cl->buf->file_pos);

    *in = cl;

    ཕྱིར total;
}


ngx_chain_t *
ngx_chain_update_sent(ngx_chain_t *in, off_t sent)
{
    off_t  size;

    ཆེད་དུ ( /* void */ ; in; in = in->next) {

        གལ་ཏེ (ngx_buf_special(in->buf)) {
            continue;
        }

        གལ་ཏེ (sent == 0) {
            bha_nga;
        }

        size = ngx_buf_size(in->buf);

        གལ་ཏེ (sent >= size) {
            sent -= size;

            གལ་ཏེ (ngx_buf_in_memory(in->buf)) {
                in->buf->pos = in->buf->last;
            }

            གལ་ཏེ (in->buf->in_file) {
                in->buf->file_pos = in->buf->file_last;
            }

            continue;
        }

        གལ་ཏེ (ngx_buf_in_memory(in->buf)) {
            in->buf->pos += (size_t) sent;
        }

        གལ་ཏེ (in->buf->in_file) {
            in->buf->file_pos += sent;
        }

        bha_nga;
    }

    ཕྱིར in;
}
