
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_RBTREE_H_INCLUDED_
#define _NGX_RBTREE_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


تعريفنوع ngx_uint_t  ngx_rbtree_مبودي_t;
تعريفنوع ngx_int_t   ngx_rbtree_مبودي_int_t;


تعريفنوع بنية ngx_rbtree_كلي_s  ngx_rbtree_كلي_t;

بنية ngx_rbtree_كلي_s {
    ngx_rbtree_مبودي_t       مبودي;
    ngx_rbtree_كلي_t     *هغو;
    ngx_rbtree_كلي_t     *دام;
    ngx_rbtree_كلي_t     *parent;
    u_char                 color;
    u_char                 data;
};


تعريفنوع بنية ngx_rbtree_s  ngx_rbtree_t;

تعريفنوع فراغ (*ngx_rbtree_سك_pt) (ngx_rbtree_كلي_t *سيو,
    ngx_rbtree_كلي_t *كلي, ngx_rbtree_كلي_t *sentinel);

بنية ngx_rbtree_s {
    ngx_rbtree_كلي_t     *سيو;
    ngx_rbtree_كلي_t     *sentinel;
    ngx_rbtree_سك_pt   سك;
};


#define ngx_rbtree_init(بشيا, s, i)                                           \
    ngx_rbtree_sentinel_init(s);                                              \
    (بشيا)->سيو = s;                                                         \
    (بشيا)->sentinel = s;                                                     \
    (بشيا)->سك = i

#define ngx_rbtree_data(كلي, type, link)                                     \
    (type *) ((u_char *) (كلي) - offsetof(type, link))


فراغ ngx_rbtree_سك(ngx_rbtree_t *بشيا, ngx_rbtree_كلي_t *كلي);
فراغ ngx_rbtree_delete(ngx_rbtree_t *بشيا, ngx_rbtree_كلي_t *كلي);
فراغ ngx_rbtree_سك_درج(ngx_rbtree_كلي_t *سيو, ngx_rbtree_كلي_t *كلي,
    ngx_rbtree_كلي_t *sentinel);
فراغ ngx_rbtree_سك_timer_درج(ngx_rbtree_كلي_t *سيو,
    ngx_rbtree_كلي_t *كلي, ngx_rbtree_كلي_t *sentinel);
ngx_rbtree_كلي_t *ngx_rbtree_next(ngx_rbtree_t *بشيا,
    ngx_rbtree_كلي_t *كلي);


#define ngx_rbt_red(كلي)               ((كلي)->color = 1)
#define ngx_rbt_black(كلي)             ((كلي)->color = 0)
#define ngx_rbt_is_red(كلي)            ((كلي)->color)
#define ngx_rbt_is_black(كلي)          (!ngx_rbt_is_red(كلي))
#define ngx_rbt_copy_color(n1, n2)      (n1->color = n2->color)


/* a sentinel must be black */

#define ngx_rbtree_sentinel_init(كلي)  ngx_rbt_black(كلي)


ساكن ngx_inline ngx_rbtree_كلي_t *
ngx_rbtree_min(ngx_rbtree_كلي_t *كلي, ngx_rbtree_كلي_t *sentinel)
{
    طالما (كلي->هغو != sentinel) {
        كلي = كلي->هغو;
    }

    أرجع كلي;
}


#endif /* _NGX_RBTREE_H_INCLUDED_ */
