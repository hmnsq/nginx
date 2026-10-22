
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#समावेश <ngx_config.h>
#समावेश <ngx_core.h>
#समावेश <ngx_event.h>


#अगर 0
#घोषणा NGX_SENDFILE_LIMIT  4096
#पूर्ण_अगर

/*
 * When DIRECTIO is enabled FreeBSD, Solaris, and MacOSX read directly
 * to an application memory from a device if parameters are aligned
 * to device sector boundary (512 bytes).  They fallback to usual read
 * operation if the parameters are not aligned.
 * Linux allows DIRECTIO only if the parameters are aligned to a filesystem
 * sector boundary, otherwise it returns EINVAL.  The sector size is
 * usually 512 bytes, however, on XFS it may be 4096 bytes.
 */

#घोषणा NGX_NONE            1


अटल ngx_inline ngx_int_t
    ngx_output_chain_as_is(ngx_output_chain_ctx_t *ctx, ngx_buf_t *buf);
अटल ngx_int_t ngx_output_chain_add_copy(ngx_pool_t *pool,
    ngx_chain_t **chain, ngx_chain_t *in);
अटल ngx_int_t ngx_output_chain_align_file_buf(ngx_output_chain_ctx_t *ctx,
    off_t bsize);
अटल ngx_int_t ngx_output_chain_get_buf(ngx_output_chain_ctx_t *ctx,
    off_t bsize);
अटल ngx_int_t ngx_output_chain_copy_buf(ngx_output_chain_ctx_t *ctx);


ngx_int_t
ngx_output_chain(ngx_output_chain_ctx_t *ctx, ngx_chain_t *in)
{
    off_t         bsize;
    ngx_int_t     rc, last;
    ngx_chain_t  *cl, *out, **last_out;

    अगर (ctx->in == शून्य && ctx->busy == शून्य
#अगर (NGX_HAVE_FILE_AIO || NGX_THREADS)
        && !ctx->aio
#पूर्ण_अगर
       )
    {
        /*
         * the short path for the case when the ctx->in and ctx->busy chains
         * are empty, the incoming chain is empty too or has the single buf
         * that does not require the copy
         */

        अगर (in == शून्य) {
            वापस ctx->output_filter(ctx->filter_ctx, in);
        }

        अगर (in->अग्र == शून्य
#अगर (NGX_SENDFILE_LIMIT)
            && !(in->buf->in_file && in->buf->file_last > NGX_SENDFILE_LIMIT)
#पूर्ण_अगर
            && ngx_output_chain_as_is(ctx, in->buf))
        {
            वापस ctx->output_filter(ctx->filter_ctx, in);
        }
    }

    /* add the incoming buf to the chain ctx->in */

    अगर (in) {
        अगर (ngx_output_chain_add_copy(ctx->pool, &ctx->in, in) == NGX_ERROR) {
            वापस NGX_ERROR;
        }
    }

    out = शून्य;
    last_out = &out;
    last = NGX_NONE;

    क्रम ( ;; ) {

#अगर (NGX_HAVE_FILE_AIO || NGX_THREADS)
        अगर (ctx->aio) {
            वापस NGX_AGAIN;
        }
#पूर्ण_अगर

        जबतक (ctx->in) {

            /*
             * cycle while there are the ctx->in bufs
             * and there are the free output bufs to copy in
             */

            bsize = ngx_buf_आकार(ctx->in->buf);

            अगर (bsize == 0 && !ngx_buf_special(ctx->in->buf)) {

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
                ctx->in = cl->अग्र;

                ngx_free_chain(ctx->pool, cl);

                जारी;
            }

            अगर (bsize < 0) {

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

                वापस NGX_ERROR;
            }

            अगर (ngx_output_chain_as_is(ctx, ctx->in->buf)) {

                /* move the chain link to the output chain */

                cl = ctx->in;
                ctx->in = cl->अग्र;

                *last_out = cl;
                last_out = &cl->अग्र;
                cl->अग्र = शून्य;

                जारी;
            }

            अगर (ctx->buf == शून्य) {

                rc = ngx_output_chain_align_file_buf(ctx, bsize);

                अगर (rc == NGX_ERROR) {
                    वापस NGX_ERROR;
                }

                अगर (rc != NGX_OK) {

                    अगर (ctx->मुक्त) {

                        /* get the free buf */

                        cl = ctx->मुक्त;
                        ctx->buf = cl->buf;
                        ctx->मुक्त = cl->अग्र;

                        ngx_free_chain(ctx->pool, cl);

                    } अन्यथा अगर (out || ctx->allocated == ctx->bufs.अंक) {

                        अवरोध;

                    } अन्यथा अगर (ngx_output_chain_get_buf(ctx, bsize) != NGX_OK) {
                        वापस NGX_ERROR;
                    }
                }
            }

            rc = ngx_output_chain_copy_buf(ctx);

            अगर (rc == NGX_ERROR) {
                वापस rc;
            }

            अगर (rc == NGX_AGAIN) {
                अगर (out) {
                    अवरोध;
                }

                वापस rc;
            }

            /* delete the completed buf from the ctx->in chain */

            अगर (ngx_buf_आकार(ctx->in->buf) == 0) {
                cl = ctx->in;
                ctx->in = cl->अग्र;

                ngx_free_chain(ctx->pool, cl);
            }

            cl = ngx_alloc_chain_link(ctx->pool);
            अगर (cl == शून्य) {
                वापस NGX_ERROR;
            }

            cl->buf = ctx->buf;
            cl->अग्र = शून्य;
            *last_out = cl;
            last_out = &cl->अग्र;
            ctx->buf = शून्य;
        }

        अगर (out == शून्य && last != NGX_NONE) {

            अगर (ctx->in) {
                वापस NGX_AGAIN;
            }

            वापस last;
        }

        last = ctx->output_filter(ctx->filter_ctx, out);

        अगर (last == NGX_ERROR || last == NGX_DONE) {
            वापस last;
        }

        ngx_chain_update_chains(ctx->pool, &ctx->मुक्त, &ctx->busy, &out,
                                ctx->tag);
        last_out = &out;
    }
}


अटल ngx_inline ngx_int_t
ngx_output_chain_as_is(ngx_output_chain_ctx_t *ctx, ngx_buf_t *buf)
{
    ngx_uint_t  sendfile;

    अगर (ngx_buf_special(buf)) {
        वापस 1;
    }

#अगर (NGX_THREADS)
    अगर (buf->in_file) {
        buf->file->thread_handler = ctx->thread_handler;
        buf->file->thread_ctx = ctx->filter_ctx;
    }
#पूर्ण_अगर

    sendfile = ctx->sendfile;

#अगर (NGX_SENDFILE_LIMIT)

    अगर (buf->in_file && buf->file_pos >= NGX_SENDFILE_LIMIT) {
        sendfile = 0;
    }

#पूर्ण_अगर

#अगर !(NGX_HAVE_SENDFILE_NODISKIO)

    /*
     * With DIRECTIO, disable sendfile() unless sendfile(SF_NOCACHE)
     * is available.
     */

    अगर (buf->in_file && buf->file->directio) {
        sendfile = 0;
    }

#पूर्ण_अगर

    अगर (!sendfile) {

        अगर (!ngx_buf_in_memory(buf)) {
            वापस 0;
        }

        buf->in_file = 0;
    }

    अगर (ctx->need_in_memory && !ngx_buf_in_memory(buf)) {
        वापस 0;
    }

    अगर (ctx->need_in_अस्थायी && (buf->memory || buf->mmap)) {
        वापस 0;
    }

    वापस 1;
}


अटल ngx_int_t
ngx_output_chain_add_copy(ngx_pool_t *pool, ngx_chain_t **chain,
    ngx_chain_t *in)
{
    ngx_chain_t  *cl, **ll;
#अगर (NGX_SENDFILE_LIMIT)
    ngx_buf_t    *b, *buf;
#पूर्ण_अगर

    ll = chain;

    क्रम (cl = *chain; cl; cl = cl->अग्र) {
        ll = &cl->अग्र;
    }

    जबतक (in) {

        cl = ngx_alloc_chain_link(pool);
        अगर (cl == शून्य) {
            वापस NGX_ERROR;
        }

#अगर (NGX_SENDFILE_LIMIT)

        buf = in->buf;

        अगर (buf->in_file
            && buf->file_pos < NGX_SENDFILE_LIMIT
            && buf->file_last > NGX_SENDFILE_LIMIT)
        {
            /* split a file buf on two bufs by the sendfile limit */

            b = ngx_calloc_buf(pool);
            अगर (b == शून्य) {
                वापस NGX_ERROR;
            }

            ngx_memcpy(b, buf, माप(ngx_buf_t));

            अगर (ngx_buf_in_memory(buf)) {
                buf->pos += (ssize_t) (NGX_SENDFILE_LIMIT - buf->file_pos);
                b->last = buf->pos;
            }

            buf->file_pos = NGX_SENDFILE_LIMIT;
            b->file_last = NGX_SENDFILE_LIMIT;

            cl->buf = b;

        } अन्यथा {
            cl->buf = buf;
            in = in->अग्र;
        }

#अन्यथा
        cl->buf = in->buf;
        in = in->अग्र;

#पूर्ण_अगर

        cl->अग्र = शून्य;
        *ll = cl;
        ll = &cl->अग्र;
    }

    वापस NGX_OK;
}


अटल ngx_int_t
ngx_output_chain_align_file_buf(ngx_output_chain_ctx_t *ctx, off_t bsize)
{
    माप_प्रकार      आकार;
    ngx_buf_t  *in;

    in = ctx->in->buf;

    अगर (in->file == शून्य || !in->file->directio) {
        वापस NGX_DECLINED;
    }

    ctx->directio = 1;

    आकार = (माप_प्रकार) (in->file_pos - (in->file_pos & ~(ctx->alignment - 1)));

    अगर (आकार == 0) {

        अगर (bsize >= (off_t) ctx->bufs.आकार) {
            वापस NGX_DECLINED;
        }

        आकार = (माप_प्रकार) bsize;

    } अन्यथा {
        आकार = (माप_प्रकार) ctx->alignment - आकार;

        अगर ((off_t) आकार > bsize) {
            आकार = (माप_प्रकार) bsize;
        }
    }

    ctx->buf = ngx_create_अस्थायी_buf(ctx->pool, आकार);
    अगर (ctx->buf == शून्य) {
        वापस NGX_ERROR;
    }

    /*
     * we do not set ctx->buf->tag, because we do not want
     * to reuse the buf via ctx->free list
     */

#अगर (NGX_HAVE_ALIGNED_DIRECTIO)
    ctx->unaligned = 1;
#पूर्ण_अगर

    वापस NGX_OK;
}


अटल ngx_int_t
ngx_output_chain_get_buf(ngx_output_chain_ctx_t *ctx, off_t bsize)
{
    माप_प्रकार       आकार;
    ngx_buf_t   *b, *in;
    ngx_uint_t   recycled;

    in = ctx->in->buf;
    आकार = ctx->bufs.आकार;
    recycled = 1;

    अगर (in->last_in_chain) {

        अगर (bsize < (off_t) आकार) {

            /*
             * allocate a small temp buf for a small last buf
             * or its small last part
             */

            आकार = (माप_प्रकार) bsize;
            recycled = 0;

        } अन्यथा अगर (!ctx->directio
                   && ctx->bufs.अंक == 1
                   && (bsize < (off_t) (आकार + आकार / 4)))
        {
            /*
             * allocate a temp buf that equals to a last buf,
             * if there is no directio, the last buf size is lesser
             * than 1.25 of bufs.size and the temp buf is single
             */

            आकार = (माप_प्रकार) bsize;
            recycled = 0;
        }
    }

    b = ngx_calloc_buf(ctx->pool);
    अगर (b == शून्य) {
        वापस NGX_ERROR;
    }

    अगर (ctx->directio) {

        /*
         * allocate block aligned to a disk sector size to enable
         * userland buffer direct usage conjunctly with directio
         */

        b->start = ngx_pmemalign(ctx->pool, आकार, (माप_प्रकार) ctx->alignment);
        अगर (b->start == शून्य) {
            वापस NGX_ERROR;
        }

    } अन्यथा {
        b->start = ngx_palloc(ctx->pool, आकार);
        अगर (b->start == शून्य) {
            वापस NGX_ERROR;
        }
    }

    b->pos = b->start;
    b->last = b->start;
    b->end = b->last + आकार;
    b->temporary = 1;
    b->tag = ctx->tag;
    b->recycled = recycled;

    ctx->buf = b;
    ctx->allocated++;

    वापस NGX_OK;
}


अटल ngx_int_t
ngx_output_chain_copy_buf(ngx_output_chain_ctx_t *ctx)
{
    off_t        आकार;
    ssize_t      n;
    ngx_buf_t   *src, *dst;
    ngx_uint_t   sendfile;

    src = ctx->in->buf;
    dst = ctx->buf;

    आकार = ngx_buf_आकार(src);
    आकार = ngx_min(आकार, dst->end - dst->pos);

    sendfile = ctx->sendfile && !ctx->directio;

#अगर (NGX_SENDFILE_LIMIT)

    अगर (src->in_file && src->file_pos >= NGX_SENDFILE_LIMIT) {
        sendfile = 0;
    }

#पूर्ण_अगर

    अगर (ngx_buf_in_memory(src)) {
        ngx_memcpy(dst->pos, src->pos, (माप_प्रकार) आकार);
        src->pos += (माप_प्रकार) आकार;
        dst->last += (माप_प्रकार) आकार;

        अगर (src->in_file) {

            अगर (sendfile) {
                dst->in_file = 1;
                dst->file = src->file;
                dst->file_pos = src->file_pos;
                dst->file_last = src->file_pos + आकार;

            } अन्यथा {
                dst->in_file = 0;
            }

            src->file_pos += आकार;

        } अन्यथा {
            dst->in_file = 0;
        }

        अगर (src->pos == src->last) {
            dst->flush = src->flush;
            dst->last_buf = src->last_buf;
            dst->last_in_chain = src->last_in_chain;

        } अन्यथा {
            dst->flush = 0;
            dst->last_buf = 0;
            dst->last_in_chain = 0;
        }

    } अन्यथा {

#अगर (NGX_HAVE_ALIGNED_DIRECTIO)

        अगर (ctx->unaligned) {
            अगर (ngx_directio_off(src->file->fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, ctx->pool->log, ngx_errno,
                              ngx_directio_off_n " \"%s\" failed",
                              src->file->name.तथ्य);
            }
        }

#पूर्ण_अगर

#अगर (NGX_HAVE_FILE_AIO)
        अगर (ctx->aio_handler) {
            n = ngx_file_aio_read(src->file, dst->pos, (माप_प्रकार) आकार,
                                  src->file_pos, ctx->pool);
            अगर (n == NGX_AGAIN) {
                ctx->aio_handler(ctx, src->file);
                वापस NGX_AGAIN;
            }

        } अन्यथा
#पूर्ण_अगर
#अगर (NGX_THREADS)
        अगर (ctx->thread_handler) {
            src->file->thread_task = ctx->thread_task;
            src->file->thread_handler = ctx->thread_handler;
            src->file->thread_ctx = ctx->filter_ctx;

            n = ngx_thread_read(src->file, dst->pos, (माप_प्रकार) आकार,
                                src->file_pos, ctx->pool);
            अगर (n == NGX_AGAIN) {
                ctx->thread_task = src->file->thread_task;
                वापस NGX_AGAIN;
            }

        } अन्यथा
#पूर्ण_अगर
        {
            n = ngx_read_file(src->file, dst->pos, (माप_प्रकार) आकार,
                              src->file_pos);
        }

#अगर (NGX_HAVE_ALIGNED_DIRECTIO)

        अगर (ctx->unaligned) {
            ngx_err_t  err;

            err = ngx_errno;

            अगर (ngx_directio_on(src->file->fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, ctx->pool->log, ngx_errno,
                              ngx_directio_on_n " \"%s\" failed",
                              src->file->name.तथ्य);
            }

            ngx_set_errno(err);

            ctx->unaligned = 0;
        }

#पूर्ण_अगर

        अगर (n == NGX_ERROR) {
            वापस (ngx_int_t) n;
        }

        अगर (n != आकार) {
            ngx_log_error(NGX_LOG_ALERT, ctx->pool->log, 0,
                          ngx_read_file_n " read only %z of %O from \"%s\"",
                          n, आकार, src->file->name.तथ्य);
            वापस NGX_ERROR;
        }

        dst->last += n;

        अगर (sendfile) {
            dst->in_file = 1;
            dst->file = src->file;
            dst->file_pos = src->file_pos;
            dst->file_last = src->file_pos + n;

        } अन्यथा {
            dst->in_file = 0;
        }

        src->file_pos += n;

        अगर (src->file_pos == src->file_last) {
            dst->flush = src->flush;
            dst->last_buf = src->last_buf;
            dst->last_in_chain = src->last_in_chain;

        } अन्यथा {
            dst->flush = 0;
            dst->last_buf = 0;
            dst->last_in_chain = 0;
        }
    }

    वापस NGX_OK;
}


ngx_int_t
ngx_chain_writer(व्योम *तथ्य, ngx_chain_t *in)
{
    ngx_chain_writer_ctx_t *ctx = तथ्य;

    off_t              आकार;
    ngx_chain_t       *cl, *ln, *chain;
    ngx_connection_t  *c;

    c = ctx->connection;

    क्रम (आकार = 0; in; in = in->अग्र) {

        अगर (ngx_buf_आकार(in->buf) == 0 && !ngx_buf_special(in->buf)) {

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

            जारी;
        }

        अगर (ngx_buf_आकार(in->buf) < 0) {

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

            वापस NGX_ERROR;
        }

        आकार += ngx_buf_आकार(in->buf);

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "chain writer buf fl:%d s:%uO",
                       in->buf->flush, ngx_buf_आकार(in->buf));

        cl = ngx_alloc_chain_link(ctx->pool);
        अगर (cl == शून्य) {
            वापस NGX_ERROR;
        }

        cl->buf = in->buf;
        cl->अग्र = शून्य;
        *ctx->last = cl;
        ctx->last = &cl->अग्र;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                   "chain writer in: %p", ctx->out);

    क्रम (cl = ctx->out; cl; cl = cl->अग्र) {

        अगर (ngx_buf_आकार(cl->buf) == 0 && !ngx_buf_special(cl->buf)) {

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

            जारी;
        }

        अगर (ngx_buf_आकार(cl->buf) < 0) {

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

            वापस NGX_ERROR;
        }

        आकार += ngx_buf_आकार(cl->buf);
    }

    अगर (आकार == 0 && !c->buffered) {
        वापस NGX_OK;
    }

    chain = c->send_chain(c, ctx->out, ctx->limit);

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                   "chain writer out: %p", chain);

    अगर (chain == NGX_CHAIN_ERROR) {
        वापस NGX_ERROR;
    }

    अगर (chain && c->ग_लिखो->ready) {
        ngx_post_event(c->ग_लिखो, &ngx_posted_अग्र_events);
    }

    क्रम (cl = ctx->out; cl && cl != chain; /* void */) {
        ln = cl;
        cl = cl->अग्र;
        ngx_free_chain(ctx->pool, ln);
    }

    ctx->out = chain;

    अगर (ctx->out == शून्य) {
        ctx->last = &ctx->out;

        अगर (!c->buffered) {
            वापस NGX_OK;
        }
    }

    वापस NGX_AGAIN;
}
