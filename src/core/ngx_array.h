
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#اگر_تعریف_نشده _NGX_ARRAY_H_INCLUDED_
#تعریف_کن _NGX_ARRAY_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


تعريفنوع بنية {
    فراغ        *elts;
    ngx_uint_t   nelts;
    اندازه_t       اندازه;
    ngx_uint_t   nalloc;
    ngx_pool_t  *pool;
} ngx_array_t;


ngx_array_t *ngx_array_create(ngx_pool_t *p, ngx_uint_t n, اندازه_t اندازه);
فراغ ngx_array_destroy(ngx_array_t *a);
فراغ *ngx_array_افزودن(ngx_array_t *a);
فراغ *ngx_array_افزودن_n(ngx_array_t *a, ngx_uint_t n);


ساكن ngx_inline ngx_int_t
ngx_array_init(ngx_array_t *array, ngx_pool_t *pool, ngx_uint_t n, اندازه_t اندازه)
{
    /*
     * set "array->nelts" before "array->elts", otherwise MSVC thinks
     * that "array->nelts" may be used without having been initialized
     */

    array->nelts = 0;
    array->اندازه = اندازه;
    array->nalloc = n;
    array->pool = pool;

    array->elts = ngx_palloc(pool, n * اندازه);
    إذا (array->elts == عدم) {
        أرجع NGX_ERROR;
    }

    أرجع NGX_OK;
}


#پایان_اگر /* _NGX_ARRAY_H_INCLUDED_ */
