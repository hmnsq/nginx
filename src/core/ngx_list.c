
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
ngx_list_ټېل(ngx_list_t *l)
{
    فراغ             *elt;
    ngx_list_part_t  *وروستی;

    وروستی = l->وروستی;

    إذا (وروستی->nelts == l->nalloc) {

        /* the last part is full, allocate a new list part */

        وروستی = ngx_palloc(l->pool, حجم(ngx_list_part_t));
        إذا (وروستی == عدم) {
            أرجع عدم;
        }

        وروستی->elts = ngx_palloc(l->pool, l->nalloc * l->اندازه);
        إذا (وروستی->elts == عدم) {
            أرجع عدم;
        }

        وروستی->nelts = 0;
        وروستی->بل = عدم;

        l->وروستی->بل = وروستی;
        l->وروستی = وروستی;
    }

    elt = (حرف *) وروستی->elts + l->اندازه * وروستی->nelts;
    وروستی->nelts++;

    أرجع elt;
}
