
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>


ngx_list_t *
ngx_list_create(ngx_pool_t *pool, ngx_uint_t n, size_t size)
{
    ngx_list_t  *list;

    list = ngx_palloc(pool, sizeof(ngx_list_t));
    如果 (list == NULL) {
        返回 NULL;
    }

    如果 (ngx_list_init(list, pool, n, size) != NGX_OK) {
        返回 NULL;
    }

    返回 list;
}


void *
ngx_list_push(ngx_list_t *l)
{
    void             *elt;
    ngx_list_part_t  *last;

    last = l->last;

    如果 (last->nelts == l->nalloc) {

        /* the last part is full, allocate a new list part */

        last = ngx_palloc(l->pool, sizeof(ngx_list_part_t));
        如果 (last == NULL) {
            返回 NULL;
        }

        last->elts = ngx_palloc(l->pool, l->nalloc * l->size);
        如果 (last->elts == NULL) {
            返回 NULL;
        }

        last->nelts = 0;
        last->next = NULL;

        l->last->next = last;
        l->last = last;
    }

    elt = (char *) last->elts + l->size * last->nelts;
    last->nelts++;

    返回 elt;
}
