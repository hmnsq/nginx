
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_PALLOC_H_INCLUDED_
#define _NGX_PALLOC_H_INCLUDED_


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


/*
 * NGX_MAX_ALLOC_FROM_POOL should be (ngx_pagesize - 1), i.e. 4095 on x86.
 * On Windows NT it decreases a number of locked pages in a kernel.
 */
#define NGX_MAX_ALLOC_FROM_POOL  (ngx_pagesize - 1)

#define NGX_DEFAULT_POOL_SIZE    (16 * 1024)

#define NGX_POOL_ALIGNMENT       16
#define NGX_MIN_POOL_SIZE                                                     \
    ngx_align((परिमाण(ngx_pool_t) + 2 * परिमाण(ngx_pool_large_t)),            \
              NGX_POOL_ALIGNMENT)


प्रकारनिर्देश शून्य (*ngx_pool_cleanup_pt)(शून्य *data);

प्रकारनिर्देश संरचना ngx_pool_cleanup_s  ngx_pool_cleanup_t;

संरचना ngx_pool_cleanup_s {
    ngx_pool_cleanup_pt   handler;
    शून्य                 *data;
    ngx_pool_cleanup_t   *next;
};


प्रकारनिर्देश संरचना ngx_pool_large_s  ngx_pool_large_t;

संरचना ngx_pool_large_s {
    ngx_pool_large_t     *next;
    शून्य                 *alloc;
};


प्रकारनिर्देश संरचना {
    u_char               *last;
    u_char               *end;
    ngx_pool_t           *next;
    ngx_uint_t            failed;
} ngx_pool_data_t;


संरचना ngx_pool_s {
    ngx_pool_data_t       d;
    माप_प्रकार                max;
    ngx_pool_t           *current;
    ngx_chain_t          *chain;
    ngx_pool_large_t     *large;
    ngx_pool_cleanup_t   *cleanup;
    ngx_log_t            *log;
};


प्रकारनिर्देश संरचना {
    ngx_fd_t              fd;
    u_char               *name;
    ngx_log_t            *log;
} ngx_pool_cleanup_file_t;


ngx_pool_t *ngx_create_pool(माप_प्रकार size, ngx_log_t *log);
शून्य ngx_destroy_pool(ngx_pool_t *pool);
शून्य ngx_reset_pool(ngx_pool_t *pool);

शून्य *ngx_palloc(ngx_pool_t *pool, माप_प्रकार size);
शून्य *ngx_pnalloc(ngx_pool_t *pool, माप_प्रकार size);
शून्य *ngx_pcalloc(ngx_pool_t *pool, माप_प्रकार size);
शून्य *ngx_pmemalign(ngx_pool_t *pool, माप_प्रकार size, माप_प्रकार alignment);
ngx_int_t ngx_pfree(ngx_pool_t *pool, शून्य *p);


ngx_pool_cleanup_t *ngx_pool_cleanup_add(ngx_pool_t *p, माप_प्रकार size);
शून्य ngx_pool_run_cleanup_file(ngx_pool_t *p, ngx_fd_t fd);
शून्य ngx_pool_cleanup_file(शून्य *data);
शून्य ngx_pool_delete_file(शून्य *data);


#endif /* _NGX_PALLOC_H_INCLUDED_ */
