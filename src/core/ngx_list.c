
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


ngx_list_t *
ngx_list_create(ngx_pool_t *pool, ngx_uint_t n, size_t size)
{
    ngx_list_t  *list;

    list = ngx_palloc(pool, परिमाण(ngx_list_t));
    यदि (list == रिक्त) {
        प्रत्यागम रिक्त;
    }

    यदि (ngx_list_init(list, pool, n, size) != NGX_OK) {
        प्रत्यागम रिक्त;
    }

    प्रत्यागम list;
}


शून्य *
ngx_list_धकेलो(ngx_list_t *l)
{
    शून्य             *elt;
    ngx_list_part_t  *last;

    last = l->last;

    यदि (last->nelts == l->nalloc) {

        /* the last part is full, allocate a new list part */

        last = ngx_palloc(l->pool, परिमाण(ngx_list_part_t));
        यदि (last == रिक्त) {
            प्रत्यागम रिक्त;
        }

        last->elts = ngx_palloc(l->pool, l->nalloc * l->size);
        यदि (last->elts == रिक्त) {
            प्रत्यागम रिक्त;
        }

        last->nelts = 0;
        last->next = रिक्त;

        l->last->next = last;
        l->last = last;
    }

    elt = (वर्ण *) last->elts + l->size * last->nelts;
    last->nelts++;

    प्रत्यागम elt;
}
