
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>
#ضمن <ngx_event.h>


#إذا 0
#تعریف_کن NGX_SENDFILE_حد  4096
#پایان_اگر

/*
 * When DIRECTIO is enabled FreeBSD, Solaris, and MacOSX read directly
 * to an application memory from a device if parameters are aligned
 * to device sector boundary (512 bytes).  They fallback to usual read
 * operation if the parameters are not aligned.
 * Linux allows DIRECTIO only if the parameters are aligned to a filesystem
 * sector boundary, otherwise it returns EINVAL.  The sector size is
 * usually 512 bytes, however, on XFS it may be 4096 bytes.
 */

#تعریف_کن NGX_NONE            1


ساكن ngx_inline ngx_int_t
    ngx_output_chain_as_is(ngx_output_chain_ctx_t *ctx, ngx_buf_t *buf);
ساكن ngx_int_t ngx_output_chain_جمع_کن_copy(ngx_pool_t *pool,
    ngx_chain_t **chain, ngx_chain_t *درون);
ساكن ngx_int_t ngx_output_chain_align_file_buf(ngx_output_chain_ctx_t *ctx,
    off_t bsize);
ساكن ngx_int_t ngx_output_chain_get_buf(ngx_output_chain_ctx_t *ctx,
    off_t bsize);
ساكن ngx_int_t ngx_output_chain_copy_buf(ngx_output_chain_ctx_t *ctx);


ngx_int_t
ngx_output_chain(ngx_output_chain_ctx_t *ctx, ngx_chain_t *درون)
{
    off_t         bsize;
    ngx_int_t     rc, آخرین;
    ngx_chain_t  *cl, *بیرون, **آخرین_بیرون;

    إذا (ctx->درون == عدم && ctx->busy == عدم
#إذا (NGX_HAVE_FILE_AIO || NGX_نخها)
        && !ctx->aio
#پایان_اگر
       )
    {
        /*
         * the short path for the case when the ctx->in and ctx->busy chains
         * are empty, the incoming chain is empty too or has the single buf
         * that does not require the copy
         */

        إذا (درون == عدم) {
            أرجع ctx->output_filter(ctx->filter_ctx, درون);
        }

        إذا (درون->بعدی == عدم
#إذا (NGX_SENDFILE_حد)
            && !(درون->buf->درون_file && درون->buf->file_آخرین > NGX_SENDFILE_حد)
#پایان_اگر
            && ngx_output_chain_as_is(ctx, درون->buf))
        {
            أرجع ctx->output_filter(ctx->filter_ctx, درون);
        }
    }

    /* add the incoming buf to the chain ctx->in */

    إذا (درون) {
        إذا (ngx_output_chain_جمع_کن_copy(ctx->pool, &ctx->درون, درون) == NGX_ERROR) {
            أرجع NGX_ERROR;
        }
    }

    بیرون = عدم;
    آخرین_بیرون = &بیرون;
    آخرین = NGX_NONE;

    لكل ( ;; ) {

#إذا (NGX_HAVE_FILE_AIO || NGX_نخها)
        إذا (ctx->aio) {
            أرجع NGX_AGAIN;
        }
#پایان_اگر

        طالما (ctx->درون) {

            /*
             * cycle while there are the ctx->in bufs
             * and there are the free output bufs to copy in
             */

            bsize = ngx_buf_اندازه(ctx->درون->buf);

            إذا (bsize == 0 && !ngx_buf_special(ctx->درون->buf)) {

                ngx_log_error(NGX_LOG_ALERT, ctx->pool->لگاریتم, 0,
                              "zero size buf in output "
                              "t:%d r:%d f:%d %p %p-%p %p %O-%O",
                              ctx->درون->buf->temporary,
                              ctx->درون->buf->recycled,
                              ctx->درون->buf->درون_file,
                              ctx->درون->buf->شروع,
                              ctx->درون->buf->pos,
                              ctx->درون->buf->آخرین,
                              ctx->درون->buf->file,
                              ctx->درون->buf->file_pos,
                              ctx->درون->buf->file_آخرین);

                ngx_debug_نقطه();

                cl = ctx->درون;
                ctx->درون = cl->بعدی;

                ngx_free_chain(ctx->pool, cl);

                استمر;
            }

            إذا (bsize < 0) {

                ngx_log_error(NGX_LOG_ALERT, ctx->pool->لگاریتم, 0,
                              "negative size buf in output "
                              "t:%d r:%d f:%d %p %p-%p %p %O-%O",
                              ctx->درون->buf->temporary,
                              ctx->درون->buf->recycled,
                              ctx->درون->buf->درون_file,
                              ctx->درون->buf->شروع,
                              ctx->درون->buf->pos,
                              ctx->درون->buf->آخرین,
                              ctx->درون->buf->file,
                              ctx->درون->buf->file_pos,
                              ctx->درون->buf->file_آخرین);

                ngx_debug_نقطه();

                أرجع NGX_ERROR;
            }

            إذا (ngx_output_chain_as_is(ctx, ctx->درون->buf)) {

                /* move the chain link to the output chain */

                cl = ctx->درون;
                ctx->درون = cl->بعدی;

                *آخرین_بیرون = cl;
                آخرین_بیرون = &cl->بعدی;
                cl->بعدی = عدم;

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
                        ctx->حرر = cl->بعدی;

                        ngx_free_chain(ctx->pool, cl);

                    } وإلا إذا (بیرون || ctx->allocated == ctx->bufs.رقم) {

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
                إذا (بیرون) {
                    اكسر;
                }

                أرجع rc;
            }

            /* delete the completed buf from the ctx->in chain */

            إذا (ngx_buf_اندازه(ctx->درون->buf) == 0) {
                cl = ctx->درون;
                ctx->درون = cl->بعدی;

                ngx_free_chain(ctx->pool, cl);
            }

            cl = ngx_alloc_chain_link(ctx->pool);
            إذا (cl == عدم) {
                أرجع NGX_ERROR;
            }

            cl->buf = ctx->buf;
            cl->بعدی = عدم;
            *آخرین_بیرون = cl;
            آخرین_بیرون = &cl->بعدی;
            ctx->buf = عدم;
        }

        إذا (بیرون == عدم && آخرین != NGX_NONE) {

            إذا (ctx->درون) {
                أرجع NGX_AGAIN;
            }

            أرجع آخرین;
        }

        آخرین = ctx->output_filter(ctx->filter_ctx, بیرون);

        إذا (آخرین == NGX_ERROR || آخرین == NGX_انجامشده) {
            أرجع آخرین;
        }

        ngx_chain_update_chains(ctx->pool, &ctx->حرر, &ctx->busy, &بیرون,
                                ctx->برچسب);
        آخرین_بیرون = &بیرون;
    }
}


ساكن ngx_inline ngx_int_t
ngx_output_chain_as_is(ngx_output_chain_ctx_t *ctx, ngx_buf_t *buf)
{
    ngx_uint_t  sendfile;

    إذا (ngx_buf_special(buf)) {
        أرجع 1;
    }

#إذا (NGX_نخها)
    إذا (buf->درون_file) {
        buf->file->thread_handler = ctx->thread_handler;
        buf->file->thread_ctx = ctx->filter_ctx;
    }
#پایان_اگر

    sendfile = ctx->sendfile;

#إذا (NGX_SENDFILE_حد)

    إذا (buf->درون_file && buf->file_pos >= NGX_SENDFILE_حد) {
        sendfile = 0;
    }

#پایان_اگر

#إذا !(NGX_HAVE_SENDFILE_NODISKIO)

    /*
     * With DIRECTIO, disable sendfile() unless sendfile(SF_NOCACHE)
     * is available.
     */

    إذا (buf->درون_file && buf->file->directio) {
        sendfile = 0;
    }

#پایان_اگر

    إذا (!sendfile) {

        إذا (!ngx_buf_درون_حافظه(buf)) {
            أرجع 0;
        }

        buf->درون_file = 0;
    }

    إذا (ctx->need_درون_حافظه && !ngx_buf_درون_حافظه(buf)) {
        أرجع 0;
    }

    إذا (ctx->need_درون_موقت && (buf->حافظه || buf->mmap)) {
        أرجع 0;
    }

    أرجع 1;
}


ساكن ngx_int_t
ngx_output_chain_جمع_کن_copy(ngx_pool_t *pool, ngx_chain_t **chain,
    ngx_chain_t *درون)
{
    ngx_chain_t  *cl, **ll;
#إذا (NGX_SENDFILE_حد)
    ngx_buf_t    *b, *buf;
#پایان_اگر

    ll = chain;

    لكل (cl = *chain; cl; cl = cl->بعدی) {
        ll = &cl->بعدی;
    }

    طالما (درون) {

        cl = ngx_alloc_chain_link(pool);
        إذا (cl == عدم) {
            أرجع NGX_ERROR;
        }

#إذا (NGX_SENDFILE_حد)

        buf = درون->buf;

        إذا (buf->درون_file
            && buf->file_pos < NGX_SENDFILE_حد
            && buf->file_آخرین > NGX_SENDFILE_حد)
        {
            /* split a file buf on two bufs by the sendfile limit */

            b = ngx_calloc_buf(pool);
            إذا (b == عدم) {
                أرجع NGX_ERROR;
            }

            ngx_memcpy(b, buf, حجم(ngx_buf_t));

            إذا (ngx_buf_درون_حافظه(buf)) {
                buf->pos += (ssize_t) (NGX_SENDFILE_حد - buf->file_pos);
                b->آخرین = buf->pos;
            }

            buf->file_pos = NGX_SENDFILE_حد;
            b->file_آخرین = NGX_SENDFILE_حد;

            cl->buf = b;

        } وإلا {
            cl->buf = buf;
            درون = درون->بعدی;
        }

#وإلا
        cl->buf = درون->buf;
        درون = درون->بعدی;

#پایان_اگر

        cl->بعدی = عدم;
        *ll = cl;
        ll = &cl->بعدی;
    }

    أرجع NGX_OK;
}


ساكن ngx_int_t
ngx_output_chain_align_file_buf(ngx_output_chain_ctx_t *ctx, off_t bsize)
{
    اندازه_t      اندازه;
    ngx_buf_t  *درون;

    درون = ctx->درون->buf;

    إذا (درون->file == عدم || !درون->file->directio) {
        أرجع NGX_DECLINED;
    }

    ctx->directio = 1;

    اندازه = (اندازه_t) (درون->file_pos - (درون->file_pos & ~(ctx->alignment - 1)));

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

    ctx->buf = ngx_create_موقت_buf(ctx->pool, اندازه);
    إذا (ctx->buf == عدم) {
        أرجع NGX_ERROR;
    }

    /*
     * we do not set ctx->buf->tag, because we do not want
     * to reuse the buf via ctx->free list
     */

#إذا (NGX_HAVE_ALIGNED_DIRECTIO)
    ctx->unaligned = 1;
#پایان_اگر

    أرجع NGX_OK;
}


ساكن ngx_int_t
ngx_output_chain_get_buf(ngx_output_chain_ctx_t *ctx, off_t bsize)
{
    اندازه_t       اندازه;
    ngx_buf_t   *b, *درون;
    ngx_uint_t   recycled;

    درون = ctx->درون->buf;
    اندازه = ctx->bufs.اندازه;
    recycled = 1;

    إذا (درون->آخرین_درون_chain) {

        إذا (bsize < (off_t) اندازه) {

            /*
             * allocate a small temp buf for a small last buf
             * or its small last part
             */

            اندازه = (اندازه_t) bsize;
            recycled = 0;

        } وإلا إذا (!ctx->directio
                   && ctx->bufs.رقم == 1
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

        b->شروع = ngx_pmemalign(ctx->pool, اندازه, (اندازه_t) ctx->alignment);
        إذا (b->شروع == عدم) {
            أرجع NGX_ERROR;
        }

    } وإلا {
        b->شروع = ngx_palloc(ctx->pool, اندازه);
        إذا (b->شروع == عدم) {
            أرجع NGX_ERROR;
        }
    }

    b->pos = b->شروع;
    b->آخرین = b->شروع;
    b->انتها = b->آخرین + اندازه;
    b->temporary = 1;
    b->برچسب = ctx->برچسب;
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

    src = ctx->درون->buf;
    dst = ctx->buf;

    اندازه = ngx_buf_اندازه(src);
    اندازه = ngx_min(اندازه, dst->انتها - dst->pos);

    sendfile = ctx->sendfile && !ctx->directio;

#إذا (NGX_SENDFILE_حد)

    إذا (src->درون_file && src->file_pos >= NGX_SENDFILE_حد) {
        sendfile = 0;
    }

#پایان_اگر

    إذا (ngx_buf_درون_حافظه(src)) {
        ngx_memcpy(dst->pos, src->pos, (اندازه_t) اندازه);
        src->pos += (اندازه_t) اندازه;
        dst->آخرین += (اندازه_t) اندازه;

        إذا (src->درون_file) {

            إذا (sendfile) {
                dst->درون_file = 1;
                dst->file = src->file;
                dst->file_pos = src->file_pos;
                dst->file_آخرین = src->file_pos + اندازه;

            } وإلا {
                dst->درون_file = 0;
            }

            src->file_pos += اندازه;

        } وإلا {
            dst->درون_file = 0;
        }

        إذا (src->pos == src->آخرین) {
            dst->flush = src->flush;
            dst->آخرین_buf = src->آخرین_buf;
            dst->آخرین_درون_chain = src->آخرین_درون_chain;

        } وإلا {
            dst->flush = 0;
            dst->آخرین_buf = 0;
            dst->آخرین_درون_chain = 0;
        }

    } وإلا {

#إذا (NGX_HAVE_ALIGNED_DIRECTIO)

        إذا (ctx->unaligned) {
            إذا (ngx_directio_off(src->file->fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, ctx->pool->لگاریتم, ngx_errno,
                              ngx_directio_off_n " \"%s\" failed",
                              src->file->name.داده);
            }
        }

#پایان_اگر

#إذا (NGX_HAVE_FILE_AIO)
        إذا (ctx->aio_handler) {
            n = ngx_file_aio_read(src->file, dst->pos, (اندازه_t) اندازه,
                                  src->file_pos, ctx->pool);
            إذا (n == NGX_AGAIN) {
                ctx->aio_handler(ctx, src->file);
                أرجع NGX_AGAIN;
            }

        } وإلا
#پایان_اگر
#إذا (NGX_نخها)
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
#پایان_اگر
        {
            n = ngx_read_file(src->file, dst->pos, (اندازه_t) اندازه,
                              src->file_pos);
        }

#إذا (NGX_HAVE_ALIGNED_DIRECTIO)

        إذا (ctx->unaligned) {
            ngx_err_t  err;

            err = ngx_errno;

            إذا (ngx_directio_on(src->file->fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, ctx->pool->لگاریتم, ngx_errno,
                              ngx_directio_on_n " \"%s\" failed",
                              src->file->name.داده);
            }

            ngx_مجموعه_errno(err);

            ctx->unaligned = 0;
        }

#پایان_اگر

        إذا (n == NGX_ERROR) {
            أرجع (ngx_int_t) n;
        }

        إذا (n != اندازه) {
            ngx_log_error(NGX_LOG_ALERT, ctx->pool->لگاریتم, 0,
                          ngx_read_file_n " read only %z of %O from \"%s\"",
                          n, اندازه, src->file->name.داده);
            أرجع NGX_ERROR;
        }

        dst->آخرین += n;

        إذا (sendfile) {
            dst->درون_file = 1;
            dst->file = src->file;
            dst->file_pos = src->file_pos;
            dst->file_آخرین = src->file_pos + n;

        } وإلا {
            dst->درون_file = 0;
        }

        src->file_pos += n;

        إذا (src->file_pos == src->file_آخرین) {
            dst->flush = src->flush;
            dst->آخرین_buf = src->آخرین_buf;
            dst->آخرین_درون_chain = src->آخرین_درون_chain;

        } وإلا {
            dst->flush = 0;
            dst->آخرین_buf = 0;
            dst->آخرین_درون_chain = 0;
        }
    }

    أرجع NGX_OK;
}


ngx_int_t
ngx_chain_writer(فراغ *داده, ngx_chain_t *درون)
{
    ngx_chain_writer_ctx_t *ctx = داده;

    off_t              اندازه;
    ngx_chain_t       *cl, *ln, *chain;
    ngx_connection_t  *c;

    c = ctx->connection;

    لكل (اندازه = 0; درون; درون = درون->بعدی) {

        إذا (ngx_buf_اندازه(درون->buf) == 0 && !ngx_buf_special(درون->buf)) {

            ngx_log_error(NGX_LOG_ALERT, ctx->pool->لگاریتم, 0,
                          "zero size buf in chain writer "
                          "t:%d r:%d f:%d %p %p-%p %p %O-%O",
                          درون->buf->temporary,
                          درون->buf->recycled,
                          درون->buf->درون_file,
                          درون->buf->شروع,
                          درون->buf->pos,
                          درون->buf->آخرین,
                          درون->buf->file,
                          درون->buf->file_pos,
                          درون->buf->file_آخرین);

            ngx_debug_نقطه();

            استمر;
        }

        إذا (ngx_buf_اندازه(درون->buf) < 0) {

            ngx_log_error(NGX_LOG_ALERT, ctx->pool->لگاریتم, 0,
                          "negative size buf in chain writer "
                          "t:%d r:%d f:%d %p %p-%p %p %O-%O",
                          درون->buf->temporary,
                          درون->buf->recycled,
                          درون->buf->درون_file,
                          درون->buf->شروع,
                          درون->buf->pos,
                          درون->buf->آخرین,
                          درون->buf->file,
                          درون->buf->file_pos,
                          درون->buf->file_آخرین);

            ngx_debug_نقطه();

            أرجع NGX_ERROR;
        }

        اندازه += ngx_buf_اندازه(درون->buf);

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, c->لگاریتم, 0,
                       "chain writer buf fl:%d s:%uO",
                       درون->buf->flush, ngx_buf_اندازه(درون->buf));

        cl = ngx_alloc_chain_link(ctx->pool);
        إذا (cl == عدم) {
            أرجع NGX_ERROR;
        }

        cl->buf = درون->buf;
        cl->بعدی = عدم;
        *ctx->آخرین = cl;
        ctx->آخرین = &cl->بعدی;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->لگاریتم, 0,
                   "chain writer in: %p", ctx->بیرون);

    لكل (cl = ctx->بیرون; cl; cl = cl->بعدی) {

        إذا (ngx_buf_اندازه(cl->buf) == 0 && !ngx_buf_special(cl->buf)) {

            ngx_log_error(NGX_LOG_ALERT, ctx->pool->لگاریتم, 0,
                          "zero size buf in chain writer "
                          "t:%d r:%d f:%d %p %p-%p %p %O-%O",
                          cl->buf->temporary,
                          cl->buf->recycled,
                          cl->buf->درون_file,
                          cl->buf->شروع,
                          cl->buf->pos,
                          cl->buf->آخرین,
                          cl->buf->file,
                          cl->buf->file_pos,
                          cl->buf->file_آخرین);

            ngx_debug_نقطه();

            استمر;
        }

        إذا (ngx_buf_اندازه(cl->buf) < 0) {

            ngx_log_error(NGX_LOG_ALERT, ctx->pool->لگاریتم, 0,
                          "negative size buf in chain writer "
                          "t:%d r:%d f:%d %p %p-%p %p %O-%O",
                          cl->buf->temporary,
                          cl->buf->recycled,
                          cl->buf->درون_file,
                          cl->buf->شروع,
                          cl->buf->pos,
                          cl->buf->آخرین,
                          cl->buf->file,
                          cl->buf->file_pos,
                          cl->buf->file_آخرین);

            ngx_debug_نقطه();

            أرجع NGX_ERROR;
        }

        اندازه += ngx_buf_اندازه(cl->buf);
    }

    إذا (اندازه == 0 && !c->buffered) {
        أرجع NGX_OK;
    }

    chain = c->send_chain(c, ctx->بیرون, ctx->حد);

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->لگاریتم, 0,
                   "chain writer out: %p", chain);

    إذا (chain == NGX_CHAIN_ERROR) {
        أرجع NGX_ERROR;
    }

    إذا (chain && c->اكتب->ready) {
        ngx_post_event(c->اكتب, &ngx_posted_بعدی_events);
    }

    لكل (cl = ctx->بیرون; cl && cl != chain; /* void */) {
        ln = cl;
        cl = cl->بعدی;
        ngx_free_chain(ctx->pool, ln);
    }

    ctx->بیرون = chain;

    إذا (ctx->بیرون == عدم) {
        ctx->آخرین = &ctx->بیرون;

        إذا (!c->buffered) {
            أرجع NGX_OK;
        }
    }

    أرجع NGX_AGAIN;
}
