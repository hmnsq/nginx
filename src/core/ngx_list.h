
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#اگر_غیر_تعریف_شدہ _NGX_LIST_H_INCLUDED_
#تعریف_کرو _NGX_LIST_H_INCLUDED_


#شامل <ngx_config.h>
#شامل <ngx_core.h>


قسمتعریف ساخت ngx_list_part_s  ngx_list_part_t;

ساخت ngx_list_part_s {
    خالی             *elts;
    ngx_uint_t        nelts;
    ngx_list_part_t  *اگلا;
};


قسمتعریف ساخت {
    ngx_list_part_t  *آخری;
    ngx_list_part_t   part;
    سائز_t            سائز;
    ngx_uint_t        nalloc;
    ngx_pool_t       *pool;
} ngx_list_t;


ngx_list_t *ngx_list_create(ngx_pool_t *pool, ngx_uint_t n, سائز_t سائز);

ساکن ngx_inline ngx_int_t
ngx_list_init(ngx_list_t *list, ngx_pool_t *pool, ngx_uint_t n, سائز_t سائز)
{
    list->part.elts = ngx_palloc(pool, n * سائز);
    اگر (list->part.elts == تہی) {
        واپس NGX_ERROR;
    }

    list->part.nelts = 0;
    list->part.اگلا = تہی;
    list->آخری = &list->part;
    list->سائز = سائز;
    list->nalloc = n;
    list->pool = pool;

    واپس NGX_OK;
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


خالی *ngx_list_ڈالو(ngx_list_t *list);


#اگر_ختم /* _NGX_LIST_H_INCLUDED_ */
