
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>
#అన్తర్భావ <ngx_event.h>


#ఒకవేళ 0
#నిర్వచించు NGX_SENDFILE_హద్దు  4096
#అయితే_ముగింపు

/*
 * When DIRECTIO is enabled FreeBSD, Solaris, and MacOSX read directly
 * to an application memory from a device if parameters are aligned
 * to device sector boundary (512 bytes).  They fallback to usual read
 * operation if the parameters are not aligned.
 * Linux allows DIRECTIO only if the parameters are aligned to a filesystem
 * sector boundary, otherwise it returns EINVAL.  The sector size is
 * usually 512 bytes, however, on XFS it may be 4096 bytes.
 */

#నిర్వచించు NGX_NONE            1


స్థిర ngx_inline ngx_int_t
    ngx_output_chain_as_is(ngx_output_chain_ctx_t *ctx, ngx_buf_t *buf);
స్థిర ngx_int_t ngx_output_chain_కూడు_copy(ngx_pool_t *pool,
    ngx_chain_t **chain, ngx_chain_t *లోపల);
స్థిర ngx_int_t ngx_output_chain_align_file_buf(ngx_output_chain_ctx_t *ctx,
    off_t bsize);
స్థిర ngx_int_t ngx_output_chain_get_buf(ngx_output_chain_ctx_t *ctx,
    off_t bsize);
స్థిర ngx_int_t ngx_output_chain_copy_buf(ngx_output_chain_ctx_t *ctx);


ngx_int_t
ngx_output_chain(ngx_output_chain_ctx_t *ctx, ngx_chain_t *లోపల)
{
    off_t         bsize;
    ngx_int_t     rc, చివరి;
    ngx_chain_t  *cl, *బహిర్గతం, **చివరి_బహిర్గతం;

    ఒకవేళ (ctx->లోపల == ఖాళీ && ctx->busy == ఖాళీ
#ఒకవేళ (NGX_HAVE_FILE_AIO || NGX_దారాలు)
        && !ctx->aio
#అయితే_ముగింపు
       )
    {
        /*
         * the short path for the case when the ctx->in and ctx->busy chains
         * are empty, the incoming chain is empty too or has the single buf
         * that does not require the copy
         */

        ఒకవేళ (లోపల == ఖాళీ) {
            తిరిగి ctx->output_filter(ctx->filter_ctx, లోపల);
        }

        ఒకవేళ (లోపల->తదుపరి == ఖాళీ
#ఒకవేళ (NGX_SENDFILE_హద్దు)
            && !(లోపల->buf->లోపల_file && లోపల->buf->file_చివరి > NGX_SENDFILE_హద్దు)
#అయితే_ముగింపు
            && ngx_output_chain_as_is(ctx, లోపల->buf))
        {
            తిరిగి ctx->output_filter(ctx->filter_ctx, లోపల);
        }
    }

    /* add the incoming buf to the chain ctx->in */

    ఒకవేళ (లోపల) {
        ఒకవేళ (ngx_output_chain_కూడు_copy(ctx->pool, &ctx->లోపల, లోపల) == NGX_ERROR) {
            తిరిగి NGX_ERROR;
        }
    }

    బహిర్గతం = ఖాళీ;
    చివరి_బహిర్గతం = &బహిర్గతం;
    చివరి = NGX_NONE;

    కొరకు ( ;; ) {

#ఒకవేళ (NGX_HAVE_FILE_AIO || NGX_దారాలు)
        ఒకవేళ (ctx->aio) {
            తిరిగి NGX_AGAIN;
        }
#అయితే_ముగింపు

        అయితే (ctx->లోపల) {

            /*
             * cycle while there are the ctx->in bufs
             * and there are the free output bufs to copy in
             */

            bsize = ngx_buf_కొలత(ctx->లోపల->buf);

            ఒకవేళ (bsize == 0 && !ngx_buf_special(ctx->లోపల->buf)) {

                ngx_log_error(NGX_LOG_ALERT, ctx->pool->సంవర్గమానం, 0,
                              "zero size buf in output "
                              "t:%d r:%d f:%d %p %p-%p %p %O-%O",
                              ctx->లోపల->buf->temporary,
                              ctx->లోపల->buf->recycled,
                              ctx->లోపల->buf->లోపల_file,
                              ctx->లోపల->buf->ప్రారంభం,
                              ctx->లోపల->buf->pos,
                              ctx->లోపల->buf->చివరి,
                              ctx->లోపల->buf->file,
                              ctx->లోపల->buf->file_pos,
                              ctx->లోపల->buf->file_చివరి);

                ngx_debug_బిందువు();

                cl = ctx->లోపల;
                ctx->లోపల = cl->తదుపరి;

                ngx_free_chain(ctx->pool, cl);

                కొనసాగు;
            }

            ఒకవేళ (bsize < 0) {

                ngx_log_error(NGX_LOG_ALERT, ctx->pool->సంవర్గమానం, 0,
                              "negative size buf in output "
                              "t:%d r:%d f:%d %p %p-%p %p %O-%O",
                              ctx->లోపల->buf->temporary,
                              ctx->లోపల->buf->recycled,
                              ctx->లోపల->buf->లోపల_file,
                              ctx->లోపల->buf->ప్రారంభం,
                              ctx->లోపల->buf->pos,
                              ctx->లోపల->buf->చివరి,
                              ctx->లోపల->buf->file,
                              ctx->లోపల->buf->file_pos,
                              ctx->లోపల->buf->file_చివరి);

                ngx_debug_బిందువు();

                తిరిగి NGX_ERROR;
            }

            ఒకవేళ (ngx_output_chain_as_is(ctx, ctx->లోపల->buf)) {

                /* move the chain link to the output chain */

                cl = ctx->లోపల;
                ctx->లోపల = cl->తదుపరి;

                *చివరి_బహిర్గతం = cl;
                చివరి_బహిర్గతం = &cl->తదుపరి;
                cl->తదుపరి = ఖాళీ;

                కొనసాగు;
            }

            ఒకవేళ (ctx->buf == ఖాళీ) {

                rc = ngx_output_chain_align_file_buf(ctx, bsize);

                ఒకవేళ (rc == NGX_ERROR) {
                    తిరిగి NGX_ERROR;
                }

                ఒకవేళ (rc != NGX_OK) {

                    ఒకవేళ (ctx->విడుదల) {

                        /* get the free buf */

                        cl = ctx->విడుదల;
                        ctx->buf = cl->buf;
                        ctx->విడుదల = cl->తదుపరి;

                        ngx_free_chain(ctx->pool, cl);

                    } లేకపోతే ఒకవేళ (బహిర్గతం || ctx->allocated == ctx->bufs.అంకె) {

                        ఆపు;

                    } లేకపోతే ఒకవేళ (ngx_output_chain_get_buf(ctx, bsize) != NGX_OK) {
                        తిరిగి NGX_ERROR;
                    }
                }
            }

            rc = ngx_output_chain_copy_buf(ctx);

            ఒకవేళ (rc == NGX_ERROR) {
                తిరిగి rc;
            }

            ఒకవేళ (rc == NGX_AGAIN) {
                ఒకవేళ (బహిర్గతం) {
                    ఆపు;
                }

                తిరిగి rc;
            }

            /* delete the completed buf from the ctx->in chain */

            ఒకవేళ (ngx_buf_కొలత(ctx->లోపల->buf) == 0) {
                cl = ctx->లోపల;
                ctx->లోపల = cl->తదుపరి;

                ngx_free_chain(ctx->pool, cl);
            }

            cl = ngx_alloc_chain_link(ctx->pool);
            ఒకవేళ (cl == ఖాళీ) {
                తిరిగి NGX_ERROR;
            }

            cl->buf = ctx->buf;
            cl->తదుపరి = ఖాళీ;
            *చివరి_బహిర్గతం = cl;
            చివరి_బహిర్గతం = &cl->తదుపరి;
            ctx->buf = ఖాళీ;
        }

        ఒకవేళ (బహిర్గతం == ఖాళీ && చివరి != NGX_NONE) {

            ఒకవేళ (ctx->లోపల) {
                తిరిగి NGX_AGAIN;
            }

            తిరిగి చివరి;
        }

        చివరి = ctx->output_filter(ctx->filter_ctx, బహిర్గతం);

        ఒకవేళ (చివరి == NGX_ERROR || చివరి == NGX_పూర్తి) {
            తిరిగి చివరి;
        }

        ngx_chain_update_chains(ctx->pool, &ctx->విడుదల, &ctx->busy, &బహిర్గతం,
                                ctx->ట్యాగ్);
        చివరి_బహిర్గతం = &బహిర్గతం;
    }
}


స్థిర ngx_inline ngx_int_t
ngx_output_chain_as_is(ngx_output_chain_ctx_t *ctx, ngx_buf_t *buf)
{
    ngx_uint_t  sendfile;

    ఒకవేళ (ngx_buf_special(buf)) {
        తిరిగి 1;
    }

#ఒకవేళ (NGX_దారాలు)
    ఒకవేళ (buf->లోపల_file) {
        buf->file->thread_handler = ctx->thread_handler;
        buf->file->thread_ctx = ctx->filter_ctx;
    }
#అయితే_ముగింపు

    sendfile = ctx->sendfile;

#ఒకవేళ (NGX_SENDFILE_హద్దు)

    ఒకవేళ (buf->లోపల_file && buf->file_pos >= NGX_SENDFILE_హద్దు) {
        sendfile = 0;
    }

#అయితే_ముగింపు

#ఒకవేళ !(NGX_HAVE_SENDFILE_NODISKIO)

    /*
     * With DIRECTIO, disable sendfile() unless sendfile(SF_NOCACHE)
     * is available.
     */

    ఒకవేళ (buf->లోపల_file && buf->file->directio) {
        sendfile = 0;
    }

#అయితే_ముగింపు

    ఒకవేళ (!sendfile) {

        ఒకవేళ (!ngx_buf_లోపల_జ్ఞాపకం(buf)) {
            తిరిగి 0;
        }

        buf->లోపల_file = 0;
    }

    ఒకవేళ (ctx->need_లోపల_జ్ఞాపకం && !ngx_buf_లోపల_జ్ఞాపకం(buf)) {
        తిరిగి 0;
    }

    ఒకవేళ (ctx->need_లోపల_తాత్కాలిక && (buf->జ్ఞాపకం || buf->mmap)) {
        తిరిగి 0;
    }

    తిరిగి 1;
}


స్థిర ngx_int_t
ngx_output_chain_కూడు_copy(ngx_pool_t *pool, ngx_chain_t **chain,
    ngx_chain_t *లోపల)
{
    ngx_chain_t  *cl, **ll;
#ఒకవేళ (NGX_SENDFILE_హద్దు)
    ngx_buf_t    *b, *buf;
#అయితే_ముగింపు

    ll = chain;

    కొరకు (cl = *chain; cl; cl = cl->తదుపరి) {
        ll = &cl->తదుపరి;
    }

    అయితే (లోపల) {

        cl = ngx_alloc_chain_link(pool);
        ఒకవేళ (cl == ఖాళీ) {
            తిరిగి NGX_ERROR;
        }

#ఒకవేళ (NGX_SENDFILE_హద్దు)

        buf = లోపల->buf;

        ఒకవేళ (buf->లోపల_file
            && buf->file_pos < NGX_SENDFILE_హద్దు
            && buf->file_చివరి > NGX_SENDFILE_హద్దు)
        {
            /* split a file buf on two bufs by the sendfile limit */

            b = ngx_calloc_buf(pool);
            ఒకవేళ (b == ఖాళీ) {
                తిరిగి NGX_ERROR;
            }

            ngx_memcpy(b, buf, పరిమాణం(ngx_buf_t));

            ఒకవేళ (ngx_buf_లోపల_జ్ఞాపకం(buf)) {
                buf->pos += (ssize_t) (NGX_SENDFILE_హద్దు - buf->file_pos);
                b->చివరి = buf->pos;
            }

            buf->file_pos = NGX_SENDFILE_హద్దు;
            b->file_చివరి = NGX_SENDFILE_హద్దు;

            cl->buf = b;

        } లేకపోతే {
            cl->buf = buf;
            లోపల = లోపల->తదుపరి;
        }

#లేకపోతే
        cl->buf = లోపల->buf;
        లోపల = లోపల->తదుపరి;

#అయితే_ముగింపు

        cl->తదుపరి = ఖాళీ;
        *ll = cl;
        ll = &cl->తదుపరి;
    }

    తిరిగి NGX_OK;
}


స్థిర ngx_int_t
ngx_output_chain_align_file_buf(ngx_output_chain_ctx_t *ctx, off_t bsize)
{
    కొలత_t      కొలత;
    ngx_buf_t  *లోపల;

    లోపల = ctx->లోపల->buf;

    ఒకవేళ (లోపల->file == ఖాళీ || !లోపల->file->directio) {
        తిరిగి NGX_DECLINED;
    }

    ctx->directio = 1;

    కొలత = (కొలత_t) (లోపల->file_pos - (లోపల->file_pos & ~(ctx->alignment - 1)));

    ఒకవేళ (కొలత == 0) {

        ఒకవేళ (bsize >= (off_t) ctx->bufs.కొలత) {
            తిరిగి NGX_DECLINED;
        }

        కొలత = (కొలత_t) bsize;

    } లేకపోతే {
        కొలత = (కొలత_t) ctx->alignment - కొలత;

        ఒకవేళ ((off_t) కొలత > bsize) {
            కొలత = (కొలత_t) bsize;
        }
    }

    ctx->buf = ngx_create_తాత్కాలిక_buf(ctx->pool, కొలత);
    ఒకవేళ (ctx->buf == ఖాళీ) {
        తిరిగి NGX_ERROR;
    }

    /*
     * we do not set ctx->buf->tag, because we do not want
     * to reuse the buf via ctx->free list
     */

#ఒకవేళ (NGX_HAVE_ALIGNED_DIRECTIO)
    ctx->unaligned = 1;
#అయితే_ముగింపు

    తిరిగి NGX_OK;
}


స్థిర ngx_int_t
ngx_output_chain_get_buf(ngx_output_chain_ctx_t *ctx, off_t bsize)
{
    కొలత_t       కొలత;
    ngx_buf_t   *b, *లోపల;
    ngx_uint_t   recycled;

    లోపల = ctx->లోపల->buf;
    కొలత = ctx->bufs.కొలత;
    recycled = 1;

    ఒకవేళ (లోపల->చివరి_లోపల_chain) {

        ఒకవేళ (bsize < (off_t) కొలత) {

            /*
             * allocate a small temp buf for a small last buf
             * or its small last part
             */

            కొలత = (కొలత_t) bsize;
            recycled = 0;

        } లేకపోతే ఒకవేళ (!ctx->directio
                   && ctx->bufs.అంకె == 1
                   && (bsize < (off_t) (కొలత + కొలత / 4)))
        {
            /*
             * allocate a temp buf that equals to a last buf,
             * if there is no directio, the last buf size is lesser
             * than 1.25 of bufs.size and the temp buf is single
             */

            కొలత = (కొలత_t) bsize;
            recycled = 0;
        }
    }

    b = ngx_calloc_buf(ctx->pool);
    ఒకవేళ (b == ఖాళీ) {
        తిరిగి NGX_ERROR;
    }

    ఒకవేళ (ctx->directio) {

        /*
         * allocate block aligned to a disk sector size to enable
         * userland buffer direct usage conjunctly with directio
         */

        b->ప్రారంభం = ngx_pmemalign(ctx->pool, కొలత, (కొలత_t) ctx->alignment);
        ఒకవేళ (b->ప్రారంభం == ఖాళీ) {
            తిరిగి NGX_ERROR;
        }

    } లేకపోతే {
        b->ప్రారంభం = ngx_palloc(ctx->pool, కొలత);
        ఒకవేళ (b->ప్రారంభం == ఖాళీ) {
            తిరిగి NGX_ERROR;
        }
    }

    b->pos = b->ప్రారంభం;
    b->చివరి = b->ప్రారంభం;
    b->చివర = b->చివరి + కొలత;
    b->temporary = 1;
    b->ట్యాగ్ = ctx->ట్యాగ్;
    b->recycled = recycled;

    ctx->buf = b;
    ctx->allocated++;

    తిరిగి NGX_OK;
}


స్థిర ngx_int_t
ngx_output_chain_copy_buf(ngx_output_chain_ctx_t *ctx)
{
    off_t        కొలత;
    ssize_t      n;
    ngx_buf_t   *src, *dst;
    ngx_uint_t   sendfile;

    src = ctx->లోపల->buf;
    dst = ctx->buf;

    కొలత = ngx_buf_కొలత(src);
    కొలత = ngx_min(కొలత, dst->చివర - dst->pos);

    sendfile = ctx->sendfile && !ctx->directio;

#ఒకవేళ (NGX_SENDFILE_హద్దు)

    ఒకవేళ (src->లోపల_file && src->file_pos >= NGX_SENDFILE_హద్దు) {
        sendfile = 0;
    }

#అయితే_ముగింపు

    ఒకవేళ (ngx_buf_లోపల_జ్ఞాపకం(src)) {
        ngx_memcpy(dst->pos, src->pos, (కొలత_t) కొలత);
        src->pos += (కొలత_t) కొలత;
        dst->చివరి += (కొలత_t) కొలత;

        ఒకవేళ (src->లోపల_file) {

            ఒకవేళ (sendfile) {
                dst->లోపల_file = 1;
                dst->file = src->file;
                dst->file_pos = src->file_pos;
                dst->file_చివరి = src->file_pos + కొలత;

            } లేకపోతే {
                dst->లోపల_file = 0;
            }

            src->file_pos += కొలత;

        } లేకపోతే {
            dst->లోపల_file = 0;
        }

        ఒకవేళ (src->pos == src->చివరి) {
            dst->flush = src->flush;
            dst->చివరి_buf = src->చివరి_buf;
            dst->చివరి_లోపల_chain = src->చివరి_లోపల_chain;

        } లేకపోతే {
            dst->flush = 0;
            dst->చివరి_buf = 0;
            dst->చివరి_లోపల_chain = 0;
        }

    } లేకపోతే {

#ఒకవేళ (NGX_HAVE_ALIGNED_DIRECTIO)

        ఒకవేళ (ctx->unaligned) {
            ఒకవేళ (ngx_directio_off(src->file->fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, ctx->pool->సంవర్గమానం, ngx_errno,
                              ngx_directio_off_n " \"%s\" failed",
                              src->file->name.దత్తాంశం);
            }
        }

#అయితే_ముగింపు

#ఒకవేళ (NGX_HAVE_FILE_AIO)
        ఒకవేళ (ctx->aio_handler) {
            n = ngx_file_aio_read(src->file, dst->pos, (కొలత_t) కొలత,
                                  src->file_pos, ctx->pool);
            ఒకవేళ (n == NGX_AGAIN) {
                ctx->aio_handler(ctx, src->file);
                తిరిగి NGX_AGAIN;
            }

        } లేకపోతే
#అయితే_ముగింపు
#ఒకవేళ (NGX_దారాలు)
        ఒకవేళ (ctx->thread_handler) {
            src->file->thread_task = ctx->thread_task;
            src->file->thread_handler = ctx->thread_handler;
            src->file->thread_ctx = ctx->filter_ctx;

            n = ngx_thread_read(src->file, dst->pos, (కొలత_t) కొలత,
                                src->file_pos, ctx->pool);
            ఒకవేళ (n == NGX_AGAIN) {
                ctx->thread_task = src->file->thread_task;
                తిరిగి NGX_AGAIN;
            }

        } లేకపోతే
#అయితే_ముగింపు
        {
            n = ngx_read_file(src->file, dst->pos, (కొలత_t) కొలత,
                              src->file_pos);
        }

#ఒకవేళ (NGX_HAVE_ALIGNED_DIRECTIO)

        ఒకవేళ (ctx->unaligned) {
            ngx_err_t  err;

            err = ngx_errno;

            ఒకవేళ (ngx_directio_on(src->file->fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_ALERT, ctx->pool->సంవర్గమానం, ngx_errno,
                              ngx_directio_on_n " \"%s\" failed",
                              src->file->name.దత్తాంశం);
            }

            ngx_సమితి_errno(err);

            ctx->unaligned = 0;
        }

#అయితే_ముగింపు

        ఒకవేళ (n == NGX_ERROR) {
            తిరిగి (ngx_int_t) n;
        }

        ఒకవేళ (n != కొలత) {
            ngx_log_error(NGX_LOG_ALERT, ctx->pool->సంవర్గమానం, 0,
                          ngx_read_file_n " read only %z of %O from \"%s\"",
                          n, కొలత, src->file->name.దత్తాంశం);
            తిరిగి NGX_ERROR;
        }

        dst->చివరి += n;

        ఒకవేళ (sendfile) {
            dst->లోపల_file = 1;
            dst->file = src->file;
            dst->file_pos = src->file_pos;
            dst->file_చివరి = src->file_pos + n;

        } లేకపోతే {
            dst->లోపల_file = 0;
        }

        src->file_pos += n;

        ఒకవేళ (src->file_pos == src->file_చివరి) {
            dst->flush = src->flush;
            dst->చివరి_buf = src->చివరి_buf;
            dst->చివరి_లోపల_chain = src->చివరి_లోపల_chain;

        } లేకపోతే {
            dst->flush = 0;
            dst->చివరి_buf = 0;
            dst->చివరి_లోపల_chain = 0;
        }
    }

    తిరిగి NGX_OK;
}


ngx_int_t
ngx_chain_writer(శూన్య *దత్తాంశం, ngx_chain_t *లోపల)
{
    ngx_chain_writer_ctx_t *ctx = దత్తాంశం;

    off_t              కొలత;
    ngx_chain_t       *cl, *ln, *chain;
    ngx_connection_t  *c;

    c = ctx->connection;

    కొరకు (కొలత = 0; లోపల; లోపల = లోపల->తదుపరి) {

        ఒకవేళ (ngx_buf_కొలత(లోపల->buf) == 0 && !ngx_buf_special(లోపల->buf)) {

            ngx_log_error(NGX_LOG_ALERT, ctx->pool->సంవర్గమానం, 0,
                          "zero size buf in chain writer "
                          "t:%d r:%d f:%d %p %p-%p %p %O-%O",
                          లోపల->buf->temporary,
                          లోపల->buf->recycled,
                          లోపల->buf->లోపల_file,
                          లోపల->buf->ప్రారంభం,
                          లోపల->buf->pos,
                          లోపల->buf->చివరి,
                          లోపల->buf->file,
                          లోపల->buf->file_pos,
                          లోపల->buf->file_చివరి);

            ngx_debug_బిందువు();

            కొనసాగు;
        }

        ఒకవేళ (ngx_buf_కొలత(లోపల->buf) < 0) {

            ngx_log_error(NGX_LOG_ALERT, ctx->pool->సంవర్గమానం, 0,
                          "negative size buf in chain writer "
                          "t:%d r:%d f:%d %p %p-%p %p %O-%O",
                          లోపల->buf->temporary,
                          లోపల->buf->recycled,
                          లోపల->buf->లోపల_file,
                          లోపల->buf->ప్రారంభం,
                          లోపల->buf->pos,
                          లోపల->buf->చివరి,
                          లోపల->buf->file,
                          లోపల->buf->file_pos,
                          లోపల->buf->file_చివరి);

            ngx_debug_బిందువు();

            తిరిగి NGX_ERROR;
        }

        కొలత += ngx_buf_కొలత(లోపల->buf);

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, c->సంవర్గమానం, 0,
                       "chain writer buf fl:%d s:%uO",
                       లోపల->buf->flush, ngx_buf_కొలత(లోపల->buf));

        cl = ngx_alloc_chain_link(ctx->pool);
        ఒకవేళ (cl == ఖాళీ) {
            తిరిగి NGX_ERROR;
        }

        cl->buf = లోపల->buf;
        cl->తదుపరి = ఖాళీ;
        *ctx->చివరి = cl;
        ctx->చివరి = &cl->తదుపరి;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->సంవర్గమానం, 0,
                   "chain writer in: %p", ctx->బహిర్గతం);

    కొరకు (cl = ctx->బహిర్గతం; cl; cl = cl->తదుపరి) {

        ఒకవేళ (ngx_buf_కొలత(cl->buf) == 0 && !ngx_buf_special(cl->buf)) {

            ngx_log_error(NGX_LOG_ALERT, ctx->pool->సంవర్గమానం, 0,
                          "zero size buf in chain writer "
                          "t:%d r:%d f:%d %p %p-%p %p %O-%O",
                          cl->buf->temporary,
                          cl->buf->recycled,
                          cl->buf->లోపల_file,
                          cl->buf->ప్రారంభం,
                          cl->buf->pos,
                          cl->buf->చివరి,
                          cl->buf->file,
                          cl->buf->file_pos,
                          cl->buf->file_చివరి);

            ngx_debug_బిందువు();

            కొనసాగు;
        }

        ఒకవేళ (ngx_buf_కొలత(cl->buf) < 0) {

            ngx_log_error(NGX_LOG_ALERT, ctx->pool->సంవర్గమానం, 0,
                          "negative size buf in chain writer "
                          "t:%d r:%d f:%d %p %p-%p %p %O-%O",
                          cl->buf->temporary,
                          cl->buf->recycled,
                          cl->buf->లోపల_file,
                          cl->buf->ప్రారంభం,
                          cl->buf->pos,
                          cl->buf->చివరి,
                          cl->buf->file,
                          cl->buf->file_pos,
                          cl->buf->file_చివరి);

            ngx_debug_బిందువు();

            తిరిగి NGX_ERROR;
        }

        కొలత += ngx_buf_కొలత(cl->buf);
    }

    ఒకవేళ (కొలత == 0 && !c->buffered) {
        తిరిగి NGX_OK;
    }

    chain = c->send_chain(c, ctx->బహిర్గతం, ctx->హద్దు);

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, c->సంవర్గమానం, 0,
                   "chain writer out: %p", chain);

    ఒకవేళ (chain == NGX_CHAIN_ERROR) {
        తిరిగి NGX_ERROR;
    }

    ఒకవేళ (chain && c->లిఖ->ready) {
        ngx_post_event(c->లిఖ, &ngx_posted_తదుపరి_events);
    }

    కొరకు (cl = ctx->బహిర్గతం; cl && cl != chain; /* void */) {
        ln = cl;
        cl = cl->తదుపరి;
        ngx_free_chain(ctx->pool, ln);
    }

    ctx->బహిర్గతం = chain;

    ఒకవేళ (ctx->బహిర్గతం == ఖాళీ) {
        ctx->చివరి = &ctx->బహిర్గతం;

        ఒకవేళ (!c->buffered) {
            తిరిగి NGX_OK;
        }
    }

    తిరిగి NGX_AGAIN;
}
