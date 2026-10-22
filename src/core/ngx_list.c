
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


ngx_list_t *
ngx_list_create(ngx_pool_t *pool, ngx_uint_t n, ناپ_t ناپ)
{
    ngx_list_t  *list;

    list = ngx_palloc(pool, حجم(ngx_list_t));
    إذا (list == عدم) {
        أرجع عدم;
    }

    إذا (ngx_list_init(list, pool, n, ناپ) != NGX_OK) {
        أرجع عدم;
    }

    أرجع list;
}


فراغ *
ngx_list_ڈالو(ngx_list_t *l)
{
    فراغ             *elt;
    ngx_list_part_t  *آخری;

    آخری = l->آخری;

    إذا (آخری->nelts == l->nalloc) {

        /* the last part is full, allocate a new list part */

        آخری = ngx_palloc(l->pool, حجم(ngx_list_part_t));
        إذا (آخری == عدم) {
            أرجع عدم;
        }

        آخری->elts = ngx_palloc(l->pool, l->nalloc * l->ناپ);
        إذا (آخری->elts == عدم) {
            أرجع عدم;
        }

        آخری->nelts = 0;
        آخری->اگلا = عدم;

        l->آخری->اگلا = آخری;
        l->آخری = آخری;
    }

    elt = (حرف *) آخری->elts + l->ناپ * آخری->nelts;
    آخری->nelts++;

    أرجع elt;
}
