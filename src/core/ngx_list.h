
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ _NGX_LIST_H_INCLUDED_
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _NGX_LIST_H_INCLUDED_


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ngx_list_part_s  ngx_list_part_t;

ਬਣਤਰ ngx_list_part_s {
    ਖਾਲੀ             *elts;
    ngx_uint_t        nelts;
    ngx_list_part_t  *ਅਗਲਾ;
};


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ngx_list_part_t  *ਆਖਰੀ;
    ngx_list_part_t   part;
    ਮਾਪ_t            ਮਾਪ;
    ngx_uint_t        nalloc;
    ngx_pool_t       *pool;
} ngx_list_t;


ngx_list_t *ngx_list_create(ngx_pool_t *pool, ngx_uint_t n, ਮਾਪ_t ਮਾਪ);

ਸਥਿਰ ngx_inline ngx_int_t
ngx_list_init(ngx_list_t *list, ngx_pool_t *pool, ngx_uint_t n, ਮਾਪ_t ਮਾਪ)
{
    list->part.elts = ngx_palloc(pool, n * ਮਾਪ);
    ਜੇ (list->part.elts == ਨਿਰਾ) {
        ਵਾਪਸ NGX_ERROR;
    }

    list->part.nelts = 0;
    list->part.ਅਗਲਾ = ਨਿਰਾ;
    list->ਆਖਰੀ = &list->part;
    list->ਮਾਪ = ਮਾਪ;
    list->nalloc = n;
    list->pool = pool;

    ਵਾਪਸ NGX_OK;
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


ਖਾਲੀ *ngx_list_ਧੱਕੋ(ngx_list_t *list);


#ਜੇ_ਅੰਤ /* _NGX_LIST_H_INCLUDED_ */
