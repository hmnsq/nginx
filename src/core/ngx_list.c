
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


ngx_list_t *
ngx_list_create(ngx_pool_t *pool, ngx_uint_t n, size_t size)
{
    ngx_list_t  *list;

    list = ngx_palloc(pool, ಪರಿಮಾಣ(ngx_list_t));
    ಯದಿ (list == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ಯದಿ (ngx_list_init(list, pool, n, size) != NGX_OK) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ಪ್ರತ್ಯಾಗಮ list;
}


ಶೂನ್ಯ *
ngx_list_ತಳ್ಳು(ngx_list_t *l)
{
    ಶೂನ್ಯ             *elt;
    ngx_list_part_t  *last;

    last = l->last;

    ಯದಿ (last->nelts == l->nalloc) {

        /* the last part is full, allocate a new list part */

        last = ngx_palloc(l->pool, ಪರಿಮಾಣ(ngx_list_part_t));
        ಯದಿ (last == ರಿಕ್ತ) {
            ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
        }

        last->elts = ngx_palloc(l->pool, l->nalloc * l->size);
        ಯದಿ (last->elts == ರಿಕ್ತ) {
            ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
        }

        last->nelts = 0;
        last->next = ರಿಕ್ತ;

        l->last->next = last;
        l->last = last;
    }

    elt = (ವರ್ಣ *) last->elts + l->size * last->nelts;
    last->nelts++;

    ಪ್ರತ್ಯಾಗಮ elt;
}
