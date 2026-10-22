
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_LIST_H_INCLUDED_
#define _NGX_LIST_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


تعريفنوع بنية ngx_list_part_s  ngx_list_part_t;

بنية ngx_list_part_s {
    فراغ             *elts;
    ngx_uint_t        nelts;
    ngx_list_part_t  *next;
};


تعريفنوع بنية {
    ngx_list_part_t  *last;
    ngx_list_part_t   part;
    size_t            size;
    ngx_uint_t        nalloc;
    ngx_pool_t       *pool;
} ngx_list_t;


ngx_list_t *ngx_list_create(ngx_pool_t *pool, ngx_uint_t n, size_t size);

ساكن ngx_inline ngx_int_t
ngx_list_init(ngx_list_t *list, ngx_pool_t *pool, ngx_uint_t n, size_t size)
{
    list->part.elts = ngx_palloc(pool, n * size);
    إذا (list->part.elts == عدم) {
        أرجع NGX_ERROR;
    }

    list->part.nelts = 0;
    list->part.next = عدم;
    list->last = &list->part;
    list->size = size;
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


فراغ *ngx_list_ادفع(ngx_list_t *list);


#endif /* _NGX_LIST_H_INCLUDED_ */
