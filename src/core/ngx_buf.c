
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


ngx_buf_t *
ngx_create_अस्थायी_buf(ngx_pool_t *pool, आकार_t आकार)
{
    ngx_buf_t *b;

    b = ngx_calloc_buf(pool);
    यदि (b == रिक्त) {
        प्रत्यागम रिक्त;
    }

    b->start = ngx_palloc(pool, आकार);
    यदि (b->start == रिक्त) {
        प्रत्यागम रिक्त;
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
    b->चरमः = b->start;
    b->अन्तः = b->चरमः + आकार;
    b->temporary = 1;

    प्रत्यागम b;
}


ngx_chain_t *
ngx_alloc_chain_link(ngx_pool_t *pool)
{
    ngx_chain_t  *cl;

    cl = pool->chain;

    यदि (cl) {
        pool->chain = cl->अग्र;
        प्रत्यागम cl;
    }

    cl = ngx_palloc(pool, परिमाण(ngx_chain_t));
    यदि (cl == रिक्त) {
        प्रत्यागम रिक्त;
    }

    प्रत्यागम cl;
}


ngx_chain_t *
ngx_create_chain_सम्बन्धि_bufs(ngx_pool_t *pool, ngx_bufs_t *bufs)
{
    u_char       *p;
    ngx_int_t     i;
    ngx_buf_t    *b;
    ngx_chain_t  *chain, *cl, **ll;

    p = ngx_palloc(pool, bufs->अङ्कः * bufs->आकार);
    यदि (p == रिक्त) {
        प्रत्यागम रिक्त;
    }

    ll = &chain;

    प्रति (i = 0; i < bufs->अङ्कः; i++) {

        b = ngx_calloc_buf(pool);
        यदि (b == रिक्त) {
            प्रत्यागम रिक्त;
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
        b->चरमः = p;
        b->temporary = 1;

        b->start = p;
        p += bufs->आकार;
        b->अन्तः = p;

        cl = ngx_alloc_chain_link(pool);
        यदि (cl == रिक्त) {
            प्रत्यागम रिक्त;
        }

        cl->buf = b;
        *ll = cl;
        ll = &cl->अग्र;
    }

    *ll = रिक्त;

    प्रत्यागम chain;
}


ngx_int_t
ngx_chain_सङ्कलय_copy(ngx_pool_t *pool, ngx_chain_t **chain, ngx_chain_t *अन्तर्)
{
    ngx_chain_t  *cl, **ll;

    ll = chain;

    प्रति (cl = *chain; cl; cl = cl->अग्र) {
        ll = &cl->अग्र;
    }

    यावत् (अन्तर्) {
        cl = ngx_alloc_chain_link(pool);
        यदि (cl == रिक्त) {
            *ll = रिक्त;
            प्रत्यागम NGX_ERROR;
        }

        cl->buf = अन्तर्->buf;
        *ll = cl;
        ll = &cl->अग्र;
        अन्तर् = अन्तर्->अग्र;
    }

    *ll = रिक्त;

    प्रत्यागम NGX_OK;
}


ngx_chain_t *
ngx_chain_get_free_buf(ngx_pool_t *p, ngx_chain_t **मुक्त)
{
    ngx_chain_t  *cl;

    यदि (*मुक्त) {
        cl = *मुक्त;
        *मुक्त = cl->अग्र;
        cl->अग्र = रिक्त;
        प्रत्यागम cl;
    }

    cl = ngx_alloc_chain_link(p);
    यदि (cl == रिक्त) {
        प्रत्यागम रिक्त;
    }

    cl->buf = ngx_calloc_buf(p);
    यदि (cl->buf == रिक्त) {
        प्रत्यागम रिक्त;
    }

    cl->अग्र = रिक्त;

    प्रत्यागम cl;
}


शून्य
ngx_chain_update_chains(ngx_pool_t *p, ngx_chain_t **मुक्त, ngx_chain_t **busy,
    ngx_chain_t **बहिर्गमनम्, ngx_buf_चिह्नकम्_t चिह्नकम्)
{
    ngx_chain_t  *cl;

    यदि (*बहिर्गमनम्) {
        यदि (*busy == रिक्त) {
            *busy = *बहिर्गमनम्;

        } अन्यथा {
            प्रति (cl = *busy; cl->अग्र; cl = cl->अग्र) { /* void */ }

            cl->अग्र = *बहिर्गमनम्;
        }

        *बहिर्गमनम् = रिक्त;
    }

    यावत् (*busy) {
        cl = *busy;

        यदि (cl->buf->चिह्नकम् != चिह्नकम्) {
            *busy = cl->अग्र;
            ngx_free_chain(p, cl);
            अनुवर्त;
        }

        यदि (ngx_buf_आकार(cl->buf) != 0) {
            भङ्ग;
        }

        cl->buf->pos = cl->buf->start;
        cl->buf->चरमः = cl->buf->start;

        *busy = cl->अग्र;
        cl->अग्र = *मुक्त;
        *मुक्त = cl;
    }
}


off_t
ngx_chain_coalesce_file(ngx_chain_t **अन्तर्, off_t सीमा)
{
    off_t         योग, आकार, aligned, fprev;
    ngx_fd_t      fd;
    ngx_chain_t  *cl;

    योग = 0;

    cl = *अन्तर्;
    fd = cl->buf->file->fd;

    कुरु {
        आकार = cl->buf->file_चरमः - cl->buf->file_pos;

        यदि (आकार > सीमा - योग) {
            आकार = सीमा - योग;

            aligned = (cl->buf->file_pos + आकार + ngx_pagesize - 1)
                       & ~((off_t) ngx_pagesize - 1);

            यदि (aligned <= cl->buf->file_चरमः) {
                आकार = aligned - cl->buf->file_pos;
            }

            योग += आकार;
            भङ्ग;
        }

        योग += आकार;
        fprev = cl->buf->file_pos + आकार;
        cl = cl->अग्र;

    } यावत् (cl
             && cl->buf->अन्तर्_file
             && योग < सीमा
             && fd == cl->buf->file->fd
             && fprev == cl->buf->file_pos);

    *अन्तर् = cl;

    प्रत्यागम योग;
}


ngx_chain_t *
ngx_chain_update_प्रेषितम्(ngx_chain_t *अन्तर्, off_t प्रेषितम्)
{
    off_t  आकार;

    प्रति ( /* void */ ; अन्तर्; अन्तर् = अन्तर्->अग्र) {

        यदि (ngx_buf_special(अन्तर्->buf)) {
            अनुवर्त;
        }

        यदि (प्रेषितम् == 0) {
            भङ्ग;
        }

        आकार = ngx_buf_आकार(अन्तर्->buf);

        यदि (प्रेषितम् >= आकार) {
            प्रेषितम् -= आकार;

            यदि (ngx_buf_अन्तर्_स्मृतिः(अन्तर्->buf)) {
                अन्तर्->buf->pos = अन्तर्->buf->चरमः;
            }

            यदि (अन्तर्->buf->अन्तर्_file) {
                अन्तर्->buf->file_pos = अन्तर्->buf->file_चरमः;
            }

            अनुवर्त;
        }

        यदि (ngx_buf_अन्तर्_स्मृतिः(अन्तर्->buf)) {
            अन्तर्->buf->pos += (आकार_t) प्रेषितम्;
        }

        यदि (अन्तर्->buf->अन्तर्_file) {
            अन्तर्->buf->file_pos += प्रेषितम्;
        }

        भङ्ग;
    }

    प्रत्यागम अन्तर्;
}
