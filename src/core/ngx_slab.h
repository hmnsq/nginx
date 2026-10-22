
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_SLAB_H_INCLUDED_
#define _NGX_SLAB_H_INCLUDED_


#અન્તર્ભાવ <ngx_config.h>
#અન્તર્ભાવ <ngx_core.h>


પ્રકારવ્યાખ્યા સંરચના ngx_slab_page_s  ngx_slab_page_t;

સંરચના ngx_slab_page_s {
    uintptr_t         slab;
    ngx_slab_page_t  *next;
    uintptr_t         prev;
};


પ્રકારવ્યાખ્યા સંરચના {
    ngx_uint_t        total;
    ngx_uint_t        used;

    ngx_uint_t        reqs;
    ngx_uint_t        fails;
} ngx_slab_stat_t;


પ્રકારવ્યાખ્યા સંરચના {
    ngx_shmtx_sh_t    lock;

    size_t            min_size;
    size_t            min_shift;

    ngx_slab_page_t  *pages;
    ngx_slab_page_t  *last;
    ngx_slab_page_t   મુક્ત;

    ngx_slab_stat_t  *stats;
    ngx_uint_t        pfree;

    u_char           *start;
    u_char           *end;

    ngx_shmtx_t       mutex;

    u_char           *log_ctx;
    u_char            zero;

    unsigned          log_nomem:1;

    શૂન્ય             *data;
    શૂન્ય             *addr;
} ngx_slab_pool_t;


શૂન્ય ngx_slab_sizes_init(શૂન્ય);
શૂન્ય ngx_slab_init(ngx_slab_pool_t *pool);
શૂન્ય *ngx_slab_alloc(ngx_slab_pool_t *pool, size_t size);
શૂન્ય *ngx_slab_alloc_locked(ngx_slab_pool_t *pool, size_t size);
શૂન્ય *ngx_slab_calloc(ngx_slab_pool_t *pool, size_t size);
શૂન્ય *ngx_slab_calloc_locked(ngx_slab_pool_t *pool, size_t size);
શૂન્ય ngx_slab_free(ngx_slab_pool_t *pool, શૂન્ય *p);
શૂન્ય ngx_slab_free_locked(ngx_slab_pool_t *pool, શૂન્ય *p);


#endif /* _NGX_SLAB_H_INCLUDED_ */
