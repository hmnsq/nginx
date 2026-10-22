
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अगर_अघोषित _NGX_LIST_H_INCLUDED_
#घोषणा _NGX_LIST_H_INCLUDED_


#समावेश <ngx_config.h>
#समावेश <ngx_core.h>


प्रकार काष्ठा ngx_list_part_s  ngx_list_part_t;

काष्ठा ngx_list_part_s {
    व्योम             *elts;
    ngx_uint_t        nelts;
    ngx_list_part_t  *अग्र;
};


प्रकार काष्ठा {
    ngx_list_part_t  *last;
    ngx_list_part_t   part;
    माप_प्रकार            आकार;
    ngx_uint_t        nalloc;
    ngx_pool_t       *pool;
} ngx_list_t;


ngx_list_t *ngx_list_create(ngx_pool_t *pool, ngx_uint_t n, माप_प्रकार आकार);

अटल ngx_inline ngx_int_t
ngx_list_init(ngx_list_t *list, ngx_pool_t *pool, ngx_uint_t n, माप_प्रकार आकार)
{
    list->part.elts = ngx_palloc(pool, n * आकार);
    अगर (list->part.elts == शून्य) {
        वापस NGX_ERROR;
    }

    list->part.nelts = 0;
    list->part.अग्र = शून्य;
    list->last = &list->part;
    list->आकार = आकार;
    list->nalloc = n;
    list->pool = pool;

    वापस NGX_OK;
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


व्योम *ngx_list_धकेलो(ngx_list_t *list);


#पूर्ण_अगर /* _NGX_LIST_H_INCLUDED_ */
