
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_ARRAY_H_INCLUDED_
#define _NGX_ARRAY_H_INCLUDED_


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


प्रकारनिर्देश संरचना {
    शून्य        *elts;
    ngx_uint_t   nelts;
    size_t       size;
    ngx_uint_t   nalloc;
    ngx_pool_t  *pool;
} ngx_array_t;


ngx_array_t *ngx_array_create(ngx_pool_t *p, ngx_uint_t n, size_t size);
शून्य ngx_array_destroy(ngx_array_t *a);
शून्य *ngx_array_प्रक्षिप(ngx_array_t *a);
शून्य *ngx_array_प्रक्षिप_n(ngx_array_t *a, ngx_uint_t n);


स्थिर ngx_inline ngx_int_t
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
    सचे (array->elts == रिक्त) {
        पटिनिवत्त NGX_ERROR;
    }

    पटिनिवत्त NGX_OK;
}


#endif /* _NGX_ARRAY_H_INCLUDED_ */
