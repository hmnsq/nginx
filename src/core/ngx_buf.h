
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ _NGX_BUF_H_INCLUDED_
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _NGX_BUF_H_INCLUDED_


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ *            ngx_buf_ਟੈਗ_t;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ngx_buf_s  ngx_buf_t;

ਬਣਤਰ ngx_buf_s {
    u_char          *pos;
    u_char          *ਆਖਰੀ;
    off_t            file_pos;
    off_t            file_ਆਖਰੀ;

    u_char          *ਸ਼ੁਰੂ;         /* start of buffer */
    u_char          *ਸਿਰਾ;           /* end of buffer */
    ngx_buf_ਟੈਗ_t    ਟੈਗ;
    ngx_file_t      *file;
    ngx_buf_t       *shadow;


    /* the buf's content could be changed */
    ਬਿਨਾਨਿਸ਼ਾਨੀ         temporary:1;

    /*
     * the buf's content is in a memory cache or in a read only memory
     * and must not be changed
     */
    ਬਿਨਾਨਿਸ਼ਾਨੀ         ਸਮ੍ਰਿਤੀ:1;

    /* the buf's content is mmap()ed and must not be changed */
    ਬਿਨਾਨਿਸ਼ਾਨੀ         mmap:1;

    ਬਿਨਾਨਿਸ਼ਾਨੀ         recycled:1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ         ਵਿੱਚ_file:1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ         flush:1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ         sync:1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ         ਆਖਰੀ_buf:1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ         ਆਖਰੀ_ਵਿੱਚ_chain:1;

    ਬਿਨਾਨਿਸ਼ਾਨੀ         ਆਖਰੀ_shadow:1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ         ਆਰਜ਼ੀ_file:1;

    /* STUB */ ਪੂਰਨ   ਅੰਕ;
};


ਬਣਤਰ ngx_chain_s {
    ngx_buf_t    *buf;
    ngx_chain_t  *ਅਗਲਾ;
};


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ngx_int_t    ਅੰਕ;
    ਮਾਪ_t       ਮਾਪ;
} ngx_bufs_t;


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ngx_output_chain_ctx_s  ngx_output_chain_ctx_t;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ngx_int_t (*ngx_output_chain_filter_pt)(ਖਾਲੀ *ctx, ngx_chain_t *ਵਿੱਚ);

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (*ngx_output_chain_aio_pt)(ngx_output_chain_ctx_t *ctx,
    ngx_file_t *file);

ਬਣਤਰ ngx_output_chain_ctx_s {
    ngx_buf_t                   *buf;
    ngx_chain_t                 *ਵਿੱਚ;
    ngx_chain_t                 *ਮੁਕਤ;
    ngx_chain_t                 *busy;

    ਬਿਨਾਨਿਸ਼ਾਨੀ                     sendfile:1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ                     directio:1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ                     unaligned:1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ                     need_ਵਿੱਚ_ਸਮ੍ਰਿਤੀ:1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ                     need_ਵਿੱਚ_ਆਰਜ਼ੀ:1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ                     aio:1;

#ਜੇ (NGX_HAVE_FILE_AIO || NGX_COMPAT)
    ngx_output_chain_aio_pt      aio_handler;
#ਜੇ_ਅੰਤ

#ਜੇ (NGX_ਧਾਗੇ || NGX_COMPAT)
    ngx_int_t                  (*thread_handler)(ngx_thread_task_t *task,
                                                 ngx_file_t *file);
    ngx_thread_task_t           *thread_task;
#ਜੇ_ਅੰਤ

    off_t                        alignment;

    ngx_pool_t                  *pool;
    ngx_int_t                    allocated;
    ngx_bufs_t                   bufs;
    ngx_buf_ਟੈਗ_t                ਟੈਗ;

    ngx_output_chain_filter_pt   output_filter;
    ਖਾਲੀ                        *filter_ctx;
};


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ngx_chain_t                 *ਬਾਹਰਲਾ;
    ngx_chain_t                **ਆਖਰੀ;
    ngx_connection_t            *connection;
    ngx_pool_t                  *pool;
    off_t                        ਹੱਦਬੰਦੀ;
} ngx_chain_writer_ctx_t;


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_CHAIN_ERROR     (ngx_chain_t *) NGX_ERROR


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_buf_ਵਿੱਚ_ਸਮ੍ਰਿਤੀ(b)       ((b)->temporary || (b)->ਸਮ੍ਰਿਤੀ || (b)->mmap)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_buf_ਵਿੱਚ_ਸਮ੍ਰਿਤੀ_only(b)  (ngx_buf_ਵਿੱਚ_ਸਮ੍ਰਿਤੀ(b) && !(b)->ਵਿੱਚ_file)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_buf_special(b)                                                   \
    (((b)->flush || (b)->ਆਖਰੀ_buf || (b)->sync)                              \
     && !ngx_buf_ਵਿੱਚ_ਸਮ੍ਰਿਤੀ(b) && !(b)->ਵਿੱਚ_file)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_buf_sync_only(b)                                                 \
    ((b)->sync && !ngx_buf_ਵਿੱਚ_ਸਮ੍ਰਿਤੀ(b)                                      \
     && !(b)->ਵਿੱਚ_file && !(b)->flush && !(b)->ਆਖਰੀ_buf)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_buf_ਮਾਪ(b)                                                      \
    (ngx_buf_ਵਿੱਚ_ਸਮ੍ਰਿਤੀ(b) ? (off_t) ((b)->ਆਖਰੀ - (b)->pos):                  \
                            ((b)->file_ਆਖਰੀ - (b)->file_pos))

ngx_buf_t *ngx_create_ਆਰਜ਼ੀ_buf(ngx_pool_t *pool, ਮਾਪ_t ਮਾਪ);
ngx_chain_t *ngx_create_chain_ਦਾ_bufs(ngx_pool_t *pool, ngx_bufs_t *bufs);


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_alloc_buf(pool)  ngx_palloc(pool, ਆਕਾਰ(ngx_buf_t))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_calloc_buf(pool) ngx_pcalloc(pool, ਆਕਾਰ(ngx_buf_t))

ngx_chain_t *ngx_alloc_chain_link(ngx_pool_t *pool);
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_free_chain(pool, cl)                                             \
    (cl)->ਅਗਲਾ = (pool)->chain;                                              \
    (pool)->chain = (cl)



ngx_int_t ngx_output_chain(ngx_output_chain_ctx_t *ctx, ngx_chain_t *ਵਿੱਚ);
ngx_int_t ngx_chain_writer(ਖਾਲੀ *ctx, ngx_chain_t *ਵਿੱਚ);

ngx_int_t ngx_chain_ਜੋੜੋ_copy(ngx_pool_t *pool, ngx_chain_t **chain,
    ngx_chain_t *ਵਿੱਚ);
ngx_chain_t *ngx_chain_get_free_buf(ngx_pool_t *p, ngx_chain_t **ਮੁਕਤ);
ਖਾਲੀ ngx_chain_update_chains(ngx_pool_t *p, ngx_chain_t **ਮੁਕਤ,
    ngx_chain_t **busy, ngx_chain_t **ਬਾਹਰਲਾ, ngx_buf_ਟੈਗ_t ਟੈਗ);

off_t ngx_chain_coalesce_file(ngx_chain_t **ਵਿੱਚ, off_t ਹੱਦਬੰਦੀ);

ngx_chain_t *ngx_chain_update_ਭੇਜਿਆ(ngx_chain_t *ਵਿੱਚ, off_t ਭੇਜਿਆ);

#ਜੇ_ਅੰਤ /* _NGX_BUF_H_INCLUDED_ */
