
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>
#अन्तर्भाव <ngx_event.h>


#जइ 0
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

    जइ (ctx->in == रिक्त && ctx->busy == रिक्त
#जइ (NGX_HAVE_FILE_AIO || NGX_THREADS)
        && !ctx->aio
#endif
       )
    {
        /*
         * the short path for the case when the ctx->in and ctx->busy chains
         * are empty, the incoming chain is empty too or has the single buf
         * that does not require the copy
         */

        जइ (in == रिक्त) {
            प्रत्यागम ctx->output_filter(ctx->filter_ctx, in);
        }

        जइ (in->next == रिक्त
#जइ (NGX_SENDFILE_LIMIT)
            && !(in->buf->in_file && in->buf->file_last > NGX_SENDFILE_LIMIT)
#endif
            && ngx_output_chain_as_is(ctx, in->buf))
        {
            प्रत्यागम ctx->output_filter(ctx->filter_ctx, in);
        }
    }

    /* add the incoming buf to the chain ctx->in */

    जइ (in) {
        जइ (ngx_output_chain_add_copy(ctx->pool, &ctx->in, in) == NGX_ERROR) {
            प्रत्यागम NGX_ERROR;
        }
    }

    out = रिक्त;
    last_out = &out;
    last = NGX_NONE;

    प्रति ( ;; ) {

#जइ (NGX_HAVE_FILE_AIO || NGX_THREADS)
        जइ (ctx->aio) {
            प्रत्यागम NGX_AGAIN;
        }
#endif

        जाव (ctx->in) {

            /*
             * cycle while there are the ctx->in bufs
             * and there are the free output bufs to copy in
             */

            bsize = ngx_buf_size(ctx->in->buf);

            जइ (bsize == 0 && !ngx_buf_special(ctx->in->buf)) {

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

                अनुवर्त;
            }

            जइ (bsize < 0) {

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

                प्रत्यागम NGX_ERROR;
            }

            जइ (ngx_output_chain_as_is(ctx, ctx->in->buf)) {

                /* move the chain link to the output chain */

                cl = ctx->in;
                ctx->in = cl->next;

                *last_out = cl;
                last_out = &cl->next;
                cl->next = रिक्त;

                अनुवर्त;
            }

            जइ (ctx->buf == रिक्त) {

                rc = ngx_output_chain_align_file_buf(ctx, bsize);

                जइ (rc == NGX_ERROR) {
                    प्रत्यागम NGX_ERROR;
                }

                जइ (rc != NGX_OK) {

                    जइ (ctx->मुक्त) {

                        /* get the free buf */

                        cl = ctx->मुक्त;
                        ctx->buf = cl->buf;
                        ctx->मुक्त = cl->next;

                        ngx_free_chain(ctx->pool, cl);

                    } अण्णहा जइ (out || ctx->allocated == ctx->bufs.अङ्कः) {

                        भङ्ग;

                    } अण्णहा जइ (ngx_output_chain_get_buf(ctx, bsize) != NGX_OK) {
                        प्रत्यागम NGX_ERROR;
                    }
                }
            }

            rc = ngx_output_chain_copy_buf(ctx);

            जइ (rc == NGX_ERROR) {
                प्रत्यागम rc;
            }

            जइ (rc == NGX_AGAIN) {
                जइ (out) {
                    भङ्ग;
                }

                प्रत्यागम rc;
            }

            /* delete the completed buf from the ctx->in chain */

            जइ (ngx_buf_size(ctx->in->buf) == 0) {
                cl = ctx->in;
                ctx->in = cl->next;

                ngx_free_chain(ctx->pool, cl);
            }

            cl = ngx_alloc_chain_link(ctx->pool);
            जइ (cl == रिक्त) {
                प्रत्यागम NGX_ERROR;
            }

            cl->buf = ctx->buf;
            cl->next = रिक्त;
            *last_out = cl;
            last_out = &cl->next;
            ctx->buf = रिक्त;
        }

        जइ (out == रिक्त && last != NGX_NONE) {

            जइ (ctx->in) {
                प्रत्यागम NGX_AGAIN;
            }

            प्रत्यागम last;
        }

        last = ctx->output_filter(ctx->filter_ctx, out);

        जइ (last == NGX_ERROR || last == NGX_DONE) {
            प्रत्यागम last;
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

    जइ (ngx_buf_special(buf)) {
        प्रत्यागम 1;
    }

#जइ (NGX_THREADS)
    जइ (buf->in_file) {
        buf->file->thread_handler = ctx->thread_handler;
        buf->file->thread_ctx = ctx->filter_ctx;
    }
#endif

    sendfile = ctx->sendfile;

#जइ (NGX_SENDFILE_LIMIT)

    जइ (buf->in_file && buf->file_pos >= NGX_SENDFILE_LIMIT) {
        sendfile = 0;
    }

#endif

#जइ !(NGX_HAVE_SENDFILE_NODISKIO)

    /*
     * With DIRECTIO, disable sendfile() unless sendfile(SF_NOCACHE)
     * is available.
     */

    जइ (buf->in_file && buf->file->directio) {
        sendfile = 0;
    }

#endif

    जइ (!sendfile) {

        जइ (!ngx_buf_in_memory(buf)) {
            प्रत्यागम 0;
        }

        buf->in_file = 0;
    }

    जइ (ctx->need_in_memory && !ngx_buf_in_memory(buf)) {
        प्रत्यागम 0;
    }

    जइ (ctx->need_in_temp && (buf->memory || buf->mmap)) {
        प्रत्यागम 0;
    }

    प्रत्यागम 1;
}


स्थिर ngx_int_t
ngx_output_chain_add_copy(ngx_pool_t *pool, ngx_chain_t **chain,
    ngx_chain_t *in)
{
    ngx_chain_t  *cl, **ll;
#जइ (NGX_SENDFILE_LIMIT)
    ngx_buf_t    *b, *buf;
#endif

    ll = chain;

    प्रति (cl = *chain; cl; cl = cl->next) {
        ll = &cl->next;
    }

    जाव (in) {

        cl = ngx_alloc_chain_link(pool);
        जइ (cl == रिक्त) {
            प्रत्यागम NGX_ERROR;
        }

#जइ (NGX_SENDFILE_LIMIT)

        buf = in->buf;

        जइ (buf->in_file
            && buf->file_pos < NGX_SENDFILE_LIMIT
            && buf->file_last > NGX_SENDFILE_LIMIT)
        {
            /* split a file buf on two bufs by the sendfile limit */

            b = ngx_calloc_buf(pool);
            जइ (b == रिक्त) {
                प्रत्यागम NGX_ERROR;
            }

            ngx_memcpy(b, buf, परिमाण(ngx_buf_t));

            जइ (ngx_buf_in_memory(buf)) {
                buf->pos += (ssize_t) (NGX_SENDFILE_LIMIT - buf->file_pos);
                b->last = buf->pos;
            }

            buf->file_pos = NGX_SENDFILE_LIMIT;
            b->file_last = NGX_SENDFILE_LIMIT;

            cl->buf = b;

        } अण्णहा {
            cl->buf = buf;
            in = in->next;
        }

#अण्णहा
        cl->buf = in->buf;
        in = in->next;

#endif

        cl->next = रिक्त;
        *ll = cl;
        ll = &cl->next;
    }

    प्रत्यागम NGX_OK;
}


स्थिर ngx_int_t
ngx_output_chain_align_file_buf(ngx_output_chain_ctx_t *ctx, off_t bsize)
{
    size_t      size;
    ngx_buf_t  *in;

    in = ctx->in->buf;

    जइ (in->file == रिक्त || !in->file->directio) {
        प्रत्यागम NGX_DECLINED;
    }

    ctx->directio = 1;

    size = (size_t) (in->file_pos - (in->file_pos & ~(ctx->alignment - 1)));

    जइ (size == 0) {

        जइ (bsize >= (off_t) ctx->bufs.size) {
            प्रत्यागम NGX_DECLINED;
        }

        size = (size_t) bsize;

    } अण्णहा {
        size = (size_t) ctx->alignment - size;

        जइ ((off_t) size > bsize) {
            size = (size_t) bsize;
        }
    }

    ctx->buf = ngx_create_temp_buf(ctx->pool, size);
    जइ (ctx->buf == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

    /*
     * we do not set ctx->buf->tag, because we do not want
     * to reuse the buf via ctx->free list
     */

#जइ (NGX_HAVE_ALIGNED_DIRECTIO)
    ctx->unaligned = 1;
#endif

    प्रत्यागम NGX_OK;
}


स्थिर ngx_int_t
ngx_output_chain_get_buf(ngx_output_chain_ctx_t *ctx, off_t bsize)
{
    size_t       size;
    ngx_buf_t   *b, *in;
    ngx_uint_t   recycled;

    in = ctx->in->buf;
    size = ctx->bufs.size;
    recycled = 1;

    जइ (in->last_in_chain) {

        जइ (bsize < (off_t) size) {

            /*
             * allocate a small temp buf for a small last buf
             * or its small last part
             */

            size = (size_t) bsize;
            recycled = 0;

        } अण्णहा जइ (!ctx->directio
                   && ctx->bufs.अङ्कः == 1
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
    जइ (b == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

    जइ (ctx->directio) {

        /*
         * allocate block aligned to a disk sector size to enable
         * userland buffer direct usage conjunctly with directio
         */

        b->start = ngx_pmemalign(ctx->pool, size, (size_t) ctx->alignment);
        जइ (b->start == रिक्त) {
            प्रत्यागम NGX_ERROR;
        }

    } अण्णहा {
        b->start = ngx_palloc(ctx->pool, size);
        जइ (b->start == रिक्त) {
            प्रत्यागम NGX_ERROR;
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

    प्रत्यागम NGX_OK;
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

#जइ (NGX_SENDFILE_LIMIT)

    जइ (src->in_file && src->file_pos >= NGX_SENDFILE_LIMIT) {
        sendfile = 0;
    }

#endif

    जइ (ngx_buf_in_memory(src)) {
        ngx_memcpy(dst->pos, src->pos, (size_t) size);
        src->pos += (size_t) size;
        dst->last += (size_t) size;

        जइ (src->in_file) {

            जइ (sendfile) {
                dst->in_file = 1;
                dst->file = src->file;
                dst->file_pos = src->file_pos;
                dst->file_last = src->file_pos + size;

            } अण्णहा {
                dst->in_file = 0;
            }

            src->file_pos += size;

        } अण्णहा {
            dst->in_file = 0;
        }

        जइ (src->pos == src->last) {
            dst->flush = src->flush;
            dst->last_buf = src->last_buf;
            dst->last_in_chain = src->last_in_chain;

        } अण्णहा {
            dst->flush = 0;
            dst->last_buf = 0;
            dst->last_in_chain = 0;
        }

    } अण्णहा {

#जइ (NGX_HAVE_ALIGNED_DIRECTIO)

        जइ (ctx->unaligned) {
            जइ (ngx_directio_off(src->file->fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, ctx->pool->log, ngx_errno,
                              ngx_directio_off_n " \"%s\" failed",
                              src->file->name.data);
            }
        }

#endif

#जइ (NGX_HAVE_FILE_AIO)
        जइ (ctx->aio_handler) {
            n = ngx_file_aio_read(src->file, dst->pos, (size_t) size,
                                  src->file_pos, ctx->pool);
            जइ (n == NGX_AGAIN) {
                ctx->aio_handler(ctx, src->file);
                प्रत्यागम NGX_AGAIN;
            }

        } अण्णहा
#endif
#जइ (NGX_THREADS)
        जइ (ctx->thread_handler) {
            src->file->thread_task = ctx->thread_task;
            src->file->thread_handler = ctx->thread_handler;
            src->file->thread_ctx = ctx->filter_ctx;

            n = ngx_thread_read(src->file, dst->pos, (size_t) size,
                                src->file_pos, ctx->pool);
            जइ (n == NGX_AGAIN) {
                ctx->thread_task = src->file->thread_task;
                प्रत्यागम NGX_AGAIN;
            }

        } अण्णहा
#endif
        {
            n = ngx_read_file(src->file, dst->pos, (size_t) size,
                              src->file_pos);
        }

#जइ (NGX_HAVE_ALIGNED_DIRECTIO)

        जइ (ctx->unaligned) {
            ngx_err_t  err;

            err = ngx_errno;

            जइ (ngx_directio_on(src->file->fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, ctx->pool->log, ngx_errno,
                              ngx_directio_on_n " \"%s\" failed",
                              src->file->name.data);
            }

            ngx_set_errno(err);

            ctx->unaligned = 0;
        }

#endif

        जइ (n == NGX_ERROR) {
            प्रत्यागम (ngx_int_t) n;
        }

        जइ (n != size) {
            ngx_log_error(NGX_LOG_ALERT, ctx->pool->log, 0,
                          ngx_read_file_n " read only %z of %O from \"%s\"",
                          n, size, src->file->name.data);
            प्रत्यागम NGX_ERROR;
        }

        dst->last += n;

        जइ (sendfile) {
            dst->in_file = 1;
            dst->file = src->file;
            dst->file_pos = src->file_pos;
            dst->file_last = src->file_pos + n;

        } अण्णहा {
            dst->in_file = 0;
        }

        src->file_pos += n;

        जइ (src->file_pos == src->file_last) {
            dst->flush = src->flush;
            dst->last_buf = src->last_buf;
            dst->last_in_chain = src->last_in_chain;

        } अण्णहा {
            dst->flush = 0;
            dst->last_buf = 0;
            dst->last_in_chain = 0;
        }
    }

    प्रत्यागम NGX_OK;
}


ngx_int_t
ngx_chain_writer(शून्य *data, ngx_chain_t *in)
{
    ngx_chain_writer_ctx_t *ctx = data;

    off_t              size;
    ngx_chain_t       *cl, *ln, *chain;
    ngx_connection_t  *c;

    c = ctx->connection;

    प्रति (size = 0; in; in = in->next) {

        जइ (ngx_buf_size(in->buf) == 0 && !ngx_buf_special(in->buf)) {

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

            अनुवर्त;
        }

        जइ (ngx_buf_size(in->buf) < 0) {

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

            प्रत्यागम NGX_ERROR;
        }

        size += ngx_buf_size(in->buf);

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "chain writer buf fl:%d s:%uO",
                       in->buf->flush, ngx_buf_size(in->buf));

        cl = ngx_alloc_chain_link(ctx->pool);
        जइ (cl == रिक्त) {
            प्रत्यागम NGX_ERROR;
        }

        cl->buf = in->buf;
        cl->next = रिक्त;
        *ctx->last = cl;
        ctx->last = &cl->next;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                   "chain writer in: %p", ctx->out);

    प्रति (cl = ctx->out; cl; cl = cl->next) {

        जइ (ngx_buf_size(cl->buf) == 0 && !ngx_buf_special(cl->buf)) {

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

            अनुवर्त;
        }

        जइ (ngx_buf_size(cl->buf) < 0) {

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

            प्रत्यागम NGX_ERROR;
        }

        size += ngx_buf_size(cl->buf);
    }

    जइ (size == 0 && !c->buffered) {
        प्रत्यागम NGX_OK;
    }

    chain = c->send_chain(c, ctx->out, ctx->limit);

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                   "chain writer out: %p", chain);

    जइ (chain == NGX_CHAIN_ERROR) {
        प्रत्यागम NGX_ERROR;
    }

    जइ (chain && c->लिख->ready) {
        ngx_post_event(c->लिख, &ngx_posted_next_events);
    }

    प्रति (cl = ctx->out; cl && cl != chain; /* void */) {
        ln = cl;
        cl = cl->next;
        ngx_free_chain(ctx->pool, ln);
    }

    ctx->out = chain;

    जइ (ctx->out == रिक्त) {
        ctx->last = &ctx->out;

        जइ (!c->buffered) {
            प्रत्यागम NGX_OK;
        }
    }

    प्रत्यागम NGX_AGAIN;
}
