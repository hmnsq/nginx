
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_RBTREE_H_INCLUDED_
#define _NGX_RBTREE_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


تعريفنوع ngx_uint_t  ngx_rbtree_کونچي_t;
تعريفنوع ngx_int_t   ngx_rbtree_کونچي_int_t;


تعريفنوع بنية ngx_rbtree_نود_s  ngx_rbtree_نود_t;

بنية ngx_rbtree_نود_s {
    ngx_rbtree_کونچي_t       کونچي;
    ngx_rbtree_نود_t     *کيري;
    ngx_rbtree_نود_t     *کانن;
    ngx_rbtree_نود_t     *parent;
    u_char                 color;
    u_char                 data;
};


تعريفنوع بنية ngx_rbtree_s  ngx_rbtree_t;

تعريفنوع فراغ (*ngx_rbtree_سيسيڤ_pt) (ngx_rbtree_نود_t *اکر,
    ngx_rbtree_نود_t *نود, ngx_rbtree_نود_t *sentinel);

بنية ngx_rbtree_s {
    ngx_rbtree_نود_t     *اکر;
    ngx_rbtree_نود_t     *sentinel;
    ngx_rbtree_سيسيڤ_pt   سيسيڤ;
};


#define ngx_rbtree_init(ڤوکوق, s, i)                                           \
    ngx_rbtree_sentinel_init(s);                                              \
    (ڤوکوق)->اکر = s;                                                         \
    (ڤوکوق)->sentinel = s;                                                     \
    (ڤوکوق)->سيسيڤ = i

#define ngx_rbtree_data(نود, type, link)                                     \
    (type *) ((u_char *) (نود) - offsetof(type, link))


فراغ ngx_rbtree_سيسيڤ(ngx_rbtree_t *ڤوکوق, ngx_rbtree_نود_t *نود);
فراغ ngx_rbtree_delete(ngx_rbtree_t *ڤوکوق, ngx_rbtree_نود_t *نود);
فراغ ngx_rbtree_سيسيڤ_نيلاي(ngx_rbtree_نود_t *اکر, ngx_rbtree_نود_t *نود,
    ngx_rbtree_نود_t *sentinel);
فراغ ngx_rbtree_سيسيڤ_timer_نيلاي(ngx_rbtree_نود_t *اکر,
    ngx_rbtree_نود_t *نود, ngx_rbtree_نود_t *sentinel);
ngx_rbtree_نود_t *ngx_rbtree_next(ngx_rbtree_t *ڤوکوق,
    ngx_rbtree_نود_t *نود);


#define ngx_rbt_red(نود)               ((نود)->color = 1)
#define ngx_rbt_black(نود)             ((نود)->color = 0)
#define ngx_rbt_is_red(نود)            ((نود)->color)
#define ngx_rbt_is_black(نود)          (!ngx_rbt_is_red(نود))
#define ngx_rbt_copy_color(n1, n2)      (n1->color = n2->color)


/* a sentinel must be black */

#define ngx_rbtree_sentinel_init(نود)  ngx_rbt_black(نود)


ساكن ngx_inline ngx_rbtree_نود_t *
ngx_rbtree_min(ngx_rbtree_نود_t *نود, ngx_rbtree_نود_t *sentinel)
{
    طالما (نود->کيري != sentinel) {
        نود = نود->کيري;
    }

    أرجع نود;
}


#endif /* _NGX_RBTREE_H_INCLUDED_ */
