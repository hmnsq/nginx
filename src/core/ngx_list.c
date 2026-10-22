
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>


ngx_list_t *
ngx_list_create(ngx_pool_t *pool, ngx_uint_t n, ਮਾਪ_t ਮਾਪ)
{
    ngx_list_t  *list;

    list = ngx_palloc(pool, ਆਕਾਰ(ngx_list_t));
    ਜੇ (list == ਨਿਰਾ) {
        ਵਾਪਸ ਨਿਰਾ;
    }

    ਜੇ (ngx_list_init(list, pool, n, ਮਾਪ) != NGX_OK) {
        ਵਾਪਸ ਨਿਰਾ;
    }

    ਵਾਪਸ list;
}


ਖਾਲੀ *
ngx_list_ਧੱਕੋ(ngx_list_t *l)
{
    ਖਾਲੀ             *elt;
    ngx_list_part_t  *ਆਖਰੀ;

    ਆਖਰੀ = l->ਆਖਰੀ;

    ਜੇ (ਆਖਰੀ->nelts == l->nalloc) {

        /* the last part is full, allocate a new list part */

        ਆਖਰੀ = ngx_palloc(l->pool, ਆਕਾਰ(ngx_list_part_t));
        ਜੇ (ਆਖਰੀ == ਨਿਰਾ) {
            ਵਾਪਸ ਨਿਰਾ;
        }

        ਆਖਰੀ->elts = ngx_palloc(l->pool, l->nalloc * l->ਮਾਪ);
        ਜੇ (ਆਖਰੀ->elts == ਨਿਰਾ) {
            ਵਾਪਸ ਨਿਰਾ;
        }

        ਆਖਰੀ->nelts = 0;
        ਆਖਰੀ->ਅਗਲਾ = ਨਿਰਾ;

        l->ਆਖਰੀ->ਅਗਲਾ = ਆਖਰੀ;
        l->ਆਖਰੀ = ਆਖਰੀ;
    }

    elt = (ਅੱਖਰ *) ਆਖਰੀ->elts + l->ਮਾਪ * ਆਖਰੀ->nelts;
    ਆਖਰੀ->nelts++;

    ਵਾਪਸ elt;
}
