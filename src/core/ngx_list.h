
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_LIST_H_INCLUDED_
#define _NGX_LIST_H_INCLUDED_


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


प्रकारनिर्देश संरचना ngx_list_part_s  ngx_list_part_t;

संरचना ngx_list_part_s {
    शून्य             *elts;
    ngx_uint_t        nelts;
    ngx_list_part_t  *next;
};


प्रकारनिर्देश संरचना {
    ngx_list_part_t  *last;
    ngx_list_part_t   part;
    माप_प्रकार            size;
    ngx_uint_t        nalloc;
    ngx_pool_t       *pool;
} ngx_list_t;


ngx_list_t *ngx_list_create(ngx_pool_t *pool, ngx_uint_t n, माप_प्रकार size);

स्थिर ngx_inline ngx_int_t
ngx_list_init(ngx_list_t *list, ngx_pool_t *pool, ngx_uint_t n, माप_प्रकार size)
{
    list->part.elts = ngx_palloc(pool, n * size);
    यदि (list->part.elts == रिक्त) {
        फर्काऊ NGX_ERROR;
    }

    list->part.nelts = 0;
    list->part.next = रिक्त;
    list->last = &list->part;
    list->size = size;
    list->nalloc = n;
    list->pool = pool;

    फर्काऊ NGX_OK;
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


शून्य *ngx_list_थप(ngx_list_t *list);


#endif /* _NGX_LIST_H_INCLUDED_ */
