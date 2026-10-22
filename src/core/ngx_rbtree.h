
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_RBTREE_H_INCLUDED_
#define _NGX_RBTREE_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


تعريفنوع ngx_uint_t  ngx_rbtree_کلید_t;
تعريفنوع ngx_int_t   ngx_rbtree_کلید_int_t;


تعريفنوع بنية ngx_rbtree_گرہ_s  ngx_rbtree_گرہ_t;

بنية ngx_rbtree_گرہ_s {
    ngx_rbtree_کلید_t       کلید;
    ngx_rbtree_گرہ_t     *بایاں;
    ngx_rbtree_گرہ_t     *دایاں;
    ngx_rbtree_گرہ_t     *parent;
    u_char                 color;
    u_char                 data;
};


تعريفنوع بنية ngx_rbtree_s  ngx_rbtree_t;

تعريفنوع فراغ (*ngx_rbtree_داخل_کرو_pt) (ngx_rbtree_گرہ_t *جڑ,
    ngx_rbtree_گرہ_t *گرہ, ngx_rbtree_گرہ_t *sentinel);

بنية ngx_rbtree_s {
    ngx_rbtree_گرہ_t     *جڑ;
    ngx_rbtree_گرہ_t     *sentinel;
    ngx_rbtree_داخل_کرو_pt   داخل_کرو;
};


#define ngx_rbtree_init(درخت, s, i)                                           \
    ngx_rbtree_sentinel_init(s);                                              \
    (درخت)->جڑ = s;                                                         \
    (درخت)->sentinel = s;                                                     \
    (درخت)->داخل_کرو = i

#define ngx_rbtree_data(گرہ, type, link)                                     \
    (type *) ((u_char *) (گرہ) - offsetof(type, link))


فراغ ngx_rbtree_داخل_کرو(ngx_rbtree_t *درخت, ngx_rbtree_گرہ_t *گرہ);
فراغ ngx_rbtree_delete(ngx_rbtree_t *درخت, ngx_rbtree_گرہ_t *گرہ);
فراغ ngx_rbtree_داخل_کرو_قدر(ngx_rbtree_گرہ_t *جڑ, ngx_rbtree_گرہ_t *گرہ,
    ngx_rbtree_گرہ_t *sentinel);
فراغ ngx_rbtree_داخل_کرو_timer_قدر(ngx_rbtree_گرہ_t *جڑ,
    ngx_rbtree_گرہ_t *گرہ, ngx_rbtree_گرہ_t *sentinel);
ngx_rbtree_گرہ_t *ngx_rbtree_next(ngx_rbtree_t *درخت,
    ngx_rbtree_گرہ_t *گرہ);


#define ngx_rbt_red(گرہ)               ((گرہ)->color = 1)
#define ngx_rbt_black(گرہ)             ((گرہ)->color = 0)
#define ngx_rbt_is_red(گرہ)            ((گرہ)->color)
#define ngx_rbt_is_black(گرہ)          (!ngx_rbt_is_red(گرہ))
#define ngx_rbt_copy_color(n1, n2)      (n1->color = n2->color)


/* a sentinel must be black */

#define ngx_rbtree_sentinel_init(گرہ)  ngx_rbt_black(گرہ)


ساكن ngx_inline ngx_rbtree_گرہ_t *
ngx_rbtree_min(ngx_rbtree_گرہ_t *گرہ, ngx_rbtree_گرہ_t *sentinel)
{
    طالما (گرہ->بایاں != sentinel) {
        گرہ = گرہ->بایاں;
    }

    أرجع گرہ;
}


#endif /* _NGX_RBTREE_H_INCLUDED_ */
