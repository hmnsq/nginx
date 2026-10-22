
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#నిర్వచితం_కాకపోతే _NGX_BUF_H_INCLUDED_
#నిర్వచించు _NGX_BUF_H_INCLUDED_


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>


రకనిర్వచనం శూన్య *            ngx_buf_ట్యాగ్_t;

రకనిర్వచనం నిర్మాణం ngx_buf_s  ngx_buf_t;

నిర్మాణం ngx_buf_s {
    u_char          *pos;
    u_char          *చివరి;
    off_t            file_pos;
    off_t            file_చివరి;

    u_char          *ప్రారంభం;         /* start of buffer */
    u_char          *చివర;           /* end of buffer */
    ngx_buf_ట్యాగ్_t    ట్యాగ్;
    ngx_file_t      *file;
    ngx_buf_t       *shadow;


    /* the buf's content could be changed */
    గుర్తులేని         temporary:1;

    /*
     * the buf's content is in a memory cache or in a read only memory
     * and must not be changed
     */
    గుర్తులేని         జ్ఞాపకం:1;

    /* the buf's content is mmap()ed and must not be changed */
    గుర్తులేని         mmap:1;

    గుర్తులేని         recycled:1;
    గుర్తులేని         లోపల_file:1;
    గుర్తులేని         flush:1;
    గుర్తులేని         sync:1;
    గుర్తులేని         చివరి_buf:1;
    గుర్తులేని         చివరి_లోపల_chain:1;

    గుర్తులేని         చివరి_shadow:1;
    గుర్తులేని         తాత్కాలిక_file:1;

    /* STUB */ పూర్ణాంకం   అంకె;
};


నిర్మాణం ngx_chain_s {
    ngx_buf_t    *buf;
    ngx_chain_t  *తదుపరి;
};


రకనిర్వచనం నిర్మాణం {
    ngx_int_t    అంకె;
    కొలత_t       కొలత;
} ngx_bufs_t;


రకనిర్వచనం నిర్మాణం ngx_output_chain_ctx_s  ngx_output_chain_ctx_t;

రకనిర్వచనం ngx_int_t (*ngx_output_chain_filter_pt)(శూన్య *ctx, ngx_chain_t *లోపల);

రకనిర్వచనం శూన్య (*ngx_output_chain_aio_pt)(ngx_output_chain_ctx_t *ctx,
    ngx_file_t *file);

నిర్మాణం ngx_output_chain_ctx_s {
    ngx_buf_t                   *buf;
    ngx_chain_t                 *లోపల;
    ngx_chain_t                 *విడుదల;
    ngx_chain_t                 *busy;

    గుర్తులేని                     sendfile:1;
    గుర్తులేని                     directio:1;
    గుర్తులేని                     unaligned:1;
    గుర్తులేని                     need_లోపల_జ్ఞాపకం:1;
    గుర్తులేని                     need_లోపల_తాత్కాలిక:1;
    గుర్తులేని                     aio:1;

#ఒకవేళ (NGX_HAVE_FILE_AIO || NGX_COMPAT)
    ngx_output_chain_aio_pt      aio_handler;
#అయితే_ముగింపు

#ఒకవేళ (NGX_దారాలు || NGX_COMPAT)
    ngx_int_t                  (*thread_handler)(ngx_thread_task_t *task,
                                                 ngx_file_t *file);
    ngx_thread_task_t           *thread_task;
#అయితే_ముగింపు

    off_t                        alignment;

    ngx_pool_t                  *pool;
    ngx_int_t                    allocated;
    ngx_bufs_t                   bufs;
    ngx_buf_ట్యాగ్_t                ట్యాగ్;

    ngx_output_chain_filter_pt   output_filter;
    శూన్య                        *filter_ctx;
};


రకనిర్వచనం నిర్మాణం {
    ngx_chain_t                 *బహిర్గతం;
    ngx_chain_t                **చివరి;
    ngx_connection_t            *connection;
    ngx_pool_t                  *pool;
    off_t                        హద్దు;
} ngx_chain_writer_ctx_t;


#నిర్వచించు NGX_CHAIN_ERROR     (ngx_chain_t *) NGX_ERROR


#నిర్వచించు ngx_buf_లోపల_జ్ఞాపకం(b)       ((b)->temporary || (b)->జ్ఞాపకం || (b)->mmap)
#నిర్వచించు ngx_buf_లోపల_జ్ఞాపకం_only(b)  (ngx_buf_లోపల_జ్ఞాపకం(b) && !(b)->లోపల_file)

#నిర్వచించు ngx_buf_special(b)                                                   \
    (((b)->flush || (b)->చివరి_buf || (b)->sync)                              \
     && !ngx_buf_లోపల_జ్ఞాపకం(b) && !(b)->లోపల_file)

#నిర్వచించు ngx_buf_sync_only(b)                                                 \
    ((b)->sync && !ngx_buf_లోపల_జ్ఞాపకం(b)                                      \
     && !(b)->లోపల_file && !(b)->flush && !(b)->చివరి_buf)

#నిర్వచించు ngx_buf_కొలత(b)                                                      \
    (ngx_buf_లోపల_జ్ఞాపకం(b) ? (off_t) ((b)->చివరి - (b)->pos):                  \
                            ((b)->file_చివరి - (b)->file_pos))

ngx_buf_t *ngx_create_తాత్కాలిక_buf(ngx_pool_t *pool, కొలత_t కొలత);
ngx_chain_t *ngx_create_chain_యొక్క_bufs(ngx_pool_t *pool, ngx_bufs_t *bufs);


#నిర్వచించు ngx_alloc_buf(pool)  ngx_palloc(pool, పరిమాణం(ngx_buf_t))
#నిర్వచించు ngx_calloc_buf(pool) ngx_pcalloc(pool, పరిమాణం(ngx_buf_t))

ngx_chain_t *ngx_alloc_chain_link(ngx_pool_t *pool);
#నిర్వచించు ngx_free_chain(pool, cl)                                             \
    (cl)->తదుపరి = (pool)->chain;                                              \
    (pool)->chain = (cl)



ngx_int_t ngx_output_chain(ngx_output_chain_ctx_t *ctx, ngx_chain_t *లోపల);
ngx_int_t ngx_chain_writer(శూన్య *ctx, ngx_chain_t *లోపల);

ngx_int_t ngx_chain_కూడు_copy(ngx_pool_t *pool, ngx_chain_t **chain,
    ngx_chain_t *లోపల);
ngx_chain_t *ngx_chain_get_free_buf(ngx_pool_t *p, ngx_chain_t **విడుదల);
శూన్య ngx_chain_update_chains(ngx_pool_t *p, ngx_chain_t **విడుదల,
    ngx_chain_t **busy, ngx_chain_t **బహిర్గతం, ngx_buf_ట్యాగ్_t ట్యాగ్);

off_t ngx_chain_coalesce_file(ngx_chain_t **లోపల, off_t హద్దు);

ngx_chain_t *ngx_chain_update_పంపిన(ngx_chain_t *లోపల, off_t పంపిన);

#అయితే_ముగింపు /* _NGX_BUF_H_INCLUDED_ */
