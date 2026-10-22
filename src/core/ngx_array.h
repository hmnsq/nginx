
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ _NGX_ARRAY_H_INCLUDED_
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _NGX_ARRAY_H_INCLUDED_


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ਖਾਲੀ        *elts;
    ngx_uint_t   nelts;
    ਮਾਪ_t       ਮਾਪ;
    ngx_uint_t   nalloc;
    ngx_pool_t  *pool;
} ngx_array_t;


ngx_array_t *ngx_array_create(ngx_pool_t *p, ngx_uint_t n, ਮਾਪ_t ਮਾਪ);
ਖਾਲੀ ngx_array_destroy(ngx_array_t *a);
ਖਾਲੀ *ngx_array_ਧੱਕੋ(ngx_array_t *a);
ਖਾਲੀ *ngx_array_ਧੱਕੋ_n(ngx_array_t *a, ngx_uint_t n);


ਸਥਿਰ ngx_inline ngx_int_t
ngx_array_init(ngx_array_t *array, ngx_pool_t *pool, ngx_uint_t n, ਮਾਪ_t ਮਾਪ)
{
    /*
     * set "array->nelts" before "array->elts", otherwise MSVC thinks
     * that "array->nelts" may be used without having been initialized
     */

    array->nelts = 0;
    array->ਮਾਪ = ਮਾਪ;
    array->nalloc = n;
    array->pool = pool;

    array->elts = ngx_palloc(pool, n * ਮਾਪ);
    ਜੇ (array->elts == ਨਿਰਾ) {
        ਵਾਪਸ NGX_ERROR;
    }

    ਵਾਪਸ NGX_OK;
}


#ਜੇ_ਅੰਤ /* _NGX_ARRAY_H_INCLUDED_ */
