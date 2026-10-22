
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#यदि_अपरिभाषितम् _NGX_BUF_H_INCLUDED_
#परिभाषय _NGX_BUF_H_INCLUDED_


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


प्रकारनिर्देश शून्य *            ngx_buf_चिह्नकम्_t;

प्रकारनिर्देश संरचना ngx_buf_s  ngx_buf_t;

संरचना ngx_buf_s {
    u_char          *pos;
    u_char          *चरमः;
    off_t            file_pos;
    off_t            file_चरमः;

    u_char          *start;         /* start of buffer */
    u_char          *अन्तः;           /* end of buffer */
    ngx_buf_चिह्नकम्_t    चिह्नकम्;
    ngx_file_t      *file;
    ngx_buf_t       *shadow;


    /* the buf's content could be changed */
    अचिह्नित         temporary:1;

    /*
     * the buf's content is in a memory cache or in a read only memory
     * and must not be changed
     */
    अचिह्नित         स्मृतिः:1;

    /* the buf's content is mmap()ed and must not be changed */
    अचिह्नित         mmap:1;

    अचिह्नित         recycled:1;
    अचिह्नित         अन्तर्_file:1;
    अचिह्नित         flush:1;
    अचिह्नित         sync:1;
    अचिह्नित         चरमः_buf:1;
    अचिह्नित         चरमः_अन्तर्_chain:1;

    अचिह्नित         चरमः_shadow:1;
    अचिह्नित         अस्थायी_file:1;

    /* STUB */ पूर्णाङ्क   अङ्कः;
};


संरचना ngx_chain_s {
    ngx_buf_t    *buf;
    ngx_chain_t  *अग्र;
};


प्रकारनिर्देश संरचना {
    ngx_int_t    अङ्कः;
    आकार_t       आकार;
} ngx_bufs_t;


प्रकारनिर्देश संरचना ngx_output_chain_ctx_s  ngx_output_chain_ctx_t;

प्रकारनिर्देश ngx_int_t (*ngx_output_chain_filter_pt)(शून्य *ctx, ngx_chain_t *अन्तर्);

प्रकारनिर्देश शून्य (*ngx_output_chain_aio_pt)(ngx_output_chain_ctx_t *ctx,
    ngx_file_t *file);

संरचना ngx_output_chain_ctx_s {
    ngx_buf_t                   *buf;
    ngx_chain_t                 *अन्तर्;
    ngx_chain_t                 *मुक्त;
    ngx_chain_t                 *busy;

    अचिह्नित                     sendfile:1;
    अचिह्नित                     directio:1;
    अचिह्नित                     unaligned:1;
    अचिह्नित                     need_अन्तर्_स्मृतिः:1;
    अचिह्नित                     need_अन्तर्_अस्थायी:1;
    अचिह्नित                     aio:1;

#यदि (NGX_HAVE_FILE_AIO || NGX_COMPAT)
    ngx_output_chain_aio_pt      aio_handler;
#यदि_अन्तः

#यदि (NGX_तन्तवः || NGX_COMPAT)
    ngx_int_t                  (*thread_handler)(ngx_thread_task_t *task,
                                                 ngx_file_t *file);
    ngx_thread_task_t           *thread_task;
#यदि_अन्तः

    off_t                        alignment;

    ngx_pool_t                  *pool;
    ngx_int_t                    allocated;
    ngx_bufs_t                   bufs;
    ngx_buf_चिह्नकम्_t                चिह्नकम्;

    ngx_output_chain_filter_pt   output_filter;
    शून्य                        *filter_ctx;
};


प्रकारनिर्देश संरचना {
    ngx_chain_t                 *बहिर्गमनम्;
    ngx_chain_t                **चरमः;
    ngx_connection_t            *connection;
    ngx_pool_t                  *pool;
    off_t                        सीमा;
} ngx_chain_writer_ctx_t;


#परिभाषय NGX_CHAIN_ERROR     (ngx_chain_t *) NGX_ERROR


#परिभाषय ngx_buf_अन्तर्_स्मृतिः(b)       ((b)->temporary || (b)->स्मृतिः || (b)->mmap)
#परिभाषय ngx_buf_अन्तर्_स्मृतिः_only(b)  (ngx_buf_अन्तर्_स्मृतिः(b) && !(b)->अन्तर्_file)

#परिभाषय ngx_buf_special(b)                                                   \
    (((b)->flush || (b)->चरमः_buf || (b)->sync)                              \
     && !ngx_buf_अन्तर्_स्मृतिः(b) && !(b)->अन्तर्_file)

#परिभाषय ngx_buf_sync_only(b)                                                 \
    ((b)->sync && !ngx_buf_अन्तर्_स्मृतिः(b)                                      \
     && !(b)->अन्तर्_file && !(b)->flush && !(b)->चरमः_buf)

#परिभाषय ngx_buf_आकार(b)                                                      \
    (ngx_buf_अन्तर्_स्मृतिः(b) ? (off_t) ((b)->चरमः - (b)->pos):                  \
                            ((b)->file_चरमः - (b)->file_pos))

ngx_buf_t *ngx_create_अस्थायी_buf(ngx_pool_t *pool, आकार_t आकार);
ngx_chain_t *ngx_create_chain_सम्बन्धि_bufs(ngx_pool_t *pool, ngx_bufs_t *bufs);


#परिभाषय ngx_alloc_buf(pool)  ngx_palloc(pool, परिमाण(ngx_buf_t))
#परिभाषय ngx_calloc_buf(pool) ngx_pcalloc(pool, परिमाण(ngx_buf_t))

ngx_chain_t *ngx_alloc_chain_link(ngx_pool_t *pool);
#परिभाषय ngx_free_chain(pool, cl)                                             \
    (cl)->अग्र = (pool)->chain;                                              \
    (pool)->chain = (cl)



ngx_int_t ngx_output_chain(ngx_output_chain_ctx_t *ctx, ngx_chain_t *अन्तर्);
ngx_int_t ngx_chain_writer(शून्य *ctx, ngx_chain_t *अन्तर्);

ngx_int_t ngx_chain_सङ्कलय_copy(ngx_pool_t *pool, ngx_chain_t **chain,
    ngx_chain_t *अन्तर्);
ngx_chain_t *ngx_chain_get_free_buf(ngx_pool_t *p, ngx_chain_t **मुक्त);
शून्य ngx_chain_update_chains(ngx_pool_t *p, ngx_chain_t **मुक्त,
    ngx_chain_t **busy, ngx_chain_t **बहिर्गमनम्, ngx_buf_चिह्नकम्_t चिह्नकम्);

off_t ngx_chain_coalesce_file(ngx_chain_t **अन्तर्, off_t सीमा);

ngx_chain_t *ngx_chain_update_प्रेषितम्(ngx_chain_t *अन्तर्, off_t प्रेषितम्);

#यदि_अन्तः /* _NGX_BUF_H_INCLUDED_ */
