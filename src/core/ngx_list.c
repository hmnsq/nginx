
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


ngx_list_t *
ngx_list_create(ngx_pool_t *pool, ngx_uint_t n, سائيز_t سائيز)
{
    ngx_list_t  *list;

    list = ngx_palloc(pool, حجم(ngx_list_t));
    إذا (list == عدم) {
        أرجع عدم;
    }

    إذا (ngx_list_init(list, pool, n, سائيز) != NGX_OK) {
        أرجع عدم;
    }

    أرجع list;
}


فراغ *
ngx_list_ڌڪو(ngx_list_t *l)
{
    فراغ             *elt;
    ngx_list_part_t  *آخري;

    آخري = l->آخري;

    إذا (آخري->nelts == l->nalloc) {

        /* the last part is full, allocate a new list part */

        آخري = ngx_palloc(l->pool, حجم(ngx_list_part_t));
        إذا (آخري == عدم) {
            أرجع عدم;
        }

        آخري->elts = ngx_palloc(l->pool, l->nalloc * l->سائيز);
        إذا (آخري->elts == عدم) {
            أرجع عدم;
        }

        آخري->nelts = 0;
        آخري->ايندڙ = عدم;

        l->آخري->ايندڙ = آخري;
        l->آخري = آخري;
    }

    elt = (حرف *) آخري->elts + l->سائيز * آخري->nelts;
    آخري->nelts++;

    أرجع elt;
}
