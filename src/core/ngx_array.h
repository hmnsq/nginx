
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#నిర్వచితం_కాకపోతే _NGX_ARRAY_H_INCLUDED_
#నిర్వచించు _NGX_ARRAY_H_INCLUDED_


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>


రకనిర్వచనం నిర్మాణం {
    శూన్య        *elts;
    ngx_uint_t   nelts;
    కొలత_t       కొలత;
    ngx_uint_t   nalloc;
    ngx_pool_t  *pool;
} ngx_array_t;


ngx_array_t *ngx_array_create(ngx_pool_t *p, ngx_uint_t n, కొలత_t కొలత);
శూన్య ngx_array_destroy(ngx_array_t *a);
శూన్య *ngx_array_నెట్టు(ngx_array_t *a);
శూన్య *ngx_array_నెట్టు_n(ngx_array_t *a, ngx_uint_t n);


స్థిర ngx_inline ngx_int_t
ngx_array_init(ngx_array_t *array, ngx_pool_t *pool, ngx_uint_t n, కొలత_t కొలత)
{
    /*
     * set "array->nelts" before "array->elts", otherwise MSVC thinks
     * that "array->nelts" may be used without having been initialized
     */

    array->nelts = 0;
    array->కొలత = కొలత;
    array->nalloc = n;
    array->pool = pool;

    array->elts = ngx_palloc(pool, n * కొలత);
    ఒకవేళ (array->elts == ఖాళీ) {
        తిరిగి NGX_ERROR;
    }

    తిరిగి NGX_OK;
}


#అయితే_ముగింపు /* _NGX_ARRAY_H_INCLUDED_ */
