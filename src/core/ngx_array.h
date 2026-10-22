
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_ARRAY_H_INCLUDED_
#define _NGX_ARRAY_H_INCLUDED_


#অন্তর্ভাৱ <ngx_config.h>
#অন্তর্ভাৱ <ngx_core.h>


প্রকারনির্দেশ সংরচনা {
    শূন্য        *elts;
    ngx_uint_t   nelts;
    size_t       size;
    ngx_uint_t   nalloc;
    ngx_pool_t  *pool;
} ngx_array_t;


ngx_array_t *ngx_array_create(ngx_pool_t *p, ngx_uint_t n, size_t size);
শূন্য ngx_array_destroy(ngx_array_t *a);
শূন্য *ngx_array_ঠেলো(ngx_array_t *a);
শূন্য *ngx_array_ঠেলো_n(ngx_array_t *a, ngx_uint_t n);


স্থির ngx_inline ngx_int_t
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
    যদি (array->elts == রিক্ত) {
        ঘূৰাই NGX_ERROR;
    }

    ঘূৰাই NGX_OK;
}


#endif /* _NGX_ARRAY_H_INCLUDED_ */
