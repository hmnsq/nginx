
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अगर_अघोषित _NGX_ARRAY_H_INCLUDED_
#घोषणा _NGX_ARRAY_H_INCLUDED_


#समावेश <ngx_config.h>
#समावेश <ngx_core.h>


प्रकार काष्ठा {
    व्योम        *elts;
    ngx_uint_t   nelts;
    माप_प्रकार       आकार;
    ngx_uint_t   nalloc;
    ngx_pool_t  *pool;
} ngx_array_t;


ngx_array_t *ngx_array_create(ngx_pool_t *p, ngx_uint_t n, माप_प्रकार आकार);
व्योम ngx_array_destroy(ngx_array_t *a);
व्योम *ngx_array_धकेलो(ngx_array_t *a);
व्योम *ngx_array_धकेलो_n(ngx_array_t *a, ngx_uint_t n);


अटल ngx_inline ngx_int_t
ngx_array_init(ngx_array_t *array, ngx_pool_t *pool, ngx_uint_t n, माप_प्रकार आकार)
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
    अगर (array->elts == शून्य) {
        वापस NGX_ERROR;
    }

    वापस NGX_OK;
}


#पूर्ण_अगर /* _NGX_ARRAY_H_INCLUDED_ */
