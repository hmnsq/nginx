
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#অন্তর্ভাব <ngx_config.h>
#অন্তর্ভাব <ngx_core.h>
#অন্তর্ভাব <ngx_event.h>


#যদি 0
#define NGX_SENDFILE_LIMIT  4096
#endif

/*
 * When DIRECTIO is enabled FreeBSD, Solaris, and MacOSX read directly
 * to an application memory from a device if parameters are aligned
 * to device sector boundary (512 bytes).  They fallback to usual read
 * operation if the parameters are not aligned.
 * Linux allows DIRECTIO only if the parameters are aligned to a filesystem
 * sector boundary, otherwise it returns EINVAL.  The sector size is
 * usually 512 bytes, however, on XFS it may be 4096 bytes.
 */

#define NGX_NONE            1


স্থির ngx_inline ngx_int_t
    ngx_output_chain_as_is(ngx_output_chain_ctx_t *ctx, ngx_buf_t *buf);
স্থির ngx_int_t ngx_output_chain_add_copy(ngx_pool_t *pool,
    ngx_chain_t **chain, ngx_chain_t *in);
স্থির ngx_int_t ngx_output_chain_align_file_buf(ngx_output_chain_ctx_t *ctx,
    off_t bsize);
স্থির ngx_int_t ngx_output_chain_get_buf(ngx_output_chain_ctx_t *ctx,
    off_t bsize);
স্থির ngx_int_t ngx_output_chain_copy_buf(ngx_output_chain_ctx_t *ctx);


ngx_int_t
ngx_output_chain(ngx_output_chain_ctx_t *ctx, ngx_chain_t *in)
{
    off_t         bsize;
    ngx_int_t     rc, last;
    ngx_chain_t  *cl, *out, **last_out;

    যদি (ctx->in == ফাঁকা && ctx->busy == ফাঁকা
#যদি (NGX_HAVE_FILE_AIO || NGX_THREADS)
        && !ctx->aio
#endif
       )
    {
        /*
         * the short path for the case when the ctx->in and ctx->busy chains
         * are empty, the incoming chain is empty too or has the single buf
         * that does not require the copy
         */

        যদি (in == ফাঁকা) {
            ফেরত ctx->output_filter(ctx->filter_ctx, in);
        }

        যদি (in->পরবর্তী == ফাঁকা
#যদি (NGX_SENDFILE_LIMIT)
            && !(in->buf->in_file && in->buf->file_last > NGX_SENDFILE_LIMIT)
#endif
            && ngx_output_chain_as_is(ctx, in->buf))
        {
            ফেরত ctx->output_filter(ctx->filter_ctx, in);
        }
    }

    /* add the incoming buf to the chain ctx->in */

    যদি (in) {
        যদি (ngx_output_chain_add_copy(ctx->pool, &ctx->in, in) == NGX_ERROR) {
            ফেরত NGX_ERROR;
        }
    }

    out = ফাঁকা;
    last_out = &out;
    last = NGX_NONE;

    জন্য ( ;; ) {

#যদি (NGX_HAVE_FILE_AIO || NGX_THREADS)
        যদি (ctx->aio) {
            ফেরত NGX_AGAIN;
        }
#endif

        যখন (ctx->in) {

            /*
             * cycle while there are the ctx->in bufs
             * and there are the free output bufs to copy in
             */

            bsize = ngx_buf_আকার(ctx->in->buf);

            যদি (bsize == 0 && !ngx_buf_special(ctx->in->buf)) {

                ngx_log_error(NGX_LOG_ALERT, ctx->pool->log, 0,
                              "zero size buf in output "
                              "t:%d r:%d f:%d %p %p-%p %p %O-%O",
                              ctx->in->buf->temporary,
                              ctx->in->buf->recycled,
                              ctx->in->buf->in_file,
                              ctx->in->buf->start,
                              ctx->in->buf->pos,
                              ctx->in->buf->last,
                              ctx->in->buf->file,
                              ctx->in->buf->file_pos,
                              ctx->in->buf->file_last);

                ngx_debug_point();

                cl = ctx->in;
                ctx->in = cl->পরবর্তী;

                ngx_free_chain(ctx->pool, cl);

                চালু;
            }

            যদি (bsize < 0) {

                ngx_log_error(NGX_LOG_ALERT, ctx->pool->log, 0,
                              "negative size buf in output "
                              "t:%d r:%d f:%d %p %p-%p %p %O-%O",
                              ctx->in->buf->temporary,
                              ctx->in->buf->recycled,
                              ctx->in->buf->in_file,
                              ctx->in->buf->start,
                              ctx->in->buf->pos,
                              ctx->in->buf->last,
                              ctx->in->buf->file,
                              ctx->in->buf->file_pos,
                              ctx->in->buf->file_last);

                ngx_debug_point();

                ফেরত NGX_ERROR;
            }

            যদি (ngx_output_chain_as_is(ctx, ctx->in->buf)) {

                /* move the chain link to the output chain */

                cl = ctx->in;
                ctx->in = cl->পরবর্তী;

                *last_out = cl;
                last_out = &cl->পরবর্তী;
                cl->পরবর্তী = ফাঁকা;

                চালু;
            }

            যদি (ctx->buf == ফাঁকা) {

                rc = ngx_output_chain_align_file_buf(ctx, bsize);

                যদি (rc == NGX_ERROR) {
                    ফেরত NGX_ERROR;
                }

                যদি (rc != NGX_OK) {

                    যদি (ctx->মুক্ত) {

                        /* get the free buf */

                        cl = ctx->মুক্ত;
                        ctx->buf = cl->buf;
                        ctx->মুক্ত = cl->পরবর্তী;

                        ngx_free_chain(ctx->pool, cl);

                    } অন্যথা যদি (out || ctx->allocated == ctx->bufs.অঙ্ক) {

                        বিরতি;

                    } অন্যথা যদি (ngx_output_chain_get_buf(ctx, bsize) != NGX_OK) {
                        ফেরত NGX_ERROR;
                    }
                }
            }

            rc = ngx_output_chain_copy_buf(ctx);

            যদি (rc == NGX_ERROR) {
                ফেরত rc;
            }

            যদি (rc == NGX_AGAIN) {
                যদি (out) {
                    বিরতি;
                }

                ফেরত rc;
            }

            /* delete the completed buf from the ctx->in chain */

            যদি (ngx_buf_আকার(ctx->in->buf) == 0) {
                cl = ctx->in;
                ctx->in = cl->পরবর্তী;

                ngx_free_chain(ctx->pool, cl);
            }

            cl = ngx_alloc_chain_link(ctx->pool);
            যদি (cl == ফাঁকা) {
                ফেরত NGX_ERROR;
            }

            cl->buf = ctx->buf;
            cl->পরবর্তী = ফাঁকা;
            *last_out = cl;
            last_out = &cl->পরবর্তী;
            ctx->buf = ফাঁকা;
        }

        যদি (out == ফাঁকা && last != NGX_NONE) {

            যদি (ctx->in) {
                ফেরত NGX_AGAIN;
            }

            ফেরত last;
        }

        last = ctx->output_filter(ctx->filter_ctx, out);

        যদি (last == NGX_ERROR || last == NGX_DONE) {
            ফেরত last;
        }

        ngx_chain_update_chains(ctx->pool, &ctx->মুক্ত, &ctx->busy, &out,
                                ctx->tag);
        last_out = &out;
    }
}


স্থির ngx_inline ngx_int_t
ngx_output_chain_as_is(ngx_output_chain_ctx_t *ctx, ngx_buf_t *buf)
{
    ngx_uint_t  sendfile;

    যদি (ngx_buf_special(buf)) {
        ফেরত 1;
    }

#যদি (NGX_THREADS)
    যদি (buf->in_file) {
        buf->file->thread_handler = ctx->thread_handler;
        buf->file->thread_ctx = ctx->filter_ctx;
    }
#endif

    sendfile = ctx->sendfile;

#যদি (NGX_SENDFILE_LIMIT)

    যদি (buf->in_file && buf->file_pos >= NGX_SENDFILE_LIMIT) {
        sendfile = 0;
    }

#endif

#যদি !(NGX_HAVE_SENDFILE_NODISKIO)

    /*
     * With DIRECTIO, disable sendfile() unless sendfile(SF_NOCACHE)
     * is available.
     */

    যদি (buf->in_file && buf->file->directio) {
        sendfile = 0;
    }

#endif

    যদি (!sendfile) {

        যদি (!ngx_buf_in_memory(buf)) {
            ফেরত 0;
        }

        buf->in_file = 0;
    }

    যদি (ctx->need_in_memory && !ngx_buf_in_memory(buf)) {
        ফেরত 0;
    }

    যদি (ctx->need_in_অস্থায়ী && (buf->memory || buf->mmap)) {
        ফেরত 0;
    }

    ফেরত 1;
}


স্থির ngx_int_t
ngx_output_chain_add_copy(ngx_pool_t *pool, ngx_chain_t **chain,
    ngx_chain_t *in)
{
    ngx_chain_t  *cl, **ll;
#যদি (NGX_SENDFILE_LIMIT)
    ngx_buf_t    *b, *buf;
#endif

    ll = chain;

    জন্য (cl = *chain; cl; cl = cl->পরবর্তী) {
        ll = &cl->পরবর্তী;
    }

    যখন (in) {

        cl = ngx_alloc_chain_link(pool);
        যদি (cl == ফাঁকা) {
            ফেরত NGX_ERROR;
        }

#যদি (NGX_SENDFILE_LIMIT)

        buf = in->buf;

        যদি (buf->in_file
            && buf->file_pos < NGX_SENDFILE_LIMIT
            && buf->file_last > NGX_SENDFILE_LIMIT)
        {
            /* split a file buf on two bufs by the sendfile limit */

            b = ngx_calloc_buf(pool);
            যদি (b == ফাঁকা) {
                ফেরত NGX_ERROR;
            }

            ngx_memcpy(b, buf, আকার(ngx_buf_t));

            যদি (ngx_buf_in_memory(buf)) {
                buf->pos += (ssize_t) (NGX_SENDFILE_LIMIT - buf->file_pos);
                b->last = buf->pos;
            }

            buf->file_pos = NGX_SENDFILE_LIMIT;
            b->file_last = NGX_SENDFILE_LIMIT;

            cl->buf = b;

        } অন্যথা {
            cl->buf = buf;
            in = in->পরবর্তী;
        }

#অন্যথা
        cl->buf = in->buf;
        in = in->পরবর্তী;

#endif

        cl->পরবর্তী = ফাঁকা;
        *ll = cl;
        ll = &cl->পরবর্তী;
    }

    ফেরত NGX_OK;
}


স্থির ngx_int_t
ngx_output_chain_align_file_buf(ngx_output_chain_ctx_t *ctx, off_t bsize)
{
    আকার_t      আকার_2;
    ngx_buf_t  *in;

    in = ctx->in->buf;

    যদি (in->file == ফাঁকা || !in->file->directio) {
        ফেরত NGX_DECLINED;
    }

    ctx->directio = 1;

    আকার_2 = (আকার_t) (in->file_pos - (in->file_pos & ~(ctx->alignment - 1)));

    যদি (আকার_2 == 0) {

        যদি (bsize >= (off_t) ctx->bufs.আকার_2) {
            ফেরত NGX_DECLINED;
        }

        আকার_2 = (আকার_t) bsize;

    } অন্যথা {
        আকার_2 = (আকার_t) ctx->alignment - আকার_2;

        যদি ((off_t) আকার_2 > bsize) {
            আকার_2 = (আকার_t) bsize;
        }
    }

    ctx->buf = ngx_create_অস্থায়ী_buf(ctx->pool, আকার_2);
    যদি (ctx->buf == ফাঁকা) {
        ফেরত NGX_ERROR;
    }

    /*
     * we do not set ctx->buf->tag, because we do not want
     * to reuse the buf via ctx->free list
     */

#যদি (NGX_HAVE_ALIGNED_DIRECTIO)
    ctx->unaligned = 1;
#endif

    ফেরত NGX_OK;
}


স্থির ngx_int_t
ngx_output_chain_get_buf(ngx_output_chain_ctx_t *ctx, off_t bsize)
{
    আকার_t       আকার_2;
    ngx_buf_t   *b, *in;
    ngx_uint_t   recycled;

    in = ctx->in->buf;
    আকার_2 = ctx->bufs.আকার_2;
    recycled = 1;

    যদি (in->last_in_chain) {

        যদি (bsize < (off_t) আকার_2) {

            /*
             * allocate a small temp buf for a small last buf
             * or its small last part
             */

            আকার_2 = (আকার_t) bsize;
            recycled = 0;

        } অন্যথা যদি (!ctx->directio
                   && ctx->bufs.অঙ্ক == 1
                   && (bsize < (off_t) (আকার_2 + আকার_2 / 4)))
        {
            /*
             * allocate a temp buf that equals to a last buf,
             * if there is no directio, the last buf size is lesser
             * than 1.25 of bufs.size and the temp buf is single
             */

            আকার_2 = (আকার_t) bsize;
            recycled = 0;
        }
    }

    b = ngx_calloc_buf(ctx->pool);
    যদি (b == ফাঁকা) {
        ফেরত NGX_ERROR;
    }

    যদি (ctx->directio) {

        /*
         * allocate block aligned to a disk sector size to enable
         * userland buffer direct usage conjunctly with directio
         */

        b->start = ngx_pmemalign(ctx->pool, আকার_2, (আকার_t) ctx->alignment);
        যদি (b->start == ফাঁকা) {
            ফেরত NGX_ERROR;
        }

    } অন্যথা {
        b->start = ngx_palloc(ctx->pool, আকার_2);
        যদি (b->start == ফাঁকা) {
            ফেরত NGX_ERROR;
        }
    }

    b->pos = b->start;
    b->last = b->start;
    b->end = b->last + আকার_2;
    b->temporary = 1;
    b->tag = ctx->tag;
    b->recycled = recycled;

    ctx->buf = b;
    ctx->allocated++;

    ফেরত NGX_OK;
}


স্থির ngx_int_t
ngx_output_chain_copy_buf(ngx_output_chain_ctx_t *ctx)
{
    off_t        আকার_2;
    ssize_t      n;
    ngx_buf_t   *src, *dst;
    ngx_uint_t   sendfile;

    src = ctx->in->buf;
    dst = ctx->buf;

    আকার_2 = ngx_buf_আকার(src);
    আকার_2 = ngx_min(আকার_2, dst->end - dst->pos);

    sendfile = ctx->sendfile && !ctx->directio;

#যদি (NGX_SENDFILE_LIMIT)

    যদি (src->in_file && src->file_pos >= NGX_SENDFILE_LIMIT) {
        sendfile = 0;
    }

#endif

    যদি (ngx_buf_in_memory(src)) {
        ngx_memcpy(dst->pos, src->pos, (আকার_t) আকার_2);
        src->pos += (আকার_t) আকার_2;
        dst->last += (আকার_t) আকার_2;

        যদি (src->in_file) {

            যদি (sendfile) {
                dst->in_file = 1;
                dst->file = src->file;
                dst->file_pos = src->file_pos;
                dst->file_last = src->file_pos + আকার_2;

            } অন্যথা {
                dst->in_file = 0;
            }

            src->file_pos += আকার_2;

        } অন্যথা {
            dst->in_file = 0;
        }

        যদি (src->pos == src->last) {
            dst->flush = src->flush;
            dst->last_buf = src->last_buf;
            dst->last_in_chain = src->last_in_chain;

        } অন্যথা {
            dst->flush = 0;
            dst->last_buf = 0;
            dst->last_in_chain = 0;
        }

    } অন্যথা {

#যদি (NGX_HAVE_ALIGNED_DIRECTIO)

        যদি (ctx->unaligned) {
            যদি (ngx_directio_off(src->file->fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, ctx->pool->log, ngx_errno,
                              ngx_directio_off_n " \"%s\" failed",
                              src->file->name.তথ্য);
            }
        }

#endif

#যদি (NGX_HAVE_FILE_AIO)
        যদি (ctx->aio_handler) {
            n = ngx_file_aio_read(src->file, dst->pos, (আকার_t) আকার_2,
                                  src->file_pos, ctx->pool);
            যদি (n == NGX_AGAIN) {
                ctx->aio_handler(ctx, src->file);
                ফেরত NGX_AGAIN;
            }

        } অন্যথা
#endif
#যদি (NGX_THREADS)
        যদি (ctx->thread_handler) {
            src->file->thread_task = ctx->thread_task;
            src->file->thread_handler = ctx->thread_handler;
            src->file->thread_ctx = ctx->filter_ctx;

            n = ngx_thread_read(src->file, dst->pos, (আকার_t) আকার_2,
                                src->file_pos, ctx->pool);
            যদি (n == NGX_AGAIN) {
                ctx->thread_task = src->file->thread_task;
                ফেরত NGX_AGAIN;
            }

        } অন্যথা
#endif
        {
            n = ngx_read_file(src->file, dst->pos, (আকার_t) আকার_2,
                              src->file_pos);
        }

#যদি (NGX_HAVE_ALIGNED_DIRECTIO)

        যদি (ctx->unaligned) {
            ngx_err_t  err;

            err = ngx_errno;

            যদি (ngx_directio_on(src->file->fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, ctx->pool->log, ngx_errno,
                              ngx_directio_on_n " \"%s\" failed",
                              src->file->name.তথ্য);
            }

            ngx_set_errno(err);

            ctx->unaligned = 0;
        }

#endif

        যদি (n == NGX_ERROR) {
            ফেরত (ngx_int_t) n;
        }

        যদি (n != আকার_2) {
            ngx_log_error(NGX_LOG_ALERT, ctx->pool->log, 0,
                          ngx_read_file_n " read only %z of %O from \"%s\"",
                          n, আকার_2, src->file->name.তথ্য);
            ফেরত NGX_ERROR;
        }

        dst->last += n;

        যদি (sendfile) {
            dst->in_file = 1;
            dst->file = src->file;
            dst->file_pos = src->file_pos;
            dst->file_last = src->file_pos + n;

        } অন্যথা {
            dst->in_file = 0;
        }

        src->file_pos += n;

        যদি (src->file_pos == src->file_last) {
            dst->flush = src->flush;
            dst->last_buf = src->last_buf;
            dst->last_in_chain = src->last_in_chain;

        } অন্যথা {
            dst->flush = 0;
            dst->last_buf = 0;
            dst->last_in_chain = 0;
        }
    }

    ফেরত NGX_OK;
}


ngx_int_t
ngx_chain_writer(শূন্য *তথ্য, ngx_chain_t *in)
{
    ngx_chain_writer_ctx_t *ctx = তথ্য;

    off_t              আকার_2;
    ngx_chain_t       *cl, *ln, *chain;
    ngx_connection_t  *c;

    c = ctx->connection;

    জন্য (আকার_2 = 0; in; in = in->পরবর্তী) {

        যদি (ngx_buf_আকার(in->buf) == 0 && !ngx_buf_special(in->buf)) {

            ngx_log_error(NGX_LOG_ALERT, ctx->pool->log, 0,
                          "zero size buf in chain writer "
                          "t:%d r:%d f:%d %p %p-%p %p %O-%O",
                          in->buf->temporary,
                          in->buf->recycled,
                          in->buf->in_file,
                          in->buf->start,
                          in->buf->pos,
                          in->buf->last,
                          in->buf->file,
                          in->buf->file_pos,
                          in->buf->file_last);

            ngx_debug_point();

            চালু;
        }

        যদি (ngx_buf_আকার(in->buf) < 0) {

            ngx_log_error(NGX_LOG_ALERT, ctx->pool->log, 0,
                          "negative size buf in chain writer "
                          "t:%d r:%d f:%d %p %p-%p %p %O-%O",
                          in->buf->temporary,
                          in->buf->recycled,
                          in->buf->in_file,
                          in->buf->start,
                          in->buf->pos,
                          in->buf->last,
                          in->buf->file,
                          in->buf->file_pos,
                          in->buf->file_last);

            ngx_debug_point();

            ফেরত NGX_ERROR;
        }

        আকার_2 += ngx_buf_আকার(in->buf);

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "chain writer buf fl:%d s:%uO",
                       in->buf->flush, ngx_buf_আকার(in->buf));

        cl = ngx_alloc_chain_link(ctx->pool);
        যদি (cl == ফাঁকা) {
            ফেরত NGX_ERROR;
        }

        cl->buf = in->buf;
        cl->পরবর্তী = ফাঁকা;
        *ctx->last = cl;
        ctx->last = &cl->পরবর্তী;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                   "chain writer in: %p", ctx->out);

    জন্য (cl = ctx->out; cl; cl = cl->পরবর্তী) {

        যদি (ngx_buf_আকার(cl->buf) == 0 && !ngx_buf_special(cl->buf)) {

            ngx_log_error(NGX_LOG_ALERT, ctx->pool->log, 0,
                          "zero size buf in chain writer "
                          "t:%d r:%d f:%d %p %p-%p %p %O-%O",
                          cl->buf->temporary,
                          cl->buf->recycled,
                          cl->buf->in_file,
                          cl->buf->start,
                          cl->buf->pos,
                          cl->buf->last,
                          cl->buf->file,
                          cl->buf->file_pos,
                          cl->buf->file_last);

            ngx_debug_point();

            চালু;
        }

        যদি (ngx_buf_আকার(cl->buf) < 0) {

            ngx_log_error(NGX_LOG_ALERT, ctx->pool->log, 0,
                          "negative size buf in chain writer "
                          "t:%d r:%d f:%d %p %p-%p %p %O-%O",
                          cl->buf->temporary,
                          cl->buf->recycled,
                          cl->buf->in_file,
                          cl->buf->start,
                          cl->buf->pos,
                          cl->buf->last,
                          cl->buf->file,
                          cl->buf->file_pos,
                          cl->buf->file_last);

            ngx_debug_point();

            ফেরত NGX_ERROR;
        }

        আকার_2 += ngx_buf_আকার(cl->buf);
    }

    যদি (আকার_2 == 0 && !c->buffered) {
        ফেরত NGX_OK;
    }

    chain = c->send_chain(c, ctx->out, ctx->limit);

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                   "chain writer out: %p", chain);

    যদি (chain == NGX_CHAIN_ERROR) {
        ফেরত NGX_ERROR;
    }

    যদি (chain && c->লিখ->ready) {
        ngx_post_event(c->লিখ, &ngx_posted_পরবর্তী_events);
    }

    জন্য (cl = ctx->out; cl && cl != chain; /* void */) {
        ln = cl;
        cl = cl->পরবর্তী;
        ngx_free_chain(ctx->pool, ln);
    }

    ctx->out = chain;

    যদি (ctx->out == ফাঁকা) {
        ctx->last = &ctx->out;

        যদি (!c->buffered) {
            ফেরত NGX_OK;
        }
    }

    ফেরত NGX_AGAIN;
}
