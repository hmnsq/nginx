
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_LIST_H_INCLUDED_
#define _NGX_LIST_H_INCLUDED_


#ଅନ୍ତର୍ଭାଵ <ngx_config.h>
#ଅନ୍ତର୍ଭାଵ <ngx_core.h>


ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା ngx_list_part_s  ngx_list_part_t;

ସଂରଚନା ngx_list_part_s {
    ଶୂନ୍ୟ             *elts;
    ngx_uint_t        nelts;
    ngx_list_part_t  *next;
};


ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ngx_list_part_t  *last;
    ngx_list_part_t   part;
    size_t            size;
    ngx_uint_t        nalloc;
    ngx_pool_t       *pool;
} ngx_list_t;


ngx_list_t *ngx_list_create(ngx_pool_t *pool, ngx_uint_t n, size_t size);

ସ୍ଥିର ngx_inline ngx_int_t
ngx_list_init(ngx_list_t *list, ngx_pool_t *pool, ngx_uint_t n, size_t size)
{
    list->part.elts = ngx_palloc(pool, n * size);
    ଯଦି (list->part.elts == ଖାଲି) {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    list->part.nelts = 0;
    list->part.next = ଖାଲି;
    list->last = &list->part;
    list->size = size;
    list->nalloc = n;
    list->pool = pool;

    ଫେରନ୍ତୁ NGX_OK;
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


ଶୂନ୍ୟ *ngx_list_ଠେଲ(ngx_list_t *list);


#endif /* _NGX_LIST_H_INCLUDED_ */
