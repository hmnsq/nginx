
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>
#ضمن <ngx_event.h>


#إذا 0
#تعریف_کرو NGX_SENDFILE_حد_بندی  4096
#جے_انت

/*
 * When DIRECTIO is enabled FreeBSD, Solaris, and MacOSX read directly
 * to an application memory from a device if parameters are aligned
 * to device sector boundary (512 bytes).  They fallback to usual read
 * operation if the parameters are not aligned.
 * Linux allows DIRECTIO only if the parameters are aligned to a filesystem
 * sector boundary, otherwise it returns EINVAL.  The sector size is
 * usually 512 bytes, however, on XFS it may be 4096 bytes.
 */

#تعریف_کرو NGX_NONE            1


ساكن ngx_inline ngx_int_t
    ngx_output_chain_as_is(ngx_output_chain_ctx_t *ctx, ngx_buf_t *buf);
ساكن ngx_int_t ngx_output_chain_جوڑو_copy(ngx_pool_t *pool,
    ngx_chain_t **chain, ngx_chain_t *وچ);
ساكن ngx_int_t ngx_output_chain_align_file_buf(ngx_output_chain_ctx_t *ctx,
    off_t bsize);
ساكن ngx_int_t ngx_output_chain_get_buf(ngx_output_chain_ctx_t *ctx,
    off_t bsize);
ساكن ngx_int_t ngx_output_chain_copy_buf(ngx_output_chain_ctx_t *ctx);


ngx_int_t
ngx_output_chain(ngx_output_chain_ctx_t *ctx, ngx_chain_t *وچ)
{
    off_t         bsize;
    ngx_int_t     rc, آخری;
    ngx_chain_t  *cl, *باہرلا, **آخری_باہرلا;

    إذا (ctx->وچ == عدم && ctx->busy == عدم
#إذا (NGX_HAVE_FILE_AIO || NGX_دھاگے)
        && !ctx->aio
#جے_انت
       )
    {
        /*
         * the short path for the case when the ctx->in and ctx->busy chains
         * are empty, the incoming chain is empty too or has the single buf
         * that does not require the copy
         */

        إذا (وچ == عدم) {
            أرجع ctx->output_filter(ctx->filter_ctx, وچ);
        }

        إذا (وچ->اگلا == عدم
#إذا (NGX_SENDFILE_حد_بندی)
            && !(وچ->buf->وچ_file && وچ->buf->file_آخری > NGX_SENDFILE_حد_بندی)
#جے_انت
            && ngx_output_chain_as_is(ctx, وچ->buf))
        {
            أرجع ctx->output_filter(ctx->filter_ctx, وچ);
        }
    }

    /* add the incoming buf to the chain ctx->in */

    إذا (وچ) {
        إذا (ngx_output_chain_جوڑو_copy(ctx->pool, &ctx->وچ, وچ) == NGX_ERROR) {
            أرجع NGX_ERROR;
        }
    }

    باہرلا = عدم;
    آخری_باہرلا = &باہرلا;
    آخری = NGX_NONE;

    لكل ( ;; ) {

#إذا (NGX_HAVE_FILE_AIO || NGX_دھاگے)
        إذا (ctx->aio) {
            أرجع NGX_AGAIN;
        }
#جے_انت

        طالما (ctx->وچ) {

            /*
             * cycle while there are the ctx->in bufs
             * and there are the free output bufs to copy in
             */

            bsize = ngx_buf_ناپ(ctx->وچ->buf);

            إذا (bsize == 0 && !ngx_buf_special(ctx->وچ->buf)) {

                ngx_log_error(NGX_LOG_ALERT, ctx->pool->لوگارتھم, 0,
                              "zero size buf in output "
                              "t:%d r:%d f:%d %p %p-%p %p %O-%O",
                              ctx->وچ->buf->temporary,
                              ctx->وچ->buf->recycled,
                              ctx->وچ->buf->وچ_file,
                              ctx->وچ->buf->شروع,
                              ctx->وچ->buf->pos,
                              ctx->وچ->buf->آخری,
                              ctx->وچ->buf->file,
                              ctx->وچ->buf->file_pos,
                              ctx->وچ->buf->file_آخری);

                ngx_debug_نقطہ();

                cl = ctx->وچ;
                ctx->وچ = cl->اگلا;

                ngx_free_chain(ctx->pool, cl);

                استمر;
            }

            إذا (bsize < 0) {

                ngx_log_error(NGX_LOG_ALERT, ctx->pool->لوگارتھم, 0,
                              "negative size buf in output "
                              "t:%d r:%d f:%d %p %p-%p %p %O-%O",
                              ctx->وچ->buf->temporary,
                              ctx->وچ->buf->recycled,
                              ctx->وچ->buf->وچ_file,
                              ctx->وچ->buf->شروع,
                              ctx->وچ->buf->pos,
                              ctx->وچ->buf->آخری,
                              ctx->وچ->buf->file,
                              ctx->وچ->buf->file_pos,
                              ctx->وچ->buf->file_آخری);

                ngx_debug_نقطہ();

                أرجع NGX_ERROR;
            }

            إذا (ngx_output_chain_as_is(ctx, ctx->وچ->buf)) {

                /* move the chain link to the output chain */

                cl = ctx->وچ;
                ctx->وچ = cl->اگلا;

                *آخری_باہرلا = cl;
                آخری_باہرلا = &cl->اگلا;
                cl->اگلا = عدم;

                استمر;
            }

            إذا (ctx->buf == عدم) {

                rc = ngx_output_chain_align_file_buf(ctx, bsize);

                إذا (rc == NGX_ERROR) {
                    أرجع NGX_ERROR;
                }

                إذا (rc != NGX_OK) {

                    إذا (ctx->حرر) {

                        /* get the free buf */

                        cl = ctx->حرر;
                        ctx->buf = cl->buf;
                        ctx->حرر = cl->اگلا;

                        ngx_free_chain(ctx->pool, cl);

                    } وإلا إذا (باہرلا || ctx->allocated == ctx->bufs.عدد) {

                        اكسر;

                    } وإلا إذا (ngx_output_chain_get_buf(ctx, bsize) != NGX_OK) {
                        أرجع NGX_ERROR;
                    }
                }
            }

            rc = ngx_output_chain_copy_buf(ctx);

            إذا (rc == NGX_ERROR) {
                أرجع rc;
            }

            إذا (rc == NGX_AGAIN) {
                إذا (باہرلا) {
                    اكسر;
                }

                أرجع rc;
            }

            /* delete the completed buf from the ctx->in chain */

            إذا (ngx_buf_ناپ(ctx->وچ->buf) == 0) {
                cl = ctx->وچ;
                ctx->وچ = cl->اگلا;

                ngx_free_chain(ctx->pool, cl);
            }

            cl = ngx_alloc_chain_link(ctx->pool);
            إذا (cl == عدم) {
                أرجع NGX_ERROR;
            }

            cl->buf = ctx->buf;
            cl->اگلا = عدم;
            *آخری_باہرلا = cl;
            آخری_باہرلا = &cl->اگلا;
            ctx->buf = عدم;
        }

        إذا (باہرلا == عدم && آخری != NGX_NONE) {

            إذا (ctx->وچ) {
                أرجع NGX_AGAIN;
            }

            أرجع آخری;
        }

        آخری = ctx->output_filter(ctx->filter_ctx, باہرلا);

        إذا (آخری == NGX_ERROR || آخری == NGX_پورا) {
            أرجع آخری;
        }

        ngx_chain_update_chains(ctx->pool, &ctx->حرر, &ctx->busy, &باہرلا,
                                ctx->ٹیگ);
        آخری_باہرلا = &باہرلا;
    }
}


ساكن ngx_inline ngx_int_t
ngx_output_chain_as_is(ngx_output_chain_ctx_t *ctx, ngx_buf_t *buf)
{
    ngx_uint_t  sendfile;

    إذا (ngx_buf_special(buf)) {
        أرجع 1;
    }

#إذا (NGX_دھاگے)
    إذا (buf->وچ_file) {
        buf->file->thread_handler = ctx->thread_handler;
        buf->file->thread_ctx = ctx->filter_ctx;
    }
#جے_انت

    sendfile = ctx->sendfile;

#إذا (NGX_SENDFILE_حد_بندی)

    إذا (buf->وچ_file && buf->file_pos >= NGX_SENDFILE_حد_بندی) {
        sendfile = 0;
    }

#جے_انت

#إذا !(NGX_HAVE_SENDFILE_NODISKIO)

    /*
     * With DIRECTIO, disable sendfile() unless sendfile(SF_NOCACHE)
     * is available.
     */

    إذا (buf->وچ_file && buf->file->directio) {
        sendfile = 0;
    }

#جے_انت

    إذا (!sendfile) {

        إذا (!ngx_buf_وچ_حافظہ(buf)) {
            أرجع 0;
        }

        buf->وچ_file = 0;
    }

    إذا (ctx->need_وچ_حافظہ && !ngx_buf_وچ_حافظہ(buf)) {
        أرجع 0;
    }

    إذا (ctx->need_وچ_عارضی && (buf->حافظہ || buf->mmap)) {
        أرجع 0;
    }

    أرجع 1;
}


ساكن ngx_int_t
ngx_output_chain_جوڑو_copy(ngx_pool_t *pool, ngx_chain_t **chain,
    ngx_chain_t *وچ)
{
    ngx_chain_t  *cl, **ll;
#إذا (NGX_SENDFILE_حد_بندی)
    ngx_buf_t    *b, *buf;
#جے_انت

    ll = chain;

    لكل (cl = *chain; cl; cl = cl->اگلا) {
        ll = &cl->اگلا;
    }

    طالما (وچ) {

        cl = ngx_alloc_chain_link(pool);
        إذا (cl == عدم) {
            أرجع NGX_ERROR;
        }

#إذا (NGX_SENDFILE_حد_بندی)

        buf = وچ->buf;

        إذا (buf->وچ_file
            && buf->file_pos < NGX_SENDFILE_حد_بندی
            && buf->file_آخری > NGX_SENDFILE_حد_بندی)
        {
            /* split a file buf on two bufs by the sendfile limit */

            b = ngx_calloc_buf(pool);
            إذا (b == عدم) {
                أرجع NGX_ERROR;
            }

            ngx_memcpy(b, buf, حجم(ngx_buf_t));

            إذا (ngx_buf_وچ_حافظہ(buf)) {
                buf->pos += (ssize_t) (NGX_SENDFILE_حد_بندی - buf->file_pos);
                b->آخری = buf->pos;
            }

            buf->file_pos = NGX_SENDFILE_حد_بندی;
            b->file_آخری = NGX_SENDFILE_حد_بندی;

            cl->buf = b;

        } وإلا {
            cl->buf = buf;
            وچ = وچ->اگلا;
        }

#وإلا
        cl->buf = وچ->buf;
        وچ = وچ->اگلا;

#جے_انت

        cl->اگلا = عدم;
        *ll = cl;
        ll = &cl->اگلا;
    }

    أرجع NGX_OK;
}


ساكن ngx_int_t
ngx_output_chain_align_file_buf(ngx_output_chain_ctx_t *ctx, off_t bsize)
{
    ناپ_t      ناپ;
    ngx_buf_t  *وچ;

    وچ = ctx->وچ->buf;

    إذا (وچ->file == عدم || !وچ->file->directio) {
        أرجع NGX_DECLINED;
    }

    ctx->directio = 1;

    ناپ = (ناپ_t) (وچ->file_pos - (وچ->file_pos & ~(ctx->alignment - 1)));

    إذا (ناپ == 0) {

        إذا (bsize >= (off_t) ctx->bufs.ناپ) {
            أرجع NGX_DECLINED;
        }

        ناپ = (ناپ_t) bsize;

    } وإلا {
        ناپ = (ناپ_t) ctx->alignment - ناپ;

        إذا ((off_t) ناپ > bsize) {
            ناپ = (ناپ_t) bsize;
        }
    }

    ctx->buf = ngx_create_عارضی_buf(ctx->pool, ناپ);
    إذا (ctx->buf == عدم) {
        أرجع NGX_ERROR;
    }

    /*
     * we do not set ctx->buf->tag, because we do not want
     * to reuse the buf via ctx->free list
     */

#إذا (NGX_HAVE_ALIGNED_DIRECTIO)
    ctx->unaligned = 1;
#جے_انت

    أرجع NGX_OK;
}


ساكن ngx_int_t
ngx_output_chain_get_buf(ngx_output_chain_ctx_t *ctx, off_t bsize)
{
    ناپ_t       ناپ;
    ngx_buf_t   *b, *وچ;
    ngx_uint_t   recycled;

    وچ = ctx->وچ->buf;
    ناپ = ctx->bufs.ناپ;
    recycled = 1;

    إذا (وچ->آخری_وچ_chain) {

        إذا (bsize < (off_t) ناپ) {

            /*
             * allocate a small temp buf for a small last buf
             * or its small last part
             */

            ناپ = (ناپ_t) bsize;
            recycled = 0;

        } وإلا إذا (!ctx->directio
                   && ctx->bufs.عدد == 1
                   && (bsize < (off_t) (ناپ + ناپ / 4)))
        {
            /*
             * allocate a temp buf that equals to a last buf,
             * if there is no directio, the last buf size is lesser
             * than 1.25 of bufs.size and the temp buf is single
             */

            ناپ = (ناپ_t) bsize;
            recycled = 0;
        }
    }

    b = ngx_calloc_buf(ctx->pool);
    إذا (b == عدم) {
        أرجع NGX_ERROR;
    }

    إذا (ctx->directio) {

        /*
         * allocate block aligned to a disk sector size to enable
         * userland buffer direct usage conjunctly with directio
         */

        b->شروع = ngx_pmemalign(ctx->pool, ناپ, (ناپ_t) ctx->alignment);
        إذا (b->شروع == عدم) {
            أرجع NGX_ERROR;
        }

    } وإلا {
        b->شروع = ngx_palloc(ctx->pool, ناپ);
        إذا (b->شروع == عدم) {
            أرجع NGX_ERROR;
        }
    }

    b->pos = b->شروع;
    b->آخری = b->شروع;
    b->سرا = b->آخری + ناپ;
    b->temporary = 1;
    b->ٹیگ = ctx->ٹیگ;
    b->recycled = recycled;

    ctx->buf = b;
    ctx->allocated++;

    أرجع NGX_OK;
}


ساكن ngx_int_t
ngx_output_chain_copy_buf(ngx_output_chain_ctx_t *ctx)
{
    off_t        ناپ;
    ssize_t      n;
    ngx_buf_t   *src, *dst;
    ngx_uint_t   sendfile;

    src = ctx->وچ->buf;
    dst = ctx->buf;

    ناپ = ngx_buf_ناپ(src);
    ناپ = ngx_min(ناپ, dst->سرا - dst->pos);

    sendfile = ctx->sendfile && !ctx->directio;

#إذا (NGX_SENDFILE_حد_بندی)

    إذا (src->وچ_file && src->file_pos >= NGX_SENDFILE_حد_بندی) {
        sendfile = 0;
    }

#جے_انت

    إذا (ngx_buf_وچ_حافظہ(src)) {
        ngx_memcpy(dst->pos, src->pos, (ناپ_t) ناپ);
        src->pos += (ناپ_t) ناپ;
        dst->آخری += (ناپ_t) ناپ;

        إذا (src->وچ_file) {

            إذا (sendfile) {
                dst->وچ_file = 1;
                dst->file = src->file;
                dst->file_pos = src->file_pos;
                dst->file_آخری = src->file_pos + ناپ;

            } وإلا {
                dst->وچ_file = 0;
            }

            src->file_pos += ناپ;

        } وإلا {
            dst->وچ_file = 0;
        }

        إذا (src->pos == src->آخری) {
            dst->flush = src->flush;
            dst->آخری_buf = src->آخری_buf;
            dst->آخری_وچ_chain = src->آخری_وچ_chain;

        } وإلا {
            dst->flush = 0;
            dst->آخری_buf = 0;
            dst->آخری_وچ_chain = 0;
        }

    } وإلا {

#إذا (NGX_HAVE_ALIGNED_DIRECTIO)

        إذا (ctx->unaligned) {
            إذا (ngx_directio_off(src->file->fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, ctx->pool->لوگارتھم, ngx_errno,
                              ngx_directio_off_n " \"%s\" failed",
                              src->file->name.انکڑے);
            }
        }

#جے_انت

#إذا (NGX_HAVE_FILE_AIO)
        إذا (ctx->aio_handler) {
            n = ngx_file_aio_read(src->file, dst->pos, (ناپ_t) ناپ,
                                  src->file_pos, ctx->pool);
            إذا (n == NGX_AGAIN) {
                ctx->aio_handler(ctx, src->file);
                أرجع NGX_AGAIN;
            }

        } وإلا
#جے_انت
#إذا (NGX_دھاگے)
        إذا (ctx->thread_handler) {
            src->file->thread_task = ctx->thread_task;
            src->file->thread_handler = ctx->thread_handler;
            src->file->thread_ctx = ctx->filter_ctx;

            n = ngx_thread_read(src->file, dst->pos, (ناپ_t) ناپ,
                                src->file_pos, ctx->pool);
            إذا (n == NGX_AGAIN) {
                ctx->thread_task = src->file->thread_task;
                أرجع NGX_AGAIN;
            }

        } وإلا
#جے_انت
        {
            n = ngx_read_file(src->file, dst->pos, (ناپ_t) ناپ,
                              src->file_pos);
        }

#إذا (NGX_HAVE_ALIGNED_DIRECTIO)

        إذا (ctx->unaligned) {
            ngx_err_t  err;

            err = ngx_errno;

            إذا (ngx_directio_on(src->file->fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, ctx->pool->لوگارتھم, ngx_errno,
                              ngx_directio_on_n " \"%s\" failed",
                              src->file->name.انکڑے);
            }

            ngx_سیٹ_errno(err);

            ctx->unaligned = 0;
        }

#جے_انت

        إذا (n == NGX_ERROR) {
            أرجع (ngx_int_t) n;
        }

        إذا (n != ناپ) {
            ngx_log_error(NGX_LOG_ALERT, ctx->pool->لوگارتھم, 0,
                          ngx_read_file_n " read only %z of %O from \"%s\"",
                          n, ناپ, src->file->name.انکڑے);
            أرجع NGX_ERROR;
        }

        dst->آخری += n;

        إذا (sendfile) {
            dst->وچ_file = 1;
            dst->file = src->file;
            dst->file_pos = src->file_pos;
            dst->file_آخری = src->file_pos + n;

        } وإلا {
            dst->وچ_file = 0;
        }

        src->file_pos += n;

        إذا (src->file_pos == src->file_آخری) {
            dst->flush = src->flush;
            dst->آخری_buf = src->آخری_buf;
            dst->آخری_وچ_chain = src->آخری_وچ_chain;

        } وإلا {
            dst->flush = 0;
            dst->آخری_buf = 0;
            dst->آخری_وچ_chain = 0;
        }
    }

    أرجع NGX_OK;
}


ngx_int_t
ngx_chain_writer(فراغ *انکڑے, ngx_chain_t *وچ)
{
    ngx_chain_writer_ctx_t *ctx = انکڑے;

    off_t              ناپ;
    ngx_chain_t       *cl, *ln, *chain;
    ngx_connection_t  *c;

    c = ctx->connection;

    لكل (ناپ = 0; وچ; وچ = وچ->اگلا) {

        إذا (ngx_buf_ناپ(وچ->buf) == 0 && !ngx_buf_special(وچ->buf)) {

            ngx_log_error(NGX_LOG_ALERT, ctx->pool->لوگارتھم, 0,
                          "zero size buf in chain writer "
                          "t:%d r:%d f:%d %p %p-%p %p %O-%O",
                          وچ->buf->temporary,
                          وچ->buf->recycled,
                          وچ->buf->وچ_file,
                          وچ->buf->شروع,
                          وچ->buf->pos,
                          وچ->buf->آخری,
                          وچ->buf->file,
                          وچ->buf->file_pos,
                          وچ->buf->file_آخری);

            ngx_debug_نقطہ();

            استمر;
        }

        إذا (ngx_buf_ناپ(وچ->buf) < 0) {

            ngx_log_error(NGX_LOG_ALERT, ctx->pool->لوگارتھم, 0,
                          "negative size buf in chain writer "
                          "t:%d r:%d f:%d %p %p-%p %p %O-%O",
                          وچ->buf->temporary,
                          وچ->buf->recycled,
                          وچ->buf->وچ_file,
                          وچ->buf->شروع,
                          وچ->buf->pos,
                          وچ->buf->آخری,
                          وچ->buf->file,
                          وچ->buf->file_pos,
                          وچ->buf->file_آخری);

            ngx_debug_نقطہ();

            أرجع NGX_ERROR;
        }

        ناپ += ngx_buf_ناپ(وچ->buf);

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, c->لوگارتھم, 0,
                       "chain writer buf fl:%d s:%uO",
                       وچ->buf->flush, ngx_buf_ناپ(وچ->buf));

        cl = ngx_alloc_chain_link(ctx->pool);
        إذا (cl == عدم) {
            أرجع NGX_ERROR;
        }

        cl->buf = وچ->buf;
        cl->اگلا = عدم;
        *ctx->آخری = cl;
        ctx->آخری = &cl->اگلا;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->لوگارتھم, 0,
                   "chain writer in: %p", ctx->باہرلا);

    لكل (cl = ctx->باہرلا; cl; cl = cl->اگلا) {

        إذا (ngx_buf_ناپ(cl->buf) == 0 && !ngx_buf_special(cl->buf)) {

            ngx_log_error(NGX_LOG_ALERT, ctx->pool->لوگارتھم, 0,
                          "zero size buf in chain writer "
                          "t:%d r:%d f:%d %p %p-%p %p %O-%O",
                          cl->buf->temporary,
                          cl->buf->recycled,
                          cl->buf->وچ_file,
                          cl->buf->شروع,
                          cl->buf->pos,
                          cl->buf->آخری,
                          cl->buf->file,
                          cl->buf->file_pos,
                          cl->buf->file_آخری);

            ngx_debug_نقطہ();

            استمر;
        }

        إذا (ngx_buf_ناپ(cl->buf) < 0) {

            ngx_log_error(NGX_LOG_ALERT, ctx->pool->لوگارتھم, 0,
                          "negative size buf in chain writer "
                          "t:%d r:%d f:%d %p %p-%p %p %O-%O",
                          cl->buf->temporary,
                          cl->buf->recycled,
                          cl->buf->وچ_file,
                          cl->buf->شروع,
                          cl->buf->pos,
                          cl->buf->آخری,
                          cl->buf->file,
                          cl->buf->file_pos,
                          cl->buf->file_آخری);

            ngx_debug_نقطہ();

            أرجع NGX_ERROR;
        }

        ناپ += ngx_buf_ناپ(cl->buf);
    }

    إذا (ناپ == 0 && !c->buffered) {
        أرجع NGX_OK;
    }

    chain = c->send_chain(c, ctx->باہرلا, ctx->حد_بندی);

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->لوگارتھم, 0,
                   "chain writer out: %p", chain);

    إذا (chain == NGX_CHAIN_ERROR) {
        أرجع NGX_ERROR;
    }

    إذا (chain && c->اكتب->ready) {
        ngx_post_event(c->اكتب, &ngx_posted_اگلا_events);
    }

    لكل (cl = ctx->باہرلا; cl && cl != chain; /* void */) {
        ln = cl;
        cl = cl->اگلا;
        ngx_free_chain(ctx->pool, ln);
    }

    ctx->باہرلا = chain;

    إذا (ctx->باہرلا == عدم) {
        ctx->آخری = &ctx->باہرلا;

        إذا (!c->buffered) {
            أرجع NGX_OK;
        }
    }

    أرجع NGX_AGAIN;
}
