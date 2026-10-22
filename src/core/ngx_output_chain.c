
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#அந்தர்பாவ <ngx_config.h>
#அந்தர்பாவ <ngx_core.h>
#அந்தர்பாவ <ngx_event.h>


#எனில் 0
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


நிலையான ngx_inline ngx_int_t
    ngx_output_chain_as_is(ngx_output_chain_ctx_t *ctx, ngx_buf_t *buf);
நிலையான ngx_int_t ngx_output_chain_add_copy(ngx_pool_t *pool,
    ngx_chain_t **chain, ngx_chain_t *in);
நிலையான ngx_int_t ngx_output_chain_align_file_buf(ngx_output_chain_ctx_t *ctx,
    off_t bsize);
நிலையான ngx_int_t ngx_output_chain_get_buf(ngx_output_chain_ctx_t *ctx,
    off_t bsize);
நிலையான ngx_int_t ngx_output_chain_copy_buf(ngx_output_chain_ctx_t *ctx);


ngx_int_t
ngx_output_chain(ngx_output_chain_ctx_t *ctx, ngx_chain_t *in)
{
    off_t         bsize;
    ngx_int_t     rc, last;
    ngx_chain_t  *cl, *out, **last_out;

    எனில் (ctx->in == வெற்று && ctx->busy == வெற்று
#எனில் (NGX_HAVE_FILE_AIO || NGX_THREADS)
        && !ctx->aio
#endif
       )
    {
        /*
         * the short path for the case when the ctx->in and ctx->busy chains
         * are empty, the incoming chain is empty too or has the single buf
         * that does not require the copy
         */

        எனில் (in == வெற்று) {
            திரும்பு ctx->output_filter(ctx->filter_ctx, in);
        }

        எனில் (in->next == வெற்று
#எனில் (NGX_SENDFILE_LIMIT)
            && !(in->buf->in_file && in->buf->file_last > NGX_SENDFILE_LIMIT)
#endif
            && ngx_output_chain_as_is(ctx, in->buf))
        {
            திரும்பு ctx->output_filter(ctx->filter_ctx, in);
        }
    }

    /* add the incoming buf to the chain ctx->in */

    எனில் (in) {
        எனில் (ngx_output_chain_add_copy(ctx->pool, &ctx->in, in) == NGX_ERROR) {
            திரும்பு NGX_ERROR;
        }
    }

    out = வெற்று;
    last_out = &out;
    last = NGX_NONE;

    க்கு ( ;; ) {

#எனில் (NGX_HAVE_FILE_AIO || NGX_THREADS)
        எனில் (ctx->aio) {
            திரும்பு NGX_AGAIN;
        }
#endif

        போது (ctx->in) {

            /*
             * cycle while there are the ctx->in bufs
             * and there are the free output bufs to copy in
             */

            bsize = ngx_buf_size(ctx->in->buf);

            எனில் (bsize == 0 && !ngx_buf_special(ctx->in->buf)) {

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

                தொடர்;
            }

            எனில் (bsize < 0) {

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

                திரும்பு NGX_ERROR;
            }

            எனில் (ngx_output_chain_as_is(ctx, ctx->in->buf)) {

                /* move the chain link to the output chain */

                cl = ctx->in;
                ctx->in = cl->next;

                *last_out = cl;
                last_out = &cl->next;
                cl->next = வெற்று;

                தொடர்;
            }

            எனில் (ctx->buf == வெற்று) {

                rc = ngx_output_chain_align_file_buf(ctx, bsize);

                எனில் (rc == NGX_ERROR) {
                    திரும்பு NGX_ERROR;
                }

                எனில் (rc != NGX_OK) {

                    எனில் (ctx->விடுவி) {

                        /* get the free buf */

                        cl = ctx->விடுவி;
                        ctx->buf = cl->buf;
                        ctx->விடுவி = cl->next;

                        ngx_free_chain(ctx->pool, cl);

                    } இல்லையெனில் எனில் (out || ctx->allocated == ctx->bufs.இலக்கம்) {

                        நிறுத்து;

                    } இல்லையெனில் எனில் (ngx_output_chain_get_buf(ctx, bsize) != NGX_OK) {
                        திரும்பு NGX_ERROR;
                    }
                }
            }

            rc = ngx_output_chain_copy_buf(ctx);

            எனில் (rc == NGX_ERROR) {
                திரும்பு rc;
            }

            எனில் (rc == NGX_AGAIN) {
                எனில் (out) {
                    நிறுத்து;
                }

                திரும்பு rc;
            }

            /* delete the completed buf from the ctx->in chain */

            எனில் (ngx_buf_size(ctx->in->buf) == 0) {
                cl = ctx->in;
                ctx->in = cl->next;

                ngx_free_chain(ctx->pool, cl);
            }

            cl = ngx_alloc_chain_link(ctx->pool);
            எனில் (cl == வெற்று) {
                திரும்பு NGX_ERROR;
            }

            cl->buf = ctx->buf;
            cl->next = வெற்று;
            *last_out = cl;
            last_out = &cl->next;
            ctx->buf = வெற்று;
        }

        எனில் (out == வெற்று && last != NGX_NONE) {

            எனில் (ctx->in) {
                திரும்பு NGX_AGAIN;
            }

            திரும்பு last;
        }

        last = ctx->output_filter(ctx->filter_ctx, out);

        எனில் (last == NGX_ERROR || last == NGX_DONE) {
            திரும்பு last;
        }

        ngx_chain_update_chains(ctx->pool, &ctx->விடுவி, &ctx->busy, &out,
                                ctx->tag);
        last_out = &out;
    }
}


நிலையான ngx_inline ngx_int_t
ngx_output_chain_as_is(ngx_output_chain_ctx_t *ctx, ngx_buf_t *buf)
{
    ngx_uint_t  sendfile;

    எனில் (ngx_buf_special(buf)) {
        திரும்பு 1;
    }

#எனில் (NGX_THREADS)
    எனில் (buf->in_file) {
        buf->file->thread_handler = ctx->thread_handler;
        buf->file->thread_ctx = ctx->filter_ctx;
    }
#endif

    sendfile = ctx->sendfile;

#எனில் (NGX_SENDFILE_LIMIT)

    எனில் (buf->in_file && buf->file_pos >= NGX_SENDFILE_LIMIT) {
        sendfile = 0;
    }

#endif

#எனில் !(NGX_HAVE_SENDFILE_NODISKIO)

    /*
     * With DIRECTIO, disable sendfile() unless sendfile(SF_NOCACHE)
     * is available.
     */

    எனில் (buf->in_file && buf->file->directio) {
        sendfile = 0;
    }

#endif

    எனில் (!sendfile) {

        எனில் (!ngx_buf_in_memory(buf)) {
            திரும்பு 0;
        }

        buf->in_file = 0;
    }

    எனில் (ctx->need_in_memory && !ngx_buf_in_memory(buf)) {
        திரும்பு 0;
    }

    எனில் (ctx->need_in_temp && (buf->memory || buf->mmap)) {
        திரும்பு 0;
    }

    திரும்பு 1;
}


நிலையான ngx_int_t
ngx_output_chain_add_copy(ngx_pool_t *pool, ngx_chain_t **chain,
    ngx_chain_t *in)
{
    ngx_chain_t  *cl, **ll;
#எனில் (NGX_SENDFILE_LIMIT)
    ngx_buf_t    *b, *buf;
#endif

    ll = chain;

    க்கு (cl = *chain; cl; cl = cl->next) {
        ll = &cl->next;
    }

    போது (in) {

        cl = ngx_alloc_chain_link(pool);
        எனில் (cl == வெற்று) {
            திரும்பு NGX_ERROR;
        }

#எனில் (NGX_SENDFILE_LIMIT)

        buf = in->buf;

        எனில் (buf->in_file
            && buf->file_pos < NGX_SENDFILE_LIMIT
            && buf->file_last > NGX_SENDFILE_LIMIT)
        {
            /* split a file buf on two bufs by the sendfile limit */

            b = ngx_calloc_buf(pool);
            எனில் (b == வெற்று) {
                திரும்பு NGX_ERROR;
            }

            ngx_memcpy(b, buf, அளவு(ngx_buf_t));

            எனில் (ngx_buf_in_memory(buf)) {
                buf->pos += (ssize_t) (NGX_SENDFILE_LIMIT - buf->file_pos);
                b->last = buf->pos;
            }

            buf->file_pos = NGX_SENDFILE_LIMIT;
            b->file_last = NGX_SENDFILE_LIMIT;

            cl->buf = b;

        } இல்லையெனில் {
            cl->buf = buf;
            in = in->next;
        }

#இல்லையெனில்
        cl->buf = in->buf;
        in = in->next;

#endif

        cl->next = வெற்று;
        *ll = cl;
        ll = &cl->next;
    }

    திரும்பு NGX_OK;
}


நிலையான ngx_int_t
ngx_output_chain_align_file_buf(ngx_output_chain_ctx_t *ctx, off_t bsize)
{
    size_t      size;
    ngx_buf_t  *in;

    in = ctx->in->buf;

    எனில் (in->file == வெற்று || !in->file->directio) {
        திரும்பு NGX_DECLINED;
    }

    ctx->directio = 1;

    size = (size_t) (in->file_pos - (in->file_pos & ~(ctx->alignment - 1)));

    எனில் (size == 0) {

        எனில் (bsize >= (off_t) ctx->bufs.size) {
            திரும்பு NGX_DECLINED;
        }

        size = (size_t) bsize;

    } இல்லையெனில் {
        size = (size_t) ctx->alignment - size;

        எனில் ((off_t) size > bsize) {
            size = (size_t) bsize;
        }
    }

    ctx->buf = ngx_create_temp_buf(ctx->pool, size);
    எனில் (ctx->buf == வெற்று) {
        திரும்பு NGX_ERROR;
    }

    /*
     * we do not set ctx->buf->tag, because we do not want
     * to reuse the buf via ctx->free list
     */

#எனில் (NGX_HAVE_ALIGNED_DIRECTIO)
    ctx->unaligned = 1;
#endif

    திரும்பு NGX_OK;
}


நிலையான ngx_int_t
ngx_output_chain_get_buf(ngx_output_chain_ctx_t *ctx, off_t bsize)
{
    size_t       size;
    ngx_buf_t   *b, *in;
    ngx_uint_t   recycled;

    in = ctx->in->buf;
    size = ctx->bufs.size;
    recycled = 1;

    எனில் (in->last_in_chain) {

        எனில் (bsize < (off_t) size) {

            /*
             * allocate a small temp buf for a small last buf
             * or its small last part
             */

            size = (size_t) bsize;
            recycled = 0;

        } இல்லையெனில் எனில் (!ctx->directio
                   && ctx->bufs.இலக்கம் == 1
                   && (bsize < (off_t) (size + size / 4)))
        {
            /*
             * allocate a temp buf that equals to a last buf,
             * if there is no directio, the last buf size is lesser
             * than 1.25 of bufs.size and the temp buf is single
             */

            size = (size_t) bsize;
            recycled = 0;
        }
    }

    b = ngx_calloc_buf(ctx->pool);
    எனில் (b == வெற்று) {
        திரும்பு NGX_ERROR;
    }

    எனில் (ctx->directio) {

        /*
         * allocate block aligned to a disk sector size to enable
         * userland buffer direct usage conjunctly with directio
         */

        b->start = ngx_pmemalign(ctx->pool, size, (size_t) ctx->alignment);
        எனில் (b->start == வெற்று) {
            திரும்பு NGX_ERROR;
        }

    } இல்லையெனில் {
        b->start = ngx_palloc(ctx->pool, size);
        எனில் (b->start == வெற்று) {
            திரும்பு NGX_ERROR;
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

    திரும்பு NGX_OK;
}


நிலையான ngx_int_t
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

#எனில் (NGX_SENDFILE_LIMIT)

    எனில் (src->in_file && src->file_pos >= NGX_SENDFILE_LIMIT) {
        sendfile = 0;
    }

#endif

    எனில் (ngx_buf_in_memory(src)) {
        ngx_memcpy(dst->pos, src->pos, (size_t) size);
        src->pos += (size_t) size;
        dst->last += (size_t) size;

        எனில் (src->in_file) {

            எனில் (sendfile) {
                dst->in_file = 1;
                dst->file = src->file;
                dst->file_pos = src->file_pos;
                dst->file_last = src->file_pos + size;

            } இல்லையெனில் {
                dst->in_file = 0;
            }

            src->file_pos += size;

        } இல்லையெனில் {
            dst->in_file = 0;
        }

        எனில் (src->pos == src->last) {
            dst->flush = src->flush;
            dst->last_buf = src->last_buf;
            dst->last_in_chain = src->last_in_chain;

        } இல்லையெனில் {
            dst->flush = 0;
            dst->last_buf = 0;
            dst->last_in_chain = 0;
        }

    } இல்லையெனில் {

#எனில் (NGX_HAVE_ALIGNED_DIRECTIO)

        எனில் (ctx->unaligned) {
            எனில் (ngx_directio_off(src->file->fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, ctx->pool->log, ngx_errno,
                              ngx_directio_off_n " \"%s\" failed",
                              src->file->name.data);
            }
        }

#endif

#எனில் (NGX_HAVE_FILE_AIO)
        எனில் (ctx->aio_handler) {
            n = ngx_file_aio_read(src->file, dst->pos, (size_t) size,
                                  src->file_pos, ctx->pool);
            எனில் (n == NGX_AGAIN) {
                ctx->aio_handler(ctx, src->file);
                திரும்பு NGX_AGAIN;
            }

        } இல்லையெனில்
#endif
#எனில் (NGX_THREADS)
        எனில் (ctx->thread_handler) {
            src->file->thread_task = ctx->thread_task;
            src->file->thread_handler = ctx->thread_handler;
            src->file->thread_ctx = ctx->filter_ctx;

            n = ngx_thread_read(src->file, dst->pos, (size_t) size,
                                src->file_pos, ctx->pool);
            எனில் (n == NGX_AGAIN) {
                ctx->thread_task = src->file->thread_task;
                திரும்பு NGX_AGAIN;
            }

        } இல்லையெனில்
#endif
        {
            n = ngx_read_file(src->file, dst->pos, (size_t) size,
                              src->file_pos);
        }

#எனில் (NGX_HAVE_ALIGNED_DIRECTIO)

        எனில் (ctx->unaligned) {
            ngx_err_t  err;

            err = ngx_errno;

            எனில் (ngx_directio_on(src->file->fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, ctx->pool->log, ngx_errno,
                              ngx_directio_on_n " \"%s\" failed",
                              src->file->name.data);
            }

            ngx_set_errno(err);

            ctx->unaligned = 0;
        }

#endif

        எனில் (n == NGX_ERROR) {
            திரும்பு (ngx_int_t) n;
        }

        எனில் (n != size) {
            ngx_log_error(NGX_LOG_ALERT, ctx->pool->log, 0,
                          ngx_read_file_n " read only %z of %O from \"%s\"",
                          n, size, src->file->name.data);
            திரும்பு NGX_ERROR;
        }

        dst->last += n;

        எனில் (sendfile) {
            dst->in_file = 1;
            dst->file = src->file;
            dst->file_pos = src->file_pos;
            dst->file_last = src->file_pos + n;

        } இல்லையெனில் {
            dst->in_file = 0;
        }

        src->file_pos += n;

        எனில் (src->file_pos == src->file_last) {
            dst->flush = src->flush;
            dst->last_buf = src->last_buf;
            dst->last_in_chain = src->last_in_chain;

        } இல்லையெனில் {
            dst->flush = 0;
            dst->last_buf = 0;
            dst->last_in_chain = 0;
        }
    }

    திரும்பு NGX_OK;
}


ngx_int_t
ngx_chain_writer(வெறுமை *data, ngx_chain_t *in)
{
    ngx_chain_writer_ctx_t *ctx = data;

    off_t              size;
    ngx_chain_t       *cl, *ln, *chain;
    ngx_connection_t  *c;

    c = ctx->connection;

    க்கு (size = 0; in; in = in->next) {

        எனில் (ngx_buf_size(in->buf) == 0 && !ngx_buf_special(in->buf)) {

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

            தொடர்;
        }

        எனில் (ngx_buf_size(in->buf) < 0) {

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

            திரும்பு NGX_ERROR;
        }

        size += ngx_buf_size(in->buf);

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "chain writer buf fl:%d s:%uO",
                       in->buf->flush, ngx_buf_size(in->buf));

        cl = ngx_alloc_chain_link(ctx->pool);
        எனில் (cl == வெற்று) {
            திரும்பு NGX_ERROR;
        }

        cl->buf = in->buf;
        cl->next = வெற்று;
        *ctx->last = cl;
        ctx->last = &cl->next;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                   "chain writer in: %p", ctx->out);

    க்கு (cl = ctx->out; cl; cl = cl->next) {

        எனில் (ngx_buf_size(cl->buf) == 0 && !ngx_buf_special(cl->buf)) {

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

            தொடர்;
        }

        எனில் (ngx_buf_size(cl->buf) < 0) {

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

            திரும்பு NGX_ERROR;
        }

        size += ngx_buf_size(cl->buf);
    }

    எனில் (size == 0 && !c->buffered) {
        திரும்பு NGX_OK;
    }

    chain = c->send_chain(c, ctx->out, ctx->limit);

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                   "chain writer out: %p", chain);

    எனில் (chain == NGX_CHAIN_ERROR) {
        திரும்பு NGX_ERROR;
    }

    எனில் (chain && c->லிக->ready) {
        ngx_post_event(c->லிக, &ngx_posted_next_events);
    }

    க்கு (cl = ctx->out; cl && cl != chain; /* void */) {
        ln = cl;
        cl = cl->next;
        ngx_free_chain(ctx->pool, ln);
    }

    ctx->out = chain;

    எனில் (ctx->out == வெற்று) {
        ctx->last = &ctx->out;

        எனில் (!c->buffered) {
            திரும்பு NGX_OK;
        }
    }

    திரும்பு NGX_AGAIN;
}
