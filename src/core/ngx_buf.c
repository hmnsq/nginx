
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#অন্তর্ভাব <ngx_config.h>
#অন্তর্ভাব <ngx_core.h>


ngx_buf_t *
ngx_create_অস্থায়ী_buf(ngx_pool_t *pool, আকার_t আকার_2)
{
    ngx_buf_t *b;

    b = ngx_calloc_buf(pool);
    যদি (b == ফাঁকা) {
        ফেরত ফাঁকা;
    }

    b->start = ngx_palloc(pool, আকার_2);
    যদি (b->start == ফাঁকা) {
        ফেরত ফাঁকা;
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
    b->end = b->last + আকার_2;
    b->temporary = 1;

    ফেরত b;
}


ngx_chain_t *
ngx_alloc_chain_link(ngx_pool_t *pool)
{
    ngx_chain_t  *cl;

    cl = pool->chain;

    যদি (cl) {
        pool->chain = cl->পরবর্তী;
        ফেরত cl;
    }

    cl = ngx_palloc(pool, আকার(ngx_chain_t));
    যদি (cl == ফাঁকা) {
        ফেরত ফাঁকা;
    }

    ফেরত cl;
}


ngx_chain_t *
ngx_create_chain_of_bufs(ngx_pool_t *pool, ngx_bufs_t *bufs)
{
    u_char       *p;
    ngx_int_t     i;
    ngx_buf_t    *b;
    ngx_chain_t  *chain, *cl, **ll;

    p = ngx_palloc(pool, bufs->অঙ্ক * bufs->আকার_2);
    যদি (p == ফাঁকা) {
        ফেরত ফাঁকা;
    }

    ll = &chain;

    জন্য (i = 0; i < bufs->অঙ্ক; i++) {

        b = ngx_calloc_buf(pool);
        যদি (b == ফাঁকা) {
            ফেরত ফাঁকা;
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
        p += bufs->আকার_2;
        b->end = p;

        cl = ngx_alloc_chain_link(pool);
        যদি (cl == ফাঁকা) {
            ফেরত ফাঁকা;
        }

        cl->buf = b;
        *ll = cl;
        ll = &cl->পরবর্তী;
    }

    *ll = ফাঁকা;

    ফেরত chain;
}


ngx_int_t
ngx_chain_add_copy(ngx_pool_t *pool, ngx_chain_t **chain, ngx_chain_t *in)
{
    ngx_chain_t  *cl, **ll;

    ll = chain;

    জন্য (cl = *chain; cl; cl = cl->পরবর্তী) {
        ll = &cl->পরবর্তী;
    }

    যখন (in) {
        cl = ngx_alloc_chain_link(pool);
        যদি (cl == ফাঁকা) {
            *ll = ফাঁকা;
            ফেরত NGX_ERROR;
        }

        cl->buf = in->buf;
        *ll = cl;
        ll = &cl->পরবর্তী;
        in = in->পরবর্তী;
    }

    *ll = ফাঁকা;

    ফেরত NGX_OK;
}


ngx_chain_t *
ngx_chain_get_free_buf(ngx_pool_t *p, ngx_chain_t **মুক্ত)
{
    ngx_chain_t  *cl;

    যদি (*মুক্ত) {
        cl = *মুক্ত;
        *মুক্ত = cl->পরবর্তী;
        cl->পরবর্তী = ফাঁকা;
        ফেরত cl;
    }

    cl = ngx_alloc_chain_link(p);
    যদি (cl == ফাঁকা) {
        ফেরত ফাঁকা;
    }

    cl->buf = ngx_calloc_buf(p);
    যদি (cl->buf == ফাঁকা) {
        ফেরত ফাঁকা;
    }

    cl->পরবর্তী = ফাঁকা;

    ফেরত cl;
}


শূন্য
ngx_chain_update_chains(ngx_pool_t *p, ngx_chain_t **মুক্ত, ngx_chain_t **busy,
    ngx_chain_t **out, ngx_buf_tag_t tag)
{
    ngx_chain_t  *cl;

    যদি (*out) {
        যদি (*busy == ফাঁকা) {
            *busy = *out;

        } অন্যথা {
            জন্য (cl = *busy; cl->পরবর্তী; cl = cl->পরবর্তী) { /* void */ }

            cl->পরবর্তী = *out;
        }

        *out = ফাঁকা;
    }

    যখন (*busy) {
        cl = *busy;

        যদি (cl->buf->tag != tag) {
            *busy = cl->পরবর্তী;
            ngx_free_chain(p, cl);
            চালু;
        }

        যদি (ngx_buf_আকার(cl->buf) != 0) {
            বিরতি;
        }

        cl->buf->pos = cl->buf->start;
        cl->buf->last = cl->buf->start;

        *busy = cl->পরবর্তী;
        cl->পরবর্তী = *মুক্ত;
        *মুক্ত = cl;
    }
}


off_t
ngx_chain_coalesce_file(ngx_chain_t **in, off_t limit)
{
    off_t         যোগ, আকার_2, aligned, fprev;
    ngx_fd_t      fd;
    ngx_chain_t  *cl;

    যোগ = 0;

    cl = *in;
    fd = cl->buf->file->fd;

    করো {
        আকার_2 = cl->buf->file_last - cl->buf->file_pos;

        যদি (আকার_2 > limit - যোগ) {
            আকার_2 = limit - যোগ;

            aligned = (cl->buf->file_pos + আকার_2 + ngx_pagesize - 1)
                       & ~((off_t) ngx_pagesize - 1);

            যদি (aligned <= cl->buf->file_last) {
                আকার_2 = aligned - cl->buf->file_pos;
            }

            যোগ += আকার_2;
            বিরতি;
        }

        যোগ += আকার_2;
        fprev = cl->buf->file_pos + আকার_2;
        cl = cl->পরবর্তী;

    } যখন (cl
             && cl->buf->in_file
             && যোগ < limit
             && fd == cl->buf->file->fd
             && fprev == cl->buf->file_pos);

    *in = cl;

    ফেরত যোগ;
}


ngx_chain_t *
ngx_chain_update_sent(ngx_chain_t *in, off_t sent)
{
    off_t  আকার_2;

    জন্য ( /* void */ ; in; in = in->পরবর্তী) {

        যদি (ngx_buf_special(in->buf)) {
            চালু;
        }

        যদি (sent == 0) {
            বিরতি;
        }

        আকার_2 = ngx_buf_আকার(in->buf);

        যদি (sent >= আকার_2) {
            sent -= আকার_2;

            যদি (ngx_buf_in_memory(in->buf)) {
                in->buf->pos = in->buf->last;
            }

            যদি (in->buf->in_file) {
                in->buf->file_pos = in->buf->file_last;
            }

            চালু;
        }

        যদি (ngx_buf_in_memory(in->buf)) {
            in->buf->pos += (আকার_t) sent;
        }

        যদি (in->buf->in_file) {
            in->buf->file_pos += sent;
        }

        বিরতি;
    }

    ফেরত in;
}
