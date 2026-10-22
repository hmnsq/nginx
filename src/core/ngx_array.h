
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_ARRAY_H_INCLUDED_
#define _NGX_ARRAY_H_INCLUDED_


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    ಶೂನ್ಯ        *elts;
    ngx_uint_t   nelts;
    size_t       size;
    ngx_uint_t   nalloc;
    ngx_pool_t  *pool;
} ngx_array_t;


ngx_array_t *ngx_array_create(ngx_pool_t *p, ngx_uint_t n, size_t size);
ಶೂನ್ಯ ngx_array_destroy(ngx_array_t *a);
ಶೂನ್ಯ *ngx_array_ತಳ್ಳು(ngx_array_t *a);
ಶೂನ್ಯ *ngx_array_ತಳ್ಳು_n(ngx_array_t *a, ngx_uint_t n);


ಸ್ಥಿರ ngx_inline ngx_int_t
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
    ಯದಿ (array->elts == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    ಪ್ರತ್ಯಾಗಮ NGX_OK;
}


#endif /* _NGX_ARRAY_H_INCLUDED_ */
