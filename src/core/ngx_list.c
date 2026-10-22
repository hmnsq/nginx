
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#অন্তর্ভাৱ <ngx_config.h>
#অন্তর্ভাৱ <ngx_core.h>


ngx_list_t *
ngx_list_create(ngx_pool_t *pool, ngx_uint_t n, size_t size)
{
    ngx_list_t  *list;

    list = ngx_palloc(pool, পরিমাণ(ngx_list_t));
    যদি (list == রিক্ত) {
        ঘূৰাই রিক্ত;
    }

    যদি (ngx_list_init(list, pool, n, size) != NGX_OK) {
        ঘূৰাই রিক্ত;
    }

    ঘূৰাই list;
}


শূন্য *
ngx_list_ঠেলো(ngx_list_t *l)
{
    শূন্য             *elt;
    ngx_list_part_t  *last;

    last = l->last;

    যদি (last->nelts == l->nalloc) {

        /* the last part is full, allocate a new list part */

        last = ngx_palloc(l->pool, পরিমাণ(ngx_list_part_t));
        যদি (last == রিক্ত) {
            ঘূৰাই রিক্ত;
        }

        last->elts = ngx_palloc(l->pool, l->nalloc * l->size);
        যদি (last->elts == রিক্ত) {
            ঘূৰাই রিক্ত;
        }

        last->nelts = 0;
        last->next = রিক্ত;

        l->last->next = last;
        l->last = last;
    }

    elt = (ৱর্ণ *) last->elts + l->size * last->nelts;
    last->nelts++;

    ঘূৰাই elt;
}
