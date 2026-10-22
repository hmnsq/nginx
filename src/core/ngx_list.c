
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#அந்தர்பாவ <ngx_config.h>
#அந்தர்பாவ <ngx_core.h>


ngx_list_t *
ngx_list_create(ngx_pool_t *pool, ngx_uint_t n, size_t size)
{
    ngx_list_t  *list;

    list = ngx_palloc(pool, அளவு(ngx_list_t));
    எனில் (list == வெற்று) {
        திரும்பு வெற்று;
    }

    எனில் (ngx_list_init(list, pool, n, size) != NGX_OK) {
        திரும்பு வெற்று;
    }

    திரும்பு list;
}


வெறுமை *
ngx_list_தள்ளு(ngx_list_t *l)
{
    வெறுமை             *elt;
    ngx_list_part_t  *last;

    last = l->last;

    எனில் (last->nelts == l->nalloc) {

        /* the last part is full, allocate a new list part */

        last = ngx_palloc(l->pool, அளவு(ngx_list_part_t));
        எனில் (last == வெற்று) {
            திரும்பு வெற்று;
        }

        last->elts = ngx_palloc(l->pool, l->nalloc * l->size);
        எனில் (last->elts == வெற்று) {
            திரும்பு வெற்று;
        }

        last->nelts = 0;
        last->next = வெற்று;

        l->last->next = last;
        l->last = last;
    }

    elt = (எழுத்து *) last->elts + l->size * last->nelts;
    last->nelts++;

    திரும்பு elt;
}
