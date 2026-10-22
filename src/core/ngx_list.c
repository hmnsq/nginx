
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#شامل <ngx_config.h>
#شامل <ngx_core.h>


ngx_list_t *
ngx_list_create(ngx_pool_t *pool, ngx_uint_t n, size_t size)
{
    ngx_list_t  *list;

    list = ngx_palloc(pool, اندازه(ngx_list_t));
    اگر (list == پوچ) {
        بازگشت پوچ;
    }

    اگر (ngx_list_init(list, pool, n, size) != NGX_OK) {
        بازگشت پوچ;
    }

    بازگشت list;
}


تهی *
ngx_list_افزودن(ngx_list_t *l)
{
    تهی             *elt;
    ngx_list_part_t  *last;

    last = l->last;

    اگر (last->nelts == l->nalloc) {

        /* the last part is full, allocate a new list part */

        last = ngx_palloc(l->pool, اندازه(ngx_list_part_t));
        اگر (last == پوچ) {
            بازگشت پوچ;
        }

        last->elts = ngx_palloc(l->pool, l->nalloc * l->size);
        اگر (last->elts == پوچ) {
            بازگشت پوچ;
        }

        last->nelts = 0;
        last->next = پوچ;

        l->last->next = last;
        l->last = last;
    }

    elt = (نویسه *) last->elts + l->size * last->nelts;
    last->nelts++;

    بازگشت elt;
}
