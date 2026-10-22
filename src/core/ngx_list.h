
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#यदि_अपरिभाषितम् _NGX_LIST_H_INCLUDED_
#परिभाषय _NGX_LIST_H_INCLUDED_


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


प्रकारनिर्देश संरचना ngx_list_part_s  ngx_list_part_t;

संरचना ngx_list_part_s {
    शून्य             *elts;
    ngx_uint_t        nelts;
    ngx_list_part_t  *अग्र;
};


प्रकारनिर्देश संरचना {
    ngx_list_part_t  *चरमः;
    ngx_list_part_t   part;
    आकार_t            आकार;
    ngx_uint_t        nalloc;
    ngx_pool_t       *pool;
} ngx_list_t;


ngx_list_t *ngx_list_create(ngx_pool_t *pool, ngx_uint_t n, आकार_t आकार);

स्थिर ngx_inline ngx_int_t
ngx_list_init(ngx_list_t *list, ngx_pool_t *pool, ngx_uint_t n, आकार_t आकार)
{
    list->part.elts = ngx_palloc(pool, n * आकार);
    यदि (list->part.elts == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

    list->part.nelts = 0;
    list->part.अग्र = रिक्त;
    list->चरमः = &list->part;
    list->आकार = आकार;
    list->nalloc = n;
    list->pool = pool;

    प्रत्यागम NGX_OK;
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


शून्य *ngx_list_प्रक्षिप(ngx_list_t *list);


#यदि_अन्तः /* _NGX_LIST_H_INCLUDED_ */
