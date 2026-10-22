
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ _NGX_ARRAY_H_INCLUDED_
#ವ್ಯಾಖ್ಯಾನಿಸು _NGX_ARRAY_H_INCLUDED_


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ಶೂನ್ಯ        *elts;
    ngx_uint_t   nelts;
    ಗಾತ್ರ_t       ಗಾತ್ರ_2;
    ngx_uint_t   nalloc;
    ngx_pool_t  *pool;
} ngx_array_t;


ngx_array_t *ngx_array_create(ngx_pool_t *p, ngx_uint_t n, ಗಾತ್ರ_t ಗಾತ್ರ_2);
ಶೂನ್ಯ ngx_array_destroy(ngx_array_t *a);
ಶೂನ್ಯ *ngx_array_ತಳ್ಳು(ngx_array_t *a);
ಶೂನ್ಯ *ngx_array_ತಳ್ಳು_n(ngx_array_t *a, ngx_uint_t n);


ಸ್ಥಿರ ngx_inline ngx_int_t
ngx_array_init(ngx_array_t *array, ngx_pool_t *pool, ngx_uint_t n, ಗಾತ್ರ_t ಗಾತ್ರ_2)
{
    /*
     * set "array->nelts" before "array->elts", otherwise MSVC thinks
     * that "array->nelts" may be used without having been initialized
     */

    array->nelts = 0;
    array->ಗಾತ್ರ_2 = ಗಾತ್ರ_2;
    array->nalloc = n;
    array->pool = pool;

    array->elts = ngx_palloc(pool, n * ಗಾತ್ರ_2);
    ಆದರೆ (array->elts == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    ಹಿಂತಿರುಗಿ NGX_OK;
}


#ಆದರೆ_ಅಂತ್ಯ /* _NGX_ARRAY_H_INCLUDED_ */
