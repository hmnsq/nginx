
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#اگر_تعریف_نشده _NGX_BUF_H_INCLUDED_
#تعریف_کن _NGX_BUF_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


تعريفنوع فراغ *            ngx_buf_برچسب_t;

تعريفنوع بنية ngx_buf_s  ngx_buf_t;

بنية ngx_buf_s {
    u_char          *pos;
    u_char          *آخرین;
    off_t            file_pos;
    off_t            file_آخرین;

    u_char          *شروع;         /* start of buffer */
    u_char          *انتها;           /* end of buffer */
    ngx_buf_برچسب_t    برچسب;
    ngx_file_t      *file;
    ngx_buf_t       *shadow;


    /* the buf's content could be changed */
    بیعلامت         temporary:1;

    /*
     * the buf's content is in a memory cache or in a read only memory
     * and must not be changed
     */
    بیعلامت         حافظه:1;

    /* the buf's content is mmap()ed and must not be changed */
    بیعلامت         mmap:1;

    بیعلامت         recycled:1;
    بیعلامت         درون_file:1;
    بیعلامت         flush:1;
    بیعلامت         sync:1;
    بیعلامت         آخرین_buf:1;
    بیعلامت         آخرین_درون_chain:1;

    بیعلامت         آخرین_shadow:1;
    بیعلامت         موقت_file:1;

    /* STUB */ صحيح   رقم;
};


بنية ngx_chain_s {
    ngx_buf_t    *buf;
    ngx_chain_t  *بعدی;
};


تعريفنوع بنية {
    ngx_int_t    رقم;
    اندازه_t       اندازه;
} ngx_bufs_t;


تعريفنوع بنية ngx_output_chain_ctx_s  ngx_output_chain_ctx_t;

تعريفنوع ngx_int_t (*ngx_output_chain_filter_pt)(فراغ *ctx, ngx_chain_t *درون);

تعريفنوع فراغ (*ngx_output_chain_aio_pt)(ngx_output_chain_ctx_t *ctx,
    ngx_file_t *file);

بنية ngx_output_chain_ctx_s {
    ngx_buf_t                   *buf;
    ngx_chain_t                 *درون;
    ngx_chain_t                 *حرر;
    ngx_chain_t                 *busy;

    بیعلامت                     sendfile:1;
    بیعلامت                     directio:1;
    بیعلامت                     unaligned:1;
    بیعلامت                     need_درون_حافظه:1;
    بیعلامت                     need_درون_موقت:1;
    بیعلامت                     aio:1;

#إذا (NGX_HAVE_FILE_AIO || NGX_COMPAT)
    ngx_output_chain_aio_pt      aio_handler;
#پایان_اگر

#إذا (NGX_نخها || NGX_COMPAT)
    ngx_int_t                  (*thread_handler)(ngx_thread_task_t *task,
                                                 ngx_file_t *file);
    ngx_thread_task_t           *thread_task;
#پایان_اگر

    off_t                        alignment;

    ngx_pool_t                  *pool;
    ngx_int_t                    allocated;
    ngx_bufs_t                   bufs;
    ngx_buf_برچسب_t                برچسب;

    ngx_output_chain_filter_pt   output_filter;
    فراغ                        *filter_ctx;
};


تعريفنوع بنية {
    ngx_chain_t                 *بیرون;
    ngx_chain_t                **آخرین;
    ngx_connection_t            *connection;
    ngx_pool_t                  *pool;
    off_t                        حد;
} ngx_chain_writer_ctx_t;


#تعریف_کن NGX_CHAIN_ERROR     (ngx_chain_t *) NGX_ERROR


#تعریف_کن ngx_buf_درون_حافظه(b)       ((b)->temporary || (b)->حافظه || (b)->mmap)
#تعریف_کن ngx_buf_درون_حافظه_only(b)  (ngx_buf_درون_حافظه(b) && !(b)->درون_file)

#تعریف_کن ngx_buf_special(b)                                                   \
    (((b)->flush || (b)->آخرین_buf || (b)->sync)                              \
     && !ngx_buf_درون_حافظه(b) && !(b)->درون_file)

#تعریف_کن ngx_buf_sync_only(b)                                                 \
    ((b)->sync && !ngx_buf_درون_حافظه(b)                                      \
     && !(b)->درون_file && !(b)->flush && !(b)->آخرین_buf)

#تعریف_کن ngx_buf_اندازه(b)                                                      \
    (ngx_buf_درون_حافظه(b) ? (off_t) ((b)->آخرین - (b)->pos):                  \
                            ((b)->file_آخرین - (b)->file_pos))

ngx_buf_t *ngx_create_موقت_buf(ngx_pool_t *pool, اندازه_t اندازه);
ngx_chain_t *ngx_create_chain_از_bufs(ngx_pool_t *pool, ngx_bufs_t *bufs);


#تعریف_کن ngx_alloc_buf(pool)  ngx_palloc(pool, حجم(ngx_buf_t))
#تعریف_کن ngx_calloc_buf(pool) ngx_pcalloc(pool, حجم(ngx_buf_t))

ngx_chain_t *ngx_alloc_chain_link(ngx_pool_t *pool);
#تعریف_کن ngx_free_chain(pool, cl)                                             \
    (cl)->بعدی = (pool)->chain;                                              \
    (pool)->chain = (cl)



ngx_int_t ngx_output_chain(ngx_output_chain_ctx_t *ctx, ngx_chain_t *درون);
ngx_int_t ngx_chain_writer(فراغ *ctx, ngx_chain_t *درون);

ngx_int_t ngx_chain_جمع_کن_copy(ngx_pool_t *pool, ngx_chain_t **chain,
    ngx_chain_t *درون);
ngx_chain_t *ngx_chain_get_free_buf(ngx_pool_t *p, ngx_chain_t **حرر);
فراغ ngx_chain_update_chains(ngx_pool_t *p, ngx_chain_t **حرر,
    ngx_chain_t **busy, ngx_chain_t **بیرون, ngx_buf_برچسب_t برچسب);

off_t ngx_chain_coalesce_file(ngx_chain_t **درون, off_t حد);

ngx_chain_t *ngx_chain_update_فرستاده(ngx_chain_t *درون, off_t فرستاده);

#پایان_اگر /* _NGX_BUF_H_INCLUDED_ */
