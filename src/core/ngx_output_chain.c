
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>
#अन्तर्भाव <ngx_event.h>


#यदि 0
#परिभाषय NGX_SENDFILE_सीमा  4096
#यदि_अन्तः

/*
 * When DIRECTIO is enabled FreeBSD, Solaris, and MacOSX read directly
 * to an application memory from a device if parameters are aligned
 * to device sector boundary (512 bytes).  They fallback to usual read
 * operation if the parameters are not aligned.
 * Linux allows DIRECTIO only if the parameters are aligned to a filesystem
 * sector boundary, otherwise it returns EINVAL.  The sector size is
 * usually 512 bytes, however, on XFS it may be 4096 bytes.
 */

#परिभाषय NGX_NONE            1


स्थिर ngx_inline ngx_int_t
    ngx_output_chain_as_is(ngx_output_chain_ctx_t *ctx, ngx_buf_t *buf);
स्थिर ngx_int_t ngx_output_chain_सङ्कलय_copy(ngx_pool_t *pool,
    ngx_chain_t **chain, ngx_chain_t *अन्तर्);
स्थिर ngx_int_t ngx_output_chain_align_file_buf(ngx_output_chain_ctx_t *ctx,
    off_t bsize);
स्थिर ngx_int_t ngx_output_chain_get_buf(ngx_output_chain_ctx_t *ctx,
    off_t bsize);
स्थिर ngx_int_t ngx_output_chain_copy_buf(ngx_output_chain_ctx_t *ctx);


ngx_int_t
ngx_output_chain(ngx_output_chain_ctx_t *ctx, ngx_chain_t *अन्तर्)
{
    off_t         bsize;
    ngx_int_t     rc, चरमः;
    ngx_chain_t  *cl, *बहिर्गमनम्, **चरमः_बहिर्गमनम्;

    यदि (ctx->अन्तर् == रिक्त && ctx->busy == रिक्त
#यदि (NGX_HAVE_FILE_AIO || NGX_तन्तवः)
        && !ctx->aio
#यदि_अन्तः
       )
    {
        /*
         * the short path for the case when the ctx->in and ctx->busy chains
         * are empty, the incoming chain is empty too or has the single buf
         * that does not require the copy
         */

        यदि (अन्तर् == रिक्त) {
            प्रत्यागम ctx->output_filter(ctx->filter_ctx, अन्तर्);
        }

        यदि (अन्तर्->अग्र == रिक्त
#यदि (NGX_SENDFILE_सीमा)
            && !(अन्तर्->buf->अन्तर्_file && अन्तर्->buf->file_चरमः > NGX_SENDFILE_सीमा)
#यदि_अन्तः
            && ngx_output_chain_as_is(ctx, अन्तर्->buf))
        {
            प्रत्यागम ctx->output_filter(ctx->filter_ctx, अन्तर्);
        }
    }

    /* add the incoming buf to the chain ctx->in */

    यदि (अन्तर्) {
        यदि (ngx_output_chain_सङ्कलय_copy(ctx->pool, &ctx->अन्तर्, अन्तर्) == NGX_ERROR) {
            प्रत्यागम NGX_ERROR;
        }
    }

    बहिर्गमनम् = रिक्त;
    चरमः_बहिर्गमनम् = &बहिर्गमनम्;
    चरमः = NGX_NONE;

    प्रति ( ;; ) {

#यदि (NGX_HAVE_FILE_AIO || NGX_तन्तवः)
        यदि (ctx->aio) {
            प्रत्यागम NGX_AGAIN;
        }
#यदि_अन्तः

        यावत् (ctx->अन्तर्) {

            /*
             * cycle while there are the ctx->in bufs
             * and there are the free output bufs to copy in
             */

            bsize = ngx_buf_आकार(ctx->अन्तर्->buf);

            यदि (bsize == 0 && !ngx_buf_special(ctx->अन्तर्->buf)) {

                ngx_log_error(NGX_LOG_ALERT, ctx->pool->लघुगणकः, 0,
                              "zero size buf in output "
                              "t:%d r:%d f:%d %p %p-%p %p %O-%O",
                              ctx->अन्तर्->buf->temporary,
                              ctx->अन्तर्->buf->recycled,
                              ctx->अन्तर्->buf->अन्तर्_file,
                              ctx->अन्तर्->buf->start,
                              ctx->अन्तर्->buf->pos,
                              ctx->अन्तर्->buf->चरमः,
                              ctx->अन्तर्->buf->file,
                              ctx->अन्तर्->buf->file_pos,
                              ctx->अन्तर्->buf->file_चरमः);

                ngx_debug_point();

                cl = ctx->अन्तर्;
                ctx->अन्तर् = cl->अग्र;

                ngx_free_chain(ctx->pool, cl);

                अनुवर्त;
            }

            यदि (bsize < 0) {

                ngx_log_error(NGX_LOG_ALERT, ctx->pool->लघुगणकः, 0,
                              "negative size buf in output "
                              "t:%d r:%d f:%d %p %p-%p %p %O-%O",
                              ctx->अन्तर्->buf->temporary,
                              ctx->अन्तर्->buf->recycled,
                              ctx->अन्तर्->buf->अन्तर्_file,
                              ctx->अन्तर्->buf->start,
                              ctx->अन्तर्->buf->pos,
                              ctx->अन्तर्->buf->चरमः,
                              ctx->अन्तर्->buf->file,
                              ctx->अन्तर्->buf->file_pos,
                              ctx->अन्तर्->buf->file_चरमः);

                ngx_debug_point();

                प्रत्यागम NGX_ERROR;
            }

            यदि (ngx_output_chain_as_is(ctx, ctx->अन्तर्->buf)) {

                /* move the chain link to the output chain */

                cl = ctx->अन्तर्;
                ctx->अन्तर् = cl->अग्र;

                *चरमः_बहिर्गमनम् = cl;
                चरमः_बहिर्गमनम् = &cl->अग्र;
                cl->अग्र = रिक्त;

                अनुवर्त;
            }

            यदि (ctx->buf == रिक्त) {

                rc = ngx_output_chain_align_file_buf(ctx, bsize);

                यदि (rc == NGX_ERROR) {
                    प्रत्यागम NGX_ERROR;
                }

                यदि (rc != NGX_OK) {

                    यदि (ctx->मुक्त) {

                        /* get the free buf */

                        cl = ctx->मुक्त;
                        ctx->buf = cl->buf;
                        ctx->मुक्त = cl->अग्र;

                        ngx_free_chain(ctx->pool, cl);

                    } अन्यथा यदि (बहिर्गमनम् || ctx->allocated == ctx->bufs.अङ्कः) {

                        भङ्ग;

                    } अन्यथा यदि (ngx_output_chain_get_buf(ctx, bsize) != NGX_OK) {
                        प्रत्यागम NGX_ERROR;
                    }
                }
            }

            rc = ngx_output_chain_copy_buf(ctx);

            यदि (rc == NGX_ERROR) {
                प्रत्यागम rc;
            }

            यदि (rc == NGX_AGAIN) {
                यदि (बहिर्गमनम्) {
                    भङ्ग;
                }

                प्रत्यागम rc;
            }

            /* delete the completed buf from the ctx->in chain */

            यदि (ngx_buf_आकार(ctx->अन्तर्->buf) == 0) {
                cl = ctx->अन्तर्;
                ctx->अन्तर् = cl->अग्र;

                ngx_free_chain(ctx->pool, cl);
            }

            cl = ngx_alloc_chain_link(ctx->pool);
            यदि (cl == रिक्त) {
                प्रत्यागम NGX_ERROR;
            }

            cl->buf = ctx->buf;
            cl->अग्र = रिक्त;
            *चरमः_बहिर्गमनम् = cl;
            चरमः_बहिर्गमनम् = &cl->अग्र;
            ctx->buf = रिक्त;
        }

        यदि (बहिर्गमनम् == रिक्त && चरमः != NGX_NONE) {

            यदि (ctx->अन्तर्) {
                प्रत्यागम NGX_AGAIN;
            }

            प्रत्यागम चरमः;
        }

        चरमः = ctx->output_filter(ctx->filter_ctx, बहिर्गमनम्);

        यदि (चरमः == NGX_ERROR || चरमः == NGX_समाप्तम्) {
            प्रत्यागम चरमः;
        }

        ngx_chain_update_chains(ctx->pool, &ctx->मुक्त, &ctx->busy, &बहिर्गमनम्,
                                ctx->चिह्नकम्);
        चरमः_बहिर्गमनम् = &बहिर्गमनम्;
    }
}


स्थिर ngx_inline ngx_int_t
ngx_output_chain_as_is(ngx_output_chain_ctx_t *ctx, ngx_buf_t *buf)
{
    ngx_uint_t  sendfile;

    यदि (ngx_buf_special(buf)) {
        प्रत्यागम 1;
    }

#यदि (NGX_तन्तवः)
    यदि (buf->अन्तर्_file) {
        buf->file->thread_handler = ctx->thread_handler;
        buf->file->thread_ctx = ctx->filter_ctx;
    }
#यदि_अन्तः

    sendfile = ctx->sendfile;

#यदि (NGX_SENDFILE_सीमा)

    यदि (buf->अन्तर्_file && buf->file_pos >= NGX_SENDFILE_सीमा) {
        sendfile = 0;
    }

#यदि_अन्तः

#यदि !(NGX_HAVE_SENDFILE_NODISKIO)

    /*
     * With DIRECTIO, disable sendfile() unless sendfile(SF_NOCACHE)
     * is available.
     */

    यदि (buf->अन्तर्_file && buf->file->directio) {
        sendfile = 0;
    }

#यदि_अन्तः

    यदि (!sendfile) {

        यदि (!ngx_buf_अन्तर्_स्मृतिः(buf)) {
            प्रत्यागम 0;
        }

        buf->अन्तर्_file = 0;
    }

    यदि (ctx->need_अन्तर्_स्मृतिः && !ngx_buf_अन्तर्_स्मृतिः(buf)) {
        प्रत्यागम 0;
    }

    यदि (ctx->need_अन्तर्_अस्थायी && (buf->स्मृतिः || buf->mmap)) {
        प्रत्यागम 0;
    }

    प्रत्यागम 1;
}


स्थिर ngx_int_t
ngx_output_chain_सङ्कलय_copy(ngx_pool_t *pool, ngx_chain_t **chain,
    ngx_chain_t *अन्तर्)
{
    ngx_chain_t  *cl, **ll;
#यदि (NGX_SENDFILE_सीमा)
    ngx_buf_t    *b, *buf;
#यदि_अन्तः

    ll = chain;

    प्रति (cl = *chain; cl; cl = cl->अग्र) {
        ll = &cl->अग्र;
    }

    यावत् (अन्तर्) {

        cl = ngx_alloc_chain_link(pool);
        यदि (cl == रिक्त) {
            प्रत्यागम NGX_ERROR;
        }

#यदि (NGX_SENDFILE_सीमा)

        buf = अन्तर्->buf;

        यदि (buf->अन्तर्_file
            && buf->file_pos < NGX_SENDFILE_सीमा
            && buf->file_चरमः > NGX_SENDFILE_सीमा)
        {
            /* split a file buf on two bufs by the sendfile limit */

            b = ngx_calloc_buf(pool);
            यदि (b == रिक्त) {
                प्रत्यागम NGX_ERROR;
            }

            ngx_memcpy(b, buf, परिमाण(ngx_buf_t));

            यदि (ngx_buf_अन्तर्_स्मृतिः(buf)) {
                buf->pos += (ssize_t) (NGX_SENDFILE_सीमा - buf->file_pos);
                b->चरमः = buf->pos;
            }

            buf->file_pos = NGX_SENDFILE_सीमा;
            b->file_चरमः = NGX_SENDFILE_सीमा;

            cl->buf = b;

        } अन्यथा {
            cl->buf = buf;
            अन्तर् = अन्तर्->अग्र;
        }

#अन्यथा
        cl->buf = अन्तर्->buf;
        अन्तर् = अन्तर्->अग्र;

#यदि_अन्तः

        cl->अग्र = रिक्त;
        *ll = cl;
        ll = &cl->अग्र;
    }

    प्रत्यागम NGX_OK;
}


स्थिर ngx_int_t
ngx_output_chain_align_file_buf(ngx_output_chain_ctx_t *ctx, off_t bsize)
{
    आकार_t      आकार;
    ngx_buf_t  *अन्तर्;

    अन्तर् = ctx->अन्तर्->buf;

    यदि (अन्तर्->file == रिक्त || !अन्तर्->file->directio) {
        प्रत्यागम NGX_DECLINED;
    }

    ctx->directio = 1;

    आकार = (आकार_t) (अन्तर्->file_pos - (अन्तर्->file_pos & ~(ctx->alignment - 1)));

    यदि (आकार == 0) {

        यदि (bsize >= (off_t) ctx->bufs.आकार) {
            प्रत्यागम NGX_DECLINED;
        }

        आकार = (आकार_t) bsize;

    } अन्यथा {
        आकार = (आकार_t) ctx->alignment - आकार;

        यदि ((off_t) आकार > bsize) {
            आकार = (आकार_t) bsize;
        }
    }

    ctx->buf = ngx_create_अस्थायी_buf(ctx->pool, आकार);
    यदि (ctx->buf == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

    /*
     * we do not set ctx->buf->tag, because we do not want
     * to reuse the buf via ctx->free list
     */

#यदि (NGX_HAVE_ALIGNED_DIRECTIO)
    ctx->unaligned = 1;
#यदि_अन्तः

    प्रत्यागम NGX_OK;
}


स्थिर ngx_int_t
ngx_output_chain_get_buf(ngx_output_chain_ctx_t *ctx, off_t bsize)
{
    आकार_t       आकार;
    ngx_buf_t   *b, *अन्तर्;
    ngx_uint_t   recycled;

    अन्तर् = ctx->अन्तर्->buf;
    आकार = ctx->bufs.आकार;
    recycled = 1;

    यदि (अन्तर्->चरमः_अन्तर्_chain) {

        यदि (bsize < (off_t) आकार) {

            /*
             * allocate a small temp buf for a small last buf
             * or its small last part
             */

            आकार = (आकार_t) bsize;
            recycled = 0;

        } अन्यथा यदि (!ctx->directio
                   && ctx->bufs.अङ्कः == 1
                   && (bsize < (off_t) (आकार + आकार / 4)))
        {
            /*
             * allocate a temp buf that equals to a last buf,
             * if there is no directio, the last buf size is lesser
             * than 1.25 of bufs.size and the temp buf is single
             */

            आकार = (आकार_t) bsize;
            recycled = 0;
        }
    }

    b = ngx_calloc_buf(ctx->pool);
    यदि (b == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

    यदि (ctx->directio) {

        /*
         * allocate block aligned to a disk sector size to enable
         * userland buffer direct usage conjunctly with directio
         */

        b->start = ngx_pmemalign(ctx->pool, आकार, (आकार_t) ctx->alignment);
        यदि (b->start == रिक्त) {
            प्रत्यागम NGX_ERROR;
        }

    } अन्यथा {
        b->start = ngx_palloc(ctx->pool, आकार);
        यदि (b->start == रिक्त) {
            प्रत्यागम NGX_ERROR;
        }
    }

    b->pos = b->start;
    b->चरमः = b->start;
    b->अन्तः = b->चरमः + आकार;
    b->temporary = 1;
    b->चिह्नकम् = ctx->चिह्नकम्;
    b->recycled = recycled;

    ctx->buf = b;
    ctx->allocated++;

    प्रत्यागम NGX_OK;
}


स्थिर ngx_int_t
ngx_output_chain_copy_buf(ngx_output_chain_ctx_t *ctx)
{
    off_t        आकार;
    ssize_t      n;
    ngx_buf_t   *src, *dst;
    ngx_uint_t   sendfile;

    src = ctx->अन्तर्->buf;
    dst = ctx->buf;

    आकार = ngx_buf_आकार(src);
    आकार = ngx_min(आकार, dst->अन्तः - dst->pos);

    sendfile = ctx->sendfile && !ctx->directio;

#यदि (NGX_SENDFILE_सीमा)

    यदि (src->अन्तर्_file && src->file_pos >= NGX_SENDFILE_सीमा) {
        sendfile = 0;
    }

#यदि_अन्तः

    यदि (ngx_buf_अन्तर्_स्मृतिः(src)) {
        ngx_memcpy(dst->pos, src->pos, (आकार_t) आकार);
        src->pos += (आकार_t) आकार;
        dst->चरमः += (आकार_t) आकार;

        यदि (src->अन्तर्_file) {

            यदि (sendfile) {
                dst->अन्तर्_file = 1;
                dst->file = src->file;
                dst->file_pos = src->file_pos;
                dst->file_चरमः = src->file_pos + आकार;

            } अन्यथा {
                dst->अन्तर्_file = 0;
            }

            src->file_pos += आकार;

        } अन्यथा {
            dst->अन्तर्_file = 0;
        }

        यदि (src->pos == src->चरमः) {
            dst->flush = src->flush;
            dst->चरमः_buf = src->चरमः_buf;
            dst->चरमः_अन्तर्_chain = src->चरमः_अन्तर्_chain;

        } अन्यथा {
            dst->flush = 0;
            dst->चरमः_buf = 0;
            dst->चरमः_अन्तर्_chain = 0;
        }

    } अन्यथा {

#यदि (NGX_HAVE_ALIGNED_DIRECTIO)

        यदि (ctx->unaligned) {
            यदि (ngx_directio_off(src->file->fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, ctx->pool->लघुगणकः, ngx_errno,
                              ngx_directio_off_n " \"%s\" failed",
                              src->file->name.दत्त);
            }
        }

#यदि_अन्तः

#यदि (NGX_HAVE_FILE_AIO)
        यदि (ctx->aio_handler) {
            n = ngx_file_aio_read(src->file, dst->pos, (आकार_t) आकार,
                                  src->file_pos, ctx->pool);
            यदि (n == NGX_AGAIN) {
                ctx->aio_handler(ctx, src->file);
                प्रत्यागम NGX_AGAIN;
            }

        } अन्यथा
#यदि_अन्तः
#यदि (NGX_तन्तवः)
        यदि (ctx->thread_handler) {
            src->file->thread_task = ctx->thread_task;
            src->file->thread_handler = ctx->thread_handler;
            src->file->thread_ctx = ctx->filter_ctx;

            n = ngx_thread_read(src->file, dst->pos, (आकार_t) आकार,
                                src->file_pos, ctx->pool);
            यदि (n == NGX_AGAIN) {
                ctx->thread_task = src->file->thread_task;
                प्रत्यागम NGX_AGAIN;
            }

        } अन्यथा
#यदि_अन्तः
        {
            n = ngx_read_file(src->file, dst->pos, (आकार_t) आकार,
                              src->file_pos);
        }

#यदि (NGX_HAVE_ALIGNED_DIRECTIO)

        यदि (ctx->unaligned) {
            ngx_err_t  err;

            err = ngx_errno;

            यदि (ngx_directio_on(src->file->fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, ctx->pool->लघुगणकः, ngx_errno,
                              ngx_directio_on_n " \"%s\" failed",
                              src->file->name.दत्त);
            }

            ngx_समुच्चयः_errno(err);

            ctx->unaligned = 0;
        }

#यदि_अन्तः

        यदि (n == NGX_ERROR) {
            प्रत्यागम (ngx_int_t) n;
        }

        यदि (n != आकार) {
            ngx_log_error(NGX_LOG_ALERT, ctx->pool->लघुगणकः, 0,
                          ngx_read_file_n " read only %z of %O from \"%s\"",
                          n, आकार, src->file->name.दत्त);
            प्रत्यागम NGX_ERROR;
        }

        dst->चरमः += n;

        यदि (sendfile) {
            dst->अन्तर्_file = 1;
            dst->file = src->file;
            dst->file_pos = src->file_pos;
            dst->file_चरमः = src->file_pos + n;

        } अन्यथा {
            dst->अन्तर्_file = 0;
        }

        src->file_pos += n;

        यदि (src->file_pos == src->file_चरमः) {
            dst->flush = src->flush;
            dst->चरमः_buf = src->चरमः_buf;
            dst->चरमः_अन्तर्_chain = src->चरमः_अन्तर्_chain;

        } अन्यथा {
            dst->flush = 0;
            dst->चरमः_buf = 0;
            dst->चरमः_अन्तर्_chain = 0;
        }
    }

    प्रत्यागम NGX_OK;
}


ngx_int_t
ngx_chain_writer(शून्य *दत्त, ngx_chain_t *अन्तर्)
{
    ngx_chain_writer_ctx_t *ctx = दत्त;

    off_t              आकार;
    ngx_chain_t       *cl, *ln, *chain;
    ngx_connection_t  *c;

    c = ctx->connection;

    प्रति (आकार = 0; अन्तर्; अन्तर् = अन्तर्->अग्र) {

        यदि (ngx_buf_आकार(अन्तर्->buf) == 0 && !ngx_buf_special(अन्तर्->buf)) {

            ngx_log_error(NGX_LOG_ALERT, ctx->pool->लघुगणकः, 0,
                          "zero size buf in chain writer "
                          "t:%d r:%d f:%d %p %p-%p %p %O-%O",
                          अन्तर्->buf->temporary,
                          अन्तर्->buf->recycled,
                          अन्तर्->buf->अन्तर्_file,
                          अन्तर्->buf->start,
                          अन्तर्->buf->pos,
                          अन्तर्->buf->चरमः,
                          अन्तर्->buf->file,
                          अन्तर्->buf->file_pos,
                          अन्तर्->buf->file_चरमः);

            ngx_debug_point();

            अनुवर्त;
        }

        यदि (ngx_buf_आकार(अन्तर्->buf) < 0) {

            ngx_log_error(NGX_LOG_ALERT, ctx->pool->लघुगणकः, 0,
                          "negative size buf in chain writer "
                          "t:%d r:%d f:%d %p %p-%p %p %O-%O",
                          अन्तर्->buf->temporary,
                          अन्तर्->buf->recycled,
                          अन्तर्->buf->अन्तर्_file,
                          अन्तर्->buf->start,
                          अन्तर्->buf->pos,
                          अन्तर्->buf->चरमः,
                          अन्तर्->buf->file,
                          अन्तर्->buf->file_pos,
                          अन्तर्->buf->file_चरमः);

            ngx_debug_point();

            प्रत्यागम NGX_ERROR;
        }

        आकार += ngx_buf_आकार(अन्तर्->buf);

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, c->लघुगणकः, 0,
                       "chain writer buf fl:%d s:%uO",
                       अन्तर्->buf->flush, ngx_buf_आकार(अन्तर्->buf));

        cl = ngx_alloc_chain_link(ctx->pool);
        यदि (cl == रिक्त) {
            प्रत्यागम NGX_ERROR;
        }

        cl->buf = अन्तर्->buf;
        cl->अग्र = रिक्त;
        *ctx->चरमः = cl;
        ctx->चरमः = &cl->अग्र;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->लघुगणकः, 0,
                   "chain writer in: %p", ctx->बहिर्गमनम्);

    प्रति (cl = ctx->बहिर्गमनम्; cl; cl = cl->अग्र) {

        यदि (ngx_buf_आकार(cl->buf) == 0 && !ngx_buf_special(cl->buf)) {

            ngx_log_error(NGX_LOG_ALERT, ctx->pool->लघुगणकः, 0,
                          "zero size buf in chain writer "
                          "t:%d r:%d f:%d %p %p-%p %p %O-%O",
                          cl->buf->temporary,
                          cl->buf->recycled,
                          cl->buf->अन्तर्_file,
                          cl->buf->start,
                          cl->buf->pos,
                          cl->buf->चरमः,
                          cl->buf->file,
                          cl->buf->file_pos,
                          cl->buf->file_चरमः);

            ngx_debug_point();

            अनुवर्त;
        }

        यदि (ngx_buf_आकार(cl->buf) < 0) {

            ngx_log_error(NGX_LOG_ALERT, ctx->pool->लघुगणकः, 0,
                          "negative size buf in chain writer "
                          "t:%d r:%d f:%d %p %p-%p %p %O-%O",
                          cl->buf->temporary,
                          cl->buf->recycled,
                          cl->buf->अन्तर्_file,
                          cl->buf->start,
                          cl->buf->pos,
                          cl->buf->चरमः,
                          cl->buf->file,
                          cl->buf->file_pos,
                          cl->buf->file_चरमः);

            ngx_debug_point();

            प्रत्यागम NGX_ERROR;
        }

        आकार += ngx_buf_आकार(cl->buf);
    }

    यदि (आकार == 0 && !c->buffered) {
        प्रत्यागम NGX_OK;
    }

    chain = c->send_chain(c, ctx->बहिर्गमनम्, ctx->सीमा);

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->लघुगणकः, 0,
                   "chain writer out: %p", chain);

    यदि (chain == NGX_CHAIN_ERROR) {
        प्रत्यागम NGX_ERROR;
    }

    यदि (chain && c->लिख->ready) {
        ngx_post_event(c->लिख, &ngx_posted_अग्र_events);
    }

    प्रति (cl = ctx->बहिर्गमनम्; cl && cl != chain; /* void */) {
        ln = cl;
        cl = cl->अग्र;
        ngx_free_chain(ctx->pool, ln);
    }

    ctx->बहिर्गमनम् = chain;

    यदि (ctx->बहिर्गमनम् == रिक्त) {
        ctx->चरमः = &ctx->बहिर्गमनम्;

        यदि (!c->buffered) {
            प्रत्यागम NGX_OK;
        }
    }

    प्रत्यागम NGX_AGAIN;
}
