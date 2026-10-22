
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_LIST_H_INCLUDED_
#define _NGX_LIST_H_INCLUDED_


#അന്തര്ഭാവ <ngx_config.h>
#അന്തര്ഭാവ <ngx_core.h>


തരനിർവചനം ഘടന ngx_list_part_s  ngx_list_part_t;

ഘടന ngx_list_part_s {
    ശൂന്യം             *elts;
    ngx_uint_t        nelts;
    ngx_list_part_t  *next;
};


തരനിർവചനം ഘടന {
    ngx_list_part_t  *last;
    ngx_list_part_t   part;
    size_t            size;
    ngx_uint_t        nalloc;
    ngx_pool_t       *pool;
} ngx_list_t;


ngx_list_t *ngx_list_create(ngx_pool_t *pool, ngx_uint_t n, size_t size);

സ്ഥിതം ngx_inline ngx_int_t
ngx_list_init(ngx_list_t *list, ngx_pool_t *pool, ngx_uint_t n, size_t size)
{
    list->part.elts = ngx_palloc(pool, n * size);
    എങ്കിൽ (list->part.elts == ശൂന്യ) {
        തിരികെ NGX_ERROR;
    }

    list->part.nelts = 0;
    list->part.next = ശൂന്യ;
    list->last = &list->part;
    list->size = size;
    list->nalloc = n;
    list->pool = pool;

    തിരികെ NGX_OK;
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


ശൂന്യം *ngx_list_തള്ളുക(ngx_list_t *list);


#endif /* _NGX_LIST_H_INCLUDED_ */
