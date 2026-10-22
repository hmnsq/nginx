
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_LIST_H_INCLUDED_
#define _NGX_LIST_H_INCLUDED_


#অন্তর্ভাৱ <ngx_config.h>
#অন্তর্ভাৱ <ngx_core.h>


প্রকারনির্দেশ সংরচনা ngx_list_part_s  ngx_list_part_t;

সংরচনা ngx_list_part_s {
    শূন্য             *elts;
    ngx_uint_t        nelts;
    ngx_list_part_t  *next;
};


প্রকারনির্দেশ সংরচনা {
    ngx_list_part_t  *last;
    ngx_list_part_t   part;
    size_t            size;
    ngx_uint_t        nalloc;
    ngx_pool_t       *pool;
} ngx_list_t;


ngx_list_t *ngx_list_create(ngx_pool_t *pool, ngx_uint_t n, size_t size);

স্থির ngx_inline ngx_int_t
ngx_list_init(ngx_list_t *list, ngx_pool_t *pool, ngx_uint_t n, size_t size)
{
    list->part.elts = ngx_palloc(pool, n * size);
    যদি (list->part.elts == রিক্ত) {
        ঘূৰাই NGX_ERROR;
    }

    list->part.nelts = 0;
    list->part.next = রিক্ত;
    list->last = &list->part;
    list->size = size;
    list->nalloc = n;
    list->pool = pool;

    ঘূৰাই NGX_OK;
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


শূন্য *ngx_list_ঠেলো(ngx_list_t *list);


#endif /* _NGX_LIST_H_INCLUDED_ */
