
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>
#ضمن <ngx_event.h>


#إذا 0
#تعریف_کړه NGX_SENDFILE_حد  4096
#د_که_پای

/*
 * When DIRECTIO is enabled FreeBSD, Solaris, and MacOSX read directly
 * to an application memory from a device if parameters are aligned
 * to device sector boundary (512 bytes).  They fallback to usual read
 * operation if the parameters are not aligned.
 * Linux allows DIRECTIO only if the parameters are aligned to a filesystem
 * sector boundary, otherwise it returns EINVAL.  The sector size is
 * usually 512 bytes, however, on XFS it may be 4096 bytes.
 */

#تعریف_کړه NGX_NONE            1


ساكن ngx_inline ngx_int_t
    ngx_output_chain_as_is(ngx_output_chain_ctx_t *ctx, ngx_buf_t *buf);
ساكن ngx_int_t ngx_output_chain_جمع_کړه_copy(ngx_pool_t *pool,
    ngx_chain_t **chain, ngx_chain_t *دننه);
ساكن ngx_int_t ngx_output_chain_align_file_buf(ngx_output_chain_ctx_t *ctx,
    off_t bsize);
ساكن ngx_int_t ngx_output_chain_get_buf(ngx_output_chain_ctx_t *ctx,
    off_t bsize);
ساكن ngx_int_t ngx_output_chain_copy_buf(ngx_output_chain_ctx_t *ctx);


ngx_int_t
ngx_output_chain(ngx_output_chain_ctx_t *ctx, ngx_chain_t *دننه)
{
    off_t         bsize;
    ngx_int_t     rc, وروستی;
    ngx_chain_t  *cl, *بهر, **وروستی_بهر;

    إذا (ctx->دننه == عدم && ctx->busy == عدم
#إذا (NGX_HAVE_FILE_AIO || NGX_تارونه)
        && !ctx->aio
#د_که_پای
       )
    {
        /*
         * the short path for the case when the ctx->in and ctx->busy chains
         * are empty, the incoming chain is empty too or has the single buf
         * that does not require the copy
         */

        إذا (دننه == عدم) {
            أرجع ctx->output_filter(ctx->filter_ctx, دننه);
        }

        إذا (دننه->بل == عدم
#إذا (NGX_SENDFILE_حد)
            && !(دننه->buf->دننه_file && دننه->buf->file_وروستی > NGX_SENDFILE_حد)
#د_که_پای
            && ngx_output_chain_as_is(ctx, دننه->buf))
        {
            أرجع ctx->output_filter(ctx->filter_ctx, دننه);
        }
    }

    /* add the incoming buf to the chain ctx->in */

    إذا (دننه) {
        إذا (ngx_output_chain_جمع_کړه_copy(ctx->pool, &ctx->دننه, دننه) == NGX_ERROR) {
            أرجع NGX_ERROR;
        }
    }

    بهر = عدم;
    وروستی_بهر = &بهر;
    وروستی = NGX_NONE;

    لكل ( ;; ) {

#إذا (NGX_HAVE_FILE_AIO || NGX_تارونه)
        إذا (ctx->aio) {
            أرجع NGX_AGAIN;
        }
#د_که_پای

        طالما (ctx->دننه) {

            /*
             * cycle while there are the ctx->in bufs
             * and there are the free output bufs to copy in
             */

            bsize = ngx_buf_اندازه(ctx->دننه->buf);

            إذا (bsize == 0 && !ngx_buf_special(ctx->دننه->buf)) {

                ngx_log_error(NGX_LOG_ALERT, ctx->pool->لوګاریتم, 0,
                              "zero size buf in output "
                              "t:%d r:%d f:%d %p %p-%p %p %O-%O",
                              ctx->دننه->buf->temporary,
                              ctx->دننه->buf->recycled,
                              ctx->دننه->buf->دننه_file,
                              ctx->دننه->buf->پیل,
                              ctx->دننه->buf->pos,
                              ctx->دننه->buf->وروستی,
                              ctx->دننه->buf->file,
                              ctx->دننه->buf->file_pos,
                              ctx->دننه->buf->file_وروستی);

                ngx_debug_ټکی();

                cl = ctx->دننه;
                ctx->دننه = cl->بل;

                ngx_free_chain(ctx->pool, cl);

                استمر;
            }

            إذا (bsize < 0) {

                ngx_log_error(NGX_LOG_ALERT, ctx->pool->لوګاریتم, 0,
                              "negative size buf in output "
                              "t:%d r:%d f:%d %p %p-%p %p %O-%O",
                              ctx->دننه->buf->temporary,
                              ctx->دننه->buf->recycled,
                              ctx->دننه->buf->دننه_file,
                              ctx->دننه->buf->پیل,
                              ctx->دننه->buf->pos,
                              ctx->دننه->buf->وروستی,
                              ctx->دننه->buf->file,
                              ctx->دننه->buf->file_pos,
                              ctx->دننه->buf->file_وروستی);

                ngx_debug_ټکی();

                أرجع NGX_ERROR;
            }

            إذا (ngx_output_chain_as_is(ctx, ctx->دننه->buf)) {

                /* move the chain link to the output chain */

                cl = ctx->دننه;
                ctx->دننه = cl->بل;

                *وروستی_بهر = cl;
                وروستی_بهر = &cl->بل;
                cl->بل = عدم;

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
                        ctx->حرر = cl->بل;

                        ngx_free_chain(ctx->pool, cl);

                    } وإلا إذا (بهر || ctx->allocated == ctx->bufs.عدد) {

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
                إذا (بهر) {
                    اكسر;
                }

                أرجع rc;
            }

            /* delete the completed buf from the ctx->in chain */

            إذا (ngx_buf_اندازه(ctx->دننه->buf) == 0) {
                cl = ctx->دننه;
                ctx->دننه = cl->بل;

                ngx_free_chain(ctx->pool, cl);
            }

            cl = ngx_alloc_chain_link(ctx->pool);
            إذا (cl == عدم) {
                أرجع NGX_ERROR;
            }

            cl->buf = ctx->buf;
            cl->بل = عدم;
            *وروستی_بهر = cl;
            وروستی_بهر = &cl->بل;
            ctx->buf = عدم;
        }

        إذا (بهر == عدم && وروستی != NGX_NONE) {

            إذا (ctx->دننه) {
                أرجع NGX_AGAIN;
            }

            أرجع وروستی;
        }

        وروستی = ctx->output_filter(ctx->filter_ctx, بهر);

        إذا (وروستی == NGX_ERROR || وروستی == NGX_بشپړ) {
            أرجع وروستی;
        }

        ngx_chain_update_chains(ctx->pool, &ctx->حرر, &ctx->busy, &بهر,
                                ctx->نښان);
        وروستی_بهر = &بهر;
    }
}


ساكن ngx_inline ngx_int_t
ngx_output_chain_as_is(ngx_output_chain_ctx_t *ctx, ngx_buf_t *buf)
{
    ngx_uint_t  sendfile;

    إذا (ngx_buf_special(buf)) {
        أرجع 1;
    }

#إذا (NGX_تارونه)
    إذا (buf->دننه_file) {
        buf->file->thread_handler = ctx->thread_handler;
        buf->file->thread_ctx = ctx->filter_ctx;
    }
#د_که_پای

    sendfile = ctx->sendfile;

#إذا (NGX_SENDFILE_حد)

    إذا (buf->دننه_file && buf->file_pos >= NGX_SENDFILE_حد) {
        sendfile = 0;
    }

#د_که_پای

#إذا !(NGX_HAVE_SENDFILE_NODISKIO)

    /*
     * With DIRECTIO, disable sendfile() unless sendfile(SF_NOCACHE)
     * is available.
     */

    إذا (buf->دننه_file && buf->file->directio) {
        sendfile = 0;
    }

#د_که_پای

    إذا (!sendfile) {

        إذا (!ngx_buf_دننه_حافظه(buf)) {
            أرجع 0;
        }

        buf->دننه_file = 0;
    }

    إذا (ctx->need_دننه_حافظه && !ngx_buf_دننه_حافظه(buf)) {
        أرجع 0;
    }

    إذا (ctx->need_دننه_لنډمهاله && (buf->حافظه || buf->mmap)) {
        أرجع 0;
    }

    أرجع 1;
}


ساكن ngx_int_t
ngx_output_chain_جمع_کړه_copy(ngx_pool_t *pool, ngx_chain_t **chain,
    ngx_chain_t *دننه)
{
    ngx_chain_t  *cl, **ll;
#إذا (NGX_SENDFILE_حد)
    ngx_buf_t    *b, *buf;
#د_که_پای

    ll = chain;

    لكل (cl = *chain; cl; cl = cl->بل) {
        ll = &cl->بل;
    }

    طالما (دننه) {

        cl = ngx_alloc_chain_link(pool);
        إذا (cl == عدم) {
            أرجع NGX_ERROR;
        }

#إذا (NGX_SENDFILE_حد)

        buf = دننه->buf;

        إذا (buf->دننه_file
            && buf->file_pos < NGX_SENDFILE_حد
            && buf->file_وروستی > NGX_SENDFILE_حد)
        {
            /* split a file buf on two bufs by the sendfile limit */

            b = ngx_calloc_buf(pool);
            إذا (b == عدم) {
                أرجع NGX_ERROR;
            }

            ngx_memcpy(b, buf, حجم(ngx_buf_t));

            إذا (ngx_buf_دننه_حافظه(buf)) {
                buf->pos += (ssize_t) (NGX_SENDFILE_حد - buf->file_pos);
                b->وروستی = buf->pos;
            }

            buf->file_pos = NGX_SENDFILE_حد;
            b->file_وروستی = NGX_SENDFILE_حد;

            cl->buf = b;

        } وإلا {
            cl->buf = buf;
            دننه = دننه->بل;
        }

#وإلا
        cl->buf = دننه->buf;
        دننه = دننه->بل;

#د_که_پای

        cl->بل = عدم;
        *ll = cl;
        ll = &cl->بل;
    }

    أرجع NGX_OK;
}


ساكن ngx_int_t
ngx_output_chain_align_file_buf(ngx_output_chain_ctx_t *ctx, off_t bsize)
{
    اندازه_t      اندازه;
    ngx_buf_t  *دننه;

    دننه = ctx->دننه->buf;

    إذا (دننه->file == عدم || !دننه->file->directio) {
        أرجع NGX_DECLINED;
    }

    ctx->directio = 1;

    اندازه = (اندازه_t) (دننه->file_pos - (دننه->file_pos & ~(ctx->alignment - 1)));

    إذا (اندازه == 0) {

        إذا (bsize >= (off_t) ctx->bufs.اندازه) {
            أرجع NGX_DECLINED;
        }

        اندازه = (اندازه_t) bsize;

    } وإلا {
        اندازه = (اندازه_t) ctx->alignment - اندازه;

        إذا ((off_t) اندازه > bsize) {
            اندازه = (اندازه_t) bsize;
        }
    }

    ctx->buf = ngx_create_لنډمهاله_buf(ctx->pool, اندازه);
    إذا (ctx->buf == عدم) {
        أرجع NGX_ERROR;
    }

    /*
     * we do not set ctx->buf->tag, because we do not want
     * to reuse the buf via ctx->free list
     */

#إذا (NGX_HAVE_ALIGNED_DIRECTIO)
    ctx->unaligned = 1;
#د_که_پای

    أرجع NGX_OK;
}


ساكن ngx_int_t
ngx_output_chain_get_buf(ngx_output_chain_ctx_t *ctx, off_t bsize)
{
    اندازه_t       اندازه;
    ngx_buf_t   *b, *دننه;
    ngx_uint_t   recycled;

    دننه = ctx->دننه->buf;
    اندازه = ctx->bufs.اندازه;
    recycled = 1;

    إذا (دننه->وروستی_دننه_chain) {

        إذا (bsize < (off_t) اندازه) {

            /*
             * allocate a small temp buf for a small last buf
             * or its small last part
             */

            اندازه = (اندازه_t) bsize;
            recycled = 0;

        } وإلا إذا (!ctx->directio
                   && ctx->bufs.عدد == 1
                   && (bsize < (off_t) (اندازه + اندازه / 4)))
        {
            /*
             * allocate a temp buf that equals to a last buf,
             * if there is no directio, the last buf size is lesser
             * than 1.25 of bufs.size and the temp buf is single
             */

            اندازه = (اندازه_t) bsize;
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

        b->پیل = ngx_pmemalign(ctx->pool, اندازه, (اندازه_t) ctx->alignment);
        إذا (b->پیل == عدم) {
            أرجع NGX_ERROR;
        }

    } وإلا {
        b->پیل = ngx_palloc(ctx->pool, اندازه);
        إذا (b->پیل == عدم) {
            أرجع NGX_ERROR;
        }
    }

    b->pos = b->پیل;
    b->وروستی = b->پیل;
    b->پای_ټکی = b->وروستی + اندازه;
    b->temporary = 1;
    b->نښان = ctx->نښان;
    b->recycled = recycled;

    ctx->buf = b;
    ctx->allocated++;

    أرجع NGX_OK;
}


ساكن ngx_int_t
ngx_output_chain_copy_buf(ngx_output_chain_ctx_t *ctx)
{
    off_t        اندازه;
    ssize_t      n;
    ngx_buf_t   *src, *dst;
    ngx_uint_t   sendfile;

    src = ctx->دننه->buf;
    dst = ctx->buf;

    اندازه = ngx_buf_اندازه(src);
    اندازه = ngx_min(اندازه, dst->پای_ټکی - dst->pos);

    sendfile = ctx->sendfile && !ctx->directio;

#إذا (NGX_SENDFILE_حد)

    إذا (src->دننه_file && src->file_pos >= NGX_SENDFILE_حد) {
        sendfile = 0;
    }

#د_که_پای

    إذا (ngx_buf_دننه_حافظه(src)) {
        ngx_memcpy(dst->pos, src->pos, (اندازه_t) اندازه);
        src->pos += (اندازه_t) اندازه;
        dst->وروستی += (اندازه_t) اندازه;

        إذا (src->دننه_file) {

            إذا (sendfile) {
                dst->دننه_file = 1;
                dst->file = src->file;
                dst->file_pos = src->file_pos;
                dst->file_وروستی = src->file_pos + اندازه;

            } وإلا {
                dst->دننه_file = 0;
            }

            src->file_pos += اندازه;

        } وإلا {
            dst->دننه_file = 0;
        }

        إذا (src->pos == src->وروستی) {
            dst->flush = src->flush;
            dst->وروستی_buf = src->وروستی_buf;
            dst->وروستی_دننه_chain = src->وروستی_دننه_chain;

        } وإلا {
            dst->flush = 0;
            dst->وروستی_buf = 0;
            dst->وروستی_دننه_chain = 0;
        }

    } وإلا {

#إذا (NGX_HAVE_ALIGNED_DIRECTIO)

        إذا (ctx->unaligned) {
            إذا (ngx_directio_off(src->file->fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, ctx->pool->لوګاریتم, ngx_errno,
                              ngx_directio_off_n " \"%s\" failed",
                              src->file->name.معلومات);
            }
        }

#د_که_پای

#إذا (NGX_HAVE_FILE_AIO)
        إذا (ctx->aio_handler) {
            n = ngx_file_aio_read(src->file, dst->pos, (اندازه_t) اندازه,
                                  src->file_pos, ctx->pool);
            إذا (n == NGX_AGAIN) {
                ctx->aio_handler(ctx, src->file);
                أرجع NGX_AGAIN;
            }

        } وإلا
#د_که_پای
#إذا (NGX_تارونه)
        إذا (ctx->thread_handler) {
            src->file->thread_task = ctx->thread_task;
            src->file->thread_handler = ctx->thread_handler;
            src->file->thread_ctx = ctx->filter_ctx;

            n = ngx_thread_read(src->file, dst->pos, (اندازه_t) اندازه,
                                src->file_pos, ctx->pool);
            إذا (n == NGX_AGAIN) {
                ctx->thread_task = src->file->thread_task;
                أرجع NGX_AGAIN;
            }

        } وإلا
#د_که_پای
        {
            n = ngx_read_file(src->file, dst->pos, (اندازه_t) اندازه,
                              src->file_pos);
        }

#إذا (NGX_HAVE_ALIGNED_DIRECTIO)

        إذا (ctx->unaligned) {
            ngx_err_t  err;

            err = ngx_errno;

            إذا (ngx_directio_on(src->file->fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, ctx->pool->لوګاریتم, ngx_errno,
                              ngx_directio_on_n " \"%s\" failed",
                              src->file->name.معلومات);
            }

            ngx_سېټ_errno(err);

            ctx->unaligned = 0;
        }

#د_که_پای

        إذا (n == NGX_ERROR) {
            أرجع (ngx_int_t) n;
        }

        إذا (n != اندازه) {
            ngx_log_error(NGX_LOG_ALERT, ctx->pool->لوګاریتم, 0,
                          ngx_read_file_n " read only %z of %O from \"%s\"",
                          n, اندازه, src->file->name.معلومات);
            أرجع NGX_ERROR;
        }

        dst->وروستی += n;

        إذا (sendfile) {
            dst->دننه_file = 1;
            dst->file = src->file;
            dst->file_pos = src->file_pos;
            dst->file_وروستی = src->file_pos + n;

        } وإلا {
            dst->دننه_file = 0;
        }

        src->file_pos += n;

        إذا (src->file_pos == src->file_وروستی) {
            dst->flush = src->flush;
            dst->وروستی_buf = src->وروستی_buf;
            dst->وروستی_دننه_chain = src->وروستی_دننه_chain;

        } وإلا {
            dst->flush = 0;
            dst->وروستی_buf = 0;
            dst->وروستی_دننه_chain = 0;
        }
    }

    أرجع NGX_OK;
}


ngx_int_t
ngx_chain_writer(فراغ *معلومات, ngx_chain_t *دننه)
{
    ngx_chain_writer_ctx_t *ctx = معلومات;

    off_t              اندازه;
    ngx_chain_t       *cl, *ln, *chain;
    ngx_connection_t  *c;

    c = ctx->connection;

    لكل (اندازه = 0; دننه; دننه = دننه->بل) {

        إذا (ngx_buf_اندازه(دننه->buf) == 0 && !ngx_buf_special(دننه->buf)) {

            ngx_log_error(NGX_LOG_ALERT, ctx->pool->لوګاریتم, 0,
                          "zero size buf in chain writer "
                          "t:%d r:%d f:%d %p %p-%p %p %O-%O",
                          دننه->buf->temporary,
                          دننه->buf->recycled,
                          دننه->buf->دننه_file,
                          دننه->buf->پیل,
                          دننه->buf->pos,
                          دننه->buf->وروستی,
                          دننه->buf->file,
                          دننه->buf->file_pos,
                          دننه->buf->file_وروستی);

            ngx_debug_ټکی();

            استمر;
        }

        إذا (ngx_buf_اندازه(دننه->buf) < 0) {

            ngx_log_error(NGX_LOG_ALERT, ctx->pool->لوګاریتم, 0,
                          "negative size buf in chain writer "
                          "t:%d r:%d f:%d %p %p-%p %p %O-%O",
                          دننه->buf->temporary,
                          دننه->buf->recycled,
                          دننه->buf->دننه_file,
                          دننه->buf->پیل,
                          دننه->buf->pos,
                          دننه->buf->وروستی,
                          دننه->buf->file,
                          دننه->buf->file_pos,
                          دننه->buf->file_وروستی);

            ngx_debug_ټکی();

            أرجع NGX_ERROR;
        }

        اندازه += ngx_buf_اندازه(دننه->buf);

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, c->لوګاریتم, 0,
                       "chain writer buf fl:%d s:%uO",
                       دننه->buf->flush, ngx_buf_اندازه(دننه->buf));

        cl = ngx_alloc_chain_link(ctx->pool);
        إذا (cl == عدم) {
            أرجع NGX_ERROR;
        }

        cl->buf = دننه->buf;
        cl->بل = عدم;
        *ctx->وروستی = cl;
        ctx->وروستی = &cl->بل;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->لوګاریتم, 0,
                   "chain writer in: %p", ctx->بهر);

    لكل (cl = ctx->بهر; cl; cl = cl->بل) {

        إذا (ngx_buf_اندازه(cl->buf) == 0 && !ngx_buf_special(cl->buf)) {

            ngx_log_error(NGX_LOG_ALERT, ctx->pool->لوګاریتم, 0,
                          "zero size buf in chain writer "
                          "t:%d r:%d f:%d %p %p-%p %p %O-%O",
                          cl->buf->temporary,
                          cl->buf->recycled,
                          cl->buf->دننه_file,
                          cl->buf->پیل,
                          cl->buf->pos,
                          cl->buf->وروستی,
                          cl->buf->file,
                          cl->buf->file_pos,
                          cl->buf->file_وروستی);

            ngx_debug_ټکی();

            استمر;
        }

        إذا (ngx_buf_اندازه(cl->buf) < 0) {

            ngx_log_error(NGX_LOG_ALERT, ctx->pool->لوګاریتم, 0,
                          "negative size buf in chain writer "
                          "t:%d r:%d f:%d %p %p-%p %p %O-%O",
                          cl->buf->temporary,
                          cl->buf->recycled,
                          cl->buf->دننه_file,
                          cl->buf->پیل,
                          cl->buf->pos,
                          cl->buf->وروستی,
                          cl->buf->file,
                          cl->buf->file_pos,
                          cl->buf->file_وروستی);

            ngx_debug_ټکی();

            أرجع NGX_ERROR;
        }

        اندازه += ngx_buf_اندازه(cl->buf);
    }

    إذا (اندازه == 0 && !c->buffered) {
        أرجع NGX_OK;
    }

    chain = c->send_chain(c, ctx->بهر, ctx->حد);

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->لوګاریتم, 0,
                   "chain writer out: %p", chain);

    إذا (chain == NGX_CHAIN_ERROR) {
        أرجع NGX_ERROR;
    }

    إذا (chain && c->اكتب->ready) {
        ngx_post_event(c->اكتب, &ngx_posted_بل_events);
    }

    لكل (cl = ctx->بهر; cl && cl != chain; /* void */) {
        ln = cl;
        cl = cl->بل;
        ngx_free_chain(ctx->pool, ln);
    }

    ctx->بهر = chain;

    إذا (ctx->بهر == عدم) {
        ctx->وروستی = &ctx->بهر;

        إذا (!c->buffered) {
            أرجع NGX_OK;
        }
    }

    أرجع NGX_AGAIN;
}
