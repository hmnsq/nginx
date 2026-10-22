
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#جيڪڏهن_وصف_نه_ٿيل _NGX_BUF_H_INCLUDED_
#وصف_ڪريو _NGX_BUF_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


تعريفنوع فراغ *            ngx_buf_ٽيگ_t;

تعريفنوع بنية ngx_buf_s  ngx_buf_t;

بنية ngx_buf_s {
    u_char          *pos;
    u_char          *آخري;
    off_t            file_pos;
    off_t            file_آخري;

    u_char          *شروعات;         /* start of buffer */
    u_char          *پڇاڙي;           /* end of buffer */
    ngx_buf_ٽيگ_t    ٽيگ;
    ngx_file_t      *file;
    ngx_buf_t       *shadow;


    /* the buf's content could be changed */
    بنانشان         temporary:1;

    /*
     * the buf's content is in a memory cache or in a read only memory
     * and must not be changed
     */
    بنانشان         يادگيري:1;

    /* the buf's content is mmap()ed and must not be changed */
    بنانشان         mmap:1;

    بنانشان         recycled:1;
    بنانشان         اندريون_file:1;
    بنانشان         flush:1;
    بنانشان         sync:1;
    بنانشان         آخري_buf:1;
    بنانشان         آخري_اندريون_chain:1;

    بنانشان         آخري_shadow:1;
    بنانشان         عارضي_file:1;

    /* STUB */ صحيح   انگ;
};


بنية ngx_chain_s {
    ngx_buf_t    *buf;
    ngx_chain_t  *ايندڙ;
};


تعريفنوع بنية {
    ngx_int_t    انگ;
    سائيز_t       سائيز;
} ngx_bufs_t;


تعريفنوع بنية ngx_output_chain_ctx_s  ngx_output_chain_ctx_t;

تعريفنوع ngx_int_t (*ngx_output_chain_filter_pt)(فراغ *ctx, ngx_chain_t *اندريون);

تعريفنوع فراغ (*ngx_output_chain_aio_pt)(ngx_output_chain_ctx_t *ctx,
    ngx_file_t *file);

بنية ngx_output_chain_ctx_s {
    ngx_buf_t                   *buf;
    ngx_chain_t                 *اندريون;
    ngx_chain_t                 *حرر;
    ngx_chain_t                 *busy;

    بنانشان                     sendfile:1;
    بنانشان                     directio:1;
    بنانشان                     unaligned:1;
    بنانشان                     need_اندريون_يادگيري:1;
    بنانشان                     need_اندريون_عارضي:1;
    بنانشان                     aio:1;

#إذا (NGX_HAVE_FILE_AIO || NGX_COMPAT)
    ngx_output_chain_aio_pt      aio_handler;
#جيڪڏهن_پورو

#إذا (NGX_ڌاڳا || NGX_COMPAT)
    ngx_int_t                  (*thread_handler)(ngx_thread_task_t *task,
                                                 ngx_file_t *file);
    ngx_thread_task_t           *thread_task;
#جيڪڏهن_پورو

    off_t                        alignment;

    ngx_pool_t                  *pool;
    ngx_int_t                    allocated;
    ngx_bufs_t                   bufs;
    ngx_buf_ٽيگ_t                ٽيگ;

    ngx_output_chain_filter_pt   output_filter;
    فراغ                        *filter_ctx;
};


تعريفنوع بنية {
    ngx_chain_t                 *ٻاهريون;
    ngx_chain_t                **آخري;
    ngx_connection_t            *connection;
    ngx_pool_t                  *pool;
    off_t                        حد_بندي;
} ngx_chain_writer_ctx_t;


#وصف_ڪريو NGX_CHAIN_ERROR     (ngx_chain_t *) NGX_ERROR


#وصف_ڪريو ngx_buf_اندريون_يادگيري(b)       ((b)->temporary || (b)->يادگيري || (b)->mmap)
#وصف_ڪريو ngx_buf_اندريون_يادگيري_only(b)  (ngx_buf_اندريون_يادگيري(b) && !(b)->اندريون_file)

#وصف_ڪريو ngx_buf_special(b)                                                   \
    (((b)->flush || (b)->آخري_buf || (b)->sync)                              \
     && !ngx_buf_اندريون_يادگيري(b) && !(b)->اندريون_file)

#وصف_ڪريو ngx_buf_sync_only(b)                                                 \
    ((b)->sync && !ngx_buf_اندريون_يادگيري(b)                                      \
     && !(b)->اندريون_file && !(b)->flush && !(b)->آخري_buf)

#وصف_ڪريو ngx_buf_سائيز(b)                                                      \
    (ngx_buf_اندريون_يادگيري(b) ? (off_t) ((b)->آخري - (b)->pos):                  \
                            ((b)->file_آخري - (b)->file_pos))

ngx_buf_t *ngx_create_عارضي_buf(ngx_pool_t *pool, سائيز_t سائيز);
ngx_chain_t *ngx_create_chain_جو_bufs(ngx_pool_t *pool, ngx_bufs_t *bufs);


#وصف_ڪريو ngx_alloc_buf(pool)  ngx_palloc(pool, حجم(ngx_buf_t))
#وصف_ڪريو ngx_calloc_buf(pool) ngx_pcalloc(pool, حجم(ngx_buf_t))

ngx_chain_t *ngx_alloc_chain_link(ngx_pool_t *pool);
#وصف_ڪريو ngx_free_chain(pool, cl)                                             \
    (cl)->ايندڙ = (pool)->chain;                                              \
    (pool)->chain = (cl)



ngx_int_t ngx_output_chain(ngx_output_chain_ctx_t *ctx, ngx_chain_t *اندريون);
ngx_int_t ngx_chain_writer(فراغ *ctx, ngx_chain_t *اندريون);

ngx_int_t ngx_chain_جوڙيو_copy(ngx_pool_t *pool, ngx_chain_t **chain,
    ngx_chain_t *اندريون);
ngx_chain_t *ngx_chain_get_free_buf(ngx_pool_t *p, ngx_chain_t **حرر);
فراغ ngx_chain_update_chains(ngx_pool_t *p, ngx_chain_t **حرر,
    ngx_chain_t **busy, ngx_chain_t **ٻاهريون, ngx_buf_ٽيگ_t ٽيگ);

off_t ngx_chain_coalesce_file(ngx_chain_t **اندريون, off_t حد_بندي);

ngx_chain_t *ngx_chain_update_موڪليل(ngx_chain_t *اندريون, off_t موڪليل);

#جيڪڏهن_پورو /* _NGX_BUF_H_INCLUDED_ */
