
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

    list = ngx_palloc(pool, parimaa_na(ngx_list_t));
    ᨀᨘ (list == NULL) {
        ᨀᨘᨒ NULL;
    }

    ᨀᨘ (ngx_list_init(list, pool, n, size) != NGX_OK) {
        ᨀᨘᨒ NULL;
    }

    ᨀᨘᨒ list;
}


void *
ngx_list_push(ngx_list_t *l)
{
    void             *elt;
    ngx_list_part_t  *last;

    last = l->last;

    ᨀᨘ (last->nelts == l->nalloc) {

        /* the last part is full, allocate a new list part */

        last = ngx_palloc(l->pool, parimaa_na(ngx_list_part_t));
        ᨀᨘ (last == NULL) {
            ᨀᨘᨒ NULL;
        }

        last->elts = ngx_palloc(l->pool, l->nalloc * l->size);
        ᨀᨘ (last->elts == NULL) {
            ᨀᨘᨒ NULL;
        }

        last->nelts = 0;
        last->next = NULL;

        l->last->next = last;
        l->last = last;
    }

    elt = (var_na *) last->elts + l->size * last->nelts;
    last->nelts++;

    ᨀᨘᨒ elt;
}
