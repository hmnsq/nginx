
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_event.h>


#གལ་སྲིད 0
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

    གལ་སྲིད (ctx->in == NULL && ctx->busy == NULL
#གལ་སྲིད (NGX_HAVE_FILE_AIO || NGX_THREADS)
        && !ctx->aio
#endif
       )
    {
        /*
         * the short path for the case when the ctx->in and ctx->busy chains
         * are empty, the incoming chain is empty too or has the single buf
         * that does not require the copy
         */

        གལ་སྲིད (in == NULL) {
            ལོག ctx->output_filter(ctx->filter_ctx, in);
        }

        གལ་སྲིད (in->next == NULL
#གལ་སྲིད (NGX_SENDFILE_LIMIT)
            && !(in->buf->in_file && in->buf->file_last > NGX_SENDFILE_LIMIT)
#endif
            && ngx_output_chain_as_is(ctx, in->buf))
        {
            ལོག ctx->output_filter(ctx->filter_ctx, in);
        }
    }

    /* add the incoming buf to the chain ctx->in */

    གལ་སྲིད (in) {
        གལ་སྲིད (ngx_output_chain_add_copy(ctx->pool, &ctx->in, in) == NGX_ERROR) {
            ལོག NGX_ERROR;
        }
    }

    out = NULL;
    last_out = &out;
    last = NGX_NONE;

    དོན་ལུ ( ;; ) {

#གལ་སྲིད (NGX_HAVE_FILE_AIO || NGX_THREADS)
        གལ་སྲིད (ctx->aio) {
            ལོག NGX_AGAIN;
        }
#endif

        སྐབས (ctx->in) {

            /*
             * cycle while there are the ctx->in bufs
             * and there are the free output bufs to copy in
             */

            bsize = ngx_buf_size(ctx->in->buf);

            གལ་སྲིད (bsize == 0 && !ngx_buf_special(ctx->in->buf)) {

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

            གལ་སྲིད (bsize < 0) {

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

                ལོག NGX_ERROR;
            }

            གལ་སྲིད (ngx_output_chain_as_is(ctx, ctx->in->buf)) {

                /* move the chain link to the output chain */

                cl = ctx->in;
                ctx->in = cl->next;

                *last_out = cl;
                last_out = &cl->next;
                cl->next = NULL;

                continue;
            }

            གལ་སྲིད (ctx->buf == NULL) {

                rc = ngx_output_chain_align_file_buf(ctx, bsize);

                གལ་སྲིད (rc == NGX_ERROR) {
                    ལོག NGX_ERROR;
                }

                གལ་སྲིད (rc != NGX_OK) {

                    གལ་སྲིད (ctx->free) {

                        /* get the free buf */

                        cl = ctx->free;
                        ctx->buf = cl->buf;
                        ctx->free = cl->next;

                        ngx_free_chain(ctx->pool, cl);

                    } དེ་མིན གལ་སྲིད (out || ctx->allocated == ctx->bufs.num) {

                        bha_nga;

                    } དེ་མིན གལ་སྲིད (ngx_output_chain_get_buf(ctx, bsize) != NGX_OK) {
                        ལོག NGX_ERROR;
                    }
                }
            }

            rc = ngx_output_chain_copy_buf(ctx);

            གལ་སྲིད (rc == NGX_ERROR) {
                ལོག rc;
            }

            གལ་སྲིད (rc == NGX_AGAIN) {
                གལ་སྲིད (out) {
                    bha_nga;
                }

                ལོག rc;
            }

            /* delete the completed buf from the ctx->in chain */

            གལ་སྲིད (ngx_buf_size(ctx->in->buf) == 0) {
                cl = ctx->in;
                ctx->in = cl->next;

                ngx_free_chain(ctx->pool, cl);
            }

            cl = ngx_alloc_chain_link(ctx->pool);
            གལ་སྲིད (cl == NULL) {
                ལོག NGX_ERROR;
            }

            cl->buf = ctx->buf;
            cl->next = NULL;
            *last_out = cl;
            last_out = &cl->next;
            ctx->buf = NULL;
        }

        གལ་སྲིད (out == NULL && last != NGX_NONE) {

            གལ་སྲིད (ctx->in) {
                ལོག NGX_AGAIN;
            }

            ལོག last;
        }

        last = ctx->output_filter(ctx->filter_ctx, out);

        གལ་སྲིད (last == NGX_ERROR || last == NGX_DONE) {
            ལོག last;
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

    གལ་སྲིད (ngx_buf_special(buf)) {
        ལོག 1;
    }

#གལ་སྲིད (NGX_THREADS)
    གལ་སྲིད (buf->in_file) {
        buf->file->thread_handler = ctx->thread_handler;
        buf->file->thread_ctx = ctx->filter_ctx;
    }
#endif

    sendfile = ctx->sendfile;

#གལ་སྲིད (NGX_SENDFILE_LIMIT)

    གལ་སྲིད (buf->in_file && buf->file_pos >= NGX_SENDFILE_LIMIT) {
        sendfile = 0;
    }

#endif

#གལ་སྲིད !(NGX_HAVE_SENDFILE_NODISKIO)

    /*
     * With DIRECTIO, disable sendfile() unless sendfile(SF_NOCACHE)
     * is available.
     */

    གལ་སྲིད (buf->in_file && buf->file->directio) {
        sendfile = 0;
    }

#endif

    གལ་སྲིད (!sendfile) {

        གལ་སྲིད (!ngx_buf_in_memory(buf)) {
            ལོག 0;
        }

        buf->in_file = 0;
    }

    གལ་སྲིད (ctx->need_in_memory && !ngx_buf_in_memory(buf)) {
        ལོག 0;
    }

    གལ་སྲིད (ctx->need_in_temp && (buf->memory || buf->mmap)) {
        ལོག 0;
    }

    ལོག 1;
}


static ngx_int_t
ngx_output_chain_add_copy(ngx_pool_t *pool, ngx_chain_t **chain,
    ngx_chain_t *in)
{
    ngx_chain_t  *cl, **ll;
#གལ་སྲིད (NGX_SENDFILE_LIMIT)
    ngx_buf_t    *b, *buf;
#endif

    ll = chain;

    དོན་ལུ (cl = *chain; cl; cl = cl->next) {
        ll = &cl->next;
    }

    སྐབས (in) {

        cl = ngx_alloc_chain_link(pool);
        གལ་སྲིད (cl == NULL) {
            ལོག NGX_ERROR;
        }

#གལ་སྲིད (NGX_SENDFILE_LIMIT)

        buf = in->buf;

        གལ་སྲིད (buf->in_file
            && buf->file_pos < NGX_SENDFILE_LIMIT
            && buf->file_last > NGX_SENDFILE_LIMIT)
        {
            /* split a file buf on two bufs by the sendfile limit */

            b = ngx_calloc_buf(pool);
            གལ་སྲིད (b == NULL) {
                ལོག NGX_ERROR;
            }

            ngx_memcpy(b, buf, parimaa_na(ngx_buf_t));

            གལ་སྲིད (ngx_buf_in_memory(buf)) {
                buf->pos += (ssize_t) (NGX_SENDFILE_LIMIT - buf->file_pos);
                b->last = buf->pos;
            }

            buf->file_pos = NGX_SENDFILE_LIMIT;
            b->file_last = NGX_SENDFILE_LIMIT;

            cl->buf = b;

        } དེ་མིན {
            cl->buf = buf;
            in = in->next;
        }

#དེ་མིན
        cl->buf = in->buf;
        in = in->next;

#endif

        cl->next = NULL;
        *ll = cl;
        ll = &cl->next;
    }

    ལོག NGX_OK;
}


static ngx_int_t
ngx_output_chain_align_file_buf(ngx_output_chain_ctx_t *ctx, off_t bsize)
{
    size_t      size;
    ngx_buf_t  *in;

    in = ctx->in->buf;

    གལ་སྲིད (in->file == NULL || !in->file->directio) {
        ལོག NGX_DECLINED;
    }

    ctx->directio = 1;

    size = (size_t) (in->file_pos - (in->file_pos & ~(ctx->alignment - 1)));

    གལ་སྲིད (size == 0) {

        གལ་སྲིད (bsize >= (off_t) ctx->bufs.size) {
            ལོག NGX_DECLINED;
        }

        size = (size_t) bsize;

    } དེ་མིན {
        size = (size_t) ctx->alignment - size;

        གལ་སྲིད ((off_t) size > bsize) {
            size = (size_t) bsize;
        }
    }

    ctx->buf = ngx_create_temp_buf(ctx->pool, size);
    གལ་སྲིད (ctx->buf == NULL) {
        ལོག NGX_ERROR;
    }

    /*
     * we do not set ctx->buf->tag, because we do not want
     * to reuse the buf via ctx->free list
     */

#གལ་སྲིད (NGX_HAVE_ALIGNED_DIRECTIO)
    ctx->unaligned = 1;
#endif

    ལོག NGX_OK;
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

    གལ་སྲིད (in->last_in_chain) {

        གལ་སྲིད (bsize < (off_t) size) {

            /*
             * allocate a small temp buf for a small last buf
             * or its small last part
             */

            size = (size_t) bsize;
            recycled = 0;

        } དེ་མིན གལ་སྲིད (!ctx->directio
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
    གལ་སྲིད (b == NULL) {
        ལོག NGX_ERROR;
    }

    གལ་སྲིད (ctx->directio) {

        /*
         * allocate block aligned to a disk sector size to enable
         * userland buffer direct usage conjunctly with directio
         */

        b->start = ngx_pmemalign(ctx->pool, size, (size_t) ctx->alignment);
        གལ་སྲིད (b->start == NULL) {
            ལོག NGX_ERROR;
        }

    } དེ་མིན {
        b->start = ngx_palloc(ctx->pool, size);
        གལ་སྲིད (b->start == NULL) {
            ལོག NGX_ERROR;
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

    ལོག NGX_OK;
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

#གལ་སྲིད (NGX_SENDFILE_LIMIT)

    གལ་སྲིད (src->in_file && src->file_pos >= NGX_SENDFILE_LIMIT) {
        sendfile = 0;
    }

#endif

    གལ་སྲིད (ngx_buf_in_memory(src)) {
        ngx_memcpy(dst->pos, src->pos, (size_t) size);
        src->pos += (size_t) size;
        dst->last += (size_t) size;

        གལ་སྲིད (src->in_file) {

            གལ་སྲིད (sendfile) {
                dst->in_file = 1;
                dst->file = src->file;
                dst->file_pos = src->file_pos;
                dst->file_last = src->file_pos + size;

            } དེ་མིན {
                dst->in_file = 0;
            }

            src->file_pos += size;

        } དེ་མིན {
            dst->in_file = 0;
        }

        གལ་སྲིད (src->pos == src->last) {
            dst->flush = src->flush;
            dst->last_buf = src->last_buf;
            dst->last_in_chain = src->last_in_chain;

        } དེ་མིན {
            dst->flush = 0;
            dst->last_buf = 0;
            dst->last_in_chain = 0;
        }

    } དེ་མིན {

#གལ་སྲིད (NGX_HAVE_ALIGNED_DIRECTIO)

        གལ་སྲིད (ctx->unaligned) {
            གལ་སྲིད (ngx_directio_off(src->file->fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, ctx->pool->log, ngx_errno,
                              ngx_directio_off_n " \"%s\" failed",
                              src->file->name.data);
            }
        }

#endif

#གལ་སྲིད (NGX_HAVE_FILE_AIO)
        གལ་སྲིད (ctx->aio_handler) {
            n = ngx_file_aio_read(src->file, dst->pos, (size_t) size,
                                  src->file_pos, ctx->pool);
            གལ་སྲིད (n == NGX_AGAIN) {
                ctx->aio_handler(ctx, src->file);
                ལོག NGX_AGAIN;
            }

        } དེ་མིན
#endif
#གལ་སྲིད (NGX_THREADS)
        གལ་སྲིད (ctx->thread_handler) {
            src->file->thread_task = ctx->thread_task;
            src->file->thread_handler = ctx->thread_handler;
            src->file->thread_ctx = ctx->filter_ctx;

            n = ngx_thread_read(src->file, dst->pos, (size_t) size,
                                src->file_pos, ctx->pool);
            གལ་སྲིད (n == NGX_AGAIN) {
                ctx->thread_task = src->file->thread_task;
                ལོག NGX_AGAIN;
            }

        } དེ་མིན
#endif
        {
            n = ngx_read_file(src->file, dst->pos, (size_t) size,
                              src->file_pos);
        }

#གལ་སྲིད (NGX_HAVE_ALIGNED_DIRECTIO)

        གལ་སྲིད (ctx->unaligned) {
            ngx_err_t  err;

            err = ngx_errno;

            གལ་སྲིད (ngx_directio_on(src->file->fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, ctx->pool->log, ngx_errno,
                              ngx_directio_on_n " \"%s\" failed",
                              src->file->name.data);
            }

            ngx_set_errno(err);

            ctx->unaligned = 0;
        }

#endif

        གལ་སྲིད (n == NGX_ERROR) {
            ལོག (ngx_int_t) n;
        }

        གལ་སྲིད (n != size) {
            ngx_log_error(NGX_LOG_ALERT, ctx->pool->log, 0,
                          ngx_read_file_n " read only %z of %O from \"%s\"",
                          n, size, src->file->name.data);
            ལོག NGX_ERROR;
        }

        dst->last += n;

        གལ་སྲིད (sendfile) {
            dst->in_file = 1;
            dst->file = src->file;
            dst->file_pos = src->file_pos;
            dst->file_last = src->file_pos + n;

        } དེ་མིན {
            dst->in_file = 0;
        }

        src->file_pos += n;

        གལ་སྲིད (src->file_pos == src->file_last) {
            dst->flush = src->flush;
            dst->last_buf = src->last_buf;
            dst->last_in_chain = src->last_in_chain;

        } དེ་མིན {
            dst->flush = 0;
            dst->last_buf = 0;
            dst->last_in_chain = 0;
        }
    }

    ལོག NGX_OK;
}


ngx_int_t
ngx_chain_writer(void *data, ngx_chain_t *in)
{
    ngx_chain_writer_ctx_t *ctx = data;

    off_t              size;
    ngx_chain_t       *cl, *ln, *chain;
    ngx_connection_t  *c;

    c = ctx->connection;

    དོན་ལུ (size = 0; in; in = in->next) {

        གལ་སྲིད (ngx_buf_size(in->buf) == 0 && !ngx_buf_special(in->buf)) {

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

        གལ་སྲིད (ngx_buf_size(in->buf) < 0) {

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

            ལོག NGX_ERROR;
        }

        size += ngx_buf_size(in->buf);

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, c->log, 0,
                       "chain writer buf fl:%d s:%uO",
                       in->buf->flush, ngx_buf_size(in->buf));

        cl = ngx_alloc_chain_link(ctx->pool);
        གལ་སྲིད (cl == NULL) {
            ལོག NGX_ERROR;
        }

        cl->buf = in->buf;
        cl->next = NULL;
        *ctx->last = cl;
        ctx->last = &cl->next;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                   "chain writer in: %p", ctx->out);

    དོན་ལུ (cl = ctx->out; cl; cl = cl->next) {

        གལ་སྲིད (ngx_buf_size(cl->buf) == 0 && !ngx_buf_special(cl->buf)) {

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

        གལ་སྲིད (ngx_buf_size(cl->buf) < 0) {

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

            ལོག NGX_ERROR;
        }

        size += ngx_buf_size(cl->buf);
    }

    གལ་སྲིད (size == 0 && !c->buffered) {
        ལོག NGX_OK;
    }

    chain = c->send_chain(c, ctx->out, ctx->limit);

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->log, 0,
                   "chain writer out: %p", chain);

    གལ་སྲིད (chain == NGX_CHAIN_ERROR) {
        ལོག NGX_ERROR;
    }

    གལ་སྲིད (chain && c->write->ready) {
        ngx_post_event(c->write, &ngx_posted_next_events);
    }

    དོན་ལུ (cl = ctx->out; cl && cl != chain; /* void */) {
        ln = cl;
        cl = cl->next;
        ngx_free_chain(ctx->pool, ln);
    }

    ctx->out = chain;

    གལ་སྲིད (ctx->out == NULL) {
        ctx->last = &ctx->out;

        གལ་སྲིད (!c->buffered) {
            ལོག NGX_OK;
        }
    }

    ལོག NGX_AGAIN;
}
