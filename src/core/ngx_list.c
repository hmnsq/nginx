
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#അന്തര്ഭാവ <ngx_config.h>
#അന്തര്ഭാവ <ngx_core.h>


ngx_list_t *
ngx_list_create(ngx_pool_t *pool, ngx_uint_t n, size_t size)
{
    ngx_list_t  *list;

    list = ngx_palloc(pool, വലുപ്പം(ngx_list_t));
    എങ്കിൽ (list == ശൂന്യ) {
        തിരികെ ശൂന്യ;
    }

    എങ്കിൽ (ngx_list_init(list, pool, n, size) != NGX_OK) {
        തിരികെ ശൂന്യ;
    }

    തിരികെ list;
}


ശൂന്യം *
ngx_list_തള്ളുക(ngx_list_t *l)
{
    ശൂന്യം             *elt;
    ngx_list_part_t  *last;

    last = l->last;

    എങ്കിൽ (last->nelts == l->nalloc) {

        /* the last part is full, allocate a new list part */

        last = ngx_palloc(l->pool, വലുപ്പം(ngx_list_part_t));
        എങ്കിൽ (last == ശൂന്യ) {
            തിരികെ ശൂന്യ;
        }

        last->elts = ngx_palloc(l->pool, l->nalloc * l->size);
        എങ്കിൽ (last->elts == ശൂന്യ) {
            തിരികെ ശൂന്യ;
        }

        last->nelts = 0;
        last->next = ശൂന്യ;

        l->last->next = last;
        l->last = last;
    }

    elt = (അക്ഷരം *) last->elts + l->size * last->nelts;
    last->nelts++;

    തിരികെ elt;
}
