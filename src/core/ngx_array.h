
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#جے_بنا_تعریف _NGX_ARRAY_H_INCLUDED_
#تعریف_کرو _NGX_ARRAY_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


تعريفنوع بنية {
    فراغ        *elts;
    ngx_uint_t   nelts;
    ناپ_t       ناپ;
    ngx_uint_t   nalloc;
    ngx_pool_t  *pool;
} ngx_array_t;


ngx_array_t *ngx_array_create(ngx_pool_t *p, ngx_uint_t n, ناپ_t ناپ);
فراغ ngx_array_destroy(ngx_array_t *a);
فراغ *ngx_array_ڈالو(ngx_array_t *a);
فراغ *ngx_array_ڈالو_n(ngx_array_t *a, ngx_uint_t n);


ساكن ngx_inline ngx_int_t
ngx_array_init(ngx_array_t *array, ngx_pool_t *pool, ngx_uint_t n, ناپ_t ناپ)
{
    /*
     * set "array->nelts" before "array->elts", otherwise MSVC thinks
     * that "array->nelts" may be used without having been initialized
     */

    array->nelts = 0;
    array->ناپ = ناپ;
    array->nalloc = n;
    array->pool = pool;

    array->elts = ngx_palloc(pool, n * ناپ);
    إذا (array->elts == عدم) {
        أرجع NGX_ERROR;
    }

    أرجع NGX_OK;
}


#جے_انت /* _NGX_ARRAY_H_INCLUDED_ */
