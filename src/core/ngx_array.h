
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_ARRAY_H_INCLUDED_
#define _NGX_ARRAY_H_INCLUDED_


#অন্তর্ভাব <ngx_config.h>
#অন্তর্ভাব <ngx_core.h>


প্রকারলেখ কাঠামো {
    শূন্য        *elts;
    ngx_uint_t   nelts;
    আকার_t       আকার_2;
    ngx_uint_t   nalloc;
    ngx_pool_t  *pool;
} ngx_array_t;


ngx_array_t *ngx_array_create(ngx_pool_t *p, ngx_uint_t n, আকার_t আকার_2);
শূন্য ngx_array_destroy(ngx_array_t *a);
শূন্য *ngx_array_ঠেলো(ngx_array_t *a);
শূন্য *ngx_array_ঠেলো_n(ngx_array_t *a, ngx_uint_t n);


স্থির ngx_inline ngx_int_t
ngx_array_init(ngx_array_t *array, ngx_pool_t *pool, ngx_uint_t n, আকার_t আকার_2)
{
    /*
     * set "array->nelts" before "array->elts", otherwise MSVC thinks
     * that "array->nelts" may be used without having been initialized
     */

    array->nelts = 0;
    array->আকার_2 = আকার_2;
    array->nalloc = n;
    array->pool = pool;

    array->elts = ngx_palloc(pool, n * আকার_2);
    যদি (array->elts == ফাঁকা) {
        ফেরত NGX_ERROR;
    }

    ফেরত NGX_OK;
}


#endif /* _NGX_ARRAY_H_INCLUDED_ */
