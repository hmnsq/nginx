
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>
#ಅನ್ತರ್ಭಾವ <ngx_event.h>


#ಯದಿ 0
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


ಸ್ಥಿರ ngx_inline ngx_int_t
    ngx_output_chain_as_is(ngx_output_chain_ctx_t *ctx, ngx_buf_t *buf);
ಸ್ಥಿರ ngx_int_t ngx_output_chain_add_copy(ngx_pool_t *pool,
    ngx_chain_t **chain, ngx_chain_t *in);
ಸ್ಥಿರ ngx_int_t ngx_output_chain_align_file_buf(ngx_output_chain_ctx_t *ctx,
    off_t bsize);
ಸ್ಥಿರ ngx_int_t ngx_output_chain_get_buf(ngx_output_chain_ctx_t *ctx,
    off_t bsize);
ಸ್ಥಿರ ngx_int_t ngx_output_chain_copy_buf(ngx_output_chain_ctx_t *ctx);


ngx_int_t
ngx_output_chain(ngx_output_chain_ctx_t *ctx, ngx_chain_t *in)
{
    off_t         bsize;
    ngx_int_t     rc, last;
    ngx_chain_t  *cl, *out, **last_out;

    ಯದಿ (ctx->in == ರಿಕ್ತ && ctx->busy == ರಿಕ್ತ
#ಯದಿ (NGX_HAVE_FILE_AIO || NGX_THREADS)
        && !ctx->aio
#endif
       )
    {
        /*
         * the short path for the case when the ctx->in and ctx->busy chains
         * are empty, the incoming chain is empty too or has the single buf
         * that does not require the copy
         */

        ಯದಿ (in == ರಿಕ್ತ) {
            ಪ್ರತ್ಯಾಗಮ ctx->output_filter(ctx->filter_ctx, in);
        }

        ಯದಿ (in->next == ರಿಕ್ತ
#ಯದಿ (NGX_SENDFILE_LIMIT)
            && !(in->buf->in_file && in->buf->file_last > NGX_SENDFILE_LIMIT)
#endif
            && ngx_output_chain_as_is(ctx, in->buf))
        {
            ಪ್ರತ್ಯಾಗಮ ctx->output_filter(ctx->filter_ctx, in);
        }
    }

    /* add the incoming buf to the chain ctx->in */

    ಯದಿ (in) {
        ಯದಿ (ngx_output_chain_add_copy(ctx->pool, &ctx->in, in) == NGX_ERROR) {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }
    }

    out = ರಿಕ್ತ;
    last_out = &out;
    last = NGX_NONE;

    ಪ್ರತಿ ( ;; ) {

#ಯದಿ (NGX_HAVE_FILE_AIO || NGX_THREADS)
        ಯದಿ (ctx->aio) {
            ಪ್ರತ್ಯಾಗಮ NGX_AGAIN;
        }
#endif

        ಯಾವತ್ (ctx->in) {

            /*
             * cycle while there are the ctx->in bufs
             * and there are the free output bufs to copy in
             */

            bsize = ngx_buf_size(ctx->in->buf);

            ಯದಿ (bsize == 0 && !ngx_buf_special(ctx->in->buf)) {

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

                ಅನುವರ್ತ;
            }

            ಯದಿ (bsize < 0) {

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

                ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
            }

            ಯದಿ (ngx_output_chain_as_is(ctx, ctx->in->buf)) {

                /* move the chain link to the output chain */

                cl = ctx->in;
                ctx->in = cl->next;

                *last_out = cl;
                last_out = &cl->next;
                cl->next = ರಿಕ್ತ;

                ಅನುವರ್ತ;
            }

            ಯದಿ (ctx->buf == ರಿಕ್ತ) {

                rc = ngx_output_chain_align_file_buf(ctx, bsize);

                ಯದಿ (rc == NGX_ERROR) {
                    ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
                }

                ಯದಿ (rc != NGX_OK) {

                    ಯದಿ (ctx->ಮುಕ್ತ) {

                        /* get the free buf */

                        cl = ctx->ಮುಕ್ತ;
                        ctx->buf = cl->buf;
                        ctx->ಮುಕ್ತ = cl->next;

                        ngx_free_chain(ctx->pool, cl);

                    } ಅನ್ಯಥಾ ಯದಿ (out || ctx->allocated == ctx->bufs.ಅಂಕೆ) {

                        ಭಙ್ಗ;

                    } ಅನ್ಯಥಾ ಯದಿ (ngx_output_chain_get_buf(ctx, bsize) != NGX_OK) {
                        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
                    }
                }
            }

            rc = ngx_output_chain_copy_buf(ctx);

            ಯದಿ (rc == NGX_ERROR) {
                ಪ್ರತ್ಯಾಗಮ rc;
            }

            ಯದಿ (rc == NGX_AGAIN) {
                ಯದಿ (out) {
                    ಭಙ್ಗ;
                }

                ಪ್ರತ್ಯಾಗಮ rc;
            }

            /* delete the completed buf from the ctx->in chain */

            ಯದಿ (ngx_buf_size(ctx->in->buf) == 0) {
                cl = ctx->in;
                ctx->in = cl->next;

                ngx_free_chain(ctx->pool, cl);
            }

            cl = ngx_alloc_chain_link(ctx->pool);
            ಯದಿ (cl == ರಿಕ್ತ) {
                ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
            }

            cl->buf = ctx->buf;
            cl->next = ರಿಕ್ತ;
            *last_out = cl;
            last_out = &cl->next;
            ctx->buf = ರಿಕ್ತ;
        }

        ಯದಿ (out == ರಿಕ್ತ && last != NGX_NONE) {

            ಯದಿ (ctx->in) {
                ಪ್ರತ್ಯಾಗಮ NGX_AGAIN;
            }

            ಪ್ರತ್ಯಾಗಮ last;
        }

        last = ctx->output_filter(ctx->filter_ctx, out);

        ಯದಿ (last == NGX_ERROR || last == NGX_DONE) {
            ಪ್ರತ್ಯಾಗಮ last;
        }

        ngx_chain_update_chains(ctx->pool, &ctx->ಮುಕ್ತ, &ctx->busy, &out,
                                ctx->tag);
        last_out = &out;
    }
}


ಸ್ಥಿರ ngx_inline ngx_int_t
ngx_output_chain_as_is(ngx_output_chain_ctx_t *ctx, ngx_buf_t *buf)
{
    ngx_uint_t  sendfile;

    ಯದಿ (ngx_buf_special(buf)) {
        ಪ್ರತ್ಯಾಗಮ 1;
    }

#ಯದಿ (NGX_THREADS)
    ಯದಿ (buf->in_file) {
        buf->file->thread_handler = ctx->thread_handler;
        buf->file->thread_ctx = ctx->filter_ctx;
    }
#endif

    sendfile = ctx->sendfile;

#ಯದಿ (NGX_SENDFILE_LIMIT)

    ಯದಿ (buf->in_file && buf->file_pos >= NGX_SENDFILE_LIMIT) {
        sendfile = 0;
    }

#endif

#ಯದಿ !(NGX_HAVE_SENDFILE_NODISKIO)

    /*
     * With DIRECTIO, disable sendfile() unless sendfile(SF_NOCACHE)
     * is available.
     */

    ಯದಿ (buf->in_file && buf->file->directio) {
        sendfile = 0;
    }

#endif

    ಯದಿ (!sendfile) {

        ಯದಿ (!ngx_buf_in_memory(buf)) {
            ಪ್ರತ್ಯಾಗಮ 0;
        }

        buf->in_file = 0;
    }

    ಯದಿ (ctx->need_in_memory && !ngx_buf_in_memory(buf)) {
        ಪ್ರತ್ಯಾಗಮ 0;
    }

    ಯದಿ (ctx->need_in_temp && (buf->memory || buf->mmap)) {
        ಪ್ರತ್ಯಾಗಮ 0;
    }

    ಪ್ರತ್ಯಾಗಮ 1;
}


ಸ್ಥಿರ ngx_int_t
ngx_output_chain_add_copy(ngx_pool_t *pool, ngx_chain_t **chain,
    ngx_chain_t *in)
{
    ngx_chain_t  *cl, **ll;
#ಯದಿ (NGX_SENDFILE_LIMIT)
    ngx_buf_t    *b, *buf;
#endif

    ll = chain;

    ಪ್ರತಿ (cl = *chain; cl; cl = cl->next) {
        ll = &cl->next;
    }

    ಯಾವತ್ (in) {

        cl = ngx_alloc_chain_link(pool);
        ಯದಿ (cl == ರಿಕ್ತ) {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

#ಯದಿ (NGX_SENDFILE_LIMIT)

        buf = in->buf;

        ಯದಿ (buf->in_file
            && buf->file_pos < NGX_SENDFILE_LIMIT
            && buf->file_last > NGX_SENDFILE_LIMIT)
        {
            /* split a file buf on two bufs by the sendfile limit */

            b = ngx_calloc_buf(pool);
            ಯದಿ (b == ರಿಕ್ತ) {
                ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
            }

            ngx_memcpy(b, buf, ಪರಿಮಾಣ(ngx_buf_t));

            ಯದಿ (ngx_buf_in_memory(buf)) {
                buf->pos += (ssize_t) (NGX_SENDFILE_LIMIT - buf->file_pos);
                b->last = buf->pos;
            }

            buf->file_pos = NGX_SENDFILE_LIMIT;
            b->file_last = NGX_SENDFILE_LIMIT;

            cl->buf = b;

        } ಅನ್ಯಥಾ {
            cl->buf = buf;
            in = in->next;
        }

#ಅನ್ಯಥಾ
        cl->buf = in->buf;
        in = in->next;

#endif

        cl->next = ರಿಕ್ತ;
        *ll = cl;
        ll = &cl->next;
    }

    ಪ್ರತ್ಯಾಗಮ NGX_OK;
}


ಸ್ಥಿರ ngx_int_t
ngx_output_chain_align_file_buf(ngx_output_chain_ctx_t *ctx, off_t bsize)
{
    size_t      size;
    ngx_buf_t  *in;

    in = ctx->in->buf;

    ಯದಿ (in->file == ರಿಕ್ತ || !in->file->directio) {
        ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;
    }

    ctx->directio = 1;

    size = (size_t) (in->file_pos - (in->file_pos & ~(ctx->alignment - 1)));

    ಯದಿ (size == 0) {

        ಯದಿ (bsize >= (off_t) ctx->bufs.size) {
            ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;
        }

        size = (size_t) bsize;

    } ಅನ್ಯಥಾ {
        size = (size_t) ctx->alignment - size;

        ಯದಿ ((off_t) size > bsize) {
            size = (size_t) bsize;
        }
    }

    ctx->buf = ngx_create_temp_buf(ctx->pool, size);
    ಯದಿ (ctx->buf == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    /*
     * we do not set ctx->buf->tag, because we do not want
     * to reuse the buf via ctx->free list
     */

#ಯದಿ (NGX_HAVE_ALIGNED_DIRECTIO)
    ctx->unaligned = 1;
#endif

    ಪ್ರತ್ಯಾಗಮ NGX_OK;
}


ಸ್ಥಿರ ngx_int_t
ngx_output_chain_get_buf(ngx_output_chain_ctx_t *ctx, off_t bsize)
{
    size_t       size;
    ngx_buf_t   *b, *in;
    ngx_uint_t   recycled;

    in = ctx->in->buf;
    size = ctx->bufs.size;
    recycled = 1;

    ಯದಿ (in->last_in_chain) {

        ಯದಿ (bsize < (off_t) size) {

            /*
             * allocate a small temp buf for a small last buf
             * or its small last part
             */

            size = (size_t) bsize;
            recycled = 0;

        } ಅನ್ಯಥಾ ಯದಿ (!ctx->directio
                   && ctx->bufs.ಅಂಕೆ == 1
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
    ಯದಿ (b == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    ಯದಿ (ctx->directio) {

        /*
         * allocate block aligned to a disk sector size to enable
         * userland buffer direct usage conjunctly with directio
         */

        b->start = ngx_pmemalign(ctx->pool, size, (size_t) ctx->alignment);
        ಯದಿ (b->start == ರಿಕ್ತ) {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

    } ಅನ್ಯಥಾ {
        b->start = ngx_palloc(ctx->pool, size);
        ಯದಿ (b->start == ರಿಕ್ತ) {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
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

    ಪ್ರತ್ಯಾಗಮ NGX_OK;
}


ಸ್ಥಿರ ngx_int_t
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

#ಯದಿ (NGX_SENDFILE_LIMIT)

    ಯದಿ (src->in_file && src->file_pos >= NGX_SENDFILE_LIMIT) {
        sendfile = 0;
    }

#endif

    ಯದಿ (ngx_buf_in_memory(src)) {
        ngx_memcpy(dst->pos, src->pos, (size_t) size);
        src->pos += (size_t) size;
        dst->last += (size_t) size;

        ಯದಿ (src->in_file) {

            ಯದಿ (sendfile) {
                dst->in_file = 1;
                dst->file = src->file;
                dst->file_pos = src->file_pos;
                dst->file_last = src->file_pos + size;

            } ಅನ್ಯಥಾ {
                dst->in_file = 0;
            }

            src->file_pos += size;

        } ಅನ್ಯಥಾ {
            dst->in_file = 0;
        }

        ಯದಿ (src->pos == src->last) {
            dst->flush = src->flush;
            dst->last_buf = src->last_buf;
            dst->last_in_chain = src->last_in_chain;

        } ಅನ್ಯಥಾ {
            dst->flush = 0;
            dst->last_buf = 0;
            dst->last_in_chain = 0;
        }

    } ಅನ್ಯಥಾ {

#ಯದಿ (NGX_HAVE_ALIGNED_DIRECTIO)

        ಯದಿ (ctx->unaligned) {
            ಯದಿ (ngx_directio_off(src->file->fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, ctx->pool->log, ngx_errno,
                              ngx_directio_off_n " \"%s\" failed",
                              src->file->name.data);
            }
        }

#endif

#ಯದಿ (NGX_HAVE_FILE_AIO)
        ಯದಿ (ctx->aio_handler) {
            n = ngx_file_aio_read(src->file, dst->pos, (size_t) size,
                                  src->file_pos, ctx->pool);
            ಯದಿ (n == NGX_AGAIN) {
                ctx->aio_handler(ctx, src->file);
                ಪ್ರತ್ಯಾಗಮ NGX_AGAIN;
            }

        } ಅನ್ಯಥಾ
#endif
#ಯದಿ (NGX_THREADS)
        ಯದಿ (ctx->thread_handler) {
            src->file->thread_task = ctx->thread_task;
            src->file->thread_handler = ctx->thread_handler;
            src->file->thread_ctx = ctx->filter_ctx;

            n = ngx_thread_read(src->file, dst->pos, (size_t) size,
                                src->file_pos, ctx->pool);
            ಯದಿ (n == NGX_AGAIN) {
                ctx->thread_task = src->file->thread_task;
                ಪ್ರತ್ಯಾಗಮ NGX_AGAIN;
            }

        } ಅನ್ಯಥಾ
#endif
        {
            n = ngx_read_file(src->file, dst->pos, (size_t) size,
                              src->file_pos);
        }

#ಯದಿ (NGX_HAVE_ALIGNED_DIRECTIO)

        ಯದಿ (ctx->unaligned) {
            ngx_err_t  err;

            err = ngx_errno;

            ಯದಿ (ngx_directio_on(src->file->fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, ctx->pool->log, ngx_errno,
                              ngx_directio_on_n " \"%s\" failed",
                              src->file->name.data);
            }

            ngx_set_errno(err);

            ctx->unaligned = 0;
        }

#endif

        ಯದಿ (n == NGX_ERROR) {
            ಪ್ರತ್ಯಾಗಮ (ngx_int_t) n;
        }

        ಯದಿ (n != size) {
            ngx_log_error(NGX_LOG_ALERT, ctx->pool->log, 0,
                          ngx_read_file_n " read only %z of %O from \"%s\"",
                          n, size, src->file->name.data);
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        dst->last += n;

        ಯದಿ (sendfile) {
            dst->in_file = 1;
            dst->file = src->file;
            dst->file_pos = src->file_pos;
            dst->file_last = src->file_pos + n;

        } ಅನ್ಯಥಾ {
            dst->in_file = 0;
        }

        src->file_pos += n;

        ಯದಿ (src->file_pos == src->file_last) {
            dst->flush = src->flush;
            dst->last_buf = src->last_buf;
            dst->last_in_chain = src->last_in_chain;

        } ಅನ್ಯಥಾ {
            dst->flush = 0;
            dst->last_buf = 0;
            dst->last_in_chain = 0;
        }
    }

    ಪ್ರತ್ಯಾಗಮ NGX_OK;
}


ngx_int_t
ngx_chain_writer(ಶೂನ್ಯ *data, ngx_chain_t *in)
{
    ngx_chain_writer_ctx_t *ctx = data;

    off_t              size;
    ngx_chain_t       *cl, *ln, *chain;
    ngx_connection_t  *c;

    c = ctx->connection;

    ಪ್ರತಿ (size = 0; in; in = in->next) {

        ಯದಿ (ngx_buf_size(in->buf) == 0 && !ngx_buf_special(in->buf)) {

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

            ಅನುವರ್ತ;
        }

        ಯದಿ (ngx_buf_size(in->buf) < 0) {

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

            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        size += ngx_buf_size(in->buf);

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "chain writer buf fl:%d s:%uO",
                       in->buf->flush, ngx_buf_size(in->buf));

        cl = ngx_alloc_chain_link(ctx->pool);
        ಯದಿ (cl == ರಿಕ್ತ) {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        cl->buf = in->buf;
        cl->next = ರಿಕ್ತ;
        *ctx->last = cl;
        ctx->last = &cl->next;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                   "chain writer in: %p", ctx->out);

    ಪ್ರತಿ (cl = ctx->out; cl; cl = cl->next) {

        ಯದಿ (ngx_buf_size(cl->buf) == 0 && !ngx_buf_special(cl->buf)) {

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

            ಅನುವರ್ತ;
        }

        ಯದಿ (ngx_buf_size(cl->buf) < 0) {

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

            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        size += ngx_buf_size(cl->buf);
    }

    ಯದಿ (size == 0 && !c->buffered) {
        ಪ್ರತ್ಯಾಗಮ NGX_OK;
    }

    chain = c->send_chain(c, ctx->out, ctx->limit);

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                   "chain writer out: %p", chain);

    ಯದಿ (chain == NGX_CHAIN_ERROR) {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    ಯದಿ (chain && c->ಲಿಖ->ready) {
        ngx_post_event(c->ಲಿಖ, &ngx_posted_next_events);
    }

    ಪ್ರತಿ (cl = ctx->out; cl && cl != chain; /* void */) {
        ln = cl;
        cl = cl->next;
        ngx_free_chain(ctx->pool, ln);
    }

    ctx->out = chain;

    ಯದಿ (ctx->out == ರಿಕ್ತ) {
        ctx->last = &ctx->out;

        ಯದಿ (!c->buffered) {
            ಪ್ರತ್ಯಾಗಮ NGX_OK;
        }
    }

    ಪ್ರತ್ಯಾಗಮ NGX_AGAIN;
}
