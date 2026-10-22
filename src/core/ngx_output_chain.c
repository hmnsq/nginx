
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>
#अन्तर्भाव <ngx_event.h>


#जर 0
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


स्थिर ngx_inline ngx_int_t
    ngx_output_chain_as_is(ngx_output_chain_ctx_t *ctx, ngx_buf_t *buf);
स्थिर ngx_int_t ngx_output_chain_add_copy(ngx_pool_t *pool,
    ngx_chain_t **chain, ngx_chain_t *in);
स्थिर ngx_int_t ngx_output_chain_align_file_buf(ngx_output_chain_ctx_t *ctx,
    off_t bsize);
स्थिर ngx_int_t ngx_output_chain_get_buf(ngx_output_chain_ctx_t *ctx,
    off_t bsize);
स्थिर ngx_int_t ngx_output_chain_copy_buf(ngx_output_chain_ctx_t *ctx);


ngx_int_t
ngx_output_chain(ngx_output_chain_ctx_t *ctx, ngx_chain_t *in)
{
    off_t         bsize;
    ngx_int_t     rc, last;
    ngx_chain_t  *cl, *out, **last_out;

    जर (ctx->in == रिक्त && ctx->busy == रिक्त
#जर (NGX_HAVE_FILE_AIO || NGX_THREADS)
        && !ctx->aio
#endif
       )
    {
        /*
         * the short path for the case when the ctx->in and ctx->busy chains
         * are empty, the incoming chain is empty too or has the single buf
         * that does not require the copy
         */

        जर (in == रिक्त) {
            परत ctx->output_filter(ctx->filter_ctx, in);
        }

        जर (in->next == रिक्त
#जर (NGX_SENDFILE_LIMIT)
            && !(in->buf->in_file && in->buf->file_last > NGX_SENDFILE_LIMIT)
#endif
            && ngx_output_chain_as_is(ctx, in->buf))
        {
            परत ctx->output_filter(ctx->filter_ctx, in);
        }
    }

    /* add the incoming buf to the chain ctx->in */

    जर (in) {
        जर (ngx_output_chain_add_copy(ctx->pool, &ctx->in, in) == NGX_ERROR) {
            परत NGX_ERROR;
        }
    }

    out = रिक्त;
    last_out = &out;
    last = NGX_NONE;

    साठी ( ;; ) {

#जर (NGX_HAVE_FILE_AIO || NGX_THREADS)
        जर (ctx->aio) {
            परत NGX_AGAIN;
        }
#endif

        पर्यंत (ctx->in) {

            /*
             * cycle while there are the ctx->in bufs
             * and there are the free output bufs to copy in
             */

            bsize = ngx_buf_size(ctx->in->buf);

            जर (bsize == 0 && !ngx_buf_special(ctx->in->buf)) {

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
                ctx->in = cl->next;

                ngx_free_chain(ctx->pool, cl);

                सुरूठेवा;
            }

            जर (bsize < 0) {

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

                परत NGX_ERROR;
            }

            जर (ngx_output_chain_as_is(ctx, ctx->in->buf)) {

                /* move the chain link to the output chain */

                cl = ctx->in;
                ctx->in = cl->next;

                *last_out = cl;
                last_out = &cl->next;
                cl->next = रिक्त;

                सुरूठेवा;
            }

            जर (ctx->buf == रिक्त) {

                rc = ngx_output_chain_align_file_buf(ctx, bsize);

                जर (rc == NGX_ERROR) {
                    परत NGX_ERROR;
                }

                जर (rc != NGX_OK) {

                    जर (ctx->मुक्त) {

                        /* get the free buf */

                        cl = ctx->मुक्त;
                        ctx->buf = cl->buf;
                        ctx->मुक्त = cl->next;

                        ngx_free_chain(ctx->pool, cl);

                    } अन्यथा जर (out || ctx->allocated == ctx->bufs.अंक) {

                        थांबा;

                    } अन्यथा जर (ngx_output_chain_get_buf(ctx, bsize) != NGX_OK) {
                        परत NGX_ERROR;
                    }
                }
            }

            rc = ngx_output_chain_copy_buf(ctx);

            जर (rc == NGX_ERROR) {
                परत rc;
            }

            जर (rc == NGX_AGAIN) {
                जर (out) {
                    थांबा;
                }

                परत rc;
            }

            /* delete the completed buf from the ctx->in chain */

            जर (ngx_buf_size(ctx->in->buf) == 0) {
                cl = ctx->in;
                ctx->in = cl->next;

                ngx_free_chain(ctx->pool, cl);
            }

            cl = ngx_alloc_chain_link(ctx->pool);
            जर (cl == रिक्त) {
                परत NGX_ERROR;
            }

            cl->buf = ctx->buf;
            cl->next = रिक्त;
            *last_out = cl;
            last_out = &cl->next;
            ctx->buf = रिक्त;
        }

        जर (out == रिक्त && last != NGX_NONE) {

            जर (ctx->in) {
                परत NGX_AGAIN;
            }

            परत last;
        }

        last = ctx->output_filter(ctx->filter_ctx, out);

        जर (last == NGX_ERROR || last == NGX_DONE) {
            परत last;
        }

        ngx_chain_update_chains(ctx->pool, &ctx->मुक्त, &ctx->busy, &out,
                                ctx->tag);
        last_out = &out;
    }
}


स्थिर ngx_inline ngx_int_t
ngx_output_chain_as_is(ngx_output_chain_ctx_t *ctx, ngx_buf_t *buf)
{
    ngx_uint_t  sendfile;

    जर (ngx_buf_special(buf)) {
        परत 1;
    }

#जर (NGX_THREADS)
    जर (buf->in_file) {
        buf->file->thread_handler = ctx->thread_handler;
        buf->file->thread_ctx = ctx->filter_ctx;
    }
#endif

    sendfile = ctx->sendfile;

#जर (NGX_SENDFILE_LIMIT)

    जर (buf->in_file && buf->file_pos >= NGX_SENDFILE_LIMIT) {
        sendfile = 0;
    }

#endif

#जर !(NGX_HAVE_SENDFILE_NODISKIO)

    /*
     * With DIRECTIO, disable sendfile() unless sendfile(SF_NOCACHE)
     * is available.
     */

    जर (buf->in_file && buf->file->directio) {
        sendfile = 0;
    }

#endif

    जर (!sendfile) {

        जर (!ngx_buf_in_memory(buf)) {
            परत 0;
        }

        buf->in_file = 0;
    }

    जर (ctx->need_in_memory && !ngx_buf_in_memory(buf)) {
        परत 0;
    }

    जर (ctx->need_in_temp && (buf->memory || buf->mmap)) {
        परत 0;
    }

    परत 1;
}


स्थिर ngx_int_t
ngx_output_chain_add_copy(ngx_pool_t *pool, ngx_chain_t **chain,
    ngx_chain_t *in)
{
    ngx_chain_t  *cl, **ll;
#जर (NGX_SENDFILE_LIMIT)
    ngx_buf_t    *b, *buf;
#endif

    ll = chain;

    साठी (cl = *chain; cl; cl = cl->next) {
        ll = &cl->next;
    }

    पर्यंत (in) {

        cl = ngx_alloc_chain_link(pool);
        जर (cl == रिक्त) {
            परत NGX_ERROR;
        }

#जर (NGX_SENDFILE_LIMIT)

        buf = in->buf;

        जर (buf->in_file
            && buf->file_pos < NGX_SENDFILE_LIMIT
            && buf->file_last > NGX_SENDFILE_LIMIT)
        {
            /* split a file buf on two bufs by the sendfile limit */

            b = ngx_calloc_buf(pool);
            जर (b == रिक्त) {
                परत NGX_ERROR;
            }

            ngx_memcpy(b, buf, आकार(ngx_buf_t));

            जर (ngx_buf_in_memory(buf)) {
                buf->pos += (ssize_t) (NGX_SENDFILE_LIMIT - buf->file_pos);
                b->last = buf->pos;
            }

            buf->file_pos = NGX_SENDFILE_LIMIT;
            b->file_last = NGX_SENDFILE_LIMIT;

            cl->buf = b;

        } अन्यथा {
            cl->buf = buf;
            in = in->next;
        }

#अन्यथा
        cl->buf = in->buf;
        in = in->next;

#endif

        cl->next = रिक्त;
        *ll = cl;
        ll = &cl->next;
    }

    परत NGX_OK;
}


स्थिर ngx_int_t
ngx_output_chain_align_file_buf(ngx_output_chain_ctx_t *ctx, off_t bsize)
{
    माप_प्रकार      size;
    ngx_buf_t  *in;

    in = ctx->in->buf;

    जर (in->file == रिक्त || !in->file->directio) {
        परत NGX_DECLINED;
    }

    ctx->directio = 1;

    size = (माप_प्रकार) (in->file_pos - (in->file_pos & ~(ctx->alignment - 1)));

    जर (size == 0) {

        जर (bsize >= (off_t) ctx->bufs.size) {
            परत NGX_DECLINED;
        }

        size = (माप_प्रकार) bsize;

    } अन्यथा {
        size = (माप_प्रकार) ctx->alignment - size;

        जर ((off_t) size > bsize) {
            size = (माप_प्रकार) bsize;
        }
    }

    ctx->buf = ngx_create_temp_buf(ctx->pool, size);
    जर (ctx->buf == रिक्त) {
        परत NGX_ERROR;
    }

    /*
     * we do not set ctx->buf->tag, because we do not want
     * to reuse the buf via ctx->free list
     */

#जर (NGX_HAVE_ALIGNED_DIRECTIO)
    ctx->unaligned = 1;
#endif

    परत NGX_OK;
}


स्थिर ngx_int_t
ngx_output_chain_get_buf(ngx_output_chain_ctx_t *ctx, off_t bsize)
{
    माप_प्रकार       size;
    ngx_buf_t   *b, *in;
    ngx_uint_t   recycled;

    in = ctx->in->buf;
    size = ctx->bufs.size;
    recycled = 1;

    जर (in->last_in_chain) {

        जर (bsize < (off_t) size) {

            /*
             * allocate a small temp buf for a small last buf
             * or its small last part
             */

            size = (माप_प्रकार) bsize;
            recycled = 0;

        } अन्यथा जर (!ctx->directio
                   && ctx->bufs.अंक == 1
                   && (bsize < (off_t) (size + size / 4)))
        {
            /*
             * allocate a temp buf that equals to a last buf,
             * if there is no directio, the last buf size is lesser
             * than 1.25 of bufs.size and the temp buf is single
             */

            size = (माप_प्रकार) bsize;
            recycled = 0;
        }
    }

    b = ngx_calloc_buf(ctx->pool);
    जर (b == रिक्त) {
        परत NGX_ERROR;
    }

    जर (ctx->directio) {

        /*
         * allocate block aligned to a disk sector size to enable
         * userland buffer direct usage conjunctly with directio
         */

        b->start = ngx_pmemalign(ctx->pool, size, (माप_प्रकार) ctx->alignment);
        जर (b->start == रिक्त) {
            परत NGX_ERROR;
        }

    } अन्यथा {
        b->start = ngx_palloc(ctx->pool, size);
        जर (b->start == रिक्त) {
            परत NGX_ERROR;
        }
    }

    b->pos = b->start;
    b->last = b->start;
    b->end = b->last + size;
    b->temporary = 1;
    b->tag = ctx->tag;
    b->recycled = recycled;

    ctx->buf = b;
    ctx->allocated++;

    परत NGX_OK;
}


स्थिर ngx_int_t
ngx_output_chain_copy_buf(ngx_output_chain_ctx_t *ctx)
{
    off_t        size;
    ssize_t      n;
    ngx_buf_t   *src, *dst;
    ngx_uint_t   sendfile;

    src = ctx->in->buf;
    dst = ctx->buf;

    size = ngx_buf_size(src);
    size = ngx_min(size, dst->end - dst->pos);

    sendfile = ctx->sendfile && !ctx->directio;

#जर (NGX_SENDFILE_LIMIT)

    जर (src->in_file && src->file_pos >= NGX_SENDFILE_LIMIT) {
        sendfile = 0;
    }

#endif

    जर (ngx_buf_in_memory(src)) {
        ngx_memcpy(dst->pos, src->pos, (माप_प्रकार) size);
        src->pos += (माप_प्रकार) size;
        dst->last += (माप_प्रकार) size;

        जर (src->in_file) {

            जर (sendfile) {
                dst->in_file = 1;
                dst->file = src->file;
                dst->file_pos = src->file_pos;
                dst->file_last = src->file_pos + size;

            } अन्यथा {
                dst->in_file = 0;
            }

            src->file_pos += size;

        } अन्यथा {
            dst->in_file = 0;
        }

        जर (src->pos == src->last) {
            dst->flush = src->flush;
            dst->last_buf = src->last_buf;
            dst->last_in_chain = src->last_in_chain;

        } अन्यथा {
            dst->flush = 0;
            dst->last_buf = 0;
            dst->last_in_chain = 0;
        }

    } अन्यथा {

#जर (NGX_HAVE_ALIGNED_DIRECTIO)

        जर (ctx->unaligned) {
            जर (ngx_directio_off(src->file->fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, ctx->pool->log, ngx_errno,
                              ngx_directio_off_n " \"%s\" failed",
                              src->file->name.data);
            }
        }

#endif

#जर (NGX_HAVE_FILE_AIO)
        जर (ctx->aio_handler) {
            n = ngx_file_aio_read(src->file, dst->pos, (माप_प्रकार) size,
                                  src->file_pos, ctx->pool);
            जर (n == NGX_AGAIN) {
                ctx->aio_handler(ctx, src->file);
                परत NGX_AGAIN;
            }

        } अन्यथा
#endif
#जर (NGX_THREADS)
        जर (ctx->thread_handler) {
            src->file->thread_task = ctx->thread_task;
            src->file->thread_handler = ctx->thread_handler;
            src->file->thread_ctx = ctx->filter_ctx;

            n = ngx_thread_read(src->file, dst->pos, (माप_प्रकार) size,
                                src->file_pos, ctx->pool);
            जर (n == NGX_AGAIN) {
                ctx->thread_task = src->file->thread_task;
                परत NGX_AGAIN;
            }

        } अन्यथा
#endif
        {
            n = ngx_read_file(src->file, dst->pos, (माप_प्रकार) size,
                              src->file_pos);
        }

#जर (NGX_HAVE_ALIGNED_DIRECTIO)

        जर (ctx->unaligned) {
            ngx_err_t  err;

            err = ngx_errno;

            जर (ngx_directio_on(src->file->fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, ctx->pool->log, ngx_errno,
                              ngx_directio_on_n " \"%s\" failed",
                              src->file->name.data);
            }

            ngx_set_errno(err);

            ctx->unaligned = 0;
        }

#endif

        जर (n == NGX_ERROR) {
            परत (ngx_int_t) n;
        }

        जर (n != size) {
            ngx_log_error(NGX_LOG_ALERT, ctx->pool->log, 0,
                          ngx_read_file_n " read only %z of %O from \"%s\"",
                          n, size, src->file->name.data);
            परत NGX_ERROR;
        }

        dst->last += n;

        जर (sendfile) {
            dst->in_file = 1;
            dst->file = src->file;
            dst->file_pos = src->file_pos;
            dst->file_last = src->file_pos + n;

        } अन्यथा {
            dst->in_file = 0;
        }

        src->file_pos += n;

        जर (src->file_pos == src->file_last) {
            dst->flush = src->flush;
            dst->last_buf = src->last_buf;
            dst->last_in_chain = src->last_in_chain;

        } अन्यथा {
            dst->flush = 0;
            dst->last_buf = 0;
            dst->last_in_chain = 0;
        }
    }

    परत NGX_OK;
}


ngx_int_t
ngx_chain_writer(शून्य *data, ngx_chain_t *in)
{
    ngx_chain_writer_ctx_t *ctx = data;

    off_t              size;
    ngx_chain_t       *cl, *ln, *chain;
    ngx_connection_t  *c;

    c = ctx->connection;

    साठी (size = 0; in; in = in->next) {

        जर (ngx_buf_size(in->buf) == 0 && !ngx_buf_special(in->buf)) {

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

            सुरूठेवा;
        }

        जर (ngx_buf_size(in->buf) < 0) {

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

            परत NGX_ERROR;
        }

        size += ngx_buf_size(in->buf);

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "chain writer buf fl:%d s:%uO",
                       in->buf->flush, ngx_buf_size(in->buf));

        cl = ngx_alloc_chain_link(ctx->pool);
        जर (cl == रिक्त) {
            परत NGX_ERROR;
        }

        cl->buf = in->buf;
        cl->next = रिक्त;
        *ctx->last = cl;
        ctx->last = &cl->next;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                   "chain writer in: %p", ctx->out);

    साठी (cl = ctx->out; cl; cl = cl->next) {

        जर (ngx_buf_size(cl->buf) == 0 && !ngx_buf_special(cl->buf)) {

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

            सुरूठेवा;
        }

        जर (ngx_buf_size(cl->buf) < 0) {

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

            परत NGX_ERROR;
        }

        size += ngx_buf_size(cl->buf);
    }

    जर (size == 0 && !c->buffered) {
        परत NGX_OK;
    }

    chain = c->send_chain(c, ctx->out, ctx->limit);

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                   "chain writer out: %p", chain);

    जर (chain == NGX_CHAIN_ERROR) {
        परत NGX_ERROR;
    }

    जर (chain && c->लिख->ready) {
        ngx_post_event(c->लिख, &ngx_posted_next_events);
    }

    साठी (cl = ctx->out; cl && cl != chain; /* void */) {
        ln = cl;
        cl = cl->next;
        ngx_free_chain(ctx->pool, ln);
    }

    ctx->out = chain;

    जर (ctx->out == रिक्त) {
        ctx->last = &ctx->out;

        जर (!c->buffered) {
            परत NGX_OK;
        }
    }

    परत NGX_AGAIN;
}
