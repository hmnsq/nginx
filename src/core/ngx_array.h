
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_ARRAY_H_INCLUDED_
#define _NGX_ARRAY_H_INCLUDED_


#અન્તર્ભાવ <ngx_config.h>
#અન્તર્ભાવ <ngx_core.h>


પ્રકારવ્યાખ્યા સંરચના {
    શૂન્ય        *elts;
    ngx_uint_t   nelts;
    size_t       size;
    ngx_uint_t   nalloc;
    ngx_pool_t  *pool;
} ngx_array_t;


ngx_array_t *ngx_array_create(ngx_pool_t *p, ngx_uint_t n, size_t size);
શૂન્ય ngx_array_destroy(ngx_array_t *a);
શૂન્ય *ngx_array_ધકેલો(ngx_array_t *a);
શૂન્ય *ngx_array_ધકેલો_n(ngx_array_t *a, ngx_uint_t n);


સ્થિર ngx_inline ngx_int_t
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
    જો (array->elts == ખાલી) {
        પાછા NGX_ERROR;
    }

    પાછા NGX_OK;
}


#endif /* _NGX_ARRAY_H_INCLUDED_ */
