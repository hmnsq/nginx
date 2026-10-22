
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_RBTREE_H_INCLUDED_
#define _NGX_RBTREE_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


تعريفنوع ngx_uint_t  ngx_rbtree_کلیل_t;
تعريفنوع ngx_int_t   ngx_rbtree_کلیل_int_t;


تعريفنوع بنية ngx_rbtree_گرێ_s  ngx_rbtree_گرێ_t;

بنية ngx_rbtree_گرێ_s {
    ngx_rbtree_کلیل_t       کلیل;
    ngx_rbtree_گرێ_t     *چەپ;
    ngx_rbtree_گرێ_t     *ڕاست;
    ngx_rbtree_گرێ_t     *parent;
    u_char                 color;
    u_char                 data;
};


تعريفنوع بنية ngx_rbtree_s  ngx_rbtree_t;

تعريفنوع فراغ (*ngx_rbtree_زیادکردن_pt) (ngx_rbtree_گرێ_t *ڕەگ,
    ngx_rbtree_گرێ_t *گرێ, ngx_rbtree_گرێ_t *sentinel);

بنية ngx_rbtree_s {
    ngx_rbtree_گرێ_t     *ڕەگ;
    ngx_rbtree_گرێ_t     *sentinel;
    ngx_rbtree_زیادکردن_pt   زیادکردن;
};


#define ngx_rbtree_init(درەخت, s, i)                                           \
    ngx_rbtree_sentinel_init(s);                                              \
    (درەخت)->ڕەگ = s;                                                         \
    (درەخت)->sentinel = s;                                                     \
    (درەخت)->زیادکردن = i

#define ngx_rbtree_data(گرێ, type, link)                                     \
    (type *) ((u_char *) (گرێ) - offsetof(type, link))


فراغ ngx_rbtree_زیادکردن(ngx_rbtree_t *درەخت, ngx_rbtree_گرێ_t *گرێ);
فراغ ngx_rbtree_delete(ngx_rbtree_t *درەخت, ngx_rbtree_گرێ_t *گرێ);
فراغ ngx_rbtree_زیادکردن_بەها(ngx_rbtree_گرێ_t *ڕەگ, ngx_rbtree_گرێ_t *گرێ,
    ngx_rbtree_گرێ_t *sentinel);
فراغ ngx_rbtree_زیادکردن_timer_بەها(ngx_rbtree_گرێ_t *ڕەگ,
    ngx_rbtree_گرێ_t *گرێ, ngx_rbtree_گرێ_t *sentinel);
ngx_rbtree_گرێ_t *ngx_rbtree_next(ngx_rbtree_t *درەخت,
    ngx_rbtree_گرێ_t *گرێ);


#define ngx_rbt_red(گرێ)               ((گرێ)->color = 1)
#define ngx_rbt_black(گرێ)             ((گرێ)->color = 0)
#define ngx_rbt_is_red(گرێ)            ((گرێ)->color)
#define ngx_rbt_is_black(گرێ)          (!ngx_rbt_is_red(گرێ))
#define ngx_rbt_copy_color(n1, n2)      (n1->color = n2->color)


/* a sentinel must be black */

#define ngx_rbtree_sentinel_init(گرێ)  ngx_rbt_black(گرێ)


ساكن ngx_inline ngx_rbtree_گرێ_t *
ngx_rbtree_min(ngx_rbtree_گرێ_t *گرێ, ngx_rbtree_گرێ_t *sentinel)
{
    طالما (گرێ->چەپ != sentinel) {
        گرێ = گرێ->چەپ;
    }

    أرجع گرێ;
}


#endif /* _NGX_RBTREE_H_INCLUDED_ */
