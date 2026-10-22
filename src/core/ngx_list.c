
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


ngx_list_t *
ngx_list_create(ngx_pool_t *pool, ngx_uint_t n, اندازه_t اندازه)
{
    ngx_list_t  *list;

    list = ngx_palloc(pool, حجم(ngx_list_t));
    إذا (list == عدم) {
        أرجع عدم;
    }

    إذا (ngx_list_init(list, pool, n, اندازه) != NGX_OK) {
        أرجع عدم;
    }

    أرجع list;
}


فراغ *
ngx_list_افزودن(ngx_list_t *l)
{
    فراغ             *elt;
    ngx_list_part_t  *آخرین;

    آخرین = l->آخرین;

    إذا (آخرین->nelts == l->nalloc) {

        /* the last part is full, allocate a new list part */

        آخرین = ngx_palloc(l->pool, حجم(ngx_list_part_t));
        إذا (آخرین == عدم) {
            أرجع عدم;
        }

        آخرین->elts = ngx_palloc(l->pool, l->nalloc * l->اندازه);
        إذا (آخرین->elts == عدم) {
            أرجع عدم;
        }

        آخرین->nelts = 0;
        آخرین->بعدی = عدم;

        l->آخرین->بعدی = آخرین;
        l->آخرین = آخرین;
    }

    elt = (حرف *) آخرین->elts + l->اندازه * آخرین->nelts;
    آخرین->nelts++;

    أرجع elt;
}
