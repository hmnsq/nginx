
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_ARRAY_H_INCLUDED_
#define _NGX_ARRAY_H_INCLUDED_


#அந்தர்பாவ <ngx_config.h>
#அந்தர்பாவ <ngx_core.h>


வகைவரை கட்டமைப்பு {
    வெறுமை        *elts;
    ngx_uint_t   nelts;
    size_t       size;
    ngx_uint_t   nalloc;
    ngx_pool_t  *pool;
} ngx_array_t;


ngx_array_t *ngx_array_create(ngx_pool_t *p, ngx_uint_t n, size_t size);
வெறுமை ngx_array_destroy(ngx_array_t *a);
வெறுமை *ngx_array_தள்ளு(ngx_array_t *a);
வெறுமை *ngx_array_தள்ளு_n(ngx_array_t *a, ngx_uint_t n);


நிலையான ngx_inline ngx_int_t
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
    எனில் (array->elts == வெற்று) {
        திரும்பு NGX_ERROR;
    }

    திரும்பு NGX_OK;
}


#endif /* _NGX_ARRAY_H_INCLUDED_ */
