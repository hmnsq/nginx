
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अगर_अघोषित _NGX_PALLOC_H_INCLUDED_
#घोषणा _NGX_PALLOC_H_INCLUDED_


#समावेश <ngx_config.h>
#समावेश <ngx_core.h>


/*
 * NGX_MAX_ALLOC_FROM_POOL should be (ngx_pagesize - 1), i.e. 4095 on x86.
 * On Windows NT it decreases a number of locked pages in a kernel.
 */
#घोषणा NGX_MAX_ALLOC_FROM_POOL  (ngx_pagesize - 1)

#घोषणा NGX_DEFAULT_POOL_आकार    (16 * 1024)

#घोषणा NGX_POOL_ALIGNMENT       16
#घोषणा NGX_MIN_POOL_आकार                                                     \
    ngx_align((माप(ngx_pool_t) + 2 * माप(ngx_pool_large_t)),            \
              NGX_POOL_ALIGNMENT)


प्रकार व्योम (*ngx_pool_cleanup_pt)(व्योम *तथ्य);

प्रकार काष्ठा ngx_pool_cleanup_s  ngx_pool_cleanup_t;

काष्ठा ngx_pool_cleanup_s {
    ngx_pool_cleanup_pt   handler;
    व्योम                 *तथ्य;
    ngx_pool_cleanup_t   *अग्र;
};


प्रकार काष्ठा ngx_pool_large_s  ngx_pool_large_t;

काष्ठा ngx_pool_large_s {
    ngx_pool_large_t     *अग्र;
    व्योम                 *alloc;
};


प्रकार काष्ठा {
    u_char               *last;
    u_char               *end;
    ngx_pool_t           *अग्र;
    ngx_uint_t            failed;
} ngx_pool_तथ्य_t;


काष्ठा ngx_pool_s {
    ngx_pool_तथ्य_t       d;
    माप_प्रकार                max;
    ngx_pool_t           *current;
    ngx_chain_t          *chain;
    ngx_pool_large_t     *large;
    ngx_pool_cleanup_t   *cleanup;
    ngx_log_t            *log;
};


प्रकार काष्ठा {
    ngx_fd_t              fd;
    u_char               *name;
    ngx_log_t            *log;
} ngx_pool_cleanup_file_t;


ngx_pool_t *ngx_create_pool(माप_प्रकार आकार, ngx_log_t *log);
व्योम ngx_destroy_pool(ngx_pool_t *pool);
व्योम ngx_reset_pool(ngx_pool_t *pool);

व्योम *ngx_palloc(ngx_pool_t *pool, माप_प्रकार आकार);
व्योम *ngx_pnalloc(ngx_pool_t *pool, माप_प्रकार आकार);
व्योम *ngx_pcalloc(ngx_pool_t *pool, माप_प्रकार आकार);
व्योम *ngx_pmemalign(ngx_pool_t *pool, माप_प्रकार आकार, माप_प्रकार alignment);
ngx_int_t ngx_pfree(ngx_pool_t *pool, व्योम *p);


ngx_pool_cleanup_t *ngx_pool_cleanup_add(ngx_pool_t *p, माप_प्रकार आकार);
व्योम ngx_pool_run_cleanup_file(ngx_pool_t *p, ngx_fd_t fd);
व्योम ngx_pool_cleanup_file(व्योम *तथ्य);
व्योम ngx_pool_delete_file(व्योम *तथ्य);


#पूर्ण_अगर /* _NGX_PALLOC_H_INCLUDED_ */
