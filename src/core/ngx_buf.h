
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#جے_بنا_تعریف _NGX_BUF_H_INCLUDED_
#تعریف_کرو _NGX_BUF_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


تعريفنوع فراغ *            ngx_buf_ٹیگ_t;

تعريفنوع بنية ngx_buf_s  ngx_buf_t;

بنية ngx_buf_s {
    u_char          *pos;
    u_char          *آخری;
    off_t            file_pos;
    off_t            file_آخری;

    u_char          *شروع;         /* start of buffer */
    u_char          *سرا;           /* end of buffer */
    ngx_buf_ٹیگ_t    ٹیگ;
    ngx_file_t      *file;
    ngx_buf_t       *shadow;


    /* the buf's content could be changed */
    بنانشانی         temporary:1;

    /*
     * the buf's content is in a memory cache or in a read only memory
     * and must not be changed
     */
    بنانشانی         حافظہ:1;

    /* the buf's content is mmap()ed and must not be changed */
    بنانشانی         mmap:1;

    بنانشانی         recycled:1;
    بنانشانی         وچ_file:1;
    بنانشانی         flush:1;
    بنانشانی         sync:1;
    بنانشانی         آخری_buf:1;
    بنانشانی         آخری_وچ_chain:1;

    بنانشانی         آخری_shadow:1;
    بنانشانی         عارضی_file:1;

    /* STUB */ صحيح   عدد;
};


بنية ngx_chain_s {
    ngx_buf_t    *buf;
    ngx_chain_t  *اگلا;
};


تعريفنوع بنية {
    ngx_int_t    عدد;
    ناپ_t       ناپ;
} ngx_bufs_t;


تعريفنوع بنية ngx_output_chain_ctx_s  ngx_output_chain_ctx_t;

تعريفنوع ngx_int_t (*ngx_output_chain_filter_pt)(فراغ *ctx, ngx_chain_t *وچ);

تعريفنوع فراغ (*ngx_output_chain_aio_pt)(ngx_output_chain_ctx_t *ctx,
    ngx_file_t *file);

بنية ngx_output_chain_ctx_s {
    ngx_buf_t                   *buf;
    ngx_chain_t                 *وچ;
    ngx_chain_t                 *حرر;
    ngx_chain_t                 *busy;

    بنانشانی                     sendfile:1;
    بنانشانی                     directio:1;
    بنانشانی                     unaligned:1;
    بنانشانی                     need_وچ_حافظہ:1;
    بنانشانی                     need_وچ_عارضی:1;
    بنانشانی                     aio:1;

#إذا (NGX_HAVE_FILE_AIO || NGX_COMPAT)
    ngx_output_chain_aio_pt      aio_handler;
#جے_انت

#إذا (NGX_دھاگے || NGX_COMPAT)
    ngx_int_t                  (*thread_handler)(ngx_thread_task_t *task,
                                                 ngx_file_t *file);
    ngx_thread_task_t           *thread_task;
#جے_انت

    off_t                        alignment;

    ngx_pool_t                  *pool;
    ngx_int_t                    allocated;
    ngx_bufs_t                   bufs;
    ngx_buf_ٹیگ_t                ٹیگ;

    ngx_output_chain_filter_pt   output_filter;
    فراغ                        *filter_ctx;
};


تعريفنوع بنية {
    ngx_chain_t                 *باہرلا;
    ngx_chain_t                **آخری;
    ngx_connection_t            *connection;
    ngx_pool_t                  *pool;
    off_t                        حد_بندی;
} ngx_chain_writer_ctx_t;


#تعریف_کرو NGX_CHAIN_ERROR     (ngx_chain_t *) NGX_ERROR


#تعریف_کرو ngx_buf_وچ_حافظہ(b)       ((b)->temporary || (b)->حافظہ || (b)->mmap)
#تعریف_کرو ngx_buf_وچ_حافظہ_only(b)  (ngx_buf_وچ_حافظہ(b) && !(b)->وچ_file)

#تعریف_کرو ngx_buf_special(b)                                                   \
    (((b)->flush || (b)->آخری_buf || (b)->sync)                              \
     && !ngx_buf_وچ_حافظہ(b) && !(b)->وچ_file)

#تعریف_کرو ngx_buf_sync_only(b)                                                 \
    ((b)->sync && !ngx_buf_وچ_حافظہ(b)                                      \
     && !(b)->وچ_file && !(b)->flush && !(b)->آخری_buf)

#تعریف_کرو ngx_buf_ناپ(b)                                                      \
    (ngx_buf_وچ_حافظہ(b) ? (off_t) ((b)->آخری - (b)->pos):                  \
                            ((b)->file_آخری - (b)->file_pos))

ngx_buf_t *ngx_create_عارضی_buf(ngx_pool_t *pool, ناپ_t ناپ);
ngx_chain_t *ngx_create_chain_دا_bufs(ngx_pool_t *pool, ngx_bufs_t *bufs);


#تعریف_کرو ngx_alloc_buf(pool)  ngx_palloc(pool, حجم(ngx_buf_t))
#تعریف_کرو ngx_calloc_buf(pool) ngx_pcalloc(pool, حجم(ngx_buf_t))

ngx_chain_t *ngx_alloc_chain_link(ngx_pool_t *pool);
#تعریف_کرو ngx_free_chain(pool, cl)                                             \
    (cl)->اگلا = (pool)->chain;                                              \
    (pool)->chain = (cl)



ngx_int_t ngx_output_chain(ngx_output_chain_ctx_t *ctx, ngx_chain_t *وچ);
ngx_int_t ngx_chain_writer(فراغ *ctx, ngx_chain_t *وچ);

ngx_int_t ngx_chain_جوڑو_copy(ngx_pool_t *pool, ngx_chain_t **chain,
    ngx_chain_t *وچ);
ngx_chain_t *ngx_chain_get_free_buf(ngx_pool_t *p, ngx_chain_t **حرر);
فراغ ngx_chain_update_chains(ngx_pool_t *p, ngx_chain_t **حرر,
    ngx_chain_t **busy, ngx_chain_t **باہرلا, ngx_buf_ٹیگ_t ٹیگ);

off_t ngx_chain_coalesce_file(ngx_chain_t **وچ, off_t حد_بندی);

ngx_chain_t *ngx_chain_update_گھلیا(ngx_chain_t *وچ, off_t گھلیا);

#جے_انت /* _NGX_BUF_H_INCLUDED_ */
