
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


ngx_list_t *
ngx_list_create(ngx_pool_t *pool, ngx_uint_t n, माप_प्रकार size)
{
    ngx_list_t  *list;

    list = ngx_palloc(pool, आकार(ngx_list_t));
    जर (list == रिक्त) {
        परत रिक्त;
    }

    जर (ngx_list_init(list, pool, n, size) != NGX_OK) {
        परत रिक्त;
    }

    परत list;
}


शून्य *
ngx_list_ढकला(ngx_list_t *l)
{
    शून्य             *elt;
    ngx_list_part_t  *last;

    last = l->last;

    जर (last->nelts == l->nalloc) {

        /* the last part is full, allocate a new list part */

        last = ngx_palloc(l->pool, आकार(ngx_list_part_t));
        जर (last == रिक्त) {
            परत रिक्त;
        }

        last->elts = ngx_palloc(l->pool, l->nalloc * l->size);
        जर (last->elts == रिक्त) {
            परत रिक्त;
        }

        last->nelts = 0;
        last->next = रिक्त;

        l->last->next = last;
        l->last = last;
    }

    elt = (अक्षर *) last->elts + l->size * last->nelts;
    last->nelts++;

    परत elt;
}
