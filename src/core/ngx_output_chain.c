
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>
#ضمن <ngx_event.h>


#إذا 0
#وصف_ڪريو NGX_SENDFILE_حد_بندي  4096
#جيڪڏهن_پورو

/*
 * When DIRECTIO is enabled FreeBSD, Solaris, and MacOSX read directly
 * to an application memory from a device if parameters are aligned
 * to device sector boundary (512 bytes).  They fallback to usual read
 * operation if the parameters are not aligned.
 * Linux allows DIRECTIO only if the parameters are aligned to a filesystem
 * sector boundary, otherwise it returns EINVAL.  The sector size is
 * usually 512 bytes, however, on XFS it may be 4096 bytes.
 */

#وصف_ڪريو NGX_NONE            1


ساكن ngx_inline ngx_int_t
    ngx_output_chain_as_is(ngx_output_chain_ctx_t *ctx, ngx_buf_t *buf);
ساكن ngx_int_t ngx_output_chain_جوڙيو_copy(ngx_pool_t *pool,
    ngx_chain_t **chain, ngx_chain_t *اندريون);
ساكن ngx_int_t ngx_output_chain_align_file_buf(ngx_output_chain_ctx_t *ctx,
    off_t bsize);
ساكن ngx_int_t ngx_output_chain_get_buf(ngx_output_chain_ctx_t *ctx,
    off_t bsize);
ساكن ngx_int_t ngx_output_chain_copy_buf(ngx_output_chain_ctx_t *ctx);


ngx_int_t
ngx_output_chain(ngx_output_chain_ctx_t *ctx, ngx_chain_t *اندريون)
{
    off_t         bsize;
    ngx_int_t     rc, آخري;
    ngx_chain_t  *cl, *ٻاهريون, **آخري_ٻاهريون;

    إذا (ctx->اندريون == عدم && ctx->busy == عدم
#إذا (NGX_HAVE_FILE_AIO || NGX_ڌاڳا)
        && !ctx->aio
#جيڪڏهن_پورو
       )
    {
        /*
         * the short path for the case when the ctx->in and ctx->busy chains
         * are empty, the incoming chain is empty too or has the single buf
         * that does not require the copy
         */

        إذا (اندريون == عدم) {
            أرجع ctx->output_filter(ctx->filter_ctx, اندريون);
        }

        إذا (اندريون->ايندڙ == عدم
#إذا (NGX_SENDFILE_حد_بندي)
            && !(اندريون->buf->اندريون_file && اندريون->buf->file_آخري > NGX_SENDFILE_حد_بندي)
#جيڪڏهن_پورو
            && ngx_output_chain_as_is(ctx, اندريون->buf))
        {
            أرجع ctx->output_filter(ctx->filter_ctx, اندريون);
        }
    }

    /* add the incoming buf to the chain ctx->in */

    إذا (اندريون) {
        إذا (ngx_output_chain_جوڙيو_copy(ctx->pool, &ctx->اندريون, اندريون) == NGX_ERROR) {
            أرجع NGX_ERROR;
        }
    }

    ٻاهريون = عدم;
    آخري_ٻاهريون = &ٻاهريون;
    آخري = NGX_NONE;

    لكل ( ;; ) {

#إذا (NGX_HAVE_FILE_AIO || NGX_ڌاڳا)
        إذا (ctx->aio) {
            أرجع NGX_AGAIN;
        }
#جيڪڏهن_پورو

        طالما (ctx->اندريون) {

            /*
             * cycle while there are the ctx->in bufs
             * and there are the free output bufs to copy in
             */

            bsize = ngx_buf_سائيز(ctx->اندريون->buf);

            إذا (bsize == 0 && !ngx_buf_special(ctx->اندريون->buf)) {

                ngx_log_error(NGX_LOG_ALERT, ctx->pool->لاگرٿم, 0,
                              "zero size buf in output "
                              "t:%d r:%d f:%d %p %p-%p %p %O-%O",
                              ctx->اندريون->buf->temporary,
                              ctx->اندريون->buf->recycled,
                              ctx->اندريون->buf->اندريون_file,
                              ctx->اندريون->buf->شروعات,
                              ctx->اندريون->buf->pos,
                              ctx->اندريون->buf->آخري,
                              ctx->اندريون->buf->file,
                              ctx->اندريون->buf->file_pos,
                              ctx->اندريون->buf->file_آخري);

                ngx_debug_نقطو();

                cl = ctx->اندريون;
                ctx->اندريون = cl->ايندڙ;

                ngx_free_chain(ctx->pool, cl);

                استمر;
            }

            إذا (bsize < 0) {

                ngx_log_error(NGX_LOG_ALERT, ctx->pool->لاگرٿم, 0,
                              "negative size buf in output "
                              "t:%d r:%d f:%d %p %p-%p %p %O-%O",
                              ctx->اندريون->buf->temporary,
                              ctx->اندريون->buf->recycled,
                              ctx->اندريون->buf->اندريون_file,
                              ctx->اندريون->buf->شروعات,
                              ctx->اندريون->buf->pos,
                              ctx->اندريون->buf->آخري,
                              ctx->اندريون->buf->file,
                              ctx->اندريون->buf->file_pos,
                              ctx->اندريون->buf->file_آخري);

                ngx_debug_نقطو();

                أرجع NGX_ERROR;
            }

            إذا (ngx_output_chain_as_is(ctx, ctx->اندريون->buf)) {

                /* move the chain link to the output chain */

                cl = ctx->اندريون;
                ctx->اندريون = cl->ايندڙ;

                *آخري_ٻاهريون = cl;
                آخري_ٻاهريون = &cl->ايندڙ;
                cl->ايندڙ = عدم;

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
                        ctx->حرر = cl->ايندڙ;

                        ngx_free_chain(ctx->pool, cl);

                    } وإلا إذا (ٻاهريون || ctx->allocated == ctx->bufs.انگ) {

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
                إذا (ٻاهريون) {
                    اكسر;
                }

                أرجع rc;
            }

            /* delete the completed buf from the ctx->in chain */

            إذا (ngx_buf_سائيز(ctx->اندريون->buf) == 0) {
                cl = ctx->اندريون;
                ctx->اندريون = cl->ايندڙ;

                ngx_free_chain(ctx->pool, cl);
            }

            cl = ngx_alloc_chain_link(ctx->pool);
            إذا (cl == عدم) {
                أرجع NGX_ERROR;
            }

            cl->buf = ctx->buf;
            cl->ايندڙ = عدم;
            *آخري_ٻاهريون = cl;
            آخري_ٻاهريون = &cl->ايندڙ;
            ctx->buf = عدم;
        }

        إذا (ٻاهريون == عدم && آخري != NGX_NONE) {

            إذا (ctx->اندريون) {
                أرجع NGX_AGAIN;
            }

            أرجع آخري;
        }

        آخري = ctx->output_filter(ctx->filter_ctx, ٻاهريون);

        إذا (آخري == NGX_ERROR || آخري == NGX_پورو) {
            أرجع آخري;
        }

        ngx_chain_update_chains(ctx->pool, &ctx->حرر, &ctx->busy, &ٻاهريون,
                                ctx->ٽيگ);
        آخري_ٻاهريون = &ٻاهريون;
    }
}


ساكن ngx_inline ngx_int_t
ngx_output_chain_as_is(ngx_output_chain_ctx_t *ctx, ngx_buf_t *buf)
{
    ngx_uint_t  sendfile;

    إذا (ngx_buf_special(buf)) {
        أرجع 1;
    }

#إذا (NGX_ڌاڳا)
    إذا (buf->اندريون_file) {
        buf->file->thread_handler = ctx->thread_handler;
        buf->file->thread_ctx = ctx->filter_ctx;
    }
#جيڪڏهن_پورو

    sendfile = ctx->sendfile;

#إذا (NGX_SENDFILE_حد_بندي)

    إذا (buf->اندريون_file && buf->file_pos >= NGX_SENDFILE_حد_بندي) {
        sendfile = 0;
    }

#جيڪڏهن_پورو

#إذا !(NGX_HAVE_SENDFILE_NODISKIO)

    /*
     * With DIRECTIO, disable sendfile() unless sendfile(SF_NOCACHE)
     * is available.
     */

    إذا (buf->اندريون_file && buf->file->directio) {
        sendfile = 0;
    }

#جيڪڏهن_پورو

    إذا (!sendfile) {

        إذا (!ngx_buf_اندريون_يادگيري(buf)) {
            أرجع 0;
        }

        buf->اندريون_file = 0;
    }

    إذا (ctx->need_اندريون_يادگيري && !ngx_buf_اندريون_يادگيري(buf)) {
        أرجع 0;
    }

    إذا (ctx->need_اندريون_عارضي && (buf->يادگيري || buf->mmap)) {
        أرجع 0;
    }

    أرجع 1;
}


ساكن ngx_int_t
ngx_output_chain_جوڙيو_copy(ngx_pool_t *pool, ngx_chain_t **chain,
    ngx_chain_t *اندريون)
{
    ngx_chain_t  *cl, **ll;
#إذا (NGX_SENDFILE_حد_بندي)
    ngx_buf_t    *b, *buf;
#جيڪڏهن_پورو

    ll = chain;

    لكل (cl = *chain; cl; cl = cl->ايندڙ) {
        ll = &cl->ايندڙ;
    }

    طالما (اندريون) {

        cl = ngx_alloc_chain_link(pool);
        إذا (cl == عدم) {
            أرجع NGX_ERROR;
        }

#إذا (NGX_SENDFILE_حد_بندي)

        buf = اندريون->buf;

        إذا (buf->اندريون_file
            && buf->file_pos < NGX_SENDFILE_حد_بندي
            && buf->file_آخري > NGX_SENDFILE_حد_بندي)
        {
            /* split a file buf on two bufs by the sendfile limit */

            b = ngx_calloc_buf(pool);
            إذا (b == عدم) {
                أرجع NGX_ERROR;
            }

            ngx_memcpy(b, buf, حجم(ngx_buf_t));

            إذا (ngx_buf_اندريون_يادگيري(buf)) {
                buf->pos += (ssize_t) (NGX_SENDFILE_حد_بندي - buf->file_pos);
                b->آخري = buf->pos;
            }

            buf->file_pos = NGX_SENDFILE_حد_بندي;
            b->file_آخري = NGX_SENDFILE_حد_بندي;

            cl->buf = b;

        } وإلا {
            cl->buf = buf;
            اندريون = اندريون->ايندڙ;
        }

#وإلا
        cl->buf = اندريون->buf;
        اندريون = اندريون->ايندڙ;

#جيڪڏهن_پورو

        cl->ايندڙ = عدم;
        *ll = cl;
        ll = &cl->ايندڙ;
    }

    أرجع NGX_OK;
}


ساكن ngx_int_t
ngx_output_chain_align_file_buf(ngx_output_chain_ctx_t *ctx, off_t bsize)
{
    سائيز_t      سائيز;
    ngx_buf_t  *اندريون;

    اندريون = ctx->اندريون->buf;

    إذا (اندريون->file == عدم || !اندريون->file->directio) {
        أرجع NGX_DECLINED;
    }

    ctx->directio = 1;

    سائيز = (سائيز_t) (اندريون->file_pos - (اندريون->file_pos & ~(ctx->alignment - 1)));

    إذا (سائيز == 0) {

        إذا (bsize >= (off_t) ctx->bufs.سائيز) {
            أرجع NGX_DECLINED;
        }

        سائيز = (سائيز_t) bsize;

    } وإلا {
        سائيز = (سائيز_t) ctx->alignment - سائيز;

        إذا ((off_t) سائيز > bsize) {
            سائيز = (سائيز_t) bsize;
        }
    }

    ctx->buf = ngx_create_عارضي_buf(ctx->pool, سائيز);
    إذا (ctx->buf == عدم) {
        أرجع NGX_ERROR;
    }

    /*
     * we do not set ctx->buf->tag, because we do not want
     * to reuse the buf via ctx->free list
     */

#إذا (NGX_HAVE_ALIGNED_DIRECTIO)
    ctx->unaligned = 1;
#جيڪڏهن_پورو

    أرجع NGX_OK;
}


ساكن ngx_int_t
ngx_output_chain_get_buf(ngx_output_chain_ctx_t *ctx, off_t bsize)
{
    سائيز_t       سائيز;
    ngx_buf_t   *b, *اندريون;
    ngx_uint_t   recycled;

    اندريون = ctx->اندريون->buf;
    سائيز = ctx->bufs.سائيز;
    recycled = 1;

    إذا (اندريون->آخري_اندريون_chain) {

        إذا (bsize < (off_t) سائيز) {

            /*
             * allocate a small temp buf for a small last buf
             * or its small last part
             */

            سائيز = (سائيز_t) bsize;
            recycled = 0;

        } وإلا إذا (!ctx->directio
                   && ctx->bufs.انگ == 1
                   && (bsize < (off_t) (سائيز + سائيز / 4)))
        {
            /*
             * allocate a temp buf that equals to a last buf,
             * if there is no directio, the last buf size is lesser
             * than 1.25 of bufs.size and the temp buf is single
             */

            سائيز = (سائيز_t) bsize;
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

        b->شروعات = ngx_pmemalign(ctx->pool, سائيز, (سائيز_t) ctx->alignment);
        إذا (b->شروعات == عدم) {
            أرجع NGX_ERROR;
        }

    } وإلا {
        b->شروعات = ngx_palloc(ctx->pool, سائيز);
        إذا (b->شروعات == عدم) {
            أرجع NGX_ERROR;
        }
    }

    b->pos = b->شروعات;
    b->آخري = b->شروعات;
    b->پڇاڙي = b->آخري + سائيز;
    b->temporary = 1;
    b->ٽيگ = ctx->ٽيگ;
    b->recycled = recycled;

    ctx->buf = b;
    ctx->allocated++;

    أرجع NGX_OK;
}


ساكن ngx_int_t
ngx_output_chain_copy_buf(ngx_output_chain_ctx_t *ctx)
{
    off_t        سائيز;
    ssize_t      n;
    ngx_buf_t   *src, *dst;
    ngx_uint_t   sendfile;

    src = ctx->اندريون->buf;
    dst = ctx->buf;

    سائيز = ngx_buf_سائيز(src);
    سائيز = ngx_min(سائيز, dst->پڇاڙي - dst->pos);

    sendfile = ctx->sendfile && !ctx->directio;

#إذا (NGX_SENDFILE_حد_بندي)

    إذا (src->اندريون_file && src->file_pos >= NGX_SENDFILE_حد_بندي) {
        sendfile = 0;
    }

#جيڪڏهن_پورو

    إذا (ngx_buf_اندريون_يادگيري(src)) {
        ngx_memcpy(dst->pos, src->pos, (سائيز_t) سائيز);
        src->pos += (سائيز_t) سائيز;
        dst->آخري += (سائيز_t) سائيز;

        إذا (src->اندريون_file) {

            إذا (sendfile) {
                dst->اندريون_file = 1;
                dst->file = src->file;
                dst->file_pos = src->file_pos;
                dst->file_آخري = src->file_pos + سائيز;

            } وإلا {
                dst->اندريون_file = 0;
            }

            src->file_pos += سائيز;

        } وإلا {
            dst->اندريون_file = 0;
        }

        إذا (src->pos == src->آخري) {
            dst->flush = src->flush;
            dst->آخري_buf = src->آخري_buf;
            dst->آخري_اندريون_chain = src->آخري_اندريون_chain;

        } وإلا {
            dst->flush = 0;
            dst->آخري_buf = 0;
            dst->آخري_اندريون_chain = 0;
        }

    } وإلا {

#إذا (NGX_HAVE_ALIGNED_DIRECTIO)

        إذا (ctx->unaligned) {
            إذا (ngx_directio_off(src->file->fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, ctx->pool->لاگرٿم, ngx_errno,
                              ngx_directio_off_n " \"%s\" failed",
                              src->file->name.معلومات);
            }
        }

#جيڪڏهن_پورو

#إذا (NGX_HAVE_FILE_AIO)
        إذا (ctx->aio_handler) {
            n = ngx_file_aio_read(src->file, dst->pos, (سائيز_t) سائيز,
                                  src->file_pos, ctx->pool);
            إذا (n == NGX_AGAIN) {
                ctx->aio_handler(ctx, src->file);
                أرجع NGX_AGAIN;
            }

        } وإلا
#جيڪڏهن_پورو
#إذا (NGX_ڌاڳا)
        إذا (ctx->thread_handler) {
            src->file->thread_task = ctx->thread_task;
            src->file->thread_handler = ctx->thread_handler;
            src->file->thread_ctx = ctx->filter_ctx;

            n = ngx_thread_read(src->file, dst->pos, (سائيز_t) سائيز,
                                src->file_pos, ctx->pool);
            إذا (n == NGX_AGAIN) {
                ctx->thread_task = src->file->thread_task;
                أرجع NGX_AGAIN;
            }

        } وإلا
#جيڪڏهن_پورو
        {
            n = ngx_read_file(src->file, dst->pos, (سائيز_t) سائيز,
                              src->file_pos);
        }

#إذا (NGX_HAVE_ALIGNED_DIRECTIO)

        إذا (ctx->unaligned) {
            ngx_err_t  err;

            err = ngx_errno;

            إذا (ngx_directio_on(src->file->fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, ctx->pool->لاگرٿم, ngx_errno,
                              ngx_directio_on_n " \"%s\" failed",
                              src->file->name.معلومات);
            }

            ngx_سيٽ_errno(err);

            ctx->unaligned = 0;
        }

#جيڪڏهن_پورو

        إذا (n == NGX_ERROR) {
            أرجع (ngx_int_t) n;
        }

        إذا (n != سائيز) {
            ngx_log_error(NGX_LOG_ALERT, ctx->pool->لاگرٿم, 0,
                          ngx_read_file_n " read only %z of %O from \"%s\"",
                          n, سائيز, src->file->name.معلومات);
            أرجع NGX_ERROR;
        }

        dst->آخري += n;

        إذا (sendfile) {
            dst->اندريون_file = 1;
            dst->file = src->file;
            dst->file_pos = src->file_pos;
            dst->file_آخري = src->file_pos + n;

        } وإلا {
            dst->اندريون_file = 0;
        }

        src->file_pos += n;

        إذا (src->file_pos == src->file_آخري) {
            dst->flush = src->flush;
            dst->آخري_buf = src->آخري_buf;
            dst->آخري_اندريون_chain = src->آخري_اندريون_chain;

        } وإلا {
            dst->flush = 0;
            dst->آخري_buf = 0;
            dst->آخري_اندريون_chain = 0;
        }
    }

    أرجع NGX_OK;
}


ngx_int_t
ngx_chain_writer(فراغ *معلومات, ngx_chain_t *اندريون)
{
    ngx_chain_writer_ctx_t *ctx = معلومات;

    off_t              سائيز;
    ngx_chain_t       *cl, *ln, *chain;
    ngx_connection_t  *c;

    c = ctx->connection;

    لكل (سائيز = 0; اندريون; اندريون = اندريون->ايندڙ) {

        إذا (ngx_buf_سائيز(اندريون->buf) == 0 && !ngx_buf_special(اندريون->buf)) {

            ngx_log_error(NGX_LOG_ALERT, ctx->pool->لاگرٿم, 0,
                          "zero size buf in chain writer "
                          "t:%d r:%d f:%d %p %p-%p %p %O-%O",
                          اندريون->buf->temporary,
                          اندريون->buf->recycled,
                          اندريون->buf->اندريون_file,
                          اندريون->buf->شروعات,
                          اندريون->buf->pos,
                          اندريون->buf->آخري,
                          اندريون->buf->file,
                          اندريون->buf->file_pos,
                          اندريون->buf->file_آخري);

            ngx_debug_نقطو();

            استمر;
        }

        إذا (ngx_buf_سائيز(اندريون->buf) < 0) {

            ngx_log_error(NGX_LOG_ALERT, ctx->pool->لاگرٿم, 0,
                          "negative size buf in chain writer "
                          "t:%d r:%d f:%d %p %p-%p %p %O-%O",
                          اندريون->buf->temporary,
                          اندريون->buf->recycled,
                          اندريون->buf->اندريون_file,
                          اندريون->buf->شروعات,
                          اندريون->buf->pos,
                          اندريون->buf->آخري,
                          اندريون->buf->file,
                          اندريون->buf->file_pos,
                          اندريون->buf->file_آخري);

            ngx_debug_نقطو();

            أرجع NGX_ERROR;
        }

        سائيز += ngx_buf_سائيز(اندريون->buf);

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, c->لاگرٿم, 0,
                       "chain writer buf fl:%d s:%uO",
                       اندريون->buf->flush, ngx_buf_سائيز(اندريون->buf));

        cl = ngx_alloc_chain_link(ctx->pool);
        إذا (cl == عدم) {
            أرجع NGX_ERROR;
        }

        cl->buf = اندريون->buf;
        cl->ايندڙ = عدم;
        *ctx->آخري = cl;
        ctx->آخري = &cl->ايندڙ;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->لاگرٿم, 0,
                   "chain writer in: %p", ctx->ٻاهريون);

    لكل (cl = ctx->ٻاهريون; cl; cl = cl->ايندڙ) {

        إذا (ngx_buf_سائيز(cl->buf) == 0 && !ngx_buf_special(cl->buf)) {

            ngx_log_error(NGX_LOG_ALERT, ctx->pool->لاگرٿم, 0,
                          "zero size buf in chain writer "
                          "t:%d r:%d f:%d %p %p-%p %p %O-%O",
                          cl->buf->temporary,
                          cl->buf->recycled,
                          cl->buf->اندريون_file,
                          cl->buf->شروعات,
                          cl->buf->pos,
                          cl->buf->آخري,
                          cl->buf->file,
                          cl->buf->file_pos,
                          cl->buf->file_آخري);

            ngx_debug_نقطو();

            استمر;
        }

        إذا (ngx_buf_سائيز(cl->buf) < 0) {

            ngx_log_error(NGX_LOG_ALERT, ctx->pool->لاگرٿم, 0,
                          "negative size buf in chain writer "
                          "t:%d r:%d f:%d %p %p-%p %p %O-%O",
                          cl->buf->temporary,
                          cl->buf->recycled,
                          cl->buf->اندريون_file,
                          cl->buf->شروعات,
                          cl->buf->pos,
                          cl->buf->آخري,
                          cl->buf->file,
                          cl->buf->file_pos,
                          cl->buf->file_آخري);

            ngx_debug_نقطو();

            أرجع NGX_ERROR;
        }

        سائيز += ngx_buf_سائيز(cl->buf);
    }

    إذا (سائيز == 0 && !c->buffered) {
        أرجع NGX_OK;
    }

    chain = c->send_chain(c, ctx->ٻاهريون, ctx->حد_بندي);

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->لاگرٿم, 0,
                   "chain writer out: %p", chain);

    إذا (chain == NGX_CHAIN_ERROR) {
        أرجع NGX_ERROR;
    }

    إذا (chain && c->اكتب->ready) {
        ngx_post_event(c->اكتب, &ngx_posted_ايندڙ_events);
    }

    لكل (cl = ctx->ٻاهريون; cl && cl != chain; /* void */) {
        ln = cl;
        cl = cl->ايندڙ;
        ngx_free_chain(ctx->pool, ln);
    }

    ctx->ٻاهريون = chain;

    إذا (ctx->ٻاهريون == عدم) {
        ctx->آخري = &ctx->ٻاهريون;

        إذا (!c->buffered) {
            أرجع NGX_OK;
        }
    }

    أرجع NGX_AGAIN;
}
