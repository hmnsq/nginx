
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_ARRAY_H_INCLUDED_
#define _NGX_ARRAY_H_INCLUDED_


#അന്തര്ഭാവ <ngx_config.h>
#അന്തര്ഭാവ <ngx_core.h>


തരനിർവചനം ഘടന {
    ശൂന്യം        *elts;
    ngx_uint_t   nelts;
    size_t       size;
    ngx_uint_t   nalloc;
    ngx_pool_t  *pool;
} ngx_array_t;


ngx_array_t *ngx_array_create(ngx_pool_t *p, ngx_uint_t n, size_t size);
ശൂന്യം ngx_array_destroy(ngx_array_t *a);
ശൂന്യം *ngx_array_തള്ളുക(ngx_array_t *a);
ശൂന്യം *ngx_array_തള്ളുക_n(ngx_array_t *a, ngx_uint_t n);


സ്ഥിതം ngx_inline ngx_int_t
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
    എങ്കിൽ (array->elts == ശൂന്യ) {
        തിരികെ NGX_ERROR;
    }

    തിരികെ NGX_OK;
}


#endif /* _NGX_ARRAY_H_INCLUDED_ */
