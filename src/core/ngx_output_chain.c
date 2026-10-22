
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_event.h>


#ប្រសិនបើ 0
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

    ប្រសិនបើ (ctx->in == NULL && ctx->busy == NULL
#ប្រសិនបើ (NGX_HAVE_FILE_AIO || NGX_THREADS)
        && !ctx->aio
#endif
       )
    {
        /*
         * the short path for the case when the ctx->in and ctx->busy chains
         * are empty, the incoming chain is empty too or has the single buf
         * that does not require the copy
         */

        ប្រសិនបើ (in == NULL) {
            ត្រឡប់ ctx->output_filter(ctx->filter_ctx, in);
        }

        ប្រសិនបើ (in->next == NULL
#ប្រសិនបើ (NGX_SENDFILE_LIMIT)
            && !(in->buf->in_file && in->buf->file_last > NGX_SENDFILE_LIMIT)
#endif
            && ngx_output_chain_as_is(ctx, in->buf))
        {
            ត្រឡប់ ctx->output_filter(ctx->filter_ctx, in);
        }
    }

    /* add the incoming buf to the chain ctx->in */

    ប្រសិនបើ (in) {
        ប្រសិនបើ (ngx_output_chain_add_copy(ctx->pool, &ctx->in, in) == NGX_ERROR) {
            ត្រឡប់ NGX_ERROR;
        }
    }

    out = NULL;
    last_out = &out;
    last = NGX_NONE;

    សម្រាប់ ( ;; ) {

#ប្រសិនបើ (NGX_HAVE_FILE_AIO || NGX_THREADS)
        ប្រសិនបើ (ctx->aio) {
            ត្រឡប់ NGX_AGAIN;
        }
#endif

        ខណៈពេល (ctx->in) {

            /*
             * cycle while there are the ctx->in bufs
             * and there are the free output bufs to copy in
             */

            bsize = ngx_buf_size(ctx->in->buf);

            ប្រសិនបើ (bsize == 0 && !ngx_buf_special(ctx->in->buf)) {

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

            ប្រសិនបើ (bsize < 0) {

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

                ត្រឡប់ NGX_ERROR;
            }

            ប្រសិនបើ (ngx_output_chain_as_is(ctx, ctx->in->buf)) {

                /* move the chain link to the output chain */

                cl = ctx->in;
                ctx->in = cl->next;

                *last_out = cl;
                last_out = &cl->next;
                cl->next = NULL;

                continue;
            }

            ប្រសិនបើ (ctx->buf == NULL) {

                rc = ngx_output_chain_align_file_buf(ctx, bsize);

                ប្រសិនបើ (rc == NGX_ERROR) {
                    ត្រឡប់ NGX_ERROR;
                }

                ប្រសិនបើ (rc != NGX_OK) {

                    ប្រសិនបើ (ctx->free) {

                        /* get the free buf */

                        cl = ctx->free;
                        ctx->buf = cl->buf;
                        ctx->free = cl->next;

                        ngx_free_chain(ctx->pool, cl);

                    } បើមិនដូច្នេះ ប្រសិនបើ (out || ctx->allocated == ctx->bufs.num) {

                        bha_nga;

                    } បើមិនដូច្នេះ ប្រសិនបើ (ngx_output_chain_get_buf(ctx, bsize) != NGX_OK) {
                        ត្រឡប់ NGX_ERROR;
                    }
                }
            }

            rc = ngx_output_chain_copy_buf(ctx);

            ប្រសិនបើ (rc == NGX_ERROR) {
                ត្រឡប់ rc;
            }

            ប្រសិនបើ (rc == NGX_AGAIN) {
                ប្រសិនបើ (out) {
                    bha_nga;
                }

                ត្រឡប់ rc;
            }

            /* delete the completed buf from the ctx->in chain */

            ប្រសិនបើ (ngx_buf_size(ctx->in->buf) == 0) {
                cl = ctx->in;
                ctx->in = cl->next;

                ngx_free_chain(ctx->pool, cl);
            }

            cl = ngx_alloc_chain_link(ctx->pool);
            ប្រសិនបើ (cl == NULL) {
                ត្រឡប់ NGX_ERROR;
            }

            cl->buf = ctx->buf;
            cl->next = NULL;
            *last_out = cl;
            last_out = &cl->next;
            ctx->buf = NULL;
        }

        ប្រសិនបើ (out == NULL && last != NGX_NONE) {

            ប្រសិនបើ (ctx->in) {
                ត្រឡប់ NGX_AGAIN;
            }

            ត្រឡប់ last;
        }

        last = ctx->output_filter(ctx->filter_ctx, out);

        ប្រសិនបើ (last == NGX_ERROR || last == NGX_DONE) {
            ត្រឡប់ last;
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

    ប្រសិនបើ (ngx_buf_special(buf)) {
        ត្រឡប់ 1;
    }

#ប្រសិនបើ (NGX_THREADS)
    ប្រសិនបើ (buf->in_file) {
        buf->file->thread_handler = ctx->thread_handler;
        buf->file->thread_ctx = ctx->filter_ctx;
    }
#endif

    sendfile = ctx->sendfile;

#ប្រសិនបើ (NGX_SENDFILE_LIMIT)

    ប្រសិនបើ (buf->in_file && buf->file_pos >= NGX_SENDFILE_LIMIT) {
        sendfile = 0;
    }

#endif

#ប្រសិនបើ !(NGX_HAVE_SENDFILE_NODISKIO)

    /*
     * With DIRECTIO, disable sendfile() unless sendfile(SF_NOCACHE)
     * is available.
     */

    ប្រសិនបើ (buf->in_file && buf->file->directio) {
        sendfile = 0;
    }

#endif

    ប្រសិនបើ (!sendfile) {

        ប្រសិនបើ (!ngx_buf_in_memory(buf)) {
            ត្រឡប់ 0;
        }

        buf->in_file = 0;
    }

    ប្រសិនបើ (ctx->need_in_memory && !ngx_buf_in_memory(buf)) {
        ត្រឡប់ 0;
    }

    ប្រសិនបើ (ctx->need_in_temp && (buf->memory || buf->mmap)) {
        ត្រឡប់ 0;
    }

    ត្រឡប់ 1;
}


static ngx_int_t
ngx_output_chain_add_copy(ngx_pool_t *pool, ngx_chain_t **chain,
    ngx_chain_t *in)
{
    ngx_chain_t  *cl, **ll;
#ប្រសិនបើ (NGX_SENDFILE_LIMIT)
    ngx_buf_t    *b, *buf;
#endif

    ll = chain;

    សម្រាប់ (cl = *chain; cl; cl = cl->next) {
        ll = &cl->next;
    }

    ខណៈពេល (in) {

        cl = ngx_alloc_chain_link(pool);
        ប្រសិនបើ (cl == NULL) {
            ត្រឡប់ NGX_ERROR;
        }

#ប្រសិនបើ (NGX_SENDFILE_LIMIT)

        buf = in->buf;

        ប្រសិនបើ (buf->in_file
            && buf->file_pos < NGX_SENDFILE_LIMIT
            && buf->file_last > NGX_SENDFILE_LIMIT)
        {
            /* split a file buf on two bufs by the sendfile limit */

            b = ngx_calloc_buf(pool);
            ប្រសិនបើ (b == NULL) {
                ត្រឡប់ NGX_ERROR;
            }

            ngx_memcpy(b, buf, parimaa_na(ngx_buf_t));

            ប្រសិនបើ (ngx_buf_in_memory(buf)) {
                buf->pos += (ssize_t) (NGX_SENDFILE_LIMIT - buf->file_pos);
                b->last = buf->pos;
            }

            buf->file_pos = NGX_SENDFILE_LIMIT;
            b->file_last = NGX_SENDFILE_LIMIT;

            cl->buf = b;

        } បើមិនដូច្នេះ {
            cl->buf = buf;
            in = in->next;
        }

#បើមិនដូច្នេះ
        cl->buf = in->buf;
        in = in->next;

#endif

        cl->next = NULL;
        *ll = cl;
        ll = &cl->next;
    }

    ត្រឡប់ NGX_OK;
}


static ngx_int_t
ngx_output_chain_align_file_buf(ngx_output_chain_ctx_t *ctx, off_t bsize)
{
    size_t      size;
    ngx_buf_t  *in;

    in = ctx->in->buf;

    ប្រសិនបើ (in->file == NULL || !in->file->directio) {
        ត្រឡប់ NGX_DECLINED;
    }

    ctx->directio = 1;

    size = (size_t) (in->file_pos - (in->file_pos & ~(ctx->alignment - 1)));

    ប្រសិនបើ (size == 0) {

        ប្រសិនបើ (bsize >= (off_t) ctx->bufs.size) {
            ត្រឡប់ NGX_DECLINED;
        }

        size = (size_t) bsize;

    } បើមិនដូច្នេះ {
        size = (size_t) ctx->alignment - size;

        ប្រសិនបើ ((off_t) size > bsize) {
            size = (size_t) bsize;
        }
    }

    ctx->buf = ngx_create_temp_buf(ctx->pool, size);
    ប្រសិនបើ (ctx->buf == NULL) {
        ត្រឡប់ NGX_ERROR;
    }

    /*
     * we do not set ctx->buf->tag, because we do not want
     * to reuse the buf via ctx->free list
     */

#ប្រសិនបើ (NGX_HAVE_ALIGNED_DIRECTIO)
    ctx->unaligned = 1;
#endif

    ត្រឡប់ NGX_OK;
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

    ប្រសិនបើ (in->last_in_chain) {

        ប្រសិនបើ (bsize < (off_t) size) {

            /*
             * allocate a small temp buf for a small last buf
             * or its small last part
             */

            size = (size_t) bsize;
            recycled = 0;

        } បើមិនដូច្នេះ ប្រសិនបើ (!ctx->directio
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
    ប្រសិនបើ (b == NULL) {
        ត្រឡប់ NGX_ERROR;
    }

    ប្រសិនបើ (ctx->directio) {

        /*
         * allocate block aligned to a disk sector size to enable
         * userland buffer direct usage conjunctly with directio
         */

        b->start = ngx_pmemalign(ctx->pool, size, (size_t) ctx->alignment);
        ប្រសិនបើ (b->start == NULL) {
            ត្រឡប់ NGX_ERROR;
        }

    } បើមិនដូច្នេះ {
        b->start = ngx_palloc(ctx->pool, size);
        ប្រសិនបើ (b->start == NULL) {
            ត្រឡប់ NGX_ERROR;
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

    ត្រឡប់ NGX_OK;
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

#ប្រសិនបើ (NGX_SENDFILE_LIMIT)

    ប្រសិនបើ (src->in_file && src->file_pos >= NGX_SENDFILE_LIMIT) {
        sendfile = 0;
    }

#endif

    ប្រសិនបើ (ngx_buf_in_memory(src)) {
        ngx_memcpy(dst->pos, src->pos, (size_t) size);
        src->pos += (size_t) size;
        dst->last += (size_t) size;

        ប្រសិនបើ (src->in_file) {

            ប្រសិនបើ (sendfile) {
                dst->in_file = 1;
                dst->file = src->file;
                dst->file_pos = src->file_pos;
                dst->file_last = src->file_pos + size;

            } បើមិនដូច្នេះ {
                dst->in_file = 0;
            }

            src->file_pos += size;

        } បើមិនដូច្នេះ {
            dst->in_file = 0;
        }

        ប្រសិនបើ (src->pos == src->last) {
            dst->flush = src->flush;
            dst->last_buf = src->last_buf;
            dst->last_in_chain = src->last_in_chain;

        } បើមិនដូច្នេះ {
            dst->flush = 0;
            dst->last_buf = 0;
            dst->last_in_chain = 0;
        }

    } បើមិនដូច្នេះ {

#ប្រសិនបើ (NGX_HAVE_ALIGNED_DIRECTIO)

        ប្រសិនបើ (ctx->unaligned) {
            ប្រសិនបើ (ngx_directio_off(src->file->fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, ctx->pool->log, ngx_errno,
                              ngx_directio_off_n " \"%s\" failed",
                              src->file->name.data);
            }
        }

#endif

#ប្រសិនបើ (NGX_HAVE_FILE_AIO)
        ប្រសិនបើ (ctx->aio_handler) {
            n = ngx_file_aio_read(src->file, dst->pos, (size_t) size,
                                  src->file_pos, ctx->pool);
            ប្រសិនបើ (n == NGX_AGAIN) {
                ctx->aio_handler(ctx, src->file);
                ត្រឡប់ NGX_AGAIN;
            }

        } បើមិនដូច្នេះ
#endif
#ប្រសិនបើ (NGX_THREADS)
        ប្រសិនបើ (ctx->thread_handler) {
            src->file->thread_task = ctx->thread_task;
            src->file->thread_handler = ctx->thread_handler;
            src->file->thread_ctx = ctx->filter_ctx;

            n = ngx_thread_read(src->file, dst->pos, (size_t) size,
                                src->file_pos, ctx->pool);
            ប្រសិនបើ (n == NGX_AGAIN) {
                ctx->thread_task = src->file->thread_task;
                ត្រឡប់ NGX_AGAIN;
            }

        } បើមិនដូច្នេះ
#endif
        {
            n = ngx_read_file(src->file, dst->pos, (size_t) size,
                              src->file_pos);
        }

#ប្រសិនបើ (NGX_HAVE_ALIGNED_DIRECTIO)

        ប្រសិនបើ (ctx->unaligned) {
            ngx_err_t  err;

            err = ngx_errno;

            ប្រសិនបើ (ngx_directio_on(src->file->fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, ctx->pool->log, ngx_errno,
                              ngx_directio_on_n " \"%s\" failed",
                              src->file->name.data);
            }

            ngx_set_errno(err);

            ctx->unaligned = 0;
        }

#endif

        ប្រសិនបើ (n == NGX_ERROR) {
            ត្រឡប់ (ngx_int_t) n;
        }

        ប្រសិនបើ (n != size) {
            ngx_log_error(NGX_LOG_ALERT, ctx->pool->log, 0,
                          ngx_read_file_n " read only %z of %O from \"%s\"",
                          n, size, src->file->name.data);
            ត្រឡប់ NGX_ERROR;
        }

        dst->last += n;

        ប្រសិនបើ (sendfile) {
            dst->in_file = 1;
            dst->file = src->file;
            dst->file_pos = src->file_pos;
            dst->file_last = src->file_pos + n;

        } បើមិនដូច្នេះ {
            dst->in_file = 0;
        }

        src->file_pos += n;

        ប្រសិនបើ (src->file_pos == src->file_last) {
            dst->flush = src->flush;
            dst->last_buf = src->last_buf;
            dst->last_in_chain = src->last_in_chain;

        } បើមិនដូច្នេះ {
            dst->flush = 0;
            dst->last_buf = 0;
            dst->last_in_chain = 0;
        }
    }

    ត្រឡប់ NGX_OK;
}


ngx_int_t
ngx_chain_writer(void *data, ngx_chain_t *in)
{
    ngx_chain_writer_ctx_t *ctx = data;

    off_t              size;
    ngx_chain_t       *cl, *ln, *chain;
    ngx_connection_t  *c;

    c = ctx->connection;

    សម្រាប់ (size = 0; in; in = in->next) {

        ប្រសិនបើ (ngx_buf_size(in->buf) == 0 && !ngx_buf_special(in->buf)) {

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

        ប្រសិនបើ (ngx_buf_size(in->buf) < 0) {

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

            ត្រឡប់ NGX_ERROR;
        }

        size += ngx_buf_size(in->buf);

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "chain writer buf fl:%d s:%uO",
                       in->buf->flush, ngx_buf_size(in->buf));

        cl = ngx_alloc_chain_link(ctx->pool);
        ប្រសិនបើ (cl == NULL) {
            ត្រឡប់ NGX_ERROR;
        }

        cl->buf = in->buf;
        cl->next = NULL;
        *ctx->last = cl;
        ctx->last = &cl->next;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                   "chain writer in: %p", ctx->out);

    សម្រាប់ (cl = ctx->out; cl; cl = cl->next) {

        ប្រសិនបើ (ngx_buf_size(cl->buf) == 0 && !ngx_buf_special(cl->buf)) {

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

        ប្រសិនបើ (ngx_buf_size(cl->buf) < 0) {

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

            ត្រឡប់ NGX_ERROR;
        }

        size += ngx_buf_size(cl->buf);
    }

    ប្រសិនបើ (size == 0 && !c->buffered) {
        ត្រឡប់ NGX_OK;
    }

    chain = c->send_chain(c, ctx->out, ctx->limit);

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                   "chain writer out: %p", chain);

    ប្រសិនបើ (chain == NGX_CHAIN_ERROR) {
        ត្រឡប់ NGX_ERROR;
    }

    ប្រសិនបើ (chain && c->write->ready) {
        ngx_post_event(c->write, &ngx_posted_next_events);
    }

    សម្រាប់ (cl = ctx->out; cl && cl != chain; /* void */) {
        ln = cl;
        cl = cl->next;
        ngx_free_chain(ctx->pool, ln);
    }

    ctx->out = chain;

    ប្រសិនបើ (ctx->out == NULL) {
        ctx->last = &ctx->out;

        ប្រសិនបើ (!c->buffered) {
            ត្រឡប់ NGX_OK;
        }
    }

    ត្រឡប់ NGX_AGAIN;
}
