
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_ARRAY_H_INCLUDED_
#define _NGX_ARRAY_H_INCLUDED_


#ଅନ୍ତର୍ଭାଵ <ngx_config.h>
#ଅନ୍ତର୍ଭାଵ <ngx_core.h>


ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ଶୂନ୍ୟ        *elts;
    ngx_uint_t   nelts;
    size_t       size;
    ngx_uint_t   nalloc;
    ngx_pool_t  *pool;
} ngx_array_t;


ngx_array_t *ngx_array_create(ngx_pool_t *p, ngx_uint_t n, size_t size);
ଶୂନ୍ୟ ngx_array_destroy(ngx_array_t *a);
ଶୂନ୍ୟ *ngx_array_ଠେଲ(ngx_array_t *a);
ଶୂନ୍ୟ *ngx_array_ଠେଲ_n(ngx_array_t *a, ngx_uint_t n);


ସ୍ଥିର ngx_inline ngx_int_t
ngx_array_init(ngx_array_t *array, ngx_pool_t *pool, ngx_uint_t n, size_t size)
{
    /*
     * set "array->nelts" before "array->elts", otherwise MSVC thinks
     * that "array->nelts" may be used without having been initialized
     */

    array->nelts = 0;
    array->size = size;
    array->nalloc = n;
    array->pool = pool;

    array->elts = ngx_palloc(pool, n * size);
    ଯଦି (array->elts == ଖାଲି) {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    ଫେରନ୍ତୁ NGX_OK;
}


#endif /* _NGX_ARRAY_H_INCLUDED_ */
