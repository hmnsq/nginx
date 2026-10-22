
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>
#ಅನ್ತರ್ಭಾವ <ngx_event.h>


#ಆದರೆ 0
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_SENDFILE_ಎಲ್ಲೆ  4096
#ಆದರೆ_ಅಂತ್ಯ

/*
 * When DIRECTIO is enabled FreeBSD, Solaris, and MacOSX read directly
 * to an application memory from a device if parameters are aligned
 * to device sector boundary (512 bytes).  They fallback to usual read
 * operation if the parameters are not aligned.
 * Linux allows DIRECTIO only if the parameters are aligned to a filesystem
 * sector boundary, otherwise it returns EINVAL.  The sector size is
 * usually 512 bytes, however, on XFS it may be 4096 bytes.
 */

#ವ್ಯಾಖ್ಯಾನಿಸು NGX_NONE            1


ಸ್ಥಿರ ngx_inline ngx_int_t
    ngx_output_chain_as_is(ngx_output_chain_ctx_t *ctx, ngx_buf_t *buf);
ಸ್ಥಿರ ngx_int_t ngx_output_chain_ಕೂಡಿಸು_copy(ngx_pool_t *pool,
    ngx_chain_t **chain, ngx_chain_t *ಒಳ);
ಸ್ಥಿರ ngx_int_t ngx_output_chain_align_file_buf(ngx_output_chain_ctx_t *ctx,
    off_t bsize);
ಸ್ಥಿರ ngx_int_t ngx_output_chain_get_buf(ngx_output_chain_ctx_t *ctx,
    off_t bsize);
ಸ್ಥಿರ ngx_int_t ngx_output_chain_copy_buf(ngx_output_chain_ctx_t *ctx);


ngx_int_t
ngx_output_chain(ngx_output_chain_ctx_t *ctx, ngx_chain_t *ಒಳ)
{
    off_t         bsize;
    ngx_int_t     rc, ಕೊನೆಯ;
    ngx_chain_t  *cl, *ಹೊರಹೋಗು, **ಕೊನೆಯ_ಹೊರಹೋಗು;

    ಆದರೆ (ctx->ಒಳ == ಖಾಲಿ && ctx->busy == ಖಾಲಿ
#ಆದರೆ (NGX_HAVE_FILE_AIO || NGX_ಎಳೆಗಳು)
        && !ctx->aio
#ಆದರೆ_ಅಂತ್ಯ
       )
    {
        /*
         * the short path for the case when the ctx->in and ctx->busy chains
         * are empty, the incoming chain is empty too or has the single buf
         * that does not require the copy
         */

        ಆದರೆ (ಒಳ == ಖಾಲಿ) {
            ಹಿಂತಿರುಗಿ ctx->output_filter(ctx->filter_ctx, ಒಳ);
        }

        ಆದರೆ (ಒಳ->ಮುಂದಿನ == ಖಾಲಿ
#ಆದರೆ (NGX_SENDFILE_ಎಲ್ಲೆ)
            && !(ಒಳ->buf->ಒಳ_file && ಒಳ->buf->file_ಕೊನೆಯ > NGX_SENDFILE_ಎಲ್ಲೆ)
#ಆದರೆ_ಅಂತ್ಯ
            && ngx_output_chain_as_is(ctx, ಒಳ->buf))
        {
            ಹಿಂತಿರುಗಿ ctx->output_filter(ctx->filter_ctx, ಒಳ);
        }
    }

    /* add the incoming buf to the chain ctx->in */

    ಆದರೆ (ಒಳ) {
        ಆದರೆ (ngx_output_chain_ಕೂಡಿಸು_copy(ctx->pool, &ctx->ಒಳ, ಒಳ) == NGX_ERROR) {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }
    }

    ಹೊರಹೋಗು = ಖಾಲಿ;
    ಕೊನೆಯ_ಹೊರಹೋಗು = &ಹೊರಹೋಗು;
    ಕೊನೆಯ = NGX_NONE;

    ಗಾಗಿ ( ;; ) {

#ಆದರೆ (NGX_HAVE_FILE_AIO || NGX_ಎಳೆಗಳು)
        ಆದರೆ (ctx->aio) {
            ಹಿಂತಿರುಗಿ NGX_AGAIN;
        }
#ಆದರೆ_ಅಂತ್ಯ

        ಆಗ (ctx->ಒಳ) {

            /*
             * cycle while there are the ctx->in bufs
             * and there are the free output bufs to copy in
             */

            bsize = ngx_buf_ಗಾತ್ರ(ctx->ಒಳ->buf);

            ಆದರೆ (bsize == 0 && !ngx_buf_special(ctx->ಒಳ->buf)) {

                ngx_log_error(NGX_LOG_ALERT, ctx->pool->ಲಘುಗಣಕ, 0,
                              "zero size buf in output "
                              "t:%d r:%d f:%d %p %p-%p %p %O-%O",
                              ctx->ಒಳ->buf->temporary,
                              ctx->ಒಳ->buf->recycled,
                              ctx->ಒಳ->buf->ಒಳ_file,
                              ctx->ಒಳ->buf->ಆರಂಭ,
                              ctx->ಒಳ->buf->pos,
                              ctx->ಒಳ->buf->ಕೊನೆಯ,
                              ctx->ಒಳ->buf->file,
                              ctx->ಒಳ->buf->file_pos,
                              ctx->ಒಳ->buf->file_ಕೊನೆಯ);

                ngx_debug_ಬಿಂದು();

                cl = ctx->ಒಳ;
                ctx->ಒಳ = cl->ಮುಂದಿನ;

                ngx_free_chain(ctx->pool, cl);

                ಮುಂದುವರಿ;
            }

            ಆದರೆ (bsize < 0) {

                ngx_log_error(NGX_LOG_ALERT, ctx->pool->ಲಘುಗಣಕ, 0,
                              "negative size buf in output "
                              "t:%d r:%d f:%d %p %p-%p %p %O-%O",
                              ctx->ಒಳ->buf->temporary,
                              ctx->ಒಳ->buf->recycled,
                              ctx->ಒಳ->buf->ಒಳ_file,
                              ctx->ಒಳ->buf->ಆರಂಭ,
                              ctx->ಒಳ->buf->pos,
                              ctx->ಒಳ->buf->ಕೊನೆಯ,
                              ctx->ಒಳ->buf->file,
                              ctx->ಒಳ->buf->file_pos,
                              ctx->ಒಳ->buf->file_ಕೊನೆಯ);

                ngx_debug_ಬಿಂದು();

                ಹಿಂತಿರುಗಿ NGX_ERROR;
            }

            ಆದರೆ (ngx_output_chain_as_is(ctx, ctx->ಒಳ->buf)) {

                /* move the chain link to the output chain */

                cl = ctx->ಒಳ;
                ctx->ಒಳ = cl->ಮುಂದಿನ;

                *ಕೊನೆಯ_ಹೊರಹೋಗು = cl;
                ಕೊನೆಯ_ಹೊರಹೋಗು = &cl->ಮುಂದಿನ;
                cl->ಮುಂದಿನ = ಖಾಲಿ;

                ಮುಂದುವರಿ;
            }

            ಆದರೆ (ctx->buf == ಖಾಲಿ) {

                rc = ngx_output_chain_align_file_buf(ctx, bsize);

                ಆದರೆ (rc == NGX_ERROR) {
                    ಹಿಂತಿರುಗಿ NGX_ERROR;
                }

                ಆದರೆ (rc != NGX_OK) {

                    ಆದರೆ (ctx->ಬಿಡುಗಡೆ) {

                        /* get the free buf */

                        cl = ctx->ಬಿಡುಗಡೆ;
                        ctx->buf = cl->buf;
                        ctx->ಬಿಡುಗಡೆ = cl->ಮುಂದಿನ;

                        ngx_free_chain(ctx->pool, cl);

                    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಹೊರಹೋಗು || ctx->allocated == ctx->bufs.ಅಂಕೆ) {

                        ನಿಲ್ಲಿಸು;

                    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ngx_output_chain_get_buf(ctx, bsize) != NGX_OK) {
                        ಹಿಂತಿರುಗಿ NGX_ERROR;
                    }
                }
            }

            rc = ngx_output_chain_copy_buf(ctx);

            ಆದರೆ (rc == NGX_ERROR) {
                ಹಿಂತಿರುಗಿ rc;
            }

            ಆದರೆ (rc == NGX_AGAIN) {
                ಆದರೆ (ಹೊರಹೋಗು) {
                    ನಿಲ್ಲಿಸು;
                }

                ಹಿಂತಿರುಗಿ rc;
            }

            /* delete the completed buf from the ctx->in chain */

            ಆದರೆ (ngx_buf_ಗಾತ್ರ(ctx->ಒಳ->buf) == 0) {
                cl = ctx->ಒಳ;
                ctx->ಒಳ = cl->ಮುಂದಿನ;

                ngx_free_chain(ctx->pool, cl);
            }

            cl = ngx_alloc_chain_link(ctx->pool);
            ಆದರೆ (cl == ಖಾಲಿ) {
                ಹಿಂತಿರುಗಿ NGX_ERROR;
            }

            cl->buf = ctx->buf;
            cl->ಮುಂದಿನ = ಖಾಲಿ;
            *ಕೊನೆಯ_ಹೊರಹೋಗು = cl;
            ಕೊನೆಯ_ಹೊರಹೋಗು = &cl->ಮುಂದಿನ;
            ctx->buf = ಖಾಲಿ;
        }

        ಆದರೆ (ಹೊರಹೋಗು == ಖಾಲಿ && ಕೊನೆಯ != NGX_NONE) {

            ಆದರೆ (ctx->ಒಳ) {
                ಹಿಂತಿರುಗಿ NGX_AGAIN;
            }

            ಹಿಂತಿರುಗಿ ಕೊನೆಯ;
        }

        ಕೊನೆಯ = ctx->output_filter(ctx->filter_ctx, ಹೊರಹೋಗು);

        ಆದರೆ (ಕೊನೆಯ == NGX_ERROR || ಕೊನೆಯ == NGX_ಮುಗಿದ) {
            ಹಿಂತಿರುಗಿ ಕೊನೆಯ;
        }

        ngx_chain_update_chains(ctx->pool, &ctx->ಬಿಡುಗಡೆ, &ctx->busy, &ಹೊರಹೋಗು,
                                ctx->ಟ್ಯಾಗ್);
        ಕೊನೆಯ_ಹೊರಹೋಗು = &ಹೊರಹೋಗು;
    }
}


ಸ್ಥಿರ ngx_inline ngx_int_t
ngx_output_chain_as_is(ngx_output_chain_ctx_t *ctx, ngx_buf_t *buf)
{
    ngx_uint_t  sendfile;

    ಆದರೆ (ngx_buf_special(buf)) {
        ಹಿಂತಿರುಗಿ 1;
    }

#ಆದರೆ (NGX_ಎಳೆಗಳು)
    ಆದರೆ (buf->ಒಳ_file) {
        buf->file->thread_handler = ctx->thread_handler;
        buf->file->thread_ctx = ctx->filter_ctx;
    }
#ಆದರೆ_ಅಂತ್ಯ

    sendfile = ctx->sendfile;

#ಆದರೆ (NGX_SENDFILE_ಎಲ್ಲೆ)

    ಆದರೆ (buf->ಒಳ_file && buf->file_pos >= NGX_SENDFILE_ಎಲ್ಲೆ) {
        sendfile = 0;
    }

#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ !(NGX_HAVE_SENDFILE_NODISKIO)

    /*
     * With DIRECTIO, disable sendfile() unless sendfile(SF_NOCACHE)
     * is available.
     */

    ಆದರೆ (buf->ಒಳ_file && buf->file->directio) {
        sendfile = 0;
    }

#ಆದರೆ_ಅಂತ್ಯ

    ಆದರೆ (!sendfile) {

        ಆದರೆ (!ngx_buf_ಒಳ_ಸ್ಮರಣೆ(buf)) {
            ಹಿಂತಿರುಗಿ 0;
        }

        buf->ಒಳ_file = 0;
    }

    ಆದರೆ (ctx->need_ಒಳ_ಸ್ಮರಣೆ && !ngx_buf_ಒಳ_ಸ್ಮರಣೆ(buf)) {
        ಹಿಂತಿರುಗಿ 0;
    }

    ಆದರೆ (ctx->need_ಒಳ_ತಾತ್ಕಾಲಿಕ && (buf->ಸ್ಮರಣೆ || buf->mmap)) {
        ಹಿಂತಿರುಗಿ 0;
    }

    ಹಿಂತಿರುಗಿ 1;
}


ಸ್ಥಿರ ngx_int_t
ngx_output_chain_ಕೂಡಿಸು_copy(ngx_pool_t *pool, ngx_chain_t **chain,
    ngx_chain_t *ಒಳ)
{
    ngx_chain_t  *cl, **ll;
#ಆದರೆ (NGX_SENDFILE_ಎಲ್ಲೆ)
    ngx_buf_t    *b, *buf;
#ಆದರೆ_ಅಂತ್ಯ

    ll = chain;

    ಗಾಗಿ (cl = *chain; cl; cl = cl->ಮುಂದಿನ) {
        ll = &cl->ಮುಂದಿನ;
    }

    ಆಗ (ಒಳ) {

        cl = ngx_alloc_chain_link(pool);
        ಆದರೆ (cl == ಖಾಲಿ) {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

#ಆದರೆ (NGX_SENDFILE_ಎಲ್ಲೆ)

        buf = ಒಳ->buf;

        ಆದರೆ (buf->ಒಳ_file
            && buf->file_pos < NGX_SENDFILE_ಎಲ್ಲೆ
            && buf->file_ಕೊನೆಯ > NGX_SENDFILE_ಎಲ್ಲೆ)
        {
            /* split a file buf on two bufs by the sendfile limit */

            b = ngx_calloc_buf(pool);
            ಆದರೆ (b == ಖಾಲಿ) {
                ಹಿಂತಿರುಗಿ NGX_ERROR;
            }

            ngx_memcpy(b, buf, ಗಾತ್ರ(ngx_buf_t));

            ಆದರೆ (ngx_buf_ಒಳ_ಸ್ಮರಣೆ(buf)) {
                buf->pos += (ssize_t) (NGX_SENDFILE_ಎಲ್ಲೆ - buf->file_pos);
                b->ಕೊನೆಯ = buf->pos;
            }

            buf->file_pos = NGX_SENDFILE_ಎಲ್ಲೆ;
            b->file_ಕೊನೆಯ = NGX_SENDFILE_ಎಲ್ಲೆ;

            cl->buf = b;

        } ಇಲ್ಲದಿದ್ದರೆ {
            cl->buf = buf;
            ಒಳ = ಒಳ->ಮುಂದಿನ;
        }

#ಇಲ್ಲದಿದ್ದರೆ
        cl->buf = ಒಳ->buf;
        ಒಳ = ಒಳ->ಮುಂದಿನ;

#ಆದರೆ_ಅಂತ್ಯ

        cl->ಮುಂದಿನ = ಖಾಲಿ;
        *ll = cl;
        ll = &cl->ಮುಂದಿನ;
    }

    ಹಿಂತಿರುಗಿ NGX_OK;
}


ಸ್ಥಿರ ngx_int_t
ngx_output_chain_align_file_buf(ngx_output_chain_ctx_t *ctx, off_t bsize)
{
    ಗಾತ್ರ_t      ಗಾತ್ರ_2;
    ngx_buf_t  *ಒಳ;

    ಒಳ = ctx->ಒಳ->buf;

    ಆದರೆ (ಒಳ->file == ಖಾಲಿ || !ಒಳ->file->directio) {
        ಹಿಂತಿರುಗಿ NGX_DECLINED;
    }

    ctx->directio = 1;

    ಗಾತ್ರ_2 = (ಗಾತ್ರ_t) (ಒಳ->file_pos - (ಒಳ->file_pos & ~(ctx->alignment - 1)));

    ಆದರೆ (ಗಾತ್ರ_2 == 0) {

        ಆದರೆ (bsize >= (off_t) ctx->bufs.ಗಾತ್ರ_2) {
            ಹಿಂತಿರುಗಿ NGX_DECLINED;
        }

        ಗಾತ್ರ_2 = (ಗಾತ್ರ_t) bsize;

    } ಇಲ್ಲದಿದ್ದರೆ {
        ಗಾತ್ರ_2 = (ಗಾತ್ರ_t) ctx->alignment - ಗಾತ್ರ_2;

        ಆದರೆ ((off_t) ಗಾತ್ರ_2 > bsize) {
            ಗಾತ್ರ_2 = (ಗಾತ್ರ_t) bsize;
        }
    }

    ctx->buf = ngx_create_ತಾತ್ಕಾಲಿಕ_buf(ctx->pool, ಗಾತ್ರ_2);
    ಆದರೆ (ctx->buf == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    /*
     * we do not set ctx->buf->tag, because we do not want
     * to reuse the buf via ctx->free list
     */

#ಆದರೆ (NGX_HAVE_ALIGNED_DIRECTIO)
    ctx->unaligned = 1;
#ಆದರೆ_ಅಂತ್ಯ

    ಹಿಂತಿರುಗಿ NGX_OK;
}


ಸ್ಥಿರ ngx_int_t
ngx_output_chain_get_buf(ngx_output_chain_ctx_t *ctx, off_t bsize)
{
    ಗಾತ್ರ_t       ಗಾತ್ರ_2;
    ngx_buf_t   *b, *ಒಳ;
    ngx_uint_t   recycled;

    ಒಳ = ctx->ಒಳ->buf;
    ಗಾತ್ರ_2 = ctx->bufs.ಗಾತ್ರ_2;
    recycled = 1;

    ಆದರೆ (ಒಳ->ಕೊನೆಯ_ಒಳ_chain) {

        ಆದರೆ (bsize < (off_t) ಗಾತ್ರ_2) {

            /*
             * allocate a small temp buf for a small last buf
             * or its small last part
             */

            ಗಾತ್ರ_2 = (ಗಾತ್ರ_t) bsize;
            recycled = 0;

        } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!ctx->directio
                   && ctx->bufs.ಅಂಕೆ == 1
                   && (bsize < (off_t) (ಗಾತ್ರ_2 + ಗಾತ್ರ_2 / 4)))
        {
            /*
             * allocate a temp buf that equals to a last buf,
             * if there is no directio, the last buf size is lesser
             * than 1.25 of bufs.size and the temp buf is single
             */

            ಗಾತ್ರ_2 = (ಗಾತ್ರ_t) bsize;
            recycled = 0;
        }
    }

    b = ngx_calloc_buf(ctx->pool);
    ಆದರೆ (b == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    ಆದರೆ (ctx->directio) {

        /*
         * allocate block aligned to a disk sector size to enable
         * userland buffer direct usage conjunctly with directio
         */

        b->ಆರಂಭ = ngx_pmemalign(ctx->pool, ಗಾತ್ರ_2, (ಗಾತ್ರ_t) ctx->alignment);
        ಆದರೆ (b->ಆರಂಭ == ಖಾಲಿ) {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

    } ಇಲ್ಲದಿದ್ದರೆ {
        b->ಆರಂಭ = ngx_palloc(ctx->pool, ಗಾತ್ರ_2);
        ಆದರೆ (b->ಆರಂಭ == ಖಾಲಿ) {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }
    }

    b->pos = b->ಆರಂಭ;
    b->ಕೊನೆಯ = b->ಆರಂಭ;
    b->ಕೊನೆ = b->ಕೊನೆಯ + ಗಾತ್ರ_2;
    b->temporary = 1;
    b->ಟ್ಯಾಗ್ = ctx->ಟ್ಯಾಗ್;
    b->recycled = recycled;

    ctx->buf = b;
    ctx->allocated++;

    ಹಿಂತಿರುಗಿ NGX_OK;
}


ಸ್ಥಿರ ngx_int_t
ngx_output_chain_copy_buf(ngx_output_chain_ctx_t *ctx)
{
    off_t        ಗಾತ್ರ_2;
    ssize_t      n;
    ngx_buf_t   *src, *dst;
    ngx_uint_t   sendfile;

    src = ctx->ಒಳ->buf;
    dst = ctx->buf;

    ಗಾತ್ರ_2 = ngx_buf_ಗಾತ್ರ(src);
    ಗಾತ್ರ_2 = ngx_min(ಗಾತ್ರ_2, dst->ಕೊನೆ - dst->pos);

    sendfile = ctx->sendfile && !ctx->directio;

#ಆದರೆ (NGX_SENDFILE_ಎಲ್ಲೆ)

    ಆದರೆ (src->ಒಳ_file && src->file_pos >= NGX_SENDFILE_ಎಲ್ಲೆ) {
        sendfile = 0;
    }

#ಆದರೆ_ಅಂತ್ಯ

    ಆದರೆ (ngx_buf_ಒಳ_ಸ್ಮರಣೆ(src)) {
        ngx_memcpy(dst->pos, src->pos, (ಗಾತ್ರ_t) ಗಾತ್ರ_2);
        src->pos += (ಗಾತ್ರ_t) ಗಾತ್ರ_2;
        dst->ಕೊನೆಯ += (ಗಾತ್ರ_t) ಗಾತ್ರ_2;

        ಆದರೆ (src->ಒಳ_file) {

            ಆದರೆ (sendfile) {
                dst->ಒಳ_file = 1;
                dst->file = src->file;
                dst->file_pos = src->file_pos;
                dst->file_ಕೊನೆಯ = src->file_pos + ಗಾತ್ರ_2;

            } ಇಲ್ಲದಿದ್ದರೆ {
                dst->ಒಳ_file = 0;
            }

            src->file_pos += ಗಾತ್ರ_2;

        } ಇಲ್ಲದಿದ್ದರೆ {
            dst->ಒಳ_file = 0;
        }

        ಆದರೆ (src->pos == src->ಕೊನೆಯ) {
            dst->flush = src->flush;
            dst->ಕೊನೆಯ_buf = src->ಕೊನೆಯ_buf;
            dst->ಕೊನೆಯ_ಒಳ_chain = src->ಕೊನೆಯ_ಒಳ_chain;

        } ಇಲ್ಲದಿದ್ದರೆ {
            dst->flush = 0;
            dst->ಕೊನೆಯ_buf = 0;
            dst->ಕೊನೆಯ_ಒಳ_chain = 0;
        }

    } ಇಲ್ಲದಿದ್ದರೆ {

#ಆದರೆ (NGX_HAVE_ALIGNED_DIRECTIO)

        ಆದರೆ (ctx->unaligned) {
            ಆದರೆ (ngx_directio_off(src->file->fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, ctx->pool->ಲಘುಗಣಕ, ngx_errno,
                              ngx_directio_off_n " \"%s\" failed",
                              src->file->name.ದತ್ತಾಂಶ);
            }
        }

#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ (NGX_HAVE_FILE_AIO)
        ಆದರೆ (ctx->aio_handler) {
            n = ngx_file_aio_read(src->file, dst->pos, (ಗಾತ್ರ_t) ಗಾತ್ರ_2,
                                  src->file_pos, ctx->pool);
            ಆದರೆ (n == NGX_AGAIN) {
                ctx->aio_handler(ctx, src->file);
                ಹಿಂತಿರುಗಿ NGX_AGAIN;
            }

        } ಇಲ್ಲದಿದ್ದರೆ
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ (NGX_ಎಳೆಗಳು)
        ಆದರೆ (ctx->thread_handler) {
            src->file->thread_task = ctx->thread_task;
            src->file->thread_handler = ctx->thread_handler;
            src->file->thread_ctx = ctx->filter_ctx;

            n = ngx_thread_read(src->file, dst->pos, (ಗಾತ್ರ_t) ಗಾತ್ರ_2,
                                src->file_pos, ctx->pool);
            ಆದರೆ (n == NGX_AGAIN) {
                ctx->thread_task = src->file->thread_task;
                ಹಿಂತಿರುಗಿ NGX_AGAIN;
            }

        } ಇಲ್ಲದಿದ್ದರೆ
#ಆದರೆ_ಅಂತ್ಯ
        {
            n = ngx_read_file(src->file, dst->pos, (ಗಾತ್ರ_t) ಗಾತ್ರ_2,
                              src->file_pos);
        }

#ಆದರೆ (NGX_HAVE_ALIGNED_DIRECTIO)

        ಆದರೆ (ctx->unaligned) {
            ngx_err_t  err;

            err = ngx_errno;

            ಆದರೆ (ngx_directio_on(src->file->fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, ctx->pool->ಲಘುಗಣಕ, ngx_errno,
                              ngx_directio_on_n " \"%s\" failed",
                              src->file->name.ದತ್ತಾಂಶ);
            }

            ngx_ಗಣ_errno(err);

            ctx->unaligned = 0;
        }

#ಆದರೆ_ಅಂತ್ಯ

        ಆದರೆ (n == NGX_ERROR) {
            ಹಿಂತಿರುಗಿ (ngx_int_t) n;
        }

        ಆದರೆ (n != ಗಾತ್ರ_2) {
            ngx_log_error(NGX_LOG_ALERT, ctx->pool->ಲಘುಗಣಕ, 0,
                          ngx_read_file_n " read only %z of %O from \"%s\"",
                          n, ಗಾತ್ರ_2, src->file->name.ದತ್ತಾಂಶ);
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        dst->ಕೊನೆಯ += n;

        ಆದರೆ (sendfile) {
            dst->ಒಳ_file = 1;
            dst->file = src->file;
            dst->file_pos = src->file_pos;
            dst->file_ಕೊನೆಯ = src->file_pos + n;

        } ಇಲ್ಲದಿದ್ದರೆ {
            dst->ಒಳ_file = 0;
        }

        src->file_pos += n;

        ಆದರೆ (src->file_pos == src->file_ಕೊನೆಯ) {
            dst->flush = src->flush;
            dst->ಕೊನೆಯ_buf = src->ಕೊನೆಯ_buf;
            dst->ಕೊನೆಯ_ಒಳ_chain = src->ಕೊನೆಯ_ಒಳ_chain;

        } ಇಲ್ಲದಿದ್ದರೆ {
            dst->flush = 0;
            dst->ಕೊನೆಯ_buf = 0;
            dst->ಕೊನೆಯ_ಒಳ_chain = 0;
        }
    }

    ಹಿಂತಿರುಗಿ NGX_OK;
}


ngx_int_t
ngx_chain_writer(ಶೂನ್ಯ *ದತ್ತಾಂಶ, ngx_chain_t *ಒಳ)
{
    ngx_chain_writer_ctx_t *ctx = ದತ್ತಾಂಶ;

    off_t              ಗಾತ್ರ_2;
    ngx_chain_t       *cl, *ln, *chain;
    ngx_connection_t  *c;

    c = ctx->connection;

    ಗಾಗಿ (ಗಾತ್ರ_2 = 0; ಒಳ; ಒಳ = ಒಳ->ಮುಂದಿನ) {

        ಆದರೆ (ngx_buf_ಗಾತ್ರ(ಒಳ->buf) == 0 && !ngx_buf_special(ಒಳ->buf)) {

            ngx_log_error(NGX_LOG_ALERT, ctx->pool->ಲಘುಗಣಕ, 0,
                          "zero size buf in chain writer "
                          "t:%d r:%d f:%d %p %p-%p %p %O-%O",
                          ಒಳ->buf->temporary,
                          ಒಳ->buf->recycled,
                          ಒಳ->buf->ಒಳ_file,
                          ಒಳ->buf->ಆರಂಭ,
                          ಒಳ->buf->pos,
                          ಒಳ->buf->ಕೊನೆಯ,
                          ಒಳ->buf->file,
                          ಒಳ->buf->file_pos,
                          ಒಳ->buf->file_ಕೊನೆಯ);

            ngx_debug_ಬಿಂದು();

            ಮುಂದುವರಿ;
        }

        ಆದರೆ (ngx_buf_ಗಾತ್ರ(ಒಳ->buf) < 0) {

            ngx_log_error(NGX_LOG_ALERT, ctx->pool->ಲಘುಗಣಕ, 0,
                          "negative size buf in chain writer "
                          "t:%d r:%d f:%d %p %p-%p %p %O-%O",
                          ಒಳ->buf->temporary,
                          ಒಳ->buf->recycled,
                          ಒಳ->buf->ಒಳ_file,
                          ಒಳ->buf->ಆರಂಭ,
                          ಒಳ->buf->pos,
                          ಒಳ->buf->ಕೊನೆಯ,
                          ಒಳ->buf->file,
                          ಒಳ->buf->file_pos,
                          ಒಳ->buf->file_ಕೊನೆಯ);

            ngx_debug_ಬಿಂದು();

            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        ಗಾತ್ರ_2 += ngx_buf_ಗಾತ್ರ(ಒಳ->buf);

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, c->ಲಘುಗಣಕ, 0,
                       "chain writer buf fl:%d s:%uO",
                       ಒಳ->buf->flush, ngx_buf_ಗಾತ್ರ(ಒಳ->buf));

        cl = ngx_alloc_chain_link(ctx->pool);
        ಆದರೆ (cl == ಖಾಲಿ) {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        cl->buf = ಒಳ->buf;
        cl->ಮುಂದಿನ = ಖಾಲಿ;
        *ctx->ಕೊನೆಯ = cl;
        ctx->ಕೊನೆಯ = &cl->ಮುಂದಿನ;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->ಲಘುಗಣಕ, 0,
                   "chain writer in: %p", ctx->ಹೊರಹೋಗು);

    ಗಾಗಿ (cl = ctx->ಹೊರಹೋಗು; cl; cl = cl->ಮುಂದಿನ) {

        ಆದರೆ (ngx_buf_ಗಾತ್ರ(cl->buf) == 0 && !ngx_buf_special(cl->buf)) {

            ngx_log_error(NGX_LOG_ALERT, ctx->pool->ಲಘುಗಣಕ, 0,
                          "zero size buf in chain writer "
                          "t:%d r:%d f:%d %p %p-%p %p %O-%O",
                          cl->buf->temporary,
                          cl->buf->recycled,
                          cl->buf->ಒಳ_file,
                          cl->buf->ಆರಂಭ,
                          cl->buf->pos,
                          cl->buf->ಕೊನೆಯ,
                          cl->buf->file,
                          cl->buf->file_pos,
                          cl->buf->file_ಕೊನೆಯ);

            ngx_debug_ಬಿಂದು();

            ಮುಂದುವರಿ;
        }

        ಆದರೆ (ngx_buf_ಗಾತ್ರ(cl->buf) < 0) {

            ngx_log_error(NGX_LOG_ALERT, ctx->pool->ಲಘುಗಣಕ, 0,
                          "negative size buf in chain writer "
                          "t:%d r:%d f:%d %p %p-%p %p %O-%O",
                          cl->buf->temporary,
                          cl->buf->recycled,
                          cl->buf->ಒಳ_file,
                          cl->buf->ಆರಂಭ,
                          cl->buf->pos,
                          cl->buf->ಕೊನೆಯ,
                          cl->buf->file,
                          cl->buf->file_pos,
                          cl->buf->file_ಕೊನೆಯ);

            ngx_debug_ಬಿಂದು();

            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        ಗಾತ್ರ_2 += ngx_buf_ಗಾತ್ರ(cl->buf);
    }

    ಆದರೆ (ಗಾತ್ರ_2 == 0 && !c->buffered) {
        ಹಿಂತಿರುಗಿ NGX_OK;
    }

    chain = c->send_chain(c, ctx->ಹೊರಹೋಗು, ctx->ಎಲ್ಲೆ);

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->ಲಘುಗಣಕ, 0,
                   "chain writer out: %p", chain);

    ಆದರೆ (chain == NGX_CHAIN_ERROR) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    ಆದರೆ (chain && c->ಲಿಖ->ready) {
        ngx_post_event(c->ಲಿಖ, &ngx_posted_ಮುಂದಿನ_events);
    }

    ಗಾಗಿ (cl = ctx->ಹೊರಹೋಗು; cl && cl != chain; /* void */) {
        ln = cl;
        cl = cl->ಮುಂದಿನ;
        ngx_free_chain(ctx->pool, ln);
    }

    ctx->ಹೊರಹೋಗು = chain;

    ಆದರೆ (ctx->ಹೊರಹೋಗು == ಖಾಲಿ) {
        ctx->ಕೊನೆಯ = &ctx->ಹೊರಹೋಗು;

        ಆದರೆ (!c->buffered) {
            ಹಿಂತಿರುಗಿ NGX_OK;
        }
    }

    ಹಿಂತಿರುಗಿ NGX_AGAIN;
}
