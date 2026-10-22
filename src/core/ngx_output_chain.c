
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_event.h>


#ਜੇ 0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_SENDFILE_ਹੱਦਬੰਦੀ  4096
#ਜੇ_ਅੰਤ

/*
 * When DIRECTIO is enabled FreeBSD, Solaris, and MacOSX read directly
 * to an application memory from a device if parameters are aligned
 * to device sector boundary (512 bytes).  They fallback to usual read
 * operation if the parameters are not aligned.
 * Linux allows DIRECTIO only if the parameters are aligned to a filesystem
 * sector boundary, otherwise it returns EINVAL.  The sector size is
 * usually 512 bytes, however, on XFS it may be 4096 bytes.
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_NONE            1


ਸਥਿਰ ngx_inline ngx_int_t
    ngx_output_chain_as_is(ngx_output_chain_ctx_t *ctx, ngx_buf_t *buf);
ਸਥਿਰ ngx_int_t ngx_output_chain_ਜੋੜੋ_copy(ngx_pool_t *pool,
    ngx_chain_t **chain, ngx_chain_t *ਵਿੱਚ);
ਸਥਿਰ ngx_int_t ngx_output_chain_align_file_buf(ngx_output_chain_ctx_t *ctx,
    off_t bsize);
ਸਥਿਰ ngx_int_t ngx_output_chain_get_buf(ngx_output_chain_ctx_t *ctx,
    off_t bsize);
ਸਥਿਰ ngx_int_t ngx_output_chain_copy_buf(ngx_output_chain_ctx_t *ctx);


ngx_int_t
ngx_output_chain(ngx_output_chain_ctx_t *ctx, ngx_chain_t *ਵਿੱਚ)
{
    off_t         bsize;
    ngx_int_t     rc, ਆਖਰੀ;
    ngx_chain_t  *cl, *ਬਾਹਰਲਾ, **ਆਖਰੀ_ਬਾਹਰਲਾ;

    ਜੇ (ctx->ਵਿੱਚ == ਨਿਰਾ && ctx->busy == ਨਿਰਾ
#ਜੇ (NGX_HAVE_FILE_AIO || NGX_ਧਾਗੇ)
        && !ctx->aio
#ਜੇ_ਅੰਤ
       )
    {
        /*
         * the short path for the case when the ctx->in and ctx->busy chains
         * are empty, the incoming chain is empty too or has the single buf
         * that does not require the copy
         */

        ਜੇ (ਵਿੱਚ == ਨਿਰਾ) {
            ਵਾਪਸ ctx->output_filter(ctx->filter_ctx, ਵਿੱਚ);
        }

        ਜੇ (ਵਿੱਚ->ਅਗਲਾ == ਨਿਰਾ
#ਜੇ (NGX_SENDFILE_ਹੱਦਬੰਦੀ)
            && !(ਵਿੱਚ->buf->ਵਿੱਚ_file && ਵਿੱਚ->buf->file_ਆਖਰੀ > NGX_SENDFILE_ਹੱਦਬੰਦੀ)
#ਜੇ_ਅੰਤ
            && ngx_output_chain_as_is(ctx, ਵਿੱਚ->buf))
        {
            ਵਾਪਸ ctx->output_filter(ctx->filter_ctx, ਵਿੱਚ);
        }
    }

    /* add the incoming buf to the chain ctx->in */

    ਜੇ (ਵਿੱਚ) {
        ਜੇ (ngx_output_chain_ਜੋੜੋ_copy(ctx->pool, &ctx->ਵਿੱਚ, ਵਿੱਚ) == NGX_ERROR) {
            ਵਾਪਸ NGX_ERROR;
        }
    }

    ਬਾਹਰਲਾ = ਨਿਰਾ;
    ਆਖਰੀ_ਬਾਹਰਲਾ = &ਬਾਹਰਲਾ;
    ਆਖਰੀ = NGX_NONE;

    ਲਈ ( ;; ) {

#ਜੇ (NGX_HAVE_FILE_AIO || NGX_ਧਾਗੇ)
        ਜੇ (ctx->aio) {
            ਵਾਪਸ NGX_AGAIN;
        }
#ਜੇ_ਅੰਤ

        ਜਦੋਂ (ctx->ਵਿੱਚ) {

            /*
             * cycle while there are the ctx->in bufs
             * and there are the free output bufs to copy in
             */

            bsize = ngx_buf_ਮਾਪ(ctx->ਵਿੱਚ->buf);

            ਜੇ (bsize == 0 && !ngx_buf_special(ctx->ਵਿੱਚ->buf)) {

                ngx_log_error(NGX_LOG_ALERT, ctx->pool->ਲਘੂਗਣਕ, 0,
                              "zero size buf in output "
                              "t:%d r:%d f:%d %p %p-%p %p %O-%O",
                              ctx->ਵਿੱਚ->buf->temporary,
                              ctx->ਵਿੱਚ->buf->recycled,
                              ctx->ਵਿੱਚ->buf->ਵਿੱਚ_file,
                              ctx->ਵਿੱਚ->buf->ਸ਼ੁਰੂ,
                              ctx->ਵਿੱਚ->buf->pos,
                              ctx->ਵਿੱਚ->buf->ਆਖਰੀ,
                              ctx->ਵਿੱਚ->buf->file,
                              ctx->ਵਿੱਚ->buf->file_pos,
                              ctx->ਵਿੱਚ->buf->file_ਆਖਰੀ);

                ngx_debug_ਬਿੰਦੂ();

                cl = ctx->ਵਿੱਚ;
                ctx->ਵਿੱਚ = cl->ਅਗਲਾ;

                ngx_free_chain(ctx->pool, cl);

                ਜਾਰੀ;
            }

            ਜੇ (bsize < 0) {

                ngx_log_error(NGX_LOG_ALERT, ctx->pool->ਲਘੂਗਣਕ, 0,
                              "negative size buf in output "
                              "t:%d r:%d f:%d %p %p-%p %p %O-%O",
                              ctx->ਵਿੱਚ->buf->temporary,
                              ctx->ਵਿੱਚ->buf->recycled,
                              ctx->ਵਿੱਚ->buf->ਵਿੱਚ_file,
                              ctx->ਵਿੱਚ->buf->ਸ਼ੁਰੂ,
                              ctx->ਵਿੱਚ->buf->pos,
                              ctx->ਵਿੱਚ->buf->ਆਖਰੀ,
                              ctx->ਵਿੱਚ->buf->file,
                              ctx->ਵਿੱਚ->buf->file_pos,
                              ctx->ਵਿੱਚ->buf->file_ਆਖਰੀ);

                ngx_debug_ਬਿੰਦੂ();

                ਵਾਪਸ NGX_ERROR;
            }

            ਜੇ (ngx_output_chain_as_is(ctx, ctx->ਵਿੱਚ->buf)) {

                /* move the chain link to the output chain */

                cl = ctx->ਵਿੱਚ;
                ctx->ਵਿੱਚ = cl->ਅਗਲਾ;

                *ਆਖਰੀ_ਬਾਹਰਲਾ = cl;
                ਆਖਰੀ_ਬਾਹਰਲਾ = &cl->ਅਗਲਾ;
                cl->ਅਗਲਾ = ਨਿਰਾ;

                ਜਾਰੀ;
            }

            ਜੇ (ctx->buf == ਨਿਰਾ) {

                rc = ngx_output_chain_align_file_buf(ctx, bsize);

                ਜੇ (rc == NGX_ERROR) {
                    ਵਾਪਸ NGX_ERROR;
                }

                ਜੇ (rc != NGX_OK) {

                    ਜੇ (ctx->ਮੁਕਤ) {

                        /* get the free buf */

                        cl = ctx->ਮੁਕਤ;
                        ctx->buf = cl->buf;
                        ctx->ਮੁਕਤ = cl->ਅਗਲਾ;

                        ngx_free_chain(ctx->pool, cl);

                    } ਨਹੀਂਤਾਂ ਜੇ (ਬਾਹਰਲਾ || ctx->allocated == ctx->bufs.ਅੰਕ) {

                        ਰੋਕੋ;

                    } ਨਹੀਂਤਾਂ ਜੇ (ngx_output_chain_get_buf(ctx, bsize) != NGX_OK) {
                        ਵਾਪਸ NGX_ERROR;
                    }
                }
            }

            rc = ngx_output_chain_copy_buf(ctx);

            ਜੇ (rc == NGX_ERROR) {
                ਵਾਪਸ rc;
            }

            ਜੇ (rc == NGX_AGAIN) {
                ਜੇ (ਬਾਹਰਲਾ) {
                    ਰੋਕੋ;
                }

                ਵਾਪਸ rc;
            }

            /* delete the completed buf from the ctx->in chain */

            ਜੇ (ngx_buf_ਮਾਪ(ctx->ਵਿੱਚ->buf) == 0) {
                cl = ctx->ਵਿੱਚ;
                ctx->ਵਿੱਚ = cl->ਅਗਲਾ;

                ngx_free_chain(ctx->pool, cl);
            }

            cl = ngx_alloc_chain_link(ctx->pool);
            ਜੇ (cl == ਨਿਰਾ) {
                ਵਾਪਸ NGX_ERROR;
            }

            cl->buf = ctx->buf;
            cl->ਅਗਲਾ = ਨਿਰਾ;
            *ਆਖਰੀ_ਬਾਹਰਲਾ = cl;
            ਆਖਰੀ_ਬਾਹਰਲਾ = &cl->ਅਗਲਾ;
            ctx->buf = ਨਿਰਾ;
        }

        ਜੇ (ਬਾਹਰਲਾ == ਨਿਰਾ && ਆਖਰੀ != NGX_NONE) {

            ਜੇ (ctx->ਵਿੱਚ) {
                ਵਾਪਸ NGX_AGAIN;
            }

            ਵਾਪਸ ਆਖਰੀ;
        }

        ਆਖਰੀ = ctx->output_filter(ctx->filter_ctx, ਬਾਹਰਲਾ);

        ਜੇ (ਆਖਰੀ == NGX_ERROR || ਆਖਰੀ == NGX_ਪੂਰਾ) {
            ਵਾਪਸ ਆਖਰੀ;
        }

        ngx_chain_update_chains(ctx->pool, &ctx->ਮੁਕਤ, &ctx->busy, &ਬਾਹਰਲਾ,
                                ctx->ਟੈਗ);
        ਆਖਰੀ_ਬਾਹਰਲਾ = &ਬਾਹਰਲਾ;
    }
}


ਸਥਿਰ ngx_inline ngx_int_t
ngx_output_chain_as_is(ngx_output_chain_ctx_t *ctx, ngx_buf_t *buf)
{
    ngx_uint_t  sendfile;

    ਜੇ (ngx_buf_special(buf)) {
        ਵਾਪਸ 1;
    }

#ਜੇ (NGX_ਧਾਗੇ)
    ਜੇ (buf->ਵਿੱਚ_file) {
        buf->file->thread_handler = ctx->thread_handler;
        buf->file->thread_ctx = ctx->filter_ctx;
    }
#ਜੇ_ਅੰਤ

    sendfile = ctx->sendfile;

#ਜੇ (NGX_SENDFILE_ਹੱਦਬੰਦੀ)

    ਜੇ (buf->ਵਿੱਚ_file && buf->file_pos >= NGX_SENDFILE_ਹੱਦਬੰਦੀ) {
        sendfile = 0;
    }

#ਜੇ_ਅੰਤ

#ਜੇ !(NGX_HAVE_SENDFILE_NODISKIO)

    /*
     * With DIRECTIO, disable sendfile() unless sendfile(SF_NOCACHE)
     * is available.
     */

    ਜੇ (buf->ਵਿੱਚ_file && buf->file->directio) {
        sendfile = 0;
    }

#ਜੇ_ਅੰਤ

    ਜੇ (!sendfile) {

        ਜੇ (!ngx_buf_ਵਿੱਚ_ਸਮ੍ਰਿਤੀ(buf)) {
            ਵਾਪਸ 0;
        }

        buf->ਵਿੱਚ_file = 0;
    }

    ਜੇ (ctx->need_ਵਿੱਚ_ਸਮ੍ਰਿਤੀ && !ngx_buf_ਵਿੱਚ_ਸਮ੍ਰਿਤੀ(buf)) {
        ਵਾਪਸ 0;
    }

    ਜੇ (ctx->need_ਵਿੱਚ_ਆਰਜ਼ੀ && (buf->ਸਮ੍ਰਿਤੀ || buf->mmap)) {
        ਵਾਪਸ 0;
    }

    ਵਾਪਸ 1;
}


ਸਥਿਰ ngx_int_t
ngx_output_chain_ਜੋੜੋ_copy(ngx_pool_t *pool, ngx_chain_t **chain,
    ngx_chain_t *ਵਿੱਚ)
{
    ngx_chain_t  *cl, **ll;
#ਜੇ (NGX_SENDFILE_ਹੱਦਬੰਦੀ)
    ngx_buf_t    *b, *buf;
#ਜੇ_ਅੰਤ

    ll = chain;

    ਲਈ (cl = *chain; cl; cl = cl->ਅਗਲਾ) {
        ll = &cl->ਅਗਲਾ;
    }

    ਜਦੋਂ (ਵਿੱਚ) {

        cl = ngx_alloc_chain_link(pool);
        ਜੇ (cl == ਨਿਰਾ) {
            ਵਾਪਸ NGX_ERROR;
        }

#ਜੇ (NGX_SENDFILE_ਹੱਦਬੰਦੀ)

        buf = ਵਿੱਚ->buf;

        ਜੇ (buf->ਵਿੱਚ_file
            && buf->file_pos < NGX_SENDFILE_ਹੱਦਬੰਦੀ
            && buf->file_ਆਖਰੀ > NGX_SENDFILE_ਹੱਦਬੰਦੀ)
        {
            /* split a file buf on two bufs by the sendfile limit */

            b = ngx_calloc_buf(pool);
            ਜੇ (b == ਨਿਰਾ) {
                ਵਾਪਸ NGX_ERROR;
            }

            ngx_memcpy(b, buf, ਆਕਾਰ(ngx_buf_t));

            ਜੇ (ngx_buf_ਵਿੱਚ_ਸਮ੍ਰਿਤੀ(buf)) {
                buf->pos += (ssize_t) (NGX_SENDFILE_ਹੱਦਬੰਦੀ - buf->file_pos);
                b->ਆਖਰੀ = buf->pos;
            }

            buf->file_pos = NGX_SENDFILE_ਹੱਦਬੰਦੀ;
            b->file_ਆਖਰੀ = NGX_SENDFILE_ਹੱਦਬੰਦੀ;

            cl->buf = b;

        } ਨਹੀਂਤਾਂ {
            cl->buf = buf;
            ਵਿੱਚ = ਵਿੱਚ->ਅਗਲਾ;
        }

#ਨਹੀਂਤਾਂ
        cl->buf = ਵਿੱਚ->buf;
        ਵਿੱਚ = ਵਿੱਚ->ਅਗਲਾ;

#ਜੇ_ਅੰਤ

        cl->ਅਗਲਾ = ਨਿਰਾ;
        *ll = cl;
        ll = &cl->ਅਗਲਾ;
    }

    ਵਾਪਸ NGX_OK;
}


ਸਥਿਰ ngx_int_t
ngx_output_chain_align_file_buf(ngx_output_chain_ctx_t *ctx, off_t bsize)
{
    ਮਾਪ_t      ਮਾਪ;
    ngx_buf_t  *ਵਿੱਚ;

    ਵਿੱਚ = ctx->ਵਿੱਚ->buf;

    ਜੇ (ਵਿੱਚ->file == ਨਿਰਾ || !ਵਿੱਚ->file->directio) {
        ਵਾਪਸ NGX_DECLINED;
    }

    ctx->directio = 1;

    ਮਾਪ = (ਮਾਪ_t) (ਵਿੱਚ->file_pos - (ਵਿੱਚ->file_pos & ~(ctx->alignment - 1)));

    ਜੇ (ਮਾਪ == 0) {

        ਜੇ (bsize >= (off_t) ctx->bufs.ਮਾਪ) {
            ਵਾਪਸ NGX_DECLINED;
        }

        ਮਾਪ = (ਮਾਪ_t) bsize;

    } ਨਹੀਂਤਾਂ {
        ਮਾਪ = (ਮਾਪ_t) ctx->alignment - ਮਾਪ;

        ਜੇ ((off_t) ਮਾਪ > bsize) {
            ਮਾਪ = (ਮਾਪ_t) bsize;
        }
    }

    ctx->buf = ngx_create_ਆਰਜ਼ੀ_buf(ctx->pool, ਮਾਪ);
    ਜੇ (ctx->buf == ਨਿਰਾ) {
        ਵਾਪਸ NGX_ERROR;
    }

    /*
     * we do not set ctx->buf->tag, because we do not want
     * to reuse the buf via ctx->free list
     */

#ਜੇ (NGX_HAVE_ALIGNED_DIRECTIO)
    ctx->unaligned = 1;
#ਜੇ_ਅੰਤ

    ਵਾਪਸ NGX_OK;
}


ਸਥਿਰ ngx_int_t
ngx_output_chain_get_buf(ngx_output_chain_ctx_t *ctx, off_t bsize)
{
    ਮਾਪ_t       ਮਾਪ;
    ngx_buf_t   *b, *ਵਿੱਚ;
    ngx_uint_t   recycled;

    ਵਿੱਚ = ctx->ਵਿੱਚ->buf;
    ਮਾਪ = ctx->bufs.ਮਾਪ;
    recycled = 1;

    ਜੇ (ਵਿੱਚ->ਆਖਰੀ_ਵਿੱਚ_chain) {

        ਜੇ (bsize < (off_t) ਮਾਪ) {

            /*
             * allocate a small temp buf for a small last buf
             * or its small last part
             */

            ਮਾਪ = (ਮਾਪ_t) bsize;
            recycled = 0;

        } ਨਹੀਂਤਾਂ ਜੇ (!ctx->directio
                   && ctx->bufs.ਅੰਕ == 1
                   && (bsize < (off_t) (ਮਾਪ + ਮਾਪ / 4)))
        {
            /*
             * allocate a temp buf that equals to a last buf,
             * if there is no directio, the last buf size is lesser
             * than 1.25 of bufs.size and the temp buf is single
             */

            ਮਾਪ = (ਮਾਪ_t) bsize;
            recycled = 0;
        }
    }

    b = ngx_calloc_buf(ctx->pool);
    ਜੇ (b == ਨਿਰਾ) {
        ਵਾਪਸ NGX_ERROR;
    }

    ਜੇ (ctx->directio) {

        /*
         * allocate block aligned to a disk sector size to enable
         * userland buffer direct usage conjunctly with directio
         */

        b->ਸ਼ੁਰੂ = ngx_pmemalign(ctx->pool, ਮਾਪ, (ਮਾਪ_t) ctx->alignment);
        ਜੇ (b->ਸ਼ੁਰੂ == ਨਿਰਾ) {
            ਵਾਪਸ NGX_ERROR;
        }

    } ਨਹੀਂਤਾਂ {
        b->ਸ਼ੁਰੂ = ngx_palloc(ctx->pool, ਮਾਪ);
        ਜੇ (b->ਸ਼ੁਰੂ == ਨਿਰਾ) {
            ਵਾਪਸ NGX_ERROR;
        }
    }

    b->pos = b->ਸ਼ੁਰੂ;
    b->ਆਖਰੀ = b->ਸ਼ੁਰੂ;
    b->ਸਿਰਾ = b->ਆਖਰੀ + ਮਾਪ;
    b->temporary = 1;
    b->ਟੈਗ = ctx->ਟੈਗ;
    b->recycled = recycled;

    ctx->buf = b;
    ctx->allocated++;

    ਵਾਪਸ NGX_OK;
}


ਸਥਿਰ ngx_int_t
ngx_output_chain_copy_buf(ngx_output_chain_ctx_t *ctx)
{
    off_t        ਮਾਪ;
    ssize_t      n;
    ngx_buf_t   *src, *dst;
    ngx_uint_t   sendfile;

    src = ctx->ਵਿੱਚ->buf;
    dst = ctx->buf;

    ਮਾਪ = ngx_buf_ਮਾਪ(src);
    ਮਾਪ = ngx_min(ਮਾਪ, dst->ਸਿਰਾ - dst->pos);

    sendfile = ctx->sendfile && !ctx->directio;

#ਜੇ (NGX_SENDFILE_ਹੱਦਬੰਦੀ)

    ਜੇ (src->ਵਿੱਚ_file && src->file_pos >= NGX_SENDFILE_ਹੱਦਬੰਦੀ) {
        sendfile = 0;
    }

#ਜੇ_ਅੰਤ

    ਜੇ (ngx_buf_ਵਿੱਚ_ਸਮ੍ਰਿਤੀ(src)) {
        ngx_memcpy(dst->pos, src->pos, (ਮਾਪ_t) ਮਾਪ);
        src->pos += (ਮਾਪ_t) ਮਾਪ;
        dst->ਆਖਰੀ += (ਮਾਪ_t) ਮਾਪ;

        ਜੇ (src->ਵਿੱਚ_file) {

            ਜੇ (sendfile) {
                dst->ਵਿੱਚ_file = 1;
                dst->file = src->file;
                dst->file_pos = src->file_pos;
                dst->file_ਆਖਰੀ = src->file_pos + ਮਾਪ;

            } ਨਹੀਂਤਾਂ {
                dst->ਵਿੱਚ_file = 0;
            }

            src->file_pos += ਮਾਪ;

        } ਨਹੀਂਤਾਂ {
            dst->ਵਿੱਚ_file = 0;
        }

        ਜੇ (src->pos == src->ਆਖਰੀ) {
            dst->flush = src->flush;
            dst->ਆਖਰੀ_buf = src->ਆਖਰੀ_buf;
            dst->ਆਖਰੀ_ਵਿੱਚ_chain = src->ਆਖਰੀ_ਵਿੱਚ_chain;

        } ਨਹੀਂਤਾਂ {
            dst->flush = 0;
            dst->ਆਖਰੀ_buf = 0;
            dst->ਆਖਰੀ_ਵਿੱਚ_chain = 0;
        }

    } ਨਹੀਂਤਾਂ {

#ਜੇ (NGX_HAVE_ALIGNED_DIRECTIO)

        ਜੇ (ctx->unaligned) {
            ਜੇ (ngx_directio_off(src->file->fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, ctx->pool->ਲਘੂਗਣਕ, ngx_errno,
                              ngx_directio_off_n " \"%s\" failed",
                              src->file->name.ਅੰਕੜੇ);
            }
        }

#ਜੇ_ਅੰਤ

#ਜੇ (NGX_HAVE_FILE_AIO)
        ਜੇ (ctx->aio_handler) {
            n = ngx_file_aio_read(src->file, dst->pos, (ਮਾਪ_t) ਮਾਪ,
                                  src->file_pos, ctx->pool);
            ਜੇ (n == NGX_AGAIN) {
                ctx->aio_handler(ctx, src->file);
                ਵਾਪਸ NGX_AGAIN;
            }

        } ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
#ਜੇ (NGX_ਧਾਗੇ)
        ਜੇ (ctx->thread_handler) {
            src->file->thread_task = ctx->thread_task;
            src->file->thread_handler = ctx->thread_handler;
            src->file->thread_ctx = ctx->filter_ctx;

            n = ngx_thread_read(src->file, dst->pos, (ਮਾਪ_t) ਮਾਪ,
                                src->file_pos, ctx->pool);
            ਜੇ (n == NGX_AGAIN) {
                ctx->thread_task = src->file->thread_task;
                ਵਾਪਸ NGX_AGAIN;
            }

        } ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
        {
            n = ngx_read_file(src->file, dst->pos, (ਮਾਪ_t) ਮਾਪ,
                              src->file_pos);
        }

#ਜੇ (NGX_HAVE_ALIGNED_DIRECTIO)

        ਜੇ (ctx->unaligned) {
            ngx_err_t  err;

            err = ngx_errno;

            ਜੇ (ngx_directio_on(src->file->fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, ctx->pool->ਲਘੂਗਣਕ, ngx_errno,
                              ngx_directio_on_n " \"%s\" failed",
                              src->file->name.ਅੰਕੜੇ);
            }

            ngx_ਸੈੱਟ_errno(err);

            ctx->unaligned = 0;
        }

#ਜੇ_ਅੰਤ

        ਜੇ (n == NGX_ERROR) {
            ਵਾਪਸ (ngx_int_t) n;
        }

        ਜੇ (n != ਮਾਪ) {
            ngx_log_error(NGX_LOG_ALERT, ctx->pool->ਲਘੂਗਣਕ, 0,
                          ngx_read_file_n " read only %z of %O from \"%s\"",
                          n, ਮਾਪ, src->file->name.ਅੰਕੜੇ);
            ਵਾਪਸ NGX_ERROR;
        }

        dst->ਆਖਰੀ += n;

        ਜੇ (sendfile) {
            dst->ਵਿੱਚ_file = 1;
            dst->file = src->file;
            dst->file_pos = src->file_pos;
            dst->file_ਆਖਰੀ = src->file_pos + n;

        } ਨਹੀਂਤਾਂ {
            dst->ਵਿੱਚ_file = 0;
        }

        src->file_pos += n;

        ਜੇ (src->file_pos == src->file_ਆਖਰੀ) {
            dst->flush = src->flush;
            dst->ਆਖਰੀ_buf = src->ਆਖਰੀ_buf;
            dst->ਆਖਰੀ_ਵਿੱਚ_chain = src->ਆਖਰੀ_ਵਿੱਚ_chain;

        } ਨਹੀਂਤਾਂ {
            dst->flush = 0;
            dst->ਆਖਰੀ_buf = 0;
            dst->ਆਖਰੀ_ਵਿੱਚ_chain = 0;
        }
    }

    ਵਾਪਸ NGX_OK;
}


ngx_int_t
ngx_chain_writer(ਖਾਲੀ *ਅੰਕੜੇ, ngx_chain_t *ਵਿੱਚ)
{
    ngx_chain_writer_ctx_t *ctx = ਅੰਕੜੇ;

    off_t              ਮਾਪ;
    ngx_chain_t       *cl, *ln, *chain;
    ngx_connection_t  *c;

    c = ctx->connection;

    ਲਈ (ਮਾਪ = 0; ਵਿੱਚ; ਵਿੱਚ = ਵਿੱਚ->ਅਗਲਾ) {

        ਜੇ (ngx_buf_ਮਾਪ(ਵਿੱਚ->buf) == 0 && !ngx_buf_special(ਵਿੱਚ->buf)) {

            ngx_log_error(NGX_LOG_ALERT, ctx->pool->ਲਘੂਗਣਕ, 0,
                          "zero size buf in chain writer "
                          "t:%d r:%d f:%d %p %p-%p %p %O-%O",
                          ਵਿੱਚ->buf->temporary,
                          ਵਿੱਚ->buf->recycled,
                          ਵਿੱਚ->buf->ਵਿੱਚ_file,
                          ਵਿੱਚ->buf->ਸ਼ੁਰੂ,
                          ਵਿੱਚ->buf->pos,
                          ਵਿੱਚ->buf->ਆਖਰੀ,
                          ਵਿੱਚ->buf->file,
                          ਵਿੱਚ->buf->file_pos,
                          ਵਿੱਚ->buf->file_ਆਖਰੀ);

            ngx_debug_ਬਿੰਦੂ();

            ਜਾਰੀ;
        }

        ਜੇ (ngx_buf_ਮਾਪ(ਵਿੱਚ->buf) < 0) {

            ngx_log_error(NGX_LOG_ALERT, ctx->pool->ਲਘੂਗਣਕ, 0,
                          "negative size buf in chain writer "
                          "t:%d r:%d f:%d %p %p-%p %p %O-%O",
                          ਵਿੱਚ->buf->temporary,
                          ਵਿੱਚ->buf->recycled,
                          ਵਿੱਚ->buf->ਵਿੱਚ_file,
                          ਵਿੱਚ->buf->ਸ਼ੁਰੂ,
                          ਵਿੱਚ->buf->pos,
                          ਵਿੱਚ->buf->ਆਖਰੀ,
                          ਵਿੱਚ->buf->file,
                          ਵਿੱਚ->buf->file_pos,
                          ਵਿੱਚ->buf->file_ਆਖਰੀ);

            ngx_debug_ਬਿੰਦੂ();

            ਵਾਪਸ NGX_ERROR;
        }

        ਮਾਪ += ngx_buf_ਮਾਪ(ਵਿੱਚ->buf);

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, c->ਲਘੂਗਣਕ, 0,
                       "chain writer buf fl:%d s:%uO",
                       ਵਿੱਚ->buf->flush, ngx_buf_ਮਾਪ(ਵਿੱਚ->buf));

        cl = ngx_alloc_chain_link(ctx->pool);
        ਜੇ (cl == ਨਿਰਾ) {
            ਵਾਪਸ NGX_ERROR;
        }

        cl->buf = ਵਿੱਚ->buf;
        cl->ਅਗਲਾ = ਨਿਰਾ;
        *ctx->ਆਖਰੀ = cl;
        ctx->ਆਖਰੀ = &cl->ਅਗਲਾ;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->ਲਘੂਗਣਕ, 0,
                   "chain writer in: %p", ctx->ਬਾਹਰਲਾ);

    ਲਈ (cl = ctx->ਬਾਹਰਲਾ; cl; cl = cl->ਅਗਲਾ) {

        ਜੇ (ngx_buf_ਮਾਪ(cl->buf) == 0 && !ngx_buf_special(cl->buf)) {

            ngx_log_error(NGX_LOG_ALERT, ctx->pool->ਲਘੂਗਣਕ, 0,
                          "zero size buf in chain writer "
                          "t:%d r:%d f:%d %p %p-%p %p %O-%O",
                          cl->buf->temporary,
                          cl->buf->recycled,
                          cl->buf->ਵਿੱਚ_file,
                          cl->buf->ਸ਼ੁਰੂ,
                          cl->buf->pos,
                          cl->buf->ਆਖਰੀ,
                          cl->buf->file,
                          cl->buf->file_pos,
                          cl->buf->file_ਆਖਰੀ);

            ngx_debug_ਬਿੰਦੂ();

            ਜਾਰੀ;
        }

        ਜੇ (ngx_buf_ਮਾਪ(cl->buf) < 0) {

            ngx_log_error(NGX_LOG_ALERT, ctx->pool->ਲਘੂਗਣਕ, 0,
                          "negative size buf in chain writer "
                          "t:%d r:%d f:%d %p %p-%p %p %O-%O",
                          cl->buf->temporary,
                          cl->buf->recycled,
                          cl->buf->ਵਿੱਚ_file,
                          cl->buf->ਸ਼ੁਰੂ,
                          cl->buf->pos,
                          cl->buf->ਆਖਰੀ,
                          cl->buf->file,
                          cl->buf->file_pos,
                          cl->buf->file_ਆਖਰੀ);

            ngx_debug_ਬਿੰਦੂ();

            ਵਾਪਸ NGX_ERROR;
        }

        ਮਾਪ += ngx_buf_ਮਾਪ(cl->buf);
    }

    ਜੇ (ਮਾਪ == 0 && !c->buffered) {
        ਵਾਪਸ NGX_OK;
    }

    chain = c->send_chain(c, ctx->ਬਾਹਰਲਾ, ctx->ਹੱਦਬੰਦੀ);

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->ਲਘੂਗਣਕ, 0,
                   "chain writer out: %p", chain);

    ਜੇ (chain == NGX_CHAIN_ERROR) {
        ਵਾਪਸ NGX_ERROR;
    }

    ਜੇ (chain && c->ਲਿਖ->ready) {
        ngx_post_event(c->ਲਿਖ, &ngx_posted_ਅਗਲਾ_events);
    }

    ਲਈ (cl = ctx->ਬਾਹਰਲਾ; cl && cl != chain; /* void */) {
        ln = cl;
        cl = cl->ਅਗਲਾ;
        ngx_free_chain(ctx->pool, ln);
    }

    ctx->ਬਾਹਰਲਾ = chain;

    ਜੇ (ctx->ਬਾਹਰਲਾ == ਨਿਰਾ) {
        ctx->ਆਖਰੀ = &ctx->ਬਾਹਰਲਾ;

        ਜੇ (!c->buffered) {
            ਵਾਪਸ NGX_OK;
        }
    }

    ਵਾਪਸ NGX_AGAIN;
}
