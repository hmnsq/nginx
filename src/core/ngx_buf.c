
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


ngx_buf_t *
ngx_create_ತಾತ್ಕಾಲಿಕ_buf(ngx_pool_t *pool, ಗಾತ್ರ_t ಗಾತ್ರ_2)
{
    ngx_buf_t *b;

    b = ngx_calloc_buf(pool);
    ಆದರೆ (b == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    b->ಆರಂಭ = ngx_palloc(pool, ಗಾತ್ರ_2);
    ಆದರೆ (b->ಆರಂಭ == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
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

    b->pos = b->ಆರಂಭ;
    b->ಕೊನೆಯ = b->ಆರಂಭ;
    b->ಕೊನೆ = b->ಕೊನೆಯ + ಗಾತ್ರ_2;
    b->temporary = 1;

    ಹಿಂತಿರುಗಿ b;
}


ngx_chain_t *
ngx_alloc_chain_link(ngx_pool_t *pool)
{
    ngx_chain_t  *cl;

    cl = pool->chain;

    ಆದರೆ (cl) {
        pool->chain = cl->ಮುಂದಿನ;
        ಹಿಂತಿರುಗಿ cl;
    }

    cl = ngx_palloc(pool, ಗಾತ್ರ(ngx_chain_t));
    ಆದರೆ (cl == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ಹಿಂತಿರುಗಿ cl;
}


ngx_chain_t *
ngx_create_chain_ದ_bufs(ngx_pool_t *pool, ngx_bufs_t *bufs)
{
    u_char       *p;
    ngx_int_t     i;
    ngx_buf_t    *b;
    ngx_chain_t  *chain, *cl, **ll;

    p = ngx_palloc(pool, bufs->ಅಂಕೆ * bufs->ಗಾತ್ರ_2);
    ಆದರೆ (p == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ll = &chain;

    ಗಾಗಿ (i = 0; i < bufs->ಅಂಕೆ; i++) {

        b = ngx_calloc_buf(pool);
        ಆದರೆ (b == ಖಾಲಿ) {
            ಹಿಂತಿರುಗಿ ಖಾಲಿ;
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
        b->ಕೊನೆಯ = p;
        b->temporary = 1;

        b->ಆರಂಭ = p;
        p += bufs->ಗಾತ್ರ_2;
        b->ಕೊನೆ = p;

        cl = ngx_alloc_chain_link(pool);
        ಆದರೆ (cl == ಖಾಲಿ) {
            ಹಿಂತಿರುಗಿ ಖಾಲಿ;
        }

        cl->buf = b;
        *ll = cl;
        ll = &cl->ಮುಂದಿನ;
    }

    *ll = ಖಾಲಿ;

    ಹಿಂತಿರುಗಿ chain;
}


ngx_int_t
ngx_chain_ಕೂಡಿಸು_copy(ngx_pool_t *pool, ngx_chain_t **chain, ngx_chain_t *ಒಳ)
{
    ngx_chain_t  *cl, **ll;

    ll = chain;

    ಗಾಗಿ (cl = *chain; cl; cl = cl->ಮುಂದಿನ) {
        ll = &cl->ಮುಂದಿನ;
    }

    ಆಗ (ಒಳ) {
        cl = ngx_alloc_chain_link(pool);
        ಆದರೆ (cl == ಖಾಲಿ) {
            *ll = ಖಾಲಿ;
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        cl->buf = ಒಳ->buf;
        *ll = cl;
        ll = &cl->ಮುಂದಿನ;
        ಒಳ = ಒಳ->ಮುಂದಿನ;
    }

    *ll = ಖಾಲಿ;

    ಹಿಂತಿರುಗಿ NGX_OK;
}


ngx_chain_t *
ngx_chain_get_free_buf(ngx_pool_t *p, ngx_chain_t **ಬಿಡುಗಡೆ)
{
    ngx_chain_t  *cl;

    ಆದರೆ (*ಬಿಡುಗಡೆ) {
        cl = *ಬಿಡುಗಡೆ;
        *ಬಿಡುಗಡೆ = cl->ಮುಂದಿನ;
        cl->ಮುಂದಿನ = ಖಾಲಿ;
        ಹಿಂತಿರುಗಿ cl;
    }

    cl = ngx_alloc_chain_link(p);
    ಆದರೆ (cl == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    cl->buf = ngx_calloc_buf(p);
    ಆದರೆ (cl->buf == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    cl->ಮುಂದಿನ = ಖಾಲಿ;

    ಹಿಂತಿರುಗಿ cl;
}


ಶೂನ್ಯ
ngx_chain_update_chains(ngx_pool_t *p, ngx_chain_t **ಬಿಡುಗಡೆ, ngx_chain_t **busy,
    ngx_chain_t **ಹೊರಹೋಗು, ngx_buf_ಟ್ಯಾಗ್_t ಟ್ಯಾಗ್)
{
    ngx_chain_t  *cl;

    ಆದರೆ (*ಹೊರಹೋಗು) {
        ಆದರೆ (*busy == ಖಾಲಿ) {
            *busy = *ಹೊರಹೋಗು;

        } ಇಲ್ಲದಿದ್ದರೆ {
            ಗಾಗಿ (cl = *busy; cl->ಮುಂದಿನ; cl = cl->ಮುಂದಿನ) { /* void */ }

            cl->ಮುಂದಿನ = *ಹೊರಹೋಗು;
        }

        *ಹೊರಹೋಗು = ಖಾಲಿ;
    }

    ಆಗ (*busy) {
        cl = *busy;

        ಆದರೆ (cl->buf->ಟ್ಯಾಗ್ != ಟ್ಯಾಗ್) {
            *busy = cl->ಮುಂದಿನ;
            ngx_free_chain(p, cl);
            ಮುಂದುವರಿ;
        }

        ಆದರೆ (ngx_buf_ಗಾತ್ರ(cl->buf) != 0) {
            ನಿಲ್ಲಿಸು;
        }

        cl->buf->pos = cl->buf->ಆರಂಭ;
        cl->buf->ಕೊನೆಯ = cl->buf->ಆರಂಭ;

        *busy = cl->ಮುಂದಿನ;
        cl->ಮುಂದಿನ = *ಬಿಡುಗಡೆ;
        *ಬಿಡುಗಡೆ = cl;
    }
}


off_t
ngx_chain_coalesce_file(ngx_chain_t **ಒಳ, off_t ಎಲ್ಲೆ)
{
    off_t         ಒಟ್ಟು, ಗಾತ್ರ_2, aligned, fprev;
    ngx_fd_t      fd;
    ngx_chain_t  *cl;

    ಒಟ್ಟು = 0;

    cl = *ಒಳ;
    fd = cl->buf->file->fd;

    ಮಾಡು {
        ಗಾತ್ರ_2 = cl->buf->file_ಕೊನೆಯ - cl->buf->file_pos;

        ಆದರೆ (ಗಾತ್ರ_2 > ಎಲ್ಲೆ - ಒಟ್ಟು) {
            ಗಾತ್ರ_2 = ಎಲ್ಲೆ - ಒಟ್ಟು;

            aligned = (cl->buf->file_pos + ಗಾತ್ರ_2 + ngx_pagesize - 1)
                       & ~((off_t) ngx_pagesize - 1);

            ಆದರೆ (aligned <= cl->buf->file_ಕೊನೆಯ) {
                ಗಾತ್ರ_2 = aligned - cl->buf->file_pos;
            }

            ಒಟ್ಟು += ಗಾತ್ರ_2;
            ನಿಲ್ಲಿಸು;
        }

        ಒಟ್ಟು += ಗಾತ್ರ_2;
        fprev = cl->buf->file_pos + ಗಾತ್ರ_2;
        cl = cl->ಮುಂದಿನ;

    } ಆಗ (cl
             && cl->buf->ಒಳ_file
             && ಒಟ್ಟು < ಎಲ್ಲೆ
             && fd == cl->buf->file->fd
             && fprev == cl->buf->file_pos);

    *ಒಳ = cl;

    ಹಿಂತಿರುಗಿ ಒಟ್ಟು;
}


ngx_chain_t *
ngx_chain_update_ಕಳುಹಿಸಿದ(ngx_chain_t *ಒಳ, off_t ಕಳುಹಿಸಿದ)
{
    off_t  ಗಾತ್ರ_2;

    ಗಾಗಿ ( /* void */ ; ಒಳ; ಒಳ = ಒಳ->ಮುಂದಿನ) {

        ಆದರೆ (ngx_buf_special(ಒಳ->buf)) {
            ಮುಂದುವರಿ;
        }

        ಆದರೆ (ಕಳುಹಿಸಿದ == 0) {
            ನಿಲ್ಲಿಸು;
        }

        ಗಾತ್ರ_2 = ngx_buf_ಗಾತ್ರ(ಒಳ->buf);

        ಆದರೆ (ಕಳುಹಿಸಿದ >= ಗಾತ್ರ_2) {
            ಕಳುಹಿಸಿದ -= ಗಾತ್ರ_2;

            ಆದರೆ (ngx_buf_ಒಳ_ಸ್ಮರಣೆ(ಒಳ->buf)) {
                ಒಳ->buf->pos = ಒಳ->buf->ಕೊನೆಯ;
            }

            ಆದರೆ (ಒಳ->buf->ಒಳ_file) {
                ಒಳ->buf->file_pos = ಒಳ->buf->file_ಕೊನೆಯ;
            }

            ಮುಂದುವರಿ;
        }

        ಆದರೆ (ngx_buf_ಒಳ_ಸ್ಮರಣೆ(ಒಳ->buf)) {
            ಒಳ->buf->pos += (ಗಾತ್ರ_t) ಕಳುಹಿಸಿದ;
        }

        ಆದರೆ (ಒಳ->buf->ಒಳ_file) {
            ಒಳ->buf->file_pos += ಕಳುಹಿಸಿದ;
        }

        ನಿಲ್ಲಿಸು;
    }

    ಹಿಂತಿರುಗಿ ಒಳ;
}
