
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#অন্তর্ভাব <ngx_config.h>
#অন্তর্ভাব <ngx_core.h>


ngx_list_t *
ngx_list_create(ngx_pool_t *pool, ngx_uint_t n, আকার_t আকার_2)
{
    ngx_list_t  *list;

    list = ngx_palloc(pool, আকার(ngx_list_t));
    যদি (list == ফাঁকা) {
        ফেরত ফাঁকা;
    }

    যদি (ngx_list_init(list, pool, n, আকার_2) != NGX_OK) {
        ফেরত ফাঁকা;
    }

    ফেরত list;
}


শূন্য *
ngx_list_ঠেলো(ngx_list_t *l)
{
    শূন্য             *elt;
    ngx_list_part_t  *last;

    last = l->last;

    যদি (last->nelts == l->nalloc) {

        /* the last part is full, allocate a new list part */

        last = ngx_palloc(l->pool, আকার(ngx_list_part_t));
        যদি (last == ফাঁকা) {
            ফেরত ফাঁকা;
        }

        last->elts = ngx_palloc(l->pool, l->nalloc * l->আকার_2);
        যদি (last->elts == ফাঁকা) {
            ফেরত ফাঁকা;
        }

        last->nelts = 0;
        last->পরবর্তী = ফাঁকা;

        l->last->পরবর্তী = last;
        l->last = last;
    }

    elt = (অক্ষর *) last->elts + l->আকার_2 * last->nelts;
    last->nelts++;

    ফেরত elt;
}
