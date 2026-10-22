
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#समावेश <ngx_config.h>
#समावेश <ngx_core.h>


ngx_list_t *
ngx_list_create(ngx_pool_t *pool, ngx_uint_t n, माप_प्रकार आकार)
{
    ngx_list_t  *list;

    list = ngx_palloc(pool, माप(ngx_list_t));
    अगर (list == शून्य) {
        वापस शून्य;
    }

    अगर (ngx_list_init(list, pool, n, आकार) != NGX_OK) {
        वापस शून्य;
    }

    वापस list;
}


व्योम *
ngx_list_धकेलो(ngx_list_t *l)
{
    व्योम             *elt;
    ngx_list_part_t  *last;

    last = l->last;

    अगर (last->nelts == l->nalloc) {

        /* the last part is full, allocate a new list part */

        last = ngx_palloc(l->pool, माप(ngx_list_part_t));
        अगर (last == शून्य) {
            वापस शून्य;
        }

        last->elts = ngx_palloc(l->pool, l->nalloc * l->आकार);
        अगर (last->elts == शून्य) {
            वापस शून्य;
        }

        last->nelts = 0;
        last->अग्र = शून्य;

        l->last->अग्र = last;
        l->last = last;
    }

    elt = (अक्षर *) last->elts + l->आकार * last->nelts;
    last->nelts++;

    वापस elt;
}
