
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ _NGX_LIST_H_INCLUDED_
#ವ್ಯಾಖ್ಯಾನಿಸು _NGX_LIST_H_INCLUDED_


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ ngx_list_part_s  ngx_list_part_t;

ರಚನೆ ngx_list_part_s {
    ಶೂನ್ಯ             *elts;
    ngx_uint_t        nelts;
    ngx_list_part_t  *ಮುಂದಿನ;
};


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ngx_list_part_t  *ಕೊನೆಯ;
    ngx_list_part_t   part;
    ಗಾತ್ರ_t            ಗಾತ್ರ_2;
    ngx_uint_t        nalloc;
    ngx_pool_t       *pool;
} ngx_list_t;


ngx_list_t *ngx_list_create(ngx_pool_t *pool, ngx_uint_t n, ಗಾತ್ರ_t ಗಾತ್ರ_2);

ಸ್ಥಿರ ngx_inline ngx_int_t
ngx_list_init(ngx_list_t *list, ngx_pool_t *pool, ngx_uint_t n, ಗಾತ್ರ_t ಗಾತ್ರ_2)
{
    list->part.elts = ngx_palloc(pool, n * ಗಾತ್ರ_2);
    ಆದರೆ (list->part.elts == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    list->part.nelts = 0;
    list->part.ಮುಂದಿನ = ಖಾಲಿ;
    list->ಕೊನೆಯ = &list->part;
    list->ಗಾತ್ರ_2 = ಗಾತ್ರ_2;
    list->nalloc = n;
    list->pool = pool;

    ಹಿಂತಿರುಗಿ NGX_OK;
}


/*
 *
 *  the iteration through the list:
 *
 *  part = &list.part;
 *  data = part->elts;
 *
 *  for (i = 0 ;; i++) {
 *
 *      if (i >= part->nelts) {
 *          if (part->next == NULL) {
 *              break;
 *          }
 *
 *          part = part->next;
 *          data = part->elts;
 *          i = 0;
 *      }
 *
 *      ...  data[i] ...
 *
 *  }
 */


ಶೂನ್ಯ *ngx_list_ತಳ್ಳು(ngx_list_t *list);


#ಆದರೆ_ಅಂತ್ಯ /* _NGX_LIST_H_INCLUDED_ */
