
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#यदि_अपरिभाषितम् _NGX_ARRAY_H_INCLUDED_
#परिभाषय _NGX_ARRAY_H_INCLUDED_


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


प्रकारनिर्देश संरचना {
    शून्य        *elts;
    ngx_uint_t   nelts;
    आकार_t       आकार;
    ngx_uint_t   nalloc;
    ngx_pool_t  *pool;
} ngx_array_t;


ngx_array_t *ngx_array_create(ngx_pool_t *p, ngx_uint_t n, आकार_t आकार);
शून्य ngx_array_destroy(ngx_array_t *a);
शून्य *ngx_array_प्रक्षिप(ngx_array_t *a);
शून्य *ngx_array_प्रक्षिप_n(ngx_array_t *a, ngx_uint_t n);


स्थिर ngx_inline ngx_int_t
ngx_array_init(ngx_array_t *array, ngx_pool_t *pool, ngx_uint_t n, आकार_t आकार)
{
    /*
     * set "array->nelts" before "array->elts", otherwise MSVC thinks
     * that "array->nelts" may be used without having been initialized
     */

    array->nelts = 0;
    array->आकार = आकार;
    array->nalloc = n;
    array->pool = pool;

    array->elts = ngx_palloc(pool, n * आकार);
    यदि (array->elts == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

    प्रत्यागम NGX_OK;
}


#यदि_अन्तः /* _NGX_ARRAY_H_INCLUDED_ */
