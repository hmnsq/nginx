
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#که_نه_تعریف_شوی _NGX_BUF_H_INCLUDED_
#تعریف_کړه _NGX_BUF_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


تعريفنوع فراغ *            ngx_buf_نښان_t;

تعريفنوع بنية ngx_buf_s  ngx_buf_t;

بنية ngx_buf_s {
    u_char          *pos;
    u_char          *وروستی;
    off_t            file_pos;
    off_t            file_وروستی;

    u_char          *پیل;         /* start of buffer */
    u_char          *پای_ټکی;           /* end of buffer */
    ngx_buf_نښان_t    نښان;
    ngx_file_t      *file;
    ngx_buf_t       *shadow;


    /* the buf's content could be changed */
    بینښه         temporary:1;

    /*
     * the buf's content is in a memory cache or in a read only memory
     * and must not be changed
     */
    بینښه         حافظه:1;

    /* the buf's content is mmap()ed and must not be changed */
    بینښه         mmap:1;

    بینښه         recycled:1;
    بینښه         دننه_file:1;
    بینښه         flush:1;
    بینښه         sync:1;
    بینښه         وروستی_buf:1;
    بینښه         وروستی_دننه_chain:1;

    بینښه         وروستی_shadow:1;
    بینښه         لنډمهاله_file:1;

    /* STUB */ صحيح   عدد;
};


بنية ngx_chain_s {
    ngx_buf_t    *buf;
    ngx_chain_t  *بل;
};


تعريفنوع بنية {
    ngx_int_t    عدد;
    اندازه_t       اندازه;
} ngx_bufs_t;


تعريفنوع بنية ngx_output_chain_ctx_s  ngx_output_chain_ctx_t;

تعريفنوع ngx_int_t (*ngx_output_chain_filter_pt)(فراغ *ctx, ngx_chain_t *دننه);

تعريفنوع فراغ (*ngx_output_chain_aio_pt)(ngx_output_chain_ctx_t *ctx,
    ngx_file_t *file);

بنية ngx_output_chain_ctx_s {
    ngx_buf_t                   *buf;
    ngx_chain_t                 *دننه;
    ngx_chain_t                 *حرر;
    ngx_chain_t                 *busy;

    بینښه                     sendfile:1;
    بینښه                     directio:1;
    بینښه                     unaligned:1;
    بینښه                     need_دننه_حافظه:1;
    بینښه                     need_دننه_لنډمهاله:1;
    بینښه                     aio:1;

#إذا (NGX_HAVE_FILE_AIO || NGX_COMPAT)
    ngx_output_chain_aio_pt      aio_handler;
#د_که_پای

#إذا (NGX_تارونه || NGX_COMPAT)
    ngx_int_t                  (*thread_handler)(ngx_thread_task_t *task,
                                                 ngx_file_t *file);
    ngx_thread_task_t           *thread_task;
#د_که_پای

    off_t                        alignment;

    ngx_pool_t                  *pool;
    ngx_int_t                    allocated;
    ngx_bufs_t                   bufs;
    ngx_buf_نښان_t                نښان;

    ngx_output_chain_filter_pt   output_filter;
    فراغ                        *filter_ctx;
};


تعريفنوع بنية {
    ngx_chain_t                 *بهر;
    ngx_chain_t                **وروستی;
    ngx_connection_t            *connection;
    ngx_pool_t                  *pool;
    off_t                        حد;
} ngx_chain_writer_ctx_t;


#تعریف_کړه NGX_CHAIN_ERROR     (ngx_chain_t *) NGX_ERROR


#تعریف_کړه ngx_buf_دننه_حافظه(b)       ((b)->temporary || (b)->حافظه || (b)->mmap)
#تعریف_کړه ngx_buf_دننه_حافظه_only(b)  (ngx_buf_دننه_حافظه(b) && !(b)->دننه_file)

#تعریف_کړه ngx_buf_special(b)                                                   \
    (((b)->flush || (b)->وروستی_buf || (b)->sync)                              \
     && !ngx_buf_دننه_حافظه(b) && !(b)->دننه_file)

#تعریف_کړه ngx_buf_sync_only(b)                                                 \
    ((b)->sync && !ngx_buf_دننه_حافظه(b)                                      \
     && !(b)->دننه_file && !(b)->flush && !(b)->وروستی_buf)

#تعریف_کړه ngx_buf_اندازه(b)                                                      \
    (ngx_buf_دننه_حافظه(b) ? (off_t) ((b)->وروستی - (b)->pos):                  \
                            ((b)->file_وروستی - (b)->file_pos))

ngx_buf_t *ngx_create_لنډمهاله_buf(ngx_pool_t *pool, اندازه_t اندازه);
ngx_chain_t *ngx_create_chain_د_bufs(ngx_pool_t *pool, ngx_bufs_t *bufs);


#تعریف_کړه ngx_alloc_buf(pool)  ngx_palloc(pool, حجم(ngx_buf_t))
#تعریف_کړه ngx_calloc_buf(pool) ngx_pcalloc(pool, حجم(ngx_buf_t))

ngx_chain_t *ngx_alloc_chain_link(ngx_pool_t *pool);
#تعریف_کړه ngx_free_chain(pool, cl)                                             \
    (cl)->بل = (pool)->chain;                                              \
    (pool)->chain = (cl)



ngx_int_t ngx_output_chain(ngx_output_chain_ctx_t *ctx, ngx_chain_t *دننه);
ngx_int_t ngx_chain_writer(فراغ *ctx, ngx_chain_t *دننه);

ngx_int_t ngx_chain_جمع_کړه_copy(ngx_pool_t *pool, ngx_chain_t **chain,
    ngx_chain_t *دننه);
ngx_chain_t *ngx_chain_get_free_buf(ngx_pool_t *p, ngx_chain_t **حرر);
فراغ ngx_chain_update_chains(ngx_pool_t *p, ngx_chain_t **حرر,
    ngx_chain_t **busy, ngx_chain_t **بهر, ngx_buf_نښان_t نښان);

off_t ngx_chain_coalesce_file(ngx_chain_t **دننه, off_t حد);

ngx_chain_t *ngx_chain_update_لېږل_شوی(ngx_chain_t *دننه, off_t لېږل_شوی);

#د_که_پای /* _NGX_BUF_H_INCLUDED_ */
