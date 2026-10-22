
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#اگر_غیر_تعریف_شدہ _NGX_ARRAY_H_INCLUDED_
#تعریف_کرو _NGX_ARRAY_H_INCLUDED_


#شامل <ngx_config.h>
#شامل <ngx_core.h>


قسمتعریف ساخت {
    خالی        *elts;
    ngx_uint_t   nelts;
    سائز_t       سائز;
    ngx_uint_t   nalloc;
    ngx_pool_t  *pool;
} ngx_array_t;


ngx_array_t *ngx_array_create(ngx_pool_t *p, ngx_uint_t n, سائز_t سائز);
خالی ngx_array_destroy(ngx_array_t *a);
خالی *ngx_array_ڈالو(ngx_array_t *a);
خالی *ngx_array_ڈالو_n(ngx_array_t *a, ngx_uint_t n);


ساکن ngx_inline ngx_int_t
ngx_array_init(ngx_array_t *array, ngx_pool_t *pool, ngx_uint_t n, سائز_t سائز)
{
    /*
     * set "array->nelts" before "array->elts", otherwise MSVC thinks
     * that "array->nelts" may be used without having been initialized
     */

    array->nelts = 0;
    array->سائز = سائز;
    array->nalloc = n;
    array->pool = pool;

    array->elts = ngx_palloc(pool, n * سائز);
    اگر (array->elts == تہی) {
        واپس NGX_ERROR;
    }

    واپس NGX_OK;
}


#اگر_ختم /* _NGX_ARRAY_H_INCLUDED_ */
