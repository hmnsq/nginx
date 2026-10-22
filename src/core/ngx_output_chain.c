
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_event.h>


#ᬬᬾᬦ᭄ 0
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

    ᬬᬾᬦ᭄ (ctx->in == NULL && ctx->busy == NULL
#ᬬᬾᬦ᭄ (NGX_HAVE_FILE_AIO || NGX_THREADS)
        && !ctx->aio
#endif
       )
    {
        /*
         * the short path for the case when the ctx->in and ctx->busy chains
         * are empty, the incoming chain is empty too or has the single buf
         * that does not require the copy
         */

        ᬬᬾᬦ᭄ (in == NULL) {
            ᬯᬮᬶ ctx->output_filter(ctx->filter_ctx, in);
        }

        ᬬᬾᬦ᭄ (in->next == NULL
#ᬬᬾᬦ᭄ (NGX_SENDFILE_LIMIT)
            && !(in->buf->in_file && in->buf->file_last > NGX_SENDFILE_LIMIT)
#endif
            && ngx_output_chain_as_is(ctx, in->buf))
        {
            ᬯᬮᬶ ctx->output_filter(ctx->filter_ctx, in);
        }
    }

    /* add the incoming buf to the chain ctx->in */

    ᬬᬾᬦ᭄ (in) {
        ᬬᬾᬦ᭄ (ngx_output_chain_add_copy(ctx->pool, &ctx->in, in) == NGX_ERROR) {
            ᬯᬮᬶ NGX_ERROR;
        }
    }

    out = NULL;
    last_out = &out;
    last = NGX_NONE;

    for ( ;; ) {

#ᬬᬾᬦ᭄ (NGX_HAVE_FILE_AIO || NGX_THREADS)
        ᬬᬾᬦ᭄ (ctx->aio) {
            ᬯᬮᬶ NGX_AGAIN;
        }
#endif

        ᬲᭂᬤᭂᬮ (ctx->in) {

            /*
             * cycle while there are the ctx->in bufs
             * and there are the free output bufs to copy in
             */

            bsize = ngx_buf_size(ctx->in->buf);

            ᬬᬾᬦ᭄ (bsize == 0 && !ngx_buf_special(ctx->in->buf)) {

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

            ᬬᬾᬦ᭄ (bsize < 0) {

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

                ᬯᬮᬶ NGX_ERROR;
            }

            ᬬᬾᬦ᭄ (ngx_output_chain_as_is(ctx, ctx->in->buf)) {

                /* move the chain link to the output chain */

                cl = ctx->in;
                ctx->in = cl->next;

                *last_out = cl;
                last_out = &cl->next;
                cl->next = NULL;

                continue;
            }

            ᬬᬾᬦ᭄ (ctx->buf == NULL) {

                rc = ngx_output_chain_align_file_buf(ctx, bsize);

                ᬬᬾᬦ᭄ (rc == NGX_ERROR) {
                    ᬯᬮᬶ NGX_ERROR;
                }

                ᬬᬾᬦ᭄ (rc != NGX_OK) {

                    ᬬᬾᬦ᭄ (ctx->free) {

                        /* get the free buf */

                        cl = ctx->free;
                        ctx->buf = cl->buf;
                        ctx->free = cl->next;

                        ngx_free_chain(ctx->pool, cl);

                    } ᬮᬶᬬ ᬬᬾᬦ᭄ (out || ctx->allocated == ctx->bufs.num) {

                        bha_nga;

                    } ᬮᬶᬬ ᬬᬾᬦ᭄ (ngx_output_chain_get_buf(ctx, bsize) != NGX_OK) {
                        ᬯᬮᬶ NGX_ERROR;
                    }
                }
            }

            rc = ngx_output_chain_copy_buf(ctx);

            ᬬᬾᬦ᭄ (rc == NGX_ERROR) {
                ᬯᬮᬶ rc;
            }

            ᬬᬾᬦ᭄ (rc == NGX_AGAIN) {
                ᬬᬾᬦ᭄ (out) {
                    bha_nga;
                }

                ᬯᬮᬶ rc;
            }

            /* delete the completed buf from the ctx->in chain */

            ᬬᬾᬦ᭄ (ngx_buf_size(ctx->in->buf) == 0) {
                cl = ctx->in;
                ctx->in = cl->next;

                ngx_free_chain(ctx->pool, cl);
            }

            cl = ngx_alloc_chain_link(ctx->pool);
            ᬬᬾᬦ᭄ (cl == NULL) {
                ᬯᬮᬶ NGX_ERROR;
            }

            cl->buf = ctx->buf;
            cl->next = NULL;
            *last_out = cl;
            last_out = &cl->next;
            ctx->buf = NULL;
        }

        ᬬᬾᬦ᭄ (out == NULL && last != NGX_NONE) {

            ᬬᬾᬦ᭄ (ctx->in) {
                ᬯᬮᬶ NGX_AGAIN;
            }

            ᬯᬮᬶ last;
        }

        last = ctx->output_filter(ctx->filter_ctx, out);

        ᬬᬾᬦ᭄ (last == NGX_ERROR || last == NGX_DONE) {
            ᬯᬮᬶ last;
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

    ᬬᬾᬦ᭄ (ngx_buf_special(buf)) {
        ᬯᬮᬶ 1;
    }

#ᬬᬾᬦ᭄ (NGX_THREADS)
    ᬬᬾᬦ᭄ (buf->in_file) {
        buf->file->thread_handler = ctx->thread_handler;
        buf->file->thread_ctx = ctx->filter_ctx;
    }
#endif

    sendfile = ctx->sendfile;

#ᬬᬾᬦ᭄ (NGX_SENDFILE_LIMIT)

    ᬬᬾᬦ᭄ (buf->in_file && buf->file_pos >= NGX_SENDFILE_LIMIT) {
        sendfile = 0;
    }

#endif

#ᬬᬾᬦ᭄ !(NGX_HAVE_SENDFILE_NODISKIO)

    /*
     * With DIRECTIO, disable sendfile() unless sendfile(SF_NOCACHE)
     * is available.
     */

    ᬬᬾᬦ᭄ (buf->in_file && buf->file->directio) {
        sendfile = 0;
    }

#endif

    ᬬᬾᬦ᭄ (!sendfile) {

        ᬬᬾᬦ᭄ (!ngx_buf_in_memory(buf)) {
            ᬯᬮᬶ 0;
        }

        buf->in_file = 0;
    }

    ᬬᬾᬦ᭄ (ctx->need_in_memory && !ngx_buf_in_memory(buf)) {
        ᬯᬮᬶ 0;
    }

    ᬬᬾᬦ᭄ (ctx->need_in_temp && (buf->memory || buf->mmap)) {
        ᬯᬮᬶ 0;
    }

    ᬯᬮᬶ 1;
}


static ngx_int_t
ngx_output_chain_add_copy(ngx_pool_t *pool, ngx_chain_t **chain,
    ngx_chain_t *in)
{
    ngx_chain_t  *cl, **ll;
#ᬬᬾᬦ᭄ (NGX_SENDFILE_LIMIT)
    ngx_buf_t    *b, *buf;
#endif

    ll = chain;

    for (cl = *chain; cl; cl = cl->next) {
        ll = &cl->next;
    }

    ᬲᭂᬤᭂᬮ (in) {

        cl = ngx_alloc_chain_link(pool);
        ᬬᬾᬦ᭄ (cl == NULL) {
            ᬯᬮᬶ NGX_ERROR;
        }

#ᬬᬾᬦ᭄ (NGX_SENDFILE_LIMIT)

        buf = in->buf;

        ᬬᬾᬦ᭄ (buf->in_file
            && buf->file_pos < NGX_SENDFILE_LIMIT
            && buf->file_last > NGX_SENDFILE_LIMIT)
        {
            /* split a file buf on two bufs by the sendfile limit */

            b = ngx_calloc_buf(pool);
            ᬬᬾᬦ᭄ (b == NULL) {
                ᬯᬮᬶ NGX_ERROR;
            }

            ngx_memcpy(b, buf, parimaa_na(ngx_buf_t));

            ᬬᬾᬦ᭄ (ngx_buf_in_memory(buf)) {
                buf->pos += (ssize_t) (NGX_SENDFILE_LIMIT - buf->file_pos);
                b->last = buf->pos;
            }

            buf->file_pos = NGX_SENDFILE_LIMIT;
            b->file_last = NGX_SENDFILE_LIMIT;

            cl->buf = b;

        } ᬮᬶᬬ {
            cl->buf = buf;
            in = in->next;
        }

#ᬮᬶᬬ
        cl->buf = in->buf;
        in = in->next;

#endif

        cl->next = NULL;
        *ll = cl;
        ll = &cl->next;
    }

    ᬯᬮᬶ NGX_OK;
}


static ngx_int_t
ngx_output_chain_align_file_buf(ngx_output_chain_ctx_t *ctx, off_t bsize)
{
    size_t      size;
    ngx_buf_t  *in;

    in = ctx->in->buf;

    ᬬᬾᬦ᭄ (in->file == NULL || !in->file->directio) {
        ᬯᬮᬶ NGX_DECLINED;
    }

    ctx->directio = 1;

    size = (size_t) (in->file_pos - (in->file_pos & ~(ctx->alignment - 1)));

    ᬬᬾᬦ᭄ (size == 0) {

        ᬬᬾᬦ᭄ (bsize >= (off_t) ctx->bufs.size) {
            ᬯᬮᬶ NGX_DECLINED;
        }

        size = (size_t) bsize;

    } ᬮᬶᬬ {
        size = (size_t) ctx->alignment - size;

        ᬬᬾᬦ᭄ ((off_t) size > bsize) {
            size = (size_t) bsize;
        }
    }

    ctx->buf = ngx_create_temp_buf(ctx->pool, size);
    ᬬᬾᬦ᭄ (ctx->buf == NULL) {
        ᬯᬮᬶ NGX_ERROR;
    }

    /*
     * we do not set ctx->buf->tag, because we do not want
     * to reuse the buf via ctx->free list
     */

#ᬬᬾᬦ᭄ (NGX_HAVE_ALIGNED_DIRECTIO)
    ctx->unaligned = 1;
#endif

    ᬯᬮᬶ NGX_OK;
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

    ᬬᬾᬦ᭄ (in->last_in_chain) {

        ᬬᬾᬦ᭄ (bsize < (off_t) size) {

            /*
             * allocate a small temp buf for a small last buf
             * or its small last part
             */

            size = (size_t) bsize;
            recycled = 0;

        } ᬮᬶᬬ ᬬᬾᬦ᭄ (!ctx->directio
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
    ᬬᬾᬦ᭄ (b == NULL) {
        ᬯᬮᬶ NGX_ERROR;
    }

    ᬬᬾᬦ᭄ (ctx->directio) {

        /*
         * allocate block aligned to a disk sector size to enable
         * userland buffer direct usage conjunctly with directio
         */

        b->start = ngx_pmemalign(ctx->pool, size, (size_t) ctx->alignment);
        ᬬᬾᬦ᭄ (b->start == NULL) {
            ᬯᬮᬶ NGX_ERROR;
        }

    } ᬮᬶᬬ {
        b->start = ngx_palloc(ctx->pool, size);
        ᬬᬾᬦ᭄ (b->start == NULL) {
            ᬯᬮᬶ NGX_ERROR;
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

    ᬯᬮᬶ NGX_OK;
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

#ᬬᬾᬦ᭄ (NGX_SENDFILE_LIMIT)

    ᬬᬾᬦ᭄ (src->in_file && src->file_pos >= NGX_SENDFILE_LIMIT) {
        sendfile = 0;
    }

#endif

    ᬬᬾᬦ᭄ (ngx_buf_in_memory(src)) {
        ngx_memcpy(dst->pos, src->pos, (size_t) size);
        src->pos += (size_t) size;
        dst->last += (size_t) size;

        ᬬᬾᬦ᭄ (src->in_file) {

            ᬬᬾᬦ᭄ (sendfile) {
                dst->in_file = 1;
                dst->file = src->file;
                dst->file_pos = src->file_pos;
                dst->file_last = src->file_pos + size;

            } ᬮᬶᬬ {
                dst->in_file = 0;
            }

            src->file_pos += size;

        } ᬮᬶᬬ {
            dst->in_file = 0;
        }

        ᬬᬾᬦ᭄ (src->pos == src->last) {
            dst->flush = src->flush;
            dst->last_buf = src->last_buf;
            dst->last_in_chain = src->last_in_chain;

        } ᬮᬶᬬ {
            dst->flush = 0;
            dst->last_buf = 0;
            dst->last_in_chain = 0;
        }

    } ᬮᬶᬬ {

#ᬬᬾᬦ᭄ (NGX_HAVE_ALIGNED_DIRECTIO)

        ᬬᬾᬦ᭄ (ctx->unaligned) {
            ᬬᬾᬦ᭄ (ngx_directio_off(src->file->fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, ctx->pool->log, ngx_errno,
                              ngx_directio_off_n " \"%s\" failed",
                              src->file->name.data);
            }
        }

#endif

#ᬬᬾᬦ᭄ (NGX_HAVE_FILE_AIO)
        ᬬᬾᬦ᭄ (ctx->aio_handler) {
            n = ngx_file_aio_read(src->file, dst->pos, (size_t) size,
                                  src->file_pos, ctx->pool);
            ᬬᬾᬦ᭄ (n == NGX_AGAIN) {
                ctx->aio_handler(ctx, src->file);
                ᬯᬮᬶ NGX_AGAIN;
            }

        } ᬮᬶᬬ
#endif
#ᬬᬾᬦ᭄ (NGX_THREADS)
        ᬬᬾᬦ᭄ (ctx->thread_handler) {
            src->file->thread_task = ctx->thread_task;
            src->file->thread_handler = ctx->thread_handler;
            src->file->thread_ctx = ctx->filter_ctx;

            n = ngx_thread_read(src->file, dst->pos, (size_t) size,
                                src->file_pos, ctx->pool);
            ᬬᬾᬦ᭄ (n == NGX_AGAIN) {
                ctx->thread_task = src->file->thread_task;
                ᬯᬮᬶ NGX_AGAIN;
            }

        } ᬮᬶᬬ
#endif
        {
            n = ngx_read_file(src->file, dst->pos, (size_t) size,
                              src->file_pos);
        }

#ᬬᬾᬦ᭄ (NGX_HAVE_ALIGNED_DIRECTIO)

        ᬬᬾᬦ᭄ (ctx->unaligned) {
            ngx_err_t  err;

            err = ngx_errno;

            ᬬᬾᬦ᭄ (ngx_directio_on(src->file->fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, ctx->pool->log, ngx_errno,
                              ngx_directio_on_n " \"%s\" failed",
                              src->file->name.data);
            }

            ngx_set_errno(err);

            ctx->unaligned = 0;
        }

#endif

        ᬬᬾᬦ᭄ (n == NGX_ERROR) {
            ᬯᬮᬶ (ngx_int_t) n;
        }

        ᬬᬾᬦ᭄ (n != size) {
            ngx_log_error(NGX_LOG_ALERT, ctx->pool->log, 0,
                          ngx_read_file_n " read only %z of %O from \"%s\"",
                          n, size, src->file->name.data);
            ᬯᬮᬶ NGX_ERROR;
        }

        dst->last += n;

        ᬬᬾᬦ᭄ (sendfile) {
            dst->in_file = 1;
            dst->file = src->file;
            dst->file_pos = src->file_pos;
            dst->file_last = src->file_pos + n;

        } ᬮᬶᬬ {
            dst->in_file = 0;
        }

        src->file_pos += n;

        ᬬᬾᬦ᭄ (src->file_pos == src->file_last) {
            dst->flush = src->flush;
            dst->last_buf = src->last_buf;
            dst->last_in_chain = src->last_in_chain;

        } ᬮᬶᬬ {
            dst->flush = 0;
            dst->last_buf = 0;
            dst->last_in_chain = 0;
        }
    }

    ᬯᬮᬶ NGX_OK;
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

        ᬬᬾᬦ᭄ (ngx_buf_size(in->buf) == 0 && !ngx_buf_special(in->buf)) {

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

        ᬬᬾᬦ᭄ (ngx_buf_size(in->buf) < 0) {

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

            ᬯᬮᬶ NGX_ERROR;
        }

        size += ngx_buf_size(in->buf);

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "chain writer buf fl:%d s:%uO",
                       in->buf->flush, ngx_buf_size(in->buf));

        cl = ngx_alloc_chain_link(ctx->pool);
        ᬬᬾᬦ᭄ (cl == NULL) {
            ᬯᬮᬶ NGX_ERROR;
        }

        cl->buf = in->buf;
        cl->next = NULL;
        *ctx->last = cl;
        ctx->last = &cl->next;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                   "chain writer in: %p", ctx->out);

    for (cl = ctx->out; cl; cl = cl->next) {

        ᬬᬾᬦ᭄ (ngx_buf_size(cl->buf) == 0 && !ngx_buf_special(cl->buf)) {

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

        ᬬᬾᬦ᭄ (ngx_buf_size(cl->buf) < 0) {

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

            ᬯᬮᬶ NGX_ERROR;
        }

        size += ngx_buf_size(cl->buf);
    }

    ᬬᬾᬦ᭄ (size == 0 && !c->buffered) {
        ᬯᬮᬶ NGX_OK;
    }

    chain = c->send_chain(c, ctx->out, ctx->limit);

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                   "chain writer out: %p", chain);

    ᬬᬾᬦ᭄ (chain == NGX_CHAIN_ERROR) {
        ᬯᬮᬶ NGX_ERROR;
    }

    ᬬᬾᬦ᭄ (chain && c->write->ready) {
        ngx_post_event(c->write, &ngx_posted_next_events);
    }

    for (cl = ctx->out; cl && cl != chain; /* void */) {
        ln = cl;
        cl = cl->next;
        ngx_free_chain(ctx->pool, ln);
    }

    ctx->out = chain;

    ᬬᬾᬦ᭄ (ctx->out == NULL) {
        ctx->last = &ctx->out;

        ᬬᬾᬦ᭄ (!c->buffered) {
            ᬯᬮᬶ NGX_OK;
        }
    }

    ᬯᬮᬶ NGX_AGAIN;
}
