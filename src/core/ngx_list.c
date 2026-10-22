
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#અન્તર્ભાવ <ngx_config.h>
#અન્તર્ભાવ <ngx_core.h>


ngx_list_t *
ngx_list_create(ngx_pool_t *pool, ngx_uint_t n, size_t size)
{
    ngx_list_t  *list;

    list = ngx_palloc(pool, કદ(ngx_list_t));
    જો (list == ખાલી) {
        પાછા ખાલી;
    }

    જો (ngx_list_init(list, pool, n, size) != NGX_OK) {
        પાછા ખાલી;
    }

    પાછા list;
}


શૂન્ય *
ngx_list_ધકેલો(ngx_list_t *l)
{
    શૂન્ય             *elt;
    ngx_list_part_t  *last;

    last = l->last;

    જો (last->nelts == l->nalloc) {

        /* the last part is full, allocate a new list part */

        last = ngx_palloc(l->pool, કદ(ngx_list_part_t));
        જો (last == ખાલી) {
            પાછા ખાલી;
        }

        last->elts = ngx_palloc(l->pool, l->nalloc * l->size);
        જો (last->elts == ખાલી) {
            પાછા ખાલી;
        }

        last->nelts = 0;
        last->next = ખાલી;

        l->last->next = last;
        l->last = last;
    }

    elt = (અક્ષર *) last->elts + l->size * last->nelts;
    last->nelts++;

    પાછા elt;
}
