
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_LIST_H_INCLUDED_
#define _NGX_LIST_H_INCLUDED_


#অন্তর্ভাব <ngx_config.h>
#অন্তর্ভাব <ngx_core.h>


প্রকারলেখ কাঠামো ngx_list_part_s  ngx_list_part_t;

কাঠামো ngx_list_part_s {
    শূন্য             *elts;
    ngx_uint_t        nelts;
    ngx_list_part_t  *পরবর্তী;
};


প্রকারলেখ কাঠামো {
    ngx_list_part_t  *last;
    ngx_list_part_t   part;
    আকার_t            আকার_2;
    ngx_uint_t        nalloc;
    ngx_pool_t       *pool;
} ngx_list_t;


ngx_list_t *ngx_list_create(ngx_pool_t *pool, ngx_uint_t n, আকার_t আকার_2);

স্থির ngx_inline ngx_int_t
ngx_list_init(ngx_list_t *list, ngx_pool_t *pool, ngx_uint_t n, আকার_t আকার_2)
{
    list->part.elts = ngx_palloc(pool, n * আকার_2);
    যদি (list->part.elts == ফাঁকা) {
        ফেরত NGX_ERROR;
    }

    list->part.nelts = 0;
    list->part.পরবর্তী = ফাঁকা;
    list->last = &list->part;
    list->আকার_2 = আকার_2;
    list->nalloc = n;
    list->pool = pool;

    ফেরত NGX_OK;
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
