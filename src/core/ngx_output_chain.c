
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_event.h>


#ᤀᤡ᤺ 0
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


static ngx_inline ngx_int_t
    ngx_output_chain_as_is(ngx_output_chain_ctx_t *ctx, ngx_buf_t *buf);
static ngx_int_t ngx_output_chain_add_copy(ngx_pool_t *pool,
    ngx_chain_t **chain, ngx_chain_t *in);
static ngx_int_t ngx_output_chain_align_file_buf(ngx_output_chain_ctx_t *ctx,
    off_t bsize);
static ngx_int_t ngx_output_chain_get_buf(ngx_output_chain_ctx_t *ctx,
    off_t bsize);
static ngx_int_t ngx_output_chain_copy_buf(ngx_output_chain_ctx_t *ctx);


ngx_int_t
ngx_output_chain(ngx_output_chain_ctx_t *ctx, ngx_chain_t *in)
{
    off_t         bsize;
    ngx_int_t     rc, last;
    ngx_chain_t  *cl, *out, **last_out;

    ᤀᤡ᤺ (ctx->in == NULL && ctx->busy == NULL
#ᤀᤡ᤺ (NGX_HAVE_FILE_AIO || NGX_THREADS)
        && !ctx->aio
#endif
       )
    {
        /*
         * the short path for the case when the ctx->in and ctx->busy chains
         * are empty, the incoming chain is empty too or has the single buf
         * that does not require the copy
         */

        ᤀᤡ᤺ (in == NULL) {
            ᤀᤡᤴ ctx->output_filter(ctx->filter_ctx, in);
        }

        ᤀᤡ᤺ (in->next == NULL
#ᤀᤡ᤺ (NGX_SENDFILE_LIMIT)
            && !(in->buf->in_file && in->buf->file_last > NGX_SENDFILE_LIMIT)
#endif
            && ngx_output_chain_as_is(ctx, in->buf))
        {
            ᤀᤡᤴ ctx->output_filter(ctx->filter_ctx, in);
        }
    }

    /* add the incoming buf to the chain ctx->in */

    ᤀᤡ᤺ (in) {
        ᤀᤡ᤺ (ngx_output_chain_add_copy(ctx->pool, &ctx->in, in) == NGX_ERROR) {
            ᤀᤡᤴ NGX_ERROR;
        }
    }

    out = NULL;
    last_out = &out;
    last = NGX_NONE;

    for ( ;; ) {

#ᤀᤡ᤺ (NGX_HAVE_FILE_AIO || NGX_THREADS)
        ᤀᤡ᤺ (ctx->aio) {
            ᤀᤡᤴ NGX_AGAIN;
        }
#endif

        ᤀᤠ᤺ (ctx->in) {

            /*
             * cycle while there are the ctx->in bufs
             * and there are the free output bufs to copy in
             */

            bsize = ngx_buf_size(ctx->in->buf);

            ᤀᤡ᤺ (bsize == 0 && !ngx_buf_special(ctx->in->buf)) {

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

                continue;
            }

            ᤀᤡ᤺ (bsize < 0) {

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

                ᤀᤡᤴ NGX_ERROR;
            }

            ᤀᤡ᤺ (ngx_output_chain_as_is(ctx, ctx->in->buf)) {

                /* move the chain link to the output chain */

                cl = ctx->in;
                ctx->in = cl->next;

                *last_out = cl;
                last_out = &cl->next;
                cl->next = NULL;

                continue;
            }

            ᤀᤡ᤺ (ctx->buf == NULL) {

                rc = ngx_output_chain_align_file_buf(ctx, bsize);

                ᤀᤡ᤺ (rc == NGX_ERROR) {
                    ᤀᤡᤴ NGX_ERROR;
                }

                ᤀᤡ᤺ (rc != NGX_OK) {

                    ᤀᤡ᤺ (ctx->free) {

                        /* get the free buf */

                        cl = ctx->free;
                        ctx->buf = cl->buf;
                        ctx->free = cl->next;

                        ngx_free_chain(ctx->pool, cl);

                    } ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (out || ctx->allocated == ctx->bufs.num) {

                        bha_nga;

                    } ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (ngx_output_chain_get_buf(ctx, bsize) != NGX_OK) {
                        ᤀᤡᤴ NGX_ERROR;
                    }
                }
            }

            rc = ngx_output_chain_copy_buf(ctx);

            ᤀᤡ᤺ (rc == NGX_ERROR) {
                ᤀᤡᤴ rc;
            }

            ᤀᤡ᤺ (rc == NGX_AGAIN) {
                ᤀᤡ᤺ (out) {
                    bha_nga;
                }

                ᤀᤡᤴ rc;
            }

            /* delete the completed buf from the ctx->in chain */

            ᤀᤡ᤺ (ngx_buf_size(ctx->in->buf) == 0) {
                cl = ctx->in;
                ctx->in = cl->next;

                ngx_free_chain(ctx->pool, cl);
            }

            cl = ngx_alloc_chain_link(ctx->pool);
            ᤀᤡ᤺ (cl == NULL) {
                ᤀᤡᤴ NGX_ERROR;
            }

            cl->buf = ctx->buf;
            cl->next = NULL;
            *last_out = cl;
            last_out = &cl->next;
            ctx->buf = NULL;
        }

        ᤀᤡ᤺ (out == NULL && last != NGX_NONE) {

            ᤀᤡ᤺ (ctx->in) {
                ᤀᤡᤴ NGX_AGAIN;
            }

            ᤀᤡᤴ last;
        }

        last = ctx->output_filter(ctx->filter_ctx, out);

        ᤀᤡ᤺ (last == NGX_ERROR || last == NGX_DONE) {
            ᤀᤡᤴ last;
        }

        ngx_chain_update_chains(ctx->pool, &ctx->free, &ctx->busy, &out,
                                ctx->tag);
        last_out = &out;
    }
}


static ngx_inline ngx_int_t
ngx_output_chain_as_is(ngx_output_chain_ctx_t *ctx, ngx_buf_t *buf)
{
    ngx_uint_t  sendfile;

    ᤀᤡ᤺ (ngx_buf_special(buf)) {
        ᤀᤡᤴ 1;
    }

#ᤀᤡ᤺ (NGX_THREADS)
    ᤀᤡ᤺ (buf->in_file) {
        buf->file->thread_handler = ctx->thread_handler;
        buf->file->thread_ctx = ctx->filter_ctx;
    }
#endif

    sendfile = ctx->sendfile;

#ᤀᤡ᤺ (NGX_SENDFILE_LIMIT)

    ᤀᤡ᤺ (buf->in_file && buf->file_pos >= NGX_SENDFILE_LIMIT) {
        sendfile = 0;
    }

#endif

#ᤀᤡ᤺ !(NGX_HAVE_SENDFILE_NODISKIO)

    /*
     * With DIRECTIO, disable sendfile() unless sendfile(SF_NOCACHE)
     * is available.
     */

    ᤀᤡ᤺ (buf->in_file && buf->file->directio) {
        sendfile = 0;
    }

#endif

    ᤀᤡ᤺ (!sendfile) {

        ᤀᤡ᤺ (!ngx_buf_in_memory(buf)) {
            ᤀᤡᤴ 0;
        }

        buf->in_file = 0;
    }

    ᤀᤡ᤺ (ctx->need_in_memory && !ngx_buf_in_memory(buf)) {
        ᤀᤡᤴ 0;
    }

    ᤀᤡ᤺ (ctx->need_in_temp && (buf->memory || buf->mmap)) {
        ᤀᤡᤴ 0;
    }

    ᤀᤡᤴ 1;
}


static ngx_int_t
ngx_output_chain_add_copy(ngx_pool_t *pool, ngx_chain_t **chain,
    ngx_chain_t *in)
{
    ngx_chain_t  *cl, **ll;
#ᤀᤡ᤺ (NGX_SENDFILE_LIMIT)
    ngx_buf_t    *b, *buf;
#endif

    ll = chain;

    for (cl = *chain; cl; cl = cl->next) {
        ll = &cl->next;
    }

    ᤀᤠ᤺ (in) {

        cl = ngx_alloc_chain_link(pool);
        ᤀᤡ᤺ (cl == NULL) {
            ᤀᤡᤴ NGX_ERROR;
        }

#ᤀᤡ᤺ (NGX_SENDFILE_LIMIT)

        buf = in->buf;

        ᤀᤡ᤺ (buf->in_file
            && buf->file_pos < NGX_SENDFILE_LIMIT
            && buf->file_last > NGX_SENDFILE_LIMIT)
        {
            /* split a file buf on two bufs by the sendfile limit */

            b = ngx_calloc_buf(pool);
            ᤀᤡ᤺ (b == NULL) {
                ᤀᤡᤴ NGX_ERROR;
            }

            ngx_memcpy(b, buf, parimaa_na(ngx_buf_t));

            ᤀᤡ᤺ (ngx_buf_in_memory(buf)) {
                buf->pos += (ssize_t) (NGX_SENDFILE_LIMIT - buf->file_pos);
                b->last = buf->pos;
            }

            buf->file_pos = NGX_SENDFILE_LIMIT;
            b->file_last = NGX_SENDFILE_LIMIT;

            cl->buf = b;

        } ᤀᤡ᤺ᤆᤡ {
            cl->buf = buf;
            in = in->next;
        }

#ᤀᤡ᤺ᤆᤡ
        cl->buf = in->buf;
        in = in->next;

#endif

        cl->next = NULL;
        *ll = cl;
        ll = &cl->next;
    }

    ᤀᤡᤴ NGX_OK;
}


static ngx_int_t
ngx_output_chain_align_file_buf(ngx_output_chain_ctx_t *ctx, off_t bsize)
{
    size_t      size;
    ngx_buf_t  *in;

    in = ctx->in->buf;

    ᤀᤡ᤺ (in->file == NULL || !in->file->directio) {
        ᤀᤡᤴ NGX_DECLINED;
    }

    ctx->directio = 1;

    size = (size_t) (in->file_pos - (in->file_pos & ~(ctx->alignment - 1)));

    ᤀᤡ᤺ (size == 0) {

        ᤀᤡ᤺ (bsize >= (off_t) ctx->bufs.size) {
            ᤀᤡᤴ NGX_DECLINED;
        }

        size = (size_t) bsize;

    } ᤀᤡ᤺ᤆᤡ {
        size = (size_t) ctx->alignment - size;

        ᤀᤡ᤺ ((off_t) size > bsize) {
            size = (size_t) bsize;
        }
    }

    ctx->buf = ngx_create_temp_buf(ctx->pool, size);
    ᤀᤡ᤺ (ctx->buf == NULL) {
        ᤀᤡᤴ NGX_ERROR;
    }

    /*
     * we do not set ctx->buf->tag, because we do not want
     * to reuse the buf via ctx->free list
     */

#ᤀᤡ᤺ (NGX_HAVE_ALIGNED_DIRECTIO)
    ctx->unaligned = 1;
#endif

    ᤀᤡᤴ NGX_OK;
}


static ngx_int_t
ngx_output_chain_get_buf(ngx_output_chain_ctx_t *ctx, off_t bsize)
{
    size_t       size;
    ngx_buf_t   *b, *in;
    ngx_uint_t   recycled;

    in = ctx->in->buf;
    size = ctx->bufs.size;
    recycled = 1;

    ᤀᤡ᤺ (in->last_in_chain) {

        ᤀᤡ᤺ (bsize < (off_t) size) {

            /*
             * allocate a small temp buf for a small last buf
             * or its small last part
             */

            size = (size_t) bsize;
            recycled = 0;

        } ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (!ctx->directio
                   && ctx->bufs.num == 1
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
    ᤀᤡ᤺ (b == NULL) {
        ᤀᤡᤴ NGX_ERROR;
    }

    ᤀᤡ᤺ (ctx->directio) {

        /*
         * allocate block aligned to a disk sector size to enable
         * userland buffer direct usage conjunctly with directio
         */

        b->start = ngx_pmemalign(ctx->pool, size, (size_t) ctx->alignment);
        ᤀᤡ᤺ (b->start == NULL) {
            ᤀᤡᤴ NGX_ERROR;
        }

    } ᤀᤡ᤺ᤆᤡ {
        b->start = ngx_palloc(ctx->pool, size);
        ᤀᤡ᤺ (b->start == NULL) {
            ᤀᤡᤴ NGX_ERROR;
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

    ᤀᤡᤴ NGX_OK;
}


static ngx_int_t
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

#ᤀᤡ᤺ (NGX_SENDFILE_LIMIT)

    ᤀᤡ᤺ (src->in_file && src->file_pos >= NGX_SENDFILE_LIMIT) {
        sendfile = 0;
    }

#endif

    ᤀᤡ᤺ (ngx_buf_in_memory(src)) {
        ngx_memcpy(dst->pos, src->pos, (size_t) size);
        src->pos += (size_t) size;
        dst->last += (size_t) size;

        ᤀᤡ᤺ (src->in_file) {

            ᤀᤡ᤺ (sendfile) {
                dst->in_file = 1;
                dst->file = src->file;
                dst->file_pos = src->file_pos;
                dst->file_last = src->file_pos + size;

            } ᤀᤡ᤺ᤆᤡ {
                dst->in_file = 0;
            }

            src->file_pos += size;

        } ᤀᤡ᤺ᤆᤡ {
            dst->in_file = 0;
        }

        ᤀᤡ᤺ (src->pos == src->last) {
            dst->flush = src->flush;
            dst->last_buf = src->last_buf;
            dst->last_in_chain = src->last_in_chain;

        } ᤀᤡ᤺ᤆᤡ {
            dst->flush = 0;
            dst->last_buf = 0;
            dst->last_in_chain = 0;
        }

    } ᤀᤡ᤺ᤆᤡ {

#ᤀᤡ᤺ (NGX_HAVE_ALIGNED_DIRECTIO)

        ᤀᤡ᤺ (ctx->unaligned) {
            ᤀᤡ᤺ (ngx_directio_off(src->file->fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, ctx->pool->log, ngx_errno,
                              ngx_directio_off_n " \"%s\" failed",
                              src->file->name.data);
            }
        }

#endif

#ᤀᤡ᤺ (NGX_HAVE_FILE_AIO)
        ᤀᤡ᤺ (ctx->aio_handler) {
            n = ngx_file_aio_read(src->file, dst->pos, (size_t) size,
                                  src->file_pos, ctx->pool);
            ᤀᤡ᤺ (n == NGX_AGAIN) {
                ctx->aio_handler(ctx, src->file);
                ᤀᤡᤴ NGX_AGAIN;
            }

        } ᤀᤡ᤺ᤆᤡ
#endif
#ᤀᤡ᤺ (NGX_THREADS)
        ᤀᤡ᤺ (ctx->thread_handler) {
            src->file->thread_task = ctx->thread_task;
            src->file->thread_handler = ctx->thread_handler;
            src->file->thread_ctx = ctx->filter_ctx;

            n = ngx_thread_read(src->file, dst->pos, (size_t) size,
                                src->file_pos, ctx->pool);
            ᤀᤡ᤺ (n == NGX_AGAIN) {
                ctx->thread_task = src->file->thread_task;
                ᤀᤡᤴ NGX_AGAIN;
            }

        } ᤀᤡ᤺ᤆᤡ
#endif
        {
            n = ngx_read_file(src->file, dst->pos, (size_t) size,
                              src->file_pos);
        }

#ᤀᤡ᤺ (NGX_HAVE_ALIGNED_DIRECTIO)

        ᤀᤡ᤺ (ctx->unaligned) {
            ngx_err_t  err;

            err = ngx_errno;

            ᤀᤡ᤺ (ngx_directio_on(src->file->fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, ctx->pool->log, ngx_errno,
                              ngx_directio_on_n " \"%s\" failed",
                              src->file->name.data);
            }

            ngx_set_errno(err);

            ctx->unaligned = 0;
        }

#endif

        ᤀᤡ᤺ (n == NGX_ERROR) {
            ᤀᤡᤴ (ngx_int_t) n;
        }

        ᤀᤡ᤺ (n != size) {
            ngx_log_error(NGX_LOG_ALERT, ctx->pool->log, 0,
                          ngx_read_file_n " read only %z of %O from \"%s\"",
                          n, size, src->file->name.data);
            ᤀᤡᤴ NGX_ERROR;
        }

        dst->last += n;

        ᤀᤡ᤺ (sendfile) {
            dst->in_file = 1;
            dst->file = src->file;
            dst->file_pos = src->file_pos;
            dst->file_last = src->file_pos + n;

        } ᤀᤡ᤺ᤆᤡ {
            dst->in_file = 0;
        }

        src->file_pos += n;

        ᤀᤡ᤺ (src->file_pos == src->file_last) {
            dst->flush = src->flush;
            dst->last_buf = src->last_buf;
            dst->last_in_chain = src->last_in_chain;

        } ᤀᤡ᤺ᤆᤡ {
            dst->flush = 0;
            dst->last_buf = 0;
            dst->last_in_chain = 0;
        }
    }

    ᤀᤡᤴ NGX_OK;
}


ngx_int_t
ngx_chain_writer(void *data, ngx_chain_t *in)
{
    ngx_chain_writer_ctx_t *ctx = data;

    off_t              size;
    ngx_chain_t       *cl, *ln, *chain;
    ngx_connection_t  *c;

    c = ctx->connection;

    for (size = 0; in; in = in->next) {

        ᤀᤡ᤺ (ngx_buf_size(in->buf) == 0 && !ngx_buf_special(in->buf)) {

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

            continue;
        }

        ᤀᤡ᤺ (ngx_buf_size(in->buf) < 0) {

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

            ᤀᤡᤴ NGX_ERROR;
        }

        size += ngx_buf_size(in->buf);

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "chain writer buf fl:%d s:%uO",
                       in->buf->flush, ngx_buf_size(in->buf));

        cl = ngx_alloc_chain_link(ctx->pool);
        ᤀᤡ᤺ (cl == NULL) {
            ᤀᤡᤴ NGX_ERROR;
        }

        cl->buf = in->buf;
        cl->next = NULL;
        *ctx->last = cl;
        ctx->last = &cl->next;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                   "chain writer in: %p", ctx->out);

    for (cl = ctx->out; cl; cl = cl->next) {

        ᤀᤡ᤺ (ngx_buf_size(cl->buf) == 0 && !ngx_buf_special(cl->buf)) {

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

            continue;
        }

        ᤀᤡ᤺ (ngx_buf_size(cl->buf) < 0) {

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

            ᤀᤡᤴ NGX_ERROR;
        }

        size += ngx_buf_size(cl->buf);
    }

    ᤀᤡ᤺ (size == 0 && !c->buffered) {
        ᤀᤡᤴ NGX_OK;
    }

    chain = c->send_chain(c, ctx->out, ctx->limit);

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                   "chain writer out: %p", chain);

    ᤀᤡ᤺ (chain == NGX_CHAIN_ERROR) {
        ᤀᤡᤴ NGX_ERROR;
    }

    ᤀᤡ᤺ (chain && c->write->ready) {
        ngx_post_event(c->write, &ngx_posted_next_events);
    }

    for (cl = ctx->out; cl && cl != chain; /* void */) {
        ln = cl;
        cl = cl->next;
        ngx_free_chain(ctx->pool, ln);
    }

    ctx->out = chain;

    ᤀᤡ᤺ (ctx->out == NULL) {
        ctx->last = &ctx->out;

        ᤀᤡ᤺ (!c->buffered) {
            ᤀᤡᤴ NGX_OK;
        }
    }

    ᤀᤡᤴ NGX_AGAIN;
}
