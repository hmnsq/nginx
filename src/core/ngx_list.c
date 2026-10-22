
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
    ܐܢ (list == عدم) {
        ܦܢܐ عدم;
    }

    ܐܢ (ngx_list_init(list, pool, n, size) != NGX_OK) {
        ܦܢܐ عدم;
    }

    ܦܢܐ list;
}


فراغ *
ngx_list_push(ngx_list_t *l)
{
    فراغ             *elt;
    ngx_list_part_t  *last;

    last = l->last;

    ܐܢ (last->nelts == l->nalloc) {

        /* the last part is full, allocate a new list part */

        last = ngx_palloc(l->pool, حجم(ngx_list_part_t));
        ܐܢ (last == عدم) {
            ܦܢܐ عدم;
        }

        last->elts = ngx_palloc(l->pool, l->nalloc * l->size);
        ܐܢ (last->elts == عدم) {
            ܦܢܐ عدم;
        }

        last->nelts = 0;
        last->next = عدم;

        l->last->next = last;
        l->last = last;
    }

    elt = (حرف *) last->elts + l->size * last->nelts;
    last->nelts++;

    ܦܢܐ elt;
}
