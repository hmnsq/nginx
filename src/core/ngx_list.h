
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#جيڪڏهن_وصف_نه_ٿيل _NGX_LIST_H_INCLUDED_
#وصف_ڪريو _NGX_LIST_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


تعريفنوع بنية ngx_list_part_s  ngx_list_part_t;

بنية ngx_list_part_s {
    فراغ             *elts;
    ngx_uint_t        nelts;
    ngx_list_part_t  *ايندڙ;
};


تعريفنوع بنية {
    ngx_list_part_t  *آخري;
    ngx_list_part_t   part;
    سائيز_t            سائيز;
    ngx_uint_t        nalloc;
    ngx_pool_t       *pool;
} ngx_list_t;


ngx_list_t *ngx_list_create(ngx_pool_t *pool, ngx_uint_t n, سائيز_t سائيز);

ساكن ngx_inline ngx_int_t
ngx_list_init(ngx_list_t *list, ngx_pool_t *pool, ngx_uint_t n, سائيز_t سائيز)
{
    list->part.elts = ngx_palloc(pool, n * سائيز);
    إذا (list->part.elts == عدم) {
        أرجع NGX_ERROR;
    }

    list->part.nelts = 0;
    list->part.ايندڙ = عدم;
    list->آخري = &list->part;
    list->سائيز = سائيز;
    list->nalloc = n;
    list->pool = pool;

    أرجع NGX_OK;
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


فراغ *ngx_list_ڌڪو(ngx_list_t *list);


#جيڪڏهن_پورو /* _NGX_LIST_H_INCLUDED_ */
