
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#അന്തര്ഭാവ <ngx_config.h>
#അന്തര്ഭാവ <ngx_core.h>
#അന്തര്ഭാവ <ngx_event.h>


#എങ്കിൽ 0
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


സ്ഥിതം ngx_inline ngx_int_t
    ngx_output_chain_as_is(ngx_output_chain_ctx_t *ctx, ngx_buf_t *buf);
സ്ഥിതം ngx_int_t ngx_output_chain_add_copy(ngx_pool_t *pool,
    ngx_chain_t **chain, ngx_chain_t *in);
സ്ഥിതം ngx_int_t ngx_output_chain_align_file_buf(ngx_output_chain_ctx_t *ctx,
    off_t bsize);
സ്ഥിതം ngx_int_t ngx_output_chain_get_buf(ngx_output_chain_ctx_t *ctx,
    off_t bsize);
സ്ഥിതം ngx_int_t ngx_output_chain_copy_buf(ngx_output_chain_ctx_t *ctx);


ngx_int_t
ngx_output_chain(ngx_output_chain_ctx_t *ctx, ngx_chain_t *in)
{
    off_t         bsize;
    ngx_int_t     rc, last;
    ngx_chain_t  *cl, *out, **last_out;

    എങ്കിൽ (ctx->in == ശൂന്യ && ctx->busy == ശൂന്യ
#എങ്കിൽ (NGX_HAVE_FILE_AIO || NGX_THREADS)
        && !ctx->aio
#endif
       )
    {
        /*
         * the short path for the case when the ctx->in and ctx->busy chains
         * are empty, the incoming chain is empty too or has the single buf
         * that does not require the copy
         */

        എങ്കിൽ (in == ശൂന്യ) {
            തിരികെ ctx->output_filter(ctx->filter_ctx, in);
        }

        എങ്കിൽ (in->next == ശൂന്യ
#എങ്കിൽ (NGX_SENDFILE_LIMIT)
            && !(in->buf->in_file && in->buf->file_last > NGX_SENDFILE_LIMIT)
#endif
            && ngx_output_chain_as_is(ctx, in->buf))
        {
            തിരികെ ctx->output_filter(ctx->filter_ctx, in);
        }
    }

    /* add the incoming buf to the chain ctx->in */

    എങ്കിൽ (in) {
        എങ്കിൽ (ngx_output_chain_add_copy(ctx->pool, &ctx->in, in) == NGX_ERROR) {
            തിരികെ NGX_ERROR;
        }
    }

    out = ശൂന്യ;
    last_out = &out;
    last = NGX_NONE;

    വേണ്ടി ( ;; ) {

#എങ്കിൽ (NGX_HAVE_FILE_AIO || NGX_THREADS)
        എങ്കിൽ (ctx->aio) {
            തിരികെ NGX_AGAIN;
        }
#endif

        ആകുമ്പോൾ (ctx->in) {

            /*
             * cycle while there are the ctx->in bufs
             * and there are the free output bufs to copy in
             */

            bsize = ngx_buf_size(ctx->in->buf);

            എങ്കിൽ (bsize == 0 && !ngx_buf_special(ctx->in->buf)) {

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

                തുടരുക;
            }

            എങ്കിൽ (bsize < 0) {

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

                തിരികെ NGX_ERROR;
            }

            എങ്കിൽ (ngx_output_chain_as_is(ctx, ctx->in->buf)) {

                /* move the chain link to the output chain */

                cl = ctx->in;
                ctx->in = cl->next;

                *last_out = cl;
                last_out = &cl->next;
                cl->next = ശൂന്യ;

                തുടരുക;
            }

            എങ്കിൽ (ctx->buf == ശൂന്യ) {

                rc = ngx_output_chain_align_file_buf(ctx, bsize);

                എങ്കിൽ (rc == NGX_ERROR) {
                    തിരികെ NGX_ERROR;
                }

                എങ്കിൽ (rc != NGX_OK) {

                    എങ്കിൽ (ctx->സ്വതന്ത്രം) {

                        /* get the free buf */

                        cl = ctx->സ്വതന്ത്രം;
                        ctx->buf = cl->buf;
                        ctx->സ്വതന്ത്രം = cl->next;

                        ngx_free_chain(ctx->pool, cl);

                    } അല്ലെങ്കിൽ എങ്കിൽ (out || ctx->allocated == ctx->bufs.അക്കം) {

                        നിർത്തുക;

                    } അല്ലെങ്കിൽ എങ്കിൽ (ngx_output_chain_get_buf(ctx, bsize) != NGX_OK) {
                        തിരികെ NGX_ERROR;
                    }
                }
            }

            rc = ngx_output_chain_copy_buf(ctx);

            എങ്കിൽ (rc == NGX_ERROR) {
                തിരികെ rc;
            }

            എങ്കിൽ (rc == NGX_AGAIN) {
                എങ്കിൽ (out) {
                    നിർത്തുക;
                }

                തിരികെ rc;
            }

            /* delete the completed buf from the ctx->in chain */

            എങ്കിൽ (ngx_buf_size(ctx->in->buf) == 0) {
                cl = ctx->in;
                ctx->in = cl->next;

                ngx_free_chain(ctx->pool, cl);
            }

            cl = ngx_alloc_chain_link(ctx->pool);
            എങ്കിൽ (cl == ശൂന്യ) {
                തിരികെ NGX_ERROR;
            }

            cl->buf = ctx->buf;
            cl->next = ശൂന്യ;
            *last_out = cl;
            last_out = &cl->next;
            ctx->buf = ശൂന്യ;
        }

        എങ്കിൽ (out == ശൂന്യ && last != NGX_NONE) {

            എങ്കിൽ (ctx->in) {
                തിരികെ NGX_AGAIN;
            }

            തിരികെ last;
        }

        last = ctx->output_filter(ctx->filter_ctx, out);

        എങ്കിൽ (last == NGX_ERROR || last == NGX_DONE) {
            തിരികെ last;
        }

        ngx_chain_update_chains(ctx->pool, &ctx->സ്വതന്ത്രം, &ctx->busy, &out,
                                ctx->tag);
        last_out = &out;
    }
}


സ്ഥിതം ngx_inline ngx_int_t
ngx_output_chain_as_is(ngx_output_chain_ctx_t *ctx, ngx_buf_t *buf)
{
    ngx_uint_t  sendfile;

    എങ്കിൽ (ngx_buf_special(buf)) {
        തിരികെ 1;
    }

#എങ്കിൽ (NGX_THREADS)
    എങ്കിൽ (buf->in_file) {
        buf->file->thread_handler = ctx->thread_handler;
        buf->file->thread_ctx = ctx->filter_ctx;
    }
#endif

    sendfile = ctx->sendfile;

#എങ്കിൽ (NGX_SENDFILE_LIMIT)

    എങ്കിൽ (buf->in_file && buf->file_pos >= NGX_SENDFILE_LIMIT) {
        sendfile = 0;
    }

#endif

#എങ്കിൽ !(NGX_HAVE_SENDFILE_NODISKIO)

    /*
     * With DIRECTIO, disable sendfile() unless sendfile(SF_NOCACHE)
     * is available.
     */

    എങ്കിൽ (buf->in_file && buf->file->directio) {
        sendfile = 0;
    }

#endif

    എങ്കിൽ (!sendfile) {

        എങ്കിൽ (!ngx_buf_in_memory(buf)) {
            തിരികെ 0;
        }

        buf->in_file = 0;
    }

    എങ്കിൽ (ctx->need_in_memory && !ngx_buf_in_memory(buf)) {
        തിരികെ 0;
    }

    എങ്കിൽ (ctx->need_in_temp && (buf->memory || buf->mmap)) {
        തിരികെ 0;
    }

    തിരികെ 1;
}


സ്ഥിതം ngx_int_t
ngx_output_chain_add_copy(ngx_pool_t *pool, ngx_chain_t **chain,
    ngx_chain_t *in)
{
    ngx_chain_t  *cl, **ll;
#എങ്കിൽ (NGX_SENDFILE_LIMIT)
    ngx_buf_t    *b, *buf;
#endif

    ll = chain;

    വേണ്ടി (cl = *chain; cl; cl = cl->next) {
        ll = &cl->next;
    }

    ആകുമ്പോൾ (in) {

        cl = ngx_alloc_chain_link(pool);
        എങ്കിൽ (cl == ശൂന്യ) {
            തിരികെ NGX_ERROR;
        }

#എങ്കിൽ (NGX_SENDFILE_LIMIT)

        buf = in->buf;

        എങ്കിൽ (buf->in_file
            && buf->file_pos < NGX_SENDFILE_LIMIT
            && buf->file_last > NGX_SENDFILE_LIMIT)
        {
            /* split a file buf on two bufs by the sendfile limit */

            b = ngx_calloc_buf(pool);
            എങ്കിൽ (b == ശൂന്യ) {
                തിരികെ NGX_ERROR;
            }

            ngx_memcpy(b, buf, വലുപ്പം(ngx_buf_t));

            എങ്കിൽ (ngx_buf_in_memory(buf)) {
                buf->pos += (ssize_t) (NGX_SENDFILE_LIMIT - buf->file_pos);
                b->last = buf->pos;
            }

            buf->file_pos = NGX_SENDFILE_LIMIT;
            b->file_last = NGX_SENDFILE_LIMIT;

            cl->buf = b;

        } അല്ലെങ്കിൽ {
            cl->buf = buf;
            in = in->next;
        }

#അല്ലെങ്കിൽ
        cl->buf = in->buf;
        in = in->next;

#endif

        cl->next = ശൂന്യ;
        *ll = cl;
        ll = &cl->next;
    }

    തിരികെ NGX_OK;
}


സ്ഥിതം ngx_int_t
ngx_output_chain_align_file_buf(ngx_output_chain_ctx_t *ctx, off_t bsize)
{
    size_t      size;
    ngx_buf_t  *in;

    in = ctx->in->buf;

    എങ്കിൽ (in->file == ശൂന്യ || !in->file->directio) {
        തിരികെ NGX_DECLINED;
    }

    ctx->directio = 1;

    size = (size_t) (in->file_pos - (in->file_pos & ~(ctx->alignment - 1)));

    എങ്കിൽ (size == 0) {

        എങ്കിൽ (bsize >= (off_t) ctx->bufs.size) {
            തിരികെ NGX_DECLINED;
        }

        size = (size_t) bsize;

    } അല്ലെങ്കിൽ {
        size = (size_t) ctx->alignment - size;

        എങ്കിൽ ((off_t) size > bsize) {
            size = (size_t) bsize;
        }
    }

    ctx->buf = ngx_create_temp_buf(ctx->pool, size);
    എങ്കിൽ (ctx->buf == ശൂന്യ) {
        തിരികെ NGX_ERROR;
    }

    /*
     * we do not set ctx->buf->tag, because we do not want
     * to reuse the buf via ctx->free list
     */

#എങ്കിൽ (NGX_HAVE_ALIGNED_DIRECTIO)
    ctx->unaligned = 1;
#endif

    തിരികെ NGX_OK;
}


സ്ഥിതം ngx_int_t
ngx_output_chain_get_buf(ngx_output_chain_ctx_t *ctx, off_t bsize)
{
    size_t       size;
    ngx_buf_t   *b, *in;
    ngx_uint_t   recycled;

    in = ctx->in->buf;
    size = ctx->bufs.size;
    recycled = 1;

    എങ്കിൽ (in->last_in_chain) {

        എങ്കിൽ (bsize < (off_t) size) {

            /*
             * allocate a small temp buf for a small last buf
             * or its small last part
             */

            size = (size_t) bsize;
            recycled = 0;

        } അല്ലെങ്കിൽ എങ്കിൽ (!ctx->directio
                   && ctx->bufs.അക്കം == 1
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
    എങ്കിൽ (b == ശൂന്യ) {
        തിരികെ NGX_ERROR;
    }

    എങ്കിൽ (ctx->directio) {

        /*
         * allocate block aligned to a disk sector size to enable
         * userland buffer direct usage conjunctly with directio
         */

        b->start = ngx_pmemalign(ctx->pool, size, (size_t) ctx->alignment);
        എങ്കിൽ (b->start == ശൂന്യ) {
            തിരികെ NGX_ERROR;
        }

    } അല്ലെങ്കിൽ {
        b->start = ngx_palloc(ctx->pool, size);
        എങ്കിൽ (b->start == ശൂന്യ) {
            തിരികെ NGX_ERROR;
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

    തിരികെ NGX_OK;
}


സ്ഥിതം ngx_int_t
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

#എങ്കിൽ (NGX_SENDFILE_LIMIT)

    എങ്കിൽ (src->in_file && src->file_pos >= NGX_SENDFILE_LIMIT) {
        sendfile = 0;
    }

#endif

    എങ്കിൽ (ngx_buf_in_memory(src)) {
        ngx_memcpy(dst->pos, src->pos, (size_t) size);
        src->pos += (size_t) size;
        dst->last += (size_t) size;

        എങ്കിൽ (src->in_file) {

            എങ്കിൽ (sendfile) {
                dst->in_file = 1;
                dst->file = src->file;
                dst->file_pos = src->file_pos;
                dst->file_last = src->file_pos + size;

            } അല്ലെങ്കിൽ {
                dst->in_file = 0;
            }

            src->file_pos += size;

        } അല്ലെങ്കിൽ {
            dst->in_file = 0;
        }

        എങ്കിൽ (src->pos == src->last) {
            dst->flush = src->flush;
            dst->last_buf = src->last_buf;
            dst->last_in_chain = src->last_in_chain;

        } അല്ലെങ്കിൽ {
            dst->flush = 0;
            dst->last_buf = 0;
            dst->last_in_chain = 0;
        }

    } അല്ലെങ്കിൽ {

#എങ്കിൽ (NGX_HAVE_ALIGNED_DIRECTIO)

        എങ്കിൽ (ctx->unaligned) {
            എങ്കിൽ (ngx_directio_off(src->file->fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, ctx->pool->log, ngx_errno,
                              ngx_directio_off_n " \"%s\" failed",
                              src->file->name.data);
            }
        }

#endif

#എങ്കിൽ (NGX_HAVE_FILE_AIO)
        എങ്കിൽ (ctx->aio_handler) {
            n = ngx_file_aio_read(src->file, dst->pos, (size_t) size,
                                  src->file_pos, ctx->pool);
            എങ്കിൽ (n == NGX_AGAIN) {
                ctx->aio_handler(ctx, src->file);
                തിരികെ NGX_AGAIN;
            }

        } അല്ലെങ്കിൽ
#endif
#എങ്കിൽ (NGX_THREADS)
        എങ്കിൽ (ctx->thread_handler) {
            src->file->thread_task = ctx->thread_task;
            src->file->thread_handler = ctx->thread_handler;
            src->file->thread_ctx = ctx->filter_ctx;

            n = ngx_thread_read(src->file, dst->pos, (size_t) size,
                                src->file_pos, ctx->pool);
            എങ്കിൽ (n == NGX_AGAIN) {
                ctx->thread_task = src->file->thread_task;
                തിരികെ NGX_AGAIN;
            }

        } അല്ലെങ്കിൽ
#endif
        {
            n = ngx_read_file(src->file, dst->pos, (size_t) size,
                              src->file_pos);
        }

#എങ്കിൽ (NGX_HAVE_ALIGNED_DIRECTIO)

        എങ്കിൽ (ctx->unaligned) {
            ngx_err_t  err;

            err = ngx_errno;

            എങ്കിൽ (ngx_directio_on(src->file->fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, ctx->pool->log, ngx_errno,
                              ngx_directio_on_n " \"%s\" failed",
                              src->file->name.data);
            }

            ngx_set_errno(err);

            ctx->unaligned = 0;
        }

#endif

        എങ്കിൽ (n == NGX_ERROR) {
            തിരികെ (ngx_int_t) n;
        }

        എങ്കിൽ (n != size) {
            ngx_log_error(NGX_LOG_ALERT, ctx->pool->log, 0,
                          ngx_read_file_n " read only %z of %O from \"%s\"",
                          n, size, src->file->name.data);
            തിരികെ NGX_ERROR;
        }

        dst->last += n;

        എങ്കിൽ (sendfile) {
            dst->in_file = 1;
            dst->file = src->file;
            dst->file_pos = src->file_pos;
            dst->file_last = src->file_pos + n;

        } അല്ലെങ്കിൽ {
            dst->in_file = 0;
        }

        src->file_pos += n;

        എങ്കിൽ (src->file_pos == src->file_last) {
            dst->flush = src->flush;
            dst->last_buf = src->last_buf;
            dst->last_in_chain = src->last_in_chain;

        } അല്ലെങ്കിൽ {
            dst->flush = 0;
            dst->last_buf = 0;
            dst->last_in_chain = 0;
        }
    }

    തിരികെ NGX_OK;
}


ngx_int_t
ngx_chain_writer(ശൂന്യം *data, ngx_chain_t *in)
{
    ngx_chain_writer_ctx_t *ctx = data;

    off_t              size;
    ngx_chain_t       *cl, *ln, *chain;
    ngx_connection_t  *c;

    c = ctx->connection;

    വേണ്ടി (size = 0; in; in = in->next) {

        എങ്കിൽ (ngx_buf_size(in->buf) == 0 && !ngx_buf_special(in->buf)) {

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

            തുടരുക;
        }

        എങ്കിൽ (ngx_buf_size(in->buf) < 0) {

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

            തിരികെ NGX_ERROR;
        }

        size += ngx_buf_size(in->buf);

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "chain writer buf fl:%d s:%uO",
                       in->buf->flush, ngx_buf_size(in->buf));

        cl = ngx_alloc_chain_link(ctx->pool);
        എങ്കിൽ (cl == ശൂന്യ) {
            തിരികെ NGX_ERROR;
        }

        cl->buf = in->buf;
        cl->next = ശൂന്യ;
        *ctx->last = cl;
        ctx->last = &cl->next;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                   "chain writer in: %p", ctx->out);

    വേണ്ടി (cl = ctx->out; cl; cl = cl->next) {

        എങ്കിൽ (ngx_buf_size(cl->buf) == 0 && !ngx_buf_special(cl->buf)) {

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

            തുടരുക;
        }

        എങ്കിൽ (ngx_buf_size(cl->buf) < 0) {

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

            തിരികെ NGX_ERROR;
        }

        size += ngx_buf_size(cl->buf);
    }

    എങ്കിൽ (size == 0 && !c->buffered) {
        തിരികെ NGX_OK;
    }

    chain = c->send_chain(c, ctx->out, ctx->limit);

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                   "chain writer out: %p", chain);

    എങ്കിൽ (chain == NGX_CHAIN_ERROR) {
        തിരികെ NGX_ERROR;
    }

    എങ്കിൽ (chain && c->ലിഖ->ready) {
        ngx_post_event(c->ലിഖ, &ngx_posted_next_events);
    }

    വേണ്ടി (cl = ctx->out; cl && cl != chain; /* void */) {
        ln = cl;
        cl = cl->next;
        ngx_free_chain(ctx->pool, ln);
    }

    ctx->out = chain;

    എങ്കിൽ (ctx->out == ശൂന്യ) {
        ctx->last = &ctx->out;

        എങ്കിൽ (!c->buffered) {
            തിരികെ NGX_OK;
        }
    }

    തിരികെ NGX_AGAIN;
}
