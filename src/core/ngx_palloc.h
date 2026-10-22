
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#यदि_अपरिभाषितम् _NGX_PALLOC_H_INCLUDED_
#परिभाषय _NGX_PALLOC_H_INCLUDED_


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


/*
 * NGX_MAX_ALLOC_FROM_POOL should be (ngx_pagesize - 1), i.e. 4095 on x86.
 * On Windows NT it decreases a number of locked pages in a kernel.
 */
#परिभाषय NGX_MAX_ALLOC_FROM_POOL  (ngx_pagesize - 1)

#परिभाषय NGX_DEFAULT_POOL_आकार    (16 * 1024)

#परिभाषय NGX_POOL_ALIGNMENT       16
#परिभाषय NGX_MIN_POOL_आकार                                                     \
    ngx_align((परिमाण(ngx_pool_t) + 2 * परिमाण(ngx_pool_large_t)),            \
              NGX_POOL_ALIGNMENT)


प्रकारनिर्देश शून्य (*ngx_pool_cleanup_pt)(शून्य *दत्त);

प्रकारनिर्देश संरचना ngx_pool_cleanup_s  ngx_pool_cleanup_t;

संरचना ngx_pool_cleanup_s {
    ngx_pool_cleanup_pt   handler;
    शून्य                 *दत्त;
    ngx_pool_cleanup_t   *अग्र;
};


प्रकारनिर्देश संरचना ngx_pool_large_s  ngx_pool_large_t;

संरचना ngx_pool_large_s {
    ngx_pool_large_t     *अग्र;
    शून्य                 *alloc;
};


प्रकारनिर्देश संरचना {
    u_char               *चरमः;
    u_char               *अन्तः;
    ngx_pool_t           *अग्र;
    ngx_uint_t            failed;
} ngx_pool_दत्त_t;


संरचना ngx_pool_s {
    ngx_pool_दत्त_t       d;
    आकार_t                max;
    ngx_pool_t           *current;
    ngx_chain_t          *chain;
    ngx_pool_large_t     *large;
    ngx_pool_cleanup_t   *cleanup;
    ngx_log_t            *लघुगणकः;
};


प्रकारनिर्देश संरचना {
    ngx_fd_t              fd;
    u_char               *name;
    ngx_log_t            *लघुगणकः;
} ngx_pool_cleanup_file_t;


ngx_pool_t *ngx_create_pool(आकार_t आकार, ngx_log_t *लघुगणकः);
शून्य ngx_destroy_pool(ngx_pool_t *pool);
शून्य ngx_reset_pool(ngx_pool_t *pool);

शून्य *ngx_palloc(ngx_pool_t *pool, आकार_t आकार);
शून्य *ngx_pnalloc(ngx_pool_t *pool, आकार_t आकार);
शून्य *ngx_pcalloc(ngx_pool_t *pool, आकार_t आकार);
शून्य *ngx_pmemalign(ngx_pool_t *pool, आकार_t आकार, आकार_t alignment);
ngx_int_t ngx_pfree(ngx_pool_t *pool, शून्य *p);


ngx_pool_cleanup_t *ngx_pool_cleanup_सङ्कलय(ngx_pool_t *p, आकार_t आकार);
शून्य ngx_pool_सततम्_cleanup_file(ngx_pool_t *p, ngx_fd_t fd);
शून्य ngx_pool_cleanup_file(शून्य *दत्त);
शून्य ngx_pool_delete_file(शून्य *दत्त);


#यदि_अन्तः /* _NGX_PALLOC_H_INCLUDED_ */
