
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ _NGX_BUF_H_INCLUDED_
#ವ್ಯಾಖ್ಯಾನಿಸು _NGX_BUF_H_INCLUDED_


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ *            ngx_buf_ಟ್ಯಾಗ್_t;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ ngx_buf_s  ngx_buf_t;

ರಚನೆ ngx_buf_s {
    u_char          *pos;
    u_char          *ಕೊನೆಯ;
    off_t            file_pos;
    off_t            file_ಕೊನೆಯ;

    u_char          *ಆರಂಭ;         /* start of buffer */
    u_char          *ಕೊನೆ;           /* end of buffer */
    ngx_buf_ಟ್ಯಾಗ್_t    ಟ್ಯಾಗ್;
    ngx_file_t      *file;
    ngx_buf_t       *shadow;


    /* the buf's content could be changed */
    ಚಿಹ್ನೆರಹಿತ         temporary:1;

    /*
     * the buf's content is in a memory cache or in a read only memory
     * and must not be changed
     */
    ಚಿಹ್ನೆರಹಿತ         ಸ್ಮರಣೆ:1;

    /* the buf's content is mmap()ed and must not be changed */
    ಚಿಹ್ನೆರಹಿತ         mmap:1;

    ಚಿಹ್ನೆರಹಿತ         recycled:1;
    ಚಿಹ್ನೆರಹಿತ         ಒಳ_file:1;
    ಚಿಹ್ನೆರಹಿತ         flush:1;
    ಚಿಹ್ನೆರಹಿತ         sync:1;
    ಚಿಹ್ನೆರಹಿತ         ಕೊನೆಯ_buf:1;
    ಚಿಹ್ನೆರಹಿತ         ಕೊನೆಯ_ಒಳ_chain:1;

    ಚಿಹ್ನೆರಹಿತ         ಕೊನೆಯ_shadow:1;
    ಚಿಹ್ನೆರಹಿತ         ತಾತ್ಕಾಲಿಕ_file:1;

    /* STUB */ ಪೂರ್ಣಾಂಕ   ಅಂಕೆ;
};


ರಚನೆ ngx_chain_s {
    ngx_buf_t    *buf;
    ngx_chain_t  *ಮುಂದಿನ;
};


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ngx_int_t    ಅಂಕೆ;
    ಗಾತ್ರ_t       ಗಾತ್ರ_2;
} ngx_bufs_t;


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ ngx_output_chain_ctx_s  ngx_output_chain_ctx_t;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ngx_int_t (*ngx_output_chain_filter_pt)(ಶೂನ್ಯ *ctx, ngx_chain_t *ಒಳ);

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (*ngx_output_chain_aio_pt)(ngx_output_chain_ctx_t *ctx,
    ngx_file_t *file);

ರಚನೆ ngx_output_chain_ctx_s {
    ngx_buf_t                   *buf;
    ngx_chain_t                 *ಒಳ;
    ngx_chain_t                 *ಬಿಡುಗಡೆ;
    ngx_chain_t                 *busy;

    ಚಿಹ್ನೆರಹಿತ                     sendfile:1;
    ಚಿಹ್ನೆರಹಿತ                     directio:1;
    ಚಿಹ್ನೆರಹಿತ                     unaligned:1;
    ಚಿಹ್ನೆರಹಿತ                     need_ಒಳ_ಸ್ಮರಣೆ:1;
    ಚಿಹ್ನೆರಹಿತ                     need_ಒಳ_ತಾತ್ಕಾಲಿಕ:1;
    ಚಿಹ್ನೆರಹಿತ                     aio:1;

#ಆದರೆ (NGX_HAVE_FILE_AIO || NGX_COMPAT)
    ngx_output_chain_aio_pt      aio_handler;
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ (NGX_ಎಳೆಗಳು || NGX_COMPAT)
    ngx_int_t                  (*thread_handler)(ngx_thread_task_t *task,
                                                 ngx_file_t *file);
    ngx_thread_task_t           *thread_task;
#ಆದರೆ_ಅಂತ್ಯ

    off_t                        alignment;

    ngx_pool_t                  *pool;
    ngx_int_t                    allocated;
    ngx_bufs_t                   bufs;
    ngx_buf_ಟ್ಯಾಗ್_t                ಟ್ಯಾಗ್;

    ngx_output_chain_filter_pt   output_filter;
    ಶೂನ್ಯ                        *filter_ctx;
};


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ngx_chain_t                 *ಹೊರಹೋಗು;
    ngx_chain_t                **ಕೊನೆಯ;
    ngx_connection_t            *connection;
    ngx_pool_t                  *pool;
    off_t                        ಎಲ್ಲೆ;
} ngx_chain_writer_ctx_t;


#ವ್ಯಾಖ್ಯಾನಿಸು NGX_CHAIN_ERROR     (ngx_chain_t *) NGX_ERROR


#ವ್ಯಾಖ್ಯಾನಿಸು ngx_buf_ಒಳ_ಸ್ಮರಣೆ(b)       ((b)->temporary || (b)->ಸ್ಮರಣೆ || (b)->mmap)
#ವ್ಯಾಖ್ಯಾನಿಸು ngx_buf_ಒಳ_ಸ್ಮರಣೆ_only(b)  (ngx_buf_ಒಳ_ಸ್ಮರಣೆ(b) && !(b)->ಒಳ_file)

#ವ್ಯಾಖ್ಯಾನಿಸು ngx_buf_special(b)                                                   \
    (((b)->flush || (b)->ಕೊನೆಯ_buf || (b)->sync)                              \
     && !ngx_buf_ಒಳ_ಸ್ಮರಣೆ(b) && !(b)->ಒಳ_file)

#ವ್ಯಾಖ್ಯಾನಿಸು ngx_buf_sync_only(b)                                                 \
    ((b)->sync && !ngx_buf_ಒಳ_ಸ್ಮರಣೆ(b)                                      \
     && !(b)->ಒಳ_file && !(b)->flush && !(b)->ಕೊನೆಯ_buf)

#ವ್ಯಾಖ್ಯಾನಿಸು ngx_buf_ಗಾತ್ರ(b)                                                      \
    (ngx_buf_ಒಳ_ಸ್ಮರಣೆ(b) ? (off_t) ((b)->ಕೊನೆಯ - (b)->pos):                  \
                            ((b)->file_ಕೊನೆಯ - (b)->file_pos))

ngx_buf_t *ngx_create_ತಾತ್ಕಾಲಿಕ_buf(ngx_pool_t *pool, ಗಾತ್ರ_t ಗಾತ್ರ_2);
ngx_chain_t *ngx_create_chain_ದ_bufs(ngx_pool_t *pool, ngx_bufs_t *bufs);


#ವ್ಯಾಖ್ಯಾನಿಸು ngx_alloc_buf(pool)  ngx_palloc(pool, ಗಾತ್ರ(ngx_buf_t))
#ವ್ಯಾಖ್ಯಾನಿಸು ngx_calloc_buf(pool) ngx_pcalloc(pool, ಗಾತ್ರ(ngx_buf_t))

ngx_chain_t *ngx_alloc_chain_link(ngx_pool_t *pool);
#ವ್ಯಾಖ್ಯಾನಿಸು ngx_free_chain(pool, cl)                                             \
    (cl)->ಮುಂದಿನ = (pool)->chain;                                              \
    (pool)->chain = (cl)



ngx_int_t ngx_output_chain(ngx_output_chain_ctx_t *ctx, ngx_chain_t *ಒಳ);
ngx_int_t ngx_chain_writer(ಶೂನ್ಯ *ctx, ngx_chain_t *ಒಳ);

ngx_int_t ngx_chain_ಕೂಡಿಸು_copy(ngx_pool_t *pool, ngx_chain_t **chain,
    ngx_chain_t *ಒಳ);
ngx_chain_t *ngx_chain_get_free_buf(ngx_pool_t *p, ngx_chain_t **ಬಿಡುಗಡೆ);
ಶೂನ್ಯ ngx_chain_update_chains(ngx_pool_t *p, ngx_chain_t **ಬಿಡುಗಡೆ,
    ngx_chain_t **busy, ngx_chain_t **ಹೊರಹೋಗು, ngx_buf_ಟ್ಯಾಗ್_t ಟ್ಯಾಗ್);

off_t ngx_chain_coalesce_file(ngx_chain_t **ಒಳ, off_t ಎಲ್ಲೆ);

ngx_chain_t *ngx_chain_update_ಕಳುಹಿಸಿದ(ngx_chain_t *ಒಳ, off_t ಕಳುಹಿಸಿದ);

#ಆದರೆ_ಅಂತ್ಯ /* _NGX_BUF_H_INCLUDED_ */
