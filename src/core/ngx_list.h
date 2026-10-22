
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#که_نه_تعریف_شوی _NGX_LIST_H_INCLUDED_
#تعریف_کړه _NGX_LIST_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


تعريفنوع بنية ngx_list_part_s  ngx_list_part_t;

بنية ngx_list_part_s {
    فراغ             *elts;
    ngx_uint_t        nelts;
    ngx_list_part_t  *بل;
};


تعريفنوع بنية {
    ngx_list_part_t  *وروستی;
    ngx_list_part_t   part;
    اندازه_t            اندازه;
    ngx_uint_t        nalloc;
    ngx_pool_t       *pool;
} ngx_list_t;


ngx_list_t *ngx_list_create(ngx_pool_t *pool, ngx_uint_t n, اندازه_t اندازه);

ساكن ngx_inline ngx_int_t
ngx_list_init(ngx_list_t *list, ngx_pool_t *pool, ngx_uint_t n, اندازه_t اندازه)
{
    list->part.elts = ngx_palloc(pool, n * اندازه);
    إذا (list->part.elts == عدم) {
        أرجع NGX_ERROR;
    }

    list->part.nelts = 0;
    list->part.بل = عدم;
    list->وروستی = &list->part;
    list->اندازه = اندازه;
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


فراغ *ngx_list_ټېل(ngx_list_t *list);


#د_که_پای /* _NGX_LIST_H_INCLUDED_ */
