
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#جے_بنا_تعریف _NGX_LIST_H_INCLUDED_
#تعریف_کرو _NGX_LIST_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


تعريفنوع بنية ngx_list_part_s  ngx_list_part_t;

بنية ngx_list_part_s {
    فراغ             *elts;
    ngx_uint_t        nelts;
    ngx_list_part_t  *اگلا;
};


تعريفنوع بنية {
    ngx_list_part_t  *آخری;
    ngx_list_part_t   part;
    ناپ_t            ناپ;
    ngx_uint_t        nalloc;
    ngx_pool_t       *pool;
} ngx_list_t;


ngx_list_t *ngx_list_create(ngx_pool_t *pool, ngx_uint_t n, ناپ_t ناپ);

ساكن ngx_inline ngx_int_t
ngx_list_init(ngx_list_t *list, ngx_pool_t *pool, ngx_uint_t n, ناپ_t ناپ)
{
    list->part.elts = ngx_palloc(pool, n * ناپ);
    إذا (list->part.elts == عدم) {
        أرجع NGX_ERROR;
    }

    list->part.nelts = 0;
    list->part.اگلا = عدم;
    list->آخری = &list->part;
    list->ناپ = ناپ;
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


فراغ *ngx_list_ڈالو(ngx_list_t *list);


#جے_انت /* _NGX_LIST_H_INCLUDED_ */
