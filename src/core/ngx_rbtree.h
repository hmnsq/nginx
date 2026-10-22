
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_RBTREE_H_INCLUDED_
#define _NGX_RBTREE_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


تعريفنوع ngx_uint_t  ngx_rbtree_مفتاح_t;
تعريفنوع ngx_int_t   ngx_rbtree_مفتاح_int_t;


تعريفنوع بنية ngx_rbtree_عقدة_s  ngx_rbtree_عقدة_t;

بنية ngx_rbtree_عقدة_s {
    ngx_rbtree_مفتاح_t       مفتاح;
    ngx_rbtree_عقدة_t     *يسار;
    ngx_rbtree_عقدة_t     *يمين;
    ngx_rbtree_عقدة_t     *parent;
    u_char                 color;
    u_char                 data;
};


تعريفنوع بنية ngx_rbtree_s  ngx_rbtree_t;

تعريفنوع فراغ (*ngx_rbtree_أدرج_pt) (ngx_rbtree_عقدة_t *جذر,
    ngx_rbtree_عقدة_t *عقدة, ngx_rbtree_عقدة_t *sentinel);

بنية ngx_rbtree_s {
    ngx_rbtree_عقدة_t     *جذر;
    ngx_rbtree_عقدة_t     *sentinel;
    ngx_rbtree_أدرج_pt   أدرج;
};


#define ngx_rbtree_init(شجرة, s, i)                                           \
    ngx_rbtree_sentinel_init(s);                                              \
    (شجرة)->جذر = s;                                                         \
    (شجرة)->sentinel = s;                                                     \
    (شجرة)->أدرج = i

#define ngx_rbtree_data(عقدة, type, link)                                     \
    (type *) ((u_char *) (عقدة) - offsetof(type, link))


فراغ ngx_rbtree_أدرج(ngx_rbtree_t *شجرة, ngx_rbtree_عقدة_t *عقدة);
فراغ ngx_rbtree_delete(ngx_rbtree_t *شجرة, ngx_rbtree_عقدة_t *عقدة);
فراغ ngx_rbtree_أدرج_قيمة(ngx_rbtree_عقدة_t *جذر, ngx_rbtree_عقدة_t *عقدة,
    ngx_rbtree_عقدة_t *sentinel);
فراغ ngx_rbtree_أدرج_timer_قيمة(ngx_rbtree_عقدة_t *جذر,
    ngx_rbtree_عقدة_t *عقدة, ngx_rbtree_عقدة_t *sentinel);
ngx_rbtree_عقدة_t *ngx_rbtree_next(ngx_rbtree_t *شجرة,
    ngx_rbtree_عقدة_t *عقدة);


#define ngx_rbt_red(عقدة)               ((عقدة)->color = 1)
#define ngx_rbt_black(عقدة)             ((عقدة)->color = 0)
#define ngx_rbt_is_red(عقدة)            ((عقدة)->color)
#define ngx_rbt_is_black(عقدة)          (!ngx_rbt_is_red(عقدة))
#define ngx_rbt_copy_color(n1, n2)      (n1->color = n2->color)


/* a sentinel must be black */

#define ngx_rbtree_sentinel_init(عقدة)  ngx_rbt_black(عقدة)


ساكن ngx_inline ngx_rbtree_عقدة_t *
ngx_rbtree_min(ngx_rbtree_عقدة_t *عقدة, ngx_rbtree_عقدة_t *sentinel)
{
    طالما (عقدة->يسار != sentinel) {
        عقدة = عقدة->يسار;
    }

    أرجع عقدة;
}


#endif /* _NGX_RBTREE_H_INCLUDED_ */
