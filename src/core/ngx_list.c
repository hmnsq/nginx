
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ଅନ୍ତର୍ଭାଵ <ngx_config.h>
#ଅନ୍ତର୍ଭାଵ <ngx_core.h>


ngx_list_t *
ngx_list_create(ngx_pool_t *pool, ngx_uint_t n, size_t size)
{
    ngx_list_t  *list;

    list = ngx_palloc(pool, ଆକାର(ngx_list_t));
    ଯଦି (list == ଖାଲି) {
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ଯଦି (ngx_list_init(list, pool, n, size) != NGX_OK) {
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ଫେରନ୍ତୁ list;
}


ଶୂନ୍ୟ *
ngx_list_ଠେଲ(ngx_list_t *l)
{
    ଶୂନ୍ୟ             *elt;
    ngx_list_part_t  *last;

    last = l->last;

    ଯଦି (last->nelts == l->nalloc) {

        /* the last part is full, allocate a new list part */

        last = ngx_palloc(l->pool, ଆକାର(ngx_list_part_t));
        ଯଦି (last == ଖାଲି) {
            ଫେରନ୍ତୁ ଖାଲି;
        }

        last->elts = ngx_palloc(l->pool, l->nalloc * l->size);
        ଯଦି (last->elts == ଖାଲି) {
            ଫେରନ୍ତୁ ଖାଲି;
        }

        last->nelts = 0;
        last->next = ଖାଲି;

        l->last->next = last;
        l->last = last;
    }

    elt = (ଅକ୍ଷର *) last->elts + l->size * last->nelts;
    last->nelts++;

    ଫେରନ୍ତୁ elt;
}
