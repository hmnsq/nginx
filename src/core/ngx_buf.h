
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_BUF_H_INCLUDED_
#define _NGX_BUF_H_INCLUDED_


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


प्रकारव्याख्या शून्य *            ngx_buf_tag_t;

प्रकारव्याख्या संरचना ngx_buf_s  ngx_buf_t;

संरचना ngx_buf_s {
    u_char          *pos;
    u_char          *last;
    off_t            file_pos;
    off_t            file_last;

    u_char          *start;         /* start of buffer */
    u_char          *end;           /* end of buffer */
    ngx_buf_tag_t    tag;
    ngx_file_t      *file;
    ngx_buf_t       *shadow;


    /* the buf's content could be changed */
    अचिन्हित         temporary:1;

    /*
     * the buf's content is in a memory cache or in a read only memory
     * and must not be changed
     */
    अचिन्हित         memory:1;

    /* the buf's content is mmap()ed and must not be changed */
    अचिन्हित         mmap:1;

    अचिन्हित         recycled:1;
    अचिन्हित         in_file:1;
    अचिन्हित         flush:1;
    अचिन्हित         sync:1;
    अचिन्हित         last_buf:1;
    अचिन्हित         last_in_chain:1;

    अचिन्हित         last_shadow:1;
    अचिन्हित         temp_file:1;

    /* STUB */ पूर्णांक   अंक;
};


संरचना ngx_chain_s {
    ngx_buf_t    *buf;
    ngx_chain_t  *next;
};


प्रकारव्याख्या संरचना {
    ngx_int_t    अंक;
    माप_प्रकार       size;
} ngx_bufs_t;


प्रकारव्याख्या संरचना ngx_output_chain_ctx_s  ngx_output_chain_ctx_t;

प्रकारव्याख्या ngx_int_t (*ngx_output_chain_filter_pt)(शून्य *ctx, ngx_chain_t *in);

प्रकारव्याख्या शून्य (*ngx_output_chain_aio_pt)(ngx_output_chain_ctx_t *ctx,
    ngx_file_t *file);

संरचना ngx_output_chain_ctx_s {
    ngx_buf_t                   *buf;
    ngx_chain_t                 *in;
    ngx_chain_t                 *मुक्त;
    ngx_chain_t                 *busy;

    अचिन्हित                     sendfile:1;
    अचिन्हित                     directio:1;
    अचिन्हित                     unaligned:1;
    अचिन्हित                     need_in_memory:1;
    अचिन्हित                     need_in_temp:1;
    अचिन्हित                     aio:1;

#जर (NGX_HAVE_FILE_AIO || NGX_COMPAT)
    ngx_output_chain_aio_pt      aio_handler;
#endif

#जर (NGX_THREADS || NGX_COMPAT)
    ngx_int_t                  (*thread_handler)(ngx_thread_task_t *task,
                                                 ngx_file_t *file);
    ngx_thread_task_t           *thread_task;
#endif

    off_t                        alignment;

    ngx_pool_t                  *pool;
    ngx_int_t                    allocated;
    ngx_bufs_t                   bufs;
    ngx_buf_tag_t                tag;

    ngx_output_chain_filter_pt   output_filter;
    शून्य                        *filter_ctx;
};


प्रकारव्याख्या संरचना {
    ngx_chain_t                 *out;
    ngx_chain_t                **last;
    ngx_connection_t            *connection;
    ngx_pool_t                  *pool;
    off_t                        limit;
} ngx_chain_writer_ctx_t;


#define NGX_CHAIN_ERROR     (ngx_chain_t *) NGX_ERROR


#define ngx_buf_in_memory(b)       ((b)->temporary || (b)->memory || (b)->mmap)
#define ngx_buf_in_memory_only(b)  (ngx_buf_in_memory(b) && !(b)->in_file)

#define ngx_buf_special(b)                                                   \
    (((b)->flush || (b)->last_buf || (b)->sync)                              \
     && !ngx_buf_in_memory(b) && !(b)->in_file)

#define ngx_buf_sync_only(b)                                                 \
    ((b)->sync && !ngx_buf_in_memory(b)                                      \
     && !(b)->in_file && !(b)->flush && !(b)->last_buf)

#define ngx_buf_size(b)                                                      \
    (ngx_buf_in_memory(b) ? (off_t) ((b)->last - (b)->pos):                  \
                            ((b)->file_last - (b)->file_pos))

ngx_buf_t *ngx_create_temp_buf(ngx_pool_t *pool, माप_प्रकार size);
ngx_chain_t *ngx_create_chain_of_bufs(ngx_pool_t *pool, ngx_bufs_t *bufs);


#define ngx_alloc_buf(pool)  ngx_palloc(pool, आकार(ngx_buf_t))
#define ngx_calloc_buf(pool) ngx_pcalloc(pool, आकार(ngx_buf_t))

ngx_chain_t *ngx_alloc_chain_link(ngx_pool_t *pool);
#define ngx_free_chain(pool, cl)                                             \
    (cl)->next = (pool)->chain;                                              \
    (pool)->chain = (cl)



ngx_int_t ngx_output_chain(ngx_output_chain_ctx_t *ctx, ngx_chain_t *in);
ngx_int_t ngx_chain_writer(शून्य *ctx, ngx_chain_t *in);

ngx_int_t ngx_chain_add_copy(ngx_pool_t *pool, ngx_chain_t **chain,
    ngx_chain_t *in);
ngx_chain_t *ngx_chain_get_free_buf(ngx_pool_t *p, ngx_chain_t **मुक्त);
शून्य ngx_chain_update_chains(ngx_pool_t *p, ngx_chain_t **मुक्त,
    ngx_chain_t **busy, ngx_chain_t **out, ngx_buf_tag_t tag);

off_t ngx_chain_coalesce_file(ngx_chain_t **in, off_t limit);

ngx_chain_t *ngx_chain_update_sent(ngx_chain_t *in, off_t sent);

#endif /* _NGX_BUF_H_INCLUDED_ */
