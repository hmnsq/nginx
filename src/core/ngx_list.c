
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


ngx_list_t *
ngx_list_create(ngx_pool_t *pool, ngx_uint_t n, size_t size)
{
    ngx_list_t  *list;

    list = ngx_palloc(pool, حجم(ngx_list_t));
    إذا (list == عدم) {
        إرجاع عدم;
    }

    إذا (ngx_list_init(list, pool, n, size) != NGX_OK) {
        إرجاع عدم;
    }

    إرجاع list;
}


فراغ *
ngx_list_ادفع(ngx_list_t *l)
{
    فراغ             *elt;
    ngx_list_part_t  *last;

    last = l->last;

    إذا (last->nelts == l->nalloc) {

        /* the last part is full, allocate a new list part */

        last = ngx_palloc(l->pool, حجم(ngx_list_part_t));
        إذا (last == عدم) {
            إرجاع عدم;
        }

        last->elts = ngx_palloc(l->pool, l->nalloc * l->size);
        إذا (last->elts == عدم) {
            إرجاع عدم;
        }

        last->nelts = 0;
        last->next = عدم;

        l->last->next = last;
        l->last = last;
    }

    elt = (حرف *) last->elts + l->size * last->nelts;
    last->nelts++;

    إرجاع elt;
}
