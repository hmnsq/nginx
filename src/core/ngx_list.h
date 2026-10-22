
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#నిర్వచితం_కాకపోతే _NGX_LIST_H_INCLUDED_
#నిర్వచించు _NGX_LIST_H_INCLUDED_


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>


రకనిర్వచనం నిర్మాణం ngx_list_part_s  ngx_list_part_t;

నిర్మాణం ngx_list_part_s {
    శూన్య             *elts;
    ngx_uint_t        nelts;
    ngx_list_part_t  *తదుపరి;
};


రకనిర్వచనం నిర్మాణం {
    ngx_list_part_t  *చివరి;
    ngx_list_part_t   part;
    కొలత_t            కొలత;
    ngx_uint_t        nalloc;
    ngx_pool_t       *pool;
} ngx_list_t;


ngx_list_t *ngx_list_create(ngx_pool_t *pool, ngx_uint_t n, కొలత_t కొలత);

స్థిర ngx_inline ngx_int_t
ngx_list_init(ngx_list_t *list, ngx_pool_t *pool, ngx_uint_t n, కొలత_t కొలత)
{
    list->part.elts = ngx_palloc(pool, n * కొలత);
    ఒకవేళ (list->part.elts == ఖాళీ) {
        తిరిగి NGX_ERROR;
    }

    list->part.nelts = 0;
    list->part.తదుపరి = ఖాళీ;
    list->చివరి = &list->part;
    list->కొలత = కొలత;
    list->nalloc = n;
    list->pool = pool;

    తిరిగి NGX_OK;
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


శూన్య *ngx_list_నెట్టు(ngx_list_t *list);


#అయితే_ముగింపు /* _NGX_LIST_H_INCLUDED_ */
