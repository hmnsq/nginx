
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


ngx_list_t *
ngx_list_create(ngx_pool_t *pool, ngx_uint_t n, ಗಾತ್ರ_t ಗಾತ್ರ_2)
{
    ngx_list_t  *list;

    list = ngx_palloc(pool, ಗಾತ್ರ(ngx_list_t));
    ಆದರೆ (list == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ಆದರೆ (ngx_list_init(list, pool, n, ಗಾತ್ರ_2) != NGX_OK) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ಹಿಂತಿರುಗಿ list;
}


ಶೂನ್ಯ *
ngx_list_ತಳ್ಳು(ngx_list_t *l)
{
    ಶೂನ್ಯ             *elt;
    ngx_list_part_t  *ಕೊನೆಯ;

    ಕೊನೆಯ = l->ಕೊನೆಯ;

    ಆದರೆ (ಕೊನೆಯ->nelts == l->nalloc) {

        /* the last part is full, allocate a new list part */

        ಕೊನೆಯ = ngx_palloc(l->pool, ಗಾತ್ರ(ngx_list_part_t));
        ಆದರೆ (ಕೊನೆಯ == ಖಾಲಿ) {
            ಹಿಂತಿರುಗಿ ಖಾಲಿ;
        }

        ಕೊನೆಯ->elts = ngx_palloc(l->pool, l->nalloc * l->ಗಾತ್ರ_2);
        ಆದರೆ (ಕೊನೆಯ->elts == ಖಾಲಿ) {
            ಹಿಂತಿರುಗಿ ಖಾಲಿ;
        }

        ಕೊನೆಯ->nelts = 0;
        ಕೊನೆಯ->ಮುಂದಿನ = ಖಾಲಿ;

        l->ಕೊನೆಯ->ಮುಂದಿನ = ಕೊನೆಯ;
        l->ಕೊನೆಯ = ಕೊನೆಯ;
    }

    elt = (ಅಕ್ಷರ *) ಕೊನೆಯ->elts + l->ಗಾತ್ರ_2 * ಕೊನೆಯ->nelts;
    ಕೊನೆಯ->nelts++;

    ಹಿಂತಿರುಗಿ elt;
}
