
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#شامل <ngx_config.h>
#شامل <ngx_core.h>
#شامل <ngx_event.h>


#اگر 0
#تعریف_کرو NGX_SENDFILE_حد_بندی  4096
#اگر_ختم

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


ساکن ngx_inline ngx_int_t
    ngx_output_chain_as_is(ngx_output_chain_ctx_t *ctx, ngx_buf_t *buf);
ساکن ngx_int_t ngx_output_chain_جوڑو_copy(ngx_pool_t *pool,
    ngx_chain_t **chain, ngx_chain_t *اندر);
ساکن ngx_int_t ngx_output_chain_align_file_buf(ngx_output_chain_ctx_t *ctx,
    off_t bsize);
ساکن ngx_int_t ngx_output_chain_get_buf(ngx_output_chain_ctx_t *ctx,
    off_t bsize);
ساکن ngx_int_t ngx_output_chain_copy_buf(ngx_output_chain_ctx_t *ctx);


ngx_int_t
ngx_output_chain(ngx_output_chain_ctx_t *ctx, ngx_chain_t *اندر)
{
    off_t         bsize;
    ngx_int_t     rc, آخری;
    ngx_chain_t  *cl, *برآمد, **آخری_برآمد;

    اگر (ctx->اندر == تہی && ctx->busy == تہی
#اگر (NGX_HAVE_FILE_AIO || NGX_دھاگے)
        && !ctx->aio
#اگر_ختم
       )
    {
        /*
         * the short path for the case when the ctx->in and ctx->busy chains
         * are empty, the incoming chain is empty too or has the single buf
         * that does not require the copy
         */

        اگر (اندر == تہی) {
            واپس ctx->output_filter(ctx->filter_ctx, اندر);
        }

        اگر (اندر->اگلا == تہی
#اگر (NGX_SENDFILE_حد_بندی)
            && !(اندر->buf->اندر_file && اندر->buf->file_آخری > NGX_SENDFILE_حد_بندی)
#اگر_ختم
            && ngx_output_chain_as_is(ctx, اندر->buf))
        {
            واپس ctx->output_filter(ctx->filter_ctx, اندر);
        }
    }

    /* add the incoming buf to the chain ctx->in */

    اگر (اندر) {
        اگر (ngx_output_chain_جوڑو_copy(ctx->pool, &ctx->اندر, اندر) == NGX_ERROR) {
            واپس NGX_ERROR;
        }
    }

    برآمد = تہی;
    آخری_برآمد = &برآمد;
    آخری = NGX_NONE;

    برائے ( ;; ) {

#اگر (NGX_HAVE_FILE_AIO || NGX_دھاگے)
        اگر (ctx->aio) {
            واپس NGX_AGAIN;
        }
#اگر_ختم

        جب (ctx->اندر) {

            /*
             * cycle while there are the ctx->in bufs
             * and there are the free output bufs to copy in
             */

            bsize = ngx_buf_سائز(ctx->اندر->buf);

            اگر (bsize == 0 && !ngx_buf_special(ctx->اندر->buf)) {

                ngx_log_error(NGX_LOG_ALERT, ctx->pool->لوگارتھم, 0,
                              "zero size buf in output "
                              "t:%d r:%d f:%d %p %p-%p %p %O-%O",
                              ctx->اندر->buf->temporary,
                              ctx->اندر->buf->recycled,
                              ctx->اندر->buf->اندر_file,
                              ctx->اندر->buf->آغاز,
                              ctx->اندر->buf->pos,
                              ctx->اندر->buf->آخری,
                              ctx->اندر->buf->file,
                              ctx->اندر->buf->file_pos,
                              ctx->اندر->buf->file_آخری);

                ngx_debug_نقطہ();

                cl = ctx->اندر;
                ctx->اندر = cl->اگلا;

                ngx_free_chain(ctx->pool, cl);

                جاری;
            }

            اگر (bsize < 0) {

                ngx_log_error(NGX_LOG_ALERT, ctx->pool->لوگارتھم, 0,
                              "negative size buf in output "
                              "t:%d r:%d f:%d %p %p-%p %p %O-%O",
                              ctx->اندر->buf->temporary,
                              ctx->اندر->buf->recycled,
                              ctx->اندر->buf->اندر_file,
                              ctx->اندر->buf->آغاز,
                              ctx->اندر->buf->pos,
                              ctx->اندر->buf->آخری,
                              ctx->اندر->buf->file,
                              ctx->اندر->buf->file_pos,
                              ctx->اندر->buf->file_آخری);

                ngx_debug_نقطہ();

                واپس NGX_ERROR;
            }

            اگر (ngx_output_chain_as_is(ctx, ctx->اندر->buf)) {

                /* move the chain link to the output chain */

                cl = ctx->اندر;
                ctx->اندر = cl->اگلا;

                *آخری_برآمد = cl;
                آخری_برآمد = &cl->اگلا;
                cl->اگلا = تہی;

                جاری;
            }

            اگر (ctx->buf == تہی) {

                rc = ngx_output_chain_align_file_buf(ctx, bsize);

                اگر (rc == NGX_ERROR) {
                    واپس NGX_ERROR;
                }

                اگر (rc != NGX_OK) {

                    اگر (ctx->آزاد) {

                        /* get the free buf */

                        cl = ctx->آزاد;
                        ctx->buf = cl->buf;
                        ctx->آزاد = cl->اگلا;

                        ngx_free_chain(ctx->pool, cl);

                    } ورنہ اگر (برآمد || ctx->allocated == ctx->bufs.عدد) {

                        توڑو;

                    } ورنہ اگر (ngx_output_chain_get_buf(ctx, bsize) != NGX_OK) {
                        واپس NGX_ERROR;
                    }
                }
            }

            rc = ngx_output_chain_copy_buf(ctx);

            اگر (rc == NGX_ERROR) {
                واپس rc;
            }

            اگر (rc == NGX_AGAIN) {
                اگر (برآمد) {
                    توڑو;
                }

                واپس rc;
            }

            /* delete the completed buf from the ctx->in chain */

            اگر (ngx_buf_سائز(ctx->اندر->buf) == 0) {
                cl = ctx->اندر;
                ctx->اندر = cl->اگلا;

                ngx_free_chain(ctx->pool, cl);
            }

            cl = ngx_alloc_chain_link(ctx->pool);
            اگر (cl == تہی) {
                واپس NGX_ERROR;
            }

            cl->buf = ctx->buf;
            cl->اگلا = تہی;
            *آخری_برآمد = cl;
            آخری_برآمد = &cl->اگلا;
            ctx->buf = تہی;
        }

        اگر (برآمد == تہی && آخری != NGX_NONE) {

            اگر (ctx->اندر) {
                واپس NGX_AGAIN;
            }

            واپس آخری;
        }

        آخری = ctx->output_filter(ctx->filter_ctx, برآمد);

        اگر (آخری == NGX_ERROR || آخری == NGX_مکمل) {
            واپس آخری;
        }

        ngx_chain_update_chains(ctx->pool, &ctx->آزاد, &ctx->busy, &برآمد,
                                ctx->لیبل);
        آخری_برآمد = &برآمد;
    }
}


ساکن ngx_inline ngx_int_t
ngx_output_chain_as_is(ngx_output_chain_ctx_t *ctx, ngx_buf_t *buf)
{
    ngx_uint_t  sendfile;

    اگر (ngx_buf_special(buf)) {
        واپس 1;
    }

#اگر (NGX_دھاگے)
    اگر (buf->اندر_file) {
        buf->file->thread_handler = ctx->thread_handler;
        buf->file->thread_ctx = ctx->filter_ctx;
    }
#اگر_ختم

    sendfile = ctx->sendfile;

#اگر (NGX_SENDFILE_حد_بندی)

    اگر (buf->اندر_file && buf->file_pos >= NGX_SENDFILE_حد_بندی) {
        sendfile = 0;
    }

#اگر_ختم

#اگر !(NGX_HAVE_SENDFILE_NODISKIO)

    /*
     * With DIRECTIO, disable sendfile() unless sendfile(SF_NOCACHE)
     * is available.
     */

    اگر (buf->اندر_file && buf->file->directio) {
        sendfile = 0;
    }

#اگر_ختم

    اگر (!sendfile) {

        اگر (!ngx_buf_اندر_حافظہ(buf)) {
            واپس 0;
        }

        buf->اندر_file = 0;
    }

    اگر (ctx->need_اندر_حافظہ && !ngx_buf_اندر_حافظہ(buf)) {
        واپس 0;
    }

    اگر (ctx->need_اندر_عارضی && (buf->حافظہ || buf->mmap)) {
        واپس 0;
    }

    واپس 1;
}


ساکن ngx_int_t
ngx_output_chain_جوڑو_copy(ngx_pool_t *pool, ngx_chain_t **chain,
    ngx_chain_t *اندر)
{
    ngx_chain_t  *cl, **ll;
#اگر (NGX_SENDFILE_حد_بندی)
    ngx_buf_t    *b, *buf;
#اگر_ختم

    ll = chain;

    برائے (cl = *chain; cl; cl = cl->اگلا) {
        ll = &cl->اگلا;
    }

    جب (اندر) {

        cl = ngx_alloc_chain_link(pool);
        اگر (cl == تہی) {
            واپس NGX_ERROR;
        }

#اگر (NGX_SENDFILE_حد_بندی)

        buf = اندر->buf;

        اگر (buf->اندر_file
            && buf->file_pos < NGX_SENDFILE_حد_بندی
            && buf->file_آخری > NGX_SENDFILE_حد_بندی)
        {
            /* split a file buf on two bufs by the sendfile limit */

            b = ngx_calloc_buf(pool);
            اگر (b == تہی) {
                واپس NGX_ERROR;
            }

            ngx_memcpy(b, buf, حجم(ngx_buf_t));

            اگر (ngx_buf_اندر_حافظہ(buf)) {
                buf->pos += (ssize_t) (NGX_SENDFILE_حد_بندی - buf->file_pos);
                b->آخری = buf->pos;
            }

            buf->file_pos = NGX_SENDFILE_حد_بندی;
            b->file_آخری = NGX_SENDFILE_حد_بندی;

            cl->buf = b;

        } ورنہ {
            cl->buf = buf;
            اندر = اندر->اگلا;
        }

#ورنہ
        cl->buf = اندر->buf;
        اندر = اندر->اگلا;

#اگر_ختم

        cl->اگلا = تہی;
        *ll = cl;
        ll = &cl->اگلا;
    }

    واپس NGX_OK;
}


ساکن ngx_int_t
ngx_output_chain_align_file_buf(ngx_output_chain_ctx_t *ctx, off_t bsize)
{
    سائز_t      سائز;
    ngx_buf_t  *اندر;

    اندر = ctx->اندر->buf;

    اگر (اندر->file == تہی || !اندر->file->directio) {
        واپس NGX_DECLINED;
    }

    ctx->directio = 1;

    سائز = (سائز_t) (اندر->file_pos - (اندر->file_pos & ~(ctx->alignment - 1)));

    اگر (سائز == 0) {

        اگر (bsize >= (off_t) ctx->bufs.سائز) {
            واپس NGX_DECLINED;
        }

        سائز = (سائز_t) bsize;

    } ورنہ {
        سائز = (سائز_t) ctx->alignment - سائز;

        اگر ((off_t) سائز > bsize) {
            سائز = (سائز_t) bsize;
        }
    }

    ctx->buf = ngx_create_عارضی_buf(ctx->pool, سائز);
    اگر (ctx->buf == تہی) {
        واپس NGX_ERROR;
    }

    /*
     * we do not set ctx->buf->tag, because we do not want
     * to reuse the buf via ctx->free list
     */

#اگر (NGX_HAVE_ALIGNED_DIRECTIO)
    ctx->unaligned = 1;
#اگر_ختم

    واپس NGX_OK;
}


ساکن ngx_int_t
ngx_output_chain_get_buf(ngx_output_chain_ctx_t *ctx, off_t bsize)
{
    سائز_t       سائز;
    ngx_buf_t   *b, *اندر;
    ngx_uint_t   recycled;

    اندر = ctx->اندر->buf;
    سائز = ctx->bufs.سائز;
    recycled = 1;

    اگر (اندر->آخری_اندر_chain) {

        اگر (bsize < (off_t) سائز) {

            /*
             * allocate a small temp buf for a small last buf
             * or its small last part
             */

            سائز = (سائز_t) bsize;
            recycled = 0;

        } ورنہ اگر (!ctx->directio
                   && ctx->bufs.عدد == 1
                   && (bsize < (off_t) (سائز + سائز / 4)))
        {
            /*
             * allocate a temp buf that equals to a last buf,
             * if there is no directio, the last buf size is lesser
             * than 1.25 of bufs.size and the temp buf is single
             */

            سائز = (سائز_t) bsize;
            recycled = 0;
        }
    }

    b = ngx_calloc_buf(ctx->pool);
    اگر (b == تہی) {
        واپس NGX_ERROR;
    }

    اگر (ctx->directio) {

        /*
         * allocate block aligned to a disk sector size to enable
         * userland buffer direct usage conjunctly with directio
         */

        b->آغاز = ngx_pmemalign(ctx->pool, سائز, (سائز_t) ctx->alignment);
        اگر (b->آغاز == تہی) {
            واپس NGX_ERROR;
        }

    } ورنہ {
        b->آغاز = ngx_palloc(ctx->pool, سائز);
        اگر (b->آغاز == تہی) {
            واپس NGX_ERROR;
        }
    }

    b->pos = b->آغاز;
    b->آخری = b->آغاز;
    b->انتہا = b->آخری + سائز;
    b->temporary = 1;
    b->لیبل = ctx->لیبل;
    b->recycled = recycled;

    ctx->buf = b;
    ctx->allocated++;

    واپس NGX_OK;
}


ساکن ngx_int_t
ngx_output_chain_copy_buf(ngx_output_chain_ctx_t *ctx)
{
    off_t        سائز;
    ssize_t      n;
    ngx_buf_t   *src, *dst;
    ngx_uint_t   sendfile;

    src = ctx->اندر->buf;
    dst = ctx->buf;

    سائز = ngx_buf_سائز(src);
    سائز = ngx_min(سائز, dst->انتہا - dst->pos);

    sendfile = ctx->sendfile && !ctx->directio;

#اگر (NGX_SENDFILE_حد_بندی)

    اگر (src->اندر_file && src->file_pos >= NGX_SENDFILE_حد_بندی) {
        sendfile = 0;
    }

#اگر_ختم

    اگر (ngx_buf_اندر_حافظہ(src)) {
        ngx_memcpy(dst->pos, src->pos, (سائز_t) سائز);
        src->pos += (سائز_t) سائز;
        dst->آخری += (سائز_t) سائز;

        اگر (src->اندر_file) {

            اگر (sendfile) {
                dst->اندر_file = 1;
                dst->file = src->file;
                dst->file_pos = src->file_pos;
                dst->file_آخری = src->file_pos + سائز;

            } ورنہ {
                dst->اندر_file = 0;
            }

            src->file_pos += سائز;

        } ورنہ {
            dst->اندر_file = 0;
        }

        اگر (src->pos == src->آخری) {
            dst->flush = src->flush;
            dst->آخری_buf = src->آخری_buf;
            dst->آخری_اندر_chain = src->آخری_اندر_chain;

        } ورنہ {
            dst->flush = 0;
            dst->آخری_buf = 0;
            dst->آخری_اندر_chain = 0;
        }

    } ورنہ {

#اگر (NGX_HAVE_ALIGNED_DIRECTIO)

        اگر (ctx->unaligned) {
            اگر (ngx_directio_off(src->file->fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, ctx->pool->لوگارتھم, ngx_errno,
                              ngx_directio_off_n " \"%s\" failed",
                              src->file->name.ڈیٹا);
            }
        }

#اگر_ختم

#اگر (NGX_HAVE_FILE_AIO)
        اگر (ctx->aio_handler) {
            n = ngx_file_aio_read(src->file, dst->pos, (سائز_t) سائز,
                                  src->file_pos, ctx->pool);
            اگر (n == NGX_AGAIN) {
                ctx->aio_handler(ctx, src->file);
                واپس NGX_AGAIN;
            }

        } ورنہ
#اگر_ختم
#اگر (NGX_دھاگے)
        اگر (ctx->thread_handler) {
            src->file->thread_task = ctx->thread_task;
            src->file->thread_handler = ctx->thread_handler;
            src->file->thread_ctx = ctx->filter_ctx;

            n = ngx_thread_read(src->file, dst->pos, (سائز_t) سائز,
                                src->file_pos, ctx->pool);
            اگر (n == NGX_AGAIN) {
                ctx->thread_task = src->file->thread_task;
                واپس NGX_AGAIN;
            }

        } ورنہ
#اگر_ختم
        {
            n = ngx_read_file(src->file, dst->pos, (سائز_t) سائز,
                              src->file_pos);
        }

#اگر (NGX_HAVE_ALIGNED_DIRECTIO)

        اگر (ctx->unaligned) {
            ngx_err_t  err;

            err = ngx_errno;

            اگر (ngx_directio_on(src->file->fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, ctx->pool->لوگارتھم, ngx_errno,
                              ngx_directio_on_n " \"%s\" failed",
                              src->file->name.ڈیٹا);
            }

            ngx_سیٹ_errno(err);

            ctx->unaligned = 0;
        }

#اگر_ختم

        اگر (n == NGX_ERROR) {
            واپس (ngx_int_t) n;
        }

        اگر (n != سائز) {
            ngx_log_error(NGX_LOG_ALERT, ctx->pool->لوگارتھم, 0,
                          ngx_read_file_n " read only %z of %O from \"%s\"",
                          n, سائز, src->file->name.ڈیٹا);
            واپس NGX_ERROR;
        }

        dst->آخری += n;

        اگر (sendfile) {
            dst->اندر_file = 1;
            dst->file = src->file;
            dst->file_pos = src->file_pos;
            dst->file_آخری = src->file_pos + n;

        } ورنہ {
            dst->اندر_file = 0;
        }

        src->file_pos += n;

        اگر (src->file_pos == src->file_آخری) {
            dst->flush = src->flush;
            dst->آخری_buf = src->آخری_buf;
            dst->آخری_اندر_chain = src->آخری_اندر_chain;

        } ورنہ {
            dst->flush = 0;
            dst->آخری_buf = 0;
            dst->آخری_اندر_chain = 0;
        }
    }

    واپس NGX_OK;
}


ngx_int_t
ngx_chain_writer(خالی *ڈیٹا, ngx_chain_t *اندر)
{
    ngx_chain_writer_ctx_t *ctx = ڈیٹا;

    off_t              سائز;
    ngx_chain_t       *cl, *ln, *chain;
    ngx_connection_t  *c;

    c = ctx->connection;

    برائے (سائز = 0; اندر; اندر = اندر->اگلا) {

        اگر (ngx_buf_سائز(اندر->buf) == 0 && !ngx_buf_special(اندر->buf)) {

            ngx_log_error(NGX_LOG_ALERT, ctx->pool->لوگارتھم, 0,
                          "zero size buf in chain writer "
                          "t:%d r:%d f:%d %p %p-%p %p %O-%O",
                          اندر->buf->temporary,
                          اندر->buf->recycled,
                          اندر->buf->اندر_file,
                          اندر->buf->آغاز,
                          اندر->buf->pos,
                          اندر->buf->آخری,
                          اندر->buf->file,
                          اندر->buf->file_pos,
                          اندر->buf->file_آخری);

            ngx_debug_نقطہ();

            جاری;
        }

        اگر (ngx_buf_سائز(اندر->buf) < 0) {

            ngx_log_error(NGX_LOG_ALERT, ctx->pool->لوگارتھم, 0,
                          "negative size buf in chain writer "
                          "t:%d r:%d f:%d %p %p-%p %p %O-%O",
                          اندر->buf->temporary,
                          اندر->buf->recycled,
                          اندر->buf->اندر_file,
                          اندر->buf->آغاز,
                          اندر->buf->pos,
                          اندر->buf->آخری,
                          اندر->buf->file,
                          اندر->buf->file_pos,
                          اندر->buf->file_آخری);

            ngx_debug_نقطہ();

            واپس NGX_ERROR;
        }

        سائز += ngx_buf_سائز(اندر->buf);

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, c->لوگارتھم, 0,
                       "chain writer buf fl:%d s:%uO",
                       اندر->buf->flush, ngx_buf_سائز(اندر->buf));

        cl = ngx_alloc_chain_link(ctx->pool);
        اگر (cl == تہی) {
            واپس NGX_ERROR;
        }

        cl->buf = اندر->buf;
        cl->اگلا = تہی;
        *ctx->آخری = cl;
        ctx->آخری = &cl->اگلا;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->لوگارتھم, 0,
                   "chain writer in: %p", ctx->برآمد);

    برائے (cl = ctx->برآمد; cl; cl = cl->اگلا) {

        اگر (ngx_buf_سائز(cl->buf) == 0 && !ngx_buf_special(cl->buf)) {

            ngx_log_error(NGX_LOG_ALERT, ctx->pool->لوگارتھم, 0,
                          "zero size buf in chain writer "
                          "t:%d r:%d f:%d %p %p-%p %p %O-%O",
                          cl->buf->temporary,
                          cl->buf->recycled,
                          cl->buf->اندر_file,
                          cl->buf->آغاز,
                          cl->buf->pos,
                          cl->buf->آخری,
                          cl->buf->file,
                          cl->buf->file_pos,
                          cl->buf->file_آخری);

            ngx_debug_نقطہ();

            جاری;
        }

        اگر (ngx_buf_سائز(cl->buf) < 0) {

            ngx_log_error(NGX_LOG_ALERT, ctx->pool->لوگارتھم, 0,
                          "negative size buf in chain writer "
                          "t:%d r:%d f:%d %p %p-%p %p %O-%O",
                          cl->buf->temporary,
                          cl->buf->recycled,
                          cl->buf->اندر_file,
                          cl->buf->آغاز,
                          cl->buf->pos,
                          cl->buf->آخری,
                          cl->buf->file,
                          cl->buf->file_pos,
                          cl->buf->file_آخری);

            ngx_debug_نقطہ();

            واپس NGX_ERROR;
        }

        سائز += ngx_buf_سائز(cl->buf);
    }

    اگر (سائز == 0 && !c->buffered) {
        واپس NGX_OK;
    }

    chain = c->send_chain(c, ctx->برآمد, ctx->حد_بندی);

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->لوگارتھم, 0,
                   "chain writer out: %p", chain);

    اگر (chain == NGX_CHAIN_ERROR) {
        واپس NGX_ERROR;
    }

    اگر (chain && c->اكتب->ready) {
        ngx_post_event(c->اكتب, &ngx_posted_اگلا_events);
    }

    برائے (cl = ctx->برآمد; cl && cl != chain; /* void */) {
        ln = cl;
        cl = cl->اگلا;
        ngx_free_chain(ctx->pool, ln);
    }

    ctx->برآمد = chain;

    اگر (ctx->برآمد == تہی) {
        ctx->آخری = &ctx->برآمد;

        اگر (!c->buffered) {
            واپس NGX_OK;
        }
    }

    واپس NGX_AGAIN;
}
