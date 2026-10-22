
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>


ngx_buf_t *
ngx_create_తాత్కాలిక_buf(ngx_pool_t *pool, కొలత_t కొలత)
{
    ngx_buf_t *b;

    b = ngx_calloc_buf(pool);
    ఒకవేళ (b == ఖాళీ) {
        తిరిగి ఖాళీ;
    }

    b->ప్రారంభం = ngx_palloc(pool, కొలత);
    ఒకవేళ (b->ప్రారంభం == ఖాళీ) {
        తిరిగి ఖాళీ;
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

    b->pos = b->ప్రారంభం;
    b->చివరి = b->ప్రారంభం;
    b->చివర = b->చివరి + కొలత;
    b->temporary = 1;

    తిరిగి b;
}


ngx_chain_t *
ngx_alloc_chain_link(ngx_pool_t *pool)
{
    ngx_chain_t  *cl;

    cl = pool->chain;

    ఒకవేళ (cl) {
        pool->chain = cl->తదుపరి;
        తిరిగి cl;
    }

    cl = ngx_palloc(pool, పరిమాణం(ngx_chain_t));
    ఒకవేళ (cl == ఖాళీ) {
        తిరిగి ఖాళీ;
    }

    తిరిగి cl;
}


ngx_chain_t *
ngx_create_chain_యొక్క_bufs(ngx_pool_t *pool, ngx_bufs_t *bufs)
{
    u_char       *p;
    ngx_int_t     i;
    ngx_buf_t    *b;
    ngx_chain_t  *chain, *cl, **ll;

    p = ngx_palloc(pool, bufs->అంకె * bufs->కొలత);
    ఒకవేళ (p == ఖాళీ) {
        తిరిగి ఖాళీ;
    }

    ll = &chain;

    కొరకు (i = 0; i < bufs->అంకె; i++) {

        b = ngx_calloc_buf(pool);
        ఒకవేళ (b == ఖాళీ) {
            తిరిగి ఖాళీ;
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
        b->చివరి = p;
        b->temporary = 1;

        b->ప్రారంభం = p;
        p += bufs->కొలత;
        b->చివర = p;

        cl = ngx_alloc_chain_link(pool);
        ఒకవేళ (cl == ఖాళీ) {
            తిరిగి ఖాళీ;
        }

        cl->buf = b;
        *ll = cl;
        ll = &cl->తదుపరి;
    }

    *ll = ఖాళీ;

    తిరిగి chain;
}


ngx_int_t
ngx_chain_కూడు_copy(ngx_pool_t *pool, ngx_chain_t **chain, ngx_chain_t *లోపల)
{
    ngx_chain_t  *cl, **ll;

    ll = chain;

    కొరకు (cl = *chain; cl; cl = cl->తదుపరి) {
        ll = &cl->తదుపరి;
    }

    అయితే (లోపల) {
        cl = ngx_alloc_chain_link(pool);
        ఒకవేళ (cl == ఖాళీ) {
            *ll = ఖాళీ;
            తిరిగి NGX_ERROR;
        }

        cl->buf = లోపల->buf;
        *ll = cl;
        ll = &cl->తదుపరి;
        లోపల = లోపల->తదుపరి;
    }

    *ll = ఖాళీ;

    తిరిగి NGX_OK;
}


ngx_chain_t *
ngx_chain_get_free_buf(ngx_pool_t *p, ngx_chain_t **విడుదల)
{
    ngx_chain_t  *cl;

    ఒకవేళ (*విడుదల) {
        cl = *విడుదల;
        *విడుదల = cl->తదుపరి;
        cl->తదుపరి = ఖాళీ;
        తిరిగి cl;
    }

    cl = ngx_alloc_chain_link(p);
    ఒకవేళ (cl == ఖాళీ) {
        తిరిగి ఖాళీ;
    }

    cl->buf = ngx_calloc_buf(p);
    ఒకవేళ (cl->buf == ఖాళీ) {
        తిరిగి ఖాళీ;
    }

    cl->తదుపరి = ఖాళీ;

    తిరిగి cl;
}


శూన్య
ngx_chain_update_chains(ngx_pool_t *p, ngx_chain_t **విడుదల, ngx_chain_t **busy,
    ngx_chain_t **బహిర్గతం, ngx_buf_ట్యాగ్_t ట్యాగ్)
{
    ngx_chain_t  *cl;

    ఒకవేళ (*బహిర్గతం) {
        ఒకవేళ (*busy == ఖాళీ) {
            *busy = *బహిర్గతం;

        } లేకపోతే {
            కొరకు (cl = *busy; cl->తదుపరి; cl = cl->తదుపరి) { /* void */ }

            cl->తదుపరి = *బహిర్గతం;
        }

        *బహిర్గతం = ఖాళీ;
    }

    అయితే (*busy) {
        cl = *busy;

        ఒకవేళ (cl->buf->ట్యాగ్ != ట్యాగ్) {
            *busy = cl->తదుపరి;
            ngx_free_chain(p, cl);
            కొనసాగు;
        }

        ఒకవేళ (ngx_buf_కొలత(cl->buf) != 0) {
            ఆపు;
        }

        cl->buf->pos = cl->buf->ప్రారంభం;
        cl->buf->చివరి = cl->buf->ప్రారంభం;

        *busy = cl->తదుపరి;
        cl->తదుపరి = *విడుదల;
        *విడుదల = cl;
    }
}


off_t
ngx_chain_coalesce_file(ngx_chain_t **లోపల, off_t హద్దు)
{
    off_t         మొత్తం, కొలత, aligned, fprev;
    ngx_fd_t      fd;
    ngx_chain_t  *cl;

    మొత్తం = 0;

    cl = *లోపల;
    fd = cl->buf->file->fd;

    చేయి {
        కొలత = cl->buf->file_చివరి - cl->buf->file_pos;

        ఒకవేళ (కొలత > హద్దు - మొత్తం) {
            కొలత = హద్దు - మొత్తం;

            aligned = (cl->buf->file_pos + కొలత + ngx_pagesize - 1)
                       & ~((off_t) ngx_pagesize - 1);

            ఒకవేళ (aligned <= cl->buf->file_చివరి) {
                కొలత = aligned - cl->buf->file_pos;
            }

            మొత్తం += కొలత;
            ఆపు;
        }

        మొత్తం += కొలత;
        fprev = cl->buf->file_pos + కొలత;
        cl = cl->తదుపరి;

    } అయితే (cl
             && cl->buf->లోపల_file
             && మొత్తం < హద్దు
             && fd == cl->buf->file->fd
             && fprev == cl->buf->file_pos);

    *లోపల = cl;

    తిరిగి మొత్తం;
}


ngx_chain_t *
ngx_chain_update_పంపిన(ngx_chain_t *లోపల, off_t పంపిన)
{
    off_t  కొలత;

    కొరకు ( /* void */ ; లోపల; లోపల = లోపల->తదుపరి) {

        ఒకవేళ (ngx_buf_special(లోపల->buf)) {
            కొనసాగు;
        }

        ఒకవేళ (పంపిన == 0) {
            ఆపు;
        }

        కొలత = ngx_buf_కొలత(లోపల->buf);

        ఒకవేళ (పంపిన >= కొలత) {
            పంపిన -= కొలత;

            ఒకవేళ (ngx_buf_లోపల_జ్ఞాపకం(లోపల->buf)) {
                లోపల->buf->pos = లోపల->buf->చివరి;
            }

            ఒకవేళ (లోపల->buf->లోపల_file) {
                లోపల->buf->file_pos = లోపల->buf->file_చివరి;
            }

            కొనసాగు;
        }

        ఒకవేళ (ngx_buf_లోపల_జ్ఞాపకం(లోపల->buf)) {
            లోపల->buf->pos += (కొలత_t) పంపిన;
        }

        ఒకవేళ (లోపల->buf->లోపల_file) {
            లోపల->buf->file_pos += పంపిన;
        }

        ఆపు;
    }

    తిరిగి లోపల;
}
