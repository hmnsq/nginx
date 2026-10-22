
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>


ngx_buf_t *
ngx_create_ਆਰਜ਼ੀ_buf(ngx_pool_t *pool, ਮਾਪ_t ਮਾਪ)
{
    ngx_buf_t *b;

    b = ngx_calloc_buf(pool);
    ਜੇ (b == ਨਿਰਾ) {
        ਵਾਪਸ ਨਿਰਾ;
    }

    b->ਸ਼ੁਰੂ = ngx_palloc(pool, ਮਾਪ);
    ਜੇ (b->ਸ਼ੁਰੂ == ਨਿਰਾ) {
        ਵਾਪਸ ਨਿਰਾ;
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

    b->pos = b->ਸ਼ੁਰੂ;
    b->ਆਖਰੀ = b->ਸ਼ੁਰੂ;
    b->ਸਿਰਾ = b->ਆਖਰੀ + ਮਾਪ;
    b->temporary = 1;

    ਵਾਪਸ b;
}


ngx_chain_t *
ngx_alloc_chain_link(ngx_pool_t *pool)
{
    ngx_chain_t  *cl;

    cl = pool->chain;

    ਜੇ (cl) {
        pool->chain = cl->ਅਗਲਾ;
        ਵਾਪਸ cl;
    }

    cl = ngx_palloc(pool, ਆਕਾਰ(ngx_chain_t));
    ਜੇ (cl == ਨਿਰਾ) {
        ਵਾਪਸ ਨਿਰਾ;
    }

    ਵਾਪਸ cl;
}


ngx_chain_t *
ngx_create_chain_ਦਾ_bufs(ngx_pool_t *pool, ngx_bufs_t *bufs)
{
    u_char       *p;
    ngx_int_t     i;
    ngx_buf_t    *b;
    ngx_chain_t  *chain, *cl, **ll;

    p = ngx_palloc(pool, bufs->ਅੰਕ * bufs->ਮਾਪ);
    ਜੇ (p == ਨਿਰਾ) {
        ਵਾਪਸ ਨਿਰਾ;
    }

    ll = &chain;

    ਲਈ (i = 0; i < bufs->ਅੰਕ; i++) {

        b = ngx_calloc_buf(pool);
        ਜੇ (b == ਨਿਰਾ) {
            ਵਾਪਸ ਨਿਰਾ;
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
        b->ਆਖਰੀ = p;
        b->temporary = 1;

        b->ਸ਼ੁਰੂ = p;
        p += bufs->ਮਾਪ;
        b->ਸਿਰਾ = p;

        cl = ngx_alloc_chain_link(pool);
        ਜੇ (cl == ਨਿਰਾ) {
            ਵਾਪਸ ਨਿਰਾ;
        }

        cl->buf = b;
        *ll = cl;
        ll = &cl->ਅਗਲਾ;
    }

    *ll = ਨਿਰਾ;

    ਵਾਪਸ chain;
}


ngx_int_t
ngx_chain_ਜੋੜੋ_copy(ngx_pool_t *pool, ngx_chain_t **chain, ngx_chain_t *ਵਿੱਚ)
{
    ngx_chain_t  *cl, **ll;

    ll = chain;

    ਲਈ (cl = *chain; cl; cl = cl->ਅਗਲਾ) {
        ll = &cl->ਅਗਲਾ;
    }

    ਜਦੋਂ (ਵਿੱਚ) {
        cl = ngx_alloc_chain_link(pool);
        ਜੇ (cl == ਨਿਰਾ) {
            *ll = ਨਿਰਾ;
            ਵਾਪਸ NGX_ERROR;
        }

        cl->buf = ਵਿੱਚ->buf;
        *ll = cl;
        ll = &cl->ਅਗਲਾ;
        ਵਿੱਚ = ਵਿੱਚ->ਅਗਲਾ;
    }

    *ll = ਨਿਰਾ;

    ਵਾਪਸ NGX_OK;
}


ngx_chain_t *
ngx_chain_get_free_buf(ngx_pool_t *p, ngx_chain_t **ਮੁਕਤ)
{
    ngx_chain_t  *cl;

    ਜੇ (*ਮੁਕਤ) {
        cl = *ਮੁਕਤ;
        *ਮੁਕਤ = cl->ਅਗਲਾ;
        cl->ਅਗਲਾ = ਨਿਰਾ;
        ਵਾਪਸ cl;
    }

    cl = ngx_alloc_chain_link(p);
    ਜੇ (cl == ਨਿਰਾ) {
        ਵਾਪਸ ਨਿਰਾ;
    }

    cl->buf = ngx_calloc_buf(p);
    ਜੇ (cl->buf == ਨਿਰਾ) {
        ਵਾਪਸ ਨਿਰਾ;
    }

    cl->ਅਗਲਾ = ਨਿਰਾ;

    ਵਾਪਸ cl;
}


ਖਾਲੀ
ngx_chain_update_chains(ngx_pool_t *p, ngx_chain_t **ਮੁਕਤ, ngx_chain_t **busy,
    ngx_chain_t **ਬਾਹਰਲਾ, ngx_buf_ਟੈਗ_t ਟੈਗ)
{
    ngx_chain_t  *cl;

    ਜੇ (*ਬਾਹਰਲਾ) {
        ਜੇ (*busy == ਨਿਰਾ) {
            *busy = *ਬਾਹਰਲਾ;

        } ਨਹੀਂਤਾਂ {
            ਲਈ (cl = *busy; cl->ਅਗਲਾ; cl = cl->ਅਗਲਾ) { /* void */ }

            cl->ਅਗਲਾ = *ਬਾਹਰਲਾ;
        }

        *ਬਾਹਰਲਾ = ਨਿਰਾ;
    }

    ਜਦੋਂ (*busy) {
        cl = *busy;

        ਜੇ (cl->buf->ਟੈਗ != ਟੈਗ) {
            *busy = cl->ਅਗਲਾ;
            ngx_free_chain(p, cl);
            ਜਾਰੀ;
        }

        ਜੇ (ngx_buf_ਮਾਪ(cl->buf) != 0) {
            ਰੋਕੋ;
        }

        cl->buf->pos = cl->buf->ਸ਼ੁਰੂ;
        cl->buf->ਆਖਰੀ = cl->buf->ਸ਼ੁਰੂ;

        *busy = cl->ਅਗਲਾ;
        cl->ਅਗਲਾ = *ਮੁਕਤ;
        *ਮੁਕਤ = cl;
    }
}


off_t
ngx_chain_coalesce_file(ngx_chain_t **ਵਿੱਚ, off_t ਹੱਦਬੰਦੀ)
{
    off_t         ਕੁੱਲ, ਮਾਪ, aligned, fprev;
    ngx_fd_t      fd;
    ngx_chain_t  *cl;

    ਕੁੱਲ = 0;

    cl = *ਵਿੱਚ;
    fd = cl->buf->file->fd;

    ਕਰੋ {
        ਮਾਪ = cl->buf->file_ਆਖਰੀ - cl->buf->file_pos;

        ਜੇ (ਮਾਪ > ਹੱਦਬੰਦੀ - ਕੁੱਲ) {
            ਮਾਪ = ਹੱਦਬੰਦੀ - ਕੁੱਲ;

            aligned = (cl->buf->file_pos + ਮਾਪ + ngx_pagesize - 1)
                       & ~((off_t) ngx_pagesize - 1);

            ਜੇ (aligned <= cl->buf->file_ਆਖਰੀ) {
                ਮਾਪ = aligned - cl->buf->file_pos;
            }

            ਕੁੱਲ += ਮਾਪ;
            ਰੋਕੋ;
        }

        ਕੁੱਲ += ਮਾਪ;
        fprev = cl->buf->file_pos + ਮਾਪ;
        cl = cl->ਅਗਲਾ;

    } ਜਦੋਂ (cl
             && cl->buf->ਵਿੱਚ_file
             && ਕੁੱਲ < ਹੱਦਬੰਦੀ
             && fd == cl->buf->file->fd
             && fprev == cl->buf->file_pos);

    *ਵਿੱਚ = cl;

    ਵਾਪਸ ਕੁੱਲ;
}


ngx_chain_t *
ngx_chain_update_ਭੇਜਿਆ(ngx_chain_t *ਵਿੱਚ, off_t ਭੇਜਿਆ)
{
    off_t  ਮਾਪ;

    ਲਈ ( /* void */ ; ਵਿੱਚ; ਵਿੱਚ = ਵਿੱਚ->ਅਗਲਾ) {

        ਜੇ (ngx_buf_special(ਵਿੱਚ->buf)) {
            ਜਾਰੀ;
        }

        ਜੇ (ਭੇਜਿਆ == 0) {
            ਰੋਕੋ;
        }

        ਮਾਪ = ngx_buf_ਮਾਪ(ਵਿੱਚ->buf);

        ਜੇ (ਭੇਜਿਆ >= ਮਾਪ) {
            ਭੇਜਿਆ -= ਮਾਪ;

            ਜੇ (ngx_buf_ਵਿੱਚ_ਸਮ੍ਰਿਤੀ(ਵਿੱਚ->buf)) {
                ਵਿੱਚ->buf->pos = ਵਿੱਚ->buf->ਆਖਰੀ;
            }

            ਜੇ (ਵਿੱਚ->buf->ਵਿੱਚ_file) {
                ਵਿੱਚ->buf->file_pos = ਵਿੱਚ->buf->file_ਆਖਰੀ;
            }

            ਜਾਰੀ;
        }

        ਜੇ (ngx_buf_ਵਿੱਚ_ਸਮ੍ਰਿਤੀ(ਵਿੱਚ->buf)) {
            ਵਿੱਚ->buf->pos += (ਮਾਪ_t) ਭੇਜਿਆ;
        }

        ਜੇ (ਵਿੱਚ->buf->ਵਿੱਚ_file) {
            ਵਿੱਚ->buf->file_pos += ਭੇਜਿਆ;
        }

        ਰੋਕੋ;
    }

    ਵਾਪਸ ਵਿੱਚ;
}
