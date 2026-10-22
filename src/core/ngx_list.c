
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#شامل <ngx_config.h>
#شامل <ngx_core.h>


ngx_list_t *
ngx_list_create(ngx_pool_t *pool, ngx_uint_t n, سائز_t سائز)
{
    ngx_list_t  *list;

    list = ngx_palloc(pool, حجم(ngx_list_t));
    اگر (list == تہی) {
        واپس تہی;
    }

    اگر (ngx_list_init(list, pool, n, سائز) != NGX_OK) {
        واپس تہی;
    }

    واپس list;
}


خالی *
ngx_list_ڈالو(ngx_list_t *l)
{
    خالی             *elt;
    ngx_list_part_t  *آخری;

    آخری = l->آخری;

    اگر (آخری->nelts == l->nalloc) {

        /* the last part is full, allocate a new list part */

        آخری = ngx_palloc(l->pool, حجم(ngx_list_part_t));
        اگر (آخری == تہی) {
            واپس تہی;
        }

        آخری->elts = ngx_palloc(l->pool, l->nalloc * l->سائز);
        اگر (آخری->elts == تہی) {
            واپس تہی;
        }

        آخری->nelts = 0;
        آخری->اگلا = تہی;

        l->آخری->اگلا = آخری;
        l->آخری = آخری;
    }

    elt = (حرف *) آخری->elts + l->سائز * آخری->nelts;
    آخری->nelts++;

    واپس elt;
}
